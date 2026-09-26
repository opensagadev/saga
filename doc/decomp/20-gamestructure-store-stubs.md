# Game structure store stub pass

Measured against `res/libTTapp.so` with the GOT-aware `objdiff-cli` fork and
the NDK r8e GCC target build. `StoreUnlockArcade`, `StoreUnlockBonus`,
`StoreUnlockBounty`, `StoreUnlockJedi`, `StoreUnlockSith`, and
`MenuDrawStorePurchase` are genuine target no-ops. Removing their `STUBBED()`
diagnostics retains 100% matching.

The short store callbacks are direct calls to `GameCam_Blend` or the purchase
API. `MenuInitStoreHolding`, `MenuExitStoreHolding`, `MenuExitStore`,
`MenuExitStorePurchase`, and `MenuInitStoreRestoring` match 100% after restoring
their calls and state writes. `MenuUpdateStoreHolding` matches 99.98%; its
remaining difference is the local string relocation in the Flurry event.

`Store_RootPackCustodian` reaches 100% with this ordering:

1. Load `field_0xefc` into a byte temporary.
2. Set `apiobj.flags_low` bit 1, update the temporary, set `field_0xefe` bit 6,
   then store the temporary back to `field_0xefc`.
3. Read `apiobj.field_0x1f4`, clear bit 0, then set bits 2 and 31.

GCC 4.7 combines independent read/modify/write operations if written as
consecutive compound assignments. An empty memory barrier after the byte
writes and an empty register barrier after the bit clear preserve the original
load and store order. The barriers emit no instructions.

The larger menu callbacks remain under reconstruction. The target uses an
unrolled sequence for 11 store packs in `MenuDrawDebugStore` and
`MenuDrawStore`, even though a source loop would be shorter. Preserve that
structure when matching these functions.

## Second batch

`NetworkSyncPause` matches all 529 target bytes. The eight player updates
compile as an unrolled sequence, but GCC schedules the three independent
stores by offset unless empty memory barriers separate them. Loading
`TOGGLEHOLDTIME` before the loop lets GCC retain its single `movss` load.

`InitSuperStory` is 99.58%, `MenuInitStore` 97.34%, and
`MenuInitStorePurchase` 99.90%. `Store_UprootPackCustodian` is 99.68%; the
only remaining difference is the equivalent x86 addressing order in one
`test` instruction (`[ecx+edx+5]` versus `[edx+ecx+5]`). Keeping an external
global's GOT-slot address live in `ecx`, then dereferencing it after the flag
stores, restored all other instruction order.

`StoreBundle_FindByName` was declared `void` despite returning an index in
the target. It now returns `i32` and has the three target bundle entries,
including the original `ORGINALPACK` spelling. It matches 93.51%. Marking
the first two successful comparisons unlikely with `__builtin_expect(..., 0)`
places their result blocks after the main return path, matching the target's
branch layout. The remaining differences are callee-save/load scheduling and
alignment padding.

## Restoring and debug menus

The target inlines each of the 11 pack checks in both the restoring and debug
menus. A normal loop loses the instruction shape and multiple call sites.
`MenuDrawDebugStore` reaches 95.33% when the pack body is macro expanded.
The product query writes a 0x304-byte structure: three 256-byte strings,
followed by a float price at offset 0x300. In the draw menu, its string at
offset 0x200 is copied into `StoreIAP[].text` as the selectable product ID.

`MenuUpdateStoreRestoring` needs purchased-product queries before bitset
tests, and `restoring_wait` resets to 3.0 seconds for each newly found item.
The list arrays store byte indices; counts and flags use separate globals.
GCC 4.7 changes the hot block order if the top-level count condition lacks
`__builtin_expect(..., 1)`. With the hints, this callback reaches 47.40%.
`MenuExitStoreRestoring` reaches 69.13% after keeping the accumulating flags
in a 32-bit local and pinning each byte count to `ecx`. The compiler still
chooses a different branch layout in the count loops.

Current other callbacks: `MenuUpdateDebugStore` 62.59%,
`MenuDrawStoreRestoring` 64.76%, `MenuUpdateStorePurchase` 62.34%, and
`MenuDrawStoreHolding` 99.97%. These scores are GOT-aware and use the
same NDK r8e GCC target build.

## Store pack data and large callbacks

`StorePack` is an initialized 11-entry, 0x34-byte table at target address
`0x6676a0`. Offset 0x04 is a `char *` product ID, offset 0x08 is a text
index, and offset 0x18 is a float floor offset. The previously all-zero
entries prevented the store menus from showing correct packs and caused
invalid character ID dereferences. Target product IDs are `EPISODE2` through
`EPISODE6`, `ARCADE`, `BONUS`, `BOUNTY`, `CHALLENGEPACK`, `JEDIPACK`, and
`EMPIREPACK`. The menu selects characters from the `id_*` pointers at
offset 0x20.

`MenuUpdateStore` has a hot touch-check path for the second half of its
12-second cycle. The radar pulse path is placed later in the function, after
the common exit. This is a useful control-flow clue for matching the NDK
r8e compiler: compare the cycle time once, then duplicate the three entry
checks in the two branches if a simple per-entry condition causes the cold
blocks to interleave. The menu uses the touch controller's 8-byte
`LastTouchPos` static symbol; its declaration and definition come from the
MechInputTouch menu controller reconstruction.

`MenuDrawStore` draws the three previous purchase panels before refreshing
their state, then the selected icon, selected product title and description,
and eligible bundles. The target stacks one 0x304-byte product structure
and unrolls 11 pack tests in the bundle price path. The bundle icon row
special-cases the selected character with icon type 0xa5 and uses 0xa7 for
the others. The 0x200 string field in the queried product is copied into
`StoreIAP[].text` for touch selection; the float price sits at 0x300.

Further compiler observations from target disassembly: the restore menu's
accept-button draw block is placed after both product loops; a cold branch
hint on its entry condition can reproduce that layout. The purchase menu
checks results in order 0, 2, 1, 3. Its 11 bundle mask tests reload the
bundle index and mask byte each time, so an empty memory barrier between
expanded checks can prevent GCC from merging loads. These observations
were used in the current source and should guide follow-up matching work.

The first implementation pass measures 22.97% for `MenuUpdateStore`
(1858 target bytes, 1612 candidate), 33.19% for `MenuDrawStore`
(5064/4809), 90.90% for `MenuUpdateStorePurchase` (1114/1107),
64.84% for `MenuDrawStoreRestoring` (944/963), and 47.40% for
`MenuUpdateStoreRestoring` (1102/1053). The data table `StorePack` is
63.46% over 572 bytes. All numbers use the GOT-aware forked `objdiff-cli`
on the linked target libraries.

For `MenuUpdateStore`, a rare-hit hint on the squared touch radius keeps
all three touch checks in the hot path and raises the match from 0% to
22.97%. Without it GCC places purchase handling after the first touch
check, then moves the remaining checks into cold blocks. A 16-byte dummy
stack object referenced through empty asm memory operands reproduces the
target's 0x120-byte stack frame and shifts the real local buffers into
their target slots, while emitting no instructions. `alignas` is not
accepted by this GCC 4.7 build; the GNU `aligned(16)` attribute does work.
