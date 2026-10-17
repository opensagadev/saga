#pragma once

// Types shared by nu2api/nucore/numem.cpp and nu2api/nucore/nucore_includes.c.

struct NUMEMALLOCATION {
    NUMEMALLOCATION *next;
    u8 unknown_04[12];
    u32 size;
    char guard[12];
};
