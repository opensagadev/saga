#pragma once

// Types shared by nu2api/nu3d/numtl.cpp and nu2api/nu3d/android/numtl_android.cpp.

struct MTL_OVERRIDE_RECORD {
    NUMTL *material;
    NUMTL *original;
    NUVERTEXDESCRIPTOR vertex_descriptor;
};
