#include "legoapi/world/levels/episode.h"

#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edstubs.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"
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
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <stdio.h>
#include <string.h>

EPISODEDATA *EDataList = NULL;

extern u8 troopercannons_beenReset;

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

u32 Episode_FindAreaFromFlags(EPISODEDATA *ep, u32 flags, u32 want) {
    for (i32 i = 0; i < (i32)ep->area_count; i++) {
        AREADATA *a = &ADataList[ep->area_ids[i]];
        if ((a->flags & flags) == want) {
            return (u8)a->index;
        }
    }
    return 0xffffffff;
}
EPISODEDATA *Episodes_ConfigureList(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, i32 maxCount,
                                    i32 *countDest) {
    NUFPAR *fp = NuFParCreate(file);
    if (fp == NULL) {
        if (countDest != NULL) {
            *countDest = 0;
        }
        return NULL;
    }

    i32 count = 0;
    bool bVar3 = false;
    EPISODEDATA *episodePtr = (EPISODEDATA *)ALIGN((usize)bufferStart->void_ptr, 4);
    bufferStart->void_ptr = (void *)episodePtr;
    EPISODEDATA *episode = episodePtr;

    while (NuFParGetLine(fp)) {
    get_word:
        NuFParGetWord(fp);
        char *a = fp->word_buf;
        if (*a == '\0') {
            continue;
        }

        if (!bVar3) {
            if (NuStrICmp(a, "episode_start") == 0 && count < maxCount) {
                episode->name_id = -1;
                episode->text_id = -1;
                episode->area_count = 0;
                episode->index = (u8)count;
                bVar3 = true;
            }
            continue;
        }

        if (NuStrICmp(a, "episode_end") == 0) {
            bVar3 = false;
            if (episode->area_count != 0) {
                count++;
                episode++;
                if (NuFParGetLine(fp) == 0) {
                    break;
                }
                goto get_word;
            }
            continue;
        }

        if (NuStrICmp(a, "area") == 0) {
            if (episode->area_count <= 9 && NuFParGetWord(fp) != 0) {
                i32 areaIndex;
                AREADATA *area = Area_FindByName(fp->word_buf, &areaIndex);
                bVar3 = true;
                if (areaIndex != -1) {
                    u32 areaCount = episode->area_count;
                    bool found = false;
                    if (areaCount != 0) {
                        if (episode->area_ids[0] == areaIndex) {
                            found = true;
                        } else {
                            for (i32 k = 1; k < (i32)areaCount; k++) {
                                if (episode->area_ids[k] == areaIndex) {
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (!found) {
                        for (i32 j = 0; j < count; j++) {
                            EPISODEDATA *prev = &episodePtr[j];
                            for (i32 byteOff = 0; byteOff < (i32)prev->area_count * 2; byteOff += 2) {
                                if (*(i16 *)((u8 *)prev->area_ids + byteOff) == areaIndex) {
                                    found = true;
                                    break;
                                }
                            }
                            if (found) {
                                break;
                            }
                        }
                    }
                    if (!found) {
                        episode->area_ids[areaCount] = (i16)areaIndex;
                        episode->area_count = (u8)(areaCount + 1);
                        if ((area->flags & (AREAFLAG_ENDING_AREA | AREAFLAG_BONUS_AREA)) == 0) {
                            episode->regular_areas += 1;
                        }
                    }
                }
            } else {
                bVar3 = true;
            }
            continue;
        }

        if (NuStrICmp(a, "name_id") == 0) {
            episode->name_id = (i16)NuFParGetInt(fp);
            bVar3 = true;
            continue;
        }

        if (NuStrICmp(a, "text_id") == 0) {
            bVar3 = true;
            episode->text_id = (i16)NuFParGetInt(fp);
            continue;
        }

        bVar3 = true;
        continue;
    }

    NuFParDestroy(fp);
    if (count != 0) {
        bufferStart->void_ptr = (void *)episode;
        if (countDest != NULL) {
            *countDest = count;
        }
        return episodePtr;
    }
    return NULL;
}

i32 Episode_ContainsArea(i32 areaId, i32 *areaIndex) {
    for (i32 i = 0; i < EPISODECOUNT; i++) {
        EPISODEDATA *episode = &EDataList[i];

        for (i32 j = 0; j < episode->area_count; j++) {
            i16 id = episode->area_ids[j];
            if (id == areaId) {
                if (areaIndex != NULL) {
                    *areaIndex = j;
                }

                return i;
            }
        }
    }

    if (areaIndex != NULL) {
        *areaIndex = -1;
    }

    return -1;
}

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

i32 CountOpenEpisodes() {
    if (Game_AreaSave == NULL) {
        return 0;
    }

    i32 open = 0;
    for (i32 i = 0; i < EPISODECOUNT; ++i) {
        if (Game_AreaSave[EDataList[i].area_ids[0]].complete != 0) {
            ++open;
        }
    }
    return open;
}

i32 Episode_IsComplete(EPISODEDATA *episode, i32 *completed_area_count) {
    if (Game_AreaSave == NULL) {
        return 0;
    }

    i32 complete = 0;
    for (i32 i = 0; i < episode->regular_areas; i++) {
        if (Game_AreaSave[episode->area_ids[i]].area_complete != 0) {
            complete++;
        }
    }

    if (completed_area_count != NULL) {
        *completed_area_count = complete;
    }
    return complete == episode->regular_areas;
}

i32 Episodes_Completed() {
    i32 completed = 0;
    for (i32 i = 0; i < EPISODECOUNT; ++i) {
        if (Episode_IsComplete(&EDataList[i], NULL) != 0) {
            ++completed;
        }
    }
    return completed;
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

i32 Episode_FindFromArea(i32 area_id) {
    for (i32 i = 0; i < EPISODECOUNT; ++i) {
        EPISODEDATA *episode = &EDataList[i];
        for (i32 j = 0; j < episode->area_count; ++j) {
            if (episode->area_ids[j] == area_id) {
                return i;
            }
        }
    }
    return -1;
}

i32 EpCompleteTotal, EpCompleteCount;
i32 EpMiniKitTotal, EpMiniKitCount;
i32 EpCharKitTotal, EpCharKitCount;
i32 EpBuildUpTotal, EpBuildUpCount;
i32 EpStoryBuildUpTotal, EpStoryBuildUpCount;
i32 EpFreePlayBuildUpTotal, EpFreePlayBuildUpCount;
i32 EpRedBrickTotal, EpRedBrickCount;
i32 EpGoldBrickTotal, EpGoldBrickCount;

i32 Episode_CountOpenAreas(i32 episode_index, i32 area_index, AREASAVE_s *saves) {
    EpCompleteTotal = EpCompleteCount = 0;
    EpMiniKitTotal = EpMiniKitCount = 0;
    EpCharKitTotal = EpCharKitCount = 0;
    EpBuildUpTotal = EpBuildUpCount = 0;
    EpStoryBuildUpTotal = EpStoryBuildUpCount = 0;
    EpFreePlayBuildUpTotal = EpFreePlayBuildUpCount = 0;
    EpRedBrickTotal = EpRedBrickCount = 0;
    EpGoldBrickTotal = EpGoldBrickCount = 0;
    if (episode_index == -1)
        return 0;
    if (GOLDBRICKFORSUPERSTORY != 0 && area_index == -1) {
        EpGoldBrickTotal = 1;
        if (Game_EpisodeSave != NULL && (Game_EpisodeSave[episode_index].flags & 0xff) != 0)
            EpGoldBrickCount = 1;
    }
    i32 open = 0;
    for (i32 i = 0; i < EDataList[episode_index].area_count; ++i) {
        const i32 id = EDataList[episode_index].area_ids[i];
        if (id != area_index && area_index != -1)
            continue;
        AREADATA *area = &ADataList[id];
        if (area == HUB_ADATA || (area->flags & 0x22) != 0)
            continue;
        if ((area->flags & 0x100) != 0) {
            if (GOLDBRICKFORSUPERBONUS != 0) {
                ++EpGoldBrickTotal;
                if (saves[i].area_complete != 0)
                    ++EpGoldBrickCount;
            }
            continue;
        }
        if ((area->flags & 4) != 0)
            continue;
        AREASAVE_s *save = &saves[id];
        ++EpCompleteTotal;
        EpMiniKitTotal += 10;
        if (save->complete != 0)
            ++open;
        if (save->area_complete != 0) {
            ++EpCompleteCount;
            ++EpGoldBrickCount;
        }
        ++EpGoldBrickTotal;
        if ((area->flags & 0x10) == 0)
            continue;
        if (save->minikit_complete != 0)
            ++EpGoldBrickCount;
        EpMiniKitCount += save->field_0x5[0];
        if (BOTHTRUEJEDIGOLDBRICKS == 0) {
            ++EpBuildUpTotal;
            EpGoldBrickTotal += 2;
            if (save->story_buildup_complete != 0 || save->freeplay_buildup_complete != 0) {
                ++EpGoldBrickCount;
                ++EpBuildUpCount;
            }
        } else {
            if (save->story_buildup_complete != 0) {
                ++EpBuildUpCount;
                ++EpGoldBrickCount;
                ++EpStoryBuildUpCount;
            }
            EpBuildUpTotal += 2;
            EpGoldBrickTotal += 3;
            if (save->freeplay_buildup_complete != 0) {
                ++EpBuildUpCount;
                ++EpGoldBrickCount;
                ++EpFreePlayBuildUpCount;
            }
        }
        ++EpRedBrickTotal;
        if (save->field_0x5[1] != 0)
            ++EpRedBrickCount;
        if (Store_IsPackUnlocked(8)) {
            ++EpCharKitTotal;
            if (GOLDBRICKFORCHALLENGE != 0)
                ++EpGoldBrickTotal;
            if (save->field_0x5[2] != 0) {
                ++EpCharKitCount;
                if (GOLDBRICKFORCHALLENGE != 0)
                    ++EpGoldBrickCount;
            }
        }
    }
    return open;
}

// ===========================================================================
// HUD / score helpers
// ===========================================================================

// ===========================================================================
// Shared gameplay helpers
// ===========================================================================

static void GenerateTrooperTeamShape(minitrooperteam_s *team, i32 initialize_rotation) {
    const i32 formation = team->formation_state & 0xf;

    if (formation == 0) {
        NUVEC width = {team->formation_width, 0.0f, 0.0f};
        NUVEC depth = {0.0f, 0.0f, team->formation_depth};
        NuVecRotateY(&width, &width, team->facing_angle);
        NuVecRotateY(&depth, &depth, team->facing_angle);

        for (i32 i = 0; i < team->trooper_count; ++i) {
            minisnowtrooper_s &trooper = team->troopers[i];
            const f32 width_scale = static_cast<f32>(qrand()) * (2.0f / 65536.0f) - 1.0f;
            const f32 depth_scale = static_cast<f32>(qrand()) * (2.0f / 65536.0f) - 1.0f;
            trooper.formation_x = width.x * width_scale + depth.x * depth_scale;

            const f32 width_scale_z = static_cast<f32>(qrand()) * (2.0f / 65536.0f) - 1.0f;
            const f32 depth_scale_z = static_cast<f32>(qrand()) * (2.0f / 65536.0f) - 1.0f;
            trooper.formation_z = width.z * width_scale_z + depth.z * depth_scale_z;
        }
        return;
    }

    if (formation == 4) {
        const u16 angle = static_cast<u16>(team->facing_angle + 0x8000);
        for (i32 i = 0; i < team->trooper_count; ++i) {
            minisnowtrooper_s &trooper = team->troopers[i];
            NUVEC offset = {trooper.formation_x, 0.0f, trooper.formation_z};
            NuVecRotateY(&offset, &offset, angle);
            trooper.formation_x = offset.x;
            trooper.formation_z = offset.z;
        }
        return;
    }

    if (formation != 1) {
        for (i32 i = 0; i < team->trooper_count; ++i) {
            NUVEC offset = {0.0f, 0.0f, static_cast<f32>(qrand()) * (1.0f / 65536.0f) * team->formation_width};
            NuVecRotateY(&offset, &offset, qrand());
            team->troopers[i].formation_x = offset.x;
            team->troopers[i].formation_z = offset.z;
        }
        return;
    }

    const i32 row_count = team->trooper_count >> 2;
    const f32 row_step = team->formation_width / static_cast<f32>(row_count);
    const u16 angle = static_cast<u16>(team->facing_angle + 0x8000);

    NUVEC along = {row_step, 0.0f, 0.0f};
    NUVEC across = {0.0f, 0.0f, team->formation_depth * 0.25f};
    NUVEC position = {-static_cast<f32>(row_count) * 0.5f * row_step, 0.0f, -team->formation_depth * 0.5f};
    NuVecRotateY(&along, &along, angle);
    NuVecRotateY(&across, &across, angle);
    NuVecRotateY(&position, &position, angle);

    minisnowtrooper_s *trooper = team->troopers;
    for (i32 column = 0; column < 4; ++column) {
        if (droid_hack != 0 && column != 0 && (column & 1) == 0) {
            position.x += across.x;
            position.z += across.z;
        }

        NUVEC current = position;
        for (i32 row = 0; row < row_count; ++row, ++trooper) {
            trooper->formation_x = current.x;
            trooper->formation_z = current.z;
            if (initialize_rotation != 0)
                trooper->rotation = team->facing_angle;
            trooper->target_rotation = team->facing_angle;
            current.x += along.x;
            current.z += along.z;
        }

        position.x += across.x;
        position.z += across.z;
    }
}

static void TrooperTeamSetStateCode(minitrooperteam_s *team) {
    team->route_state = (team->route_state & 0x1f) | ((team->waypoint_state >> 3) << 5);

    const i32 waypoint_count = (*reinterpret_cast<u16 *>(&team->waypoint_state) >> 6) & 7;
    const i32 waypoint = qrand() / (0xffff / waypoint_count + 1);
    team->waypoint_state = (team->waypoint_state & 0xc7) | ((waypoint & 7) << 3);

    team->route_point = &team->path->pts[team->route_state >> 5];
    team->formation_state = (team->formation_state & 0xf) | ((qrand() / 0x4000) << 4);
    team->state_timer = static_cast<f32>(qrand()) * (4.0f / 65536.0f) + 1.0f;

    const NUVEC &destination = team->path->pts[(team->waypoint_state >> 3) & 7];
    const NUVEC &origin = team->path->pts[team->route_state >> 5];
    team->facing_angle = NuAtan2D(destination.x - origin.x, destination.z - origin.z);

    if (droid_hack == 0) {
        if ((team->formation_state & 0xf0) == 0x10)
            team->formation_state = 0x11;
        else
            team->formation_state = (team->formation_state & 0xf0) | (qrand() / (0xffff / 3 + 1) & 0xf);
    } else {
        team->formation_state = (team->formation_state & 0xf0) | 4;
    }

    GenerateTrooperTeamShape(team, 0);
    for (i32 i = 0; i < team->trooper_count; ++i) {
        minisnowtrooper_s &trooper = team->troopers[i];
        trooper.state_flags &= ~0x40;
        if (droid_hack != 0) {
            trooper.state_flags = (trooper.state_flags & 0x8f) | 0x20;
        } else {
            trooper.state_flags = (trooper.state_flags & 0xcf) | (((qrand() / (0xffff / 3 + 1) + 1) & 3) << 4);
        }
        trooper.formation_index = static_cast<u8>(i);
    }
    team->route_state |= 0x10;
}

void InitMiniSnowTroopers(WORLDINFO_s *world, i32 team_count, i32 trooper_count, i32 use_droids) {
    if (g_lowEndLevelBehaviour != 0)
        return;

    droid_hack = use_droids;
    if (world == NULL) {
        teamswitchtimer = 120.0f;
        return;
    }

    trooperteamcount = team_count;

    if (world->mini_trooper_teams == NULL) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->mini_trooper_teams = world->giz_buffer.void_ptr;
        world->giz_buffer.addr += team_count * sizeof(minitrooperteam_s);
    }

    minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
    if (world->mini_trooper_packets == NULL) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        minisnowtrooper_s *last_packet = NULL;
        for (i32 i = 0; i < team_count; ++i) {
            last_packet = static_cast<minisnowtrooper_s *>(world->giz_buffer.void_ptr);
            teams[i].troopers = last_packet;
            world->giz_buffer.addr += trooper_count * sizeof(minisnowtrooper_s);
        }
        world->mini_trooper_packets = last_packet;
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    }

    if (world->mini_trooper_storage == NULL) {
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 0x40);
        world->mini_trooper_storage = world->giz_buffer.void_ptr;
        world->giz_buffer.addr += team_count * trooper_count * 0xc0;
    }

    for (i32 i = 0; i < team_count; ++i)
        *reinterpret_cast<u16 *>(&teams[i].waypoint_state) &= 0xfe3f;

    for (i32 team_index = 0; team_index < team_count; ++team_index) {
        minitrooperteam_s *team = &teams[team_index];
        team->reserved_02c = 2000000.0f;
        team->trooper_count = static_cast<u8>(trooper_count);

        const i32 side = trooper_side[team_index] & 1;
        team->team_flags = (team->team_flags & ~2) | (side << 1);
        team->bolt_type = trooper_boltid[side];
        team->debris_timer = static_cast<f32>(qrand()) * (10.0f / 65536.0f) + 4.0f;

        char group_name[128];
        sprintf(group_name, "group%d", team_index + 1);
        team->path = edSpline_SplineFind(world->current_gscn, group_name);
        if (team->path == NULL) {
            team->state_flags &= ~4;
            continue;
        }

        u16 waypoint_data = *reinterpret_cast<u16 *>(&team->waypoint_state);
        waypoint_data = (waypoint_data & 0xfe3f) | ((team->path->length & 7) << 6);
        *reinterpret_cast<u16 *>(&team->waypoint_state) = waypoint_data;
        team->route_state = (team->route_state & 0x1f) | ((team_index % 5) << 5);
        team->state_flags |= 5;
        team->route_point = &team->path->pts[0];
        team->origin_x = team->route_point->x;
        team->origin_z = team->route_point->z;
        team->formation_width = static_cast<f32>(trooper_count >> 2) * 0.875f;
        team->formation_depth = 3.5f;

        if (droid_hack != 0) {
            NUVEC direction;
            NuVecSub(&direction, &team->path->pts[0], &team->path->pts[1]);
            team->facing_angle = NuAtan2D(direction.x, direction.z);
            team->formation_state = (team->formation_state & 0xf0) | 1;
        } else {
            team->facing_angle = static_cast<u16>(qrand());
            team->formation_state = (team->formation_state & 0xf0) | (qrand() / (0xffff / 3 + 1));
        }

        GenerateTrooperTeamShape(team, 1);
        if ((team->formation_state & 0xf) == 1) {
            team->formation_state &= 0xf;
            team->state_timer = static_cast<f32>(qrand()) * (10.0f / 65536.0f);
        } else {
            team->formation_state = (team->formation_state & 0xf) | 0x30;
        }

        NUVEC shadow_position = {team->route_point->x, 10.0f, team->route_point->z};
        team->reserved_02c = GameShadow(NULL, &shadow_position, 5.0f, -1);
        if (team->reserved_02c == 2000000.0f) {
            team->reserved_02c = 0.0f;
            for (i32 i = 0; i < team->path->length; ++i)
                team->reserved_02c += team->path->pts[i].y;
            if (team->path->length != 0)
                team->reserved_02c /= static_cast<f32>(team->path->length);
        }

        for (i32 i = 0; i < trooper_count; ++i) {
            minisnowtrooper_s *trooper = &team->troopers[i];
            trooper->formation_index = static_cast<u8>(i);
            trooper->state_flags = (trooper->state_flags & 0xcf) | (((qrand() / (0xffff / 3 + 1) + 1) & 3) << 4);
            trooper->state_flags &= ~0xc;
            trooper->speed_divisor = 6;
            trooper->timer =
                (team->formation_state & 0xf) == 1 ? 0.0f : static_cast<f32>(i) / static_cast<f32>(trooper_count);
            trooper->rotation = team->facing_angle;
            trooper->shot_position.x = trooper->formation_x + team->route_point->x;
            trooper->shot_position.z = trooper->formation_z + team->route_point->z;

            if (team->reserved_02c == 2000000.0f) {
                NUVEC position = {trooper->shot_position.x, 10.0f, trooper->shot_position.z};
                trooper->shot_position.y = GameShadow(NULL, &position, 5.0f, -1);
            } else {
                trooper->shot_position.y = team->reserved_02c;
            }

            trooper->target_rotation =
                NuAtan2D(trooper->shot_position.x - (team->route_point->x + trooper->formation_x),
                         trooper->shot_position.z - (team->route_point->z + trooper->formation_z));
        }
    }

    teamswitchtimer = 120.0f;
}

i32 TrooperShoot(WORLDINFO_s *world, minitrooperteam_s *team, minisnowtrooper_s *trooper, u16 *shot_angle,
                 i32 team_index) {
    NUVEC shot_position = trooper->shot_position;
    NUVEC direction;

    if (droid_hack != 0) {
        f32 player_distances[2] = {10000.0f, 10000.0f};
        for (i32 i = 0; i < 2; ++i) {
            if (static_cast<i8>(Player[i]->apiobj.flags_low) < 0)
                player_distances[i] = NuVecDist(&Player[i]->apiobj.position, &shot_position, NULL);
        }

        GameObject_s *target;
        if (player_distances[1] > player_distances[0]) {
            if (player_distances[0] >= 60.0f)
                return 0;
            target = Player[0];
        } else {
            if (player_distances[1] >= 60.0f)
                return 0;
            target = Player[1];
        }
        NuVecSub(&direction, &target->apiobj.position, &shot_position);
    } else {
        minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);
        f32 nearest_distance_squared = 1.0e9f;
        minitrooperteam_s *target = NULL;

        for (i32 i = 0; i < trooperteamcount; ++i) {
            minitrooperteam_s *candidate = &teams[i];
            if (i == team_index || (candidate->state_flags & 1) == 0 ||
                ((team->team_flags ^ candidate->team_flags) & 2) == 0)
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
    const f32 sin_x = NU_SIN_LUT(pitch);
    const f32 cos_x = NU_COS_LUT(pitch);
    const f32 sin_y = NU_SIN_LUT(*shot_angle);
    const f32 cos_y = NU_COS_LUT(*shot_angle);
    matrix.m00 = cos_y;
    matrix.m01 = 0.0f;
    matrix.m02 = -sin_y;
    matrix.m03 = 0.0f;
    matrix.m10 = sin_x * sin_y;
    matrix.m11 = cos_x;
    matrix.m12 = sin_x * cos_y;
    matrix.m13 = 0.0f;
    matrix.m20 = cos_x * sin_y;
    matrix.m21 = -sin_x;
    matrix.m22 = cos_x * cos_y;
    matrix.m23 = 0.0f;
    matrix.m30 = 0.0f;
    matrix.m31 = 0.0f;
    matrix.m32 = 0.0f;
    matrix.m33 = 1.0f;

    BOLT_s *bolt = Bolt_Add(NULL, &shot_position, &matrix, team->bolt_type, 0);
    if (bolt != NULL)
        Bolt[bolt->index].flags &= ~4;
    return 1;
}

NuMechPtr<MechObjectInterface, 4> BobaRocketTarget;

void SetBobaRocketTarget(MechObjectInterface *target) {
    BobaRocketTarget = target;
}

bool FireBountyHunterRocket(GameObject_s *object) {
    if (object->character_context == -1) {
        characterdata_s *data = object->apiobj.character_data;
        if ((data->model_flags & 0x01000000) != 0 || object->field_0x108e == 6) {
            i32 locator = data->game_character->rocket_locator;
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL &&
                (object->apiobj.field_0x27c == -1 || (object->apiobj.player_controlled && Cheat_IsOn(39)))) {
                if (object->timer_d5c == 2.0f) {
                    object->timer_d5c = 0.0f;
                    FindGameMsgsWithID(2, 1, -1, NULL);
                }
                object->context_animation = object->apiobj.field_0x27d == 0 ? 77 : 2;
                object->context_animation_timer =
                    AnimDuration(object->id, object->apiobj.field_0x27d == 0 ? 77 : 2, 0.0f, 0.0f, 1);
                object->field_0xe22 &= ~4;
                if (object->context_animation_timer > 0.0f) {
                    object->character_context = 20;
                }
                return object->character_context == 20;
            }
        }
    }
    return false;
}

static __used__ void KilledTrooperCannon(GameObject_s *object) {
    if (netclient != 0)
        return;

    i32 i;
    for (i = 0; i < 4; ++i) {
        if (troopercannons[i].object == object)
            break;
    }

    if (i < 4) {
        TROOPERCANNON_s &cannon = troopercannons[i];
        DeactivateCharacter(cannon.character_name);
        GizBuildIt_SetToStart(cannon.buildit, 0, 0);
        GIZMO *gizmo = GizmoFindByData(WORLD->gizmo_sys, gizbuildit_gizmotype_id, cannon.buildit);
        GizmoActivate(WORLD->gizmo_sys, gizmo, 1, 1);
        GizBuildit_SetVisibility(cannon.buildit, 1);
        cannon.rebuilding = 1;
        WORLD->level_progress->destroyed_trooper_cannon_mask |= 1u << i;
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
        memset(&cannon, 0, sizeof(cannon));

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

        if (cannon.buildit != NULL && netclient == 0 && GizBuildIt_AtEnd(cannon.buildit)) {
            if (cannon.object != NULL) {
                if (cannon.rebuilding != 0) {
                    ActivateCharacter(cannon.character_name, NULL, 0);
                    GizBuildit_SetVisibility(cannon.buildit, 0);
                    cannon.rebuilding = 0;
                    WORLD->level_progress->destroyed_trooper_cannon_mask &= ~(1u << i);
                }
            } else {
                GizBuildIt_KillParts(cannon.buildit);
                GizBuildIt_SetToStart(cannon.buildit, 0, 0);
                GizBuildit_SetVisibility(cannon.buildit, 0);
            }
        }

        if (cannon.object != NULL && netclient == 0) {
            GameObject_s *callback_object = cannon.object->field_0xcc0;
            if (callback_object == NULL)
                callback_object = cannon.object;
            if (callback_object->field_0xeb4 == NULL)
                callback_object->field_0xeb4 = KilledTrooperCannon;
        }
    }
}

static inline i32 MiniTrooperRandomIndex(u8 count) {
    return qrand() / (0xffff / count + 1);
}

static inline f32 MiniTrooperTurnSpeed() {
    return static_cast<f32>(qrand()) * (3.0f / 65536.0f);
}

static inline void MiniTrooperSetWanderTarget(minisnowtrooper_s *trooper, i32 shot, u16 shot_angle) {
    trooper->target_rotation = shot != 0 ? static_cast<u16>(shot_angle + qrand() / 73) : static_cast<u16>(qrand());
}

static inline void MiniTrooperMove(minisnowtrooper_s *trooper) {
    static const f32 speeds[4] = {0.3f, 0.4f, 0.5f, 0.6f};

    NUVEC movement = v001;
    movement.z *= FRAMETIME * speeds[(trooper->state_flags >> 4) & 3];
    NuVecRotateY(&movement, &movement, trooper->rotation);
    trooper->shot_position.x += movement.x;
    trooper->shot_position.z += movement.z;

    if (trooper->timer >= 1.0f / static_cast<f32>(trooper->speed_divisor)) {
        trooper->timer = 0.0f;
        trooper->state_flags = (trooper->state_flags & ~0xc) | (((trooper->state_flags & 0xc) + 4) & 0xc);
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
    minitrooperteam_s *teams = static_cast<minitrooperteam_s *>(world->mini_trooper_teams);

    for (i32 team_index = 0; team_index < trooperteamcount; ++team_index) {
        minitrooperteam_s *team = &teams[team_index];
        if ((team->state_flags & 4) == 0)
            continue;

        i32 shoot_due = 0;
        team->fire_timer -= FRAMETIME;
        if (team->fire_timer <= 0.0f) {
            shoot_due = team->state_flags & 1;
            team->fire_timer = (team->team_flags & 2) != 0 ? 0.5f : 1.0f;
        }

        team->origin_x = team->route_point->x;
        team->origin_z = team->route_point->z;

        f32 total_x = v000.x;
        f32 total_z = v000.z;
        const f32 camera_dot =
            (team->position.x - camera_x) * camera_dir_x + (team->position.z - camera_z) * camera_dir_z;
        if (camera_dot > 0.1f)
            team->state_flags |= 1;
        else
            team->state_flags &= ~1;

        const u8 state = team->formation_state >> 4;
        team->route_state = (team->route_state & ~0x10) | (((team->route_state & 0xf) != state) ? 0x10 : 0);

        i32 mode = state;
        if (droid_hack != 0) {
            if ((team->formation_state & 0xe0) == 0x20)
                team->formation_state = (team->formation_state & 0xf) | 0x10;

            if (player != NULL) {
                GameObject_s *nearest = player;
                if (player2 != NULL &&
                    player2->apiobj.collision_position.x - team->position.x >
                        player->apiobj.collision_position.x - team->position.x &&
                    player2->apiobj.collision_position.z - team->position.z >
                        player->apiobj.collision_position.z - team->position.z) {
                    nearest = player2;
                }
                if (NuVecDist(team->route_point, &nearest->apiobj.collision_position, NULL) < 40.0f) {
                    team->formation_state &= 0xf;
                    mode = 0;
                } else {
                    mode = team->formation_state >> 4;
                }
            } else {
                mode = team->formation_state >> 4;
            }
        }

        const i32 selected = MiniTrooperRandomIndex(team->trooper_count);
        i32 shot = 0;
        u16 shot_angle = 0;
        bool timed_transition = false;
        bool immediate_transition = false;

        if (mode == 0) {
            for (i32 i = 0; i < team->trooper_count; ++i) {
                minisnowtrooper_s *trooper = &team->troopers[i];
                if (shoot_due != 0 && i == selected)
                    shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                trooper->state_flags = (trooper->state_flags & ~0xc) | 4;
                trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                    MiniTrooperSetWanderTarget(trooper, shot, shot_angle);

                total_x += trooper->shot_position.x;
                total_z += trooper->shot_position.z;
            }
            timed_transition = true;
        } else if (mode == 2) {
            u8 stopped_count = 0;
            for (i32 i = 0; i < team->trooper_count; ++i) {
                minisnowtrooper_s *trooper = &team->troopers[i];
                if (shoot_due != 0 && i == selected)
                    shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                if ((trooper->state_flags & 0x40) != 0) {
                    ++stopped_count;
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                        MiniTrooperSetWanderTarget(trooper, shot, shot_angle);
                } else {
                    const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                    const f32 target_x = team->origin_x + slot->formation_x;
                    const f32 target_z = team->origin_z + slot->formation_z;
                    const f32 dx = trooper->shot_position.x - target_x;
                    const f32 dz = trooper->shot_position.z - target_z;
                    if (dx * dx + dz * dz <= 0.04f) {
                        trooper->state_flags |= 0x40;
                        ++stopped_count;
                    } else {
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                        MiniTrooperMove(trooper);
                    }
                }

                total_x += trooper->shot_position.x;
                total_z += trooper->shot_position.z;
            }
            immediate_transition = stopped_count == team->trooper_count;
        } else if (mode == 1) {
            if ((team->route_state & 0x10) != 0) {
                team->route_state &= ~0x10;
                if (team->path != NULL) {
                    team->waypoint_state = (team->waypoint_state & 0xf8) | ((team->waypoint_state >> 3) & 7);
                    const NUVEC &point = team->path->pts[team->waypoint_state & 7];
                    team->facing_angle = NuAtan2D(point.x - team->origin_x, point.z - team->origin_z);
                    team->formation_state = (team->formation_state & 0xf0) | (droid_hack == 1 ? 4 : 2);
                    GenerateTrooperTeamShape(team, 0);
                    for (i32 i = 0; i < team->trooper_count; ++i)
                        team->troopers[i].formation_index = static_cast<u8>(i);
                }
            }

            u8 stopped_count = 0;
            u8 aligned_count = 0;
            for (i32 i = 0; i < team->trooper_count; ++i) {
                minisnowtrooper_s *trooper = &team->troopers[i];
                if (shoot_due != 0 && i == selected)
                    shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                if ((trooper->state_flags & 0x40) != 0) {
                    ++stopped_count;
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                        MiniTrooperSetWanderTarget(trooper, shot, shot_angle);
                } else {
                    const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                    const f32 target_x = team->origin_x + slot->formation_x;
                    const f32 target_z = team->origin_z + slot->formation_z;
                    const f32 dx = trooper->shot_position.x - target_x;
                    const f32 dz = trooper->shot_position.z - target_z;
                    if (dx * dx + dz * dz <= 0.04f) {
                        trooper->state_flags |= 0x40;
                        ++stopped_count;
                    } else {
                        trooper->target_rotation =
                            NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                        trooper->rotation =
                            SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    }
                }

                if ((trooper->state_flags & 0x80) != 0) {
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

                if ((trooper->state_flags & 0x40) == 0)
                    MiniTrooperMove(trooper);
                total_x += trooper->shot_position.x;
                total_z += trooper->shot_position.z;
            }
            immediate_transition = stopped_count == team->trooper_count && stopped_count == aligned_count;
        } else {
            for (i32 i = 0; i < team->trooper_count; ++i) {
                minisnowtrooper_s *trooper = &team->troopers[i];
                if (shoot_due != 0 && i == selected)
                    shot = TrooperShoot(world, team, trooper, &shot_angle, team_index);

                const minisnowtrooper_s *slot = &team->troopers[trooper->formation_index];
                f32 target_x = team->origin_x + slot->formation_x;
                f32 target_z = team->origin_z + slot->formation_z;
                f32 dx = trooper->shot_position.x - target_x;
                f32 dz = trooper->shot_position.z - target_z;
                if (dx * dx + dz * dz <= 0.04f) {
                    trooper->formation_index = static_cast<u8>(MiniTrooperRandomIndex(team->trooper_count));
                    trooper->state_flags =
                        (trooper->state_flags & ~0x30) | (((qrand() / (0xffff / 3 + 1) + 1) & 3) << 4);
                    slot = &team->troopers[trooper->formation_index];
                    target_x = team->origin_x + slot->formation_x;
                    target_z = team->origin_z + slot->formation_z;
                }

                trooper->target_rotation =
                    NuAtan2D(target_x - trooper->shot_position.x, target_z - trooper->shot_position.z);
                trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                if ((trooper->state_flags & 0x40) != 0) {
                    trooper->rotation = SeekRot(trooper->rotation, trooper->target_rotation, MiniTrooperTurnSpeed());
                    if (static_cast<i32>(trooper->rotation) - static_cast<i32>(trooper->target_rotation) <= 0x16c)
                        MiniTrooperSetWanderTarget(trooper, shot, shot_angle);
                } else {
                    MiniTrooperMove(trooper);
                }

                total_x += trooper->shot_position.x;
                total_z += trooper->shot_position.z;
            }
            timed_transition = true;
        }

        if (timed_transition) {
            team->state_timer -= FRAMETIME;
            if (team->state_timer <= 0.0f)
                TrooperTeamSetStateCode(team);
        } else if (immediate_transition) {
            TrooperTeamSetStateCode(team);
        }

        const f32 count = static_cast<f32>(team->trooper_count);
        team->position.x = total_x / count;
        team->position.y = v000.y;
        team->position.z = total_z / count;

        if (team->debris_timer > 0.0f && droid_hack == 0) {
            team->debris_timer -= FRAMETIME;
            if (team->debris_timer <= 0.0f) {
                const i32 debris_trooper = MiniTrooperRandomIndex(team->trooper_count);
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
                team->debris_timer = static_cast<f32>(qrand()) * (4.0f / 65536.0f) + 2.0f;
            }
        }

        team->route_state = (team->route_state & 0xf0) | (team->formation_state >> 4);
    }
}

static const i32 TrooperStepFrames[4] = {1, 2, 1, 0};

void DrawMiniSnowTroopers(WORLDINFO_s *world) {
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
        minitrooperteam_s *team = &teams[team_index];
        if ((team->state_flags & 4) == 0) {
            continue;
        }
        if ((team->state_flags & 1) == 0) {
            continue;
        }

        team->height = 0.0f;
        for (i32 i = 0; i < team->trooper_count; ++i) {
            minisnowtrooper_s *trooper = &team->troopers[i];
            NUVEC position;
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

            const i32 frame = TrooperStepFrames[(trooper->state_flags >> 2) & 3];
            NUMTX *matrix = next[frame];
            const u16 draw_rotation = static_cast<u16>(trooper->rotation + 0x8000);
            const f32 cosine = NU_COS_LUT(draw_rotation);
            const f32 sine = NU_SIN_LUT(draw_rotation);
            NUMTX rotation = {cosine, 0.0f, -sine,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                              sine,   0.0f, cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
            *matrix = rotation;

            const f32 step_time = 1.0f / static_cast<f32>(trooper->speed_divisor);
            const f32 step =
                1.0f - (NU_SIN_LUT(static_cast<i32>(trooper->timer / step_time * 32768.0f + 65536.0f)) + 1.0f) * 0.5f;
            const u16 frame_phase = static_cast<u16>((trooper->state_flags >> 2) << 14);
            const i32 phase = static_cast<i32>(static_cast<f32>(frame_phase) + step * 65536.0f);
            position.y = ground + fabsf(NU_SIN_LUT(phase)) * 0.04f;
            NuMtxTranslate(matrix, &position);

            next[frame] = matrix + 1;
            ++counts[frame];
            if ((team->team_flags & 2) == 0) {
                ++first_side_counts[frame];
            }
        }

        team->height /= static_cast<f32>(static_cast<i32>(team->trooper_count));
    }

    for (i32 frame = 0; frame < 3; ++frame) {
        NuSpecialBurstDrawAt(&hothtroopers[frame], first_side_counts[frame], matrices[frame], 1);
    }
    for (i32 frame = 0; frame < 3; ++frame) {
        NuSpecialBurstDrawAt(&hothtroopers[frame + 3], counts[frame] - first_side_counts[frame],
                             matrices[frame] + first_side_counts[frame], 1);
    }
}

static __used__ void seed_chase(f32 *, i32, abi_long) {
    STUBBED();
}
