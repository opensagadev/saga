#include "nu2api/nufile/android/nufile_android.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "decomp.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/tmclient.h"
#include "java/asset_manager.h"
#include "java/android.h"

char g_datfileMode = 1;

NuFileAndroidAPK::NuFileAndroidAPK(const char *filepath, NuFile::OpenMode::T mode)
    : NuFileBase(filepath, mode, 0x534f2020) {
}

NuFileAndroidAPK::~NuFileAndroidAPK() {
    Close();
}

i64 NuFileAndroidAPK::Seek(i64 offset, NuFile::SeekOrigin::T origin) {
    i32 next;
    switch (origin) {
        case NuFile::SeekOrigin::START: next = offset; break;
        case NuFile::SeekOrigin::CURRENT: next = position + offset; break;
        case NuFile::SeekOrigin::END: next = file_size + offset; break;
        default: next = position; break;
    }
    position = MIN(file_size, (MAX(0, next)));
    return position;
}

isize NuFileAndroidAPK::Read(void *buf, usize size) {
    if (chunk_size != 0) {
        u64 remaining = size < static_cast<usize>(file_size - position) ? size : file_size - position;
        i32 start = position;
        u8 *output = static_cast<u8 *>(buf);
        while (position < file_size) {
            u32 index = static_cast<u32>(position) / chunk_size;
            if (asset_index != index) {
                char next_path[256];
                strcpy(next_path, asset_path);
                char *part = strstr(next_path, ".0000.jpg");
                sprintf(part, ".%04d.jpg", index);
                AAsset_close(asset);
                asset = AAssetManager_open(g_assetManager, next_path, AASSET_MODE_UNKNOWN);
                asset_index = index;
            }
            i32 current = position;
            i32 chunk = chunk_size;
            AAsset_seek(asset, current - index * chunk, SEEK_SET);
            i32 amount = ((current & -chunk) + chunk) - current;
            if (static_cast<i64>(amount) > static_cast<i64>(remaining)) {
                amount = remaining;
            }
            u32 count = AAsset_read(asset, output, amount);
            remaining -= count;
            position += count;
            output += count;
            if (remaining == 0) break;
        }
        return position - start;
    } else {
        AAsset_seek(asset, position, SEEK_SET);
        return AAsset_read(asset, buf, size);
    }
}

isize NuFileAndroidAPK::Write(const void *buf, usize size) {
    return 0;
}

void NuFileAndroidAPK::Close() {
    if (asset != NULL) {
        AAsset_close(asset);
        asset = NULL;
    }
}

static FILE *g_fileHandles[32] = {NULL};

i32 NuPSFileRead(NUPSFILE index, void *dest, i32 len) {
    return fread(dest, 1, len, g_fileHandles[index]);
}

i32 NuPSFileWrite(NUPSFILE index, const void *src, i32 len) {
    LOG_DEBUG("index=%d, src=%p, len=%zu", index, src, len);

    FILE **files = g_fileHandles;
    return fwrite(src, 1, len, files[index]);
}

NUPSFILE NuPSFileOpen(char *filepath, NUFILEMODE mode) {
    char path[0x400];
    NUPSFILE ps_file;
    FILE *file;

    if (mode == NUFILE_MODE_CNT) {
        return -1;
    }

    memset(path, 0, sizeof(path));
    NuStrCpy(path, filepath);

    for (char *cursor = path; *cursor != '\0'; cursor++) {
        if (*cursor == '\\') {
            *cursor = '/';
        }
    }

    ps_file = NuGetFileHandlePS();

    file = NULL;
    switch (mode) {
        case NUFILE_READ:
            file = fopen(path, "rb");
            break;
        case NUFILE_WRITE:
            file = fopen(path, "wb");
            break;
        case NUFILE_APPEND:
            file = fopen(path, "ab+");
            break;
        default:
            return -1;
    }

    if (file == NULL) {
        return -1;
    }

    g_fileHandles[ps_file] = file;

    LOG_DEBUG("Opened file %s with index %d", path, ps_file);
    return ps_file;
}

NUPSFILE NuGetFileHandlePS(void) {
    for (i32 i = 0; i < 32; i++) {
        if (g_fileHandles[i] == NULL) {
            return i;
        }
    }

    return -1;
}

i32 NuPSFileClose(NUPSFILE index) {
    fclose(g_fileHandles[index]);
    g_fileHandles[index] = NULL;
    return 1;
}

i64 NuPSFileLSeek(NUPSFILE index, i64 offset, NUFILESEEK seekMode) {
    LOG_DEBUG("file=%d, offset=0x%llx, seekMode=%d", index, offset, seekMode);

    i32 whence = 0;

    switch (seekMode) {
        case NUFILE_SEEK_START:
            whence = SEEK_SET;
            break;
        case NUFILE_SEEK_CURRENT:
            whence = SEEK_CUR;
            break;
        case NUFILE_SEEK_END:
            whence = SEEK_END;
            break;
    }

    i64 value = 0;

    if (fseek(g_fileHandles[index], offset, whence) == 0) {
        value = ftell(g_fileHandles[index]);
    }

    return value;
}

void NuPSFileInitDevices(i32 device_id, i32 reboot_iop, i32 eject) {
    if (nuapi_use_target_manager) {
        the_tm_client = new TMClient(nuapi_use_target_manager, nuapi_target_manager_mac_address);
    }

    read_critical_section = NuThreadCreateCriticalSection();
    chdir(NuIOS_GetAppBundlePath());
}
