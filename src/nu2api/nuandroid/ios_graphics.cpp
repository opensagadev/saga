// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nuandroid/ios_graphics.h"

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
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include "nu2api/nu3d/nutex.h"
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
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "nu2api/nuandroid/ios_device.h"

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

extern "C" {
    GLuint g_colorRenderbuffer;
    GLuint g_depthRenderbuffer;
}

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

