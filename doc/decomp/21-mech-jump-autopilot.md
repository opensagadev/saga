# Jump autopilot reconstruction notes

The Android x86 target's `MechJumpAutoPilotAddon` has seven contiguous `VuVec`
values at offsets `0x24`, `0x34`, `0x44`, `0x54`, `0x64`, `0x74`, and `0x84`,
followed by a float at `0x94`. Each vector's fourth component is at the
`+0xc` offset. Treating these as separate 12-byte `NUVEC` values shifts every
later access and prevents a match. In the target, the `0x44` vector is the
current simulated position, `0x54` is simulated velocity, `0x64` is the
horizontal landing plane intersection, and `0x74` is a terrain intersection.

`LookForLandingSpotAroundPoint(VuVec const&)` returns `bool` even though the
old declaration said `void`. The mangled name omits the return type, so a
wrong declaration still links. Its caller tests `%al` immediately after the
call. Verify caller instructions when a stub's return type seems doubtful.

The `OnProcess` state dispatch has a six-entry jump table for states `0..5`.
State `1` analyzes the trajectory, `2` chooses a landing point, `3` adjusts
the jump, `4` calls the empty `ModifyJump`, and `5` calls the empty
`ProcJumpingToCertainDoom`. State `0` only accumulates elapsed time. The
`LEGOCONTEXT_JUMP` GOT slot in this binary resolves to the global at
`0x006672b4`; the target compares a signed byte character context to the
32-bit value at that address.

The bottom plane test uses exactly `0x1p-23f` (float bits `0x34000000`) as
an epsilon. Its ratio condition is surprisingly `t > 0.0f || t <= 1.0f`:
the target's first comparison jumps to a cold block for `t <= 0`, and that
block jumps back into the intersection body when `t <= 1`. Thus every finite
ratio reaches the body; NaN does not. Do not replace this with the usual
interval test. The trajectory simulator takes at most 50 steps of `0.1f`
and stops when the height falls below the initial height minus `2.0f` or a
terrain contact changes the state.
