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

## Loading screens and rectangle drawing

`Draw_LOADED`, `Draw_LOADING`, `Draw_LOADFAILED`, and
`Draw_LOADCORRUPT` each make one `MenuSmartTextEx` call. The first two
use Y `-0.4f`, two lines, and their matching `apitxt_GAMELOADED` or
`apitxt_GAMELOADING` text; the latter two use Y zero, three lines, and
`apitxt_FAILEDTOLOAD` or `apitxt_CORRUPTLOAD`. All four use
`MENUTEXTSCALE` on every axis and `MENUNORMALR/G/B` plus `MenuA`.
Their target sizes are 181/181/180/180 bytes, and all four match 100%.

`DrawRectRGBA` scales width and height, applies the same alignment bit
tests as `DrawMessageBoxRGBA`, and converts normalized coordinates with
`10240.0f` and `3584.0f` before calling `NuRndrRect2di`. A first pass
used `2560.0f` for X and appeared to score 99.92308%, but resolving the
literal-pool relocation showed that this was a real value mismatch. A
rebuild is needed to score the corrected value.

## Internal calling convention and floating-point conversion

The target `DrawFalconSpotLights` receives its pointer in EAX even
though its C++ symbol has one ordinary pointer parameter. Declaring the
static function `__attribute__((regparm(1)))` reproduces that calling
convention. One shared 16-byte-aligned `NUMTX` local lets GCC use the
target's realigned stack frame for both light draws. It now scores
99.82667%; remaining differences are local/global displacements and one
register choice. Four phase arrays advance after the corresponding
light's optional draw, even when its point of interest is absent.

`DrawGameMessage_Targets` uses `GameTimer.time_elapsed_mod_seconds`
(offset `+8`) for the arrow pulse. Convert its `u8` message alpha to
`i32` before converting to float. Converting `u8` directly to float
made GCC emit unsigned-int float conversion scaffolding that is absent
in the target. Moving `y_offset = 0.3f * scale` to after the `NuFmod`
call raises the 706-byte body's match from 92.32353% to 99.94118%.

## Material clip and animated status text

`DisplayListMaterialClipUpdate` duplicates the semantics of the
existing `nudlist.cpp` material clip helper: compare two used-byte
buffers, inspect the eight materials of each changed byte, and suppress
items with blend flag at `NUMTL+0xb0` and blend byte `NUMTL+0xf8`
equal to `0xff`. The initial attempt scored 29.64985%. The target
passes `scene` in EAX (`regparm(1)`), unrolls all eight bit cases, and
checks the `nmtls` bound after each item write. Those changes raised the
match to 53.183975%. Its outer loop also advances a running material
index by eight; source that recomputes `byte_index * 8` changes registers
and instructions. A running index is now staged for a later comparison.

`DrawStatusTextFraction` uses a stack-local matrix, unlike
`DrawStatusText`'s static matrix. It prints the suffix before two
animated copies of the current value. The current first pass scores
73.16456% of its 1,463-byte target; the matrix setup and font API
sequence are present, with argument and arithmetic ordering still to
tune.

## Starfighter calling convention and matrix frames

`DrawStarFighter` takes its only pointer in EAX, so its file-local
definition needs `__attribute__((regparm(1)))`. The target keeps two
16-byte-aligned `NUMTX` locals in a 0xb0-byte aligned stack frame: one
for special objects at ESP+0x30 and one for scaled character models at
ESP+0x70. An unaligned local changes the prologue, and putting the
locals in exclusive branches lets GCC reuse the same stack slot.
`NUMTX_ALIGNED16` repairs the prologue, but a first semantic pass still
only scores 46.30337% against the 778-byte body because GCC reuses the
matrix slot and schedules the model lookup differently. The fighter
layout places a NUMTX at +0, scale at +0xf0, draw flags at +0xfc, and
model ID at +0xfe. The special ID -307 emits debris effect slot 49.
Using one `NUMTX_ALIGNED16 matrices[2]` local forces two distinct 64-byte
slots without emitted barrier instructions. Promoting the signed model
ID to `i32` makes GCC load it with one `movsx` instead of `movzx` and a
later sign extension. An intermediate pass reached 85.74719%; the array
form awaits GOT-aware scoring.

## Hint, bloom, swipe, and minikit control flow

The target `DrawHint_LSW` consists of a 50-byte public guard and a
2,087-byte internal `.part.19` body. The public function loads its two
stack arguments into EAX/EDX, checks `FadeSys.fade`, then tail jumps to
the internal body. A monolithic source function makes the public symbol
1,854 bytes and scores 0%. Use a noinline internal function with the
target asm symbol and `regparm(2)`; keep only the guard in the public
function. This split is staged and compiled, with score pending.

`DrawAlphaImage` computes the first row's vertex alpha and reuses cached
alpha for later rows, while still transforming every current vertex.
The target tests `row > 0` so the cached path falls through. For both
vertices, directional bias exactly 1.0 uses the linear blend ratio
directly; other bias values call `NuPowFast`. Leaving out this fast path
is a real behavior and control-flow mismatch.

`SwipeDecalRenderer` stores color before UVs for every vertex. Its UV
branch writes float UVs on the fallthrough path and packed half UVs out
of line; a local inline writer reproduces that order. The strip's half
width has raw float bits `0x3d199999`, represented in source as
`0.037499998f`; rounding `0.0375f` produces different bits. The
constructor's width clamp uses comparisons with specific NaN behavior,
so ordinary adjacent `if` statements become `minss`/`maxss` and differ
from the target's branchy sequence.

`DrawStatusMiniKit` builds its two scale vectors before its direction
dispatch and checks the piece count bounds at the loop tail. Moving
those operations to the top of the loop made a smaller body with a very
different instruction order despite similar visible behavior. Its
piece rotation order is Y, Z, then X, including the translation row.
