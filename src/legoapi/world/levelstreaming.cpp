#include "decomp.h"
#include "batman.h"
#include "legoapi/world/area.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nutime.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "globals.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numath/nufloat.h"

#include <string.h>

extern i32 Players_AveragePos(struct nuvec_s *, struct SOCKPOSITION_s *);
extern i16 InsideLineXZ(f32, f32, f32, f32, f32, f32);

void LevelStreaming_Update(WORLDINFO_s *world) {
    if (LOADEROFF != 0 || BGLOAD == 0 || world->area == NULL || (world->area->flags & 8) != 0 || NewLData != NULL ||
        Mission_Active(NULL) != NULL) {
        return;
    }

    other_level = -1;
    if (no_more_loads == 0) {
        CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
        if (other_level_override != -1) {
            other_level = other_level_override;
        } else if (CUTSTOPGAME != 0 && cut != NULL && cut->skip_level != -1 &&
                   LDataList[cut->skip_level].area_index == LDataList[world->level_idx].area_index) {
            other_level = cut->skip_level;
        } else if ((world->current_level->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
            NUVEC position;
            if (Players_AveragePos(&position, NULL) != 0) {
                other_level = -1;
                f32 best_distance = 1000000000.0f;
                for (i32 i = 0; i < world->door_count; ++i) {
                    i32 level = InStory() != 0 || world->doors[i].freeplay_level == -1 ? world->doors[i].level
                                                                                       : world->doors[i].freeplay_level;
                    if (level != world->level_idx) {
                        f32 distance = NuVecDistSqr(&world->doors[i].pos, &position, NULL);
                        if (distance < best_distance) {
                            best_distance = distance;
                            other_level = world->doors[i].level;
                        }
                    }
                }

                LEVELLOAD_s *load = LevelLoad;
                i32 i;
                for (i = 0; i < LevelLoadCount; ++i, ++load) {
                    if (load->level == world->level_idx &&
                        (((load->flags & 1) != 0 && InStory() != 0) || ((load->flags & 2) != 0 && InStory() == 0))) {
                        break;
                    }
                }
                if (i < LevelLoadCount) {
                    if (load->first_level == load->second_level) {
                        other_level = load->first_level;
                    } else if (LEGOSPL_SPLIT != -1 && world->camera_splines[LEGOSPL_SPLIT] != NULL) {
                        NUVEC *points = world->camera_splines[LEGOSPL_SPLIT]->pts;
                        if (InsideLineXZ(position.x, position.z, points[0].x, points[0].z, points[1].x, points[1].z) !=
                            0) {
                            other_level = load->first_level;
                        } else {
                            other_level = load->second_level;
                        }
                    }
                }

                if (other_level != -1 && InStory() == 0 && (LDataList[other_level].flags & LEVEL_OUTRO) != 0) {
                    LEVELDATA_s *status_level = Area_FindStatusLevel(world->area, NULL);
                    if (status_level != NULL) {
                        other_level = status_level->idx;
                    }
                }
                if (other_level != -1 && InStory() == 0 &&
                    (LDataList[other_level].flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) != 0) {
                    other_level = -1;
                }
                if (other_level != -1 && CutScenePlayer_Active() != NULL &&
                    (LDataList[other_level].flags & LEVEL_STATUS) != 0) {
                    other_level = -1;
                }
            }
        }
    }

    if (waiting_for_level == -1 && bgGetProcActive() == NULL && other_level != -1 &&
        WorldInfo_OtherLevel(world) != other_level && LDataList[other_level].area_index == world->level_sub_id) {
        NUTIME time;
        NuTimeGet(&time);
        if (NuTimeSeconds(&time) > g_BgLoadDelayHackTimer) {
            waiting_for_level = other_level;
            next_level = other_level;
            abort_load = 0;
            WaitingForLevelTime = 0.0f;
            bgPostRequest(WorldInfo_StreamLevel, NULL, NULL, 0);
        }
    } else if (waiting_for_level != -1 && bgGetProcActive() != NULL && other_level != -1 &&
               other_level != waiting_for_level && LDataList[other_level].area_index == world->level_sub_id) {
        abort_load = 1;
    }
}

void LevelStreaming_DoorOverride(WORLDINFO_s *world, LEVELDATA_s *level, f32 radius, f32 *distance) {
    NUVEC position;
    if (level != NULL && Players_AveragePos(&position, NULL) != 0) {
        DOOR_s *door = Door_FindByIndex(world, -1, level->idx, &position);
        if (door != NULL) {
            f32 distance_squared = NuVecDistSqr(&position, &door->pos, NULL);
            if (distance_squared < radius * radius) {
                other_level_override = level->idx;
                if (distance != NULL) {
                    *distance = NuFsqrt(distance_squared);
                }
            }
        }
    }
}

void LevelProgress_ReserveBufferSpace(variptr_u *buf, variptr_u) {
    LevelProgressData = (void *)ALIGN(buf->addr, 0x10);
    buf->addr = (usize)LevelProgressData + 0x229b0;
}
