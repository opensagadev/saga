#pragma once

#include "decomp.h"

// Types shared by legoapi/world/levels/hub.cpp and legoapi/ai/game/lsw_hub_ai.cpp.

enum HUB_DOOR_MENU_ID {
    HUB_DOOR_MENU_STANDARD = 15,
    HUB_DOOR_MENU_BONUS = 16,
    HUB_DOOR_MENU_VEHICLE = 17,
    HUB_DOOR_MENU_EPISODE_BONUS = 18,
};
enum HUB_VEHICLE_COLLECTION_MASK : u32 {
    HUB_VEHICLE_COLLECTION_ALLOWED = 0x04002000,
    HUB_VEHICLE_COLLECTION_REQUIRED = 0x00002000,
};
enum HUB_FREEPLAY_MODEL_FLAGS : u32 {
    HUB_FREEPLAY_MODEL_VEHICLE = 0x00002000,
    HUB_FREEPLAY_MODEL_MINIKIT = 0x04000000,
};
enum HUB_AUDIO_EVENT {
    HUB_AUDIO_EVENT_OPEN_DOOR_MENU = 0x2d,
};
enum {
    HUB_VEHICLE_ID_CAPACITY = 344,
};
struct HUBAREAINFO_s {
    const char *area_name;
    const char *door_name;
    const char *lock_name;
    const char *bonus_gizmo_name;
    const char *bonus_gizmo_name_2;
    f32 panel_offset;
    f32 panel_scale;
    u8 flags;
    u8 force_open;
    AREADATA_s *area;
    GIZMO *door;
    nuhspecial_s lock;
    GIZMO *bonus_gizmo;
    GIZMO *bonus_gizmo_2;
};
struct HUBEPISODEINFO_s {
    i16 episode;
    i16 flags;
    const char *door_name;
    const char *lock_on_name;
    const char *lock_off_name;
    u8 force_open;
    EPISODEDATA *data;
    GIZMO *door;
    nuhspecial_s lock_on;
    nuhspecial_s lock_off;
};
struct ARCADE_LEVEL_s {
    AREADATA **area;
    i16 *character_id;
};

DECOMP_ASSERT(offsetof(HUBAREAINFO_s, area) == 0x20, "HUBAREAINFO_s area offset");
DECOMP_ASSERT(offsetof(HUBAREAINFO_s, force_open) == 0x1d, "HUBAREAINFO_s force_open offset");
DECOMP_ASSERT(sizeof(HUBAREAINFO_s) == 0x3c, "HUBAREAINFO_s size");
DECOMP_ASSERT(sizeof(HUBEPISODEINFO_s) == 0x34, "HUBEPISODEINFO_s size");
