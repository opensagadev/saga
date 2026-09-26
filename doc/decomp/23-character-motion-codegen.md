# Character motion and camera matching notes

These observations come from `res/libTTapp.so` and the Android NDK r8e x86
GCC 4.7 target. Use the GOT-aware fork of `objdiff-cli` for score checks.

## Callbacks split across source files

`ChrisAnakinAUpdate` loads `WORLD->space_level` into `%eax` and tail-calls
`ProcessSpaceLevel`. The target helper has the local name
`_ZL17ProcessSpaceLevelP12spacelevel_s` and reads its argument from `%eax`.
The current source has the helper in `items/collect/bolts.cpp` and the caller
in `characters/motion/chris_stubs.cpp`, so both declarations need the same
assembler name and `regparm(1)` attribute. Hidden visibility allows a direct
intra-library call. `DrawSpaceLevel` uses the same arrangement with
`_ZL14DrawSpaceLevelP12spacelevel_s` and `regparm(1)`.

`AtatPart_Stop` and `AtatPart_Update` are local target symbols, but Episode V
stores their function addresses as part callbacks. Their cross-file source
declarations use explicit `_ZL...` assembler names and hidden visibility to
keep the target identity while allowing the episode file to refer to them.

## Camera mini-cut construction

`GameCameraMakeMiniCut2` first sets a 12-byte global `nugspline_s`: length
`2`, point size `12`, and `pts = ObstacleCamCutPts`. It conditionally copies
the camera and target vectors into two adjacent `NUVEC` points, then calls
`GameCameraMakeMiniCut` with the four timing floats, the last integer border
argument, and zero for hold. Two preceding integer arguments independently
enable tracking the original camera and target pointers. The target makes a
separate copy of the helper call in the branch where no target vector was
provided. That duplicated call is a useful clue when adjusting source block
order for matching.

The reconstructed `GameCameraMakeMiniCut2` is 99.8% matched. The remaining
two instructions load and test the follow-camera flag in `%ecx`, while the
target uses `%edi`. Forcing `%edi` with a zero-byte extended asm constraint
moves the mismatch to the follow-target flag in `%ebp`; also forcing `%ebp`
changes the frame and worsens the whole function. Treat this as a joint
register-allocation problem, not two independent register substitutions.

## Source types visible in instructions

`cbNearClipAtCursor` loads byte offset `0x11` of `eduiitem_s`, masks bit `0`,
and stores the result to `near_clip_at_cursor`. The `flags` member has exactly
that offset. `Animate_POD` writes object offset `0x42`; the animation packet
starts at object offset `0x08`, so this is `requested_animation` at packet
offset `0x3a`. Always subtract an embedded structure's base offset before
identifying a field in disassembly.
The POD fallback reads object offset `0xe10` (`previous_block_animation` in
the current type) rather than the more commonly used action context animation
at `0x79a`.
The POD merge path is most similar when the signed low byte of
`apiobj.field_0x1f8` first branches around the path, and a signed 8-bit
`apiobj.field_0x27c` supplies the merge index. An `i32` temporary causes a
32-bit comparison where the target compares `%al` with `0xff`.

`DogFightARestart` fills the four initialized `DogDebKey` integers with `-1`.
The target uses `pcmpeqd` to make a vector of all-one bits and one aligned
`movdqa` store. The `DogDebKey` data symbol is 16 bytes, with all four values
initialized to `-1` in `.data`.
Writing through an SSE `__m128i` pointer with `_mm_set1_epi32(-1)` reproduces
the target's 30-byte function, including its unusual delayed frame-pointer
setup. Four scalar stores compile to a 45-byte mismatch.

`ChrisAnakinBInit` uses an integer 12-byte copy from `NuSpecialGetPos` into
`RadialMoveCentre`. A `memcpy` of exactly `sizeof(NUVEC)` compiles to that copy;
three scalar float assignments use `movss` instead. The remaining single-digit
diffs in this function are offsets to local `.bss` and `.rodata`, which follow
whole-library data layout rather than the C++ statement sequence.

The tiny `ChrisAnakinAUpdate`/draw wrappers in `chris_stubs.cpp` need
`optimize("O3,omit-frame-pointer")` at function scope. `optimize("O3")` alone
retained `push %ebp`/`pop %ebp` and could only reach 50% despite the correct
call target and register ABI. With explicit frame-pointer omission the three
wrappers match exactly.
