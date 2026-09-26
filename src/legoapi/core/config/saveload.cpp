#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
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
extern i16 (*memcard_hashfn)(void);

#define SAVELOAD_TARGET_OPT __attribute__((optimize("O2", "omit-frame-pointer")))

void InitMemCard() {
}

SAVELOAD_TARGET_OPT i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *) {
    return 0;
}

SAVELOAD_TARGET_OPT void SerialiseInt(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

void FS_GetDirList(char *, char *, char *) {
    STUBBED();
}

SAVELOAD_TARGET_OPT void SerialiseChar(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 1, 1);
}

void FS_PrevNameLen(char *) {
    STUBBED();
}

void FS_SortStrings(char *, char *, i32) {
    STUBBED();
}

SAVELOAD_TARGET_OPT void SerialiseFloat(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

SAVELOAD_TARGET_OPT void SerialiseNuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

SAVELOAD_TARGET_OPT void SerialiseNuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

SAVELOAD_TARGET_OPT void SerialiseShort(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 1);
}

SAVELOAD_TARGET_OPT void SerialiseVuMtx(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 16);
}

SAVELOAD_TARGET_OPT void SerialiseVuVec(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void FS_MoveCursorUp(i32) {
    STUBBED();
}

SAVELOAD_TARGET_OPT void SerialiseString(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char *>(data), count);
}

SAVELOAD_TARGET_OPT void SerialiseColour3(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

void FS_FileNameFilter(char *) {
    STUBBED();
}

SAVELOAD_TARGET_OPT void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d", date[4] - 'A', date[5] - 'A', date[6] + 1915);
}

SAVELOAD_TARGET_OPT void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d:%.2d:%.2d", date[3] - 'A', date[2] - 'A', date[1] - 'A');
}

void FS_MoveCursorDown(i32) {
    STUBBED();
}

void FS_GetDirTextWidth() {
    STUBBED();
}

void FS_GetFilterString(char *, char *) {
    STUBBED();
}

SAVELOAD_TARGET_OPT i32 getsaveload_status() {
    return saveload_status;
}

SAVELOAD_TARGET_OPT i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed) {
    static i32 LastPad;
    static float PadRepeat;

    if (pad->digital_buttons != LastPad) {
        LastPad = pad->digital_buttons;
        PadRepeat = 0.5f;
        return LastPad;
    }

    PadRepeat -= elapsed;
    if (PadRepeat >= 0.0f) {
        return 0;
    }
    PadRepeat += repeat;
    return LastPad;
}

SAVELOAD_TARGET_OPT void SerialiseNuHSpecial(EdStream &stream, void *data, i32) {
    nuhspecial_s *special = static_cast<nuhspecial_s *>(data);
    if (stream.mode == 2)
        stream.SerialiseString(NuSpecialGetName(special), 0);

    if (stream.mode == 1) {
        char name[128];
        stream.SerialiseString(name, 128);
        NuSpecialFind(NULL, special, name, 0);
        theLevelEditor.GetScene(0);
        for (i32 index = 0; index < theLevelEditor.editable_scene_count; ++index) {
            if (NuSpecialFind(theLevelEditor.GetScene(index), special, name, 0))
                break;
        }
    }
}

SAVELOAD_TARGET_OPT void SerialiseStringAddr(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char **>(data), count);
}

void FS_BuildFilterBlocks(char *) {
    STUBBED();
}

SAVELOAD_TARGET_OPT void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d %.2d:%.2d:%.2d", date[4] - 'A', date[5] - 'A', date[6] + 1915,
            date[3] - 'A', date[2] - 'A', date[1] - 'A');
}

void FS_BuildFilterOutBlocks(char *) {
    STUBBED();
}

void FS_SetCursorToLastFileName() {
    STUBBED();
}

SAVELOAD_TARGET_OPT i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    return 0;
}

extern "C" {

    void (*savesuccessfn)(void);
    void *memcard_headerdata;
    i32 memcard_headerdatasize;
    void *memcard_headerdatabuffer;

    void FS_SetFileSelPathFromName(void) {
        STUBBED();
    }

    SAVELOAD_TARGET_OPT void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize,
                                                   void *header, i32 headerSize, i32 autosave,
                                                   void (*drawSaveIcon)(void)) {
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

    SAVELOAD_TARGET_OPT void SetSaveSuccessFn(void (*callback)(void)) {
        savesuccessfn = callback;
    }

    void UpdateSaveSlots(void) {
        const f32 elapsed = NuTimeGetFrameTime();
        if (memcard_loadmessage_delay > 0.0f) {
            memcard_loadmessage_delay -= elapsed;
        }
        if (memcard_loadresult_delay > 0.0f) {
            memcard_loadresult_delay -= elapsed;
        }
        if (memcard_savemessage_delay > 0.0f) {
            memcard_savemessage_delay -= elapsed;
        }
        if (memcard_saveresult_delay > 0.0f) {
            memcard_saveresult_delay -= elapsed;
        }

        if (memcard_savestarted != 0 && saveload_status == 1) {
            memcard_savestarted = 0;
            MenuSaveOccurred = 1;
        }

        if (memcard_saveneeded != 0 && saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0 &&
            memcard_savedata != NULL && memcard_savedatabuffer != NULL) {
            const i32 save_hash = memcard_hashfn != NULL ? memcard_hashfn() : -1;

            if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL) {
                memmove(memcard_extra_savedatabuffer, memcard_extra_savedata, memcard_extra_savedatasize);
                const i32 checksum = ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                static_cast<i32 *>(memcard_extra_savedatabuffer)[memcard_extra_savedatasize / sizeof(i32)] = checksum;
                saveloadASSave(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4, -1);
            }

            memmove(memcard_savedatabuffer, memcard_savedata, memcard_savedatasize);
            const i32 checksum = ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize);
            static_cast<i32 *>(memcard_savedatabuffer)[memcard_savedatasize / sizeof(i32)] = checksum;
            saveloadASSave(memcard_slot, memcard_savedatabuffer, memcard_savedatasize + 4, save_hash);

            memcard_saveneeded = 0;
            memcard_savestarted = 1;
            memcard_savefailed = 0;
        }

        if (memcard_loadstarted != 0 && saveload_status == 1) {
            memcard_loadstarted = 0;
            memcard_loadcorrupt = 0;

            const i32 stored_checksum = static_cast<i32 *>(memcard_savedatabuffer)[memcard_savedatasize / sizeof(i32)];
            const i32 computed_checksum = ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize);
            if (computed_checksum != stored_checksum) {
                memcard_loadcorrupt = 1;
                saveload_autosave = -1;
            }

            if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL) {
                const i32 stored_extra_checksum =
                    static_cast<i32 *>(memcard_extra_savedatabuffer)[memcard_extra_savedatasize / sizeof(i32)];
                const i32 computed_extra_checksum =
                    ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                if (computed_extra_checksum != stored_extra_checksum) {
                    memcard_loadcorrupt = 1;
                    saveload_autosave = -1;
                }
            }

            if (memcard_loadcorrupt == 0) {
                memmove(memcard_savedata, memcard_savedatabuffer, memcard_savedatasize);
                if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL) {
                    memmove(memcard_extra_savedata, memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                }
                MenuLoadOccurred = 1;
            }
        }

        if (memcard_loadneeded == 0 || saveload_status != 1 || saveload_cardtype != 2 || saveload_cardformatted == 0 ||
            memcard_savedata == NULL || memcard_savedatabuffer == NULL) {
            return;
        }

        if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL) {
            saveloadASLoad(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4);
        }
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

    void loadsaveCallEachFrame(void) {
        saveloadASCallEachFrame();
        UpdateSaveSlots();
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
