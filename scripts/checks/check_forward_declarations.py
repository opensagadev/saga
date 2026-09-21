#!/usr/bin/env python3
"""Compare the canonical function declarations collected by libclang."""

from __future__ import annotations

import argparse
from collections import defaultdict
from dataclasses import dataclass
import os
from pathlib import Path
import sys
from typing import Iterable, Sequence


@dataclass(frozen=True, order=True)
class FunctionDeclaration:
    path: str
    line: int
    column: int
    is_definition: bool
    name: str
    semantic_identity: str
    symbol_usr: str
    canonical_type: str
    structural_type: str

    @classmethod
    def parse(cls, line: str) -> FunctionDeclaration:
        fields = line.rstrip("\n").split("\t")
        if len(fields) != 9:
            raise ValueError(f"expected 9 tab-separated fields, got {len(fields)}")
        path, source_line, column, definition, name, identity, usr, type_name, type_identity = fields
        return cls(
            path=path,
            line=int(source_line),
            column=int(column),
            is_definition=definition == "1",
            name=name,
            semantic_identity=identity,
            symbol_usr=usr,
            canonical_type=type_name,
            structural_type=type_identity,
        )

    @property
    def location(self) -> str:
        return f"{self.path}:{self.line}:{self.column}"


@dataclass(frozen=True)
class SignatureMismatch:
    declaration: FunctionDeclaration
    definitions: tuple[FunctionDeclaration, ...]
    reason: str


def read_records(paths: Iterable[Path]) -> list[FunctionDeclaration]:
    records: set[FunctionDeclaration] = set()
    for path in paths:
        with path.open(encoding="utf-8") as stream:
            for line_number, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                try:
                    records.add(FunctionDeclaration.parse(line))
                except ValueError as error:
                    raise ValueError(f"{path}:{line_number}: {error}") from error
    return sorted(records)


def find_mismatches(records: Iterable[FunctionDeclaration]) -> list[SignatureMismatch]:
    groups: dict[str, list[FunctionDeclaration]] = defaultdict(list)
    for record in records:
        if record.semantic_identity and record.symbol_usr:
            groups[record.semantic_identity].append(record)

    mismatches = []
    for declarations in groups.values():
        definitions = tuple(record for record in declarations if record.is_definition)
        if not definitions:
            continue

        for declaration in (record for record in declarations if not record.is_definition):
            if any(
                definition.structural_type == declaration.structural_type
                for definition in definitions
            ):
                continue
            same_symbol = tuple(
                definition
                for definition in definitions
                if definition.symbol_usr == declaration.symbol_usr
            )
            reason = (
                "conflicting canonical type"
                if same_symbol
                else "no definition has this parameter signature"
            )
            mismatches.append(SignatureMismatch(declaration, same_symbol or definitions, reason))

    return sorted(
        mismatches,
        key=lambda mismatch: (
            mismatch.declaration.path,
            mismatch.declaration.line,
            mismatch.declaration.column,
            mismatch.declaration.canonical_type,
        ),
    )


def runfile_records() -> list[Path]:
    runfiles_directory = os.environ.get("RUNFILES_DIR")
    if runfiles_directory:
        return sorted(
            path
            for path in Path(runfiles_directory).rglob("*.tsv")
            if any(part.endswith("_function_declarations") for part in path.parts)
        )

    manifest_path = os.environ.get("RUNFILES_MANIFEST_FILE")
    if manifest_path:
        records = []
        with Path(manifest_path).open(encoding="utf-8") as manifest:
            for line in manifest:
                logical, separator, physical = line.rstrip("\n").partition(" ")
                if separator and logical.endswith(".tsv") and "_function_declarations/" in logical:
                    records.append(Path(physical))
        return sorted(records)

    return []


def print_mismatch(mismatch: SignatureMismatch) -> None:
    declaration = mismatch.declaration
    print(
        f"{declaration.location}: {mismatch.reason}: "
        f"{declaration.name} has type {declaration.canonical_type}",
        file=sys.stderr,
    )
    for definition in mismatch.definitions:
        print(
            f"  definition at {definition.location}: {definition.canonical_type}",
            file=sys.stderr,
        )


def parse_arguments(arguments: Sequence[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("records", nargs="*", type=Path)
    parser.add_argument(
        "--list",
        action="store_true",
        help="print every collected declaration and definition",
    )
    return parser.parse_args(arguments)


def main(arguments: Sequence[str] | None = None) -> int:
    options = parse_arguments(arguments)
    record_paths = options.records or runfile_records()
    if not record_paths:
        print("no libclang declaration records were supplied", file=sys.stderr)
        return 1

    records = read_records(record_paths)
    if options.list:
        for record in records:
            kind = "definition" if record.is_definition else "declaration"
            print(f"{record.location}\t{kind}\t{record.name}\t{record.canonical_type}")

    mismatches = find_mismatches(records)
    for mismatch in mismatches:
        print_mismatch(mismatch)
    if mismatches:
        print(
            f"found {len(mismatches)} mismatched forward declaration(s)",
            file=sys.stderr,
        )
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
