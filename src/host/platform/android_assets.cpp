#include "decomp.h"
#include "java/asset_manager.h"
#include "java/native_window.h"

// Host games use filesystem assets; there is no Android APK asset manager.
extern "C" AAsset *AAssetManager_open(AAssetManager *, const char *, int) {
    STUBBED();
    return NULL;
}
extern "C" void AAsset_close(AAsset *) {
    STUBBED();
}
extern "C" off_t AAsset_seek(AAsset *, off_t, int) {
    STUBBED();
    return -1;
}
extern "C" int AAsset_read(AAsset *, void *, size_t) {
    STUBBED();
    return -1;
}
extern "C" off_t AAsset_getLength(AAsset *) {
    STUBBED();
    return 0;
}

// The host renderer does not create Android native windows.
extern "C" int32_t ANativeWindow_getWidth(ANativeWindow *) {
    STUBBED();
    return 0;
}
extern "C" int32_t ANativeWindow_getHeight(ANativeWindow *) {
    STUBBED();
    return 0;
}
extern "C" int32_t ANativeWindow_setBuffersGeometry(ANativeWindow *, int32_t, int32_t, int32_t) {
    STUBBED();
    return -1;
}
