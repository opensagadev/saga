#include <string.h>
#include <stdio.h>

#include "decomp.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/render/fx/parts.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
extern "C" AIPATHNODE_s *AIPathFindNode(AISYS_s *, AIPATH_s *, char *);
extern "C" void AIPathNodeUpdatePos(AISYS_s *, AIPATH_s *, AIPATHNODE_s *);
extern "C" i32 GetSfxId(const char *);
extern "C" void PlaySfxByIdAndSetVolume(i32, NUVEC *, f32);
void TiePart_Kill(PART_s *, i32) asm("_ZL12TiePart_KillP6PART_si") __attribute__((visibility("hidden")));
void TiePart_Move(PART_s *, f32) asm("_ZL12TiePart_MoveP6PART_sf") __attribute__((visibility("hidden")));
void TiePart_Impact(PART_s *) asm("_ZL14TiePart_ImpactP6PART_s") __attribute__((visibility("hidden")));
void TiePart_KillExplode(PART_s *, i32) asm("_ZL19TiePart_KillExplodeP6PART_si") __attribute__((visibility("hidden")));
void TieSpinZPart_Move(PART_s *, f32) asm("_ZL17TieSpinZPart_MoveP6PART_sf") __attribute__((visibility("hidden")));
void TrenchMove(GameObject_s *) asm("_ZL10TrenchMoveP12GameObject_s") __attribute__((visibility("hidden")));
void TrenchKilledCallback(GameObject_s *) asm("_ZL20TrenchKilledCallbackP12GameObject_s")
    __attribute__((visibility("hidden")));
i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
NUGSPLINE *edSpline_SplineFind(NUGSCN *, char *);
extern "C" {
    extern i16 id_STORMTROOPER;
    extern i16 id_BEACHTROOPER;
    extern i16 id_IMPERIALSHUTTLEPILOT;
}

struct BLOCKADERUNNERD_LEVFLAG_s {
    u8 obstacle15_active;
    u8 obstacle14_active;
    u8 reserved_02[0x0e];
};

static_assert(sizeof(BLOCKADERUNNERD_LEVFLAG_s) == 0x10, "LevFlag size");
extern BLOCKADERUNNERD_LEVFLAG_s LevFlag;
i32 test_tb = 1;
void *deathstarescapeb_netpacket;
u8 tatooineA_nodesNeedUpdating = 1;
u8 mosEisleyB_nodesNeedUpdating = 1;
f32 trench_spawn_height = 40.0f;
f32 trench_spawn_distance = 20.0f;

// Episode 4 level handlers, in the game's Episode_IV progression:
// blockade runner / tatooine / mos eisley / death star rescue / escape /
// battle.

// ===========================================================================
// Blockade runner (BlockadeRunner_B / BlockadeRunner_C / BlockadeRunner_D)
// ===========================================================================

void BlockadeRunnerB_Init(WORLDINFO_s *world) {
    LevBlowUp[0] = GizmoBlowUp_FindByName(world, "thermo_041");
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "arm_pop_1_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "arm_pop_2_2", 1);

    if (NuSpecialExistsFn(&LevHSpecial[0])) {
        char *names[5] = {"arm_1_null1", "arm_1_null2", "arm_1_null3", "arm_1_null4", "arm_2_null1"};
        for (i32 i = 0; i < 5; ++i) {
            GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[0];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }
    if (NuSpecialExistsFn(&LevHSpecial[1])) {
        char *names[5] = {"arm_1_null5", "arm_1_null6", "arm_1_null7", "arm_1_null8", "arm_2_null2"};
        for (i32 i = 0; i < 5; ++i) {
            GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[1];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }

    GIZBUILDIT_s *buildit = GizBuildIt_Find(world, "buildit5");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
    buildit = GizBuildIt_Find(world, "buildit6");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
    buildit = GizBuildIt_Find(world, "buildit7");
    if (buildit != NULL)
        buildit->radius_scale = 2.0f;
}

void BlockadeRunnerC_Init(WORLDINFO_s *world) {
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle8");
}

void BlockadeRunnerB_Update(WORLDINFO_s *) {
    if (LevBlowUp[0] != NULL && InStory())
        LevBlowUp[0]->state_flags &= ~0x80;
}

static i32 PartKill_DrawCreature(PART_s *) {
    return false;
}

static void PartKill_EjectedCreature(PART_s *part, i32) {
    for (i32 i = 0; i < 8; ++i) {
        if (LevGamePart[i] == part) {
            if (LevGameObject[i] != NULL) {
                KillGameObject(LevGameObject[i], 4, 0);
                LevGameObject[i] = NULL;
            }
            LevGamePart[i] = NULL;
        }
    }
}

static void BlockadeRunnerD_EjectCreature(i32 eject_index) {
    static NUVEC eject_vec[2] = {{13.0f, 1.6f, 7.93f}, {13.0f, 1.6f, 6.55f}};
    static NUVEC eject_mom = {-0.5f, 0.0f, 0.0f};
    i32 models[4] = {id_STORMTROOPER, id_BEACHTROOPER, id_STORMTROOPER, id_IMPERIALSHUTTLEPILOT};

    if (static_cast<u32>(eject_index) > 1)
        return;

    for (i32 i = 0; i < 8; ++i) {
        if (LevGamePart[i] == NULL) {
            NUMTX matrix;
            NuMtxSetTranslation(&matrix, &eject_vec[eject_index]);
            i32 model = models[qrand() / 0x4000];
            LevGameObject[i] =
                AddDynamicCreature(model, &eject_vec[eject_index], 0, "UST", NULL, NULL, 0, NULL, NULL, 0, 0);
            if (LevGameObject[i] != NULL) {
                LevGameObject[i]->ai.animation_override_from = 0xe9;
                LevGameObject[i]->ai.animation_override_to = 5;
                LevGameObject[i]->field_0xefc |= 0x10;
                LevGameObject[i]->field_0xf00 = (LevGameObject[i]->field_0xf00 & ~0x20) | ((test_tb & 1) << 5);
                LevGameObject[i]->field_0xeff |= 4;

                ADDPART_s params = Default_ADDPART;
                params.matrix = &matrix;
                params.velocity = &eject_mom;
                params.field_14 = 0.15f;
                params.field_18 = 0.15f;
                params.gravity = 0.0f;
                params.special = &WORLD->lev_objs[254].special;
                params.flags = 0x8698;
                params.field_40 = PartCollide_3D;
                params.field_44 = PartKill_EjectedCreature;
                params.draw_fn = PartKill_DrawCreature;
                params.time_step = FRAMETIME;
                LevGamePart[i] = AddPart(&params);
                LevGamePart[i]->field_100 = 30.0f;
                return;
            }
        }
    }
}

void BlockadeRunnerD_Update(WORLDINFO_s *world) {
    if (netclient != 0)
        return;

    for (i32 i = 0; i < 8; i++) {
        GameObject_s *game_object = LevGameObject[i];
        if (game_object == nullptr)
            continue;
        PART_s *part = LevGamePart[i];
        if (part == nullptr)
            continue;

        if ((part->active & 1) == 1) {
            game_object->field_0x1086 = 5;
            game_object->vehicle_orientation = part->transform;
            game_object->saved_position = game_object->apiobj.position = part->position;
        }

        if ((part->active & 1) == 0 || part->field_1c0 != &PartKill_EjectedCreature) {
            KillGameObject(game_object, 4, 0);
            LevGameObject[i] = nullptr;
        }
    }

    if (GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 1, 1) != 0) {
        if (LevFlag.obstacle15_active == 0) {
            LevFlag.obstacle15_active = 1;
            BlockadeRunnerD_EjectCreature(0);
        }
    } else {
        LevFlag.obstacle15_active = 0;
    }

    if (GizmoGetOutput(world->gizmo_sys, LevGizmo[1], 1, 1) != 0) {
        if (LevFlag.obstacle14_active == 0) {
            LevFlag.obstacle14_active = 1;
            BlockadeRunnerD_EjectCreature(1);
        }
    } else {
        LevFlag.obstacle14_active = 0;
    }
}

void BlockadeRunnerD_Reset(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle15");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle14");
    LevFlag.obstacle15_active = 0;
    LevFlag.obstacle14_active = 0;
}

// ===========================================================================
// Tatooine (Tatooine_A / B / C / D)
// ===========================================================================

void TatooineA_Init(WORLDINFO_s *world) {
    NUGSPLINE *spline = edSpline_SplineFind(world->current_gscn, "teleport_01");
    if (spline != NULL)
        spline->pts[0].x += 0.3f;

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "thermaldet_011");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "thermaldet_021");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "bhblock_011");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    blowup = GizmoBlowUp_FindByName(world, "bhblock_021");
    if (blowup != NULL)
        blowup->draw_flags |= 2;

    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force13");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force14");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force15");
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "force1_b");

    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_a", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_c", &direction);

    GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, "force16");
    if (force != NULL)
        force->field_0xaa |= 0x80;
    tatooineA_nodesNeedUpdating = 1;
}

void TatooineB_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_barrel14");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel13");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel15");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel16");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel17");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel18");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_barrel19");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
}

void TatooineC_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b = GizmoBlowUp_FindByName(world, "boxblowup_3");
    if (b != NULL)
        b->field_0xa0 |= 0x10000;
}

void TatooineD_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "final_bpush", 1);
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "box1_c");

    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "box1_c", "box1_a", &direction);
    if (direction)
        LevPathCnxDir |= 1;

    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "box1_c", "box1_b", &direction);
    if (direction)
        LevPathCnxDir |= 2;
}

void TatooineA_Update(WORLDINFO_s *world) {
    GIZFORCE_s **forces = LevGizForce;
    GIZFORCE_s *first = forces[0];
    if (first == NULL)
        return;
    GIZFORCE_s *second = forces[1];
    if (second == NULL)
        return;
    GIZFORCE_s *third = forces[2];
    if (third == NULL)
        return;

    GIZFORCEGROUP_s *group = first->group;
    if (__builtin_expect(group != NULL && (group->field_0x24 & 2) != 0, 0)) {
        if (tatooineA_nodesNeedUpdating != 0)
            return;
        tatooineA_nodesNeedUpdating = 1;
        AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }
        connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }

        AIPATHNODE_s *node = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
        if (node == NULL)
            return;
        GIZFORCE_s *selected = group->forces[2];
        if (selected == first) {
            node->position.x = -16.44f;
            node->position.z = -0.16f;
        } else if (selected == second) {
            node->position.x = -16.58f;
            node->position.z = -0.21f;
        } else if (selected == third) {
            node->position.x = -16.71f;
            node->position.z = -0.25f;
        }
        if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL)
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node);
        return;
    }

    if (tatooineA_nodesNeedUpdating == 0)
        return;
    tatooineA_nodesNeedUpdating = 0;
    AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
    connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
}

void TatooineD_Update(WORLDINFO_s *world) {
    AIPATHNODE_s *node = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
    if (node != NULL && NuSpecialExistsFn(&LevHSpecial[0])) {
        NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[0]);
        if (position != NULL) {
            node->position.x = position->x - 0.75f;
            node->position.z = position->z;
            if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL)
                AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node);
        }
    }

    AIPATHCNX_s *first = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
    AIPATHCNX_s *second = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
    if (first != NULL && second != NULL) {
        i32 direction = LevPathCnxDir;
        if (second->traversal_flags[(direction >> 1) & 1] & 0x08000000)
            first->traversal_flags[direction & 1] |= 0x80000000;
        else
            first->traversal_flags[direction & 1] &= ~0x80000000;
    }
}

// ===========================================================================
// Mos Eisley (MosEisley_A / B / D / E)
// ===========================================================================

void MosEisleyA_Init(WORLDINFO_s *world) {
    char name[32];
    for (i32 i = 1; i <= 5; ++i) {
        sprintf(name, "big_bin_lid_gr%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL)
            blowup->field_0x124 = 1;
    }
    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL)
            blowup->field_0x124 = 1;
    }

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "evap_091");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "evap_061");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
        blowup->draw_flags |= 2;
    }

    GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, "force1");
    if (force != NULL)
        force->strength_0x6c = 0.75f;
    force = GizForce_FindByName(world->giz_force_sys, "obstacle1");
    if (force != NULL)
        force->strength_0x6c = 0.75f;
}

void MosEisleyB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "land_speeder_terrain1", 1);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "Wash");
    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force1");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force2");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force3");

    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "stack_a");
    LevAIPathNode[1] = AIPathFindNode(world->ai_sys, NULL, "stack_b");
    LevAIPathNode[2] = AIPathFindNode(world->ai_sys, NULL, "stack_c");
    LevAIPathNode[3] = AIPathFindNode(world->ai_sys, NULL, "stack_d");
    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_a", "stack_b", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_b", "stack_c", &direction);
    LevPathCnx[2] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_c", "stack_d", &direction);
    LevPathCnx[3] = AIPAthFindPathCnx(world->ai_sys, NULL, "stack_d", "stack_e", &direction);
    mosEisleyB_nodesNeedUpdating = 1;

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "junk_071");
    if (blowup != NULL) {
        blowup->field_0x125[0] = 1;
        blowup->draw_flags |= 0x10000;
    }
    blowup = GizmoBlowUp_FindByName(world, "junk_081");
    if (blowup != NULL) {
        blowup->field_0x125[0] = 1;
        blowup->draw_flags |= 0x10000;
    }
    blowup = GizmoBlowUp_FindByName(world, "evap_082");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->draw_flags |= 0x18000;
    }

    if (NuSpecialExistsFn(&LevHSpecial[0])) {
        char *names[5] = {"arm_1_null1", "arm_1_null2", "arm_1_null3", "arm_1_null4", "arm_2_null1"};
        for (i32 i = 0; i < 5; ++i) {
            blowup = GizmoBlowUp_FindByName(world, names[i]);
            if (blowup != NULL && blowup->type != NULL) {
                blowup->field_0x124 = 1;
                blowup->override_special = &LevHSpecial[0];
                blowup->draw_flags |= 0xc00000;
                GizBlowup_InitSingleTerrain(blowup);
            }
        }
    }

    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "heater_3_1_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "heater_3_1_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "heater_3_1_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "heater_3_1_4", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "heater_3_1_5", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[6], "heater_3_1_6", 1);

    blowup = GizmoBlowUp_FindByName(world, "null_pop1");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[3];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop2");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[4];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop3");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[5];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
    blowup = GizmoBlowUp_FindByName(world, "null_pop11");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.7f;
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[6];
        blowup->draw_flags |= 0xc10000;
        GizBlowup_InitSingleTerrain(blowup);
    }
}

void MosEisleyD_Init(WORLDINFO_s *world) {
    char name[32];
    char *special_names[6] = {"big_gate_1a", "big_gate_1b", "big_gate_2a", "big_gate_2b", "big_gate_3a", "big_gate_3b"};
    for (i32 i = 1; i <= 6; ++i) {
        sprintf(name, "NULL_door_pop%d", i);
        NuSpecialFind(world->current_gscn, &LevHSpecial[i - 1], special_names[i - 1], 1);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x128 = 0.7f;
            blowup->field_0x124 = 1;
            blowup->override_special = &LevHSpecial[i - 1];
            blowup->draw_flags |= 0xc00000;
            GizBlowup_InitSingleTerrain(blowup);
        }
    }

    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x124 = 1;
            blowup->draw_flags |= 0x10000;
        }
    }
    for (i32 i = 1; i <= 8; ++i) {
        sprintf(name, "big_bin_lid_gr%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup != NULL) {
            blowup->field_0x124 = 1;
            blowup->draw_flags |= 0x10000;
        }
    }

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "evap_041");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "evap_031");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
}

void MosEisleyE_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force1");
    GIZFORCE_s *force = world->giz_force_sys->forces;
    for (i32 i = 0; i < world->giz_force_sys->count; i++, force++)
        force->state_flags |= 0x40;
}

void MosEisleyB_Update(WORLDINFO_s *world) {
    GIZFORCE_s **forces = LevGizForce;
    GIZFORCE_s *first = forces[0];
    if (first == NULL)
        return;
    GIZFORCE_s *second = forces[1];
    if (second == NULL)
        return;
    GIZFORCE_s *third = forces[2];
    if (third == NULL)
        return;

    GIZFORCEGROUP_s *group = first->group;
    if (__builtin_expect(group != NULL && (group->field_0x24 & 2) != 0, 0)) {
        if (mosEisleyB_nodesNeedUpdating != 0)
            return;
        mosEisleyB_nodesNeedUpdating = 1;
        for (i32 i = 0; i < 4; ++i) {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[i]);
            if (connection != NULL) {
                connection->traversal_flags[0] &= ~0x80000000;
                connection->traversal_flags[1] &= ~0x80000000;
            }
        }

        AIPATHNODE_s *node0 = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
        if (node0 == NULL)
            return;
        AIPATHNODE_s *node1 = static_cast<AIPATHNODE_s *>(LevAIPathNode[1]);
        if (node1 == NULL)
            return;
        AIPATHNODE_s *node2 = static_cast<AIPATHNODE_s *>(LevAIPathNode[2]);
        if (node2 == NULL)
            return;
        AIPATHNODE_s *node3 = static_cast<AIPATHNODE_s *>(LevAIPathNode[3]);
        if (node3 == NULL)
            return;

#define SET_STACK_NODE_POSITIONS(x0, z0, x1, z1, x2, z2, x3, z3)                                                       \
    do {                                                                                                               \
        node0->position.x = x0;                                                                                        \
        node0->position.z = z0;                                                                                        \
        node1->position.x = x1;                                                                                        \
        node1->position.z = z1;                                                                                        \
        node2->position.x = x2;                                                                                        \
        node2->position.z = z2;                                                                                        \
        node3->position.x = x3;                                                                                        \
        node3->position.z = z3;                                                                                        \
    } while (0)

        GIZFORCE_s *selected = group->forces[0];
        if (selected == first) {
            if (group->forces[1] == second)
                SET_STACK_NODE_POSITIONS(24.12f, -14.69f, 23.75f, -14.78f, 23.32f, -14.91f, 23.32f, -14.63f);
            else
                SET_STACK_NODE_POSITIONS(24.12f, -14.69f, 23.75f, -14.78f, 23.34f, -14.57f, 23.34f, -14.84f);
        } else if (selected == second) {
            if (group->forces[1] == first)
                SET_STACK_NODE_POSITIONS(22.75f, -15.12f, 23.27f, -14.93f, 23.72f, -14.78f, 23.32f, -14.61f);
            else
                SET_STACK_NODE_POSITIONS(22.75f, -15.12f, 23.27f, -14.93f, 23.27f, -14.62f, 23.66f, -14.73f);
        } else if (selected == third) {
            if (group->forces[1] == first)
                SET_STACK_NODE_POSITIONS(22.79f, -14.62f, 23.29f, -14.58f, 23.73f, -14.70f, 23.34f, -14.87f);
            else
                SET_STACK_NODE_POSITIONS(22.79f, -14.62f, 23.29f, -14.58f, 23.27f, -14.85f, 23.63f, -14.76f);
        }
#undef SET_STACK_NODE_POSITIONS

        if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL) {
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node0);
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node1);
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node2);
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node3);
        }
        return;
    }

    if (mosEisleyB_nodesNeedUpdating == 0)
        return;
    mosEisleyB_nodesNeedUpdating = 0;
    for (i32 i = 0; i < 4; ++i) {
        AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[i]);
        if (connection != NULL) {
            connection->traversal_flags[0] |= 0x80000000;
            connection->traversal_flags[1] |= 0x80000000;
        }
    }
}

void MosEisleyE_Update(WORLDINFO_s *) {
    if ((texanimbits & 2) == 0 && LevGizmo[0] != NULL && LevGizmo[0]->object != NULL &&
        GizForce_Complete(static_cast<GIZFORCE_s *>(LevGizmo[0]->object)))
        texanimbits |= 2;
}

void MosEisleyE_Reset(WORLDINFO_s *) {
    texanimbits &= ~2;
}

void MosEisleyD_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, MOSEISLEYE_LDATA, 7.5f, NULL);
}

i32 MosEisleyC_PastBarrier(GameObject_s *object) {
    if (WORLD->current_level == MOSEISLEYC_LDATA)
        return object->apiobj.position.z > 5.85f;
    return 0;
}

// ===========================================================================
// Death Star rescue (DeathStarRescue_B / C)
// ===========================================================================

void DeathStarRescueB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "reactor_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "reactor_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "reactor_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "reactor_4", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "reactor_5", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "reactor_6", 1);

    volatile u8 *flags = reinterpret_cast<volatile u8 *>(&LevFlag);
    flags[5] = 0;
    flags[4] = 0;
    flags[3] = 0;
    flags[2] = 0;
    flags[1] = 0;
    flags[0] = 0;

    LevSfxId[0] = GetSfxId("env_tractorbeam_lp");
    LevSfxId[1] = GetSfxId("env_tractorbeam_off");

    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "dummy_exp2");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.2f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_1");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_2");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_3");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_4");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_5");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
    blowup = GizmoBlowUp_FindByName(world, "blowup_block_6");
    if (blowup != NULL)
        blowup->draw_flags |= 0x10000;
}

void DeathStarRescueC_Init(WORLDINFO_s *world) {
#define SETUP_RESCUE_PANEL(name)                                                                                       \
    do {                                                                                                               \
        GIZMOBLOWUP_s *panel = GizmoBlowUp_FindByName(world, name);                                                    \
        if (panel != NULL) {                                                                                           \
            panel->field_0x128 = 0.3f;                                                                                 \
            panel->field_0x124 = 1;                                                                                    \
        }                                                                                                              \
    } while (0)
    SETUP_RESCUE_PANEL("panel_11");
    SETUP_RESCUE_PANEL("panel_21");
    SETUP_RESCUE_PANEL("panel_31");
    SETUP_RESCUE_PANEL("panel_41");
    SETUP_RESCUE_PANEL("panel_51");
    SETUP_RESCUE_PANEL("panel_61");
    SETUP_RESCUE_PANEL("panel_71");
    SETUP_RESCUE_PANEL("panel_81");
#undef SETUP_RESCUE_PANEL
}

void DeathStarRescueB_Update(WORLDINFO_s *world) {
    u8 *flags = reinterpret_cast<u8 *>(&LevFlag);
#define UPDATE_RESCUE_REACTOR(index)                                                                                   \
    do {                                                                                                               \
        if (flags[index] == 0) {                                                                                       \
            NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[index]);                                                \
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[index]);                                       \
            if (animation != NULL) {                                                                                   \
                f32 frame = animation->ltime;                                                                          \
                f32 end = NuAnimEndFrameOld(world->current_gscn->instance_animation_data[animation->anim_ix]);         \
                PlaySfxByIdAndSetVolume(LevSfxId[0], position, 0.25f);                                                 \
                if (frame >= end) {                                                                                    \
                    PlaySfxByIdAndSetVolume(LevSfxId[1], NULL, 0.4f);                                                  \
                    flags[index] = 1;                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)
    UPDATE_RESCUE_REACTOR(0);
    UPDATE_RESCUE_REACTOR(1);
    UPDATE_RESCUE_REACTOR(2);
    UPDATE_RESCUE_REACTOR(3);
    UPDATE_RESCUE_REACTOR(4);
    UPDATE_RESCUE_REACTOR(5);
#undef UPDATE_RESCUE_REACTOR
}

void DeathStarRescueB_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, DEATHSTARRESCUED_LDATA, 8.5f, NULL);
}

void DeathStarRescueC_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, DEATHSTARRESCUEE_LDATA, 8.5f, NULL);
}

// ===========================================================================
// Death Star escape (DeathStarEscape_A / B / C / D)
// ===========================================================================

i32 DeathStarShieldDown() {
    if (LevGizmo[0] == NULL)
        LevGizmo[0] = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, "bigbang1");
    if (LevGizmo[0] == NULL || LevGizmo[0]->object == NULL)
        return 0;

    GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(LevGizmo[0]->object);
    if (blowup->output_flags & 1)
        return 1;
    if (__builtin_expect(static_cast<i8>(blowup->state_flags) < 0, 1)) {
        GameObject_s **players = Player;
        GameObject_s *player = players[0];
        if (player != NULL) {
            if (WORLD->current_level != DEATHSTARBATTLED_LDATA ||
                !ObjInNarrowSock(player, WORLD->sock_sys, WORLD->level_idx)) {
                TORPEDOPACKET_s *packet = player->torpedo;
                if (packet != NULL) {
                    if (packet->count != 0 || (packet->field_0x1 & 2) != 0)
                        return 1;
                }
            }
        }

        player = players[1];
        if (player != NULL) {
            if (WORLD->current_level != DEATHSTARBATTLED_LDATA ||
                !ObjInNarrowSock(player, WORLD->sock_sys, WORLD->level_idx)) {
                TORPEDOPACKET_s *packet = player->torpedo;
                if (packet != NULL) {
                    if (packet->count != 0)
                        return 1;
                    return (packet->field_0x1 >> 1) & 1;
                }
            }
        }
    }
    return 0;
}

void DeathStarEscapeA_Init(WORLDINFO_s *world) {
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle8");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle9");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle10");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle11");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle12");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle1");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
}

void DeathStarEscapeB_Init(WORLDINFO_s *world) {
    deathstarescapeb_netpacket = SetLevelHack(4);
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "WindowClean", NULL);
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "lift_up", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "lift_down", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "sponge", 1);

    fakeanimendframe[0] = 0.0f;
    nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[0]);
    if (animation != NULL) {
        nuanimdata_s *data = LevHSpecial[0].scene->instance_animation_data[animation->anim_ix];
        if (data != NULL)
            fakeanimendframe[0] += NuAnimEndFrameOld(data);
    }
    animation = NuSpecialGetInstAnim(&LevHSpecial[1]);
    if (animation != NULL) {
        nuanimdata_s *data = LevHSpecial[1].scene->instance_animation_data[animation->anim_ix];
        if (data != NULL)
            fakeanimendframe[0] += NuAnimEndFrameOld(data);
    }
}

void DeathStarEscapeB_Draw(WORLDINFO_s *) {
    if (LevGameObject[0] != NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0) {
        NUMTX matrix = LevGameObject[0]->joint_matrices[1];
        NuSpecialDrawAt(&LevHSpecial[2], &matrix);
    }
    if (LevGameObject[1] != NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0) {
        NUMTX matrix = LevGameObject[1]->joint_matrices[1];
        NuSpecialDrawAt(&LevHSpecial[2], &matrix);
    }
}

void DeathStarEscapeC_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_door_31");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "cup_built1");
    if (blowup != NULL)
        blowup->draw_flags |= 2;
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "door_push", 1);
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "droid_rescue_a",
                                      "droid_rescue_b", &LevPathCnxDir);
}

void DeathStarEscapeA_Update(WORLDINFO_s *) {
    if (GameCam->sock_position.location.sock == 2) {
        NUVEC position = {0.0f, -5.5f, 0.0f};
        position.x = QRAND_FLOAT() * 2.0f + 36.5f;
        position.z = QRAND_FLOAT() * 2.0f + 8.5f;
        LevChatterSfx("Dianoga_Groan", &position);
    }
}

void DeathStarEscapeB_Update(WORLDINFO_s *world) {
    nuanimdata_s *volatile data0 = NULL;
    nuanimdata_s *volatile data1 = NULL;
    if (netclient == 0)
        *static_cast<u8 *>(deathstarescapeb_netpacket) = static_cast<u8>(static_cast<i32>(LevAIMessage[0]->value));

    if (LevGameObject[0] == NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0)
        LevGameObject[0] = GetNamedGameObject(world->ai_sys, "WASHER_1");
    if (LevGameObject[1] == NULL && *static_cast<u8 *>(deathstarescapeb_netpacket) == 0)
        LevGameObject[1] = GetNamedGameObject(world->ai_sys, "WASHER_2");
    nuinstanim_s *animation0 = NuSpecialGetInstAnim(&LevHSpecial[0]);
    if (animation0 != NULL)
        data0 = LevHSpecial[0].scene->instance_animation_data[animation0->anim_ix];
    nuinstanim_s *animation1 = NuSpecialGetInstAnim(&LevHSpecial[1]);
    if (animation1 != NULL)
        data1 = LevHSpecial[1].scene->instance_animation_data[animation1->anim_ix];
    if (data0 == NULL || data1 == NULL)
        return;

    if (NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        fakeanimframe[0] = animation0->ltime;
    } else if (NuSpecialGetVisibilityFn(&LevHSpecial[1])) {
        fakeanimframe[0] = NuAnimEndFrameOld(data0) + animation1->ltime;
    } else {
        fakeanimframe[0] = 0.0f;
    }
}

void DeathStarEscapeC_Update(WORLDINFO_s *) {
    if (LevPathCnx[0] != NULL) {
        NUVEC *position;
        if (NuSpecialGetVisibilityFn(&LevHSpecial[2]) && (position = NuSpecialGetDrawPos(&LevHSpecial[2])) != NULL &&
            position->x < 77.25f) {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
            if (LevAIMessage[0] != NULL)
                LevAIMessage[0]->value = 1.0f;
        } else {
            AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
            connection->traversal_flags[0] |= 0x80000000;
            connection->traversal_flags[1] |= 0x80000000;
        }
    }
}

void DeathStarEscapeD_Update(WORLDINFO_s *) {
    NUVEC position = {0.0f, -5.5f, 0.0f};
    position.x = QRAND_FLOAT() * 4.0f + 32.5f;
    position.z = QRAND_FLOAT() * 4.0f + 12.5f;
    LevChatterSfx("Dianoga_Groan", &position);
}

void DeathStarEscapeC_Reset(WORLDINFO_s *) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "RescuedDroids", NULL);
}

void DeathStarEscapeB_AlwaysUpdate(WORLDINFO_s *) {
    NUVEC position;
    SOCKPOSITION socket_position;
    if (Players_AveragePos(&position, &socket_position) && socket_position.location.sock != -1) {
        if (DEATHSTARESCAPEA_LDATA != NULL && socket_position.location.sock < 6)
            other_level_override = DEATHSTARESCAPEA_LDATA->idx;
        else if (DEATHSTARESCAPEC_LDATA != NULL && socket_position.location.sock > 6)
            other_level_override = DEATHSTARESCAPEC_LDATA->idx;
    }
}

void KillParts_TIEFIGHTER(ADDPART_s *params, i32 part_index, i32 variant, GameObject_s *object, i32 mode, u16 xrot,
                          u16 yrot, nuvec_s *velocity) {
    params->flags = static_cast<u32>(variant) < 1 ? 0x400 : 0x10;
    f32 speed_ratio =
        object->apiobj.horizontal_velocity_magnitude / object->apiobj.character_data->game_character->run_speed;
    if (speed_ratio <= 0.25f) {
        params->flags = 0x90;
        params->stop_fn = PartStop_Flickerer;
        params->draw_fn = PartDraw_Flickerer;
        params->field_3c = PartImpact_Brick;
        params->gravity = -8.0f;
        params->velocity = velocity;
        AddPart(params);
        return;
    }

    if (mode == 0) {
        params->field_44 = TiePart_KillExplode;
        params->flags = 0x111;
        params->field_a4 = FRAMETIME;
        params->velocity = &v000;
        AddPart(params);
        return;
    }

    if (__builtin_expect(mode == 1, 0)) {
        NUVEC spin_velocity = {object->apiobj.velocity.x * 0.75f, object->apiobj.velocity.x * 0.75f,
                               object->apiobj.velocity.z * 0.75f};
        if (part_index == 5) {
            NuVecRotateX(&spin_velocity, &spin_velocity, -static_cast<i32>(xrot));
            NuVecRotateY(&spin_velocity, &spin_velocity, -static_cast<i32>(yrot));
            params->move_fn = TiePart_Move;
        } else {
            NuVecRotateX(&spin_velocity, &spin_velocity, xrot);
            NuVecRotateY(&spin_velocity, &spin_velocity, yrot);
            params->move_fn = TieSpinZPart_Move;
        }
        params->flags = 0x111;
        params->field_a4 = 2.0f;
        params->velocity = &spin_velocity;
        params->field_44 = TiePart_Kill;
        params->field_3c = TiePart_Impact;
        PlaySfx("Tie_Spins", &object->apiobj.collision_position);
        AddPart(params);
        return;
    }

    params->field_20 = 0.5f;
    params->field_44 = TiePart_Kill;
    params->flags = 0x91;
    params->draw_fn = PartDraw_Flickerer;
    params->field_3c = TiePart_Impact;
    params->velocity = velocity;
    AddPart(params);
}

// ===========================================================================
// Death Star battle (DeathStarBattle_C / D)
// ===========================================================================

void DeathStarBattleC_AlwaysUpdate(WORLDINFO_s *) {
    if (DEATHSTARBATTLEMIDTRO_LDATA != NULL)
        other_level_override = DEATHSTARBATTLEMIDTRO_LDATA->idx;
}

void DeathStarBattleDDraw(WORLDINFO_s *) {
}

void DeathStarBattleDInit(WORLDINFO_s *) {
}

void DeathStarBattleDReset(WORLDINFO_s *) {
    memset(&trenchrun, 0, sizeof(trenchrun));
}

void DeathStarBattleDUpdate(WORLDINFO_s *world) {
    if (netclient == 0) {
        trenchrun.position.y = -10.5f;
        trenchrun.position.x = player->apiobj.collision_position.x;
        if (trenchrun.position.x < -500.0f) {
            trenchrun.position.y = trench_spawn_height - 10.5f;
            i32 &midtro = *reinterpret_cast<i32 *>(&trenchrun.reserved_0x18[0]);
            if (FreePlay == 0 && midtro == 0) {
                if (trenchrun.objects[0] != NULL) {
                    KillGameObject(trenchrun.objects[0], 4, 0);
                    trenchrun.objects[0] = NULL;
                }
                if (trenchrun.objects[1] != NULL) {
                    KillGameObject(trenchrun.objects[1], 4, 0);
                    trenchrun.objects[1] = NULL;
                }
                if (trenchrun.objects[2] != NULL) {
                    KillGameObject(trenchrun.objects[2], 4, 0);
                    trenchrun.objects[2] = NULL;
                }
                NewCutScene(NULL, world->cutscene_sys, "deathstarbattle_midtro_ingame", 1);
                midtro = 1;
            }
        }

        trenchrun.position.z = player->apiobj.collision_position.z;
        f32 lower = trenchrun.objects[0] != NULL ? 62.0f : 60.0f;
        f32 upper = trenchrun.objects[2] != NULL ? 70.0f : 72.0f;
        if (trenchrun.position.z < lower)
            trenchrun.position.z = lower;
        else if (trenchrun.position.z > upper)
            trenchrun.position.z = upper;

        if (aicreature_sets_alive[0] == 0) {
            f32 &timer = *reinterpret_cast<f32 *>(&trenchrun.reserved_0x18[4]);
            timer -= FRAMETIME;
            if (timer <= 0.0f) {
                timer = 5.0f;
#define SPAWN_TRENCH_SHIP(slot, model, ox, oy, oz)                                                                     \
    do {                                                                                                               \
        NUVEC spawn = {player->apiobj.collision_position.x + trench_spawn_distance,                                    \
                       player->apiobj.collision_position.y + trench_spawn_height,                                      \
                       player->apiobj.collision_position.z};                                                           \
        NUVEC offset = {ox, oy, oz};                                                                                   \
        NuVecAdd(&spawn, &spawn, &offset);                                                                             \
        GameObject_s *ship = AddDynamicCreature(model, &spawn, 0xc000, "TrenchBaddie", &player->ai.path_info, NULL, 0, \
                                                NULL, NULL, 0, 1);                                                     \
        if (ship != NULL) {                                                                                            \
            ship->field_0xf04 |= 4;                                                                                    \
            ship->field_0xeb4 = TrenchKilledCallback;                                                                  \
            ship->move_override = TrenchMove;                                                                          \
            ship->movement_spline_offset = offset;                                                                     \
            trenchrun.objects[slot] = ship;                                                                            \
        }                                                                                                              \
    } while (0)
                SPAWN_TRENCH_SHIP(0, id_TIEFIGHTER, 0.0f, 0.0f, 2.0f);
                i32 model =
                    *reinterpret_cast<i32 *>(&trenchrun.reserved_0x18[0]) != 0 ? id_TIEFIGHTER : id_TIEFIGHTERDARTH;
                SPAWN_TRENCH_SHIP(1, model, 0.0f, 1.0f, 0.0f);
                SPAWN_TRENCH_SHIP(2, id_TIEFIGHTER, 0.0f, 0.0f, -2.0f);
#undef SPAWN_TRENCH_SHIP
            }
        }
    }

    volatile u8 *flags = reinterpret_cast<volatile u8 *>(&LevFlag);
    if (flags[4] != 0)
        return;
    if (NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        PlaySfx("ffield", NuSpecialGetDrawPos(&LevHSpecial[0]));
    } else {
        PlaySfx("ffieldoff", NuSpecialGetDrawPos(&LevHSpecial[0]));
        flags[4] = 1;
    }
}
