# Original translation-unit reconstruction

At the latest measured target build, there are 514 current translation
units and a 45.8886% whole-binary fuzzy match. This is a source-ownership
overview; the matching percentage does not measure how many original file
boundaries are known. “Reconstructed” below means the source boundary has
evidence and passed a matching comparison; it does not mean every body is
exact.

The original `res/libTTapp.so` has no DWARF or `STT_FILE` entries. It does
have 325 static initializers (320 distinct basenames), 21 embedded source
paths, and local text/data symbols. Normal function addresses often form
sequential file runs, but address adjacency and constructor-local blocks
alone do not prove a TU. `matching.json` units describe the *current*
build, never independent original ownership.

For the complete text/data symbol surface, generate the machine ledger:

```sh
PYTHONPATH=. python3 scripts/restructure/generate_original_tu_map.py
PYTHONPATH=. python3 -m unittest scripts.restructure.test_original_tu_map scripts.restructure.test_calibrate_tu_map scripts.restructure.test_text_adjacency scripts.restructure.test_original_local_xrefs
PYTHONPATH=. python3 scripts/restructure/generate_original_tu_map.py \
  --current bazel-out/k8-fastbuild/bin/src/libTTapp.so --units matching.json \
  --with-local-xrefs --output .work/original-tu-map-current.json
```

The ledgers are ignored. They preserve address, section, size, binding,
aliases, and optionally candidate current owners.
It inventories 32,596 allocated original symbols, including 14,541 text,
9,384 read-only, 6,625 BSS, and 1,796 data entries; aliases remain distinct.
The original/current target have 14,561/11,559 dynamic relocations, 190/186
PLT slots, and `0x4a58`/`0x3c70` bytes of `.got`; these are linker/layout
diagnostics, not TU ownership proof by themselves.
Twelve symbol-free include/comment shells were removed; the include-only
`legoai.cpp` remains because it owns an original static initializer. An
artificial local `RotDiff` stub was removed, exposing the real owner in
`socksysall.cpp` and gaining one exact match.
The ledger also records 13,042 individual `.text` adjacency links as weak
evidence, not TU assignments. Against current-build `STT_FILE` labels, only
3,189 links were assessable and 89.84% joined the same file; address order
alone is insufficient to reconstruct the original boundaries.
The optional Capstone-based pass verifies 998 original function-to-LOCAL-object
instruction references (989 writable state, nine read-only constants). Of
496 links with unique current source candidates on both ends, 30 cross
current-file boundaries, of which 29 have a size-concordant current object
candidate. These are diagnostic, not automatic moves. The
largest cluster links editor UI callbacks to local editor state. This pass
requires Capstone; the default ledger does not.
The writable-state links form 346 minimum same-TU components covering 1,201
symbols, not complete files. Thirty-two components span the initializer-order
blocks, mostly because a constructor references `VuVec` locals that appear
*after* it in the symbol table. This directly demonstrates why those blocks
cannot be treated as original file boundaries.
Use embedded paths, corroborated local-static references, and contiguous
ordinary text runs in that order; treat names, initializer order, and
optimization shape as supporting clues. A `.c` basename does not prove C
linkage.

## TU overview

| Original cluster / strongest anchor | Current source owner(s) | State and remaining structure |
|---|---|---|
| Android scene: embedded `nugscn_android.c` paths and scene run `0x2fd760–0x2fefbd` | `nu2api/nu3d/android/nugscn_android.cpp` and its header | Shared, offset-checked native scene type; teardown reconstructed. Creation/fixup and related graphics-data boundaries still need body/owner review. Original `NuGScnDestroyPS` has C++ linkage. |
| Android display-list and primitive runs: local helpers/tables plus `0x29aaa9+` and `0x29cbc9–0x29d3d1` text | `nu2api/nu3d/android/nudlist_android.c`, `nuprim_android.c`, renderer neighbors | Principal source split is reconstructed; executor, callback, and primitive bodies still differ. These `.c` files require their measured C++ compilation mode. |
| Android material/render state: local callback/data clusters | `nu2api/nu3d/android/numtl_android.cpp`, `nurndr_android.c` and renderer neighbors | Multiple evidenced slices moved; remaining render-state and callback codegen must be measured per unit. |
| Android portal visibility: contiguous `nuportal_android.c` helpers at `0x2fc209–0x2fc7fe` | `nu2api/nu3d/android/nuportal_android.cpp` with real platform header; shared portal engine remains in `nuportal.cpp` | Five helpers now co-located at the existing default optimization, preserving all three exact matches and whole score. Forced `-O2`/`-O3` trials regressed and were rejected. `PortalVisiFlags` remains original GLOBAL data. |
| Android scratch, graphics clear, rain, FMV, and time: embedded paths/initializer runs | `nuscratch_android.c`, `ios_graphics.cpp`, `nurain_android.c`, `nufmv_android.cpp`, `nutime_android.c` | Source boundaries accepted; individual scratch/clear/media bodies remain incomplete. |
| DDS and texture-animation owners: DDS initializer/local vectors and texture-animation run | `nu2api/nu3d/NuDDSFunctions.cpp`, `nu2api/nu3d/nutexanim.cpp` | File/data ownership largely reconstructed; DDS mip/description and some animation bodies remain. |
| NuQT quadtree: contiguous `0x261783–0x2626f4` run and verified node layout | `nu2api/nucore/nuqt.cpp` and `nuqt.h` | Core owner reconstructed; insertion is still a body-level gap. |
| NuMemory pool/manager: pool run, method ABI and vtable/local data | `nu2api/nucore/NuMemoryPool.cpp`, `NuMemoryManager.cpp` | Strong boundaries and many exact helpers; page lifecycle and large-bin methods remain. |
| Animation action/context tables: consecutive `LEGOACT_*` and `LEGOCONTEXT_*` data | `legoapi/characters/motion/animation.cpp`, `contexts.cpp` | Tables and owner split reconstructed; some initialization/body ownership remains provisional. |
| `game_deb.cpp` action/context lookup data: adjacent `ExtraActionData`, exported pointers, and paired local tables before its initializer | `legoapi/render/fx/game_deb.cpp` and canonical `game_deb.h`; previously `globals.cpp` | Complete data cluster moved together. Target object retains original names, binding and sizes (`0x108`, `0x750`, `0x650`); function scores are unchanged. |
| `saveload.c`: contiguous `0x308045–0x308bbc` text run and four function-local 4 KiB buffers | `gameframework/saveload.cpp` and `saveload.h`; formerly split across `characters.cpp` and a catch-all config file | All 28 reported functions now share the evidenced owner, with original LOCAL buffer names, sizes and order. `fullcodename` is exact; the remaining unfinished card APIs retain explicit stubs without matching regression. |
| `episodeVI.cpp`: consecutive `0x228db0–0x22a2c0` level/bonus tail, with `NewTown_Reset` referencing LOCAL `prevOnTaunTaun` | `legoapi/world/levels/episodeVI.cpp`; formerly also `lego_city.cpp`, `senate.cpp`, `new_town.cpp` | Ten handlers and four local state bytes co-located; three unjustified tiny files removed. Whole fuzzy rose 0.0028 points, `NewTown_Init` 13.61→99.61, no exact matches lost; the later `Platform_*`/bonus tail moved without score changes. `LegoCity_Reset` slipped 94.17→93.74 on local EBX displacements while its state order/spacing remain original; GOT placement is still open. |
| Streaks: `0x4a3f80–0x4a4ca2` text run and local `streakhdrs_used` reference from `DrawStreaks` | `legoapi/actions/character/streaks.cpp` and `streaks.h`; formerly `DrawStreaks` was in `render.cpp` | The five exported streak functions now share the evidenced owner and a real API header. Whole and per-function matching are unchanged; unfinished bodies remain stubs. |
| Android shader program: contiguous `0x2a5390–0x2a5cde` helpers/program run and local `programPool` | `nu2api/nu3d/android/nushaderprogram_android.cpp`; shared declarations in `nushader_internal.h` | Full owning group moved from `nushader.cpp`, with the `0x810` pool and allocator still local. The real mapping helper now owns its original function-local `uniformName`; no neighbors regressed. |
| Shader object and base: embedded `shaderbuilder/android/nushaderobject.cpp` path, object run `0x309da0–0x30d5b2`, separate `_GLOBAL__sub_I_nushaderobjectbase.cpp` and base run `0x30e3b0–0x30e7a1` | `nu2api/nu3d/android/nushaderobject.cpp`, `nu2api/nu3d/nushaderobjectbase.cpp`; previously split across `nushader.cpp` and catch-all files | Complete object/base families split with real local shader data and original global `GetUsageMask`; its placeholder was removed. `-O3` and internal header declarations retained. Matching improved; remaining bodies are a separate task. |
| Nu3D dynamic-light pools: three adjacent locals, their direct constructor/create/destroy xrefs | `nu2api/nu3d/nu3d_includes.cpp`; previously `nucore.cpp` | All three pools and four direct owner methods moved together. Original local bindings and sizes (`0x90`, `0x210`, `0x3f10`) survive; all other class methods remain provisional. |
| ANI3/NuAnim: apparent `0x2bd180–0x2c8253` run, local `KeyStructSizes`/`CurveGroupMasks`, and evaluator `scale_array` | `nu2api/nucore/nuanim3.cpp` (`-O2`) for the 11-player/extractor dependency group; `nucore_plain.cpp` (`-O3`) for evaluator/accumulator and six-function curve tail; `nuanim.cpp` unchanged | The two local tables and their direct users now share a source owner. Isolated trial gained 0.0001 fuzzy points with no regressed functions; the complete original ANI3 boundary remains **open**. Whole-family co-location trials regressed and were rejected. |
| API-object character/animation run: address order plus static state/callback references | `legoapi/items/base/apiobject.cpp` and character/action neighbors | Partially reconstructed with real shared declarations; animation-packet/helper ownership and low bodies remain. |
| Character scene/icon/variant family: adjacent text, local `CharScene_Area`, `IconScene`, `IconPath`, `CharVariant`, and `CHARVARIANTCOUNT` | `characters.cpp`, `gamemenuall.cpp`, `charconfig.cpp`; shared API in `character.h` | **Open.** `IconPath` and the exported 23-entry variant pointer table now have the original data type/content, and variant methods have real near-exact bodies. Partial co-location trials regressed neighbors and were rejected; resolve the full owner boundary before retrying. |
| Mini-cut camera state: `GameCam` points to local `_ZL10GameCamera` in the `_GLOBAL__sub_I_gizminicut.cpp` block | `legoapi/gizmos/trigger/gizminicut.cpp`; previously `globals.cpp` | State and exported pointer co-located with the original owner. The target keeps the original local name and `0x230` size; matching is unchanged. Adjacent camera functions remain separate pending xrefs. |
| Gizmo registration, blowups, and pickups: initializer blocks, callback arrays and adjacent runs `0x4b7540–0x4c28a0` | Singular owners under `legoapi/gizmo` and `legoapi/gizmos` | Major source/data placement accepted; collision, registration, and pickup bodies remain. Do not merge unrelated helpers by name alone. |
| Zipup/rope: zipup run `0x1d1440–0x1d4910` and local start-point helper | `legoapi/gizmos/door/zipups.cpp`, `props/objects/zipup.cpp`, rope owner | Zipup stages are partly reconstructed; rope draw and some state remain open. |
| Panel: `0x140950–0x1449c0` run; `Panel_Clear` and `DrawPanel` access the same local `redbrickslidetime` | `legoapi/menus/core/panel.cpp` and `legoapi/render/core/render.cpp` | `DrawTimer` and its state moved. **Open:** full `-O2` co-location is structurally supported but costs 0.0049 overall points via `DrawPanel`; `-O3` helps the initializer but drops `PanelRender` by 10 points. Both trials were rejected; the accessor remains temporary. |
| Post-filter: `nupostfilter.cpp` local block, including `texturePool` at `0x11cd820` | `nu2api/nu3d/android/nupostfilter.cpp`; framebuffer pair in `nuposteffect_plain.cpp` | Measured split and genuine Init/Destroy flows improved matching. Both files require `-O2`. The 0x810-byte local pool has no verified text xref or field layout, so it is not fabricated in source. |
| NuSound buffer: embedded `nusound_buffer.cpp :53` path and adjacent method run | `nu2api/nusound/nusound_buffer.cpp` | Owner and `-O3` are supported; allocation control flow improved, with other method bodies still open. |
| `numaths.c`: text `[0x290b20, 0x292e9c)` including its trailing static initializer and constructor | Split across `nu2api/numath` files and nearby catch-alls | **Open.** The preceding `0x290b01` `numath_includes.c` constructor closes a different unit; a second local `VuVecSet` starts the next unit at `0x292e9c`. Consolidate only this measured run, preserving its constructed vector constants and default-mode codegen. |
| Terrain data/collision: `gameliball.cpp` initializer block owns local `TerI`, `SphereData`, `PlatCallback`, and impact data | `legoapi/render/core/terrain*.cpp`, `gameliball.cpp`, terrain functions in `hits.cpp`, `transform.cpp`, `surfaces.cpp`, `episode.cpp` | **Open.** `TerI` users span six current files, including verified original terrain-run functions; a state-only move would need a false cross-TU bridge. The text run `0x36de60–0x38f460` also interleaves unrelated animation. Plan the complete dependency cut before migrating. |
| Editor tools: local camera/UI/cursor/menu state under `_GLOBAL__sub_I_edtoolsall.cpp`, text span `0x330900–0x3a9c88`; separate RTL starts next | `gameapi/edtools/edtoolsall_plain.cpp`, `edtoolsall.cpp`, `edui.c`, editor callbacks | **Open.** The `-O2` plain owner already holds much real state and many exact helpers. Audit the UI/camera/file tail and local callbacks before co-location; the `edui.c` `__used__` stubs are not evidence for optimization or a real boundary. |
| Menu/customiser, timing, and large catch-all files | `customise.cpp`, `timing.cpp`, `nucore_plain.cpp`, `edtoolsall*.cpp`, `aisys.cpp`, `gameobjects.cpp` | **Open.** Split only complete, evidenced dependent groups. Do not sacrifice exact neighbors to reproduce a basename or a speculative optimization level. |

## Remaining reconstruction plan

1. Extend the complete symbol ledger into a confidence-ranked original TU
   map: text runs, local data/rodata/BSS xrefs, embedded paths, constructor
   order, and GOT/relocation references. Record ambiguity rather than assign
   every symbol from address adjacency alone.
2. Calibrate each supported owner's compiler mode and optimization against
   exact neighbors (the portal helpers require the default mode; forced
   `-O2`/`-O3` regressed), then move whole dependency groups with real
   public/internal headers.
3. Review each moved unit for symbol binding/ABI, data and relocation effects,
   header ownership, and a sensible source boundary. Clean up catch-all and
   obsolete files; avoid arbitrary one-function or functionless files.
4. After each move, compare whole and per-function scores, exact transitions,
   symbol coverage, target/native/WASM builds, and Cantina smoke. Commit and
   push tested batches around 2,000 changed lines.
5. Prioritize the unresolved terrain LOCAL-state cut, editor callbacks, Panel
   co-location regression, and catch-all/resource owners. Do not invent
   unused pools or merge a constructor-delimited block without direct xrefs.

For each sensible move, compare function and whole scores, exact-match
transitions, symbol coverage, and neighboring bodies. Run target, native,
WASM, repository checks, and the 120-frame Map/Cantina smoke at appropriate
intervals. The original tiny-movement `MovePlayer` sanitizer flake is
documented by its prior history and is intentionally preserved for
matching. No assembly, calling-convention or visibility attributes for
score, fake initializers, dummy symbols, or source-local linker-only
`extern` bridges.
