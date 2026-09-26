# PR #107 integration and portability audit

Branch `fabus1184/implement` was rebased onto PR #107 commit
`6a6bb4d02ac7d3135dc148628057b2ca461d0015`. The reconstruction is retained,
but the imported instruction-forcing shortcuts are not part of the matching
contract: no handwritten inline assembly, register constraints, `regparm`,
or function-level optimization/inlining attributes added solely for scores.
Use ordinary C/C++ and the per-source Bazel optimization map instead.
Earlier experiment notes describing those shortcuts are historical results,
not recipes to copy. Linkage-only assembler labels for retail symbol names
remain distinct from executable inline assembly.

## Build repairs

- Empty assembly barriers and register constraints were removed. Swipe
  direction dispatch, plane-distance min/max, and clipping copies use C++.
- `DogFightARestart` resets all four `DogDebKey` integers with `memset`,
  without an SSE-only header. Its aligned data layout remains unchanged.
- `JumpTriggerPacket` offsets `+4` and `+8` contain `GameObject_s *` and
  `TouchHolder *`, not `u32` values. `DECOMP_ASSERT` checks the retail offsets;
  native pointers retain their full width.
- `LevelLocator` and `LevelCodeSpline` are pointer globals too. Episode VI
  no longer truncates their addresses on 64-bit Windows.
- `NuMemoryManager::DumpBlock` uses the existing `END_TAG` pointer type.
- Overlapping save-menu timer definitions were consolidated after rebase;
  the imported save translation unit retains its per-file `-O2` setting.
- Forward declarations for the shield/disco queries, creature spawning,
  special-list callback, and relocation callbacks agree with their definitions.
  The relocation callbacks at `0x316af4` and `0x316af9` are confirmed empty;
  their `STUBBED()` diagnostics were removed only after disassembly inspection.

## Retail evidence for behavior repairs

`MechInputTouchGestureBasedController::OnDoubleClick` stores the player and
holder at packet offsets `+4` and `+8` before `TriggerJumpTask`. For example,
retail instructions at `0x44f99f`/`0x44f9a7` and
`0x44fcc6`/`0x44fcd1` perform those stores. `OnSwipe` does the same at
`0x44f2d4`/`0x44f2e0`. The imported source omitted these assignments, leaving
null pointers that `TriggerJumpTask` immediately dereferences. All four
affected packet construction paths now supply both pointers.

The Death Star turret `OnSwipe` at `0x45f1d0` returns `1` for a consumed
swipe; otherwise it returns with the holder address still in EAX. Callers
test AL. The source explicitly preserves that low-byte truth value rather
than falling off a bool-returning function, which is undefined C++ behavior.
This is an unusual retail behavior, not a newly invented false-return guard.

`SetupBlowupSfx` (`0x4c7130`) and `DrawTorpedoTargetSprite` (`0x1f94f0`)
contain only NOP padding followed by RET. Their empty bodies are confirmed;
source-level NOP assembly is unnecessary.

## Continued low-match work

`Batarang_StartTargetting` does not scan for a nearest enemy. It resolves the
level object, uses `WORLD`, and initializes the sight and velocity in the
retail order. The field currently named `BATARANG_s::cooldown` is a signed
16-bit level-object index; the retail lookup sign-extends it. The correction
also removes null checks absent from the original.

`Technos_FindTgt` tests a missing techno object and an already-controlled
object separately, matching the original early-return control flow.

The linked results for those two functions are 92.678% and 84.366%,
respectively (both previously 0%).

`LightSabreDebris` improves from 0% to 66.187%. Its effect selection uses the
retail default/override flow; it validates the first blade joint before
loading the second, and declares the start/middle/end points in retail stack
order. It still emits the same three debris calls in that order, with no
new guards. The remaining difference is compiler control flow/register
allocation: 648 bytes versus the original 637. Experiments stayed in `/tmp`.

The next low-match pass restores ordinary C++ control flow in six functions,
without changing their per-file optimization settings:

- `oneAtOnce_GetHoldRange` (`0x17cb60`): the null-entry and index-limit tests
  belong to the loop condition, in that order. This keeps the original
  211-byte loop instead of unrolling it, and improves 0% to 99.594%.
- `Animate_CRITTER` (`0x171e40`): preserves the fall-model checks after the
  contact-grace and character-speed tests, and the nested fall/run/walk
  selection. The run threshold uses `>= ? WALK : RUN`, matching the retail
  comparison's unordered result too. It improves 0% to 79.000%.
- `ThingManager::DisplayThings`, `RenderThings`, and `ResetThings`
  (`0x4252c0`, `0x425390`, `0x425670`): counted loops retain the original
  null/skip checks, virtual-call slots, and profiling calls. Each improves
  0% to 56.397%; basic-block layout still differs.
- `Ledges_Draw` (`0x1df0d0`): restores the 16-byte-aligned local matrix and
  the count reload after drawing (`0x1df1ad`–`0x1df1b6`). Skipped entries
  use the cached count. It improves 0% to 72.507%.

These are linked-binary scores, not claims of full instruction matching.

`oneAtOnce_CanAttack` (`0x17cb10`) now matches 100% (previously 53%). Its
return type is `i32`, not `bool`: retail zero-extends the comparison into
EAX, and the three callers at `0x18b3d3`, `0x19b7b8`, and `0x19b8bd` test
the full register. Its unassigned-player case is a separate early return.
The definition and AI caller declaration now agree.

`UpdateCharacterLoad` also uses a 32-bit character index: the retail loop
increments ESI at `0x108ab3` and compares it with `CHARCOUNT` at `0x108abc`,
without narrowing it back to 16 bits. Only that verified type correction
was retained. Attempts to recover the store-pack loop stayed in `/tmp`;
the function remains at 0% and its existing `-O3` mode was not changed.

## Touch-holder array lifetime

`MechInputTouchGestureTrackingSystem` owns a real `TouchHolder[10]`, not a
byte buffer with manually invoked destructors. The retail constructor
(`0x500bc0`) initializes each holder's two managed references, then its
20 swipe samples, then its flags and timers before advancing by `0x3bc`.
The destructor (`0x500a40`) walks backwards from `this + 0x2588`, destroying
the previous-target reference before the target reference in each holder.
Ordinary member constructors and automatic array destruction now express
that order; the empty system-destructor body still performs member cleanup.

All accesses in this translation unit now use `holders` and `trackers`.
The former literal offsets `0x30`, `0x3bc`, and `0x2588` only describe the
32-bit layout and cannot be used to access the larger 64-bit objects.
`DECOMP_ASSERT` retains the retail layout checks without imposing those
offsets on native pointers. No guards or optimization overrides were added.

Linked-binary results for this unit:

- constructor: 38.325% to 53.373%; destructor: 0% to 56.122%
  (304 bytes versus retail's 312, instead of the old 2,262-byte unrolled body);
- `LookForDown` and `LookForRelease`: 93.662% / 93.634% to 100%;
- `LookForHold`: 92.238% to 99.993%;
- registration, unregistration, and touch lookup also improve;
- typed accesses change alias analysis and code generation in neighboring
  functions: clicks moves from 59.556% to 58.262%, swipe from 57.059% to
  47.770%, and data reading from 49.140% to 49.107%. These remain partial
  matches; the portable member accesses are retained.

Target, native, and WebAssembly builds pass, as does a Windows x64 syntax
check. An isolated ASan/UBSan test passes with both 32-bit and 64-bit
pointers: it checks all ten holders' initialized fields, array destruction
with twenty live managed references and one surviving reference, and
target destruction before holder destruction.
