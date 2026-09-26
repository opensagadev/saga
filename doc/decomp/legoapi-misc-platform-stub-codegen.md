# Misc platform stubs and mixed optimization

The four `legoapi/misc` files in this audit contained eleven `STUBBED()`
markers. Their target bodies fall into three groups:

| Target symbols | Target behavior | NDK r8e object diff |
| --- | --- | ---: |
| `StillMemRestore`, `StreamSeek`, `StreamCache`, `DebugLog`, `Debug_Print` | Empty body: eight `nop` instructions and `ret` | 100% each |
| `StreamRead`, `StreamCacheCheckComplete` | Return zero: `xor eax,eax`, six `nop`, `ret` | 100% each |
| `DEVCDDVDROM_Interrogate`, `DEVMEMORYCARD_Interrogate` | Store `1` to `NUFILE_DEVICE::status` at offset `0xc`, return `1` | 100% each |
| `bgprocIsFrozen` | Return `bgproc_frozen` through the GOT | 97.857% object diff; only unresolved relocation operands differ |
| `MemFileBoundsCheck` | Compute `memfiles[file].end - memfiles[file].ptr + 1` and store the otherwise unused result | 89.412% object diff |

The empty and constant-return functions have nine-byte target bodies, but the
translation units' default `-O0` setting makes an ordinary empty C++ body a
five-byte frame-based function. On NDK r8e GCC 4.7, a function-level
`optimize("O2", "omit-frame-pointer")` attribute restores the target no-frame
body. `optimize("O2")` alone leaves the `-O0` frame-pointer setting in effect;
`#pragma GCC optimize("O2", "omit-frame-pointer")` alone did not change these
functions when placed after the headers. The two options must be specified in
the function attribute in this configuration. Check both the body and the
return type: the target `StreamRead` and `StreamCacheCheckComplete` symbols
return zero even though their initial placeholder declarations were `void`.

`MemFileBoundsCheck` is an unoptimized exception in the same source as the
optimized no-op `StillMemRestore`. The simple C++ expression reproduces the
target's repeated GOT loads, 20-byte record stride, frame, register order, and
unused stack result. The remaining 2-byte size difference is repeated twice:
the target uses `lea (base,index),eax; mov 4/8(eax),eax`, while GCC currently
uses `add base,eax; lea 4/8(eax),eax; mov (eax),eax`. Cast-based pointer
expressions achieved the target size but disturbed register choice and dropped
the match to about 67%, so keep the straightforward expression pending a
better source-level fit.

For object-only verification without a linked build, compile these sources
with the NDK r8e x86 GCC, Android 9 x86 sysroot, and `-O0` as the base mode,
then run the user's GOT-aware `objdiff-cli diff` against each object symbol.
Relocations in an object file can account for a small reported mismatch even
when the final linked instructions have the target shape, as with
`bgprocIsFrozen`.
