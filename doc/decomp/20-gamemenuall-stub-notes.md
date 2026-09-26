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
