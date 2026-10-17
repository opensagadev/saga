// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/world/mission.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include <string.h>

extern GameObject_s *FindGameObject(i32, u32, i32, i32, i32);

MISSIONSYS *MissionSys = NULL;

MISSIONSYS *Missions_Configure(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd, MISSIONSAVE *save) {
    nufpar_s *fp;
    i32 i;
    MISSIONSYS *dest;
    i32 charId;
    MISSIONSYS sys;
    MISSIONDATA *buffer;

    fp = NuFParCreate(file);
    if (fp != NULL) {
        memset(&sys, 0, sizeof(sys));

        sys.flags = 1;
        bufferStart->addr = ALIGN(bufferStart->addr, alignof(MISSIONDATA));
        buffer = static_cast<MISSIONDATA *>(bufferStart->void_ptr);
        sys.missions = buffer;
        sys.mission_save = save;

        while (sys.count < 20 && NuFParGetLine(fp) != 0) {
            NuFParGetWord(fp);

            i = NuStrICmp(fp->word_buf, "party");
            if (i == 0) {
                while ((sys.character_count < 8 && (i = NuFParGetWord(fp), i != 0))) {
                    charId = CharIDFromName(fp->word_buf);
                    if (charId != -1) {
                        sys.character_ids[sys.character_count] = (i16)charId;
                        sys.character_count = sys.character_count + 1;
                    }
                }
            } else {
                i = NuStrICmp(fp->word_buf, "pickups");
                if (i == 0) {
                    i = NuFParGetWord(fp);
                    if (i != 0) {
                        i = NuStrICmp(fp->word_buf, "on");
                        if (i == 0) {
                            sys.flags |= 1;
                        } else {
                            i = NuStrICmp(fp->word_buf, "off");
                            if (i == 0) {
                                sys.flags &= 0xfe;
                            }
                        }
                    }
                } else {
                    i = NuStrICmp(fp->word_buf, "mission");
                    if (i == 0) {
                        buffer->find_char = -1;
                        buffer->level = -1;
                        buffer->name_id = -1;
                        buffer->text_id = -1;
                        buffer->bounty = 0x186a0;
                        buffer->bounty2 = 0x186a0;
                        buffer->time = 0xb4;
                        buffer->count = sys.count;
                        while (NuFParGetWord(fp) != 0) {
                            if (NuStrICmp(fp->word_buf, "find") == 0) {
                                if (NuFParGetWord(fp) != 0) {
                                    buffer->find_char = CharIDFromName(fp->word_buf);
                                }
                            } else if (NuStrICmp(fp->word_buf, "in_level") == 0) {
                                if (NuFParGetWord(fp) != 0 && Level_FindByName(fp->word_buf, &charId) != NULL) {
                                    buffer->level = (i16)charId;
                                }
                            } else if (NuStrICmp(fp->word_buf, "time") == 0) {
                                u16 time = (u16)NuFParGetInt(fp);
                                buffer->time = time < 3 ? 3 : time;
                            } else if (NuStrICmp(fp->word_buf, "bounty") == 0) {
                                buffer->bounty = NuFParGetInt(fp);
                                buffer->bounty2 = NuFParGetInt(fp);
                            } else if (NuStrICmp(fp->word_buf, "name_id") == 0) {
                                buffer->name_id = (i16)NuFParGetInt(fp);
                            } else if (NuStrICmp(fp->word_buf, "text_id") == 0) {
                                buffer->text_id = (i16)NuFParGetInt(fp);
                            }
                        }
                        if (buffer->find_char != -1 && buffer->level != -1) {
                            sys.count = sys.count + 1;
                            ++buffer;
                            bufferStart->addr += sizeof(MISSIONDATA);
                        }
                    }
                }
            }
        }

        NuFParDestroy(fp);
        if (sys.count != 0) {
            bufferStart->addr = ALIGN(bufferStart->addr, alignof(MISSIONSYS));
            dest = static_cast<MISSIONSYS *>(bufferStart->void_ptr);
            memmove(dest, &sys, sizeof(sys));
            bufferStart->addr = ALIGN(bufferStart->addr + sizeof(MISSIONSYS), alignof(MISSIONSYS));
            return dest;
        }
    }

    return NULL;
}

void Mission_Clear(MISSIONSYS *ms) {
    if (ms == NULL) {
        ms = MissionSys;
        if (ms == NULL) {
            return;
        }
    }
    ms->mission = NULL;
    ms->field8_0x1d = 0;
    ResetTimer(&ms->timer, 0.0f);
}

MISSIONDATA *Mission_Active(MISSIONSYS *ms) {
    if (ms == NULL) {
        ms = MissionSys;
        if (ms == NULL) {
            return NULL;
        }
    }
    if (ms->field8_0x1d == 0) {
        return NULL;
    }
    return ms->mission;
}

i32 Mission_CurrentState(MISSIONSYS *ms) {
    u8 c;

    if (ms == NULL) {
        ms = MissionSys;
    }
    if (ms == NULL) {
        return 0;
    }
    c = ms->field8_0x1d;
    if (c != 0 && ms->mission != NULL) {
        return (i32)c;
    }
    return 0;
}

i32 Missions_PartyAvailable(MISSIONSYS *ms) {
    i32 i;

    if (ms == NULL) {
        ms = MissionSys;
        if (ms == NULL) {
            return 1;
        }
    }
    for (i = 0; i < ms->character_count; i++) {
        if (Collection_Got(ms->character_ids[i]) == 0) {
            return 0;
        }
    }
    return 1;
}

i32 Missions_NumCompleted(MISSIONSYS *ms, MISSIONSAVE *save, i32 count) {
    i32 total = 0;
    i32 i;

    if (ms == NULL) {
        ms = MissionSys;
    }
    if (ms != NULL) {
        for (i = 0; i < ms->count; i++) {
            if (count != 0) {
                save->completed[i] = 1;
                if (save->best_times[i] == 0.0f) {
                    save->best_times[i] = (f32)ms->missions[i].time - 1.0f;
                }
                total++;
            } else if (save->completed[i] != 0) {
                total++;
            }
        }
    }
    return total;
}

GameObject_s *Mission_FindTarget(MISSIONSYS *ms, u64 *target) {
    GameObject_s *obj;

    if (ms == NULL) {
        ms = MissionSys;
    }
    if (ms == NULL) {
        if (target != NULL) {
            *target = 0;
        }
        return NULL;
    }

    obj = FindGameObject((u32)ms->mission->find_char, 0, 0, 1, 0);
    if (target != NULL) {
        u64 v = 0;
        if (obj != NULL) {
            v = *(u64 *)&obj->apiobj.field_0x1e4;
        }
        *target = v;
    }
    return obj;
}

i32 Missions_PickupsOff(MISSIONSYS *ms) {
    if (ms == NULL) {
        ms = MissionSys;
    }
    if (ms == NULL) {
        return 0;
    }
    if (ms->field8_0x1d == 0) {
        return 0;
    }
    if (ms->mission == NULL) {
        return 0;
    }
    return (ms->flags ^ 1) & 1;
}

