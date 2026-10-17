#pragma once

#include "decomp.h"

// Types shared by legoapi/cutscenes/cutscene.cpp and legoapi/cutscenes/gcutscn.cpp.

// Name table passed as NuGCutSceneFixUp's third argument: count at +0x08 and 0x34-byte entries at +0x0c
// whose first field is the trigger target name (offsets from the original's trigger fix-up loop).
struct CutSceneTriggerNameEntry {
    char *name;
    u8 unknown_04[0x30];
};
struct CutSceneTriggerNameTable {
    u32 unknown_00[2];
    i32 count;
    CutSceneTriggerNameEntry *entries;
};

DECOMP_ASSERT(offsetof(CutSceneTriggerNameTable, entries) == 0x0c, "cutscene trigger name table entries offset");
DECOMP_ASSERT(sizeof(CutSceneTriggerNameEntry) == 0x34, "cutscene trigger name entry stride");
