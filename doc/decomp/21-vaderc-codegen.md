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

Commit `53289dd3` tried independent platform guards, a short-circuit death
predicate, and `__builtin_expect(netclient == 0, 1)` together. The combined
probe reduced the match and was reverted by `32b912ea`; it does not tell us
which of the three changes caused the regression. Keep later probes separate.

## Candidate block and integer-width probes

The target entry at `0x2110ed` jumps forward for `netclient != 0`, leaving
the host platform scan at `0x211122` as fallthrough. The current build does
the reverse: it jumps to the host body at `0x39f3a8` and falls through the
client path. A separate, untested source probe is an early
`if (netclient != 0) goto client_obstacle_reset;`, followed by the host guard
and body, with the label immediately before the existing client condition.
Retain that second `netclient` check: target code reloads it at `0x211490`
after calls that may change global state. GCC may still reorder the blocks,
so measure the resulting CFG before keeping the edit.

The target's repeated single-player platform checks sign extend a 16-bit ID
at `0x211962` and compare its low word directly with the player field at
`0x211978`. Current code zero extends the ID at `0x39f8d3`, loads the player
field separately, then compares two registers. Try `i32 platform_id =
vader_c.platform_ids[i]` in place of the `i16` local, and put the local on
the left of each equality comparison. This may alter GCC's unrolled loop and
register allocation in several repeated checks; it remains untested.
