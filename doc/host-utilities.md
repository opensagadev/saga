# Host utilities

> Agent/reference document. Human host workflows are summarized in
> [`CONTRIBUTING.md`](../CONTRIBUTING.md).

The host executable is a diagnostic aid for the decompilation. A successful
host run does not justify behavior that is absent from the original binary;
game-side fixes must still come from the original code, data, and ABI.

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

The default fixture is
`res/SavedGames/SaveGame0.LEGO Star Wars - The Complete Saga_SavedGame`.
For reproducible tests, supply a fixed fixture with `--save`. The input is read
only and its envelope, payload size and checksum are validated. Autosave is
disabled and any incidental writes go to a unique `.work/smoke/<run>/` directory.
Rendering uses a hidden SDL window with dummy audio and no MSAA; an X display
and working GLES driver are still required. `--visible` shows the window.

A pass requires the requested world, a gameplay socket system, an active player
with a controller, finite player position/time, and 300 advancing simulation
frames by default (`--frames`). `--frames 0` disables the frame limit; watchdog
timeouts, including the overall deadline, remain active. It does not prove visual
correctness, successful movement, or level completion. A separate watchdog thread
detects stopped frame completion and stalled gameplay after readiness (`--stall-ms`,
default 10000). An overall deadline also catches loading loops that keep drawing
frames but never reach gameplay (`--timeout-ms`, default 90000). Do not set sanitizer
environment overrides that disable error halting: debug smoke builds default to
halting on UBSan errors as well as ASan errors.

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

## Non-interactive visual captures

Use the window utility in hidden, muted mode when running alongside other desktop
applications:

```sh
bazel run --config=native //src:run_native -- window --offscreen --mute --capture
```

- `--offscreen` creates a hidden, non-focusable window and ignores desktop
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
