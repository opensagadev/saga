# Original translation-unit reconstruction

At the latest measured target build, there are 490 current translation
units and a 46.0849% whole-binary fuzzy match. The independent structural
dashboard reports 48.2% largest-run text grouping and 99.8% of assessable
same-TU writable-state links satisfied (only 42.6% of original links are
assessable); neither is a completion percentage.
The strict linked-GOT diagnostic currently covers 70.7% of original slots
and agrees on 17.4% of shared-target order; this is a layout warning, not
evidence for a specific TU boundary.
This is a source-ownership overview; the matching percentage does not measure
how many original file boundaries are known. “Reconstructed” below means the source boundary has
evidence and passed a matching comparison; it does not mean every body is
exact.

The original `res/libTTapp.so` has no DWARF or `STT_FILE` entries. It does
have 325 static initializers (320 distinct basenames), 21 embedded source
paths, and local text/data symbols. Normal function addresses often form
sequential file runs, but address adjacency and constructor-local blocks
alone do not prove a TU. `matching.json` units describe the *current*
build, never independent original ownership.
On the current build with file labels hidden, adjacent local-text pairs identify
the same TU with 90.1% precision, and initializer-delimited blocks have a
90.0% majority-owner fraction. These are useful survey heuristics, not safe
boundaries without corroborating local references and linked matching.
All 490 current units emit at least one named symbol. Six have no matched
function but still own real data or a tentative ELF `COMMON` definition;
they are not automatically empty TU shells.

For the complete text/data symbol surface, generate the machine ledger:

```sh
PYTHONPATH=. python3 scripts/restructure/generate_original_tu_map.py
PYTHONPATH=. python3 -m unittest scripts.restructure.test_original_tu_map scripts.restructure.test_calibrate_tu_map scripts.restructure.test_text_adjacency scripts.restructure.test_original_local_xrefs scripts.restructure.test_original_relocations
PYTHONPATH=. python3 scripts/restructure/generate_original_tu_map.py \
  --current bazel-out/k8-fastbuild/bin/src/libTTapp.so --units matching.json \
  --with-local-xrefs --with-relocations \
  --output .work/original-tu-map-current.json
```

The ledgers are ignored. They preserve address, section, size, binding,
aliases, and optionally candidate current owners. For a compact whole-project
structural dashboard, build the fully linked target first; report generation
reads that ELF and does not rebuild it:

```sh
bazel build --config=target //src:saga_target
bazel run //scripts:generate_bazel_objdiff_report
PYTHONPATH=. python3 scripts/restructure/compare_symbol_placement.py \
  --current bazel-out/k8-fastbuild/bin/src/libTTapp.so --units matching.json
PYTHONPATH=. python3 scripts/restructure/compare_symbol_placement.py \
  --current bazel-out/k8-fastbuild/bin/src/libTTapp.so --units matching.json \
  --component 588 --limit 10
```

It reports same-name source coverage by original section, clustering of
uniquely attributed functions in original address order, verifiable
function-to-local-state TU constraints and their largest splits, and the
separate exact/fuzzy body-matching baseline. These independent measures are
not combined into a false overall completion percentage; current-source
candidate names are not proven original TU boundaries. A local-xref ledger is
reused only when its ELF paths, unit manifest, and timestamps match; otherwise
it is recomputed. Current object ownership includes tentative ELF `COMMON`
definitions; same-TU scoring excludes duplicate original
name/type/binding-class identities and requires size-concordant state
candidates. `--component` reports one minimum same-TU local-reference group,
resolving ELF symbol IDs rather than JSON list positions; it does not claim
that the group is a complete original file. Its split ranking also excludes
duplicate original identities, such as the two different `frameStartTime`
locals, rather than assigning a same-name current object. To compare a proposed TU slice,
run:

```sh
PYTHONPATH=. python3 scripts/restructure/compare_symbol_placement.py \
  --source src/nu2api/nu3d/android/nurndr_android.c \
  --start 0x293168 --end 0x295f80 --section .text
```

The selected-source output is a compact ordered-symbol alignment score for the
TU and overall same-TU consistency from original
function-to-local-state references. The score is
`2 × longest-common-subsequence / (original symbols + current symbols)`: it
falls for missing, extra, or reordered names. Same-TU consistency reports both
the satisfied fraction and the fraction of original state links that can be
assessed; it is not a claim of total reconstruction progress. The script reuses
a fresh ignored ledger or computes local-reference evidence itself; `--no-xrefs`
skips that pass. Use `--diff` to print the original
linked-ELF symbols by address, the selected current `.o` symbols by section
offset, and their unified name-order diff. `--ranks` and
`--ranks --show-addresses` expose finer placement diagnostics. `--global-order`
adds a conservative whole-linked `.text`/`.data`/`.bss` order proxy, excluding
duplicate local names. These order
metrics do not measure matching bodies, prove TU boundaries, or cover GOT slots
and relocations. For a separate, conservative linked `.got` order diagnostic:

```sh
PYTHONPATH=. python3 scripts/restructure/compare_got_order.py \
  --current bazel-out/k8-fastbuild/bin/src/libTTapp.so
```

It pairs only uniquely named, exact, allocated `R_386_RELATIVE` targets with
matching section, type, binding, and size. Its two independent measures are
shared-target coverage against all original `.got` slots and longest-common-
subsequence order among shared targets. The current baseline is 3,365/4,758
slots comparable (70.7%) and 584/3,365 comparable targets in order (17.4%).
Imports, aliases, duplicate targets, and `.got.plt` are excluded. Missing
targets can reflect unfinished bodies; order differences can reflect several
linker and source-layout causes. A same-named local in two different builds
is still a comparable label, not a proven common source identity. Neither
measure proves a TU boundary, and
this is not a byte-for-byte GOT-slot or dynamic-relocation placement diff.
It inventories 32,596 allocated original symbols, including 14,541 text,
9,384 read-only, 6,625 BSS, and 1,796 data entries; aliases remain distinct.
The original/current target have 14,561/11,559 dynamic relocations, 190/186
PLT slots, and `0x4a58`/`0x3c70` bytes of `.got`; these are linker/layout
diagnostics, not TU ownership proof by themselves.
The opt-in relocation pass records all 14,751 original dynamic relocation
sites, including 4,758 in `.got`, 7,013 in `.data`, and 2,463 in
`.data.rel.ro*`. It identifies a containing named object at 9,568 sites
and an exact named static target at 11,114 sites. Its section offsets and
pointer targets help test data grouping and GOT order; a relocation site
or pointer target alone cannot establish the emitting TU.
All 4,758 original `.got` slots have relocation entries, while the current
target has 3,868 slots, a gap of 890. Missing code/data references from
unfinished bodies can cause this as well as TU layout; do not attribute the
gap to file placement alone.
Twelve symbol-free include/comment shells were removed; the include-only
`legoai.cpp` remains because it owns an original static initializer. An
artificial local `RotDiff` stub was removed, exposing the real owner in
`socksysall.cpp` and gaining one exact match.
Three later one-function files were also retired: `FtpFile::Accept()` joined
its FTP methods, `NuVecCheckForSNANs` joined its terrain caller, and
`RandomIDFromFlags` joined the character-variant run. All three moves preserved
the exact-match count; the public character declaration lives in `charconfig.h`.
The mixed `terrain_runtime.cpp` was removed: `TerrainTrackFlush` now lives with
terrain tracking, and `DebrisSetTimeIncrement` with debris timing state.
The original LOCAL `crashdata` now lives with its three verified users in
`gameliball.cpp`; `CrashDataPtr` moved there from generic memory code, and
terrain file-loading callers use a real owner header. This raised assessable
same-TU state links to 2,242/2,249 without an exact-match loss; whole fuzzy
changed by -0.000072 percentage points.
The data-only `nuthread.cpp` was retired: original BSS places
`nu_current_thread_id` immediately after the six `nuthread.c` vectors, and
the global now shares their TU. This preserves the body score; exact BSS
offset order within the object remains a separate compiler-layout question.
The data-only `nutexanm.c` was also retired: its critical-section global now
shares `nutexanim.cpp` with all current users, and the four original BSS names
(`g_texAnimCriticalSection`, `ntal_free`, `ntal_first`, `ntalsysbuff`) have the
same relative order in the owning object. Exact/fuzzy matching is unchanged.
The one-function menu placeholder `gamemessages_stubs.cpp` was removed:
`numeminit` now sits between the exact `memmove` and near-exact
`NuMemGetExternal` in their original memory-TU order, with its declaration in
`numem.h`. Matching is unchanged.
The exact `Hat_GetAbsTargetPos` also moved from a duplicate `props/objects`
file into its original position between two hat-machine methods in the
existing gizmo owner. Its real header now declares it; the exact match holds.
`InitPaintPuzzle` left a one-function placeholder and now sits between its
`Reset` and `Update` peers in `gizmo.cpp`, with the public trio declared in
`gizmo.h`; exact matching is unchanged and fuzzy rises slightly.
The ledger also records 13,042 individual `.text` adjacency links as weak
evidence, not TU assignments. Against current-build `STT_FILE` labels, only
3,189 links were assessable and 89.84% joined the same file; address order
alone is insufficient to reconstruct the original boundaries.
The optional Capstone-based pass verifies 9,347 original function-to-LOCAL-object
instruction references (9,313 writable state, 23 read-only constants, 11
unclassified). It follows branch/loop paths only while every reachable
predecessor agrees on the exact EBX, ECX, or EDX PIC base, respecting each
register's call-preservation rule. Of 4,645 links with unique current source
candidates on both ends, 406 cross current-file boundaries, of which 386 have
a size-concordant current object candidate. These are diagnostic, not automatic moves. The
largest cluster links editor UI callbacks to local editor state. This pass
requires Capstone; the default ledger does not.
The writable-state links form 772 minimum same-TU components covering 5,333
symbols, not complete files. 288 components span the initializer-order
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
| Android material/render state: local callback/data clusters | `nu2api/nu3d/android/numtl_android.cpp`, `nurndr_android.c` and renderer neighbors | The material alpha-test BSS group and original `-1` last-state initializers now live with their material setter. Remaining callback boundaries and codegen need per-unit review. |
| Material override list: three `NuMtl*Variant/Override` exports directly reference the local `g_overrideList` | `nu2api/nu3d/numtl.cpp`; formerly the three stubs in `nurndr_plain.cpp` | The exports now share their evidenced list owner, closing three verified splits without changing body matching. Their stub signatures and the wider renderer/material TU boundary remain to reconstruct before publishing a typed API. |
| `nurndr_android.c`: original text `[0x293168, 0x295f80)` through its named constructor, with VAO, callback, blend, and shadow locals | `nu2api/nu3d/android/nurndr_android.c` | **Mostly placed.** Renderer/diagnostic helpers and the VAO counter share the original `-O0` run; two obsolete one-function files were removed. Text alignment is 96.1% (61 in order; 62 original, 65 current); all five assessable LOCAL-state links agree. The five existing binding-cache BSS symbols also have zero order inversions; `g_boundSkinPacket` remains absent. Record-array layout and shader attribute-name data remain open. |
| `nurndr.c`: original text `[0x295f80, 0x296aac)` including specular, present, and FX | `nu2api/nu3d/android/nurndr.c` | **Mostly placed.** Specular and FX moved from separate files without rewriting their bodies; FX became exact, and all three assessable LOCAL-state links agree. Ten of 11 original text symbols are present in source order; missing `VuVecSet` and two extra header-emitted helpers keep ordered-symbol alignment at 87.0%. |
| `nurendercontext.cpp`: original run `[0x2a30d0, 0x2a3b0c)`, named initializer, and contiguous context data/BSS | `nu2api/nu3d/nurendercontext.cpp`; formerly `nucore_plain.cpp`, `nurenderthread.cpp`, `nurndr_android.c` | All eight reported context functions are in original address order (100% ordered-symbol alignment); the 11 existing BSS names have zero relative-order inversions. Retained `-O3`: `-O2` tied on whole fuzzy/exact counts, while `-O0` had four fewer exact functions and 0.0157 fewer fuzzy points. The original `g_renderContext_zFunc = 3` initializer is restored; target/native/WASM and 120-frame Map smoke pass, with one more exact match and no exact losses. Absent timing/event state is not fabricated. |
| `nucamera.c`: original run `[0x2a3b0c, 0x2a5386)` ending in `_GLOBAL__sub_I_nucamera.c` | `nu2api/nu3d/nucamera.c`, compiled as C++ | Six existing global bodies now occur in original order under the original basename (60.0% whole-run alignment). The exact `NuVecMtxTransformBlock` survived migration from `nuvec.cpp`; callback-local `lastId` now has the original symbol name and `-1` initializer. Original `clip_test_mtx` anchors the BSS boundary. The two local VU helpers, six constructor-initialized vectors, and initializer remain open; do not emit unused definitions solely for score. |
| Android platform-stub run from `0x316a49`, including `NuRndrGradClear` and renderer no-ops | `nu2api/nucore/android/stubs_android.cpp`; formerly also `nurndr_grad.cpp` and `nurndr_noops.cpp` | Four adjacent functions moved to the 36-function run; all remain exact. The two tiny source files were removed. `NuRndrClear`/`NuRndrGradClear` now have canonical signatures in `nurndr.h`, replacing scattered local prototypes. |
| Android portal visibility: contiguous `nuportal_android.c` helpers at `0x2fc209–0x2fc7fe` | `nu2api/nu3d/android/nuportal_android.cpp` with real platform header; shared portal engine remains in `nuportal.cpp` | Five helpers now co-located at the existing default optimization, preserving all three exact matches and whole score. Forced `-O2`/`-O3` trials regressed and were rejected. `PortalVisiFlags` remains original GLOBAL data. |
| Android scratch, graphics clear, rain, FMV, and time: embedded paths/initializer runs | `nuscratch_android.c`, `ios_graphics.cpp`, `nurain_android.c`, `nufmv_android.cpp`, `nutime_android.c` | Source boundaries accepted; individual scratch/clear/media bodies remain incomplete. |
| DDS and texture-animation owners: DDS initializer/local vectors and texture-animation run | `nu2api/nu3d/NuDDSFunctions.cpp`, `nu2api/nu3d/nutexanim.cpp` | File/data ownership largely reconstructed; DDS mip/description and some animation bodies remain. |
| Texture manager: original `NuTexDestroy` at `0x2fb710` references LOCAL `texture_list` alongside texture allocation and lookup methods | `nu2api/nu3d/nutex.cpp`; formerly one destroy stub in `nurndr_plain.cpp` | The existing destroy stub now shares the `-O3` source owner with its real texture-list state and neighboring API. Whole/per-function matching is unchanged; its body remains unfinished. |
| Still/pause screen: embedded `screen.cpp` path and eight-function `0x48c540–0x48cf10` LOCAL-state component | `legoapi/render/core/screen.cpp` and `screen.h`; formerly two draw bodies in `render.cpp` | All eight functions and five current LOCAL objects now share the owner; the eight functions emit in original order. `pause_rndr_mtl` is LOCAL and callers use a real screen header. The two moved draw bodies rise to 78.75% and 45.84%; `-O2` beats a measured `-O3` trial. Whole fuzzy rises 46.0601%→46.0647%. One unrelated render neighbor changes only a temporary register and falls from exact to 99.86%; no source-level correction is supported. |
| Shader-source lookup: adjacent `LoadShaderSource`/`LookupPreloadedShaderObject` at `0x30dac0–0x30dc60`, with function-local 16 KiB storage in the original `nushadermanagerios.cpp` initializer block | `nu2api/nu3d/nushadermanager_plain.cpp` and existing `nushader_plain.h`; formerly stray helpers in `screen.cpp` | Both helpers now sit between `LookupHash` and `createShader` in original text order beside their current callers; the local buffer moves with its function. `LoadShaderSource` rises 99.64%→99.96% at the manager's measured `-O3`, with exact count unchanged. The complete original shader-manager platform split remains open. |
| Display-list creation and debug: `NuDisplayListCreate` (`0x2e87d0`), `DisplayListPrintItem` (`0x2ec550`), and `DisplayListCreateDynMtlList` (`0x2ef660`) lie within the display-list run | `nu2api/nu3d/nudlist.cpp`/`nudlist.h`; formerly split across `nu2api_nucore_misc.cpp` and `supportall.cpp` | Three existing definitions share the display-list owner and its real header; exact matching is unchanged, with a small fuzzy gain. The print body remains an explicit stub. |
| NuQT quadtree: contiguous `0x261783–0x2626f4` run and verified node layout | `nu2api/nucore/nuqt.cpp` and `nuqt.h` | Core owner reconstructed; insertion is still a body-level gap. |
| NuQFnt early helpers: `0x2c9a40–0x2ca7a7` font-mode/Unicode run with shared font state | `nu2api/nu3d/nuqfnt.cpp` plus remaining wrappers in `nucore_plain.cpp` | `UnicodeToIndexFast` moved out of menu text to the font owner without a match change, and its declaration is in `nuqfnt.h`. **Open:** a measured, all-at-once trial of the remaining early wrappers must protect their exact matches across the current `-O3`/`-O2` split; later font functions are a separate address run. |
| NuMemory pool/manager: pool run, method ABI and vtable/local data | `nu2api/nucore/NuMemoryPool.cpp`, `NuMemoryManager.cpp` | Strong boundaries and many exact helpers; page lifecycle and large-bin methods remain. |
| Animation action/context tables: consecutive `LEGOACT_*` and `LEGOCONTEXT_*` data | `legoapi/characters/motion/animation.cpp`, `contexts.cpp` | Tables and owner split reconstructed; some initialization/body ownership remains provisional. |
| Collection list and unlock queries: original `0x4db6e0–0x4dd3f0` run and LOCAL `CollectList`/`CollectCount` in the `collection.cpp` initializer block | `legoapi/items/base/collection.cpp` and `collection.h`; formerly two state readers in unrelated misc/character files | All seven functions and both LOCAL objects now share one owner; 14/14 candidate state links are source-assigned without splits or GLOBAL stand-ins. Function and BSS order match the original. `CollectIDUnlocked` rises 25.96→29.69%, whole fuzzy 46.075012→46.079280%, and exact matching stays at 4,759; absent collection behavior remains a separate body-level gap. |
| Part-debris table: original `InitPartTable` → `LoadPartFile` → `AddPartDebris` at `0x4a2e80–0x4a3270`, tied to LOCAL `PDEBCOUNT`/`PDebNameList` under the `parts.cpp` initializer | `legoapi/render/fx/parts.cpp` and `parts.h`; formerly the loader and state in terrain, insertion in generic support | Three functions and two real LOCAL objects now share one owner in original relative order; all 5/5 state links are source-assigned without splits. Callers use the parts header rather than duplicate prototypes. `InitPartTable` rises 64.40→99.85% and `AddPartDebris` 77.93→89.97%, while `LoadPartFile` falls 69.45→64.49%; whole fuzzy still rises 46.079280→46.079770%, with all 4,759 exact functions retained. The loader codegen and full original TU extent remain open. |
| `game_deb.cpp` action/context lookup data: adjacent `ExtraActionData`, exported pointers, and paired local tables before its initializer | `legoapi/render/fx/game_deb.cpp` and canonical `game_deb.h`; previously `globals.cpp` | Complete data cluster moved together. A normal declaration reorder makes GCC 4.7 emit all five in original linked order (zero inversions among the ten pairs), retaining names, binding, sizes (`0x108`, `0x750`, `0x650`), and body matching. **Open:** pointer/array spacing still differs; do not force it with attributes, fake state, or a TU split. |
| `saveload.c`: contiguous `0x308045–0x308bbc` text run and four function-local 4 KiB buffers | `gameframework/saveload.cpp` and `saveload.h`; formerly split across `characters.cpp` and a catch-all config file | All 28 reported functions now share the evidenced owner, with original LOCAL buffer names, sizes and order. `fullcodename` is exact; the remaining unfinished card APIs retain explicit stubs without matching regression. |
| `episodeVI.cpp`: consecutive `0x228db0–0x22a2c0` level/bonus tail, with `NewTown_Reset` referencing LOCAL `prevOnTaunTaun` | `legoapi/world/levels/episodeVI.cpp`; formerly also `lego_city.cpp`, `senate.cpp`, `new_town.cpp` | Ten handlers and four local state bytes co-located; three unjustified tiny files removed. Whole fuzzy rose 0.0028 points, `NewTown_Init` 13.61→99.61, no exact matches lost; the later `Platform_*`/bonus tail moved without score changes. `LegoCity_Reset` slipped 94.17→93.74 on local EBX displacements while its state order/spacing remain original; GOT placement is still open. |
| Streaks: `0x4a3f80–0x4a4ca2` text run and local `streakhdrs_used` reference from `DrawStreaks` | `legoapi/actions/character/streaks.cpp` and `streaks.h`; formerly `DrawStreaks` was in `render.cpp` | The five exported streak functions now share the evidenced owner and a real API header. Whole and per-function matching are unchanged; unfinished bodies remain stubs. |
| Android shader program: contiguous `0x2a5390–0x2a5cde` helpers/program run and local `programPool` | `nu2api/nu3d/android/nushaderprogram_android.cpp`; shared declarations in `nushader_internal.h` | Full owning group moved from `nushader.cpp`. The original five-object BSS sequence—uniform-record storage, `g_currentShaderProgram`, pool, allocator, function-local `uniformName`—now has 100% ordered alignment. The pointer moved from renderer without a matching change; remaining body work is separate. |
| Shader object and base: embedded `shaderbuilder/android/nushaderobject.cpp` path, object run `0x309da0–0x30d5b2`, separate `_GLOBAL__sub_I_nushaderobjectbase.cpp` and base run `0x30e3b0–0x30e7a1` | `nu2api/nu3d/android/nushaderobject.cpp`, `nu2api/nu3d/nushaderobjectbase.cpp`; previously split across `nushader.cpp` and catch-all files | Complete object/base families split with real local shader data and original global `GetUsageMask`; its placeholder was removed. `-O3` and internal header declarations retained. Matching improved; remaining bodies are a separate task. |
| Shader-manager hash redirect: `LookupHash` at `0x30d910` between manager methods and `NuIOS_GetShaderProgramKey` | `nu2api/nu3d/nushadermanager_plain.cpp`; formerly menu text | The existing implementation now shares its callers and canonical shader header at `-O3`, retaining its 89.49% body score and whole exact matching. |
| Nu3D dynamic-light pools: three adjacent locals, their direct constructor/create/destroy xrefs | `nu2api/nu3d/nu3d_includes.cpp`; previously `nucore.cpp` | All three pools and four direct owner methods moved together. Original local bindings and sizes (`0x90`, `0x210`, `0x3f10`) survive; all other class methods remain provisional. |
| ANI3/NuAnim: apparent `0x2bd180–0x2c8253` run, local `KeyStructSizes`/`CurveGroupMasks`, and evaluator `scale_array` | `nu2api/nucore/nuanim3.cpp` (`-O2`) for the 11-player/extractor dependency group; `nucore_plain.cpp` (`-O3`) for evaluator/accumulator and six-function curve tail; `nuanim.cpp` unchanged | The two local tables and their direct users now share a source owner. Isolated trial gained 0.0001 fuzzy points with no regressed functions; the complete original ANI3 boundary remains **open**. Whole-family co-location trials regressed and were rejected. |
| `apiobject.c`: `AnimEndFrame` through `ConfigureCharacterList`, ending at `_GLOBAL__sub_I_apiobject.c` (`0x3d223a`) | `legoapi/items/base/apiobject.cpp` and its real shared declarations | All 78 scored bodies in this run are already assigned to the owner in original order; local animation/API state and the six initializer-constructed vectors are likewise attributed there. No further high-confidence source move is available without recovering absent TU-local math helpers or changing bodies. Neighboring globals are not assigned by adjacency. |
| Post-API-object span: AI expression, editor path/area/creature/locator, and gameplay functions before the next named initializer at `0x52b116` (`glutils.c`) | Several AI/editor/gameplay owners | **Boundary unresolved.** The ~1.3 MiB initializer-free interval is not one inferred TU. Its verified local-state links are sparse or already co-located; missing editor iterator state and ambiguous `attr` candidates prevent a clean atomic cut based on adjacency alone. |
| Editor path connection state: one original LOCAL `aipathcnxtypes`/`naipathcnxtypes` pair is referenced by path UI, registration and drawing helpers | `editor/edpath.cpp` with public API in `editor/path_connections.h` and `edpath.h` | The duplicate incompatible state was removed; one correctly typed 0x4c-byte record and local array now serve registration and UI. The drawing stub also moved beside its original state, closing four verified editor-path splits. All 29 exact neighbors remain exact, `pathEditor_Process` rises from 0% to 28.61%, and whole fuzzy rises 45.9964→46.0600%. **Open:** the full original TU extent and drawing body. |
| Editor cylinder check: `AISysSetPathCylinderCheck` writes a two-byte LOCAL flag read by `aieditor_Proc` in the adjacent original editor run | `editor/aieditor.cpp`; API remains in `aisys.h` | Setter and real local state now share the reader's owner, closing one verified split with no exact loss and a small fuzzy gain. The wider editor/AI TU boundary remains unresolved. |
| Editor interaction callbacks: original `cbInteractMenuTitle` → scroll-up → scroll-down run at `0x38fc30–0x38fd14`; title references two LOCAL editor UI state objects | `gameapi/edtools/edtoolsall_plain.cpp` and `edui.h`; formerly the three callbacks were in game-menu code | All three emit in original order beside their UI state; both scroll callbacks stay exact, whole matching is unchanged, and two verified writable-state splits close (2,225/2,243 satisfied). The broader callback-table owner remains open. |
| Character scene/icon/variant family: adjacent text, local `CharScene_Area`, `IconScene`, `IconPath`, `CharVariant`, and `CHARVARIANTCOUNT` | `characters.cpp`, `gamemenuall.cpp`, `charconfig.cpp`; shared API in `character.h` | **Open.** `IconPath` and the exported 23-entry variant pointer table now have the original data type/content, and variant methods have real near-exact bodies. Partial co-location trials regressed neighbors and were rejected; resolve the full owner boundary before retrying. |
| Player and character layers: original `GetOtherActivePlayer` → `AdjustLayerBits` → `FixUpLayers` run shares local `SpecialLayer`; `LayerFromName` → `MakeLayerList_Name` → category initialization is a separate `0x460a80` run | `characters/core/players.cpp` and `charconfig.cpp`; formerly the layer pair/table and name lookup were in `gizflow.cpp`, and the list builder in menu text | Each group now has one plausible source owner and canonical declarations. The four paired player functions emit in original order (zero inversions); exact matching stays at 4,760, fuzzy rises 45.995903→45.996340, and the list builder 98.48→99.61%. Full original player TU extent remains open. |
| Vehicle motion: original `Move_POD` → `Move_VEHICLE` → `Move_REPUBLICGUNSHIP` run; `Move_VEHICLE` references the local hover-height flag | `characters/motion/move.cpp` and `characters/motion.h`; formerly the vehicle stub was in `players.cpp` | The existing stub now shares its evidenced state owner and has a canonical motion declaration. One verified state split closes with no exact or fuzzy body change; the broader motion TU boundary remains open. |
| Mini-cut camera state: `GameCam` points to local `_ZL10GameCamera` in the `_GLOBAL__sub_I_gizminicut.cpp` block | `legoapi/gizmos/trigger/gizminicut.cpp`; previously `globals.cpp` | State and exported pointer co-located with the original owner. The target keeps the original local name and `0x230` size; matching is unchanged. Adjacent camera functions remain separate pending xrefs. |
| Gizmo registration, blowups, and pickups: initializer blocks, callback arrays and adjacent runs `0x4b7540–0x4c28a0`; `SpecialMiniKits_Configure` → `Reset` → `Draw` at `0x4c2000–0x4c28a0` share pickup-local state | Singular owners under `legoapi/gizmo` and `legoapi/gizmos`; the three special-minikit methods now share `gizmos/fx/gizmopickups.cpp` and its API header | The mini-kit run has 3/3 paired symbols in order and 2/2 state links co-located, closing one split with no matching regression. Other pickup bodies remain. Do not merge unrelated helpers by name alone. |
| Flow-box runtime: `0x4b1710–0x4b4afe` run, local `flowboxtypes` callback table and loop checksum under the `gizflow.cpp` initializer | `legoapi/gizmo/base/gizflow.cpp`; formerly runtime callbacks/state in `gizmo.cpp` | The complete processing cluster, table and four public runtime methods now share one source with a real module header. Measured `-O3` raises whole fuzzy by about 0.035 points and two exact functions versus the prior commit; no exact losses. The current emitted function order still differs, and original TU extent beyond this run remains open. |
| Zipup/rope: zipup run `0x1d1440–0x1d4910` and local start-point helper | `legoapi/gizmos/door/zipups.cpp`, `props/objects/zipup.cpp`, rope owner | Zipup stages are partly reconstructed; rope draw and some state remain open. |
| Grapple transport: consecutive `Grapple_FindNearestInList` through `Grapple_LookAtPos` (`0x4d5040–0x4d70f5`) and local grapple state | `legoapi/gizmos/transport/grapples.cpp`; formerly seven stubs in `gizmo/gizmos/gizmos_grapples.cpp` | All seven placeholders moved intact, eliminating the emptied file and two verified state splits. The selected 13-function run now has 13/13 paired symbols in exact original order, with 3/3 assessable local-state links co-located. Matching remains 4,758 exact with a negligible fuzzy gain; real bodies and wider grapple data layout remain open. |
| Snake body: seven-function original `InitSnakes` → `SnakeBeenHit` run, tied to local `snake_hspecials` and the `snake.cpp` initializer block | `legoapi/actions/character/snake.cpp`; formerly `DrawSnakeBody` in generic `render.cpp` | All seven existing definitions now share one source and emit in exact original relative order, closing one verified state split. The render call uses the real snake header; body matching is unchanged. |
| Episode I levels: text `0x1fa8e0–0x203d50`, including pod-race/mushroom handlers interleaved with level functions | `legoapi/world/levels/episodeI.cpp`; formerly five `Action_*` handlers in `gizmos_gizactions.cpp` and three `PodSeek*CutSound` handlers in `speederchase.cpp` | Eight existing stubs moved into original relative text order with their level-owned state, closing fifteen verified local-state splits without changing body matching. The measured run has 67 paired text symbols with zero relative-order inversions and 79/79 assessable local-state links co-located. The separate gizmo action-table run remains in its own owner; `Action_BoulderSection` is outside this level run. |
| Level-object runtime: adjacent `LevelObjects_InitForGame` → `LevelObject_AddExtra` → platform-ID helpers at `0x475400–0x4759b0`, with four original LOCAL BSS objects used by the first two functions | `legoapi/world/levelobjects.cpp`; formerly `LevelObject_AddExtra` in `level.cpp` and four GLOBAL state definitions in `globals.cpp` | The two state users and all four real LOCAL objects now share one source; all seven distinct state links are co-located, and the four emitted functions follow original relative text order. `LevelObject_AddExtra` rises 42.94→65.96% and whole fuzzy 46.070103→46.070942% with no exact loss. The index object still emits ahead of the other three BSS objects; the wider `levels.cpp` TU boundary is not proven by this component alone. |
| Hub menus and door lights: select/bonus callback run plus `TurnEpisodeDoorLightsOn` between hub functions at `0x1a6550`, referencing local `HubEpisodeInfo` | `legoapi/world/levels/hub.cpp`; formerly callbacks in `gamemenuall.cpp` and the light stub in `lighting.cpp` | The callbacks and door-light stub now share their evidenced hub state owner, closing seven verified splits without changing body matching. Public declarations use real menu/level headers; larger menu-run ownership and optimization still need review. |
| Socket parser: `SockPar*` callback run `0x3c0594–0x3c15da` and local parser arena in original `socksysall.c` | `legoapi/props/system/socksysall.cpp`; formerly callbacks/table in `gameapi/ai/gameai_sockpar.cpp` | All 46 existing callbacks and the unchanged keyword table now share the original parser-state owner; the emptied source was removed. The callback run has 47 paired symbols and one relative-order inversion, versus 497 before ordering; two verified local-state splits closed. Exact matches are unchanged, fuzzy improved slightly. The original table is 456 bytes versus the current 376, so its contents/layout remain open. |
| Bolt-type parser: `BT_*` callbacks interleave bolt helpers at `0x47ee30–0x47fadd`, sharing four local parser-state objects and a `bolts.cpp` initializer | `legoapi/items/collect/bolts.cpp`; formerly also `gameapi/ai/gameai_bt.cpp` | The parser callbacks/state/configure stub now share the bolt owner. Measured `-O3` preserves all 11 prior exact bolt functions and raises whole fuzzy by 0.0308 points; the initializer rises from 0 to 99.35% and the separate file is removed. **Open:** the missing 216-byte keyword table, callback emission order, exact TU extent, and the distant `Bolt_HitPlat` fragment. |
| Cutscene instance lists: `instNuGCutSceneFind`/`CleanUp` and `PetesHackOfDeath` reference local active/background lists; the latter directly follows `NuGCutSceneSysPostBackgroundLoad` | `legoapi/cutscenes/cutscene.cpp`; formerly two stubs in `cutscene_stubs.cpp` and the hack stub in `gamemenuall.cpp` | All three stubs now share the evidenced list owner, closing four verified splits total without changing body matching. The broader cutscene stub file remains unassigned. |
| Cutscene lookup run: `CutScene_FindInst` → `CutScene_Find` → `NewCutScene` at `0x49d010–0x49d180` | `legoapi/cutscenes/cutscenes.cpp`; formerly `CutScene_Find` alone in `cutscene_find.cpp` | The lookup now emits between its original neighbors, improves from 99.39% to 99.69%, and retires the single-function source. Wider cutscene TU boundaries remain open. |
| View-text menu pair: adjacent `MenuUpdateViewTextStrings`/`MenuDrawViewTextStrings` at `0x48f6a0–0x48fef0`; the draw method references local `Text_StringBits` | `legoapi/menus/core/text.cpp` and `text.h`; formerly both stubs in `gamemenuall.cpp` | Both share the text-state owner in original relative order, closing one verified split without changing matching. |
| Game-menu header state: `MenuInitialiseEx` and eleven later menu functions share three LOCAL header colors plus slot-navigation state in original component 523 | `legoapi/menus/screens/gamemenuall.cpp` with `gameapi/gui/apimenu_internal.h`; formerly the initializer, its exact wrapper, exact `MenuReset`, callback pointers, and colors were in `apimenu.cpp` | All 12 assigned functions and six present LOCAL objects now share one source; 39/40 distinct state links are assessable and none cross files. The colors have their original LOCAL binding; `MenuInitialise` and `MenuReset` stay exact, the six-function `MenuSetColours`→`MenuReset` run emits in original text order, and the callback pointers emit in original relative data order. `MenuInitialiseEx` rises 70.16→75.06%, and whole fuzzy rises 46.070942→46.075012% without exact loss. The seventh LOCAL object, `MenuDrawDeleting::messageswitched`, is absent because that function is stubbed; this component is not the full game-menu TU. |
| Panel: `0x140950–0x1449c0` run; `Panel_Clear` and `DrawPanel` access the same local `redbrickslidetime` | `legoapi/menus/core/panel.cpp` and `legoapi/render/core/render.cpp` | `DrawTimer` and its state moved. **Open:** full `-O2` co-location is structurally supported but costs 0.0049 overall points via `DrawPanel`; `-O3` helps the initializer but drops `PanelRender` by 10 points. Both trials were rejected; the accessor remains temporary. |
| Status-screen draw/update: original `0x2502c0–0x255dcf` and local gold-brick/demo state | `legoapi/menus/screens/gamestatus_lsw.cpp` and its header; formerly `DrawStatusScreen` in generic render | Three assigned functions now follow original relative order in one source; six assessable local-state links co-locate, one `KitPart` link remains unassessable because the current object is absent. Exact matches stay 4,759; whole fuzzy rises 46.079770→46.079830%. |
| Cheat-system run: `Cheats_Init` through `Cheat_SetArea` at `0x4e10e0–0x4e196f`, with local `CheatSystem` and power-up timer | Still split between `core/config/cheat.cpp` and `cheats.cpp` | **Open:** full co-location at current `-O3` lowers the fuzzy score; applying the current `cheat.cpp` `-fPIE` mode to the whole group loses an exact match. Both trials were reverted. The menu's direct access to a currently global `CheatSystem` also conflicts with the original LOCAL binding; resolve source semantics before another move. |
| Combat opponent state: original `ObjOpponent`/`PunchCode`/`SetComboOpponent`/`SwipeCode` run and three LOCAL range/behind objects | Still split between `actions/character/speederchase.cpp` and `characters/motion/move.cpp` | **Open:** moving the complete state group into the `-O2` move owner closes the local links but lowers `ObjOpponent` and whole fuzzy; trial reverted. Determine the original optimization/codegen boundary without per-function attributes. |
| Post-filter: `nupostfilter.cpp` local block, including `texturePool` at `0x11cd820` | `nu2api/nu3d/android/nupostfilter.cpp`; framebuffer pair in `nuposteffect_plain.cpp` | Measured split and genuine Init/Destroy flows improved matching. Both files require `-O2`. The 0x810-byte local pool has no verified text xref or field layout, so it is not fabricated in source. |
| TimeBar profiling: slot/render functions and local render/peak state | `nu2api/nu3d/android/nutimebar_plain.cpp`; formerly three APIs in `nucore_plain.cpp` | `NuTimeBarInit` and both render-setting APIs now share their evidenced state owner; the exact init wrapper survives unchanged. Public declarations moved to the real TimeBar header, removing a frame-loop prototype. Six verifiable local-state splits closed with no body-matching change. Full original TU boundary and basename remain open. |
| NuTime frame helpers: consecutive `NuTimeStartFrame`/`GetStartFrame`/`GetSinceStartFrame` between other NuTime APIs | `nu2api/nucore/nutime.cpp`; formerly three stubs in `nucore_plain.cpp` | The three existing stubs now share the time module in original relative order; exact matching is unchanged and fuzzy rises slightly. The original 4-byte local `frameStartTime` is still absent; the unrelated 8-byte utility static is not assigned to it by name. |
| NuHtml/graph run: contiguous `0x2d5c30–0x2d73ef` helpers and their HTML/graph state, ending before TimeBar | Still split among `supportall.cpp`, `nucore_plain.cpp`, and `render/fx/edsplines.cpp` | A complete dedicated-TU trial at both `-O2` and `-O3` retained all 4,760 exact functions but lowered whole fuzzy by 0.00004 points, so it was reverted. This is separate from the later display-list owner; source ordering/optimization remains open. |
| NuSound buffer: embedded `nusound_buffer.cpp :53` path and adjacent method run | `nu2api/nusound/nusound_buffer.cpp` | Owner and `-O3` are supported; allocation control flow improved, with other method bodies still open. |
| Music pair lookups: `ActionFromQuiet`/`AmbientFromQuiet` immediately precede `ConfigureMusic` and reference its two local table pointers | `nu2api/nusound/nusound3_include.cpp`; formerly lookup bodies in `legoapi/audio/sfx.cpp` | Both lookups now share their original local-state owner and read the tables populated by `ConfigureMusic`, rather than unrelated private arrays. Their pair strides and `-1` handling are retained; each focused body match rose from 66.5% to 67.4%, with a slight whole-fuzzy gain. Public declarations live beside `ConfigureMusic` in `nusound.h`; world code includes that owner header. |
| SFX group buffer: nine-function `0x347830–0x347e20` run; `GroupBuffer_RemoveGroup` directly references local `NumSfxInst` | `nu2api/numusic/sfx.cpp` and `sfx.h`; formerly a separate `legoapi/misc/gamelib_utilities.cpp` | All nine functions now emit in original relative order beside the SFX state; five exact matches and whole fuzzy are preserved, one verified state split closes, and the redundant source is removed. The wider audio TU boundary remains open. |
| Editor grass page load: `edgraLoadPage` references local grass-thinning settings alongside the editor grass page state | `gameapi/edtools/edtoolsall_plain.cpp` and `edgra.h`; formerly a generic editor stub file | The existing explicit stub now shares the evidenced grass owner, closing two verified state splits without an exact-match loss; its implementation and the wider editor TU boundary remain open. |
| Fade frame-wait state: both still-fade initializers and `SetFramesToWait` reference local `FRAMES_TO_WAIT` inside the fade initializer block | `legoapi/render/light/fade.cpp` and `fade.h`; formerly the setter and state were in `core/input/timing.cpp` | The setter and state now share the fade owner, closing two verified state splits. Normal definition ordering raised selected ordinary-text alignment to 78.0%. Four getter definitions in the module header now have the original weak binding and remain byte-exact; their COMDAT sections are not comparable in that ordinary-text score. Whole exact/fuzzy matching is unchanged. Linked placement and the wider boundary remain open. |
| Message-box text: `MessageBoxInitMtl` → `DrawMessageBoxRGBA` → `DrawMessageBox` → `SmartTextEx` in the original `0x42d530–0x42dd80` run | `legoapi/menus/core/text.cpp`; formerly three stubs split between game-menu and render files | Three existing stubs now share the text owner and its local font-scale state; the measured run has 5/5 assessable local-state links co-located. Exact/fuzzy body matching is unchanged. |
| `numaths.c`: text `[0x290b20, 0x292e9c)` including its trailing static initializer and constructor | `nu2api/numath/numaths.c`, compiled as C++; formerly split across seven math/engine files and two game files | All 37 reported exported functions in the measured run now share one source in original address order; the empty `nucamvu0.c` shell was removed. The shared internal `VuVec` helper header emits this TU's byte-exact LOCAL `VuVecSet` and six original-sized LOCAL constants; its original-named constructor is exact. The initial consolidation raised whole fuzzy by 0.0292 points and gained eight exact functions, none lost. **Open:** original LOCAL `rand` and final optimization calibration. The preceding `numath_includes.c` constructor remains separate. |
| `nufloat_android.c`: text `[0x292e9c, 0x293165)` with LOCAL `VuVecSet`, four exported float helpers, six LOCAL vectors, and its initializer | `nu2api/numath/nufloat_android.c`, compiled as C++; formerly split across `nufloat.c` and `numaths_plain.cpp` | The four existing bodies now form the evidenced original run. Its own `VuVecSet` and six constants come from the same narrow internal header as `numaths.c`; constructor and exported signatures retain their original names, binding, and sizes. The move adds a second byte-exact LOCAL helper and an exact constructor without regressing exported functions. |
| Terrain data/collision: `_GLOBAL__sub_I_gameliball.cpp` and original strong-local component 463 | `legoapi/render/core/terrain*.cpp` | **Open:** its 91 functions/48 LOCAL objects now map to two current sources (43 terrain stubs, 48 terrain core), down from ten at survey time; their original address order alternates in 21 owner runs. All 34 unpaired LOCAL objects have one same-sized GLOBAL counterpart (11 core, 23 stubs). This provisionally resolves all 295 distinct LOCAL-state pairs, of which 116 still cross the two source files; co-locate users before restoring local binding. Terrain APIs have a real owner header, and the wall-deflect, rotation, scan-control, pickup, extended-shadow, collision, and platform-connection groups are more closely placed without an exact-match loss. This is a minimum same-TU constraint, not proof that adjacent editor code belongs here. |
| Terrain platform-skin cache: original strong-local component 467 under the same initializer block | `legoapi/render/core/terrain_stubs.cpp` | All six functions and ten LOCAL objects now share one source; its 25 writable-state pairs have no current split or provisional counterpart. The six emitted functions follow original address order, and seven adjacent cache BSS symbols follow original relative order; two later BSS names remain reversed. Restoring nine real LOCAL bindings improves whole fuzzy matching to 46.070103% without an exact-match loss. `SkinPlatform` itself remains outside this component pending a wider matching-safe cut. |
| Debris generator/seed run: original component 446 and adjacent trigger processor; LOCAL `debrisseed` | `legoapi/render/fx/game_deb.cpp`; formerly four seed/generation functions in `terrain_stubs.cpp` and `supportall.cpp` | All 12 component functions and the original-named LOCAL seed now share one source in original relative order, with 12/12 state links co-located; the adjacent `DebrisProcessTriggers` and its shared helper moved with generation. Exact matches stay 4,759, whole fuzzy rises 46.079830→46.084892%, and linked GOT order is unchanged. The broader debris TU boundary remains under survey. |
| Editor tools: local camera/UI/cursor/menu state under `_GLOBAL__sub_I_edtoolsall.cpp`, text span `0x330900–0x3a9c88`; RTL follows | `gameapi/edtools/edtoolsall_plain.cpp`, `edtoolsall.cpp`, `edptlall.cpp`, remaining editor callbacks | **Open.** The `-O2` plain owner holds 49 local editor-UI callbacks from the removed `edui.c`, plus two menu callbacks referencing its state; 42 verified splits closed without body regression. Two repeat-box callbacks share the particle editor owner with both sliders, closing four more splits. Six called item-creation stubs have typed declarations in `edui.h`. A 28-stub `edptl*` co-location trial preserved every retained body's score, but all 28 moved static stubs disappeared under the owner's `-O2`, lowering whole fuzzy by 0.0058 points; it was reverted. Missing real callback-table references are a likely cause to investigate, not a reason to force emission. |
| `rtl.c`: contiguous text `0x3a9c88–0x3bc1bf`, named initializer, adjacent writable state and pointer-table relocations | `legoapi/render/core/rtl.c`, compiled as C++; formerly `rtl.cpp`, `edrtl.cpp`, `edrtlall.cpp`, `edrtlcallbacks.cpp`, HUD and five smaller fragments | Core RTL and editor state now share the evidenced source: 200/203 original text names are present, including an exact initializer, and all 11 uniquely paired linked-`.data` names now have zero order inversions. The editor consolidation closed 51 verifiable local-state splits; its timer moved out of `globals.cpp`, and the batch gained two exact matches overall. Existing function definitions have since been reordered in coherent groups without altering bodies: ordered-symbol alignment rose from 23.9% to 66.3% (136 symbols in order), and pairwise inversions fell from 6,668 to 2,315. All 184 assessable RTL local-state links remain co-located; exact/fuzzy matching is unchanged by the ordering pass. **Open:** interleaved core/export order breaks, absent local VU/NuFabs helpers, much absent state, and byte-level pointer-table relocation order. Input `.data.rel.local` tables link into `.data`; that input-section name alone is not a placement regression. Retained `-O3` after an `-O2` trial lost 14 exact matches and 0.2534 fuzzy points. |
| Menu/customiser, timing, and large catch-all files | `customise.cpp`, `timing.cpp`, `nucore_plain.cpp`, `edtoolsall*.cpp`, `aisys.cpp`, `gameobjects.cpp` | **Open.** `APIMenuDrawMemCardSlots` now sits with load/save menu state in `gamemenuall.cpp`, leaving its public declaration in `apimenu.h`. Complete gizmo-interface and `BaseThing`/`ThingManager` extraction trials lost exact matches and were reverted. Split further only in evidenced dependent groups; address adjacency or a cleaner basename alone is insufficient. |

Of the 103 non-component text neighbors in the terrain span, 39 are editor
callbacks/functions with their own descriptor evidence. The other 64 are
predominantly terrain-named setup, collision, scan, skin, and platform code;
`RenderQuads` and a few generic helpers remain provisional rather than
assigned to the terrain TU by adjacency alone.
A combined `SkinPlatform`/`PlatformConnect`/`SkinPlatformSize`/`SkinFlipTab`
move built but lowered `SkinPlatform` 34.00277→33.96122 and whole fuzzy
46.065376→46.065290; it was reverted pending wider TU consolidation.
Moving the adjacent `InsideLineF`/`InsidePolLines` pair also left their own body
scores unchanged but lowered whole fuzzy 46.065680→46.064198 through other
terrain bodies; that trial was reverted.

The numerical-solver locals `Newton_Raphson` and
`Laguerre_With_Deflation` belong to the original Vorbis LPC run near
`0x513340`, not `numaths.c`. Removing their duplicate forced-emission stubs
from `numaths.c` lets the already-linked real Vorbis implementations match
at 99.99% and 99.98%.

## Remaining reconstruction plan

1. Extend the complete symbol ledger into a confidence-ranked original TU
   map: text runs, local data/rodata/BSS xrefs, embedded paths, constructor
   order, and GOT/relocation references. Record ambiguity rather than assign
   every symbol from address adjacency alone.
2. Calibrate each supported owner's compiler mode and optimization against
   exact neighbors (the portal helpers require the default mode; forced
   `-O2`/`-O3` regressed). The current terrain/gameliball and grass owner
   compile at `-O3`, while `edtoolsall_plain.cpp` is `-O2`; this is a
   migration risk, not proof of separate original TUs. Move whole dependency
   groups with real public/internal headers.
3. Review each moved unit for symbol binding/ABI, data and relocation effects,
   header ownership, and a sensible source boundary. Clean up catch-all and
   obsolete files; avoid arbitrary one-function or functionless files.
4. After each move, compare whole and per-function scores, exact transitions,
   symbol coverage, target/native/WASM builds, and the available gameplay smoke. Commit and
   push coherent tested batches at roughly hourly intervals.
5. First classify the 103 text neighbors and source dependencies around the
   unresolved terrain LOCAL-state component; then move its full
   91-function/48-object closure as one uncommitted cut, provisionally into
   the existing `gameliball.cpp` initializer owner. Extract only the evidenced
   group: `terrain_stubs.cpp` also contains earlier debris and later AI/particle
   functions from other original address runs, so merging the whole file would
   create a false TU. Exclude interleaved editor callbacks unless their own
   xrefs prove shared ownership. Then revisit editor
   callbacks, the Panel co-location regression, and catch-all/resource owners;
   do not invent unused pools or treat a constructor-delimited block as a TU.

The current `matching.json` lists eight units with no reported functions and
33 with one. This is a review queue, not a deletion rule: some are legitimate
data owners (`animation.cpp`, `contexts.cpp`) or initializer-only units
(`legoai.cpp`). Even include-only `grabber.cpp`, `giztorpedo.cpp`, and
`tightrope.cpp` emit original-named 93-byte constructors; textual function
counts miss these. Remove or merge a tiny file only after checking its actual
object symbols and the original TU evidence. Move stray source-local
prototypes into real headers as their owning families are reconstructed.

For each sensible move, compare function and whole scores, exact-match
transitions, symbol coverage, and neighboring bodies. Run target, native,
WASM, repository checks, and the 120-frame Map smoke at appropriate
intervals. The current fixture lists Map but no Cantina gameplay destination;
`--area Cantina` exits before simulation. The original tiny-movement `MovePlayer` sanitizer flake is
documented by its prior history and is intentionally preserved for
matching. No assembly, calling-convention or visibility attributes for
score, fake initializers, dummy symbols, or source-local linker-only
`extern` bridges.
