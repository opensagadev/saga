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
