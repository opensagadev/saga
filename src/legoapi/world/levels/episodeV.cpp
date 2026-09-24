#include "decomp.h"
#include "globals.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 dagobah_training;
AILOCATOR_s *locator;
GameObject_s *gameobj;
extern u8 troopercannons_beenReset;
void Asteroid_PartKill(PART_s *, i32);
void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *);
void PartCollide_3D(PART_s *);
void ResetTrooperCannons(WORLDINFO_s *, i32);
void UpdateTrooperCannons(WORLDINFO_s *);
EXPLOSION *Detonate(NUVEC *, u16);
extern "C" void NewPartRotation(PART_s *);
extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

static GameObject_s *Vader_obj;
static GIZAIMESSAGE_s *Vader_ai_message;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {
    GIZBOMBGEN *HothBattleC_BombGenerator = NULL;
    HOTHBATTLE_MELEE_s melee;
    u8 dagobahA_nodesNeedUpdating = 1;
}

void DagobahA_Init(WORLDINFO_s *world) {
    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force3");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force4");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force5");
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "force1_a");
    LevAIPathNode[1] = AIPathFindNode(world->ai_sys, NULL, "force1_b");
    LevAIPathNode[2] = AIPathFindNode(world->ai_sys, NULL, "force1_c");
    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_a", "force1_b", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_c", &direction);
    LevPathCnx[2] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_c", "force1_d", &direction);
    dagobahA_nodesNeedUpdating = 1;
}

void DagobahB_Init(WORLDINFO_s *) {
    dagobah_training = 0;
}

void DagobahC_Init(WORLDINFO_s *world) {
    Vader_obj = FindGameObject(id_DARTHVADER, 1, 1, 0, 0);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force20");
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "Thermo_Box1");
    if (blowup != NULL) {
        blowup->draw_flags |= 2;
    }
}

void DagobahE_Init(WORLDINFO_s *world) {
    GIZAIMESSAGE_s *completed = CheckGizAIMessage(gizaimessagesys, "CompletedTraining", NULL);
    if (FreePlay == 0 && completed != NULL && completed->value == 0.0f) {
        dagobah_training = 1;
        DOOR_s *door = Door_FindByName(world, "door_e_to_b");
        if (door != NULL) {
            door->flags |= DOOR_FLAG_DO_NOT_USE;
        }
        door = Door_FindByName(world, "door_b_to_e");
        if (door != NULL) {
            door->flags |= DOOR_FLAG_DO_NOT_USE;
        }
    } else {
        dagobah_training = 0;
        DOOR_s *door = Door_FindByName(world, "door_e_to_b");
        if (door != NULL) {
            door->flags &= ~DOOR_FLAG_DO_NOT_USE;
        }
        door = Door_FindByName(world, "door_b_to_e");
        if (door != NULL) {
            door->flags &= ~DOOR_FLAG_DO_NOT_USE;
        }
    }
    SetGizAIMessage(gizaimessagesys, "DagobahTraining", static_cast<f32>(dagobah_training), NULL);
    SetGizAIMessage(gizaimessagesys, NULL, 1.0f, completed);
}

void DagobahB_Reset(WORLDINFO_s *world) {
    LevSafePlatID[1] = -1;
    LevSafePlatID[0] = -1;

    if (NuSpecialFind(world->current_gscn, &LevHSpecial[0], "pad_2_base_2", 1) != 0) {
        if (world->terrain != NULL) {
            LevSafePlatID[0] = FindPlatInst(NuSpecialGetInstanceix(&LevHSpecial[0]));
        }
    }

    if (NuSpecialFind(world->current_gscn, &LevHSpecial[1], "pad_4_base_2", 1) != 0) {
        if (world->terrain != NULL) {
            LevSafePlatID[1] = FindPlatInst(NuSpecialGetInstanceix(&LevHSpecial[1]));
        }
    }
}

void DagobahC_Panel(WORLDINFO_s *) {
    if (netclient == 0) {
        Vader_ai_message = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
        if (Vader_obj != NULL && Vader_ai_message != NULL && Vader_ai_message->value == 1.0f) {
            DrawBossHitPoints(Vader_obj);
        }
    }
}

void KillParts_ATAT(ADDPART_s *, i32, i32, GameObject_s *) {
    STUBBED();
}

f32 rocket_speed = 1.2f;

void BobaRocket_Kill(PART_s *part, i32) {
    EXPLOSION *explosion = Detonate(&part->position, 0);
    if (explosion != NULL && Arcade != 0 && part->owner != NULL &&
        (Player[0] == part->owner || Player[1] == part->owner) &&
        (part->owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 && static_cast<u8>(part->owner->apiobj.field_0x27c) <= 1) {
        explosion->field_0x24 |= 0x10000;
        explosion->object = part->owner;
    }
}

void BobaRocket_Move(PART_s *, float) {
    STUBBED();
}

void DagobahA_Update(WORLDINFO_s *) {
    STUBBED();
}

void HothBattleA_Draw(WORLDINFO_s *world) {
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattleA_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothBattleB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "snow_ball_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "snow_ball_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "snow_ball_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "snow_ball_4", 1);
}

void HothBattleC_Draw(WORLDINFO_s *world) {
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattleC_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothBattleE_Draw(WORLDINFO_s *world) {
    if (NuIOS_IsLowEndDevice()) {
        return;
    }
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattleE_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeA_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeB_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeC_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeD_Init(WORLDINFO_s *) {
    STUBBED();
}

void HothBattleA_Reset(WORLDINFO_s *world) {
    GIZMO *gizmo = LevGizmo[0];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if (minikitCounter_A == 10 && (pickup->state_flags & 8) == 0) {
        GizmoActivate(world->gizmo_sys, gizmo, 1, 1);
        return;
    }
    GizmoSetVisibility(world->gizmo_sys, gizmo, 0, 1);
}

void HothBattleC_Reset(WORLDINFO_s *world) {
    GIZMO *bomb_generator = GizmoFindByName(world->gizmo_sys, bombgen_gizmotype_id, "bomb_generator1");
    if (bomb_generator != NULL && bomb_generator->object != NULL) {
        HothBattleC_BombGenerator = static_cast<GIZBOMBGEN *>(bomb_generator->object);
    }

    GIZMO *gizmo = LevGizmo[1];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if (minikitCounter_C == 10 && (pickup->state_flags & 8) == 0) {
        GizmoActivate(world->gizmo_sys, gizmo, 1, 1);
        return;
    }
    GizmoSetVisibility(world->gizmo_sys, gizmo, 0, 1);
}

void HothBattleE_Panel(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeA_Reset(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeB_Reset(WORLDINFO_s *world) {
    locator = AIPathFindLocator(world->ai_sys, "snow_mob");
    gameobj = GetNamedGameObject(world->ai_sys, "snowmob_1");
    TerSurface[9].movement_scale = TerSurface[17].movement_scale;
    TerSurface[9].flags = TerSurface[17].flags & ~2u;
}

void HothEscapeC_Reset(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeD_Reset(WORLDINFO_s *) {
    STUBBED();
}

void BobaRocket_Deflect(PART_s *part) {
    part->flags = (part->flags & ~0x4000u) | 0x80;
    NewPartRotation(part);
}

void HothBattleA_Update(WORLDINFO_s *world) {
    UpdateMiniSnowTroopers(world);
}

void HothBattleC_Update(WORLDINFO_s *world) {
    if (netclient == 0 && HothBattleC_BombGenerator != NULL && !HothBattleC_BombGenerator->active &&
        LevAIMessage[0] != NULL && LevAIMessage[0]->value > 0.0f) {
        HothBattleC_BombGenerator->active = 1;
    }
    UpdateMiniSnowTroopers(world);
}

void HothBattleE_Update(WORLDINFO_s *) {
    STUBBED();
}

void HothEscapeA_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void HothEscapeB_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
    if (netclient == 0 && locator != NULL && gameobj != NULL) {
        locator->position = *NUMTX_GET_ROW_VEC(&gameobj->joint_matrices[1], 3);
    }
}

void HothEscapeC_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void HothEscapeD_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void CloudCityTrapA_Init(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityTrapB_Init(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityTrapA_Reset(WORLDINFO_s *) {
    if (netclient == 0)
        troopercannons_beenReset = 0;
}

void CloudCityTrapC_Panel(WORLDINFO_s *) {
    if (netclient == 0 && LevGameObject[0] != NULL && LevAIMessage[0] != NULL) {
        if (LevAIMessage[0]->value == 1.0f) {
            DrawBossHitPoints(LevGameObject[0]);
        } else {
            DrawBossHitPoints(NULL);
        }
    }
}

void CloudCityTrapC_Reset(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityEscapeA_Init(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityEscapeC_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "gas_1_animin", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "gas_2_animin", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "gas_3_animin", 1);
}

void CloudCityTrapA_Update(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityTrapB_Update(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityTrapC_Update(WORLDINFO_s *) {
    STUBBED();
}

void HothBattle_Melee_init(HOTHBATTLE_MELEE_s *melee) {
    if (melee != NULL) {
        melee->waves[0].field_0x0 = 0;
        melee->field_0x0 = 0;
        melee->field_0x1 = 0;
        melee->field_0x2 = 1;
        melee->field_0x4 = -1;
    }
}

void CloudCityEscapeA_Panel(WORLDINFO_s *) {
    if (netclient == 0) {
        GameObject_s *boba = FindGameObject(id_BOBAFETT, 1, 1, 1, 0);
        if (boba != NULL && LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f) {
            DrawBossHitPoints(boba);
        } else if (LevAIMessage[0] != NULL && LevAIMessage[0]->value == 0.0f) {
            DrawBossHitPoints(NULL);
        }
    }
}

void CloudCityEscapeA_Reset(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "BobaFightStarted", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "Built_C3PO", NULL);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizbuildit_gizmotype_id, "buildit2");
}

void HothBattleE_UpdateWave() {
    STUBBED();
}

void CloudCityEscapeA_Update(WORLDINFO_s *) {
    STUBBED();
}

void CloudCityEscapeC_Update(WORLDINFO_s *) {
    TerSurface[14].flags = 0x2002;
    for (i32 index = 0; index < 3; ++index) {
        if (NuSpecialExistsFn(&LevHSpecial[index])) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[index]);
            if (animation != NULL && animation->ltime == 1.0f) {
                PlaySfx("env_steam_lp", NuSpecialGetDrawPos(&LevHSpecial[index]));
                TerSurface[14].flags |= 0x4042;
            }
        }
    }
}

void HothBattle_StartNewWave() {
    STUBBED();
}

void HothEscapeC_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, HOTHESCAPED_LDATA, 7.5f, NULL);
}

i32 isHothBattleWaveCreature(GameObject_s *object) {
    for (i32 wave = 0; wave < 4; ++wave) {
        for (i32 creature = 0; creature < 4; ++creature) {
            if (melee.waves[wave].creatures[creature] == object)
                return 1;
        }
    }
    return 0;
}

void HothBattle_ManageBackgroundCreatures() {
    STUBBED();
}

// ===========================================================================
// Asteroid chase (AsteroidChase_A / B / C / D)
// ===========================================================================

struct ASTEROID_s {
    nuhspecial_s special;
    GIZMOBLOWUP_s *blowup;
    i16 rotation_speed_x;
    i16 rotation_speed_y;
    i16 rotation_speed_z;
    u8 activated;
    u8 reserved_17;
};
DECOMP_ASSERT(sizeof(ASTEROID_s) == 0x18, "ASTEROID_s size");

i32 nasteroids;
ASTEROID_s asteroids[128];

static void Asteroid_AddParts(GIZMOBLOWUP_s *blowup) {
    i32 special_indices[4] = {0, -1, -1, -1};
    const i32 part_count = qrand() / 0x4000 + 1;
    for (i32 index = 1; index < part_count; ++index) {
        special_indices[index] = qrand() / (0xffff / 3 + 1) + 1;
    }

    for (i32 index = 0; index < part_count; ++index) {
        nuhspecial_s *special = &LevHSpecial[special_indices[index]];
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }

        NUANGVEC rotation = {qrand(), qrand(), qrand()};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotateXYZVU0(&matrix, &rotation);
        NuMtxTranslate(&matrix, &blowup->position);

        NUVEC velocity = {0.0f, 0.0f, static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 3.0f + 2.0f};
        NuVecRotateY(&velocity, &velocity, qrand());

        ADDPART_ALIGNED16 params = Default_ADDPART;
        params.matrix = &matrix;
        params.velocity = &velocity;
        NUVEC centre;
        NuSpecialGetRadius(special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.gravity = 0.0f;
        params.special = special;
        params.flags = index == 0 ? 0x800019b : 0x8000193;
        params.field_40 = PartCollide_3D;
        params.field_44 = Asteroid_PartKill;
        params.time_step = FRAMETIME;
        params.field_a4 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 3.0f + 7.0f;

        PART_s *part = AddPart(&params);
        if (part != NULL) {
            part->force_player_mask = index == 0 ? 1 : 2;
        }
    }
}

static __used__ void Asteroids_Update() {
    ASTEROID_s *asteroid = asteroids;
    for (i32 index = 0; index < nasteroids; ++index, ++asteroid) {
        GIZMOBLOWUP_s *blowup = asteroid->blowup;
        if (blowup != NULL) {
            if ((static_cast<u16>(blowup->status_flags) & 0x4001) == 0x4000) {
                asteroid->activated = 0;
                blowup->field_0xf0 += static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_x) * FRAMETIME);
                blowup->field_0xf2 += static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_y) * FRAMETIME);
                blowup->state_flags |= 1;
                blowup->field_0xf4 += static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_z) * FRAMETIME);
                GizmoBlowupUpdateMatrix(blowup);
                continue;
            }
        } else if (NuSpecialGetVisibilityFn(&asteroid->special) != 0) {
            asteroid->activated = 0;
            NUMTX *matrix = NuSpecialGetDrawMtx(&asteroid->special);
            if (matrix != NULL) {
                NuMtxPreRotateX(matrix, static_cast<i32>(static_cast<f32>(asteroid->rotation_speed_x) * FRAMETIME));
                NuMtxPreRotateY(matrix, static_cast<i32>(static_cast<f32>(asteroid->rotation_speed_y) * FRAMETIME));
                NuSpecialUpdate(&asteroid->special);
                continue;
            }
        }

        if (asteroid->activated == 0) {
            asteroid->activated = 1;
            if (blowup != NULL) {
                blowup->field_0xa8 = 0;
                Asteroid_AddParts(blowup);
            }
        }
    }
}

static void Asteroids_Reset(WORLDINFO_s *world) {
    static const i32 maxrotspd[3] = {0x1555, 0x38e, 0x16c};
    nuhspecial_s specials[128];

    memset(asteroids, 0, sizeof(asteroids));
    nasteroids = 0;

    i32 special_count = NuSpecialFindMulti(world->current_gscn, specials, "asteroid", 128, 0);
    if (special_count == 0)
        return;

    if (special_count > 0) {
        for (i32 special_index = 0; special_index < special_count; ++special_index) {
            for (i32 type_index = 0; type_index < world->gizmo_blowup_type_count; ++type_index) {
                NuSpecialCompare(&world->gizmo_blowup_types[type_index].special, &specials[special_index]);
            }

            if (NuSpecialGetVisibilityFn(&specials[special_index]) != 0) {
                ASTEROID_s *asteroid = &asteroids[nasteroids];
                asteroid->special = specials[special_index];

                char *name = NuSpecialGetName(&asteroid->special);
                i32 asteroid_type;
                if (name == NULL || NuStrIStr(name, "asteroid_a") != NULL || NuStrIStr(name, "asteroid_pop") != NULL) {
                    asteroid_type = 0;
                } else if (NuStrIStr(name, "asteroid_b") != NULL) {
                    asteroid_type = 1;
                } else if (NuStrIStr(name, "asteroid_c") != NULL) {
                    asteroid_type = 2;
                } else {
                    asteroid_type = 0;
                }

                i32 max_speed = maxrotspd[asteroid_type];
                asteroid->rotation_speed_x = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
                asteroid->rotation_speed_y = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
                asteroid->rotation_speed_z = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
                ++nasteroids;
            }
        }
    }

    for (i32 blowup_index = 0; blowup_index < world->gizmo_blowup_count; ++blowup_index) {
        ASTEROID_s *asteroid = &asteroids[nasteroids];
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[blowup_index];
        char *name = blowup->name;
        i32 asteroid_type;
        if (name == NULL || NuStrIStr(name, "asteroid_a") != NULL || NuStrIStr(name, "asteroid_pop") != NULL) {
            asteroid_type = 0;
        } else if (NuStrIStr(name, "asteroid_mid") != NULL) {
            asteroid_type = 1;
        } else {
            continue;
        }

        asteroid->blowup = blowup;
        i32 max_speed = maxrotspd[asteroid_type];
        asteroid->rotation_speed_x = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        asteroid->rotation_speed_y = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        asteroid->rotation_speed_z = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        ++nasteroids;
    }
}

void AsteroidChaseA_Init(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseB_Init(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseB_Draw(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseC_Init(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseD_Init(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseA_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseB_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseC_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseD_Panel(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseA_Update(WORLDINFO_s *) {
    Asteroids_Update();
}

void AsteroidChaseB_Update(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseC_Update(WORLDINFO_s *) {
    STUBBED();
}

void AsteroidChaseD_Update(WORLDINFO_s *) {
    STUBBED();
}
