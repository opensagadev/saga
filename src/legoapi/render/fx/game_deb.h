#pragma once

#include "decomp.h"

struct PartHeader;
struct debinftype;

extern "C" {
extern PartHeader **DmaDebTypes;
extern i32 EDPP_MAX_DMADEBTYPES;
extern i32 freeDmaDebType;
extern i32 edpp_types_used;
extern debinftype **debtab;
}
