#include "legoapi/gizmo/base/gizmo.h"
struct instNUGCUTSCENE_s;
extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *);
GIZMO *GizmoFindByData(GIZMOSYS *, i32, void *);
extern i32 addbolt_nosfx;
#include "legoapi/gizmo/base/gizmo.h"
extern nuhspecial_s disco_on_spin[3], walllights_disco[2], walllights[2], striplights[2], discolights[2],
    discorm_wall_off, discorm_wall_on;
extern GIZMO *disco_off_spina[3], *gizTurrets[2];
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include <stdio.h>
void ClearAICreatures();
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"

#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include <string.h>
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
static_assert(sizeof(struct GUNSHIP_LEVFLAG_s) == 16, "LevFlag must be 16 bytes");
extern struct GUNSHIP_LEVFLAG_s LevFlag;

// --- Cross-file entry points / id globals ---
//
// AIPAthFindPathCnx remains local here because it is called with a different
// argument arity than in episodeI (both are byte-matched as-is), so it cannot
// live in a shared header.

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH *, void *, void *, void *); // legoapi/ai pathfinding
}

// --- File-local statics (original _ZL... symbols; not renamed) ---------------

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
DECOMP_ASSERT(sizeof(KAMINODISCO_s) == 1000, "Kamino disco original layout");
static KAMINODISCO_s kaminodisco;
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
KAMINOC_PACKET_s *kaminoc_netpacket;

nuhspecial_s disco_on_spin[3];
GIZMO *disco_off_spina[3];

static GIZAIMESSAGE_s *dooku_c; // _ZL7dooku_c
struct dooku_state_s {
    i32 hit_message;
    nuhspecial_s node;
};
static dooku_state_s dooku_state;

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
DECOMP_ASSERT(sizeof(KAMINO_E_s) == 120, "Kamino E state ABI");
static KAMINO_E_s kamino_e;

static void *pursuit_state[0x20]; // bounty-hunter pursuit state
static i16 gunship_bolts[2];      // gun-ship bolt type ids
static u8 gunship_flags[0xa];     // gun-ship weapon-select flags
static void *gunship_weapons[4];  // gun-ship gizmo weapons

struct ZAMARROW_s {
    GameObject_s *object;
    f32 timer;
};

struct PURSUIT_TRAFFIC_ENTRY_s {
    nuhspecial_s special;
    i32 platform_id;
    f32 animation_frame;
    u8 pad_0x14[0x139 - 0x14];
    u8 active;
    i8 direction;
    u8 pad_0x13b[0x140 - 0x13b];
};

struct PURSUIT_TRAFFIC_s {
    PURSUIT_TRAFFIC_ENTRY_s entries[95];
    u8 pad_0x76c0[0x77e0 - 0x76c0];
    i8 count;
    u8 pad_0x77e1[2];
    i8 side;
};

struct PURSUIT_ARROW_COLOURS_s {
    u8 pad_0x0000[0x1340];
    u32 colour[3];
    u8 pad_0x134c[2];
    u8 enabled;
};

ZAMARROW_s zamarrow;
i32 pursuit_c_hack = 1;
f32 traffic_test_z = -360.0f;

extern i16 id_PADMECLAWED, id_BATTLEDROIDSECURITY, id_SUPERBATTLEDROID, id_JARJAR, id_LUMINARA, id_SHAAKTI,
    id_BATTLEDROIDGEONOSIAN;
void RemoveGameObject(GameObject_s *, i32);
#include "legoapi/world/mission.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/numath/nurand.h"
i32 OnOrInsidePlane(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32 *);
void CompleteLevel(WORLDINFO_s *);
i32 jedib_seed = 17;
void *jedib_netpacket;
f32 jedib_create_step_Normal = 2.0f;
f32 jedib_create_step_LowEnd = 3.0f;
f32 jedib_outer_r = 8.0f;
f32 jedib_offset = 0.5f;
f32 jedib_inner = 3.0f;
f32 jedib_proximity = 0.5f;
i32 jedib_min_baddies_per_goody = 1;
i32 jedib_max_baddies_per_goody = 3;
f32 jedib_safe_r = 9.0f;
f32 jedib_exclusion_r = 3.0f;
i32 jedib_n_active;
i32 jedib_n_drawn;
i16 *JediB_playerids[3] = {&id_PADMECLAWED, &id_ANAKINPADAWAN, &id_OBIWANKENOBIJEDIMASTER};
NUVEC jedib_offsets[3] = {{0.10000000149011612f, -0.10000000149011612f, -0.10000000149011612f},
                          {-0.10000000149011612f, -0.10000000149011612f, -0.10000000149011612f},
                          {-0.10000000149011612f, -0.10000000149011612f, 0.10000000149011612f}};
i32 jedib_yrot[3] = {24576, 40960, 57344};
struct JEDIB_PHASE_s {
    i16 *model;
    char *script;
};
static JEDIB_PHASE_s jedi_b_phase1[8] = {
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_BATTLEDROIDSECURITY, const_cast<char *>("phase_droids")},
    {&id_BATTLEDROIDSECURITY, const_cast<char *>("phase_droids")},
    {&id_BATTLEDROIDSECURITY, const_cast<char *>("phase_droids")},
    {&id_BATTLEDROIDSECURITY, const_cast<char *>("phase_droids")},
    {&id_BATTLEDROIDSECURITY, const_cast<char *>("phase_droids")},
    {NULL, NULL},
};
static JEDIB_PHASE_s jedi_b_phase2[8] = {
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {NULL, NULL},
};
static JEDIB_PHASE_s jedi_b_phase3[8] = {
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_SUPERBATTLEDROID, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {&id_DROIDEKA, const_cast<char *>("phase_droids")},
    {NULL, NULL},
};

#include "gameapi/ai/aisys/aipath.h"
struct JEDIB_ENTRY_s {
    NUVEC position; // 0x00
    i32 rotation;
    i32 model_id;
    GameObject_s *object;
    JEDIB_ENTRY_s *linked_entry;
    AILOCATOR locator; // 0x1c
    u32 model_state;   // 0x58
    u8 flags;
    u8 padding_0x5d[3];
};
struct JEDIB_STATE_s {
    JEDIB_ENTRY_s entries[264];
    i16 count; // 0x6300
    i16 objective_count;
    i16 phase;
    i16 phase_state;
    f32 timer;
    u32 random_seed;
    nuhspecial_s restraint_specials[3][4]; // 0x6310
    GameObject_s *party[3];                // 0x63a0
    GIZAIMESSAGE_s *phase_message;
    GIZAIMESSAGE_s *phase_complete_message;
    GIZAIMESSAGE_s *objectives_message;
    GIZAIMESSAGE_s *restraint_messages[3];
    i16 target_models[6]; // 0x63c4
    u8 target_complete[6];
    u8 padding_0x63d6[2];
    GameObject_s *jango; // 0x63d8
    f32 create_timer;
    // Keep the original four-byte ABI alignment on targets that align u64 to eight.
    union __attribute__((packed, aligned(4))) {
        u64 target_mask;
        u32 target_masks[2];
    };
    u8 replacement_unlock_mask;
    u8 padding_0x63e9[3];
};
DECOMP_ASSERT(sizeof(JEDIB_ENTRY_s) == 0x60, "JediB entry ABI");
DECOMP_ASSERT(offsetof(JEDIB_STATE_s, count) == 0x6300, "JediB state count ABI");
DECOMP_ASSERT(offsetof(JEDIB_STATE_s, party) == 0x63a0, "JediB party ABI");
DECOMP_ASSERT(sizeof(JEDIB_STATE_s) == 0x63ec, "JediB state ABI");
static JEDIB_STATE_s jedi_b;
static void JediBKilledCallback(GameObject_s *object);

// Episode 2 level handlers, in the game's Episode_II progression:
// pursuit (coruscant bounty-hunter) / kamino / factory (geonosis droid
// factory) / jedi / gunship / bonus gunship / dooku, then the NewTown bonus.
//
// The bounty-hunter pursuit and bonus-gunship functions came from a separate
// pursuit.cpp translation unit in the original binary (_GLOBAL__sub_I_pursuit.cpp);
// JediB currently lives here per the Episode II level grouping.

// ===========================================================================
// Coruscant — bounty-hunter pursuit (Zam Wesell)
// ===========================================================================

void BountyHunterPursuitA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "Jango")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "b1")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "b2")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "b3")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "b4")) != NULL)
        b->field_0x9f |= 0x20;
}

void BountyHunterPursuitB_Init(WORLDINFO_s *) {
}

void BountyHunterPursuitC_Init(WORLDINFO_s *) {
}

void BountyHunterPursuitD_Init(WORLDINFO_s *) {
}

void BountyHunterPursuitA_Reset(WORLDINFO_s *world) {
    pursuit_state[0] = 0;
    pursuit_state[1] = 0;
    pursuit_state[0] = GetNamedGameObject(world->ai_sys, "pursuit_a");
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "za1")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "za2")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "za3")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "za4")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "za5")) != NULL)
        b->field_0x9f |= 0x20;
}

void BountyHunterPursuitB_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_1");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_2");
    LevGizmo[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_3");
    LevGizmo[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_4");
    LevGizmo[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_5");
    LevGizmo[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitb_6");
    pursuit_state[0] = GetNamedGameObject(world->ai_sys, "pursuitb_exit");
}

static void UpdateZamArrow(WORLDINFO_s *world) {
    NUVEC position = zamarrow.object->apiobj.upper_position;
    position.y += 1.0f;
    GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
        AddGameMessage(const_cast<char *>(" "), &position, 0.05f, NULL, 0.0f, 255, 63, 63, 0x10083, 0.0f));
    if (message == NULL) {
        return;
    }

    message->icon = 0x134;
    message->alpha = static_cast<u8>(NU_SIN_LUT(static_cast<i32>(zamarrow.timer * 16384.0f)) * 128.0f);
    PURSUIT_ARROW_COLOURS_s *colours = reinterpret_cast<PURSUIT_ARROW_COLOURS_s *>(world->lev_objs);
    if (colours->enabled != 0) {
        message->color1 = colours->colour[0];
        message->color2 = colours->colour[1];
        message->color3 = colours->colour[2];
    }
}

void BountyHunterPursuitC_Reset(WORLDINFO_s *world) {
    zamarrow = {};
    zamarrow.object = GetNamedGameObject(world->ai_sys, const_cast<char *>("ai_zam"));

    PURSUIT_TRAFFIC_s *traffic = reinterpret_cast<PURSUIT_TRAFFIC_s *>(world->trafficanim_sys);
    if (traffic == NULL) {
        return;
    }

    traffic->side = 0;
    PURSUIT_TRAFFIC_ENTRY_s *entry = traffic->entries;
    for (i32 i = 0; i < traffic->count; ++i, ++entry) {
        if (pursuit_c_hack == 0) {
            entry->direction = 0;
            continue;
        }

        NUMTX first;
        NUMTX last;
        EvalAnim(&entry->special, 1.0f, &first, 1);
        EvalAnim(&entry->special, entry->animation_frame, &last, 1);
        if (traffic_test_z > first.m32) {
            entry->direction = traffic_test_z > last.m32 ? -1 : 0;
        } else {
            entry->direction = last.m32 > traffic_test_z ? 1 : 0;
        }
    }
}

void BountyHunterPursuitD_Reset(WORLDINFO_s *world) {
    pursuit_state[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_1");
    pursuit_state[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_2");
    pursuit_state[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_3");
    pursuit_state[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_4");
    pursuit_state[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_5");
    pursuit_state[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_6");
    pursuit_state[6] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_7");
    pursuit_state[7] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_8");
    pursuit_state[8] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_9");
    pursuit_state[9] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_10");
    pursuit_state[10] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_11");
    pursuit_state[11] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "pursuitd_12");
    pursuit_state[12] = GetNamedGameObject(world->ai_sys, "pursuitd_last");
}

void BountyHunterPursuitA_Update(WORLDINFO_s *world) {
    if (zamarrow.object != NULL && (zamarrow.object->apiobj.field_0x1f8 & 0x1000) != 0 &&
        zamarrow.object->apiobj.field_0x287 == 0) {
        if (FadeSys.fade == 0.0f && pause_rndr_on == 0) {
            zamarrow.timer = MIN(1.0f, zamarrow.timer + FRAMETIME + FRAMETIME);
            if (0.1f > NuFmod(GameTimer.time_elapsed_mod_seconds, 0.2f)) {
                UpdateZamArrow(world);
            }
        } else {
            zamarrow.timer = 0.0f;
        }
    }
}

void BountyHunterPursuitB_Update(WORLDINFO_s *world) {
    if (LevAIMessage[0] == NULL || LevAIMessage[0]->value != 1.0f || zamarrow.object == NULL ||
        (zamarrow.object->apiobj.field_0x1f8 & 0x1000) == 0 || zamarrow.object->apiobj.field_0x287 != 0) {
        return;
    }

    if (FadeSys.fade == 0.0f && pause_rndr_on == 0) {
        zamarrow.timer = MIN(1.0f, zamarrow.timer + FRAMETIME + FRAMETIME);
        if (0.1f > NuFmod(GameTimer.time_elapsed_mod_seconds, 0.2f)) {
            UpdateZamArrow(world);
        }
    } else {
        zamarrow.timer = 0.0f;
    }
}

void BountyHunterPursuitC_Update(WORLDINFO_s *world) {
    if (zamarrow.object != NULL && (zamarrow.object->apiobj.field_0x1f8 & 0x1000) != 0 &&
        zamarrow.object->apiobj.field_0x287 == 0) {
        if (FadeSys.fade == 0.0f && pause_rndr_on == 0) {
            zamarrow.timer = MIN(1.0f, zamarrow.timer + FRAMETIME + FRAMETIME);
            if (0.1f > NuFmod(GameTimer.time_elapsed_mod_seconds, 0.2f)) {
                UpdateZamArrow(world);
            }
        } else {
            zamarrow.timer = 0.0f;
        }
    }

    PURSUIT_TRAFFIC_s *traffic = reinterpret_cast<PURSUIT_TRAFFIC_s *>(world->trafficanim_sys);
    if (traffic == NULL || player == NULL) {
        return;
    }

    if (traffic_test_z <= player->apiobj.position.z) {
        if (traffic->side == 1) {
            return;
        }
        traffic->side = 1;
    } else {
        if (traffic->side == -1) {
            return;
        }
        traffic->side = -1;
    }

    PURSUIT_TRAFFIC_ENTRY_s *entry = traffic->entries;
    for (i32 i = 0; i < traffic->count; ++i, ++entry) {
        if (traffic_test_z > player->apiobj.position.z) {
            entry->active = entry->direction == 1;
        } else {
            entry->active = entry->direction == -1;
        }
    }
}

void BountyHunterPursuitD_Update(WORLDINFO_s *) {
}

// ===========================================================================
// Kamino
// ===========================================================================

i32 KaminoInside() {
    if (WORLD->area != NULL && WORLD->area == KAMINO_ADATA) {
        if (WORLD->current_level == KAMINOA_LDATA) {
            if (CUTSTOPGAME == 0) {
                if (GameCam->sock_position.location.sock == 5)
                    return 1;
                if (GameCam->sock_position.location.sock == 6)
                    return 1;
            } else {
                return 1;
            }
        } else if (WORLD->current_level == KAMINOE_LDATA) {
            if (CUTSTOPGAME == 0 && GameCam->sock_position.location.sock != 0x1e)
                return 1;
        }
    }
    return 0;
}

i32 KaminoDiscoOn() {
    return kaminodisco.mode == 2;
}

i32 KaminoInDiscoRoom() {
    i32 r = 0;
    if (WORLD->current_level == KAMINOC_LDATA)
        r = (GameCam->sock_position.location.sock == 0x15);
    return r;
}

void KaminoA_AlwaysUpdate(WORLDINFO_s *) {
    bool v = 0;
    if (CUTSTOPGAME == 0) {
        u8 b = GameCam->sock_position.location.sock;
        if (b != 5)
            v = (b != 6);
    }
    object_switches[1] = v;
}

#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/gizaimessage.h"
GIZTURRET_s *GizTurret_FindByName(GIZTURRETSYS_s *, char *);
extern i32 obstacle_gizmotype_id;
nuhspecial_s walllights[2], walllights_disco[2], striplights[2], discolights[2];
nuhspecial_s discorm_wall_on, discorm_wall_off;
GIZMO *gizTurrets[2];
i32 last;

void KaminoC_Init(WORLDINFO_s *world) {
    char name[32];
    memset(&kaminodisco, 0, sizeof(kaminodisco));
    kaminoc_netpacket = static_cast<KAMINOC_PACKET_s *>(SetLevelHack(sizeof(KAMINOC_PACKET_s)));
    kaminodisco.tile_count = 0;
    do {
        if (kaminodisco.tile_count < 9)
            sprintf(name, "dot_off_0%d", kaminodisco.tile_count + 1);
        else
            sprintf(name, "dot_off_%d", kaminodisco.tile_count + 1);
        NuSpecialFind(WORLD->current_gscn, &kaminodisco.off[kaminodisco.tile_count], name, 1);
        if (kaminodisco.tile_count < 9)
            sprintf(name, "dot_flash_0%d", kaminodisco.tile_count + 1);
        else
            sprintf(name, "dot_flash_%d", kaminodisco.tile_count + 1);
        NuSpecialFind(WORLD->current_gscn, &kaminodisco.pending[kaminodisco.tile_count], name, 1);
        if (kaminodisco.tile_count < 9)
            sprintf(name, "dot_select_0%d", kaminodisco.tile_count + 1);
        else
            sprintf(name, "dot_select_%d", kaminodisco.tile_count + 1);
        NuSpecialFind(WORLD->current_gscn, &kaminodisco.occupied[kaminodisco.tile_count], name, 1);
        if (kaminodisco.tile_count < 9)
            sprintf(name, "dot_finish_0%d", kaminodisco.tile_count + 1);
        else
            sprintf(name, "dot_finish_%d", kaminodisco.tile_count + 1);
        NuSpecialFind(WORLD->current_gscn, &kaminodisco.complete[kaminodisco.tile_count], name, 1);
        if (kaminodisco.tile_count < 9)
            sprintf(name, "dot_on_0%d", kaminodisco.tile_count + 1);
        else
            sprintf(name, "dot_on_%d", kaminodisco.tile_count + 1);
        NuSpecialFind(WORLD->current_gscn, &kaminodisco.final[kaminodisco.tile_count], name, 1);
        if (!NuSpecialExistsFn(&kaminodisco.off[kaminodisco.tile_count]) ||
            !NuSpecialExistsFn(&kaminodisco.pending[kaminodisco.tile_count]) ||
            !NuSpecialExistsFn(&kaminodisco.occupied[kaminodisco.tile_count]) ||
            !NuSpecialExistsFn(&kaminodisco.complete[kaminodisco.tile_count]))
            break;
        ++kaminodisco.tile_count;
    } while (kaminodisco.tile_count < 16);
    kaminodisco.area = AISysFindArea(WORLD->ai_sys, "DISCO");
    NuSpecialFind(WORLD->current_gscn, &walllights[0], "walllights1", 1);
    NuSpecialFind(WORLD->current_gscn, &walllights[1], "walllights2", 1);
    NuSpecialFind(WORLD->current_gscn, &walllights_disco[0], "walllights1_disco", 1);
    NuSpecialFind(WORLD->current_gscn, &walllights_disco[1], "walllights2_disco", 1);
    NuSpecialFind(WORLD->current_gscn, &striplights[0], "striplights1", 1);
    NuSpecialFind(WORLD->current_gscn, &striplights[1], "striplights1b", 1);
    NuSpecialFind(WORLD->current_gscn, &discolights[0], "discolight1", 1);
    NuSpecialFind(WORLD->current_gscn, &discolights[1], "discolight2", 1);
    NuSpecialFind(WORLD->current_gscn, &discorm_wall_on, "discorm_wall_on", 1);
    NuSpecialFind(WORLD->current_gscn, &discorm_wall_off, "discorm_wall_off", 1);
    sprintf(name, "disco_on_spin%d", 3);
    NuSpecialFind(WORLD->current_gscn, &disco_on_spin[0], name, 1);
    if (!netclient) {
        sprintf(name, "disco_off%d", 1);
        disco_off_spina[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, name);
    }
    sprintf(name, "disco_on_spin%d", 4);
    NuSpecialFind(WORLD->current_gscn, &disco_on_spin[1], name, 1);
    if (!netclient) {
        sprintf(name, "disco_off%d", 2);
        disco_off_spina[1] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, name);
    }
    sprintf(name, "disco_on_spin%d", 5);
    NuSpecialFind(WORLD->current_gscn, &disco_on_spin[2], name, 1);
    if (!netclient) {
        sprintf(name, "disco_off%d", 3);
        disco_off_spina[2] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, name);
    }
    if (!netclient) {
        gizTurrets[0] = GizmoFindByName(world->gizmo_sys, turret_gizmotype_id, "turret01");
        gizTurrets[1] = GizmoFindByName(world->gizmo_sys, turret_gizmotype_id, "turret02");
        GIZTURRET_s *turret = GizTurret_FindByName(world->giz_turret_sys, "turret01");
        if (turret)
            turret->field_0x140 = 0.6f;
        turret = GizTurret_FindByName(world->giz_turret_sys, "turret02");
        if (turret)
            turret->field_0x140 = 0.6f;
        LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizaimessage_gizmotype_id, "msg_KaminoCProgress");
        LevGizmo[1] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "JANGOFIELD01");
    }
    last = 0;
}

void KaminoC_Reset(WORLDINFO_s *world) {
    memset(kaminodisco.tile_state, 0, sizeof(kaminodisco.tile_state));
    kaminodisco.phase = 0;
    kaminodisco.first_tile = -1;
    kaminodisco.second_tile = -1;
    kaminodisco.timer = 0.0f;
    kaminodisco.completion_sound = 0;
    kaminoc_netpacket->pending_mask = 0;
    kaminoc_netpacket->complete_mask = 0;
    kaminoc_netpacket->occupied_mask = 0;
    kaminoc_netpacket->final_mask = 0;
    kaminoc_netpacket->off_mask = -1;
    kaminoc_netpacket->complete = 0;
    for (i32 i = 0; i < kaminodisco.tile_count; ++i) {
        NuSpecialSetVisibility(&kaminodisco.off[i], 1);
        NuSpecialSetVisibility(&kaminodisco.pending[i], 0);
        NuSpecialSetVisibility(&kaminodisco.complete[i], 0);
    }
    kaminodisco.next_tile_message = SetGizAIMessage(gizaimessagesys, "NextDiscoTile", 0.0f, NULL);
    kaminodisco.complete_message = SetGizAIMessage(gizaimessagesys, "DiscoComplete", 0.0f, NULL);
    for (i32 i = 0; i < 3; ++i) {
        NuSpecialSetVisibility(&disco_on_spin[i], 0);
        if (netclient == 0) {
            GizmoSetVisibility(world->gizmo_sys, disco_off_spina[i], 1, 0);
            if (static_cast<GIZFORCE_s *>(disco_off_spina[i]->object)->anim_set->state == 0)
                GizmoActivate(world->gizmo_sys, disco_off_spina[i], 1, 0);
        }
    }
}

static inline bool KaminoDiscoPlayerInArea(WORLDINFO_s *world) {
    APIOBJECT *first = world->ai_sys->player_1;
    if (first == NULL && world->ai_sys->player_2 == NULL)
        return false;
    if (kaminodisco.area == NULL)
        return false;
    i32 index = static_cast<i32>(kaminodisco.area - world->ai_sys->areas);
    u64 mask = static_cast<u64>(static_cast<i64>(static_cast<i32>(1u << (index & 31))));
    return (first->ai_area_mask & mask) != 0;
}
static inline i32 KaminoDiscoChooseTile(i32 excluded, u8 state) {
    i32 candidates[16], count = 0;
    for (i32 i = 0; i < kaminodisco.tile_count; ++i)
        if (i != excluded && kaminodisco.tile_state[i] == state)
            candidates[count++] = i;
    return count ? candidates[NuRand(NULL) % count] : -1;
}
static inline bool KaminoDiscoOccupied(GameObject_s *object, NUVEC *position) {
    if (object == NULL || (object->apiobj.object_flags & 0x1001) != 0x1001 || !object->apiobj.field_0x27d)
        return false;
    f32 x = position->x - object->apiobj.position.x;
    f32 y = position->y - object->apiobj.position.y;
    f32 z = position->z - object->apiobj.position.z;
    return (x * x + y * y) + z * z < 0.04000000283122063f;
}
struct KaminoProgressGizmo_s {
    u8 reserved_00[0x98];
    u8 flags;
};
void KaminoC_Update(WORLDINFO_s *world) {
    KaminoProgressGizmo_s *progress = static_cast<KaminoProgressGizmo_s *>(LevGizmo[0]->object);
    if (GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 0, 0)) {
        GizmoSetVisibility(world->gizmo_sys, LevGizmo[1], 0, 1);
    } else if (!(progress->flags & 1)) {
        GizmoActivate(world->gizmo_sys, LevGizmo[1], 1, 1);
        if (GizmoGetOutput(world->gizmo_sys, gizTurrets[0], 0, 0) &&
            GizmoGetOutput(world->gizmo_sys, gizTurrets[1], 0, 0))
            GizmoSetVisibility(world->gizmo_sys, LevGizmo[1], 0, 1);
    }
    kaminoc_netpacket->sound_mask = 0;
    SetGizAIMessage(gizaimessagesys, "NextDiscoTile", 0.0f, kaminodisco.next_tile_message);
    SetGizAIMessage(gizaimessagesys, "DiscoComplete", 0.0f, kaminodisco.complete_message);
    switch (kaminodisco.phase) {
        case 0:
            if (KaminoDiscoPlayerInArea(world)) {
                kaminoc_netpacket->complete = 0;
                kaminodisco.phase = 1;
                kaminodisco.timer = 0.0f;
                kaminodisco.first_tile = KaminoDiscoChooseTile(-1, 0);
                kaminodisco.second_tile = KaminoDiscoChooseTile(kaminodisco.first_tile, 0);
                if (kaminodisco.first_tile != -1 && kaminodisco.second_tile != -1) {
                    kaminodisco.tile_state[kaminodisco.first_tile] = 1;
                    kaminodisco.tile_state[kaminodisco.second_tile] = 1;
                }
            }
            break;
        case 1: {
            i32 occupied_by_player = -1;
            bool first_occupied = false;
            bool second_occupied = false;
            u8 old_state = kaminodisco.tile_state[kaminodisco.first_tile];
            kaminodisco.tile_state[kaminodisco.first_tile] = 1;
            NUVEC *position = NuSpecialGetPos(&kaminodisco.pending[kaminodisco.first_tile]);
            for (i32 i = 0; i < 8; ++i) {
                GameObject_s *object = Player[i];
                if (KaminoDiscoOccupied(object, position)) {
                    first_occupied = true;
                    if (object == player)
                        occupied_by_player = kaminodisco.first_tile;
                    kaminodisco.tile_state[kaminodisco.first_tile] = 2;
                    break;
                }
            }
            if (old_state != kaminodisco.tile_state[kaminodisco.first_tile] &&
                kaminodisco.tile_state[kaminodisco.first_tile] == 2)
                kaminoc_netpacket->sound_mask |= 1u << (kaminodisco.first_tile & 31);
            old_state = kaminodisco.tile_state[kaminodisco.second_tile];
            kaminodisco.tile_state[kaminodisco.second_tile] = 1;
            position = NuSpecialGetPos(&kaminodisco.pending[kaminodisco.second_tile]);
            for (i32 i = 0; i < 8; ++i) {
                GameObject_s *object = Player[i];
                if (KaminoDiscoOccupied(object, position)) {
                    second_occupied = true;
                    if (object == player)
                        occupied_by_player = kaminodisco.second_tile;
                    kaminodisco.tile_state[kaminodisco.second_tile] = 2;
                    break;
                }
            }
            if (old_state != kaminodisco.tile_state[kaminodisco.second_tile] &&
                kaminodisco.tile_state[kaminodisco.second_tile] == 2)
                kaminoc_netpacket->sound_mask |= 1u << (kaminodisco.second_tile & 31);
            if (first_occupied && second_occupied) {
                kaminodisco.timer = 0.0f;
                kaminodisco.tile_state[kaminodisco.first_tile] = 3;
                kaminodisco.tile_state[kaminodisco.second_tile] = 3;
                kaminodisco.first_tile = KaminoDiscoChooseTile(-1, 0);
                kaminodisco.second_tile = KaminoDiscoChooseTile(kaminodisco.first_tile, 0);
                if (kaminodisco.first_tile != -1 && kaminodisco.second_tile != -1) {
                    kaminodisco.tile_state[kaminodisco.first_tile] = 1;
                    kaminodisco.tile_state[kaminodisco.second_tile] = 1;
                } else {
                    kaminodisco.phase = 2;
                    for (i32 i = 0; i < kaminodisco.tile_count; ++i)
                        kaminodisco.tile_state[i] = 0;
                }
                break;
            }
            kaminodisco.timer += FRAMETIME;
            if (kaminodisco.timer > 2.5f) {
                kaminodisco.timer = 0.0f;
                for (i32 i = 0; i < 2; ++i) {
                    i32 tile = KaminoDiscoChooseTile(-1, 3);
                    if (tile != -1)
                        kaminodisco.tile_state[tile] = 0;
                }
            }
            if (player2 == NULL) {
                GameObject_s *companion = Player[0];
                if (companion == player)
                    companion = Player[1];
                if (companion != NULL) {
                    if (occupied_by_player == kaminodisco.first_tile)
                        SetGizAIMessage(gizaimessagesys, "NextDiscoTile", static_cast<f32>(kaminodisco.second_tile + 1),
                                        kaminodisco.next_tile_message);
                    else if (occupied_by_player == kaminodisco.second_tile)
                        SetGizAIMessage(gizaimessagesys, "NextDiscoTile", static_cast<f32>(kaminodisco.first_tile + 1),
                                        kaminodisco.next_tile_message);
                }
            }
            break;
        }
        case 2:
            SetGizAIMessage(gizaimessagesys, "DiscoComplete", 1.0f, kaminodisco.complete_message);
            kaminoc_netpacket->complete = 1;
            if (!KaminoDiscoPlayerInArea(world)) {
                KaminoC_Reset(world);
                return;
            }
            for (i32 i = 0; i < kaminodisco.tile_count; ++i)
                kaminodisco.tile_state[i] = 4;
            kaminodisco.timer += FRAMETIME;
            if (kaminodisco.timer > 20.0f) {
                KaminoC_Reset(world);
                return;
            }
            break;
    }
    kaminoc_netpacket->final_mask = 0;
    kaminoc_netpacket->off_mask = 0;
    kaminoc_netpacket->pending_mask = 0;
    kaminoc_netpacket->occupied_mask = 0;
    kaminoc_netpacket->complete_mask = 0;
    for (i32 i = 0; i < kaminodisco.tile_count; ++i) {
        switch (kaminodisco.tile_state[i]) {
            case 0:
                kaminoc_netpacket->off_mask |= 1u << (i & 31);
                break;
            case 1:
                kaminoc_netpacket->pending_mask |= 1u << (i & 31);
                break;
            case 2:
                kaminoc_netpacket->occupied_mask |= 1u << (i & 31);
                break;
            case 3:
                kaminoc_netpacket->final_mask |= 1u << (i & 31);
                break;
            case 4:
                kaminoc_netpacket->complete_mask |= 1u << (i & 31);
                break;
        }
    }
    for (i32 i = 0; i < kaminodisco.tile_count; ++i) {
        u32 bit = 1u << (i & 31);
        NuSpecialSetVisibility(&kaminodisco.off[i], kaminoc_netpacket->off_mask & bit);
        NuSpecialSetVisibility(&kaminodisco.pending[i], kaminoc_netpacket->pending_mask & bit);
        NuSpecialSetVisibility(&kaminodisco.occupied[i], kaminoc_netpacket->occupied_mask & bit);
        NuSpecialSetVisibility(&kaminodisco.final[i], kaminoc_netpacket->final_mask & bit);
        NuSpecialSetVisibility(&kaminodisco.complete[i], kaminoc_netpacket->complete_mask & bit);
        if (kaminoc_netpacket->sound_mask & bit)
            PlaySfx("Kam_DiscoFloorPanelOn",
                    reinterpret_cast<NUVEC *>(&NuSpecialGetDrawMtx(&kaminodisco.complete[i])->m30));
    }
    if (kaminoc_netpacket->complete) {
        for (i32 i = 0; i < 3; ++i) {
            NuSpecialSetVisibility(&disco_on_spin[i], 1);
            GizmoSetVisibility(world->gizmo_sys, disco_off_spina[i], 0, 0);
        }
        NuSpecialSetVisibility(&walllights_disco[0], 1);
        NuSpecialSetVisibility(&walllights_disco[1], 1);
        NuSpecialSetVisibility(&walllights[0], 0);
        NuSpecialSetVisibility(&walllights[1], 0);
        NuSpecialSetVisibility(&striplights[0], 0);
        NuSpecialSetVisibility(&striplights[1], 1);
        NuSpecialSetVisibility(&discolights[0], 1);
        NuSpecialSetVisibility(&discolights[1], 1);
        NuSpecialSetVisibility(&discorm_wall_off, 0);
        NuSpecialSetVisibility(&discorm_wall_on, 1);
        if (!kaminodisco.completion_sound) {
            PlaySfx("Kam_DiscoFloorPanelDone", NuSpecialGetDrawPos(&kaminodisco.off[3]));
            kaminodisco.completion_sound = 1;
        }
    } else {
        for (i32 i = 0; i < 3; ++i) {
            NuSpecialSetVisibility(&disco_on_spin[i], 0);
            GizmoSetVisibility(world->gizmo_sys, disco_off_spina[i], 1, 0);
        }
        NuSpecialSetVisibility(&walllights_disco[0], 0);
        NuSpecialSetVisibility(&walllights_disco[1], 0);
        NuSpecialSetVisibility(&walllights[0], 1);
        NuSpecialSetVisibility(&walllights[1], 1);
        NuSpecialSetVisibility(&striplights[0], 1);
        NuSpecialSetVisibility(&striplights[1], 0);
        NuSpecialSetVisibility(&discolights[0], 0);
        NuSpecialSetVisibility(&discolights[1], 0);
        NuSpecialSetVisibility(&discorm_wall_on, 0);
        NuSpecialSetVisibility(&discorm_wall_off, 1);
    }
}

void KaminoD_Init(WORLDINFO_s *world) {
    for (i32 i = 1; i < 13; i++) {
        char buf[0x10];
        sprintf(buf, "DOT%i", i);
        GIZOBSTACLE_s *g = GizObstacle_FindByName(world->giz_obstacle_sys, buf);
        if (g->field_0x3c != 0.0f) {
            break;
        }

        g->field_0x3c = 13.5f;
    }

    GIZMOBLOWUP_s *target = GizmoBlowUp_FindByName(world, "target_a11");
    if (target != NULL) {
        target->field_0x124 = 1;
    }
}

#include "legoapi/gizmos/object/gizpanel.h"
extern "C" i32 FindPlatInst(i32);

void KaminoE_Init(WORLDINFO_s *world) {
    kaminoe_netpacket = SetLevelHack(0x14);
    GIZMO_s *g = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "endblock_b08");
    if (g != NULL)
        LevForce = *(i32 *)g;
}

void KaminoE_Reset(WORLDINFO_s *world) {
    char name[16];
    memset(&kamino_e, 0, sizeof(kamino_e));
    kamino_e.landing_area = AISysFindArea(WORLD->ai_sys, "landing_pad");
    if (NuSpecialFind(world->current_gscn, &kamino_e.slave1, "slave1", 1))
        kamino_e.platform = FindPlatInst(NuSpecialGetInstanceix(&kamino_e.slave1));
    for (i32 i = 0; i < 4; ++i) {
        sprintf(name, "turret%d", i + 1);
        GIZMO *gizmo = GizmoFindByName(world->gizmo_sys, turret_gizmotype_id, name);
        if (gizmo) {
            kamino_e.turrets[i] = static_cast<GIZTURRET_s *>(gizmo->object);
            kamino_e.turrets[i]->field_0xe4 = &kamino_e.position;
            kamino_e.turrets[i]->field_0x12c = 2;
            kamino_e.turrets[i]->flags |= 1;
        }
        sprintf(name, "R4_t%d", i + 1);
        gizmo = GizmoFindByName(world->gizmo_sys, gizpanel_gizmotype_id, name);
        if (gizmo)
            kamino_e.panels[i] = static_cast<GIZPANEL_s *>(gizmo->object);
    }
    kamino_e.jango_fight = SetGizAIMessage(gizaimessagesys, "JangoFight", 0.0f, NULL);
    kamino_e.can_fire = SetGizAIMessage(gizaimessagesys, "Slave1CanFire", 0.0f, NULL);
    kamino_e.reset_turrets = SetGizAIMessage(gizaimessagesys, "ResetTurrets", 0.0f, NULL);
    kamino_e.show_hearts = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    kamino_e.minicut_started = CheckGizAIMessage(gizaimessagesys, "MiniCutStarted", NULL);
    CutScene_Find(world->cutscene_sys, "Ep2_Kamino_Intro2");
    kamino_e.intro_cutscene = CutScene_Find(world->cutscene_sys, "Ep2_Kamino_Intro2");
}

static NUVEC kamino_e_centre = {56.5f, -2.890000104904175f, 8.0f};
static NUVEC kamino_e_gunoffset[2] = {{0.2f, -1.1f, 0.9f}, {-0.2f, -1.1f, 0.9f}};
void KaminoE_Update(WORLDINFO_s *world) {
    if (kamino_e.reset_turrets != NULL && kamino_e.reset_turrets->value == 1.0f) {
        kamino_e.active_turret = NULL;
        for (i32 i = 0; i < 4; ++i) {
            GIZMO *gizmo = GizmoFindByData(WORLD->gizmo_sys, gizpanel_gizmotype_id, kamino_e.panels[i]);
            GizmoActivate(WORLD->gizmo_sys, gizmo, 1, 1);
            kamino_e.turrets[i]->flags &= ~0x10;
            kamino_e.turrets[i]->field_0x12e = 1;
        }
        kamino_e.reset_turrets->value = 0.0f;
    }
    if (netclient)
        return;
    GameObject_s *jango = FindGameObject(id_JANGOFETT, 1, 1, 1, 0);
    GameObject_s *other_jango = FindGameObject(id_JANGOFETT, 4, 1, 1, 0);
    if (jango != NULL) {
        if (jango->current_hp <= 0) {
            if (FreePlay)
                KillBossCompleteLevel(id_JANGOFETT, 0, 0.0f);
            else
                KillBossNewLevel(id_JANGOFETT, 0, 0.0f, KAMINOOUTRO_LDATA->idx);
        }
    } else
        jango = other_jango;
    i32 connection_direction = 0;
    if (kamino_e.jango_fight->value == 0.0f && !FreePlay && kamino_e.intro_cutscene != NULL &&
        instNuGCutSceneIsFinished(static_cast<instNUGCUTSCENE_s *>(kamino_e.intro_cutscene->instance)))
        kamino_e.jango_fight->value = 1.0f;
    if (kamino_e.active_turret != NULL && (kamino_e.active_turret->flags & 0x10))
        kamino_e.active_turret = NULL;
    NUMTX *matrix = NuSpecialGetDrawMtx(&kamino_e.slave1);
    NUVEC desired = {0.0f, 0.0f, 6.5f};
    NuVecRotateX(&desired, &desired, kamino_e.orbit_pitch);
    NuVecRotateY(&desired, &desired, kamino_e.orbit_yaw);
    NuVecAdd(&desired, &desired, &kamino_e_centre);
    kamino_e.elapsed_time += FRAMETIME;
    f32 degrees = kamino_e.elapsed_time * 360.0f;
    desired.y +=
        ((NuTrigTable[(static_cast<i32>((degrees / 3.0f) * 182.04444885253906f) >> 1) & 0x7fff] + 1.0f) * 0.5f) * 0.2f;
    desired.z +=
        ((NuTrigTable[(static_cast<i32>((0.25f * degrees) * 182.04444885253906f) >> 1) & 0x7fff] + 1.0f) * 0.5f) * 0.2f;
    if (kamino_e.minicut_started != NULL && kamino_e.minicut_started->value > 0.0f)
        NuSpecialSetVisibility(&kamino_e.slave1, 1);
    if (kamino_e.departing)
        PlaySfx("Slave1_EngineLp", &kamino_e.position);
    switch (kamino_e.departing) {
        case 0:
            if (kamino_e.jango_fight->value > 0.0f) {
                kamino_e.departing = 1;
                kamino_e.departure_timer = 0.0f;
                GizObstacle_FindByName(world->giz_obstacle_sys, "slave1_debris");
            }
            kamino_e.position.x = kamino_e_centre.x;
            kamino_e.position.y = -2.8f;
            kamino_e.position.z = kamino_e_centre.z;
            kamino_e.pitch = -0x4000;
            kamino_e.yaw = 0;
            kamino_e.orbit_pitch = -3640;
            kamino_e.orbit_yaw = 0x4000;
            break;
        case 1: {
            kamino_e.departure_timer += FRAMETIME;
            f32 ratio, base;
            if (kamino_e.departure_timer < 4.0f) {
                i32 angle =
                    static_cast<i32>(((0.25f * kamino_e.departure_timer) * 180.0f - 90.0f) * 182.04444885253906f);
                ratio = (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
                base = (1.0f - ratio) * -2.8f;
            } else {
                kamino_e.departure_timer = 0.0f;
                kamino_e.departing = 2;
                ratio = 1.0f;
                base = -0.0f;
            }
            kamino_e.position.y = ratio * desired.y + base;
            break;
        }
        case 2:
            kamino_e.departure_timer += FRAMETIME;
            if (kamino_e.departure_timer < 4.0f)
                kamino_e.pitch = SeekRot(kamino_e.pitch, 0, kamino_e.departure_timer);
            else {
                kamino_e.pitch = SeekRot(kamino_e.pitch, 0, 4.0f);
                kamino_e.departing = 3;
                kamino_e.departure_timer = 0.0f;
            }
            break;
        case 3:
            kamino_e.departure_timer += FRAMETIME;
            if (kamino_e.departure_timer < 4.0f)
                kamino_e.yaw = SeekRot(kamino_e.yaw, 0xc000, kamino_e.departure_timer);
            else {
                kamino_e.yaw = SeekRot(kamino_e.yaw, 0xc000, 4.0f);
                kamino_e.departure_timer = 0.0f;
                kamino_e.departing = 4;
            }
            kamino_e.pitch = SeekRot(kamino_e.pitch, 0, 4.0f);
            break;
        case 4: {
            kamino_e.departure_timer += FRAMETIME;
            f32 ratio;
            if (kamino_e.departure_timer < 4.0f) {
                i32 angle =
                    static_cast<i32>(((0.25f * kamino_e.departure_timer) * 180.0f - 90.0f) * 182.04444885253906f);
                ratio = (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
            } else {
                kamino_e.departure_timer = 0.0f;
                kamino_e.departing = 5;
                ratio = 1.0f;
            }
            kamino_e.position.x = desired.x * ratio + kamino_e_centre.x * (1.0f - ratio);
            kamino_e.position.z = desired.z * ratio + kamino_e_centre.z * (1.0f - ratio);
            kamino_e.pitch = SeekRot(kamino_e.pitch, 0xb60, 4.0f);
            kamino_e.yaw = SeekRot(kamino_e.yaw, 0xc000, 4.0f);
            break;
        }
        case 5: {
            NUVEC aim, delta;
            bool have_target = false;
            if (kamino_e.active_turret != NULL) {
                aim = *NuSpecialGetDrawPos(&kamino_e.active_turret->primary_anim_obj->special);
                aim.y += 0.5f;
                have_target = true;
            } else if (kamino_e.can_fire->value == 1.0f) {
                GameObject_s *nearest = NULL;
                f32 best = 1000000000.0f;
                for (i32 i = 0; i < 8; ++i) {
                    GameObject_s *object = Player[i];
                    if (object == NULL || (object->apiobj.object_flags & 0x1001) != 0x1001 ||
                        (object->apiobj.character_data->model_flags & 0x80000))
                        continue;
                    f32 distance = NuVecDistSqr(&object->apiobj.collision_position,
                                                reinterpret_cast<NUVEC *>(&matrix->m30), &delta);
                    if (distance < best) {
                        best = distance;
                        nearest = Player[i];
                    }
                }
                if (nearest != NULL) {
                    aim.x = nearest->apiobj.collision_position.x;
                    aim.y = 0.1f + nearest->apiobj.field_0x218;
                    aim.z = nearest->apiobj.collision_position.z;
                    have_target = true;
                }
            }
            i32 target_pitch, target_yaw;
            if (have_target) {
                f32 z = aim.z - kamino_e_centre.z;
                kamino_e.orbit_pitch = -3640;
                if (z > 2.5f)
                    z = 2.5f;
                else if (z < -2.5f)
                    z = -2.5f;
                kamino_e.orbit_yaw = static_cast<i32>(((z / 2.5f) * 30.0f) * 182.04444885253906f) + 0x4000;
                NUVEC offset = {0.0f, -kamino_e_gunoffset[0].y, -kamino_e_gunoffset[0].z};
                NuVecMtxRotate(&offset, &offset, matrix);
                NuVecAdd(&offset, &offset, &aim);
                NuVecSub(&delta, &offset, &kamino_e.position);
                target_yaw = static_cast<i32>(NuAtan2(delta.x, delta.z) * 10430.3779296875f);
                f32 horizontal = NuFsqrt(delta.x * delta.x + delta.z * delta.z);
                target_pitch = static_cast<i32>(NuAtan2(-delta.y, horizontal) * 10430.3779296875f);
                f32 timer = kamino_e.departure_timer + FRAMETIME;
                if (timer > 0.15f) {
                    NUMTX bolt_matrix = *matrix;
                    NUVEC origin;
                    kamino_e.departure_timer = 0.0f;
                    NuVecMtxTransform(&origin, &kamino_e_gunoffset[kamino_e.fire_index], matrix);
                    f32 distance = NuVecDist(&origin, &aim, NULL);
                    i32 adjustment = static_cast<i32>(NuAtan2(-kamino_e_gunoffset[kamino_e.fire_index].x, distance) *
                                                      10430.3779296875f);
                    NuMtxPreRotateY(&bolt_matrix, adjustment);
                    addbolt_nosfx = 1;
                    BOLT_s *bolt = Bolt_Add(NULL, &origin, &bolt_matrix, 0x27, 0x800);
                    if (bolt != NULL)
                        bolt->flags |= 0x10;
                    kamino_e.fire_index = !kamino_e.fire_index;
                    bolt_matrix = *matrix;
                    NuVecMtxTransform(&origin, &kamino_e_gunoffset[kamino_e.fire_index], matrix);
                    adjustment = static_cast<i32>(NuAtan2(-kamino_e_gunoffset[kamino_e.fire_index].x, distance) *
                                                  10430.3779296875f);
                    NuMtxPreRotateY(&bolt_matrix, adjustment);
                    addbolt_nosfx = 1;
                    bolt = Bolt_Add(NULL, &origin, &bolt_matrix, 0x27, 0x800);
                    if (bolt != NULL)
                        bolt->flags |= 0x10;
                    kamino_e.fire_index = !kamino_e.fire_index;
                    PlaySfx("Kam_Slave1BlasterFire", &origin);
                } else
                    kamino_e.departure_timer = timer;
            } else {
                kamino_e.orbit_pitch = -3640;
                kamino_e.orbit_yaw = 0x4000;
                target_pitch = 0xb60;
                target_yaw = 0xc000;
            }
            SeekVec(&kamino_e.position, &kamino_e.position, &desired, 1.0f);
            kamino_e.pitch = SeekRot(kamino_e.pitch, target_pitch, 1.0f);
            kamino_e.yaw = SeekRot(kamino_e.yaw, target_yaw, 1.0f);
            break;
        }
    }
    NuMtxSetTranslation(matrix, &kamino_e.position);
    NuMtxPreRotateY(matrix, kamino_e.yaw);
    NuMtxPreRotateX(matrix, kamino_e.pitch);
    i32 *connection =
        static_cast<i32 *>(AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, (void *)"Bridge1_a",
                                             (void *)"Bridge1_b", &connection_direction));
    AIAREA *fight_area = AISysFindArea(WORLD->ai_sys, "Fight");
    if (connection == NULL || jango == NULL || fight_area == NULL)
        return;
    if (connection[connection_direction] >= 0 && connection[!connection_direction] >= 0)
        return;
    i32 area_index = static_cast<i32>(fight_area - world->ai_sys->areas);
    u64 mask = static_cast<u64>(static_cast<i64>(static_cast<i32>(1u << (area_index & 31))));
    if (jango->apiobj.ai_area_mask & mask)
        return;
    AILOCATOR *wait = AIPathFindLocator(world->ai_sys, "WAIT");
    if (wait == NULL)
        return;
    jango->apiobj.position = wait->position;
    jango->apiobj.field_0x276 = wait->direction;
    jango->apiobj.facing_angle = wait->direction;
    jango->apiobj.movement_facing_angle = wait->direction;
    jango->ai.path_info = wait->path_info;
    jango->apiobj.initial_position = wait->position;
    jango->apiobj.collision_position = wait->position;
    plr_lastpos = wait->position;
    jango->apiobj.start_position = wait->position;
    jango->apiobj.respawn_position = wait->position;
    jango->apiobj.last_safe_position = wait->position;
    jango->saved_position = wait->position;
    jango->apiobj.velocity = v000;
    InitSurfaceInfo(jango);
}

void KaminoE_AlwaysUpdate(WORLDINFO_s *) {
    bool v = 1;
    if (CUTSTOPGAME == 0)
        v = (GameCam->sock_position.location.sock == 0x1e);
    object_switches[1] = v;
}

void KaminoE_Draw(WORLDINFO_s *world) {
    if (netclient == 0) {
        if (kamino_e.show_hearts->value > 0.0f) {
            GameObject_s *obj = (GameObject_s *)FindGameObject((i32)(i16)id_JANGOFETT, 1, 1, 1, 0);
            if (obj != NULL && kamino_e.show_hearts != NULL && kamino_e.show_hearts->value == 1.0f)
                DrawBossHitPoints(obj);
        }
    }
    NuSpecialSetDrawMtx(&kamino_e.slave1, NuSpecialGetDrawMtx(&kamino_e.slave1));
    NuSpecialSetVisibility(&kamino_e.slave1, 1);
}

void KaminoE_CheckPlatHit(BOLT_s *) {
}

void KaminoF_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "f1")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
    }
    if ((b = GizmoBlowUp_FindByName(world, "f2")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
    }
    if ((b = GizmoBlowUp_FindByName(world, "f3")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
    }
}

void KaminoOutro_Init(WORLDINFO_s *) {
    bool v = 0;
    if (CUTSTOPGAME == 0) {
        u8 b = GameCam->sock_position.location.sock;
        if (b != 5)
            v = (b != 6);
    }
    object_switches[1] = v;
}

void NbKaminoA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "nb1")) != NULL) {
        b->field_0x128 = 1.0f;
        b->field_0x124 = 1;
    }
    if ((b = GizmoBlowUp_FindByName(world, "nb2")) != NULL) {
        b->field_0x128 = 1.0f;
        b->field_0x124 = 1;
    }
}

// ===========================================================================
// Geonosis — droid factory (Factory_B / Factory_G)
// ===========================================================================

void FactoryB_Init(WORLDINFO_s *world) {
    factoryb_netpacket = SetLevelHack(0x4);
    InitPaintPuzzle(world);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv1");
    LevGizObst[1] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv2");
    LevGizObst[2] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv3");
    LevGizObst[3] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv4");
    LevGizObst[4] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv5");
    LevGizObst[5] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv6");
    LevGizObst[6] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv7");
    LevGizObst[7] = GizObstacle_FindByName(world->giz_obstacle_sys, "conv8");
    for (i32 i = 0; i < 8; i++) {
        if (LevGizObst[i] != NULL) {
            LevGizObst[i]->mode = 3;
            LevGizObst[i]->state = 0;
        }
    }
}

void FactoryB_Reset(WORLDINFO_s *world) {
    ResetPaintPuzzle(world);
    factoryb_cut = NewCutScene(NULL, world->cutscene_sys, "fb_cut", 0);
    if (factoryb_cut != NULL) {
        CUTSCENEDATA_s *scene = (CUTSCENEDATA_s *)factoryb_cut->scene;
        if (scene != NULL) {
            scene->field_0x88 |= 2;
            scene->field_0x88 |= 8;
        }
    }
    factoryb_conveyor_stopped_msg = CheckGizAIMessage(gizaimessagesys, "conv_stopped", NULL);
}

void FactoryB_Update(WORLDINFO_s *) {
}

void FactoryB_Draw(WORLDINFO_s *) {
    DrawPaintLights();
}

void FactoryG_Init(WORLDINFO_s *world) {
    if (netclient != 0)
        return;
    GIZMO *g = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force_g1");
    if (g != NULL)
        force_array[0] = (GIZFORCE_s *)g->object;
    g = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force_g2");
    if (g != NULL)
        force_array[1] = (GIZFORCE_s *)g->object;
    g = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force_g3");
    if (g != NULL)
        force_array[2] = (GIZFORCE_s *)g->object;
    g = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force_g4");
    if (g != NULL)
        force_array[3] = (GIZFORCE_s *)g->object;
}

void FactoryG_Update(WORLDINFO_s *world) {
    if (netclient != 0)
        return;
    i32 complete = 0;
    if (GizForce_Complete(force_array[0]))
        complete++;
    if (GizForce_Complete(force_array[1]))
        complete++;
    if (GizForce_Complete(force_array[2]))
        complete++;
    if (GizForce_Complete(force_array[3]))
        complete++;
    if (ObiWan == NULL) {
        ObiWan = (GameObject_s *)FindGameObject((i32)(i16)id_OBIWANKENOBIJEDIMASTER, 0x400, 0, 1, 0);
        return;
    }
    if (complete == 4) {
        if (FreePlay == 0)
            NewCutScene(NULL, world->cutscene_sys, "factory_escape", 1);
    } else {
        ObiWan->apiobj.position = {79.2f, 0.75f, -10.5f};
    }
}

// ===========================================================================
// Jedi (Jedi_B)
// ===========================================================================

static void JediBKilledCallback(GameObject_s *object) {
    if (object == NULL)
        return;
    i32 i;
    for (i = 0; i < jedi_b.objective_count; ++i) {
        if (jedi_b.entries[256 + i].object == object)
            break;
    }
    if (i < jedi_b.objective_count) {
        jedi_b.entries[256 + i].object = NULL;
        i32 live = 0;
        for (i32 j = 0; j < jedi_b.objective_count; ++j) {
            if (jedi_b.entries[256 + j].object != NULL)
                ++live;
        }
        if (live == 0)
            jedi_b.objective_count = 0;
        return;
    }
    for (i = 0; i < jedi_b.count; ++i) {
        if (jedi_b.entries[i].object == object)
            break;
    }
    if (i < jedi_b.count) {
        JEDIB_ENTRY_s *entry = &jedi_b.entries[i];
        if (g_lowEndLevelBehaviour) {
            for (i32 j = 0; j < 4; ++j) {
                if (entry->model_id == jedi_b.target_models[j] && jedi_b.target_complete[j] == 0) {
                    jedi_b.target_complete[j] = 1;
                    jedi_b.create_timer = 0.0f;
                    break;
                }
            }
        } else {
            for (i32 j = 0; j < 6; ++j) {
                if (entry->model_id == jedi_b.target_models[j] && jedi_b.target_complete[j] == 0) {
                    jedi_b.target_complete[j] = 1;
                    jedi_b.create_timer = 0.0f;
                    break;
                }
            }
        }
        entry->object = NULL;
        entry->flags = (entry->flags | 1) & ~0x18;
        if ((entry->flags & 2) != 0) {
            if (!FreePlay && (jedi_b.replacement_unlock_mask & 1) == 0) {
                entry->model_id = id_LUMINARA;
                jedi_b.replacement_unlock_mask |= 1;
            } else if (!FreePlay && (jedi_b.replacement_unlock_mask & 2) == 0) {
                entry->model_id = id_SHAAKTI;
                jedi_b.replacement_unlock_mask |= 2;
            } else {
                entry->model_id = NuRandFloat() < 0.5f ? id_BATTLEDROIDGEONOSIAN : id_GEONOSIAN;
            }
        } else {
            entry->model_id = id_BOB;
        }
        entry->flags &= ~0x1c;
        entry->model_state = 0;
        return;
    }
}

static i32 JediBInitLocator(WORLDINFO_s *world, NUVEC *position, i32 direction, AILOCATOR_s *locator, f32 radius) {
    AISYS_s *system = world->ai_sys;
    if (system != NULL) {
        AIANTINODE *node = system->antinodes;
        for (i32 i = 0; i < system->antinode_count; ++i, ++node) {
            if (node->radius != 0.0f) {
                f32 x = position->x - node->position.x;
                f32 z = position->z - node->position.z;
                f32 expanded = node->radius + 1.0f;
                f32 distance = x * x + z * z;
                if (distance < expanded * expanded) {
                    f32 scale = expanded / NuFsqrt(distance);
                    position->x = x * scale + node->position.x;
                    position->z = z * scale + node->position.z;
                    break;
                }
            }
        }
    }
    memset(locator, 0, sizeof(*locator));
    if (radius * radius > position->x * position->x + position->z * position->z) {
        f32 height = GameShadow(NULL, position, 5.0f, 0);
        if (height != 2000000.0f) {
            position->y = height;
        }
        AISysGetPathPos(world->ai_sys, position, &locator->path_info, NULL, 255);
        if ((locator->path_info.flags & 1) != 0) {
            locator->position = *position;
            locator->direction = direction;
            return 1;
        }
    }
    return 0;
}

void JediB_Init(WORLDINFO_s *world) {
    if (Mission_Active(MissionSys))
        return;
    memset(&jedi_b, 0, sizeof(jedi_b));
    jedi_b.random_seed = jedib_seed;
    jedib_netpacket = SetLevelHack(0x20);
    if (!netclient) {
        f32 step = g_lowEndLevelBehaviour ? jedib_create_step_LowEnd : jedib_create_step_Normal;
        for (f32 x = -jedib_outer_r; x <= jedib_outer_r; x += step) {
            for (f32 z = -jedib_outer_r; z <= jedib_outer_r; z += step) {
                NUVEC position;
                f32 offset = jedib_offset;
                position.x = offset - (offset + offset) * NuRandFloatSeeded((u32 *)&jedi_b.random_seed) + x;
                position.y = 0.0f;
                offset = jedib_offset;
                position.z = offset - (offset + offset) * NuRandFloatSeeded((u32 *)&jedi_b.random_seed) + z;
                f32 distance = position.x * position.x + position.z * position.z;
                if (!(jedib_outer_r * jedib_outer_r > distance))
                    continue;
                if (!(distance > jedib_inner * jedib_inner))
                    continue;
                f32 height = GameShadow(NULL, &position, 5.0f, 0);
                if (height != 2000000.0f)
                    position.y = height;
                i32 rotation = NuRandIntSeeded((u32 *)&jedi_b.random_seed);
                AILOCATOR locator;
                if (netclient || !JediBInitLocator(world, &position, rotation, &locator, jedib_outer_r))
                    continue;
                if (jedi_b.count > 255)
                    continue;
                JEDIB_ENTRY_s *good = &jedi_b.entries[jedi_b.count++];
                good->position = position;
                good->locator = locator;
                good->flags &= ~2;
                NUVEC delta = {0.0f, 0.0f, jedib_proximity};
                i32 angle = NuRandIntSeeded((u32 *)&jedi_b.random_seed);
                NuVecRotateY(&delta, &delta, NuRandIntSeeded((u32 *)&jedi_b.random_seed));
                u32 random = NuRandIntSeeded((u32 *)&jedi_b.random_seed);
                i32 count = jedib_min_baddies_per_goody +
                            random % (u32)(jedib_max_baddies_per_goody - jedib_min_baddies_per_goody);
                for (i32 i = 0; i < count && jedi_b.count <= 255; ++i) {
                    if (i != 0)
                        angle = NuAngAdd(angle, 65536 / count);
                    NuVecRotateY(&position, &delta, angle);
                    NuVecAdd(&position, &position, &good->position);
                    if (!netclient && JediBInitLocator(world, &position, angle, &locator, jedib_outer_r)) {
                        JEDIB_ENTRY_s *bad = &jedi_b.entries[jedi_b.count++];
                        bad->flags |= 2;
                        bad->position = position;
                        bad->locator = locator;
                        bad->linked_entry = good;
                        bad->rotation = angle;
                        good->linked_entry = bad;
                    }
                }
            }
        }
    }
    NUGSPLINE *spline = edSpline_SplineFind(world->current_gscn, const_cast<char *>("teleport_01"));
    if (spline) {
        f32 height = spline->pts[0].y - 0.35f;
        spline->pts[0].y = height;
        spline->pts[1].y = height;
    }
    LevBlowUp[0] = GizmoBlowUp_FindByName(world, "Thermo_011");
}

void JediB_Reset(WORLDINFO_s *world) {
    if (Mission_Active(MissionSys) || netclient)
        return;
    for (i32 i = 0; i < jedi_b.count; ++i) {
        jedi_b.entries[i].object = NULL;
        jedi_b.entries[i].flags &= ~0x19;
        jedi_b.entries[i].model_id = -1;
    }
    for (i32 i = 0; i < jedi_b.objective_count; ++i) {
        jedi_b.entries[256 + i].object = NULL;
        jedi_b.entries[256 + i].flags &= ~1;
    }
    ClearAICreatures();
    char name[40];
    for (i32 i = 0; i < 3; ++i) {
        sprintf(name, "pillar%d_01a", i + 1);
        NuSpecialFind(world->current_gscn, &jedi_b.restraint_specials[i][0], name, 1);
        sprintf(name, "pillar%d_01b", i + 1);
        NuSpecialFind(world->current_gscn, &jedi_b.restraint_specials[i][1], name, 1);
        sprintf(name, "pillar%d_01c", i + 1);
        NuSpecialFind(world->current_gscn, &jedi_b.restraint_specials[i][2], name, 1);
        sprintf(name, "pillar%d_01ba", i + 1);
        NuSpecialFind(world->current_gscn, &jedi_b.restraint_specials[i][3], name, 1);
        jedi_b.party[i] = NULL;
        if (!FreePlay) {
            for (i32 player = 0; player < 8; ++player) {
                if (Player[player] && Player[player]->id == *JediB_playerids[i]) {
                    jedi_b.party[i] = Player[player];
                    break;
                }
            }
        }
    }
    jedi_b.phase_message = SetGizAIMessage(gizaimessagesys, "Phase", 0.0f, NULL);
    jedi_b.phase_complete_message = SetGizAIMessage(gizaimessagesys, "PhaseComplete", 0.0f, NULL);
    jedi_b.objectives_message = SetGizAIMessage(gizaimessagesys, "ObjectivesLeft", 0.0f, NULL);
    jedi_b.restraint_messages[0] = SetGizAIMessage(gizaimessagesys, "RestrainPadme", 0.0f, NULL);
    jedi_b.restraint_messages[1] = SetGizAIMessage(gizaimessagesys, "RestrainAnakin", 0.0f, NULL);
    jedi_b.restraint_messages[2] = SetGizAIMessage(gizaimessagesys, "RestrainObiWan", 0.0f, NULL);
    jedi_b.jango = NULL;
    jedi_b.replacement_unlock_mask &= ~3;
}

void JediB_Update(WORLDINFO_s *world) {
    if (Mission_Active(MissionSys) != NULL || netclient != 0)
        return;

    for (i32 i = 0; i != 3; ++i) {
        if (FreePlay != 0 && jedi_b.party[i] == NULL) {
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][0]))
                    jedi_b.party[i] =
                        AddDynamicCreature(*JediB_playerids[i], NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][0]),
                                           0, const_cast<char *>("Party"), NULL, NULL, 1, NULL, NULL, 0, 0);
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][1]))
                    jedi_b.party[i] =
                        AddDynamicCreature(*JediB_playerids[i], NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][1]),
                                           0, const_cast<char *>("Party"), NULL, NULL, 1, NULL, NULL, 0, 0);
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][2]))
                    jedi_b.party[i] =
                        AddDynamicCreature(*JediB_playerids[i], NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][2]),
                                           0, const_cast<char *>("Party"), NULL, NULL, 1, NULL, NULL, 0, 0);
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][3]))
                    jedi_b.party[i] =
                        AddDynamicCreature(*JediB_playerids[i], NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][3]),
                                           0, const_cast<char *>("Party"), NULL, NULL, 1, NULL, NULL, 0, 0);
            }
        }
        if (jedi_b.party[i] == NULL || jedi_b.restraint_messages[i]->value != 1.0f)
            continue;
        {
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][0])) {
                    GameObject_s *object = jedi_b.party[i];
                    object->apiobj.position = *NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][0]);
                    NuVecAdd(&jedi_b.party[i]->apiobj.position, &jedi_b.party[i]->apiobj.position, &jedib_offsets[i]);
                    object = jedi_b.party[i];
                    object->apiobj.velocity.x = 0.0f;
                    object->saved_position = object->apiobj.position;
                    object->apiobj.velocity.y = 0.0f;
                    object->apiobj.field_0x276 = (u16)jedib_yrot[i];
                    object->apiobj.velocity.z = 0.0f;
                    object->apiobj.movement_stuck_time = 0.0f;
                }
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][1])) {
                    GameObject_s *object = jedi_b.party[i];
                    object->apiobj.position = *NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][1]);
                    NuVecAdd(&jedi_b.party[i]->apiobj.position, &jedi_b.party[i]->apiobj.position, &jedib_offsets[i]);
                    object = jedi_b.party[i];
                    object->apiobj.velocity.x = 0.0f;
                    object->saved_position = object->apiobj.position;
                    object->apiobj.velocity.y = 0.0f;
                    object->apiobj.field_0x276 = (u16)jedib_yrot[i];
                    object->apiobj.velocity.z = 0.0f;
                    object->apiobj.movement_stuck_time = 0.0f;
                }
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][2])) {
                    GameObject_s *object = jedi_b.party[i];
                    object->apiobj.position = *NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][2]);
                    NuVecAdd(&jedi_b.party[i]->apiobj.position, &jedi_b.party[i]->apiobj.position, &jedib_offsets[i]);
                    object = jedi_b.party[i];
                    object->apiobj.velocity.x = 0.0f;
                    object->saved_position = object->apiobj.position;
                    object->apiobj.velocity.y = 0.0f;
                    object->apiobj.field_0x276 = (u16)jedib_yrot[i];
                    object->apiobj.velocity.z = 0.0f;
                    object->apiobj.movement_stuck_time = 0.0f;
                }
            }
            {
                if (NuSpecialGetVisibilityFn(&jedi_b.restraint_specials[i][3])) {
                    GameObject_s *object = jedi_b.party[i];
                    object->apiobj.position = *NuSpecialGetDrawPos(&jedi_b.restraint_specials[i][3]);
                    NuVecAdd(&jedi_b.party[i]->apiobj.position, &jedi_b.party[i]->apiobj.position, &jedib_offsets[i]);
                    object = jedi_b.party[i];
                    object->apiobj.velocity.x = 0.0f;
                    object->saved_position = object->apiobj.position;
                    object->apiobj.velocity.y = 0.0f;
                    object->apiobj.field_0x276 = (u16)jedib_yrot[i];
                    object->apiobj.velocity.z = 0.0f;
                    object->apiobj.movement_stuck_time = 0.0f;
                }
            }
        }
    }

    f32 timer = jedi_b.timer;
    jedi_b.target_mask = 0;
    if (!(timer < 4.0f)) {
        if (jedi_b.phase == 7) {
            if (jedi_b.jango != NULL) {
                GameObject_s *target = player2;
                if (target == NULL || jedi_b.jango->ai.opponent != target)
                    target = player;
                u32 index = target->apiobj.field_0x289;
                jedi_b.target_mask |= (u64)1 << index;
            }
        } else {
            f32 safe_radius_sq = jedib_safe_r * jedib_safe_r;
            f32 radius_sq = player->apiobj.collision_position.x * player->apiobj.collision_position.x +
                            player->apiobj.collision_position.z * player->apiobj.collision_position.z;
            if (radius_sq > safe_radius_sq) {
                u32 index = player->apiobj.field_0x289;
                jedi_b.target_mask |= (u64)1 << index;
            }
            if (player2 != NULL) {
                radius_sq = player2->apiobj.collision_position.x * player2->apiobj.collision_position.x +
                            player2->apiobj.collision_position.z * player2->apiobj.collision_position.z;
                if (radius_sq > safe_radius_sq) {
                    u32 index = player2->apiobj.field_0x289;
                    jedi_b.target_mask |= (u64)1 << index;
                }
            }
        }
    } else {
        if (player != NULL) {
            u32 index = player->apiobj.field_0x289;
            jedi_b.target_mask |= (u64)1 << index;
        }
        if (player2 != NULL) {
            u32 index = player2->apiobj.field_0x289;
            jedi_b.target_mask |= (u64)1 << index;
        }
    }
    GameObject_s *jango = jedi_b.jango;
    if (jango != NULL && jedi_b.phase == 7) {
        {
            GameObject_s *object = Player[0];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[1];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[2];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[3];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[4];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[5];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[6];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
        {
            GameObject_s *object = Player[7];
            if (object != NULL) {
                object->ai_opponent_exclusion_mask = 0;
                if ((i8)object->apiobj.object_flags >= 0)
                    object->ai_opponent_exclusion_mask = (u64)1 << jango->apiobj.field_0x289;
            }
        }
    } else {
        if (Player[0] != NULL)
            Player[0]->ai_opponent_exclusion_mask = 0;
        if (Player[1] != NULL)
            Player[1]->ai_opponent_exclusion_mask = 0;
        if (Player[2] != NULL)
            Player[2]->ai_opponent_exclusion_mask = 0;
        if (Player[3] != NULL)
            Player[3]->ai_opponent_exclusion_mask = 0;
        if (Player[4] != NULL)
            Player[4]->ai_opponent_exclusion_mask = 0;
        if (Player[5] != NULL)
            Player[5]->ai_opponent_exclusion_mask = 0;
        if (Player[6] != NULL)
            Player[6]->ai_opponent_exclusion_mask = 0;
        if (Player[7] != NULL)
            Player[7]->ai_opponent_exclusion_mask = 0;
    }

    switch (jedi_b.phase_state) {
        case 0: {
            JEDIB_PHASE_s *phases[3] = {jedi_b_phase1, jedi_b_phase2, jedi_b_phase3};
            if (netclient == 0) {
                if (jedi_b.phase > 2 && timer < 3.0f) {
                    jedi_b.timer = timer + FRAMETIME;
                } else {
                    ++jedi_b.phase;
                    if (jedi_b.phase == 6)
                        jedi_b.phase = 7;
                    else if (jedi_b.phase >= 1 && jedi_b.phase <= 3) {
                        JEDIB_PHASE_s *spawn = phases[jedi_b.phase - 1];
                        while (spawn != NULL && spawn->model != NULL && jedi_b.objective_count < 8) {
                            if (g_lowEndLevelBehaviour != 0 && jedi_b.objective_count == 5)
                                break;
                            char name[16];
                            sprintf(name, "phase%d_%d", jedi_b.phase, jedi_b.objective_count + 1);
                            AILOCATOR *locator = AIPathFindLocator(world->ai_sys, name);
                            if (locator != NULL) {
                                JEDIB_ENTRY_s *entry = &jedi_b.entries[256 + jedi_b.objective_count];
                                entry->locator = *locator;
                                entry->object =
                                    AddDynamicCreature(*spawn->model, &locator->position, locator->direction,
                                                       spawn->script, &locator->path_info, NULL, 1, NULL, NULL, 0, 0);
                                GameObject_s *object = jedi_b.entries[256 + jedi_b.objective_count].object;
                                if (object != NULL) {
                                    jedi_b.entries[256 + jedi_b.objective_count].model_id = *spawn->model;
                                    object->field_0xefb |= 1;
                                    object->ai.locator = locator;
                                    object->field_0xeb4 = JediBKilledCallback;
                                    ++jedi_b.objective_count;
                                }
                            }
                            ++spawn;
                        }
                    } else if (jedi_b.phase >= 4 && jedi_b.phase <= 6) {
                        for (i32 i = 0; i != 6; ++i)
                            jedi_b.target_complete[i] = 0;
                        for (i32 i = 0; i < jedi_b.count; ++i)
                            jedi_b.entries[i].flags &= ~0x18;
                        for (i32 i = 0; i < (g_lowEndLevelBehaviour != 0 ? 4 : 6); ++i) {
                            i32 index = NuRand(NULL) % jedi_b.count;
                            i32 model = jedi_b.phase == 4   ? id_BATTLEDROIDSECURITY
                                        : jedi_b.phase == 5 ? ((i & 1) ? id_BATTLEDROIDSECURITY : id_SUPERBATTLEDROID)
                                                            : ((i & 1) ? id_DROIDEKA : id_SUPERBATTLEDROID);
                            while ((jedi_b.entries[index].flags & 8) != 0 || (jedi_b.entries[index].flags & 2) == 0 ||
                                   jedi_b.entries[index].model_id == model) {
                                if (++index >= jedi_b.count)
                                    index = 0;
                            }
                            JEDIB_ENTRY_s *entry = &jedi_b.entries[index];
                            if (entry->object != NULL) {
                                RemoveGameObject(entry->object, 1);
                                entry->object = NULL;
                                entry->flags &= ~1;
                            }
                            entry->model_id = model;
                            jedi_b.target_models[i] = (i16)model;
                            entry->flags |= 8;
                        }
                        jedi_b.create_timer = 0.0f;
                    }
                    if (jedi_b.phase_message == NULL)
                        jedi_b.phase_message = CheckGizAIMessage(gizaimessagesys, "Phase", NULL);
                    if (jedi_b.phase_complete_message == NULL)
                        jedi_b.phase_complete_message = CheckGizAIMessage(gizaimessagesys, "PhaseComplete", NULL);
                    if (jedi_b.objectives_message == NULL)
                        jedi_b.objectives_message = CheckGizAIMessage(gizaimessagesys, "ObjectivesLeft", NULL);
                    if (jedi_b.restraint_messages[0] == NULL)
                        jedi_b.restraint_messages[0] = CheckGizAIMessage(gizaimessagesys, "RestrainPadme", NULL);
                    if (jedi_b.restraint_messages[1] == NULL)
                        jedi_b.restraint_messages[1] = CheckGizAIMessage(gizaimessagesys, "RestrainAnakin", NULL);
                    if (jedi_b.restraint_messages[2] == NULL)
                        jedi_b.restraint_messages[2] = CheckGizAIMessage(gizaimessagesys, "RestrainObiWan", NULL);
                    jedi_b.phase_message->value = (f32)jedi_b.phase;
                    jedi_b.objectives_message->value = (f32)jedi_b.objective_count;
                    jedi_b.phase_complete_message->value = 0.0f;
                    jedi_b.timer = 0.0f;
                    jedi_b.phase_state = 1;
                }
            }
            break;
        }
        case 1: {
            jedi_b.timer = timer + FRAMETIME;
            switch (jedi_b.phase) {
                case 1:
                case 2:
                case 3:
                    jedi_b.objectives_message->value = (f32)jedi_b.objective_count;
                    if (jedi_b.phase_complete_message->value == 1.0f) {
                        jedi_b.timer = 0.0f;
                        jedi_b.phase_state = 2;
                    }
                    break;
                case 4:
                case 5:
                case 6: {
                    i32 complete =
                        jedi_b.target_complete[0] && jedi_b.target_complete[1] && jedi_b.target_complete[2] &&
                        jedi_b.target_complete[3] &&
                        (g_lowEndLevelBehaviour != 0 || (jedi_b.target_complete[4] && jedi_b.target_complete[5]));
                    jedi_b.create_timer += FRAMETIME;
                    if (jedi_b.create_timer > 5.0f) {
                        i32 last = -1;
                        i32 i;
                        for (i = 0; i < jedi_b.count; ++i) {
                            if (jedi_b.entries[i].flags & 0x10)
                                break;
                            if (jedi_b.entries[i].flags & 8)
                                last = i;
                        }
                        if (i == jedi_b.count && last != -1)
                            jedi_b.entries[last].flags |= 0x10;
                        jedi_b.create_timer = 0.0f;
                    }
                    if (complete) {
                        jedi_b.timer = 0.0f;
                        jedi_b.phase_state = 2;
                    }
                    break;
                }
                case 7:
                    if (jango == NULL) {
                        jango = (GameObject_s *)FindGameObject(id_JANGOFETT, 1, 1, 0, 0);
                        jedi_b.jango = jango;
                    }
                    if (jango != NULL && (jango->apiobj.field_0x287 != 0 || jango->current_hp == 0)) {
                        jedi_b.timer = 0.0f;
                        jedi_b.phase_state = 2;
                    }
                    break;
            }
            break;
        }
        case 2: {
            if (timer < 0.1f)
                jedi_b.timer = timer + FRAMETIME;
            else {
                if (jedi_b.phase > 6) {
                    if (FreePlay == 0)
                        NewLData = JEDI_OUTRO_LDATA;
                    CompleteLevel(world);
                }
                jedi_b.timer = 0.0f;
                jedi_b.phase_state = 0;
            }
        }
    }

    jedib_n_active = 0;
    for (i32 i = 0; i < jedi_b.count; ++i) {
        JEDIB_ENTRY_s *entry = &jedi_b.entries[i];
        GameObject_s *object = entry->object;
        if (object != NULL) {
            if (entry->flags & 0x10) {
                object->ai.locator = NULL;
                GameObject_s *first = player;
                CHARACTERDATA *first_data = first->apiobj.character_data;
                if ((first_data->model_flags & 0x80000) == 0 &&
                    (first->apiobj.objptr->id != id_JARJAR || (i8)first->apiobj.object_flags < 0) &&
                    (first_data->game_character->uses_weapon_action != 4 ||
                     object->apiobj.character_data->game_character->uses_weapon_action != 4)) {
                    object->ai.opponent = first;
                } else {
                    if (object->ai.opponent == first)
                        object->ai.opponent = NULL;
                    GameObject_s *second = player2;
                    if (second != NULL) {
                        // The original deliberately reuses first's model and weapon checks here.
                        if ((second->apiobj.character_data->model_flags & 0x80000) == 0 &&
                            (first->apiobj.objptr->id != id_JARJAR || (i8)first->apiobj.object_flags < 0) &&
                            (first_data->game_character->uses_weapon_action != 4 ||
                             object->apiobj.character_data->game_character->uses_weapon_action != 4))
                            object->ai.opponent = second;
                        else if (object->ai.opponent == second)
                            object->ai.opponent = NULL;
                    }
                }
            } else {
                if ((OnOrInsidePlane(&object->apiobj.position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 2.0f,
                                     NULL) ||
                     OnOrInsidePlane(&entry->object->apiobj.position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL,
                                     2.0f, NULL)) &&
                    (OnOrInsidePlane(&entry->locator.position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 2.0f,
                                     NULL) ||
                     OnOrInsidePlane(&entry->locator.position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 2.0f,
                                     NULL))) {
                    object = entry->object;
                    entry->position = object->apiobj.position;
                    entry->rotation = object->apiobj.field_0x276;
                    RemoveGameObject(object, 1);
                    entry->object = NULL;
                    continue;
                }
                object = entry->object;
                if (object != NULL && object->apiobj.ai->opponent == NULL && entry->linked_entry != NULL &&
                    entry->linked_entry->object != NULL)
                    object->apiobj.ai->opponent = entry->linked_entry->object;
            }
        } else {
            if ((entry->flags & 0x10) == 0 &&
                (OnOrInsidePlane(&entry->position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 1.5f, NULL) ||
                 OnOrInsidePlane(&entry->position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 1.5f, NULL))) {
                object = entry->object;
            } else {
                bool rotate_position =
                    (entry->flags & 1) != 0 &&
                    !OnOrInsidePlane(&entry->position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 0.5f, NULL) &&
                    !OnOrInsidePlane(&entry->position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 0.5f, NULL);
                NUVEC position = entry->position;
                if (rotate_position)
                    NuVecRotateY(&position, &position, 0x8000);
                if (entry->model_id == -1) {
                    if ((entry->flags & 2) == 0)
                        entry->model_id = id_BOB;
                    else if (FreePlay == 0 && (jedi_b.replacement_unlock_mask & 1) == 0) {
                        entry->model_id = id_LUMINARA;
                        jedi_b.replacement_unlock_mask |= 1;
                    } else if (FreePlay == 0 && (jedi_b.replacement_unlock_mask & 2) == 0) {
                        entry->model_id = id_SHAAKTI;
                        jedi_b.replacement_unlock_mask |= 2;
                    } else
                        entry->model_id = NuRandFloat() < 0.5f ? id_BATTLEDROIDGEONOSIAN : id_GEONOSIAN;
                    entry->flags &= ~0x1c;
                    entry->model_state = 0;
                }
                entry->object =
                    AddDynamicCreature(entry->model_id, &position, entry->rotation, const_cast<char *>("gen_bdroids"),
                                       &entry->locator.path_info, NULL, 1, NULL, NULL, 0, 0);
                object = entry->object;
                if (object == NULL)
                    continue;
                object->ai.locator = &entry->locator;
                object->field_0xeb4 = JediBKilledCallback;
                if (entry->model_id == id_BOB) {
                    if (entry->model_state != 0) {
                        object->field_0x1054 = entry->model_state;
                        if (entry->flags & 4)
                            object->field_0xefd |= 2;
                        else
                            object->field_0xefd &= ~2;
                        object = entry->object;
                    } else {
                        entry->model_state = object->field_0x1054;
                        if (object->field_0xefd & 2)
                            entry->flags |= 4;
                        else
                            entry->flags &= ~4;
                    }
                }
            }
        }
        if (object != NULL) {
            ++jedib_n_active;
            if ((entry->flags & 0x10) == 0) {
                object->ai_opponent_exclusion_mask |= jedi_b.target_mask;
            }
        }
    }
}

void JediB_DrawPanel(WORLDINFO_s *) {
}

// ===========================================================================
// Gunship (Gunship_A / Gunship_B)
// ===========================================================================

void GunshipA_Init(WORLDINFO_s *world) {
    gunship_bolts[1] = (i16)BoltType_FindIDByName("gunbolt2", world);
    gunship_bolts[0] = (i16)BoltType_FindIDByName("gunbolt1", world);
    gunship_flags[0] = 0;
    gunship_flags[1] = 0;
    gunship_flags[2] = 0;
    gunship_flags[3] = 0;
    gunship_flags[4] = 0;
    gunship_flags[5] = 1;
    gunship_flags[6] = 1;
    gunship_flags[7] = 1;
    gunship_flags[8] = 1;
    gunship_flags[9] = 1;
    InitMiniSnowTroopers(world, 0xa, 0x20, 0);
    gunship_weapons[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gunw1");
    gunship_weapons[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gunw2");
    gunship_weapons[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gunw3");
    gunship_weapons[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gunw4");
}

void GunshipA_Update(WORLDINFO_s *world) {
    UpdateMiniSnowTroopers(world);
}

void GunshipA_Draw(WORLDINFO_s *world) {
    if (TimingBarSet == 5) {
        TBOPENFN("gun_timing", 5);
        DrawMiniSnowTroopers(world);
    } else {
        DrawMiniSnowTroopers(world);
        if (TimingBarSet == 5)
            TBCLOSEFN("gun_timing", 5);
    }
}

void GunshipB_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun1");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun2");
    LevGizmo[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun3");
    LevGizmo[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun4");
    LevGizmo[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun5");
    LevGizmo[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun6");
    LevGizmo[6] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun7");
    LevGizmo[7] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "gun8");
}

i32 GunshipInLevel(LEVELDATA_s *level) {
    if (BONUS_GUNSHIPA_LDATA == NULL)
        return 0;
    return BONUS_GUNSHIPA_LDATA == level;
}

f32 gunshipb_seekmomseek = 5.0f;
f32 gunshipb_seekmom = 5.0f;
f32 gunshipb_seekrange = 4.0f;
f32 SeekValF(f32 current, f32 target, f32 rate);

// Original: 1,436 bytes.
void GunShip_DragBombSeekBlowUp(GameObject_s *object) {
    if (object->character_context != 0x34)
        return;
    f32 nearest_distance = gunshipb_seekrange * gunshipb_seekrange;
    GIZMOBLOWUP_s *nearest = NULL;
    NUVEC offset, nearest_offset, direction;
    for (i32 i = 0; i < 8; ++i) {
        if (LevGizmo[i] == NULL)
            continue;
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(LevGizmo[i]->object);
        if (blowup == NULL || (blowup->status_flags & 0x800001) != 0x800000)
            continue;
        f32 distance = NuVecDistSqr(&blowup->mid_position, &object->apiobj.collision_position, &offset);
        if (nearest_distance > distance) {
            nearest_distance = distance;
            nearest_offset = offset;
            nearest = blowup;
        }
    }
    if (nearest == NULL)
        return;
    NuVecNorm(&direction, &nearest_offset);
    f32 momentum = (1.0f - NuFsqrt(nearest_distance) / gunshipb_seekrange) * gunshipb_seekmom;
    direction.x *= momentum;
    direction.z *= momentum;
    object->apiobj.velocity.x = SeekValF(object->apiobj.velocity.x, direction.x, gunshipb_seekmomseek);
    object->apiobj.velocity.z = SeekValF(object->apiobj.velocity.z, direction.z, gunshipb_seekmomseek);
}

// ===========================================================================
// Bonus gunship (Bonus_Gunship_A / Bonus_Gunship_B)
// ===========================================================================

void BonusGunshipA_Reset(WORLDINFO_s *) {
    gunship_player_dead = 0;
    if (LevFlag.progress == GUNSHIP_ACTIVE)
        LevFlag.progress = GUNSHIP_WON;
    LevFlag.exit = 0;
    bonus_gunship_store_progress_flag = 0;
}

void BonusGunshipA_Update(WORLDINFO_s *world) {
    if (LevFlag.progress == GUNSHIP_INACTIVE) {
        bool found = false;
        if (Player[0] != NULL && Player[0]->field_0x661 == 0 && Player[0]->field_0x68c > 0.001f) {
            found = true;
        } else if (Player[1] != NULL && Player[1]->field_0x661 == 0 && Player[1]->field_0x68c > 0.001f) {
            found = true;
        }
        if (found) {
            if (netclient != 0) {
                LevFlag.progress = GUNSHIP_ACTIVE;
            } else {
                Doors_SetLastDoor((DOOR_s *)Door_FindByName(world, "bonus_door"));
                bonus_gunship_store_progress_flag = 1;
                StoreLevelProgress(world);
                bonus_gunship_store_progress_flag = 0;
                LevFlag.progress = GUNSHIP_ACTIVE;
            }
        }
    }
    if (gunship_player_dead == 0) {
        if ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0) ||
            (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0)) {
            gunship_player_dead = 1;
            ResetLevel(world, "bonus", 1);
        }
    }
}

void BonusGunshipB_Init(WORLDINFO_s *world) {
    bonusgunshipb_netpacket = (struct BONUSGUNSHIP_NETPACKET_s *)SetLevelHack(0xc);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obs");
}

void BonusGunshipB_Reset(WORLDINFO_s *) {
    LevFlag.progress = GUNSHIP_INACTIVE;
    LevFlag.exit = 0;
    MiscTime = 0;
    gunship_player_dead = 0;
    bonus_gunship_store_progress_flag = 0;
}

void BonusGunshipB_Update(WORLDINFO_s *world) {
    if (netclient != 0) {
        LevFlag.progress = bonusgunshipb_netpacket->state;
        LevFlag.exit = bonusgunshipb_netpacket->sub;
        MiscTime = bonusgunshipb_netpacket->time;
    } else {
        if (gunship_player_dead == 0 && ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0) ||
                                         (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0))) {
            gunship_player_dead = 1;
            ResetLevel(world, "bonus_gunship", 1);
        }
        bonusgunshipb_netpacket->state = LevFlag.progress;
        bonusgunshipb_netpacket->sub = LevFlag.exit;
        bonusgunshipb_netpacket->time = MiscTime;
    }
    if (LevFlag.progress == 0) {
        if (LevDeaths > 0) {
            float x = (float)LevDeaths * LevDeaths + 1.0f;
            if (GameTimer.time_elapsed >= x)
                LevFlag.progress = GUNSHIP_ACTIVE;
        }
    } else if (LevFlag.progress == GUNSHIP_ACTIVE) {
        if (MiscTime > 5.0f)
            MiscTime = 5.0f;
    }
}

void BonusGunshipB_Panel(WORLDINFO_s *) {
    if (LevFlag.progress == GUNSHIP_ACTIVE) {
        if (MiscTime > 60.0f)
            DrawTimer((i32)MiscTime + 1, 0, 0);
    }
}

// ===========================================================================
// Dooku (Dooku_C)
// ===========================================================================

void DookuC_Init(WORLDINFO_s *world) {
    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "dooku");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "dooku1");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "dooku2");
    void *path1 = AIPathFindNode(world->ai_sys, NULL, "path1");
    LevAIPathNode[0] = path1;
    void *path2 = AIPathFindNode(world->ai_sys, NULL, "path2");
    LevAIPathNode[1] = path2;
    void *path3 = AIPathFindNode(world->ai_sys, NULL, "path3");
    LevAIPathNode[2] = path3;
    void *path4 = AIPathFindNode(world->ai_sys, NULL, "path4");
    LevAIPathNode[3] = path4;
    char buf[0x40];
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, 0, path1, path2, buf);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, 0, path2, path3, buf);
    LevPathCnx[2] = AIPAthFindPathCnx(world->ai_sys, 0, path3, path4, buf);
    LevPathCnx[3] = AIPAthFindPathCnx(world->ai_sys, 0, path4, (void *)"conn", buf);
    dookuC_nodesNeedUpdating = 1;
}

void DookuC_Reset(WORLDINFO_s *world) {
    dooku_c = 0;
    dooku_state = {};
    if (netclient == 0) {
        dooku_c = SetGizAIMessage(gizaimessagesys, "dooku_total", 0.0f, NULL);
        dooku_state.hit_message = (i32)(usize)CheckGizAIMessage(gizaimessagesys, "dooku_hits", NULL);
    }
    NuSpecialFind(world->current_gscn, &dooku_state.node, "dooku_node", 1);
}

void DookuC_Update(WORLDINFO_s *world) {
    if (netclient == 0) {
        if (FreePlay != 0) {
            KillBossCompleteLevel((i32)(i16)id_COUNTDOOKU, 0, 0.0f);
        } else if (DOOKUOUTRO_LDATA != NULL) {
            KillBossNewLevel((i32)(i16)id_COUNTDOOKU, 0, 0.0f, DOOKUOUTRO_LDATA->idx);
        }
    }
    DrawForceBackEffect(&dooku_state.node);
}

void DookuC_DrawPanel(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    GameObject_s *obj = (GameObject_s *)FindGameObject((i32)(i16)id_COUNTDOOKU, 1, 1, 1, 0);
    if (obj != NULL && dooku_c != 0 && obj->apiobj.anim_packet.time_secondary == 1.0f)
        DrawBossHitPoints(obj);
}
