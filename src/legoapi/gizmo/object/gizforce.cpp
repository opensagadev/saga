#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

NUVEC *GetZapOrigin(GameObject_s *object) {
    NUVEC *origin = &object->apiobj.collision_position;
    if (object->apiobj.field_0x288 != 0) {
        PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
        i32 joint = config->weapon_shoot_joints[0];
        if (joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL) {
            joint = config->weapon_joints[0];
        }
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            origin = reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
        }
    }
    return origin;
}

extern f32 ForceThrowGravity;
void EndForce(GameObject_s *, i32);
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);

void ReleaseForce(GameObject_s *object, i32 mode) {
    if (object->character_context == 0x1b) {
        if (object->force_target != NULL) {
            if (object->force_target->character_context == 0x1c) {
                object->force_target->character_context = -1;
                object->force_target->force_target = NULL;
            }
            object->force_target = NULL;
        }
    } else if (object->character_context == 0x1d) {
        if (object->force_part != NULL) {
            object->force_part->force_player_mask &= ~(1u << (object->apiobj.field_0x27c & 31));
            if (object->force_part->force_player_mask == 0)
                object->force_part->force_player_mask = -1;
            object->force_part->gravity = ForceThrowGravity;
            object->force_part = NULL;
        }
        object->character_context = -1;
    } else if (object->character_context == 8) {
        if ((object->apiobj.flags_low & 0x80) != 0)
            GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
    } else
        return;
    EndForce(object, mode);
}

void ResetForceGlow(PLAYERPACKET_s *packet) {
    packet->force_glow_x = 0.0f;
    packet->force_glow_y = 0.0f;
    packet->force_glow_z = 0.0f;
    packet->force_glow_object = NULL;
    packet->force_glow_candidate = NULL;
    packet->force_glow_intensity = 0.2f;
}

void ForceLightning_Origin(GameObject_s *object, NUVEC *primary, NUVEC *secondary) {
    *primary = object->apiobj.collision_position;
    if (secondary != NULL)
        secondary->y = 1000000000.0f;
    if (object->apiobj.field_0x288 == 0)
        return;
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    i32 joint = config->hand_joints[0];
    if (joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL)
        return;
    *primary = *reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
    if (secondary == NULL || (object->weapon_scale != 0.0f && object->weapon_scale_state != 2))
        return;
    joint = config->hand_joints[1];
    if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
        *secondary = *reinterpret_cast<NUVEC *>(&object->joint_matrices[joint].m30);
    }
}

void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);

void EndForce(GameObject_s *object, i32) {
    if (object != NULL && (object->apiobj.field_0x1f4 & 0x40000) == 0) {
        object->force_glow_object = NULL;
        object->field_0xe22 &= ~2;
        object->force_glow_candidate = NULL;
        object->character_context = -1;
    }
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}
