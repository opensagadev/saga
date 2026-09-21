#include <string.h>
#include <stdio.h>
#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"

extern i32 LevFlag[4];

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nutex.h"

i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
extern i16 id_PALPATINE;
extern i16 id_WOOKIEE;

static GIZAIMESSAGE_s *KashyyykA_msg_TotalWookies;
static NUVEC TempleC_StreamStatusPos = {52.57f, 0.76f, -16.54f};
static GIZAIMESSAGE_s *KashyyykA_msg_WookiesToRescue;
static GameObject_s *Grievous_obj; // current Grievous boss object
static f32 BoulderWait;
static i32 i_boulder;
PART_s *boulder_part[2];
i32 boulder_blowup_type = -1;

void Boulder_Move(PART_s *, f32);
void Boulder_Kill(PART_s *, i32);
struct VADERA_s {
    GIZAIMESSAGE_s *in_control_room_message;
    GIZAIMESSAGE_s *ceiling_collapse_message;
    GIZAIMESSAGE_s *timer_message;
    f32 timer;
    u16 count;
    i16 subtitle;
    GIZFORCE_s *forces[4];
    AILOCATOR_s *big_jump_locator;
    i32 collapse_started;
    u8 reset_flag;
    u8 reserved_2d[0x03];
};
DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");

static VADERA_s vader_a;
static GIZAIMESSAGE_s *vader_b_complete_msg; // Vader B "complete" message handle
static u8 vader_b_playersDead;               // Vader B player-death flag

static void VaderA_StartCollapseStage(WORLDINFO_s *world);

struct CRUISERCNETPACKET_s {
    i16 free_palpatine;
    i16 dooku_fight;
};

struct CRUISERC_s {
    GIZAIMESSAGE_s *dooku_fight;
    GameObject_s *count_dooku;
    GIZAIMESSAGE_s *free_palpatine;
    GameObject_s *palpatine;
    AILOCATOR_s *palpatine_locator;
};
DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");

extern "C" {
    CRUISERCNETPACKET_s *crusiserc_netpacket = NULL;
}
static CRUISERC_s cruiser_c;

// Episode 3 level handlers, in the game's Episode_III progression:
// dogfight / cruiser / grievous / kashyyyk / temple / vader / a-new-hope.

// ===========================================================================
// Dogfight (Dogfight_A)
// ===========================================================================

void SpaceResetAudioPoint();
void ProcessCurrentSpeed(WORLDINFO_s *, speedup_s *);
extern AREADATA *DOGFIGHT_ADATA;

speedup_s DogFightSpeedList[] = {
    {58.0f, 0.5f},  {72.0f, 1.0f},  {174.0f, 0.5f}, {183.0f, 1.0f}, {207.0f, 0.5f},
    {220.0f, 1.0f}, {313.0f, 0.5f}, {335.0f, 1.0f}, {0.0f, 0.0f},
};

void ChrisDogFightAInit(WORLDINFO_s *) {
    STUBBED();
}

void ChrisDogFightAReset(WORLDINFO_s *) {
    STUBBED();
}

void ChrisDogFightAUpdate(WORLDINFO_s *world) {
    SpaceResetAudioPoint();
    ProcessCurrentSpeed(world, DogFightSpeedList);

    if (AreaGlobals.values.field_0x00 == 0 && *((u8 *)LevFlag) == 0 && DOGFIGHT_ADATA != NULL &&
        Game.area_save[DOGFIGHT_ADATA->index].area_complete == 0 && GamePlayTimer.time_elapsed >= 3.0f) {
        *((u8 *)LevFlag) = 1;
    }
}

void ChrisDogFightADraw(WORLDINFO_s *) {
    STUBBED();
}

void ChrisDogFightAPanel(WORLDINFO_s *) {
    STUBBED();
}

// ===========================================================================
// Cruiser (Cruiser_A / Cruiser_C / Cruiser_D)
// ===========================================================================

void CruiserAInit(WORLDINFO_s *world) {
    *((u8 *)LevFlag) = 0;
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "starfighter1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "starfighter2", 1);
    if (FreePlay)
        *((u8 *)LevFlag) = 2;
}

void CruiserAUpdate(WORLDINFO_s *) {
    if (*((u8 *)LevFlag) == 1) {
        void *sp;

        *((u8 *)LevFlag) = 2;
        sp = (void *)LevHSpecial;
        if (NuSpecialExistsFn(sp) != 0)
            NuSpecialSetVisibility(sp, 1);
        sp = (char *)sp + 0xc;
        if (NuSpecialExistsFn(sp) != 0)
            NuSpecialSetVisibility(sp, 1);
    }
}

void CruiserCReset(WORLDINFO_s *) {
    crusiserc_netpacket = static_cast<CRUISERCNETPACKET_s *>(SetLevelHack(4));
    cruiser_c.count_dooku = NULL;
    cruiser_c.dooku_fight = NULL;
    cruiser_c.free_palpatine = NULL;
    cruiser_c.palpatine = NULL;
    cruiser_c.palpatine_locator = NULL;
}

void CruiserCUpdate(WORLDINFO_s *) {
    if (cruiser_c.free_palpatine == NULL)
        cruiser_c.free_palpatine = CheckGizAIMessage(gizaimessagesys, "FreePalpatine", NULL);
    if (cruiser_c.dooku_fight == NULL)
        cruiser_c.dooku_fight = CheckGizAIMessage(gizaimessagesys, "DookuFight", NULL);
    if (netclient != 0) {
        cruiser_c.free_palpatine->value = crusiserc_netpacket->free_palpatine;
        cruiser_c.dooku_fight->value = crusiserc_netpacket->dooku_fight;
    }
    if (cruiser_c.palpatine == NULL)
        cruiser_c.palpatine = (GameObject_s *)FindGameObject((i32)(i16)id_PALPATINE, 0x400, 1, 1, 0);
    if (cruiser_c.palpatine_locator == NULL)
        cruiser_c.palpatine_locator = AIPathFindLocator(WORLD->ai_sys, "Palpatine");
    if (cruiser_c.count_dooku == NULL)
        cruiser_c.count_dooku = (GameObject_s *)FindGameObject((i32)(i16)id_COUNTDOOKU, 1, 1, 1, 0);
    if (FreePlay == 0 && cruiser_c.palpatine != NULL && cruiser_c.palpatine_locator != NULL &&
        cruiser_c.free_palpatine != NULL && cruiser_c.free_palpatine->value < 2.0f) {
        cruiser_c.palpatine->apiobj.position = cruiser_c.palpatine_locator->position;
        cruiser_c.palpatine->apiobj.field_0x276 = cruiser_c.palpatine_locator->direction;
    }
}

void CruiserCPanel(WORLDINFO_s *) {
    if (netclient == 0) {
        if (cruiser_c.count_dooku != NULL && cruiser_c.dooku_fight != NULL && cruiser_c.dooku_fight->value == 1.0f)
            DrawBossHitPoints(cruiser_c.count_dooku);
        else
            DrawBossHitPoints(NULL);
    }
}

void CruiserDInit(WORLDINFO_s *) {
    STUBBED();
}

void CruiserDReset(WORLDINFO_s *) {
    STUBBED();
}

void CruiserDUpdate(WORLDINFO_s *) {
    STUBBED();
}

// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================

void GrievousA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "grievous_1")) != NULL) {
        nuvec_s pos = {5.42f, 2.76f, 1.79f};
        NuSpecialSetDrawPos(&b->type->animated_special, &pos);
        UpdateMidPos(b);
    }
    if ((b = GizmoBlowUp_FindByName(world, "grievous_2")) != NULL)
        b->field_0x124 = 1;
    if ((b = GizmoBlowUp_FindByName(world, "grievous_3")) != NULL)
        b->field_0x124 = 1;
}

void GrievousA_Reset(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    Grievous_obj = (GameObject_s *)FindGameObject((i32)(i16)id_GRIEVOUS, 1, 1, 0, 0);
    if (Grievous_obj != NULL)
        DrawBossHitPoints(Grievous_obj);
}

void GrievousA_Update(WORLDINFO_s *world) {
    if (netclient != 0)
        return;

    if (Grievous_obj == NULL)
        return;

    if (Grievous_obj->current_hp > 0)
        return;

    if (FreePlay == 0)
        KillBossPlayCutScene((i32)(i16)id_GRIEVOUS, 0, 0.0f, "ep3_GeneralGrievous_Outro");
    else
        KillBossCompleteLevel((i32)(i16)id_GRIEVOUS, 0, 0.0f);
}

// ===========================================================================
// Kashyyyk (Kashyyyk_A / Kashyyyk_B / Kashyyyk_C / Kashyyyk_D)
// ===========================================================================

void KashyyykA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "bridge_1_switc1")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_1_switc2")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_2_switc1")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
    if ((b = GizmoBlowUp_FindByName(world, "bridge_2_switc2")) != NULL) {
        b->field_0x128 = 0.3f;
        b->field_0x124 = 1;
        b->field_0xa0 &= ~2;
    }
}

void KashyyykB_Init(WORLDINFO_s *) {
    STUBBED();
}

void KashyyykC_Init(WORLDINFO_s *world) {
    GIZFORCE_s *f = GizForces_FindForce(world, "kashyyyk_boss");
    if (f != NULL) {
        if (f->force_strength == 3.0f)
            f->force_strength = 20.0f;
        f->strength_0x6c = 1.0f;
    }
}

void KashyyykD_Init(WORLDINFO_s *) {
    i_boulder = 0;
    BoulderWait = 0.0f;
    boulder_blowup_type = GizmoBlowupGetTypeFromNameTableId(WORLD, GizmoBlowupGetNameTableId("ball_blowup_null"));
}

void KashyyykA_Panel(WORLDINFO_s *) {
    if (Mission_Active(MissionSys) != NULL)
        return;
    i16 characters[3] = {id_WOOKIEE, id_WOOKIEE, id_WOOKIEE};
    char rescued[3] = {1, 1, 1};
    if (KashyyykA_msg_TotalWookies != NULL && KashyyykA_msg_TotalWookies->value > 0.0f &&
        KashyyykA_msg_WookiesToRescue != NULL && KashyyykA_msg_WookiesToRescue->value > 0.0f) {
        i32 remaining = (i32)KashyyykA_msg_WookiesToRescue->value;
        if (remaining > 3)
            remaining = 3;
        for (i32 i = 0; i < remaining; ++i)
            rescued[i] = 0;
        i32 total = (i32)KashyyykA_msg_TotalWookies->value;
        if (total > 3)
            total = 3;
        DrawMeleeTargets(characters, rescued, NULL, total);
    }
}

void KashyyykA_Reset(WORLDINFO_s *) {
    KashyyykA_msg_TotalWookies = CheckGizAIMessage(gizaimessagesys, "TotalWookies", NULL);
    KashyyykA_msg_WookiesToRescue = CheckGizAIMessage(gizaimessagesys, "WookiesToRescue", NULL);
}

void KashyyykB_Reset(WORLDINFO_s *) {
    STUBBED();
}

void KashyyykD_Reset(WORLDINFO_s *world) {
    char name[0x100];
    for (i32 i = 0; i < 2; ++i) {
        sprintf(name, "ball%ib", i + 1);
        if (NuSpecialFind(world->current_gscn, &LevHSpecial[i], name, 1))
            NuSpecialSetVisibility(&LevHSpecial[i], 0);
        boulder_part[i] = NULL;
        sprintf(name, "ball%i", i + 1);
        if (NuSpecialFind(world->current_gscn, &LevHSpecial[i], name, 1)) {
            NuSpecialSetVisibility(&LevHSpecial[i], 0);
            LevInstAnim[i] = NuSpecialGetInstAnim(&LevHSpecial[i]);
            LevInstAnim[i]->playing = 0;
            LevInstAnim[i]->ltime = 0.0f;
        }
    }
}

i32 AnakinGreenSabre(GameObject_s *obj) {
    i32 result = 0;
    if (FreePlay == 0 && obj->id == id_ANAKINPADAWAN && WORLD->area != NULL) {
        if (WORLD->area == JEDI_ADATA || WORLD->area == DOOKU_ADATA)
            result = 1;
    }
    return result;
}

void KashyyykA_Update(WORLDINFO_s *) {
    STUBBED();
}

void KashyyykB_Update(WORLDINFO_s *) {
    STUBBED();
}

void KashyyykC_Update(WORLDINFO_s *) {
    STUBBED();
}

void KashyyykD_Update(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    if (BoulderWait <= 0.0f) {
        if (boulder_part[i_boulder] != NULL)
            return;
        if (LevInstAnim[i_boulder] != NULL && LevHSpecial[i_boulder].display_special != NULL) {
            ADDPART_s params = Default_ADDPART;
            params.matrix = &numtx_identity;
            params.velocity = &v000;
            params.owner = NULL;
            params.field_14 = 0.3f;
            params.field_18 = 0.3f;
            params.gravity = 0.0f;
            params.special = &LevHSpecial[i_boulder];
            params.flags = 0x80861a;
            params.move_fn = Boulder_Move;
            params.field_40 = PartCollide_3D;
            params.field_44 = Boulder_Kill;
            params.time_step = FRAMETIME;
            PART_s *part = AddPart(&params);
            if (part != NULL)
                boulder_part[i_boulder] = part;
            LevInstAnim[i_boulder]->ltime = 1.0f;
            LevInstAnim[i_boulder]->playing = 1;
            LevInstAnim[i_boulder]->mtx = numtx_identity;
            NuSpecialSetVisibility(&LevHSpecial[i_boulder], 1);
        }
        ++i_boulder;
        if (i_boulder == 2)
            i_boulder = 0;
        BoulderWait = 8.0f;
    } else {
        BoulderWait -= FRAMETIME;
    }
}

// ===========================================================================
// Temple (Temple_A / Temple_C)
// ===========================================================================

void TempleA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "temple_statue")) != NULL)
        b->field_0xa0 |= 2;
    if ((b = GizmoBlowUp_FindByName(world, "temple_pillar")) != NULL)
        b->field_0xa0 |= 2;
}

void TempleC_Init(WORLDINFO_s *world) {
    char *force_names[10] = {"force8", "force10", "force5",  "force4",  "force3",
                             "force2", "force1",  "force23", "force21", "force19"};
    for (i32 i = 0; i < 10; ++i) {
        GIZFORCE_s *force = GizForces_FindForce(world, force_names[i]);
        if (force != NULL && (force->config_flags & 0x400) != 0) {
            for (GAMEANIMOBJ_s *object = force->anim_set->objects; object != NULL; object = object->next) {
                if ((object->flags & 1) == 0)
                    object->current_frame = 0.5f;
            }
        }
    }
    GIZMOBLOWUP_s *blowup;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box11")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box21")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "thermo_box31")) != NULL)
        blowup->field_0xa0 |= 2;
    if ((blowup = GizmoBlowUp_FindByName(world, "Thermo1")) != NULL)
        blowup->field_0xa0 |= 2;
}

void TempleC_AlwaysUpdate(WORLDINFO_s *) {
    NUVEC position;
    if (TEMPLESTATUS_LDATA != NULL && Players_AveragePos(&position, NULL) &&
        NuVecXZDistSqr(&position, &TempleC_StreamStatusPos, NULL) < 100.0f)
        other_level_override = TEMPLESTATUS_LDATA->idx;
}

// ===========================================================================
// Vader (Vader_A / Vader_B / Vader_C)
// ===========================================================================

void *vadera_netpacket;

void VaderA_Init(WORLDINFO_s *world) {
    memset(&vader_a, 0, sizeof(vader_a));
    vadera_netpacket = SetLevelHack(8);
    vader_a.big_jump_locator = AIPathFindLocator(world->ai_sys, "Bigjump_0");
    vader_a.in_control_room_message = SetGizAIMessage(gizaimessagesys, "InControlRoom", 0.0f, NULL);
    vader_a.ceiling_collapse_message = SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 0.0f, NULL);
    vader_a.timer_message = SetGizAIMessage(gizaimessagesys, "timer", 0.0f, NULL);
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "LozTheTosser1");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
}

void VaderB_Init(WORLDINFO_s *) {
    STUBBED();
}

i32 Vader_ObiWanKilledAnakin;
void *vaderc_netpacket;

void VaderC_Init(WORLDINFO_s *world) {
    char *names[10] = {"rock1", "rock2", "rock3", "rock4", "rock5", "rock6", "rock7", "rock8", "rock10", "rock11"};
    memset(&vader_c, 0, sizeof(vader_c));
    vader_c.final_fight_message = CheckGizAIMessage(gizaimessagesys, "FinalFight", NULL);
    vader_c.big_jump_locator = AIPathFindLocator(world->ai_sys, "Bigjump_0");
    for (i32 i = 0; i < 10; ++i) {
        if (NuSpecialFind(world->current_gscn, &vader_c.rocks[i], names[i], 1))
            vader_c.platform_ids[i] = FindPlatInst(NuSpecialGetInstanceix(&vader_c.rocks[i]));
        else
            vader_c.platform_ids[i] = -1;
    }
    Vader_ObiWanKilledAnakin = 0;
    vaderc_netpacket = SetLevelHack(1);
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle19");
}

void VaderA_Reset(WORLDINFO_s *world) {
    DrawTimer(0, 0, 1);

    if (netclient == 0 && vader_a.count != 0) {
        VaderA_StartCollapseStage(world);
    }

    if (vader_a.timer_message != NULL) {
        vader_a.timer_message->value = 0.0f;
    }

    vader_a.collapse_started = 1;
    vader_a.reset_flag = 0;
}

void VaderB_Reset(WORLDINFO_s *) {
    vader_b_complete_msg = SetGizAIMessage(gizaimessagesys, "VaderBComplete", 0.0f, NULL);
    vader_b_playersDead = 0;
}

void VaderC_Reset(WORLDINFO_s *) {
    vader_c.field_0x94 = 0;
    vader_c.field_0x95 = 0;
}

void VaderA_Update(WORLDINFO_s *) {
    STUBBED();
}

void VaderB_Update(WORLDINFO_s *) {
    if (netclient == 0 && vader_b_complete_msg != NULL && vader_b_complete_msg->value == 1.0f && VADERB_LDATA != NULL)
        GoToNewLevel(VADERC_LDATA->idx);
    if (vader_b_playersDead == 0 &&
        ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0 && (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0) ||
         (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0 && (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0))) {
        vader_b_playersDead = 1;
        ResetLevel(NULL, NULL, 1);
    }
}

void VaderC_Update(WORLDINFO_s *) {
    STUBBED();
}

void VaderA_DrawPanel(WORLDINFO_s *) {
    if (vader_a.count <= 2) {
        if (vader_a.count != 0) {
            DrawTimer((i32)vader_a.timer + 1, vader_a.subtitle, 0);
            vader_a.subtitle = 0;
        }
    }
}

void VaderB_DrawPanel(WORLDINFO_s *) {
    STUBBED();
}

void VaderC_DrawPanel(WORLDINFO_s *) {
    if (netclient == 0 && vader_c.final_fight_message != NULL && vader_c.final_fight_message->value == 1.0f) {
        if (player2 != NULL) {
            DrawBossHitPoints(NULL);
        } else {
            GameObject_s *opponent = Player[0];
            if (opponent == player)
                opponent = Player[1];
            if (opponent != NULL)
                DrawBossHitPoints(opponent);
        }
    }
}

void VaderA_GoneThroughDoor(WORLDINFO_s *world, DOOR_s *door) {
    if (netclient == 0 && vader_a.count == 0)
        VaderA_StartCollapseStage(world);
    if (door != NULL && NuStrICmp(door->name, "door_control") == 0)
        door->active = 1;
}

static __used__ void VaderA_StartCollapseStage(WORLDINFO_s *world) {
    i32 path_index;
    nuhspecial_s specials[100];
    u32 *path_connection = static_cast<u32 *>(
        AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "Block1_a", "Block1_b", &path_index));

    if (path_connection != NULL) {
        path_connection[path_index] |= 0x80000000;
    }

    vader_a.forces[0] = GizForces_FindForce(world, "force4");
    vader_a.forces[1] = GizForces_FindForce(world, "InControlRoom");
    vader_a.forces[2] = GizForces_FindForce(world, "ceiling_collapse");
    vader_a.forces[3] = GizForces_FindForce(world, "wobble");
    vader_a.count = 1;
    vader_a.timer = 30.0f;
    vader_a.in_control_room_message =
        SetGizAIMessage(gizaimessagesys, "InControlRoom", 1.0f, vader_a.in_control_room_message);
    vader_a.ceiling_collapse_message =
        SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 1.0f, vader_a.ceiling_collapse_message);

    i32 special_count = NuSpecialFindMulti(world->current_gscn, specials, "wobble", 100, 0);
    for (i32 i = 0; i < special_count; ++i) {
        NuSpecialSetVisibility(&specials[i], 0);
    }
}

// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================

void ANewHopeA_Init(WORLDINFO_s *world) {
    GIZOBSTACLE_s *g;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle6")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle7")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle8")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle9")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle10")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle11")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle12")) != NULL)
        g->field_a1_0xa1 |= 1;
    if ((g = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle13")) != NULL)
        g->field_a1_0xa1 |= 1;
}
