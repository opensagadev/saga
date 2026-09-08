#pragma once
#include "decomp.h"
#include "nu2api/numath/numtx.h"

struct PART_s;
struct nuhspecial_s;
struct rtldata_s;

struct ADDPART_s {
    NUMTX *matrix;
    u32 field_04;
    NUVEC *momentum;
    u32 field_0c[2];
    f32 field_14, field_18, gravity, field_20;
    nuhspecial_s *special;
    i32 field_28;
    u32 flags;
    u32 field_30[3];
    void (*impact)(PART_s *);
    u32 field_40[3];
    void (*stop)(PART_s *);
    u32 field_50;
    void (*draw)(PART_s *);
    i32 field_58;
    u32 field_5c;
    i32 field_60, field_64;
    f32 field_68, field_6c;
    i32 field_70, field_74, field_78;
    f32 field_7c;
    u32 field_80;
    i32 field_84;
    f32 field_88, frame_step;
    i32 field_90;
    rtldata_s *light_data;
    NUVEC scale;
    u32 field_a4[7];
    f32 field_c0;
    u32 field_c4;
};
DECOMP_ASSERT(sizeof(ADDPART_s) == 0xc8, "Part creation descriptor size");
DECOMP_ASSERT(offsetof(ADDPART_s, special) == 0x24, "Part special offset");
DECOMP_ASSERT(offsetof(ADDPART_s, impact) == 0x3c, "Part impact callback offset");
DECOMP_ASSERT(offsetof(ADDPART_s, stop) == 0x4c, "Part stop callback offset");
DECOMP_ASSERT(offsetof(ADDPART_s, draw) == 0x54, "Part draw callback offset");
DECOMP_ASSERT(offsetof(ADDPART_s, frame_step) == 0x8c, "Part frame step offset");
DECOMP_ASSERT(offsetof(ADDPART_s, light_data) == 0x94, "Part lighting offset");
extern "C" {
extern ADDPART_s Default_ADDPART;
void AddPart(ADDPART_s *part);
}
void SetKillPartMom(NUVEC *momentum);
void PartImpact_Brick(PART_s *part);
void PartStop_Flickerer(PART_s *part);
void PartDraw_Flickerer(PART_s *part);
