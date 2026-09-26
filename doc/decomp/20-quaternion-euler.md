# Quaternion Euler conversion

`NuEulerXYZFromQuat` belongs in `src/nu2api/numath/nuquat.cpp`, immediately
after `NuQuatFromEulerXYZ`. The Android x86 target is 549 bytes at `0x284818`
and has the same `-O0` frame and quaternion copy as that source file. Leaving
it in the `-O3` mixed-owner `nucore_plain.cpp` prevents instruction matching.

The four arguments are `NUANG *x, *y, *z, NUQUAT *input`. The routine copies
the quaternion, clamps `-2*(x*z-w*y)` to `[-1,1]`, writes `NuASin` to `*y`,
then computes `*x` and `*z` with `NuAtan2D` using the corresponding doubled
cross products and squared-component denominators.

With the original expression order and one local quaternion copy, the NDK
r8e output matches every instruction except five PC displacements to literal
pool floats (`-2`, `-1`, `1`). The forked GOT-aware report scores 99.96753%.
Those `.rodata` displacements are outside the fork's GOT normalization; they
also account for small residuals in adjacent math functions. Preserve the
floating expression order and the `NUANG` output type when moving similar
math functions between owners.
