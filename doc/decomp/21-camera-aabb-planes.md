# Camera AABB plane classification

`NuCameraIntersectsAABB` is a 1,627 byte Android x86 routine at `0x2d7ff0`.
Its arguments are a center, an extent, a far clip distance, and a scissor flag;
its result is 0 (outside), 1 (inside), or 2 (intersecting). The original
checks the near and far planes, then the four world space frustum side planes.
It checks the four scissor side planes only for a partial frustum intersection
when the scissor flag is set. On a fully inside frustum result, the scissor
flag returns 1 before the near and far partial checks. These branches are
visible at `0x2d8366`–`0x2d8649`.

Each side plane distance uses the matrix **column** `j` in this exact order:
`center.x*m0j + center.y*m1j + center.z*m2j + m3j`. GCC unrolls all four
distances and comparisons. `NuVecMtxTransform` computes the positive radii
from `AbsFrustrumPlanes` or `AbsScissorPlanes`; it writes only three floats.
The original nonetheless compares the fourth distance with the fourth stack
radius at offset `0xdc`, which is never written in this routine. Preserve the
fourth comparison when reconstructing code, even though it is a source bug.
The scissor pass reuses the same local radius and does not check near/far
planes again.

Write the near radius as `AbsNearPlane.x * extent.x` (and likewise for y/z).
The mathematically equivalent reversed operand order makes GCC 4.7 load the
extent pointer into a register before the plane pointer, changing seven
instructions at the routine's start. The original loads `AbsNearPlane` first.

The matching score for the reconstructed source is pending a target build and
comparison with the forked GOT-aware `objdiff-cli`.
