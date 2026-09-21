#include "nu2api/nu3d/nupostresources.h"

#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuvuvec.hpp"

extern "C" {

    void NuFramebufferInitEx(void) {
    }

    SAGA_HOST_WEAK nuframebuffer_s *NuFramebufferCreate(void) {
    }

    void NuFramebufferDestroy(nuframebuffer_s *) {
    }

    void NuFramebufferCopyTex2D(i32, nueffecttex_s *, i32, i32, i32, i32, i32) {
    }

    void NuFramebufferAttachTex2D(nuframebuffer_s *, i32, nueffecttex_s *, i32) {
    }

    nueffecttex_s *NuFramebufferGetAttachedTex(nuframebuffer_s *, i32, i32 *, i32 *) {
        return NULL;
    }

    void NuFramebufferResolve(i32, bool) {
    }

    void NuFramebufferResolveAll(bool) {
    }

    void NuFramebufferResolveMultisample(i32) {
        NuFramebufferResolveAll(true);
    }

    void NuFramebufferEnableGuards(nuframebuffer_s *, bool) {
    }

    void NuFramebufferBind(nuframebuffer_s *) {
    }

    nuframebuffer_s *NuFramebufferGetBound(void) {
        return NULL;
    }

    i32 NuFramebufferGetWidth(nuframebuffer_s *framebuffer) {
        return *reinterpret_cast<const i32 *>(reinterpret_cast<const u8 *>(framebuffer) + 0xdc);
    }

    i32 NuFramebufferGetHeight(nuframebuffer_s *framebuffer) {
        return *reinterpret_cast<const i32 *>(reinterpret_cast<const u8 *>(framebuffer) + 0xe0);
    }

    void NuFramebufferDrawBuffers(void) {
    }

    nuframebuffer_s *NuFramebufferGetDefault(void) {
        return NULL;
    }

    nuframebuffer_s *NuFramebufferGetFrontBuffer(void) {
        return NULL;
    }

    void *NuFramebufferGetBackBuffer(void) {
        return NULL;
    }

    void NuFramebufferSwapBuffers(void) {
    }

    i32 NuFramebufferGetSamples(nuframebuffer_s *framebuffer) {
        return *reinterpret_cast<const i32 *>(reinterpret_cast<const u8 *>(framebuffer) + 0xe8);
    }

    void NuFramebufferClear(u32 clear_flags, u32 colour) {
        Nu360_dxClear(clear_flags, colour);
    }

    void NuFramebufferSetClearColor(void) {
    }

    nuframebuffer_s *NuFramebufferGetObject(i32) {
        return NULL;
    }

} // extern "C"

void NuFramebuffer360BeginZPass(i32) {
}

void NuFramebuffer360EndZPass(void) {
}

bool NuFramebuffer360HasZPass(void) {
    STUBBED();
    return false;
}

i32 NuFramebuffer360GetTileCount(nuframebuffer_s *) {
    STUBBED();
    return 0;
}
