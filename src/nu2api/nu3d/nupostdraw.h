#pragma once

#include "decomp.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostshaderparams.h"
#include "nu2api/nu3d/android/nurndr_android.h"

extern u32 g_lastBoundVAO;
extern void *g_nuFullscreenVertexFormat;

static inline void PostBindProgram(nushaderprogram_s *program) {
    g_boundShader = program != NULL ? program->program : 0;
    glUseProgram(g_boundShader);
    g_currentShaderProgram = program;
}

// These draw sequences are inlined into the original generic filters.
// NuPostFilterGen::renderQuad/Grid themselves are Android no-ops.
static inline void PostBlurDrawQuad();
static inline void PostMainDrawGrid();
static void PostDrawQuad(bool grid = false) {
    // Setting the declaration alone does not bind its attributes. Reuse the
    // full reference draw closures, including attribute enable/disable state.
    if (grid)
        PostMainDrawGrid();
    else
        PostBlurDrawQuad();
}

// Original generic blur inlines these immediate GL operations.
extern void (*g_glConstantSetterTable[4])(u32, i32, const void *);

struct PostBlurVertexAttribute {
    u32 type, size, normalized, reserved, offset, stride;
};
struct PostBlurVertexFormat {
    u32 mask;
    PostBlurVertexAttribute attributes[13];
};

static inline void PostBlurDrawQuad() {
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, NuPostFilter::m_fullscreenVertexBuffer);
    PostBlurVertexFormat *format = static_cast<PostBlurVertexFormat *>(g_nuFullscreenVertexFormat);
    g_boundVertexFormat = reinterpret_cast<NuVertexFormatPS *>(format);
    u32 active = format->mask;
    u32 disable = g_activeAttributes & ~active;
    u32 enable = active & ~g_activeAttributes;
    g_activeAttributes = active;
    u32 i = 0;
    do {
        if ((active & 1) != 0) {
            PostBlurVertexAttribute *attribute = &format->attributes[i];
            if ((enable & 1) != 0)
                glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attribute->size, attribute->type, static_cast<u8>(attribute->normalized),
                                  attribute->stride, reinterpret_cast<void *>(attribute->offset));
        } else if ((disable & 1) != 0) {
            glDisableVertexAttribArray(i);
        }
        ++i;
        active >>= 1;
        disable >>= 1;
        enable >>= 1;
    } while ((active | disable | enable) != 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
static inline void PostMainDrawGrid() {
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, NuPostFilter::m_fullscreenGridVertexBuffer);
    PostBlurVertexFormat *format = static_cast<PostBlurVertexFormat *>(g_nuFullscreenVertexFormat);
    g_boundVertexFormat = reinterpret_cast<NuVertexFormatPS *>(format);
    u32 active = format->mask;
    u32 disable = g_activeAttributes & ~active;
    u32 enable = active & ~g_activeAttributes;
    g_activeAttributes = active;
    u32 i = 0;
    do {
        if ((active & 1) != 0) {
            PostBlurVertexAttribute *attribute = &format->attributes[i];
            if ((enable & 1) != 0)
                glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attribute->size, attribute->type, static_cast<u8>(attribute->normalized),
                                  attribute->stride, reinterpret_cast<void *>(attribute->offset));
        } else if ((disable & 1) != 0) {
            glDisableVertexAttribArray(i);
        }
        ++i;
        active >>= 1;
        disable >>= 1;
        enable >>= 1;
    } while ((active | disable | enable) != 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NuPostFilter::m_fullscreenGridIndexBuffer);
    glDrawElements(GL_TRIANGLES, NuPostFilter::m_quadGridPrimCount * 3, GL_UNSIGNED_SHORT, NULL);
}
