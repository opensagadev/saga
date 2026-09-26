# Item collection and MiniKit codegen notes

These observations use the Android NDK r8e GCC target and the project's GOT-aware `objdiff-cli` fork.

## Intentional nop bodies

`SetupBlowupSfx` and `DrawTorpedoTargetSprite` are each eight one-byte `nop` instructions followed by `ret` in the target, despite having external callers. The project compiler flags already emit six entry nops for an empty C++ body. Add exactly two inline `nop` instructions to produce the target nine-byte body. Adding eight explicit nops produces fourteen.

## Local symbol ABI across source files

Some target-local functions are called from code in another recovered source file. `ProcessSpaceLevel(spacelevel_s*)` has the exact symbol `_ZL17ProcessSpaceLevelP12spacelevel_s` and receives its pointer in `%eax` (`regparm(1)`). A normal nonstatic C++ declaration changes the symbol and a plain external declaration changes the calling convention. Declare it with an explicit asm symbol label, hidden visibility, and `regparm(1)` at both definition and call sites. The MiniKit detector draw callback likewise uses an `_ZL...` local label even though reconstructed code needs a cross-file reference.

## Branch and conversion details

`CollectAllCharacters` has two separate loops selected before iteration. The first skips collection type 8; the second accepts only type 1. This affects GCC block order compared with a single loop whose predicate depends on a flag.

`MiniKit_GameMsg_Update` multiplies the result of `NuFmod(time, 4.0f)` by `0.25f`, then by `65536.0f` before integer truncation. Its `NuTrigTable` index masks the truncated angle with `0x7fff` directly; applying the more common angle right-shift would select different entries and instructions.

The MiniKit detector draw callback uses `ucomiss` branches for floating comparisons, including unordered (NaN) cases, and uses `cvttss2si` to truncate alpha. Preserve the source comparison directions and explicit casts when refining block order; algebraically equivalent tests can alter unordered behavior and branch layout.
