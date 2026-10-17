#pragma once

#include "decomp.h"

// Types shared by legoapi/gizmo/base/gizflow.cpp and legoapi/gizmo/base/gizmo.cpp.

struct FLOWREMAP {
    i16 parents[16];
    i16 children[32];
    u8 parent_outputs[16];
};
struct FLOWCONDITIONTYPE {
    i32 type;
    char *name;
    nufpcomfn *load;
};
struct FLOWBOXTYPE {
    void (*reset)(GIZFLOW_s *, FLOWBOX_s *);
    i32 (*process)(GIZFLOW_s *, FLOWBOX_s *, u8);
    i32 (*check_output)(GIZFLOW_s *, FLOWBOX_s *, u8);
};

DECOMP_ASSERT(offsetof(FLOWREMAP, children) == 0x20, "Flow remap children offset");
DECOMP_ASSERT(offsetof(FLOWREMAP, parent_outputs) == 0x60, "Flow remap output offset");
DECOMP_ASSERT(sizeof(FLOWCONDITIONTYPE) == 12, "Flow condition type ABI");
DECOMP_ASSERT(sizeof(FLOWREMAP) == 0x70, "Flow remap ABI");
