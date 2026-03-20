import sys
from pathlib import Path

if __package__ in (None, ""):
    _repo = Path(__file__).resolve().parents[1]
    if str(_repo) not in sys.path:
        sys.path.insert(0, str(_repo))
    from wsh_shell_adapter import (
        AdapterConfig,
        PtyProcessTransport,
        WshShellAdapter,
        load_adapter_config_from_toml,
    )
else:
    from . import (
        AdapterConfig,
        PtyProcessTransport,
        WshShellAdapter,
        load_adapter_config_from_toml,
    )


DEFAULT_CONFIG_PATH = Path(__file__).resolve().with_name("config.toml")
DEFAULT_BINARY = "example/build/example"
DEFAULT_CWD = "."


def _repo_root() -> Path:
    return Path(__file__).resolve().parents[1]


def _resolve_path(value: str, repo_root: Path) -> Path:
    path = Path(value)
    if path.is_absolute():
        return path
    return (repo_root / path).resolve()


def _load_settings() -> tuple[AdapterConfig, str, str]:
    cfg_path = DEFAULT_CONFIG_PATH
    adapter, doc = load_adapter_config_from_toml(cfg_path, return_toml_data=True)

    local_data = doc.get("local_shell", {})
    if local_data is not None and not isinstance(local_data, dict):
        raise ValueError("`local_shell` section must be a table.")
    binary = (
        str(local_data.get("binary", DEFAULT_BINARY))
        if isinstance(local_data, dict)
        else DEFAULT_BINARY
    )
    cwd = str(local_data.get("cwd", DEFAULT_CWD)) if isinstance(local_data, dict) else DEFAULT_CWD

    return adapter, binary, cwd


def _run_repl(adapter: WshShellAdapter) -> int:
    print("Connected. Type command and press Enter. Type 'exit' or 'quit' to stop.")
    while True:
        try:
            cmd = input("local-shell> ").strip()
        except EOFError:
            print("")
            return 0
        except KeyboardInterrupt:
            print("")
            return 0

        if not cmd:
            continue
        if cmd in ("exit", "quit"):
            return 0

        try:
            result = adapter.execute(cmd)
        except Exception as exc:
            print(f"error: {exc}")
            continue

        if result.text:
            print(result.text)
        else:
            print("<empty>")


def main() -> int:
    repo_root = _repo_root()

    try:
        adapter_cfg, binary_cfg, cwd_cfg = _load_settings()
    except ValueError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    binary = _resolve_path(binary_cfg, repo_root)
    cwd = _resolve_path(cwd_cfg, repo_root)

    if not binary.exists():
        print(f"error: binary not found: {binary}", file=sys.stderr)
        print("hint: run `make example` in repository root.", file=sys.stderr)
        return 2

    transport = PtyProcessTransport([str(binary)], cwd=str(cwd))
    adapter = WshShellAdapter(config=adapter_cfg, transport=transport)

    try:
        adapter.sync()
        return _run_repl(adapter)
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    finally:
        adapter.close()


if __name__ == "__main__":
    raise SystemExit(main())
