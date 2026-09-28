#include "decomp.h"
#include "java/asset_manager.h"
#include "java/native_window.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Host games use filesystem assets; there is no Android APK asset manager.
struct AAsset {
    FILE *file;
    off_t length;
};

extern "C" AAsset *AAssetManager_open(AAssetManager *, const char *path, int) {
    if (path == NULL)
        return NULL;
    FILE *file = fopen(path, "rb");
    if (file == NULL)
        return NULL;
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }
    auto length = ftell(file);
    if (length < 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }
    AAsset *asset = static_cast<AAsset *>(malloc(sizeof(AAsset)));
    if (asset == NULL) {
        fclose(file);
        return NULL;
    }
    asset->file = file;
    asset->length = length;
    return asset;
}
extern "C" void AAsset_close(AAsset *asset) {
    if (asset != NULL) {
        fclose(asset->file);
        free(asset);
    }
}
extern "C" off_t AAsset_seek(AAsset *asset, off_t offset, int whence) {
    if (asset == NULL || offset > LONG_MAX || offset < LONG_MIN ||
        fseek(asset->file, offset, whence) != 0)
        return -1;
    return ftell(asset->file);
}
extern "C" int AAsset_read(AAsset *asset, void *buffer, size_t length) {
    if (asset == NULL || buffer == NULL)
        return -1;
    if (length > INT_MAX)
        length = INT_MAX;
    size_t count = fread(buffer, 1, length, asset->file);
    return count == 0 && ferror(asset->file) ? -1 : static_cast<int>(count);
}
extern "C" off_t AAsset_getLength(AAsset *asset) {
    return asset == NULL ? 0 : asset->length;
}

// Host windows expose their dimensions through the engine screen singleton.
extern "C" int32_t ANativeWindow_getWidth(ANativeWindow *window) {
    return window != NULL && NuScreen::Exists() ? static_cast<int32_t>(NuScreen::Get()->GetWidth()) : 0;
}
extern "C" int32_t ANativeWindow_getHeight(ANativeWindow *window) {
    return window != NULL && NuScreen::Exists() ? static_cast<int32_t>(NuScreen::Get()->GetHeight()) : 0;
}
extern "C" int32_t ANativeWindow_setBuffersGeometry(ANativeWindow *window, int32_t width, int32_t height, int32_t) {
    if (window == NULL || width <= 0 || height <= 0)
        return -1;
    if (!NuScreen::Exists())
        NuScreen::Create();
    NuScreen::Get()->SetSceeenDimensions(static_cast<f32>(width), static_cast<f32>(height));
    return 0;
}
