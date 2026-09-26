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

## Return width in Hoth Battle initializers

The shared declaration of `BoltType_FindIDByName` currently returns `i8`,
but the three Hoth Battle initializer call sites store its return value as
`i16` without sign extension. Calling through that declaration makes GCC
insert `cbw` after each call, shifting the following blocks and reducing
match. A file-local `i16` declaration with the same assembler symbol
reproduces the target call sites. With that alias, `HothBattleA_Init`,
`HothBattleC_Init`, and `HothBattleE_Init` have the exact target sizes
(466, 631, and 514 bytes) and match 99.76636%, 99.71724%, and 99.837395%.
All residual differences are string or local literal address operands.

## Unrolled level special updates

GCC unrolls a three-iteration loop over adjacent `LevHSpecial` entries.
The `CloudCityEscapeA_Update` target has the three gas animation checks
as separate blocks with shared return logic. Its source needs the desk
visibility check first, surface flags reset next, the gas loop gated by
the panel state, and the BuildIt/AI-message completion check last.
This gives the exact 531-byte target size and 99.912% match. Its seven
remaining differences are string and `1.0f` literal addresses.

`HothEscapeC_Init`, `AsteroidChaseA_Init`, and `CloudCityEscapeA_Init`
also match their exact target sizes (574, 207, and 471 bytes). Each
remaining argument mismatch is a string or constant address; none is a
different instruction or control-flow block. The GOT-aware fork reports
99.5037%, 99.545456%, and 99.42593% respectively.

## AT-AT particle callbacks and Hoth wave setup

`KillParts_ATAT` stores pointers to `AtatPart_Update` and `AtatPart_Stop`
in its particle descriptor. Those functions were file-local in `move.cpp`.
Giving them external linkage with their target `_ZL...` assembler names
and hidden visibility lets the Episode V handler reference the exact
callbacks. Setting the velocity components in z/y/x order and using an
unsigned comparison for `variant < 1` reproduces the entire 120-byte
target at 100%.

`HothBattle_StartNewWave` has an `i32` result (0 while a mini cut camera
is active, 1 after it spawns wave types). The target's block order is a
switch over `melee.field_0x1`: Probe, rider, AT-AT, or a combined wave.
Writing the cases in that order with the low-end count adjustment and a
final `for` over `melee.creature_count` gives the exact 470-byte target
size and 99.75207% match. Its six remaining mismatches are only the
three wave-name string addresses, each referenced twice.

## Dagobah force paths and Boba rocket movement

`DagobahA_Update` first returns if any of the three force gizmos are
missing. GCC keeps the inactive path before the much larger active path
when the active condition uses `__builtin_expect(..., 0)`. The active
path clears bit 31 on both traversal flags of three connections, selects
one of six node-position tables, then updates three path nodes. The
inactive path sets bit 31 on those connections. This layout currently
matches 84.36684% of the 932-byte target. The remaining gap includes
register choices for the third force and node pointers; forcing EBP with
an inline register constraint made the output worse, so prefer source
order and lifetime changes over a fixed-register declaration.

`BobaRocket_Move` has two control-flow arms selected by the sign bit of
`part->active`: target seeking on the nonnegative arm and ballistic
flight plus a debris trail on the negative arm. Load `part->recipient`
before that branch, and share the spin integer and `NUVEC delta` across
the arms. In particular, reusing `delta` for negated trail momentum
produces the target's 0x90-byte stack frame; a separate momentum vector
increases the frame. The target loads a recipient's collision position
in z/y/x order before storing x/y/z, which affects SSE instruction and
register order. These source-order changes raised the GOT-aware linked
match from 68.04601% to 75.38957%. The current generated size is 1498
bytes versus 1612 target bytes, so more control-flow and register work
remains.
