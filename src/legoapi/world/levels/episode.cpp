#include "legoapi/world/levels/episode.h"

#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edstubs.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx_inline.h"

#include <stdio.h>
#include <string.h>

extern u8 troopercannons_beenReset;

i16 GetGenericGoon(i32);

struct TROOPERCANNON_s {
    GIZMO *base;
    GIZBUILDIT_s *buildit;
    GameObject_s *object;
    char character_name[32];
    u8 rebuilding;
    u8 reserved_0x2d[3];
};
DECOMP_ASSERT(sizeof(TROOPERCANNON_s) == 0x30, "Trooper cannon state size");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, character_name) == 0x0c, "Trooper cannon name offset");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, rebuilding) == 0x2c, "Trooper cannon rebuilding offset");

TROOPERCANNON_s troopercannons[4];

i32 droid_hack;
static i32 trooperteamcount;
static f32 teamswitchtimer;

extern i32 GizBuildIt_AtEnd(GIZBUILDIT_s *buildit);
extern GIZMO *GizmoFindByData(GIZMOSYS *system, i32 type_id, void *data);

extern f32 text3d_width;

void Text_MakeScore(u32 score, char *text);

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// Episode-global logic and helpers shared across episodes: completion/
// episode bookkeeping, super-story, boss/cutscene-adjacent helpers. The
// per-episode level *handlers* live in the matching episodeI..VI.cpp files.

// ===========================================================================
// Episode bookkeeping
// ===========================================================================

GameObject_s *FindGameObject(i32, u32, i32, i32, i32);

GameObject_s *BossKilled(i32 character_id) {
    GameObject_s *object = FindGameObject(character_id, 1, 1, 1, 0);
    if (object != NULL) {
        if ((object->field_0xefb & 8) != 0) {
            if (object->apiobj.field_0x287 == 0 && object->current_hp != 0) {
                object = NULL;
            }
        } else {
            object = NULL;
        }
    }
    return object;
}

void ReCalculateCompletionPoints();

void Episodes_CompleteAllSuperStories() {
    if (Game_EpisodeSave != NULL) {
        for (i32 i = 0; i < EPISODECOUNT; ++i) {
            EPISODESAVE_s *episode = &Game_EpisodeSave[i];
            episode->superstory_complete = 1;
            if (episode->superstory_score_target == 0) {
                episode->superstory_score_target = 1234560;
            }
            if (episode->superstory_time_limit <= 0.0f) {
                episode->superstory_time_limit = 3598.76f;
            }
        }
        ReCalculateCompletionPoints();
    }
}

void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *blowup) {
    char script[32] = "GoonBox";
    i32 model = GetGenericGoon(0);
    f32 choice = qrand() * (1.0f / 65535.0f) * 100.0f;
    if (!(choice < 50.0f)) {
        choice -= 50.0f;
        if (choice < 50.0f) {
            i32 count = static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 3.0f);
            for (i32 i = 0; i <= count; ++i) {
                i32 angle = static_cast<u16>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 65536.0f);
                GameObject_s *object =
                    AddDynamicCreature(model, &blowup->mid_position, angle, script, NULL, NULL, 1, NULL, NULL, 0, -1);
                if (object != NULL) {
                    object->field_0x1038 = 0.0f;
                    object->field_0x1004 = 0.0f;
                    object->apiobj.field_0xa8 = 0.0f * object->apiobj.character_data->model_scale;
                } else {
                    i32 coins =
                        (static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 1000.0f) / 100) * 100;
                    if (coins < 100)
                        coins = 100;
                    NUVEC direction = v010;
                    NuVecRotateX(&direction, &direction, static_cast<u16>(blowup->field_0xf6));
                    NuVecRotateY(&direction, &direction, static_cast<u16>(blowup->field_0xf8));
                    AddPickups(coins, ReleaseHearts(), 0, 0, &blowup->mid_position, &direction, 0.0f, -1, 1.5f,
                               2000000.0f, NULL, 1, 1, true);
                }
            }
            return;
        }
        choice -= 50.0f;
        if (choice < 0.0f)
            return;
    }
    i32 coins = (static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 1000.0f) / 100) * 100;
    if (coins < 100)
        coins = 100;
    NUVEC direction = v010;
    NuVecRotateX(&direction, &direction, static_cast<u16>(blowup->field_0xf6));
    NuVecRotateY(&direction, &direction, static_cast<u16>(blowup->field_0xf8));
    AddPickups(coins, ReleaseHearts(), 0, 0, &blowup->mid_position, &direction, 0.0f, -1, 1.5f, 2000000.0f, NULL, 1, 1,
               true);
}

// ===========================================================================
// HUD / score helpers
// ===========================================================================

// ===========================================================================
// Shared gameplay helpers
// ===========================================================================

static void GenerateTrooperTeamShape(minitrooperteam_s *team, i32 initialize_rotation) {
    NUVEC width;
    NUVEC depth;
    NUVEC position;
    minisnowtrooper_s *trooper;

    switch (team->formation_shape) {
        case 0: {
            width.x = team->formation_width;
            width.z = 0.0f;
            NuVecRotateY(&width, &width, team->facing_angle);
            depth.x = 0.0f;
            depth.z = team->formation_depth;
            NuVecRotateY(&depth, &depth, team->facing_angle);

            trooper = team->troopers;
            for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                trooper->formation_x = width.x * (static_cast<f32>(qrand()) * (2.0f / 65535.0f) - 1.0f) +
                                       depth.x * (static_cast<f32>(qrand()) * (2.0f / 65535.0f) - 1.0f);
                trooper->formation_z = width.z * (static_cast<f32>(qrand()) * (2.0f / 65535.0f) - 1.0f) +
                                       depth.z * (static_cast<f32>(qrand()) * (2.0f / 65535.0f) - 1.0f);
            }
            break;
        }
        case 4: {
            const u16 angle = static_cast<u16>(team->facing_angle + 0x8000);
            depth.y = 0.0f;
            trooper = team->troopers;
            for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                depth.x = trooper->formation_x;
                depth.z = trooper->formation_z;
                NuVecRotateY(&position, &depth, angle);
                trooper->formation_x = depth.x;
                trooper->formation_z = depth.z;
            }
            break;
        }
        default:
            trooper = team->troopers;
            for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                width.x = 0.0f;
                width.z = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * team->formation_width;
                NuVecRotateY(&width, &width, qrand());
                trooper->formation_x = width.x;
                trooper->formation_z = width.z;
            }
            break;
        case 1: {
            const i32 row_count = team->trooper_count >> 2;
            width.x = team->formation_width / static_cast<f32>(row_count);
            width.z = 0.0f;
            depth.x = 0.0f;
            depth.z = team->formation_depth * 0.25f;
            position.x = -(static_cast<f32>(row_count) * 0.5f * width.x);
            position.z = -(depth.z + depth.z);
            NuVecRotateY(&width, &width, team->facing_angle + 0x8000);
            NuVecRotateY(&depth, &depth, team->facing_angle + 0x8000);
            NuVecRotateY(&position, &position, team->facing_angle + 0x8000);

            trooper = team->troopers;
            for (i32 column = 0; column < 4; ++column) {
                if (droid_hack != 0 && column != 0 && (column & 1) == 0) {
                    position.x += depth.x;
                    position.z += depth.z;
                }

                f32 x = position.x;
                f32 z = position.z;
                for (i32 row = 0; row < row_count; ++row, ++trooper) {
                    trooper->formation_x = x;
                    trooper->formation_z = z;
                    if (initialize_rotation != 0)
                        trooper->rotation = team->facing_angle;
                    trooper->target_rotation = team->facing_angle;
                    z += width.z;
                    x += width.x;
                }

                position.x += depth.x;
                position.z += depth.z;
            }
            break;
        }
    }
}

static void TrooperTeamSetStateCode(minitrooperteam_s *team) {
    team->route_index = team->waypoint_index;
    team->waypoint_index = qrand() / (0xffff / team->waypoint_count + 1);

    team->route_point = &team->path->pts[team->route_index];
    team->formation_mode = qrand() / 0x4000;
    team->state_timer = static_cast<f32>(qrand()) * (4.0f / 65536.0f) + 1.0f;

    const NUVEC &destination = team->path->pts[team->waypoint_index];
    const NUVEC &origin = team->path->pts[team->route_index];
    team->facing_angle = NuAtan2D(destination.x - origin.x, destination.z - origin.z);

    if (droid_hack == 0) {
        if (team->formation_mode == 1)
            team->formation_state = 0x11;
        else
            team->formation_shape = qrand() / (0xffff / 3 + 1);
    } else {
        team->formation_shape = 4;
    }

    GenerateTrooperTeamShape(team, 0);
    minisnowtrooper_s *trooper = team->troopers;
    for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
        trooper->state_flags &= ~0x40;
        if (droid_hack != 0) {
            trooper->state_flags = (trooper->state_flags & 0x8f) | 0x20;
        } else {
            const i32 slot = qrand() / (0xffff / 3 + 1) + 1;
            trooper->state_flags = (trooper->state_flags & 0xcf) | ((slot & 3) << 4);
        }
        trooper->formation_index = static_cast<u8>(i);
    }
    team->route_state |= 0x10;
}

i32 TrooperShoot(WORLDINFO_s *world, minitrooperteam_s *team, minisnowtrooper_s *trooper, u16 *shot_angle,
                 i32 team_index) {
    NUVEC shot_position;
    shot_position.x = trooper->shot_position.x;
    shot_position.y = trooper->shot_position.y;
    shot_position.z = trooper->shot_position.z;
    NUVEC direction;

    if (droid_hack != 0) {
        f32 first_distance = 9999.0f;
        if (static_cast<i8>(Player[0]->apiobj.flags_low) < 0)
            first_distance = NuVecDist(&Player[0]->apiobj.position, &shot_position, NULL);
        f32 second_distance = 9999.0f;
        if (static_cast<i8>(Player[1]->apiobj.flags_low) < 0)
            second_distance = NuVecDist(&Player[1]->apiobj.position, &shot_position, NULL);

        GameObject_s *target;
        if (second_distance > first_distance && first_distance < 60.0f) {
            target = Player[0];
        } else if (second_distance < 60.0f) {
            target = Player[1];
        } else {
            return 0;
        }
        NuVecSub(&direction, &target->apiobj.position, &shot_position);
    } else {
        minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
        f32 nearest_distance_squared = 1.0e9f;
        minitrooperteam_s *target = NULL;

        for (i32 i = 0; i < trooperteamcount; ++i) {
            minitrooperteam_s *candidate = &teams[i];
            if (i == team_index || (candidate->state_flags & 1) == 0 || team->team_side == candidate->team_side)
                continue;

            const f32 x = candidate->position.x - team->position.x;
            const f32 z = candidate->position.z - team->position.z;
            const f32 distance_squared = x * x + z * z;
            if (distance_squared < nearest_distance_squared) {
                team->target_index = i;
                target = candidate;
                nearest_distance_squared = distance_squared;
            }
        }

        if (nearest_distance_squared >= 1.0e9f || target == NULL)
            return 0;
        NuVecSub(&direction, &target->position, &shot_position);
    }

    *shot_angle = static_cast<u16>(NuAtan2D(direction.x, direction.z) + qrand() / 37 - 0x38e);
    u16 pitch;
    FindAnglesXY(&direction, &pitch, shot_angle);

    NUMTX matrix;
    matrix.m11 = matrix.m22 = NU_COS_LUT(pitch);
    matrix.m12 = NU_SIN_LUT(pitch);
    matrix.m21 = -matrix.m12;
    matrix.m00 = 1.0f;
    matrix.m01 = matrix.m02 = matrix.m03 = matrix.m23 = matrix.m10 = matrix.m20 = matrix.m13 = matrix.m30 = matrix.m31 =
        matrix.m32 = 0.0f;
    matrix.m33 = 1.0f;
    NuMtxRotateYInline(&matrix, *shot_angle);

    BOLT_s *bolt = Bolt_Add(NULL, &shot_position, &matrix, team->bolt_type, 0);
    if (bolt != NULL)
        Bolt[bolt->index].flags &= ~4;
    return 1;
}

void InitMiniSnowTroopers(WORLDINFO_s *world, i32 team_count, i32 trooper_count, i32 use_droids) {
    if (g_lowEndLevelBehaviour == 0) {
        NUVEC shadow_position = v000;
        droid_hack = use_droids;
        if (world == NULL) {
            teamswitchtimer = 120.0f;
            return;
        }

        trooperteamcount = team_count;

        if (world->mini_trooper_teams == NULL) {
            world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
            world->mini_trooper_teams = world->giz_buffer.void_ptr;
            world->giz_buffer.void_ptr = static_cast<minitrooperteam_s *>(world->giz_buffer.void_ptr) + team_count;
        }

        if (world->mini_trooper_packets == NULL) {
            world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
            minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
            for (i32 i = 0; i < team_count; ++i) {
                world->mini_trooper_packets = world->giz_buffer.void_ptr;
                teams[i].troopers = static_cast<minisnowtrooper_s *>(world->giz_buffer.void_ptr);
                world->giz_buffer.addr += trooper_count * sizeof(minisnowtrooper_s);
            }
            world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        }

        if (world->mini_trooper_storage == NULL) {
            world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 0x40);
            world->mini_trooper_storage = world->giz_buffer.void_ptr;
            world->giz_buffer.void_ptr =
                static_cast<char *>(world->giz_buffer.void_ptr) + team_count * trooper_count * 0xc0;
        }

        for (i32 i = 0; i < team_count; ++i) {
            minitrooperteam_s *team = &static_cast<minitrooperteam_s *>(world->mini_trooper_teams)[i];
            team->waypoint_count = 0;
        }

        for (i32 team_index = 0; team_index < trooperteamcount; ++team_index) {
            minitrooperteam_s *team = &static_cast<minitrooperteam_s *>(world->mini_trooper_teams)[team_index];
            team->reserved_02c = 2000000.0f;
            team->trooper_count = static_cast<u8>(trooper_count);

            const u8 side = trooper_side[team_index] & 1;
            team->team_side = side;
            team->bolt_type = trooper_boltid[side];
            team->debris_timer = static_cast<f32>(qrand()) * (10.0f / 65535.0f) + 4.0f;

            char group_name[16];
            sprintf(group_name, "group%d", team_index + 1);
            team->path = edSpline_SplineFind(world->current_gscn, group_name);
            if (team->path == NULL) {
                team->state_flags &= ~4;
                continue;
            }

            const u32 state_flags = team->state_flags;
            team->waypoint_count = team->path->length;
            team->route_index = team_index % 5;
            team->route_point = &team->path->pts[0];
            team->state_flags = state_flags | 5;
            team->origin_x = team->route_point->x;
            team->origin_z = team->route_point->z;
            team->formation_width = static_cast<f32>(team->trooper_count >> 2) * 0.875f;
            team->formation_depth = 3.5f;

            if (droid_hack != 0) {
                NUVEC direction;
                NuVecSub(&direction, &team->path->pts[0], &team->path->pts[1]);
                team->facing_angle = NuAtan2D(direction.x, direction.z);
                team->formation_shape = 1;
            } else {
                team->facing_angle = static_cast<u16>(qrand());
                team->formation_shape = qrand() / (0xffff / 3 + 1);
            }

            GenerateTrooperTeamShape(team, 1);
            if (team->formation_shape == 1) {
                team->formation_mode = 0;
                team->state_timer = static_cast<f32>(qrand()) * (10.0f / 65535.0f);
            }
            team->formation_mode = 3;

            shadow_position.x = team->route_point->x;
            shadow_position.z = team->route_point->z;
            shadow_position.y = 10.0f;
            team->reserved_02c = GameShadow(NULL, &shadow_position, 5.0f, -1);
            if (team->reserved_02c == 2000000.0f) {
                team->reserved_02c = 0.0f;
                for (i32 i = 0; i < team->path->length; ++i)
                    team->reserved_02c += team->path->pts[i].y;
                team->reserved_02c /= static_cast<f32>(team->path->length);
            } else {
                team->reserved_02c = 2000000.0f;
            }

            shadow_position.y = team->reserved_02c;
            minisnowtrooper_s *troopers = team->troopers;
            for (i32 i = 0; i < team->trooper_count; ++i) {
                const i32 random_state = qrand() / (0xffff / 3 + 1) + 1;
                troopers[i].state_flags = (troopers[i].state_flags & 0xcf) | ((random_state & 3) << 4);
                team->troopers[i].state_flags &= ~0xc;
                team->troopers[i].speed_divisor = 6;
                troopers = team->troopers;
                minisnowtrooper_s *trooper = &troopers[i];
                if (team->formation_shape == 1) {
                    trooper->timer = 0.0f;
                } else {
                    trooper->timer =
                        static_cast<f32>(i) * (1.0f / static_cast<f32>(static_cast<i32>(team->trooper_count)));
                    trooper->rotation = team->facing_angle;
                }
                trooper->shot_position.x = trooper->formation_x + team->route_point->x;
                trooper->shot_position.z = trooper->formation_z + team->route_point->z;

                shadow_position.x = trooper->shot_position.x;
                shadow_position.z = trooper->shot_position.z;
                if (team->reserved_02c == 2000000.0f) {
                    shadow_position.y = 10.0f;
                    shadow_position.y = GameShadow(NULL, &shadow_position, 5.0f, -1);
                }
                troopers = team->troopers;
                troopers[i].shot_position.y = shadow_position.y;
            }

            minisnowtrooper_s *trooper = troopers;
            for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                trooper->formation_index = static_cast<u8>(i);
                minisnowtrooper_s *rotation_destination = team->troopers;
                const minisnowtrooper_s *slot = &rotation_destination[i];
                rotation_destination->target_rotation =
                    NuAtan2D(trooper->shot_position.x - (team->route_point->x + slot->formation_x),
                             trooper->shot_position.z - (team->route_point->z + slot->formation_z));
            }
        }

        teamswitchtimer = 120.0f;
    }
}

extern MechWeakPtr<MechObjectInterface, 4> BobaRocketTarget;

void SetBobaRocketTarget(MechObjectInterface *target) {
    BobaRocketTarget = target;
}

bool FireBountyHunterRocket(GameObject_s *object) {
    if (object->character_context != -1)
        return false;
    characterdata_s *data = object->apiobj.character_data;
    if ((data->model_flags & 0x01000000) == 0 && object->field_0x108e != 6)
        return false;
    i32 locator = data->game_character->rocket_locator;
    if (locator == -1 || object->apiobj.character_model->points_of_interest[locator] == NULL)
        return false;
    if (object->apiobj.field_0x27c != -1) {
        if (!object->apiobj.player_controlled)
            return false;
        if (!Cheat_IsOn(39))
            return false;
    }
    if (object->timer_d5c == 2.0f) {
        object->timer_d5c = 0.0f;
        FindGameMsgsWithID(2, 1, -1, NULL);
    }
    const i32 animation = object->apiobj.field_0x27d == 0 ? 77 : 2;
    object->context_animation = object->apiobj.field_0x27d == 0 ? 77 : 2;
    object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, 1);
    object->field_0xe22 &= ~4;
    if (object->context_animation_timer > 0.0f) {
        object->character_context = 20;
    }
    return object->character_context == 20;
}

static __used__ void KilledTrooperCannon(GameObject_s *object) {
    if (netclient != 0)
        return;

    for (i32 i = 0; i < 4; ++i) {
        if (troopercannons[i].object == object) {
            TROOPERCANNON_s &cannon = troopercannons[i];
            DeactivateCharacter(cannon.character_name);
            GizBuildIt_SetToStart(cannon.buildit, 0, 0);
            GIZMO *gizmo = GizmoFindByData(WORLD->gizmo_sys, gizbuildit_gizmotype_id, cannon.buildit);
            GizmoActivate(WORLD->gizmo_sys, gizmo, 1, 1);
            GizBuildit_SetVisibility(cannon.buildit, 1);
            cannon.rebuilding = 1;
            WORLD->level_progress->destroyed_trooper_cannon_mask |= 1u << i;
            return;
        }
    }
}

void InitTrooperCannons(WORLDINFO_s *) {
    memset(troopercannons, 0, sizeof(troopercannons));
}

void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id) {
    if (troopercannons_beenReset != 0) {
        if (netclient == 0 || troopercannons[0].object != NULL)
            return;
    }

    i32 cannon_index = 0;
    char name[32];
    for (i32 number = 1; number < 5; ++number) {
        TROOPERCANNON_s &cannon = troopercannons[cannon_index];
        cannon = TROOPERCANNON_s();

        sprintf(name, "trooper_cannon%d", number);
        cannon.buildit = GizBuildIt_Find(world, name);
        if (cannon.buildit != NULL) {
            sprintf(name, "troopercannon_%d", number);
            cannon.object = GetNamedGameObject(world->ai_sys, name);
            if (cannon.object != NULL)
                NuStrCpy(cannon.character_name, name);
        }

        sprintf(name, "BASE%d", number);
        cannon.base = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);

        if (netclient != 0) {
            if (cannon.object != NULL)
                ++cannon_index;
            continue;
        }

        if (cannon.buildit != NULL && cannon.object != NULL) {
            if ((world->level_progress->destroyed_trooper_cannon_mask & (1u << cannon_index)) != 0) {
                GizBuildIt_SetToStart(cannon.buildit, 0, 0);
                cannon.rebuilding = 1;
            } else {
                if (cannon.base != NULL) {
                    GizmoSetVisibility(world->gizmo_sys, cannon.base, 1, 1);
                    GizObstacle_PlayBackwards(static_cast<GIZOBSTACLE_s *>(cannon.base->object));
                }
                GizBuildIt_Finish(cannon.buildit);
                GizBuildit_SetVisibility(cannon.buildit, 0);
                cannon.rebuilding = 0;
                ActivateCharacter(name, NULL, 0);
                AddGameDebris(world->debris_sys, 0x5c, &cannon.object->apiobj.collision_position);
                GameObject_s *trooper = AddDynamicCreature(trooper_id, &cannon.object->apiobj.collision_position,
                                                           cannon.object->apiobj.field_0x276, "CannonTrooper",
                                                           &cannon.object->ai.path_info, NULL, 0, NULL, NULL, 0, 0);
                if (trooper != NULL)
                    TakeOverGameObject(trooper, cannon.object, 0, 1);
            }
        }
        ++cannon_index;
    }
    troopercannons_beenReset = 1;
}

void UpdateTrooperCannons(WORLDINFO_s *) {
    for (i32 i = 0; i < 4; ++i) {
        TROOPERCANNON_s &cannon = troopercannons[i];

        if (cannon.buildit != NULL && netclient == 0) {
            if (cannon.object != NULL) {
                if (GizBuildIt_AtEnd(cannon.buildit) && cannon.rebuilding != 0) {
                    ActivateCharacter(cannon.character_name, NULL, 0);
                    GizBuildit_SetVisibility(cannon.buildit, 0);
                    cannon.rebuilding = 0;
                    WORLD->level_progress->destroyed_trooper_cannon_mask &= ~(1u << i);
                }
            } else if (GizBuildIt_AtEnd(cannon.buildit)) {
                GizBuildIt_KillParts(cannon.buildit);
                GizBuildIt_SetToStart(cannon.buildit, 0, 0);
                GizBuildit_SetVisibility(cannon.buildit, 0);
            }
        }

        if (cannon.object == NULL || netclient != 0)
            break;

        GameObject_s *callback_object = cannon.object->field_0xcc0;
        if (callback_object == NULL)
            callback_object = cannon.object;
        if (callback_object->field_0xeb4 == NULL)
            callback_object->field_0xeb4 = KilledTrooperCannon;
    }
}

static inline i32 MiniTrooperRandomIndex(minitrooperteam_s *team) {
    return qrand() / (0xffff / team->trooper_count + 1);
}

static inline f32 MiniTrooperTurnSpeed() {
    return static_cast<f32>(qrand()) * (3.0f / 65535.0f);
}

static inline void MiniTrooperSetWanderTarget(minisnowtrooper_s *trooper, i32 shot, u16 *shot_angle) {
    if (shot != 0)
        trooper->target_rotation = static_cast<u16>(qrand() / 73 + *shot_angle);
    else
        trooper->target_rotation = static_cast<u16>(qrand());
}

static const f32 trooperspeedtab[4] = {0.3f, 0.4f, 0.5f, 0.6f};

static inline void MiniTrooperMove(minisnowtrooper_s *trooper) {
    NUVEC movement;
    movement.x = v001.x;
    movement.y = v001.y;
    movement.z = v001.z;
    const u8 speed = trooper->state_flags >> 4;
    movement.z = movement.z * FRAMETIME * trooperspeedtab[speed & 3];
    NuVecRotateY(&movement, &movement, trooper->rotation);
    trooper->shot_position.x += movement.x;
    trooper->shot_position.z += movement.z;

    if (trooper->timer >= 1.0f / static_cast<f32>(static_cast<i32>(trooper->speed_divisor))) {
        trooper->timer = 0.0f;
        trooper->state_flags = (trooper->state_flags & ~0xc) | ((((trooper->state_flags >> 2) + 1) << 2) & 0xc);
    }
    trooper->timer += FRAMETIME;
}

void UpdateMiniSnowTroopers(WORLDINFO_s *world) {
    if (g_lowEndLevelBehaviour != 0 || hothtroopers == NULL)
        return;

    const f32 camera_dir_x = GameCam->dir.x;
    const f32 camera_dir_z = GameCam->dir.z;
    const f32 camera_x = GameCam->pos.x;
    const f32 camera_z = GameCam->pos.z;
    for (i32 team_index = 0; team_index < trooperteamcount; ++team_index) {
        minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
        minitrooperteam_s *team = &teams[team_index];
        if ((team->state_flags & 4) == 0)
            continue;

        NUVEC total = v000;
        i32 shoot_due = 0;
        team->fire_timer -= FRAMETIME;
        if (__builtin_expect(team->fire_timer <= 0.0f, 0)) {
            shoot_due = team->state_flags & 1;
            if ((team->team_flags & 2) != 0) {
                team->fire_timer = 0.5f;
            } else {
                team->fire_timer = 1.0f;
            }
        }

        team->origin_x = team->route_point->x;
        team->origin_z = team->route_point->z;

        const f32 camera_dot =
            (team->position.x - camera_x) * camera_dir_x + (team->position.z - camera_z) * camera_dir_z;
        if (camera_dot <= 0.1f)
            team->state_flags &= ~1;
        else
            team->state_flags |= 1;

        const u8 state = team->formation_state >> 4;
        const u8 changed = (team->route_state & 0xf) != state;
        team->route_state = (team->route_state & ~0x10) | (changed << 4);

        u8 mode = state;
        if (droid_hack != 0) {
            if (__builtin_expect((team->formation_state & 0xe0) == 0x20, 0))
                team->formation_state = (team->formation_state & 0xf) | 0x10;

            if (player != NULL) {
                GameObject_s *nearest = player;
                const f32 player_x = player->apiobj.collision_position.x - team->position.x;
                const f32 player_z = player->apiobj.collision_position.z - team->position.z;
                if (player2 != NULL && player2->apiobj.collision_position.x - team->position.x > player_x &&
                    player2->apiobj.collision_position.z - team->position.z > player_z) {
                    nearest = player2;
                }
                if (__builtin_expect(NuVecDist(team->route_point, &nearest->apiobj.collision_position, NULL) < 40.0f, 1)) {
                    team->formation_state &= 0xf;
                    mode = 0;
                } else {
                    mode = team->formation_state >> 4;
                }
            } else {
                mode = team->formation_state >> 4;
            }
        }

        minisnowtrooper_s *trooper = team->troopers;
        u16 shot_angle;
        switch (mode) {
            case 0: {
                const i32 selected = MiniTrooperRandomIndex(team);
                i32 shot = 0;
                for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                    trooper->state_flags = (trooper->state_flags & ~0xc) | 4;
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                        MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);

                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                team->state_timer -= FRAMETIME;
                if (team->state_timer <= 0.0f)
                    TrooperTeamSetStateCode(team);
                break;
            }
            case 2: {
                const i32 selected = MiniTrooperRandomIndex(team);
                i32 shot = 0;
                u8 stopped_count = 0;
                for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                    if ((trooper->state_flags & 0x40) != 0) {
                        ++stopped_count;
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                            MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);
                    } else {
                        const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                        const f32 target_x = team->origin_x + slot->formation_x;
                        const f32 target_z = team->origin_z + slot->formation_z;
                        const f32 dx = trooper->shot_position.x - target_x;
                        const f32 dz = trooper->shot_position.z - target_z;
                        if (dx * dx + dz * dz <= 0.2f * 0.2f) {
                            trooper->state_flags |= 0x40;
                            ++stopped_count;
                        }
                    }
                    if (__builtin_expect((trooper->state_flags & 0x40) == 0, 1)) {
                        const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                        const f32 target_z = team->origin_z + slot->formation_z;
                        const f32 target_x = team->origin_x + slot->formation_x;
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (__builtin_expect((trooper->state_flags & 0x40) == 0, 1))
                            MiniTrooperMove(trooper);
                    }
                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                if (stopped_count == team->trooper_count)
                    TrooperTeamSetStateCode(team);
                break;
            }
            case 1: {
                const i32 selected = MiniTrooperRandomIndex(team);
                i32 shot = 0;
                if (team->mode_changed) {
                    team->mode_changed = 0;
                    if (team->path != NULL) {
                        team->waypoint_current = team->waypoint_index;
                        const NUVEC &point = team->path->pts[team->waypoint_current];
                        team->facing_angle = NuAtan2D(point.x - team->origin_x, point.z - team->origin_z);
                        team->formation_shape = droid_hack != 0 ? 4 : 1;
                        GenerateTrooperTeamShape(team, 0);
                        trooper = team->troopers;
                        for (i32 i = 0; i < team->trooper_count; ++i, ++trooper)
                            trooper->formation_index = static_cast<u8>(i);
                    }
                }

                u8 stopped_count = 0;
                u8 aligned_count = 0;
                for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                    if (__builtin_expect((trooper->state_flags & 0x40) != 0, 1)) {
                        ++stopped_count;
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (__builtin_expect(static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c, 1))
                            MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);
                    } else {
                        const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                        const f32 target_x = team->origin_x + slot->formation_x;
                        const f32 target_z = team->origin_z + slot->formation_z;
                        const f32 dx = trooper->shot_position.x - target_x;
                        const f32 dz = trooper->shot_position.z - target_z;
                        if (dx * dx + dz * dz <= 0.2f * 0.2f) {
                            trooper->state_flags |= 0x40;
                            ++stopped_count;
                        }
                    }

                    if ((trooper->state_flags & 0x40) == 0) {
                        const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                        const f32 target_z = team->origin_z + slot->formation_z;
                        const f32 target_x = team->origin_x + slot->formation_x;
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    }

                    if (__builtin_expect((trooper->state_flags & 0x80) != 0, 0)) {
                        ++aligned_count;
                    } else if ((trooper->state_flags & 0x40) != 0) {
                        i32 difference = static_cast<i16>(trooper->rotation - team->facing_angle);
                        if (difference < 0)
                            difference = -difference;
                        if (difference <= 0x16c) {
                            trooper->state_flags |= 0x80;
                            ++aligned_count;
                        }
                    }

                    if (__builtin_expect((trooper->state_flags & 0x40) == 0, 0))
                        MiniTrooperMove(trooper);
                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                if (aligned_count == team->trooper_count && stopped_count == aligned_count)
                    TrooperTeamSetStateCode(team);
                break;
            }
            default: {
                const i32 selected = MiniTrooperRandomIndex(team);
                i32 shot = 0;
                for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                    const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                    f32 target_x = team->origin_x + slot->formation_x;
                    f32 target_z = team->origin_z + slot->formation_z;
                    f32 dx = trooper->shot_position.x - target_x;
                    f32 dz = trooper->shot_position.z - target_z;
                    if (dx * dx + dz * dz <= 0.2f * 0.2f) {
                        trooper->formation_index = static_cast<u8>(MiniTrooperRandomIndex(team));
                        trooper->state_flags =
                            (trooper->state_flags & ~0x30) | (((qrand() / (0xffff / 3 + 1) + 1) & 3) << 4);
                        slot = &team->troopers[trooper->formation_index];
                        target_x = team->origin_x + slot->formation_x;
                        target_z = team->origin_z + slot->formation_z;
                    }

                    trooper->target_rotation =
                        NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if ((trooper->state_flags & 0x40) == 0) {
                        MiniTrooperMove(trooper);
                    } else {
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (__builtin_expect(static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c, 1))
                            MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);
                    }

                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                team->state_timer -= FRAMETIME;
                if (team->state_timer <= 0.0f)
                    TrooperTeamSetStateCode(team);
                break;
            }
        }

        const f32 count = static_cast<f32>(static_cast<i32>(team->trooper_count));
        team->position.x = total.x / count;
        team->position.y = total.y;
        team->position.z = total.z / count;

        if (team->debris_timer > 0.0f && droid_hack == 0) {
            team->debris_timer -= FRAMETIME;
            if (team->debris_timer <= 0.0f) {
                const i32 debris_trooper = MiniTrooperRandomIndex(team);
                if ((team->state_flags & 1) != 0) {
                    NUVEC position = {team->troopers[debris_trooper].shot_position.x, team->height,
                                      team->troopers[debris_trooper].shot_position.z};
                    const i32 effect_index = troopers_gdeb[0];
                    if (effect_index >= 0 && effect_index < world->debris_sys->capacity &&
                        world->debris_sys->entries[effect_index].effect != -1) {
                        AddVariableShotDebrisEffect(world->debris_sys->entries[effect_index].effect, &position, 80, 0,
                                                    0);
                    }
                    AddGameDebris(world->debris_sys, troopers_gdeb[1], &position);
                    AddGameDebris(world->debris_sys, troopers_gdeb[2], &position);
                    AddGameDebris(world->debris_sys, troopers_gdeb[3], &position);
                }
                team->debris_timer = static_cast<f32>(qrand()) * (4.0f / 65535.0f) + 2.0f;
            }
        }

        team->previous_mode = team->formation_mode;
    }
}

static const i32 TrooperStepFrames[4] = {1, 2, 1, 0};

void DrawMiniSnowTroopers(WORLDINFO_s *world) {
    NUVEC position;
    if (g_lowEndLevelBehaviour != 0 || hothtroopers == NULL) {
        return;
    }

    minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
    const i32 trooper_count = teams[0].trooper_count;
    const i32 matrix_count = trooper_count * trooperteamcount;
    NUMTX *matrices[3];
    matrices[0] = static_cast<NUMTX *>(world->mini_trooper_storage);
    matrices[1] = matrices[0] + matrix_count;
    matrices[2] = matrices[1] + matrix_count;

    NUMTX *next[3] = {matrices[0], matrices[1], matrices[2]};
    i32 counts[3] = {0, 0, 0};
    i32 first_side_counts[3] = {0, 0, 0};

    for (i32 team_index = 0; team_index < trooperteamcount; ++team_index) {
        minitrooperteam_s *team = &static_cast<minitrooperteam_s *>(world->mini_trooper_teams)[team_index];
        if ((team->state_flags & 4) == 0) {
            continue;
        }
        if ((team->state_flags & 1) == 0) {
            continue;
        }

        team->height = 0.0f;
        minisnowtrooper_s *trooper = team->troopers;
        for (i32 i = 0; i < team->trooper_count; ++i, ++trooper) {

            position.x = trooper->shot_position.x;
            position.z = trooper->shot_position.z;

            f32 ground;
            if (((i + GameTimer.update_count) & 0xf) == 0) {
                ground = team->reserved_02c;
                if (ground == 2000000.0f) {
                    position.y = 10.0f;
                    ground = GameShadow(NULL, &position, 5.0f, -1);
                }
                trooper->shot_position.y = ground;
            } else {
                ground = trooper->shot_position.y;
            }
            team->height += ground;

            const i32 frame = TrooperStepFrames[trooper->step_frame];
            const u16 draw_rotation = static_cast<u16>(trooper->rotation + 0x8000);
            NUMTX *matrix = next[frame];
            const f32 cosine = NU_COS_LUT(draw_rotation);
            const f32 sine = NU_SIN_LUT(draw_rotation);
            NUMTX rotation = {cosine, 0.0f, -sine,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                              sine,   0.0f, cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
            *matrix = rotation;

            const u16 frame_phase = static_cast<u16>(static_cast<u8>(trooper->state_flags >> 2) << 14);
            const f32 step_time = 1.0f / static_cast<f32>(static_cast<i32>(trooper->speed_divisor));
            const f32 step =
                1.0f -
                (NU_SIN_LUT(static_cast<i32>(trooper->timer * (1.0f / step_time) * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
            const u16 phase = static_cast<i32>(static_cast<f32>(static_cast<i32>(frame_phase)) + step * 16384.0f);
            position.y = ground + fabsf(NuTrigTable[static_cast<u16>(phase / 2)]) * 0.04f;
            NuMtxTranslate(matrix, &position);

            next[frame] = matrix + 1;
            ++counts[frame];
            if (!team->team_side) {
                ++first_side_counts[frame];
            }
        }

        team->height /= static_cast<f32>(static_cast<i32>(team->trooper_count));
    }

    NUMTX *second_side[3];
    for (i32 frame = 0; frame < 3; ++frame) {
        NuSpecialBurstDrawAt(&hothtroopers[frame], first_side_counts[frame], matrices[frame], 1);
        second_side[frame] = matrices[frame] + first_side_counts[frame];
    }
    for (i32 frame = 0; frame < 3; ++frame) {
        NuSpecialBurstDrawAt(&hothtroopers[frame + 3], counts[frame] - first_side_counts[frame], second_side[frame], 1);
    }
}
