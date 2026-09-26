# Mech jump landing spot: nine ray probes

`MechJumpAutoPilotAddon::LookForLandingSpotAroundPoint(VuVec const&)` is a
0xc00-byte function in the Android x86 target at `0x44a170`. Its apparent
size comes mostly from nine fully expanded copies of one ray test. A small
replacement loop may reproduce the game behavior but match little of the
instruction stream if NDK r8e GCC leaves the loop rolled.

The function constructs nine 16-byte `VuVec` offsets on the stack. The first
is `VuVec_Zero`. The next eight use `NuTrigTable` at successive 45-degree
angles. Their x component is the sine, their z component is the cosine, and
their radii alternate `0.2f` and `0.5f`. The target reads table elements
`0x1000` through `0x7000` and `0x0000` directly rather than calling a
trigonometric function.

Each probe starts at `(point.x + offset.x,
field_24.y + ((3 * field_94 + 0.5f) - 0.5f), point.z + offset.z)`.
Its downward displacement is `-(3 * field_94 + 0.5f)`. The two arithmetic
steps in the start height are visible in the target assembly, so they should
not be algebraically simplified when matching float code. The function calls
`GameRayCast` with radius zero
and mask zero, then adds the possibly modified displacement to the start
position to obtain the hit. A hit sets the boolean return value even when it
does not beat the previous score.

The score is
`((hit.x - field_24.x)^2 + (hit.z - field_24.z)^2) *
(hit.y - field_24.y + 0.1f)`. The initial best score is `-2000000000.0f`.
On a strictly greater score, the target stores `(hit.x, hit.y, hit.z, 0)`
at object offset `0x84`. It uses `ucomiss`/`jbe`, so equality and NaN do
not replace the best hit.

Compiler details to retain when tuning the match:

- `VuVec_Zero` is read from the translation unit's local 16-byte BSS object;
  replacing it with literal zeros can alter the setup block.
- The displacement vector has `w = 1.0f`, while the selected landing point
  has `w = 0.0f`.
- The original has a 16-byte aligned stack frame and 16-byte offset stride.
- The original places a shared `found` byte and best-score float in the
  frame, and later probes compare against that float.
- Set `found` after the score check in source. The target duplicates the
  store to this byte across the winning and losing score branches; putting
  it before the comparison can move the store ahead of `ucomiss`.
- Branches for a hit whose score is no greater than the best go to cold
  blocks after the main sequence. This layout matters to objdiff's block
  alignment and may depend on the original GCC optimization level.
