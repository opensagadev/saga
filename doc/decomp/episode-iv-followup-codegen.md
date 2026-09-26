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
