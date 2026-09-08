# _saga_

![Progress](https://img.shields.io/badge/matching-32.43%25-orange)
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
| `(root)` | 61.8% | 0.0% |
| `MechInputTouch` | 10.7% | 6.3% |
| `editor` | 3.2% | 1.7% |
| `gameapi` | 22.6% | 3.8% |
| `gameframework` | 99.9% | 5.9% |
| `gamelib` | 16.9% | 5.3% |
| `java` | 96.0% | 0.0% |
| `legoapi` | 30.5% | 9.9% |
| `legoapi/actions` | 20.4% | 1.4% |
| `legoapi/ai` | 39.9% | 1.7% |
| `legoapi/audio` | 53.4% | 10.2% |
| `legoapi/characters` | 26.5% | 6.9% |
| `legoapi/core` | 26.3% | 6.6% |
| `legoapi/cutscenes` | 34.6% | 3.9% |
| `legoapi/gizmo` | 33.9% | 11.1% |
| `legoapi/gizmos` | 48.5% | 32.6% |
| `legoapi/items` | 30.5% | 8.8% |
| `legoapi/menus` | 24.2% | 6.6% |
| `legoapi/misc` | 22.5% | 4.2% |
| `legoapi/props` | 41.9% | 2.5% |
| `legoapi/render` | 28.7% | 7.6% |
| `legoapi/world` | 26.7% | 6.3% |
| `legogame` | 49.5% | 11.1% |
| `nu2api` | 44.8% | 19.8% |

<!-- matching-table-end -->

## Legal ⚖️

This educational research project contains reconstructed code only. Do not
commit game assets, leaked source code, or other copyrighted game data. The
original game remains the property of its owners.
