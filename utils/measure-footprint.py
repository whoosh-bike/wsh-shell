#!/usr/bin/env python3
"""Measure the flash/RAM footprint of wsh-shell for different feature sets.

Builds the library once per configuration against a synthetic firmware stub,
links it for a real MCU target and reports how many bytes belong to the shell's
own object files (libc, startup code and the stub itself are excluded, so the
numbers stay comparable between configurations and between machines).

    # cumulative ladder, README-style table
    python3 utils/measure-footprint.py --markdown

    # what each feature costs on top of the "everything disabled" baseline
    python3 utils/measure-footprint.py --mode each --markdown

    # a single, explicit configuration
    python3 utils/measure-footprint.py --mode single --enable WSH_SHELL_HISTORY

Requires an arm-none-eabi toolchain (override with --cc/--size-prefix for a
different target).
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
SRC_DIR = REPO_ROOT / "src"
CFG_TEMPLATE = SRC_DIR / "wsh_shell_cfg_def.h"

# Every boolean knob the config template exposes. Order matters only for --mode each.
FEATURE_FLAGS = [
    "WSH_SHELL_PRINT_SYS_ENABLE",
    "WSH_SHELL_PRINT_INFO_ENABLE",
    "WSH_SHELL_PRINT_WARN_ENABLE",
    "WSH_SHELL_PRINT_ERR_ENABLE",
    "WSH_SHELL_INTERACTIVE_MODE",
    "WSH_SHELL_HISTORY",
    "WSH_SHELL_AUTOCOMPLETE",
    "WSH_SHELL_PS1_CUSTOM",
    "WSH_SHELL_PROMPT_WAIT",
    "WSH_SHELL_DEF_COMMAND",
    "WSH_SHELL_SESSION",
    "WSH_SHELL_SUBCOMMANDS",
    "WSH_SHELL_PRINT_OPT_HELP_ENABLE",
    "WSH_SHELL_CMD_PRINT_OPT_OVERVIEW",
]

# Cumulative ladder: each step adds its flags to everything above it. Mirrors the
# footprint table in README.md / docs/index.md.
LADDER = [
    ("All features disabled", []),
    (
        "+WSH_SHELL_PRINT_SYS/INFO/WARN/ERR",
        [
            "WSH_SHELL_PRINT_SYS_ENABLE",
            "WSH_SHELL_PRINT_INFO_ENABLE",
            "WSH_SHELL_PRINT_WARN_ENABLE",
            "WSH_SHELL_PRINT_ERR_ENABLE",
        ],
    ),
    ("+WSH_SHELL_INTERACTIVE_MODE", ["WSH_SHELL_INTERACTIVE_MODE"]),
    ("+WSH_SHELL_HISTORY", ["WSH_SHELL_HISTORY"]),
    ("+WSH_SHELL_AUTOCOMPLETE", ["WSH_SHELL_AUTOCOMPLETE"]),
    ("+WSH_SHELL_PS1_CUSTOM", ["WSH_SHELL_PS1_CUSTOM"]),
    ("+WSH_SHELL_PROMPT_WAIT", ["WSH_SHELL_PROMPT_WAIT"]),
    ("+WSH_SHELL_DEF_COMMAND", ["WSH_SHELL_DEF_COMMAND"]),
    ("+WSH_SHELL_SESSION", ["WSH_SHELL_SESSION"]),
    ("+WSH_SHELL_PRINT_OPT_HELP", ["WSH_SHELL_PRINT_OPT_HELP_ENABLE"]),
    ("+WSH_SHELL_CMD_PRINT_OPT_OVERVIEW", ["WSH_SHELL_CMD_PRINT_OPT_OVERVIEW"]),
    ("+WSH_SHELL_SUBCOMMANDS", ["WSH_SHELL_SUBCOMMANDS"]),
]

# Touches every public entry point so the linker cannot garbage-collect a feature
# that a real integration would keep alive.
STUB_SOURCE = r"""
#include "wsh_shell.h"

static WshShell_t Shell;

/* Named symbol so the measured sizeof(WshShell_t) can be read back with nm. */
char WshShell_SizeProbe[sizeof(WshShell_t)];

static const WshShellUser_t Users[1];
static const WshShellCmd_t* Cmds[1];

static WshShellHistory_t HistStorage;
static WshShellHistory_t HistRead(void) { return HistStorage; }
static void HistWrite(WshShellHistory_t h) { HistStorage = h; }

static WshShellSession_t SessStorage;
static WshShellSession_t SessRead(void) { return SessStorage; }
static void SessWrite(WshShellSession_t s) { SessStorage = s; }

int main(void) {
    WshShell_Init(&Shell, "dev", 0, 0);
    WshShellUser_Attach(&Shell.Users, Users, 1, 0);
    WshShellCmd_Attach(&Shell.Commands, Cmds, 1);
    WshShellHistory_Init(&Shell.HistoryIO, HistRead, HistWrite);
    WshShellSession_Init(&Shell.SessionIO, SessRead, SessWrite);
    WshShell_Auth(&Shell, "user", "pass");
    WshShell_SessionArm(&Shell, 1);
    WshShell_SessionRestore(&Shell);
    WshShell_SessionIsKeepActive(&Shell);
    WshShell_DeAuth(&Shell, "stub");

    for (;;)
        WshShell_InsertChar(&Shell, (char)WshShell_SizeProbe[0]);
}
"""

# Flash-resident output sections; everything else is RAM.
FLASH_SECTIONS = (".text", ".rodata", ".glue", ".ARM.extab", ".ARM.exidx", ".init", ".fini")


@dataclass
class Measurement:
    name: str
    flash: int
    ram_static: int
    struct_size: int
    flags: list[str] = field(default_factory=list)


def render_config(enabled: set[str]) -> str:
    """Return the default config template with every feature flag forced on/off."""
    text = CFG_TEMPLATE.read_text()
    for flag in FEATURE_FLAGS:
        value = "1" if flag in enabled else "0"
        text, subs = re.subn(
            rf"^(#define\s+{flag}\s+)\d+", rf"\g<1>{value}", text, count=1, flags=re.MULTILINE
        )
        if subs == 0:
            raise SystemExit(f"Flag {flag} not found in {CFG_TEMPLATE} — update FEATURE_FLAGS")
    return text


def parse_map(map_path: Path, objects: set[str]) -> tuple[int, int]:
    """Sum the input-section sizes the linker placed from *objects*.

    Returns (flash, ram_static). ld wraps long section names onto the next line,
    so a bare section name is joined with the line that follows it.
    """
    lines = map_path.read_text(errors="replace").split("\n")
    entry_re = re.compile(r"^\s*(\.[\w.$]+)\s+0x[0-9a-f]+\s+0x([0-9a-f]+)\s+(\S+)")

    flash = ram = 0
    idx = 0
    while idx < len(lines):
        line = lines[idx]
        if re.match(r"^ (\.[\w.$]+)\s*$", line) and idx + 1 < len(lines):
            line = line.rstrip() + " " + lines[idx + 1].strip()
            idx += 1

        match = entry_re.match(line)
        if match:
            section, size, obj = match.group(1), int(match.group(2), 16), match.group(3)
            if Path(obj).name in objects:
                if section.startswith(FLASH_SECTIONS):
                    flash += size
                else:
                    ram += size
        idx += 1

    return flash, ram


def probe_struct_size(nm: str, elf: Path) -> int:
    out = subprocess.run(
        [nm, "--print-size", "--radix=d", str(elf)], capture_output=True, text=True, check=True
    ).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[3] == "WshShell_SizeProbe":
            return int(parts[1])
    return 0


def measure(name: str, flags: list[str], args: argparse.Namespace) -> Measurement:
    enabled = set(flags)

    with tempfile.TemporaryDirectory(prefix="wsh-footprint-") as tmp:
        tmp_dir = Path(tmp)
        (tmp_dir / "wsh_shell_cfg.h").write_text(render_config(enabled))
        stub = tmp_dir / "footprint_stub.c"
        stub.write_text(STUB_SOURCE)

        common = [
            args.cc,
            f"-mcpu={args.cpu}",
            "-mthumb",
            args.opt,
            "-ffunction-sections",
            "-fdata-sections",
            f"-I{tmp_dir}",
            f"-I{SRC_DIR}",
        ]

        shell_objects: set[str] = set()
        obj_files: list[str] = []
        for source in sorted(SRC_DIR.glob("*.c")) + [stub]:
            obj = tmp_dir / (source.stem + ".o")
            run(common + ["-c", str(source), "-o", str(obj)], args)
            obj_files.append(str(obj))
            # The stub stands in for integrator code — never counted as shell cost.
            if source != stub:
                shell_objects.add(obj.name)

        elf = tmp_dir / "footprint.elf"
        map_file = tmp_dir / "footprint.map"
        run(
            [args.cc, f"-mcpu={args.cpu}", "-mthumb", args.opt]
            + obj_files
            + [
                "-o",
                str(elf),
                "-specs=nano.specs",
                "-specs=nosys.specs",
                "-Wl,--gc-sections",
                f"-Wl,-Map={map_file}",
            ],
            args,
        )

        flash, ram = parse_map(map_file, shell_objects)
        struct_size = probe_struct_size(args.nm, elf)

    return Measurement(
        name=name, flash=flash, ram_static=ram, struct_size=struct_size, flags=sorted(enabled)
    )


def run(cmd: list[str], args: argparse.Namespace) -> None:
    proc = subprocess.run(cmd, capture_output=True, text=True)
    if proc.returncode != 0:
        sys.stderr.write(proc.stdout + proc.stderr)
        raise SystemExit(f"command failed: {' '.join(cmd)}")
    if args.verbose and proc.stderr.strip():
        sys.stderr.write(proc.stderr)


def print_markdown(results: list[Measurement], baseline: Measurement | None) -> None:
    if baseline is None:
        print("| Config | FLASH, KB | ΔFLASH, KB | RAM, KB | sizeof(WshShell_t), B |")
        print("| ------ | --------- | ---------- | ------- | --------------------- |")
        prev = None
        for r in results:
            # Cumulative ladder: the delta is what this step's flags added.
            delta = "—" if prev is None else f"{(r.flash - prev) / 1024:+.5f}"
            print(
                f"| {r.name} | {r.flash / 1024:.5f} | {delta} "
                f"| {r.ram_static / 1024:.5f} | {r.struct_size} |"
            )
            prev = r.flash
        return

    print("| Feature | FLASH, KB | ΔFLASH, KB | RAM, KB | ΔRAM, KB |")
    print("| ------- | --------- | ---------- | ------- | -------- |")
    for r in results:
        print(
            f"| `{r.name}` | {r.flash / 1024:.5f} | {(r.flash - baseline.flash) / 1024:+.5f} "
            f"| {r.ram_static / 1024:.5f} | {(r.ram_static - baseline.ram_static) / 1024:+.5f} |"
        )


def print_table(results: list[Measurement]) -> None:
    width = max(len(r.name) for r in results)
    prev = None
    for r in results:
        delta = "" if prev is None else f"  ({(r.flash - prev) / 1024:+8.5f} KB)"
        print(
            f"{r.name:<{width}}  {r.flash / 1024:8.5f} KB  "
            f"RAM {r.ram_static / 1024:7.5f} KB  sizeof(WshShell_t) {r.struct_size:4d} B{delta}"
        )
        prev = r.flash


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument(
        "--cc", default="arm-none-eabi-gcc", help="compiler (default: arm-none-eabi-gcc)"
    )
    parser.add_argument("--nm", default="arm-none-eabi-nm", help="nm binary matching --cc")
    parser.add_argument("--cpu", default="cortex-m7", help="target CPU (default: cortex-m7)")
    parser.add_argument("--opt", default="-O1", help="optimization level (default: -O1)")
    parser.add_argument(
        "--mode",
        choices=("ladder", "each", "drop", "single"),
        default="ladder",
        help="ladder: cumulative table; each: cost of one feature on the bare baseline; "
        "drop: what disabling one feature saves in a full build; single: one explicit config",
    )
    parser.add_argument(
        "--enable",
        action="append",
        default=[],
        metavar="FLAG",
        help="mode=single: flag to enable (repeatable)",
    )
    parser.add_argument("--markdown", action="store_true", help="print a Markdown table")
    parser.add_argument("--json", action="store_true", help="print raw JSON")
    parser.add_argument("--verbose", action="store_true", help="forward compiler warnings")
    args = parser.parse_args()

    if shutil.which(args.cc) is None:
        raise SystemExit(f"{args.cc} not found in PATH — install the toolchain or pass --cc")

    results: list[Measurement] = []
    baseline: Measurement | None = None

    if args.mode == "ladder":
        acc: list[str] = []
        for name, flags in LADDER:
            acc += flags
            results.append(measure(name, list(acc), args))
    elif args.mode == "each":
        baseline = measure("baseline (all disabled)", [], args)
        results.append(baseline)
        for flag in FEATURE_FLAGS:
            results.append(measure(flag, [flag], args))
    elif args.mode == "drop":
        # A feature's real cost depends on what else is on (e.g. subcommands only
        # pull their weight once the default command exists), so measure the
        # saving against a full build rather than a bare one.
        baseline = measure("baseline (all enabled)", list(FEATURE_FLAGS), args)
        results.append(baseline)
        for flag in FEATURE_FLAGS:
            results.append(measure(f"-{flag}", [f for f in FEATURE_FLAGS if f != flag], args))
    else:
        unknown = [f for f in args.enable if f not in FEATURE_FLAGS]
        if unknown:
            raise SystemExit(f"unknown flag(s): {', '.join(unknown)}")
        results.append(measure("custom", list(args.enable), args))

    if args.json:
        print(json.dumps([r.__dict__ for r in results], indent=2))
    elif args.markdown:
        print(f"- Build options: {args.cpu}, `{args.opt}` optimization, {args.cc}")
        if results:
            print(f"- sizeof(WshShell_t) = {results[-1].struct_size} bytes (full config)\n")
        print_markdown(results[1:] if baseline else results, baseline)
    else:
        print_table(results)

    return 0


if __name__ == "__main__":
    sys.exit(main())
