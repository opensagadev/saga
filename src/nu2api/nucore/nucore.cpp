#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nuapi.h"

#include <new>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "nu2api/nucore/nustring.h"

#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuCopyFilter.h"
#include "nu2api/nucore/NuDataPortManager.h"
#include "nu2api/nucore/NuDeferredFilter.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/NuMainFilter.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilter.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuMotionFilter.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/NuPlatform.h"
#include "nu2api/nucore/NuPostFilter.h"
#include "nu2api/nucore/NuPostFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilter.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuVoiceAndroid.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/android/nuiosdl_gl.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"

struct VuVec {
    f32 x, y, z, w;
};
static f32 motionFactorPan = 0.01f;
static f32 motionFactorPull = -0.024f;
static f32 motionFactorPanClamp = 0.02f;

extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp) {
    motionFactorPan = pan;
    motionFactorPull = pull;
    motionFactorPanClamp = clamp;
}

u32 NuPostFilter::m_fullscreenVertexBuffer, NuPostFilter::m_fullscreenIndexBuffer;
u32 NuPostFilter::m_fullscreenVertexFormat, NuPostFilter::m_fullscreenGridVertexBuffer;
u32 NuPostFilter::m_fullscreenGridIndexBuffer;
i32 NuPostFilter::m_quadGridPrimCount;
extern u32 g_lastBoundVAO;
extern void *g_nuFullscreenVertexFormat;
extern "C" f32 g_renderContext_projection[16];
extern "C" f32 g_renderContext_view[16];
extern "C" void NuRenderContextSetViewProj(NUMTX *, NUMTX *);
extern "C" void NuFramebufferClear(u32, u32);
extern "C" f32 NuPow(f32, f32);
extern "C" f32 NuLog2(f32);

static void PostBindProgram(nushaderprogram_s *program) {
    g_boundShader = program != NULL ? program->program : 0;
    glUseProgram(g_boundShader);
    g_currentShaderProgram = program;
}

// These draw sequences are inlined into the original generic filters.
// NuPostFilterGen::renderQuad/Grid themselves are Android no-ops.
static void PostDrawQuad(bool grid = false) {
    g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER,
                 grid ? NuPostFilter::m_fullscreenGridVertexBuffer : NuPostFilter::m_fullscreenVertexBuffer);
    NuIOS_SetVertexFormat(reinterpret_cast<usize>(g_nuFullscreenVertexFormat));
    if (grid) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NuPostFilter::m_fullscreenGridIndexBuffer);
        glDrawElements(GL_TRIANGLES, NuPostFilter::m_quadGridPrimCount * 3, GL_UNSIGNED_SHORT, NULL);
    } else {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

NuDataPortManager NuPostFilterGen::resourceManager;
NuPostDataPort NuPostFilterGen::portOutFramebuffer = {-1, NULL};
NuPostDataPort NuPostFilterGen::portColorBuffer = {-1, NULL};
NuPostDataPort NuPostFilterGen::portNormalBuffer = {-1, NULL};
NuPostDataPort NuPostFilterGen::portVelocityBuffer = {-1, NULL};
NuPostDataPort NuPostFilterGen::portDepthRTBuffer = {-1, NULL};
NuPostDataPort NuPostFilterGen::portDepthBuffer = {-1, NULL};
nuframebuffer_s *NuPostFilterGen::blurFbo, *NuPostFilterGen::copyFbo;
nueffecttex_s *NuPostFilterGen::workTex;
nushaderprogram_s *NuPostFilterGen::copyTexProgram, *NuPostFilterGen::copyTexLodProgram;
nushaderprogram_s *NuPostFilterGen::copyTexColorDepthProgram, *NuPostFilterGen::blendTexProgram;
nushaderprogram_s *NuPostFilterGen::blur5x5Program, *NuPostFilterGen::blur7x7Program;
nushaderprogram_s *NuPostFilterGen::blurGuardProgram;

NuApplicationState *NuCore::m_applicationState;
NuThreadManager *NuCore::m_threadManager;

void NuCore::Initialize() {
    GetApplicationState();

    m_threadManager = new NuThreadManager();
}

NuApplicationState *NuCore::GetApplicationState(void) {
    if (m_applicationState != NULL) {
        return m_applicationState;
    }

    NuApplicationState *state = NU_ALLOC_T(NuApplicationState, 1, "", NUMEMORY_CATEGORY_NONE);
    if (state != NULL) {
        new (state) NuApplicationState();
    }

    m_applicationState = state;

    return state;
}

NuApplicationState::NuApplicationState() : status(NUAPPLICATIONSTATUS_IDLE) {
}

void NuPlatform::Destroy() {
}

void NuPlatform::Exists() {
}

NuPlatform::NuPlatform() {
}

NuPlatform::~NuPlatform() {
}

void NuCopyFilter::destroyResources() {
    NuFramebufferDestroy(copy_fbo);
    NuPostFilterGen::destroyResources();
}

void NuCopyFilter::initResources() {
    NuPostFilterGen::initResources();
    copy_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(copy_fbo, 0, workTex, 0);
    input_fbo = copy_fbo;
}

void NuCopyFilter::render(nuframebuffer_s *output) {
    nueffecttex_s *color = NuFramebufferGetAttachedTex(input_fbo, 0, NULL, NULL);
    if (copy_texture != NULL)
        copy(color, copy_texture, output);
    else
        copy(color, output);
}

void NuCopyFilter::reset() {
    input_fbo = copy_fbo;
    copy_texture = NULL;
}

void NuMainFilter::initResources() {
    NuMainFilterGen::initResources();
    programs[15] = NuShaderProgramCreateIOS(blur7x7_vx, blur7x7dof_px);
}

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
    g_lastBoundVAO = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &m_fullscreenGridVertexBuffer);
    glGenBuffers(1, &m_fullscreenGridIndexBuffer);
    extern i32 nurndr_pixel_width, nurndr_pixel_height;
    i32 rows = static_cast<i32>((static_cast<f32>(nurndr_pixel_height) / nurndr_pixel_width) * 16.0f);
    i32 size = (rows + 1) * 17 * 3 * sizeof(f32);
    f32 *grid = static_cast<f32 *>(NU_ALLOC(size, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    for (i32 y = 0; y <= rows; ++y) {
        for (i32 x = 0; x <= 16; ++x) {
            i32 i = (y * 17 + x) * 3;
            grid[i] = -1.0f + x * 0.125f;
            grid[i + 1] = 1.0f - (static_cast<f32>(y) / rows + static_cast<f32>(y) / rows);
            grid[i + 2] = 0.0f;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreenGridVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, grid, GL_STATIC_DRAW);
    NU_FREE(grid);
    u16 *indices = static_cast<u16 *>(NU_ALLOC(rows * 192, 4, 1, "", NUMEMORY_CATEGORY_NONE));
    for (i32 y = 0; y < rows; ++y) {
        for (i32 x = 0; x < 16; ++x) {
            i32 i = (y * 16 + x) * 6, v = y * 17 + x;
            indices[i] = v;
            indices[i + 1] = v + 1;
            indices[i + 2] = v + 17;
            indices[i + 3] = v + 17;
            indices[i + 4] = v + 1;
            indices[i + 5] = v + 18;
        }
    }
    g_lastBoundVAO = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fullscreenGridIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rows * 192, indices, GL_STATIC_DRAW);
    NU_FREE(indices);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nupostfilter.cpp", 0x7d);
}

void NuPostFilter::renderFrustum(numtx_s *) {
}

void NuDeviceSpecs::Exists() {
}

NuDeviceSpecs::~NuDeviceSpecs() {
}

NuDynamicLight::NuDynamicLight() {
}

void NuDynamicLight::addShadowCasterScene(nugscn_s *) {
}

void NuDynamicLight::bindShaderResources(nushaderprogram_s *) {
}

void NuDynamicLight::clone(variptr_u *, variptr_u) {
}

void NuDynamicLight::computeBoundingSpace(VuVec const *, VuMtx *) {
}

void NuDynamicLight::computeClippingPlanes(VuMtx const &, bool, VuVec &, VuVec &, VuVec &, VuVec &, VuVec &, VuVec &) {
}

void NuDynamicLight::computeFrustumCube(nucamera_s const *, VuVec *, VuVec *) {
}

void NuDynamicLight::computeLightSpace(nuvec_s *, nuvec_s *, numtx_s *, numtx_s *) {
}

void NuDynamicLight::computeShadowClippingPlanes(VuVec const &, VuVec const *, VuVec *) {
}

void NuDynamicLight::computeShadowFrustrumCapsule(VuVec const &, VuVec const *, VuVec &, VuVec &, float &) {
}

void NuDynamicLight::computeWarpEffect(NuDynamicLight::RenderSet &) {
}

void NuDynamicLight::create() {
}

void NuDynamicLight::destroy(NuDynamicLight *) {
}

void NuDynamicLight::refreshShadowTransform(NuDynamicLight::RenderSet &) {
}

void NuDynamicLight::renderShadowMap(i32, nuframebuffer_s *) {
}

void NuDynamicLight::resetGeometry() {
}

void NuDynamicLight::setCameraViewProj(numtx_s *, numtx_s *) {
}

void NuDynamicLight::setupCustomCameraFrustum(nucamera_s *, float const *, i32) {
}

void NuDynamicLight::testShadowExtrusion(VuVec const &, VuVec const &, i32) {
}

void NuDynamicLight::testShadowExtrusions(VuVec const &, VuVec const &) {
}

void NuMotionFilter::initResources() {
}

NuMainFilterGen::NuMainFilterGen() {
    dof_strength = dof_near = dof_far = 1.0f;
    dof_mode = 3;
    dof_bias = 0.0f;
    bloom = NULL;
    dof_blur = 3.0f;
    blur_radius = 5.0f;
    blur_gain = 2.1f;
    downsample_lod = 0;
    motion_scale = motion_maximum = 0.0f;
    motion_falloff = 1.0f;
}

void NuMainFilterGen::destroyResources() {
    NuFramebufferDestroy(blur_fbo);
    blur_fbo = NULL;
    NuPostFilterGen::destroyResources();
}

void NuMainFilterGen::destroyTextureResources() {
}

void NuMainFilterGen::initResources() {
    NuPostFilterGen::initResources();
    blur_fbo = NuFramebufferCreate();
    dof_enabled = bloom_enabled = motion_blur_enabled = false;
    active_filter_count = 0;
}

void NuMainFilterGen::initTextureResources(i32 width, i32 height) {
    blur_texture = NuEffectTexCreate2D(width / 2, height / 2, 2, 1, 2);
    downsample_lod = 0;
    i32 w = width, h = height;
    while (w >= 128 && h >= 128 && downsample_lod < 3) {
        w >>= 1;
        h >>= 1;
        ++downsample_lod;
    }
    downsample_texture = NuEffectTexCreate2D(w, h, 1, 1, 2);
    if (height < 704) {
        dof_blur -= 1.0f;
        blur_radius -= 0.85f;
        blur_gain -= 0.5f;
    }
}

void NuMainFilterGen::preprocessBlurTextures(nueffecttex_s *color, nueffecttex_s *normal) {
    if (!dof_enabled && !bloom_enabled)
        return;
    copy(color, 1, color, 0, normal == NULL ? copyTexProgram : programs[16], normal);
    i32 levels = downsample_lod;
    if (dof_enabled) {
        i32 size = color->width < color->height ? color->width : color->height;
        levels = static_cast<i32>(NuLog2(static_cast<f32>(size))) - 6;
    }
    blur7x7Loopback(color, 1, color, 2, 1, levels, true, 1.0f, blur7x7Program);
    if (bloom_enabled) {
        f32 threshold[4] = {bloom->threshold < 0 ? 0 : (bloom->threshold > 1 ? 1 : bloom->threshold), 0, 0, 0};
        const f32 scale_bias[4] = {1, 1, 0, 0};
        PostBindProgram(programs[17]);
        NuShaderProgramSetVertexParamfv(programs[17], 0xae, threshold, 4);
        NuShaderProgramSetVertexParamfv(programs[17], 0xaf, scale_bias, 4);
        copy(downsample_texture, 0, color, downsample_lod, programs[17], NULL);
        i32 passes = static_cast<i32>(bloom->blur_iterations);
        if (passes > 0)
            blur7x7Loopback(downsample_texture, 0, downsample_texture, 0, passes, 1, true, 1.0f, blur7x7Program);
        f32 remainder = bloom->blur_iterations - passes;
        if (remainder > 0)
            blur7x7Loopback(downsample_texture, 0, downsample_texture, 0, 1, 1, true, remainder, blur7x7Program);
    }
}

void NuMainFilterGen::preprocessDofMotionBlur(nueffecttex_s *) {
    i32 selection = 1;
    if (dof_enabled) {
        u32 bias_bits;
        memcpy(&bias_bits, &dof_bias, sizeof(bias_bits));
        selection = bias_bits == 1 ? (motion_blur_enabled ? 4 : 3) : (motion_blur_enabled ? 2 : 0);
    }
    nushaderprogram_s *program = programs[selection];
    PostBindProgram(program);
    if (dof_enabled) {
        f32 fov, aspect, near_z, far_z;
        NuMtxGetPerspectiveD3D(reinterpret_cast<NUMTX *>(g_renderContext_projection), &fov, &aspect, &near_z, &far_z);
        f32 projection_scale = far_z / (far_z - near_z);
        f32 strength_near = dof_strength * dof_near;
        f32 product = strength_near * dof_far;
        f32 span = dof_far - dof_near;
        f32 denominator = near_z * projection_scale * span;
        f32 values[4] = {product / denominator, strength_near / span - (projection_scale * product) / denominator,
                         dof_blur, 0};
        NuShaderProgramSetVertexParamfv(program, 0x83, values, 4);
    }
    if (motion_blur_enabled) {
        NUMTX bias = {0.5f, 0, 0, 0, 0, -0.5f, 0, 0, 0, 0, 1, 0, 0.5f, 0.5f, 0, 1};
        NUMTX inverse_bias, inverse_previous, transform;
        NuMtxInvH(&inverse_bias, &bias);
        NuMtxInvH(&inverse_previous, &motion_previous);
        NuMtxMulH(&transform, &inverse_bias, &inverse_previous);
        NuMtxMulH(&transform, &transform, &motion_current);
        NuMtxMulH(&transform, &transform, &bias);
        NuMtxTranspose(&transform, &transform);
        NuShaderProgramSetFragmentParamfv(program, 0x84, reinterpret_cast<f32 *>(&transform), 16);
    }
    if (dof_enabled || motion_blur_enabled) {
        i32 width, height;
        NuEffectTexGetDimension(blur_texture, 0, &width, &height);
        NuFramebufferAttachTex2D(blur_fbo, 0, blur_texture, 0);
        NuFramebufferBind(blur_fbo);
        NuRenderContextSetViewport(0, 0, width, height);
        PostDrawQuad();
        NuFramebufferResolveAll(true);
        if (dof_mode > 0)
            blur7x7Loopback(blur_texture, 0, blur_texture, 1, dof_mode, 1, true, 1.5f, programs[15]);
    }
}

void NuMainFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *normal = static_cast<NuProxyBuffer *>(portNormalBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    if (normal->texture != NULL)
        NuPostResolve(normal);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    nueffecttex_s *depth_destination = NuFramebufferGetAttachedTex(output, 4, NULL, NULL);
    if (bloom_enabled || dof_enabled)
        preprocessBlurTextures(color->texture, normal->texture);
    if (dof_enabled || motion_blur_enabled)
        preprocessDofMotionBlur(depth->texture);
    i32 selection = (dof_enabled ? 4 : 0) | (bloom_enabled ? (bloom->directional ? 1 : 2) : 0);
    nushaderprogram_s *program = NULL;
    switch (selection) {
        case 1:
            program = programs[8];
            break;
        case 2:
            program = programs[6];
            break;
        case 4:
            program = programs[5];
            break;
        case 5:
            program = programs[9];
            break;
        case 6:
            program = programs[7];
            break;
    }
    PostBindProgram(program);
    if (dof_enabled) {
        f32 params[4] = {dof_blur, 1, 0, 0};
        f32 jitter[4] = {static_cast<f32>(lrand48()) * 4.656613e-10f, static_cast<f32>(lrand48()) * 4.656613e-10f, 0,
                         0};
        NuShaderProgramSetVertexParamfv(program, 0x80, params, 4);
        NuShaderProgramSetVertexParamfv(program, 0x81, jitter, 4);
    }
    if (bloom_enabled) {
        f32 params[4] = {blur_gain * bloom->intensity, bloom->blend, 0, 0};
        NuShaderProgramSetVertexParamfv(program, 0x82, params, 4);
        f32 angle[4] = {bloom->intensity * bloom->near_scale, (bloom->far_scale - bloom->near_scale) * bloom->intensity,
                        bloom->near_angle / 180.0f, 180.0f / (bloom->far_angle - bloom->near_angle)};
        NuShaderProgramSetVertexParamfv(program, 0x88, angle, 4);
        if (bloom->directional) {
            const f32 pi = 3.1415927f;
            f32 directional[4] = {bloom->direction_near_scale * bloom->intensity,
                                  (bloom->direction_far_scale - bloom->direction_near_scale) * bloom->intensity,
                                  (bloom->direction_near_angle * pi) / 180.0f,
                                  180.0f / ((bloom->direction_far_angle - bloom->direction_near_angle) * pi)};
            NuShaderProgramSetVertexParamfv(program, 0x89, directional, 4);
            NUVEC4 direction = {bloom->direction.x, bloom->direction.y, bloom->direction.z, 1};
            NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
            direction.w = bloom->direction_bias;
            NuShaderProgramSetVertexParamfv(program, 0x8a, &direction.x, 4);
        }
    }
    NuFramebufferBind(output);
    NuRenderContextSetViewport(0, 0, destination->width, destination->height);
    PostDrawQuad(true);
    color->texture = destination;
    color->kind = 0;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
    if (depth_destination != NULL) {
        depth->texture = depth_destination;
        depth->kind = 4;
        depth->enabled = true;
        depth->resolved = false;
        portDepthBuffer.set(depth);
    }
}

void NuMainFilterGen::reset() {
    enabled = false;
    dof_enabled = false;
    bloom_enabled = false;
    motion_blur_enabled = false;
    active_filter_count = 0;
}

void NuPostFilterGen::GetSampleOffsets_GaussBlur5x5(i32 width, i32 height, VuVec *samples, float scale) {
    const i32 offsets[13][2] = {{-2, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -2}, {0, -1}, {0, 0},
                                {0, 1},  {0, 2},   {1, -1}, {1, 0},  {1, 1},  {2, 0}};
    const f32 weights[13] = {0.053990968f, 0.14676267f, 0.24197073f, 0.14676267f,  0.053990968f,
                             0.24197073f,  0.3989423f,  0.24197073f, 0.053990968f, 0.14676267f,
                             0.24197073f,  0.14676267f, 0.053990968f};
    f32 dx = 1.0f / width, dy = 1.0f / height;
    for (i32 i = 0; i < 13; ++i) {
        samples[i].x = offsets[i][0] * dx;
        samples[i].y = offsets[i][1] * dy;
        samples[i].z = weights[i] / 2.1698399f * scale;
        samples[i].w = 0.0f;
    }
}

void NuPostFilterGen::blend(nueffecttex_s *, nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(blendTexProgram);
    PostDrawQuad();
}

void NuPostFilterGen::blur5x5(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                              i32 iterations, i32 levels, bool mip_chain) {
    PostBindProgram(blur5x5Program);
    for (i32 level = first_lod; level < first_lod + levels; ++level) {
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            VuVec samples[13];
            GetSampleOffsets_GaussBlur5x5(width, height, samples, 1.0f);
            f32 scale_bias[4] = {1, 1, 0.0f / width, 0.0f / height};
            NuFramebufferAttachTex2D(blurFbo, 0, output, output_lod);
            NuFramebufferBind(blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            NuShaderProgramSetVertexParamfv(blur5x5Program, 0xa0, &samples[0].x, 52);
            NuShaderProgramSetVertexParamfv(blur5x5Program, 0xad, scale_bias, 4);
            PostDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

static void PostBlur7x7(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod, i32 iterations,
                        i32 levels, bool mip_chain, f32 radius, nushaderprogram_s *program, bool separate) {
    PostBindProgram(program);
    i32 guard = static_cast<i32>(ceil(radius)) * 3;
    for (i32 level = first_lod; level < first_lod + levels; ++level) {
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            const i32 offsets[7] = {0, 1, 2, 3, -1, -2, -3};
            const f32 weights[7] = {0.34f, 0.18f, 0.10f, 0.05f, 0.18f, 0.10f, 0.05f};
            VuVec horizontal[7], vertical[7];
            for (i32 i = 0; i < 7; ++i) {
                horizontal[i].x = (radius / width) * offsets[i];
                horizontal[i].y = 0.0f;
                vertical[i].x = 0.0f;
                vertical[i].y = (radius / height) * offsets[i];
                horizontal[i].z = vertical[i].z = weights[i];
                horizontal[i].w = vertical[i].w = 0.0f;
            }
            f32 bias[4] = {1, 1, 0.0f / width, 0.0f / height};
            nueffecttex_s *work = NuPostFilterGen::workTex;
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, separate ? work : output, separate ? 0 : output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            NuShaderProgramSetVertexParamfv(program, 0xa0, &horizontal[0].x, 28);
            NuShaderProgramSetVertexParamfv(program, 0xad, bias, 4);
            PostDrawQuad();
            if (separate) {
                PostBindProgram(NuPostFilterGen::blurGuardProgram);
                if (out_height < work->height) {
                    const f32 edge[4] = {1, 0, 0, 1};
                    NuShaderProgramSetVertexParamfv(NuPostFilterGen::blurGuardProgram, 0x80, edge, 4);
                    NuRenderContextSetViewport(0, out_height, out_width, guard);
                    PostDrawQuad();
                }
                if (out_width < work->width) {
                    const f32 edge[4] = {0, 1, 1, 0};
                    NuRenderContextSetViewport(out_width, 0, guard, out_height);
                    NuShaderProgramSetVertexParamfv(NuPostFilterGen::blurGuardProgram, 0x80, edge, 4);
                    PostDrawQuad();
                }
            }
            NuFramebufferResolve(0, true);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            if (separate)
                PostBindProgram(program);
            bias[0] = separate ? static_cast<f32>(out_width) / work->width : 1.0f;
            bias[1] = separate ? static_cast<f32>(out_height) / work->height : 1.0f;
            bias[2] = 0.0f / (separate ? work->width : out_width);
            bias[3] = 0.0f / (separate ? work->height : out_height);
            NuShaderProgramSetVertexParamfv(program, 0xa0, &vertical[0].x, 28);
            NuShaderProgramSetVertexParamfv(program, 0xad, bias, 4);
            PostDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

void NuPostFilterGen::blur7x7Loopback(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                                      i32 iterations, i32 levels, bool mip_chain, float radius,
                                      nushaderprogram_s *program) {
    PostBlur7x7(source, source_lod, textures, first_lod, iterations, levels, mip_chain, radius, program, false);
}

void NuPostFilterGen::blur7x7Separate(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                                      i32 iterations, i32 levels, bool mip_chain, float radius,
                                      nushaderprogram_s *program) {
    PostBlur7x7(source, source_lod, textures, first_lod, iterations, levels, mip_chain, radius, program, true);
}

void NuPostFilterGen::copy(nueffecttex_s *destination, i32 lod, nueffecttex_s *, i32, nushaderprogram_s *program,
                           nueffecttex_s *) {
    i32 width, height;
    NuEffectTexGetDimension(destination, lod, &width, &height);
    NuFramebufferAttachTex2D(copyFbo, 0, destination, lod);
    NuFramebufferBind(copyFbo);
    NuRenderContextSetViewport(0, 0, width, height);
    PostBindProgram(program);
    PostDrawQuad();
    NuFramebufferResolve(0, true);
}

void NuPostFilterGen::copy(nueffecttex_s *color, nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(copyTexColorDepthProgram);
    if (NuFramebufferGetWidth(output) == color->width)
        NuFramebufferGetHeight(output);
    PostDrawQuad();
}

void NuPostFilterGen::copy(nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(copyTexProgram);
    PostDrawQuad();
}

void NuPostFilterGen::copyDepth(nueffecttex_s *, nuframebuffer_s *) {
}

void NuPostFilterGen::destroyResources() {
    NuFramebufferDestroy(input_fbo);
    input_fbo = NULL;
}

void NuPostFilterGen::destroySharedResources() {
}

void NuPostFilterGen::destroySharedTextureResources() {
}

void NuPostFilterGen::initResources() {
    input_fbo = NuFramebufferCreate();
}

void NuPostFilterGen::initSharedResources() {
    NuPostDataPort *ports[] = {&portOutFramebuffer, &portColorBuffer,   &portNormalBuffer,
                               &portVelocityBuffer, &portDepthRTBuffer, &portDepthBuffer};
    const char *names[] = {"postEffect.outFramebuffer", "postEffect.colorBuffer",   "postEffect.normalBuffer",
                           "postEffect.velocityBuffer", "postEffect.depthRTBuffer", "postEffect.depthBuffer"};
    for (i32 i = 0; i < 6; ++i) {
        if (ports[i]->index >= 0)
            --ports[i]->manager->entries[ports[i]->index].references;
        ports[i]->manager = &resourceManager;
        ports[i]->index = resourceManager.registerPort(names[i], NULL);
    }
    blurFbo = NuFramebufferCreate();
    copyFbo = NuFramebufferCreate();
}

void NuPostFilterGen::initSharedTextureResources(i32 width, i32 height) {
    workTex = NuEffectTexCreate2D(width, height, 1, 0x11, 2);
}

void NuPostFilterGen::renderFrustum(numtx_s *) {
}

void NuPostFilterGen::renderQuad() {
}

void NuPostFilterGen::renderQuadGrid() {
}

__attribute__((weak)) void NuPostFilterGen::reset() {
    enabled = false;
}

__attribute__((weak)) void NuPostFilterGen::resetAll() {
}

void NuDeferredFilter::initResources() {
}

i32 NuDataPortManager::registerPort(char const *name, void *data) {
    for (i32 i = 0; i < 256; ++i) {
        if (NuStrCmp(entries[i].name, name) == 0) {
            entries[i].data = data;
            return i;
        }
    }
    for (i32 i = 0; i < 256; ++i) {
        if (entries[i].references == 0) {
            memmove(entries[i].name, name, NuStrLen(name) + 1);
            entries[i].data = data;
            ++entries[i].references;
            return i;
        }
    }
    return -1;
}

NuMotionFilterGen::NuMotionFilterGen() {
    scale = maximum = 0.0f;
    falloff = 1.0f;
}

void NuMotionFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *velocity = static_cast<NuProxyBuffer *>(portVelocityBuffer.get());
    NuPostResolve(color);
    NuPostResolve(velocity);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    PostBindProgram(programs[0]);
    f32 ratio = maximum / scale;
    f32 params[4] = {ratio, 0.5f, 0, 0};
    f32 weights[7][4];
    for (i32 i = 0; i < 7; ++i) {
        weights[i][0] = (NuPow(static_cast<f32>(i + 1) / 7.0f, falloff) - 0.5f) * ratio;
        weights[i][1] = weights[i][0];
        weights[i][2] = weights[i][3] = 0.0f;
    }
    NuShaderProgramSetFragmentParamfv(programs[0], 0x8a, params, 4);
    NuShaderProgramSetFragmentParamfv(programs[0], 0x8b, &weights[0][0], 28);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 4;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

void NuSpeedBlurFilter::initResources() {
}

void NuApplicationState::SetStatus(NUAPPLICATIONSTATUS value) {
    status = value;
}

NUAPPLICATIONSTATUS NuApplicationState::GetStatus() const {
    return status;
}

NuApplicationState::~NuApplicationState() {
}

NuDeferredFilterGen::NuDeferredFilterGen() {
    for (i32 i = 0; i < 6; ++i)
        shadow_fbos[i] = NULL;
    light_fbo = NULL;
    enabled = false;
    sample_count = 4;
    parameters[0] = 0.0f;
    parameters[1] = 0.5f;
}

void NuDeferredFilterGen::destroyResources() {
    for (i32 i = 0; i < 6; ++i)
        NuFramebufferDestroy(shadow_fbos[i]);
    NuFramebufferDestroy(light_fbo);
    NuPostFilterGen::destroyResources();
}

void NuDeferredFilterGen::destroyTextureResources() {
    for (i32 i = 0; i < 4; ++i)
        textures[i] = NULL;
}

void NuDeferredFilterGen::initResources() {
    NuPostFilterGen::initResources();
    for (i32 i = 0; i < 6; ++i)
        shadow_fbos[i] = NuFramebufferCreate();
    light_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(light_fbo, 0, textures[0], 0);
    dynamic_light_count = deferred_geometry_count = 0;
    resetAll();
}

void NuDeferredFilterGen::initTextureResources(i32 width, i32 height) {
    textures[2] = NuEffectTexCreate2D(768, 1344, 1, 1, 4);
    textures[3] = NuEffectTexCreate2D(640, 2048, 1, 1, 4);
    textures[4] = textures[5] = NULL;
    textures[0] = NuEffectTexCreate2D(width / 2, height, sample_count, 1, 2);
    textures[1] = NuEffectTexCreate2D(1, 1, 1, 0, 2);
}

void NuDeferredFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuPostResolve(color);
    NuPostResolve(static_cast<NuProxyBuffer *>(portNormalBuffer.get()));
    NuPostResolve(static_cast<NuProxyBuffer *>(portDepthRTBuffer.get()));
    NuPostResolve(static_cast<NuProxyBuffer *>(portDepthBuffer.get()));
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    bool first = true;
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        NuDynamicLight *light = dynamic_lights[i];
        const u8 *data = reinterpret_cast<const u8 *>(light);
        if (*reinterpret_cast<const i32 *>(data + 0x7bc) == 0)
            continue;
        NUMTX view, projection;
        memcpy(&view, g_renderContext_view, sizeof(view));
        memcpy(&projection, g_renderContext_projection, sizeof(projection));
        i32 shadow_count = *reinterpret_cast<const i32 *>(data + 0x6d0);
        for (i32 shadow = 0; shadow < shadow_count; ++shadow) {
            // The original selects consecutive texture members beginning at 0x30.
            nueffecttex_s *shadow_texture = textures[shadow + 2];
            NuFramebufferAttachTex2D(shadow_fbos[shadow], 4, shadow_texture, 0);
            NuFramebufferBind(shadow_fbos[shadow]);
            i32 width, height;
            NuEffectTexGetDimension(shadow_texture, 0, &width, &height);
            NuRenderContextSetViewport(0, 0, width, height);
            NuFramebufferClear(0x300, 0xffffffff);
            light->renderShadowMap(shadow, shadow_fbos[shadow]);
            NuFramebufferResolveAll(true);
        }
        NuRenderContextSetViewProj(&view, &projection);
        NuFramebufferBind(light_fbo);
        if (first)
            NuFramebufferClear(0x900, 0x00ff0000);
        i32 type = *reinterpret_cast<const i32 *>(data + 0x7b8);
        nushaderprogram_s *program = programs[type == 0 || type == 1 ? 0 : 1];
        if (!first)
            PostBindProgram(NULL);
        PostBindProgram(program);
        light->bindShaderResources(program);
        PostDrawQuad();
        NuFramebufferResolve(0, false);
        first = false;
    }
    copy(textures[0], 1, textures[0], 0, copyTexProgram, NULL);
    blur7x7Loopback(textures[0], 1, textures[0], 2, 2, sample_count - 2, true, 1.0f, blur7x7Program);
    NuFramebufferBind(output);
    i32 width = NuFramebufferGetWidth(output);
    i32 height = NuFramebufferGetHeight(output);
    NuRenderContextSetViewport(0, 0, width, height);
    PostBindProgram(programs[2]);
    f32 params[4] = {parameters[1], static_cast<f32>(sample_count) - 1.0f, parameters[2], parameters[3]};
    NuShaderProgramSetFragmentParamfv(programs[2], 0xa0, params, 4);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 0;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

void NuDeferredFilterGen::renderStencilMask(NuDynamicLight &) {
}

void NuDeferredFilterGen::resetAll() {
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        dynamic_lights[i]->resetGeometry();
    }
    dynamic_light_count = 0;
    deferred_geometry_count = 0;
}

void NuMotionAccumFilter::initResources() {
}

NuSpeedBlurFilterGen::NuSpeedBlurFilterGen() {
}

void NuSpeedBlurFilterGen::computeSpeedBlur(VuVec &result) {
    NUMTX previous_inverse, current_inverse;
    NuMtxInvH(&previous_inverse, const_cast<NUMTX *>(&parameters->previous));
    NuMtxInvH(&current_inverse, const_cast<NUMTX *>(&parameters->current));
    NUVEC4 motion;
    NuVec4MtxTransformH(&motion, reinterpret_cast<NUVEC4 *>(&previous_inverse) + 3,
                        const_cast<NUMTX *>(&parameters->current));
    if (motion.z < 0.0f) {
        NuVec4MtxTransformH(&motion, reinterpret_cast<NUVEC4 *>(&current_inverse) + 3,
                            const_cast<NUMTX *>(&parameters->previous));
        motion.x = -motion.x;
        motion.y = -motion.y;
        motion.z = -motion.z;
    }
    NuVec4Scale(&motion, &motion, 1.0f / motion.w);
    motion.x *= motionFactorPan;
    motion.y *= motionFactorPan;
    motion.z *= motionFactorPull;
    NuVec4Scale(&motion, &motion, parameters->scale);
    result.x = motion.x < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.x > motionFactorPanClamp ? motionFactorPanClamp : motion.x);
    result.y = motion.y < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.y > motionFactorPanClamp ? motionFactorPanClamp : motion.y);
    result.z = motion.z;
    result.w = 0.0f;
}

void NuSpeedBlurFilterGen::destroyTextureResources() {
}

void NuSpeedBlurFilterGen::initTextureResources(i32 width, i32 height) {
    texture = NuEffectTexCreate2D(width / 2, height / 2, 1, 1, 2);
}

void NuSpeedBlurFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    NuPostResolve(depth);
    struct {
        f32 depth[4];
        VuVec motion;
    } values;
    // The original supplies two vec4 registers; only the first two depth
    // components and the motion vector are assigned.
    values.depth[0] = 0.0f;
    values.depth[1] = parameters->scale * 100000.0f;
    computeSpeedBlur(values.motion);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    if (texture != NULL)
        copy(texture, 0, color->texture, 0, copyTexProgram, NULL);
    PostBindProgram(programs[0]);
    NuShaderProgramSetVertexParamfv(programs[0], 0x80, values.depth, 8);
    NuFramebufferBind(output);
    PostDrawQuad(true);
    color->texture = destination;
    color->kind = 0;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

f32 NuMotionAccumFilterGen::GetTiming(i32 *last_frame) {
    const f32 frame_time = nuapi.forced_frame_time;
    if (frames != 0 && frame_time != 0.0f) {
        if (blend > 1.0f) {
            blend = 1.0f;
        } else if (blend < 0.0f) {
            blend = 0.0f;
        }
        *last_frame = current_frame == frames - 1;
        return frame_time / frames;
    }
    *last_frame = 1;
    return 0.0f;
}

NuMotionAccumFilterGen::NuMotionAccumFilterGen() {
    blend = 1.0f;
    frames = mode = 0;
}

void NuMotionAccumFilterGen::destroyResources() {
    NuFramebufferDestroy(accumulation_fbo);
    accumulation_fbo = NULL;
    NuPostFilterGen::destroyResources();
}

void NuMotionAccumFilterGen::destroyTextureResources() {
}

void NuMotionAccumFilterGen::initResources() {
    NuPostFilterGen::initResources();
    accumulation_fbo = NuFramebufferCreate();
    NuFramebufferAttachTex2D(accumulation_fbo, 0, accumulation_texture, 0);
}

void NuMotionAccumFilterGen::initTextureResources(i32 width, i32 height) {
    accumulation_texture = NuEffectTexCreate2D(width, height, 1, 1, 2);
}

void NuMotionAccumFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuPostResolve(color);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    f32 weights[256], sum = 0.0f;
    f32 exponent;
    memcpy(&exponent, &mode, sizeof(exponent));
    for (i32 i = 0; i < frames; ++i) {
        weights[i] = NuPow(static_cast<f32>(i + 1) / frames, exponent);
        sum += weights[i];
    }
    f32 reciprocal_sum = 1.0f / sum;
    for (i32 i = 0; i < frames; ++i)
        weights[i] *= reciprocal_sum;
    sum = 0.0f;
    for (i32 i = 0; i < frames; ++i) {
        sum += weights[i];
        weights[i] /= sum;
    }
    current_frame = current_frame + 1 == frames ? 0 : current_frame + 1;
    PostBindProgram(program);
    f32 params[4] = {weights[current_frame], 0, 0, 0};
    NuShaderProgramSetFragmentParamfv(program, 0x8a, params, 4);
    i32 width, height;
    NuEffectTexGetDimension(accumulation_texture, 0, &width, &height);
    NuFramebufferBind(accumulation_fbo);
    NuRenderContextSetViewport(0, 0, width, height);
    PostDrawQuad();
    NuFramebufferResolve(0, true);
    PostBindProgram(program);
    NuShaderProgramSetFragmentParamfv(program, 0x8a, params, 4);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 4;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

void NuNetEmu::FindPacket(nunetaddr_s *, i32) {
}

NuNetEmu::NuNetEmu() {
}

void NuNetEmu::RecvFrom(void *, i32, nunetaddr_s &) {
}

void NuNetEmu::SendTo(void *, i32, nunetaddr_s *, i32) {
}

void NuNetEmu::SetConditions(NuNetEmu::eConditions) {
}

void NuNetEmu::SplitSendPacket(NuNetEmu::EmuPacket *) {
}

void NuNetEmu::Update() {
}
