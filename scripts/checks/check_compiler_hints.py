#!/usr/bin/env python3
"""Reject branch-prediction hints and optimization attributes in source."""

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "src"
SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".h", ".hh", ".hpp", ".inc"}
ATTRIBUTE_BODY = r"__attribute__\s*\(\((?:[^()]|\([^()]*\))*"
PATTERNS = (
    ("__builtin_expect", re.compile(r"\b__builtin_expect\b")),
    ("#pragma GCC optimize", re.compile(r"#\s*pragma\s+GCC\s+optimize\b")),
    ("optimize attribute", re.compile(r"\b(?:__)?optimize(?:__)?\s*\(")),
    (
        "hot/cold attribute",
        re.compile(ATTRIBUTE_BODY + r"\b(?:__)?(?:hot|cold)(?:__)?\b"),
    ),
)


def main() -> int:
    errors: list[str] = []
    for path in sorted(SRC.rglob("*")):
        if not path.is_file() or path.suffix not in SOURCE_SUFFIXES:
            continue
        text = path.read_text(encoding="utf-8", errors="replace")
        for number, line in enumerate(text.splitlines(), start=1):
            for name, pattern in PATTERNS:
                if pattern.search(line):
                    errors.append(f"{path.relative_to(ROOT)}:{number}: {name}")
                    break

    if errors:
        for error in errors:
            print(f"compiler-hints: {error}", file=sys.stderr)
        print(
            "compiler-hints: code layout must come from source structure, "
            "not branch hints or optimization attributes",
            file=sys.stderr,
        )
        return 1
    print("compiler-hints: no branch hints or optimization attributes in src")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
