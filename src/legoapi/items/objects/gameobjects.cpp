// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>
#include "legoapi/world/world_shared.h"
#include "legoapi/items/base/apiobject.h"
#include <new>
#include "nu2api/numath/nuang.h"
#include "legoapi/world/levels/levels.h"
#include <stdlib.h>
#include "nu2api/nu3d/nurndrstat.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <math.h>
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "nu2api/nu3d/nugscn.h"
#include "batman.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "editor/edpath.h"
#include "nu2api/nucore/nulist.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/numechptr.hpp"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath_types.h"
#include <ctype.h>
#include <limits.h>

i32 lightning_type;

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
i32 timebase_updates = 1;

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

u8 grapple_attach_frames = 5;

extern "C" {
    ed_module_s edTimingDesc = {NULL,
                                NULL,
                                "Timings",
                                ThingManager::edTimingInit,
                                NULL,
                                ThingManager::edTimingEnter,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                0x656d6974,
                                ThingManager::edTimingProc,
                                ThingManager::edTimingRender,
                                NULL};
}

i32 BaseThing::RemoveDependancies(ThingRemoveData *) {
    return 1;
}

void BaseThing::EnterLevel(ThingLevelData *) {
}

void BaseThing::ExitLevel(ThingLevelData *) {
}

void BaseThing::Reset(ThingResetData *) {
}

void BaseThing::Process(ThingProcessData *) {
}

void BaseThing::ProcessEvenWhenPaused(ThingProcessData *) {
}

void BaseThing::ProcessOnlyWhenPaused(ThingProcessData *) {
}

void BaseThing::Render(ThingRenderData *) {
}

void BaseThing::Display(ThingRenderData *) {
}

void BaseThing::Effects(ThingRenderData *) {
}

i32 speeder_hitpoints_lost;

GameObject_s *player_tag_to;
GameObject_s *player_tag_from;
f32 player_tag_timer;
i32 do_player_tag;

f32 Hub_PadSpeed[2];
u16 Hub_PadAngle[2];
GameObject_s *CarWashHack;
f32 AIFireIntervalMul = 1.0f;
f32 GhostLightMul = 1.0f;
f32 GhostLightTargetMul = 1.0f;
f32 CurrentSpeedOverride;
f32 lightning_sizew[2] = {0.02f, 0.02f};
f32 lightning_sizel[2] = {0.1f, 0.1f};
f32 lightning_sizewab[2] = {0.01f, 0.01f};
f32 lightning_endw[2] = {0.1f, 0.1f};
u32 lightning_col[2] = {0xff808040, 0xff808040};
f32 CurrentSpeed = 10.0f;
f32 BaseCurrentSpeed = 10.0f;

struct GameObject_s;

extern "C" {
    extern i16 id_TROOPERCANNON;
}

i32 addcreature_override_id_check;
f32 default_mover_extra = 0.05f;

extern "C" i16 id_LANDSPEEDER;
extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

struct EDCREATURE_s;

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
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" i16 id_MINIATST, id_MINIATTE;

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

static void ClearLastSafeTakeoverSource(GameObject_s *object) {
    if (object->takeover_source != NULL &&
        (LEGOCONTEXT_GETIN == -1 || object->character_context != LEGOCONTEXT_GETIN) &&
        (LEGOCONTEXT_BEENTAKENOVER == -1 || object->character_context != LEGOCONTEXT_BEENTAKENOVER))
        object->takeover_source = NULL;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void GameObjectDimensions(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    if (object->use_model_origin != 0 && object->field_0xd24 == 1.0f &&
        api.character_data->player_config->collision_origin_joint != -1 &&
        api.character_model->points_of_interest[api.character_data->player_config->collision_origin_joint] != NULL &&
        api.field_0x288 != 0) {
        const f32 radius = object->field_0x1004 * api.character_data->player_config->collision_origin_radius;
        api.field_0x1dc = radius;
        api.field_0x1e0 = radius;
    } else {
        api.field_0x1dc = api.collision_radius;
        api.field_0x1e0 = api.collision_height;
    }
    if (GameObjectDimensionsExtraFn != NULL)
        GameObjectDimensionsExtraFn(object);
}

MechObjectInterface *GameObject_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL)
        new CharacterObjectInterface(*this);
    return mech_object_interface;
}

void GameObject_s::KillTasks() {
    MechTouchTask *task = touch_task;
    while (task != NULL) {
        MechTouchTask *next = task->next;
        task->OnStop();
        delete task;
        task = next;
    }
    touch_task = NULL;
}

bool GameObject_s::IsRunningTaskType(HashedKey const &type) {
    MechTouchTask *task = touch_task;
    while (task != NULL) {
        MechTouchTask *next = task->next;
        if (task->GetHashId().value == type.value)
            return true;
        task = next;
    }
    return false;
}

void GameObject_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

MechAddonCollection *GameObject_s::GetAddons(bool create) {
    if (addons == NULL && create) {
        MechObjectInterface *object = GetMechObjectInterface();
        addons = new MechAddonCollection(*object);
    }
    return addons;
}

GameObject_s *AddGameObject(i32 id) {
    GameObject_s *object = reinterpret_cast<GameObject_s *>(APIObjectCreate(WORLD->api_object_sys));
    if (object == NULL) {
        return NULL;
    }

    const u8 object_index = object->apiobj.field_0x289;
    object->field_0x661 = 0xff;
    object->apiobj.field_0x27f = 0xff;
    object->apiobj.field_0x280 = 0xff;
    object->field_0x1086 = 2;
    object->apiobj.flags_low |= APIOBJECT_FLAG_IN_USE;
    object->apiobj.character = 1;
    object->field_0x1054 = 1;
    object->apiobj.collision_identity_mask = u64(1) << object_index;
    object->apiobj.field_0xa8 = 1.0f;
    object->field_0x1004 = 1.0f;
    object->field_0x1020 = 2000000.0f;
    object->apiobj.field_0x281 = 0xff;
    object->apiobj.field_0x218 = object->apiobj.water_height = object->apiobj.field_0x220 = 2000000.0f;

    HIGHGAMEOBJECT = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.flags_low & APIOBJECT_FLAG_IN_USE) != 0) {
            HIGHGAMEOBJECT = i + 1;
        }
    }

    object->ai.owner = object;
    object->apiobj.field_0x2a8 = 0;
    object->apiobj.field_0x2ac = 0;
    object->apiobj.objptr = object;
    object->apiobj.ai = &object->ai;

    MechAddonCollection *addons = object->GetAddons(true);
    if (addons != NULL) {
        MechEdgeStopAddon *edge_stop = new MechEdgeStopAddon(*object->GetMechObjectInterface());
        addons->Add(*edge_stop);
        if (VehicleArea != 0 || id == id_SPEEDERBIKE || id == id_SPEEDERBIKESNOW || id == id_STAP || id == id_STAP2 ||
            id == id_TROOPERCANNON || id == id_CANNON || id == id_MOSCANNON || id == id_ATST || id == id_BASKETCANNON ||
            id == id_BIGGUN) {
            MechObjectInterface *target = object->GetMechObjectInterface();
            MechAutofireAddon *addon = NU_ALLOC_T(MechAutofireAddon, 1, "", 0);
            if (addon != NULL)
                new (addon) MechAutofireAddon(*target);
            addons->Add(*addon);
        }
    }
    return object;
}

void GameObject_s::ClearAddons() {
    delete addons;
    addons = NULL;
}

void RemoveGameObject(GameObject_s *obj, i32) {
    if (obj == NULL) {
        return;
    }

    obj->KillTasks();
    obj->ClearAddons();
    obj->ClearMechObjectInterface();

    GameObject_s *other = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++, other++) {
        other->apiobj.collision_contact_mask &= ~obj->apiobj.collision_identity_mask;
        other->apiobj.ai_awareness_mask &= ~(1ULL << obj->apiobj.field_0x289);
        other->ai_seen_mask &= ~(1ULL << obj->apiobj.field_0x289);
        other->ai_opponent_exclusion_mask &= ~(1ULL << obj->apiobj.field_0x289);
    }

    if (obj->pad_gamepad != NULL) {
        obj->pad_gamepad->allocated_5a &= ~1u;
    }
    DestroySnakeBody(obj);
    APIObjectDestroy(WORLD->api_object_sys, &obj->apiobj);

    i32 object_count = 0;
    for (i32 i = 0; i < 64; i++) {
        if (Obj[i].apiobj.in_use) {
            object_count = i + 1;
        }
    }
    HIGHGAMEOBJECT = object_count;
    for (i32 i = 0; i < 8; i++) {
        if (LevGameObject[i] == obj) {
            LevGameObject[i] = NULL;
        }
    }
}

void ClearGameObjects(APIOBJECTSYS_s *api_object_sys) {
    for (i32 i = 0; i < 64; i++) {
        Obj[i].KillTasks();
        Obj[i].ClearAddons();
        Obj[i].ClearMechObjectInterface();
    }
    memset(Obj, 0, sizeof(GameObject_s) * 64);
    APIObjectDestroyAll(api_object_sys);
    HIGHGAMEOBJECT = 0;
}

void ScaleGameObject(GameObject_s *object) {
    const f32 scale = object->apiobj.field_0xa8;
    CHARACTERDATA *character = object->apiobj.character_data;
    object->apiobj.scaled_radius = character->field13_0x2c * scale;
    object->apiobj.collision_radius = object->field_0x1008 * scale;
    object->apiobj.collision_height = object->apiobj.collision_radius * object->collision_y_scale;
    object->apiobj.scaled_height = (character->field16_0x38 - character->field15_0x34) * object->field_0x1004;
}

__attribute__((optimize("O2", "tree-vectorize"))) void GetTopBot(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    const f32 bottom = character->field15_0x34;
    const f32 top = character->field16_0x38;
    object->field_0xffc = bottom;
    object->field_0x1000 = top;

    if (object->field_0x1008 == 0.0f) {
        object->collision_y_scale = 0.0f;
    } else {
        object->collision_y_scale = (top - bottom) / (object->field_0x1008 * 2.0f);
    }
}

void SetToLastSafePos(GameObject_s *object) {
    NUVEC position = {object->apiobj.last_safe_position.x, object->apiobj.last_safe_position.y,
                      object->apiobj.last_safe_position.z};
    object->apiobj.start_position = position;
    object->saved_position = object->apiobj.position = object->apiobj.start_position;
}

i32 UnderPlayerControl(GameObject_s *object) {
    return object->apiobj.player_controlled ||
           (object->field_0xcc0 != NULL && object->field_0xcc0->apiobj.player_controlled);
}

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only) {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.flags_low & APIOBJECT_FLAG_IN_USE) != 0 &&
            (vehicle_only == 0 || (object->apiobj.flags_high & 0x10) != 0) &&
            (required_flags == 0 || (object->apiobj.field_0x1f4 & required_flags) == required_flags) &&
            (character_id == -1 || object->id == character_id) &&
            (alive_only == 0 ||
             (object->apiobj.field_0x287 == 0 && (CInfo[object->character_context].flags & 0x8000) == 0)) &&
            (non_level_only == 0 || object->field_0x107c == -1)) {
            return object;
        }
    }
    return NULL;
}

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match) {
    GameObject_s *nearest = NULL;
    f32 nearest_distance = 100000000.0f;
    f32 radius_squared = radius * radius;
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            (object->field_0xe20 & 0x20) || (CInfo[object->character_context].flags & 0x8000))
            continue;
        if (character_id != -1 && object->id != character_id)
            continue;
        if (player_index >= 0) {
            if (player_index <= 1) {
                if (object->apiobj.field_0x27c != player_index)
                    continue;
            } else if (player_index == 99) {
                if (object->apiobj.field_0x27c == -1)
                    continue;
            } else if (player_index == 100) {
                if (object->apiobj.field_0x27c != -1)
                    continue;
            }
        }
        if (exclude && object == exclude)
            continue;
        if (required_flags && (object->apiobj.character_data->model_flags & required_flags) != required_flags)
            continue;
        if (animation != -1 && object->apiobj.character_model->model_data_b[animation] == NULL)
            continue;
        if (filter && !filter(object))
            continue;
        f32 x = object->apiobj.collision_position.x - position->x;
        f32 y = object->apiobj.collision_position.y - position->y;
        f32 z = object->apiobj.collision_position.z - position->z;
        f32 distance = horizontal_only ? x * x + z * z : x * x + y * y + z * z;
        if (exclude) {
            if (radius_squared <= 0.0f || distance < radius_squared) {
                f32 combined_radius = exclude->apiobj.collision_radius + object->apiobj.collision_radius + extra_radius;
                if (distance < combined_radius * combined_radius && distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = object;
                    if (first_match)
                        break;
                }
            }
        } else if ((radius_squared <= 0.0f || distance < radius_squared) && distance < nearest_distance) {
            nearest_distance = distance;
            nearest = object;
            if (first_match)
                break;
        }
    }
    if (nearest && distance_squared)
        *distance_squared = nearest_distance;
    return nearest;
}

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance) {
    const f32 no_floor_height = 2000000.0f;
    const f32 floor_height = object->apiobj.field_0x218;
    if (floor_height != no_floor_height) {
        const f32 radius_scale = object->apiobj.collision_radius / 0.225f;
        i32 height_steps = static_cast<i32>(height);
        if (height_steps < 0) {
            height_steps = 0;
        }
        f32 tolerance = static_cast<f32>(height_steps) * 0.025f;
        f32 radius_tolerance = radius_scale * tolerance;
        if (radius_tolerance < tolerance) {
            radius_tolerance = tolerance;
        }
        tolerance = radius_tolerance;

        const f32 floor_distance = object->apiobj.collision_min.y - floor_height;
        if (distance != NULL) {
            *distance = floor_distance;
        }
        return tolerance > floor_distance;
    }

    if (distance != NULL) {
        *distance = no_floor_height;
    }
    return 0;
}

void GameObjectToCameraDistances() {
    const NUVEC camera_position = GameCam->pos;
    GameObject_s *object = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & required_flags) != required_flags) {
            continue;
        }

        const f32 dx = camera_position.x - object->apiobj.position.x;
        const f32 dy = camera_position.y - object->apiobj.position.y;
        const f32 dz = camera_position.z - object->apiobj.position.z;
        object->ai_update_distance = NuFsqrt(dx * dx + dy * dy + dz * dz);
    }
}

void GameObjectOrigin(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    api.field_0x1f4 |= 0x100u;

    f32 predicted_vertical_displacement;
    f32 origin_x;
    f32 origin_z;
    PLAYERCHARACTERCONFIG_s *config;
    i32 model_origin_joint;
    if (object->use_model_origin != 0 &&
        (config = api.character_data->player_config, model_origin_joint = config->model_origin_joint) != -1 &&
        api.character_model != NULL && api.character_model->points_of_interest[model_origin_joint] != NULL) {
        f32 predicted_x;
        f32 predicted_z;
        if ((object->field_0xe24 & GAMEOBJECT_E24_FLAG_JOINT_MATRICES_UPDATED) != 0) {
            const f32 frame_time = FRAMETIME;
            predicted_x = api.previous_velocity.x * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            predicted_z = api.previous_velocity.z * frame_time;
            api.collision_position.z = object->joint_matrices[model_origin_joint].m32 + predicted_z;
            api.collision_position.x = object->joint_matrices[model_origin_joint].m30 + predicted_x;
            api.collision_position.y = object->joint_matrices[model_origin_joint].m31 + predicted_vertical_displacement;
            api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
        } else {
            api.collision_position = {0.0f, -object->character_bottom, 0.0f};
            NuVecMtxRotate(&api.collision_position, &api.collision_position, &api.field_0xb8);
            NuVecAdd(&api.collision_position, &api.collision_position, &api.position);
            const f32 frame_time = FRAMETIME;
            predicted_x = api.previous_velocity.x * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            predicted_z = api.previous_velocity.z * frame_time;
        }
        api.collision_position.x += predicted_x;
        api.collision_position.y += predicted_vertical_displacement;
        api.collision_position.z += predicted_z;
        origin_x = api.position.x + predicted_x;
        origin_z = api.position.z + predicted_z;
    } else if (object->use_model_origin != 0 && object->field_0xd24 == 1.0f && config->collision_origin_joint != -1 &&
               api.character_model->points_of_interest[config->collision_origin_joint] != NULL &&
               api.field_0x288 != 0) {
        api.collision_position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[config->collision_origin_joint], 3);
        const f32 frame_time = FRAMETIME;
        origin_x = api.position.x + api.previous_velocity.x * frame_time;
        origin_z = api.position.z + api.previous_velocity.z * frame_time;
        predicted_vertical_displacement = api.previous_velocity.y * frame_time;
    } else {
        api.field_0x1f4 ^= 0x100u;
        predicted_vertical_displacement = api.previous_velocity.y * FRAMETIME;
        api.collision_position.x = api.position.x + api.previous_velocity.x * FRAMETIME;
        api.collision_position.z = api.position.z + api.previous_velocity.z * FRAMETIME;
        api.collision_position.y = api.position.y + predicted_vertical_displacement;
        api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
        origin_x = api.collision_position.x;
        origin_z = api.collision_position.z;
    }

    const f32 radius = api.field_0x1dc;
    api.collision_origin.x = origin_x;
    api.collision_origin.z = origin_z;
    api.upper_position.x = origin_x;
    api.upper_position.z = origin_z;
    api.lower_position.z = origin_z;
    api.lower_position.x = origin_x;
    api.collision_min.x = api.collision_position.x - radius;
    api.collision_max.x = api.collision_position.x + radius;
    api.collision_min.z = api.collision_position.z - radius;
    api.collision_max.z = api.collision_position.z + radius;
    api.collision_min.y = api.collision_position.y - api.field_0x1e0;
    api.collision_max.y = api.collision_position.y + api.field_0x1e0;
    api.upper_position.y = api.collision_max.y;
    api.lower_position.y = api.collision_min.y;
    api.collision_origin.y = api.collision_min.y + predicted_vertical_displacement;

    object->ai.terrain_origin = api.collision_origin;
    const u32 context_flags = CInfo[object->character_context].flags;
    if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_TOP) != 0 &&
        (object->context_variant_flags & 0x08) != 0) {
        object->ai.terrain_origin.y += api.collision_max.y - api.collision_min.y;
    } else if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_POSITION) != 0) {
        object->ai.terrain_origin = api.position;
    } else {
        object->ai.terrain_origin.y = api.collision_origin.y - object->terrain_origin_floor_offset;
        if (object->ai.terrain_origin.y < api.field_0x218 &&
            api.field_0x218 < api.collision_origin.y + api.scaled_height) {
            object->ai.terrain_origin.y = api.field_0x218;
        }
    }
}

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene) {
    if (enabled != 0) {
        object->portal_enabled = 1;
        object->portal_update_room = update_room;
        if (room != -1)
            object->room_id = room;
        else
            object->room_id = scene != NULL ? NuPortalWhichRoom(scene, &object->apiobj.position) : -1;
    } else {
        object->portal_enabled = 0;
    }
}

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface) {
    object->apiobj.position = *position;
    object->apiobj.field_0x276 = angle;
    object->apiobj.facing_angle = angle;
    object->apiobj.movement_facing_angle = angle;
    object->apiobj.initial_position = object->apiobj.position;
    object->apiobj.collision_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.respawn_position = object->apiobj.position;
    object->apiobj.last_safe_position = object->apiobj.position;
    object->ai_update_position = object->apiobj.position;
    object->reset_velocity = v000;
    object->apiobj.velocity = v000;
    InitSurfaceInfo(object);
    if (set_on_surface != 0) {
        SetObjOnSurface(object, 0);
    }
    if (path_info != NULL) {
        object->ai.path_info = *path_info;
    } else {
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
    }
}

void UpdateLastSafePosition(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    object->field_0xf02 &= ~0x40;
    if (api.field_0x287 == 0 && (LEGOCONTEXT_DOOMED == -1 || object->character_context != LEGOCONTEXT_DOOMED) &&
        (api.flags_low & 4) == 0) {
        api.field_0x1c0 = api.position;
        GameObject_s *terrain_object = object->field_0xcc0 != NULL && LEGOCONTEXT_BEENTAKENOVER != -1 &&
                                               object->character_context == LEGOCONTEXT_BEENTAKENOVER
                                           ? object->field_0xcc0
                                           : object;
        const i8 surface = terrain_object->apiobj.field_0x281;
        i32 unsafe;
        if (surface != -1 && (TerSurface[surface].flags & 0xc041) != 0) {
            unsafe = 1;
        } else {
            const i8 layer = terrain_object->apiobj.field_0x27f;
            unsafe = 0;
            if (layer != -1 && ((TerLayer[layer].flags & 1) != 0 || layer == 9))
                unsafe = 1;
        }
        if (unsafe && VehicleArea != 0 && BonusArea != 0)
            unsafe =
                !(static_cast<GAMECHARACTERDATA *>(terrain_object->apiobj.character_data->field11_0x24)->field_0x28 >
                  0.0f);
        if (WORLD->current_level == NEWTOWN_LDATA && terrain_object->field_0xe31 == 1 &&
            terrain_object->apiobj.position.y > 0.7f)
            unsafe = true;
        do {
            if ((object->field_0xf00 & 4) != 0)
                break;
            const i8 context = object->character_context;
            if ((CInfo[context].flags & (8ull << 32)) != 0)
                break;
            if (api.field_0x27d == 0 &&
                (context == -1 || (context != LEGOCONTEXT_BEENTAKENOVER && context != LEGOCONTEXT_EATEN &&
                                   context != LEGOCONTEXT_GETIN)) &&
                VehicleArea == 0 && object->id != id_TRAININGREMOTE) {
                const u32 flags = api.character_data->model_flags;
                if (((flags & 0x2000) == 0 ||
                     !(static_cast<GAMECHARACTERDATA *>(api.character_data->field11_0x24)->field_0x28 > 0.0f)) &&
                    ((flags & 0x8000) == 0 || object->field_0xe31 != 1))
                    break;
            }
            if (!(object->surface_normal.y > 0.9f) || api.field_0x218 == 2000000.0f)
                break;
            const i16 platform = object->field_0x1078;
            i16 shadow_platform;
            if (!((platform == -1 || platform == LevSafePlatID[0] || platform == LevSafePlatID[1]) &&
                  ((shadow_platform = api.supporting_platform_id) == -1 || shadow_platform == LevSafePlatID[0] ||
                   shadow_platform == LevSafePlatID[1])) &&
                (LastSafePosExtraFn == NULL || !LastSafePosExtraFn(object)))
                break;
            if (__builtin_expect(!(api.lower_position.y > api.field_0x218 - 0.1f) || unsafe ||
                                     (VehicleArea != 0 && TouchHacks::CheckForAboutToRunIntoKillTerrain(*object, 1.0f)),
                                 0))
                break;
            object->field_0xf02 |= 0x40;
            api.last_safe_position = api.position;
            object->fall_recovery_timer = 0.0f;
            if ((object->ai.path_info.flags & 1) != 0 && (object->ai.path_info.connection->traversal_flags[1] |
                                                          object->ai.path_info.connection->traversal_flags[0]) == 0) {
                ClearLastSafeTakeoverSource(object);
                api.respawn_position = api.last_safe_position;
                api.respawn_enabled = 1;
            }
            if (__builtin_expect(static_cast<u8>(api.field_0x27c) < 2 && object->field_0xda8 <= 0.0f &&
                                     GameCam->blend_time >= GameCam->blend_duration &&
                                     object->sock_position.location.sock != -1,
                                 1)) {
                ClearLastSafeTakeoverSource(object);
                object->last_safe_camera_position = api.last_safe_position;
            }
            goto safe_position_done;
        } while (false);
        {
            i8 edge_layer;
            if (__builtin_expect((api.flags_low & 0x80) != 0, 1) && (edge_layer = api.field_0x27f) != -1 &&
                __builtin_expect((TerLayer[edge_layer].flags & 1) != 0, 1) && !NoLayerKill(object)) {
                if (object->fall_recovery_timer <= 0.0f)
                    object->fall_hover_height = api.position.y;
                object->fall_recovery_timer += FRAMETIME;
            } else {
                object->fall_recovery_timer = 0.0f;
            }
            if ((WORLD->current_level->flags & 0x40000) != 0) {
                api.last_safe_position = api.position;
            }
        }
    }
safe_position_done:
    if (!object->ignore_last_safe_path_pos && !api.player_controlled &&
        (LEGOCONTEXT_EATEN == -1 || object->character_context != LEGOCONTEXT_EATEN) && (api.flags_high & 0x20) != 0) {
        if ((object->ai.path_info.flags & 1) != 0)
            ClearLastSafeTakeoverSource(object);
        api.last_safe_position = api.respawn_position;
    }
    object->field_0xf00 &= ~4;
}

void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter) {
    object->can_use_object = target;
    object->use_action = action;
    object->use_action_parameter = parameter;
    object->use_action_frames = grapple_attach_frames;
}

