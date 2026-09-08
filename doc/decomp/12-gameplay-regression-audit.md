# Gameplay regression audit

Initial report: character switching, cantina AI movement, object/character
collision, pickup/build audio, Force availability glow, and incorrect level
object state (completed cantina builds and a blue minikit in story mode).

These are open regressions. A reconstructed function or passing build does
not establish that a reported gameplay issue is fixed.

## Force availability rendering

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
threshold. No visual gameplay validation has been performed.

## Character switching

`CanPullLevers` (`0x46b150`, 59 bytes) now returns the original model-flag
capability result, at **99.882%** matching (relocated operands differ). Its
integer-return declaration and definition replace the void stub; the definition
lives with the player capability helpers at the original optimization level.
This supplies another dependency of the still-incomplete `InitPlayerAI`.

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
`Player_ToggleCharacter` and its `NewPlayerCharacter` dependency remain empty.
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
establish that all AI movement is correct: `InitPlayerAI` still contains only
its first original operation, `StarWars_AutoSetAICapabilities`, while the
original also initializes masks, capabilities, movement and action state and
calls the recovered reset.

The pre-existing `Player_CopyEssentials` reconstruction was checked against
Ghidra and objdiff: `_Z21Player_CopyEssentialsP12GameObject_sS0_` matches 100%.
This only verifies the copied state fields.

`Player_ToggleCharacter` is still an empty stub. Its original implementation
returns in the hub and when Free Play is off. It is separate from ordinary
tagging. There were already uncommitted changes in `Tag_Check`, `TagCode`,
and deferred player-tag handling when this audit began.

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
