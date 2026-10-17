// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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
#include "nu2api/nuandroid/ios_device.h"

// ---------------------------------------------------------------------------
// ios_graphics — iOS/Android GLES2 platform glue
//
// Original iOS path managed its own EGL / framebuffer and a dedicated render
// thread.  On Android the same code is reused: the "iOS" name is historical.
// This file contains platform plumbing, thread handoff, frame pacing, and
// path helpers.
// ---------------------------------------------------------------------------

// Backing drawable size driven by NuRenderDevice::InitialiseOpenGLContext.
i32 g_backingWidth;
i32 g_backingHeight;

// Nu-side DX-style clear flags, translated to GL masks below.
static constexpr u32 kNuClear_Color = 0x100;
static constexpr u32 kNuClear_Depth = 0x200;
static constexpr u32 kNuClear_Stencil = 0x800;


// ---------------------------------------------------------------------------
// Framebuffer setup
// ---------------------------------------------------------------------------

SAGA_HOST_WEAK void NuIOS_AllocateSystemFramebuffers(void);

// ---------------------------------------------------------------------------
// Diagnostics / caps
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Filesystem paths; the Android documents selection is cached after first use.
// ---------------------------------------------------------------------------

// Native diagnostics supply a strong host-configured documents-path override.

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

// Timestamp when the current frame's render was kicked — original
// @0x66c9a8, set in NuIOS_WakeRenderThread and sampled in
// NuIOS_WaitForRenderThreadCompletion for 60 Hz pacing.



// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.



extern "C" {

    GLuint g_colorRenderbuffer;
    GLuint g_depthRenderbuffer;
}

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

extern "C" {
// Original 0x317070. The packed colour has RGBA bytes from least to most
// significant; clearing depth also restores writable GL depth state.
extern "C" void Nu360_dxClear(u32 clear_flags, u32 colour) {
    static u32 lastColour = 0xff00ff00;
    GLbitfield glMask = 0;

    if ((clear_flags & kNuClear_Color) != 0) {
        glMask |= GL_COLOR_BUFFER_BIT;
        if (colour != lastColour) {
            const float r = static_cast<float>(colour & 0xff) / 255.0f;
            const float g = static_cast<float>((colour >> 8) & 0xff) / 255.0f;
            const float b = static_cast<float>((colour >> 0x10) & 0xff) / 255.0f;
            const float a = static_cast<float>(colour >> 0x18) / 255.0f;
            glClearColor(r, g, b, a);
            lastColour = colour;
        }
    }
    if (__builtin_expect((clear_flags & kNuClear_Depth) != 0, 0)) {
        glMask |= GL_DEPTH_BUFFER_BIT;
        if (g_renderContext_zFunc != 2) {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }
        g_renderContext_zFunc = 2;
    }
    if ((clear_flags & kNuClear_Stencil) != 0) {
        glMask |= GL_STENCIL_BUFFER_BIT;
    }
    glClear(glMask);
}

    void NuIOS_CheckCurrentFramebuffer(void) {
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            glCheckFramebufferStatus(GL_FRAMEBUFFER);
        }
    }

}

void NuIOS_AllocateSystemFramebuffers(void) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 106);
    NuCheckGLErrorsFL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 108);

    // Texture-cache shadow state — driver may have been torn down.
    memset(g_lastBound2DTexIds, 0, sizeof(g_lastBound2DTexIds));
    memset(g_lastBoundCubeTexIds, 0, sizeof(g_lastBoundCubeTexIds));

    g_earlyColorFramebuffer = 0;

    if (NuIOS_ShouldUseMSAA()) {
        glGenFramebuffers(1, &g_earlyColorMSAAFramebuffer);
    }

    g_defaultFramebuffer = 0;
    g_currentFramebuffer = 0;

    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 260);
}

extern "C" {
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

}

void NuIOSInitOpenGLES(void) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 308);
    NuIOS_AllocateSystemFramebuffers();
    glFrontFace(GL_CW);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 312);
}

