#pragma once
#include "decomp.h"

#include "nu2api/nucore/common.h"
#include "nu2api/numath/numtx.h"

struct edgra_clump_s {
    i32 special_index;
    i32 element_count;
    NUVEC position;
    f32 size;
    f32 field_18;
    i32 flags;
    f32 field_20;
    u8 page, unknown_25, unknown_26, kind;
    i32 seed;
    f32 field_2c, field_30;
    i16 rotation_z, rotation_y;
    f32 near_distance, far_distance;
    i16 individual_index;
    u8 field_42, field_43;
    f32 field_44;
    NUMTX *matrices;
    void *vector_buffer;
};
struct edgra_individual_s {
    NUVEC position;
    f32 field_0c;
    i16 field_10, field_12;
    u8 unknown_14[0xc];
};
DECOMP_ASSERT(sizeof(edgra_individual_s) == 0x20, "individual grass record size");
edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);
DECOMP_ASSERT(sizeof(edgra_clump_s) == 0x50, "edgra clump size");

// edGra terrain swap protection (module gameapi/edtools, edtoolsall_plain.cpp).

#ifdef __cplusplus
extern "C" {
#endif
    void edGraInitTerrainSwapProtection();
    void edGraEnableTerrainSwap();
    void edGraDisableTerrainSwap();
    void edgraStartPage(i8 page);
    void edgraStopPage(i8 page);
#ifdef __cplusplus
}
#endif
