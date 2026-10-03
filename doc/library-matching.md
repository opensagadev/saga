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

The Android compatibility patch changes C++ keyword identifiers and target linkage/integration only;
it does not alter codec algorithms. `-fpermissive` is limited to the external
Vorbis sources to accept their C void-pointer conversions. The public linkage
define propagates to Android consumers. Native builds retain C compilation
and the existing system libraries; WASM retains its separate, newer Vorbis
dependency.

A separate retail-scalar patch restores individually verified arithmetic:
Squish Vec3/Vec4 Min/Max subtraction-based selects and Vec4 double floor/ceil
arguments with float narrowing; Vorbis residue float scale/integer entropy
multiplication and two local float constants in psy_init. Version pins,
scalar configuration, compiler options, signatures and codec algorithms stay
unchanged. These are local source changes, not a global single-precision flag.
The Squish patch affects the pinned Squish source; native Vorbis still uses
its system library and WASM Vorbis its separate newer source.

The four isolated owners retain every original-backed function and previous
exact match. Same-object proofs cover changed literal/service roles, full
storage, symbol surface, CFI and unscored PIC thunks. GNU64/i386 O3/SSE
ASan/UBSan/LSan diagnostics pass on both old and new sources: 97 Squish cluster
cases, 100 public RangeFit cases, 138 residue classification/storage cases,
and 7,000 exact ATH/noise arithmetic cases per ABI and revision. These finite
diagnostics do not certify audio/gameplay equivalence, the entire codecs,
private target ABI, or unchanged pre-existing psychoacoustic bounds/shift
issues.

The linked scalar follow-up increases overall fuzzy matching from68.827390%
to68.989170% (+0.161780 percentage points). Only the six expected codec rows
change, all positively, retaining all6,293 existing raw exact matches and
the original report denominator. The generated report remains authoritative.

The subsequent PCA follow-up restores seven explicit double libm argument
boundaries, retaining float polynomial arithmetic, local narrowing, and the
widened float1/3 exponent. Only ComputePrincipleComponent improves,
85.914560%→99.924050%; overall matching reaches68.993220%, retaining all
6,293 raw exacts. Same-object ABI/storage/service qualification and four
GNU64/i386 O3/SSE sanitizer lanes pass, each with100 actual ColourSet-produced
cases and6,196 version-specific arithmetic checks. These bounded diagnostics
do not certify arbitrary matrices, complete compression or private target ABI.

The ColourSet constructor follow-up likewise restores a double sqrt argument
and existing float assignment. Both constructor aliases rise90.316580% to
99.969850%; overall matching reaches68.996500%, retaining all6,293 exacts.
Actual producer/Remap diagnostics pass on both revisions and both GNU ABIs:
4,160 cases and282,943 checks per lane, including every initialized q/256
weight for q1..4096. No weight-bit divergence is observed in this finite
catalog; the instruction-fidelity improvement is not a codec-output claim.

## Validation and remaining work

In the earlier integration against main `3b3e5bb`, the x86 build increased overall
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

That earlier audit found real differences in `_ov_open1`, `vorbis_synthesis_headerin`,
`_vp_psy_init`, `_vp_noisemask`, `_vp_offset_and_mix`, and `res1_class`.
Some are float-versus-double differences; enabling single-precision constants
globally improves a few functions but regresses already-matching ones.

The earlier Squish audit found real differences in `CompressAlphaDxt5`,
`ClusterFit::Compress3`, `ClusterFit::Compress4`, `ColourSet` constructors,
`ComputePrincipleComponent`, and `RangeFit` constructors. The constructors
each have C1/C2 aliases, accounting for eight report entries. Tested broad
compiler options did not bring every function above 95%.

This change is a substantial improvement, **not a claim that every library
function reaches 95% or 100%**. Leave pure relocation differences for the
separate relocation work, and investigate the remaining instruction bodies
without changing the report to hide them.
