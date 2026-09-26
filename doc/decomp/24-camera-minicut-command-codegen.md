# Camera mini-cut command construction

`GameCameraMakeMiniCut3` is a 2412-byte target function in the NDK r8e x86
build. Its 19 arguments continue beyond the register saves at stack offsets
`0x60` through `0xa8`. When reading the target disassembly, account for the
four saved registers and the 76-byte local area before identifying arguments.

The function builds a list of `Minicam_AddCommand` calls in a fixed flag order:

| Flag | Command | Value |
| --- | --- | --- |
| `0x80` | 8 | Focus pointer |
| `0x100` | 16 | Position vector |
| `0x2000` | 17 | Spline pointer |
| `0x1` | 9 | Distance float |
| `0x10`, `0x2` | 13, 10 | Pitch integer |
| `0x20`, `0x4` | 14, 11 | Yaw integer |
| `0x40`, `0x8` | 15, 12 | Roll integer |

Mode and easing arguments different from `-1` add commands 6 and 7. A
`0x200` flag resets the mini camera, adds command 1, and seeds its target and
position before calling `GameCameraMakeMiniCut2`. The target passes
`1000000000.0f` as the cut end time and forwards the byte at `MiniCam+0x384`
as its borders argument. That byte is set to 1 at the end when the caller's
border argument is nonzero.

The duration flags `0x800` and `0x1000` create commands 4 and 5. If neither
duration flag nor `0x400` is present, the function forces `0x800` and zero
blend time. A `0x400` flag adds final command 3 and copies the blend-out time
to `ObstacleCamBlendOutTime`. When flags `0x1` and `0x4` coexist, blend-in time
is positive, and blend time is zero, the function substitutes `0.01f` for
blend time. The target's read-only constant at `0x576748` has bytes
`0a d7 23 3c`.

The target issues each command with a by-value `NUVEC` argument, even when the
command ignores it. Those calls repeatedly load the three words of global
`v000` into outgoing stack slots. This explains much of the large function's
size and means a shared helper or cached local zero vector can harm matching.

The first reconstruction is intentionally pending integrated build and
GOT-aware objdiff measurement. Its control flow is based on target assembly,
and the exact duration block order remains to be tuned after fan-in.
