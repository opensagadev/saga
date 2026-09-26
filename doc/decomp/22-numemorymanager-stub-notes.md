# NuMemoryManager stub notes

This audit compares the NDK r8e GCC build with `res/libTTapp.so` through the
GOT-aware `objdiff-cli` fork.

## Verified findings

- `NuMemoryManager::Dump` and the four base `IErrorHandler` methods have
  nine-byte no-op or zero-return target bodies. The base `Dump` and
  `IErrorHandler::OpenDump` were checked at 100% after removing `STUBBED()`;
  the other three handlers have the same empty body shape.
- `NuMemoryManager::Validate` traverses every page and every block under the
  manager mutex. It validates each block's end tag, also validates the next
  block's end tag unless that block is the page end, and checks both free-list
  pointers for free blocks. The first C++ reconstruction is 97.612500%
  (287 target bytes). The remaining instructions differ in page pointer load
  order and compare operand order.
- `ValidateBlockDeferredContent` tests `DebugHeader::flags.alloc_flags & 0x20`
  at byte offset `0x0a`. This is a low-byte allocation flag, not an unknown
  high-byte flag. It scans words after the first payload word for the target's
  `0x7fbf7fbf` fill value and reports changed content through the error
  handler. A `do ... while` loop reflects the target's lack of a zero-count
  pretest. The initial implementation is 60.162500% (304 target bytes), with
  control flow and register allocation still to tune.

## Signature and ABI clues

- `_MultiBlockAlloc` returns a success flag in `eax` despite its current
  original `void` declaration. Zero count and failed bulk allocation return
  zero; successful splitting of the bulk block returns one.
- `ReleaseExternalPage` likewise returns zero or one. It unlinks an external
  page only if the page has a single free block spanning the whole page.
- `FindAndTouchMatchingBlocks` returns the number of matched blocks, used by
  `DumpBlocksForContext` when grouping blocks.
- `DumpBlock` returns a `u16` category, which `DumpBlocksForContext` uses as an
  index in its 128-entry category totals table.

Correct the declarations in both the real `NuMemoryManager.h` and the mirror
`nu2api_nucore_types.h` when implementing these methods. The latter is used by
the cross-translation-unit decompilation interface, so a mismatch there can
silently give callers the wrong return value.

## Control-flow and layout patterns

- Page walks use `page->first_header` up to `page->end`, advancing by
  `BLOCK_SIZE(header->value)`. Several routines call `ValidateBlockEndTags` on
  **every** block, including free ones, even when the body only processes
  allocated blocks. Keep the validation at the loop tail for
  `FindAndTouchMatchingBlocks` and `DumpBlocksForContext`.
- `DebugHeader::flags` is one 16-bit field: low seven bits are allocation
  flags, bits 7 through 11 are the context ID, and the top four bits include
  the touched marker. Grouping and leak cleanup must change only their own
  subfield; writing the whole word would erase other state.
- `FindAndTouchMatchingBlocks` skips the reference block but does **not** skip
  previously touched candidates. It only matches nonnull debug names, then
  compares allocation flags, context, category, optional backtrace words, and
  optionally the payload as a C string. It runs the payload comparison even
  when the backtrace comparison has already failed.
- `StrandBlocksForContext` counts newly moved blocks separately from all
  blocks already in the stranded context. The former drives the leak message;
  the latter becomes `stats.unknown_18`.
- `ReleaseExternalPage` has one shared mutex unlock and return for both success
  and failure. Returning directly from the successful unlink lowers the
  direct-object match from 78.46667% to 41.4% because GCC emits a different
  epilogue and branch layout.
- The dump routines use the footer's encoded manager index when subtracting
  end-tag overhead: high five bits of the last word are `index+1`, unless
  they are 31, in which case the preceding word contains the index. Indices
  at least 30 use an eight-byte footer rather than four. For grouped rows,
  subtract `(m_headerSize - 4)` once per block, then one footer adjustment.
- The target uses CRLF in all report rows, and its dump row format is selected
  by grouped count, string allocation flag, and debug mode. Exact string
  literals affect instruction selection and the literal pool, so preserve the
  row text when tuning the diff.

## Current reconstruction probes

All seven formerly stubbed methods in the second batch compile as a standalone
translation unit under NDK r8e GCC 4.7 with this file's `-O2` option. The
following scores compare that **unlinked object** with `res/libTTapp.so` using
the forked GOT-aware `objdiff-cli`. PIC relocation operands can still differ
until the full shared library is linked. These scores are a baseline for
control-flow tuning, not a claim of exact matches.

| Method | Target/current bytes | Object probe match |
|---|---:|---:|
| `_MultiBlockAlloc` | 426 / 547 | 28.286884% |
| `ReleaseExternalPage` | 242 / 212 | 41.4% |
| `StrandBlocksForContext` | 497 / 443 | 5.387597% |
| `FreeStrandedBlocks` | 634 / 634 | 61.6% |
| `FindAndTouchMatchingBlocks` | 588 / 478 | 35.373417% |
| `DumpBlock` | 1383 / 1314 | 17.546196% |
| `DumpBlocksForContext` | 1320 / 1600 | 27.779457% |

After aligning its page-load order, validation position, and capacity
calculation, `FreeStrandedBlocks` reaches **91.7375%** (634 target / 618 current
bytes) in the direct-object diff. Compute capacity as the available bytes less
four or eight footer bytes, call `ClearUsedBlock`, then divide the saved
capacity by pointer size. GCC emits the target's two arithmetic alternatives,
conditional move, and post-call shift; folding the entire expression before
the call instead gives a different `sbb` sequence and lowers the match.
The full target Bazel build succeeds, and the linked-library diff for this
version is **92.175%**. The remaining 16-byte size difference begins with a
different `this` register choice and also affects later branch displacements.
Giving `ReleaseExternalPage` the shared exit yields **78.46667%** (242 target /
230 current bytes) in the direct-object diff. Splitting its block-value tests
or forcing `eax` with an empty assembly constraint did not improve that score;
the constraint actually grew the function to 254 bytes.
The fully linked GOT-aware diff for the shared-exit version is **78.8%**.

For `_MultiBlockAlloc`, make the last iteration explicit: compare `i` with
`count - 1`, use the entire remaining byte count for that last block, and
clear the remainder. A ternary `i + 1 == count ? remaining : stride` causes
GCC to duplicate the conversion and validation call sequence for the last
block. The explicit branch shares the call sequence and raises the object
match from 28.286884% to 67.94262%. Writing the manager-index size adjustment
as an `if (idx >= 30) adjusted_size += 4` produces the target's conditional
move instead of an `sbb` sequence; together these changes reach **75.77869%**
with the exact 426-byte target size. Hoisting `m_headerSize + 4` into a named
overhead variable lowered the match, so keep the original inline expression.
The linked GOT-aware diff for this version is **76.14754%**.

The largest remaining difference is block layout. For example,
`StrandBlocksForContext` validates each block after processing its debug
fields and has a direct early return that clears all output references when
debug mode is disabled. Putting validation before the debug branch radically
changes branch destinations and register lifetimes even when the method has
the same external behavior.

The decisive `StrandBlocksForContext` detail is the five-bit context field.
After assigning `debug->flags.ctx_id = stranded_ctx.id`, read the field back
before comparing it with the full 32-bit stranded ID. GCC must account for
truncation to five bits and generates the target's reload and second compare.
Assigning the full ID to a local instead suppresses that control flow and
leaves the match around 8.56%. Keeping `BLOCK_SIZE` expressions in the paths
that need them, and marking the stranded-count branch unlikely with
`__builtin_expect(..., 0)`, moves the count block after the no-debug exit like
the target. Reversing the largest-block comparison to
`BLOCK_SIZE(largest->value) < BLOCK_SIZE(header->value)` also improves register
selection. The direct-object result reaches **96.17054%** (497 target / 491
current bytes); the remaining differences are mostly stack-slot and branch
operands.
The linked GOT-aware diff is **96.325584%**.
