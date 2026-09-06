# Animation runtime inventory

This inventory describes functional coverage, not instruction-match quality. It
was audited against `res/libTTapp.so` on 2026-09-06. A function is only called
complete when its data layout and behavior have original-binary evidence; a
symbol-shaped empty body is not counted as an implementation.

## Active character runtime

| area | principal functions | status |
|---|---|---|
| packet state | `ResetAnimPacket`, `ResetMiniAnimPacket`, `AnimPacket_FullToMini`, `AnimPacket_MiniToFull` | Implemented. Mini-packet offsets and reset assignments were corrected against the original instructions. |
| action selection | `Animate_CHARACTER`, `Animate_JEDI`, `Animate_DEFAULT`, `Animate_BARMAN`, `Animate_CANNON`, `Animate_WALKER`, `Animate_HOVERDROID`, `Animate_SPEEDERBIKE`, `Animate_REPUBLICGUNSHIP` | Implemented or audited. The small mode handlers were transcribed from their original symbols. |
| time and transitions | `UpdateAnimPacket`, `UpdateMiniAnimPacket`, `SetAnimFrame`, `PlayAnim`, `AnimEndFrame`, `AnimStopFrame`, `AnimSpeed`, `AnimMiscFlags`, `AnimBlendingFromTo` | Implemented. The apparently redundant source-reverse assignment in `UpdateAnimPacket` also exists in the original and is intentionally retained. |
| ANI4 scalar sampling | `ANI_SimpleAni3PlayerV4Joint`, `ANI_SimpleAni3PlayerV4Joint_Blend` | Implemented for the original packed type-6/constant path. |
| ANI4 quaternion sampling | `ANI_SimpleAni3PlayerV4Joint_Quat3`, `ANI_SimpleAni3PlayerV4Joint_Quat3W` and both blend variants | Implemented. Supports type 6, 7, 9 and constant curves, XYZ-to-W reconstruction, XYZW storage, harmonization, normalization and root translation. |
| mixed rotations | `ANI_SimpleAni3PlayerV4Joint_EulerQuat` and blend variant | Implemented. Euler clips are converted with the original radians-to-NUANG factor before quaternion blending. |
| quaternion interpolation | `VuQuatSlerpFast` | Implemented from original `0x286a8b`, including shortest-path selection, the `0.85` nlerp threshold and the original acos/cos polynomial constants. |
| pose accumulation | `NuAnimBuffAccumulate_3`, `NuHGobjEvalAnim2Root_3`, `NuHGobjEvalAnimBlend2Root_3` | Implemented. Mixed Euler/quaternion blends now restore the original `NuAnimPushSetUseQuatsFlag`/pop guard. |
| hierarchy evaluation | `NuAnimBuffEvaluate_3`, `NuAnimBuffEvaluate_3_QuatB`, callbacks and procedural overrides | Functional for ANI4/ANI5. Quaternion construction is shared by the evaluator; the legacy QuatB ABI entry routes to that path. |

## Animation data and scene instances

| area | principal functions | status |
|---|---|---|
| ANI4 fixup/load | `ANI_FixUpAddrs`, `NuAnimData2Fixup`, `NuAnimData2LoadBuffEx`, `NuAnimData2LoadBuffFromPAK`, `NuAnimGetAnimLOD`, `NuAnimNumNodes`, `NuAnimEndFrame` | Implemented for the formats used by the current runtime. |
| curve extraction | `ANI_Ani3ExtractAllNodeCurves`, `NuAnimCurveExtractAllNodeCurves_3`, `CalcValue1648`, `CalcValue1648Get2Values` | Implemented. Integer and scalar curve consumers remain separate where required. |
| scene instance animation | `ReadInstAnimBlock`, `ReadInstAnimBlockDlist`, `EvalAnim`, `NuSpecialSetInstAnim`, `NuSpecialSetInstAnimTime` | Load/evaluation path implemented; see remaining compatibility entries below for unused test/editor helpers. |
| character redirects | `RedirectAnim`, `NeedsPretendAnim`, `FindAnimIX`, `GetAnimTimeRandom` | Implemented. |
| animated game objects | `GameAnimSet_Play`, stop/reset/jump/position/state helpers, `GameAnimSet_Draw`, reflection draw | Implemented. `GameAnimSet_Draw` is the exact visible-object loop at original `0x4a9900`. |

## Remaining runtime gaps

These are real original functions, but they are not on the currently assigned
generic/Jedi character path or are subsystems that are not yet reconstructed.
They must not be filled with guessed default behavior.

| group | remaining symbols | impact / dependency |
|---|---|---|
| specialized character selection | `Animate_POD`, `Animate_ATAT`, `Animate_BEAST`, `Animate_WEIRDO`, `Animate_CRITTER`, `Animate_VEHICLE`, `Animate_DROIDEKA`, `Animate_PROTOCOL`, `Animate_ASTROMECH`, `Animate_GEONOSIAN`, `Animate_BATTLEDROID`, `Animate_SUPERBATTLEDROID` | All are substantial original routines. They are presently not assigned because `ExtraCharacterFixUpAfterConfig` only restores the Jedi override and otherwise `CharacterPostLoad` installs `Animate_CHARACTER`. Reconstruct the dispatch assignments and each mode together. |
| legacy hierarchy animation | `NuHGobjEvalAnim`, legacy branch of `NuHGobjEvalAnim2Root`, `NuHGobjEvalAnimBlend` | ANI1/ANI2-style curve evaluation remains pending. ANI4/ANI5 uses the implemented `*2Root_3` path. |
| cutscene animation copies | `NewCopyAnims`, local `copyAnims` | No current source caller; needed when the remaining cutscene clone/merge workflow is restored. |
| progress persistence | `GameAnimSys_StoreProgress`, `GameAnimSys_GetProgressData`, `GameAnimSys_ReStoreProgress`, `GameAnimSys_AllocateLevelProgressData` | Save/restore for animated scene objects is still empty. |
| secondary animated systems | custom piece animation helpers, traffic animation system, customiser animation-load selection | Separate incomplete subsystems, not skeletal sampling/evaluation. |

## Editor and compatibility surface

The empty `NuAnimCurve*` creation/destruction/apply functions in
`nucore_plain.cpp`, `NuAnimDataChunkCreate`/destroy, `NuSpecialTestAnim`, and
edanim sound placement are editor or legacy-authoring APIs. They remain explicit
stubs until a caller and the original ownership/lifetime contract are recovered.

`NuQuatSlerpFast` (without the `Vu` prefix) is a separate original math routine
and is not referenced by the current source. Its present normalize-after-lerp
implementation is functional but not an instruction-faithful reconstruction;
the active animation blend path uses the reconstructed `VuQuatSlerpFast`.

## Verification checklist

For changes to the active path, run:

```bash
bazel build --config=target //src:saga_target
bazel build --config=native //src:saga_native
bazel test //scripts/checks:checks
git diff --check
```

Useful original symbols for focused comparison:

```text
0x286a8b VuQuatSlerpFast
0x2bdd60 ANI_SimpleAni3PlayerV4Joint_Quat3
0x2bf270 ANI_SimpleAni3PlayerV4Joint_Quat3W
0x2c0680 ANI_SimpleAni3PlayerV4Joint_EulerQuat
0x2c2020 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3
0x2c3450 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W
0x2c4850 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat
0x2bd180 NuAnimBuffEvaluate_3
0x2bc8f0 NuAnimBuffEvaluate_3_QuatB
0x2ce760 NuHGobjEvalAnimBlend2Root_3
```
