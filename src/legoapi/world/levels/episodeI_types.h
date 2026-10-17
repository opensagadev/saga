#pragma once

// Types shared by legoapi/world/levels/episodeI.cpp and legoapi/characters/motion/move.cpp.

// This level's view of the shared 16-byte LevFlag scratch. byte0 holds the
// pod-race progress state, byte1 the mushroom-collapse state.
struct LEVFLAGBYTES_s {
    u8 podrace_state;  // 0x00
    u8 mushroom_state; // 0x01
    u8 pad[14];        // 0x02
};
// Pacemaker display data stored at LevObjs[0] for the pacemaker object.
struct PACEMAKERDATA_s {
    char pad_0x00[0x1340];
    u32 color1; // 0x1340
    u32 color2; // 0x1344
    u32 color3; // 0x1348
    char pad_0x134c[0x134e - 0x134c];
    u8 enabled; // 0x134e
};
// AI locator view used as gungan spawn groups (AIPathFindLocator results:
// position at 0x10, rotation at 0x1c). Overlays AILOCATOR_s.
struct GUNGAN_GROUP_s {
    char pad_0x00[0x10];
    NUVEC pos;             // 0x10
    i32 rot;               // 0x1c
    AIPATHINFO_s pathinfo; // 0x20 embedded route cursor
};
// WORLDINFO::lev_objs uses the engine's 16-byte special-entry layout.
struct WORLDLEVOBJ_s {
    void *special; // 0x00
    char pad_0x04[0xa];
    u8 enabled; // 0x0e
    char pad_0x0f[1];
};

static_assert(sizeof(struct LEVFLAGBYTES_s) == 16, "LevFlag must be 16 bytes");
