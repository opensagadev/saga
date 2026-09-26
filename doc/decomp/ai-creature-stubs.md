# AI creature stub reconstruction notes

These notes cover `QueryLocalMessage` and `SpawnMeleeCreatureType` in the
Android x86 target. Measure with the GOT-aware `objdiff-cli` fork against
`res/libTTapp.so`; compile with NDK r8e GCC 4.7.

## Verify a stub's ABI before filling it

`QueryLocalMessage` was declared `void QueryLocalMessage(void)` by its stub,
but the target reads two stack arguments and returns a pointer. Its entire
body is a nullable second-argument choice: read the pointer at second
argument + `0x2c`, or at first argument + `0xc4`. Correcting the signature
and expressing the two returns directly matched the target at 100%.

When a stub's name gives no signature clues, check the target's argument
loads and return register before implementing it. A source-level `STUBBED()`
placeholder may have an incorrect return type or parameter list.

## Hoth melee wave fields and control flow

`HOTHBATTLE_MELEE_s::waves` begins at `melee + 0x08` with a stride of `0x28`.
Within a wave, `field_0x18` is the desired count and `reserved_1a` is the
current live count. `SpawnMeleeCreatureType` returns 1 when the quota is met;
it returns 0 when it cannot find a free slot, locator, or new object.

The spawn path tries up to 50 `spawn` locators. It rejects points inside both
camera planes 1 and 2 at offset `0.5f`, then accepts points inside either
plane at offset `2.5f`. It randomizes a horizontal displacement using the
character's collision radius and `NuFloatRand(&GAMERAND)`, except that the
AT-AT creation call uses the locator's original position. When the wave's
character ID is `id_ATAT` and its live count is zero, it first tries the
named `ATAT_EDIT` object. The target calls `KillGameObject(..., 4, 0)` on
other wave creatures whose offscreen timer exceeds `5.0f`.

## GCC control-flow observations

For the large spawn function, a local `u8` copy of the live count for the
cleanup loop improved the measured match because GCC keeps that byte live
across the loop. Re-reading `melee.waves[type].reserved_1a` at every source
condition caused repeated loads in the rebuilt loop. The target reloads it
after a kill, when the call may change global state.

NDK GCC 4.7 at `-O3` unrolled the four-entry free-slot search and used a
`0x8c`-byte frame in this reconstruction. At `-O2`, it kept a loop and used
the target's `0x9c`-byte frame, raising the direct-object match from about
48% to 56%. This is a useful flag check when a small fixed-count loop has
unexpected block order. Forcing a local variable into `%edi` with a register
declaration reduced the match and added spills; source lifetime and control
flow should be adjusted before imposing a register.
