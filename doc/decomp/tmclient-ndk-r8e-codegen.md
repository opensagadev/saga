# TMClient handle allocator: NDK r8e code generation

The original `TMClient` methods form one contiguous `-O3` translation unit:
constructor, `AllocHandle`, `Connect`, `SendTTY`, `FOpen`, `FClose`, then the
remaining empty methods. `src/nu2api/nufile/tmclient.cpp` already has the
`-O3` per-file override. Defining these methods in an unlisted `-O0` source
file changes every substantive function's shape.

`AllocHandle` scans sixteen 32-byte slots, starting after a static local
index, wrapping with `& 15`. GCC 4.7 at `-O3` completely unrolls this fixed
count loop into sixteen `testb $1, (this, index)` probes. The same source at
`-O2` retains a loop. This was verified with the repository's NDK r8e compiler
using `-S -x c++ -o - -`, so the probe wrote no project files.

The slot's active state must be a one-bit `unsigned` bitfield. With a plain
`unsigned char`, GCC emits `movzbl` followed by a register `testb` and saves
an extra register. With the bitfield, it emits the target's direct memory
`testb` and shared success block: write the static index, shift it by five,
`orb $1` into the slot, and return. On sixteen occupied slots it writes the
index and returns `-1`. `FClose` clears the same bit with `andb $0xfe`.

The local command buffers in `SendTTY` and `FClose` are `0x10c` and `0x2c`
bytes. Increasing either by four moved the local address from target
`esp+0x14` to `esp+0x10` even though the total stack frame size stayed the
same. Writing the FClose suffix as `index > 9 ? index + 'W' : index + '0'`
produces the target's `jg` branch and fallthrough order; the equivalent
`index <= 9` expression reverses them.

Direct NDK object comparison to the target linked ELF produced the exact
function sizes and instruction sequence. The remaining fuzzy differences
are calls, PIC/GOT offsets, and local literal relocations, which cannot have
the same numeric address in an unlinked object.
