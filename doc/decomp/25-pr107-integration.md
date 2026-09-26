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
