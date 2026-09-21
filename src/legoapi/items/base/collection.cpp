#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/characters/motion.h"

u32 GizmoBlowups_TotalScore(void *world);
extern i32 DoubleScore;

#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/core/gamehint.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include <stdlib.h>
#include "legoapi/characters/motion.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
struct GIZMOPICKUP_s;
struct PART_s;
struct starfighter_s;

struct APICHARACTERMODELLIST_s;

COLLECTID *TempCollectID = NULL;

static i32 CollectCount;
static COLLECTID *CollectList;
i32 COLLECTION_COMPLETIONCOUNT = 0;

i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count) {
    i32 i;
    COLLECTID *p;

    TempCollectID = NULL;

    if (list == NULL) {
        list = CollectList;
        count = CollectCount;
        if (list == NULL)
            return id;
    }

    p = list;
    for (i = 0; i < count; i++, p++) {
        if (p->id == id) {
            TempCollectID = p;
            return i;
        }
    }

    return -1;
}

i32 Collection_Got(i32 id) {
    if (InCollectList_Index(id, NULL, 0) == -1) {
        return 0;
    }

    i32 area = AreaFromMiniKitID(id);
    if (area != -1) {
        if (Game_AreaSave == NULL) {
            return 0;
        }
        return Game_AreaSave[area].minikit_complete >= SAVE_COMPLETE ? 2 : 0;
    }

    if (Game_CharacterSave != NULL && (Game_CharacterSave[id] & SAVE_CHARACTER_AVAILABLE) == 0) {
        return 0;
    }
    return 1;
}

void Collection_Configure(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd) {
    byte bVar1;
    ushort uVar2;
    i16 sVar3;
    COLLECTID *collect;
    i32 iVar4;
    AREADATA *pAVar5;
    i32 i;
    char *buf;

    nufpar_s *fp = NuFParCreate(file);
    if (fp != NULL) {
        CollectCount = 0;
        collect = (COLLECTID *)ALIGN(bufferStart->addr, 4);
        CollectList = collect;
        bufferStart->void_ptr = collect;

        COLLECTION_COMPLETIONCOUNT = 0;

        while (NuFParGetLine(fp) != 0) {

        LAB_004eb7f3:
            if (NuFParGetWord(fp) != 0 && NuStrICmp(fp->word_buf, "collect") == 0 && NuFParGetWord(fp) != 0) {
                sVar3 = CharIDFromName(fp->word_buf);

                LOG_DEBUG("Collection_Configure: Found collect id %s -> %d", fp->word_buf, sVar3);

                collect->id = sVar3;

                if (sVar3 != -1 && InCollectList_Index((i32)sVar3, CollectList, CollectCount) == -1) {
                    collect->type = 0;
                    collect->field2_0x3 = 0xff;
                    collect->can_buy = 0;
                    collect->field3_0x4 = 0;
                    collect->field6_0xa = 0;
                    collect->field5_0x9 = 0;
                    collect->cheat_code[0] = '\0';

                LAB_004eb886:
                    iVar4 = NuFParGetWord(fp);

                    do {
                        if (iVar4 == 0) {
                            bVar1 = collect->type;
                            if (bVar1 == 0) {
                                if (collect->can_buy != 0) {
                                LAB_004eb8ce:
                                    collect->field5_0x9 = 1;
                                    COLLECTION_COMPLETIONCOUNT = COLLECTION_COMPLETIONCOUNT + 1;
                                }
                            } else if (bVar1 != 8 && bVar1 != 7)
                                goto LAB_004eb8ce;

                            CollectCount = CollectCount + 1;
                            iVar4 = NuFParGetLine(fp);
                            collect = collect + 1;

                            if (iVar4 == 0)
                                goto LAB_004eb900;

                            goto LAB_004eb7f3;
                        }

                        iVar4 = NuStrICmp(fp->word_buf, "story");
                        if (iVar4 != 0)
                            goto LAB_004eb920;

                        collect->type = 1;
                        iVar4 = NuFParGetWord(fp);

                    } while (true);
                }
            }
        }

    LAB_004eb900:
        NuFParDestroy(fp);
        if (CollectCount < 1) {
            CollectList = NULL;
            return;
        }
        bufferStart->void_ptr = collect;
    }

    return;

LAB_004eb920:
    iVar4 = NuStrICmp(fp->word_buf, "area_complete");
    if (iVar4 == 0) {
        iVar4 = NuFParGetWord(fp);
        if (iVar4 != 0 && (pAVar5 = Area_FindByName(fp->word_buf, &i), pAVar5 != NULL)) {
            collect->type = 2;
            collect->field2_0x3 = (byte)i;
        }
    } else {
        iVar4 = NuStrICmp(fp->word_buf, "all_episodes_complete");
        if (iVar4 == 0) {
            collect->type = 3;
        } else {
            iVar4 = NuStrICmp(fp->word_buf, "in_pack");
            if (iVar4 == 0) {
                iVar4 = NuFParGetWord(fp);
                if ((iVar4 != 0) && (iVar4 = Store_FindPack(-1, fp->word_buf), iVar4 != -1)) {
                    collect->type = 8;
                    collect->field2_0x3 = (byte)iVar4;
                }
            } else {
                iVar4 = NuStrICmp(fp->word_buf, "100_percent");
                if (iVar4 == 0) {
                    collect->type = 7;
                } else {
                    iVar4 = NuStrICmp(fp->word_buf, "gold_bricks");
                    if (iVar4 == 0) {
                        collect->type = 6;
                        iVar4 = NuFParGetInt(fp);
                        uVar2 = (ushort)(iVar4 >> 31);
                        collect->field6_0xa = ((ushort)iVar4 ^ uVar2) - uVar2;
                    } else {
                        iVar4 = NuStrICmp(fp->word_buf, "all_minikits_complete");
                        if (iVar4 == 0) {
                            collect->type = 4;
                        } else {
                            iVar4 = NuStrICmp(fp->word_buf, "minikit");
                            if (iVar4 == 0) {
                                collect->type = 5;
                            } else {
                                iVar4 = NuStrICmp(fp->word_buf, "buy_in_shop");
                                if (iVar4 == 0) {
                                    collect->can_buy = 1;
                                    iVar4 = NuFParGetInt(fp);
                                    collect->field3_0x4 = iVar4;
                                } else {
                                    iVar4 = NuStrICmp(fp->word_buf, "cheat_code");
                                    if (((iVar4 == 0) && (iVar4 = NuFParGetWord(fp), iVar4 != 0)) &&
                                        (iVar4 = NuStrLen(fp->word_buf), iVar4 == 6)) {
                                        buf = collect->cheat_code;
                                        NuStrCpy(buf, fp->word_buf);
                                        NuStrUpr(buf, buf);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    goto LAB_004eb886;
}

f32 COLLECTION_DX = 0.15f;
f32 COLLECTION_DY = -0.2f;
f32 COLLECTION_ICONSIZE = 0.16f;
i32 LEGOOBJ_ICON_FRAME_NEUTRAL = -1;
i32 LEGOOBJ_ICON_FRAME_BLUE = -1;
i32 LEGOOBJ_ICON_FRAME_GREEN = -1;
nuhspecial_s *collection_draw_hspecial;
i32 (*collection_draw_IsValidFn)(COLLECTION_s *, i32);
void (*Collection_GetSelectingPlayerIDsFn)(i16 *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern FadeSystem FadeSys;

void Collection_Draw(COLLECTION_s *collection, float x, float y, float scale, APICHARACTERMODELLIST_s *models,
                     float alpha, i32 hide_selected) {
    nuhspecial_s *special = collection_draw_hspecial;
    i32 (*valid)(COLLECTION_s *, i32) = collection_draw_IsValidFn;
    collection_draw_hspecial = NULL;
    collection_draw_IsValidFn = NULL;
    if (collection->list == NULL || FadeSys.fade > 0.0f)
        return;
    const u32 count = collection->count_y;
    const u32 columns = collection->count_x;
    if (count == 0 || columns == 0)
        return;
    f32 dx = COLLECTION_DX * scale;
    f32 size = COLLECTION_ICONSIZE * scale;
    const u32 rows = count / columns + (count % columns != 0);
    if (Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0) {
        dx *= 0.75f;
        size *= 0.875f;
    }
    i32 selected_x[2] = {-1, -1};
    i32 selected_y[2] = {-1, -1};
    if (models != NULL && Collection_GetSelectingPlayerIDsFn != NULL) {
        i16 ids[2] = {-1, -1};
        Collection_GetSelectingPlayerIDsFn(ids);
        if (ids[0] != -1 || ids[1] != -1) {
            i32 found = 0;
            for (u32 row = 0; row < rows; ++row)
                for (u32 col = 0; col < columns; ++col) {
                    const u32 index = row * columns + col;
                    if (found != 2 && index < count &&
                        (collection->list[index].id == ids[0] || collection->list[index].id == ids[1])) {
                        selected_x[found] = col;
                        selected_y[found++] = row;
                    }
                }
        }
    }
    const f32 dy = COLLECTION_DY * scale;
    collection->field_14 = dy;
    if (alpha > 1.0f)
        alpha = 1.0f;
    f32 py = y - static_cast<i32>(rows - 1) * dy * 0.5f;
    for (u32 row = 0; row < rows; ++row) {
        f32 px = x - static_cast<i32>(columns - 1) * dx * 0.5f;
        for (u32 col = 0; col < columns; ++col, px += dx) {
            const u32 index = row * columns + col;
            if (index >= count)
                continue;
            COLLECTID *entry = &collection->list[index];
            i32 id = entry->id;
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(entry) + 0x14) = px;
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(entry) + 0x18) = py;
            if (alpha <= 0.0f)
                continue;
            i32 frame = LEGOOBJ_ICON_FRAME_NEUTRAL;
            f32 opacity = 1.0f;
            i32 model_index;
            if (models != NULL && InModelList(models, id, &model_index) != 0) {
                if (hide_selected != 0)
                    continue;
                opacity = 0.25f;
                if (model_index == 0 || model_index == 1) {
                    frame = model_index == 0 ? LEGOOBJ_ICON_FRAME_BLUE : LEGOOBJ_ICON_FRAME_GREEN;
                    opacity =
                        NuTrigTable[(static_cast<i32>(GlobalTimer.time_elapsed_mod_seconds * 65536.0f) >> 1) & 0x7fff] *
                            0.125f +
                        0.375f;
                }
            }
            const i32 unlocked = valid != NULL ? valid(collection, index) : Collection_Got(id);
            if (unlocked == 0) {
                opacity *= 0.25f;
                id = -1;
            }
            opacity *= alpha;
            if (opacity <= 0.0f)
                continue;
            u32 neighbours = 0;
            for (i32 player = 0; player < 2; ++player) {
                if (selected_x[player] == -1 || selected_y[player] == -1)
                    continue;
                const i32 ax = abs(static_cast<i32>(col) - selected_x[player]);
                const i32 ay = abs(static_cast<i32>(row) - selected_y[player]);
                if ((ax == 0 && ay == 1) || (ax == 1 && ay == 0))
                    neighbours |= 1;
                else if (ax == 1 && ay == 1)
                    neighbours |= 2;
                else if ((ax == 0 && ay == 2) || (ax == 2 && ay == 0))
                    neighbours |= 4;
            }
            if ((neighbours & 1) != 0)
                opacity *= 0.333f;
            else if ((neighbours & 2) != 0)
                opacity *= 0.5f;
            else if ((neighbours & 4) != 0)
                opacity *= 0.666f;
            if (special == NULL)
                DrawCharIcon(id, px, py, 0.002f, size, frame, opacity, opacity, 1, NULL);
            else {
                drawcharicon_hspecial_spin =
                    static_cast<i32>((NuFmod(GameTimer.time_elapsed, 3.0f) / 3.0f) * 65536.0f) + index * 0x1555;
                DrawCharIcon(-1, px, py, 0.002f, size, frame, opacity, opacity, 1, unlocked != 0 ? special : NULL);
            }
        }
        py += dy;
    }
}

void Collection_GetPos(COLLECTION_s *collection, i32 id, float *x, float *y) {
    COLLECTID *list = collection->list;
    i32 count = collection->count_y;

    for (i32 i = 0; i < count; i++) {
        if (list[i].id == id) {
            float *f = (float *)&list[i].cheat_code[8];
            *x = f[0];
            *y = f[1];
            return;
        }
    }
}

i32 Collection_GetIDList(COLLECTION_s *collection, u32 model_flag_mask, u32 required_model_flags, i16 *ids,
                         i32 *first_id, i32 *second_id, i32 unused) {
    (void)unused;
    if (first_id != NULL) {
        *first_id = -1;
    }
    if (second_id != NULL) {
        *second_id = -1;
    }

    i32 result_count = 0;
    for (i32 index = 0; index < collection->count_y; ++index) {
        const i16 id = collection->list[index].id;
        if ((CDataList[id].model_flags & model_flag_mask) != required_model_flags || Collection_Got(id) == 0) {
            continue;
        }

        if (ids != NULL) {
            ids[result_count] = id;
        }
        ++result_count;

        if (first_id != NULL && *first_id == -1) {
            *first_id = id;
        } else if (first_id != NULL && second_id != NULL && *second_id == -1) {
            *second_id = id;
        }
    }

    if (ids != NULL) {
        ids[result_count] = -1;
    }
    return result_count;
}

void Collection_CreateMaster(char *file, i16 *idlist, COLLECTION_s *collection, i32 param4, float param5) {
    collection->list = CollectList;
    collection->count_x = (u16)param4;
    collection->count_y = (u16)CollectCount;
    collection->field_8 = idlist;
    collection->field_c = file;
    collection->field_10 = param5;
}

void Collection_CreateCustom(char *name, i16 *id_list, COLLECTION_s *collection, u32 required_model_flags,
                             u32 excluded_model_flags, u32 required_game_flags, i32 require_buyable, i32 columns,
                             VARIPTR *buffer, VARIPTR *, i32 use_all_characters, f32 scale) {
    collection->count_x = static_cast<u16>(columns);
    collection->count_y = 0;
    collection->field_8 = id_list;
    collection->field_c = name;
    collection->field_10 = scale;

    buffer->addr = ALIGN(buffer->addr, 4);
    collection->list = reinterpret_cast<COLLECTID *>(buffer->void_ptr);

    if (use_all_characters == 0) {
        for (i32 index = 0; index < CollectCount; ++index) {
            COLLECTID &source = CollectList[index];
            const i32 id = source.id;
            if (id < 0) {
                continue;
            }
            if (excluded_model_flags != 0 && (CDataList[id].model_flags & excluded_model_flags) != 0) {
                continue;
            }
            if (require_buyable != 0 && source.can_buy == 0) {
                continue;
            }
            if (required_game_flags != 0 && (GCDataList[id].flags_090 & required_game_flags) != required_game_flags) {
                continue;
            }
            if (required_model_flags != 0 &&
                (CDataList[id].model_flags & required_model_flags) != required_model_flags) {
                continue;
            }
            collection->list[collection->count_y++] = source;
        }
    } else {
        for (i32 id = 0; id < CHARCOUNT; ++id) {
            if (required_model_flags != 0 &&
                (CDataList[id].model_flags & required_model_flags) != required_model_flags) {
                continue;
            }
            if (excluded_model_flags != 0 && (CDataList[id].model_flags & excluded_model_flags) != 0) {
                continue;
            }
            if (required_game_flags != 0 && (GCDataList[id].flags_090 & required_game_flags) != required_game_flags) {
                continue;
            }
            COLLECTID &entry = collection->list[collection->count_y++];
            memset(&entry, 0, sizeof(entry));
            entry.id = static_cast<i16>(id);
        }
    }

    buffer->addr += static_cast<usize>(collection->count_y) * sizeof(COLLECTID);
}

COLLECTID *CollectIDUnlocked(i32 id) {
    i32 index = InCollectList_Index(id, CollectList, CollectCount);
    if (index == -1) {
        return NULL;
    }

    COLLECTID *entry = &CollectList[index];
    if (Game_CharacterSave != NULL && (Game_CharacterSave[id] & SAVE_CHARACTER_UNLOCKED) != 0) {
        return entry;
    }

    switch (entry->type) {
        case 0:
            return entry;
        case 2:
            if (static_cast<i8>(entry->field2_0x3) == -1 || Game_AreaSave == NULL) {
                return NULL;
            }
            return Game_AreaSave[entry->field2_0x3].area_complete != 0 ? entry : NULL;
        case 3:
            if (Episodes_Completed() != EPISODECOUNT) {
                return NULL;
            }
            return Game_100PercentComplete() != 0 ? entry : NULL;
        case 4:
            return AllMiniKitsDone(Game_AreaSave) != 0 ? entry : NULL;
        case 6:
            if (Game_CompletionSave == NULL ||
                reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->gold_bricks < entry->field6_0xa) {
                return NULL;
            }
            return entry;
        case 7:
            return Game_100PercentComplete() != 0 ? entry : NULL;
        case 8:
            return Store_IsPackUnlocked(static_cast<i8>(entry->field2_0x3)) != 0 ? entry : NULL;
        default:
            return NULL;
    }
}

i32 Collection_GotAnyOfType(i32 type, u32 flags) {
    i32 count;

    if (CollectList == NULL)
        return 0;

    count = CollectCount;
    if (count <= 0)
        return 0;

    for (i32 i = 0; i < count; i++) {
        i32 id = CollectList[i].id;
        if (type == -1) {
            if (flags == 0) {
                if (Collection_Got(id))
                    return 1;
            } else if ((CDataList[id].model_flags & flags) == flags) {
                if (Collection_Got(id))
                    return 1;
            }
        } else {
            if ((signed char)GCDataList[id].field275_0x116 != type)
                continue;
            if (flags == 0) {
                if (Collection_Got(id))
                    return 1;
            } else if ((CDataList[id].model_flags & flags) == flags) {
                if (Collection_Got(id))
                    return 1;
            }
        }
    }

    return 0;
}

void CollectAllCharacters(i32) {
    STUBBED();
}

static __used__ void Collection_GetSelectingPlayerIDs(i16 *) {
    STUBBED();
}

void ReleaseEat(GameObject_s *object) {
    u8 flags = object->field_0xe24;
    GameObject_s *eaten = static_cast<GameObject_s *>(object->field_0x780);
    if (eaten != NULL && (flags & 1) != 0) {
        KillGameObject(eaten, 2, 0);
        flags = object->field_0xe24;
        object->field_0x780 = NULL;
    }
    if (static_cast<u8>(object->field_0x7a5 - 0x38) <= 1) {
        object->field_0x780 = NULL;
        object->field_0x7a5 = 0xff;
    }
    object->field_0xe24 = flags & ~1;
}

void ShipDropCoins(starfighter_s *) {
    STUBBED();
}

i32 AddToCollection(i32 id) {
    if (id > 0 && id < CHARCOUNT && InCollectList_Index(id, NULL, 0) != -1 && Collection_Got(id) == 0) {
        if (Game_CharacterSave != NULL)
            Game_CharacterSave[id] |= SAVE_CHARACTER_AVAILABLE | SAVE_CHARACTER_UNLOCKED;
        return 1;
    }
    return 0;
}

void ResetCoinPacket(COINPACKET_s *packet) {
    if (packet != NULL) {
        packet->scale = 1.0f;
        packet->double_score_timer = 0.0f;
        packet->active = 1;
    }
}

void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index) {
    if (packet == NULL) {
        return;
    }
    if (active == 0) {
        packet->active = 1;
        packet->scale = 1.0f;
        packet->double_score_timer = 0.0f;
        if (BonusArea != 0) {
            if (packet->coins < 10) {
                packet->coins = 0;
                return;
            }
            u32 decrement = static_cast<u32>(static_cast<i32>(20000.0f * FRAMETIME));
            decrement = (decrement / 10) * 10;
            if (decrement < 10) {
                decrement = 10;
            } else if (packet->coins < decrement) {
                packet->coins = 0;
                return;
            }
            packet->coins -= decrement;
        }
        return;
    }

    packet->scale = SeekLinearF(packet->scale, 1.0f, 3.0f * FRAMETIME);
    if (((DoubleScore >> player_index) & 1) == 0) {
        packet->active = 1;
        packet->double_score_timer = 0.0f;
    } else if (packet->double_score_timer > 0.0f) {
        packet->double_score_timer -= FRAMETIME;
        if (packet->double_score_timer <= 0.0f) {
            packet->active = 1;
        }
    }
}

u32 GizmoBlowups_TotalScore(void *);
u32 GizForce_TotalScore(void *);
u32 GizObstacles_TotalScore(void *);
u32 GizTurrets_TotalScore(void *);
u32 GameAI_TotalScore();

u32 TotalLevelCoinTally(WORLDINFO_s *world, u32 *pickups, u32 *blowups, u32 *buildits, u32 *forces, u32 *obstacles,
                        u32 *turrets, u32 *characters) {
    u32 value = GizmoPickups_TotalScore(world);
    u32 total = value;
    if (pickups != NULL)
        *pickups = value;
    value = GizmoBlowups_TotalScore(world);
    total += value;
    if (blowups != NULL)
        *blowups = value;
    value = GizBuildIts_TotalScore(world);
    total += value;
    if (buildits != NULL)
        *buildits = value;
    value = GizForce_TotalScore(world);
    total += value;
    if (forces != NULL)
        *forces = value;
    value = GizObstacles_TotalScore(world);
    total += value;
    if (obstacles != NULL)
        *obstacles = value;
    value = GizTurrets_TotalScore(world);
    total += value;
    if (turrets != NULL)
        *turrets = value;
    value = 0;
    if (world->area != NULL && (world->area->flags & 0x100) != 0)
        value = GameAI_TotalScore();
    total += value;
    if (characters != NULL)
        *characters = value;
    return total;
}

COLLECTION_s *GetFreePlayCollection(i32 area) {
    const u16 area_flags = ADataList[area].flags;
    if ((area_flags & AREAFLAG_VEHICLE_AREA) == 0) {
        return &CharacterCollection;
    }
    if ((area_flags & AREAFLAG_BONUS_AREA) != 0) {
        return &MiniKitCollection;
    }
    return &VehicleCollection;
}

static i32 CompletionPointInfo_ReCalculate[7];
extern i32 POINTS_PER_HINT;
extern i32 SHOPHINTCOUNT;
extern i16 HintTab[24];

void ReCalculateCompletionPoints() {
    Game.completion = 0;
    Game.gold_bricks = 0;
    memset(CompletionPointInfo_ReCalculate, 0, sizeof(CompletionPointInfo_ReCalculate));

    for (i32 index = 0; index < MasterCollection.count_y; ++index) {
        COLLECTID *entry = &MasterCollection.list[index];
        if (entry->field5_0x9 == 0) {
            continue;
        }
        CompletionPointInfo_ReCalculate[2] += POINTS_PER_CHARACTER;
        if (Collection_Got(entry->id) != 0) {
            AddToCompletionPoints(POINTS_PER_CHARACTER);
        }
    }

    for (i32 index = 0; index < AREACOUNT; ++index) {
        AREADATA *area = &ADataList[index];
        AREASAVE_s *save = &Game.area_save[index];
        const u16 flags = area->flags;
        if (area == HUB_ADATA ||
            (flags & (AREAFLAG_ENDING_AREA | AREAFLAG_TEST_AREA | AREAFLAG_NO_COMPLETION_POINTS)) != 0) {
            continue;
        }

        if ((flags & 0x100) != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_SUPERBONUSCOMPLETE;
            if (save->area_complete != 0) {
                AddToCompletionPoints(POINTS_PER_SUPERBONUSCOMPLETE);
                if (GOLDBRICKFORSUPERBONUS != 0) {
                    AddToGoldBricks();
                }
            }
            continue;
        }

        if ((flags & AREAFLAG_BONUS_AREA) != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TIMETRIAL;
            if (save->area_complete != 0 || save->challenge_trial_time < static_cast<f32>(area->challenge_trial_time)) {
                AddToCompletionPoints(POINTS_PER_TIMETRIAL);
                AddToGoldBricks();
            }
            continue;
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_STORY;
        if (save->area_complete != 0) {
            AddToCompletionPoints(POINTS_PER_STORY);
            if ((flags & AREAFLAG_NO_GOLDBRICK) == 0) {
                AddToGoldBricks();
            }
        }

        if ((flags & AREAFLAG_MINIKIT) == 0) {
            if ((flags & AREAFLAG_TRUE_JEDI) != 0 &&
                (save->story_buildup_complete != 0 || save->freeplay_buildup_complete != 0)) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
            continue;
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_MINIKIT;
        if (save->minikit_complete != 0) {
            AddToCompletionPoints(POINTS_PER_MINIKIT);
            AddToGoldBricks();
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_TRUEJEDI;
        if (save->story_buildup_complete != 0) {
            AddToCompletionPoints(POINTS_PER_TRUEJEDI);
            AddToGoldBricks();
        }
        if (BOTHTRUEJEDIGOLDBRICKS != 0) {
            CompletionPointInfo_ReCalculate[1] += POINTS_PER_TRUEJEDI;
            if (save->freeplay_buildup_complete != 0) {
                AddToCompletionPoints(POINTS_PER_TRUEJEDI);
                AddToGoldBricks();
            }
        } else if (save->story_buildup_complete == 0 && save->freeplay_buildup_complete != 0) {
            AddToCompletionPoints(POINTS_PER_TRUEJEDI);
            AddToGoldBricks();
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_REDBRICK;
        if (save->red_brick_collected != 0) {
            AddToCompletionPoints(POINTS_PER_REDBRICK);
        }

        CompletionPointInfo_ReCalculate[1] += POINTS_PER_CHALLENGE;
        if (save->reserved_0x7 != 0) {
            AddToCompletionPoints(POINTS_PER_CHALLENGE);
            if (GOLDBRICKFORCHALLENGE != 0) {
                AddToGoldBricks();
            }
        }
    }

    if (MissionSys != NULL && MissionSys->count != 0) {
        for (i32 index = 0; index < MissionSys->count; ++index) {
            CompletionPointInfo_ReCalculate[3] += POINTS_PER_MISSION;
            if (Game.mission_save.completed[index] != 0) {
                AddToCompletionPoints(POINTS_PER_MISSION);
                AddToGoldBricks();
            }
        }
    }

    for (i32 index = 0; index < EPISODECOUNT; ++index) {
        CompletionPointInfo_ReCalculate[0] += POINTS_PER_SUPERSTORY;
        if ((Game.episode_save[index].flags & SAVE_SUPERSTORY_COMPLETE) != 0) {
            AddToCompletionPoints(POINTS_PER_SUPERSTORY);
            if (GOLDBRICKFORSUPERSTORY != 0) {
                AddToGoldBricks();
            }
        }
    }

    for (i32 index = 0; index < 44; ++index) {
        CompletionPointInfo_ReCalculate[4] += POINTS_PER_CHEAT;
        if ((Game.extra_purchased_bits[index >> 5] & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_CHEAT);
        }
    }

    for (i32 index = 0; index < SHOPHINTCOUNT; ++index) {
        HINT_s *hint = Hint_FindHint(HintTab[index]);
        if (hint == NULL || hint->shop_price == 0) {
            continue;
        }
        CompletionPointInfo_ReCalculate[5] += POINTS_PER_HINT;
        if ((Game.shop_hint_purchased_bits[index >> 5] & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_HINT);
        }
    }

    for (i32 index = 0; index < SHOPGOLDBRICKS; ++index) {
        CompletionPointInfo_ReCalculate[6] += POINTS_PER_GOLDBRICK;
        if ((Game.shop_gold_brick_purchased_bits & (1U << (index & 31))) != 0) {
            AddToCompletionPoints(POINTS_PER_GOLDBRICK);
            AddToGoldBricks();
        }
    }
}
