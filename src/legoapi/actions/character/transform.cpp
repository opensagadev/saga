#include "legoapi/actions/character/transform.h"
#include "legoapi/gizmo/object/gizmoblowups.h"

#include "nu2api/numath/nuquat.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" i16 id_MINISLAVE1;

void TurnCodeCamSafe(GameObject_s *object, NUMTX *matrix);
void ResetForceGlow(PLAYERPACKET_s *packet);

void ApplyExtraRotation(GameObject_s *object, numtx_s *matrix) {
    switch (object->character_context) {
        case 42:
            TurnCodeCamSafe(object, matrix);
            break;
        case 58: {
            const f32 phase = 1.0f - object->context_animation_timer / object->airborne_action_duration;
            const f32 wave = NuTrigTable[(static_cast<i32>(32768.0f * phase + 16384.0f) >> 1) & 0x7fff];
            const i32 rotation = static_cast<i32>((1.0f - (wave + 1.0f) * 0.5f) * 65536.0f);
            if (object->field_0x7a3 == 0) {
                NuMtxPreRotateZ(matrix, rotation);
            } else {
                NuMtxPreRotateZ(matrix, -rotation);
            }
            break;
        }
    }

    if (object->id == id_MINISLAVE1) {
        NuMtxPreRotateX(matrix, -16384);
        NuMtxPreRotateY(matrix, 32768);
    }
}

void Transform_MoveCode(WORLDINFO_s *world, GameObject_s *object) {
    ResetForceGlow(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if (object->character_context != 86) {
        bool eligible = false;
        if ((object->apiobj.character_data->game_character->flags_090 & 0x01000000) != 0 &&
            object->apiobj.player_controlled != 0 && object->apiobj.field_0x27d != 0 && ObjLandReady(object)) {
            eligible = true;
        }
        if (!eligible) {
            return;
        }
        GIZMOBLOWUP_s *candidate = world->gizmo_blowups;
        if (candidate == NULL) {
            return;
        }
        f32 nearest_distance = 4.0f;
        NUVEC forward;
        NuVecRotateY(&forward, &v001, object->apiobj.movement_facing_angle);
        f32 min_x = object->apiobj.collision_position.x - 2.0f;
        f32 min_y = object->apiobj.collision_position.y - 2.0f;
        f32 min_z = object->apiobj.collision_position.z - 2.0f;
        f32 max_x = object->apiobj.collision_position.x + 2.0f;
        f32 max_y = object->apiobj.collision_position.y + 2.0f;
        f32 max_z = object->apiobj.collision_position.z + 2.0f;
        GIZMOBLOWUP_s *nearest = NULL;
        i32 candidate_count = world->gizmo_blowup_count;
        for (i32 i = 0; i < candidate_count; ++i, ++candidate) {
            bool valid = false;
            if ((candidate->status_flags & 0x0080c001) == 0x0080c000 && (candidate->draw_flags & 0x10) != 0) {
                valid = true;
                if ((candidate->draw_flags & 0x20) != 0 && ShadowMode == 0) {
                    valid = false;
                }
            }
            if (!valid) {
                continue;
            }
            if (candidate->mid_position.x < min_x || candidate->mid_position.x > max_x ||
                candidate->mid_position.z < min_z || candidate->mid_position.z > max_z ||
                candidate->mid_position.y < min_y || candidate->mid_position.y > max_y) {
                continue;
            }
            NUVEC delta;
            f32 distance = NuVecDistSqr(&candidate->mid_position, &object->apiobj.collision_position, &delta);
            if (distance < nearest_distance) {
                if (!(delta.x * forward.x + delta.z * forward.z > 0.0f)) {
                    candidate_count = world->gizmo_blowup_count;
                    continue;
                }
                nearest = candidate;
                nearest_distance = distance;
            }
            candidate_count = world->gizmo_blowup_count;
        }
        if (nearest != NULL) {
            object->force_glow_kind = 4;
            object->force_glow_position = nearest->mid_position;
            object->force_glow_object = nearest;
            object->field_0xd8c = nearest->target_scale;
            object->field_0xd80 = 1.0f;
            if ((object->pad_gamepad->buttons_held & GAMEPAD_SPECIAL) != 0) {
                object->field_0x788 = nearest;
                object->context_animation_timer = 0.0f;
                object->character_context = 86;
                object->context_animation = 160;
                object->context_flags &= ~0x40;
                f32 duration = AnimDuration(object->id, 160, 0.0f, 0.0f, 1);
                if (duration <= 0.0f) {
                    duration = 1.0f;
                }
                object->airborne_action_duration = duration;
                GIZMOBLOWUP_s *target = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
                object->apiobj.movement_facing_angle =
                    NuAtan2D(target->mid_position.x - object->apiobj.collision_position.x,
                             target->mid_position.z - object->apiobj.collision_position.z);
            }
        }
    } else {
        i32 trigger = 0;
        f32 *frame = NULL;
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
            frame = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0);
            if (frame == NULL) {
                return;
            }
        }
        GIZMOBLOWUP_s *target = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
        object->apiobj.movement_facing_angle = NuAtan2D(target->mid_position.x - object->apiobj.collision_position.x,
                                                        target->mid_position.z - object->apiobj.collision_position.z);
        if (frame != NULL && (object->context_flags & 0x40) == 0) {
            f32 marker = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
            if (marker >= 1.0f && *frame >= marker) {
                trigger = 1;
            }
        }
        object->context_animation_timer += FRAMETIME;
        if (object->context_animation_timer >= object->airborne_action_duration) {
            object->character_context = -1;
            if ((object->context_flags & 0x40) == 0) {
                trigger = 1;
            }
        }
        if (trigger != 0) {
            object->context_flags |= 0x40;
            GizmoBlowupBlowup(static_cast<GIZMOBLOWUP_s *>(object->field_0x788), 1, 8, 1, NULL, 1);
        }
    }
}

static inline void RotateTargetMatrix(NUMTX *matrix, i32 angle) {
    const f32 cosine = NuTrigTable[((16384 + angle) >> 1) & 0x7fff];
    const f32 sine = NuTrigTable[(angle >> 1) & 0x7fff];
    const f32 x0 = matrix->m00;
    const f32 x1 = matrix->m10;
    const f32 x2 = matrix->m20;
    const f32 x3 = matrix->m30;
    matrix->m00 = x0 * cosine - matrix->m01 * sine;
    matrix->m01 = x0 * sine + matrix->m01 * cosine;
    matrix->m10 = x1 * cosine - matrix->m11 * sine;
    matrix->m11 = x1 * sine + matrix->m11 * cosine;
    matrix->m20 = x2 * cosine - matrix->m21 * sine;
    matrix->m21 = x2 * sine + matrix->m21 * cosine;
    matrix->m30 = x3 * cosine - matrix->m31 * sine;
    matrix->m31 = x3 * sine + matrix->m31 * cosine;
}

void Transform_DrawTarget(nuvec_s *position, float radius, float alpha) {
    NUVEC direction;
    NuVecSub(&direction, position, NUMTX_GET_ROW_VEC(&pNuCam->mtx, 3));
    const f32 distance = NuFsqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    NuVecNorm(&direction, &direction);
    f32 offset = distance - 0.2f;
    if (offset > radius) {
        offset = radius;
    } else if (distance < 0.2f) {
        offset = 0.0f;
    }
    NuVecScale(&direction, &direction, offset);
    NUVEC draw_position;
    NuVecSub(&draw_position, position, &direction);
    if (offset != 0.0f) {
        radius *= (distance - offset) / distance;
    }
    NUVEC scale;
    scale.x = radius;
    scale.z = radius;
    scale.y = radius;
    NUMTX matrix;
    if (WORLD->lev_objs[0x49].active != 0) {
        NuMtxSetScale(&matrix, &scale);
        const u16 angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 7.31f) / 7.31f * 65536.0f);
        RotateTargetMatrix(&matrix, -angle);
        NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
        NuMtxTranslate(&matrix, &draw_position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[0x49].special, &matrix, alpha);
    }
    if (WORLD->lev_objs[0x4a].active != 0) {
        NuMtxSetScale(&matrix, &scale);
        const u16 angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 9.875f) / 9.875f * 65536.0f);
        RotateTargetMatrix(&matrix, -angle);
        NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
        NuMtxTranslate(&matrix, &draw_position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[0x4a].special, &matrix, alpha);
    }
}

GameObject_s *Transform_TargettedByObj(void *target) {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
            (object->apiobj.character_data->game_character->flags_090 & 0x01000000) != 0 &&
            object->field_0xd80 > 0.0f && object->field_0xd8c > 0.0f && object->force_glow_object == target) {
            return object;
        }
    }
    return NULL;
}

void GizmoBlowup_TransformDraw_Game(GIZMOBLOWUP_s *blowup) {
    if (Transform_TargettedByObj(blowup) != 0) {
        return;
    }

    Transform_DrawTarget(&blowup->mid_position, 1.4f * blowup->target_scale, 0.4f);
}

void InterpolateRotationMatrix(numtx_s *result, numtx_s *first, numtx_s *second, float fraction) {
    SeekVec(NUMTX_GET_ROW_VEC(result, 0), NUMTX_GET_ROW_VEC(first, 0), NUMTX_GET_ROW_VEC(second, 0), fraction);
    SeekVec(NUMTX_GET_ROW_VEC(result, 1), NUMTX_GET_ROW_VEC(first, 1), NUMTX_GET_ROW_VEC(second, 1), fraction);
    SeekVec(NUMTX_GET_ROW_VEC(result, 2), NUMTX_GET_ROW_VEC(first, 2), NUMTX_GET_ROW_VEC(second, 2), fraction);
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 0), NUMTX_GET_ROW_VEC(result, 0));
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 1), NUMTX_GET_ROW_VEC(result, 1));
    NuVecNorm(NUMTX_GET_ROW_VEC(result, 2), NUMTX_GET_ROW_VEC(result, 2));
}

void QuatInterpolateRotationMatrix(NUMTX *result, NUMTX *first, NUMTX *second, f32 fraction) {
    NUQUAT a, b, interpolated;
    NuMtxToQuat(second, &b);
    NuMtxToQuat(first, &a);
    NuQuatSlerp(&interpolated, &a, &b, fraction);
    NuQuatToMtx(&interpolated, result);
}
