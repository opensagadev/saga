#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"

#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/audio/audio.h"
#include <stdio.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 ForcePushed_SuperPush_Occurring(GameObject_s *first, GameObject_s *second);
void StartFlatten(GameObject_s *source, GameObject_s *target);

i32 objhitobj_nohurtsfx;
i32 objhitobj_noattackerrumble;
i32 objhitobj_throwkillpartsup;
extern i32 objhitobj_noimpactsfx;
extern i16 *objhitobj_killparts_yrot;
extern BOLT_s *objhitobj_bolt;
extern i32 ObstacleCamHoldUntilPlayersMove;
extern i32 disable_narrow_socks, players_cannot_exit_speeder, BuildUpDone;
extern f32 BuildUpScale, DrawBuildUpTime, builduptime;
extern i32 MiniCutCam;
f32 SHIELDFLICKERTIME = 0.1f;
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
i32 Hub_InMenu();
i32 RotDiff(u16, u16);
void NewRumble(nupad_s *, f32, i32);
i32 NewBlockAction(GameObject_s *);
void PlayerTakeHit(GameObject_s *, GameObject_s *);
void ReleaseForce(GameObject_s *, i32);
void LoseHelmet(GameObject_s *, i32, i32);
void TakeHitRumble(GameObject_s *, f32);
void KillRumble(GameObject_s *);
void PlayHurtSfx(GameObject_s *);
void SnakeBeenHit(GameObject_s *);
void PopBalloon(GameObject_s *);
i32 Player_HasFastBuild(GameObject_s *);
i32 Player_HasInvincibility(GameObject_s *);
i32 ReleaseHearts();
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void Arcade_Kill(i32, i32);
i32 qrand();
GAMEPAD_s *ViewCamGetGamePad();
void Player_ClearContext(GameObject_s *, i32);
extern "C" i32 AddGameDebris(APIDEBRISSYS_s *, i32, NUVEC *);
extern "C" void NuSpecialSetVisibility(void *, i32);
static const u8 objhit_damage_joints[3] = {6, 8, 7};

extern i16 id_ANAKINJEDI;
extern i16 id_ATAT;
extern i16 id_BODYGUARD;
extern i16 id_DRAGBOMB;
extern i16 id_GAMORREANGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_OBIWANKENOBIEP3;
extern i16 id_ROYALGUARD;
extern i16 id_SNAKE;
extern i16 id_SPEEDERBIKE;
extern i16 id_WOOKIEE;
extern AREADATA_s *HOTHBATTLE_ADATA;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *SPEEDERCHASE_ADATA;
extern LEVELDATA_s *CRUISERC_LDATA;
extern LEVELDATA_s *HUB_LDATA;
extern LEVELDATA_s *SPEEDERCHASEA_LDATA;
extern LEVELDATA_s *VADERC_LDATA;

i32 CannotKill(GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    return (CInfo[object->character_context].flags & 0x2000000) != 0 ||
           ((data->field_0x94 & 0x800) != 0 && object->apiobj.field_0x27c == -1);
}

static i32 Collide2Objects(APIOBJECT *first, APIOBJECT *second) {
    const f32 first_vertical_velocity = first->velocity.y;
    const f32 second_vertical_velocity = second->velocity.y;
    GameObject_s *first_object = first->objptr;
    GameObject_s *second_object = second->objptr;
    i32 collision_result = ForcePushed_SuperPush_Occurring(first_object, second_object);
    if (collision_result == 0) {
        if (WORLD->current_level == DOGFIGHTA_LDATA) {
            collision_result = APIObjectCollision(first, second);
        } else {
            collision_result = APIObjectCollision2D(first, second);
        }

        if (collision_result == 2 && VehicleArea == 0) {
            GAMECHARACTERDATA *first_data =
                static_cast<GAMECHARACTERDATA *>(first_object->apiobj.character_data->field11_0x24);
            GAMECHARACTERDATA *second_data =
                static_cast<GAMECHARACTERDATA *>(second_object->apiobj.character_data->field11_0x24);
            if ((first_data->flags_090 & 0x1000) != 0 &&
                first->horizontal_velocity_magnitude > first_data->movement_speed * 0.5f) {
                StartFlatten(first_object, second_object);
            } else if ((second_data->flags_090 & 0x1000) != 0 &&
                       second->horizontal_velocity_magnitude > second_data->movement_speed * 0.5f) {
                StartFlatten(second_object, first_object);
            } else if (second_vertical_velocity < 0.0f && (first_data->flags_090 & 0x40) != 0 &&
                       first_object->field_0x107c == -1) {
                first->velocity.y += second_vertical_velocity * 0.5f;
            } else if (first_vertical_velocity < 0.0f && (second_data->flags_090 & 0x40) != 0 &&
                       second_object->field_0x107c == -1) {
                second->velocity.y += first_vertical_velocity * 0.5f;
            }
        }

        f32 maximum_velocity = first->character_data->game_character->movement_speed * 1.25f;
        if (maximum_velocity < 1.0f) {
            maximum_velocity = 1.0f;
        }
        f32 velocity_squared = first->velocity.x * first->velocity.x + first->velocity.z * first->velocity.z;
        if (velocity_squared > maximum_velocity * maximum_velocity) {
            const f32 scale = maximum_velocity / NuFsqrt(velocity_squared);
            first->velocity.x *= scale;
            first->velocity.z *= scale;
        }

        maximum_velocity = second->character_data->game_character->movement_speed * 1.25f;
        if (maximum_velocity < 1.0f) {
            maximum_velocity = 1.0f;
        }
        velocity_squared = second->velocity.x * second->velocity.x + second->velocity.z * second->velocity.z;
        if (velocity_squared > maximum_velocity * maximum_velocity) {
            const f32 scale = maximum_velocity / NuFsqrt(velocity_squared);
            second->velocity.x *= scale;
            second->velocity.z *= scale;
        }
    } else {
        collision_result = (second_object->apiobj.field_0x1e8 & first_object->apiobj.field_0x1f0) != 0 ||
                                   (second_object->apiobj.field_0x1e4 & first_object->apiobj.field_0x1ec) != 0 ||
                                   (first_object->apiobj.field_0x1e8 & second_object->apiobj.field_0x1f0) != 0 ||
                                   (first_object->apiobj.field_0x1e4 & second_object->apiobj.field_0x1ec) != 0
                               ? 1
                               : 2;
        first->field_0x1ec |= second->field_0x1e4;
        first->field_0x1f0 |= second->field_0x1e8;
        second->field_0x1ec |= first->field_0x1e4;
        second->field_0x1f0 |= first->field_0x1e8;
    }
    return collision_result;
}

void KillRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.7f, 0);
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        }
    }
}

void FloatRumble(GameObject_s *object) {
    if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0) {
        const f32 strength = MIN(1.0f, NuFabs(object->apiobj.velocity.y) /
                                               object->apiobj.character_data->game_character->movement_speed * 0.15f +
                                           0.25f);
        NewRumble(object->pad_gamepad->pad, strength, 0);
        NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
    }
}

i16 InsideLineF(f32 point_u, f32 point_v, f32 line_start_u, f32 line_start_v, f32 line_end_u, f32 line_end_v) {
    const f32 side =
        (point_u - line_start_u) * (line_end_v - line_start_v) + (point_v - line_start_v) * (line_start_u - line_end_u);
    if (side < 0.0f) {
        return 0;
    }
    return 1;
}

i16 InsideLineXZ(f32 point_u, f32 point_v, f32 line_start_u, f32 line_start_v, f32 line_end_u, f32 line_end_v) {
    const f32 side =
        (point_u - line_start_u) * (line_end_v - line_start_v) + (point_v - line_start_v) * (line_start_u - line_end_u);
    return side >= 0.0f;
}

void ObjHitShield(GameObject_s *attacker, GameObject_s *target, i32 damage, BOLT_s *) {
    if (target == NULL) {
        return;
    }
    if (attacker != NULL && (attacker->apiobj.flags_low & 1) == 0) {
        attacker = NULL;
    }
    if (target->field_0xd24 == 1.0f && target->timer_d28 > 0.0f) {
        if (attacker != NULL) {
            if (static_cast<i8>(attacker->apiobj.flags_low) < 0) {
                AlertSurroundingCreatures(attacker, &attacker->apiobj.collision_position);
            } else if (attacker->character_context != 0x39) {
                damage = 0;
            }
        }
    } else if (attacker != NULL && static_cast<i8>(attacker->apiobj.flags_low) < 0) {
        AlertSurroundingCreatures(attacker, &attacker->apiobj.collision_position);
    }

    GameAudio_PlaySfx(0x41, &target->apiobj.collision_position, GameAudio_GetPlrSfxBits(attacker), 0);
    if (static_cast<i8>(target->apiobj.flags_low) < 0) {
        TakeHitRumble(target, 0.6f);
    }

    if (MiniCutCam == 0 || static_cast<i8>(target->apiobj.flags_low) >= 0) {
        if (target->field_0xe37 != 0 && damage > 0) {
            if (static_cast<i8>(target->apiobj.flags_low) < 0 && Player_HasInvincibility(target)) {
                damage = 0;
            }
            i32 shield = target->field_0xe37 - damage;
            f32 first_rumble;
            f32 second_rumble;
            if (shield > 0) {
                PlaySfx("DDEkaHit", &target->apiobj.collision_position);
                target->field_0xe37 = shield;
                target->timer_d28 = SHIELDFLICKERTIME;
                first_rumble = 0.5f;
                second_rumble = 0.0f;
            } else {
                PlaySfx("DDEkaFlicker", &target->apiobj.collision_position);
                target->timer_d28 = 0.5f;
                target->field_0xe37 = 0;
                target->field_0xd24 = 0.0f;
                first_rumble = 0.8f;
                second_rumble = 0.3f;
            }
            if (attacker == NULL) {
                return;
            }
            NewRumble(attacker->pad_gamepad->pad, first_rumble, 0);
            if (second_rumble > 0.0f) {
                NewRumble(attacker->pad_gamepad->pad, second_rumble, 0);
            }
            return;
        }
    }
    if (attacker != NULL) {
        NewBuzz(attacker->pad_gamepad->pad, 0.1f, 0);
    }
}

i16 InsidePolLines(f32 point_x, f32 point_y, f32 point_z, f32 edge_a_x, f32 edge_a_y, f32 edge_a_z, f32 edge_b_x,
                   f32 edge_b_y, f32 edge_b_z, NUVEC *normal) {
    // Project onto the plane perpendicular to the dominant normal component.
    // The call order preserves the polygon winding in that projection.
    if (NuFabs(normal->y) >= NuFabs(normal->x) && NuFabs(normal->y) >= NuFabs(normal->z)) {
        if (0.0f > normal->y) {
            if (InsideLineF(point_x, point_z, 0.0f, 0.0f, edge_a_x, edge_a_z) == 0 ||
                InsideLineF(point_x, point_z, edge_b_x, edge_b_z, 0.0f, 0.0f) == 0) {
                return 0;
            }
            return InsideLineF(point_x, point_z, edge_a_x, edge_a_z, edge_b_x, edge_b_z) != 0;
        }

        if (InsideLineF(point_x, point_z, edge_a_x, edge_a_z, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_x, point_z, 0.0f, 0.0f, edge_b_x, edge_b_z) == 0) {
            return 0;
        }
        return InsideLineF(point_x, point_z, edge_b_x, edge_b_z, edge_a_x, edge_a_z) != 0;
    }

    if (NuFabs(normal->y) > NuFabs(normal->x) || NuFabs(normal->z) > NuFabs(normal->x)) {
        if (0.0f > normal->z) {
            if (InsideLineF(point_y, point_x, 0.0f, 0.0f, edge_a_y, edge_a_x) == 0 ||
                InsideLineF(point_y, point_x, edge_b_y, edge_b_x, 0.0f, 0.0f) == 0) {
                return 0;
            }
            return InsideLineF(point_y, point_x, edge_a_y, edge_a_x, edge_b_y, edge_b_x) != 0;
        }

        if (InsideLineF(point_y, point_x, edge_a_y, edge_a_x, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_y, point_x, 0.0f, 0.0f, edge_b_y, edge_b_x) == 0) {
            return 0;
        }
        return InsideLineF(point_y, point_x, edge_b_y, edge_b_x, edge_a_y, edge_a_x) != 0;
    }

    if (0.0f > normal->x) {
        if (InsideLineF(point_y, point_z, edge_a_y, edge_a_z, 0.0f, 0.0f) == 0 ||
            InsideLineF(point_y, point_z, 0.0f, 0.0f, edge_b_y, edge_b_z) == 0) {
            return 0;
        }
        return InsideLineF(point_y, point_z, edge_b_y, edge_b_z, edge_a_y, edge_a_z) != 0;
    }

    if (InsideLineF(point_y, point_z, 0.0f, 0.0f, edge_a_y, edge_a_z) == 0 ||
        InsideLineF(point_y, point_z, edge_b_y, edge_b_z, 0.0f, 0.0f) == 0) {
        return 0;
    }
    return InsideLineF(point_y, point_z, edge_a_y, edge_a_z, edge_b_y, edge_b_z) != 0;
}

i32 ObjHitObj_Flags(GameObject_s *object) {
    if (object == NULL)
        return 0;
    const bool player = (object->apiobj.flags_low & 0x80) != 0;
    const u16 ordinary = player ? 0x80c : 0x00a;
    const u16 special = player ? 0x824 : 0x022;
    const u16 scripted = player ? 0x804 : 0x002;
    if ((object->apiobj.field_0x1f4 & 0x10001) != 0)
        return scripted | 0x10;
    return (object->apiobj.field_0x1f4 & 4) != 0 ? special : ordinary;
}

void CollideGameObjects(WORLDINFO_s *world) {
    if (world->level_sub_id != -1 && (world->area->flags & 0x80) != 0) {
        return;
    }

    u32 narrow_speeder_mask = 0;
    u32 flattened_mask = 0;
    u32 player_collision_mask = 0;
    u32 no_vertical_movement_mask = 0;
    u32 vertical_movement_mask = 0;
    u32 player_slot_mask = 0;

    i32 object_count = HIGHGAMEOBJECT;
    GameObject_s *objects = Obj;
    GameObject_s *object = objects;
    for (i32 index = 0; index < object_count; ++index, ++object) {
        const i32 flags = object->apiobj.flags_low;
        if ((flags & APIOBJECT_FLAG_IN_USE) == 0) {
            continue;
        }

        const u32 bit = static_cast<u32>(static_cast<u64>(1) << object->apiobj.field_0x289);
        if (object->apiobj.field_0x27c != -1) {
            player_slot_mask |= bit;
        }
        GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
        if (!(data->field_0x28 > 0.0f)) {
            no_vertical_movement_mask |= bit;
        } else {
            vertical_movement_mask |= bit;
        }
        if (((flags & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 || (object->field_0xf02 & 8) != 0) &&
            (data->flags_090 & 0x40) == 0) {
            player_collision_mask |= bit;
        }
        if (object->character_context == 0x3d && (flags & APIOBJECT_FLAG_PLAYER_ACTIVE) == 0) {
            flattened_mask |= bit;
        }
        if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 && object->id == id_SPEEDERBIKE &&
            object->apiobj.field_0x27c != -1) {
            narrow_speeder_mask |= bit;
        }
    }

    APIOBJECT *collision_objects[64];
    NUVEC collision_minimums[64];
    NUVEC collision_maximums[64];
    i32 collision_count = 0;
    object = objects;
    for (i32 index = 0; index < object_count; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            object->apiobj.model_draw_result == 0 || object->use_model_origin <= 1) {
            continue;
        }

        const i8 context = object->character_context;
        if ((CInfo[context].flags & 0x8000) != 0 || context == 0x3b || context == 0x3c || context == 0x0f ||
            context == 0x39) {
            continue;
        }
        if (context == 0x26 && object->field_0x7a7 != -1 && (object->context_variant_flags & 2) != 0) {
            continue;
        }
        if (context == 0x1f && object->field_0x7a3 == 1) {
            continue;
        }

        GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
        if ((data->flags_090 & 0x8000) != 0 || (object->action_flags & 0x20) != 0 ||
            (context == 0x47 && object->field_0x788 != NULL &&
             (*reinterpret_cast<u8 *>(static_cast<u8 *>(object->field_0x788) + 0x68) & 1) != 0)) {
            continue;
        }

        collision_objects[collision_count] = &object->apiobj;
        collision_minimums[collision_count] = object->apiobj.collision_min;
        collision_maximums[collision_count] = object->apiobj.collision_max;
        ++collision_count;

        object->apiobj.collision_mask_low = 0;
        object->apiobj.collision_mask_high = 0;
        if ((data->flags_090 & 0x1000) != 0) {
            object->apiobj.collision_mask_low = flattened_mask;
        }

        if (object->field_0x107c != -1) {
            object->apiobj.collision_mask_low |= player_collision_mask;
            for (i32 player_index = 0; player_index < 8; ++player_index) {
                GameObject_s *player = Player[player_index];
                if (player == NULL || (player->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player == object ||
                    !(player->field_0xdc4 > 0.0f) || player->apiobj.supporting_platform_id != object->field_0x107c ||
                    (object->field_0x107c == player->field_0x1078 &&
                     (player->apiobj.field_0x27d != 0 || GameObjectNearFloor(player, 3.0f, NULL) != 0))) {
                    continue;
                }
                NUVEC direction = {object->apiobj.collision_position.x - player->apiobj.collision_position.x, 0.0f,
                                   object->apiobj.collision_position.z - player->apiobj.collision_position.z};
                NuVecNorm(&direction, &direction);
                object->apiobj.movement_direction.x = direction.x;
                object->apiobj.movement_direction.z = direction.z;
            }
        }

        if (object->character_context == CHARACTER_CONTEXT_JUMP && object->airborne_collision_target != NULL) {
            const u64 bit = static_cast<u64>(1) << object->airborne_collision_target->apiobj.field_0x289;
            object->apiobj.collision_exclusion_mask |= bit;
        }
        if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 && object->id == id_SPEEDERBIKE &&
            object->apiobj.field_0x27c != -1) {
            object->apiobj.collision_mask_low |= narrow_speeder_mask;
        }

        if (VehicleArea != 0 && WORLD->current_level != BOUNTYHUNTERPURSUITA_LDATA &&
            WORLD->current_level != DOGFIGHTA_LDATA) {
            data = object->apiobj.character_data->game_character;
            if (object->apiobj.field_0x27c == -1 && data->field_0x28 != 0.0f) {
                object->apiobj.collision_mask_low |=
                    (vertical_movement_mask & player_slot_mask) | no_vertical_movement_mask;
            } else if (data->field_0x28 == 0.0f) {
                object->apiobj.collision_mask_low |= (~player_slot_mask) & vertical_movement_mask;
            } else {
                object->apiobj.collision_mask_low |= vertical_movement_mask;
            }
        }
    }

    APIObjectCollisions(collision_count, collision_objects, collision_minimums, collision_maximums, Collide2Objects);
}

bool CalculateRayBoxIntersection(VuVec const &minimum, VuVec const &maximum, VuVec const &start, VuVec const &direction,
                                 float maximum_distance, float &distance) {
    f32 near_distance;
    f32 far_distance;
    if (direction.x >= 0.0f) {
        near_distance = (minimum.x - start.x) / direction.x;
        far_distance = (maximum.x - start.x) / direction.x;
    } else {
        near_distance = (maximum.x - start.x) / direction.x;
        far_distance = (minimum.x - start.x) / direction.x;
    }

    f32 axis_near_distance;
    f32 axis_far_distance;
    if (direction.y >= 0.0f) {
        axis_near_distance = (minimum.y - start.y) / direction.y;
        axis_far_distance = (maximum.y - start.y) / direction.y;
    } else {
        axis_near_distance = (maximum.y - start.y) / direction.y;
        axis_far_distance = (minimum.y - start.y) / direction.y;
    }

    if (near_distance > axis_far_distance || axis_near_distance > far_distance) {
        return false;
    }
    near_distance = MAX(axis_near_distance, near_distance);
    far_distance = MIN(axis_far_distance, far_distance);

    if (direction.z >= 0.0f) {
        axis_near_distance = (minimum.z - start.z) / direction.z;
        axis_far_distance = (maximum.z - start.z) / direction.z;
    } else {
        axis_near_distance = (maximum.z - start.z) / direction.z;
        axis_far_distance = (minimum.z - start.z) / direction.z;
    }

    if (near_distance > axis_far_distance || axis_near_distance > far_distance) {
        return false;
    }
    far_distance = MIN(axis_far_distance, far_distance);
    near_distance = MAX(axis_near_distance, near_distance);
    distance = near_distance;
    return maximum_distance > near_distance && far_distance > 0.0f;
}

f32 CalcCapsuleIntersectDistance(VuVec const &start, VuVec const &direction, f32 maximum_distance, VuVec const &centre,
                                 f32 radius) {
    const f32 delta_x = centre.x - start.x;
    const f32 delta_y = centre.y - start.y;
    const f32 delta_z = centre.z - start.z;
    f32 distance = direction.x * delta_x + direction.y * delta_y + direction.z * delta_z;

    if (distance >= 0.0f && distance < maximum_distance) {
        const f32 distance_from_axis_squared =
            delta_x * delta_x + delta_y * delta_y + delta_z * delta_z - distance * distance;
        if (radius * radius >= distance_from_axis_squared) {
            return distance;
        }
    }
    return 1.0e9f;
}

i32 CheckCol(nutex_s *, i32, i32, i32, i32) {
    return true;
}

void HitRumble(GameObject_s *object) {
    if (object != NULL) {
        if (object->apiobj.player_controlled) {
            NewRumble(object->pad_gamepad->pad, 0.5f, 0);
            NewBuzzFrames(object->pad_gamepad->pad, 2, 0);
        }
    }
}

i32 ObjHitObj(GameObject_s *attacker, GameObject_s *target, i32 damage, u16 flags, i32 probe, i32) {
    if (target == NULL || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
        target->field_0x101c > 0.0f)
        return 0;
    i32 no_hurt = objhitobj_nohurtsfx;
    i32 no_impact = objhitobj_noimpactsfx;
    i32 no_rumble = objhitobj_noattackerrumble;
    i32 throw_up = objhitobj_throwkillpartsup;
    i16 *parts_angle = objhitobj_killparts_yrot;
    i32 random = qrand();
    objhitobj_nohurtsfx = 0;
    objhitobj_noimpactsfx = 0;
    objhitobj_noattackerrumble = 0;
    objhitobj_throwkillpartsup = 0;
    objhitobj_killparts_yrot = NULL;
    BOLT_s *bolt = objhitobj_bolt;
    objhitobj_bolt = NULL;
    if (Hub_InMenu() && (target->apiobj.flags_low & 0x80))
        return 0;
    if (target->apiobj.character_data->game_character->flags_090 & 0x8000)
        return 0;
    if (target->character_context == 95 || target->character_context == 96)
        return 0;
    if (target->character_context == 90 && (!target->field_0x7a3 || (attacker && !(attacker->apiobj.flags_low & 0x80))))
        return 0;
    if (!(damage == -1 && (flags & 0x200))) {
        if (target->character_context == 0 &&
            (target->action_movement_state == 3 || target->action_movement_state == 4))
            return 0;
        if (target->character_context == 13 || target->character_context == 14)
            return 0;
    }
    if (CInfo[target->character_context].parameter & 4)
        return 0;
    if (attacker && !(attacker->apiobj.flags_low & 1))
        attacker = NULL;
    if (target->apiobj.field_0x27c != -1)
        ObstacleCamHoldUntilPlayersMove = 0;
    i32 hit = damage;
    bool instant = damage == -1;
    if (target->character_context == 76 || target->character_context == 81) {
        hit = 0;
        instant = false;
    }
    if (attacker && (attacker->apiobj.flags_low & 0x80))
        AlertSurroundingCreatures(attacker, &target->apiobj.collision_position);
    if (hit > 0 || instant)
        target->field_0xef8 |= 1;
    if (!(flags & 0x100)) {
        damage = 0;
    } else {
        if ((CInfo[target->character_context].flags & 0x4000000) ||
            (target->character_context != -1 && (target->character_context == LEGOCONTEXT_LAND_SLAM ||
                                                 target->character_context == LEGOCONTEXT_LAND_LUNGE))) {
            i32 angle = RotDiff(attacker->apiobj.movement_facing_angle, target->apiobj.movement_facing_angle);
            if (angle < 0)
                angle = -angle;
            if (angle > 0x4000 && !(LEGOCONTEXT_COMBO != -1 && attacker->character_context == LEGOCONTEXT_COMBO &&
                                    attacker->combo_branch == 6)) {
                if (target->apiobj.flags_low & 0x80)
                    NewRumble(target->pad_gamepad->pad, 0.75f, 0);
                if (LEGOCONTEXT_HOLD != -1 && target->character_context == LEGOCONTEXT_HOLD) {
                    NewBlockAction(target);
                    if ((target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD) &&
                        (target->character_context == 24 || target->character_context == 12))
                        GameAudio_PlaySfx(74, &target->apiobj.collision_position, 0, 0);
                }
                return 0;
            }
        }
    }
    if (target->field_0xd24 >= 1.0f) {
        if ((flags & 0x100) &&
            (instant || (LEGOCONTEXT_COMBO != -1 && attacker->character_context == LEGOCONTEXT_COMBO &&
                         attacker->combo_branch == 6)))
            damage = target->field_0xe37;
        ObjHitShield(attacker, target, damage, bolt);
        return 0;
    }
    if (flags == 0)
        flags = ObjHitObj_Flags(attacker);
    if ((target->field_0xefb & 8) || WORLD->current_level == VADERC_LDATA) {
        if (static_cast<u32>(hit) >= 2)
            hit = 1;
    } else if (attacker && (flags & 0x100) && attacker->character_context == 5 && attacker->combo_branch == 6 &&
               target->apiobj.field_0x27c == -1) {
        hit = -1;
    }
    if ((target->field_0xefa & 8) && (flags & 0x80)) {
        hit = 0;
    } else {
        if (WORLD->current_level == SPEEDERCHASEA_LDATA) {
            if (!disable_narrow_socks && (flags & 4) && (target->apiobj.flags_low & 0x80)) {
                hit = 0;
                goto attributed_hit;
            }
            if (target->id == id_SPEEDERBIKE && !(target->apiobj.flags_low & 0x80)) {
                if (attacker && !(attacker->apiobj.character_data->model_flags & 0x2000))
                    hit = 0;
                else if (target->ai.creature_set != 2)
                    hit = 0;
                goto attributed_hit;
            }
        }
        if (WORLD->area == HOTHBATTLE_ADATA &&
            ((target->id == id_ATAT && !(target->apiobj.flags_low & 0x80) &&
              (!attacker || !(attacker->apiobj.flags_low & 0x80))) ||
             (attacker && attacker->id == id_ATAT && !(target->apiobj.flags_low & 0x80) &&
              !(attacker->apiobj.flags_low & 0x80)))) {
            if (hit == -1 && target->id == id_ATAT) {
                if (target->character_context != 23)
                    hit = 0;
            } else
                hit = attacker && attacker->id == id_ATAT && target->id == id_DRAGBOMB ? -1 : 0;
            goto attributed_hit;
        }
        if (flags & 1)
            goto attributed_hit;
        if ((flags & 2) && !(target->apiobj.flags_low & 0x80) && WORLD->current_level != HUB_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((flags & 4) && (target->apiobj.field_0x1f4 & 0x10405) == 0x400 && WORLD->current_level != HUB_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((flags & 12) == 8 && (target->apiobj.flags_low & 0x80) && WORLD->current_level != HUB_LDATA &&
            WORLD->current_level != VADERC_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((MiniCutCam && (target->apiobj.flags_low & 0x80)) || target->pad_gamepad == ViewCamGetGamePad() ||
            ((target->apiobj.character_data->model_flags & 0x20000000) && target->field_0xcc0 == NULL))
            hit = 0;
    }
attributed_hit:
    if (attacker) {
        target->last_attacker = attacker;
        if (attacker->apiobj.flags_low & 0x80) {
            if ((target->apiobj.character_data->game_character->flags_090 & 0x40) && !VehicleArea &&
                (WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks))
                hit = -1;
            if (WORLD->current_level == HUB_LDATA &&
                AIScriptSetBaseScriptStateByName(&target->ai.script_process, "TakenHitFromPlayer")) {
                AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
                goto impact;
            }
        }
    }
    if (AIScriptSetBaseScriptStateByName(&target->ai.script_process, "TakenHit"))
        AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
impact:
    if (!no_impact) {
        if (attacker && attacker->id == id_BODYGUARD)
            PlaySfx("Grv_GuardImpact", &target->apiobj.collision_position);
        else if (attacker && attacker->id == id_IMPERIALGUARD)
            PlaySfx("wpn_bib_stab", &target->apiobj.collision_position);
        else if (bolt) {
            if (!(bolt->type_id >= 27 && bolt->type_id <= 29) && WORLD->area &&
                (WORLD->area == PODRACE_ADATA || WORLD->area == PODSPRINT_ADATA)) {
                PlaySfx("Pod_TuskHit", &bolt->position);
                no_hurt = 1;
            }
        } else if (VehicleArea || (WORLD->current_level == SPEEDERCHASEA_LDATA && !disable_narrow_socks)) {
            i32 bits =
                attacker && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1 ? 1 << attacker->apiobj.field_0x27c : 0;
            GameAudio_PlaySfx(40, &target->apiobj.collision_position, bits, 0);
        } else if (flags & 0x40) {
            if (attacker && attacker->character_context == 38 &&
                (AnimMiscFlags(attacker->apiobj.character_model, attacker->context_animation) & 4))
                PlaySfx("WhipHit", &target->apiobj.collision_position);
            else {
                i32 bits = attacker && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1
                               ? 1 << attacker->apiobj.field_0x27c
                               : 0;
                GameAudio_PlaySfx(74, &target->apiobj.collision_position, bits, 0);
            }
        } else if (attacker) {
            if (attacker->apiobj.character_data->model_flags & 8)
                GameAudio_PlaySfx(65, &target->apiobj.collision_position, GameAudio_GetPlrSfxBits(attacker), 0);
            else
                GameAudio_PlaySfx(74, &target->apiobj.collision_position, 0, 0);
        }
    }
    i32 result;
    i32 health;
    i32 coins;
    i32 hearts;
    u16 computed_angle;
    if (target->spawn_protection_timer > 0.0f || (target->field_0xefe & 0x40)) {
        result = 0;
        if (target->field_0xefd & 0x10)
            PlayerTakeHit(target, attacker);
        goto finish;
    }
    if (target->character_context == 21 && (target->field_0xefb & 8)) {
        target->spawn_protection_timer = 2.5f;
        result = 0;
        goto finish;
    }
    if ((target->apiobj.character_data->model_flags & 0x10) && !target->current_hp && (flags & 0x40) &&
        static_cast<f32>(random) * 1.5259021893143654e-05f > 0.75f) {
        DeactivatePlayer(target, 5.0f, NULL);
        result = 0;
        goto finish;
    }
    if (hit != -1 && !(target->flicker_time <= 0.0f) && (!attacker || !(attacker->apiobj.flags_low & 0x80)))
        return 0;
    if (!(hit > 0 && (target->apiobj.flags_low & 0x80))) {
        if (target->character_context == 45 && !Player_HasFastBuild(target))
            GizBuildIt_SetToStart(static_cast<GIZBUILDIT_s *>(target->field_0x788), 1, 1);
        Player_ClearContext(target, 0);
        ReleaseForce(target, 1);
    }
    if (target->field_0x108e && !TouchHacks::TouchControlsActive)
        LoseHelmet(target, 0, 0);
    if (hit != -1) {
        if (!target->current_hp || ((target->apiobj.flags_low & 0x80) && Player_HasInvincibility(target))) {
            if (target->id == id_ROYALGUARD || target->id == id_WOOKIEE)
                SetFlicker(target, 0.4f);
            PlayerTakeHit(target, attacker);
            result = 0;
            if (target->apiobj.flags_low & 0x80)
                TakeHitRumble(target, 0.666f);
            goto hurt;
        }
        health = target->current_hp - hit;
        if (health > 0) {
            if ((target->apiobj.character_data->model_flags & 0x40000000) && target->character_context == 23)
                goto refill;
            goto surviving_hit;
        }
    } else
        health = 0;
    no_impact = players_cannot_exit_speeder && target->id == id_SPEEDERBIKE && target->field_0xcc0 &&
                target->apiobj.field_0x27c != -1;
    if ((target->apiobj.character_data->model_flags & 0x20000000) && !ObjIsTargetSpeeder(target) && !no_impact) {
        coins = 0;
        if ((target->apiobj.flags_low & 0x80) && target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
            coins = LoseCoins(target, 1);
        AddPickups(coins, 0, 0, 0, &target->apiobj.collision_position, NULL, 2.0f,
                   attacker ? attacker->apiobj.field_0x27c : -1, 1.0f, 2000000.0f, attacker, 1, 0, false);
        DeactivatePlayer(target, 1000000000.0f, NULL);
        target->current_hp = target->hitpoints;
        SetFlicker(target, 0.4f);
        result = 0;
        goto hurt;
    }
    if (target->apiobj.character_data->model_flags & 0x40000000)
        goto refill;
    if ((target->field_0xefb & 8) && (!FreePlay || WORLD->current_level != CRUISERC_LDATA))
        goto surviving_hit;
    if (probe) {
        result = 2;
        goto finish;
    }
    if (target->character_context == 93) {
        PopBalloon(target);
        target->current_hp = 1;
        result = 0;
        goto hurt;
    }
    target->current_hp = 0;
    coins = 0;
    hearts = 0;
    if (target->apiobj.field_0x27c == -1) {
        if (!(target->apiobj.flags_low & 0x80)) {
            coins = BonusArea ? static_cast<u16>(target->apiobj.character_data->game_character->field_0xee)
                              : (Cheat_IsOn(16) ? 350 : 0);
            hearts = ReleaseHearts();
        }
    } else if (target->apiobj.flags_low & 0x80) {
        if (target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
            coins = LoseCoins(target, 1);
        if (!BuildUpDone)
            BuildUpScale = 1.5f;
        DrawBuildUpTime = 1.0f;
        builduptime = 1.0f;
    }
    if (hearts || coins > 0)
        AddPickups(coins, hearts, 0, 0, &target->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, attacker,
                   !BonusArea && coins < 2500, BonusArea ? 1 : 0, false);
    if (target->torpedo && WORLD->area != SPEEDERCHASE_ADATA && (!WORLD->area || !(WORLD->area->flags & 4)))
        DropTorpedoPickups(target->torpedo, target->torpedo->count);
    if (!parts_angle) {
        if (!attacker)
            parts_angle = NULL;
        else if (flags & 0x240) {
            computed_angle = NuAtan2D(target->apiobj.position.x - attacker->apiobj.position.x,
                                      target->apiobj.position.z - attacker->apiobj.position.z);
            parts_angle = reinterpret_cast<i16 *>(&computed_angle);
        }
    }
    if (WORLD->current_level == VADERC_LDATA && !netclient) {
        GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, "FinalFight", NULL);
        if (message && message->value == 1.0f && (!attacker || (attacker->apiobj.flags_low & 0x80))) {
            grab_screen_image = 1;
            if (FreePlay)
                CompleteLevel(WORLD);
            else {
                char name[16];
                nuhspecial_s special;
                for (i32 i = 1; i != 12; ++i) {
                    sprintf(name, "rock%d", i);
                    if (NuSpecialFind(WORLD->current_gscn, &special, name, 1))
                        NuSpecialSetVisibility(&special, 0);
                }
                NewCutScene(NULL, WORLD->cutscene_sys,
                            attacker && attacker->id == id_OBIWANKENOBIEP3 && target->id == id_ANAKINJEDI
                                ? const_cast<char *>("ep3_darthvader_outro2")
                                : const_cast<char *>("ep3_darthvader_outro1"),
                            1);
            }
            return 0;
        }
    }
    if (no_impact && target->field_0xcc0) {
        KillParts(target->field_0xcc0, -1, target->field_0xcc0->id == id_BODYGUARD ? 4 : -1, 1, 0.0f, 0,
                  reinterpret_cast<u16 *>(parts_angle));
        KillGameObject(target->field_0xcc0, 2, 0);
    }
    KillParts(target, -1, target->id == id_BODYGUARD ? 4 : -1, 1, throw_up ? 1.0f : 0.0f, 0,
              reinterpret_cast<u16 *>(parts_angle));
    KillGameObject(target, 2, 0);
    if (target->apiobj.flags_low & 0x80)
        GameCam_Judder(GameCam, 0.2f, 0, NULL);
    result = 2;
    goto finish;
surviving_hit:
    if (target->apiobj.flags_low & 0x80)
        TakeHitRumble(target, 0.666f);
    target->current_hp = health;
    if (hit != 0) {
        if (AIScriptSetBaseScriptStateByName(&target->ai.script_process, "LostHitPoints"))
            AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
        SetFlicker(target, 0.4f);
        if (target->id == id_SNAKE)
            SnakeBeenHit(target);
        PlayerTakeHit(target, attacker);
        if (target->apiobj.field_0x27c != -1 && hit > 0) {
            u8 value = target->field_0xe38 - hit;
            target->field_0xe38 = value ? value : 1;
            if ((target->apiobj.character_data->model_flags & 0x20) && target->apiobj.field_0x288) {
                i32 index = target->field_0xe38 - 1;
                if (index > 2)
                    index = 2;
                index = objhit_damage_joints[2 - index];
                if (target->apiobj.character_model->points_of_interest[index])
                    AddGameDebris(WORLD->debris_sys, 113,
                                  reinterpret_cast<NUVEC *>(&target->joint_matrices[index].m30));
            }
        }
    } else
        PlayerTakeHit(target, attacker);
    result = 1;
    if (target->id == id_BODYGUARD && target->current_hp == 1)
        KillParts(target, 4, -1, 1, 0.0f, 0, NULL);
    goto hurt;
hurt:
    if (!no_hurt && target->character_context != 23)
        PlayHurtSfx(target);
    goto finish;
finish:
    if (attacker && (attacker->apiobj.flags_low & 0x80)) {
        if (!no_rumble) {
            if (result == 2)
                KillRumble(attacker);
            else
                HitRumble(attacker);
        }
        if (!(flags & 0x4000) && result == 2 && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1 && Arcade)
            Arcade_Kill(attacker->apiobj.field_0x27c, target->apiobj.field_0x27c);
    }
    return result;
refill:
    coins = 0;
    if ((target->apiobj.flags_low & 0x80) && target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
        coins = LoseCoins(target, 1);
    AddPickups(coins, 0, 0, 0, &target->apiobj.collision_position, NULL, 2.0f,
               attacker ? attacker->apiobj.field_0x27c : -1, 1.0f, 2000000.0f, attacker, 1, 0, false);
    if (target->character_context != 23)
        SetFlicker(target, 0.4f);
    Player_ClearContext(target, 1);
    target->current_hp = target->hitpoints;
    result = 0;
    goto hurt;
}
