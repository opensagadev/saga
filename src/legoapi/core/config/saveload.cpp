#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 memcard_cardchanged;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
extern i32 numfilteroutblocks;
extern char filteroutblocks[8][12];
extern i32 numfilterblocks;
extern char filterblocks[8][96];

extern char FS_FileList[65536];
extern char FS_LastFileName[64];
extern char FS_Path[256];
extern i32 FS_NumFiles;
extern i32 FS_CurrentPosFileNum;
extern i32 FS_CursorLine;
extern char FS_ParentEntry[8];
extern i32 FS_SortMode;
extern char * FS_CurrentCursorPos;
extern char * FS_CurrentPos;
extern char * FS_FileListEnd;
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

f32 memcard_autosavecanceldelay = 1.5f;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
f32 memcard_createmessage_delay;
f32 memcard_createresult_delay;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
f32 memcard_message_delay;
f32 memcard_result_delay;
i32 memcard_justformatted;
i32 saveload_autosavedisabled;

void InitMemCard() {
}

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *) {
    return 0;
}

void SerialiseInt(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 1, 1);
}

i32 FS_PrevNameLen(char *name);


void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseNuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseNuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void SerialiseShort(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void SerialiseVuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

void SerialiseVuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char *>(data), count);
}

void SerialiseColour3(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

extern char * SortStrings[4];

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status() {
    return saveload_status;
}

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(data);
    if (stream.mode == 2) {
        char *name = NuSpecialGetName(special);
        stream.SerialiseString(name, 0);
    }

    if (stream.mode == 1) {
        char name[128];
        stream.SerialiseString(name, 128);
        NuSpecialFind(NULL, special, name, 0);
        theLevelEditor.GetScene(0);
        for (i32 index = 0; index < theLevelEditor.reset_pending; ++index) {
            if (NuSpecialFind(theLevelEditor.GetScene(index), special, name, 0))
                break;
        }
    }
}

void SerialiseStringAddr(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char **>(data), count);
}

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    return 0;
}

extern "C" {
    void (*savesuccessfn)(void);
    void *memcard_headerdata;
    i32 memcard_headerdatasize;
    void *memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

    void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void)) {
        memcard_hashfn = reinterpret_cast<i16 (*)(void)>(makeSaveHash);
        memcard_savedata = save;
        memcard_savedatasize = saveSize;
        memcard_savedatabuffer = NU_ALLOC(saveSize + 4, 4, 1, "Main", 0);

        memcard_autosave = autosave;
        memcard_drawasiconfn = drawSaveIcon;
        memcard_headerdata = header;
        memcard_headerdatasize = headerSize;
        memcard_headerdatabuffer = NU_ALLOC(headerSize + 4, 4, 1, "Main", 0);

        i32 saveSlots;
        if (header == nullptr) {
            saveSlots = 6;
            if (slots < 7) {
                saveSlots = slots;
            }
        } else {
            saveSlots = 5;
            if (slots < 6) {
                saveSlots = slots;
            }
        }
        SAVESLOTS = saveSlots;
    }

    void SetSaveSuccessFn(void (*callback)(void)) {
        savesuccessfn = callback;
    }

    void UpdateSaveSlots(void) {
        const f32 elapsed = NuTimeGetFrameTime();
        if (memcard_autosavecanceldelay > 0.0f)
            memcard_autosavecanceldelay -= elapsed;
#define ADVANCE_SAVE_DELAY(message, result)                                                                            \
    if (message > 0.0f) {                                                                                              \
        message -= elapsed;                                                                                            \
    } else if (result > 0.0f) {                                                                                        \
        result -= elapsed;                                                                                             \
    }
        ADVANCE_SAVE_DELAY(memcard_loadmessage_delay, memcard_loadresult_delay);
        ADVANCE_SAVE_DELAY(memcard_savemessage_delay, memcard_saveresult_delay);
        if (memcard_autosavepredelay > 0.0f) {
            memcard_autosavepredelay -= elapsed;
        } else if (memcard_autosavepostdelay > 0.0f) {
            memcard_autosavepostdelay -= elapsed;
            if (memcard_autosavepostdelay <= 0.0f)
                memcard_autosaveinprogress = 0;
        }
        ADVANCE_SAVE_DELAY(memcard_formatmessage_delay, memcard_formatresult_delay);
        ADVANCE_SAVE_DELAY(memcard_createmessage_delay, memcard_createresult_delay);
        ADVANCE_SAVE_DELAY(memcard_deletemessage_delay, memcard_deleteresult_delay);
        ADVANCE_SAVE_DELAY(memcard_message_delay, memcard_result_delay);
#undef ADVANCE_SAVE_DELAY

        if (saveload_cardtype != 2)
            memcard_justformatted = 0;
        if (saveload_autosavedisabled != 0) {
            if (memcard_autosavestarted != 0) {
                memcard_savefailed = 1;
                memcard_saveresult_delay = 1.5f;
            }
            memcard_autosavestarted = 0;
            memcard_autosavepredelay = 0.0f;
            memcard_autosaveinprogress = 0;
            memcard_autosaveneeded = 0;
            memcard_autosavepostdelay = 0.0f;
            memcard_autosavedisabled = 1;
            saveload_autosavedisabled = 0;
        }

        if (memcard_loadneeded != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                memcard_loadmessage_delay = 1.5f;
                if (memcard_savedatabuffer == NULL || memcard_savedatasize <= 0) {
                    memcard_loadneeded = 0;
                    memcard_loadstarted = 0;
                    memcard_loadfailed = 1;
                    return;
                }
                if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL)
                    saveloadASLoad(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4);
                saveloadASLoad(memcard_slot, memcard_savedatabuffer, memcard_savedatasize + 4);
                memcard_loadneeded = 0;
                memcard_loadcorrupt = 0;
                if (saveload_status == 1) {
                    memcard_loadstarted = 0;
                    memcard_loadfailed = 1;
                } else {
                    memcard_loadstarted = 1;
                    memcard_loadfailed = 0;
                }
            }
        } else if (memcard_loadstarted != 0 && saveload_status == 1) {
            memcard_loadstarted = 0;
            if (saveload_error == 0 && memcard_savedatabuffer != NULL && memcard_savedatasize >= 0) {
                i32 stored_checksum;
                memcpy(&stored_checksum, static_cast<u8 *>(memcard_savedatabuffer) + memcard_savedatasize, 4);
                if (ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize) != stored_checksum) {
                    saveload_autosave = -1;
                    memcard_loadcorrupt = 1;
                }
                if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                    memcard_extra_savedatasize >= 0) {
                    i32 stored_extra_checksum;
                    memcpy(&stored_extra_checksum,
                           static_cast<u8 *>(memcard_extra_savedatabuffer) + memcard_extra_savedatasize, 4);
                    if (ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize) !=
                        stored_extra_checksum) {
                        memcard_loadcorrupt = 1;
                        saveload_autosave = -1;
                    }
                }
                if (memcard_loadcorrupt == 0 && memcard_savedata != NULL) {
                    memmove(memcard_savedata, memcard_savedatabuffer, memcard_savedatasize);
                    if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                        memcard_extra_savedatasize >= 0)
                        memmove(memcard_extra_savedata, memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                    MenuLoadOccurred = 1;
                }
            } else if (saveload_error == 0x80) {
                memcard_loadcorrupt = 1;
                saveload_autosave = -1;
            } else {
                memcard_loadfailed = 1;
                saveload_autosave = -1;
            }
        }

        if (memcard_autosaveneeded != 0) {
            memcard_autosaveneeded = 0;
            if (saveload_autosave != -1) {
                memcard_autosavepredelay = 1.0f;
                memcard_autosavestarted = 1;
                memcard_autosaveinprogress = 1;
            }
        } else if (memcard_autosavestarted != 0 && memcard_saveneeded == 0 && memcard_savestarted == 0 &&
                   !(memcard_autosavepredelay > 0.0f)) {
            memcard_slot = saveload_autosave;
            memcard_saveneeded = 1;
        }

        if (memcard_saveneeded == 0) {
            if (memcard_savestarted != 0 && saveload_status == 1) {
                memcard_savestarted = 0;
                if (saveload_error == 0) {
                    MenuSaveOccurred = 1;
                    if (savesuccessfn != NULL)
                        savesuccessfn();
                    if (memcard_autosavestarted != 0) {
                        memcard_autosavepostdelay = 1.0f;
                        memcard_autosavestarted = 0;
                    }
                } else {
                    memcard_savefailed = 1;
                    if (saveload_autosave != -1) {
                        memcard_saveresult_delay = 1.5f;
                        saveload_autosave = -1;
                        memcard_autosavedisabled = 1;
                        saveload_autosavedisabled = 1;
                    }
                    memcard_autosavestarted = 0;
                    memcard_autosaveinprogress = 0;
                }
            }
        } else {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                memcard_savemessage_delay = 1.5f;
                if (memcard_savedatabuffer == NULL || memcard_savedata == NULL || memcard_savedatasize <= 0) {
                    memcard_saveneeded = 0;
                    memcard_savestarted = 0;
                    memcard_savefailed = 1;
                    return;
                }
                i32 save_hash = memcard_hashfn != NULL ? memcard_hashfn() : -1;
                if (memcard_slot != -1) {
                    if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                        memcard_extra_savedatasize >= 0) {
                        memmove(memcard_extra_savedatabuffer, memcard_extra_savedata, memcard_extra_savedatasize);
                        i32 checksum = ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                        memcpy(static_cast<u8 *>(memcard_extra_savedatabuffer) + memcard_extra_savedatasize, &checksum,
                               4);
                        saveloadASSave(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4, -1);
                    }
                    memmove(memcard_savedatabuffer, memcard_savedata, memcard_savedatasize);
                    i32 checksum = ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize);
                    memcpy(static_cast<u8 *>(memcard_savedatabuffer) + memcard_savedatasize, &checksum, 4);
                    saveloadASSave(memcard_slot, memcard_savedatabuffer, memcard_savedatasize + 4, save_hash);
                }
                memcard_saveneeded = 0;
                memcard_savestarted = 1;
                memcard_savefailed = 0;
                memcard_justformatted = 0;
            }
        }

        if (memcard_deleteneeded != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                saveloadASDelete(memcard_slot);
                memcard_deleteneeded = 0;
                memcard_deletemessage_delay = 1.5f;
                memcard_deletestarted = 1;
                memcard_deletefailed = 0;
            }
        } else if (memcard_deletestarted != 0 && saveload_status == 1) {
            memcard_deletefailed = saveload_error != 0;
            memcard_deletestarted = 0;
        }
        if (memcard_formatme != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2) {
                saveloadASFormat();
                memcard_formatmessage_delay = 1.5f;
                memcard_formatme = 0;
                memcard_formatting = 1;
                memcard_formatfailed = 0;
                memcard_justformatted = 0;
            }
        } else if (memcard_formatting != 0 && saveload_status == 3) {
            memcard_formatting = 0;
            if (saveload_error == 0)
                memcard_justformatted = 1;
            else
                memcard_formatfailed = 1;
        }
    }

    void loadsaveCallEachFrame(void) {
        saveloadASCallEachFrame();
        if (saveload_cardchanged != 0) {
            memcard_cardchanged = 1;
        }
        UpdateSaveSlots();

        memcard_slotsused = 0;
        if (saveload_savepresent != 0) {
            i32 slots = SAVESLOTS;
            const i32 slot_capacity = sizeof(saveload_slotused) / sizeof(saveload_slotused[0]);
            if (slots > slot_capacity) {
                slots = slot_capacity;
            }
            i32 used = 0;
            for (i32 slot = 0; slot < slots; ++slot) {
                if (saveload_slotused[slot] != 0) {
                    ++used;
                }
            }
            memcard_slotsused = used;
        }
    }

    i32 TriggerAutoSave(void) {
        if (memcard_autosave != 0 && memcard_autosaveenabled != 0 && saveload_autosave != -1) {
            memcard_autosaveneeded = 1;
            MenuSaveOccurred = 0;
            return 1;
        }
        return 0;
    }

} // extern "C"
