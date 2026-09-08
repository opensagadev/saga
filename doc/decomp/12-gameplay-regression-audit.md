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

`UpdateQueue` remains a partial match (52.967%). The decoder's original
`OpenStream` also primes at most two buffers; that limit was not increased.

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

## Flow-loader checkpoint (in progress)

The two-pass `LoadGizFlow` parser and its box, collapse, condition, action,
and gizmo callbacks are reconstructed from the original binary. Timer,
random, and special constructors now return the registered gizmo, as the
loader expects. Runtime allocations use the recovered structure sizes.

This remains unfinished: `ResetGizFlow`, `ResetGizFlowPointers`, and
`ProcessGizFlow` still need reconstruction before level-object visibility
and progression can be considered fixed. No successful gameplay result is
claimed for this checkpoint. Initial target comparison gives `LoadGizFlow`
79.453%, `createGizSpecial` 87.879%, `createGizRandom` 13.991%, and
`createGizTimer` 0%. The latter two retain their current default optimization
settings; their original optimized instruction shapes remain unresolved.

## End-level results reconstruction (in progress)

The original results-stage table at ELF `0x6236c0` contains 34 records of
`0x20` bytes, including its sentinel. Registration, coin counting/drawing,
prompt menu drawing/input, save-stage control, packet finishing, and
exit/fade updates have been reconstructed from the original functions.
`STATUSPACKET_s` remains `0x14c` bytes; callback and result fields now have
types matching their original offsets.

This is **not yet a fix for the black screen**. `InitStatusScreen` remains
unimplemented, as do several award draw/update callbacks. Its original
`0x18ab`-byte implementation creates the mode-specific stage sequence and
performs completion/reward bookkeeping. Do not bypass these stages or
substitute unconditional rewards to make the screen advance. Original
pseudocode alone is insufficient to call the reconstruction matched.

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
builds pass. No end-level runtime success or Cantina return has been
verified for this work yet.

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
