# Episode I CreatePod compiler notes

The original `Action_CreatePod` is a 1,815-byte GCC 4.7 function. The first
functional reconstruction built at 2,328 bytes and scored 7.654912% with the
GOT-aware `objdiff-cli` fork. Its control flow was broadly reconstructed, but
the compiler chose a different stack frame and loop exit, causing instruction
alignment to collapse across the function. Aligning the local points and
changing the loop exit raised the score to 36.430730% (2,344 bytes). Keeping
`RacePodAlign` out of line raised it to 64.448364% (1,784 bytes).

## Stack alignment controls the frame shape

The original prologue saves `ebp`, sets `ebp = esp`, aligns `esp` to 16 bytes,
then reserves `0x90` bytes. The first reconstruction used ordinary 16-byte
point structs and GCC omitted the frame pointer and alignment step. Its
arguments were addressed relative to `esp` rather than `ebp`, so virtually
every stack operand differed.

Declare the local spline points and direction with
`__attribute__((aligned(16)))` when matching this function. Keep the
`racepod_s` entry type at its original 0x98-byte stride. Giving the struct
type 16-byte alignment would enlarge its array stride to 0xa0 bytes.

## Counted loop exit matters

The original parameter loop increments an index and exits on equality with
`param_count` (`cmp edi, [ebp+0x18]` followed by `je`). A source loop written
as `i < param_count` compiled to a less-than-or-equal branch with the operand
order reversed. Since the function first checks `param_count > 0`, expressing
the loop as `i != param_count` preserves behavior while inducing the original
exit condition. It produced `je`, though GCC still reversed the operands of
the `cmp` against the original.

## Preserve the helper call

The target calls a separate `RacePodAlign` helper at the midpoint of
`Action_CreatePod`. With one source call site, GCC inlined the entire helper,
adding roughly 500 bytes to the action. Marking it `noinline` restored a
separate call and nearly matched the target function size. The target helper
is a compiler `.constprop` clone with three original call sites, including
two in `PodRaceUpdate`. The current source only calls it from
`Action_CreatePod`, so GCC also propagates the action's fixed blend value and
omits the blend register from the clone call. The target passes blend in
`xmm0`. Reconstruct the other call sites before tuning this clone's ABI.

## Remaining spline scan and frame mismatch

The current 64.45% build reserves `0xa0` stack bytes after alignment; the
target reserves `0x90`. All three 16-byte vector locals are consequently
addressed 16 bytes higher in the rebuild: for example, the first two
`CalcSplinePointFromDist` output buffers use `esp+0x70` and `esp+0x80`
instead of target `esp+0x60` and `esp+0x70`. The direction buffer likewise
uses `esp+0x90` instead of `esp+0x80`. Retain aligned locals while reducing
the scalar/live-range footprint that precedes them; removing alignment would
lose the target prologue entirely.

The target spline-ID scan at `0x1fef17–0x1fef43` advances an ID-field pointer
by `0x52c` while counting indices in `ecx`, then multiplies the final index
by `0x52c` to form the selected spline pointer. The current source carries a
`flightspline_s *spline` through the loop and the rebuild at the corresponding
site advances that pointer directly. A distinct, untested source probe is
to scan with an integer index into the `PodRace` data, then form `spline`
after the loop. Preserve the 32-entry limit and target null check when
reconstructing that scan. This pointer lifetime difference also changes
`esi`/`eax`/`ecx` allocation throughout the following unrolled pod-slot
search.
