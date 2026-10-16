#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nupostresources.h"

struct NativeEffectTexture {
    NUNATIVETEX native;
    nueffecttex_s *effect;
    i32 unknown_2c;
};
DECOMP_ASSERT(sizeof(NativeEffectTexture) == 0x30, "Native effect texture stride");
DECOMP_ASSERT(offsetof(NativeEffectTexture, effect) == 0x28, "Native effect texture link");

struct NativeEffectTexturePool {
    NativeEffectTexture entries[32];
    i32 count;
    i32 used;
    u8 unknown_608[8];

    NativeEffectTexturePool() : count(0), used(0) {}
};
DECOMP_ASSERT(sizeof(NativeEffectTexturePool) == 0x610, "Native effect texture pool size");

struct EffectTexturePool {
    nueffecttex_s entries[0x800 / sizeof(nueffecttex_s)];
    u8 unknown_7f8[8];
    i32 count;
    i32 used;
    i32 free;
    u8 unknown_80c[4];

    EffectTexturePool() : count(0), used(0), free(0) {}
};
DECOMP_ASSERT(sizeof(EffectTexturePool) == 0x810, "Effect texture pool size");

static NativeEffectTexturePool nativeTexPool;
static EffectTexturePool texturePool;

extern "C" {
    void NuEffectTexLockVP(void *buffer, void *buffer_end) {
        (void)buffer;
        (void)buffer_end;
    }
    void NuEffectTexUnlockVP(void) {
    }
    void *NuEffectTexGetLockedVP(void) {
        // Retail returns null.
        return NULL;
    }
    void *NuEffectTexCreate1D(void) {
        // Retail returns null.
        return NULL;
    }
    void NuEffectTexCreateCube(void) {
        // Retail no-op.
    }
    void NuEffectTex360Create2D_aliased(void) {
        // Retail no-op.
    }
    void NuEffectTex360Create3D_aliased(void) {
        // Retail no-op.
    }
    void NuEffectTexCreateCube_aliased(void) {
        // Retail no-op.
    }
    void NuEffectTexCreateFromNativeTex(void) {
        // Retail no-op.
    }
    void NuEffectTexMapNative(nueffecttex_s *texture) {
        // Retail no-op.
    }
    void NuEffectTexUnmapNative(void) {
        // Retail no-op.
    }
    void NuEffectTexGetDimension(nueffecttex_s *texture, i32 lod, i32 *width, i32 *height) {
        const i16 *dimensions = reinterpret_cast<const i16 *>(texture);
        *width = (dimensions[1] >> lod) & ~1;
        *height = (dimensions[2] >> lod) & ~1;
    }
    nueffecttex_s *NuEffectTexGetEffectFromNative(i32 tex_id) {
        NativeEffectTexture *texture = (NativeEffectTexture *)NuTexGetNative(tex_id);
        isize index = ((u8 *)texture - (u8 *)nativeTexPool.entries) / (isize)sizeof(NativeEffectTexture);
        if ((u32)index < 32)
            return texture->effect;
        return NULL;
    }
}
