# Virtual console controller matching notes

The Android x86 target uses NDK r8e GCC and has 11 substantive
`MechInputTouchVirtualConsoleController` stubs. Compare these methods with the
GOT-aware `objdiff-cli` fork. The plain code addresses and GOT displacements
are not stable between the original and the reconstructed shared object.

## Class layout

The constructor writes a primary vtable pointer at `+0x00` and a second
vtable pointer at `+0x6c`. The second base is
`MechInputTouchGestureTracker`; the thunk for `OnDown` subtracts `0x6c` from
`this`. The first base is `MechInputTouchMainController`, whose size is
`0x6c`. Treating this class as a standalone four-byte object caused all
member accesses and destructor code to be wrong.

| Offset | Field | Evidence |
| --- | --- | --- |
| `+0x70` | active byte | `Deactivate` tests and clears it |
| `+0x74` | D-pad touch | `OnDown`, `OnRelease` |
| `+0x78` | drag touch | `OnDown`, `OnRelease` |
| `+0x7c`–`+0x88` | four UI buttons | `UpdateButtonPositions`, `Deactivate` |
| `+0x8c` | D-pad UI element | `UpdateDPadPos`, `OnDown`, `Deactivate` |
| `+0x90`, `+0x94` | optional UI elements | `Deactivate` removes and deletes them |

`MechTouchUIElement::position` starts at `+0x08`; its `owner` pointer is at
`+0x38`. D-pad animations are two adjacent `MechTouchUIAnimation` records at
element offsets `+0x40` and `+0x5c`. `OnDown` starts both animations toward
`1.0f`, and `OnRelease` starts them toward zero. Each uses a duration of
`0.15f`.

## Control flow and constants

`UpdateButtonPositions` places four buttons around
`SuperOptions.right_control_x/y`, with a vertical radius of `0.23f` on normal
screens and `0.29f` on small screens. The horizontal radius is scaled by
`GetAspectRatio()`. The target stores each position's `z = 0` and `w = 1`
before its `x/y` values. `ResetButtonPositionsToDefault` checks the touch
system control mode at `MechSystems + 0x2c`, copies the appropriate static
defaults to `SuperOptions`, then calls both position update methods.

The target's `ShouldBeActive` tests its global gates in order, with early
returns. In particular a paused game is allowed only for menu ID 25, then
menu IDs 12 and 16 are rejected. It checks `(WORLD->current_level->flags &
0x4e2) == 2`, the `TouchControlsActive` byte, and the player button's party
selector. If the selector is present and its byte at `+0x88` is clear, the
return value comes from `BlendedOut()`.
