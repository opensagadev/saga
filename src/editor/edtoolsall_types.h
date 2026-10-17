#pragma once

// Types shared by editor/edtoolsall.cpp and legoapi/ai/core/aisysall.cpp.

struct CreaturePositionRecord {
    u8 unknown_00[0x28];
    nuvec_s position;
    i32 angle;
    u8 unknown_38[0x3c - 0x38];
    EDAIPATH_s *path;
    u8 unknown_40[0x5c - 0x40];
    u8 across_count;
    u8 unknown_5d[3];
    f32 x_spacing;
    f32 z_spacing;
};
