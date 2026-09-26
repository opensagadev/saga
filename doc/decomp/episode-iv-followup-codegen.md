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
