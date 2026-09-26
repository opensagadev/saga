# Death Star turret touch controller codegen notes

The Android x86 target builds `MechInputTouchDeathStarTurretController` with two
bases: `MechInputTouchMainController` at offset `0` (size `0x6c`) and
`MechInputTouchGestureTracker` at `0x6c`. The derived controller is `0x7c`
bytes. Its own data are an active byte at `0x70`, an aiming `TouchHolder *` at
`0x74`, and a `GIZTURRET_s *` at `0x78`. The target's tracker virtual thunks
subtract `0x6c` from `this` before jumping to `OnDown`, `OnRelease`, and
`OnSwipe`. Missing the second base makes the methods and vtable differ even
when their bodies are correct.

The target `Update` calls `Deactivate` and `Activate` through the main base
vtable slots `0x20` and `0x24`. It clears only `stick_values[0]` and
`stick_values[1]`, then uses the held touch and turret to aim. This is a useful
control-flow fingerprint for related touch controllers.

The target's angle calculation first projects the turret position with
`NuCameraTransformScreenClip`. It multiplies the horizontal screen difference
by `8192.0f` and the positive vertical difference by `3641.0f`, truncates to
integers, and passes `8.0f` to each `SeekRot` call. The `NuAtan2D` arguments
are the camera's Z difference first and X difference second. Reversing them
changes the turret yaw even though the code still compiles and looks plausible.

The target `OnSwipe` returns immediately when the sampled Y position does
not satisfy the comparison, leaving the return register holding the touch
pointer. This suggests a missing explicit return in the original C++ source;
verify the compiler output before trying to normalize that path.
