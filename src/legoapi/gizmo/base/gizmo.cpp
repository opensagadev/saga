// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/actions/combat/hits.h"
#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"

#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"

#include <stdio.h>
#include <string.h>
#include "legoapi/world/world_shared.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nu3d/nugscn.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include <stdlib.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include <float.h>
#include "editor/edpath.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/world/areas.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/misc/androidbatman.h"
#include <limits.h>
#include "legoapi/render/fx/particles.h"
#include <cfloat>

static i32 DefaultGizmo_GetOutput(GIZMO *, i32, i32);
static char *DefaultGizmo_GetOutputName(GIZMO *, i32);
static i32 DefaultGizmo_GetNumOutputs(GIZMO *);

ADDGIZMOTYPE Default_ADDGIZMOTYPE = {
    NULL,                       // name
    NULL,                       // prefix
    -1,                         // id
    0,                          // unknown1
    0,                          // unknown2
    NULL,                       // get_max_gizmos_fn
    NULL,                       // add_gizmos_fn
    NULL,                       // early_update_fn
    NULL,                       // late_update_fn
    NULL,                       // draw_fn
    NULL,                       // panel_draw_fn
    NULL,                       // unknown_fn
    NULL,                       // get_gizmo_name_fn
    DefaultGizmo_GetOutput,     // get_output_fn
    DefaultGizmo_GetOutputName, // get_output_name_fn
    DefaultGizmo_GetNumOutputs, // get_num_outputs_fn
    NULL,                       // activate_fn
    NULL,                       // activate_rev_fn
    NULL,                       // set_visibility_fn
    NULL,                       // get_visibility_fn
    NULL,                       // get_pos_fn
    NULL,                       // using_special_fn
    NULL,                       // bolt_hit_plat_fn
    NULL,                       // get_best_bolt_target_fn
    NULL,                       // bolt_hit_fn
    NULL,                       // allocate_progress_data_fn
    NULL,                       // clear_progress_fn
    NULL,                       // store_progress_fn
    NULL,                       // reset_fn
    NULL,                       // reserve_buffer_space_fn
    NULL,                       // load_fn
    NULL,                       // post_load_fn
    NULL,                       // add_level_sfx_fn
};

GIZMOTYPES *gizmotypes;

static void GizmoAppendSpecialError(GIZMOSYS *gizmo_sys, const char *text) {
    if (text == NULL || gizmo_sys->error_log == NULL) {
        return;
    }

    if (NuStrLen(gizmo_sys->error_log) + NuStrLen(text) <= gizmoerrorlogsize) {
        NuStrNCat(gizmo_sys->error_log, text, gizmoerrorlogsize);
    } else {
        gizmo_sys->flags |= GIZMOSYS_FLAG_ERROR_LOG_OVERFLOW;
    }
}

i32 gizmoerrorlogsize = 0x800;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct nuhspecial_s;

struct EDCREATURE_s;

typedef isize __attribute__((may_alias)) NuGCutWord;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct eduimenu_s;
struct eduiitem_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern EdInputContext * Input;

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
} // extern "C"

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }
} // extern "C"

// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.

// Game-specific pre-load limits. These must be applied before WorldInfo
// reserves each level's fixed-size gameplay systems.

static i32 DefaultGizmo_GetOutput(GIZMO *, i32, i32) {
    return 0;
}

static char *DefaultGizmo_GetOutputName(GIZMO *, i32) {
    return "default";
}

static i32 DefaultGizmo_GetNumOutputs(GIZMO *) {
    return 1;
}

i32 GizmoFileReadName(char *name) {
    i32 name_length = EdFileReadChar();
    if (name_length == 0) {
        return 0;
    }

    EdFileRead(name, name_length);
    return 1;
}

VARIPTR *GizmoBufferAlloc(VARIPTR *buffer, VARIPTR *buffer_end, i32 size) {
    VARIPTR *ptr = NULL;

    if (buffer_end != NULL && buffer != NULL) {
        if (buffer_end->addr > buffer->addr + size) {
            ptr = (VARIPTR *)ALIGN(buffer->addr, 16);
            buffer->addr = (usize)ptr + size;
            memset(ptr, 0, size);
        }
    }

    return ptr;
}

void RegisterGizmoTypes(VARIPTR *buffer, VARIPTR *buffer_end, REGISTERGIZMOTYPEFN *register_gizmo_type_fns,
                        i32 unknown) {
    ADDGIZMOTYPE *addgizmo;
    GIZMOTYPES *types;
    GIZMOTYPE *gizmo;
    void *pvVar4;
    VARIPTR *pvVar3;
    i32 i;

    if (gizmotypes != NULL || register_gizmo_type_fns == NULL || *register_gizmo_type_fns == NULL) {
        return;
    }

    i32 ntypes = 0;
    for (; register_gizmo_type_fns[ntypes] != NULL; ntypes++) {
    }

    types = (GIZMOTYPES *)GizmoBufferAlloc(buffer, buffer_end, sizeof(GIZMOTYPES));
    gizmotypes = types;
    if (types == NULL) {
        return;
    }

    types->unknown = unknown;
    types->types = (GIZMOTYPE *)GizmoBufferAlloc(buffer, buffer_end, ntypes * sizeof(GIZMOTYPE));
    gizmo = gizmotypes->types;

    if (gizmo == NULL) {
        return;
    }

    gizmotypes->count = ntypes;

    for (i32 n = 0; n < gizmotypes->count; n++, gizmo++) {
        addgizmo = register_gizmo_type_fns[n](n);

        if (*addgizmo->prefix != '\0' && n != 0) {
            i = 0;
            do {
                while (gizmotypes->types[i].prefix[0] == '\0') {
                    i = i + 1;
                    if (i == n)
                        goto copy_data;
                }
                NuStrICmp(addgizmo->prefix, gizmotypes->types[i].prefix);
                i = i + 1;
            } while (i != n);
        }

    copy_data:
        NuStrCpy(gizmo->name, addgizmo->name);
        NuStrNCpy(gizmo->prefix, addgizmo->prefix, sizeof(gizmo->prefix));
        gizmo->fns.unknown1 = addgizmo->fns.unknown1;
        gizmo->fns.get_max_gizmos_fn = addgizmo->fns.get_max_gizmos_fn;
        gizmo->fns.add_gizmos_fn = addgizmo->fns.add_gizmos_fn;
        gizmo->fns.early_update_fn = addgizmo->fns.early_update_fn;
        gizmo->fns.late_update_fn = addgizmo->fns.late_update_fn;
        gizmo->fns.draw_fn = addgizmo->fns.draw_fn;
        gizmo->fns.panel_draw_fn = addgizmo->fns.panel_draw_fn;
        gizmo->fns.get_gizmo_name_fn = addgizmo->fns.get_gizmo_name_fn;
        gizmo->fns.get_output_fn = addgizmo->fns.get_output_fn;
        gizmo->fns.get_output_name_fn = addgizmo->fns.get_output_name_fn;
        gizmo->fns.get_num_outputs_fn = addgizmo->fns.get_num_outputs_fn;
        gizmo->fns.activate_fn = addgizmo->fns.activate_fn;
        gizmo->fns.activate_rev_fn = addgizmo->fns.activate_rev_fn;
        gizmo->fns.set_visibility_fn = addgizmo->fns.set_visibility_fn;
        gizmo->fns.get_visibility_fn = addgizmo->fns.get_visibility_fn;
        gizmo->fns.get_pos_fn = addgizmo->fns.get_pos_fn;
        gizmo->fns.using_special_fn = addgizmo->fns.using_special_fn;
        gizmo->fns.bolt_hit_plat_fn = addgizmo->fns.bolt_hit_plat_fn;
        gizmo->fns.get_best_bolt_target_fn = addgizmo->fns.get_best_bolt_target_fn;
        gizmo->fns.bolt_hit_fn = addgizmo->fns.bolt_hit_fn;
        gizmo->fns.allocate_progress_data_fn = addgizmo->fns.allocate_progress_data_fn;
        gizmo->fns.clear_progress_fn = addgizmo->fns.clear_progress_fn;
        gizmo->fns.store_progress_fn = addgizmo->fns.store_progress_fn;
        gizmo->fns.reset_fn = addgizmo->fns.reset_fn;
        gizmo->fns.reserve_buffer_space_fn = addgizmo->fns.reserve_buffer_space_fn;
        gizmo->fns.load_fn = addgizmo->fns.load_fn;
        gizmo->fns.post_load_fn = addgizmo->fns.post_load_fn;
        gizmo->fns.add_level_sfx_fn = addgizmo->fns.add_level_sfx_fn;

        pvVar4 = (void *)addgizmo->fns.allocate_progress_data_fn;

        if (pvVar4 != NULL && gizmotypes->unknown != 0) {
            pvVar3 = GizmoBufferAlloc(buffer, buffer_end, gizmotypes->unknown * sizeof(VARIPTR));
            gizmo->buffer = pvVar3;
            if (pvVar3 != NULL && gizmotypes->unknown > 0) {
                i = 0;
                auto gizmo_buffer = gizmo->buffer;
                while (1) {
                    void *pvVar5 = gizmo->fns.allocate_progress_data_fn(buffer, buffer_end);
                    pvVar3[i].void_ptr = pvVar5;
                    i = i + 1;
                    if (gizmotypes->unknown <= i)
                        break;
                    pvVar3 = gizmo_buffer;
                }
            }
        }
    }
}

GIZMOSYS *CreateGizmoSys(void *world, VARIPTR *buf, VARIPTR *buf_end) {
    GIZMOSYS *gizmo_sys = NULL;

    if (gizmotypes != NULL) {
        gizmo_sys = reinterpret_cast<GIZMOSYS *>(GizmoBufferAlloc(buf, buf_end, sizeof(GIZMOSYS)));
        if (gizmo_sys != NULL) {
            gizmo_sys->sets = reinterpret_cast<GIZMOSET *>(
                GizmoBufferAlloc(buf, buf_end, gizmotypes->count * static_cast<i32>(sizeof(GIZMOSET))));

            if (gizmo_sys->sets != NULL) {
                GIZMOSET *set = gizmo_sys->sets;
                GIZMOTYPE *type = gizmotypes->types;
                for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++set, ++type) {
                    set->type = type;

                    if (type->fns.get_max_gizmos_fn != NULL) {
                        set->max_count = type->fns.get_max_gizmos_fn(world);
                    }
                    if (set->max_count != 0) {
                        set->gizmos = reinterpret_cast<GIZMO *>(
                            GizmoBufferAlloc(buf, buf_end, set->max_count * static_cast<i32>(sizeof(GIZMO))));
                    }
                    if (type->fns.reserve_buffer_space_fn != NULL) {
                        set->unknown = type->fns.reserve_buffer_space_fn(world);
                    }
                }
            }

            if (gizmoerrorlogsize != 0) {
                gizmo_sys->error_log = reinterpret_cast<char *>(GizmoBufferAlloc(buf, buf_end, gizmoerrorlogsize));
            }
        }
    }

    return gizmo_sys;
}

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *) {
    return 0;
}

void AddLevelSfxGizmoSys(GIZMOSYS_s *gizmo_sys, void *world_info, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOSET *set = gizmo_sys->sets;
    GIZMOTYPE *type = gizmotypes->types;
    i32 i = 0;
    while (i < gizmotypes->count) {
        if (type->fns.add_level_sfx_fn != NULL) {
            type->fns.add_level_sfx_fn(world_info, set->unknown, sfx_ids, sfx_count, max_sfx_count);
        }
        ++i;
        ++set;
        ++type;
    }
}

void GizmoSysClearLevelProgress(void *unknown, i32 type_id) {
    GIZMOTYPE *type = gizmotypes->types;
    if (gizmotypes == NULL || gizmotypes->count <= 0) {
        return;
    }

    if (type_id < 0) {
        for (i32 i = 0; i < gizmotypes->count; i++, type++) {
            if (type->fns.clear_progress_fn != NULL) {
                type->fns.clear_progress_fn(unknown, NULL);
            }
        }
    } else {
        for (i32 i = 0; i < gizmotypes->count; i++, type++) {
            if (type->fns.clear_progress_fn != NULL) {
                void *progress = NULL;
                if (type->buffer != NULL && type_id < gizmotypes->unknown) {
                    progress = type->buffer[type_id].void_ptr;
                }

                type->fns.clear_progress_fn(unknown, progress);
            }
        }
    }
}

void GizmoSysStoreProgress(GIZMOSYS_s *system, void *world, i32 progress_index) {
    GizmoSysClearLevelProgress(world, progress_index);
    if (gizmotypes == NULL || system == NULL)
        return;
    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = system->sets;
    for (i32 i = 0; i < gizmotypes->count; ++i, ++type, ++set) {
        if (type->fns.store_progress_fn != NULL) {
            if (progress_index >= 0 && type->buffer != NULL)
                type->fns.store_progress_fn(world, set->unknown, type->buffer[progress_index].void_ptr);
            else
                type->fns.store_progress_fn(world, set->unknown, NULL);
        }
    }
}

void GizmoSysReset(GIZMOSYS *gizmo_sys, void *world, i32 progress_index) {
    if (gizmotypes != NULL && gizmo_sys != NULL) {
        GIZMOTYPE *type = gizmotypes->types;
        GIZMOSET *set = gizmo_sys->sets;

        for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++set, ++type) {
            if (type->fns.reset_fn != NULL) {
                if (progress_index >= 0) {
                    if (__builtin_expect(type->buffer != NULL, 1)) {
                        type->fns.reset_fn(world, set->unknown, type->buffer[progress_index].void_ptr);
                    } else {
                        type->fns.reset_fn(world, set->unknown, NULL);
                    }
                } else {
                    type->fns.reset_fn(world, set->unknown, NULL);
                }
            }
        }
    }
}

void GizmoSysEarlyUpdate(GIZMOSYS *gizmo_sys, void *world_info, float delta_time) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 i = 0; i < gizmotypes->count; i++, set++, type++) {
        if (type->fns.early_update_fn != NULL) {
            type->fns.early_update_fn(world_info, set->unknown, delta_time);
        }
    }
}

void GizmoSysLateUpdate(GIZMOSYS *gizmo_sys, void *world_info, float delta_time) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 i = 0; i < gizmotypes->count; i++, set++, type++) {
        if (type->fns.late_update_fn != NULL) {
            type->fns.late_update_fn(world_info, set->unknown, delta_time);
        }
    }
}

void GizmoSysDraw(GIZMOSYS *gizmo_sys, void *world_info, float delta_time) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 i = 0; i < gizmotypes->count; i++, set++, type++) {
        if (type->fns.draw_fn != NULL) {
            type->fns.draw_fn(world_info, set->unknown, delta_time);
        }
    }
}

void GizmoSysPanelDraw(GIZMOSYS *gizmo_sys, void *world_info, float delta_time) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 i = 0; i < gizmotypes->count; i++, set++, type++) {
        if (type->fns.panel_draw_fn != NULL) {
            type->fns.panel_draw_fn(world_info, set->unknown, delta_time);
        }
    }
}

i32 GizmoGetTypeIDByName(GIZMOSYS_s *gizmo_sys, char *name) {
    if (gizmotypes == NULL || name == NULL || gizmo_sys == NULL || gizmotypes->count <= 0) {
        return -1;
    }

    for (i32 i = 0; i < gizmotypes->count; i++) {
        if (NuStrCmp(gizmotypes->types[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

void GizmoTypeStoreProgress(GIZMOSYS_s *system, void *world, i32 progress_index, i32 type_id, char *name) {
    if (name != NULL && type_id == -1)
        type_id = GizmoGetTypeIDByName(system, name);
    if (type_id == -1)
        return;
    GIZMOTYPE *type = &gizmotypes->types[type_id];
    GIZMOSET *set = &system->sets[type_id];
    void *progress = NULL;
    if (type->buffer != NULL && progress_index >= 0 && progress_index < gizmotypes->unknown)
        progress = type->buffer[progress_index].void_ptr;
    if (type->fns.clear_progress_fn != NULL)
        type->fns.clear_progress_fn(world, progress);
    if (type->fns.store_progress_fn != NULL)
        type->fns.store_progress_fn(world, set->unknown, progress);
}

i32 GizmoTypeGetProgress(GIZMOSYS_s *system, void *, i32 progress_index, i32 type_id, char *name, void **result) {
    if (name != NULL && type_id == -1)
        type_id = GizmoGetTypeIDByName(system, name);
    if (type_id == -1)
        return 0;
    GIZMOTYPE *type = &gizmotypes->types[type_id];
    void *progress = NULL;
    if (type->buffer != NULL && progress_index >= 0 && progress_index < gizmotypes->unknown)
        progress = type->buffer[progress_index].void_ptr;
    *result = progress;
    return type->fns.unknown1 != -1 ? type->fns.unknown1 : 0;
}

void LoadGizmoSys(GIZMOSYS_s *gizmo_sys, void *world, char *config_file) {
    if (gizmo_sys != NULL) {
        gizmo_sys->flags |= GIZMOSYS_FLAG_LOADING;
        if (gizmo_sys->error_log != NULL) {
            memset(gizmo_sys->error_log, 0, gizmoerrorlogsize);
        }
        gizmo_sys->flags &= ~3;

        if (gizmotypes != NULL) {
            char gizmo_name[32];
            char path[256];
            sprintf(path, "%s.giz", config_file);

            EdFileSetMedia(1);
            if (EdFileOpen(path, NUFILE_READ) != 0) {
                EdFileReadInt();
                i32 name_length = EdFileReadInt();
                while (name_length != 0) {
                    memset(gizmo_name, 0, sizeof(gizmo_name));
                    EdFileRead(gizmo_name, name_length);

                    i32 data_length = EdFileReadInt();
                    i32 type_id = GizmoGetTypeIDByName(gizmo_sys, gizmo_name);
                    if (data_length > 0 && type_id >= 0 && type_id < gizmotypes->count &&
                        gizmotypes->types[type_id].fns.load_fn != NULL &&
                        gizmotypes->types[type_id].fns.load_fn(world, gizmo_sys->sets[type_id].unknown) != 0) {
                        name_length = EdFileReadInt();
                        continue;
                    }

                    while (data_length != 0) {
                        EdFileReadChar();
                        --data_length;
                    }
                    name_length = EdFileReadInt();
                }
                EdFileClose();
            }

            // File callbacks may replace or clear the registry before post-load dispatch.
            if (gizmotypes != NULL) {
                GIZMOSET *set = gizmo_sys->sets;
                GIZMOTYPE *type = gizmotypes->types;
                for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++type, ++set) {
                    if (type->fns.post_load_fn != NULL) {
                        type->fns.post_load_fn(world, set->unknown);
                    }
                }
            }
        }

        gizmo_sys->flags &= ~GIZMOSYS_FLAG_LOADING;
    }
}

i32 ResetGizmoType(GIZMOSYS *gizmo_sys, i32 type_id, char *name) {
    if (name != NULL && type_id == -1) {
        type_id = GizmoGetTypeIDByName(gizmo_sys, name);
    }

    if (type_id == -1) {
        return 0;
    }

    gizmo_sys->sets[type_id].count = 0;
    SAGA_HOST_SAFE_MEMSET(gizmo_sys->sets[type_id].gizmos, 0, gizmo_sys->sets[type_id].max_count * sizeof(GIZMO));

    return 1;
}

void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world) {
    if (gizmotypes != NULL && gizmo_sys != NULL) {
        GIZMOTYPE *type = gizmotypes->types;
        GIZMOSET *set = gizmo_sys->sets;
        for (i32 type_id = 0; type_id < gizmotypes->count; ++type_id, ++set, ++type) {
            ResetGizmoType(gizmo_sys, type_id, NULL);
            if (type->fns.add_gizmos_fn != NULL) {
                type->fns.add_gizmos_fn(gizmo_sys, type_id, world, set->unknown);
            }
        }
    }

    if (giz_flow != NULL && giz_flow->pointers_need_reset != 0) {
        ResetGizFlowPointers(giz_flow);
    }
}

GIZMO *AddGizmo(GIZMOSYS *gizmo_sys, i32 type_id, char *name, void *object) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return NULL;
    }

    if (name != NULL) {
        type_id = GizmoGetTypeIDByName(gizmo_sys, name);
    }

    if (type_id > -1 && gizmotypes->count > type_id) {
        GIZMOSET *set = &gizmo_sys->sets[type_id];
        if (set->count < set->max_count) {
            GIZMO *gizmo = &set->gizmos[set->count];
            if (gizmo != NULL) {
                if (object != NULL) {
                    gizmo->object = object;
                }

                gizmo->unknown = 0;
                gizmo->type_id = (u8)type_id;
                set->count++;

                return gizmo;
            }
        }
    }

    return NULL;
}

GIZMO *GizmoFindByData(GIZMOSYS *gizmo_sys, i32 type_id, void *data) {
    if (gizmotypes == NULL || data == NULL || gizmo_sys == NULL) {
        return NULL;
    }

    if (type_id >= 0 && type_id <= gizmotypes->count) {
        GIZMOTYPE &type = gizmotypes->types[type_id];
        GIZMOSET &set = gizmo_sys->sets[type_id];
        if (type.fns.get_gizmo_name_fn == NULL) {
            return NULL;
        }

        GIZMO *gizmo = set.gizmos;
        for (i32 i = 0; i < set.count; ++i, ++gizmo) {
            if (gizmo->object == data) {
                return gizmo;
            }
        }
        return NULL;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
        if (type->fns.get_gizmo_name_fn != NULL) {
            GIZMO *gizmo = set->gizmos;
            for (i32 i = 0; i < set->count; ++i, ++gizmo) {
                if (gizmo->object == data) {
                    return gizmo;
                }
            }
        }
    }
    return NULL;
}

GIZMO *GizmoFindByName(GIZMOSYS *gizmo_sys, i32 type_id, char *name) {
    if (gizmotypes == NULL || name == NULL || gizmo_sys == NULL) {
        return NULL;
    }

    if (type_id >= 0 && type_id <= gizmotypes->count) {
        GIZMOTYPE &type = gizmotypes->types[type_id];
        GIZMOSET &set = gizmo_sys->sets[type_id];
        if (type.fns.get_gizmo_name_fn == NULL) {
            return NULL;
        }

        GIZMO *gizmo = set.gizmos;
        for (i32 i = 0; i < set.count; ++i, ++gizmo) {
            if (NuStrICmp(type.fns.get_gizmo_name_fn(gizmo), name) == 0) {
                return gizmo;
            }
        }
        return NULL;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
        if (type->fns.get_gizmo_name_fn == NULL) {
            continue;
        }

        GIZMO *gizmo = set->gizmos;
        for (i32 gizmo_index = 0; gizmo_index < set->count; ++gizmo_index, ++gizmo) {
            if (NuStrICmp(type->fns.get_gizmo_name_fn(gizmo), name) == 0) {
                return gizmo;
            }
        }
    }

    return NULL;
}

char *GizmoGetName(GIZMO *gizmo) {
    GIZMOTYPES *types = gizmotypes;
    if (types == NULL || gizmo == NULL || gizmo->type_id >= types->count) {
        return NULL;
    }

    GIZMOGETGIZMONAMEFN get_name = types->types[gizmo->type_id].fns.get_gizmo_name_fn;
    if (get_name == NULL) {
        return NULL;
    }
    return get_name(gizmo);
}

i32 GizmoGetNumOutputs(GIZMOSYS_s *gizmo_sys, GIZMO_s *gizmo) {
    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL) {
        return 0;
    }
    if (gizmo->type_id >= gizmotypes->count || gizmotypes->types[gizmo->type_id].fns.get_num_outputs_fn == NULL) {
        return 1;
    }
    return gizmotypes->types[gizmo->type_id].fns.get_num_outputs_fn(gizmo);
}

i32 GizmoGetOutput(GIZMOSYS *gizmo_sys, GIZMO *gizmo, i32 unknown1, i32 unknown2) {
    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL) {
        return 0;
    }

    if (gizmo->type_id >= gizmotypes->count || gizmotypes->types[gizmo->type_id].fns.get_output_fn == NULL) {
        return 0;
    }

    return gizmotypes->types[gizmo->type_id].fns.get_output_fn(gizmo, unknown1, unknown2);
}

char *GizmoGetOutputName(GIZMOSYS *gizmo_sys, GIZMO *gizmo, i32 output_index) {
    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL) {
        return NULL;
    }

    if (gizmo->type_id >= gizmotypes->count || gizmotypes->types[gizmo->type_id].fns.get_output_name_fn == NULL) {
        return "default";
    }

    return gizmotypes->types[gizmo->type_id].fns.get_output_name_fn(gizmo, output_index);
}

void GizmoActivate(GIZMOSYS *gizmo_sys, GIZMO *gizmo, i32 unknown1, i32 unknown2) {
    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL || gizmo->type_id >= gizmotypes->count) {
        return;
    }

    GIZMOTYPE *type = &gizmotypes->types[gizmo->type_id];
    if (type->fns.activate_fn == NULL) {
        return;
    }

    if (unknown1 != 0 && type->fns.set_visibility_fn != NULL) {
        type->fns.set_visibility_fn(gizmo, 1);
    }

    type->fns.activate_fn(gizmo, unknown1);
    LOG_INFO("gizmo activate type=%s gizmo=%p active=%d", type->name, (void *)gizmo, unknown1);
}

void GizmoSetVisibility(GIZMOSYS *gizmo_sys, GIZMO *gizmo, i32 visibility, i32 unknown) {
    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL || gizmo->type_id >= gizmotypes->count ||
        gizmotypes->types[gizmo->type_id].fns.set_visibility_fn == NULL) {
        return;
    }

    if (visibility == 0 && gizmotypes->types[gizmo->type_id].fns.activate_fn != NULL) {
        gizmotypes->types[gizmo->type_id].fns.activate_fn(gizmo, 0);
    }

    gizmotypes->types[gizmo->type_id].fns.set_visibility_fn(gizmo, visibility);
}

void GizmoActivateReverse(GIZMOSYS_s *system, GIZMO_s *gizmo, i32 reverse, i32 visibility, i32) {
    const i32 command = visibility == 0 ? 2 : 6;
    const i32 query = visibility == 0 ? 3 : 7;
    if (gizmo == NULL || gizmotypes == NULL || gizmotypes->types[gizmo->type_id].fns.activate_rev_fn == NULL) {
        return;
    }
    if (gizmotypes->types[gizmo->type_id].fns.activate_rev_fn(gizmo, reverse, query) == 0) {
        return;
    }
    gizmotypes->types[gizmo->type_id].fns.activate_rev_fn(gizmo, reverse, command);
    LOG_INFO("gizmo reverse activation type=%s gizmo=%p reverse=%d visibility=%d",
             gizmotypes->types[gizmo->type_id].name, (void *)gizmo, reverse, visibility);
    if (visibility != 0) {
        GizmoSetVisibility(system, gizmo, reverse == 0, 1);
    }
}

i32 GizmoGetVisibility(GIZMOSYS *gizmo_sys, GIZMO *gizmo) {

    if (gizmotypes == NULL || gizmo == NULL || gizmo_sys == NULL || gizmo->type_id >= gizmotypes->count ||
        gizmotypes->types[gizmo->type_id].fns.get_visibility_fn == NULL) {
        return 0;
    }

    return gizmotypes->types[gizmo->type_id].fns.get_visibility_fn(gizmo);
}

NUVEC *GizmoGetPos(GIZMOSYS_s *, GIZMO_s *gizmo) {
    if (gizmotypes != NULL && gizmo != NULL) {
        GIZMOGETPOSFN get_pos = gizmotypes->types[gizmo->type_id].fns.get_pos_fn;
        if (get_pos != NULL) {
            return get_pos(gizmo);
        }
    }

    return NULL;
}

i32 GizmoGetGizmosUsingSpecial(GIZMOSYS *gizmo_sys, void *world, GIZMO **result, i32 result_capacity, char *name) {
    if (gizmo_sys == NULL || gizmotypes == NULL || result == NULL || gizmotypes->count <= 0 || result_capacity <= 0) {
        return 0;
    }

    i32 type_index = 0;
    i32 result_count = 0;
    for (; type_index < gizmotypes->count && result_count < result_capacity; ++type_index) {
        GIZMOUSINGSPECIALFN using_special = gizmotypes->types[type_index].fns.using_special_fn;
        if (using_special != NULL) {
            const i32 added = using_special(result + result_count, world, result_capacity - result_count, name);
            if (added == -1) {
                return result_capacity;
            }
            result_count += added;
            if (result_count > result_capacity) {
                return result_capacity;
            }
        }
    }
    return result_count;
}

i32 GizmoGetGuid(GIZMOSYS_s *, GIZMO_s *) {
    return -1;
}

i32 GizmoSysWriteInfo(GIZMOSYS_s *gizmo_sys, char *path, nugscn_s *scene) {
    nuhspecial_s special;
    GIZMO gizmo;
    if (gizmotypes == NULL) {
        return 0;
    }
    if (NuFileExists(path)) {
        char backup[256];
        sprintf(backup, "%s.bak", path);
        NuFileCopy(backup, path);
    }
    EdFileSetMedia(1);
    if (!EdFileOpen(path, NUFILE_WRITE)) {
        return 0;
    }

    EdFileWriteInt(4);
    EdFileWriteInt(gizmotypes->count);
    if (gizmotypes->count != 0) {
        GIZMOTYPE *type = gizmotypes->types;
        GIZMOSET *set = gizmo_sys->sets;
        for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
            i32 length = strlen(type->name) + 1;
            EdFileWriteInt(length);
            if (length != 0) {
                EdFileWrite(type->name, length);
            }
            length = NuStrLen(type->prefix) + 1;
            EdFileWriteChar(static_cast<char>(length));
            if (length != 0) {
                EdFileWrite(type->prefix, length);
            }
            EdFileWriteInt(set->count);
            GIZMO *gizmo = set->gizmos;
            for (i32 gizmo_index = 0; gizmo_index < set->count; ++gizmo_index, ++gizmo) {
                char *name = GizmoGetName(gizmo);
                if (name != NULL) {
                    length = strlen(name) + 1;
                    EdFileWriteInt(length);
                    if (length > 0) {
                        EdFileWrite(name, length);
                    }
                } else {
                    EdFileWriteInt(0);
                }
                i32 outputs = GizmoGetNumOutputs(gizmo_sys, gizmo);
                EdFileWriteInt(outputs);
                for (i32 output_index = 0; output_index < outputs; ++output_index) {
                    name = GizmoGetOutputName(gizmo_sys, gizmo, output_index);
                    if (name != NULL) {
                        length = strlen(name) + 1;
                        EdFileWriteInt(length);
                        if (length > 0) {
                            EdFileWrite(name, length);
                        }
                    } else {
                        EdFileWriteInt(0);
                    }
                }
            }
        }
    }

    if (gizspecial_gizmotype_id != -1) {
        GIZMOTYPE *special_type = &gizmotypes->types[gizspecial_gizmotype_id];
        i32 length = NuStrLen(special_type->prefix) + 1;
        EdFileWriteChar(static_cast<char>(length));
        if (length != 0) {
            EdFileWrite(special_type->prefix, length);
        }

        gizmo.type_id = static_cast<u8>(gizspecial_gizmotype_id);
        i32 outputs = GizmoGetNumOutputs(gizmo_sys, &gizmo);
        EdFileWriteChar(static_cast<char>(outputs));
        for (i32 output_index = 0; output_index < outputs; ++output_index) {
            char *name = GizmoGetOutputName(gizmo_sys, &gizmo, output_index);
            if (name != NULL) {
                length = NuStrLen(name) + 1;
                EdFileWriteInt(length);
                if (length > 0) {
                    EdFileWrite(name, length);
                }
            } else {
                EdFileWriteInt(0);
            }
        }
    }

    i32 special_count = NuGScnNumSpecials(scene);
    EdFileWriteInt(special_count);
    for (i32 special_index = 0; special_index < special_count; ++special_index) {

        NuGScnGetSpecial(&special, scene, special_index);
        char *name = NuSpecialGetName(&special);
        i32 length = strlen(name) + 1;
        EdFileWriteInt(length);
        EdFileWrite(name, length);
    }
    EdFileClose();
    return 1;
}

i32 GizmoIsNameUnique(GIZMOSYS *gizmo_sys, char *name) {
    if (name == NULL || gizmo_sys == NULL) {
        return 1;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index, ++type, ++set) {
        if (type->fns.get_gizmo_name_fn == NULL) {
            continue;
        }

        GIZMO *gizmo = set->gizmos;
        for (i32 gizmo_index = 0; gizmo_index < set->count; ++gizmo_index, ++gizmo) {
            if (NuStrICmp(name, type->fns.get_gizmo_name_fn(gizmo)) == 0) {
                return 0;
            }
        }
    }

    return 1;
}

i32 GizmoNameUsesPrefix(char *name, char *prefix) {
    if (prefix == NULL || name == NULL) {
        return 0;
    }

    while (*name != '\0') {
        if (NuToUpper(*name) != NuToUpper(*prefix)) {
            return 0;
        }
        ++prefix;
        ++name;
    }

    return 1;
}

i32 GizmoGetUniqueName(GIZMOSYS *gizmo_sys, char *prefix, char *name, char *result, i32 result_size) {
    if (name == NULL || gizmo_sys == NULL || result == NULL) {
        return 0;
    }

    for (i32 type_index = 0; type_index < gizmotypes->count; ++type_index) {
        if (NuStrICmp(prefix, gizmotypes->types[type_index].prefix) != 0 &&
            GizmoNameUsesPrefix(name, gizmotypes->types[type_index].prefix)) {
            return 0;
        }
    }

    char suffix[16];
    char shortened_name[64];
    i32 name_length = NuStrLen(name);

    for (i32 suffix_number = 1; suffix_number != 999; ++suffix_number) {
        sprintf(suffix, "%d", suffix_number);
        i32 suffix_length = NuStrLen(suffix);

        if (name_length + suffix_length >= result_size) {
            name_length = result_size - suffix_length;
            if (name_length <= 0) {
                return 0;
            }

            NuStrNCpy(shortened_name, name, name_length);
            name = shortened_name;
        }

        sprintf(result, "%s%s", name, suffix);
        if (GizmoIsNameUnique(gizmo_sys, result)) {
            return 1;
        }
    }

    return 0;
}

i32 GizmoSys_BoltHitPlat(GIZMOSYS *gizmo_sys, void *world_info, BOLT *bolt, unsigned char *unknown) {
    if (gizmotypes == NULL || bolt == NULL || gizmo_sys == NULL) {
        return 0;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 i = 0; i < gizmotypes->count; i++, set++, type++) {
        if (type->fns.bolt_hit_plat_fn != NULL) {
            i32 result = type->fns.bolt_hit_plat_fn(world_info, set->unknown, bolt, unknown);
            if (result != 0) {
                return 1;
            }
        }
    }

    return 0;
}

i32 Gizmo_FindNuSpecial(nugscn_s *scene, nuhspecial_s *special, char *name, i32 flags, GIZMOSYS *gizmo_sys,
                        char *prefix, char *suffix) {
    if (NuSpecialFind(scene, special, name, flags) != 0) {
        return 1;
    }

    if (gizmo_sys == NULL || gizmo_sys->error_log == NULL || (gizmo_sys->flags & GIZMOSYS_FLAG_LOADING) == 0) {
        return 0;
    }

    GizmoAppendSpecialError(gizmo_sys, name);
    if (prefix != NULL) {
        GizmoAppendSpecialError(gizmo_sys, "\t\t");
        GizmoAppendSpecialError(gizmo_sys, prefix);
    }
    if (suffix != NULL) {
        GizmoAppendSpecialError(gizmo_sys, "\t\t");
        GizmoAppendSpecialError(gizmo_sys, suffix);
    }
    GizmoAppendSpecialError(gizmo_sys, "\n");
    return 0;
}

