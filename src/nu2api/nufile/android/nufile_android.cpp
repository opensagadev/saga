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

char g_datfileMode = 1;

i64 NuFileAndroidAPK::Seek(i64 offset, NuFile::SeekOrigin::T origin) {
    i32 next;
    switch (origin) {
        case NuFile::SeekOrigin::START:
            next = offset;
            break;
        case NuFile::SeekOrigin::CURRENT:
            next = position + offset;
            break;
        case NuFile::SeekOrigin::END:
            next = file_size + offset;
            break;
        default:
            next = position;
            break;
    }
    position = MIN(file_size, (MAX(0, next)));
    return position;
}

isize NuFileAndroidAPK::Write(const void *buf, usize size) {
    STUBBED();
    return 0;
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
    NUPSFILE ps_file;
    FILE *file;

    if (mode == NUFILE_MODE_CNT) {
        return -1;
    }

    char path[0x400] = {0};
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
