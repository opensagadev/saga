# Nucore misc stub audit

The target measurements here use the GOT-aware `objdiff-cli` fork against
`res/libTTapp.so` and the NDK r8e GCC build in `bazel-bin/src/libTTapp.so`.
The source is `src/nu2api/nucore/nu2api_nucore_misc.cpp`.

## Reusable code generation observations

- The Android in-app-purchase methods and `NuMemory::MemErrorHandler` methods
  are genuine empty or zero-return functions in the target. Their target bodies
  are nine bytes: `ret` (or `xor eax, eax; ret`) plus padding NOPs. Removing
  `STUBBED()` from those methods preserves the match. Representative functions
  from each shape measured 100%.
- `NuIOS_GetShaderProgramKey` has a `ShaderObjectKey` return value, even though
  its mangled name names only its input parameter. NDK r8e GCC uses a hidden
  return pointer for this one-word struct on x86 (`ret 4` in the target). A
  `void` declaration cannot reproduce that ABI. The function uses
  `LookupHash(key.key, &local, g_shaderProgramRedirects, 417)` and returns the
  redirected key if found, otherwise the input key.
- `nudeferredshadingenum_e` must be a 32-bit enum. The prior empty dummy struct
  had a different ABI and prevented the target's selector load from matching.
  With the corrected enum and a three-case switch,
  `NuDynamicLightingGetParameterfv` reaches 100%.
- `NuDynamicLightTestShadowExtrusions` ignores its fourth argument and calls
  `NuDynamicLight::testShadowExtrusions` with references to the two vector
  arguments. The direct member call reaches 100%.
- `NuGCutRigidForceInstanced` loops over `scene->rigid_system->count` and ORs
  bit 1 into each rigid's `flags` byte. The natural indexed loop reaches 100%.

## Initial scores after replacing all 28 markers

| Function | Target bytes | GOT-aware match |
| --- | ---: | ---: |
| `NuVpSetDestRect` | 90 | 100% |
| `NuGCutRigidForceInstanced` | 57 | 100% |
| `NuDynamicLightingGetParameterfv` | 100 | 100% |
| `NuDynamicLightTestShadowExtrusions` | 50 | 100% |
| `NuVpSetSourceRect` | 205 | 82.895836% |
| `NuCameraTransformScissorClip` | 292 | 83.252630% |
| `NuIOS_GetShaderProgramKey` | 112 | 53.914288% |
| `NuFrameEndBgLoadPS` | 333 | 0% |

The original `NuHtmlFlush` symbol already has its implementation in
`src/legoapi/misc/supportall.cpp`. The unused local static stub in this file
was removed. The remaining functions need instruction-order and ABI work; the
initial implementations capture their target behavior but are not yet exact.
