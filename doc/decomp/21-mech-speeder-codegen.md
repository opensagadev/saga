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

## Swipe angle calculation

IsDownSwipe and IsUpSwipe pass end.x - start.x and
end.y - start.y to NuAtan2D, narrow its result to 16 bits, call
RotDiff(angle, 0x8000) or RotDiff(angle, 0), take the signed absolute
value, and compare with 0x1c71. The target uses an arithmetic-shift,
XOR, subtraction sequence for the absolute value. Source expression
diff < 0 ? -diff : diff is a useful way to request that sequence at -O2.

OnSwipe indexes touch history in 0x2c-byte records beginning at
TouchHolder + 0x34; the current position is at TouchHolder + 0x2c.
