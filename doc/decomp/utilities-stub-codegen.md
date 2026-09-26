# Utility stub recovery notes

These notes cover `src/legoapi/misc/utilities.cpp`. Diff scores must come from
the GOT-aware `objdiff-cli` fork, with the original `res/libTTapp.so` as side
1 and the NDK r8e target build as side 2.

## Return types hidden by mangling

The C++ mangled names do not encode ordinary return types. Several old stubs
were declared `void`, yet the original routines return values:

| Symbol | Recovered return | Original size |
| --- | --- | ---: |
| `LineCrossedXZ` | `i32` (0, 1, or 2) | 226 bytes |
| `RatioAlongLineXZ` | `f32` | 253 bytes |
| `LineToPlaneDistance` | `f32` | 203 bytes |
| `I64ToX` | `char *` (output + 16) | 302 bytes |
| `XToI64` | `i64` | 819 bytes |
| `rawClip` | `i32` (emitted vertex count) | 624 bytes |

Check the return registers and callers before retaining a placeholder return
type. `CatI64ToX` uses the pointer returned by `I64ToX` to append the NUL.

## Hex conversion codegen

`I64ToX` writes the high 32-bit word followed by the low 32-bit word, eight
lowercase hex digits each. It writes no terminator. GCC builds a 17-byte local
`char hex[] = "0123456789abcdef"` array and emits direct stores for all 16
digits. A loop or a call to `IToX` produces a different body.

`XToI64` is also fully unrolled. For each of exactly 16 signed input chars,
the original calculates `decimal = digit - '0'` and
`letter = digit - 'W'`, chooses the latter for `digit >= ':'`, then shifts
and ORs a signed 64-bit accumulator. This unusual test is what produces the
target `cmp 0x3a; cmovl` sequence. It does not validate characters or stop
at a NUL. The first decoded digit sign extends into the 64-bit accumulator.

## Geometry and clipping control flow

`LineCrossedXZ` evaluates four signed 2D cross products in order, with early
returns. Reordering algebraically equivalent terms changes SSE register
allocation and float rounding. `RatioAlongLineXZ` rotates the query point and
line direction by the negated `NuAtan2D` angle through `NuTrigTable`, then
clamps the projected ratio to 0..1. `LineToPlaneDistance` computes signed
plane distance at both endpoints and returns the closest distance only when
both are strictly on the same side; a crossing returns zero.

`rawClip` traverses the fixed, 96-byte `cubeEdgeIndices[12][2]` table. The
third argument is unused. An endpoint is kept only for strictly positive
plane distance. Each edge emits either zero vertices or a pair; for a crossing
the inside endpoint is copied first, then the interpolated point with `w=0`.
Copies preserve the source `w`. The emitted vertex count ranges from 0 to 24.
The table is an original local symbol named `_ZL15cubeEdgeIndices`, so keep
its name and 32-bit integer layout for matching.

## NDK r8e GCC matching details

For a negative SSE branch, `if (value < 0.0f)` can make GCC swap the
`ucomiss` operands and use `ja`. In `LineCrossedXZ`, the original uses
`ucomiss value, zero; jb`; spelling the condition as
`if (!(value >= 0.0f))` reproduces it. The final branch keeps return value 2
in EAX while calculating the fourth cross product. Empty `+a` and `+x`
constraints anchor that register value after the first float load. Together
these yield a 100% instruction match for the 226-byte routine.

`__builtin_fminf` and `__builtin_fmaxf` emitted external calls with this
toolchain. The original `LineToPlaneDistance` uses single instructions
`minss` and `maxss`; inline SSE operations on the scalar accumulator make
its 203-byte body match exactly.

First full target build, measured with the GOT-aware fork:

| Function | First attempt | Refined |
| --- | ---: | ---: |
| `LineCrossedXZ` | 88.490560% | 100% |
| `RatioAlongLineXZ` | 86.566666% | 92.316666% |
| `LineToPlaneDistance` | 78.857140% | 100% |
| `I64ToX` | 40.303370% | 61.079% |
| `XToI64` | 35.805460% | 36.468% |
| `rawClip` | 22.210192% | 54.720000% |

For `rawClip`, copying the inside endpoint as a whole `VuVec` made GCC
load and store it through general-purpose registers. Explicit x/y/z/w field
stores reuse the XMM values already loaded for the plane dot product, as in
the target. The interpolation branches store y, z, x and zero the new w
before the divisions. Those changes raised the score from 22.21% to 54.72%.
The target also aligns the stack to 16 bytes, but forcing alignment with an
extra aligned local grew the frame from 16 to 32 bytes and reduced the score;
do not retain that particular probe.

The 64-bit formatter is sensitive to where the word halves become live. Keep
the low word extraction after digit 7 and a memory scheduling barrier after
digit 1. A no-op ESI/EAX constraint for the high word and output pointer,
followed by a memory clobber before constructing the local digit table, gets
the target's two saved registers and raises the score. Extracting the halves
with `__builtin_memcpy` avoids strict aliasing concerns and another ten
points of compiler mismatch. The parser is harder: forcing the input pointer
to ECX shrinks its generated body from 1040 to 789 bytes, near the target's
819, but instruction scheduling still differs heavily. Its first-attempt and
refined scores are in the table above.

Further direct NDK r8e object probing of `rawClip` reached **59.732483%**
(610 generated bytes versus 624 original). The x86 branch uses five SSE
instructions to copy the second inside endpoint; ordinary aggregate assignment
and `__builtin_memcpy` both emitted general-purpose register copies instead.
Aligning the live count local to 16 bytes also reproduces the original
`and esp, -16` prologue and 16-byte frame. The linked shared-object score for
this last probe still needs measurement; direct-object scores differ slightly
because calls and GOT references have unresolved relocations.
