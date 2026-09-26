# Legacy hierarchy animation blend

`NuHGobjEvalAnimBlend` is 777 bytes at `0x2ce450` in the original Android x86
binary. It evaluates two legacy animation chunks, optionally maps procedural
joint overrides, blends each joint's curves, then concatenates its local
matrix with its parent's world matrix. The input animation format stores a
chunk pointer array at offset `0xc` and each chunk stores curve set pointers
at offset `0x8`.

The missing-curve path is a useful GCC 4.7 matching pattern. The original
chooses a `NUMTX *` pointing either to the evaluated local matrix or directly
to the object's bind matrix. The root case copies from that pointer into the
output in one set of 16 scalar word moves. Copying the bind matrix into a
temporary first emits a second set of moves and grows the routine from about
777 to 1,049 bytes. Keep the selected pointer through both the root copy and
the parent matrix multiply.

The original uses `u8` induction variables for both override and joint loops.
It sets only `scales[255]` to `{1,1,1}` before evaluation, preserving scale
values written by earlier joints. It does not guard either animation pointer
against null. These details distinguish this legacy path from ANI4/ANI5
evaluation and affect the generated branches and stack layout.

The source was syntax checked with NDK r8e GCC 4.7. A linked target build and
forked GOT-aware `objdiff-cli` comparison are still pending.
