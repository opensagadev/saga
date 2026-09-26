# Visibility occlusion codegen

`NuVisiOcclusion` matches 100% of the 341-byte Android x86 target at
`0x2d5a30` with the forked GOT-aware `objdiff-cli`. Its grid record contains
camera X/Z bounds, dimensions and scales, a mode, output size, and a table of
compressed cell pointers. It decodes two-bit commands into literal copies or
zero/`0xff` runs in `OcclusionBitArray`.

Two source details closed the final gap and apply to similar routines:

- Keep a `u8 *volatile *` pointer to a table slot when the target loads the
  pointed-to value twice around global-state stores. Caching the value in a
  local made GCC hoist a GOT load and changed the register allocation through
  the rest of this function. The volatile slot forces the second read while
  preserving the target's pointer in a callee-saved register.
- Keep a packed command tag in `u8` through the branch and negation. An `i32`
  expression produced `cmp eax` and signed `jg`; `u8` produced the target's
  `cmp al`, unsigned `ja`, and `movzbl` after negation for the `memset` value.

The target uses a strict `output < end` loop and calls `memmove` for literal
runs. It updates `CurrentViewBox` only when the selected cell changes.
