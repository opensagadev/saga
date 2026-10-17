// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nufile/nu2api_nufile_types.h"
#include "nu2api/nucore/nuvuvec.hpp"

#include "nu2api/nufile/android/nufile_android.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/tmclient.h"
#include "java/asset_manager.h"
#include "java/android.h"

NuFileAndroidAPK *NuFileAndroidAPK::ms_fileId[0x400];

NuFileDeviceAndroidAPK::~NuFileDeviceAndroidAPK() {
}

void NuFileAndroidAPK::Close() {
    if (asset != NULL) {
        AAsset_close(asset);
        asset = NULL;
    }
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
            if (remaining == 0)
                break;
        }
        return position - start;
    } else {
        AAsset_seek(asset, position, SEEK_SET);
        return AAsset_read(asset, buf, size);
    }
}

NuFileAndroidAPK::~NuFileAndroidAPK() {
    Close();
}

void NuFileAndroidAPK::Init() {
    for (i32 i = 0; i < 0x400; ++i) {
        ms_fileId[i] = NULL;
    }
}

i32 NuFileAndroidAPK::SetFileId(NuFileAndroidAPK *file) {
    for (i32 i = 0; i < 0x400; i++) {
        if (ms_fileId[i] == NULL) {
            ms_fileId[i] = file;
            return i + 0x2000;
        }
    }
    return -1;
}

void NuFileAndroidAPK::ResetId(i32 id) {
    ms_fileId[id - 0x2000] = NULL;
}

NuFileAndroidAPK::NuFileAndroidAPK(const char *filepath, NuFile::OpenMode::T mode)
    : NuFileBase(filepath, mode, 0x534f2020) {
}

NuFileAndroidAPK *NuFileAndroidAPK::Open(const char *path, NuFile::OpenMode::T mode) {
    if (mode == 1 || mode == 2) {
        return NULL;
    }
    AAsset *first_asset = AAssetManager_open(g_assetManager, path, AASSET_MODE_UNKNOWN);
    if (first_asset == NULL) {
        return NULL;
    }
    NuFileAndroidAPK *file = new NuFileAndroidAPK(path, mode);
    strcpy(file->asset_path, path);
    file->file_size = AAsset_getLength(first_asset);
    file->chunk_size = 0;
    file->position = 0;
    file->asset = first_asset;
    file->asset_index = 0;
    char next_path[256];
    strcpy(next_path, file->asset_path);
    char *part = strstr(next_path, ".0000.jpg");
    if (part != NULL) {
        file->chunk_size = file->file_size;
        u32 index = 0;
        for (;;) {
            sprintf(part, ".%04d.jpg", ++index);
            AAsset *next = AAssetManager_open(g_assetManager, next_path, AASSET_MODE_UNKNOWN);
            if (next == NULL) {
                break;
            }
            file->file_size += AAsset_getLength(next);
            AAsset_close(next);
        }
    }
    return file;
}

NuFileBase *NuFileDeviceAndroidAPK::CreateNuFile(char const *path, NuFile::OpenMode::T mode) const {
    return NuFileAndroidAPK::Open(path, mode);
}

NuFileDeviceAndroidAPK::NuFileDeviceAndroidAPK(char const *name, NuFile::InitData const &) {
    device_type = NUFILE_DEVICE_ANDROID_APK;
    separator = "/";
    label = name;
    flags = 9;
    mount_name = "";
}

