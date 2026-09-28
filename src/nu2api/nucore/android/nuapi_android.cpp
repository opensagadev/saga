#include "decomp.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nu3d/android/nurenderthread.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nusound/nusound.h"

extern "C" void NuInitDebrisRenderer(VARIPTR *buffer, VARIPTR buffer_end);
extern "C" void NuIOSMtlInit(void);

extern "C" NUPADREC *PadRecPtr(void) {
    return NULL;
}

void NuXboxLiveInit(void) {
}

void InitializeGLMutex(void) {
}

i32 NuInitHardwarePS(VARIPTR *buf, VARIPTR *buf_end, i32 heap_size) {
    NuIOSThreadInit();

    NuIOS_IsLowEndDevice();

    g_vaoLifetimeMutex = NuThreadCreateCriticalSection();
    g_texAnimCriticalSection = NuThreadCreateCriticalSection();
    InitializeGLMutex();

    NuPad_Interface_InputManagerInitialise();

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0xf9);
    NuIOSMtlInit();
    NuInitDebrisRenderer(buf, *buf_end);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0xfe);

    NuRenderThreadCreate();

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0x103);
    NuShaderManagerInit(buf, *buf_end);
    NuRenderContextInit();
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0x108);

    nurndr_pixel_width = g_backingWidth;
    nurndr_pixel_height = g_backingHeight;

    NuSound3InitV(buf, *buf_end, 0, 0);

    return 0;
}

i32 NuInitHardwareParseArgsPS(i32 setup_tok, char **args) {
    return 0;
}

void NudxFw_D3DBeginCriticalSection() {
    // This is undoubtedly a __FILE__ and __LINE__ usage in the original.
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0xd3);
}

void NudxFw_D3DEndCriticalSection() {
    // This is undoubtedly a __FILE__ and __LINE__ usage in the original.
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nucore/android/nuapi_android.c", 0xd7);
}

i32 NuPs2ApplyDeadZone(i32 raw_value, i32 dead_zone) {
    i32 value = raw_value - 128;
    if (value > 0) {
        if (value < dead_zone) {
            value = 0;
        } else {
            value = (value - dead_zone) * 255 / (255 - dead_zone);
        }
    } else if (value > -dead_zone) {
        value = 0;
    } else {
        value = (value + dead_zone) * 255 / (255 - dead_zone);
    }
    return value;
}

static char g_smbPath[256];

void Nu360ConfigureSMBSharing(char **path) {
    NuFileSetCurrentDirectory("d:\\");
    if (static_cast<bool>(NuFileLoadBuffer("smbpath.txt", g_smbPath, sizeof(g_smbPath)))) {
        NuFileSetCurrentDirectory(g_smbPath);
        *path = g_smbPath;
    }
}
