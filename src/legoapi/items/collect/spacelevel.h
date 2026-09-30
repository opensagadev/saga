#pragma once

#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/nu3d/nuhspecial.h"

struct anakin_door_setup_s {
    char *name;
    char *secondary_name;
    f32 initial_offset;
    f32 speed;
    i16 flags;
    u8 unknown_12[2];
    NUVEC direction;
    f32 unknown_20;
    f32 minimum_offset;
};
DECOMP_ASSERT(sizeof(anakin_door_setup_s) == 0x28, "Anakin door setup size");
DECOMP_ASSERT(offsetof(anakin_door_setup_s, direction) == 0x14, "Anakin door direction offset");

struct anakin_door_s {
    NUMTX matrix;
    NUMTX secondary_matrix;
    NUMTX original_matrix;
    NUMTX original_secondary_matrix;
    NUVEC direction;
    f32 unknown_10c;
    nuhspecial_s special;
    nuhspecial_s secondary_special;
    f32 minimum_offset;
    f32 offset;
    f32 speed;
    f32 unknown_134;
    u8 active;
    u8 flags;
    i16 has_secondary;
    i16 platform_id;
    u8 unknown_13e[2];
};
DECOMP_ASSERT(sizeof(anakin_door_s) == 0x140, "Anakin door state size");
DECOMP_ASSERT(offsetof(anakin_door_s, special) == 0x110, "Anakin door special offset");
DECOMP_ASSERT(offsetof(anakin_door_s, active) == 0x138, "Anakin door active offset");

// The legacy level callbacks require storage for twelve door records.
extern anakin_door_s *volatile AnakinC;
extern anakin_door_setup_s DoorSetupList[15];

struct dogfight_door_s {
    char name[12];
    f32 distance;
    f32 timer;
};
DECOMP_ASSERT(sizeof(dogfight_door_s) == 0x14, "dogfight_door_s size");

struct dogfight_doors_s {
    dogfight_door_s doors[7];
    u32 unknown_08c;
};
DECOMP_ASSERT(sizeof(dogfight_doors_s) == 0x90, "dogfight_doors_s size");

struct spacelevel_action_config_s {
    f32 unknown_00;
    f32 unknown_04;
    i32 unknown_08;
    f32 unknown_0c;
    f32 unknown_10;
    f32 unknown_14;
    i32 unknown_18;
    f32 unknown_1c;
    f32 unknown_20;
    i32 unknown_24;
    f32 unknown_28;
    i32 unknown_2c;
};
DECOMP_ASSERT(sizeof(spacelevel_action_config_s) == 0x30, "spacelevel_action_config_s size");

struct anakin_action_config_s {
    spacelevel_action_config_s common;
    i32 unknown_30;
    f32 unknown_34;
    f32 unknown_38;
    i32 unknown_3c;
    u32 unknown_40[4];
};
DECOMP_ASSERT(sizeof(anakin_action_config_s) == 0x50, "anakin_action_config_s size");

extern spacelevel_action_config_s Actions_DogFightA;
extern anakin_action_config_s Actions_AnakinA;
extern dogfight_doors_s DogFightDoors;

struct starfighter_s {
    NUMTX matrix;
    u8 reserved_040[0xf0 - 0x40];
    f32 scale;
    u8 reserved_0f4[8];
    i16 draw_flags;
    i16 model_id;
    u8 reserved_100[0x110 - 0x100];
    i32 active;
    u8 reserved_114[0x128 - 0x114];
};
DECOMP_ASSERT(sizeof(starfighter_s) == 0x128, "starfighter runtime stride");
DECOMP_ASSERT(offsetof(starfighter_s, scale) == 0xf0, "starfighter draw scale");
DECOMP_ASSERT(offsetof(starfighter_s, model_id) == 0xfe, "starfighter model ID");
DECOMP_ASSERT(offsetof(starfighter_s, active) == 0x110, "starfighter active flag");

struct spacelevel_cross_s {
    u8 reserved_00[0x10];
    NUVEC4 local_position;
    u8 reserved_20[0x10];
    NUVEC4 world_position;
    u16 flags, enabled;
    u8 reserved_44[4];
    u32 colour;
    f32 scale;
};
DECOMP_ASSERT(sizeof(spacelevel_cross_s) == 0x50, "space cross stride");
DECOMP_ASSERT(offsetof(spacelevel_cross_s, enabled) == 0x42, "space cross enable offset");
DECOMP_ASSERT(offsetof(spacelevel_cross_s, scale) == 0x4c, "space cross scale offset");

struct spacelevel_flight_group_s {
    NUVEC4 position;
    starfighter_s fighters[5];
    NUVEC4 velocity;
    NUVEC4 camera_position;
    NUVEC4 target;
    u8 reserved_608[8];
    i32 active, state;
    u8 reserved_618[4];
    i32 draw_target;
    f32 duration, speed;
    u8 reserved_628[0x658 - 0x628];
};
DECOMP_ASSERT(sizeof(spacelevel_flight_group_s) == 0x658, "space flight group stride");
DECOMP_ASSERT(offsetof(spacelevel_flight_group_s, fighters) == 0x10, "space group fighters");
DECOMP_ASSERT(offsetof(spacelevel_flight_group_s, active) == 0x610, "space group active");
DECOMP_ASSERT(offsetof(spacelevel_flight_group_s, draw_target) == 0x61c, "space group target marker");

struct quickbolt_s {
    NUMTX matrix;
    NUVEC4 velocity;
    f32 duration;
    u8 reserved_54[4];
    i32 type;
    u8 reserved_5c[4];
};
DECOMP_ASSERT(sizeof(quickbolt_s) == 0x60, "quick bolt stride");
DECOMP_ASSERT(offsetof(quickbolt_s, duration) == 0x50, "quick bolt duration offset");
DECOMP_ASSERT(offsetof(quickbolt_s, type) == 0x58, "quick bolt model type offset");
struct quickboltinfo {
    quickbolt_s *bolts;
    i32 count, used;
};
DECOMP_ASSERT(sizeof(quickboltinfo) == 0xc, "quick bolt info size");

struct spacelevel_starfighter_s {
    i32 reset_state;
    i32 reset_target;
    u8 unknown_008[0x40 - 0x08];
    i32 reset_timer;
    u8 unknown_044[0x90 - 0x44];
    i32 reset_effect;
    u8 unknown_094[0x98 - 0x94];
    u32 reset_colour;
    i32 reset_effect_timer;
    u8 unknown_0a0[0x128 - 0x0a0];
};
DECOMP_ASSERT(sizeof(spacelevel_starfighter_s) == 0x128, "spacelevel_starfighter_s size");

struct spacelevel_last_starfighter_s {
    i32 reset_state;
    i32 reset_target;
    u8 unknown_008[0x40 - 0x08];
    i32 reset_timer;
    u8 unknown_044[0x90 - 0x44];
    i32 reset_effect;
    u8 unknown_094[0x98 - 0x94];
    u32 reset_colour;
    i32 reset_effect_timer;
    u8 unknown_0a0[0x0a8 - 0x0a0];
};
DECOMP_ASSERT(sizeof(spacelevel_last_starfighter_s) == 0x0a8, "spacelevel_last_starfighter_s size");

struct spacelevel_trooper_team_s {
    i32 reset_state;
    i32 reset_target;
    u8 unknown_008[0x40 - 0x08];
    i32 reset_timer;
    u8 unknown_044[0x90 - 0x44];
    i32 reset_effect;
    u8 unknown_094[0x98 - 0x94];
    u32 reset_colour;
    i32 reset_effect_timer;
    u8 unknown_0a0[0x1b8 - 0x0a0];
};
DECOMP_ASSERT(sizeof(spacelevel_trooper_team_s) == 0x1b8, "spacelevel_trooper_team_s size");

struct spacelevel_fighter_group_s {
    spacelevel_starfighter_s fighters[4];
    spacelevel_trooper_team_s trooper_team;
};
DECOMP_ASSERT(sizeof(spacelevel_fighter_group_s) == 0x658, "spacelevel_fighter_group_s size");

struct spacelevel_large_record_s {
    u8 unknown_000[0x404];
    f32 saved_value;
    f32 reset_value;
    u8 unknown_40c[0x514 - 0x40c];
    i32 saved_state;
    i32 reset_state;
    u8 unknown_51c[0x52c - 0x51c];
};
DECOMP_ASSERT(sizeof(spacelevel_large_record_s) == 0x52c, "spacelevel_large_record_s size");

struct spacelevel_scale_s {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
DECOMP_ASSERT(sizeof(spacelevel_scale_s) == 0x10, "spacelevel_scale_s size");

struct spacelevel_s {
    union {
        struct {
            NUMTX player_matrix;
            u16 player_matrix_flags;
            u16 player_matrix_state;
            u8 unknown_044[0x48 - 0x44];
            u32 player_colour;
            NUMTX camera_matrix;
            f32 camera_matrix_w;
            u16 camera_matrix_flags;
            u16 camera_matrix_state;
            u8 unknown_094[0x98 - 0x94];
            u32 camera_colour;
            f32 camera_value;
        };
        spacelevel_cross_s crosses[2];
    };
    union {
        struct {
            u8 unknown_0a0[0x1b0 - 0x0a0];
            spacelevel_fighter_group_s fighter_groups[7];
            spacelevel_starfighter_s final_fighters[4];
            spacelevel_last_starfighter_s last_starfighter;
            spacelevel_scale_s draw_scale;
            anakin_action_config_s *unknown_3370;
            i32 unknown_3374;
            i32 unknown_3378;
            i32 unknown_337c;
            u8 unknown_3380[0x338c - 0x3380];
            void *unknown_338c;
        };
        struct {
            u8 reserved_0a0[0xd0 - 0xa0];
            spacelevel_flight_group_s flight_groups[8];
        };
    };
    spacelevel_large_record_s large_records[256];
    union {
        spacelevel_starfighter_s queued_starfighters[96];
        starfighter_s queued_fighters[96];
    };
    u8 unknown_5ce90[0x62e90 - 0x5ce90];
    union {
        struct {
            void *reset_buffer;
            i32 reset_buffer_count;
            i32 reset_buffer_used;
        };
        quickboltinfo quick_bolts;
    };
    NUVEC player_origin;
    f32 player_origin_padding;
    NUVEC camera_origin;
    i32 unknown_62eb8;
    NUVEC direction;
    f32 unknown_62ec8;
    f32 direction_length;
    f32 inverse_direction_length;
    f32 unknown_62ed4;
    f32 door_time;
    f32 door_countdown;
    f32 door_elapsed;
    f32 normalized_speed;
    f32 value_one_a;
    f32 value_one_b;
    i32 unknown_62ef0;
    // The allocator reserves a further 4 KiB; its contents are not recovered.
    u8 unknown_62ef4[0x1000];
};
DECOMP_ASSERT(offsetof(spacelevel_s, fighter_groups) == 0x1b0, "fighter group array offset");
DECOMP_ASSERT(offsetof(spacelevel_s, flight_groups) == 0xd0, "space flight group array offset");
DECOMP_ASSERT(offsetof(spacelevel_s, large_records) == 0x3390, "large record array offset");
DECOMP_ASSERT(offsetof(spacelevel_s, queued_starfighters) == 0x55f90, "queued fighter array offset");
DECOMP_ASSERT(offsetof(spacelevel_s, reset_buffer) == 0x62e90, "reset buffer offset");
DECOMP_ASSERT(offsetof(spacelevel_s, direction) == 0x62ebc, "direction offset");
DECOMP_ASSERT(offsetof(spacelevel_s, normalized_speed) == 0x62ee4, "space normalized speed offset");
DECOMP_ASSERT(offsetof(spacelevel_s, unknown_62ef0) == 0x62ef0, "space allocator state offset");
DECOMP_ASSERT(sizeof(spacelevel_s) == 0x63ef4, "spacelevel_s allocation size");
