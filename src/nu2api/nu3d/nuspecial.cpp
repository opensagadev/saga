#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec4.h"

extern "C" i32 nuspecial_reflection;
i32 nuspecial_draw_state;

extern "C" void NuSpecialMtl(NUMTL *material) {
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialForceMtl(NUMTL *material) {
    if (material != NULL) {
        nuspecial_draw_state |= NUSPECIAL_DRAW_FORCE_MATERIAL;
    } else {
        nuspecial_draw_state &= ~NUSPECIAL_DRAW_FORCE_MATERIAL;
    }
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialMtlMap(i32 count, NUMTL **materials) {
    if (count != 0) {
        nurndr_nforced_mtls = count;
        nuspecial_draw_state |= NUSPECIAL_DRAW_MATERIAL_MAP;
        nurndr_forced_mtl_table = materials;
        return;
    } else {
        nuspecial_draw_state &= ~NUSPECIAL_DRAW_MATERIAL_MAP;
        nurndr_forced_mtl_table = NULL;
        return;
    }
}

void NuSpecialReflection(i32 reflection) {
    nuspecial_reflection = reflection;
    RndrStateSetReflection(reflection);
}

extern "C" i32 NuSpecialGetNumSpecials(NUGSCN *scene) {
    i32 count = scene->numspecial;
    if (count == 0 && scene->display_list != NULL) {
        count = scene->display_list->nspecials;
    }
    return count;
}

extern "C" i32 NuSpecialGetFirst(NUGSCN *scene, nuhspecial_s *special, i32 flags) {
    if (scene->specials != NULL) {
        special->scene = scene;
        special->special = scene->specials;
        special->display_special = NULL;
        return 1;
    }
    if (scene->display_list->nspecials != 0) {
        special->scene = scene;
        special->special = NULL;
        special->display_special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
        return 1;
    }
    special->scene = NULL;
    special->special = NULL;
    special->display_special = NULL;
    return 0;
}

extern "C" void NuSpecialGetNext(nuhspecial_s *special) {
    if (special->special != NULL) {
        special->special = static_cast<u8 *>(special->special) + 0x4c;
    } else if (special->display_special != NULL) {
        ++special->display_special;
    }
}

namespace {

    struct NuLegacyInstanceLayout {
        u8 pad_00[0x44];
        union {
            u8 flags;
            struct {
                u8 visible : 1;
                u8 on_screen : 1;
                u8 unused_flag_2 : 1;
                u8 no_visibility_test : 1;
                u8 unused_flags_4_7 : 4;
            };
        };
        u8 pad_45[3];
        nuinstanim_s *animation;
    };

    DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, instance) == 0x40, "legacy special instance offset");
    DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, name) == 0x44, "legacy special name offset");
    DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, flags) == 0x48, "legacy special flags offset");
    DECOMP_ASSERT(offsetof(NuLegacyInstanceLayout, flags) == 0x44, "legacy instance flags offset");
    DECOMP_ASSERT(offsetof(NuLegacyInstanceLayout, animation) == 0x48, "legacy instance animation offset");
} // namespace

extern "C" i32 NuGScnNumSpecials(NUGSCN *scene) {
    if (scene->display_list != NULL)
        return scene->display_list->nspecials;
    return scene->numspecial;
}

extern "C" void NuGScnGetSpecial(nuhspecial_s *special, NUGSCN *scene, i32 index) {
    if (scene != NULL && special != NULL) {
        if (scene->display_list != NULL && index < scene->display_list->nspecials) {
            special->scene = scene;
            special->special = NULL;
            special->display_special = &static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials)[index];
            return;
        }
        if (index < scene->numspecial) {
            special->scene = scene;
            special->special = &reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials)[index];
            special->display_special = NULL;
            return;
        }
    }
    special->scene = NULL;
    special->special = NULL;
    special->display_special = NULL;
}

extern "C" i32 NuSpecialExistsFn(void *special_ptr) {
    if (special_ptr != NULL) {
        nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
        if (special->special != NULL) {
            return 1;
        }
        if (special->display_special != NULL) {
            return 1;
        }
    }
    return 0;
}

extern "C" nuinstanim_s *NuSpecialGetInstAnim(nuhspecial_s *special) {
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->animation;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL && display->instance_animation != reinterpret_cast<nuinstanim_s *>(-1)) {
        return display->instance_animation;
    }
    return NULL;
}

extern "C" NUMTX *NuSpecialGetInstanceMtx(nuhspecial_s *special) {
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        return static_cast<NUMTX *>(legacy->instance);
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL) {
        return &display->draw_mtx;
    }
    return NULL;
}

extern "C" i32 NuSpecialTestAnim(nuhspecial_s *special) {
    if (special->display_special != NULL) {
        nuinstanim_s *animation = special->display_special->instance_animation;
        if (animation != NULL && animation != reinterpret_cast<nuinstanim_s *>(-1)) {
            return 1;
        }
    } else if (special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        if (static_cast<NuLegacyInstanceLayout *>(legacy->instance)->animation != NULL) {
            return 1;
        }
    }
    return 0;
}

extern "C" void NuSpecialSetMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special->display_special != NULL) {
        special->display_special->instance_mtx = *matrix;
    } else {
        *static_cast<NUMTX *>(special->special) = *matrix;
    }
}

extern "C" i32 NuSpecialGetCollision(nuhspecial_s *special) {
    if (special == NULL || special->scene == NULL) {
        return 0;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        return legacy->flags & NULEGACYSPECIAL_FLAG_COLLISION;
    }
    if (special->display_special != NULL) {
        return special->display_special->flags & NUDISPLAYSPECIAL_FLAG_COLLISION;
    }
    return 0;
}

extern "C" void NuSpecialSetCollision(nuhspecial_s *special, i32 enabled) {
    if (special == NULL || special->scene == NULL) {
        return;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        if (enabled != 0) {
            legacy->flags |= NULEGACYSPECIAL_FLAG_COLLISION;
        } else {
            legacy->flags &= ~NULEGACYSPECIAL_FLAG_COLLISION;
        }
    } else if (special->display_special != NULL) {
        if (enabled != 0) {
            special->display_special->flags |= NUDISPLAYSPECIAL_FLAG_COLLISION;
        } else {
            special->display_special->flags &= ~NUDISPLAYSPECIAL_FLAG_COLLISION;
        }
    }
}

extern "C" void NuSpecialSetDrawPos(nuhspecial_s *special, NUVEC *pos) {
    if (special == NULL || special->scene == NULL) {
        return;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NUMTX *matrix = static_cast<NUMTX *>(legacy->instance);
        matrix->m30 = pos->x;
        matrix->m31 = pos->y;
        matrix->m32 = pos->z;
    } else {
        NUMTX *matrix = &special->display_special->draw_mtx;
        matrix->m30 = pos->x;
        matrix->m31 = pos->y;
        matrix->m32 = pos->z;
        if ((special->scene->display_list->instance_visibility_enabled & 1) != 0) {
            NuDisplayListUpdateSpecial(special);
            special->display_special->flags |= NUDISPLAYSPECIAL_FLAG_MATRIX_UPDATED;
        }
    }
}

extern "C" f32 NuSpecialGetAnimEndFrame(nuhspecial_s *special) {
    if (special == NULL) {
        return 0.0f;
    }
    NUGSCN *scene = special->scene;
    if (scene == NULL) {
        return 0.0f;
    }

    nuinstanim_s *instance_animation = NuSpecialGetInstAnim(special);
    if (instance_animation == NULL) {
        return 0.0f;
    }

    nuanimdata_s *animation = scene->instance_animation_data[instance_animation->anim_ix];
    if (animation != NULL) {
        return NuAnimEndFrameOld(animation);
    }

    if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) == 0 ||
        scene->animation_end_frames == NULL) {
        return 0.0f;
    }

    const u16 lookup_index = instance_animation->end_frame_lookup_index;
    const u32 end_frame = scene->animation_end_frames[lookup_index - 1].end_frame;
    return static_cast<f32>(end_frame);
}

f32 NuSpecialGetAnimPos(nuhspecial_s *special) {
    nuinstanim_s *animation = NuSpecialGetInstAnim(special);
    if (animation == NULL || special->scene->instance_animation_data[animation->anim_ix] == NULL) {
        return 0.0f;
    }
    f32 end_frame = NuSpecialGetAnimEndFrame(special);
    f32 position = (animation->ltime - 1.0f) / (end_frame - 1.0f);
    if (position < 0.0f) {
        return 0.0f;
    }
    return position > 1.0f ? 1.0f : position;
}

extern "C" char *NuSpecialGetName(nuhspecial_s *special) {
    if (special == NULL) {
        return NULL;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display != NULL) {
        return display->name;
    }
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    return legacy != NULL ? legacy->name : NULL;
}

extern "C" i32 NuSpecialGetNoVisiTestFn(nuhspecial_s *special) {
    if (special->scene == NULL) {
        return 0;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->no_visibility_test;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    return display != NULL ? display->flags & NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST : 0;
}

extern "C" void NuSpecialSetNoVisiTest(nuhspecial_s *special, i32 enabled) {
    if (special->scene == NULL) {
        return;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        instance->no_visibility_test = enabled;
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL) {
        return;
    }
    if (enabled != 0) {
        display->flags |= NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST;
    } else {
        display->flags &= ~NUDISPLAYSPECIAL_FLAG_NO_VISIBILITY_TEST;
    }
}

extern "C" void NuSpecialSetOnScreen(nuhspecial_s *special, i32 enabled) {
    if (special->scene == NULL)
        return;
    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        instance->on_screen = enabled;
        return;
    }
    NUDISPLAYSPECIAL *display = special->display_special;
    if (display == NULL)
        return;
    if (enabled != 0)
        display->flags |= NUDISPLAYSPECIAL_FLAG_ON_SCREEN;
    else
        display->flags &= ~NUDISPLAYSPECIAL_FLAG_ON_SCREEN;
}

extern "C" void NuSpecialSetInstanceMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special != NULL && special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        *static_cast<NUMTX *>(legacy->instance) = *matrix;
    }
}

extern "C" i32 NuSpecialGetVisibilityFn(void *special_ptr) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
    if (special->scene == NULL) {
        return 0;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->visible;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    return display != NULL ? (display->flags >> 1) & 1 : 0;
}

extern "C" void NuSpecialSetVisibility(void *special_ptr, i32 visible) {
    if (special_ptr == NULL) {
        return;
    }
    nuhspecial_s *special = static_cast<nuhspecial_s *>(special_ptr);
    NUGSCN *scene = special->scene;
    if (scene == NULL) {
        return;
    }

    NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
    if (legacy != NULL) {
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        if (instance != NULL) {
            instance->visible = visible;
        }
        if (visible != 0) {
            legacy->flags |= NULEGACYSPECIAL_FLAG_COLLISION;
        } else {
            legacy->flags &= ~NULEGACYSPECIAL_FLAG_COLLISION;
        }
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL) {
        return;
    }

    if (visible != 0) {
        display->flags |= NUDISPLAYSPECIAL_FLAG_VISIBLE | NUDISPLAYSPECIAL_FLAG_COLLISION;
        NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
        if ((display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0) {
            display_scene->visibility_flags[display->instance_ix] |= NUDL_INSTANCE_FLAG_VISIBLE;
        }
        return;
    }

    display->flags &= ~(NUDISPLAYSPECIAL_FLAG_VISIBLE | NUDISPLAYSPECIAL_FLAG_COLLISION);
    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
    if ((display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0) {
        display_scene->visibility_flags[display->instance_ix] &= ~NUDL_INSTANCE_FLAG_VISIBLE;
    }
}

extern "C" void NuDisplayListUpdateSpecial(nuhspecial_s *special) {
    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(special->scene->display_list);
    NUMTX draw_matrix;
    draw_matrix = *NuSpecialGetDrawMtx(special);

    NUVEC4 local_center;
    NUVEC half_extent;
    half_extent.x = (display->bounds_max.x - display->bounds_min.x) * 0.5f;
    half_extent.y = (display->bounds_max.y - display->bounds_min.y) * 0.5f;
    half_extent.z = (display->bounds_max.z - display->bounds_min.z) * 0.5f;

    local_center.x = (display->bounds_max.x + display->bounds_min.x) * 0.5f;
    local_center.y = (display->bounds_max.y + display->bounds_min.y) * 0.5f;
    local_center.z = (display->bounds_max.z + display->bounds_min.z) * 0.5f;

    NUVEC world_extent;
    world_extent.x = NuFabs(draw_matrix.m00) * half_extent.x + NuFabs(draw_matrix.m10) * half_extent.y +
                     NuFabs(draw_matrix.m20) * half_extent.z;
    world_extent.y = NuFabs(draw_matrix.m01) * half_extent.x + NuFabs(draw_matrix.m11) * half_extent.y +
                     NuFabs(draw_matrix.m21) * half_extent.z;
    world_extent.z = NuFabs(draw_matrix.m02) * half_extent.x + NuFabs(draw_matrix.m12) * half_extent.y +
                     NuFabs(draw_matrix.m22) * half_extent.z;

    NuVecMtxTransform(reinterpret_cast<NUVEC *>(&local_center), reinterpret_cast<NUVEC *>(&local_center), &draw_matrix);
    local_center.w = NuFsqrt(world_extent.x + world_extent.y + world_extent.z);

    if ((display_scene->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
        NUVEC4 *clip_bounds = reinterpret_cast<NUVEC4 *>(display_scene->clip_bounds);
        const i32 bounds_index = display->instance_ix * 2;
        clip_bounds[bounds_index] = local_center;
        clip_bounds[bounds_index + 1].x = world_extent.x;
        clip_bounds[bounds_index + 1].y = world_extent.y;
        clip_bounds[bounds_index + 1].z = world_extent.z;
    } else {
        NUVEC4 *clip_bounds = reinterpret_cast<NUVEC4 *>(display_scene->clip_bounds);
        const i32 bounds_index = display->instance_ix * 2;
        clip_bounds[bounds_index].x = local_center.x - world_extent.x;
        clip_bounds[bounds_index].y = local_center.y - world_extent.y;
        clip_bounds[bounds_index].z = local_center.z - world_extent.z;
        clip_bounds[bounds_index + 1].x = local_center.x + world_extent.x;
        clip_bounds[bounds_index + 1].y = local_center.y + world_extent.y;
        clip_bounds[bounds_index + 1].z = local_center.z + world_extent.z;
    }

    DisplayListUpdateSpecialTransformPS(special, &draw_matrix);
}

extern "C" void NuSpecialUpdate(nuhspecial_s *special) {
    if (special == NULL) {
        return;
    }

    NUDISPLAYSPECIAL *display = static_cast<NUDISPLAYSPECIAL *>(special->display_special);
    if (display == NULL || special->scene == NULL) {
        return;
    }

    NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(special->scene->display_list);
    if (display_scene == NULL ||
        (display_scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) == 0) {
        return;
    }

    display->flags |= NUDISPLAYSPECIAL_FLAG_MATRIX_UPDATED;
    NuDisplayListUpdateSpecial(special);
}

extern "C" i32 NuSpecialGetOnScreenFn(nuhspecial_s *special) {
    if (special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->on_screen;
    }
    return 1;
}
