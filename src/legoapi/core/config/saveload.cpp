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
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
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
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
i32 numfilteroutblocks;
char filteroutblocks[8][12];
i32 numfilterblocks;
char filterblocks[8][96];

char FS_FileList[0x10000];
char FS_LastFileName[64];
char FS_Path[256];
i32 FS_NumFiles;
i32 FS_CurrentPosFileNum;
i32 FS_CursorLine;
char FS_ParentEntry[8] = "D######";
i32 FS_SortMode = 3;
char *FS_CurrentCursorPos = FS_FileList;
char *FS_CurrentPos = FS_FileList;
char *FS_FileListEnd = FS_FileList;
char FS_Title[64] = "Title";
char FS_Filter[256] = "*.nup | *.hgp   ";
char FS_FilterOut[256] = "_PC. | _360. | _PS3.";
f32 FS_X = 50.0f;
f32 FS_Y = 40.0f;
f32 FS_Width = 248.0f;
f32 FS_W;
f32 FS_H;
u8 FS_Active;
u8 FS_RefreshDir;
u8 FS_ShowVolumes;
void (*FS_Callback)(char *, char *);

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

void FS_GetDirList(char *path, char *filter, char *filter_out) {
    FS_BuildFilterBlocks(filter);
    FS_BuildFilterOutBlocks(filter_out);

    FS_CurrentCursorPos = FS_FileList;
    FS_CurrentPos = FS_FileList;
    FS_NumFiles = 0;
    FS_CurrentPosFileNum = 0;
    FS_CursorLine = 0;

    NUFILE directory = NuFileOpenDir(path);
    __builtin_memcpy(FS_FileList, FS_ParentEntry, 7);
    NuStrCpy(FS_FileList + 7, "..");
    ++FS_NumFiles;

    char *volatile next;
    char entry[0x118] __attribute__((aligned(16)));
    volatile char header[7];
    if (directory != 0) {
        next = FS_FileList + 10;
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if (*reinterpret_cast<u16 *>(name) == '.' || (*reinterpret_cast<u32 *>(name) & 0xffffff) == 0x2e2e ||
                !(entry[0] & 8)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'D';
            header[4] = static_cast<u8>(entry[11]) + 'A';
            header[3] = static_cast<u8>(entry[10]) + 'A';
            header[2] = static_cast<u8>(entry[9]) + 'A';
            header[1] = static_cast<u8>(entry[8]) + 'A';
            header[5] = static_cast<u8>(entry[12]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[14]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(FS_FileList + 10, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        next = FS_FileList + 10;
        FS_SortStrings(next, next, FS_SortMode);
    }

    char *file_begin = next;
    directory = NuFileOpenDir(path);
    if (directory != 0) {
        while (i32 name_length = NuFileReadDir(directory, entry)) {
            char *name = entry + 0x18;
            if ((entry[0] & 8) || !FS_FileNameFilter(name)) {
                continue;
            }
            if (next + name_length + 8 >= FS_FileList + sizeof(FS_FileList)) {
                break;
            }
            header[0] = 'F';
            header[4] = static_cast<u8>(entry[19]) + 'A';
            header[3] = static_cast<u8>(entry[18]) + 'A';
            header[2] = static_cast<u8>(entry[17]) + 'A';
            header[1] = static_cast<u8>(entry[16]) + 'A';
            header[5] = static_cast<u8>(entry[20]) + 'A';
            u8 encoded_year = static_cast<u8>(entry[22]) - 123;
            header[6] = encoded_year;
            __builtin_memcpy(next, const_cast<const char *>(header), 6);
            next[6] = encoded_year;
            char *name_dest = next + 7;
            NuStrCpy(name_dest, name);
            name_dest[name_length] = '\0';
            next = name_dest + name_length + 1;
            ++FS_NumFiles;
        }
        FS_SortStrings(file_begin, next, FS_SortMode);
        NuFileCloseDir(directory);
    } else {
        FS_SortStrings(file_begin, file_begin, FS_SortMode);
    }
    FS_FileListEnd = next;
}

void SerialiseChar(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 1, 1);
}

i32 FS_PrevNameLen(char *name) {
    if (__builtin_expect(name == FS_FileList, 0)) {
        return 0;
    }
    char *previous = name - 2;
    if (__builtin_expect(*previous == '\0', 0)) {
        return 1;
    }
    if (__builtin_expect(previous == FS_FileList, 0)) {
        return 2;
    }
    i32 length = 0;
    for (;;) {
        --previous;
        ++length;
        if (*previous == '\0') {
            return length + 1;
        }
        if (previous == FS_FileList) {
            return length + 2;
        }
    }
}

static inline u32 FS_EncodedDateKey(const char *name) {
    u32 key = static_cast<u8>(name[6]) - 'A';
    key = key * 13 + static_cast<u8>(name[5]) - 'A';
    key = key * 33 + static_cast<u8>(name[4]) - 'A';
    key = key + ((key + (key << 1)) << 3) + static_cast<u8>(name[3]) - 'A';
    key *= 61;
    key += static_cast<u8>(name[2]);
    key -= 'A';
    key *= 61;
    return key + static_cast<u8>(name[1]) - 'A';
}

void FS_SortStrings(char *start, char *end, i32 mode) {
    char tmp[256];

    if (static_cast<u32>(mode) > 1) {
        const i32 direction = mode == 2 ? 1 : -1;
        for (char *current = start; current != end;) {
            u32 current_key = FS_EncodedDateKey(current);
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const u32 candidate_key = FS_EncodedDateKey(candidate);
                const i32 candidate_length = NuStrLen(candidate);
                const i32 compare = candidate_key < current_key ? 1 : (candidate_key > current_key ? -1 : 0);
                if (compare == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_key = candidate_key;
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    } else {
        const i32 direction = mode == 0 ? 1 : -1;
        for (char *current = start; current != end;) {
            i32 current_length = NuStrLen(current);
            char *candidate = current + NuStrLen(current) + 1;
            while (candidate != end) {
                const i32 candidate_length = NuStrLen(candidate);
                if (NuStrICmp(current + 7, candidate + 7) == direction) {
                    NuStrCpy(tmp, candidate);
                    memmove(current + candidate_length + 1, current, candidate - current);
                    memcpy(current, tmp, candidate_length + 1);
                    current_length = candidate_length;
                }
                candidate += candidate_length + 1;
            }
            current += current_length + 1;
        }
    }
}

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

void FS_MoveCursorUp(i32 steps) {
    i32 remaining = steps;
    while (remaining > 0) {
        i32 currentPosLength = FS_PrevNameLen(FS_CurrentPos);
        i32 cursorLength = FS_PrevNameLen(FS_CurrentCursorPos);
        char *cursor = FS_CurrentCursorPos;

        if (cursor == FS_CurrentPos) {
            FS_CurrentPos = cursor - currentPosLength;
            if (FS_CurrentPosFileNum != 0) {
                --FS_CurrentPosFileNum;
            }
        } else {
            --FS_CursorLine;
        }

        FS_CurrentCursorPos = cursor - cursorLength;
        --remaining;
    }
}

void SerialiseString(EdStream &stream, void *data, i32 count) {
    stream.SerialiseString(static_cast<char *>(data), count);
}

void SerialiseColour3(EdStream &stream, void *data, i32) {
    stream.SerialiseBuffer(data, 4, 3);
}

i32 FS_FileNameFilter(char *name) {
    char lower[256];
    NuStrCpy(lower, name);
    NuStrToLower(lower);

    for (i32 i = 0; i < numfilteroutblocks; ++i) {
        if (strstr(lower, filteroutblocks[i]) != NULL) {
            return 0;
        }
    }

    for (i32 i = 0; i < numfilterblocks; ++i) {
        const char *block = filterblocks[i];
        char *scan = lower;
        do {
            char *match;
            if (block[0] != '\0') {
                match = strstr(scan, block);
                if (match == NULL) {
                    break;
                }
                scan = match + 1;
            } else {
                match = scan;
                scan = NULL;
                goto eligible_filter;
            }

            if (block[12] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 12);
            if (match == NULL) {
                continue;
            }
            if (block[24] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 24);
            if (match == NULL) {
                continue;
            }
            if (block[36] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 36);
            if (match == NULL) {
                continue;
            }
            if (block[48] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 48);
            if (match == NULL) {
                continue;
            }
            if (block[60] == '\0') {
                goto eligible_filter;
            }
            match = strstr(match, block + 60);
            if (match == NULL) {
                continue;
            }
            if (block[72] != '\0') {
                continue;
            }

        eligible_filter:
            i32 dots = 0;
            for (char *p = match; *p != '\0'; ++p) {
                if (*p == '.') {
                    if (++dots > 1) {
                        break;
                    }
                }
            }
            if (dots < 2) {
                return 1;
            }
        } while (scan != NULL);
    }
    return 0;
}

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d", date[4] - 'A', date[5] - 'A', date[6] + 1915);
}

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d:%.2d:%.2d", date[3] - 'A', date[2] - 'A', date[1] - 'A');
}

void FS_MoveCursorDown(i32 steps) {
    char **currentPos = &FS_CurrentPos;
    char **cursorPos = &FS_CurrentCursorPos;
    i32 remaining = steps;
    while (remaining > 0) {
        i32 currentPosLength = NuStrLen(*currentPos);
        i32 cursorLength = NuStrLen(*cursorPos);
        char *next = *cursorPos + cursorLength + 1;
        char *listEnd = FS_FileListEnd;

        if (next < listEnd) {
            i32 *cursorLine = &FS_CursorLine;
            i32 line = *cursorLine;
            if (line == 13) {
                char *nextTop = *currentPos + currentPosLength + 1;
                if (listEnd > nextTop) {
                    *currentPos = nextTop;
                    ++FS_CurrentPosFileNum;
                }
            } else {
                *cursorLine = line + 1;
            }
            *cursorPos = next;
        }
        --remaining;
    }
}

f32 FS_GetDirTextWidth() {
    char date_time[64];
    f32 max_width = 0.0f;
    for (char *entry = FS_FileList; entry < FS_FileListEnd;) {
        char *name = entry + 7;
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = NuQFntPrintLenU(system_qfont, name);
        if (entry[6] != '#') {
            NuQFntSetPointSize(system_qfont, 0.8f, 1.0f);
            FS_MakeDateTimeString(reinterpret_cast<FS_FILEENTRYHDR *>(entry), date_time);
            width += NuQFntPrintLenU(system_qfont, date_time);
        }
        if (width > max_width) {
            max_width = width;
        }
        entry = name + NuStrLen(name) + 1;
    }
    return max_width;
}

char *FS_GetFilterString(char *input, char *output) {
    while (*input == ' ' || *input == '*') {
        ++input;
    }

    char *end = input;
    if (*end != '|' && *end != '\0') {
        do {
            ++end;
        } while (*end != '\0' && *end != '|' && *end != '*');

        char *trimmed = end;
        while (trimmed != input && trimmed[-1] == ' ') {
            --trimmed;
        }
        for (; input < trimmed; ++input) {
            *output++ = *input;
        }
    }
    *output = '\0';
    return end;
}

i32 getsaveload_status() {
    return saveload_status;
}

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed) {
    static i32 LastPad;
    static float PadRepeat;

    if (pad->digital_buttons != LastPad) {
        LastPad = pad->digital_buttons;
        PadRepeat = 0.5f;
        return LastPad;
    }

    PadRepeat -= elapsed;
    if (PadRepeat < 0.0f) {
        PadRepeat += repeat;
        return LastPad;
    }
    return 0;
}

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

void FS_BuildFilterBlocks(char *input) {
    numfilterblocks = 0;
    NuStrToLower(input);
    i32 slot = 0;
    while (*input != '\0') {
        input = FS_GetFilterString(input, &filterblocks[0][0] + numfilterblocks * 96 + slot * 12);
        if (*input == '|') {
            filterblocks[numfilterblocks][(slot + 1) * 12] = '\0';
            ++numfilterblocks;
            slot = 0;
            ++input;
        } else {
            ++slot;
        }
    }
    if (slot != 0) {
        ++numfilterblocks;
    }
}

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output) {
    const u8 *date = reinterpret_cast<const u8 *>(entry);
    sprintf(output, " %.2d/%.2d/%d %.2d:%.2d:%.2d", date[4] - 'A', date[5] - 'A', date[6] + 1915, date[3] - 'A',
            date[2] - 'A', date[1] - 'A');
}

void FS_BuildFilterOutBlocks(char *input) {
    numfilteroutblocks = 0;
    NuStrToLower(input);
    while (*input != '\0') {
        input = FS_GetFilterString(input, filteroutblocks[numfilteroutblocks]);
        if (filteroutblocks[numfilteroutblocks][0] != '\0') {
            ++numfilteroutblocks;
        }
        if (*input == '|') {
            ++input;
        }
    }
}

void FS_SetCursorToLastFileName() {
    char *last_name = FS_LastFileName;
    if (last_name[0] == '\0' || FS_NumFiles <= 0) {
        return;
    }
    char *entry = FS_FileList;
    for (i32 index = 0; index < FS_NumFiles; ++index) {
        if (NuStrICmp(entry + 7, last_name) == 0) {
            FS_CurrentCursorPos = entry;
            FS_CurrentPos = entry;
            FS_CurrentPosFileNum = index;
            FS_CursorLine = 0;
            if (index == 0 || index + 13 < FS_NumFiles) {
                return;
            }
            char *current = entry;
            i32 last_visible = index + 12;
            volatile i32 old_visible;
            do {
                i32 previous_length = FS_PrevNameLen(current);
                if (previous_length == 0) {
                    return;
                }
                current = FS_CurrentPos - previous_length;
                --FS_CurrentPosFileNum;
                FS_CurrentPos = current;
                ++FS_CursorLine;
                if (last_visible == 13) {
                    return;
                }
                old_visible = last_visible--;
            } while (old_visible >= FS_NumFiles);
            return;
        }
        entry += NuStrLen(entry) + 1;
    }
}

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad) {
    if (FS_Active == 0)
        return 0;
    if (FS_RefreshDir != 0) {
        FS_GetDirList(FS_Path, FS_Filter, FS_FilterOut);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSet(system_qfont);
        NuQFntSetPointSize(system_qfont, 1.0f, 1.0f);
        f32 width = FS_GetDirTextWidth() * 0.0625f;
        NuQFntPopCoordinateSystem();
        if (width < 240.0f)
            width = 240.0f;
        FS_Width = width + 8.0f;
        FS_GetPadWithRepeat(pad, 0.05f, elapsed);
        if (FS_LastFileName[0] != '\0')
            FS_SetCursorToLastFileName();
        FS_RefreshDir = 0;
    }
    const i32 buttons = FS_GetPadWithRepeat(pad, 0.05f, elapsed);
    if ((buttons & 0x4000) != 0)
        FS_MoveCursorDown(1);
    if ((buttons & 0x1000) != 0)
        FS_MoveCursorUp(1);
    if ((buttons & 1) != 0)
        FS_MoveCursorDown(14);
    if ((buttons & 4) != 0)
        FS_MoveCursorUp(14);
    if ((buttons & 2) != 0)
        FS_MoveCursorDown(FS_NumFiles);
    if ((buttons & 8) != 0)
        FS_MoveCursorUp(FS_NumFiles);
    if ((buttons & 0x80) != 0) {
        FS_RefreshDir = 2;
        FS_SortMode = static_cast<i32>(static_cast<u32>(FS_SortMode) + 1);
        if (FS_SortMode == 4)
            FS_SortMode = 0;
        NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    }
    if ((buttons & 0x20) != 0) {
        const i32 length = NuStrLen(FS_Path);
        if (length != 0 && FS_Path[length - 1] == '\\')
            FS_Path[length - 1] = '\0';
        char *separator = NuStrRChr(FS_Path, '\\');
        if (separator != NULL)
            separator[1] = '\0';
        else
            NuStrCat(FS_Path, "\\");
        FS_RefreshDir = 1;
        FS_LastFileName[0] = '\0';
    }
    if ((buttons & 0x40) != 0) {
        if (FS_CurrentCursorPos[0] == 'V') {
            FS_ShowVolumes = 0;
            NuStrCpy(FS_Path, FS_CurrentCursorPos + 7);
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else if (FS_CurrentCursorPos[0] == 'D') {
            if (FS_CurrentCursorPos[7] == '.' && FS_CurrentCursorPos[8] == '.' && FS_CurrentCursorPos[9] == '\0') {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] == '\\')
                    FS_Path[length - 1] = '\0';
                char *separator = NuStrRChr(FS_Path, '\\');
                if (separator != NULL)
                    separator[1] = '\0';
                else
                    NuStrCat(FS_Path, "\\");
            } else {
                const i32 length = NuStrLen(FS_Path);
                if (length != 0 && FS_Path[length - 1] != '\\')
                    NuStrCat(FS_Path, "\\");
                NuStrCat(FS_Path, FS_CurrentCursorPos + 7);
                NuStrCat(FS_Path, "\\");
            }
            FS_RefreshDir = 1;
            FS_LastFileName[0] = '\0';
        } else {
            FS_Active = 0;
            NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
            if (FS_Callback != NULL)
                FS_Callback(FS_Path, FS_CurrentCursorPos + 7);
            return 1;
        }
    }
    return 0;
}

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad) {
    return ProcessFileSel3(elapsed, pad);
}

extern "C" void FileSelKill(void) {
    NuStrCpy(FS_LastFileName, FS_CurrentCursorPos + 7);
    FS_Active = 0;
}

void ProcessFileSel(f32 elapsed, nupad_s *pad) {
    ProcessFileSel3(elapsed, pad);
    RenderFileSel();
    if ((pad->digital_buttons & 0x10) != 0)
        FileSelKill();
}

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *)) {
    if (title != NULL)
        NuStrCpy(FS_Title, title);
    if (path != NULL)
        NuStrCpy(FS_Path, path);
    if (filter != NULL)
        NuStrCpy(FS_Filter, filter);
    else
        FS_Filter[0] = '\0';
    if (filter_out != NULL)
        NuStrCpy(FS_FilterOut, filter_out);
    else
        FS_FilterOut[0] = '\0';
    FS_Active = 1;
    FS_Callback = callback;
    FS_RefreshDir = 2;
}

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height) {
    FS_X = static_cast<f32>(x);
    FS_Y = static_cast<f32>(y) * 0.5f;
    RenderFileSel3(0);
    *height = static_cast<i32>(FS_H + FS_H);
    *width = static_cast<i32>(FS_W);
}

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *) {
    return 0;
}

extern "C" {
    void (*savesuccessfn)(void);
    void *memcard_headerdata;
    i32 memcard_headerdatasize;
    void *memcard_headerdatabuffer;

    void FS_SetFileSelPathFromName(char *name) {
        char file_name[256] = "";
        NuStrCpy(FS_Path, name);
        if (NuStrRChr(FS_Path, '.') != NULL) {
            char *separator = NuStrRChr(FS_Path, '\\');
            if (separator != NULL) {
                NuStrCpy(file_name, separator + 1);
                separator[1] = '\0';
            } else {
                NuStrCpy(file_name, name);
                FS_Path[0] = '\0';
            }
        }
        if (file_name[0] != '\0') {
            reinterpret_cast<i32 (*)(char *, char *, i32)>(&NuFileExtConvert)(FS_LastFileName, file_name, 255);
        } else {
            FS_LastFileName[0] = '\0';
        }
    }

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
                    memcard_loadcorrupt = 1;
                    saveload_autosave = -1;
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
            memcard_saveneeded = 1;
            memcard_slot = saveload_autosave;
        }

        if (memcard_saveneeded != 0) {
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
        } else if (memcard_savestarted != 0 && saveload_status == 1) {
            memcard_savestarted = 0;
            if (saveload_error == 0) {
                MenuSaveOccurred = 1;
                if (savesuccessfn != NULL)
                    savesuccessfn();
                if (memcard_autosavestarted != 0) {
                    memcard_autosavestarted = 0;
                    memcard_autosavepostdelay = 1.0f;
                }
            } else {
                memcard_savefailed = 1;
                if (saveload_autosave != -1) {
                    saveload_autosave = -1;
                    memcard_saveresult_delay = 1.5f;
                    saveload_autosavedisabled = 1;
                    memcard_autosavedisabled = 1;
                }
                memcard_autosavestarted = 0;
                memcard_autosaveinprogress = 0;
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
                memcard_formatme = 0;
                memcard_formatmessage_delay = 1.5f;
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
