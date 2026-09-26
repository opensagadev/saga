#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/world/world.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"

static i32 FindTeleport_Direction;

f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
void PlayJumpSfx(GameObject_s *object, i32 type);
void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);

static inline void Teleport_GetSegment(GameObject_s *object, TELEPORT_s *teleport, NUVEC **start, NUVEC **end) {
    NUVEC *points = teleport->path->pts;
    const bool forwards = (object->context_variant_flags & 4) != 0;

    if ((teleport->flags & 4) != 0) {
        *start = &points[forwards ? 0 : 3];
        *end = &points[forwards ? 3 : 0];
    } else if (forwards) {
        *start = &points[object->field_0x7a4 == 0 ? 0 : 2];
        *end = &points[object->field_0x7a4 == 0 ? 1 : 3];
    } else {
        *start = &points[object->field_0x7a4 == 0 ? 3 : 1];
        *end = &points[object->field_0x7a4 == 0 ? 2 : 0];
    }
}

static inline f32 Teleport_GetSegmentDuration(GameObject_s *object, TELEPORT_s *teleport, NUVEC *start, NUVEC *end) {
    if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL) {
        const f32 animation_speed = AnimSpeed(object->apiobj.character_model, object->context_animation);
        if (animation_speed != 0.0f)
            return NuVecXZDist(start, end, NULL) / NuFabs(animation_speed);
    }
    return (teleport->flags & 4) != 0 ? 3.0f : 1.0f;
}

static inline void Teleport_SetFacing(GameObject_s *object, NUVEC *start, NUVEC *end, bool all_angles) {
    const u16 facing = NuAtan2D(end->x - start->x, end->z - start->z);
    object->apiobj.movement_facing_angle = facing;
    if (all_angles) {
        object->apiobj.facing_angle = facing;
        object->apiobj.field_0x276 = facing;
    }
}

TELEPORT_s *Teleport_Find(GameObject_s *object, float range_squared, VuVec *position) {
    if (WORLD->teleports == NULL)
        return NULL;
    f32 radius = 2.5f * object->apiobj.collision_radius;
    const f32 radius_squared = radius * radius;
    i32 nearest = -1;
    f32 nearest_distance = 100000000.0f;
    for (i32 i = 0; i < WORLD->teleport_count; ++i) {
        TELEPORT_s *teleport = &WORLD->teleports[i];
        if (teleport->active != 0 || teleport->enabled == 0)
            continue;
        if (NuSpecialExistsFn(&teleport->blocking_special)) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&teleport->blocking_special);
            if (animation != NULL) {
                if (animation->playing || animation->ltime == 1.0f)
                    continue;
            } else if (NuSpecialGetVisibilityFn(&teleport->blocking_special)) {
                continue;
            }
        }
        const f32 fallback_range = (teleport->flags & 2) != 0 ? teleport->range_squared : radius_squared;
        NUVEC *point = teleport->path->pts;
        f32 distance = NuVecDistSqr(&object->apiobj.collision_position, point, NULL);
        if (((range_squared != 0.0f && distance < range_squared) || distance < fallback_range) &&
            (nearest == -1 || distance < nearest_distance)) {
            if (position != NULL && point != NULL) {
                position->x = point->x;
                position->y = point->y;
                position->z = point->z;
            }
            FindTeleport_Direction = 0;
            nearest = i;
            nearest_distance = distance;
        }
        if ((teleport->flags & 1) != 0)
            continue;
        point = &teleport->path->pts[teleport->path->length - 1];
        distance = NuVecDistSqr(&object->apiobj.collision_position, point, NULL);
        if (((range_squared != 0.0f && distance < range_squared) || distance < fallback_range) &&
            (nearest == -1 || distance < nearest_distance)) {
            if (position != NULL && point != NULL) {
                position->x = point->x;
                position->y = point->y;
                position->z = point->z;
            }
            FindTeleport_Direction = 1;
            nearest = i;
            nearest_distance = distance;
        }
    }
    return nearest == -1 ? NULL : &WORLD->teleports[nearest];
}

void Teleports_Reset(WORLDINFO_s *world) {
    if (world->teleports == NULL || WORLD->teleport_count <= 0) {
        return;
    }

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->active = 0;
        teleport->enabled = 1;
        teleport->field_78 = 0;
        teleport->field_74 = 0;
        teleport->field_7a = 0;
        teleport->field_76 = 0;
    }
}

void Teleport_MoveCode(GameObject_s *object, i32 start_immediately) {
    if ((object->apiobj.character_data->model_flags & 0x40000) == 0 && SuperWeirdo(object) == 0)
        return;

    if (object->field_0x7a5 != 15) {
        if (object->apiobj.field_0x27d == 0 || object->apiobj.field_0x287 != 0)
            return;

        switch (object->field_0x7a5) {
            case 1:
            case 0xff:
            case 2:
            case 3:
            case 4:
                break;
            default:
                if (objInNetWaitContext(object, 15) == 0)
                    return;
                break;
        }

        if (!object->apiobj.player_controlled && object->use_action != 3)
            return;
        if (object->touch_task != NULL &&
            object->touch_task->GetHashId().value != MechTouchTaskUseTeleport::HashId.value)
            return;

        TELEPORT_s *teleport = Teleport_Find(object, 0.0f, NULL);
        if (teleport == NULL)
            return;

        if (objInNetWaitContext(object, 15) != 0) {
            object->context_animation_timer -= FRAMETIME;
            if (object->context_animation_timer <= 0.0f) {
                object->field_0x7a5 = 0xff;
                object->big_jump_data = NULL;
            }
        }

        if (start_immediately == 0 && (objInNetWaitContext(object, 15) == 0 || object->big_jump_data == NULL)) {
            if (object->apiobj.player_controlled)
                object->field_0xe24 |= 0x20;
            return;
        }

        object->field_0x788 = teleport;
        object->field_0x7a5 = 15;
        teleport->active = 1;
        object->context_variant_flags = (object->context_variant_flags & ~4) | (FindTeleport_Direction == 0 ? 4 : 0);
        PlayJumpSfx(object, 0);
        if (object->apiobj.player_controlled) {
            Hint_SetComplete(0x268);
            Hint_SetComplete(0x622);
        }

        if (object->id == id_YODA || object->id == id_YODAGHOST)
            object->context_animation = 4;
        else
            object->context_animation = 30;
        object->field_0x7a3 = 0;
        object->field_0x7a4 = 0;

        NUVEC *segment_start;
        NUVEC *segment_end;
        Teleport_GetSegment(object, teleport, &segment_start, &segment_end);
        object->airborne_action_duration = Teleport_GetSegmentDuration(object, teleport, segment_start, segment_end);
        object->context_animation_timer = object->airborne_action_duration;
        Teleport_SetFacing(object, segment_start, segment_end, false);
        object->context_x_rotation = 0;
        object->apiobj.velocity = v000;
        return;
    }

    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(object->field_0x788);

    if (object->field_0x7a3 == 0) {
        const bool forwards = (object->context_variant_flags & 4) != 0;
        if (NuSpecialExistsFn(&teleport->flap1_special) &&
            NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap1_special), NULL) <
                0.36f) {
            teleport->field_78 = forwards ? 0x4000 : -0x4000;
            if ((object->context_x_rotation & 1) == 0) {
                object->context_x_rotation |= 1;
                PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
            }
        }
        if (NuSpecialExistsFn(&teleport->flap2_special) &&
            NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap2_special), NULL) <
                0.36f) {
            teleport->field_7a = forwards ? -0x4000 : 0x4000;
            if ((object->context_x_rotation & 2) == 0) {
                object->context_x_rotation |= 2;
                PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
            }
        }
    }

    if (object->field_0x7a3 != 0) {
        if (object->field_0x7a3 != 1)
            return;
        object->context_animation_timer -= FRAMETIME;
        f32 progress = 1.0f - object->context_animation_timer * 0.5f;
        f32 shadow_height = 2000000.0f;
        bool has_surface = false;
        if (object->context_animation_timer <= 0.0f) {
            object->field_0x7a3 = 0;
            NUVEC *segment_start;
            NUVEC *segment_end;
            Teleport_GetSegment(object, teleport, &segment_start, &segment_end);
            object->airborne_action_duration =
                Teleport_GetSegmentDuration(object, teleport, segment_start, segment_end);
            object->context_animation_timer = object->airborne_action_duration;
            Teleport_SetFacing(object, segment_start, segment_end, true);

            shadow_height = GameShadow(NULL, &object->apiobj.position, 5.0f, -1);
            object->apiobj.field_0x218 = shadow_height;
            has_surface = shadow_height != 2000000.0f;
            GetSurfaceInfo(object, has_surface, shadow_height);
            progress = 1.0f;
        }
        NUVEC *points = teleport->path->pts;
        NUVEC *start = &points[(object->context_variant_flags & 4) != 0 ? 1 : 2];
        NUVEC *end = &points[(object->context_variant_flags & 4) != 0 ? 2 : 1];
        object->apiobj.position.x = start->x + (end->x - start->x) * progress;
        object->apiobj.position.y = start->y + (end->y - start->y) * progress;
        object->apiobj.position.z = start->z + (end->z - start->z) * progress;
        object->apiobj.velocity = v000;
        if (has_surface)
            object->apiobj.position.y = shadow_height - object->character_bottom * object->apiobj.field_0xa8;
        return;
    }

    bool completed = false;
    if (object->apiobj.character_model->model_data_b[object->context_animation] == NULL ||
        CurrentAnim(&object->apiobj.anim_packet) == object->context_animation) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f) {
            object->field_0x7a5 = 0xff;
            object->context_animation_timer = 0.0f;
            teleport->active = 0;
            if ((teleport->flags & 0x10) != 0)
                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            AlertSurroundingCreatures(object, &object->apiobj.collision_position);
            completed = true;
        }
    }

    const f32 progress = 1.0f - object->context_animation_timer / object->airborne_action_duration;
    NUVEC *segment_start;
    NUVEC *segment_end;
    Teleport_GetSegment(object, teleport, &segment_start, &segment_end);
    NUVEC target;
    target.x = segment_start->x + (segment_end->x - segment_start->x) * progress;
    target.y = segment_start->y + (segment_end->y - segment_start->y) * progress;
    target.z = segment_start->z + (segment_end->z - segment_start->z) * progress;
    SeekVec(&object->apiobj.position, &object->apiobj.position, &target, 10.0f);
    object->apiobj.velocity = v000;

    NUVEC shadow_position = object->apiobj.position;
    shadow_position.y += object->character_bottom * object->apiobj.field_0xa8;
    const f32 shadow_height = GameShadow(NULL, &shadow_position, 5.0f, -1);
    object->apiobj.field_0x218 = shadow_height;
    if (shadow_height != 2000000.0f) {
        object->apiobj.position.y = shadow_height - object->character_bottom * object->apiobj.field_0xa8;
        GetSurfaceInfo(object, 1, shadow_height);
    } else {
        GetSurfaceInfo(object, 0, shadow_height);
    }
    object->apiobj.position.y += progress * 0.0025f;

    if (!completed)
        return;
    if ((teleport->flags & 4) != 0 || object->field_0x7a4 != 0) {
        object->apiobj.velocity.y = -0.1f;
        object->apiobj.field_0x27d = 0;
        return;
    }

    object->field_0x7a4 = 1;
    object->field_0x7a5 = 15;
    object->field_0x7a3 = 1;
    object->context_animation_timer = 2.0f;
    NUVEC *points = teleport->path->pts;
    object->apiobj.start_position = points[(object->context_variant_flags & 4) != 0 ? 1 : 2];
    object->apiobj.position = object->apiobj.start_position;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
}

void Teleport_NetMoveCode(GameObject_s *object) {
    if (object->field_0x7a5 != 15 || object->field_0x7a3 != 0)
        return;

    TELEPORT_s *teleport = static_cast<TELEPORT_s *>(object->field_0x788);
    const bool forwards = (object->context_variant_flags & 4) != 0;
    const i16 flap1_rotation = forwards ? 0x4000 : -0x4000;
    const i16 flap2_rotation = forwards ? -0x4000 : 0x4000;

    if (NuSpecialExistsFn(&teleport->flap1_special) &&
        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap1_special), NULL) < 0.36f) {
        teleport->field_78 = flap1_rotation;
        if ((object->context_x_rotation & 1) == 0) {
            object->context_x_rotation |= 1;
            PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
        }
    }

    if (NuSpecialExistsFn(&teleport->flap2_special) &&
        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&teleport->flap2_special), NULL) < 0.36f) {
        teleport->field_7a = flap2_rotation;
        if ((object->context_x_rotation & 2) == 0) {
            object->context_x_rotation |= 2;
            PlaySfx(const_cast<char *>("env_door_flap"), &object->apiobj.collision_position);
        }
    }
}

i32 Teleport_UpdateHints(HINT_s *hint) {
    if (WORLD->teleport_count <= 0 || player->field_0x7a5 != 0xff)
        return 0;
    for (i32 i = 0; i < 2; ++i) {
        GameObject_s *object = Player[i];
        if (object == NULL || ((object->field_0xe24 & 0x20) == 0 && Teleport_Find(object, 1.5625f, NULL) == NULL))
            continue;
        if (hint->control_mode_ids[0] == 0x268)
            return AvailableToPlayer(0x40000, -1, 0, 1) != 0;
        if (hint->control_mode_ids[0] == 0x622 && FreePlay != 0 && AvailableToPlayer(0x40000, -1, 0, 1) == 0)
            return 1;
        return 0;
    }
    return 0;
}

void Teleports_UpdateAfterGameObjects(WORLDINFO_s *world) {
    if (world->teleports == NULL || WORLD->teleport_count <= 0)
        return;

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->field_74 = SeekRot(teleport->field_74, teleport->field_78, 5.0f);
        if (NuSpecialExistsFn(&teleport->flap1_special)) {
            NUMTX matrix = teleport->flap1_matrix;
            if ((teleport->flags & 8) != 0)
                NuMtxPreRotateX(&matrix, -teleport->field_74);
            else
                NuMtxPreRotateX(&matrix, teleport->field_74);
            NuSpecialSetDrawMtx(&teleport->flap1_special, &matrix);
            NuSpecialUpdate(&teleport->flap1_special);
        }

        teleport->field_76 = SeekRot(teleport->field_76, teleport->field_7a, 5.0f);
        if (NuSpecialExistsFn(&teleport->flap2_special)) {
            NUMTX matrix = teleport->flap2_matrix;
            if ((teleport->flags & 8) != 0)
                NuMtxPreRotateX(&matrix, -teleport->field_76);
            else
                NuMtxPreRotateX(&matrix, teleport->field_76);
            NuSpecialSetDrawMtx(&teleport->flap2_special, &matrix);
            NuSpecialUpdate(&teleport->flap2_special);
        }
    }
}

void Teleports_UpdateBeforeGameObjects(WORLDINFO_s *world) {
    if (world->teleports == NULL || WORLD->teleport_count <= 0)
        return;

    TELEPORT_s *teleport = world->teleports;
    for (i32 i = 0; i < WORLD->teleport_count; ++i, ++teleport) {
        teleport->field_78 = 0;
        teleport->field_7a = 0;
    }
}

#include "legoapi/gizmo/base/TeleportObjectInterface.h"

void TELEPORT_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

MechObjectInterface *TELEPORT_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new TeleportObjectInterface(*this, -1);
    }
    return mech_object_interface;
}
