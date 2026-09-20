#include "legoapi/world/world_shared.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"
#include "globals.h"

#include <string.h>

struct numtl_s;
typedef struct numtl_s NUMTL;

extern "C" {

    extern NUGLOBALRNDRSTATE render_state;
    extern nurenderscene_s currentScene;

    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);

    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *matrix, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *deformer_weights);

    void AddColourPick(void) {
        STUBBED();
    }

    void *RndrStateBuildVertexGroupsStates(NURNDRSTATE *state);
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

    void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context) {
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, &visibility_context));
        const bool portal_filter = visibility != NULL && portal_special_objects != 0 &&
                                   visibility->portal_marker != NULL && portals_enabled != 0;
        const bool shadow_pass = currentScene.unknown_3c != NULL;

        if ((scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0 ||
            noscenespecials != 0) {
            return;
        }

        NUGSCN temporary_scene;
        NUGSCN *gscene = scene->gscene;
        if (gscene == NULL) {
            temporary_scene.display_list = scene;
            gscene = &temporary_scene;
        }

        nuhspecial_s special_handle;
        special_handle.scene = gscene;
        for (i32 index = 0; index < scene->nspecials; ++index) {
            NUDISPLAYSPECIAL *special = &static_cast<NUDISPLAYSPECIAL *>(scene->specials)[index];
            if ((special->flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                continue;
            }

            const i32 instance_index = special->instance_ix;
            if (shadow_pass &&
                (static_cast<u8>(scene->visibility_flags[instance_index]) & NUDL_INSTANCE_FLAG_CASTS_SHADOW) == 0) {
                continue;
            }
            if (portal_filter &&
                (static_cast<u8>(scene->visibility_flags[instance_index]) & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) ==
                    0 &&
                (visibility->portal_bits[instance_index >> 3] & (1U << (instance_index & 7))) == 0) {
                continue;
            }

            special_handle.display_special = special;
            NUMTX *draw_matrix = special->draw_mtx_ptr;
            if (draw_matrix == NULL || draw_matrix == reinterpret_cast<NUMTX *>(-1)) {
                draw_matrix = &special->draw_mtx;
            }
            NuDisplayListRndrSpecial(&special_handle, draw_matrix, 0, NULL, NULL);
        }
    }

    void FmvTimePS(void) {
        STUBBED();
    }

    void PerspectMidPoint(NUVEC *result, NUVEC *first, NUVEC *second, NUVEC *camera_position) {
        f32 first_distance = NuVecDist(camera_position, first, NULL);
        f32 second_distance = NuVecDist(camera_position, second, NULL);
        f32 ratio = first_distance / (first_distance + second_distance);
        result->x = first->x + (second->x - first->x) * ratio;
        result->y = first->y + (second->y - first->y) * ratio;
        result->z = first->z + (second->z - first->z) * ratio;
    }

    void RndrMaskScreen(void) {
        STUBBED();
    }

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

    void RndrStateUpdate(void *, NUMTL *, NUDISPLAYLISTITEM *) {
        STUBBED();
    }

    void RndrStateUpdateFx(void *, NUDISPLAYLISTITEM *) {
        STUBBED();
    }

} // extern "C"
