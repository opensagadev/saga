# Dogfight A helper linkage and register arguments

The original `libTTapp.so` places `ChrisDogFightAInit`, `ChrisDogFightAReset`,
`ChrisDogFightADraw`, `ResetSpaceLevel`, and `DrawSpaceLevel` together. The
reconstruction splits the last two helpers into `chris.cpp` and `hud.cpp`.
Both target helper symbols have local ELF binding and names beginning `_ZL`.

The NDK r8e GCC build passes `ResetSpaceLevel`'s `WORLDINFO_s *` in `eax` and
`spacelevel_s *` in `edx`. It passes `DrawSpaceLevel`'s sole pointer in `eax`.
Ordinary cross-file declarations use the stack and cannot reproduce these
call sites. The declarations therefore preserve the target `_ZL` symbol names
with GNU `__asm__` labels, use hidden visibility, and specify `regparm(2)` or
`regparm(1)` respectively. Check both sides when moving a former static helper
to another translation unit: the source name alone does not preserve the ABI.

In the target, `ChrisDogFightAPanel` is an empty body (eight alignment NOPs
and `ret`). `ChrisDogFightADraw` is a tail jump to `DrawSpaceLevel` after
loading `world->space_level` from offset `0x5120`.

`ChrisDogFightAInit` resets `large_records[i] + 0x400` for 256 records, then
initializes flight splines only for `DOGFIGHTA_LDATA`. Its next loop copies
the reset value and state at offsets `0x408` and `0x514` to `0x404` and
`0x518`. The five looked-up blowups are `Shoot_a11`, `Shoot_a1`, `Shoot_a21`,
`Shoot_b1`, and `Shoot_a31`; their target scales are multiplied by `1.5f`.
