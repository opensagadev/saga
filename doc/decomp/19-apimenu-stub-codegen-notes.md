# `apimenu.cpp` stub matching notes

These observations come from `res/libTTapp.so` and the installed GOT-aware
`objdiff-cli` 3.8.1 fork. The source changes are on `codex/gui-stubs`.

## Unsigned color selection in bonus screens

Both `MenuDrawBonusWin` and `MenuDrawBonusComplete` select their red and green
channels from `menu_flash`. The target uses an **unsigned** comparison with 1:

```asm
cmp dword ptr [eax], 1
sbb edx, edx
and edx, 0x30
add edx, 0x8f
```

The analogous red channel uses `and eax, 0x40; add eax, 0xbf`. This corresponds
to green `143 + (u32(menu_flash) < 1 ? 48 : 0)` and red
`191 + (u32(menu_flash) < 1 ? 64 : 0)`. Signed comparisons and plain color
ternaries compiled to `cmov` and matched roughly half the function. The
unsigned expressions make the color arithmetic match exactly. In the final
combined build, each draw callback retains only an initial guard register
difference.

## Language list's second word is text

`MenuDrawSelectLanguage` passes the second word of each
`Text_LanguageList` entry directly to `GameDrawMenuEntry`. It does **not**
index `TTab`; that extra lookup created an 11-byte size difference. The
existing `LANGUAGEDATA::unknown_04` is typed `i32`; for this i386 target it
holds a `char *` at run time. A cast at the call site yields an 87-byte
function at 99.5% matching. The only remaining differences are the two
instructions that load and test the initial `LANGUAGECOUNT` value in `edx`
instead of `edi`; the subsequent loop matches.

## MiniKit draw calls

`MenuDrawMiniKit` shares its `DrawCharIcon` setup with `MenuDrawShop`, then
calls `DrawPlayerIconPrompts`. The original calls
`NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f)` twice and discards both
returns. Keeping both calls reproduces the target's x87 stack pops. The
function is 375 bytes on both sides and scores 99.97403%; the two reported
differences are addresses of floating constants in the literal pool.

## Callback results

| Callback | GOT-aware match |
| --- | ---: |
| `MenuEnterTitles` | 100% (empty in the target) |
| `MenuInitHowToPlay` | 100% (empty in the target) |
| `MenuUpdateMiniKit` | 100% |
| `MenuDrawBonusWin` | 99.625% |
| `MenuDrawMiniKit` | 99.97403% |
| `MenuUpdateBonusComplete` | 99.92105% |
| `MenuDrawBonusComplete` | 99.63415% |
| `MenuUpdateBonusWin` | 99.52631% |
| `MenuDrawSelectLanguage` | 99.5% |
| `MenuUpdateSelectLanguage` | 99.16666% |

The two bonus update callbacks are 153 bytes on both sides. Their remaining
differences are floating constant and string addresses; `MenuUpdateBonusWin`
also has one two-instruction register choice for reading `BonusWinFlag`.
