
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numemory.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

extern i32 nufile_buffering_enabled;
char *iop_image_name = "ioprp300.img";

struct NuFileAddress {
    void *address;
    i64 position;
};

static i32 MAX_FILE_ADDRESS;
static NuFileAddress *addr_ref;
static NuFileAddress *addr_pos;
static i32 ref_count;
static i32 pos_count;

struct NuFileWriteBlock {
    i32 tag;
    i32 size_sign;
    i64 position;
};

static NuFileWriteBlock nufile_wblkinfo[1024];
static i32 wblkcnt = 1;
static i32 iff_padsize = 16;

struct NuFileReadBlock {
    i32 tag;
    i32 size;
    i64 position;
};
static NuFileReadBlock nufile_blkinfo[1024];
static i32 blkcnt = 1;

extern "C" {

    i32 NuFileAlign(NUFILE file, i32 mask) {
        u8 padding = 0xfe;
        i32 count = 0;
        i64 position = NuFilePos(file);
        while ((position & mask) != 0) {
            ++position;
            ++count;
            NuFileWrite(file, &padding, 1);
        }
        return count;
    }
    i32 NuFileAlignRead(NUFILE file, i32 mask) {
        i32 count = 0;
        i64 position = NuFilePos(file);
        while ((position & mask) != 0) {
            ++position;
            ++count;
            NuFileReadChar(file);
        }
        return count;
    }
    i32 NuFileAppendPath(char *dest, char *path, char *name) {
        NuStrCpy(dest, path);
        i32 length = strlen(dest);
        char *end = dest + length;
        // The original advances past the terminator before writing the separator.
        if (*end++ != '\\') {
            *end++ = '\\';
        }
        NuStrCpy(end, name);
        return NuStrLen(dest);
    }
    i32 NuFileBeginBlkRead(NUFILE file, i32 expected_tag) {
        i32 index = blkcnt++;
        i64 position = NuFilePos(file);
        if (index > 1 && nufile_blkinfo[index - 1].position + nufile_blkinfo[index - 1].size <= position) {
            --blkcnt;
            return 0;
        }
        nufile_blkinfo[index].position = NuFilePos(file);
        NuFileRead(file, &nufile_blkinfo[index], 8);
        if (nufile_blkinfo[index].size < 0) {
            nufile_blkinfo[index].size = -nufile_blkinfo[index].size;
        }
        return nufile_blkinfo[index].tag;
    }
    void NuFileBeginBlkWrite(NUFILE file, i32 tag, i32 negative_size) {
        i32 index = wblkcnt++;
        nufile_wblkinfo[index].position = NuFilePos(file);
        nufile_wblkinfo[index].tag = tag;
        if (negative_size == 1) {
            nufile_wblkinfo[index].size_sign = -1;
        } else {
            nufile_wblkinfo[index].size_sign = 1;
        }
        NuFileWriteInt(file, tag);
        NuFileWriteInt(file, tag);
    }
    void NuFileCheckBadGameDiscStatus(void) {
    }
    void NuFileCloseDir(void) {
    }
    i32 NuFileCopy(char *dest, char *source) {
        char buffer[1024];
        return NuFileCopyEx(dest, source, buffer, sizeof(buffer));
    }
    i32 NuFileCopyEx(char *dest, char *source, void *buffer, i32 capacity) {
        i32 remaining;
        i32 copied = 0;
        NUFILE input = NuFileOpen(source, NUFILE_READ);
        NUFILE output = NuFileOpen(dest, NUFILE_WRITE);
        if (input != 0 && output != 0) {
            remaining = NuFileOpenSize(input);
            while (remaining != 0) {
                i32 count = NuFileRead(input, buffer, remaining < capacity ? remaining : capacity);
                NuFileWrite(output, buffer, count);
                copied += count;
                remaining -= count;
            }
        }
        if (input != 0) {
            NuFileClose(input);
        }
        if (output != 0) {
            NuFileClose(output);
        }
        return copied;
    }
    void NuFileCreateDir(void) {
    }
    void NuFileCreatePath(void) {
    }
    i32 NuFileEOF(NUFILE file) {
        i64 position = NuFilePos(file);
        i64 size = NuFileOpenSize(file);
        return position >= size;
    }
    void NuFileEndBlkRead(NUFILE file) {
        i32 index = --blkcnt;
        i64 position = NuFilePos(file);
        if (nufile_blkinfo[index].position + nufile_blkinfo[index].size != position) {
            NuFileSeek(file, nufile_blkinfo[index].position + nufile_blkinfo[index].size, NUFILE_SEEK_START);
        }
    }
    void NuFileEndBlkWrite(NUFILE file) {
        i64 position;
        i32 index;
        i32 padding;
        i32 size;
        i32 zero = 0;
        index = --wblkcnt;
        position = NuFilePos(file);
        size = position - nufile_wblkinfo[index].position;
        padding = iff_padsize - size % iff_padsize;
        if (padding != iff_padsize) {
            position += padding;
            size = size + padding;
            // The original supplies this four-byte local even when padding exceeds four.
            NuFileWrite(file, &zero, padding);
        }
        if (nufile_wblkinfo[index].size_sign < 0) {
            size = -size;
        }
        NuFileSeek(file, nufile_wblkinfo[index].position + 4, NUFILE_SEEK_START);
        NuFileWrite(file, &size, 4);
        NuFileSeek(file, position, NUFILE_SEEK_START);
    }
    i32 NuFileExtractExt(char *dest, char *path) {
        char *start = strrchr(path, '.');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        NuStrCpy(dest, start);
        return NuStrLen(dest);
    }
    i32 NuFileExtractFile(char *dest, char *path) {
        char *start = strrchr(path, '\\');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        char *end = strrchr(path, '.');
        if (end == NULL) {
            end = path + strlen(path);
        }
        i32 length = end - start;
        strncpy(dest, start, length);
        dest[length] = '\0';
        return length;
    }
    i32 NuFileExtractFilename(char *dest, char *path) {
        char *start = strrchr(path, '\\');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        NuStrCpy(dest, start);
        return NuStrLen(dest);
    }
    i32 NuFileExtractPath(char *dest, char *path) {
        char *end = strrchr(path, '\\');
        if (end != NULL) {
            ++end;
        } else {
            end = path;
        }
        i32 length = end - path;
        strncpy(dest, path, length);
        dest[length] = '\0';
        return length;
    }
    void NuFileFormat(void) {
    }
    i32 NuFileFormatName(char *dest, char *name, i32 capacity) {
        return default_device->format_name_fn(default_device, dest, name, capacity);
    }
    i32 NuFileGetBlkSize(void) {
        return nufile_blkinfo[blkcnt - 1].size;
    }
    NUFILE_DEVICE *NuFileGetCurrentDevice(void) {
        return default_device;
    }
    i32 NuFileGetCurrentDirectory(char *dest) {
        return NuStrCpy(dest, default_device->cur_dir);
    }
    void NuFileGetCurrentDllPath(char *dest) {
        NuStrCpy(dest, default_device->root);
        NuStrCat(dest, default_device->dll_dir);
        NuFileUpCase(default_device, dest + NuStrLen(default_device->root));
    }
    i32 NuFileGetCurrentPath(char *dest) {
        return default_device->format_name_fn(default_device, dest, "", 0x7fff);
    }
    i32 NuFileGetCurrentSysPath(char *dest) {
        return default_device->format_name_fn(default_device, dest, "", 0x7fff);
    }
    i32 NuFileGetEndianSwap(void) {
        return NuFile_SwapEndianOnWrite;
    }
    void NuFileGetInfo(void) {
    }
    i32 NuFileGetMediaMode(void) {
        return 0;
    }
    void NuFileInitAddress(i32 capacity) {
        if (capacity != MAX_FILE_ADDRESS) {
            if (addr_ref != NULL)
                NU_FREE(addr_ref);
            if (addr_pos != NULL)
                NU_FREE(addr_pos);
            MAX_FILE_ADDRESS = capacity;
            addr_ref = (NuFileAddress *)NU_ALLOC(MAX_FILE_ADDRESS * sizeof(NuFileAddress), 4, 1, "", 0);
            addr_pos = (NuFileAddress *)NU_ALLOC(MAX_FILE_ADDRESS * sizeof(NuFileAddress), 4, 1, "", 0);
        }
        ref_count = 0;
        pos_count = 0;
    }
    i32 NuFileIsNewer(nufile_info_s *first, nufile_info_s *second) {
        if (second->year > first->year)
            return 1;
        if (second->year < first->year)
            return 0;
        if (second->month > first->month)
            return 1;
        if (second->month < first->month)
            return 0;
        if (second->day > first->day)
            return 1;
        if (second->day < first->day)
            return 0;
        if (second->hour > first->hour)
            return 1;
        if (second->hour < first->hour)
            return 0;
        if (second->minute > first->minute)
            return 1;
        if (second->minute < first->minute)
            return 0;
        if (second->second > first->second)
            return 1;
        if (second->second < first->second)
            return 0;
        return 0;
    }
    void *NuFileLoad(char *path) {
        i32 buffering = nufile_buffering_enabled;
        nufile_buffering_enabled = 0;
        void *data = NULL;
        NUFILE file = NuFileOpen(path, NUFILE_READ);
        if (file != 0) {
            i32 size = NuFileOpenSize(file);
            if (size > 0) {
                data = NU_ALLOC(size, 4, 1, "", 0);
                if (data != NULL) {
                    NuFileRead(file, data, size);
                }
            }
            NuFileClose(file);
        }
        nufile_buffering_enabled = buffering;
        return data;
    }
    void NuFileOpenDir(void) {
    }
    void NuFilePatchAddress(NUFILE file) {
        i32 ref;
        i32 pos;
        i64 offset;
        for (ref = 0; ref < ref_count; ++ref) {
            for (pos = 0; pos < pos_count; ++pos) {
                if (addr_ref[ref].address == addr_pos[pos].address) {
                    offset = addr_pos[pos].position - addr_ref[ref].position;
                    NuFileSeek(file, addr_ref[ref].position, NUFILE_SEEK_START);
                    NuFileWrite(file, &offset, 4);
                }
            }
        }
        NuFileSeek(file, 0, NUFILE_SEEK_END);
    }
    void NuFileRename(void) {
    }
    void NuFileSetAddress(NUFILE file, void *address) {
        i32 index;
        if (address != NULL) {
            for (index = 0; index < pos_count; ++index) {
                if (addr_pos[index].address == address)
                    break;
            }
            if (index == pos_count)
                ++pos_count;
            addr_pos[index].address = address;
            addr_pos[index].position = NuFilePos(file);
        }
    }
    void NuFileSetBadGameDisc(void) {
    }
    void NuFileSetCurrentDevice(NUFILE_DEVICE *device) {
        default_device = device;
    }
    void NuFileSetCurrentDirectory(char *path) {
        i32 length;
        char separator[2] = "\\";
        if (*path != '\0') {
            length = NuStrCpy(default_device->cur_dir, path);
            if (length != 0 && path[length - 1] != '\\' && path[length - 1] != '/') {
                separator[0] = default_device->dir_separator;
                NuStrCat(default_device->cur_dir, separator);
            }
        } else {
            default_device->cur_dir[0] = '\0';
        }
    }
    void NuFileSetCurrentDllDirectory(char *path) {
        char *separator = "\\";
        if (*path != '\0') {
            NuStrCpy(default_device->dll_dir, path);
            NuStrCat(default_device->dll_dir, separator);
        } else {
            default_device->dll_dir[0] = '\0';
        }
    }
    void NuFileSetCurrentSysDirectory(char *path) {
        char *source;
        char *dest;
        char *separator = "\\";
        if (*path != '\0') {
            NuStrCpy(default_device->sys_dir, path);
            NuStrCat(default_device->sys_dir, separator);
        } else {
            NuStrCpy(default_device->sys_dir, "sys");
            source = iop_image_name + 5;
            dest = default_device->sys_dir + strlen(default_device->sys_dir);
            while (*source != '.') {
                *dest = *source;
                ++dest;
                ++source;
            }
            *dest++ = '\\';
            *dest = '\0';
        }
        NuFileUpCase(default_device, default_device->sys_dir);
    }
    i32 NuFileSwapEndianOnWrite(i32 enabled) {
        i32 previous = NuFile_SwapEndianOnWrite;
        NuFile_SwapEndianOnWrite = enabled;
        return previous;
    }
    void NuFileTidyAddress(void) {
        if (addr_ref != NULL) {
            NU_FREE(addr_ref);
            addr_ref = NULL;
        }
        if (addr_pos != NULL) {
            NU_FREE(addr_pos);
            addr_pos = NULL;
        }
        MAX_FILE_ADDRESS = 0;
    }
    void NuFileWriteAddress(NUFILE file, void *address) {
        static i32 pad;
        if (address != NULL) {
            addr_ref[ref_count].address = address;
            addr_ref[ref_count].position = NuFilePos(file);
            ++ref_count;
        }
        NuFileWrite(file, &pad, 4);
    }
    void NuFileWriteString(NUFILE file, const char *text) {
        NuFileWrite(file, const_cast<char *>(text), strlen(text));
    }
    i32 NuFileWriteStringV(NUFILE file, const char *format, ...) {
        i32 length;
        va_list args;
        char text[1024];
        va_start(args, format);
        vsprintf(text, format, args);
        va_end(args);
        length = strlen(text);
        return NuFileWrite(file, text, length);
    }
}
