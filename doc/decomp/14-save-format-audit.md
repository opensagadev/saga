# Save-format audit

Reference: Android x86 `res/libTTapp.so`, inspected with `nm`, `objdump`, raw
little-endian data reads, and `scripts/objdiff-cli.py`. No Ghidra result was used
for the changes below. Payload offsets in this document exclude the envelope.
The live property table is available with `save schema` / `save schema --options`;
see [host utilities](../host-utilities.md) for commands and editing syntax.

## Layout and evidence

`Game` occupies `0x7e58` bytes. The utility covers every file byte and checks
that its schema has neither gaps nor overlapping properties. This is a storage
inventory, not a claim that every reserved byte has a gameplay interpretation.

| Payload offset | Recovered storage | Original evidence |
|---|---|---|
| `0x0000` | Unidentified byte | Cleared by `NewGame`; no specific consumer established. |
| `0x0001` | Difficulty byte, initially 5 | `ResetAICreatures`, `ManageGameObjects`, `KillGameObject`, `ReleaseHearts`. This is not a format version. |
| `0x0002` | Two unidentified bytes | Cleared by `NewGame`. |
| `0x0004` | 13 option bytes | `InitGameBeforeConfig` (`0x11b880`), `MenuDrawOptions`, `MenuUpdateOptions`, audio/video option consumers. |
| `0x0011` | 84-byte level records, capacity 366 | `InitGameBeforeConfig` sets `Game_LevelSave` to this exact unaligned offset; `GizmoPickups_Reset`, `SuperCounter_AnyCollected`, `Arcade_AwardPoint` establish the stride. |
| `0x7829` | Three remaining level-storage bytes | Capacity inferred from the interval ending at `area_save`; not a claim that all 366 level IDs are configured. |
| `0x782c` | 72 area records, 12 bytes each | Area indexing throughout `NewGame`, `CollectAllMiniKits`, `ReCalculateCompletionPoints`, status-screen code. |
| `0x7b8c` | **Six** episode records, 12 bytes each | `NewGame` initializes six time/score pairs; Super Story status paths use this interval. The former nine-record declaration overlapped shop storage. |
| `0x7bd4` | Three shop-hint purchase masks | `InitShop` (`0x245710`) and `ReCalculateCompletionPoints`. Shop-list indices, not tutorial IDs. |
| `0x7be0` | Four shop-character purchase masks | `InitShop`, `Shop_CollectAllCharacters` (`0x24bda0`); runtime loop caps entries at 100. |
| `0x7bf0` | Two extra-unlock masks | `CodeMenu`, red-brick collection, `BuyAllShopExtras` (`0x11edc0`). |
| `0x7bf8` | Shop gold-brick purchase mask | `InitShop`; completion calculation scans 14 shop bricks. |
| `0x7bfc` | Shared-engine suit availability mask | `Suits_CollectAll` (`0x1cf4e0`), `Suit` data. Former `initial_store_pack_flags` name was misleading. |
| `0x7c00` | Two extra-purchase masks | `BuyAllShopExtras`, `InitShop`, completion calculation. Separate from unlocked and currently enabled extras. |
| `0x7c08` | Six tutorial-hint masks | `SET_HINT_COMPLETE` (`0x22ec50`), `HINT_COMPLETE` (`0x22ed10`), `CLEAR_HINT_COMPLETE`: two banks, each three words for IDs 0..95. |
| `0x7c20` | Unsigned stud balance | Score/coin routines; normal addition caps at 4,000,000,000. |
| `0x7c24` | 16-bit completion points | `AddToCompletionPoints` (`0x47a4f0`), `MakeSaveHash` (`0x1197c0`). Display percent depends on configured `COMPLETIONPOINTS`. |
| `0x7c26` | Gold-brick total byte | `AddToGoldBricks` (`0x47a5a0`). |
| `0x7c27` | Reward flags byte | Completion reward callbacks use bit 0 (100%) and bit 1 (all gold bricks). |
| `0x7c28` | Hub construction flags byte | `Hub_Update` indexes build entries; high bit is set from `LevGizmo` output. Index names are retained where asset meaning is unresolved. |
| `0x7c29` | Indiana Jones unlock marker | `IndyUnlocked_UpdateHint` and hub display. |
| `0x7c2a` | Two unidentified/alignment bytes | No field-specific consumer established. |
| `0x7c2c` | Gameplay seconds float | `GameTiming` (`0x4887e0`) and hub time display. |
| `0x7c30` | Two custom characters, second starts at `+0x38` | `Customiser_CopyDefaultPiecesToSave` (`0x4a1bd0`), `Customiser_GetIcon`, `GameObj_GetName` (`0x13d350`). Recovered prefix is 111 bytes, followed by one preserved byte. |
| `0x7ca0` | **Twenty** mission time floats | `MenuDrawMissions`, mission/status paths. |
| `0x7cf0` | **Twenty** mission completion bytes | Mission/status paths use `mission_save + 0x50 + mission_index`; total mission record is `0x64`, not `0x0c`. |
| `0x7d04` | 340 character flag bytes | `Collection_Got`, `CollectIDUnlocked` (`0x4dc560`), `AddToCollection`, `NewGame`. IDs come from configured character data. |

Each level record has ten 8-byte pickup names at `+0x00`, a count at `+0x50`,
two unidentified bytes at `+0x51`, and arcade wins at `+0x53`. Each area record
has availability, story completion, story/free-play stud-buildup completion,
**minikit-set completion at +4**, **minikit count at +5**, **red brick at +6**,
one unidentified/alignment byte at +7, and challenge time at +8.
`CollectAllMiniKits` writes 10 at +5 and 1 at +4; `BuyAllShopExtras` sets +6.

Custom characters have nine copied 16-bit piece indices, a two-byte unresolved
slot, a 32-byte name, and a stored-name selection byte at +0x34 (secondary
+0x6c). `GameObj_GetName` uses the saved name only when that byte is nonzero;
it is not a character-unlock flag. `FinishWeirdoNames` formats 15 display
characters. Remaining gaps are retained, without inventing piece/flag meanings.

## Options and enumerations

Per-game option offsets are relative to `options_save`: +0/+1 player rumble,
+2 surround sound, +3 sound volume, +4 music volume, +5 master volume,
+6 music enabled, +7/+8 initialized-zero unresolved bytes, +9/+10 unresolved
bytes, +11 widescreen, +12 brightness. Volume and brightness menu ranges are
0..10; the utility also permits storage-width values for forensic editing.

`SuperOptions` is 24 bytes: pack mask at +0, control selection at +2, D-pad
position lock at +3, four position floats at +4/+8/+12/+16, music toggle at +20,
bundle mask at +21, and two unresolved trailing bytes. The lock meaning comes
from `VirtualControlDPad_LockButton_OnClick_Callback` (`0x453a60`).

| Flag family | Confirmed values |
|---|---|
| Character | `AVAILABLE=1` (owned/playable), `UNLOCKED=2` (collection unlocked). |
| Reward | `100_PERCENT=1`, `ALL_GOLD_BRICKS=2`. |
| Arcade | `BATTLE=1`, `COLLECT=2`, `HUNT=4`; original `Arcade_Mode` order. |
| Episode | Low byte is completion status; canonical completion value 1. Upper bits remain preserved. |
| Store pack | Bits 0..10: Episode II, III, IV, V, VI, arcade, bonus, bounty, challenge, Jedi, Sith. |
| Store bundle | Bits 0..2: prequel, original trilogy, complete. |
| Suit | Bits 0..7: shadow, glide, demolition, sonar, water, technology, magnet, attract. Original collect-all writes all 32 bits. |
| Boolean/progress | `OFF/ON`, `INCOMPLETE/COMPLETE`, and `CHARACTER_DEFAULT/SAVED_NAME`, according to the field. |
| Indexed masks | `BIT_0`..`BIT_31` address a bit within a word; schema explains the owning list/bank. |

Store names were read directly from `StorePack` at `0x6676a0` (11 records,
stride `0x34`) and `StoreBundle` at `0x667660` (three records, stride `0x0c`).
The old store/area episode enum names were off by one; their callers were
renamed together so emitted numeric behavior stays the same.

All 44 `Cheat` names and their order were compared with the original table at
`0x619e80`, stride `0x20`. These indices select both extra masks. They are
**not** the `Cheat[i].flag` runtime-effect bits. The host reuses the canonical
`Cheat` names rather than duplicating its string table. `SAVE_EXTRA_INDEX`
records the index enum; other serialized enum values live alongside it in
`src/legoapi/core/save_values.h`. Unknown bits remain numeric and round-trip.

The host combines the multiword records into logical masks, without changing
the engine structs. Character purchases form one 128-bit field. The 90 names
in `save_names.hpp` were recovered using the existing archive extractor on
`chars\collection.txt`: valid, unique `buy_in_shop` entries in source order.
`LoadPerm2` requests exactly this filter and the original
`Collection_CreateCustom` checks `COLLECTID.can_buy` at +8. All 90 identifiers
were checked against the shipped character configuration. These bits index the
shop list, not `character_save`.

Extra unlocks/purchases each form one 64-bit field. Tutorial completion forms
one 192-bit field, with the second bank starting at bit 96. Its 54 named hints
per bank follow the original `Hints_LSW` records; callback names plus original
console text IDs distinguish entries without guessing display text.
`Hint_LoadAllGameState` confirms that the saved bit is the hint's table index.
`ShopHintTab` (`0x624fe4`, two bytes) contains only -1, so names cannot be
assigned to the separate 96-bit shop-hint purchase storage. Unidentified bits
in every logical mask retain `BIT_n` names across the entire field width.
Listings show the full numeric mask and each set bit's number/name in comments,
followed by an importable named assignment. Full-width decimal and hex parsing
does not depend on the host's native integer width.

## Envelope and limits

`PCSaveSlot` (`0x308454`) zeroes the `0x2028`-byte envelope and sets magic
`0x52474d48`, version 1 and its size. The extra-data offset at +20 selects a
prefix before the payload. Other envelope fields/buffers are exposed verbatim;
there is no evidence to assign them gameplay meanings. Payloads end with a
four-byte `ChecksumSaveData` result and four-byte slot code. The utility calls
the reconstructed checksum/hash routines and preserves arbitrary prefix bytes.

The original has no debug field/type information. Unresolved fields are kept
explicitly unresolved in the schema. Character/area/mission IDs and customizer
piece indices depend on loaded game configuration; storage capacities do not
prove those IDs exist in an asset set. Creation without a template does not
load assets or synthesize consistent 100% progress. Editing completion counters
does not imply all related pickups, purchases and rewards have been earned.

When structs or enums change, update the utility registry and descriptions,
rerun the exact-byte/schema coverage checks, and review both schema commands.

## Reconstructed routines and validation

Four previously empty routines now perform the original save updates. The
source-file optimization settings were preserved. Measured with
`scripts/objdiff-cli.py` against the rebuilt target:

| Routine | Original bytes | Current bytes | Match |
|---|---:|---:|---:|
| `CollectAllMiniKits` | 82 | 82 | 99.885% |
| `BuyAllShopExtras` | 181 | 181 | 99.889% |
| `Shop_CollectAllCharacters` | 202 | 185 | 49.246% |
| `Suits_CollectAll` | 40 | 44 | 56.714% |
| `Customiser_CopyDefaultPiecesToSave` (previous restoration, rechecked) | 239 | 239 | 100% |

The two near-total matches differ in relocation/GOT operands. The shop-character
routine still differs in register allocation/loop layout; the suit routine
retains the current compiler mode's extra frame setup. These are not claimed
as full binary matches. Existing save consumers were also compared after
constant/member renaming; names do not alter stored flag values or offsets.

Validation: target and Linux native builds, the four repository checks in
`//scripts/checks:checks`, and nine native save integration tests. The latter
cover corrected original offsets, enum combinations and invalid names, complete
schema byte coverage without overlap, random byte-for-byte export/import,
NaN/subnormal float preservation, odd-length prefixes, checksum/hash updates,
default paths, and failed-write preservation. No real save was edited.
Logical-mask tests also cross 32/64/96/128-bit boundaries, verify original
character-name positions, and check maximum/overflow values through 192 bits.

## Loaded progress reset during the title-to-hub transition

An isolated native `window --script-load` run with a copy of the played save
showed slot 0 loading correctly, followed by `MenuUpdateNewGame` calling
`NewGame` on the next frame and clearing the loaded progress. A GDB watchpoint
on `Game.area_save[30].complete` captured both the load and the subsequent reset.
The original `UpdateGameMenu` at `0x1192b0` skips menu callbacks when `NewLData`
is non-null. Restoring that guard prevents the title menu from starting a new
game while the requested load transition is pending. The larger routine remains
partially reconstructed; this is a restored condition, not a full match.

After the fix, the same scripted load retained both BlockadeRunner flags at 1
through `Hub_Reset`. Episode IV's `DE4` door and Chapter 1's `de4_1` door both
had obstacle runtime flags `0x10`, with the blocked bit `0x08` clear. Native and
target builds and all four repository checks passed. Runtime tracing used an
isolated copy, leaving the played save unchanged.

## Chapter doorway selection

The chapter trigger itself was present and active, but `Hub_Update` omitted the
selection that `Hub_ActivateDoorMenu` checks. The latter's original code at
`0x1b64d5..0x1b64ef` rejects a destination whose area differs from
`last_hub_area`; this check is retained.

Recovered chapter-door branches from original `Hub_Update`:

| Original instructions | Behavior |
| --- | --- |
| `0x1b6c59..0x1b6ccb` | Scan the original `HubAreaInfo` order for the first obstacle with output 1 (`NotAtStart`); select its area's index. |
| `0x1b6ccd..0x1b6d06` | Block other chapter obstacles while the selected door is opening. |
| `0x1b77d1..0x1b7893` | With no opening door, unblock entries allowed by area progress, bonus output 0, or the table's force-open byte. |
| `0x1b6d08..0x1b6d30` | Reset panel state during a fade or while a menu is active. |
| `0x1b8328..0x1b83c5`, `0x1b87af..0x1b87d3` | Give the episode panel priority; record `hub_area` and `last_hub_area` when the chapter-panel timer reaches zero, then seek toward the selected state at `2 * FRAMETIME`. |

This restores the chapter-door subgraph, not the entire hub update. Other
unreconstructed branches, including minikit-viewer proximity selection, remain
outside this change. Full-function objdiff remains low (7.983%); it is not a
claim of a matched function.

An isolated GDB regression used the loaded Episode IV Chapter 1 obstacle and
door records. It advanced the obstacle with `GizObstacle_JumpToEnd` followed by
`GameAnimSet_EvaluateState`, invoked `Hub_Update`, then crossed the real trigger
with `Doors_Check`. The observed selection was area 30, and the crossing opened
menu 15 with `hub_new_level=151` (BlockadeRunner, area 30). No selection or save
flags were forced by production code. This is a controlled trigger test, not a
claim of manual end-to-end play. Native/target builds and all four repository
checks passed.
