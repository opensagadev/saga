# Gameplay regression audit

Initial report: character switching, cantina AI movement, object/character
collision, pickup/build audio, Force availability glow, and incorrect level
object state (completed cantina builds and a blue minikit in story mode).

These are open regressions. A reconstructed function or passing build does
not establish that a reported gameplay issue is fixed.

## Force availability rendering

### Current selection-path audit

The 2026-09-08 native diagnostic `/tmp/saga-glow-natural.gdb` observed
127 `ForceGlowCode` calls for player zero, zero candidates and zero positive
opacity samples. `DrawForceGlowSprite` was not reached. This used the ordinary
scripted route without injecting game state; it hit the 30-second harness
timeout at stage 7 and exited 1. No sanitizer diagnostic was reported. This
is limited route coverage, not a successful gameplay test or proof that a
nearby eligible lamp was rejected.

The original compiler names the glow update
`_ZL13ForceGlowCodeP12GameObject_si.part.34`; the current compiler emits the
unsuffixed local name. For comparison only, `objcopy --redefine-sym` on a
copy of the reference in `/tmp/saga-force-glow-reference.so` gives **98.934%**
with equal 1,428-byte bodies. The original repository binary is untouched.
Remaining differences include relocated operands, a saved special pointer,
a radius reload and instruction ordering; this is not an exact match.

The upstream `GizForce_FindBestForceTarget` compares at **15.635%**, with a
711-byte current body versus the original 2,121 bytes at `0x1ca5a0`.
Its current simplified nearest-target loop needs original-binary recovery
before changing selection behavior. Evidence is in
`/tmp/saga-force-target-current.diff` and
`/tmp/saga-force-glow-normalized.diff`.

The subsequent recovery replaces that loop with the original capability and
group filters, animation-object candidate list, previous-target restriction,
oldest-check selection, LOS ray/platform tests and cached visibility filtering.
The original local `possible_forcetargets` array contains 384 16-byte records.
The player LOS cache is 396 words: 12 visibility words and 384 update counters.
The function returns zero in EAX; both its definition and caller declaration
now return `i32`. The busy field at Force offset `0x3c` is tested as raw 32-bit
bits, matching the original integer test. No optimization changes or Ghidra
were used.

This recovery compares at **33.007%**, 1,974 current bytes versus 2,121 original
bytes (`/tmp/saga-force-selection-loop.diff`). Substantial instruction and
control-flow layout differences remain; this is not a fully matched function.
Target and native builds succeed and all four repository checks pass
(`/tmp/saga-force-selection-{target,native,checks}.log`).

The native GDB fixture `/tmp/saga-force-selection-runtime.gdb` moves only the
diagnostic player collision position/facing near each of the seven visible
Cantina objects and invokes selection. All seven select successfully:
`Light_5`, `Light_6`, `Light_8`, `cup0`, `cup15`, `cup17`, `cup21`.
The player's real LOS cache is present; each call returns zero, and no
sanitizer diagnostic occurs. This controlled test does not prove natural
acquisition or visible glow.

The follow-up placement fixture did not reach `DrawForceGlowSprite`; the
longer run encountered the previously recorded AI angle sanitizer diagnostic.
Ten early samples in `/tmp/saga-force-selection-glow-state.log` show active
glow model 221, zero target/opacity and zero terrain-contact flags while the
player falls after placement. No glow state was injected. A grounded,
appropriately facing runtime test is still required before claiming a visual
fix; the early airborne samples are not evidence of a rendering failure.

The grounded follow-up `/tmp/saga-force-glow-grounded.gdb` preserves the
player's ground height during placement near `Light_5` and holds the diagnostic
facing angle at zero on entry to `ForceCode`. It does not inject contact flags,
targets, glow opacity or Force input. The ordinary game update selects the
lamp, sets contact flags and calls `DrawForceGlowSprite` through
`DrawParaphernalia` while character context is `-1`. Model 221 is active,
radius is 0.208661 and initial draw alpha is 0.169857. This fixture exits zero
without sanitizer diagnostics.

The corrected capture fixture `/tmp/saga-force-glow-capture.gdb` completes
120 update samples with exit zero and no sanitizer diagnostics
(`/tmp/saga-force-glow-capture-clean.log`). It reaches alpha 1. The inspected
capture `.work/capture/run_576131569/window_1746.ppm` visibly shows the green
availability glow around the wall lamp. This establishes rendering in the
controlled grounded/facing setup, not a complete unassisted gameplay test.
The first capture attempt also showed the glow but stopped on a debugger
printf dereferencing a cleared target; that was a fixture error, not a game
crash, and is superseded by the clean run.

An explicit split of cached and uncached final-selection loops reduced
matching to 31.312% (1,992 bytes). It was reverted. The restored target build
succeeds and the function remains **33.007%**, 1,974 bytes
(`/tmp/saga-force-selection-restored-target.log` and
`/tmp/saga-force-selection-restored.diff`). No behavior workaround was retained.

The original `DrawForceGlowSprite` is at ELF address `0x1602f0`
(`0x1702f0` in the Ghidra program rebased by `0x10000`). Its symbol is
`_Z19DrawForceGlowSpriteP7nuvec_sfifP12GameObject_s`.

The source routine was empty. The recovered implementation in
`src/legoapi/render/core/render.cpp` follows the original camera-distance
offset, perspective scale correction, two rotations (20 and 16.777 seconds),
level-object availability tests, alpha draws, and shadow rendering state.
The original stack places the scale at a 16-byte boundary and realigns the
matrix stack frame. Its rotation operations are expanded in the function.

The original callers are `DrawGameObjectsDraw` and `DrawParaphernalia`.
Their ordinary Force-glow branches have been recovered. The latter remains
largely unfinished, including its separate context `0x22` effect; neither
caller is claimed to be completely matched.

The latest measured sprite routine match is **92.658%**, not 100%.
Remaining differences include register allocation, instruction ordering,
the first rotation's table loads, and stack-address reuse. Continue comparing
the actual binary; this percentage is a snapshot rather than an acceptance
threshold. The initial sprite recovery had no visual validation; the later
controlled lamp capture is recorded above.

## Character switching

`CanPullLevers` (`0x46b150`, 59 bytes) now returns the original model-flag
capability result, at **99.882%** matching (relocated operands differ). Its
integer-return declaration and definition replace the void stub; the definition
lives with the player capability helpers at the original optimization level.
This supplies another dependency of `InitPlayerAI`.

### Shared character reset recovery

`ResetPlayerMoves` (original `0x0fe940`, 328 bytes) was empty. Its recovered
sequence resets the player packet, movement vector and toggle hold time,
selects the idle animation, resets animation/character data, clears flicker
and coin state, and resets draw offset. It compares at **71.592%**; remaining
differences are instruction ordering, register allocation and relocated
operands. Its `SetFlicker` dependency (`0x0fc580`, 26 bytes) now matches **100%**,
writing the recovered timer at `+0x1024` and clearing three bits at `+0xe26`.
Both field offsets have ABI assertions. These were recovered with binutils
and `objdiff-cli.py`, without Ghidra or added gameplay guards.

The native sanitizer fixture `/tmp/saga-resetmoves-runtime.log` reached an
active Cantina at game time 3.015376 after 22 normal reset calls. The player
had flicker time zero and flicker flags zero; no sanitizer error was reported.
This is initialization coverage, not proof of free-play switching:
`Player_ToggleCharacter` and its `NewPlayerCharacter` dependency were
subsequently recovered as documented below; complete matching remains open.
Target/native builds and all four repository checks pass for the recovery.

`ResetPlayerAI` (`0x0fc5a0`, 384 bytes) is also recovered, at **96.507%**.
It clears the original navigation/action fields and 24-byte path cursor,
resets route and animation-override sentinels, and calls
`AISysGetCharacterPathPos(WORLD->ai_sys, object, packet, 0xff, 1)`. Remaining
differences are relocated operands and the placement of the `+0x109c` zero
store. Newly exposed fields retain the original byte/dword write widths.
Target/native builds and the four checks passed. The sanitizer trace
`/tmp/saga-resetai-runtime.log` observed one normal reset and reached active
Cantina gameplay at time 3.015691 without a sanitizer error. This does not
establish that all AI movement is correct.

`InitPlayerAI` (`0x0fc720`, 1416 bytes) now includes the original route-mask,
capability, movement/action-state and collision-state initialization before
calling `ResetPlayerAI`. It is a **42.085% partial match**, with instruction
ordering, branch layout and constant-loading differences still under review.
The definition moved from the unoptimized placeholder `legoai.cpp` to the
`-O2` player helpers beside the reset; effective compile commands were checked
with Bazel aquery. No optimization overrides were changed. Newly exposed
padding fields have ABI offset assertions, and existing integer aliases for
float sentinels remain available to their current callers.

The original eight-byte writable data symbol `_0xffffffffffffffff`
(`0x667e18`, all bits set) is restored and used for the unrestricted route
mask, instead of embedding two immediate constants. Restoring this load and
the original valid-route branch order improves the initializer from 36.636%
to 42.085%; `nm` verifies the symbol's eight-byte `.data` definition. This
does not establish a fix for the destination-processing runtime failure.

Target/native builds and four repository checks pass. The native fixture
`/tmp/saga-initai-runtime.log` observed 22 initializer calls and active Cantina
at time 3.012280, but also reported an invalid lookup index in `nutrig.cpp`.
That run is **not a sanitizer pass**; the angle-calculation failure is being
traced before claiming gameplay improvement. No character-switching or NPC
movement fix is established by this initializer recovery alone.

The longer fixture `/tmp/saga-initai-ubsan-long.log` reproduces the failure in
`MovePlayer`'s AI input normalization (`move.cpp:352`). A read-only breakpoint
trace (`/tmp/saga-initai-infinite.log`) captured deltas 0.00057220459 and
0.000133514404, `NuFsqrt` returning zero, and both normalized inputs becoming
infinite. The original `NuFsqrt` also uses the 1e-6 cutoff (rodata `0x57b7e0`),
and original `MovePlayer` at `0x102100` performs the same reciprocal-distance
normalization. Therefore changing the cutoff or adding an angle guard would
not recover the original behavior. The upstream AI destination/stop handling
needs investigation; the initializer is not yet validated for gameplay.

Follow-up comparison of `AISysProcessCharacter` found its radius constraint
checks packet `+0x180` (`0x3f8df1`), not the adjacent movement-target pointer
at `+0x184`. That access and the original left-to-right addition of mover
height, clearance and stopping distance are restored. Matching improves from
**44.885% to 51.040%**. Target/native builds and all four checks pass.
The read-only fixture `/tmp/saga-ai-radius-runtime.log` still captures infinite
angle inputs for NPC character 170, with movement mode 0, stop byte 0, mover
height 0.15, stopping distance 0 and no movement target. These corrections do
not establish a fix for the NPC failure; path/arrival processing remains under
investigation.

The next arrival trace (`/tmp/saga-ai-arrival-all.log`) identifies movement
mode **1** (`AIMoveToDestination`), not wander. The residual is already present
at `AISysProcessCharacter` return with `process_ai=1`. The current path node
has a nonzero radius (2.44486) and four connections; the failing destination
differs from that node's position. Both character IDs 170 and 202 have failed.
This directs further recovery toward destination/goal-range handling, rather
than assuming an empty node or a character-specific movement fault.

Original `AIMoveToDestination` at `0x3f6010..0x3f607c` checks the requested
range and a strict (-0.5, 0.5) vertical interval before the position-reset branch
at `0x3f6def`. Its current same-connection shortcut bypasses this handling.
An isolated source candidate adding that branch still reproduced the angle
failure (`/tmp/saga-arrival-candidate-runtime.log`) and lowered the partial
match from 11.209% to 10.181%. It was removed, along with an unsuccessful
store-order experiment; the original 11.209% score and target/native builds
were restored. The candidate is not a retained fix. The surrounding original
destination adjustment and goal-range logic still require recovery.

A lighter breakpoint fixture now captures the failing character 170 request
with `movement_parameter=0`, `fallback_stopping_distance=0`, and identical
current/destination connection pointers (`/tmp/saga-ai-range-light.log`). The
original range-arrival branch at `0x3f601b..0x3f6024` skips a zero range, so
restoring that branch alone cannot resolve this captured case. The fixture
stopped at infinite `NuAtan2D` inputs; its subsequent position-print command
failed because `GameObject_s` has no member named `api`. It is diagnostic
evidence, not a successful gameplay run. The preceding per-call trace timed
out without capturing an arrival and supplies no additional result.

Original `MovePlayer` at `0x101de8..0x101e34` gates this path on the same stop
byte, context and exact-zero horizontal deltas. Its normalization at
`0x10214f` uses a direct reciprocal. Original `fxyd` also uses `cvttss2si`
followed by a scaled table read (`0x28f4cf..0x28f4d3`), with no finite-input
guard. Consequently the sanitizer observation alone does not establish that
the arithmetic differs from the original. Continue tracing movement request
and stop-state production; do not add a cutoff or angle-table workaround.

The stop-byte write audit identifies a control-system selector, not an arrival
flag: original `Action_SetControlSystem` clears GameObject offset `0x3fc` on
activation, sets it for a case-insensitive `rotational` parameter, and returns
1 even when inactive or the object is absent. Its empty return-0 stub is now
reconstructed in full from `0x184940..0x1849d4`, matching **96.872%** at the
original 149-byte size. Target/native builds and all four checks pass. An
isolated native fixture confirms inactive state 7 remains 7, `RoTaTiOnAl`
sets state 1, zero parameters clear it to 0, and a null packet returns 1;
all calls return 1 and the fixture exits without sanitizer errors
(`/tmp/saga-control-system-runtime.log`). Natural Cantina usage and the
movement regression remain unverified by this isolated action test.

Natural follow-up tracing reaches the same invalid-angle inputs without any
`SetControlSystem` call beforehand. The failing NPC runs script `party`, state
`GoToIdleLocator`, action `GoToLocator` with parameters `WALK`, `mintime=5`,
`maxtime=15`. It is holding on that action with roughly 12 seconds left, zero
range/stopping distance and identical current/destination connections. The
requested locator is at y=0.01 while the actual character is on y≈0; the tiny
horizontal residual is preserved in `movement_position`. Both read-only
fixtures exit at the breakpoint (`/tmp/saga-ai-control-natural.log` and
`/tmp/saga-ai-active-action.log`), not after a successful gameplay interval.

Original `GoToLocator` reads global `ai_moveradius` in its reach calculation
at `0x3f3086..0x3f3095`; replacing the hard-coded 0.1 restores that load and
improves the partial match **38.577% to 39.207%**, retaining 2196 current bytes.
Target/native builds and all four checks pass. The global defaults to 0.1, so
this correction is not evidence of fixing the captured failure. Further
recovery should follow this identified action and its timed locator hold.

The full tail disassembly confirms the original keeps issuing movement while
the locator timer counts down (`0x3f2fda..0x3f3013`, then
`0x3f30e1..0x3f3116`); stopping requests during that interval would not match.
Its timer setup uses the same random interval and 0.01 fallback currently
implemented. A separate parser discrepancy remains: `name` and `teleport`
both consume the following locator token (`0x3f3249..0x3f3273`), whereas the
current parser skips standalone `teleport`. A candidate restoring that branch
and the original personal-name guard reduced matching to 19.312%; it was
removed. Rebuilding restores **39.207%**, 2196 bytes. No parser candidate is
retained, and this discrepancy does not affect the captured WALK/mintime/
maxtime parameter list. The original timed hold is not itself evidence of
the Cantina regression's cause.

`AIMoveDirectlyToDestination` was checked in full: **99.978%**, with only the
GOT-base relocation differing. It needs no behavior change. An isolated
teleport-parser candidate also lowered `GoToLocator` to 17.621% and was
removed; the restored target builds successfully.

An isolated 32-bit harness now executes the original `NuAtan2D` bytes at
`0x28f936` directly from mapped ELF load segments. The audited call chain
uses only internal PIC calls and GOTOFF data, with no imports or constructors.
Finite controls return 8192 for (1,1), 16384 for (1,0), 0 for (0,1), and
40960 for (-1,-1). The captured (inf,inf) input returns **0** without crashing;
(-inf,inf) returns 0 and either negative-y infinite pair returns 32768.
Harness/source: `/tmp/saga-original-angle.c`; results:
`/tmp/saga-original-angle.log`, exit 0. This confirms that the sanitizer
failure differs from the original machine-code behavior: its NaN-to-integer
conversion and scaled 32-bit table addressing wrap to the first entry.
It does not prove the original game naturally produces these inputs or that
Cantina movement is correct. No source angle guard or cutoff was added.

A subsequent current-build Cantina tagging fixture extends the earlier
transfer-only check to 180 rendered frames (`/tmp/saga-tag-sustained.log`).
It places an eligible Obi-Wan within the original search radius, supplies
tag input, and then resumes ordinary updates. Player slot 0 and the controller
remain assigned to Obi-Wan, who renders and responds to movement input.
However, this is not a sanitizer-clean run: the angle-table error also occurs.
A second fixture stops at the diagnostic (`/tmp/saga-tag-angle.log`) five
draws after transfer. `MovePlayer` is processing a different NPC, with
`accepts_player_input=false`, deltas `(-0.000738143921, 0.000556945801)`,
distance zero and reciprocal infinity. The failing call comes from the NPC
pass of `UpdateGameObjects`, not the newly controlled character. These
observations establish sustained control after the controlled tag transfer
and keep the separate AI normalization investigation open; they do not prove
natural target acquisition or correct Cantina movement.

`AICreatureResumeScript` (`0x0fd3f0`, 139 bytes) was another empty handoff
dependency. It now calls `AIScriptProcessorInit` with the existing base script
and that script's base state, then clears the active-reference count at
processor offset `0x64`. A null base script leaves the object alone. The
function now lives with creature initialization in `characters.cpp`, retaining
that source's existing `-O2` setting; no optimization override was added.
It compares at **84.172%**, at the original 139-byte size, with register and
argument-store ordering differences still present. Target/native builds and
all four repository checks pass.

The first runtime fixture incorrectly assumed the initial player had a base
script and stopped on a debugger null dereference, not a game crash. The
corrected controlled fixture resumes the scripted Obi-Wan object after tag
transfer: the base script is preserved, its active state returns from
`0xd0ef4f70` to the script's base state `0xd0ef47b0`, and active-reference
count is zero (`/tmp/saga-resume-runtime-fixed.log`, exit 0, no sanitizer
diagnostic before fixture completion). This checks script resumption, not
resolution of the separate movement normalization issue.

The former `CalculateIntersection` implementation substituted the shared
path node for both corner diversions. It is replaced by the original
connection-pair cache, four endpoint arrangements, corridor test and mirrored
right/left boundary intersections. The obsolete static placeholder in
`ai_sys.cpp` is removed. The real helper stays out of line, matching the
original caller convention, and compares at **86.810%** (831 bytes versus
823 original). `CalculateRightIntersection` reconstructs the original
0.05 radius margin, clamped node radii, inline angle approximation, rotated
boundary segments, segment intersection and height interpolation. It compares
at **83.278%**, with the original 1588-byte size. A temporary target copy
normalizes its compiler `.isra.0` suffix to original `.isra.50` for comparison;
the reference remains unchanged. Both functions remain partially matched.

An isolated 32-bit harness executes the original right-intersection bytes
from mapped ELF segments (`/tmp/saga-original-intersection.c`). It resolves
only the audited `sqrtf`/`sin` imports to local libm, relocates the sine-table
pointer, and calls original `NuTrigInit` before testing. Initial harness
failures were unresolved imports/table initialization, not game crashes.
Four right-angle cases with different node radii, including clamping below
the collision margin, produce coordinates agreeing with the reconstructed
native helper within `1e-6`. A fifth straight-corridor case rejects the
intersection and preserves the output sentinel in both implementations
(`/tmp/saga-original-intersection.log`, `/tmp/saga-intersection-runtime.log`).
This checks geometry against original machine code, with local libm and host
floating-point differences as limits; it is not full byte equivalence.

Target/native builds and all four checks pass. The current Cantina integration
fixture retains tagged-player control for 180 rendered frames and responds to
movement input, but still reports the known NPC angle-table sanitizer error
(`/tmp/saga-intersection-integration.log`). Full movement correctness remains
open, including the incomplete destination solver and formation updates.

`GetNextConnection` now reconstructs the original general-matrix selection,
route membership and boundary masks, mapped route lookup, and nearest-exit
distance selection. The obsolete private placeholder is removed from
`ai_sys.cpp`; the destination solver calls the recovered helper in place of
its general-matrix-only lookup. The helper compares at **35.790%** (905 bytes
versus 849 original), and `AIMoveToDestination` improves from **11.209%** to
**12.588%**. These remain partial matches; the full solver still needs recovery.

An isolated ELF mapping fixture executes the original helper at `0x3ed4a0`
using its original private register calling convention. Eight cases cover
general selection in both directions, an unavailable matrix entry, route
membership rejection, mapped route selection, boundary fallback, an outside
goal with no exits, and a null goal. Native GDB calls agree on both the selected
connection and the direction output, including rejection after direction was
already updated (`/tmp/saga-original-nextconnection.log`,
`/tmp/saga-nextconnection-runtime.log`). These cases do not exercise the
nearest-exit distance calls or prove ordinary NPC movement. Target/native
builds and all four repository checks pass.

`AIPathNodeDistanceToPathNode` now follows the original cache eligibility
(`route == 0xff` and no excluded mask), mapped-route traversal and recursive
exit search. Its previous general-matrix traversal incorrectly reused cached
distances with exclusions and omitted the route matrix. The original terminates
on a repeated connection and returns the distance accumulated so far; it does
not impose the former node-count iteration limit. Null connections return
without filling the newly selected cache slot, whereas unavailable matrix
entries cache `FLT_MAX`. These distinctions are preserved rather than adding
fallback guards. The function improves from **8.377%** to **46.753%** (1252
bytes versus 1358 original), with its C ABI and source optimization unchanged.

The same isolated 32-bit fixture runs against original machine code at
`0x3ecf50` and the native reconstruction. All 13 return values and both cache
slots agree: normal traversal, two cache hits, exclusion bypass, mapped route,
recursive exit, repeated connection, null connection, missing route matrix,
membership rejection, invalid mapped index, no exits and unavailable entry
(`/tmp/saga-original-distance.log`, `/tmp/saga-distance-runtime.log`). The
fixture does not establish complete byte equivalence or full NPC correctness.
Target/native builds and all four checks pass. The sustained Cantina tagging
fixture retains player/pad ownership through 180 rendered frames and movement
input (`/tmp/saga-distance-integration.log`). The earlier NPC angle-table
sanitizer error does not recur in this run; this bounded, timing-dependent
observation does not close the movement regression.

The next cross-graph recovery exposes the original packet bytes at `0x13e`
and `0x13f` as diversion search cursor and selected node. `AIMoveFindDivertNode`
checks four candidates per call, retaining a closer previously selected node.
The special-route branch uses the link table and can stop when its cursor
returns to the starting entry. Entry validation uses `>= count`, while the
original increment wrap uses `> count`; neither condition is silently changed.
An isolated fixture allocates the extra indexed entries and compares eight
cases over three calls each. All 24 cursor/selected-node states agree with
original machine code (`/tmp/saga-original-divert.log`,
`/tmp/saga-divert-runtime.log`). This does not verify allocation assumptions
for every loaded path.

The destination solver now prepares the selected node's position and first
connection when its goal is on another graph. It clears the fallback path
info, selects the original endpoint direction/distance, sets zero stopping
distance, and computes `NuFmax(node.radius - 1, 1)`. Four controlled snapshots
agree with the equivalent original `AIMoveAdjustDestinationPath` preparation
for both endpoint directions, the radius floor and an isolated node
(`/tmp/saga-original-prep.log`, `/tmp/saga-prep-runtime.log`). The native
fixture stops at the existing missing-current-connection guard after
preparation; it does not exercise the complete solver.

`AIMoveToDestination` improves from **12.588%** to **15.483%**. The new private
search helper has the expected scalar-replaced calling convention and 1267
bytes versus 1299 original, but still reports **0%** under objdiff's alignment
score. A temporary target copy normalizes `.isra.1` to original `.isra.26`;
the reference remains unchanged. Behavioral fixture agreement is not a claim
of matching assembly. Later exit-path selection and restoration of the saved
goal remain incomplete, so cross-graph movement is not considered fixed.
Target/native builds and all four checks pass. The Cantina fixture retains
tagged-player control for 180 rendered frames without an ASan/UBSan report
in this run (`/tmp/saga-divert-entry-integration.log`); it does not establish
that cross-graph transition code executed.

The first local commit exposed a code-generation regression: the extra
endpoint-count rejection in the incomplete caller caused GCC to classify
`GetNextConnection` as cold and emit it in `.text.unlikely` at 763 bytes.
The original solver directly indexes its connection endpoints without that
rejection branch. Removing the unsupported branch restores the helper's
905-byte `.text` implementation and **35.790%** match, while improving the
caller to the score above. No hot/cold attributes or optimization overrides
are used to force this result.
After this correction, all four preparation snapshots still agree with the
original, the four checks pass, and the 180-frame Cantina fixture completes
without an ASan/UBSan report (`/tmp/saga-divert-flow-prep.log`,
`/tmp/saga-divert-flow-integration.log`).

The subsequent recovery adds `AIMoveChooseExitNodePath` and
`AIMoveAdjustDestinationPath`. The selector chooses the closer connection
endpoint using distance squared minus node radius squared (ties select the
second endpoint), prefers the requested graph when it belongs to the linked
group, and otherwise compares that group's exit nodes. It preserves the
original retained distance when a candidate graph has no exit nodes.
Seven isolated cases agree with the original machine code on return value
and selected graph (`/tmp/saga-original-exit.log`,
`/tmp/saga-exit-runtime.log`). The adjustment helper's four destination-state
snapshots also agree (`/tmp/saga-adjust-runtime.log`). These helpers compare
at **51.962%** (615/690 bytes) and **25.845%** (332/460 bytes), respectively,
after normalizing compiler `.isra` suffixes in a temporary target copy.

The caller now handles a missing destination connection on the current graph
as the original does: it prepares a diversion. Arrival with a nonzero movement
parameter, no supporting platform, vertical difference strictly between
`-0.5` and `0.5`, and distance squared below the parameter squared stops at
the object's position for an ordinary destination. For a diversion with
navigation bit 2 set, it restores the saved goal, chooses an exit graph and
adjusts the destination there. Four early-return cases agree with the
original full solver (`/tmp/saga-original-arrival.log`,
`/tmp/saga-arrival-runtime.log`), including a tiny residual and the original
squared negative-parameter behavior. The zero-parameter NPC case is not
covered by that arrival condition and remains open.

Removing unsupported post-preparation path-equality/node-array rejection and
its extra blocked-flag write restores the original three null checks and
owner-position return. `AIMoveToDestination` now compares at **17.761%**;
`GetNextConnection` retains **35.790%**. These remain partial reconstructions:
the second exit-selection branch, supporting-platform arrival alternative,
and final stopping-distance adjustment still need recovery. Controlled
helper agreement does not establish complete cross-graph gameplay.
Target/native builds and all four repository checks pass. Repeated arrival
and exit fixtures retain all eleven expected results. The Cantina test
retains player control for 180 frames but reproduces the known NPC angle-table
UBSan error (`/tmp/saga-arrival-integration.log`); it is not sanitizer-clean
and does not close the AI movement regression.

A native build-sound inventory confirms event 0x3a resolves to `MK-Pickup`
(SFX 50, sample 357, 22050 Hz, enabled) and event 0x3b to `LegoForm` (SFX 128,
sample 434, 11025 Hz, enabled and looping). Both have volume 16383. The
read-only fixture exits successfully (`/tmp/saga-build-sound-inventory.log`);
loaded metadata alone does not establish audible playback. Original
`GizBuildIts_LateUpdate` confirms those event numbers at `0x4cfd60` and
`0x4d0908`; do not replace them based on their names. It also has an event
0x3c call at `0x4d0389`, guarded by buildit offset 0x82 bit 2, whose surrounding
update branch still needs comparison with the reconstructed build logic.
`GameAudio_PlaySfx` matches 99.900%; `GameAudio_PlaySfxById` matches 99.446%
with equivalent two-position dispatch. These checks direct the next build
sound investigation toward update-call coverage and actual voice playback.

The event-0x3c branch is part of loose-piece hopping, not the step-completion
call. Original `GizBuildIts_LateUpdate` subtracts FRAMETIME from a positive
shared `gizhopsfxwait` (`0x12a2e20`) at entry, and from per-piece animation-data
offset 0xc0 when processing that piece. When the piece timer reaches zero and
the shared cooldown has expired, interaction bit 0x04 at buildit offset 0x82
causes playback; otherwise `qrand() <= 0x7fff` selects playback. The call uses
event 0x3c and buildit position at offset 0x2c. The cooldown resets to
`qrand() * (1/65535) * 0.2 + 0.1` in the original instruction order.
Before recovery, the late-update routine omitted this entire per-piece
transform/hopping path and matched **5.600%** (1795 current bytes versus
6532 original). The current reconstruction restores this animation path
together with its sound branch and shared cooldown.
Assembly evidence: `/tmp/saga-build-update-original.asm`, especially
`0x4cf5e6..0x4cf607`, `0x4cfea8..0x4cfebd`, `0x4d0358..0x4d03ce`, and
`0x4d0775..0x4d078d`.

The hopping path also requires `NuSpecialGetOnScreenFn` (`0x2c8d30`, 38
bytes), previously an empty void stub. Its complete body and integer-return
declaration are restored in the existing O3 special-query module, removing
the stub from nucore. It matches **100%**. With a valid handle, absent scene
or absent legacy special returns 1; otherwise it reads legacy-instance
offset 0x44 bit 1. It does not consult the display-special visibility flags.
Target/native builds and all four checks pass.
The isolated native fixture confirms both absent cases return 1 and all
flag values 0 through 15 return exactly bit 1. It exits successfully without
sanitizer errors (`/tmp/saga-onscreen-runtime.log`).

Recovered hopping details: linked pieces begin from
`start_mtx` and skip already built indices; unlinked pieces begin from
`NuSpecialGetMtx`. New hops require the linked `was_drawn` byte or the
unlinked on-screen query. A selected hop sets `wobble_time=0.2` and chooses
one of six axis/sign values using `qrand()/0x2aab`. The phase is
`wobble_time/0.2`, with base angle
`int(3640 * NU_SIN_LUT((1-phase)*65536))`, negated for odd axis values.
The selected normalized matrix axis is dotted with `v010`; its absolute
alignment attenuates rotation by
`1-(1-NU_SIN_LUT(alignment*16384+16384))*alignment` before the matching
X/Y/Z pre-rotation. Lift is `NU_SIN_LUT(phase*32768)` times original global
`GIZBUILDITWOBBLEJUMPHEIGHT` (0.05 at `0x6686e0`) times buildit offset 0x50.
Linked results update `draw_mtx`; unlinked results call `NuSpecialSetDrawMtx`
and `NuSpecialUpdate`. These operations are now integrated through
`AnimateLooseBuildItPieces`. The enclosing late-update function matches
**15.628%** (1919 bytes, with the helper emitted separately); its original
6532-byte body remains substantially incomplete. This score does not establish
a full match or a gameplay fix.

Target and native builds and all four repository checks pass
(`/tmp/saga-build-hop-{target,native,checks}.log`). A native Cantina run
naturally reaches event 0x3c for idle, unbuilt `frame_2` (30 pieces), at
`(-51.0323, 0.0609311, -54.6899)`, through the restored helper and
`GizBuildIts_LateUpdate`. The debugger exits successfully with no sanitizer
errors (`/tmp/saga-build-hop-runtime.log`). Its 1206 observed draw-matrix
updates include other callers and do not independently verify this object's
transforms. The breakpoint stops before playback and the run is muted;
audibility and visual correctness remain unverified.

The next recovered branch is the build-step start debris emission
(`0x4cf7d0`, `0x4d0b5c..0x4d0bc4`). If the decremented timer remains
positive and its previous value equals `step_duration`, the original obtains
the current piece's draw position and emits `GizBuilditGDeb[qrand()/0x2aab]`
before hiding an unlinked piece. This branch is now restored. Late-update
matching increases to **16.145%** (2078 bytes, helper still separate).
Target/native builds, all four checks, and `git diff --check` pass
(`/tmp/saga-build-start-{target,native,checks}.log`). A controlled native
fixture now sets the loaded Cantina `frame_2` object's builders-active byte
and resets its timer to the existing 0.3-second duration. The update reaches
the breakpoint after debris emission and piece processing, with previous
timer 0.3 and current timer 0.283018. The debugger exits successfully with no
sanitizer errors (`/tmp/saga-build-start-runtime.log`). This verifies execution
of the recovered branch with a loaded object, not natural player activation,
visual correctness, or completion of building.

Remaining automatic-build path: original `0x4cf9f8` calls
`GizBuildit_AutoBuildPosFn(world, &buildit->start_position, &position, &angle)`
when available. A successful callback drives a reverse loop over unbuilt
pieces, seeking each draw position toward a circular arrangement at speed
6 (`0x4cfb60..0x4cfcff`); linked pieces update draw-matrix translation and
unlinked pieces call `NuSpecialSetDrawPos`/`NuSpecialUpdate`. Regardless of
callback success, the original then decrements the step timer. Its positive
timer branch calls `GizMoveAttractoBuildItPiece` (`0x4d052e`), whereas the
current shared manual/automatic branch hides unlinked pieces. The attraction
function itself remains an empty stub in `gizmo/object/gizbuildit.cpp`;
original address `0x4cda80`, size 1040 bytes, disassembly
`/tmp/saga-build-attract-original.asm`. Reconstruct this body and its caller
together before claiming automatic building matches.

The original also distinguishes unlinked placement branches: manual placement
requires an instance animation (`0x4d0ea2`), while automatic placement checks
for one (`0x4d0dff`) and restores draw position from `NuSpecialGetPos` after
event 0x3b. The current shared placement helper does not preserve that
distinction. These are assembly findings, not additional runtime fixes.

Attraction reconstruction exposed a prerequisite ABI error: original
`NuMtxToQuat` takes `(NUMTX *, NUQUAT *)`, confirmed by matrix reads from
argument one at `0x284a67..0x284a8c` and both calls at
`0x4cdc40..0x4cdc6a`. The declaration, definition, and two cutscene callers
now use that order. Its current body matches **23.08%**, 1239 bytes;
the body itself remains incomplete. Target/native builds and all four checks
pass (`/tmp/saga-quat-abi-{target,native,checks}.log`). No runtime or full-match
claim follows from this ABI correction. `GizMoveAttractoBuildItPiece` remains
to be implemented: progress is `1-step_timer/step_duration`; for positive
progress it converts translation-cleared current/end matrices to quaternions,
slerps orientation, interpolates translation and adds a 0.1-scaled half-sine
vertical arc. It writes the current draw matrix, then copies to linked
animation data or calls `NuSpecialSetDrawMtx` for an unlinked piece.

The attraction stub has now been replaced with that recovered body in its
existing O3 source file. It matches **92.788%** (1032 current bytes versus
1040 original); target/native builds and all four checks pass
(`/tmp/saga-attract-{target,native,checks}.log`). The caller's missing
automatic-build path is still not wired up. The first controlled runtime
fixture selected `Vehicle` without validating its timer state and reported
different expected/actual midpoint positions, so it is not a pass. A corrected
fixture selects an idle object with positive step duration and succeeds:
`frame_2` expected and actual midpoint positions are
`(-51.1182, 0.1, -54.6936)`; the zero-progress linked copy preserves that
position. The debugger exits successfully with no sanitizer errors
(`/tmp/saga-attract-runtime-valid.log`). This does not verify orientation,
visual correctness, or natural automatic-build activation.

Aligning matrix/quaternion declaration order with the original stack layout
and expressing translation as subtract, scale, then add raises attraction
matching to **93.319%** (1032 bytes). The target build passes
(`/tmp/saga-attract-vector-target.log`); the remaining instruction differences
include translation scheduling/register allocation and relocation operands.
Native compilation and all four checks also pass for this retained version
(`/tmp/saga-attract-vector-{native,checks}.log`). A direct-result-matrix
translation experiment lowered matching to 88.004% (1052 bytes), so it was
rejected and the 93.319% version restored and rebuilt
(`/tmp/saga-attract-restored-target.log`, `/tmp/saga-attract-restored.diff`).

An automatic-positioning caller candidate compiled but reduced late-update
matching from 16.145% to 4.729% (3033 bytes); it was removed and the previous
version rebuilt (`/tmp/saga-auto-restored-target.log`). Preserve the original
control-flow structure when reintegrating this path. The recovered constants
are 4.5 seconds, initial radius 0.5, radius reduction `blend*0.3`, and angle
increment `5*blend*360*65536/360`, where
`blend=1+sin(((4.5-timer)/4.5)*16384+32768+16384)` in LUT angle units.
Original angle conversion truncates to signed integer before narrowing to
16 bits; preserve that sequence. The rejected patch is recorded only as an
experiment in `/tmp/saga-auto-position-candidate.patch` and is not active code.

Quaternion dependency: the original `NuMtxToQuat` positive-trace branch uses
`m12-m21`, `m20-m02`, and `m01-m10`, opposite the previous reconstruction's
rotation signs. It computes the square root before storing `w=s*0.5` and
inverting `s`. Its other branch chooses the largest diagonal using cyclic
indices `{1,2,0}`, computes `sqrt(mii-(mjj+mkk)+1)`, and inverts only when
that result is nonzero. The full indexed algorithm is now restored, improving
matching from 23.08% to **78.789%** (928 current bytes versus 1000 original).
Target/native builds and all four checks pass
(`/tmp/saga-quat-body-{target,native,checks}.log`). Known-rotation runtime
verification is still required; earlier attraction translation checks do not
verify this corrected orientation behavior.

Known-rotation native checks now pass: identity gives `(0,0,0,1)`, positive
90-degree X gives `(0.707107,0,0,0.707107)`, and the three 180-degree axis
rotations give the corresponding unit-axis quaternion with w=0. The debugger
exits successfully without sanitizer errors (`/tmp/saga-quat-runtime.log`).
Restoring flat matrix indexing and local declaration order further improves
matching to **83.622%** (1006 bytes), with a successful target build
(`/tmp/saga-quat-flat-target.log`). Full byte matching remains unfinished.
Native compilation and all four repository checks pass for the retained flat
indexing version (`/tmp/saga-quat-flat-{native,checks}.log`). Mixed flat and
row/column indexing was tested and rejected at 82.890%; restoring flat indexing
and rebuilding confirms **83.622%** (`/tmp/saga-quat-restored.diff`).

Character-switch routing audit: original `Player_ToggleCharacter` at
`0x46c730` rejects an active world whose area equals non-null `HUB_ADATA`
(`0x46c775..0x46c787`) and returns when `FreePlay` is zero
(`0x46c7d8..0x46c7e2`). It also rejects fades, CInfo context flag 0x100,
and object offset 0xcc0 before cycling. This identified the former stub as a
Free Play cycling gap; its recovery preserves hub restrictions rather than
address Cantina tagging. Current `MovePlayer` calls `Tag_Check`, which is
the separate path to inspect for Cantina switching. Original GOT resolution
also identifies `Player_ToggleSubCharacterFn`, `TOGGLEHOLDTIME`,
`TOGGLEREPEATTIME`, `GAMEPAD_TOGGLELEFT/RIGHT`, and
`Player_ToggledCharacterFn`; the reference disassembly is
`/tmp/saga-toggle-original.asm`.

Current tagging revalidation: `Tag_FindGameObject_TRANSFER` matches **99.773%**,
461 bytes, with distance/facing/context eligibility checks consistent with the
original (`/tmp/saga-tag-find-current.diff`). The existing controlled Cantina
fixture was rerun after the AI initialization and quaternion changes. It places
an eligible NPC near the player and injects `GAMEPAD_TAG`; `Tag_NewTransfer`
observes Player[0] changed to the target, source slot -1, target slot 0, and
the target's gamepad equal to GamePad[0]. The debugger exits successfully
without sanitizer errors (`/tmp/saga-tag-current-runtime.log`). This confirms
current transfer execution under controlled positioning/input, not natural
target acquisition or the user's complete switching experience.

A follow-up observation preserves NPC positions and injects the tag button
only when the original target finder returns a candidate. The first run ends
normally without a transfer event (`/tmp/saga-tag-natural-position.log`).
Because that fixture did not report its search count, the absence of an event
does not establish whether eligible targeting checks ran. A second fixture
adds a final count and exits normally after **619 searches with no candidate**
(`/tmp/saga-tag-natural-count.log`). This scripted route does not exercise
natural target acquisition and is not evidence that character switching is
fixed.

Tag indicator gap: `Tag_DrawIcon_LSW` remains a stub (original `0x145220`,
532 bytes; `/tmp/saga-tag-icon-original.asm`). Original gates include
VehicleArea, FadeSys.fade, player-active bit, LEVEL_HIDE_ICONS, and optional
`Tag_NoHiddenIconFn`. It draws when object byte 0xefe bit 0x10 is set or the
0xd5c timer permits blinking (below 2 seconds, `fmod(timer,0.4)>=0.2`). It
copies `AddGameMsg_Default`, sets byte 0x4f to 1, scale 3, text `LEGOASCII_UP`,
position.y += object[0xffc]*object[0xa8], PlayerRGB, flags 0x87, and alpha
`int(48*sin(fmod(GameTimer.time,0.5)*2*65536)+80)`. The callback and text
registration remain commented in game.cpp, so recovering the draw body alone
would not establish visible functionality. Restore the data and registration
from original evidence together with the body.

The tag-indicator body is now restored at **94.266%** matching (540 current
bytes versus 532 original), along with original `PlayerRGB` bytes
`00 7f ff 00 ff 00`, null `LEGOASCII_UP`, and null `Tag_NoHiddenIconFn`.
Target/native builds and all four checks pass
(`/tmp/saga-tag-icon-{target,native,checks}.log`). Registration and runtime
verification remain outstanding, so the indicator is not claimed visible.
The original hidden-icon callback is a 96-byte static function at `0x11a530`;
it compares the active world's level against a specific level pointer and,
only for that level, tests GameCam byte 1 against 5. The current callback stub
also has the wrong void return type; its integer result must be recovered
before registration.

Registration is now restored in `InitGameAfterConfig`: `Tag_DrawIconFn`,
`LEGOASCII_UP=ASCII_UP`, and `Tag_NoHiddenIconFn`. The original glyph bytes
are c2 ac; `ASCII_UP` is restored as an initialized pointer. The hidden-icon
callback is recovered beside its registration, retaining static linkage and
removing the old void stub. It returns whether camera sock is 5 only in
DEATHSTARESCAPEB; otherwise 0. It matches **99.889%**, 96 bytes, with only
relocation differences (`/tmp/saga-tag-hidden.diff`). Target build passes
(`/tmp/saga-tag-register-target.log`); runtime visibility remains unverified.

Native registration fixture: the callback runs through `Tag_Check` in
Cantina, with `LEGOASCII_UP` containing original bytes c2 ac 00 and the hidden
callback non-null. After explicitly setting indicator bit 0x10, the fixture
observes `AddGameMsg` arguments scale=3, flags=0x87, RGB=(0,127,255),
alpha=72, byte 0x4f=1, and position=(-26.418,0.690804,-50.6508).
The debugger exits successfully without sanitizer errors
(`/tmp/saga-tag-icon-runtime.log`). This verifies controlled message submission
only: the breakpoint precedes message processing, so neither rendering nor
natural indicator activation is established. Native build and all four checks
also pass (`/tmp/saga-tag-register-{native,checks}.log`).

The follow-up controlled fixture reaches `Text3DEx` through DrawGameMessages,
DrawPanel, and PanelRender with the glyph copied into GameMessage storage.
Observed screen position is `(0.000779929,0.248,1)`, x/y scale 1.30428,
RGB=(0,127,255), alpha=72, alignment=1. The debugger exits successfully without
sanitizer errors (`/tmp/saga-tag-icon-draw.log`). Thus submission, lifetime,
projection, and dispatch to the text function execute for the indicator.
The breakpoint still precedes text decoding/font rendering, so visible glyph
output and natural indicator activation remain unverified.

Visual capture now confirms an arrow glyph is rendered in the controlled
indicator-state run. The debugger completes 698 indicator calls and exits
normally without sanitizer errors (`/tmp/saga-tag-icon-capture.log`). Inspected
frame `.work/capture/run_569156497/window_2147.ppm` (PNG copy
`/tmp/saga-tag-icon-capture.png`) shows the arrow near the player's feet.
This establishes visible glyph output; it does not establish correct placement
relative to the original game or natural activation, because the debugger
holds indicator bit 0x10 on throughout the capture.

Height follow-up confirms the near-feet position is consistent with the
original calculation: object offset 0xffc is the unscaled lower character
bound, populated from CHARACTERDATA offset 0x34, while object offset 0xa8 is
character scale. `SetGameObjectCharacterData` matches **99.947%** (89 bytes);
original `GetTopBot` at `0x46dc90` also copies character-data 0x34 to object
0xffc and 0x38 to 0x1000. Current GetTopBot matches 78.150% with the same
bound stores. No visual height adjustment was made. Evidence:
`/tmp/saga-tag-height-init.diff`, `/tmp/saga-tag-height-update.diff`.

`AIFormationFollow` (`0x3f2750`, 491 original bytes) now replaces its empty
body with the original row/column offset, reversal, movement instruction and
forward look-target sequence. It matches **81.944%** (440 current bytes).
Target/native builds and all four repository checks pass. An isolated native
debugger fixture with row position `(10,0,20)`, yaw zero, column one and spacing
two observes destination `(8,0,20)`, look target `(10,0,120)` and movement flags
9; reversal produces `(12,0,20)`. Centering produces x=10 for three columns
and x=9 for four columns. An invalid row leaves a sentinel destination x=123
unchanged. The expanded fixture exits successfully without sanitizer errors
(`/tmp/saga-formation-follow-runtime-expanded.log`). This verifies these
helper branches, not natural formation playback or resolution of the Cantina
movement regression. `AIMoveInstruction` still lacks the original formation
leader update through `FormationMove`; that dispatch remains incomplete.

The next original-binary dispatch correction restricts formation following
to modes 1 and 4; mode 5 remaps to direct mode 1, and other modes retain their
requested instruction. Previously every mode except 5 entered formation
following. `AIMoveInstruction` improves from **61.519% to 67.766%**. Target and
native builds and all four checks pass. The native isolated fixture tests
requests 0 through 7: resulting modes are `0,1,2,3,1,1,6,7`. Requested stopping
distance 2 and range 3 survive for the direct modes; formation-follow modes
use the original zero arguments. The fixture exits successfully without
sanitizer errors (`/tmp/saga-formation-dispatch-runtime.log`). This does not
exercise formation-leader updates. The missing static callbacks are
`RowMoveWander` (`0x3d7530`, 1736 bytes) and `RowMoveTowards` (`0x3d7c00`,
1851 bytes), called through `FormationMove` (`0x3d2d00`, 307 bytes).

The pre-existing `Player_CopyEssentials` reconstruction was checked against
Ghidra and objdiff: `_Z21Player_CopyEssentialsP12GameObject_sS0_` matches 100%.
This only verifies the copied state fields.

`NewPlayerCharacter` (`0x0ff530`, 1760 original bytes) now reconstructs the
original model replacement, AI/path-state preservation, movement and surface
reset, health restoration, slide/velocity handling, and special-character
layer selection. It compares at **71.300%** (1737 bytes), so full matching
remains unfinished. `InitCreature` moved with their shared private layer
helpers from `game_object.cpp` to `characters.cpp`; the original shared helper
calls support this ownership. The existing source optimization settings stay
unchanged (`characters.cpp` at `-O2`, `game_object.cpp` at default `-O0`),
verified with Bazel aquery. `InitCreature` improves from **0% to 53.571%**.

`SetLayers_BOB` (`0x0fb540`) compares at **99.989%**, with the original
415-byte size and only the relocated GOT-base operand differing. It retains
all seven random draws, including the unused second draw. The Mos Eisley
helper (`0x0fb890`) compares at **99.908%**, with the original 266-byte size;
only relocated operands differ. For this comparison only, a temporary target
copy normalizes the compiler's `.isra.1` suffix to the original `.isra.2`.
The reference binary is unchanged. Sequential layer writes and unmasked
shifts follow the original instructions; all layer-table entries are below 32.

The native sanitizer swap fixture directly replaced Qui-Gon with Obi-Wan,
preserved health, path/AI state, player slot and pad pointer, and reached the
ordinary character draw path. Invalid and unchanged IDs left the object
unchanged (`/tmp/saga-new-player-runtime.log`). Separate deterministic fixtures
check both layer helpers at seeds 0, 1, 12345 and 65535, including the final
RNG state and BOB's unrelated flag preservation
(`/tmp/saga-bob-layers-runtime.log`, `/tmp/saga-mos-layers-runtime.log`).
These fixtures passed without sanitizer diagnostics. Target/native builds
and all four repository checks pass. This is direct swap/helper coverage;
input-driven cycling was tested separately in the following recovery.

`Player_ToggleCharacter` was an empty stub. Its original implementation
returns in the hub and when Free Play is off. It is separate from ordinary
tagging. There were already uncommitted changes in `Tag_Check`, `TagCode`,
and deferred player-tag handling when this audit began.

The toggle routine's `ViewCamGetGamePad` dependency now returns the original
camera-owned input pointer at `ViewCam + 0x24`. `ViewCamSetActive` preserves
the original null-player early return, activation target copy and pad binding,
and deactivation pad clearing. Its comparison is **99.885%** (92 bytes);
`ViewCamGetGamePad`, `ViewCamGetTgt` and `ViewCamGetMode` compare at
**99.714%**, all at their original sizes, with only relocated operands
differing. The 40-byte `ViewCam` initializer matches the reference bytes
exactly; unresolved middle fields retain their original words. No source
optimization setting changed. Target/native builds and all four checks pass.
The native sanitizer fixture `/tmp/saga-viewcam-runtime.log` checks the
null-player path, activation, all accessors and deactivation successfully.
This establishes the input-ownership dependency, not character cycling.

`Player_ToggleCharacter` (`0x46c730`, 2391 original bytes) now reconstructs
the original input press/held-repeat paths, context and hub/story gates,
loaded-model iteration, collection/vehicle/bonus/terrain eligibility,
height-clearance rejection, swap callbacks, hint/camera update and directional
sound calls. It compares at **34.366%** (2399 bytes), so the reconstruction
is still far from a complete match. The original globals include a 0.25-second
repeat time and lift-button mask 8. `InitGameAfterConfig` at `0x11ce76`
loads the Free Play hint global and assigns 600 at `0x11ce7c`; this missing
registration is restored. All evidence came from binutils and objdiff.

Target/native builds and all four repository checks pass. The controlled
native sanitizer fixture `/tmp/saga-toggle-branches.log` retains loaded
Cantina assets but temporarily enables Free Play and removes the hub-area
gate to exercise cycling. It verifies that the unmodified hub and Story
gates reject cycling, both held directions reset the timer, a held direction
waits for the timer, a pressed right direction changes Qui-Gon (104) to
Obi-Wan (1), and the next expired held-button repeat selects character 168
and restores the original 0.25-second interval. The resulting character
reaches ordinary rendering. The fixture exits successfully without sanitizer
diagnostics. This does not verify a full Free Play level, every eligibility
branch, audible sounds, or ordinary Cantina tagging; those remain open.

A hidden native run confirmed `Tag_Mode == 2` in the cantina, with only
`Player[0]` assigned. The original `CheckResetBits` (ELF `0x11f360`) selects
mode 3 in `HUB_ADATA`, otherwise mode 1. That missing selection is restored,
together with the nearby-character/party search, collection eligibility,
locked-pack handling, and failed-tag fallback in `Tag_Check`.

`Tag_FindGameObject_TRANSFER` (ELF `0xd98c9`) was an unused empty placeholder
in `game_object.cpp`. It now lives with its caller and private `Tag_Mode` in
`tagging.cpp`, retaining the original static symbol. The source stays at its
configured `-O2`; keeping the search out of line recovers the original EAX
argument convention. Its measured match is **99.773%**, with relocated
addresses and three uses of a different SIMD temporary register remaining.

The two 0x38-byte transfer records are recovered, replacing disconnected
timer globals. `Tag_ResetTransfers` matches all instructions modulo addresses
(99.333%). `Tag_NewTransfer` follows the original three random heights,
positions, timer reset, and first/any-tag flags; its match remains partial
(72.752%). The surrounding `Tag_Check` remains partial, including its pack
hint rendering, and transfer trail updates remain unfinished.

A GDB fixture placed a real cantina character 0.3 units in the original
forward search half-plane and injected `GAMEPAD_TAG`. The real tag path
changed `Player[0]` to that character, moved its player index from -1 to 0,
assigned `GamePad[0]`, and removed the old character's player index. Placing
the character behind the player was rejected. Fixture position/input changes
were debugger-only. This verifies the ownership transition, not every tag
mode, controller, or visual effect.

## Other open paths

- Collision: `CollideGameObjects` already calls `APIObjectCollisions` and
  `Collide2Objects`. Establish which eligibility test or downstream response
  diverges before changing collision behavior. A live cantina snapshot at
  player age 121 had 22 objects and three eligible collision objects. The
  other objects had zero draw results; early spawn-frame snapshots alone
  were misleading. Original collision eligibility and call ordering were
  checked; no collision behavior was changed in this pass.
- Audio: build completion/piece placement already call `GameAudio_PlaySfx`.
  Trace event selection, sound-ID initialization, loading, and playback;
  do not add duplicate playback calls to hide a downstream failure.
- Level objects: compare gizmo initial/reset/progress state and mission-mode
  handling against the binary. Do not hide a specific minikit or hardcode
  cantina construction state.
- AI movement: the synthesized input previously normalized every nonzero
  destination offset to full speed. The original scales its magnitude by
  `distance / ai_moveradius`, caps it at one, and applies its configured run
  speed, stop threshold, walk/tiptoe limits, and movement-stop flags. Those
  rules are restored. `ai_moveradius` is the original 0.1 (ELF `.data`
  `0x664f00`), not a tuned workaround. A native fixture using a real cantina
  NPC with run speed 1.2 observed speeds 0, 0.299995, and 1.2 at destination
  distances 0, 0.025, and 0.1; its explicit stop flag produced zero speed.
  `MovePlayer` as a whole remains partially reconstructed and its current
  objdiff score is 0%; this fixture does not establish a full match or prove
  all natural cantina wandering is correct.

A controlled native overlap between two real cantina characters reached
`APIObjectCollision2D` through `CollideGameObjects` and exchanged X velocities
from `0.1/-0.1` to `-0.1/0.1` (return value 2). This narrows the collision
investigation: character response is present. The fixture displaced the NPC
without synchronizing every terrain-history field, so its subsequent
position jump is not evidence of a production teleport bug. Level-object
collision and normal unmodified gameplay remain unverified.

## Audible Force-sound starvation

The user reproduced clicks/gaps while using the Force on a cantina lamp
that releases coins. The host mixer reported recovery after 21–22 ms on
the same player, twice per activation in the supplied log. The warning is
emitted only when a playing stream that supplied no PCM later supplies PCM
again; it is not merely the end of a one-shot sound.

The shipped `levels/map/map/map.giz` contains version-16 `GizForce` records
including `Light_1` and other lights. `Audio/Audio.cfg` defines `JForceUse`
as a looping sound, and `MovePlayer`'s Force-use path requests it. This is
the relevant candidate; the warning currently identifies only the player
address, so its association with that sample still needs runtime confirmation.

The queue refill decision in `NuVoiceAndroid::UpdateQueue` uses the original
hardware flag byte at `+0x17e` and its queue-count high-water mark. The
investigation recovered `OnPlayerEvent`'s original source-type test before
locking and its separate stop/refill branches. Recovering Boolean source
bitfields at `+0x30` and hardware flag bitfields at `+0x17e` produces the
original branch and flag-write instructions. Its measured match improved
from 14.944% to **99.972%**: the only differing instruction is the relocated
GOT-base addition. This is an instruction match modulo relocation, not a
byte-identical linked address or a verified underrun fix.

`UpdateQueue` now matches **99.967%**, up from 52.967%, after restoring its
outer interface guard and the original refill/queue-ran-ahead flag write order.
Only the GOT-base and error-message address operands differ. The decoder's
original `OpenStream` also primes at most two buffers; that limit was not increased.
Target/native builds and all four repository checks pass. The native audio
utility (`/tmp/saga-audio-queue-runtime.log`, SDL dummy driver) passed with
one hardware player, 660 kB consumed, and RMS 3123.8. This proves title-music
PCM reaches the host mixer, not that the reported Force-lamp gaps are fixed.

Further native traces (`/tmp/saga-force-audio-trace8.log` through
`/tmp/saga-force-audio-trace11.log`) requested `JForceUse` at the player's
position using GDB. The loaded sample is index 341, has a resource reference,
and lies within the listener's distance limit. Requests enter pending playback;
`NuSoundSystem::CreateVoice` returns a voice, `Play` requests the memory sample,
and its non-null callback reaches `SubmitBuffer`. However, both the OpenSL
player and queue are null, so no buffer is enqueued for this sample.

The loaded WAV descriptor reports mono 16-bit PCM at **10,985 Hz**, with 7,840
bytes of audio. `NuSoundAndroid::IsValidSampleRate` rejects this rate and
matches the original at **100%**; `CreateHardwareVoice` matches **96.395%**.
These comparisons used `objdiff-cli.py`, without Ghidra. Extracting the shipped
WAV to `/tmp/saga-jforceuse.wav` confirms the file itself contains 10,985 Hz,
mono 16-bit PCM and a 7,840-byte data chunk. Its full length is 10,550 bytes,
including RIFF metadata. Do not broaden the original validator merely to play
this sample. This synthetic request is
not the lamp interaction and does not identify the sound responsible for the
user's audible gaps; music and footstep buffers did enqueue successfully in
the same traces. Debugger timing is unsuitable for measuring underrun duration.

The follow-up `/tmp/saga-legoform-audio-trace.log` requested the shipped
`LegoForm` loop (11,025 Hz, 27,270 PCM bytes). This voice remained active and
successfully enqueued buffers. Its memory-feed end events (`feed=0`, `loop=1`,
`event=1`) repeatedly led to refills and host starvation warnings on the same
player. Thus this path reproduces the warning without an Ogg decoder. The
host currently signals end only on a subsequent empty pull, rather than on
the pull that consumes the final available frames. Exact gap duration and
association with the user's lamp still require a less intrusive runtime test.
`UpdateHardwareVoice` now compares at **94.027%**, improved from 93.446% by
retaining the source pointer across weak-callback construction, as the original
does. Its refill control flow agrees with the original; remaining differences
are concentrated in weak-pointer code. Target/native builds and all four
repository checks pass (`/tmp/saga-loop-source-*.log`). This is a fidelity
improvement, not a demonstrated gap fix.
The rebuilt native audio utility passed with one player, 680 kB consumed and
RMS 5125.3 (`/tmp/saga-loop-source-audio.log`, SDL dummy output). This checks
title-music mixing, not the reported Force-lamp loop.

[Android's OpenSL implementation](https://android.googlesource.com/platform/system/media/+/2b10d238deaa6df9789c8230fff2453df4a62277/opensles/libopensles/android_AudioPlayer.cpp)
also dispatches `HEADATEND` on an empty buffer queue while retaining the playing
state. Therefore the host's empty-pull trigger alone is insufficient evidence
of incorrect event semantics. The remaining comparison must account for
AudioTrack buffering and host device buffering, rather than simply advancing
the callback or increasing the game's queue depth.

The original `GizForces_Update` calls `GameAudio_PlaySfxById` for object sounds
and falls back to `LegShakeL`/`LegoForm`; those branches remain missing from the
current partial update. The separate empty `GizForceSFX_Configure` is a legacy
parser: the original skips it when the loaded gizmo version exceeds 15, so it
does not explain the version-16 Cantina objects. Its callbacks establish that
offsets 0x86/0x88/0x8a mean process/completion/return sounds; current field names
start/loop/stop are misleading. No speculative sound calls were added.

Both extraction utilities wrote their requested output but exited with a
four-byte LeakSanitizer report from global `NuSoundAndroid` construction;
they are not recorded as clean utility passes.

The queue-state query incorrectly reserved only one `u32`. Android's
[`SLAndroidSimpleBufferQueueState`](https://android.googlesource.com/platform/system/media/%2B/gingerbread/opensles/include/SLES/OpenSLES_Android.h)
contains two `u32` fields, `count` and `index`. The shared ABI declaration
and target query now reserve both, and the host adapter returns both,
advancing the index as entries are consumed and resetting it on clear.
This corrects an ABI mismatch, but is **not an established fix for the
audible starvation**. Buffer sizes, refill thresholds, and warnings were
not changed.

`GroupBuffer_GetSample` and `NuSound3IsSampleLoaded` were compared with
Ghidra. Both gate group selection on the original loaded-sample state.
`PickupCoin` and `LegoSingle` are groups in the shipped configuration;
their missing playback still needs loaded-state/runtime tracing.

The newer pickup trace (`/tmp/saga-pickup-audio-contact2.log`) positioned the
diagnostic player at an existing Cantina stud without modifying pickup flags
or calling the sound functions. Normal gameplay requested `PickupCoin` twice
and reached `NuVoiceAndroid::SubmitBuffer` for `Coin1.wav`; it also requested
`PickupCoinB` and reached `SubmitBuffer` for `CoinBlue.wav`. These voices used
memory feed type 0. This establishes dispatch to the submission entry point,
not audible playback or proof of successful enqueue. No sanitizer error was
reported. The window utility ultimately returned 1 at its 60-second timeout:
the diagnostic had stopped the scripted stage, retained its held movement input,
and used `--camera-free`. It is not recorded as a passing window utility test.
The ordinary walking fixture before it missed the stud and made no pickup
request; another attempt failed assigning a non-addressable GDB timer local.

## Flow-loader checkpoint (in progress)

The two-pass `LoadGizFlow` parser and its box, collapse, condition, action,
and gizmo callbacks are reconstructed from the original binary. Timer,
random, and special constructors now return the registered gizmo, as the
loader expects. Runtime allocations use the recovered structure sizes.

This remains unfinished: the execution callbacks have initial reconstructions,
but matching and object-specific gameplay verification are incomplete.
Level-object visibility and progression are not yet considered fixed.
Initial target comparison gives `LoadGizFlow`
79.453%, `createGizSpecial` 87.879%, `createGizRandom` 13.991%, and
`createGizTimer` 0%. The latter two retain their current default optimization
settings; their original optimized instruction shapes remain unresolved.

`GizFlowStoreProgress` and `PerformActionFlowBox` are now **100% matched**
against the original target. The progress record is `0x144` bytes: a valid
word followed by five arrays of 16 flag words. The action dispatcher walks
the recovered linked records and invokes the original action callback with
its arguments and count. Target build and all four repository checks pass.
These restore dependencies; execution and gameplay verification remain outstanding.

`Loop_CountLoopingInputsEx` now traverses child links with the original
checksum visitation and root feedback counting. The target compiler expands
the recursive traversal as in the original; current matching is **87.739%**.
The helper is now connected to reset, but does not by itself establish a
runtime correction to level-object state.

`CheckOutputActionFlowBox` now returns the original completion bit with the
original full-width integer callback return and matches **100%**.
The indirect caller tests EAX; the earlier Boolean declaration was too narrow
despite producing the same function instructions. `CheckOutputGizmoFlowBox`
recovers the original Story/Free Play eligibility and aggregate gizmo-output
query; its current match is **69.513%**. The original three-entry callback
table at ELF `0x66c840` is restored with the original local symbol name.
`CheckOutputConditionFlowBox` matches **74.927%** and the parent-gated
`ProcessActionFlowBox` matches **91.429%**. These output callbacks alone
do not establish correct gameplay state.

`ResetGizFlowPointers` now resolves named references and invokes the original
reset/visibility callbacks; its current match is **98.785%**. `ResetGizFlow`
restores initialization, saved flags, loop counts, and mode-dependent hiding,
with a first full-function match of **37.355%**. Both live with the callback
table in `gizmo_sys.cpp`; the callbacks retain `-O3` when moved from
`gizmo.cpp`. The previously empty reset definitions moved from the `-O2`
file to their `-O3` helpers, without changing any per-file compiler options.
Original reference-reset instruction structure was recovered by that placement.

`CheckResetBits` again calls flow reset at its original position before
tag-mode selection, using saved progress at `LEVEL_PROGRESS + 0x2c20` and
the original world flag at `0x5174`. A native GDB run observed reset of
**122 Cantina flow boxes** and active gameplay beyond three seconds without
a sanitizer failure. This verifies reset integration, not construction or
collectible progression.

The initial execution reconstruction now includes `ProcessGizFlow` (78.2%),
`ProcessGizmoFlowBox` (89.151%), `ProcessConditionFlowBox` (51.952%),
`ResetForLoopEx` (99.877%), and `GizmoActivateReverse` (31.478%). A subsequent
binutils/objdiff pass removed the obsolete `__used__` marker from the now-called
parent-completion helper, allowing the compiler to recover register arguments.
Its traversal now matches **31.821%**, up from 17.09%. Restoring the original
separate reverse-activation branches improved `ProcessFlowBox` from 0% to
**15.931%**. Neither function is fully matched.

The native execution fixture `/tmp/saga-flow-execution-runtime3.log` reached
Cantina gameplay at 3.016591 seconds, with 122 boxes, frame 178, 36 active boxes,
and 61 finished boxes, without a sanitizer error. This establishes execution
integration only; it does not prove correct outside-Cantina builds or Story
minikit visibility. After the source-shape refinements, the repeated native
fixture (`/tmp/saga-flow-shape-runtime.log`) reached 3.011934 seconds with the
same frame and active/finished counts and no sanitizer error. Target and native
builds, all four repository checks (including duplicate definitions), and
`git diff --check` passed for this checkpoint.

The next binutils/objdiff pass recovered the driver's original frame reload
placement: `ProcessGizFlow` now matches **100%**. In `GizmoActivateReverse`,
computing query/command values before the null checks and restoring the original
early exits improved matching from 31.478% to **99.739%**. Its remaining diff
contains address offsets and the register used for the final reverse-zero test;
there are no remaining branch or callback differences in this wrapper.
`GizFlowStoreProgress`, `PerformActionFlowBox`, and `CheckOutputActionFlowBox`
remain **100%** in `/tmp/saga-flow-driver-matching.json`. This does not change
the incomplete status of reset, individual flow-box processing, or gameplay
verification. The target/native builds and all four repository checks pass.
The repeated native fixture (`/tmp/saga-flow-driver-runtime.log`) reached
Cantina at 3.016922 seconds, again with 122 boxes, frame 178, 36 active and
61 finished, without sanitizer errors. It did not exercise a reversing object
or verify the reported outside-Cantina and Story collectible cases.

### Negotiations Story pickup observation

The native fixture `/tmp/saga-story-pickup-runtime3.log` subsequently reached
`Negotiations_a` at 3.016129 seconds with `FreePlay=0` and `ChallengeMode=0`.
Pickup type 7 is named `Charkit` in the loaded configuration. Its mode-disable
byte was 1; all four instances (indices 108–111) had state flags `0x27`, with
the `DRAWN` bit (`0x10`) clear. Other pickup types, including ordinary minikits,
had their mode-disable byte clear. No sanitizer error occurred in this run.

`GizmoPickups_SetOnOff` is called at the original position in `NuMain` and
currently matches **65.850%**. Original assembly explicitly disables type 7
outside Challenge mode, consistent with the observed state. This is evidence
for current Story startup filtering, not proof that every later level state
or Challenge-mode transition is correct. The renderer's pickup path and mode
filter still require fuller matching. The fixture changes only level selection
in GDB; it does not alter pickup flags or the mode globals.

The first fixture attempt was invalid: it left the harness's Cantina movement
script running during the level transition and hit a null player snapshot in
`host/harness/window.cpp:253`. A second attempt had an unsafe GDB breakpoint
condition before world initialization. The successful third attempt corrected
the fixture by stopping scripted input before requesting the level and checking
world/player availability. No engine workaround was added for either test error.

A subsequent assembly comparison retained the original ordering of the Arcade
type exclusions (`9`, then `5`) and expressed the mutually exclusive mode rules
as one conditional chain. The match improved slightly to **65.858%**; alternate
pointer-loop forms reduced matching and were discarded. Target/native builds
and all four checks pass. The native mode-cycle fixture
(`/tmp/saga-pickup-mode-cycle.log`) observed type 7's disable byte change
**1 → 0 → 1** across Story → Challenge → Story at 3.009639, 4.011301,
and 5.011653 seconds. Challenge disabled all nine other pickup types; returning
to Story re-enabled them. No sanitizer error occurred.

The four Charkit instances remained undrawn even during this Challenge phase,
so this fixture proves mode-filter transitions only, not their visible rendering.
The mode changes were diagnostic GDB assignments; pickup flags were untouched.

## End-level results reconstruction (in progress)

### Negotiations outro black screen

The shipped outro transitions to `negotiations_status`, a separate level
marked `LEVEL_STATUS`. `FixUpLevels` only registered the generic `status`
level's callbacks; its final original loop over `LEVELCOUNT` was missing.
That loop assigns `UpdateStatusScreen` and `DrawStatusScreen` to every status
level (and to `STATUS_LDATA`). Restoring it increases the complete function's
objdiff score from **95.615% to 96.671%**. The function is not yet fully
matched; the restored loop's core instruction sequence matches the original.

The native diagnostic enters the configured `FailedNeg_Outro` level and lets
the cutscene choose its destination. It now reaches `InitStatusScreen` with
`NewLData == negotiations_status`, two player objects, and then
`UpdateStatusScreen` with an active results packet. Target/native builds and
all four repository checks pass. This test differs from the earlier fixture
that assigned the generic `STATUS_LDATA` directly and missed the broken path.
The final native run cleared the wipe (`FadeSys.fade == 0`), called the
results draw callback in phase 1, and advanced through stage types
13, 35, 36, 19, 9, 10, 11, and 12. Selecting Cantina through the results
menu reached `FinishStatusPacket` through the normal exit callback. The
diagnostic stopped at that call; it did not verify the subsequent hub load
or visually inspect captured pixels. No result stage was skipped by the
fixture, and no gameplay transition override was added to the source.

### Results exit: world cleanup crashes

The native Continue Story reproduction reached the reported null item in
`NuDisplayListExecute`. A hardware watchpoint showed that
`WorldInfo_Reset` cleared the outgoing world's allocation buffer while a
sort-priority entry in that buffer was still registered. The original
`WorldInfo_Dump` calls `CharScenes_LevelDump` before `CutScenes_Destroy`;
the reconstruction omitted that call and left `CharScenes_LevelDump` empty.
The recovered teardown removes each level character scene and clears its
scene pointer, reloading the scene table after `NuGScnRemove` as the original
does. Its function match increases from **12.632% to 96.395%**. Remaining
instruction differences mean it is not an exact match. `WorldInfo_Dump`
itself is still a partial reconstruction, with other original cleanup
subsystems outstanding.

The separately reported Cantina-return overflow is an incorrect global
reference in `WorldInfo_Reset`. Original code chooses the opposite element
of `WorldInfo[2]` by comparing against the array base. The reconstruction
compared against the mutable `WORLD` pointer and could form `WorldInfo + 2`.
The array selection is restored directly from the original. The whole
function's matching score remains **0%**, so this correction does not make
the complete function matched. No null-item guard, array clamp, or render
skip has been added. The native results-menu fixture returned to an active
Cantina (`Map`) with the fade cleared and more than three seconds of gameplay,
without either reported sanitizer failure.

Continue Story subsequently reached `Gungan_A`, exposing a separate invalid
path pointer in wildlife spawning. Original `GunganA_Update` passes an
embedded `AIPATHINFO` at locator offset `0x20`; the reconstruction dereferenced
its first word and passed the path object as the cursor. The recovered layout
also restores the signed locator count and model-list offset `0x10c`. The
original single locator random draw, enemy-count comparison, common spawn
timer reset, and branch structure are restored. The function's score improves
from **34.489% to 99.117%**, still short of an exact match. The native fixture
then advanced through the next intro into `Gungan_A` and ran for ten seconds
past the original spawn crash.

Target/native builds and four repository checks pass for the transition
work. Overall fuzzy matching increases from **25.706978% to 25.717796%**,
with **1,259 exact functions** retained. These figures do not establish full
matching of the partially reconstructed world cleanup functions.

The original results-stage table at ELF `0x6236c0` contains 34 records of
`0x20` bytes, including its sentinel. Registration, coin counting/drawing,
prompt menu drawing/input, save-stage control, packet finishing, and
exit/fade updates have been reconstructed from the original functions.
`STATUSPACKET_s` remains `0x14c` bytes; callback and result fields now have
types matching their original offsets.

`InitStatusScreen` now has a reconstruction of the mode-specific stage
sequence and completion/reward bookkeeping, but its full-function match is
only **3.571%**. Several results functions still need instruction matching.
Do not bypass these stages or substitute unconditional rewards to make the
screen advance. Original pseudocode alone is insufficient to call the
reconstruction matched.

One dependency had conflated `StatusCollectList` with `Game_CompletionSave`.
The former is eight character IDs, terminated by `-1`; the latter points
at completion points, gold-brick count, and completion flags. The original
`newCharactersCollected` returns the number of IDs. Its list and return
value are now recovered, and completion queries use the original save
pointer instead of interpreting those IDs as a pointer.

`AddGoldBrickMessage` belongs with the results-screen-local
`goldbrickmsgcount`, returns `1`, and resets through the results initializer.
Its reconstructed instruction sequence matches apart from address/layout
operands (objdiff **99.824%**, original size 68 bytes).
`LevelComplete_LSW_Update` is only **51.968%** matched; its control flow has
been reconstructed, but it is not an instruction match. Target and native
builds pass. An earlier generic-status diagnostic reached Cantina with its
test coin total preserved; that did not cover the Negotiations-specific
status level. See the focused outro regression above.

## Pickup mode filtering and priority sound dispatch

`GizmoPickups_SetOnOff` was empty. Its original mode-dependent pickup-type
disable flags are now reconstructed, and the missing disabled-type check in
the pickup draw loop is restored, including the original temporary-pickup
exception. The routine is **65.850%** matched and remains a partial instruction
match. A native per-frame debugger fixture observed disabled-type vectors
`0000000100` in story, `1111111011` in Challenge, `0000100100` in Super Story,
and `0000000100` after returning to story. This verifies the mode flags;
an actual Negotiations visual regression run remains outstanding.

The required `Arcade_GetMode` implementation matches **99.871%**. Its original
three-entry mode table (flags `0x62`, `0x24`, `0x58`) and 24-byte menu-item data
are restored from `.data` at ELF `0x621180` and `0x6211a0`. The existing free-play
caller incorrectly multiplied a typed 12-byte table index by three; it now
uses the original 12-byte stride.

`NuSound3Play3dPri` incorrectly forwarded distance falloff. The original
passes two zero floats; restoring those arguments gives **99.955%** matching,
with only the GOT relocation differing. A native cantina fixture resolved
`PickupCoin` and `LegoSingle` to loaded group members and observed pending
playback voices increase from zero to one to two after those requests.
This establishes sample availability and voice enqueueing, not audible
end-to-end playback or resolution of the Force-lamp starvation.

Target, native, WebAssembly, and all four repository checks passed after
these source changes. No sound-buffer thresholds were adjusted.

## Missing level object-control graph

The shipped `levels\\map\\map\\map.git` is 20,675 bytes and contains 122
`FlowBox` records, including 39 `StartInvisible` directives. In particular,
the `Vehicle_1` box references the `GizBuildit` named `Vehicle` and specifies
both `StartInvisible` and `FinishedInvisible`. `LoadGizFlow` currently returns
NULL unconditionally; the original uses two parser passes, remaps graph
links, and then resets/processes the resulting graph. These missing routines
are a concrete cause of level visibility/progression rules not running.
Do not compensate by forcing individual buildables invisible.

A native cantina snapshot found 11 buildables, all with build state zero
and zero pieces built. Most first animations were stopped at their configured
start frame; `glassPins` was running at frame 188.5 despite its configured
64/65 segment, and needs further tracing. Terrain platform capacity was 464.
The state snapshot does not prove the visual transforms or collision are
correct.

Graph prerequisites `FlowBoxFindByName` and `SetGizFlowVisible` are restored
from the original, matching **99.354%** and **84.259%**, respectively. The
loader and graph execution remain unimplemented; these helpers alone do not
resolve the level-object regression.

## Verification

### 2026-09-08 commit checkpoint

The following units were checked in isolation, with other pending changes
temporarily saved and then restored. No temporary stash remains from this
checkpoint.

| Commit | Recovery | Aggregate fuzzy match |
| --- | --- | --- |
| `7236662` | Previous checkpoint | 26.092190% |
| `82ea2f1` | Matrix-to-quaternion ABI and rotation branches | 26.105015% |
| `b08e9eb` | Build-piece motion, hop effects and visibility query | 26.140535% |
| `d36a6a1` | Force target filtering and cached LOS selection | 26.148338% |
| `abb7db4` | Player AI initialization and formation control state | 26.174232% |
| `88923c2` | Character tag indicator and registration | 26.189165% |

The report's exact-function count remains 1,281 at each checkpoint. Every
code commit passed its normal pre-commit hook: repository checks, target,
native and WASM static analysis, target build, symbol checks and matching
report generation. The complete native executable also builds successfully
(`/tmp/saga-final-units-native.log`).

Direct final comparisons against the target artifact retain the measured
results: `NuMtxToQuat` 83.622%, `NuSpecialGetOnScreenFn` 100%,
`GizMoveAttractoBuildItPiece` 93.319%, `InitPlayerAI` 42.085%,
`Tag_DrawIcon_LSW` 94.266%, and `GizForce_FindBestForceTarget` 33.007%
(`/tmp/saga-final-unit-matches.txt`). These are partial reconstructions except
for the exact visibility query. The documented controlled runtime tests do
not close the remaining gameplay/audio regressions or establish full matches.

Target and native builds and the four tests in `//scripts/checks:checks`
passed during this investigation. These are build/structural checks, not
gameplay regression tests or proof of complete matching.

Reproduce the symbol comparisons after building the target:

```bash
bazel build --config=target //src:saga_target
python3 scripts/objdiff-cli.py _Z19DrawForceGlowSpriteP7nuvec_sfifP12GameObject_s
python3 scripts/objdiff-cli.py _Z21Player_CopyEssentialsP12GameObject_sS0_
python3 scripts/objdiff-cli.py _ZN14NuVoiceAndroid13OnPlayerEventEj
python3 scripts/objdiff-cli.py _ZN14NuVoiceAndroid11UpdateQueueEv
```

If another build changes `bazel-bin`, pass `-t` with the target-config
artifact reported by Bazel rather than accidentally comparing a native or
debug configuration.
