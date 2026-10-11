#include "host/platform/wasm/gl_context_state.hpp"

#include <GLES3/gl3.h>

#include <mutex>

#include "nu2api/nu3d/NuRenderDevice.h"

// Android gives every GL thread one of four shared EGL contexts. Objects are
// shared, but each context keeps its own bindings, so a loader thread binding
// a texture for upload never disturbs the render thread's bindings or the
// renderer's binding caches. WebGL offers a single context to all threads.
// Track the per-context bindings the game relies on and restore them when a
// thread takes the GL critical section.

extern "C" {
    void __real_glActiveTexture(GLenum texture);
    void __real_glBindTexture(GLenum target, GLuint texture);
    void __real_glDeleteTextures(GLsizei count, const GLuint *textures);
    void __real_glBindBuffer(GLenum target, GLuint buffer);
    void __real_glDeleteBuffers(GLsizei count, const GLuint *buffers);
    void __real_glUseProgram(GLuint program);
    void __real_glBindFramebuffer(GLenum target, GLuint framebuffer);
    void __real_glBindRenderbuffer(GLenum target, GLuint renderbuffer);
    void __real_glPixelStorei(GLenum name, GLint value);
    void __real_glEnableVertexAttribArray(GLuint index);
    void __real_glDisableVertexAttribArray(GLuint index);
}

namespace {
    constexpr i32 host_gl_context_count = 4;
    constexpr u32 host_gl_texture_units = 32;
    constexpr u32 host_gl_vertex_attributes = 32;

    struct HostGLBindings {
        GLenum active_texture = GL_TEXTURE0;
        GLuint texture_2d[host_gl_texture_units] = {};
        GLuint texture_cube[host_gl_texture_units] = {};
        GLuint array_buffer = 0;
        GLuint element_array_buffer = 0;
        GLuint program = 0;
        GLuint draw_framebuffer = 0;
        GLuint read_framebuffer = 0;
        GLuint renderbuffer = 0;
        GLint unpack_alignment = 4;
        GLint pack_alignment = 4;
        u32 enabled_attributes = 0;
    };

    std::mutex host_gl_state_mutex;
    // The bindings currently in the WebGL context, and those of each emulated
    // EGL context.
    HostGLBindings host_gl_live;
    HostGLBindings host_gl_contexts[host_gl_context_count];

    // Bindings a thread changes apply to the live context and, once the thread
    // has an emulated context, to that context's saved state.
    template <typename Update> void HostGLRecord(Update update) {
        update(host_gl_live);
        const i32 index = gt_glContextIndex;
        if (index >= 0 && index < host_gl_context_count) {
            update(host_gl_contexts[index]);
        }
    }

    u32 HostGLActiveUnit(const HostGLBindings &bindings) {
        return bindings.active_texture - GL_TEXTURE0;
    }

    void HostGLApply(const HostGLBindings &target) {
        HostGLBindings &live = host_gl_live;
        for (u32 unit = 0; unit < host_gl_texture_units; ++unit) {
            if (live.texture_2d[unit] == target.texture_2d[unit] &&
                live.texture_cube[unit] == target.texture_cube[unit]) {
                continue;
            }
            if (HostGLActiveUnit(live) != unit) {
                __real_glActiveTexture(GL_TEXTURE0 + unit);
                live.active_texture = GL_TEXTURE0 + unit;
            }
            if (live.texture_2d[unit] != target.texture_2d[unit]) {
                __real_glBindTexture(GL_TEXTURE_2D, target.texture_2d[unit]);
            }
            if (live.texture_cube[unit] != target.texture_cube[unit]) {
                __real_glBindTexture(GL_TEXTURE_CUBE_MAP, target.texture_cube[unit]);
            }
        }
        if (live.active_texture != target.active_texture) {
            __real_glActiveTexture(target.active_texture);
        }
        if (live.array_buffer != target.array_buffer) {
            __real_glBindBuffer(GL_ARRAY_BUFFER, target.array_buffer);
        }
        if (live.element_array_buffer != target.element_array_buffer) {
            __real_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, target.element_array_buffer);
        }
        if (live.program != target.program) {
            __real_glUseProgram(target.program);
        }
        if (live.draw_framebuffer != target.draw_framebuffer) {
            __real_glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.draw_framebuffer);
        }
        if (live.read_framebuffer != target.read_framebuffer) {
            __real_glBindFramebuffer(GL_READ_FRAMEBUFFER, target.read_framebuffer);
        }
        if (live.renderbuffer != target.renderbuffer) {
            __real_glBindRenderbuffer(GL_RENDERBUFFER, target.renderbuffer);
        }
        if (live.unpack_alignment != target.unpack_alignment) {
            __real_glPixelStorei(GL_UNPACK_ALIGNMENT, target.unpack_alignment);
        }
        if (live.pack_alignment != target.pack_alignment) {
            __real_glPixelStorei(GL_PACK_ALIGNMENT, target.pack_alignment);
        }
        const u32 changed_attributes = live.enabled_attributes ^ target.enabled_attributes;
        for (u32 index = 0; index < host_gl_vertex_attributes; ++index) {
            if ((changed_attributes & (1u << index)) == 0) {
                continue;
            }
            if ((target.enabled_attributes & (1u << index)) != 0) {
                __real_glEnableVertexAttribArray(index);
            } else {
                __real_glDisableVertexAttribArray(index);
            }
        }
        live = target;
    }

    // Deleting an object unbinds it from the current context only; other
    // contexts keep a binding to a name that no longer refers to it. Clear
    // those too, so a recycled name is never rebound behind the game's back.
    template <typename Clear> void HostGLForgetDeleted(GLsizei count, const GLuint *names, Clear clear) {
        if (names == nullptr) {
            return;
        }
        for (GLsizei i = 0; i < count; ++i) {
            if (names[i] == 0) {
                continue;
            }
            clear(host_gl_live, names[i]);
            for (HostGLBindings &context : host_gl_contexts) {
                clear(context, names[i]);
            }
        }
    }
} // namespace

void HostGLActivateThreadContext() {
    const i32 index = gt_glContextIndex;
    if (index < 0 || index >= host_gl_context_count) {
        return;
    }
    // Threads may also call GL outside the critical section, so compare with
    // the live bindings even when this context was the last one applied.
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    HostGLApply(host_gl_contexts[index]);
}

extern "C" void __wrap_glActiveTexture(GLenum texture) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glActiveTexture(texture);
    if (texture - GL_TEXTURE0 < host_gl_texture_units) {
        HostGLRecord([texture](HostGLBindings &bindings) { bindings.active_texture = texture; });
    }
}

extern "C" void __wrap_glBindTexture(GLenum target, GLuint texture) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glBindTexture(target, texture);
    HostGLRecord([target, texture](HostGLBindings &bindings) {
        const u32 unit = HostGLActiveUnit(bindings);
        if (target == GL_TEXTURE_2D) {
            bindings.texture_2d[unit] = texture;
        } else if (target == GL_TEXTURE_CUBE_MAP) {
            bindings.texture_cube[unit] = texture;
        }
    });
}

extern "C" void __wrap_glDeleteTextures(GLsizei count, const GLuint *textures) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glDeleteTextures(count, textures);
    HostGLForgetDeleted(count, textures, [](HostGLBindings &bindings, GLuint name) {
        for (u32 unit = 0; unit < host_gl_texture_units; ++unit) {
            if (bindings.texture_2d[unit] == name) {
                bindings.texture_2d[unit] = 0;
            }
            if (bindings.texture_cube[unit] == name) {
                bindings.texture_cube[unit] = 0;
            }
        }
    });
}

extern "C" void __wrap_glBindBuffer(GLenum target, GLuint buffer) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glBindBuffer(target, buffer);
    HostGLRecord([target, buffer](HostGLBindings &bindings) {
        if (target == GL_ARRAY_BUFFER) {
            bindings.array_buffer = buffer;
        } else if (target == GL_ELEMENT_ARRAY_BUFFER) {
            bindings.element_array_buffer = buffer;
        }
    });
}

extern "C" void __wrap_glDeleteBuffers(GLsizei count, const GLuint *buffers) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glDeleteBuffers(count, buffers);
    HostGLForgetDeleted(count, buffers, [](HostGLBindings &bindings, GLuint name) {
        if (bindings.array_buffer == name) {
            bindings.array_buffer = 0;
        }
        if (bindings.element_array_buffer == name) {
            bindings.element_array_buffer = 0;
        }
    });
}

extern "C" void __wrap_glUseProgram(GLuint program) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glUseProgram(program);
    HostGLRecord([program](HostGLBindings &bindings) { bindings.program = program; });
}

extern "C" void __wrap_glBindFramebuffer(GLenum target, GLuint framebuffer) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glBindFramebuffer(target, framebuffer);
    HostGLRecord([target, framebuffer](HostGLBindings &bindings) {
        if (target == GL_FRAMEBUFFER || target == GL_DRAW_FRAMEBUFFER) {
            bindings.draw_framebuffer = framebuffer;
        }
        if (target == GL_FRAMEBUFFER || target == GL_READ_FRAMEBUFFER) {
            bindings.read_framebuffer = framebuffer;
        }
    });
}

extern "C" void __wrap_glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glBindRenderbuffer(target, renderbuffer);
    HostGLRecord([renderbuffer](HostGLBindings &bindings) { bindings.renderbuffer = renderbuffer; });
}

extern "C" void __wrap_glPixelStorei(GLenum name, GLint value) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glPixelStorei(name, value);
    HostGLRecord([name, value](HostGLBindings &bindings) {
        if (name == GL_UNPACK_ALIGNMENT) {
            bindings.unpack_alignment = value;
        } else if (name == GL_PACK_ALIGNMENT) {
            bindings.pack_alignment = value;
        }
    });
}

extern "C" void __wrap_glEnableVertexAttribArray(GLuint index) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glEnableVertexAttribArray(index);
    if (index < host_gl_vertex_attributes) {
        HostGLRecord([index](HostGLBindings &bindings) { bindings.enabled_attributes |= 1u << index; });
    }
}

extern "C" void __wrap_glDisableVertexAttribArray(GLuint index) {
    std::lock_guard<std::mutex> lock(host_gl_state_mutex);
    __real_glDisableVertexAttribArray(index);
    if (index < host_gl_vertex_attributes) {
        HostGLRecord([index](HostGLBindings &bindings) { bindings.enabled_attributes &= ~(1u << index); });
    }
}
