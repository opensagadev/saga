# Gizmo stub recovery and GCC patterns

These notes cover `src/legoapi/gizmo/base/gizmo.cpp` and `gizactions.cpp` in the
Android NDK r8e GCC 4.7 target. Measure each function with the forked GOT-aware
`objdiff-cli`, using `res/libTTapp.so` and `bazel-bin/src/libTTapp.so`.

## Recovering local globals through the GOT

The target's local GOT entries often use `R_386_RELATIVE`, so `readelf -r` does
not print a symbol name. Dump the GOT word and look up its value in
`readelf -Ws` before naming a field. For example, the two GOT loads in
`TeleportObjectInterface::TargetedFlash` read words at `0x615f8c` and
`0x615f94`; the entries hold `0x01268860` (`hackFlashTimer`) and `0x01268870`
(`hackFlashingSpecial`). The function sets the timer to `1.0f` and chooses
`teleport.flap1_special` or `flap2_special` from the interface index.

The same method recovered the paint puzzle globals at `0x6a9810` through
`0x6a9930`. Their exported sizes distinguish scalars, three-element arrays,
six-element arrays, and 12-byte `nuhspecial_s` arrays. Resolve the GOT base
from the return address of `__x86.get_pc_thunk.bx`, not from the following
`add` instruction: using the latter shifts every computed address by the size
of the `add` instruction.

## Static callback table copied to the stack

`RegisterGizmoTypes_Batman` copies 29 words from `.data` at `0x623040` to a
stack array using `rep movs`, then passes it to `RegisterGizmoTypes` with
count parameter `12`. The 29 words are 28 registration callbacks and a final
null pointer. A file-scope callback array followed by `memcpy` expresses this
pattern; an array built from assignments inside the function generates a
different instruction sequence. The Batman table differs from the LSW table,
including `Shards`, `Signals`, `TightRopes`, `Ledges`, `SecurityDoors`,
`Attractos`, and `GuideLines`.

## Control flow and literal details

- `ResetPaintPuzzle` computes a paint index as signed `qrand() / 21846`. GCC
  lowers this constant division to a signed magic multiply (`0x2fffa001`),
  arithmetic shifts, and sign correction. Keep the division in source.
- `InitPaintPuzzle` contains fixed groups of six and three gizmo/special
  lookups. At `-O3`, GCC unrolls these short loops, so explicit calls and
  fixed-size loops may both be worth measuring before hand-tuning block order.
- `UpdatePaintPuzzle` is a five-state switch. Keep the common network packet
  writes after the switch, including the default path, to preserve its shared
  tail. State 3 has two phases: reverse the two source paint animations and
  start the mixed animation, then wait for the mixed animation to reach its
  end state.
- `GizAction_ChangeTechnoTgt` calls `NuStrICmp` with the result of a failed
  `NuStrIStr("target=")` for four movement options. That result is null and
  `NuStrICmp` returns `-1`, making those options ineffective in the target.
  Preserve the call sequence when matching. Its `NO_TARGET` branch clears the
  old target and then enters the ordinary retarget path; the final fallback
  writes the requested kind (`0xff`) to `target_mode`.
- `GizmoGetNumOutputs` returns an integer: `0` for missing global types,
  gizmo, or system; `1` for an invalid type or missing callback; otherwise it
  calls the type callback. Its former `void` declaration hid the real return
  value consumed by `GizmoSysWriteInfo`.
- `GizmoSysWriteInfo` writes the length including the null byte before each
  name. The type prefix and the extra `GizSpecial` output count use one-byte
  lengths. The writer copies an existing file to `path.bak` with
  `NuFileCopy(backup, path)` before opening the new file.

## Distinguishing padding from behavior

`GizmoGetGuid` returns `-1`, and `DefaultGizmo_GetOutput` returns `0`; their
target bodies are constants with padding. Remove the misleading `STUBBED()`
diagnostics without inventing extra behavior.

## Initial match measurements

These are GOT-aware `objdiff-cli diff` scores against the first complete stub
pass. They give the next agent a concrete starting point for codegen tuning.

| Function | Target bytes | Match |
| --- | ---: | ---: |
| `DefaultGizmo_GetOutput` | 9 | 100% |
| `RegisterGizmoTypes_Batman` | 127 | 99.95652% |
| `TeleportObjectInterface::TargetedFlash` | 68 | 100% |
| `GizmoGetGuid` | 12 | 100% |
| `GizmoSys_BoltHit` | 443 | 20.516129% |
| `ResetPaintPuzzle` | 206 | 96.796875% |
| `InitPaintPuzzle` | 758 | 99.94505% |
| `UpdatePaintPuzzle` | 1334 | 42.588234% |
| `GizmoSysWriteInfo` | 1064 | 65.423485% |
| `GizmoGetNumOutputs` | 119 | 99.5946% |
| `GizActions_ActivateBelt` | 178 | 96.39286% |
| `GizActions_PlayCutscene` | 161 | 99.652176% |
| `GizActions_PlayRadio` | 309 | 93.141304% |
| `GizActions_PlayObstacle` | 547 | 86.35811% |
| `GizAction_SetAIState` | 897 | 26.959822% |
| `GizAction_ChangeTechnoTgt` | 943 | 51.333332% |
| `GizAction_ActivatePartEffect` | 220 | 87.52% |
| `GizAction_SetPickupVisibility` | 511 | 51.0% |
| `GizActions_CompleteLevel` | 377 | 99.98214% |

The low scores mostly reflect block order and load/register choice after
functional reconstruction; do not treat them as verified 100% matches.

## Follow-up control-flow findings

- `GizmoSys_BoltHit` passes `points + 1` to a type's `bolt_hit_fn` when flag
  `0x200` is set, but passes the original `points` to `BoltSys->debris` after a
  hit. Keep two pointers. Reassigning the parameter produces the wrong debris
  argument and changes the register lifetime. The target initially keeps the
  bolt in `edx` and gizmo system in `edi`; an empty output-constrained asm at
  function entry steers GCC r8e toward these choices, raising the match from
  26.15% to 27.58% in this source layout.
- In `UpdatePaintPuzzle`, declare the network packet pointer after the switch
  and write state 4's byte directly through the global packet pointer. This
  gives GCC a switch jump table and shared packet tail like the target. The
  target's state 2 records all three completed paint indexes into a two-slot
  array without a bounds check. It only chooses a blend when the count is
  exactly two, and handles both orders of each paint pair. Keeping those
  details raised the match from 42.59% to 59.56%.
- `GizAction_SetAIState` copies `v000` into its local origin before parsing
  parameters, even though early exits do not use it. Moving this initialization
  ahead of the parser raised the match from 26.96% to 27.52%.

The measured follow-up scores use the same GOT-aware CLI and target build:
`GizmoSys_BoltHit` 27.580645% (429 local bytes), `UpdatePaintPuzzle`
59.564705% (1257 local bytes), and `GizAction_SetAIState` 27.522322%
(857 local bytes). Other entries in the initial table remain at their stated
scores.

`GizmoSysWriteInfo` has several branch and call details hidden by the file
format. It uses `strlen` for ordinary type, gizmo, output, and scene special
names, but `NuStrLen` for type/special prefixes and the extra `GizSpecial`
outputs. It tests each computed length before `EdFileWrite` except for scene
special names, even after adding one for the terminator. It calls
`NuGScnNumSpecials(scene)` once and reuses that count. After writing the gizmo
type count it skips loading `gizmo_sys->sets` when the count is exactly zero;
for a negative count it still loads the sets pointer, then exits the type loop.
Reproducing these details raised the GOT-aware match from 65.423485% to
69.14235% (1112 local bytes). The remaining large prologue difference is a
stack realignment in the local build whose trigger is not yet confirmed.

`GizAction_SetPickupVisibility` writes three fields from the pickup's state
byte separately: `state_enabled`, `state_visible`, and `state_activated`. GCC
emits two masks and shifts that do not arise from a single combined `0x86`
mask expression, despite equivalent ordinary flag behavior. The target also
dereferences `WORLD->gizmo_pickup_sys` without a null check and advances a
pickup pointer by the 44-byte struct size each loop iteration. Using the
bitfields and pointer walk raised its GOT-aware score from 51.0% to 81.12%
(497 local bytes versus 511 target bytes). An explicit null check for each
loop pointer reduced the score, so that hypothesis was discarded.

`GizAction_SetAIState` checks every parameter for `Character` and then checks
the same parameter for `range` independently. Only `range`, `type`, and
`State` form an `else if` chain. Turning the `Character` check into part of
that chain skipped target calls and reordered most of the parser. The target
also rejects global type ID `0xff`, rather than `-1`. Correcting both details
raised its GOT-aware match from 27.522322% to 67.99107% (904 local bytes
versus 897 target bytes).

In the object scan, the target computes all three squared components of the
`NuVecSub` result before scanning the type IDs. GCC otherwise delays the
floating-point arithmetic until after the type scan because only matching
objects use the distance. An empty `+x` output constraint on the computed
distance keeps the arithmetic before that scan without adding instructions;
the match rose to 69.4375% with the same 904-byte local size. Forcing the type
index into `eax` reduced the score and was not retained.

`GizActions_PlayObstacle` clears runtime flag bits `0x0c`, then applies the
parsed stay-shut bit at position 3 and stay-open bit at position 2. Expressing
the update through a byte temporary with those operations in sequence raised
the GOT-aware match from 86.35811% to 86.77027% (523 local bytes).

`UpdatePaintPuzzle` already has its five major switch blocks in target order.
Its state-2 loop over three source paint obstacles also unrolls as in the
target. The next mismatch was eager pointer loading: panel outputs are checked
before dereferencing their corresponding obstacles, and state 3 reads the
second and mixed obstacle only after reaching those branches. The common
packet tail tests byte 2 before overwriting bytes 0, 1, and 3. Delaying those
dereferences and capturing the packet test first raised the GOT-aware match
from 59.564705% to 72.91765% (1273 local bytes versus 1334 target bytes).
The stage-3 success transition uses an explicit branch in the target, but a
literal source `if/else` lowered the direct-object score and was reverted.

The blend gate's predicate order matters. For the orange pair, test
`b == 1 && a == 0` before `b == 0 && a == 1`; for green, test
`a == 2 && b == 1` before `b == 2 && a == 1`; for purple, test
`b == 2 && a == 0` before `a == 2 && b == 0`. GCC then caches and reuses
equality results like the target, rather than generating a separate decision
tree. In the common packet tail, read `painttry` and `painttarget` before the
packet pointer. Duplicating the three packet writes in both branches of a
`packet[2] == 1` test lets GCC merge the stores while retaining the compare
before them and branching on its flags afterward. Put the sound effect test
after those branches; moving it inside the first branch changes the block
layout. These changes raised the direct NDK r8e object comparison from
72.22647% to 84.16765% (1283 local bytes). The direct object baseline for
the prior linked 72.91765% version was 72.22647%; the final linked score for
this follow-up source remains to be measured.
