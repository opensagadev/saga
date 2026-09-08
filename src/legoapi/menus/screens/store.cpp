#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/levels.h"

#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"

#include <string.h>

struct GameObject_s;
struct LEVEL_PROGRESS_s;
struct WORLDINFO_s;

extern GAMEPAD_s GamePad[64];
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

extern void TurnEpisodeDoorLightsOn(i32);

STOREPACK StorePack[11] = {0};

i32 Store_FindPack(i32 id, char *name) {
    if (id == -1) {
        if (name == NULL) {
            return -1;
        }

        if (NuStrCmp(StorePack[0].name, name) == 0) {
            return 0;
        } else if (NuStrCmp(StorePack[1].name, name) == 0) {
            return 1;
        } else if (NuStrCmp(StorePack[2].name, name) == 0) {
            return 2;
        } else if (NuStrCmp(StorePack[3].name, name) == 0) {
            return 3;
        } else if (NuStrCmp(StorePack[4].name, name) == 0) {
            return 4;
        } else if (NuStrCmp(StorePack[5].name, name) == 0) {
            return 5;
        } else if (NuStrCmp(StorePack[6].name, name) == 0) {
            return 6;
        } else if (NuStrCmp(StorePack[7].name, name) == 0) {
            return 7;
        } else if (NuStrCmp(StorePack[8].name, name) == 0) {
            return 8;
        } else if (NuStrCmp(StorePack[9].name, name) == 0) {
            return 9;
        } else if (NuStrCmp(StorePack[10].name, name) == 0) {
            return 10;
        } else {
            return -1;
        }
    }

    if (name == NULL) {
        if (StorePack[0].id != NULL && *StorePack[0].id == id) {
            return 0;
        } else if (StorePack[1].id != NULL && *StorePack[1].id == id) {
            return 1;
        } else if (StorePack[2].id != NULL && *StorePack[2].id == id) {
            return 2;
        } else if (StorePack[3].id != NULL && *StorePack[3].id == id) {
            return 3;
        } else if (StorePack[4].id != NULL && *StorePack[4].id == id) {
            return 4;
        } else if (StorePack[5].id != NULL && *StorePack[5].id == id) {
            return 5;
        } else if (StorePack[6].id != NULL && *StorePack[6].id == id) {
            return 6;
        } else if (StorePack[7].id != NULL && *StorePack[7].id == id) {
            return 7;
        } else if (StorePack[8].id != NULL && *StorePack[8].id == id) {
            return 8;
        } else if (StorePack[9].id != NULL && *StorePack[9].id == id) {
            return 9;
        } else if (StorePack[10].id != NULL && *StorePack[10].id == id) {
            return 10;
        } else {
            return -1;
        }
    }

    if ((StorePack[0].id != NULL && *StorePack[0].id == id) || NuStrCmp(StorePack[0].name, name) == 0) {
        return 0;
    } else if ((StorePack[1].id != NULL && *StorePack[1].id == id) || NuStrCmp(StorePack[1].name, name) == 0) {
        return 1;
    } else if ((StorePack[2].id != NULL && *StorePack[2].id == id) || NuStrCmp(StorePack[2].name, name) == 0) {
        return 2;
    } else if ((StorePack[3].id != NULL && *StorePack[3].id == id) || NuStrCmp(StorePack[3].name, name) == 0) {
        return 3;
    } else if ((StorePack[4].id != NULL && *StorePack[4].id == id) || NuStrCmp(StorePack[4].name, name) == 0) {
        return 4;
    } else if ((StorePack[5].id != NULL && *StorePack[5].id == id) || NuStrCmp(StorePack[5].name, name) == 0) {
        return 5;
    } else if ((StorePack[6].id != NULL && *StorePack[6].id == id) || NuStrCmp(StorePack[6].name, name) == 0) {
        return 6;
    } else if ((StorePack[7].id != NULL && *StorePack[7].id == id) || NuStrCmp(StorePack[7].name, name) == 0) {
        return 7;
    } else if ((StorePack[8].id != NULL && *StorePack[8].id == id) || NuStrCmp(StorePack[8].name, name) == 0) {
        return 8;
    } else if ((StorePack[9].id != NULL && *StorePack[9].id == id) || NuStrCmp(StorePack[9].name, name) == 0) {
        return 9;
    } else if ((StorePack[10].id != NULL && *StorePack[10].id == id) || NuStrCmp(StorePack[10].name, name) == 0) {
        return 10;
    } else {
        return -1;
    }
}

void Store_UnlockPack(i32, bool) {
}

extern AREADATA *VADER_ADATA;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern void StoreProgressAICharacter(LEVEL_PROGRESS_s *);
extern void Grabber_StoreProgress(WORLDINFO_s *, LEVEL_PROGRESS_s *);
extern void GizmoSysStoreProgress(GIZMOSYS_s *, void *, i32);
extern void GizFlowStoreProgress(GIZFLOW_s *, GIZFLOWPROGRESS_s *);
extern void StoreSceneProgress(NUGSCN *, SCENEPROGRESS_s *, i32);
extern void GameAnimSys_StoreProgress(GAMEANIMSYS_s *, i32);

void StoreLevelProgressFn(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress, i32 area_progress) {
    i32 index;
    if (area_progress != 0) {
        if (world == NULL || world->area == NULL)
            return;
        index = world->area->level_count;
    } else {
        if (VADER_ADATA != NULL && VADER_ADATA == WORLD->area)
            return;
        if (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area && bonus_gunship_store_progress_flag == 0)
            return;
        if (world == NULL)
            return;
        index = (i8)world->current_level->area_level_index;
        if (world->area != NULL && (world->area->flags & 4) != 0)
            goto store_flags;
    }
    StoreProgressAICharacter(progress);
    Grabber_StoreProgress(world, progress);
    GizmoSysStoreProgress(world->gizmo_sys, world, index);
    if (progress != NULL) {
        GizFlowStoreProgress(world->giz_flow, &progress->giz_flow_progress);
        StoreSceneProgress(world->current_gscn, reinterpret_cast<SCENEPROGRESS_s *>(progress), 0);
    }
    GameAnimSys_StoreProgress(world->game_anim_sys, index);
    for (i32 i = 0; i < world->processor_count; ++i) {
        if (progress == NULL || NuStrLen(world->processors[i].name) == 0)
            continue;
        for (i32 j = 0; j < 32; ++j) {
            if (NuStrLen(progress->scripts[j].name) == 0) {
                NuStrCpy(world->level_progress->scripts[j].name, world->processors[i].name);
                for (i32 k = 0; k < 4; ++k)
                    progress->scripts[j].params[k] = world->processors[i].processor.params[k];
                break;
            }
            if (NuStrICmp(progress->scripts[j].name, world->processors[i].name) == 0) {
                memcpy(progress->scripts[j].params, world->processors[i].processor.params, 16);
                break;
            }
        }
    }
store_flags:
    if (progress != NULL) {
        if (world->level_progress != progress)
            memmove(progress->disabled_effect_names, world->level_progress->disabled_effect_names, 0xc0);
        reinterpret_cast<u8 *>(&progress->flags)[0] =
            (reinterpret_cast<u8 *>(&progress->flags)[0] & ~4) | ((world->field_0x5174 & 1) << 2) | 2;
    }
}

bool Store_IsPackUnlocked(i32) {
    return 1;
}

bool Store_IsPackAvailable(i32, char *reason) {
    if (reason != NULL) {
        *reason = '\0';
    }
    if (memcard_autosavestarted != 0 || memcard_autosavepostdelay > 0.0f) {
        return false;
    }
    return memcard_autosavepredelay <= 0.0f;
}

void StoreBundle_FindByName(char *) {
}

void Store_RestorePurchases() {
}

void Store_RootPackCustodian(i32, GameObject_s *) {
}

void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress) {
    if (progress == NULL)
        return;
    progress->disabled_ai_object_mask[0] = 0;
    progress->disabled_ai_object_mask[1] = 0;
    GameObject_s *object = Obj;
    i32 count = HIGHGAMEOBJECT;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0 &&
            object->ai.reset_mode == 4) {
            u64 bit = (u64)1 << i;
            progress->disabled_ai_object_mask[0] |= (u32)bit;
            progress->disabled_ai_object_mask[1] |= (u32)(bit >> 32);
        }
    }
}

void Store_HubDrawFloorTargets(WORLDINFO_s *) {
}

void Store_HubInitFloorTargets(WORLDINFO_s *) {
}

void Store_UprootPackCustodian(i32, GameObject_s *) {
}

void StoreStatusTakeOverObjectSys() {
}

static __used__ void StoreUnlockEp2() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[1].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(1);
        }
    }
}
static __used__ void StoreUnlockEp3() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[2].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(2);
        }
    }
}
static __used__ void StoreUnlockEp4() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[3].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(3);
        }
    }
}
static __used__ void StoreUnlockEp5() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[4].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(4);
        }
    }
}
static __used__ void StoreUnlockEp6() {
    if (Game_AreaSave != NULL) {
        if (EDataList != NULL) {
            Game_AreaSave[EDataList[5].area_ids[0]].complete = 1;
        }
        if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
            TurnEpisodeDoorLightsOn(5);
        }
    }
}
static __used__ void StoreUnlockJedi() {
}
static __used__ void StoreUnlockSith() {
}
static __used__ void StoreUnlockBonus() {
}
static __used__ void StoreUnlockArcade() {
}
static __used__ void StoreUnlockBounty() {
}
static __used__ void StoreUnlockChallenge() {
}
