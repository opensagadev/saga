# Low-match structural triage (2026-09-26)

Baseline: `main` commit `1b3725b5f4f329724a0117b3fd910a31bf570840`.
Both before and after reports use the local reference SHA-256
`30d3fe55503416139a1d0f9b25fc73924aab4b12dd731aafc08b4a1c9bd950d5`.
The baseline was regenerated from the unmodified linked library, not assumed
from the committed report. This pass targets fuzzy function matching, not
byte-for-byte ELF layout, GOT ordering, or data placement.

## Retained changes

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `NetworkObjectManager::SendPushMessage` | 0% | 74.93% |
| `NetPredictor::AllowPush` | 0% | 57.09% |
| `AdjustLayerBits` | 0% | 45.80% |

- `ReplicatorData` has four words, not three. Retail `AllowPush` copies offsets
  `+0`, `+4`, `+8`, and `+0xc` at `0x52e620`–`0x52e637`. Preserve the otherwise
  unused final word and assert its target layout.
- `AllowPush` returns `i32`, not `bool`: its virtual caller tests EAX at
  `0x532847`. The base and all overrides now agree. Its two cursor allocations
  are separately aligned, and the allowed result shares the timestamp update.
- `SendPushMessage` uses a pointer loop over the eight peer entries. The former
  integer-index loop was fully unrolled at the existing `-O3`, producing 3,127
  bytes versus retail's 810. The pointer loop produces 770 bytes without
  changing the optimization setting.
- `AdjustLayerBits` uses the signed layer indices with their `-1` sentinel,
  and branches to OR the Geonosian layer into the existing mask. Retail
  sign-extends both indices and does not emit the reconstructed extra `& 31`.

Overall fuzzy matching: **62.948578% → 62.977623%**. Six function scores
improve and one regresses; no full matches are lost. The ABI-correct fourth
word changes local stack layout in `CalcReplicatorDataSize`, whose score
changes from 69.32% to 69.21%. Do not shrink the structure to recover that
small score difference.

## Rejected or deferred experiments

- Moving the unchanged `NuSoundWeakPtr<T>::Set` body outside its class restores
  the retail call in `NuSoundDecoder::RequestBuffer` (`0x31fb62`), improving
  that function from 0% to 46.06% in the linked library. However, it also
  regresses multiple streamer functions and slightly reduces the aggregate
  score. The shared-header change was reverted. A future attempt must check
  all weak-pointer callers, not just `RequestBuffer`.
- `ShoveSystemCheckGameObject` remains 0% with its assigned `-O1` after a
  count-cache/control-flow experiment. An isolated diagnostic compilation
  of unchanged source at `-O2` reaches 82.47872% against the relocatable object.
  This is evidence to audit translation-unit/optimization provenance, not
  permission to change the map or add a function optimization attribute.
  The experiment and configuration change were not retained.
- `LevelComplete_LSW_Draw` is a 274-byte retail wrapper with a compiler-generated
  `.part.27` callee. The reconstruction is a 2,467-byte unsplit function.
  Active-first and nested control-flow rewrites and a temporary caller probe
  did not reproduce splitting. All were reverted; do not hand-author a
  register-ABI clone or repeatedly rewrite this body without new evidence.

## Verification

The target build, all five `//scripts/checks:checks` tests, symbol-surface
check, and `git diff --check` pass. The symbol check reports no missing
required symbols and no extra-symbol baseline drift. `matching.json` and
the README badge were regenerated from the final linked library. No native,
WASM, or gameplay execution was performed in this pass.

## Arcade reconstruction batch

Baseline: `5dd0ad35`. The nine remaining arcade stubs now implement the
retail scoring, kill counters, coin thresholds, save progression, panel, and
end-menu behavior. Definitions follow the original function order. The
mobile `Arcade_BothPlayersActive` really is a constant true function; do not
replace it with a speculative player-presence check.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `Arcade_AwardPoint` | 5.12% | 100% |
| `Arcade_PlayerKilled` | 11.35% | 100% |
| `Arcade_Kill` | 15.00% | 100% |
| `Arcade_DrawEndMenu` | 10.50% | 100% |
| `Arcade_UpdatePanel` | 9.55% | 99.93% |
| `Arcade_DrawPanel` | 6.67% | 99.89% |
| `Arcade_UpdateEndMenu` | 11.05% | 99.92% |
| `Arcade_AIKilled` | 4.88% | 99.28% |
| `Arcade_CoinCollected` | 4.47% | 99.35% |
| `Blowup_Activate` | 0.13% | 17.28% |

Structural findings:

- The per-player kill arrays occupy `AreaGlobals + 0x24` and `+0x2c`.
  Named array aliases preserve the old field names and layout assertions.
- `Arcade_Score` is a separate two-word array from `Arcade_Points`.
- The per-level mode mask is an eight-bit value. Keeping it as a `u32`
  unnecessarily widens the test and introduces another saved register.
- Player/AI/coin entry points reject unsigned player indices greater than
  one. AI and coin thresholds use unsigned comparisons.
- Three near-full panel/menu scores differ only in local data or literal
  addresses. The scorer still penalizes these slightly. Do not spend a
  source-reconstruction pass trying to fix them by moving constants or
  changing the scoring metric.
- The opponent expression `(player_index + 1) & 1` retains the retail mask
  but emits `add` where retail uses `xor`. Equivalent XOR/complement forms
  let GCC remove the mask and score slightly lower. No optimizer override
  or instruction-forcing workaround was retained.

`Blowup_Activate` now groups activation-only work and updates its flag fields
directly. Two adjacent functions change only the scratch registers in one
load/test pair each: `Blowup_SetVisibility` 99.57% → 99.24%, and
`Blowup_AddGizmos` 87.93% → 87.58%. The complete batch remains net-positive:
**62.977623% → 63.022400%**, with four new full fuzzy matches and none lost.

Further unsuccessful experiments were reverted:

- Debris collision Y-expression grouping, one-based loop traversal, ordered
  lifetime gates, and cached sample times did not recover the retail loop
  shape. A cached-time variant reached only 5.72% against an object file.
- `refpack` explicit hash initialization, zero-length copy guards, and hash
  update placement still scored 0%. Revisit only with new structural evidence.
- Equivalent branch/arithmetic forms of `Player_HasDoubleBoltDamage_FromBolt`
  and `InstantKillParts` generated the same instructions.

Verification: target compilation and the complete linked report succeeded.
A focused 32-bit harness linked the actual NDK-compiled arcade object and
passed assertions for invalid player indices, disabled arcade, score/reset
gating, all three mode-save bits, completion/autosave, AI thresholds, coin
mode precedence and resets, null players, end-menu timing, and draw gating.
This is not a full gameplay run. The temporary harness is not a canonical
repository test target.

## Credits, icon animation, and particle scaling

Baseline: `33650891`. Five more stubs are reconstructed without changing
their source files' optimization settings.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `Credits_Load` | 0.87% | 39.10% |
| `Credits_DrawPanel` | 3.00% | 99.91% |
| `Credits_UpdateMenu` | 2.31% | 81.00% |
| `UpdateIconWibble` | 3.09% | 99.71% |
| `CreateScaledPARTEffect` | 2.98% | 58.41% |

Overall fuzzy matching: **63.022400% → 63.094010%**. Eight scores improve,
none regress, and no full matches are lost. Reintroducing the writable
credits duration also improves `Credits_GetInfo` from 92.73% to 99.95%.

- Credits entries and styles are both 24 bytes. The loader owns at most
  1,000 entries, reads `stuff\\text\\english_credits.txt`, accepts the first
  positive duration override while the duration is still its 120-second
  default, and maintains per-style colour overrides. Title/name entries
  share a row. The completion panel and skip/audio/fade state machine are
  reconstructed from the same original TU.
- The four icon angular velocities are a previously missing private `i32`
  array. Each icon updates its own timer, consumes two random values on
  expiry, and wraps its angle through an integer conversion. A normal
  inline helper called for the four channels reproduces retail's structure;
  a rolled loop scored only 23.77% against the object.
- Particle scaling follows the parent type stored at `+0x174`, reuses the
  closest active relative scale within retail's **1.1** tolerance, and clones
  a free type otherwise. The source type's name is shortened to 12 characters
  before its three-digit suffix. Slot zero is populated but its allocation
  still returns the previous closest ID, exactly as retail does. The input
  check accepts 128 despite the 128-element table; normal loaded IDs remain
  below 128. This pre-existing binary edge case was not silently changed.
- `Credits_DrawPanel` and `UpdateIconWibble` have the original instruction
  structure; their remaining score differences are data/literal addresses.
  Explicitly splitting the loader's initialization/search/colour and style
  cases did not improve its score. The simpler, better-scoring form was kept.

The target build and full linked report pass. A temporary 32-bit test harness
compiled the same sources with the NDK and unchanged optimization settings;
function/data sections were enabled only in the test objects so unrelated
engine functions could be discarded at link time. Tests pass for credits
loading, duration/colour overrides, paired rows, draw clipping, the 1,000-entry
cap, allocation/parser failure, completion rendering, skip/audio/fade timing,
icon random-call order and angle wrapping, and particle reuse/allocation.
These are focused tests with mocked external services, not a full gameplay run.

Further structural triage: retail `NuFrameEndBgLoadPS` and
`UCStretchToCorners` exhibit unoptimized frame/temporary patterns while their
current owners compile at `-O2`; no source churn or optimization override was
attempted. `Push_UpdateHints` has eight unrolled retail player checks versus
a rolled current loop. Resolve these provenance questions before spending
another speculative matching pass on them.

## Challenge rewards and asteroid levels

Baseline: `4ce3464d`. Eight low-matching stubs are reconstructed, with the
existing optimization settings unchanged.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `ChallangeCash_Update` | 2.49% | 70.88% |
| `AsteroidChaseB_Init` | 4.00% | 99.49% |
| `AsteroidChaseB_Draw` | 16.15% | 99.92% |
| `AsteroidChaseB_Update` | 2.75% | 91.38% |
| `AsteroidChaseC_Init` | 3.13% | 99.73% |
| `AsteroidChaseC_Update` | 2.91% | 90.68% |
| `AsteroidChaseD_Init` | 0.84% | 93.63% |
| `AsteroidChaseD_Update` | 0.86% | 99.78% |

Overall fuzzy matching: **63.094010% → 63.2454%**. Eleven scores improve,
one decreases slightly, and no full matches are lost. `Asteroids_Reset`
changes from 80.80% to 80.75%; a direct before/after disassembly comparison
shows only local-data/literal references moving, not changed instructions or
control flow.

The useful structural correction in this batch is source ownership:
`DrawFalconSpotLights` and its four private timer arrays were stranded in
`render.cpp`, despite being used by the asteroid B initialization/update/draw
group. They now live together in `episodeV.cpp`. Both effective Bazel compile
actions use `-O3` and the same target options. Removing the now-unnecessary
`__used__` retention attribute from this genuinely called static helper lets
GCC infer retail's register argument automatically. No calling-convention
attribute is introduced. The helper itself improves 97.87% → 99.93%, and its
draw caller improves from an initial reconstruction at 90.92% to 99.92%.
The helper body and its pre-existing local matrix alignment are unchanged.

Other recovered contracts:

- The final asteroid is a 60-byte record: one special, eight blowup pointers,
  a signed 16-bit count, three signed rotation speeds, and three signed
  rotations. Its network packet is eight bytes. Initialization caps target
  collection at eight and replaces near-zero random speeds; clients seek the
  host angles while the host advances and publishes them.
- Asteroid B collects up to eight classic blowups but awards pickups for the
  first four, once per destruction/reappearance cycle. Falcon lights wait
  until the frame after the timer reaches two seconds and likewise clear
  one frame after fading to zero. The signed 32-bit area-mask shift is kept
  as observed rather than silently widened to a different 64-bit operation.
- Asteroid D retains an old turret pointer when its named gizmo is missing,
  but replaces it when a gizmo is found, even if the object is null. Fourteen
  of the sixteen turrets receive escape-ship targets; indices 8 and 9 are
  intentionally skipped. Existing targets are not overwritten. Destruction
  decrements the counter once, clients do not advance the completion timer,
  and story/free-play routes differ after twelve seconds.
- Challenge rewards emit 50 coin messages only when crossing one second,
  use three random calls per message, and stagger delays by 0.1 seconds.
  The empty-queue check at 1.5 seconds precedes the burst check, so a large
  initial timestep can skip the burst. Stage completion uses strict `>`.

No attempt was made to force missing stack realignment in the challenge or
asteroid C update functions. Asteroid D's initializer uses a single clear of
the 96-byte escape array; retail redundantly clears pieces of that storage.
These residual shapes are recorded instead of adding score-only attributes
or redundant clearing solely for an instruction match.

The target build and full linked report pass. A temporary focused 32-bit
harness links the NDK-compiled source objects, with function/data sections
enabled only for test dead stripping. Assertions pass for the 50-message
reward burst, random-call order, message fields, queue/timing boundaries,
stage advancement, asteroid B pickup/reset and spotlight draw/fade gates,
asteroid C target cap/reinitialization, host/client rotation and translation
preservation, and asteroid D missing/null turrets, target mapping, cutscene
fire interval, one-time destruction, and both completion destinations.
External services are mocked; this is not a full gameplay run.

## Hoth wave controller and background creatures

Baseline: `eef5a5a4`, after rebasing onto main's cutscene-flag correction
(`2bdd275d`). Both remaining Hoth controller stubs are reconstructed at the
existing `-O3` setting.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `HothBattleE_UpdateWave` | 0.59% | 82.54% |
| `HothBattle_ManageBackgroundCreatures` | 0.99% | 71.58% |
| `HothBattleE_Init` | 98.74% | 99.84% |

Overall fuzzy matching: **63.2454% → 63.3283%**. Three scores improve and
two decrease slightly; no full matches are lost. Direct before/after
disassembly shows only moved data/literal references in
`SpawnMeleeCreatureType` (56.79% → 56.77%), and those plus one scratch-register
load/test change in `AsteroidChaseC_Init` (99.73% → 99.62%). Their behavior
and control flow are unchanged.

Recovered contracts:

- The global melee timer is a float at `+8`, not the first word of wave
  zero. Four 40-byte wave records start at `+0xc`; six background-creature
  pointers start at `+0xac`. The complete record remains 200 bytes. Layout
  assertions preserve all established accesses, while named fields expose
  the transition state and initial, remaining, and active counts.
- Wave changes wait five seconds except for the first wave, select normal
  or low-end camera scripts, wait for the cutscene to begin and end, and
  retry wave startup until it succeeds. Final-wave completion clears the
  record's tail and routes story/free-play differently, then continues the
  camera-state setup as retail does.
- Cleanup tests character **model flags at `+4`**, not the separate flags
  at `+0x40`, and excludes AT-ATs and player objects. Dead wave objects and
  those with a nonzero owner-index byte are removed and counted once, with
  the original pitch cue.
- Wave three creates two antinodes using the completed spawn-loop index
  for both radii. The signed 32-bit exclusion-mask expression is retained.
  Do not replace either with a superficially more natural per-object value.
- Background spawning caps probes/AT-STs at 5/6 normally and 2/2 on low-end
  devices, suppresses creature types already present in the wave, and
  assigns each successful spawn to its locator. Locator lookup still occurs
  before the disabled/transition gates. Failed probe creation breaks into
  cleanup/AT-ST handling; a missing locator returns immediately.
- Reverse background cleanup preserves retail's handling of null holes;
  it is not a generic compact-all-nonnull operation.

Two bounded source-form experiments were retained: declaring the AT-ST
limit before the probe limit and expressing the loop comparisons in retail
operand order raised the background object comparison from 50.66% to
71.16%; separating the positive transition phase from the cutscene gate
raised the controller from 81.45% to 82.11%. Linked scores are slightly
higher. Remaining differences are predominantly branch placement, stack
slots, and register allocation; no optimizer or calling-convention
attributes were added.

The target build and full linked report pass. Focused 32-bit tests link the
actual NDK-compiled source with external services mocked. Assertions cover
all transition phases and delay boundaries, normal/low-end scripts, missing
scripts, object cleanup and one-time removal, count repair, antinode radii
and masks, both final destinations, normal/low-end spawn limits, locator
assignment, null holes, and locator/object allocation failures. These are
not a full gameplay run. Function/data sections are enabled only in the
temporary test object for dead stripping, not in matching builds.

## Hoth panel, shared level allocation, and bonus pickups

Baseline: `4ea624d4`. Four further stubs are reconstructed without changing
optimization settings.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `HothBattleE_Panel` | 0.88% | 78.59% |
| `ChrisAllocLevelStuff` | 6.27% | 99.97% |
| `LegoCity_Update` | 0.98% | 57.66% |
| `NewTown_Update` | 1.36% | 59.51% |

Overall fuzzy matching: **63.328278% → 63.422913%**. Six scores improve and
four decrease slightly; no full matches are lost. The three Cloud City
handlers (`CloudCityTrapA_Update`, `CloudCityTrapB_Update`,
`CloudCityTrapC_Panel`) and `DeathStar2BattleD_InZapRange` have only changed
scratch-register load/test pairs and data/literal references in a direct
before/after comparison. No behavior or control flow changes in those bodies.

The allocator exposes an important missing contract: dogfight state reserves
`0x63ef4` bytes, not the formerly described `0x62ef0` prefix. Its last known
word at `+0x62ef0` is cleared; a further 4 KiB remains opaque. The recovered
float at `+0x62ee4` is normalized speed, initialized to one and replaced by
socket speed divided by eleven when nonzero. The 256-record reset arena is
inside the allocation at `+0x5ce90`. No whole-block clear is added.

The same allocator reserves a `0xaf24`-byte pod-race record for the three
pod-race levels. `PODRACE_s` and its existing pointer-bearing lap records now
live in a shared header so both allocation and initialization use the same
`sizeof`, including on 64-bit hosts. The source definitions and target layout
are unchanged. The world flag at `+0x511c` is named and asserted. Other levels
clear that flag but leave the existing data pointer alone, as retail does.

Hoth's panel uses local 16-entry target/defeated arrays and a persistent
16-float alpha array. Waves one/two insert a row break after half-plus-one
targets; wave three has one row; wave four places its AT-AT first, then probes,
a separator, and AT-STs. Defeated targets seek alpha 0.4 by steps of 0.1;
other entries reset to one. A mini cut suppresses all drawing. Clients forward
the existing 88-byte packet's 12-entry arrays and count directly; the retail
panel does not publish a host packet, so no inferred publication was added.

The city/town updates scan all eight player slots for live vehicle objects
with owner-index byte zero whose linked rider is in context `0x3b`. Occupancy is stored
as **0/255**, not boolean 0/1. Only an occupancy change updates pickup groups,
and pickups with runtime bit 8 are excluded. Occupancy is remembered even if
the pickup array is null or empty; a missing world/system returns before
changing that history. Lego City maps tractor/tauntaun/mooncar/towncar to
groups 3/4/5/6; New Town maps tauntaun/firetruck/lifeboat to 2/3/4.

Focused tests pass against the actual NDK objects for all Hoth panel rows,
alpha transitions, cutscene suppression, client forwarding, allocation sizes
and fields, zero/negative/NaN socket speeds, and all city/town player slots,
eligibility gates, duplicate riders, entry/exit, absent pickup arrays, and
2,000 randomized occupancy transitions. The allocator tests also pass with
a 64-bit host object: space/pod-race allocations grow to 409344/45096 bytes
from the target's 409332/44836 bytes. External services are mocked, and this
is not a full gameplay run. The full target and native builds also pass.

Additional bounded experiments were not retained: swapping GEONOSIAN fall
animation tests did not recover its branch layout; expanding the eight rumble
checks through an ordinary inline helper still had different cold-block
placement (36.20%, 815 bytes versus retail 707). A ternary occupancy-change
expression slightly helped one bonus level but hurt the other; the simpler
XOR form is retained. These are not reasons to change optimization settings.

## Signal occupancy, suit exchange, and Death Star lightning

Baseline: `9cf24397`. Three further stubs are reconstructed at their existing
`-O3` setting.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `Signals_Update` | 0.92% | 95.30% |
| `Signal_MoveCode` | 1.87% | 33.40% |
| `DeathStar2BattleD_Update` | 1.77% | 93.08% |

Overall fuzzy matching: **63.422913% → 63.505257%**. Ten scores improve,
two decrease, one reaches 100%, and no full matches are lost.

- Signals consume one random value even while inactive. Active unused signals
  copy `AddGameMsg_Default`, override the text/position/colour/flags fields,
  and fade toward one. Used signals scan all eight players and retain their
  in-use bit only for the matching character inside both geometric bounds;
  their scale fades toward zero. Inactive signals clear the bit and scale
  immediately. The squared proximity constant is `0x3efae14a`, reproduced by
  `(0.6f + 0.1f) * (0.6f + 0.1f)`, not rounded to `0.49f`. Both comparisons
  are strict and reject NaNs. The ordinary eight-element loop naturally
  unrolls with the existing compiler settings.
- Signal movement owns context `0x4c` and animation `0x96`. Missing animation
  data advances the timer; present-but-not-playing animation data pauses it.
  Exchange happens at the animation marker or completion, swaps the stored
  suit pointers, replaces capability bits, and records the new suit among
  the ten area suit bits. The old exchange bit is captured before
  `StartEndOfJump`; the subsequent feedback test reads the possibly updated
  bit. No inferred character reload or sound effect is added.
- `GameObject + 0x788` gains a typed signal-pointer alias while retaining the
  opaque alias and its offset assertion. This layout-neutral union changes
  GCC alias analysis in existing consumers: `Grapple_LookAtPos` gains its
  retail reloads and reaches 100%, and `GizGetBuildItPlayerPos` rises from
  76.42% to 93.20%. Direct before/after inspection shows the two decreases
  are one extra pointer reload in `MechTouchTaskPullLever::Update`
  (62.44% → 61.60%) and two reordered independent loads/stores in
  `Grapple_DrawLine` (54.44% → 54.41%), plus relocated data references.
- Death Star lightning traverses player pointers without unrolling, enables
  and positions each light halfway to the target, draws the beam, applies
  feedback and disorientation, and preserves all four random draws per
  affected player, including the otherwise unused first draw. A destroyed
  inner shield suppresses that traversal and can emit a pickup on integer
  timer transitions into multiples of three. Pickup direction uses cosine
  for X and sine for Z; random pickup radius is 10–25. Global rumble is a
  separate optional prelude and does not suppress either path.

Bounded experiments: moving the signal fade-target initialization emitted
identical code. Alternate suit-exchange branch/goto layouts scored worse;
the straightforward conditional form is retained. The remaining suit score
is principally a basic-block-placement problem, not a reason to add compiler
attributes. Named Death Star temporaries and explicit colour assignments
slightly improved its object comparison; its emitted size equals retail's
1,136 bytes.

Target and native builds pass. Focused tests link the NDK-compiled functions
with external services mocked and also pass against 64-bit host objects.
Coverage includes all eight signal/player slots, message-default preservation,
strict geometry and NaN boundaries, 2,000 randomized occupancy frames, every
signal-entry gate, animation/timer transitions, all ten suits and an external
suit, capability replacement, single exchange, and feedback. Death Star tests
cover all players, light/beam vectors, random consumption and threshold
boundaries, missing/inactive shields, timer crossings (including negative
times), pickup parameters, and the global-rumble prelude. The previous
2,000-frame city/town pickup regression also passes. Tests use function/data
sections only in temporary test objects; matching builds are unchanged. No
full gameplay run was performed.

## Batarang flight and duplicate placeholder ownership

Baseline: `06f6edb7`. The Batarang changes retain the source's `-O3` setting.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `Batarang_SeekToTarget` | 0.64% | 93.27% |
| `Batarang_InitRicochet` | 6.63% | 85.60% |
| `Batarang_Ricochet` | 68.39% | 99.97% |
| `seed_chase` | 2.61% | 100% |
| `_fseek64_wrap` | 20.00% | 100% |
| `ParseAIPathCnxFlag` | 7.50% | 86.05% |

Overall fuzzy matching: **63.505257% → 63.560116%**. Six scores improve,
none decrease, two reach 100%, and no full matches are lost. The reference
code denominator remains 4,722,419 bytes and 13,459 function records.

Flight now uses the existing `Batarang_GetTargetPos`, moved unchanged from
`bolts.cpp` to its actual caller's `batarang.cpp`, instead of a simplified
substitute. The original local entry point remains emitted using its existing
retention annotation: retail also calls it from an unreconstructed target-marker
path in `Batarang_MoveCode`. Removing retention with only the current caller
inlines it and loses the required symbol. Its remaining private-register-ABI
difference is deferred until that real second caller is recovered; no calling
convention attribute or artificial call is added.

The seek path distinguishes a lost/out-of-range target from helper failure,
uses the owner's joint when returning in flight, accelerates steering after
one second, and ray-tests only before four seconds. Target type and platform
identity determine whether an impact starts a ricochet. Non-ricochet flight
decays the signed ricochet count, integrates velocity, and tests a strict
0.25-unit arrival radius. The ricochet timer tests its value at entry before
advancing, rather than expiring one frame early. Initialization rotates and
combines normalized vectors, retains 80% speed, and seeks each component by
ten; the old simplified 75%-speed reflection was not retail behavior.

Five unused nine-byte local placeholders duplicated real linked functions:

- `episode.cpp::seed_chase` and `legoapi_misc.cpp::_fseek64_wrap` duplicated
  the pinned libvorbis implementations in `psy.c` and `vorbisfile.c`.
- `edpath.cpp::ParseAIPathCnxFlag` duplicated the parser in `aisys.cpp`.
- `render/fx/edsplines.cpp::SplineLength` duplicated the local helper in
  `socksysall.cpp` (the separate public editor function is retained).
- `parts.cpp::UpdateAnimTimer` duplicated the helper used by `apiobject.cpp`.

Each exact local symbol occurs once in retail. Source searches found no calls
to the placeholders; the real bodies and their callers are unchanged. Removing
only the placeholders leaves one emitted symbol apiece and removes three
ambiguous source assignments. The library functions were already 100% matches;
name-only pairing had selected the placeholders. Library ownership is outside
the report's game-source unit list, so these two entries become unassigned
rather than being incorrectly attributed to game stubs. The spline and timer
scores are unchanged. This extends the duplicate trap documented in the audio
audit: inspect all same-name local symbols before reconstructing a stub. A
follow-up scan found no other short/large duplicate pair with a unique retail
symbol except translation-unit startup functions, which were left alone.

Target and native builds and all five repository tests pass. Focused tests
link the actual NDK-compiled Batarang source and also pass with a 64-bit host
object. Coverage includes all target kinds, owner/joint fallback, helper
failure, platform filtering, steering/ray-test timing boundaries, strict
arrival distance, signed count decay, timer NaNs, ricochet limits, and 1,000
randomized rotation/speed samples. External math/terrain services are mocked;
this is not a full gameplay run. Function/data sections are enabled only in
temporary test objects, not matching builds.

## Rancor exclusions and Sarlacc disco display

Baseline: `1f09ba25`. Both reconstructed handlers retain Episode VI's `-O3`.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `JabbasPalaceE_Update` | 0.93% | 75.97% |
| `SarlaccPitB_SpecialUpdate` | 2.22% | 99.85% |
| `SarlaccPitB_Reset` | 87.18% | 92.04% |

Overall fuzzy matching: **63.560116% → 63.639454%**. Five scores improve,
four decrease slightly, and no full matches are lost. Direct before/after
inspection attributes the decreases to literal relocation, a short/long branch
encoding and alignment fill in `EmperorFightA_Reset`, scratch-register choices
and independent load/store scheduling in `SarlaccPitC_Reset`, and scratch
registers in `SarlaccPitC_Update`. `SetLevelSfxBits` changes only literal address
operands. Their behavior and control-flow destinations are unchanged.

The Rancor handler performs host-only boss completion/level routing, then
rebuilds the boss's 64-bit opponent-exclusion mask. A live creature set excludes
all present players; otherwise the three safe areas decide exclusion. The
retail area expression is a **signed 32-bit shift widened to 64 bits**, not a
64-bit area shift: bit 31 sign-extends. The reconstructed expression explicitly
masks the shift count and preserves that widening on both pointer widths.
Player identities use the separate full 64-bit mask. Two proximity explosions
each play their sound once on activation and reset their latch when inactive
or absent. A combined loop condition scored 50.69% in the object comparison;
separate creature-set branches recover the original unrolled paths and reach
75.88% without optimizer attributes.

Sarlacc's private `sarlaccdisco` now has a typed 1,024-byte target layout:
five 16-element special arrays, area/obstacle/message pointers, count, height,
and still-opaque scalar fields. Initializer/reset accesses use typed members
and `sizeof`, so host pointers and special records cannot overlap their
neighbors through hardcoded 32-bit offsets. Target offset assertions preserve
the retail contract. The 20-byte network packet gains its height, six signed
panel masks, active byte, and 16-bit sound latch. The recovered data global
`disco_base_offset` is initialized to retail's `-0.12f`.

Display hides all five variants before selecting the first set mask in
off/flash/select/on/finish order. Clients seek the height toward the packet
**once per panel**, while the host publishes its height once per panel. The
base special follows that height plus the offset, and the sound mask uses the
on-special's matrix translation. Active disco updates its completion message,
three radios, mirror-ball obstacle, floor/light visibility, shutter, and paired
doors; the completion sound is latched until disco becomes inactive. Missing
position/animation pointers are handled exactly where retail checks them.

A shared selected-special variable merged the five display paths and scored
25.58%. An ordinary inline helper called from each original branch preserves
the separate call sites, reproduces the retail 1,580-byte body, and scores
98.98% before linking. No helper symbol or forced inlining is added. The typed
initializer keeps its original instruction shape and score. Reloading the
signed count after each reset iteration now improves the typed reset to 92.04%;
the earlier byte-array experiment in the Episode VI notes had not done so.

Target/native builds and all five repository tests pass. Focused 32-bit NDK
and 64-bit host tests cover boss routing, every player/area slot, signed area
masks, explosion sound edges, and 2,000 randomized exclusion frames. Sarlacc
tests cover initialization for every count 0–16, typed pointer assignments,
reset visibility, all five display priorities at all 16 panel bits, host/client
height updates, missing positions, completion sound reset/replay, and missing
special/animation gates. The 64-bit Sarlacc test also passes AddressSanitizer
and UBSan. The previous Death Star lightning regression passes against the
final NDK object. External services are mocked; no full gameplay run is claimed.

A bounded `Push_UpdateHints` experiment was not retained: eight explicit calls
to an ordinary inline predicate still differ in prologue and branch layout at
the source's required `-O2` (0% versus the existing 0.50%). Changing optimization
settings is not a permitted shortcut.

## Sarlacc puzzle progression and Geonosian fall timer

Baseline: `63f13577`. Episode VI remains `-O3`; `gameanim.cpp` remains `-O2`.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `SarlaccPitB_Update` | 0.39% | 51.34% |
| `Animate_GEONOSIAN` | 0.60% | 54.32% |

Overall fuzzy matching: **63.639454% → 63.696384%**, with four improved
scores, one small decrease, and no full matches lost. The decreased
`DeathStar2BattleD_Init` score (99.91% → 99.86%) is an equivalent exchange of
two stack spill slots, plus literal references; direct before/after inspection
shows no changed behavior. `SarlaccPitC_Update` and `SetLevelSfxBits` improve
incidentally. The matching denominator is unchanged.

The previously opaque Sarlacc fields at `+0x3cc` are sixteen byte panel states;
`+0x3dd` is the phase, the next two signed bytes are selected panel indices,
and `+0x3e0` is a floating timer. `+0x3fc` is a real position pointer, now
pointer-width-aware on hosts. Reset's four-word clear becomes an equivalent
fixed-size `memset` without changing its score. Recovered retail data globals
are `sarlaccdiscotime = 40`, `discoheightseek = 2`, and `discoheight = 1.27`.

Host progression clears sound edges, refreshes three AI messages, queries the
special panel, and requires two completed build-its plus animation-set state
2. Entering the disco area selects two distinct off panels. Both occupied
panels latch on and select the next pair; exhausting the available panels
enters completion and fills the panel states with the finish variant. The
active packet byte changes on the following frame, as in retail. Every 2.5
seconds without simultaneous occupancy, up to two lit panels revert to off.
The first qualifying player in each eight-slot scan determines occupancy and
whether the human-controlled character should request the companion's help.
The distance test is strictly below `0.2f * 0.2f`, using object position rather
than collision position, and requires both `0x1001` object flags and contact.

The area bit uses the same signed-32-bit widening as the Rancor handler. Host
updates publish five masks and ease the floor toward the target height; clients
skip those decisions. Both paths animate the visible engine lump and call the
shared display update. Losing readiness during the puzzle resets and continues
through mask/display publication. Expiry or lost readiness during completed
disco resets and returns immediately, leaving the old panel masks untouched
for that frame. The initial no-panel/one-panel case retains retail's `-1`
indices instead of silently adding a new completion transition.

The first source reconstruction is 4,793 bytes versus retail's 4,747. Tests of
an occupancy variable in the loop condition and of ending the loop by changing
its index both produce 4,729 bytes but slightly worse object matching
(50.86% versus 50.93%); neither is retained. Remaining differences are mainly
block order, scratch allocation, and loop shape. No optimizer/calling-convention
attributes or hand-written assembly were introduced.

Geonosian's timer now selects a floating result and performs one final store,
instead of distinct add/store and integer-zero-store source paths. Its
animation predicates and call order are unchanged. Reordering the three
high-jump animation alternatives gives no useful improvement, so their original
source order is retained. The local helper's compiler-inferred register ABI
remains intact.

Target/native builds and all five repository tests pass. The actual NDK-built
Sarlacc test covers all eight player slots, every init/display count and mask,
4,096 area-index/area-mask combinations, exact distance/time thresholds,
sound edges including bit 15, readiness/reset paths, companion hints, height
clamps, completion timer NaNs, network-client preservation, and engine motion.
It also checks 2,000 randomized occupancy frames. Geonosian tests exhaust all
65,536 signed animation IDs with both high-jump flag states and seven timer
values, including negative, NaN, and infinity, plus animation changes made by
the idle callback. Both test suites pass on 64-bit hosts with AddressSanitizer
and UBSan. External services are mocked; no full gameplay run is claimed.

## Death Star fire rendering and progression

Baseline: `473319dc`; Episode VI remains `-O3`.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `DeathStar2BattleFire_Draw` | 1.60% | 94.22% |
| `DeathStar2BattleFire_Update` | 0.42% | 99.41% |

Overall fuzzy matching: **63.696384% → 63.824688%**, with four improved scores,
one 0.0019-point decrease, and no full matches lost. The tiny
`SetLevelSfxBits` decrease consists entirely of literal-address operands in
`lea` instructions. The two other changed drawing scores improve incidentally.

Draw recovers three independent passes over a local `SPLINEPOS_s`: backward
from the fire front, forward from the rear to the front, then backward from the
rear. Each pass steps 40 world units even when its particles are clipped. The
clip global is **signed integer** `fire_clip_dist = 1000`, converted to float
for a strict squared-distance comparison; it is not a floating configuration
value. The point-along query supplies angles, while particle position comes
from the local spline record. Pitch and yaw narrow to signed 16-bit arguments.
The pauses, matrix initialization calls, and global spline preservation follow
retail. The first source version has the exact 1,680-byte retail size; remaining
differences mainly schedule independent call arguments differently.

Update recovers `fireDeltaPos`, computed from player zero's run speed or a
30-unit fallback, and preserves both random generators' consumption order.
It updates the fire cube, seeds the rear spline when required, then checks all
eight non-excluded player slots in fire-local coordinates. Negative longitudinal
distance emits particles at the **nearest player found so far**, but damage is
applied to the current slot. Controlled-player contact slows the fire. The
hurt-sound suppression flag is saved before emission and restored after a hit;
the impact-sound flag follows retail's one-shot hit behavior. The recovered
0.2/1/1.75 speed decisions include gradual recovery without clamping its final
step. Front movement has the level/startup-timer gate; rear movement is always
backward at the unscaled base speed. Optional explosion audio precedes the
continuous lantern sound. The first source version is 4,557 bytes versus
retail's 4,549, with no matching-only attributes or assembly.

Target/native builds and all five repository tests pass. Focused NDK and
sanitized 64-bit host tests cover all three render passes, exact and NaN clip
boundaries, integer-to-float rounding, signed angles, pause behavior, unchanged
global splines, all player slots, nearest-emitter/current-victim separation,
ignored damage, suppression-flag restoration, speed boundaries/recovery,
startup/level movement gates, spline endpoints and NaNs, random/effect/sound
boundaries, and 2,000 randomized player frames. External services are mocked;
no full gameplay run is claimed.

## Character loading, nearest detonators, and player rumble

Baseline: `447c5d9c`. Character core remains `-O3`; detonators and gamepads
remain `-O2`.

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `UpdateCharacterLoad` | 0.00% | 29.77% |
| `Detonator_FindNearest` | 0.98% | 30.37% |
| `NewRumbleAllPlayers` | 0.95% | 36.56% |

Overall fuzzy matching: **63.824688% → 63.847305%**. Only these three scores
change; none regress and no full matches are lost.

The character loader's integer-index store-pack loop expanded into eleven
copies, making its body 1,698 bytes versus retail's 868. A typed pointer loop
with a separate pack index restores traversal and reduces it to 962 bytes.
The separate index avoids pointer-distance division before each store query.
Fixed-character priority, low-end gates, callback reloads, capacity checks,
collection filtering, and the random candidate calculation are unchanged.
An alternate shared selection label and a value-terminated integer scan were
tested separately and not retained. Remaining block/register differences are
not a reason to change this file's optimization mode.

`Detonator_FindNearest` returns a **`DETONATOR_s *`**, not `void`. Its previously
opaque first twelve bytes are the logical `NUVEC position`; the render position
at `+0xc` is not used for this search. The reconstructed query considers ten
active slots in order, with separate owner-filtered and unrestricted paths.
Zero radius uses a finite squared-distance limit of `1e9`; other radii are
squared, including negative values. Strict comparisons preserve the first
equal-distance entry and reject NaN distances. Ordinary inline calls recover
the fixed-slot structure without adding a helper symbol or forced-inlining
attribute. A compact loop is behaviorally correct but reaches only 4.03% in
the object comparison; fixed-slot calls reach 30.18% (30.37% linked). The
remaining differences include stack storage and branch scheduling.

Player rumble likewise has eight explicit retail slot checks. Ordinary inline
calls preserve null/controller-flag/pad gates, per-call integer conversion,
duration selection, and the reload of later player slots after sound callbacks.
The scaled floating amount is calculated once, as in retail. An early-return
helper form produces the same score as its nested equivalent. No optimization
or calling-convention attributes were added.

`Prompt_LSW_Update` was investigated but left unchanged. Retail indexes the
two activity bytes, tests the adjacent challenge/mission bytes together, and
uses a shared post-scan selection path. Typed aliases alone leave its object
score at 0%; bounded shared-loop variants reach only 10.37–14.59% while still
changing frame and branch structure substantially. All prompt/header
experiments were removed. Do not repeat them without new evidence about the
remaining control-flow or compiler provenance.

Target/native builds and all five repository checks pass. Focused NDK and
sanitized 64-bit host tests cover character-loading gates and priorities,
all store packs, callback mutations, and 5,000 randomized selections; every
detonator activity mask, all owners/slots, strict radius and tie behavior,
NaNs/infinities, unchanged source records, and 10,000 randomized queries;
and every rumble slot mask, controller/pad gates, callback mutations,
duration/rate boundaries and NaNs, and 5,000 randomized cases. Sound/loading
services and the distance callback are mocked; no gameplay run is claimed.

## Customiser accessories and Anakin door state

The next batch raises linked fuzzy matching from **63.847305% to 63.918064%**.
Four functions improve, three scores regress, and no 100% matches are lost.
Both source files retain `-O2`; `chris.cpp` also retains its existing
`-fno-ipa-sra`. No source ownership or calling-convention changes are involved.

| Function | Before | After |
|---|---:|---:|
| `Customiser_DumpAccessories` | 0.72% | 97.16% |
| `Customiser_DrawAccessories` | 1.95% | 24.21% |
| `ChrisAnakinCReset` | 1.36% | 76.78% |
| `ChrisAnakinCUpdate` | 5.16% | 75.58% |

Accessory cleanup uses the existing typed `Accessory[2][9]` resource array.
The loaded flag must equal one, and only model-slot `-1` suppresses a side.
Each named category either removes its scene and clears the scene pointer
after the callback, or restores the original material texture, updates that
material, and destroys the reloaded texture ID. Retail does not clear texture
IDs or the loaded flag, and scene ownership takes precedence over texture
ownership. Eighteen ordinary inline calls recover the fixed-slot structure;
there is no forced inlining or additional emitted helper.

Accessory drawing selects the helmet locator and character side once, then
checks positive piece counts, category exclusions, helmet-layer flags, and
special existence. Each matrix is copied after the existence callback;
reflection state is read after the primary draw. The two matrix locals use
the existing aligned type because retail explicitly realigns their stack.
A post-increment resource cursor improves the object score from 18.50% to
24.07%; the linked result is 24.21%. The remaining frame, saved-piece
selection, and loop-scheduling differences are not solved by adding artificial
padding or changing optimization.

`AnakinC` was incorrectly typed as a `GameObject_s` pointer. Reset/update
establish an array of twelve `0x140` door records, with four matrices, two
special handles, translation state, activity, and a platform ID. The shared
header now expresses that pointer-width-aware layout with target assertions.
The existing volatile pointer and its unallocation behavior are preserved.
`DoorSetupList` is recovered as fifteen `0x28` records; all string pairs and
scalar payloads were compared against the linked retail table at `0x624b80`.
Its last primary name is an empty string, not NULL. The reconstruction keeps
that data and the retail caller contract: allocated storage and twelve
successful door matches, or a null-name terminator supplied by the caller.
It does not invent allocation or a new safety termination condition.

Reset compacts successful lookups into those twelve slots. A failed secondary
lookup leaves the old secondary flag untouched; a successful lookup narrows
the existence result to 16 bits. The original/current matrices use chained
assignment, matching the retail copy order. Returning directly from the
null-name cleanup path raises object matching from 22.71% to 76.53%; changing
the primary lookup from early-continue to nested form makes no difference.
Update preserves inactive records, clamps ordered offsets to their minimum,
transforms both door halves, and reloads callback-visible state. Explicit
stores in the two clamp branches and a three-float position copy improve its
object score from 7.60% to 75.15%, avoiding a compiler-generated SIMD select
without changing floating-point semantics.

`Customiser_AddPartAccessories` is unchanged in source but falls 0.66 points;
before/after disassembly shows register allocation, instruction scheduling,
and literal placement changes with unchanged operations. The `TrueHero` and
`MiniKit` draw scores move by -0.0024 and -0.0017 points respectively from
relocated references only. All three were reviewed; no gameplay correction
was needed in those neighboring functions.

Target/native builds and all five repository checks pass. Focused NDK and
ASan/UBSan 64-bit host tests pass for both subsystems. Accessory tests cover
all category masks, ownership combinations, character sides, model-slot
sentinels, 16 locators, unsigned saved-piece indices, reflection order,
callback mutations, and 5,000 randomized draw cases. Door tests cover
lookup/secondary failures, compaction and inactive tails, preserved fields,
narrowing, all 4,096 activity masks, NaNs/infinities/signed zero, callback
reloads, and 3,000 randomized reset/update cases. Rendering and scene services
are mocked; no gameplay execution is claimed.

## Flight-spline evaluation and XZ intersection

The spline batch raises linked fuzzy matching from **63.918064% to
63.952187%**. Four functions improve, one caller score regresses slightly,
and no 100% matches are lost. `render/fx/edsplines.cpp` keeps its existing
`-O3`; no source moves, optimization overrides, or calling-convention
attributes are introduced.

| Function | Before | After |
|---|---:|---:|
| `CalcSplinePoint` | 1.73% | 53.31% |
| `CalcSplinePointFromDist` | 8.89% | 99.98% |
| `BezierLineEval` | 6.27% | 54.97% |
| `EvaluateSplineXZIntersection` | 1.76% | 78.17% |

`flightspline_s` was empty even though pod creation already accessed its
fields through raw byte offsets. `FlightSpline_Init` establishes a `0x52c`
stride: 64 four-float points, count at `0x400`, total distance at `0x410`,
64 cumulative distances at `0x414`, and ID at `0x524`. The shared pod header
now expresses those verified fields and keeps the remaining bytes opaque.
The first `0xa580` bytes of `PODRACE_s` become 32 such records without moving
the lap entries or changing the allocation size. Pod creation and startup
use this shared type. The loader itself remains a stub.

Point evaluation constructs normalized, ten-unit endpoint tangents, applies
the retail cubic expression to XYZ, and linearly interpolates W. Its fraction
is computed before the index clamp; the upper clamp is the point count, not
count minus two. This retains the retail requirement for valid neighboring
point storage rather than adding different endpoint or invalid-count behavior.
Count and point data are reloaded after normalization callbacks where retail
does so. Endpoint and normalization temporaries preserve their documented
16-byte stack alignment. Snapshotting the outer control points and expressing
the component operations directly improves object matching from 46.33% to
52.45%; individual endpoint stores reach 53.21%. Remaining temporary-lifetime,
frame, and register-allocation differences are left documented rather than
forcing spills or altering compiler flags.

Distance conversion uses the first cumulative distance strictly above the
query, divides its interpolated index by the point count, and forwards one
for distances at least the total length. If no interval matches, it forwards
the unchanged query. Its linked score is 99.98%; the object-level differences
are relocation references. Bézier evaluation preserves the four Bernstein
weights, float operation grouping, input/output aliasing, and zero W.

The XZ intersection query clears both output records before assigning their
spline pointers and narrowed loop flags. Scan bounds use the full-width loop
arguments. The first spline supplies `logical_count - 1` segments; the second
supplies `logical_count`, including its closing segment even when not looping.
Only strictly closer ordered distances replace the result; a zero-distance
hit exits the inner scan, not the outer scan. Finalization measures each
selected segment, scales its stored fraction, and calls `MoveSplinePosition`
with `0.00001f`. Point stride is the retail hardcoded three-float size, not
the spline's `pt_size` field. Conditional count expressions and shared
fraction-local scope raise object matching from 73.95% to 78.03%.

The typed pod caller changes from 36.43% to 36.29%. Before/after disassembly
shows equivalent pointer arithmetic, an earlier load of spline length, and
relocated constants; its function size remains 2,344 bytes. No behavior
correction or optimization change was made to that caller.

Target/native builds and all five repository checks pass. Focused NDK and
ASan/UBSan 64-bit host harnesses pass for both spline subsystems. Evaluation
tests cover counts 2 through 61, segment/fraction boundaries, output aliasing,
normalization callback mutations, distance fallback, signed zero and
non-finite coordinates, Bézier aliases/non-finite parameters, and 5,000
randomized cases. Intersection tests cover counts -2 through 8, seven loop
flag values, null/empty inputs, aliased outputs, strict ties and non-finite
distances, wraparound, callback-modified counts/point arrays/fractions,
finalization order, and 5,000 randomized cases. Geometry services are mocked;
no gameplay execution or invalid-storage support is claimed.

## WEIRDO animation dispatch

Recovering `Animate_WEIRDO` raises its linked match from **1.60% to 58.80%**
and aggregate fuzzy matching from **63.952187% to 63.965984%**. No other
function scores change. The file retains `-O2`, and the three shared private
animation helpers retain their existing local symbols and compiler-inferred
calling conventions. This removes the last `STUBBED()` body in `gameanim.cpp`;
the old comment promising restoration of local linkage after four missing
callers was stale and has been corrected.

The dispatcher preserves context-owned animation, ground/fall gates, jump
priority, forced weapon idle, movement and idle variants, extra-action remaps,
blend checking, and the final idle callback. Fall timing uses the animation
selected after that callback. Saber sound is restricted to weapon IDs 101,
103, 105, and 107; the imperial guard is excluded, the bodyguard has a separate
loop, and the remaining sound depends on the dark-side model flag. Sound
lookup is a separate statement before reading playback volume, matching the
retail callback ordering. That correction improves object matching from
54.31% to 58.52% and prevents an early volume snapshot. Separate lookup calls
inside each sound branch score 57.27%, while explicit fall-path labels emit
the same 58.52% code as the retained structured expression; both experiments
were removed.

Target/native builds and all five repository checks pass. NDK and ASan/UBSan
64-bit harnesses pass for every signed 16-bit weapon ID and animation ID,
both high-jump states, all signed 8-bit contexts with 256 weapon-idle flag
combinations, callback mutations, non-finite movement/timer values, sound
ordering, and 10,000 randomized cases. The test oracle follows the retail
dispatcher and reuses the existing private helpers to test their integration;
it does not independently re-validate those helpers. External animation and
sound services are mocked; no gameplay run is claimed.

### Deferred facing-push-block reconstruction

`NearestFacingPushBlock` is another structural ABI issue: the retail function
returns a `pushblock_s *`, while the current stub returns `void`. Retail gates
on `LEGOCONTEXT_PUSH`, tests cardinal facing windows, scene visibility,
vertical/lateral bounds and packed direction flags, then picks the strictly
nearest candidate inside a squared-distance limit. Two provisional bodies
compiled under the unchanged `pushblocks.cpp -O1` setting produce 1,360 and
1,334 bytes versus 1,474 retail bytes and both score 0%. Capturing the bounds
does not resolve the very different branch layout, folded angle ranges,
duplicated early-return stores, or `0x40` versus `0x50` frame. Both code probes
were removed before integration and are not behavior-validated. Investigate
source/compiler provenance before repeating these trials; do not change the
optimization setting or add matching-only attributes to force a score.

## Detonator movement and placement

Recovering `Detonator_MoveCode` raises its linked match from **0.71% to
37.05%**. Correcting `ThermalDetonator_MoveCode` raises it from **2.02% to
32.18%**. Together they raise aggregate fuzzy matching from **63.965984% to
63.996925%**.
This removes the last stub in `detonator.cpp`, retaining its `-O2` setting,
existing record layouts, and the canonical animation/antinode service ABIs.
The only other report change is **-0.0019 points** for unchanged
`SetLevelSfxBits`: its before/after disassembly has the same instruction
sequence and size, with shifted string-relative operands. No exact matches
are lost.

The recovered state machine distinguishes tap-to-place/pick-up from
hold-to-detonate, including the signed input latch and context gates. Holding
selects the strictly oldest active record across all owners, starting with a
finite `-1.0f` age limit. Both placement scans stop at three owned records;
their first available slot may be an active record belonging to another
object, not just an inactive one. Pickup preparation leaves destination Y
untouched. An unfinished pickup whose animation timer expires deliberately
falls through to placement after clearing its context. The animation-frame
gates, antinode registration/removal, callback-visible target and rotation
reloads, and attachment sound follow the retail order.

Three bounded source-shape trials were measured: compact slot loops score
31.52% in the object comparison; short-circuit slot visitors score 37.26%
but emit an extra out-of-line helper call; separating the slot visit from
its count test scores 36.81% and restores fully inlined scans. The latter is
retained for its retail call structure (37.05% after linking). Remaining
differences include push/pop versus frame-slot register saves, frame size,
branch placement, and frame-pointer lifetimes. No matching-only attributes
or optimization changes were used; defer further tuning until new evidence
explains those structural differences.

Target/native builds and all five repository checks pass. NDK 32-bit and
ASan/UBSan 64-bit harnesses pass for all signed context values, all input
latch bytes with finite/non-finite timer boundaries, all **59,049**
inactive/owned/other-owned slot patterns, **10,000** randomized oldest-record
selections, animation frame/timer/marker edge combinations, pickup and
placement failures, and service callback mutations. These tests exercise the
existing nearest-query and detonation helpers as part of the integration;
external geometry, animation, audio, and antinode services are mocked. No
retail gameplay run is claimed.

The thermal routine previously used a hand-written particle scan with an
invented draw-callback filter. Retail instead calls `FindPart(NULL, 0,
object)`, now declared in its owning `parts.h`. The entry resource gate is
slot **0xe9**, not the thrown model at **0xea**, and does not apply to an
already-running throw animation. Entry preserves the movement request bit,
resets the animation only when `AnimPlaying` succeeds, and clears the context
completion bit after that callback. An unavailable frame freezes the timer;
expiry requests the throw if not already completed; the no-clip event uses
strictly less than half a second. Existing requests propagate to the context
completion bit even when no new frame event fires. Young active particles
also block repeat detonation for unordered ages, following the retail
comparison. One reconstruction trial scores 31.91% before linking; further
branch-layout tuning is deferred.

The thermal harness additionally covers all signed contexts, all particle
activity bytes, all weapon-state bytes and animation variants, resource-slot
selection, no-frame freeze, completed/requested flag combinations,
finite/non-finite age/frame/marker/timer boundaries, optional reset, and
callback changes to context, model, animation, flags, and frame time. Both
NDK and sanitized 64-bit runs pass, with particle lookup and other engine
services mocked.

### Additional low-score structural triage

`DisplaySceneRndrSpecials` has an existing body but its current
`render_stubs.cpp` compile command has no optimization option (therefore
`-O0`), while the retail code shows optimized register/branch behavior.
Resolve source ownership and compiler provenance before trying local
expression changes. No source or build option was changed.

`CollideBoltStarFighter` has a different blocker: its retail private ABI
passes the bolt and fighter in EAX/EDX and returns an integer, while the
current local stub returns `void` and its callers remain unreconstructed.
The fighter type is also empty. Recover the shared fighter layout and real
callers together so GCC can infer the private convention; do not add a
calling-convention attribute to the isolated stub. This pass inspected the
retail body but made no speculative source changes to that cluster.

## Random character selection and balloon release

Recovering `RandomIDFromFlags` raises its linked match from **4.27% to
60.75%**, and `LetGoOfBalloon` rises from **3.93% to 75.86%**. Aggregate
fuzzy matching rises from **63.996925% to 64.009445%**. These remove the last
stub bodies in `charconfig.cpp` and `carrying.cpp`, respectively; their
`-O2` and `-O3` settings are unchanged. No other function scores change and
no exact matches are lost.

Random selection filters model-list exclusions, signed customiser IDs,
default movement, required model/game flags, packed visibility flags,
collection ownership, and optional hat support in retail callback order.
It stores at most 500 signed 16-bit IDs, but continues invoking filters for
later characters. Zero and one candidate do not consume random state;
larger lists use `qrand() / (65535 / count + 1)`, not modulo. The character
array pointer is captured before traversal while the count is reloaded
after callbacks. The existing hat helper is reused without modification.

The first source placement slightly changed register allocation in three
unchanged neighbors, including an exact category lookup. One bounded
definition-order trial places random selection after the category helpers,
consistent with their retail address ordering. This restores all three
neighbor scores without changing random selection's 429-byte body or its
60.41% object score. Ownership, compiler options, and calling conventions
remain unchanged; no artificial register or inlining controls are used.

Balloon release clears context before checking the level resource, resolves
the signed hand locator, constructs its translation matrix and rotated
velocity, and copies particle defaults after the rotation callback. Radius
lookup precedes reloading the resource pointer and particle fields. The
new particle receives the low 16 bits of the object's hit flags only after
successful allocation. One body trial scores 75.35% before linking;
remaining differences include the retail aligned frame and register/stack
layout, which are not forced with matching-only attributes.

Target/native builds and all five repository checks pass. Focused 32-bit
NDK and 64-bit host ASan/UBSan tests pass for both functions. Random selection
tests cover all 32 required-mask bits, packed flag bytes, callback ordering
and mutations, the 500-entry cap, signed ID narrowing, all 65,536 random
values at six representative counts, and 1,000 randomized cases. Its host
harness disables ASan global instrumentation (`-mllvm -asan-globals=0`) so
unused parser callback tables can be garbage-collected; stack/heap ASan and
UBSan remain enabled. Balloon tests retain default ASan instrumentation and
cover all signed contexts, all 16 hand locators, byte flags, allocation
outcomes, default snapshots, and callback changes to resources and frame
time. Engine services are mocked; no gameplay execution is claimed.

### Rejected shared sound-list experiment

`NuSoundSystem::CreateVoice` remains a shared-representation issue. Its
retail intrusive-list conversions preserve null before applying an offset;
the current `NuEListOffset::GetLinks` applies the offset unconditionally.
A null-preserving helper raises this function from 0.74% to 15.22%, but the
full report regresses overall: 14 functions improve and seven regress,
including clock removal/destruction, sample unloading, and voice stopping.
Inverting the decoder-selection branches scores 0%, both with and without
the helper change. All experiments were reverted, and no behavior-validation
claim is made for them. Audit the shared intrusive-list representation and
its callers together before revisiting this cluster; isolated source-shape
churn or optimizing only one caller is not a useful next step.

## Minikit collection, completion burst, and message ABI

Recovering `CollectMinikit` raises its linked match from **3.04% to 68.65%**;
`AddStatusMiniKitParts` rises from **0.77% to 73.41%**. Aggregate fuzzy
matching rises from **64.009445% to 64.062720%**, retaining the source's
`-O3` setting. Each body was compiled in one source-shape trial (67.93% and
72.41% before linking). No exact matches are lost. The unchanged
`CharMiniKit_Draw` improves by 1.06 points through register allocation;
`MiniKit_LSW_Draw` loses 0.03 points through four register-operand changes.
Both neighbors retain their previous sizes; their remaining before/after
differences are relocated operands. These are not behavior changes.

Collection requires resource slot **0xce**. It captures the save-slot index
from `WORLD->level_sub_id`, constructs a model message with panel target,
animation/end callbacks, and starts the panel display timer even if message
allocation fails. Only a valid save-slot index with fewer than ten saved
pieces records a name and narrowed level ID, increments the transient
counts, and emits demo hints at counts one and ten. Retail accepts names of
length eight, including the terminator temporarily overlapping the level
field before that field is written; longer names become empty. The
transient ten-entry array's valid-capacity precondition is preserved.
Debris and pickup sound follow regardless of saved progress. Callback-time
world/count changes are reloaded at the same points as retail.

The completion burst snapshots three panel light colours/directions and
ambient colour into the newly recovered, canonical **0x144-byte**
`rtldata_s KitPartRTL`. Every enabled saved piece emits a particle and five
coin messages, consuming exactly seventeen random values. The coin delay
accumulates by 0.1 seconds across pieces, and only the first coin emits
rumble and debris. Each message chooses its player after copying defaults,
then reloads the current coin scale, resource, score, and duration. Panel Y
and lighting are entry snapshots. Resource IDs retain signed 16-bit
interpretation, allocation failures do not stop the sequence, and panel
lights are restored even when no pieces are enabled. Existing `KitPart`
ownership/linkage is not changed and no aligned-stack attribute is added.

### Shared callback and special-handle representation

The new collection callback exposed a structural bug in `ADDGAMEMSG`:
offset **0x3c** is a per-tick function pointer, not a float. Its existing
offset-0x40 callback is instead invoked during drawing. Retail
`UpdateGameMessages` calls the stored tick pointer at runtime offset
**0x104**, while the previous reconstruction passed it through a `u32`,
truncating pointers on 64-bit hosts. Both delay and tick callbacks now have
function-pointer types in the public, queue, and renderer layouts.

Model messages also store a three-pointer special handle, previously copied
as three floats. The shared union and queue copy now preserve that handle's
full width. All existing `extra_position` callers were checked: they pass
level-object special handles, not position vectors. Target offsets/sizes
are asserted, and native-independent size/callback-offset assertions keep
the queue and rendering views consistent. These shared repairs change no
Android function scores; the target queue's instruction sequence is
unchanged apart from relocated operands.

Target/native builds and all five repository checks pass. Focused NDK
32-bit and ASan/UBSan 64-bit tests pass for all resource/save bytes, ten
transient slots, name-length boundaries, signed level narrowing, demo-count
transitions, allocation outcomes, and service callback mutations. Completion
tests cover all **1,024** enabled masks with eight mutation modes, random
endpoints, five-message delay order, snapshot timing, and signed model IDs.
The real queue tests cover full-width handle/callback round trips, all 128
slots, delayed/tick/end dispatch, text/null-special cases, and rejected
specials. A further integration harness executes the reconstructed
collection, real queue insertion, minikit animation tick, completion, and
expiry together. External engine services remain mocked; no gameplay run
or complete host-rendering audit is claimed.

### Deferred minikit counter completion

`IncrementMinikitCounter` needs additional behavioral evidence before
reconstruction. Its retail completion paths pass the local vector at
`esp+0x58` to `AddGameMsgCount`, but the initial `Players_AveragePos` call
fills `esp+0x4c` and no write to the later vector appears in the function.
The camera offset uses a third vector at `esp+0x64`. Do not silently replace
the uninitialized-position path with an assumed intended position, or add
an uninitialized C++ read merely to imitate its stack shape. This pass
inspected the full body and existing average-position helper; it leaves
the stub unchanged pending caller/retail-runtime evidence.

## Versioned flight-spline loading

`FlightSpline_Init` is reconstructed at the existing `-O3`, with its ordinary
C++ linkage and signature unchanged. The linked match rises from **0.80% to
84.71%**, taking the aggregate from **64.062720% to 64.117065%**. The report
has nine improvements, no regressions, and no lost exact matches; the other
eight improvements are below 0.004 percentage points each.

The full retail body at `0x2389d0` establishes these distinct stages:

- Append the mutable `FSP_Extension` (`".FSP"`) to the supplied world's
  config path, select editor-file media 1, and leave all destination data
  untouched if opening fails.
- Read all spline records before computing or loading distances. Promote
  the recovered float/integer fields at `0x408`, `0x40c`, `0x514`, `0x51c`,
  and `0x520` into the canonical `flightspline_s`; retain unknown bytes at
  `0x404` and `0x518`. The structure remains `0x52c` bytes.
- Versions through 1 default the second float and first trailing integer
  to zero; versions through 2 default the later pair to `-1` and the
  record index. All versions read four floats per point and set `0x528`
  to one.
- Versions through 3 integrate backward from parameter 1 to 0, clamping
  the last step. The step is `PODRACE_SPLINEINC` only when the current
  global world's area matches non-null `PODRACE_ADATA`; otherwise it is
  `0.01f`. These globals are re-read between math callbacks.
- Version 4 consumes the serialized length but replaces it with ten
  samples per point interval, storing accumulated lengths at `0x414`.
  The literals are independently rounded `sample / 10.0f`, not repeated
  additions of `0.1f`. Later versions read the length and table directly.
- Clear only `point_count` in remaining destination slots, then close the
  file. The capacity argument is not a file-count clamp, and a nonpositive
  file count starts the clearing pass at zero. Retail trusts file/storage
  sizes and valid evaluator inputs; no new clamping or input policy is
  invented here.

The first source shape scored 82.755% in the object. Keeping the version-4
sample vectors alive across the point loop restores the retail copy after
the tenth sample and reaches 84.187% in the object / 84.712% linked. GCC
unrolls the ten-sample loop itself. No alignment, inlining, optimization,
or calling-convention attributes were added. Remaining differences include
stack allocation, register selection, and the legacy integration loop;
they do not justify speculative tuning without new evidence.

Verification: target and native builds and all five repository checks pass.
An isolated harness replaces only the evaluator with a deterministic mock
and checks 1,327 complete call/state traces against a retail-offset oracle:
versions -1 through 6, failed opens, nonpositive file counts, zero and full
point arrays, trailing-slot preservation, capacity below file count,
127-byte config paths, pod/non-pod worlds, several integration steps,
and file/math callbacks that change counts and world state. A separate
84-case integration harness uses the real `CalcSplinePoint` on straight
splines and checks lengths, cumulative distances, and untouched bytes.
Both harnesses pass NDK-compiled 32-bit runs and 64-bit ASan/UBSan with
normal global instrumentation. External file services are mocked; this
does not constitute gameplay execution.

## Nearest-obstacle return ABI and arrow rendering

This batch raises linked fuzzy matching from **64.117065% to 64.148240%**:
`GizObstacle_FindNearest` improves **1.98% → 60.98%**, and `RndrArrow`
improves **2.20% → 99.53%**. These are the only changed function scores;
there are no regressions or lost exact matches.

The obstacle query's placeholder incorrectly returned `void`. Retail
returns the selected `GIZOBSTACLE_s *`; its caller in `PushCode` tests the
result and stores it in the game object's obstacle pointer at `0x788`.
The corrected declaration is in `gizobstacles.h`. The existing `-O3` and
symbol name are unchanged.

The query captures the obstacle-array base, but reloads the unsigned
16-bit count after callbacks. It filters by the full integer mode (`-1`
is the wildcard), both enabled/visible progress bits, and the destroyed
runtime bit. A non-null object parameter selects animated average positions
when a set exists; the object itself is not dereferenced. Other candidates
use their stored position. Selection is strictly below `1.0e9f`, so ties
retain the first candidate and NaN distances never win. A null system
returns null without writing the optional distance; an empty/nonmatching
system writes the initial bound. As in retail, animated sets must produce
an average position; the helper's return value is not a fallback selector.
The current partial `PushCode` still lacks this retail caller path; this
batch does not claim to reconstruct the surrounding push state machine.

The first obstacle source form measured 60.778% in the object / 60.984%
linked. A bounded raw-mask versus bitfield experiment generated the same
995-byte function and score. GCC folds the two progress checks into one
mask, unlike retail's two tests; remaining differences also involve loop
unswitching and registers. No compiler or attribute workaround is retained.

`RndrArrow` restores four zero-initialized vectors, the asymmetric arrow
outline, one aspect-ratio snapshot, four in-place rotations, and separate
scale/aspect/translation operations. It submits primitive type 1, format 5,
no material, four colored vertices at z=0, and a final end call. Per-vertex
resolution, stream pointer, and overbrightening state are read afresh.
The existing typed color helper preserves alpha while halving RGB when
required, without touching UV storage. The original `-O2` is preserved.

Combined transform expressions initially produced 938 bytes / 73.702%
object matching. Expressing the three transform passes separately, as in
the retail schedule and nearby quad renderer, yields the original 890-byte
size and 98.932% object / 99.534% linked matching. The remaining 19 linked
instruction differences are color-calculation register/operand choices;
they are not grounds for more speculative tuning.

Verification: target/native builds and all five repository checks pass.
The obstacle oracle passes 134,173 cases on NDK 32-bit and sanitized 64-bit
builds: every progress/runtime byte combination, signed/full-width modes,
strict/tied/NaN/infinite distances, count/base/flag/position mutations,
optional output, and all 65,535 array entries. Its host harness disables
ASan global instrumentation because otherwise unused callback tables keep
the entire obstacle update subsystem linked; the obstacle arrays are heap
allocated and heap/stack ASan plus UBSan remain enabled. The arrow oracle
passes 262,584 cases on NDK 32-bit and full-global ASan/UBSan 64-bit builds:
color-channel and overbrightening bytes, signed angles, floating boundaries,
exact initialization/order, and callback mutations of resolution, aspect,
rotated vectors, and stream cursors. Math/render services are mocked;
no visual or gameplay execution is claimed.

## AI action ownership and gizmo dispatch

This batch raises linked fuzzy matching from **64.148240% to 64.189720%**:

| Function | Before | After |
| --- | ---: | ---: |
| `Action_MoveForward` | 2.04% | 98.82% |
| `Action_PullLever` | 2.23% | 75.99% |
| `Action_UseTechno` | 2.86% | 81.82% |

`Action_MoveForward` had both a global placeholder and a private working
implementation already installed in the AI registry. Promote the working
definition to ordinary external linkage and remove the placeholder. The
registry still points to this implementation; no calling-convention or
optimization attributes are introduced. Its corrected guard uses global
`player`, not `Player[0]`, and checks packet/owner/object in retail order.
It reloads the parameter pointer after each callback, preserves the
two-stage degree-to-angle truncation, and searches for `turn` rather than
`turn=`. The random-direction flag is a 32-bit integer. First-entry yaw,
random consumption, path gating, and captured-object movement are retained.
A valid processor remains a retail precondition after the early guards.
The result is the original 851-byte size, with remaining differences in
stack initialization and register selection.

The two gizmo actions also incorrectly returned `void`, and their script
table entries were null. Both now return `i32` and are bound in
`lego_aiactiondefs`. Retail and linked table contents verify `PullLever`
at index 144 and `UseTechno` at index 146.

`Action_PullLever` parses `lever=` and `instant` before validating the
packet. It preserves an existing target when lookup fails, skips floor
projection for a lever already being pulled, and supports instant setting
of the pull flag, animation frame `0x8000`, and progress 1 without a packet.
Normal operation requests movement, tests the object's lever capability,
uses a strict squared-radius comparison, and presses the current special
button mask. Incapable Free Play characters request a toggle when their
timer goes below zero. Completion requires context `0x4a` and the captured
lever pointer. Sharing the post-parse lever lookup/guards, rather than
duplicating them inside the first-entry block, raises the initial 0%
object result to 75.644%. Remaining block scheduling and register choices
do not justify compiler workarounds.

`Action_UseTechno` captures the object before parsing `name=`, projects the
techno ground position, then requires active/visible/not-complete flags.
It requests movement and, within the strict radius, sets the look target
and calls `GameObjectSetCanUse` with action 2, mode 1, and parameter 0.
Its capability-failure branch preserves Free Play timer/toggle behavior.
`GizTechno_CanUseTechno` really returns 1 in retail; its short existing
body is not a missing reconstruction. The first source form reaches
81.345% in the object. GCC combines the three flag tests; this and block
placement account for much of the remaining difference.

Four function scores improve, including a small register-only change in
the unchanged `Action_FollowCharacter`. Ten unchanged functions lose less
than 0.02 percentage points each from data/GOT operand changes; their
instruction structure is unchanged. No exact matches are lost. The
temporary `Action_FollowPlayer` register regression from the lever-only
build disappears in the complete batch. Source optimization stays `-O3`.

Verification: target/native builds and all five repository checks pass.
NDK 32-bit and full-global ASan/UBSan 64-bit harnesses pass 67,363 forward,
199,508 lever, and 106,419 techno cases. Coverage includes all yaw values,
path bytes, lever flag words, ability/context bytes, techno flag bytes,
signed counts/first-entry values, fractional turn conversion, random
consumption, strict/NaN/infinite distances, timer boundaries, lookup
failures, instant operation without a packet, and callback-driven changes
to parameters, targets, world, radius, object, gamepad, and button masks.
The harness source copies omit only unrelated registry instances so their
unused callback graphs can be discarded; production registry binding is
checked separately. External services are mocked, not gameplay-executed.

## Timing-bar labels and shiny-metal hint traversal

This batch raises linked fuzzy matching from **64.189720% to 64.217150%**:

| Function | Before | After |
| --- | ---: | ---: |
| `TBOPENFN` | 4.36% | 62.78% |
| `TBCLOSEFN` | 3.36% | 91.93% |
| `ShinyMetal_UpdateHint` | 2.04% | 73.88% |

These are the only changed function scores. There are no regressions or
lost exact matches; both source files retain their existing `-O2` setting.

The timing-bar stubs lacked four private twelve-entry label tables. Each
entry contains eleven name bytes and a one-byte type, with a checked
twelve-byte stride. Types 2/3/4/5 select the game/player/AI/draw set. Use
the existing canonical time-bar services and externally owned set IDs;
do not replace them with unused same-named namespace-local HUD stubs.

Opening searches the original count for the first case-sensitive match.
On a miss it reloads the count, checks the twelve-slot limit, copies into
the retail 256-byte local buffer, and ignores empty names. New names are
truncated to ten characters. The count is reloaded after string callbacks
and incremented after the begin callback. Full untruncated names are used
for lookup, so reopening a long name can allocate duplicate truncated
labels; closing the full name will not find that truncated label. Closing
uses a pointer traversal with a snapshotted count and ends only the first
match. Reset changes only counts, not stored labels. Counts in 0..12 and
input names fitting the local buffer remain trusted retail preconditions.

Ordinary `inline` start/end helpers improve matching without forced-inline
attributes. Replacing the close function's index-based traversal with the
retail twelve-byte pointer loop raises object matching from 71.344% to
91.248%. The remaining close gap includes GCC's missing private game-end
clone; do not hand-author a register-ABI clone. Opening remains larger
than retail because of different helper inlining and block generation.

`ShinyMetal_UpdateHint` already had its behavior reconstructed. Its loop
unnecessarily kept the world pointer live across the distance callback.
An explicit count local, refreshed after an unsuccessful distance test,
reproduces retail's register-resident bound between callbacks. It still
reloads both world and count after that call, while preserving the captured
blowup-array base. Object matching improves to 73.332%, with 738 bytes
versus retail's 736. A nested-filter-only experiment produced 0% and was
not retained. Remaining differences are predominantly block placement
and register choices, not missing hint logic.

Verification: target/native builds and all five repository checks pass.
The timing-bar harness passes 15,554 cases on NDK 32-bit and normal-global
64-bit ASan/UBSan builds: all four types, all slot counts, name lengths
0..255, first-match/truncation behavior, invalid types, reset preservation,
callback count/set mutations, and 10,000 stateful oracle operations.
The hint harness passes 23,225 cases on both architectures, including
signed counts, packed filter bits, hint/character/Free Play combinations,
strict/NaN/infinite distances, first selection, palace proximity, and
callback changes to count, world, array, player, hint, and availability.
Only the unrelated `Hints_LSW` dispatch table is excluded from host ASan
registration so its unused callback graph can be discarded; tested arrays,
objects, globals, stack, and heap retain normal instrumentation. External
services are mocked; no gameplay or visual profiling run is claimed.

Additional low-score triage, deferred rather than repeatedly tuned:

- `eduiGradStageSetHSV` already implements HSV conversion. GCC duplicates
  color packing across switch arms. Reusing the canonical HSV helper still
  produces 660 bytes against retail's 446 and 0% object matching; this
  candidate is not retained.
- `WindShear` has a wrong zero-argument placeholder. Retail accepts output
  and input matrices plus signed amplitude and speed/seed arguments. Its
  source ownership belongs with the rendering path, and the LUT angle
  conversion needs an audited policy for large frame-derived values.
  Reconstruct the caller/signature and ownership before implementing it;
  do not invent floating-point guards or matching-only conversion tricks.
- `ImplodeMakeTree` needs missing private heap/length/code helpers and
  shared Huffman state. Its return ABI is also wrong. This is a grouped
  reconstruction, not a useful isolated stub target.
- `NuGScnReadForMultiRender` needs the private graphics reader's ownership
  and scene clone ABI audited together. Retail copies **0x20c** bytes per
  scene, while the current `NUGSCN` declaration ends at **0x1f8**. Audit the
  missing tail and all allocations before adding clone copies; do not
  copy past the current type or expose an artificial private register ABI.

## Socket scene objects and horizontal timing-bar rendering

This batch raises linked fuzzy matching from **64.217150% to 64.254920%**:

| Function | Before | After |
| --- | ---: | ---: |
| `SockParObj` | 3.88% | 99.93% |
| `SockSysSetObjectVisibility` | 8.89% | 100% |
| `SockSysTrackInSplineInfo` | 4.65% | 100% |
| `NuTimeBarSetRenderHorizontal` | 1.50% | 78.29% |

`SOCK + 0xf8` is a pointer to `nuhspecial_s` handles, followed by a
16-bit count at `0xfc`; both offsets are asserted. The parser aligns its
cursor, resolves successive names, advances the handle count only on a
successful lookup, and advances the buffer by the final count. No matches
leave a null object array. Parser callbacks can change the active socket,
scene, cursor, and count; the source retains retail's reloads. The end
pointer is a presence gate, not a capacity check. Sufficient caller-owned
storage and a valid active socket/parser remain preconditions.

Visibility takes `(SOCKSYS *, i32 index, i32 visible)`, not zero arguments.
It captures the selected socket once, then reloads the object-array pointer
and unsigned count after each service call. The track-in query returns
`i32` and takes a system, socket position, optional vector output, and
optional distance output. It preserves the null/-1/valid/track-in gates,
uses the existing real linear spline evaluator, and supplies a local vector
when only distance is requested. Valid socket/segment indices are trusted.
Both functions now match retail exactly under the unchanged default `-O0`.
The parser has only residual local-data operand differences.

The pointer audit also found fixed Android byte counts in `SockSysInit`.
Use `sizeof(SOCKSYS)`, `64 * sizeof(SOCK)`, and natural alignment so the
64-bit structure is fully allocated and cleared. The Android initializer
is instruction-identical before/after this change; the strict capacity
comparison and cursor alignment on failure are retained.

The horizontal profiler's placeholder also had a wrong zero-argument ABI.
It now accepts a set ID, measures label width, maintains unsigned maximum
and recent timing peaks, emits horizontal rectangles and labels, and
performs slot/peak resets. Engine-set suppression happens before font
calls; other sets are required to exist. Width/height rounding, separate
coordinate arithmetic, font/state reloads, and the reset-all special case
for set -1 / slot 0 follow retail. No begin/end-scene calls are invented.
Labels remain trusted internal format strings, as in the original.

The shared unsigned-to-float conversion splits into two sixteen-bit
components. Casting each bounded component to signed `i32` before float
conversion removes GCC's unnecessary unsigned-conversion scaffolding.
This is safe over the complete `u32` domain and does not change the
conversion result. The horizontal function reaches retail's 1,349-byte
size, with remaining register/stack scheduling differences. Its source
stays at `-O2`. The existing vertical renderer changes 5.92% → 5.23% from
conversion/register scheduling, and destruction changes 99.32% → 99.15%
from a register choice. Both diffs were reviewed; no logic changes or exact
matches are lost. Overall, four scores improve and two regress, with two
new exact matches. Do not tune attributes or compiler flags for the gaps.

Verification: target/native builds and all five repository checks pass.
NDK 32-bit and 64-bit ASan/UBSan socket harnesses pass 630,791 cases:
all name-success masks, alignment offsets, empty/65,535-entry/wrapping
counts, guard preservation, callback state changes, all 64 socket slots,
full validity bytes, optional/aliased outputs, real spline interpolation,
floating boundaries, and exact/insufficient allocator capacity. Only the
unrelated parser dispatch table is excluded from ASan registration; all
tested state remains instrumented. The horizontal-renderer oracle passes
334,600 cases on both architectures with normal global instrumentation:
signed counts, engine/initialization/reset combinations, full-width unsigned
timings, exact render/font event traces, callback mutations, and 327,680
unsigned-conversion samples. Services are mocked; no visual/gameplay run
is claimed.

Further read-only triage:

- `Action_BoulderSection` passes the packet, not the script processor, to
  `AIParamToFloat` for `boulder_range=` and `attack_time=`. The callee reads
  processor fields at `+4` and `+0x14`; the bare parameter path passes the
  real processor. Audit this retail ABI/type-punning behavior before
  binding the action. No speculative cast or implementation was retained.
- `BoxTreeRndrRec` needs the owning visibility-tree types and caller to
  reproduce its private EAX/EDX/XMM calling convention naturally. It has
  no reconstructed caller or complete types in its current owner; do not
  implement it as an isolated forced-register-ABI helper.

## Batch 24: texture grid, HTML line graph, and weapon flag audit

Linked fuzzy matching improves from **64.254920% to 64.280630%**:
`MapToGrid` rises from 5.060% to **91.470%**, and `NuHtmlHLineGraph`
from 2.093% to **84.159%**. Two scores improve, none regress, and no
exact matches are lost. Optimization settings and the denominator stay
unchanged.

`MapToGrid` belongs with the texture manager: retail places it between
`NuTexManagerInit` and `NuTexManagerStream`. Remove the character-file
stub and restore the body in `nutex.cpp`. The shared 0x40-byte manager
now names the grid centre, X/Z extents, and signed column/row counts;
size and field-offset assertions preserve the target layout. The
allocator still only aligns/reserves the manager, without clearing it.

Grid coordinates use `(position - centre + extent * 0.5) * (count / extent)`.
Negative values clamp to zero, but the upper test is strictly `>`:
an exact edge remains `count`, while values beyond it become
`count - 0.001`. The two `NuFloor` calls, integer outputs, and fractional
remainders retain retail ordering and alias behavior. Despite its name,
this build's `NuFloor` truncates toward zero. A store-order experiment
did not change code generation; remaining differences are arithmetic
scheduling/register allocation, not a reason for optimization overrides.

The line graph's missing ABI is `(title, width, height, values, count,
maximum, labels)`, not a zero-argument function. Restore its exact
two-stage HTML formatting, integer tick rounding, 16 scan lines per
row, initial eight-step segment, subsequent 17-step segments, zero-slope
one-pixel lines, negative-delta reversal, and final-row slope reset.
Use the real `setpoint`, `setnextpoint`, and `getnextdatapoint` helpers.
The data contract includes `values[count]` as a look-ahead sample for
positive counts, and still reads `values[0]` for empty/negative counts.
Titles and labels are trusted internal diagnostic strings: the retail
256-byte formatter and subsequent format-string writer are preserved,
not made suitable for untrusted input. `maximum` must be nonzero and
the arithmetic/conversions must remain representable.

The fast-weapon audit found a real error independent of the low scores:
`FastWeaponIn` suppresses audio for `gun_on` (0x800), while
`FastWeaponOut` tests `gun_off` (0x400). Replace the misleading shared
constant with distinct names, also used by the existing parser and
automatic transitions. The mask fix leaves fuzzy scores unchanged.
A nested-condition experiment did not improve matching and was not
retained; do not repeat branch permutations without new evidence.

Validation passes on the 32-bit NDK toolchain and 64-bit ASan/UBSan,
with normal global instrumentation throughout:

- **148,226 grid cases** cover signed dimensions/counts, exact and
  near edges, valid infinity/clamping cases, all vector aliases and
  shared index outputs, callback mutations, a 131,073-position sweep,
  and all 16 manager-allocation alignments. NaN/out-of-range
  float-to-integer inputs are outside the retail-defined contract;
  they are not claimed as supported.
- **5,280 HTML graph cases** compare every emitted string and final
  interpolation state, including negative/empty counts, null label
  arrays/entries, rising/falling/constant series, signed widths and
  maxima, rounding, and sample/label changes during writes. Heap sample
  arrays have exactly the required look-ahead capacity.
- **144,480 fast-weapon cases** cover every low 16-bit animation flag
  pattern, signed contexts, all model-mask and weapon-state bytes,
  strict/NaN scale gates, nullable animation entries, Jedi/alternate
  audio precedence, and callback mutations. Sound services are mocked.

Target/native builds and all five repository tests pass. These are
focused output/behavior tests, not a gameplay or visual run.

Further read-only triage:

- `StarFighterAlign` needs the actual `starfighter_s` layout and its
  owning `ProcessStarFighter` call path. The current type is empty;
  retail uses matrix/state fields and a private EAX/EDX/XMM0 convention.
  The partial space-level reset records are not interchangeable with
  that type. Reconstruct the group rather than adding forced ABI casts.
- `CC_sfx_misc` already has the relevant behavior. Retail unrolls six
  slot comparisons, while the established `-O2` build keeps a loop.
  No manual unrolling or flag override was retained.
- `MenuDrawEpisodes` calls the private `DrawEpisodesMenu`, currently
  misplaced as a stub in `hud.cpp`. Audit/consolidate that ownership
  before treating its private register convention as a local mismatch.
- `ReleaseUnreferencedPages_OLD` exposes two issues needing a grouped
  pool audit: the existing 0x400-byte free-list placeholder cannot hold
  256 pointers on a 64-bit host, and the retail rejection path appears
  to revisit the same recycled page instead of advancing. The supplied
  release handlers always succeed, but silently rewriting the rejection
  path would not be faithful reconstruction. No allocator changes were
  included in this batch.

## Batch 25: HTML bar graphs and turret damage/collision

Linked fuzzy matching improves from **64.280630% to 64.359140%**:
seven functions improve, none regress against the published baseline,
and `SphereSphereOverlap` gains an exact match. Optimization settings
and the denominator remain unchanged.

Restore the nine-argument contracts and bodies of `NuHtmlVBarGraph`
(2.542% to **88.475%**) and `NuHtmlHBarGraph` (1.040% to **84.745%**).
The vertical graph uses a 50-pixel axis, an 80-percent height scale,
and descending quarter ticks; the horizontal graph uses a 12-percent
label column, an 88-percent width scale, and ascending quarter ticks.
Both clamp only the upper bar dimension, preserve signed tick rounding,
cycle the supplied palette, and snapshot the value/color before output
callbacks while reloading labels afterward. Negative palette counts
reuse the first entry. Keep the retail two-stage HTML formatting and
the same trusted-string/representable-arithmetic contract as the line
graph. Vertical count/maximum must be nonzero; horizontal count must not
be -1 and its maximum must be nonzero. These were the last two stubs in
`nuhtml.cpp`.

`GizTurrets_Hit` returns a full integer indicating whether it accepted
the hit; the old void stub concealed that contract. Restore signed-byte
health tests and wrapping subtraction, forced destruction, survivor
buzz, random camera judder, animation visibility/role traversal, blowup
and audio dispatch, repeatable pickup rewards, and camera hints.
Callback-sensitive fields and linked-list successors are reloaded at
their retail points. Restore `LEGOHINT_SHOOTCAMERAS` as shared state:
retail initializes it to -1 and game configuration assigns 0x266.
The linked hit score rises from 2.270% to **76.951%**; configuration
initialization rises from 67.250% to **67.711%**.

Restore `GizTurrets_BoltHit` candidate filtering, broad-phase bounds,
reverse sample traversal, first-overlap/strict-nearest selection,
damage dispatch, rumble, deflection, and targeting cleanup. Keep the
otherwise unused bolt-type/cheat calls because they are observable.
Bounds reject with strict greater-than comparisons: unordered/NaN
bounds do not themselves reject a candidate. The candidate-array base
is retained while the system count is reloaded after callbacks.
Retail requires a valid bolt when a candidate is hit, and a valid
`BoltSys` for targeting cleanup; no invented null-bolt behavior is added.
Index-based sample traversal avoids forming an out-of-array pointer
for empty samples.

The shared sphere helper was incorrectly declared `bool`; retail clears
EAX and returns an integer, and its callers test the full register.
Correct the definition and canonical header, removing two conflicting
local declarations. `SphereSphereOverlap` improves from 95% to **100%**,
with a small improvement to `GizmoBlowUp_Hit` as well. Add the canonical
deflected-bolt declaration for the restored call path.

Bolt collision improves from 1.856% to **9.038%**, but its remaining
large mismatch is primarily block order and register allocation.
Two bounded nested-loop/return-shape experiments scored worse (6.932%
object-level versus 8.905%) and were not retained. Adding the real
same-unit caller also changes the hit routine's code generation from
the intermediate 82.78% to 76.95%; the instruction review still shows
the recovered behavior. Do not repeat branch permutations or add
matching-only attributes/optimization overrides to chase these scores.

Validation passes on the 32-bit NDK toolchain and 64-bit ASan/UBSan,
with normal global instrumentation:

- **61,056 cases per bar graph** compare every emitted string, input
  mutations, null labels/palettes, signed dimensions/maxima, palette
  wrapping, out-of-range samples, and callback snapshot/reload ordering.
- **267,530 turret-hit cases** cover byte health and flags, full-width
  damage boundaries, random outcomes, player slots, scores/angles,
  animation roles, nullable sets, rewards/hints, and callback mutations.
- **136,091 bolt-collision cases** exercise the actual hit routine,
  all flag/health/damage bytes, nearest ties, reverse sample order,
  integer overlap results beyond 0/1, owner/type dispatch, NaN/infinite
  bounds, deflection, targeting callbacks, and count/base/state changes.
- **58,249 sphere cases** cover touching/separated bounds, signed radii,
  symmetry, aliases, and NaN/infinite coordinates without input writes.
- The **5,280 line-graph cases** are rebuilt and rerun to check the
  neighboring implementation and shared header.

Target/native builds and all five repository tests pass. External
rendering, collision, audio, pickup, and camera services are mocked in
the focused turret/graph tests; no gameplay or visual run is claimed.

## Batch 26: screen clearing, fade loops, and renderer-owned masking

Linked fuzzy matching improves from **64.359140% to 64.403740%**.
Six functions improve, none regress, and two become exact matches.
The optimization map and matching denominator are unchanged.

- `ClearScreen`: 4.475% to **93.128%**.
- `FadeLoop`: 3.750% to **95.984%**.
- `FadeLoop_SetObj`: 17.500% to **100%**.
- `FadeLoop_UsingObj`: 35.000% to **100%**.
- `FadeLoop_DrawObj`: 10.769% to **77.000%**.
- `RndrMaskScreen`: 1.921% to **93.094%**.

`ClearScreen` draws a normalized four-vertex black quad with 0x80 alpha,
zero depth, and full-range UVs, then restores the coordinate stack.
Use the existing shared primitive helpers, including their float/half
UV representation and callback-sensitive stream cursor. Do not replace
this draw with a render-target clear; they are different operations.

Recover the fade loop's shared scene/special handle and integer
`FadeLoop_UsingObj` return ABI. Object selection stores the scene before
lookup and clears it only on failure; clearing the scene does not erase
the special handle. Object drawing checks existence, applies a 0.125
scale and Z translation of 1, and passes alpha through unchanged.
The retail draw has stack realignment absent from the reconstruction;
no matching-only alignment/calling-convention attribute was added.

The loop fades from 0 to 1 for direction zero and from 1 to 0 otherwise.
Rate is reciprocal duration, or 10 for zero duration. It initializes
`FRAMETIME` from `DEFAULTFRAMETIME`, seeks after frame begin, renders
the optional object and blue/cyan text, runs the optional draw callback,
then ends the scene and enables terrain swapping around frame end.
Store the returned frame time before disabling the swap. Only direction
exactly 1 clears the selected scene and calls `FinishLoop(2)` afterward.
Duration/frame-time inputs must let the seek reach its endpoint;
nonconvergent NaN/negative-time behavior is not silently capped.

`RndrMaskScreen` belongs with the existing `pZClearMaterial` and
`pAlphaMask` statics in `nu3d/nurndr.cpp`, not the gameplay render stub
unit. Remove the misplaced zero-argument stub and recover its contract:
texture ID, clear rectangle, mask rectangle, and coordinate-mode index.
The retail three-entry lookup maps indices 0/1/2 to PS2/normalized/
absolute coordinates. Update the mask texture's low 16 bits, begin a
scene, draw the zero-depth clear rectangle with zero UVs, then draw the
unit-depth textured mask. Each quad separately pushes/restores the
coordinate system; material handles and stream state retain their
retail reload points. Initialization must already have created the
materials, and the mode index must be in range.

Validation passes on the 32-bit NDK toolchain and 64-bit ASan/UBSan,
with normal global instrumentation:

- **23,088 clear-screen cases** verify exact vertex writes, all half-UV
  patterns, untouched storage, cursor changes, and stack restoration.
- **7,974 fade-group cases** verify lookup success/failure and reentrant
  state changes, exact frame/render/callback traces, signed directions,
  zero and positive durations, changing frame times, endpoint colors,
  cleanup, and direct draw alpha including NaN/infinity.
- **69,127 screen-mask cases** cover every low 16-bit texture ID plus
  signed extremes, all coordinate modes, UV patterns, signed/nonfinite
  rectangles, material/cursor callback changes, and stack restoration.

Target/native builds and all five repository tests pass. Rendering and
timing services are mocked; no gameplay or visual run is claimed.

Bounded experiments not retained:

- `GizObstacles_BoltHit` has an audited active-gizmo-array traversal,
  per-sample radius reload, reverse sample selection, and the same
  bolt/cheat dispatch pattern as turrets. A candidate remains 0% at the
  established `-O3`, and an isolated `-O2` diagnostic is also 0%.
  The corresponding turret diagnostic only rises from 8.905% to 10.246%
  object-level. This does not justify an optimization-map change.
  The unvalidated obstacle candidate remains outside the repository.
- Reversing the shared `NuRndrPrimUV` branch order raises ten existing
  callers and lowers two, for +0.0064 aggregate points, but drops
  `NuRndrLine3d` from 21.33% to 3.40% and does not improve either new
  screen function. Restore the original helper instead of retaining
  that cross-caller tradeoff. No exact matches were lost in the trial.

## Batch 27: save-slot rendering and card-warning transitions

Linked fuzzy matching improves from **64.403740% to 64.481260%**.
Six functions improve, none regress, and no exact matches are lost.
The optimization map and matching denominator are unchanged.

- `APIMenuDrawGameState`: 1.308% to **99.268%**.
- `APIMenuDrawMemCardSlots`: 0.844% to **87.167%**.
- `MenuUpdateCardWarning`: 6.885% to **51.033%**.
- The unchanged `MenuDrawSaveConfirm`, `MenuUpdateFormatting`, and
  `DrawMenuEntryEx` also improve slightly. Their before/after diffs only
  change local addresses, jump encoding/alignment, or register choices.

The slot renderer uses the existing four-argument callback ABI: X, Y,
highlight, and slot index. Recover controller/touch colour selection,
unsigned colour-to-float interpolation, signed truncation before byte
narrowing, the 32-byte formatted slot label, and occupied/empty/no-space
messages. Label drawing narrows `MenuA` to a byte, while the following
smart-text call receives its full signed value. Reload scale, alpha,
slot usage, free space, and message colours after the label callback.
The supplied slot must index the six-entry save array, and the formatted
label must fit its retail buffer. No new truncation policy was invented.

The carousel decrements positive left/right slide counters, starts a
ten-frame slide when selection changes, and shows a centred three-slot
window with an extra departing slot while sliding. For at most three
slots it centres the complete list; above six it uses `memcard_slotsused`
as the existing-slot boundary. Preserve the cached window/count and
per-callback reloads of selection, last column, slide state, colours,
scale, alpha, and the slot-info function pointer. The callback is required
when an existing slot is drawn. New-save text uses 0.85 X/Y scale;
navigation arrows use doubled scale and appear only with both slide
counters zero. Arrow colour and scale are captured before the left-arrow
callback, but alpha and the right-arrow availability test are reloaded.
Its second argument is a Y coordinate, not elapsed time.

Card-warning state 0 waits for save/load status exactly 1, then records
the warning/last flow and backs out. State 7 backs out on confirmation
with the select sound; state 3 backs out immediately. Preserve the
previous-state snapshot and callback ordering. The reconstructed
compiler removes the redundant transition-reset tail that retail still
contains, accounting for much of the remaining mismatch. Do not add
volatile state or change optimization settings merely to retain it.

Validation passes with the original 32-bit toolchain and 64-bit
ASan/UBSan:

- **19,424 slot-state cases** cover all colour byte values, selected and
  unselected controller/touch modes, pulse boundaries, signed alpha and
  free-space comparisons, all six slots, geometry, and callback changes.
- **38,536 carousel cases** cover empty/small/extended lists, signed
  slide counters, ten-frame progression, callback replacement, selection
  and count changes, colour interpolation, arrows, and nonfinite geometry.
- **25,088 integration cases** run the actual carousel and slot renderer
  together across all 64 occupancy patterns, controller/touch selection,
  small/extended lists, and available/insufficient storage.
- **249,156 card-warning cases** use the real `BackupMenu`, verify the
  whole menu array, and exercise state/status/input boundaries, four stack
  depths, and state-mutating enter/exit callbacks.

Carousel and warning tests retain normal global instrumentation. Only
the unrelated `GameMenuInfo`/`MenuInfo` callback tables are excluded from
ASan registration in the isolated slot-state and combined-renderer tests;
all tested globals, stack, and heap remain instrumented. Target/native
builds and all five repository tests pass. Rendering is mocked, and no
gameplay or visual run is claimed.

Deferred bounded experiment: `MenuUpdateAutoSaveCancel` has an audited
`MenuASCancelFinished` flag and initially true local `firstTimeIn` byte.
An existing finished flag clears before backing out. On save failure,
first entry disables prompts and sets a five-second delay; positive delay
waits, otherwise prompts are enabled before `NewMenu(1000, -1, -1)`, then
the finished/first-entry flags and delay reset before checking current
confirm/cancel input. NaN delay follows the retry path. Two equivalent
source forms compile to the same poorly aligned 290-byte/0% object body
versus retail's 258 bytes. The unvalidated candidate and its added state
were not retained; revisit with new control-flow/compiler evidence.

## Batch 28: texture-script loading ABI and occlusion statistics

Linked fuzzy matching improves from **64.481260% to 64.508064%**.
Four functions improve, none regress, and one new exact match is gained:

- `NuTexAnimProgReadCFG`: 3.203% to **81.502%**.
- `NuTexAnimProgReadScript`: 87.194% to **100%**.
- `InitTexAnimScripts`: 71.375% to **87.422%**.
- `OcclusionManager::RenderStats`: 4.330% to **99.784%**.

The configuration stub was missing its complete three-argument contract:
path, forward allocation cursor, and scratch-region end. The script
wrapper also incorrectly exposed only two of its four arguments. Recover
the end pointer and integer FPS argument in the shared header and sole
existing caller. `InitTexAnimScripts` passes the current `permbuffer_end`
and truncated `DEFAULTFPS` after path-building callbacks, retains its
four-byte per-script and sixteen-byte final alignment, and leaves a null
input list unaligned. A nonnull empty list still receives final alignment.
The original parser itself ignores its final two arguments; do not invent
FPS processing or bounds checks in `NuTexAnimProgParseFile`.

Recover the initialized `nutexanim_usepakfile = 1` global and the loader's
two paths:

- Archive mode replaces the final extension with `.pak`, narrows file
  size to its low 32 bits, and returns immediately if that size is zero.
  Otherwise load the archive at `(end - size)` rounded down to sixteen
  bytes. Preserve its original start separately from the advancing
  archive-load cursor. Parse the basename `.cfg` item and build a reverse
  list of script basenames immediately below the original archive start.
  Load each script item as a memory file, parse it, then close it. A
  successful archive path does not perform final forward-buffer alignment.
- With archive mode disabled, read the text configuration using full
  script paths and a reverse name list below the supplied scratch end.
  Read scripts in that resulting reverse order and align the forward
  cursor to sixteen bytes. A failed archive load aligns the forward
  cursor before falling through to this text path; a zero archive size
  does not fall back.

Basename extraction tries the last `/` first and only searches for `\\`
if there is no slash. Preserve ignored word-length/item-info returns and
the distinction between parser close and parser destroy. Reusing the
address-escaped archive cursor for list iteration is supported by the
retail stack slot and improves object matching from 79.626% to 80.808%;
the linked result is 81.502%. Remaining differences are stack allocation,
register selection and local control-flow alignment. No optimization
override, volatile state, calling-convention attribute, or helper ABI was
added to chase these differences.

These APIs retain retail's valid-input requirements: generated paths
must fit the 128-byte configuration and 72-byte script buffers (script
names at most 57 bytes with the fixed prefix/suffix), the scratch end
byte is writable, and the arena has room for the archive, name list, and
programs without overlap. Parser/open failures can leave the preexisting
list sentinel untouched; tests supply a valid empty sentinel on those
paths rather than inventing new failure behavior. FPS conversion requires
a representable integer result. Native pointer arithmetic uses full-width
`usize`, while the existing parser retains natural program alignment.

The occlusion statistics renderer uses existing typed state, suppresses
output while uninitialized, disabled, or taking a screen grab, and emits
the original two-pass diagnostic text. It uses PS2 coordinates, 0.7 scale
and point size, position `(112, 112)`, translucent black then cyan, and
calls length scale before height scale for the second-pass offset.
Counters display their signed 32-bit bit patterns; the visible count
subtracts in unsigned arithmetic before conversion. Reload the global
font after each callback, format counters after the first colour call,
and reuse the formatted text after the first print callback. Only the
coordinate and print-mode stacks are restored. The header now declares
the existing screen-grab flag; its ownership and definition are unchanged.

Validation with original-toolchain 32-bit objects and full-global 64-bit
ASan/UBSan passes:

- **16,384 configuration integration cases** exercise archive/text modes,
  failed size/load/open/parser operations, ignored item-info status,
  reverse order, mixed separators, empty words, cursor mutation, and both
  alignments. The harness uses the real script wrapper, parser, command
  table, program initialization, assembler, and linked program list;
  empty and one-instruction programs are checked along with entire arena
  contents and external call traces.
- **22,464 script-caller cases** exercise null/empty/multiple lists, all
  alignment residues, maximum valid path lengths, signed/fractional FPS,
  and callback changes to names, FPS, end pointer, and allocation cursor.
- **65,856 occlusion-rendering cases** exercise all gate combinations,
  signed counter boundaries and wrapping differences, nonfinite scale
  values, and state/font mutations at every external call boundary.

Target/native builds and all five repository tests pass. File/parser
services and rendering are mocked; no asset-loading gameplay or visual
run is claimed. All eleven PR checks were green on the preceding batch.

## Display-list diagnostics and pickup callbacks batch (29)

Baseline: `278d05b4`. Linked fuzzy matching rises from **64.508064% to
64.558060%**. Three functions improve and three regress; no exact matches
are gained or lost:

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `DisplayListPrintItem` | 1.184% | 78.704% |
| `Pup_CollectRedBrick` | 4.719% | 87.843% |
| `Pup_CollectCharKit` | 5.250% | 84.988% |

The display-list diagnostic printer's third argument is a filter count,
not a depth. Recover the two-argument C ABIs of `DisplayListDebugPS` and
`DisplayListPrintItemPS` in the shared header. Retail Android's debug hook
returns zero without touching the supplied buffer; its print hook is
intentionally empty. Both remain exact matches. Do not manufacture an
unknown-type diagnostic when the platform debug hook returns zero: in
that case the existing index text is appended again.

Recover all 28 padded labels from the retail switch table and immediate
stores; each label was checked byte-for-byte. The printer builds its
256-byte text/detail buffers even without an output handle. A zero filter
count selects all types, a negative count selects none, and positive
counts compare the current item type against the array. HTML output
requires selection, a positive debug level, and a nonzero handle. The
platform print hook still runs for every nonzero handle even if filtered
out or debug output is disabled. Preserve the green/blue prefix, low-32-bit
next-pointer text, line break, and conditional black-font reset. Reload
mutable item fields after external calls. The original contains no
console logger call for a zero handle. Separate colour-copy branches and
the boolean filter loop improve the first object candidate from 70.628%
to 78.507%; the linked result is 78.704%. Remaining differences include
stack/register selection and instruction scheduling, not missing labels.

Restore both pickup callbacks in `gizmopickup.cpp`, using the existing
typed world, save, object, and message layouts. Publish the existing
minikit callbacks and panel coordinates through their canonical headers.
Both pickups copy the entire `AddGameMsg_Default` before overwriting their
position/target, scale, flags `0x2112d`, duration, icon, special handle,
tick/end callbacks, and byte `0x4d`. Preserve the stack target vector's
lifetime through the synchronous queue call and native-width pointers.

- Red bricks emit debris `0x62` and the `MK-Pickup` sound before reloading
  `WORLD` and checking the save byte. Area `-1` bypasses the save check.
  Eligible pickups queue icon `0xd2` without a model-active gate, set the
  draw timer even if allocation fails, and reload the world after queuing
  before setting area red-brick state. Pad buzzing is always last. The
  callback does not write the persistent save byte.
- Character kits buzz first, emit debris `0x13`, then gate on the current
  `0xcf` model's active byte. A queued message receives target type six
  only when allocation succeeds. Draw time and the signed count update
  still occur on allocation failure. Counts below ten increment, including
  negative counts; ten and larger remain unchanged. There is no sound call
  in this callback.
- Move the existing static `EndRedBrickMessage` from the unrelated hint
  unit into its pickup owner, in retail definition order. The actual
  callback reference makes its old `__used__` retention attribute
  unnecessary. Its unchanged body remains **100%**: nonzero area state
  becomes two, scale becomes two, then sound, all-player rumble, and
  current-camera judder execute in order.

The two reconstructed pickup bodies have the retail sizes (415 and 408
bytes). Read the complete object diffs; remaining differences are
scheduling and register/stack operands. No optimization-map changes,
inline assembly, calling-convention attributes, or artificial retention
references are used. The aggregate report also records these collateral
changes, with complete before/after diffs reviewed:

- `GameMsg_EndDelay_Game`: 99.212% to 94.773%, register allocation and
  instruction scheduling after removing the misowned static callback.
- `NuDisplayListCaptureSortPriority`: 68.829% to 68.614%, two integer
  register operands after adding the printer body.
- `CollectMinikit`: 68.652% to 68.616%, a literal-address representation.

Retain the correct ownership and larger verified gains rather than adding
matching-only source artifacts to hide these differences.

Original-toolchain 32-bit and full-global 64-bit ASan/UBSan harnesses pass:

- **1,492,992 diagnostic-printer cases** per architecture cover every
  item type, ID classes, signed index/count boundaries, filter hits/misses,
  debug/handle gates, pointer bits, all platform fallback modes, and
  callback changes to type, ID, next pointer, filter, and debug state.
  Exact output and call traces are checked. Platform-supplied text must
  still fit the retail fixed buffers.
- **53,888 pickup cases** per architecture invoke the real pickup-table
  bindings and check the entire copied message, target coordinates, queue
  result writes, and final state. Coverage includes all 72 save slots,
  area `-1`, every save/model byte value, signed counter extremes,
  allocation failures, and callback changes to world, pad, camera,
  coordinates, defaults, and area counters. The relocated completion
  callback is checked separately with signed state boundaries and
  mutations at each external call. Unrelated callbacks retained by ASan's
  real table registration are aborting mocks, not excluded globals.

Target/native builds and all five repository checks pass. External
rendering, input, audio, and queue services are mocked; no gameplay or
visual validation is claimed. All eleven PR checks on the baseline commit
were verified green before publication of this batch.

### Additional low-match triage

`NuTouchInputStick::Render` and the related button renderer require a
shared-helper contract audit before reconstruction. Retail passes a
packed colour converted to float as `RndrUnfilledCircle`'s progress
argument, with integer colour argument 128. The initialized white/grey
values (`0x32ffffff` and `0x32646464`) make the helper's progress-times-360
integer conversion out of range. The current C++ helper therefore has
undefined conversion behavior for these actual caller values, while the
retail x86 instruction yields its integer-indefinite value. Do not silently
swap the arguments to make the image plausible, or restore the callers
without auditing this behavior on target and native platforms. No touch
renderer experiment was retained.

This prerequisite is resolved in batch 87 below: a defined conversion
preserves the retail integer-indefinite result, and both restored callers
are exercised together with the actual circle/arrow bodies.

`NuErrorSleep` likewise passes an uninitialized local `va_list` to its
font-print helper in retail. No speculative variadic reconstruction was
made. These findings are recorded to avoid repeated low-yield attempts.

## Customiser texture, selection, and configuration batch (30)

Baseline: `9452302d`. Restore four stubs and correct the shared texture
save/restore signedness contract without changing either owner's `-O2`
configuration. Linked fuzzy matching rises from **64.558060% to
64.571270%**; five functions improve and one regresses. Name initialization
gains an exact match, while the unchanged piece lookup loses its exact
code-generation alignment:

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `Customiser_RestoreModelTextureIDs` | 6.154% | 99.090% |
| `Customiser_Set100PercentPieces` | 12.000% | 78.250% |
| `Customiser_SaveModelTextureIDs` | 63.813% | 78.400% |
| `Customiser_InitNames` | 7.778% | 100% |
| `Customiser_PieceConfig` | 12.000% | 99.971% |
| `Customiser_FindPieceByName` | 100% | 96.594% |

Texture restoration visits both character IDs through `APICharacterLoaded`
and skips missing models. For each of nine categories, a zero saved texture
skips material inspection; a nonzero saved texture is narrowed to its low
16 bits for every matching material, followed by `NuMtlUpdate`. A material's
unsigned tag byte is compared with the category's **signed** tag. Negative
category tags therefore never match an unsigned material tag. Remove the
incorrect unsigned category cast from the existing save function too:
retail explicitly sign-extends the category byte in both functions. Saving
sign-extends each matching texture ID into its 32-bit cache, with the last
matching material winning. Restoration reloads hierarchy, material count,
material pointers, category data, and cached texture values after material
callbacks rather than freezing state across those calls.

Completion selection checks the customiser and save pointers, then scans
each category using a cached signed count and an advancing piece pointer.
The last piece with flag `0x80` selects the primary character, and the last
with `0x100` selects the secondary character. The flags are independent;
both can select the same piece. Empty or negative counts leave the save
untouched, and selected indices retain retail's low-word narrowing. This
does not itself test game completion or unlock pieces. The cached-count
pointer loop is evidenced by retail and improves the first 58.200% object
candidate to 78.250%; remaining differences are register/stack allocation
and addressing, not missing selection behavior.

Name initialization handles two slots in order. Character ID `-1` or
localized-text ID `-1` skips that slot. Otherwise copy the text into the
customiser's 128-byte name buffer and redirect the current `TTab` entry to
that buffer. Keep the text ID captured before the copy, but reload the
global text table afterwards and reload character data for the second
slot. Duplicate character/text IDs consequently allow the second slot to
copy the first buffer and become the final text-table owner. The customiser
must outlive those table references and valid names must fit the buffers.
No extra saved-name/default-name behavior is invented.

Recover the original mutable 84-byte local `Customiser_GameSetting` table
(six records plus sentinel), validating all string pointers and flags
against retail data at `0x621260`:

| Keyword | Model flags | Gameplay flags |
| --- | ---: | ---: |
| `bountyhunter` | `0x01000000` | `0` |
| `jedi` | `0x00000008` | `0` |
| `sith` | `0x0000000c` | `0x00000002` |
| `blaster` | `0x00100080` | `0x40000000` |
| `alreadygothat` | `0` | `0x00000010` |
| `stormtrooperhelmet` | `0` | `0x00040000` |

The configuration callback compares the parser's current word against
each setting case-insensitively, ORs both masks from the first matching
record, and stops. Unknown words leave the piece unchanged. It neither
advances the parser nor clears existing flags. Keep the parser word and
table flags reloadable across comparison callbacks. Names and parser
callbacks are declared through the existing customiser header.

The complete before/after diff for the neighboring
`Customiser_FindPieceByName` has seven changed instructions: two register
pairs and an equivalent conditional/fall-through branch arrangement. Its
body is unchanged and 161 focused lookup cases pass. Retain the coherent
source placement and larger verified improvements rather than introducing
artificial declarations, attributes, or padding to recover that alignment.
The near-exact configuration score differs only in a local table address;
restoration retains two equivalent effective-address expressions and
different local alignment. No layout-only score chase is retained.

Original-toolchain 32-bit and full-global 64-bit ASan/UBSan tests pass on
the actual source, totaling **315,171 cases per architecture**:

- 68,129 restoration cases cover all signed-category/unsigned-material
  byte pairs, both characters, missing models, negative/zero counts,
  all integer texture boundaries including `INT_MIN`, and callback
  mutations to hierarchy, counts, pointers, categories, IDs, and textures.
- 131,096 saving cases cover all tag pairs, all signed 16-bit model IDs,
  missing inputs, zero/negative counts, and signed texture storage.
- 65,567 completion cases cover every availability word, all nine
  categories, independent flags/last-match precedence, null inputs,
  negative counts, and index narrowing through 65,537.
- 161 lookup cases cover all 36 fixture pieces, misses, case folding,
  optional outputs, and a null customiser.
- 48,601 name cases cover sentinel/duplicate IDs, empty through maximum
  127-byte names, both table owners, and callback changes to the global
  tables and second slot.
- 1,617 parser cases cover all retail keywords/masks, case variants,
  unknown/whitespace words, preserved bits, and callback mutation.

Target/native builds and all five repository tests pass. External model
lookup, material updates, and string services are mocked; gameplay and
visual execution are not claimed. All eleven PR checks on the preceding
batch were verified green before publication.

### Deferred availability and file-selector work

`Customiser_PieceAvailable` has a recoverable integer return contract,
but two natural source forms scored 0% and 3.750% in isolated objects
because GCC places the demo branch after the ordinary path instead of
retail's demo-first fall-through. A diagnostic-only `-O3` build of the
second form produced the same score, so this is not evidence for a flag
change. Neither reconstruction nor its provisional header/type additions
is retained. The audited contract for a later attempt is:

- Demo mode returns whether availability bit `0x10` is clear.
- Otherwise flags `0x180` require `Game_100PercentComplete`.
- A non-sentinel signed character ID absent from `InCollectList_Index`
  returns one immediately; a listed but unowned ID returns zero.
- Model flags `0xc` require `Collection_GotAnyOfType(-1, mask)`.
- The signed byte at piece offset `0x11`, unless `-1`, requires
  `Collection_GotAnyOfType(type, 0)`. This byte needs a canonical field
  when the reconstruction is resumed.

`ProcessFileSel3` also has an incorrect void placeholder: retail returns
an integer selection result. Its real file-list helpers and state are in
`core/config/saveload.cpp`, while its stub is in the menu unit; those
owners currently use `-O2` and `-O3` respectively. Reconstruct it with a
coherent file-selector state/header audit, including the missing byte
flags, dimensions, filter strings, and callback, rather than layering new
local declarations into the wrong owner. No file-selector edit is retained.

## File-selector state and language ladder batch (31)

Baseline: `12e6709a`. Restore the file-selector control/state family beside
its real directory, filter, sort, cursor, and pad-repeat helpers in
`core/config/saveload.cpp`. The new shared `fileselect.h` replaces local
incorrect signatures in the menu unit. Live compile commands confirm the
destination uses `-O2 -fomit-frame-pointer` and the former stub owner uses
`-O3`; neither setting is changed. Also replace the device-language
function's hand-written comparison lambda and early returns with the
evidenced cached prefix-selection ladder.

Linked fuzzy matching rises from **64.571270% to 64.628180%**, with six
improvements, two small collateral regressions, **three exact matches
gained and none lost**:

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `ProcessFileSel3(float, nupad_s*)` | 1.603% | 98.374% |
| `StartFileSel` | 7.368% | 100% |
| `FileSelKill` | 26.250% | 100% |
| `ProcessFileSel(float, nupad_s*)` | 23.333% | 100% |
| `RenderFileSel2` | 16.154% | 99.962% |
| `NuIOS_GetDeviceLanguage` | 2.858% | 99.533% |
| `SaveSystemInitialiseEx` | 99.987% | 99.108% |
| `NuQFntCreate` | 56.323% | 56.058% |

`ProcessFileSel2` remains at 100%, now with the correct integer result
forwarded from `ProcessFileSel3`. Return types do not change these symbol
names, so the former void declarations concealed an ABI error.
`StartFileSel` takes five arguments and `RenderFileSel2` takes four, not
the no-argument placeholder signatures.

### File-selector contract

Recover the actual global storage from retail symbol sizes and data:
64-byte title and last-name buffers, 256-byte path/include/exclude buffers,
single-byte active/refresh/volume flags, floating dimensions, and a native
callback pointer. Initialized values are title `Title`, include filter
`*.nup | *.hgp   ` (three trailing spaces), exclude filter
`_PC. | _360. | _PS3.`, X 50, Y 40, and width 248. Existing file-list and
cursor state stays in its original reconstructed owner.

The processor returns zero immediately when inactive, without touching
the pad. Refresh rebuilds the filtered directory, configures the PS2 font
coordinate system, measures text at scale 1/16, clamps the width to a
minimum 240 and adds an eight-unit border. It consumes the repeat helper
once, restores the last-name cursor when nonempty, clears refresh, and
then calls the repeat helper again for the actual input. The double call
is intentional and changes held-input timing.

Movement bits are independent and ordered: down one, up one, down 14,
up 14, down the current file count, and up the current file count. Sort
increments the signed mode with defined 32-bit wrapping, maps exactly
four to zero, saves the selected name, and requests refresh mode two.
The parent button removes one trailing backslash, truncates after the
last remaining backslash, or appends one when none exists; it does not
silently replace a bare path with the root. Parent and confirm can both
execute in the same frame.

Confirm handles volume and directory records separately. A volume clears
the show-volumes byte and replaces the path. A directory named exactly
`..` repeats the parent operation; any other directory appends its name
with the evidenced backslash rules. These paths request refresh one and
clear the remembered name. Every other record type selects a file:
deactivate, copy its name, reload the callback and current cursor, call
the callback if present, and return one even if the callback reactivates
the selector. Do not cache state across string/font callbacks that retail
reloads.

Startup preserves title/path for null arguments, but null filters clear
the corresponding strings. It then sets active one, stores the callback,
and requests refresh two. Kill copies the selected name before clearing
active, without an active-state gate. The legacy processor wrapper runs
the processor and renderer before checking held pad bit `0x10`, then
kills if set; this is not the pressed-button field. The coordinate wrapper
stores X and half Y, calls `RenderFileSel3(0)`, then writes doubled height
before width, including when the output pointers alias.

The **full `RenderFileSel3` renderer remains a stub**. This batch restores
control/state and wrapper behavior, not the selector's visual UI. Valid
inputs still must fit the original buffers: title/selected names up to
63 characters, paths/filters up to 255, and enough space for appended
directory names and separators. No out-of-range float-to-integer behavior
is claimed for the dimension outputs.

### Device-language structural finding

Retail has 23 ordered, case-sensitive prefix tests, including specific
`en-us`, `fr-ca`, `es-mx`, and `pt-br` cases before their three-character
language prefixes. Validate every literal against original read-only data
at `0x562600` and every result against the full disassembly. Any cached
index other than -1 is returned unchanged. A recognized prefix populates
the cache; unknown strings leave -1 so later calls can retry.

An ordinary `memcmp` trial scored 3.642% in the isolated object, while
merely substituting `strncmp` into the old early-return structure scored
0%. Neither is retained. The natural `if (cache == -1)` / `if` / `else if`
assignment ladder followed by one cached return produces the original
1,197-byte structure: GCC itself expands the first 13 comparisons to
`repe cmpsb` and leaves the last ten as `strncmp` calls. No inline assembly,
special builtins, function attributes, or optimization changes are needed.
The retained object scores 98.801%; its linked score is 99.533%, with only
23 local literal-address differences remaining. This is a structural
source lesson, not a reason to hand-code compiler artifacts.

Complete diffs were reviewed for all retained functions. The main file
processor retains equivalent call-argument scheduling and tail-merging
differences. Both collateral regressions have exactly three changed
instructions relative to the baseline: a shifted local constant, a short
conditional jump becoming near, and removal of an alignment instruction.
Their source bodies are unchanged. Retain the verified improvements
without forcing padding or artificial source layout.

### Validation

Original-toolchain 32-bit and full-global 64-bit ASan/UBSan fixtures pass
on the actual source, **544,207 cases per architecture**:

- 87,458 processor/integration cases use the real directory, filtering,
  sorting, cursor, width, and repeat helpers. Cover all 512 relevant input
  combinations, 20 cursor positions, eight path shapes, every active and
  refresh byte, signed mode boundaries, unknown record types, refresh
  failures, callback state changes, remembered selections, NaN/negative
  elapsed times, and valid maximum-length paths/names.
- 8,192 startup cases cover all optional-argument combinations, lengths
  through 255 with the title limited to its own capacity, and callback
  presence/absence.
- 3,528 coordinate-wrapper cases cover full-width signed input
  coordinates, negative and boundary output dimensions within the valid
  conversion range, store order, and aliased output pointers.
- 445,029 language cases cover every byte substituted at all 64 buffer
  positions for each recognized prefix, truncated and non-terminated
  locale buffers, all first-two-byte combinations, arbitrary cached
  integer values, case sensitivity, unchanged input, cache persistence,
  and retry after an unknown locale.

Target/native builds and all five repository tests pass. External
filesystem, font, string, and render services are mocked in the selector
fixtures; gameplay and full UI execution are not claimed. All eleven PR
checks on the preceding customiser batch were green before publication.

## VAO lookup and options-menu behavior batch (32)

Baseline: `68494b63`. Linked fuzzy matching rises from **64.628180% to
64.650734%**. Two functions improve and two unchanged neighbors have small
register-allocation regressions; exact-match counts are unchanged:

| Function | Linked fuzzy before | Linked fuzzy after |
| --- | ---: | ---: |
| `NuIOS_GetOrCreateVAO` | 3.761% | 99.974% |
| `MenuUpdateOptions` | 0% | 65.126% |
| `MenuUpdateSave` | 70.367% | 69.959% |
| `MenuUpdateHints` | 11.641% | 11.500% |

### Renderer lookup layout

Recover the actual LOCAL `g_vaoRecords` array: retail symbol size `0xa000`
at `0x77eb20`, divided into 2,048 records of 20 bytes. Each record stores
two unsigned keys at offsets 0/4, a native `NuVertexFormatPS*` at 8,
the third unsigned key at 12, and the returned four-byte handle at 16.
Keep the three keys opaque: the retained helper has no direct retail call
site establishing more specific parameter meanings. The native record
may grow for pointer width; only the target layout is asserted.

The original unsigned comparison establishes the counter/index type.
Search in insertion order and return the first record matching all four
keys. On a miss, write the four keys into the next slot, increment the
count, and return that slot's **existing** handle. Despite its name,
the Android function neither generates a GL object nor writes/clears the
handle. Reset only clears the count, preserving all records and handles.
Do not invent GL calls or reset-time cleanup. As in retail, the caller
must keep the count within capacity and may not insert into a full table.

The ordinary `-O0` reconstruction reproduces all 371 bytes of instruction
structure. The linked diff has only three local counter-address changes;
the table references pair by their restored real symbol. No attributes,
flags, assembly, or forced linker retention were added.

### Options menu: incomplete behavior behind a zero score

The old implementation was not merely differently optimized. It omitted
the title-level music override, 1.5-second sound-preview state, and preview
positioning; it also changed callback order, cursor feedback, acceptance
fall-through, and controller handling. Recover both real file-local
variables (`opts_sfx_wait`, initialized to 1.5 at `0x617d30`, and
`opts_sfx_i`, BSS at `0x680e70`) in the existing `-O3` menu owner.

- Apply sound volume first. Outside the titles level, use the ordinary
  music-volume setter. On titles, set the music volume directly to the
  queried options volume when the super-option is enabled, or zero.
- Decrement the preview wait on every update, including cancellation.
  At or below zero, add 1.5 once, increment the index with defined 32-bit
  wrapping, map exactly one to zero, and enable this frame's preview.
  This is not a catch-up loop; unordered/NaN waits do not expire.
- Cancel calls `BackupMenu` before resolving/storing the back sound.
  Confirmation alone does not universally exit or play a selection sound.
- Control mode toggles its byte after the selection-sound callback and
  captures the resulting mode before `MechSystems::Get`. A callback change
  to that byte must not change the captured mode. There is no controller
  connectivity gate in the retail update function.
- Surround confirmation toggles the byte and resolves selection audio.
  Without confirmation, an expired preview and enabled surround setting
  play `PickupCoinB` around the current camera position at the configured
  sound-fade radius. The angle is the low 16 bits of the truncated
  `fmod(GlobalTimer.time_elapsed, 8) * 0.125 * 65536`, using the real
  sine/cosine table. Camera/radius are read after the remainder call.
- The volume row copies the old master volume into the selected column
  **before** changing the byte. Left takes precedence when nonzero;
  otherwise right increments only below ten. The other option rows toggle
  music and, outside demo mode, widescreen.
- Preserve the incrementing row cursor through the conditional ladder.
  After a callback, acceptance compares the reloaded selected row with
  that cursor, not an unconditional fixed 4/5. This matters when callbacks
  change the selection. Acceptance compares all 13 options bytes, resolves
  its sound, copies the then-current options, backs out, checks music, and
  stores the saved sound result last.

The canonical audio header now declares the existing volume query and
direct setter. Existing named option fields replace anonymous aliases in
the restored body; no shared layout changes are made. A first candidate
let C++ evaluate `MechSystems::Get` before reading the control-mode byte;
the full assembly review exposed that difference, and an explicit captured
mode corrected it before validation. Remaining code-generation differences
include ESI/EDI assignment, title-music block placement, integer-angle
normalization, and local alignment. Further speculative permutations were
not retained.

Complete before/after collateral diffs contain six register-only operand
changes in `MenuUpdateSave` and four in `MenuUpdateHints`. Neither body's
source changes in this batch. No exact function is lost.

### Validation and follow-up findings

Original-toolchain 32-bit and full-global 64-bit ASan/UBSan tests pass on
the actual source, **456,532 cases per architecture**:

- 27,516 VAO cases cover every count through 2,048, first/middle/last
  matches, every key independently, duplicate first-match precedence,
  null and full-width format pointers, arbitrary existing handles, reset
  preservation, boundary insertion, and 10,000 randomized operations.
  Out-of-capacity insertions are deliberately excluded.
- 429,016 options cases cover every option byte and input combination,
  all menu rows and signed row extremes, both title/demo states, exact
  callback traces, all 65,536 angle values, timer/NaN/infinity boundaries,
  signed counter wrapping, and nine families of callback mutations.
  The real `BackupMenu` and exit callback run in the fixture. Genuine
  constructed polymorphic Mech fixtures retain vptr sanitization.
  Invalid floating-to-integer preview-angle inputs are not claimed.

Target/native builds and all five repository tests pass. External audio
and Mech services are mocked; no gameplay or audible/visual result is
claimed. All eleven checks on `68494b63` were verified green.

The PhoneOS post/pump stubs need their real queue before safe work on their
bodies. Retail has five semaphores, two counters, 128 records of a 24-byte
message plus four-byte token, and a final token field: the LOCAL queue is
`0xe5c` bytes. Its constructor/destructor and queue synchronization must be
reconstructed together, including platform-sized semaphore storage and
the `0x0fffffff` token sentinel. No fake initialized storage or partial
message-queue implementation is added here.

`MenuUpdateHints` also remains behaviorally incomplete: its 423-byte
retail body synthesizes scrolling input, filters `Hints_LSW` by control
mode and available text, clamps per-mode scroll state, and seeks the
displayed position even after cancellation. Its shared two-element LOCAL
target/current arrays must be audited with `MenuDrawHints`, whose current
body is likewise incomplete, before reconstructing that pair.

## Batch 33: hint-menu state and rendering ownership

The paired audit above confirms a shared-state reconstruction, not two
independent wrappers. The retail update/draw bodies are adjacent at
`0x22ed80` (423 bytes) and `0x22ef30` (1,672 bytes), immediately after the
hint-system helpers. They reference LOCAL `updatehints_target_y` at
`0x6b1e38` and `updatehints_current_y` at `0x6b1e40`, each eight bytes,
and the 55-entry `Hints_LSW` table. Both functions move from the menu
catch-all into `legoapi/menus/core/hint.cpp`, alongside that table and
their recovered two-element integer/float arrays. Existing source options
remain unchanged: the old menu owner is `-O3`, the hint owner is `-O2`.
No optimization override, attribute, or build membership change is added.

| Function | Before | After |
|---|---:|---:|
| `MenuUpdateHints` | 11.500000% | 75.798080% |
| `MenuDrawHints` | 6.046205% | 84.412544% |

Linked fuzzy matching rises **64.650734% → 64.684235%**, with two
improvements, no regressions, and no exact-match transitions.

### Recovered behavior

- Active touch confirmation synthesizes up for selected item zero and
  down for every other item. Cancel invokes `BackupMenu` but still counts,
  clamps and interpolates the hints afterward; it does not resolve audio.
  Up has priority even when the target cannot decrement. Down uses the
  original 32-bit wrapping increment.
- Filtering rejects flags `0x2c`; mode zero additionally rejects `0x10`,
  while the alternate mode rejects a missing second text ID. Only non-null
  translated text counts. A target at or beyond the count becomes
  `count - 1`, including `-1` for an empty list. Existing negative targets
  are not independently clamped. `SeekValF` uses speed five and writes the
  mode captured before the callback, even if that callback changes modes.
- Rendering computes the original colour pulse from `HintRGB[2]`, captures
  text/icon X before the first remainder callback, and applies the menu
  entrance slide only to icon X. The up/down arrow text is followed by its
  six corresponding hit-region writes; the height uses one captured aspect
  ratio. It does not emit the placeholder header/back entry.
- Valid translated rows consume spacing and rotation phase even when above
  the upper clipping boundary. Rejected/null rows do not. Text fades between
  Y = 0.2 and 0.3; scrolling stops once the next Y is at most -1.5. Button
  pulse scale is assigned before expanding the translated text into the
  1,024-byte buffer, then reset after drawing. Translation is reloaded after
  the pulse helper, while the selected text ID stays captured.
- Each rendered icon uses the typed `WORLD->lev_objs[0xd4].special`, not a
  fixed byte offset in the native layout. The special pointer is captured
  before the rotation remainder callback. Later rows reload control mode
  and translation table after callbacks. All original angle narrowing is
  preserved without floating-to-small-integer overflow.

The menu table now consumes declarations from the hint header. Shared
arrow-text, colour-table and `BackupMenu` declarations are canonicalized in
their existing subsystem headers without changing any ABI.

The first update candidate used a common loop with mode checks inside it
and scored 43.144% in isolation. The retained mode-specific filter loops
follow the two distinct retail scan paths and score 75.365% in isolation.
The renderer scores 83.538% in isolation. Complete linked diffs were read:
remaining differences are filter-loop/block ordering, register allocation,
equivalent scroll arithmetic, and local data/constant addresses. There is
no further optimization-flag or instruction-shape search in this batch.

### Validation

The actual source passes **151,746 update cases** and **73,489 draw cases**
per architecture using the NDK x86 compiler and full-global 64-bit
ASan/UBSan. Coverage includes all 256 filter bytes, both valid control
modes, every input combination, signed target extremes/wrapping, empty
through 54-row tables, missing IDs/translations, every 16-bit pulse phase,
scroll clipping/fade boundaries including NaN/infinity, captured aspect
ratio, ten callback-mutation families, exact event/state snapshots, and a
1,023-character expanded string. The real hint table remains instrumented;
unrelated services retained through its callback pointers are mocked and
abort if called. Mode values outside the original two-slot contract and
invalid float-to-integer rotation inputs are not claimed as supported.

Target/native builds and all five repository checks pass. Rendering,
remainder/trig services, interpolation and menu backup are mocked in this
fixture; no gameplay or visual output is claimed. All eleven GitHub checks
on the preceding published commit `a078395c` were verified green.

## Batch 34: PhoneOS message queue and semaphore return ABI

Complete the queue prerequisite recorded in batch 32. The existing
`nuphoneos.cpp` stays at `-O3`, with register/post/pump in their original
relative order and LOCAL callback/queue storage. No build flags change.

| Function | Before | After |
|---|---:|---:|
| `NuPhoneOSMessagePost` | 2.763158% | 33.480263% |
| `NuPhoneOSMessagePump` | 3.652174% | 10.278261% |
| `NuThreadQueue<NuPhoneOSMessage, 128>::~NuThreadQueue` | 0% | 100% |

Linked fuzzy matching improves **64.684235% → 64.693110%**: three
improvements, no regressions, one exact match gained, and none lost.

The recovered queue has five real `NuThreadSemaphore` members, two
32-bit counters, 128 message/token records and a final waiting token.
Target assertions cover its `0xe5c` size and the counters/records/token
at offsets `0x50`, `0x54`, `0x58` and `0xe58`. Native semaphore storage
grows naturally; there is no fixed-size pthread placeholder. Construction
sets semaphore capacities to 128, 128, 1, 1, 1, clears the counters,
sets the token sentinel to `0x0fffffff`, and signals all 128 free slots.
Ordinary C++ member destruction reproduces all five reverse-order
destructor calls exactly. The queue cannot be copied.

Posting acquires a free slot, either with a nonblocking try or a wait,
copies the 24-byte message and sentinel token, updates empty/nonempty
notifications when necessary, advances the wrapping write counter, and
signals the occupied slot. A failed nonblocking acquisition returns
without waiting. The optional final wait observes the queue becoming
empty, **not completion of the last callback**. The header now explains
this distinction without changing the exported three-argument ABI.

Pumping first handles pending pause, resume and become-active flags in
that order. Each installed lifecycle callback gets null data, followed by
clearing its flag; later flags/callbacks are reloaded. It then consumes
available queued records, signals matching non-sentinel tokens, advances
the read counter, updates empty/nonempty notifications, and releases the
slot **before** dispatching the callback with the local payload copy.
Null callbacks consume records normally. The three previously absent
lifecycle flag objects are restored with their original four-byte widths.

The shared `NuThreadSemaphore::TryWait` declaration was also wrong:
every retail caller that uses its result tests AL, not EAX. Its canonical
return and local result are now `bool`. The linked semaphore constructor,
destructor, wait, try-wait and signal all remain exact matches. The queue's
cross-thread counters use relaxed atomic loads/stores to preserve the
retail x86 instructions without introducing C++ data races; the existing
semaphores provide record publication and slot-reuse synchronization.
The recovered contract is one producer and one consumer, not an invented
multi-producer lock-free queue.

Full post/pump assembly and object diffs were inspected. The post candidate
has a 28-byte frame instead of retail's 172-byte frame; retail retains
several intermediate aggregate copies absent from the simple recovered
message type. The pump's remaining mismatch is dominated by loop/return
block placement, with its record-copy and notification sequences present.
Do not add redundant copies, manual unrolling or ABI attributes just to
inflate these scores. The original constructor symbol includes its old
filename and unrelated VuVec constants; no filename alias or artificial
vector initialization is added to chase that compiler-generated artifact.

### Validation

- **54,180 trace/state cases per architecture** pass against the NDK x86
  build and full-global 64-bit ASan/UBSan build. They cover all seven
  event IDs, every occupancy 0 through 128, ring/counter wrapping including
  `UINT_MAX`, blocking/nonblocking and drain waits, lifecycle flag and
  callback combinations, matching/nonmatching/sentinel tokens, callback
  registration changes, callback-posted messages and nested pumping.
  Constructor capacities/free-slot signals and reverse destruction order
  are checked separately. Semaphore scheduling is deterministic in this
  trace fixture; records and callback payloads are compared byte for byte.
- A separate host integration fixture uses the actual pthread semaphore
  implementation. Full-queue dropping, a blocked 129th post, acknowledgement
  before callback return, counter wrapping, and **100,000 concurrent FIFO
  messages** pass under both ASan/UBSan and ThreadSanitizer.
- Target/native builds and all five repository checks pass. No gameplay,
  external platform lifecycle wiring, multiple producers, concurrent
  callback registration, invalid event IDs, or invalid message pointers
  are claimed as tested. Lifecycle flag mutation is tested on the consumer
  thread; the queue stress test does not invent asynchronous flag writers.

All eleven GitHub checks on the preceding commit `8d4809f4` were verified
green before this batch was committed.

## Batch 35: autosave callbacks and hub episode initialization

Linked fuzzy matching improves from **64.693110% to 64.701454%**:

| Function | Before | After |
| --- | ---: | ---: |
| `MenuEnterAutoSaveCancel` | 32.308% | **100%** |
| `MenuDrawAutoSaveCancel` | 18.261% | **100%** |
| `MenuUpdateAutoSaveWarning` | 17.500% | **99.375%** |
| `MenuUpdateDoNotRemoveCard` | 22.105% | **99.947%** |
| `MenuInitEpisodes` | 6.761% | **85.254%** |

### Autosave state and callback order

The original four-byte `MenuASCancelFinished` object was absent. Restore it
with the two callbacks that consume it. Entry clears both autosave flags
and the menu's **last row**, not its selection, only when the finished flag
is zero. Drawing similarly gates on the flag, calls `Draw_AUTOSAVECANCEL`,
then reloads `memcard_savefailed` before deciding whether to call `Draw_OK`.
The text helper is genuinely empty in the reference binary; it remains
empty, and its declaration now belongs to the renderer's shared header.

The autosave-warning updater first backs out on any nonzero card-change
flag. It then reads the **original menu pointer's current confirm input**,
even after exit/enter callbacks, and may set the selection sound and back
out a second time without entering the new parent. A combined `else` or an
early return after the first backup would be wrong. The remove-card updater
backs out only after the strict `menu_time > 2.0f` threshold and status 1;
NaN time does not pass that gate.

All four first candidates have the original byte lengths. The two exact
functions require no source-shape tuning. The warning's residual mismatch
is one load/test register pair; the remove-card residual is a literal
address. The previously documented `MenuUpdateAutoSaveCancel` trial is not
repeated: its body remains a stub, so this is not a claim that the complete
autosave-cancellation workflow works yet.

### Episode initializer ownership and widths

`MenuInitEpisodes` at `0x1b3f30` immediately follows the private
`DrawEpisodesMenu` and precedes episode update/draw and the already
hub-owned select-mode callbacks. Move the initializer out of the generic
menu source into `hub.cpp`, preserving both files' existing `-O3` settings.
Its menu-table caller now uses the canonical header declaration.

Restore the original global objects `lastepisodesmode`, `episodesmode`, and
`i_episodes` as **one-byte** state, plus `episodestime` and
`episodesduration` as floats. Initialization sets last mode to -1, time to
zero, mode to zero and duration to 0.6. If save data exists, the selected
episode's first area's **complete byte at offset 0**, not `area_complete`
at offset 1, determines whether the selection falls back to zero. The
episode index and first area ID are sign-extended from 8 and 16 bits.

A non-sentinel level with a non-sentinel episode selects mode 2 and copies
that episode unless a hub-start door is present. A door independently
selects mode 2 even without such a level. The passed menu is untouched.
Shared assertions verify the 28-byte episode stride, first area at offset
4, and level episode at offset `0xae`. No additional index bounds are
invented; callers must supply valid backing tables for the indices used.

The 242-byte candidate versus retail's 264 bytes retains the full behavior.
GCC caches the level episode byte and merges a return path that retail
keeps separate. No volatile loads, branch hints, ABI attributes, padding,
or compiler options are added to imitate those differences.

### Collateral changes and verification

Six functions improve and three regress, with **two exact matches gained
and none lost**. The unchanged `MenuUpdateSelectControls` improves
58.565% to 59.326%. The unchanged hub neighbors change as follows:

- `MenuInitSelectMode`: 99.828% to 99.655%, one register pair.
- `MenuDrawSelectMode`: 37.162% to 33.518%, register allocation and
  instruction scheduling around existing draw-call argument setup.
- `MenuDrawBonusMode`: 88.422% to 88.348%, one register pair plus relocated
  local data/literals. All six differing literal references across these
  renderers retain identical four-byte values before and after.

Full before/after linked diffs for all three regressions were inspected;
their source bodies are unchanged. They were not behavior-tested by the
new focused fixtures, and their remaining reconstruction work is not
claimed complete.

**30,249 autosave cases per architecture** pass NDK x86 and full-global
64-bit ASan/UBSan builds: signed/nonboolean flags, all four stack depths,
both/null exit and enter callbacks, original-menu input mutations, sound
and cursor changes, second backups, exact/adjacent floating thresholds,
NaN/infinite timers, and draw callbacks mutating save/finished state. The
actual `BackupMenu`, `BackupMenuNoFn`, and `MenuRememberCursor` run against
an independent stack/state oracle; render services are mocked.

**143,360 episode initializer cases per architecture** pass NDK x86 and
full-global 64-bit ASan/UBSan builds: every signed episode byte, every save
completion byte, signed area-ID endpoints, null save state, every level
episode byte, missing/present start doors, and level sentinel/valid slots.
Biased, adequately sized fixture tables make the signed index cases valid
C++ accesses; no invalid-pointer or out-of-bounds contract is implied.
An unrelated arcade table receives inert references in the host fixture
so normal ASan instrumentation remains enabled for all globals.

Target/native builds and all five repository checks pass. There is no
gameplay or visual run, and the other episode-menu stubs remain open.
All eleven PR checks on preceding commit `7f44010b` are green.

## Batch 36: stripped renderer diagnostics

Linked fuzzy matching improves from **64.701454% to 64.719740%**.
Five functions improve, one regresses, **four exact matches are gained**,
and none are lost:

| Function | Before | After |
| --- | ---: | ---: |
| `DumpAttributeBindings` | 6.452% | **100%** |
| `MultilineDump` | 12.500% | **100%** |
| `DumpShaderSource` | 4.762% | **96.119%** |
| `DumpProgramSource` | 13.333% | **100%** |
| `DumpShaderAttributes` | 7.692% | **100%** |

These five consecutive functions at `0x293ce9–0x2940a5` belong to the
existing `nurndr_android.c` owner, which remains `-O0`. Although their names
sound like loggers, retail contains **no output calls** here. It retains
the GL queries, allocation, shader lookup, and temporary string processing
after diagnostic logging was stripped. Do not add invented console output
or force the compiler to preserve logging that is absent in the reference.

`DumpAttributeBindings` queries all 16 fixed attribute slots. For enabled
slots it requests buffer binding, size, stride, type, normalization and
pointer in that order. `DumpShaderAttributes` queries the active count,
uses its original static 256-byte `attributeName` buffer, obtains each
location **before** trimming the first `[` suffix, and refreshes the loop
bound after external calls. Its outputs are otherwise unused in retail.

`DumpShaderSource` queries the source length, allocates that exact byte
count from current thread memory with alignment 4, zero-fill flag 1,
empty allocation name and category 0, then fetches the source. It examines
the two signed material shader IDs in order, reloading the current material
between lookups, and stops at the first non-null shader object whose vertex
or fragment handle matches. The recovered key local belonged to the
stripped diagnostic heading. The source then passes through `MultilineDump`
and is freed through a fresh thread-memory lookup.

The material descriptor's adjacent `shader_id` and `shader_variant_id`
now also have a real two-element `shader_ids` array alias. This avoids
out-of-bounds pointer arithmetic from one scalar member to the next while
retaining existing field access and target offsets. Assertions verify the
pair begins at descriptor offset `0x140` and occupies four bytes.

`DumpProgramSource` asks GL for at most two attached shaders and invokes
the actual recovered source routine in order. `MultilineDump` retains the
original const-qualified ABI, but its callers must supply **writable,
nonempty** strings: it searches starting after the first character and
temporarily replaces each later newline with NUL, then restores it. Its
retained line-start local is part of the stripped line-logging code. An
empty one-byte allocation, failed allocation, invalid GL return values,
null current material, or read-only text is not made safe by this batch.
No permissive behavior is invented for those cases.

The first source candidates already recover the original instruction
structure for all four exact functions. `DumpShaderSource` is 305 bytes
versus retail's 309: its key load is direct rather than LEA plus load, and
its loop comparison reads the local directly rather than first loading a
register. No artificial wrapper type, casts, attributes or compiler flags
are introduced to force those two expressions.

### Shared-layout regression and validation

The unchanged `NuMtlRegisterForOverride` drops **97.940% to 93.507%** after
the shader-ID array alias is introduced. Its before/after linked diff is
fully inspected: scratch registers and two independent store positions
change, and GCC reloads the same allocated material pointer around the
shader-ID stores. Its body grows from 241 to 245 bytes. Allocation, lookup,
copy, field values and update calls are unchanged. This function is not
behavior-tested by the new diagnostic fixture; the shared pair itself is.

**192,576 cases per architecture** pass against the actual NDK x86 source
and full-global 64-bit ASan/UBSan build:

- **65,536 binding masks**, checking exact query order and nonboolean
  enabled results for every slot.
- **30,720 attribute cases**, covering lengths 0 through 255, leading,
  trailing, repeated and absent array suffixes, empty/count boundaries,
  callback-mutated counts, full-width program handles, and all 256 bytes
  of persistent name storage after lookup and trimming.
- **24,640 source/program cases**, covering both real routines together,
  zero/one/two attachments, signed shader-ID endpoints, all first/second
  match and null-object combinations, sources through 2,047 characters,
  newline patterns, and callbacks changing material or thread-memory state.
  Exact traces are compared with an independent source-call oracle; GL and
  allocator services are mocked, with exact-sized instrumented allocations.
- **6,144 multiline cases**, verifying complete buffer/canary preservation
  across lengths 1 through 1,024 and byte/newline patterns.
- **65,536 shader-pair cases**, verifying scalar/array alias reads and
  writes for every 16-bit pattern without modifying neighboring fields.

Target/native builds and all five repository checks pass. No live GL
context, logging output, allocation failure, malformed driver output, or
gameplay execution is claimed as tested.
All eleven PR checks on preceding commit `078d3b48` are green.

## Batch 37: movie wrapper and skip callback ABI

Linked fuzzy matching improves from **64.719740% to 64.731090%**, with
three improvements, no regressions and no exact-match transitions:

| Function | Before | After |
| --- | ---: | ---: |
| `Movie_Play` | 4.696% | **99.948%** |
| `Movie_CallBack` | 19.091% | **99.682%** |
| `Movies_ConfigureList` (unchanged body) | 90.759% | **90.823%** |

The movie wrapper's six-argument signature was present but its **integer
return was incorrectly declared void**. The private callback was likewise
a void, forced-emitted stub. Recover both integer returns and the four real
local state objects: `movie_skipped`, `MoviePlayTime`, `MovieFrameTime`, and
`MovieInputFn`. The callback now emits naturally because the wrapper passes
its address to `NuFmvPlayV`; the matching-only `__used__` placeholder is gone.
Their source order follows the callback/configuration/play run in retail,
with the owner's existing `-O3` setting unchanged.

`Movie_Play` clears the skipped flag, constructs regional `movies\\pal\\`
or `movies\\ntsc\\` paths in two 256-byte arrays, appends the name, and
adds `.sub` and `.pss`. It kills audio only when `NOSOUND` is zero, then
sets the frame interval, clears playback time and installs the supplied
input callback or `GamePads_SkipMovie`. These actions still happen when
either allocation-cursor pointer is null; that later gate returns zero.

The successful path forwards the exact recovered tagged option sequence
to `NuFmvPlayV`, including the local float volume pointer, real callback
pointer, start-cursor pointer and dereferenced end cursor. Known pointer
arguments retain native width. No additional meaning is invented for the
remaining numeric tags. The default-input and kill-audio declarations now
come from their canonical module headers.

The return convention is **0 for a failed call or absent cursor pointer,
1 for a nonzero skipped flag, and 2 otherwise**. In particular, the
original CMP/SBB/NOT/ADD sequence returns 2, not 1, when the flag is zero.
Existing trailer callers ignore the return but now see the correct shared
prototype. The callback invokes installed input first, reloads the timer,
accepts nonzero input only at `MoviePlayTime >= 0.2f`, sets the skipped
flag on acceptance, then advances time using the current frame interval.
NaN time does not pass the threshold. Null input still advances time and
returns zero. Previously accepted skip state is not cleared by a later
unaccepted frame.

Two evidence-led source forms were compared. Selecting the region string
inside one call produced CMOV rather than retail's two call sites; the
retained ordinary `if`/`else` restores the original branch. Keeping the
callback's input result and then normalizing it through the threshold
recovers its original shared return path. Final sizes are exactly the
retail **481 bytes** and **96 bytes**. Full linked diffs contain only six
local-state address differences each, plus one literal address in the
callback; do not permute their data to chase those residual fractions.

### Validation and platform limit

**152,113 cases per architecture** pass against NDK x86 and full-global
64-bit ASan/UBSan with a recording playback mock:

- 93,312 wrapper cases cover PAL/audio/input flags, missing and aliased
  cursor pointers, nonboolean backend returns, skip flags, frame runs,
  current-state reloads and callbacks changing input, time, audio or cursor
  state. Reentrant input invokes the real wrapper with absent cursors.
- 481 path cases cover every valid name length through 240 for PAL and 239
  for NTSC, including maximum 255-character resulting paths and nonfinite
  volume values. Oversized names remain outside the recovered contract.
- 58,320 callback cases cover null/custom/default input, signed input
  results, negative and nonfinite time/steps, exact and adjacent 0.2-second
  values, existing skipped flags and callback-mutated/reentrant state.

The same 152,113-case fixture also passes on both architectures with the
**actual `nufmv_android.cpp` backend** in place of the recording mock.
That backend returns 1 immediately and never invokes the callback, just
as the reference Android `NuFmvPlayV` does. Those runs verify wrapper
integration with this platform behavior, **not video playback**. Input,
audio and string services are otherwise mocked; the recording variant
exercises the real movie callback and validates every variadic argument.
Target/native builds and all five repository checks pass. No gameplay or
actual multimedia execution is claimed.

## Batch 38: private command-line parser and bootstrap ownership

`ParseCommandLine()` improves from **5.479% to 99.726%** at its original
291-byte size. Its restored call immediately after `NuAPIInit` also raises
`NuInitHardware` from **19.53% to 19.69%**. The parser, its private
argument cursors, and the real bootstrap caller now share the default-`-O0`
`nu2api/nucore/nuapi.cpp` owner. The misplaced one-stub
`legoapi/core/startup/startup.cpp` is removed; it remains recoverable in Git.
Neither source had an optimization override, and none was added.

Original LOCAL `argc` and `argv` are four-byte BSS objects at `0x74ded4`
and `0x74ded8`. The parser repeatedly checks signed positive `argc` and a
non-null current argument, recognizes `PADRECORD` and `PADPLAY` through
`NuStrICmp`, consumes their following argument into the recording path,
sets the corresponding mode, and then advances the common argument cursor.
The two destination offsets, `nuapi + 0x48` and `nuapi + 0x5c`, now have
target-only layout assertions. Pointers and their arithmetic retain native
width. The final linked diff has only twenty private-state/string address
differences; no instruction-shape work remains in this body.

The reference Android `NuCommandLine` is empty, and no writer to these
private cursors was found in the original text. Consequently the normal
Android path still starts with zero arguments. Do not invent host process
argument plumbing or a platform provider to make the recovered parser run.
The bootstrap call itself is evidenced at `0x2692da`, directly after the
`NuAPIInit` call.

### Compiler-generated score caveat

The linked report changes **64.731090% to 64.729100%**, with two improved
bodies, one regressed ambiguous initializer row, and no exact losses.
Removing the obsolete startup unit removes its incidental header-generated
vector initializer. The report then scores original
`__static_initialization_and_destruction_0` at `0x316eda` as **0% rather
than 99.70%**. This is an ambiguous duplicate-symbol pairing effect, not
removal of scratch initialization: the real `nuscratch_android.c`
initializer remains 373 bytes, with the same six vector constructions and
instruction structure before and after, and its uniquely named
`_GLOBAL__sub_I_nuscratch_android.c` wrapper remains **100%**.

No dummy initializer, include-only translation unit, score filter, or
denominator change is used to hide this artifact. Together with batch 37,
the retained unit raises the published whole score **64.719740% to
64.729100%**: five improved functions, this one artifact regression, and
no exact-match transitions. This is consistent with prioritizing recovered
behavior and coherent source ownership over compiler-artifact layout.

### Validation and bounded scope

**353,280 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan: 327,680 argument sequences, 20,480 comparator-mutation traces,
and 5,120 calls through the actual `NuAPIInit`/`NuInitHardware` path.
Tests cover case-insensitive keywords, unknown and prefix tokens, null and
absent following paths, repeated commands, signed count boundaries, and
comparison callbacks changing the cursors or recording state. Complete
state and ordered comparison traces are checked against a separate oracle.
Bootstrap services are mocked and their seventeen-step ordering is checked.

The pre-existing hardware setup dispatcher also routes `NUAPI_SETUP_END`
through its platform fallback and consumes another variadic argument.
The integration fixture supplies that explicit null pointer; this batch
does **not** claim to repair or fully validate the unrelated setup-token
dispatcher. Target/native builds and all five repository checks pass.
No real device bootstrap or pad-recording file I/O was exercised.

## Batch 39: shader-key entry points and mutable digest state

Linked fuzzy matching improves **64.729100% to 64.736800%**, with four
improved functions, **two new exact matches**, and no regressions:

| Function | Before | After |
| --- | ---: | ---: |
| `NuShaderObjectKeyGenerate2` | 16.154% | **100%** |
| `NuShaderObjectKeyGenerate4` | 5.833% | **39.236%** |
| `NuShaderObjectKeySetUberShaderHash` | 7.119% | **100%** |
| `NuShaderObjectKeyGenerate3` | 42.395% | **44.083%** |

The three no-argument placeholders in `pending_stubs.cpp` hid real
parameterized APIs. They now follow `Generate3` in its existing `-O3`
shader-manager owner, matching the consecutive original `0x309620` through
`0x309d09` run. The former catch-all stays `-O2`; no optimization override
is changed. `Generate2` constructs a native-width `ShaderMtlDescFilter`,
forwards descriptor/material pointers and the two scalar arguments into
`internalInit`, then forwards that filter and the pixel-stage argument to
`Generate3`. Its 104-byte linked body matches completely.

The more important state defect was the private **constant** digest in
`Generate3`. Retail instead reads GLOBAL, writable `uberShader2_md5`, a
16-byte object at `0x636e20`, immediately before `uberShader2` at
`0x636e40`. Restore its original bytes and place it alongside the embedded
text in the text's existing `batman.cpp` owner; this does not establish
that provisional asset owner's complete original TU boundary. The shared
shader header declares the object and recovered APIs. Both key builders
now read the same mutable digest, so calling the setter has real effect.

The setter copies sixteen bytes in forward order, accepts exact self-alias,
and uses its original LOCAL, zero-initialized `defaultHash16` for a null
input. **Null resets to zero, not to the initial embedded digest.** Its
ordinary loop naturally emits retail's vectorized non-overlap path and
scalar overlap path. Defining the digest in the manager itself exposed its
alignment to GCC and changed one store instruction; grouping the real
digest with its adjacent shader asset restores the original external-object
access and the complete 186-byte match, without alignment attributes.

`Generate4` restores an output pointer and the two observed 32-bit argument
words, described as flags and selector. It zeros the 104-byte serialized
input, writes the fixed byte at offset 20, copies eight digest bytes at
offset 12, serializes all four flag bytes at offset 4 and the selector's low
nibble at offset 8, then repeats the low three flag bytes at offset 9.
The inverse CRC is stored into the output's high half before the forward
CRC is called and ORed into the low half. Keep that intermediate store and
subsequent output reload. These byte offsets describe a serialized hash
protocol, not native pointer-bearing structure accesses.

The retained `Generate4` is 301 rather than 249 bytes. Full linked review
finds a non-realigned frame, different byte extraction/register scheduling,
and a mask instruction in place of zero extension. No synthetic alignment,
calling-convention attribute, compiler hint or unrolling was added to chase
the remaining score. The existing larger `Generate3` remains incomplete;
only its digest access is repaired in this batch.

### Validation and limits

**307,200 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan, both with a recording CRC mock and with the actual `CRC16.cpp`
implementation checked against an independent bitwise CRC oracle:

- 12,288 setter cases cover all byte values, input alignments, exact-sized
  allocations, input preservation, exact self-alias and null resets.
- 262,144 `Generate4` cases verify every serialized byte, signed selector
  boundaries, flag-byte patterns, setter-to-key integration, call ordering,
  digest changes during hashing and CRC callbacks changing the output.
- 32,768 direct/`Generate2` integration cases verify descriptor/material
  pointer forwarding, scalar boundaries, filter lifetime, vertex-versus-
  pixel digest selection and setter resets through the real `Generate3`.
  Filter methods are controlled mocks and other descriptor fields are
  bounded fixtures, **not** a complete material-key reconstruction audit.

Actual CRC runs validate complete key values; callback mutation assertions
belong to the recording variant. Four unused GL table targets are guarded
by aborting mocks so full ASan global instrumentation can remain enabled.
The linked digest's sixteen initial bytes match retail exactly. Target and
native builds and all five repository checks pass. No live shader
compilation, GL context or gameplay execution was tested.

## Batch 40: touch-system update lifecycle and embedded native storage

`MechSystems::Process` improves **5.295% to 44.910%**, raising linked fuzzy
matching **64.736800% to 64.744120%** without any regressed functions or
exact-match transitions. The constructor remains **100%** matched.

The old body only called `Init`. Retail additionally updates 32 move
markers, four swipe markers, and removes three expired tag controls, then
processes the current world's auto-jump manager when present. Restore
that sequence with ordinary loops in the existing `-O2` owner. Slots are
reloaded after callbacks: a callback can replace its own slot, and tag
removal can clear or replace the pointer before the subsequent delete.
The world lookup happens even if initialization has not enabled the UI.

Move markers are deleted only with flag bit 3 set and scale at most
`0.001f`; swipe alpha must be strictly less than `0.01f`; tag controls
require their fading flag and first-fade value at most zero. NaNs do not
pass these comparisons. Frame time is read for each update call, not
cached across callbacks. Virtual deletion retains real managed-reference
invalidation for marker and tag objects. Target-only assertions document
the three cleanup-value offsets and the initialization byte.

Before restoring these calls, fix a real native-layout defect: the three
placement-constructed UI objects and click tracker previously occupied
fixed Android-size buffers. Their polymorphic native forms are larger.
Move the `MechSystems` declaration after the complete UI types and size
each buffer with `sizeof` of its actual type. Its alignment attribute is
required for placement construction of those objects, not instruction
matching; the NDK GCC 4.7 compiler does not implement `alignas` (verified
in a bounded compile probe). All original Android offsets and the
`0x293c` class size remain asserted and unchanged.

The constructor's five pointer clears also previously used 32-bit words
from `unknown_0x10`, leaving native pointers partly initialized. Replace
them with the actual pointer members and remove the unused word overlay.
Do not initialize the sixth, menu-controller pointer: that would add
behavior absent from the original. Explicit construction/destruction
order for the embedded UI controls is preserved.

The 413-byte update body is shorter than retail's 873 bytes because the
four swipe slots and three tag slots remain loops instead of duplicated
blocks. Full object-diff review confirms this, the related stack/register
differences and relocation operands; no compiler-forcing attributes,
hints, padding or manual expansion were introduced.

**24,576 lifecycle cases per architecture**, plus an empty-bootstrap
smoke case, pass on NDK x86 and full-global 64-bit ASan/UBSan. They link
the actual `MechSystems.cpp` constructor, initializer, update and destructor
with independently constructed typed objects. Tests check embedded buffer
alignment, sentinel preservation, complete pointer initialization, all
slots across mixed presence masks, byte flags, exact/adjacent thresholds,
signed zero, infinities and NaNs. Ordered traces cover callback replacement,
future-slot removal, changed frame time/initialization/world state, tag
removal clearing or replacing a slot, virtual destruction, and real
`NuMechPtr` invalidation.

Child UI constructors and external update/UI/world services are mocks;
this does not claim complete UI behavior, the wider destructor contract,
or gameplay execution. Target/native builds and all five repository checks
pass. All eleven PR checks on preceding commit `859bed90` are green.

## Batch 41: camera-cut completion action and miscellaneous pickups

Restore two small stubs in their existing `-O3` owners. Linked fuzzy matching
rises **64.744120% to 64.753750%**: `Action_EndCameraCut` improves **6.269%
to 99.179%**, and `AddMiscPickups` improves **7.925% to 91.321%**. No other
function score changes or exact-match transitions occur. Combined with
batch 40, three functions improve without regressions since `859bed90`.

### Camera completion and registration

The original 238-byte camera handler returns `i32`, not `void`. It always
returns one, and only scans parameters when both `first_time` and
`MiniCutCam` are nonzero. Each parameter first searches case-insensitively
for `end_time=` anywhere in the string, then for `blend_out_time=` only
if the first key was absent. The end key stores the parsed duration plus
the camera time reloaded **after** `AIParamToFloat`; the blend key stores
the parsed value directly. Negative counts do not enter the loop. Do not
retest the initial camera gate after callbacks or replace substring
matching with prefix matching.

Repair the missing `EndCameraCut` entry in `lego_aiactiondefs` and declare
the real signature in the shared gizmo-action header. Original and rebuilt
ELF inspection verifies record 125, the handler pointer, and `(1, 0, 0)`
flags. Records are twelve bytes on Android, not twenty; the original entry
is at `0x61f5dc` and points to `0x18fff0`.

The rebuilt body is also 238 bytes. Its remaining eight differing
instructions are six scratch-register choices in the initial gates and
two private literal operands. No ABI, optimization, or register-forcing
change was made to chase these differences.

### Pickup forwarding

`AddMiscPickups(position, player_id, coins, torpedoes)` calls `ReleaseHearts`
only when the torpedo count is exactly zero. It then calls `AddPickups`
with the input coin count, either the returned heart count or input
torpedo count, no powerups, the supplied position, shared `v010` direction,
speed five, player ID, unit scale, duration `2000000.0f`, null owner, and
final arguments `(1, 0, true)`. Restore the canonical `ReleaseHearts`
declaration through its header and name the wrapper's parameters by their
actual meanings. Signed negative values are forwarded, not clamped.

The 269-byte result differs from the 280-byte original in branch placement:
GCC lays out the no-heart path first and moves `ReleaseHearts` to the other
branch. Full linked review finds equivalent forwarding on both paths.
No prediction hint or attribute was added to force the original ordering.

### Validation and limits

**327,680 camera-action cases per architecture** pass on NDK x86 and
full-global 64-bit ASan/UBSan. Tests cover all three-token sequences from
eight keyword/miss/precedence patterns, negative/zero/positive counts,
both initial gates, signed-zero/infinite/NaN floats, exact search/parse
traces, callback changes to camera state, and future parameter replacement.
The production translation unit's unrelated locator registrations remain
active; their unused services have aborting mocks. The action-table binding
is inspected separately in the complete linked image, not exercised through
the full script VM.

**49,152 pickup-forwarding cases per architecture** cover signed integer
boundaries, null/ordinary/direction-aliased positions, every forwarded
argument, callback order, and callback changes to vector storage. Tests
link the actual separately compiled production object; only its downstream
`AddPickups` definition is weakened in a test copy so a strong recording
mock can replace it. Wrapper instructions and repository linkage are not
changed. Native tests explicitly enable semantic interposition: without
it, Clang legitimately omits arguments unused by the real same-unit callee,
invalidating that mock seam. These are wrapper-contract tests, not tests of
the downstream pickup spawning implementation.

Both suites retain full global ASan instrumentation. Batch 40's 24,576
lifecycle cases are also rebuilt and rerun on both architectures after
the final header cleanup. Target/native builds, all five repository checks,
and `git diff --check` pass. No gameplay execution was performed.

## Batch 42: integer conversion and texture-format diagnostics

Restore two value-returning stubs. Linked matching rises **64.753750%
to 64.760590%**, with four improved functions, no regressions and no
exact-match transitions. `charToInt` improves **8.936% to 77.447%**;
`GetNativeTextureFormatName` improves **9.890% to 99.681%**. The unchanged
font reader and texture-file loader gain small address-only improvements;
before/after review finds two literal/table operands in each, not changed
behavior.

`charToInt` returns `i32`, recognizes only an initial minus sign, and sums
signed character-minus-`'0'` terms from right to left with decimal place
weights. It does not skip whitespace, accept a plus sign specially, validate
digits, or stop at a non-digit. Empty strings and a lone minus yield zero.
Keep arithmetic modulo 32 bits using `u32`, including the initial index
decrement, rather than introduce signed-overflow undefined behavior. Add its
shared declaration to the existing utility header. Its current catch-all
owner stays `-O3`; adjacency to `getNumDigits` is insufficient evidence to
move it to that helper's `-O2` owner.

The first isolated parser draft used a signed `length - 1` and matched
95.170% before linking, but that subtraction can overflow after `strlen`
is narrowed. The retained explicit wrapping index produces 123 rather than
136 bytes and prevents GCC from making the same induction transformation.
One direct reverse-pointer-loop probe fell to 63.617% and changed the extreme
length gate; discard it. Keep the safe arithmetic without hints, artificial
attributes or repeated compiler permutations. Null/nonterminated inputs are
not valid; multi-gigabyte string behavior was not exercised.

The texture-name helper returns a persistent string pointer, not `void`.
Decode the original 120-entry jump table and all 26 non-default labels;
unrecognized values return `"Not defined"`, including holes within the
enum range and signed out-of-range inputs. Restore missing luminance and
render-target enum names with their verified values and preserve all
existing values. The default label is intentionally not replaced by a
modernized format name.

Move this helper from the gameplay catch-all beside `GetNativeTextureFormat`
in `nu2api/nu3d/android/nutex_ios_ex.cpp`, preserving `-O3` at both owners
and declaring it in the texture API header. Retail has the 255-byte name
helper at `0x29f150` immediately followed by the converter at `0x29f250`,
inside the texture-loading run with an embedded platform source path.
The rebuilt helper is also 255 bytes; every remaining difference is a PIC,
literal, or jump-table address operand. This placement does not claim the
entire original translation-unit boundary is recovered.

**139,924 integer-parser cases per architecture** pass against the actual
production unit on NDK x86 and full-global 64-bit ASan/UBSan. An independent
forward Horner oracle checks both signs for every one- and two-byte input,
embedded terminators, decimal overflow boundaries, and 8,320 randomized
strings through 64 bytes. Exact-sized allocations check termination bounds
and input preservation. No external parsing service is mocked.

**85,540 texture-name cases per architecture** pass with the actual platform
unit and full global ASan/UBSan: all signed 16-bit inputs, four signed-32-bit
boundaries and 20,000 full-width random values. Verify exact spelling,
persistent/default pointer identity, and the thirteen added enum values.
No GL services are exercised or mocked. Target/native builds and all five
repository checks pass; no gameplay or graphics-context run was performed.

## Batch 43: typed mouse state and emulated touch input

`NuInputDevice::ConvertToEmulatedTouchFromMouse` improves **7.925% to
16.321%**, raising whole fuzzy matching **64.760590% to 64.761140%** with
no other score changes. The existing public `Update` caller does not regress.
All eleven PR checks for published batch-41 commit `1a7b0979` are green.

The old conversion body was empty. The original sets both current and last
valid type to touch, clears both type indices and the current touch count,
and emits zero, one or two touches in mouse-button order. Each active button
produces active value one, copies its release and press bytes, copies mouse
X/Y, and uses the button index as touch ID. It then clears all sixteen mouse
bytes and replaces capabilities with `0x400`. It does not clear unused
touch records, padding or the two additional per-touch float fields.

Replace the mouse record's six anonymous bytes with two three-byte button
states, retain two reserved bytes, and correct its two coordinate fields
from `u32` to `f32`. Original `movss` loads and their touch-coordinate stores
establish the float interpretation. Static assertions preserve the sixteen-
byte record and offsets 8/12 on every host. No source uses the removed
anonymous fields; platform mouse readers already clear the complete record.

Use an ordinary two-button loop in the existing `-O2` unit. The rebuilt
197-byte body is smaller than the original 313 bytes: the latter has separate
button blocks, a vectorized type/index store and a different frame. Do not
force vectorization, add alignment or manually expand the loop merely for
matching. Its behavior is now present despite the remaining low score.

**65,536 cases per architecture** pass with NDK x86 and full-global 64-bit
ASan/UBSan: 16,384 direct conversions compare the complete object against an
oracle; 49,152 integrations call the real constructor and public `Update`,
with emulation enabled/disabled and an already-touch device. Tests exercise
both button masks, non-boolean byte patterns, all byte values for press and
release, signed-zero/infinite/NaN/subnormal coordinates, reserved bytes,
untouched touch slots and fields, copied previous-touch state and actual
translator dispatch arguments.

The existing canonical friend is the private-access test seam; no private
macro rewrite, raw native offset or fake vtable is used. Platform polling,
the translator and scalar maximum service are mocks; the separately owned
empty destructor has an empty fixture definition. Actual mouse hardware,
button widgets and the full input manager were not run. Target/native
builds and all five repository checks pass.

## Batch 44: play-cutscene action and script registration

`Action_PlayCutScene` improves **8.750% to 99.562%**, raising whole fuzzy
matching **64.761140% to 64.764220%** with no other score changes. Correct
its return type to `i32` and restore the null `PlayCutScene` script-table
callback using the canonical action header.

On the first invocation, scan positive-count parameters with `NuStrIStr`.
The actual keyword is `"name"`, **not** `"name="`; the selected pointer is
always five bytes after the match, without validating the separator. The
last matching parameter wins, even if later parameters do not match. Only
after scanning does the handler load `WORLD->cutscene_sys` and call
`NewCutScene(NULL, system, name, 0)`. It ignores the returned cut pointer and
always returns one. Do not add world/system guards or validate the name
beyond the original scan; `WORLD` is required only on the request path,
while a null cutscene system is forwarded to the downstream API.

The rebuilt function is 160 bytes, exactly the original size. Only two
independent zeroing instructions exchange order and one literal address
differs. Full ELF table inspection verifies original record 110 at
`0x61f528`, callback `0x190890`, and `(1, 0, 0)` flags against the rebuilt
table. The previously restored `EndCameraCut` record remains correct.

**122,880 cases per architecture**, plus three null-world gate checks,
pass on NDK x86 and full-global 64-bit ASan/UBSan. The matrix covers every
three-token sequence from eight miss/case/embedded-key/separator patterns,
negative/zero/positive counts, zero/positive/negative first-time values,
null/non-null cutscene systems, queue success/failure, and search callbacks
changing the current world, its system, or a future parameter. Tests verify
the exact search order, selected pointer, final callback arguments and
return value. `NewCutScene` and unrelated locator services are mocks; no
full script-VM or cutscene playback execution is claimed.

Rebuild and rerun all **327,680 neighboring end-camera cases per
architecture** against this final owner. Target/native builds and all five
repository checks pass. No compiler hints, calling-convention attributes
or source optimization changes were introduced.

## Batch 45: cached Android documents path and preserved host override

`NuIOS_GetDocumentsPath` improves **7.683% to 99.905%**, raising whole fuzzy
matching **64.764220% to 64.769320%** with no other changed scores. Restore
the original **256-byte GLOBAL `g_internalPath`** cache and its shared
declaration; the already-recovered `g_internalDataPath` remains owned by
the JNI/platform state unit and is accessed through its canonical header.

On an empty cache, the original probes
`mnt/sdcard/TTGames/com.ttfusion.legosaga/save.here` in `"rb"` mode. Success
copies `mnt/sdcard/TTGames/com.ttfusion.legosaga/` into the cache before
closing the marker. Failure copies `g_internalDataPath` and appends `/`.
The missing leading slash in those literals and unconditional appended
slash are retail behavior, not mistakes to normalize here. A nonempty
cache skips all probing. The returned pointer is the mutable shared cache;
close failures do not alter the control flow.

The platform fallback keeps its existing `SAGA_HOST_WEAK` boundary so
`host/platform/documents.cpp` continues to supply the host-configured path.
The nearby comment records that real platform-linkage requirement. No
matching-only attribute is added. At 261 bytes the target shape is exact;
the only two linked differences are mode/marker string address operands.

**16,320 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan with file access replaced by recording wrappers. Coverage includes
input lengths 0–254, ordinary/slash/high-byte path contents, cached/uncached
states, probe success/failure, post-probe source/cache changes, close-time
cache changes, ignored close failures, repeat calls, cache reset and complete
buffer preservation. The source-length bound leaves room for the appended
slash and terminator in the fixed cache; longer inputs retain the original
unbounded-copy precondition and are not claimed safe. No actual Android
path is opened by the fixture.

An additional **301 native ASan/UBSan cases** link the real strong host
override alongside the platform owner and verify persistent pointer identity,
configured paths and the existing 255-byte truncation behavior, with no
Android marker probes. Target/native builds and all five repository checks
pass. No device storage or game-save execution was tested.

Across batches 42–45, seven functions improve with no regressions or exact
matches lost. The five body/API restorations are distinct from two incidental
literal-address improvements documented in batch 42.

## Batch 46: AI-message pool allocation portability

The low-scoring `CheckGizAIMessage` already implements the original lookup,
prefix handling and free-to-active transfer. One isolated common-return
rewrite reduces its score from **8.324% to 0%** and is rejected; do not repeat
this block-order permutation or add compiler hints. Its existing unchecked
copy for names containing `msg_` retains the original bounded-input
precondition.

Inspection of the surrounding lifecycle finds a real native defect:
`CreateGizAIMessageSys` allocates and clears a hard-coded 24-byte header,
then allocates 56 bytes per message. On a 64-bit diagnostic host these
pointer-bearing types occupy 48 and 64 bytes. An exact-size allocation
fixture reproduces an ASan heap-buffer-overflow in `ResetGizAIMessageSys`
while writing immediately beyond the 24-byte header.

Use `sizeof` for both allocations and header clearing, consistent with the
existing typed pool reset. Add Android header-size and member-offset
assertions. Preserve the original first-allocation failure, zero-capacity
pool and partially initialized header returned after second-allocation
failure; no new count policy is introduced.

**780 allocation/lifecycle cases per architecture** pass on NDK x86 and
full-global 64-bit ASan/UBSan, linking the actual message, linked-list and
string implementations. Coverage includes capacities 0–64, either allocation
failure, four initial memory patterns, exact requested sizes, full header
clearing, repeated activation/exhaustion/reset, case-insensitive reuse,
enumeration, value clearing without metadata clearing, cached-pointer/null
gates, the 26-character unprefixed boundary and a 31-character embedded-prefix
name. Only the allocator is replaced; full AI-world setup and invalid or
overflowing capacities are not exercised.

The target remains **64.769320%**, with **no changed function scores**;
`CreateGizAIMessageSys` retains its exact 100% match. Target/native builds and
all five repository checks pass. This is a portability repair, not a fuzzy
score gain.

## Batch 47: spaceship matrix helpers

`ChrisGetSpaceShipMatrix` and `ChrisGetTargetedSpaceShipMatrix` both improve
from **9.545% to 100%**, raising whole fuzzy matching to **64.776550%**.
These two original 189-byte functions are identical: copy the complete
`object->apiobj.field_0xb8` matrix, then call `NuMtxPreRotateY` with `0x8000`.
The targeted variant does not select a different matrix or inspect targeting
state. Both require valid object/output pointers; no extra guards are added.
Use the existing typed matrix member and expose the recovered signatures in
the owner header. Existing source optimization is unchanged.

**8,192 cases per architecture** pass with NDK x86 and full-global 64-bit
ASan/UBSan. Half record the rotation call to verify all 64 copied bytes,
the exact angle and output pointer, one callback invocation and preservation
of the callback's output. The other half link the real rotation implementation
and real trig-table initializer and compare the six rotated components with
an independent scalar oracle. Both halves exercise both helpers, separate
output and in-place aliases, untouched matrix components, complete source
object preservation and output guards. Recording tests use arbitrary float
bit patterns; arithmetic tests use bounded finite values. No live spaceship
rendering is claimed. Target/native builds and all five checks pass, with
two new exact matches and no regressions.

## Batch 48: customiser menu exit

`CustomiserMenu_End` improves **10.500% to 97.425%**, raising whole fuzzy
matching to **64.780250%**. Restore level-lock slot 1 and its two-second
delay, remember the currently selected menu cursor, reset the menu, reset
`JoinInTimer` to zero, then set both private customiser modes to 2. Only
after those callbacks does the original check `GAMEDEMO`. A nonzero value
becomes 2 and requests `HUB_LDATA` through `NewLData`, with all four next-area,
free-play, player-list and model-list flags set to 1. The normal path leaves
those values untouched. The existing canonical flag declarations are reused.

The rebuilt function has the original 200-byte size. Remaining differences
are two private mode-storage operands and register/load scheduling in the
demo branch. No optimization or calling-convention adjustments are made.
The neighboring `Customiser_SetUpCharacterData` changes **15.023% to
15.048%** through four register-only instruction differences; this is not a
second behavior restoration. No function scores regress.

**3,360 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan. Tests cover all ten valid menu levels, signed demo/flag boundary
values, null/non-null hub destinations and each external callback changing
demo, destination, flags, lock/time or menu-level state. They verify callback
order, exact selected cursor/timer arguments, post-callback condition loads,
normal-path preservation, timer output and final private modes. The actual
owner is compiled separately from the fixture with a test-only private-mode
accessor; there is no production accessor or source-copy replacement.
Menu and timer services are recording mocks, so full menu rendering and
area loading are not claimed. Target/native builds and all five checks pass.

## Batch 49: customiser toggle label return ABI

Correct `Customise_GetToggleString` from a void stub to an `i32` text-ID
return, improving **12.000% to 94.257%**. Whole fuzzy matching becomes
**64.781980%**, with no other changed function scores. Resolve the original
GOT entries to the two-byte signed globals `tCANCEL` and `tEDITNAME`, and add
their shared declarations to the text owner header.

Mode 1 returns the sign-extended cancel ID; mode 0 returns the edit-name ID.
For other modes, the original advances a local mode modulo 3, skipping zero,
before returning edit-name. Preserve that observed loop without modifying
the stored mode. Explicit unsigned addition preserves the target's wrapping
increment at `INT_MAX` without introducing signed-overflow UB. The valid
player indices remain 0 and 1. No UI-state mutation or localization lookup
is invented.

The rebuilt helper has the original 100-byte size. Its remaining differences
are the private mode-array operand and one zero-mode branch scheduled before
rather than after the invariant division-constant load. No follow-up branch
permutations or compiler hints are used.

**544,296 toggle cases per architecture** pass on NDK x86 and full-global
64-bit ASan/UBSan: every signed 16-bit text-ID value across all three normal
modes and both players, every signed 16-bit mode, four signed 32-bit mode
boundaries and 20,000 randomized player/mode cases. The independent oracle
selects solely on whether the initial mode is 1 and checks both stored modes
are preserved. The final owner also reruns all **3,360 exit cases**, with
**6,720 exit-to-toggle integrations** verifying that exiting leaves both
players on the edit-name label. The same test-only private-mode accessor is
used; no production test seam is added. Target/native builds and all five
repository checks pass.

Across batches 46–49, five functions improve, two exact matches are gained
and no function scores regress. Four bodies are restored; one additional
register-allocation improvement is incidental. The message-pool repair
preserves matching while fixing a reproduced 64-bit allocation overflow.

## Batch 50: socket enable/disable controls

`SockOn` and `SockOff` both improve **10.303% to 100%**, raising whole fuzzy
matching to **64.785820%** with no other changed scores. Each original is
101 bytes under the owner's unchanged default `-O0`.

Both ignore null systems and indices outside 0–63. `SockOn` clears the
16-bit flag `0x100` only when set; `SockOff` sets it only when clear. Name
that verified flag `SOCK_FLAG_DISABLED` and assert its member offset at
`0x68`. The original checks the fixed 64-entry allocation, not the number
of populated rails, and neither checks the selected socket's `valid` byte.
Do not invent either additional gate. A non-null pool is required only
for a non-null system and in-range index.

**264,240 calls per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan. Coverage includes every 16-bit flag word distributed over all
64 slots, eight independent flag patterns at every slot, repeated calls,
signed rail-count boundaries, null systems and invalid indices with either
null or valid pools. Complete pool comparisons include both guard entries
and every non-flag byte; the system header is preserved. The real source
is compiled separately from the fixture. Full-global ASan keeps the parser
dispatch table, whose unrelated external services have aborting mocks;
the tested flag-only functions make no service calls. Target/native builds
and all five checks pass. No live camera/rail traversal is claimed.

Read-only follow-up: `NuSpecialDrawAtAlpha` is already behavior-complete,
but its original frame and scalar literal loads are unoptimized while the
current `nucore_plain.cpp` owner is optimized. One isolated direct-handle,
separate-guard/common-result rewrite scores **9.556%** and is rejected.
Do not repeat expression variants; resolve the actual NuSpecial wrapper
translation-unit grouping before changing placement, without altering
existing optimization settings or adding per-function overrides.

## Batch 51: push-block knock impulse

`KnockPushBlock` improves **12.353% to 77.706%**, raising whole fuzzy
matching to **64.787740%** with no other changed scores. A null block does
nothing. Otherwise negate the three input direction components into the
block's typed velocity, scale it in place by exactly `0.005f`
(`0x3ba3d70a`), then OR `0x80` into the current runtime flag byte. The flag
write follows the scaling call, preserving callback-driven flag changes.
No input-vector null check or flag clearing is added.

The rebuilt body has the original 139-byte size and stack realignment under
the unchanged `-O1` source settings. Remaining differences are register-save,
argument and address-calculation scheduling, a final SSE register choice
and the sign-mask operand. No alignment or compiler attributes are added.

**7,684 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan: 6,146 recording cases check bitwise sign changes, exact scale
bits, in-place argument identity, arbitrary float patterns, every flag byte,
post-scaling flag mutations and null-block gates; 1,538 integrations use the
real vector scaler on bounded finite values, including signed zero and
subnormals. Tests exercise external input, the velocity itself and the
neighboring target-velocity vector, comparing the complete typed block and
preserving the external source. No full block physics or collision traversal
is claimed. Target/native builds and all five repository checks pass.

## Batch 52: death feedback controller forwarding

`DieRumble` improves **16.154% to 78.269%**, raising whole fuzzy matching
to **64.789290%** with no other changed scores. Restore the null-object and
signed player-flag gates, then issue rumble strength `1.0f` followed by buzz
duration `0.3f`, both with mode zero. Reload the controller and its pad after
the first call; do not recheck the player flag after that callback. Active
objects require a controller, but a null pad is forwarded to the existing
services, which handle it. The canonical gamepad header declares the helper.

The unchanged `-O3` owner emits 133 bytes versus the original 117. The call
sequence and operands agree; a duplicated early-return epilogue and inverted
active-branch layout account for the remaining mismatch. No additional
branch permutations, attributes or compiler hints are attempted.

**23,042 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan. Half use recording rumble/buzz services; half link the actual
gamepad owner and record the sound-layer callback. Coverage includes every
flag byte, null objects, inactive objects with null controllers, null and
distinct pads, callbacks changing either the controller or its pad pointer,
and callbacks clearing the player flag. Tests check exact float bits, mode,
call order, real service conversion to rumble amount 255, and complete object
and controller preservation outside callback mutations. Sound hardware is
not exercised. Target/native builds and all five repository checks pass.

## Batch 53: scene instance-ID file header

`ReadInstanceIDs` improves **20.000% to 100%**, raising whole fuzzy matching
to **64.790670%** with no other changed scores. Restore the original two
integer reads followed by a memory-file address query: store the first word
as the scene's instance-ID count, discard the second, then retain the returned
borrowed pointer. The body is the original 82 bytes under unchanged `-O3`.

Replace the scene's opaque eight bytes with `i32 num_instance_ids` and
`void *instance_ids`, asserting target offsets `0x58` and `0x5c`. The pointed
record format is not established by this helper and remains opaque. Native
builds now have a pointer-width field rather than an assumed four-byte slot.
The routine neither copies the records nor consumes their payload, performs
no validation and does not own the memory-file buffer. No count-based gate,
null-scene check or invented file-error policy is added.

**65,536 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan, using the separately compiled real owner and recording file
services. Tests cover signed count/header/handle boundaries, randomized
header words, null/interior/one-past returned addresses, exact call order,
the count store before the second read and callback mutations of both
fields. Complete scene comparisons preserve neighboring state. These are
header/service-contract tests, not a complete scene-file integration.
Target/native builds and all five repository checks pass.

## Batch 54: speeder-bike pickup collision scale

The private `GizmoPickups_Collide2D` callback improves **20.952% to 99.952%**,
raising whole fuzzy matching to **64.791880%** with no other changed scores.
Return exactly `2.0f` only when the current level equals `SPEEDERCHASEA_LDATA`
and the object's signed 16-bit ID equals `id_SPEEDERBIKE`; otherwise return
positive zero. Resolve all three original GOT references and the `2.0f`
literal at `0x5767cc`. Reuse existing typed fields and declarations, retaining
the callback's original private linkage and existing symbol-retention marker.
The 72-byte rebuilt body differs only in the floating-literal operand. No
source relocation, extra null gate or compiler adjustment is made.

**524,290 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan. Tests cover every 16-bit character-ID pattern, matching and
different IDs, equal/distinct/null level identities and null objects only on
the nonmatching-level path. Exact return bits and complete object/world state
are checked. A test-only accessor exposes the private callback from the
actual owner compiled separately from the fixture. No production accessor
is added, and live pickup traversal or startup callback wiring is not claimed.
Target/native builds and all five repository checks pass.

## Batch 55: Indy gizmo registration table

`RegisterGizmoTypes_Indy` improves **18.261% to 99.957%**, raising whole fuzzy
matching to **64.793670%** with no other changed scores. Decode the original
24-word table at `0x622fe0`: 23 registration callbacks in their exact order,
followed by a null terminator. Restore a writable local copy and forward the
two buffer cursors plus the original final argument `12` to the shared
registrar. That argument is not the number of callbacks. Reuse canonical
callback headers and add the wrapper declaration alongside the LSW/Batman
registration entry points.

The body has the original 103-byte size under the existing character owner's
`-O3` settings, differing only in the table-address operand. No translation
unit move or compiler workaround is used.

**10,240 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan, compiling the actual owner separately from the fixture. Tests
verify all 23 callback identities and positions, the null terminator, exact
buffer/mode forwarding, null and aliased cursor pointers, callback cursor
mutations and a fresh table after the shared-registrar mock clears the prior
copy. The individual registration callbacks are identifying mocks; complete
gizmo allocation and live level initialization are not claimed. Target/native
builds and all five repository checks pass.

Across batches 50–55, seven functions improve, three exact matches are gained
and no function scores regress. Existing optimization settings, symbol rules
and matching denominators are unchanged.

## Batch 56: shared gizmo registrar allocation repair

Following the restored Indy entry point into the shared registrar exposes
two native allocation defects and a missing retail failure gate. Replace
the fixed 12-byte header and four-byte progress slots with `sizeof(GIZMOTYPES)`
and `sizeof(VARIPTR)`. Assert the target header size and pointer offset in
the implementation. The corresponding native sizes are 16 and eight bytes;
the Android sizes remain 12 and four.

Before the change, full-global ASan reproduces two eight-byte heap-overflow
writes: the header's type pointer in a 13-byte arena, and progress pointers
in a 353-byte arena whose last allocation reserves only 48 bytes for twelve
native pointer slots. Both reproductions pass after the repair.

The original instruction at `0x4af669` tests the newly allocated progress
array and skips its initialization on failure. The reconstruction instead
tested the already-valid header, allowing a null-array write. Restore the
array check on every platform. Keep partial initialization and strict arena
capacity semantics unchanged; do not invent a rollback or retry policy.

**3,391 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan. These link the actual registrar, bump allocator, string helpers
and Indy wrapper. Coverage spans 0–23 types, progress counts 0/1/2/12/31,
four callback-presence patterns, seven capacity boundaries, partial/failing
allocations, mixed empty/nonempty prefixes, null/empty callback tables,
already-initialized gates and all Indy registrations. Complete arena bytes,
cursor positions, callback counts, header/record fields and full-width
progress pointers are compared against an independent layout oracle.
Registration descriptors and progress payloads are test fixtures; unrelated
retained virtual-interface callbacks have aborting mocks. Negative/overflowing
counts, misaligned or malformed arenas and live gizmo initialization are not
claimed as supported.

This correctness restoration changes `RegisterGizmoTypes` from **73.706% to
71.436%**, leaving whole fuzzy matching at **64.793310%**. No other function
score changes and no exact match is lost. Retain the verified missing guard
instead of preserving a crash for a better instruction score; no compiler
workarounds are used. Target/native builds and all five checks pass.

## Batch 57: LZ2K buffer entry point and shutdown

Restore `ExplodeBuffer` from a parameterless void stub to the original C
entry point accepting input/output pointers and returning a signed size.
Its match rises **10.000% to 99.975%**. Check initialization, obtain the
uncompressed size through `ExplodeBufferSize`, and only for a nonzero result
read the compressed-size word at byte 8. Forward the payload at byte 12,
output pointer, compressed size and uncompressed size to the existing
`ExplodeBufferNoHeader`, returning its result. Correct the swapped parameter
names in that helper's declaration; its established ABI and implementation
are unchanged. Restore `ExplodeExit`'s conditional initialization-flag clear,
improving **40.000% to 99.800%**.

Both have the original sizes, 132 and 36 bytes, under the owner's unchanged
default `-O0`. Only private initialization-state operands differ. Whole fuzzy
matching becomes **64.796280%**, with no other changed scores in this batch.

**57,344 wrapper/state cases per architecture** pass on NDK x86 and
full-global 64-bit ASan/UBSan: signed header/result boundaries, randomized
forwarded values, zero/nonzero initialization states, null outputs, exact
call order, callback-driven flag changes, repeated exits and disabled calls
with null inputs. Another **1,548 cases per architecture** use the actual
header parser, no-header driver and ring-buffer copier. They cover all invalid
single-byte magic substitutions, zero/small output sizes, boundaries around
8,192 and 16,384 bytes, literal runs, three-byte repeated backreferences,
output guards and both disabled entry points.

Fixtures compile the actual owner separately, with test-only private-state
accessors and weakened service symbols in temporary objects. Native objects
use ordinary PIC semantic interposition at `-O0`. The integration supplies
decoded symbols and offsets instead of using the entropy decoder; arbitrary
compressed-stream correctness and malformed-stream safety are not claimed.
Production linkage, attributes and compiler options are unchanged.
Target/native builds and all five repository checks pass.

## Batch 58: reconnect socket-camera configuration callbacks

Restore the ten socket-camera callbacks stranded in `nufpar.cpp` and absent
from the socket command table. Move the complete private callback group to
`socksysall.cpp`, alongside `sockpar_sock`, the related callbacks and their
table. Live compile commands confirm both owners use default `-O0`; no source
membership or optimization settings change. Replace the opaque socket tail
with nine named float fields and a terrain-camera flag, retaining its legacy
byte-array view and adding target offset assertions for `0x114`–`0x138`.

The nine scalar callbacks capture the selected socket, read one float and
store it without clamping. The terrain callback reads no parser input and
sets its field to 1. Explicit capture preserves the original pre-parser
selection even if a service changes `sockpar_sock`, including in C++17 native
builds. At `-O0` this local occupies a stack slot instead of the original
saved register: each float callback is 69 rather than 65 bytes, improving
**19.048% to 75.667%**. The terrain callback improves **44.444% to 100%**.
No register hints or expression permutations are used. Whole fuzzy matching
becomes **64.803696%**, with ten improvements and no score regressions in
this batch.

Restore table rows 46–55 using the exact original keyword spelling and order.
All **57 rows**, including the existing entries and terminator, compare equal
to the original table after resolving string/function pointers.
**122,880 recording cases per architecture** pass on NDK x86 and full-global
64-bit ASan/UBSan: finite/quiet-NaN/infinity/signed-zero/subnormal values,
randomized full socket contents, global socket redirection/nulling by the
parser callback, ignored context pointers and parser-free terrain updates.
Complete comparisons verify both sockets and the selected global.

Another **2,160 cases per architecture** link the real parser, case-insensitive
dispatch and numeric/string helpers. They exercise every restored keyword,
all eight command-stack depths, three keyword casings, signed/fractional
values and absent values. The fixture uses the parser's actual 512-byte wrap
configuration, not its 514-byte backing-store size. The final owner also
reruns all **264,240 socket-toggle cases per architecture**. Target/native
builds and all five checks pass; live camera behavior is not claimed.

## Batch 59: fixed-width hexadecimal appenders

`CatIToX` and `CatI64ToX` improve **22.105% and 26.471% to 100%**. Both scan
to the existing terminator, call the corresponding fixed-width formatter,
then write NUL through its returned pointer. The resulting bodies have the
original 65- and 81-byte sizes. Add canonical formatter/appender declarations
and make the utility header self-contained for its existing float types.

Real-formatter integration reproduces a UBSan failure in `IToX`: shifting
`2147483647` left by eight overflows its signed intermediate. The two halves
of `I64ToX` use the same pattern. Perform each left shift in `u32`, retaining
the original signed intermediate afterward. This preserves target bit
patterns and both existing formatter scores. An initial wider unsigned-local
variant reduced the 64-bit formatter's score and was replaced by this smaller
repair; no undefined shift is retained for matching.

**266,240 cases per architecture** pass on NDK x86 and full-global 64-bit
ASan/UBSan with both actual owners and no formatter mocks. An independent
unsigned-nibble oracle checks signed 32/64-bit boundaries, randomized values,
prefix lengths 0–64, arbitrary nonzero prefix bytes, lowercase zero-padded
digits and the final terminator. Allocations have exactly the required tail
capacity, with prefix guards and complete-buffer comparisons. These routines
retain the original valid-buffer/capacity preconditions.

Whole fuzzy matching is **64.806030%**, with no other score changes in this
batch. Target/native builds and all five repository checks pass. Across
batches 56–59, fourteen functions improve, three exact matches are gained and
none is lost. The sole score regression is the documented registrar repair
that restores allocation-failure handling and fixes native pointer storage.

## Batch 60: directory sort callback contracts

Baseline: `733a831c`. Restore the four name/size sort callbacks in
`gamemenuall.cpp`, retaining its live `-O3` setting. All four become **100%**:
size ascending **26.250%**, name ascending/descending **30.000%**, and size
descending **35.000%** before this change. Whole fuzzy matching reaches
**64.808740%**, with four exact matches gained and no regressions.

Move the existing `FilePickDirectoryEntry` definition from the editor source
to its shared `edui.h`, retaining all fields and its 0x118-byte layout. Assert
the size, signed size field at four, signed year at 0x16, and inline filename
at 0x18. Publish all six comparators with the real `qsort`-compatible
`i32(const void *, const void *)` contract. The existing year comparators keep
their bodies and 100% scores after adopting this shared record type.
Name order uses the actual `NuStrCmp`, including its signed-character
semantics. Size order uses comparisons, not overflowing subtraction.

NDK x86 and full-global 64-bit ASan/UBSan pass **1,179,648 comparator cases**
and **12,480 real `qsort` integrations per architecture**, linking the actual
menu and string owners. Coverage includes signed size/year boundaries,
all first-differing byte pairs, filename-prefix lengths through 253,
self/equal comparisons, ascending/descending order, empty through 64-entry
arrays, exact record preservation and boundary guards. Unrelated retained
menu services are aborting mocks. Target/native builds and all five checks
pass; no directory UI or file-system enumeration is claimed.

## Batch 61: hierarchy loading entry point

Restore `NuHGobjRead(VARIPTR *, char *)` beside the Android hierarchy helpers
at the existing `-O0`. Its local end cursor is all-bits-one, and it forwards
the path, caller-owned cursor pointer and by-value end to `NuGHGRead`.
Use pointer-width `usize` for the sentinel rather than truncating it to 32
bits on diagnostic hosts. Retain the existing void declaration: the original
leaves the callee's result in EAX, but no original call site was found that
establishes a different public return contract.

The wrapper improves **22.222% to 100%**, reproducing the original 58-byte
body under fuzzy comparison and raising whole matching to **64.809685%** with
no collateral changes. **51,200 cases per architecture** pass NDK x86 and
full-global 64-bit ASan/UBSan: null/non-null forwarded arguments, full-width
cursor bits, by-value sentinel, single-call ordering, callback modifications
to the cursor/path and preservation of neighboring objects. The graphics
loader is mocked; successful asset loading is not claimed. Target/native
builds and all five checks pass.

## Batch 62: terrain accessor and legacy geometry count

Correct `CrashDataPtr` from a void stub to the pointer-returning accessor for
its owner's existing `crashdata`. It improves **12.500% to 99.875%**; only
the private BSS operand differs. Both terrain readers already maintain that
pointer. No new ownership or lifetime semantics are introduced.

`NuFadeSetFxCodeMtls` is another incorrect void placeholder. The original
Android body simply counts a `nugeom_s` linked list starting at one, returns
that count, and never reads or writes the byte-buffer argument. Restore this
exact contract, not behavior guessed from its name. Introduce the recovered
geometry link prefix in a canonical header and replace the empty scaffolding
definition; the unknown tail is explicitly unreconstructed. The `-O0`
45-byte function improves **17.500% to 100%**.

Whole matching reaches **64.810880%**, with no other score changes. Per
architecture, **4,100 geometry-list cases** and **6,528 actual terrain-reader
integrations plus a failed-load reset** pass NDK x86 and full-global 64-bit
ASan/UBSan. Geometry tests cover empty/reverse-linked lists through 1,024
nodes, ignored null/non-null byte buffers and complete input preservation.
Terrain tests use synthetic serialized chunks and the real two readers to
verify reset timing, last-crash-chunk selection, reused buffers, zero-sized
payloads, cursor advancement, path lengths through 95, and unchanged file
bytes. Only the file-loading service is mocked. Spatial-index/group loading
and cyclic or unbounded geometry lists are outside these tests. Target/native
builds and all five checks pass.

## Batch 63: door vehicle keyword behavior

`D_vehicle` previously consumed and discarded every word. Retail consumes
exactly one word. Case-insensitive `all` replaces the full mask with 0xffff;
otherwise the registered `LevelCharacterTypeIDFn` resolves the word, its
result is narrowed to the low byte, and values below 64 set one mask bit.
There is no null callback guard on the lookup path. Reload the selected door
after parser/resolver calls, since the original does not retain it across
those boundaries.

Preserve an important retail quirk: its variable shift is **32-bit**, so IDs
32–63 alias bits 0–31 and never set the high word. Express this as unsigned
`1u << (type & 31)` before widening to the mask, avoiding undefined shift
counts or signed bit-31 overflow. A promoted signed integer local reproduces
the original byte-extension and signed comparison; retaining a byte local
emitted a shorter byte comparison and scored 90.956%.

The retained original-sized 186-byte body improves **16.756% to 94.622%**.
Remaining differences are private-state operands and store/epilogue
scheduling on the `all` path; no hints or ABI attributes are added. Two
unchanged neighbors improve slightly: `Door_RegisterGizmo` **99.327% to
99.418%**, and `Doors_Configure` **8.651% to 8.684%**. Their before/after
binary diffs were reviewed; these are operand/layout effects, not additional
restored behavior.

**36,864 recording cases** and **73,728 real-parser integrations per
architecture** pass NDK x86 and full-global 64-bit ASan/UBSan. Tests invoke the
actual keyword table with the real string comparator, cover all low-byte IDs,
signed/full-width resolver boundaries, existing high/low mask bits, missing
and mixed-case `all` values, parser/resolver changes to the current door, all
eight command-stack depths and three keyword casings. Extra words remain
available to the caller after the vehicle value is consumed. Unrelated
keyword services abort if reached; character-name resolution is mocked.

Target/native builds and all five checks pass. Across batches 60–63, whole
fuzzy matching rises **64.806030% to 64.813960%**: ten functions improve,
none regress, six exact matches are gained and none is lost. All eleven PR
checks on the baseline commit `733a831c` are green.

### Further structural triage

Do not attack `MakeWingFormation` as an isolated 2.045% function. The original
private helper takes arguments in EAX, EDX and XMM0, and its only caller is
the still-unfinished `ProcessSpaceLevel`. The current caller and helper are
in separate owners, and the space-level overlay does not describe the
original eight 0x658-byte formation records coherently. Restore the caller,
shared layout and source ownership as a group; no standalone compiler
experiments or calling-convention overrides were attempted.

`ImplodeFReadMem` likewise needs the compression private-state/TU group:
the original is `-O0`, its current owner uses `-O3`, and its private remaining
input counter is not declared there. Its low score is not justification for
another isolated control-flow permutation. No edit or compiler trial was
made for that function.

## Batch 64: public socket bit accessors

Restore the public `SetSockBit` and `SockBitSet` bodies at their existing
`-O0`. The query returns a 32-bit integer, not the previous boolean helper
result; correct its canonical declaration too. Retail divides the signed
index by 32 with truncation toward zero and masks the shift count to five
bits. Consequently indices -31 through -1 address the first word, while the
previous arithmetic-right-shift helper addressed memory before the array.
An actual-owner UBSan reproducer confirms that old negative-index access.
Keep the separate private helpers unchanged; this is not a new bounds-check
policy for other socket operations.

`SetSockBit` improves **22.806% to 82.581%** and `SockBitSet` **15.290% to
85.613%**, with no other score changes. Whole matching reaches **64.816055%**.
The first natural recovered bodies are retained; remaining shift/register
differences do not justify compiler permutations. **389,120 get/set/repeated-set
sequences per architecture** pass NDK x86 and full-global 64-bit ASan/UBSan,
covering every defined array index from -31 through 63, both words, zero/all/
random mask patterns, integer query results and complete neighboring-record
preservation. Indices outside that original valid range remain unsupported.
Target/native builds and all five repository checks pass.

## Batch 65: shared runtime lighting storage and insertion

The low-scoring RTL insertion helpers expose a structural host-memory bug:
public `rtldata_s` reserved only 324 bytes, while the private pointer-bearing
`rtlidata_s` expands to 376 bytes on a 64-bit host. Calling the actual
`rtlResetEx` on a heap-allocated public record produces an ASan heap overflow.
Other public helpers and `rtlCalcLights` also used hardcoded Android offsets
to access native pointers or fields.

Move the recovered layout into canonical `rtldata.h`, retaining both original
ABI type tags through an empty public derived record. Public storage now
shares the actual private layout; valid base conversions replace unrelated
record casts. The target still asserts the original 0x144-byte size and field
offsets. Colour/vector union views retain both existing consumers' types.
Use typed fields in insertion, renderer forwarding, specular accessors and
the existing lighting calculation. Other runtime users already access those
public fields by name. Serialized light-set conversion and the incomplete
light-selection algorithm are not claimed as repaired here.

Recover `InsertLight` directly from its original two three-slot loops,
including the missing `rtl_error = 1` when neither list accepts a light.
Only type 1 uses the ambient list; every other byte value uses the directional
list. Preserve strict comparisons, ties, NaN rejection, stable shifts and
unchanged error state after successful insertion. `InsertAntiLight` appends
up to three pointers/strengths through the shared layout. Remove the unused,
non-original duplicate `rtlInsertLight` helper and an unused unsafe cast in
the selection loop.

`InsertAntiLight` improves **19.385% to 100%** and `InsertLight` **26.700% to
98.292%**. The latter differs only in byte promotion/comparison and a nop;
no matching-only hints are introduced. Typed accesses improve the existing
`rtlCalcLights` **27.301% to 30.319%** without a broader algorithm rewrite.
The selection loop decreases **10.036% to 9.962%** because removal of its
unused local shifts stack slots; its before/after binary diff was reviewed.
Keep that small cost rather than an unused unsafe cast. `rtlApplySetScale`
retains its score and uses a genuine public local record for the reset API.
Across batches 64–65, whole fuzzy matching reaches **64.824730%**: five
functions improve, one regresses, one exact match is gained and none is lost.

Per architecture, the actual runtime owner passes **8,192 reset cases,
1,679,616 insertion cases, 864 anti-light cases, 8,192 renderer/specular cases,
24,576 real-vector-math calculations and 1,024 public/local apply pairs**.
NDK x86 and 64-bit heap/stack ASan plus UBSan cover all light type bytes,
finite/NaN/infinite strengths, full-width pointers, reset/cache gates, complete
guarded-record preservation, callback argument identity, null specular gates,
directional/ambient/anti-light mixtures, rotations, scale and empty light sets.
The original native allocation reproducer passes after the repair. The RTL
fixture disables ASan global registration solely to discard unrelated editor
callback graphs; it does not claim full-global ASan coverage. Renderer,
profiling and unused list/random services are mocked; vector math, square
root and trigonometric initialization are real. Negative anti-light counts
are invalid original inputs and are not tested as supported. No live rendering
or gameplay is claimed. Target/native builds and all five checks pass.
All eleven PR checks for the preceding commit `482fe33a` are green.

## Batch 66: obstacle-controlled techno output

Restore the missing mode-3 path in `Techno_GetOutput`. After the existing
active/visible/target gates, compare the controlled gizmo type's name against
`GIZOBSTACLE` case-insensitively. On a match, reload the controlled gizmo after
the comparator and return whether its obstacle animation is at the end state.
The original does not add null checks inside that validated path. Mode 1
continues to report the panel's completion bit; other modes return zero.
Existing canonical gizmo, obstacle and animation-set fields cover the entire
chain without raw offsets or new overlays.

The first natural recovery improves **24.415% to 88.585%** at the owner's
unchanged `-O3`. Remaining differences are early completion-bit calculation
and branch scheduling. Two untouched neighbors have small collateral costs:
`Technos_LateUpdate` **99.963% to 99.685%**, and `Technos_Reset` **81.913% to
81.767%**. Their before/after diffs contain register choices, one moved load
and literal operands, not changed source behavior. No exact match is lost;
whole fuzzy matching reaches **64.826996%**.

Per architecture, **1,310,720 real-comparator/gate cases** and **38,400
callback-reload cases** pass NDK x86 and full-global 64-bit ASan/UBSan. Coverage
includes every flags/mode byte, all 256 type indices, five animation states,
null outer/target gates, mixed-case names and near misses, ignored output
arguments, unchanged guarded panel storage, and comparator-driven target,
mode and flag changes. The string comparator itself is real; its recording
wrapper injects the deliberate mutation cases. Target/native builds and all
five repository checks pass.

### Additional rejected low-score work

`FindMtlInHGObj` already has the correct one-based material scan; its zero
score needs source-owner/optimization evidence, not another scan rewrite.
The historical function-local optimization advice in chapter 21 is superseded
by the current skill. `FS_PrevNameLen`, the music track/path helpers and
several allocator wrappers are also already implemented; no speculative
compiler variants were run on them.

The three legacy frame-start wrappers are not a self-contained easy win:
their original `NuTimeGetTime` callee is an empty five-byte body, but the
wrappers consume its unspecified EAX value as a timestamp. Do not invent a
portable clock result or reproduce undefined return behavior merely to raise
their scores. No timing source change was made.

## Batch 67: shared client mine views

Restore `GetClientMineInfo`, which exports the addresses of 64 client mine
positions and two 64-bit masks in the existing `client_mines` global. The
previous void stub did not initialize any output. A canonical `CLIENTMINES_s`
in `netplay.h` replaces the unrelated 197-word array: typed positions and
64-bit masks share storage with the existing two-word level logic. Convert
all current mine-state consumers to those fields and reset the actual record
size. Android offsets 0x300/0x308/0x310 and the original 788-byte global size
remain asserted and verified in the linked ELF; 64-bit hosts naturally add
four trailing padding bytes for mask alignment. The opaque final word stays
uninterpreted. Existing mine-selection and bit-mask behavior is unchanged.

The accessor reaches **30% to 100%**, reproducing the original 50-byte body.
Typed accesses also improve `UpdatePodRaceMines` **42.725% to 43.629%**.
There are no regressions or lost exact matches; whole fuzzy matching reaches
**64.828026%**. **65,536 export/write/read/alias sequences per architecture**
pass NDK x86 and full-global 64-bit ASan/UBSan. Tests cover every position,
arbitrary float bits, both mask word views, full-width output pointers,
unchanged record/tail bytes, stable addresses and two aliased mask outputs
(the second assignment wins). The fixture supplies the shared global using
the canonical type; no live network or pod-race gameplay is claimed.
Target/native builds and all five repository checks pass.

## Batch 68: menu highlight traversal

Correct `FlushMenuHighlights` from a zero-argument placeholder to the original
menu-pointer ABI. Walk the existing item chain and clear only the highlight
bit on type-zero items; other types and flag bits remain untouched. The
canonical editor-menu header supplies the shared types and declaration. The
existing source owner and `-O3` setting are unchanged.

The first direct traversal improves **30% to 100%**, matching all 40 original
bytes under fuzzy comparison. An untouched neighbor, `MenuInCriticalMemoryCard`,
changes **100% to 99.483%**: its initial global load and test use ECX instead
of EDX, with all remaining instructions unchanged. Record this exact-match
loss rather than modifying the unrelated predicate to steer registers.
Whole fuzzy matching reaches **64.828606%**.

**33,280 guarded traversal/idempotence cases per architecture** pass NDK x86
and full-global 64-bit ASan/UBSan. Coverage includes empty through 64-item
lists, forward/reverse links, every flag byte, zero/nonzero/signed-boundary
types, untouched menu storage and complete neighboring-node preservation.
Unrelated menu services have aborting mocks. Null menus and cyclic lists are
not valid original inputs. Target/native builds and all five checks pass.

## Batch 69: platform movie-time return ABI

The original 38-byte `FmvTimePS` returns positive floating-point zero; it is
not a void function. Correct the canonical platform header and existing
`-O0` definition, without moving it to the differently optimized movie-play
backend. The direct constant return improves **30.909% to 99.909%**; only a
literal-pool operand differs. No other scores change, and whole matching
reaches **64.829160%**. A function-pointer ABI test verifies exact positive-zero
bits on NDK x86 and a full-global 64-bit ASan/UBSan build. Target/native builds
and all five repository checks pass. This restores the original unsupported
platform result, not a movie playback clock.
All eleven PR checks for the preceding commit `57cd2eca` are green.

## Batch 70: background-load frame timing ownership

Move the unchanged `NuFrameEndBgLoadPS` implementation out of the temporary
`nu2api_nucore_misc.cpp` catch-all into the existing `nucore_frame.cpp` owner.
The destination already owns `NuFrameEnd` and `NuFrameSetMinDelay`, sharing
the same `nuapi.time2` state and `bgSuspendMain` service. These functions also
belong to the same original address neighborhood. The live compile commands
show the catch-all at `-O2` and the frame owner at default `-O0`; neither mode,
source membership nor the optimization map changes. This is a source-owner
repair, not revival of the superseded function-local optimization attribute
experiment. The original ELF has no source-file records, so the precise
original filename remains unproven. Add its canonical C++ declaration to
`nuapi.h`, preserving `_Z18NuFrameEndBgLoadPSi`.

The unchanged body improves **0% to 99.908%**, with all 333 original bytes
accounted for and only eight float-literal operands differing. No other
scores change and no exact matches are lost; whole fuzzy matching reaches
**64.836210%**. **983,520 timing/order/threshold/reload cases per architecture**
pass NDK x86 and full-global 64-bit ASan/UBSan. The actual moved implementation
is linked against recording timing/suspend services. Tests cover the four
supported frame rates, neighboring floats, signed zero, infinities/quiet
NaNs, positive/negative fractional scanlines, inclusive delay thresholds and
callback-driven FPS changes. The fixture also checks the original time2
argument, local timing values and unchanged API state. Out-of-range float
conversions and signed arithmetic overflow are not valid fixture inputs;
this is not a real scheduler/timing-backend integration test. Target/native
builds and all five repository checks pass.

## Batch 71: memory-pool storage and callback contract

The legacy-release triage exposed a defect in the active release path too:
the class reserved 1,024 bytes but treated them as 256 native pointers.
On 64-bit hosts the second half aliases later class fields and extends past
the object. A real empty-pool `ReleaseUnreferencedPages` call reproduces a
sanitized invalid-pointer crash at free-list slot 128. Replace the byte
placeholder and its cast with a typed 256-pointer member; constructor and
release-all initialization use its actual size. Android retains its asserted
0x440-byte class layout; the 64-bit host record is 2,176 bytes. Existing
factory allocations already use `sizeof(NuMemoryPool)`.

Both original release routines test the callback's AL result, so recover the
boolean `IEventHandler::ReleasePage` contract consistently in the interface
and the two existing overrides. Remove the conflicting empty pool class in
the catch-all types header and include the canonical declaration. Virtual
slot order, symbol names, compiler options and active release logic stay
unchanged. `ReleaseUnreferencedPages` improves **58.716% to 59.405%**; no other
scores change, including the exact constructor, release-all and callback
implementations. Whole fuzzy matching reaches **64.836334%**.

**16,384 cases per architecture** pass NDK x86 and full-global 64-bit
ASan/UBSan. The fixture executes the real constructor, page insertion,
free-list/page merge sorting, active release/recycle paths, statistics,
release-all and destructor, with every free-list slot and all four-page
live/release masks exercised. Allocation services and page event callbacks
are recording mocks. The NDK fixture mocks pthread entry points because
Android mutex storage cannot be passed to the differently sized glibc mutex
implementation; the host fixture uses real pthreads. This is single-threaded
validation, not a concurrency proof. Native/target builds and all repository
checks pass after replacing an initially rejected `HOST_BUILD` assertion
guard with the standard `DECOMP_ASSERT` mechanism.

`ReleaseUnreferencedPages_OLD` remains deferred: after a rejected release it
does not advance the current page. Repeated rejection can form a recycled
self-link; subsequent success can leave the recycle list referring to freed
metadata. Restoring that path as portable C++ or silently repairing it is
not justified by the current evidence. The active release path advances
correctly and is covered by the retained tests.

## Batch 72: scalar-math source ownership

Move the existing `NuPow`, its private 32-slot cache, `NuLog2` and `NuPower2`
from the optimized plain-symbol catch-all into `nufloat.c`, beside the
already recovered `NuPowFast`. The original cached and fast power bodies
are adjacent; their scalar-float ownership and unoptimized instruction
family support this grouping. Current live actions confirm catch-all `-O3`
versus scalar-math default `-O0`, with no build-option changes. Preserve all
bodies and private symbols, add canonical C-linkage declarations, and remove
the two redundant local declarations in the post-filter consumer. Spell the
numeric casts in C-compatible form: the target action compiles this `.c`
file as C++, while native/WASM declaration indexing parses it as C.

`NuPow` improves **5.111% to 99.700%**, `NuLog2` **48.636% to 99.955%**, and
`NuPower2` **28.286% to 54.952%**. No other scores change and no exact matches
are lost. Whole fuzzy matching reaches **64.844820%**. The cached power body
has identical instruction structure, with private/literal operands and
separate loop-local stack slots remaining. The integer helper still uses
stack locals instead of retail registers; no register hints are added.

Per architecture, NDK x86 and full-global 64-bit ASan/UBSan pass **66,892
cache/IEEE cases**, **131,163 integer cases**, and **32,768 logarithm dispatch
cases**. Power uses real libm with recording log/exp wrappers, checking
first-use initialization, cache hits/misses and wraparound, NaNs, infinities,
signed zero, and the retail sentinel collision (`FLT_MAX` to exponent zero
initially returns cached zero). Integer tests stay in the original defined
terminating domain through 2^30; larger positive inputs are not made safe
by inventing a different overflow policy. The log helper's dependency is
mocked to verify argument forwarding and exact float scaling. Target/native
builds and all five repository checks pass.

## Batch 73: Android dead-zone source ownership

Move the unchanged `NuPs2ApplyDeadZone` body from `nucore_plain.cpp` to
`android/nuapi_android.cpp`. The original places it alongside `PadRecPtr`,
hardware initialization and the SMB helper immediately before the
`_GLOBAL__sub_I_nuapi_android.c` initializer. The destination already owns
that Android API family and uses default `-O0`; its flags remain unchanged.
The existing canonical `nupad.h` declaration preserves plain C linkage.

Matching improves **25.727% to 100%**, reproducing all 152 original bytes.
No other score changes; whole fuzzy matching reaches **64.847210%**.
**917,760 cases per architecture** pass NDK x86 and full-global 64-bit
ASan/UBSan: every byte input across 769 signed dead zones, plus every signed
16-bit input across eleven representative dead zones. A widened-arithmetic
oracle checks signed truncating division and strict dead-zone boundaries.
Zero-divisor and overflowing paths are excluded rather than silently given
new behavior. Target/native builds and all five repository checks pass.

## Batch 74: adjacent SMB-path source ownership

Move the existing `Nu360ConfigureSMBSharing` and its private 256-byte buffer
to the same Android API owner, retaining its C++ name and complete behavior.
It follows the dead-zone helper in the original source neighborhood. Add
the canonical declaration in `nuapi.h`; the external file-service calls and
their order are unchanged. Matching improves **39.690% to 99.931%**, with
only two string operands left. No collateral changes or exact losses;
whole fuzzy matching reaches **64.848570%**.

**32,769 cases per architecture** pass NDK x86 and full-global 64-bit
ASan/UBSan, checking directory reset before loading, 256-byte buffer limits,
all valid string lengths, zero/nonzero/signed load results, stable buffer
addresses, callback-mutated output cells and full-width pointer assignment.
File/directory services are mocks: no actual working directory or SMB share
is touched. Target/native builds and all five checks pass. All eleven PR
checks for the preceding published commit `6a2da11d` are green.

## Batch 75: material lookup and its actual caller

Resolve the earlier zero-score `FindMtlInHGObj` deferral with new ownership
evidence. Its only recovered caller is `instGetLookAtLocatorInfo` in
`gcutscn.cpp`, which performs the three one-based material lookups. Both
functions are adjacent in the original cutscene block. Move the unchanged
scan from `things.cpp` to that existing caller owner and declare it in the
cutscene header. Live commands confirm source default `-O0` and destination
`-O2`; no flags or function attributes are changed.

The first owner-correct build improves **0% to 100%**, with all 85 bytes
matching and the caller's score unchanged. No regressions or exact losses;
whole fuzzy matching reaches **64.850370%**. **1,164,800 cases per
architecture** pass NDK x86 and full-global 64-bit ASan/UBSan: signed counts,
every byte material tag, full-width search values, first/last/duplicate
matches, reversed material arrays and untouched scene/material storage.
Target/native builds and all repository checks pass. These tests use the
declared `NUGSCN` view; they do not validate the full look-at path's existing
hierarchy/scene casts. In particular, the two recovered record prefixes
need a separate 64-bit layout audit before claiming native cutscene
integration.

## Batch 76: page-allocation selection check

Restore the original explicit head/capacity check after the page search and
relinking in `NuMemoryPool::PageAlloc`. Keep the unusual retail selection
rule: the first later page with insufficient space is moved to the front,
not the first page with enough space. Reload the selected head before the
allocation callback decision, and reload again after the callback. No
compiler settings, branch hints or allocation-failure policy change.

One bounded source trial improves **35.379% to 38.158%**, with no collateral
changes. The remaining differences are loop layout, instruction scheduling
and registers; no additional variants are tried. Whole fuzzy matching
reaches **64.850530%**.

**284,375 cases per architecture** pass NDK x86 and full-global 64-bit
ASan/UBSan. Tests use the actual pool constructor, page insertion, allocator,
release-all and destructor, covering zero through four initial pages,
all 625 four-page capacity patterns, seven alignments and thirteen request
sizes. The recording allocator callback checks the post-relink list, adds
a real page through `AddPage`, and verifies the ignored return value. Tests
also check exact output addresses, metadata/count updates, untouched page
contents and cleanup. NDK pthread calls are mocked for ABI compatibility;
the host uses real recursive mutexes. Target/native builds and all five
repository checks pass. This is not a concurrent allocator stress test.

## Batch 77: pointer-block and wind-group source ownership

Move unchanged `NuPtrBlockRead` beside `NuPtrBlockFix` in `nuptrblock.cpp`,
matching their contiguous retail bodies. Move unchanged `NuWindFreeGrp`
beside its allocator in `nuwind_groups.cpp`; the original places both in
the wind-group run ending at `_GLOBAL__sub_I_nuwind.c`. Live target actions
confirm default `-O0` for both destinations, versus `-O3`/`-O2` for their
former plain/misc owners. Add canonical declarations and remove redundant
local declarations. The wind helpers retain C++ linkage and are declared
only to C++ consumers of the otherwise shared header.

Both helpers reach **100%**, from **54.294%** and **54.875%** respectively.
No other scores change; whole fuzzy matching reaches **64.851230%**.
Target/native builds and all five repository checks pass. Per architecture,
NDK x86 and full-global 64-bit ASan/UBSan pass **131,073 wind-release cases**
(all 16-bit states, null and repeated release, untouched adjacent storage)
and **67,584 pointer-block cases**. The latter uses a recording memory-file
lookup and the real fix-up body, checking handles, table lengths/positions,
null and nonzero fixups, return address and untouched words. Its native
checks verify the existing 32-bit serialized-word arithmetic, not native
dereferencing of reconstructed pointers; full 64-bit asset relocation is
outside this ownership-only change.

## Batch 78: typed touch-control tint guard

Move `TouchHacks::TintStack` construction and destruction into the existing
`TouchHacks.cpp` (`-O2`), whose neighboring methods immediately follow them
in the original. Remove the now-empty, default-`-O0` utility catch-all.
Ownership alone raises the constructor **45.846% to 56.077%** and destructor
**26.471% to 78.824%**. The remaining scalar SSE copies expose a type issue:
the saved value is a `NUCOLOUR3` record, not a float array reinterpreted as
that record at the renderer callback. Use the canonical color type and
ordinary record assignment, eliminating that cast. Both functions then
match **100%**, with all four constructor/destructor alias symbols intact.
No other scores change; whole fuzzy matching reaches **64.853880%**.

Per architecture, NDK x86 and full-global 64-bit ASan/UBSan pass **65,536
nested guard cases**, including **131,072 ordered restores**. Verify the
constructor preserves all global state, nested lifetimes restore the right
color, destructors preserve unrelated lighting fields and call the renderer
only after restoring the global ambient. Signed zero, infinities, NaNs and
subnormal bit patterns are preserved. The renderer is a recording callback;
no live rendering is claimed. Target/native builds and all five repository
checks pass; compiler options remain unchanged.

## Batch 79: cutscene scalar versus encoded-integer curves

The full retail `NuGCutCharAnimProcess_3` body rounds curves 6 and 7 as
floats without consulting their curve types; only layer-mask curve 11
uses `GetIntCurveVal`. The reconstruction incorrectly applied that helper
to all three, interpreting type-10 visibility/index float bits as integers.
Restore ordinary signed rounding for the first two curves, snapshot node
flags at the original point, and reload curve count after matrix callbacks
before deciding the rate/blend outputs. Keep the explicit returned-vector
assignment visible in the original scale path.

The first corrected spelling scores 0%; one bounded revision preserving
the original main/fallback branch arrangement and vector assignment raises
the function **15.3125% to 44.898438%**. An unrelated PVR loader in the same TU
also improves **26.419777% to 27.199627%** from changed register scheduling.
No regressions or exact-match changes; whole fuzzy matching reaches
**64.860146%**. Remaining frame slots and block layout are not pursued with
alignment, register or calling-convention annotations.

NDK x86 and full-global 64-bit ASan/UBSan pass **40,960 cases per
architecture** covering count boundaries, type-10 versus ordinary curves,
rounding ties/signs, all optional-output combinations, inactive characters,
signed start frames, rotation/translation/sign flips and post-callback count
changes. Matrix operations are recording mocks, not a full animation test.
The old body fails the same fixture's rate assertion when a matrix callback
changes the count; the corrected body passes. Unrelated retained vtable
dependencies in the native ASan fixture use fail-fast mocks. Target/native
builds and all five repository checks pass.

## Batch 80: rejected cutscene-animation grouping

Test grouping `GetIntCurveVal`, `NuGCutCharAnimProcess_3` and the legacy
`NuGCutCharAnimProcess` beside the existing cutscene locator evaluators in
`nugcutscene_anim.cpp`. The functions are contiguous in the original and the
destination is already `-O2`; no compiler options change. This does not
improve any of the three scores. It reduces two unrelated misc-TU scores
slightly, so revert the move and its temporary declaration cleanup. A fresh
linked report reproduces batch 79 exactly. Do not repeat this ownership
trial without new evidence about the source group.

As a diagnostic, both NDK x86 and full-global 64-bit ASan/UBSan pass the
grouped trial's **90,112 legacy-format cases**, **8,192 no-animation cases**,
and **81,920 direct/dispatched modern-format cases**. The real processing
bodies use recording curve/time/matrix callbacks. Checks cover signed legacy
counts and curve-type promotion, constant-versus-callback values, exact
callback order, optional outputs, both modern magic values, and matrix/state
preservation. This isolated test is not evidence of full cutscene playback,
nor a reason to retain a non-improving move.

## Batch 81: CameraCut action and script binding

Replace the 2,451-byte retail `Action_CameraCut` stub with the complete
parameter/target/camera dispatch. Correct its return type to `i32` (every
retail exit returns 1), expose canonical action/camera helper declarations,
and restore the `CameraCut` action-table binding. The reference table's
function pointer at file offset `0x61e5c8` points to `0x1a1510`, immediately
after the `CameraCut` name pointer, with flags 1. Preserve the existing
`-O3` owner and all compiler options.

Keep case-insensitive substring parsing and parameter precedence, immediate
all-zero reset dispatch, infinite end times, target locator/character/special
priority and failures, the packet owner's `objptr` for `myself`, and the
explicit-position sentinel. Current-camera failure goes directly to the
derived camera, bypassing explicit coordinates and the camera locator.
Delta coordinates affect only a successful current-camera lookup. Angle
inputs truncate to integers before conversion to engine units, including
the signed X-angle negation. Preserve target pointer identity for following.

`Action_CameraCut` improves **0.833333% to 82.432290%**. The unchanged dynamic
camera action improves **92.616130% to 92.808660%**; unchanged `Action_AddToSet`
declines **92.476190% to 92.396830%** after the table binding changes linked
layout. Whole fuzzy matching reaches **64.902640%**; no exact matches are
lost. Do not introduce stack-alignment or calling-convention annotations to
chase the remaining retail frame/layout differences.

NDK x86 and full-global 64-bit ASan/UBSan pass **32,817 cases per
architecture**, covering all target-source priorities, lookup outcomes,
packet/owner/self-reference cases, camera precedence/fallbacks, optional
flags, exact callback order/arguments, angle truncation, partial and NaN
coordinates, reset short-circuiting, inactive/nonpositive-count gates,
duplicates and overlapping parameter keys. String, lookup, camera and vector
callbacks are recording mocks; this is not a live camera/playback test.
Unrelated helpers retained by the source's locator-registration initializer
have fail-fast mocks. Target/native builds and all five repository checks
pass.

## Batch 82: speeder chase action, tuning data and script binding

Recover `Action_SpeederBeingChased` (`0x23da00`, 2,436 retail bytes) from its
empty action catchall into the existing `-O3` speeder-chase owner. Its retail
neighbors are the speeder init/reset/update/panel routines, target query,
`SpeedersDroppedBack`, and AT-AT helpers. Correct the return ABI to `i32` and
restore the verified `SpeederBeingChased` script binding: the original table
function pointer at file offset `0x61e808` points to `0x23da00`, with flags 0.
Check the linked rebuilt table as well as the source declaration.

Reconstruct the four seven-float `SPEEDERCHASEVALUES` rows and ten initialized
globals, including the original `players_going_forward = 1`. All 152 bytes of
the table and these globals match the reference data exactly. Give the
processor's existing action-data union a real integer latch member; do not
store the integer 1 as a fake pointer. Use canonical object, gamepad, rail,
character, and script types without changing their target layouts.

Preserve two-player reference selection, rail wrapping, speed/height seeking,
health loss, mode transitions, shooting/jump requests, and the kill-distance
exit. Snapshot the rail array and base speed but preserve callback-sensitive
player reloads and shoot-rate reads. Retail only resets mode bytes greater
than 4 although the tuning table has four rows: state 4 remains an invalid
input, not an invented fifth mode. Preserve the peculiar mode-1 timer branch,
including its apparently unreachable ordinary negative-timer jump subcase.

The initial reconstruction scores about 50%; restoring player reloads raises
the linked result **1.067194% to 52.583004%**. `SpeederChaseA_Reset` improves
**99.550000% to 99.633330%**, `SpeederChaseA_Update` improves **72.155240% to
72.162240%**, and unchanged `PodDust` declines **71.887500% to 71.852500%**.
Whole fuzzy matching before the later main rebase reaches **64.929210%**.
No exact matches are lost and no compiler settings change.

NDK x86 and full-global 64-bit ASan/UBSan pass **98,661 cases per architecture**:
initial/active states, valid mode pairs, full signed health-byte combinations,
one/two players, wrapped/NaN distances, timing, input requests, callback field
mutations and player replacement. Retail-invalid state 4 is excluded. Rail,
seek, random and kill services are recording mocks, not live speeder gameplay.

## Batch 83: analog-stick corner stretching and original source ownership

Recover `UCStretchToCorners` (`0x270374`, 497 bytes) and move its render-core
stub to `nupad_android.cpp`, which retains default `-O0`. Retail places it
between `ScaleAndClamp` and `NuPadReadPS` in the run ending with
`_GLOBAL__sub_I_nupad_android.c`. Use the canonical `NuFabs`/`NuFsqrt` helpers,
signed 16-bit coordinate arguments, exact float constants, original arithmetic
ordering, truncation and horizontal-before-vertical stores. Both inputs must
be read before either write so aliased arguments behave as in retail.

The linked score improves **3.111111% to 99.896290%**, with no other changed
scores. Whole fuzzy matching before rebase reaches **64.939400%**. NDK x86
and full-global 64-bit ASan/UBSan each pass **2,172,688 coordinate/alias cases**,
including every signed coordinate against boundary inputs, all aliased inputs,
deterministic pairs, square-root arguments and adjacent-byte preservation.
The sqrt fixture uses real `sqrtf`; this is not controller-hardware testing.

## Batch 84: customiser piece availability and bounded block-layout trials

Recover the 266-byte `Customiser_PieceAvailable` at `0x1b91c0`, correcting the
empty `void` placeholder to `i32` in its unchanged `-O2` owner. Promote the
signed collection category at `CUSTOMPIECE + 0x11` into a canonical `i8` field
with a target offset assertion; the record remains `0x28` bytes. Expose the
canonical piece-query and `Collection_GotAnyOfType(i32, u32)` declarations.

Preserve the demo-only availability-bit check, completion gates, character
lookup and collection gates, model mask `0x0c`, and signed category filtering.
A character absent from the collection list returns success immediately,
bypassing the later restrictions. Reload character/category fields after
callbacks; nonzero helper responses are truth values, not necessarily 1.
The parent `Customiser_Configure` remains a stub, so this batch does not claim
to integrate or complete the whole customiser menu.

The initial early-return spelling scores 0% linked despite passing behavior
tests. Three bounded source trials retain the same flags: nesting the normal
path scores 3.75% in the object; a precomputed-result form scores 91.181%;
an explicit demo rejection branch with a shared success return scores
99.236% in the object. Retain only the last, which improves the linked result
**5.833334% to 99.791664%**. It has the original 266-byte size. Do not repeat
the rejected forms or add branch/register/optimization hints. No collateral
scores change from this final revision.

NDK x86 and full-global 64-bit ASan/UBSan each pass **780,304 cases**, covering
all availability words, all signed character/category values, demo modes,
model masks, callback return combinations/order, callback mutations, the
unknown-character bypass and unchanged structure bytes. Collection/completion
services are recording mocks. Final target/native builds and all five
repository checks pass.

## Main rebase after batch 83

Rebase all 47 branch commits onto `f1f9d789` (push-block/spinner interaction
restoration, PR #112), preserving and restoring the uncommitted batches above.
Retain main's pushing, push-block, object-field and initialization changes.
The overlapping obstacle query keeps this branch's implementation after
rechecking the complete retail disassembly and main's new caller: the object
argument selects animated versus stored positions, and a null system must
not write the optional distance. Both declarations have the same pointer ABI.
The query remains **60.983540%**, not main's lower-scoring reconstruction.
The earlier note that the surrounding push caller is missing is now obsolete.

Recompile the resolved nearest-query fixture: **134,173 cases** pass on NDK
x86 and full-global 64-bit ASan/UBSan. Unrelated dispatch-table paths retained
by ASan have fail-fast mocks. Regenerate the report rather than trusting
conflicted generated percentages. Combined main integration and batches
82–84 raise fuzzy matching **64.902640% to 64.958600%**: nine functions improve,
two decline, and no exact matches change. The declines are main's imported
`PushCode` reconstruction (**6.72% to 0%**) and the **0.035-point** `PodDust`
collateral change above. Do not replace the newly restored push behavior
with a stub to recover its accidental score. No new agents are started.

## Batch 85: customiser configuration and complete canonical allocation

Recover the 3,117-byte `Customiser_Configure` at `0x4a1cc0` and its local
category lookup in the existing `-O2` owner. Correct the public return from
`void` to `CUSTOMISER *`, and the default availability callback from `bool`
to `i32`. The latter retains its 100% score. Retail returns null for two
invalid character IDs, an existing singleton, parser creation failure, or no
pieces; it does not return the existing singleton.

Fix the structural prerequisites rather than allocating from the old
recovered prefix: retail clears `0xd18` bytes, not `0xc50`. Complete
`CUSTOMISER` with locator indices, X/Y offsets and the trailing state range,
using target size/offset assertions. Recover the initialized piece ID fields
and the original nine-row `CustomSetData` table, including its nonzero byte
tags and shared-scene names. Replace the unrelated padded `GAME_CUSTOMISER_s`
view with `CUSTOMISER *` throughout the singleton's consumers. The old view
would read character IDs from a fixed byte prefix on pointer-wide hosts.

Preserve parser command order, the 440-piece limit, all flag/character/variant
keywords, optional extension/weapon callbacks, signed narrowing, locator and
layer bounds, and stable grouping of piece records into nine categories.
Empty-token lines terminate parsing. Unknown lines/categories are skipped.
Names are copied before publishing the arena cursor. Allocate typed runtime
records with their actual alignment/size on native builds; the Android
allocation retains four-byte alignment and the original strides. An absent
save gets a four-byte-rounded serialized allocation; unrelated save bytes
remain untouched. Repeated default flags overwrite a side's selection until
both sides have been found, after which that category's scan stops.

The first source form improves **1.033113% to 35.715233%**. One bounded trial
marks the private category lookup ordinary `inline`, allowing GCC to perform
partial inlining as in retail. Retain it: configuration reaches **44.303310%**,
and the naturally generated `.part.0` helper improves **0% to 61.880950%**.
Do not hand-author the clone or its private register ABI. The compiler still
chooses a different split point and frame layout; no attributes or compiler
flags are added. `Customiser_FindPieceByName` improves **96.594200% to 100%**.
The final linked unit raises fuzzy matching **64.958600% to 64.989160%**,
with eight improved functions, no regressions and no exact matches lost.

NDK x86 and full-global 64-bit ASan/UBSan each pass **198,474 cases** covering
all 440-piece boundaries, all arena alignments, both save-allocation modes,
optional callbacks, full default-flag combinations, keyword parsing, signed
IDs/variants, locator/layer bounds, singleton/failure exits, category data,
pointer identity, full runtime/save memory images and sentinel preservation.
Parser/string/character services are fixture implementations; the real
configuration/default-copy bodies run. Final target/native builds and all
five repository checks pass. The caller that wires this configuration into
game startup is still missing, so this is not full customiser integration.

## Batch 86: terrain cube impact

Recover the 2,128-byte `CubeImpact` at `0x3855d0`. Replace the no-argument
stub with the verified C-linkage signature: current and previous matrices,
normal, scale and output point. Move the body from the terrain stub collector
beside its original terrain neighbors, preserving both owners' `-O3` mode.
The original constant table supplies the eight signed unit-cube corners.

Transform every corner first through the previous matrix and then in-place
through the current matrix, with homogeneous input W zero. Select the first
strictly closest corner below 10,000 whose current normal projection exceeds
its previous projection; default to corner zero if none qualifies. Preserve
strict ties, unordered comparisons, callback-visible normal reloads, and the
final scale-plus-current-translation XYZ writes. Do not cache state across
transform callbacks or add speculative null handling.

The first ordinary C++ body improves **1.037037% to 99.555560%**. GCC naturally
unrolls the loop and realigns the stack. The remaining frame/temporary-slot
differences do not justify alignment or register hints. Overall fuzzy matching
rises **64.989160% to 65.033560%**, with no other scores or exact matches changed.

NDK x86 and full-global 64-bit ASan/UBSan each pass **173,032 cases** covering
all selected corners, threshold neighbors, strict ties, infinities/NaNs,
positive/negative/zero scales, random matrices, shared matrix pointers,
output-normal aliasing, all 16 callback identities/order, callback mutations
and unchanged surrounding bytes. The transform callback is a recording math
implementation; the real impact body runs. Target/native builds and all five
repository checks pass. No reconstructed caller currently invokes this helper,
so no end-to-end terrain or visual validation is claimed.

## Batch 87: touch renderers and the circle conversion prerequisite

Resolve the earlier shared-helper deferral before restoring the 573-byte
`NuTouchInputStick::Render` and 247-byte `NuTouchInputButton::Render` in their
unchanged `-O3` owner. Recover the original signed `white`/`grey` globals and
their `0x32ffffff`/`0x32646464` initializers. Keep the verified, surprising
circle arguments: colour 128 and the selected packed colour converted to
floating progress. Do not swap them to produce a more plausible picture.
Preserve the four directional arrows, strict +/-0.2 thresholds, button-mask
angle selection, pre-callback colour snapshots and later coordinate/state
reloads. Canonicalize the circle/arrow/aspect declarations in shared headers.

Retail `RndrUnfilledCircle` multiplies progress by 360 and uses `CVTTSS2SI`;
unordered/out-of-range results become negative integer-indefinite, skipping
the segment loop after its two initial vertices. Express that result with
an explicit float range check before conversion, identically on target and
native builds. This is needed for the recovered callers' ordinary initialized
values, not only hypothetical malformed input. No intrinsics, assembly,
calling-convention attributes or optimization overrides enter production code.

The first renderer bodies improve **3.750000% to 99.928570%** and
**7.500000% to 100%**, respectively. The portable conversion guard reduces
the circle helper **86.672810% to 75.059906%**; retain this intentional
tradeoff rather than reintroducing undefined conversion or host-only behavior.
Overall fuzzy matching rises **65.033560% to 65.047600%**; these are the only
three changed functions, with one exact match gained and none lost.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow fixtures pass:

- **1,000,016 conversion values** compared with an SSE hardware oracle,
  including signed bounds, infinities, NaNs and random float bit patterns;
- **157,168 renderer cases** checking actual virtual dispatch, all 16-bit
  button masks, threshold neighbors, signed colours, nonfinite geometry,
  callback mutations/order and unchanged object state;
- **19,040 circle primitive cases** checking geometry, loop/truncation edges,
  float/half UVs, colour adjustment and callback-visible renderer globals;
- **4,096 integration cases** running the actual touch, circle and arrow
  bodies together with the initialized colours, verifying primitive counts
  and vertex-buffer bounds.

Primitive submission/rotation and trigonometric services are fixture
implementations; no GPU or visual validation is claimed. Four unused timing
globals retained by full ASan registration have fixture storage. Final target
and native builds and all five repository checks pass. Default touch-layout
creation is still a stub, so this does not complete the touch input system.

## Batch 88: legacy display-list bounds updater

Recover the 1,536-byte `xxxNuDisplayListUpdateSpecial` at `0x2ea380` in
its existing `supportall.cpp` owner, preserving `-O2` and its C++ signature.
Capture the original scene/display pointers before the draw-matrix callback,
copy the returned matrix, transform all eight corners in retail order, then
perform the seven minimum reductions followed by seven maximum reductions.
Reload the bounds format, destination and instance index at their original
callback boundaries. The center/extent path deliberately retains retail's
`sqrt(extent.x + extent.y + extent.z)`, not a presumed Euclidean radius.

The min/max path in retail copies the uninitialized W lanes of its local
vectors. Do not reproduce that undefined read. Use typed XYZ vectors and
leave the destination W lanes unchanged, matching the active
`NuDisplayListUpdateSpecial` writer's min/max behavior. The reconstructed
clipping/distance consumers use XYZ only. The center/extent path still writes
the computed radius to the first W lane and leaves the second untouched.
No fabricated padding values, alignment attributes, optimization changes or
manual unrolling are used to improve the score.

The first ordinary C++ body reaches **49.395%** in the isolated object and
**49.547300%** when linked, up from **1.418919%**. Overall fuzzy matching rises
**65.047600% to 65.063240%**. This is the only changed score in the unit;
there are no regressions or exact-match transitions. Remaining differences
include the original's realigned frame, 16-byte temporary strides and
unrolled calls; ownership/alignment guesses are not justified by this audit.

NDK x86 and full-global 64-bit ASan/UBSan each pass **100,000 cases** covering
all eight corners, min/max ordering, both formats, every callback mutation
boundary, scene/display snapshots, live destination/index/flag reloads,
matrix snapshots and mutations, finite/nonfinite geometry, strict ties,
signed zero, random matrices and unchanged padding/surrounding records.
Transform/min/max/square-root services are recording fixture implementations.
Target/native builds and all five repository checks pass. No reconstructed
caller invokes this legacy entry point, so no visual integration is claimed.

## Batch 89: typed customiser touch geometry

Recover the six XYZ touch positions, six widths and six heights at target
offsets `0xc98`, `0xce0` and `0xcf8`, plus the six request bytes at `0xd10`.
Keep the existing named request members as aliases and assert the target
offsets and `0xd18` structure size. Replace raw Android byte offsets in the
menu touch-release and pause-button consumers with these canonical members.
This repairs native pointer-width-dependent offsets without altering target
code generation: every linked function score remains unchanged at an overall
**65.063240%**. Preserve the existing six unrolled hit tests, strict rectangle
edges, first-hit priority and horizontal-before-vertical gesture order.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**59,814 cases** using the actual menu constructor, virtual release dispatch
and pause-button body. Coverage includes all overlap masks, all six slots,
swipe thresholds, captured/unowned/disabled touches, negative/zero/nonfinite
geometry, all 256 prior quit-byte values and complete customiser/canary images.
Target/native builds and all five repository checks pass. UI/system services
are fixture implementations; no visual validation is claimed.

The audited 3,471-byte `CustomiserMenu_Draw` candidate is deferred. Its first
ordinary C++ body scores 0% because the still-stubbed update producer never
writes the private name-edit X/Y state; GCC correctly folds those arrays away.
Keep the candidate outside the repository and reconstruct update/draw as a
group before retrying. Do not make the arrays volatile, export them or change
compiler flags to defeat this legitimate optimization. The update audit also
finds a backwards name-space trim without a proven lower bound; resolve that
contract before implementing it. The typed touch layout is a separately
validated prerequisite, not a claim that either menu body is now recovered.

## Batch 90: emitter orientation

Recover the 967-byte `DebrisEmitterOrientation` at `0x34f640` in its existing
`-O3` owner. Retail calls identity initialization, reloads `debkeydata`, then
performs local Z/Y/X rotations over all four matrix rows before clearing the
orientation cache word. It does not call the exported rotation helpers or
clear the translation row afterward. Express the scalar operations with the
canonical matrix members and signed 16-bit lookup angles; preserve homogeneous
lanes and the post-callback global reload. No flags or attributes change.

The first body reaches **70.034%** in the isolated object and improves the
linked score **5.372549% to 70.156860%**. Overall fuzzy matching rises
**65.063240% to 65.076510%**; this is the only changed score, with no regressions
or exact-match transitions. Remaining differences are argument/register
allocation and instruction scheduling, not a reason for ABI hints.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**272,144 callback-fixture cases** and **272,144 integration cases using the
real `NuMtxSetIdentity`**. Coverage exhausts each signed 16-bit angle axis,
mixed triples, all tested record slots, the disabled handle, arbitrary LUT
values, signed zeros, infinities/NaNs, translation rows, dirty-cache clearing
and surrounding bytes. The callback fixture additionally changes the identity
matrix, rebinds the global record array and mutates the lookup table. NaN
payloads are not compared; other float results are checked bitwise. Target and
native builds and all five repository checks pass. No particle-system or
visual integration is claimed.

## Batch 91: vertical timing-bar rendering

Audit all 2,320 bytes of `NuTimeBarSetRender` at `0x2f8d60` in its unchanged
`-O3` owner. Restore the original created-set contract, GPU-frame gate and
unsigned timing conversion. Preserve the rectangle's subtract-after-add
rounding, callback-visible screen-size reloads, initial discarded baseline
query, three bottom-up decimal digits and conditional fourth digit, 9,999
clamp, unconditional final height query and alternating label offsets.
Keep the captured set pointer across scene callbacks, but reload the active
peak buffers, labels, font and material at their original boundaries.
Use the existing real slot-reset operation. Remove speculative null/index/
GPU-slot-count guards absent from retail; callers must supply created sets
when rendering is active. Compiler options and repository checks are unchanged.

The first ordinary C++ candidate improves **5.229656% to 59.467%** in its
isolated object and **60.000000%** when linked. The unchanged destructor's
layout also improves **99.153850% to 99.318680%**. Overall fuzzy matching
rises **65.076510% to 65.103430%**, with two improved scores, no regressions
and no exact-match transitions. Remaining digit-loop layout/register choices
do not justify manual instruction shaping.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**96,365 vertical state/trace cases** and the existing **334,600 horizontal
regression cases**. Vertical coverage includes every valid set, signed slot
counts, all initialization/reset/GPU/engine flag combinations, full-width
unsigned timing values, digit thresholds, fractional font metrics, nonfinite
GPU-frame rates and mutations at 100 callback positions. Tests verify exact
render/font events, cached versus reloaded state, global set/buffer rebinding,
font/material changes, real slot resets and complete resulting arrays.
The fixture records a label cleared by a callback without dereferencing it;
it does not claim that the real variadic font renderer accepts null labels.
Graphics/font services are mocks, and screen/font conversions are tested
within their valid integer ranges. Target/native builds and all five
repository checks pass; no GPU or visual integration is claimed.

## Batch 92: gizmo progress loop structure

Audit the complete 317-byte `GizmoSysStoreProgress` at `0x4af9e0`. The
existing behavior is correct, but separate hand-written negative/nonnegative
loops obscure the common count gate and compiler-generated loop split.
Express one logical iteration over the captured type/set arrays, retaining
the live registry count, initial clear callback, null gates and per-iteration
function/buffer/data loads. Keep the source's existing `-O3` mode and ABI.

Two bounded source forms were tested. A nullable argument with one call
reaches **20.505%** in isolation; explicit null/data call branches reach
**24.588%** and are retained. GCC performs the invariant loop split itself.
The linked score improves **5.443299% to 24.793814%**, raising overall fuzzy
matching **65.103430% to 65.104720%**, with no other score changes. Do not
chase the remaining cold-block/register placement with compiler hints.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**91,504 dispatch/callback cases**. Coverage includes all eight-slot callback
masks, null/mixed/full progress buffers, negative indices including `INT_MIN`,
all four fixture progress slots, signed counts, null system/registry gates,
clear/store ordering and callback mutations of registry/count/type/set/buffer
state. Complete traces and resulting state are compared. Clear/store services
are fixtures; unrelated parser-table dependencies retained by ASan use
fail-fast mocks. Target/native builds and all five repository checks pass.

## Batch 93: pursuit timer and traffic comparison corrections

Audit all three pursuit A/B/C update bodies. Retail doubles `FRAMETIME`
before adding the arrow timer; `(timer + frame) + frame` is not equivalent
for floating-point rounding, overflow or nonfinite values. Use
`(frame + frame) + timer` in all three handlers, preserving the existing
NaN-retaining upper clamp, pause/fade gates and remainder callback order.
The C traffic branch selects side +1 whenever the strict greater-than test
is false, including unordered values. Express that predicate directly rather
than substituting `<=`, which incorrectly selects side -1 for NaNs.

Four source-line corrections, with no ownership/ABI/optimization changes,
improve linked scores:

- A: **83.777780% to 84.333336%**;
- B: **60.984375% to 64.031250%**;
- C: **4.802084% to 20.864584%**.

Overall fuzzy matching rises **65.104720% to 65.106340%**; no other scores
change and no exact matches are lost. The separate original pursuit TU and
private arrow-helper clone remain ownership work, not an invitation to add
calling-convention or optimization attributes.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**324,880 timer/traffic cases**. Tests cover every gate combination, threshold
neighbors, signed zero, infinities/NaNs, 10,000 random float-bit pairs, every
signed side/direction byte, negative/zero/one/94/95 entry counts, null player/
traffic, callback rebinding and complete traffic memory images. The remainder
service records its inputs and suppresses arrow submission; no new validation
of the unchanged arrow renderer is claimed. Six unrelated character-ID globals
have fixture storage for retained ASan tables. Target/native builds and all
five repository checks pass; no gameplay or visual run is claimed.

Two later isolated shared-arrow-helper trials were rejected without changing
the retained sources. Moving the duplicated gates/timer into an ordinary
static `UpdateZamArrow` raises C to 43.177% but drops A/B to 4.349%/21.688%;
GCC emits the complete helper out of line instead of retail's partial split.
An ordinary `inline` trial emits the entire payload in each caller and drops
C to 0%. Neither recovers the original `.isra.5.part.6` boundary. Do not repeat
this regrouping without new ownership/splitting evidence, and do not force
the private ABI or clone name. These were compiler-only trials, not new
behavior or aggregate-report validations.

## Batch 94: gizmo loader registry gates

Audit the full 589-byte `LoadGizmoSys` at `0x4b0080`. The initial gate tests
the global registry pointer, not its count. Restore that null check, so a
missing registry is safe and a present empty registry still opens and scans
the file. Restore the separate pointer check after file callbacks, which
can clear or replace the registry before post-load dispatch. Retain the
captured type/set arrays, live count across post-load callbacks, original
flag transitions, error-log clearing and payload-skip/load ordering. No
source ownership, ABI or optimization settings change.

The first ordinary source correction reaches **54.852%** in isolation and
improves the linked score **7.857988% to 55.266273%**. Overall fuzzy matching
rises **65.106340% to 65.112240%**, with no other score changes or exact-match
losses. Remaining stack reloads and instruction scheduling are not pursued.

NDK x86 and full-global 64-bit ASan/UBSan/float-cast-overflow each pass
**146,560 state/trace cases**. Coverage includes null system/registry/log
gates, signed registry counts and resolver IDs, all callback masks and flag
bytes, failed opens, successful/rejected payload callbacks, every valid
name-record length through 32 bytes, path lengths through 251 bytes,
payload sizes 0–8 and nineteen callback-mutation scenarios. Complete event
traces, consumed input and resulting registry/set/system/log state agree
with the independent control-flow oracle. ASan retains normal global
instrumentation. The old source reproduces two null dereferences (at entry
and after close), while an empty-registry case fails the expected trace.

File services, name resolution and load/post-load callbacks are fixtures;
this is not a disk/parser or gameplay integration claim. Names include a
terminator within the fixed 32-byte buffer, paths fit the original buffer,
and negative payload lengths or callback invalidation of an actively used
registry remain outside the tested contract. Target/native builds and all
five repository checks pass.

Further read-only triage confirms that the 146-byte `NuVisiInstTree` needs
the shared instance-tree layout and its private `ClipInstTree` clone, not
an isolated wrapper implementation or forced register-passing attributes.
No source experiment was made for that visibility group.

## Batch 95: episode-selection menus

After rebasing onto main `05429366`, the linked baseline is **65.124480%**.
Restore the private `DrawEpisodesMenu` and public episode update/draw callbacks
in `hub.cpp`, next to their initializer and their original private
`EpisodeNumerals` table. This closes the previously documented ownership split;
the existing `-O3` configuration is unchanged. Remove the HUD/menu placeholders,
restore the six-byte signed `i_clip` array and initialized
`hub_goto_clipsmenu_episode`, and publish the shared clip-grid declaration.

Reference control flow distinguishes the six-episode grid, seven-column clip
selection, play confirmation and exit animation. Preserve physical-pad versus
touch input, confirm-before-cancel priority, directional repetition and wrapping,
unavailable-episode opacity, item hitboxes, and the exit state's two frame-time
increments. Keep the ordinary static C++ helper: GCC supplies its private
register convention without calling-convention attributes. Branch source order
is material: the first grid-before-episode trial produced zero fuzzy matching
for the helper; recovering the original episode-first control-flow order raises
it to **69.594666%**. Update/draw reach **32.404540% / 85.771240%**.

Overall linked fuzzy matching reaches **65.240974%** (+0.116494 percentage
points), with **6,262** exact functions unchanged. The Android target, all five
repository checks, cross-file declaration check and symbol-surface check pass.
This is assembly-reviewed reconstruction, not a gameplay integration claim;
no controller/touch runtime harness has yet been run for this batch.

## Batch 96: word-based font justification and glyph submission

Recover `NuQFntPrintJustifiedRSW` from its complete reference control flow.
The old line-by-line approximation treated encoded hyphens as forced line
breaks, dropped them, and used the wrong width/stretch decisions. Restore
word-segment accumulation, punctuation-adjacent spacing, collapsed spaces,
single-word stretching, final-line scale limiting and the mandatory initial
line (including empty strings). Keep saved spacing/gap restoration and matrix
mode forwarding. It improves **0% to 22.646488%** at the unchanged `-O2`.

Also recover glyph-width caching across Unicode callbacks, signed digit-glyph
indices, left-associated advance arithmetic, and stream advancement from the
captured vertex pointer. Keep half-colour calculation outside the glyph loop
and refresh second 2D UVs after primitive submission. These correctness changes
do not yet improve the zero-scoring `NuQFntPrintCharW`; its remaining stack,
register and block-layout gaps are not forced with attributes or flags.

Overall linked fuzzy matching rises **65.240974% to 65.249770%**, with no other
function-score changes or exact-match loss. Extracted production bodies pass
**10,009** wrapping boundary/property cases and **960** vertex/state cases on
32-bit native and 64-bit ASan/UBSan. Wrapping checks preserve collapsed input
across random word/hyphen/punctuation sequences, line positions, height and
spacing restoration. Vertex checks cover 2D/3D flags, mono-width digits, spaces,
texture/no-texture dimensions, half UVs, overbrightening, scales, complete
coordinates/colours and cursor/count/coordinate-stack state. Rendering/font
services are fixtures, not real graphics or a retail execution oracle; callback
mutation and invalid/out-of-capacity text are outside these tests. LeakSanitizer
is disabled because sandbox tracing prevents it from running; ASan/UBSan remain
enabled with ordinary global instrumentation.

Three bounded force-progress loop trials remain at zero matching despite
recovering reference-style signed indices, a separate 128-entry exit and
ordinary nested group loops. All are reverted. The progress layout is already
correct; do not repeat these control-flow-only trials without new evidence.

## Batch 97: customizer menu and minikit animation

Replace the empty `CustomiserMenu_Update` and `CustomiserMenu_Draw` bodies in
their existing `customise.cpp` owner, retaining the effective `-O2`. Recover
the private category/name cursors and 38-character alphabet from the original
symbols/data, and give the two reserved menu-packet byte arrays semantic
aliases without changing their layout. Restore player swapping, physical and
touch input, repeat/conflict handling, piece randomisation and availability,
name-edit backups/commit/cancel/filtering, two-player demo exit agreement,
save-change detection and the auto-save completion handoff.

Restore name/glyph rendering, preview icons, idle pulses, touch hitboxes and
drop-in opacity. Constants are read from the original literal pool, not
approximated. The update improves **0.423654% to 46.128860%**; its two-player
setup is unrolled as in the reference. The draw improves **0.784314% to
19.261438%**. One fixed-width name-trimming expansion lowers the draw score
and is reverted; keep the bounded loop. The blank-name trim in the updater
checks the lower bound before reading, unlike the original's underflowing
all-space loop, and reaches its intended localized default-name path.

Also correct `DrawStatusMiniKit`: only current/new pieces receive the sliding
oscillation, whereas completed pieces take the built-scale branch directly.
Reload piece-array state after callbacks as in the reference. This function
still scores zero; no compiler or calling-convention workaround is introduced.

Overall linked fuzzy matching rises **65.249770% to 65.315160%**, or
**+0.190680 percentage points** from this branch's main baseline. Exact
functions remain **6,262**. There are tiny linked-layout score variations in
the camera/material-clip functions; do not attribute those to behavior fixes.
Extracted production bodies pass fixed customizer input/save/drawing fixtures
and minikit completed/new-piece, bounds and callback-array replacement cases
on native i386 and 64-bit ASan/UBSan. Tests include name cancellation, bad-word
rollback, all-space localized fallback, side swapping, directional conflicts,
demo agreement, save success/deferred completion and preview hitboxes.
Rendering, audio, pad, save and random services are fixtures, not a gameplay
integration test or a retail runtime oracle. LeakSanitizer is disabled for
the sandbox; normal address/undefined-behavior instrumentation remains on.

Read-only matcher triage finds another reason not to chase zero scores blindly:
the linked x86 GOT recovery in the installed objdiff fork conservatively skips
entire functions containing indirect branches (including switch tables).
Such functions retain raw GOT offsets while straightforward functions receive
symbolic GOT normalization. This is a deliberate control-flow proof boundary,
not evidence that their C++ should be distorted. The matcher, metric and
baseline are unchanged in this batch.

## Batch 98: private space-flight drawing closure

The reference's contiguous Chris level-function cluster contains
`DrawStarFighter`, `DrawSpaceLevel` and all three level draw callbacks, together
with the existing reset/spline/radial callbacks and `DogDebKey`. Recover this
private closure in `chris.cpp` at its unchanged effective `-O2`: move the fighter
renderer out of `render.cpp`, remove the empty HUD space-draw placeholder and
move its three direct draw callers from `chris_stubs.cpp`/`episodeIII.cpp`.
Keep exact local symbol spelling and ordinary static linkage. The restored
callers make synthetic retention annotations unnecessary; removing those
annotations lets GCC infer private register passing. No register-passing
attribute, optimization override or manual assembly is used.

Add asserted runtime views for the eight five-fighter groups, their markers,
96 queued fighters, two cross controls and quick-bolt records. Preserve the
old reset views and allocation size: their addresses cover suffixes of fighter
records, not the matrix origins used by drawing. Recover `Jetpos`'s verified
12-byte initialized data and the original private quick-bolt model tables.
Restore active/group gates, marker transforms, queue/bolt rendering, twin
exhaust creation/update/free and player reloads after service calls.

`DrawSpaceLevel` improves **0.429338% to 53.974957%**, and `DrawStarFighter`
improves **84.398880% to 85.747190%**. The linked report reaches
**65.379440%**, or **+0.254960 percentage points** from main. The three draw
wrappers become exact, increasing exact functions **6,262 to 6,265**. The
stack matrices retain the reference's 16-byte alignment; this does not change
the score, and remaining instruction-layout differences are deferred.

Extracted production drawing bodies pass native i386 and 64-bit ASan/UBSan
fixtures covering all 40 grouped fighters, inactive-group gates, the final
queued slot, both marker transforms, all four bolt model pairs, zero-duration
bolts, both players' twin exhausts, cleanup, null-player preservation and a
callback that clears a player between exhaust updates. The special missile
draw checks its scaling/debris branch. Graphics/debris/model services are
fixtures; this is not real rendering or flight-gameplay integration. Ordinary
global sanitizer instrumentation is enabled; LeakSanitizer remains disabled
for sandbox compatibility.

## Batch 99: space-flight formation, update and reset closure

Continue the original `chris` symbol neighborhood with the nearly empty
`MakeWingFormation`, `StarFighterAlign`, `ProcessStarFighter` and
`ProcessSpaceLevel`. Move their original callers into the same existing
`-O2` owner. The original `ChrisDogFightAUpdate` explicitly calls
`ProcessSpaceLevel` at `0x23b118`; the former reconstruction omitted that
call. Without this second caller, GCC inlines the updater into
`ChrisAnakinAUpdate` and its original local symbol disappears. Restoring
the caller retains the ordinary private function and makes the Anakin
wrapper exact, without a calling-convention or retention attribute.

Recover the whole group beginning at level offset `0xa0`, including its
leader matrix; the drawing-only suffix previously began at `0xd0`.
The five fighters still begin at `0xe0`, and all drawing addresses are
unchanged. Add verified pointer-bearing starfighter fields, spline records
and action-pair views. Preserve the target's `0x128` fighter stride,
`0x658` group stride and `0x63ef4` allocation with assertions; host arrays
and allocations follow their native pointer-bearing types.

The private alignment calls pass distance/duration in XMM0 and a separate
integer mode on the stack. Ghidra misidentifies those stack words as float
parameters. Check the call instructions directly before reconstructing
formation modes, death banking and spline-spawn alignment. Recover literal
values from the original pool instead of interpreting decompiler bitcasts.
The reference's aligned stack vectors use the existing aligned vector and
matrix types; effective build options are unchanged.

Restore formation-slot selection, five-ship initialization, death/escort
paths, spline lookahead, targeting/interception, circular bolt allocation,
missile spawning, action timing, player hits, spawn/repeat schedules and
checkpoint doors. Integrate reset with these canonical fields: the old
queued view cleared unrelated vector words, copied the repeat count in the
wrong direction, and omitted draw-scale/checkpoint/rumble constants. Reset
now clears the actual active and parent/spline fields, also on 64-bit hosts.

The linked report reaches **65.492920%**, or **+0.368440 percentage points**
from main. `MakeWingFormation` reaches **64.132576%**, `StarFighterAlign`
**84.962500%**, `ProcessStarFighter` **37.052630%**, `ProcessSpaceLevel`
**26.112532%**, and `ResetSpaceLevel` **54.866325%**. Exact functions increase
**6,265 to 6,266**. Drawing scores are unchanged. The roughly two-point PR
target remains unfinished; instruction-layout differences are deferred.

Extracted production code passes 64-bit ASan/UBSan and optimized native
i386 fixtures for all eight formation slots, full-slot refusal, alignment
distance/mode gates, death and parent following, spline completion and
audio pitch, last-slot target acquisition, bolt wraparound/full/NaN slots,
action rewind/wait/formation commands, callback-sensitive player hits,
seven spawn-model classes, repeat counters, checkpoint selection and all
group/queued reset fields. The prior drawing fixtures also pass on both
ABIs. Math services are compiled separately to preserve the production
translation-unit boundary; matrix pre-rotation services record calls,
rather than validate actual rotation rendering. These are focused fixtures,
not full flight-gameplay integration. LeakSanitizer remains disabled for
sandbox compatibility.

## Batch 100: swept space-bolt collisions and coin history

Restore the remaining private collision closure in the evidenced `chris`
owner, with unchanged compiler options. `ChrisExtraBoltCollision` checks
the level-allocation flag, space-state pointer and bolt flags, then checks
five fighters in each of eight enabled groups and all 96 queued fighters.
Stop on the first hit. Move the empty private `CollideBoltStarFighter` out
of `bolts.cpp`; its real caller now retains it without `__used__` or an
explicit calling-convention attribute.

Recover relative-velocity swept-sphere intersection over `[-FRAMETIME, 0]`,
including stationary, tangent and ordered-float rejection paths. Restore
escort exceptions, debris callbacks, projectile deactivation, pickup/heart
effects, owner credit, hit audio and the fighter hit counter. The allocator's
final 4 KiB is a 256-entry coin-history table keyed by spline ID and spawn
time; assert the `0x10` record stride and `0x62ef4` table offset. Move
`ShipDropCoins` from its byte-offset implementation in `collection.cpp`
into the original owner and use the shared pointer-bearing fields.

Two guards cover invalid states without altering valid retail paths:
formation ships have no spline identity, so skip coin-history insertion
instead of dereferencing null; reject a corrupted history count above 256
before scanning its fixed array. History duplicate/cap handling remains
unchanged for valid counts. A bounded-loop first draft scored zero for this
small function; placing the capacity guard before the ordinary counted scan
retains safety and raises it to **70.711860%**. Do not remove the guards to
chase reference crashes.

`CollideBoltStarFighter` reaches **73.729256%**, and
`ChrisExtraBoltCollision` improves **0.597826% to 58.501358%**. Overall
matching reaches **65.546680%**, or **+0.422200 percentage points** from main,
with **6,266** exact functions unchanged. The approximately two-point PR
goal is still in progress.

Production-body fixtures pass 64-bit ASan/UBSan and optimized native i386:
coin-history duplicate/new identities, slot 255 and full/corrupt capacities,
null spline, stationary/swept/future/past/tangent/miss/NaN intersections,
relative ship velocity, both escort IDs, pickup amounts and owner credit,
audio selection, allocation/flag gates, every grouped slot, inactive groups,
the final queued slot and first-hit termination. Services are fixtures, not
full flight-gameplay integration; LeakSanitizer is disabled for the sandbox.

## Batch 101: clip-viewer panels and free-play menu lifecycle

Restore the cutscene-viewer branch of `Hub_DrawAreaStats`, including episode
and clip titles, the signed chapter sentinel, and selection-transition fades.
Mode 18 does not require a valid area index. Door panels compare the configured
door's index, rather than pointer identity. Recover the reference's `0.05`
vertical icon offsets and exact `0.20100002` horizontal constant; preserve
live area-flag reloads after callbacks. Keep the normal-panel invalid-index
guard. The function improves **1.486945% to 9.691906%**.

Restore `Hub_DrawFreePlaySelect` and its private cursor closure together with
`Hub_UpdateFreePlaySelect`. The old updater treated selection state as a roster
index and launched directly, so simply restoring drawing left its private
opacity/timer state disconnected. Recover entry, selection, return, team
assembly and launch states, two-player confirmation/cancellation, touch's
9999 confirmation sentinel, directional repeats and grid navigation, network
rosters, challenge/arcade launch state, easing, name prompts, touch hitboxes,
radar pulses and the final two-row roster. Share the existing icon-wibble
state through its owning header; add the missing network roster storage in
`netplay`, with its verified 49-entry allocation. Compiler options and public
signatures are unchanged; private callers retain natural compiler specialization.

Preserve the reference's exact-zero versus negative transition-timer behavior
and its pre-update confirmation count. Add narrow malformed-data guards:
bound a roster while leaving its terminator, cap a network roster at its
verified allocation, avoid division by zero or endless duplicate-only grid
navigation, ignore invalid touch indices, and avoid challenge initialization
through a null next-level pointer. These guards do not change valid data paths.

The renderer improves **11.027806% to 35.422245%**. The complete updater's
indirect switch table crosses the linked matcher's GOT-proof boundary and
currently reports **0%**, down from the old partial body's **9.455767%**.
Do not change compiler options or the scoring contract to conceal that limit.
Overall matching still reaches **65.572440%** (**+0.447960 percentage points**
from main), with **6,266** exact functions. The approximately two-point goal
remains unfinished.

Extracted production bodies pass optimized i386 and 64-bit ASan/UBSan fixtures:
all clip/title/fade combinations, index-based doors, chapters and completion
variants; free-play transition geometry, prompts, locked models, radar timing,
roster capacity, touch/controller priority, opposing directions, exact-zero
timers, confirmations/cancellations, network filtering, launch and malformed
grid boundaries. Rendering/audio/network services are stand-ins, not gameplay
integration. LeakSanitizer is disabled for the sandbox; the menu fixture also
disables vptr checks because its radar service stand-in has no engine object.

## Batch 102: free-play roster producer and selector consumer

Restore the missing resident, capability-category, Imperial-access, vehicle,
minikit and extra-model portions of `MakeFreePlayModelList`. Preserve duplicate
checks, the early `PlayerList` writes, the 48-model limit and the reference's
resident count even when an entry duplicates another model. Recover category
hat requirements and retry, customiser exclusions, unlocked Imperial selection,
live area-flag reloads and ordered extra-model dependencies. Keep signatures
and compiler options unchanged; share the canonical `Move_DEFAULT` declaration.

Initialize an empty roster's terminator, avoid forming `ADataList[-1]`, and bound
the vehicle scratch list. With the complete producer restored, remove the hub's
temporary collection fallback, which is absent from the reference. Bound its
resident/bonus scan by the actual model count so duplicate resident counts
cannot make it read beyond the roster. Preserve its filtering and shuffle.

The producer improves **9.693764% to 43.912025%**; the selector helper reaches
**8.046808%**. Overall matching reaches **65.598720%**, or **+0.474240 percentage
points** from main, with **6,266** exact functions. The approximately two-point
goal remains in progress.

Extracted producer and consumer bodies pass optimized i386 and 64-bit
ASan/UBSan fixtures: explicit/fallback players, duplicate writes, residents,
arcade suppression, category/hat retry, Imperial access and exclusions,
minikits/vehicles, chained extra models, empty input, capacity and selector
filtering. Services remain stand-ins, not full gameplay integration;
LeakSanitizer is disabled for sandbox compatibility.

## Batch 103: AI loader stream alignment and legacy versions

Remove the version-20-only gate from `AISysLoadEx` after recovering the
reference's section and field gates. Path connection indices change from bytes
to shorts to integers; version 1 has its extra byte; old nodes use the default
height tolerance and lack route masks/special-route IDs. Gate areas, locators,
locator sets, antinodes and the game-specific loader at their actual versions.

Fix creature stream alignment for shipped assets too: always read its type name,
including an empty/default script name, and read spawn counts and path state in
older versions. Recover legacy respawn/stagger/range defaults. Read vectors as
three float fields as in the reference, and pass the actual caller's buffer end
to script loading instead of the temporary pak cursor. Keep the locator-entry
allocation multiplier despite its unusual appearance: the reference confirms
it. Public signatures, ABI declarations and compiler options are unchanged.

The function improves **13.837194% to 26.910553%**. A bounded single-function
parser/shared-scratch reconstruction passed fixtures but reported zero matching
and was not retained; keep the corrected sectioned parser and document this
source-structure debt rather than changing flags or calling conventions.
Overall matching reaches **65.616190%**, **+0.491710 percentage points** from
main, with **6,266** exact functions. The approximately two-point goal continues.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized i386:
versions 1 through 20, empty/nonempty paths, all connection-index widths, node
heights, route-mask and special-route gates, default/explicit scripts, creature
fields and callbacks, nonempty areas/locators/sets, references, activation areas,
antinodes and special handles, route character masks, pak success/fallback and
missing files. Services are stand-ins, not full asset/gameplay integration;
LeakSanitizer is disabled for sandbox compatibility.

## Batch 104: terrain query handles and platform reach

Restore the explicit position/movement/radius arguments of `NewScanHandelFull`
and `NewScanHandelSubset`, instead of serializing an unrelated global query.
Recover cell/group bounds, skin allocation, masks, platform motion expansion,
rotating-platform radial/vertex tests and subset wall filtering. Preserve the
full handle's override state and copy wall pairs into its arena so subsequent
queries do not overwrite them. Bound allocation; retain pointer-width headers
for native consumers. Remove the unused serialization helper.

Recover `ScanTerrainPlatform`'s broad-phase formulas, including the non-mode-one
squared movement reach and exact 0.05 expansion. Keep existing transformation
helpers and compiler options: a single-function reconstruction scored worse.
The subset improves **2.659130% to 16.001740%** and platform scan **5.650155%
to 12.652219%**. The restored full body reports zero; its helper/source layout
remains unresolved. Overall matching is **65.624590%**, **+0.500110 percentage
points** from main, with **6,266** exact functions.

Extracted bodies pass optimized i386 and 64-bit ASan/UBSan fixtures: explicit
query arguments, static/platform/rotating geometry, motion expansion, masks,
visibility, subsets, triangle normals, vertical scaling, wall-copy lifetime,
invalid inputs and repeated arena allocation. Services are stand-ins, not
full gameplay integration; LeakSanitizer is disabled for the sandbox.

## Batch 105: pod-race update, panel and mixed-font text

Replace the incomplete `PodRaceUpdate` body with countdown/startup sounds,
elapsed-time failure and adaptive lap allowance, host/client synchronization,
spline-driven racers, Sebulba's mine throws, transform/position publication,
and one-shot level resets. Recover the 20-byte network packet, attempt counter
and pod displacement/spline views. Keep private `RacePodAlign` and its natural
caller closure; no calling-convention or compiler-option changes. Initialize
the close-range mine velocity: the reference leaves it indeterminate.

Restore the pod panel's actual lap object drawing, numeric pulse scaling and
start-countdown colour. Remove the null-text draw and erroneous early return.
`Text3DEx` now resets empty-result metrics, handles separate button-font runs,
nonuniform button scaling, inline colour presets/reset, half-intensity RGB and
follow-on colour state. Recover preset tables from the binary; bound invalid
preset escapes rather than indexing outside the tables.

`PodRaceUpdate` improves **4.656635% to 10.575923%** and `PodRacePanel` reaches
**81.398735%**. Text's restored body scores **6.461642%**, below its incomplete
**11.325707%** baseline; its source/compiler layout remains debt. The combined
unit improves overall matching to **65.642480%**, **+0.518000 percentage points**
from main. The approximately two-point goal continues.

Extracted bodies pass optimized i386 and 64-bit ASan/UBSan fixtures for race
timers, fading, network state, spline advancement/end motion, transforms,
close/medium/far mine throws and resets; panel object/numeric rendering;
text/button segmentation, three scale modes, presets, half colours, alignment,
follow-on state, empty/hidden input and long/multibyte boundaries. Rendering,
math and engine services are stand-ins, not full gameplay integration.
LeakSanitizer is disabled for sandbox compatibility.

## Batch 106: indexed lighting and HUD timer states

Recover the complete `rtlApplySetScaleLoop` producer/consumer closure: indexed
or dynamic traversal, identity masks, inner/outer falloff, modifiers, ambient
and directional priorities, anti-lights, cached shadows and specular wobble.
Use the canonical pointer-width index representation in `IndexLights` and
recover version-two through version-four conversion/defaults and UID/chain
initialization in `rtlLoadSet`. Bound full scans and fall back to them when a
cell would exceed the original signed-char count. Packed 32-bit asset decoding
on 64-bit hosts remains separate preexisting debt; typed fixtures do not prove
real asset loading there.

Restore `UpdateStats`' missing minikit, red-brick, True Jedi, gold-brick and
power-up states. Preserve ordered menu queries, challenge and Super Story
differences, network red-brick gates, per-player coin aggregation and one-shot
awards. Share the existing canonical message declaration. Public signatures,
compiler options and calling conventions are unchanged.

The lighting scan improves **9.961600% to 43.038400%**; the HUD update improves
**9.256757% to 9.948648%**. Loader/index helper scores regress with the restored
versions and safe producer/consumer representation, but the combined unit
raises overall matching to **65.671646%**, **+0.547166 percentage points** from
main, with **6,270** exact functions. The approximately two-point goal continues.

Extracted production-body fixtures pass optimized i386 and 64-bit ASan/UBSan:
lighting masks, falloff, shadows, wobble, dynamic/indexed sets, full cells,
legacy versions and UID wrap; HUD menu/fade/pause gates, timers, challenge,
Super Story, True Jedi completion/awards, hub gold bricks and power-ups.
Services are stand-ins, not full gameplay or packed-asset integration;
LeakSanitizer is disabled for sandbox compatibility.

## Batch 107: customizer previews and status/save drawing

Recover `Customiser_Update`'s preview angles, head/arms/legs animation,
hat-height interpolation, active/other-player selection, animation availability,
touch-arrow rendering/hitboxes and texture gates. Correct its reversed use of
the intro-state and draw-delay fields and restore the 30-frame animation step.
Move the private `UpdateCustomPieceAnim` into its actual caller's translation
unit; both source and destination use the effective `-O2` command. Recover the
binary's random normalization constant. No forced calling convention is used.

Recover `Customiser_Draw3D`'s GC-data owner, cape preview override, hidden hats,
hat translation, hand-held weapons and lightsaber layers. Its matrix uses the
existing aligned type because the original frame is verified 16-byte aligned.
Preserve the reference's touch-hitbox publication order. Fix setup so a replacing
torso suppresses head model flags but still inherits head gameplay flags.

Restore `DrawStatusScreen`'s demo options and empty-status countdown wobble;
restore `DrawGameState`'s current-game percentage, entry colours, message-box
mode and two-line empty/no-space text. Keep public signatures and compiler
options unchanged; bound save-slot indexing and missing resources.

Customizer update improves **20.133759% to 28.945860%**, drawing **13.484848%
to 20.353535%** and setup **15.048014% to 15.230132%**. Status drawing improves
**10.994845% to 21.864262%** and save drawing **4.385246% to 23.893442%**.
Overall matching reaches **65.693980%**, **+0.569500 percentage points** from
main, with **6,270** exact functions. The approximately two-point goal continues.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized i386
(SSE arithmetic for the custom-piece random endpoint): intro availability and
expiry, draw delay, preview-side selection, touch edges/idle pulse, hat offsets,
texture changes, cape/hats/weapons/lightsabers, inherited flags; status callbacks,
fade/demo gates, countdown and stage alpha; current/used/empty/no-space saves,
menu colours and invalid slots. Engine/rendering services are stand-ins, not
full gameplay integration; LeakSanitizer is disabled for sandbox compatibility.

## Batch 108: spinner outputs, aimed detonators and menu input routing

Recover `GizSpinners_Update`'s animation visibility, failure debris, disabled
input release, timed output pauses and output-marker return motion. Name the
pause float at the verified 0x2d4 offset without changing the record layout.
Bound arms and outputs to their canonical arrays. Cap the arm count before
traversal: putting a constant bound in the loop caused GCC to expand eight
matrix-copy bodies (5,696 bytes); the capped runtime traversal avoids that
duplication with unchanged `-O3`. The retail owner-release path incorrectly
indexes Player by spinner index; release actual matching owners instead of
copying the unrelated/OOB access. Preserve the binary's animation rate field,
pause timer source and rotation constants.

Restore `ThermalDetonator_Throw`'s hint updates, throw locator, Jango randomized
orientation, normalized joint basis and obstruction ray. Recover managed-target
arc aiming, target release and the reference's X-only clamp, fixed vertical
velocity and blocked-origin correction. Restore radius-derived part dimensions,
the collision callback, position pointer and active marker. Remove the extra
movement-flag clear absent from the reference. Missing resources and locator
indices are guarded; no compile-option or calling-convention changes.

Restore `UpdateGameMenu`'s controller ownership, pause/network pad selection,
credits player state, 15-entry button history, startup input delay, fade/editor/
level-transition gates, loader-preserved widescreen setting, navigation sounds,
resume and area-reset behavior. Its ID queries belong with the text/menu-entry
helpers evidenced by reference symbol neighbors; move them to `text.cpp`,
preserving that file's existing `-O2` and the input updater's existing `-O3`.
Both queries remain exact. The fuller updater currently scores **0%**, down
from **10.994350%**; retain the recovered behavior and record the unresolved
control-flow/register-layout mismatch, not a claimed per-function gain.

Spinner matching improves **16.920895% to 27.322388%**; throwing improves
**4.619512% to 46.248780%**. Overall matching is **65.714170%**, **+0.589690
percentage points** from main, with **6,270** exact functions. Target remains
approximately **67.124480%**. All eleven checks passed for the preceding commit.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized i386
SSE builds: throw defaults, hints, managed target lifetime and X clamps, valid/
missing locators, normalized and Jango transforms, blocked throws, allocation
failure and part callbacks; spinner output entry/expiry, owners in slot seven,
visibility/debris, positive/negative return motion, exact marker snap and array
bounds; combined/single/pause/network/credits controllers, startup threshold,
loader transitions, editor/fade gates, sound/resume, history and invalid input.
Service calls are mocks, not a full game-runtime test; LeakSanitizer is disabled.

## Batch 109: area configuration and AI kill selectors

Restore `Area_Configure`'s mission roster shuffle and mission target, streamed
level options, supercounter blocks, music propagation and story/freeplay coin
and challenge settings. Preserve the reference's lone `story_only` token
behavior and initial `AreaMusic` value. Correct `AreaMusic` from 16-bit to
32-bit: its reference BSS symbol is four bytes and its stores are 32-bit.
Publish counters through the character arena with target-compatible alignment
and capacity checks; bound model, level, pickup and counter arrays. Initialize
counter runtime fields rather than copying uninitialized scratch bytes.

Correct `Action_Kill`'s AI-controlled flag, selection precedence (all AI before
creature set before area), area-system ownership and packet-owner object
indirection. Explicitly selected objects do not need the group traversal's
in-use/character flags. Preserve dead checks, debris/parts and respawn modes.
Separate the group traversals and debris call branches to recover natural
compiler specialization, without changing optimization or calling convention.

Area configuration improves **17.381910% to 17.859297%** and AI kill improves
**22.329342% to 26.444110%**. Overall matching reaches **65.718450%**,
**+0.593970 percentage points** from main, with **6,270** exact functions.
All eleven GitHub checks passed for the preceding commit; the approximately
two-point goal continues.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized i386
SSE builds: missing-file defaults, model deduplication, mission shuffle, music
flags/tracks, AI messages, coin/time settings, streaming options and duplicates,
counter locations/colours/capacity/arena exhaustion; AI selection precedence,
actual area ownership, packet indirection, opponents, exclusion/dead checks,
high area-mask bits, parts/debris and respawn flags. Services are stand-ins,
not a packed-asset or full game-runtime integration test; LeakSanitizer is
disabled for sandbox compatibility.

## Batch 110: versioned particle effects and callback-sensitive weapons

Restore `FileLoadSingleEffectType`'s legacy formats instead of accepting only
versions 34 through 41 while its caller accepts versions 5 through 41. Recover
short-based frequency/timing conversion, discarded legacy fields, emitter
defaults, float versus byte colour keys, collision/torus gates, old sound
tables and versioned trail/radial fields. Preserve untouched runtime fields
and use canonical members after pointer-bearing data, not target byte offsets
on 64-bit hosts. Consume excess sound records without overflowing the four
stored slots; clamp negative counts. The first draft passed the count read
directly into the side-effect-unsafe `MAX` macro; the zero-count fixture caught
the double read, and the final code reads once before clamping.

Recover `DrawWeapons`'s character-data reloads after cheat/Anakin services and
between hands. Preserve the explicit-weapon branch decision instead of testing
possibly replaced character data after subsequent callbacks. Restore ordered
float distinctions in `CodeMenu`'s slide/repeat gates and `PartCollide`'s age,
XZ overlap and shield checks. Snapshot the particle player mask per object,
while allowing callback changes to affect subsequent objects. No compiler
options, public signatures, attributes or matching normalization are changed.

Effect loading improves **25.140778% to 58.571846%**; weapon drawing improves
**0% to 45.720722%**. Code-menu and particle-collision matching remain **0%**;
retain the verified behavior corrections without claiming a per-function gain.
Overall matching reaches **65.795960%**, **+0.671480 percentage points** from
main, with **6,270** exact functions. All eleven GitHub checks passed for the
preceding area/AI commit. The approximately two-point goal continues.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized i386
SSE builds: **1,720** effect-record cases spanning every version 0 through 42,
signed/zero frequencies, colour widths, defaults, negative/zero/oversized sound
counts, special names and untouched runtime fields; weapon families, cheats,
four hands, hilt scaling, trails, two-sided blades, reflections and callbacks
replacing character data; shop navigation, cancellation, unlock deduplication,
network input and NaN slides; collision ages/XZ bounds/shields including NaN,
2D versus 3D, pickups/torpedoes, deflection, impulses and callback mask changes.
Services are stand-ins, not rendering/gameplay or packed-asset integration;
LeakSanitizer is disabled for sandbox compatibility.

## Batch 111: fixed-roster selectors and canonical grapple fields

Recover `AvailableToPlayer`'s selector dispatch and eight explicit player slots
from the reference. Keep the predicate in locally scoped macros, following the
existing fixed-slot `UpdateExplosions` convention; do not add optimizer flags
or forced-inlining attributes. Separate-loop and private-helper experiments
did not recover the reference's specialized flag/weapon/context branches.
The final dispatch improves matching **9.245283% to 42.385933%** without
changing results or cheat-service calls. The free-play roster path remains
unchanged.

Correct `Grapple_MoveCode`'s canonical movement-facing angle (offset `0x5a`,
not the distinct facing angle at `0x58`), byte-sized `use_action` check and
half-body-height rope limit. Matching improves **18.462273% to 19.468004%**.
A complete grapple movement candidate recovered automatic entry, hanging,
jump ascent and swing-cycle damping and passed isolated fixtures, but scored
0% or near zero under several ordinary source structures. It is deferred in
temporary files rather than committed as a matching gain. Substantial
grapple behavior is therefore still incomplete in the committed function.

Recover `GizPanel_Update`'s fixed player probes, retained tracking state across
untracked panels, radius-squared threshold and yaw-relative pitch clamp.
Its matching falls **4.762963% to 0.789630%** despite the verified corrections;
do not claim a per-function improvement. Raw instructions confirm one
`0.245f` vertical offset for every model-2 probe, including player zero; a
provisional double-offset interpretation was rejected before committing.

The net batch raises overall matching **65.795960% to 65.824610%**,
**+0.700130 percentage points** from the rebased main baseline, with **6,270**
exact functions. The approximately two-point goal is not complete. All eleven
GitHub checks passed for the preceding effects/weapon commit.

Extracted committed production bodies pass 64-bit ASan/UBSan and optimized
i386 SSE fixtures: **100,000** availability/oracle cases, identical cheat
call counts and explicit coverage of all eight slots; **2,401** panel angle
combinations, every player probe, retained state, timers, invalid players and
NaN distances; grapple field separation, nonzero action-frame bytes and rope
limits. External services are stand-ins, not full gameplay integration.

## Batch 112: distance selectors, callbacks and fixed collision slots

Recover the eight explicit player slots in both distance selectors and debris
collision handling, and the ten explicit detonator slots. Use local macros
with unchanged public signatures, owners and optimization modes. Preserve
callback reloads, first-player ties and ordered floating comparisons: later
NaN distances must not replace an existing selection, and NaN protection
timers must not admit a player to debris damage. Detonator instruction checks
confirm the ordinary ordered thresholds, including NaN owner charge.

Recover `SetMoveAndAnimateFunctions`'s outer mask/callback dispatch instead of
testing every optional condition within one generic loop. A zero mask disables
its filter even with a nonzero requested value; absent callbacks leave their
fields untouched. Do not access game-character data when both its mask and
movement-type filter are disabled. Preserve signed byte movement types.

Raw reference instructions for `FindFurthestPlayerFromVec` write to absolute
address zero rather than its output argument; no dynamic text relocation
repairs those stores. Do not reproduce that unsafe reference defect. Retain
the valid output-parameter behavior already provided by the reconstruction.

Matching improves: nearest **10.457627% to 63.364407%**, furthest
**9.867392% to 49.756523%**, callback assignment **3.943870% to 35.453472%**,
detonators **8.665255% to 82.582630%**, and debris/player collisions
**14.922028% to 81.072130%**. Overall reaches **65.954880%**, **+0.830400
percentage points** from main, with **6,270** exact functions. All eleven
GitHub checks passed for batch 111. The two-point goal remains unfinished.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **100,000** distance/oracle queries, ties, NaNs, all eight slots and
callback replacements; **100,000** callback-selector cases covering masks,
signed types, all callback combinations, counts and absent optional data;
all ten detonator slots, timing thresholds, projection, NaNs and callbacks;
all eight debris slots, eligibility guards, ordered timer tests, both damage
checks and callback ordering. Services are stand-ins, not engine integration.

## Batch 113: hub transitions, range checks, climbing and music

Recover the hub door helper's ten area checks, eight player path updates and
eight table lookups, including short circuits and callback reloads. The
reference uses the separate current `player` global, not `Player[0]`; reload
it between area services. Matching improves **15.849056% to 42.490566%**.
Recover the active-range query's eight probes and shared successful return:
**20.891891% to 60.124325%**. Recover the climb helper's eight explicit angles
and shared result stores: **18.038252% to 71.448090%**. Raw instructions
confirm sign extension of the terrain service's low byte; no calling-
convention attributes are added to approximate its private optimized ABI.

Recover six fixed music track slots with outer stop/fade dispatch, retaining
voice state checks, service order and fade flags: **15.105140% to 59.266354%**.
Recover eight ordered dodge-hint checks, retaining the early model-flag veto
and both animation alternatives: **16.115625% to 52.243750%**.

Retain two narrow behavior corrections even though their individual matching
does not improve: reload the tagging source after callbacks and only read
optional character data when needed (`CheckForPlayersTurnedOff`,
**5.473088% to 0%**); use ordered six-axis explosion bounds and the canonical
game-character pointer (`UpdateExplosion_Generic`, **9.010430% to 8.408084%**).
Explicit tagging fanout and explosion ring expansion scored zero and are
deferred; do not continue source-layout trials without new evidence.

The net batch reaches **66.018790%**, **+0.894310 percentage points** from
main, with **6,270** exact functions. All eleven GitHub checks passed for
batch 112. The approximately two-point goal remains unfinished. Owners,
compiler options, public signatures and scoring normalization are unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **90** hub area/table combinations plus cleanup and callback
reloads; all player range/tag slots, thresholds, NaNs and lazy optional data;
all climb stop positions and misses, node selection and byte terrain results;
**50,000** music stop/fade oracle cases with identical service sequences;
all dodge slots, both animation alternatives and short circuits; explosion
NaNs on every bounding axis, contact boundaries, canonical flags and particle
call counts. External services are stand-ins, not full engine integration.

## Batch 114: snake caller closures and hub/projectile behavior

Recover the shared snake-debris caller closures with a locally scoped macro,
preserving segment order, matrix scaling, optional specials, particle fields
and lighting. `SnakeBeenHit` improves **8.266394% to 76.032780%** and
`BlowUpSnakeBody` **17.456375% to 87.570470%**. Keep the macro local to the
two callers rather than adding forced-inline or ABI attributes. The adjacent
`EatVictim` changes **100% to 99.531250%** after translation-unit codegen
shifts; its behavior and source are unchanged.

Recover all five hub selection states from the decoded reference jump table,
including single-vehicle launch, trailer launch, pack eligibility, lost-temple
rejection, two-player direction/confirmation priority and touch inputs.
Preserve ordered timer comparisons: NaN launches states 1/2 but does not
complete states 3/4. Retain the invalid-area safety guard instead of the
reference's unsafe area-array access. Use the canonical game save and the
reference-inferred integer selector widths. `Hub_UpdateSelectMode` improves
**14.719807% to 49.115944%**.

Replace the `GizObstacles_BoltHit` stub with active-obstacle eligibility,
ordered six-axis bounds, reverse sphere probes and nearest-hit selection.
The initial distance limit is **1,000,000,000**, verified from this function's
literal address; its local `.LC4` label must not be confused with another
translation unit's 1.0 literal. Preserve bolt-type/cheat service calls,
signed owner index, deflection, attacker rumble and targeting callbacks.
Matching reaches **20.900000%** from the stub.

Overall matching reaches **66.065210%**, **+0.940730 percentage points**
from the rebased main baseline, with **6,269** exact functions. All eleven
GitHub checks passed for batch 113. The approximately two-point goal remains
unfinished. Owners, optimization modes, public ABI and score normalization
are unchanged. The font renderer already has its vertex helpers inlined;
defer further closure experiments there without new structural evidence.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: snake counts 0–11, both scales, special filtering and all particle
fields; all hub states, fade/render gates, thresholds/NaNs, choice limits,
eligibility, vehicle counts and controller/touch priority; obstacle reverse
probe ordering, nearest ties, all bounds' NaNs, flags, signed owners,
deflection/rumble service sequences and callback changes to active count.
External services are stand-ins, not full engine integration.

## Batch 115: level paths, thermal throws, minikits and boulder AI

Restore Dooku's four path-connection flag transitions and six coordinate
arrangements, with live node/path-system reloads after callbacks. Retain
boss-kill/outro guards and force effects. `DookuC_Update` improves
**10.920000% to 23.516363%**.

Recover `ThermalDetonator_ThrowMom` as a particle-creation operation rather
than a caller-velocity calculation. Preserve the supplied velocity, default
position, locator normalization, Jango random rotations, ignored ray result,
particle fields, sound and rumble. Matching improves **6.336283% to
61.176990%**. The ordinary throw implementation is unchanged.

Restore both Hoth minikit counters, pickup guards, completion camera angles,
gizmo activation and messages. Reload the gizmo after the camera callback.
`IncrementMinikitCounter` improves **2.641510% to 40.735847%**. Deliberately
use the valid averaged-player position for every message: the retail
completion branch passes an uninitialized local, which is not reproduced.

Replace `Action_BoulderSection`'s stub with parameter parsing, category
short circuits, ordered nearest selection, toggle timing, weapon retention,
look-target assignment and player-path following. Raw assembly verifies the
integer return channel; repair the stub's incorrect void return without
changing its mangled symbol or argument ABI. Parameter parsing uses the
packet's embedded script processor for named values and the separate
processor for the movement argument, matching the actual pointer identities.
The reference symbol table confirms `boulder_part` is only two pointers
(eight bytes), although the action also probes padding after it. Preserve
the recovered two-entry global and omit that out-of-bounds third probe;
also guard missing follow-player data. Matching improves **2.297872% to
57.314890%**.

Overall matching reaches **66.098850%**, **+0.974370 percentage points**
from the rebased main baseline, with **6,269** exact functions. All eleven
GitHub checks passed for batch 114. The approximately two-point goal remains
unfinished. Source owners, optimization modes and scoring normalization are
unchanged; the only signature correction is the reference-proven action
return type described above.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: all six Dooku coordinate arrangements bit-for-bit, traversal flags,
missing data and callback replacements; 72 thermal locator/Jango/allocation
combinations, matrix normalization, particle fields and unchanged input;
6,144 minikit count/flag cases, camera parameters and completion messaging;
3,136 boulder nearest/category combinations, ties, processor identities,
ordered timer/NaN cases and absent follow-player data. External services are
stand-ins, not full engine integration.

## Batch 116: terrain bounds, scan order and caller closures

Recover the shared bound, wall-list and scaled-shape closures inside their
retail scan callers using local macros, without forced-inline attributes.
Preserve the bound expressions' separate padding/radius operations and their
caller-specific addition order. Recover the sweep-diagonal `NuFsqrt` call,
including the wall-only caller where its result is unused, and the main
scan's object-scale radius adjustment and rotating-platform range rejection.
Retain guards for disabled groups and absent rotating matrices.

Recover static terrain, pickup, then platform group ordering. Raw reference
stores and the `TerrainPlatformMoveCheck`/`PlatformChecks` consumers confirm
the saved `scan_list` points to the start of the platform section, not the
start of all scan records. Store that boundary after fixed/pickup groups and
before platform traversal. Clear `TerrOverRideScan` even when the one-shot
`IgnoreWallSplines` flag suppresses wall collection.

`ScanTerrain` improves **12.396799% to 24.492273%**; wall-only scanning
**5.813115% to 22.331148%**; `NewScanHandelFull` **0% to 11.876629%**.
The unchanged `HitTerrain` shifts **78.555950% to 79.972030%** from
translation-unit codegen. Overall matching reaches **66.137150%**,
**+1.012670 percentage points** from main, with **6,269** exact functions.
All eleven GitHub checks passed for batch 115. The approximately two-point
goal remains unfinished; owners, compiler options, ABI and score
normalization are unchanged.

The large shared platform closure alone regressed net matching. Expanding
its triangle normal loop also scored zero in both main scan entry points;
both experiments were removed. Do not repeat them without new evidence.
Sanitizer tests caught a macro-local radius name collision before commit;
the corrected macro uses a distinct local and evaluates its outputs once.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **4,116** bitwise bound/radius cases; scaled triangle/quad parity,
zero normals and NaN scale; static/pickup/platform ordering and the saved
platform boundary; override suppression cleanup, disabled platform groups
and absent rotating matrices; both scan modes and four masks with 160 wall
segments capped safely at 64 records. Existing handle spatial filtering,
subsets, copied walls, platforms and arena-boundary tests also pass. External
services are stand-ins, not full engine integration.

## Batch 117: particle envelopes and opponent-selection behavior

Recover the reference's explicit eight-stage rotation and size envelopes,
including post-stage termination checks and the live `edui_last_item` reload
after slider formatting. Rotation improves **34.047092% to 63.155%** and
size **22.137032% to 34.256%**. Color and torus expansion trials regressed
and were removed. A separate NDK experiment confirms the reference's
unsigned-byte-to-float intermediate, but the color trial still scored zero;
do not repeat it without resolving the surrounding structural difference.

Restore `Action_SetOpponent`'s droid roster filtering and persistent cycling,
named and last-attacker selections, nearest-enemy clear, and type callbacks
plus first matching object lookup. Preserve the reference's slot-seven
unordered-timer behavior, candidate accumulation across parameters and
parameter precedence. Remove the extra packet-opponent write: the reference
only stores the game object's opponent. Retain guards for absent character
data and cap the original ten-entry array when malformed scripts repeat the
droid parameter. Matching improves **16.536873% to 60.67%**.

Overall matching reaches **66.151820%**, **+1.027340 percentage points**
from main, with **6,269** exact functions. All eleven GitHub checks passed
for batch 116. The two-point goal remains unfinished. Owners, optimization,
ABI and scoring are unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **1,944** envelope numeric/termination/reload combinations, equal
and reversed ranges, NaNs, infinities, post-callback termination changes,
menu allocation failure, widget arguments and callback-driven last-item
replacement; **344** opponent roster/type combinations, round-robin state,
signed-byte type translation, script priority, nearest clear visibility,
missing callbacks, repeated-parameter capacity and packet preservation.
External services are stand-ins, not full engine integration.

## Batch 118: retained post-filter draw and parameter closures

Restore quad/grid draws through the existing private full vertex-binding
helpers, including attribute enable/disable masks, pointers, buffer state
and indexed grid drawing. The public vertex-format setter remains unchanged.
Recover the seven fixed motion samples and thirteen Gaussian taps, retaining
the reference's float operations and normalization. Reuse the private
register-search/constant-setter closure at motion, accumulation, blur,
speed and depth-of-field call sites; preserve first-register-match behavior
and callback-driven program reloads. Do not enable the retained filter graph
or change Android no-op/null-return entry points.

Motion improves **21.480713% to 75.827896%**, accumulation
**23.276102% to 65.953600%**, the three copy overloads to **85.535090%**,
**85.895164%** and **87.417270%**, blend to **85.535090%**, blur 5x5 to
**83.940200%**, speed to **69.088980%**, depth-of-field to **81.749%** and
Gaussian offsets **17.247313% to 81.634410%**. Overall matching reaches
**66.251830%**, **+1.127350 percentage points** from main, with **6,269**
exact functions. All eleven GitHub checks passed for batch 117. The
approximately two-point goal remains unfinished. Ownership, compiler
options, ABI and scoring normalization are unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **8,192** quad/grid transitions over six active attribute bits;
**864** motion numeric/proxy combinations including NaNs, infinities and
program replacement during a setter; **3,840** accumulation frame/weight
combinations; **512** Gaussian dimension/scale combinations; **112** shader
register/first-match/setter cases. These use service stand-ins, not a real
GL context or full engine integration. Invalid accumulation frame counts
outside 1..256 remain deferred rather than introducing unrelated policy.

An `Action_Kill` expansion trial produced 11,114 bytes instead of the
reference's 4,428 and scored zero. It was fully removed, including its
behavior changes. Do not repeat the sixteen specialized-loop expansion
without new compiler/structural evidence.

## Batch 119: fixed customizer previews, music tracks and emulator conversions

Recover the customizer's two explicit character previews and fixed layer /
piece categories, including head suppression by replacing torsos, gameplay
flag merging and cape layer exclusion. Preserve the existing null, count
and selection-index guards: absent/empty torso, cape and weapon tables are
not dereferenced merely because the original assumes configured arrays.
Matching improves **15.230132% to 23.089403%**.

Restore six fixed pause and resume track closures, retaining track order,
voice/status filtering and pause-bit writes after the stream callbacks.
Pause improves **27.575580% to 73.744190%**, resume
**27.343023% to 73.831400%**. Recover the network emulator's reference-proven
unsigned conversion closure: signed casts of bounded high/low sixteen-bit
components, multiply by 65,536, then add. This is equivalent over the full
`u32` domain and follows the existing profiler conversion audit, not a
compiler-option workaround. Retain send/queue/statistics ordering and the
post-statistics floating-point ratio test. `NuNetEmu::Update` improves
**6.453039% to 77.375694%**.

Overall matching reaches **66.280270%**, **+1.155790 percentage points**
from main, with **6,269** exact functions. All eleven GitHub checks passed
for batch 118. The approximately two-point goal remains unfinished.
Ownership, optimization, ABI and score normalization remain unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **20,000** customizer state/guard comparisons; **50,000** six-track
pause/resume state and service-order cases; **1,000,008** unsigned conversion
comparisons and **20,000** emulator queue/timing/statistics cases, including
counter replacement by callbacks, unsigned timestamp wrap and linked-list
removal. Emulator deadline conversions are tested within the existing
defined numeric range; zero bandwidth / out-of-range float-to-int policy
remains deferred. Services are stand-ins, not full gameplay/audio/network
integration.

Three eight-slot push-hint expansions scored zero and were removed.
Clipping shared-edge capture and per-corner arithmetic-order trials also
regressed and were fully removed. Structured emulator flow alone emitted
the same bytes; its gain comes from the evidenced conversion closure.
Hardware bootstrap remains deferred pending its variadic cursor and
cross-owner initialization prototype audit. The incomplete batarang
targeting caller requires recovering its natural private target-search
helper, not directly exposing the compiler's `.isra.0` calling convention.

## Batch 120: gameplay timing and canonical super-story timer

Restore the omitted level-name fade, pause, pickup flicker, join-in,
gameplay, overall gameplay, bonus, super-story, mission and challenge
timing paths. Preserve service order, the 36,000-second super-story cap,
mission expiry's network-client gate, and the reference's unordered
challenge-time comparison. Capture the double-score step before the menu
callback. Retain null-world and malformed mission/index guards.

Replace the provisional four-float `SuperStoryTimer` with the canonical
16-byte `TIMER`; update all four consumers, removing the raw timer cast.
The exported global name, storage size and section remain unchanged.
`GameTiming` improves **29.68% to 87.301%**. Overall matching reaches
**66.296400%**, **+1.171920 percentage points** from main, with **6,269**
exact functions. All eleven GitHub checks passed for batch 119; the
approximately two-point goal remains unfinished.

The extracted production body passes **16,384** state, expiry and
service-order comparisons under 64-bit ASan/UBSan and optimized i386 SSE.
Fixtures cover pause/screenshot/cutscene gates, missing level/area/save
data, two-player availability, super-story cap crossings, network-client
mission gating, NaN challenge limits and optional game-time output.
Services are stand-ins, not full gameplay integration.

NewCast full candidate, roof-only and alternative structured-selection
trials all regressed and were fully removed; unresolved callback-visible
selection updates and ceiling policy remain deferred. The full SmartTextEx
wrapping/measurement/box trial emitted a natural GCC `.part.1` clone and
regressed; it and its forward declarations were fully removed. Do not
force inlining or compiler attributes to hide the discrepancy.
FindNearestBreak punctuation/capture and Force-gizmo guard/bounds trials
also regressed and were fully removed. No ownership, optimization, ABI or
scoring changes are used.

## Batch 121: portal debug visibility and cutscene reset subsystems

Restore the four camera rays, scaled world-space corners, eight alternating
white/red debug lines, camera unlock/relock and subsequent room traversal.
Capture the near clip before the room-query callback. Recover the reference's
exported `nuvec_one` / `nuvec_minus_one` three-float initialized objects;
the frustum builder receives these canonical objects rather than local copies.
`NuPortalVisibility` improves **28.79558% to 85.572%**.

Restore cutscene chaining/callback clearing, rigid special visibility and
matrix resets, locator reset callbacks and VFX handles, trigger first-frame
state evaluation, character reset and camera-lock reset. Promote the locator
entry's target `+0x08` reset callback to its proven four-argument function
pointer type. Describe the external trigger owner's known state pointer and
four-byte records with canonical provisional structures; use these members
in both reset and the existing update, avoiding fixed pointer offsets in
host execution. Keep the existing missing-system guards. The external owner's
first twelve bytes remain unknown, not an invented complete runtime layout.
`instNuGCutSceneReset` improves **22.56% to 61.859%**.

Four packed quaternion evaluators use the reference's ordered lower frame
clamp, mapping NaN to the first key while retaining negative zero, and the
first evaluator uses signed division by four. Their aggregate improves
slightly; two individual scores regress by about 0.14 and 0.01 points.
Do not restore undefined NaN-to-integer conversions just to recover those
small differences.

Overall matching reaches **66.327040%**, **+1.202560 percentage points**
from main, with **6,269** exact functions. All eleven GitHub checks passed
for batch 120. The approximately two-point goal remains unfinished.
Ownership, optimization settings, public ABI and scoring stay unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE
fixtures: **8,000** portal state/service/geometry cases, **10,000** cutscene
reset cases and **12,000** four-format animation cases. Tests include
camera and locator metadata changes during callbacks, missing systems,
first-frame trigger visibility, preserved state-word upper bytes, NaNs,
infinities, negative zero and key boundaries. Animation interpolation and
external services use stand-ins; this is not full engine integration.

The eight-slot quick-path expansion regressed from 19.57% to zero and was
fully reverted. Its original private path helper passes arguments through
registers and SSE as a compiler optimization; the current source retains
ordinary portable calls. Do not add calling-convention attributes or infer
an extra public argument from the decompiler's undefined stack temporary.

## Batch 122: batarang targeting closure and throw transitions

Restore the private search called by both `Batarang_MoveCode` (screen-space)
and `Batarang_Release` (forward automatic targeting). Recover object, ten
detonator and gizmo filters, duplicate selection rejection, first-match versus
nearest-match ordering, ray/platform acceptance and five-slot FIFO rollover.
Object screen selection intentionally retains its 64-unit-squared comparison
limit for the subsequent detonator scan; it is not an ordinary closest-of-all
selection. Automatic candidates use the reference's 16-unit-squared limit.
Null-world handling lives at both callers. The ordinary three-argument private
helper naturally becomes GCC's `.isra.0` clone, without ABI annotations or
explicit clone calls, and matches **37.038322%**.

Restore aim ramp, held-action release, animation event/duration gates, buffered
jump, input cancellation, HUD sight/target markers and message colours. Raw
disassembly at `0x1d9313`–`0x1d9355` establishes the three colour writes that
the decompiler omitted. Grouping the returned RGB components as a three-element
array recovers the non-realigned stack frame naturally. `Batarang_MoveCode`
improves **18.244944% to 34.422470%**.

Release uses the reference's four-unit horizontal velocity, keeps ricochet
state, obtains position after the rumble callback, and uses the configured
joint when available. Automatic release searches before creating the inline
type-three fallback two units ahead. Recover the fallback's three-float payload
at target `+0x08` in `BATARANG_TARGET_s` and signed `batarang_joint` at config
`+0x112`, with layout assertions and retained compatibility views.
`Batarang_Release` improves **16.741072% to 58.080357%**.

Keep existing null/flight guards. New marker calls reject negative or out-of-
storage target indices inside the position helper, while retaining the
in-flight return-to-owner case at index equal to the active count. Its old
temporary `__used__` marker is retired now that both real callers exist.
The helper regresses **71.9802% to 32.72277%** with these guard/caller changes;
seek-to-target improves **93.26887% to 95.49056%**. The full batch is positive
and loses no exact functions. Do not undo bounds safety merely for its score.

Overall matching reaches **66.355930%**, **+1.231450 percentage points**
from main, with **6,269** exact functions. All eleven GitHub checks passed
for batch 121. The approximately two-point goal remains unfinished.
Compiler options, ownership, public ABI and score normalization stay unchanged.

Extracted production bodies pass 64-bit ASan/UBSan and optimized i386 SSE:
**20,000** target-selection/raycast/FIFO cases, **30,000** throw transitions and
**10,000** release cases, including null worlds, callback replacement of the
batarang, post-callback position, joint origins, inline fallback pointers,
HUD colours and bounded marker access. Selection and caller state machines
are isolated with service stand-ins, not full gameplay integration. Existing
out-of-range joint metadata and unbounded timer-to-integer inputs remain
separate policy audits.

Batch 121 follow-up: typed trigger-state update lowered its own score from
83.916664% to 82.766670%, and two animation evaluator scores declined slightly;
these are documented semantic/layout fixes in a net-positive batch, not
hidden by changing the comparison settings.

## Batch 123: rejected editor/player source-shape trials

The reference's player checks and particle colour stages contain fixed-slot
closures. Unrolling them under their live `-O2` owners did not improve
matching: the player routine stayed at zero, and the colour routine fell
from 21.344992% to zero, including a reference-proven bounded high/low
sixteen-bit colour conversion. These trials were reverted completely.

The path renderer has separate equal-radius and solid-wall geometry branches;
restoring their direct perpendicular sides reduced matching from 25.327837%
to 2.336%. The path editor's two fixed first-free connection-pair searches
passed 131,072 exhaustive canonical-record cases under both host sanitizers
and optimized i386, but reduced its score from 28.321720% to 22.997%.
Restoring its forward/backward traversal closures and fixed menu slot lookup
reduced it further to 14.019%. All editor trials were reverted. No source
ownership, optimization or scoring changes were made to accommodate them.

## Batch 124: character attachment animation and save-state protocol

Restore `Animate_CHARACTER`'s Jabba hub override, held special-action choices,
uncontrolled-context jetpack idle/run choices, Penguin umbrella packet and
glidepack context variants. Correct the pending-target fallback to inspect
the model's fall-animation entry, not unrelated target bytes. Restore the
game-character fall gating and use canonical suit and mini-packet fields.
Keep guards for missing worlds, attachment IDs and character-system arenas.
The routine improves **32.489490% to 66.885890%**.

Restore `UpdateSaveSlots`'s mutually exclusive message/result delay pairs,
autosave pre/post timing, load failure distinctions, save completion callback,
deletion and formatting state transitions. A request and its completion are
mutually exclusive within one update. Missing save/load buffers fail before
later operations. Recover the original four-byte `saveload_error` BSS global
in the framework save module and publish its declaration. The routine improves
**34.625600% to 67.099200%**.

Checksum trailers use their exact byte offset with `memcpy`, including sizes
not divisible by four. Keep the existing extra-buffer checksum validation:
retail's raw instructions at `0x42a4ae` call the extra-data checksum but
`0x42a4b3` compares the earlier main-data checksum in EDI against the main
trailer again. This integrity safeguard deliberately differs from that
reference bug; it is not hidden by comparison normalization. Buffer capacity,
very large sizes and callback mutation of allocation metadata remain separate
audits; fixtures use allocated, bounded buffers and service stand-ins.

Make inline `NuASin` use the polynomial addition grouping already recovered
by the 99.921210%-matching standalone implementation. Preserve multiplication
grouping, coefficients and the defined input domain. This improves retained
render-grid and batarang ricochet callers. The path renderer declines slightly
to 24.320078%; animation-neighbor/register changes are below 0.27 points.
No exact function is lost. Overall matching reaches **66.392570%**,
**+1.268090 percentage points** from main, with **6,269** exact functions;
the approximately two-point goal remains unfinished.

Extracted production-body fixtures pass 64-bit ASan/UBSan and optimized
i386 SSE: **60,000** character selection/attachment cases, **20,000** save
state and multi-frame protocol cases, and **2,000,001** valid-domain arcsine
comparisons against the retained standalone polynomial. Animation handlers,
storage services and square root are stand-ins, not complete gameplay or
filesystem integration. All eleven GitHub checks passed for batch 122.

## Batch 125: hardware bootstrap and variadic setup cursor

Restore `NuInitHardware`'s token widths, four pad output pointers, setup
options and complete initialization order. The platform parser now receives
the address of the actual variadic cursor rather than consuming a pointer
argument unconditionally; END does not consume another argument. Publish the
recovered deferred-FX and sound-target-manager globals. Preserve the legacy
mode token's consumed-but-unused behavior: this bootstrap selects mode 2.

Use canonical public initialization declarations, including the framebuffer
setup arguments and occlusion manager. `va_list *` expresses the platform
parser's cursor contract portably and retains the original `(i32, char **)`
mangled name on i386. Do not add a third renderer-init argument merely because
the original caller writes an unused extra stack slot: its callee reads only
the existing two arguments. No calling-convention, optimization or alignment
shortcuts are introduced.

Capture the arena end after platform initialization and retain that snapshot
through geometry, font and occlusion initialization. Material override receives
the snapshot's address, not the caller's live end pointer. Recover aspect,
brightness, host filesystem, persistent DFS selection, pads/online, initial
clear/swap and the successful return value.

`NuInitHardware` improves **19.691824% to 79.919815%**. Whole-binary matching
reaches **66.424866%**, **+1.300386 percentage points** from main; all **6,269**
exact functions are retained. All eleven GitHub checks passed for batch 124.
The approximately two-point goal is still unfinished.

Extracted production-body fixtures pass **8,193 cases on each ABI**:
64-bit ASan/UBSan and optimized i386 SSE. They cover every flag combination,
all sixteen pad-output masks, mixed option widths, platform cursor consumption,
END-only calls, invalid format retention, persistent/empty DFS names, cached
arena-end semantics and complete service order. Engine services are stand-ins,
not an Android or full gameplay execution.

Two reference-informed eight-way `FindPart` filter specializations decline
from 31.284% to 13.407%/14.013% under the unchanged O2 owner. Revert both;
do not retain the larger source merely because its semantics resemble retail.

## Batch 126: socket search, rail samples and camera bounds

Recover `BestSockPosition`'s alternating segment search, genuine initialized
search state and shared successful return path. Invalid/out-of-bounds sockets
return zero; valid outer bounds without an accepted segment return the original
1,000,000 failure distance. Keep ordered inner bounds and the reference outer
rejection tests, including their unordered comparison behavior. Copy the first
eight resolved bytes after `FillSockPosition` without type-punned word reads.
The routine improves **0% to 93.943214%**.

Restore `SockRailAngles`'s three fixed corner-average samples when its explicit
spline argument is null; do not substitute an independently populated
`sock->mid`. Recover the local helper's returned output cursor, integer
edge count, live camera-rail length and open/looping endpoint rules. Its score
improves **37.894497% to 74.100914%**. Restore the socket-local plane/line
predicates' original integer results; no calling-convention attributes are used.

Restore the camera owner's used plain LOCAL `VuVecMtxMul` at original
`0x2a3b35`, supported by the query's direct calls and the recovered camera TU
boundary. Do not move or artificially emit the unused copy in `vumath.c`.
`NuCameraClipTestExtentsAxisAligned` restores temporary global far-plane
adjustment, helper calls, six rejection tests and conditional scissor tests.
Raw SSE instructions establish the fourth/far/near projection grouping:
translation + (z product + (x product + y product)). Ghidra's flattened
expression obscures this rounding-sensitive order. Preserve the fourth
absolute scissor translation and restore the far plane before classification.
The query improves **23% to 89.820710%**, with unchanged center/extent arguments.

Whole-binary matching reaches **66.503426%**, **+1.378946 percentage points**
from main, with **6,270** exact functions and no exact function lost.
All eleven GitHub checks passed for batch 125. The two-point target remains
unfinished; compiler flags, scoring normalization and source ownership maps
are unchanged.

Production-body fixtures pass on 64-bit ASan/UBSan and optimized i386 SSE:
**399,600** bounded socket cases, **90,000** rail geometry/callback cases and
**65,536** camera cases. Socket services check plane/edge arguments, search
order, short-circuiting, output bytes and post-fill snapshot timing. Rail tests
cover explicit versus corner samples, open/looping endpoints and callback
length mutation. Camera tests include cancellation, zero extents, unordered
inputs, far overrides, all outcomes and byte-exact global restoration.
Math/engine callbacks are stand-ins; these are not full gameplay integration
or an Android device run.

## Batches 127–128: light calculation, editor drawing and state publication

Recover `rtlCalcLights`'s direct callback-sensitive light reads, three fixed
direction scales, ambient intensity multiplication and ordered saturation.
Null directional slots use the original shared `nuvec_y` vector; restore that
verified twelve-byte `(0, 1, 0)` global alongside the existing math vectors.
The original local routine returns void, and its only caller ignores the
result. Its score improves **30.318650% to 58.001470%**.

Restore editor light types 3, 6, 7 and 8, separate radius-pair switch arms,
direct line endpoint math and post-callback set reads. Mask each packed RGB
component before its unsigned shift, avoiding the previous negative signed
shift. `edrtlDrawLightEx` improves **33.998290% to 54.582905%**. Do not add a
fourth `RndrOSquare` parameter: the original caller writes an extra material
slot, but the callee reads only its existing three arguments.

Restore the two renderer-local lighting wrappers' shared-state writes before
their platform calls. `rtlSetLights` calls these wrappers rather than bypassing
them. Both local setters and the public submission routine become exact:
**three new exact functions**, with no exact function lost. Whole-binary
matching reaches **66.530655%**, **+1.406175 percentage points** from main,
with **6,273** exact functions. All eleven GitHub checks passed for batch 126;
the approximately two-point goal remains unfinished.

Extracted production-body tests pass on 64-bit ASan/UBSan and optimized i386
SSE: **50,000** lighting cases and **20,000** editor/state-publication cases
on each ABI. Lighting tests include mutable selected lights during callbacks,
all supported types, null positions, shared vector reads and unordered ambient
clamping. Editor tests cover bounded negative/over-one colours, selection,
radius ordering, geometry and service order; state tests check publication
before callbacks and callback mutation before ambient submission. Engine and
math services are stand-ins, not complete renderer integration.

A restored Huffman tree/helper trial regresses overall matching and is fully
reverted. Live compile actions show that its current `inflate.cpp` owner uses
O3, while the reference helper family has O0 code shape. That alone does not
prove source ownership. Do not change options, move the family or force helper
emission merely to improve the score. Build configuration, normalization and
ownership maps remain unchanged.

## Batches 129–130: grapple transitions and light-set selection

Recover `Grapple_MoveCode`'s activation gates, ledge release, jump services,
animation-rate floors, rope-depth limits and phase-boundary amplitude changes.
Restore the verified `LEGOHINT_GRAPPLE` global initialized to -1. Preserve
existing null/context safeguards and reload the current grapple after services.
The function improves **19.468004% to 20.639%**; this is a modest matching gain,
despite substantial recovered behavior. Production-body fixtures pass on
64-bit ASan/UBSan and optimized i386 SSE: twenty focused scenarios and a
multi-frame ledge acquisition/release transition per ABI. Mock engine services
check callback order, hint/jump parameters and state transitions; these are not
full gameplay integration tests.

Restore `rtlApplySetScaleLoop`'s distinct ambient, directional and anti-light
paths, direct distance expressions, nested upper/lower falloff clamp, integer
restriction state and short-circuit grid coordinates. Keep the existing light
array and modifier-index safeguards. The raw reference confirms the nested
clamp's repeated expression, rather than a call to `ClampUnit`. New shadow
selection publishes its blend after the vector callback; refreshing an already
selected shadow does not restart its blend. The score improves
**43.038400% to 82.144000%**, without optimization or ownership changes.

The extracted lighting-loop fixture passes **20,000** bounded cases on each of
64-bit ASan/UBSan and optimized i386 SSE. It checks all light types, identity
and restriction masks, null positions, dynamic versus indexed iteration,
radius/falloff and modifier selection, specular wobble and shadow callback
mutation/order. Math and renderer services are stand-ins.

Whole-binary matching reaches **66.569060%**, **+1.444580 percentage points**
from main, with **6,273** exact functions and no exact function lost. All eleven
GitHub checks passed for batches 127–128. The approximately two-point goal
remains unfinished; build flags and scoring normalization are unchanged.

## Batches 131–136: Euler quaternion sampling, turret traversal and wind loops

Recover the two V4 Euler-quaternion players' endpoint decoding and quaternion
interpolation rather than interpolating Euler angles before conversion. Restore
fixed three-axis skip groups, the reference's count-before-first-joint clamp,
single-key nonblend sampling, and the blend player's distinct upper-key test.
Constant nonblend rotations need only one Euler conversion; animated blend
rotations always convert both endpoints before harmonizing and normalizing.
The blend path preserves root-translation publication before blending, including
aliasing with the output joint, and normalizes the absent-rotation identity blend.
The nonblend player improves **23.303432% to 30.146180%**, and the blend player
**23.958164% to 35.286736%**. Public signatures and compiler options are unchanged.

Production-body fixtures pass **20,000** bounded animation cases per ABI on
64-bit ASan/UBSan and optimized i386 SSE: mixed packed/constant rotations,
quarter-three packet transitions, skipped joints, count clamping, single keys,
default groups, flags, frame limits, conversion/normalization calls, blend state,
and null/separate/aliased root outputs. Math services are stand-ins, not an
Android animation integration test. Generic unsigned-conversion and branchless
frame-selection trials regressed or failed to improve matching and were reverted.

Recover the turret update's initial count check and do-loop, live count reloads,
flag-clearing order, and three separate eligibility gates. Matching improves
**1.011% to 4.347758%**. A focused fixture extracts the edited outer loop and
gates, replacing the unchanged inner engine body with a callback. **50,000**
cases pass on both ABIs, including early continues, all flag combinations,
rotation-sound state, zero counts and callback count shrink/extension. It does
not test turret firing or the complete update body. Matrix/controller branch
reordering regressed matching and was reverted.

Recover `NuWindUpdateArray`'s separate wind-only and contact loops, shared
interaction latch, reusable scalar state, direct distance expressions and
reference Z-wave arithmetic grouping. Raw instructions, not flattened Ghidra
conditions, determine the ordered visibility and contact comparisons. Retain
the existing wrapping counter increments. Matching improves **68.013% to
91.000%**. **20,000** bounded three-group/matrix oracle cases pass on both ABIs,
including null/all-null positions, all eight candidate slots, nearest selection,
wind-only/contact paths, clamp limits, inactive/undrawn/zero-scale groups,
multi-group latch behavior and square-root call counts. Additional checks cover
NaN visibility bounds and an empty update. The oracle independently uses the
previous combined-loop body with the verified Z-wave grouping; math services
are stand-ins. An initial split-loop trial with incorrect branch grouping and
duplicate square roots was replaced before retention.

Terrain persistent-state types/evaluation-order trials did not improve matching
and were reverted. Fixed push-hint fanout also scored worse and was reverted;
the earlier rejected experiment remains applicable. The unresolved variadic
contract of `NuErrorSleep` remains deferred, not reconstructed speculatively.

Whole-binary matching reaches **66.614655%**, **+1.490175 percentage points**
from main, with **6,273** exact functions and no exact function lost. All eleven
GitHub checks passed for commit **65a78f4f** (batches 129–130). The approximately
two-point goal continues. No optimization, ownership or scoring changes are made.

## Batches 137–142: object traversal and debris-frame tables

Restore `UpdateGameObjects`' four cached object-pointer walks. Each pass captures
`Obj` once and retains a live `HIGHGAMEOBJECT` limit across callbacks; the lighting
pass captures its array before `FindGameObject`. Restore the missing AI, player
terrain/animation, lighting and collision timing callbacks. Raw instructions
confirm that the lighting tail calls `TBOPENFN`, not `TBCLOSEFN`; preserve that
reference behavior and recheck `TimingBarSet` before opening collision timing.
Recover the cadence miss's early continue, aggregate saved-position copies, and
two fixed player-indicator/message blocks. Ordinary fixed-expansion macros share
the two message bodies without forcing inlining or changing compiler options.
Matching improves **29.453% to 43.470867%**.

A focused production traversal/cadence fixture passes **20,000** cases on each
of 64-bit ASan/UBSan and optimized i386 SSE, including callback array replacement,
count shrink/extension, empty passes, cadence flags and saved-position restoration.
The unchanged engine bodies are replaced with callbacks; this does not verify
the complete object update, message rendering or timing-service implementation.

Recover `GenericDebinfoDmaTypeUpdate`'s shared key index and scalar interpolation
state, direct key-array accesses, repeated trigonometric expressions, unsigned
texture-coordinate extraction and reference reciprocal scaling. Preserve the
FLY effect's ordered coordinate stores and per-colour clamp sequence. Matching
improves **60.059% to 87.231770%**. Production-body fixtures pass **20,000** bounded
cases per ABI, checking all 64 frames, geometry, colour packing, allocation and
exhaustion, FLY repair, gaps and repeated keys. The independent previous-loop
oracle uses the reference-verified unsigned/reciprocal texture extraction; its
trigonometric table and string service are test stand-ins.

Whole-binary matching reaches **66.677536%**, **+1.553056 percentage points**
from main. All eleven GitHub checks passed for commit **8c1ccb6c** (batches
131–136). The approximately two-point goal remains in progress. No optimization,
ownership, ABI-attribute or scoring changes are made.

## Batches 143–150: socket generation and camera accumulation

Recover `SockSys_GenerateData`'s cached socket/segment walks, live spline
lengths and buffer advancement from `SockRailAngles`' actual return pointer.
Restore the three inclusive capacity guards, four-rail normal-call order,
corner-averaged segment midpoints independent of an optional middle rail,
separate total-length accumulation and the reference's broad-separation versus
narrow-overlap comparisons. Use the public overlap-bit service and immediate
nested-loop exit when an overlap is found. Matching improves **47.207100% to
93.096370%**. A production-body fixture passes **10,000** cases per ABI on
64-bit ASan/UBSan and optimized i386 SSE, covering generated bytes, allocation
limits, buffer endpoints, open/closed and two/four-rail geometry, masks and a
callback that changes live lengths. Geometry/math services are stand-ins.

Recover `SockSysCamera`'s shared scratch/scalar state, delayed candidate lookup,
public bit query, ordered arena predicates, per-player arena height addition,
look-target arithmetic grouping and target scaling after scalar publication.
Restore direct single-player pullback and separate fixed spatial/planar
two-player pullbacks without the extra positive-distance gate. Pitch/yaw wraps
use a signed integer conversion before narrowing, as in the reference.
Matching improves **72.991165% to 81.080020%**. Production-body fixtures pass
**20,000** bounded cases per ABI, checking all outputs, packet state, nullable
outputs/rails, failed socket lookup, exclusions, one/two-player modes, arena
and pullback paths, clamps and rotation wraps. Their independent previous-body
oracle incorporates reference-verified arithmetic and conversion corrections;
this is not Android gameplay integration or arbitrary nonfinite-input coverage.

Two `Hub_Update` fixed-fanout trials and two `AIMoveToDestination` scalar/
arithmetic-layout trials scored worse and were fully reverted. They do not
justify changing optimization, ABI attributes, ownership or scoring.

Whole-binary matching reaches **66.728470%**, **+1.603990 percentage points**
from main, with **6,273** exact functions. Native build and repository checks
pass. All eleven GitHub checks passed for **cd236e31** (batches 137–142).
The approximately two-point goal remains in progress.

## Batches 151–161: menu state, animation loading and gameplay closures

Recover `UpdateStats`' super-story/challenge dispatch, direct player coin
accumulation, reference store order and HUD visibility predicates. Matching
improves **9.948648% to 61.113514%**. Its complete production-body fixture
passes **10,000** randomized valid states and targeted callback mutation cases
on host ASan/UBSan and optimized i386 SSE, comparing 23 outputs and service
order. Reference preconditions are restored: valid world/current level,
initialized camera in the hub, valid gameplay area index and initialized
translation table for rewards. Invalid engine initialization is not covered.

Recover `GizPanel_Update`'s eight fixed player probes, reusable vector locals,
live player reload after distance callbacks and shared pitch clamp. Matching
improves **0.789630% to 13.106667%**. Both ABI fixtures pass **2,401** angle
cases plus slot priority, invalid players, callback replacement/count changes
and NaN-distance/timer cases. Raw unordered timer-clamp behavior is preserved;
a provisional ordered clamp was rejected before integration.

Recover `Hub_DrawAreaStats`' cached area base, signed episode sentinel and
reference valid-index contract (**9.691906% to 35.291122%**). Focused i386
fixtures cover clip transitions, doors, chapter sentinel and completion state.
`Hub_DrawPanel` now routes on the camera socket byte, uses the reference strict
challenge-time comparison and exact reward coordinates (**39.473606% to
39.770596%**). Its extracted sections pass host sanitizers and i386 tests for
room/mode distinction, below/equal/above/NaN times and reward branches.
`Hub_Update` reloads player eligibility after each gizmo callback; matching
reaches **15.747258%**. Its selection/fountain/fade section fixture passes
activation, deactivation and replacement cases; it is not a full update test.

Recover `UpdateAnimPacket`'s interruption/transition and shared timer dispatch,
including the live overlay reload after end-frame callbacks. Matching reaches
**76.789830%**; **100,000** bounded production-body/oracle cases pass per ABI.
Recover `APILoadCharacterModels`' direct path/flag scans, post-condense list
reload, byte-width fixup publication and missing character-local BSA fallback,
including packed-excluded entries. Raw BSA loads do not redirect animation
paths. Matching reaches **86.448270%**. **20,000** independent oracle cases
pass per ABI, covering pack/raw failures, redirects, hierarchy reuse/cleanup,
fixups, metadata byte preservation and callback list replacement. Existing
512-byte path capacity and deterministic reused-model pack paths are retained.

Recover `InitStatusScreen`'s sequential flag publication, typed completion
byte, overflow-aware 32-bit coin sum and live mission index after score
callbacks. Matching reaches **20.167751%**. **10,000** valid-state fixtures
compare full packet/save bytes and service traces on host and i386; host
ASan/UBSan uses the same bodies with the unrelated renderer table external.

Restore missing `LightSabreStreakCode` collision push, spark sound/cooldown,
rumble, NPC gizmo destruction, part hit/deflection and surrounding alerts.
Preserve callback-sensitive metadata/effect reloads and ordered AABB predicates.
Matching reaches **44.497646%**. Thirteen focused actual-source cases pass
under host sanitizers and i386; they are not an exhaustive combat oracle.
`GameObjectStuffAfterAnimation` restores audio/target callback snapshots,
reference float operand order and three explicit Landspeeder emission sites,
reaching **48.345737%**. Callback cases and **3,200** emission combinations
pass on both fixture ABIs.

Recover `NewScanRot`'s signed cache ages, nested static group/shape scan and
scratch-vector normal construction, reaching **29.944540%**. **9,217** state,
cache, call-order and normal fixtures pass on both ABIs. This is ordinary
source-level helper closure; no forced inlining or optimization change is used.

Recover `GizBuildIts_LateUpdate`'s separate manual/automatic progression,
eight builder probes, inactive-piece bounds, direct audio position pointers,
callback-sensitive orbit objects and fixed wobble-axis bodies. Finishing
callbacks returning to idle correctly reach wobble. Manual NaN timers remain
pending while automatic NaNs complete, as in retail. Matching reaches
**22.305555%**. Host sanitizer and i386 fixtures cover these cases, rewards,
real trig values and callback count/axis/array mutation. Manual unlinked
completion requires an animation instance; automatic completion permits null.

Recover `ShootCode`'s signed byte context, reference weapon-in branch sharing,
guarded metadata lookup and immediate-shot tree (**13.116515%**). Remove the
unnecessary reconstructed `__used__` marker: the unchanged compiler naturally
infers the original private call ABI. Both objects retain all 190 text symbols
and 11 direct calls from ten callers; no calling-convention attribute is added.
Twenty-eight actual-source NDK i386 fixture checks pass for player/NPC gates,
null metadata/model early exits, weapon transitions and signed contexts.

Recover `DrawParaphernalia`'s normal/flickering shield dispatch, live config
reload, pre-scale locator and reflection snapshot (**39.311780%**). Its actual
shield block passes host sanitizers/i386 callback and flicker cases, including
NaN timers. The original normal shield scale is retained when its flicker
timer is not positive. Correct `DrawMiniKitCount` to use `GlobalTimer`, not
`GameTimer`; 10,000 production-body time cases and distinct-clock checks pass
on both ABIs, with no normalized-score change.

Camera occlusion expansion, directional ladder, broad AI/terrain reorder and
several particle expansions regressed and were rejected. Placement-only
rendering improved in isolation, but combining it with the larger shield unit
regressed; only the verified shield unit is retained. Raw instructions also
disproved a suspected `NuASin` associativity difference; its header is unchanged.

Whole-binary matching reaches **66.904200%**, **+1.779720 percentage points**
from main, with **6,275** exact functions. Target/native builds and all five
repository tests pass. All eleven GitHub checks passed for **acb9f298**.
Fixtures use mocked engine/math services and do not establish Android gameplay
integration. The approximately two-point cycle continues; ownership, compiler
options and matching normalization remain unchanged.

## Batches 162–175: cutscene branches, clipping and render closures

Recover `MovePlayer_ROLLING`'s local Y/X/Y rotation closure, including all
matrix rows, without changing shared math helpers. Matching improves
**44.997390% to 76.436030%**. The production NDK i386 body and previous-body
oracle pass 156 cases, including callback matrix mutation and Euler publication.
`Hub_DrawFreePlaySelect` restores complete valid-capacity list traversal
(**35.422245% to 37.611740%**). Focused host/sanitizer fixtures cover duplicate
selection, boundary capacity and terminator/canary preservation. Original valid
inputs require at most 339 selected entries in mode 2, and 340 including seed
entries in modes 3/4; arbitrary overflowing inputs are not supported.

Recover generic `ANI_SimpleAni3PlayerV4Joint`'s fixed nine-curve skip closure,
node/type cursors, clamp branches and signed division (**18.765038% to
36.280075%**). Host ASan/UBSan and i386 fixtures pass 100,000 differential cases
per ABI, including dispatch/default groups. Valid joint/sample bounds remain
required; NaN-to-integer conversion is not defined or claimed.
`NuCameraClipTestExtentsGeneric` initializes each outcode in its classification
loop rather than clearing the complete array first, allowing the original
register-packed closure (**44.893864% to 64.241950%**). No geometry math changes.
Both ABI fixtures pass 32,768 differential cases and five boundary probes,
including nonfinite coordinates, flags, transforms and aliased depth output.

Recover explicit optional-output/count branches in legacy and modern cutscene
character processing. `NuGCutCharAnimProcess` improves **4.475191% to
89.305340%**; `NuGCutCharAnimProcess_3` improves **44.898438% to 71.488280%**.
Signed legacy counts, modern rounding, sentinel order and callback-sensitive
count reloads remain unchanged. Each ABI passes 90,112 legacy, 8,192
no-animation and 81,920 modern/direct output cases against independent oracles.

Recover `DrawPanel`'s 512-byte text buffer, controller-two green tint, paused
player alpha branches and distinct owned/borrowed icon submissions
(**43.765343% to 45.216286%**). Modified-section fixtures pass 10,000 randomized
cases plus targeted host sanitizer/i386 probes. Raw instructions disprove two
provisional changes: bonus-score shifts are logical 0/1, not signed masks;
challenge/mission NaN timers remain NaN. Neither rejected change is integrated.
`DrawStatusScreen` restores stage-array rebasing after callbacks, cached demo
labels, two explicit option closures and live fallback color reads
(**21.890034% to 51.864260%**). Its complete-body fixture passes 10,000 bounded
oracle cases and callback mutations on both ABIs. Menu pulse inputs are finite
and initialized stage arrays must remain valid for the current iteration index.

Recover `GizObstacles_Update`'s mode-7 start-or-stop dispatch, live animation/SFX
reloads, stable traversal/count snapshot, reward-gated resets and unsigned pickup
angles (**5.870968% to 47.526882%**). Both ABI fixtures pass 64 eligibility and
640 dispatch cases plus traversal, reward and callback probes. Recover
`UpdateExplosion_Generic`'s cached object walker and post-arcade owner reload
(**8.408084% to 13.610169%**); complete production-source fixtures cover owner
replacement, 12 live traversal cases and 66 emission angles. `JediB_Update`
restores its second-plane live object lookup and stay/remove branches
(**45.921864% to 46.792060%**); focused culling fixtures pass both ABIs. Callback
replacement must preserve the reference's required non-null object when a
second plane or explosion owner is actually dereferenced.

Recover `NuDisplayListRndrSpecial`'s wind matrix, forced LOD/material selection,
owning-list usage flags, transform-packet reuse, shadow-only draws, per-light
render sets and clip-result cleanup (**16.781250% to 21.759115%**). Its actual
function and unchanged helper bodies pass 2,048 instrumented scenarios plus
sentinel, NaN and null-guard cases on host sanitizers/i386. The existing empty
`WindShear` is recovered with its verified four-argument C ABI, seeded random,
unsigned frame conversion and complete matrix shear. Raw SSE proves the
pre-callback frequency product and quarter+(half+base) waveform grouping;
1,040 separate/in-place calls match all 16 fields bit-exactly on both ABIs after
correction. WindShear itself remains 0% in its unchanged owner/optimization
mode; caller-plus-helper restoration gives a positive whole-binary result.

Recover `DrawCables`'s per-control-point terrain sampling/interpolation,
chained subdivision cursor, half-step termination, phase clamp and shared
line/rope vertex packet. Preserve the original start-floor NaN selection,
flat-path counter behavior and duplicated rope arguments. Both actual-body
fixtures pass terrain/callback, zero-span, sentinel, phase and rope/flat cases;
the previous body fails the terrain-query probe. `UpdateCables` separately
restores target and locator snapshots across geometry callbacks; 19 cases pass
per ABI for both wrapping directions and counts 0/1/2/3/14/15.

Recover miniature snow-trooper team reloads, post-turn stop-bit rechecks,
normal/nonzero-droid formation selection and exact random/movement constants.
Formation 4 intentionally copies the unrotated source after an out-of-place
rotation, as in retail. Actual update/shape fixtures pass host sanitizers and
i386, including settling/NaN, callback replacement and random endpoints.
Correct the timed particle routine's emission cap to 99; full routine fixtures
pass regular/glass and allocation limits on both ABIs, with no score change.

Whole-binary matching reaches **67.076330%**, **+1.951850 percentage points**
from main, with **6,276** exact functions. All eleven GitHub checks passed for
**00434e77**. Fixtures use mocked engine/math services and do not establish
Android gameplay integration. Ownership, compiler options and scoring remain
unchanged; explicit particle expansion and several curve/force layout trials
regressed and were rejected. The approximately two-point cycle continues.

## Batches 176–179: complete the two-point cycle

Recover `FireCode`'s context ladder, delayed-press gate, bolt-class flag and
pre-callback target-mode snapshot. The delayed-press parameter, not the float
cooldown duration, controls timed rejection; the original class bit is
`0x04000000`. A null area must not select the bonus-gunship branch when its
area identity is also null. Isolated matching improves **44.231% to 56.947%**.
The complete-body host sanitizer and production i386 fixtures pass 384 cases.
Removing the existing `__used__` marker only after restoring the missing
parameter behavior naturally retains the exact private symbol and all callers:
all 181 translation-unit text symbols survive. A removal-only experiment that
lost the exact symbol was rejected. No calling-convention attribute is added.

Recover `GizmoPushBlockInitAndReset`'s traversal snapshots, canonical instance
animation end-frame lookup, post-evaluation snap count and single matrix
snapshot. Restore four explicit terrain probes, callback-sensitive metadata
writes and original support-height summation order. Isolated matching improves
**7.586449% to 50.175232%**. Actual-body sanitizer and optimized i386 fixtures
cover counts 0–4, animation endpoints, capacity/exhaustion, array replacement
in all three passes, terrain sentinels, equal/unequal/NaN support heights and
callback mutation. Its pre-existing function-level `optimize("O2")` is unchanged;
this batch neither introduces nor relies on a new optimization override.

Recover `RenderOccluders`'s six explicit vertex submissions, material snapshot,
signed masked colors and count publication. Restore `IsOccludedSphere`'s ordered
radius/bounds rejection and four explicit edge closures, including normal
initialization before normalization. Renderer negative-depth tests deliberately
retain their original unordered behavior. Isolated scores improve
**20.402325% to 58.855812%** and **42.135048% to 44.717040%**, respectively.
The neighboring `RenderStats` decreases **98.701035% to 84.257730%** through
ordinary translation-unit code generation; the net weighted gain remains
positive. Separate-TU mocked-service fixtures pass 2,048 renderer and 32,768
sphere cases on sanitizer/i386 builds. Geometry/color-conversion inputs obey
the valid reference contract; the fixtures do not claim arbitrary nonfinite
color conversion is defined.

Recover `Move_VEHICLE`'s metadata reads after weapon/drop-in-out callbacks and
the movement-disabled gate, then reload metadata after later callbacks.
The Hoth hover hack is tested only inside the positive hover-height branch;
the fallback height is read after the hover query. Isolated matching improves
**18.205% to 26.926%**, or **28.699%** together with the corrected `FireCode`.
Both actual-body fixtures pass five focused callback/gate cases, including
null metadata on the early disabled path. Flight, smoke and combat services
are mostly skipped or mocked: this is not a complete vehicle gameplay oracle.

The combined target builds and reaches **67.128075%**, **+2.003595 percentage
points** from main baseline `05429366`, with **6,276** exact functions. Ownership,
compiler options and scoring remain unchanged. Merge requires all GitHub checks
to pass on the final PR head, not merely a previous passing commit. All fixtures
remain diagnostics with mocked services, not Android gameplay validation.

## Batches 180–189: new cycle, prioritize missing closures

PR #120 was squash-merged as `2a57d7b7` after all eleven final-head checks
passed. The new main baseline is **67.128075%** with **6,276** exact functions;
the next two-point target is **69.128075%**.

`AddVariableShotPARTEffect` restores ordered scheduling and rate selection,
plus the rotation snapshot after three random calls (**0% to 66.685%**).
Raw SSE, not the decompiler's reversed expression, proves that an unordered
requested rate is retained while either signed zero selects the default.
Both actual-body fixtures and a separate rate-predicate probe pass on host
sanitizers/i386. Inputs that would convert nonfinite emission times to integer
are outside the fixture's valid contract.

Restore `GenericBlend`/`EulerBlend`'s original integer return ABI, helper-result
propagation, packed-key signed division and key-count upper clamp. Header and
Euler forward declaration agree; callers still discard the result. Isolated
scores improve **39.416% to 40.221%** and **35.093% to 35.498%**. Both ABI
fixtures pass 100,000 generic and 20,000 Euler cases. Unaffected declaration-only
consumer sections are byte-identical; NaN-to-integer inputs remain excluded.

`DrawSubItems` restores extras' unlocked/locked rendering branches and live
item identifiers (**25.937% to 26.225%**, 10,000 actual-body cases per ABI).
`InitMiniSnowTroopers` restores sentinel/terrain fallback, team reloads,
stored byte-count loops, deferred formation indexing, the original first-trooper
rotation quirk and random scaling (**27.176304% to 28.148096%**). Actual-body
host/sanitizer/i386 fixtures cover both formation widths, callback mutation,
zero-length padded spline storage and random endpoints.

`GameCreatureOpponentSelection` restores the ordered expired-alert comparison
(**47.002860% to 47.095783%**, 70 cases per ABI). `InitStatusScreen` reloads the
mission event count after completion/reward callbacks (**19.774% to 19.925%**,
10,000 full-body cases per ABI). `Hub_Update` restores episode-door and build-it
reloads (**15.389605% to 17.036242%**); its fixtures exercise the affected actual
loops, not the whole hub function. `GizBuildIts_LateUpdate` reuses the original
completion/finishing position scratch while retaining a separate orbit scratch
(**21.998537% to 24.315058%**); actual-body fixtures pass both ABIs.

Recover `TerrainPlatformEmbeddedScan`'s four scale/rotation traversal closures,
three/four-vertex transforms, source-versus-rebuilt normals and shared cross
operands (**0% to 25.995962%**). Actual-body fixtures pass 2,048 cases and three
exits on i386 and host sanitizers. Host alignment sanitizing is specifically
excluded: the existing four-byte stream header precedes eight-byte host pointers;
the production i386 stream is naturally aligned. Geometry, stream capacity,
sentinels and required callback replacements obey the reference contract.

Recover `SmartTextEx`'s eight-line fixed-buffer layout, explicit newlines,
adaptive break helpers, uncapped zero line limit, horizontal squeeze,
measurement/box-only behavior, signed box alpha, live font selection and global
state restoration (**17.631% to 33.673%**). Preserve existing break-helper
ownership and C++ linkage. Both actual-body fixtures pass 10,000 valid randomized
cases plus targeted callback/box/layout probes against a separate layout oracle.
Nine generated reference-invalid negative-copy-span cases were excluded before
candidate execution. Decoded/encoded lengths are bounded to 511 and layout to
eight lines; malformed input and nonfinite integer conversions are not claimed
safe. Box reserved/Z fields and early-out global state remain untouched.

`DrawAlphaImage` restores `(1 + coordinate) - 1` rounding, paired direction-Y
snapshot, pre-begin strip row and original integer color packing
(**27.794% to 30.209%**). Actual-body host sanitizer/i386 fixtures pass finite
sampling, aspect mutation, recursive begin, half-UV and state-restoration probes.

For size-deficit triage, count compiler `.part`/`.isra`/constant-propagated bodies
before assuming missing code: `Text3DEx`'s 301-byte entry also has a 2,725-byte
partition. Complete low-score movement, curve, AI and menu bodies are bounded
audits, not invitations to inflate source. Zero-gain cutscene/font/collision
correctness proposals are held separately. New compiler attributes, ABI
shortcuts, ownership changes and scoring changes are not used. All mocked-service
fixtures are diagnostics, not full Android gameplay validation.

## Batches 190–199: state, spawn, pipeline and loader closures

The first commit `27dbd17b` on PR #121 reaches **67.216774%** and passes all
eleven GitHub checks. Subsequent integrated changes reach **67.254760%**,
**+0.126685 percentage points** from this cycle's main baseline, with **6,276**
exact functions. The next merge threshold remains **69.128075%**.

`Action_Kill` restores embedded parameter parsing, null-opponent selection
preservation and stable object walkers across kill callbacks
(**26.319360% to 28.767466%**). Actual-body host sanitizer/i386 fixtures each
pass 773 cases. Its original larger body contains loop-unswitched copies;
those are not recreated merely to inflate source. `Action_SetSide` restores
owner IN_USE/objptr selection, byte-narrowed type sentinel and callback-before-
capacity ordering, substring parameters, positive-range/NaN dispatch and
null-anchor zero distance. It retains the original `NuVecSub` callback and
XYZ arithmetic association, and clears alert state without clearing the firing
target (**31.597803% to 46.384617%**). Both actual-body fixtures pass 134 cases.
Its full-TU audit includes four small collateral regressions; the weighted
gain remains positive. Existing helper ownership is unchanged.

`Players_InitPositions` corrects bonus second-player Z indices and the original
`mission_door`/`door_to_network` literals (**35.538% to 35.792%**). Preserve the
retail first-player-X subtraction rather than repairing that quirk. All 67
actual-body probes pass both ABIs. `ResetPlayer` restores forced next-sock
setup, gunship and Coruscant spawn paths, co-op signed segment offsets, the
specialized vector-rotation call, hover-sentinel reuse and vehicle narrow-sock/
speed/velocity initialization (**41.498% to 57.270%**). Actual-body fixtures
using unchanged vector math pass 3,522 cases on host sanitizers/i386. Signed
player slots, missing sock systems, callbacks and incomplete sockets are covered;
these mocked-service tests are not complete gameplay integration.

`EdManipulator::SelectRotator` computes hover plane/angle before the second
press query (**24.817772% to 25.822289%**). The exact production body passes
i386 fixtures. Host sanitizer probes are explicitly a fixed-32-bit state-view
simulation inside allocated opaque storage, not native 64-bit ABI proof.
The class's pre-existing hardcoded-offset representation is not broadly rewritten
to accommodate the fixture. `pathEditor_Process` restores two original local
nearest-node scans (**28.173334% to 31.663225%**); both ABI fixtures pass runtime/
current/all-path selection, height/NaN gates and callback ownership. One genuine
unrestricted helper call remains, retaining the retail generic helper symbol.
Three-scan symbol-loss and fixed-connection expansion regressions were rejected.

`Customiser_LoadAll` permits non-null empty category names and reloads category
fields after string/scene callbacks (**44.834% to 47.186%**). Both actual-body
fixtures pass 10,000 cases against an independent allocation/resource model.
Paths/counts/arena capacities are bounded; callback replacement cannot leave
null/dangling categories. Tiny neighboring register-allocation regressions are
included in the net gain. Nine explicit allocation blocks explain much of its
remaining size deficit; no manual expansion was introduced.

`NuPostEffectRender` queries all five attachments and resolves multisampling
before publishing proxies; filter globals remain live across `isEnabled`
callbacks, while attachment uses the original entry colour snapshot
(**28.369% to 57.276%**). Canonical virtual-mock actual-body host sanitizer/i386
fixtures independently distinguish publication, filter-selection and colour-
snapshot corrections. Render/reset, disabled filters, duplicate depth attachment
queries, padding and shader cleanup are also covered.

Recover `ScanTerrainPlatform`'s separate translating/rotating traversals,
scratch-backed transforms, source/material filtering and three/four-vertex normal
reconstruction (**12.600619% to 17.897833%**). Raw SSE proves all unordered motion
axes select non-positive bounds; the decompiler's ordered `<=0` interpretation
was rejected. Restore grouped squared-distance sums before radius addition;
the old association fails a bitwise service-argument probe. Actual-body fixtures
pass 4,096 cases plus four full-capacity modes on fully sanitized host/i386.
Unlike the previous embedded-stream fixture, this header uses pointer-sized
slots and needs no host alignment exclusion. The reconstruction-only static
`TerrainScanPlatformGroup` naturally specializes after losing this caller; it
has no retail counterpart. Retail public definitions remain intact. Collateral
`ScanTerrain` and `NewScanHandelFull` regressions are counted in the positive
whole-TU result; an exploratory forward-declaration replacement error in a
temporary copy was caught by the symbol audit and discarded before integration.

`GizSpinner_Load` restores unconditional name reads, legacy special fallback and
frame reads, first-primary absolute duration, version-3 state/type preservation,
live animation-set lookups and valid old-point eligibility. Bounds add the arm
minimum correctly and retain common extent initialization with the original
half-extent constant (**23.218950% to 28.259924%**). Actual-body host sanitizer/
i386 fixtures pass versions 1–9, missing resources, negative/zero durations,
bounded frame conversion, set replacement and callback order. Nonfinite
float-to-integer frame conversion is outside the valid fixture contract.

Bounded complete-body audits of `cbPtlColMenu`, `DrawRopeSingle`,
`DrawBuildUpBar`, `Door_FindByIndex`, `Doors_Configure` and `DookuC_Update`
found no missing substantive closure. Original fixed-call fanout, embedded
matrix math or duplicated paths explain their size deficits. No source inflation,
forced inlining, compiler-option or ownership changes are used to chase those
shapes. The cycle continues; merge requires all checks on its eventual final head.

## Batches 200–206: race, parser, cast, reset and store closures

The integrated target report reaches **67.301920%**, **+0.173845 percentage
points** from PR #121's **67.128075%** main baseline, still **6,276** exact
functions. The merge threshold remains **69.128075%**; isolated function gains
are not aggregate percentage-point gains.

`PodRaceSnipersUpdate` restores both-player strict range selection, the cached
bolt type, unconditional timer progress, alternating target/intercept selection,
random fallback and full-distance yaw/pitch (**36.549618% to 55.358780%**).
Preserve the retail range-origin/bolt-position distinction and local player
index. Actual-body sanitizer/i386 fixtures pass; interception and random-angle
services are instrumented deterministic mocks, not full physics validation.
The aligned matrix uses the existing canonical aligned type, not a new ABI
or code-generation attribute.

`CutScenes_Load` restores its parser closure (**25.069021% to 29.020866%**):
override callback, lowercase name handling, signed character-count division,
typed flags, level bypass and failure rollback. Raw allocation proves `CUTINFO`
is **0x194**, not 0x198: remove the unused trailing field and its sole
configuration write, retaining all established offsets. The final pointer-table
cursor advances by count-squared pointer slots, as the original multiply proves.
Actual `Configure`/`Load` bodies pass 20,000 bounded allocation/parser cases on
host sanitizers/i386. `CutScene_Configure`'s small score regression is counted.
The separate configured-list placeholder remains deferred: its full trial
regressed. No claim of complete configured-list or malformed-count support.

`NewCast` recovers separate first/second plane paths, quad edge tests, zero-normal
fallback, ordinary negative-roof sign and extended blocker behavior, cast-index
publication and the full result epilogue (**10.449809% to 18.155020%**).
Actual-body fixtures compare outputs and `InsideLineF` traces in 8,192 cases on
fully sanitized host/i386. Existing reconstruction-only shadow-triangle helper
specialization naturally disappears; there is no retail symbol to preserve for
that helper. Public/retail-private definitions and compiler options stay intact.

`PreResetCode` replaces seven zero constant placeholders with verified retail
values and corrects the context-not-ten flag gate (**65.965% to 80.296%**).
Use canonical members instead of fixed target offsets, naming the existing
0xdb8 float through a padding-preserving union and target offset assertion.
Existing aligned vector scratch matches raw stack alignment. Actual-body
fixtures with production RNG/vector/trig math pass 4,128 cases on host
sanitizers/i386, including complete object/RNG state, ledge and action boundaries.
Do not reinterpret the valid trig-table threshold indices as angle mistakes.

`Action_CnxController` excludes `on_frames` from creation parsing, explicitly
resets OPEN state for CLOSED options and restores target/animation/output
substring precedence (**53.785862% to 68.727650%**). Both ABI fixtures pass
350 actual-body cases; only this function changes among 416 normalized TU
functions. Frame float-to-integer behavior and existing fallback guards remain.

`MenuDrawStore` restores nested panels, alpha snapshots, 0.15 bundle spacing,
green owned overlays, selected-character touch records, signed low-word title
indices, retained widths, description-box dimensions and localized savings
layout (**32.906% to 52.667%**). Both ABI and host sanitizer actual-body fixtures
pass 10,000 callback/layout cases plus 1,000 valid low-word title-index cases.
Successful SDK responses must initialize price; valid nonempty text/output-box
services and bounded strings are required, as in the original. Retail ordered
price comparisons and repeated-addition geometry are retained. Neighbor scores
are unchanged; full Android purchase/render integration is not claimed.

`PodSprintA_Update`'s focused boulder unit corrects typed area-base subtraction,
retail sign-extended high-word mask and canonical object traversal/member access
(**46.530910% to 48.216362%**). Host sanitizer/i386 actual-body probes cover all
32 indices, especially bit-31's all-high-bits quirk. Adjacent scores are unchanged.
This is deliberately partial: offline lap/countdown/audio/GO closures remain
documented defects whose bounded trials regressed, not completed gameplay paths.

Further bounded negative audits reserve the complete dynamic-material correction
(two individually positive parts regress when combined), bomb-generator spawn
script/callback correction and panel-arm sign corrections. Do not replay their
layout variants or present neutral/regressing correctness units as matching
gains. Full-body audits first distinguish missing semantics from compiler fanout,
embedded helpers and register allocation before investing in fixtures.

Batch 207 restores `NuGCutSceneSysRender`'s single entry eligibility/direction
decision, independently sampled stage frames, stable character/locator owners
and live callback/array/count/parent-transform reads (**27.685184% to
48.211113%**). Existing null asset-system guards are retained; the higher-scoring
unguarded experiment is not integrated. Frozen guarded actual-body fixtures pass
20,000 cases on host sanitizers and the unchanged NDK i386 compilation, including
null asset systems and callback mutations. Tiny collateral cutscene-TU score
changes are included. The linked report reaches **67.306340%**, **+0.178265
percentage points** from main, still **6,276** exact functions.

## Batches 208–215: menu transitions, events and persistent handles

Checkpoint `444e0481` is pushed on PR #121 at **67.306340%**, with all eleven
GitHub checks passing on that exact head. Subsequent linked changes reach
**67.346100%**, **+0.218025 percentage points** from main, still **6,276** exact
functions. The cycle target remains **69.128075%**, not yet reached.

`AISysProcess` restores an intermediate antinode-angle store before `NuAngAdd`
(**36.441120% to 37.379440%**). Both ABI actual-body fixtures pass 108 cases.
The boundary is observable to diagnostic interposition; retail `NuAngAdd` is
pure, so this does not establish a retail gameplay failure. This unit is partial:
group-pass ordering and locator ownership repairs regressed together and remain
reserved. Fixed-sixteen group fanout is not recreated merely for compiler shape.

`pathEditorDrawPath` computes equal-radius perpendiculars directly instead of
calling the rotation service (**24.203686% to 24.437439%**). Existing null/index
and positive-distance angle-math safeguards remain. The larger trial removing
the distance guard is superseded: coincident unequal radii lead to original
nonfinite float-to-integer behavior, not a reason to drop a validity safeguard.
Canonical-layout actual-body sanitizer/i386 probes cover equal/unequal radii,
solid paths, routes, shared nodes, deduplication and invalid-index rejection.

`UpdateMenu` restores full input masks, opposing-held suppression, touch/pre-
repeat output snapshots, transition-before-navigation gating, captured bounds,
row-clamp exclusions, directional priority and column-change timer reset
(**16.249% to 42.147%**). Both ABI and host sanitizer full-body fixtures pass
10,000 cases plus 1,000 persistent eight-frame sequences; all neighboring scores
are unchanged. Menu indices, ordered bounds and float/integer conversions must
stay valid. Four original private fast-direction locks have only reads/clears
and no observed nonzero setter/address escape; their zero-state behavior is the
documented scope. Do not invent setters or volatility to retain those branches.

`LoadAreaCharacters` resets touch-prompt eligibility each frame and starts icon
animation on the frame after readiness, without forcing crawl mode to stage two
(**35.406136% to 35.993496%**). Eight full actual-body loading-loop scenarios pass
host sanitizers/i386 with canonical service objects. Preserve the original two
music ticks on sound-enabled frames. Wider second-icon timing corrections
regressed and remain separate; full loader correctness is not claimed. Host
i386 probes use focused canonical assertions, not an unrelated Android pthread
layout override. `Area_Configure` is closure-complete for valid mission counts;
retain its eight-character capacity guard.

`LightSabreComboCode` skips a latched event-one query and freshly queries event
zero at completion (**24.074492% to 35.329570%**). The combined positive unit is
retained even though a partial guard-only trial scores higher. Actual full-body
fixtures exercise the changed queue/completion paths in 1,792 cases on host
sanitizers and the unchanged NDK i386 build; unchanged starter/draw/block paths
are not covered by those focused probes. All other 180 defined functions are
unchanged. Separate Punch/Jedi corrections regress and are reserved.

`Pulses_Update` restores live pulse-slot reloads after services, once-per-pulse
direction setup and a radius snapshot across both players (**25.871622% to
31.520270%**). Actual-body host sanitizer/i386 probes cover array replacement,
cached player ownership, lifecycle, velocity, rumble and live count changes.
Raw branches reject unordered flicker/radius inputs but permit unordered spawn
protection and mode-zero halfspaces; do not apply uniform NaN-policy cleanup.
Preserve the original in-place initial direction rotation quirk.

`NewScanHandelFull` now explicitly requests unscaled persistent-handle geometry
from the existing reconstruction-only platform helper (**9.887054% to
11.846221%**); ordinary query scans request the existing scaled mode.
`ScanTerrain` also improves (**23.714680% to 24.445364%**), and the other 123
terrain function scores are unchanged. There are no retention-only calls.
Actual-body host sanitizer/i386 fixtures pass 8,192 streams plus boundary/wall
probes. Unit scale, origin Y=16,777,216 and transformed Y=1 previously rounded
to zero; direct handle copying correctly retains one. The safe owned wall-copy
tail is preserved, and no skin-allocation origin-mutation claim is made.

`LSW_Hub_InitAI` stops at the first incomplete counter pair and creates Area
before ServingCustomer; `Hub_MakeListCharactersAvailable` snapshots the signed
character ID before the pack query (**36.893616% to 38.216312%**, **32.081272%
to 35.611310%**). Both ABI actual-body fixtures pass 601 cases. A small UpdateAI
register-allocation regression is included in the net gain. Original fixed
counter/pack expansion is not manually reproduced.

Reserved full portal-debug and Huffman-tree restorations currently reduce the
weighted matching result under unchanged compilation. Retail lacks STT_FILE
and debug provenance: combined-core constructor adjacency versus remote refpack
placement supports a translation-unit provenance audit but does not prove a
specific original included file or authorize optimization/ownership shortcuts.
The cycle continues; its final merge still requires the target and green checks.

## Batch 216: debris scheduling closure

`DebrisProcessGeneration` restores unconditional emission-epoch publication after
each attempted generation, persistent pause state across attempts, and late
cut-on/render-group reloads after service callbacks (**47.442280% to
48.262367%**). Existing allocation and attempt bounds remain; canonical named
fields require no layout change. All other original-backed TU scores are
unchanged. Exact-body host sanitizer/i386 fixtures pass seven scenarios,
including inactive/no-chunk scheduling, finite pause cancellation, generator
epoch overwrite and callback-driven reactivation gates. Services are mocked;
the reload tests establish the reference contract, not that every current
production callback mutates those fields. Linked matching reaches **67.346690%**,
**+0.218615 percentage points** from main, still **6,276** exact functions.

## Batches 217–219: loader ownership and query cache closure

`AISysLoadEx` always allocates the zero-node route pointer array, allocates
serialized route names at their exact length (node names still need length+1),
and finalizes distance-cache sentinels after node parsing and special lookup
(**26.411840% to 36.168600%**). All neighboring scores remain unchanged.
Canonical actual-body host sanitizer/i386 fixtures pass 729 cases across
versions 1–20, paths, routes, allocator requests, callback state and load tails.
The fixture records requested sizes using host-width pointers; it is not a
64-bit execution of the target allocator's 32-bit cursor contract. Other
creature/area/locator helpers execute empty-record paths, not exhaustive records.

`instNuGCutSceneCreate` caches the character asset subsystem across callbacks,
keeps its arrays/count live, publishes the constructed subsystem after the
walker, and reads bounds through the live instance asset owner (**21.022058%
to 23.117647%**). Both ABI actual-body fixtures pass 20,000 cases, including
unchanged production NDK i386 compilation. Original-valid arena capacities and
counts remain required. Pointer-bearing locator format eight is covered on
i386 and explicitly excluded on the host: its pre-existing raw payload offsets
are not 64-bit safe. Thread/create services are mocked; no gameplay claim.

`NewScan` fills its persistent static cache independently of material mask,
restores integer argument roles without changing their types or public ABI,
walks the complete reserved platform cell and adds platform motion to world
bounds before subtracting origin (**18.939339% to 21.600183%**). The direct
original-backed cache walk replaces a reconstruction-only generic helper call;
fixed sixteen-entry cache lookup fanout is not manually expanded. Two small
ray-cast improvements and a TerrainPlatformEmbedded regression are included in
the positive whole-TU result. Host sanitizer/i386 full actual-body fixtures pass
8,197 calls with independent failing regressions for all four repaired behaviors,
including changing masks, stale active platforms and finite large-origin math.

`LoadPerm` rechecks language count after the language service (**40.552258% to
40.612904%**). Both ABI and host sanitizer fixtures pass eight setup/cleanup
scenarios. Their stop boundary precedes unrelated legal rendering conversion;
this is not complete loading-loop validation. The verified long-frame clamp
and signed legal-alpha corrections regress and remain reserved.

`PushCode` restores collision-center pointers, spinner/obstacle admission,
updated-timer gates, wall-jump flag/override/duration, obstacle steering and
sampled state, expiry camera blend and mutually exclusive landing/grunt audio.
It remains **0%**, with all eleven neighbors unchanged and no linked regression.
This unit is a behavior repair, not a claimed fuzzy gain. Canonical host
sanitizer and unchanged GCC 4.7 i386 actual-body fixtures pass 2,816 cases and
20,589 assertions, using real vector/RNG dependencies. All changed branches are
covered; unchanged lunge, jump-exit and failure-cooldown paths are not. Existing
compiler attributes are unchanged. The removed action-sentinel gates did not
guard an array access and incorrectly rejected original scripted allow_push=0
entry; the candidate fixture covers that absent-animation contract.

Linked matching reaches **67.363785%**, **+0.235710 percentage points** from main,
still **6,276** exact functions. Reserved ordinary `RenderFileSel3` restoration
does not naturally emit the retail private clone, and both Timed5 trials
regress; no naming/attribute/optimization shortcut is accepted. The cycle still
requires **69.128075%** and all final-head GitHub checks before merging.

## Batches 220–221: byte opacity, message queues and script state

`AllMiniKits_LSW_Draw` restores signed float-to-i32 truncation followed by
original low-byte opacity (**0% to 60.086807%**). The existing status-alpha helper
can return values above one; passing an unrestricted i32 did not match the
original mask. No other TU body changes. Exact production-body host sanitizer
and i386 SSE fixtures pass count/opacity cases, thirteen timing thresholds and
10,000 timing samples, including callback area replacement. The initial i386
diagnostic used x87 arithmetic and failed a strict float assertion; rebuilding
with the target's SSE arithmetic passes without weakening assertions or
changing production flags. Conversions remain finite and i32-representable.

`AddGameMsg` restores queue protection mask **0x1000**, rather than **0x10**,
and constructs a temporary initialized message for its end callback when the
queue is exhausted and incoming field_4d is nonzero (**15.094% to 30.306%**).
Reference-ordered initialization, captured callback and zero elapsed/duration
selection are retained; all neighboring scores are unchanged. The existing
field_4d-zero exhausted-queue NULL guard stays: retail instead overwrites its
current protected cursor slot, not an invalid minus-one slot. Independent
queue/callback state-machine tests pass 10,000 cases on host sanitizer/i386 SSE,
comparing the full queue, input bytes, cursor and callback payloads. They cover
1,111 temporary callbacks and 1,978 total callbacks per run. Callback recipients
must not retain the ephemeral pointer; inputs must not alias queue destinations.
Finite nonnegative durations, valid pointers/cursor and buffers are required;
no nonfinite policy or full Android message rendering validation is claimed.

`Action_SetScriptState` restores case-insensitive substring parameter matching,
retains the default target when no named-object service is registered, reloads
its processor after state lookup, and walks the cached object base with a live
object count (**9.621622% to 75.221620%**). Existing prelookup guards remain;
a postlookup null guard safely handles callback invalidation, which retail
assumes cannot happen. Six tiny scratch-register collateral changes include a
NotWithParty regression in the positive linked result. Canonical actual-body
host sanitizer/i386 fixtures pass 187 cases and four independent old-body
regressions; lookup/parser/init services are mocked.

Linked matching reaches **67.396140%**, **+0.268065 percentage points** from main,
still **6,276** exact functions. Full Teleport/EngageOpponent/ledge/SkinPlatformSize
audits found no substantive missing closure; no ownership/optimization/fanout
experiment is repeated. The callback-owner cutscene Update trial and proximity
gizmo trial are reserved after their bounded negative/neutral gates. BoxTree's
514-byte missing closure still requires a distinct context type, original public
signature and resolution of its documented cross-owner private-ABI boundary;
it is not repaired with an attribute, alias or speculative caller hookup.

## Batches 222–223: super-story time and retail-width lean

`SuperStoryTime_LSW_Draw` restores its missing active-stage drawing body: coin
entry/title-exit fades, live post-service best-time state, interpolation, time
formatting and previous-record parentheses. Its public wrapper improves
**38.181820% to 100%**; GCC independently emits a private clone. No private
alias, register attribute or compiler option forces that split. One tiny
TrueHero improvement and equal-score neighbor drifts are included. Canonical
host sanitizer/i386 SSE actual-body fixtures pass 10,000 finite timer samples,
22 threshold/record cases and guard/callback probes. Formatter/drawing/string
services are mocked; valid finite i32-representable conversions and bounded
formatter output remain required. The private clone is not claimed exact.

`TurnCode` explicitly truncates lean floats to i32 before narrowing to i16,
matching both original low-word stores. A valid phase yields **36,408**, outside
i16 range; the old direct float-to-short conversion fails float-cast sanitizing.
Host sanitizer and unchanged NDK i386 actual-body fixtures pass 1,500 cases and
4,875 checks, including 104 wrapped lean values. The bounded oracle masks the
low word explicitly. All 181 owner text bodies remain byte-identical, and the
linked score is unchanged: this is a portability repair, not a fuzzy gain.

Linked matching reaches **67.396600%**, **+0.268525 percentage points** from main,
with **6,277** exact functions.
Checkpoint `41a89dd7` has all eleven final-head GitHub checks passing; the cycle
continues below its required **69.128075%** merge threshold.

## Batches 224–225: super-story score and local AI parser contracts

`SuperStoryScore_LSW_Draw` restores its missing active-stage title, interpolated
score, previous-record parentheses and coin-total drawing. Its public wrapper
improves **35% to 100%**, while Time remains at **100%**. GCC naturally emits
the private score body; no clone name, attribute or compiler option forces it.
The original high/low-word unsigned score conversion can round UINT_MAX to
2^32 in float; an explicit endpoint guard preserves the hardware's zero result
without an undefined float-to-u32 conversion. Post-title timer/score reloads,
post-text previous score and total-pointer reloads, signed opacity then byte
narrowing, and the original unordered post-callback timer branch are retained.
Canonical actual-body host sanitizer/i386 SSE fixtures pass 20,000 samples,
phase/full-u32 boundary cases and four callback probes, including a finite
entry timer replaced with NaN after title drawing. Formatter and draw services
are mocked; finite i32-representable opacity and bounded formatter output remain
required. All other original-backed owner scores remain unchanged.

`GizmoBlowupEarlyUpdate` restores override/type reloads after existence,
midpoint, animation, blowup and debris services, and clears delayed activation
for unordered/nonpositive animation time (**29.156977% to 30.017443%** in the
paired isolated gate). Existing guards and cursor/live-count behavior remain.
Focused actual-body host sanitizer and unchanged NDK i386 fixtures pass
callback/lifecycle probes and 1,000 timer cases. Services are mocked; callback
mutation tests establish the reference contract, not known retail gameplay
mutations. All other original-backed owner scores remain unchanged in the
linked before/after report. Unresolved radius ordering is not rewritten.

`Action_SetPath` restores case-insensitive substring parsing, the original
unvalidated separator after `path`, a cached path-system owner for named paths,
and live successful path-pointer reloads (**22.500% to 46.182434%**).
`Action_CircleLocator` restores matched-pointer offsets for embedded name,
teleport and goalrange tokens (**29.582857% to 54.514286%**). The unusual
teleport offset is original behavior, not reinterpreted as a new command.
Shared `ActionParamValue`, callback priority and existing validity guards stay.
Exact production-body host sanitizer/i386 SSE fixtures pass 356 cases, including
embedded tokens, colon separators, service owner replacement, live arrays,
direction flags and bit-preserving float parameters. Small neighboring
scratch-register regressions are included in the positive whole-TU gate.

The ScriptState integration review found that its prior shared helper tolerated
NULL parameter entries, whereas `NuStrIStr` dereferences its input. A minimal
local NULL-entry guard preserves that safety behavior despite reducing the
paired ScriptState score **75.221620% to 68.145940%**; it still improves on the
original **9.621622%** baseline. Both ABI actual-body fixtures pass 191 cases;
the exact old body passes mixed-NULL probes and the unguarded intermediate
reproduces a sanitizer failure. No other owner score changes in this guard's
isolated gate. SetPath/CircleLocator already required non-NULL parameters before
their parser correction; no broader helper or parameter-array policy changes.

## Batch 226: coin display position and byte opacity

`Coins_LSW_Draw` captures the score X position before icon/formatting services
and explicitly narrows signed i32 opacity to its original low byte
(**53.559376% to 55.146873%**). Every neighboring score is unchanged in the
paired owner gate. Exact production body plus pure production math helpers
pass 20,000 independent trace/state cases on host sanitizers and i386 SSE,
including 4,108 prior-body capture differences on its defined conversion domain.
Draw/formatter boundaries are mocked, with live scale/score/title reads and
cached position/opacity tested. Finite i32-representable conversion inputs,
positive phase-one/three durations, valid pointers and bounded formatter output
remain required. A separate old-body sanitizer probe rejects direct conversion
of opacity 512 to u8; no clamping or invented nonfinite conversion policy is added.

The final combined checkpoint, including ScriptState's safety guard, reaches
**67.402695%**, **+0.274620 percentage points** from main, with **6,278** exact
functions. Target/native builds and all five repository checks pass. The cycle
continues below **69.128075%**; a green checkpoint does not authorize early merge.

## Batch 227: particle-editor callback boundaries

`edppDrawCursor` restores first-marker endpoint capture before second-endpoint
rotations, live nearest-particle selection across coordinate/font and sphere
services, warning-count reload after color selection, and independent copy-mode
checks between type, clipboard and highlight drawing. Its existing lambdas,
effect-owner snapshot and guards remain; no fixed geometry fanout is expanded.
The linked function improves **23.548721% to 34.694542%**. In the isolated
whole-owner gate it gains approximately **834** original-weighted matching bytes,
with no neighboring score changes or loss of the 74 exact owner functions.
Canonical actual-body host sanitizer/i386 SSE fixtures pass 104 scenarios,
including six independent prior-body failures, stable readout/mode combinations
and signed particle-count rounding. Math/font/render/helper services are mocked
with identity rotations: these tests prove sequencing, not that production math
services mutate editor globals or that interactive editor visuals are validated.
Valid service-selected particle indices and existing buffer contracts remain.

Linked matching reaches **67.420290%**, **+0.292215 percentage points** from main,
with **6,278** exact functions. The target build passes; final native/check and
GitHub validation belong to the next combined checkpoint.

### Closed bounded audits: avoid repeating source-shape trials

- `ANI_SimpleAni3PlayerV4Joint_Quat3` and `Quat3W`: complete supported-format,
  cursor, quaternion-W/sign and scale/min decoding already present. Raw quarter-3
  instructions scale the tangent before multiplying the delta, exactly as the
  current helpers do. A decompiler-derived reassociation suspicion was rejected
  before trial. NaN-to-cursor and negative joint ranges are outside valid input;
  do not introduce unrolling, forced inlining or alignment tricks to chase the
  remaining register/stack differences.
- `ZipUp_MoveCode`: full state/service closure already present. Its one raw-backed
  threshold distinction (`0.01f` versus `0.1f * 0.1f`) is matching-neutral in
  the bounded gate, **19.677511%** before/after, with all 27 neighbors unchanged;
  it is reserved without runtime fixtures. Retail's inline inverse-trig
  polynomial is not a missing service or permission to expand helper ownership.
- `SuperCarry_MoveCode`: complete states zero through seven and all 36 services
  reviewed; apparent jump-setup fallthrough and retained blowup owner are retail
  behavior. No substantive missing closure or source trial found.
- `NuMemoryPool::ReleaseUnreferencedPages_OLD`: the earlier rejection/self-link
  safety finding still applies. The 508-byte retail versus nine-byte stub gap
  does not justify repeating that already-deferred unsafe restoration.
- `CutScene_DrawCharacter`: the earlier complete callback/slot/shadow/locator
  closure trial remains **0%** (6,340 to 6,489 current bytes versus 6,588 retail),
  despite its recorded 20,000-case host/i386 behavior probes. The exact local
  entry is **0x498db0**. That unresolved private-ABI/codegen boundary is reserved;
  neither a fresh export nor another callback-only trial is justified without
  new evidence. The source patch remains unintegrated, not claimed complete.

## Batch 228: minikit-detector projection and flags

`GameMsg_Draw_MiniKitDetector` restores signed screen-edge snapping after
behind-camera negation, green/blue only for zero behind-camera state, and
model-opacity flag **0x10000** instead of bit one. The existing symbol declaration,
visibility and source owner are unchanged. Its linked match improves
**13.968085% to 32.393616%**; neighboring original scores remain unchanged in
the isolated owner gate. Exact production-body host sanitizer/i386 SSE fixtures
pass 20,000 independent projection/message/trace cases, including 14,069
prior-body differences, finite clipping ties, unsigned camera-range thresholds,
all relevant flag/behind states, model/character/text branches and callback
reloads. Services are mocked; canonical real helper implementations were
inspected but no full rendering test is claimed. Valid signed-nonnegative model
indices, pointers, positive camera range and bounded arrow text are required.

Linked matching reaches **67.425590%**, **+0.297515 percentage points** from main,
still **6,278** exact functions. Pushed checkpoint `4800416c` has all eleven
GitHub checks successful on that exact head; the newer local unit continues
below the required **69.128075%** merge threshold.

## Batch 229: streaming-audio error and context publication

`NuSoundStreamingSample::Open` closes an empty first stream before its single
buffer-context publication and routes empty-data loader result four through the
same original error translation as ordinary open failures. Sample error is two,
not four. Its linked match improves **9.898255% to 15.947675%**; neighboring
`ReCue` improves slightly (**61.496773% to 61.851612%**), while all other owner
bodies remain unchanged in the isolated gate. Canonical headers, 64-bit size
tests, buffer/descriptor ownership, allocation cleanup and existing context
initialization are unchanged; GCC naturally adds its switch-value table.
Exact production-body host sanitizer and unchanged NDK i386 fixtures pass all
first-buffer flag bytes, high/low u64 sizes, mapped errors, preloaded data,
second-fill count changes, close-time buffer replacement, allocation failures,
null headers and early exits. The original `CloseStream` wrapper dispatches
virtual `Close`; the diagnostic loader observes the previously published context.
The old body fails that observation. Services are mocked and allocation success
means valid storage; no device playback, real files or concurrent streamer
validation is claimed.

Linked matching reaches **67.427530%**, **+0.299455 percentage points** from main,
still **6,278** exact functions. The integrated target/native builds and all
five repository checks pass.

`MechHintUIButton::Process` is also closed after full reference/raw review: its
hint transition, captured pulse state, slide interpolation, visibility and
two separate menu queries are already present. Its fresh unchanged owner
baseline is **4.391167%**; no source-shape trial or runtime fixture is justified
by that low score alone.

## Batch 230: deferred music restoration

`SoundUpdate` permits deferred restoration when its updated active-state counter
reaches 25–64, while retaining preseek admission only above 64 and the live
counter check after preseek. Its linked match improves **4.042553% to
7.868085%**; all other 87 owner units remain unchanged in the isolated gate.
This is observable with the production `RestoreGameMusic` helper, which requests
deferred resume below 25, and `MusicPreSeek`, which resets the frame counter.
Frozen actual SoundUpdate and both actual helper bodies pass 5,115 cases on
host sanitizers and i386 SSE. Audio services record calls without mutating Music;
the old body fails the independent real-helper frame-24-to-25 resume assertion.
Active, waiting, transition and linked states, disabled audio, boundaries and
safe nonfinite comparisons are covered. Valid track indices/pointers and
representable volume conversions remain required; no device playback test.

Linked matching reaches **67.428320%**, **+0.300245 percentage points** from main,
still **6,278** exact functions. A fresh main fetch confirms the baseline remains
`2a57d7b7`; no upstream rebase is needed at this checkpoint.

## Batch 231: ordered dodge comparisons

`DodgeCode` preserves the original unordered outcomes at entry-speed admission,
active-timer expiration and side-direction selection. Only three comparisons
change; ABI, services, finite behavior and safety guards remain unchanged.
Its linked match improves **1.987124% to 4.287554%**. The other 180 owner
functions remain byte-identical in the isolated gate, including prior turn,
shooting and lightsabre repairs. Exact production-body host sanitizer fixtures
pass **4,840 cases / 35,063 checks**, and the unchanged NDK i386 fixture passes.
The untouched body fails 1,715 unordered-path assertions but passes the complete
finite-only oracle. Rotation uses the actual helper and real trig initialization;
gameplay services are bounded stand-ins, not full game integration. No calling-
convention attribute or nonfinite-to-integer conversion is introduced.

Linked matching reaches **67.428894%**, **+0.300819 percentage points** from main,
with **6,278** exact functions. Target/native builds and all five repository
checks pass for the combined batches 227–231 checkpoint.

### Additional closed gates after batch 231

`NuSoundVoice::Play` already preserves both state queries, live initial-buffer
limits, source/flag reloads, temporary weak-pointer lifetime and hardware service
order. Full reference/raw review finds no missing closure at **20.258883%**
linked matching. Keep existing weak-pointer null guards; do not repeat helper
duplication, vtable reinterpretation or instruction-layout trials.

`eduicbRenderProp` has real margin/value-anchor, edit-material, measurement-query
and host-safe property-flag discrepancies. One combined unchanged-options trial
falls **15.641960% to 0%** in the original-side object gate and loses **587.26
original-weighted bytes** net across its owner. It remains incomplete and reserved,
not integrated or fixture-verified. No partial matching-only variant was retained.

`NuSound3Update` has real forward-list service-order and stereo-start/retirement
discrepancies. One unchanged-options trial falls **16.419268% to 12.177748%**;
all other comparable runtime bodies remain unchanged. The request-table copy
direction was already correct, not a repair. Safe null-sample cleanup and early
unlink remain; no shared weak-pointer/list rewrite or fixture run follows the
negative gate. The evidence patch is reserved, not a completed restoration.

`Action_SetUseOneAtOnce` has caller-specific substring/default-target/myself
reload and live world-AI lookup differences from its shared parser. A bounded
local restoration retaining null-entry/owner guards falls **13.621429% to
9.321428%**, with a **15.23-byte net original-weighted loss** across the owner.
It remains reserved without fixtures or integration, not a faithful-completion
claim. Do not retry it as parser inlining or arbitrary source-shape inflation.

`NuGetVertexDeclaration` is closed after complete raw and 561-line reference
review: cache bounds, packed precedence, all thirteen attributes, float/half
cases, signed offsets and strides already agree. Its **36.791046%** linked
score and size gap do not justify copying retail's fixed branch fanout. No
source trial or fixture was needed.

## Batch 232: force-push selection

`FindForcePushTarget` excludes the Emperor restriction when the area is null,
rejects unordered distance/direction comparisons and gives animation 0x2b
priority over slot five. Existing style selection and service ownership remain.
Linked matching improves **14.659259% to 17.069630%**; the other eleven owner
functions remain byte-identical in the isolated gate. Frozen actual-body host
sanitizer and NDK i386 fixtures pass **8,969 cases / 187,093 checks**; the prior
body fails 564 independent assertions. Real distance and random helpers are
used; gameplay services are mocks. Fixtures cover valid contexts/animation
matrices, area/player/hostility filters, finite/nonfinite comparisons, nearest
ties and activation order. Diagnostic context storage accommodates signed-byte
indices; no full combat/physics validation is claimed.

Linked matching reaches **67.430430%**, **+0.302355 percentage points** from main,
still **6,278** exact functions.

## Batch 233: thermal-detonator shadow and camera update

`PartUpdate_ThermalDetonator` restores the missing inactive shadow/reflection,
layer/material flags and camera-socket damping. Its beep uses the countdown
at field 0x100, not the elapsed field at 0xf4. The camera gate uses socket
byte one, not the unrelated mode field. Linked matching improves **12.520661%
to 67.256195%**; the neighboring throw score remains unchanged. Keep the null-
part guard, add null environment guards and exclude retail's unsafe surface -1
array access. Shadow heights retain their original service-capture timing.
Frozen actual-body host sanitizer and i386 SSE fixtures pass **27,207 cases**,
including countdown/elapsed independence, all flag bytes, surface/layer bounds,
captured/live shadow fields and X-before-Z damping through the actual SeekValF
helper. Two independent prior-body assertions fail. Diagnostic service mutation
does not claim pure ShadowInfo/EShadowInfo perform those mutations in production;
no real terrain/render/device validation is claimed.

Linked matching reaches **67.437130%**, **+0.309055 percentage points** from main,
still **6,278** exact functions.

## Batch 234: backdrop opacity closure

`BackDrop_Alpha` restores title/credits multiplication and status-stage/menu
fade assignment, replacing unrelated colour-black/wait conditions. It observes
menu state after GetMenuID, returns zero for unordered fade time and preserves
negative-time extrapolation rather than clamping. Existing null-alpha safety
and new null-level/stage guards remain. Linked matching improves **19.094118%
to 39.670590%**; small colour/draw collateral changes are counted in the net
gain. Frozen actual-body host sanitizer and i386 SSE fixtures pass **10,280
cases**; the previous body fails an independent opacity assertion. Canonical
32-bit layout assertions are enabled in the i386 fixture; no full UI rendering
test is claimed.

Combined linked matching reaches **67.438620%**, **+0.310545 percentage points**
from main, with **6,278** exact functions. Target/native builds and all five
repository checks pass. Checkpoint `4821c894` has all eleven GitHub checks
successful on that exact head; these newer local changes await the next push.

The backdrop i386 fixture uses host GCC 16.2.1 with 32-bit SSE arithmetic;
the production matching build and isolated whole-TU gate use unchanged NDK
r8e GCC 4.7. Do not describe that diagnostic fixture as an NDK runtime test.

Further complete reference/raw audits close `eduicbProcessTextPick`,
`Attracto_MoveCode` and `ObjHitObj` without trials: their supported state/service
closure is already present. Text-picker deletion/insertion size differences
come mainly from retail vectorized loops versus current safe copying; invalid
cursor/null states do not justify removing guards. Attractor deposit/suction,
shard selection/ray/collection/rumble and reticle paths are present. Object-hit
flicker/shield unordered comparisons, wrapped damage, debris indices and
AI/arcade/kill service paths were reviewed. Historical documentation calling
ObjHitObj a placeholder is stale. None is claimed runtime-validated by these
evidence-only audits; no shape-only trial or fixture run follows.

`TightRope_MoveCode` and its emitted move-update/attach helpers are also
closed after full reference and raw review. Airborne saved-rope reloads,
landing gates, animation 0x8f timing, explicit jumps, movement clamping and
attachment slope/offset behavior are already present. The repeated literal
label for negative/positive one was disambiguated by raw addresses. No
candidate, compilation trial or runtime-validation claim follows this audit.

### Bounded follow-up gates after batch 234

Do not repeat `Grabber_Update`, `MoveGameCamera` or
`DisplayListLinkDynamicMtls` trials without new evidence: their complete
current owners match previously examined baselines. The grabber's rejected
dispatch/source-shape variants and original-owner merge remain closed. A
dynamic-material reload/publication restoration previously lost **8.41
original-weighted bytes** and remains reserved. Camera raw `UCOMISS`/`JB`
branches seek on unordered values; the suggested negated-less-than blend
guards would incorrectly snap instead. No giant camera reaudit, fresh
compilation or runtime-validation claim follows that bounded triage.

`DebrisProcessControlChunks` has a real pre-lock head snapshot difference.
One combined exact-options clock-predicate/snapshot restoration loses
**627.47 original-weighted bytes** across its owner, with all five exact
functions preserved; it is reserved without fixtures or integration. Clock
selection differs only outside valid indices zero/one for the existing
two-element stack. The actual lock can sleep while occupied; concurrency is
not runtime-validated. Raw expiry admission already rejects unordered values,
despite Ghidra's misleading less-than exit expression. Keep the current
ordered comparison; do not retry fixed key-slot/glass-reset fanout or NaN
admission variants. Other collision/publication differences remain unaudited,
not a claim of exhaustive faithful completion.

`PunchCode`'s missing common gizmo-behind check already has a reserved
original-backed correction, **9.093% to 9.084%** in its earlier isolated gate.
The current body matches that audited baseline. Do not repeat this tiny
negative trial or describe the untested reserve as completed behavior.

`NewScanHandelSubset` is closed after its complete reference and disputed raw
geometry review. Vertex storage starts after six bounds floats: the proposed
x/z-offset discrepancy was a false lead, withdrawn before any trial. Four-axis
walks, translating bounds and rotating squared reach already agree for the
supported geometry contract. Legacy rejected-group cursors/wall-pointer streams
are not permission to break the current safe owned-handle producer/consumer
format. No source trial, score gain or runtime fixture follows this audit.

## Batch 235: guarded portal selection and techno range

`NuPortalWhichRoom` restores pre-insertion candidate-room snapshots, ordered
final plane acceptance and unsigned single-room returns. Inner plane traversal
still continues on unordered distance; portal-side NaN still selects the back
room. A new guard ignores later unused candidate writes, preventing the old
two-slot array overflow after a third match. Linked matching improves
**11.131579% to 35.426315%**; all other 244 scorable owner functions remain
unchanged in the isolated gate. Frozen actual-body host sanitizer and host
i386 SSE fixtures pass **20,031 cases**. Removing only the guard independently
fails on the fourth accepted room. Multiple selected indices must remain below
32768; large unsigned single-room returns use fully allocated room records.
Long accepted streams verify safe selected-pair behavior, not equivalence to
retail scratch-space corruption.

`Techno_MoveCode` uses the raw-verified radius increment **0.25**, replacing
the incorrect **2,000,000**, and reloads its typed control object before target
movement. Its linked score decreases **30.221520% to 28.455696%**; that small
correctness loss is included inside this net-positive combined unit, not hidden
or called an individual matching gain. Actual MoveCode, nearest/readiness/
movement helpers and GameCam_Blend pass **1,622,293 finite cases** on host
sanitizers and an unchanged NDK GCC 4.7 i386 body linked with the host runtime.
The independent raw-bit range oracle covers both strict boundaries and distant
rejection. Camera blending does not mutate the object's control pointer; no
invented callback capability or real gameplay validation is claimed.

Combined linked matching reaches **67.441830%**, **+0.313755 percentage points**
from main, with **6,278** exact functions. Target/native builds and all five
repository checks pass.

## Batch 236: mission and shop spline overrides

`LevelSplines_InitForLevel` restores per-eligible-entry bounty/shop start
selection, two stable cutscene-player availability queries, return-door
suppression, alternate minimum length and start-camera clearing. Overrides
also run when the normal named spline is absent. Keep the null-scene skip and
bound newly indexed start/camera slots. Linked matching improves **33.412060%
to 44.663315%**, with no isolated owner collateral changes. Frozen actual-body
host sanitizer and host i386 SSE fixtures pass **38 cases**; old bounty/shop
assertions fail independently. Availability's actual helper is a pure global
pointer return; no fabricated first-nonnull/second-null sequence is tested.
Valid area indices and allocation storage remain loader preconditions.

Linked matching reaches **67.443650%**, **+0.315575 percentage points** from main.

## Batch 237: thermal-detonator impact closure

`PartImpact_ThermalDetonator` restores special-platform/material attachment,
shadow/water classification, sound-before-active/captured-stop ordering and
brick-impact throttling. Captured extra-shadow height must be ordered above
the live position to splash; unordered brick-wait rejects bounce. Preserve
null-part/material bounds and add a null-WORLD trail guard. Linked matching
improves **34.380283% to 52.014084%**; small throw/mom/water-splash collateral
gains are counted. Frozen actual-body host sanitizer and host i386 SSE fixtures
pass **68,379 cases**, including actual Brick, Stop_Flickerer and math helpers.
The old attachment assertion fails independently. Diagnostic service mutations
only probe capture/reload timing; they do not claim shipped pure shadow helpers
perform those mutations. No real terrain/audio/gameplay validation is claimed.

Linked matching reaches **67.446130%**, **+0.318055 percentage points** from main.

## Batch 238: gradient-picker control and endpoint capture

`eduicbProcessGradPick` gives either left trigger priority over the right
triggers; `eduicbRenderGradPick` retains the endpoint across the render service
and then reloads that endpoint's time/colour/link. Both target scores remain
**0%**. The whole-TU gain is only **0.63272 original-weighted bytes** in
neighboring number rendering; there is no claim of an individual picker score
gain. Frozen actual-body host sanitizer/float-cast-overflow and host i386 SSE
fixtures pass **20,000 process plus 20,000 render cases** each, with 1,190/526
independent old-body divergences. Geometry/conversions are finite and bounded;
nodes remain live/acyclic. Render-service mutation is diagnostic, not a claim
that production rendering edits picker nodes. Config callbacks' registered
identities are checked, not invoked through generated entries.

The combined checkpoint reaches **67.446144%**, **+0.318069 percentage points**
from main, with **6,278** exact functions. Target/native builds and all five
repository checks pass. Original ownership, ABI, compiler options and scoring
are unchanged. Except for the explicitly NDK-compiled techno body, the new
i386 runtime fixtures use host GCC/SSE; production builds and isolated gates
still use unchanged NDK r8e GCC 4.7. A fresh main fetch remains `2a57d7b7`.

### Bounded follow-up gates after batch 238

`MenuUpdateEpisodes` is already closure-complete from batch 95; its current
32.405% agrees with that recorded baseline. `ManageGameObjects` already has
negative pointer-walk/layout reserves. Do not repeat either audit or trial
without new evidence. A bounded `Tag_Check` raw/source audit finds no missing
supported state/service path; eleven retail pack probes versus the current
counted loop explain much of the size difference. No fanout trial or runtime
validation follows this triage.

`TrueHero_LSW_Draw` has a real missing `" - "` title separator. The single
literal-only whole-TU trial changes no instruction/relocation body and gains
zero weighted bytes; it remains reserved without fixtures. Retail's local
text buffer is 252 bytes, not a guessed 256. No resize or NaN-to-int trial is
warranted.

`eduicbRenderTextPick` queries cursor length/prefix/glyph/baseline/height before
the no-draw guard in retail. One bounded service-order restoration loses
**436.64 original-weighted bytes**, with all 31 exact owner functions preserved;
it remains reserved without fixtures or a second shape trial. Actual font
query helpers do not draw or mutate editor state, so this is not a claimed
visual-output correction or production callback-mutation test. The initial
`eduicbRenderGraph` call census finds its services present, not exhaustive
semantic equivalence.

`NuRndrSetDebBox`'s suspected multiplication grouping difference is a Ghidra
flattening artifact. Raw producers multiply range by the matrix before the
near/far factor, as current code does. MINSS/MAXSS retain the prior accumulator
on equal/unordered comparisons, also as current code does. No trial follows.
`NuRndrCircle` is byte-identical to its earlier audited source baseline; the
raw count-minus-one denominator, separate trig calls, triangle/UV stream and
integer return are represented. No repeat export or compiler trial follows.

## Batch 239: custom-collection exclusion source

`Collection_CreateCustom` now reads excluded model flags from the runtime
character system's `char_data`, not the permanent `CDataList`. Required model
and game masks still use their original permanent tables. Both master-list
and all-character modes retain existing ID/buffer contracts and mask-zero
short circuits. Linked matching improves **34.426605% to 37.717125%**, with
the other 21 isolated owner functions unchanged.

Frozen actual-body host sanitizer and host i386 SSE fixtures pass **8,196
cases** each, including distinct runtime/permanent tables, both modes, mask
combinations, buyability, retained negative-master-ID safety, zero counts,
alignment, arena canaries and cursor advancement. The old body fails the first
independent differing-table assertion. Character IDs and output allocation
remain valid caller preconditions; optional tables may be null only when their
corresponding masks are zero. No full collection-menu gameplay test is claimed.

Linked matching reaches **67.447770%**, **+0.319695 percentage points** from main,
with **6,278** exact functions. Target/native builds and all five repository
checks pass. Checkpoint `1b43572c` has all eleven GitHub checks successful on
that exact head; this newer local unit awaits the next push.

## Batch 240: coin-pickup bounds, sources and message capture

`GizmoPickup_CollectCoin` rejects negative player indices with the original
unsigned admission guard. It reads `DoubleScoreTime` independently of
`BonusTimer`, uses the direct `GizmoPickupType` array, sign-extends the model
ID before testing the minus-one sentinel and sets both HUD lifetimes to
`1.0f + COINMSGTIME`. Target scale is captured before the message template is
copied after the active-model gate. Linked matching improves **20.913420% to
36.601730%**, with no isolated owner collateral changes.

Frozen actual-body host sanitizer and host i386 SSE fixtures pass **2,970
cases** each, including all signed player bytes, independent score/bonus
timers, the five actual main-total routing modes, area/rumble masks, score
doublings, direct-table alias distinction, signed model sentinels and exact
message fields/event order. Actual routing, random and buzz helpers are
included. Independent old-body probes fail for the invalid index, lifetime,
sound source, table alias and model sentinel. Service mutation probes are
diagnostic only; actual routing does not perform those mutations. Type/model
indices and world allocations remain valid caller preconditions. No full
terrain/audio/gameplay validation is claimed.

Linked matching reaches **67.451000%**, **+0.322925 percentage points** from main,
with **6,278** exact functions. Target/native builds and all five repository
checks pass.

`GizForces_BoltHit` has a real missing world-context type lookup, and its Hit
helper lacks retail's animation-set null guard. The one combined original-
backed gate remains neutral at the BoltHit zero-score floor; other runtime
functions outside those two are unchanged. The repair remains reserved
without fixtures or further variants. The lookup is a pure table query, not
a fabricated callback mutation; private helper ABI/name discrepancies are
not permission to force calling conventions or clone names.

`eduicbInteractProp` has retail-backed label-interior hitbox, held-button,
menu-capture, row-refresh and canonical flag-access discrepancies. The single
combined gate loses **34.485 original-weighted matched bytes** across its
owner TU; all 31 existing exact functions remain exact. It stays reserved
without fixtures, integration or another variant. Callback mutation was not
demonstrated in production. `eduicbColourSlider`'s bounded raw/source audit
finds its supported services present, not exhaustive semantic equivalence.

## Batch 241: BEAST unordered comparisons and metadata reads

`Animate_BEAST` restores retail's unordered-inclusive idle comparison and
ordered walk threshold. Canonical character metadata is loaded where consumed;
pad suppression and the both-clip threshold use the live pad owner, while the
positive-input admission retains the captured pad. Actual idle/fall queries
are pure; reloads are not claimed as production callback mutations. The single
unchanged-action owner gate changes only BEAST and gains approximately **32.12
original-weighted matched bytes**. Original ABI/options and safety contracts
are unchanged.

Frozen actual-body host sanitizer, host i386 SSE and production-NDK i386
fixtures each pass **100,000 cases**: 28,784 finite and 71,216 safe unordered.
The old body diverges on 8,482 cases, with zero finite divergences. Actual
`MoveAnim_Check` and `UseFallAnim` bodies are included; default-idle and idle-
scheduler boundaries are explicitly limited to valid standard-idle metadata
and requests 1/25. No full idle scheduler, pointer mutation or gameplay
validation is claimed.

The linked report reaches **67.451690%**, **+0.323615 percentage points** from
main, with **6,278** exact functions. Target/native builds and all five
repository checks pass.

## Batch 242: OGG outgoing weak-pointer lifetime

`NuSoundDecoderOGG::Decode` now passes the callback weak pointer as a direct
temporary, matching retail's single outgoing live node instead of a named
local plus its by-value copy. The canonical weak-pointer implementation and
the already-correct `Read` call remain unchanged. Other owner function bodies
and the symbol surface are unchanged. Linked Decode improves **13.232484%
to 48.318470%**.

Frozen actual Decode/SubmitBuffer bodies and canonical weak-pointer code pass
**12,000 cases** each on host sanitizers and host i386. Cases check live weak
counts with/without resident pointers, destruction, ring requests, locks,
flags, buffer canaries, rounding and 64-bit accounting. The old body fails the
independent callback count assertion. Host fixtures explicitly seed valid
canonical sentinel nodes because the existing biased `+8` constructor is
32-bit-specific; i386 tests that unchanged constructor. Decoder/audio service
boundaries are mocked; actual Vorbis/audio playback is not claimed.

Linked matching reaches **67.460880%**, **+0.332805 percentage points** from
main, with **6,278** exact functions. Target/native builds and all five
repository checks pass.

## Batch 243: debris metadata reads across lock acquisition

`DebReAlloc2` reads the originally selected effect's particle type and time
group after acquiring the control-stack lock, separately reloads the current
effect-table entry for lifetime/trail arithmetic, and uses fresh effect entries
for orphan rendering. Acquisition can yield through `NuThreadSleep`; actual
list-insertion helpers do not publish metadata. Existing capacity/arena and
null-render-slot guards remain unchanged. Linked matching improves
**19.470210% to 41.668278%**; the other eleven isolated owner functions and
their symbols remain unchanged.

Frozen actual-body host sanitizer and production-action i386 fixtures pass
**63,888 cases / 17,764,560 checks** each. Cases cover all chunk counts 0..32,
both pools, uneven/zero particle counts, capacity boundaries, render slots,
matrix/position fields, counters, retained chunks and service order. Four
synchronous mock-lock publications exercise changed metadata/table entries;
the old body fails 586,368 expectations. No concurrent threads or C++ race-
safety claim is made; production helpers are not invented mutation callbacks.

Linked matching reaches **67.472560%**, **+0.344485 percentage points** from
main, with **6,278** exact functions. Target/native builds and all five
repository checks pass. Batches 239–243 form the next verified checkpoint.

## Batch 244: security-door visibility and floor-marker rotation

`SecurityDoors_Draw` keeps active, unopened floor markers inside the captured
entry-visibility gate. Rotation uses retail's squared-distance divided by six,
clamped to one, followed by the original 16384/49152 offsets. Actual nearest-
character and matrix helpers remain unchanged, including existing null-chain
safety. Linked matching improves **30.198381% to 76.522270%**; all other isolated
owner functions are unchanged.

Frozen actual-body host sanitizer and production-NDK i386 fixtures each pass
**24,751 cases**, covering visibility, opened/active doors, finite rotation and
alpha boundaries, nearest-player flags and missing chains, geometry and map
failures. Renderer services are typed recorders, not gameplay validation.
Independent old-body probes fail hidden-marker and finite-alpha expectations.
Target/native builds and all five checks pass. Global matching reaches
**67.483510%**, **+0.355435 points** from main, with **6,278** exact functions.

## Batch 245: torpedo update service order and radius rounding

`Torpedo_UpdateJobbies` updates position and both seek rotations before the
nonfirst-item angle/debris services, passes the two angle outputs in retail
order and preserves the separately rounded radius expression. Owner snapshots,
steal filters and existing bounds/null guards remain unchanged. Linked matching
improves **36.174603% to 43.203705%**; the other 21 isolated functions retain
their scores.

Frozen actual-body host sanitizer and host i386 SSE fixtures each pass
**4,121 cases / 39 transfers**. Three isolated old-behavior probes independently
fail order, angle and radius assertions. Finite math, valid packet allocations
and counts 0..5 are fixture contracts; angle/debris services are recorders, not
fabricated owner mutations or full gameplay. Target/native builds and all five
checks pass. Global matching reaches **67.485950%**, **+0.357875 points** from
main, with **6,278** exact functions.

## Batch 246: Jedi ordered weapon-sound admission

`Animate_JEDI` rejects unordered weapon scale along with nonpositive scale,
matching retail's `UCOMISS/JBE` before audio services. This one-predicate patch
preserves BEAST and every other owner body, literal and symbol. Linked JEDI
reaches **32.829365%**; the isolated score improves **29.559525% to 32.551586%**.

Frozen actual-body host sanitizer, host i386 SSE and production-action NDK
i386 fixtures each pass **100,000 cases**: 77,778 non-NaN and 22,222 unordered.
Independent audio/packet/fall-timer assertions pass; old audio differs in
17,777 cases and never on non-NaN cases. Actual movement/jump/manage helpers
are included; pure idle-query and scheduler boundaries remain limited to
valid standard-idle metadata. No NaN-consuming audio-engine or gameplay claim
is made. Target/native builds and all five checks pass. Global matching reaches
**67.486660%**, **+0.358585 points** from main, with **6,278** exact functions.

## Batch 247: KillParts caller-service closure

`KillParts` restores calls to the existing TIE-fighter and AT-AT helpers,
the Boba/Sarlacc/story exclusion, MiniDroideka minikit exception, byte-96 layer
flags, `VehicleArea` gating and original five/six-unit vertical impulses.
Random angles use retail constants and signed truncation before unsigned
narrowing; variant selection divides by 21846 and starts from zero-initialized
BSS. Generic mode admission uses equality to zero. Existing safety guards,
helper definitions and the ordinary bounded retry loop remain unchanged.
Linked matching improves **32.056168% to 43.350426%**; the single whole-TU gate
gains **442.394 original-weighted matched bytes**, preserving all seven exact
functions and every neighboring score.

Actual caller plus existing TIE/AT-AT/momentum helper fixtures pass **35 cases**
on host sanitizers and host i386 SSE. Five independent baseline groups fail.
Math/render/audio/animation services are controlled substitutes; pointer reload
and default-packet timing are not claimed exhaustive, and no production
callback mutation or gameplay validation is inferred. Target/native builds and
all five checks pass. Global matching reaches **67.496100%**, **+0.368025 points**
from main, with **6,278** exact functions.

## Batch 248: menu render live state

`eduiMenuRender` reloads selection for each row, uses live menu origin/width
for the final vertical extent, and reloads arrow centre between line services.
It preserves the initial selection-presence snapshot and all existing callback
contracts. Linked matching improves **32.652300% to 33.163450%**; the other
compared original-backed owner functions retain their scores.

Actual-body host, sanitizer and host i386 SSE fixtures each pass **29,000 cases**
against an independent full-flow reference. Real item/clipped-font bodies and
canonical lock-pointer types are used. Renderer/font mutations are reload
diagnostics, not production-mutation claims; callback ABI discrepancies are
unchanged. Valid live lists, bounded geometry and representable conversions
remain prerequisites. Target/native builds and all five checks pass. Global
matching reaches **67.496414%**, **+0.368339 points** from main, with **6,278**
exact functions.

## Batch 249: collection opacity and spacing snapshot

`Collection_Draw` captures vertical spacing before the selecting-player query
and rejects unordered final opacity. The initial alpha and fade guards retain
their different retail unordered behavior. Linked matching improves
**31.768518% to 37.192593%**; neighboring owner scores and symbols are unchanged.

Actual-body host sanitizer and host i386 SSE fixtures each pass **389 cases**,
plus a separate diagnostic spacing-snapshot probe. Actual collection/list/model
helpers are included; rendering and area/fmod boundaries are controlled.
Independent baseline NaN-opacity and snapshot assertions fail. No production
callback mutation or full collection-menu gameplay claim is made.
Target/native builds and all five checks pass. Global matching reaches
**67.499504%**, **+0.371429 points** from main, with **6,278** exact functions.

## Reserved caller and collision corrections after batch 243

`Bolt_HitGameObjectRC` omits owner rumble after its deactivation/buzz path.
Removing only the premature jump is negative: **32.506268% to 17.180450%**,
with all 65 owner neighbors unchanged. `DebrisSingleTorusCollisionCheckScaleYFlag`
has unordered-age admission discrepancies; its one correction loses **9.981834
weighted bytes**, preserving all 49 neighbors and five exact functions.
`DebrisSingleCollisionCheckScaleYFlag` has ordered-age and finite vertical-
rounding discrepancies, but the single combined gate remains neutral at zero,
with all 49 neighbors and five exact functions unchanged.

`NuSoundMemoryManager::MoveLargestTrailingBufferIntoBuffer` incorrectly counts
first-candidate adjacency and repeats a later tie query; its one complete
selection correction loses **2.727533 weighted bytes**, with 51 scored neighbors
unchanged. The real adjacency helper is pure; a valid-pool witness is not a
runtime test. `SetComboOpponent` admits unordered range; its single predicate
correction loses **4.246980 weighted bytes**, with all neighbors unchanged.
Private ABI and prior negative placement trials are untouched.

All five corrections remain reserved without runtime fixtures, integration
or repeated shape variants. Bounded full-raw caller reviews of `MovePlayer`,
`Move_DROIDEKA`, `Technos_Draw` and `eduicbRenderTexturePick` identify no new
substantive omission; they are not claims of byte-exact/runtime equivalence.

## Batch 250: techno reset floor sentinel

`Technos_Reset` tests retail's explicit 2000000 floor-height sentinel and clears
ground offset after either reset branch. NaN retains its ordinary branch;
existing platform and null guards remain unchanged. Linked matching improves
**81.766990% to 83.165050%**, with all isolated neighboring runtime bodies
unchanged. Actual-body host sanitizer and production-NDK i386 fixtures each
pass **60,485 cases**, including sentinel/ordinary/unordered heights, canonical
angle math, progress boundaries, canaries and recorded shadow/platform calls.
Old-body finite-height and offset assertions fail. Shadow services are mocks,
not world collision validation. Target/native builds and all five checks pass.
Global matching reaches **67.499626%**, **+0.371551 points** from main.

## Batch 251: torpedo draw matrix and model fallthrough

`DrawTorpedos` uses retail's post-rotation Y matrix operation and independent
second model test. Both retail models can draw in one call; existing counts,
guards and prior torpedo-update changes remain unchanged. Linked matching
improves **27.065727% to 27.981220%**; the other 21 owner functions retain their
scores. Actual-body host sanitizer and host i386 SSE fixtures each pass
**4,321 cases / 2,700 paired draws**. Independent old rotation and model tests
fail. Finite angles/timers/scales and valid counts 0..5 remain prerequisites;
draw services are recorders, not Android rendering. Target/native builds and
all five checks pass. Global matching reaches **67.499800%**, **+0.371725 points**.

## Batch 252: PVR face metadata and upload service closure

`NuIOS_CreateGLTexFromPVRInMemory` restores per-call cube-face metadata order,
cube binding-cache predicates, platform/default-flag compressed admission and
unconditional uncompressed uploads. Compressed fallback receives the actual
face target twice. New metadata parsing checks complete record headers and
declared payload bounds and uses memcpy for unaligned records. Existing
format guards and ownership/ABI/options remain unchanged. Linked matching
improves **27.199627% to 29.662313%**; all 47 other scored isolated owner
functions retain their scores.

Actual-body host sanitizer and production-action NDK i386 fixtures each pass
**40,522 cases / 3,754,140 checks**. Four independent baseline assertions fail
on both ABIs. Coverage includes all 720 face permutations, multiple/unaligned/
truncated records, platform and format combinations, cache/service ordering,
mips and dimensions. Recording services do not test a GPU or codec. Callers
still supply an allocated aligned 52-byte header, declared metadata and enough
pixel data, valid dimensions, fewer than 32 mips and at most six faces. No
arbitrary malformed-allocation safety is claimed. Formats 4/5 preserve the
existing guarded surface; retail table evidence establishes only entries 0..3.
The cross-owner retail-byte/current-i32 `comeFromHash` discrepancy is untouched.
Target/native builds and all five checks pass. Global matching reaches
**67.501080%**, **+0.373005 points** from main, with **6,278** exact functions.

## Batch 253: AI area inside-mask signed widening

`GameAISysStartFrame` reproduces retail's wrapping unsigned 32-bit inside bit
followed by explicit signed widening. Outside removal retains its separate
full 64-bit bit. Valid area indices remain 0..63; no signed-shift overflow or
shift-by-32 expression is introduced. Existing geometry, timer/null/client
guards, paths, service boundaries and ABI/options remain unchanged. Linked
matching reaches **40.463688%**; isolated matching improves **37.164806% to
40.337990%**, with all other 359 owner units retaining their scores.

Actual-body host sanitizer and host i386 SSE fixtures each pass **106,826
cases** across all 64 indices, masks, inclusive/outside positions, eligibility,
cardinal rotations, guards, traversal, path bookkeeping and processor order.
The independent baseline occupancy assertion fails. Actual vector helpers are
included; path-update/script services are probes, not full AI emulation or
fabricated production mutation. Existing NaN geometry and callback path-owner
reload questions are outside this patch. Target/native builds and all five
checks pass. Global matching reaches **67.501980%**, **+0.373905 points** from
main, with **6,278** exact functions. Batches 244–253 form the next checkpoint.

## Further reserved gates after batch 249

`Batarangs_Update` has finite timer-clamp, lost-target skip and feedback-order
discrepancies; its one combined gate regresses **33.885323% to 21.155964%**,
with all 18 neighboring functions unchanged. The separate existing lost-target
index-5 out-of-bounds risk is not permission to reproduce retail adjacent-byte
access. `eduicbRenderExpander`'s child-chain traversal and portable canonical
hover-bit repair lose **4.663985 weighted bytes**, preserving all neighboring
scores and exact functions. Neither trial has runtime fixtures or integration.

`edppLoadPage`'s two orphan-counter references and `ZapCode`'s two byte-offset/
typed-animation-index corrections are independently retail-backed but neutral
at **31.903720%** and **49.138410%** respectively. Neighboring scores and exact
functions are preserved; no fixtures or further variants are run. The private
loader helper and Zap ABI discrepancies remain untouched.

`Move_WEIRDO`'s retail caller-side context guard is same-semantics with the
current early-return helper, but loses **17.903518 weighted bytes**; every
neighboring body remains identical. `eduicbRenderGraph`'s 1.125 row multiplier,
font query order and signed arithmetic half correction lose **8.884740 weighted
bytes**, with all neighboring scores and exacts preserved. Its separate missing
sixth callback parameter remains unresolved; no signature/ABI workaround is
trialed. Both negative proposals are frozen without fixtures or variants.

`GizBombGens_Update` omits the verified `dragbomb` spawn-script literal in three
calls. A combined ordinary Player-load/control/literal gate loses **153.545172
weighted bytes**. A separately authorized literal-only contract gate is neutral,
with every original-backed score unchanged. Both remain reserved without
fixtures; no production Player mutation is invented. `DeactivatedCode`'s
proposed unordered predicate change was withdrawn before editing or compilation:
the complete raw branch truth table confirms the existing predicate already
has the relevant behavior. This is an audit retraction, not a compiled gate.

## Batch 254: torpedo bonus and machine target branch closure

`TorpedoCode` restores the bonus target fallback, cheat/count branch ownership,
live bonus-mode bolt selection and null-result timer decay. Existing count,
world, area and target guards remain; preceding torpedo fixes are preserved.
Linked matching improves **37.076088% to 39.493477%**; the isolated full-owner
gate improves **36.750000% to 39.145653%**, gaining **49.543704 weighted bytes**
without changing neighboring scored bodies. Actual-body host sanitizer and
production-NDK i386 fixtures each pass **31,110 cases**, with real target/vector/
angle helpers and recorded machine/audio/HUD services. Three independent
baseline bonus/timer/machine assertions fail. Valid bounded counts, finite
geometry and allocated service state remain prerequisites. Count trimming is
not exercised: retail's adjacent-array access is not reproduced or reinterpreted
as permission for out-of-bounds indexing. Target/native builds and all five
checks pass. Global matching reaches **67.503040%**, **+0.374965 points**.

## Batch 255: colour picker marker coordinates and ordering

`eduicbRenderColourPick` restores truncation-toward-zero width division and the
ordinary descending white-then-black marker loops, applying offsets before
coordinate truncation. Existing hue/value helper ABI, guards, HSV conversion
and out-of-range handling remain unchanged. Linked matching improves
**35.849270% to 36.794167%**; the isolated gate improves **35.774715% to
36.717990%**, gaining **25.855168 weighted bytes**. Every other original-backed
owner score and all 31 exact functions are retained.

Actual-body host sanitizer and host i386 SSE fixtures each pass **231 cases**:
224 width/saturation/hue combinations, a no-draw case and six independently
suppressed marker diagnostics. Gradient/swatch/marker geometry and draw order
are recorded; renderer services are not GPU validation. Selected finite
coordinates keep the existing colour shifts defined. The baseline fails the
independent coordinate/order assertion. Target/native builds and all five
checks pass. Global matching reaches **67.503590%**, **+0.375515 points**.

## Batch 256: attack opponent effects, parsing and live geometry

`Action_AttackOpponent` restores the captured owner game-object source, local
substring offsets, opponent AI admission, owner effect ordering, 1.5 protection
threshold and one-at-once opponent source. It retains existing entry/pad guards
and adds safe nested object guards. Ordered/unordered hold and attack branches
follow verified raw operand order and destinations. Nonpositive or unordered
attack range sets movement suppression before ACTION; positive range rejects
context 0x5a and recomputes live packet-owner distance after movement services.
Shared parser/opponent helpers and ABI/options remain unchanged.

Linked matching improves **33.168640% to 63.325443%**. The unchanged-action
isolated gate improves **32.902367% to 63.000000%**; only this function changes
among 415 normalized owner units. Actual-body host sanitizer and host i386 SSE
fixtures each pass **481 cases**, plus a separate diagnostic capture/reload
probe on both. Real string/math/one-at-once/group-null movement bodies are
included. The float-parameter leaf records selected strings and uses strtof;
formation services are outside the tested domain. The mutation wrapper exists
only in the separate diagnostic and does not claim production movement mutates
positions. Baseline suppression and independent live-distance assertions fail.
Target/native builds and all five checks pass. Global matching reaches
**67.513040%**, **+0.384965 points**, with **6,278** exact functions.

## Batch 257: wall collision local hit accumulator type

`HitWallSpline` restores a local float zero/one accumulator, matching retail's
MOVSS success stores and CVTTSS2SI return. The public i32 signature, geometry,
query publication, guards and compile action are unchanged. Both possible
values convert exactly. Linked matching reaches **31.915709%**; the isolated
full-owner gate improves **31.070880% to 31.829502%**, gaining **19.731758
weighted bytes**. All other original-backed owner scores and four exact
functions remain unchanged.

Actual-body host O2 sanitizer and production-action NDK i386 fixtures each
pass **12,446 cases**, with 670 hits, 11,776 misses and 318 penetrating hits.
Every query byte, material and global hit output matches the untouched baseline;
independent segment, penetration/time-clamp and coarse-miss expectations pass.
Real vector normalization/magnitude/square-root bodies use system libm.
Finite bounded multi-segment geometry is diagnostic coverage, not terrain-engine
integration. Target/native builds and all five checks pass. Global matching
reaches **67.513460%**, **+0.385385 points**, retaining **6,278** exact functions.

## Further reserved gates after batch 253

`NuDisplayListSwapBuffersEndFrame`'s defined visibility-bit clear repairs a
valid bit-zero shift-by-32 expression but loses **281.662433 weighted bytes**:
isolated **26.707693% to 10.575824%**. All 22 neighboring scores and text-symbol
surface remain unchanged. `edgraCalculatePage`'s two verified distribution
constants are neutral at **36.174114%**: 27 literal-reference bodies change,
but every original-backed score and exact count is retained. Neither receives
fixtures, integration or further shape variants.

`edgraLoadPage`'s rejected-record slot reuse correction loses **19.227744
weighted bytes**, **48.506447% to 47.637200%**, with neighboring scores/exacts
preserved. It affects placement and last-clump tracking, not demonstrated
premature capacity exhaustion: file count is capped to initial free slots.
`Pulses_Configure`'s joint line/token outer condition loses **765.308599 weighted
bytes**, **48.254010% to 0%**, with all six neighboring bodies unchanged.
Tokenless comma-only or empty-quoted input is the verified parser distinction;
ordinary blank/comment lines are skipped by the real parser. Both trials stop
without fixtures, variants or integration.

`LedgeTerrain_MoveCode`'s proposed unordered timeout correction was retracted
before any source edit or compile trial: verified UCOMISS operand order and
the JB continuation already match the current predicate. This is an audit
closure, not a losing compiled gate.

## Batch 258: turret cooldown local branch/store reconstruction

`GizTurrets_Update` mirrors retail's local subtraction, ordered-negative
branch and mutually exclusive field stores. Downstream field reloads remain;
no gameplay change or missing-service closure is claimed. Linked matching
improves **4.347758% to 5.375295%**; the unchanged-action full-owner gate
improves **4.162864% to 5.190401%**, gaining **57.316031 weighted bytes**.
All other function bodies, exacts and public symbol/type surface are unchanged.
Host sanitizer, host i386 SSE and production-action NDK i386 diagnostics each
pass **100,000 full-body cases**, including 12,500 independent raw-clamp cases
(2,084 quiet NaNs) and 450 firing scenarios. The parent independently reruns
host sanitizer and NDK i386. All 24 modeled boundary services are exercised.
Matrix/seek/intercept/audio/bolt/special services are deterministic stubs, not
real engine validation; only cooldown predicates receive exceptional floats.
Opaque target-kind-1 layout is unchanged and untested. Target/native builds
and all five checks pass. Global matching reaches **67.514660%**,
**+0.386585 points**, retaining **6,278** exact functions.

## Batch 259: pickup challenge/mission value-selection source form

`AddPickups` uses one ordinary equivalent ternary for the existing challenge/
mission coin gate. Short-circuit mission invocation, guards, helpers, types,
ownership and options remain unchanged. This is not a gameplay repair.
Linked matching improves **38.539032% to 39.005577%**; the isolated full-owner
gate improves **38.241634% to 38.708180%**, gaining **10.651245 weighted bytes**.
The other nine text symbols retain identical bodies. The compiler still emits
a branch in the mission block: the measured gain is downstream register/
scheduling codegen, not a claimed restored CMOVE instruction.

Actual-body host address/undefined/float-cast sanitizers and NDK i386 each pass
**401,501 checks** covering challenge/mission short-circuit, arcade/network,
panel/parts/consolidation, finite touch geometry, heart recipients, vehicle,
torpedo and powerup routes. An independent scalar coin decomposition oracle
uses division rather than the greedy loop. Math vector bodies are real;
mission/render/audio/parts and MakePartVector are explicit diagnostic boundaries
without invented world/player publication. Valid allocated level/world state,
bounded counts and finite conversions remain prerequisites. Target/native
builds and all five checks pass. Global matching reaches **67.514900%**,
**+0.386825 points**, retaining **6,278** exact functions.

## Further reserved gates after batch 257

`NuGCutRigidCalcMtx`'s three float rotation temporaries postpone conversion
until all curve evaluations finish, as verified in raw. Its one gate loses
**15.592617 weighted bytes**, **34.199314% to 32.776630%**, preserving all 47
neighboring scores and 19 exact functions. `ANI_Ani3ExtractAllNodeCurves`'s
signed mask-byte comparison against the full curve index loses **38.195086
weighted bytes**, **31.381704% to 28.340694%**, with all ten neighboring scores
and text surface unchanged. The packed-bit type-10 memcpy is already correct;
no misleading numeric conversion is attempted. Neither trial gets fixtures,
integration or further source variants.

`pathEditorCreateData`'s canonical-width field counters and retail nonempty-
path special-route helper guard lose **235.460688 weighted bytes**,
**23.950617% to 18.265907%**, with all neighboring scores unchanged and no exact
loss. The participants-times-sizeof-path allocation is confirmed in retail
and deliberately retained. The combined negative proposal remains frozen;
no split follow-up gate, fixture or integration is performed.

## Batch 260: prompt-menu signed colour conversion

`Status_DrawPromptMenu` restores the retail signed RGB conversion without
changing its canonical byte-valued text arguments. This is a source-type
reconstruction, not a new mask or gameplay repair. Linked matching improves
**33.905940% to 44.371290%**; the unchanged-action whole-owner gate gains
**197.755108 weighted bytes**, including its small nonexact helper change.
No exact function is lost. Frozen actual-body host sanitizer and host i386 SSE
diagnostics each pass **20,000 cases**, comparing events, menu/packet/state and
palette bytes against the prior body and an independent oracle. Finite phase
and conversion bounds are enforced; injected menu/controller/palette changes
are diagnostic seams, not claims about production mutations. Target/native
builds and all five checks pass. Global matching reaches **67.519080%**.

## Batch 261: cylinder intersection arithmetic association

`CheckCylinder` restores the two raw-backed floating-point associations for
forward edge parameter and normal Z, preserving all types, guards and helpers.
Linked matching improves **21.537415% to 24.899660%**; the unchanged-action
whole-owner gate gains **91.822938 weighted bytes**, with all other scores and
four exact functions retained. Actual-body host sanitizer and production-NDK
i386 diagnostics each pass **30,003 cases**: 1,957 forward hits, 2,040 overlaps,
26,006 misses and 1,134 corrected finite normals. Real vector/trigonometric
bodies run; an independent forward oracle and bytewise query comparison cover
unchanged fields. The prior body fails a finite normal-bit witness on both
ABIs. Rare changed-admission ties and full gameplay are not claimed covered.
Target/native builds and all five checks pass; global matching **67.521020%**.

## Batch 262: full-width area resets

`NewArea` resets both canonical full-width saved-minikit counters and all four
arcade player/AI kill counters rather than only their low bytes or first field.
Only six assignments change; no layout, helper or ABI changes. Linked matching
reaches **51.088080%**; isolated matching improves **46.466320% to 50.414510%**,
gaining **33.243760 weighted bytes**, with all 22 other owner bodies unchanged.
Canonical full-body host i386 SSE and a host64 sanitizer six-store block
diagnostic each pass **4,352 cases**, including all byte counts and area IDs
-1, 0, 1 and 71. The unchanged full body is not host64-safe due to preexisting
fixed-width pointer assumptions. Both old-body controls fail reset comparison.
The fixture's initial unsigned-health oracle was corrected to the canonical
signed health field before the final passing run; production code was not
changed for that fixture error. Target/native builds and all five checks pass;
global matching **67.521730%**, retaining **6,278** exact functions.

## Batch 263: live stun animation and facing arithmetic

`StunGameObject` reloads the model/table and selected animation after context
services, returns from the live animation, and restores the retail final facing
expression `-(object - attacker)`, including signed-zero argument bits.
Linked matching improves **26.314960% to 33.641730%**; the unchanged-action
whole-owner gain is **80.615253 weighted bytes**, including three small nonexact
neighbor changes. No exact function is lost. Root independently reruns host
address/undefined sanitizers and host i386 SSE: **40,344 cases** each, plus
separate diagnostic callback/model/live-return probes. Actual ClearContext,
AnimSpeed and trigonometric bodies run. ResetContexts uses an explicit pointer/
order boundary probe instead of unsafe fabricated host-overlay writes.
Injected model and duration mutations are diagnostics, not asserted production
behavior. Finite angle lookup bounds remain enforced. Target/native builds and
all five checks pass. Global matching reaches **67.523445%**, **+0.395370 points**
from cycle main, retaining **6,278** exact functions.

## Additional closed reserves after batch 263

`oneAtOnce_MaintainArray`'s retail per-slot hysteresis and repeated shortened
prefix sorting loses **40.145957 weighted bytes**, **39.349650% to 36.045456%**.
The first pass includes the null sentinel; an incumbent shifted right may get
the 0.75 bias again. Counted/initialized prior storage guards remain. All other
13 owner bodies are unchanged. `NuTexAnimEnvProc`'s two-callsite unsigned-word
condition helper trial is byte-identical across the complete objects and
neutral at **45.732178%**. Negative caller conditions still select default;
there is no public ABI change or missing interpreter behavior claim.

`SpecialMiniKits_Draw`'s stable item cursor and live world count with a null
guard loses **36.548784 weighted bytes**, **11.284553% to 7.524390%**; all other
owner bodies remain unchanged. `NuLgtArcLaserDraw`'s raw-backed 16-bit exponent
field form loses **172.749626 weighted bytes**, **39.847070% to 36.195630%**,
preserving 47 neighboring scores and 19 exacts. Its unusual prior-vertex +50
left-bound comparison is verified retail behavior and retained.
`GizForces_Reset`'s missing-previous/animation-set zero-height fallback loses
**42.088763 weighted bytes**, **33.002754% to 30.110193%**, with all other 50
text bodies unchanged. Valid-but-empty animation lists retain -2e9. None of
these nonpositive gates receives fixtures, integration or further variants.

`edanimDoInput`'s initially positive partial is **withdrawn**, not integrated:
actual-body fixtures expose an additional fresh-selection cycling omission.
The complete retail mode-transition, live-pad and fresh-selection guard unit
loses **74.087909 weighted bytes**, **28.468966% to 25.141378%**, preserving
all 74 exacts. No passing-fixture claim is made for either version; no further
shape trial is run after the complete correction's negative gate.

## Batch 264: minikit text opacity conversion

`MiniKit_LSW_Draw` restores signed intermediate conversion and canonical
byte-width opacity at its four text sites, including the active title.
Linked matching reaches **39.006943%**; the unchanged-action whole-owner gate
gains **46.672798 weighted bytes**, with all 24 neighboring scores retained.
Root independently runs actual-body sanitizer and production-NDK i386
diagnostics: **144,242 checks / 15,218 calls** each. The actual finished-alpha
helper and independent modulo-byte oracle cover finite timer/count paths;
negative inactive timing demonstrates a real 384-versus-128 prior witness.
Active clamping is unchanged. No renderer fidelity or out-of-domain conversion
claim is made. Target/native builds and all five checks pass; global matching
**67.524420%**, retaining **6,278** exact functions.

## Batch 265: cutscene completion independent of playback rate

`CutScenes_Update` limits the ordered positive-rate condition to sound cues,
not completion, restores strict previous-frame comparison and live instance
reads after services, and retains missing-instance guards. Linked matching
reaches **48.951218%**; unchanged-action whole-owner gain **46.545643 weighted
bytes**, with all other bodies retained. Root independently runs full-body
sanitizer and production-NDK i386 diagnostics: **100,000 cases** each.
The same-oracle old-body NDK control fails finite zero-rate finished case 2:
three instances queued instead of four. Non-stopcut, music-status-zero paths
include negative/NaN rates and separately labelled callback/invalid-instance
diagnostics; stopcut transitions and active-music paths are not covered.
No real audio/concurrency claim is made. Target/native builds and all five
checks pass; global matching **67.525430%**, retaining **6,278** exacts.

## Batch 266: canonical material texture-wrap fields

`NuShaderObjectGLSLSetupTextureStates` uses existing canonical material
bitfields instead of hard-coded byte offset 0x41 in its four diffuse cases.
Both wrap values remain captured after binding and before parameter calls.
Linked matching reaches **61.951122%**; the unchanged-action full-owner gate
gains **71.645974 weighted bytes**, preserving all 31 other scorable functions
and six exacts. Root independently runs sanitizer and production-NDK i386
candidate fixtures: **22,800 cases / 785,589 checks** each. Target baseline
also passes; host64 baseline reports **30,824** argument/order failures because
canonical attributes move from offset 64 to 80 with host pointer width.
Recorded GL/texture/wind services include diagnostic mutation seams, not GPU
validation. Existing cube-cache valid-unit bounds remain prerequisites.
Target/native builds and all five checks pass; global matching **67.526930%**,
retaining **6,278** exact functions.

## Batch 267: complete shop item confirmation and clamp placement

`ItemMenu` restores guarded per-store clamp calls, including repeated picked-
item clamps, and limits entry-selection reset to the two special picked 0/5
routes. Valid pending picked=-1 confirmation retains a fresh menu-column
candidate. The initial clamp-only proposal is superseded: its actual-body
oracle exposed the preexisting reset omission, and the complete corrected unit
was remeasured before integration. Linked matching reaches **81.129590%**;
unchanged-action full-owner gain **1,321.714337 weighted bytes**, with all 25
neighboring scores retained and no exact loss. Twenty-two disassembly-level
owner changes are relocation/compiler collateral, not 22 source edits.

Root independently runs host sanitizer and host i386 SSE diagnostics:
**20,000 complete-body cases** each, including actual ten-flag input, clamp
and sine helpers; 39 prior reset counterexamples; code/submenu transitions;
cancel returns; menu-stack/state bytes; and full recorded hint/audio/camera/
math arguments. Finite conversions, valid arrays and menu-stack bounds are
prerequisites. Math services are pure, not invented mutation seams; real
camera/audio/gameplay is not validated. Target/native builds and all five
checks pass; global matching **67.555020%**, retaining **6,278** exacts.

## Batch 268: signed terrain loader count

`TerrainInitEx` captures the loader return in an ordinary signed-16-bit local,
as in retail, before failure testing and group loops. The real untouched
loader returns only -1 or 0..32767 because its chunk count is signed 16-bit
and it admits at most one group per chunk. This is same-domain type/source
reconstruction, not a gameplay repair. Linked matching reaches **33.995697%**;
whole-owner gain **77.235392 weighted bytes**, preserving all other scores
and four exacts. Root independently runs full baseline/candidate sanitizer
and NDK-action i386 diagnostics: **2,115 cases**, **148,641,356** and
**142,919,612 checks** respectively (including byte-range lengths). Canonical
scene/terrain records, real sqrt, valid loader-domain counts, geometry,
material/version, scene/remap/display and allocation paths are compared.
Three maximum-count cases use synthetic disabled groups; file loading,
allocation and flush remain diagnostic services. Hook-only prior line wrapping
is reconciled with normalized full-source identity. Target/native builds and
all five checks pass; global matching **67.556650%**, **6,278** exacts.

## Batch 269: editor axis arrow association

`EdManipulator::DrawAxis` restores `(delta * (Scale * 0.25)) * 0.5` rather
than precombining the half-size; its captured radius and service fanout remain.
Linked matching reaches **25.883648%**; whole-owner gain **2.505193 weighted
bytes**, preserving all neighboring scores and 74 exacts. Root independently
runs actual-body sanitizer and host i386 SSE: **29 cases** each, with actual
axis-locator/identity helpers and recorded complete renderer sequence. Finite
large/underflow scales, matrix modes and colour paths are covered; the old
body fails the finite Scale=4e19 -2e38-versus-negative-infinity witness.
Existing host64 class-offset limitations are explicitly accommodated, not
repaired or hidden. A separate Scale-mutation probe is diagnostic only; no
real renderer validation is claimed. Target/native builds and all five
checks pass; global matching **67.556720%**, retaining **6,278** exacts.

## Batch 270: canonical startup configuration stores

`InitGameAfterConfig` restores eleven existing canonical camera, icon, input,
screen, targeting, grass, powerup-text and last-coin stores. Public types and
headers remain unchanged. Whole-owner gain **133.749248 weighted bytes**;
all other 45 scores retained. Linked target **70.670820%**, global
**67.559660%**, **6,278** exacts. Root independently runs actual complete-body
sanitizer and host i386 SSE diagnostics: **4,352 cases** each. The old i386
body fails the first camera-reset case. Canonical backing records, typed
service traces, the preconfigure checkpoint and changed final states are
covered. Missing private redirect/callback ownership is not claimed closed;
real startup/game integration remains unvalidated. Target/native and all five
checks pass.

## Batch 271: force loader version-fourteen sound reset

`GizForces_Load` retains all three embedded-name reads but clears the three
signed sound IDs for version 14, restoring the original legacy-config
fallback eligibility. Along-socket traversal and existing guards remain.
Whole-owner gain **21.378491 weighted bytes**, other 50 text bodies retained;
linked target **25.851393%**, global **67.560104%**, **6,278** exacts.
Root reruns sanitizer and production-NDK i386 actual-body fixtures:
**9,223 calls / 1,227,507 checks** each, with actual animation-file, callback
and name-reading helpers. Versions, optional sound masks, socket/world-area
paths, parsed fields and complete stream consumption are checked. Old-body
version-14 control fails. Other lookup services are deterministic typed seams,
not real gameplay/audio validation. Target/native and all five checks pass.

## Batch 272: terrain quad transform sequence

`TerrDrawPlatCol` transforms the fourth quad vertex regardless of the rotating
flag, and transforms normal zero again rather than normal one. A nonnull
matrix guard preserves existing safety. Whole-owner gain **59.476257 weighted
bytes**, four owner exacts retained; linked target **36.043780%**, global
**67.561356%**, **6,278** exacts. The independent signed-loader-count change
from batch 268 is preserved when reconciling the frozen owner snapshot.
Root runs actual full-body sanitizer and production-NDK i386 diagnostics:
**8,195 cases / 424,760 checks** each; old-body misses **3,156** finite cases
(1,577 rotating, 1,579 nonrotating). Actual alias-safe four-row matrix math,
all matrix fields, threshold-adjacent finite geometry, colours, transform/
render ordering and input preservation are covered. Rendering is recorded,
not GPU validation; unordered threshold behavior is unchanged. Target/native
and all five checks pass.

## Batch 273: editor locator-set allocation failure

`AISYSRebuildFromEditorData` skips set population when its real fallible
allocator returns NULL, but still rebuilds creatures, invokes the game
callback, rebuilds antinodes and selects the level path. Whole-owner gain
**65.138520 weighted bytes**; neighboring direction score unchanged, no exact
loss. Linked target **48.211000%**, global **67.562740%**, **6,278** exacts.
Root verifies actual-body/helper freezes and runs sanitizer and host i386 SSE:
**10,723 checks** each. Real allocator boundary cases, subsequent smaller
creature allocation, normal areas/locators/creatures/antinodes and 64-member
limits are covered; old-body control dereferences the failed set allocation.
List/path/lookup services are diagnostic seams; replacement callbacks are
explicit boundary probes, not established production mutations. Target/native
and all five checks pass.

## Batch 274: packed texture codec argument

`NuGScnReadTexturesPS` passes retail's literal `true` to `NuTexCreatePS`;
the sign-derived boolean still controls six-slot cube bookkeeping. Whole-owner
gain **6.244578 weighted bytes**, all 20 neighboring scores and six exacts
retained. Linked target **50.330738%**, global **67.562874%**, **6,278** exacts.
Root verifies exact loader/callee freezes and reruns sanitizer and production-
NDK i386: **14,150 cases / 16,076,467 checks** each. Actual texture/platform
dispatch bodies, valid packed streams, all platforms, hash mode, cube slots,
payload arena restoration and fallback are covered; baseline routes positive
ATITC entries through DDS. Codec/GPU services are mocked. Existing malformed-
stream and unchecked payload bounds are not claimed repaired. Target/native
and all five checks pass.

## Batch 275: reset progress slot reloads

`CheckResetBits` in `supportall.cpp` reloads the canonical signed-byte level
index for progress clearing, animation restoration and gizmo reset rather
than retaining a local across those services. Whole-owner gain **59.409745
weighted bytes**; other 49 scores retained. Linked target **70.430885%**,
global **67.564100%**, **6,278** exacts. Root verifies full reference/raw/body
and real callees, then runs sanitizer and host i386 SSE: **4,352 checks** each.
Valid slots, reset masks and player counts are covered. Callback replacement
probes are diagnostic, not established production mutations; populated AI
reinitialization is not covered. Target/native and all five checks pass.

## Batch 276: reserved editor animation service-loop form

`edanimUpdateObjects` captures particle/sound bounds and reloads them after
emission, lookup, erasure and playback at the original service boundaries.
Whole-owner gain **564.163253 weighted bytes**, including a **1.178294-byte**
isolated neighbor loss; 576 other isolated scores retained. Linked target
**61.763863%**, global **67.576100%**, **6,277** exacts. The linked neighbor
`edbitsRegisterSaveFormat` loses exact status (**100% to 99.638560%**), unlike
its already-inexact isolated baseline. The complete candidate is therefore
reserved and removed from the working tree; no neighbor tweaks or variants.
Root verifies whole
owner/body freezes, complete raw instructions and real erase helpers, then
runs sanitizer and production-NDK i386: **100,000 cases** each. Baseline NDK
also passes with identical event counts, supporting source-form equivalence.
Bounded effect counts, finite intervals and existing scene prerequisites are
covered; pure math/render services are diagnostic seams. No arbitrary count
mutation or full-engine proof is claimed. Target/native and all five checks
pass. These figures describe the reserved trial, not an accepted global gain.

## Batch 277: obstacle loader version contracts

`GizObstacles_Load` restores early-version default extents and unconditional
flags reads, the `GizObstacle` diagnostic prefix, **12 / 1.75** legacy scatter
heights, and default sound setup only through version twelve. Existing guards
and ABI are retained. Whole-owner gain **676.075866 weighted bytes**, all 47
other retail scores and ten isolated exacts retained. Linked target
**83.821580%**. Root verifies complete reference/raw/body/callback/helper
freezes and runs sanitizer and production-NDK i386: **15,127 calls /
1,725,439 checks** each. Valid versioned streams, all fields, consumption,
animation callbacks, actual error log and sound-default helper are covered.
Old-body sound control fails; low-level lookup/allocation/file services are
explicit diagnostic seams. No malformed-stream/full-engine claim. Target/
native and all five checks pass. Batch 278 below records the accepted combined
global after removing the reserved batch 276 trial.

## Batch 278: spline forward traversal loop form

`MoveSplinePosition` spells the already-guarded positive traversal as do/while
with the existing interpolation exit. Signed strides/segments, endpoint
stores, guards and unordered predicates are unchanged. Whole-owner gain
**76.965291 weighted bytes**, other scores and four isolated exacts retained.
Linked target **20.012987%**; accepted global **67.580124%**, **6,278** exacts,
with the batch 276 candidate removed and `edbitsRegisterSaveFormat` restored
to **100%**. Root verifies full raw/reference/body/real math freezes and runs
sanitizer and production-NDK i386: **30,421 cases / 1,395,093 checks** each.
Independent traversal oracle, both strides, loop flags, boundaries, endpoints,
NaNs, safe guards, state bytes/canaries and vector-service order are covered.
Both actual baseline and candidate agree; no gameplay callback mutation or
full-game proof is claimed. Target/native and all five checks pass.

## Batch 279: locator editor modifier dispatch

`locatorEditor_Process` admits all held-`0x100` input into the modifier branch,
queries nearest only for pressed-`0x100` there, and routes idle modifier input
to hover refresh. The unrelated pressed-`0x01` query is removed. Existing
navigation priority, guards, ABI/options and menu/rotation bodies are retained.
Whole-owner gain **59.325738 weighted bytes**, all other 29 scores and the
isolated exact retained. Linked target **45.213470%**, global **67.581380%**,
**6,278** exacts. Root reads full reference/literals/raw/body/helpers/types,
verifies frozen source identity, and runs sanitizer and host i386 SSE:
**1,637 checks** each. Actual list/nearest/angle/distance/rotation bodies run;
camera/creature/menu seams are diagnostic. Host alignment sanitizer alone is
disabled for the existing raw pointer store at editor offset `0x3692c`; target
layout coverage uses i386. Unrelated creation/reordering paths and retail sine
table bits are not claimed verified. Target/native and all five checks pass.

## Batch 280: canonical AI full-mask source

`InitPlayerAI` reads `_0xffffffffffffffff` for FreePlay and character types
above 63, matching the original GOT/data source instead of synthesizing ones.
Default global remains all ones; no production mutation is claimed. Whole-
owner gain **25.863665 weighted bytes**, all other 62 normalized functions
and exacts retained. Linked target **52.976190%**, global **67.581924%**,
**6,278** exacts. Root verifies complete reference/raw/body/GOT/data/helpers
and runs host sanitizer **2,048** FreePlay cases plus host i386 SSE **8,192**
all-mode/all-type cases. Full publication equivalence is covered; capabilities,
lever and final reset services are recorders. Existing raw pointer alignment
and widened packet offsets restrict host coverage to equivalence; final mask
oracle and callback paths use i386. Target/native and all five checks pass.

## Batch 281: editor animation cursor source form

`edanimDrawCursor` uses original direct component additions for the emitter
endpoint and one parameter-admission branch enclosing detail printing. The
initialized render packet, live index reload, ABI/options and all other
services remain unchanged. Whole-owner gain **142.998673 weighted bytes**;
the changed neighboring body retains its original score. Linked target
**67.516230%**, global
**67.584990%**, **6,278** exacts. Root verifies complete raw/reference/literals,
body/owner freezes and actual rotation/addition math, then runs sanitizer and
production-NDK i386 **100,000 cases** each. Root baseline NDK control also
passes **100,000**, supporting same-semantics source reconstruction. Complete
packets, materials, service order and typed print arguments are checked using
pure recorders and diagnostic sine-table data; GPU/font/scene traversal and
libc null-string formatting are not validated. Target/native and all five
checks pass.

## Batch 282: pickup loader versioned distance and area scale

`GizmoPickups_Load` floors draw distance at ten through version five, then at
one hundred for valid vehicle areas through version six. Newer versions keep
the file distance. The existing single-caller version-five scale helper now
returns one for an out-of-range area index, matching the original guards.
Whole-owner gain **122.212684 weighted bytes**, ten isolated exacts retained;
two small collateral scratch-register operand score shifts are included.
Linked target **42.035713%**, global **67.587610%**, **6,278** exacts. Root verifies full reference/
raw/body/helper identities and runs sanitizer and production-NDK i386:
**46,463 calls / 1,541,347 checks** each. Full valid-stream version/count/
distance/scale/flags/area-boundary matrix, byte consumption and record/tail
preservation are covered. Root old-body version-six control fails. Gravity
is a typed service recorder; its unchanged actual helper was inspected, not
executed. Invalid baseline area dereferences and full-game behavior are not
tested. Target/native and all five checks pass.

## Batch 283: recycled creature input mode and empty animations

`InitCreature` assigns canonical gamepad `input_mode = 1` instead of retaining
stale mode bits, and keeps animation one when all 233 animation slots are
empty. The existing canonical `model_data_b` replaces its raw offset access;
all null/table guards remain. Real allocation/reset helpers were inspected:
they do not clear the recycled input-mode byte. Whole-owner gain **17.264399
weighted bytes**, all other 50 normalized functions retained. Linked global
**67.587975%**, **6,278** exacts. Root verifies full reference/raw/literals,
body/helper freezes and actual service contracts, then runs host sanitizer
and host i386 SSE **196,952 cases** each. Complete state and service order,
all mode bytes, valid layer branches, empty/first/final animation slots and
invalid IDs are covered. Services are typed recorders; unchanged layer helpers
execute. Host full baseline equivalence is restricted to null-model paths
because the old raw pointer access is not valid under widened host layout;
i386 checks both full bodies. No full-game behavior is claimed. Target/native
and all five checks pass.

## Additional bounded reserves after batch 263

- `HatMachine_Draw`: the complete original-backed idle frame/negative timer/
  zero-boundary/32768-phase correction costs **22.564519 weighted bytes**
  (55.755726% to 55.025955%); all 25 other scores and seven exacts retained.
  Reserve the complete unit without variants or runtime fixtures.
- `FindGameObject`: complete guarded alive/CInfo closure costs **16.70 weighted
  bytes** (26.043083% to 25.086168%); no exact loss. Reserve without fixtures.
- `routeEditor_Process`: the original-backed nearest-node selection changes
  no fuzzy score or whole-owner weight. Reserve without fixtures or variants.
- `antinodeEditor_Process`: the original-backed opposite-direction rotation
  timer reset is whole-owner neutral; reserve without fixtures or variants.
- `unref`: the original-backed pointer-bound copy-loop form costs **73.150788
  weighted bytes** (20.411112% to 14%); other 49 scores retained. Reserve
  without fixtures or variants. Existing overlap semantics are already present.
- `MenuUpdateStore`: original fixed-eight purchase pulse costs **66.416941
  weighted bytes** (21.447887% to 17.873240%); all 54 other scores retained.
  Reserve without fixtures or source-form retries.
- `instNuGCutLocatorUpdate` complete dispatch correction costs **419.844400
  weighted bytes** (25.215881% to zero); exacts retained. Reserve the complete
  unit without cherry-picking. Look-at closure additionally requires canonical
  matrix-helper returns and private state ownership; no forged compiler clone,
  ABI shortcut or artificial helper fanout is authorized.
- Complete bounded raw/source censuses of `SubItemMenu`, `TextCrawl_Draw`,
  `Tag_UpdateHint` (including decoded jump table), `Dodge_UpdateHint`,
  `KillGameObject`, `GameObjectRotation`, `GizPanel_FindNearest`,
  `Batarangs_Draw`, `NuAnimBuffEvaluate_3`, `NuAnimBuffEvaluate_QuatB` and
  `AISysCharacterTestPathCnx` found no substantive missing closure. No trials.
- Fresh complete censuses of `Action_GetLocatorFromSet`,
  `GizForce_FindBestForceTarget`, `edanimLoadPage`, `WorldInfo_Init`,
  `StoreLevelProgressFn` and `HeadMovement` found no substantive
  missing closure; no trials.

## PR 121 rebase onto main a496c28b

Main advanced through PR 122 while checkpoint 12 was running. Preserve its
source restructuring, host hooks, bitfields and optimization map; regenerate
reports rather than replaying obsolete generated scores. The refreshed main
baseline is **68.002460%**, **6,290** exact functions; the next two-point
threshold is **70.002460%**. The rebased checkpoint measures **68.440850%**,
**6,292** exacts, with no loss among main's mapped exact functions. Historical
batch scores above describe the earlier baseline, not additive current gains.

Resolve source conflicts narrowly: retain pulse captures/reloads alongside
upstream guards, language-count revalidation, audio state publication, force
target comparisons/animation precedence, thermal surface and callback ordering,
techno control-pointer reloads, and bounded canonical spline overrides. Pickup
loading combines the original old-version floor with upstream's unordered
fallback. Pulse timer/protection and pickup NaN behavior are explicitly tested
as preserved upstream policy, not asserted to equal retail on those inputs.

Root independently reruns fresh exact-body diagnostics with current headers:
pulses (14 retained-contract cases plus five unordered-policy probes), language
setup (eight cases), splines (38), force targets (8,969), thermal update (27,207)
and impact (68,394), techno (1,622,293), pickup loading (46,529 calls), and audio
stream opening. Host sanitizer and i386 diagnostics pass; force, pickup, techno
and audio i386 use refreshed production NDK actions, while thermal/spline/pulse/
language i386 are host SSE diagnostics. These remain bounded service fixtures,
not full-game integration. Rebased target/native builds and all five checks pass.

## Batch 284: shared scale-axis lifetime

`EdManScale::Process` transforms and normalizes the selected first axis in place
in both axis groups and retains that result across selected entries, as the
original does. Remove only the per-entry copy; preserve second-axis behavior,
matrix/attribute operations, guards and the pre-existing ABI attribute.
Whole-owner gain **103.088993 weighted bytes**; all 74 isolated exacts retained.
The full owner baseline and production compile action remain byte-identical
after rebase. Linked global **68.443030%**, **6,292** exacts; no mapped exact loss
from the rebased checkpoint. Root reruns current-header full ASan/UBSan and
host i386 SSE fixtures: **290 cases / 9,986 checks** each. Actual math/attribute/
average-position helpers execute; manipulator/registry services are bounded
diagnostic seams. No complete editor or nonfinite-input equivalence is claimed.

## Batches 285–293: post-main contract improvements

Keep the refreshed **68.002460%** main baseline and **70.002460%** cycle
threshold. These linked measurements are sequential, not additive isolated
percentages. Root independently reviews each full original/body, relevant raw
instructions, canonical helpers and fixtures before applying narrow patches.
Compiler options, existing ABI attributes, source ownership and scoring remain
unchanged. Target/native builds and all five repository checks pass per unit.

- **285, `InitShop`:** retain original live scene reads, pre-service selection
  resets and missing-camera cleanup. Whole-owner gain **13.567571 weighted
  bytes**; linked **68.443330%**. Root host sanitizer, host and host i386
  diagnostics each pass **10,000 cases**. Actual `LoadShelfSplines` executes;
  allocation, shelf and camera services are bounded. Full shop gameplay and
  existing unsupported shelf-storage paths are not claimed.
- **286, `EdPP::SaveEffects`:** reload effect/type data at original service
  boundaries and publish the completed writer limit after refreshing the
  record count. Whole-owner gain **1,659.357460 weighted bytes**; target
  **57.866894% → 81.759730%**; linked **68.478470%**. Root current-header
  sanitizer and production-action NDK i386 diagnostics pass **100,000 schema
  cases** each; baseline i386 also passes. Real field writers execute. Valid
  finite schema bounds and mocked file services do not establish full
  filesystem or cross-platform wire-format equivalence.
- **287, portal recursion:** copy the complete canonical frustum while keeping
  the original plane alias and required arena footprint, then replace its room
  identifier. Whole-owner gain **23.985348 weighted bytes**; linked
  **68.478990%**. Root sanitizer and NDK i386 diagnostics pass **13,320
  cases** each. Bounded graph/arena tests establish copy and alias lifetime,
  not complete portal geometry or rendering.
- **288, `DoInput`:** restore required-pad pause dispatch, captured player/pad
  use, normal START source, state-change publication and ordered timer gates.
  Preserve upstream autosave unordered-input policy. Whole-owner gain
  **234.920443 weighted bytes**; linked **68.484024%**. Root sanitizer and
  host i386 SSE diagnostics pass **70,656 cases** each. Actual no-pad/cutscene
  predicates execute; pause/resume/restore are bounded service recorders.
- **289, `IncomingBolt`:** compute rounded travel distance before squaring,
  rather than four left-associated multiplications. Whole-owner gain
  **9.837743 weighted bytes**; linked **68.484245%**. Root sanitizer and host
  i386 SSE diagnostics pass **12,063 cases** each, with **34** old/new
  divergences validated against their respective arithmetic oracles. Real
  geometry helpers execute; finite boundary witnesses detect the omission.
- **290, `Levers_Draw`:** reload the ready pulse phase independently and retain
  the incomplete status special when disabled. Whole-owner gain **202.459799
  weighted bytes**; linked **68.488580%**. Root sanitizer and NDK i386 each
  pass **9,538,100 checks / 645,120 sweep calls**, plus seven direct guard/
  multi-draw calls. Actual math, tint, nearest-player and animation helpers
  execute; rendering/debris are bounded services. Nonfinite phase casts and
  unavailable animation data remain outside the fixture contract.
- **291, `PlatformChecks`:** traverse the canonical pointer-sized scan header
  and shape entries, preserving signed-positive groups and zero terminators.
  This fixes the wider-host copier while retaining the Android representation.
  Whole-owner gain **72.692343 weighted bytes**; linked **68.490110%**.
  Root sanitizer passes **5,512 cases / 7,948,331 checks**; NDK i386 passes
  **6,024 / 10,300,659**. Actual target body executes; diagnostic terrain
  services stop at the no-hit branch. Full terrain physics is not claimed.
- **292, `pathEditor_Render`:** restore original public zero-BSS
  `EdGetCnxFlagNames` callback, first matching connection's flags/footer,
  quoted route label and original live-path read boundaries with null guards.
  Canonical editor ownership is deliberate, not original file-symbol proof.
  Whole-owner gain **79.841931 weighted bytes**; linked **68.491820%**.
  Root freshly compiled sanitizer and host i386 SSE each pass **1,736 cases /
  5,251 checks**; actual `NuStrLen` executes. Font publication probes are
  diagnostic, not assertions that production font services mutate editor
  state. Callback defaults null and has no discovered internal writer;
  shipped activation and arbitrary external format strings are not covered.
- **293, `GameCam_UpdateLookRot`:** restrict digital input to left-stick source
  two and restore pitch/yaw seek steps as doubled rounded products of
  **1820/2730 × FRAMETIME**. Whole-owner gain **61.752090 weighted bytes**;
  linked **68.493120%**. Root freshly compiled sanitizer and host i386 SSE
  each pass **145,160 cases**, including **130,824** independently validated
  old-body divergences. Actual seek helpers execute without state mutation;
  callback pad-replacement probes are explicitly diagnostic.

All **6,292** linked exacts remain; mapped exact-set comparison from batch 284
through batch 293 loses none. GitHub's eleven checks all succeeded on pushed
checkpoint **eb4c525f**; these newer units still require their own pushed-head
CI and do not inherit that result.

## Additional complete reserves after batch 293

- `DrawTopShelf`: restore first-model idle pulse scale and second-model Y
  association together. One unchanged-action whole-owner gate is **neutral**,
  including all 27 original-backed scores and identical emitted symbol set.
  Preserve the ordinary correctness discrepancy as a reserve; no fixtures,
  variants or integration. Its unchanged private helper is **1,328 bytes**.
- `DrawParaphernalia` config-read timing: one complete raw-backed state-read
  restoration costs **299.161038 weighted bytes**, **39.253960% → 36.789295%**.
  All 13 isolated exacts remain. Checked real services do not prove config
  replacement; no production-mutation claim, fixtures, retries or integration.

## Batches 295–298: grid, text, mouse camera and spinner contracts

- **295, `CutScenePlayer_DrawGrid`** (original `0x1b2490`, 1,273 bytes): restore
  the once-called available-player snapshot and original **128 × opacity**,
  signed truncation followed by byte selection. Whole-owner gain **106.558413
  weighted bytes**; target **48.833977% → 57.204630%**; linked **68.495415%**.
  Root freshly compiled sanitizer and unchanged-action NDK i386 each pass
  **100,000 finite cases plus two diagnostic renderer owner-switch probes**.
  Actual completion predicates execute; availability/bounds/render/text are
  typed recorders. Valid pointers/divisors and nonnegative first rendered area
  are required; no NaN opacity cast or backend fidelity claim. The pure
  available-player query itself does not mutate state.
- **296, `Text_LoadAndFixUpStrings`** (original `0x48ddd0`, 1,071 bytes):
  restore the distinct legacy-byte, UTF-8 and UTF-16 encoding/ID/filter/empty
  token contracts, including the original `xbox` platform filter. Generic
  parser helpers already dispatch wide input; merely missing wide wrappers
  was not a separate behavioral gap. Whole-owner gain **16.558442 weighted
  bytes**; target **38.365190% → 39.911263%**; linked **68.495804%**. Root
  freshly compiles the complete actual parser/string translation units with
  both exact loader bodies: sanitizer and host i386 SSE each pass **13,064
  cases per body**, **4,754** independently validated differing cases.
  Only file I/O is mocked. Coverage includes encoding, packed-wide false
  filters, ID bounds, empty strings, duplicate IDs, parser exhaustion and
  canaries; malformed/oversized buffers, surrogate pairing and endian repair
  remain outside this unit. Root disables LSan for its sandbox runs; the
  contributing agent separately passes leak-enabled diagnostics.
- **297, `do_maya_mouse_camera`** (original `0x3a4a70`, 1,329 bytes): retain
  smoothed angle history, multiply drag zoom by `distance_speed`, and restore
  both original MINSS source-selection clamps. Whole-owner gain **574.093047
  weighted bytes**; target **0% → 43.197370%**; linked **68.508026%**. Root
  freshly compiled sanitizer and host i386 SSE each pass **65,873 cases**,
  **8,332** old-body divergences. Actual angle lookup/table, trigonometry,
  square root, rotation and vector helpers execute. Angle/int conversions
  stay finite; nonfinite distance clamps are tested separately. Android's
  actual keyboard/mouse producers return zero: nonzero input tests are
  dormant editor diagnostics, not live Android input/gameplay validation.
- **298, `GizSpinner_Draw`** (original `0x4c8540`, 711 bytes): restore the
  second special-existence query and use active `WORLD`'s level reflection
  override. Preserve contextual fallback only when global world/level is
  absent, explicitly safety policy outside the original valid-world domain.
  Whole-owner gain **333.430634 weighted bytes**, including a small positive
  `InitTerrain` collateral change; target **26.541176% → 73.070590%**;
  linked **68.515130%**. Root freshly compiled sanitizer and production-action
  NDK i386 each pass **2,379,435 checks / 108,676 calls**. Actual pure special
  queries and reflection math execute; renderer/shadow services are typed
  recorders. Preserve original shadow-enable-before-skip and unreflected-arm
  behavior; no invented helper mutation or complete renderer claim.

Target/native builds and all five repository checks pass after each unit.
Linked **6,292** exact functions remain, with no mapped exact loss from batch
284 through batch 298. Current main gain is **0.512670 percentage points**,
not the requested two points; the merge threshold stays **70.002460%**.

`Players_Init` story-roster index correction (`0xfe280`) is a separate
**unintegrated positive correctness reserve**: original compares the second
four-byte model record, not the third. One unchanged-action whole-owner gate
gains only **0.047479 weighted byte**, with all 62 neighbors unchanged. Stop
before large runtime fixture investment for usage efficiency; no integration,
runtime equivalence or negative-gate claim. `Player_ToggleCharacter`'s complete
bounded raw/source census found no substantive new omission; no trial.

### Batch 300: pod flag selectors and rounded sprint/pitch contracts

`Move_POD` (original `0x1738e0`, 1,814 bytes) restores two canonical
selectors: object word `field_0x1f4 & 0x40000` corresponds to retail byte
`0x1f6 & 4`, and `model_flags & 0x10000000` corresponds to character byte
`7 & 0x10`. Sprint acceleration uses `(1.4f - 1.0f)`, whose rounded float
matches retail `0x3ecccccc`, rather than the adjacent `0.4f` literal.
Engine pitch selects the average-speed branch only for ordered `< 1.1f`;
equality and unordered values select the saturated branch.

One frozen, unchanged-action whole-owner gate gains **25.386440 weighted
bytes**, with all four exact owner functions and the complete symbol surface
retained; no neighboring mapped function changes. Root reviewed the complete
reference, literals, current body, canonical field layouts, critical raw
instructions, actual seek/trigonometry helpers and full fixture. Fresh root
host ASan/UBSan/float-cast-overflow and production-NDK i386 builds each pass
**125,875 cases / 6,276,842 checks**. The original body control fails 83,516
assertions in the agent's identical bounded fixture, including 3,156 exact
coefficient witnesses. Real trigonometric initialization and scalar seek
execute; gameplay/audio services are synchronous recorders, not a complete
gameplay implementation. Post-hit publication probes are diagnostic, not
proof that production helpers mutate the tested field. Nonfinite averages
are limited to the changed branch without integer conversions; unchanged
camera-facing nonfinite policy and cached vehicle/pad metadata remain outside
this correction's equivalence claim. Existing null-area delegation remains.

Target/native builds and all five repository checks pass. Linked fuzzy
matching reaches **68.515686%**, with **6,292** exact functions and no mapped
exact loss from batch 298. Gain against current main is **0.513226 percentage
points**; the two-point merge threshold remains **70.002460%**.

### Fresh core/runtime census: avoid repeated low-yield trials

The complete `NuDynamicLight::computeClippingPlanes` (5,050 bytes) source,
helpers, original export, raw instructions and actual corner-table literals
already contain both clip-space tables, eight perspective transforms and all
six output-plane publications. A bounded complete raw/source census of
`PlaySfxByIdEx` (2,176 bytes) likewise finds its sample flags, listener/group
admission, six playback dispatches and fade restoration already present;
retail deliberately bypasses restoration on certain early exits. Neither
census claims runtime validation or justifies a compilation trial.

`NuMusic::Process` (original `0x313890`, 1,787 bytes) has all fader, duck,
stream/status, gain, mix and playback-time services. One full-reference/raw-
backed source-form correction restores the nested upper/lower gain clamp and
zero/stop block, preserving unordered gain-to-one policy and the two-voice
loop. Its single unchanged-action whole-owner gate loses **30.270708 weighted
bytes**, retaining 94 neighboring scores and 12 exacts. Named symbols and the
actual initializer remain; four unscored compiler-local labels differ.
Freeze this complete negative unit without fixtures, retries or integration.
Do not reopen already closed `edpartLoadSingleType` or
`NuDisplayListAddRenderScene` based merely on code-size deficits.

The current complete `MovePlayer_DIRECTIONAL` (14,602 bytes),
`MovePlayer_VEHICLEDIRECTIONAL` (9,744 bytes) and `JumpCode` (8,829 bytes)
bodies are byte-identical to their earlier bounded closure snapshots.
Directional service/state inventories already cover movement modes, pivots,
terrain, speed and velocity seeks; the proposed heading discrepancy relied on
a hypothetical mutating getter, while the actual getter is read-only. The
vehicle slowdown grouping was already retracted after raw producer review.
Jump's sentinel-callback correction already had one negative gate. Do not
repeat exports, trials or fixtures for these unchanged bodies without new
substantive evidence; these notes are bounded prior audits, not blanket
runtime-equivalence proofs.

### Batches 301–303: debris records, finite effect positions and touch ownership

- **301, `BuildDebrisVerts`** (`0x29765c`, 3,384 bytes): restore four
  16-byte corner records, position/extent/texture-offset transform order,
  pre-service fraction/frame snapshots and final counter publication order.
  A local trivial `NUVEC4`/`NUVEC` union exposes an active typed XYZ view;
  the fourth component remains untouched. The initial `VuVec` attempt was
  rejected for introduced constructor calls, then corrected once with plain
  storage. Authoritative whole-owner gain **240.911461 weighted bytes**,
  **58.283997% → 65.403130%**, all four exacts and symbol surface retained;
  linked **68.520805%**. Root fresh host sanitizers and production-NDK O0
  candidate/baseline diagnostics each pass **242 cases / 145,253 checks**.
  Real transform, repeat, buffer rollover and packet helpers execute; GL and
  list-item allocation are bounded recorders. Helpers do not mutate frames.
  Both old and new arbitrary optimized i386 diagnostics fail identically on
  a pre-existing matrix/vector view dependency; broad optimized-native
  equivalence is not claimed. Finite lifetime/repeat casts and bounded buffers
  are required; NaN near-plane admission is covered without invalid casts.
- **302, `AddAnimEffects`** (`0x3caa06`, 4,233 bytes): reject equal finite
  interval endpoints above one; snap aggregate Y only in the genuine
  zero-position fallback, and individual locator Y only when positions
  already exist. Collision-min override remains unsnapped even with zero
  locators. Preserve fallback locator storage initialization and all guards.
  Whole-owner gain **140.779632 weighted bytes**, **75.470825% → 78.796590%**;
  90 neighboring bodies and symbol surface unchanged; linked **68.523800%**.
  Root freshly compiled sanitizer and production-NDK i386 fixtures each pass
  **100,000 finite cases + seven directed cases**, including three actual
  footprint recorder calls. Root freshly rebuilt old NDK body fails all three
  independent timing/average/collision-min witnesses. Real vector helpers
  execute; debris/rate/random/audio/footprint services are nonmutating
  recorders, not production algorithms or callback-mutation proof.
- **303, gesture tracker `ReadData`** (`0x502100`, 3,216 bytes): pass current
  X/Y (`unknown_04`/`unknown_08`) rather than current Y/prior X, and assign the
  weak target reference only after an admitted query. Disabled admission
  retains the prior reference; an admitted NULL result still clears it.
  Whole-owner gain **561.402970 weighted bytes**, **49.039180% → 66.495740%**;
  all 29 neighboring bodies and symbol surface retained; linked **68.535706%**.
  Root fresh host ASan/UBSan and exact production-NDK owner linked to host
  i386 support each pass **13,376 cases + actual reference invalidation**;
  focused candidate witnesses pass and both old NDK witnesses fail. Root
  LeakSanitizer also passes outside ptrace. Actual constructors, touch lookup,
  coordinate conversion and `NuMechPtr` ownership execute; menu/query services
  are nonmutating recorders. Bound touch counts to ten and require the existing
  valid-object reference contract. The 704 separately counted NaN countdown
  cases preserve upstream ordered sampling policy, not general nonfinite
  gameplay equivalence.

Target/native builds and all five repository checks pass after each unit;
**6,292** linked exact functions remain with no mapped exact loss from batch
300. Main remains `a496c28b`; current gain is **0.533246 percentage points**.
The required two-point threshold remains **70.002460%**. All eleven GitHub
checks passed on checkpoint 14's exact head `e2e8997a`; a newer commit requires
a fresh exact-head CI run.

### Batch 304 reserve and additional closed inventories

`PodRaceInit` (`0x1fffa0`, 1,155 bytes) has a concrete unintegrated correction:
only spline initialization is non-client-only; mine/client reset and all ten
area/debris/part lookups are common retail work. `sizeof(minesys)` preserves
the canonical target reset extent while covering host-width pointers. Exact
NDK layout assertions confirm size `0x748` and radius/area/count/part offsets.
One complete unchanged-action gate remains **0% → 0%, net zero**, retaining
all 77 non-target bodies. Reserve without fixtures, variants or integration.

Current `GizForces_StoreProgress` and `GizObstacleUpdate_Proximity` are
byte-identical to their already rejected closure snapshots; no repeated
export, trial or fixture. New bounded resource censuses find no missing light
clone fields or decoder-worker dispatch. Streamer dispatch is independently
corroborated by the complete hub audit; weak-pointer temporaries account for
apparent bulk. Loader helper expansion likewise gives no qualified new lead.
These are evidence-only closures, not runtime validation. The previously
reconstructed `MechTouchTaskPlannedGoTo::Update` remains closed absent new
substantive evidence.

### Batches 305–306: canonical terrain allocation and D-pad rendering

**305, `NewTerrainScaleYMask`** (`0x385fa0`, 2,403 bytes): replace the
remaining `0x948` scratch allocation with `sizeof(TerrainQuery_s)`. The
canonical record has four pointer fields: target size/scan offset remain
`0x948`/`0x148`, whereas host64 requires `0x958`/`0x158`. The old host request
undersized the record by sixteen bytes. This is deliberately a small native
allocation correction paired with the positive D-pad unit, not a matching
gain: unchanged-action O3 baseline/candidate **whole objects are byte-identical**
(`cb5346a00a3cb7c967db9b1e1872331bd9bfb0935a198995dc39529cddb5df0e`).
Root independently runs the canonical-header diagnostic under host ASan/UBSan
and NDK i386: actual scratch allocation, scan-arena endpoints, following
allocation and LIFO release pass on both widths. The small i386 diagnostic
compiles actual allocator source with the terrain action's O3; it is not a
claim about the allocator owner's production instruction shape. No terrain
physics fixture, new query-lifetime rewrite, variant or target behavior change.

**306, `VirtualControlDPad::Render`** (`0x4530c0`, 1,309 bytes): restore
horizontal arrow spacing from `0.2f` to `0.4f` of the captured scaled radius.
The optional embedded mover copies the controller's **button mover**, offset
`0x94`, not its distinct lock button at `0x90`; preserve the NULL gate and
valid-controller precondition. Restore menu-before-screen query order and
four separate aspect reads with height calculated before each getter.
One necessary pre-fixture raw-backed repair corrects the flattened width
association to `aspect * (radius * scale)`; the provisional packet is not
integrated. Final exact-owner O2 gate **0% → 83.517240%**, **1,093.240672
weighted bytes gained**; all 29 neighboring bodies and T/W/V surface retained.

Root freshly rebuilds full-owner host sanitizer and exact production-NDK
owner/i386-support diagnostics: **28,224 finite render cases + 24 ordered-NaN
predicate diagnostics** pass on both. LeakSanitizer passes outside ptrace.
Both focused candidate witnesses pass; freshly recompiled old NDK owner fails
geometry and optional-widget-source witnesses as expected. Canonical object
sizes and optional-field offsets are asserted on i386. Actual widget and
texture-button bodies execute with canonical reference ownership; draw,
material and screen services are bounded nonmutating diagnostic doubles.
The aspect helper uses real pure backing-height/width arithmetic. No GL,
device, gameplay, callback mutation or concurrent resize validation is claimed.
Alpha is bounded below one so the existing signed-opacity shift remains
defined; no alpha-one/nonfinite-opacity equivalence claim. NaN sticks/timer
only exercise ordered predicates, not conversions or geometry.

Target/native builds and all five checks pass for the combined integration;
linked matching reaches **68.559050%**, **0.556590 percentage points** over
current main, with all **6,292** exacts retained. All eleven GitHub checks
passed on checkpoint 15's exact head `e3994abc`; these newer local changes
still require their own pushed-head CI verification.

### Additional bounded censuses and batch 308 reserve

Do not reopen unchanged quaternion players/blends and their six decoder/skip
helpers: current bodies are byte-identical to the prior full decode audit;
existing full blend references and raw tangent scaling disprove the proposed
new reassociation. W-blend's local/discarded slerp result remains intentional.
No new export, trial or fixture. World/action and UI/core censuses likewise
screen prior closed candidates before fresh bounded raw/body checks of
`UpdateMiniSnowTroopers`, `AIPathNodeUpdatePos`, `BlockCode`,
`Action_ShootAtOpponent`, `NuFileDevice::FormatName`,
`AIPathCnxControllerCreate`, `GetNextConnection`, editor grey/float/integer
sliders and creature-editor service blocks. No substantive complete-unit
target omission qualified there. These screens are not exhaustive equivalence
proofs or runtime validation. Creature-editor host path-check storage remains
an untrialed shared-type/consumer concern, not a claimed overwrite witness.
`areaEditor_Process` is source-screened only, not raw-closed.

**308, `AddStreakPoints`** (`0x4a3f80`, 1,399 bytes): two finite arithmetic
distinctions are raw-proven: both interpolation loops use rounded reciprocal
`1/count` times index, and the Bezier control-A coefficient groups
`inverse * (3 * amount) * inverse`. The complete two-expression O3 unit
regresses **38.395653 weighted bytes**, **42.200550% → 39.456043%**;
all existing symbols/exacts remain. Reserve without partial retry or fixtures.
The retail private Bezier helper's two unused integer parameters and natural
constprop clone remain an unresolved source/ABI difference; do not invent
argument values or register-passing attributes to force its code shape.

### Batches 307 and 309: blowup cursor and copied render matrices

**307, `GizmoBlowUp_Hit`** (`0x4bd890`, 1,469 bytes): retain the initial
typed blowup cursor across the live-count loop, advancing by the canonical
`0x12c` record; reload draw flags after the optional filter call. Initial
properties remain captured for earlier gates. The actual registered speeder
filter is a pure name test: no gameplay callback-mutation defect is claimed.
Existing six ordered bounds checks, including upstream NaN rejection, remain.
One complete unchanged O3-owner gate gains **20.836237 weighted bytes**,
**38.430267% → 39.848663%**, retaining neighboring bodies, symbols and exacts.
Root independently rebuilds and runs host sanitizer and NDK-action extracted
body/i386 diagnostics: **10,389 checks over 1,062 Hit calls** pass on each,
including 1,056 finite calls and six separate NaN safety cases. Actual vector,
distance, sphere, string and pure speeder helpers execute; destruction, bolt,
cheat and other downstream services remain bounded diagnostic doubles. The
baseline also passes these pure-helper cases; no invented mutation witness.
Linked matching reaches **68.559500%**, with all 6,292 exacts retained.

**309, `NuRenderContextSetViewProj`** (`0x2a33e0`, 1,124 bytes): restore
the three verified retail local static 64-byte `scale`, `translate`, and
`proj` matrices. Translation inputs are loaded after the scale helper. The
inverse view and view/projection product consume the copied context matrices;
position W precedes XYZ, and view submission precedes depth remapping. Final
perspective/frustum helpers intentionally still read the original projection
pointer, not a frozen value. One unchanged O3-owner gate gains **167.008528
weighted bytes**, **58.823010% → 73.681420%**; all four exacts remain. Full
defined-symbol comparison adds only the three original BSS records, removes
none, and retains all text definitions. No fabricated alignment or ABI change.

Root independently reruns host ASan/UBSan and production-NDK-action/i386
fixtures: **256 cases / 39,424 checks** pass on each, with actual matrix/trig
owners and a nonmutating shader recorder. Baseline ordinary controls pass
64 cases / 7,808 checks; full alias controls fail the copied-view-position
witness as expected. Four whole-array pointer modes cover disjoint external
and context-global inputs, not partial overlap or self-copy. Double-precision
oracle comparisons are tolerance-bounded, not bitwise retail equivalence.
No production caller is observed to pass these global aliases; this tests
the bounded pointer contract, not a demonstrated gameplay defect. Singular,
nonfinite, concurrent and reentrant inputs are excluded. NDK fixtures include
the full owner with diagnostic section GC; they are not linked frozen-owner
object or Android-device execution claims. Actual helper reconstruction
limitations remain. Leak checking is disabled for this sanitizer harness.

Combined target/native builds and all five repository checks pass. Linked
matching reaches **68.563050%**, **0.560590 percentage points** over current
main, with all **6,292** mapped exacts retained from checkpoint 15. The current
cycle still requires **70.002460%** and all eleven checks on its final head
before a squash merge. Batches 305–309 form checkpoint 16; fresh GitHub CI
verification is required after its push.

### Checkpoint 17: batches 310–315

All eleven GitHub checks passed on checkpoint 16's exact head `ad3413ac`.
The following integrations retain the same main baseline, **68.002460%**;
the merge threshold remains **70.002460%**, not a relative percentage gain.

**310, `NuMemoryManager::ReleaseUnreferencedPages`** (`0x0f2ce0`, 283 bytes):
restore the omitted empty, nonexternal page traversal under the existing
mutex. Cache both neighbors before unlinking the free span and calling the
live release handler: the actual Mem1 handler frees the allocation. On
success repair neighbors/head without accessing the freed page; on rejection
restore its bin membership. Advance using the cached next pointer. No
unverified statistics subtraction or `AddPage` change. One unchanged O2
whole-owner trial gains **159.731061 original-weighted bytes**, target
**17.273810% → 72.726190%**; all exacts and original symbols remain.

Root independently freshly compiles/runs canonical actual-body diagnostics
with actual constructor, bin/stat helpers and Mem1/free and Mem2/rejection
services: **3,906 exhaustive page sequences** pass on both host ASan/UBSan
with LeakSanitizer enabled and production-action NDK i386. Sequences cover
up to five pages, five page states and five allocation extents, including
head/middle/tail and consecutive releases. Native typed page/header storage
is supplied directly: the pre-existing host `AddPage` byte-20 first-header
and four-byte-footer layout debt is excluded, not repaired or validated.
The NDK diagnostic uses typed mutex recorders and verifies lock/unlock
counts; no concurrent allocator or Android-device test is claimed.

**311, `Animate_ASTROMECH`** (`0x16ca80`, 549 bytes): context-target fallback
tests canonical clip-table slot 5, not a word in the target-position record.
Both field-0x28 landing-idle comparisons admit unordered/nonpositive values
as the original `ucomiss/jbe` does. Other predicates and scheduler reloads
remain. One complete unchanged O2-owner trial gains **20.264551 weighted
bytes**, **29.426470% → 33.117645%**; all 80 neighbors, nine owner exacts
and the original symbol surface remain.

Root independently reruns freshly compiled host sanitizer and NDK i386
diagnostics: each passes **100,004 cases**, all eleven selection/scheduler
route bits, and four baseline-failure controls. Actual idle scheduler,
ResetCharacterIdle and pure UseFallAnim execute; NewCharacterIdle compiles
but expiry is excluded. GetDefaultIdle uses a declared bounded typed pure
1/25 query because its existing byte-100/472 pointer-table accesses have
wider-host debt. RNG/duration are bounded recorders, not actual services.
Nonfinite inputs exercise comparisons only; no NaN arithmetic/conversion
or full scheduler, helper-layout, game/device validation is claimed.

**312, `SetHeadTarget`** (`0x46fab0`, 263 bytes): round the RNG fraction once
before multiplying either delay endpoint, preserving the retail finite
arithmetic association and exact literal. Whole unchanged O2-owner gain
**41.618598 weighted bytes**, **28.017544% → 43.842106%**; existing exacts
and canonical text/data surface remain. Root's fresh host sanitizer and
NDK-action extracted-body/i386 diagnostics each pass **463,753 cases /
4,173,780 checks**, including all RNG states and signed priority pairs.
There are **59,071 finite grouping witnesses**; the freshly compiled old
body fails 120,824 checks. Actual pure qrand executes. All guards, alias
cache behavior and full object bytes are checked; no device test or
full-owner-object fixture link is claimed.

**313, `GizmoBlowupTypeRemove`** (`0x4b8930`, 428 bytes): remap references
through allocated type capacity, while compacting only active records.
Keep active-removal validation and explicit upstream fallback for absent
or undersized level capacity outside the valid retail domain. The tiny
positive unchanged O3 whole-owner gain is **0.178330 weighted bytes**,
**10.600000% → 10.641666%**; all neighbors/exacts/original symbols remain.
Root freshly reruns host sanitizer and production-action extracted-body
NDK i386 diagnostics: **2,436 checks / 84 calls** each, comprising 72 valid
retail calls and twelve separately labelled safety cases. Canonical typed
records and independent index/compaction oracle verify entire arrays;
the old body fails the allocated-tail-reference control. No observed
production caller or demonstrated gameplay defect is claimed.

**314 reserve, `ShaderMtlDescFilter::internalInit`** (`0x309240`, 374 bytes):
the complete base-variant/bit-0x10 field-18 predicate and canonical material
attribute-byte anchor are raw/type/producer/consumer supported. The sole
unchanged O3-owner trial nevertheless loses **100.256130 weighted bytes**,
**26.806452% → 0%**. All other bodies/exacts/symbols remain. Do not integrate,
split or retry the unit; no runtime fixtures were run. Generate3's separate
raw material-byte wider-host debt is not repaired by this proposal.

Each positive integration passed full target/native builds and all five
repository checks, with **6,292 exacts retained**. Combined linked matching
is **68.567780%**, **+0.565320 percentage points** over main. These local
changes require their own pushed-head GitHub checks before any merge.

**315, four AI toggle actions**: restore complete `CanAttack`, `NoIdleSpeed`,
`SetDontMove` and `NotWithParty` caller contracts in the existing AI owner.
Default target is the owner's canonical `apiobj.objptr`; character queries
use the original case-insensitive substring and passed-system lookup,
including `myself`, and replace the target even when lookup fails. NoIdle
requires an initial target before parsing. DontMove's FALSE rule reads the
first parameter. Shared parser and UseOne source remain unchanged; existing
NULL-entry safety is retained outside the original string-helper domain.

One unchanged O3 whole-owner gate gains **459.899672 weighted bytes**, with
no lost exacts or original/exported symbols. Ordinary compiler elimination
of the synthetic, non-retail shared helper and generated `.part`/startup-name
differences do not justify forced retention or a compiler variant. UseOne's
natural inlining score loss is included in the reported net gain. Root
verifies exact extraction of all four bodies, five actual string helpers
and actual named lookup, then freshly rebuilds host sanitizer and each
helper's own production-action NDK object/i386 diagnostics: **24,784 ordinary
cases + 5,520 separately marked NULL-entry safety cases / 202,741 checks**
pass on each. Host LeakSanitizer passes. A typed named-object callback records
only real query/return effects; no fabricated mutations or gameplay test.
Target/native builds and all five repository checks pass. Combined linked
matching is **68.577610%**, **+0.575150 points** over main, retaining all
**6,292 exacts**. All eleven GitHub checks passed on independently verified
checkpoint 17 head `03e7691d2afdb2fcb30f555a65b422a1983dd538`.

### Closed frontiers and structural prerequisites after checkpoint 16

Fresh bounded raw/body censuses do not reopen previously complete hint/input,
Origin/FindGameObject/NearestBreak, Huffman/Implode, line/circle rendering,
RGB/font/save UI, shader-transpose, TerrainImpactNorm, PodCollision,
WallShuffle, CCsfxmisc, JumpAnimCode or RigidUpdate units. Prior neutral and
negative whole-unit trials remain reserved. These are bounded read-only
contract checks, not exhaustive runtime-equivalence claims; unchanged bytes
alone are not evidence that a low matching score is fixable by another trial.

`NuGScnReadForMultiRender` remains a genuine loader stub with a structural
prerequisite: retail shallow-copies **0x20c** bytes, but canonical `NUGSCN`
ends at **0x1f8**. Generic serialized-size allocation/relocation does not
name or qualify the missing five words. The grouped private graphics reader,
GHG fixup/readers and multi-render callers also require verified local call
ownership; neither original filename nor forged register ABI is justified.
Recover supported-version tail fields, producers and allocation bounds before
implementation. No padding clone, invented ownership or option change.

Visibility-tree restoration likewise still needs producer-qualified node
storage and bounds. Raw box-node **32-byte** and instance-node **12-byte**
field accesses do not establish tree-array capacities. A follow-up identifies
retail `myvisi` as a four-byte BSS borrowed context alias at `0x11a2a20`,
assigned by `NuVisiInstTree`, not an allocation or tree owner. Original
two-bit output at context byte-20 is not the
one-bit occlusion result at byte-24. Camera-plane helper availability alone
does not justify a wrapper-only restoration or guessed format conversion.

### Reserved complete units after checkpoint 17

The following sole unchanged-action whole-owner trials remain unintegrated;
no runtime fixtures or source variants were spent on their neutral/negative
results. All retain existing exact functions and the original symbol surface.

- **316, MoveToMarker::Render**: original bob/height/position association and
  unsigned alpha-word packing form a complete correction. The canonical
  `move.cpp` replay is net neutral (0% target before/after, 780 → 776 bytes).
  The preliminary temporary-basename startup-symbol mismatch is superseded
  by that replay, not evidence for another source trial.
- **317, GameAntinodeUpdate**: original timer admission is ordered `> 0` and
  expiry ordered `<= 0`; the current negated comparisons differ for NaN.
  Complete correction loses **151.539065 weighted original bytes**,
  **46.148026% → 33.424343%**. No production NaN witness is claimed.
- **318, NuMemoryManager::_TryBlockAlloc**: restore actual bin selection,
  dirty large-bin sorting, aligned carving and recursive page request.
  Preserve the raw bitmap OR-of-bin-index oddity and upstream null-handler
  guard. Complete unchanged O2 unit loses **359.360386 weighted bytes**,
  including DumpBlock collateral; target **25.876574% → 1.534005%**.
- **320, AreasOpenAll**: save mode byte is `field_0x5[2]`, not the completion
  byte, and completion-point recalculation is outside the optional hub gate.
  Complete correction is neutral, **18.891026%** before/after. Actual store
  query and empty hub helper are not replaced with invented effects.
- **321, GizSpinnerUpdate**: restore the finite nonzero below-tiptoe speed
  floor while retaining existing nonfinite guard policy. Actual O3 owner,
  not an O2 override, loses **30.844041 weighted bytes**,
  **39.826004% → 38.650097%**. Unpublished pure scratch translation is not
  manufactured as observable rendering.
- **322, ConvertToUsedBlock**: retail normal-footer range includes manager
  index 29 (`cmp 0x1d/jbe`); extended footer starts at 30. Debug unknown bits
  start at zero while the context field keeps its five bits. Complete paired
  correction loses **22.024125 weighted bytes**,
  **56.277420% → 52.090324%**. Neither half is integrated independently.

### Bounded serialized-scene producer follow-up

Read-only analysis of all **1,133 loose GSC/GHG roots** in both local saga
archives finds **1,125 format-two** and **eight format-one** records. Root
words `+1f8/+1fc/+200/+204` are registered nullable relative-pointer slots in
every record, all NULL. `+208` is a registered nonnull pointer in all eight
format-one records and an unregistered zero word in format two. Thus the
fifth word is not a universal scalar. No semantic member name, pointee type,
or individual allocation boundary is established; no canonical padding or
loader implementation is proposed from these observations.

All observed roots have at least **0x418 bytes** remaining in their decoded
blob, so a 0x20c copy fits this observed input domain only. Decoded scene
content agrees byte-for-byte between archives (sorted-name aggregate SHA-256
`2be7909ec2a946f8785c6940e32172a18db6f36d0397c030b285de4a875121ff`).
The backup requires retail TT block-kind interpretation, verified against
`DecodeDeflated`; the scoped parser translates a single final block header
in memory and checks decoded length and complete consumption. No asset is
written or repacked. Nested PAKs and arbitrary accepted versions are not
covered. Every observed scene's `+134` instance-tree pointer is NULL, so
these assets do not resolve the missing tree-array capacity contract.

The actual graphics reader supplies the tree via a generic relocated
residual scene blob, not a recovered typed runtime builder. Generic fixup's
relocation count is not a node count. The independently identified
`DisplayListBeforeFrame.constprop.173` resets display-scene two-bit buffers,
not Evaluate's separate scratch or one-bit occlusion buffer. Box-tree
context ownership and nonnull supported instance-tree producer evidence
remain prerequisites; no guessed buffer conversion or type expansion.

### Checkpoint 18: allocator, Unicode and cutscene lifetime contracts

**319, NuMemoryManager::_BlockReAlloc** restores actual address/allocation/end
validation, fixed-width footer owner dispatch, quantized payload equality,
original encoded alignment and the real `_BlockAlloc` fallback. Shrinking
blocks now relocate when their quantized payload differs; copy spans use the
smaller payload. Existing NULL-input, zero-size and replacement-failure guards
remain. The sole unchanged O2 whole-owner gate gains **178.65577184 weighted
original bytes**, target **29.346940% → 60.911564%** (318 → 590 bytes).
All existing owner exacts and nonliteral names/types remain; the only new
literal is the original-backed ReAlloc `__FUNCTION__` string.

Root independently rebuilt and ran **432 host ASan/UBSan cases** with leak
detection enabled, and **400 i386 cases linked against the full frozen NDK
owner object**. Host tests execute the real constructor, flag/size calculation
and ReAlloc body with declared typed allocation/free/validation doubles, not
the host allocator. i386 tests use real pages, bins, allocation, free, validation,
Mem1/Mem2 services and release, with Bionic pthread diagnostic adapters.
Normal/extended manager indices 0/1/28/30/31/255 and modes 0/4/12 are covered,
including bounded fallback/failure, ownership routing, alignment, flags and
payload canaries. Both old-body controls independently confirm three old
behavior witnesses; those observations are not retail-oracle successes.
Index 29, nonzero debug context, destructor context cleanup and concurrency
are excluded for separately documented helper debts. No rejected batch-322
helper repair is smuggled into this unit; native64 full allocator correctness
is not claimed.

**323, UnicodeToIndexFast** restores initial midpoint, old-midpoint boundary
updates, cached unsigned key and high-then-low adjacent termination. Original
sorted duplicate-key ordinals are preserved, including `[65,65,65,66]` selecting
ordinal two rather than the old body's ordinal one. Existing count/upper-key
guards remain, with an explicit singleton equality/miss safeguard rather than
restoring original stalls or out-of-bounds reads. No assertion is made that
shipped fonts actually contain duplicate keys. Sole unchanged O2 whole-owner
gain is **7.8658246 weighted bytes**, **11.886076% → 15.177216%**;
all 29 owner exacts and nonliteral data identities/extents remain.

Root rebuilt actual lookup, EncodeChar, UTF8toQCode, PrintLenU,
EncodeUnicodeString and UTF8 decoder bodies under host sanitizers and their
actual per-owner NDK O2/O3/default-O0 actions. **49,351 cases / 133,250 checks**
pass on each ABI, including odd/even sorted maps of length 2–257, duplicate
keys, exact-size singleton/pair allocations, canaries and 128 font helper cases.
Host ASan/UBSan/float-cast-overflow and LeakSanitizer pass. Both old-body
controls fail exactly five duplicate-key lookup/encoding/width oracle checks,
without sanitizer errors. A fixture-only `word` name collision was corrected
to `float_bits`; no production variant was introduced. Malformed UTF8,
unsorted maps, enormous counts and device/GL execution are not covered.

**326, instNuGCutSceneDestroy** restores optional character-data cleanup after
real End and before list unlink. Owners are captured after End, live arrays are
reloaded per record and the unsigned count is refreshed only after an actual
callback return. Flag-two characters do not require a nonnull model for this
callback. The sole unchanged O2 whole-owner gate gains **133.12512841 weighted
bytes**, **25.921053% → 85.618420%** (115 → 231 bytes), retaining both exacts.
The sole new undefined reference is the verified original
`NuCutSceneCharacterDestroyData` callback object; all existing names/types and
data extents remain. Current InitSystem does not register a concrete DestroyData
service, so this restores the optional API contract, not a claimed active
gameplay destructor.

Root fully reviewed and independently ran **100,000 randomized + 13 directed
cases** on host ASan/UBSan/LeakSanitizer and NDK-produced i386. Canonical records,
actual callback storage/setter and complete actual End, EndButNotSystems and
ResetCamLock bodies execute. Both old-body controls abort on the same missing
cleanup event oracle. Typed Eval/Release/Destroy API recorders and six explicitly
diagnostic callback mutations test capture/live reload, callback identity,
bounded count changes and callbacks-before-unlink. They are not assertions
about registered gameplay services. Rigid and locator finalizers are absent
and guarded by abort seams; malformed capacities, geometry and Android hardware
are excluded. The initial NDK diagnostic link failed only because fixture
`fprintf(stderr)` referenced Bionic `__sF`; a frozen, historical-preserving
test-only revision uses `printf` and enables leak detection. Both repaired
diagnostic builds pass; production source and gated objects were unchanged.

Combined linked matching is **68.584400%**, **+0.581940 percentage points**
over current main. The whole-binary measure remains **6,292 exact functions**.
All **6,291 exact ELF-mapped records** from checkpoint 17 remain, checked by
original address/symbol including ambiguous and unassigned records; the ELF
mapping omits five objdiff records and is not identical to the aggregate count.
Target/native/WASM builds and all five repository checks
pass. This remains below the **70.002460%** cycle merge threshold.

### Fresh bounded censuses after checkpoint 18

Complete raw/current-body screens close the following fresh low-score units
without compiler trials or runtime-fixture expenditure: CreateScaledEffect,
terrainpickupinit, HitPoly, DebrisShift, edppDrawSpheres, Blowups_Reset,
GizSpinner_UsingSpecial, Episodes_ConfigureList, GenerateTrooperTeamShape,
ThingManager::ProcessThings, MovePlayer_GUNSHIPIN, Move_SPEEDERBIKE and
Move_DROIDGENERIC. This is qualified source/raw closure, not exhaustive
runtime or byte equivalence. Existing producer bounds and safety policy remain.

CreateScaledEffect's bounded twelve-byte temporary copy is deliberately not
replaced by retail's potentially overflowing strlen-plus-one copy. HitPoly's
primary `<=` versus secondary `<` and integer return accumulator are genuine
original contracts already present. Trooper formation-four copies the unrotated
input despite the intervening rotation; current code preserves that oddity.
Its arithmetic alternatives have no established difference under the actual
fixed-depth/count-derived producer domain. ThingManager already reloads count,
objects and profiling state across all three virtual passes.

Speeder-bike pad reload and subtype recheck are raw-proven distinctions, but
the actual intervening services do not establish a synchronous writer path;
AwkwardShape's PushAway excludes the bike and changes other objects' velocities.
Two unordered speed/timer predicates are likewise recorded without a valid
nonfinite producer. No artificial service mutation or NaN gameplay witness
qualifies a source trial. Droid self-destruct guards belong to its concrete
helper and are already present. Low scores alone do not reopen these units.

### Batches 324–325: save metadata and touch-effect display

**324, loadsaveCallEachFrame** restores ASCall first, nonzero card-change
latch, UpdateSaveSlots, then unconditional occupied-count reset and nonzero
slot counting when save-present is nonzero. Negative occupied words count.
Canonical six-slot capacity clamps oversized configuration; negative counts
perform no reads. The original four-byte BSS `saveload_cardchanged` is restored
in the framework owner with canonical C-linkage declaration, not an invented
platform writer. Sole unchanged-action gain is **76.76789382 weighted bytes**,
target **22.578947% → 79.868420%** (32 → 140 bytes). All fourteen config and
eleven framework exacts retain; framework text scores are unchanged. Five
existing BSS addresses shift by four bytes without changing types/extents.

Root rebuilt and ran **385 nonmutating dispatch + 70 actual-helper cases per
ABI**, with host ASan/UBSan/float-cast-overflow/LeakSanitizer. Actual ASCall,
UpdateSaveSlots, initialization/info scanning, asynchronous save/load, file
header writing, checksums, paths/strings and time bodies execute. All 64 file
occupancy masks, bounded payload round trips, strict time completion, guarded
malformed-buffer early returns, autosave postdelay and inter-call latch pass
in a cleaned private temporary directory. Clock/documents-path/synchronous-lock
services are declared diagnostics, not device/thread integration. Both old
dispatch controls exit 17 on the bounded-occupancy oracle, not a crash.

**325, MechSystems::Display** always renders TouchUI first, then only when
initialized is nonzero renders nonnull swipe slots 0–3 and radar slots 0–3
in separate ordinary loops. It does not initialize or render level UI/movers.
Canonical offsets are swipes **0x290c**, radars **0x2928**, flag **0x2938**.
Sole unchanged O2 gain is **11.964286 weighted bytes**, target
**18.089285% → 24.071428%** (39 → 113 bytes); the prior exact and nonliteral
surface/data extents retain. The two new Render references are original-backed.

Root's **694 cases per ABI** cover all 256 effect masks off/on, noncanonical
nonzero initialization, all 32 UI slots, menu/visibility gates, all 64 radar
stage masks off/on, styles, quarter-turn geometry, colours and half/full UVs.
Actual UI constructor/render, swipe/pulse render, pulse constructor, trig,
rotation and inline primitive writers execute. Canonical polymorphic objects
are normally constructed with declared lifecycle doubles and valid placement
storage, not fabricated vptrs/memset. Primitive/singleton/aspect recorders do
not claim full production constructors or hardware rendering. Arbitrary
geometry and nonnull level-UI sentinels are excluded. Both old controls exit
18 on missing swipe dispatch.

NDK fixture helper excerpts use the wrapper's O2 action, not each helper's
production optimization mode; separate whole-owner gates preserve all actual
actions. A missing standalone host `__FILENAME__=__FILE__` definition was
restored from the existing native build authority. Historical harness revisions
and hashes remain, with no actual body changes.

### Batch 327: original language IDs and pointer storage

Text_GetLanguagePath now maps **2 → French, 3 → Spanish, 5 → Italian**;
Japanese/German/Danish/default-English mappings remain. Seven original mutable
`char *txtpath_*` globals replace private local arrays, with verified D/4-byte
symbols, relocation addends and read-only initial literal pointees. No pointer
writer or writable literal bytes are inferred. Canonical API declarations and
C++ linkage remain. The sole unchanged O2 owner gate gains **80.83 weighted
bytes**, **39.540540% → 98.540540%** (95 → original 137 bytes), retaining all
exacts and 66/67 normalized bodies. The only surface changes are seven verified
globals and seven individually identified private arrays, plus two zero-size
LOCAL NOTYPE selector jump labels independently checked by raw/readelf.
No general name/ABI normalization or source variant was used.

Root rebuilt/reran **26,385 cases / 175,509 checks per ABI**, with host
ASan/UBSan/LeakSanitizer, actual selector/LoadStrings/SetLanguage and real
NuStrCpy/Cat. Bounded parser/intro/locale recorders verify paths, service order,
buffer alignment and unchanged arena. Signed-ID boundaries, exhaustive
[-4096,4096] and 4,096 representative bit patterns are not all 2^32 IDs.
All 21×21 current/input language pairs run with optional callback present/NULL.
Diagnostic replaced/NULL global pointer identities are selector-only; no
invalid path is passed to LoadStrings. Six focused old controls plus the old
full grid all assert against the same original mapping oracle. NDK text/string
excerpts use the text owner's O2 action; typed recorder driver is host i386 SSE,
not each helper's own production optimization or Android locale/file testing.

### Batches 329–330: turret damage routing and fade edge conversion

**329, GizTurrets_BoltHitPlat** keeps the original unconditional pure type
lookup after a platform match. Owner restrictions now suppress **damage**,
not owner attribution; every nonnull owner retains its signed player slot.
The restricted path lazily avoids dereferencing the returned type, including
a lookup miss. Unrestricted missing types remain outside the valid contract.
Existing system/count, active/visible/exclusion and animation traversal gates
remain. Sole unchanged O3 whole-owner gain is **108.143395548 weighted bytes**,
**7.5283017% → 37.905660%**, retaining all six exacts and every emitted name,
kind and data extent. Canonical source basename staging preserves the actual
compiler-generated startup identity; it is not an option or source variant.

Root independently compiled and ran **1,083 calls / 26,964 checks per ABI**,
plus two directed candidate modes. Actual Plat, Hit, FindByID, qrand, trig,
vector rotation/addition and set visibility execute. Canonical eight-slot
Player storage is populated; slots -1/0/1/7, global/world type boundaries,
three restricted NULL-type cases, health-byte wrapping, gates and first-match
traversal are bounded. Typed camera/audio/visibility/pickup/hint/centre seams
record order without mutating gameplay inputs. Both safe old-body controls
abort on independent damage or attribution assertions, not NULL dereferences.
Host ASan/UBSan/float-cast-overflow/LeakSanitizer pass; the NDK diagnostic uses
the actual turret owner's O3 action for extracted helpers as well, not each
helper's individual production action. Device/gameplay concurrency is excluded.

**330, DrawFadeScreenWipe** reads the original FadeSys storage directly and
retains reverse-edge float products through gradient-start conversion:
`int(product - gradient_width)`, independently of the solid edge's truncation.
The original negated ordered rate/fade predicates are restored in the same
complete unit. Current startup aliases pFadeInfo to FadeSys; no ordinary
pointer-reassignment or BeginScene mutation is claimed. Sole unchanged O3
owner gain is **41.50131832 weighted bytes**, **18.094890% → 24.832117%**,
retaining all thirteen exacts and all names/types/data extents. The only
undefined-reference removal is pFadeInfo, with FadeSys already present.

Root independently rebuilt/reran **9,225 cases / 571,180 checks per ABI**,
using actual fade, BeginScene, EndScene, gradient/solid rectangle bodies and
canonical colour/UV writers. Typed primitive capture checks coordinates,
materials, order, packed/full UVs, stale-scene reset and actual scene-ring copy.
Finite fades cover negative/zero/fractional/one/above-one, direction precedence
and rates including unordered/infinite values. Eight no-edge NaN-fade cases
avoid every integer conversion. Arbitrary nonfinite/out-of-range edge fades
are excluded. A replaced pFadeInfo case is explicitly diagnostic only.
The old body passes 192 stable cases and fails six separate expected oracle
controls on each ABI. Host sanitizer/leak/float-conversion checks pass; NDK
helpers use the fade owner's actual O3 action with fixture-only section GC,
not their individual owner options or GPU execution. A Bionic-specific oracle
isfinitef link dependency was replaced only in the test driver by an IEEE-bit
finite check; historical harness inputs and the unchanged actual bodies remain.

Checkpoint 18's exact head **e81fe6388a5a7ad1ac11fbcc2cc9156d459e6b93**
passed all eleven GitHub checks. Its initial repository-check dependency fetch
failed with GitHub HTTP 500 before tests; the failed-job retry passed.

### Batch 327 bounded unfinished-frontier exhaustion

The object/editor census does not identify three new unfinished >=500-byte
units. ImplodeMakeTree and NuErrorSleep retain their prior structural blockers;
scene/visibility prerequisites remain separate. Full raw/current checks close
routeEditor_cbRenameRoute, pathEditor_QuickOnPathCheck and SpaceResetAudioPoint:
the smaller bodies express retail expansion or private ABI, not missing
ordinary behavior. Actual music setters do not mutate the door timer, so no
artificial reload witness or compiler trial is introduced.

The terrain/support screen covers 282 report functions, 76 >=500 bytes, with
no explicit unfinished marker in its five direct owners. ReadTerrain has a
complete current descriptor/spatial-bound path; pickup was already closed.
rtlLoadSet was recovered earlier, edppLoadPage is a prior neutral reserve,
and edbriLoadPage has a bounded current reader without a newly qualified
omission. This is a bounded source/frontier screen, not full retail equivalence
of every loader. Packed asset/native-pointer decoding remains preexisting
debt; no guessed widening, scene-tail fields or allocation sizes are added.

### Checkpoint 19 linked integration

The combined batches **324/325/327/329/330** reach **68.591210%**, or
**+0.588750 percentage points** over current main. Target/native/WASM builds
and all five repository tests pass. Root independently compares the whole
raw objdiff address/symbol exact set against checkpoint 18: **all 6,292 exact
records remain**, including the exact thunk omitted by ELF function mapping.
This full raw check supersedes relying only on the 6,291 mapped exact records.
The cycle remains below its **70.002460%** merge threshold.

Checkpoint 19's exact pushed head **b82bc221ed8bba3e3877af2eaa87e2a5ef245dac**
passed all eleven GitHub checks; root independently verified their conclusions.

### Batches 331–335: bounded contract gates and reserves

**331, Animate_SUPERBATTLEDROID**, restores the complete original clip fallback
and finite-state admission contract in an isolated unchanged O2 owner. Its sole
whole-owner gate is negative: **−16.16098848 original-weighted bytes**,
**32.496933% → 30.092024%**, retaining all nine exacts and the complete symbol,
undefined-reference and nonfunction extent surface. The complete unit remains
reserved; no partial variant, second compiler trial or runtime fixture follows.

**332, NuAnimBuffProceduralAnimation**, restores the missing upper clamp on
all three wrapped signed16 rotation axes, preserving the original reversed-
limit precedence, unlimited signed32 angles and all map/translation/scale
paths. The sole unchanged O3 whole-owner gate gains **8.6281698 weighted bytes**,
**68.910830% → 70.273890%**, preserving forty exacts and the entire symbol/U/data
surface. Actual object text is **601 → 633 bytes** by nm; the isolated gate's
zero target-size metadata came from an absent report key, not emitted size.

Root independently rebuilt and ran **10,000 finite random + 141 directed
cases / 1,549,404 checks per ABI**, with host ASan/UBSan/float-cast-overflow/
LeakSanitizer and the actual NDK O3 action. Complete actual bodies and the real
setter/storage execute through a diagnostic function-pointer dispatch, not a
full asset loader/evaluator or gameplay path. Canonical bounded pose/map arrays
check every pose scalar bitwise, unchanged inputs and buffer capacities, all
flags, limits, wrap, repeated mapped joints, skipped map255 and global fallback.
Three safe old-body controls fail the independent upper-limit oracle on each
ABI. A test-only Bionic stdout link dependency was removed with `fflush(NULL)`;
historical manifests remain and both ABIs were freshly rerun afterward.

**334, BlendRootFn**, corrects all three complete source/target interpolation
argument orders in isolation. Its sole unchanged O0 owner gate is **neutral**:
**66.500000% → 66.500000%**, 1,504 bytes on both sides, all seven exacts and
the full surface retained. Original duplicate LOCAL helper records are paired
by objdiff's scored record, not guessed source identity. The complete unit is
reserved without runtime fixtures, partial variants or recompilation.

**335, edrtlCalculateBurnout**, restores the raw-proven increasing-dispersion
MAX selection; other threshold/intensity/hysteresis and nearest32 paths stay
unchanged. The sole unchanged O0 whole-owner gate gains just **0.034391 weighted
bytes**, **64.772110% → 64.773810%**, preserving fourteen exacts and all symbol,
undefined and nonfunction extents. This is a tiny gain, not a broad matching fix.
Temporary storage exhaustion stopped assembly before candidate object creation;
root preserved logs, recoverably relocated old scratch artifacts and resumed
only the identical frozen candidate, reusing the original baseline.

Root independently rebuilt/reran **178 cases / 1,948 checks per ABI**, plus
**152 stable old cases / 1,662 checks** and two clean old dispersion assertions.
Actual distance/vector/sqrt helpers and canonical32 burnout records execute;
finite radii, nearest/tie/edge selection, ordinary channel drift, instant/latch
and local-output bounds are covered. Host leak/undefined/address/float-cast
sanitizers pass. NDK excerpts use the actual O0 owner action with fixture-only
section GC, not each helper's separate optimization or Android/GPU execution.
No alias, NaN or fabricated service mutation is used as evidence.

**333, LoadPerm2**, restores the entire omitted customiser/special-move/status-
music/purchase-restoration tail and the original public i16[5] animation
allow-list in isolation. Its sole unchanged O3 owner gate is negative:
**−8.596875 weighted bytes**, **48.542970% → 47.886720%**, 894 → 1,311 bytes
(original 1,310). All fifty other normalized bodies and the strict existing
surface retain; only original tail dependencies, ten-byte data and three
original literal values are added. Root independently verified the identical
twelve-byte LOCAL compiler switch table despite its numeric suffix change.
The complete unit is reserved without production integration, partial trials
or diagnostic fixtures; a missing original contract does not guarantee a gain.

The linked **332/335** integration measures **68.591390%**, retaining every
one of checkpoint 19's **6,292 raw exact address/symbol records**. Target build
passes; broader checkpoint verification follows with the remaining units.

### Checkpoint 19 structural and fresh-frontier audit

The fresh LOCAL-xref ledger has **3,285/3,287** binding/size-concordant unique
writable-state relationships co-owned, **59.850692%** assessable coverage.
The only qualifying split is component 513's two links from particle save
and scene lookup to the original LOCAL sixteen-byte `nullobjectname` array.
All three current owners use O2; their existing substitutions have identical
name content and no proved mutating producer or missing behavior. This is
one minimum component (five functions/two objects), not three broad fixes or
a promised matching gain. Other ranked splits rely on GLOBAL substitutes and
fail the strict binding criterion. The ledger explicitly precedes 332/335;
its later stale-input rejection was retained, not bypassed by retimestamping.

The broader owner-weight audit supplies no verified compiler/type/TU-wide
fix. Motion, terrain and editor weighted deficits identify workload, not
recoverable gain. Earlier neutral/negative ANI3 merges and scene-tail producer
blockers remain reserved. The historical editor repeat-box ownership split
is already resolved by textual inclusion and real menu registration; the
translation-unit overview now warns against reopening it from stale figures.

A bounded fresh menu screen closes store-restoration update, view-text draw,
hint processing, shop selection and filename filtering without a new supported
missing contract. Unsigned menu-flash distinctions have no negative current
producer; BuyShop's actual return is always one. A dispatch screen likewise
closes LevelStreamingUpdate and the cannon-name contract. Partial laser draw
screening supplies no newly qualified body/helper/layout lead; it is not full
retail equivalence. No source variants or fixtures follow these closed screens.

### Batch 336: complete hub reset contracts

`Hub_Reset` restores the original 20–51 show / 52–83 hide dispatch using
defined modulo64 consumer bits. The two visibility tests are independent;
the PSP hide range no longer depends on its paired show bit. Existing nullable
special, scene, instance, display and episode-door guards remain. Real
`Episode_CountOpenAreas` runs before the force-open fallback, retaining all
sixteen aggregate-counter side effects even when the force flag is set.

The sole unchanged O3 complete-owner gate gains **170.24539221 weighted bytes**,
**42.273197% → 65.690720%**, 566 → 662 bytes (original 727). All fifty-one
other normalized bodies, existing exacts and the complete symbol/U/data surface
retain. Fourteen renumbered zero-sized LOCAL NOTYPE compiler labels were
individually verified; only those precise pairs were excluded from name
comparison. The gate reused the same objects after diagnostic parser repair,
without a source variant or recompilation.

Root independently rebuilt and ran **258 cases / 31,980 checks per ABI**:
162 canonical visibility/guard cases and 96 actual-counter/force/door cases.
Actual reset, door, count, visibility and existence bodies execute with their
canonical tables and bounded records. Host address/undefined/leak sanitizers
pass; NDK excerpts use the hub O3 action with a host-i386 diagnostic driver,
not full Android gameplay or each helper's separate owner action. Both safe
old-body controls fail only their expected assertions. The preexisting
`CheckResetBits` producer's shifts beyond63 are explicitly excluded and
unchanged; there is no claimed exhaustive superbonus/area-policy validation.

The linked **332/335/336** result is **68.595024%**, **+0.592564 percentage
points** from current main, retaining all **6,292 raw exact address/symbol
records**. The cycle remains below **70.002460%**.

### Fresh AI census: resolved GOT identities, no trial

A three-function motion/AI screen closes `AIPathCnxControlSysUpdate`,
`AISysGetCharacterPathPos` and `Action_SetAIOverrideControl` without a supported
finite omission. The initial case5 hypothesis swapped END and CURRENT GOT
entries. Root resolved both RELATIVE pointers and raw loads: 17c8b4 converts
CURRENT, and 17c8ba tests CURRENT, agreeing with the complete Ghidra reference
and current source. Actual route/reset helpers account for the other apparent
missing writes. The premise is retracted; no candidate, compiler trial,
fixture or expanded search follows this closed census.

### Checkpoint 20 verification

The integrated 332/335/336 checkpoint passes target, native and WASM builds,
all five repository tests and the complete format/tidy/forward-declaration/
symbol/report commit hook. Its fresh mapped and raw reports agree at
**68.595024%**; the complete raw exact address/symbol set retains all **6,292**
records from checkpoint19. No compiler options, calling conventions, public
layout or assets changed. Root independently verified all eleven GitHub checks
passed on exact pushed head `5e4a6cb2e2cbbcd955f8a0be788cf672c7bbf9fa`.

### Batch 347: complete turret priority unit reserved

`GizTurrets_OpponentSelection` restores the original unconditional first
admitted candidate in priority10/8 modes, retaining pure-distance mode's1e9
sentinel, priority precedence, tie comparisons, nullable guards and all other
filters. Canonical finite serialized coordinates/radii admit the discrepancy;
no particular shipped large-radius asset is asserted. Safe repeated signed
cursor operation requires a nonnegative initialized cursor and count<=127;
the unrelated count128 wrap debt is unchanged.

Its sole unchanged O3 complete-owner gate is **neutral: zero weighted bytes**,
39 emitted / 37 original-backed functions, every original score unchanged,
all six exacts and the complete readelf symbol/type/bind/visibility/storage/
nonfunction-extent/multiplicity and U Counters retained. Two bulk reports
verify unique scored LOCAL pair identities against original raw addresses.
The complete unit is reserved without production integration, runtime fixtures,
recompilation or partial/source-shape variants. The accompanying bounded
ChangeTechnoTgt/pickup-collision census finds no supported finite omission.

### Checkpoint20 fresh object census

Exactly three further fresh screens close `LevelObjects_InitForLevel`,
`ActivateCharacter` and `FinishWeirdoNames` without qualified omissions.
Resolved scene switch/GOT routing and actual lookup/surface/path helpers
account for the first two complete bodies. The third already expresses the
original uppercasing/space-padding contract; raw vectorization is not missing
behavior. Existing arena/count/name-translation storage debts are recorded,
not guessed repaired. No exports, compiler trials or fixtures follow this
closed census.

### Batch 349: extras-menu fade and row-colour contract

`MenuDrawExtras` restores the original absolute-Y fade: thresholds0.15/0.6,
the exact positive divisor `0.45000002f` (IEEE0x3ee66667), and explicit signed
integer then byte alpha conversion. Rows8–43 select custom RGB independently
of purchase status; every row publishes223/63/0. Existing nullable name/text
fallbacks, bounded formatting, purchase/bonus checks and menu guards retain.

The sole unchanged O3 full-owner gate gains **151.8541904 weighted bytes**,
**32.556652% → 49.812810%**,800 → 824 bytes (original880). All seventeen
exacts and the complete readelf symbol/type/bind/visibility/storage/nonfunction
extent/multiplicity and undefined-reference Counters retain. Two original
bulk reports verify the same unique original raw-address/size pairs; only
the requested original score changes. No compiler-label exception is needed.

Root independently built and ran **332 cases per ABI** with complete actual
draw/update/cheat/scroll/row/header/text-wrapper bodies, all400 canonical menu
geometry slots, ordered leaf traces, fade boundaries, RGB reset side effects
and purchase controls. Host address/undefined/float-cast/leak sanitizers and
NDK32 diagnostics pass; both safe old-body controls fail their named assertion
without a crash. Font/GPU, controller, audio lookup and navigation are declared
stable recorder seams, not full gameplay. Helpers use the diagnostic fixture
owner's options, not claims of each separate helper action. Inputs are finite,
with canonical MenuA0–128 and bounded typed menu/save/cheat storage.

### Batches 352/354: complete negative units reserved

The ordinary `StartDoorPositions` i32 result contract preserves all spline
publications but its sole unchanged O3 full-owner gate loses **43.1721171
weighted bytes**,74.945740% → 70.914730%. All thirty-three original-backed
functions, five exacts and complete symbol/U Counters retain. Existing callers
ignore the result; no new gameplay consumer is claimed.

The complete `DrawMiniSnowTroopers` original16384 phase/rate unit loses
**9.07044796 weighted bytes**,48.215233% → 47.572850%, across twenty-two
original-backed functions. Undefined references retain; compiler labels38/39
change storage and40 is added. Negative net already closes the unit, so no
compiler-label exemption or parser repair follows. Both units remain frozen
reserves without source variants, production integration or runtime fixtures.

The complete NULL-index `NuHGobjRndrMtxDwa` count-one contract (batch353)
likewise loses **13.9692366 weighted bytes**,54.844370% → 53.599340%, in its
sole unchanged O3 gate. All246 original-backed functions, forty exacts and
complete symbol/U Counters retain. The first preparation's permissive literal
filter and missing-score default were corrected before any compilation; the
candidate remained byte-identical. It is reserved without integration or
fixtures; current callers' non-NULL lists are not a gameplay NULL producer.

### Batches 348/351: unknown comparison coverage remains unqualified

Component513's six frozen O2 objects were compiled once. The first detailed
bulk diagnostic failed in objdiff's section-order pass before serialization;
its empty output is retained, not treated as a score. Six fast summary reports
reuse those same objects. The deduplicated union has585 original function
records, no ambiguous physical pairings or pairing drift, retained prior
exacts/private helpers/external-U closure, but thirteen unknown records and
unqualified compiler-label/data-extent surface changes. Its known paired
subtotal is -4.5830496 bytes; full net remains bounded
[-3831.5830496,3822.4169504], **not a measured negative or positive**. No
partial ownership variant, score normalization or fixture follows.

Batch351's complete `NuGScnUpdate` waiting-frame candidate remains frozen
after one O3 baseline/candidate compile. Three omitted-score rows account for
3026 original bytes; missing scores are not assigned zero. Two emitted entries
move, and `NuDisplaySceneRndr` has a nonrelocated same-text local call that a
relocation-only dependency parser does not cover. Whole-owner net remains
unknown, not negative/neutral. No production integration or fixture follows
this unqualified owner gate. A future full-linked trial of the same candidate
would require separate explicit verification, not silently dropping coverage.

### Checkpoint 21 verification

The integrated extras-menu repair passes target, native and WASM builds, all
five repository tests and the complete format/tidy/forward-declaration/symbol/
report hook. Fresh mapped and raw linked reports agree at **68.598274%**,
**+0.595814 percentage points** from current main, with all **6,292** original
exact address/symbol records retained. Production owner bytes equal the
sole frozen positive candidate after formatting. Root independently verified
all eleven GitHub checks passed on exact pushed head
`875d714983db21ca6d83df9085eaa5c50fb9a7c7`; the cycle remains below
**70.002460%**.

### Batch 356: explicit matrix serialization reserved

The original sixteen named matrix-field endian calls in `Bolt_Init` are
expressed using canonical `NUMTX` members in offset order, retaining the
swap flag, payload copy, cursor advancement and every other owner byte.
This is a source-form proposal, not a missing operation or gameplay fix.
The sole unchanged O3 complete-owner gate loses **478.4467935 weighted
bytes**, **58.661440% → 47.993730%**. All sixty-three original-backed
functions, five exacts and complete symbol/storage/nonfunction-extent/U
Counters retain. An ambient-environment preparation error was corrected
before compilation; actual action environment and arguments were preserved.
The negative unit remains frozen without production integration, variants
or runtime-fixture expenditure.

### Batch 350: network-player tag-state update

`MovePlayer_NETWORK` restores the original unconditional `Tag_Check` call
after glow-model selection and before `ForcePushCode`. The later terrain tag
update remains separate. The unchanged O2 complete-owner gate gains
**32.87102942 weighted bytes**,69.889534% → 71.909880%,1627 → 1643 bytes
(original1627). All four owner exacts retain; the only undefined-reference
addition is the original-backed `Tag_Check`. The unscored rendering helper
has complete byte/relocation/referenced-storage and decoded branch-target
invariance, proving zero delta without assigning an absolute missing score.

Root independently runs **974 cases / 40,041 checks per ABI** with complete
actual network/tag/takeover/math/helper bodies. Host address/undefined/leak
sanitizers and NDK32 diagnostics pass. Each safe old-body control fails exactly
four named tag flag/HUD/context-delay/pause assertions without a crash.
Controller/GPU/audio/navigation/combat leaf services are declared stable
recorders, not full gameplay. Actual finite takeover publication executes;
remote-bit admission and context51 are explicitly seeded states, not claims
that unrecovered outer producers execute. Helper excerpts use their own
production actions. Fixture-only declaration/include/event-name repairs
preserve actual source bodies and historical failed diagnostics.

Fresh mapped/raw linked reports agree at **68.598970%**, retaining all
**6,292** exact address/symbol records from checkpoint21. Target, native and
WASM builds pass. The cycle remains below **70.002460%**.

### Batches 357/360: frozen reserves

The original signed curve-type capture in `NuHGobjEvalDwaBlend2` is expressed
once per existing arm without changing serialized u8 storage or supported
curve behavior. Its sole unchanged O3 complete-owner gate loses
**10.82043056 weighted bytes**,31.273912% → 30.382608%. All246 original-backed
functions, forty exacts and full symbol/storage/undefined-reference Counters
retain. No integration, variants or runtime fixture follow the negative unit.

The complete best-bolt-target1e9 sentinel candidate has zero known weighted
delta across46 uniquely scored original-backed functions, with eight known
exacts retained. `GizForces_BoltHit`1045B and `StoreProgress`2960B remain
unscored; full net is **unknown**, not measured neutral. Ten removed/nine
added named compiler-literal/storage rows fail the strict surface gate,
although undefined references retain. It remains unqualified without
recompilation, report reruns, speculative score normalization or fixtures.

### Checkpoint 22 verification

The integrated network tag-state repair passes target, native and WASM
builds, all five repository tests and the complete forward-declaration/tidy/
symbol/report pre-commit checks. Fresh linked reports agree at
**68.598970%**, **+0.596510 percentage points** from current main; the complete
raw exact address/symbol set retains all **6,292** checkpoint21 records.
Production source equals the frozen positive candidate. Root independently
verified all eleven GitHub checks passed on exact pushed head
`7177d103cabe759fe211cc929963345dd93a14b4`; no merge is authorized below the
cycle's **70.002460%** threshold.

### Batch 355: character-config cursor and string source form

`CharConfig_ConfigureAll` restores the original captured character-list cursor,
advancing at every loop increment including skipped records, and the original
path-prefix/directory/filename string-service order. No loader, parser, public
layout or compiler option changes. Known production configuration services do
not replace the published list/count; arbitrary replacement callbacks and
concurrent startup re-entry are outside the equivalence claim.

Its sole unchanged O2 complete-owner gate gains **53.78615816 weighted bytes**,
57.471745% → 60.570026%,1757 → 1781 bytes (original1736). Full symbol/U Counters
retain. The unique original93B initializer remains unscored: complete217B
emitted body,27 relocations, referenced storage and decoded branch destinations
are identical, proving precisely zero delta, not an absolute invented score.
All other unscored physical clones/thunks are likewise retained and audited.

Root independently runs **64 scenarios / 5,866 checks per ABI** using exact
changed loop/string fragments and actual `NuStrCpy`/`NuStrCat`. Host address/
undefined/leak sanitizers and NDK32 diagnostics pass. The excerpts use the
configuration owner's O2 diagnostic context, not claims about each string
helper's separate action. Count0–8, admission skips and fixture-only post-block
continues retain typed cursor identities. Long255-byte paths and NULL sources
are helper/block-only diagnostics, not admission through private CharConfig's
separate128-byte path. No full loader/parser/startup runtime test is claimed.

### Batch 359: cannon prefix admission and chain stop

`UpdateTrooperCannons` restores object-present/absent mutually exclusive AtEnd
checks and stops after the first absent cannon or nonlocal-client admission.
Existing linked-object fallback and nonempty callback preservation retain.
Actual server Reset producers can leave first/interior holes followed by live
records; this is not a fabricated callback-mutation witness.

The sole unchanged O3 full-owner gate gains **387.13707162 weighted bytes**,
54.156864% → 83.026146%,1138 → 1197 bytes (original1341). All22 original-backed
scores are covered, other21 unchanged; full symbol/U Counters retain, with no
owner exact loss. Root independently executes **70 finite cases** using complete
actual updater/Init/Reset/Killed bodies, real AtEnd and real string leaf:
**2,199 host / 2,202 NDK32 checks**. Host address/undefined/leak sanitizers pass;
each helper uses its separate frozen production action in the NDK lane. Both
safe old-body controls fail only the named later-record chain-stop assertion.
Typed lookup/activation/build/visibility services are stable declared seams;
no full particles, animation, assets, network or gameplay validation is claimed.

### Batch 361: particle-cursor marker scratch lifetime

The eight existing cursor markers now reuse one canonical `NUVEC` scratch:
rotate/publish the start, then rotate/publish the end. All colors, submissions,
outer caches and other owner bytes retain. This is an original-backed source
form, not a missing operation or an artificial alignment requirement.

The sole unchanged O2 complete-owner gate gains **1,432.96007394 weighted
bytes**,32.699528% → 54.064670%,7443 → 7283 bytes (original6707). All74 owner
exacts and full symbol/storage/undefined-reference Counters retain. Twenty
unscored original records, including the initializer and destructor aliases,
have full emitted-byte/relocation/referenced-storage/decoded-branch invariance;
their delta is proven zero without inventing an absolute missing score.

Root runs **64 cases / 512 bitwise marker-pair comparisons per ABI** using the
exact changed marker block, actual Y/Z rotations and actual trig-table producer.
Host address/undefined/leak sanitizers and NDK32 diagnostics pass; the helpers
use their own unchanged production actions. Stable line recording compares
initialized position/color/count/order/material/matrix fields, not full editor
or gameplay behavior. Angles span valid u16 inputs and cameras remain finite.
An NDK/glibc diagnostic-only `isfinite` link incompatibility was repaired solely
in the fixture assertion with equivalent exponent-bit classification. The old
host binary and five NDK objects were reused; all old artifacts/failure logs
remain frozen. Actual production blocks, helpers and test inputs did not change.

### Batch 362: canonical lighting scratch-record stride

`computeWarpEffect`'s ten transformed scratch records use canonical `NUVEC4`
storage matching retail's sixteen-byte stride. Actual vector transformation
publishes only XYZ; projection creates a typed XYZ snapshot for `NuVecAdd`.
No fourth-component read, invented padded type, new prefix alias, alignment
attribute, public ABI or compiler option is introduced. The independent input
corner-copy-loop lead is deferred, not a source variant of this measured unit.

The sole unchanged O2 complete-owner gate gains **48.32541525 weighted bytes**,
65.750000% → 66.320885%,8646 → 9206 bytes (original8465). All141 original-backed
records are covered, all27 exacts and complete symbol/U Counters retain. Root
runs **48 finite cases / ten records / 5,664 checks per ABI** using the exact
changed private helpers, actual vector math and canonical reciprocal helper.
Host address/undefined/float-cast/leak sanitizers and production-NDK32 object
diagnostics pass. Bitwise XYZ/bounds identity, a finite scalar oracle and record
guards pass with poisoned W never read. This is not full lighting, renderer,
Android/Bionic or arbitrary projective-input validation.

Two isolated diagnostic authority repairs retain the sole matching objects and
gate: a reviewed doc-only ledger change was pinned against complete old/current
snapshots; then all51 consumed inputs were enumerated and the single actual NDK
fixincludes `stdio.h` explicitly hashed. All eight fixture objects and both
existing binaries were reused for the run-only resume. No matching recompilation,
blanket dependency exemption, source repair or compiler-option retry occurred.
Future packets snapshot the evolving review ledger instead of treating its live
append-only edits as changes to a candidate's code authority.

### Batches 358/363/365: excluded complete units

The complete `EdDrawPolySector` candidate's known target gain is only
0.48042872 weighted bytes; an unscored initializer and changed compiler-label
surface leave whole-owner net **unknown**. It stays unqualified without a
positive-net claim, integration or runtime-fixture expenditure.

The complete collision output-cursor/live accepted-tail reload unit loses
**2,075.78836506 weighted bytes**,78.098410% → 19.160637%,3446 → 3403 bytes
(original3522), in its sole unchanged O3 gate. The first pass/capacity/guards
retain. Full symbol/U Counters retain, with no exact loss; only the target's
score changes. The existing creature-action stream already has the original
parameter cursor and is not another candidate.

The complete `GizmoBlowupBurstDraw` shadow argument correction loses
**75.4481655 weighted bytes**,73.473690% → 69.574560%,1903 → 1895 bytes
(original1935), in its sole unchanged O3 gate. Full symbol/U Counters and exacts
retain. Both negative units remain frozen without variants, production changes
or runtime fixtures. The PanelHint, AveragePos, DROIDEKA, bounding-box, voice
coefficient and alpha-grid bounded censuses yielded no further supported unit;
stack realignment alone is not evidence of an aligned consumer requirement.

### Checkpoint 23 verification

The four integrated units above (355/359/361/362) pass target, native and WASM
builds, all five repository tests and the complete format/forward-declaration/
tidy/symbol/report checks. Fresh mapped and raw linked reports agree at
**68.639720%**, **+0.637260 percentage points** from current main and
**+0.040750 points** from checkpoint22. The complete raw address/symbol exact
set retains all **6,292** records with no losses. After formatting, all four
production owners byte-equal their sole frozen positive candidates. The WASM
link retains the previously observed SceneObjectHelper destructor-signature
warning; its build succeeds, and this batch does not change that owner.
Checkpoint head `e215b86eac9b861093e7af3775ae4fa8582c97c0` has all eleven
GitHub checks successful, independently verified against that exact head.
The macOS target job initially failed before compilation while fetching
`platforms`: the hosted runner could not resolve `github.com`. Only that job
was retried after inspecting its failure log; the retry passes. No code change
or broad workflow rerun was necessary. The cycle remains below **70.002460%**
and is not eligible for merging.

### Batches 366/367/369: economical exclusions

The complete `HatMachineProcess` hint-radius correction (squared threshold
1.0 → 2.25) has **zero measured whole-owner gain** in the sole unchanged O3
comparison. All 26 original scores and seven exacts retain, but the complete
local literal-symbol Counter changes. No production integration, source
variant or runtime fixture follows. An initial proposed sound-position hunk
was retracted during preparation, before either compilation, because the
live source already contained the original position.

`Tag_UpdateTransfers`' six scalar beam-input blocks lose target matching.
The strict initial gate stops on unscored zero-extent thunks; a separate
root-reviewed diagnostic reuses the same two objects and original reports.
It covers all nine emitted functions, proves the two complete 12-byte thunks
have unchanged representation/dependency references, and pairs all six
scored records by unique original raw address and extent. The remaining
unscored initializer is original raw `0xe2220`, 93 bytes. Known weighted delta
is **−156.33633245 bytes**; allowing the initializer its entire possible
**[−93, +93]** delta gives **[−249.33633245, −63.33633245]** whole-owner net.
Even the upper bound is negative. No missing absolute score, initializer
invariance or complete exact-retention claim is fabricated, and no further
compile, matching report, runtime fixture or source variant is needed.

The complete `PooCode` literal/audio/pickup-contract correction produces both
unchanged-action O2 objects and both original reports. The gate stops on an
unscored `MoveToMarker::Render` record; literal storage-label differences
also fail its strict surface check. Its observed target-only improvement is
not a qualified whole-owner gain. Keep the complete unit frozen without
integration or an expensive runtime/invariance proof.

### Compiler-artifact checks versus the fuzzy-matching objective

The user's objective is fuzzy instruction matching, not identical incidental
compiler artifact spelling. A strict complete ELF Counter remains a useful
diagnostic, but a renamed zero-size LOCAL/NOTYPE `.text` compiler label is not
automatically a changed public/private function ABI or data object. Any such
exception requires a specific reviewed identity/storage/reference proof with
all other symbols retained; it must not become a blanket `.L`/`.LC` exemption
or conceal literal payload, initialization, linkage or function-coverage
changes. Keep initial strict results and both sole objects/reports immutable.
Whole-owner scores and unknown coverage still require independent evidence.

### Batches 368/371: screen-grab conversion and hub object cursor

`NuRndrHighResScreenGrab` explicitly promotes its unsigned sixteen-bit width
through the canonical signed integer before converting to float. All 65,536
widths have exactly the same finite value; the original uses zero-extension
and signed `CVTSI2SS`. Its sole unchanged O3 comparison gains **129.59161574
weighted bytes**,57.789074% → 61.904400%,3154 → 3138 bytes (original3149).
All scored owner records and two exacts retain. A separate complete startup,
constant-storage, initialization-array and thunk proof establishes zero delta
for the remaining unscored physical records, without inventing absolute scores.

Root executes both old/new conversions on host sanitizers and production-NDK32
diagnostics: **65,536 widths, 2,051,035 admitted products and 10,760,014 checks
per ABI**. Another439,303 products are excluded before narrowing outside the
declared finite unsigned-conversion domain. Ordered SSE arithmetic and an
independent integer binary32 oracle agree. This tests the changed arithmetic,
not full renderer/GPU behavior or allocation capacity. All thirty actual NDK
fixture dependencies are explicitly qualified. A metadata-only supplement
records four previously unlisted standard headers; existing objects, binaries
and successful runs are reused, with no recompilation or matching-report retry.

`Hub_GoneThroughDoor` traverses the stable object pool with a typed cursor while
retaining the live high-water bound and actual removal services. The sole
unchanged O3 comparison gains **272.2004073 weighted bytes**,42.122643% →
51.878930%,2756 → 2772 bytes (original2790). Complete symbol/U Counters retain;
only the target score changes, with no exact loss. Root executes the actual
changed loop, removal/API lifecycle and torpedo helpers: **260 cases / 136,184
checks per ABI**, both old/new passing host sanitizer/leak and NDK32 diagnostics.
Counts0..64, sparse pools, live shrink, masks/LOS/pads, packet exhaustion and
release/readmission are covered. Four declared empty lifecycle seams assert
their null-state admission; this is not arbitrary gameplay callback validation.
An isolated fixture identifier collision is repaired only in the harness; four
already compiled baseline helper objects are reused. Original failure artifacts
and both metadata repair histories remain frozen.

### Batches 370/372/374: bounded qualifications and exclusions

The frozen `NuLgtLaserDraw` typed-array cursor proposal has a known scored-owner
gain of201.44141238 weighted bytes. Its first strict gate stops on eight renamed
zero-size LOCAL text labels. A specific same-object proof resolves all42 switch
table slots in two byte-identical dynamic-light functions, complete decoded
branches, storage and relocation targets; all other symbols and forty known
exacts retain. An unscored93-byte initializer leaves a conservative whole-owner
interval **[108.44141238, 294.44141238]**, strictly positive. This is a qualified
bound, not an exact whole-owner score. It was not integrated in checkpoint24.
Root subsequently runs both old/new loop-header/admission excerpts with the
actual producer and random helper: **264 finite cases / 53,911 checks per ABI**
pass host address/undefined/float-cast/leak sanitizers and production-NDK32
diagnostics. All consumed headers are qualified before compilation, retaining
the producer's O3 and random helper's O2 actions. Counts0..64, fixed56-byte
record addresses/order, all six continue predicates, material/pause admission,
saved seed/count stores and legal terminal one-past pointers are covered.
Projection/length values are declared typed finite inputs; omitted geometry,
primitive/GPU services and arbitrary callback mutation are not validated.
The full production owner byte-equals the sole frozen candidate after integration.
The laser-only linked verification reaches68.652470%,+0.650010 percentage points
from main, retaining all6,292 raw exact records. Combined checkpoint verification
and its subsequent GitHub checks remain pending.

The complete obstacle nearest-distance sentinel correction has **zero known
gain** across47 scored records and ten retained known exacts. One unscored
1,768-byte proximity function leaves full net unknown; six removed/five added
literal-storage labels also fail the complete Counter. No expensive invariance
proof, runtime fixture, source variant or production integration follows.

The dormant Batman icon red-channel correction gains only **20.29759615 known
weighted bytes**. All six scored physical records are uniquely paired and its
two complete12-byte thunks retain representation. The unscored93-byte
initializer leaves **[−72.70240385, +113.29759615]** whole-owner net, which cannot
qualify positive integration. Keep the sole pair frozen without expensive
initializer/runtime work. No Batman asset or OBB work is involved. The separate
bolt helper and two animation-joint censuses reveal no supported missing unit;
manual loop unrolling or fanout solely for compiler shape is not pursued.

### Checkpoint 24 verification

The two integrated units (368/371) pass target, native and WASM builds, all
five repository tests and complete formatting, forward-declaration, tidy,
symbol and report checks. Fresh raw and mapped linked reports agree at
**68.648230%**, **+0.645770 percentage points** from current main and
**+0.008510 points** from checkpoint23. All **6,292** raw address/symbol exact
records retain with no losses. Both formatted production owners byte-equal
their sole frozen candidates. The known unrelated WASM destructor-signature
warning remains non-fatal. All eleven GitHub checks are successful on exact
head `d54cae25f65b5dc8b2ee5db977a12d10ce9148b2`, independently verified. The cycle remains below
70.002460% and is not eligible for merging.

### Batch 373: Melee idle-branch reserve

The complete idle-state branch correction for `DrawMeleeTargetsNumber` loses
**38.614194 weighted bytes**,52.719826% → 49.362070%,998 → 1046 bytes
(original1150), in its sole unchanged O3 pair. All25 other original-backed
scores, two known exacts and complete symbol/U Counters retain. The initial
gate rejects objdiff's decimal-string address/size fields; a separately frozen
metadata-only repair accepts canonical unsigned decimal strings and verifies
all26 finite scores against unique original raw-address/extent pairs. No
compilation, report or original failed-output rerun occurs. The startup is
actually scored98.235290% on both sides, not assigned a missing score of zero.
One natural hitpoints clone and two zero-size thunks remain explicit unbacked
null-score records. This negative mapped-owner result does not warrant further
clone/initializer proof or a runtime fixture. The complete unit remains frozen
without source variants or production integration.

### Checkpoint 25 preparation: bounded fresh-body review

The two selected world/terrain bodies, `SarlaccPitB_Update` (4,747 original
bytes) and `TerrainImpact` (2,545 bytes), retain their full finite response,
state and service contracts. No missing gameplay block is identified. The
latter's repeated output-count indexing remains a possible canonical
source-form lead under separate review, not an invented count/output alias bug.
`AddDynamicCreature` (2,528 bytes) and `Action_SetRunSpeed` (1,363 bytes)
likewise retain their inspected finite contracts. Apparent run-speed clear and
seek/multiply anomalies are original behavior and are not changed. None of
these census observations is a measured improvement or runtime proof.

### Alias-inclusive uncertainty in the headline metric

The whole-binary report's denominator is the sum of all13,459 function rows:
4,722,419 original bytes. Deduplicating physical address/extent pairs gives
4,667,302 bytes instead. Consequently, physical deduplication is useful for
representation auditing but can understate uncertainty in the measured metric.
An unscored D1/D2 alias pair must retain both measured weights until its delta
is independently qualified; missing scores are not zero. Batch378 illustrates
this: eleven physical unknown extents total383 bytes, but twenty measured alias
rows total644 bytes. The initial known407.6943899-byte gain therefore has
interval[−236.3056101,+1051.6943899], not a positive physical-only bound.
A bounded proof of complete bodies, decoded transfers, canonical outgoing
targets, storage and COMDATs subsequently establishes unchanged representation
for all unknowns (17 functions,24 storage records and two complete thunks).
This qualifies their zero delta without assigning any absolute missing score.
Runtime preparation and production integration are separate pending steps.

The further full-body censuses cover `EmperorFightA_Update`,
`AISysProcessCharacter`, `NewPlayerCharacter`, `AISysCreatureInteraction2D`,
`DrawGameObjectsProcess` and `UpdatePartEmits`. No supported large missing
contract is identified. Their existing bitfield clear, object cursor,
40-emitter indexing and cold service paths are retained. These are bounded
structural observations, not exact-match or runtime-equivalence claims.

### Batch379: TerrainImpact output-tail reserve

Repeated canonical output-count indexing instead of a cached record reference
gains49.00143 known weighted bytes in the sole unchanged O3 comparison;
49.341030% → 51.266430%,2424 → 2480 bytes (original2545). All125 scored
owner rows and the complete symbol/undefined surfaces retain. Actual original
unscored identities include the93-byte `gameliball.cpp` initializer alias,
339-byte natural `TerrainKillPlayer` clone (optimizer suffix differs), and
12/15-byte report-inferred thunks. Their459-byte uncertainty leaves whole net
[−409.99857,+508.00143]. Same-object bytes, direct edges and reciprocal local
100% are diagnostic, not a full outgoing-dependency proof or missing scores
of zero. The small gain remains unqualified; no production integration,
further compiler/report variants, runtime fixture or expensive proof follows.

### Batch377: BuildIt first-occupied short circuit

`GizBuildIt_FindNearest` stops its existing eight-player occupancy search after
the first match. Its sole unchanged O3 pair gains145.4961405 known weighted
bytes, including a small retained-owner collateral improvement; the target
rises54.536170% → 68.272340%,1077 → 1045 bytes (original1055). Complete
symbol/U Counters and ten known exacts retain. The four unscored helper names
cover three physical COMDAT bodies; full bytes, aliases, decoded internal
branches, storage and absence of outgoing references independently prove
unchanged representation, without inventing an absolute score.

Exact old/new occupancy-loop excerpts using canonical GameObject/BuildIt
records pass864 admitted cases and4,320 checks per ABI on host sanitizers
(including leak detection) and NDK32 diagnostics. All non-null player pointers
designate stable valid records; cases cover every first-match slot, null masks,
configured contexts and later matches. Full nearest-search geometry/gameplay
is explicitly outside this bounded source-form test. Both actual header
closures are qualified before builds. A finite23-header metadata supplement
preserves the original stopped preflights. A subsequent host-only real
NuMemoryPS-provider include repairs isolated fixture compilation; the already
successful NDK object is reused unchanged, with no matching variant or
recompilation. The integrated full owner byte-equals its sole frozen candidate.

### Checkpoint25 verification

The two integrated units (370/377) pass target, native and WASM builds, all
five repository tests and complete format, forward-declaration, tidy, symbol
and generated-report checks. Fresh raw and mapped linked reports agree at
**68.655550%**, **+0.653090 percentage points** from current main and
**+0.007320 points** from checkpoint24. All6,292 previous raw address/symbol
exacts retain; `GizBuildIt_TurnOff` at raw0x4cd940 becomes exact, bringing the
total to **6,293**. Both formatted source owners byte-equal their sole frozen
candidates. GitHub checks for this new checkpoint remain pending; the cycle
is below70.002460% and is not eligible for merging. Platform/editor runtime
preparations and further particle/quaternion candidate comparisons are separate
work, not improvements included in this checkpoint.

Checkpoint25's exact pushed head
`4a3815f796198b56480b0ba4768fd508af4d1376` subsequently passes all eleven
GitHub checks. The head identity, count and SUCCESS conclusions are independently
checked; the matching threshold remains unmet.

### Batch376: platform shape-count traversal

The sole unchanged O3 comparison gains135.77285925 known weighted bytes;
`SkinPlatform` rises33.919666% → 42.540165%,1478 → 1430 bytes
(original1575). All seven scored original rows and complete symbol/U Counters
retain. Complete own-section thunk bodies, COMDATs, decoded references and
storage independently qualify unchanged representation for the two unscored
thunks, without assigning absolute scores.

Full old/new `SkinPlatform` bodies with actual terrain, transform, square-root
and trig helpers pass780 cases and617,365 checks per ABI, with matching digest
`9e5a5126`, on host ASan/UBSan/leak detection and NDK32 Linux-libc diagnostics.
The fourteen actual preflight translation units are qualified before any build;
a finite four-header metadata supplement preserves and qualifies the first
stopped preflight. Independent geometry/output-byte checks cover stable,
disjoint canonical streams, signed empty counts, multiple batches, mirrored
triangles/quads and finite threshold cases. Malformed streams, nonfinite input,
overlap, live count mutation and full gameplay are outside the admitted domain.
Mode-zero/two trig initialization is linked, not claimed tested. Production
integration byte-equals the sole frozen owner candidate; linked checkpoint
verification is pending.

### Batches380/381: particle and quaternion reserves

The sole O2 `UpdateParts` typed-cursor comparison has exactly zero known gain
across124 scored originals and retains seven known exacts. Two compiler-storage
names change, so its complete symbol surface is unqualified. The original
93-byte initializer and3351-byte `PartCollide` remain unscored, not zero-weight.
No further proof, runtime fixture, source variant or integration is justified.
An exact checkpoint25 documentation-append metadata qualification precedes the
sole comparison and preserves the original frozen snapshot.

The sole O3 `NuAnimBuffEvaluate_3_QuatB` cursor comparison loses208.18635174
known weighted bytes;79.029600% → 69.484146%,2082 → 2179 bytes
(original2181). All twenty scored originals, one known exact and complete
symbol/U Counters retain; two unscored thunks remain explicitly unknown.
Existing quaternion-slot/root-scale allocation debts are recorded, not
silently repaired or runtime-validated. No variant, runtime or integration
follows this negative comparison.

### Batch378: editor-input runtime qualification

Full frozen old/new Update bodies and fifteen actual helper excerpts pass all
256 finite cases per ABI on host ASan/UBSan/leak detection and NDK32 Linux-libc
diagnostics. Canonical live pad/menu/camera/context records cover digital masks,
pad admission, property release, menu state, analogue cursor reset, repeat-state
transitions and complete ray bytes. Independent state-transition checks agree.
The sole unchanged production O2 gate and complete unknown-representation proof
remain the matching authority; diagnostic helpers all use the owner O2 options,
not each helper's individual production action. No mutated callback, complete
editor/gameplay or Android-device/Bionic execution is claimed.

Both actual preprocessor closures qualify against finite pinned catalogs before
either object build, and build dependencies equal those preflights. A PREP-only
failure-output repair changes `fprintf(stderr,...)` to `printf(...)`, avoiding a
Bionic `__sF` dependency in the Linux-libc witness. Prior fixture, manifest and
scripts are preserved; actual bodies, helpers, cases and action options are
unchanged. The sole runtime passes without retry. Integrated production source
byte-equals the sole frozen owner candidate; linked verification is pending.

### Checkpoint26 linked gate (before commit)

The integrated platform/editor owners pass target, native and WASM builds.
Independent raw and mapped linked reports agree at **68.667076%**,
**+0.664616 percentage points** from main and **+0.011526 points** from
checkpoint25. All **6,293** previous raw address/symbol exact identities retain;
none are gained or lost. Local full checks and pushed-head CI remain separate
pending qualifications. The70.002460% merge threshold is not reached.

The complete pre-commit qualification subsequently passes all five repository
tests, format, canonical forward-declaration, target/native/WASM tidy and symbol
checks, plus generated Pages report refresh. Both formatted owners still
byte-equal their sole frozen candidates; generated68.667076%/6,293 agrees with
the independently checked linked reports. No new pushed-head CI result is
claimed before the next commit.

Checkpoint26's pushed head `8c6689b848d2eaf63521624e07ddbe91943c10c5`
subsequently passes all eleven GitHub checks. Exact head identity, check count
and SUCCESS conclusions are independently verified; the +2-point threshold
remains unmet.

### Batches382/385: excluded comparisons

The sole O2 cutscene stream-skip correction loses56.328159549 known weighted
bytes. `instNuGCutSceneUpdate` falls2.0513513% → 0.4783784%,3130 → 3111 bytes
(original3581). Complete current-stream/version guards and the separately
audited skip-stream contract are retained in the isolated candidate; earlier
initial-state/dispatch debts are not claimed solved. All81 finite original
pairs, two known exacts and complete symbol/U Counters retain. Camera-update
and additional unbacked helper/initializer/thunk scores remain explicitly
unknown. No further proof, runtime, variant or integration follows the loss.

The sole O2 pad-camera staged-quotient comparison raises its target61.830040%
→ 64.739130%, gaining34.5890801 known weighted bytes across37 uniquely scored
original pairs. All three known exacts and complete symbol/U Counters retain.
Two distinct original93-byte LOCAL initializer identities remain unscored:
their alias-inclusive uncertainty bound is186 bytes, not a guessed zero score
or deduplicated93-byte bound. The conditional interval excluding unqualified
unbacked thunks is[-151.4109199,+220.5890801] bytes, so net improvement is not
established. Root reproduces the read-only census from the same two frozen
reports/objects; no additional compiler/report, expensive representation proof,
runtime or integration is performed. The older gate's first-unknown stop is
preserved; future packets use complete unknown inventories.

### Batches384/386: full-body censuses

Fresh complete reference exports close the previously partial
`AIScriptLoadAllPakFile` and `Hub_UpdateMiniKits` audits. Canonical source,
full original bodies and actual producer/helper evidence reveal no large
missing contract or new grounded matching proposal. AI derivation/inherited
parameter expansion is compiler inlining evidence, not permission for manual
fanout. The minikit audit separately reserves a narrow language-correctness
issue: a finite negative combined angle is truncated to signed i32 and then
wrapped to u16 in retail, while direct float-to-u16 is out of range. This is
a symbolic finite witness, not a measured matching gain or runtime validation.
No candidate, compiler comparison, fixture or production change is prepared
for either census; malformed input/allocator policy debts remain unchanged.

### Batch387: display-scene clone registration

The sole unchanged O2 whole-owner gate gains45.03835926 original-weighted bytes;
`NuDisplaySceneClone` rises58.605656% → 60.732113%,2226 → 2229 bytes
(original2118). All23 original-backed rows are finitely and uniquely scored;
three known exacts and complete symbol/U Counters retain. The three unbacked
helpers are separately qualified from these same objects: complete body bytes,
decoded internal branches, resolved CALL/GOT relocations, storage and thunk
COMDAT prove unchanged representation. Their absolute scores/original weights
remain unassigned, not zero. No new comparison report or compilation is used
for this bounded proof.

Exact frozen old/new final-registration fragments with canonical scene,
sort-priority and manager records pass480 cases per mode/ABI. Host
ASan/UBSan/leak detection passes72,640 checks; NDK32 Linux-libc diagnostics pass
72,644 checks (including four target-layout assertions). All four outputs share
digest`b87ff7b1`. Independent signed-key/tie ordering, full links/counts,
unrelated fields, canaries and legal one-past formation checks cover stable
nonnegative bounded arrays, existing sorted lists, planes-2..2 and equal keys.
All four actual preprocessor dependency closures qualify before any object
build; build closures equal the preflights. This source-form fixture does not
test complete clone allocation, rendering, platform services, pthread locking
or concurrency. Production owner byte-equals the sole frozen candidate;
linked checkpoint verification remains pending.

### Batch389: area-list census

The fresh full2441-byte `Areas_ConfigureList` reference and canonical parser,
startup/storage/helper audit establish no missing valid-data contract or
grounded source-form change. The twelve-target screen distinguishes this
complete census from other partial or historical entries; `Area_Configure` is
a different previously closed function. No candidate, compiler comparison or
runtime fixture follows. Existing malformed-number and unused buffer-end
policies are not silently repaired or universally validated.

### Batch383: turret reset contracts

The sole unchanged O3 whole-owner comparison gains120.885335999 weighted bytes;
`GizTurrets_Reset` rises47.169174% → 59.567670%,995 → 1047 bytes
(original975). All37 finitely scored original pairs, six exacts and complete
symbol/U Counters retain. Both unbacked zero-size thunk symbols are qualified
from their actual12-byte section contents, complete decoded bodies, storage,
COMDAT and absence of outgoing references; their absolute scores and original
weights are not invented.

Retail-backed corrections initialize the complete signed platform id for all
roles, guard actual animation services against a null set, seed the average
fallback before the real helper, and use primary times inverse-secondary for
the relative transform. Full frozen old/new Reset bodies and twelve actual
helper bodies pass592 candidate cases and10,724 checks per ABI;576 safe old
controls expose576 platform,96 average and216 transform defects. Independent
matrix and complete record/payload/progress/visibility checks cover platform
indices through257, missing and filtered handles, rigid quarter rotations,
unequal translations, all progress bit planes and the63/64 index boundary.
Separate old-null child processes reproduce the actual helper fault; the
candidate null guard passes. Host ASan/UBSan/leak detection and actual-owner-O3
NDK32 Linux-libc diagnostics both pass, with preprocessor closures qualified
before compilation. Diagnostic helpers use the owner O3 options, not each
helper's own production action. Fixed-room portal and unreachable animated
evaluation seams do not claim complete portal, animation, gameplay or Bionic
validation. Integrated production source byte-equals the frozen candidate;
linked checkpoint verification remains pending.

### Batch388: collision-position fallback reserve

The sole unchanged O3 owner comparison substitutes canonical collision-position
coordinates in the character-model fallback and gains5.2166092 known weighted
bytes. Its target rises73.53684% → 73.69925%,3045 → 3061 bytes
(original3212);61 known exacts and complete symbol/U Counters retain. The
original-backed initializer and additional unbacked helpers/thunks remain
explicitly unscored, so net owner improvement is not established. No additional
comparison, expensive closure proof, runtime fixture or production integration
follows. The existing unordered-aware radius condition remains unchanged;
misleading decompiler syntax is not used to change its NaN behavior.

### Checkpoint27 linked gate (before commit)

Integrated turret-reset and display-clone owners pass target, native and WASM
builds. Independent raw and mapped linked reports agree at **68.670600%**,
**+0.668140 percentage points** from main and **+0.003524 points** from
checkpoint26. All **6,293** previous raw address/symbol exact identities retain,
with none gained or lost. Main remains`a496c28beece66a24ccdf7336571b6836f45e6aa`
after fetch and is already an ancestor. Full local checks and final pushed-head
CI are separate qualifications; the70.002460% merge threshold remains unmet.

Full local pre-commit qualification subsequently passes all five repository
tests, formatting, canonical forward declarations, target/native/WASM tidy,
symbol checks and generated matching refresh. Both formatted production owners
still byte-equal their frozen candidates. Pushed-head CI is not inferred from
these local successes.

### Batch390: debris predicate comparison excluded

The sole unchanged O3 whole-owner comparison loses222.7832232 known weighted
bytes. `DebrisFindAllOfType` falls45.814816% → 21.386831%,979 → 995 bytes
(original912). All103 finite original-backed rows, both exacts and complete
symbol/U Counters retain; three unbacked compiler rows remain separately
unscored. The retail-backed effect-before-activity predicate does not establish
a positive matching unit. No variants, additional reports, runtime fixture or
production integration follows. The finite history screen remains immutable
and distinguishes full target closure from source-only neighboring screens.

### Batch391: staged menu captures excluded

The sole unchanged O2 complete editor-owner comparison loses114.83465067 known
weighted bytes. `cbPtlStartVelMenu` falls49.674770% → 42.583588%,1317 → 1261
bytes (original1539), with two additional finite collateral changes included.
All31 known exacts retain across568 finite original pairs. Two original-backed
gradient callbacks remain unscored, totaling2171 alias-inclusive original bytes;
eight additional unbacked compiler/helper names also remain unknown. Complete
undefined Counters retain, but eleven local compiler-label/string entries are
removed and eleven added. The strict qualification stop preserves all unknown
rows and storage evidence; no generic-label exemption, further comparison,
representation proof, runtime or production integration follows. This is a
negative known subtotal and failed qualification, not an invented absolute
score for the unknown functions.

Checkpoint27's pushed head`6b6df8a2dbc6e3bd46a11803dbeee78137c27a7d`
subsequently passes all eleven GitHub checks. Exact head identity, check count
and completed SUCCESS conclusions are independently verified. The +2-point
merge threshold remains unmet; this success does not qualify a later head.

### Post-checkpoint27 world/status census

The frozen twelve-name history screen promotes only`CalculateWorldSize` to a
fresh complete raw/current/helper-layout audit. Its legacy min/max and display
center/extent bounds formats, extrema selection and six canonical output
components are already represented; no grounded missing contract or candidate
is promoted. Eleven other names retain their explicit historical dispositions,
including`UpdatePodRaceMines`, whose prior typed-storage collateral work is not
a full-body closure. No compiler comparison, runtime fixture or production
change follows this census. Parent review reads the complete audit record,
not an independent reproduction of every child helper/raw read.

### Batches392/393: cutscene prerequisites and dispatch reserve

The fixed twelve-name animation/cutscene census reveals a genuinely missing
trigger-name binding tail in`NuGCutSceneFixUp`. Its52-byte definition-name rows
cannot be conflated with the independently proven four-byte runtime trigger
state records. Retail loader calls pass a null definition owner; no canonical
name-table producer is resolved. The missing tail remains a typed-owner
prerequisite reserve, without a guessed pointer signature or raw overlay.

The separate sole O2 rigid-render candidate restores the existing typed
collision callback after optional post-render and before locator traversal.
It gains28.3007709 known weighted bytes; target35.056180% → 39.230335%,
698 → 730 bytes (original678). All81 finite original pairs and both known
exacts retain. Complete Counters record only the explicitly reviewed addition
of one undefined`NuCutSceneRigidCollisionCheck` reference, backed by actual
GLOBAL4-byte storage, GOT resolution and canonical setter/typedef; there is no
generic service/label exemption. The original2100-byte camera-update row and
six unbacked helper/compiler rows remain unknown. The conditional original-row
lower bound is−2071.6992291 bytes before unbacked-representation qualification,
so whole-owner improvement is not established. No further comparison,
expensive closure proof, runtime or production integration follows. Missing
absolute scores and deltas are not assigned zero.

### Post-checkpoint27 object and renderer censuses

Two separately frozen twelve-name screens promote only `GameFog_Update`
(1877 original bytes, finite64.705260%) and `DebrisSetup2` (2413 original
bytes, finite64.560760%) to complete child raw/current/helper-layout audits.
The fog state transitions, finite interpolation and publication contracts
are already present. Its unordered timer difference and existing signed
colour-shift debt are explicitly outside the admitted finite census, not
silently qualified as runtime-safe. Debris setup's allocation sequence,
deliberate partial scale clear, material flags, effect packing and rotation
also retain their actual contracts; optimized scalar fanout does not justify
manual unrolling. Neither census promotes a candidate, export, comparison,
runtime fixture or production edit. Each other eleven-name set remains a
metadata/history screen, not a claimed complete closure. Parent review reads
both complete audit records without claiming to reproduce every child raw
or helper read.

### Batch395: capacity-backed minikit construction excluded

The sole unchanged O3 complete-owner comparison loses308.1114741 known
weighted bytes. `SpecialMiniKits_Configure` falls46.715908% → 17.287878%,
1122 → 1143 emitted bytes (original1047). All33 original-backed rows have
unique finite scores, all eleven known exacts retain and complete Counter/U
surfaces retain. Canonical current world endpoints, reset/rearrangement and
the preceding traffic loader establish the capacity authority, but rejected
records still need the local fallback at a short tail. That safeguard is not
removed to chase the retail shape. The negative unit closes without variants,
further comparisons, runtime fixtures or production integration; unknown
unbacked compiler rows are not assigned absolute scores.

### Batch396: grass device-tier correction remains unqualified

The complete retail455-byte `LoadGrassFile` body reads the existing signed
device tier <=2 after its unconditional SpeederChase filter. The cached
mid-range flag covers only tier1; canonical actual renderer/device producers
also admit tier2. The sole unchanged O3 candidate restores that predicate and
filter order, retaining the current missing-singleton fallback and the exact
loader arguments. Its sole complete-owner comparison gains20.98300295 known
weighted bytes: target51.106796% → 55.718445%,449 → 459 emitted bytes.
All125 original-backed rows have unique finite scores and all four known
exacts retain. The canonical existing declaration adds exactly one consumed
header, separately verified against each side's actual dependency closure.

Complete Counters nevertheless remove four local compiler labels and add
four others plus the independently reviewed singleton reference. The sole
named reference exception does not permit these other surface differences.
Six unbacked initializer/helper/thunk rows remain explicitly unknown; no
positive-size exact-name original row is missing from the finite subtotal.
The unit closes as unqualified without further comparisons, variants,
representation-proof expenditure, runtime fixtures or production integration.
Neither unknown absolute scores nor unknown deltas are assigned zero.

### Batch397: serializer source-form trial excluded

The sole unchanged O2 full-owner comparison loses67.26136292 known weighted
bytes. `EdClass::Serialise` falls52.110767% → 46.292310%,890 → 891 emitted
bytes (original1156). All232 original-backed rows have unique finite scores,
all41 known exacts retain and full Counter/U surfaces retain. The original
reloads stream mode after writing, but actual stream helpers preserve it;
the independent second mode test is only a stable-domain source-form trial,
not a claimed callback/gameplay fix. The negative unit closes without
variants, further comparisons, runtime fixtures or production integration.
Unbacked compiler representations do not acquire fabricated absolute scores.

### Batch399: memory-context diagnostic remains unqualified

The sole unchanged O2 comparison restores the original non-debug leak report,
post-pop context-name read and modulo-byte ASCII predicates. Its known owner
subtotal gains139.31649985 weighted bytes: `PopContext`54.180927% →
62.305622%,1513 → 1684 emitted bytes (original1727), with a small collateral
`DumpBlocksForContext`27.954683% → 27.879154% decrease included. All78
original-backed rows have unique finite scores and all24 known exacts retain;
the full undefined-reference multiset retains. Complete Counters nevertheless
remove ten and add eleven literal-label/storage identities. The original
diagnostic literal does not imply a blanket compiler-label exemption. The
unit closes unqualified without variants, further comparisons, runtime or
production integration. The two unbacked thunks remain explicitly unknown;
their zero symbol extents are not scored or treated as zero absolute weight.

### Batch398: decoder temporary-name restoration excluded

The sole unchanged O3 complete-owner comparison loses111.701739624 known
weighted bytes. `CreateDecoder` falls21.798851% → 6.0804596%,260 → 904 emitted
bytes (original716); a small `Initialise` increase is included in the owner
subtotal. All110 original-backed rows have unique finite scores and all26
isolated known exacts retain. The canonical empty-string reference is backed
by full raw use, GLOBAL4-byte storage, GOT bytes and relocation, with exactly
one named U-addition exception; this does not waive the other20 removed and
23 added Counter identities. Six emitted helper/initializer/thunk rows remain
explicitly unbacked and unknown. No runtime, variant, report retry or
production integration follows the negative comparison. The approved safety
guards for allocation failure and names beyond the retail u16-capacity domain
are not removed to chase matching.

### Batch400: fresh script-processor census closes without a candidate

A separately frozen twelve-name metadata/history screen promotes only
`AIScriptProcess` (raw0x3e5060,1883 original bytes, finite60.904053%) to a
complete child raw/current/canonical-helper audit. Reference-stack conditions,
action reset/evaluation/live advancement, script/state reference transitions,
ordered interrupt expiry and keep-blocked publication are already present.
No omitted contract or specifically supported natural source-form unit is
established. No export, candidate, comparison, runtime or production change
follows. The other eleven names retain their individual metadata/history
dispositions; two unresolved full closures are not falsely marked faithful.
Parent review reads the complete audit without claiming to reproduce every
child raw/helper read or validate the full script engine at runtime.

### Batch401: spline neighbor correction remains unqualified

The sole unchanged O3 whole-owner trial restores the original independent
nearest-distance MIN reduction and first-index neighbor priority. The actual
loader admits two allocated point records; at length2/index0 the original
selects next1, whereas the previous independent ternaries select next0.
No gameplay use of such an area spline or invalid-length safety is inferred.
All existing guards and the actual LineIntersectXY helper remain unchanged.

The known subtotal gains23.62008688 original-weighted bytes:
`OutSideSplineArea`20.318897% → 22.877953%,825 → 841 emitted bytes
(original923). All15 original-backed rows have unique finite scores; there
are no known exacts to lose, and full Counter/U surfaces retain. Three
unbacked rows nevertheless include the398-byte private SplinePointAngles
clone and two zero-extent thunks. Their unknown absolute scores/deltas are
not zero; invariant extents alone are not complete representation proofs.
The small unit closes unqualified without additional comparisons, variants,
expensive closure proofs, runtime fixtures or production integration.

### Batch394: character shadow cutoff and continuation restored

The complete899-byte retail `CharShadows_Update` uses0.075f, backed by the
raw0x57681c literal bytes9a99993d and the full raw joint-height comparison.
Restore this cutoff, captured object-count/accepted-tail refresh and the
stored u16 joint-mask continuation. The latter two are source forms on the
admitted stable table, not claimed callback mutation bugs. Keep all existing
guards, five-shadow/16-joint bounds, radius99 policy and nonfinite policy.

The sole actual O3 whole-owner pair gains83.58153133 original-weighted
bytes: Update44.188680% → 53.485847%,860 → 864 emitted bytes. All four
original-backed rows have unique finite exact raw-VA/size/name/binding
pairings, no known exact losses, no collateral changes and full Counter/U
retention. Root independently inspects both unbacked thunks' complete12-byte
executable COMDAT sections, bytes, storage, symbol identities and full
relocations, including unchanged eh_frame references. These specific
representations are invariant; zero symbol extents are not zero storage or
invented absolute matching scores.

Root builds and runs full actual old/current Update bodies with actual
GameShadow, NewShadowEx/NewScanRot/NewCast, PlatOnOff, terrain geometry and
angle/math helpers. All24 exact extracts, two untouched full data/allocator
owners, actual context/layer tables and14 individual production actions are
pinned. All32 host/NDK preprocessing closures qualify before object builds;
each actual compile dependency map equals its preflight map. Current and
old legacy-control each pass316 cases/225,315 checks per ABI. Host ASan,
UBSan and leak checks pass; NDK32 uses unchanged owner options and Linux libc
for this bounded diagnostic, not a Bionic gameplay validation.

The real ordinary plane0.0875 over jointY0 and extended floor−2/material3
produces the canonical rejection layer. Current passes; old fails exactly
the one documented0.075 cutoff assertion with status1 and no sanitizer
diagnostic. Default-count cases separately require five real casts per
admitted object, preventing a zero-work fixture from falsely qualifying.
Counts−3/0/1/2/16/64, masks/POI holes, guards, radius boundaries, real signed
contexts, threshold neighbors, layers, platform disable/restore, hover masks,
nonzero slope angles and96 seeded cases also check whole64-object stores,
canaries, terrain/cache state and declared timing service order. Active
rotating/skinned platforms, renderer/GPU, arbitrary callback mutation and
full gameplay remain outside this finite claim.

Preserved first diagnostic failures exposed a missing canonical globals
header, host GAMECHARACTERDATA/config-view pointer expansion and Bionic
stderr FILE linkage. Fresh diagnostic-only packets repair these wrappers
and canonical fixture inputs; candidate/helper/oracle bodies and matching
reports are not varied or retried. The final runtime packet is
`/tmp/saga-charshadows-runtime394-configfix.kYMXEN`, freeze
faed2c72de918348d8b2ad295823c91a343fd0074686708498d5ddaabe1369f0.
Two failed packets are recoverably archived in ignored.cache with original
path links and291 frozen evidence hashes revalidated after a /tmp user quota
failure. No evidence or user files are deleted. Production source byte-equals
the sole frozen candidate; linked checkpoint verification follows separately.

### Checkpoint28: linked shadow-cutoff verification

Fresh raw and mapped whole-binary reports agree at68.672380% fuzzy matching,
+0.001780 percentage points versus checkpoint27 and+0.669920 versus the
current68.002460% main baseline. All6,293 exact raw address/symbol records
retain; there are no new exacts. The original name/alias-inclusive denominator
remains4,722,419 bytes and every raw key/extent is retained. Production
charshadows.cpp byte-equals the sole frozen batch394 candidate. Target,
native and WASM builds and all five repository tests pass. Final normal
hook and pushed-head CI results are recorded separately; the70.002460%
cycle merge threshold is not reached.

### Batch402: parent-condition source form closed unqualified

Root reviews the complete original/current `ProcessFlowBox`, canonical parent
loader and actual output/activation services, then executes the sole frozen
whole-owner pair in `/tmp/saga-gizmo402-census.8vAZ5e/gate402`. Manifest SHA256
990512c69212cdb0abb8938f4f951072f826928e0e507c5fc18f2572556ca447 pins
93 source inputs,47 external/generated inputs and the unchanged actual O3
action. Both actual consumed dependency closures qualify. The sole change
expresses short-circuit parent success with a bool rather than comparing the
post-break index with the parent count; this is a stable loaded-topology
source form, not an invented callback-mutation gameplay bug. Retail's
output_indices[0], private ABI and all other body bytes remain untouched.

The known subtotal gains137.4764415 original-weighted bytes. LOCAL target
raw0x4b1bb0/original925 bytes rises8.402174% → 23.264492%,946 → 930
emitted bytes. All57 original-backed rows pair uniquely with finite scores;
both known exacts and every undefined identity retain. The strict Counter
removes.L687/.L688 and adds.L691/.L692, all zero-size LOCAL/NOTYPE `.text`
labels. These are not silently exempted; the two unbacked thunks also lack a
new independently reviewed representation proof. The modest unit closes
unqualified without extra proof, runtime, variants, compiler/report retries
or production integration. Sole root execution finishes with status1 and
preserves both objects/reports and the initial strict result.

### Checkpoint27 service/world censuses: two faithful targets

Two fixed twelve-entry censuses close without proposed changes. The service
packet `/tmp/saga-service-frontier12.ufPbBL/census.md` has SHA256
991c4b97d23edebfcc90ab8d8e1bdb292adf452c603f37df00aec7a52d51cd2f.
Its sole complete child audit is LOCAL `fnAudioSample`,raw0x348f70/2807
bytes/78.39917%. All eighteen keyword branches, enabled/disabled filename
handling, signed bucket/index chains, reference counts, persistent filename
policy, clamps and terminal68-byte copy are already present, backed by
canonical parser/string/CRC services and producer/consumer records. The other
eleven entries are current-source/history screens, not original equivalence
proofs or measured reserves.

The world/AI packet `/tmp/saga-rowtowards-cp27-census.y6C2IY/audit.md` has
SHA256992aa92ed7bce67074daf07ddb87a518877f0c6e0542bd2ac43018e208d1d56a.
Its sole complete child audit is LOCAL `RowMoveTowards`,raw0x3d7c00/1851
bytes/76.75903%. Route inheritance/admission, turn geometry, endpoint
reversal, asymmetric leader eligibility and straight movement are already
present, with canonical16-member/four-row admission and actual math/speed
services. Retail stack realignment alone does not justify an alignment
attribute. The other eleven entries remain metadata/history-only. Reviewed
and later archival ledger hashes are distinguished in the packet. Root reads
both complete audits but does not claim to have independently repeated their
raw/helper closures. Neither census runs an export, compiler, report or
runtime, and neither changes production.

### Checkpoint28: exact pushed-head CI verified

All eleven PR121 checks complete SUCCESS on exact pushed head
98d2b8dc3df180c80c22334c0ae69b8b8447717b, independently verified using
the PR head and all check conclusions. The normal commit hook passes and
preserves the human author plus verified Codex co-author trailer. The
68.672380% checkpoint remains below70.002460%; no merge is attempted.

### Checkpoint28 additional faithful audio/path censuses

Root reads both complete child audits; this is not an independent repetition
of their full raw/helper proof or a runtime-equivalence claim. No candidate,
export, compile, matching report, runtime or production integration follows.

`CalculatePositionalMix`,GLOBAL raw0x328cc0/5626 bytes/85.52264%, closes
without substantial admitted-finite omission. All five modes, listener
admission/detachment/attachment, screen transform, coefficient/MAX blend,
gain/LFE and metadata publication are present. The surprising second-listener
outer angle intentionally uses the first angle in retail too. Nonfinite
admission differences and tiny stable-endpoint source forms are reserved,
not promoted. Packet `/tmp/saga-positionalmix-cp28.9EGdAO/audit.md` has SHA256
f0dd991ef155e9619eba149b3d7f013e4cf9952ec74e3c823a32140c4a4ab79a.

Plain-C `WithinConnection`,raw0x3eee20/2179 bytes/68.46311%, has its complete
route/checksum, radius erosion, both actual dynamic-node refreshes, circle/
capsule/endcap/interpolated-height geometry, shared intersection polynomial
and path-info publication. Canonical u16 route masks ordinarily select0..15
or sentinel0xff. Inherited route64..254 shifts remain outside the defined
C++ domain; scratch Y remains uninitialized before the actual rotation copies
it, as in the original scratch setup. Thus this is an algorithmic census,
not a fully defined C++/sanitizer-equivalence claim. No tiny UB-only matching
correction is proposed. Packet `/tmp/saga-withinconnection-cp28.s7qkZU/audit.md`
has SHA25664daff513ecf92e54b231e50589a9ed339c9bea6350d7621d5c75d16cec5a7d1.

### Checkpoint28 additional finite faithful censuses

Four further child audits close without candidates, exports, compiler/report
trials, runtime fixtures or production changes. Root reads their complete
audits, not independently repeats all raw/helper proof. Deficit rankings are
opportunity indicators, never measured gains. Other screened names remain
metadata/source-only unless their own prior full closure is cited.

- `Bolts_Draw`,raw0x480320/1389 bytes/54.619050%: all32 slots, persistent
  scale scratch, actual scale callback, primary/glow/reflection/shadow draws,
  unordered sentinel tests and paused/end gates are present. Retail inlined
  rotation math is not grounds for manual helper fanout. Audit
  `/tmp/saga-boltsdraw-cp28.SVAZ6i/audit.md`,SHA256
  db155cf0ebad84dc0107711a22f1d2b1b8fdfb4d35217251b999d739647dcf90.
- `PlatSkinEndReigster`,raw0x38eee0/1406 bytes/52.36011%: reverse scratch,
  conditional bounds,49 cells,257 count clears, secondary platform-link
  quirk and cache metadata are present. Initialized allocated arenas, valid
  scene indices and defined finite conversions are required; no active
  gameplay callsite is established. Initial prose literal-address errors
  are preserved and explicitly corrected by the final freeze. Audit
  `/tmp/saga-platskin-cp28-census.oRYKvx/audit.md`,SHA256
  2af617a61fb119af20b81700bdc8136a8a16f62411c2b3543eec7b2b354ad18e.
- `areaEditor_Process`,raw0x3e0470/3322 bytes/54.480520%: options, hover,
  select/create/move/scale, delete, rotation, height and list cycling are
  present. Actual camera service explains the preserved initial-selection
  skip. The wider-host shared-storage concern stays separately deferred.
  Audit `/tmp/saga-menu-area-cp28.2hVA0x/audit.md`,SHA256
  4cfbca11b8067e1ff9ffa1b90a2c94045ed46c26242051d0997ebb66c7fd6d68;
  its append-only addendum clarifies497 owner lines, not nominal500.
- `DrawTouchPrompt`,raw0x458b80/1052 bytes/34.245%: icon/text branches,
  controller/timer pulse, real unused remainder service, canonical literals
  and helper submissions are present. The known unordered-wave discrepancy
  is outside the finite timer/conversion domain; no NaN-only trial follows.
  Audit `/tmp/saga-render-cp28-touchprompt.W2jYnK/audit.md`,SHA256
  9e5dfc74d65f023f8c9358af3d1611df0b4cb124e6e24fa2e51391aa7403ac81.

### Checkpoint28 final bounded faithful censuses

Root reads the child audits; these are static closures, not measured gains
or root repetitions of all binary/helper evidence. No candidates, compiler
trials, exports, runtime fixtures or production changes result.

- `Move_CHARACTER`,raw0x16f500/2399 bytes/70.559260%: input/context,
  weapon/jetpack/gravity, rocket event, landing, grab, victim and modifier
  tails are present. Actual producer/helper effects explain the required
  existing reloads. Audit `/tmp/saga-move-character-closure.f9inZP/audit.md`,
  SHA2567ac878bc6dc441064218850f3914fcabb25ffc4d05d4bf456e901423bf1c3dba.
- `Collection_GetIDList`,raw0x4dd480/1076 bytes/53.317610%: flag mask,
  all nonzero ownership returns (including2), optional first/second outputs,
  current list/count accesses and final sentinel are present. Loaded valid
  IDs and adequate outputs are required; malformed indices or metadata
  aliasing are not supported by this audit. Audit
  `/tmp/saga-collectionid-cp28.hvdgC8/audit.md`,SHA256
  0f2ec4a81ccea2c0ab4f6fd8edbc242c812d86d09ff8c9d56cc3ae894b232965.
- `NuDynamicLight::setupCustomCameraFrustum`,raw0x2b96b0/1280 bytes/
  53.353897%: cache setup, split copy, per-set frustum/shadow/matrix/capsule,
  split planes and camera restoration are present. Original constructor
  independently confirms two RenderSets; admitted count1..3 cannot reach
  retail's oversized SIMD paths. No invented array capacity/alignment or
  copy fanout follows. Audit `/tmp/saga-customfrustum-cp28.zFc1zC/audit.md`,
  SHA25649e39d3d3e581e851efcfc79c8b486bf6d67de81b6f61833c81177b1acd97a64.

### Batches403/404: single gates and protobuf-default diagnosis

Both frozen candidates receive exactly one complete production-option
baseline/candidate compile pair and two bulk original/object reports. Full
dependency/action/tool freezes and literal ELF Counter/U retention pass.
Neither initial gate authorizes integration or runtime; no variants or
report retries follow. A separate protocol/representation review is below.

- Batch403 restores the separate local `Loader` in `RequestDecode` through
  semaphore signalling. The known subtotal improves294.8269404 weighted
  original bytes; target29.50186→57.5539%,396→892 emitted bytes, original1051.
  Seven known exacts retain. The historical gate rejects an omitted score
  for original-backed501-byte `RequestBuffer`; bx/cx thunks also remain
  unbacked, with no independent representation qualification. Capture
  `/tmp/saga-requestdecode403.KkQkcV/gate-capture.json`,SHA256
  dc58eb16d4b06fa1708ebbe9a1d7760e567f779b5419f7c5468bf4475135348f.
- Batch404 changes only three selected signed-size guard expressions in
  `AndroidOBBUtils::LookupPackagePath`; no assets/package data are touched.
  Target1658→1283 emitted bytes, original1139. Baseline report score8.006803%
  is explicit, candidate field is omitted. The original strict gate closes
  unqualified, with three finite collateral rows unchanged. Capture
  `/tmp/saga-obb-guard-sourceform.V0QZzB/gate404/gate-capture.json`,SHA256
  039bc36d8e6e28cb4af046b5597a2d185a3445d6a2adf7192254f06a2021e10c.

Root then independently diagnoses the protocol, without changing/re-running
these immutable gates. Installed objdiff-cli3.8.1 SHA256
aadcbc10c958934220cfbf8f6008fb0303b7c3c4fea087e2f657b5e690837eb3
equals the local release executable in `/home/fabian/git/objdiff`.
The local schema `objdiff-core/protos/report.proto` makes this a non-optional
proto3 float. The generated release serializer omits0 and decoder restores0;
`objdiff-cli/src/cmd/report.rs` also records0 for an absent internal score.
Both captured reports declare version2. Thus an omitted field on a validated
existing row is a known *report metric* zero, not necessarily a successfully
computed instruction comparison. This does not permit scoring absent rows,
unbacked helpers, invalid numbers or unresolved pairings as zero.

The production report mapper already decodes this protobuf default. Future
experimental gates must follow the same verified schema rule after exact
row identity validation (see07-diagnostics), instead of falsely stopping on
every zero-valued score. Historical packet outputs remain unchanged. This
correction does not qualify403's unbacked helpers or manufacture a404 gain;
404's decoded target metric actually declines from8.006803% to0.

Root subsequently qualifies403's specific unbacked bx/cx representations
independently: each complete12-byte code section (including eight NOP bytes),
flags/alignment, COMDAT membership/signature, absence of code relocations,
entire referenced CIE/FDE and its exact R_386_PC32 target/addend retain.
Only the relative CIE-position field is normalized after prior extent drift;
no code, instruction operands or absolute scores are fabricated. The separate
read-only `/tmp/saga-requestdecode403-protocol-proof.py` checks frozen input
hashes, both report versions, the exact501-byte/raw0x31fa90 protocol-zero row,
all37 original-backed owner functions, seven retained exacts and full Counter/U.
It passes without compiler/report execution or modifying the historical gate.
The metric delta remains+294.8269404 weighted original bytes. This specific
post-gate protocol correction permits bounded real-service runtime preparation
for the unchanged403 candidate; it is not a blanket unknown-score exemption.

Two further child faithful closures produce no candidates or experiments:
`Bolt_Debris_LSW`,raw0x1be610/1107 bytes/54.855373%, retains all point/debris,
live double-damage, special shake/rumble and explosive-cheat/arcade tails;
its dogfight indexed-tail storage debt is not certified host-safe. Audit
`/tmp/saga-boltdebris-cp28.isa6CJ/audit.md` is read completely by root.
`NuSoundSystem::CreateEffect`,raw0x31bda0/1180 bytes/39.44856%, retains all
eight allocations, actual constructors and initialise/list-registration tail.
Redundant remaining-delay initialization is not a missing behavior; valid
scratch/list and sufficient list-node allocation remain required. Audit
`/tmp/saga-createeffect-cp28.UKbmPd/audit.md`,SHA256
23318d25bc9fbff0f8bb9da049c98554af5e6f8ebd6fda176f91faa45749ceb2,
is read completely by root. Neither audit claims recursive engine/runtime
equivalence or identifies an original omission to implement.

`NuSoundStreamer::RequestFill`,raw0x3262b0/1115 bytes/58.676365%, also
closes after its first complete raw audit: local QueueElement registration,
queue copy, atomic publication, Signal and post-signal local unlink are
already present. Full actual producer/worker/fill-service records support
that bounded static result, not a host64 or concurrency claim. Root reads
the complete audit `/tmp/saga-requestfill-cp28.5yQLT7/audit.md`,SHA256
00e7b1b9aff5a217e659baea8264e4f6336a368706eff3249585f397df1190f3.
No candidate, new report, compiler or runtime follows this census.

The separate403 protocol/representation script has SHA256
78cd599aa6cdb6b2a55182716fd231c154ce570ebdd765039af019c4755fbf3b;
its retained result `/tmp/saga-requestdecode403-protocol-proof-result.json`
has SHA256bf408f359da01eee6bdf1987fc5cfecfc22e928274b640d2da5aa6b0b3e4bbe0.

### Checkpoint29 CI and batches405/406 measured gates

All eleven GitHub checks complete successfully on exact pushed checkpoint29
head `ea76697111708f144c0ebf47c6daae3947078745`. Matching remains68.672380%,
+0.669920 percentage points from current main,6293 raw exacts; the +2-point
cycle threshold is not reached.

Batch405 moves only the existing `NuSoundVoice` eight-gain memset after
the existing SetState call, matching retail construction order. One unchanged
O3 owner pair and two reports pass strict name/alias-inclusive qualification:
C1/C2 each22.638159→80.6579%,736→733 emitted bytes,749 original bytes,
net+869.13572018 weighted original bytes. All91 original-backed owner rows
are accounted for,50 exacts retain and full ELF Counter/U retains. The two
unbacked thunks keep unknown absolute scores; their representation review
and bounded constructor verification are separate prerequisites. This is a
natural source-form improvement, not a gameplay bug claim. Capture
`/tmp/saga-voiceconstructor-cp28.9Tk9JL/gate-capture.json`,SHA256
e5740624ed6c257d233fc38cb79959ad75a45b47ff89ac8c20614d8e433f3f93.
Before execution, root corrects a protocol snapshot filename collision;
original manifest/audit/snapshots retain and the candidate remains identical.
Pure parser controls pass three valid cases and nineteen rejected cases.

Batch406 removes only the unsigned narrowing of canonical signed camera_index
in the lookahead comparison. The unchanged O2 gate accounts for all82
original-backed owner rows, retains two exacts and full Counter/U. Target
0→0.57738096%,2305 emitted bytes both sides,2100 original bytes;
net+12.12500016 weighted original bytes. The old immutable camera census is
not rewritten. Ordinary nonnegative camera indices give identical branch
results for all byte schedule values; no authored -1/255 witness or whole
camera/geometry runtime claim is invented. Capture
`/tmp/saga-cutcam-sourceform406.jolf7b/gate406/gate-capture.json`,SHA256
881fc735216abe463ce0e76981ba9d6df367933015a9510e164866d342e8a931.
Neither isolated result is a linked gain or authorizes integration yet.

Batch403's first genuine host-i386 ASan/UBSan diagnostic builds actual old/new
decoder owners and canonical weak/source/buffer/semaphore services after all
nine -E/-MD preflights pass. The first current matrix returns failed checks
only for an overly strong whole-slot byte-stability oracle, with empty
sanitizer stderr: actual weak-list insertion/unlink legitimately changes
previously queued nodes' prev/next links. No production defect or sanitizer
success is inferred from this failed run. Original outputs are preserved;
an additive fixture-only correction must check payload stability and real
list integrity, not forbid legitimate link updates. No production source,
matching candidate, compiler options or captured matching report changes.
Initial stdout `/tmp/saga-requestdecode-runtime403.fKl0EI/runs/current-matrix.stdout`,
SHA2568bcd75ca8ec112e23bc926ea6069cbb1dc312641a37f4ae7d4a1b766b1fa414d.

The additive403 oracle-only repair subsequently passes. It compiles just
two revised fixture objects and reuses all seven initial actual owner/helper
objects byte-exactly; no matching/compiler-owner retry occurs. Both revised
fixture preflight closures retain the same canonical/system headers. Current
and old matrices each cover61 cases/2545 Signal observations (618880 and
617607 checks respectively); current witness passes163 checks, old witness
fails exactly the single expected local-Loader-at-Signal check. All four
stderr captures are empty under ASan/UBSan with leak detection. Ordinary
callback destruction clears registered pointers through actual canonical
weak services. The result
`/tmp/saga-requestdecode-runtime403-oracle-r2.Wbo8OR/runs/result.json`,SHA256
fd7b3dd8ca8bf5c18c7748574b08ade554c607a176739b5bd21af0e4a25ce8f6,
qualifies the narrow sequential lifetime/queue change, not scheduler,
concurrency, codec/device/audio output or Android Bionic execution.

Batch406's proportional pure integer-promotion proof also passes all32768
admitted camera0..127/nextbyte0..255 pairs (128 equal,32640 unequal), using
verbatim changed expressions and pinned actual canonical i8/u8 declarations.
This is algebraic predicate verification, not C++ runtime/full-camera testing.
Result `/tmp/saga-cutcam-sourceform406.jolf7b/predicate406/root-result.json`,
SHA25693479a451ae0bf9ebf21ca3d76b11ea3ad23fe6ab47adf442a6b188d5b5a916d.

Batch405's separate complete thunk-representation proof passes: both actual
12-byte bx/cx sections, zero-sized GLOBAL/HIDDEN symbols, COMDAT membership,
code relocations and associated CIE/FDE representation retain. Only the
relative CIE pointer is normalized; unknown absolute scores stay unknown.
The genuine GNU-i386 constructor diagnostic subsequently passes all six
preflight closures and builds both versions. Its first current execution
produces empty stdout/stderr and fails the expected-output gate. The wrapper
did not retain the process return code, so no sanitizer success, constructor
defect or startup failure is inferred. Original files/builds are preserved;
an additive same-binary diagnostic must capture the missing process status.

Batch407's sole O3 whole-owner pair improves Doors_Check (raw0x4876b0,
1152 bytes) from61.96063 to79.8189%,1116 to1134 emitted bytes;
known net+205.7272704 weighted original bytes. All33 original owner rows,
five exacts and undefined identities retain. The strict gate remains closed
on one added zero-sized LOCAL/NOTYPE .LC7 in .rodata.cst4. A separate complete
literal-storage/use proof is required; there is no blanket label exemption,
source variant, compiler retry or extra report.

Batch408's sole unchanged O3 whole-owner gate passes: PodRaceCUpdate
(raw0x1fe890,869 bytes)33.189575 to85.175354%,744 to885 emitted bytes,
net+451.75641951 weighted original bytes. All73 original-backed owner rows,
nine exacts and complete Counter/U retain. The complete source correction
restores zero-fade admission, pulse comparison, canonical CUTINFO.instance
finished bit, same-frame state progression, all ten boost slots and completion.
Actual loader failure can publish a null instance; a separately documented
null-instance guard preserves safety outside the valid retail domain. Root
also verifies the original GOT slot0x6124f0 has R_386_RELATIVE addend0x6a0f00,
the actual LevHSpecial base. Bounded behavioral verification is still pending;
isolated owner gains are not linked-binary progress or integration claims.

The additive405 status capture records SIGSYS, with empty outputs and all
frozen artifacts unchanged. Root then obtains approval to execute the exact
same two binaries, environment and oracle outside the syscall-restricted
sandbox, changing only the exclusive capture directory. Both OLD and CURRENT
pass14096 cases/1705616 checks, return0 and have empty sanitizer stderr;
ASan/UBSan and leak detection remain enabled. This supports bounded sequential
final-state/source/weak-reference equivalence, not Android audio, factory/device
execution, concurrency or intermediate-state publication. Initial failed
captures remain immutable. Current/old status SHA256 are respectively
657e944f4e41774f16059ff276f05df403d24817132ee2fe6857017ca04bbf0d
and70b7ee385b55bb428b5e6d18d41505c55dd33ec048782af5489684387dc0daff.
Root integrates the exact frozen403 and405 source changes after their narrow
verification; their combined linked gain has not yet been measured.

The strict low-score motion screen finds no fresh unit among14 original
functions at least1500 bytes and at most30%: each has a named prior complete
audit, integrated closure or measured reserve. Three adjacent movement owners
also have prior coverage. Root reads the full history-only census
`/tmp/saga-motion-cp28-history-screen.Ki9ISd/audit.md`; this is eligibility
exhaustion, not a fresh semantic-equivalence proof or reason to replay trials.
Subsequent selection widens the score band while retaining original-backed
natural source forms as eligible; no gameplay bug is required for a matching
improvement. Rejected source variants and artificial compiler-shape changes
remain excluded. Root also integrates the exact406 signed comparison after
its finite promotion proof; full linked measurement is still pending.

### Checkpoint30 linked audio/camera unit

The exact403 decoder lifetime,405 constructor clear order and406 signed
comparison changes pass target/native/WASM builds and all five repository
tests. One fresh whole-binary report and live Bazel ownership mapping measure
68.697290%,+0.024910 percentage points over checkpoint28 and+0.694830 over
current main. All6293 prior raw name/address/size exact identities and the
complete original denominator retain; all452 mapped owners and assignment
counts are unchanged. Raw report SHA256
d2910eb322199cb86eb3f5af0e3c6523aa85bfe34ebf0a469fb7374dd48a9484;
mapped report SHA256
97fded975eb54b82752736ba760146069473beddafdb8562f5ff398e661b9208.
407/408 remain separately pending, not part of this linked checkpoint. The
70.002460% cycle threshold remains unmet; no merge is authorized by this gain.

Checkpoint30 commits and pushes as db8b08c815faa04d8cada0e37ffffa4434ef4d06,
retaining Fabian's authorship and the verified Codex co-author trailer. The
full normal hook passes; generated matching.json is byte-identical to the
independent checkpoint30 mapping. All eleven GitHub checks complete SUCCESS
on this independently verified exact pushed head; the cycle threshold remains
unmet.

Batch407's additive scoped literal proof passes without changing its failed
first gate, objects or reports: the only symbol delta is one zero-sized local
.LC7 backed by a complete4-byte positive-zero cell. The entire old16-byte
scalar pool and all existing cells/uses retain at+4; vector and string pools
are byte-identical. Exactly two R_386_GOTOFF references are scalar UCOMISS
zero comparisons inside Doors_Check, replacing register-zero comparisons.
This qualifies this literal only, not a blanket compiler-label exception or
an invented original score. Result SHA256
ae89d5e3c6376e150f2b857ba34b3372c986a98cc8a064541d400d4f4ad2bce2.
The separate pure control-flow proof passes2304 finite single-door cases and
233 bounded record arrays, checking unchanged admission, short circuit,
terminal outcomes, exact helper/math expressions and cursor/count traversal
under the actual nonterminal helpers' immutable-list/count contract. It is
not compiled gamebody, numeric/nonfinite geometry, threading or gameplay
verification. Result SHA256
9da12a11bb76297c6087a9f904cb1ada642daa2329dfb68ce9b584fe645ac1de.
Root integrates the exact frozen407 candidate for a subsequent checkpoint;
the measured205.7272704 owner bytes are not yet a linked gain.

The fresh full NuDynamicLight::computeShadowClippingPlanes census
(raw0x2b8890,2734 bytes,26.31962%) finds all six face tests/publications,
both actual edge tables, twelve-edge silhouette traversal and plane-count
bound already present. Root reads its complete audit
`/tmp/saga-shadowclip-cp28.nsZD5c/audit.md`,SHA256
7515d81e52886866551d038c5218d11bf4c9793c6309f47ad26e19e55ae1cffb.
No export, candidate, compiler/report or runtime follows. The widened37-row
gizmo/props/action history screen is only named-history triage, not a blanket
equivalence conclusion. The proposed UpdatePushBlocks captured-base cursor
does not restore retail's base reload and is withdrawn before any trial.

Batch409 prepares only the AIRetreatFromDestination ordered range-clamp
predicates parameter>1 and parameter<0, preserving every other body byte.
Root reads the full audit/current body and independently checks the three
original branch blocks. The actual owner is O3; raw0x3f72e0/3422 bytes and
mapped42.38976% are not measured candidate gains. Normal new_retreat defaults
to1 and bypasses this legacy body. Finite endpoint/order behavior is unchanged;
raw unordered retention is disclosed without a valid-NaN gameplay claim.
No compile/report has run yet. Audit SHA256
30b4c886d2c6f1c03bb4a3c6701c46f63d9d31f733ee0c67b1576672b74bd8ca;
patch SHA256386bf6c5575eee0fd2c854d554b71aa489c31994e1d7dc8a705a981900686ba9.

Root stages409 from the actual unchanged O3 action and90 identical canonical
inputs per side, then runs the sole complete owner pair. All16 original rows,
complete symbol Counter and undefined identities qualify, but the candidate
loses44.92901212 original-weighted bytes:42.352776 to41.03983%, both3195
emitted bytes. No exacts are lost (none in this owner). The mapped linked
baseline42.38976% is not substituted for the isolated owner baseline. The
negative trial is reserved without source integration, variants, compiler/report
retry or runtime-fixture work. Initial objects and gate capture are preserved
under `/tmp/saga-retreat-clamp409.qNCZRP`.

The first408 diagnostic stops at host64 baseline compilation, before any
runtime: canonical numemory.h requires NuMemoryPS declarations missing from
the extracted fixture's include order. Its build log and failed outer capture
remain immutable. An additive packaging-only repair must use the actual
canonical header, not invented declarations or altered candidate/oracle code.

Root fully reviews and executes408's additive canonical-header wrapper,
preserving the initial packet/failure capture. Both host64 and GNU i386 SSE
candidate diagnostics pass all14 named cases and1165 checks with ASan/UBSan
and leak detection enabled. On each ABI, the five safe old-body controls fail
exactly their named fade/pulse/slot/completion assertions; paused, absent
pacemaker and absent-cut controls pass. Original state0 present-cut controls
are excluded because their scene-as-instance access exceeds the canonical
scene extent; no padding fabricates validity. Actual cutscene lookup, NuFmod,
special setters and pacemaker-display body execute, while mandatory update
calls and message allocation failure are typed diagnostic seams. This is
bounded state/helper validation, not NDK/Bionic, rendering, concurrency or
full gameplay validation. Results SHA256
da38c28e481e0be613c8b5c2698d841cb3821d1ee95f98ef942ebb5023fc0ede.
Root integrates the exact frozen408 candidate; its isolated owner gain is
not yet a linked checkpoint gain.

Batch410 proposes only replacing edppLoadPage's duplicated particle-budget
block with the actual existing same-owner static UpdateTotalPtls(debtab[index])
call. Retail0x36c801 directly calls raw0x34d810 after table publication; the
current static helper already emits its natural compiler-private ABI without
attributes. Source ownership, O2 action, guards, helper and counters stay
unchanged. Root rejects the initial gate's stale explicit-score-only parser
before compiler/report execution and requests an additive qualified-v2
decoder repair. No source variant or measured gain is claimed.

### Checkpoint31 linked door/podrace unit

The qualified407 door traversal and408 podrace changes, with only canonical
formatting after frozen-candidate integration, measure68.711250% in one fresh
linked raw report and live Bazel mapping: +0.013960 percentage points versus
checkpoint30 and+0.708790 versus current main. All6293 prior raw
name/address/size exact identities, the4,722,419-byte original denominator
and all452 mapped owners/assignment counts retain. Raw SHA256
f37417128deb7bfc44adf7c8e6e91ffea75452f44f9bc16589357b02fff5a3aa;
mapped SHA256
09ac9e59353eb3ea13a5987b7ad3266f39bdde8cfef471e668bc79c2f69aa228.
Target/native/WebAssembly builds and the five repository tests pass. The
normal commit hook will independently regenerate the report. Batch410 and
411 are not part of this checkpoint. The70.002460% cycle threshold remains
unmet; no merge follows from these gains.

Checkpoint31's full normal format/tidy/symbol/report hook passes and its
generated matching.json is byte-identical to the independent mapping. Root
commits/pushes9f0c8a3c1c47b62413a4d9c57a96ba0573834a5d with Fabian's
authorship and the verified Codex trailer, independently verifies the PR
head identity and starts final-head CI monitoring. Current main remains
a496c28beece66a24ccdf7336571b6836f45e6aa; no rebase is needed.

Batch410's corrected sole O2 owner pair scores all570 original-backed rows
and retains31 exacts: net+181.70018942 original-weighted bytes, including the
neighbor callback gain and graph loss. edppLoadPage improves31.90372 to40.34792%,
1917 to1678 emitted bytes. The strict capture initially stops only on27
removed/27 added zero-extent LOCAL NOTYPE text labels. Root fully reviews and
executes the additive same-object proof: all134 jump-table slots resolve to
the same function/instruction endpoints, all nine complete containing bodies
and their decoded control/relocation closures are byte-identical, and the
entire560-byte rodata table/constant pool retains. Every changed label has
only internal jump-table references. All other Counter/U identities retain.
This qualifies those exact54 labels, not a blanket compiler-symbol exception
or an invented score/owner for the eight unbacked emitted names. Frozen
objects/reports and initial failed capture remain untouched. Lean changed-block
diagnostics are pending; no410 production integration is claimed.
Scoped proof result SHA256
475166511949a16752763b62852397570c072029c3363b935979ba37ec0fa4cb.

Batch411's fresh full NuSoundSystem destructor audit identifies the missing
embedded routing-table name retirement before owning-list release. The
minimal proposal adds only a nonvirtual ordinary inline destructor clearing
name[0], capacity, length and pointer, with a null-name safety guard. Existing
canonical data layout, matrix ownership, all other implicit member cleanup
and allocator contracts remain unchanged. Root fully reads the audit/proposal
and actual routing constructors/GetName/NuEList deletion, then approves PREP.
The actual target dependency census finds28 header consumers among483 compile
actions, so a positive primary-owner trial alone will not authorize integration.
No411 compilation/report/runtime or shared-header production edit has run.

Root subsequently stages411's exact frozen header-only System pair and runs
the sole unchanged O3 gate. All110 original-backed rows and26 exacts qualify,
including natural D0/D1/D2 variants, but the known alias-inclusive metric
loses3.86532006 original-weighted bytes. D1 and D2 each change51.071663 to
50.90228%,1157 to1173 emitted bytes; D0 is unchanged. Six removed/six added
Counter identities and six unbacked emitted names remain explicit, unresolved
qualification stops; neither their scores nor deltas are invented. The known
negative already closes this proposal. No representation-proof expenditure,
27-owner collateral pairs, runtime, variants or integration follows.
Manifest SHA256959499b6f783412d8c5c6149f1284824bc37233ef97ff9c91b962d4023860f1e;
capture SHA2564c85c2844eb0a9c80b26ebf5e3813c47917c612569bda333e61807a62d024fd9.

Checkpoint31 subsequently passes all eleven GitHub checks on the independently
verified exact pushed head9f0c8a3c1c47b62413a4d9c57a96ba0573834a5d.
Final-head capture SHA256
06bb4d7e937e43a2decd6687b91d97ee289338257f30eaaa87bac396074e057c.
The merge threshold remains unmet; no squash or new-cycle PR is authorized
by this checkpoint's matching result.

Root fully reviews and executes410's lean exact-removed-block versus existing
helper diagnostic. GNU64 and GNU i386 SSE each pass260 finite cases, respectively
969212 and965056 checks, with genuine ASan/UBSan/float-cast sanitizing and
leak detection enabled. Independent phase/count and stable typed DebReAlloc
recorders verify all512 slots, repeated matching keys, publication/order,
sentinels and untouched canonical storage. Trail count0..3 gives multipliers
1..4. Canonical numemory.h enters the actual Android memory header once under
explicit ANDROID; complete consumed GNU/canonical input closures are pinned
before object compilation. This is changed-block validation, not a full
loader/file/allocator/locking/GPU/concurrency/NDK/Bionic gameplay claim.
Runtime result SHA256
e326cd287475d1ff52f6db0dc8818b0e26993d315859d5be8d02085106876fbd.
Root integrates only the exact frozen410 proposal. The measured owner gain
still requires fresh linked checkpoint verification before being counted.

### Checkpoint32 linked editor helper unit

One fresh linked raw report and independent live Bazel mapping measure
68.715110%,+0.003860 percentage points versus checkpoint31 and+0.712650
versus current main. All6293 prior raw exact name/address/size identities,
the4,722,419-byte original denominator and complete ownership counts retain.
The only production change is410's existing particle-budget helper call.
Target/native/WebAssembly builds and all five repository tests pass; the
normal commit hook will independently regenerate its report. Future OGG,
terrain and editor-save proposals are not integrated or counted here.
Raw SHA25673521bcf404e29fcb69025c9ace92333bd3f442d6aa82f00168c332d176524bd;
mapped SHA2564c17fa6f621601c23a6c1c4a4af2648db4d8399b2073c21fdff42fd346708674.
The70.002460% cycle threshold is not reached.

Checkpoint32's full normal format/tidy/symbol/report hook passes and its
generated matching.json is byte-identical to the independent mapping. Root
commits and pushes8a40b80f5e67cad3cee9f87f63581e91b10d8847 with Fabian's
authorship and verified Codex attribution. Final-head PR checks are being
monitored; no pending checks or threshold status are represented as a merge.

All eleven checks subsequently pass on independently verified exact head
8a40b80f5e67cad3cee9f87f63581e91b10d8847. Final-head capture SHA256
1df8d2b27d5ffa3f4c5ff32b08690d7b3a31b09ec89a28f2f24716c7f0636510.
The +2 percentage-point merge threshold is still unmet.

### Batches412–414 source-backed reserve qualification

412 freezes only DecodeOggChunk's output-only bitstream variable lifetime and
six-channel stores as the original direct three-element rotation. Actual
ov_read_filter/wrapper source never reads or retains that output pointer.
The unchanged O2 full-owner pair gains8.5771194 original-weighted bytes across
30 original-backed rows, retaining ten exacts and the complete Counter/U
surface. The target changes56.00901 to57.193695%; emitted size639 to657 bytes.
Gate manifest SHA25694dfa92a0c025a5e990f43e7790e7f09a48cf3c403f5ac1ef75819ec20e267d9;
capture SHA25657c89302e9e47c5258a614440f92325b390dfc91fb041d6d85de33c59bb51529.
The two explicitly unbacked PC thunks receive no invented original score,
extent or weight. Root's same-object proof verifies their actual12-byte code,
COMDAT membership, normalized unwind and complete direct-call closure.
Proof SHA256deb875a98c2eac548d58a355848fdb9a7393e0a2f64fdacb366a5e95dd52dbf8.

Root fully reviews and executes the lean exact-old/new PCM excerpts using
canonical integer types, independent channel-order oracle, real aligned
arrays and sentinels. GNU64 and genuine GNUi386 each pass397312 cases and
1191936 checks with ASan/UBSan and leak detection enabled, empty runtime
stderr and ELF32 sanitizer linkage verified. The393216 coordinate-basis
cases cover every16-bit value in each coordinate, plus4096 bounded
multiframe cases; neither all2^96 frames nor every offset/pattern combination
is claimed. This is PCM fragment validation, not codec/bitstream execution,
decoder lifecycle, concurrent store observation or NDK/Bionic gameplay.
Host result SHA2563fb7111c4bc3f38b2dd628e1f045ed7bb207b78f8295250f07590aaf438e9aba;
i386 result SHA2561a1b62c0aae565fc063a69b38670b10c2f43f5220dcfe844250699d6746e6971.
Root integrates only the byte-identical frozen412 candidate; linked gain is
not counted until the next independent checkpoint.

413 freezes only the first64-slot terrain aging loop's canonical captured
cursor/count form, still after CurTerr's null guard. Signed positive-only
decrements, all remaining terrain logic, canonical24-byte record and actual
O3 owner options remain unchanged. The sole pair gains10.4366936
original-weighted bytes across103 backed rows, retaining two exacts and the
complete Counter/U surface. Target59.88802 to60.533855%, same1528-byte size.
Manifest SHA256cb7a6c0fd442ec39fda73ff82e613bcf04af65d2860afa83422a32b00b58e937;
capture SHA25647c79b84fd789ececef589a888608ef01df95bf62ff626efb1f0a7dc52b37804.
Root separately qualifies the exact unbacked93-byte startup and both12-byte
PC thunks: whole code, outgoing REL targets/addends, actual constant storage,
BSS layout, init-array publication, normalized unwind and incoming calls
retain. Proof SHA25680ccd82df4020ccfc02bce66f9fca7befbba49017b46f3787036b4e7b182934c.
No original metric is invented for them. Root fully reads and executes the
lean exact-old/new guarded-loop diagnostic using genuine TERRSET storage and
canonical records, with no terrain-engine scaffolding. GNU64 and genuine
GNUi386-SSE each pass131456 cases,1314566 checks and262914 loop calls under
ASan/UBSan/leak detection, empty stderr. Both signed fields exhaust all65536
i16 values with all-null/all-nonnull IDs;384 mixed-ID boundary cases verify
every slot, whole records, guards and untouched storage. Actual159-header
inventories are reviewed as observed GNU provenance, not pre-frozen NDK
authority. ELF32 and sanitizer linkage retain. No full terrain remainder,
physics/GPU/concurrency or gameplay-bug claim is made; both source forms pass.
Host result SHA256f7fece21ed27ecd17c78878ea89419fae17717b83b6bb5d9a02e742c5686fbf9;
i386 result SHA256b0c6dbe1d64a96257573103372b8f4391080e3021e8eff1aacabc5e2980f526e.
Root integrates only the byte-identical frozen413 candidate; linked gain
remains pending the next independent checkpoint.

414's sole unchanged-O2 editor-save pair gains510.085188 original-weighted
bytes across232 backed rows, retaining41 exacts and the complete Counter/U
surface. Save changes53.309677 to67.47871%,3689 to3651 emitted bytes.
Only the existing ten-scene macro captures the canonical buffer after
PreSave/name preparation and editable admission; all invocations and existing
function attributes remain byte-identical. No new optimization authority is
introduced. Manifest SHA256ac36d1d148a51334e2a043cfb82f5ea94f335b6875ea351571d56feb31e2a3a0;
capture SHA256ea2e7ab73d21e363fde23790b4daaf1826704375df56bf4684e3bc1dfacc573b.
Both explicit unbacked PC thunks pass the same scoped representation proof,
SHA2567fed42dba4bec78789c9c4f84111ae983d06b436de1211d526a7498a62c3288e.
Root fully reviews the complete unchanged old/new Save, actual canonical
lifetimes and32 helper definitions, explicit stable two-method ClassEditor,
WriteStream and device seams, and independent ten-scene output/order oracle.
The first metadata extraction's missing KHR membership is repaired using
individually named prior actual377/378 dependency catalogs. Root also catches
and corrects the diagnostic-only first block-size oracle to23 before any
preprocessing/build; prior artifacts remain immutable. All four finite
dependency preflights qualify before any fixture builds (162 GNU/115 NDK
files), and all four objects compile. The first link then stops on the
omitted real EditorSettings constructor, with no runtime yet executed.

Additive r2 extracts that exact canonical constructor as the33rd lifetime
helper, qualifies both new closures before its two builds and reuses all
four already-frozen fixture objects without recompilation. The full Save,
32 prior helpers, scene inputs, oracle, options and failed-run history retain.
GNU64 ASan/UBSan/LSan and actual-action NDK32 Linux-libc old/candidate each
pass240 cases and25877 checks, identical trace91ac7b51 and empty stderr.
Real memory-file helpers and stream headers run; registered serialization,
disk/device services, DAT/card/APK alternatives and full Android/Bionic
gameplay are not claimed. Helper excerpts use diagnostic owner O2, not
each helper's production optimization. No artificial pointer-mutation or
expected baseline-failure witness is introduced. ELF classes and host
sanitizer linkage verify. Root integrates only the byte-identical frozen414
candidate; linked gain remains pending the next checkpoint.
r1 manifest SHA256f29e82fc465c9fecfe60aacb874fee80c5ee504da59f7596048f72603ccaba26;
r2 manifest SHA256ecd34114318f943f5fbb9b48a1a36a80a41093064d56fb620abe3bc966577470;
runtime result SHA2563bbf23937a6e524e932a3d22fb80054af696ea2ad1d338579880dfd5a25947b5.

The fresh cp32 animation/cutscene history screen finds no new proposal:
all16 mapped >=1500-byte/<=60% rows lead to already complete audits, measured
reserves or integrated units. No repeated raw exports, compiler or runtime
expenditure follows. The unique existing v2 DrawCharacter original row has
6588-byte extent and omitted scalar, which decodes to report-metric0 under
the established protocol; it is not an absent-row unknown. This correction
does not qualify its unresolved private-ABI/codegen comparison or reopen it.

A separate new ForceThrowCode audit finds a raw-backed fifth GizForce_Throw
literal1 versus source0, but both actual helpers ignore that argument. The
original LOCAL .isra.13 and emitted .isra.15 have no exact-name pairing;
unchanged exact-name callers alone do not establish the changed private
body's score or delta. Root reads the complete audit and preserves the sole
literal proposal at /tmp/saga-force-throw-cp32.651qfy without compiler/report,
runtime, production changes or invented clone mapping. It remains reserved
pending independent comparison/identity evidence, not a measured losing
trial or claimed gameplay fix. Audit SHA25679df8d2a8da7011cc804754dbcc105ce61649ea828f9c2c96f5fa368eb95b1fb;
patch SHA25623307de28fbb4e6eec825cd1d590599439d60377647cf0c1d8e0443054e4c52a.

### Batch415 matching-neutral additive world-loading correction

A fresh complete raw/export/current/type/producer audit identifies the actual
WorldInfo_Load omission: optional vehicle+bonus CharacterMiniKits_Load must
be followed by LoadTerrainFile, LoadGrassFile and LoadBridgeFile unless a
service requests abort. Raw12b1be jumps back to12ad8b, with calls at12ad8e,
12ada0 and12adb1 and each existing abort check. The actual minikit helper
does not perform those environment loads; real loaders initialize/publish
terrain and page handles even when assets are absent. Canonical area keywords
admit flags5 and level defaults admit the nonexcluded0x4e0 path. No deployed
asset usage, deep parser execution or native64 loader safety is claimed.

Root fully reviews the sole remove-else proposal and actual unchanged O3
owner/dependency gate. The two original reports are byte-identical: all22
original-backed rows, zero existing owner exacts and full Counter/U surface
retain with no qualification errors; known net0. WorldInfo_Load remains
79.80697% and3193 emitted bytes in the object comparison. Explicit unbacked
startup/thunks remain unresolved and receive no fabricated metric. The known
neutral already closes this matching trial without representation proofs,
runtime fixtures, source variants or integration. Its concrete behavior
correction is preserved separately, not mislabeled as already fixed.
Manifest SHA2561284aec4fdbc7f23ee1246a970d5e708280ab6f85afb3ad632abeaa9c0475c9f;
patch SHA256fa1a84327c3dea6c9a5c41ccc65116a64686457866703aa843c6ceea701620af;
capture SHA2566afb588248e763565e4cfd6744e23d1230726e7aecc9d4e0535fdc30de67f5bf;
both report SHA256249b39eb9a7a0f05b1c76ac8cd511f3c39e3232737f2d5ed7b9ecb43b17b9c5d.
Initial metadata proof-path and subsequent documentation call-address repairs
are preserved before the sole compiler gate; neither changes the candidate.

### Checkpoint33 linked Ogg, terrain and editor-save unit

One fresh raw linked report and independent live Bazel mapping measure
68.726395%,+0.011285 percentage points versus checkpoint32 and+0.723935
versus current main. All6293 prior raw exact name/address/size identities,
the4,722,419-byte original denominator and complete ownership counts retain.
Only the frozen412/413/414 candidates are integrated; neutral415 and the
unpaired ForceThrow literal remain reserved. Target/native/WebAssembly
builds and all five repository tests pass. The normal commit hook will
independently regenerate its report before commit/push. The70.002460%
cycle threshold is not reached; no merge or replacement PR follows yet.
Raw SHA256dee4b2500c1b74f2d6d02cb28f16bf1ef3d5128c3b96d2d853094e2c049c77fa;
mapped SHA25621380097802225bcbabc768432890e49a7da8d8bfdb9962d44c8eae4e421ffa1.

Checkpoint33's full normal format/tidy/symbol/report hook passes; its generated
matching.json is byte-identical to the independent mapping. Root commits and
pushes946ccdc2a9be6cd768fbc66a0b1b2058688a7bb2 with Fabian's authorship
and verified Codex attribution. Final-head checks are being monitored;
neither pending CI nor the unmet threshold is represented as a merge.

All eleven GitHub checks subsequently complete successfully on that exact
946ccdc2 head. Independent final-head capture SHA256
42a184a2fad9a7fd54ec3cd678fe3c91d5648e9007234d40ea4bf42ab97039e7
qualifies the identity and all COMPLETED/SUCCESS conclusions. Main remains
a496c28beece66a24ccdf7336571b6836f45e6aa after a fresh fetch; the unmet
70.002460% cycle threshold still prevents a merge.

Bounded history-first cp33 screens exclude all twelve highest-weight item/
object opportunities and the already complete TerrainPlayer census plus five
negative source forms. Its current body is byte-identical to the prior owner
snapshots. A single genuinely new full raw/current/helper rendering census
closes blur7x7Separate without a supported omission; work-texture creation is
a retail no-op, so initialized rendering inputs or gameplay are not certified.
No repeat compiler, report or runtime expense follows these closures.

### Batches416/417/419 bounded rejected trials

Root reviews each complete original body, canonical types/helpers and one
frozen full-owner production-action pair before measurement. Bolt_Find's
original million-unit nearest-distance sentinel replaces0.49, but both full
reports are identical:63 backed rows/five exacts, full Counter/U and known
net0. Batch416 closes without representation proofs, runtime or integration.
Its capture SHA256e055aa79e27a52232e56cabda788649522b8a98460ac03d7b42f6181dd7f4a5f.
The actual torpedo callers supply no position; no gameplay correction is claimed.

Batch417 restores the locator callback's original two guarded insertion
operations. The sole pair adds one112-byte generated LOCAL helper, so the
strict gate stops. An additive read-only capture of those existing objects
and reports qualifies30 backed rows and+148.82818422 weighted bytes, but the
83-byte AddLocatorToSet wrapper falls100→99.46429%. Exact retention closes
the candidate immediately: no clone proof, runtime, variants or integration.
Unbacked helper/startup/thunk metrics remain unknown. Additive capture SHA256
242384fdcdcb100cf33155a41974dd25acd5ff5885fc243a18cb48cbc0bfe2f2;
capture script1590ef6c63c9c9ce0b68de00681c5075b89095f674def31955d736a496e0fe11.

Batch419 restores the two original short-circuit slot5 fall-animation loads
instead of an eager cached boolean. The unchanged O2 owner pair measures
−48.28124002 weighted bytes; only Animate_BATTLEDROID changes37.802920→
28.744526%,534→581 emitted bytes. All79 backed rows/nine exacts and full
Counter/U retain. It closes negative without thunk proof, runtime, variants
or integration. Capture SHA2564f91d07019dc9c408aa07dd034c37aa3b68f7632274c65e6b96cfcae79216e13.
Original caller registers/stack disprove Ghidra's MoveAnimManage argument
cast; no private ABI attributes or fake mutating helper witness are added.

### Batches420/421 captured registration and rendering state

The complete602-byte RegisterGestureTracker raw body snapshots slots0..8
before admission and insertion. One ordinary nine-element bool snapshot loop
replaces the source's repeated vacancy reads, retaining all ten typed records,
signed priority ordering, slot9 rejection and the existing pair-copy loop.
The sole unchanged O3 owner pair gains49.71354528 weighted bytes across28
backed rows, preserving nine exacts and complete Counter/U. Target19.025806→
27.283870%,524→586 emitted bytes. Independently reviewed PC-thunk code,
COMDAT, unwind and complete direct-call closure retain; their original absolute
scores remain unknown. Manifest SHA2569f398c6d3de9777bcb6d300eeef3738c961c15b5b1c9ccef2d75ff49a9a47df4;
capture18e05fd51479bfbb4b88391674e31f023252da16c82e510e5fbc55b7bc7f7d72;
thunk proofbbdf7f9518a6c14f1b3800c58300f49508123da120c2a763c29a4b3d57307fc2.

Root runs the exact old/new full member bodies with canonical class lifetime,
real constructors/destructors and ten-record/canary/other-state checks. Each
GNU O3 host64/i386 SSE ASan/UBSan/LSan binary passes31,744 occupancy/priority
cases and984,115 checks; all12 preprocessing closures are approved before
compilation and actual build dependencies retain. This is stable-table
equivalence, not an invented callback/concurrency/gameplay witness. The only
unrelated Update vtable dependency is an uncalled fail-fast diagnostic seam.
Runtime capture SHA256bcaf2f593baa94f04c7839b87a23ca28c99c3c4f86d1f9c191066d443d8e6b7e.

Batch421 captures the existing TexQuadSubmit3D helper's typed VARIPTR cell
once and advances that same cell, as all four original vertex publications
do. No wrapper expansion, new helper, ABI/options/ownership change occurs.
The sole O2 pair gains21.34508688 weighted bytes;48 backed rows/five exacts
and complete Counter/U retain. Target39.217392→42.422360%,620→620 bytes.
Its two unbacked PC-thunks receive the same explicit representation/edge
qualification, never invented original scores. Manifest SHA256
e147b0bfc6d650b580bedc8a1cb4dd832f3e1fe57ca9251710ca7204d0247618;
capturef054529c570a67833fe1651ca895367b465e09b8c9ece5be3a892eb37a30f40c;
thunk proof9223ae457c0291a530fe5ee33e790866e99df14f84475a971508e459ab5b5acb.

Both exact full helper/wrapper versions pass GNU O2 host64/i386 SSE sanitizer
and leak checks:16,992 cases/2,310,243 checks per ABI, all char flag modes,
signed colours, half-UV untouched bytes, finite count boundaries and complete
typed storage/canaries. Observed actual system-header inventories are reviewed,
not mislabeled pre-frozen NDK dependencies. Begin/End admit valid vertex state
and record count/cursor; material/transform/packet/GPU fidelity is not claimed.
Runtime result SHA256a05bbe07efd4897b5fa0fa67e8144a3b22e259d19f548b7072b6847adfccfe01
andf1d26a277915357dd179ddde3f1af1fd339abac5c7f1e2d608a525633ff072ea.
Root integrates only the byte-identical frozen420/421 candidate owners so far.

### Batch418 lever cursor and original frame publication

The full509-byte original Update captures its typed lever base and publishes
negative32768-scaled animation frames in both branches. One ordinary live-count
cursor loop and float-to-i32-to-u16 conversion restore these operations without
negative float-to-unsigned undefined behavior. The sole unchanged O3 owner pair
gains194.09842954 weighted bytes across29 backed rows; nine exacts retain with
no qualification errors and unchanged undefined symbols. Update13.805555→
51.814816%,504→504 bytes; no additional candidate or report pair is attempted.
Strict Counter changes stop the historical gate until additive read-only
qualification maps every44 old literal/label identities and their consumers.
The sole genuinely new four-byte negative32768 constant matches original
PT_LOAD bytes and has exactly two Update references. Numeric Reset jump-table
labels retain exact locations/storage/edges; all other storage is unchanged.
Three backed functions' small register-byte changes are explicitly inventoried,
not mislabeled invariant. Two unbacked thunks retain code/COMDAT/unwind and
complete direct-call closure, without fabricated original absolute metrics.
Manifest SHA256127f960262154de17be73c42a24dacd64c8cd90042b0d86886a40ccd24e6ebb4;
capturecba1dc01e88121c31ea8fd68532d25a1d30bd5def8be743b929bffe040f1638d;
storage proof02f9b280efc11dbabcf2ecacc6f4fd52ef81bffec895b797fc2bf11457220d2c;
thunk proofee5f451ebea1fb2d73cfaa4d1c02e963e038e1856896805103d869c9866a30da.

Root runs both full actual bodies using canonical typed allocated pools and
finite state/timer/progress admission. GNU O3 host64 and i386 SSE ASan/UBSan/LSan
each pass276 cases/8,834 checks; the old body has8,818 expected intermediate
retail deviations and its separate control exits2 cleanly. The independent
modulo-frame oracle avoids the changed cast expression. Exact state, canaries,
unprocessed tails and nonmutating audio-call position/order retain; full audio,
engine or malformed/nonfinite inputs are not certified. Both explicit finite
dependency closures qualify before either build, with no prefix exemptions.
Runtime manifest90a39c4596ec4e3a37a5c1f19fa30a1b27b8b10eb055f594b6c9200447c94084;
closure434222009eef4b12e41dbc607ffb7c4336af6233b91ee91d22c21a2eef6c9644;
both run logsbfe8f0abc0ebc0f56036ee33b69e22e9b6647af745972fdc1b0e5313277ffa7c.
Root catches a patch-context mistake before any linked report, restores the
untouched AddGizmos loop and verifies the entire integrated owner byte-identical
to the frozen candidate. The initial build log is retained; the corrected
target is rebuilt before measurement. Only validated418/420/421 enter this unit.

### Checkpoint34 linked lever, gesture and rendering unit

One fresh whole-linked report and independent live Bazel mapping measure
68.732025%,+0.005630 percentage points versus checkpoint33 and+0.729565
versus current main. All6293 prior raw exact name/address/size identities,
the4,722,419-byte original denominator and complete ownership counts retain.
Corrected target/native/WebAssembly builds and all five repository tests pass.
Raw SHA2564c2ed110e620ce412e1f452aa75c8f5e4e9b8ae55645ef4a1adabf13a1e4ca4f;
mapped1c721045fb28f503b07df9d73277c0aa47e72d2af34d20caaa18242d620ee203.
The normal commit hook will independently regenerate the report before push.
The70.002460% cycle threshold remains unmet; queued422–426 are not counted.

Checkpoint34's full normal format/tidy/symbol/report hook passes and its
generated matching.json is byte-identical to the independent mapped capture.
Root commits/pushes93adf13a7baf9b34bcc7d58434a069b487c8ca87 with Fabian's
authorship and Codex attribution. Final-head checks are monitored separately;
pending CI and the unmet cycle threshold do not permit a merge.

All eleven GitHub checks subsequently complete successfully on the exact
93adf13a head. Independently captured final-head SHA256
1c75f531253a9099516539ae0846a78493da513bb1ae533b6130fcf4042194e7
qualifies both identity and all COMPLETED/SUCCESS conclusions. The unmet
70.002460% cycle threshold still prevents a merge.

### Batch425 bounded rejected text-decoder trial

Root reviews the complete516-byte original decoder, real codeword helper,
canonical UTF16/UTF8 producers and one frozen full-owner actual O2 pair.
Six omitted CJK normalization branches and the promoted continuation-byte
range expression are restored with individually bounded480-byte publications
and short-circuit NUL lookahead. The sole gate's63 uniquely backed subtotal
is−146.6391504 weighted bytes; TextDecode28.418440→qualified protocol0,
367→800 emitted bytes, all two known exacts and complete Counter/U retain.
The original-backed startup pairing remains explicitly unknown on both
sides; unbacked Text3DEx.part.0/thunk scores also remain unknown. This is a negative known
subtotal, not a fully qualified whole-owner total. The matching trial closes
without further representation proofs, runtime, variants, reports or integration.
The actual behavior correction remains separately frozen, not claimed fixed.
Manifest SHA256d31e96b0d5ee377fef01b1522393e5288c9f02526501af371c9032b80b1eba7f;
capture09d01c876c368293472eba6ae74a902053bae54c2c1c215da8486d87689b4f9a.

### Batch422 original current-manager effect-node assignment

Root reviews the complete366-byte original Handle assignment, canonical typed
node/list/effect lifetime and allocation calls before the sole actual O2 pair.
The original allocates a12-byte/aligned4 zeroed node on the current manager,
not PushNuListNode's scratch-manager/accounting discipline; guarded placement
construction reloads the live source value before the existing Append contract.
One necessary standard new header is already in the actual production closure.
The pair gains156.37868988 weighted bytes across44 backed rows, retaining
all eleven exacts; target32.556602→75.283020%,290→352 bytes, only target
score changes. Complete Counter/U deltas explicitly replace Push with BlockAlloc,
add one-byte empty string storage and renumber the unchanged float1 constant.
Original raw allocation targets/arguments/empty string, all other code/REL,
startup consumer/storage and canonical append links are independently proved;
two unbacked thunks retain code/COMDAT/unwind/call closure, never absolute scores.
Manifest SHA25620dcb5be909c6d3c6e874df21c2b34ad23f27145e59fdcc5098ebce31970706d;
captureb352334e82a41cc662e42db5f320ee447e98cce454001ef422ffd3e4d20faa2e;
specific proof95e88419a9a47595e5cbbef8cf97d2dc366332f7353f30fc34185d24e954c19a;
thunk proof9c3dfabfacc1cd34516ef78dc4ebcf8898ecb919004d6fdd2cfa8a8d7901ff17.

Root separately approves all four actual GNU O2 preflight closures before any
objects, then runs full old/new bodies on host64/i386 SSE with ASan/UBSan/
float-cast/leak checks. Each passes32 cases: old8,700/8,702 checks, new7,548/
7,550. Real canonical Handle/list/effect lifetimes, ordered links/values,
duplicate references, empty-source removal and complete traced allocation
disciplines retain. The old and new expected disciplines differ deliberately;
this is not false allocation equivalence. One identical fixture-only friend
admits nonempty lists; no public gameplay producer was found or fabricated.
Allocation services are typed seams, not real TLS/allocator certification.
NULL voices only; unchanged non-NULL voice paths are raw-qualified but playback,
Voice lifetime/hardware/concurrency are not runtime-certified. Each actual
destructor releases all nodes; source/effects/output lifetimes are genuine.
Runtime manifest49c5f30787a26039118b2c544ac882a98a52c096b79ec7ff062186352a5d922a;
staged manifest e2f36758cdd24d0d94428cf712ff2bc7682e2b4cc71fd4cd5ec56d290349aaa3;
reviewed preflight4079879d72adeb0c9eaf1d44874daaf5df88ac9fd9e56c99aa78f3632236eee6;
resultb1f12868039ce09a43d1ee12d8faa1449f5a692e901c45c0888733ce7fac582e.
Root integrates the whole byte-identical frozen owner; linked measurement follows
at the next checkpoint, not implied by this isolated gain.

### Batch424 network-point captured traversal qualification

The full1479-byte original network search captures its typed item base/promoted
u8 count before one-based ascending traversal; scratch dx/dz writes still occur
for blocked/nonwinning candidates. One ordinary source correction retains these
operations and the count255 sentinel256 without helpers/options/ABI changes.
The sole actual O3 pair gains166.14424887 weighted bytes across57 uniquely
backed rows, retaining all four exacts and complete Counter/U with no gate errors.
Target45.065790→56.299343%,1474→1471 bytes; only target score changes.
Manifest SHA25624cfc523d558d806e25885c6d7f9578e85f125057da588704e8331df9168b8ae;
capture7bed4e5fca9191002f7c5ec89525810cb17568e7ce0dac28d47c1da2c5f8ed12.

Four unbacked emitted units remain absolute-score unknown:93-byte startup,
1124-byte private state helper and two12-byte PC-thunk sections. An additive
full-owner representation proof covers every non-target full normalized body,
all allocated storage/REL/COMDAT/init-array, four complete FDE/CIE records and
122 incoming code/data/unwind edges. Its first execution stops on two unowned
alignment jumps; failed script/log are frozen. A targeted complete direct/REL
census then independently identifies exactly two identical15-byte EB0D/13-NOP
gaps, preceding terminal backward jumps, no gap symbols/REL/incoming edges and
bounded next-function destinations. Root approves only these exact descriptors;
every other unowned transfer still fails. The qualified additive proof passes,
without compiler/report/source retries or invented original weights/scores.
Script SHA25651aa4df80a143f981e35e2641c2d3fe41be085a18030de2ee598d5d15d6fcf94;
proof443126830374d61467983f51a208dc0b0971cf72e67e3a23770c4838808e442d.
Root completes both staged GNU O3 host64/i386-SSE diagnostics after personally
reviewing the actual header/backend/link closures and finite additive atomic/LTO
catalog. Both145-case runs pass724 checks with ASan/UBSan and leak detection;
both executable representations and recursive library identities qualify before
either execution. Canonical initialized networks cover counts0/1/255, blocked
and nonwinning scratch writes, direction flags and NULL-connection fallback.
Independent binary32 result checks and complete input/network immutability pass.
No invalid indices, concurrent mutation, full gameplay or Android runtime claim.
Stage9a4843c537d093106558bc814d31dcad2b23a48e96a932e9bd350e5e046942bb;
resume catalog7c9d7b4a1bd8a50e72e5d579edb550ad47c446b211b4789b40ac39c0587befdf;
result7083deed4a306584e605e5145de7943b44dfa8f4d60fc95e990e50d0cad41b60.
Root integrates the complete byte-identical frozen owner; linked gain remains
subject to the next independent whole-library checkpoint.

### Batch427 bounded motion history screen

A fixed twelve-name checkpoint34 motion/combat screen finds only previously
closed reconstructions or recorded compiler-layout debt. This is a history
eligibility screen, not a fresh complete-body equivalence proof. No new
candidate, compiler/report trial or runtime is prepared. FindTargetObject's
recorded frame/register/trace differences do not establish omitted behavior;
FindFurthestPlayerFromVec's retained NULL safety is not replaced with retail
absolute-address writes. Low percentages alone do not reopen rejected variants.
Frozen audit SHA25695a13ca48a71d2e50a74feadc3420418334e2f83d1b510fe30ff263b1aaa86f6.

### Batch426 captured episode-clip traversal

The complete393-byte original captures player/level/item bases and initial
promoted u16 extent, refreshing the extent only after an accepted ID write.
One typed traversal retains NULL player/output behavior and ascending IDs.
The sole actual O2 owner pair gains33.72713424 weighted bytes across20 backed
rows; both exacts and complete Counter/U retain. Target27.508196→36.090164%,
159→195 bytes; two unbacked thunks retain complete representation/dependencies
under a separate same-object proof, never assigned absolute scores.
Manifest696ddd65b113fb0aaff99412882ffc109c6a14893a578e353d80e9ba360226c2;
captureffcaf9044ca60a6e287abfc887d9fd2383b45d1af3c814953cf60733e7585ff3;
proof1ad9611b1f78e26d999606e13751d67103075c4aa708679da984f5fa99860c5c.

Root reviews both actual GNU header/backend/link preflights before objects.
A missing finite atomic-script/library and embedded no-LTO-wrapper inventory
is repaired additively before execution; original runners/preflights remain
frozen, no source/command/report retry. Existing ANDROID declarations accompany
HOST_BUILD without Bionic headers. All four full old/new GNU O2 host64/i386
SSE sanitizer/leak runs pass21,552 cases/3,079,697 checks each, with empty runtime
stderr. Canonical live arrays, signed episode extremes, optional output, extent
0/1/128, ordering/canaries and complete input/global immutability are checked.
No fabricated alias mutation, parser/gameplay execution or universal128-clip
asset bound is claimed. Caller output capacity remains a valid-domain precondition.
Additive catalogff7f6bbd12e40db23e9cee96867900a824b5b51a915bc70b387d6af079bcd2f3;
host resultf5f12fc0284517b7655b0daa8d8693a85d9dc175401ac8a991d8147cefd5bc3e;
i386 result7d0f992293026ff67984f0ff23111d0e282346bceff1d4087ea3e18cfd50ecbe.
Root integrates the complete byte-identical frozen owner; linked gain follows
at the next checkpoint rather than being inferred from the isolated pair.

### Batch428 rejected debris-expiry load trial

Root reviews all1472 original bytes, canonical32-chunk/eight-key records,
complete real list/lock helpers and arena producers. One typed expiry-only
table reload retains the initially captured effect for active/panel/render/key
operations. No fabricated helper mutation, concurrent arena safety or gameplay
bug witness is claimed. Both actual O3 preprocess closures/commands are fully
reviewed before the sole frozen owner pair and two original reports.
The nine-backed-row subtotal falls463.63717794 weighted bytes; DebFree
45.193550→13.727273%,1328→1368 bytes, AllChunks79.619050→79.261900%.
One exact and complete Counter/U retain with no qualification errors.
Unbacked startup/bx/cx scores remain unknown; this is not a qualified whole-owner
total. Negative trial closes without further proof/runtime/variants/integration.
Gate manifest49998515f9e1676761d91dac623d1cb1b9f35350fb372106d3eae356c38bb9b6;
capturec7e2e0a6b2046587e8aa2c2907fb701e1c103cba37aa0b356004bb1254fb29a1.

Two additional bounded history/frontier screens avoid repeating existing work.
The twelve-name below20% screen contains only earlier restorations/reserves;
audit5cf23a92beff1405fb3877eb18b7e5b75d67c1971c68eb5192b72b7cffac8521.
The renderer screen promotes only NuPostEffectInit for a fresh full raw/current/
canonical helper/producer audit: all six allocations and ordered resources/
lock/reset operations are present; actual2048-byte filter storage fits1308 bytes.
No missing GPU services are inferred from genuine retail platform no-ops.
Other eleven entries are metadata/history-only, not newly proved full bodies.
No candidate or gate is prepared for either screen.
The analogous twelve-name gizmo/object screen also promotes no unit, excluding
named earlier complete closures/reserves before new raw/helper review; it is
history-only. Audit405a83992ed2a8aba3d837d9fd75b0077e88ecf8c5e1edefd21d0b2a31b232ec.

### Checkpoint35 linked integration

The independently rebuilt Android library integrates only the fully qualified
sound-handle422, network-direction424 and episode-clips426 frozen owners.
GNU native, WASM and all five repository tests pass. Live Bazel ownership maps
the whole-library version2 report to68.739555%, +0.007530 percentage points from
checkpoint34 and +0.737095 from main68.002460%. All6293 previous raw exacts,
original name/VA/extent multiplicities,4722419-byte denominator and452-unit
ownership inventory retain. No isolated gain is substituted for linked gain.
Raw report66d2f1c7e87625fda194920cc42e4c8e7a7ffc0157d7b757a45f6afcdf8fb0ad;
mapped29399aaeb7aecfe579dd2e83158f91461d51ccb32ff661768c37b785555da6b5.
The +2-point threshold70.002460% remains unmet; no merge is authorized by this
checkpoint. Normal commit-hook regeneration and pushed-head CI still follow.

Batch423's one additive representation proof initially stops before proof at a
mutable unrelated producer pin after the approved426 integration. The failure
and original gate/scripts/reports remain preserved. Root reviews an explicit
historical snapshot boundary covering all182 old inputs and the exact complete
426 file delta, then the additive proof passes: all77 non-target normalized
bodies, five unknown full representations/unwind/incoming, allocated storage
and complete symbol/undefined inventories qualify. The only surface delta is
the specifically proved removed0.0f literal .LC1 and original-backed typed Game
reference; no prior undefined symbol is lost or absolute unknown score invented.
Proof09861bb75cd95d4236dc04f3a6940b5f56d8c45c0f0b7c2594ce31112028dcb0.
Runtime preparation is authorized, but423 is not integrated at checkpoint35.

Checkpoint35's normal commit hook passes all checks, target/native/WASM tidy,
symbol inventory and report regeneration. Its mapped report is byte-identical
to the independent capture. Commitbc155affde47bb962a2b1113c7cf314cfdf010a6
preserves Fabian's authorship and the verified Codex co-author trailer; pushed
toPR121. All eleven checks are independently verified COMPLETED/SUCCESS on
that exact pushed head. The merge threshold still remains unmet.
The hook formats only line wrapping in422's allocation expression;424/426
remain byte-identical to their frozen candidates, and the complete mapped
report stays byte-identical to the independent pre-hook linked capture.

### Batch431 rejected touch-layout admission trial

Root reviews all508 original bytes, actual menu/controller helpers, pause/frame
producers and canonical level/area/control-mode fields. One minimal correction
uses retail's global Paused admission and conditional second GetMenuID call,
retaining the bool signature and all existing guards. Actual producer stability
is explicit; no fictitious divergence/mutation or gameplay defect is claimed.
After both complete actual O3 preprocess closures/commands receive root review,
the sole frozen owner pair yields a negative32-backed-row subtotal:
−63.00033374 weighted bytes; target17.327870→4.9262295%,481→517 bytes.
All five exacts retain; no comparison qualification error. Full symbol Counter
differs only by added UND Paused; unbacked bx/cx absolute scores remain unknown.
This is not a qualified whole-owner negative total or an automatic U exemption.
Negative closes without representation proof, runtime, retries or integration.
Manifest61f2eb3c570995e723332a6df6f4e9a4e8ecd5f0db415906a75b7aa36d4e6b51;
capture8772129d9ff1d3e989a315153fe7bb60528dee82e2a8fb5c85d5de831966b3ab.

### Batch433 rejected deflate cursor trial

Root reviews all1059 original bytes, complete decoder/Huffman helpers, original
constant tables and actual file-reader storage/callers. One guarded source-end
cursor retains forward overlapping byte expansion and each current_pos store;
invalid table/history/capacity domains remain excluded, not claimed repaired.
Both complete actual O3 preprocessing commands and14-input consumed closures
receive root review before the sole frozen owner pair and two bulk reports.
The eight-backed-row subtotal falls34.06913136 weighted bytes; target
46.282894→43.065790%,1035→1035 bytes. One isolated exact retains, complete
Counter/U agree and no original comparison qualification error occurs.
Unbacked bx/cx absolute scores remain unknown; this is not a qualified whole-
owner total. Negative closes without further proof/runtime/variants/integration.
Manifested31aceb55fae2f7807668084f8767c56d1ce604d5c587d9486cb09ad3f35876;
capture22008f3e85c23be2f02e056b8290f427d84507bf7fb75b2f9730ff05fb775b9a.

### Batch432 rejected AI-reset count trial

Root reviews all589 original bytes, canonical64-object/AI records and actual
script/state/string/object-reset/count producers. One carried signed count with
five existing admitted-tail refreshes retains all guards and service order;
stable rejected paths are not a fabricated callback/gameplay defect witness.
A metadata-only external .d path resolver failure is preserved and explicitly
repaired additively; a root-created log inside the strict partial inventory also
stops before writes, then is preserved outside that inventory. No source,
compiler command or comparison pair is repeated for these metadata stops.
Actual live aquery confirms unchanged O2, not root's earlier O3 assumption.
Both149-input actual preprocessing closures/commands receive full root review
before the sole pair/two bulk reports. The13-backed-row subtotal falls
20.94220717 weighted bytes; target55.111110→51.555557%,541→588 bytes.
One isolated exact retains, full Counter/U agree, no qualification errors.
Unbacked bx/cx absolute scores remain unknown, not a whole-owner total.
Negative closes without proof/runtime/variants/integration.
Manifest2bb27326b3a96de667641f6713f23c10d196f154956932972149e3a6350cce78;
capturef29a8d083e7f48e06759b257f35b966a683368ca275680a3a7d090386446073a.

### Batch434 provisional editor-path dataflow trial

Root reviews the complete2656-byte original target, private helper, canonical
editor records, actual index/import producers and all five actual callers.
One captured signed-promoted node index and post-helper connection-slot reload
retain existing fanout/guards; stable helpers are not a fabricated mutation
witness. Retail's private register ABI remains an untouched limitation.
Fresh Android aquery confirms actual O2. Both147-input actual preprocessing
closures and commands receive root review before the sole owner pair and two
bulk reports. The53-backed-row subtotal rises118.93271210 weighted bytes;
target67.710710→72.162730%,2720→2848 bytes. No baseline exact or comparison
qualification errors; complete Counter/U retain. The render row also changes
53.318386→53.352016% at unchanged1928 bytes, not yet proved incidental.
Five private/startup/thunk absolute scores remain unknown. This provisional
subtotal is not a qualified whole-owner gain or integration authority: an
additive same-object representation proof is prepared before any runtime.
Manifest96173988d01af861c9c8ce9a9be34a71dbc0ba309c001d55485ae71444da7e1e.

### Batch423 runtime-qualified PodRaceB integration

Root reviews both actual GNU O3 host64/i386 preprocessing, complete individually
pinned363-input union and exact backend/link/runtime closures before objects.
Both old/new builds retain the reviewed dependencies and actual ELF interpreters
and sanitizer DSOs. Thirty candidate cases pass400 checks per ABI; thirteen
safe old controls pass158, and eight named old failures return ordinary failure
without sanitizer errors. Actual thirteen extracted helpers are retained;
stable lap/race/audio-allocation boundaries are disclosed, not fabricated
gameplay mutation. No NDK/full-engine runtime claim or invalid domain admission.
Runtime resultsdd4e1385d345fdc840baa6c6bb407fd946d1d62c70f4896e5feb4c29067f4bd4.
Root integrates only the byte-identical frozen episodeI.cpp candidate: original
instance readiness, negative countdown boundary, add-then-cap, unfaded pulse/
alpha logic and original-backed area completion/hint threshold. The qualified
isolated gain232.00716485 weighted bytes is not the linked-project result;
checkpoint36's independent linked report and all6293 prior raw exacts follow.

### Batch435 rejected tube-motion unit

Root reviews the complete536-byte original target, canonical context flags,
actual pool/count producers, pure cylinder helper, final audio route and caller.
One frozen context-mask/typed cursor/split flags unit retains service order;
invalid pool capacity and context domains remain excluded. Fresh Android aquery
confirms O3; both142-input actual preprocessing closures and commands receive
root review before the sole owner pair/two bulk reports. The27-backed-row
subtotal falls12.22229808 weighted bytes: target39.107693→36.738460%,573→592
bytes. Eleven exacts retain; full Counter/U agree and no qualification errors.
The ObjInTube row changes98.928570→99.464290%; unbacked bx/cx absolute scores
remain unknown, not a whole-owner total. Negative closes the entire candidate
without representation proof, runtime, variants, retries or integration.
Manifestc6c1db58b819ac518b5c876d659eb944deb9ee2ec480d3da032877121813fc25.

### Batch436 provisional quadtree indexed-entry trial

Root reviews the complete2134-byte original LOCAL AddElementR, canonical union/
header, actual allocation/storage helpers and only actual tree producers.
One frozen indexed-entry/first-child predicate unit retains original helper
behavior; NULL-copy/capacity, initialization, overflow and cyclic-tree debt stay
excluded, not repaired or fabricated as gameplay evidence. Fresh Android aquery
has NO optimization argument; the compiler default remains unchanged, with no
added-O0. Both15-input actual preprocessing closures and full commands receive
root review before the sole pair/two bulk reports. The11-backed-row subtotal
rises603.85926040 weighted bytes; target66.505880→94.802940%,1693→2166 bytes.
Two existing exacts and full Counter/U retain; no qualification errors. Unknown
bx remains unscored; this is provisional, not a qualified whole-owner total.
Separate additive same-object proof precedes any runtime or integration.
Manifest4f1952179d8e6096be43a7936e553afc03e48f202f497d58bdaad328fdd610c1.

### Checkpoint36 independent linked validation

Only423 is integrated;434/436 remain provisional and435 is closed negative.
Target/native/WASM builds and all five repository checks pass. Independent
original/linked measurement is68.744510%, +0.004955 points from checkpoint35
and +0.742050 from main68.002460%. All6293 prior raw exacts, original raw
name/VA/extent multiplicities,4722419-byte denominator and452-unit ownership
inventory retain. Whole-project progress is not extrapolated from isolated
owner gains. Raw18ffc003b0ab1aff57a844d465bf77cb43b8258cfe5e6e5c78b1720a9aec50e9;
mappedadfeea2d27b493d64977a5f489757e6fc35cd7afcbc619c5f26534159f9d468b.
The70.002460% merge threshold remains unmet. Normal commit-hook regeneration,
Codex attribution, push and exact pushed-head CI validation follow.

Checkpoint36's normal hook passes full format/tidy/symbol/report checks. Its
generated report is byte-identical to the independent capture; episodeI.cpp
stays byte-identical to423's frozen candidate. Commit536af50c8007ae6bd4642864fae18422d18c4a52
preserves Fabian's authorship and the verified Codex co-author trailer and is
pushed toPR121. Initial exact-head checks pass repository/matching/site checks;
platform checks are still pending, not declared green.

Subsequent independent GitHub inspection verifies all eleven checks
COMPLETED/SUCCESS on exact pushed head536af50c8007ae6bd4642864fae18422d18c4a52.
PR121 remains open because the70.002460% linked matching threshold is unmet.

Batch434's sole additive r2 proof stops at the complete normalized render body
comparison; failed log and all frozen objects/reports/scripts remain preserved.
Read-only metadata isolates two actual register bytes at Render+39d/+3a0:
baseline loads/tests member+24 intoECX, candidate intoEAX. Same JE destination
and PC-relocated targets alone do not qualify these opcode differences.
An exact both-branch register/flags-use audit follows separately; no generic
normalizer exemption, proof rerun, runtime or production integration is granted.

### Batch438 finite minikit status census without a candidate

A worker freezes87 eligible item/object/props rows, subtracting82 historical
named/family exclusions before cap12, leaving only five metadata survivors.
Exactly one complete raw/current/type/helper/producer audit covers1130-byte
MiniKit_LSW_Update at1ee8b0, including all status states and real service order.
No substantial finite-valid-domain omission supports a source candidate;
the trial closes without compiler/preprocessor/report/runtime expenditure.
Raw state6 unordered-time handling remains a disclosed nonfinite discrepancy,
not a claimed finite-equivalence or gameplay repair. The other four survivors
remain deferred, not audited/closed. Frozen worker audit438 records the scope;
this census is not a new root runtime/full-engine equivalence certification.

### Batch436 non-target representation qualification

Root executes the separately reviewed same-object proof once. All ten
non-target functions and the unscored12-byte bx thunk retain complete decoded
bodies after the specifically checked AddElementR call-displacement adjustment.
Full storage, symbol identities,37 relocations, groups and12 unwind entries
retain their documented relationships. No register/opcode masking or unknown
score imputation is used. Runtime preparation follows separately; no production
integration yet. Proof capture786bb96840dcebaa49a46efe65cf1c120751707b4cba31c9a41b1eadd4e00f21
pins the sole matching pair, not a new comparison.

### Batches439/440 finite faithful closures without trials

Worker439 audits the complete5735-byte KaminoC_Update and actual canonical
level state, packet masks, literals, producers and direct services. Existing
phase/occupancy/selection/visibility/sound logic agrees in the admitted loaded
level domain; NULL-first-player and low tile-count debts also exist in retail.
Worker440 audits the complete1105-byte HomeNearestTorpTarget with canonical
packet/bolt/type records and actual finder/vector/angle/matrix services.
Its steering, lifetime snapshots and target publication agree; invalid target
type/draw-position and unconfigured type debts remain excluded. Neither audit
supports a source candidate, compilation, export or runtime trial. These are
bounded worker static closures, not root full-gameplay equivalence certificates.
KaminoE and UpdateRippleSet are the next separately scoped deferred audits;
other survivors remain unknown. Frozen440 manifest20b9f95b8ee784c7ad245b49af0753c75417a5e16bf296b66e7112a52f535932.

### Batch437 Whip source-form proposal and metadata boundary

Root reads the complete1854-byte original, complete current owner and actual
duration/helper/type/producer contracts. Retail reloads the canonical target
field after duration computation before phase-zero aiming. The sole proposal
adds that typed reload inside the existing guard; the real duration helpers do
not mutate the target, so this is source-form matching, not a gameplay bug or
fabricated callback witness. Active scheduling remains unresolved. Fresh aquery
confirms the unchangedO3 action; an initial empty overescaped query is preserved
and never used as compiler authority. The first metadata-only gate staging
stops on an indentation mismatch in its verifier, before preprocessing,
objects or reports. Failed partial staging is preserved; a separate additive
metadata correction must keep the frozen source candidate unchanged.

Root reviews the additive indentation-only correction and both actual146-input
preprocessing closures before the sole unchangedO3 owner pair. The trial loses
33.8958477 original-weighted bytes: Whip_MoveCode48.257618% to46.429363%,
1682 to1730 emitted bytes. All three backed functions retain their identities,
Counter/U inventories and zero prior exacts; the two unbacked thunks remain
unknown. This negative candidate closes without variants, runtime, non-target
qualification or production integration. The first failed staging remains
preserved and is not recast as a matching attempt.

Batch434's separately reviewed additive r3 qualification admits only the exact
two Render register bytes after both branch def-use audits. EAX is killed before
either successor uses it; ECX is neither read nor published before the linked
font path overwrites it. All other body bytes, relocations, storage, groups and
unwind relationships retain their exact checked invariants. This is a narrow
representation proof, not a generic register normalizer or gameplay runtime
certificate; the failed r2 comparison remains preserved. A bounded scalar
bitmap diagnostic follows separately before integration.

The first434/436 runtime preflights stop on exact GNU dry-plan metadata paths,
before objects or runtime. Additive metadata dispatchers preserve their frozen
fixtures, candidate bodies, headers and sanitizer/compiler options; they admit
only the separately reviewed shared directory/files and exact named dependency
outputs. Actual both-ABI input review remains required before any compilation.

The corrected434 preflight individually qualifies both39-input header closures,
actual backend/link inputs and sanitizer ELF dependencies. GNU64/i386 diagnostics
each pass520200 admitted bitmap pair/slot cases,65536 promotion-only signed-i16
values and7479411 checks with ASAN/UBSAN/LSan enabled. This certifies only the
four exact bitmap scalar expressions, not path geometry, renderer execution or
the private helper ABI. Root integrates the exact frozen434 source candidate;
linked whole-binary retention/improvement is still required before committing.
Runtime result023dbfc46d9ca308e31c1cd6a64c66575239a59f2778876dd03fd542d8819c66.

### Bounded faithful closures and unmeasured source-form leads

Separate workers close the deferred5430-byte KaminoE_Update and514-byte
SplineKnot::Smooth as faithful in their documented initialized finite domains.
KaminoE retains all six phases, both-bolt first-distance reuse and actual
connection/WAIT reset semantics; Smooth retains all neighbor cases, tangentW0
and the actual return-zero terrain-ray service. No source candidate or matching
trial follows those closures, and no full-engine/runtime claim is made.

The683-byte GizSpinner_ResetAll audit likewise finds faithful material behavior;
the original target/previous adjacent-u16 store order remains an explicitly
unmeasured reserve, not a claim of zero possible gain. A metadata nomination or
shared-owner inventory is not a completed function audit. The separate671-byte
GizmoPickups_StoreProgress has an ordinary captured typed cursor/count and
independent512-stop lead, plus original memset order. The816-byte UpdateRippleSet
has a stable scan-phase frame/list/count capture and final-count publication
lead. Each has only one frozen source proposal; matching is still unmeasured.

Batch436's reviewed GNU64/i386 default-O0 sanitizer diagnostics each pass2763
finite initialized-storage cases and32685 checks;2760 old controls pass, while
the old first-child predicate fails the separately expected safe fallback case.
Actual12-byte i386 and24-byte native entries are distinguished, with no claim
for serialized/native layout equivalence, NULL-copy debt or full-engine runtime.
ASAN/UBSAN/LSan produce no findings. The exact frozen436 candidate is integrated;
runtime result1c51e04716e79e4b864b8a09e2b15ab5b5c76b3f915bc0e7fa62ee72c1ae6830.
The isolated target rises66.50588% to94.80294%, with603.8592604 positive
original-weighted bytes,11 backed functions and both prior exacts retained.

The next bounded CP36 history-first deformation screen subtracts all14 eligible
previously audited/trialed bodies before its cap; the rendering screen likewise
subtracts13 audited/reserved rendering bodies and leaves24 outside its scope.
Neither supports a new trial. These are narrow no-new-target findings, not a
claim that all low-matching functions are exhausted.

Batch442's sole unchanged-O3 whole-owner pair follows root review of both143
actual preprocessing inputs and the exact scan-only source difference. It loses
461.56401888 original-weighted bytes: UpdateRippleSet68.633026% to12.068808%,
858 to867 emitted bytes. All19 backed functions, fullCounter/U and zero prior
exacts retain; two zero-extent thunks remain explicitly unknown. The negative
candidate closes without variants, representation proofs, runtime or integration.

### Checkpoint37 linked validation

The integrated434/436 changes pass final native, WASM and Android target builds
and all five repository checks. One independent linked report reaches68.759820%
fuzzy matching, +0.015310pp over checkpoint36 and +0.757360pp over main.
All6293 raw exacts, original identities,4722419-byte denominator and source
ownership retain. The +2pp threshold70.002460% is not reached; no merge is
authorized by this checkpoint. Raw report9dfc40bab1f342aa6c995a20e1f6f19975d20897962ac6738ae49516751cd0cf;
mapped reportb226682efff28eb8fc8fe65f1625eef5e20adc79090849ab6c894db2374acd37.

### Batch443 pickup-progress provisional positive

Root selects the exact gizmos/fx action from all three fresh owner actions;
the first empty-filter query is preserved but is not compiler authority. After
reviewing the exact180-entry production dependency file (178 unique inputs),
both actual preprocessing closures and the cursor/init-order-only source
difference, root executes one unchanged-O3 full-owner pair and two bulk reports.
StoreProgress rises62.570470% to94.543625%,710 to670 emitted bytes, with
214.53987005 positive original-weighted bytes. All33 backed functions and11
prior exacts, fullCounter/U and original identities retain without pairing
errors. Four unbacked private/thunk bodies remain unknown; they need separately
reviewed specific representation proof, not imputed zero scores. Finite runtime
preparation follows separately. No production integration is authorized yet.
Gate manifestaa097e08780cd0fdad96128c924b2b6b7fb23c054216babfe903f8583cb8df32.

The separately root-reviewed443 proof retains all36 non-target representations,
including the1515/809-byte private parts,93-byte startup and both actual12-byte
COMDAT thunks. Only four individually asserted CALL/LEA operands change; each
still resolves to its exact original function entry. All REL/addends, static
incoming/interior references,37 unwind records,184 raw symbols, complete storage,
groups and executable/container gaps are accounted for. Unknown absolute scores
remain unknown. Qualification30a8eab98dd205d1f2d01f7bb789ad869e8c0b01469d7b433dcf4362a275c37b.
Both initial GNU runtime compilations fail before objects because the fixture
omits the canonical NuMemoryPS declaration header. Failed logs/closures are
preserved; an additive fixture include-order repair does not change candidate,
records, options, oracle or matching evidence. Runtime remains unqualified.

The exact pushed checkpoint37 headb8773c32da9890d0308be4aa7373c6104b8a5885
passes all eleven GitHub checks (COMPLETED/SUCCESS). Its below-threshold matching
gain still does not authorize merging.

Separate complete audits close2970-byte KeepOnScreen and1388-byte edpartLoadPageEx
as faithful in their explicitly admitted initialized finite domains. The former
retains last-response/count and original-velocity overwrite quirks; the latter
retains versioned loading and start-before-publication order, with both actual
retail endian/count services verified as nine-byte no-ops. Shared malformed-input
and nonfinite debt is recorded without speculative repairs. Neither yields a
candidate, compiler trial or runtime fixture. Source-only motion survivors and
partial editor nominations remain open rather than being blanket-excluded.

The additive443 runtime packet changes only canonical NuMemoryPS declaration
include order and exact r1 output paths. ROOT reviews both actual complete
294-input catalogs, unchanged backend/link/runtime closures and both compiled
ELFs before the sole sanitizer execution. Old and new GNU64/i386 each pass5497
cases,811893 checks and10737 calls, including negative counts, allocated counts
above512, all flag bytes, bitmap word boundaries and NULL combinations. Stable
allocated record storage and the original always-zero activated bitmap quirk
retain; ASAN/UBSAN/LSan emit no findings. Resultce996a1d57d201a81e6a7f59c30c120c7b4c7132af51921792646d13cbb0f747.
The exact frozen443 candidate is now integrated; no full-engine/Android runtime
or broader alias/gameplay claim is inferred from these native diagnostics.

Batch444's sole unchanged-O3 whole-owner pair measures1348.16085963 positive
original-weighted bytes; Titles_Draw47.594593% to92.103040%,3221 to3061 bytes.
All25 backed functions and the prior exact retain, with no pairing or undefined
surface changes. The gate explicitly reserves six renamed zero-extent LOCAL
text labels (.L169..174 versus .L206..211), not an automatic renumbering waiver.
Specific whole-object qualification and finite runtime remain pending; no
candidate integration, matching retry or alternative source variant occurs.
Capture2b69f7c26f1e5f21c8b07a52851e313ba04ade0aeb5277379d4d7a1966b2b127.

### Checkpoint38 linked validation

The integrated443 pickup change passes native, WASM and Android target builds
and all five repository checks. One independent linked report reaches68.764360%
fuzzy matching, +0.004540pp over checkpoint37 and +0.761900pp over main.
All6293 raw exacts, original identities,4722419-byte denominator and source
ownership retain. The +2pp threshold70.002460% is not reached; no merge is
authorized by this checkpoint. Raw report8fca470fc6904d9e2e727eb87deee8d24493b3ff3db9cfd107ab4269f1423e89;
mapped report09c463e024ed06db3955c2f4da66868961d4a996399fce8a7c2644c4b23faedc.
Exact pushed-head GitHub validation follows separately. Batch444 remains
isolated and is not included in this linked gain.

Batch445's sole unchanged-O2 whole-owner pair follows ROOT review of the full
147-input actual preprocessing closure and exact three-line original traversal
flag transfer. It loses49.68204704 original-weighted bytes: Enter42.672276% to
41.613860%,4085 to4149 emitted bytes. All53 backed functions, original identities,
fullCounter/U and zero prior exacts retain without pairing errors. Five unbacked
private/compiler bodies remain unknown. This negative candidate closes without
variants, compiler/report retries, representation qualification, runtime or
production integration; the original quota-failed metadata packet is preserved.

The initial444 same-object proof stops on the GameTimer ELF symbol-table index
293 versus291 in an otherwise unchanged Level_Update instruction/relocation.
Its failure and all original artifacts remain preserved. An additive specific
proof checks exactly33 literal/undefined-symbol index permutations, retaining
full symbol identities, complete raw tables and every actual relocation index;
it does not change source, objects, matching reports or score policy.

The additive444-r1 specific proof passes: all28 bodies,27 non-target
representations,827 full symbols,27 sections and28 unwind records retain.
The six zero-extent labels and all33 exact symbol-index permutations are
individually qualified; unknown absolute scores remain unknown.
Qualificationa0cc235f5e79399c6dae453f9dc8083da4ebcd2c1d263358dd53d1b26395eea6.
Both initial444 GNU runtime preflights stop before objects because the dry-plan
emits an already admitted side directory with a trailing slash. All partial
metadata and failures are preserved; an additive exact-directory normalization
repair is prepared separately. Runtime and production integration remain pending.
MenuUpdateBonusMode remains OPEN: prior broad case/source nominations do not
constitute a separate full-body/helper closure. This correction does not change
Titles444's already frozen selection, candidate or matching evidence.

All eleven GitHub checks are independently verified COMPLETED/SUCCESS on the
exact checkpoint38 pushed head8c671d7829362398651314049045c05992a55d46.
The below-threshold gain still does not authorize merging.

Batch446's initial representation proof stops on a GCC D5 group signature
defined in its GROUP section rather than the member code section. The failed
proof remains preserved. The additive446-r1 proof qualifies exactly three
such unchanged LOCAL NOTYPE signatures and all eight member-defined signatures,
their raw symbol/header/name/payload bytes and complete whole-object code,
relocations, data, incoming edges,43 labels,171 cases and143 unwind records.
Qualification4989c2a8180e019c6303b875ac71928435ef59d17e120f67a25bf52a95abb022.

The frozen446 old/new GNU64/i386 O2 sanitizer diagnostics each pass896 cases
and9859 checks with identical same-ABI output, using actual canonical records,
context/cheat producers and math helpers. All four runs have empty stderr and
status0 under ASan/UBSan/leak detection. Stable direct SuperPush admission is
diagnostic only: no shipped nonzero producer, concurrency or full-engine/NDK
equivalence is claimed; existing math/helper debt remains outside this change.
Resultff8e2fe03f902911ca6e9c4c432570569ad1408a5f85747361720b8023d89737.
The exact frozen446 source form is integrated pending linked validation.

Batch447's sole original-O3 owner pair gains192.92421995 original-weighted
bytes: turret best-target53.449665% to69.953020%,1065 to1112 emitted bytes.
All37 backed rows and six exacts retain. Acceptance stops on specific constant
storage changes (.LC23 cst4/.LC28 cst16 to .LC23 cst16) and two unknown thunks;
whole-object qualification remains pending. No source variant or matching
rerun is made, and this isolated gain is not counted in the linked report.

Batch446's sole unchanged-O2 whole-owner pair measures368.390733 positive
original-weighted bytes: ForcePushed_SetTargetMom46.503570% to74.625000%,
1273 to1281 emitted bytes. All136 backed rows and4 exacts retain; it is the
only score change. The full non-label Counter and undefined surface retain.
Seventeen removed/added label names correspond to43 renamed symbol roles;
specific qualification must check every role and all11 unbacked private/thunk
bodies without assigning unknown scores zero. No source variation or matching
retry follows. Capture9fbce899dc265da738ed8928fe9ef4fa1ca994ea702c8caed485e1eeeabb984f.

The additive444-r1 runtime repair normalizes only the exact already-admitted
header/side directory identities, preserving original scripts, failed metadata
and logs. ROOT reviews the complete334-input two-ABI catalog, actual unchanged
backend/link/runtime closures and all four compiled ELFs before execution.
Old/new GNU64/i386 sanitizer/leak diagnostics pass92/94 cases and1828/1853
checks per ABI. The two candidate-only quiet-NaN cases avoid old invalid
float-to-i32 conversions; old UB/nonrepresentable inputs remain excluded.
Actual timer/vector/special helper excerpts and canonical records are used;
the stable full-signature renderer recorder and finite diagnostic trig table
do not establish production GPU/NDK/full-engine equivalence.
Result1e8f85c4e3af6d5aa7e055503b1254bb1eaddf7f2b942162a9125fb53136862f.
The exact frozen444 candidate is now integrated pending linked verification.

### Checkpoint39 linked validation

The integrated444 ordered title comparisons pass native, WASM and Android
target builds and all five repository checks. One independent linked report
reaches68.793020% fuzzy matching, +0.028660pp over checkpoint38 and +0.790560pp
over main. All6293 raw exacts, original identities,4722419-byte denominator
and source ownership retain. The +2pp threshold70.002460% is not reached;
no merge is authorized by this checkpoint.
Raw reportd640bd581de7a74628eab5db133553e669cef5715f28e8f2f2b8848e5c0c2ba7;
mapped reporta6e3b5c553e9aee8607e2c7cf552f28a184277551572c8dc39a4a09be4d75890.
Exact pushed-head GitHub validation follows separately. Batch446 remains
isolated and is not included in this linked gain.

All eleven GitHub checks are independently verified COMPLETED/SUCCESS on the
exact checkpoint39 pushed head7b5440cc4867b8c032e7660148f0175bf0654b5b.
The below-threshold gain still does not authorize merging.

### Checkpoint40 linked validation

The integrated446 force-push loop form passes native, WASM and Android target
builds and all five repository checks. One independent linked report reaches
68.800860% fuzzy matching, +0.007840pp over checkpoint39 and +0.798400pp over
main. ForcePushed_SetTargetMom is the only linked score change; all6293 raw
exacts, original identities,4722419-byte denominator and source ownership
retain. The +2pp threshold70.002460% remains unmet; no merge is authorized.
Raw report5d2bce5114a0af0b1c162dcfcd2f451d3e8c9014246a37de810ffa7084f1c0c2;
mapped report60d4e03506505f91c65960d28e35f6d5049bc2cdbddc8cedbd103a7524a504dc.
Exact pushed-head GitHub validation follows separately. Turret447 remains
isolated pending specific qualification and is not included in this gain.

All eleven GitHub checks are independently verified COMPLETED/SUCCESS on the
exact checkpoint40 pushed head61c2b4af1275504caa17bce1c22ee93560ca2693.
The below-threshold gain still does not authorize merging.

### Subsequent isolated candidate qualifications

The sole450 bonus-menu owner pair loses400.92598720 original-weighted bytes:
MenuUpdateBonusMode51.631866% to36.891940%,2633 to2695 emitted bytes. All six
exacts retain. This candidate is closed with its objects/reports preserved;
no source variant, matching retry, runtime or production integration follows.
Capture881f9690d4a1729097c924860a6cfdc93ee555a0ef06f60edc1befb93a1b3534.

The sole447 specific representation proof passes all39 bodies/FDEs, the exact
five startup literal roles, target-only scalar removal, seven undefined-symbol
roles, complete storage/COMDAT/symbol tables and static incoming census.
The two thunk absolute scores remain None; only their invariant representation
delta is established. Known gain192.92421995 bytes remains isolated pending
runtime verification.
Qualification71f67e39e5e95afaa479d356599d7062abe0496dbfd449595b2ff3267d93348e.

The sole448 mine owner pair gains308.08738774 original-weighted bytes:
UpdatePodRaceMines44.072464% to64.973915%,1339 to1431 emitted bytes, with all
nine exacts retained. Its specific proof passes all78 bodies/FDEs, exactly13
direct CALL displacement descriptors and target-following alignment, all518
symbols,1493 to1501 relocations, storage/COMDAT/container and incoming census.
Five unbacked absolute scores remain None. Runtime verification and production
integration remain pending; this gain is not counted in the linked report.
Qualificationa20e395cbd2ec220f165b2279fc7ac8e9a5b1f6aa3b85eb3fde401c8f6825e02.

The sole449 camera owner pair gains129.93183504 original-weighted bytes:
GameCameraMakeMiniCut3 49.532770% to54.919662%,2323 to2406 emitted bytes,
retaining all three exacts. Its specific proof passes all39 unchanged nontarget
bodies,40 FDEs, exactly11 label roles/12 case entries, complete storage,
symbols/COMDAT/container and static incoming census. Both distinct original
93-byte startup rows remain ambiguous/None; no original-TU identity is inferred.
Their actual emitted startup/thunk/storage dependencies are invariant. Runtime
verification and production integration remain pending; no linked gain counted.
Qualification9457ebe2f82413eb7d6aeb0331c952503532d38d639d73bd966bcb5e189f8343.

The frozen447 GNU64/i386 O3 ASan/UBSan/leak diagnostic passes333 ordinary
cases/9782 checks per executable with identical old/new same-ABI output.
Real lazy interface allocation and registered weak-reference deletion are
exercised. The separately admitted finite225000000 distance witness passes
in the candidate and produces exactly four expected old output failures per
ABI, with empty sanitizer stderr. This geometry is diagnostic, not a shipped
asset claim; no concurrency, NDK/Bionic or full-engine equivalence is asserted.
Result1bd347768e85a2e2a5f9929378d8cb7c9c3981393595409c9cabba3e80e0f797.
The exact frozen447 source is integrated pending linked verification.

Before any449 runtime staging, ROOT identifies the missing final blank newline
in utilities.py versus prepare.py's exact446 lineage assertion. No runtime,
compiler or matching retry occurred. The frozen original packet is preserved;
an additive runtime-only metadata repair is prepared separately.

The separate449 runtime r1 repairs exactly one terminal LF in the utility;
the original packet and four fixture/helper/authority/workflow copies remain
byte-identical. No matching compiler/report/proof retry occurred. Both exact
GNU64/i386 O2 ASan/UBSan/leak old/new diagnostics pass20258 cases and283613
checks per executable with identical logical digestc78831a9 and empty stderr.
All fourteen flags, bounded duration comparisons, reset/append and valid
type3/4/5 capacity31/32 cases are covered using actual five canonical helpers.
Type6/7 count32, dangling borrowed pointers, nonfinite conversions and border
width debt remain excluded. This is source/helper regression, not NDK/device,
floating-exception timing, concurrency or whole-engine equivalence.
Result6af568b5490e48063b4f5ac666566d392c0a6aa7a696f5bd34943356678c9124.
The exact frozen449 source is integrated pending linked verification.

### Checkpoint41 linked validation

The integrated447 turret threshold snapshots/nearest-distance seed and449
camera guarded remainder pass Android target, native and WASM builds and all
five repository checks. One independent linked report reaches68.807740% fuzzy
matching, +0.006880pp over checkpoint40 and +0.805280pp over current main.
Only these two targets change score. All6293 raw exacts, original identity
multiset,4722419-byte denominator and full source ownership summary retain.
The +2pp threshold70.002460% is not reached; no merge is authorized.
Raw report6e9a52bb8bc20d6e0748495d4381afebdabc37550ce7188e2b9b244bf3b2c5b4;
mapped report0892d2264dc8005c03a3dda5d1d03b657c3ab50934e7e45630d54b6ab1dccb9e.
Exact pushed-head GitHub validation follows separately. Mine448 remains
isolated pending runtime verification and is not included in this gain.

All eleven GitHub checks are COMPLETED/SUCCESS on exact checkpoint41 head
7c63c4221d43b022a917d42e7d0a8fa47eff6a12, with main still
a496c28beece66a24ccdf7336571b6836f45e6aa. The +2pp gate remains unmet;
this green checkpoint does not authorize merging.

### Isolated over-gap gesture452 and mine448 diagnostics

The sole452 whole-owner O2 pair restores early initialized packet capture,
guarded speed reload after normalization and lowercase global-player velocity
backup/rollback. Null-config speed-zero guards remain; null global player
deliberately falls back to the argument object instead of reproducing retail's
null dereference. Actual global identity is proved by its original GOT slot,
not inferred from Player[0]. No callback mutation or shipped multiplayer claim.
The pair gains212.72231837 original-weighted bytes,37.780373% to59.915890%,
1089 to989 emitted bytes. All12 exacts and complete symbol/undefined surfaces
retain. Specific representation and runtime proofs are pending; no linked
gain or production integration is claimed.
Capture68872b3a568ccb07f3e4d82b1321541f42c2220e011fa81aade52a9cc506dea9.

Mine448 original runtime preparation is preserved with pre-integration camera
source provenance. Additive r1 freezes the same three used camera helpers;
its first GNU64 metadata preflight stops on unlisted cstddef before objects.
ROOT fully reviews that sole consumed header; additive r2 individually pins
it, retaining all131 prior GNU headers. Both r2 metadata preflights pass, but
the first host64 old fixture compilation stops on misspelled VARIPTR.voidptr;
canonical common.h declares void_ptr. All failed evidence is retained. A
separate one-token fixture spelling repair is authorized, not a matching or
test-oracle variant. No mine runtime pass or linked gain is yet claimed.

Mine448 additive r3 repairs only that fixture member spelling and its strict
metadata assertion. Both GNU64/i386 O3 sanitizer builds pass. The sole runtime
run stops at host64-new-controls on the repeated collision/reset counter
assertion; old host64 reset/helper controls pass, and no i386 runtime follows
the failure. This result remains unqualified pending read-only oracle/admission
review. No production change, matching retry or runtime-pass claim follows.

The sole451 editor rotation candidate restores three raw-backed translation-row
copies into the existing escaped average scratch. Its O2 full-owner pair loses
9.02935875 original-weighted bytes:25.462633% to24.754448%,1103 to1291 emitted
bytes. All74 exacts and symbol/undefined surfaces retain. This candidate is
closed without variants, representation proof, runtime or production edits.

The initial specific452 representation proof stops before output because its
original-thunk assertion wrongly requires absence rather than absence of a
positive-size exact-name row. All original/candidate objects and reports stay
unchanged. Only an additive exact-row metadata repair is being prepared;
unbacked thunk scores and original weights remain unassigned.

The additive452-r1 metadata repair pins both original LOCAL HIDDEN zero-size
thunk rows exactly and leaves all remaining proof constraints unchanged. Its
sole execution passes all44 function names/40 physical bodies and FDEs,
254 symbols,14 COMDATs, full relocations/storage/container and static incoming
references. No nontarget instruction-operand exception is needed. Only the
target extent, five following function coordinates, exact target-following
padding and sixteen zero vtable-alignment bytes differ. Absolute thunk scores
remain unknown; their complete emitted representations have zero delta.
Qualificatione16e04df4a82df1bbcdd84dd3521575f8e20a0399d8a35eb04390fc308367e40.
The target remains isolated while bounded canonical runtime preparation proceeds.

### Mine448 and turret453 qualification

Mine448 additive r4 repairs only fixture construction: canonical APIOBJECT
position and collision_position share a union, so its prior separate writes
unintentionally moved contacts outside the admitted radius. All assertions and
target bodies remain unchanged. Both GNU64/i386 O3 ASan/UBSan builds pass.
Each new ABI passes2150 cases/95259 checks/2129 target calls with identical
diagnostic digest. Old i386 passes98 retained-control cases and fails the six
independent acknowledgement, presence, word, overlap, SFX-position and NaN
witnesses as expected. Old GNU64 passes21 reset/helper/storage cases with zero
target calls; its fixed-byte pool makes target admission invalid. No old64
equivalence or full-engine/asset/audio/GPU certification is claimed.
Runtime result981a0b40f6f12533faa001fa37fcaedc2d639af01d5b7d602a6a4cc9a3da560e.
The previously qualified sole mine body is integrated pending linked validation.

Turret453 restores the two retail zero-extended16-bit angle loads without
changing ABI or options. The sole O3 owner pair gains12.902678186 weighted
bytes,5.3446107% to5.5759244%, with all6 exacts retained. Its specific proof
requires exactly two BF-to-B7 opcode bytes across the entire31580-byte ELF;
all other bytes,38 nontarget bodies,39 FDEs, full relocations, symbols, storage
and container remain identical. Exhaustive65536 component-payload integer
algebra proves both sine/cosine LUT indices identical for signed and unsigned
widening in the actual consumer. This is not a gameplay-failure claim.
Qualification8b5fa4f0884e5fcdfdfa4ffa514ca9690f7049574d6c846f323adf51f70781c2.
The sole two casts are integrated pending linked validation.

### Checkpoint42 linked validation

Android target, native and WASM builds and all five repository tests pass.
The independent linked report reaches68.814570%: +0.006830pp over checkpoint41
and +0.812110pp over main. Only UpdatePodRaceMines44.182610%→65.200000%
and GizTurrets_Update5.375295%→5.614477% change. All6293 raw exacts,
original identity multiset,4722419-byte denominator and complete ownership
summary retain. The70.002460% merge threshold remains unmet.
Raw reportc0e4aba394674ea40af81b67c1428b5fc7703a5987ef1c23839cd6d472f26226;
mapped report54f428413ef97ac10833c23448a11a5c84b8a1bf415fbae2e235a24a49b85429.
Exact pushed-head GitHub checks are still pending; no merge is claimed.

### Isolated DPad454 closure and over-gap452 fixture prerequisite

DPad454's sole O2 pair replaces only the raw-backed complete strength
expression. Both owner compiles and consumed dependency closures pass; the
candidate report's initial serialization fails EDQUOT. All original objects,
the complete baseline report, partial candidate report and failure log remain
frozen. Additive r1 reuses the baseline and serializes only the same candidate
object once to a fresh path, with unchanged strict comparison logic.
The completed comparison loses37.91062407 original-weighted bytes:
30.287823%→26.959410%,1109 bytes unchanged. All27 original-backed names,
complete symbol/undefined surfaces retain; three unbacked emitted names remain
unknown. This candidate is closed without further variants, representation
proof, runtime or production integration.

Over-gap452's original diagnostic stage and both212-input metadata preflights
pass. Both sole old ABI fixture compiles stop because the fixture names
unrelated CHARACTERDATA_s rather than APIOBJECT's actual characterdata_s
pointee. No runtime has run. Failed evidence remains intact; only an additive
canonical fixture-type correction is being prepared, with target/helper bodies
and all runtime assertions unchanged. No linked gain or runtime PASS is claimed.

### Over-gap452 additive fixture qualification and integration

The additive runtime452-r1 changes only four exact fixture/common type tokens
to the canonical characterdata_s pointee. The complete failed original packet
remains hash-pinned and unchanged. Both fresh dependency preflights retain212
inputs per side; all329 catalog inputs independently pass SHA verification.
Actual commands, backend dry plans, library/ELF closure and helper bodies
equal the fully reviewed original apart from these type tokens, exclusive
output paths and compiler-generated temporary names.
Catalog03be941c69954eb23b159663d6a1b94d342baa0e23a9d61f6fb039d83312e48d.

Both GNU O2/SSE host64 and i386 old/new builds pass. Each actual binary passes
8448 cases:768 admitted and7680 rejected, including2816 null-global cases.
Old diagnostics pass128650 checks each; new diagnostics pass128394 checks each
including256 distinct-global rollback cases. All four runs return0 with empty
ASan/UBSan/LSan stderr. Actual task-denial, canonical terrain casting and
separate old/new recipient oracles are bounded source diagnostics, not retail
execution, successful-task, dynamic-terrain, asset or whole-engine certification.
Result2311094a9387f535cf1632454615434c67036deb60ec51b8e68419a419876426.
The sole previously qualified candidate is integrated byte-exactly as whole
ownere4be9371c05331e8fcc1275acfd69894a09b5604cdba6286ecf88c374aa63539,
pending linked validation; no isolated matching pair/report/proof is repeated.

Checkpoint42's exact pushed headed932080a833a2bf0366d38c11e35a5979ef424a
has all11 GitHub checks COMPLETED/SUCCESS. Its +0.812110pp main-relative gain
still does not satisfy the +2pp merge threshold, so the PR remains open.

### Checkpoint43 linked validation

Android target, native and WASM builds and all five repository checks pass.
The independent linked report reaches68.819084%: +0.004514pp over checkpoint42
and +0.816624pp over main. Only ProcessAutoJumpOverGap changes,
37.981308%→60.140186%, with all6293 raw exacts, original identity multiset,
4722419-byte denominator and complete ownership summary retained.
Raw report5a02cf011619045384929193cd7e1eee472a02e529216e27bdd4d8bd29a000cf;
mapped report1c15dc105ca11f7b38e8f7332680396479462eaf60e63aa7c79f05bc012b7cb3.
An initial report command stopped before comparison because a diagnostic
build changed Bazel's convenience symlink. Resolving the actual target output
through cquery restores reporting without another matching build or variant.
The70.002460% merge threshold remains unmet; no merge is claimed.

### Checkpoint45: batarang unordered rejection

Batch455's frozen candidate passes all four GNU O3/SSE host64/i386 old/new
ASan/UBSan diagnostics, with leak detection enabled and empty stderr. Each run
covers10589 bounded cases: finite automatic/manual selection, tick/ray ordering,
ranking, full canonical batarang and target storage, canaries and actual target
position consumption. Old manual unordered-screen diagnostics select360 cases;
the recovered rejection selects none. Actual vector, distance, rotation,
GameRayCast, TerrainPlatId and GetTargetPos bodies are retained. The deep terrain
raycast uses a typed, validated stable-miss boundary; the four-angle LUT is not
asset or exhaustive-angle coverage. Automatic NaN distance independently rejects
in both versions; no fake forward-test witness or full Android/gameplay claim is
made. Runtime result
5be2f30d9422306deddaa1a9be52714e6b8b1e6291f13d16a5aee8ab015f10d9;
both-ABI input catalog
79efbaaacabe09413c43e8729e42885e06e3532a379dd4e7b67e4984cad22fc7.
The integrated owner is byte-exactly
b8aed289507ac1749556b27253507d3b8e03cdc3dd4ede2a29000fcc7da223b0.

Android target, native and WASM builds and all five repository checks pass.
The independent linked report reaches68.823350%: +0.000340pp over checkpoint44
and +0.820890pp over main. Only Batarang_FindTarget's .isra.0 body changes,
39.060220%→39.729927%, with all6293 exacts, original identity multiset,
4722419-byte denominator and ownership summary retained.
Raw report1f5b2f4e7d4c2b35a7a53a97b5498afb7ee9963f4db60df522f7dabc30920b12;
mapped report8a12728e638445c2dd15260f6eeeb6b243279102bd544e7f52a6cc26816edae1.
The first ownership-mapping invocation lacked permission to use Bazel's output
base; the same existing raw report was mapped with authorized access, without
another matching comparison. Both logs are preserved.

Checkpoint44's Linux CI initially stopped before compilation on a GitHub HTTP500
fetching apple_support1.24.2. The failed job is retried after workflow completion;
the other ten exact-head checks passed. This is not a source diagnostic, and no
build configuration workaround is introduced. The +2pp merge threshold remains
unmet.

The sole batch458 idle repetition/blend-out candidate has a provisional
1.1171875-byte gain,33.088543%→33.260418%,584→612 emitted bytes. All48
original-backed names and three exacts retain. Exactly one original-backed
animduration_blendouttime undefined reference is separately qualified; all
existing references retain. The unbacked SetLayers private clone and PC thunks
remain unknown. Batch459's draw-matrix/current-global-level candidate has a
provisional85.88137232-byte gain,44.597702%→56.459770%,600→776 bytes.
All49 backed names, ten exacts and full symbol/undefined surfaces retain; its
NuMechPtr destructor aliases and thunks remain unknown. These are isolated
whole-owner results, not production integration or runtime certification. Both
require separate same-object qualification and bounded canonical diagnostics;
neither authorizes another source variant or matching comparison.

The fresh Batarang manual-character distance suspicion closes without a trial:
the full original2439-byte body restores64.0 before the merge and deliberately
retains that limit for subsequent categories. Batch122 already documents this
recovered behavior. A broader family-name history search is required before
promoting an exact private-helper name whose older prose uses its public caller.

### Checkpoint43 CI and isolated batches455–457

Exact pushed head e334ce104b5cbfe8801b240ef32a8ec327304847 has all11
GitHub checks COMPLETED/SUCCESS. Its +0.816624pp main-relative gain remains
below the +2pp merge threshold; the PR is not merged.

Batch455 changes only four original-backed unordered rejection predicates in
Batarang_FindTarget: the two automatic forward tests and manual blowup screen
X/Y tests. Distance/ranking policy, including the retained64 handoff, is
unchanged. Its sole actual-O3 owner pair/two original reports gains16.28971515
weighted bytes, target38.970802%→39.638687%,2242 bytes unchanged. All17
original-backed names, known exacts and full symbol/undefined surfaces retain;
two unbacked PC thunks remain unknown. The sole same-object qualification passes
all18 non-target bodies,19 FDEs, full REL/storage/incoming closure and the
explicit133-slot literal/symbol permutation. Qualification
14afc238c12adf1125293e0d4dba17b5cceb127676d79dac0cb1060e1f418f92.
Bounded canonical diagnostics remain pending, with no production integration.
Captureba373a689e61bde823ddd115e4b503ef5b84e5e83aa7a6ed66d96a6c6821a07e.
The prior Batch122 fixture's NaN oracle follows current predicates and is not
retail-policy certification. Automatic NaN distance can independently reject
a candidate; do not manufacture a forward-test witness with fake helpers.

Batch456 restores LineToSphereIntersection's endpoint-roundtrip direction,
ordered expanded quadratic arithmetic, two actual square-root calls and both
ratios before nullable output publication. Its sole actual-O2 owner pair/two
original reports gains184.9378042 weighted bytes, target42.140244%→68.750000%,
463→711 bytes. All49 original-backed names and ten exacts retain; full symbol
and undefined surfaces retain. The sole same-object qualification passes all51
complete physical bodies/FDEs, full relocation/control inventories, startup,
storage, literal and two COMDAT representations. The only target-related layout
changes are explicitly enumerated; all50 non-target representations retain.
Two unbacked PC thunks keep unknown absolute scores and original weights, with
identical complete emitted representations. Qualification
f117ad4b9dda2aacd7379c63d5a9fc984b80b343aada14906b01d329fb7b015b.
Bounded canonical diagnostics and linked validation are recorded below.
Capturee2b113f8a7890eceb1e5b8ce350550f39fa732eaf327096664740e0361cc0605.

Batch457's complete original-backed GizTorp_Draw effect-ID/Fmod/global-scale
candidate, including the safe captured-system fallback outside retail's valid
global domain, loses282.40984548 weighted bytes in its sole actual-O3 owner
pair/two original reports:46.144386%→9.844920%,745→821 bytes. It also adds
the actual NuFmod undefined reference; that surface change is not exempted.
No known exact is lost; both unbacked PC thunks remain unknown. The negative
candidate is closed without variants, representation proof, runtime or
production integration.
Capturee248c09dc1f2a80c59a81c9c1b5d3871eed7a3d6119b30056dfdf5ae85ffee2d.

The new complete read-only GenerateWaypoints audit finds no missing contract
within initialized, allocated, bounded path storage. The retained index30
pair→index32 final-write hazard exists in original and current code; this is
boundary debt, not a universal safety claim or a matching proposal. Receipt
db10fc1ede4aee86fe86384b44f36f2d27383a35bd52417a8e2d9b5b267cfe05.
The separate complete cbFileSaveEffects audit likewise finds no supported
repair: the defaults, page choices, overrides, backup/save/UI sequencing and
directory restoration are represented. Neither low score justifies a new
source-copy/register variant or a matching experiment.

### Checkpoint44: line-sphere arithmetic and publication

The frozen batch456 candidate passes all four GNU O2/SSE host64/i386 old/new
ASan/UBSan diagnostics, with leak detection enabled and empty stderr. Each run
passes24 finite target cases, five actual NuFsqrt threshold checks,192 assertions
and three separately checked old-mechanism discrepancies. Canonical vector
storage, nullable/shared outputs, untouched rejection sentinels, exact squared
epsilon boundaries and adjacent discriminants are covered. The complete actual
nufloat_android.c helper is unchanged; no fake callback or helper-call-count
assertion substitutes for its behavior. Endpoint quantization and expanded-C
cancellation cases are finite diagnostics, not shipped-camera asset witnesses.
No full editor/camera, Android runtime, output/input alias, nonfinite, hardware
libm or universal floating-environment claim is made.
Runtime result6fd09779303ea89f56ff05f5cea8d68103ab52da21d7398eab5af8e0af40009d;
both-ABI input catalog6d9b181619fb20e1d29e0a01d86d30d8b94742c7c62b33e32bd1a7f71e6e4cda.
The sole candidate is integrated byte-exactly as utilities.cpp owner
65ecf4b01a6feb766f0994128d39e3655c770880361c3eb10d83f12c5b133ef7.

Android target, native and WASM builds and all five repository checks pass.
The independent linked report reaches68.823010%: +0.003926pp over checkpoint43
and +0.820550pp over main. Only LineToSphereIntersection changes,
42.097560%→68.853660%, with all6293 raw exacts, original identity multiset,
4722419-byte denominator and complete ownership summary retained.
Raw report9fdc9fd7ee759dcd4ccc99509589d1e934ab104fc3f1b6194181da28168dc863;
mapped report6741f5215994cb2628180790f22f755ccbc5cedd56b1b90f7bd6df582ec7e1b1.
The initial metadata verifier stopped on a string-versus-integer size assertion;
an additive verifier decodes the existing report's numeric size without another
comparison, build or source variant. Both the failed verifier and partial output
are preserved. The70.002460% merge threshold remains unmet; no merge is claimed.

### Checkpoint46: idle selection and BuildIt reflections

Batches458–460 are integrated as their exact qualified owner bytes. Idle
selection draws from the raw repetition range before publishing/clamping, then
subtracts AnimDuration's actual blend-out publication rather than FRAMETIME.
BuildIt drawing restores the local matrix copy/scratch reuse and current-global
level override, retaining the captured-world fallback outside the valid original
global domain. MatrixReflection's zero-result hook vetoes the override selection,
not reflection itself. The separate VU0 helper and ResetCharacterIdle remain
unchanged; no calling-convention, compiler-option or source-placement shortcut
is introduced.

The separate same-object proofs qualify all emitted representations, including
unbacked clone/destructor/thunk deltas, without inventing their absolute scores.
Batch458 result3ff042461dfd2d5d0d29088f7f1e93ad15edfbbc3cc6021684167723044830ce;
batch459 additive resultfacc320839ea6d9963e934d86edd69054c832b757c5bc749de0b110035c2f45f;
batch460 additive resultc38b70189185dbab81dffd4ccc7bb5358a4bb6be7aa2dd64ff3d9f02cfec8b2f.
The original459 proof rejected a defined weak destructor GOT address load;
the additive proof admits only that exact startup registration row. The
original460 proof rejected unequal incoming thunk coordinates; its additive
schema2 proof qualifies the exact PIC call/add role with the coordinates still
explicitly unequal. Original failures and objects/reports remain preserved;
neither repair repeats code generation or matching comparisons.

All twelve old/new GNU host64/i386 sanitizer runs pass with leak detection
enabled and empty stderr. Idle passes974 finite cases/10875 checks per run,
using actual RNG/animation helpers and three explicit old-policy controls.
BuildIt passes83 cases/3286 checks per run with actual reset/object-pool/list/
matrix helpers. Three context diagnostics and the nonmutating frustum/GPU trace
seams are explicitly limited; there is no manufactured matrix-mutation witness.
Reflection passes40 cases per run,1729 old/1793 new checks and four zero-hook
controls. Its typed diagnostic callback is not the installed private game hook.
No full renderer/gameplay, shipped-asset range, Android runtime or universal
floating-environment equivalence is certified.

The first BuildIt diagnostic fixture compilation stopped before linking on
three globals incorrectly defined with C rather than canonical C++ linkage.
A fresh additive fixture repairs only those declarations; all target/helper/
oracle/options bytes and failed evidence are preserved. Its fresh full consumed
dependency/backend/link catalogs precede all objects and runtime. Final runtime
results: idle1726e00837aaead824832066759fc04e9afffa427954280417e4d3ae305c5dcd;
BuildIta33baf7abeb798eca3fc9a4dc7e6876b45809e623d1265b1e9b1b2995ceb5f2f;
reflection5223e72e92486f8012b11f781cc8df3520b807018639a28cafce67505774b3e1.

Android target, native and WASM builds and all five repository checks pass.
The independent linked score reaches68.825820%, +0.002470pp over checkpoint45
and +0.823360pp over main. Only NewCharacterIdle33.114582%→33.312500%,
GizBuildIts_Draw44.626440%→56.459770% and
MatrixReflection88.451920%→92.750000% change. All6293 raw exacts, original
identity multiset,4722419-byte denominator and full ownership summary retain.
Raw report69f601fb4f6e1ab41e2ad34b94ec83dfa1894ed32a38d700a54a5c3bbe62853c;
mapped report8364335d7d888e9b82be6c0eb7901b3a5dd466f1adc4d578891e7f9b30048fac.
Checkpoint44's infrastructure retry and checkpoint45's exact pushed head now
have all11 checks COMPLETED/SUCCESS. Checkpoint46's pushed head
92fc7741800e5c7bb71e3c6688c6e0752f9b498e also has all11 checks
COMPLETED/SUCCESS; the70.002460% merge threshold remains unmet.

The sole batch461 OnClick two-getter/canonical obstacle-mode pair has a known
38.24376024-byte gain,43.050420%→44.160866%,2884→2899 emitted bytes.
All42 original-backed names and12 exacts retain, but the full symbol Counter
changes one local text label.L466→.L548. This is not automatically exempted;
exact label/reference, symbol-string permutation, adjusting-thunk and unknown
PC-thunk qualification remains separate pending work. No runtime or integration
is claimed, and no source variant or object/report retry is authorized.

### Checkpoint47 preparation: OnClick qualification and graph triage

The separate batch461 same-object qualification now passes: all44 emitted names,
40 physical bodies/FDEs,14 COMDAT groups, storage, incoming dependencies and
all REL/control records are covered. The exact renamed false-return label's two
switch-table references, nine UND/string-allocation permutation and natural
adjust-this108/JMP-to-entry role are explicitly qualified. Raw thunk bytes remain
separately unequal; the two private PC thunks receive no invented absolute score
or original weight. Qualification result
1cbbccad058dedd780c1dfd3cd8f8a411adea6ff6a7f532e0f2b219828efb9f6.
The original gate disposition and sole object/report pair remain preserved.
Runtime validation and production integration are still pending.

A separate read-only agent audit completes eduicbProcessGraph's supported body,
three direct graph services, creator/init/free-temp and selected-menu dispatcher.
The LOCAL1367-byte function at0x3903c0 remains38.012657% in checkpoint46;
no supported omission or candidate is nominated, so no compiler experiment runs.
This is distinct from the reserved RenderGraph callback-parameter ABI issue,
which remains unresolved. Malformed controls, invalid indices and arbitrary
allocator/nonfinite behavior are not certified. ROOT reviewed the readable
closure and immutable inventory, not an independent full raw-body re-audit.
Audit96b3cd75965c97bcce34ee6b1172179c76eb0db92588eb2ae7e4dbf20f21865e;
freeze61f6a1d32e9d9d3de8d280a6aee60a875c1eeca2be477a0ec3df350ddfbd03de.

Batch462 PlayRadio's sole minimal matched-source cleanup correction is reserved:
the whole-owner actualO3 pair loses76.86803215 original-weighted bytes,
44.300716%→39.818615%,1656→1624 emitted bytes. All four original-backed
rows are uniquely qualified; no exact is lost and full Counter/U retain.
Both private PC thunks remain explicitly unscored; no additional qualification
or runtime is pursued after the negative known gain. Production stays unchanged,
with no variant, repeated object/report or speculative fanout rewrite.
Capturefd410d49538e95b77d0df58ca58c80c9c7ad0515de01d2025491d38813325b48.
ROOT first checked the full actualO3 argv/environment and both143-input E
closures against the frozen94-project/49-external catalog. A read-only review
assertion initially compared raw dependency spellings too strictly; the43
external names intentionally become absolute under the already-reviewed command.
Their exact resolved identities, order and hashes retain. Only that metadata
comparison was corrected; no preprocessing, compilation or matching was repeated.

Batch463's history-first ProbeSemantics check stops before a new raw audit:
the concrete checkpoint41 census already covers the complete898-byte body,
canonical parameter/type/semantic/cache helpers and critical-section services,
with no omitted operation found. Its owner hash remains exactly
03a7fb3b6ab4a44d2dc1b7c64daeae85c87a06fae48ed5bdbeb1f02e326a22f7.
That actual closure supersedes the later medium shortlist's source-only UNKNOWN
label; a missing ledger name is not freshness evidence. ROOT read the complete
historical census08acf9e70300497fed27be535042361f58bd8f9200ccd698df108430950f0a52.
No candidate, export, raw re-audit, compiler experiment or runtime runs.

Checkpoint47's OnClick runtime PREP passes both metadata preflights, but its
first GNU64/i386 old-side links stop on missing LEGOCONTEXT_JUMP and
LEGOCONTEXT_BIGJUMP definitions. No runtime has run. The original fixture,
catalog and failed link logs remain immutable; an additive fixture-only repair
must use the real context definitions, with no target candidate variation.

The cost-frontier metadata shortlist is corrected by the concrete flat-text
checkpoint24 census: both DrawGameObjectsProcess and UpdatePartEmits already
have complete original-body/helper/producer audits. Their entire owners and
canonical layout headers retain the recorded hashes. The later bounded Markdown
history query missed that TXT authority; it is not evidence of a new omission.
ROOT read the complete historical census before requesting any repeated raw
audit. DrawGameObjectsProcess closes without export or experiment. The documented
small typed pair-cursor opportunity in UpdatePartEmits's final switch flush may
receive one grounded source-form proposal; no gain is predicted. DrawWeapons's
batch110 reconstruction likewise remains a historical authority, not a fresh
callback-mutation proposal. These exclusions do not certify exact matching or
exhaust the wider source-form frontier.

OnClick's additive runtime461 fixture repair now passes all four GNU O2 SSE
ASan/UBSan/LSan diagnostics. Each executes22 cases with603/605 native64 checks
and627/627 i386 checks; all four stderr files are empty. The two old64 byte-policy
controls remain explicitly distinct from canonical typed mode selection; i386
asserts the actual mode offset0x91. Actual stable getters, obstacle publication,
task constructors/destructors and managed references execute. StartNewTask is
only a typed handoff recorder, not engine task activation or gameplay; native
multiple inheritance is not retail private-ABI certification. No changing-query
witness, fake task position storage or unrelated field mutation is introduced.
Both failed original links remain preserved; only the two real context globals
were added in the separately frozen repair. Catalog
a7116476978bef577b9b00129cdcede521afbe9cea4cc30cb9500bfe1d49d7d0;
four-run result1fa10bdfcf9e4a5efac30b60c65b1c91e8476854f7de91806e69c7621d2c6fa7.
ROOT integrated the exact qualified candidate owner90e2539b057ad4fcdfe354661e894d5b61e711bb3776508a9e31260433dfa29a.
Production build and independent linked checkpoint47 verification follow;
no additional source-form proposal is integrated in this checkpoint yet.

Checkpoint47's independent linked verification passes at68.826630%,
+0.000810pp over checkpoint46 and+0.824170pp over main. Only OnClick changes,
43.061226%→44.166866%; all6293 raw exact identities, the4722419-byte denominator,
original identity multiset and complete ownership summary retain. Android,
native and WASM builds pass. Raw report
ee9ae1f375c74506e190a7873552ea2ef9e0c72bccb8dee48689c76877425f2b;
mapped report0ba2b446dae213595f7a483e7d401ccd348228528628301b007c09f461017fab.
The70.002460% merge threshold remains unmet; new-head CI is not yet claimed.

Checkpoint47 commit efb0b45c7f7f7b142f15159f545585af90080824 subsequently
passes all11 exact-head GitHub checks (build37080807129/report37080805279).
The score remains68.826630%; green CI does not authorize merging below the
70.002460% threshold.

### Batches464/465/467 — bounded source-form trials closed negative

These are source-form experiments, not newly discovered gameplay bugs. ROOT
reuses the completed canonical/helper/history closure, reviews each actual
unchanged O2/O3 action and finite dependency catalog, and executes exactly one
full-owner pair and two original/object reports per candidate. Both actual
preprocessed streams differ only by the approved source unit; all inputs are
hash-checked. No compiler flags, attributes, ABI, ownership or helpers change.

* Batch464 `UpdatePartEmits` final deferred switch-pair cursor/end: actualO2,
  172 consumed identities. Known original-byte weighted delta−15.577089;
  target64.092180%→63.746790%, both3778B. All126 backed names and7 exacts
  retain; U retains. Two12-byte compiler switch objects rename
  CSWTCH.618/.630→.619/.631 without qualification; bx/cx remain unknown.
  Capture5d5d8255be863724951eb9360ea6231194c06c6bf3db1671965179f1081a032f,
  packet `/tmp/saga-part-emits463.AVY8T3/gate464`.
* Batch465 `NewScanRot` outer platform-group cursor/end: actualO3,
  154 consumed identities. Known weighted delta−786.66861689;
  target29.893126%→18.269787%,5880→5912B. All125 backed names and4 exacts
  retain; U retains. Minor NewRayCastScaleYMask/TerrainPlatformEmbedded
  report changes and four local text-label renames are captured, not waived.
  Six unbacked/private/startup/thunk representations remain unknown.
  Capturecc51c2d42a22892f639d5b7ce55aab15f167b5c1ec4f05d795278752d92cd742,
  packet `/tmp/saga-newscanrot-platform-cursor.auZv4z/gate465`.
* Batch467 `instNuGCutRigidSysUpdate` second actual visibility query after
  SetDrawMtx: actualO2,152 consumed identities. Known weighted delta
  −100.1885388; target17.273886%→qualified report-v2 scalar default0%,
  562→578B. All82 backed names and2 exacts retain; full Counter/U retain.
  Six unbacked/private/startup/thunk representations remain unknown.
  Capture2c99a57058404fcb9255d3c9f848ff0ecdef6cde89bfdc88850280f891159ba4,
  packet `/tmp/saga-rigid-visible-sourceform.tSyx7s/gate467`.

All three candidates close immediately: no source variant, object/report
retry, representation proof, runtime fixture or production integration.
Unknown scores/deltas are not imputed to zero; each negative is a known
subtotal sufficient to reject this fixed candidate, not full retail-fidelity
certification. Preserve the source/raw/action/closure/object/report artifacts
and these histories before screening a future frontier.

Batch466 `CustomiserMenu_Draw` terminal closing-bracket/break source form
also closes negative. ActualO2 consumes192 ordered dependency spellings,
189 unique identities (139 project/50 external). Known original-byte weighted
delta−65.16632421; target19.114380%→17.236929%,3106→3161B. All18 backed
owner names retain,0 existing exacts, full Counter/U retain; bx/cx remain
unknown. Capture6b987ec7ec8ecf7a4a370f530f49857c7c13d5be2914eeeaa7104f930e188296,
packet `/tmp/saga-custom-name-terminal-prep.ujqRhh/gate466`. ROOT verifies
the exact finite name-slot/letter/active-side records and actual Text3D
helper behavior, without a changing-callback witness or gameplay claim.
No variant, retry, proof, runtime fixture or production integration follows.
All four bounded source-form trials leave production matching unchanged.

Batch468's sole post-NuVecNorm typed force-system reload is provisionally
positive: known weighted delta+36.07794844B, target48.113636%→50.227272%,
1638→1654B, with all48 backed names,8 exacts and full Counter/U retained.
Its private Hit clone and bx/cx thunk representations remain unqualified;
no runtime or production integration is authorized by this subtotal alone.
Packet `/tmp/saga-force-system-reload468.EUKC6U/gate468` retains both reports.

A bounded original4395-byte Squish Compress3 audit finds subtraction-based
endpoint clamp predicates and double floor/ceil service calls differing from
the current scalar helpers. This is a concrete source-contract lead, not a
measured gain or final compressed-output divergence. Shared-consumer review
must precede any candidate; library versions, flags and algorithms stay fixed.

Metadata staging exposed the tmpfs per-user quota despite reported free space.
Eight inactive disassemblies (293578745B) are losslessly relocated into the
ignored `.cache/saga-evidence-archive-20261002` directory, with verified SHA256
and original-path links. Both interrupted metadata copies are preserved;
no compiler or report ran until the original frozen stage completed normally.

Checkpoint48 commit e6c7e51d9407756c3889d3e1124a38e02fe7d8ad passes
all11 exact-head GitHub checks. It records the bounded trials without changing
the68.826630% linked score or permitting a below-threshold merge.

### Batch468 qualification and checkpoint49 integration

ROOT qualifies the same sole objects/reports, without a compiler or report
retry. The private Hit clone and bx/cx thunks retain their exact bodies,
incoming/outgoing roles and CFI; absolute unknown scores remain None. The first
metadata proof stops on four shifted callback words in the existing40-byte
NUFPCOMJMP table. Its failed script/log are preserved. A fresh additive proof
checks exactly offsets4/12/20/28 against the same uniquely identified function
entries, all eight REL rows, the sole Configure/NuFParPushCom consumer and
actual typed parser dispatcher. The other24 table bytes and every other
non-executable storage byte remain exact. This is representation admission,
not a generic data-section exemption or parser-runtime certificate.
Successful receipt77856e2754efb1a36f78682eb52ea8fff92e7f840326a56a00f591cbcbdae93c.

Actual canonical records and unmodified vector/Fsqrt/atan/trig helpers support
four GNU64/i386 O3/SSE ASan/UBSan/LSan diagnostics. Each covers696 finite
cases across three actual threshold profiles, directional/planar/horizontal
modes, flags, bounds, strict nearest/range/ties and previous-target fallback.
The stable typed bolt query cannot mutate records; both sources must pass.
The initial fixture incorrectly rejects a diagonal target in the60-degree
profile. ROOT preserves that failed run and reviews a fresh sibling changing
only this independent expected answer; every other scenario is analytically
checked. All four corrected runs pass696 cases/6571 checks with empty stderr.
Resultb6632f0b66a9f648f81608de5ee03ee931bf74bc6c6e42d23857ccb16ed8d712.
No callback-mutation witness, gameplay bug, private ABI or full-engine claim.

ROOT integrates only the post-NuVecNorm typed set->unknown reload. The complete
source owner hash is73b283c709fa00bf87440e681952ab823e8ce22e874bf7b82481d59b5861f78e.
Android/native/WASM builds and all five repository checks pass. Independent
linked checkpoint49 is68.827390%, +0.000760pp over checkpoint47 and+0.824930pp
over main. Only the target48.241478%→50.355114% and completesfx
99.433334%→99.933334% change. All6293 raw exact identities, original identity
multiset, ownership summary and4722419-byte denominator retain. Raw report
29aa82efc7d75ec74f87c55021ae8314b8c49bb8ba679701c45476c61e30128a;
mapped report2fa5903d4739a0b8912a54460c445fb2aa6d4e651667f3be6f4e8b5f1be74711.
New-head CI is not yet claimed;70.002460% remains the merge threshold.

### Batch469 scalar Squish contract — isolated positive, not integrated

The original Compress3 and Compress4 bodies independently confirm float
subtraction-based Vec4 clamps and double floor/ceil arguments with narrowing.
The fixed header candidate retains version1.10, scalar configuration, O3,
class/algorithm/loop structure and every other input. ROOT repairs two PREP
packaging issues before execution (EOF preservation and frozen action copy),
then reviews both actual preprocessed streams and all145 consumed inputs.
The unsuffixed Squish -iquote argument resolves to the cache, but all ten
actually consumed Squish inputs remain the exact staged local files; this
path-resolution limitation is recorded, not concealed or retried.
The sole object pair and two original reports yield known+6542.06775762B:
Compress3 26.444748%→99.733970%,3627→4395B; Compress4
33.527912%→99.696600%,4408→5019B. All five backed names and one exact retain.
Capture931239b2d5b06120d5ed0a7314aa067299d1ab4f27bb5ed9ab0eea17ded24ee9.
Strict Counter/U checks stop on the expected floorf/ceilf→floor/ceil and
explicit coefficient-storage changes. Original-backed service/literal/thunk
qualification and finite codec diagnostics are pending. No source integration,
unknown-score imputation or final compressed-output divergence is claimed.

### Batches469–472: bounded scalar codec follow-up

Checkpoint49 commit7459cf36b39d9808dcbdf77e142f6dff85b6f663 subsequently
passes all11 exact-head GitHub checks. Main remainsa496c28; the linked merge
threshold remains70.002460%, not the sum of isolated gains below.

ROOT qualifies the same469 objects/reports, all24 original double floor/ceil
call windows and every changed literal consumer. The thunk, retained bodies,
CFI, COMDAT, vtable and full storage remain explicitly covered; unknown scores
stay None. The first metadata proof has an incorrect string-table boundary;
its failed log is preserved and an additive r1 changes only15→16. Successful
receipt1f4c07521e6cd554c21df8e6ec52a165451c0743f9f1c4a42a155ac8b4d088e4.
Both first runtime preflights stop before objects on ten uncataloged GNU
headers. An additive r2 pins exactly those ten observed identities, retaining
all original159 pins, fixture, helpers and options. Four GNU64/i386 O3/SSE
ASan/UBSan/LSan runs pass97 cases/9736 checks each with empty stderr.
Result82229c6e38f292f9d283b05cf9a649a64f71f23716ffe245e8fcb284c2e703c2.
The canonical finite black/white producer demonstrates only the original-backed
singular RGB intermediate clamp; no final compressed-byte divergence is claimed.

Batch470 changes only residue scale100.0f and integer entropy multiplication.
The sole res0 owner pair yields known+499.94170834B, res1_class
18.519773%→99.943504%,623→614B; all16 backed names/two exacts retain.
Specific literal/registry/thunk/full-object receipt
4e4d9cc0ad908fb06c535caaeae8fed9e7a750f272b3698b17ce3634e6af7d47.
The first runtime admission rejects a mismatched batch spelling before compiler
metadata. Fresh r1 corrects only that exact schema assertion. Four actual
allocator/ripcord/canonical-type diagnostics pass138 cases/18219 checks each,
including two finite classification witnesses, with empty sanitizer stderr.
Resultd33f974d6dba0368ebd17eae00b6b69f16d001988c0db1c40045b810b8cb5a4a.
These are residue classification/storage tests, not audio or gameplay fixtures.

Batch471 changes only six Vec3 subtraction-based Min/Max selects. The sole
RangeFit owner pair yields known+391.96197162B under unchanged C1/C2 alias
weighting:33.407185%→41.858284%,1529→1857B. Both near-exact Compress siblings
retain. The literal Counter difference is a three-slot1.0/31.5/63.5 payload
permutation, not LC10→LC7 payload equality. The first proof omits two cold-load
REL operands; additive r1 restores the exact complete consumer vectors without
weakening them. Receipt
a565e5467f7510196541de59ac3d6c432c7889dcdde2edb5e6597ef52ef82a74.
Four actual public RangeFit diagnostics pass100 cases/14719 checks each,
including count0..16, independent packing/decode and known endpoint blocks.
Result5c0f70eed2bfa3044c79e54a257235f0cb10eb9887fef4fff814de41a11e3a6c.
No private-state shim, output-divergence or whole-codec certificate is claimed.

Batch472's sole two local float suffixes in psy_init yield
known+174.59368960B,92.304690%→99.531250%,2448→2416B; all18 backed names,
two exacts and full Counter/U retain. Capture
a168c86eadfbcc1a03167034272c4fa9fb154d724b9ec038f5b497308cdea4a1.
The first metadata proof incorrectly includes the changed target's own39
self-branches among retained incoming callers. Its failure remains preserved.
Additive r1 pins the target's complete40 control rows separately on each side;
all15 retained callers and16 PIC pairs still require exact equality. Full
storage, symbol surface, CFI and the unscored12-byte thunk remain covered.
Receipt44d1a5c0f596f4c58e7207d7f122d380616d67e58d8c5c0144cb5b7f16ef04ba.
Both arithmetic preflights stop before objects on one uncataloged GNU C++
fenv compatibility wrapper. Fresh r1 adds only that exact reviewed pin;
fixture, statements, catalogs and launcher stay byte-identical. Four GNU64/i386
O3/SSE ASan/UBSan/float-cast-overflow/LSan runs pass7000 cases/375214 checks
each, with731 finite arithmetic witnesses and empty stderr. The independent
bounded integer rounding oracle and fixed shipped coefficient/fraction catalog
do not certify actual audio or runtime frequency distribution. Existing
negative Bark shifts and the upper-row k17 access are unchanged; this candidate cannot certify
whole-function sanitizer portability.
Resultd219e8524158472ec00d80f2512d20e49eced6c708871bb8d5abae81f90956b0.
No new ABI attribute, optimization flag, codec version or algorithm is proposed.

A distinct full _ov_open1 audit finds no safe local candidate. Its97.905060%
body requests n+8 bytes in retail but writes4*n+8, with actual header producers
counting entries. The first ordinary n=1 case requests9B and writes12B.
Current typed calloc retains the correct requested extent and native long
width; allocator slack is not a portable contract. No unsafe reproduction,
candidate or compiler/runtime trial is admitted.

The separate full vorbis_synthesis_headerin audit also yields no safe local
candidate. Its98.850945% body requests n+4 bytes for each comment array in
retail but writes4*n bytes. Two admitted empty comments already request6B
and write8B per array. Current canonical typed allocations remain unchanged;
no underallocation, flag/version change or speculative codec repair is tried.

ROOT integrates the four candidates through two separate pinned-archive
patches; the existing Android Vorbis compatibility patch remains unchanged.
Squish hunks use zero context to preserve upstream whitespace without a
space-before-tab patch warning. The first Bazel repository refresh rejects
missing multi-file boundaries before compilation; adding explicit diff file
headers fixes only packaging. The Android build then passes and all four
fetched source hashes exactly equal the qualified candidates. Version, source
membership, ABI and optimization choices remain unchanged.

Independent linked checkpoint50 is68.989170%, +0.161780pp over checkpoint49
and+0.986710pp over unchanged main. Exactly six expected rows improve:
Compress3 26.390177%→99.948160%, Compress4 33.449028%→99.940540%,
RangeFit C1/C2 33.329340%→41.830338%, res1_class18.548023%→99.994350%,
and psy_init92.650390%→99.908200%. All6293 raw exact identities, the full
original identity multiset, ownership summary and4722419-byte denominator
retain. No other original row's score changes. Android/native/WASM builds
and all five repository checks pass; the normal pre-commit workflow is pending.
Raw report92f45aed29eddb301f1461b0b28d100b75e85100046e2f8e659d6f0aade60bdf;
mapped reportb1ab619e3d8c04e73d6e1443f95d8201079ad5c386c36bb89ac9bd62cc63871b.
New-head CI is not yet claimed. The70.002460% cycle threshold still prohibits
a merge below+2pp even if checks pass.

Checkpoint50 commit7678e4d9e8d1204e8cf40bc5b8be0357a70a246a passes the
normal complete pre-commit workflow: all five tests, native declaration
validation, Android/native/WASM clang-tidy, target build, zero missing symbols
and report generation. The committed mapped report exactly equals the
independent checkpoint50 report hash above. Human author and the required
Codex co-author trailer retain. Push succeeds; all11 exact-head GitHub checks
subsequently pass. A fresh main fetch still resolves toa496c28. The below+2pp
gain remains insufficient for merging.

The first full edppDrawTorus audit covers the1595-byte original, canonical
effect/key records, actual emission/pool producers, cursor caller and complete
immediate drawing service. Current lifetime, ordered age, first inclusive
segment, interpolation, scale and outgoing draw behavior are faithful.
There is no missing behavior candidate or measured trial. A separately scoped
original-backed structural comparison may examine the actual fixed-offset
interpolation branches; this does not authorize flags, attributes, fabricated
behavior or a blind source-shape sweep.

### Batch473: one selected-time source-form trial, rejected

The complete raw body shows dynamically indexed values but selected times
retained through all three interpolation tails. Current code instead reloads
times through indexed pointers. One ordinary helper-only candidate captures
both times in the same seven ordered inclusive comparisons, preserving first
match, value indices, arithmetic grouping, zero fallback and the shared helper
used elsewhere. No fabricated behavior difference is claimed.

ROOT retains the canonical checkpoint50 owner object/report rather than
recompiling a baseline. Fresh live action473 retains O2/ABI/argv/environment.
The sole canonical candidate build passes. An initial manual dependency path
lookup fails on an absent execroot external symlink; the corrected post-build
review covers the same162 actual dependency names. It is explicitly a
review-time receipt, not a pre-build pin assertion.

The sole linked report retains every original identity and all6293 exacts,
but yields net−13.88196337B: target22.061947%→22.359882% (+4.75206325B)
and cbPtlEmitMenu61.003510%→59.603508% (−18.63402662B). No other score
changes. ROOT rejects the candidate, restores the exact original source and
rebuilds to the byte-identical baseline owner object. No variant, report retry
or runtime fixture is executed; runtime PREP stops before any source is written.
Failed whole report9d66e4af5937d71e3987a2783db37a98c544712bb7ba3c9d1df9ed3d0335d163.

### Batch474: seven explicit PCA double boundaries, positive trial

The full1366-byte original ComputePrincipleComponent body independently
confirms double sqrt/atan2/pow/cos/sin boundaries with float local round-trips.
The pow exponent is widened float1/3, not double1/3. Polynomial coefficients,
grouping, branches, eigenvector helpers and current Vec3 selects remain
unchanged. One seven-line source candidate explicitly promotes only those
arguments, including direct double sqrt into theta's atan2. No global precision
flag, codec version, attribute, private helper ABI or algorithm changes.

ROOT retains the actual canonical checkpoint50 maths object, pins all138
actual consumed dependencies before the candidate, and applies the source
through the existing archive patch. The sole O3 canonical build passes;
the fetched complete maths.cpp exactly equals the frozen candidate. Every
dependency except that owner remains byte-identical. The sole linked report
changes only PCA85.914560%→99.924050%, known+191.3696334B, retaining every
original identity and all6293 raw exact matches. Covariance, private helper
and RangeFit caller scores do not change. This small gain is not a promised
constructor improvement or codec-byte/gameplay divergence.
Raw report6b9b77f415fe1b0865ec3164b7b92adf30e73b0f98a31fa97e4c8fd7f11a544f.
Specific service/literal/storage/thunk qualification passes on those same
canonical objects and existing reports; no isolated recompile or second
matching trial is introduced. Three original-backed bodies, the unscored
12-byte thunk, full storage/symbol/CFI/PIC roles and precise double services,
widened exponent and switch entries are covered. Receipt
a860c61c0be89bc377d9f3d32563a5ab78588ef04bc1a64550f1dc8e01a6e10b.

GNU64/i386 O3/SSE ASan/UBSan/float-cast-overflow/LSan diagnostics pass on
both complete old/new Squish sources:100 actual ColourSet-produced cases
and6196 checks per lane, including22 distinct-root and74 repeated-root
cases. Each version agrees bitwise with its own precision-boundary oracle;
this is not a general eigensolver, encoder-output or private-ABI certificate.
Runtime receiptf639ecdc10e1f4f89d0a204faebca0caa2cddb62ac9dc86617b21b3e4012acaa.
Final preflight reuses the reviewed finite169-header/108-backend catalog
without additions; all four consumed-input/link plans are reviewed before
objects and both build manifests before execution. Nonzero constant RGB is
not assumed to yield exact zero covariance after float centroid rounding;
the overbroad unexecuted draft assertion is narrowed before freeze.

Mapped checkpoint51 is68.993220%, +0.004050pp over checkpoint50 and
+0.990760pp over main. Ownership summary,4722419-byte denominator and all
6293 exact identities retain. Android/native/WASM builds and all five checks
pass; the normal commit workflow and new-head GitHub checks remain pending.
Mapped report9316d88f0ebc3bbb4fa95e32cadad1b2778434b76d0c9f36160c64f8ad678433.
The70.002460% merge threshold remains unchanged.

Checkpoint51 commit0b4b0cc65394155490cb9a3b23e884615d411fac passes the
complete normal hook, including all five tests, declaration validation,
three clang-tidy modes, target/symbol checks and report generation. The
committed report is byte-identical to the independently mapped report.
Human author and Codex attribution retain; push succeeds. New-head GitHub
checks are still pending at this receipt, not inherited from checkpoint50.

All11 exact-head checkpoint51 GitHub checks subsequently complete successfully.
The+0.990760pp cycle gain remains below+2pp, so the PR is not merged.

### Batch475: constructor double square-root boundary

The complete803-byte original ColourSet C1/C2 alias body promotes each
initialized float weight to double, computes SQRTSD (cold sqrt(double)),
then narrows into the same float array. One argument cast restores that
source-local boundary without changing producer admission, accumulation,
layout, ABI, version or options. Canonical O3 build passes; all138 actual
pre-build dependency pins retain except the exactly qualified source owner.

The sole linked report changes only C1/C2:90.316580%→99.969850%, keeping
all6293 exact identities. The existing alias-row report counts+155.0315162B;
the one physical constructor contributes77.5157581B. No report normalization
is changed. Full retained Remap/thunk, storage/literals/PIC/CFI and precise
sqrt service/target-alignment qualification passes. Receipt
498b6cfafebbdd9399f2bbe7284a9d22be88dc042754281e784b6d3bd2c6c93c.

Four GNU64/i386 O3/SSE ASan/UBSan/float-cast-overflow/LSan lanes pass,
each4160 actual constructor/Remap cases and282943 checks. Directed actual
producers cover every initialized q/256 weight (q1..4096); mask/transparency,
duplicate/first-record/remap and live-storage guards also pass. Both versions
agree with their own sqrt boundary oracle; observed precision differences
are zero in this catalog, not an invented codec-output witness or global
equivalence claim. Runtime receipt
9b74f6e21ca9e6082913249f60d880667e8c27c569382dbe05a8c1c11779d549.

Mapped checkpoint52 is68.996500%, +0.003280pp over checkpoint51 and
+0.994040pp over main. Full original identities, ownership summary and
4722419-byte denominator retain. Android/native/WASM and all five checks
pass. Normal commit workflow/new-head CI remain pending. Sole raw report
1ccc95773ac20fbe2a89d207f9c05c3fc50fd402989ca190a50792eff97e1181;
mapped1a8e952603e55423629ee5184fa29c706d045a06737f6f495042916ffbd736b4.

A complete11528-byte CompressAlphaDxt5 behavioral and emitted-structure
audit finds no grounded candidate. Current first-fit fixed16-pixel expansion
and second-fit16-iteration loop already match the original structures; manual
unrolling would not restore an observed omission. No trial is executed.

### Batch476: space free-fighter cursor rejected

One original-backed typed cursor recipe for ProcessSpaceLevel's96-slot
free-fighter search reduces the linked score:26.112532%→25.758312%.
Together with two collateral changes the net is−36.8120336 weighted bytes;
all6293 exact identities retain. The candidate is reverted and the canonical
owner object restores byte-identically to its pre-trial baseline. No variant,
runtime qualification or additional score report is attempted.

### Batch478: shop first-dispatch source form rejected

Replacing only the first shop-item switch with original-ordered byte tests
retains all branch bodies and the separate live price dispatch, but its sole
canonical trial reduces DrawSubItemMenu2D35.854256%→23.457432%.
Linked net is−464.50899528 weighted bytes, with all6293 exact identities
retained. The source is reverted without variants or runtime qualification.

### Batch480: footstep semantic unit measured, not integrated

The complete658-byte retail AddFootSteps body admits only forward/forward-
wrap directions from LOOPED and uses slots2/3 unless config0x10000 selects
all four. Canonical parser/timer producers support finite distinguishing
states; current extra reverse traversal and unconditional four slots are
recorded reconstruction debt, not declared faithful. The sole combined
portable correction reduces27.479769%→26.791908% (−4.52612538 weighted
bytes); no other linked row changes and all6293 exact identities retain.
It is reverted for this matching-only unit, with no variant/runtime claim.

### Batches477/479: result-score selection and pause predicates

FinishStatusPacket's732-byte retail body selects HUB zeros versus BonusScore
before its two OldBonusScore stores. Moving the existing copies into the
non-HUB else restores that ordinary source form, preserving final distinct
initialized i32[2] values and service order. The sole linked change is
25.191860%→28.616280% (+25.0667544 weighted bytes). Actual O3 owner review
retains all eight other bodies/relative relocations, storage/literals, public
surface and CFI instructions; only target size769→761, two subsequent
function coordinates, associated unwind coordinates and alignment gap change.
There is no concurrency or exact instruction-order equivalence claim.

SystemPauseCallback's741-byte retail body rejects unordered game_time at
both player gates, but admits unordered post-delay at the player1 gate just
as at player0. Three source predicates restore that precise policy; all
finite behavior/equalities and services retain. The sole linked score change
is38.375000%→38.613094% (+1.76427654 weighted bytes). Its complete9968-byte
owner ELF differs at only six target instruction bytes; every other byte,
including symbols/storage/literals/relocations/unwind/thunks, is unchanged.
No shipped NaN producer or end-to-end OS/audio runtime claim is made.

An initial dependency capture rejected three repeated header names before
recording input pins. The pause source/object were restored exactly; a fresh
baseline records178 unique actual inputs and their three repeated occurrences
before the unchanged candidate. No failed capture is claimed as authority.

Both canonical candidates pass Android/native/WASM and all five repository
tests. All6293 exact identities, ownership summary and4722419-byte denominator
retain. The existing integrated raw report is
3e2ecaa724c301e9c45405a78762c654c85196d61ffdd9bbecc403ebc11ca6a6;
metadata-only mapping3eb7d38c4c8c485db61356e190da3e9a68858bb24b0136656bfa9a4ea04ca43a.
Checkpoint52 candidate68.997060%, +0.003840pp over checkpoint51 and
+0.994600pp over main; commit workflow/new-head CI remain pending.

Checkpoint52 is committed as b438fd395f4ad9b338e4ba022055e23c8a26bf69;
all eleven GitHub checks pass for that exact head. It remains below this PR's
+2pp merge threshold.

### Batch481: voice destructor cached sentinel rejected

The complete630-byte D1/D2 alias body already includes its cleanup through
explicit and automatic destruction. One retail-backed typed end-sentinel
capture reduces both alias rows39.870370%→37.722220%, report net
−27.066690 weighted bytes (one physical body, −13.533345 bytes).
All6293 exact identities retain. The sole candidate is reverted; source and
canonical owner object restore byte-identically. No variant or runtime claim.

### Batches482/485/486: fixed candidates rejected

Teleport_Find's retail-backed carried table cursor and serviced-exit signed
count recipe reduces36.098038%→23.519608% (−99.369597 weighted bytes).
cbPtlCopyEffect's captured capacity/carried vacancy-slot cursor reduces
37.254333%→36.092487% (−7.41257748 weighted bytes). Each changes only its
target report row, retains all6293 exact identities and is reverted without
variants. Both source and canonical owner objects restore byte-identically.

PodRaceAUpdate's complete629-byte audit finds missing pacemaker updates,
non-sentinel mine-spawn thresholds, successful lap-distance increments and
the common final mine update. The actual original LOCAL16-byte float table
is verified, not synthetic. The sole combined portable restoration reduces
39.500000%→32.146152% (−46.25570392 weighted bytes), with no other report
row changed and all6293 exact identities retained. It is reverted for this
matching-only unit; these finite behavioral omissions remain explicit debt,
not a faithful closure. No variant or runtime certification follows.

### Batch483: verified editor axis correction, score-neutral

The complete635-byte retail float-property callback uses canonical leftY
at pad+0xa3 for coarse10*sensitivity, then rightY at+a1 for fine0.1*sensitivity.
Current source swapped those roles. Eight typed member references are corrected
without changing arithmetic, center128 overwrite policy, clamps, formatting,
reparse or virtual setter services. Existing real spline Step controls admit
finite distinguishing inputs; no executed editor/device session is claimed.

The unchanged O2 owner emits an equal228552-byte ELF differing at exactly
three fixed target field-displacement bytes. Every other ELF byte, including
services, REL, CFI, storage, literals and symbol binding, is identical. Retail
binding is WEAK; the already existing emitted GLOBAL binding remains unchanged.
The full linked report is byte-identical to the baseline, all6293 exact
identities retain, and net gain is zero. This ordinary behavior correction is
retained, not counted as a matching gain. Final target/native/WASM builds and
all five repository checks pass for the retained source state.

### Batch484: retail-backed interface-cleanup cursors

The full675-byte retail cleanup captures and carries nine canonical typed
collection pointers, retaining live count reads; its final PART pass instead
reloads/indexes the base. Only those nine portable cursor forms are restored.
All existing NULL guards, global-WORLD obstacle selection, collection order,
actual typed destruction and the final particle loop remain unchanged.
Admission requires initialized, stable contiguous pools with valid owned
interfaces and counts within capacity; malformed assets are not certified.

The sole changed linked row improves35.245193%→52.682693%, net
+117.703125 weighted bytes. All6293 exact identities retain. Actual O3 owner
review retains all50 other function names/48 physical bodies and their REL,
storage/literals, startup/thunks, symbol surface and padding. Target size
643→595 shifts20 subsequent entries/FDE coordinates by−48; its changed
stack-CFA and epilogue advance agree with actual emitted instructions.
ROOT independently corroborates retained body/REL and frozen object hashes.
Final Android/native/WASM builds and all five repository checks pass.

Checkpoint53 candidate68.999560%, +0.002500pp over checkpoint52 and
+0.997100pp over main. Existing raw report
b0bda82acb7619dc5fd4b858c70b47cdde3e79d541513a4ff1e30b9847273b41;
metadata-only map39ac7af1b89440586e9e856b8f0f4678492177feb50d7983361f6003e4d2c7f9.
No new score generation is used for this mapping; commit/new-head CI pending.

Checkpoint53 is committed as08ed7ca851565facef4784335dad753a4d251552.
All eleven GitHub checks pass for that exact head. The +2pp merge threshold
is not reached; green checks alone do not authorize this cycle's merge.

### Fresh finite closures and remaining producer prerequisites

Complete original/current audits of NetFtpManager::Reset(180B),
VaderC_Update(4536B), GameObjOwnsAnyCables(155B), and
SceneObjectHelper::GetNextObject(292B) identify no missing finite behavior.
Keep the surprising retail first-cable ownership predicate and inverse scene
filter/initial-NULL owned-list admission; intuitive rewrites would change
their contracts. These are source-only closures, not new matching trials or
full-engine runtime certifications. Prior negative mechanisms remain closed.

MechEdgeStopAddon's complete157B startup initializes the same six16B vector
values and real4B hash object. Packed versus scalar vector stores and hash
inlining are representation differences, not an omitted initialization.
supportall's146B startup has the same known vector/manager initialization;
its exported16B BigBuffer still lacks a canonical type/storage mapping.
A bounded whole-retail direct-reference census finds only the startup's two
zero stores, not a meaningful producer/consumer for its remaining words.
This does not exclude indirect or external users and does not permit a
placeholder, padding record, dummy constructor, or guessed owner move.

The next low-score history screen covers exactly physical ranks31–60 of212
original100–599B functions below40% in frozen checkpoint52. It is history
metadata only, not a body audit. Nine exact receipts remain unlocated;
152 later rows remain unscreened. An unlocated receipt is neither a faithful
closure nor automatic permission to retry a family's rejected mechanism.

eduiGetAnalougePadValue(264B) and AddVariableShotDebrisEffectMtx(575B) also
close without proposals: unsigned pad axes/thresholds/coefficient overwrite
and signed-angle matrix construction/real key-storage lifetime already agree.
These audits do not certify all upstream calibration, allocation or rendering.

CharScenes_AreaLoad's full400B audit finds a real retail scalar alias: its
lookup output overwrites the outer traversal index. The actual lookup writes
−1 on failure; naive restoration can restart/stall traversal. Real model
producers preserve prefixes, reuse entries and skip failed loads, so no
monotonic list-position invariant is established. Preserve the current safe
separate index. No patch, fixture-aligned domain or matching trial is proposed.

### Batch487: retail-backed legacy AI-path reset cursors

The original252B GameAILoad callback captures its typed path-system/path-array,
per-path connection count and connection cursor. Restore only that reset-loop
source form, retaining the live outer count and all four canonical flag stores.
Actual loader allocations establish stable disjoint records; zero connections
neither dereference nor increment the captured pointer. All services and the
existing positive tag-length/initialized string/supplied arena safeguards stay
unchanged. Retail's uninitialized local allocator cursor is not recreated;
this is not whole-original-trailer or shipped-asset equivalence.

The sole linked row improves10.519481%→58.558440%, +121.05817668 weighted
bytes, with all6293 exact identities retained. Actual O3 owner retains377
named function identities/634 named undefined symbols. Only the target body
and explicit callback/bound-call coordinate operands change; all actual
destinations retain. Its334→290B body changes alignment and shifts later
entries by−32. Storage changes are only22 verified callback addresses; literals,
groups, startup, other bodies and non-target CFI instructions retain. ROOT
independently reproduces the compact owner observations and all4178 bound
non-target control-edge identities. Android/native/WASM and five checks pass.

Existing linked report69.002120% SHA
cc3a6a769a102c6cd71d3b85420c70bb9780dd322808c502e7e9fe84dfad2ac8;
metadata-only mapped SHA
e838285009931902092540546cff0a751ebf6c782d9f91d260e9a33fe294e6a6.

### Batch488: lever output branch form rejected

One behavior-neutral retail-backed output1/2/0 branch order and explicit
output0 booleanization reduces19.522728%→9.590909% (−14.30181936 weighted
bytes). Only its144B report row changes, all6293 exact identities retain.
Revert the sole candidate; source and canonical owner object restore exactly.
Keep the current NULL safeguards and pure typed helper. No variant or
runtime claim follows.

### Batch489: restore the real recursive memory-buffer lock lifecycle

The complete313B sound memory-manager startup constructs the actual GLOBAL4B
NuSoundMemoryBuffer::s_cs with recursive mutex attributes and registers its
NuCriticalSection destructor. Current source instead used a plain static
pthread mutex. Use the existing canonical NuCriticalSection static type and
its Lock/Unlock methods, preserving instance layout and all manager algorithms.
Its actual constructor ignores the name argument; NULL invents no debug string.
Real Defragment/SwapSimilarBuffers paths acquire the buffer lock recursively;
the source-derived valid allocation witness is not claimed executed.

The sole linked row improves12.363636%→99.527275%, +272.82219007 weighted
bytes. All6293 exact identities retain. The actual O3 owner's entire5676B
ordinary text and266 resolved relocations remain identical. Existing main
FDEs retain; only startup and naturally emitted/removed compiler-helper FDEs
change. s_cs remains4B. Six local vector records pack12B earlier and five
packed vector literals become one scalar1.0 literal plus zero/immediate stores,
preserving all actual values. Only this real initializer gains lifecycle work.

The added34B weak D1/D2 destructor aliases match the canonical linked provider;
an unused cx thunk disappears while bx retains. These are ordinary header
emissions, not fabricated helpers or attributes. All69 actual dependencies
were pinned before the patch/build; only the intended CPP/HPP contents change.
The extra weak aliases move two metadata assignments to ambiguous ownership:
12663 assigned,258 ambiguous,533 unassigned; original row identities retain.

A24-line Linux diagnostic using the unchanged actual NuCriticalSection header
confirms nested trylock succeeds for the canonical class and returns EBUSY for
a plain static mutex. This is not a Bionic, full Defragment, contention or
shutdown-order test. Final Android/native/WASM and all five repository tests
pass for the retained state.

Checkpoint54 candidate69.007910%, +0.008350pp over checkpoint53 and
+1.005450pp over main. Existing raw report
2adb04625d36c583ea46cf8b3966622e6bbbb12992e199aad852084b0d039f5c;
metadata-only mapaff803bdddbc57796995797bb386db66289838602a42356eb327ce28cf9841be.
No new scoring experiment is used for this map; commit/new-head CI pending.

## Checkpoint55: static critical-section lifetimes and voice-list endpoint

Checkpoint54 commit5d1cdbdd has all11 GitHub checks SUCCESS at its exact head.
Four subsequent single-recipe Android trials retain all6293 exact identities:

| Batch / original function | Before% | After% | Original-weighted gain B |
| --- | ---: | ---: | ---: |
| 490 / voice startup325B | 15.508474 | 99.559320 | 273.16524950 |
| 491 / sample startup313B | 41.745453 | 99.527275 | 180.85710286 |
| 492 / Android input startup313B | 12.363636 | 99.527275 | 272.82219007 |
| 493 / StopAllVoices109B | 19.268293 | 53.390244 | 37.19292659 |

The first three restore original-backed static recursive construction and
registered destruction using the existing NuCriticalSection, not handwritten
startup code or a new provider. Voice constructs state then release with the
two exact original names; it adds the missing4B release static but invents no
release locking. Sample uses NULL because the actual constructor ignores its
name argument. Android input likewise uses NULL and preserves all ten direct
pthread boundaries through the existing offset-zero mutex member. No instance
layout, owner, optimization, signatures, flags or scorer changes are made.

ROOT independently compares the entire ordinary voice15329B/sample1633B/
input2874B text and their271/80/97 resolved relocations: byte-identical.
The full emitted startup bodies were read, including destructor registration.
Compact owner reviews explicitly enumerate canonical weak ctor/destructor
emissions, U services, literal/vector packing and CFI changes. Sample's BSS
and vtable remain exact; voice/input vector packing changes preserve all six
values. No arbitrary literal, label, COMDAT or unwind exemption is used.

Canonical before-build dependencies are pinned separately: voice87 paths
(85 unchanged), sample68 (66 unchanged), input136 (135 unchanged), system91
(90 unchanged). Later intentional header edits are not substituted for those
captured candidate-build inputs. All four final Android objects retain their
immutable candidate hashes after the host configuration round trips.

StopAllVoices captures the initialized list's stable endpoint once, retaining
Front, Stop(true), and the next-link read AFTER Stop. Empty and multiple-voice
traversal have the same admitted order. This is an original-backed source form,
not a claimed behavior bug; it does not retry the rejected shared GetLinks
null-conversion family or expose private Links. Target80B becomes79B; custom
subclass reentrancy, corrupt/dangling lists and the existing host64 biased
sentinel debt are not certified.

Android/native/WASM builds and all five repository tests pass for the retained
combined state. These are build/diagnostic checks, not device audio/input,
sample reentrancy, contention, gameplay or shutdown-thread-quiescence tests.
The canonical mutex provider is unchanged; no artificial engine harness is
added to manufacture a threading witness.

Existing raw whole report
e5ec5ff41b7092e563d5fa3a96cf3c9753c4aadd30c88ae0c671b0fb81cced42;
metadata-only map412fe7a63b1603c5bdb7a3e861103fefc22b543d711c077cb419d9a5cf235acd.
Mapped counts12661 assigned/260 ambiguous/533 unassigned reflect two further
naturally duplicated weak ctor aliases, not removed original identities.
Candidate69.024080%, +0.016170pp over checkpoint54, +1.021620pp over main;
still below the70.002460% merge threshold. New-head CI remains pending.

History triage must name its selection universe: this fixed CP52 band has237
mapped-only600..3999B rows below40%, or289 when ambiguous/unassigned rows
are included. Rank numbers from these universes are not interchangeable.
Coverage is reconciled by original physical identity; numeric zero for an
unpaired private compiler clone is not a certified current counterpart score.
Unlocated receipts remain unknown, not automatically fresh or faithful.
The original1967B Nu3D initializer now has a full census; its split current
startup pairing remains unknown. A genuine texture-lock lifecycle fragment
is nominated but not applied, reassembled or credited with matching gain.

## Checkpoint56: pool/file/handle lifetimes and warning-color restoration

Checkpoint55 head caf7771b has all 11 GitHub checks SUCCESS. Four fixed
Android trials retain all 6,293 exact functions and the complete original
identity set, without changing ABI, owners, options or scoring:

| Batch / original function | Before % | After % | Original-weighted gain B |
| --- | ---: | ---: | ---: |
| 494 / pool startup, 313 B | 0 | 51.618183 | 161.56491279 |
| 495 / handle startup, 313 B | 41.745453 | 99.527275 | 180.85710286 |
| 496 / file-device startup, 313 B | 0 | 51.618183 | 161.56491279 |
| 497 / DrawPanel, 12,845 B | 54.664720 | 54.667225 | 0.32176725 |

The first three restore named original recursive-mutex construction and
registered destruction through the existing NuCriticalSection. Pool and
file-device headers only forward-declare the class; its definition is included
in their CPPs. Handle already obtains the canonical definition. All existing
direct pthread boundaries use its offset-zero mutex member; instance layouts,
instance mutex lifetimes and ordinary method code remain unchanged.

Full ordinary pool/handle/file-device text (3,718/2,617/3,518 B) and resolved
references are independently byte-exact. Compact owner receipts account for
all old functions/storage, literal/vtable/group surfaces, added weak destructor
aliases, U services and concrete CFI allocation. Handle's sole startup-only
cx thunk disappears with its exact single code consumer; no unrelated callable
is removed. Its emitted 313 B startup reproduces all original vector/lifetime
roles. Pool/file-device emit only the 108 B lifetime fragment: their original
six private vector records and prefix remain unreconstructed, not fabricated.

DrawPanel restores player 0 blue (63,127,255), player 1 green (0,255,0), as
proved by original indices/stack RGB arguments, actual SmartTextEx forwarding
and the earlier accepted fixture. Git history 2a57d7b7 introduced the first
call's reversal; the second accepted correction also was not retained. This is
restoration drift, not another rejected expression variant. The entire 58,724 B
object differs at precisely six immediate low bytes; every other byte, symbol,
relocation, storage shape, literal, CFI record and padding byte is identical.

Before-build actual dependencies are pinned separately: pool 50 (48 unchanged),
handle 83 (81), file-device 57 (55), panel 188 (187). Added nuthread.h dependencies
for pool/file-device are explicit post-build inventories, not retroactive pins.
Android/native/WASM builds and all five repository tests pass; all four Android
objects retain their frozen candidate hashes after configuration round trips.
These checks do not certify device audio/IO, concurrency or shutdown ordering.

The distinct sound-loader lifetime recipe 498 restores an original-backed
recursive lifecycle but lowers its composite 721 B startup from 51.271427 to
50.892857 (-2.72948970 weighted B). All exacts survive; nevertheless the entire
three-line recipe is reverted to its exact source/object baseline. It remains
recorded reconstruction debt, with no runtime, source-order or subset retry.

Raw retained whole report
f29593f82b9f4fe2b6c5dafea7512b3c3cb61deb096174af56cb768c98220e25;
metadata-only map 76a273c038b898b2116a2af4161ea23cad19f4d54388f8d23964019ef30fdd82.
Counts: 452 units, 12,663 assigned / 260 ambiguous / 531 unassigned rows;
denominator 4,722,419 B. Two newly emitted startup counterparts move out of
unassigned status without changing original identities. Candidate 69.034750%,
+0.010670 pp over checkpoint55, +1.032290 pp over main; still below the
70.002460% merge threshold. Commit/new-head checks remain pending.

The fixed CP55 large-low selection has 57 finite physical bodies below 40%,
size >=4,000 B (55 assigned, two unassigned). Exact history reconciliation is
complete for those 55 assigned bodies, not a universal fidelity claim. A new
complete Hub_Update census finds no finite-domain omission; focused historical
mock replacement tests are not evidence of actual helper mutations. Separate
RenderGraph six-word callback-contract recovery is still source-only review.

### Batch499 — recover the stored six-word editor render callback contract

The original LOCAL C `eduicbRenderGraph` (raw0x392d60/4706B) reads selection
from fixed-frame argument6 at0x3934b0, separately from argument5 width.
The complete original GLOBAL dispatcher670B and actual menu call pass six
words; the graph creator stores this callback at item+0x40. The canonical
stored type previously had only five arguments and required an incompatible
cast at dispatch. A valid unselected positive-width graph therefore selected
the bright cursor colour incorrectly.

One coherent unit adds the sixth argument to the shared stored type and all19
providers, calls it directly, forwards it through Filter->Prop, and makes Graph
use `selected`. The17 other providers accept an unnamed ignored argument:
this reconstructs the uniform stored contract, not proof of their original
unused formal arities. Existing LOCAL linkage, attributes, allocation/layout,
scalar1.25f, signed division, query order and safety policy remain unchanged.
The earlier scalar/signed-half negative recipe remains closed.

Fresh actual O2 action921cba72e06aa98ad31d3ec3c36d706817b080a3381ec44eaaec4926f28a9331
pins162 consumed inputs before the patch; only CPP/HPP change and the complete
dependency paths retain. Baseline/candidate338252B ELF objects differ in
exactly one byte: Graph's CMP displacement18->1c (physical0x29f16). All1800
symbol rows,578 FUNC identities/extents,7986 REL records,577 other bodies,
complete storage/literals/groups/gaps/startup/CFI are byte-identical. Filter
retains its5B tail jump and dispatcher its643B six-word call.

Raw whole report2f2d778a6d01e17b109e425652529a4be2c3191df848948d48ea29804dc5ad89
changes only Graph36.991703%->36.992737%, +0.04866004 weightedB; all original
rows and6293 exacts retain, whole69.034750% rounded. Android/native/WASM
builds and all five repository checks pass; restored target object retains
candidate SHA919e73ac458080cf68a26f5f8bf07939883913c23254238b6ab2bfe430ea311f.
These are diagnostic builds, not executed graph rendering or engine-wide
runtime certification. Source/owner receipts:
`/tmp/saga-rendergraph-callback-cp55.P8hzHx/audit.md` and
`/tmp/saga-rendergraph499-owner.hw5QtO/audit.md`.

Checkpoint56 commit40aa5327eaf0fb767283cc4ba2e10a12d9f9ad8b now has all11
exact-head PR checks SUCCESS. It remains below the70.002460% merge threshold.
The bounded remaining named-mutex screen finds five objects with original
manual/lazy/per-slot lifetimes already present, not more static-RAII omissions.
DisplaySceneRndrSpecials432B has all finite-domain operations; original source
owner/optimization is still unproved, so no optimization override is made.

### Batches500–503 — sound lock identity and canonical vector contracts

500 changes only the two weak-pointer static declarations/default definitions
to the already-existing NuSoundCriticalSection, adding its canonical header.
The original callback startup319B registers that class's D1/D2 at31e660/34B,
not NuCriticalSection's eed50. Independent complete Buffer startup313B and
System constructor515B corroborate the existing one-mutex wrapper. Both old
classes already supplied recursive services; this is a score-neutral registered
class-identity repair, not a new behavior or runtime/concurrency fix. The original
out-of-line NuCriticalSection constructor boundary remains unrecovered.

Fresh500 O3 actionb6709e226b357d6612e2e0edb6cdb5025363017adf91ab6c2e4207c589e8a218
pins48 old consumed inputs. The new sync-header dependency is explicitly
post-build discovery, not retroactive baseline admission. Complete owner
qualification retains18 ordinary physical text sections/25 common alias rows,
12 U rows,8B lock BSS,three vtables,19 ordinary COMDATs and18 ordinary FDEs.
Natural lifetime deltas are fully inventoried: old88B constructor aliases become
the existing94B LOCAL recursive helper, destructor class identity changes with
identical34B code/REL/CFI, startup114->98B, ignored41B name literal disappears.
All linked scores/original rows/6293 exacts retain. Raw neutral report
6b625eaa835f15e46a51bd1bc13ac622b2840e9e2414301aaca1d994aae72652;
receipt `/tmp/saga-batch500-owner-qualification.Kix51E/README.md`.

501 restores canonical NuVector data/capacity/length order, constructor order,
clear-length/free-buffer/clear-capacity-and-data destruction, and four-record
rounded growth. Entire original SetSampleTable580B and actual worker621B prove
capacity+4/length+8, not the previous reversed fields. Both original114B
destructors share that clear/free/clear contract. Scalar delete already reaches
the same BlockFree heap: no wrong-heap or different-free-flags claim is made.
Current instantiation is only the trivial32B filename record. Actual flag0x41
preserves old storage on successful relocation, qualifying the existing copy/free
sequence without changing allocator flags or fabricating callback mutations.

Fresh501 O3 action0c4ab84409c5e2e00f1a36e67bf055e6e96c4edb8b2fd64c4a8dd75098751de0
pins171 consumed inputs. Both D1/D2 identities28.857143%->100% share ONE
physical114B body; worker66.825584%->66.912790%. SetSampleTable regresses
43.967106%->21.519737%; this collateral is retained explicitly, not hidden.
Whole net +32.55252302 weightedB,69.035446%,6295 exact identities; all previous
6293 retain. Raw reportef400c3c5045e9dd9ed412aaf3c48b2390c87c58e860e7c0908ed2612cd9875a.

502's sole five-cast MoveGameCamera unit explicitly signed-truncates before
u16 wrapping, supported by real negative look/judder producers and original
CVTTSS2SI/MOVZX instructions. It regresses45.297882%->44.898552%,
-94.02224850 weightedB, with exacts retained; all five edits are reverted
byte-for-byte and the original canonical object hash restored. This genuine
signed-conversion source defect remains unresolved, not a fidelity certificate.
No subset/alternate conversion recipe follows. Immutable negative report
06bb57a0c35006e7c0105d23683daf63bc66616c77fa938601b084a3ba4430e8.

503 is a separate ordinary typed record-copy reconstruction, not a501 layout/
growth subset retry. The complete original580B producer copies records in an
ascending typed loop. One generic assignment loop replaces only memmove, with
the same real successful/disjoint allocation and sole trivial-record admission;
no eight-field manual expansion, forced unroll, flags or attributes. Native
member values are preserved, not a promise to preserve host padding bytes.
SetSampleTable21.519737%->38.552630%, +98.79077940 weightedB. It remains
below its pre501 score; the overall retained result is positive. All original
rows/6295 exact identities retain, whole69.037540%, +1.035080 pp over main.
Raw reporte3c975a050891874cc23d035990ca19b7010209ebf6c5c68bd6b83241aeb48a9;
source receipt `/tmp/saga-nuvector-typed-copy.wgUFEl/audit.md`.

New bounded UpdateGameObjects full raw/current audit finds no finite omission;
GameAIProcess complete current/reference plus focused raw/helper review also
finds none, but does not certify every original instruction or transitive helper.
All admission/safety/allocator-lifetime limits remain explicit. These receipts
do not convert low fuzzy scores into missing engine functionality claims.

Checkpoint57 final qualification includes both complete immutable owner censuses:
501 `/tmp/saga-nuvector501-owner-qualification.1bNUYQ/audit.md`
(SHA1e5eac5cf66fc98be36010bca40175153eaf0c6e50ef02323ddaa88463a4e74b),
503 `/tmp/saga-nuvector-copy503-objects.IX4Dl5/audit.md`
(SHAe0e2dcb22494ac272d064dc806e8709e7d533095344c1e21c617d3eef0450a0f).
ROOT independently checks both actual ELF inventories: 56 sections,224 symbols,
51 named FUNC rows each. 501 changes five named bodies (D1/D2 one physical
body),635->637 REL; 503 changes only SetSampleTable524->545B,637->634 REL.
The latter's50 other named bodies/relative REL, all85 U names and all storage
retain; memmove remains an owner U because another unchanged function uses it.
Its following alignment/function coordinate and target unwind changes are
explicitly accounted, not ignored. 501's worker scratch rotations have finite
ordinary-cdecl def/use accounting; no private register ABI is asserted.

Final Android/native/WASM builds and all five repository checks pass. After
the host configuration round trip, all three retained Android objects match
their frozen candidates; the rejected camera object's exact baseline hash also
retains. These checks do not certify running audio, rendering, concurrency,
overflow/allocation-failure handling or a general nontrivial-T container.
Metadata-only final map SHA
dd7a7918693a67ee146ade2c2f2f568cec7a4a500b96b28f6fa7f94db8e506d6:
452 units,12,663 assigned/260 ambiguous/531 unassigned rows,13,454 mapped
and13,459 raw rows,6295 exact identities,4,722,419 B denominator.
Retained69.037540% is +1.035080 pp over main;70.002460% remains the merge
threshold. This checkpoint's commit/new-head CI checks remain pending.

### Batch504 — restore two evidenced empty shader-table lifetimes

Checkpoint57 commit64fc052d7fcb55af8f843c5f25a5dec3b081b0ec is pushed;
all11 exact-head PR checks SUCCESS. Main remains
a496c28beece66a24ccdf7336571b6836f45e6aa; no merge below70.002460%.
The hook-generated checkpoint57 map exactly matches the separately reviewed
dd7a7918...506d6 map. Its ordinary editor formatting changes no object bytes.

The complete original shader startup377B constructs/registers named LOCAL
vertexShaders119db00/12B and pixelShaders119db0c/12B. Both registrations resolve
to the original114B NuVector<Pair<const unsigned char*,unsigned int>> D1/D2.
Its complete destructor clears length+8, conditionally frees data through the
real memory services, then clears capacity+4/data+0; no Pair element access.
The now-canonical shared NuVector supplies that lifecycle. One minimal unit
includes it, forward-declares the verified global two-type Pair template, and
adds exactly those two genuine default objects to the existing shader owner.
No guessed Pair fields/stride, fake vector prefix, source fusion, explicit
initializer or emission attribute. Unused dependent insertion/resize members
are not instantiated. Unknown original table consumers are not certified;
future element insertion requires independent canonical Pair reconstruction.

Fresh O3 action655d68a078ee62eec84bce1c37b5b9fb42421ce153cbdf0c1f0f3ef1bcd32c03
pins45 actual inputs BEFORE patch/build; only the owner changes. Added28
header/toolchain dependencies are explicitly AFTER-build discovery, not
retroactive pins. Baseline objectd8413805bd29089a3c1a1ae84b506d7e4e5b7946262dd4bc6580d8a3d70e6334;
candidate1c71fb6606fe3363a89b079a8114dc2875c685ac0a8c7ce9231a340bce59c400.
ROOT independently checks all six prior physical bodies and relative REL:
exact bytes retain. Natural helper identities map oldconstprop.0->new.1
(344B), oldconstprop.1->new.2 (394B), not a same-name/size blanket exemption.
The added D1/D2 names represent ONE physical114B body; new startup142B.

Whole report88421f72f962c23ebab5eac23f67b576c21e3ee93d9b9131b01343c2ade066d6
changes only startup0%->44.655174% and both original destructor identities
0%->100%; alias-inclusive +396.35000598 weightedB. All original rows/6295
exacts retain; now6297 exact identities and69.045920%, +1.043460 pp over main.
Metadata-only mape1967414a1e53bae23b84629227241eee5f1049b36da06b41155e3e3bd64b899
has452 units,12,666 assigned/260 ambiguous/528 unassigned rows, unchanged
13,454 mapped/13,459 raw rows and4,722,419 B denominator. Android/native/WASM
and all five repository tests pass; restored target candidate hash retains.
Source prerequisite `/tmp/saga-shader-pair-lifetime-prerequisite.PnPozg/audit.md`
SHA65cc5c6c3d66980e19958c064147de23194c50c92016763e4110902f2fc0e023.
Complete owner collateral receipt
`/tmp/saga-shader-pair504-objects.mSqaaB/audit.md`
SHAb11e7207644409a8e1371d8a3f91c10c272f84d13684f1d900892d3772a2f4b4
is read/accepted: all2193B ordinary text/padding and94 REL roles retain;
five old storage objects,30 old U identities, all six old FDEs/CIE, literals
and bx COMDAT retain. Exactly24B named vector BSS,142B startup,114B aliased
destructor, five grounded U names and their initialization/CFI metadata enter.
No GPU/Pair element/runtime claim.

The distinct seven-body history reconciliation and final52 small-low rows
use their existing immutable selections, not reranked scores. The latter
selection is CP52, not CP55/56; all first160 identities reconcile exactly.
GameCreatureOpponentSelection now has a complete bounded raw/current/helper
primary, with no new finite omission. Its integrated timer<=0 stays unchanged;
historical replacement diagnostics remain distinct from actual helper effects.

### Batch505 — correct the real 2D line-strip topology (score-neutral)

Full531B NuIOSDLGeom2DCallback original293ad6 and its signed jump table57b800
prove mode mapping[4,5,1,3,4], not current[4,5,1,2,4]. Real LineRect2di
requests type3 and emits four corners plus a repeated first vertex. One token
GL_LINE_LOOP->GL_LINE_STRIP restores the original four-segment topology; no
visible-pixel/GPU runtime difference is asserted. Producer header16/count+a,
vertex24, callback ABI and signed shader ID are already canonical. Existing
safe guards, helpers, packet storage and defaultO0 remain unchanged.
Source receipt `/tmp/saga-geom2d-cp57.K3kasZ/audit.md`
SHA47bd8cdb32a7a6fd5fe01da6c40c0cf5ea3fe5697585ce7d4770f3492e99afc3;
ROOT reads the complete raw target/table, current callback, actual primitive
producer and rectangle caller plus actual NDK enum definitions independently.

Fresh actual action61a791571d1139d74ed42a233466c645ed7fccce22b39726bd5c605a681c6fc5
pins130 consumed dependencies BEFORE patch/build,129 unchanged; dependency
paths retain exactly. Entire27956B owner ELF differs ONLY at physical12620:
02->03 in .rodata+748, actual kPrimModes+12. All188 symbol rows,595 REL
records, complete code/CFI/groups/literals/storage/gaps remain identical apart
from that approved20B table member. Baseline object
912b6941155fb8398fa4f080a6f6448a70ac3d8e8b705590317840599f2358b1;
candidate0e17220acb90a34af4aef06698e2f41f19ece188e52f80af08f92e118e241736.
Whole505 report is BYTE-IDENTICAL to504, SHA88421f72...de066d6: no score
change, all6297 exacts/original rows retain,69.045920%. Retained expressly as
a genuine original-backed topology correction, not numerical matching progress
or a neutral source-form retry. Android/native/WASM and all five repository
checks pass; target round trip preserves both504/505 candidate hashes.

Checkpoint58 retains only504's positive empty-table lifetime and505's neutral
original topology correction. Matching69.045920%, +0.008380 pp over57 and
+1.043460 pp over main; all6297 exacts and original identities retain. Final
reviewed metadata map remains e1967414...b64b899. Commit/new-head checks
pending; no merge below70.002460%. CRC_ProcessStringNIgnoreCase and
AILocatorSet_AssignFurthestLocator full bounded original/current reviews find
no new finite correction, preserving their explicit caller/asset-domain limits.

### Batch506 — canonical VuVec type-only recipe rejected whole

Checkpoint58 commit6df5e2a92be15cb3b1ca6088c73e692e5024af69 is pushed;
all11 exact-head checks SUCCESS, verified07:58 UTC. No merge below70.002460%.
The existing canonical VuVec and nucore.cpp's separate four-scalar definition
disagree. One fixed three-file recipe moved the existing class/asserts verbatim
into a type-only header, retained every existing header-static constant with
its old provider, and replaced only the private definition. No invented base,
alignment, static objects, body edits, flags or initializer ownership.
Source packet `/tmp/saga-dynamiclight-canonical-type-cp58.MiuX5R/type-only-review.md`
SHA84d1fdc7a642e852b5afff07238e375dd76b31d5808e63b4b1ad4d72b39d5b6b.

Fresh O2 action0b2ca32aa0c35be7dbde1638f4feade3d4e1484bd1e93e5612ac07dbfab71d64
pins125 actual dependencies BEFORE patch. Baseline owner object5350097c...13917;
candidate de9ba581...ce36. Separately frozen ordinary section/storage/CFI,
named-symbol, relocation and group fingerprints for all374 actual old-header
consumers remain identical; debug information is outside that fingerprint.
Whole506 SHA5d581ce5f4c80269958d1a9f9167d866b50c867e62f1a4acab6e518a8e27db08
retains all original identities/6297 exacts but changes seven scores, net
−228.34393652 weightedB:69.041100% versus69.045920%. Two blur gains do not
offset clipping/tap losses. FULL recipe reverted, including deleting the newly
created type-only header; exact old source/header and owner object hashes
restore. No positive subset, alternate class shape or scheduling retry retained.
The canonical type disagreement remains source debt, not a new missing block.

### Batch507 — real Message pool lifetime rejected whole

Original177B Message startup and registered47B D1/D2 prove global
TPool<TData<1200>,512>,512 records of1200-byte payload plus a u32 reference,
reference-only construction and forward conditional final reference clear.
The current nested MessageData array lacks that registered terminal lifecycle.
One complete bounded recipe reconstructs ONLY these explicit specializations,
the actual indexing interface, and the existing MessageData alias/global name.
No unused generic family, fake prefix, helper fanout, forced emission or second
element teardown. Real NetMessage/Ftp/CloneMessageData consumers were reviewed;
exhausted-pool, cursor, concurrency and shutdown-order safety are not certified.
Source receipt `/tmp/saga-startup-lifetimes-cp58.sI7yDC/proposal.md`
SHA851272c195a16eb1dd0dbb61ddafe18e038c06125884f46b0852b409c93f0cea.

Fresh O3 action56ef495f6a9a2458bffe238c5877b070fb76ff3259f5e55aee292ebda80a2778
pins23 owner inputs BEFORE patch; all321 actual header consumers have separately
captured ordinary-surface fingerprints. Baseline Message9d075746...e96d;
candidate0adfb07f...6bd9. Five provider surfaces change: Message,Ftp,Network,
game network and bolts. Whole507 SHA599c0d6e1cf6f0c11705cd8b36d0e17ac7031c771233a51af4e62abd74460e2a
retains all original identities/6297 exacts;22 scores change, net−110.56836756
weightedB:69.043620%. Startup0%->57.444443% and D1/D2 0%->78.333336%
do not offset consumer losses. FULL recipe reverted, no indexing/template
variant or positive subset. Exact old source/header/Message object restore;
all321 ordinary provider fingerprints independently return to baseline.

### Batch508 — canonical typed blowup midpoint link (score-neutral)

Original complete473B GunganA_Init publishes parent+0x50 into child+0x120;
complete95B GizBlowupObjectInterface::GetPos and the target finder consume
exactly three f32 coordinates. Canonical mid_position is already NUVEC; the
two real producers and both readers establish a borrowed midpoint pointer.
One three-line unit changes void* to NUVEC* and publishes &mid_position instead
of the compatibility byte-array view. No layout, lifetime, ABI, callback,
reader body, guard or numeric expression changes. Original nominal source
spelling is not inferred from machine code; no gameplay bug or gain claimed.
Source receipt `/tmp/saga-mech-record-cp58.eQn4PO/audit.md`
SHA387e7ea699b93a6d65a7e7193904db53cc253221c1c4c17a2185c2f2344b9e99.

Fresh O3 action8349346021402a2464c9099fc7c40494eab95d6c0bdf303294e7de3f5d921801
pins167 actual inputs BEFORE patch; only owner/header source changes. The
entire episode owner object remains byte-identical, SHA
c7acf2bab2b50b9ad395a51eea157e15cac13b00f5d3f67f303cbae0177304ed.
All306 actual header consumers' ordinary ELF/CFI/group/symbol/REL fingerprints
retain. Whole508 is BYTE-IDENTICAL to505, SHA88421f72...de066d6;
69.045920%, all6297 exacts/original identities retain. This is expressly a
neutral canonical type improvement, not numerical matching progress.
Native/WASM builds and all five repository checks PASS; restored target owner
hash and all306 ordinary provider fingerprints retain. Both rejected owners'
exact baseline hashes also retain after this configuration round trip.
Checkpoint59 keeps ONLY508's neutral canonical pointer declaration; no score
gain over58 and no rejected source variant retained. Commit/new-head checks
pending; main+2pp threshold unchanged. Shared editor cursor/creature storage,
cutscene trigger definitions and safe Ogg allocator wiring remain qualified
prerequisites, not local-overlay or allocation-failure shortcuts. In particular,
OggAllocMem's existing80B exact match prevents its needed safe null guard in
this retained-exacts pass; no unsafe header redirection is attempted.

## Checkpoint60: subsystem deletion and texture-lock lifetime

Checkpoint59 head455655473dc21971883c76112ff365a6ae91ee17 has all11
GitHub checks COMPLETED/SUCCESS, verified before batch509. No merge below
70.002460%. The following fixed units preserve compiler options and ownership;
no calling-convention attribute, forced clone or source-form search is used.

### Batch509 — externally owned editor subsystem storage

The complete original29B EdSubSystem D0 at55a610 resets its vptr and returns
without freeing storage. D1/D2 at55a5f0 physically alias another29B body.
Original global delete f0510/59B genuinely calls BlockFree, so the absence of
deallocation is a class storage contract, not an empty global allocator.
The real176B static SceneObjectHelper embeds the subsystem at+c; its original
startup registers D1 for finalization. RegisterSubSystem borrows the embedded
links. No production D0 call or valid delete-static-object witness is claimed.
One ordinary class-local no-op operator delete restores that callable policy;
the existing out-of-line destructor, virtual slots and binding remain intact.
Source receipt `/tmp/saga-edsubsystem-deleting-lifetime-cp58.HiDmlK/audit.md`
SHA9b4bb87673db9bd03ee7deebff358d3e41a2fcff24956a100dfe934ad7f6fc7e.

Fresh O2 action40f27c5281276cceefc0ea04d8be816191b4eb8d55220e14046019271b1ccca6
pins149 actual consumed inputs BEFORE patch. Baseline owner8614971c...89806;
candidatefe01a087...702bed. Of375 existing emitted functions,374 retain exact
body bytes, relative relocations and binding; only D0 changes56B->34B and
removes its global delete call. All274 other actual header-provider ordinary
ELF/storage/CFI/group/symbol/REL surfaces retain. The275 baseline objects were
also archived and byte-verified before mutation. Whole509 report
53ee373b73942d45bbb965a461268c74ed7d0398ecf442aa89e54633a54621a1
changes ONLY D0:0%->50%, +14.5 original-weightedB. All6297 exacts and original
identities retain. Remaining strong/weak and inlining differences are unforced.

### Batch510 — complete arcade-row migration rejected

One seven-file recipe replaces three named pointer/state groups with the
canonical pointer/i8/u8/u16 rows[3] and migrates EVERY real consumer. Target
ArcadeItem remains24B with8B rows, choice+4/count+5 and the same three static
labels/counts12/3/1. Native typed rows naturally grow to16B; current fixed
8-byte traversal is a genuine host-width defect. No invalid-pointer runtime
witness, corrupt queued-index safety or target gain is assumed. Frozen450
input/blip behavior and historical branch/ABI variants stay untouched.
Source receipt `/tmp/saga-arcade-shared-rows-cp58.2DvCOU/audit.md`
SHAe243302eae32cb6dac103048c0a6213960ea625c1620a59907219a2e1414323e.

Fresh six-action closure captures actual inputs and objects BEFORE mutation:
globals implicitO0, arcadeO2 and the four other ownersO3. All233 actual header
provider objects are byte-verified in a recoverable pretrial archive. Only
hub's ordinary provider surface changes. Whole510 report
6d5c349a57cf49b2b7d9c30820c989b893cac66e425d26c0e73e19fc98829810
retains6297 exacts and all original identities, but MenuUpdateBonusMode falls
51.873627%->51.653847%, net−5.978016B; every other score retains. FULL seven-file
recipe reverted to its exact source pins. Target restoration independently
returns all233 ordinary provider fingerprints to baseline. No subset or
alternate access recipe; the host-width prerequisite remains a negative reserve.

### Batch511 — restore the original texture mutex lifetime (neutral)

The previously frozen Nu3D startup census supplies the actual criticalSection
fragment: recursive mutexattr init/settype1/init/destroy and canonical
NuCriticalSection destruction registered against the real global. One exact
two-file recipe uses that existing class instead of PTHREAD_MUTEX_INITIALIZER;
the three existing direct lock/unlock calls address its mutex member. C-linkage
global name, target4B storage and every texture operation/guard remain intact.
No full1967B startup reassembly, private pairing or shutdown-thread witness.
Source receipt `/tmp/saga-nu3d-startup-full.ILAZ0y/audit.md`
SHA4e55c6821159f32519860054a5c0ad0125781cccbf16daa032a4ed240d383989.

Fresh actual actione083e7cd676485bba2d1c83089275e1a49ce6f8979cc9c1e0df9bf7c7e59c8c5
establishes O3 despite an older nomination's unresolved option observation;
all71 consumed old inputs pin BEFORE patch. Baseline942a2ff6...afa2;
candidatee2ebf262...86e9. All28 existing functions retain exact body bytes,
relative relocations and binding; only natural108B startup and canonical
34B weak D1/D2 aliases appear. Existing data/BSS/literal sections retain;
criticalSection remains GLOBAL4B BSS. All121 other actual header-provider
ordinary surfaces retain. Whole511 SHA
f3ef1629e8478c9a898d3f4b842c4ed77661982669b0f8f4e59c35810aca85f3
preserves EVERY original function score/identity and6297 exacts. Retained
expressly for original lifecycle fidelity, not numerical matching progress.

Combined matching69.046234%, +0.000314pp over59 and +1.043774pp over main.
Metadata-only mapb15b22e6d62bd52619cc29e4856009b3a0a3034ac03a9b70b994417d0e901925
retains452 units,12666 assigned/260 ambiguous/528 unassigned and4722419B
denominator. Native/WASM/Android builds and all five repository checks PASS;
both retained owner hashes and ordinary provider observations survive the
configuration round trip. Normal commit hook and exact new-head CI follow.
Full bounded hint and3196B shader-custom-setter audits locate all retail dataflow
without a new qualified correction; compiler-private pairing and host material
layout debt remain explicit. No neutral hint rename or forced ABI split is tried.

## Checkpoint61 — concrete editor free sizes and trigger storage predicate

Checkpoint60 head c6fd46bd7708efbd6f5e3a089cc27af80555933a passes all11
GitHub checks; the build workflow's exact head/status/conclusion independently
confirms COMPLETED/SUCCESS. Main remains a496c28beece66a24ccdf7336571b6836f45e6aa.
Neither this checkpoint nor60 reaches the70.002460% merge threshold.
Historical named /tmp primaries from earlier checkpoints are currently
unlocated; durable accepted/negative receipts remain authority. This does not
establish that their functions were never audited, or exhaust source forms.

### Batch512 — derived editor-control deallocation policies

Complete original Enum/Bit D0 bodies at55ac60/55aca0 pass20/24 to FreePool;
real placement-new producers allocate sizeof their concrete classes and
PropertyMenu::Destroy dispatches through EdControl*. Current derived deletes
instead inherit the base16-byte policy. All three sizes occupy target bucket0,
so this is not evidence of target corruption. Restore the two concrete-size
policies using sizeof, with the same inline-definition convention as the seven
existing sibling policies. No destructor body, weak attribute, virtual slot,
layout, source owner, optimization or emission variant is changed.
Source audit SHA076d4374bde16ee3a731814a238a9d1089f61df3c57d1af616aeb3be0d2db2a4
is preserved in the byte-verified ignored cp61-source-audits.tar.gz archive.

Fresh canonical O2 action pins149 consumed inputs before mutation and archives
all275 actual header-consuming objects, verified byte-for-byte. Of375 emitted
function identities, only Enum/Bit D0 bodies change; the other373 bodies,
extents, bindings and sections retain. All274 other consuming objects retain
their complete bytes. All59 ordinary non-CFI relocation sections normalize
identically; compiler CFI ordering/relocations change and are not represented
as raw-identical. Candidate owner SHA
d08be4d52258ddb4b33b390ad06f2375964e1997127c96e1c951d99ece595178.
Whole report50213f975229f6e9ecaeae98440829077bbfcfe814fa238241ef96f0e01ca979
changes ONLY the two63B D0 scores38.875%->38.9375%, net+0.07875B.
All13459 original rows and6297 exacts retain. This tiny positive is not a
meaningful overall percentage increase. Symbol coverage passes without an
ignore/baseline change; no production destruction fixture is claimed.

### Batch513 — AI trigger force-availability bit test

The complete3163B AITriggerSetSysProcess source-only audit identifies the raw
DWORD load/test at1e2cba..1e2cc5, whereas the current predicate uses floating
equality. Use the already canonical field_0x3c_bits union member. Actual typed
allocation/zeroing and reset produce +0.0; three real consumers already use
bit-zero. No actual negative-zero producer or gameplay failure is asserted.
All other branches, admission conditions, helpers and allocation remain intact.
Audit SHAc81ca5eefc06583d7a1d0fff5dbae8615fd3093770a8eee527c8554f2ded13e5,
full original raw SHAe6fbe550a7bdaa3041088a991fcf5b824f489dba8e499a508a15340298b3e959,
and the sole one-line recipe are preserved in the same verified source archive.
Fresh canonical O3 action pins147 consumed inputs before mutation. Existing
ten-function emitted inventory retains; only the nominated body changes.
Candidate owner SHAbf1b5fbaec865d50c0e86358b99b5f97964f84f8e02030aafe1832076cf1ada9.
Whole reported50a8d3d84eab27e8c9b608f0d5fc3ca5fa51c2a65bac6466de8b4c2f941d5e
changes ONLY this score40.538666%->41.057335%, net+16.40550047B;
all13459 rows and6297 exacts retain.

Combined matching69.046585%, +1.044125pp versus main. Metadata-only mapping
SHA18e7e6ac62f9584556fe3be4bd7bc330460f6de97ccd459460514ccd042405bb
retains452 units and the4722419B denominator. Android/native/WebAssembly
builds and all five repository checks pass; both candidate objects survive
the target configuration round trip. Normal commit hook and new exact-head
GitHub checks follow. No score is claimed for history-only screens or the
separate bounded Minicam_Update no-gap source audit.
