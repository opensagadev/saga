# Mech touch stub pass: measured batch

This batch removed 42 `STUBBED()` markers across the core touch source,
Bonus Cavalry, Death Star turret, Podrace, and JumpAutopilot. The measured
snapshot is commit `ea2ba3a9`, built with NDK r8e GCC using
`bazelisk build --config=target --jobs=2 //src:saga_target`. Percentages below
come from the GOT-aware fork of `objdiff-cli`, with `res/libTTapp.so` as target
and `bazel-bin/src/libTTapp.so` as base. Later tuning commits need new scores.

Of the 42 former stubs, 22 reached exact 100% in this snapshot. The exact
matches include the core no-op methods, two layout builders, the fake button
and dummy button constructors, 12 touch controller callbacks, and other
small helpers. Additional controller destructors and fake button
`IsPressed()` also matched exactly.

| Area | Remaining measured scores |
| --- | --- |
| Core touch | `MechAutoJumpGetBest` 76.98%; Console layout 96.05%; Gesture layout 98.83%; Podrace and Speeder layouts 94.15%; Faker Render 33.89%, Update 46.87% |
| Bonus Cavalry | constructor 82.45%; Update 34.57% |
| Death Star turret | constructor 83.22%; OnSwipe 91.67%; Update 94.01% |
| Podrace | Update 94.87% |
| JumpAutopilot | Analyse 39.27%; CalculateModified 72.65%; Bottom 15.07%; LandingPoint 78.92%; LandingSpot 54.92%; TerrInt 77.89%; OnProcess 1.58% |

The striking first-pass `OnProcess` result had a 663-byte target and a
663-byte current body, yet only 1.58% alignment. The target uses a frame
pointer, stack realignment, switch call blocks before its initialization
block, and one character pointer register across six velocity stores.
The first GCC output changed all four details, so size alone was weak
evidence of match.

## Second measured batch

The source-only tuning through commit `ee653f60` also passed the NDK r8e
target build. The same GOT-aware forked `objdiff-cli` measured 23 of the 42
former stubs at exactly 100%; Death Star `OnSwipe` is the new exact match.
The Bazel server was shut down after scoring. Full symbol scores were
saved outside the repository as `score-mech-2.txt` in the task workspace.

| Function or area | First pass | Second pass | Target/current bytes in second pass |
| --- | ---: | ---: | ---: |
| `MechAutoJumpGetBest` | 76.98% | 96.94% | 627/626 |
| Console layout | 96.05% | 99.99% | 464/464 |
| Podrace and Speeder layouts | 94.15% each | 99.99% each | 472/472 each |
| Faker `Render` | 33.89% | 33.89% | 1013/1117 |
| Faker `Update` | 46.87% | 62.26% | 301/327 |
| Bonus Cavalry `Update` | 34.57% | 68.99% | 1133/1128 |
| Death Star `OnSwipe` | 91.67% | 100% | 41/41 |
| Death Star `Update` | 94.01% | 98.49% | 606/606 |
| Podrace `Update` | 94.87% | 86.24% | 396/380 |
| Jump `AnalyseJumpTrajectory` | 39.27% | 49.02% | 336/336 |
| Jump `LookForBottomInt` | 15.07% | 76.85% | 182/182 |
| Jump `LookForTerrInt` | 77.89% | 61.27% | 307/307 |
| Jump `LookForLandingSpotAroundPoint` | 54.92% | 56.64% | 3072/2880 |
| Jump `OnProcess` | 1.58% | 48.28% | 663/708 |

The `OnProcess` branch correction changed its control-flow graph and
raised alignment substantially, but grew the body by 45 bytes. The
Bottom intersection OR comparison and store-order correction raised its
score by 61.78 points while retaining the exact 182-byte size. Podrace
`Update` and Jump `LookForTerrInt` regressed in this tuning pass and
should be revisited from their first-pass source if work resumes.

The three specialized touch controllers share the same ABI pattern:
`MechInputTouchMainController` followed by `MechInputTouchGestureTracker`,
then an active byte at `0x70` and a touch pointer at `0x74`. The old stub
declarations omitted both bases and declared several `bool` callbacks as
`void`, yet still linked because C++ return types are not encoded in these
method symbols. Check callers for `%al` tests when reconstructing a callback.

Further per-function findings are in
[`21-mech-core.md`](21-mech-core.md),
[`21-bonus-cavalry-controller.md`](21-bonus-cavalry-controller.md),
[`20-deathstar-touch-controller.md`](20-deathstar-touch-controller.md),
[`21-mech-podrace-controller.md`](21-mech-podrace-controller.md),
[`21-mech-jump-autopilot.md`](21-mech-jump-autopilot.md), and
[`mech-jump-landing-spot.md`](mech-jump-landing-spot.md).
