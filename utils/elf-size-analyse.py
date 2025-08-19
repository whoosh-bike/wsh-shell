#!/usr/bin/env python3
import argparse
import os
import subprocess

from dotenv import load_dotenv
from tabulate import tabulate

# source .venv/bin/activate
# python3 ./utils/elf-size-analyse.py ./example/blue_pill/build/blue_pill.elf

# Define symbol types for flash vs RAM
FLASH_TYPES = {"T", "t", "r", "R"}  # code + read-only const
RAM_TYPES = {"D", "d", "B", "b"}  # initialized/uninitialized data in RAM


def parse_nm_line(line):
    """Parse a single line of nm output: address, size, type, name"""
    parts = line.strip().split()
    if len(parts) < 4:
        return None
    addr, size_hex, sym_type, name = parts[0], parts[1], parts[2], parts[3]
    size = int(size_hex, 16)
    return addr, size, sym_type, name


def run_nm(nm_path, elf_file):
    """Run nm on the ELF file and return the output lines"""
    cmd = [nm_path, "--print-size", "--size-sort", "-S", elf_file]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"nm failed: {result.stderr}")
    return result.stdout.splitlines()


def main():
    # Load environment variables from .env
    load_dotenv()

    parser = argparse.ArgumentParser(description="Analyze ELF size using arm-none-eabi-nm")
    parser.add_argument("elf", help="Path to ELF file to analyze")
    parser.add_argument(
        "--nm",
        default=os.getenv("NM_PATH", "arm-none-eabi-nm"),
        help="Path to nm tool (default: from NM_PATH in .env)",
    )
    args = parser.parse_args()

    # Run nm and parse output
    lines = run_nm(args.nm, args.elf)

    flash_total = 0
    ram_total = 0
    symbols = []

    for line in lines:
        parsed = parse_nm_line(line)
        if not parsed:
            continue
        addr, size, sym_type, name = parsed

        # Only keep symbols with WshShell in their name
        if not any(x in name for x in ["WshShell", "WSH_SHELL"]):
            continue

        if size == 0:
            continue

        if sym_type in FLASH_TYPES:
            section = "FLASH"
            flash_total += size
        elif sym_type in RAM_TYPES:
            section = "RAM"
            ram_total += size
        else:
            section = "OTHER"

        symbols.append((name, size, sym_type, f"0x{addr}", section))

    # Build table using tabulate
    table = sorted(symbols, key=lambda x: -x[1])
    table.append(("TOTAL FLASH", flash_total / 1024, "kBytes", "-", "FLASH"))
    table.append(("TOTAL RAM", ram_total / 1024, "kBytes", "-", "RAM"))

    print(
        tabulate(
            table,
            headers=["Symbol", "Size (bytes)", "Section type", "Addr", "Section"],
            tablefmt="github",
        )
    )


if __name__ == "__main__":
    main()
