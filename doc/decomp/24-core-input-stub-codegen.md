# Core input stub reconstruction

Target: Android NDK r8e x86 GCC 4.7, `res/libTTapp.so`. Both
`src/legoapi/core/input/gamepads.cpp` and `timing.cpp` have explicit `-O2`
entries in `bazel/android_per_file_copts.bazelrc`. Use the installed GOT-aware
fork of `objdiff-cli` for final linked scores; an object-to-library diff still
reports some PIC relocation operands as different.

## Controller input

`Controller_Read` at `0x000e3d70` is 141 bytes. The old declaration had no
parameters and returned `void`; the target accepts twelve stack arguments and
returns integer `1`. It ignores the first argument. After a call to
`Controller_IsConnected`, the connected branch returns immediately. The
disconnected branch stores `0x80` to four byte outputs, zero to four more byte
outputs, zero to two 32-bit outputs, then writes six individual zero bytes to
the remaining output. The six byte stores occur *after* the 32-bit stores.
Matching that statement order and keeping six explicit stores reproduces the
target's 141-byte body with the NDK compiler. A direct object-to-library
GOT-aware diff reports 99.625%; the only observed differences are unresolved
PIC relocation operands. The target and direct object instruction layouts and
stack offsets are otherwise identical.

`GamePads_NetHost`, `GamePads_NetClient`, `GamePads_NetReset`, `Controller_Init`,
`Controller_Exit`, `Controller_Update`, and
`VirtualControlButton_OnDown_Callback` are true empty functions. At this
translation unit's `-O2`, an empty function compiles to eight one-byte NOPs
and `ret` (nine symbol bytes). Remove `STUBBED()` without adding manual NOPs.

## Space rumble

`SpaceRumbleProcess` casts twelve rays (six axial, six rotated) from the
current player position, tracks the nearest hit, then controls camera shake
and player rumble. The target is 929 bytes. Both its local origin vector and
matrix require 16-byte alignment. In a direct GCC 4.7 build, aligned locals
move the origin to `esp+0x40`, ray to `esp+0x50`, and matrix to `esp+0x60` and
recover the target's EBP/EDI/ESI/EBX frame shape. Plain `NUMTX` or `NUVEC`
locals produce a frameless function with different register allocation. The
axis component store order also changes the final code even when every ray
has the same values.
The target decrements `SpaceRumbleTimer` after the shake path as well as the
no-hit path; making the timer code a common tail recovers the target jump and
raises the direct object score from 88% to 95%. The occasional pulse passes
zero as the second `NewRumbleAllPlayers` float argument. Spell the strength
clamp with the constant as the first operand to produce `movss xmm1,[0.35];
minss xmm1,xmm0`. The remaining structural gap is the Player 1 fallback
block: target places it after the ray-switch blocks, but GCC puts it beside
the entry with the current source.

## Touch callbacks and pause

The D-pad down callback copies the difference between the element position
(`+0x8`, `+0xc`) and touch down position (`+0xc`, `+0x10`) to element scratch
fields `+0x80`, `+0x84`. The button mover callback uses `+0x7c`, `+0x80`.
Both compile to the target's exact 45- and 42-byte bodies; the ordinary
button callback is the nine-byte no-op above. These offsets should be checked
again when the shared `TouchHolder` type is changed on another branch.

`PerformPauseButtonStuff` uses one menu ID dispatch rather than a `switch`.
GCC 4.7 folds particular equalities into bitmask and range checks: target
compares `0xd, 1, 8, 0x11` in that order, folds `0x3f0/0x3f8` with
`value & ~8`, and folds `0x3f4/0x3f5` with unsigned `value - 0x3f4 <= 1`.
Equivalent reordered `||` expressions produce different compare sequences
and basic block placement.
The target cold trace order after the hot common return is Paused, close menu,
customizer, then PauseGame. Our current object puts PauseGame first. GCC 4.7
`bb-reorder.c` connects traces by branch frequency, so moving labels in source
alone does not correct the order. A `__builtin_expect` hint on
`GameMenuLevel == 0` raises the direct object score to about 63%, but the
cold-block order remains unresolved.

`GamePads_SkipMovie` actually returns `i32`, despite its old `void` stub
declaration. It returns one when either player presses skip and zero
otherwise. This correction yields the target's 183-byte instruction stream
in a direct NDK compile, apart from unresolved object relocations. The D-pad
lock click handler likewise has a target-sized, instruction-identical
140-byte body before linking.

## Pickup flicker

`UpdatePickupFlicker` derives its cycle length from `31.5f / FRAMETIME`,
divides by five for frame rates above 30, rounds the result up to an even
number, and sets the midpoint test to half that length. At 30 or fewer
frames, it selects a six-frame cycle and midpoint three. It increments or
wraps `PickupFlickerFrame` against the current `PickUpFlickerFrames` global.
The order of the independent constants three and six in the fallback block
affects whether GCC emits `mov ecx,3; mov edx,6` or the reverse order.
Empty inline assembly with input constraints `"a"(flicker_frames_ptr)` and
`"c"(flicker_test)` forces the target's register allocation and immediate
order without emitting an instruction. This is used only after confirming the
target's `GOT` load, `mov ecx,3`, `mov edx,6`, store sequence.
A plain `if (period % 2) ++period` becomes branchless `and/cmp/sbb` at `-O2`.
A zero-byte `asm volatile("")` in the odd branch preserves the target's
`test` and cold increment block without adding machine instructions.
Comparing the next frame against the just-written global, rather than the
local period, lets GCC forward the stored value and emit the target `cmovl`
frame-wrap sequence without another global load.
