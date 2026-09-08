# Android native reconstruction

This change restores native interfaces used by the original APK's Java activity
and adds an NDK r8e ARMv7 build. The intended deployment keeps the original Java
and replaces `libTTapp.so`. It does not include a replacement Java application.

**ARMv7 is only for runtime testing on real Android hardware. It is not a
matching target.** Android x86 (`--config=target`) remains the matching target
against `res/libTTapp.so`; all matching percentages and regression comparisons
in this document and the generated report refer to x86. ARM ABI/layout checks
support device execution and do not imply ARM assembly matching.

## Implemented areas

- JNI activity lifecycle, surface handling, input forwarding, device information,
  package paths, APK assets and expansion-file access.
- EGL window/context initialization, GLES extension loading and render-state
  transitions; camera, material, font and occlusion dependencies.
- OpenSL ES integration and sound source/decoder/voice, streaming, listener,
  routing, effect, weak-pointer and memory-manager behavior.
- ARM layout assertions checked against the original ARM library. ARM-specific
  selection remains in the toolchain and ABI assertions; host asset/window stubs
  remain under `src/host/platform` and are excluded from the target.

The Android GLES extension-pointer table is also selected only for the target;
the host adapter uses private pointer names so they cannot collide with GL
functions exported by Emscripten or a native GL implementation. Native component
source lists select the JNI stub instead of the Android activity implementation.
Binary-layout checks use the existing `DECOMP_ASSERT` boundary: Android builds
retain the checks, while host/WASM builds use their platform's alignment and
mutex layout.

The original instruction shapes support corrections to per-source optimization
for Android file access, sound and occlusion. O0 frame/error, wind-group and
specular-light code now has separate source ownership instead of inheriting O3
from mixed files. No new per-function optimization attributes, injected assembly,
assertion bypasses or matching-score exceptions are used.

## Matching comparison

Both x86 builds were compared to `res/libTTapp.so` with the ttdecomp fork of
objdiff-cli 3.8.1. Main was independently built from an archive of
`90c48f9261ac37d1ec8944d0f00f299774034af4`; its working checkout was not used.
The committed `matching.json` and README table were regenerated with
`scripts/generate_bazel_objdiff_report.py`, the generator used by pre-commit.

| Metric | Main | This change |
|---|---:|---:|
| Whole-binary fuzzy match | 18.173262% | 19.808786% |
| Exact function matches | 1,021 | 1,182 |
| Exact matched code bytes | 40,829 | 44,456 |

The fuzzy increase is **1.635524 percentage points**. No previously exact
function loses its exact match. Of the initial 126 named score declines, four
remain; each is below one percentage point. They are reported rather than
hidden or compensated for with layout padding.

| Function | Main | This change | Original size |
|---|---:|---:|---:|
| `NuRenderDevice::SelectEGLConfig()` | 96.884610% | 96.019230% | 247 bytes |
| `NuHGobjEvalAnim2Root_3` | 99.932205% | 99.677960% | 257 bytes |
| `NuVisiEvaluate` | 96.850000% | 96.662500% | 312 bytes |
| `NuHGobjRndrMtxDwa` | 53.963577% | 53.913906% | 1,122 bytes |

These functions total 1,938 original bytes. Their score decreases amount to
3.933207 fuzzy-weighted byte equivalents, **not** a literal count of changed
bytes. Disassembly shows register-allocation/scheduling and linked-address
variation; this is not a zero-regression matching result.

## Validation and device status

Verified on 2026-09-07:

- `bazel build --config=target //src:saga_target` passes.
- `bazel build --config=android-armv7 //src:saga_target` passes, with undefined
  references rejected by the linker.
- `bazel build --config=wasm //src:saga_wasm` passes through the final HTML,
  JavaScript and WASM link on Windows.
- `bazel test //scripts/checks:checks` passes all four checks: optimization map,
  duplicate definitions, host boundaries and Pages tests.
- The symbol check passes: no unignored missing symbols and no extra-symbol
  baseline changes.
- `git diff --check` passes.

The full optional pre-commit pipeline was not run: native/clang-tidy coverage
remains unverified with the host's missing pkg-config development dependencies.
The report generator and checks above were run separately.

An earlier ARM build installed in the original APK reached its title/splash on
a OnePlus N200 running Android 12. Touch did not respond; menu/gameplay is not
verified. The final matching fixes have not been retested on the device. This
is native reconstruction progress, not a claim of complete Android playability.
