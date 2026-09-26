# Episode I Sebulba action codegen notes

Reference: `res/libTTapp.so`, `Action_Sebulba` at `0x1ff4f0` and
`Action_NewSebulba` at `0x2028c0`. Both functions return an integer status
despite older source stubs declaring `void`: invalid packet/owner/object
returns 1, and a handled action returns 0.

## Calls whose results are discarded

`Action_Sebulba` calls `MidDistanceFromSockStart` four times in sequence.
The first two results form a player minus Sebulba distance. The next two
results are discarded before the speed calculation. Preserve all four calls:
the helper has no pure or const attribute, and GCC retains the two apparently
unused calls. Removing them changes the instruction stream.

## In-place vector helpers

The target computes a local X offset for both player and Sebulba with
`NuVecSub`, then calls `NuVecRotateY` with identical output/input pointers
for each vector. It blends only X into a third vector:

```cpp
goal.x = sebulba_offset.x * sebulba_lerpf +
         player_offset.x * (1.0f - sebulba_lerpf);
goal.y = 0.0f;
goal.z = 0.0f;
NuVecAdd(&goal, &goal, &ahead.midpoint);
```

The output of `NuVecAdd` is also in place. Separate output temporaries alter
stack allocation and can lower the instruction match even when the math is
equivalent.

## Sock distance wrapping

`Action_NewSebulba` reads `WORLD->sock_sys->sock[index].unknown_98` as the
total distance for a socket. It normalizes a distance difference into
`[-length / 2, length / 2]` by adding or subtracting one length. The target
uses this to choose the player farther ahead and then to compare Sebulba to
that player's position. This field is at `SOCK + 0x98`; the `SOCK` stride is
`0x13c`.

## Signed byte locals and branch layout

In `Action_NewSebulba`, the goal index is stored as a signed byte. Keeping a
local `i8 index` makes GCC 4.7 emit `cmp al, 3`; widening the local to `i32`
emits `cmp eax, 3` even though the load is still sign extended. Write the
in-range arm first (`if (index <= 3)`) to match the target's forward `jle`
to the indexed array load. The opposite test reverses the two array-load
blocks. The branch inversion alone raised the GOT-aware match from 68.32% to
68.64%.

The socket index must remain signed for the `-1` sentinel and the `SOCK`
array subscript. For comparing its low byte to `player->field_0x661`, cast
the local to `u8` and compare directly against the field. GCC then emits
`cmp dl, [eax+0x661]` instead of loading and sign extending the player's
field into a second register. With the signed goal index, this raised the
match further to 69.74%. The target uses `cl` for the comparison; that
remaining register difference is part of broader allocation drift.

## PIC global lookup

The Android x86 build uses `__x86.get_pc_thunk.bx` and then a fixed add to
form a GOT base. For these functions the base is `0x616870` in the reference
ELF. A load such as `[ebx - 0x25f8]` therefore reads GOT entry
`0x614278`, which contains the address of `sockstep`. Check both the GOT
entry and the symbol table before naming a referenced scalar: nearby entries
include `test_seek_rate`, `test_factor`, and the separate
`player`/`player2` globals.
