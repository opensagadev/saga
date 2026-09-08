# Bounty mission, clip-player, and shop reconstruction

Reference: Android x86 `res/libTTapp.so`. Evidence was read with `nm`,
`objdump`, little-endian data reads, and `scripts/objdiff-cli.py`.
No Ghidra or forced-unlock path was used. This is a partial reconstruction;
the whole area is **not matched**.

## Recovered behavior

The clip catalog is configured from `cut\\clips.txt` during game startup.
The original static `CutScenePlayer_Accept` returns 1; the former stub returned
0 and the configuration call was commented out. The loaded catalog supplies
the shop's clip count and player pointer.

Each `CUTSCENEPLAYERCLIP` occupies `0x44` bytes: a signed level ID at +0,
clip type at +2, signed guest-episode ID at +3, and a 64-byte name at +4.
Types 0–3 mean intro, midtro, outro, and ending. The player occupies `0x20`
bytes. Its signed short at +0xa is a return **door index**, not a menu ID.
`ResetPlayerStarts` consumes it through `Door_FindByIndex`.

`CutScenePlayer_CanStart` checks story completion for ordinary areas and
`Episode_IsComplete` for bonus-area clips, returning 1 or 2 respectively.
It returns 0 for unavailable clips. The episode sentinel is signed -1.
There is no independent per-clip unlock bitmap in the save.

`CUTSCENEPLAYEROBJ` occupies `0x10` bytes. Original parser and playback code
access its flag byte at +0xc: show=1, hide=2, animation-end=4. The three
following bytes are padding. Playback prioritizes show, then hide, then
animation-end.

`Missions_PartyAvailable` checks `Collection_Got` for every configured party
character. The reconstructed `Hub_Update` branches use that result for the
Jabba doorway latch and lock models. Entering the mission interaction region
also requires an active, stationary player in the original character contexts.
The menu respects both players' ready state, supports selection and cancellation,
initializes the selected mission, and schedules its level in story mode.

Shop selection now calls the original clip availability and start routines and
preserves the shop return state. Character, hint, and extra item IDs are read
as unsigned shorts in `SelectShopItem`, as in the original. Purchase operations
use the existing save masks; they do not manufacture completion flags.

The shop reconstruction also covers controller input, category/submenu
transitions, code entry, shelf movement, item rendering, prompts, and purchase
feedback. `InitShop` builds the character-code range followed by 44 extra
codes, fills the 117-entry clip catalog, and initializes the original scales
and push distances. Animation interpolation uses a 32768-unit half-turn;
continuous object rotation uses a 65536-unit full turn.

The submenu's touch selection reads row at `MENU_s+0xa` and column at `+0x8`:
row 1 contains shelf items, with column 3 selected; row 2 contains confirmation
and cancellation. `DrawShopPanel` invokes the panel callback only while the
shop is active. Shelf spline counts are signed shorts captured before the
position arrays are cleared. The final direction calculation uses the
original point index equal to the stored count; it has not been changed to
count minus one.

## Matching checkpoint

### Counter entry and camera follow-up

The missing counter interaction was a chain of omitted original behavior:

| Owner | Original evidence | Recovered behavior |
|---|---|---|
| `TerrainPlayer` | `0x103a98..0x103ae8`, `0x1050b8` | Consume the ground probe through `GetSurfaceInfo`, including clearing floor material on a miss. Without this, the player retained the material from its spawn position. |
| `Hub_Update` | `0x1b80bd`, `0x1b8048`, `0x1b8690` | Detect a grounded, stationary active player on surface 14 in contexts -1, 49 or 50; respect the entry cooldown/latch; copy `Game` to `TempGame`, capture `MenuPacket`, and open menu 13. |
| `HubShopUnlocked` | `0x1ae1f0` | Returns true in the reference binary; counter entry does not need another save unlock. |
| `MoveGameCamera` | `0x10f486`, `0x110de5`, `0x111e15` | Select mode 8, use the shelf look target and original three sine offsets, and start a one-second blend on entry/exit. |
| Camera blend state | `0x1107a0`, `0x110818`, `0x111af3`, `0x112e28` | Capture destination vectors at camera offsets `0x158` and `0x188`, advance the blend, interpolate position and wrapped angles, then apply camera seeking. |
| `MenuInMemoryCard` | `0x428090`, data at `0x665a24` | Linux host link wrapper handles the original `MenuInfo[-1].id` padding read after `MenuReset`: return false without an out-of-bounds access. Shared game code and the matching target retain the original implementation (99.792%, relocation differences). |

This reconstructs the counter path, not the complete large hub, terrain or
camera dispatchers. Whole-function objdiff remains 0% for `Hub_Update` and
`MoveGameCamera`, and 4.507% for `TerrainPlayer` at this checkpoint. The
addressed branches were reviewed with objdump and objdiff; no Ghidra or
invented proximity trigger was used.

Native verification used an isolated copy of slot 2. The test moved the player
to `(-26.3, -49.5)` in the cantina's X/Z plane, then let normal terrain and hub
frames detect the surface, open menu 13 and complete camera mode 8's blend.
It did not set the surface ID or `SHOPACTIVE`. A queued menu cancellation ran
through normal menu updates; camera mode 1 returned, and the latch prevented
immediate re-entry while the player remained at the counter. The run also
reported an unresolved AI warning: `MovePlayer` passed infinite X/Z deltas to
`NuAtan2D`, producing an invalid lookup index. This does not constitute a
sanitizer-clean whole-game run.

Both `//src:saga_target` and native `//src:saga_native` built successfully;
all four `//scripts/checks:checks` tests passed after placing the padding-read
compatibility wrapper in the Linux platform layer.

These percentages are objdiff instruction similarity, not proof of complete
behavioral equivalence. Scores change when other work relocates symbols.
The near-100% entries explicitly marked below differ only in relocated addresses.
Per-source optimization settings were preserved.

| Function | Original address | Similarity | Remaining difference |
|---|---|---:|---|
| `CutScenePlayer_Accept` | `0x11a350` | 100% | None |
| `CutScenePlayer_Reset` | `0x49dd60` | 99.750% | Relocations only |
| `CutScenePlayer_Active` | `0x49dda0` | 99.833% | Relocations only |
| `CutScenePlayer_Available` | `0x49dd80` | 99.714% | Relocations only |
| `CutScenePlayer_Start` | `0x49df60` | 99.891% | Relocations only |
| `CutScenePlayer_CanStart` | `0x49de80` | 81.045% | Branches and register allocation |
| `CutScenePlayer_Configure` | `0x49d8b0` | 65.175% | Parser control flow and register allocation |
| `CutScenePlayer_CountEpisodeClips` | `0x49dbd0` | 27.607% | Original has separate loop variants |
| `CutScenePlayer_GetText` | `0x49e020` | 52.102% | Branch layout and operands |
| `CutScenePlayer_SetObjects` | `0x49e320` | 76.105% | Loop and branch layout |
| `CutScenePlayer_DrawGrid` | `0x1b2490` | 49.185% | Drawing and loop code |
| `MenuInitMissions` | `0x255ea0` | 99.158% | Loop comparison and relocations |
| `MenuUpdateMissions` | `0x256910` | 10.057% | Substantial instruction differences |
| `MenuDrawMissions` | `0x256020` | 82.114% | Drawing and prompt setup |
| `EndMissionsMenu` | `0x255e30` | 99.714% | Relocations only |
| `InitMission` | `0x2568c0` | 99.950% | Relocations only; existing reconstruction |
| `Missions_PartyAvailable` | `0x4e0e90` | 99.951% | Relocations only; existing reconstruction |
| `BuyShopItem` | `0x2469b0` | 99.875% | Relocations only |
| `SelectShopItem` | `0x246b10` | 58.520% | Control flow and register allocation |
| `SelectSubItem` | `0x247d00` | 92.658% | Epilogue sharing and padding |
| `Shop_GetInput` | `0x241ae0` | 97.458% | `regparm(1)` and `used` removed; GCC now produces the register argument naturally. Remaining register allocation, load-order and relocation differences. |
| `ItemMenu` | `0x241ee0` | 24.210% | Substantial instruction differences |
| `DrawItemMenu2D` | `0x2427a0` | 99.792% | Relocations only |
| `DrawCodeMenu` | `0x242910` | 99.780% | Relocations only |
| `CodeMenu` | `0x242a50` | 0% | Substantial control-flow differences; not matched |
| `DrawSubItemMenu2D` | `0x2436f0` | 23.843% | Substantial drawing/control-flow differences |
| `Shop_DrawCharacter.part.0` | `0x2445a0` | 91.603% | Register allocation; measured with compiler clone names paired as described below |
| `Shop_UpdateHint` | `0x244ad0` | 61.981% | Control flow and register allocation |
| `LoadShelfSplines` | `0x244d10` | 99.810% | Relocations only |
| `InitExtraList` | `0x245090` | 99.883% | Relocations only |
| `InitAlphaList` | `0x245250` | 99.940% | Relocations only |
| `UpdateCharacterIDs` | `0x2456c0` | 61.500% | Loop shape and register allocation |
| `InitShop` | `0x245710` | 76.058% | Initialization and loop code |
| `GetShopCamLookPos` | `0x246920` | 99.786% | Relocations only |
| `CheckCash` | `0x246980` | 99.833% | Relocations only |
| `MoveSubItemsRight` | `0x247050` | 95.178% | Interpolation/register differences |
| `MoveSubItemsLeft` | `0x2476a0` | 97.459% | Interpolation/register differences |
| `SubItemMenu` | `0x247e10` | 0% | Substantial control-flow differences; not matched |
| `DoShopMenu` | `0x248970` | 72.493% | Callback stack and branch layout |
| `UpdateShop` | `0x248ae0` | 74.409% | Initialization and control flow |
| `DrawItem` | `0x248d40` | 85.066% | Local ordering, branches and registers |
| `DrawCodeMenu3D` | `0x248e30` | 93.309% | Scale/angle calculation and registers |
| `DrawQuestion` | `0x249840` | 99.938% | Relocations only |
| `DrawArrow` | `0x249900` | 99.984% | Relocations only |
| `DrawTopShelf` | `0x2499f0` | 0% | Substantial drawing-code differences; not matched |
| `DrawSubItems` | `0x24a220` | 25.314% | Substantial drawing-code differences |
| `DrawSubItemMenu3D` | `0x24b9e0` | 99.875% | Relocations only |
| `DrawShop3D` | `0x24ba00` | 99.850% | Relocations only |
| `DrawShopPanel` | `0x24ba50` | 98.800% | Callback/global addressing |
| `DrawShopPrompts` | `0x24ba80` | 77.193% | Conditions and drawing setup |
| `Shop_CollectAllCharacters` | `0x24bda0` | 49.246% | Loop shape and register allocation |

The compiler emits `Shop_DrawCharacter` as `.part.3` in this build, versus
`.part.0` in the original. Its paired score was measured by copying the target
ELF to `/tmp`, renaming only that local symbol with `objcopy --redefine-sym`,
then running `objdiff-cli.py` on the copy. The production binary was not
modified. The unpaired report shows 0% because it cannot identify the clone.

Reproduce a comparison after building the target:

```sh
bazel build --config=target //src:saga_target
python3 scripts/objdiff-cli.py _Z20CutScenePlayer_Startii \
  -t bazel-bin/src/libTTapp.so --no-color
```

## Verification and remaining scope

Target and native builds and all four `//scripts/checks:checks` checks passed.
Native GDB diagnostics used an isolated scripted copy of SaveGame2, not the
interactive save directory. The checks established:

- 117 clips loaded, all available with the completed save.
- Clearing the first clip's story-completion byte makes it unavailable.
- Clearing a required bounty character makes the party unavailable; restoring
  the byte restores availability.
- Episode clip counts without/with guests are 17/19, 24/24, 19/19, 18/20,
  17/17, and 18/18. These agree with the loaded level and clip records.
- Mission-menu initialization, right-navigation, and confirmation select the
  corresponding mission and schedule its configured level.
- Mission navigation was rechecked using `GamePad.unknown_10`, the original
  alternate pressed-input word at +0x10.
- Shelf movement wraps and opposite movements restore all seven item IDs.
- Confirming a category opens its submenu; touching row 1/column 3 selects
  an item and row 2/column 1 cancels purchase confirmation.
- The real extra code `K0HOMF` sets purchased and unlocked save bits and
  supplies the displayed extra name. An invalid code sets neither state
  and does not charge coins.
- Owned, unaffordable, and red-brick-locked extras reject purchase without
  charging or setting purchase bits.

These are diagnostic function checks, not an end-to-end gameplay test.
`Hub_Update` remains a larger partial reconstruction. The shop submenu and
shelf routines now have reconstructed bodies, but the large functions above
remain unmatched. Clip playback through the complete shop UI is not
established. The two-player
prompt setup in `MenuDrawMissions` preserves an original defect: the both-active
branch does not initialize the second player's three prompt locals. The
reconstruction does not invent a second-player ready-state branch. Two-player
prompt rendering remains unverified and requires a separate bug decision;
the successful one-player diagnostic does not cover it.

## Shared hint UI dependency

`Hint_CancelCurrent` (`0x4e73f0`), `Hint_SetHint` (`0x4e77b0`), and
`Hint_SetHintFromId` (`0x4e7a60`) still have stub bodies. Their binary review
identified an initialization dependency: `initHintSys` (`0x4e7250`) must
construct a `MechHintUIButton` through `MechTouchUITexButton` and register it
with `MechTouchUI`. That constructor and initialization are also stubs;
`hintUIButton` therefore remains null. The original display/cancel routines
dereference it unconditionally. They must be reconstructed together with
the initialization and UI lifecycle, rather than enabled with invented
null guards or a dummy button.

Recovered runtime fields are now typed: hint price at +8, display duration
at +0xc, repeat delay at +0x10, display callback at +0x18, and the pending
hint pointer at button +0x7c. Hint flags include complete-on-display (1),
shop-purchased (2), and direct-display (8). These are runtime structures,
not additional serialized save fields. Cancellation advances the active
display timer to duration minus 0.5 when necessary and clears its repeat
timer. Direct-display hints invoke their callback; other hints queue the
button transition. A future reconstruction must also cover the button's
processing/rendering and `Hint_Process`, which remain unfinished.

Update this audit when the structures, recovered behavior, or matching status
change. The live source and a fresh objdiff run remain authoritative.

## Shop panel dispatch recovery (2026-09-08)

The missing category text had a missing caller: `Hub_DrawPanel`
(`0x1af380`, original size `0x28d7`) was empty. The original calls
`DrawShopPanel` at `0x1af3c2`, after its new-game map-title fade and before
episode, area, minikit-viewer, and build-progress panels. The recovered body
preserves that dispatch and reconstructs those surrounding branches. It does
not move the shop call into a different menu or renderer.

`DrawItemMenu2D` also used the wrong localization variable for category 2:
the original GOT reference at `0x2428e8` is `tEXTRAS`, not
`tHELPANDOPTIONS`. Its high instruction-match percentage did not establish
that the global reference was correct; the instrumented original-code
comparison exposed the different title.

| Function | Current match | Original/current bytes |
|---|---:|---:|
| `Hub_DrawPanel` | 30.886% | 10455 / 10788 |
| `Hub_DrawMiniKitCount` | 96.569% | 603 / 603 |
| `DrawItemMenu2D` | 99.792% | 358 / 358 |

The private counter helper now formats the count, selects the normal or
challenge-kit icon, clears the one-shot icon selector, and reproduces the
original scale and rotation arithmetic. Its calling convention is inferred
by the original compiler from real callers; there is no forced ABI or
optimization attribute.

Verification used mapped original and target x86 machine code, with rendering
outputs instrumented and identical initialized state:

- All 960 combinations of shop activity, six categories, title opacity,
  new-game camera activity, and fade-boundary times produced identical text
  calls, including position, scale, colour, and alpha.
- All 720 counter cases agreed on formatted text, selected icon, position,
  scale, rotations, and clearing the one-shot selector.
- A native capture entered the shop through the physical counter surface and
  displayed the `Characters` category title. The text callback ran 53 times
  for that title. ASan and UBSan were enabled.
- Two subsequent character-submenu capture attempts stopped before shop
  entry on the outstanding `MovePlayer` / `NuAtan2D` invalid-angle error
  (`nutrig.cpp:62`). These are failures, not successful submenu checks.

Target and native builds and all four repository checks passed. This is not
a claim that the entire shop or other hub panels are complete: several panel
leaf functions, including `Hub_DrawAreaStats`, `Hub_DrawImportantBrick`,
`Hub_DrawArcadeStats`, and `Hub_DrawSuperBonusStats`, remain stubs. The larger
panel function and shop submenu routines still need matching improvements
and broader runtime verification. No gizmo implementation was changed.
