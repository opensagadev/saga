#include "decomp.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"

#include <string.h>

extern "C" {
    NUGLOBALRNDRSTATE render_state = {};

    static void NuRndrSetAmbientLight(NUCOLOUR3 *colour) {
        NuRndrLightingStateCurrent.ambient = *colour;
        NuRndrSetAmbientLightPS(colour);
    }
}

#include "nu2api/numath/vuvec_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"

extern "C" {
    static void RndrStateClear(NUGLOBALRNDRSTATE *state) {
        state->state.mtl = NULL;
        state->state.tex_id = -1;
        state->state.global_id = -1;
        state->state.lights_id = -1;
        state->state.camera_id = -1;
        state->state.fog_id = -1;
        state->state.konst_id = -1;
        state->state.reflection_id = -1;
    }

    void RndrStateUpdate(void *, NUMTL *, NUDISPLAYLISTITEM *) {
    }

    void RndrStateUpdateFx(void *, NUDISPLAYLISTITEM *) {
    }

    i32 NuRndrSetAmbientLightPS(const NUCOLOUR3 *colour) {
        render_state.ambient_intensity.r = colour->r;
        render_state.ambient_intensity.g = colour->g;
        render_state.ambient_intensity.b = colour->b;
        render_state.light_state = nullptr;
        render_state.state.global_id++;
        render_state.state.lights_id++;
        return 1;
    }

    i32 NuRndrSetAmbientLightSpecular(const NUCOLOUR4 *colour) {
        render_state.global_specular = colour->a;
        NuRndrSetAmbientLightPS(reinterpret_cast<const NUCOLOUR3 *>(colour));
        return 0;
    }

    i32 NuRndrSetDirectionalLightsPS(const NUVEC *dir0, const NUCOLOUR3 *colour0, const NUVEC *dir1,
                                     const NUCOLOUR3 *colour1, const NUVEC *dir2, const NUCOLOUR3 *colour2) {
        NUMTX *view = NuCameraGetViewMtx();
        render_state.light_intensity[0] = *colour0;
        render_state.light_intensity[1] = *colour1;
        render_state.light_intensity[2] = *colour2;
        NuVecMtxRotate(&render_state.light_direction[0], const_cast<NUVEC *>(dir0), view);
        NuVecMtxRotate(&render_state.light_direction[1], const_cast<NUVEC *>(dir1), view);
        NuVecMtxRotate(&render_state.light_direction[2], const_cast<NUVEC *>(dir2), view);
        NuVecNorm(&render_state.light_direction[0], &render_state.light_direction[0]);
        NuVecNorm(&render_state.light_direction[1], &render_state.light_direction[1]);
        NuVecNorm(&render_state.light_direction[2], &render_state.light_direction[2]);
        render_state.light_state = nullptr;
        render_state.state.global_id++;
        render_state.state.lights_id++;
        return 1;
    }

    void NuRndrStateSetSpecularLight(const NUMTX *matrix, const NUCOLOUR3 *colour) {
        if (matrix != nullptr) {
            render_state.specular_mtx = *matrix;
        }
        if (colour != nullptr) {
            render_state.specular_colour = *colour;
        }
        render_state.light_state = nullptr;
        render_state.state.global_id++;
        render_state.state.lights_id++;
    }

    void NuRndrStateSetSpecularLightEx(const NUVEC *direction, const NUMTX *matrix, const NUCOLOUR3 *colour) {
        render_state.specular_mtx = *matrix;
        render_state.specular_colour = *colour;
        render_state.specular_intensity = *direction;
        render_state.light_state = nullptr;
        render_state.state.global_id++;
        render_state.state.lights_id++;
    }

    i32 NuRndrStateUpdateCameraState(void) {
        NUMTX *projection = NuCameraGetProjectionMtx();
        NUMTX *view = NuCameraGetViewMtx();
        render_state.view = *view;
        render_state.proj_00 = projection->m00;
        render_state.proj_11 = projection->m11;
        render_state.proj_22 = projection->m22;
        render_state.proj_23 = projection->m23;
        render_state.proj_32 = projection->m32;
        render_state.proj_20 = projection->m20;
        render_state.proj_21 = projection->m21;
        NuVpGetPosition2(&render_state.vpx, &render_state.vpy);
        NuVpGetSize2(&render_state.vpw, &render_state.vph);
        render_state.vpx *= (f32)nurndr_pixel_width / 640.0f;
        render_state.vpw *= (f32)nurndr_pixel_width / 640.0f;
        render_state.vpy *= (f32)nurndr_pixel_height / 224.0f;
        render_state.vph *= (f32)nurndr_pixel_height / 224.0f;
        render_state.camera_state = nullptr;
        render_state.state.global_id++;
        render_state.state.camera_id++;
        return 1;
    }

    void NuRndrStateSetFogEnabled(i32 enabled) {
        render_state.fog_enabled = enabled;
        render_state.fog_state = NULL;
        ++render_state.state.global_id;
        ++render_state.state.fog_id;
    }

    i32 NuRndrStateGetFogEnabled(void) {
        return render_state.fog_enabled;
    }

    void NuRndrStateSetFogState(f32 near_distance, f32 far_distance, u32 colour, f32 density) {
        render_state.fog_near = near_distance;
        render_state.fog_far = far_distance;
        render_state.fog_rgba = colour;
        render_state.fog_density = density;
        render_state.fog_state = NULL;
        ++render_state.state.global_id;
        ++render_state.state.fog_id;
    }

    NULIGHTSTATE *RndrStateBuildLightState(NUGLOBALRNDRSTATE *state) {
        i32 i;
        f32 alpha = 1.0f;
        VARIPTR *buffer = NuDisplayListGetBuffer();
        NULIGHTSTATE *packet = (NULIGHTSTATE *)buffer->void_ptr;
        buffer->u8_ptr += sizeof(NULIGHTSTATE);
        packet->ambient_intensity.r = state->ambient_intensity.r;
        packet->ambient_intensity.g = state->ambient_intensity.g;
        packet->ambient_intensity.b = state->ambient_intensity.b;
        packet->ambient_intensity.a = 1.0f;
        for (i = 0; i < 3; ++i) {
            packet->light_intensity[i].r = state->light_intensity[i].r;
            packet->light_intensity[i].g = state->light_intensity[i].g;
            packet->light_intensity[i].b = state->light_intensity[i].b;
            packet->light_intensity[i].a = 1.0f;
            packet->light_direction[i].x = state->light_direction[i].x;
            packet->light_direction[i].y = state->light_direction[i].y;
            packet->light_direction[i].z = state->light_direction[i].z;
            packet->light_direction[i].w = 1.0f;
        }
        packet->specular_mtx = state->specular_mtx;
        packet->specular_colour = state->specular_colour;
        packet->specular_intensity = render_state.specular_intensity;
        return packet;
    }
}

void *RndrStateBuildKonstState(nuglobalrndrstate_s *state) {
    VARIPTR *buffer = NuDisplayListGetBuffer();
    f32 *konst = static_cast<f32 *>(buffer->void_ptr);
    f32 *result = konst;

    if (state->const_tint_enabled == 0) {
        konst[0] = 1.0f;
        konst[1] = 1.0f;
        konst[2] = 1.0f;
    } else {
        konst[0] = state->const_tint.r;
        konst[1] = state->const_tint.g;
        konst[2] = state->const_tint.b;
    }
    konst[3] = state->const_alpha_enabled == 0 ? 1.0f : state->const_alpha;
    buffer->addr += sizeof(f32) * 4;
    return result;
}

void *RndrStateBuildReflectionState(nuglobalrndrstate_s *state) {
    VARIPTR *buffer = NuDisplayListGetBuffer();
    void *result = buffer->void_ptr;
    *buffer->u32_ptr++ = state->reflection;
    return result;
}

extern "C" {
    void *RndrStateBuildFogState(NUGLOBALRNDRSTATE *state) {
        VARIPTR *buffer = NuDisplayListGetBuffer();
        void *result = buffer->void_ptr;

        *buffer->u32_ptr = state->fog_enabled;
        buffer->u32_ptr++;
        if (state->fog_enabled != 0) {
            *buffer->u32_ptr = state->fog_rgba;
            buffer->u32_ptr++;
            *reinterpret_cast<f32 *>(buffer->u32_ptr) = state->fog_near;
            buffer->u32_ptr++;
            *reinterpret_cast<f32 *>(buffer->u32_ptr) = state->fog_far;
            buffer->u32_ptr++;
            *reinterpret_cast<f32 *>(buffer->u32_ptr) = state->fog_density;
            buffer->u32_ptr++;
        }
        return result;
    }

    void *RndrStateBuildVertexGroupsStates(NURNDRSTATE *) {
        f32 *output;
        i8 *input;
        i32 i;
        i32 max_groups = 128;
        VARIPTR *buffer;
        i32 vectors;
        VARIPTR packet;
        if (nuspecial_vertex_states) {
            buffer = NuDisplayListGetBuffer();
            packet = *buffer;
            *buffer->u32_ptr++ = nuspecial_vertex_states->count;
            *buffer->u32_ptr++ = nuspecial_vertex_states->flags;
            output = buffer->f32_ptr;
            vectors = (nuspecial_vertex_states->count + 3) / 4;
            buffer->u8_ptr += vectors * 16;
            input = nuspecial_vertex_states->values;
            for (i = 0; i < nuspecial_vertex_states->count; ++i) {
                *output = (f32)*input;
                ++output;
                ++input;
            }
            return packet.void_ptr;
        }
        return nullptr;
    }

    void *RndrStateBuildVertexOffsetsStates(NURNDRSTATE *) {
        VARIPTR *buffer;
        VARIPTR packet;
        if (nuspecial_vertex_noffsets > 0) {
            buffer = NuDisplayListGetBuffer();
            packet = *buffer;
            *buffer->u32_ptr++ = nuspecial_vertex_noffsets;
            memmove(buffer->void_ptr, nuspecial_vertex_offsets.void_ptr, nuspecial_vertex_noffsets * sizeof(NUVEC4));
            buffer->u8_ptr += nuspecial_vertex_noffsets * sizeof(NUVEC4);
            return packet.void_ptr;
        }
        return nullptr;
    }

    void RndrStateCopyGlobalState(NUGLOBALRNDRSTATE *state) {
        memcpy(state, &render_state, sizeof(*state));
        state->fog_state = NULL;
        state->camera_state = NULL;
        state->light_state = NULL;
        state->konst_state = NULL;
    }

    void RndrStateResetGlobalState(NUGLOBALRNDRSTATE *state) {
        state->const_alpha_enabled = 0;
        state->const_tint_enabled = 0;
        state->fog_state = NULL;
        state->light_state = NULL;
        state->camera_state = NULL;
        state->konst_state = NULL;
        state->reflection_state = NULL;
        render_state.state.global_id = 0;
        render_state.state.lights_id = 0;
        render_state.state.camera_id = 0;
        render_state.state.fog_id = 0;
        render_state.state.konst_id = 0;
        RndrStateClear(&render_state);
    }

    void RndrStateResetSharedGlobalState(void) {
        RndrStateResetGlobalState(&render_state);
    }

    void NuRndrStateInit(void) {
        memset(&render_state, 0, sizeof(render_state));
    }

    void DisplayListUpdateRenderState(NUDISPLAYLIST *dl, NUGLOBALRNDRSTATE *global) {
        if (global == nullptr) {
            return;
        }
        if (dl->state->global_id == global->state.global_id) {
            return;
        }

        if (dl->state->lights_id != global->state.lights_id) {
            if (global->light_state == nullptr) {
                global->light_state = RndrStateBuildLightState(global);
            }
            NuDisplayListLinkItem(dl, 0x94, global->light_state);
            dl->state->lights_id = global->state.lights_id;
        }

        if (dl->state->camera_id != global->state.camera_id) {
            if (global->camera_state == nullptr) {
                NUMTX projection = {};
                projection.m00 = global->proj_00;
                projection.m11 = global->proj_11;
                projection.m22 = global->proj_22;
                projection.m23 = global->proj_23;
                projection.m32 = global->proj_32;
                projection.m20 = global->proj_20;
                projection.m21 = global->proj_21;
                VARIPTR *buffer = NuDisplayListGetBuffer();
                global->camera_state = buffer->void_ptr;
                *buffer->u32_ptr++ = (global->state.camera_id + 5) * (global->state.global_id + 13) + nuapi.frame_count;
                *buffer->mtx_ptr++ = global->view;
                *buffer->mtx_ptr++ = projection;
                *buffer->f32_ptr++ = global->vpx;
                *buffer->f32_ptr++ = global->vpy;
                *buffer->f32_ptr++ = global->vpw;
                *buffer->f32_ptr++ = global->vph;
            }
            NuDisplayListLinkItem(dl, 0x9a, global->camera_state);
            dl->state->camera_id = global->state.camera_id;
        }
        if (dl->state->fog_id != global->state.fog_id) {
            if (global->fog_state == nullptr) {
                global->fog_state = RndrStateBuildFogState(global);
            }
            NuDisplayListLinkItem(dl, 0xa6, global->fog_state);
            dl->state->fog_id = global->state.fog_id;
        }
        if (dl->state->konst_id != global->state.konst_id) {
            if (global->konst_state == nullptr) {
                global->konst_state = RndrStateBuildKonstState(global);
            }
            NuDisplayListLinkItem(dl, 0xa5, global->konst_state);
            dl->state->konst_id = global->state.konst_id;
        }
        if (dl->state->reflection_id != global->state.reflection_id) {
            if (global->reflection_state == nullptr) {
                global->reflection_state = RndrStateBuildReflectionState(global);
            }
            NuDisplayListLinkItem(dl, 0xab, global->reflection_state);
            dl->state->reflection_id = global->state.reflection_id;
        }
        if (dl->state->vertex_groups_id != global->state.vertex_groups_id) {
            void *groups = RndrStateBuildVertexGroupsStates(&global->state);
            if (groups != nullptr) {
                NuDisplayListLinkItem(dl, 0xa9, groups);
            }
            dl->state->vertex_groups_id = global->state.vertex_groups_id;
        }
        dl->state->global_id = global->state.global_id;
    }

    void DisplayListUpdateRenderStateShadow(NUDISPLAYLIST *list, NURNDRSTATE *state) {
        if (!state)
            return;
        if (list->state->global_id == state->global_id)
            return;
        if (list->state->vertex_groups_id != state->vertex_groups_id) {
            void *groups = RndrStateBuildVertexGroupsStates(state);
            if (groups)
                NuDisplayListLinkItem(list, 0xa9, groups);
            list->state->vertex_groups_id = state->vertex_groups_id;
        }
        list->state->global_id = state->global_id;
    }

    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl) {
        render_state.const_alpha_mtl = mtl;
        render_state.const_alpha_enabled = alpha_enabled;
        render_state.const_alpha = alpha;
        if (tint != NULL && tint_enabled != 0) {
            render_state.const_tint = *tint;
        }
        render_state.const_tint_enabled = tint_enabled;
        render_state.konst_state = NULL;
        render_state.state.global_id++;
        render_state.state.konst_id++;
    }

    void RndrStateSetReflection(i32 reflection) {
        render_state.reflection = reflection;
        render_state.reflection_state = NULL;
        render_state.state.global_id++;
        render_state.state.reflection_id++;
    }
}
