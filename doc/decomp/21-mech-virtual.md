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

The constructor clears the four button pointers, the two touch pointers, the
active byte, and the optional pointers. It does **not** clear the D-pad pointer
at `+0x8c`; adding that initializer creates an extra store absent from the
target. `Update` assigns the D-pad pointer during lazy UI creation.

`MechTouchUIElement::position` starts at `+0x08`; its `owner` pointer is at
`+0x38`. D-pad animations are two adjacent `MechTouchUIAnimation` records at
element offsets `+0x40` and `+0x5c`. `OnDown` starts both animations toward
`1.0f`, and `OnRelease` starts them toward zero. Each uses a duration of
`0.15f`.

The build initially omitted this translation unit from
`bazel/android_per_file_copts.bazelrc`. GCC therefore compiled it without
optimization, producing a frame pointer and zero match for even the simple
`UpdateDPadPos`. The target uses `-O2`; add the per-file option before
interpreting instruction-level diffs. With `-O2`,
`ResetButtonPositionsToDefault` matches 100% in the GOT-aware fork.

`LoadPerm` in the target makes nine separate `NuTexRead` calls in texture
index order `2, 3, 0, 1, 4, 5, 6, 7, 8`. Replacing them with a loop over a
temporary descriptor table shrinks the local function and defeats matching;
retain the individual calls. The `hasDoneLoadPerm` symbol is one byte in the
target BSS, and its final store uses `movb`.

The other controller globals have distinct sizes: `lookAtMeBlendDone` is a
one-byte BSS flag, while `s_noInputTimer` is four-byte `f32`. Declaring either
flag as `i32` changes load and store instructions throughout `Update`.

In `Update`'s lazy UI creation, the target calls `NuIOS_IsSmallScreen()` and
`GetAspectRatio()` before allocating the D-pad. It retains
`aspect_radius = aspect * radius` across that constructor call. The button
positions are then calculated between their individual allocations. GCC
retains the source's `0.0f * aspect_radius` and `0.0f * radius` operations;
replacing those with direct `x` and `y` coordinates removes target floating
point instructions. Preserve the call and calculation order when tuning this
large function.

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

`ProcessDragMovement` uses a `0.05f` displacement threshold and requires the
touch to be held for more than `0.2f`. Its strength is
`clamp((distance - 0.05f) * 4.0f, 0, 1) * 1.4`; the target applies sine and
cosine lookup values from `NuTrigTable` and clamps the output stick values to
`[-1, 1]`. The drag values go to inherited `stick_values[2]` and `[3]` at
offsets `+0x38` and `+0x3c`.
The unsuffixed `1.4` matters: the unsaturated path converts the float strength
to double, multiplies by a double constant, then converts back to float. A
`1.4f` literal removes that target instruction sequence. The target evaluates
cosine for Y before sine for X, then stores X and Y in that order.
It loads/subtracts the Y displacement before X, but computes the squared
distance as `dx * dx + dy * dy`; reversing those operands changes the SSE
addition and register schedule even though the arithmetic is equivalent.
It also loads the sine and cosine lookup values before comparing the scaled
distance with `1.0f`. Computing the clamp before reading the table leads to
a different schedule in the NDK compiler.

`OnDown` enters its drag path only when `down_position.x < 0.0f`. Expressing
that condition directly preserves the target's ordered SSE comparison and
NaN behavior. Cache the D-pad pointer before the timer check so the target
can retain one register through both animation calls, position writes, and
the owner assignment.

The virtual controller destructor explicitly deletes the four UI buttons and
the D-pad in that order before calling its main-controller base destructor.
`Deactivate` only removes those five from the UI; it deletes the two optional
controls at `+0x90` and `+0x94`. These lifetimes explain why an otherwise
empty destructor compiled into only the base call and missed most of the
target function.
The deleting destructor frees the controller through `NU_FREE`, which calls
the thread memory manager; default C++ `operator delete` emits a different
call sequence.
