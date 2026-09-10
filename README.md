# _saga_

![Progress](https://img.shields.io/badge/matching-36.74%25-orange)
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
| `MechInputTouch` | 12.9% | 17.1% |
| `editor` | 3.2% | 2.0% |
| `gameapi` | 24.1% | 14.4% |
| `gameframework` | 100.0% | 52.9% |
| `gamelib` | 19.0% | 15.6% |
| `java` | 96.1% | 73.1% |
| `legoapi` | 34.1% | 26.5% |
| `legoapi/actions` | 22.7% | 2.9% |
| `legoapi/ai` | 45.7% | 20.5% |
| `legoapi/audio` | 53.7% | 43.1% |
| `legoapi/characters` | 32.3% | 17.3% |
| `legoapi/core` | 28.1% | 14.4% |
| `legoapi/cutscenes` | 34.7% | 13.5% |
| `legoapi/gizmo` | 38.0% | 33.3% |
| `legoapi/gizmos` | 48.7% | 45.7% |
| `legoapi/items` | 36.3% | 34.6% |
| `legoapi/menus` | 26.2% | 26.1% |
| `legoapi/misc` | 26.0% | 11.9% |
| `legoapi/props` | 43.0% | 11.2% |
| `legoapi/render` | 33.9% | 22.1% |
| `legoapi/world` | 27.6% | 29.7% |
| `legogame` | 50.6% | 56.1% |
| `nu2api` | 56.8% | 51.5% |

<!-- matching-table-end -->

## Legal ⚖️

This educational research project contains reconstructed code only. Do not
commit game assets, leaked source code, or other copyrighted game data. The
original game remains the property of its owners.
