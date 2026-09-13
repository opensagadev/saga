# saga

![Progress](https://img.shields.io/badge/matching-45.68%25-orange)
[![Bazel build](https://github.com/ttdecomp/saga/actions/workflows/build-bazel.yaml/badge.svg)](https://github.com/ttdecomp/saga/actions/workflows/build-bazel.yaml)
[![Discord](https://img.shields.io/discord/1467775700894224555?color=%235865F2&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/2HJuMtzA7q)

**saga** is a matching decompilation of the Android x86 release of
*LEGO Star Wars: The Complete Saga*.

The goal of this project is to recreate readable source code that compiles into
the exact same machine code as the original game. This allows us to guarantee
that we have identical behavior to the original.

Once complete, this decompilation is planned to serve as the foundation for
[saga64](https://github.com/opensagadev/saga64), a more modern, portable version of the game.

Visit [opensaga.dev](https://opensaga.dev/) to learn more about the project,
[follow its progress](https://opensaga.dev/progress/), or
[try the experimental web build](https://opensaga.dev/play/) with your own game files.

See [CONTRIBUTING.md](CONTRIBUTING.md) for setup instructions and development workflows.

## Legal

Clone and build every variant:

```sh
git clone https://github.com/ttdecomp/saga.git
cd saga

bazel build --config=target //src:saga_target
bazel build --config=native //src:saga_native
bazel build --config=wasm //src:saga_wasm
bazel test //scripts/checks:checks
```

Optimized, sanitizer-free host builds are available as
`--config=native_release` and `--config=wasm_release`; both compile with
`-O2`.

To run the native or browser build after supplying your own game assets, see
[CONTRIBUTING.md](CONTRIBUTING.md):

```sh
bazel run --config=native //src:run_native -- window
bazel run --config=wasm //scripts:wasm_server
```

<!-- matching-table-start -->

## Matching progress 📊

See https://ttdecomp.github.io/saga/

| Directory | Fuzzy % | Funcs % |
|---|---:|---:|
| `(root)` | 62.0% | 50.0% |
| `MechInputTouch` | 28.3% | 34.6% |
| `editor` | 8.1% | 13.8% |
| `gameapi` | 31.5% | 18.0% |
| `gameframework` | 100.0% | 52.9% |
| `gamelib` | 27.7% | 24.1% |
| `java` | 96.1% | 73.1% |
| `legoapi` | 42.4% | 30.0% |
| `legoapi/actions` | 37.5% | 7.2% |
| `legoapi/ai` | 48.0% | 24.3% |
| `legoapi/audio` | 59.6% | 46.1% |
| `legoapi/characters` | 41.5% | 19.7% |
| `legoapi/core` | 30.7% | 21.3% |
| `legoapi/cutscenes` | 41.5% | 17.5% |
| `legoapi/gizmo` | 50.5% | 42.3% |
| `legoapi/gizmos` | 50.7% | 45.0% |
| `legoapi/items` | 45.3% | 37.5% |
| `legoapi/menus` | 30.9% | 27.3% |
| `legoapi/misc` | 29.4% | 13.5% |
| `legoapi/props` | 59.3% | 21.8% |
| `legoapi/render` | 40.7% | 27.2% |
| `legoapi/world` | 39.8% | 29.6% |
| `legogame` | 50.8% | 60.0% |
| `nu2api` | 72.2% | 58.3% |

<!-- matching-table-end -->

## Legal ⚖️

This educational research project contains reconstructed code only. Do not
commit game assets, leaked source code, or other copyrighted game data. The
original game remains the property of its owners.
