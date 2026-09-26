# Speeder chase touch controller notes

The Android x86 target was built with NDK r8e GCC 4.7. Match these methods against
res/libTTapp.so with the GOT-aware objdiff-cli fork after a target build.

## Object layout and secondary vtable

MechInputTouchSpeederChaseController derives from
MechInputTouchMainController (size 0x6c) and then
MechInputTouchGestureTracker. The tracker vptr is at this + 0x6c.
Its callback thunks subtract 0x6c from the incoming this pointer before
jumping to the real method. The original stubs declared those callbacks as
void, but the target methods return bool; most return the constant true.
The derived state starts at 0x70: active touch pointer, two floats at
0x74 and 0x78, and two flags at 0x7c and 0x7d.
The deleting destructor uses NU_FREE, which calls NuMemoryGet,
GetThreadMem, and BlockFree(ptr, 0). A class-specific operator delete
is needed to match that path; global delete emits a different call.

## Swipe angle calculation

IsDownSwipe and IsUpSwipe pass end.x - start.x and
end.y - start.y to NuAtan2D, narrow its result to 16 bits, call
RotDiff(angle, 0x8000) or RotDiff(angle, 0), take the signed absolute
value, and compare with 0x1c71. The target uses an arithmetic-shift,
XOR, subtraction sequence for the absolute value. At -O2, source expression
diff < 0 ? -diff : diff instead optimized the comparison into an unsigned
range test (add 0x1c71, compare 0x38e2). To request the target sequence,
write sign = diff >> 31; diff ^= sign; diff -= sign; then compare diff.

OnSwipe indexes touch history in 0x2c-byte records beginning at
TouchHolder + 0x34; the current position is at TouchHolder + 0x2c.

## Pending control-flow alignment

The target's direction wrappers test the helper's AL return and join shared
0/1 return blocks. A standalone NDK r8e GCC 4.7 `-O2` probe shows that ordinary
`bool` and `i32` wrappers fold this shape to a direct return or `movzbl`.
The current source uses x86-only `asm goto` to request the target's two branch
edges; its linked match score is still unverified. Native builds use ordinary
C++ fallbacks. The target Speeder `Update` checks `WORLD` for the active touch
path, places the negative swipe branch on the fallthrough path, and clamps X
with `minss(x, +1)` or `maxss(-1, x)` depending on the first comparison. Its
constructor zeros cooldown before swipe Y. These pending edits require a
GOT-aware linked diff before treating the codegen pattern as confirmed.
