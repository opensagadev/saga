# MechInputTouch gesture matching notes

The target is the NDK r8e GCC build of `libTTapp.so`. Score changes must be checked with the GOT-aware fork of `objdiff-cli`.

## Gesture dispatch

`MechInputTouchGestureTrackingSystem` derives from `NuTouchInputElement`. Its 0x30-byte base is followed by ten touch holders at `this + 0x30`, with a stride of `0x3bc`, then ten priority-ordered tracker entries at `this + 0x2588`. The constructor passes type 3, color `0xffff00ff`, and index 0 to the base constructor. `LookForDown`, `LookForRelease`, and `LookForHold` use an outer loop over holders. GCC expands the inner loop over all ten tracker entries into straight-line blocks, stopping at the first tracker whose virtual callback returns true. The callback vtable slots are `OnDown +0`, `OnRelease +4`, and `OnHold +0x10`. A source loop is preferable to manually spelling ten calls; the compiler's unrolling produces the target shape.

`LookForGestures` calls `LookForDown`, `LookForClicks`, `LookForRelease`, `LookForHold`, and `LookForSwipe` in that order. `Process` calls `ReadData` followed by `LookForGestures`. `Update` does nothing for null touch data; otherwise it uses `Player[0]`, falling back to `Obj`, and calls `Process`.

Each holder contains 20 swipe samples of size `0x2c`. A sample has touch x/y/time, followed by object position and velocity as two `VuVec` values. `ReadData` copies each sample as 11 dwords when it shifts the history. The five timing floats at holder `+0x3a4` through `+0x3b4` retain the current and four prior touch durations; the float at `+0x3b8` is the sample countdown. `TimeSampleDelta` is an exported 0.05f global. The close of a touch moves the duration through this chain and resets its ID to -1.

## Holder references and controller return types

`TouchHolder` fields at `+0x14` and `+0x20` are 12-byte `NuMechPtr<MechObjectInterface, 4>` values. They were initially represented as a raw pointer and padding. In `OnClick`, assignment through the existing `NuMechPtr` template explains hundreds of bytes of link manipulation in the target. The first byte at holder `+4` is set on a click.

`MechInputTouchGestureBasedController::PerformCloseMechanic` returns `bool`; `OnClick` tests its return byte. `TriggerJumpTask` likewise returns `bool`, and both automatic jump methods test that return. Their old `void` declarations suppressed needed data flow.

## Close mechanic search

`PerformCloseMechanic` first rejects linked characters other than Yoda and nonzero `VehicleArea`. It tries teleport, zip up, hat machine, lever, panel, then an attackable game object. The target uses `GameObject_s::field_0x1008` as the search radius. For hat machine, lever, and panel, the accepted squared distance is `(field_0x1008 + 0.2f)^2`. The fallback object search uses `4.0f * field_0x1008` for both radius arguments. Teleport additionally requires vertical distance below `apiobj.scaled_height` and horizontal squared distance below `0.1225f`. Each successful branch creates the corresponding touch task and calls `StartNewTask`.

## Gesture controller branches

`OnSwipe` checks for a nearby zip up before looking at movement direction, so any swipe there starts `MechTouchTaskUseZipUp`. Its jump packet has type 3, the zero `VuVec`, and start/end touch coordinates from the selected history sample and current touch. Its `isBucking` value comes from bit 1 of the character config byte at `+0x96`. During an active jump task, a down swipe chooses marker style 1 or 2 from `GameObject_s + 0x7a9`; other swipes choose style 0. The marker calls precede the `GameObject_s + 0xf04` input bit writes.

`OnHold` tests the target interface type through virtual `GetObjectType`: 12 is a part, 5 Force, 6 build, and 2 character. Types 5 and 6 flash the target and start their corresponding task. The character path can create a `MechTouchTaskTag`, `MechTouchTaskUseForce`, `MechTouchTaskAttack`, or `MechTouchTaskBlock`, or create a tag button. The tag button is assigned into the controller's `NuMechPtr` at `+0xac`; the target inlines the linked-list unlink/link operations, accounting for a large middle section of the function. A consumed hold sets holder byte `+7`.

`OnDoubleClick` tests `VehicleArea` first and delegates to the click callback if it is nonzero. Its close-range character or obstacle path saves the player's current velocity and target velocity, calls `CalculateJumpVelToHitPoint`, writes the jump velocity to both fields, and restores them if `TriggerJumpTask` fails. This save/restore pattern also appears in both automatic jump methods. Packet `field_4` and `field_8` carry the character and touch-holder addresses for automatic jumps; those fields had previously looked like unused padding.

`ProcessAutoJumpOverGap` excludes Rescue E and Gungan A levels, and gates Mos Eisley A on a character model flag. It checks for a danger zone or ledge ahead using a 0.3 second lookahead, searches a planned jump connection, and falls back to `CheckJumpForLandingSpot`. `ProcessAutoJumpWhenStuck` tracks low motion relative to half the input magnitude for 0.2 seconds, then probes floor height ahead and triggers a jump once. Its `field_a6` byte prevents repeated triggers until movement resumes. For both methods, calling `TriggerJumpTask` with `false, true, true` and checking its bool return is essential to the conditional velocity restore.

## First measured target build

After compiling with the NDK r8e GCC target configuration, the GOT-aware fork of `objdiff-cli report generate` measured these changes from the Speeder wave 1 baseline:

| Function | Before | After |
| --- | ---: | ---: |
| Main controller `Update` | 18.26% | 100.00% |
| Tracker `LookForDown` / `LookForRelease` / `LookForHold` | 3.23% / 3.21% / 3.36% | 93.66% / 93.63% / 92.24% |
| Tracker `ReadData` / `LookForClicks` / `LookForSwipe` | 0.72% / 1.72% / 1.89% | 49.14% / 59.56% / 57.06% |
| Gesture controller `OnClick` / `OnHold` / `OnSwipe` | 0.53% / 1.34% / 1.67% | 43.06% / 40.60% / 60.17% |
| Gesture controller `OnDoubleClick` / `PerformCloseMechanic` | 1.11% / 1.49% | 24.23% / 55.28% |
| Gesture controller `ProcessAutoJumpWhenStuck` | 2.35% | 33.51% |
| Tracker constructor / `ProcessAutoJumpOverGap` | 4.80% / 1.59% | 0.00% / 0.00% |

The report omits the `fuzzy_match_percent` key when a function scores zero. Verify such entries with a direct `objdiff-cli diff` before treating them as missing symbols. The tracker constructor and over-gap function both exist in the linked library; their generated control flow needs more work. The report's whole-game fuzzy figure moved from 59.448704% to 59.736923% across the combined gesture and context batch; this comparison does not isolate the gesture contribution.

The constructor's zero score has a specific compiler cause. A C++ member `TouchHolder holders[10]` makes GCC unroll the ten nontrivial `NuMechPtr` default constructors before the explicit holder loop. The target initializes each pair of references *inside* its holder loop, then clears 20 samples, ID/flags, an empty reference assignment, and six timing floats. Raw holder storage with placement construction per iteration should preserve the loop and its intended register allocation, but changing that layout requires a broad dependent rebuild.

For the over-gap jump, target level checks come first. A missing `field_90` falls back to `object.touch_task->touch_holder`; it does not disable the jump. The excluded action contexts are `LEGOCONTEXT_JUMP` and `LEGOCONTEXT_LAND_JUMP`. The jump packet retains the original velocity while a separate normalized candidate temporarily replaces both object velocity fields. The call to `NuVecNorm` occurs only when the original magnitude is below the character config speed at `+0x1c`.
