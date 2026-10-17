
#include <string.h>
#include "legoapi/world/level.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/gizmos/fx/gizmopickups.h"

GameObject_s *CutDeadVehiclePlayer;
extern i32 reset_reimport;
extern void GizmoTypeStoreProgress(GIZMOSYS_s *, void *, i32, i32, char *);

void ResetLevel(WORLDINFO_s *world, char *cutscene, i32) {
    CutDeadVehiclePlayer = NULL;
    if (world != NULL && NewCutScene(NULL, world->cutscene_sys, cutscene, 0) != NULL) {
        ResetBits |= 7;
        if (player != NULL && player->apiobj.field_0x287 != 0)
            CutDeadVehiclePlayer = player;
        else if (player2 != NULL && player2->apiobj.field_0x287 != 0)
            CutDeadVehiclePlayer = player2;
        reset_reimport = 1;
    } else {
        NewGameMode();
        reset_restart = 1;
        grab_screen_image = 1;
        ResetBits |= 0x17;
        GizmoTypeStoreProgress(WORLD->gizmo_sys, WORLD, (i8)WORLD->current_level->area_level_index, gizmopickup_typeid,
                               NULL);
    }
}

void *SetLevelHack(i32 size) {
    memset(LevelHackData, 0, 0x80);
    memset(OldLevelHackData, 0, 0x80);
    LevelHackSize = size;
    LevelHackSendTimer = 0;
    return LevelHackData;
}
