#pragma once

#include "decomp.h"

// Types shared by legoapi/world/levels/episodeII.cpp and legoapi/characters/motion/move.cpp.

// This level's view of the shared 16-byte LevFlag scratch. byte0 holds the
// bonus-gunship milestone state; byte1 a secondary state.
enum GUNSHIP_STATE_e {
    GUNSHIP_INACTIVE = 0, // never entered
    GUNSHIP_ACTIVE = 1,   // player aboard / stage running
    GUNSHIP_WON = 2,      // stage finished
};
struct GUNSHIP_LEVFLAG_s {
    u8 progress; // 0x00 -> GUNSHIP_STATE_e
    u8 exit;     // 0x01
    u8 pad[14];  // 0x02
};
// Kamino disco-room state (original _ZL11kaminodisco).
struct KAMINODISCO_s {
    AIAREA *area;
    nuhspecial_s off[16];
    nuhspecial_s pending[16];
    nuhspecial_s occupied[16];
    nuhspecial_s complete[16];
    nuhspecial_s final[16];
    u8 tile_state[16];
    i8 tile_count;
    union {
        u8 phase;
        u8 mode;
    };
    i8 first_tile;
    i8 second_tile;
    f32 timer;
    u8 completion_sound;
    u8 reserved_3dd[3];
    GIZAIMESSAGE_s *next_tile_message;
    GIZAIMESSAGE_s *complete_message;
};
struct KAMINOC_PACKET_s {
    i16 off_mask;
    i16 pending_mask;
    i16 occupied_mask;
    i16 complete_mask;
    i16 final_mask;
    i16 sound_mask;
    u8 complete;
    u8 reserved;
};
struct DOOKU_C_s {
    GIZAIMESSAGE_s *total_message;
    GIZAIMESSAGE_s *hit_message;
    nuhspecial_s node;
};
struct KAMINO_E_s {
    GIZAIMESSAGE_s *jango_fight;
    GIZAIMESSAGE_s *can_fire;
    GIZAIMESSAGE_s *reset_turrets;
    GIZAIMESSAGE_s *show_hearts;
    GIZAIMESSAGE_s *minicut_started;
    CUTINFO *intro_cutscene;
    AIAREA *landing_area;
    nuhspecial_s slave1;
    GIZTURRET_s *turrets[4];
    GIZTURRET_s *active_turret;
    GIZPANEL_s *panels[4];
    NUVEC position;
    i32 pitch;
    i32 yaw;
    i32 roll;
    i32 orbit_pitch;
    i32 orbit_yaw;
    f32 departure_timer;
    f32 elapsed_time;
    i8 fire_index;
    u8 departing;
    i16 platform;
};
struct KaminoProgressGizmo_s {
    u8 reserved_00[0x98];
    u8 flags;
};
// One generated combatant slot. The record outlives its GameObject_s: a baddie
// that drifts outside the play planes is removed and its transform written back
// to `position` / `angle`, so the same slot can spawn it again once it is back
// in range.
struct JEDIB_CREATURE_s {
    nuvec_s position;          // 0x00, respawn position
    i32 angle;                 // 0x0c
    i32 id;                    // 0x10, character id, -1 picks one at spawn time
    GameObject_s *object;      // 0x14
    JEDIB_CREATURE_s *partner; // 0x18, opponent handed over when this slot has none
    AILOCATOR locator;         // 0x1c
    i32 field_0x58;            // 0x58
    // 0x5c. The bits are written individually (the original stores them as
    // bitfields); `flags` with JEDIB_CREATURE_FLAGS masks is for combined tests.
    union {
        u8 flags;
        struct {
            u8 spawned_behind : 1;
            u8 random_type : 1;
            u8 two_row_hp : 1;
            u8 in_wave : 1;
            u8 released : 1;
        };
    };
    u8 pad_0x5d[3];
};
enum JEDIB_CREATURE_FLAGS : u8 {
    JEDIB_CREATURE_SPAWNED_BEHIND = 0x01,
    JEDIB_CREATURE_RANDOM_TYPE = 0x02,
    JEDIB_CREATURE_TWO_ROW_HP = 0x04,
    JEDIB_CREATURE_IN_WAVE = 0x08,
    JEDIB_CREATURE_RELEASED = 0x10,
};
// A phase spawn list, terminated by a null id.
struct JEDIB_PHASE_s {
    i16 *id;
    char *script;
};
// Jedi_B arena state (original _ZL6jedi_b).
struct JEDIB_s {
    JEDIB_CREATURE_s baddies[256];       // 0x0000
    JEDIB_CREATURE_s goodies[8];         // 0x6000
    i16 baddie_count;                    // 0x6300
    i16 goody_count;                     // 0x6302
    i16 phase;                           // 0x6304
    i16 state;                           // 0x6306
    f32 timer;                           // 0x6308
    u32 seed;                            // 0x630c
    nuhspecial_s hero_spawns[3][4];      // 0x6310
    GameObject_s *heroes[3];             // 0x63a0
    GIZAIMESSAGE_s *msg_phase;           // 0x63ac
    GIZAIMESSAGE_s *msg_phase_complete;  // 0x63b0
    GIZAIMESSAGE_s *msg_objectives_left; // 0x63b4
    GIZAIMESSAGE_s *msg_restrain[3];     // 0x63b8
    i16 wave_ids[6];                     // 0x63c4
    char wave_spawned[6];                // 0x63d0
    u8 pad_0x63d6[2];
    GameObject_s *boss; // 0x63d8
    f32 wave_timer;     // 0x63dc
    // One bit per player that generated baddies must not target, OR'd into each
    // spawned baddie's own exclusion mask.
    union SAGA_HOST_PACKED_ALIGN4 {
        u64 exclusion_mask; // 0x63e0
        struct {
            u32 exclusion_mask_low;
            u32 exclusion_mask_high;
        };
    };
    u8 unique_spawned; // 0x63e8, the one-off Luminara / Shaak Ti slots
    u8 pad_0x63e9[3];
};
// Panel state the host mirrors to clients (jedib_netpacket, SetLevelHack slot of 0x20 bytes).
struct JEDIBNETPACKET_s {
    i16 state; // 0x00
    i16 phase; // 0x02
    i16 count; // 0x04
    i16 pad_0x06;
    i16 ids[8];      // 0x08
    char spawned[8]; // 0x18
};

DECOMP_ASSERT(sizeof(DOOKU_C_s) == 20, "Dooku C state ABI");
DECOMP_ASSERT(sizeof(JEDIBNETPACKET_s) == 0x20, "JEDIBNETPACKET_s ABI");
DECOMP_ASSERT(sizeof(JEDIB_CREATURE_s) == 0x60, "JEDIB_CREATURE_s ABI");
DECOMP_ASSERT(sizeof(JEDIB_s) == 0x63ec, "JEDIB_s ABI");
DECOMP_ASSERT(sizeof(KAMINODISCO_s) == 1000, "Kamino disco original layout");
DECOMP_ASSERT(sizeof(KAMINO_E_s) == 120, "Kamino E state ABI");
static_assert(sizeof(struct GUNSHIP_LEVFLAG_s) == 16, "LevFlag must be 16 bytes");
