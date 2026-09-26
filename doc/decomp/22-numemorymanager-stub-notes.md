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

The largest remaining difference is block layout. For example,
`StrandBlocksForContext` validates each block after processing its debug
fields and has a direct early return that clears all output references when
debug mode is disabled. Putting validation before the debug branch radically
changes branch destinations and register lifetimes even when the method has
the same external behavior.
