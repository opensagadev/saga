#pragma once

#include "decomp.h"

// Types shared by nu2api/nu3d/numtl.cpp and nu2api/nu3d/android/numtl_android.cpp.

struct MTL_OVERRIDE_RECORD {
    NUMTL *material;
    NUMTL *original;
    NUVERTEXDESCRIPTOR vertex_descriptor;
};

DECOMP_ASSERT(sizeof(MTL_OVERRIDE_RECORD) == 12, "Material override payload ABI");
