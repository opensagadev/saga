# Touch UI gesture tracker stub audit

The six base `MechInputTouchGestureTracker` callbacks in
`src/MechInputTouch/MechTouchUIElements.cpp` intentionally return `false`.
Their target functions occupy consecutive 16-byte-aligned slots at
`0x451c80` through `0x451cd0`. Each body is `xor eax, eax`, six NOPs, and
`ret`; the padding between slots is outside the function body. The existing
`return false` matched that behavior, so the `STUBBED()` diagnostics were
removed. Override callbacks elsewhere in the file implement the actual UI
behavior.

The six callbacks are `OnDown`, `OnRelease`, `OnClick`, `OnDoubleClick`,
`OnHold`, and `OnSwipe`. No register-allocation or control-flow pattern beyond
the target's ordinary zero-return sequence was found here.

## Low-match UI functions

The target's `MechTouchUICharIcon::Process` reads `selector->icon_count`
(offset zero) and compares it with one before setting the icon depth. The
earlier source instead read `selector->field_0x88`. The target also clamps
`alpha_elapsed + FRAMETIME` with compare and branch; writing `MIN` lets this
GCC emit `minss` and changes the block layout. Its disabled-state transition
compares the two raw byte values, avoiding the extra `setne` operations from
normalizing each value to `bool`.

In `MechTouchUIPlayerButton::SetupTargetIds` and `TriggerTagNext`, the target
tests only the low 16 bits of `apiobj` flags at offset `0x1f8`. An explicit
`u16` cast is needed because the struct currently exposes the field as a
32-bit value. The target's `SetupTargetIds` calls `Cheats_CheckFlags(0x100)`
inside qualifying paths through the model loop, so hoisting a cached result
before the loop changes both behavior and block order. `TriggerTagNext` begins
with the `FreePlay` test; an early `player == NULL` guard in source also
changed the whole function's layout.
