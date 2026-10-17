#include "nu2api/nu3d/nuhspecial.h"
#include <stdio.h>
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuvport.h"
// nurndr_plain.cpp — Host "plain" renderer backend.
//
// This TU is the host replacement for the PS2 rendering TU (nurndr).
// It owns renderer scene/present paths and primitive drawing helpers:
//
//   1. Scene lifecycle  — NuRndrBeginScene / NuRndrClear / NuRndrEndScene
//      builds the current `nurenderscene_s` (0x218 bytes in the original BSS)
//      and queues it into a 16-slot ring consumed by the render thread.
//
//   2. Primitive drawing helpers that call the immediate-mode API in
//      android/nuprim_android.c, including quad expansion there.
//
//   3. Frame present   — moved to the original android/nurndr.c TU.
//
// Remaining renderer entry points are retained as link stubs until their
// subsystems are transcribed. Their signatures are not yet recovered, so
// they are left as `void(void)`.

#include <float.h>
#include <string.h>
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nucore.hpp"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/nurndr_plain_types.h"

// ---------------------------------------------------------------------------
// Scene state — mirrors original BSS layout
// ---------------------------------------------------------------------------


extern "C" {
    // Scene currently being built.  Size is 0x218 bytes; original lives in BSS
    // and is referenced as a plain object by all render/present code.
extern struct nurenderscene_s currentScene;

    // Deferred ring: NuRndrEndScene copies the completed scene here; the render
    // thread drains it.  Stride is 0x218, 16 slots (0x2180 bytes total) in the
    // original.
extern i32 sceneParametersCount;
extern struct nurenderscene_s sceneParameters[16];

} // extern "C"

// Swap/present pacing flags (original BSS).
volatile bool g_isBlockedInSwapScreen = false;

// ---------------------------------------------------------------------------
// Immediate-mode 2D stream state
// ---------------------------------------------------------------------------

// Display-list cursor for the 2D stream.  Defined in nudlist.cpp.

// ---------------------------------------------------------------------------
// Forward declarations for C-visible helpers
// ---------------------------------------------------------------------------

extern "C" {
    i32 NuDynamicLightIsEnabled(void *);
    void NuDynamicLightAddRenderScene(struct NuDynamicLight *, i32, i32);
}

// ---------------------------------------------------------------------------
// Immediate-mode 2D API
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Scene lifecycle
// ---------------------------------------------------------------------------







// ---------------------------------------------------------------------------
// Link stubs — retained for compatibility, not yet implemented
// ---------------------------------------------------------------------------
//
// Every function below exists in the original binary.  Signatures have not
// been recovered, so they are kept as void(void) stubs.  Grouped by
// subsystem so it is obvious what is still missing.

// Scene / GScn
using NUGSCNVIDEOMEMFN = void (*)(NUGSCN *);

extern NUGSCNVIDEOMEMFN gscene_to_video_mem;
extern NUGSCNVIDEOMEMFN video_mem_to_gscene;

extern "C" void NuGScnFromVideoMem(NUGSCNVIDEOMEMFN callback);
class NuDynamicLight;
extern "C" {
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);






    extern nurenderscene_s currentScene;
    i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);

    // Retail partially inlines the clipping test into NuDisplaySceneRndr and
    // keeps the body as its stack-argument .part clone.
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

// Material

extern "C" void NuDisplayListAnimateMtls(f32 frame_time);

extern "C" void NuMtlAnimate(f32 frame_time);
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

extern i32 global_frame_count;
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

extern "C" void NuRndrGradRect2di(i32 x, i32 y, i32 w, i32 h, i32 *colour, numtl_s *mtl);


extern "C" i32 NuRndrLine2d(NuLineVertex2D *vertices, NUMTL *material);
extern "C" void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, NUMTL *material);
extern "C" i32 NuRndrLineStrip2d(NuLineVertex2D *vertices, NUMTL *material, i32 count);
extern "C" void NuRndrLineStrip2di(i32 *positions, f32 *uvs, i32 count, i32 colour, NUMTL *material);

extern "C" void NuRndrRect(f32 x, f32 y, f32 z, f32 width, f32 height, f32 u0, f32 v0, f32 u1, f32 v1, i32 colour,
                           NUMTL *material);
extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);
extern "C" void NuRndrRect2di(i32 x, i32 y, i32 w, i32 h, i32 colour, numtl_s *mtl);
extern "C" void NuRndrSetCullDebug(void);
extern "C" {
}


extern "C" {
}



extern "C" void NuRndrGrid(NUVEC *centre, NUVEC *size, i32 columns, i32 rows);

extern "C" void NuRndrSphere(NUVEC *centre, f32 radius, i32 colour, f32 vertical_scale);
extern "C" void NuRndrSphereEx(NUVEC *centre, f32 radius, i32 colour, f32 vertical_scale, i32 angle_step);
extern "C" void NuRndrSphereMtx(NUMTX *matrix, i32 colour, i32 segments, NUMTL *material);
extern "C" void NuRndrSphereTRS(NUVEC *translation, NUANGVEC *rotation, NUVEC *scale, i32 colour, i32 segments,
                                NUMTL *material);

extern "C" void NuRndrTriStrip2di(i32 *positions, f32 *uvs, i32 count, i32 colour, NUMTL *material);

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
