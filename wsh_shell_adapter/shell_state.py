"""The shell state line (docs/usage/shell-state.md).

A shell built with WSH_SHELL_STATE_LINE prints one JSON object on its own line
whenever its state changes, and in answer to ENQ (0x05):

    {"evt":"shell","state":"busy","user":"admin","busy":"sniff:cell","keepLeft":9}

    login — nobody is logged in: answer the login prompt;
    ready — logged in, prompt free: send commands, nothing to verify;
    busy  — a prompt-wait holds the input (a stream, a question): Enter or
            Ctrl+C would end it, so a host that wants it kept writes nothing.
"""

from __future__ import annotations

import json
import re
from typing import Any, Optional

ENQ = b"\x05"

STATE_LOGIN = "login"
STATE_READY = "ready"
STATE_BUSY = "busy"
SHELL_STATES = (STATE_LOGIN, STATE_READY, STATE_BUSY)

_STATE_LINE_RE = re.compile(r'\{"evt":"shell"[^\r\n{}]*\}')
# Room for a line split across reads, without rescanning a whole stream.
_WINDOW_CHARS = 512


def parse_shell_state(line: str) -> Optional[dict[str, Any]]:
    """One state line as a dict, or None if it isn't one."""
    try:
        obj = json.loads(line)
    except ValueError:
        return None
    if not isinstance(obj, dict) or obj.get("evt") != "shell" or obj.get("state") not in SHELL_STATES:
        return None
    return obj


def find_shell_states(text: str) -> list[dict[str, Any]]:
    """Every state line in a block of (ANSI-free) shell text, in order."""
    found = []
    for match in _STATE_LINE_RE.finditer(text):
        state = parse_shell_state(match.group(0))
        if state is not None:
            found.append(state)
    return found


class ShellStateWatch:
    """For a host reading the stream itself: feed it cleaned text chunk by
    chunk, get back the newest state line each time one completes."""

    def __init__(self) -> None:
        self._window = ""

    def feed(self, text: str) -> Optional[dict[str, Any]]:
        self._window = (self._window + text)[-_WINDOW_CHARS:]
        latest: Optional[dict[str, Any]] = None
        end = 0
        for match in _STATE_LINE_RE.finditer(self._window):
            state = parse_shell_state(match.group(0))
            if state is not None:
                latest = state
            end = match.end()
        if end:
            # Each line is reported once.
            self._window = self._window[end:]
        return latest
