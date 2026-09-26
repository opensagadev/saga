# Episode III lift code generation

These observations come from the original Android x86 `libTTapp.so` and the
GOT-aware fork of `objdiff-cli` against the NDK r8e GCC 4.7 build.

## Cruiser D

`CruiserDInit` has ten separate `sprintf("Tube%d", 1..10)` and
`Tube_FindByName` call sequences, all using one stack name buffer. A source
loop changes the control-flow shape; explicit call sites reproduce the target
unrolling. The target has a 16-byte aligned stack frame and keeps the name
buffer at `esp+0x20`. An aligned local buffer is being measured as a way to
recover that frame and its register choices.

`CruiserDUpdate` checks all eight `Player` slots in eight consecutive blocks.
Each candidate must be unowned (`apiobj.field_0x287 == 0`) and either stand on
the lift platform or lie above its drawn Z position. A successful candidate
receives `ObjHitObj(NULL, victim, -1, 0, 0, 1)` and then
`KillGameObject(victim, 2, 0)`. A helper call or source loop changes these
blocks; expanding the same source body eight times preserves the target's
shape.

The 12-byte `cruiserd_netpacket` carries animation frame, speed, and a flags
word. The lift-chase float at offset `0x28` belongs to
`CruiserD_LiftChase_msg`, not the net packet. This distinction matters both
for correctness and register allocation. The `CruiserD_LiftChase` global is
already defined in `sfx.cpp`, so its declaration here must be `extern`.
Only clients copy the packet back into the lift animation; the local update
records the current animation frame and returns.

On a first reconstruction, GCC placed the local eight-player path before the
client packet-copy path. The target has the client path immediately after the
entry checks and sends the local path to a later block. Marking
`netclient != 0` likely with `__builtin_expect` and correcting the message
write changed the GOT-aware match for `CruiserDUpdate` from 0.0% to
70.54703% (target 1,920 bytes). That probe still copied the client packet on
the local path. Correcting that behavior measures 65.74753%; the higher
number is not the retained implementation. This is a case where a branch
probability and the lifetime of one pointer change the layout of most of a
function.

## Verified empty callbacks

The target implementations of `KashyyykB_Init`, `KashyyykB_Reset`,
`KashyyykA_Update`, `KashyyykB_Update`, `KashyyykC_Update`, `VaderB_Init`, and
`VaderB_DrawPanel` are all 9-byte no-ops. Their `STUBBED()` markers can be
removed without adding behavior; each remained 100% in the GOT-aware diff.
