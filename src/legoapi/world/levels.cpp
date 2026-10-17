#include "decomp.h"
#include "batman.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"

// Level-system stubs that have not yet been split into a subsystem file.

i32 GetTableLocator(void);

AILOCATOR_s *LocalGetRandomLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                   f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);

AILOCATOR_s *getSpawnLocator(float clip_radius, char *name);

void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32) {
    if (no_more_loads == 0) {
        volatile i32 *abort = &abort_load;
        no_more_loads = 1;
        *abort = 1;
    }

    Door_Reset();
    NewLData = level;
    new_level_from_menu = 1;
    newlevelfrommenu_newmenuid = menu_id;
    newlevelfrommenu_newmenuy = menu_y;

    if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA) {
        return;
    }

    if (SuperStory != 0) {
        hub_from_superstory = SuperStoryEpisode;
        return;
    }

    MISSIONDATA *mission = Mission_Active(NULL);
    if (mission != NULL && NewLData != NULL && NewLData == HUB_LDATA) {
        hub_from_mission = static_cast<i8>(mission->count);
    }
}

i32 GetCounterLocator(i32);

void OffPlat(i32);
