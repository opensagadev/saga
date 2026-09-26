# `gamemenuall.cpp` stub audit

The retail target has 63 `STUBBED()` callbacks in the current source. Its
symbol table and disassembly show that ten are genuine no-ops: each contains
eight NOP instructions followed by `ret` (9 bytes). These markers can be
removed without inventing game behavior:

`MenuDrawClips`, `MenuInitClips`, `MenuUpdateClips`,
`MenuDrawCardWarning`, `MenuEnterInsertCard`, `MenuDrawFormatConfirm`,
`MenuEnterNoMemoryCard`, `Draw_CHECKINGMEMORYCARD`,
`Draw_DONOTREMOVEMEMORYCARD`, and `Draw_SPACENEEDED`.

The first substantive batch uses direct target call and data-flow evidence:
`RenderFileSel`, `MenuIsAvailable`, `MenuDrawInsertCard`,
`MenuEnterCardWarning`, `MenuDrawAutoSaveWarning`,
`MenuDrawDoNotRemoveCard`, `MenuEnterAutoSaveWarning`,
`MenuUpdateNotEnoughSpace`, `MenuSetTopBottom`, and `ProcessFileSel2`.
All 20 callbacks match **100%** with the installed GOT-aware `objdiff-cli`
3.8.1 fork. The source's `MenuIsAvailable` return type and the C linkage
signatures of `MenuSetTopBottom` and `ProcessFileSel2` had been incorrect;
using the retail signatures is necessary for their exact matches.

Do not infer behavior from the name or from `STUBBED()` alone. In this file,
identically marked callbacks range from true no-ops to the 3,429-byte
`MenuUpdateEpisodes`.

## Shared deleting and formatting draw layout

`MenuDrawDeleting` (599 bytes) and `MenuDrawFormatting` (527 bytes) have the
same three-phase layout. Each has a function-local static
`messageswitched`:

1. While work is queued, running, or its message delay is positive, reset
   `messageswitched`, draw the progress message at `y = 0.2f`, and call the
   retail no-op `Draw_DONOTREMOVEMEMORYCARD`.
2. On failure, reset `MenuAlpha` and `MenuA` once, without drawing a result
   message.
3. On success, reset those fade values once and draw the completion text at
   `y = 0.0f`.

The deletion callback first copies `apitxt_DELETEGAME` into `MenuHeader` and
updates its RGB header fields; formatting does not. The retained source
matches 99.68595% and 99.669815%, respectively, using the GOT-aware fork.
Most remaining differences are direct references to static TU storage whose
addresses move in the rebuilt library, plus a small register choice.

## End-screen timer conversion

`MenuDrawEndMission` and `MenuDrawEndChallenge` share the same flashing timer
layout. The retail code tests `NuFmod(GameTimer.time_elapsed, 0.3f)` against
`0.2f`; it reads the first `GameTimer` float, not the similarly named
`time_elapsed_mod_seconds` field at offset 8. Both convert a `u16` time limit
with one signed `cvtsi2ss` instruction. In this compiler, casting the `u16`
directly to `f32` expands into a high-word correction path, so cast through
`i32` first. Clamp the resulting time with an ordinary `if (remaining < 0)`;
the compiler folds that into `xorps` plus `maxss`. Calling `fmaxf` instead
emits a library call. Each callback then calls `Text_MakeTime` and `Text3D`
with the same layout. The GOT-aware fork reports 99.989130% and 99.990000%
for the retail-sized callbacks; their only displayed mismatch is the shifted
rodata address of the `0.2f` literal.

## Memory-card callback branch layout

The file-corrupt callback reads the preceding menu ID repeatedly. Keeping a
cached local or selecting the next ID with a ternary makes GCC merge branches
and drops its match below 25%. Writing the three explicit previous-menu paths
and assigning `MENUFNINFO.wrap` with separate branches reproduces the retail
308-byte body, at 99.973335% with the GOT-aware fork.

`MenuUpdateInsertCard` and `MenuUpdateNoMemoryCard` test cancel and confirm as
two independent `if` statements; the retail code can run both in one call.
Other confirmation callbacks use `else if`, so do not globally consolidate
these input checks. For formatting, a likely-true branch hint on
`memcard_formatting` moves the shared result-delay block before the timer
check, closer to retail order. The same state machine uses distinct message
and result delay floats; both need definitions in this translation unit.

## Menu background primitive

The retail menu fade draws two vertices through `NuPrim2DAddXYZ`, setting
their color at offset 12 in the current stream buffer. The stream buffer
global is a pointer to a `VARIPTR`; retain its address and dereference it
for each vertex, because the primitive call may advance the cursor. GCC
removes the overbrightening mask when it proves a color shifted left by 24
has only alpha bits. An empty register constraint makes the value opaque to
that optimization and preserves the retail branch and mask. A likely-true
hint on the rising fade branch also keeps the retail block order. The direct
NDK r8e object reproduces the 327-byte target body; linked GOT-aware scoring
remains to be verified.
