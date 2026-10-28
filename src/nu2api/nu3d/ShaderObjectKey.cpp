#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/ShaderManagerOpenGL.h"
#include "gamelib/util/CRC16.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nushader_plain.h"
#include "nu2api/nu3d/nu2api_nu3d_types.h"
#include "nushader.h"
#include "nushader_internal.h"

// Shader object key generation (retail ShaderObjectKey.cpp): the 104-byte
// material block hashed with the forward/inverse CRC-16 pair.

namespace nu2api {

    // ---------------------------------------------------------------------------
    // Key generation
    //
    // Builds the 104-byte descriptor block that is double-hashed (forward +
    // reverse CRC-16) to produce the 32-bit program key. Field names below
    // mirror the Ghidra block layout but are documented by purpose.
    // ---------------------------------------------------------------------------

    extern "C" void NuShaderObjectKeyGenerate3(u32 *outKey, const ::ShaderMtlDescFilter *filter, i32 pixelStage) {
        const NUSHADERMTLDESC *desc = filter->desc;
        const u8 *descBytes = reinterpret_cast<const u8 *>(desc);

        union KeyFlags {
            struct {
                u8 lightmapA : 1;
                u8 lightmapB : 1;
                u8 low : 4;
                u8 clearedLo : 2;
                u8 clearedHi : 1;
                u8 mid : 3;
                u8 forced : 1;
                u8 extraMapA : 1;
                u8 extraMapB : 1;
                u8 bit15 : 1;
                u8 bits16 : 4;
                u8 lightmapSelect : 1;
                u8 bits21 : 3;
                u8 byte3;
                u32 nibble : 4;
                u32 rest : 28;
            } bits;
            i32 words[2];
        };

        // Fixed 104-byte hash input; zero-initialised then sparsely populated.
        struct {
            KeyFlags flags;
            u8 block[0x68];
        } keyData;
        u8 *const block = keyData.block;
        __builtin_memset(block, 0, 0x68);
        block[0x14] = 1;

        for (i32 i = 0; i < 8; ++i)
            block[0x0c + i] = pixelStage ? 0 : uberShader2_md5[i];

        KeyFlags &flags = keyData.flags;
        if (filter->variant == 0) {
            flags = *reinterpret_cast<const KeyFlags *>(descBytes + 0x1b8);
            flags.bits.clearedLo = 0;
            flags.bits.clearedHi = 0;
        } else {
            flags.words[0] = 0;
            flags.words[1] = 0;
            flags.bits.forced = 1;
        }

        // Filtered flag word (bytes 0x04..0x07, nibble at 0x08, copy at 0x09..0x0b).
        const i32 word = flags.words[0];
        i32 byte0 = word;
        i32 byte1 = (word << 16) >> 24;
        i32 byte2 = (word << 8) >> 24;
        i32 byte3 = word >> 24;
        block[0x04] = static_cast<u8>(byte0);
        block[0x05] = static_cast<u8>(byte1);
        block[0x06] = static_cast<u8>(byte2);
        block[0x07] = static_cast<u8>(byte3);
        block[0x08] = static_cast<u8>(flags.bits.nibble);
        block[0x09] = static_cast<u8>(byte0);
        block[0x0a] = static_cast<u8>(byte1);
        block[0x0b] = static_cast<u8>(byte2);

        // Vertex flags
        const i32 vtxFlags = filter->getVertexFlags();
        block[0x00] = static_cast<u8>(vtxFlags);
        block[0x01] = static_cast<u8>(vtxFlags >> 8);
        block[0x02] = static_cast<u8>(vtxFlags >> 16);
        block[0x03] = static_cast<u8>(static_cast<u32>(vtxFlags) >> 24);

        // Filter-derived booleans
        block[0x21] = filter->field_0x10 != 0;
        block[0x22] = filter->field_0x14 != 0;
        block[0x23] = filter->field_0x18 != 0;

        // Auxiliary texture ids: selected only when the base flag / threshold says
        // the texture is actually bound.
        u8 specularId;
        if (__builtin_expect(filter->variant == 0, 0)) {
            const KeyFlags *descFlags = reinterpret_cast<const KeyFlags *>(&filter->desc->flagsbits_1b8);
            if (__builtin_expect(descFlags->bits.lightmapA, 1)) {
                block[0x15] = descBytes[0xa9];
            } else if (descFlags->bits.lightmapB) {
                block[0x15] = descBytes[0xa9];
            } else {
                block[0x15] = 0;
            }
            specularId = filter->desc->specular_map_tid > filter->texture_id_threshold ? descBytes[0xaa] : 0;
        } else {
            block[0x15] = 0;
            specularId = 0;
        }
        block[0x16] = specularId;

        // Diffuse-map presence + ids
        block[0x17] = static_cast<u8>(filter->hasDiffuseMap(0));
        block[0x18] = filter->hasDiffuseMap(1) ? descBytes[0xab] : 0;
        block[0x19] = filter->hasDiffuseMap(2) ? descBytes[0xac] : 0;
        block[0x1a] = filter->hasDiffuseMap(3) ? descBytes[0xad] : 0;

        // Optional extra map id (0xae)
        u8 extraMap = 0;
        if (__builtin_expect(filter->variant == 0, 1)) {
            const KeyFlags *descFlags = reinterpret_cast<const KeyFlags *>(&filter->desc->flagsbits_1b8);
            if (__builtin_expect(descFlags->bits.extraMapA, 1)) {
                extraMap = descBytes[0xae];
            } else if (descFlags->bits.extraMapB) {
                extraMap = descBytes[0xae];
            }
        }
        block[0x1b] = extraMap;

        // Blend ops for layers 1..3
        block[0x1c] = filter->hasLayer(1) ? desc->blend_op2 : 0;
        block[0x1d] = filter->hasLayer(2) ? desc->blend_op3 : 0;
        block[0x1e] = filter->hasLayer(3) ? desc->blend_op4 : 0;

        // Constants required by the original hashing contract
        block[0x1f] = 1;
        block[0x20] = 1;

        // Shadow / animation variant selectors
        if (__builtin_expect(filter->variant == 0, 0)) {
            const NUSHADERMTLDESC *d = filter->desc;
            const KeyFlags *descFlags = reinterpret_cast<const KeyFlags *>(&d->flagsbits_1b8);
            u8 v24;
            if (descFlags->bits.lightmapA && d->lightmap_tex_id[1] > filter->texture_id_threshold) {
                v24 = 2;
            } else if (descFlags->bits.lightmapB) {
                v24 = d->lightmap_tex_id[1] > filter->texture_id_threshold;
            } else {
                v24 = 0;
            }
            block[0x24] = v24;

            block[0x25] = descFlags->bits.lightmapSelect
                              ? static_cast<u8>((d->lightmap_tex_id[0] > filter->texture_id_threshold) + 1)
                              : 0;
        } else {
            block[0x24] = 0;
            block[0x25] = 0;
        }

        block[0x26] = filter->desc->field_1e4 > 0;
        block[0x27] = filter->desc->field_1e8 > 0;

        // Texcoord type heuristic over the material attribute bytes at mtl+0x40..0x42.
        struct MtlAttribBytes {
            u8 alphaMode : 4;
            u8 bits4 : 4;
            u8 bits8 : 6;
            u8 zMode : 2;
            u8 bits16 : 4;
            u8 alphaTest : 3;
            u8 bit23 : 1;
        };
        const MtlAttribBytes *attr = reinterpret_cast<const MtlAttribBytes *>(&filter->mtl->attribs);
        const i32 unitsLow = attr->alphaTest;
        const u8 vtxType = attr->alphaMode;

        bool texcoordIsLong;
        if (unitsLow == 5 || unitsLow == 3) {
            texcoordIsLong = true;
        } else {
            texcoordIsLong = (vtxType == 10) | ((unitsLow & 3) == 2);
        }
        if (__builtin_expect(attr->zMode == 0, 1) || attr->zMode == 2 || vtxType == 0 || vtxType == 10) {
            block[0x28] = texcoordIsLong;
        } else {
            block[0x28] = 0;
        }

        block[0x29] = desc->unknown_a8;

        const u32 hi = CRC16::hashInverse(block, 0x68);
        outKey[0] = hi << 16;
        const u32 lo = CRC16::hash(block, 0x68);
        outKey[0] |= lo & 0xffff;
    }

} // namespace nu2api

extern "C" void NuShaderObjectKeyGenerate2(u32 *key, const NUSHADERMTLDESC *desc, const NUMTL *material, i32 flags,
                                           i32 variant, i32 pixel_stage) {
    ShaderMtlDescFilter filter;
    filter.internalInit(desc, material, flags, variant);
    NuShaderObjectKeyGenerate3(key, &filter, pixel_stage);
}

extern "C" void NuShaderObjectKeyGenerate4(u32 *key, i32 flags, i32 selector) {
    // Serialized hash input, not a native pointer-bearing material structure.
    u8 block[0x68] __attribute__((aligned(16))) = {};
    block[0x14] = 1;
    block[0x0c] = uberShader2_md5[0];
    block[0x0d] = uberShader2_md5[1];
    block[0x0e] = uberShader2_md5[2];
    block[0x0f] = uberShader2_md5[3];
    block[0x10] = uberShader2_md5[4];
    block[0x11] = uberShader2_md5[5];
    block[0x12] = uberShader2_md5[6];
    block[0x13] = uberShader2_md5[7];
    i32 byte0 = flags;
    i32 byte1 = (flags << 16) >> 24;
    i32 byte2 = (flags << 8) >> 24;
    i32 byte3 = flags >> 24;
    i32 index = selector & 0xf;
    block[0x04] = static_cast<u8>(byte0);
    block[0x05] = static_cast<u8>(byte1);
    block[0x06] = static_cast<u8>(byte2);
    block[0x07] = static_cast<u8>(byte3);
    block[0x08] = static_cast<u8>(index);
    block[0x09] = static_cast<u8>(byte0);
    block[0x0a] = static_cast<u8>(byte1);
    block[0x0b] = static_cast<u8>(byte2);
    *key = CRC16::hashInverse(block, sizeof(block)) << 16;
    *key |= CRC16::hash(block, sizeof(block)) & 0xffff;
}

extern "C" void NuShaderObjectKeySetUberShaderHash(const u8 *hash) {
    static u8 defaultHash16[16];
    if (hash == NULL) {
        hash = defaultHash16;
    }
    for (i32 i = 0; i < 16; ++i) {
        uberShader2_md5[i] = hash[i];
    }
}
