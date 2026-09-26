#pragma once

#include <stddef.h>

#include "nu2api/nucore/common.h"

#ifdef __cplusplus
extern "C" {
#endif
    extern i32 saveload_status;
    extern i32 saveload_autosave;
    extern i32 saveload_savepresent;
    extern i32 saveload_slotid;
    extern i32 saveload_cardtype;
    extern i32 saveload_cardformatted;
    extern i32 saveload_freespace;
    extern i32 saveload_filecorrupt;
    extern char id_test[17];
    extern u8 code_test[16];

    extern i32 saveload_slotused[6];
    extern i32 saveload_slotcode[6];

    extern i32 SAVESLOTS;
    extern i32 SAVESIZE_ADDITIONAL;
    extern void *memcard_savedata;
    extern i32 memcard_savedatasize;
    extern void *memcard_savedatabuffer;
    extern void *memcard_extra_savedata;
    extern i32 memcard_extra_savedatasize;
    extern void *memcard_extra_savedatabuffer;
    extern void (*memcard_drawasiconfn)(void);
    extern i32 memcard_autosave;
    extern i32 memcard_autosaveenabled;
    extern i32 memcard_autosavestarted;
    extern i32 memcard_autosaveinprogress;
    extern i32 memcard_autosaveneeded;
    extern f32 memcard_autosavepredelay;
    extern f32 memcard_autosavepostdelay;
    extern i32 g_writingSaveCriticalSection;

    void saveloadInit(VARIPTR *buf, VARIPTR buf_end, i32, char *prodcode, char *iconname, char *unicodename, i32 unk);

    i32 saveloadLoadSlot(i32 slot, void *buffer, i32 size);
    i32 saveloadSaveSlot(i32 slot, void *buffer, i32 size);

    i32 TriggerExtraDataLoad(void);
    i32 TriggerAutoSave(void);
    bool TriggerExtraDataSave(void);

    void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                              void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

    i32 ChecksumSaveData(void *buffer, i32 size);

    void saveloadASSave(i32 slot, void *buffer, i32 size, u32 hash);
    void saveloadASLoad(i32 slot, void *buffer, i32 size);
    void saveloadASDelete(i32 slot);
    void saveloadASCallEachFrame(void);
    void saveloadASFormat(void);
    void saveloadAutoSave(void);
    i32 saveloadCheckCardFormatted(void);
    i32 saveloadCheckCardFreeSpace(i32 slot);
    i32 saveloadCheckCardPresent(void);
    i32 saveloadCheckCardType(void);
    i32 saveloadCheckKeyCode(char *id, u8 *code);
    i32 saveloadCheckSlotsUsed(void);
    i32 saveloadDeleteSlot(i32 slot);
    i32 saveloadFormatCard(void);
    i32 saveloadLoadIcon(void);
    i32 saveloadUnFormatCard(void);
    i32 saveloadWriteKeyCode(char *id, u8 *code);
#ifdef __cplusplus
}

char *slotname(i32 index);
char *slotfolder(i32 index);
void createslotfolder(i32 slot);
char *fullslotname(i32 index);
char *fullcodename(i32 index);
i32 saveloadGetDirectory();
#endif
