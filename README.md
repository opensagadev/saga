# _saga_

![Progress](https://img.shields.io/badge/matching-36.52%25-orange)
[![Bazel build](https://github.com/ttdecomp/saga/actions/workflows/build-bazel.yaml/badge.svg)](https://github.com/ttdecomp/saga/actions/workflows/build-bazel.yaml)
[![Discord](https://img.shields.io/discord/1467775700894224555?color=%235865F2&logo=discord&logoColor=%23FFFFFF)](https://discord.gg/2HJuMtzA7q)
[![status & wasm build](https://img.shields.io/badge/status%20%26%20wasm%20build-click%20here-orange?style=flat)](https://opensaga.dev/)

This is a decompilation of _LEGO Star Wars: The Complete Saga_, based on the
Android x86 release. The repository builds three variants:

| Variant  | Purpose                                             |
| -------- | --------------------------------------------------- |
| `target` | Android x86 shared library used for binary matching |
| `native` | Linux or Windows diagnostic executable              |
| `wasm`   | Browser diagnostic build                            |

Game assets and the original binary are not included or required to compile.

## Build 🔨

You need Git and an x86-64 Linux C/C++ toolchain. The Linux `native`
variant uses system libraries: it needs your distribution's
32-bit SDL 3, Vorbis, EGL, GLES, and `pkg-config` development support. Bazel
downloads the toolchains and libraries used by `target` and `wasm`.

Install Bazelisk as `bazel` and it will select the version in `.bazelversion`.
Alternatively, install that Bazel version directly.

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
| `MechInputTouch` | 12.7% | 16.9% |
| `editor` | 3.2% | 1.7% |
| `gameapi` | 24.1% | 14.2% |
| `gameframework` | 100.0% | 52.9% |
| `gamelib` | 16.9% | 9.4% |
| `java` | 96.1% | 73.1% |
| `legoapi` | 33.8% | 25.6% |
| `legoapi/actions` | 21.2% | 2.4% |
| `legoapi/ai` | 45.7% | 20.3% |
| `legoapi/audio` | 53.7% | 43.1% |
| `legoapi/characters` | 32.2% | 17.0% |
| `legoapi/core` | 28.0% | 14.4% |
| `legoapi/cutscenes` | 34.6% | 11.8% |
| `legoapi/gizmo` | 37.2% | 28.7% |
| `legoapi/gizmos` | 48.6% | 45.2% |
| `legoapi/items` | 36.3% | 34.4% |
| `legoapi/menus` | 25.6% | 24.5% |
| `legoapi/misc` | 23.7% | 10.4% |
| `legoapi/props` | 43.0% | 11.2% |
| `legoapi/render` | 33.8% | 21.7% |
| `legoapi/world` | 27.5% | 29.3% |
| `legogame` | 50.6% | 56.1% |
| `nu2api` | 56.7% | 50.7% |

<!-- matching-table-end -->

## Legal ⚖️

This educational research project contains reconstructed code only. Do not
commit game assets, leaked source code, or other copyrighted game data. The
original game remains the property of its owners.
