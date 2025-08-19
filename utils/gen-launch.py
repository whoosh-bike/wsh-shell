#!/usr/bin/env python3
import logging
import os
import re
from pathlib import Path

from dotenv import load_dotenv

# source .venv/bin/activate
# python3 ./utils/gen-launch.py

# Logging setup
logging.basicConfig(level=logging.INFO, format="%(message)s")
log = logging.getLogger("launchgen")

# Load environment variables from .env file (if exists)
env_path = Path(".env")
if env_path.exists():
    load_dotenv(dotenv_path=env_path)
    log.info("Loaded environment variables from .env")
else:
    log.warning(".env file not found; relying on system environment")


def generate_vscode_launch_from_template() -> None:
    """Generates .vscode/launch.json from a template using environment variables"""

    template_path = Path(".vscode/launch.template.json")
    output_path = Path(".vscode/launch.json")

    if not template_path.exists():
        log.error("Template file not found: .vscode/launch.template.json")
        return

    def substitute(match: re.Match) -> str:
        key = match.group(1)
        value = os.getenv(key)
        if value is None:
            log.warning(f"Environment variable '{{{key}}}' not set")
            return f"<missing:{key}>"
        return value

    content = re.sub(r"\{\{\s*(\w+)\s*\}\}", substitute, template_path.read_text(encoding="utf-8"))
    output_path.write_text(content, encoding="utf-8")
    log.info(f"Generated: {output_path}")


if __name__ == "__main__":
    generate_vscode_launch_from_template()
