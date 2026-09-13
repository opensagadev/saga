// GLES2 display-list callbacks and immediate GL state — see nurndr_android.c.
#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/android/nudlist_callbacks.h"
#include "nu2api/nu3d/nurendercontext.h"

struct numtl_s;
struct nunativetex_s;

struct NuVertexFormatPS;

struct NuFaceOnTransformPacket {
    NUMTX world;
    f32 magnitude;
    NUMTX face_on;
};

struct NuFaceOnDrawPacket {
    u32 reserved;
    i32 face_count;
    u32 vertex_buffer;
    i32 first_vertex;
};

// Renderer state shared with adjacent TUs. Original ownership varies by symbol.
extern u32 g_boundShader;
struct nushaderprogram_s;
// Defined by the shader-program TU; shared with render-state consumers.
extern nushaderprogram_s *g_currentShaderProgram;
extern numtl_s *g_boundMaterial;
extern numtl_s *g_LastMtl;
extern usize g_boundVertexFormat;
extern NuVertexFormatPS *g_nuPrimVertexFormat;
extern NuVertexFormatPS *g_nuFaceOnVertexFormat;
extern NuVertexFormatPS *g_nuDebrisVertexFormat;
extern u32 g_activeAttributes;
extern u32 g_alphaRef;
extern u32 g_alphaFunc;
extern i32 g_alphaTestEnabled;
extern u32 g_lastAlphaRef;
extern u32 g_lastAlphaBlend;
extern i32 g_renderingReflection;
void NuIOS_CopyBackbufferToTexture(nunativetex_s *tex, bool depth);

void NuIOS_SetCullMode(i32 mode);
extern "C" void NuMtlSetRenderStatesPS(numtl_s *mtl);
extern "C" void NuIOS_SetVertexFormat(usize fmt);
void NuIOSDLPreWarmGeomCallback(void *arg);
void NuIOS_ResetVAODuplicateFinder();
