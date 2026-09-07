#include "gamelib_util_types.h"

#include <stdio.h>
#include <string.h>

#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"

#include "java/asset_manager.h"
#include "java/android.h"

char AndroidOBBUtils::ms_packageName[3][512];
bool AndroidOBBUtils::ms_initializedPackage[3];
bool AndroidOBBUtils::ms_initializedPackageIsAsset[3];

void AndroidOBBUtils::InitPackagePaths() {
    memset(ms_initializedPackage, 0, sizeof(ms_initializedPackage));
    LookupPackagePath(NULL, NuFileDeviceAndroidOBBType::MAIN);
    LookupPackagePath(NULL, NuFileDeviceAndroidOBBType::PATCH);
}

i32 AndroidOBBUtils::LookupPackagePath(char *output, NuFileDeviceAndroidOBBType::T type) {
    char directory[512];
    char filename[512];
    if (output != NULL) {
        output[0] = '\0';
    }
    i32 prefix_length = NuStrIStr(g_externalDataPath, "Android/Data") + 8 - g_externalDataPath;
    if ((type == NuFileDeviceAndroidOBBType::MAIN && g_obbMainSize > 0) ||
        (type != NuFileDeviceAndroidOBBType::MAIN && g_obbPatchSize > 0)) {
        directory[0] = '\0';
        sprintf(filename, "%s%s.%04d.0000.%s", directory,
                type == NuFileDeviceAndroidOBBType::MAIN ? "main" : "patch",
                type == NuFileDeviceAndroidOBBType::MAIN ? g_obbMainVersion : g_obbPatchVersion, "jpg");
        AAsset *asset = AAssetManager_open(g_assetManager, filename, AASSET_MODE_UNKNOWN);
        if (asset != NULL) {
            AAsset_close(asset);
            strcpy(ms_packageName[type], filename);
            ms_initializedPackage[type] = true;
            ms_initializedPackageIsAsset[type] = true;
            if (output != NULL) {
                strcpy(output, filename);
            }
            return 2;
        }
    }
    if ((type == NuFileDeviceAndroidOBBType::MAIN && g_obbMainSize > 0) ||
        (type != NuFileDeviceAndroidOBBType::MAIN && g_obbPatchSize > 0)) {
        memcpy(directory, g_externalDataPath, prefix_length);
        directory[prefix_length] = '\0';
        strcat(directory, "obb/");
        strcat(directory, g_activityName);
        strcat(directory, "/");
        sprintf(filename, "%s%s.%04d.%s.%s", directory,
                type == NuFileDeviceAndroidOBBType::MAIN ? "main" : "patch",
                type == NuFileDeviceAndroidOBBType::MAIN ? g_obbMainVersion : g_obbPatchVersion,
                g_activityName, "obb");
        FILE *file = fopen(filename, "rb");
        if (file != NULL) {
            fclose(file);
            strcpy(ms_packageName[type], filename);
            ms_initializedPackage[type] = true;
            ms_initializedPackageIsAsset[type] = false;
            if (output != NULL) {
                strcpy(output, filename);
            }
            return 1;
        }
    }
    if ((type == NuFileDeviceAndroidOBBType::MAIN && g_obbMainSize > 0) ||
        (type != NuFileDeviceAndroidOBBType::MAIN && g_obbPatchSize > 0)) {
        prefix_length -= 9;
        memcpy(directory, g_externalDataPath, prefix_length);
        directory[prefix_length] = '\0';
        strcat(directory, "/TTGames/");
        strcat(directory, g_activityName);
        strcat(directory, "/obb/");
        sprintf(filename, "%s%s.%04d.%s.%s", directory,
                type == NuFileDeviceAndroidOBBType::MAIN ? "main" : "patch",
                type == NuFileDeviceAndroidOBBType::MAIN ? g_obbMainVersion : g_obbPatchVersion,
                g_activityName, "obb");
        FILE *file = fopen(filename, "rb");
        if (file != NULL) {
            fclose(file);
            strcpy(ms_packageName[type], filename);
            ms_initializedPackage[type] = true;
            ms_initializedPackageIsAsset[type] = false;
            if (output != NULL) {
                strcpy(output, filename);
            }
            return 1;
        }
    }
    return 0;
}

i32 AndroidOBBUtils::OpenFile(char const *name) {
    NuFileDeviceAndroidOBBType::T type;
    if (NuStrIStr(const_cast<char *>(name), "GAME.DAT") != NULL) {
        type = NuFileDeviceAndroidOBBType::MAIN;
    } else if (NuStrIStr(const_cast<char *>(name), "PATCH.DAT") != NULL) {
        type = NuFileDeviceAndroidOBBType::PATCH;
    } else {
        return 0;
    }
    if (ms_initializedPackage[type]) {
        return NuFileOpen(ms_packageName[type], NUFILE_READ);
    }
    return 0;
}
