# Code-generation override cleanup

Date: 2026-09-08.

The source audit removed explicit `regparm`/`sseregparm` calling conventions,
their compatibility macros, per-function `optimize` attributes (including loop
unrolling), forced stack alignment, and `noinline`/`always_inline` attributes.
Translation-unit compiler options remain governed by the existing Bazel map.
Do not reintroduce these attributes to improve an instruction similarity score.

## Authorized symbol-retention fallback

The user subsequently authorized `noinline` if the original compilation could
not be recovered. Isolated NDK r8e GCC 4.7 builds at `-O1`, `-O2` and `-O3`
inline all three of `CutScene_Configure`, `UpdatePodRaceMines` and
`Tag_FindGameObject_TRANSFER`. Their configured translation-unit levels are
`-O2`, `-O3` and `-O2`, respectively; those levels remain unchanged. These three
functions now use the authorized `noinline` fallback to preserve their original
local text symbols. This is not a claim to have recovered the original compiler
decision, and does not authorize other calling-convention or codegen overrides.

The synthetic `CharConfig_GetKeywords` accessor was removed, along with its
extra-symbol baseline entry. The parser references `ConfigChar_Keywords`
directly across the reconstructed source split, with hidden linkage and its
original ELF name `_ZL19ConfigChar_Keywords` (original address `0x666aa0`).

Verification after this fallback: the function-symbol check reports **0 extra,
0 missing**; target/native builds and all four repository checks pass.
Objdiff measures `CutScene_Configure` at 24.373%, `UpdatePodRaceMines` at
42.678%, and `Tag_FindGameObject_TRANSFER` at 99.773%. Restored symbol coverage
does not imply complete instruction matching.

## Earlier cleanup

Local `volatile` qualifiers were removed from cutscene ground height, the hat
offset pointer, turret progress clearing, and the camera scissor-state variable.
Thread-shared state and the memory-pool ABI retain their existing qualifiers.
Symbol names, visibility, weak linkage, layout attributes and symbol-retention
markers were not broadly stripped: those serve distinct ABI/linkage purposes.
`Shop_GetInput` also lost its retention marker, allowing normal GCC local-call
optimization; it measures 97.458% without a forced calling convention.

The assembly alias from C `NuShaderObjectInit` to its C++ compiled-shader
overload was removed. The original functions have different signatures and
bodies (`0x30b8f0` versus `0x30b970`). The recovered C entry point takes vertex
source and its length, supplies the original magenta fragment source, calls
the source-based GLSL initializer, and probes semantics. The source-based
GLSL initializer itself remains unfinished; this cleanup does not claim to
complete shader-source compilation.

The reconstructed C entry point measures 99.931%; its two differing
instructions contain relocated addresses. Target and native builds and all
four repository checks passed after the cleanup.
The native counter-entry, camera-transition and normal menu-cancellation
probe also passed using isolated saves after removing the attributes.

Earlier matching tables are historical checkpoints; attribute removal may
lower their scores. Measure the live target instead of restoring overrides
to preserve those numbers.
