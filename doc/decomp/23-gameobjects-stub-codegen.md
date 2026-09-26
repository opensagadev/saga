# Game object stub reconstruction notes

Reference: Android x86 `res/libTTapp.so`. Compile with the NDK r8e GCC 4.7
at this file's `-O3` setting and compare with the GOT-aware `objdiff-cli` fork.

## Retail no-ops

The five `Condition_NumForceObjects`, `Condition_Indy`, `Condition_PSP`,
`Condition_CheatProgress`, and `Condition_CutScenePlaying` callbacks return
zero. Their target bodies are nine bytes: `fldz`, six one-byte `nop`s, `ret`.
`GameAttackInit`, `SpecialObject::SetInitialPosition`,
`GameThingManager::AddLevelOnlyThings`, nine `BaseThing` interface defaults,
and `LEGO_AllGoldBricksFn` are empty bodies. Each target body is eight
one-byte `nop`s followed by `ret`. The `nop`s are retained inside each symbol
and are reproduced by an empty source body at `-O3`; they do not represent
missing behavior. A GOT-aware direct-object report confirms all eighteen
reconstructed no-op/zero functions at 100%.

## `LEGO_100PercentFn`

Target `0x11a710`, 131 bytes. The function checks `CharacterCustomiser`, calls
`Customiser_Set100PercentPieces`, looks up `"hat_hair_00"` with
`Customiser_FindPieceByName`, and stores the second result as an `i16` into
`customiser->save->pieces[first result]` only when the lookup succeeds and
both results differ from `-1`. The lookup's return value matters even though
the current implementation elsewhere declares it `void`; the return type is
absent from the mangled name, so this source unit declares the observed
pointer return. The direct object has the target's exact 131-byte instruction
shape. Its raw diff is 99.25% because six PC thunk, GOT, call, and literal
relocations cannot resolve until linking.

## `DrawPackButton`

Target `0x24c8e0`, 2,511 bytes. The retail callback uses a large stack product
record whose price float lies at byte offset `0x300`, plus a 128-byte format
buffer. The product ABI is currently absent from `NuIOS_InAppProduct`, so a
local layout supplies that offset for this one callback.

The callback computes a half-second pulse with `NuFmod`, indexes
`NuTrigTable` using the integer angle's low 15 bits, then scales the result
by `0.2f` and adds `0.8f`. It formats the selected pack's localized label and
price when purchases are available. It then walks three `StoreBundle` entries
via their mask members, tests the selected pack bit, and totals the prices of
locked packs in each bundle. The eleven `Store_IsPackUnlocked` calls in the
target are unrolled with constant indices, 0 through 10; a normal counted
loop produces a different control-flow shape. Each conditional purchase-price
call uses the same stack product record. If the total exceeds the bundle
price, it draws `tBUNDLESAVINGSAVAILABLE` above the button. The common tail
draws the panel object, the `">"` glyph, and updates the touch controller's
active, position, width, and selected-pack globals.

Retail GOT mapping recovered from the linked ELF: `GameTimer`, `NuTrigTable`,
`StorePack`, `StoreBundle`, `TTab`, `text3d_height`, `WORLD`, `ICONSIZE`,
`tBUNDLESAVINGSAVAILABLE`, and five
`MechInputTouchMenuController::PackButton*` globals. The target loop keeps a
pointer to each bundle's `pack_mask`, advancing by 12 bytes and loading the
corresponding product name from four bytes before the mask. This pointer
choice is observable in the emitted `test (%edi),%eax` and
`mov -4(%edi),%eax` instructions.

The first reconstruction preserves this behavior but the direct object still
has different stack offsets, register allocation, and block order. The raw
relocatable diff reports 0% for the whole symbol despite many shared
instructions; compare the linked build and individual regions before
interpreting that figure as a semantic failure.
