#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct numtl_s;
struct numtx_s;
extern "C" {
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, numtx_s *transform, numtl_s *mtl, void *faceon);
    void DisplayListCreateFxItemPS(void *item, i32 type);
    void DisplayListDestroyFxItemPS(void *item);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
}
