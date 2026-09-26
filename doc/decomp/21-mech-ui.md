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

The target has four static `Cheats_CheckFlags(0x100)` call sites inside
`SetupTargetIds` and no call before the model loop. A single per-model cached
check still changes the call graph; each qualifying condition must contain
its own short-circuit call.

The target's `MechTouchUIPlayerButton::Process` lays out the one-time chooser
initialization before the periodic update loop. Put the `chooser_mode != 0`
branch first in source to reproduce that fallthrough. The initialization
search leaves `field_0x144[index]` unchanged when it finds no player, and the
periodic search does not prefilter negative target IDs. The target reloads
`selector` after calling `BlendOut`, so the following `BlendedOut` check
needs a fresh non-null test. It tests `player` separately inside each chooser
branch after the common `disabled` test.

`TriggerTagNext` advances an index, wraps 32 to zero, and stops after testing
the starting index again. Its target loop has no separate offset counter.
Writing `(current_index + offset) & 31` changes the loop condition and the
generated blocks.
