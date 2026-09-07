# Squish and Vorbis matching

The original x86 library identifies its codec as libVorbis 1.3.2. Comparing
1.3.1 and 1.3.3 did not improve on that version. Squish 1.11 contains the
same codec sources as 1.10. Keep the existing version pins.

The Android build needs the following integration details:

- Squish is compiled with `-O3`, using its default scalar implementation.
- Vorbis is compiled as C++ with `-O3 -DNDEBUG`. Its exported and internal
  function names in the original use C++ linkage. `NDEBUG` also enables the
  NDK's inline ctype routines.
- Vorbis uses `OggAllocMem`, `OggReAllocMem`, and `OggFreeMem`. The allocation
  wrapper zeroes memory, so the upstream malloc and calloc paths both use it.
- The original has local floor/residue registry arrays in several translation
  units. The Android compatibility patch defines these arrays in the registry
  header, retaining external linkage for the backend export bundles.

The patch changes C++ keyword identifiers and target linkage/integration only;
it does not alter codec algorithms. `-fpermissive` is limited to the external
Vorbis sources to accept their C void-pointer conversions. The public linkage
define propagates to Android consumers. Native builds retain C compilation
and the existing system libraries; WASM retains its separate, newer Vorbis
dependency.

## Validation and remaining work

Against main `3b3e5bb`, the final integrated x86 build increased overall
fuzzy match from **19.808786% to 23.734074%** (+3.925288 percentage points),
and exact report matches from **1,182 to 1,218**. No function declined by more
than 0.01 percentage points in that comparison. The generated matching report
is authoritative for the final committed build.

The full pre-commit workflow passed: formatting, all four repository tests,
Android/native/WASM clang-tidy, the Android build, symbol verification (zero
missing symbols), and matching report generation. Windows validation used the
pinned LLVM 22.1.8 binaries from a local repository override, explicit MinGW
parser target/header paths, and local Python launchers for Emscripten.
No checks or warning settings were disabled.

Isolated instruction reviews found **199/205 Vorbis functions** and **28/36
Squish functions** identical after verified relocation/address differences
were excluded. This classification is separate from the committed objdiff
score and does not change its normalization. The separate Squish PCH
initializer is not included in the 36 library functions.

Vorbis still has real differences in `_ov_open1`, `vorbis_synthesis_headerin`,
`_vp_psy_init`, `_vp_noisemask`, `_vp_offset_and_mix`, and `res1_class`.
Some are float-versus-double differences; enabling single-precision constants
globally improves a few functions but regresses already-matching ones.

Squish still has real differences in `CompressAlphaDxt5`,
`ClusterFit::Compress3`, `ClusterFit::Compress4`, `ColourSet` constructors,
`ComputePrincipleComponent`, and `RangeFit` constructors. The constructors
each have C1/C2 aliases, accounting for eight report entries. Tested broad
compiler options did not bring every function above 95%.

This change is a substantial improvement, **not a claim that every library
function reaches 95% or 100%**. Leave pure relocation differences for the
separate relocation work, and investigate the remaining instruction bodies
without changing the report to hide them.
