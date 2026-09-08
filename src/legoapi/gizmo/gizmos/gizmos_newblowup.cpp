#include "decomp.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"

i32 (*BlowupExFunc)(GIZMOBLOWUP_s *, u32) = NULL;
bool SphereSphereOverlap(NUVEC *, f32, NUVEC *, f32);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *, i32, i32, i32, GameObject_s *, i32);
void Bolt_AddDeflectedBolt(BOLT_s *, NUVEC *, NUVEC *, u8 *);
void NewRumble(nupad_s *, f32, i32);

GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *object, NUVEC *points, i32 point_count, f32 radius, NUVEC *minimum,
                               NUVEC *maximum, BOLT_s *bolt, u32 hit_type, u8 *flags) {
    const u32 exclude_flag_1 = EXBLOWUPFLAGS & 1;
    const u32 exclude_flag_2 = EXBLOWUPFLAGS & 2;
    const bool airborne_damage =
        object != NULL &&
        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x28 > 0.0f;
    GIZMOBLOWUP_s *nearest = NULL;
    f32 nearest_distance = 1000000.0f;
    if (WORLD->gizmo_blowups == NULL) {
        return NULL;
    }
    for (i32 index = 0; index < WORLD->gizmo_blowup_count; ++index) {
        GIZMOBLOWUP_s *blowup = &WORLD->gizmo_blowups[index];
        const u32 properties = blowup->draw_flags;
        if ((blowup->status_flags & 0x804001) != 0x804000 ||
            (exclude_flag_1 && (properties & 0x80000) && airborne_damage)) {
            continue;
        }
        if (exclude_flag_2 && (properties & 0x200000) && hit_type - 8 < 2 && BlowupExFunc != NULL &&
            BlowupExFunc(blowup, hit_type)) {
            continue;
        }
        if (bolt != NULL && (blowup->platform_id != -1 || (properties & 0x8000) == 0 ||
                             ((properties & 0x80000) && (object == NULL || object->field_0xcc0 == NULL)))) {
            continue;
        }
        if (hit_type == 0 && (properties & 0x10000) == 0) {
            continue;
        }
        if ((hit_type == 3 && (properties & 0x200000)) || ((properties & 0x20) && ShadowMode == 0) ||
            ((properties & 0x40) != 0) != (hit_type == 7)) {
            continue;
        }
        const NUVEC &center = blowup->mid_position;
        const f32 extent = blowup->target_scale;
        if (!(center.x - extent <= maximum->x && minimum->x <= center.x + extent && center.z - extent <= maximum->z &&
              minimum->z <= center.z + extent && center.y - extent <= maximum->y && minimum->y <= center.y + extent)) {
            continue;
        }
        for (i32 point = point_count - 1; point >= 0; --point) {
            if (SphereSphereOverlap(&blowup->mid_position, extent, &points[point], radius)) {
                NUVEC *origin = object != NULL ? &object->apiobj.collision_position : &points[point];
                const f32 distance = NuVecDistSqr(origin, &blowup->mid_position, NULL);
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = blowup;
                }
                break;
            }
        }
    }
    if (nearest == NULL) {
        return NULL;
    }
    i32 damage;
    i32 cause;
    if (hit_type == 7 || hit_type == 2) {
        damage = -1;
        cause = 9;
    } else {
        damage = 1;
        if (bolt != NULL) {
            damage = BoltType_FindByID(bolt->type_id, WORLD)->field_3c;
            if (Cheats_CheckFlags(2) && (bolt->flags & 3)) {
                damage *= 2;
            }
        }
        cause = hit_type == 3 ? 10 : (hit_type == 0 ? 13 : 3);
    }
    if (GizmoBlowupBlowup(nearest, 1, cause, damage, NULL, 1) == 0) {
        if (bolt != NULL) {
            NUVEC normal;
            NuVecSub(&normal, &nearest->position, &bolt->position);
            NuVecNorm(&normal, &normal);
            Bolt_AddDeflectedBolt(bolt, &bolt->field_0xac, &normal, flags);
        }
    } else if (object != NULL) {
        NewRumble(object->pad_gamepad->pad, 0.4f, 0);
        GameCam_HitJudder();
    }
    if (BoltSys->stop_targeting != NULL) {
        BoltSys->stop_targeting(object, points);
    }
    return nearest;
}

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
extern i32 ShadowMode;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
i32 (*GizmoBlowUp_NoTargetFn)(WORLDINFO_s *, GameObject_s *) = NULL;
DECOMP_ASSERT(offsetof(WORLDINFO_s, blowup_target_candidates) == 0x50d4, "Blowup target candidate-list ABI");

GIZMOBLOWUP_s *GizmoBlowUp_Target(GameObject_s *object, nuvec_s *position, nuvec_s *direction, float radius,
                                  float range_squared, i32 directional, i32 planar, i32 bolt_id) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    BOLTTYPE_s *bolt = BoltType_FindByID(bolt_id, world);
    u16 near_angle = TargetDeg_Near, mid_angle = TargetDeg_Mid, far_angle = TargetDeg_Far;
    f32 near_distance = TargetDist_Near2, mid_distance = TargetDist_Mid2;
    GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
    if (blowup == NULL)
        return NULL;
    if (GizmoBlowUp_NoTargetFn != NULL && GizmoBlowUp_NoTargetFn(world, object) != 0)
        return NULL;
    NUVEC aim = *direction;
    f32 min_x = position->x - radius, min_z = position->z - radius;
    f32 max_x = position->x + radius, max_z = position->z + radius;
    if (directional != 0 && (bolt->field_60 & 0x20000) != 0) {
        aim.y = 0.0f;
        NuVecNorm(&aim, &aim);
    }
    i32 count = static_cast<i32>(world->field_0x50d0);
    if (count <= 0) {
        count = world->gizmo_blowup_count;
        if (world->field_0x50d0 == 0)
            blowup = world->gizmo_blowups;
    }
    GIZMOBLOWUP_s *nearest = NULL, *fallback = NULL;
    f32 nearest_distance = 100000000.0f;
    for (i32 i = 0; i < count; ++i, ++blowup) {
        if (world->field_0x50d0 != 0)
            blowup = world->blowup_target_candidates[i];
        if ((blowup->draw_flags & 0x8000) == 0 || (blowup->output_flags & 1) != 0 ||
            (blowup->status_flags & 0x08800000) == 0 || (blowup->visibility_flags & 0x40) == 0)
            continue;
        if ((blowup->draw_flags & 0x20) != 0 && ShadowMode == 0)
            continue;
        if (blowup->mid_position.x < min_x || blowup->mid_position.x > max_x || blowup->mid_position.z < min_z ||
            blowup->mid_position.z > max_z)
            continue;
        NUVEC delta;
        f32 distance = planar != 0 ? NuVecXZDistSqr(&blowup->mid_position, position, &delta)
                                   : NuVecDistSqr(&blowup->mid_position, position, &delta);
        if (!(range_squared > distance))
            continue;
        if (directional == 0) {
            u16 angle = NuAtan2D(blowup->mid_position.x - position->x, blowup->mid_position.z - position->z);
            NuVecRotateY(&aim, &v001, angle);
        }
        if ((bolt->field_60 & 0x20000) != 0)
            delta.y = 0.0f;
        NuVecNorm(&delta, &delta);
        f32 dot = NuVecDot(&delta, &aim);
        u16 angle = distance < near_distance && directional != 0 ? near_angle
                    : distance < mid_distance                    ? mid_angle
                                                                 : far_angle;
        if (!(dot > NuTrigTable[((i32)angle + 0x4000) >> 1 & 0x7fff]) || !(nearest_distance > distance))
            continue;
        if (object != NULL && object->attack_blowup_target == blowup) {
            fallback = blowup;
        } else {
            nearest_distance = distance;
            nearest = blowup;
        }
    }
    return nearest != NULL ? nearest : fallback;
}

f32 GizmoBlowUpOpponent_Range2;
i32 GizmoBlowUpOpponent_Behind;
extern i16 LEGOACT_PUNCH_BEHIND;

GIZMOBLOWUP_s *GizmoBlowUpOpponent(GameObject_s *object, f32 range, f32 extra_radius, f32 minimum_radius, i32 mode,
                                   u32 mask, u32 value, u32 secondary_mask) {
    if (forceNextAttackOpponent != NULL && (object->apiobj.flags_low & 0x80) != 0) {
        return forceNextAttackOpponent->GetGizBlowup();
    }
    GIZMOBLOWUP_s *nearest = NULL;
    f32 nearest_distance = GizmoBlowUpOpponent_Range2;
    if (WORLD->gizmo_blowups != NULL) {
        nearest_distance = range * range;
        GizmoBlowUpOpponent_Behind = 0;
        for (i32 i = 0; i < WORLD->gizmo_blowup_count; ++i) {
            GIZMOBLOWUP_s *target = &WORLD->gizmo_blowups[i];
            u32 flags = target->draw_flags;
            if ((target->status_flags & 0x80c001) != 0x80c000 ||
                ((object->apiobj.flags_low & 0x80) != 0 && target->platform_id != -1 &&
                 target->platform_id == object->apiobj.supporting_platform_id) ||
                ((flags & 0x20) != 0 && ShadowMode == 0))
                continue;
            if (mask != 0 && (value == 0 ? (flags & mask) == 0 : (flags & mask) != value))
                continue;
            if (secondary_mask != 0 && (target->secondary_flags & secondary_mask) == 0)
                continue;
            if (mode == 4 && (flags & 0x80) == 0)
                continue;
            NUVEC delta;
            f32 distance;
            if (mode == 5) {
                f32 bottom = object->apiobj.collision_min.y;
                f32 top = object->apiobj.collision_max.y;
                if ((top - bottom) * 1.5f + top < target->bounds_min.y || target->bounds_max.y < bottom)
                    continue;
                distance = NuVecXZDistSqr(&object->apiobj.collision_position, &target->mid_position, &delta);
            } else {
                if (target->bounds_min.y > object->apiobj.collision_max.y)
                    continue;
                distance = NuVecDistSqr(&object->apiobj.collision_position, &target->mid_position, &delta);
            }
            if (distance >= nearest_distance)
                continue;
            if (mode == 2) {
                GizmoBlowUpOpponent_Behind = 0;
                nearest = target;
                nearest_distance = distance;
                continue;
            }
            if (extra_radius > 0.0f) {
                f32 radius = object->apiobj.field_0x1dc + target->target_scale;
                f32 maximum = radius + extra_radius;
                if (distance >= maximum * maximum)
                    continue;
                if (minimum_radius > 0.0f) {
                    f32 minimum = radius + minimum_radius;
                    if (distance < minimum * minimum)
                        continue;
                }
            }
            NuVecRotateY(&delta, &delta, -object->apiobj.movement_facing_angle);
            i32 behind = 0;
            if (delta.z >= 0.0f) {
                if (mode != 4 || LEGOACT_PUNCH_BEHIND == -1 ||
                    object->apiobj.character_model->model_data_b[LEGOACT_PUNCH_BEHIND] == NULL)
                    continue;
                behind = 1;
            }
            nearest = target;
            nearest_distance = distance;
            GizmoBlowUpOpponent_Behind = behind;
        }
    }
    GizmoBlowUpOpponent_Range2 = nearest_distance;
    return nearest;
}

GIZMOBLOWUP_s *GizmoBlowUp_FindByName(WORLDINFO_s *world, char *name) {
    GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
    if (blowup != NULL) {
        for (i32 i = 0; i < world->gizmo_blowup_count; ++i, ++blowup) {
            if (NuStrICmp(blowup->name, name) == 0)
                return blowup;
        }
    }
    return NULL;
}

GIZMOBLOWUP_s *GizmoBlowUp_FindFromPlatID(WORLDINFO_s *world, i32 platform_id) {
    GIZMOBLOWUP_s *blowup = world->gizmo_blowups;
    GIZMOBLOWUP_s *fallback = NULL;
    if (blowup != NULL) {
        for (i32 i = 0; i < world->gizmo_blowup_count; ++i, ++blowup) {
            if (blowup->platform_id == platform_id) {
                if ((blowup->status_flags & 0x804001) == 0x804000)
                    return blowup;
                if (fallback == NULL)
                    fallback = blowup;
            }
        }
    }
    return fallback;
}

void GIZMOBLOWUP_s::ClearMechObjectInterface() {
    if (mech_object_interface != NULL)
        delete mech_object_interface;
}

MechObjectInterface *GIZMOBLOWUP_s::GetMechObjectInterface() {
    if (mech_object_interface != NULL)
        return mech_object_interface;
    new GizBlowupObjectInterface(*this);
    return mech_object_interface;
}
