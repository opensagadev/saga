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
record fields `0x408` → `0x404` and `0x514` → `0x518`. The five looked-up
blowups are `Shoot_a11`, `Shoot_a1`, `Shoot_a21`,
`Shoot_b1`, and `Shoot_a31`; their target scales are multiplied by `1.5f`.

The initial `0x400` reset loop loads `world->space_level` during each
iteration. Caching that pointer before the loop makes GCC keep it in a
register and changes the target's `imul; add [world+0x5120]; store` sequence.
The subsequent copy loop instead uses a record pointer and compares it with
the one-past-end pointer at `space + 0x55f90`; an integer loop makes GCC emit
an extra counter and different offsets. In that loop, `reset_value` at
`+0x408` is copied to `saved_value` at `+0x404`, while `saved_state` at
`+0x514` is copied to `reset_state` at `+0x518`. The state direction is easy
to reverse when reading the field names alone.
