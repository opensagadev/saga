#pragma once

// Types shared by legoapi/render/core/terrain_stubs.cpp and legoapi/render/core/gameliball.cpp.

struct TERRAIN_PLATFORM_CALLBACK {
    void (*function)(void *);
    void *argument;
};
