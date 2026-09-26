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

## Signature and ABI clues for the remaining functions

- `_MultiBlockAlloc` returns a success flag in `eax` despite its current
  `void` declaration. Zero count and failed bulk allocation return zero;
  successful splitting of the bulk block returns one.
- `ReleaseExternalPage` likewise returns zero or one. It unlinks an external
  page only if the page has a single free block spanning the whole page.
- `FindAndTouchMatchingBlocks` returns the number of matched blocks, used by
  `DumpBlocksForContext` when grouping blocks.
- `DumpBlock` returns a `u16` category, which `DumpBlocksForContext` uses as an
  index in its 128-entry category totals table.

These declarations need correcting when the corresponding bodies are filled.
