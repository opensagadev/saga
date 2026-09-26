# Podrace touch controller reconstruction

Target: `res/libTTapp.so`, Android NDK r8e x86 GCC 4.7. Compare code with the GOT-aware `objdiff-cli` fork.

## Multiple inheritance and callback return types

`MechInputTouchPodraceController` derives from `MechInputTouchMainController` followed by `MechInputTouchGestureTracker`. The second base begins at offset `0x6c`. Target symbols include `non-virtual thunk to ... OnDown/OnRelease` that subtract `0x6c` from `this` and jump to the body. The body returns `1` unconditionally, so these callbacks return `bool`, even though the old placeholder declarations said `void`.

The object has an active byte at `0x70` and a `TouchHolder *` at `0x74`, for total size `0x78`. `OnDown` stores the first active touch and `OnRelease` clears it only if it is the same touch. The constructor and destructor need the base class calls to produce the target vtable and lifecycle code. This pattern may apply to the Bonus Cavalry, Death Star Turret, and Speeder Chase controllers, whose declarations were also initially skeletal.

Both callback bodies have two separate `return 1` blocks in the target. Express the non-mutating case as an early return, followed by the field update and another return, to encourage the same block layout under GCC `-O2`.

## `Update` control flow

The target tests `player`, `NewMode`, `NewLData`, `FadeSys.fade`, `Paused`, `CUTSTOPGAME`, menu IDs 12 and 16, `TouchHacks::TouchControlsActive`, `MiniCutCam`, and `MechSystems::Get()->PlayerButton().selector` in that order. It calls `Activate` or `Deactivate` virtually through the input element vtable (`+0x24` or `+0x20`). It then clears the first two stick values. With a captured touch and valid `WORLD`, it sets the third previous-button byte, optionally sets the vertical stick to `-1` in `PODSPRINT_ADATA`, clamps three times the touch's horizontal position into `[-1, 1]`, and calls `UpdateButtons()`.

The optimized clamp uses `ucomiss` followed by `minss` on one branch and `maxss` on the other. Existing gesture-controller source uses the `MAX(-1.0f, (MIN(value, 1.0f)))` macro pattern; use that before trying manual `if` chains when matching this instruction shape.
