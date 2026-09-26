#include <string.h>

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
#include "nu2api/numath/numtx.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
extern "C" AIPATHNODE_s *AIPathFindNode(AISYS_s *, AIPATH_s *, char *);
extern "C" void AIPathNodeUpdatePos(AISYS_s *, AIPATH_s *, AIPATHNODE_s *);
i32 GizBlowup_InitSingleTerrain(GIZMOBLOWUP_s *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
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

void TatooineA_Init(WORLDINFO_s *) {
    STUBBED();
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

void TatooineA_Update(WORLDINFO_s *) {
    STUBBED();
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

void MosEisleyA_Init(WORLDINFO_s *) {
    STUBBED();
}

void MosEisleyB_Init(WORLDINFO_s *) {
    STUBBED();
}

void MosEisleyD_Init(WORLDINFO_s *) {
    STUBBED();
}

void MosEisleyE_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force1");
    GIZFORCE_s *force = world->giz_force_sys->forces;
    for (i32 i = 0; i < world->giz_force_sys->count; i++, force++)
        force->state_flags |= 0x40;
}

void MosEisleyB_Update(WORLDINFO_s *) {
    STUBBED();
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

void DeathStarRescueB_Init(WORLDINFO_s *) {
    STUBBED();
}

void DeathStarRescueC_Init(WORLDINFO_s *) {
    STUBBED();
}

void DeathStarRescueB_Update(WORLDINFO_s *) {
    STUBBED();
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

void DeathStarEscapeB_Init(WORLDINFO_s *) {
    STUBBED();
}

__attribute__((force_align_arg_pointer)) void DeathStarEscapeB_Draw(WORLDINFO_s *) {
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

void DeathStarEscapeB_Update(WORLDINFO_s *) {
    STUBBED();
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

void KillParts_TIEFIGHTER(ADDPART_s *, i32, i32, GameObject_s *, i32, u16, u16, nuvec_s *) {
    STUBBED();
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

void DeathStarBattleDUpdate(WORLDINFO_s *) {
    STUBBED();
}
