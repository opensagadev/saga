# Bonus Cavalry touch controller reconstruction

The Android x86 target's `MechInputTouchBonusCavalryController` constructor,
destructor, and `OnDown`/`OnRelease` thunks establish a two-base layout:
`MechInputTouchMainController` at offset 0 and
`MechInputTouchGestureTracker` at offset `0x6c`. The tracker thunks subtract
`0x6c` from `this` before entering the derived methods. The derived class has
an `active` byte at `0x70` and a `TouchHolder *` at `0x74`, for total size
`0x78`. A declaration with only a virtual destructor silently loses the
tracker thunks and places the controller on a different ABI.

The target's `OnDown` and `OnRelease` are deliberately asymmetric. `OnDown`
claims the first touch if no touch is active; `OnRelease` clears the touch
only when the same holder releases. Both return `true` on every path. Their
signatures therefore return `bool`, even though the old stubs said `void`.

In `Update`, the target calls `NuFsqrt` twice on the same drag length squared
after a `0.05f` dead-zone check. The second call is part of the original
code generation and should not be folded by hand when matching. The
controller projects a point one object radius along `apiobj.facing_angle`
with `NuCameraTransformScreenClip`, then uses the projected screen direction
to rotate the touch drag into its two stick axes. The gunship B level uses
a `0.2f` drag scale and snaps the first axis to +/-1 above absolute `0.3f`;
other levels use `0.5f`. GOT-aware objdiff resolves the level pointer as
`BONUS_GUNSHIPB_LDATA`. The target reloads `WORLD` and the level pointer
after the camera projection call before the final snap decision. Preserve
both comparisons; caching the first result across that call changes the
observable behavior and the compiler's block graph.
The target also reloads `player` after the second `NuFsqrt` and after
`NuCameraTransformScreenClip`. Keep those accesses as separate global reads
across the calls instead of carrying the first object pointer throughout
`Update`.

The target places a shared `UpdateButtons()` epilogue immediately after the
first drag-length dead-zone test, before the activation guard's cold blocks
and the full steering math. Expressing null-touch and dead-zone paths as
early returns helps GCC place that block there. Nesting all steering math in
one positive `if` moves the epilogue to the far end of the function and
changes the block alignment even when the instruction count is similar.

The `MechSystems` field read at `0x271c` during `Update` is not named in the
current type map. The source accesses it by offset pending type recovery.

This translation unit needs a file-specific `-O2` target flag. Without it,
the Bazel fastbuild default produces `push ebp; mov ebp, esp; ...; ret` stubs,
while the target's tiny touch methods have early-return branches and aligned
blocks characteristic of optimized GCC 4.6. The neighboring main and gesture
controller translation units already have `-O2` entries.
