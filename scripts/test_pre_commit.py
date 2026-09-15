"""Regression tests for the commit hook's source-formatting scope."""

from pathlib import Path
import subprocess
import tempfile
import unittest

from scripts.pre_commit import format_sources


class FormatSourcesTest(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        (self.root / "src").mkdir()
        self.staged = self.root / "src/staged.cpp"
        self.unstaged = self.root / "src/unstaged.cpp"
        self.staged.write_text("int a=1;\n", encoding="utf-8")
        self.unstaged.write_text("int b=1;\n", encoding="utf-8")
        self.git("init", "-q")
        self.git("config", "user.name", "Test")
        self.git("config", "user.email", "test@example.invalid")
        self.git("add", "src")
        self.git("commit", "-qm", "baseline")
        self.formatter = self.root / "formatter.py"
        self.formatter.write_text(
            "#!/usr/bin/env python3\n"
            "from pathlib import Path\n"
            "import sys\n"
            "for arg in sys.argv[1:]:\n"
            "    if not arg.startswith('-'):\n"
            "        path = Path(arg)\n"
            "        path.write_text(path.read_text().replace('=1', '= 1'))\n",
            encoding="utf-8",
        )
        self.formatter.chmod(0o755)

    def git(self, *args: str) -> str:
        result = subprocess.run(
            ["git", *args], cwd=self.root, check=True, capture_output=True, text=True
        )
        return result.stdout

    def test_formats_only_staged_source(self) -> None:
        self.staged.write_text("int a=1; // staged\n", encoding="utf-8")
        self.git("add", "src/staged.cpp")
        self.unstaged.write_text("int b=1; // unstaged\n", encoding="utf-8")

        self.assertEqual(format_sources(self.root, self.formatter), 0)
        self.assertEqual(self.staged.read_text(encoding="utf-8"), "int a= 1; // staged\n")
        self.assertEqual(self.unstaged.read_text(encoding="utf-8"), "int b=1; // unstaged\n")
        self.assertEqual(self.git("diff", "--cached", "--name-only"), "src/staged.cpp\n")
        self.assertIn("a= 1", self.git("show", ":src/staged.cpp"))

    def test_rejects_overlap_without_touching_worktree(self) -> None:
        self.staged.write_text("int a=1; // staged\n", encoding="utf-8")
        self.git("add", "src/staged.cpp")
        self.staged.write_text("int a=1; // unstaged too\n", encoding="utf-8")

        self.assertEqual(format_sources(self.root, self.formatter), 1)
        self.assertEqual(self.staged.read_text(encoding="utf-8"), "int a=1; // unstaged too\n")
        self.assertIn("a=1", self.git("show", ":src/staged.cpp"))


if __name__ == "__main__":
    unittest.main()
