#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuprim.h"

// Immediate-mode vertex; UV storage is either two floats or packed half UVs.
struct PrimVertexRaw {
    f32 x, y, z;
    u32 color;
    union {
        u32 uv[2];
        f32 float_uv[2];
        u16 half_uv[4];
    };
};
DECOMP_ASSERT(sizeof(PrimVertexRaw) == 0x18, "PrimVertexRaw size");
DECOMP_ASSERT(offsetof(PrimVertexRaw, float_uv) == 0x10, "PrimVertexRaw float UV offset");
DECOMP_ASSERT(offsetof(PrimVertexRaw, half_uv) == 0x10, "PrimVertexRaw half UV offset");

static inline u16 NuRndrFloatToHalf(f32 value) {
    // Preserve the reference's signed intermediates and packed result bits.
    union {
        f32 value;
        u32 bits;
    } conversion = {value};
    i32 mantissa = conversion.bits & 0x7fffff;
    i32 sign = conversion.bits >> 31;
    i32 exponent = static_cast<i32>((conversion.bits >> 23) & 0xff) - 0x70;
    i16 result = (mantissa >> 13) | (sign << 15);
    if (exponent >= 0) {
        const i16 candidate = static_cast<i16>(exponent * 0x400);
        result |= exponent > 31 ? static_cast<i16>(0x7c00) : candidate;
    }
    return *reinterpret_cast<const u16 *>(&result);
}

static inline void NuRndrPrimSetColour(i32 colour) {
    if (g_NuPrim_NeedsOverbrightening)
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color = colour;
    else
        ((PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr)->color =
            ((colour >> 1) & 0x007f7f7f) | (colour & 0xff000000);
}

static inline void NuRndrPrimPosition(f32 x, f32 y, f32 z) {
    PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
    vertex->x = x;
    vertex->y = y;
    vertex->z = z;
    g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(PrimVertexRaw);
    ++g_NuPrim_VertexCount;
}

static inline void NuRndrPrimUV(f32 u, f32 v) {
    PrimVertexRaw *vertex = (PrimVertexRaw *)g_NuPrim_StreamBufferPtr->void_ptr;
    if (g_NuPrim_NeedsHalfUVs != 0) {
        vertex->half_uv[0] = NuRndrFloatToHalf(u);
        vertex->half_uv[1] = NuRndrFloatToHalf(v);
    } else {
        vertex->float_uv[0] = u;
        vertex->float_uv[1] = v;
    }
}

static inline void NuRndrPrimTexturedColour(f32 u, f32 v, i32 colour) {
    PrimVertexRaw *vertex;
    if (!g_NuPrim_NeedsHalfUVs) {
        vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);
        vertex->float_uv[0] = u;
        vertex->float_uv[1] = v;
    } else {
        vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);
        vertex->half_uv[0] = NuRndrFloatToHalf(u);
        vertex->half_uv[1] = NuRndrFloatToHalf(v);
    }
    i32 adjusted_colour = colour;
    if (!g_NuPrim_NeedsOverbrightening) {
        adjusted_colour = ((colour >> 1) & 0x007f7f7f) | (colour & 0xff000000);
    }
    vertex->color = adjusted_colour;
}
