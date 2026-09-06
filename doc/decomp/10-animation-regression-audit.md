# Animation regression audit

Date: 2026-09-05. Baseline: `52c4aa7`. Upstream integrated: `9e0c691`.

The baseline was rebuilt in an isolated checkout with NDK r8e GCC 4.7. Its regenerated fuzzy score reproduces 16.205053%. The audited build scores 17.535992%; Move_JEDI scores 63.431034%.

## Fixes and platform review

- Fixed missing id_JAWA definition/registration and incorrect C++ linkage for the original C GetSfxId export. Final target has no new undefined symbols.
- Restored texture interpreter, parser callbacks and state to nutexanim.cpp, preserving its O3 setting and original static linkage. Removed callback used attributes; most simple callbacks now exceed 99.8%.
- Corrected the blowup predicate to the original integer return ABI (100% match). Verified O2 settings for TouchHacks and touch-task constructors using isolated target builds.
- Corrected Force push/deflect/throw rumble constants against original instructions and literal data.
- Removed noinline/used and forced stack-alignment attributes from reviewed animation/rendering functions. Their numerical losses are retained rather than reintroducing compiler tricks.
- The PR adds no HOST_BUILD or Emscripten branches in engine code. Keyboard handling is shared under src/host. Removed WASM animation-reset and keyboard overrides. New pointer-width conditionals only guard ABI assertions. Names nucamera_farclip_hack and hackFlashTimer exist in the original ELF.

## Original 48 reported drops

Scores compare with the original ELF. A higher incidental score for an empty stub is not evidence of more correct behavior.

| Symbol | Baseline % | Before audit % | Audited % | Finding |
| --- | ---: | ---: | ---: | --- |
| `_ZN13MechTouchTaskC1ER36MechInputTouchGestureBasedController` | 44.33 | 11.83 | 83.50 | Restored constructor fields and verified O2 compilation. |
| `_ZN13MechTouchTaskC2ER36MechInputTouchGestureBasedController` | 44.33 | 11.83 | 83.50 | Restored constructor fields and verified O2 compilation. |
| `_Z17MenuUpdateNewGameP6MENU_s` | 69.19 | 67.29 | 67.29 | Removed volatile alias; equivalent low-three-bit update. Baseline match 97.79%. |
| `_ZN10TouchHacks18CanBlowupBeBlownUpER13GIZMOBLOWUP_si` | 22.83 | 0.00 | 100.00 | Corrected integer return ABI and verified O2 compilation. |
| `_Z20SpecialMove_GetFlagsij` | 4.00 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z17SpecialMove_CheckP12GameObject_sS0_` | 3.78 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z13ComboHitFrameP12GameObject_si` | 8.80 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z10CannotKillP12GameObject_s` | 21.00 | 19.35 | 19.35 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_GLOBAL__sub_I_charconfig.cpp` | 29.41 | 0.00 | 0.00 | Compiler-generated initialization changes with definitions/optimization; not a gameplay routine. |
| `_GLOBAL__sub_I_gameanim.cpp` | 2.35 | 0.00 | 0.00 | Compiler-generated initialization changes with definitions/optimization; not a gameplay routine. |
| `_ZL12JumpAnimCodeP12GameObject_s` | 25.03 | 20.28 | 20.28 | Restored original airborne timer and fall transition; checked original control flow. |
| `AnimPlaying` | 10.00 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z14MoveGameCameraP12GAMECAMERA_s` | 1.47 | 0.00 | 0.00 | Body unchanged; restored TU optimization changes emitted code. |
| `_Z9SlideCodeP12GameObject_s` | 4.31 | 0.00 | 0.00 | Empty stub remains empty; restored TU optimization changes incidental score. |
| `_ZL16Lever_UpdateHintP6HINT_s` | 19.33 | 17.78 | 17.78 | Still returns false; restored TU optimization changes incidental score. |
| `_Z12SnapPosTakenP11WORLDINFO_sP11pushblock_sP7nuvec_si` | 7.58 | 3.85 | 3.85 | Empty stub remains empty; restored TU optimization changes incidental score. |
| `_Z7NewGamev` | 94.09 | 90.46 | 90.46 | Removed volatile byte-array alias; same low-three-bit clear on canonical u32. Baseline match 95.23%. |
| `_Z14GizForce_ThrowP12GameObject_sP10GIZFORCE_sffi` | 3.93 | 0.00 | 0.00 | Corrected pointer return ABI; remains a placeholder. |
| `_ZL27GizObstacleUpdate_ProximityP13GIZOBSTACLE_s` | 11.08 | 1.24 | 1.24 | Body unchanged; restored exclusion/average-position callees change inlining. |
| `_Z19GizmoBlowUpOpponentP12GameObject_sfffijjj` | 1.45 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z15GizmoBlowUp_HitP12GameObject_sP7nuvec_sifS2_S2_P6BOLT_sjPh` | 1.14 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z22GizmoBlowupsFinalSetupP11WORLDINFO_s` | 86.69 | 84.91 | 84.91 | Body unchanged; baseline match 98.01%, relocation differences. |
| `_Z9ZapTargetP12GameObject_s` | 3.33 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z20InitGameObjectLightsv` | 7.36 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z19GameAISysStartFrameP7AISYS_s` | 40.30 | 37.31 | 37.31 | Area mask split into original two u32 fields; equivalent bit operations. Baseline match 96.23%. |
| `NuPortalVisibility` | 25.83 | 0.00 | 0.00 | Body and size unchanged; direct baseline match 99.27%. Relocation changes disrupt alignment against the original. |
| `_Z16DrawGameMessagesv` | 51.91 | 42.27 | 42.27 | Original easing, clamp, interpolation, alpha and dispatch checked; fixes incorrect old flags/formulas. |
| `rtlDynamicAlloc` | 2.70 | 0.00 | 0.00 | Replaced empty placeholder with original-derived behavior and corrected ABI fields. |
| `_Z11ScanTerrainiii` | 17.74 | 2.89 | 2.89 | Static scaling retained in shared helper; added platform and wall-spline paths. Instruction matching remains low. |
| `DebrisReScale` | 91.74 | 90.47 | 90.47 | Body unchanged; removing volatile SURFACEBITS_DUST changes loads. Baseline match 98.68%. |
| `NuTexAnimSetSignals` | 66.33 | 11.67 | 66.33 | Restored original static state and source ownership. |
| `_ZL7pftaRetP8nufpar_s` | 30.00 | 0.00 | 99.82 | Restored static parser state and original source/optimization ownership. |
| `_ZL10pftaRependP8nufpar_s` | 30.00 | 0.00 | 99.82 | Restored static parser state and original source/optimization ownership. |
| `_ZL7pftaEndP8nufpar_s` | 30.00 | 0.00 | 99.82 | Restored static parser state and original source/optimization ownership. |
| `_ZL10pftaRepeatP8nufpar_s` | 7.10 | 0.00 | 75.68 | Restored static parser state and original source/optimization ownership. |
| `_ZL7pftaOffP8nufpar_s` | 11.58 | 0.00 | 99.89 | Restored static parser state and original source/optimization ownership. |
| `_ZL6pftaOnP8nufpar_s` | 11.58 | 0.00 | 99.89 | Restored static parser state and original source/optimization ownership. |
| `_ZL8pftaRateP8nufpar_s` | 5.64 | 0.00 | 99.90 | Restored static parser state and original source/optimization ownership. |
| `_ZL8pftaWaitP8nufpar_s` | 5.64 | 0.00 | 99.90 | Restored static parser state and original source/optimization ownership. |
| `_ZL11pftaTexAdjRP8nufpar_s` | 5.00 | 0.00 | 99.95 | Restored static parser state and original source/optimization ownership. |
| `_ZL10pftaTexAdjP8nufpar_s` | 5.95 | 0.00 | 99.95 | Restored static parser state and original source/optimization ownership. |
| `_ZL8pftaTexRP8nufpar_s` | 10.48 | 0.00 | 99.90 | Restored static parser state and original source/optimization ownership. |
| `_ZL7pftaTexP8nufpar_s` | 10.48 | 0.00 | 99.90 | Restored static parser state and original source/optimization ownership. |
| `_ZL14pftaScriptMaskP8nufpar_s` | 15.71 | 0.00 | 99.86 | Restored static parser state and original source/optimization ownership. |
| `_ZL12pftaUntiltexP8nufpar_s` | 4.15 | 0.00 | 77.45 | Restored static parser state and original source/optimization ownership. |
| `_ZL8pftaBtexP8nufpar_s` | 3.24 | 0.00 | 73.84 | Restored static parser state and original source/optimization ownership. |
| `_ZL12PlayerCamPosP12GameObject_sP7nuvec_sS2_` | 7.16 | 0.00 | absent | Helper optimized into callers under restored TU optimization; standalone symbol absent. |
| `_ZL18DrawLightningBoltsP12GameObject_sS0_i` | 1.30 | 0.00 | absent | Existing empty stub moved to caller TU and optimized away; lightning remains incomplete. |

## Additional score changes

O2 changes incidental scores of still-empty touch-task and TouchHacks methods, plus compiler-generated initialization/destructor code. It does not remove implemented method bodies. Upstream also replaces previously empty shader-filter code.

Attribute removal leaves DrawItem at 52.41% (previously 85.03%), Draw3DObject at 60.91% (69.37%), and Draw3DObjectAlpha at 62.04% (69.42%). Their bodies are unchanged. MoveAnim_Check loses roughly one point after removing noinline. These are real instruction-match losses.

A bounded DrawItem investigation compared the original instructions with isolated GCC 4.7 O3 builds. Changing scale assignment order, addition order and local declaration placement did not improve its score. Reversing rotation assignments gained only 0.08 percentage points in the isolated object; explicit translation field copies scored worse. The original realigns the stack to 16 bytes; the current scalar locals do not require that alignment. No verified original type or source construct was found to explain it. The existing body is retained without forced alignment or compiler-option overrides; recovering that score requires further code-generation research.

## Validation and limits

- Target Android x86 library and WASM bundle build.
- Optimization-map, duplicate-definition and host-boundary checks pass.
- No new undefined target symbols or unresolved character IDs.
- Native verification stopped during dependency discovery: pkg-config and required native packages are unavailable. Native compilation was not verified.
- Symbol inspection finds no missing required global functions. Four standalone local symbols are absent after optimization: PlayerCamPos, DrawParaphernalia, DrawLightningBolts and BlockCode. BlockCode has a constant-propagated clone; the others are inlined or eliminated. No attributes were added to force their presence.
- CI's symbol gate records these four exact local exceptions and the cross-file CharConfig_GetKeywords accessor. The lightning exception explicitly records its unfinished implementation; these exceptions do not change matching scores or the reference symbol surface. All target, native and WASM CI builds passed before this metadata correction.
- Gameplay was not retested. This audit does not certify rendering, attacks or collisions as regression-free. ObjHitObj, GizForce_Throw and DrawLightningBolts remain placeholders; particle/debris behavior and several character paths remain incomplete. The PR is a draft for continued matching and gameplay review.
