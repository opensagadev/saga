#!/usr/bin/env python3
"""Compare current source ownership with the original's translation-unit layout.

The original `libTTapp.so` was compiled without `-ffunction-sections`, so each
original translation unit's `.text` is one contiguous address range. Static
initializers (`.text.startup`) and weak COMDAT functions are placed elsewhere
and are ignored here.

This tool reads only the committed `matching.json`: original addresses, sizes
and scores, plus the current unit that owns each function. A current unit
whose functions fall into several original address runs is fragmented. Each
function outside the unit's largest run is a stray, and the unit immediately
before it in the original address order is where it physically sits.

Strays are evidence, not instructions: a stray between two runs of one other
unit usually belongs to that unit's original translation unit, but neighbouring
original TUs with no current owner can make a stray look misplaced.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
from dataclasses import dataclass
import json
import os
from pathlib import Path
import sys


@dataclass(frozen=True)
class Function:
    address: int
    size: int
    score: float
    name: str
    source: str


@dataclass
class Run:
    source: str
    functions: list[Function]

    @property
    def size(self) -> int:
        return sum(function.size for function in self.functions)


def workspace_root() -> Path:
    bazel_workspace = os.environ.get("BUILD_WORKSPACE_DIRECTORY")
    if bazel_workspace:
        return Path(bazel_workspace).resolve()
    return Path(__file__).resolve().parents[1]


def load_functions(report: dict) -> tuple[list[Function], int]:
    """Return owned layout functions in address order and the scored byte total."""
    weak = set(report["original_text_symbols"]["weak"])
    by_address: dict[int, tuple[dict, str | None]] = {}
    for unit in report["units"]:
        for function in unit["functions"]:
            by_address.setdefault(function["address"], (function, unit["source"]))
    for function in report["ambiguous_functions"] + report["unassigned_functions"]:
        by_address.setdefault(function["address"], (function, None))

    total = sum(function["size"] for function, _ in by_address.values())
    owned = [
        Function(
            address=address,
            size=function["size"],
            score=function.get("match_percent") or 0.0,
            name=function["name"],
            source=source,
        )
        for address, (function, source) in sorted(by_address.items())
        if source is not None
        and function["name"] not in weak
        and not function["name"].startswith("_GLOBAL__sub_I_")
    ]
    return owned, total


def address_runs(functions: list[Function]) -> list[Run]:
    runs: list[Run] = []
    for function in functions:
        if runs and runs[-1].source == function.source:
            runs[-1].functions.append(function)
        else:
            runs.append(Run(function.source, [function]))
    return runs


def strays(runs: list[Run]) -> list[tuple[Function, str | None]]:
    """Return functions outside their unit's largest run, with the host unit."""
    largest: dict[str, int] = {}
    for index, run in enumerate(runs):
        best = largest.get(run.source)
        if best is None or run.size > runs[best].size:
            largest[run.source] = index
    result = []
    for index, run in enumerate(runs):
        if largest[run.source] == index:
            continue
        host = runs[index - 1].source if index else None
        result.extend((function, host) for function in run.functions)
    return result


def lost(function: Function) -> float:
    return function.size * (100.0 - function.score) / 100.0


def short(source: str | None) -> str:
    if source is None:
        return "(start of .text)"
    return source.removeprefix("src/")


def print_summary(runs: list[Run], stray: list, total: int, limit: int) -> None:
    units = {run.source for run in runs}
    fragmented = {function.source for function, _ in stray}
    lost_all = Counter()
    for run in runs:
        for function in run.functions:
            lost_all[run.source] += lost(function)
    stray_bytes = sum(function.size for function, _ in stray)
    owned_bytes = sum(run.size for run in runs)
    print(f"units: {len(units)}, contiguous: {len(units) - len(fragmented)}, fragmented: {len(fragmented)}")
    print(
        f"stray functions: {len(stray)}, {stray_bytes} of {owned_bytes} bytes "
        f"({stray_bytes / owned_bytes * 100:.1f}%), "
        f"{sum(lost(function) for function, _ in stray) / total * 100:.2f} points lost"
    )
    print(
        "points lost in fragmented units: "
        f"{sum(lost_all[source] for source in fragmented) / total * 100:.2f}, "
        "contiguous units: "
        f"{sum(lost_all[source] for source in units - fragmented) / total * 100:.2f}"
    )
    print("\nlargest stray groups (count, points lost, owner -> sits in):")
    groups: dict[tuple[str, str | None], list[Function]] = defaultdict(list)
    for function, host in stray:
        groups[(function.source, host)].append(function)
    ranked = sorted(groups.items(), key=lambda item: -len(item[1]))
    for (source, host), functions in ranked[:limit]:
        points = sum(map(lost, functions)) / total * 100
        print(f"{len(functions):5} {points:6.3f}  {short(source)} -> {short(host)}")


def print_unit(runs: list[Run], source: str, total: int) -> int:
    indices = [index for index, run in enumerate(runs) if run.source == source]
    if not indices:
        print(f"{source}: no owned layout functions in the report", file=sys.stderr)
        return 1
    largest = max(indices, key=lambda index: runs[index].size)
    for index in indices:
        run = runs[index]
        first, last = run.functions[0], run.functions[-1]
        tag = "main" if index == largest else "stray"
        print(
            f"\n[{tag}] 0x{first.address:x}-0x{last.address + last.size:x} "
            f"{run.size} bytes, after {short(runs[index - 1].source if index else None)}, "
            f"before {short(runs[index + 1].source if index + 1 < len(runs) else None)}"
        )
        for function in run.functions:
            print(f"  0x{function.address:x} {function.size:6} {function.score:9.5f}%  {function.name}")
    return 0


def parse_args(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--report", type=Path, help="matching report (default: workspace matching.json)")
    parser.add_argument("--unit", help="list one unit's original address runs, e.g. src/legoapi/world/levels/hub.cpp")
    parser.add_argument("--limit", type=int, default=30, help="stray groups to list in the summary")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    report_path = args.report or workspace_root() / "matching.json"
    with report_path.open(encoding="utf-8") as report_file:
        report = json.load(report_file)
    functions, total = load_functions(report)
    runs = address_runs(functions)
    if args.unit:
        return print_unit(runs, args.unit, total)
    print_summary(runs, strays(runs), total, args.limit)
    return 0


if __name__ == "__main__":
    sys.exit(main())
