#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"

struct rtldata_s;
struct burnset_s;

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
            u8 cast_shadow : 1;
            u8 has_specular : 1;
            u8 reserved_flags : 5;
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
    f32 blend_rate;
    f32 blend;
    u8 group_id;
    i8 field_79;
    i8 field_7a;
    i8 field_7b;
    rtl_s *field_7c;
    u8 pad_80[0xc];
};
DECOMP_ASSERT(sizeof(rtl_s) == 0x8c, "RTL light size");
DECOMP_ASSERT(offsetof(rtl_s, colour) == 0x24, "RTL colour offset");
DECOMP_ASSERT(offsetof(rtl_s, inner_radius) == 0x3c, "RTL radius offset");
DECOMP_ASSERT(offsetof(rtl_s, type) == 0x58, "RTL type offset");
DECOMP_ASSERT(offsetof(rtl_s, uid) == 0x6a, "RTL UID offset");
DECOMP_ASSERT(offsetof(rtl_s, blend_rate) == 0x70, "RTL blend rate offset");
DECOMP_ASSERT(offsetof(rtl_s, blend) == 0x74, "RTL blend factor offset");
DECOMP_ASSERT(offsetof(rtl_s, field_7b) == 0x7b, "RTL modifier index offset");
DECOMP_ASSERT(offsetof(rtl_s, field_7c) == 0x7c, "RTL chain base offset");
DECOMP_ASSERT(offsetof(rtl_s, group_id) == 0x78, "RTL group ID offset");

struct rtlfog_s {
    f32 start;
    f32 end;
    u32 colour;
    i32 low_quality_density;
    u32 low_quality_colour;
    i32 type;
    u8 depth_of_field_fstop;
    u8 reserved_19[3];
    f32 radius;
    NUVEC position;
    f32 density;
    f32 start_psp;
    f32 end_psp;
    u8 reserved_38[4];
    f32 density_wii;
    u8 reserved_40[0xc];
};
DECOMP_ASSERT(sizeof(rtlfog_s) == 0x4c, "RTL fog size");
DECOMP_ASSERT(offsetof(rtlfog_s, low_quality_density) == 0x0c, "RTL low-quality fog density offset");
DECOMP_ASSERT(offsetof(rtlfog_s, depth_of_field_fstop) == 0x18, "RTL fog depth-of-field offset");
DECOMP_ASSERT(offsetof(rtlfog_s, density) == 0x2c, "RTL fog density offset");
DECOMP_ASSERT(offsetof(rtlfog_s, start_psp) == 0x30, "RTL PSP fog start offset");
DECOMP_ASSERT(offsetof(rtlfog_s, end_psp) == 0x34, "RTL PSP fog end offset");
DECOMP_ASSERT(offsetof(rtlfog_s, density_wii) == 0x3c, "RTL Wii fog density offset");

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
    extern u16 rtltimer1;
    extern f32 rtltimer1adv;

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
    void rtlResetEx(rtldata_s *, i32);
    void rtlApplySetScale(void *, rtldata_s *, NUVEC *, NUMTX *, i32, f32);
    i32 rtlDynamicMasterEnable(i32);
    i32 rtlDynamicSetDirection(i32, NUVEC *);
    i32 rtlGetDirection(usize, i32, void **);
    void rtlSetShadowFlickerScale(NUVEC *);
    void rtlSetShadowFlickerBlendTime(f32);
    void rtlSetModifiers(f32 *, char **, i32);
    rtlset *rtlGetCurrentSet(void);
    void rtlSetMinR(f32);
    rtl_s *rtlAlloc(void);
    void rtlFree(rtl_s *);
    rtlfog_s *fogAlloc(void);
    void fogFree(rtlfog_s *);
    void rtlSetLights(rtldata_s *);
    void rtlSetSpecularLight(rtldata_s *);
    f32 rtlSpecularValue(rtldata_s *);
    void rtlSetSpecularValue(rtldata_s *, f32);
    rtlset *rtlLoadSet(char *, VARIPTR *, i32);
    burnset_s *edrtlBurnoutLoad(char *, VARIPTR *, i32);
    void rtlSaveSet(char *, rtlset *);
    void rtlProcessLights(void *, f32);
    rtlfog_s *rtlGetFogSet(rtlset *, NUVEC *);
    rtlfog_s *edrtlGetFogSet(void);
    void edrtlCalculateBurnoutEx(burnset_s *, NuBloomParameters *, NUVEC *, f32);
}
