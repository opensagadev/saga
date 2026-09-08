# Character switching and HUD recovery

The active scope covers the complete switching path and its HUD dependencies,
including the missing portrait and the arrow reported after switching. The
shop work is paused. No unrelated gizmo implementation is part of this work.

## Verified timer and scripted-switch unit (2026-09-08)

The original `Tag_DrawIcon_LSW` loads a float at object offset `0xd5c`
(`0x1452e5`). The reconstruction converted the integer union member instead,
turning a two-second timer's bit pattern into a large positive number. It now
reads `hud_icon_timer` and preserves the original unordered-float exit.
Matching improves from 94.266% to 95.459% (532 original / 540 current bytes).

The original `UpdateGameObjects` valid-player pass decrements that timer when
positive (`0x135664`–`0x13568d`), before movement dispatch. That missing block
is restored without clamping: the final subtraction may cross below zero.
The larger function remains incomplete and has a 0% instruction match.

`TagCharacter` (`0x24ddc0`) is no longer empty. It returns an integer, validates
the requested target, checks the eight player slots in original priority
order, calls `TagCode`, handles the delayed-transfer result, and exchanges
the active-player bits after a successful switch. Its initial match is
41.552% (1013 original / 1023 current bytes); it is not fully matched.

Verification:

- 2,160 mapped original/current `TagCharacter` cases agreed on target choice,
  call arguments, return value, active-player bits, and delayed-transfer
  fields. `TagCode` was instrumented with five return values and controlled
  side effects. These cases do not verify `TagCode` itself.
- 1,536 mapped arrow cases agreed on visibility, position, scale, colour,
  alpha and message flags, including timer boundaries and NaN. Final message
  submission was instrumented.
- A native ASan/UBSan run invoked the recovered switch against the nearest
  eligible loaded Cantina character. The switch returned 1, the target timer
  started at 2.0 and reached -0.01596675 after 180 further hub updates. No
  sanitizer error was reported. This is a controlled function-call test,
  not a normal controller/touch-input playthrough.
- An earlier attempt using `Player[1]` stopped because that save had no
  second player. It did not test switching. The captured image from the
  successful run still showed loading portraits, so visible in-game HUD
  recovery is not claimed from that image.

## Transfer effects recovery (2026-09-08)

`Tag_UpdateTransfers` (`0x4fdeb0`, 2111 original bytes) now recovers both
players' three particle trails. It preserves the original half-second timer,
the chained sine phases, collision-bound interpolation, and copying each
position after the particle callback. A missing player resets its timer;
landing exactly on 0.5 still emits the final particles. NaN timers are skipped.

The instruction match improves from the empty body's 1.014% to 45.572%
(2089 current bytes). Shared position storage and source/destination vectors
reproduce the original 0x50-byte stack frame. Branch placement and register
allocation still differ, so this is recovered behavior, not a complete match.
The source optimization settings are unchanged.

All 2,048 mapped original/current comparisons agree exactly on emitted
positions, effect arguments, timers, and stored trail positions. Cases cover
both player-presence masks, timer boundaries including NaN, four frame steps,
and particle callbacks that modify the supplied position. The target and
native builds pass. These comparisons instrument particle submission and do
not establish visible effects in an ordinary playthrough.

## Remaining scope

Recover and compare the original bodies and callers in this area, rather than
stopping at the two visible symptoms:

| Path | Current work remaining |
|---|---|
| `TagCharacter`, `Action_TagCharacter` | Improve the recovered body; recover the empty script action and its registration/caller path |
| `TagCode`, `Tag_Check`, `Player_ToggleCharacter` | Audit and match complete transition/input logic and state writes |
| `Tag_NewTransfer`, `Tag_ResetTransfers`, `Tag_UpdateTransfers` | Improve lifecycle matching; recovered update is 45.572% |
| `Tag_DrawIcon_LSW`, `Tag_DrawIcon_Batman`, `Tag_NoHiddenIcon` | Finish renderer matching and verify actual mode dispatch |
| `Tag_UpdateHint`, `HoldTag_UpdateHint`, hint cancellation | Recover hint lifecycle dependencies without unrelated hint work |
| `DrawPanel`, `DrawCharIcon`, icon-scene lookup/loading | Match portrait selection, visibility and icon-resource dependencies |
| `UpdateGameObjects`, character reset/new-character paths | Audit remaining shared timer/state reads and writes |
| Mech touch tag/party callbacks | Compare touch input dispatch and switching completion |

Runtime acceptance still requires ordinary repeated switching through the
relevant inputs, visible portraits after the original blink interval, the
original arrow lifetime, and no sanitizer errors. The broader gameplay/audio
goal remains incomplete.
