# Batman tag icon codegen notes

`Tag_DrawIcon_Batman` is a 1,565-byte target routine compiled with NDK r8e
x86 GCC 4.7 at `-O2`. Its local position and `ADDGAMEMSG` template are both
16-byte aligned. The target checks all 16 `points_of_interest` pointers in
sequence, accumulates the corresponding joint matrix translations, and uses
the highest Y value. Writing sixteen visible checks reproduces the long
straight-line branch chain better than a counted loop.

The target copies `AddGameMsg_Default` into a local message twice with
`rep movsd`. The first copy appears dead from the reconstructed semantics, but
it shapes the entire register and stack layout. An early initialized
`ADDGAMEMSG_ALIGNED16` plus an empty compiler-only memory read, followed by a
second assignment before queuing messages, raised the direct object-to-DSO
GOT-aware score from 45.98% to 56.65%. The empty read emits no instruction;
it only prevents GCC from deleting the first copy.

For the Y maximum, `__builtin_fmaxf` emitted libm calls in this GCC build;
SSE intrinsics and inline assembly also changed register allocation badly.
The ordinary `candidate > current ? candidate : current` expression produced
`maxss` for the later checks and scored best. The first check still branches,
and the target's `maxss` placement differs, so this remains a matching lead.

Direct `.o`-to-`libTTapp.so` objdiff scores also count unresolved GOT and
branch relocations as mismatches. Confirm final percentages on the linked
Android target when the integrated build completes.
