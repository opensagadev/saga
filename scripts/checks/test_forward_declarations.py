"""Tests for the libclang declaration-record checker."""

import unittest

from scripts.checks.check_forward_declarations import (
    FunctionDeclaration,
    find_mismatches,
)


class ForwardDeclarationCheckTest(unittest.TestCase):
    def record(
        self,
        *,
        path: str = "src/example.cpp",
        line: int = 1,
        is_definition: bool = False,
        name: str = "update",
        semantic_identity: str = "c:@F@update",
        symbol_usr: str = "c:@F@update#I#",
        canonical_type: str = "void (int)",
        structural_type: str = "function<void,int>",
    ) -> FunctionDeclaration:
        return FunctionDeclaration(
            path,
            line,
            1,
            is_definition,
            name,
            semantic_identity,
            symbol_usr,
            canonical_type,
            structural_type,
        )

    def test_matches_same_canonical_symbol_and_type(self) -> None:
        records = [
            self.record(),
            self.record(path="src/definition.cpp", is_definition=True),
        ]
        self.assertEqual(find_mismatches(records), [])

    def test_reports_parameter_mismatch(self) -> None:
        records = [
            self.record(
                symbol_usr="c:@F@update#f#",
                canonical_type="void (float)",
                structural_type="function<void,float>",
            ),
            self.record(path="src/definition.cpp", is_definition=True),
        ]
        mismatches = find_mismatches(records)
        self.assertEqual(len(mismatches), 1)
        self.assertEqual(mismatches[0].reason, "no definition has this parameter signature")

    def test_accepts_matching_signature_with_different_usr(self) -> None:
        records = [
            self.record(symbol_usr="c:@F@update#I#"),
            self.record(
                path="src/definition.cpp",
                is_definition=True,
                symbol_usr="c:@F@update#I#external",
            ),
        ]
        self.assertEqual(find_mismatches(records), [])

    def test_reports_return_type_mismatch(self) -> None:
        records = [
            self.record(canonical_type="bool (int)"),
            self.record(
                path="src/definition.cpp",
                is_definition=True,
                canonical_type="int (int)",
                structural_type="function<int,int>",
            ),
        ]
        mismatches = find_mismatches(records)
        self.assertEqual(len(mismatches), 1)
        self.assertEqual(mismatches[0].reason, "conflicting canonical type")

    def test_keeps_namespaces_separate(self) -> None:
        records = [
            self.record(semantic_identity="c:@N@one@F@update"),
            self.record(
                path="src/definition.cpp",
                is_definition=True,
                semantic_identity="c:@N@two@F@update",
            ),
        ]
        self.assertEqual(find_mismatches(records), [])

    def test_ignores_declarations_without_repository_definition(self) -> None:
        self.assertEqual(find_mismatches([self.record()]), [])


if __name__ == "__main__":
    unittest.main()
