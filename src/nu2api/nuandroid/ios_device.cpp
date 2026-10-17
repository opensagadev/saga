// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.
// This is the first unit in .text (0xe3110); it has no constructor, so the
// reference keeps no trace of its file name.

#include "nu2api/nuandroid/ios_graphics.h"
#include "java/android.h"

#include <GLES2/gl2.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "decomp.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "nu2api/nuandroid/ios_device.h"

// ---------------------------------------------------------------------------
// Filesystem paths; the Android documents selection is cached after first use.
// ---------------------------------------------------------------------------

char g_internalPath[256];

// Native diagnostics supply a strong host-configured documents-path override.

i32 g_bgloadAsFastAsPossibleHack;

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

extern "C" {
    extern "C++" {
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

    // Locale string is owned by the Android/JNI TU; cache only its index here.
    i32 g_languageIndex = -1; // original .data @0x616b80

#include "nu2api/nucore/nucore_plain_types.h"
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }

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
} // extern "C"

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

// ---------------------------------------------------------------------------
// Diagnostics / caps
// ---------------------------------------------------------------------------

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
