#include "legoapi/gizmos/fx/guidelines.h"

#include "decomp.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"

struct GUIDELINEPROGRESS {
    u32 state[2];
};

static void *GuideLines_ReserveBufferSpace(void *world_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    world->guidelines = NULL;
    world->guideline_count = 0;
    if (world->current_level->max_guidelines != 0) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->guidelines = static_cast<GUIDELINE *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += world->current_level->max_guidelines * sizeof(GUIDELINE);
    }
    return world->guidelines;
}

static void GuideLines_Reset(void *world_info, void *, void *progress_data) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_info);
    if (world == NULL || world->guidelines == NULL || world->guideline_count <= 0) {
        return;
    }

    GUIDELINE *guideline = world->guidelines;
    for (i32 i = 0; i < world->guideline_count; i++, guideline++) {
        guideline->active = 1;
        guideline->visible = 1;

        if (i <= 31 && progress_data != NULL) {
            u32 mask = 1u << i;
            guideline->visible = (static_cast<GUIDELINEPROGRESS *>(progress_data)->state[1] & mask) != 0;
            guideline->active = (static_cast<GUIDELINEPROGRESS *>(progress_data)->state[0] & mask) != 0;
        }
    }
}

static void GuideLines_Draw(void *, void *, float) {
}

static void GuideLine_Activate(GIZMO *gizmo, i32 active) {
    if (gizmo != NULL) {
        static_cast<GUIDELINE *>(gizmo->object)->active = active != 0;
    }
}

static void GuideLine_SetVisibility(GIZMO *gizmo, i32 visible) {
    if (gizmo != NULL) {
        static_cast<GUIDELINE *>(gizmo->object)->visible = visible != 0;
    }
}

static i32 GuideLines_GetMaxGizmos(void *guideline) {
    WORLDINFO *world = static_cast<WORLDINFO *>(guideline);
    return world != NULL ? world->current_level->max_guidelines : 0;
}

static char *GuideLine_GetGizmoName(GIZMO *gizmo) {
    return gizmo != NULL ? static_cast<char *>(gizmo->object) : NULL;
}

static i32 GuideLine_GetOutput(GIZMO *gizmo, i32, i32) {
    GUIDELINE *guideline = static_cast<GUIDELINE *>(gizmo->object);
    return guideline->active && guideline->visible;
}

static char *GuideLine_GetOutputName(GIZMO *, i32) {
    return "Active";
}

static i32 GuideLine_GetNumOutputs(GIZMO *gizmo) {
    return 1;
}

static void GuideLines_ClearProgress(void *, void *progress_data) {
    if (progress_data != NULL) {
        GUIDELINEPROGRESS *progress = static_cast<GUIDELINEPROGRESS *>(progress_data);
        progress->state[0] = 0xffffffff;
        progress->state[1] = 0xffffffff;
    }
}

static void *GuideLines_AllocateProgressData(VARIPTR *buffer, VARIPTR *buffer_end) {
    return GizmoBufferAlloc(buffer, buffer_end, sizeof(GUIDELINEPROGRESS));
}

static void GuideLines_AddGizmos(GIZMOSYS *gizmo_sys, i32 type_id, void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    for (i32 index = 0; index < world->guideline_count; ++index) {
        if (NuStrLen(world->guidelines[index].name) != 0) {
            AddGizmo(gizmo_sys, type_id, NULL, &world->guidelines[index]);
        }
    }
}

static i32 GuideLines_Load(void *world_ptr, void *) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    if (world->guideline_count != 0) {
        return 0;
    }
    EdFileReadInt();
    world->guideline_count = EdFileReadInt();
    for (i32 index = 0; index < world->guideline_count; ++index) {
        EdFileRead(world->guidelines[index].name, sizeof(world->guidelines[index].name));
        EdFileReadNuVec(&world->guidelines[index].start_position);
        EdFileReadNuVec(&world->guidelines[index].end_position);
    }
    return 1;
}

static void GuideLines_StoreProgress(void *world_ptr, void *, void *progress_ptr) {
    WORLDINFO *world = static_cast<WORLDINFO *>(world_ptr);
    GUIDELINEPROGRESS *progress = static_cast<GUIDELINEPROGRESS *>(progress_ptr);
    if (progress == NULL) {
        return;
    }
    progress->state[0] = ~0u;
    progress->state[1] = ~0u;
    if (world == NULL || world->guidelines == NULL) {
        return;
    }
    GUIDELINE *guideline = world->guidelines;
    for (i32 index = 0; index < world->guideline_count; ++index, ++guideline) {
        if (index == 32) {
            break;
        }
        const u32 bit = 1u << index;
        if (guideline->visible == 0) {
            progress->state[(index >> 5) + 1] &= ~bit;
        }
        if (guideline->active == 0) {
            progress->state[index >> 5] &= ~bit;
        }
    }
}

GUIDELINE *GuideLine_FindNearest(nuvec_s *position, WORLDINFO_s *world, i32 *endpoint, float *distance) {
    GUIDELINE *nearest = NULL;
    f32 nearest_distance = 1000000000.0f;
    i32 nearest_endpoint = -1;
    GUIDELINE *guideline = world->guidelines;
    for (i32 index = 0; index < world->guideline_count; ++index, ++guideline) {
        f32 candidate_distance = NuVecDistSqr(position, &guideline->start_position, NULL);
        if (candidate_distance < nearest_distance) {
            nearest_distance = candidate_distance;
            nearest = guideline;
            nearest_endpoint = 0;
        }
        candidate_distance = NuVecDistSqr(position, &guideline->end_position, NULL);
        if (candidate_distance < nearest_distance) {
            nearest_distance = candidate_distance;
            nearest = guideline;
            nearest_endpoint = 1;
        }
    }
    if (endpoint != NULL) {
        *endpoint = nearest_endpoint;
    }
    if (distance != NULL) {
        *distance = nearest_distance;
    }
    return nearest;
}

ADDGIZMOTYPE *GuideLines_RegisterGizmo(i32 type_id) {
    static ADDGIZMOTYPE addtype;

    addtype = Default_ADDGIZMOTYPE;
    addtype.name = "GuideLine";
    addtype.prefix = "";
    addtype.fns.unknown1 = 8;
    addtype.fns.get_max_gizmos_fn = GuideLines_GetMaxGizmos;
    addtype.fns.add_gizmos_fn = GuideLines_AddGizmos;
    addtype.fns.early_update_fn = NULL;
    addtype.fns.late_update_fn = NULL;
    addtype.fns.draw_fn = GuideLines_Draw;
    addtype.fns.panel_draw_fn = NULL;
    addtype.fns.get_gizmo_name_fn = GuideLine_GetGizmoName;
    addtype.fns.get_output_fn = GuideLine_GetOutput;
    addtype.fns.get_output_name_fn = GuideLine_GetOutputName;
    addtype.fns.get_num_outputs_fn = GuideLine_GetNumOutputs;
    addtype.fns.activate_fn = GuideLine_Activate;
    addtype.fns.activate_rev_fn = NULL;
    addtype.fns.set_visibility_fn = GuideLine_SetVisibility;
    addtype.fns.get_visibility_fn = NULL;
    addtype.fns.get_pos_fn = NULL;
    addtype.fns.using_special_fn = NULL;
    addtype.fns.bolt_hit_plat_fn = NULL;
    addtype.fns.get_best_bolt_target_fn = NULL;
    addtype.fns.bolt_hit_fn = NULL;
    addtype.fns.allocate_progress_data_fn = GuideLines_AllocateProgressData;
    addtype.fns.clear_progress_fn = GuideLines_ClearProgress;
    addtype.fns.store_progress_fn = GuideLines_StoreProgress;
    addtype.fns.reset_fn = GuideLines_Reset;
    addtype.fns.reserve_buffer_space_fn = GuideLines_ReserveBufferSpace;
    addtype.fns.load_fn = GuideLines_Load;
    addtype.fns.post_load_fn = NULL;
    addtype.fns.add_level_sfx_fn = NULL;

    return &addtype;
}
