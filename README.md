# saga

![Progress](https://img.shields.io/badge/matching-46.00%25-orange)
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
| `editor` | 8.2% | 13.9% |
| `gameapi` | 30.9% | 17.6% |
| `gameframework` | 92.5% | 34.4% |
| `gamelib` | 27.7% | 24.1% |
| `java` | 96.1% | 73.1% |
| `legoapi` | 42.8% | 30.0% |
| `legoapi/actions` | 39.3% | 10.6% |
| `legoapi/ai` | 48.0% | 24.3% |
| `legoapi/audio` | 59.8% | 46.1% |
| `legoapi/characters` | 42.8% | 19.8% |
| `legoapi/core` | 30.6% | 22.6% |
| `legoapi/cutscenes` | 41.5% | 17.4% |
| `legoapi/gizmo` | 55.1% | 43.8% |
| `legoapi/gizmos` | 49.1% | 43.9% |
| `legoapi/items` | 46.3% | 36.9% |
| `legoapi/menus` | 32.1% | 27.9% |
| `legoapi/misc` | 28.3% | 11.4% |
| `legoapi/props` | 61.2% | 42.1% |
| `legoapi/render` | 39.9% | 24.1% |
| `legoapi/world` | 38.9% | 29.0% |
| `legogame` | 53.8% | 58.7% |
| `nu2api` | 70.7% | 58.5% |

<!-- matching-table-end -->

## Legal ⚖️

This educational research project contains reconstructed code only. Do not
commit game assets, leaked source code, or other copyrighted game data. The
original game remains the property of its owners.
