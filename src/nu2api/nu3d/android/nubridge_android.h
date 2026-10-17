#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct nugscn_s;
struct nuhspecial_s;
struct numtl_s;
struct nuvec_s;
i32 NuBridgeAlloc(void);
void NuBrdigeDrawRope(numtl_s *material, nuvec_s *first, nuvec_s *second, i32, i32 *boundaries, i32 colour);
void ropesegment(numtl_s *, nuvec_s *, i32, i32);
extern "C" {
    i32 NuBridgeCreate(nugscn_s *, nuhspecial_s *, nuhspecial_s *, nuvec_s *, nuvec_s *, f32, i16, f32, f32, f32, f32, i32, f32, f32, i32, u32);
    void NuBridgeInit(void);
    void NuBridgeOn(i32 enabled);
    void NuBridgeRemove(i32 bridge);
}
