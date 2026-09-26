# MechInputTouch gesture matching notes

The target is the NDK r8e GCC build of `libTTapp.so`. Score changes must be checked with the GOT-aware fork of `objdiff-cli`.

## Gesture dispatch

`MechInputTouchGestureTrackingSystem` keeps ten touch holders at `this + 0x30`, with a stride of `0x3bc`. It keeps ten priority-ordered tracker entries at `this + 0x2588`, each containing a tracker pointer and a priority. `LookForDown`, `LookForRelease`, and `LookForHold` use an outer loop over holders. GCC expands the inner loop over all ten tracker entries into straight-line blocks, stopping at the first tracker whose virtual callback returns true. The callback vtable slots are `OnDown +0`, `OnRelease +4`, and `OnHold +0x10`. A source loop is preferable to manually spelling ten calls; the compiler's unrolling produces the target shape.

`LookForGestures` calls `LookForDown`, `LookForClicks`, `LookForRelease`, `LookForHold`, and `LookForSwipe` in that order. `Process` calls `ReadData` followed by `LookForGestures`. `Update` does nothing for null touch data; otherwise it uses `Player[0]`, falling back to `Obj`, and calls `Process`.

## Holder references and controller return types

`TouchHolder` fields at `+0x14` and `+0x20` are 12-byte `NuMechPtr<MechObjectInterface, 4>` values. They were initially represented as a raw pointer and padding. In `OnClick`, assignment through the existing `NuMechPtr` template explains hundreds of bytes of link manipulation in the target. The first byte at holder `+4` is set on a click.

`MechInputTouchGestureBasedController::PerformCloseMechanic` returns `bool`; `OnClick` tests its return byte. `TriggerJumpTask` likewise returns `bool`, and both automatic jump methods test that return. Their old `void` declarations suppressed needed data flow.

## Close mechanic search

`PerformCloseMechanic` first rejects linked characters other than Yoda and nonzero `VehicleArea`. It tries teleport, zip up, hat machine, lever, panel, then an attackable game object. The target uses `GameObject_s::field_0x1008` as the search radius. For hat machine, lever, and panel, the accepted squared distance is `(field_0x1008 + 0.2f)^2`. The fallback object search uses `4.0f * field_0x1008` for both radius arguments. Teleport additionally requires vertical distance below `apiobj.scaled_height` and horizontal squared distance below `0.1225f`. Each successful branch creates the corresponding touch task and calls `StartNewTask`.
