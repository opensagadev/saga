#pragma once

#include "decomp.h"

struct PartHeader;
struct debinftype;
struct ACTIONINFO_s;
struct EXTRAACTIONDATA_s;
struct CHARACTER_CONTEXT_INFO_s;

extern ACTIONINFO_s *ActionInfo;
extern EXTRAACTIONDATA_s ExtraActionData[];
extern CHARACTER_CONTEXT_INFO_s *CInfo;
extern "C" void DebrisSetTimeIncrement(f32 increment);

extern "C" {
    extern PartHeader **DmaDebTypes;
    extern i32 EDPP_MAX_DMADEBTYPES;
    extern i32 freeDmaDebType;
    extern i32 edpp_types_used;
    extern debinftype **debtab;
}
