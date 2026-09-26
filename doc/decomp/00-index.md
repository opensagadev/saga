# SAGA matching-decompilation knowledge base

> Agent/reference documentation. Human setup and development workflows are in
> [`README.md`](../../README.md) and [`CONTRIBUTING.md`](../../CONTRIBUTING.md).

This directory documents the Android x86 binary, its GCC 4.7 ABI and code
generation, and the workflow for comparing reconstructed code with the
original `res/libTTapp.so`.

## Build modes

| mode | purpose | command |
|---|---|---|
| target | matching Android x86 shared object | `bazel build --config=target //src:saga_target` |
| native | Linux diagnostic executable | `bazel build --config=native //src:saga_native` |
| native | Linux direct gameplay smoke test | `bazel build --config=native //src:saga_smoke` |
| native | Windows diagnostic executable | `bazel build --config=native --config=windows-mingw //src:saga_native` |
| wasm | browser diagnostic bundle | `bazel build --config=wasm //src:saga_wasm` |

The target is `bazel-bin/src/libTTapp.so` and deliberately has no `main`.
Native and WASM builds define `HOST_BUILD` and use the host harness.

## Documentation map

The [PR #107 integration audit](25-pr107-integration.md) supersedes historical
experiment notes that recommend inline assembly, register-passing attributes,
or function-level optimization tricks. Those shortcuts are not permitted by
the matching skill; the reconstructed behavior is retained in portable C/C++.

| file | use it for |
|---|---|
| [01-toolchain.md](01-toolchain.md) | NDK r8e compiler, flags, optimization map, and dependencies |
| [02-codegen.md](02-codegen.md) | GCC 4.7 instruction-shape patterns |
| [03-matching.md](03-matching.md) | current build and verification workflow |
| [04-types-abi.md](04-types-abi.md) | i686 ABI, types, mangling, layouts, and thunks |
| [05-source-conventions.md](05-source-conventions.md) | source placement, linkage, stubs, and globals |
| [06-target-binary.md](06-target-binary.md) | measured properties of the original ELF |
| [07-diagnostics.md](07-diagnostics.md) | symptom-to-cause mismatch diagnosis |
| [08-asm-review.md](08-asm-review.md) | raw symbol and disassembly review |
| [09-objdiff-cli.md](09-objdiff-cli.md) | compact per-symbol objdiff helper |
| [10-animation-regression-audit.md](10-animation-regression-audit.md) | historical animation-related regression audit |
| [11-animation-runtime-inventory.md](11-animation-runtime-inventory.md) | animation runtime coverage, active paths, and remaining gaps |
| [13-post-processing-audit.md](13-post-processing-audit.md) | directional-light intensity, retained Android post-effects, and runtime limitations |
| [14-save-format-audit.md](14-save-format-audit.md) | save layout, serialized enums, original-binary evidence, and unresolved fields |
| [15-translation-unit-reconstruction.md](15-translation-unit-reconstruction.md) | evidence and staged plan for original TU ownership and optimization |
| [16-game-object-addons.md](16-game-object-addons.md) | game object extension layouts and behavior |
| [17-matching-synthesis.md](17-matching-synthesis.md) | cross-file matching observations and next targets |
| [18-gameaiprocess-codegen-notes.md](18-gameaiprocess-codegen-notes.md) | measured GCC 4.7 control-flow and register-allocation experiments in `GameAIProcess` |
| [18-nucore-touch-control-flow.md](18-nucore-touch-control-flow.md) | touch input action priority and fixed-size removal loops |
| [19-apimenu-stub-codegen-notes.md](19-apimenu-stub-codegen-notes.md) | API menu callback reconstruction and match results |
| [19-nusound-stub-audit.md](19-nusound-stub-audit.md) | verified sound no-ops and OGG callback codegen findings |
| [20-gamemenuall-stub-notes.md](20-gamemenuall-stub-notes.md) | game menu callbacks and their measured match results |
| [20-gamestructure-store-stubs.md](20-gamestructure-store-stubs.md) | game structure and store callback control flow |
| [20-gizmo-stub-codegen-notes.md](20-gizmo-stub-codegen-notes.md) | gizmo action state and GCC matching patterns |
| [20-deathstar-touch-controller.md](20-deathstar-touch-controller.md) | Death Star touch controller ABI and callback matching |
| [20-episode1-sebulba-codegen.md](20-episode1-sebulba-codegen.md) | Sebulba action state and movement trace matching |
| [21-cutscene-stub-codegen.md](21-cutscene-stub-codegen.md) | cutscene instance reconstruction and remaining mismatches |
| [21-bonus-cavalry-controller.md](21-bonus-cavalry-controller.md) | cavalry controller touch behavior and code generation |
| [21-dogfight-helper-linkage.md](21-dogfight-helper-linkage.md) | Episode III helper linkage needed for exact calls |
| [21-editor-test-menu.md](21-editor-test-menu.md) | editor test menu translation-unit placement and callbacks |
| [21-mech-context.md](21-mech-context.md) | touch context-task ABI and state layout |
| [21-mech-core.md](21-mech-core.md) | main touch controller structures and constructor matches |
| [21-mech-jump-autopilot.md](21-mech-jump-autopilot.md) | jump autopilot reconstruction and target branch behavior |
| [21-mech-menu.md](21-mech-menu.md) | touch menu controller callbacks |
| [21-mech-podrace-controller.md](21-mech-podrace-controller.md) | podrace touch controller behavior and layout |
| [21-mech-speeder-codegen.md](21-mech-speeder-codegen.md) | speeder touch controller ABI and gesture handlers |
| [21-mech-ui.md](21-mech-ui.md) | touch UI callback no-op audit |
| [21-mech-virtual.md](21-mech-virtual.md) | virtual console controller layout and code generation |
| [21-nucore-misc-stub-audit.md](21-nucore-misc-stub-audit.md) | core rendering wrappers, frame timing, and camera matching |
| [21-render-core-stub-notes.md](21-render-core-stub-notes.md) | renderer stub audit and geometry matches |
| [21-vaderc-codegen.md](21-vaderc-codegen.md) | Vader level control flow and branch layout |
| [22-episode-i-action-codegen.md](22-episode-i-action-codegen.md) | Episode I action handler and mine-creation findings |
| [22-episodeIII-lift-codegen.md](22-episodeIII-lift-codegen.md) | Episode III dogfight and lift handler code generation |
| [22-mech-jump-codegen-notes.md](22-mech-jump-codegen-notes.md) | jump timing branches and NDK frame patterns |
| [22-numemorymanager-stub-notes.md](22-numemorymanager-stub-notes.md) | memory manager ABI and stranded-block control flow |
| [23-audio-stub-audit.md](23-audio-stub-audit.md) | duplicate Ogg symbol audit and exact audio no-ops |
| [23-createpod-codegen.md](23-createpod-codegen.md) | pod creation helper inlining and stack alignment |
| [23-gameobjects-stub-codegen.md](23-gameobjects-stub-codegen.md) | game object callbacks and pack button rendering |
| [23-numouse-stub-notes.md](23-numouse-stub-notes.md) | Android mouse constant-return ABI |
| [23-objectsall-codegen.md](23-objectsall-codegen.md) | boulder and breakable object comparison patterns |
| [23-world-hub-codegen-notes.md](23-world-hub-codegen-notes.md) | world hub bonus mode and arcade statistics |
| [episode-iv-vi-stub-codegen.md](episode-iv-vi-stub-codegen.md) | Episode IV–VI handler block-order findings |
| [episode-iv-followup-codegen.md](episode-iv-followup-codegen.md) | Episode IV level setup and exact-size handlers |
| [episode-vi-followup-codegen.md](episode-vi-followup-codegen.md) | Episode VI level handler branches and register choices |
| [mech-input-stub-pass.md](mech-input-stub-pass.md) | measured touch controller stub coverage |
| [mech-jump-landing-spot.md](mech-jump-landing-spot.md) | jump landing target ABI and scalar codegen |
| [utilities-stub-codegen.md](utilities-stub-codegen.md) | utility geometry, clipping, and 64-bit conversion patterns |
| [22-gcc47-control-flow.md](22-gcc47-control-flow.md) | GCC 4.7 branch prediction, trace layout, and dump workflow |
| [22-virtual-widgets.md](22-virtual-widgets.md) | virtual touch widget layout, behavior, and compiler patterns |
| [18-items-collect-codegen.md](18-items-collect-codegen.md) | item collection and minikit callback code generation |
| [19-nucore-occlusion.md](19-nucore-occlusion.md) | visibility occlusion reconstruction |
| [20-quaternion-euler.md](20-quaternion-euler.md) | quaternion to Euler wrapper and register choices |
| [21-camera-aabb-planes.md](21-camera-aabb-planes.md) | camera box intersection plane ordering |
| [21-mech-gestures.md](21-mech-gestures.md) | touch gesture dispatch and compiler patterns |
| [22-legacy-hgobj-blend.md](22-legacy-hgobj-blend.md) | legacy hierarchical object animation blending |
| [23-character-motion-codegen.md](23-character-motion-codegen.md) | character motion callbacks and AT-AT matching |
| [24-actions-combat-codegen.md](24-actions-combat-codegen.md) | combat action control flow and register allocation |
| [24-camera-minicut-command-codegen.md](24-camera-minicut-command-codegen.md) | camera mini-cut command reconstruction |
| [24-core-input-stub-codegen.md](24-core-input-stub-codegen.md) | gamepad and timing callback matching |
| [24-hub-bonus-menu-control-flow.md](24-hub-bonus-menu-control-flow.md) | bonus menu helper block order |
| [agent-snake-specialmoves.md](agent-snake-specialmoves.md) | snake special move control flow |
| [agent-tag-newtransfer-codegen.md](agent-tag-newtransfer-codegen.md) | tag transfer layout and register use |
| [agent-tagging-codegen.md](agent-tagging-codegen.md) | character tagging and Batman icon matching |
| [ai-creature-stubs.md](ai-creature-stubs.md) | creature AI callback reconstruction |
| [legoapi-misc-platform-stub-codegen.md](legoapi-misc-platform-stub-codegen.md) | platform buffer, stream, and status callbacks |
| [tmclient-ndk-r8e-codegen.md](tmclient-ndk-r8e-codegen.md) | TMClient translation unit and compiler matching |

## Non-negotiable matching facts

1. Mangled names must match exactly. `int` and `long` are both 32-bit here but
   have different Itanium ABI encodings.
2. C versus C++ linkage is part of the symbol contract.
3. Source-file optimization is part of the code-generation contract. Read
   `bazel/android_per_file_copts.bazelrc`; no entry means `-O0`.
4. Function and string-literal order can affect emitted bytes because target
   builds disable function/data sections.
5. Matching code uses the limited NDK system C++ runtime, not the modern STL.
6. `HOST_BUILD` behavior is diagnostic only and must not alter the target.
7. The target remains a shared object named `libTTapp.so`; desktop executables
   are host-only (`saga_native` and the Linux `saga_smoke` test runner).

## Workflow in one screen

```bash
bazel build --config=target //src:saga_target
bazel test //scripts/checks:checks
bazel run //scripts:objdiff_cli -- _Z5qrandv
bazel run //scripts/checks:check_symbols
```

`objdiff-cli.py` requires `objdiff-cli` on `PATH`. `checks/check_symbols.py` requires
the original binary. These direct `python3` commands use the system interpreter.
The Bazel test suite uses a downloaded, pinned Python 3.12.12 runtime, does not
use system site-packages, and does not need a venv.

## Key repository paths

| path | role |
|---|---|
| `.bazelrc` | target/native/WASM configuration entry points |
| `src/BUILD.bazel` | source membership, outputs, defines, and platform flags |
| `bazel/android_per_file_copts.bazelrc` | exact target optimization overrides |
| `bazel/android_cc_toolchain_config.bzl` | NDK r8e C++ toolchain definition |
| `scripts/BUILD.bazel` | hermetic Python checks and tools |
| `src/decomp.h` | native-only logging and unfinished-code diagnostics |
| `src/nu2api/nucore/fixed_width.h` | ABI-width types |
| `res/libTTapp.so` | original reference binary |

## Per-function authoring checklist

1. Confirm the exact symbol spelling and linkage in the original binary.
2. Locate the current source owner with `rg`.
3. Confirm the source file's optimization options with `bazel aquery`.
4. Preserve the ABI and use the GCC 4.7 patterns documented here.
5. Rebuild the target and compare the symbol directly.
6. Run `bazel test //scripts/checks:checks` before committing.
