# Menu touch controller matching notes

## ABI and compiler patterns

- `MechInputTouchMenuController` has two polymorphic bases in the target: `MechInputTouchMainController` at offset `0` and `MechInputTouchGestureTracker` at offset `0x6c`. The second vtable emits `this`-adjusting thunks that subtract `0x6c` before entering `OnClick`, `OnDown`, and the other gesture callbacks. A declaration with only a vptr loses those thunks and changes the constructor and destructor bodies.
- The derived constructor calls the main controller constructor, installs both vptrs, clears fields at `0x70`, `0x74`, and `0x78`, and registers its gesture tracker with priority `50`. `Activate` also registers; `Deactivate` unregisters. The registration target is `MechSystems::Get()->gesture_tracking_system` at offset `0x84`. The first two derived fields are a tracked `TouchHolder*` and `MENU*`; the third is a byte flag.
- `UpdateButtons(int)` reads `backButtonPressedLastFrame` and calls `PerformPauseButtonStuff()` only when the sign bit of the new button state clears after being set. This global is distinct from `MechInputTouchMenuController::PackButtonPressed` despite the similar name.
- The target `Render()` contains eight one-byte NOPs and a return. `OnDoubleClick` and `OnSwipe` each zero `eax`, execute six one-byte NOPs, and return false. GCC 4.7 emits these NOPs automatically for empty or constant-return routines at `-O2`; explicit inline NOPs are additive and overshoot the target. This was confirmed with a standalone NDK r8e compile.
- This translation unit's target functions use optimized PIC code. The NDK r8e GCC 4.7 compiler retains an `-O0` frame pointer policy when `#pragma GCC optimize("O2")` is applied after the command-line `-O0`, despite optimizing much of the body. A per-file Bazel `-O2` option restores frameless code for this translation unit.

## Menu pack touch globals

The target exports `PackButtonW`, `PackButtonX`, `PackButtonY`, `PackButtonActive`, and `LastTouchPos`. `OnClick` checks the active flag, clears it, forms a three-component displacement from the holder's touch position, divides the X displacement by `GetAspectRatio()`, and uses `NuVecMag` to test against `PackButtonW`. A hit sets `PackButtonPressed`.

## Unfinished larger gestures

`OnDown` and `OnHold` use `GameMenu[GameMenuLevel]` and the free-play collection for menu 17. The collection's selectable count is `count_y` at offset `6`, each `COLLECTID` is `0x1c` bytes, and the selection radius is `fabsf(0.5f * collection->field_14)`. Coordinates come from `TouchHolder::down_position` for `OnDown` and `touch_position` for `OnHold`. Both divide the X displacement by `GetAspectRatio()` before `NuVecMag`. `OnRelease` is about 2 KB and contains several interaction paths. The target GOT-relative references can be resolved by reading the GOT entry and looking up its destination in `nm -n`, since several globals have `R_386_RELATIVE` relocations rather than named dynamic relocations.
