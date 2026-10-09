#include "nu2api/nu3d/NuRenderDevice.h"

#include <emscripten/html5_webgl.h>

#include "decomp.h"
#include "host/platform/graphics.hpp"
#include "host/platform/wasm/graphics.hpp"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nucore.hpp"

namespace {
    bool host_msaa_enabled = true;
} // namespace

void HostSetMsaaEnabled(bool enabled) {
    host_msaa_enabled = enabled;
}

void NuRenderInspectEGLConfig(EGLDisplay, EGLConfig) {
    STUBBED();
}

void NuRenderDevice::BeginCriticalSection(const char *, i32) {
    pthread_mutex_lock(&this->mutex2);
    const i32 previous_lock_count = this->lock_count++;
    if (previous_lock_count == 0) {
        if (gt_glContextIndex == -1) {
            gt_glContextIndex = g_nextGLContextIndex;
            g_nextGLContextIndex = (g_nextGLContextIndex + 1) % 4;
        }
        emscripten_webgl_make_context_current(reinterpret_cast<uintptr_t>(this->contexts[gt_glContextIndex]));
    }
}

void NuRenderDevice::EndCriticalSection(const char *, i32) {
    --this->lock_count;
    pthread_mutex_unlock(&this->mutex2);
}

void NuRenderDevice::SwapBuffers() {
    if (NuCore::GetApplicationState()->GetStatus() == 1) {
        return;
    }

    const EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = reinterpret_cast<uintptr_t>(this->contexts[3]);
    if (context != 0 && emscripten_webgl_make_context_current(context) == EMSCRIPTEN_RESULT_SUCCESS) {
        i32 width = static_cast<i32>(this->width);
        i32 height = static_cast<i32>(this->height);
        emscripten_webgl_get_drawing_buffer_size(context, &width, &height);
        if (g_earlyColorFramebuffer != 0 && width > 0 && height > 0) {
            HostPresentWasmFramebuffer(width, height);
        }
        emscripten_webgl_commit_frame();

        // Resize between frames, after presenting the existing color buffer.
        if (g_earlyColorTexture != 0 && width > 0 && height > 0 &&
            (width != g_backingWidth || height != g_backingHeight)) {
            NUVIEWPORT2 viewport;
            NuVpGetCurrent2(&viewport);
            HostResizeWasmFramebuffer(width, height);
            this->width = this->backing_width = static_cast<u32>(width);
            this->height = this->backing_height = static_cast<u32>(height);
            g_backingWidth = nurndr_pixel_width = width;
            g_backingHeight = nurndr_pixel_height = height;
            this->nominal_aspect_ratio = DetermineNominalAspectRatio(this->width, this->height);
            this->aspect_ratio = static_cast<f32>(width) / static_cast<f32>(height);
            NuVpSetCurrent2(&viewport);
        }
    }
}

void NuRenderDevice::InitialiseOpenGLContext(ANativeWindow *) {
    pthread_mutex_lock(&this->mutex);
    if (!this->context_valid) {
        EmscriptenWebGLContextAttributes attributes;
        emscripten_webgl_init_context_attributes(&attributes);
        attributes.alpha = false;
        attributes.depth = true;
        attributes.stencil = false;
        attributes.antialias = host_msaa_enabled;
        attributes.majorVersion = 2;
        attributes.minorVersion = 0;
        attributes.explicitSwapControl = true;
        attributes.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_ALWAYS;
        attributes.renderViaOffscreenBackBuffer = true;

        const EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attributes);
        if (context == 0 || emscripten_webgl_make_context_current(context) != EMSCRIPTEN_RESULT_SUCCESS) {
            LOG_ERR("failed to create WebGL context");
            pthread_mutex_unlock(&this->mutex);
            return;
        }

        const EGLContext stored_context = reinterpret_cast<EGLContext>(context);
        const EGLSurface stored_surface = reinterpret_cast<EGLSurface>(static_cast<uintptr_t>(1));
        for (i32 i = 0; i < 4; ++i) {
            this->contexts[i] = stored_context;
            this->pbuffers[i] = stored_surface;
        }
        this->egl_display = reinterpret_cast<EGLDisplay>(static_cast<uintptr_t>(1));

        i32 width = 0;
        i32 height = 0;
        emscripten_webgl_get_drawing_buffer_size(context, &width, &height);
        this->width = this->backing_width = static_cast<u32>(width);
        this->height = this->backing_height = static_cast<u32>(height);
        g_backingWidth = static_cast<i32>(this->backing_width);
        g_backingHeight = static_cast<i32>(this->backing_height);
        nurndr_pixel_width = width;
        nurndr_pixel_height = height;
        this->context_valid = true;
        emscripten_webgl_make_context_current(0);
    }
    pthread_mutex_unlock(&this->mutex);
}
