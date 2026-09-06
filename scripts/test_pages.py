"""Verify generated page separation and deployable asset paths."""

import json
from pathlib import Path
import tempfile
import unittest

from scripts.plot_binary_match_map import generate_site


class PagesTest(unittest.TestCase):
    def test_generates_progress_and_player_at_custom_output(self):
        report = {
            "schema_version": 1,
            "text": {"address": 4096, "size": 512},
            "sections": [],
            "units": [],
            "measures": {
                "fuzzy_match_percent": 0,
                "matched_functions": 0,
                "total_functions": 0,
                "matched_code_percent": 0,
            },
            "summary": {"bazel_units": 0},
        }
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            report_path = root / "matching.json"
            report_path.write_text(json.dumps(report), encoding="utf-8")
            output = root / "site" / "index.html"
            generate_site(report_path, output, 512, 512)
            progress = output.read_text(encoding="utf-8")
            player = (output.parent / "play/index.html").read_text(encoding="utf-8")

            self.assertIn('id="progress"', progress)
            self.assertNotIn('id="obb-file"', progress)
            self.assertNotIn("Module.callMain", progress)
            self.assertNotIn("coi-serviceworker.js", progress)
            self.assertNotIn("/*__DATA__*/", progress)
            self.assertIn('href="./play/"', progress)
            self.assertIn('id="obb-file"', player)
            self.assertIn('id="loading-screen"', player)
            self.assertNotIn('id="progress"', player)
            self.assertNotIn("d3.min.js", player)
            self.assertIn('src="./coi-serviceworker.js"', player)
            self.assertIn('runtimeScript.src = "../saga.js"', player)
            self.assertIn('href="../site.css"', player)
            self.assertTrue((output.parent / "site.css").is_file())
            self.assertTrue((output.parent / "play/coi-serviceworker.js").is_file())
            self.assertTrue((output.parent / ".nojekyll").is_file())
            self.assertFalse((output.parent / "coi-serviceworker.js").exists())


if __name__ == "__main__":
    unittest.main()
