# Jump autopilot code generation notes

`src/MechInputTouch/MechJumpAutopilotAddon.cpp` is built at `-O3` with the
NDK r8e x86 GCC 4.7 compiler. Inspect this file with the forked GOT-aware
`objdiff-cli`; the linked `.so` has different GOT displacements from the
reference.

## Floating-point OR and unordered comparison

In `LookForBottomInt`, the reference divides the vertical displacement to
get a fraction `t`. It compares `t` with zero and branches to a cold block
unless `t > 0`. The cold block compares one with `t` and branches back to
the intersection body when `t <= 1`. This is the source condition
`t > 0.0f || t <= 1.0f`. It accepts every ordered finite value, but returns
for NaN because both relational tests are false. GCC retains both checks
and places the second one after the body. Replacing the OR with the more
plausible `t > 0 && t <= 1` adds a separate zero test before the hot body,
changes block order, and changes behavior. Check the actual constants in
the target GOT: `-0xa0164` from `0x616870` is zero; `-0xa01e8` is one.

## Frame and cold block layout

The reference `OnProcess` begins `push ebp; mov ebp,esp; lea esp,-0xc;
and esp,-16; lea esp,-0x10`, then saves `esi`, `ebx`, and `edi` in frame
slots. The prior reconstruction omitted the frame pointer and stack
realignment. GCC's `optimize("no-omit-frame-pointer")` and
`force_align_arg_pointer` function attributes are a candidate for that
prologue. The reference places five switch case calls before the cold
initialization block, after the normal return. A rare-branch
`__builtin_expect(..., 0)` on first jump initialization is a candidate
to recover that trace order. These source probes require a compiled
GOT-aware diff before treating them as matched.

At the end of initialization, the reference squares vertical velocity,
XORs its sign bit, divides by twice gravity, then **adds** position Y.
Spell the expression `position.y + (-(vy * vy)) / (gravity + gravity)` to
preserve the target's operation order; `position.y - (vy * vy) / ...`
selects a later subtraction.

The reference movement block keeps `character` in `edx` while writing X
speed to movement direction, target velocity, and actual velocity, then
computes and writes Z speed to the same three destinations. The previous
build reloaded `character` several times between those stores and
computed Z before the X stores. A block-local `GameObject_s *` cache is
being tested to retain the target pointer reuse. The reference also
checks `state <= 5` before the velocity stores, even though the jump
table dispatch comes afterward; this is GCC's instruction scheduling,
not a different state machine.

## State-zero branch reachability

At `0x44afd5`, the reference checks `state`. A nonzero state runs the
velocity stores and dispatches through the switch; each case at
`0x44b090` through `0x44b110` jumps straight to the common `started`
update at `0x44b000`. Only `state == 0` reaches the context test at
`0x44afe6` and the initialization block at `0x44b140`. Placing the
initialization test after the switch makes every case fall into that
test, changing both reachability and GCC's block order. The source now
routes nonzero states directly to the common tail. Its effect on the
match percentage is pending the next GOT-aware build.
