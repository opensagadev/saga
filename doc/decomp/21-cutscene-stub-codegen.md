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

## Cleanup and end-state routines

The cleanup list uses 16-byte records containing a handle, a scene pointer,
the instance's accumulated stream duration, and a flag byte. `ResetCleanUp`
aligns the supplied list storage to four bytes and stores eight other
parameters into `Defrag*` globals. `AddCleanUpItem` calls the configured
`DefragGetInstFn`, appends a record, advances the list pointer, and increments
its size. Both functions produced 100% GOT-aware matches (134 and 119 bytes).

`CalculateAverageCentre` uses each 16-byte `instNUGCUTRIGID_s` as a special
handle. It only counts handles for which `NuSpecialExistsFn` succeeds, sums
positions from `NuSpecialGetPos`, and multiplies the sum by `1.0f / count`.
An optional matrix transforms the instance's bounds center rather than the
output average. Null instance, scene, rigid system, or rigid array skips this
transform. The initial implementation scored 92.09574% at 350 bytes versus
the target's 354.

`JumpToEnd` and `JumpToLastFrame` need the existing static rigid and locator
finalization helpers in `cutscene.cpp`, so their definitions live there. They
set `ForcePlayEndFrame` for the duration of finalization, evaluate a reverse
frame when `flags_8a & 4` is set, and reset the camera lock afterward. The
first implementations scored 73.78% and 74.57143%, respectively; both are
four bytes larger than the target, so their block and store order still need
work.

`instNuGCutRigidSysEnd` is a static callee of the jump routines. The source
had `__used__` on this helper, which kept its ordinary stack-argument ABI.
Removing that annotation let GCC optimize the local calls and raised the
jump scores to 79.38% and 78.34694%. This is an interprocedural compiler
effect: a function marker can change its callers even when its body is
unchanged. The helper itself still matches poorly and needs separate work.

## Cutscene memory display

`DisplayCutSceneMemory` is a debug graph, not a no-op. It uses one `NuPrim2DBegin`
call, writes a packed color into `g_NuPrim_StreamBufferPtr->u32_ptr[3]` before
every vertex, and draws cutscene and instance allocation bars. Separate
instance storage uses two list passes and y ranges 206–208 and 209–211;
shared storage uses one pass and y range 206–210. The target converts 32-bit
unsigned byte offsets to `f32` as `high16 * 65536.0f + low16`, which affects
the SSE instruction sequence. The first reconstruction scored 66.41629%
against its 2,000-byte target and removed the final `STUBBED()` marker from
this file. The 2192-byte generated function has further block-order work.

## Remaining cutscene source files

The target's `instNuGCutSceneFind` scans the active intrusive list and calls
`NuStrICmp(input_name, instance->name)`. `PetesHackOfDeath` only clears that
active-list head. Their first implementations score 99.97222% and 99.875%;
the remaining differences are relocation displacements.

`CutScene_OverrideConfigFileName_LSW` checks the active `PODSPRINT_ADATA`
area, confirms the `episodei\\ep1_podrace_` prefix, then appends `_sprint`
for eight case-insensitive suffixes. The target has separate return and
common append blocks; the first version scored 91.416664%.

`instNuGCutSceneCleanUp` uses four passes over the 16-byte defrag records:
destroy unused scenes, destroy old instances, relocate selected scenes in
ascending address order, and recreate instances. The target uses the cursor
returned by `RelocateCutScene` and switches it to `DefragInstBaseMem` when a
separate instance pool exists. The first implementation scored 30.340782%
at 501 bytes against a 594-byte target; its loop block order needs work.

For `instCutSceneTimeElapsed`, declaring `rate`, `accumulated_stream_duration`,
and `current_frame` as locals before the rate test makes GCC load both frame
values before its `jnp` branch. Reordering the two locals changes the chosen
SSE registers. That raised its score from 39.142857% to 53.261906%; the
remaining gap is mostly NaN-aware branch arrangement and zero-return blocks.

`FindMtlInHGObj` lives in an otherwise `-O0` translation unit but the target
function is optimized. A function-local `optimize("O2")` produced an 85.09091%
match while leaving neighboring functions at their measured settings. This
material index is one-based, with zero for no match.

`instNuGCutGetNextRigidInfo` in `gcutscn.cpp` needs its scene and instance
rigid-system pointers live in `%edx` and `%ecx` before testing flags. GCC
otherwise sinks their loads into the final copy block and changes the frame
size. Empty register constraints on these live variables and the rigid-array
address raised the match from 80.27869% to 93.2623%. The remaining five
instruction differences are prologue scheduling. The other reconstructed
`gcutscn.cpp` routines scored 99.76923% (`FindSceneStateObj`), 99.32743%
(`instGetLookAtLocatorInfo`), and 100% (`instNuGCutContainsInstancedRigids`).

An aligned local `VARIPTR cursor __attribute__((aligned(16)))` in
`instNuGCutSceneCleanUp` induces the target's frame-pointer and 16-byte
realignment prologue. This is more localized than changing the calling
convention or TU optimization. It raised the score slightly, to 31.379889%;
the remaining mismatch is loop layout.

`CutScenes_ConfigureList` builds a temporary array of 12-byte records and
copies it into an aligned arena after parsing. Its target reserves a roughly
12 KB frame; two filename buffers and a stack-resident duplicate flag affect
every local offset. Keeping the `cutscene_end` path as fallthrough, indexing
`entries[CUTCOUNT]` directly, and testing that duplicate flag at the loop top
raised the first reconstruction from 29.056538% to 56.222614%. The current
1088-byte body is close to the target's 1101-byte size, but its parser block
order remains a major source of mismatches.

`RelocateCutScene` returns the new scene pointer. The target writes the
relocation delta into the source scene before `memmove`, then reloads the
copied scene's `string_delta` for nested pointer additions instead of
keeping one register copy of the delta. The target tests only the low byte
of `version` and uses null-preserving conditional pointer assignments for
some arrays. Reconstructing those details raised the match from 56.443005%
to 71.04663% against its 1307-byte target. For repeated relocation loops,
watch whether GCC emits `cmovne` or a branch and whether the test uses the
old pointer or the adjusted pointer.
