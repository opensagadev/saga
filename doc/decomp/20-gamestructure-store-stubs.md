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
