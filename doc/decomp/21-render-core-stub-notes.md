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
