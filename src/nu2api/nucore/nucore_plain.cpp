// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

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

extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

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
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
    GLuint g_colorRenderbuffer;
    GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
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

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

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
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

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
    void NuIOS_CheckCurrentFramebuffer(void) {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            glCheckFramebufferStatus(GL_FRAMEBUFFER);
        }
    }
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
    void NuIOS_DeallocateSystemFramebuffers(void) {
        NuIOS_DeallocateSystemRenderbuffer(g_colorRenderbuffer);
        if (g_earlyColorFramebuffer != 0) {
            glDeleteFramebuffers(1, &g_earlyColorFramebuffer);
            g_earlyColorFramebuffer = 0;
        }
        if (g_earlyColorTexture != 0) {
            glDeleteTextures(1, &g_earlyColorTexture);
            g_earlyColorTexture = 0;
        }
        if (g_defaultFramebuffer != 0) {
            glDeleteFramebuffers(1, &g_defaultFramebuffer);
            g_defaultFramebuffer = 0;
        }
        if (g_colorRenderbuffer != 0) {
            glDeleteRenderbuffers(1, &g_colorRenderbuffer);
            g_colorRenderbuffer = 0;
        }
        if (g_depthRenderbuffer != 0) {
            glDeleteRenderbuffers(1, &g_depthRenderbuffer);
            g_depthRenderbuffer = 0;
        }
        g_currentFramebuffer = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void NuIOS_DeallocateSystemRenderbuffer(GLuint) {
        // Retail no-op.
    }
    // Locale string is owned by the Android/JNI TU; cache only its index here.
    i32 g_languageIndex = -1; // original .data @0x616b80

    // original 0xe3640 — exact locale-matching ladder of the original.
    i32 NuIOS_GetDeviceLanguage(void) {
        if (g_languageIndex == -1) {
            if (strncmp(g_language, "en-us", 5) == 0)
                g_languageIndex = 0x12;
            else if (strncmp(g_language, "en-", 3) == 0)
                g_languageIndex = 1;
            else if (strncmp(g_language, "fr-ca", 5) == 0)
                g_languageIndex = 2;
            else if (strncmp(g_language, "fr-", 3) == 0)
                g_languageIndex = 2;
            else if (strncmp(g_language, "it-", 3) == 0)
                g_languageIndex = 5;
            else if (strncmp(g_language, "de-", 3) == 0)
                g_languageIndex = 4;
            else if (strncmp(g_language, "es-mx", 5) == 0)
                g_languageIndex = 3;
            else if (strncmp(g_language, "es-", 3) == 0)
                g_languageIndex = 3;
            else if (strncmp(g_language, "ja-", 3) == 0)
                g_languageIndex = 0;
            else if (strncmp(g_language, "ko-", 3) == 0)
                g_languageIndex = 0xd;
            else if (strncmp(g_language, "nl-", 3) == 0)
                g_languageIndex = 6;
            else if (strncmp(g_language, "pt-br", 5) == 0)
                g_languageIndex = 0x10;
            else if (strncmp(g_language, "pt-", 3) == 0)
                g_languageIndex = 7;
            else if (strncmp(g_language, "zh-", 3) == 0)
                g_languageIndex = 0x13;
            else if (strncmp(g_language, "hu-", 3) == 0)
                g_languageIndex = 1;
            else if (strncmp(g_language, "ru-", 3) == 0)
                g_languageIndex = 0xc;
            else if (strncmp(g_language, "pl-", 3) == 0)
                g_languageIndex = 0xb;
            else if (strncmp(g_language, "cs-", 3) == 0)
                g_languageIndex = 10;
            else if (strncmp(g_language, "el-", 3) == 0)
                g_languageIndex = 9;
            else if (strncmp(g_language, "da-", 3) == 0)
                g_languageIndex = 8;
            else if (strncmp(g_language, "no-", 3) == 0)
                g_languageIndex = 0xf;
            else if (strncmp(g_language, "sv-", 3) == 0)
                g_languageIndex = 0xe;
            else if (strncmp(g_language, "fi-", 3) == 0)
                g_languageIndex = 0x11;
        }
        return g_languageIndex;
    }
    i32 NuIOS_HardwareSupportsRetina(void) {
        return 1;
    }
    i32 NuIOS_IsLowestEndDevice(void) {
        return g_isLowestEndDevice;
    }
    i32 NuIOS_IsMidRangeDevice(void) {
        return 0;
    }
    i32 NuIOS_IsSmallScreen(void) {
        NuScreen *screen = NuScreen::Get();
        return screen->GetWidth() * screen->GetHeight() < 10000.0f;
    }
    void NuIOS_RecordFlurryEvent(char *event_name) {
        JNIEnv *env = NULL;
        if (g_javaVM->functions->GetEnv(g_javaVM, (void **)&env, JNI_VERSION_1_6) < 0) {
            return;
        }

        jclass activity_class = g_activityClass;
        jstring event = env->functions->NewStringUTF(env, event_name);
        jmethodID method =
            env->functions->GetStaticMethodID(env, activity_class, "FlurryEvent", "(Ljava/lang/String;)V");
        env->CallStaticVoidMethod(activity_class, method, event);
        env->functions->DeleteLocalRef(env, event);
    }
    void NuIOS_ShowAchievements(void) {
        // Retail no-op.
    }

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

    void *NuMemAllocFn(u32 size) {
        return NU_ALLOC(size, 4, 1, "", 0);
    }
    void NuMemFreeFn(void *ptr) {
        NU_FREE(ptr);
    }
    void *NuMemReAllocFn(void *ptr, u32 size) {
        return NuMemoryGet()->GetThreadMem()->_BlockReAlloc(ptr, size, 4, 1, "", 0);
    }

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

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
    i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
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

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
#undef NUGCUT_CURVE_VALUE
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
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
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
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
    void NuRegisterEndFrameCallBackFn(void (*callback)(void)) {
        nuapi_endframe_callbackfn = callback;
    }

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

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);
