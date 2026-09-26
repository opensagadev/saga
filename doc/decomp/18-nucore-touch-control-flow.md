# Touch input reconstruction notes

`NuPad_Interface_TouchScreenInput` in the Android x86 target is 3,282 bytes at
`0x271a70`. Compare with the forked GOT-aware `objdiff-cli`; ordinary raw GOT
displacements obscure the useful matches.

The first four integer arguments are two `(x, y)` pairs, not a touch ID and
pressure. Both pairs are converted to floats and divided by `g_backingWidth`
and `g_backingHeight`. Action priority is cancel, move, up, down. Move creates
a touch with the first pair and flag bytes `(0, 0, 1)`; up removes the touch
whose current or previous pair equals the second pair; down updates a touch
whose current pair equals the second pair, writing both coordinate pairs and
flags `(1, 0, 0)`.

## Repeated codegen patterns

- The cancel and up paths each contain an inlined 24-byte touch removal loop.
  The compiler copies the three flag bytes, four floats, and ID individually,
  leaving the padding byte untouched. Each float load/store interleaves with
  the corresponding flag byte. A struct assignment or `memmove` produces a
  different SSE copy sequence. Pointing one `volatile u8 *` at the source
  touch's first float and writing the destination at offsets `-24` and `-28`
  reproduces most of the scalar loop.
- The ten-slot searches are fully unrolled at `-O3`. The create search checks
  each of the three flag bytes separately, while up/down only check flags 0
  and 2. Marking every search record `volatile` changes register allocation
  and can make the function much larger; compare each search separately.
- The failed up/down search selects index 10 and then accesses that slot. This
  is present in the target, even though the touch array has ten entries.
  Cancelling with no nearby active touch clears all 240 event bytes, the
  count, and the ten used-ID bytes.

Current implementation matches 10.68% with the GOT-aware report, up from the
1.24% placeholder. Further work should align the cancel branch's `edi` count
and `esi` nearest index register choices, plus the removal tail's two
`movlps` zero stores and count-store order.
