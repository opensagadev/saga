#include "decomp.h"
#include "batman.h"
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/object/takeoverobjects.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legogame/game.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numusic/numusic.h"

i32 openlevels = 0;

static void AddToModelList(APICHARACTERMODELLIST_s *list, i32 *count, i32 capacity, i32 character_id, i32 load_model,
                           EXTRAMODEL *extra_models) {
    if (*count < capacity && InModelList(list, character_id, NULL) == 0) {
        list[*count].count = static_cast<i16>(load_model);
        list[*count].model_id = static_cast<i16>(character_id);
        ++*count;
        list[*count].model_id = -1;
    }

    if (extra_models != NULL && list != reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_PlayerModelList) &&
        load_model != 0) {
        for (; extra_models->model_list != NULL; ++extra_models) {
            if (*extra_models->model_list == character_id) {
                const i32 extra_id = *static_cast<i16 *>(extra_models->field_04);
                if (extra_id != -1 && *count < capacity && InModelList(list, extra_id, NULL) == 0) {
                    list[*count].model_id = static_cast<i16>(extra_id);
                    list[*count].count = static_cast<i16>(load_model);
                    ++*count;
                    list[*count].model_id = -1;
                }
                break;
            }
        }
    }
}

void Areas_OpenAll(i32 mode) {
    i32 area_index;

    if (mode != 0 && !Store_IsPackUnlocked(STORE_PACK_CHALLENGE)) {
        return;
    }

    for (area_index = 0; area_index < AREACOUNT; area_index++) {
        if ((ADataList[area_index].flags & AREAFLAG_MINIKIT) != 0) {
            if ((ADataList[area_index].episode_index == AREA_EPISODE_II && !Store_IsPackUnlocked(STORE_PACK_EPISODE_II)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_III && !Store_IsPackUnlocked(STORE_PACK_EPISODE_III)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_IV && !Store_IsPackUnlocked(STORE_PACK_EPISODE_IV)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_V && !Store_IsPackUnlocked(STORE_PACK_EPISODE_V)) ||
                (ADataList[area_index].episode_index == AREA_EPISODE_VI && !Store_IsPackUnlocked(STORE_PACK_EPISODE_VI))) {
                continue;
            }
        } else if ((ADataList[area_index].flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_SUPER_BONUS_AREA)) == AREAFLAG_BONUS_AREA &&
                   ADataList[area_index].episode_index != AREA_EPISODE_NONE && !Store_IsPackUnlocked(STORE_PACK_ARCADE)) {
            continue;
        }

        if (mode != 0) {
            Game.area_save[area_index].reserved_0x7 = 1;
        } else {
            Game.area_save[area_index].complete = 1;
            Game.area_save[area_index].area_complete = 1;
            if (ADataList[area_index].challenge_trial_time != 0) {
                Game.area_save[area_index].challenge_trial_time =
                    static_cast<f32>(static_cast<u32>(ADataList[area_index].challenge_trial_time)) - 0.5f;
            }
        }
    }

    Game.field_0x2[1] = 1;
    if (WORLD != NULL && WORLD->current_level == HUB_LDATA) {
        Hub_LockUnlockDoors(WORLD);
    }
    ReCalculateCompletionPoints();
}

AREADATA *Areas_ConfigureList(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, i32 count, i32 *countDest) {
    nufpar_s *fp = NuFParCreate(file);
    if (fp == NULL) {
        if (countDest != NULL)
            *countDest = 0;
        return NULL;
    }

    i32 area_count = 0;
    i32 in_area = 0;
    AREADATA *area = (AREADATA *)ALIGN((usize)bufferStart->void_ptr, 4);
    bufferStart->void_ptr = area;
    AREADATA *area_base = area;

    while (NuFParGetLine(fp)) {
        NuFParGetWord(fp);
        char *word = fp->word_buf;
        if (*word == '\0')
            continue;

        if (in_area) {
            if (NuStrICmp(word, "area_end") == 0) {
                in_area = 0;
                if (area->dir[0] != '\0' && area->file[0] != '\0' && area->level_count != 0 &&
                    (area->flags & AREAFLAG_TEST_AREA) == 0) {
                    area++;
                    area_count++;
                }
            } else if (NuStrICmp(fp->word_buf, "dir") == 0) {
                if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0x3f)
                    NuStrCpy(area->dir, fp->word_buf);
                in_area = 1;
            } else if (NuStrICmp(fp->word_buf, "file") == 0) {
                if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) <= 0x1f)
                    NuStrCpy(area->file, fp->word_buf);
                in_area = 1;
            } else if (NuStrICmp(fp->word_buf, "level") == 0) {
                if (area->level_count > 0xb || NuFParGetWord(fp) == 0) {
                    in_area = 1;
                } else {
                    i32 li;
                    Level_FindByName(fp->word_buf, &li);
                    in_area = 1;
                    if (li != -1) {
                        in_area = area->level_count;
                        if (in_area == 0) {
                            area->levels[0] = (i16)li;
                            area->level_count = 1;
                        } else {
                            i32 k;
                            if (area->levels[0] != li) {
                                for (k = 1; k < in_area; k++) {
                                    if (area->levels[k] == li)
                                        break;
                                }
                                if (k == in_area) {
                                    area->levels[in_area] = (i16)li;
                                    area->level_count = (u8)(in_area + 1);
                                }
                            }
                        }
                        in_area = 1;
                    }
                }
            } else if (NuStrICmp(fp->word_buf, "single_buffer") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_SINGLE_BUFFER;
            } else if (NuStrICmp(fp->word_buf, "minikit") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_MINIKIT;
                if (NuFParGetWord(fp) != 0)
                    area->minikit_id = CharIDFromName(fp->word_buf);
            } else if (NuStrICmp(fp->word_buf, "true_jedi") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_TRUE_JEDI;
            } else if (NuStrICmp(fp->word_buf, "test_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_TEST_AREA;
            } else if (NuStrICmp(fp->word_buf, "hub_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_HUB_AREA;
            } else if (NuStrICmp(fp->word_buf, "override_things_scene") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_OVERRIDE_THINGS_SCENE;
            } else if (NuStrICmp(fp->word_buf, "vehicle_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_VEHICLE_AREA;
            } else if (NuStrICmp(fp->word_buf, "ending_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_ENDING_AREA;
            } else if (NuStrICmp(fp->word_buf, "bonus_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_BONUS_AREA;
            } else if (NuStrICmp(fp->word_buf, "super_bonus_area") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_SUPER_BONUS_AREA;
            } else if (NuStrICmp(fp->word_buf, "nocharactercollision") == 0 ||
                       NuStrICmp(fp->word_buf, "nocharactercollisions") == 0 ||
                       NuStrICmp(fp->word_buf, "no_character_collision") == 0 ||
                       NuStrICmp(fp->word_buf, "no_character_collisions") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_NO_CHARACTER_COLLISION;
            } else if (NuStrICmp(fp->word_buf, "nopickupgravity") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_NOPICKUPGRAVITY;
            } else if (NuStrICmp(fp->word_buf, "no_gold_brick") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_NO_GOLDBRICK;
            } else if (NuStrICmp(fp->word_buf, "no_completion_points") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_NO_COMPLETION_POINTS;
            } else if (NuStrICmp(fp->word_buf, "no_freeplay") == 0) {
                in_area = 1;
                area->flags |= AREAFLAG_NO_FREEPLAY;
            } else if (NuStrICmp(fp->word_buf, "name_id") == 0) {
                in_area = 1;
                area->name_id = NuFParGetInt(fp);
            } else if (NuStrICmp(fp->word_buf, "text_id") == 0) {
                area->text_id = NuFParGetInt(fp);
                in_area = 1;
                if (NuFParGetWord(fp) != 0)
                    area->text_id_value = (byte)abs(NuAToI(fp->word_buf));
            } else if (NuStrICmp(fp->word_buf, "timetrial_time") == 0) {
                in_area = 1;
                area->challenge_trial_time = NuFParGetInt(fp);
            } else if (NuStrICmp(fp->word_buf, "redbrick_cheat") == 0 ||
                       NuStrICmp(fp->word_buf, "redbrick_extra") == 0) {
                if (NuFParGetWord(fp) == 0) {
                    in_area = 1;
                } else {
                    area->cheat = Cheat_FindByName(fp->word_buf);
                    in_area = 1;
                }
            }
        } else {
            if (NuStrICmp(word, "area_start") != 0 || count <= area_count)
                continue;
            in_area = 1;
            area->dir[0] = '\0';
            area->file[0] = '\0';
            area->levels[0] = -1;
            area->name_id = 0xffff;
            area->flags = AREAFLAG_NONE;
            area->index = (u8)area_count;
            area->level_count = 0;
            area->cheat = 0xff;
            area->super_counter_count = 0;
            area->super_counters = NULL;
            area->challenge_trial_time = 0;
            area->episode_index = 0xff;
            area->area_index = 0xff;
            area->area_music = -1;
            area->minikit_id = 0xffff;
            area->true_hero_targets[0] = 0;
            area->true_hero_targets[1] = 0;
            area->text_id = 0xffff;
            area->text_id_value = 1;
            area->hub_player_ids = NULL;
        }
    }

    NuFParDestroy(fp);
    if (area_count != 0) {
        bufferStart->void_ptr = area;
        if (countDest != NULL)
            *countDest = area_count;
        i32 j = 0;
        if (0 < area_count) {
            do {
                while (true) {
                    if (area_base[j].challenge_trial_time == 0) {
                        if ((area_base[j].flags & AREAFLAG_SUPER_BONUS_AREA) == AREAFLAG_BONUS_AREA)
                            area_base[j].challenge_trial_time = (i16)AREA_DEFAULTBONUSTIMETRIALTIME;
                        else if ((area_base[j].flags & AREAFLAG_MINIKIT) != 0)
                            area_base[j].challenge_trial_time = (i16)AREA_DEFAULTCHALLENGETIME;
                    }
                    if (area_base[j].cheat != 0xff)
                        Cheat_SetArea((i32)(char)area_base[j].cheat, j);
                    if (area_base[j].challenge_trial_time != 0 &&
                        (area_base[j].flags & (AREAFLAG_SUPER_BONUS_AREA | AREAFLAG_MINIKIT)) == AREAFLAG_MINIKIT)
                        break;
                    j++;
                    if (area_count <= j)
                        return area_base;
                }
                area_base[j].challenge_trial_time = 1200;
                j++;
            } while (j < area_count);
            return area_base;
        }
    }
    return NULL;
}

void Areas_ConfigureResidents(VARIPTR *buffer, VARIPTR *) {
    if (ADataList == NULL) {
        return;
    }

    buffer->addr = ALIGN(buffer->addr, 4);
    AREADATA *area = ADataList;
    for (i32 area_index = 0; area_index < AREACOUNT; ++area_index, ++area) {
        area->hub_player_ids = NULL;

        if ((area->flags & AREAFLAG_ENDING_AREA) != 0) {
            continue;
        }

        char path[0x100];
        NuStrCpy(path, "levels\\");
        NuStrCat(path, area->dir);
        NuStrCat(path, "\\");
        NuStrCat(path, area->file);
        NuStrCat(path, ".txt");

        NUFPAR *parser = NuFParCreate(path);
        if (parser == NULL) {
            continue;
        }

        i32 resident_count = 0;
        while (NuFParGetLine(parser) != 0) {
            if (NuFParGetWord(parser) == 0) {
                continue;
            }
            if (NuStrICmp(parser->word_buf, "character") != 0) {
                continue;
            }
            if (NuFParGetWord(parser) == 0) {
                continue;
            }
            i32 character_id = CharIDFromName(parser->word_buf);
            if (character_id == -1) {
                continue;
            }
            if (NuFParGetWord(parser) == 0) {
                continue;
            }
            if (NuStrICmp(parser->word_buf, "resident") != 0) {
                continue;
            }

            if (area->hub_player_ids == NULL) {
                area->hub_player_ids = buffer->i16_ptr;
            }
            area->hub_player_ids[resident_count++] = static_cast<i16>(character_id);
        }
        NuFParDestroy(parser);

        if (resident_count != 0) {
            area->hub_player_ids[resident_count] = -1;
            buffer->i16_ptr = &area->hub_player_ids[resident_count + 1];
        }
    }
    buffer->addr = ALIGN(buffer->addr, 4);
}

AREADATA *Area_FindByName(char *name, i32 *indexDest) {
    for (i32 i = 0; i < AREACOUNT; i++) {
        if (NuStrICmp(ADataList[i].file, name) == 0) {
            if (indexDest != NULL) {
                *indexDest = i;
            }
            return &ADataList[i];
        }
    }

    if (indexDest != NULL) {
        *indexDest = -1;
    }

    return NULL;
}

void Areas_FixUp(AREAFIXUP *fixup) {
    if (fixup != NULL) {
        for (AREAFIXUP *f = fixup; f->name != NULL; f++) {
            if (f->area != NULL) {
                *f->area = Area_FindByName(f->name, NULL);
            }
        }
    }
}

struct LEVELDATA_s *Area_FindStatusLevel(AREADATA *area, i32 *indexDest) {
    if (indexDest != NULL) {
        *indexDest = -1;
    }

    if (area == NULL || area->level_count == 0) {
        return NULL;
    }

    for (i32 i = 0; i < area->level_count; i++) {
        i32 levelIdx = area->levels[i];
        LEVELDATA *level = &LDataList[levelIdx];
        if (level->flags & LEVEL_STATUS) {
            if (indexDest != NULL) {
                *indexDest = levelIdx;
            }
            return level;
        }
    }

    return NULL;
}

LEVELDATA *Area_FindNextPlayLevel(i32 levelIdx) {
    LEVELDATA *level = &LDataList[levelIdx];
    i32 areaIdx = level->area_index;
    i32 areaLevelIdx = level->area_level_index;

    if (areaIdx != -1) {
        AREADATA *area = &ADataList[areaIdx];
        if (areaLevelIdx < area->level_count - 1) {
            LEVELDATA *result = &LDataList[area->levels[areaLevelIdx]];
            if ((result->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
                return result;
            }
            while (areaLevelIdx != area->level_count - 2) {
                result = &LDataList[area->levels[areaLevelIdx + 1]];
                ++areaLevelIdx;
                if ((result->flags & (LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == 0) {
                    return result;
                }
            }
        }
    }
    return level;
}

void SuperCounters_Reset(i32 area_index) {
    if (area_index != -1) {
        AREADATA *area = &ADataList[area_index];
        SUPERCOUNTER *super_counters = area->super_counters;
        if (super_counters != NULL) {
            i32 i = 0;
            while (area->super_counter_count > i) {
                super_counters[i].collected_count = 0;
                ++i;
            }
        }
    }
}

void SuperCounters_FixUpGizmos(WORLDINFO_s *world) {
    if (world->area == NULL || world->area->super_counters == NULL) {
        return;
    }
    for (i32 i = 0; i < world->area->super_counter_count; ++i) {
        SUPERCOUNTERPICKUP *pickup = world->area->super_counters[i].pickups;
        for (i32 j = 0; j < world->area->super_counters[i].pickup_count; ++j, ++pickup) {
            pickup->gizmo = GizmoFindByName(world->gizmo_sys, -1, pickup->name);
            pickup->position_gizmo = NULL;
            memset(&pickup->position_special, 0, sizeof(pickup->position_special));
            if (pickup->position_name[0] != '\0') {
                if (pickup->use_special == 0) {
                    pickup->position_gizmo = GizmoFindByName(world->gizmo_sys, -1, pickup->position_name);
                } else {
                    NuSpecialFind(world->current_gscn, &pickup->position_special, pickup->position_name, 1);
                }
            }
        }
    }
}

SUPERCOUNTER *SuperCounters_FindPickup(WORLDINFO_s *world, GIZMO_s *gizmo, nuvec_s *position,
                                       SUPERCOUNTERPICKUP **pickup_dest) {
    if (world->area != NULL && world->area->super_counters != NULL) {
        SUPERCOUNTER *nearest_counter = NULL;
        SUPERCOUNTERPICKUP *nearest_pickup = NULL;
        f32 nearest_distance = 1000000000.0f;
        SUPERCOUNTER *counter = world->area->super_counters;
        for (i32 i = 0; i < world->area->super_counter_count; ++i, ++counter) {
            SUPERCOUNTERPICKUP *pickup = counter->pickups;
            for (i32 j = 0; j < counter->pickup_count; ++j, ++pickup) {
                if (pickup->level_index != world->level_idx || pickup->gizmo != gizmo) {
                    continue;
                }
                if (pickup->position_gizmo == NULL) {
                    if (pickup_dest != NULL) {
                        *pickup_dest = pickup;
                    }
                    return counter;
                }
                NUVEC *pickup_position = GizmoGetPos(world->gizmo_sys, pickup->position_gizmo);
                if (pickup_position != NULL) {
                    f32 distance = NuVecDistSqr(position, pickup_position, NULL);
                    if (distance < nearest_distance) {
                        nearest_counter = counter;
                        nearest_pickup = pickup;
                        nearest_distance = distance;
                    }
                }
            }
        }
        if (pickup_dest != NULL) {
            *pickup_dest = nearest_pickup;
        }
        return nearest_counter;
    }
    if (pickup_dest != NULL) {
        *pickup_dest = NULL;
    }
    return NULL;
}

void SuperCounter_ActivateGizmoPickup(GIZMO_s *gizmo, GIZMOPICKUP_s *gizmo_pickup) {
    NUVEC position;
    if (Players_AveragePos(&position, NULL) == 0) {
        position = GameCam->pos;
    }
    SUPERCOUNTERPICKUP *pickup;
    SUPERCOUNTER *counter = SuperCounters_FindPickup(WORLD, gizmo, &position, &pickup);
    if (counter != NULL) {
        gizmo_pickup->state_flags &= ~(GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_VISIBLE);
        if (counter->collected_count < counter->pickup_count) {
            if (++counter->collected_count == counter->pickup_count) {
                gizmo_pickup->state_flags |= GIZMOPICKUP_STATE_ENABLED | GIZMOPICKUP_STATE_VISIBLE;
            }
            NUVEC *message_position;
            if (pickup->position_gizmo != NULL) {
                message_position = GizmoGetPos(WORLD->gizmo_sys, pickup->position_gizmo);
            } else if (NuSpecialExistsFn(&pickup->position_special)) {
                message_position = NuSpecialGetDrawPos(&pickup->position_special);
            } else {
                message_position = &gizmo_pickup->position;
            }
            AddGameMsgCount(message_position, counter->collected_count, counter->pickup_count, counter->red, counter->green,
                            counter->blue, 0.75f);
            GameAudio_PlaySfx(0x53, NULL, 0, 0);
        }
    }
}

SUPERCOUNTER *SuperCounter_FindFromNameAndLevel(char *name, WORLDINFO_s *world, SUPERCOUNTERPICKUP **pickup_dest) {
    if (world->area != NULL && world->area->super_counters != NULL) {
        SUPERCOUNTER *counter = world->area->super_counters;
        for (i32 i = 0; i < world->area->super_counter_count; ++i, ++counter) {
            SUPERCOUNTERPICKUP *pickup = counter->pickups;
            for (i32 j = 0; j < counter->pickup_count; ++j, ++pickup) {
                if (pickup->level_index == world->level_idx && NuStrICmp(pickup->name, name) == 0) {
                    if (pickup_dest != NULL) {
                        *pickup_dest = pickup;
                    }
                    return counter;
                }
            }
        }
    }
    if (pickup_dest != NULL) {
        *pickup_dest = NULL;
    }
    return NULL;
}

i32 SuperCounter_AnyCollected(SUPERCOUNTER *counter, WORLDINFO_s *world) {
    SUPERCOUNTERPICKUP *pickup = counter->pickups;
    for (i32 i = 0; i < counter->pickup_count; ++i, ++pickup) {
        if (world->level_sub_id != -1) {
            for (i32 j = 0; j < AreaGlobals.values.field_0x10; ++j) {
                if (NewMiniPiece[j].level == pickup->level_index &&
                    NuStrICmp(NewMiniPiece[j].name, pickup->name) == 0) {
                    return 1;
                }
            }
        }
        if (Game_LevelSave != NULL) {
            LEVELSAVE_s *save = &reinterpret_cast<LEVELSAVE_s *>(Game_LevelSave)[pickup->level_index];
            for (i32 j = 0; j < save->minikit_count; ++j) {
                if (NuStrICmp(save->minikit_names[j], pickup->name) == 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void SuperCounters_ResetProcessed(WORLDINFO_s *world) {
    if (world->area != NULL && world->area->super_counters != NULL && world->area->super_counter_count != 0) {
        i32 i = 0;
        do {
            world->area->super_counters[i].processed_flags &= ~2;
        } while (world->area->super_counter_count > ++i);
    }
}

void Area_Configure(i32 area, i32 param, EXTRAMODEL *models, i16 *s) {
    Area_PlayerModelCount = 0;
    Area_StoryModelCount = 0;
    Area_PlayerModelList[0] = -1;
    Area_StoryModelList[0].model_id = -1;
    Area_FreePlayModelCount = 0;
    Area_FreePlayModelList[0] = -1;
    Area_MissionModelCount = 0;
    Area_MissionModelList[0].model_id = -1;
    LevelLoad[0].level = -1;
    LevelLoadCount = 0;

    i32 area_music = -1;
    if (area != -1) {
        area_music = ADataList[area].area_music;
        ADataList[area].super_counters = NULL;
        ADataList[area].super_counter_count = 0;
    }
    AreaMusic = area_music;

    if (Mission_Active(MissionSys) != NULL && MissionSys->character_count != 0) {
        i16 mission_characters[64];
        const i32 count = MissionSys->character_count;
        memmove(mission_characters, MissionSys->character_ids, count * sizeof(i16));
        if (count > 2) {
            for (i32 swap = 0; swap < count * 3; ++swap) {
                const i32 first = qrand() / (0xffff / MissionSys->character_count + 1);
                const i32 second =
                    (qrand() / (0xffff / (MissionSys->character_count - 1) + 1) + first) % MissionSys->character_count;
                const i16 character = mission_characters[first];
                mission_characters[first] = mission_characters[second];
                mission_characters[second] = character;
            }
        }
        APICHARACTERMODELLIST_s *player_models = reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_PlayerModelList);
        for (i32 index = 0; index < MissionSys->character_count; ++index) {
            if (Area_MissionModelCount < 0x30)
                Area_MissionModelList[Area_MissionModelCount++] = {mission_characters[index], 1};
            if (Area_PlayerModelCount < 8)
                player_models[Area_PlayerModelCount++] = {mission_characters[index], 1};
        }
        if (Area_MissionModelCount < 0x30)
            Area_MissionModelList[Area_MissionModelCount++] = {MissionSys->mission->find_char, 1};
        Area_MissionModelList[Area_MissionModelCount].model_id = -1;
    }

    char path[128];
    NuStrCpy(path, "levels\\");
    if (area == -1) {
        NuStrCat(path, LDataList[param].dir);
        NuStrCat(path, "\\");
        NuStrCat(path, LDataList[param].name);
    } else {
        NuStrCat(path, ADataList[area].dir);
        NuStrCat(path, "\\");
        NuStrCat(path, ADataList[area].file);
    }
    NuStrCat(path, ".txt");

    NUFPAR *fp = NuFParCreate(path);
    if (fp == NULL) {
        Area_PlayerModelList[0] = s[0];
        Area_PlayerModelList[1] = 1;
        Area_PlayerModelList[2] = s[1];
        Area_PlayerModelList[3] = 1;
        Area_PlayerModelList[4] = -1;
        Area_PlayerIDList[0] = s[0];
        Area_PlayerIDList[1] = s[1];
        Area_PlayerIDList[2] = -1;
        Area_PlayerModelCount = 2;
        Area_StoryModelList[0] = {s[0], 1};
        Area_StoryModelList[1] = {s[1], 1};
        Area_StoryModelList[2].model_id = -1;
        Area_StoryModelCount = 2;
        return;
    }

    SUPERCOUNTER counters[10];
    SUPERCOUNTER *counter = NULL;
    i32 counter_count = 0;
    bool in_counter = false;
    while (NuFParGetLine(fp) != 0) {
        NuFParGetWord(fp);
        if (fp->word_buf[0] == '\0') {
            continue;
        }
        if (in_counter) {
            if (NuStrICmp(fp->word_buf, "supercounter_end") == 0) {
                if (counter->pickup_count != 0) {
                    ++counter_count;
                    in_counter = false;
                }
            } else if (NuStrICmp(fp->word_buf, "pickup") == 0) {
                if (counter->pickup_count >= 10 || NuFParGetWord(fp) == 0 || NuStrLen(fp->word_buf) > 7)
                    continue;
                SUPERCOUNTERPICKUP *pickup = &counter->pickups[counter->pickup_count];
                NuStrCpy(pickup->name, fp->word_buf);
                pickup->level_index = -1;
                pickup->position_name[0] = '\0';
                pickup->use_special = 1;
                while (NuFParGetWord(fp) != 0) {
                    if (NuStrICmp(fp->word_buf, "in_level") == 0) {
                        i32 level_index;
                        if (NuFParGetWord(fp) != 0 && Level_FindByName(fp->word_buf, &level_index) != NULL)
                            pickup->level_index = static_cast<i16>(level_index);
                    } else if (NuStrICmp(fp->word_buf, "draw_at_gizmo") == 0) {
                        if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) < 16) {
                            NuStrCpy(pickup->position_name, fp->word_buf);
                            pickup->use_special = 0;
                        }
                    } else if (NuStrICmp(fp->word_buf, "draw_at_obj") == 0) {
                        if (NuFParGetWord(fp) != 0 && NuStrLen(fp->word_buf) < 16) {
                            NuStrCpy(pickup->position_name, fp->word_buf);
                            pickup->use_special = 1;
                        }
                    }
                }
                if (pickup->level_index != -1)
                    ++counter->pickup_count;
            } else if (NuStrICmp(fp->word_buf, "colour") == 0) {
                counter->red = static_cast<u8>(NuFParGetInt(fp));
                counter->green = static_cast<u8>(NuFParGetInt(fp));
                counter->blue = static_cast<u8>(NuFParGetInt(fp));
            } else if (NuStrICmp(fp->word_buf, "all_together") == 0) {
                counter->processed_flags |= 1;
            }
        } else {
            if (NuStrICmp(fp->word_buf, "supercounter_start") == 0) {
                if (counter_count < 10) {
                    counter = &counters[counter_count];
                    // Runtime pointer fields must not inherit uninitialized stack
                    // bytes when these definitions are copied into the arena.
                    memset(counter, 0, sizeof(*counter));
                    counter->red = counter->green = counter->blue = 0xff;
                    in_counter = true;
                }
                continue;
            }
            if (NuStrICmp(fp->word_buf, "character") == 0) {
                if (NuFParGetWord(fp) == 0) {
                    continue;
                }
                const i32 character_id = CharIDFromName(fp->word_buf);
                if (character_id == -1 || NuFParGetWord(fp) == 0) {
                    continue;
                }

                if (NuStrICmp(fp->word_buf, "player") == 0) {
                    if (Area_MissionModelCount == 0) {
                        AddToModelList(reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_PlayerModelList),
                                       &Area_PlayerModelCount, 8, character_id, 1, models);
                    }
                    AddToModelList(Area_StoryModelList, &Area_StoryModelCount, 0x30, character_id, 1, models);
                } else if (NuStrICmp(fp->word_buf, "resident") == 0) {
                    AddToModelList(Area_StoryModelList, &Area_StoryModelCount, 0x30, character_id, 1, models);
                    AddToModelList(reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_FreePlayModelList),
                                   &Area_FreePlayModelCount, 0x30, character_id, 1, models);
                    AddToModelList(Area_MissionModelList, &Area_MissionModelCount, 0x30, character_id, 1, models);
                } else if (NuStrICmp(fp->word_buf, "cutscene") == 0) {
                    AddToModelList(Area_StoryModelList, &Area_StoryModelCount, 0x30, character_id, 0, models);
                }
                continue;
            }
            if (NuStrICmp(fp->word_buf, "streaming") == 0) {
                if (LevelLoadCount >= 12 || NuFParGetWord(fp) == 0)
                    continue;
                LEVELLOAD_s *load = &LevelLoad[LevelLoadCount];
                bool story_token = false;
                if (NuStrICmp(fp->word_buf, "story_only") == 0) {
                    load->flags = (load->flags & ~2) | 1;
                    story_token = NuFParGetWord(fp) != 0;
                }
                if (NuStrICmp(fp->word_buf, "freeplay_only") == 0) {
                    load->flags = (load->flags & ~1) | 2;
                    if (NuFParGetWord(fp) == 0 && !story_token)
                        continue;
                } else {
                    // The reference applies both bits after a lone story_only
                    // token; retain that sequential token-processing behavior.
                    load->flags |= 3;
                }
                i32 level_index;
                Level_FindByName(fp->word_buf, &level_index);
                if (level_index == -1 || LDataList[level_index].area_index != area)
                    continue;
                i32 existing = 0;
                while (existing < LevelLoadCount && LevelLoad[existing].level != level_index)
                    ++existing;
                if (existing != LevelLoadCount)
                    continue;
                load->level = static_cast<i16>(level_index);
                if (NuFParGetWord(fp) == 0)
                    continue;
                Level_FindByName(fp->word_buf, &level_index);
                if (level_index == -1 || LDataList[level_index].area_index != area)
                    continue;
                load->first_level = static_cast<i16>(level_index);
                load->second_level = -1;
                if (NuFParGetWord(fp) != 0) {
                    Level_FindByName(fp->word_buf, &level_index);
                    if (level_index != -1 && LDataList[level_index].area_index == area)
                        load->second_level = static_cast<i16>(level_index);
                }
                if (load->second_level == -1)
                    load->second_level = load->first_level;
                ++LevelLoadCount;
                continue;
            }
            if (area != -1 && NuStrICmp(fp->word_buf, "music") == 0) {
                if (NuFParGetWord(fp) != 0) {
                    AREADATA *area_data = &ADataList[area];
                    area_data->area_music = GetMusicIndex(fp->word_buf, MusicInfo, -1);
                    const i32 quiet = music_man.GetTrackHandle(TRACK_CLASS_QUIET, fp->word_buf);
                    const i32 action = music_man.GetTrackHandle(TRACK_CLASS_ACTION, fp->word_buf);
                    const i32 silence = music_man.GetTrackHandle(TRACK_CLASS_NOMUSIC, fp->word_buf);
                    for (i32 index = 0; index < area_data->level_count; ++index) {
                        const i32 level_index = area_data->levels[index];
                        LEVELDATA *level = &LDataList[level_index];
                        if ((level->flags & 0xe2) == 2) {
                            level->music_index = area_data->area_music;
                            level->unknown_0a8 = area_data->area_music;
                            level->music_tracks[0][0] = quiet;
                            level->music_tracks[1][0] = action;
                            level->music_tracks[2][0] = silence;
                            level->music_tracks[0][1] = quiet;
                            level->music_tracks[1][1] = action;
                            level->music_tracks[2][1] = silence;
                        }
                    }
                }
                continue;
            }
            if (area != -1 && NuStrICmp(fp->word_buf, "story_coins") == 0) {
                ADataList[area].true_hero_targets[0] = NuFParGetInt(fp);
                if (g_lowEndLevelBehaviour != 0 && &ADataList[area] == DOGFIGHT_ADATA)
                    ADataList[area].true_hero_targets[0] = 40000;
                continue;
            }
            if (area != -1 && NuStrICmp(fp->word_buf, "freeplay_coins") == 0) {
                ADataList[area].true_hero_targets[1] = NuFParGetInt(fp);
                if (g_lowEndLevelBehaviour != 0 && &ADataList[area] == DOGFIGHT_ADATA)
                    ADataList[area].true_hero_targets[1] = 40000;
                continue;
            }
            if (area != -1 && NuStrICmp(fp->word_buf, "timetrial_time") == 0) {
                ADataList[area].challenge_trial_time = static_cast<u16>(NuFParGetInt(fp));
                continue;
            }
            if (NuStrICmp(fp->word_buf, "AIMessage") == 0) {
                if (NuFParGetWord(fp) != 0) {
                    GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, fp->word_buf, NULL);
                    if (message != NULL) {
                        message->flags |= 1;
                        while (NuFParGetWord(fp) != 0) {
                            char *output = NuStrIStr(fp->word_buf, "output");
                            if (output != NULL) {
                                i32 index = NuAToI(output + 6);
                                if (NuFParGetWord(fp) != 0 && static_cast<u32>(index) < 8) {
                                    message->output_values[index] = static_cast<i8>(NuAToI(fp->word_buf));
                                    if (index >= message->output_count)
                                        message->output_count = static_cast<i8>(index + 1);
                                }
                            }
                        }
                    }
                }
                continue;
            }
        }
    }
    NuFParDestroy(fp);

    APICHARACTERMODELLIST_s *player_models = reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_PlayerModelList);
    if (Area_PlayerModelCount == 0) {
        i16 player_id = Area_StoryModelList[0].model_id;
        if (player_id == -1) {
            player_id = reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_FreePlayModelList)[0].model_id;
        }
        if (player_id == -1) {
            player_id = s[0];
        }
        if (player_id != -1) {
            player_models[0] = {player_id, 0};
            player_models[1] = {player_id, 0};
            Area_PlayerModelCount = 2;
        }
    } else if (Area_PlayerModelCount == 1) {
        player_models[1] = {player_models[0].model_id, 1};
        Area_PlayerModelCount = 2;
    }

    player_models[Area_PlayerModelCount].model_id = -1;
    Area_StoryModelList[Area_StoryModelCount].model_id = -1;
    reinterpret_cast<APICHARACTERMODELLIST_s *>(Area_FreePlayModelList)[Area_FreePlayModelCount].model_id = -1;
    Area_MissionModelList[Area_MissionModelCount].model_id = -1;
    for (i32 i = 0; i < Area_PlayerModelCount; ++i) {
        Area_PlayerIDList[i] = player_models[i].model_id;
    }
    Area_PlayerIDList[Area_PlayerModelCount] = -1;
    if (counter_count != 0 && area != -1) {
        const usize destination = ALIGN(characterbuffer_ptr.addr, alignof(SUPERCOUNTER));
        const usize bytes = counter_count * sizeof(SUPERCOUNTER);
        if (destination <= characterbuffer_end.addr && bytes <= characterbuffer_end.addr - destination) {
            ADataList[area].super_counters = reinterpret_cast<SUPERCOUNTER *>(destination);
            memmove(ADataList[area].super_counters, counters, bytes);
            characterbuffer_ptr.addr = destination + bytes;
            ADataList[area].super_counter_count = static_cast<u8>(counter_count);
        }
    }
}

void ClearUpAreaData() {
    i32 object_index;

    if (HIGHGAMEOBJECT > 0) {
        for (object_index = 0; object_index < HIGHGAMEOBJECT; object_index++) {
            GameObject_s *obj = &Obj[object_index];
            if ((obj->apiobj.field_0x1f8 & 1) != 0) {
                FreeTorpedoPacket((TORPEDOPACKET_s **)&obj->torpedo);
                RemoveGameObject(&Obj[object_index], 1);
            }
        }
    }
    if (Area != -1 && Area == last_area) {
        return;
    }
    if (Customiser_AccessoriesLoaded == 2) {
        Customiser_RestoreModelTextureIDs(CharacterCustomiser);
    } else if (Customiser_AccessoriesLoaded == 1) {
        Customiser_DumpAccessories(CharacterCustomiser);
    }
    Customiser_AccessoriesLoaded = 0;
    APIDumpCharacterModels(0);
    IconScenes_Dump();
    CharScenes_AreaDump();
    if (big_icon_scene != NULL) {
        NuGScnRemove(big_icon_scene);
    }
    big_icon_scene = NULL;
    if (area_scene != NULL) {
        NuGScnRemove(area_scene);
    }
    area_scene = NULL;
    if (vehicle_scene != NULL) {
        NuGScnRemove(vehicle_scene);
    }
    vehicle_scene = NULL;
    Particles_DumpAreaPage();
}

void ClearAreaProgress(i32 a, i32 b) {
    for (i32 i = 0; i < 12; i++) {
        ClearLevelProgress(i, NULL);
    }
    areaSuitBits = Game.initial_store_pack_flags;
    if (b == 0 || a == -1) {
        return;
    }
    AreaGlobals.values.field_0x14 = 0;
    Game.area_save[a] = BackupGame.area_save[a];

    AREADATA *area = &ADataList[a];
    for (i32 i = 0; i < area->level_count; i++) {
        i32 level_index = area->levels[i];
        Game.level_records[level_index] = BackupGame.level_records[level_index];
    }
}

void Areas_CompleteAllBuildUps(AREASAVE_s *save) {
    if (save == NULL) {
        return;
    }
    for (i32 i = 0; i < AREACOUNT; ++i) {
        if ((ADataList[i].flags & (AREAFLAG_TRUE_JEDI | AREAFLAG_MINIKIT)) != 0 && save[i].complete != 0) {
            save[i].true_hero_complete[0] = 1;
            save[i].true_hero_complete[1] = 1;
        }
    }
}

void NewArea() {
    i32 i;
    u8 area_ep;

    LSW1 = 0;
    LSW2 = 0;
    if (WORLD != NULL && WORLD->area != NULL) {
        i8 episode = static_cast<i8>(WORLD->area->episode_index);
        if (episode >= 0) {
            if (episode <= 2) {
                LSW1 = 1;
            } else {
                LSW2 = 1;
            }
        }
    }
    HIGHJUMPHEIGHT = (LSW1 != 0 || Arcade != 0) ? 1.14f : 0.75f;
    ClearAreaProgress(Area, 0);
    if (WORLD->level_progress != NULL && (WORLD->level_progress->flags & 1) == 0) {
        WORLD->level_progress->data = WORLD->progress_data;
        WORLD->level_progress->flags |= 1;
    }
    SuperCounters_Reset(Area);
    AreaGlobals.values.field_0x10 = 0;
    if (Area == -1) {
        area_ep = 0;
    } else {
        area_ep = Game.area_save[Area].minikit_count;
    }
    AreaGlobals.values.field_0x0c = area_ep;
    AreaGlobals.values.field_0x14 = area_ep;
    Door_UseCutCam = 0;
    AreaGlobals.values.field_0x00 = 0;
    AreaGlobals.values.field_0x1c = 0;
    AreaGlobals.values.field_0x08 = 0;
    AreaGlobals.values.field_0x18 = 0;
    AreaGlobals.values.arcade_player_kills[0] = 0;
    AreaGlobals.values.arcade_player_kills[1] = 0;
    AreaGlobals.values.arcade_ai_kills[0] = 0;
    AreaGlobals.values.arcade_ai_kills[1] = 0;
    BuildUpTotal = 0;
    BuildUpDone = 0;
    if (WORLD->area == HOTHBATTLE_ADATA) {
        ResetMinikitCounter();
    }
    for (i = 0; i < 8; i++) {
        PlayerProgress[i].hitpoints = DEFAULT_PLAYERHITPOINTS;
        if (Player[i] != NULL) {
            Player[i]->current_hp = Player[i]->hitpoints;
        }
    }
    ResetTimer(&AreaTimer, 0.0f);
    memcpy(&BackupGame, &Game, sizeof(GAMESAVE_s));
    NewAreaMusicChanges();
    VehicleAreaRememberSpeed = 0.0f;
    ClearTakeOverObjectSys();
    BonusScore[0] = OldBonusScore[0];
    BonusScore[1] = OldBonusScore[1];
    BonusCoinTotal = 0;
    Door_Last = NULL;
    Door_Reset();
    LevelChange = 1;
    BombGenerator_PlayerBomb[0] = 0;
    BombGenerator_PlayerBomb[1] = 0;
    Lap = 1;
}
