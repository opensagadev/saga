# Host utilities

> Agent/reference document. Human host workflows are summarized in
> [`CONTRIBUTING.md`](../CONTRIBUTING.md).

The host executable is a diagnostic aid for the decompilation. A successful
host run does not justify behavior that is absent from the original binary;
game-side fixes must still come from the original code, data, and ABI.

## Save-file inspection and editing

The `save` utility runs without opening a window, starting audio, or loading an
OBB. Paths refer to individual `SaveGameN.*_SavedGame` files, including the
separate SuperOptions save. Only the reconstructed Android/mobile envelope
(version 1, `0x2028`-byte header) with a `GAMESAVE_s` or `SUPEROPTIONS_s` payload
is accepted; unrelated PC/console formats are not inferred from their names.

The default file is
`res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame`, relative
to the working directory (the repository root when using `//src:run_native`).
The filename comes from the game's `slotname(0)` routine. Omit the path for
`list`, `edit`, or `create`; bare `save` is equivalent to `save list`. Creation
still refuses an existing destination. The parent directory must already exist.

Interactive native gameplay and the utility both use `res/SavedGames/`.
Slot 0 holds game progress; slot 3 holds SuperOptions, including store
pack entitlements. Scripted window runs use their own
`.work/host-documents-scripted/<run>/SavedGames/` directory. Reload the save after
editing; an already running game retains its in-memory progress and can overwrite
external edits when it saves.

```sh
bazel run --config=native //src:run_native -- save list \
  'res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame'
```

```sh
bazel run --config=native //src:run_native -- save list
bazel run --config=native //src:run_native -- save list --filter coins
bazel run --config=native //src:run_native -- save edit coins=50000
bazel run --config=native //src:run_native -- save schema
bazel run --config=native //src:run_native -- save schema --filter area_save
bazel run --config=native //src:run_native -- save schema --options
```

`schema` explains every property as a bordered, aligned table with wrapped
descriptions. Use `schema --tsv` for tab-separated output: property name,
storage type, first element's absolute file offset, element byte size, count,
stride in bytes, accepted values, and description.
Its fields and offsets come from the same canonical structure registry as
`list` and `edit`. Identical array elements share one inclusive, zero-based range:
`area_save[0..71].minikit_count`, `episode_save[0..5].flags`, or
`customizer.pieces[0..8]`. An area's fields have count 72 and stride 12;
element `i` is at `file_offset + i * stride` for these zero-based ranges.
Scalar rows have count 1 and stride 0. Byte buffers remain compact `bytes[N]`
types. Unknown fields and padding are included. `list` still emits individual
stored values, and edits still use individual indices rather than range syntax.
Unknown meanings are explicitly labeled, and numeric ranges describe storage
limits rather than valid combinations of gameplay progress.

Without a path, `schema` describes the standard game layout without reading or
creating a file; `--options` selects the SuperOptions layout. With an explicit
path, it detects the payload type and includes the file's extra-data prefix in
its offsets. `--filter text` works with either layout and with `list`; the older
`list <path> <filter>` form remains supported. Use `--path <path>` in place of
the positional path for filenames beginning with `--` or containing `=`.

Key schema distinctions:

| Property/family | Meaning |
|---|---|
| `coins` | Stud/coin balance. |
| `completion` | Completion **points**; displayed percentage depends on the game's `COMPLETIONPOINTS` total. |
| `gameplay_seconds` | Accumulated game time in seconds. |
| `difficulty` | AI difficulty threshold (initially 5), not a save-format version. |
| `level_save[i].*` | Ten pickup-name slots, pickup count and arcade win flags; 366 record slots of 84 bytes. |
| `area_save[i]` | Area progression, story/free-play buildup completion, minikits, and challenge time; `i` is a game area ID. |
| `episode_save[i]` | Super Story best time, best score, and completion status. |
| `mission_save.*` | Twenty best times followed by twenty completion bytes. |
| `hint_completion_bits` | One 192-bit logical set: console hints at bits 0..95, touch hints at bits 96..191. |
| `shop_*_purchased_bits` | Shop-list purchase indices, separate from character IDs and tutorial hints. |
| `extra_unlocked_bits`, `extra_purchased_bits` | Separate unlock/purchase masks indexed by the original 44-entry `Cheat` table. |
| `character_save[i]` | `AVAILABLE=1` (owned/playable), `UNLOCKED=2` (unlocked for collection). |
| `suit_flags` | Shared-engine Batman suit mask, separate from store purchases. |
| `reward_flags` | `100_PERCENT=1` and `ALL_GOLD_BRICKS=2` reward callbacks already fired. |
| `customizer.*` | Both custom characters' piece IDs, fixed-size names, and stored-name selection flags. |
| `options.*` | Separate SuperOptions data, including touch positions, control selection, music, and pack unlock bits. |
| `checksum`, `slot_code` | Automatically derived fields; preserving manual values requires `--keep-derived`. |
| Other `field*` and opaque arrays | Undecoded or reserved storage; consult `schema` for each field's recovery status. |

```sh
bazel run --config=native //src:run_native -- save list '/path/to/save'
bazel run --config=native //src:run_native -- save list '/path/to/save' area_save
bazel run --config=native //src:run_native -- save edit '/path/to/save' \
  coins=50000 'area_save[0].minikit_count=10' 'character_save[12]=3'
bazel run --config=native //src:run_native -- save create '/path/to/new-save' \
  --from '/path/to/template' coins=100000 'customizer.primary_name=text:Player'
bazel run --config=native //src:run_native -- save create '/path/to/new-save' coins=1000
bazel run --config=native //src:run_native -- save create '/path/to/options-save' \
  --options options.music_enabled=1 options.left_control_x=0.25
```

`list` emits `key=value` lines covering **every byte**, including all header
buffers, padding, flags, unknown regions, checksum, and slot code. Names and
offsets come from the canonical save structures. Large byte arrays are emitted
in full as `hex:...`; their individual bytes can be edited with `member[index]`.
`byte[N]` addresses any absolute file byte, with decimal or `0x` hexadecimal
offsets. Unknown fields retain their existing placeholder names: exposing the
bytes does not imply that their gameplay meaning has been recovered.

Integers accept decimal or `0x` hexadecimal values with strict width/range
checks. Signed fields accept negative decimal values. Float fields accept finite
decimal values. Byte arrays accept an exact-length `hex:...` value, or
`text:...` with zero padding and space for a terminator. Names are fixed byte
buffers, not an encoding conversion API. Quote indexed properties in shells
such as zsh. Indices are zero-based; level/character/area IDs are game-data IDs.

Flags accept the enum names shown by `schema`, joined with `|` for bit masks.
`list` includes each enum field's raw number and, for masks, the set bit numbers
and their names in comment lines. The following `key=value` line is still
directly editable/importable. Unknown scalar flag bits retain a numeric
remainder; unidentified bits in logical masks use `BIT_n` across the full width.

Multiword masks are single logical fields: `shop_character_purchased_bits`
is 128 bits, `shop_hint_purchased_bits` is 96, each extra mask is 64, and
`hint_completion_bits` is 192. All accept full-width decimal or `0x` hexadecimal
values as well as named combinations; `NONE` clears the field. The schema shows
the bit-to-name mapping, independent of the underlying 32-bit word boundaries.
The original word-index assignments remain accepted as compatibility aliases,
but are not emitted by `schema` or `list`.

The character purchase names follow the original 90-entry shop list, including
vehicles. They are original configuration names, not numeric character IDs.
Extra names reuse `Cheat[]`; tutorial names identify the original hint callback
and text ID, prefixed with `console.` or `touch.`. The original shop-hint table
is empty, so its reserved storage remains numbered. Shared engine enums are in
`src/legoapi/core/save_values.h`; logical host mappings are in
`src/host/harness/save_names.hpp`. Recheck those mappings if the game assets change.

```sh
bazel run --config=native //src:run_native -- save edit \
  'character_save[12]=AVAILABLE|UNLOCKED' 'reward_flags=100_PERCENT|ALL_GOLD_BRICKS'
bazel run --config=native //src:run_native -- save edit \
  'extra_purchased_bits=scorex10|fastbuild' 'area_save[0].minikit_complete=COMPLETE'
bazel run --config=native //src:run_native -- save edit \
  'shop_character_purchased_bits=gonkdroid|wookie|slave1'
bazel run --config=native //src:run_native -- save edit \
  'hint_completion_bits=console.AutoJump_1568|touch.AutoJump_1568'
```

The minikit completion flag is at area offset 4, the actual count at offset 5,
and the red-brick flag at offset 6. These are independent stored properties;
editing one does not fabricate corresponding pickups or recompute all progress.
`save_version`, `field30_0x7c2c`, `initial_store_pack_flags`, and `field_0x7bf8`
remain accepted scalar input aliases for their recovered names.

See [save-format evidence](decomp/14-save-format-audit.md) for original symbols,
offsets, value tables, and unresolved fields. When save structs or enums change,
update the utility registry/descriptions and run its coverage/round-trip checks;
review the `save schema` and `save schema --options` output again. There is no
schema-generation script or copied generated table to maintain.

```sh
bazel run --config=native //src:run_native -- save list '/path/to/save' > /tmp/save.params
bazel run --config=native //src:run_native -- save edit '/path/to/save' \
  --output '/path/to/edited-save' --params /tmp/save.params coins=999
```

Parameter files contain one assignment per line, with optional blank lines and
whole-line `#` comments. Assignments apply in argument order, so later values
override earlier values. Listings can be imported directly. The original
`ChecksumSaveData` routine regenerates the payload checksum; `MakeSaveHash`
regenerates the game slot code with the save system's signed callback behavior.
Options saves use slot code `0xffffffff`. `--keep-derived` preserves explicitly
supplied or existing checksum/slot-code bytes, including corrupt diagnostic
fixtures. A checksum mismatch is reported by `list` and can be repaired by
`edit` without assignments. Header edits must preserve the supported layout.

Fresh game creation calls the reconstructed `NewGame` routine. Because the
utility does not configure game assets, area trial times, character IDs,
localized customizer names, and other asset-dependent defaults need a template
or explicit parameters. `--from` preserves the entire template, including
unknown bytes and any extra-data prefix. Fresh `--options` creation starts with
the game's zero-initialized `SUPEROPTIONS_s` storage. These are diagnostic save
builders; arbitrary combinations of valid field values need not describe
consistent gameplay progress.

Edits validate all requests before writing, write a temporary sibling, check
write/close errors, then publish atomically. `create` and `edit --output` refuse
an existing destination; ordinary `edit` replaces its input. Use `--output` to
retain the original. Unmodified bytes are preserved except automatically
derived checksum and slot code. Existing file permissions are retained for
in-place edits on POSIX.

The serializer uses `SaveLoad`, `GAMESAVE_s`, `SUPEROPTIONS_s`, and their nested
types directly; it does not introduce a second game save schema or replace
slot-based game filesystem behavior. The customizer's previously empty
`Customiser_CopyDefaultPiecesToSave` was recovered from original symbol
`_Z34Customiser_CopyDefaultPiecesToSaveP10CUSTOMISERP15CUSTOMISESAVE_s`:
it copies nine primary and nine secondary pieces and falls back to the
customizer's save pointer when the explicit destination is null. The recovered
routine matches the original 239-byte function at 100% in objdiff.

Run the integration checks after building native (pass the resolved binary
path if another Bazel configuration subsequently changes `bazel-bin`):

```sh
python3 scripts/checks/check_save_utility.py "$(realpath bazel-bin/src/saga_native)"
```

The checks cover field encoding, new files, options saves, checksum/slot-code
updates, lossless parameter export/import (including unknown bytes and unusual
float values), extra-data prefixes, malformed files, and failed-edit preservation.

## Save schema reference

Display the current schema directly from the utility:

```sh
bazel run --config=native //src:run_native -- save schema
bazel run --config=native //src:run_native -- save schema --options
```

## Direct gameplay smoke tests (Linux)

`saga_smoke` is a separate host executable linked against the native engine.
It loads a validated mobile game-save payload after permanent assets initialize,
selects the requested destination and runs the normal level-loading/game loop.
Test-only link wrappers use the existing synchronous permanent-data loader to
skip startup screens. They are absent from `saga_native`, WASM and Android.

```sh
bazel build --config=native //src:saga_smoke
bazel run --config=native //src:run_smoke -- --list
bazel run --config=native //src:run_smoke -- --area Negotiations --frames 300
bazel run --config=native //src:run_smoke -- --level Negotiations_b --save '/path/to/fixture'
bazel run --config=native //src:run_smoke -- --level Map --visible
```

Names are case-insensitive internal asset names; `Map` is the Cantina. `--list`
prints area/level pairs. `--area` selects its first gameplay level, excluding
intro/midtro/outro/status entries. Asset loading still takes time; “direct” means
there is no menu interaction or startup-screen wait.

The default fixture is the same slot-0 path used by the save utility above.
For reproducible tests, supply a fixed fixture with `--save`. The input is read
only and its envelope, payload size and checksum are validated. Autosave is
disabled and any incidental writes go to a unique `.work/smoke/<run>/` directory.
Rendering uses a hidden SDL window with dummy audio and no MSAA; an X display
and working GLES driver are still required. `--visible` shows the window.

A pass requires the requested world, a gameplay socket system, an active player
with a controller, finite player position/time, and 300 advancing simulation
frames by default (`--frames`). `--frames 0` disables the frame limit; watchdog
timeouts, including the overall deadline, remain active. It does not prove visual correctness, successful
movement, or level completion. A separate watchdog thread detects stopped frame
completion and stalled gameplay after readiness (`--stall-ms`, default 10000).
An overall deadline also catches loading loops that keep drawing frames but
never reach gameplay (`--timeout-ms`, default 90000). Do not set sanitizer
environment overrides that disable error halting: debug smoke builds default
to halting on UBSan errors as well as ASan errors.

Exit statuses: **0** pass, **1** failure, **2** invalid input/destination/save,
**124** watchdog timeout. Crashes retain their abnormal process exit status.
The watchdog terminates the process without waiting for potentially hung engine
workers. Diagnostics go to stderr with a `smoke:` prefix.

```sh
python3 scripts/checks/check_smoke_utility.py "$(realpath bazel-bin/src/saga_smoke)"
```

This checks malformed saves, missing/invalid arguments and destinations, both
watchdog deadlines, 120 frames in Negotiations, 300 frames in Anakin's Flight,
and unchanged fixture bytes.

The audio utility is retained: it checks the real game's sound pipeline through
OpenSL emulation and SDL, requiring a player, non-silent mixed PCM and at least
three seconds of audio consumed. This checks audio output that gameplay frame
progress alone cannot establish.

## Non-interactive visual captures

Use the window utility in hidden, muted mode when running alongside other desktop
applications:

```sh
bazel run --config=native //src:run_native -- window --offscreen --mute --capture
```

- `--offscreen` creates a hidden, non-focusable SDL window and ignores desktop
  input events. Rendering and framebuffer readback remain active.
- `--mute` selects SDL's dummy audio driver before audio initialization, so no
  sound reaches the real device.
- `--capture` writes changed frames to `.work/capture/`. While an image is
  changing, captures are rate-limited to roughly 500 ms; stable transitions
  are retained when their framebuffer hash changes.
- `--script-input` adds the host-only deterministic touch sequence used to
  reach the load/save flow. It may be combined with all three flags above.

On Linux (or an MSYS2 environment that provides GNU `timeout`), the standard
unattended menu check is therefore:

```sh
timeout 38s bazel run --config=native //src:run_native -- window --offscreen --mute --script-input --capture
```

On Windows the executable is `bazel-bin/src/saga_native.exe`. macOS users can
install GNU coreutils and invoke the equivalent `gtimeout`; native macOS is not
currently part of the supported CI matrix.

The command implementations belong under `src/host/harness/`. Actual platform
adapters belong under `src/host/platform/` and are limited to imported APIs,
filesystem/environment access, or a build-selected platform interface. Host
code must not replace portable game or engine functions.

Windows supplies the imported `lrand48` and `srand48` APIs in
`src/host/platform/windows/posix_random.cpp`. They share the 48-bit generator state
and do not change `rand()` state. The multiplier, increment and initial state
agree with Android Bionic's
[`rand48.h`](https://github.com/aosp-mirror/platform_bionic/blob/android-4.0.4_r2.1/libc/private/rand48.h).
This replaces an older Windows seed-only substitute after post-processing
recovery introduced the original `lrand48` calls. Six explicit seeds agree
with Linux libc for 10,000 draws each; the Bionic default agrees with libc
seeded with `0x1234abcd` for another 10,000 draws. An initial comparison to
unseeded Linux libc differed because its default state differs from Bionic.
The adapter compiles with MinGW; target/Linux native builds and repository
checks pass. Full Windows linkage still requires the CI graphics dependencies.
