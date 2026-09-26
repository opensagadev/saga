# GCC 4.7 branch prediction and block layout

This page records why source order often differs from the Android x86 target's machine-code order. It applies to optimized functions, especially the `-O2` touch task translation units. Read [the toolchain notes](01-toolchain.md) and check the translation unit's effective flags before applying it elsewhere.

## Version and scope

The NDK r8e `i686-linux-android-g++` binary used by our Bazel toolchain reports `gcc version 4.7 (GCC)` and was configured for `i686`, Atom tuning, and SSE floating-point math. The [NDK r8e x86 setup](https://android.googlesource.com/platform/ndk/+/a82a10a1953e6762e3fee226f7910e00237b08b1/toolchains/x86-4.7/setup.mk) lists `-O2 -g -DNDEBUG -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300` among its release flags. Our actual Bazel flags come from `.bazelrc`, `bazel/android_per_file_copts.bazelrc`, and the selected toolchain, so inspect a compile action before assuming every NDK makefile flag applies. The checked-in NDK source download script dates its toolchain source snapshot to 2013-03-19, but the compiler's `-v` output does not establish an exact upstream patch release. The upstream GCC 4.7.2 source below is an algorithm reference, not proof that Android's fork is identical.

The [GCC 4.7 optimization manual](https://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Optimize-Options.html) documents that `-O2` enables `-freorder-blocks`; probability guessing is also enabled by default at this level. As a result, C++ statement order only establishes a starting control-flow graph. It does not reliably choose the final fallthrough or nearby machine-code block.

## The layout pass

The [upstream 4.7.2 `bb-reorder.c`](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-4.7.2/gcc/bb-reorder.c) describes a greedy Software Trace Cache algorithm. Its first trace starts at the function entry. It repeatedly appends the most probable successor, defers lower probability or colder successors, and finally joins the traces. It can rotate short loops or duplicate a block when joining traces.

The ordinary pass uses four rounds. Its branch probability thresholds, in thousandths, are `400, 200, 100, 0`; successor execution-frequency thresholds are `500, 200, 50, 0`. Lower thresholds bring deferred blocks into later traces. These are **layout thresholds**, not the predicted probability assigned to a particular `if`.

When choosing between eligible successors, `better_edge_p` first considers branch probability. Probabilities within roughly 10% of the current best probability are treated as a tie; the next preference is the *lower* successor frequency, then the original adjacency. This can make an apparently colder edge follow the current block. The pass also preserves an existing fallthrough edge for some blocks ending in a call, so a source rewrite near a call can have limited influence. Inspect the concrete RTL dump rather than assuming the visually obvious hot branch will be adjacent.

## Where probabilities come from

The upstream [`predict.def`](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-4.7.2/gcc/predict.def) defines heuristics that feed the CFG: floating-point comparisons have a 90% default prediction, non-null pointers 85%, guarded calls 71%, early returns 61%, and loop exits 91%. These numbers apply to the heuristic's selected edge and may be combined or superseded by other predictions. They are not a blanket probability for every branch of that kind.

Changing `if (x <= limit)` into `if (x > limit)`, moving an early return, or changing the lifetime of a value across a call may change the CFG, prediction, trace membership, and register allocation. For floating-point values, first check the exact `ucomiss` operands and `ja`/`jbe`/`jp` sequence: algebraically reversed comparisons can change NaN behavior. Preserve the target's semantics while tuning layout.

## Matching workflow

1. Confirm the translation unit's effective `-O` level and toolchain in a Bazel compile action. Do not apply `-O2` broadly just because a single function benefits; other functions in the same file may already match.
2. Draw the target CFG from the disassembly, including fallthrough edges, calls, loops, and any out-of-line cold blocks. Compare conditions and NaN behavior before changing source polarity.
3. Use the same NDK compiler and flags with `-fdump-rtl-bbro` to see `STC - round`, trace membership, frequencies, and final order. The [GCC 4.7 debugging options](https://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/Debugging-Options.html) document this dump. If Bazel discards undeclared outputs, capture its compile action and rerun that one translation unit outside the sandbox with the dump flag.
4. Make one source-shape change at a time, rebuild, and measure the symbol with the GOT-aware fork of `objdiff-cli`. A byte mismatch caused by local GOT displacement is not a control-flow mismatch.

Observed touch-task examples are in [21-mech-context.md](21-mech-context.md). In `PlannedGoTo::Update`, the target's jump path tests `distance_squared > 1.96f || vertical_delta > scaled_height`; writing the inverse condition changes the emitted branch and out-of-line block placement. In the same function, a waypoint reference held across `CanJump` changes register lifetimes and spills even when the high-level result is the same.
