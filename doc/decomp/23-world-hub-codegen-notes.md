# World hub matching notes (Android NDK r8e x86 GCC 4.7)

These observations come from `res/libTTapp.so` and the GOT-aware `objdiff-cli` fork. They apply to `src/legoapi/world/levels.cpp` and `src/legoapi/world/levels/hub.cpp`.

## Short stubs and alignment

The target `GetTableLocator` and `GetCounterLocator` return zero; `OffPlat` and `Hub_CallBarman` return without side effects. The three `Hub*Unlocked` functions return true. Their eight inserted NOPs are function-entry padding, not omitted gameplay logic. Removing `STUBBED()` from the corresponding source bodies preserves the code path because the target build defines that macro to nothing.

## Hub bonus menu state

`MenuDrawBonusMode` shares the broad rendering sequence of `MenuDrawSelectMode`, but its fade gate checks `FadeSys.fade` for bonus modes 1 through 4. It draws arcade stats when `bonusmodearcade` is set, episode bonus stats when `hub_bonusepisode != -1`, otherwise super bonus stats. It then draws player 0's icon, the menu entries, and prompts. The normal prompt alpha is `0.333f`; it becomes the menu alpha when the arcade play row is selected and its area exists. Copying the select-mode prompt call verbatim misses that condition.

Do not leave `__used__` on a static helper after restoring its real caller. With `__used__` on `Hub_DrawBonusModeMenu`, GCC uses stack arguments from `MenuDrawBonusMode`; the target passes the selected index in `EAX` and alpha in `XMM0`. Removing that one attribute raised `MenuDrawBonusMode` from 85.59% to 87.44% in the GOT-aware object diff and brought candidate size from 990 to 971 bytes against the 974-byte target. The same issue can affect other formerly uncalled static stubs.

At the draw prompt gate, `if (MainRenderTime > 0.0f) return;` produces the target's `ucomiss` direction. Folding this with the following mode/alpha check produces the opposite comparison and loses a few matching instructions. Moving the prompt alpha declaration or converting its area test to a ternary did not affect GCC's output. The target loads default prompt alpha into `XMM0` before testing arcade mode, while the current candidate defers that load and uses `XMM2`; the underlying prompt values and calls agree.

`MenuUpdateBonusMode` has a dense five-way switch. Mode 0 processes input; mode 1 starts Super Story after a timer; modes 2 and 3 both build the free-play model list; mode 4 wipes back to the hub. The target decrements `BlipL`, `BlipR`, `BlipU`, and `BlipD` for both players before the switch. These are eight `float` values in four two-element arrays. The input branch checks controller 0, controller 1, then the active `MENU`. This order affects which input wins when more than one arrives during the same frame.

The arcade row data has an eight-byte stride within `ARCADEITEM_s`: the signed choice index is at byte 4 and the unsigned choice count at byte 5. Its left/right choices wrap around. Touch menu input maps the menu's left event to the choice-forward flag and its right event to the choice-backward flag in the target. Preserve that mapping when simplifying control flow.

## Arcade progress loop

`Hub_DrawArcadeStats` counts up to twelve `ArcadeLevel` entries. GCC at `-O3` fully unrolls the fixed-count loop in the target, so a hand-written twelve-way branch tree is unnecessary at first. An entry with `area->flags & 0x800` does not contribute to the total; the target tests byte `0x7b` because `AREADATA_s::flags` starts at `0x7a`. A contributing entry is counted as completed when its save has `area_complete` or its `challenge_trial_time` threshold exceeds the saved time. Cast the unsigned 16-bit threshold through signed `int` before converting to `float`: the target emits a single `cvtsi2ss` rather than GCC's longer unsigned conversion sequence. The final count feeds `Hub_DrawImportantBrick(211, ...)`. The title is drawn even without `Game_AreaSave`; then the counts are zero. A two-player warning is drawn only in the no-menu state when `Arcade_BothPlayersActive()` is false.
