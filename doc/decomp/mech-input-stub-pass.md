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

The striking `OnProcess` result has a 663-byte target and a 663-byte current
body, yet only 1.58% alignment. The target uses a frame pointer, stack
realignment, switch call blocks before its initialization block, and one
character pointer register across six velocity stores. The current GCC
output changes all four details, so size alone is weak evidence of match.
Its score should be revisited after block order and prologue work.

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
