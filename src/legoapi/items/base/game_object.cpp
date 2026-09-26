#include "legoapi/world/world_shared.h"

#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>
#include <new>

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;
struct BOLT_s;
struct debinftype;

extern void SetGameObjectCharacterData(GameObject_s *obj);
extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);
extern void Hub_ResetPanel();
extern f32 VehicleTurnOrLoopOffset(GameObject_s *object);
extern void StartTurn(GameObject_s *object);

extern "C" {
    extern i16 id_MOSEISLEYCITIZEN;
    extern i16 id_CANTINAALIEN;
    extern i16 id_CLOUDCITYCITIZEN;
    extern i16 id_GEONOSIAN;
    extern i16 id_BOB;
    extern i16 id_SPEEDERBIKE;
    extern i16 id_SPEEDERBIKESNOW;
    extern i16 id_STAP;
    extern i16 id_STAP2;
    extern i16 id_TROOPERCANNON;
    extern i16 id_CANNON;
    extern i16 id_MOSCANNON;
    extern i16 id_ATST;
    extern i16 id_BASKETCANNON;
    extern i16 id_BIGGUN;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_GAMORREANGUARD;
    extern i16 id_ZAMSSPEEDER;
}

i32 addcreature_override_id_check;
f32 default_mover_extra = 0.05f;
f32 trench_max_height_move = 5.0f;
f32 trench_roll_f = 5000.0f;
f32 trench_seek_z = 0.5f;
f32 trench_seek_y = 0.5f;
f32 trench_seek_x = 4.0f;
extern f32 TURNTIME;
f32 LOOPTIME = 1.5f;

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
void TrenchMove(GameObject_s *) asm("_ZL10TrenchMoveP12GameObject_s") __attribute__((visibility("hidden")));
void TrenchKilledCallback(GameObject_s *) asm("_ZL20TrenchKilledCallbackP12GameObject_s")
    __attribute__((visibility("hidden")));

__used__ void TrenchMove(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    object->pad_gamepad->previous_input_angle = object->pad_gamepad->input_angle;
    object->pad_gamepad->previous_input_magnitude = object->pad_gamepad->input_magnitude;
    object->previous_movement_angle = api.field_0x276;
    object->field_0xefd &= ~0x40;
    object->field_0x1086 = 0;
    if (api.model_draw_result != 0)
        object->field_0xf1c = 0.0f;
    else
        object->field_0xf1c += FRAMETIME;
    if (object->character_context == 0x2b)
        object->turn_braking += FRAMETIME;

    if (object->character_context != 0x2a) {
        if ((api.movement_facing_angle > 0x8000) != (player->apiobj.movement_facing_angle > 0x8000) ||
            (player->character_context == 0x2a &&
             (api.movement_facing_angle <= 0x8000) != (player->apiobj.movement_facing_angle > 0x8000) &&
             player->context_animation_timer < 0.5f * TURNTIME)) {
            StartTurn(object);
        } else if (object->character_context != 0x36 && player->character_context == 0x36 &&
                   player->context_animation_timer < 0.75f * LOOPTIME) {
            object->character_context = 0x36;
            object->context_animation = 1;
            object->airborne_action_duration = LOOPTIME;
            object->context_animation_timer = LOOPTIME;
        }
    }

    f32 target_x;
    if (object->character_context == 0x2a) {
        const f32 phase = object->context_animation_timer / TURNTIME;
        if (api.movement_facing_angle > 0x8000)
            target_x = phase * 10.0f + (1.0f - phase) * -10.0f + trenchrun.position.x;
        else
            target_x = phase * -10.0f + (1.0f - phase) * 10.0f + trenchrun.position.x;
    } else if (api.movement_facing_angle > 0x8000) {
        target_x = trenchrun.position.x + 10.0f;
    } else {
        target_x = trenchrun.position.x - 10.0f;
    }
    api.position.x = SeekValF(api.position.x, target_x, trench_seek_x);
    f32 target_y = trenchrun.position.y + object->movement_spline_offset.y + VehicleTurnOrLoopOffset(object);
    if (target_y - api.position.y > trench_max_height_move)
        target_y = api.position.y + trench_max_height_move;
    api.position.y = SeekValF(api.position.y, target_y, trench_seek_y);
    api.position.z = SeekValF(api.position.z, trenchrun.position.z + object->movement_spline_offset.z, trench_seek_z);
    NuVecSub(&api.velocity, &api.position, &api.start_position);
    NuVecScale(&api.velocity, &api.velocity, 1.0f / FRAMETIME);

    if ((api.character_data->game_character->flags_090 & 1) != 0) {
        i32 roll = 0;
        if (object->character_context != 0x36 && object->character_context != 0x2a &&
            object->character_context != 0x3a) {
            roll = static_cast<i32>(api.velocity.z * trench_roll_f);
        }
        if (static_cast<i16>(player->apiobj.movement_facing_angle) >= 0)
            roll = -roll;
        i32 target_roll;
        if (roll < -0x10000) {
            target_roll = -0x2000;
        } else if (roll > 0x10000) {
            target_roll = 0x2000;
        } else {
            target_roll = roll / 4;
            if (target_roll < -0x2000)
                target_roll = -0x2000;
            else if (target_roll > 0x2000)
                target_roll = 0x2000;
        }
        object->movement_lean_angle = SeekRot(object->movement_lean_angle, target_roll, 8.0f);
    }
    APIObjectVelocities(object);
    GameObjectOrigin(object);
}

__used__ void TrenchKilledCallback(GameObject_s *object) {
    for (i32 i = 0; i < 3; ++i) {
        if (trenchrun.objects[i] == object) {
            trenchrun.objects[i] = NULL;
            break;
        }
    }
}

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *object) {
    if (WORLD->current_level == CRUISERE_LDATA && object->field_0x1020 == 2000000.0f &&
        object->apiobj.position.x < 11.0f) {
        object->field_0x1087 = 3;
        object->field_0x1020 = -39.2f;
    }
    if (WORLD->current_level == DEATHSTARRESCUED_LDATA) {
        if (object->apiobj.position.z > 20.75f) {
            object->field_0x1020 = 22.4f;
            object->field_0x1087 = 3;
        }
        if (object->apiobj.position.x < -20.75f) {
            object->field_0x1087 = 1;
            object->field_0x1020 = -22.4f;
        }
    }
}

static __used__ void PauseGame_ExtraCode() {
    Hub_ResetPanel();
}

static __used__ i32 SpecialObjectFilter(void *object) {
    return theSceneObjectHelper.scene_id == static_cast<SpecialObject *>(object)->scene_id;
}
