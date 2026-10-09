#include "host/platform/compressed_texture.hpp"
#include "host/platform/wasm/graphics.hpp"

#include <GLES3/gl3.h>

#include <cstring>
#include <vector>

#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nuandroid/ios_graphics.h"

namespace {
    GLuint host_depth_buffer = 0;
}

extern "C" void __real_NuGScnFixupPS(NUGSCN *scene);
extern "C" void __real__Z15NuGScnDestroyPSP8nugscn_s(NUGSCN *scene);

extern "C" void __wrap_NuGScnFixupPS(NUGSCN *scene) {
    // WASM shares one context across threads. Scene prewarming also writes the
    // renderer's global material/vertex caches, so protect the whole operation.
    BeginCriticalSectionGL(__FILE__, __LINE__);
    __real_NuGScnFixupPS(scene);
    EndCriticalSectionGL(__FILE__, __LINE__);
}

extern "C" void __wrap__Z15NuGScnDestroyPSP8nugscn_s(NUGSCN *scene) {
    // Destruction takes the geometry lifetime mutex and then calls GL. Keep
    // the GL lock outside that mutex, matching scene fixup's lock order.
    BeginCriticalSectionGL(__FILE__, __LINE__);
    __real__Z15NuGScnDestroyPSP8nugscn_s(scene);
    EndCriticalSectionGL(__FILE__, __LINE__);
}

void HostPresentWasmFramebuffer(i32 width, i32 height) {
    // The scene renderer caches texture, shader, depth and vertex state. A
    // presentation draw must not replace those bindings behind its back.
    GLint read_framebuffer = 0;
    GLint draw_framebuffer = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_framebuffer);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_framebuffer);
    const GLboolean scissor_enabled = glIsEnabled(GL_SCISSOR_TEST);
    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, g_earlyColorFramebuffer);
    // Emscripten maps framebuffer 0 to its explicit-swap backbuffer.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, g_backingWidth, g_backingHeight, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, read_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_framebuffer);
    if (scissor_enabled) {
        glEnable(GL_SCISSOR_TEST);
    }
}

void HostResizeWasmFramebuffer(i32 width, i32 height) {
    GLint texture = 0;
    GLint renderbuffer = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &renderbuffer);
    glBindTexture(GL_TEXTURE_2D, g_earlyColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindRenderbuffer(GL_RENDERBUFFER, host_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

void NuIOS_AllocateSystemFramebuffers(void) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 106);

    memset(g_lastBound2DTexIds, 0, sizeof(g_lastBound2DTexIds));
    memset(g_lastBoundCubeTexIds, 0, sizeof(g_lastBoundCubeTexIds));

    glGenFramebuffers(1, &g_earlyColorFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, g_earlyColorFramebuffer);

    glGenTextures(1, &g_earlyColorTexture);
    glBindTexture(GL_TEXTURE_2D, g_earlyColorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_earlyColorTexture, 0);

    glGenRenderbuffers(1, &host_depth_buffer);
    HostResizeWasmFramebuffer(g_backingWidth, g_backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, host_depth_buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    g_defaultFramebuffer = 0;
    g_currentFramebuffer = g_earlyColorFramebuffer;

    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nuandroid/ios_graphics.cpp", 260);
}

extern "C" void NuIOS_UploadCompressedTexture(GLenum target, GLint level, GLenum internal_format, GLsizei width,
                                              GLsizei height, GLint border, GLsizei image_size, const void *data) {
    std::vector<u8> rgba;
    if (HostDecodeCompressedTexture(internal_format, width, height, image_size, data, rgba)) {
        glTexImage2D(target, level, GL_RGBA, width, height, border, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    } else if (internal_format != 0x8d64 && internal_format != 0x8c00 && internal_format != 0x8c02) {
        glCompressedTexImage2D(target, level, internal_format, width, height, border, image_size, data);
    }
}

bool NuIOS_TextureFormatSupported(i32 format) {
    return format == NUTEX_ETC1 || g_renderDevice.enabled_extensions[format];
}

GLenum NuIOS_PlatformVertexAttributeType(GLenum type) {
    // WebGL 2 exposes half floats under the core GL_HALF_FLOAT enum rather
    // than the OES token stored in the original Android vertex formats.
    return type == 0x8d61 ? 0x140b : type;
}

isize NuIOS_PlatformPrepareImmediateVertexData(isize data_address, usize data_size) {
    static GLuint immediate_vertex_buffer = 0;
    if (immediate_vertex_buffer == 0) {
        glGenBuffers(1, &immediate_vertex_buffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, immediate_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, data_size, reinterpret_cast<const void *>(data_address), GL_STREAM_DRAW);
    return 0;
}
