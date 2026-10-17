#include "nu2api/nu3d/nuhspecial.h"
#include <stdio.h>

#include <float.h>
#include <string.h>
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/numath/nuvec.h"

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nu3d_includes.h"

extern "C" {
    // Scene currently being built.  Size is 0x218 bytes; original lives in BSS
    // and is referenced as a plain object by all render/present code.
extern struct nurenderscene_s currentScene;
} // extern "C"

// Swap/present pacing flags (original BSS).
volatile bool g_isBlockedInSwapScreen = false;

extern "C" {
    void NuDynamicLightAddRenderScene(struct NuDynamicLight *, i32, i32);
}

// Scene / GScn
using NUGSCNVIDEOMEMFN = void (*)(NUGSCN *);

extern NUGSCNVIDEOMEMFN gscene_to_video_mem;
extern NUGSCNVIDEOMEMFN video_mem_to_gscene;
extern "C" void NuGScnFromVideoMem(NUGSCNVIDEOMEMFN callback);
class NuDynamicLight;
extern "C" {
    extern nurenderscene_s currentScene;

#if defined(__i386__)
#define NUDISPLAYSCENERNDR_BODY_ATTR __attribute__((noinline, noclone, regparm(0)))
#else
#define NUDISPLAYSCENERNDR_BODY_ATTR __attribute__((noinline, noclone))
#endif
    static NUDISPLAYSCENERNDR_BODY_ATTR void NuDisplaySceneRndrBody(NUDLDLISTSCENE *scene)
        __asm__("NuDisplaySceneRndr.part.101");

static void NuDisplaySceneRndrBody(NUDLDLISTSCENE *scene);
}

extern "C" void NuGScnToVideoMem(NUGSCNVIDEOMEMFN callback);

extern "C" void NuMtlAnimateSetMask(i32 mask);
extern "C" void NuMtlAnimateSetSpeedScale(f32 speed_scale);
static void NuMtlCreate3D(void) {
    STUBBED();
}

static void NuMtlSetRenderStatesPS(void) {
    STUBBED();
}

// Debug / visualisation geometry
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimUV(f32 u, f32 v);
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimSetColour(i32 colour);
static inline void NuRndrPrimUV(f32 u, f32 v);
extern "C" __attribute__((force_align_arg_pointer)) i32 NuRndrCircle(f32 x, f32 y, f32 radius, f32 aspect, i32 count, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                            numtl_s *material);

extern "C" void NuRndrGlobalFrameCountPause(i32 paused);

// NuRndrRect2di and NuRndrGradRect2di write the attributes for the current
// vertex before handing its position to NuPrim2DAddXYZ.  The latter advances
// the stream cursor, so doing this in one helper preserves the ordering of
// the original immediate-mode implementation.
static inline u32 NuRndrPrimColour(u32 colour) {
    if (g_NuPrim_NeedsOverbrightening != 0) {
        return colour;
    }
    return (colour & 0xff000000u) | ((colour >> 1) & 0x007f7f7fu);
}

// Shader / texture / vertex state
extern "C" {
    void *g_boundCameraPacket;
}
extern "C" void NuShaderGetDirtyMask(NUSHADERUSAGEMASK *mask, NUSHADEROBJECT *shader) {
    memmove(mask, shader->usage_mask, sizeof(*mask));
    const i32 frame = NuRndrGlobalFrameCount();
    if (shader->last_uniform_frame != frame) {
        shader->last_uniform_frame = frame;
        return;
    }
    void *light_packet = *g_packetToShaderStateMappings[0].packet;
    if (shader->last_light_packet == light_packet) {
        mask->semantics[0] &= ~g_packetToShaderStateMappings[0].mask.semantics[0];
        mask->semantics[1] &= ~g_packetToShaderStateMappings[0].mask.semantics[1];
        mask->semantics[2] &= ~g_packetToShaderStateMappings[0].mask.semantics[2];
        mask->semantics[3] &= ~g_packetToShaderStateMappings[0].mask.semantics[3];
    } else {
        shader->last_light_packet = light_packet;
    }
    void *camera_packet = *g_packetToShaderStateMappings[1].packet;
    if (shader->last_camera_packet == camera_packet) {
        mask->semantics[0] &= ~g_packetToShaderStateMappings[1].mask.semantics[0];
        mask->semantics[1] &= ~g_packetToShaderStateMappings[1].mask.semantics[1];
        mask->semantics[2] &= ~g_packetToShaderStateMappings[1].mask.semantics[2];
        mask->semantics[3] &= ~g_packetToShaderStateMappings[1].mask.semantics[3];
    } else {
        shader->last_camera_packet = camera_packet;
    }
}
