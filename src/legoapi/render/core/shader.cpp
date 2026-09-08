#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nushader.h"
#include <cstring>
#include "nu2api/nucore/nuthread.h"

u32 g_cachedUniformMask[4];
u32 g_textureSemanticMask[4];
i32 g_shaderBufferCriticalSection;

ShaderManagerOpenGL::ShaderManagerOpenGL(VirtualStackAllocator &) {
    g_cachedUniformMask[0] = 0;
    g_cachedUniformMask[1] = 0xffe00000;
    g_cachedUniformMask[3] = 0x1f;
    g_cachedUniformMask[2] = 0xf900eb91;
    g_textureSemanticMask[1] = 0;
    g_textureSemanticMask[2] = 0;
    g_textureSemanticMask[3] = 0;
    g_textureSemanticMask[0] = 0x1fffff;
    g_shaderBufferCriticalSection = NuThreadCreateCriticalSection();
}

bool ShaderManagerOpenGL::createShader(ShaderMtlDescFilter &filter, bool pixelStage, NuShaderObject *object, i32 id) {
    ShaderObjectKey key = {0};
    NuShaderObjectKeyGenerate3(&key.key, &filter, pixelStage);
    return createShader(key, object, id);
}

void ShaderManagerOpenGL::setElementfv(SHADERSEMANTIC_enum semantic, i32, float const *values) {
    // The original Android implementation ignores the element range.
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

void ShaderManagerOpenGL::setElementsfv(SHADERSEMANTIC_enum semantic, i32, i32, float const *values) {
    // The original Android implementation ignores the element range.
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

void ShaderManagerOpenGL::setElementsfv_transpose(SHADERSEMANTIC_enum semantic, i32, i32, float const *values) {
    // The original Android implementation ignores the element range.
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

void ShaderManagerOpenGL::setfv(SHADERSEMANTIC_enum semantic, float const *values) {
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

ShaderManagerOpenGL::~ShaderManagerOpenGL() {
}

u32 ShaderMtlDescFilter::getVertexFlags() const {
    const u8 *vertex_descriptor = reinterpret_cast<const u8 *>(&desc->vtx_desc);
    const u8 texture_flags = vertex_descriptor[1];
    const u8 attribute_flags = vertex_descriptor[2];
    const u8 extra_attribute_flags = vertex_descriptor[3];
    const bool extended = variant == 0 || desc->unknown_1b4 != 0;

    u32 flags = (attribute_flags >> 2) & 1;
    if ((attribute_flags & 3) != 0 && (attribute_flags & 0x40) == 0) {
        flags |= 2;
    }

    const u8 texture_count = (texture_flags >> 1) & 3;
    if (extended) {
        if (texture_count != 0) {
            flags |= 4;
        }
        if (texture_count >= 2) {
            flags |= 8;
        }
        if (texture_count == 3) {
            flags |= 0x10;
        }
    }
    if ((attribute_flags & 3) != 0 && (attribute_flags & 0x40) != 0) {
        flags |= 0x40;
    }
    if ((attribute_flags & 8) != 0) {
        flags |= 0x80;
    }
    if ((extra_attribute_flags & 1) != 0) {
        flags |= 0x100;
    }
    if ((extra_attribute_flags & 2) != 0) {
        flags |= 0x400;
    }
    if (extended) {
        if (static_cast<i8>(desc->flagsbits_1bb) < 0) {
            flags |= 0x800;
        }
        if ((texture_flags & 1) != 0) {
            flags |= 0x1000;
        }
    }
    return flags;
}

bool ShaderMtlDescFilter::hasDiffuseMap(i32 layer) const {
    if (variant != 0 && desc->unknown_1b4 == 0) {
        return false;
    }
    if (layer < 0 || layer >= 4) {
        return false;
    }
    return desc->diffuse_map_tex_id[layer] > texture_id_threshold;
}

bool ShaderMtlDescFilter::hasLayer(i32 layer) const {
    if (variant != 0) {
        return layer == 0 && (desc->unknown_1b4 & 1) != 0;
    }

    switch (layer) {
        case 0:
            return true;
        case 1:
            return (desc->flagsbits_1b8 & 0x40) != 0 && desc->blend_op2 != 0xff;
        case 2:
            return static_cast<i8>(desc->flagsbits_1b8) < 0 && desc->blend_op3 != 0xff;
        case 3:
            return (desc->byte4 & 1) != 0 && desc->blend_op4 != 0xff;
        default:
            return false;
    }
}

void ShaderMtlDescFilter::internalInit(nushadermtldesc_s const *material_desc, numtl_s const *material, i32 flags,
                                       i32 texture_threshold) {
    const u8 *material_bytes = reinterpret_cast<const u8 *>(material);
    desc = material_desc;
    mtl = material;
    flags_in = flags;
    variant = flags & 3;
    texture_id_threshold = texture_threshold;

    const u8 *vertex_descriptor = reinterpret_cast<const u8 *>(&desc->vtx_desc);
    const bool special_vertex_path = (vertex_descriptor[2] & 4) != 0 || (material_bytes[0x41] & 0x40) != 0;
    const bool base_variant = variant == 0;
    field_0x10 = base_variant && (flags & 0x10) != 0 && !special_vertex_path;
    field_0x18 = !special_vertex_path;
    field_0x14 = base_variant && (flags & 0x20) != 0 && !special_vertex_path;

    layer_count = 0;
    if (base_variant) {
        layer_count = field_0x10 == 0 && (desc->flagsbits_1ba & 1) == 0 ? 1 : 2;
    }
    if (field_0x18 != 0) {
        ++layer_count;
    }
    if (field_0x14 != 0) {
        ++layer_count;
    }
}

namespace nu2api {
    extern void *g_shaderManager;
}

extern "C" void NuShaderManagerSetfv(i32 semantic, const f32 *values) {
    static_cast<ShaderManagerOpenGL *>(nu2api::g_shaderManager)
        ->setfv(static_cast<SHADERSEMANTIC_enum>(semantic), values);
}

extern "C" void NuShaderManagerSetElementfv(i32 semantic, i32 element, const f32 *values) {
    static_cast<ShaderManagerOpenGL *>(nu2api::g_shaderManager)
        ->setElementfv(static_cast<SHADERSEMANTIC_enum>(semantic), element, values);
}

extern "C" void NuShaderManagerSetElementsfv(i32 semantic, i32 first_element, i32 count, const f32 *values) {
    static_cast<ShaderManagerOpenGL *>(nu2api::g_shaderManager)
        ->setElementsfv(static_cast<SHADERSEMANTIC_enum>(semantic), first_element, count, values);
}

extern "C" void NuShaderManagerSetElementsfv_transpose(i32 semantic, i32 first_element, i32 count, const f32 *values) {
    static_cast<ShaderManagerOpenGL *>(nu2api::g_shaderManager)
        ->setElementsfv_transpose(static_cast<SHADERSEMANTIC_enum>(semantic), first_element, count, values);
}
