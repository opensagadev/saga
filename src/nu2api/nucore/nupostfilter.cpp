#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/NuRenderDevice.h"

// NuPostFilter shared resources (retail nu3d/android/nupostfilter.cpp; the
// generic post-effect code in nu3d/android/nupostfilter.cpp is part of the
// nu3d_includes.cpp unity build).

extern u32 g_lastBoundVAO;

u32 NuPostFilter::m_fullscreenVertexBuffer, NuPostFilter::m_fullscreenIndexBuffer;
u32 NuPostFilter::m_fullscreenVertexFormat, NuPostFilter::m_fullscreenGridVertexBuffer;
u32 NuPostFilter::m_fullscreenGridIndexBuffer;
i32 NuPostFilter::m_quadGridPrimCount;

void NuPostFilter::initSharedResources(i32, i32) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x2a);
    NuPostFilterGen::initSharedResources();
    NuPostFilterGen::copyTexProgram = NuShaderProgramCreateIOS(default_vx, copytex_px);
    NuPostFilterGen::copyTexLodProgram = NULL;
    NuPostFilterGen::blur5x5Program = NULL;
    NuPostFilterGen::blur7x7Program = NULL;
    NuPostFilterGen::blurGuardProgram = NULL;
    const f32 vertices[12] = {-1, 1, 0, -1, -1, 0, 1, 1, 0, 1, -1, 0};
    glGenBuffers(1, &m_fullscreenVertexBuffer);
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &m_fullscreenGridVertexBuffer);
    glGenBuffers(1, &m_fullscreenGridIndexBuffer);
    i32 rows = static_cast<i32>((static_cast<f32>(nurndr_pixel_height) / nurndr_pixel_width) * 16.0f);
    i32 vertex_count = (rows + 1) * 17;
    i32 triangle_count = rows * 16 * 2;
    i32 size = vertex_count * 3 * sizeof(f32);
    f32 *grid = static_cast<f32 *>(NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    f32 *position = grid;
    for (i32 y = 0; y <= rows; ++y) {
        const f32 fraction = static_cast<f32>(y) / rows;
        const f32 screen_y = 1.0f - (fraction + fraction);
        for (i32 x = 0; x <= 16; ++x) {
            position[0] = -1.0f + x * 0.125f;
            position[1] = screen_y;
            position[2] = 0.0f;
            position += 3;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenGridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, grid, GL_STATIC_DRAW);
    NU_FREE(grid);
    u16 *indices = static_cast<u16 *>(NU_ALLOC(triangle_count * 3 * sizeof(u16), 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // Each row contains 16 cells, with the original two-triangle winding.
    u16 *index = indices;
    for (i32 y = 0; y < rows; ++y) {
        for (i32 x = 0; x < 16; ++x) {
            const i32 vertex = y * 17 + x;
            index[0] = vertex;
            index[1] = vertex + 1;
            index[2] = vertex + 17;
            index[3] = vertex + 17;
            index[4] = vertex + 1;
            index[5] = vertex + 18;
            index += 6;
        }
    }
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullscreenGridIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_count * 3 * sizeof(u16), indices, GL_STATIC_DRAW);
    NU_FREE(indices);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x7d);
}

void NuPostFilter::renderFrustum(numtx_s *) {
    STUBBED();
}
