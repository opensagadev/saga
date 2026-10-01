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
