# Episode IV stub reconstruction notes

These measurements use the GOT-aware `objdiff-cli` fork against
`res/libTTapp.so`, with the Android x86 NDK r8e target Bazel build.

| Handler | Current match | Target bytes | Main remaining difference |
| --- | ---: | ---: | --- |
| `BlockadeRunnerB_Init` | 99.645% | 975 | 16 string address operands |
| `DeathStarEscapeB_Draw` | 99.390% | 462 | Two reordered pairs of matrix loads and stores |
| `DeathStarShieldDown` | 92.113% | 379 | Register choices and narrow-socket tail block order |
| `TatooineD_Init` | 84.470% | 262 | Local register and argument preparation order |
| `TatooineD_Update` | 88.129% | 225 | Path node slot reloads, connection load order, unsigned shift |

## Short arrays before repeated calls

The target `BlockadeRunnerB_Init` loads five blowup name pointers onto its
stack before the first `GizmoBlowUp_FindByName` call, then emits five call and
field-update blocks in a row. Five explicit repeated call statements produced
the right operations but placed the blocks after the function's return and
scored **0%** because the block order differed. A local five-element `char *`
array and a five-iteration `for` loop caused NDK r8e GCC at `-O3` to unroll the
loop, preload the names, and match the target's 975-byte layout. This changed
the score to **99.645%**. The remaining 16 differences are string GOTOFF
operands from the current linked `.rodata` layout.

## Stack alignment and frame pointer

`DeathStarEscapeB_Draw` copies a 64-byte joint matrix by value to a local
`NUMTX`, then passes that local to `NuSpecialDrawAt`, twice. The plain source
matched 94.467% at the correct 462-byte size, but used a frame without `ebp`
and allocated `0x54` bytes. Adding `force_align_arg_pointer` reproduced the
target's `push ebp; mov ebp,esp; and esp,-16; lea esp,[esp-0x50]` prologue and
its epilogue. Four load/store scheduling differences remain.

## Signed state and integer return

`DeathStarShieldDown` tests blowup state byte `0x9e` as **signed**. Returning
`i32` and writing the negative-state case as the positive fallthrough restored
the target's main block order. An early-return version with a small loop over
the two players scored 11.062%; explicit first- and second-player checks score
92.113%. The target obtains the player array through one GOT load and handles
the second player's torpedo bit as an integer `0` or `1` return.

## Further Episode IV handlers

| Handler | Match | Target bytes | Current bytes |
| --- | ---: | ---: | ---: |
| `MosEisleyA_Init` | 99.850% | 762 | 762 |
| `TatooineA_Init` | 96.800% | 477 | 477 |
| `TatooineA_Update` | 97.883% | 344 | 338 |
| `DeathStarRescueB_Init` | 99.468% | 566 | 566 |
| `DeathStarRescueC_Init` | 99.459% | 340 | 340 |
| `DeathStarEscapeB_Init` | 99.762% | 336 | 336 |
| `DeathStarEscapeB_Update` | 97.008% | 438 | 438 |

The two constant-bound loops in `MosEisleyA_Init` (five lid groups and eight
lids) unroll completely at `-O3`. Their repeated `sprintf`/find/write blocks
and the single reused 32-byte name buffer reproduce the target's 762-byte
function; the remaining differences are string addresses.

`TatooineA_Update` initially scored 19% with its active branch inline. The
target tests the group-active bit and jumps to the active branch after the
main return, leaving the inactive branch inline. Writing the active case as a
positive `if (__builtin_expect(condition, 0))` and checking the three force
pointers sequentially gave 97.883%. The state byte is set to **1** on
activation and cleared to **0** on deactivation. Reversing this state update
still looked plausible in source but contradicted the target disassembly.

`DeathStarEscapeB_Update` has the same aligned `ebp` frame pattern as its draw
partner. `force_align_arg_pointer` repaired its prologue. The target zeroes
two local animation-data pointers before checking the network state; volatile
local pointer slots preserve those initial stores. It continues to animation
handling after attempted object lookups even if a lookup returns null. An
extra source guard after the lookups added instructions and reduced the match;
removing it helped reach 97.008% at the exact 438-byte size.

For `DeathStarRescueB_Init`, the target has six `blowup_block_N` lookups.
Missing the sixth produced a 534-byte function and 93.833%; restoring it
reached the exact 566-byte size and 99.468%. Check the entire target tail when
a repeated string sequence seems to end at a round count.

## Mos Eisley B setup

`MosEisleyB_Init` matches 99.644% at the target's exact 1,661-byte size. It
uses the same five-name local array pattern as `BlockadeRunnerB_Init` for the
terrain blowups. The target preloads those pointers and unrolls the loop.
It also reuses one stack `direction` output across four path connection
lookups. Remaining differences are linked string GOTOFF operands.

## Final Episode IV stub pass

| Handler | Match | Target bytes | Current bytes |
| --- | ---: | ---: | ---: |
| `MosEisleyD_Init` | 99.820% | 1,798 | 1,798 |
| `MosEisleyB_Update` | 73.049% | 1,011 | 977 |
| `DeathStarRescueB_Update` | 93.714% | 1,168 | 1,170 |
| `KillParts_TIEFIGHTER` | 73.068% | 557 | 590 |
| `DeathStarBattleDUpdate` | 67.338% | 1,502 | 1,514 |

`MosEisleyD_Init` confirms the fixed-array pattern: six local gate-name
pointers are initialized before the first `sprintf`, and GCC unrolls the six
door blocks. Two eight-iteration bin-lid loops also unroll. The function is
the target's exact size and only linked string addresses differ.

`MosEisleyB_Update` has the same branch order as `TatooineA_Update`: check
three force pointers, leave the inactive branch inline, and put the active
branch after the return. The target checks all four path nodes before
choosing among six x/z position layouts and calls `AIPathNodeUpdatePos` four
times with fresh path pointer loads. The current source implements those
paths, but its float-setting block and register order need further tuning.

`DeathStarRescueB_Update` repeats six reactor checks. Explicit macro
expansion preserves the target's six active bodies; a large ordinary loop
risks leaving one loop body where the target has six. The target checks the
six flag bytes in order and places the first active body next to that chain,
then the others out of line. The sound effect uses a 0.25 volume while the
animation plays and a 0.4 volume when it reaches its end frame. Using a
nonvolatile byte pointer for `LevFlag` raises the match from 83.634% to
93.714%: GCC emits the target's direct memory byte compares and the same
`0x5c` stack frame. A volatile pointer instead emits byte loads followed by
tests and chooses a different prologue. Explicit `goto` labels did not alter
the optimizer's remaining reverse body order.

`KillParts_TIEFIGHTER` uses an unsigned `variant < 1` expression. NDK r8e
GCC encodes its initial flags assignment as `cmp 1; sbb; and 0x3f0; add
0x10`; signed comparison or equality is a different instruction sequence.
The spinning path starts with `{velocity.x * 0.75f, velocity.x * 0.75f,
velocity.z * 0.75f}`. The middle component really repeats x. Five particle
callbacks in `parts.cpp` were static, so this pass exposes them with hidden
linkage and their original `_ZL...` assembler names for cross-file pointers.
Marking the `mode == 1` spin branch unlikely with `__builtin_expect` moves
it after the other mode branches and raises the match from 45.947% to
73.068%. A read-only disassembly pass found two remaining source issues:
the target feeds the second `u16` rotation argument into `NuVecRotateX` and
the first into `NuVecRotateY`, and it writes spin `params->flags = 0x111`
before building the velocity vector. These need a measured follow-up.
The target's out-of-line order is mode 0, low-speed, then spin; a shared
`AddPart` tail for the first two and the default path may eliminate repeated
epilogues. Reversing the outer float comparison is another block-order probe.

`DeathStarBattleDUpdate` reads `player->apiobj.collision_position`, which is
at player offsets `0x80` through `0x88`; `apiobj.position` is a different
field. It clamps the target z to 60/62 and 70/72 depending on which of the
outer two ships exist. Three distinct spawn blocks rebuild the same base
position before applying offsets `(0,0,2)`, `(0,1,0)`, `(0,0,-2)`. Its trench
move and kill callbacks also needed hidden cross-file linkage while retaining
the original `_ZL...` assembler names.
For a measured follow-up, put the second ship's model ternary directly in
`AddDynamicCreature` after its offset is constructed; a separate model local
currently moves that test ahead of the spawn setup and spills it. The target
also checks flag byte 4 directly (use a nonvolatile pointer), leaves the
visibility-zero sound path inline, and stores `trenchrun.objects[slot]`
before the spawned ship's final spline-offset field.
