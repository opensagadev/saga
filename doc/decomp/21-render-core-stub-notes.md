# Render core stub matching notes

These observations come from the NDK r8e x86 GCC build of
`src/legoapi/render/core/render.cpp`. Scores were measured against
`res/libTTapp.so` with the GOT-aware `objdiff-cli` fork.

## Empty target routines

The target's `DrawBox_Now`, `DrawLine_Now`, `DrawArrow_Now`,
`DrawCross_Now`, `DrawBoxMtx_Now`, `DrawCameraTarget`,
`DrawCameraTarget2`, `Draw_NOMEMORYCARD`, and `Draw_AUTOSAVECANCEL`
each have a nine-byte nop-padded empty body. Remove the `STUBBED()`
marker and retain an empty body. All nine remain 100% matches.

## Return value controls the final branch

`DrawPanel3DObjectMtxNoAlpha` has an `i32` return value. Its false
branch returns zero, while its successful branch returns the result of
`NuSpecialDrawAt(special, matrix)`. Writing a separate `return 0` after
the draw call produced 99.833336%: GCC routed that branch through the
`xor eax, eax` block. Returning the draw call directly makes the final
branch skip that zeroing instruction and restores the exact 107-byte
target body.

`PauseRenderOff` calls `GetMenuID()` and returns zero. The call result is
discarded, but the call remains in the target's 29-byte body. Its source
return type should be `i32`, matching the callback's return register.

## Simple menu draw wrapper

`Draw_OK` copies `MENUBOTY` to `menu->draw_y` (offset `0x98`) and calls
`DrawMenuEntry(menu, apitxt_OK)`. The target's two GOT references resolve
to those exact globals. This source produces the exact 64-byte body.

## Geometry array placement and repeated argument reads

`DrawAreaCylinder` is a single call to `LocaledbitsDrawSolidCircleXY`.
Its radius is `NuFmin(size->x, size->z)`, and its Y bounds are
`centre->y` and `centre->y + size->y`; this yields the exact 155-byte
body.

`DrawAABox` builds two 16-byte-aligned `NUVEC` locals. Declare the
minimum before the maximum, then assign all maximum components before
all minimum components. The declaration order places the minimum at the
lower stack address, while the assignment order makes GCC compute the
sum before the difference for each coordinate. This produces the exact
177-byte target body. Reversing the declaration order changes only the
two local stack addresses and scores 99.818184%.

`DrawAreaBox` uses four X/Z corners, SIMD rotation, and 12 direct
`AiRndrLine3dDbg` calls. The target keeps the four arrays on the stack
in the order local X, rotated X, local Z, rotated Z. Recompute the Y
arguments from `position` and `size` at each call; caching lower and
upper Y changes the outgoing-call layout and lowered the match to
57.641846%. Declaring cosine before sine restores the target's register
choice and call size. The current body scores 99.96809%; remaining
reported differences are the sign-mask constant-pool displacement and
stores of duplicate values into the four corner lanes.

## Paint-light branch shape

`DrawPaintLights` loops over the signed count at byte 1 of
`factoryb_netpacket`, draws each existing `paintlights` special using
`green_light`, then sets visibility for three `painttargetcolour`
specials. The colour selector is byte 3 of the packet. GCC 4.7 folds
`colour == 0 || colour == 4` into a bit test, unlike the target's
`cmp 4` followed by `test 0`. A zero-instruction `asm volatile` barrier
between two explicit branches preserves both comparisons. The current
implementation scores 87.02128%; the remaining difference is the
placement of the first hidden-colour block, which lengthens one jump.
