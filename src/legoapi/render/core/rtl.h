#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec.h"

struct rtl_s {
    NUVEC position;
    NUVEC direction;
    NUVEC ambient;
    NUVEC colour;
    NUVEC secondary_colour;
    f32 inner_radius;
    f32 outer_radius;
    f32 parameters[4];
    f32 parameter_54;
    u8 type;
    union {
        u8 flags;
        struct {
            u8 disabled : 1;
            u8 reserved_flags : 7;
        };
    };
    i16 pitch;
    i16 yaw;
    i16 field_5e;
    i16 field_60;
    u8 pad_62[2];
    f32 field_64;
    u16 field_68;
    i16 uid;
    f32 intensity;
    u8 pad_70[9];
    i8 field_79;
    i8 field_7a;
    u8 field_7b;
    u32 field_7c;
    u8 pad_80[0xc];
};
DECOMP_ASSERT(sizeof(rtl_s) == 0x8c, "RTL light size");
DECOMP_ASSERT(offsetof(rtl_s, colour) == 0x24, "RTL colour offset");
DECOMP_ASSERT(offsetof(rtl_s, inner_radius) == 0x3c, "RTL radius offset");
DECOMP_ASSERT(offsetof(rtl_s, type) == 0x58, "RTL type offset");
DECOMP_ASSERT(offsetof(rtl_s, uid) == 0x6a, "RTL UID offset");

struct rtlfog_s {
    u8 reserved_00[0x8];
    u32 colour;
    u8 reserved_0c[0x8];
    i32 type;
    u8 reserved_18[0x4];
    f32 radius;
    NUVEC position;
    u8 reserved_2c[0x20];
};
DECOMP_ASSERT(sizeof(rtlfog_s) == 0x4c, "RTL fog size");

struct rtlset {
    u32 header;
    rtl_s lights[128];
    rtlfog_s fog[32];
};
DECOMP_ASSERT(offsetof(rtlset, lights) == 4, "RTL set light-array offset");
DECOMP_ASSERT(offsetof(rtlset, fog) == 0x4604, "RTL set fog-array offset");
DECOMP_ASSERT(sizeof(rtlset) == 0x4f84, "RTL set size");

extern "C" {
    extern rtlset *curr_set;

    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    i32 rtlDynamicAlloc(void);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);
    i32 rtlFindByUserId(usize, i32);
    void rtlDynamicFree(i32);
    bool rtlDynamicEnable(i32, i32);
    i32 rtlDynamicSetType(i32, i32);
    i32 rtlDynamicSetColours(i32, NUVEC *, NUVEC *);
    i32 rtlDynamicSetPos(i32, NUVEC *);
    i32 rtlDynamicSetRadii(i32, f32, f32);
}
