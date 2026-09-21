#include "legoapi/world/levels/episode.h"

#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

EPISODEDATA *EDataList = NULL;

struct TROOPERCANNON_s {
    u32 field_0x00;
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

void TrooperShoot(WORLDINFO_s *, minitrooperteam_s *, minisnowtrooper_s *, u16 *, i32) {
    STUBBED();
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

void ResetTrooperCannons(WORLDINFO_s *, i32) {
    STUBBED();
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

void UpdateMiniSnowTroopers(WORLDINFO_s *) {
    STUBBED();
}

static __used__ void seed_chase(f32 *, i32, abi_long) {
    STUBBED();
}
