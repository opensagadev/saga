# Cutscene stub code generation

The target here is the Android x86 binary built with GCC from NDK r8e. Compare
with the GOT-aware `objdiff-cli` fork; plain byte diffs misidentify addresses
and GOT references.

## Shared matrix update tail

`instNuGCutSceneSetPos`, `instNuGCutSceneTranslate`, and
`instNuGCutSceneRotateY` all finish with the same inlined bounds-center code.
The scene's `bounds` pointer is two `NUVEC` values. Compute each component as
`(bounds[1].component + bounds[0].component) * 0.5f`, store it at instance
offset `0x60`, or store three zero words if the bounds pointer is null. Then
call `NuVecMtxTransform(&instance->transformed_bounds_center,
&instance->transformed_bounds_center, &instance->matrix)`. Keeping this tail
in each function lets GCC emit the target's register and branch layout.

`Translate` initializes the instance matrix only when `flags_88 & 0x80` is
clear, and then always calls `NuMtxTranslate`. `SetPos` and `RotateY` set the
flag and call the corresponding matrix operation unconditionally. The first
batch produced target-sized functions for all three; `Translate` scored
99.98039% while the others scored 89.04444% because GCC hoisted the second
argument load and matrix address before the flag write. This is an instruction
order issue, so avoid changing the arithmetic when revisiting it.

## Bit field writes

`SetRepeat` clamps the signed input at 31, masks to five bits, and replaces
bits 13–17 of the 32-bit word at instance offset `0x88`. This includes the
three flag bytes following `flags_88`, so a byte-only write does not match.
The first implementation scored 98.75% and emitted the target's 46-byte size.

`WaitAtEnd` replaces bit 6 of byte `flags_8c` with `(enabled & 1) << 6`.
The target loads the second argument as a byte before loading the instance
pointer; a direct C expression generated the opposite load order and a 32-bit
second-argument load, scoring 77.77778%. The target is 34 bytes.

## Other signatures

Several placeholders used `void(void)` even though the target takes an
instance pointer or returns a value. `instCutSceneTimeElapsed` returns `f32`,
and `instNuGCutSceneCharGetStartMtx` returns `i32`. The latter copies the
base matrix after a case-insensitive name match and compiled to a 219-byte
100% match in the first batch. `instNuGCutScenePlay` takes a second direction
argument; the first implementation matched 94.111115% at the target's
261-byte size.
