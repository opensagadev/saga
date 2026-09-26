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
