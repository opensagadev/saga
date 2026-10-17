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

        // Fixed 104-byte hash input; zero-initialised then sparsely populated.
        u8 block[0x68] = {};
        block[0x14] = 1;

        u32 flagsWord = 0;
        u8 nibble = 0;

        for (i32 i = 0; i < 8; ++i)
            block[0x0c + i] = pixelStage ? 0 : uberShader2_md5[i];
        if (filter->variant == 0) {
            flagsWord = *reinterpret_cast<const u32 *>(descBytes + 0x1b8) & 0xfffffe3fu;
            nibble = descBytes[0x1bc];
        } else {
            flagsWord = 0x1000;
            nibble = 0;
        }

        // Filtered flag word (bytes 0x04..0x07, nibble at 0x08, copy at 0x09..0x0b).
        block[0x04] = static_cast<u8>(flagsWord);
        block[0x05] = static_cast<u8>(flagsWord >> 8);
        block[0x06] = static_cast<u8>(flagsWord >> 16);
        block[0x07] = static_cast<u8>(flagsWord >> 24);
        block[0x08] = nibble & 0xf;
        block[0x09] = block[0x04];
        block[0x0a] = block[0x05];
        block[0x0b] = block[0x06];

        // Vertex flags
        const u32 vtxFlags = filter->getVertexFlags();
        block[0x00] = static_cast<u8>(vtxFlags);
        block[0x01] = static_cast<u8>(vtxFlags >> 8);
        block[0x02] = static_cast<u8>(vtxFlags >> 16);
        block[0x03] = static_cast<u8>(vtxFlags >> 24);

        // Filter-derived booleans
        block[0x21] = filter->field_0x10 != 0;
        block[0x22] = filter->field_0x14 != 0;
        block[0x23] = filter->field_0x18 != 0;

        // Auxiliary texture ids: selected only when the base flag / threshold says
        // the texture is actually bound.
        if (__builtin_expect(filter->variant == 0, 0)) {
            const u8 baseFlag = descBytes[0x1b8];
            block[0x15] = ((baseFlag & 1) || (baseFlag & 2)) ? descBytes[0xa9] : 0;
            if (__builtin_expect(filter->texture_id_threshold < *reinterpret_cast<const i32 *>(descBytes + 0x48), 0)) {
                block[0x16] = descBytes[0xaa];
            } else {
                block[0x16] = 0;
            }
        }

        // Diffuse-map presence + ids
        block[0x17] = static_cast<u8>(filter->hasDiffuseMap(0));
        block[0x18] = filter->hasDiffuseMap(1) ? descBytes[0xab] : 0;
        block[0x19] = filter->hasDiffuseMap(2) ? descBytes[0xac] : 0;
        block[0x1a] = filter->hasDiffuseMap(3) ? descBytes[0xad] : 0;

        // Optional extra map id (0xae)
        if (__builtin_expect(filter->variant == 0, 1)) {
            const u8 b1 = descBytes[0x1b9];
            if (__builtin_expect((b1 & 0x20) || (b1 & 0x40), 0)) {
                block[0x1b] = descBytes[0xae];
            }
        }

        // Blend ops for layers 1..3
        block[0x1c] = filter->hasLayer(1) ? desc->blend_op2 : 0;
        block[0x1d] = filter->hasLayer(2) ? desc->blend_op3 : 0;
        block[0x1e] = filter->hasLayer(3) ? desc->blend_op4 : 0;

        // Constants required by the original hashing contract
        block[0x1f] = 1;
        block[0x20] = 1;

        // Shadow / animation variant selectors
        if (filter->variant == 0) {
            const i32 thresholdA = *reinterpret_cast<const i32 *>(descBytes + 0x34);
            const i32 thresholdB = *reinterpret_cast<const i32 *>(descBytes + 0x38);
            const u8 baseFlag = descBytes[0x1b8];

            u8 v24 = 0;
            if (!((baseFlag & 1) != 0 && !(filter->texture_id_threshold >= thresholdB))) {
                if ((baseFlag & 2) != 0) {
                    v24 = filter->texture_id_threshold < thresholdB;
                }
            } else {
                v24 = 2;
            }
            block[0x24] = v24;

            const u8 b2 = descBytes[0x1ba];
            if (!(b2 & 0x10)) {
                block[0x25] = 0;
            } else {
                block[0x25] = static_cast<u8>((filter->texture_id_threshold < thresholdA) + 1);
            }
        }

        block[0x26] = *reinterpret_cast<const i32 *>(descBytes + 0x1e4) > 0;
        block[0x27] = *reinterpret_cast<const i32 *>(descBytes + 0x1e8) > 0;

        // Texcoord type heuristic over the material attribute bytes at mtl+0x40..0x42.
        const u8 *mtlBytes = reinterpret_cast<const u8 *>(filter->mtl);
        const u8 unitsLow = (mtlBytes[0x42] >> 4) & 7;
        const u8 vtxType = mtlBytes[0x40] & 0xf;

        bool texcoordIsLong = false;
        if (unitsLow != 3 && unitsLow != 5) {
            texcoordIsLong = (vtxType == 10) || ((unitsLow & 3) == 2);
        } else {
            texcoordIsLong = true;
        }
        const u8 attribC0 = mtlBytes[0x41] & 0xc0;
        if (attribC0 == 0 || attribC0 == 0x80 || vtxType == 0 || vtxType == 10) {
            block[0x28] = texcoordIsLong ? 1 : 0;
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
    block[0x04] = static_cast<u8>(flags);
    block[0x05] = static_cast<u8>(flags >> 8);
    block[0x06] = static_cast<u8>(flags >> 16);
    block[0x07] = static_cast<u8>(flags >> 24);
    block[0x08] = static_cast<u8>(selector & 0xf);
    block[0x09] = block[0x04];
    block[0x0a] = block[0x05];
    block[0x0b] = block[0x06];
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
