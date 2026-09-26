# Character action parser and snake codegen notes

These observations use the pinned Android NDK r8e GCC 4.7 at `-O3` and the
GOT-aware `objdiff-cli` fork. Development scores below compare the original
shared object against a directly compiled object. A subsequent Bazel link gave
these scores:

| Function | Target bytes | Rebuilt bytes | Linked match |
| --- | ---: | ---: | ---: |
| `SpecialMove_ConfigParticipant` | 274 | 274 | 99.97% |
| `SpecialMoves_Configure` | 794 | 749 | 85.51% |
| `DrawSnakeBody` | 448 | 445 | 79.76% |
| `UpdateSnakeBody` | 1117 | 1176 | 47.29% |

The participant helper's only linked differences are two literal string
addresses; its instruction shape matches the target.

## Parser block order

`SpecialMoves_Configure` at original `0x497b60` has two nested line loops with
one shared outer word-reading block. Its valid `specialmove_end` path increments
the count, reads the next line, and branches directly to that word block. An
ordinary nested `while`/`break` reconstruction placed the end validation and
attribute handlers in a very different order (15.64% direct match). Explicit
`outer_line`, `outer_word`, and `inner_line` labels produced the original first
`0x110` bytes and raised the match to 72.59%. Telling GCC that the end marker
branch is likely with `__builtin_expect(... == 0, 1)` moved validation into the
fallthrough path (81.49%). Marking a positive final move count as likely raised
it to 84.98%. Use these annotations only after comparing the actual target CFG.
This branch hint also made GCC place the participant helper in `.text.unlikely`
and changed its code from 98.92% to 9.53%. Adding `hot` to that helper restored
its original 274-byte shape and 98.92% score without changing the caller's
84.98% score. Check helper section placement whenever adding branch hints.

The local `SpecialMove_ConfigParticipant` uses the first two arguments in `eax`
and `edx`, with the remaining pointers on the stack. A static function marked
`__attribute__((regparm(2), hot))` reproduced the call convention and reached
98.92% direct match. Keep this helper next to `SpecialMoves_Configure`; a
placeholder in another translation unit creates a second local symbol.

## Snake stack and object access

`DrawSnakeBody` at original `0x4e94f0` uses `esi` for the object and `edi` for
the segment index, reloading `object->snake_body` after calls. Keeping a
long-lived local body pointer changed both registers and the loop CFG. Using a
`do` loop with direct `object->snake_body` access raised the direct match from
58.19% to 78.68%. Declaring the scale vector before the matrix and angle
vector moved scale to stack `esp+0x28` and angles to `esp+0x34`, matching the
target slots; the direct match rose to 79.32%.

`UpdateSnakeBody` at original `0x4e9090` copies the object's
`apiobj.lower_position` at offset `0x19c`, then adds `body->scale * 0.15f` to
Y. `apiobj.position` at offset `0x5c` is a different position and gave an
incorrect reconstruction. Direct access to `object->snake_body` also improved
the update routine's match from 30.54% to 46.84%, although its register and
stack layout still need refinement.
