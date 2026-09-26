# NuSound stub and compiler audit

Audited on 2026-09-25 against the Android x86 reference `res/libTTapp.so`,
starting from `main` at `500732fb`. Measurements used the installed
GOT-aware `objdiff-cli` fork and `bazelisk build --config=target
//src:saga_target`.

## Verified no-op surface

The 76 explicit `STUBBED()` sites under `src/nu2api/nusound` were checked
against the reference. Seventy-five compiled to a **100% match** already.
Every one of those reference functions is nine bytes long: a no-op `ret` or
a constant return padded with NOPs. The markers were removed because their
host-side warning incorrectly described implemented reference behavior as
missing. The one exception was
`NuSoundSystem::TitleHasUserMusicControl()`: the reference returns
`true` (`mov eax, 1`, six NOPs, `ret`), while the source returned
`false`. Returning `true` yields a **100% match**.

This audit concerns the explicit markers, not every incomplete sound
function. Large functions with partial reconstructions still need work.

## OGG read callback

`NuSoundDecoderOGG::OGGReadCallbacksDecoder::Read` is a 1,227-byte
reference function with a partial reconstruction. Its baseline was
**31.488672%**.

| Source pattern | Callback match |
| --- | ---: |
| Local weak pointer followed by `callback.Set(decoder)` and pass by value | 31.488672% |
| Pass `NuSoundWeakPtr<NuSoundBufferCallback>(decoder)` directly | 56.770226% |
| Also retain frame pointer and realign the stack | 66.423950% |
| Also mark zero-length input as the unlikely branch | **68.614880%** |

The local-pointer form compiles an extra copy constructor and associated
recursive list-lock operations when `RequestBuffer` takes its argument by
value. A direct temporary is built in the outgoing argument area, matching
the reference's single weak-pointer lifetime. This pattern is relevant to
other `NuSoundWeakPtr` call sites.

The reference enters with `push ebp; mov ebp, esp` and
`and esp, -16`. The function-specific GCC attributes
`optimize("no-omit-frame-pointer")` and `force_align_arg_pointer`
recover that prologue. The reference branches over the normal body on
zero-length input to a cold return near the end; `__builtin_expect(size ==
0, 0)` moves that return in the same direction.

The callback remains incomplete at **68.614880%** (current size 1,120
bytes versus reference 1,227). The main residual differences include the
weak-pointer list-link sequence, stack-slot layout, register allocation,
and return-block placement. Reordering equivalent min expressions did
not change codegen. Other branch hints reduced the score. A generic
`NuSoundWeakPtrObj::Link` rewrite and artificial stack padding did not
improve this callback and were reverted.

## Save-load translation-unit optimization

The same NDK r8e GCC 4.7 source produces different assembly when the
translation unit is compiled at `-O2` rather than at `-O0` with an
`optimize("O2", "omit-frame-pointer")` attribute on each function. This
matters for `src/legoapi/core/config/saveload.cpp`, whose previous Bazel
optimization map left the translation unit at `-O0`. Direct NDK r8e
object comparisons against the retail shared object gave:

| Function | TU `-O0`, function `-O2` | TU `-O2` |
| --- | ---: | ---: |
| `SaveSystemInitialiseEx` | 67.675674% | 98.702705% |
| `FS_GetPadWithRepeat` | 91.000000% | 99.333336% |
| `FS_SortStrings` | 67.19% | 94.953% |

Existing functions in that same source file also respond: the direct
object score for `InitMemCard` rises from 24.44% to 100%,
`TriggerAutoSave` from 56.75% to 98.25%, `UpdateSaveSlots` from 0%
to 32.07%, and `loadsaveCallEachFrame` from 20.26% to 22.58%.

With full-TU `-O2`, `FS_BuildFilterBlocks` and
`FS_BuildFilterOutBlocks` also compile to the exact retail function
sizes, 0xa5 and 0x83 bytes. `FS_GetPadWithRepeat` gains the retail
six- and seven-byte NOP padding before branch targets. The function
attribute alone does not enable that alignment, even with an explicit
`optimize("align-jumps=16")` option. Register allocation in
`SaveSystemInitialiseEx` and `FS_SortStrings` changes as well. These
are reasons to test the optimization setting at the translation-unit
level before tuning individual branches or registers.

The effect is not uniform: in a separate source reconstruction,
`FS_PrevNameLen` scored 45.33% at TU `-O0` and 20.59% at TU `-O2`
in the object comparison. Keep function-level exceptions or further
source tuning available when evaluating the full-TU change.

The percentages above compare a manually compiled `.o` with the retail
linked `.so`. Calls, GOT loads, and address relocations therefore still
count as argument differences. A linked Bazel build, measured with the
GOT-aware `objdiff-cli` fork, is required before claiming a function is
100% matched. A TU-wide `-O2` change also affects every existing
function in this file; check its whole-game report before accepting it.

The linked GOT-aware report confirmed a net gain after adding
`-O2,-fomit-frame-pointer` for this translation unit. With identical
reconstructed source, whole-game fuzzy match rose from **61.227620% to
61.250072%**, exact functions from **6,139 to 6,145**, and exact code
bytes from **508,809 to 509,714**. `InitMemCard`,
`TriggerAutoSave`, `FS_GetDirTextWidth`, `FS_BuildFilterBlocks`,
`FS_BuildFilterOutBlocks`, and `FS_SetFileSelPathFromName` became exact.
`SaveSystemInitialiseEx` reached 99.98649%, and `FS_SortStrings` reached
98.06512%. The same setting lowered `FS_PrevNameLen` from 45.71795% to
20.97436%, `FS_GetDirList` from 68.71043% to 63.30116%, and two
filter routines slightly. Those functions need source-level tuning;
the net whole-game and exact-match gains justify retaining the map entry.

For the pad-repeat body, source comparison direction also matters:
`if (PadRepeat < 0.0f)` produces the retail
`xorps xmm1, xmm1; ucomiss xmm1, xmm0; ja` sequence. The equivalent
`if (PadRepeat >= 0.0f)` instead loads a zero constant from memory and
reverses the operand/branch order.

For `FS_GetDirList`, a local `char entry[0x118]` with
`__attribute__((aligned(16)))` reproduces the reference's EBP frame
and 16-byte stack alignment. Its seven-byte packed metadata header
uses 4/2/1-byte copies when written with a `volatile char header[7]`
and a seven-byte `memcpy`; writing the date bytes in order 4, 3, 2,
1, 5 preserves the retail load order. Explicit 16/24-bit comparisons
for `.` and `..` also match the directory skip checks.

In `FS_SetFileSelPathFromName`, NDK r8e differentiates
`char file_name[256] = ""` from `= {}`: the former emits a four-byte
zero store followed by a 63-word `rep stos`, matching the target;
the latter emits a 64-word `rep stos`. The function attribute
`force_align_arg_pointer` reproduces its EBP frame and 16-byte stack
realignment. Together, these changes raised the direct object match
from 55.45% to 93.91% before TU-level optimization.

In `SerialiseNuHSpecial`, passing `NuSpecialGetName(special)` directly
to the virtual `EdStream::SerialiseString` call causes GCC to load the
virtual slot before calling `NuSpecialGetName`. Storing the name in a
local first makes it load the virtual slot afterward, as in retail.
That source expression change raised the direct object match from
88.81% to 98.30% and recovered the exact 331-byte function size.
The scene loop reads `LevelEditor+0x2a0`, which this tree names
`reset_pending`; `editable_scene_count` is at `+0x29c` and was an
incorrect reconstruction here. Correcting the field preserves the
331-byte size and raises the direct object score to 98.319%. The
remaining non-relocation mismatch is the loop comparison operand
order (`cmp edi, [esi+0x2a0]` in retail versus the equivalent
`cmp [esi+0x2a0], edi` followed by the inverse branch).

In `FS_SetCursorToLastFileName`, NDK r8e GCC reduces the plain
`last_visible-- >= FS_NumFiles` loop condition to a register comparison
and reserves 28 stack bytes. Capturing the old value in a volatile
local before the comparison instead emits a stack store and reload and
reserves 44 bytes, as the retail function does. This raised the direct
object match from 75.97% to 80.57% without changing its 249-byte size.
