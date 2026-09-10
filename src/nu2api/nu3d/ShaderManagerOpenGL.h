#pragma once
#include "nu2api/nu3d/nushader.h"
struct VirtualStackAllocator;
struct nushadermtldesc_s;
struct numtl_s;
struct ShaderMtlDescFilter {
    u32 getVertexFlags() const;
    bool hasDiffuseMap(i32) const;
    bool hasLayer(i32) const;
    void internalInit(nushadermtldesc_s const *, numtl_s const *, i32, i32);

    const nushadermtldesc_s *desc; // 0x00
    const numtl_s *mtl;            // 0x04
    i32 flags_in;                  // 0x08
    i32 variant;                   // 0x0c
    i32 field_0x10;
    i32 field_0x14;
    i32 field_0x18;
    i32 layer_count; // 0x1c
    i32 texture_id_threshold;
};
DECOMP_ASSERT(sizeof(ShaderMtlDescFilter) == 0x24, "ShaderMtlDescFilter ABI");
DECOMP_ASSERT(offsetof(ShaderMtlDescFilter, variant) == 0x0c, "ShaderMtlDescFilter variant offset");
DECOMP_ASSERT(offsetof(ShaderMtlDescFilter, texture_id_threshold) == 0x20,
              "ShaderMtlDescFilter texture threshold offset");

extern "C" void NuShaderObjectKeyGenerate3(u32 *, const ShaderMtlDescFilter *, i32);
struct ShaderObjectKey {
    u32 key;
};
struct nushadermtldesc_s;
enum SHADERSEMANTIC_enum : i32;
struct NuShaderObject : NUSHADEROBJECT {};

template <typename T> struct ShaderManagerTemplate {
    static f32 shininessFactor;

    T slots[400];
    i32 last_allocated;
    NUSHADEROBJECT *bound_slot;

    ShaderManagerTemplate() {
        for (i32 i = 0; i < 400; ++i)
            NuShaderObjectCreate(&slots[i]);
        ++slots[0].glsl.base.field1;
        bound_slot = nullptr;
        last_allocated = -1;
    }
    virtual ~ShaderManagerTemplate() {
        for (i32 i = 400; i != 0;)
            NuShaderObjectDestroy(&slots[--i]);
    }
    virtual bool createShader(ShaderMtlDescFilter &, bool, T *, i32) {
        return false;
    }
    virtual bool createShader(ShaderObjectKey const &, T *, i32) {
        return false;
    }
    virtual void adaptShaderMaterialForShaderVersion(nushadermtldesc_s *) {
    }
};
template <typename T> f32 ShaderManagerTemplate<T>::shininessFactor = 1.0f;

struct ShaderManagerOpenGL : ShaderManagerTemplate<NuShaderObject> {
    ShaderManagerOpenGL(VirtualStackAllocator &);
    virtual ~ShaderManagerOpenGL();
    virtual bool createShader(ShaderMtlDescFilter &, bool, NuShaderObject *, i32);
    virtual bool createShader(ShaderObjectKey const &, NuShaderObject *, i32);
    virtual void adaptShaderMaterialForShaderVersion(nushadermtldesc_s *);
    void setElementfv(SHADERSEMANTIC_enum, i32, float const *);
    void setElementsfv(SHADERSEMANTIC_enum, i32, i32, float const *);
    void setElementsfv_transpose(SHADERSEMANTIC_enum, i32, i32, float const *);
    void setfv(SHADERSEMANTIC_enum, float const *);
};
DECOMP_ASSERT(sizeof(ShaderManagerOpenGL) == 0x4bc8c, "Shader manager size");
DECOMP_ASSERT(offsetof(ShaderManagerOpenGL, slots) == 4, "Shader manager slots offset");
DECOMP_ASSERT(offsetof(ShaderManagerOpenGL, last_allocated) == 0x4bc84, "Shader manager cursor offset");
DECOMP_ASSERT(offsetof(ShaderManagerOpenGL, bound_slot) == 0x4bc88, "Shader manager bound slot offset");
