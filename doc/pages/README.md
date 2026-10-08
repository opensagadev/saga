# Website moved

The standalone website and GitHub Pages deployment now live in
[opensaga.dev](https://github.com/opensagadev/opensaga.dev).
That repository pins Saga as a submodule, consumes the committed `matching.json`,
and builds `//src:saga_wasm`. It owns templates, shared styles, navigation,
the player, progress explorer, and NuDat application.

Saga still owns matching report generation and the minimal Emscripten host
shell used for local engine development. See `CONTRIBUTING.md` for those commands.
Generated files from previous local site builds can be discarded.
