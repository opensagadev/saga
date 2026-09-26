# Vader C update: GCC 4.7 control-flow observations

The Android x86 target's `VaderC_Update` is 4,536 bytes. The first C++
reconstruction was 4,510 bytes, but the GOT-aware `objdiff-cli` match was only
39.036438%. Similar size did not imply similar block order or register choice.

## Fixed-size array checks

At `-O3`, GCC expands the ten `vader_c.platform_ids` checks into separate
instruction sequences. In the target's two-player path, each array entry is
loaded and compared with `-1` separately for Player 0 and Player 1. Placing
both player checks below a shared `if (platform_id == -1) continue` caused the
rebuild to check `-1` once per entry, then reuse the loaded ID. The rebuilt
function retained the game behavior but diverged across the largest block.

When a target repeats a guard around two side-effecting checks, preserve the
two guarded expressions in source. Do not deduplicate a guard for readability
until the generated control flow has been checked.

## Boolean temporaries and block placement

Local `bool dead0`, `dead1`, and `both_controlled` values compiled into a
shift/xor/and sequence and a `cmove`. The target tests each flag and branches
to the next candidate. A short-circuit condition expresses the target's
branching structure more closely. This also matters when a large function
contains several out-of-line branches: the first reconstruction placed the
client obstacle reset before the host platform checks, while the target put
the host checks immediately after the entry guard.

The follow-up source change uses independent platform guards, a short-circuit
death predicate, and `__builtin_expect(netclient == 0, 1)` to guide block
placement. Its effect on the match percentage remains to be measured.
