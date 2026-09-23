#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct edbridge_s {
    i32 instance_id;
    NUVEC position;
    f32 length;
    f32 field_14;
    i16 rotation_z;
    i16 rotation_y;
    u8 connection_index;
    i8 field_1d;
    i8 field_1e;
    u8 field_1f;
    i32 special_20;
    i32 special_24;
    f32 field_28, field_2c, field_30, field_34, field_38, field_3c;
    u8 red, green, blue, field_43;
};
DECOMP_ASSERT(sizeof(edbridge_s) == 0x44, "edbridge_s size");

// Bridge-editor state and lifecycle shared across its reconstructed owners.
extern "C" {
    extern i32 edbri_page_used[8];
    extern edbridge_s edBridges[64];

    void edbriStartPage(i32 page);
    void edbriStartAllPages(void);
}
