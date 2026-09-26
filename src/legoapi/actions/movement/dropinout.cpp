#include "decomp.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "globals.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 drop_in_teleport;
extern i32 teleport_all_freeplay_modes;

void ResetPlayerMoves(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);
void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);
void ReleaseEat(GameObject_s *object);
void AICreatureResumeScript(GameObject_s *object);

void StartDropIn(GameObject_s *object) {
    if (WORLD->current_level == PODSPRINTA_LDATA || (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area)) {
        return;
    }

    object->character_context = CHARACTER_CONTEXT_DROP_IN;
    const i16 animation = (object->apiobj.character_data->model_flags & 0x2000) == 0 ? 5 : 1;
    object->drop_transition_time = 0.0f;
    object->drop_transition_duration = 0.5f;
    object->context_animation = animation;
}

void StartDropOut(GameObject_s *object) {
    if (WORLD->current_level == PODSPRINTA_LDATA || (PODRACE_ADATA != NULL && PODRACE_ADATA == WORLD->area)) {
        return;
    }

    object->character_context = CHARACTER_CONTEXT_DROP_OUT;
    const i16 animation = CurrentAnim(&object->apiobj.anim_packet);
    object->drop_transition_time = 0.0f;
    object->drop_transition_duration = 0.5f;
    object->context_animation = animation;
}

void DropInOutCode(GameObject_s *object) {
    const u8 context = static_cast<u8>(object->character_context);
    if (context != CHARACTER_CONTEXT_DROP_IN && context != CHARACTER_CONTEXT_DROP_OUT) {
        return;
    }

    object->drop_transition_time += FRAMETIME;
    if (object->drop_transition_time < object->drop_transition_duration) {
        if (WORLD->current_level == PODSPRINTA_LDATA) {
            GameObject_s *other = Player[0] == object ? Player[1] : Player[0];
            object->apiobj.position = other->apiobj.position;
        }
        return;
    }
    if (context == CHARACTER_CONTEXT_DROP_OUT) {
        GameObject_s *other = Player[0] == object ? Player[1] : Player[0];
        object->apiobj.position = other->apiobj.position;
    }
    object->character_context = -1;
    if ((object->apiobj.character_data->model_flags & 0x2000) == 0) {
        object->apiobj.field_0x27d = 0;
        object->apiobj.velocity.y = -0.1f;
    }
}

f32 DropInOutScale(GameObject_s *object) {
    const u8 context = static_cast<u8>(object->character_context);
    if (context == CHARACTER_CONTEXT_DROP_IN) {
        const f32 angle = object->drop_transition_time / object->drop_transition_duration * 16384.0f;
        return NuTrigTable[(static_cast<i32>(angle) >> 1) & 0x7fff];
    }
    if (context == CHARACTER_CONTEXT_DROP_OUT) {
        const f32 angle = object->drop_transition_time / object->drop_transition_duration * 16384.0f + 16384.0f;
        return NuTrigTable[(static_cast<i32>(angle) >> 1) & 0x7fff];
    }
    return 1.0f;
}

i32 FreePlay_DropInToPlayerPos(GameObject_s *object) {
    i32 result = 0;
    if ((WORLD == NULL || WORLD->current_level != FACTORYF_LDATA) && player != NULL && FreePlay != 0 &&
        drop_in_teleport > 1 && VehicleArea == 0 && (Arcade != 0 || teleport_all_freeplay_modes != 0) &&
        object->field_0xcc0 == NULL && (CInfo[static_cast<i8>(object->character_context)].flags & 0x8000) == 0 &&
        object->character_context != 0x2b && object->apiobj.model_draw_result == 0 && (object->tag_flags & 2) == 0) {
        NUVEC position = player->apiobj.last_safe_position;
        if (position.x == player->apiobj.position.x && position.y == player->apiobj.position.y &&
            position.z == player->apiobj.position.z) {
            position.z += 0.01f;
        }

        if (NuCameraClipTestSphere(&position, object->apiobj.field_0x1e0, &numtx_identity) == 0) {
            ResetPlayerMoves(object);
            Player_ClearContext(object, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
            object->apiobj.start_position = position;
            object->apiobj.position = position;
            object->apiobj.velocity = v000;
            object->apiobj.field_0x276 = player->apiobj.field_0x276;
            ResetPlayerMoves(object);
            object->apiobj.movement_stuck_time = 0.0f;
            InitSurfaceInfo(object);
            SetObjOnSurface(object, 1);
            GameObjectOrigin(object);
            AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
            result = 1;
        }
    }
    return result;
}

void DropOut(i32 player_index, i32 resume, i32 silent, i32) {
    GameObject_s *object = Player[player_index];
    if (object == NULL) {
        return;
    }

    if (silent == 0) {
        if (object->field_0xcc0 == NULL) {
            NuPadSetStatus(object->pad_gamepad - GamePad, 0);
        }
        GameAudio_PlaySfx(0x20, &object->apiobj.collision_position, 0, 0);
    }

    ReleaseTakeOver(object, 1);
    const i16 secondary_lean_angle = object->secondary_lean_angle;
    const i16 tertiary_lean_angle = object->tertiary_lean_angle;
    const i16 movement_lean_angle = object->movement_lean_angle;
    ReleaseEat(object);
    if (VehicleArea != 0) {
        ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet),
                          reinterpret_cast<CHARACTERDATA_s *>(object->apiobj.character_data));
        object->secondary_lean_angle = secondary_lean_angle;
        object->movement_lean_angle = movement_lean_angle;
        object->tertiary_lean_angle = tertiary_lean_angle;
    }

    object->apiobj.flags_low &= 0x7f;
    AICreatureResumeScript(object);
    i32 vehicle_area = 0;
    if (VehicleArea != 0) {
        StartDropOut(object);
        vehicle_area = VehicleArea;
    }

    u8 hitpoints;
    if (object->apiobj.field_0x287 != 0 || object->character_context == 0x2b) {
        hitpoints = object->hitpoints;
    } else {
        hitpoints = object->current_hp;
    }
    PlayerProgress[player_index].hitpoints = hitpoints;
    object->field_0xdec = 0.0f;
    if (vehicle_area == 0) {
        GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
    } else {
        GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    }
    if (resume != 0) {
        ResumeGame(1, 1);
    }

    GameObject_s *other = Player[player_index == 0];
    if (other != NULL) {
        NUVEC direction;
        NuVecSub(&direction, &other->apiobj.collision_position, &GameCam->pos);
        if (GameRayCast(&GameCam->pos, &direction, 0.0f, 0x1f) != 0) {
            GameCam_Reset(GameCam);
        }
    }
}
