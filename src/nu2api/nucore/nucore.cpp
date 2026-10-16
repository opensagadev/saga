#include "decomp.h"
#include <float.h>
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
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/numtl.h"
extern "C" void DisplaySceneRndrSpecials(NUDLDLISTSCENE *, i32, void *);
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nurand.h"
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
#include "gamelib/util/Utilities.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nushader.h"
#include "nu2api/nu3d/nupostdraw.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nupostshaders.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nuvuvectype.hpp"

extern const f32 nuvec4_one[4];
extern "C" void NuShaderManagerSetfv(i32, const f32 *);

extern u32 g_lastBoundVAO;
extern void *g_nuFullscreenVertexFormat;

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

void NuPostFilterGen::GetSampleOffsets_GaussBlur5x5(i32 width, i32 height, VuVec *samples, float scale) {
    f32 dx = 1.0f / width, dy = 1.0f / height;
    // Retail assigns the thirteen diamond taps before its weight-scale loop.
    // Preserve zero products and the positive two-step additions as written.
    samples[0].x = -2.0f * dx;
    samples[0].y = dy * 0.0f;
    samples[0].z = 0.053990968f;
    samples[0].w = 0.0f;
    samples[1].x = -dx;
    samples[1].y = -dy;
    samples[1].z = 0.14676267f;
    samples[1].w = 0.0f;
    samples[2].x = -dx;
    samples[2].y = dy * 0.0f;
    samples[2].z = 0.24197073f;
    samples[2].w = 0.0f;
    samples[3].x = -dx;
    samples[3].y = dy;
    samples[3].z = 0.14676267f;
    samples[3].w = 0.0f;
    samples[4].x = dx * 0.0f;
    samples[4].y = -2.0f * dy;
    samples[4].z = 0.053990968f;
    samples[4].w = 0.0f;
    samples[5].x = dx * 0.0f;
    samples[5].y = -dy;
    samples[5].z = 0.24197073f;
    samples[5].w = 0.0f;
    samples[6].x = dx * 0.0f;
    samples[6].y = dy * 0.0f;
    samples[6].z = 0.3989423f;
    samples[6].w = 0.0f;
    samples[7].x = dx * 0.0f;
    samples[7].y = dy;
    samples[7].z = 0.24197073f;
    samples[7].w = 0.0f;
    samples[8].x = dx * 0.0f;
    samples[8].y = dy + dy;
    samples[8].z = 0.053990968f;
    samples[8].w = 0.0f;
    samples[9].x = dx;
    samples[9].y = -dy;
    samples[9].z = 0.14676267f;
    samples[9].w = 0.0f;
    samples[10].x = dx;
    samples[10].y = dy * 0.0f;
    samples[10].z = 0.24197073f;
    samples[10].w = 0.0f;
    samples[11].x = dx;
    samples[11].y = dy;
    samples[11].z = 0.14676267f;
    samples[11].w = 0.0f;
    samples[12].x = dx + dx;
    samples[12].y = dy * 0.0f;
    samples[12].z = 0.053990968f;
    samples[12].w = 0.0f;
    for (i32 i = 0; i < 13; ++i) {
        samples[i].z = (samples[i].z / 2.1698399f) * scale;
    }
}

void NuDeferredFilterGen::render() {
    nuframebuffer_s *output = static_cast<nuframebuffer_s *>(portOutFramebuffer.get());
    NuProxyBuffer *normal = static_cast<NuProxyBuffer *>(portNormalBuffer.get());
    NuProxyBuffer *color = static_cast<NuProxyBuffer *>(portColorBuffer.get());
    NuProxyBuffer *depth_rt = static_cast<NuProxyBuffer *>(portDepthRTBuffer.get());
    NuProxyBuffer *depth = static_cast<NuProxyBuffer *>(portDepthBuffer.get());
    NuPostResolve(color);
    NuPostResolve(normal);
    NuPostResolve(depth_rt);
    NuPostResolve(depth);
    nueffecttex_s *destination = NuFramebufferGetAttachedTex(output, 0, NULL, NULL);
    f32 last_sample = static_cast<f32>(sample_count) - 1.0f;
    bool first = true;
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        NuDynamicLight *light = dynamic_lights[i];
        if (light->reserved_7bc == 0)
            continue;
        NUMTX view, projection;
        memcpy(&view, g_renderContext_view, sizeof(view));
        memcpy(&projection, g_renderContext_projection, sizeof(projection));
        for (i32 shadow = 0; shadow < light->active_render_set_count; ++shadow) {
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
        i32 type = light->parameter_5;
        nushaderprogram_s *program = programs[type == 0 || type == 1 ? 0 : 1];
        if (!first) {
            g_boundShader = 0;
            glUseProgram(0);
            g_currentShaderProgram = NULL;
        }
        {
            nushaderprogram_s *bound = program;
            g_boundShader = bound != NULL ? bound->program : 0;
            glUseProgram(g_boundShader);
            g_currentShaderProgram = bound;
        }
        light->bindShaderResources(program);
        PostBlurDrawQuad();
        NuFramebufferResolve(0, false);
        first = false;
    }
    copy(textures[0], 1, textures[0], 0, copyTexProgram, NULL);
    blur7x7Loopback(textures[0], 1, textures[0], 2, 2, sample_count - 2, true, 1.0f, blur7x7Program);
    NuFramebufferBind(output);
    i32 width = NuFramebufferGetWidth(output);
    i32 height = NuFramebufferGetHeight(output);
    NuRenderContextSetViewport(0, 0, width, height);
    {
        nushaderprogram_s *bound = programs[2];
        g_boundShader = bound != NULL ? bound->program : 0;
        glUseProgram(g_boundShader);
        g_currentShaderProgram = bound;
    }
    f32 params[4] = {parameters[1], last_sample, parameters[2], parameters[3]};
    PostBlurSetVertexParam(programs[2], 0x80a0, params, 4);
    PostBlurDrawQuad();
    color->kind = 0;
    color->texture = destination;
    color->resolved = false;
    color->enabled = true;
    portColorBuffer.set(color);
}

void NuDeferredFilterGen::resetAll() {
    for (i32 i = 0; i < dynamic_light_count; ++i) {
        dynamic_lights[i]->resetGeometry();
    }
    dynamic_light_count = 0;
    deferred_geometry_count = 0;
}

void NuDynamicLight::addShadowCasterScene(nugscn_s *scene) {
    if (render_set_capacity > 0) {
        RenderSet *current = render_sets;
        RenderSet *end = render_sets + render_set_capacity;
        do {
            RenderSet &set = *current;
            set.scene_cursor[0] = NULL;
            set.scene_first[0] = NULL;
            set.scene_end[0] = NULL;
            set.scene_cursor[1] = NULL;
            set.scene_first[1] = NULL;
            set.scene_end[1] = NULL;
        } while (++current != end);
    }
    NUDLDLISTSCENE *dl = scene->display_list;
    i32 instance = 0;
    for (i32 lod = 0; lod < dl->nclip_objects; ++instance, ++lod) {
        if ((dl->visibility_flags[instance] & 0x21) == 0x21) {
            if (__builtin_expect(dl->lod_ranges[lod] != 0.0f, 1)) {
                f32 dx = global_camera.mtx.m30 - dl->clip_bounds[instance].center.x;
                f32 dy = global_camera.mtx.m31 - dl->clip_bounds[instance].center.y;
                f32 dz = global_camera.mtx.m32 - dl->clip_bounds[instance].center.z;
                f32 distance = dx * dx + dy * dy + dz * dz;
                while (dl->lod_ranges[lod] > distance)
                    ++lod;
            }
            NUCLIPOBJECT *object = &dl->clip_objects[lod];
            for (i32 i = 0; i < render_set_capacity; ++i) {
                RenderSet &set = render_sets[i];
                NUPORTALBOX &box = scene->portal_boxes[instance];
                f32 sx = set.capsule_end.x - set.capsule_center.x;
                f32 sy = set.capsule_end.y - set.capsule_center.y;
                f32 sz = set.capsule_end.z - set.capsule_center.z;
                f32 radius = set.capsule_radius + box.first_w;
                f32 length = sy * sy + sx * sx + sz * sz;
                f32 ox = box.first.x - set.capsule_center.x;
                f32 oy = box.first.y - set.capsule_center.y;
                f32 oz = box.first.z - set.capsule_center.z;
                f32 dot = sx * ox + sy * oy + sz * oz;
                f32 projection = (1.0f / length) * dot;
                f32 positive = (0.0f < projection ? 1.0f : 0.0f) * projection;
                f32 t = (positive <= 1.0f ? 1.0f : 0.0f) * positive + (1.0f < positive ? 1.0f : 0.0f);
                f32 distance = (ox * ox + oy * oy + oz * oz) + (length * t) * t - (dot + dot) * t;
                if (__builtin_expect(distance > radius * radius, 0))
                    continue;
                VuVec minimum, maximum;
                minimum.x = box.first.x - box.second.x;
                minimum.z = box.first.z - box.second.z;
                minimum.y = box.first.y - box.second.y;
                maximum.x = box.first.x + box.second.x;
                maximum.y = box.first.y + box.second.y;
                maximum.z = box.first.z + box.second.z;
                if (!!!testShadowExtrusion(minimum, maximum, i)) {
                    continue;
                }
                for (i32 j = 0; j < object->nmaterials; ++j) {
                    NUMTL *material = scene->mtls[object->material_ids[j]];
                    i32 channel = material->shader_desc.unknown_1b4 != 0;
                    if ((usize)set.scene_cursor[channel] >= (usize)set.scene_end[channel]) {
                        NUDISPLAYLISTITEM *cursor =
                            (NUDISPLAYLISTITEM *)((display_list_buffer->addr + 15) & ~(usize)15);
                        set.scene_cursor[channel] = cursor;
                        display_list_buffer->addr = (usize)(cursor + 49);
                        if (set.scene_end[channel]) {
                            set.scene_end[channel]->next = cursor;
                            set.scene_end[channel]->type = 0x8d;
                            set.scene_end[channel]->id = 1;
                        } else
                            set.scene_first[channel] = cursor;
                        set.scene_end[channel] = cursor + 48;
                        material = scene->mtls[object->material_ids[j]];
                    }
                    set.scene_cursor[channel][0].id = 3;
                    set.scene_cursor[channel][0].type = 0x80;
                    set.scene_cursor[channel][0].next = material;
                    set.scene_cursor[channel][1] = dl->items[object->indices[j] - 1];
                    set.scene_cursor[channel][2] = dl->items[object->indices[j]];
                    set.scene_cursor[channel][1].id = 3;
                    set.scene_cursor[channel][2].id = 3;
                    set.scene_cursor[channel] += 3;
                }
            }
        }
        while (dl->lod_ranges[lod] != 0.0f)
            ++lod;
    }
    DisplaySceneRndrSpecials(dl, 0, NULL);
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        if (set.scene_first[0]) {
            NUDISPLAYLIST *list = &set.display_lists[0];
            NuDisplayListLinkItems(list, 1);
            NUMTL *material = scene->mtls[0];
            list->items->next = material;
            list->items->id = 3;
            list->items->type = 0x80;
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[0], set.scene_cursor[0]);
        }
        if (set.scene_first[1]) {
            NUDISPLAYLIST *list = &set.display_lists[1];
            NuDisplayListLinkItems(list, 1);
            NUMTL *material = scene->mtls[0];
            list->items->type = 0x80;
            list->items->next = material;
            list->items->id = 3;
            ++list->items;
            NuDisplayListLinkList(list, set.scene_first[1], set.scene_cursor[1]);
        }
    }
}

static inline void CloneLightVector(NUVEC4 &destination, const NUVEC4 &source) {
    destination.x = source.x;
    destination.y = source.y;
    destination.z = source.z;
    destination.w = source.w;
}
NuDynamicLight *NuDynamicLight::clone(variptr_u *arena, variptr_u) {
    NuDynamicLight *copy = reinterpret_cast<NuDynamicLight *>((arena->addr + 31) & ~usize(31));
    arena->addr = reinterpret_cast<usize>(copy) + ((sizeof(NuDynamicLight) + 31) & ~usize(31));
    RenderSet *destination_ptr = copy->render_sets;
    const RenderSet *source_ptr = render_sets;
    do {
        RenderSet &destination = *destination_ptr;
        const RenderSet &source = *source_ptr;
        destination.view = source.view;
        destination.projection = source.projection;
        destination.warp = source.warp;
        destination.shadow_transform = source.shadow_transform;
        memcpy(destination.reserved_100, source.reserved_100, sizeof(source.reserved_100));
        destination.warp_factor = source.warp_factor;
        CloneLightVector(destination.corners[0], source.corners[0]);
        CloneLightVector(destination.corners[1], source.corners[1]);
        CloneLightVector(destination.corners[2], source.corners[2]);
        CloneLightVector(destination.corners[3], source.corners[3]);
        CloneLightVector(destination.corners[4], source.corners[4]);
        CloneLightVector(destination.corners[5], source.corners[5]);
        CloneLightVector(destination.corners[6], source.corners[6]);
        CloneLightVector(destination.corners[7], source.corners[7]);
        CloneLightVector(destination.shadow_planes[0], source.shadow_planes[0]);
        CloneLightVector(destination.shadow_planes[1], source.shadow_planes[1]);
        CloneLightVector(destination.shadow_planes[2], source.shadow_planes[2]);
        CloneLightVector(destination.shadow_planes[3], source.shadow_planes[3]);
        CloneLightVector(destination.shadow_planes[4], source.shadow_planes[4]);
        CloneLightVector(destination.shadow_planes[5], source.shadow_planes[5]);
        CloneLightVector(destination.shadow_planes[6], source.shadow_planes[6]);
        CloneLightVector(destination.shadow_planes[7], source.shadow_planes[7]);
        CloneLightVector(destination.shadow_planes[8], source.shadow_planes[8]);
        CloneLightVector(destination.shadow_planes[9], source.shadow_planes[9]);
        CloneLightVector(destination.shadow_planes[10], source.shadow_planes[10]);
        CloneLightVector(destination.shadow_planes[11], source.shadow_planes[11]);
        destination.shadow_plane_count = source.shadow_plane_count;
        CloneLightVector(destination.far_plane, source.far_plane);
        CloneLightVector(destination.capsule_center, source.capsule_center);
        CloneLightVector(destination.capsule_end, source.capsule_end);
        destination.capsule_radius = source.capsule_radius;
        destination.display_lists[0] = source.display_lists[0];
        destination.display_lists[1] = source.display_lists[1];
        {
            nurndrstate_s *second = source.render_states[1];
            nurndrstate_s *first = source.render_states[0];
            destination.render_states[1] = second;
            destination.render_states[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.list_items[1];
            NUDISPLAYLISTITEM *first = source.list_items[0];
            destination.list_items[1] = second;
            destination.list_items[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.scene_cursor[1];
            NUDISPLAYLISTITEM *first = source.scene_cursor[0];
            destination.scene_cursor[1] = second;
            destination.scene_cursor[0] = first;
        }
        {
            NUDISPLAYLISTITEM *second = source.scene_first[1];
            NUDISPLAYLISTITEM *first = source.scene_first[0];
            destination.scene_first[1] = second;
            destination.scene_first[0] = first;
        }
        {
            NUDISPLAYLISTITEM *first = source.scene_end[0];
            NUDISPLAYLISTITEM *second = source.scene_end[1];
            destination.scene_end[1] = second;
            destination.scene_end[0] = first;
        }
        destination.reserved_33c[0] = source.reserved_33c[0];
        destination.reserved_33c[1] = source.reserved_33c[1];
        destination.reserved_33c[2] = source.reserved_33c[2];
        destination.reserved_33c[3] = source.reserved_33c[3];
        destination.reserved_33c[4] = source.reserved_33c[4];
        destination.reserved_33c[5] = source.reserved_33c[5];
        destination.reserved_33c[6] = source.reserved_33c[6];
        destination.reserved_33c[7] = source.reserved_33c[7];
        destination.geometry_count = source.geometry_count;
        ++source_ptr;
        ++destination_ptr;
    } while (destination_ptr != copy->render_sets + 2);
    copy->reserved_6c0[0] = reserved_6c0[0];
    copy->reserved_6c0[1] = reserved_6c0[1];
    copy->reserved_6c0[2] = reserved_6c0[2];
    copy->render_set_capacity = render_set_capacity;
    copy->active_render_set_count = active_render_set_count;
    copy->reserved_6d4[0] = reserved_6d4[0];
    copy->reserved_6d4[1] = reserved_6d4[1];
    copy->reserved_6d4[2] = reserved_6d4[2];
    copy->reserved_6d4[3] = reserved_6d4[3];
    copy->reserved_6d4[4] = reserved_6d4[4];
    copy->reserved_6d4[5] = reserved_6d4[5];
    copy->reserved_6d4[6] = reserved_6d4[6];
    copy->reserved_6d4[7] = reserved_6d4[7];
    copy->reserved_6d4[8] = reserved_6d4[8];
    copy->reserved_6d4[9] = reserved_6d4[9];
    copy->reserved_6d4[10] = reserved_6d4[10];
    copy->reserved_6d4[11] = reserved_6d4[11];
    copy->reserved_6d4[12] = reserved_6d4[12];
    copy->reserved_6d4[13] = reserved_6d4[13];
    copy->reserved_6d4[14] = reserved_6d4[14];
    copy->reserved_6d4[15] = reserved_6d4[15];
    copy->reserved_6d4[16] = reserved_6d4[16];
    copy->reserved_6d4[17] = reserved_6d4[17];
    copy->reserved_6d4[18] = reserved_6d4[18];
    copy->reserved_6d4[19] = reserved_6d4[19];
    copy->direction.x = direction.x;
    copy->direction.y = direction.y;
    copy->direction.z = direction.z;
    copy->reserved_730 = reserved_730;
    copy->view = view;
    copy->projection = projection;
    copy->parameter_4 = parameter_4;
    copy->parameter_5 = parameter_5;
    copy->reserved_7bc = reserved_7bc;
    copy->reserved_7c0[0] = reserved_7c0[0];
    copy->reserved_7c0[1] = reserved_7c0[1];
    CloneLightVector(copy->reserved_7c8, reserved_7c8);
    copy->used_on_specials = used_on_specials;
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
    return copy;
}

void NuDynamicLight::resetGeometry() {
    for (i32 i = 0; i < render_set_capacity; ++i) {
        RenderSet &set = render_sets[i];
        set.geometry_count = 0;
        NuDisplayListReset(&set.display_lists[0]);
        NUDISPLAYLISTITEM *item0 = set.display_lists[0].first;
        item0->type = 0x8d;
        item0->next = NULL;
        item0->id = 1;
        NuDisplayListReset(&set.display_lists[1]);
        NUDISPLAYLISTITEM *item1 = set.display_lists[1].first;
        item1->type = 0x8d;
        item1->id = 1;
        item1->next = NULL;
    }
}

void NuDynamicLight::renderShadowMap(i32 index, nuframebuffer_s *) {
    RenderSet &set = render_sets[index];
    if (set.display_lists[0].mtl_last == set.display_lists[0].first &&
        set.display_lists[1].mtl_last == set.display_lists[1].first && set.geometry_count == 0)
        return;
    refreshShadowTransform(set);
    NuDisplayListSetItemTable(1);
    NUMTX matrix;
    NUMTX bias = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5f, 0, 0, 0, 0.5f, 1};
    NuMtxMulH(&matrix, &set.warp, &bias);
    NuRenderContextSetViewProj(&matrix, &numtx_identity);
    if (set.display_lists[0].mtl_last != set.display_lists[0].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[0].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        if (g_renderContext_zFunc != 0) {
            glEnable(0xb71);
            glDepthMask(1);
            glDepthFunc(0x203);
        }
        g_renderContext_zFunc = 0;
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[0].first);
        set.display_lists[0].mtl_last = set.display_lists[0].first;
    }
    if (set.display_lists[1].mtl_last != set.display_lists[1].first) {
        NUDISPLAYLISTITEM *item = set.display_lists[1].mtl_last;
        item->type = 0x84;
        item->id = 4;
        item->next = NULL;
        if (g_renderContext_zFunc != 0) {
            glEnable(0xb71);
            glDepthMask(1);
            glDepthFunc(0x203);
        }
        g_renderContext_zFunc = 0;
        memcpy(g_renderContext_kTint, nuvec4_one, sizeof(g_renderContext_kTint));
        NuShaderManagerSetfv(0x44, nuvec4_one);
        NuDisplayListDrawItems(set.display_lists[1].first);
        set.display_lists[1].mtl_last = set.display_lists[1].first;
    }
    for (i32 i = 0; i < set.geometry_count; ++i)
        NuDisplayListDrawRenderScene(set.reserved_33c[i]);
    NuDisplayListSetItemTable(0);
}

void NuDynamicLight::setCameraViewProj(NUMTX *view, NUMTX *projection) {
    cacheCameraView = *view;
    cacheCameraProj = *projection;
}

NUMTX NuDynamicLight::cacheCameraView;
NUMTX NuDynamicLight::cacheCameraProj;
