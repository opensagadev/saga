# Combat action codegen notes

These notes cover `DeflectPart` in `fighting.cpp` and `DrawRopeCurved` in `rope.cpp` in the Android x86 NDK r8e build. Use the GOT-aware `objdiff-cli` fork for byte matching.

## Deflecting a part

`DeflectPart` copies the old part's 64-byte matrix before `KillPart(part, 0)`. It also saves its owner, flags, type ID, radius field, and source special before that call. The respawned `ADDPART_s` starts as a full copy of `Default_ADDPART`, so a zero-initialized local is not equivalent. The target copies that 0xc8-byte default with `rep movsd`.

The target tests the deflect argument and old owner before constructing the new part. It calls `MakeThrowVector` only when the deflecting object's low flag byte at API object offset `0x1f8` is signed negative and the old owner's byte at offset `0x27c` is `-1`. Otherwise it starts with `(0, 0, 2)` and applies random X and Y rotations. The new part receives `ObjHitObj_Flags(object)` at offset `0x218`, then the old part's update callback at offset `0x1b4` runs, if present. The rumble/camera/block effects run afterward, even if no new part was created.

## Rope curve

`DrawRopeCurved` ignores its `flags` and material arguments and draws through the global `SolidMtl3D`. It returns when point count is at most two. It starts with a line from `start` to the midpoint of `start` and `points[0]`, draws three segments for each consecutive pair of points, and ends with a line to the final point.

Each group of three segments follows a quadratic Bézier curve from the previous midpoint through the current point to the midpoint of the current and next points. The target's optimized coefficients are close to `(4/9, 4/9, 1/9)` and `(1/9, 4/9, 4/9)`, but the six float constants have distinct bit patterns from single-precision rounding. The last curve endpoint also retains tiny nonzero coefficients instead of using a plain midpoint assignment. Using exact target constants matters for the generated loads and arithmetic order. Each draw uses a two-element `NURND_VERTEX3D` array, white color in both vertices, `NuRndrLine3d`, and a null matrix.
