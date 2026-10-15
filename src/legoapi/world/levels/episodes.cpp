#include "legoapi/world/levels/episode.h"

#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

// Episode list (retail episodes.cpp): parsing the episode list and the
// episode/area completion queries.

EPISODEDATA *EDataList = NULL;

static bool Episode_HasArea(const EPISODEDATA *episode, i32 area_index) {
    for (i32 i = 0; i < episode->area_count; i++) {
        if (episode->area_ids[i] == area_index) {
            return true;
        }
    }
    return false;
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
    i32 in_episode = 0;
    EPISODEDATA *list = (EPISODEDATA *)ALIGN((usize)bufferStart->void_ptr, 4);
    bufferStart->void_ptr = (void *)list;
    EPISODEDATA *episode = list;

    while (NuFParGetLine(fp)) {
        NuFParGetWord(fp);
        if (fp->word_buf[0] == '\0') {
            continue;
        }

        if (in_episode) {
            if (NuStrICmp(fp->word_buf, "episode_end") == 0) {
                in_episode = 0;
                if (episode->area_count != 0) {
                    count++;
                    episode++;
                }
            } else if (NuStrICmp(fp->word_buf, "area") == 0) {
                if (episode->area_count < 10 && NuFParGetWord(fp) != 0) {
                    i32 area_index;
                    AREADATA *area = Area_FindByName(fp->word_buf, &area_index);
                    if (area_index != -1) {
                        if (Episode_HasArea(episode, area_index)) {
                            continue;
                        }
                        // An area may only belong to one episode.
                        i32 j;
                        for (j = 0; j < count; j++) {
                            if (Episode_HasArea(&list[j], area_index)) {
                                break;
                            }
                        }
                        if (j == count) {
                            const i32 area_count = episode->area_count;
                            episode->area_ids[area_count] = (i16)area_index;
                            episode->area_count = (u8)(area_count + 1);
                            if ((area->flags & (AREAFLAG_ENDING_AREA | AREAFLAG_BONUS_AREA)) == 0) {
                                episode->regular_areas++;
                            }
                        }
                    }
                }
            } else if (NuStrICmp(fp->word_buf, "name_id") == 0) {
                episode->name_id = (i16)NuFParGetInt(fp);
            } else if (NuStrICmp(fp->word_buf, "text_id") == 0) {
                episode->text_id = (i16)NuFParGetInt(fp);
            }
        } else if (NuStrICmp(fp->word_buf, "episode_start") == 0 && count < maxCount) {
            episode->name_id = -1;
            episode->text_id = -1;
            episode->area_count = 0;
            episode->index = (u8)count;
            in_episode = 1;
        }
    }

    NuFParDestroy(fp);
    if (count == 0) {
        return NULL;
    }
    bufferStart->void_ptr = (void *)episode;
    if (countDest != NULL) {
        *countDest = count;
    }
    return list;
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

u32 Episode_FindAreaFromFlags(EPISODEDATA *ep, u32 flags, u32 want) {
    for (i32 i = 0; i < (i32)ep->area_count; i++) {
        AREADATA *a = &ADataList[ep->area_ids[i]];
        if ((a->flags & flags) == want) {
            return (u8)a->index;
        }
    }
    return 0xffffffff;
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
    EpCompleteCount = EpCompleteTotal = 0;
    EpMiniKitCount = EpMiniKitTotal = 0;
    EpCharKitCount = EpCharKitTotal = 0;
    EpBuildUpCount = EpBuildUpTotal = 0;
    EpStoryBuildUpCount = EpStoryBuildUpTotal = 0;
    EpFreePlayBuildUpCount = EpFreePlayBuildUpTotal = 0;
    EpRedBrickCount = EpRedBrickTotal = 0;
    EpGoldBrickCount = EpGoldBrickTotal = 0;
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
            if (GOLDBRICKFORSUPERBONUS == 0)
                continue;
            ++EpGoldBrickTotal;
            if (saves[i].area_complete == 0)
                continue;
            ++EpGoldBrickCount;
            continue;
        }
        if ((area->flags & 4) != 0)
            continue;
        ++EpCompleteTotal;
        EpMiniKitTotal += 10;
        AREASAVE_s *save = &saves[id];
        if (save->complete != 0)
            ++open;
        ++EpGoldBrickTotal;
        if (save->area_complete != 0) {
            ++EpCompleteCount;
            ++EpGoldBrickCount;
        }
        if ((area->flags & 0x10) == 0)
            continue;
        ++EpGoldBrickTotal;
        if (save->minikit_complete != 0)
            ++EpGoldBrickCount;
        EpMiniKitCount += save->field_0x5[0];
        if (BOTHTRUEJEDIGOLDBRICKS != 0) {
            ++EpBuildUpTotal;
            ++EpGoldBrickTotal;
            if (save->true_hero_complete[0] != 0) {
                ++EpBuildUpCount;
                ++EpGoldBrickCount;
                ++EpStoryBuildUpCount;
            }
            ++EpBuildUpTotal;
            ++EpGoldBrickTotal;
            if (save->true_hero_complete[1] != 0) {
                ++EpBuildUpCount;
                ++EpGoldBrickCount;
                ++EpFreePlayBuildUpCount;
            }
        } else {
            ++EpBuildUpTotal;
            ++EpGoldBrickTotal;
            if (save->true_hero_complete[0] != 0 || save->true_hero_complete[1] != 0) {
                ++EpBuildUpCount;
                ++EpGoldBrickCount;
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
