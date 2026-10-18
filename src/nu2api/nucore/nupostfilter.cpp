#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/NuRenderDevice.h"

// NuPostFilter shared resources (retail nu3d/android/nupostfilter.cpp; the
// generic post-effect code in nu3d/android/nupostfilter.cpp is part of the
// nu3d_includes.cpp unity build).

extern u32 g_lastBoundVAO;

// Embedded Android post-filter GLSL (reference data symbols).
char default_vx[] = "precision highp float;\r\n"
                    "\r\n"
                    "//NU2API TEXTURES : 0 \r\n"
                    "//NU2API CONSTANTS : 0 \r\n"
                    "//NU2API ATTRIBS : 0 \r\n"
                    "\r\n"
                    "//#version 130\r\n"
                    "\r\n"
                    "struct VS_IN {\r\n"
                    "    vec4 _ObjPos;\r\n"
                    "};\r\n"
                    "\r\n"
                    "struct VS_OUT {\r\n"
                    "    vec4 _position;\r\n"
                    "    vec2 _Texture;\r\n"
                    "};\r\n"
                    "\r\n"
                    "struct VS_OUT2 {\r\n"
                    "    vec4 _position1;\r\n"
                    "};\r\n"
                    "\r\n"
                    "struct VS_IN3 {\r\n"
                    "    vec4 _ObjPos1;\r\n"
                    "    vec4 _color;\r\n"
                    "};\r\n"
                    "\r\n"
                    "struct VS_OUT3 {\r\n"
                    "    vec4 _position2;\r\n"
                    "    vec4 _color1;\r\n"
                    "};\r\n"
                    "\r\n"
                    "struct PS_IN3 {\r\n"
                    "    vec4 _color2;\r\n"
                    "};\r\n"
                    "\r\n"
                    "attribute vec4 cg_Vertex;\r\n"
                    "varying vec4 cg_TexCoord0;\r\n"
                    "\r\n"
                    " // main procedure, the original name was default_vx\r\n"
                    "void main()\r\n"
                    "{\r\n"
                    "\r\n"
                    "    VS_OUT _Out;\r\n"
                    "\r\n"
                    "    _Out._Texture = cg_Vertex.xy*5.00000000E-001 + 5.00000000E-001;\r\n"
                    "    cg_TexCoord0.xy = _Out._Texture;\r\n"
                    "    gl_Position = cg_Vertex;\r\n"
                    "    return;\r\n"
                    "} // main end\r\n"
                    "\r\n"
                    "\r\n"
                    "\r\n";

char copytex_px[] =
    "precision lowp float;\r\n"
    "\r\n"
    "#define sampler3D samplerCube\r\n"
    "vec4 texture(sampler2D sampler, vec2 coord, float lod) { return texture2D(sampler, coord); }\r\n"
    "vec4 texture(samplerCube sampler, vec3 coord, float lod) { return textureCube(sampler, coord); }\r\n"
    "vec4 textureLod(sampler2D sampler, vec2 coord, float lod) { return texture2D(sampler, coord); }\r\n"
    "vec4 textureLod(samplerCube sampler, vec3 coord, float lod) { return textureCube(sampler, coord); }\r\n"
    "vec4 texture(sampler2D sampler, vec2 coord) { return texture2D(sampler, coord); }\r\n"
    "vec4 texture(samplerCube sampler, vec3 coord) { return textureCube(sampler, coord); }\r\n"
    "\r\n"
    "//NU2API TEXTURES : 1 \t_diffuse_tex 0, \r\n"
    "//NU2API CONSTANTS : 0 \r\n"
    "\r\n"
    "//#version 130\r\n"
    "\r\n"
    "struct PS_IN {\r\n"
    "    vec2 _Texture;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_IN {\r\n"
    "    int dummy;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUTTHRESHOLD {\r\n"
    "    vec4 _Texture1;\r\n"
    "};\r\n"
    "\r\n"
    "struct PS_INTHRESHOLD {\r\n"
    "    vec4 _Texture2;\r\n"
    "};\r\n"
    "\r\n"
    "vec4 _ret_0;\r\n"
    "uniform sampler2D _diffuse_tex;\r\n"
    "varying vec4 cg_TexCoord0;\r\n"
    "#define cg_FragColor gl_FragColor\r\n"
    "\r\n"
    " // main procedure, the original name was copytex_px\r\n"
    "void main()\r\n"
    "{\r\n"
    "\r\n"
    "\r\n"
    "    _ret_0 = texture(_diffuse_tex, cg_TexCoord0.xy);\r\n"
    "    cg_FragColor = _ret_0;\r\n"
    "    return;\r\n"
    "} // main end\r\n"
    "\r\n"
    "\r\n"
    "\r\n";

char blur7x7_vx[] =
    "precision highp float;\r\n"
    "\r\n"
    "//NU2API TEXTURES : 0 \r\n"
    "//NU2API CONSTANTS : 2 \t_offsetWeights 160, \t_scaleBias 173, \r\n"
    "//NU2API ATTRIBS : 1 \t0 0\r\n"
    "\r\n"
    "//#version 130\r\n"
    "\r\n"
    "struct VS_IN {\r\n"
    "    vec4 _position;\r\n"
    "};\r\n"
    "\r\n"
    "struct PS_IN {\r\n"
    "    vec2 _Texture;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT5X5 {\r\n"
    "    vec4 _position1;\r\n"
    "    vec4 _offsets[7];\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT7X7 {\r\n"
    "    vec4 _position2;\r\n"
    "    vec4 _offsetWeights1[7];\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUTBLURGUARD {\r\n"
    "    vec4 _position3;\r\n"
    "    vec2 _texcoord1;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT_SM1 {\r\n"
    "    vec4 _position4;\r\n"
    "    vec2 _texcoord0;\r\n"
    "    vec2 _texcoord11;\r\n"
    "    vec2 _texcoord2;\r\n"
    "    vec2 _texcoord3;\r\n"
    "};\r\n"
    "\r\n"
    "uniform vec4 _offsetWeights[13];\r\n"
    "uniform vec4 _scaleBias;\r\n"
    "attribute vec4 cg_Vertex;\r\n"
    "varying vec4 cg_TexCoord4;\r\n"
    "varying vec4 cg_TexCoord1;\r\n"
    "varying vec4 cg_TexCoord6;\r\n"
    "varying vec4 cg_TexCoord3;\r\n"
    "varying vec4 cg_TexCoord0;\r\n"
    "varying vec4 cg_TexCoord5;\r\n"
    "varying vec4 cg_TexCoord2;\r\n"
    "\r\n"
    " // main procedure, the original name was blur7x7_vx\r\n"
    "void main()\r\n"
    "{\r\n"
    "\r\n"
    "    VS_OUT7X7 _vout;\r\n"
    "    vec2 _texcoord;\r\n"
    "\r\n"
    "    _texcoord = vec2(float((cg_Vertex.xy*vec2( 5.00000000E-001, -5.00000000E-001) + 5.00000000E-001).x), "
    "float((cg_Vertex.xy*vec2( 5.00000000E-001, -5.00000000E-001) + 5.00000000E-001).y));\r\n"
    "    _vout._offsetWeights1[0].xy = _offsetWeights[0].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[0].xy = _vout._offsetWeights1[0].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[1].xy = _offsetWeights[1].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[1].xy = _vout._offsetWeights1[1].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[2].xy = _offsetWeights[2].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[2].xy = _vout._offsetWeights1[2].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[3].xy = _offsetWeights[3].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[3].xy = _vout._offsetWeights1[3].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[4].xy = _offsetWeights[4].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[4].xy = _vout._offsetWeights1[4].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[5].xy = _offsetWeights[5].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[5].xy = _vout._offsetWeights1[5].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    _vout._offsetWeights1[6].xy = _offsetWeights[6].xy + _texcoord;\r\n"
    "    _vout._offsetWeights1[6].xy = _vout._offsetWeights1[6].xy*_scaleBias.xy + _scaleBias.zw;\r\n"
    "    cg_TexCoord2 = vec4(float(_vout._offsetWeights1[2].x), float(_vout._offsetWeights1[2].y), "
    "float(_offsetWeights[2].z), float(_offsetWeights[2].w));\r\n"
    "    cg_TexCoord5 = vec4(float(_vout._offsetWeights1[5].x), float(_vout._offsetWeights1[5].y), "
    "float(_offsetWeights[5].z), float(_offsetWeights[5].w));\r\n"
    "    cg_TexCoord0 = vec4(float(_vout._offsetWeights1[0].x), float(_vout._offsetWeights1[0].y), "
    "float(_offsetWeights[0].z), float(_offsetWeights[0].w));\r\n"
    "    cg_TexCoord3 = vec4(float(_vout._offsetWeights1[3].x), float(_vout._offsetWeights1[3].y), "
    "float(_offsetWeights[3].z), float(_offsetWeights[3].w));\r\n"
    "    cg_TexCoord6 = vec4(float(_vout._offsetWeights1[6].x), float(_vout._offsetWeights1[6].y), "
    "float(_offsetWeights[6].z), float(_offsetWeights[6].w));\r\n"
    "    cg_TexCoord1 = vec4(float(_vout._offsetWeights1[1].x), float(_vout._offsetWeights1[1].y), "
    "float(_offsetWeights[1].z), float(_offsetWeights[1].w));\r\n"
    "    cg_TexCoord4 = vec4(float(_vout._offsetWeights1[4].x), float(_vout._offsetWeights1[4].y), "
    "float(_offsetWeights[4].z), float(_offsetWeights[4].w));\r\n"
    "    gl_Position = cg_Vertex;\r\n"
    "    return;\r\n"
    "} // main end\r\n"
    "\r\n"
    "\r\n"
    "\r\n";

char blur7x7dof_px[] =
    "precision lowp float;\r\n"
    "\r\n"
    "#define sampler3D samplerCube\r\n"
    "vec4 texture(sampler2D sampler, vec2 coord, float lod) { return texture2D(sampler, coord); }\r\n"
    "vec4 texture(samplerCube sampler, vec3 coord, float lod) { return textureCube(sampler, coord); }\r\n"
    "vec4 textureLod(sampler2D sampler, vec2 coord, float lod) { return texture2D(sampler, coord); }\r\n"
    "vec4 textureLod(samplerCube sampler, vec3 coord, float lod) { return textureCube(sampler, coord); }\r\n"
    "vec4 texture(sampler2D sampler, vec2 coord) { return texture2D(sampler, coord); }\r\n"
    "vec4 texture(samplerCube sampler, vec3 coord) { return textureCube(sampler, coord); }\r\n"
    "\r\n"
    "//NU2API TEXTURES : 1 \t_diffuse_tex 0, \r\n"
    "//NU2API CONSTANTS : 0 \r\n"
    "\r\n"
    "//#version 130\r\n"
    "\r\n"
    "struct VS_IN {\r\n"
    "    int dummy;\r\n"
    "};\r\n"
    "\r\n"
    "struct PS_IN {\r\n"
    "    vec2 _Texture;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT5X5 {\r\n"
    "    vec4 _offsets[7];\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT7X7 {\r\n"
    "    vec4 _offsetWeights1[7];\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUTBLURGUARD {\r\n"
    "    vec2 _texcoord;\r\n"
    "};\r\n"
    "\r\n"
    "struct VS_OUT_SM1 {\r\n"
    "    vec2 _texcoord0;\r\n"
    "    vec2 _texcoord1;\r\n"
    "    vec2 _texcoord2;\r\n"
    "    vec2 _texcoord3;\r\n"
    "};\r\n"
    "\r\n"
    "vec2 _TMP3;\r\n"
    "vec4 _TMP2;\r\n"
    "vec4 _TMP1;\r\n"
    "VS_OUT7X7 _In1;\r\n"
    "vec2 _c0018;\r\n"
    "vec2 _c0020;\r\n"
    "uniform sampler2D _diffuse_tex;\r\n"
    "varying vec4 cg_TexCoord4;\r\n"
    "varying vec4 cg_TexCoord1;\r\n"
    "varying vec4 cg_TexCoord6;\r\n"
    "varying vec4 cg_TexCoord3;\r\n"
    "varying vec4 cg_TexCoord0;\r\n"
    "varying vec4 cg_TexCoord5;\r\n"
    "varying vec4 cg_TexCoord2;\r\n"
    "#define cg_FragColor gl_FragColor\r\n"
    "\r\n"
    " // main procedure, the original name was blur7x7dof_px\r\n"
    "void main()\r\n"
    "{\r\n"
    "\r\n"
    "    vec4 _prev;\r\n"
    "    vec4 _vColor;\r\n"
    "    vec2 _ba1;\r\n"
    "\r\n"
    "    _In1._offsetWeights1[2] = vec4(float(cg_TexCoord2.x), float(cg_TexCoord2.y), float(cg_TexCoord2.z), "
    "float(cg_TexCoord2.w));\r\n"
    "    _In1._offsetWeights1[5] = vec4(float(cg_TexCoord5.x), float(cg_TexCoord5.y), float(cg_TexCoord5.z), "
    "float(cg_TexCoord5.w));\r\n"
    "    _In1._offsetWeights1[0] = vec4(float(cg_TexCoord0.x), float(cg_TexCoord0.y), float(cg_TexCoord0.z), "
    "float(cg_TexCoord0.w));\r\n"
    "    _In1._offsetWeights1[3] = vec4(float(cg_TexCoord3.x), float(cg_TexCoord3.y), float(cg_TexCoord3.z), "
    "float(cg_TexCoord3.w));\r\n"
    "    _In1._offsetWeights1[6] = vec4(float(cg_TexCoord6.x), float(cg_TexCoord6.y), float(cg_TexCoord6.z), "
    "float(cg_TexCoord6.w));\r\n"
    "    _In1._offsetWeights1[1] = vec4(float(cg_TexCoord1.x), float(cg_TexCoord1.y), float(cg_TexCoord1.z), "
    "float(cg_TexCoord1.w));\r\n"
    "    _In1._offsetWeights1[4] = vec4(float(cg_TexCoord4.x), float(cg_TexCoord4.y), float(cg_TexCoord4.z), "
    "float(cg_TexCoord4.w));\r\n"
    "    _c0018 = vec2(float(_In1._offsetWeights1[0].x), float(_In1._offsetWeights1[0].y));\r\n"
    "    _TMP1 = texture(_diffuse_tex, _c0018);\r\n"
    "    _prev = vec4(float(_TMP1.x), float(_TMP1.y), float(_TMP1.z), float(_TMP1.w));\r\n"
    "    _vColor = _prev*vec4( 1.00000000E+000, 1.00000000E+000, 1.42822266E-001, 1.42822266E-001);\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[1].x), float(_In1._offsetWeights1[1].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[2].x), float(_In1._offsetWeights1[2].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[3].x), float(_In1._offsetWeights1[3].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[4].x), float(_In1._offsetWeights1[4].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[5].x), float(_In1._offsetWeights1[5].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _c0020 = vec2(float(_In1._offsetWeights1[6].x), float(_In1._offsetWeights1[6].y));\r\n"
    "    _TMP2 = texture(_diffuse_tex, _c0020);\r\n"
    "    _ba1 = vec2(float(_TMP2.z), float(_TMP2.w));\r\n"
    "    _vColor.zw = _vColor.zw + _ba1/7.00000000E+000;\r\n"
    "    _TMP3 = max(vec2(float(_prev.z), float(_prev.w)), vec2(float(_vColor.z), float(_vColor.w)));\r\n"
    "    _vColor.zw = vec2(float(_TMP3.x), float(_TMP3.y));\r\n"
    "    cg_FragColor = vec4(float(_vColor.x), float(_vColor.y), float(_vColor.z), float(_vColor.w));\r\n"
    "    return;\r\n"
    "} // main end\r\n"
    "\r\n"
    "\r\n"
    "\r\n";

u32 NuPostFilter::m_fullscreenVertexBuffer, NuPostFilter::m_fullscreenIndexBuffer;
u32 NuPostFilter::m_fullscreenVertexFormat, NuPostFilter::m_fullscreenGridVertexBuffer;
u32 NuPostFilter::m_fullscreenGridIndexBuffer;
i32 NuPostFilter::m_quadGridPrimCount;

void NuPostFilter::initSharedResources(i32, i32) {
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x2a);
    NuPostFilterGen::initSharedResources();
    NuPostFilterGen::copyTexProgram = NuShaderProgramCreateIOS(default_vx, copytex_px);
    NuPostFilterGen::copyTexLodProgram = NULL;
    NuPostFilterGen::blur5x5Program = NULL;
    NuPostFilterGen::blur7x7Program = NULL;
    NuPostFilterGen::blurGuardProgram = NULL;
    const f32 vertices[12] = {-1, 1, 0, -1, -1, 0, 1, 1, 0, 1, -1, 0};
    glGenBuffers(1, &m_fullscreenVertexBuffer);
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &m_fullscreenGridVertexBuffer);
    glGenBuffers(1, &m_fullscreenGridIndexBuffer);
    i32 rows = static_cast<i32>((static_cast<f32>(nurndr_pixel_height) / nurndr_pixel_width) * 16.0f);
    i32 vertex_count = (rows + 1) * 17;
    i32 triangle_count = rows * 16 * 2;
    i32 size = vertex_count * 3 * sizeof(f32);
    f32 *grid = static_cast<f32 *>(NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    f32 *position = grid;
    for (i32 y = 0; y <= rows; ++y) {
        const f32 fraction = static_cast<f32>(y) / rows;
        const f32 screen_y = 1.0f - (fraction + fraction);
        for (i32 x = 0; x <= 16; ++x) {
            position[0] = -1.0f + x * 0.125f;
            position[1] = screen_y;
            position[2] = 0.0f;
            position += 3;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenGridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, grid, GL_STATIC_DRAW);
    NU_FREE(grid);
    u16 *indices = static_cast<u16 *>(NU_ALLOC(triangle_count * 3 * sizeof(u16), 4, 1, "", NUMEMORY_CATEGORY_NONE));
    // Each row contains 16 cells, with the original two-triangle winding.
    u16 *index = indices;
    for (i32 y = 0; y < rows; ++y) {
        for (i32 x = 0; x < 16; ++x) {
            const i32 vertex = y * 17 + x;
            index[0] = vertex;
            index[1] = vertex + 1;
            index[2] = vertex + 17;
            index[3] = vertex + 17;
            index[4] = vertex + 1;
            index[5] = vertex + 18;
            index += 6;
        }
    }
    if (g_lastBoundVAO != 0)
        g_lastBoundVAO = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullscreenGridIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_count * 3 * sizeof(u16), indices, GL_STATIC_DRAW);
    NU_FREE(indices);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x7d);
}

void NuPostFilter::renderFrustum(numtx_s *) {
    STUBBED();
}
