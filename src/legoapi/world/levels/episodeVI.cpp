#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/world/level.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/render/core/rtl.h"
#include "nu2api/numath/nutrig.h"
i32 Player_HasInvincibility(GameObject_s *);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
extern "C" i32 GetSfxId(const char *);
extern i32 testlaser_type;
extern f32 testlaser_sizew, testlaser_sizel, testlaser_sizewab, testlaser_endw;
#include "nu2api/nucore/nustring.h"
#include "gameapi/ai/aisys/aisys.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
extern u8 LevFlag[16];
struct SarlaccBattlePacket {
    f32 height;
    i16 off_mask;
    i16 flash_mask;
    i16 select_mask;
    i16 on_mask;
    i16 finish_mask;
    i16 sound_mask;
    u8 disco_active;
    u8 reserved;
    i16 completion_sound_played;
};
DECOMP_ASSERT(sizeof(SarlaccBattlePacket) == 20, "Sarlacc battle packet size");
DECOMP_ASSERT(offsetof(SarlaccBattlePacket, disco_active) == 0x10, "Sarlacc disco active offset");
DECOMP_ASSERT(offsetof(SarlaccBattlePacket, completion_sound_played) == 0x12, "Sarlacc sound latch offset");
SarlaccBattlePacket *sarlaccb_netpacket;
struct SarlaccDisco {
    AIAREA_s *area;
    nuhspecial_s off[16];
    nuhspecial_s flash[16];
    nuhspecial_s select[16];
    nuhspecial_s on[16];
    nuhspecial_s finish[16];
    GIZOBSTACLE_s *off_obstacle;
    GIZOBSTACLE_s *on_obstacle;
    u8 panel_state[16];
    i8 count;
    i8 phase;
    i8 first_panel;
    i8 second_panel;
    f32 timer;
    f32 initial_height;
    f32 height;
    f32 field_3ec;
    GIZAIMESSAGE_s *help_message;
    GIZAIMESSAGE_s *complete_message;
    GIZAIMESSAGE_s *state_message;
    NUVEC *last_selected_position;
};
DECOMP_ASSERT(sizeof(SarlaccDisco) == 0x400, "Sarlacc disco state size");
DECOMP_ASSERT(offsetof(SarlaccDisco, off) == 4, "Sarlacc off array offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, flash) == 0xc4, "Sarlacc flash array offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, select) == 0x184, "Sarlacc select array offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, on) == 0x244, "Sarlacc on array offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, finish) == 0x304, "Sarlacc finish array offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, off_obstacle) == 0x3c4, "Sarlacc obstacle offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, panel_state) == 0x3cc, "Sarlacc panel states offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, count) == 0x3dc, "Sarlacc count offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, timer) == 0x3e0, "Sarlacc timer offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, height) == 0x3e8, "Sarlacc height offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, complete_message) == 0x3f4, "Sarlacc completion message offset");
DECOMP_ASSERT(offsetof(SarlaccDisco, last_selected_position) == 0x3fc, "Sarlacc last selected position offset");
static SarlaccDisco sarlaccdisco;
f32 disco_base_offset = -0.12f;
f32 sarlaccdiscotime = 40.0f;
f32 discoheightseek = 2.0f;
f32 discoheight = 1.27f;
void PlayRadio(char *, char *, i32);
i32 GizBuildIt_AtEnd(GIZBUILDIT_s *);
void SarlaccPitB_SpecialUpdate(WORLDINFO_s *);
GIZMO *obstMirrorBall;
GIZMO *forceMirrorBall;
nuhspecial_s LevSpecial[7];
void *LevelBuildits[2];
extern i32 obstacle_gizmotype_id, force_gizmotype_id;
extern "C" i16 id_RANCOR;
i32 GizBuildIt_AtEnd(GIZBUILDIT_s *);
void SarlaccPitB_SpecialUpdate(WORLDINFO_s *);
static __used__ i32 power;
static __used__ i32 recharging;
static __used__ i32 target_shield[2];
static __used__ volatile i32 taken_over;
static __used__ i32 gizmoblowuptargetcount;
static __used__ GIZMOBLOWUP_s *gizmoblowuptarget[4];
static __used__ f32 rechargetimer;

// Episode 6 level handlers, in the game's Episode_VI progression:
// jabbas palace / sarlacc pit / speeder chase / endor battle / death star 2
// battle / emperor fight, plus the senate bonus.

// ===========================================================================
// Jabba's Palace (JabbasPalace_A / B / D / E)
// ===========================================================================

void JabbasPalaceA_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "grill_02", 1);
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "entrance11");
    if (blowup != NULL)
        blowup->field_0xa0 |= 2;
    blowup = GizmoBlowUp_FindByName(world, "entrance21");
    if (blowup != NULL)
        blowup->field_0xa0 |= 2;
}

void JabbasPalaceB_Init(WORLDINFO_s *world) {
    LevGizObst[0] = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle5");
    LevBlowUp[0] = GizmoBlowUp_FindByName(world, "prison_stone1");
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "prison_blast1");
    if (blowup != NULL)
        blowup->field_0xa0 |= 2;
}

void JabbasPalaceE_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "prox_explo1");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "prox_explo2");
    LevFlag[6] = 0;
    LevFlag[7] = 0;
}

void JabbasPalaceA_Reset(WORLDINFO_s *) {
    TerSurface[14].flags = 0xa040;
    TerSurface[9].flags = 0x2040;
}

void JabbasPalaceB_Reset(WORLDINFO_s *) {
    TerSurface[14].flags = 0xa040;
    TerSurface[9].flags = 0x2040;
}

void JabbasPalaceD_Reset(WORLDINFO_s *world) {
    TerSurface[14].flags = 0xa040;
    TerSurface[9].flags = 0x2040;
    if (world->current_level == JABBASPALACED_LDATA && world->push_block_count > 0) {
        pushblock_s *push_block = world->push_blocks;
        NUDISPLAYSPECIAL_s *display_special = push_block->special.display_special;
        if (display_special != NULL) {
            NUGSCN *scene = push_block->special.scene;
            if (scene != NULL && scene->display_list != NULL)
                scene->display_list->visibility_flags[display_special->instance_ix] |= 8;
        }
    }
}

void JabbasPalaceE_Reset(WORLDINFO_s *world) {
    LevGameObject[0] = GetNamedGameObject(world->ai_sys, "AI_rancor");
    LevArea[0] = AISysFindArea(world->ai_sys, "Alcove_1");
    LevArea[1] = AISysFindArea(world->ai_sys, "Alcove_2");
    LevArea[2] = AISysFindArea(world->ai_sys, "Back_Room");
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    TerSurface[14].flags = 0xa040;
    TerSurface[9].flags = 0x2040;
}

void JabbasPalaceE_Panel(WORLDINFO_s *) {
    if (!netclient) {
        if (LevGameObject[0] != NULL && LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f)
            DrawBossHitPoints(LevGameObject[0]);
        else
            DrawBossHitPoints(NULL);
    }
}

void JabbasPalaceA_Update(WORLDINFO_s *) {
    NUVEC position = {-0.01503f, 0.7414f, 11.5824f};
    if (NuSpecialExistsFn(&LevHSpecial[0]) && !NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        AIANTINODE *antinode = AIAntinodeCreateSingleFrame(&position, 1.0f);
        if (antinode != NULL) {
            antinode->type = 2;
            antinode->base_radius = 0.82443f;
            antinode->base_height = 1.0f;
        }
    }
}

void JabbasPalaceE_Update(WORLDINFO_s *world) {
    if (!netclient) {
        if (FreePlay)
            KillBossCompleteLevel(id_RANCOR, 0, 0.3f);
        else
            KillBossNewLevel(id_RANCOR, 0, 0.3f, JABBASPALACE_OUTRO_LDATA->idx);
    }
    GameObject_s *rancor = LevGameObject[0];
    if (rancor != NULL) {
        rancor->ai_opponent_exclusion_mask = 0;
        if (aicreature_sets_alive[0] != 0) {
            for (i32 index = 0; index < 8; ++index) {
                GameObject_s *player = Player[index];
                if (player != NULL)
                    rancor->ai_opponent_exclusion_mask |= 1ULL << player->apiobj.field_0x289;
            }
        } else {
            // Retail widens a signed 32-bit area mask, including its sign bit.
            for (i32 index = 0; index < 8; ++index) {
                GameObject_s *player = Player[index];
                if (player != NULL && ((LevArea[0] != NULL &&
                                        (player->apiobj.ai_area_mask &
                                         static_cast<i32>(1u << ((LevArea[0] - world->ai_sys->areas) & 31))) != 0) ||
                                       (LevArea[1] != NULL &&
                                        (player->apiobj.ai_area_mask &
                                         static_cast<i32>(1u << ((LevArea[1] - world->ai_sys->areas) & 31))) != 0) ||
                                       (LevArea[2] != NULL &&
                                        (player->apiobj.ai_area_mask &
                                         static_cast<i32>(1u << ((LevArea[2] - world->ai_sys->areas) & 31))) != 0))) {
                    rancor->ai_opponent_exclusion_mask |= 1ULL << player->apiobj.field_0x289;
                }
            }
        }
    }
    GIZMOBLOWUP_s *blowup = LevGizmo[0] != NULL ? static_cast<GIZMOBLOWUP_s *>(LevGizmo[0]->object) : NULL;
    if (blowup != NULL && (blowup->status_flags & 1) != 0) {
        if (LevFlag[6] == 0) {
            PlaySfx("exp_minecart", &blowup->position);
            LevFlag[6] = 1;
        }
    } else {
        LevFlag[6] = 0;
    }
    blowup = LevGizmo[1] != NULL ? static_cast<GIZMOBLOWUP_s *>(LevGizmo[1]->object) : NULL;
    if (blowup != NULL && (blowup->status_flags & 1) != 0) {
        if (LevFlag[7] == 0) {
            PlaySfx("exp_minecart", &blowup->position);
            LevFlag[7] = 1;
        }
    } else {
        LevFlag[7] = 0;
    }
}

// ===========================================================================
// Sarlacc Pit (SarlaccPit_A / B / C)
// ===========================================================================

void SarlaccPitA_Draw(WORLDINFO_s *) {
    GIZAIMESSAGE_s *message = NULL;
    if (gizaimessagesys != NULL)
        message = CheckGizAIMessage(gizaimessagesys, "Boba_Dead", NULL);
    if (message == NULL || message->value != 0.0f) {
        DrawBossHitPoints(NULL);
    } else {
        GameObject_s *boba = FindGameObject(id_BOBAFETT, 1, 1, 1, 0);
        if (gizaimessagesys != NULL)
            message = CheckGizAIMessage(gizaimessagesys, "BobaFightStarted", NULL);
        if (message != NULL && boba != NULL && message->value == 1.0f)
            DrawBossHitPoints(boba);
    }
}

void SarlaccPitA_Reset(WORLDINFO_s *world) {
    LevSafePlatID[0] = -1;
    if (NuSpecialFind(world->current_gscn, &LevHSpecial[0], "float_skiff_2", 1) == 0) {
        return;
    }
    if (world->terrain == NULL) {
        return;
    }
    LevSafePlatID[0] = FindPlatInst(NuSpecialGetInstanceix(&LevHSpecial[0]));
}

void SarlaccPitB_Init(WORLDINFO_s *) {
    memset(&sarlaccdisco, 0, sizeof(sarlaccdisco));
    sarlaccb_netpacket = static_cast<SarlaccBattlePacket *>(SetLevelHack(sizeof(SarlaccBattlePacket)));
    i8 *disco_index = &sarlaccdisco.count;
    *disco_index = 0;
    char name[32];
    for (;;) {
        if (*disco_index <= 8)
            sprintf(name, "dot_off_0%d", *disco_index + 1);
        else
            sprintf(name, "dot_off_%d", *disco_index + 1);
        NuSpecialFind(WORLD->current_gscn, &sarlaccdisco.off[*disco_index], name, 1);

        if (*disco_index <= 8)
            sprintf(name, "dot_flash_0%d", *disco_index + 1);
        else
            sprintf(name, "dot_flash_%d", *disco_index + 1);
        NuSpecialFind(WORLD->current_gscn, &sarlaccdisco.flash[*disco_index], name, 1);

        if (*disco_index <= 8)
            sprintf(name, "dot_select_0%d", *disco_index + 1);
        else
            sprintf(name, "dot_select_%d", *disco_index + 1);
        NuSpecialFind(WORLD->current_gscn, &sarlaccdisco.select[*disco_index], name, 1);

        if (*disco_index <= 8)
            sprintf(name, "dot_on_0%d", *disco_index + 1);
        else
            sprintf(name, "dot_on_%d", *disco_index + 1);
        NuSpecialFind(WORLD->current_gscn, &sarlaccdisco.on[*disco_index], name, 1);

        if (*disco_index <= 8)
            sprintf(name, "dot_finish_0%d", *disco_index + 1);
        else
            sprintf(name, "dot_finish_%d", *disco_index + 1);
        NuSpecialFind(WORLD->current_gscn, &sarlaccdisco.finish[*disco_index], name, 1);

        if (!NuSpecialExistsFn(&sarlaccdisco.off[*disco_index]) ||
            !NuSpecialExistsFn(&sarlaccdisco.flash[*disco_index]) ||
            !NuSpecialExistsFn(&sarlaccdisco.select[*disco_index]) ||
            !NuSpecialExistsFn(&sarlaccdisco.on[*disco_index]) ||
            !NuSpecialExistsFn(&sarlaccdisco.finish[*disco_index]))
            break;

        if (*disco_index == 0) {
            NUVEC *position = NuSpecialGetPos(&sarlaccdisco.off[0]);
            if (position != NULL) {
                f32 height = position->y;
                sarlaccdisco.initial_height = height;
                sarlaccdisco.height = height;
            }
        }
        ++*disco_index;
        if (*disco_index > 15)
            break;
    }
    sarlaccdisco.area = AISysFindArea(WORLD->ai_sys, "DISCO");
    NuSpecialFind(WORLD->current_gscn, &LevHSpecial[0], "force_engine_lump", 1);
    NuSpecialFind(WORLD->current_gscn, &LevHSpecial[1], "disco_base", 1);
}

void SarlaccPitB_Reset(WORLDINFO_s *world) {
    memset(sarlaccdisco.panel_state, 0, sizeof(sarlaccdisco.panel_state));
    sarlaccdisco.phase = 0;
    sarlaccdisco.timer = 0.0f;
    sarlaccdisco.first_panel = -1;
    sarlaccdisco.second_panel = -1;

    for (i32 index = 0; index < sarlaccdisco.count; ++index) {
        NuSpecialSetVisibility(&sarlaccdisco.off[index], 1);
        NuSpecialSetVisibility(&sarlaccdisco.flash[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.on[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.finish[index], 0);
    }

    sarlaccdisco.help_message = SetGizAIMessage(gizaimessagesys, "HelpWithDisco", 0.0f, NULL);
    sarlaccdisco.complete_message = SetGizAIMessage(gizaimessagesys, "DiscoComplete", 0.0f, NULL);
    sarlaccdisco.state_message = SetGizAIMessage(gizaimessagesys, "DiscoState", 0.0f, NULL);
    sarlaccdisco.off_obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "disco_off");
    sarlaccdisco.on_obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "disco_on");

    NuSpecialFind(world->current_gscn, &LevSpecial[0], "floor_disco", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[1], "light1_a", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[2], "disco_ball1", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[3], "disco_ball2", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[4], "shutter_1", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[5], "jabba_door1", 1);
    NuSpecialFind(world->current_gscn, &LevSpecial[6], "jabba_door2", 1);
    forceMirrorBall = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force5");
    obstMirrorBall = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle5");
    LevelBuildits[0] = GizBuildIt_Find(world, "buildit6");
    LevelBuildits[1] = GizBuildIt_Find(world, "buildit4");
    LevelLocator = AIPathFindLocator(world->ai_sys, "DiscoHelp");

    if (NuSpecialExistsFn(&LevSpecial[0]))
        NuSpecialSetVisibility(&LevSpecial[0], 0);
    if (NuSpecialExistsFn(&LevSpecial[1]))
        NuSpecialSetVisibility(&LevSpecial[1], 0);
    if (NuSpecialExistsFn(&LevSpecial[2]))
        NuSpecialSetVisibility(&LevSpecial[2], 0);
    if (GizForce_Complete(static_cast<GIZFORCE_s *>(forceMirrorBall->object)))
        GizObstacle_Stop(static_cast<GIZOBSTACLE_s *>(obstMirrorBall->object));
}

void SarlaccPitB_Update(WORLDINFO_s *world) {
    static NUVEC lumppos;
    static f32 enginetimer;
    if (netclient == 0) {
        sarlaccb_netpacket->sound_mask = 0;
        SetGizAIMessage(gizaimessagesys, "HelpWithDisco", 0.0f, sarlaccdisco.help_message);
        SetGizAIMessage(gizaimessagesys, "DiscoComplete", 0.0f, sarlaccdisco.complete_message);
        SetGizAIMessage(gizaimessagesys, "DiscoState", sarlaccdisco.phase, sarlaccdisco.state_message);
        GIZMO *panel = GizmoFindByName(world->gizmo_sys, gizspecial_gizmotype_id, "qaz_panel2");
        GizmoGetOutput(world->gizmo_sys, panel, 0, 0);
        i32 ready = 0;
        if (GizBuildIt_AtEnd(static_cast<GIZBUILDIT_s *>(LevelBuildits[0])) &&
            GizBuildIt_AtEnd(static_cast<GIZBUILDIT_s *>(LevelBuildits[1])) &&
            static_cast<GIZSPECIAL_s *>(panel->object)->anim_set->state == 2) {
            if (sarlaccdisco.complete_message->value == 0.0f)
                SetGizAIMessage(gizaimessagesys, "discoflooropen", 1.0f, NULL);
            ready = 1;
        }

        i32 candidates[16];
        switch (sarlaccdisco.phase) {
            case 0:
                sarlaccb_netpacket->disco_active = 0;
                if (NuSpecialExistsFn(&LevSpecial[0]) && ready &&
                    ((player != NULL && (player->apiobj.ai_area_mask &
                                         static_cast<i32>(1u << ((sarlaccdisco.area - world->ai_sys->areas) & 31)))) ||
                     (player2 != NULL &&
                      (player2->apiobj.ai_area_mask &
                       static_cast<i32>(1u << ((sarlaccdisco.area - world->ai_sys->areas) & 31)))))) {
                    sarlaccdisco.phase = 1;
                    sarlaccdisco.timer = 0.0f;
                    i32 count = 0;
                    for (i32 index = 0; index < sarlaccdisco.count; ++index)
                        if (sarlaccdisco.panel_state[index] == 0)
                            candidates[count++] = index;
                    sarlaccdisco.first_panel = count != 0 ? candidates[NuRand(NULL) % count] : -1;
                    count = 0;
                    for (i32 index = 0; index < sarlaccdisco.count; ++index)
                        if (index != sarlaccdisco.first_panel && sarlaccdisco.panel_state[index] == 0)
                            candidates[count++] = index;
                    sarlaccdisco.second_panel = count != 0 ? candidates[NuRand(NULL) % count] : -1;
                    if (sarlaccdisco.first_panel != -1 && sarlaccdisco.second_panel != -1) {
                        sarlaccdisco.panel_state[sarlaccdisco.first_panel] = 1;
                        sarlaccdisco.panel_state[sarlaccdisco.second_panel] = 1;
                    }
                }
                break;
            case 1: {
                sarlaccb_netpacket->disco_active = 0;
                if (!ready) {
                    SarlaccPitB_Reset(world);
                    break;
                }
                i32 first_occupied = 0;
                i32 controlled_panel = -1;
                u8 previous = sarlaccdisco.panel_state[sarlaccdisco.first_panel];
                sarlaccdisco.panel_state[sarlaccdisco.first_panel] = 1;
                NUVEC *position = NuSpecialGetPos(&sarlaccdisco.flash[sarlaccdisco.first_panel]);
                for (i32 index = 0; index < 8; ++index) {
                    GameObject_s *object = Player[index];
                    if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
                        object->apiobj.field_0x27d != 0) {
                        f32 dx = position->x - object->apiobj.position.x;
                        f32 dz = position->z - object->apiobj.position.z;
                        if (dx * dx + dz * dz < 0.2f * 0.2f) {
                            sarlaccdisco.panel_state[sarlaccdisco.first_panel] = 2;
                            first_occupied = 1;
                            if (object == player)
                                controlled_panel = sarlaccdisco.first_panel;
                            break;
                        }
                    }
                }
                if (previous != sarlaccdisco.panel_state[sarlaccdisco.first_panel] &&
                    sarlaccdisco.panel_state[sarlaccdisco.first_panel] == 2) {
                    sarlaccdisco.last_selected_position =
                        NUMTX_GET_ROW_VEC(NuSpecialGetDrawMtx(&sarlaccdisco.on[sarlaccdisco.first_panel]), 3);
                    sarlaccb_netpacket->sound_mask |= 1 << sarlaccdisco.first_panel;
                }
                i32 second_occupied = 0;
                previous = sarlaccdisco.panel_state[sarlaccdisco.second_panel];
                sarlaccdisco.panel_state[sarlaccdisco.second_panel] = 1;
                position = NuSpecialGetPos(&sarlaccdisco.flash[sarlaccdisco.second_panel]);
                for (i32 index = 0; index < 8; ++index) {
                    GameObject_s *object = Player[index];
                    if (object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
                        object->apiobj.field_0x27d != 0) {
                        f32 dx = position->x - object->apiobj.position.x;
                        f32 dz = position->z - object->apiobj.position.z;
                        if (dx * dx + dz * dz < 0.2f * 0.2f) {
                            sarlaccdisco.panel_state[sarlaccdisco.second_panel] = 2;
                            second_occupied = 1;
                            if (object == player)
                                controlled_panel = sarlaccdisco.second_panel;
                            break;
                        }
                    }
                }
                if (previous != sarlaccdisco.panel_state[sarlaccdisco.second_panel] &&
                    sarlaccdisco.panel_state[sarlaccdisco.second_panel] == 2) {
                    sarlaccdisco.last_selected_position =
                        NUMTX_GET_ROW_VEC(NuSpecialGetDrawMtx(&sarlaccdisco.on[sarlaccdisco.second_panel]), 3);
                    sarlaccb_netpacket->sound_mask |= 1 << sarlaccdisco.second_panel;
                }
                if (first_occupied && second_occupied) {
                    sarlaccdisco.timer = 0.0f;
                    sarlaccdisco.panel_state[sarlaccdisco.first_panel] = 3;
                    sarlaccdisco.panel_state[sarlaccdisco.second_panel] = 3;
                    i32 count = 0;
                    for (i32 index = 0; index < sarlaccdisco.count; ++index)
                        if (sarlaccdisco.panel_state[index] == 0)
                            candidates[count++] = index;
                    sarlaccdisco.first_panel = count != 0 ? candidates[NuRand(NULL) % count] : -1;
                    count = 0;
                    for (i32 index = 0; index < sarlaccdisco.count; ++index)
                        if (index != sarlaccdisco.first_panel && sarlaccdisco.panel_state[index] == 0)
                            candidates[count++] = index;
                    sarlaccdisco.second_panel = count != 0 ? candidates[NuRand(NULL) % count] : -1;
                    if (sarlaccdisco.first_panel == -1 || sarlaccdisco.second_panel == -1) {
                        sarlaccdisco.phase = 2;
                        for (i32 index = 0; index < sarlaccdisco.count; ++index)
                            sarlaccdisco.panel_state[index] = 4;
                    } else {
                        sarlaccdisco.panel_state[sarlaccdisco.first_panel] = 1;
                        sarlaccdisco.panel_state[sarlaccdisco.second_panel] = 1;
                    }
                } else {
                    sarlaccdisco.timer += FRAMETIME;
                    if (sarlaccdisco.timer > 2.5f) {
                        sarlaccdisco.timer = 0.0f;
                        for (i32 pass = 0; pass < 2; ++pass) {
                            i32 count = 0;
                            for (i32 index = 0; index < sarlaccdisco.count; ++index)
                                if (sarlaccdisco.panel_state[index] == 3)
                                    candidates[count++] = index;
                            i32 selected = count != 0 ? candidates[NuRand(NULL) % count] : -1;
                            if (selected != -1)
                                sarlaccdisco.panel_state[selected] = 0;
                        }
                    }
                    if (player2 == NULL && LevelLocator != NULL) {
                        GameObject_s *partner = Player[0];
                        if (partner == player)
                            partner = Player[1];
                        if (partner != NULL) {
                            NUVEC *help_position = NULL;
                            if (controlled_panel == sarlaccdisco.first_panel)
                                help_position = NuSpecialGetPos(&sarlaccdisco.flash[sarlaccdisco.second_panel]);
                            else if (controlled_panel == sarlaccdisco.second_panel)
                                help_position = NuSpecialGetPos(&sarlaccdisco.flash[sarlaccdisco.first_panel]);
                            if (help_position != NULL) {
                                SetGizAIMessage(gizaimessagesys, "HelpWithDisco", 1.0f, sarlaccdisco.help_message);
                                LevelLocator->position.x = help_position->x;
                                LevelLocator->position.z = help_position->z;
                            }
                        }
                    }
                }
                break;
            }
            case 2:
                sarlaccb_netpacket->disco_active = 1;
                sarlaccdisco.timer += FRAMETIME;
                if (sarlaccdisco.timer > sarlaccdiscotime || !ready) {
                    sarlaccb_netpacket->disco_active = 0;
                    SarlaccPitB_Reset(world);
                    return;
                }
                break;
        }
        if (sarlaccdisco.phase != 0) {
            f32 amount = (discoheight - sarlaccdisco.height) / (discoheight - sarlaccdisco.initial_height);
            amount = amount < 0.0f ? 0.0f : amount > 1.0f ? 1.0f : amount;
            f32 rate = 1.0f - (NU_SIN_LUT(amount * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
            sarlaccdisco.height = SeekValF(sarlaccdisco.height, discoheight, rate * discoheightseek);
        }
        sarlaccb_netpacket->off_mask = 0;
        sarlaccb_netpacket->flash_mask = 0;
        sarlaccb_netpacket->select_mask = 0;
        sarlaccb_netpacket->on_mask = 0;
        sarlaccb_netpacket->finish_mask = 0;
        for (i32 index = 0; index < sarlaccdisco.count; ++index) {
            switch (sarlaccdisco.panel_state[index]) {
                case 0:
                    sarlaccb_netpacket->off_mask |= 1 << index;
                    break;
                case 1:
                    sarlaccb_netpacket->flash_mask |= 1 << index;
                    break;
                case 2:
                    sarlaccb_netpacket->select_mask |= 1 << index;
                    break;
                case 3:
                    sarlaccb_netpacket->on_mask |= 1 << index;
                    break;
                case 4:
                    sarlaccb_netpacket->finish_mask |= 1 << index;
                    break;
            }
        }
    }
    if (LevFlag[0] == 0 && NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        lumppos = *NuSpecialGetDrawPos(&LevHSpecial[0]);
        LevFlag[0] = 1;
        enginetimer = 0.05f;
    }
    if (LevFlag[0] != 0) {
        enginetimer -= FRAMETIME;
        if (enginetimer <= 0.0f)
            enginetimer = 0.1f;
        f32 offset = 1.0f - (NU_SIN_LUT(enginetimer * 10.0f * 32768.0f + 16384.0f) + 1.0f) * 0.5f;
        f32 z = lumppos.z + (offset * 0.01f - 0.005f);
        NuSpecialGetDrawPos(&LevHSpecial[0])->z = z;
        PlaySfx("env_curtain_lp", NuSpecialGetDrawPos(&LevHSpecial[0]));
    }
    SarlaccPitB_SpecialUpdate(world);
}

static inline void SarlaccDiscoShow(nuhspecial_s *special) {
    NuSpecialSetVisibility(special, 1);
    NUVEC *position = NuSpecialGetDrawPos(special);
    if (position != NULL) {
        position->y = sarlaccdisco.height;
        NuSpecialSetDrawPos(special, position);
    }
}

void SarlaccPitB_SpecialUpdate(WORLDINFO_s *) {
    for (i32 index = 0; index < sarlaccdisco.count; ++index) {
        NuSpecialSetVisibility(&sarlaccdisco.off[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.flash[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.select[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.on[index], 0);
        NuSpecialSetVisibility(&sarlaccdisco.finish[index], 0);
        if (netclient)
            sarlaccdisco.height = SeekValF(sarlaccdisco.height, sarlaccb_netpacket->height, 8.0f);
        else
            sarlaccb_netpacket->height = sarlaccdisco.height;

        if ((sarlaccb_netpacket->off_mask & (1 << index)) != 0)
            SarlaccDiscoShow(&sarlaccdisco.off[index]);
        else if ((sarlaccb_netpacket->flash_mask & (1 << index)) != 0)
            SarlaccDiscoShow(&sarlaccdisco.flash[index]);
        else if ((sarlaccb_netpacket->select_mask & (1 << index)) != 0)
            SarlaccDiscoShow(&sarlaccdisco.select[index]);
        else if ((sarlaccb_netpacket->on_mask & (1 << index)) != 0)
            SarlaccDiscoShow(&sarlaccdisco.on[index]);
        else if ((sarlaccb_netpacket->finish_mask & (1 << index)) != 0)
            SarlaccDiscoShow(&sarlaccdisco.finish[index]);
        NUVEC *position = NuSpecialGetDrawPos(&LevHSpecial[1]);
        if (position != NULL) {
            position->y = sarlaccdisco.height + disco_base_offset;
            NuSpecialSetDrawPos(&LevHSpecial[1], position);
        }
        if ((sarlaccb_netpacket->sound_mask & (1 << index)) != 0)
            PlaySfx("Kam_DiscoFloorPanelOn", NUMTX_GET_ROW_VEC(NuSpecialGetDrawMtx(&sarlaccdisco.on[index]), 3));
    }
    if (sarlaccb_netpacket->disco_active != 0) {
        SetGizAIMessage(gizaimessagesys, "DiscoComplete", 1.0f, sarlaccdisco.complete_message);
        PlayRadio("Speaker2", "Speaker21", 1);
        PlayRadio("Speaker1", "Speaker11", 1);
        PlayRadio("decks", "decks", 1);
        if (obstMirrorBall != NULL)
            GizObstacle_PlayForwards(static_cast<GIZOBSTACLE_s *>(obstMirrorBall->object));
        if (NuSpecialExistsFn(&LevSpecial[0]))
            NuSpecialSetVisibility(&LevSpecial[0], 1);
        if (NuSpecialExistsFn(&LevSpecial[1]))
            NuSpecialSetVisibility(&LevSpecial[1], 1);
        if (NuSpecialExistsFn(&LevSpecial[4])) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevSpecial[4]);
            if (animation != NULL)
                animation->playing = 1;
        }
        if (NuSpecialExistsFn(&LevSpecial[5]) && NuSpecialExistsFn(&LevSpecial[6])) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevSpecial[5]);
            if (animation != NULL)
                animation->playing = 1;
            animation = NuSpecialGetInstAnim(&LevSpecial[6]);
            if (animation != NULL)
                animation->playing = 1;
        }
        if (sarlaccb_netpacket->completion_sound_played == 0) {
            PlaySfx("Kam_DiscoFloorPanelDone", NuSpecialGetDrawPos(&sarlaccdisco.off[0]));
            sarlaccb_netpacket->completion_sound_played = 1;
        }
    } else {
        PlayRadio("Speaker2", "Speaker21", 0);
        PlayRadio("Speaker1", "Speaker11", 0);
        PlayRadio("decks", "decks", 0);
        sarlaccb_netpacket->completion_sound_played = 0;
    }
}

void SarlaccPitC_Init(WORLDINFO_s *) {
    power = 0;
    recharging = 0;
    target_shield[0] = 0;
    target_shield[1] = 0;
}

void SarlaccPitC_Reset(WORLDINFO_s *world) {
    char name[32];
    taken_over = 0;
    gizmoblowuptargetcount = 0;
    LevGameObject[0] = GetNamedGameObject(world->ai_sys, "cannon_1");

    sprintf(name, "cover_%d", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[16], name, 1);
    sprintf(name, "cover_%d1", 1);
    GIZMO *gizmo = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    if (gizmo != NULL) {
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
        gizmoblowuptarget[gizmoblowuptargetcount] = blowup;
        blowup->field_0x9f |= 8;
        ++gizmoblowuptargetcount;
        UpdateMidPos(blowup);
    }
    sprintf(name, "cover_%d", 2);
    NuSpecialFind(world->current_gscn, &LevHSpecial[17], name, 1);
    sprintf(name, "cover_%d1", 2);
    gizmo = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    if (gizmo != NULL) {
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
        gizmoblowuptarget[gizmoblowuptargetcount] = blowup;
        blowup->field_0x9f |= 8;
        ++gizmoblowuptargetcount;
        UpdateMidPos(blowup);
    }
    sprintf(name, "spinner%d_null_1", 1);
    gizmo = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    if (gizmo != NULL) {
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
        gizmoblowuptarget[gizmoblowuptargetcount] = blowup;
        ++gizmoblowuptargetcount;
        UpdateMidPos(blowup);
    }
    sprintf(name, "spinner%d_null_1", 2);
    gizmo = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    if (gizmo != NULL) {
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
        gizmoblowuptarget[gizmoblowuptargetcount] = blowup;
        ++gizmoblowuptargetcount;
        UpdateMidPos(blowup);
    }

#define FIND_SHOT(index, format, number)                                                                               \
    sprintf(name, format, number);                                                                                     \
    NuSpecialFind(world->current_gscn, &LevHSpecial[index], name, 1)
    FIND_SHOT(0, "shot_0%d", 1);
    FIND_SHOT(1, "shot_0%d", 2);
    FIND_SHOT(2, "shot_0%d", 3);
    FIND_SHOT(3, "shot_0%d", 4);
    FIND_SHOT(4, "shot_0%d", 5);
    FIND_SHOT(5, "shot_0%d", 6);
    FIND_SHOT(6, "shot_0%d", 7);
    FIND_SHOT(7, "shot_0%d", 8);
    FIND_SHOT(8, "shot_0%da", 1);
    FIND_SHOT(9, "shot_0%da", 2);
    FIND_SHOT(10, "shot_0%da", 3);
    FIND_SHOT(11, "shot_0%da", 4);
    FIND_SHOT(12, "shot_0%da", 5);
    FIND_SHOT(13, "shot_0%da", 6);
    FIND_SHOT(14, "shot_0%da", 7);
    FIND_SHOT(15, "shot_0%da", 8);
#undef FIND_SHOT

    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizpanel_gizmotype_id, "panel2");
#define RESET_SHOT(index, level)                                                                                       \
    NuSpecialSetVisibility(&LevHSpecial[index], power <= level);                                                       \
    NuSpecialSetVisibility(&LevHSpecial[(index) + 8], power > level)
    RESET_SHOT(0, 0);
    RESET_SHOT(1, 1);
    RESET_SHOT(2, 2);
    RESET_SHOT(3, 3);
    RESET_SHOT(4, 4);
    RESET_SHOT(5, 5);
    RESET_SHOT(6, 6);
    RESET_SHOT(7, 7);
#undef RESET_SHOT
}

void SarlaccPitC_Update(WORLDINFO_s *world) {
    if (LevGameObject[0] != NULL && gizmoblowuptargetcount == 4) {
        if (LevGameObject[0]->field_0xcc0 != NULL) {
            world->field_50d0 = 4;
            world->blowup_target_candidates = gizmoblowuptarget;
            if (taken_over == 0)
                taken_over = 1;
        } else if (taken_over != 0) {
            taken_over = 0;
            world->field_50d0 = 0;
            world->blowup_target_candidates = NULL;
        }

        if (power == 0) {
            if ((LevGameObject[0]->field_0xef8 & 8) != 0)
                LevGameObject[0]->field_0xef8 &= ~8;
        } else if (static_cast<i8>(LevGameObject[0]->quick_shoot_bolt_id) >= 0) {
            --power;
            NuSpecialSetVisibility(&LevHSpecial[power], 1);
            NuSpecialSetVisibility(&LevHSpecial[power + 8], 0);
            GIZPANEL_s *panel = static_cast<GIZPANEL_s *>(LevGizmo[0]->object);
            if (panel->state)
                panel->state = 0;
        }

        if (recharging == 0) {
            if (static_cast<u32>(power) <= 7 && LevGizmo[0] != NULL &&
                GizmoGetOutput(world->gizmo_sys, LevGizmo[0], 0, 0) != 0) {
                recharging = 1;
                rechargetimer = 0.0f;
            }
        } else {
            rechargetimer -= FRAMETIME;
            if (rechargetimer <= 0.0f) {
                LevGameObject[0]->field_0xef8 |= 8;
                NuSpecialSetVisibility(&LevHSpecial[power], 0);
                NuSpecialSetVisibility(&LevHSpecial[power + 8], 1);
                PlaySfx("imp_c3po_magnet_drop", NuSpecialGetDrawPos(&LevHSpecial[power + 8]));
                ++power;
                if (power == 8) {
                    recharging = 0;
                    PlaySfx("env_magnet_on", NuSpecialGetDrawPos(&LevHSpecial[power + 8]));
                } else {
                    rechargetimer = 0.25f;
                }
            }
        }
    }

    if (target_shield[0] == 0) {
        if (gizmoblowuptarget[0] != NULL && (gizmoblowuptarget[0]->visibility_flags & 0x40) == 0) {
            PlaySfx("ffieldoff", NuSpecialGetDrawPos(&LevHSpecial[16]));
            target_shield[0] = 1;
        } else {
            PlaySfx("ffield", NuSpecialGetDrawPos(&LevHSpecial[16]));
        }
    }
    if (target_shield[1] == 0) {
        if (gizmoblowuptarget[1] != NULL && (gizmoblowuptarget[1]->visibility_flags & 0x40) == 0) {
            PlaySfx("ffieldoff", NuSpecialGetDrawPos(&LevHSpecial[17]));
            target_shield[1] = 1;
        } else {
            PlaySfx("ffield", NuSpecialGetDrawPos(&LevHSpecial[17]));
        }
    }
}

i32 SarlaccPitDiscoActive(WORLDINFO_s *world) {
    return world->current_level == SARLACCPITB_LDATA && sarlaccb_netpacket->disco_active != 0;
}

// ===========================================================================
// Bonus levels: Lego City, Senate, New Town
// ===========================================================================

static u8 prevOnTaunTaun;
static u8 prevOnTractor;
static u8 prevOnMoonCar;
static u8 prevOnTownCar;
static u8 prevOnFireTruck;
static u8 prevOnLifeBoat;

void LegoCity_Init(WORLDINFO_s *world) {
    char name[16];
    i32 i = 1;
    for (;;) {
        sprintf(name, "lamp_%d", i);
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, name);
        if (blowup == NULL)
            break;
        blowup->field_0x128 = 0.3f;
        blowup->field_0x124 = 1;
        ++i;
    }
}

void LegoCity_Reset(WORLDINFO_s *world) {
    prevOnTaunTaun = 0;
    prevOnTractor = 0;
    prevOnMoonCar = 0;
    prevOnTownCar = 0;

    GIZMOPICKUP_s *pickup = world->pickup_sys->pickups;
    if (pickup == NULL) {
        return;
    }
    if (world->pickup_sys->pickup_count <= 0) {
        return;
    }

    for (i32 pickup_index = 0; pickup != NULL && pickup_index < world->pickup_sys->pickup_count;
         ++pickup_index, ++pickup) {
        if ((pickup->runtime_flags & 8) == 0) {
            switch (pickup->type_id) {
                case 3:
                case 4:
                case 5:
                case 6:
                    pickup->collected = 0;
                    break;
                default:
                    break;
            }
        }
    }
}

void LegoCity_Update(WORLDINFO_s *world) {
    if (world == NULL || world->pickup_sys == NULL) {
        return;
    }
    u8 onTractor = 0;
    u8 onTaunTaun = 0;
    u8 onMoonCar = 0;
    u8 onTownCar = 0;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = Player[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1000) != 0 && object->apiobj.field_0x287 == 0 &&
            object->field_0xcc0 != NULL && object->field_0xcc0->character_context == CHARACTER_CONTEXT_LINKED_OBJECT) {
            if (object->id == id_TRACTOR) {
                onTractor = 0xff;
            } else if (object->id == id_TAUNTAUN) {
                onTaunTaun = 0xff;
            } else if (object->id == id_MOONCAR) {
                onMoonCar = 0xff;
            } else if (object->id == id_TOWNCAR) {
                onTownCar = 0xff;
            }
        }
    }
    u8 changed = (onTractor ^ prevOnTractor) | (onTaunTaun ^ prevOnTaunTaun) | (onMoonCar ^ prevOnMoonCar) |
                 (onTownCar ^ prevOnTownCar);
    prevOnTractor = onTractor;
    prevOnTaunTaun = onTaunTaun;
    prevOnMoonCar = onMoonCar;
    prevOnTownCar = onTownCar;
    if (changed == 0) {
        return;
    }
    GIZMOPICKUP_s *pickup = world->pickup_sys->pickups;
    for (i32 index = 0; pickup != NULL && index < world->pickup_sys->pickup_count; ++index, ++pickup) {
        if ((pickup->runtime_flags & 8) == 0) {
            switch (pickup->type_id) {
                case 3:
                    pickup->collected = onTractor;
                    break;
                case 4:
                    pickup->collected = onTaunTaun;
                    break;
                case 5:
                    pickup->collected = onMoonCar;
                    break;
                case 6:
                    pickup->collected = onTownCar;
                    break;
            }
        }
    }
}

void SenateA_Init(WORLDINFO_s *world) {
    char *names[] = {"deton_0110",
                     "deton_0111",
                     "deton_011",
                     "deton_012",
                     "deton_013",
                     "deton_014",
                     "deton_015",
                     "deton_017",
                     "deton_018",
                     "deton_019",
                     "console_btm19",
                     "console_btm110",
                     "console_btm11",
                     "console_btm18",
                     "console_btm13",
                     "console_btm16",
                     "console_btm15",
                     "console_btm14",
                     NULL};
    for (i32 i = 0; names[i] != NULL; ++i) {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, names[i]);
        if (blowup != NULL)
            blowup->draw_flags |= 2;
    }
}

void NewTown_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "dummy_exp8");
    char buf[0x18];
    i32 i = 1;
    for (;;) {
        sprintf(buf, "Pop_%d_House_61", i);
        GIZMOBLOWUP_s *g = GizmoBlowUp_FindByName(world, buf);
        if (g == NULL)
            break;
        g->field_0xa0 |= 2;
        i++;
    }
}

void NewTown_Reset(WORLDINFO_s *world) {
    u32 seed = 17;
    prevOnTaunTaun = 0;
    prevOnFireTruck = 0;
    prevOnLifeBoat = 0;

    GIZMOPICKUP_s *pickup = world->pickup_sys->pickups;
    if (pickup == NULL)
        return;
    if (world->pickup_sys->pickup_count <= 0)
        return;
    for (i32 i = 0; pickup != NULL && i < world->pickup_sys->pickup_count; ++i, ++pickup) {
        if (!(pickup->runtime_flags & 8)) {
            switch (pickup->type_id) {
                case 2:
                case 3:
                case 4:
                    pickup->collected = 0;
                    break;
                default:
                    break;
            }
        }
        if (pickup->type_id == 4) {
            f32 x = NuRandFloatSeeded(&seed);
            f32 z = NuRandFloatSeeded(&seed);
            pickup->position.x = x * 4.0f - 7.656 - 1.5;
            pickup->position.z = z * 4.0f - 5.871 - 1.5;
        }
    }
}

void NewTown_Update(WORLDINFO_s *world) {
    if (world == NULL || world->pickup_sys == NULL) {
        return;
    }
    u8 onTaunTaun = 0;
    u8 onFireTruck = 0;
    u8 onLifeBoat = 0;
    for (i32 index = 0; index < 8; ++index) {
        GameObject_s *object = Player[index];
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1000) != 0 && object->apiobj.field_0x287 == 0 &&
            object->field_0xcc0 != NULL && object->field_0xcc0->character_context == CHARACTER_CONTEXT_LINKED_OBJECT) {
            if (object->id == id_TAUNTAUN) {
                onTaunTaun = 0xff;
            } else if (object->id == id_FIRETRUCK) {
                onFireTruck = 0xff;
            } else if (object->id == id_LIFEBOAT) {
                onLifeBoat = 0xff;
            }
        }
    }
    u8 changed = (onTaunTaun ^ prevOnTaunTaun) | (onFireTruck ^ prevOnFireTruck) | (onLifeBoat ^ prevOnLifeBoat);
    prevOnTaunTaun = onTaunTaun;
    prevOnFireTruck = onFireTruck;
    prevOnLifeBoat = onLifeBoat;
    if (changed == 0) {
        return;
    }
    GIZMOPICKUP_s *pickup = world->pickup_sys->pickups;
    for (i32 index = 0; pickup != NULL && index < world->pickup_sys->pickup_count; ++index, ++pickup) {
        if ((pickup->runtime_flags & 8) == 0) {
            switch (pickup->type_id) {
                case 2:
                    pickup->collected = onTaunTaun;
                    break;
                case 3:
                    pickup->collected = onFireTruck;
                    break;
                case 4:
                    pickup->collected = onLifeBoat;
                    break;
            }
        }
    }
}

// ===========================================================================
// Endor battle (EndorBattle_A / C)
// ===========================================================================

void EndorBattleA_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "bbq_popnull", 1);
}

void EndorBattleC_Init(WORLDINFO_s *world) {
    GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, "force3");
    if (force != NULL)
        force->force_strength = 10.0f;
    force = GizForce_FindByName(world->giz_force_sys, "force4");
    if (force != NULL)
        force->force_strength = 10.0f;
}

void EndorBattleA_Update(WORLDINFO_s *) {
    static f32 parttimer = 0.75f;
    nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[0]);
    if (animation != NULL && animation->playing && NuSpecialGetVisibilityFn(&LevHSpecial[0])) {
        if (parttimer >= 0.75f)
            AddPartDebris(WORLD->part_debris_sys, 7, NuSpecialGetDrawPos(&LevHSpecial[0]));
        parttimer -= FRAMETIME;
        if (parttimer <= 0.0f)
            parttimer = 0.75f;
    }
}

void Platform_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], const_cast<char *>("slave1_level"), 0);
}

void Platform_Reset(WORLDINFO_s *) {
    NuSpecialSetVisibility(&LevHSpecial[0], 0);
}

void E1CharacterBonus_Init(WORLDINFO_s *world) {
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle16");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
}

// ===========================================================================
// Death Star 2 battle
// ===========================================================================

void DeathStar2BattleD_Init(WORLDINFO_s *world) {
    char name[256];
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "reactor1");

    sprintf(name, "lecnode_%i1", 1);
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 1);
    GIZMO *obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[1] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    sprintf(name, "lecnode_%i1", 2);
    LevGizmo[2] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 2);
    obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[2] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    sprintf(name, "lecnode_%i1", 3);
    LevGizmo[3] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 3);
    obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[3] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    sprintf(name, "lecnode_%i1", 4);
    LevGizmo[4] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 4);
    obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[4] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    sprintf(name, "lecnode_%i1", 5);
    LevGizmo[5] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 5);
    obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[5] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    sprintf(name, "lecnode_%i1", 6);
    LevGizmo[6] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, name);
    sprintf(name, "obstacle%d", 6);
    obstacle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);
    if (obstacle != NULL && obstacle->object != NULL)
        LevGizObst[6] = static_cast<GIZOBSTACLE_s *>(obstacle->object);

    LevGizmo[7] = GizmoFindByName(world->gizmo_sys, blowup_gizmotype_id, "shield_inner1");
    LevFlag[5] = 0;
}

GIZMOBLOWUP_s *DeathStar2BattleD_InZapRange(GameObject_s *object);
void DisorientateCode(GameObject_s *object, NUVEC *target, f32 distance);

void DeathStar2BattleD_Update(WORLDINFO_s *) {
    if (LevFlag[0] != 0 && qrand() < 0x800) {
        NewRumbleAllPlayers(QRAND_FLOAT(), 0.0f, 0, 0);
    }

    GIZMO *shield_gizmo = LevGizmo[7];
    if (shield_gizmo != NULL) {
        GIZMOBLOWUP_s *shield = static_cast<GIZMOBLOWUP_s *>(shield_gizmo->object);
        if (shield != NULL && (shield->status_flags & 1) != 0) {
            if (static_cast<i32>(AreaTimer.time_elapsed) % 3 == 0 &&
                static_cast<i32>(AreaTimer.last_time_elapsed) % 3 != 0 &&
                NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) < 0.5f) {
                i32 angle = qrand();
                NUVEC position = shield->mid_position;
                NUVEC direction = {NU_COS_LUT(angle), 0.0f, NU_SIN_LUT(angle)};
                f32 radius = NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 15.0f + 10.0f;
                AddPickups(0, 0, 1, 0, &position, &direction, 3.0f, -1, radius, 2000000.0f, NULL, 1, 0, true);
            }
            return;
        }
    }

    GameObject_s **players_end = Player + 8;
    for (GameObject_s **player = Player; player != players_end; ++player) {
        if (*player == NULL || (*player)->dynamic_light_id == -1) {
            continue;
        }
        GIZMOBLOWUP_s *blowup = DeathStar2BattleD_InZapRange(*player);
        if (blowup == NULL) {
            continue;
        }

        NUVEC colour;
        NUVEC position;
        NUVEC delta;
        rtlDynamicEnable((*player)->dynamic_light_id, 1);
        f32 radius = NuVecDist(&blowup->mid_position, &(*player)->apiobj.collision_position, &delta) * 0.5f;
        rtlDynamicSetRadii((*player)->dynamic_light_id, radius, radius + 5.0f);
        qrand();
        colour.x = 0.0f;
        if (qrand() > 0x7fff) {
            colour.y = 0.25f;
            colour.z = 0.25f;
        } else {
            colour.y = 2.0f;
            colour.z = 2.0f;
        }
        rtlDynamicSetColours((*player)->dynamic_light_id, &colour, NULL);
        position.x = (*player)->apiobj.collision_position.x + delta.x * 0.5f;
        position.y = (*player)->apiobj.collision_position.y + delta.y * 0.5f;
        position.z = (*player)->apiobj.collision_position.z + delta.z * 0.5f;
        rtlDynamicSetPos((*player)->dynamic_light_id, &position);
        f32 length = NuVecDist(&(*player)->apiobj.collision_position, &blowup->mid_position, &delta);
        NuLgtLaser(0, 1.0f, 1.0f, 0.01f, &blowup->mid_position, &delta, 0xff808040, 1.5f, length);
        NewRumble((*player)->pad_gamepad->pad, QRAND_FLOAT() * 0.3f, 0);
        if (qrand() < 0x1000) {
            NewBuzzFrames((*player)->pad_gamepad->pad, 1, 0);
        }
        PlaySfx("ForceLightningLp", &(*player)->apiobj.collision_position);
        DisorientateCode(*player, &blowup->mid_position, 225.0f);
    }
}

GIZMOBLOWUP_s *DeathStar2BattleD_InZapRange(GameObject_s *object) {
    if (object == NULL || static_cast<i8>(object->apiobj.field_0x1f8) >= 0 || object->apiobj.field_0x287 != 0)
        return NULL;

    GIZMOBLOWUP_s *nearest;
    f32 nearest_distance = 225.0f;
    GIZMO *first_gizmo = LevGizmo[1];
    if (first_gizmo != NULL) {
        nearest = static_cast<GIZMOBLOWUP_s *>(first_gizmo->object);
        if (nearest != NULL && (nearest->status_flags & 0x800001) == 0x800000 && LevGizObst[1] != NULL &&
            LevGizObst[1]->anim_set->state != 0) {
            f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &nearest->mid_position, NULL);
            if (distance >= nearest_distance)
                nearest = NULL;
            nearest_distance = distance < nearest_distance ? distance : nearest_distance;
        } else
            nearest = NULL;
    } else
        nearest = NULL;
#define CHECK_ZAP_RANGE(index)                                                                                         \
    if (LevGizmo[index] != NULL) {                                                                                     \
        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(LevGizmo[index]->object);                                 \
        if (blowup != NULL && (blowup->status_flags & 0x800001) == 0x800000 && LevGizObst[index] != NULL &&            \
            LevGizObst[index]->anim_set->state != 0) {                                                                 \
            f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &blowup->mid_position, NULL);              \
            if (distance < nearest_distance) {                                                                         \
                nearest = blowup;                                                                                      \
                nearest_distance = distance;                                                                           \
            }                                                                                                          \
        }                                                                                                              \
    }
    CHECK_ZAP_RANGE(2);
    CHECK_ZAP_RANGE(3);
    CHECK_ZAP_RANGE(4);
    CHECK_ZAP_RANGE(5);
    CHECK_ZAP_RANGE(6);
#undef CHECK_ZAP_RANGE
    return nearest;
}

void DeathStar2BattleA_AlwaysUpdate(WORLDINFO_s *) {
    if (!FreePlay && DEATHSTAR2BATTLEMIDTRO_LDATA != NULL)
        other_level_override = DEATHSTAR2BATTLEMIDTRO_LDATA->idx;
}

// ===========================================================================
// Emperor fight (EmperorFight_A)
// ===========================================================================

struct EmperorFightAPacket {
    i32 state;
    u8 enabled;
    u8 zap;
    u8 player_floor_mask;
    u8 reserved;
};
DECOMP_ASSERT(sizeof(EmperorFightAPacket) == 8, "Emperor fight packet ABI");
EmperorFightAPacket *emperorfighta_netpacket;
f32 eFloor_timer;
i32 floor_route_on;
static u64 routemask_efloor_on;
static u64 routemask_efloor_off;
static nuhspecial_s *hspecial_efloor;

void EmperorFightA_Init(WORLDINFO_s *world) {
    emperorfighta_netpacket = static_cast<EmperorFightAPacket *>(SetLevelHack(8));
    emperorfighta_netpacket->enabled = 1;
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "lift_rlight_bon1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[4], "lift_rlight_bon2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[5], "lift_rlight_bon3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[6], "lift_rlight_bon4", 1);
    LevFlag[1] = 0;
    LevFlag[2] = 0;
    LevFlag[3] = 0;
    LevFlag[4] = 0;
    NuSpecialFind(world->current_gscn, &LevHSpecial[7], "lift_rarr_on1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[8], "lift_rarr_on2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[9], "lift_rarr_on3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[10], "lift_rarr_on4", 1);
    LevFlag[5] = 0;
    LevFlag[6] = 0;
    LevFlag[7] = 0;
    LevFlag[8] = 0;
    NuSpecialFind(world->current_gscn, &LevHSpecial[30], "dot_on_01", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[31], "dot_on_02", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[32], "dot_on_03", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[33], "dot_on_04", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[34], "dot_on_05", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[35], "dot_on_06", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[36], "dot_on_07", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[37], "dot_on_08", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[38], "dot_on_09", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[39], "dot_on_10", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[40], "dot_on_11", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[41], "dot_on_12", 1);
    reinterpret_cast<u8 *>(LevSfxFlag)[0] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[1] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[2] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[3] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[4] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[5] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[6] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[7] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[8] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[9] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[10] = 0;
    reinterpret_cast<u8 *>(LevSfxFlag)[11] = 0;
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle25");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "Deton_011");
        if (blowup != NULL)
            blowup->draw_flags |= 2;
    }
    {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "Deton_031");
        if (blowup != NULL)
            blowup->draw_flags |= 2;
    }
    {
        GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "thermo_light1");
        if (blowup != NULL)
            blowup->draw_flags |= 2;
    }
    char name[10];
    {
        sprintf(name, "force%d", 23);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 24);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 25);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 26);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 27);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 28);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 29);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 30);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 31);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
    {
        sprintf(name, "force%d", 32);
        GIZFORCE_s *force = GizForce_FindByName(world->giz_force_sys, name);
        if (force != NULL)
            force->force_strength = 30.0f;
    }
}

extern i32 obstacle_gizmotype_id, force_gizmotype_id;
void EmperorFightA_Reset(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "ElectricFloor", NULL);
    GIZMO *gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle21");
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[0] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force15");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force16");
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "lift_force_lnull1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "lift_force_rnull1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "lift_main1", 1);
    LevArea[0] = AISysFindArea(world->ai_sys, "ElectricFloor");
    routemask_efloor_on = 0;
    routemask_efloor_off = 0;
    eFloor_timer = 0.0f;
    emperorfighta_netpacket->state = 0;
    for (i32 i = 0; i < world->ai_sys->path_sys->active_path->route_count; ++i) {
        if (NuStrICmp(world->ai_sys->path_sys->active_path->routes[i].name, "electric_on") == 0)
            routemask_efloor_on = static_cast<u64>(1) << i;
        else if (NuStrICmp(world->ai_sys->path_sys->active_path->routes[i].name, "electric_off") == 0)
            routemask_efloor_off = static_cast<u64>(1) << i;
        if (routemask_efloor_on != 0 && routemask_efloor_off != 0)
            break;
    }
    hspecial_efloor = &LevHSpecial[67];
    for (i32 i = 0; i < 20; ++i) {
        char name[32];
        if (i < 9)
            sprintf(name, "elecpad_0%d_on", i + 1);
        else
            sprintf(name, "elecpad_%d_on", i + 1);
        NuSpecialFind(world->current_gscn, &hspecial_efloor[i], name, 1);
    }
    gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "eFloor_AlwaysOn");
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[1] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
    gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "eFloor_AlwaysOf");
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[2] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
    gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "eFloor_OnOff");
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[3] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
}

static inline u64 EmperorFloorAreaMask(WORLDINFO_s *world) {
    i32 index = static_cast<i32>(LevArea[0] - world->ai_sys->areas);
    return static_cast<u64>(static_cast<i64>(static_cast<i32>(1u << (index & 31))));
}
void EmperorFightA_Update(WORLDINFO_s *world) {
    if (LevGameObject[0] == NULL)
        LevGameObject[0] = FindGameObject(id_THEEMPEROR, 1, 1, 0, 0);
    if (FreePlay)
        KillBossCompleteLevel(id_THEEMPEROR, 0, 0.0f);
    else
        KillBossPlayCutScene(id_THEEMPEROR, 0, 0.0f, "emperorfight_outro");
    i32 client = netclient;
    EmperorFightAPacket *packet = emperorfighta_netpacket;
    if (!client) {
        packet->player_floor_mask = 0;
        {
            GameObject_s *player = Player[0];
            if (player != NULL && !player->apiobj.field_0x287 && player->field_0x101c <= 0.0f && LevArea[0] != NULL &&
                (player->apiobj.ai_area_mask & EmperorFloorAreaMask(world)))
                packet->player_floor_mask |= 1 << 0;
        }
        {
            GameObject_s *player = Player[1];
            if (player != NULL && !player->apiobj.field_0x287 && player->field_0x101c <= 0.0f && LevArea[0] != NULL &&
                (player->apiobj.ai_area_mask & EmperorFloorAreaMask(world)))
                packet->player_floor_mask |= 1 << 1;
        }
        f32 state = LevAIMessage[1]->value;
        if (state == 1.0f) {
            if (packet->player_floor_mask) {
                if (packet->zap) {
                    eFloor_timer += FRAMETIME;
                    if ((packet->state == 1 || packet->state == 3) && eFloor_timer > 2.0f) {
                        packet->state = 2;
                        eFloor_timer = 0.0f;
                    } else if (packet->state == 2 && eFloor_timer > 3.0f) {
                        packet->state = 3;
                        eFloor_timer = 0.0f;
                    }
                } else {
                    packet->state = 1;
                    packet->zap = 1;
                    eFloor_timer = 0.0f;
                }
            }
        } else if (state == 2.0f) {
            packet->zap = 0;
            packet->state = 4;
        }
    }
    for (i32 i = 0; i < 2; ++i)
        if (Player[i] != NULL && Player[i]->character_context == 0x42)
            Player[i]->character_context = -1;
    if (client && packet->zap && packet->player_floor_mask && LevGameObject[0] != NULL) {
        AILOCATOR *locator = AIPathFindLocator(world->ai_sys, "Zap");
        NUVEC hands[2], end, delta;
        ForceLightning_Origin(LevGameObject[0], &hands[0], &hands[1]);
        end = locator->position;
        end.x += 0.2f - NuRandFloat() * 0.4f;
        end.z += 0.2f - NuRandFloat() * 0.4f;
        for (i32 i = 0; i < 2; ++i) {
            if (hands[i].y != 1000000000.0f) {
                f32 distance = NuVecDist(&end, &hands[i], &delta);
                NuLgtLaser(lightning_type, lightning_sizew[0], lightning_sizel[0], lightning_sizewab[0], &hands[i],
                           &delta, lightning_col[0], lightning_endw[0], distance);
                PlaySfx("ForceLightningLp", &locator->position);
            }
        }
        LevGameObject[0]->ai.movement_look_target = &locator->position;
        packet = emperorfighta_netpacket;
    }
    bool floor_active = false;
    if (hspecial_efloor != NULL && packet->player_floor_mask) {
        for (i32 floor = 0; floor < 20; ++floor) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&hspecial_efloor[floor]);
            if (animation == NULL || !(animation->ltime > 1.0f))
                continue;
            NUVEC *floor_position = NuSpecialGetDrawPos(&hspecial_efloor[floor]);
            floor_active = true;
            for (i32 i = 0; i < 2; ++i) {
                GameObject_s *player = Player[i];
                if (player->apiobj.field_0x287 || !(player->field_0x101c <= 0.0f) ||
                    !(emperorfighta_netpacket->player_floor_mask & (1 << i)))
                    continue;
                if ((static_cast<i8>(player->apiobj.object_flags) < 0) && Player_HasInvincibility(player))
                    continue;
                player = Player[i];
                if (!(player->apiobj.position.y > floor_position->y))
                    continue;
                f32 dx = player->apiobj.position.x - floor_position->x;
                if (!(dx < 0.35f && dx > -0.35f))
                    continue;
                f32 dz = player->apiobj.position.z - floor_position->z;
                if (!(dz < 0.35f && dz > -0.35f))
                    continue;
                player->character_context = 0x42;
                if (!(player->apiobj.field_0x1f4 & 0x40000))
                    ObjHitObj(NULL, player, 1, 0, 0, 1);
                player = Player[i];
                if (dx > 0.0f)
                    player->apiobj.velocity.x += 0.2f;
                else
                    player->apiobj.velocity.x -= 0.2f;
                if (dz > 0.0f)
                    player->apiobj.velocity.z += 0.2f;
                else
                    player->apiobj.velocity.z -= 0.2f;
                NUVEC floor_end, player_end, delta;
                floor_end.x = player->apiobj.collision_position.x;
                f32 random = NuRandFloat();
                floor_end.x += 0.2f - (random + random) * 0.2f;
                if (floor_end.x > floor_position->x + 0.35f)
                    floor_end.x = floor_position->x + 0.35f;
                else if (floor_position->x - 0.35f > floor_end.x)
                    floor_end.x = floor_position->x - 0.35f;
                floor_end.y = floor_position->y;
                floor_end.z = Player[i]->apiobj.collision_position.z;
                random = NuRandFloat();
                floor_end.z = (0.2f - (random + random) * 0.2f) + floor_end.z;
                if (floor_end.z > floor_position->z + 0.35f)
                    floor_end.z = floor_position->z + 0.35f;
                else if (floor_position->z - 0.35f > floor_end.z)
                    floor_end.z = floor_position->z - 0.35f;
                player = Player[i];
                player_end.x = player->apiobj.collision_position.x;
                f32 height = player->apiobj.upper_position.y - player->apiobj.lower_position.y;
                f32 base = player->apiobj.lower_position.y + 0.25f * height;
                player_end.y = base + (height * 0.5f) * (static_cast<f32>(qrand()) * 0.000015259021893143654f);
                player_end.z = Player[i]->apiobj.collision_position.z;
                f32 distance = NuVecDist(&player_end, &floor_end, &delta);
                NuLgtLaser(testlaser_type, testlaser_sizew, testlaser_sizel, testlaser_sizewab, &floor_end, &delta,
                           0xff808040, testlaser_endw, distance);
                NUVEC *sound_position = &Player[i]->apiobj.collision_position;
                i32 sound = GetSfxId("ForceLightningLp");
                GameAudio_PlaySfxById(sound, sound_position, 0, 1);
                break;
            }
        }
        if (floor_active) {
            for (i32 i = 0; i < 2; ++i) {
                GameObject_s *player = Player[i];
                if (!player->apiobj.field_0x287 && player->field_0x101c <= 0.0f && LevArea[0] != NULL &&
                    (player->apiobj.ai_area_mask & EmperorFloorAreaMask(world)) &&
                    player->apiobj.collision_position.x < 1.5f) {
                    player->apiobj.last_safe_position.x = 1.5f;
                    player->field_0xf00 |= 4;
                    player->apiobj.respawn_position.x = 1.5f;
                }
            }
        }
        packet = emperorfighta_netpacket;
    }
    if (!netclient && routemask_efloor_on && routemask_efloor_off) {
        bool update_routes = false;
        if (packet->enabled || (floor_active && !floor_route_on)) {
            floor_route_on = 1;
            packet->enabled = 0;
            update_routes = true;
        } else if (!floor_active && floor_route_on) {
            floor_route_on = 0;
            update_routes = true;
        }
        if (update_routes) {
            AIPATH *path = world->ai_sys->path_sys->active_path;
            for (i32 i = 0; i < path->connection_count; ++i) {
                AIPATHCNX *connection = &path->connections[i];
                if (connection->route_mask & static_cast<u32>(routemask_efloor_on)) {
                    if (floor_route_on) {
                        connection->traversal_flags[0] &= 0x7fffffff;
                        connection->traversal_flags[1] &= 0x7fffffff;
                    } else {
                        connection->traversal_flags[0] |= 0x80000000;
                        connection->traversal_flags[1] |= 0x80000000;
                    }
                } else if (connection->route_mask & static_cast<u32>(routemask_efloor_off)) {
                    if (floor_route_on) {
                        connection->traversal_flags[0] |= 0x80000000;
                        connection->traversal_flags[1] |= 0x80000000;
                    } else {
                        connection->traversal_flags[0] &= 0x7fffffff;
                        connection->traversal_flags[1] &= 0x7fffffff;
                    }
                }
            }
            for (i32 i = 0; i < 2; ++i)
                if (Player[i] != NULL && (Player[i]->apiobj.object_flags & 0x1001) == 0x1001)
                    AISysGetCharacterPathPos(WORLD->ai_sys, &Player[i]->apiobj, &Player[i]->ai, 0xff, 1);
            if (LevGameObject[0] != NULL)
                AISysGetCharacterPathPos(WORLD->ai_sys, &LevGameObject[0]->apiobj, &LevGameObject[0]->ai, 0xff, 1);
            packet = emperorfighta_netpacket;
        }
    }
    switch (packet->state) {
        case 0:
        case 4:
            GizObstacle_PlayBackwards(LevGizObst[1]);
            GizObstacle_PlayBackwards(LevGizObst[2]);
            GizObstacle_PlayBackwards(LevGizObst[3]);
            break;
        case 1:
            GizObstacle_PlayForwards(LevGizObst[1]);
            GizObstacle_PlayForwards(LevGizObst[2]);
            GizObstacle_PlayForwards(LevGizObst[3]);
            break;
        case 2:
            GizObstacle_PlayForwards(LevGizObst[1]);
            GizObstacle_PlayBackwards(LevGizObst[2]);
            GizObstacle_PlayBackwards(LevGizObst[3]);
            break;
        case 3:
            GizObstacle_PlayForwards(LevGizObst[1]);
            GizObstacle_PlayBackwards(LevGizObst[2]);
            GizObstacle_PlayForwards(LevGizObst[3]);
            break;
    }
    if (!LevFlag[4]) {
        {
            if (!LevFlag[1] && NuSpecialGetVisibilityFn(&LevHSpecial[1 + 2])) {
                PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[1 + 2]));
                LevFlag[1] = 1;
            }
        }
        {
            if (!LevFlag[2] && NuSpecialGetVisibilityFn(&LevHSpecial[2 + 2])) {
                PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[2 + 2]));
                LevFlag[2] = 1;
            }
        }
        {
            if (!LevFlag[3] && NuSpecialGetVisibilityFn(&LevHSpecial[3 + 2])) {
                PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[3 + 2]));
                LevFlag[3] = 1;
            }
        }
        {
            if (!LevFlag[4] && NuSpecialGetVisibilityFn(&LevHSpecial[4 + 2])) {
                PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[4 + 2]));
                LevFlag[4] = 1;
            }
        }
    }
    u8 *sound_flags = reinterpret_cast<u8 *>(LevSfxFlag);
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 0]) && !sound_flags[0]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[0]));
            sound_flags[0] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 0]))
            sound_flags[0] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 1]) && !sound_flags[1]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[1]));
            sound_flags[1] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 1]))
            sound_flags[1] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 2]) && !sound_flags[2]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[2]));
            sound_flags[2] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 2]))
            sound_flags[2] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 3]) && !sound_flags[3]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[3]));
            sound_flags[3] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 3]))
            sound_flags[3] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 4]) && !sound_flags[4]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[4]));
            sound_flags[4] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 4]))
            sound_flags[4] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 5]) && !sound_flags[5]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[5]));
            sound_flags[5] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 5]))
            sound_flags[5] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 6]) && !sound_flags[6]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[6]));
            sound_flags[6] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 6]))
            sound_flags[6] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 7]) && !sound_flags[7]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[7]));
            sound_flags[7] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 7]))
            sound_flags[7] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 8]) && !sound_flags[8]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[8]));
            sound_flags[8] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 8]))
            sound_flags[8] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 9]) && !sound_flags[9]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[9]));
            sound_flags[9] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 9]))
            sound_flags[9] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 10]) && !sound_flags[10]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[10]));
            sound_flags[10] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 10]))
            sound_flags[10] = 0;
    }
    {
        if (NuSpecialGetVisibilityFn(&LevHSpecial[30 + 11]) && !sound_flags[11]) {
            PlaySfx("env_block_light_on", NuSpecialGetDrawPos(&LevHSpecial[11]));
            sound_flags[11] = 1;
        } else if (!NuSpecialGetVisibilityFn(&LevHSpecial[30 + 11]))
            sound_flags[11] = 0;
    }
}

void EmperorFightA_Panel(WORLDINFO_s *) {
    drawbosshitpoints_2rows = 1;
    if (LevGameObject[0] != NULL && LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f)
        DrawBossHitPoints(LevGameObject[0]);
}

// ===========================================================================
// Fire / slow-down helpers (Death Star 2 fire)
// ===========================================================================

SPLINEPOS_s fireSplinePos;
SPLINEPOS_s fireBackPos;
f32 runningTotalPos;
f32 fireSpeedScale;
f32 fireDeltaPos;
i32 fire_clip_dist = 1000;
extern void (*LEGO_SET_SLOWDOWNFN)(GameObject_s *);
void DeathStar2BattleFire_SetSlowDownMul(GameObject_s *object);
void DeathStar2BattleFire_UpdateSlowDownMul(f32 dt);
extern i32 objhitobj_nohurtsfx, objhitobj_noimpactsfx;
extern "C" void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

void DeathStar2BattleFire_Draw(WORLDINFO_s *world) {
    u16 yaw = 0;
    u16 pitch = 0;
    NUVEC point = v000;
    NUMTX matrix;
    NUVEC difference;
    NUVEC average;
    SPLINEPOS_s position;
    if (Paused != 0)
        return;

    position = fireSplinePos;
    while (position.normalized_position > 0.0f) {
        NuMtxSetIdentity(&matrix);
        PointAlongSpline(LevelCodeSpline[0], position.normalized_position, &point, &yaw, &pitch, 0);
        Players_AveragePos(&average, NULL);
        difference.x = average.x - position.position.x;
        difference.y = average.y - position.position.y;
        difference.z = average.z - position.position.z;
        if (NuVecMagSqr(&difference) < fire_clip_dist)
            AddVariableShotDebrisEffectTimed1(world->debris_sys->entries[121].effect, &position.position, 33, FRAMETIME,
                                              pitch, yaw, NULL);
        MoveSplinePosition(&position, -40.0f);
    }

    position = fireBackPos;
    while (position.normalized_position < fireSplinePos.normalized_position) {
        NuMtxSetIdentity(&matrix);
        PointAlongSpline(LevelCodeSpline[0], position.normalized_position, &point, &yaw, &pitch, 0);
        Players_AveragePos(&average, NULL);
        difference.x = average.x - position.position.x;
        difference.y = average.y - position.position.y;
        difference.z = average.z - position.position.z;
        if (NuVecMagSqr(&difference) < fire_clip_dist)
            AddVariableShotDebrisEffectTimed1(world->debris_sys->entries[121].effect, &position.position, 67, FRAMETIME,
                                              pitch, yaw, NULL);
        MoveSplinePosition(&position, 40.0f);
    }

    position = fireBackPos;
    while (position.normalized_position > 0.0f) {
        NuMtxSetIdentity(&matrix);
        PointAlongSpline(LevelCodeSpline[0], position.normalized_position, &point, &yaw, &pitch, 0);
        Players_AveragePos(&average, NULL);
        difference.x = average.x - position.position.x;
        difference.y = average.y - position.position.y;
        difference.z = average.z - position.position.z;
        if (NuVecMagSqr(&difference) < fire_clip_dist)
            AddVariableShotDebrisEffectTimed1(world->debris_sys->entries[121].effect, &position.position, 33, FRAMETIME,
                                              pitch, yaw, NULL);
        MoveSplinePosition(&position, -40.0f);
    }
}

void DeathStar2BattleFire_Init(WORLDINFO_s *world) {
    LEGO_SET_SLOWDOWNFN = DeathStar2BattleFire_SetSlowDownMul;
    LevelCodeSpline[0] = NuSplineFind(world->current_gscn, const_cast<char *>("fire"));
    NuSpecialFind(WORLD->current_gscn, &LevHSpecial[0], const_cast<char *>("fire_cube"), 0);
    InitSplinePosition(&fireSplinePos, LevelCodeSpline[0], 0.0f, 0);
    InitSplinePosition(&fireBackPos, LevelCodeSpline[0], 0.0f, 0);
    runningTotalPos = 0.0f;
    fireSpeedScale = 1.0f;
}

void DeathStar2BattleFire_Update(WORLDINFO_s *world) {
    u16 yaw = 0;
    u16 pitch = 0;
    NUVEC point = v000;
    NUMTX matrix;
    NUVEC difference;
    DeathStar2BattleFire_UpdateSlowDownMul(FRAMETIME);
    if (Player[0] != NULL)
        fireDeltaPos =
            1.25f * static_cast<GAMECHARACTERDATA *>(Player[0]->apiobj.character_data->field11_0x24)->run_speed;
    else
        fireDeltaPos = 30.0f;

    f32 choice = NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 4.0f;
    i32 effect;
    if (choice < 1.0f)
        effect = 33;
    else if (choice < 2.0f)
        effect = 48;
    else if (choice < 3.0f)
        effect = 35;
    else
        effect = 36;
    f32 along = fireSplinePos.normalized_position;
    along += NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 0.1f;
    PointAlongSpline(LevelCodeSpline[0], along, &point, &yaw, &pitch, 0);
    i16 y_rotation = qrand();
    i16 z_rotation = qrand();
    AddVariableShotDebrisEffectTimed1(world->debris_sys->entries[effect].effect, &point, 90, FRAMETIME, z_rotation,
                                      y_rotation, NULL);

    NuMtxSetIdentity(&matrix);
    PointAlongSpline(LevelCodeSpline[0], fireSplinePos.normalized_position, &point, &yaw, &pitch, 0);
    NuMtxPreRotateY(&matrix, yaw);
    NuMtxTranslate(&matrix, &fireSplinePos.position);
    if (fireSplinePos.normalized_position < 1.0f)
        NuSpecialSetDrawMtx(&LevHSpecial[0], &matrix);
    else
        NuSpecialSetVisibility(&LevHSpecial[0], 0);
    if (fireBackPos.normalized_position <= 0.0f)
        fireBackPos = fireSplinePos;

    f32 nearest_distance = 1000000000.0f;
    GameObject_s *nearest = NULL;
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] != NULL && (Player[index]->action_flags & 0x20) == 0) {
            NuVecSub(&difference, &Player[index]->apiobj.position, &fireSplinePos.position);
            NuVecRotateY(&difference, &difference, -static_cast<i32>(yaw));
            if (difference.z < nearest_distance) {
                nearest_distance = difference.z;
                if (difference.z < 0.0f)
                    nearest = Player[index];
            }
            if (difference.z < 0.0f) {
                i32 no_hurt = objhitobj_nohurtsfx;
                if (Player[index]->apiobj.field_0x287 != 0)
                    fireSpeedScale = 0.2f;
                // Retail emits at the nearest player found so far, then hits the current slot.
                AddVariableShotDebrisEffectTimed3(world->debris_sys->entries[115].effect, &nearest->apiobj.position,
                                                  &nearest->apiobj.velocity, 40, FRAMETIME, NULL, NULL);
                if ((Player[index]->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) == 0) {
                    objhitobj_nohurtsfx = 1;
                    objhitobj_noimpactsfx = 1;
                    ObjHitObj(NULL, Player[index], 1, 0, 0, 1);
                    objhitobj_nohurtsfx = no_hurt;
                }
            }
        }
    }
    if (fireSpeedScale < 1.0f && nearest == NULL)
        fireSpeedScale += FRAMETIME / 3.0f;
    else if (fireSpeedScale == 0.2f && nearest != NULL)
        fireSpeedScale = 0.2f;
    else if (nearest_distance > 10.0f)
        fireSpeedScale = 1.75f;
    else
        fireSpeedScale = 1.0f;

    if (WORLD->current_level == DEATHSTAR2BATTLEE_LDATA || LevelTimer.time_elapsed > 0.5f)
        MoveSplinePosition(&fireSplinePos, fireDeltaPos * FRAMETIME * fireSpeedScale);
    MoveSplinePosition(&fireBackPos, -(fireDeltaPos * FRAMETIME));
    if (NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) > 0.95f) {
        switch (static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 3.0f)) {
            case 1:
                PlaySfx("explode_SDest", &fireSplinePos.position);
                break;
            case 2:
                PlaySfx("explode_SDG", &fireSplinePos.position);
                break;
            default:
                PlaySfx("exp_asteroid", &fireSplinePos.position);
                break;
        }
    }
    PlaySfx("env_lantern_lp", &fireSplinePos.position);
}

static f32 slowDownTimer[2];
f32 DeathStar2BattleFire_GetSlowDownMul(GameObject_s *object) {
    if (object != NULL) {
        i32 index = object->apiobj.field_0x27c;
        if ((u8)index < 2) {
            if (slowDownTimer[index] < 0.0f)
                slowDownTimer[index] = 0.0f;
            else
                return (3.0f - slowDownTimer[index]) / 3.0f;
        }
    }
    return 1.0f;
}
void DeathStar2BattleFire_SetSlowDownMul(GameObject_s *object) {
    if (WORLD->current_level != DEATHSTAR2BATTLEE_LDATA && WORLD->current_level != DEATHSTAR2BATTLEF_LDATA &&
        WORLD->current_level != DEATHSTAR2BATTLEG_LDATA)
        return;
    if (object != NULL) {
        i32 index = object->apiobj.field_0x27c;
        if ((u8)index < 2) {
            object->apiobj.velocity = v000;
            slowDownTimer[index] = 3.0f;
        }
    }
}
void DeathStar2BattleFire_UpdateSlowDownMul(f32 dt) {
    for (i32 index = 0; index < 2; ++index)
        if (slowDownTimer[index] > 0.0f)
            slowDownTimer[index] -= dt;
}
