# Core touch input reconstruction notes

Target: `res/libTTapp.so`, compared with the GOT-aware `objdiff-cli` fork. These notes cover `MechInputTouch.cpp`, `MechInputTouchButton.cpp`, and `MechSystems.cpp`.

## Target no-ops

The target bodies for `MechInputTouchSystem::AddChangeLayoutButtons`, `CreateGamePlayLayoutBlank`, `MechInputTouchButton::Render`, `MechInputTouchButton::Update`, `MechInputTouchButtonControlled::ControlledRender`, and `MechSystems::Reset` are eight `nop` instructions followed by `ret` (nine bytes each). All were already 100% in the baseline report despite their `STUBBED()` markers. Removing the markers documents their actual behavior. `MechInputTouchMainDummyButton`'s constructor was also already 100%; its marker followed its complete member initializers.

## Layout builder pattern

The six substantive `CreateGamePlayLayout*` functions have the same sequence after controller construction. They append the controller to the selected device layout, then a `MechInputTouchMainDummyStick` of type `1`, then four `MechInputTouchMainDummyButton` objects with `(id, type)` pairs `(0x80, 0)`, `(0x20, 3)`, `(0x40, 2)`, `(0x10, 1)`. The type `1` stick reads the main controller's right-stick values. Each append writes to `layout.elements[layout.unknown_c8]` and increments the count without a bounds check.

The layout starts at device offset `0xd4 + index * 0xcc`, with count at layout offset `0xc8`. `NuVirtualTouchDevice` currently exposes these fields privately, so `GetTouchLayout` accesses the verified ABI offsets directly. All six builders call the target no-op `AddChangeLayoutButtons` first. GestureBased first calls `GetAspectRatio`; the target call passes no object argument. Declaring that member `static` removes the one extra stack store in the reconstructed GestureBased builder.

| Layout | Allocation | Controller constructor | Additional store |
| --- | --- | --- | --- |
| Console | `NU_ALLOC(0x98, 4, 1, "Main", 0)` | VirtualConsole `(0)` | None |
| GestureBased | `new`, size `0xb8` | GestureBased `(0, {0})` | `MechSystems::gesture_controller` |
| Podrace | `NU_ALLOC(0x78, 4, 1, "Main", 0)` | Podrace `(0)` | `gesture_controller` |
| Cavalry | `new`, size `0x78` | BonusCavalry `(0)` | `gesture_controller` |
| DeathStarTurret | `new`, size `0x7c` | DeathStarTurret `(0)` | `gesture_controller` |
| SpeederChase | `NU_ALLOC(0x80, 4, 1, "Main", 0)` | SpeederChase `(0)` | `gesture_controller` |

The `NU_ALLOC` cases check for null before invoking the constructor; plain `new` cases do not. The currently incomplete Podrace, Cavalry, DeathStar, and VirtualConsole class declarations do not express their target main-controller inheritance, so the layout builders preserve target object sizes and cast their base address for the dummy controls. Correcting those declarations and constructors in their owning files will improve runtime behavior and may change codegen here.

For the `NU_ALLOC` cases, preserve the allocator result as a typed controller pointer and use placement construction only when it is non-null. Initializing a separate controller pointer to null before allocation adds `xor edi, edi`, a later register move, and a three-operand `imul` for the layout index. The target holds the allocation result in `edi` directly and uses `imul ebp, 0xcc` for the layout address.

## Fake button and drawing

`MechInputTouchButtonFaker` inherits `MechInputTouchButton` in the target. Its constructor calls the seven-argument base constructor with button ID `0`, then clears `is_pressed` at offset `0x11d`. It overrides `IsPressed()` to return that byte. The target `Update` scans 24-byte touch entries from `NuInputTouchData`, requests a touch lock when a down touch falls inside its rectangle, sets `is_pressed` for a matching locked ID, and releases a vanished lock.

The target `Render` calls `NuRndrCircle` and `NuRndrRect` with zero UVs and `g_nuMtlHandleNull`. Circles use 64 segments and `width / height` aspect. Ordinary buttons draw a `0.8 * width` circle in their `id` color when released; when pressed, they first draw a white `width` circle, then a white `0.8 * width` circle. Button index `0x800` draws a white `0.9 * width` circle, a `0.7 * width` circle colored red or black by pressed state, and two white rectangles. Their positions are `(x + 0.3 * width, y + 0.25 * height)` and `(x + 0.6 * width, y + 0.25 * height)`; each is `0.15 * width` by `0.5 * height`. The four target globals absent from prior source are `colourWhite = 0xffffffff`, `colourBlack = 0xff000000`, `colourRed = 0xff0000ff`, and a zero-initialized `g_nuMtlHandleNull` pointer.

## Auto-jump selection

`MechAutoJumpGetBest` returns `MechAutoJumpConnection*`; the old `void` stub had the wrong return type. It exits unless touch controls are active and `WORLD->mech_auto_jump_manager` exists. Both search paths traverse `jump_connections` and require `allow_streak` and `use_path_direction` on each candidate. For packets other than type `3`, the input angle is the low 16 bits of the second argument, with a strict best-difference threshold of `0x2aab`; candidate rotation adds `0x8000` when connection direction is nonzero. Type `3` derives a swipe angle from packet end minus start, projects both path nodes to screen coordinates, and uses a `0x4000` threshold. The comparison is `abs(RotDiff(candidate, desired)) < best_difference`, so ties keep the first candidate. Node indices are selected by `direction` and `direction == 0`.

The target reloads `WORLD->mech_auto_jump_manager->jump_connections` for every `NuLinkedListGetNext` call. Caching a `NULISTHDR*` changes the saved register choice, reduces each loop by three instructions, and shifts the stack frame from `0x4c` to `0x5c` after later spills. In the swipe branch, it computes both node-position pointers before either `NuCameraTransformScreenClip` call. Computing the second pointer after the first call spills path, connection, and direction, adding about 16 instructions. Keep the order explicit while tuning.

## Measured match and compiler patterns

The whole-game GOT-aware report before this pass measured AutoJump at 2.56%, six substantive layout builders at 4.04–4.52%, Faker constructor at 14.48%, Faker Render at 2.23%, and Faker Update at 5.12%. The no-ops and MainDummyButton constructor were already exact. Rebuild and rerun the GOT-aware report after integration to capture the actual gains.

The first combined build measured AutoJump at 76.98%; Console at 96.05%, GestureBased at 98.83%, Podrace and SpeederChase at 94.15%, and Cavalry and DeathStarTurret at 100%. Faker constructor and `IsPressed` are 100%; Faker Render is 33.89% and Update is 46.87%. The remaining ordinary no-ops are 100%. These scores precede the control-flow refinements in `b90d96f3` and the AutoJump pointer-order change.

For Faker Update, indexing `touch_events[i]` made GCC recalculate `data + i * 24` and reload the touch count after calls. The target keeps a byte cursor in `esi`, advances it by `0x18` with `lea`, and keeps an integer loop counter in `edi`. Its loop compares `i == count` after increment, so `i != count` in source preserves the equality branch; `i < count` generated a different unsigned range branch. It also shares the vanished-lock cleanup between a zero-touch path and the loop exit. A byte cursor rooted at the `NuInputTouchData` address preserves target offsets: the event state is cursor `+6`, coordinates are `+8`/`+0xc`, and touch ID is `+0x18`.

For Faker Render, the target block order is ordinary pressed, ordinary released, then special index `0x800`. Writing the special case as the lexical fallthrough made GCC place it before both ordinary paths and enlarge the function. Both index checks now jump to a shared special block, leaving the ordinary draw calls in target order. The final colored circle uses red as its fallthrough color and branches to black at the end of the function.

## `FindTargetObject` block layout

The first combined GOT-aware report measures this 7,833-byte target function at 50.81% with a 7,570-byte local body. Its 44 direct calls have the same callee counts in both binaries, including 12 `CalcCapsuleIntersectDistance` calls. The main discrepancy is layout and stack allocation: the target reserves `0x220` bytes and the local build reserves `0x230`. The target's ray vectors occupy stack offsets `0x150`/`0x160`/`0x170`; the local vectors occupy `0x160`/`0x170`/`0x180`. The target spills `WORLD` at `0xb8`, versus the local `0xc4`. This one-frame shift affects hundreds of argument comparisons.

The target initializes `best_distance` with an immediate `25.0f` store before testing `VehicleArea`, then stores `5000.0f` on the true path. A ternary expression generated a post-test XMM constant load; an explicit initial assignment and `if` reproduces the immediate store order under NDK r8e GCC 4.7.

The blowup loop is especially sensitive to branch prediction. The target branches out of line on a nonzero `field_0x124`, lets the box bounds calculation fall through, and calls `CalculateRayBoxIntersection` near function offset `0x03c8`. The prior local build kept the sphere calculation inline and moved the box call near `0x150a`. Swapping the `if (sphere)` arms or hinting the aggregate `sphere` value did not fix the first predicate's branch direction. Marking **the first predicate itself** as unlikely with `__builtin_expect(item->field_0x124 != 0, 0)` changed the local `je` to the target's `jne` and moved the box call into the early trace in a standalone NDK r8e compile. This illustrates that GCC's trace formation responds to the edge on the original short-circuit test, not necessarily to a hint on the combined Boolean result.

The target also advances both a blowup item pointer and a pointer to its midpoint z component by `0x12c` each loop. The source now spells out that midpoint cursor, which gives the standalone compile the same parallel-pointer shape. The object-to-target GOT-aware score for the two source refinements and this cursor rose from 50.56% for the previously built object to 51.72% in the standalone diagnostic object. A linked build is needed for the authoritative score. The `0x230` stack frame remains in the diagnostic output; tracing the extra 16 bytes is still open.
