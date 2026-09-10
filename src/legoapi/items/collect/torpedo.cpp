#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void PartDraw_Torp(PART_s *) {
}

extern i16 id_YWING, id_MINIYWING, id_TIEBOMBER, id_MINITIEBOMBER, id_MINISTARDESTROYER;

i32 getMaxTorpedos(GameObject_s *object) {
    if (object != NULL && (object->id == id_YWING || object->id == id_MINIYWING || object->id == id_TIEBOMBER ||
                           object->id == id_MINITIEBOMBER || object->id == id_MINISTARDESTROYER))
        return 5;
    return 3;
}

void AddTorpedoAsPart(nuvec_s *, nuvec_s *, float, float) {
}

void PodCollisionCode(GameObject_s *) {
}

void FreeTorpedoPacket(TORPEDOPACKET_s **) {
}

extern i16 id_ATAT;
void GetShootOrigin_LSW(GameObject_s *object, nuvec_s *position) {
    *position = object->apiobj.collision_position;
    if (object->id == id_ATAT) {
        u16 angle = object->apiobj.field_0x276;
        f32 scale = object->apiobj.field_0x1dc;
        position->x += (NU_SIN_LUT(angle) * scale) * 1.5f;
        position->z += (scale * NU_COS_LUT(angle)) * 1.5f;
    }
}

void InitTorpedoPackets() {
}

extern i16 id_ATAT, id_CLONEWALKER, id_ATST, id_ATST_LOWRES;
i32 GetShootDirection_LSW(GameObject_s *object, nuvec_s *direction) {
    NUVEC temporary;
    if (direction == NULL)
        direction = &temporary;
    if (object->field_0x1086 == 4) {
        NUMTX matrix = object->apiobj.field_0xb8;
        NuMtxPreRotateY(&matrix, 0x8000);
        NuVecMtxRotate(direction, &v001, &matrix);
        return object->apiobj.facing_angle;
    }
    characterdata_s *model = object->apiobj.character_data;
    GAMECHARACTERDATA *data = model->game_character;
    if (data->weapon_shoot_joints[0] != -1 &&
        ((object->id == id_ATAT && (object->apiobj.flags_low & 0x80) == 0) || object->id == id_CLONEWALKER ||
         object->id == id_ATST || object->id == id_ATST_LOWRES)) {
        direction->x = direction->y = 0.0f;
        direction->z = object->id == id_ATAT ? 1.0f : -1.0f;
        NuVecMtxRotate(direction, direction, &object->joint_matrices[data->weapon_shoot_joints[0]]);
        return NuAtan2D(direction->x, direction->z);
    }
    i32 angle;
    if ((model->model_flags & 0x2000) != 0 || (data->flags_090 & 0x80) != 0) {
        angle = object->apiobj.facing_angle;
        if (object->character_context == 0x2a &&
            1.0f - object->context_animation_timer / object->airborne_action_duration >= 0.25f)
            angle -= 0x8000;
    } else
        angle = object->apiobj.movement_facing_angle;
    direction->x = NuTrigTable[static_cast<u16>(angle) >> 1];
    direction->y = 0.0f;
    direction->z = NuTrigTable[((static_cast<u16>(angle) + 0x4000) >> 1) & 0x7fff];
    return angle;
}
