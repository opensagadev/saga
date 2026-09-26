# Virtual console widget reconstruction

Target: Android x86 `res/libTTapp.so`, built with the NDK r8e GCC toolchain.
Compare per symbol with the GOT-aware `objdiff-cli` fork. Absolute call and GOT
displacements shift in the reconstructed shared object.

## ABI and object layout

All four `VirtualControl*` widgets derive from `MechTouchUITexButton`. The
`MechTouchUIElement` base is at offset zero and provides `position`, `owner`,
callbacks, `Process`, and `Render`. The texture button base occupies `0x78`
bytes. The derived vtables confirm inherited destructors.

| Widget | Size | Fields after texture button base |
| --- | ---: | --- |
| `VirtualControlButton` | `0x7c` | button type at `+0x78` |
| `VirtualControlButtonMover` | `0x88` | controller pointer at `+0x78`, drag offset at `+0x7c`, pulse phase at `+0x84` |
| `VirtualControlDPad` | `0x118` | stick values `+0x78`, drag offset `+0x80`, arrow material `+0x88`, controller `+0x8c`, embedded mover `+0x90` |
| `VirtualControlDPad_LockButton` | `0x7c` | D-pad pointer at `+0x78` |

The D-pad destructor destroys its separate arrow material, then the embedded
mover's texture button base, then its own texture button base. The embedded
mover constructor runs before the D-pad initializes its callback and arrow
material.

## Constructor constants and texture mapping

`VirtualControlButton` maps button types `0, 1, 2, 3` to texture indices
`1, 3, 0, 2`. The mover uses texture index `8` and radius `0.1f`. The D-pad
uses texture index `4` for the button base, texture index `5` for an additional
material, and its passed radius. The lock button uses radius `0.1f` and texture
index `6` when `SuperOptions.dpad_locked` is clear, or `7` when set. The button
and lock button `Render` overrides only call `MechTouchUITexButton::Render`.

The mover's default homogeneous position is
`(SuperOptions.right_control_x, SuperOptions.right_control_y, 1, 1)`; button
and D-pad constructors form `(x, y, 0, 1)` from their `NuVec2` argument.

## Compiler and behavior observations

The mover's pulse phase is an integer. `Process(elapsed)` adds
`static_cast<i32>(elapsed * 65536.0f)`, then uses
`NuTrigTable[(phase >> 1) & 0x7fff]`. A floating field at `+0x84` or a
different unit scale gives an incorrect multiply/convert sequence. It writes
both `scale_to` and `scale` and copies `scale_duration` to `scale_elapsed`.

When touched, the mover sets its scale to `1.1f`, adds the saved drag offset to
the touch position, clamps its center to the screen bounds, writes
`SuperOptions.right_control_x/y`, and calls the controller's
`UpdateButtonPositions`. Small-screen center bounds are `x=[0.38, 0.62]`,
`y=[-0.49, 0]`; normal bounds are `x=[0.23, 0.77]`, `y=[-0.62, 0]`.

The lock button `Process` copies the D-pad position and clamps the lock button
near its lower corner. Its hovered state sets alpha to `1.0f` and scale to
`1.2f`; otherwise alpha is `0.4f` and scale is `1.0f`. It stores each value in
both the current and destination animation fields, and sets animation elapsed
to duration to suppress interpolation.
