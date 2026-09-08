#include "legoapi/world/world.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/qrand.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/render/fx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/level.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/area.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/gizmos/transport/tubes.h"

void HomeNearestTorpTarget(BOLT_s *, TORPEDOPACKET_s *);
void *FindNearestTorpTarget(WORLDINFO_s *, NUVEC *, f32, u8 *);
void FindAnglesXY(NUVEC *, u16 *, u16 *);
extern AREADATA *BOUNTYHUNTERPURSUIT_ADATA;
extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
void TorpedoHitTarget(BOLT_s *);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *, i32, i32, i32, GameObject_s *, i32);
void GizTurrets_Hit(void *, GIZTURRET_s *, NUVEC *, i32, i32);
i32 GizObstacles_Hit(void *, GIZOBSTACLE_s *, NUVEC *, i32, i32);

void TorpedoCode(GameObject_s *, i32, float) {
}

f32 Torpedo_Scale(BOLT_s *bolt) {
    if (bolt != NULL && bolt->owner != NULL)
        return 2.0f * bolt->owner->apiobj.field_0x1dc;
    return 3.0f;
}

void Torpedo_Shoot(GameObject_s *) {
}

void Tube_MoveCode(GameObject_s *, WORLDINFO_s *) {
}

void Tube_SetObjBit(TUBE *tube, i32 object_index) {
    tube->occupied_object_masks[object_index / 32] |= 1U << object_index;
}

void Torpedo_EndBolt(BOLT_s *bolt) {
    TorpedoHitTarget(bolt);
    GameCam_Judder(GameCam, qrand() < 0x8000 ? 0.4f : -0.4f, 2, NULL);
    NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
}

void Tube_FindByName(WORLDINFO_s *, char *) {
}

i32 Tube_InCylinder(GameObject_s *object, TUBE *tube, f32 *horizontal_distance_squared, i32 ignore_height) {
    if (tube == NULL || object == NULL) {
        return 0;
    }

    if (ignore_height == 0) {
        if (tube->position.y > object->apiobj.collision_max.y || object->apiobj.collision_min.y > tube->top) {
            return 0;
        }
    }

    const f32 delta_x = object->apiobj.collision_position.x - tube->position.x;
    const f32 delta_z = object->apiobj.collision_position.z - tube->position.z;
    const f32 distance_squared = delta_x * delta_x + delta_z * delta_z;

    f32 radius_squared = tube->radius_squared;
    if ((tube->flags & TUBE_FLAG_TOUCH_RADIUS) != 0 && TouchHacks::TouchControlsActive) {
        radius_squared *= 0.8f;
    }

    if (distance_squared > radius_squared) {
        return 0;
    }
    if (horizontal_distance_squared != NULL) {
        *horizontal_distance_squared = distance_squared;
    }
    return 1;
}

void TorpedoHitTarget(BOLT_s *bolt) {
    if (bolt == NULL || bolt->owner == NULL || bolt->owner->torpedo == NULL)
        return;

    TORPEDOPACKET *packet = bolt->owner->torpedo;
    if (packet->target != NULL) {
        NUVEC *position = NULL;
        f32 radius;
        switch (packet->target_type) {
            case 2: {
                GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(packet->target);
                radius = obstacle->field_0x58;
                position = &obstacle->evaluated_position;
                break;
            }
            case 0: {
                GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(packet->target);
                radius = blowup->target_scale;
                position = &blowup->mid_position;
                break;
            }
            case 1: {
                GIZTURRET_s *turret = static_cast<GIZTURRET_s *>(packet->target);
                radius = NuSpecialGetOriginRadius(&turret->primary_anim_obj->special);
                position = NuSpecialGetDrawPos(&turret->primary_anim_obj->special);
                break;
            }
        }
        if (position != NULL) {
            NUVEC delta;
            NuVecSub(&delta, position, &bolt->position);
            if (NuVecMagSqr(&delta) < radius * radius) {
                i32 player = bolt->owner != NULL ? static_cast<i8>(bolt->owner->apiobj.field_0x27c) : -1;
                TORPEDOPACKET *hit_packet = bolt->owner->torpedo;
                u8 target_type = hit_packet->target_type;
                void *target = hit_packet->target;
                if (target != NULL) {
                    switch (target_type) {
                        case 0:
                            GizmoBlowupBlowup(static_cast<GIZMOBLOWUP_s *>(target), 1, 11, 1, NULL, 1);
                            break;
                        case 1:
                            GizTurrets_Hit(WORLD, static_cast<GIZTURRET_s *>(target), &bolt->position, player, -1);
                            break;
                        case 2:
                            GizObstacles_Hit(WORLD, static_cast<GIZOBSTACLE_s *>(target), &bolt->position, player, -1);
                            break;
                    }
                }
            }
        }
    }
    packet->field_0x1 = 1;
}

void Torpedo_InitBolt(BOLT_s *bolt) {
    if (bolt->owner != NULL && bolt->owner->torpedo != NULL) {
        HomeNearestTorpTarget(bolt, bolt->owner->torpedo);
        bolt->flags &= ~4U;
    }
}

void *FindNearestTorpTarget(WORLDINFO_s *world, NUVEC *position, f32 distance_squared, u8 *target_type) {
    if (world->gizmo_blowup_count == 0)
        return NULL;
    void *nearest = NULL;
    u8 type = 0;
    NUVEC delta;
    for (i32 i = 0; i < world->gizmo_blowup_count; ++i) {
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[i];
        if ((blowup->visibility_flags & 0x40) != 0 && (blowup->status_flags & 0x20008000) != 0 &&
            (blowup->status_flags & 0x800001) == 0x800000 && (blowup->draw_flags & 0x1000000) != 0) {
            f32 distance = NuVecDistSqr(&blowup->mid_position, position, &delta);
            if (distance < distance_squared) {
                distance_squared = distance;
                nearest = blowup;
            }
        }
    }
    if (WORLD->area != NULL && WORLD->area == BOUNTYHUNTERPURSUIT_ADATA) {
        if (world->giz_turret_sys != NULL) {
            GIZTURRET_s *turret = world->giz_turret_sys->turrets;
            for (i32 i = 0; i < world->giz_turret_sys->count; ++i, ++turret) {
                if ((turret->flags & 4) != 0 && (turret->flags & 2) != 0 && (turret->flags & 0x30) == 0 &&
                    turret->primary_anim_obj != NULL) {
                    NUVEC *draw_position = NuSpecialGetDrawPos(&turret->primary_anim_obj->special);
                    if (draw_position != NULL) {
                        f32 distance = NuVecDistSqr(draw_position, position, &delta);
                        if (distance < distance_squared) {
                            distance_squared = distance;
                            nearest = turret;
                            type = 1;
                        }
                    }
                }
            }
        }
    }
    if (WORLD->area != NULL && WORLD->area == BOUNTYHUNTERPURSUIT_ADATA) {
        if (world->giz_obstacle_sys != NULL) {
            for (i32 i = 0; i < world->giz_obstacle_sys->active_gizmo_count; ++i) {
                GIZOBSTACLE_s *obstacle =
                    static_cast<GIZOBSTACLE_s *>(world->giz_obstacle_sys->active_gizmos[i]->object);
                if ((obstacle->progress_flags & 2) != 0 && (obstacle->progress_flags & 1) != 0 &&
                    (obstacle->runtime_flags & 0x80) == 0) {
                    f32 distance = NuVecDistSqr(&obstacle->evaluated_position, position, &delta);
                    if (distance < distance_squared) {
                        distance_squared = distance;
                        nearest = obstacle;
                        type = 2;
                    }
                }
            }
        }
    }
    if (nearest != NULL && target_type != NULL)
        *target_type = type;
    return nearest;
}

void HomeNearestTorpTarget(BOLT_s *bolt, TORPEDOPACKET_s *packet) {
    BOLTTYPE_s *type = BoltType_FindByID(15, WORLD);
    if (packet == NULL || bolt == NULL || (packet->field_0x1 & 8) != 0)
        return;
    f32 range = (bolt->lifetime - bolt->time) * bolt->speed;
    void *target = packet->target;
    u8 target_type;
    if (target == NULL) {
        target = FindNearestTorpTarget(WORLD, &bolt->position, range * range, &target_type);
        if (target == NULL) {
            if (bolt->time == 0.0f)
                bolt->velocity.y += 6.0f;
            return;
        }
    } else {
        target_type = packet->target_type;
    }

    NUVEC delta;
    NUVEC *target_position = NULL;
    switch (target_type) {
        case 0:
            target_position = &static_cast<GIZMOBLOWUP_s *>(target)->mid_position;
            break;
        case 1: {
            target_position = NuSpecialGetDrawPos(&static_cast<GIZTURRET_s *>(target)->primary_anim_obj->special);
            break;
        }
        case 2:
            target_position = &static_cast<GIZOBSTACLE_s *>(target)->evaluated_position;
            break;
    }
    if (target_position != NULL)
        NuVecSub(&delta, target_position, &bolt->position);
    f32 height = delta.y;
    delta.y = 0.0f;
    f32 distance = NuVecMag(&delta);
    f32 speed = bolt->speed;
    if (bolt->time == 0.0f) {
        bolt->velocity.y += 6.0f;
    } else if ((packet->field_0x1 & 0x10) == 0) {
        u16 current_x, current_y, target_x, target_y;
        FindAnglesXY(&bolt->velocity, &current_x, &current_y);
        bolt->velocity.x = 0.0f;
        bolt->velocity.y = 0.0f;
        bolt->velocity.z = bolt->speed;
        delta.y = height + 0.1f;
        NuVecNorm(&delta, &delta);
        FindAnglesXY(&delta, &target_x, &target_y);
        f32 seek = (1.0f + NU_SIN_LUT(bolt->time / type->field_14 * 16384.0f + 32768.0f + 16384.0f)) * 15.0f;
        target_x = SeekRot(current_x, target_x, seek);
        target_y = SeekRot(current_y, target_y, seek);
        NUMTX matrix __attribute__((aligned(16)));
        NuMtxSetIdentity(&matrix);
        NUANGVEC angles;
        angles.x = target_x;
        angles.y = target_y;
        NuMtxSetRotationXYVU0(&matrix, &angles);
        NuVecMtxRotate(&bolt->velocity, &bolt->velocity, &matrix);
        NuVecNorm(&bolt->field_0xac, &bolt->velocity);
        f32 lifetime = distance / speed + 0.05f + bolt->time;
        if (lifetime > bolt->lifetime && (packet->field_0x1 & 8) == 0)
            bolt->lifetime = lifetime;
    }
    packet->field_0x1 |= 4;
    packet->target = target;
}

void Torpedo_Ricochet(BOLT_s *bolt, TORPEDOPACKET_s *packet) {
    if (packet == NULL || bolt == NULL)
        return;
    if ((packet->field_0x1 & 8) != 0) {
        if (packet->ricochet_time == 0.0f) {
            NUVEC axis;
            NuVecCross(&axis, &packet->ricochet_position, &bolt->velocity);
            NuVecNorm(&axis, &axis);
            NUMTX matrix __attribute__((aligned(16)));
            NuMtxSetIdentity(&matrix);
            f32 lengths = NuVecMag(&packet->ricochet_position) * NuVecMag(&bolt->velocity);
            f32 dot = NuVecDot(&packet->ricochet_position, &bolt->velocity);
            f32 cosine = lengths == 0.0f || dot == 0.0f ? 0.0f : dot / lengths;
            f32 absolute = NuFabs(cosine);
            f32 root = NuFsqrt(1.0f - cosine * cosine);
            f32 smaller = MIN(root, absolute);
            f32 quadrant = CLAMP((absolute - 0.70710677f) * 3.40282e38f, -1.0f, 1.0f);
            f32 sign = MIN(cosine * 3.40282e38f, 1.0f);
            sign = MAX(sign, -1.0f);
            f32 product = quadrant * sign;
            f32 x = smaller * product;
            f32 square = x * x;
            f32 cube = x * square;
            f32 fourth = square * square;
            f32 angle_radians = (product + sign) * 0.785398f - x;
            angle_radians += (x * -0.166667f) * square;
            angle_radians += (-0.075f * square) * cube;
            angle_radians += (-0.0446429f * cube) * fourth;
            angle_radians += (-0.0303819f * fourth) * (square * cube);
            i16 angle = 0x4000 - static_cast<i32>(angle_radians * 10430.4f);
            NuMtxSetRotationAxis(&matrix, 0x8000 - 2 * angle, &axis);
            NuVecMtxRotate(&bolt->velocity, &bolt->velocity, &matrix);
            NuVecScale(&bolt->velocity, &bolt->velocity, 0.8f);
            bolt->speed *= 0.8f;
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[71].effect, &bolt->position, 60, FRAMETIME, 0,
                                              0, NULL);
            packet->ricochet_time += FRAMETIME;
            packet->field_0x1 |= 0x10;
        } else if (packet->ricochet_time < 0.2f) {
            packet->ricochet_time += FRAMETIME;
        } else {
            packet->field_0x1 &= ~0x18;
            packet->ricochet_position = v000;
        }
    }
    bolt->lifetime = bolt->time + 0.0001f;
}

i32 Tube_IsObjBitSet(TUBE *tube, i32 object_index) {
    return tube->occupied_object_masks[object_index / 32] >> object_index & 1;
}

void Torpedo_UpdateBolt(BOLT_s *) {
}

void Torpedo_InitRicochet(BOLT_s *bolt, nuvec_s *position) {
    if (bolt != NULL && bolt->owner != NULL && bolt->owner->torpedo != NULL) {
        TORPEDOPACKET *packet = bolt->owner->torpedo;
        if (packet->target != NULL) {
            if (packet->field_02 < 5) {
                packet->field_0x1 |= 8;
                packet->ricochet_position = *position;
                packet->ricochet_time = 0.0f;
                ++packet->field_02;
            } else {
                packet->field_0x1 |= 0x10;
            }
        }
    }
}

void Torpedo_UpdateJobbies(GameObject_s *) {
}

TUBE *Tube_InAnyCylinder(WORLDINFO_s *world, GameObject_s *object, i32 ignore_height) {
    TUBE *tube = world->tubes;
    if (tube != NULL) {
        for (i32 index = 0; index < world->tube_count; ++index, ++tube) {
            if ((tube->flags & (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE | TUBE_FLAG_DIRECTIONAL)) ==
                    (TUBE_FLAG_ACTIVE | TUBE_FLAG_VISIBLE) &&
                Tube_InCylinder(object, tube, NULL, ignore_height) != 0) {
                return tube;
            }
        }
    }
    return NULL;
}
