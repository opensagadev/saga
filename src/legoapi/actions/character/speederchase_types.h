#pragma once

// Types shared by legoapi/actions/character/speederchase.cpp and gameapi/edtools/edtoolsall.cpp.

struct SPEEDERCHASEANETPACKET_s {
    u8 field_0x0;
    u8 speeder_count;
    u8 speeders_killed;
    u8 field_0x3;
    u8 panel_state;
    u8 field_0x5;
    u8 field_0x6;
};
