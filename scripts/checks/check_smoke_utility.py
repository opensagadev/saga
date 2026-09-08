#!/usr/bin/env python3
"""Integration checks; requires the native smoke binary, OBB and an X display."""
import hashlib
import pathlib
import subprocess
import sys
import tempfile


def main():
    binary = str(pathlib.Path(sys.argv[1]).resolve())
    root = pathlib.Path(__file__).resolve().parents[2]
    fixture = root / "res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame"
    original = fixture.read_bytes()

    def run(args, status, message):
        result = subprocess.run([binary, *args], cwd=root, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, text=True, timeout=100)
        assert result.returncode == status, (args, result.returncode, result.stdout)
        assert message in result.stdout, (args, result.stdout)

    run(["--help"], 0, "Usage:")
    run([], 2, "specify exactly one")
    run(["--area", "Negotiations", "--frames", "-1"], 2, "numeric options")
    with tempfile.TemporaryDirectory() as temporary:
        corrupt = pathlib.Path(temporary) / "corrupt.save"
        data = bytearray(original)
        data[-8] ^= 1  # checksum, not the independently derived slot hash
        corrupt.write_bytes(data)
        run(["--area", "Negotiations", "--save", str(corrupt)], 2, "invalid game save")
        run(["--area", "Negotiations", "--save", str(corrupt) + ".missing"], 2, "invalid game save")
    run(["--area", "does-not-exist"], 2, "unknown destination")
    run(["--area", "Negotiations", "--timeout-ms", "1"], 124, "overall deadline")
    run(["--area", "Negotiations", "--stall-ms", "1"], 124, "stopped completing frames")
    run(["--area", "Negotiations", "--frames", "120"], 0, "smoke: PASS")
    # Version-7 turret records with a nonempty optional sound name previously
    # shifted the following record and overflowed the animation-name buffer.
    run(["--area", "AnakinsFlight", "--frames", "300"], 0, "smoke: PASS")
    assert hashlib.sha256(fixture.read_bytes()).digest() == hashlib.sha256(original).digest(), "fixture was modified"
    print("Smoke utility: input validation, watchdogs, gameplay and fixture preservation passed")


if __name__ == "__main__":
    main()
