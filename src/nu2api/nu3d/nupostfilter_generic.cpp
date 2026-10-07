// Generic post-filter implementation included by the original nu3d owner.
// Android deferred render/reset and scalar Gauss sampling retain their nucore owner.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/NuMainFilterGen.h"
#include "nu2api/nucore/NuMotionFilterGen.h"
#include "nu2api/nucore/NuMotionAccumFilterGen.h"
#include "nu2api/nucore/NuSpeedBlurFilterGen.h"
#include "nu2api/nucore/NuDeferredFilterGen.h"
#include "nu2api/nucore/nuvuvectype.hpp"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"

extern const f32 nuvec4_one[4];
extern "C" void NuShaderManagerSetfv(i32, const f32 *);

static f32 motionFactorPan = 0.01f;
static f32 motionFactorPull = -0.024f;
static f32 motionFactorPanClamp = 0.02f;

extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp) {
    motionFactorPan = pan;
    motionFactorPull = pull;
    motionFactorPanClamp = clamp;
}

nuframebuffer_s *NuPostFilterGen::blurFbo, *NuPostFilterGen::copyFbo;
nueffecttex_s *NuPostFilterGen::workTex;
nushaderprogram_s *NuPostFilterGen::copyTexProgram, *NuPostFilterGen::copyTexLodProgram;
nushaderprogram_s *NuPostFilterGen::copyTexColorDepthProgram, *NuPostFilterGen::blendTexProgram;
nushaderprogram_s *NuPostFilterGen::blur5x5Program, *NuPostFilterGen::blur7x7Program;
nushaderprogram_s *NuPostFilterGen::blurGuardProgram;

NuMainFilterGen::NuMainFilterGen() {
    dof_strength = 1.0f;
    dof_near = 1.0f;
    dof_far = 1.0f;
    dof_bias = 0;
    dof_mode = 3;
    bloom = NULL;
    dof_blur = 3.0f;
    blur_radius = 5.0f;
    blur_gain = 2.1f;
    downsample_lod = 0;
    motion_scale = 0.0f;
    motion_maximum = 0.0f;
    motion_falloff = 1.0f;
}

void NuMainFilterGen::destroyResources() {
    NuFramebufferDestroy(blur_fbo);
    blur_fbo = NULL;
    NuPostFilterGen::destroyResources();
}

void NuMainFilterGen::destroyTextureResources() {
    STUBBED();
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
    for (i32 i = 0; i < 3; ++i) {
        if (w < 128 || h < 128)
            break;
        ++downsample_lod;
        w >>= 1;
        h >>= 1;
    }
    downsample_texture = NuEffectTexCreate2D(w, h, 1, 1, 2);
    if (height < 704) {
        dof_blur -= 1.0f;
        blur_radius -= 0.85f;
        blur_gain -= 0.5f;
    }
}

void NuMainFilterGen::preprocessBlurTextures(nueffecttex_s *color, nueffecttex_s *normal) {
    i32 width = color->width;
    i16 height = color->height;
    if (!dof_enabled && !bloom_enabled)
        return;
    if (normal != NULL)
        copy(color, 1, color, 0, programs[16], normal);
    else
        copy(color, 1, color, 0, copyTexProgram, NULL);
    i32 levels = downsample_lod;
    if (dof_enabled) {
        if (width < height)
            levels = static_cast<i32>(NuLog2(static_cast<f32>(width))) - 6;
        else
            levels = static_cast<i32>(NuLog2(static_cast<f32>(height))) - 6;
    }
    blur7x7Loopback(color, 1, color, 2, 1, levels, true, 1.0f, blur7x7Program);
    if (bloom_enabled) {
        f32 threshold[4] = {bloom->threshold < 0 ? 0 : (bloom->threshold > 1 ? 1 : bloom->threshold), 0, 0, 0};
        static f32 scaleBias[4] = {1, 1, 0, 0};
        PostBindProgram(programs[17]);
        PostBlurSetVertexParam(programs[17], 0xae, threshold, 4);
        PostBlurSetVertexParam(programs[17], 0xaf, scaleBias, 4);
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
        selection = dof_bias == 1 ? (motion_blur_enabled ? 4 : 3) : (motion_blur_enabled ? 2 : 0);
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
        PostBlurSetVertexParam(program, 0x83, values, 4);
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
        PostBlurSetVertexParam(program, 0x8084, reinterpret_cast<f32 *>(&transform), 16);
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
    nueffecttex_s *normal_texture = normal->texture;
    nueffecttex_s *color_texture = color->texture;
    nueffecttex_s *depth_texture = depth->texture;
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    nueffecttex_s *depth_destination = NuFramebufferGetAttachedTex(output, 4, NULL, NULL);
    if (bloom_enabled || dof_enabled)
        preprocessBlurTextures(color_texture, normal_texture);
    if (dof_enabled || motion_blur_enabled)
        preprocessDofMotionBlur(depth_texture);
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
    g_boundShader = program != NULL ? program->program : 0;
    glUseProgram(g_boundShader);
    g_currentShaderProgram = program;
    if (dof_enabled) {
        f32 params[4] = {dof_blur, 1, 0, 0};
        f32 jitter[4] = {static_cast<f32>(lrand48()) * 4.656613e-10f, static_cast<f32>(lrand48()) * 4.656613e-10f, 0,
                         0};
        PostBlurSetVertexParam(program, 0x80, params, 4);
        PostBlurSetVertexParam(program, 0x81, jitter, 4);
    }
    if (bloom_enabled) {
        f32 params[4] = {blur_gain * bloom->intensity, bloom->blend, 0, 0};
        PostBlurSetVertexParam(program, 0x82, params, 4);
        f32 angle[4] = {bloom->intensity * bloom->near_scale, (bloom->far_scale - bloom->near_scale) * bloom->intensity,
                        bloom->near_angle / 180.0f, 180.0f / (bloom->far_angle - bloom->near_angle)};
        PostBlurSetVertexParam(program, 0x88, angle, 4);
        if (bloom->directional) {
            const f32 pi = 3.1415927f;
            f32 directional[4] = {bloom->direction_near_scale * bloom->intensity,
                                  (bloom->direction_far_scale - bloom->direction_near_scale) * bloom->intensity,
                                  (bloom->direction_near_angle * pi) / 180.0f,
                                  180.0f / ((bloom->direction_far_angle - bloom->direction_near_angle) * pi)};
            PostBlurSetVertexParam(program, 0x89, directional, 4);
            NUVEC4 direction = {bloom->direction.x, bloom->direction.y, bloom->direction.z, 1};
            NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
            direction.w = bloom->direction_bias;
            PostBlurSetVertexParam(program, 0x8a, &direction.x, 4);
        }
    }
    NuFramebufferBind(output);
    NuRenderContextSetViewport(0, 0, destination->width, destination->height);
    PostMainDrawGrid();
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

void NuPostFilterGen::blend(nueffecttex_s *, nueffecttex_s *, nuframebuffer_s *output) {
    NuFramebufferBind(output);
    PostBindProgram(blendTexProgram);
    PostDrawQuad();
}

void NuPostFilterGen::blur5x5(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                              i32 iterations, i32 levels, bool mip_chain) {
    PostBindProgram(blur5x5Program);
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
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
            VuVec scale_bias(1, 1, 0.0f / width, 0.0f / height);
            NuFramebufferAttachTex2D(blurFbo, 0, output, output_lod);
            NuFramebufferBind(blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(blur5x5Program, 0xa0, &samples[0].x, 52);
            PostBlurSetVertexParam(blur5x5Program, 0xad, &scale_bias.x, 4);
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
    {
        nushaderprogram_s *bound = program;
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            f32 dx = radius / width;
            f32 dy = radius / height;
            VuVec horizontal[7], vertical[7];
            horizontal[0] = VuVec(dx * 0.0f, 0.0f, .34f, 0.0f);
            vertical[0] = VuVec(0.0f, dy * 0.0f, .34f, 0.0f);
            horizontal[1] = VuVec(dx * 1.0f, 0.0f, .18f, 0.0f);
            vertical[1] = VuVec(0.0f, dy * 1.0f, .18f, 0.0f);
            horizontal[2] = VuVec(dx * 2.0f, 0.0f, .10f, 0.0f);
            vertical[2] = VuVec(0.0f, dy * 2.0f, .10f, 0.0f);
            horizontal[3] = VuVec(dx * 3.0f, 0.0f, .05f, 0.0f);
            vertical[3] = VuVec(0.0f, dy * 3.0f, .05f, 0.0f);
            horizontal[4] = VuVec(dx * -1.0f, 0.0f, .18f, 0.0f);
            vertical[4] = VuVec(0.0f, dy * -1.0f, .18f, 0.0f);
            horizontal[5] = VuVec(dx * -2.0f, 0.0f, .10f, 0.0f);
            vertical[5] = VuVec(0.0f, dy * -2.0f, .10f, 0.0f);
            horizontal[6] = VuVec(dx * -3.0f, 0.0f, .05f, 0.0f);
            vertical[6] = VuVec(0.0f, dy * -3.0f, .05f, 0.0f);
            VuVec bias(1, 1, 0.0f / width, 0.0f / height);
            VuVec output_bias(1, 1, 0.0f / out_width, 0.0f / out_height);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &horizontal[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &vertical[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &output_bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
}

void NuPostFilterGen::blur7x7Separate(nueffecttex_s *source, i32 source_lod, nueffecttex_s *textures, i32 first_lod,
                                      i32 iterations, i32 levels, bool mip_chain, float radius,
                                      nushaderprogram_s *program) {
    i32 guard = static_cast<i32>(ceil(radius)) * 3;
    i32 work_width = workTex->width;
    i32 work_height = workTex->height;
    {
        nushaderprogram_s *bound = program;
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    for (i32 lod = 0; lod < levels; ++lod) {
        i32 level = first_lod + lod;
        nueffecttex_s *input = level == first_lod ? source : (mip_chain ? textures : textures + level - 1);
        i32 input_lod = level == first_lod ? source_lod : (mip_chain ? level - 1 : 0);
        nueffecttex_s *output = mip_chain ? textures : textures + level;
        i32 output_lod = mip_chain ? level : 0;
        for (i32 pass = 0; pass < iterations; ++pass) {
            i32 width, height, out_width, out_height;
            NuEffectTexGetDimension(input, input_lod, &width, &height);
            NuEffectTexGetDimension(output, output_lod, &out_width, &out_height);
            f32 dx = radius / width;
            f32 dy = radius / height;
            VuVec horizontal[7] = {
                {dx * 0.0f, 0.0f, .34f, 0.0f},  {dx * 1.0f, 0.0f, .18f, 0.0f},  {dx * 2.0f, 0.0f, .10f, 0.0f},
                {dx * 3.0f, 0.0f, .05f, 0.0f},  {dx * -1.0f, 0.0f, .18f, 0.0f}, {dx * -2.0f, 0.0f, .10f, 0.0f},
                {dx * -3.0f, 0.0f, .05f, 0.0f},
            };
            VuVec vertical[7] = {
                {0.0f, dy * 0.0f, .34f, 0.0f},  {0.0f, dy * 1.0f, .18f, 0.0f},  {0.0f, dy * 2.0f, .10f, 0.0f},
                {0.0f, dy * 3.0f, .05f, 0.0f},  {0.0f, dy * -1.0f, .18f, 0.0f}, {0.0f, dy * -2.0f, .10f, 0.0f},
                {0.0f, dy * -3.0f, .05f, 0.0f},
            };
            VuVec bias(1, 1, 0.0f / width, 0.0f / height);
            VuVec output_bias;
            nueffecttex_s *work = NuPostFilterGen::workTex;
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, work, 0);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            PostBlurSetVertexParam(program, 0xa0, &horizontal[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &bias.x, 4);
            PostBlurDrawQuad();
            {
                {
                    nushaderprogram_s *bound = NuPostFilterGen::blurGuardProgram;
                    g_boundShader = bound != NULL ? bound->program : 0;
                    glUseProgram(g_boundShader);
                    g_currentShaderProgram = bound;
                }
                if (out_height < work_height) {
                    const VuVec edge(1, 0, 0, 1);
                    PostBlurSetVertexParam(NuPostFilterGen::blurGuardProgram, 0x80, &edge.x, 4);
                    NuRenderContextSetViewport(0, out_height, out_width, guard);
                    PostBlurDrawQuad();
                }
                if (out_width < work_width) {
                    const VuVec edge(0, 1, 1, 0);
                    NuRenderContextSetViewport(out_width, 0, guard, out_height);
                    PostBlurSetVertexParam(NuPostFilterGen::blurGuardProgram, 0x80, &edge.x, 4);
                    PostBlurDrawQuad();
                }
            }
            NuFramebufferResolve(0, true);
            NuFramebufferAttachTex2D(NuPostFilterGen::blurFbo, 0, output, output_lod);
            NuFramebufferBind(NuPostFilterGen::blurFbo);
            NuRenderContextSetViewport(0, 0, out_width, out_height);
            {
                nushaderprogram_s *bound = program;
                g_boundShader = bound != NULL ? bound->program : 0;
                glUseProgram(g_boundShader);
                g_currentShaderProgram = bound;
            }
            output_bias.x = static_cast<f32>(out_width) / work_width;
            output_bias.y = static_cast<f32>(out_height) / work_height;
            output_bias.z = 0.0f / work_width;
            output_bias.w = 0.0f / work_height;
            PostBlurSetVertexParam(program, 0xa0, &vertical[0].x, 28);
            PostBlurSetVertexParam(program, 0xad, &output_bias.x, 4);
            PostBlurDrawQuad();
            NuFramebufferResolve(0, true);
            input = output;
            input_lod = output_lod;
        }
    }
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
    STUBBED();
}

void NuPostFilterGen::destroyResources() {
    NuFramebufferDestroy(input_fbo);
    input_fbo = NULL;
}

void NuPostFilterGen::destroySharedResources() {
    STUBBED();
}

void NuPostFilterGen::destroySharedTextureResources() {
    STUBBED();
}

void NuPostFilterGen::initResources() {
    input_fbo = NuFramebufferCreate();
}

static inline void PostRegisterPort(NuPostDataPort &port, NuDataPortManager &manager, const char *name) {
    if (port.index >= 0)
        --port.manager->entries[port.index].references;
    port.manager = &manager;
    port.index = manager.registerPort(name, NULL);
}

void NuPostFilterGen::initSharedResources() {
    PostRegisterPort(portOutFramebuffer, resourceManager, "postEffect.outFramebuffer");
    PostRegisterPort(portColorBuffer, resourceManager, "postEffect.colorBuffer");
    PostRegisterPort(portNormalBuffer, resourceManager, "postEffect.normalBuffer");
    PostRegisterPort(portVelocityBuffer, resourceManager, "postEffect.velocityBuffer");
    PostRegisterPort(portDepthRTBuffer, resourceManager, "postEffect.depthRTBuffer");
    PostRegisterPort(portDepthBuffer, resourceManager, "postEffect.depthBuffer");
    blurFbo = NuFramebufferCreate();
    copyFbo = NuFramebufferCreate();
}

void NuPostFilterGen::initSharedTextureResources(i32 width, i32 height) {
    workTex = NuEffectTexCreate2D(width, height, 1, 0x11, 2);
}

void NuPostFilterGen::renderFrustum(numtx_s *) {
    STUBBED();
}

void NuPostFilterGen::renderQuad() {
    STUBBED();
}

void NuPostFilterGen::renderQuadGrid() {
    STUBBED();
}

__attribute__((weak)) void NuPostFilterGen::reset() {
    enabled = false;
}

__attribute__((weak)) void NuPostFilterGen::resetAll() {
}

i32 NuDataPortManager::registerPort(char const *name, void *data) {
    for (Entry *entry = entries; entry < entries + 256; ++entry) {
        if (NuStrCmp(entry->name, name) == 0) {
            entry->data = data;
            return entry - entries;
        }
    }
    for (Entry *entry = entries; entry < entries + 256; ++entry) {
        if (entry->references == 0) {
            memmove(entry->name, name, NuStrLen(name) + 1);
            entry->data = data;
            ++entry->references;
            return entry - entries;
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
#define MOTION_SAMPLE(index)                                                                                           \
    weights[index][0] = (NuPow(static_cast<f32>(index + 1) / 7.0f, falloff) - 0.5f) * ratio;                           \
    weights[index][1] = weights[index][0];                                                                             \
    weights[index][2] = weights[index][3] = 0.0f;
    MOTION_SAMPLE(0)
    MOTION_SAMPLE(1)
    MOTION_SAMPLE(2)
    MOTION_SAMPLE(3)
    MOTION_SAMPLE(4)
    MOTION_SAMPLE(5)
    MOTION_SAMPLE(6)
#undef MOTION_SAMPLE
    PostBlurSetVertexParam(programs[0], 0x808a, params, 4);
    PostBlurSetVertexParam(programs[0], 0x808b, &weights[0][0], 28);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 4;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}

NuDeferredFilterGen::NuDeferredFilterGen() {
    memset(shadow_fbos, 0, sizeof(shadow_fbos));
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

void NuDeferredFilterGen::renderStencilMask(NuDynamicLight &) {
    STUBBED();
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
    motion.x = motion.x < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.x > motionFactorPanClamp ? motionFactorPanClamp : motion.x);
    motion.y = motion.y < -motionFactorPanClamp ? -motionFactorPanClamp
                                                : (motion.y > motionFactorPanClamp ? motionFactorPanClamp : motion.y);
    memcpy(&result.xyz, &motion, sizeof(result.xyz));
    result.w = 0.0f;
}

void NuSpeedBlurFilterGen::destroyTextureResources() {
    STUBBED();
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
    nueffecttex_s *color_texture = color->texture;
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    if (texture != NULL)
        copy(texture, 0, color_texture, 0, copyTexProgram, NULL);
    PostBindProgram(programs[0]);
    PostBlurSetVertexParam(programs[0], 0x80, values.depth, 8);
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
    STUBBED();
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
    for (i32 i = 0; i < frames; ++i) {
        f32 exponent;
        memcpy(&exponent, &mode, sizeof(exponent));
        weights[i] = NuPow(static_cast<f32>(i + 1) / frames, exponent);
        sum += weights[i];
    }
    if (frames > 0) {
        f32 reciprocal_sum = 1.0f / sum;
        for (i32 i = 0; i < frames; ++i)
            weights[i] *= reciprocal_sum;
        sum = 0.0f;
        for (i32 i = 0; i < frames; ++i) {
            sum += weights[i];
            weights[i] /= sum;
        }
    }
    current_frame = current_frame + 1 == frames ? 0 : current_frame + 1;
    PostBindProgram(program);
    f32 params[4] = {weights[current_frame], 0, 0, 0};
    PostBlurSetVertexParam(program, 0x808a, params, 4);
    i32 width, height;
    NuEffectTexGetDimension(accumulation_texture, 0, &width, &height);
    NuFramebufferBind(accumulation_fbo);
    NuRenderContextSetViewport(0, 0, width, height);
    PostDrawQuad();
    NuFramebufferResolve(0, true);
    PostBindProgram(program);
    PostBlurSetVertexParam(program, 0x808a, params, 4);
    NuFramebufferBind(output);
    PostDrawQuad();
    color->texture = destination;
    color->kind = 4;
    color->enabled = true;
    color->resolved = false;
    portColorBuffer.set(color);
}
