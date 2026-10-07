// Original NuGetVertexDeclaration @0x2fc9a0. Cache interleaved vertex layouts.
#include <string.h>

#include "nu2api/nu3d/android/nuvertexformat_android.h"

#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nuvuvec.hpp"

// g_vertexFormatPool: original static bss 0x11b90a0 (_ZL18g_vertexFormatPool),
// 256 entries of 0x140 bytes each.
static NuVertexFormatPoolEntryPS g_vertexFormatPool[256];

// g_allocatedDescriptorCount: original static bss 0x11cd0a0
// (_ZL26g_allocatedDescriptorCount).
static i32 g_allocatedDescriptorCount;

static_assert(sizeof(NuVertDeclAttribPS) == 0x18, "attrib record must be 6 dwords");
static_assert(sizeof(NuVertexFormatPS) == 0x13c, "format array must be mask + 13 attrib records");
static_assert(sizeof(NuVertexFormatPoolEntryPS) == 0x140, "pool record stride must be 0x140");

NuVertexFormatPS *NuGetVertexDeclaration(NUVERTEXDESCRIPTOR vtx_desc) {
    const i32 count = g_allocatedDescriptorCount;
    for (i32 i = 0; i < count; ++i) {
        if (memcmp(&g_vertexFormatPool[i].key, &vtx_desc, sizeof(vtx_desc)) == 0)
            return &g_vertexFormatPool[i].format;
    }
    if (static_cast<u32>(count) > 255)
        return NULL;
    NuVertexFormatPoolEntryPS *record = &g_vertexFormatPool[count];
    NuVertexFormatPS *format = &record->format;
    NUVERTEXDESCRIPTOR descriptor_copy = vtx_desc;
    const u8 texture_coordinates = vtx_desc.tex_coord_mode;
    descriptor_copy.tex_coord_mode = texture_coordinates;
    const u32 descriptor = descriptor_copy.flags;
    // Each optional stream is decoded first: 0 = absent, 1 = float, 2 = packed bytes.
    const i32 normal = (descriptor & 0x880008) ? 2 : vtx_desc.has_normal;
    const i32 tangent = (descriptor & 0x1000020) ? 2 : vtx_desc.has_tangent;
    const i32 binormal = (descriptor & 0x2000080) ? 2 : vtx_desc.has_binormal;
    const bool diffuse = vtx_desc.has_diffuse;
    const bool specular = (descriptor & 0x600) != 0;
    i32 texture_mode, half_texture_mode;
    if (vtx_desc.has_half_uvs) {
        half_texture_mode = vtx_desc.tex_coord_mode;
        texture_mode = 0;
    } else {
        texture_mode = vtx_desc.tex_coord_mode;
        half_texture_mode = 0;
    }
    const i32 weights = (descriptor & 0x8000) ? 2 : (descriptor >> 14) & 1;
    const i32 colour2 = (descriptor & 0x20000) ? 2 : (descriptor >> 16) & 1;
    const bool skinned = (descriptor & 0x4000000) != 0;
    const bool instanced = (descriptor >> 22) & 1;
    i16 offset = 0;
#define VERTEX_ATTRIBUTE(slot, gl_type, components, normalize, bytes)                                                  \
    format->attribs[slot].type = gl_type;                                                                              \
    format->attribs[slot].size = components;                                                                           \
    format->attribs[slot].normalized = normalize;                                                                      \
    format->attribs[slot].unknown_0c = 0;                                                                              \
    format->attribs[slot].offset = offset;                                                                             \
    format->attrib_mask |= 1u << slot;                                                                                 \
    offset += bytes
    VERTEX_ATTRIBUTE(0, 0x1406, 3, 0, 12);
    switch (normal) {
        case 1:
            VERTEX_ATTRIBUTE(3, 0x1406, 3, 0, 12);
            break;
        case 2:
            VERTEX_ATTRIBUTE(3, 0x1401, 4, 1, 4);
            break;
    }
    switch (tangent) {
        case 1:
            VERTEX_ATTRIBUTE(4, 0x1406, 3, 0, 12);
            break;
        case 2:
            VERTEX_ATTRIBUTE(4, 0x1401, 4, 1, 4);
            break;
    }
    switch (binormal) {
        case 1:
            VERTEX_ATTRIBUTE(5, 0x1406, 3, 0, 12);
            break;
        case 2:
            VERTEX_ATTRIBUTE(5, 0x1401, 4, 1, 4);
            break;
    }
    if (diffuse) {
        VERTEX_ATTRIBUTE(1, 0x1401, 4, 1, 4);
    }
    if (specular) {
        VERTEX_ATTRIBUTE(2, 0x1401, 4, 1, 4);
    }
    switch (texture_mode) {
        case 1:
            VERTEX_ATTRIBUTE(6, 0x1406, 2, 0, 8);
            break;
        case 2:
            VERTEX_ATTRIBUTE(6, 0x1406, 4, 0, 16);
            break;
        case 3:
            VERTEX_ATTRIBUTE(6, 0x1406, 4, 0, 16);
            VERTEX_ATTRIBUTE(7, 0x1406, 2, 0, 8);
            break;
        case 4:
            VERTEX_ATTRIBUTE(6, 0x1406, 4, 0, 16);
            VERTEX_ATTRIBUTE(7, 0x1406, 4, 0, 16);
            break;
        case 5:
            VERTEX_ATTRIBUTE(6, 0x1406, 2, 0, 8);
            VERTEX_ATTRIBUTE(7, 0x1406, 2, 0, 8);
            break;
    }
    switch (half_texture_mode) {
        case 1:
            VERTEX_ATTRIBUTE(6, 0x8d61, 2, 0, 4);
            break;
        case 2:
            VERTEX_ATTRIBUTE(6, 0x8d61, 4, 0, 8);
            break;
        case 3:
            VERTEX_ATTRIBUTE(6, 0x8d61, 4, 0, 8);
            VERTEX_ATTRIBUTE(7, 0x8d61, 2, 0, 4);
            break;
        case 4:
            VERTEX_ATTRIBUTE(6, 0x8d61, 4, 0, 8);
            VERTEX_ATTRIBUTE(7, 0x8d61, 4, 0, 8);
            break;
    }
    switch (weights) {
        case 1:
            VERTEX_ATTRIBUTE(10, 0x1406, 2, 0, 8);
            break;
        case 2:
            VERTEX_ATTRIBUTE(10, 0x1401, 4, 1, 4);
            break;
    }
    switch (colour2) {
        case 1:
            VERTEX_ATTRIBUTE(11, 0x1406, 3, 0, 12);
            break;
        case 2:
            VERTEX_ATTRIBUTE(11, 0x1401, 4, 0, 4);
            break;
    }
    if (skinned) {
        VERTEX_ATTRIBUTE(8, 0x1401, 4, 1, 4);
        VERTEX_ATTRIBUTE(9, 0x1401, 4, 1, 4);
    }
    i32 extra_stride = 0;
    if (instanced) {
        format->attribs[12].type = 0x1406;
        format->attribs[12].size = 3;
        format->attribs[12].normalized = 0;
        format->attribs[12].unknown_0c = 1;
        format->attribs[12].offset = 0;
        format->attrib_mask |= 0x1000;
        extra_stride = 12;
    }
    const u32 mask = format->attrib_mask;
    for (i32 i = 0; i < 13; ++i) {
        if (mask & (1u << i))
            format->attribs[i].stride = format->attribs[i].unknown_0c ? extra_stride : offset;
    }
    record->key = vtx_desc.flags;
    g_allocatedDescriptorCount = count + 1;
    return format;
#undef VERTEX_ATTRIBUTE
}
