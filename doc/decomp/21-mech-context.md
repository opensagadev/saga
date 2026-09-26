# Mech touch context task matching notes

Target: `res/libTTapp.so`, i386 GCC from Android NDK r8e. Measure with the GOT-aware `objdiff-cli` fork; ordinary byte comparisons misclassify local GOT displacements.

## Planned movement task ABI

`MechTouchTaskPlannedGoTo` derives from `MechTouchTask`, despite its earlier placeholder declaration. The target allocates `0x710` bytes. Useful offsets:

| Offset | Meaning |
| --- | --- |
| `0x18` | Path analysis state (`0`, `1`, `2`) |
| `0x1c` | Heap array of 16-byte path samples |
| `0x4c` | Optional `MechTouchTaskGoTo *` |
| `0x50` | Embedded `MechTempPosInterface` |
| `0x6c` | `NuMechPtr<MechObjectInterface, 4>` target |
| `0x78` | 32 waypoint records, stride `0x34` |
| `0x6f8` | Current waypoint index |
| `0x6fc`–`0x6ff` | Four independent state bytes |
| `0x700` | External completion flag pointer |
| `0x704` | `NuMechPtr<MoveToMarker, 4>` |

Each waypoint record starts with a byte flag and padding, a `VuVec` at `+4`, an integer at `+0x14`, and an embedded `MechTempPosInterface` at `+0x18`. The constructor initializes all 32 records in a `0x34`-stride loop. The repeated vtable writes seen in that loop are the inline construction of `NuMechPtr_ManagedObject`, `MechObjectInterface`, then `MechTempPosInterface`, not three separate interface objects.

## Compiler and source patterns

- Assignment of `NuMechPtr<T, 4>` compiles into lengthy inline linked-list detach/attach control flow. `MechTouchTaskPlannedDoubleClickGoTo::OnStart` contains two such blocks for `move_to_marker`; use the typed member assignment rather than hand-coded list operations.
- `MechTouchTaskPlannedGoTo::OnResume` compares horizontal squared distance to `0.01f` with `ucomiss` and a strict `ja` branch. `BackgroundProcess` uses `0.25f` with the same strict comparison. Preserve comparison order to keep equality and NaN behavior.
- `MechTouchTaskPlannedGoTo::SetupForAnalysis` calls `NuCeil` on horizontal squared distance divided by `1.21f`, converts its float return to integer, then uses at least 5 or 10 samples. Its sample allocation is `(sample_count + 1) * 16` bytes; watch the inclusive initialization loop.
- `MechTouchTaskPlannedDoubleClickGoTo::OnStart` branches at horizontal squared distance `4.0f`. Its long size includes marker pointer bookkeeping and construction of the `0x710`-byte planned task.
- `MechInputTouchGestureBasedController::TriggerJumpTask` and `StartJumpUsingAIPath` both return `bool` in the target. Their old `void` declarations hid a branch in planned movement that restores the character velocities when the jump trigger fails. `MechAutoJumpGetBest` similarly returns a `MechAutoJumpConnection *` despite an old `void` stub.
- `JumpTriggerPacket::field_4[0]` contains the character pointer and `field_4[1]` contains the touch holder pointer. The packet's embedded velocity has `w=1.0f`; its bytes at `+0x1c` contain a full `VuVec` destination.
- Path samples use `-1000000000.0f` in their Y component as an unvisited sentinel. `AnalysePath` advances through at most three inclusive indices per call, while `GenerateWaypoints` pairs records around gaps larger than `0.15f`. The `0x34`-byte records duplicate position into both the outer `VuVec` and the embedded temporary interface.
- The target `PlannedGoTo::Update` compares its unsigned analysis state with `1` using `jbe`; a signed field emits `jle` even though current state values are only `0` through `2`. Keep the field unsigned.
- The target jump paths test `distance_squared > 1.96f || vertical_delta > scaled_height` with `ucomiss distance, threshold` and `ja`. Reversing the condition changes both block order and unordered float behavior. In `Update` and `PlannedDoubleClickGoTo::OnResume`, the two jump helpers write distinct stack return slots, and their components then flow as scalar SSE values through `NuAtan2D`, character velocity stores, and packet construction. A shared `VuVec` return variable produces a large run of integer vector copies instead.
- A waypoint reference retained across `TouchHacks::CanJump` changes register allocation in `Update`. The target reads the active byte and action before that call, then recomputes the waypoint address afterward.
- The target planned-task constructor constructs all embedded waypoint interfaces and the marker pointer before it writes several simple fields (`0x6fd`, `0x6fc`, `0x6fe`, `0x1c`, `0x700`, `0x4c`). Put scalar initialization in the constructor body when matching that ordering. Within each waypoint record, the embedded interface is constructed before the outer active flag, position, and action are filled.

For the GCC 4.7 branch prediction and block layout algorithm behind these control-flow mismatches, see [22-gcc47-control-flow.md](22-gcc47-control-flow.md).

## First implementation scores

GOT-aware measurements against a target build after the initial reconstruction:

| Function | Match |
| --- | ---: |
| `PlannedGoTo::OnStop` | 100% |
| `PlannedGoTo::OnStart` | 85.51% |
| `PlannedGoTo::~PlannedGoTo` | 79.19% |
| `PlannedGoTo::BackgroundProcess` | 74.45% |
| `PlannedGoTo::OnResume` | 63.63% |
| `PlannedGoTo::SetupForAnalysis` | 56.98% |
| `PlannedGoTo` constructor | 56.55% |
| `PlannedDoubleClickGoTo::OnResume` | 60.58% |
| `PlannedDoubleClickGoTo::OnStart` | 55.11% |
| `PlannedGoTo::Update` | 23.60% |

The task is functional source reconstruction, but the long routines still have control-flow and register differences. These percentages measure function similarity, not runtime correctness.
