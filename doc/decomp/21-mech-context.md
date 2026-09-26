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
