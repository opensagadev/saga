# Episode IV–VI stub and codegen notes

The Android target in `res/libTTapp.so` contains 73 `STUBBED()` markers across
`episodeIV.cpp`, `episodeV.cpp`, and `episodeVI.cpp` at the start of this pass.
The target symbol sizes range from 9-byte no-ops to multi-kilobyte level
handlers. Use the GOT-aware `objdiff-cli` fork when checking them.

## Confirmed no-ops

`DeathStarBattleDDraw`, `DeathStarBattleDInit`, and
`HothEscapeA/C/D_Reset` are each nine bytes in the target. Their empty C++
bodies compile to the same eight `nop` bytes and `ret`, at 100% match.
`STUBBED()` only printed a diagnostic, so removing it makes these bodies
faithful.

## Common GCC PIC call pattern

The NDK r8e compiler emits a `push ebx`, PC thunk, GOT base adjustment,
`lea esp, [esp-0x18]`, stack argument store, call, GOT global store, stack
restore, `pop ebx`, `ret` sequence for `HothEscapeA_Init` and
`HothEscapeD_Init`. A direct `InitTrooperCannons(world);` followed by
`troopercannons_beenReset = 0;` reproduces the 43-byte target exactly.

`HothEscapeB_Init` uses the same call and store after a `netclient == 0`
branch. Its two `lea` instructions referencing the `snow_mob` and
`snowmob_1` strings are the only remaining differences at 99.6875%: the
local and target constant-pool labels have different GOTOFF displacements.
The control flow, instruction sizes, and remaining instructions match.

`HothBattleE_Update` matches exactly at 62 bytes when the low-end-device
test gates `UpdateMiniSnowTroopers`, then the zero `netclient` test gates
`HothBattleE_UpdateWave`.

## Integer return hidden by cross-file bool declarations

`SarlaccPitDiscoActive` has a target `setne al; movzx eax, al` ending.
Declaring its definition `bool` removes the `movzx` and produces an
83-byte function at 95.83%. Declaring the definition `i32` restores the
86-byte target and reaches 100%. Other files declare it `bool`, but the
return type is not encoded in this C++ function's mangled name, and the
integer result is 0 or 1. Check target return register width before
assuming that a call site's boolean use determines the original return
type.

`SarlaccPitC_Init` emits exactly the target's four 32-bit zero stores;
its remaining differences are local BSS offsets for `power`,
`recharging`, and the two `target_shield` elements. These are numeric
GOTOFF operands in the disassembly and differ as the whole game's BSS
layout changes.

`CloudCityTrapA/B_Init` and `CloudCityTrapC_Reset` can be reproduced with
ordinary C++ calls and field assignments. Their instruction streams and
sizes match (173, 241, and 309 bytes); their residual differences are
string and float literal address operands. Keep exact string values and
argument order even when these address operands do not match.

## Block order in Cloud City Trap updates

`CloudCityTrapB_Update` compiled to 296 bytes and 87.975% when the boss
completion condition was written as an early return. Writing the positive
condition and nesting the completion calls inside it compiled to the
target's 312-byte block order and 99.975%. The only two remaining
differences are the `1.0f` constant address operands.

`CloudCityTrapC_Update` initially matched 0% despite equivalent route
flag logic. The target first loads the path connection, direction, the
`BIGJUMP` and `R2D2GLIDE` masks, and the cleared route flags. It then
computes the active mask from the two obstacles and player position.
Moving the route-flag calculation before the obstacle tests reproduced
the complete 258-byte instruction stream at 99.97183%; only the `-20.0f`
and `-21.0f` constant addresses differ. Preserve this computation order
when reconstructing nearby level handlers.
