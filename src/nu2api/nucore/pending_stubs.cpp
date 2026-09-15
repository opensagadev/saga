// Pending-transcription stand-ins (original exports without decompiled
// bodies yet). Collected here so each is an explicit, greppable TODO;
// they previously lived as anonymous extern-C stubs that shadowed real
// transcriptions elsewhere.

#include "decomp.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/ShaderManagerOpenGL.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/nurand.h"

extern "C" void NuShaderManagerForceShader(void) {
    STUBBED();
}

extern "C" void *NuShaderManagerGetInstance(void) {
    return g_shaderManager;
}

extern "C" f32 NuShaderManagerGetShininessFactor(void) {
    return ShaderManagerTemplate<NuShaderObject>::shininessFactor;
}

extern "C" void NuShaderManagerLoadCompiledShaders(void) {
    STUBBED();
}

extern "C" void NuShaderManagerSetShininessFactor(f32 shininess) {
    ShaderManagerTemplate<NuShaderObject>::shininessFactor = shininess;
}

extern "C" void NuShaderObjectKeyGenerate2(void) {
    STUBBED();
}

extern "C" void NuShaderObjectKeyGenerate4(void) {
    STUBBED();
}

extern "C" void NuShaderObjectKeySetUberShaderHash(void) {
    STUBBED();
}
