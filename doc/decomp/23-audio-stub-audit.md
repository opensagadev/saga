# Audio placeholder and duplicate-symbol audit

The Android target has four short audio entry points whose bodies are inert:

| Symbol | Target bytes | Source owner |
| --- | --- | --- |
| `CheckMusicSwapInstant()` | `xor eax,eax`, six `nop`, `ret` | `legoapi/audio/audio.cpp` |
| `ResumeGameAudio()` | eight `nop`, `ret` | `legoapi/audio/sfx.cpp` |
| `SOUND_SFXRequest_Table()` | eight `nop`, `ret` | `legoapi/audio/sfx.cpp` |
| `SfxCheckMusicOnOff(OPTIONSSAVE_s*)` | eight `nop`, `ret` | `legoapi/audio/sfx.cpp` |

The empty bodies and literal zero return already produced those exact sequences in
the prior target build. `STUBBED()` expands to nothing for the Android target;
removing the macro eliminates misleading runtime instrumentation on host builds
while retaining the Android code. Do not invent behavior for these functions
based on their names alone.

`bark_noise_hybridmp` is an important duplicate-symbol trap. The target contains
a 1,541-byte local function and a 1,116-byte `.constprop.1` clone. The pinned
libvorbis 1.3.2 dependency (`@libvorbis//:vorbis`, compiled as C++ at `-O3`)
already emits both with those sizes. An artificial nine-byte `__used__` stub in
`legoapi/audio/gamelib_ogg.cpp` emitted a *third* same-name local symbol and
could make a name-only objdiff pairing select the wrong body. Remove the
duplicate rather than reimplementing upstream libvorbis code in the game tree.
Inspect all same-name LOCAL symbols on both sides before editing a stub with a
library-looking name:

```powershell
readelf -Ws res/libTTapp.so | rg bark_noise_hybridmp
readelf -Ws bazel-bin/src/libTTapp.so | rg bark_noise_hybridmp
```

Reference source: [Xiph libvorbis 1.3.2 `lib/psy.c`](https://github.com/xiph/vorbis/blob/v1.3.2/lib/psy.c).
