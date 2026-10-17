// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// The original unit constructs the homogeneous VuVec header constants.
#include "nu2api/nucore/nuvuvec.hpp"

#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nufile/nufile.h"

#include <cstdio>

extern "C" {
extern "C" void NuPs2VideoScreenDump(char *filename, i32 format, f32 scale_x, f32 scale_y, i32 face, i32 x, i32 y) {
    char path[256];
    if (face >= 0) {
        sprintf(path, "%s%d.bmp", filename, face);
    } else {
        sprintf(path, "%s.bmp", filename);
        i32 suffix = 0;
        for (;;) {
            if (NuFileSize(path) <= 0)
                break;
            sprintf(path, "%s%03d.bmp", filename, suffix++);
        }
    }
    NuFramebufferGetFrontBuffer();
}

}
