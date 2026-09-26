# Combat action codegen notes

These notes cover `DeflectPart` in `fighting.cpp` and `DrawRopeCurved` in `rope.cpp` in the Android x86 NDK r8e build. Use the GOT-aware `objdiff-cli` fork for byte matching.

## Deflecting a part

`DeflectPart` copies the old part's 64-byte matrix before `KillPart(part, 0)`. It also saves its owner, flags, type ID, radius field, and source special before that call. The respawned `ADDPART_s` starts as a full copy of `Default_ADDPART`, so a zero-initialized local is not equivalent. The target copies that 0xc8-byte default with `rep movsd`.

For a direct GCC 4.7 comparison, `-fomit-frame-pointer -mno-stackrealign` reproduces this function's frameless PIC prologue. Letting the standalone compiler realign the stack introduced `mov ebp, esp; and esp, -16` and shifted every local slot. The source declaration order also matters: save the source special immediately before the radius float to reproduce the target's stack slots. Marking the signed API flag test as expected true with `__builtin_expect(test, 1)` makes GCC put the owner test inline and the thrown-vector path at the tail, as in the target.

For the initial deflection velocity, the target stores `y = 0` before `x = 0`, then `z = 2`. GCC preserves that order here; swapping the two zero assignments removes two instruction mismatches.

An empty `asm("" : "+a"(deflect))` immediately after `KillPart` constrains the deflect argument to EAX. This emits no instruction, but changes the subsequent reload and test to match the target's EAX choice. Place the constraint after the call so it does not disturb the saved values or argument setup.

An empty `asm("" : : "m"(matrix))` at the same point forces GCC to materialize the saved matrix before the saved radius and special pointer. Along with the EAX constraint, this aligns every non-relocation instruction in a direct NDK object comparison.

The order of writes into the copied `ADDPART_s` also controls register choice after the `rep movsd`: write matrix, velocity, special, radius, gravity, type ID, flags, owner, collision callback, then frame time. Writing owner earlier makes GCC keep the object pointer in a different stack slot and moves several later stores.

The target tests the deflect argument and old owner before constructing the new part. It calls `MakeThrowVector` only when the deflecting object's low flag byte at API object offset `0x1f8` is signed negative and the old owner's byte at offset `0x27c` is `-1`. Otherwise it starts with `(0, 0, 2)` and applies random X and Y rotations. The new part receives `ObjHitObj_Flags(object)` at offset `0x218`, then the old part's update callback at offset `0x1b4` runs, if present. The rumble/camera/block effects run afterward, even if no new part was created.

The target zero-extends AX into EAX after `ObjHitObj_Flags`. An ordinary `static_cast<u16>` on the declared 32-bit result produces `and eax, 0xffff` in GCC 4.7. Calling the same symbol through a `u16 (*)(GameObject_s *)` function pointer makes GCC emit `movzx eax, ax` and still a direct call. This return-width quirk is worth checking whenever a target zero-extends a call result.

## Rope curve

`DrawRopeCurved` ignores its `flags` and material arguments and draws through the global `SolidMtl3D`. It returns when point count is at most two. It starts with a line from `start` to the midpoint of `start` and `points[0]`, draws three segments for each consecutive pair of points, and ends with a line to the final point.

Each group of three segments follows a quadratic Bézier curve from the previous midpoint through the current point to the midpoint of the current and next points. The target's optimized coefficients are close to `(4/9, 4/9, 1/9)` and `(1/9, 4/9, 4/9)`, but the six float constants have distinct bit patterns from single-precision rounding. The last curve endpoint also retains tiny nonzero coefficients instead of using a plain midpoint assignment. Using exact target constants matters for the generated loads and arithmetic order. Each draw uses a two-element `NURND_VERTEX3D` array, white color in both vertices, `NuRndrLine3d`, and a null matrix.

The first copy from the second vertex to the first updates only the position. After each of the three curve draws, the target copies all `0x24` bytes of the vertex, including normal, color, and UV fields. Using full vertex copies at all four sites adds a larger stack frame and extra instructions; using only position copies at all four sites omits target instructions.

Splitting the previous midpoint into scalar X, Y, and Z values, with only Y marked `volatile`, improves the direct NDK object match from 65.64% to 70.99%. The Y spill frees an SSE register and brings the loop's X register closer to the target. Keeping a separate `next` pointer for midpoint reads while indexing the current point raises it to 71.42%, with the target's `0xcc` stack frame. Volatile on both Y and Z lowers the score again; register pressure depends on the precise scalar lifetime.
