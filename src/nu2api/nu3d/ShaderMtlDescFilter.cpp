#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nushader.h"
#include <cstring>
#include "nu2api/nucore/nuthread.h"

// Material descriptor filter (retail ShaderMtlDescFilter.cpp).

void ShaderMtlDescFilter::internalInit(nushadermtldesc_s const *material_desc, numtl_s const *material, i32 flags,
                                       i32 texture_threshold) {
    const u8 *material_bytes = reinterpret_cast<const u8 *>(material);
    desc = material_desc;
    mtl = material;
    flags_in = flags;
    texture_id_threshold = texture_threshold;
    variant = flags & 3;

    const u8 *vertex_descriptor = reinterpret_cast<const u8 *>(&material_desc->vtx_desc);
    field_0x10 = variant == 0 && (flags & 0x10) != 0 && (vertex_descriptor[2] & 4) == 0 &&
                 (material_bytes[0x41] & 0x40) == 0;
    field_0x18 = (variant != 0 || (flags & 0x10) != 0) && (vertex_descriptor[2] & 4) == 0 &&
                 (material_bytes[0x41] & 0x40) == 0;
    field_0x14 =
        variant == 0 && (flags & 0x20) != 0 && (vertex_descriptor[2] & 4) == 0 && (material_bytes[0x41] & 0x40) == 0;
    layer_count = variant == 0 ? 1 : 0;
    if (field_0x10 != 0 || (variant == 0 && (desc->flagsbits_1ba & 1) != 0)) {
        ++layer_count;
    }
    if (field_0x18 != 0) {
        ++layer_count;
    }
    if (field_0x14 != 0) {
        ++layer_count;
    }
}

bool ShaderMtlDescFilter::hasLayer(i32 layer) const {
    if (variant != 0) {
        if (layer == 0) {
            return desc->unknown_1b4 & 1;
        }
        return false;
    }

    switch (layer) {
        case 0:
            return true;
        case 3:
            return (desc->byte4 & 1) != 0 && desc->blend_op4 != 0xff;
        case 1:
            return (desc->flagsbits_1b8 & 0x40) != 0 && desc->blend_op2 != 0xff;
        case 2:
            return static_cast<i8>(desc->flagsbits_1b8) < 0 && desc->blend_op3 != 0xff;
        default:
            return false;
    }
}

bool ShaderMtlDescFilter::hasDiffuseMap(i32 layer) const {
    if (variant != 0 && desc->unknown_1b4 == 0) {
        return false;
    }
    switch (layer) {
        case 0:
            return desc->diffuse_map_tex_id[0] > texture_id_threshold;
        case 1:
            return desc->diffuse_map_tex_id[1] > texture_id_threshold;
        case 2:
            return desc->diffuse_map_tex_id[2] > texture_id_threshold;
        case 3:
            return desc->diffuse_map_tex_id[3] > texture_id_threshold;
    }
    return false;
}

u32 ShaderMtlDescFilter::getVertexFlags() const {
    const u8 *vertex_descriptor = reinterpret_cast<const u8 *>(&desc->vtx_desc);
    const u8 attribute_flags = vertex_descriptor[2];
    u32 flags = desc->vtx_desc.has_no_transform;
    if ((attribute_flags & 3) != 0 && (attribute_flags & 0x40) == 0) {
        flags |= 2;
    }
    if (variant == 0 || desc->unknown_1b4 != 0) {
        const u8 texture_count = desc->vtx_desc.unknown_1_2_4;
        if (texture_count != 0) {
            flags |= 4;
        }
        if ((variant == 0 || desc->unknown_1b4 != 0) && texture_count >= 2) {
            flags |= 8;
        }
        if ((variant == 0 || desc->unknown_1b4 != 0) && texture_count == 3) {
            flags |= 0x10;
        }
    }
    if ((attribute_flags & 3) != 0 && (attribute_flags & 0x40) != 0) {
        flags |= 0x40;
    }
    if ((attribute_flags & 8) != 0) {
        flags |= 0x80;
    }
    const u8 extra_attribute_flags = vertex_descriptor[3];
    if ((extra_attribute_flags & 1) != 0) {
        flags |= 0x100;
    }
    if ((extra_attribute_flags & 2) != 0) {
        flags |= 0x400;
    }
    if (variant == 0 && static_cast<i8>(desc->flagsbits_1bb) < 0) {
        flags |= 0x800;
    }
    if ((variant == 0 || desc->unknown_1b4 != 0) && (vertex_descriptor[1] & 1) != 0) {
        flags |= 0x1000;
    }
    return flags;
}
