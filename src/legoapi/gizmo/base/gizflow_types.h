#pragma once

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
