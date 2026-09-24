#include "decomp.h"
#include <string.h>
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec4.h"

DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, instance) == 0x40, "legacy special instance offset");
DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, name) == 0x44, "legacy special name offset");
DECOMP_ASSERT(offsetof(NuSpecialLegacyLayout, flags) == 0x48, "legacy special flags offset");
DECOMP_ASSERT(offsetof(NuLegacyInstanceLayout, flags) == 0x44, "legacy instance flags offset");
DECOMP_ASSERT(offsetof(NuLegacyInstanceLayout, animation) == 0x48, "legacy instance animation offset");

extern "C" {
    f32 nuspecial_const_alpha = 1.0f;
    NUCOLOUR3 nuspecial_const_tint = {1.0f, 1.0f, 1.0f};
    i32 nuspecial_clip_state = -1;

    i32 special_debug;
    i32 nuspecial_const_alpha_enabled;
    i32 nuspecial_const_tint_enabled;
    i32 nuspecial_draw_state;
    i32 nuspecial_vertex_noffsets;
    VARIPTR nuspecial_vertex_offsets;
    NUSPECIALVERTEXSTATES *nuspecial_vertex_states;
    i32 nuspecial_reflection;
    i32 nuspecial_shadowLightCount;
    void *nuspecial_shadowLight[4];
    i32 nuspecial_shadowLightClipOverride[4];
    i32 nuspecial_shadowLightHaveClipOverrides;
}

extern "C" void NuSpecialList(void) {
    STUBBED();
}

i32 NuSpecialFind(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 flags) {
    (void)flags; // Present in the exported ABI; unused by the original body.

    nuhspecial_s *handle = dest;
    if (name != NULL && scene != NULL) {
        NUDLDLISTSCENE *display_scene = reinterpret_cast<NUDLDLISTSCENE *>(scene->display_list);
        if (display_scene != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(display_scene->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrICmp(name, special->name) == 0) {
                    handle->scene = scene;
                    handle->display_special = reinterpret_cast<NUDISPLAYSPECIAL_s *>(special);
                    handle->special = NULL;
                    return 1;
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (NuStrICmp(name, special->name) == 0) {
                    handle->scene = scene;
                    handle->special = special;
                    handle->display_special = NULL;
                    return 1;
                }
            }
        }
    }

    handle->scene = NULL;
    handle->special = NULL;
    handle->display_special = NULL;
    return 0;
}

extern "C" i32 NuSpecialFindMultiWC(NUGSCN *scene, nuhspecial_s *dest, char (*wildcards)[20], char *pattern,
                                    i32 capacity, i32 flags) {
    i32 count = 0;
    if (scene != NULL) {
        if (scene->display_list != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrICmpWC(pattern, special->name, wildcards != NULL ? *wildcards : NULL) == 0) {
                    if (dest != NULL) {
                        dest->scene = scene;
                        dest->display_special = special;
                        dest->special = NULL;
                        ++dest;
                    }
                    if (wildcards != NULL) {
                        ++wildcards;
                    }
                    if (++count >= capacity) {
                        return count;
                    }
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (NuStrICmpWC(pattern, special->name, wildcards != NULL ? *wildcards : NULL) == 0) {
                    if (dest != NULL) {
                        dest->scene = scene;
                        dest->special = special;
                        dest->display_special = NULL;
                        ++dest;
                    }
                    if (wildcards != NULL) {
                        ++wildcards;
                    }
                    if (++count >= capacity) {
                        return count;
                    }
                }
            }
        }
    }
    return count;
}

extern "C" i32 NuSpecialFindMulti(NUGSCN *scene, nuhspecial_s *dest, char *name, i32 capacity, i32 flags) {
    i32 count = 0;
    if (scene != NULL) {
        if (scene->display_list != NULL) {
            NUDISPLAYSPECIAL *special = static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials);
            for (i32 i = 0; i < scene->display_list->nspecials; ++special, ++i) {
                if (NuStrIStr(special->name, name) != NULL) {
                    ++count;
                    dest->scene = scene;
                    dest->display_special = special;
                    dest->special = NULL;
                    ++dest;
                    if (count >= capacity) {
                        return count;
                    }
                }
            }
        } else {
            NuSpecialLegacyLayout *special = reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials);
            for (i32 i = 0; i < scene->numspecial; ++special, ++i) {
                if (NuStrIStr(special->name, name) != NULL) {
                    ++count;
                    dest->scene = scene;
                    dest->special = special;
                    dest->display_special = NULL;
                    ++dest;
                    if (count >= capacity) {
                        return count;
                    }
                }
            }
        }
    }
    return count;
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

extern "C" NUVEC *NuSpecialGetPos(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    if (handle->display_special != NULL) {
        return reinterpret_cast<NUVEC *>(&static_cast<NUMTX *>(handle->display_special)->m30);
    }
    if (handle->special != NULL) {
        return reinterpret_cast<NUVEC *>(&static_cast<NUMTX *>(handle->special)->m30);
    }
    return NULL;
}

extern "C" NUVEC *NuSpecialGetDrawPos(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    if (display != NULL) {
        usize instance_animation = reinterpret_cast<usize>(display->instance_animation);
        NUVEC *draw_position = NUMTX_GET_ROW_VEC(&display->draw_mtx, 3);
        NUVEC *animated_position = reinterpret_cast<NUVEC *>(instance_animation + offsetof(NUMTX, m30));
        return instance_animation != 0 && instance_animation != static_cast<usize>(-1) ? animated_position
                                                                                       : draw_position;
    }

    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy == NULL) {
        return NULL;
    }
    NUMTX *instance = reinterpret_cast<NUMTX *>(legacy->instance);
    NUMTX *draw_mtx = *reinterpret_cast<NUMTX **>(legacy->instance + 0x48);
    NUVEC *instance_position = NUMTX_GET_ROW_VEC(instance, 3);
    NUVEC *draw_position = reinterpret_cast<NUVEC *>(reinterpret_cast<usize>(draw_mtx) + offsetof(NUMTX, m30));
    return draw_mtx != NULL ? draw_position : instance_position;
}

extern "C" NUMTX *NuSpecialGetMtx(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->display_special != NULL) {
        return static_cast<NUMTX *>(handle->display_special);
    }
    return static_cast<NUMTX *>(handle->special);
}

extern "C" void NuSpecialSetMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special->display_special != NULL) {
        special->display_special->instance_mtx = *matrix;
    } else {
        *static_cast<NUMTX *>(special->special) = *matrix;
    }
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

extern "C" void NuSpecialVertexStates(NUSPECIALVERTEXSTATES *states) {
    nuspecial_vertex_states = states;
    ++render_state.state.global_id;
    ++render_state.state.vertex_groups_id;
}

extern "C" void NuSpecialVertexOffsets(i32 count, VARIPTR offsets) {
    nuspecial_vertex_offsets = offsets;
    nuspecial_vertex_noffsets = count;
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

extern "C" void NuSpecialForceMtl(NUMTL *material) {
    if (material != NULL) {
        nuspecial_draw_state |= NUSPECIAL_DRAW_FORCE_MATERIAL;
    } else {
        nuspecial_draw_state &= ~NUSPECIAL_DRAW_FORCE_MATERIAL;
    }
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialMtl(NUMTL *material) {
    nurndr_forced_mtl = material;
}

extern "C" void NuSpecialConstAlpha(i32 enabled, f32 alpha) {
    if (enabled != 0) {
        nuspecial_const_alpha = alpha;
        nuspecial_draw_state |= 1;
    } else {
        nuspecial_draw_state &= ~1;
    }
    nuspecial_const_alpha_enabled = enabled;
}

extern "C" void NuSpecialConstTint(i32 enabled, NUVEC *tint) {
    if (enabled != 0) {
        memcpy(&nuspecial_const_tint, tint, sizeof(nuspecial_const_tint));
        nuspecial_draw_state |= 2;
    } else {
        nuspecial_draw_state &= ~2;
    }
    nuspecial_const_tint_enabled = enabled;
}

void NuSpecialReflection(i32 reflection) {
    nuspecial_reflection = reflection;
    RndrStateSetReflection(reflection);
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

extern "C" i32 NuSpecialGetOnScreenFn(nuhspecial_s *special) {
    if (special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        NuLegacyInstanceLayout *instance = static_cast<NuLegacyInstanceLayout *>(legacy->instance);
        return instance->on_screen;
    }
    return 1;
}

extern "C" void NuSpecialSetRenderPlane(void) {
    STUBBED();
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

extern "C" void NuSpecialSetAlphaTest(void) {
    STUBBED();
}

extern "C" i32 NuSpecialGetInstanceix(nuhspecial_s *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        for (i32 i = 0; i < scene->instance_count; ++i) {
            if (scene->instances + i * 0x50 == legacy->instance) {
                return i;
            }
        }
        return -1;
    }

    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    return display != NULL ? display->instance_ix : -1;
}

extern "C" i32 NuSpecialNumMtls(nuhspecial_s *special) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object->next != NULL) {
            object = object->next;
        }
        i32 count = 0;
        for (NuSpecialLegacyMaterialLink *link = object->materials; link != NULL; link = link->next) {
            ++count;
        }
        return count;
    }
    if (special->display_special != NULL) {
        NUDISPLAYSPECIAL *display = special->display_special;
        i32 level = 0;
        while (display->clip_range[level] != 0.0f) {
            ++level;
        }
        return display->clip_objects[level].nmaterials;
    }
    return 0;
}

extern "C" NUMTL *NuSpecialGetMtl(nuhspecial_s *special, i32 index) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object->next != NULL) {
            object = object->next;
        }
        NuSpecialLegacyMaterialLink *link = object->materials;
        while (index != 0) {
            if (link == NULL) {
                return NULL;
            }
            --index;
            link = link->next;
        }
        return link->material;
    } else if (special->display_special != NULL) {
        NUDISPLAYSPECIAL *display = special->display_special;
        i32 level = 0;
        while (display->clip_range[level] != 0.0f) {
            ++level;
        }
        return special->scene->display_list->mtls[display->clip_objects[level].material_ids[index]];
    }
    return NULL;
}

extern "C" void NuSpecialGetBounds(void *special, NUVEC *minimum, NUVEC *maximum) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->special == NULL) {
        NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
        if (display != NULL) {
            *minimum = display->min;
            *maximum = display->max;
        }
        return;
    }

    NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    NuSpecialLegacyInstanceBoundsLayout *instance =
        reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
    NuSpecialLegacyObjectBoundsLayout *object =
        static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
    while (object->next != NULL) {
        object = object->next;
    }
    *minimum = object->minimum;
    *maximum = object->maximum;
}

extern "C" void NuDynamicLightTestShadowExtrusionsSpecial(NuDynamicLight *light, void *special, NUMTX *matrix) {
    VuVec minimum;
    VuVec maximum;
    NuSpecialGetBounds(special, &minimum.xyz, &maximum.xyz);
    NuVecMtxTransform(&minimum.xyz, &minimum.xyz, matrix);
    NuVecMtxTransform(&maximum.xyz, &maximum.xyz, matrix);
    light->testShadowExtrusions(minimum, maximum);
}

extern "C" void NuSpecialSetBounds(nuhspecial_s *special, NUVEC *minimum, NUVEC *maximum) {
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(special->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(special->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        while (object != NULL) {
            object->minimum = *minimum;
            object->maximum = *maximum;
            object = object->next;
        }
    } else {
        NuSpecialBoundsDisplayLayout *display =
            reinterpret_cast<NuSpecialBoundsDisplayLayout *>(special->display_special);
        display->min = *minimum;
        display->max = *maximum;
    }
}

extern "C" void NuSpecialGetRadius(void *special, NUVEC *position, f32 *radius) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    if (handle->special != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        *radius = object->radius;
        *position = object->center;
    } else {
        NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
        *position = display->center;
        *radius = display->radius;
    }
}

extern "C" f32 NuSpecialGetOriginRadius(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        return object->origin_radius;
    }
    return NuVecMag(&static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->center) +
           static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special)->radius;
}

extern "C" NUMTX *NuSpecialGetDrawMtx(void *special) {
    NuSpecialHandleLayout *handle = reinterpret_cast<NuSpecialHandleLayout *>(special);
    NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
    if (legacy != NULL) {
        NUMTX *instance = reinterpret_cast<NUMTX *>(legacy->instance);
        NUMTX *draw_mtx = *reinterpret_cast<NUMTX **>(legacy->instance + 0x48);
        return draw_mtx != NULL ? draw_mtx : instance;
    }
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    if (display != NULL) {
        usize draw_mtx = reinterpret_cast<usize>(display->instance_animation);
        if (draw_mtx != 0 && draw_mtx != static_cast<usize>(-1)) {
            return reinterpret_cast<NUMTX *>(display->instance_animation);
        }
        return &display->draw_mtx;
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

extern "C" void NuSpecialSetInstanceMtx(nuhspecial_s *special, NUMTX *matrix) {
    if (special != NULL && special->scene != NULL && special->special != NULL) {
        NuSpecialLegacyLayout *legacy = static_cast<NuSpecialLegacyLayout *>(special->special);
        *static_cast<NUMTX *>(legacy->instance) = *matrix;
    }
}

extern "C" i32 NuSpecialClipTestExtents(void *special, void *matrix_arg) {
    NUMTX *matrix = static_cast<NUMTX *>(matrix_arg);
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    if (handle->special != NULL) {
        NuSpecialLegacySceneLayout *scene = reinterpret_cast<NuSpecialLegacySceneLayout *>(handle->scene);
        NuSpecialLegacyRuntimeLayout *legacy = static_cast<NuSpecialLegacyRuntimeLayout *>(handle->special);
        NuSpecialLegacyInstanceBoundsLayout *instance =
            reinterpret_cast<NuSpecialLegacyInstanceBoundsLayout *>(legacy->instance);
        NuSpecialLegacyObjectBoundsLayout *object =
            static_cast<NuSpecialLegacyObjectBoundsLayout *>(scene->objects[instance->object_index]);
        return NuCameraClipTestExtents(&object->minimum, &object->maximum, matrix, 0.0f, 0);
    }
    NuSpecialBoundsDisplayLayout *display = static_cast<NuSpecialBoundsDisplayLayout *>(handle->display_special);
    return NuCameraClipTestExtents(&display->min, &display->max, matrix, 0.0f, 0);
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

extern "C" i32 NuSpecialCompare(nuhspecial_s *first, nuhspecial_s *second) {
    if (first->special != NULL && first->special == second->special) {
        return 1;
    }
    if (first->display_special != NULL && first->display_special == second->display_special) {
        return 1;
    }
    return 0;
}

extern "C" void NuSpecialClear(void *special) {
    NuSpecialHandleLayout *handle = static_cast<NuSpecialHandleLayout *>(special);
    handle->scene = NULL;
    handle->special = NULL;
    handle->display_special = NULL;
}

extern "C" NUSPECIALVERTEXSTATES *NuVertexStatesCreate(VARIPTR *buffer, i32 count) {
    buffer->addr = ALIGN(buffer->addr, 4);
    NUSPECIALVERTEXSTATES *states = (NUSPECIALVERTEXSTATES *)buffer->void_ptr;
    buffer->u8_ptr += sizeof(NUSPECIALVERTEXSTATES);
    states->count = count;
    states->flags = 0;
    i32 blocks = count / 16;
    if (count & 15)
        ++blocks;
    states->block_count = blocks;
    i32 size = blocks * 16;
    states->values = (i8 *)ALIGN(buffer->addr, 16);
    buffer->addr = ALIGN(buffer->addr, 16) + size;
    for (i32 i = 0; i < size; ++i)
        states->values[i] = 0;
    return states;
}

extern "C" void NuVertexStatesSetGroupState(NUSPECIALVERTEXSTATES *states, i32 group, i32 value) {
    states->values[group] = value;
}

extern "C" i32 NuSpecialSetClipping(i32 enabled, i32 state) {
    i32 previous = nuspecial_clip_state;
    nuspecial_clip_state = enabled != 0 ? state : -1;
    return previous;
}

extern "C" void NuSpecialAddShadowLight(void) {
    STUBBED();
}

extern "C" void NuSpecialClearShadowLights(void) {
    STUBBED();
}

extern "C" i32 NuSpecialGetActiveShadowLights(void) {
    return nuspecial_shadowLightCount;
}

extern "C" i32 NuSpecialHasActiveShadowLights(void) {
    return nuspecial_shadowLightCount > 0;
}

extern "C" void *NuSpecialGetShadowLight(i32 index) {
    return nuspecial_shadowLight[index];
}

extern "C" i32 NuSpecialClipTestShadowLights(NUVEC *, NUVEC *, i32) {
    STUBBED();
    return 0;
}

extern "C" i32 NuSpecialHaveShadowClipTestResults(void) {
    return nuspecial_shadowLightHaveClipOverrides;
}

extern "C" i32 NuSpecialGetShadowClipTestResult(i32 index) {
    if (nuspecial_shadowLightHaveClipOverrides != 0) {
        return nuspecial_shadowLightClipOverride[index];
    }
    return -1;
}

extern "C" void NuSpecialClearShadowClipTestResults(void) {
    nuspecial_shadowLightHaveClipOverrides = 0;
}

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
