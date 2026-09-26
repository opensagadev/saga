#include <string.h>
#include <stdio.h>
#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/motion.h"
#include "legoapi/render/fx.h"

extern i32 LevFlag[4];

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}
#include "legoapi/render/core/render.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
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

struct CRUISERDNETPACKET_s {
    f32 frame;
    f32 speed;
    u32 flags;
};
DECOMP_ASSERT(sizeof(CRUISERDNETPACKET_s) == 12, "Cruiser D packet size");

extern "C" {
    CRUISERDNETPACKET_s *cruiserd_netpacket;
    extern i32 CruiserD_LiftChase;
}
static i32 CruiserE_ix;
static nuhspecial_s CruiserD_Lift;
static nuinstanim_s *CruiserD_LiftAnim;
static i32 CruiserD_Lift_plat_id;
static GIZAIMESSAGE_s *CruiserD_LiftChase_msg;
static f32 CruiserD_frame = 1.0f;
static i32 CruiserD_direction = 1;

// Episode 3 level handlers, in the game's Episode_III progression:
// dogfight / cruiser / grievous / kashyyyk / temple / vader / a-new-hope.

// ===========================================================================
// Dogfight (Dogfight_A)
// ===========================================================================

void SpaceResetAudioPoint();
void ProcessCurrentSpeed(WORLDINFO_s *, speedup_s *);
extern AREADATA *DOGFIGHT_ADATA;
void DogFightARestart();
void ResetSpaceLevel(WORLDINFO_s *, spacelevel_s *) __asm__("_ZL15ResetSpaceLevelP11WORLDINFO_sP12spacelevel_s")
    __attribute__((visibility("hidden")));
void DrawSpaceLevel(spacelevel_s *) __asm__("_ZL14DrawSpaceLevelP12spacelevel_s") __attribute__((visibility("hidden")));

speedup_s DogFightSpeedList[] = {
    {58.0f, 0.5f},  {72.0f, 1.0f},  {174.0f, 0.5f}, {183.0f, 1.0f}, {207.0f, 0.5f},
    {220.0f, 1.0f}, {313.0f, 0.5f}, {335.0f, 1.0f}, {0.0f, 0.0f},
};

void ChrisDogFightAInit(WORLDINFO_s *world) {
    ChrisAllocLevelStuff(world);
    ResetSpaceLevel(world, world->space_level);

    for (i32 i = 0; i < 256; ++i) {
        *reinterpret_cast<i32 *>(&world->space_level->large_records[i].unknown_000[0x400]) = 0;
    }

    if (world->current_level == DOGFIGHTA_LDATA) {
        FlightSpline_Init(world, reinterpret_cast<flightspline_s *>(world->space_level->large_records), 256);
    }

    spacelevel_s *current_space = WORLD->space_level;
    spacelevel_large_record_s *record = current_space->large_records;
    spacelevel_large_record_s *end = &current_space->large_records[256];
    for (; record != end; ++record) {
        record->saved_value = record->reset_value;
        record->reset_state = record->saved_state;
    }

    LevBlowUp[0] = GizmoBlowUp_FindByName(world, "Shoot_a11");
    LevBlowUp[1] = GizmoBlowUp_FindByName(world, "Shoot_a1");
    LevBlowUp[2] = GizmoBlowUp_FindByName(world, "Shoot_a21");
    LevBlowUp[3] = GizmoBlowUp_FindByName(world, "Shoot_b1");
    GIZMOBLOWUP_s *last = GizmoBlowUp_FindByName(world, "Shoot_a31");
    LevBlowUp[4] = last;

    LevBlowUp[0]->target_scale *= 1.5f;
    LevBlowUp[1]->target_scale *= 1.5f;
    LevBlowUp[2]->target_scale *= 1.5f;
    LevBlowUp[3]->target_scale *= 1.5f;
    last->target_scale *= 1.5f;
}

void ChrisDogFightAReset(WORLDINFO_s *world) {
    SpaceResetAudioPoint();
    ResetSpaceLevel(world, world->space_level);
    DogFightARestart();
    BOLT_OVERRIDE_PLAYERBOLTSPEED = 150.0f;
    BOLT_OVERRIDE_PLAYERBOLTDURATION = 1.5f;
    music_man.StopTrack(2, 0);
    music_man.StopTrack(0x20, 0);
}

void ChrisDogFightAUpdate(WORLDINFO_s *world) {
    SpaceResetAudioPoint();
    ProcessCurrentSpeed(world, DogFightSpeedList);

    if (AreaGlobals.values.field_0x00 == 0 && *((u8 *)LevFlag) == 0 && DOGFIGHT_ADATA != NULL &&
        Game.area_save[DOGFIGHT_ADATA->index].area_complete == 0 && GamePlayTimer.time_elapsed >= 3.0f) {
        *((u8 *)LevFlag) = 1;
    }
}

void ChrisDogFightADraw(WORLDINFO_s *world) {
    DrawSpaceLevel(world->space_level);
}

void ChrisDogFightAPanel(WORLDINFO_s *) {
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

void CruiserDInit(WORLDINFO_s *world) {
    if (world->area->level_count != 0) {
        for (i32 i = 0; i < world->area->level_count; i++) {
            if (static_cast<u16>(world->area->levels[i]) == static_cast<u16>(CRUISERE_LDATA->idx))
                CruiserE_ix = i;
        }
    }

    if (NuSpecialFind(WORLD->current_gscn, &CruiserD_Lift, "lift", 1)) {
        CruiserD_LiftAnim = NuSpecialGetInstAnim(&CruiserD_Lift);
        CruiserD_Lift_plat_id = FindPlatInst(NuSpecialGetInstanceix(&CruiserD_Lift));
    }

    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizpanel_gizmotype_id, "panel1");
    cruiserd_netpacket = static_cast<CRUISERDNETPACKET_s *>(SetLevelHack(12));

    char name[16] __attribute__((aligned(16)));
#define FIND_CRUISER_D_TUBE(NUMBER)                                                                                    \
    sprintf(name, "Tube%d", NUMBER);                                                                                   \
    if (TUBE *tube = Tube_FindByName(world, name))                                                                     \
    tube->flags |= TUBE_FLAG_TOUCH_RADIUS
    FIND_CRUISER_D_TUBE(1);
    FIND_CRUISER_D_TUBE(2);
    FIND_CRUISER_D_TUBE(3);
    FIND_CRUISER_D_TUBE(4);
    FIND_CRUISER_D_TUBE(5);
    FIND_CRUISER_D_TUBE(6);
    FIND_CRUISER_D_TUBE(7);
    FIND_CRUISER_D_TUBE(8);
    FIND_CRUISER_D_TUBE(9);
    FIND_CRUISER_D_TUBE(10);
#undef FIND_CRUISER_D_TUBE
}

void CruiserDReset(WORLDINFO_s *) {
    CruiserD_LiftChase_msg = CheckGizAIMessage(gizaimessagesys, "LiftChase", NULL);
    MiscTime = 0.0f;

    if (NuSpecialExistsFn(&CruiserD_Lift) != 0 && CruiserD_LiftAnim != NULL) {
        if ((*(u8 *)((u8 *)LevelProgressData + CruiserE_ix * 0x2e24 + 0x2800) & 1) == 0) {
            CruiserD_frame = 1.0f;
            CruiserD_direction = 1;
            CruiserD_LiftAnim->playing = 1;
            CruiserD_LiftAnim->ltime = 1.0f;
            CruiserD_LiftAnim->tfactor = 0.1f;
        } else if (CruiserD_direction < 0) {
            CruiserD_LiftAnim->ltime = CruiserD_frame;
            CruiserD_LiftAnim->playing = 1;
            CruiserD_LiftAnim->tfactor = -0.1f;
        } else {
            f32 end_frame = *(f32 *)CruiserD_Lift.scene->instance_animation_data[CruiserD_LiftAnim->anim_ix];
            CruiserD_LiftAnim->playing = 0;
            CruiserD_LiftAnim->tfactor = 0.1f;
            CruiserD_LiftAnim->ltime = end_frame;
        }
    }

    CruiserD_LiftChase = 0;
}

void CruiserDUpdate(WORLDINFO_s *) {
    CruiserD_LiftChase = 0;
    if (!NuSpecialExistsFn(&CruiserD_Lift) || CruiserD_LiftAnim == NULL || CruiserD_LiftChase_msg == NULL)
        return;

    if (__builtin_expect(netclient != 0, 1)) {
        if (CruiserD_LiftAnim->playing)
            PlaySfx("Cru_HugeWallMoveLp", NuSpecialGetDrawPos(&CruiserD_Lift));
        CruiserD_frame = CruiserD_LiftAnim->ltime;
        CruiserD_LiftAnim->ltime = cruiserd_netpacket->frame;
        CruiserD_LiftAnim->tfactor = cruiserd_netpacket->speed;
        CruiserD_LiftAnim->playing = (cruiserd_netpacket->flags & 1) != 0;
        return;
    }

    CruiserD_LiftChase_msg->value = 0.0f;
    if (CruiserD_direction >= 0 && LevGizmo[0] != NULL && LevGizmo[0]->object != NULL &&
        (((u8 *)LevGizmo[0]->object)[0x68] & 2) != 0) {
        CruiserD_direction = -1;
        CruiserD_LiftAnim->playing = 1;
        CruiserD_LiftAnim->tfactor = -0.1f;
    }

    if (!CruiserD_LiftAnim->playing) {
        CruiserD_frame = CruiserD_LiftAnim->ltime;
        return;
    }

    if (CruiserD_Lift_plat_id != -1) {
        if (CruiserD_direction >= 0) {
            CruiserD_LiftChase_msg->value = 1.0f;
            CruiserD_LiftChase = 1;
        }
        NUVEC *lift_pos = NuSpecialGetDrawPos(&CruiserD_Lift);
#define CRUISERD_CHECK_PLAYER(index)                                                                                   \
    {                                                                                                                  \
        GameObject_s *victim = Player[index];                                                                          \
        if (victim != NULL && victim->apiobj.field_0x287 == 0 &&                                                       \
            (victim->apiobj.supporting_platform_id == CruiserD_Lift_plat_id || victim->apiobj.pos_z > lift_pos->z)) {  \
            ObjHitObj(NULL, victim, -1, 0, 0, 1);                                                                      \
            KillGameObject(victim, 2, 0);                                                                              \
            if (CruiserD_direction >= 0 && MiscTime == 0.0f)                                                           \
                MiscTime = 1.0f;                                                                                       \
        }                                                                                                              \
    }
        CRUISERD_CHECK_PLAYER(0);
        CRUISERD_CHECK_PLAYER(1);
        CRUISERD_CHECK_PLAYER(2);
        CRUISERD_CHECK_PLAYER(3);
        CRUISERD_CHECK_PLAYER(4);
        CRUISERD_CHECK_PLAYER(5);
        CRUISERD_CHECK_PLAYER(6);
        CRUISERD_CHECK_PLAYER(7);
#undef CRUISERD_CHECK_PLAYER
        if (MiscTime > 0.0f) {
            MiscTime -= FRAMETIME;
            if (MiscTime <= 0.0f) {
                MiscTime = 0.0f;
                if (ChallengeMode != 3)
                    ResetLevel(NULL, NULL, 1);
            }
        }
    }

    if (CruiserD_LiftAnim->playing)
        PlaySfx("Cru_HugeWallMoveLp", NuSpecialGetDrawPos(&CruiserD_Lift));

    CruiserD_frame = CruiserD_LiftAnim->ltime;
}

// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================

void GrievousA_Init(WORLDINFO_s *world) {
    GIZMOBLOWUP_s *b;
    if ((b = GizmoBlowUp_FindByName(world, "blast_2_blowup1")) != NULL) {
        nuvec_s pos = {5.42f, 2.76f, 1.79f};
        NuSpecialSetDrawPos(&b->type->animated_special, &pos);
        UpdateMidPos(b);
    }
    if ((b = GizmoBlowUp_FindByName(world, "blast_1_blowup1")) != NULL)
        b->field_0x124 = 1;
    if ((b = GizmoBlowUp_FindByName(world, "blast_1_blowup2")) != NULL)
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
}

void KashyyykC_Init(WORLDINFO_s *world) {
    GIZFORCE_s *f = GizForces_FindForce(world, "force16");
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
}

void KashyyykB_Update(WORLDINFO_s *) {
}

void KashyyykC_Update(WORLDINFO_s *) {
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
    if ((b = GizmoBlowUp_FindByName(world, "builditt_blow1")) != NULL)
        b->field_0xa0 |= 2;
    if ((b = GizmoBlowUp_FindByName(world, "deton_build1")) != NULL)
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

struct VADERANETPACKET_s {
    u16 count;
    i16 subtitle;
    f32 timer;
};

static NUVEC vadar_cam_pos = {0.0f, 0.88f, -12.4f};
static NUVEC vadar_cam_tgt = {0.0f, 2.0f, -17.4f};

void GameCameraMakeMiniCut2(NUVEC *, NUVEC *, i32, f32, f32, f32, f32, i32, i32, i32);
void TickTockSfx();
int LoseCoins(GameObject_s *, i32);

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
    if (vader_a.timer_message != NULL)
        vader_a.timer_message->value += FRAMETIME;

    if (netclient == 0) {
        if (vader_a.collapse_started != 0) {
            vader_a.collapse_started = 0;
            if (Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[0], 1);
            if (Player[1] != NULL && (Player[1]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[1], 1);
            if (Player[2] != NULL && (Player[2]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[2], 1);
            if (Player[3] != NULL && (Player[3]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[3], 1);
            if (Player[4] != NULL && (Player[4]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[4], 1);
            if (Player[5] != NULL && (Player[5]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[5], 1);
            if (Player[6] != NULL && (Player[6]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[6], 1);
            if (Player[7] != NULL && (Player[7]->apiobj.field_0x1f8 & 1) != 0)
                SetObjOnSurface(Player[7], 1);
        }

        if (vader_a.big_jump_locator != NULL && player != NULL && player->apiobj.supporting_platform_id != -1)
            vader_a.big_jump_locator->position = player->apiobj.lower_position;
    }

    if (vader_a.count != 0) {
        f32 previous_time = vader_a.timer;
        vader_a.timer -= FRAMETIME;

        if (static_cast<i16>(vader_a.count) <= 2) {
            if (vader_a.forces[0] != NULL && GizForce_Complete(vader_a.forces[0])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[0] = NULL;
            }
            if (vader_a.forces[1] != NULL && GizForce_Complete(vader_a.forces[1])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[1] = NULL;
            }
            if (vader_a.forces[2] != NULL && GizForce_Complete(vader_a.forces[2])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[2] = NULL;
            }
            if (vader_a.forces[3] != NULL && GizForce_Complete(vader_a.forces[3])) {
                vader_a.timer += 20.0f;
                if (TouchHacks::TouchControlsActive)
                    vader_a.timer += 10.0f;
                vader_a.subtitle = 1;
                vader_a.forces[3] = NULL;
            }

            static const f32 time_limits[3] = {30.0f, 15.0f, 0.0f};
            if (time_limits[static_cast<i16>(vader_a.count)] > vader_a.timer) {
                if (vader_a.count == 1) {
                    SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 2.0f, vader_a.ceiling_collapse_message);
                } else if (vader_a.count == 2) {
                    SetGizAIMessage(gizaimessagesys, "ceiling_collapse", 3.0f, vader_a.ceiling_collapse_message);
                    vader_a.timer = 2.0f;
                    NuCameraGetMtx();
                    GameCameraMakeMiniCut2(&vadar_cam_pos, &vadar_cam_tgt, 0, 0.0f, 2.0f, 0.0f, 0.5f, 0, 0, 1);
                }
                ++vader_a.count;
            }

            if (vader_a.subtitle != 0 ||
                (vader_a.timer > 0.0f && static_cast<i32>(previous_time) != static_cast<i32>(vader_a.timer)))
                TickTockSfx();
        } else if (netclient == 0 && vader_a.timer <= 0.0f) {
            if (Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0)
                LoseCoins(Player[0], 1);
            if (Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.field_0x1f8) < 0)
                LoseCoins(Player[1], 1);
            KillGameObject(player, 2, 0);
        }
    }

    if (vader_a.reset_flag == 0 &&
        ((Player[0] != NULL && Player[0]->apiobj.field_0x287 != 0 && (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0) ||
         (Player[1] != NULL && Player[1]->apiobj.field_0x287 != 0 && (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0))) {
        if (GameCam->sock_position.location.sock != 0 || player->apiobj.field_0x287 != 0) {
            vader_a.reset_flag = 1;
            ResetLevel(NULL, NULL, 1);
        }
    }

    VADERANETPACKET_s *packet = static_cast<VADERANETPACKET_s *>(vadera_netpacket);
    if (netclient == 0) {
        packet->count = vader_a.count;
        packet->subtitle = vader_a.subtitle;
        packet->timer = vader_a.timer;
    } else {
        vader_a.count = packet->count;
        vader_a.subtitle = packet->subtitle;
        vader_a.timer = packet->timer;
    }
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

void VaderC_Update(WORLDINFO_s *world) {
    extern TERRSET *CurTerr;
    extern i32 obstacle_gizmotype_id;

    if (netclient == 0 && vader_c.final_fight_message != NULL && ChallengeMode == 0 &&
        vader_c.final_fight_message->value == 0.0f && vader_c.big_jump_locator != NULL && player != NULL &&
        player->apiobj.supporting_platform_id != -1) {
        vader_c.big_jump_locator->position = player->apiobj.lower_position;

        u8 progress = vader_c.field_0x94;
        for (i32 i = 0; i < 10; ++i) {
            i16 platform_id = vader_c.platform_ids[i];
            if (platform_id == -1)
                continue;

            if (Player[0] != NULL && Player[0]->apiobj.field_0x27d != 0 &&
                Player[0]->apiobj.supporting_platform_id == platform_id &&
                Player[0]->apiobj.position.y >=
                    static_cast<NUMTX *>(CurTerr->platforms[platform_id].scene_object)->m31) {
                progress |= 1;
                vader_c.field_0x94 = progress;
            }
            if (Player[1] != NULL && Player[1]->apiobj.field_0x27d != 0 &&
                Player[1]->apiobj.supporting_platform_id == platform_id &&
                Player[1]->apiobj.position.y >=
                    static_cast<NUMTX *>(CurTerr->platforms[platform_id].scene_object)->m31) {
                progress |= 2;
                vader_c.field_0x94 = progress;
            }
        }

        if (progress == 3) {
            vader_c.final_fight_message->value = 1.0f;
            DOOR_s *door = Door_FindByName(world, "door_fight");
            if (door != NULL)
                Door_GoThrough(world, door, 1);
        }
    }

    if (netclient != 0 && GameTimer.time_elapsed < 5.0f) {
        GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle20");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle21");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle22");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
        gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, "obstacle23");
        if (gizmo != NULL && gizmo->object != NULL)
            static_cast<GIZOBSTACLE_s *>(gizmo->object)->progress_flags &= ~1;
    }

    if (vader_c.field_0x95 == 0) {
        bool dead0 = Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 0x80) != 0 &&
                     Player[0]->apiobj.field_0x287 != 0 && (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0;
        bool dead1 = Player[1] != NULL && (Player[1]->apiobj.field_0x1f8 & 0x80) != 0 &&
                     Player[1]->apiobj.field_0x287 != 0 && (Player[1]->apiobj.field_0x1f4 & 0x40000) == 0;
        bool both_controlled = Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 0x80) != 0 && Player[1] != NULL &&
                               (Player[1]->apiobj.field_0x1f8 & 0x80) != 0;
        if ((dead0 || dead1) && (static_cast<u8 *>(vaderc_netpacket)[0] == 0 || !both_controlled) &&
            (ChallengeMode == 0 || AreaGlobals.values.field_0x1c <= 9)) {
            vader_c.field_0x95 = 1;
            if (vader_c.final_fight_message->value == 0.0f)
                ResetLevel(NULL, NULL, 1);
        }
    }

    if (LevGizObst[0] != NULL && LevGizObst[0]->anim_set != NULL && LevGizObst[0]->anim_set->objects != NULL &&
        LevGizObst[0]->anim_set->objects->instance_animation != NULL) {
        GAMEANIMOBJ_s *object = LevGizObst[0]->anim_set->objects;
        nuinstanim_s *anim = object->instance_animation;
        if (object->end_frame > 200.0f) {
            f32 factor = anim->ltime;
            if (factor >= 200.0f) {
                factor = (object->end_frame - factor) / (object->end_frame - 200.0f);
                factor *= factor;
            }
            if (anim->fparam1 == 0.0f)
                anim->tfactor = 1.0f;
            else
                anim->tfactor = factor * anim->fparam1;
        }
    }
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
