# Core touch input reconstruction notes

Target: `res/libTTapp.so`, compared with the GOT-aware `objdiff-cli` fork. These notes cover `MechInputTouch.cpp`, `MechInputTouchButton.cpp`, and `MechSystems.cpp`. Match scores for the new bodies are pending a combined build.

## Target no-ops

The target bodies for `MechInputTouchSystem::AddChangeLayoutButtons`, `CreateGamePlayLayoutBlank`, `MechInputTouchButton::Render`, `MechInputTouchButton::Update`, `MechInputTouchButtonControlled::ControlledRender`, and `MechSystems::Reset` are eight `nop` instructions followed by `ret` (nine bytes each). All were already 100% in the baseline report despite their `STUBBED()` markers. Removing the markers documents their actual behavior. `MechInputTouchMainDummyButton`'s constructor was also already 100%; its marker followed its complete member initializers.

## Layout builder pattern

The six substantive `CreateGamePlayLayout*` functions have the same sequence after controller construction. They append the controller to the selected device layout, then a `MechInputTouchMainDummyStick` of type `1`, then four `MechInputTouchMainDummyButton` objects with `(id, type)` pairs `(0x80, 0)`, `(0x20, 3)`, `(0x40, 2)`, `(0x10, 1)`. The type `1` stick reads the main controller's right-stick values. Each append writes to `layout.elements[layout.unknown_c8]` and increments the count without a bounds check.

The layout starts at device offset `0xd4 + index * 0xcc`, with count at layout offset `0xc8`. `NuVirtualTouchDevice` currently exposes these fields privately, so `GetTouchLayout` accesses the verified ABI offsets directly. All six builders call the target no-op `AddChangeLayoutButtons` first. GestureBased first calls `GetAspectRatio`; the target call passes no object argument, suggesting this member was originally static. Its current declaration is non-static, so check this when tuning that builder.

| Layout | Allocation | Controller constructor | Additional store |
| --- | --- | --- | --- |
| Console | `NU_ALLOC(0x98, 4, 1, "Main", 0)` | VirtualConsole `(0)` | None |
| GestureBased | `new`, size `0xb8` | GestureBased `(0, {0})` | `MechSystems::gesture_controller` |
| Podrace | `NU_ALLOC(0x78, 4, 1, "Main", 0)` | Podrace `(0)` | `gesture_controller` |
| Cavalry | `new`, size `0x78` | BonusCavalry `(0)` | `gesture_controller` |
| DeathStarTurret | `new`, size `0x7c` | DeathStarTurret `(0)` | `gesture_controller` |
| SpeederChase | `NU_ALLOC(0x80, 4, 1, "Main", 0)` | SpeederChase `(0)` | `gesture_controller` |

The `NU_ALLOC` cases check for null before invoking the constructor; plain `new` cases do not. The currently incomplete Podrace, Cavalry, DeathStar, and VirtualConsole class declarations do not express their target main-controller inheritance, so the layout builders preserve target object sizes and cast their base address for the dummy controls. Correcting those declarations and constructors in their owning files will improve runtime behavior and may change codegen here.

## Fake button and drawing

`MechInputTouchButtonFaker` inherits `MechInputTouchButton` in the target. Its constructor calls the seven-argument base constructor with button ID `0`, then clears `is_pressed` at offset `0x11d`. It overrides `IsPressed()` to return that byte. The target `Update` scans 24-byte touch entries from `NuInputTouchData`, requests a touch lock when a down touch falls inside its rectangle, sets `is_pressed` for a matching locked ID, and releases a vanished lock.

The target `Render` calls `NuRndrCircle` and `NuRndrRect` with zero UVs and `g_nuMtlHandleNull`. Circles use 64 segments and `width / height` aspect. Ordinary buttons draw a `0.8 * width` circle in their `id` color when released; when pressed, they first draw a white `width` circle, then a white `0.8 * width` circle. Button index `0x800` draws a white `0.9 * width` circle, a `0.7 * width` circle colored red or black by pressed state, and two white rectangles. Their positions are `(x + 0.3 * width, y + 0.25 * height)` and `(x + 0.6 * width, y + 0.25 * height)`; each is `0.15 * width` by `0.5 * height`. The four target globals absent from prior source are `colourWhite = 0xffffffff`, `colourBlack = 0xff000000`, `colourRed = 0xff0000ff`, and a zero-initialized `g_nuMtlHandleNull` pointer.

## Auto-jump selection

`MechAutoJumpGetBest` returns `MechAutoJumpConnection*`; the old `void` stub had the wrong return type. It exits unless touch controls are active and `WORLD->mech_auto_jump_manager` exists. Both search paths traverse `jump_connections` and require `allow_streak` and `use_path_direction` on each candidate. For packets other than type `3`, the input angle is the low 16 bits of the second argument, with a strict best-difference threshold of `0x2aab`; candidate rotation adds `0x8000` when connection direction is nonzero. Type `3` derives a swipe angle from packet end minus start, projects both path nodes to screen coordinates, and uses a `0x4000` threshold. The comparison is `abs(RotDiff(candidate, desired)) < best_difference`, so ties keep the first candidate. Node indices are selected by `direction` and `direction == 0`.

## Baseline before reconstruction

The whole-game GOT-aware report before this pass measured AutoJump at 2.56%, six substantive layout builders at 4.04–4.52%, Faker constructor at 14.48%, Faker Render at 2.23%, and Faker Update at 5.12%. The no-ops and MainDummyButton constructor were already exact. Rebuild and rerun the GOT-aware report after integration to capture the actual gains.
