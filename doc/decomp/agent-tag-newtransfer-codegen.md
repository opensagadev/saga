# Tag_NewTransfer block order under NDK r8e GCC

`Tag_NewTransfer` is compiled with NDK r8e GCC 4.7 at `-O2`. The GOT-aware
`objdiff-cli` fork measured the function directly against the compiled
`tagging.o`. This direct object comparison includes unresolved relocation
mismatches, so its percentage is lower than the final linked comparison.

An ordinary nested `if (flags_low < 0)` puts the return epilogue before the
`Tag_DoneFirst` handling and branches forward to that handling. Changing the
condition to an early return and marking the return path unlikely with
`__builtin_expect(condition, 0)` puts the handling before the epilogue, matching
the target's `jns` to the epilogue. Marking the `Tag_DoneFirst == 1` case
unlikely moves its six-byte store and jump after the long transfer body. These
two hints changed the direct object score from **72.257% to 83.115%** and
reduced differing instructions from **83 to 68**.

Explicit empty inline-assembly register constraints forced the target's
`source` in `esi` and `target` in `edi`, but caused a different prologue and
lowered the direct score to 80.027%. Register choice is still unresolved; do
not treat a register-only improvement as a match without checking the whole
function. The remaining large mismatch is the repeated height interpolation:
the target computes `(max_y - min_y)` in `xmm2` or `xmm1` and multiplies a
loaded height in `xmm0`, while the current code computes the range in `xmm0`.
