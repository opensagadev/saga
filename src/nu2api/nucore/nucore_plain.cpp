// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"
extern "C" {
}
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.




extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraRelock(void);
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------


void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
    void DisplayListDestroyFxItemPS(void *item);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");
static void DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                           NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                           void **transform_packet, NUMTX *skin_matrices,
                                           DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster);

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
static void DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                                  NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                                  void **transform_packet, f32 alpha);
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);

    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

    void NuIOS_AwardAchievement(void) {
        // Retail no-op.
    }
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
    void NuIOS_ShowAchievements(void) {
        // Retail no-op.
    }

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------


    static void *dirnames[16];
    static i32 numdirs;

    i32 NuSysDirClose(i32 index) {
        NuMemoryGet()->GetThreadMem()->BlockFree(dirnames[index], 0);
        dirnames[index] = NULL;
        --numdirs;
        return 0;
    }
    i32 NuSysDirOpen(void) {
        // Retail returns zero.
        return 0;
    }
    i32 NuSysDirRead(void) {
        // Retail returns zero.
        return 0;
    }

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

void NuQFntDestroy(VUFNT *font);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
    i32 NuLgtArcLaserOldCnt;


void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjEvalAnim(void);
extern i32 ddmaxjoints;

#include "nu2api/nucore/nucore_plain_types.h"
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
    u32 NuWindRand(void);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
#undef NUGCUT_CURVE_VALUE
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------


i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
static void ClipInstTree(NuInstanceTree_s *tree, i32 tree_index);
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerRenderStats(void);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

