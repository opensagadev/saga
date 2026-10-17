// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/android/nutex_android.h"
#include "nu2api/nu3d/ShaderManagerOpenGL.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nu3d/nu2api_nu3d_types.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nushader_internal.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"

#include <GLES2/gl2.h>
#include <cstdio>
#include <cstring>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nucore/nucore_includes.h"

// iOS/GLES shader manager (retail nushadermanagerios.cpp): material
// adaptation for the mobile uber-shader, redirect-table lookups and program
// creation from the preloaded shader objects.

using nu2api::HashRedirect;
using nu2api::LoadedUniqueShaderRecord;

// Redirect / preloaded tables (nudlist_tables_gen.inc, built into
// nushadermanager_gen.cpp).
extern HashRedirect g_vertexShaderRedirects[792];
extern HashRedirect g_pixelShaderRedirects[602];
extern LoadedUniqueShaderRecord g_loadedUniqueVertexShaders[368];
extern LoadedUniqueShaderRecord g_loadedUniquePixelShaders[558];

u32 g_cachedUniformMask[4];
u32 g_textureSemanticMask[4];
i32 g_shaderBufferCriticalSection = -1;

void ShaderManagerOpenGL::adaptShaderMaterialForShaderVersion(NUSHADERMTLDESC *desc) {
    u8 *vtx_desc = reinterpret_cast<u8 *>(&desc->vtx_desc);
    const u8 original_vtx_flags3 = vtx_desc[3];
    u8 mobile_flags3 = desc->flagsbits_1bb;

    if ((original_vtx_flags3 & 4) != 0) {
        mobile_flags3 |= 0x80;
        desc->flagsbits_1bb = mobile_flags3;
        desc->flags |= 0x200000;
    }

    const u8 original_flags1 = desc->byte4;
    const u8 original_flags2 = desc->flagsbits_1ba;
    u8 mobile_flags1 = original_flags1 & 0xf7;
    desc->byte4 = mobile_flags1;
    desc->flagsbits_1ba = original_flags2 & 0x7f;

    const u32 packed_mobile_flags = *reinterpret_cast<u32 *>(&desc->flagsbits_1b8);
    u8 mobile_flags0 = desc->flagsbits_1b8;
    if ((packed_mobile_flags & 0x04001011) == 0x04001001 && desc->unknown_a8 > 2) {
        const u8 original_flags0 = mobile_flags0;
        mobile_flags0 &= 0x3f;
        desc->flagsbits_1b8 = mobile_flags0;
        desc->byte4 = original_flags1 & 0xf6;
        if ((original_flags0 & 8) == 0) {
            mobile_flags0 |= 0x10;
            desc->flagsbits_1b8 = mobile_flags0;
        }
        mobile_flags1 = desc->byte4;
        desc->normal_map_tid = 0;
    }

    const u32 original_shader_flags = desc->flags;
    mobile_flags0 &= 0xfc;
    desc->flagsbits_1b8 = mobile_flags0;
    desc->flags = original_shader_flags & 0xffff9ffc;
    desc->byte4 = mobile_flags1 & 0x9f;

    if ((original_shader_flags & 0x10000) == 0) {
        if (__builtin_expect((original_shader_flags & 0x20) != 0, 0)) {
            mobile_flags0 &= 0xf4;
            desc->flags = original_shader_flags & 0xffff9ff4;
            desc->flagsbits_1b8 = mobile_flags0;
        }
    } else {
        mobile_flags0 &= 0xdc;
        desc->flags = original_shader_flags & 0xffff9fdc;
        desc->flagsbits_1b8 = mobile_flags0;
    }

    if ((desc->flags & 0x40200) == 0) {
        desc->flags &= 0xfffffbff;
    }
    desc->byte4 = mobile_flags1 & 0x9b;

    desc->flagsbits_1b8 = mobile_flags0 & 0xfb;
    const u8 mobile_flags2 = original_flags2 & 0x48;
    desc->flagsbits_1ba = mobile_flags2;
    desc->flagsbits_1bb = mobile_flags3 & 0x27;
    desc->flags = desc->flags & 0xf916fffb;

    if (__builtin_expect(::RemoveNormalMaps != 0 && ::RemoveDirectionalMaps != 0, 0)) {
        vtx_desc[3] = original_vtx_flags3 & 0xfc;
        desc->normal_map_tid = 0;
        vtx_desc[0] &= 0x0f;
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(desc) + 0x38) = 0;
        desc->flags = desc->flags & 0xf916fffa;
        if ((mobile_flags3 & 4) != 0) {
            reinterpret_cast<u8 *>(&desc->field_1bc)[0] |= 8;
            desc->flagsbits_1ba = mobile_flags2 | 4;
            desc->flagsbits_1bb = (mobile_flags3 & 0x27) | 4;
        }
    }

    u8 &mobile_capabilities = reinterpret_cast<u8 *>(&desc->field_1bc)[0];
    mobile_capabilities = (mobile_capabilities & 0xfb) | ((NuIOS_IsLowEndDevice() == 0) << 2);
}

bool ShaderManagerOpenGL::createShader(ShaderMtlDescFilter &filter, bool pixelStage, NuShaderObject *object, i32 id) {
    ShaderObjectKey key = {0};
    NuShaderObjectKeyGenerate3(&key.key, &filter, pixelStage);
    return createShader(key, object, id);
}

ShaderManagerOpenGL::~ShaderManagerOpenGL() {
}

bool LookupHash(u32 key, u32 *value, HashRedirect *redirects, u32 count) {
    i32 upper = static_cast<i32>(count) - 1;
    if (upper < 0) {
        return false;
    }

    i32 index = upper / 2;
    HashRedirect *redirect = &redirects[index];
    if (redirect->key == key) {
        *value = redirect->value;
        return true;
    }

    i32 lower = 0;
    while (true) {
        if (key > redirect->key) {
            lower = index + 1;
        } else {
            upper = index - 1;
        }
        if (lower > upper) {
            return false;
        }

        index = (lower + upper) / 2;
        redirect = &redirects[index];
        if (redirect->key == key) {
            *value = redirect->value;
            return true;
        }
    }
}

extern HashRedirect g_shaderProgramRedirects[417];

ShaderObjectKey NuIOS_GetShaderProgramKey(ShaderObjectKey const &key) {
    ShaderObjectKey result;
    u32 redirected_key __attribute__((aligned(16)));
    if (LookupHash(key.key, &redirected_key, g_shaderProgramRedirects, 417))
        result.key = redirected_key;
    else
        result.key = key.key;
    return result;
}

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

bool LoadShaderSource(char **source, i32 *size, u32 key, bool pixel_stage) {
    static char storage[0x4000];

    *source = NULL;
    *size = 0;

    char path[256];
    sprintf(path, "%s/0x%08x.ios_%s", "builtshaders/ios", key, pixel_stage ? "pcode" : "vcode");

    NUFILE file = NuFileOpen(path, NUFILE_READ);
    if (file == 0) {
        return false;
    }

    *size = NuFileOpenSize(file);
    NuFileRead(file, storage, *size);
    NuFileClose(file);

    if (!pixel_stage) {
        char *precision = strstr(storage, "precision mediump float;");
        if (precision != NULL) {
            const char *replacement = "precision highp float;  ";
            memcpy(precision, replacement, NuStrLen(replacement));
        }
    } else {
        char *precision = strstr(storage, "precision lowp float;");
        if (precision != NULL && strstr(storage, "_envmap_samplerCube") != NULL) {
            const char *replacement = "precision mediump float;";
            memcpy(precision, replacement, NuStrLen(replacement));
        }
    }

    storage[*size] = '\0';
    *source = storage;
    return true;
}

bool LookupPreloadedShaderObject(u32 key, u32 **shader, LoadedUniqueShaderRecord *records, u32 count) {
    i32 upper = static_cast<i32>(count) - 1;
    if (upper < 0) {
        return false;
    }

    i32 index = upper / 2;
    LoadedUniqueShaderRecord *record = &records[index];
    if (record->key == key) {
        *shader = &record->gl_shader;
        return true;
    }

    i32 lower = 0;
    while (true) {
        if (key > record->key) {
            lower = index + 1;
        } else {
            upper = index - 1;
        }
        if (lower > upper) {
            return false;
        }

        index = (lower + upper) / 2;
        record = &records[index];
        if (record->key == key) {
            *shader = &record->gl_shader;
            return true;
        }
    }
}

bool ShaderManagerOpenGL::createShader(const ::ShaderObjectKey &key, NuShaderObject *outObject, i32 param) {
    using namespace nu2api;

    u32 vertexKey = key.key;
    u32 pixelKey = key.key;

    LookupHash(vertexKey, &vertexKey, g_vertexShaderRedirects, 0x318);
    LookupHash(pixelKey, &pixelKey, g_pixelShaderRedirects, 0x25a);

    GLuint *vertexSlot = nullptr;
    GLuint *pixelSlot = nullptr;
    LookupPreloadedShaderObject(vertexKey, &vertexSlot, g_loadedUniqueVertexShaders, 0x170);
    LookupPreloadedShaderObject(pixelKey, &pixelSlot, g_loadedUniquePixelShaders, 0x22e);

    if (vertexSlot == nullptr || pixelSlot == nullptr) {
        return false;
    }

    // Compile on demand, guarded by the file critical section like the original.
    if (*vertexSlot == 0) {
        NuThreadCriticalSectionBegin(file_criticalsection);
        NuThreadCriticalSectionBegin(g_shaderBufferCriticalSection);
        char *vertexSource;
        i32 vertexSize;
        const bool loaded = LoadShaderSource(&vertexSource, &vertexSize, vertexKey, false);
        const bool compiled =
            loaded && NuShaderObjectGenerateGLSLShader(vertexSlot, GL_VERTEX_SHADER, vertexSource, vertexSize);
        NuThreadCriticalSectionEnd(g_shaderBufferCriticalSection);
        NuThreadCriticalSectionEnd(file_criticalsection);
        if (!compiled) {
            return false;
        }
    }

    if (*pixelSlot == 0) {
        NuThreadCriticalSectionBegin(file_criticalsection);
        NuThreadCriticalSectionBegin(g_shaderBufferCriticalSection);
        char *pixelSource;
        i32 pixelSize;
        const bool loaded = LoadShaderSource(&pixelSource, &pixelSize, pixelKey, true);
        const bool compiled =
            loaded && NuShaderObjectGenerateGLSLShader(pixelSlot, GL_FRAGMENT_SHADER, pixelSource, pixelSize);
        NuThreadCriticalSectionEnd(g_shaderBufferCriticalSection);
        NuThreadCriticalSectionEnd(file_criticalsection);
        if (!compiled) {
            return false;
        }
    }

    NuShaderObjectInit(reinterpret_cast<nushaderobject_s *>(outObject),
                       reinterpret_cast<const nushaderobjectkey_s *>(&key), param, static_cast<u32>(*vertexSlot),
                       static_cast<u32>(*pixelSlot), static_cast<eSHADERVERSION>(5));
    return true;
}

void ShaderManagerOpenGL::setfv(SHADERSEMANTIC_enum semantic, float const *values) {
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
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
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

void ShaderManagerOpenGL::setElementsfv_transpose(SHADERSEMANTIC_enum semantic, i32, i32, float const *values) {
    nu2api::ShaderUniformRecord &uniform = nu2api::g_shaderUniforms[semantic];
    const i32 count = static_cast<i32>(uniform.data.metadata[0]);
    if (count <= 4) {
        std::memcpy(uniform.data.values, values, static_cast<size_t>(count) << 4);
    }
}

