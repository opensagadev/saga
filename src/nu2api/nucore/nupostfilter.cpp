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
    i32 size = (rows + 1) * 17 * 3 * sizeof(f32);
    f32 *grid = static_cast<f32 *>(NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // The original expands each fixed-width row into 17 vertex writes.
    f32 *row = grid;
    for (i32 y = 0; y <= rows; ++y, row += 51) {
        const f32 fraction = static_cast<f32>(y) / rows;
        const f32 screen_y = 1.0f - (fraction + fraction);
#define GRID_VERTEX(column)                                                                                            \
    row[(column) * 3] = -1.0f + (column) * 0.125f;                                                                     \
    row[(column) * 3 + 1] = screen_y;                                                                                  \
    row[(column) * 3 + 2] = 0.0f
        GRID_VERTEX(0);
        GRID_VERTEX(1);
        GRID_VERTEX(2);
        GRID_VERTEX(3);
        GRID_VERTEX(4);
        GRID_VERTEX(5);
        GRID_VERTEX(6);
        GRID_VERTEX(7);
        GRID_VERTEX(8);
        GRID_VERTEX(9);
        GRID_VERTEX(10);
        GRID_VERTEX(11);
        GRID_VERTEX(12);
        GRID_VERTEX(13);
        GRID_VERTEX(14);
        GRID_VERTEX(15);
        GRID_VERTEX(16);
#undef GRID_VERTEX
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenGridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, grid, GL_STATIC_DRAW);
    NU_FREE(grid);
    u16 *indices = static_cast<u16 *>(NU_ALLOC(rows * 192, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // Each row contains 16 cells, with the original two-triangle winding.
    u16 *index_row = indices;
    i32 vertex = 0;
    for (i32 y = 0; y < rows; ++y, index_row += 96, vertex += 17) {
#define GRID_CELL(column)                                                                                              \
    index_row[(column) * 6] = vertex + (column);                                                                       \
    index_row[(column) * 6 + 1] = vertex + (column) + 1;                                                               \
    index_row[(column) * 6 + 2] = vertex + (column) + 17;                                                              \
    index_row[(column) * 6 + 3] = vertex + (column) + 17;                                                              \
    index_row[(column) * 6 + 4] = vertex + (column) + 1;                                                               \
    index_row[(column) * 6 + 5] = vertex + (column) + 18
        GRID_CELL(0);
        GRID_CELL(1);
        GRID_CELL(2);
        GRID_CELL(3);
        GRID_CELL(4);
        GRID_CELL(5);
        GRID_CELL(6);
        GRID_CELL(7);
        GRID_CELL(8);
        GRID_CELL(9);
        GRID_CELL(10);
        GRID_CELL(11);
        GRID_CELL(12);
        GRID_CELL(13);
        GRID_CELL(14);
        GRID_CELL(15);
#undef GRID_CELL
    }
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullscreenGridIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rows * 192, indices, GL_STATIC_DRAW);
    NU_FREE(indices);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x7d);
}

void NuPostFilter::renderFrustum(numtx_s *) {
    STUBBED();
}
