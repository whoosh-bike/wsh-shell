import sys
from pathlib import Path

if __package__ in (None, ""):
    _repo = Path(__file__).resolve().parents[1]
    if str(_repo) not in sys.path:
        sys.path.insert(0, str(_repo))
    from wsh_shell_adapter import (
        WshShellAdapter,
        describe_serial_target,
        load_adapter_config_from_toml,
        prompt_for_serial_port,
        resolve_serial_port,
    )
else:
    from . import (
        WshShellAdapter,
        describe_serial_target,
        load_adapter_config_from_toml,
        prompt_for_serial_port,
        resolve_serial_port,
    )


DEFAULT_CONFIG_PATH = Path(__file__).resolve().with_name("config.toml")


def _run_repl(adapter: WshShellAdapter) -> int:
    print("Connected. Type command and press Enter. Type 'exit' to stop.")

    while True:
        try:
            cmd = input("usb-shell> ").strip()
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

        if cmd == ":sync":
            try:
                adapter.sync()
                print("sync: ok")
            except Exception as exc:
                print(f"sync error: {exc}")
            continue

        if cmd == ":status":
            status = adapter.status()
            print(f"state={status['state']} port={status['port']}")
            continue

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
    try:
        cfg = load_adapter_config_from_toml(DEFAULT_CONFIG_PATH)
    except ValueError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    print(describe_serial_target(port=cfg.port, vid=cfg.vid, pid=cfg.pid))
    try:
        requested_port = cfg.port
        cfg.port = resolve_serial_port(
            port=cfg.port,
            vid=cfg.vid,
            pid=cfg.pid,
            chooser=prompt_for_serial_port,
        )
        if not requested_port:
            print(f"Selected serial port: `{cfg.port}`")
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    adapter = WshShellAdapter(config=cfg)

    try:
        adapter.sync()
        status = adapter.status()
        print(f"sync: ok (port={status['port']})")
        return _run_repl(adapter)
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1
    finally:
        adapter.close()


if __name__ == "__main__":
    raise SystemExit(main())
