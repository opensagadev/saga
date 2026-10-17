// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.
// This is the first unit in .text (0xe3110); it has no constructor, so the
// reference keeps no trace of its file name.

// ==== src/nu2api/nuandroid/ios_graphics.cpp: declarations ====
#include "nu2api/nuandroid/ios_graphics.h"
#include "java/android.h"

#include <GLES2/gl2.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "decomp.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nucore/nuonline.h"
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/nuqfnt.h"
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
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/numath/numtx.h"
#include "globals.h"

// ---------------------------------------------------------------------------
// ios_graphics — iOS/Android GLES2 platform glue
//
// Original iOS path managed its own EGL / framebuffer and a dedicated render
// thread.  On Android the same code is reused: the "iOS" name is historical.
// This file contains platform plumbing, thread handoff, frame pacing, and
// path helpers.
// ---------------------------------------------------------------------------

// Backing drawable size driven by NuRenderDevice::InitialiseOpenGLContext.

// Nu-side DX-style clear flags, translated to GL masks below.


// ---------------------------------------------------------------------------
// Framebuffer setup
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Diagnostics / caps
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Filesystem paths; the Android documents selection is cached after first use.
// ---------------------------------------------------------------------------

char g_internalPath[256];

// Native diagnostics supply a strong host-configured documents-path override.

i32 g_bgloadAsFastAsPossibleHack;
extern "C" i32 NuIOS_IsMidRangeDevice(void);

// ---------------------------------------------------------------------------
// Render-thread handoff
//
// Game thread  ──Wakes──► render thread (via g_wakeRenderThread)
// Render thread ──Signals──► game thread "allowed to render" observability
// Render thread ──Signals──► game thread "frame done" (g_renderThreadDoneThread)
//
// Three independent mutex/cond pairs mirror the original iOS implementation.
// The split keeps the "awaiting wake" debug flag independent from the actual
// wake/completion signals.
// ---------------------------------------------------------------------------

static pthread_mutex_t g_wakeRenderMutex;
static pthread_cond_t g_wakeRenderCondition;
static pthread_mutex_t g_renderThreadDoneThreadMutex;
static pthread_cond_t g_renderThreadDoneThreadCondition;
static pthread_mutex_t g_awaitingRenderWakeMutex;
static pthread_cond_t g_awaitingRenderWakeCondition;
static i32 g_awaitingRenderWake;
static i32 g_wakeRenderThread;
static i32 g_renderThreadDoneThread;

// Timestamp when the current frame's render was kicked — original
// @0x66c9a8, set in NuIOS_WakeRenderThread and sampled in
// NuIOS_WaitForRenderThreadCompletion for 60 Hz pacing.
i64 g_renderStartTime;

extern "C" i64 getCurrentTime(void);



// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.



// C++-linkage helpers defined in sibling TUs.

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

    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");


    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {

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

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

    // Locale string is owned by the Android/JNI TU; cache only its index here.
    i32 g_languageIndex = -1; // original .data @0x616b80

i32 NuIOS_IsMidRangeDevice(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

#include "nu2api/nucore/nucore_plain_types.h"
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }

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
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== functions in original address order ====

i32 NuIOS_ShouldUseMSAA(void) {
    return 0;
}

void NuIOS_InitRenderThread() {
    pthread_mutex_init(&g_wakeRenderMutex, nullptr);
    pthread_cond_init(&g_wakeRenderCondition, nullptr);
    pthread_mutex_init(&g_renderThreadDoneThreadMutex, nullptr);
    pthread_cond_init(&g_renderThreadDoneThreadCondition, nullptr);
    pthread_mutex_init(&g_awaitingRenderWakeMutex, nullptr);
    pthread_cond_init(&g_awaitingRenderWakeCondition, nullptr);
}

extern "C" {
    void NuIOS_DeallocateSystemRenderbuffer(GLuint) {
        // Retail no-op.
    }

}

i32 NuIOS_IsLowEndDevice(void) {
    return g_isLowEndDevice;
}

extern "C" {
    i32 NuIOS_IsLowestEndDevice(void) {
        return g_isLowestEndDevice;
    }

    i32 NuIOS_IsMidRangeDevice(void) {
        return 0;
    }

}

void NuIOS_SetRenderComplete(void) {
    pthread_mutex_lock(&g_renderThreadDoneThreadMutex);
    if (g_renderThreadDoneThread == 0) {
        g_renderThreadDoneThread = 1;
        pthread_cond_signal(&g_renderThreadDoneThreadCondition);
    }
    pthread_mutex_unlock(&g_renderThreadDoneThreadMutex);
}

void NuIOS_SetRenderIncomplete(void) {
    pthread_mutex_lock(&g_renderThreadDoneThreadMutex);
    g_renderThreadDoneThread = 0;
    pthread_mutex_unlock(&g_renderThreadDoneThreadMutex);
}

// Render thread: block until the game thread wakes us for the next frame.
void NuIOS_WaitUntilAllowedToRender(void) {
    // Observability: tell the game thread we are now parked and awaiting
    // the wake signal (used by debug overlays / asserts on iOS).
    pthread_mutex_lock(&g_awaitingRenderWakeMutex);
    g_awaitingRenderWake = 1;
    pthread_cond_signal(&g_awaitingRenderWakeCondition);
    pthread_mutex_unlock(&g_awaitingRenderWakeMutex);

    pthread_mutex_lock(&g_wakeRenderMutex);
    while (g_wakeRenderThread == 0) {
        pthread_cond_wait(&g_wakeRenderCondition, &g_wakeRenderMutex);
    }
    g_wakeRenderThread = 0;

    pthread_mutex_lock(&g_awaitingRenderWakeMutex);
    g_awaitingRenderWake = 0;
    pthread_mutex_unlock(&g_awaitingRenderWakeMutex);
    pthread_mutex_unlock(&g_wakeRenderMutex);
}

extern "C" {
    i32 NuIOS_IsSmallScreen(void) {
        NuScreen *screen = NuScreen::Get();
        return screen->GetWidth() * screen->GetHeight() < 10000.0f;
    }

    i32 NuIOS_HardwareSupportsRetina(void) {
        return 1;
    }

}

u32 NuIOS_YieldThread(void) {
    static u32 count;

    u32 frequency;
    if (NuIOS_IsLowEndDevice()) {
        if (g_bgloadAsFastAsPossibleHack == 0) {
            return sched_yield();
        }
        frequency = 8;
    } else if (NuIOS_IsMidRangeDevice()) {
        frequency = g_bgloadAsFastAsPossibleHack != 0 ? 8 : 2;
    } else {
        frequency = g_bgloadAsFastAsPossibleHack != 0 ? 4 : 2;
    }

    if (__builtin_expect(++count % frequency == 0, 1)) {
        return sched_yield();
    }
    return 0;
}

extern "C" {
// Original 0xe3450 adds raw nanoseconds to seconds scaled by 1000.
extern "C" i64 getCurrentTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (i64)ts.tv_sec * 1000 + (i64)ts.tv_nsec;
}

}

// original 0xe34b0 — pace the remainder of the 16 ms frame budget since
// g_renderStartTime was stamped in NuIOS_WakeRenderThread, then join on the
// render-thread completion flag.  Sleeps < 2 ms are skipped to avoid
// nanosleep overhead dominating the frame.
void NuIOS_WaitForRenderThreadCompletion(void) {
    constexpr i64 kFrameBudgetMs = 16;
    constexpr i64 kMinSleepMs = 2;

    i64 elapsed_ms = getCurrentTime();
    elapsed_ms -= g_renderStartTime;
    i64 remaining_ms = kFrameBudgetMs;
    remaining_ms -= elapsed_ms;

    if (remaining_ms >= 0) {
        if (remaining_ms > kFrameBudgetMs) {
            remaining_ms = kFrameBudgetMs;
        }
        if (remaining_ms >= kMinSleepMs) {
            struct timespec sleep_time = {};
            sleep_time.tv_nsec = remaining_ms * 1000000L;
            nanosleep(&sleep_time, nullptr);
        }
    }

    pthread_mutex_lock(&g_renderThreadDoneThreadMutex);
    while (g_renderThreadDoneThread == 0) {
        pthread_cond_wait(&g_renderThreadDoneThreadCondition, &g_renderThreadDoneThreadMutex);
    }
    g_renderThreadDoneThread = 0;
    pthread_mutex_unlock(&g_renderThreadDoneThreadMutex);
}

// original 0xe3590 — kick the render thread for the next frame.
void NuIOS_WakeRenderThread(void) {
    g_renderStartTime = getCurrentTime();

    pthread_mutex_lock(&g_wakeRenderMutex);
    if (g_wakeRenderThread == 0) {
        g_wakeRenderThread = 1;
        pthread_cond_signal(&g_wakeRenderCondition);
    }
    pthread_mutex_unlock(&g_wakeRenderMutex);
}

char *NuIOS_GetAppBundlePath(void) {
    static char storedBundlePath[4096];

    if (storedBundlePath[0] == '\0') {
        strcpy(storedBundlePath, "dummyPath");
    }
    return storedBundlePath;
}

extern "C" {
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

}

// Native diagnostics supply a strong host-configured documents-path override.
SAGA_HOST_WEAK char *NuIOS_GetDocumentsPath(void) {
    if (g_internalPath[0] == '\0') {
        FILE *marker = fopen("mnt/sdcard/TTGames/com.ttfusion.legosaga/save.here", "rb");
        if (marker != NULL) {
            strcpy(g_internalPath, "mnt/sdcard/TTGames/com.ttfusion.legosaga/");
            fclose(marker);
        } else {
            strcpy(g_internalPath, g_internalDataPath);
            strcat(g_internalPath, "/");
        }
    }
    return g_internalPath;
}

extern "C" {
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

}
