// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/world/world.h"
#include "legoapi/props/system/socksys.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx_inline.h"

#include <stdio.h>
#include <string.h>

#include "legoapi/actions/character/simpletons.h"

i32 droid_hack;
static i32 trooperteamcount;
static f32 teamswitchtimer;

// Shared gameplay helpers

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
    movement = v001;
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

static const i32 TrooperStepFrames[4] = {1, 2, 1, 0};

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
        trooper->stopped = 0;
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

void InitMiniSnowTroopers(WORLDINFO_s *world, i32 team_count, i32 trooper_count, i32 use_droids) {
    if (g_lowEndLevelBehaviour == 0) {
        NUVEC shadow_position = v000;
        droid_hack = use_droids;
        if (world == NULL) {
            teamswitchtimer = 120.0f;
            return;
        }

        trooperteamcount = team_count;

        if (__builtin_expect(world->mini_trooper_teams == NULL, 0)) {
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

            team->team_side = trooper_side[team_index];
            team->bolt_type = trooper_boltid[team->team_side];
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
            for (u32 i = 0; i < team->trooper_count; ++i) {
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
                rotation_destination->target_rotation =
                    NuAtan2D(trooper->shot_position.x - (team->route_point->x + rotation_destination[i].formation_x),
                             trooper->shot_position.z - (team->route_point->z + rotation_destination[i].formation_z));
            }
        }

        teamswitchtimer = 120.0f;
    }
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

        if (second_distance > first_distance && first_distance < 60.0f) {
            NuVecSub(&direction, &Player[0]->apiobj.position, &shot_position);
        } else if (__builtin_expect(second_distance < 60.0f, 0)) {
            NuVecSub(&direction, &Player[1]->apiobj.position, &shot_position);
        } else {
            return 0;
        }
    } else {
        minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
        f32 nearest_distance_squared = 1.0e9f;
        minitrooperteam_s *target = NULL;

        for (i32 i = 0; i < trooperteamcount; ++i) {
            minitrooperteam_s *candidate = &teams[i];
            if (__builtin_expect(
                    i == team_index || (candidate->state_flags & 1) == 0 || team->team_side == candidate->team_side, 1))
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

        if (!(nearest_distance_squared < 1.0e9f) || target == NULL)
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
    if (__builtin_expect(bolt != NULL, 1))
        Bolt[bolt->index].flags &= ~4;
    return 1;
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
        if ((teams[team_index].state_flags & 4) == 0)
            continue;

        NUVEC total = v000;
        i32 shoot_due = 0;
        teams[team_index].fire_timer -= FRAMETIME;
        if (__builtin_expect(teams[team_index].fire_timer <= 0.0f, 0)) {
            shoot_due = teams[team_index].state_flags & 1;
            teams[team_index].fire_timer = (teams[team_index].team_flags & 2) != 0 ? 0.5f : 1.0f;
        }

        teams[team_index].origin_x = teams[team_index].route_point->x;
        teams[team_index].origin_z = teams[team_index].route_point->z;

        const f32 camera_dot = (teams[team_index].position.x - camera_x) * camera_dir_x +
                               (teams[team_index].position.z - camera_z) * camera_dir_z;
        if (camera_dot <= 0.1f)
            teams[team_index].state_flags &= ~1;
        else
            teams[team_index].state_flags |= 1;

        const u8 state = teams[team_index].formation_state >> 4;
        teams[team_index].mode_changed = state != teams[team_index].previous_mode;

        u8 mode = state;
        if (droid_hack != 0) {
            if (__builtin_expect((teams[team_index].formation_state & 0xe0) == 0x20, 1))
                teams[team_index].formation_state = (teams[team_index].formation_state & 0xf) | 0x10;

            if (player != NULL) {
                GameObject_s *nearest = player;
                const f32 player_x = player->apiobj.collision_position.x - teams[team_index].position.x;
                const f32 player_z = player->apiobj.collision_position.z - teams[team_index].position.z;
                if (__builtin_expect(player2 != NULL &&
                                         player2->apiobj.collision_position.x - teams[team_index].position.x >
                                             player_x &&
                                         player2->apiobj.collision_position.z - teams[team_index].position.z > player_z,
                                     1)) {
                    nearest = player2;
                }
                if (__builtin_expect(
                        NuVecDist(teams[team_index].route_point, &nearest->apiobj.collision_position, NULL) < 40.0f,
                        1)) {
                    teams[team_index].formation_state &= 0xf;
                    mode = 0;
                } else {
                    mode = teams[team_index].formation_state >> 4;
                }
            } else {
                mode = teams[team_index].formation_state >> 4;
            }
        }

        minisnowtrooper_s *trooper = teams[team_index].troopers;
        u16 shot_angle;
        switch (mode) {
            case 0: {
                const i32 selected = MiniTrooperRandomIndex(&teams[team_index]);
                i32 shot = 0;
                for (i32 i = 0; i < teams[team_index].trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, &teams[team_index], trooper, &shot_angle, team_index);

                    trooper->state_flags = (trooper->state_flags & ~0xc) | 4;
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                        MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);

                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                teams[team_index].state_timer -= FRAMETIME;
                if (teams[team_index].state_timer <= 0.0f)
                    TrooperTeamSetStateCode(&teams[team_index]);
                break;
            }
            case 2: {
                const i32 selected = MiniTrooperRandomIndex(&teams[team_index]);
                i32 shot = 0;
                u8 stopped_count = 0;
                for (i32 i = 0; i < teams[team_index].trooper_count; ++i, ++trooper) {
                    if (__builtin_expect(i == selected && shoot_due != 0, 1))
                        shot = TrooperShoot(world, &teams[team_index], trooper, &shot_angle, team_index);

                    if (trooper->stopped != 0) {
                        ++stopped_count;
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                            MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);
                    } else {
                        const minisnowtrooper_s *slot = &teams[team_index].troopers[trooper->formation_index];
                        const f32 target_x = teams[team_index].origin_x + slot->formation_x;
                        const f32 target_z = teams[team_index].origin_z + slot->formation_z;
                        const f32 dx = trooper->shot_position.x - target_x;
                        const f32 dz = trooper->shot_position.z - target_z;
                        if (dx * dx + dz * dz <= 0.2f * 0.2f) {
                            trooper->state_flags |= 0x40;
                            ++stopped_count;
                        }
                    }
                    if (__builtin_expect(trooper->stopped == 0, 1)) {
                        const minisnowtrooper_s *slot = &teams[team_index].troopers[trooper->formation_index];
                        const f32 target_z = teams[team_index].origin_z + slot->formation_z;
                        const f32 target_x = teams[team_index].origin_x + slot->formation_x;
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (__builtin_expect(trooper->stopped == 0, 1))
                            MiniTrooperMove(trooper);
                    }
                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                if (stopped_count == teams[team_index].trooper_count)
                    TrooperTeamSetStateCode(&teams[team_index]);
                break;
            }
            case 1: {
                const i32 selected = MiniTrooperRandomIndex(&teams[team_index]);
                i32 shot = 0;
                if (teams[team_index].mode_changed) {
                    teams[team_index].mode_changed = 0;
                    if (teams[team_index].path != NULL) {
                        teams[team_index].waypoint_current = teams[team_index].waypoint_index;
                        const NUVEC &point = teams[team_index].path->pts[teams[team_index].waypoint_current];
                        teams[team_index].facing_angle =
                            NuAtan2D(point.x - teams[team_index].origin_x, point.z - teams[team_index].origin_z);
                        teams[team_index].formation_shape = droid_hack != 0 ? 4 : 1;
                        GenerateTrooperTeamShape(&teams[team_index], 0);
                        trooper = teams[team_index].troopers;
                        for (i32 i = 0; i < teams[team_index].trooper_count; ++i, ++trooper)
                            trooper->formation_index = static_cast<u8>(i);
                    }
                }

                u8 stopped_count = 0;
                u8 aligned_count = 0;
                for (i32 i = 0; i < teams[team_index].trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, &teams[team_index], trooper, &shot_angle, team_index);

                    if (__builtin_expect(trooper->stopped != 0, 1)) {
                        ++stopped_count;
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        if (__builtin_expect(static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c, 1))
                            MiniTrooperSetWanderTarget(trooper, shot, &shot_angle);
                    } else {
                        const minisnowtrooper_s *slot = &teams[team_index].troopers[trooper->formation_index];
                        const f32 target_x = teams[team_index].origin_x + slot->formation_x;
                        const f32 target_z = teams[team_index].origin_z + slot->formation_z;
                        const f32 dx = trooper->shot_position.x - target_x;
                        const f32 dz = trooper->shot_position.z - target_z;
                        if (!(dx * dx + dz * dz > 0.2f * 0.2f)) {
                            trooper->stopped = 1;
                            ++stopped_count;
                        }
                    }

                    if (trooper->stopped == 0) {
                        const minisnowtrooper_s *slot = &teams[team_index].troopers[trooper->formation_index];
                        const f32 target_z = teams[team_index].origin_z + slot->formation_z;
                        const f32 target_x = teams[team_index].origin_x + slot->formation_x;
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    }

                    if (trooper->aligned != 0) {
                        ++aligned_count;
                    } else if (trooper->stopped != 0) {
                        i32 difference = static_cast<i16>(trooper->rotation - teams[team_index].facing_angle);
                        if (difference < 0)
                            difference = -difference;
                        if (difference <= 0x16c) {
                            trooper->aligned = 1;
                            ++aligned_count;
                        }
                    }

                    if (__builtin_expect(trooper->stopped == 0, 0))
                        MiniTrooperMove(trooper);
                    total.x += trooper->shot_position.x;
                    total.z += trooper->shot_position.z;
                }
                if (__builtin_expect(aligned_count == teams[team_index].trooper_count && stopped_count == aligned_count,
                                     1))
                    TrooperTeamSetStateCode(&teams[team_index]);
                break;
            }
            default: {
                const i32 selected = MiniTrooperRandomIndex(&teams[team_index]);
                i32 shot = 0;
                for (i32 i = 0; i < teams[team_index].trooper_count; ++i, ++trooper) {
                    if (i == selected && shoot_due != 0)
                        shot = TrooperShoot(world, &teams[team_index], trooper, &shot_angle, team_index);

                    const minisnowtrooper_s *slot = &teams[team_index].troopers[trooper->formation_index];
                    f32 target_x = teams[team_index].origin_x + slot->formation_x;
                    f32 target_z = teams[team_index].origin_z + slot->formation_z;
                    f32 dx = trooper->shot_position.x - target_x;
                    f32 dz = trooper->shot_position.z - target_z;
                    if (dx * dx + dz * dz <= 0.2f * 0.2f) {
                        trooper->formation_index = static_cast<u8>(MiniTrooperRandomIndex(&teams[team_index]));
                        trooper->state_flags =
                            (trooper->state_flags & ~0x30) | (((qrand() / (0xffff / 3 + 1) + 1) & 3) << 4);
                        slot = &teams[team_index].troopers[trooper->formation_index];
                        target_x = teams[team_index].origin_x + slot->formation_x;
                        target_z = teams[team_index].origin_z + slot->formation_z;
                    }

                    trooper->target_rotation =
                        NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (trooper->stopped == 0) {
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
                teams[team_index].state_timer -= FRAMETIME;
                if (teams[team_index].state_timer <= 0.0f)
                    TrooperTeamSetStateCode(&teams[team_index]);
                break;
            }
        }

        const f32 count = static_cast<f32>(static_cast<i32>(teams[team_index].trooper_count));
        teams[team_index].position.x = total.x / count;
        teams[team_index].position.y = total.y;
        teams[team_index].position.z = total.z / count;

        if (teams[team_index].debris_timer > 0.0f && droid_hack == 0) {
            teams[team_index].debris_timer -= FRAMETIME;
            if (teams[team_index].debris_timer <= 0.0f) {
                const i32 debris_trooper = MiniTrooperRandomIndex(&teams[team_index]);
                if ((teams[team_index].state_flags & 1) != 0) {
                    NUVEC position = {teams[team_index].troopers[debris_trooper].shot_position.x,
                                      teams[team_index].height,
                                      teams[team_index].troopers[debris_trooper].shot_position.z};
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
                teams[team_index].debris_timer = static_cast<f32>(qrand()) * (4.0f / 65535.0f) + 2.0f;
            }
        }

        teams[team_index].previous_mode = teams[team_index].formation_mode;
    }
}

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

