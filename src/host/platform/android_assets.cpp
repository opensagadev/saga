#include "java/asset_manager.h"
#include "java/native_window.h"

// Host games use filesystem assets; there is no Android APK asset manager.
extern "C" AAsset *AAssetManager_open(AAssetManager *, const char *, int) {
    return NULL;
}
extern "C" void AAsset_close(AAsset *) {
}
extern "C" off_t AAsset_seek(AAsset *, off_t, int) {
    return -1;
}
extern "C" int AAsset_read(AAsset *, void *, size_t) {
    return -1;
}
extern "C" off_t AAsset_getLength(AAsset *) {
    return 0;
}

// The host renderer does not create Android native windows.
extern "C" int32_t ANativeWindow_getWidth(ANativeWindow *) {
    return 0;
}
extern "C" int32_t ANativeWindow_getHeight(ANativeWindow *) {
    return 0;
}
extern "C" int32_t ANativeWindow_setBuffersGeometry(ANativeWindow *, int32_t, int32_t, int32_t) {
    return -1;
}
