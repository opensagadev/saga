#include "nu2api_nufile_types.h"
#include <stdio.h>
#include <string.h>
#include "java/asset_manager.h"
#include "java/android.h"

NuFileAndroidAPK *NuFileAndroidAPK::ms_fileId[0x400];

void NuFileAndroidAPK::Init() {
    for (i32 i = 0; i < 0x400; ++i) {
        ms_fileId[i] = NULL;
    }
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

void NuFileAndroidAPK::ResetId(i32 id) {
    ms_fileId[id - 0x2000] = NULL;
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
