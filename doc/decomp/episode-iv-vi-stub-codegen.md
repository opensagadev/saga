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
