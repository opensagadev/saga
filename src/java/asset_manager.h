#pragma once

#include <stddef.h>
#include <sys/types.h>

struct AAssetManager;
struct AAsset;
enum { AASSET_MODE_UNKNOWN = 0 };
extern "C" {
    AAsset *AAssetManager_open(AAssetManager *, const char *, int);
    void AAsset_close(AAsset *);
    off_t AAsset_seek(AAsset *, off_t, int);
    int AAsset_read(AAsset *, void *, size_t);
    off_t AAsset_getLength(AAsset *);
}
