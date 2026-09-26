# objectsall target reconstruction notes

Reference: Android x86 `res/libTTapp.so`; compile with Android NDK r8e GCC 4.7 and measure with the installed GOT-aware `objdiff-cli` fork. The target symbols below are scattered across the retail binary even though this checkout collects them in `objectsall.cpp`.

| Function | Target address | Target code bytes | Main observation |
| --- | ---: | ---: | --- |
| `Boulder_Kill` | `0x20e030` | 191 | Search the two `boulder_part` slots, then clear the matching level animation and special. |
| `Boulder_Move` | `0x20e0f0` | 463 | Copy part position to `boulder_oldpos`, copy special draw matrix to part, derive velocity when `scale_time > 1.0f`, and stop the animation when it reaches the end. |
| `FindNextBreak` | `0x42d0c0` | 352 | Search up to 20 UTF-8 characters forward for punctuation/space boundaries. |
| `FindNearestBreak` | `0x42d220` | 654 | Search up to 20 rounds in both directions, checking forward before backward. |
| `AddDevice` | `0x258c26` | 297 | Unoptimized whole-struct device copy, three current-directory copies, then increment `numdevices`. |

## Data and ABI findings

- `boulder_oldpos` is a 24-byte BSS symbol at `0x6b0344`, immediately before the two `boulder_part` pointers at `0x6b0360`. It was absent from the source declarations. `Boulder_Move` copies each part's `position` to its slot **before** obtaining and copying the draw matrix; the prior position is later used to compute `velocity` through `NuVecSub` and `NuVecScale`.
- The global at `Boulder_Move+0x17` is `netclient` (`0x12a522c`) after resolving its GOT slot. The next slot is `LevelChange`; off-by-four errors in a manual GOT dump can silently substitute the wrong global. The `netclient` early return precedes the part lookup.
- Both break functions return `i32` in `eax`; the old placeholder declarations used `void`. GCC's `add $-128; cmp $63` byte sequence recognizes UTF-8 continuation bytes `0x80..0xbf`. Punctuation checks use subtraction by `','` followed by `cmp $1`, thereby grouping comma and hyphen. A space before `?`, `!`, `;`, or `:` is skipped so the break can attach punctuation to the preceding word. A period before another period is also skipped.
- The break functions adjust a fallback position around `~` after 20 unsuccessful rounds. The emitted `sete/setne` plus subtraction is a boolean decrement of the byte index, not arithmetic on a UTF-8 code point.
- `AddDevice`'s target has a frame pointer and `rep movsd` with count `0x8d`: 141 dwords = `0x234` bytes, the size of `NUFILE_DEVICE`. It uses an `imul` by `0x234` to index `devices`, then calls `NuStrCpy` for `cur_dir`, `sys_dir`, and `dll_dir` using `default_device` as the source. Source must explicitly return the address of the newly added device even though the placeholder had `void` return type.

## Codegen rules to reuse

1. Resolve each local GOT slot by dumping four bytes at the slot address in `.got` and mapping that value with `nm -S -C`. Nearby globals with similar names can silently produce plausible but incorrect source.
2. For a large struct assignment at GCC `-O0`, retaining `dest = source` can produce `rep movsd`, while `memcpy` may route through a library call or different inline expansion. This matters for the device table's `0x234` byte records.
3. In a bidirectional text search, the target's `setl`/`setg` instructions express conditional `+1` and `-1` of offsets without branches. The order of forward and backward punctuation tests controls the chosen break on a tie.
4. GCC 4.7 retains the source operand order for some floating compares. In `Boulder_Move`, spelling the threshold as `!(1.0f >= part->scale_time)` emits target `ucomiss constant, field; jae` and the target's 463-byte body. Spelling the equivalent test as `part->scale_time > 1.0f` emitted `ucomiss field, constant; jbe` and a 466-byte body in a direct NDK r8e compile. The negated comparison also preserves the target's unordered NaN branch behavior.
5. The two-slot boulder lookup is sensitive to branch layout. An explicit `if (slot[0] == part) ... else if (slot[1] == part) ...` places the first `Boulder_Move` match in a cold block and the second in the fallthrough, matching the target. A two-iteration `for` loop produced the reverse layout with the same behavior.
6. The text-break punctuation guard also depends on its CFG shape. Four ordered `if (next == punctuation) goto continue_scan;` checks retain the target's four individual comparisons and restore `FindNextBreak`'s 352-byte size. A combined `&&` predicate let GCC merge `':'` and `';'` into a range check and produced a 344-byte body; a `switch` lowered to a different layout and matched poorly.

## Remaining match work

Linked GOT-aware results after the pass: `AddDevice` 100%, `Boulder_Move` 99.75%, `Boulder_Kill` 94.37%, `FindNextBreak` 63.52%, and `FindNearestBreak` 13.42%. `Boulder_Move` has only two `cmp` operand directions and four local rodata/string offsets left. `Boulder_Kill` differs at the second `cmp`, its short-versus-near `je`, and two compiler alignment nops. `FindNextBreak` has the right 352-byte size, but GCC orders the initial period case and several shared returns differently. `FindNearestBreak` uses a 0x2c stack frame in the current build versus 0x3c in retail; the current source spills its initial index to the argument slot and uses `edx` for the backward search where retail keeps the index in `ebp` and uses `eax` for the backward search. A combined punctuation predicate, ordered `goto` guards, and a local copy of the index have all been tried; those variants did not improve its match. These are codegen and block-placement problems, not missing text-boundary cases identified so far.
