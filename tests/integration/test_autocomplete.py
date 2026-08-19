"""Tests for Tab completion (WSH_SHELL_AUTOCOMPLETE).

Completion rewrites the input line in place, so these drive the keyboard directly
and read the raw PTY echo rather than going through the adapter.
"""

import re

from .conftest import ShellSession

_ANSI_RE = re.compile(rb"\x1b(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")


def _clean(raw: bytes) -> str:
    return _ANSI_RE.sub(b"", raw).decode("utf-8", errors="replace")


class TestCommandCompletion:
    def test_unique_prefix_completes_and_runs(self, session: ShellSession) -> None:
        # "du" matches only "dump"; completion appends a trailing space, so Enter
        # must execute the completed command.
        session.type("du")
        session.press("tab")
        out = _clean(session.enter())
        assert "00000000" in out, f"Expected the dump command to run, got:\n{out}"

    def test_completed_line_has_no_stray_characters(self, session: ShellSession) -> None:
        session.type("ex")
        session.press("tab")
        line = _clean(session.read_raw(0.3))
        # The completion is echoed as "exit " — a truncated or overrun candidate
        # row would show a mangled name here.
        assert "exit" in line
        session.press("ctrl_c")

    def test_no_match_reports_it(self, session: ShellSession) -> None:
        session.type("zzz")
        session.press("tab")
        assert "no matches" in _clean(session.read_raw(0.3))
        session.press("ctrl_c")

    def test_ambiguous_prefix_lists_candidates(self, session: ShellSession) -> None:
        # Both "wsh" and the example commands start differently, so an empty
        # prefix lists everything the user may run.
        session.press("tab")
        out = _clean(session.read_raw(0.3))
        assert "dump" in out and "wsh" in out
        session.press("ctrl_c")
