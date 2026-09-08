#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/door/plugs.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/qrand.h"
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void GizBlowup_Respawn(GIZMOBLOWUP_s *);
void GizmoBlowUp_AddEffects(NUVEC *, GIZMOBLOWUP_s *, i32, i32, GameObject_s *);
void DrawObjectOnCharacter(WORLDINFO_s *, GameObject_s *, i32, nuhspecial_s *, i32, i32, NUMTX *, i32, u32, NUMTX *,
                           NUVEC *, f32, f32);
void QuatInterpolateRotationMatrix(NUMTX *, NUMTX *, NUMTX *, f32);
extern i32 dco_locatorposonly;
extern ADDPART_s Default_ADDPART;
extern "C" PART_s *AddPart(ADDPART_s *);
u16 ObjHitObj_Flags(GameObject_s *);
f32 SUPERCARRY_THROWSPEED_XZ = 3.0f;
f32 SUPERCARRY_RELEASESPEED_XZ = 2.0f;
f32 SUPERCARRY_DROPSPEED_XZ = 0.2f;
f32 SUPERCARRY_DROPSPEED_Y = 1.0f;
f32 SUPERCARRY_THROWSPEED_Y = 3.0f;
f32 SUPERCARRY_OBJGRAVITY = -6.0f;
// Original defaults: 0x668d30 and 0x668d40, each a four-byte integer.
i32 SuperCarry_AlignObject = 1;
i32 SuperCarry_KeepObjectLevel = 1;

// Original 0x4fab00, 184 bytes.
static __used__ void SuperCarry_PartImpact(PART_s *part) {
    f32 intensity = NuVecMag(&part->velocity);
    if (intensity >= 4.0f)
        intensity = 1.0f;
    else
        intensity *= 0.25f;
    if (intensity > 0.0f) {
        const i32 axis = qrand() > 0x7fff ? 2 : 0;
        GameCam_Judder(NULL, intensity * 0.3f, axis, &part->position);
    }
}

// Original 0x4fabc0, 347 bytes. The original returns an integer.
static __used__ i32 SuperCarry_TurnBlowupBackOn(GIZMOBLOWUP_s *blowup, NUVEC *position, u16 angle, i32 keep_matrix) {
    blowup->state_flags |= 0x80;
    blowup->visibility_flags |= 0x40;
    GizBlowup_Respawn(blowup);
    blowup->state_flags |= 1;
    f32 distance;
    PLUG *plug = Plug_FindNearest(WORLD->plug_sys, position, &distance, 1);
    if (plug != NULL && distance < 0.5625f) {
        Plug_MakeDrawMtx(plug, &blowup->transform);
        plug->flags |= PLUG_FLAG_PLUGGED;
        blowup->state_flags &= static_cast<u8>(~0x80);
        blowup->field_0x9f |= 0x10;
        return 1;
    }
    if (keep_matrix == 0) {
        if (SuperCarry_AlignObject != 0)
            NuMtxSetRotationY(&blowup->transform, angle);
        else
            blowup->transform = numtx_identity;
        NuMtxTranslate(&blowup->transform, position);
    }
    return 0;
}

// Original 0x4fad20, 112 bytes.
static __used__ void SuperCarry_PartKill(PART_s *part, i32) {
    GIZMOBLOWUP_s *blowup = part->carried_blowup;
    if ((blowup->secondary_flags & 4) != 0)
        SuperCarry_TurnBlowupBackOn(blowup, &part->position, part->rotation_y, 0);
    else
        GizmoBlowUp_AddEffects(&part->position, blowup, 0, 15, NULL);
}

// Original 0x4fad90, 673 bytes.
void SuperCarry_DrawObject(GameObject_s *object) {
    if (static_cast<i8>(object->field_0xe22) >= 0) {
        if (LEGOCONTEXT_SUPERCARRY == -1 || object->character_context != LEGOCONTEXT_SUPERCARRY)
            return;
        switch (object->field_0x7a3) {
            case 0:
                if ((object->context_flags & 0x40) == 0)
                    return;
                break;
            case 1:
            case 4:
            case 5:
                if ((object->context_flags & 0x40) != 0)
                    return;
                break;
            case 2:
            case 3:
            case 6:
            case 7:
                break;
            default:
                return;
        }
    }
    NUMTX matrix;
    NUVEC offset;
    NUVEC *translation = NULL;
    if (SuperCarry_AlignObject != 0) {
        NuVecRotateY(&offset, &object->carried_object_basis[0], object->apiobj.field_0x276);
        NuMtxSetRotationY(&matrix, object->carried_object_angle);
        translation = &offset;
    } else {
        matrix = numtx_identity;
        *NUMTX_GET_ROW_VEC(&matrix, 0) = object->carried_object_basis[0];
        *NUMTX_GET_ROW_VEC(&matrix, 1) = object->carried_object_basis[1];
        *NUMTX_GET_ROW_VEC(&matrix, 2) = object->carried_object_basis[2];
    }
    if (SuperCarry_KeepObjectLevel != 0) {
        NuMtxRotateY(&matrix, object->apiobj.field_0x276);
        dco_locatorposonly = 1;
    }
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
    DrawObjectOnCharacter(WORLD, object, -1, &blowup->type->special, config->hand_joints[0], config->hand_joints[1],
                          object->joint_matrices, object->field_0x1088, object->field_0x1054, &matrix, translation,
                          1.0f, 1.0f);
}

// Original 0x4fb040, 410 bytes. Aligned carry uses the vector storage as offsets.
void SuperCarry_GetObjectPos(GameObject_s *object, NUVEC *position, NUVEC *secondary_position) {
    *position = object->apiobj.collision_position;
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    const i32 first = config->hand_joints[0];
    if (first != -1 && object->apiobj.character_model->points_of_interest[first] != NULL) {
        const i32 second = config->hand_joints[1];
        if (second != -1 && object->apiobj.character_model->points_of_interest[second] != NULL) {
            *position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[first], 3);
            NUVEC *other = NUMTX_GET_ROW_VEC(&object->joint_matrices[second], 3);
            position->x += (other->x - position->x) * 0.5f;
            position->y += (other->y - position->y) * 0.5f;
            position->z += (other->z - position->z) * 0.5f;
        }
    }
    if (SuperCarry_AlignObject != 0) {
        NUVEC offset;
        if (secondary_position != NULL) {
            NuVecRotateY(&offset, &object->carried_object_basis[1], object->apiobj.field_0x276);
            NuVecAdd(secondary_position, position, &offset);
        }
        NuVecRotateY(&offset, &object->carried_object_basis[0], object->apiobj.field_0x276);
        NuVecAdd(position, position, &offset);
    }
}

// Original 0x4fb1e0, 2147 bytes.
void SuperCarry_Throw(GameObject_s *object, i32 mode) {
    PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
    const i32 first = config->hand_joints[0];
    if (first == -1 || object->apiobj.character_model->points_of_interest[first] == NULL)
        return;
    NUMTX matrix;
    if (SuperCarry_KeepObjectLevel != 0) {
        if (SuperCarry_AlignObject != 0) {
            NuMtxSetRotationY(&matrix, object->carried_object_angle);
        } else {
            matrix = numtx_identity;
            *NUMTX_GET_ROW_VEC(&matrix, 0) = object->carried_object_basis[0];
            *NUMTX_GET_ROW_VEC(&matrix, 1) = object->carried_object_basis[1];
            *NUMTX_GET_ROW_VEC(&matrix, 2) = object->carried_object_basis[2];
        }
        NuMtxRotateY(&matrix, object->apiobj.field_0x276);
        NuMtxTranslate(&matrix, NUMTX_GET_ROW_VEC(&object->joint_matrices[first], 3));
    } else {
        matrix = object->joint_matrices[first];
    }
    const i32 second = object->apiobj.character_data->player_config->hand_joints[1];
    if (second != -1 && object->apiobj.character_model->points_of_interest[second] != NULL) {
        NUVEC position = *NUMTX_GET_ROW_VEC(&matrix, 3);
        if (SuperCarry_KeepObjectLevel == 0) {
            NUMTX left = matrix;
            NUMTX right = object->joint_matrices[second];
            left.m30 = left.m31 = left.m32 = 0.0f;
            right.m30 = right.m31 = right.m32 = 0.0f;
            QuatInterpolateRotationMatrix(&matrix, &left, &right, 0.5f);
        }
        matrix.m30 = (position.x + object->joint_matrices[second].m30) * 0.5f;
        matrix.m31 = (position.y + object->joint_matrices[second].m31) * 0.5f;
        matrix.m32 = (position.z + object->joint_matrices[second].m32) * 0.5f;
    }
    if (SuperCarry_KeepObjectLevel == 0) {
        NUVEC position = *NUMTX_GET_ROW_VEC(&matrix, 3);
        NUMTX basis = numtx_identity;
        *NUMTX_GET_ROW_VEC(&basis, 0) = object->carried_object_basis[0];
        *NUMTX_GET_ROW_VEC(&basis, 1) = object->carried_object_basis[1];
        *NUMTX_GET_ROW_VEC(&basis, 2) = object->carried_object_basis[2];
        NuMtxMulR(&matrix, &basis, &matrix);
        *NUMTX_GET_ROW_VEC(&matrix, 3) = position;
    } else if (SuperCarry_AlignObject != 0) {
        NUVEC offset;
        NuVecRotateY(&offset, &object->carried_object_basis[0], object->apiobj.field_0x276);
        NuMtxTranslate(&matrix, &offset);
    }
    ADDPART_s params = Default_ADDPART;
    params.matrix = &matrix;
    NUVEC velocity;
    f32 speed;
    if (mode == 0)
        speed = SUPERCARRY_THROWSPEED_XZ;
    else if (mode == 1)
        speed = SUPERCARRY_RELEASESPEED_XZ;
    else
        speed = SUPERCARRY_DROPSPEED_XZ;
    velocity.x = NU_SIN_LUT(object->apiobj.facing_angle) * speed;
    velocity.z = NU_COS_LUT(object->apiobj.facing_angle) * speed;
    velocity.y = mode == 2 ? SUPERCARRY_DROPSPEED_Y : SUPERCARRY_THROWSPEED_Y;
    params.velocity = &velocity;
    GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
    params.field_14 = params.field_18 = 0.5f * blowup->target_scale;
    params.special = &blowup->type->special;
    params.flags = (blowup->secondary_flags & 4) != 0 ? 0x8011 : 0x8111;
    params.owner = object;
    params.time_step = FRAMETIME;
    params.gravity = SUPERCARRY_OBJGRAVITY;
    params.field_3c = SuperCarry_PartImpact;
    params.field_a4 = 3.0f;
    params.field_44 = SuperCarry_PartKill;
    params.lighting = reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
    PART_s *part = AddPart(&params);
    if (part != NULL) {
        part->force_flags = static_cast<u16>(ObjHitObj_Flags(object));
        part->carried_blowup = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
        part->rotation_y = object->carried_object_angle + object->apiobj.movement_facing_angle;
    }
}
