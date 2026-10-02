# 07 — Mismatch diagnostics

> Agent/reference document. Human matching workflows are in
> [`CONTRIBUTING.md`](../../CONTRIBUTING.md).

Scope: diagnosing differences between `res/libTTapp.so` and the Bazel target
`bazel-bin/src/libTTapp.so` built with NDK r8e GCC 4.7.

## Investigation loop

```bash
bazel build --config=target //src:saga_target
bazel run //scripts:objdiff_cli -- SYMBOL
bazel test //scripts/checks:checks
```

For the affected source file, also confirm its effective optimization options:

```bash
rg -n 'source/path\.cpp' bazel/android_per_file_copts.bazelrc
bazel aquery --config=target \
  'mnemonic("CppCompile", deps(//src:saga_target))' --output=commands
```

No optimization-map entry means `-O0`.

## Diagnose by symptom

### Whole function has a different shape

Check the optimization level first. An accidental `-O0`/`-O2`/`-O3` mismatch
changes frame setup, reloads, inlining, branches, and register allocation at
once. Fix the build metadata rather than contorting the C source.

### Branch direction or compare operands differ

Signedness and source ordering both matter. `a > b` and `b < a` can produce
mirrored `cmp`/jump sequences. Confirm the ABI types, then try the source form
that expresses the original operand order.

### `setcc`, branch, or `cmov` differs

Check whether the result type is `bool` or `int`, whether both branches assign a
value, and whether the condition has side effects. Ternaries, early returns,
and assignment-then-return forms are not interchangeable on GCC 4.7.

### Extra loads or missing reloads

Look for cached globals, pointer aliases, and `volatile`. Add `volatile` only
when the original behavior requires an observable reload; it is not a generic
matching hint.

### Float instructions differ

Confirm `float` versus `double`, literal suffixes, intermediate casts, and the
comparison result type. The target normally uses SSE math; x87 often signals a
conversion or ABI boundary.

### Calls appear or disappear

At optimized levels, inspect inlining, static linkage, and constant propagation.
At `-O0`, verify that a helper was not accidentally declared `inline` or moved
to a different source file.

### Tail call versus call-and-return

Small changes to return expressions, cleanup, or local lifetimes can inhibit a
tail call. Compare the final source operation with the original control flow.

### Every argument read is off by four bytes

If each `N(%esp)` argument load is four bytes higher than the original, the
reconstruction has an extra leading parameter. A common cause is a method that
the original declared `static`: static and non-static members mangle
identically, but only the latter receives `this`. Callbacks with two same-typed
parameters can also have the used and ignored parameter swapped.

### Only the scratch register of a load-and-test differs

With Atom tuning, peephole2 rewrites `cmp $0, MEM` as `mov MEM, %reg; test
%reg, %reg`. GCC 4.7 picks `%reg` with `peep2_find_free_register`, whose
static `search_ofs` rotates through the allocation order and persists across
every function compiled in the same translation unit. The chosen register
therefore depends on how many such rewrites preceded the function in compile
(output) order, not only on the function itself: the same source compiled
alone can pick a different register. A tiny function that differs only here
is usually correct; the mismatch comes from an earlier function in the
translation unit that differs, is missing, or is emitted in a different order.
Do not rewrite the function to chase the register.

The usual cause of that mismatch is function order. This compiler emits a
translation unit's definitions in source order, including at `-O2`/`-O3`, so
the original binary's address order within a unit is the original source
order. Reordering definitions to that order (with no body changes) has
repaired many single-register mismatches, for example in `edfile.cpp` and
`ledges.cpp`. Measure the whole unit afterwards: some Bazel units combine
functions from more than one original unit and get worse when reordered.

### Only PIC-relative displacements differ

When every remaining difference is a `GOTOFF` displacement (`[ebx-0x9b2c0]`
versus `[ebx-0xb4648]`, a renumbered `.LC` label, or a `.data`/`.bss` static
at `[ecx+0x86a0]`), the function body is already correct. Those displacements
encode the distance from the GOT to the referenced `.rodata`/`.data`/`.bss`
item, so they only match when the data layout of the whole binary matches.
No per-function source change can fix them; move on to another function.

### `setcc; test %al, %al; jcc` in a plain `-O0` condition

A straight-line `-O0` `if` normally compiles to a fused `cmp`/`jcc`. The
`setcc`/`test` form appears when the condition is gimplified into a temporary:

- an assignment inside the condition, `if ((p = f()) != NULL)`; or
- a `volatile` operand, `if (shared_size == decoded_size)`. A `volatile`
  object that is tested with an empty body also keeps its otherwise dead
  load and `test`.

Check the variable's other uses before declaring it `volatile`; data shared
with another thread (for example the file decode buffers) is the expected case.

### `-O0` call frame is 16 bytes larger than the original

At `-O0`, a call to a `static` function that is already defined earlier in
the unit does not reserve the usual 16-byte-aligned outgoing area: a
one-argument wrapper uses `lea -0x8(%esp),%esp` instead of `-0x28`. If the
original frame is smaller, define the static callee before its callers rather
than forward-declaring it (`NuSinApprox3` in `nutrig.cpp`).

### Two `-O0` locals share one stack slot in the original

If two of the reconstruction's locals live in one slot in the original, the
original source reused a single variable for both purposes. Merge them;
renaming alone does not change the slot.

### Two or three stores appear in reverse order

A chained assignment `v.x = v.y = v.z = k` stores `z` first, while an
aggregate initializer or separate statements store `x` first. Choose the form
that reproduces the original store order.

### A deleting destructor calls a different deallocator

If the original `D0` destructor calls `BlockFree` (through
`NuMemoryGet()->GetThreadMem()`) or `MemoryManager::FreePool` where the
reconstruction calls the global `operator delete`, the class (or a base
class) declares its own `static void operator delete(void *)`. Use the
existing `NU_FREE` form or a pooled `FreePool(pointer, sizeof(Class))`; every
derived class that does not declare its own delete inherits it.

### A derived destructor calls a base destructor that the original inlines

Compare symbol bindings with `nm`: a base destructor that is weak (`W`) in the
original but strong (`T`) in the reconstruction was defined inline, so the
original inlines it into derived destructors. Marking the existing definition
`inline` (or moving it into the class) reproduces that. The reverse also
happens: a class with only an implicit destructor gets an inline one, while
the original defines it out of line and its `D0` calls `D1`.

An `inline` definition in a `.cpp` file is only visible to that unit. If
another unit calls the destructor out of line (a derived class, or an
explicit destructor call), host builds fail to link with an undefined
reference. Move the definition into the header so every user can inline it,
as the original's weak copies imply. This is only safe when the destructor
is not the class's key function: when it is the first declared virtual,
making it inline turns the vtable and typeinfo into vague-linkage symbols
that GCC 4.7 emits in unrelated units, which the `-fno-rtti` target cannot
link. Such a destructor keeps its `inline` definition in its unit (or takes
`SAGA_HOST_LINKABLE_DTOR` from `decomp.h`) so the matching target preserves
the original's inlining while host builds get a strong, linkable definition.

### Only the static initializer shape differs between `-O2` and `-O3`

`_GLOBAL__sub_I_*` functions are a useful witness for a unit's optimization
level. When a unit's static initializer only reproduces the original at the
other level, and the unit's other functions are equal or closer there,
change the level in `bazel/android_per_file_copts.bazelrc`. Check the whole
binary afterwards: compiler-generated `-O0` helpers such as
`__static_initialization_and_destruction_0` are paired by name, so removing
one can unpair an unrelated original copy without any real regression.

### A static callee uses register arguments in the original

GCC gives a `static` function a local register calling convention (arguments
in `eax`/`edx`, tail calls as `jmp`) when every caller is in the same unit.
If the original passes arguments in registers but the reconstruction uses
the stack, look for a caller in another file reaching the helper through an
`__asm__` symbol name. Moving those callers back into the helper's unit and
making the helper `static` again restores the convention.

### Stack realigns with `and $-16, %esp`

Plain `-O2` functions do not realign the stack. A frame pointer plus
`and $0xfffffff0, %esp` in the original means some local needs 16-byte
alignment even when it holds no vector data. Give the corresponding local an
`aligned(16)` type (for example `NUMTX_ALIGNED16`) and check that it lands at
the same aligned frame offset.

### Stack size or member offsets differ

Recheck structure layout, packing, field order, pointer width, enum width, and
`abi_long`/`abi_ulong`. Do not patch offsets manually to compensate for an
incorrect type.

### Global is in `.data` instead of `.bss`

Zero-initialized globals belong in `.bss`; nonzero initializers belong in
`.data`. Match the section and initialization behavior, not only the runtime
value.

### Function or literal addresses drift

Target builds disable function/data sections, so definition order and first use
of string literals can affect layout. Check for inserted helpers, moved
definitions, and reordered literals before changing unrelated code.

### Symbol is missing

Confirm exact mangling, C/C++ linkage, namespace/class ownership, constness,
parameter signedness, and whether the function became local or inline. Use:

```bash
bazel run //scripts/checks:check_symbols -- --list
```

### Extra symbol appears

Determine whether it is intended reconstructed code, a compiler-generated
clone, or an accidental public helper. Prefer local/static linkage for helpers
that are not present in the original symbol surface.

## Verified GCC 4.7 reminders

- `-O0` stack allocation commonly uses `lea`, not `sub`.
- `-fno-exceptions` does not remove static-local guard variables.
- `int` and `long` have equal width but different C++ mangling.
- `__builtin_expect` can change layout and is not a no-op.
- Zero initialization can change both section placement and instruction shape.
- Modern-compiler intuition is not evidence for GCC 4.7 output.

## Minimal experiment

When a source-form question remains ambiguous, create a temporary tiny file
outside the repository, compile it with the same NDK compiler and relevant
optimization level, and inspect it with `i686-linux-android-objdump -dr`. Keep
the experiment narrow enough to answer one code-generation question.

## Completion checklist

1. The target rebuilds successfully.
2. The affected symbol's diff improves or disappears.
3. Its symbol name and linkage remain correct.
4. The source file still has the intended optimization level.
5. `bazel test //scripts/checks:checks` passes.
