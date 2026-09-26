# Episode I action code generation

The original `Action_MushroomCollapse` and `Action_SetLapTime` are AI action
callbacks returning `i32` with `eax = 1` on every exit. Their old source stubs
declared `void`, which made even a behaviorally correct body fail to match the
epilogue. The same return convention should be checked in the remaining
Episode I action stubs.

Both handlers test the `first_time` argument before touching state, then walk
the `char **params` array only when `param_count > 0`. A linear `if` /
`else if` chain of `NuStrIStr` calls reproduces GCC's original block order,
including out-of-line blocks for later recognized keys. Keep the key order
from the target disassembly. For each match, pass the character just after the
key to `AIParamToFloat`. The original assembly passes the *third* action
argument to `AIParamToFloat`, although the repository's prototype names it
`AIPACKET_s *` and the callee prototype names that slot
`AISCRIPTPROCESS *`; the source needs an explicit cast to preserve that call.

The original code reads floating values returned in `st(0)`, then uses
`fstp` into a field or a stack slot before integer conversion. Assigning the
`AIParamToFloat` result to a `float` field or casting it to `i32` reproduces
this pattern under the pinned NDK r8e GCC.

The first build of `Action_SetLapTime` matched 99.812% using the GOT-aware
`objdiff-cli` fork. Its ten remaining mismatches are symbol or string address
operands; instruction opcodes, registers, and control flow match. The first
`Action_MushroomCollapse` build matched 99.131%; most remaining mismatches are
address operands, with a few register choices around the float maximum test.

The Episode I handlers `RescueB_Init`, `RetakeG_Reset`, `MaulA_Update`,
`MaulD_Init`, `MaulD_Update`, `MaulE_Init`, `MaulE_Update`, `MaulF_Update`, and
`AnakinsFlightB_Update` have genuine empty target bodies. Each target symbol
is eight `nop` bytes followed by `ret`. Remove `STUBBED()` from these source
bodies; do not invent behavior to fill the padding. The GOT-aware fork reported
100% for each after removing the markers.

`CreatePodRaceMine` is a file-local function with its pointer argument in
`eax`, unlike the normal stack argument convention. Mark it `regparm(1)` and
`noinline` to keep the standalone local symbol and its call shape. The target
uses a `NUVEC` stack local for the candidate mine position, takes a ground
height from `GameShadow`, checks ten possible exclusion areas, raycasts from
the player, and fills the first free entry in the 64-mine array. Its first
reconstructed build reaches 70.845% instruction similarity. The source also
needs the C-linkage `mine_max_tile` data symbol initialized to `0x2000`; it
is placed in this translation unit for now, so its `.data` address differs
from the original. Register allocation and the free-slot loop are the main
remaining code differences.
