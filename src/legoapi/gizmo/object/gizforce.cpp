#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GetZapOrigin(GameObject_s *) {
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

void SetForceBack(GameObject_s *, nuvec_s *, float, i32) {
}

void ResetForceBack() {
    ForceBackObj = NULL;
    ForceBackPos = NULL;
}

void ResetForceGlow(PLAYERPACKET_s *packet) {
    packet->force_glow_x = 0.0f;
    packet->force_glow_y = 0.0f;
    packet->force_glow_z = 0.0f;
    packet->force_glow_object = NULL;
    packet->force_glow_candidate = NULL;
    packet->force_glow_intensity = 0.2f;
}

void ForceLightning_Origin(GameObject_s *, nuvec_s *, nuvec_s *) {
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

void GizForceSFX_Configure(WORLDINFO_s *world, char *config) {
    (void)world;
    (void)config;
}
