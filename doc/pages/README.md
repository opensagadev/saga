# Matching report page

> Agent/reference document. Human matching and Pages workflows are in
> [`CONTRIBUTING.md`](../../CONTRIBUTING.md).

The repository-root `matching.json` is generated from one whole-binary
`objdiff-cli` report for
`res/libTTapp.so` and `bazel-bin/src/libTTapp.so`. The generator uses Bazel's
C++ compile actions and the symbol tables in their output objects to assign
original functions to source translation units.

From the repository root:

```sh
bazel build --config=target //src:saga_target
bazel run //scripts:generate_bazel_objdiff_report
bazel run //scripts:plot_binary_match_map
```

Both scripts are Bazel `py_binary` targets, so these commands use the pinned
Python 3.12.12 toolchain from `MODULE.bazel` rather than system site-packages.
The report generator also requires the external `objdiff-cli` executable on
`PATH` and the locally supplied `res/libTTapp.so` reference binary.

The first script writes the repository-root `matching.json`. Its top-level
`units` list contains
the Bazel source path, object path, and assigned functions. Each function has
its original virtual address, `.text` offset, original symbol size, and
whole-binary objdiff match percentage. Duplicate symbols whose unit cannot be
chosen safely are kept in `ambiguous_functions`; symbols absent from every
Bazel object are kept in `unassigned_functions`.

It also rewrites the marker-delimited matching table and overall progress badge
in the repository `README.md`. Directory fuzzy percentages are weighted by
original function size; function percentages count exact 100% matches. The new
whole-binary report cannot attribute data symbols to source directories, so it
does not reproduce the old CMake table's per-directory data column.

The report also stores the original binary's global, local, and weak text
symbol names in `original_text_symbols`. CI uses that derived symbol surface to
run `check_symbols` without requiring the copyrighted reference binary.

The second script fills `scripts/plot_binary_match_map.html` with the compact
report payload to produce the progress homepage, `index.html`. The generator
also copies `scripts/wasm_player.html` to `play/index.html` and the shared
`scripts/site.css` to `site.css`. The experimental player contains no matching
report or D3 dependency; the progress page does not initialize the game.

The player accepts local files, a server OBB at the site root, or a remote
URL. Opening `play/?obb=<encoded-url>` shows a dedicated loading splash and
automatically starts the game after browser isolation, WASM initialization,
and the download finish. Existing homepage `?obb=` links redirect to the
player with their query intact. Selecting a file or submitting a URL also
shows loading progress and starts the game as soon as the data is ready.
The Pages workflow builds the WASM target, copies `saga.js` and `saga.wasm`
to `play/`, and deploys `doc/pages/`. The player loads those artifacts from
its own directory so pthread workers stay inside the isolation service worker
scope.

Remote OBB URLs are fetched directly without a proxy. Cross-origin sources
must allow CORS requests from the player origin. If a source does not allow
browser downloads, download the OBB separately and select the local file.
A generated service worker in `play/` provides the cross-origin isolation
required by the threaded WASM build on GitHub Pages. It intercepts only
document and worker-script requests; OBB downloads and other fetches use the
browser network path directly, keeping long downloads independent of the
service worker lifetime.

The pre-commit hook is a small shim for the `//scripts:pre_commit` Bazel
`py_binary`. That target runs the build, symbol check, and Bazel report
generator when the ignored original binary is available, then stages
`matching.json` and `README.md` automatically. It skips these binary-dependent
steps on checkouts that do not have `res/libTTapp.so`; Pages HTML generation
belongs to the GitHub Actions runner.
