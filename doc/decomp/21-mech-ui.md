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
