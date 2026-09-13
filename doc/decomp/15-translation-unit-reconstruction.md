# Original translation-unit reconstruction

At the latest measured target build, there are 527 current translation
units and a 45.8762% whole-binary fuzzy match. This is a
source-ownership overview, not a claim that 45.8762% of the original file
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
PYTHONPATH=. python3 -m unittest scripts.restructure.test_original_tu_map scripts.restructure.test_calibrate_tu_map
```

The ledger writes ignored `.work/original-tu-map.json`. It preserves
address, section, size, binding, aliases, and candidate current owners.
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
| Android scratch, graphics clear, rain, FMV, and time: embedded paths/initializer runs | `nuscratch_android.c`, `ios_graphics.cpp`, `nurain_android.c`, `nufmv_android.cpp`, `nutime_android.c` | Source boundaries accepted; individual scratch/clear/media bodies remain incomplete. |
| DDS and texture-animation owners: DDS initializer/local vectors and texture-animation run | `nu2api/nu3d/NuDDSFunctions.cpp`, `nu2api/nu3d/nutexanim.cpp` | File/data ownership largely reconstructed; DDS mip/description and some animation bodies remain. |
| NuQT quadtree: contiguous `0x261783–0x2626f4` run and verified node layout | `nu2api/nucore/nuqt.cpp` and `nuqt.h` | Core owner reconstructed; insertion is still a body-level gap. |
| NuMemory pool/manager: pool run, method ABI and vtable/local data | `nu2api/nucore/NuMemoryPool.cpp`, `NuMemoryManager.cpp` | Strong boundaries and many exact helpers; page lifecycle and large-bin methods remain. |
| Animation action/context tables: consecutive `LEGOACT_*` and `LEGOCONTEXT_*` data | `legoapi/characters/motion/animation.cpp`, `contexts.cpp` | Tables and owner split reconstructed; some initialization/body ownership remains provisional. |
| `game_deb.cpp` action/context lookup data: adjacent `ExtraActionData`, exported pointers, and paired local tables before its initializer | `legoapi/render/fx/game_deb.cpp` and canonical `game_deb.h`; previously `globals.cpp` | Complete data cluster moved together. Target object retains original names, binding and sizes (`0x108`, `0x750`, `0x650`); function scores are unchanged. |
| Android shader program: contiguous `0x2a5390–0x2a5cde` helpers/program run and local `programPool` | `nu2api/nu3d/android/nushaderprogram_android.cpp`; shared declarations in `nushader_internal.h` | Full owning group moved from `nushader.cpp`, with the `0x810` pool and allocator still local. The real mapping helper now owns its original function-local `uniformName`; no neighbors regressed. |
| Shader object and base: embedded `shaderbuilder/android/nushaderobject.cpp` path, object run `0x309da0–0x30d5b2`, separate `_GLOBAL__sub_I_nushaderobjectbase.cpp` and base run `0x30e3b0–0x30e7a1` | `nu2api/nu3d/android/nushaderobject.cpp`, `nu2api/nu3d/nushaderobjectbase.cpp`; previously split across `nushader.cpp` and catch-all files | Complete object/base families split with real local shader data and original global `GetUsageMask`; its placeholder was removed. `-O3` and internal header declarations retained. Matching improved; remaining bodies are a separate task. |
| Nu3D dynamic-light pools: three adjacent locals, their direct constructor/create/destroy xrefs | `nu2api/nu3d/nu3d_includes.cpp`; previously `nucore.cpp` | All three pools and four direct owner methods moved together. Original local bindings and sizes (`0x90`, `0x210`, `0x3f10`) survive; all other class methods remain provisional. |
| ANI3/NuAnim: contiguous `0x2bd180–0x2c8253` run and one local `KeyStructSizes` | `gameanim.cpp`, `nucore_plain.cpp`, `nuanim.cpp` | **Open.** The player/evaluator/extractor family must migrate together; a partial move would duplicate original file-local data. Current `-O2`/`-O3` split needs whole-family measurement. |
| API-object character/animation run: address order plus static state/callback references | `legoapi/items/base/apiobject.cpp` and character/action neighbors | Partially reconstructed with real shared declarations; animation-packet/helper ownership and low bodies remain. |
| Mini-cut camera state: `GameCam` points to local `_ZL10GameCamera` in the `_GLOBAL__sub_I_gizminicut.cpp` block | `legoapi/gizmos/trigger/gizminicut.cpp`; previously `globals.cpp` | State and exported pointer co-located with the original owner. The target keeps the original local name and `0x230` size; matching is unchanged. Adjacent camera functions remain separate pending xrefs. |
| Gizmo registration, blowups, and pickups: initializer blocks, callback arrays and adjacent runs `0x4b7540–0x4c28a0` | Singular owners under `legoapi/gizmo` and `legoapi/gizmos` | Major source/data placement accepted; collision, registration, and pickup bodies remain. Do not merge unrelated helpers by name alone. |
| Zipup/rope: zipup run `0x1d1440–0x1d4910` and local start-point helper | `legoapi/gizmos/door/zipups.cpp`, `props/objects/zipup.cpp`, rope owner | Zipup stages are partly reconstructed; rope draw and some state remain open. |
| Panel: `0x140950–0x1449c0` run; `Panel_Clear` and `DrawPanel` access the same local `redbrickslidetime` | `legoapi/menus/core/panel.cpp` and `legoapi/render/core/render.cpp` | `DrawTimer` and its state moved into Panel. **Open:** a complete buildable co-location trial reduced `DrawPanel` by 1.80 points, so it was rejected. Resolve optimization/dependency boundaries before another move; the accessor remains temporary. |
| Post-filter: `nupostfilter.cpp` local block, including `texturePool` at `0x11cd820` | `nu2api/nu3d/android/nupostfilter.cpp`; framebuffer pair in `nuposteffect_plain.cpp` | Measured split and genuine Init/Destroy flows improved matching. Both files require `-O2`. The 0x810-byte local pool has no verified text xref or field layout, so it is not fabricated in source. |
| NuSound buffer: embedded `nusound_buffer.cpp :53` path and adjacent method run | `nu2api/nusound/nusound_buffer.cpp` | Owner and `-O3` are supported; allocation control flow improved, with other method bodies still open. |
| Terrain data/collision: `gameliball.cpp` initializer block owns local `TerI`, `SphereData`, `PlatCallback`, and impact data | `legoapi/render/core/terrain*.cpp`, `gameliball.cpp`, terrain functions in `hits.cpp`, `transform.cpp`, `surfaces.cpp`, `episode.cpp` | **Open.** `TerI` users span six current files, including verified original terrain-run functions; a state-only move would need a false cross-TU bridge. The text run `0x36de60–0x38f460` also interleaves unrelated animation. Plan the complete dependency cut before migrating. |
| Menu/customiser, timing, and large catch-all files | `customise.cpp`, `timing.cpp`, `nucore_plain.cpp`, `edtoolsall*.cpp`, `aisys.cpp`, `gameobjects.cpp` | **Open.** Split only complete, evidenced dependent groups. Do not sacrifice exact neighbors to reproduce a basename or a speculative optimization level. |

## Next structural work

1. Explain the Panel co-location regression before retrying its boundary;
   keep the measured trial separate from accepted source.
2. Consolidate the full ANI3/NuAnim family, including its sole original
   local key-size table; test optimization across substantial bodies.
3. Continue mapping live text/data owners in catch-all files using the
   generated ledger. Leave the unused post-filter pool unresolved until
   its layout or runtime use is evidenced.

For each sensible move, compare function and whole scores, exact-match
transitions, symbol coverage, and neighboring bodies. Run target, native,
WASM, repository checks, and the 120-frame Map/Cantina smoke at appropriate
intervals. The original tiny-movement `MovePlayer` sanitizer flake is
documented by its prior history and is intentionally preserved for
matching. No assembly, calling-convention or visibility attributes for
score, fake initializers, dummy symbols, or source-local linker-only
`extern` bridges.
