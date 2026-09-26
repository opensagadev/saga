# Android mouse stubs

The Android `numouse.cpp` target contains fixed-return functions. The four
integer button readers compile to `push ebp; mov ebp, esp; mov eax, 0; pop ebp;
ret` (10 bytes). The nine floating readers load the same zero constant through
the PIC literal pool, pass it through `xmm0`, then return it on the x87 stack
(38 bytes). `NuMouseRead` is an empty five-byte function.

Removing `STUBBED()` while retaining `return 0` or `return 0.0f` reproduces
the target shapes under NDK r8e GCC. A direct translation-unit object compared
with the GOT-aware fork of `objdiff-cli` gives 100% for each integer reader and
98.545456% for each floating reader. The floating mismatch is confined to
the unresolved PIC thunk, GOT base, and literal-pool address arguments in the
unlinked object; the instruction sequence and size agree. Recheck against the
fully linked `bazel-bin/src/libTTapp.so` when the queued build completes.
