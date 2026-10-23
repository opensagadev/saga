// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include <stdio.h>
#include <string.h>
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"

#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "gameapi/ai/aisys/aipath.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/gizmo/base/gizmo.h"

// Coruscant bounty-hunter pursuit levels (original pursuit.cpp translation unit).

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

// UpdateZamArrow is inlined into its callers apart from the message update,
// which retail keeps as the split-out body of the IPA-SRA clone (it only needs
// &world->lev_objs).
static __attribute__((noinline, noclone)) void
UpdateZamArrowBody(LEVEL_OBJECT_RUNTIME_s **lev_objs) __asm__("_ZL14UpdateZamArrowP11WORLDINFO_s.isra.5.part.6");

static inline void UpdateZamArrow(WORLDINFO_s *world) {
    if (zamarrow.object != NULL && (zamarrow.object->apiobj.field_0x1f8 & 0x1000) != 0 &&
        zamarrow.object->apiobj.field_0x287 == 0) {
        if (FadeSys.fade == 0.0f && pause_rndr_on == 0) {
            zamarrow.timer = (FRAMETIME + FRAMETIME) + zamarrow.timer;
            if (zamarrow.timer > 1.0f) {
                zamarrow.timer = 1.0f;
            }
            if (0.1f > NuFmod(GameTimer.time_elapsed_mod_seconds, 0.2f)) {
                UpdateZamArrowBody(&world->lev_objs);
            }
        } else {
            zamarrow.timer = 0.0f;
        }
    }
}

static void UpdateZamArrowBody(LEVEL_OBJECT_RUNTIME_s **lev_objs) {
    NUVEC position = zamarrow.object->apiobj.upper_position;
    position.y += 1.0f;
    GAMEMESSAGE_s *message = static_cast<GAMEMESSAGE_s *>(
        AddGameMessage(const_cast<char *>(" "), &position, 0.05f, NULL, 0.0f, 255, 63, 63, 0x10083, 0.0f));
    if (message == NULL) {
        return;
    }

    message->alpha = static_cast<u8>(NU_SIN_LUT(static_cast<i32>(zamarrow.timer * 16384.0f)) * 128.0f);
    message->icon = 0x134;
    PURSUIT_ARROW_COLOURS_s *colours = reinterpret_cast<PURSUIT_ARROW_COLOURS_s *>(*lev_objs);
    if (colours->enabled != 0) {
        message->color1 = colours->colour[0];
        message->color2 = colours->colour[1];
        message->color3 = colours->colour[2];
    }
}

// ===========================================================================
// Coruscant — bounty-hunter pursuit (Zam Wesell)
// ===========================================================================

void BountyHunterPursuitA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "Spire1")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire2")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire3")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire4")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire5")) != NULL)
        b->field_0x9f |= 0x20;
}

void BountyHunterPursuitA_Reset(WORLDINFO_s *world) {
    zamarrow = {};
    zamarrow.object = GetNamedGameObject(world->ai_sys, "ai_zam");
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "Spire1")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire2")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire3")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire4")) != NULL)
        b->field_0x9f |= 0x20;
    if ((b = GizmoBlowUp_FindByName(world, "Spire5")) != NULL)
        b->field_0x9f |= 0x20;
}

void BountyHunterPursuitA_Update(WORLDINFO_s *world) {
    UpdateZamArrow(world);
}

void BountyHunterPursuitB_Init(WORLDINFO_s *) {
    STUBBED();
}

void BountyHunterPursuitB_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator1");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator2");
    LevGizmo[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator3");
    LevGizmo[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator4");
    LevGizmo[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator5");
    LevGizmo[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator6");
    LevGameObject[0] = GetNamedGameObject(world->ai_sys, "ai_zam");
    zamarrow = {};
    zamarrow.object = GetNamedGameObject(world->ai_sys, "ai_zam");
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "CompletedB", NULL);
}

void BountyHunterPursuitB_Update(WORLDINFO_s *world) {
    if (LevAIMessage[0] == NULL || LevAIMessage[0]->value != 1.0f) {
        return;
    }

    UpdateZamArrow(world);
}

void BountyHunterPursuitC_Init(WORLDINFO_s *) {
    STUBBED();
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

void BountyHunterPursuitC_Update(WORLDINFO_s *world) {
    UpdateZamArrow(world);

    PURSUIT_TRAFFIC_s *traffic = reinterpret_cast<PURSUIT_TRAFFIC_s *>(world->trafficanim_sys);
    if (traffic == NULL || player == NULL) {
        return;
    }

    if (traffic_test_z > player->apiobj.position.z) {
        if (traffic->side == -1) {
            return;
        }
        traffic->side = -1;
    } else {
        if (traffic->side == 1) {
            return;
        }
        traffic->side = 1;
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

void BountyHunterPursuitD_Init(WORLDINFO_s *) {
    STUBBED();
}

void BountyHunterPursuitD_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator1");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator2");
    LevGizmo[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator3");
    LevGizmo[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator4");
    LevGizmo[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator5");
    LevGizmo[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator6");
    LevGizmo[6] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator7");
    LevGizmo[7] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator8");
    LevGizmo[8] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator9");
    LevGizmo[9] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator10");
    LevGizmo[10] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator11");
    LevGizmo[11] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "generator12");
    LevGameObject[0] = GetNamedGameObject(world->ai_sys, "ai_zam");
}

void BountyHunterPursuitD_Update(WORLDINFO_s *) {
    if (LevGameObject[0] == NULL || LevGameObject[0]->field_0xe37 == 0)
        return;

    f32 nearest_distance_sq = 1000000000.0f;
    GIZMOBLOWUP_s *nearest = NULL;

#define CHECK_PURSUIT_GENERATOR(index)                                                                                 \
    if (__builtin_expect(LevGizmo[index] != NULL, 1)) {                                                                                     \
        GIZMOBLOWUP_s *generator = static_cast<GIZMOBLOWUP_s *>(LevGizmo[index]->object);                              \
        if ((generator->output_flags & 1) == 0) {                                                                      \
            f32 distance_sq = NuVecDistSqr(&generator->position, &LevGameObject[0]->apiobj.collision_position, NULL);  \
            if (distance_sq < nearest_distance_sq) {                                                                   \
                nearest_distance_sq = distance_sq;                                                                     \
                nearest = generator;                                                                                   \
            }                                                                                                          \
        }                                                                                                              \
    }
    CHECK_PURSUIT_GENERATOR(0)
    CHECK_PURSUIT_GENERATOR(1)
    CHECK_PURSUIT_GENERATOR(2)
    CHECK_PURSUIT_GENERATOR(3)
    CHECK_PURSUIT_GENERATOR(4)
    CHECK_PURSUIT_GENERATOR(5)
    CHECK_PURSUIT_GENERATOR(6)
    CHECK_PURSUIT_GENERATOR(7)
    CHECK_PURSUIT_GENERATOR(8)
    CHECK_PURSUIT_GENERATOR(9)
    CHECK_PURSUIT_GENERATOR(10)
    CHECK_PURSUIT_GENERATOR(11)
#undef CHECK_PURSUIT_GENERATOR

    if (nearest == NULL) {
        LevGameObject[0]->field_0xe37 = 0;
        DrawBossHitPoints(LevGameObject[0]);
        return;
    }
    if (nearest_distance_sq < 1000000.0f) {
        NUVEC delta;
        f32 distance = NuVecDist(&LevGameObject[0]->apiobj.collision_position, &nearest->position, &delta);
        NuLgtLaser(0, 1.0f, 1.0f, 0.01f, &nearest->position, &delta, 0xff808040, 1.5f, distance);
    }
}

