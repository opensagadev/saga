#pragma once

#include "nu2api/nucore/NuPostFilterGen.h"

struct numtx_s;

// Retail emits a weak NuPostFilter vtable whose slots are NuPostFilterGen's.
struct NuPostFilter : NuPostFilterGen {
    static void initSharedResources(i32, i32);
    void renderFrustum(numtx_s *);
    static u32 m_fullscreenVertexBuffer, m_fullscreenIndexBuffer, m_fullscreenVertexFormat;
    static u32 m_fullscreenGridVertexBuffer, m_fullscreenGridIndexBuffer;
    static i32 m_quadGridPrimCount;
};
