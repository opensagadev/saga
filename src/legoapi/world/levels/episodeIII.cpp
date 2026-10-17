// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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
#include "legoapi/characters/motion/chris.h"
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
#include "legoapi/world/levels/episodeIII_types.h"
DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");

static VADERA_s vader_a;
static GIZAIMESSAGE_s *vader_b_complete_msg; // Vader B "complete" message handle
static u8 vader_b_playersDead;               // Vader B player-death flag

static void VaderA_StartCollapseStage(WORLDINFO_s *world);


DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");

extern "C" {
    CRUISERCNETPACKET_s *crusiserc_netpacket = NULL;
}
static CRUISERC_s cruiser_c;

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

extern AREADATA *DOGFIGHT_ADATA;

speedup_s DogFightSpeedList[] = {
    {58.0f, 0.5f},  {72.0f, 1.0f},  {174.0f, 0.5f}, {183.0f, 1.0f}, {207.0f, 0.5f},
    {220.0f, 1.0f}, {313.0f, 0.5f}, {335.0f, 1.0f}, {0.0f, 0.0f},
};

// ===========================================================================
// Cruiser (Cruiser_A / Cruiser_C / Cruiser_D)
// ===========================================================================

void CruiserAInit(WORLDINFO_s *world);

void CruiserAUpdate(WORLDINFO_s *);

void CruiserCReset(WORLDINFO_s *);

void CruiserCPanel(WORLDINFO_s *);

void CruiserCUpdate(WORLDINFO_s *);

void CruiserDInit(WORLDINFO_s *world);

void CruiserDUpdate(WORLDINFO_s *);

void CruiserDReset(WORLDINFO_s *);

// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================

void GrievousA_Init(WORLDINFO_s *world);

void GrievousA_Reset(WORLDINFO_s *);

void GrievousA_Update(WORLDINFO_s *world);

// ===========================================================================
// Kashyyyk (Kashyyyk_A / Kashyyyk_B / Kashyyyk_C / Kashyyyk_D)
// ===========================================================================

void KashyyykA_Init(WORLDINFO_s *world);

void KashyyykB_Init(WORLDINFO_s *);

void KashyyykC_Init(WORLDINFO_s *world);

void KashyyykD_Init(WORLDINFO_s *);

void KashyyykA_Panel(WORLDINFO_s *);

void KashyyykA_Reset(WORLDINFO_s *);

void KashyyykB_Reset(WORLDINFO_s *);

void KashyyykD_Reset(WORLDINFO_s *world);

i32 AnakinGreenSabre(GameObject_s *obj);

void KashyyykA_Update(WORLDINFO_s *);

void KashyyykB_Update(WORLDINFO_s *);

void KashyyykC_Update(WORLDINFO_s *);

void KashyyykD_Update(WORLDINFO_s *);

// ===========================================================================
// Temple (Temple_A / Temple_C)
// ===========================================================================

void TempleA_Init(WORLDINFO_s *world);

void TempleC_Init(WORLDINFO_s *world);

void TempleC_AlwaysUpdate(WORLDINFO_s *);

// ===========================================================================
// Vader (Vader_A / Vader_B / Vader_C)
// ===========================================================================

void *vadera_netpacket;


static NUVEC vadar_cam_pos = {0.0f, 0.88f, -12.4f};
static NUVEC vadar_cam_tgt = {0.0f, 2.0f, -17.4f};

void GameCameraMakeMiniCut2(NUVEC *, NUVEC *, i32, f32, f32, f32, f32, i32, i32, i32);
void TickTockSfx();
int LoseCoins(GameObject_s *, i32);

void VaderA_Init(WORLDINFO_s *world);

void VaderB_Init(WORLDINFO_s *);

i32 Vader_ObiWanKilledAnakin;
void *vaderc_netpacket;

void VaderC_Init(WORLDINFO_s *world);

void VaderA_Reset(WORLDINFO_s *world);

void VaderB_Reset(WORLDINFO_s *);

void VaderC_Reset(WORLDINFO_s *);

void VaderA_Update(WORLDINFO_s *);

void VaderB_Update(WORLDINFO_s *);

void VaderC_Update(WORLDINFO_s *world);

void VaderA_DrawPanel(WORLDINFO_s *);

void VaderB_DrawPanel(WORLDINFO_s *);

void VaderC_DrawPanel(WORLDINFO_s *);

void VaderA_GoneThroughDoor(WORLDINFO_s *world, DOOR_s *door);

static void VaderA_StartCollapseStage(WORLDINFO_s *world);

// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================

void ANewHopeA_Init(WORLDINFO_s *world);

// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern void (*BuckStartExtraFn)(GameObject_s *);
void NewRumble(nupad_s *, f32, i32);

void Buck_Start(GameObject_s *object, GameObject_s *rider);

extern i16 id_SNOWMOB;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);



extern PART_s *boulder_part[2];
extern i32 boulder_blowup_type;
NUVEC boulder_oldpos[2];
extern "C" void KillPart(PART_s *, i32);

void Boulder_Kill(PART_s *part, i32);

void Boulder_Move(PART_s *part, float);






void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount);


i32 Conveyor_AdjustSpeed(NUVEC *velocity);

void LevelObjects_InitForLevel(WORLDINFO_s *world);

void EquivalentObjects_Configure(WORLDINFO_s *world, char *config);

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"

#include <stdio.h>
#include <string.h>

void edpartScaleType(i32 type, f32 scale);
void edpartInitType(i32 type);
i32 edpartSaveEffects(char *path, char page);

extern "C" {
extern i32 edpart_set_part;
extern i32 edpart_filter;
extern char edpart_filter_string[16];
extern i32 edpart_first_time_this_level;
extern i32 edpart_curr;
extern i32 edpart_create_type;
extern i32 edpart_nearest_orphans;
extern i32 edpart_nearest_duplicates;
extern i8 edpart_effect_list;
extern i32 edpart_emitrotx;
extern i32 edpart_emitroty;
extern i32 edpart_emitrotz;
extern i32 edpart_roty;
extern i32 edpart_rotz;
extern f32 edpart_offset;
extern i32 edpart_dpad_mode;
extern i32 edpart_copy_mode;
extern i32 edpart_copy_enclosed;
extern i32 edpart_copy_source[8];
extern i32 edpart_copy_source_count;
extern NUVEC edpart_copy_source_vec;
extern i32 edpart_copyrotz;
extern f32 edpart_copy_size;
extern i32 edpart_copyroty;
extern i32 edpart_snap_enabled;
extern i32 edpart_refroty;
extern i32 edpart_refrotz;
extern i32 edpart_cam_ax;
extern i32 edpart_cam_ay;
    extern NUVEC edpart_cam_pos;
extern i32 edpart_num_orphans;
extern i32 edpart_readout;
extern f32 edpart_scale_factor;
extern NUMTL * edpart_mtl;
extern NUMTL * edpart_boxmtl;
    extern i32 edpart_nearest;
    extern part_emit_s *edpart_nearest_emit;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern i32 part_platimpactcnt;
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern char edbits_general_save_directory[256], edbits_general_save_name[256], edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    extern i32 edbits_override_backups;
    void ResetParts(void);
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_type_menu;
    void eduiMenuRender(eduimenu_s *menu);
extern eduimenu_s * edpart_active_menu;
extern eduimenu_s * edpart_opt_menu;
extern eduimenu_s * edpart_sscale_menu;
extern eduimenu_s * edpart_message_menu;
extern eduimenu_s * edpart_name_menu;
extern eduimenu_s * edpart_data_menu;
extern eduimenu_s * edpart_scaletype_menu;
extern eduimenu_s * edpart_switchtype_menu;
extern eduimenu_s * edpart_switch_menu;
extern eduimenu_s * edpart_soundcontrol_menu;
extern eduimenu_s * edpart_soundid_menu;
extern eduimenu_s * edpart_soundx_menu;
extern eduimenu_s * edpart_sounds_menu;
extern eduimenu_s * edpart_debrisscale_menu;
extern eduimenu_s * edpart_partindex_menu;
extern eduimenu_s * edpart_impactpart_menu;
extern eduimenu_s * edpart_debrisindex_menu;
extern eduimenu_s * edpart_diedebris_menu;
extern eduimenu_s * edpart_impactdebris_menu;
extern eduimenu_s * edpart_emitterdebris_menu;
extern eduimenu_s * edpart_trail2debris_menu;
extern eduimenu_s * edpart_trail1debris_menu;
extern eduimenu_s * edpart_debrissettings_menu;
extern eduimenu_s * edpart_cutoff_menu;
extern eduimenu_s * edpart_emittime_menu;
extern eduimenu_s * edpart_changegenrate_menu;
extern eduimenu_s * edpart_varemit_menu;
extern eduimenu_s * edpart_varstart_menu;
extern eduimenu_s * edpart_grav_menu;
extern eduimenu_s * edpart_emitvel_menu;
extern eduimenu_s * edpart_emit_menu;
extern eduimenu_s * edpart_instancescale_menu;
extern eduimenu_s * edpart_tint_menu;
extern eduimenu_s * edpart_instanceflags_menu;
extern eduimenu_s * edpart_instorient_menu;
extern eduimenu_s * edpart_maxlife_menu;
extern eduimenu_s * edpart_instancesettings_menu;
extern eduimenu_s * edpart_instanceorphans_menu;
extern eduimenu_s * edpart_thingsinstance_menu;
extern eduimenu_s * edpart_worldinstance_menu;
extern eduimenu_s * edpart_instance_menu;
extern eduimenu_s * edpart_leveltype_menu;
extern eduimenu_s * edpart_generaltype_menu;
extern eduimenu_s * edpart_type_menu;
}




extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_name_menu;
    extern i32 edpart_nearest;
    extern i32 edpart_create_type;
    extern f32 edpart_scale_factor;
    extern part_typedesc_s part_types[128];
}

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32);

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *, u32);
static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32);

extern "C" {
extern i32 edpart_which_scene;
extern i32 edpart_set_debris;
extern i32 edpart_part_list;
extern i32 edpart_particle_list;
}


static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32);
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSoundIDMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32);
static void edpartLevelTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void edpartFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);


static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32);





static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32);


static void edpartSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);








static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32);





static void edpartWorldInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);




static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32);



// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartPartIndexMenu(eduimenu_s *menu) __asm__("_ZL19edpartPartIndexMenuP10eduimenu_sP10eduiitem_sj.isra.12");
static void edpartPartIndexMenu(eduimenu_s *menu);

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartThingsInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);





static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32);




static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);


// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartDebrisIndexMenu(eduimenu_s *menu) __asm__("_ZL21edpartDebrisIndexMenuP10eduimenu_sP10eduiitem_sj.isra.13");
static void edpartDebrisIndexMenu(eduimenu_s *menu);

static void edpartLevelDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);








static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);










static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceOrphans(eduimenu_s *menu, eduiitem_s *, u32);



static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceDuplicates(eduimenu_s *menu, eduiitem_s *, u32);


extern "C" {
extern ed_module_s edpartdesc;
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

extern i16 TerrImpact;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
extern NUVEC TerrImpactPos;
extern NUVEC TerrImpactNormal;
extern NUVEC ShadNorm;
extern NUVEC ShadRoofNorm;
extern NUVEC EShadNorm;
extern NUVEC EShadRoofNorm;
extern f32 ShadRoofY;
extern f32 EShadRoofY;
extern TerrainLastImpact_s TerrLastImpact;
extern TERRSET * CurTerr;
// Runtime-selected groups appended after the fixed terrain allocation.
extern i32 WallSplinesOnly;
extern "C" {
extern i32 IgnoreWallSplines;
}
#include "legoapi/render/core/terrain_stubs_types.h"

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

i32 HitTerrain(void);
void RayImpact(NUVEC *);

extern "C" {
    i32 DebrisCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisCollisionCheckScaleY(NUVEC *, f32, f32);
    i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisTorusCollisionCheckScaleY(NUVEC *, f32, f32);
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 EDPP_SCALE_TYPES;
    extern debinftype *effecttypes;
    extern debscale_s *debscale;
    extern f32 panelglobaltime;
    extern f32 globaltime;
    extern f32 renderpanelglobaltime;
    extern f32 renderglobaltime;
    extern f32 timeincrement;
    extern i32 globalframes;
    extern i32 update_debris_enabled;
    extern i32 debris_setup_called;
    extern usize debris_trash_space;
    extern usize debris_trash_size;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern debris_chunk_control_s *debris_chunk_controls;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debkeydatatype_s *debris_keystack;
    extern u32 *spherecolldata;
    extern u32 *toruscolldata;
    extern i32 numspherecolldata;
    extern i32 numtoruscolldata;
    extern NUMTL *DebMat[10];
    extern i32 debris_suspended;
    void DebrisReScale(i32, f32);
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
    extern "C++" {
        void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
        void DebrisCleanUpDmaDebTypeTables(void);
        void DebrisProcessAllocation(void);
        void DebrisProcessControlChunks(i32);
        void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
    }
}

void DebrisFreeOldestDmaDebTypeTable();

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
extern "C" i32 part_types_used;
extern "C" f32 partglobaltime;
void edpartScaleType(i32 index, f32 scale);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page);

void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags);

extern "C" {

void AITerrInit(void);

f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    i32 NewShadowOnPlatform(void);

i32 AITerrShadowOnPlatform(void);

i32 CheckForPlatInst(i32 instance);


i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

void DebFreeAllCreatedEffects(void);

void DebFreeAllDMADebTablesInstantly(void);

void DebFreeAllPanelEffects(void);

void Debris(i32 update_panel_time);

i32 DebrisAllCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

} // extern "C"



extern "C" {

void DebrisEmitterMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);

void DebrisEmitterOrientationMtx(i32 handle, NUMTX *source);

void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z);

i32 DebrisFindAllOfType(i32 effect_index, NUVEC *positions, i32 *handles, i32 capacity, i32 skip, i32 active_only);

i32 DebrisGetConeProperties(i32 handle, NUVEC *origin, NUVEC *end, f32 *start_radius, f32 *end_radius,
                                NUVEC *direction, f32 *length, f32 *speed);

f32 DebrisGetDuration(i32 effect_index);

char *DebrisGetName(i32 effect_index);

i32 DebrisGetParticleCount(i32 handle);

i32 DebrisGetRingProperties(i32 handle, NUVEC *origin, NUVEC *direction, f32 *radius);

void DebrisOrientation(i32 handle, i16 z, i16 y);

void DebrisOrientationMtx(i32 handle, NUMTX *source);

void DebrisParticleMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisPopulateInstance(i32 handle, f32 duration);

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);

void DebrisProcessTimeSlip(void);

i32 DebrisQueryPriority(i32 effect_index);

void DebrisReScale(i32 effect_index, f32 scale);

void DebrisReflectionOrientation(i32 handle, i16 x, i16 y, f32 plane, f32 scale);

extern NUVEC * CutoffCameraVec;

void DebrisRegisterCutoffCameraVec(void *position);

void DebrisReserveTrashableSpace(VARIPTR *buffer, VARIPTR, i32 chunk_count, i32 glass_chunk_count);

void DebrisResetTimers(void);

void DebrisSetDetailLevels(i32 handle, i32 detail_levels);

void DebrisSetDrawFlag(i32 handle, i8 draw_flag);

void DebrisSetFacing(i32 handle, u8 enabled, i16 x_angle, i16 y_angle);

void DebrisSetGroupID(i32 handle, i16 group);

void DebrisSetPriority(i32 handle, u16 priority, u8 automatic);

extern i32 debris_render_group;
void DebrisSetRenderGroup(i32 group);

void DebrisSetRoomID(i32 handle, nugscn_s *scene);

void DebrisSetTrigger(i32 handle, i32 first, i32 second, f32 third);

void DebrisSetUserData(i32 handle, void *user_data);

void DebrisShift(NUMTX *transform);

void DebrisStartOffset(i32 handle, f32 offset);

void DebrisStatusAlwaysOff(i32 *handle);

void DebrisStatusAlwaysOn(i32 *handle);

void DebrisStatusNormal(i32 *handle);

i32 DebrisTorusCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisTorusCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void DebrisTrashableSetup(VARIPTR *buffer, VARIPTR *);

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

void DrawPlatform(i32 platform_index);

i32 FindPlatInst(i32 instance);

i32 NewMSituTerrEx(NUVEC *position, void *data, NUVEC *minimum, NUVEC *maximum, f32 radius);

i32 NewPlatInst(void *object, i32 instance);

i32 NewPlatInstMSitu(void *object, i32 source);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void NewTerrain(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius, f32 collision_radius,
                    i32 scan_flags);

void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void PartTerrInit(void);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstCenter(i32 index, NUVEC *position);

i32 PlatInstGetHit(i32 index);

void PlatInstRotate(i32 index, i32 rotate);

void PlatSkinMemRigister(void *start, void *end, i32 maximum);

void PlatSkinMemReset(void);

i32 PlatInstSkinRegisterEx(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags,
                               TERRSET *source);

i32 PlatInstSkinRegister(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags);

void PlatOnOff(i32 index, i32 enabled);

void *PlatSkinEndReigster(i32 cache_slots);

void PlatformRemoveCallback(void (*function)(void *));

void PlatformUpdateCallback(void (*function)(void *), void *argument);

void SortDebrisRenderStack(void);

const char *TerrErrorString(i32 error);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

i32 TerrainFreeId(void *id);

TERRSET *TerrainGetCur(void);

TERRAIN_GROUP *TerrainGetModelByInst(i32 instance);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

void *terraininit(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene, i32 group);

void UpdatePlatinst(i32 index, void *object);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern ADDPART_s Default_ADDPART;

struct rtlset;
void NewPartOrderedRotation(PART_s *);
void UpdatePartEmits(f32);

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.
extern i32 debris_suspended;

    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    extern f32 globaltime;
    extern f32 partglobaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void DebrisEmitterMomentum(i32, f32, f32, f32);
    void DebrisParticleMomentum(i32, f32, f32, f32);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    void DebrisSetUserData(i32, void *);
    i32 CreateScaledPARTEffect(i32, f32);
    extern i32 debris_suspended;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern u32 partseed;
    extern NUGSCN *part_scene[32];

void NuPartEnableRayCasts(i32 enabled);

u32 NuPartGetSeed(void);

void NuPartSetSeed(i32 seed);

    extern part_type_s part_types[128];
    extern NUGSCN *part_scene[32];
    extern part_emit_s part_emits[40];
    extern i32 part_emits_used;
    void CheckPartCount();
    i32 AddPARTEffect(i32, NUVEC *);
    void PARTStartOffset(i32, f32);

    void ResetParts(void);

    i32 DebAlloc(void);
    void DebrisStartOffset(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisOrientation(i32, i16, i16);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;



// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
extern f32 ForceThrowSpeed, ForceThrowGravity;
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);



extern i32 GAMEDEMO;



extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);


void PowerUp_Particles(WORLDINFO_s *, NUVEC *);






void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;




extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

void AddFiniteShotDebrisEffectUserData(i32 *handle, i32 effect, NUVEC *position, i32 count, void *user_data);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 AddPARTEffect(i32 effect, NUVEC *position);

    void KillPart(PART_s *, i32);
    void NewPartRotation(PART_s *);
    void DebrisPopulateInstance(i32, f32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

void AddRotatedDebrisEffect(i32 *handle, i32 effect, f32 x, f32 y, f32 z, i16 rotation_x, i16 rotation_y);

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position, NUVEC *orientation, NUVEC *momentum,
                                         i32 count, f32 scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

i32 AddScaledVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                          i16 y_rotation, f32 scale);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

i32 AddScaledVariableShotDebrisEffect1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect2(i32 effect_index, NUVEC *position, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, i32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                           i8 flags, f32 scale);

i32 AddScaledVariableShotDebrisEffect5(i32 effect, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 time, NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                           u16 priority, i8 flags, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

    void AddVariableShotDebrisEffectMtx(i32, NUVEC *, i32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation,
                                        NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed3(i32 effect_index, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void CheckPartCount(void);

void DrawParts(i32 keep_offscreen);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 GetMaxPartTypes(void);

i32 GetPartCount(void);

char *GetPartName(i32 index);

extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
extern i32 part_types_used;
extern i32 part_emits_used;
extern NUGSCN * part_scene[32];
    // The editor loader clears these page IDs with aligned vector stores.
extern i32 part_scene_pageid[32];
extern i32 part_platimpactcnt;
extern i16 part_platimpactlist[4];
    void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void KillAllParts(void);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

void KillPartsByScene(NUGSCN *scene);

void NewPartRotation(PART_s *part);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

f32 PARTGetTotalOffTime(i32 index);

f32 PARTGetTotalOnTime(i32 index);

i32 PARTLookupType(char *name);

extern i32 edpart_load_particle_page;

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void PARTStartOffset(i32 index, f32 offset);

void ParticleReset(void);

void RemovePARTEffect(i32 index);

void ResetParts(void);

    void DebrisStatusAlwaysOff(i32 *);
    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);


extern "C" {
extern i32 edpart_nearest;
extern NUVEC edpart_cam_pos;
extern part_emit_s * edpart_nearest_emit;
extern part_typedesc_s * edpart_nearest_type;
}

extern "C" {
extern i32 edbits_part_level_page;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}


extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);


void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_3D(PART_s *);
void PartCollide_2D(PART_s *);
void PartUpdate_Heart(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);




void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);


void PartCollide_2D(PART_s *part);

void PartCollide_3D(PART_s *part);



// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

void UpdatePartEmits(f32 time);





extern "C" i32 GetSfxId(const char *);

void PartImpact_Brick(PART_s *part);

void PartUpdate_Heart(PART_s *part);


void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);




void NewPartOrderedRotation(PART_s *part);

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

extern "C" i32 edSfxAllCount;

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")






extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
extern "C" i32 NuRndrDoingScreenGrab;



// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppPtlDestroy(i32 index);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}

void edppPtlShelve(i32 index);


extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.





static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);



static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);





















// Particle editor UI/menu callbacks.








static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);





static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);



static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);





static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);




static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);



static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);



static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);


static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);



static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);


static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);




static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);


static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

















static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);














// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);


// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDetermineNearest(f32 distance);
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

















// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif


// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);

#include "gameapi/edtools/edtools_types.h"


extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}












void edbriDetermineNearest(f32);
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}










edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern f32 edanim_cam_dist;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}



extern "C" void NuBridgeRemove(i32 bridge);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);




void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);


void edcamSetContoller(i32 invert_pitch);



extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);


static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern EdRegistry theRegistry;
extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
extern VuVec SpherePos1;
extern VuVec SpherePos2;
extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern "C" void eduiSetCameraEnabled(i32);
extern "C" void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
extern "C" i32 edbri_nearest;
extern "C" NUVEC edbri_cam_pos;
extern "C" NUMTL *edbri_mtl, *edbri_mtl_zoff;
extern "C" f32 edbri_length, edbri_width;
extern "C" i32 edbri_rotz, edbri_roty, edbri_planks, edbri_post_interval;
extern "C" i32 edbri_plank_instance_type, edbri_post_instance_type, edbri_bridges_used;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void edbitsDrawCircleTilted(NUVEC *, f32, i32, NUMTL *, i32, i32);
extern "C" void eduiAddPropTextPickEnt(eduimenu_s *, eduiitem_s *);
extern "C" void PlaySfxById(i32, nuvec_s *);
extern i32 EdType_Char;
extern i32 EdType_Short;
extern i32 EdType_Int;
extern i32 EdType_Float;
extern i32 EdType_VuVec;
extern i32 EdType_VuMtx;
extern i32 EdType_Enumeration;
extern i32 EdType_String;
extern i32 EdType_Colour3;
extern i32 EdType_NuHSpecial;
extern i32 EdType_NuVec;
extern i32 EdType_NuMtx;

void SerialiseChar(EdStream &, void *, i32);
void SerialiseShort(EdStream &, void *, i32);
void SerialiseInt(EdStream &, void *, i32);
void SerialiseFloat(EdStream &, void *, i32);
void SerialiseVuVec(EdStream &, void *, i32);
void SerialiseVuMtx(EdStream &, void *, i32);
void SerialiseString(EdStream &, void *, i32);
void SerialiseColour3(EdStream &, void *, i32);
void SerialiseNuHSpecial(EdStream &, void *, i32);
void SerialiseNuVec(EdStream &, void *, i32);
void SerialiseNuMtx(EdStream &, void *, i32);

extern f32 Scale;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern EdSystem theEdSystem;




void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float);
void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32);
void EdDrawMtx(VuMtx const *);

extern "C" {
extern char edgra_filter_string[16];
extern i32 edgra_mode;
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern i32 edgra_copy_source;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, u16, u8);
    void DebrisSetRoomID(i32, NUGSCN *);
    void DebrisSetDetailLevels(i32, i32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    i32 AddPARTEffect(i32, NUVEC *);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_usememcard;
    extern i32 edpp_copy_mode, edpp_copyrotz, edpp_copyroty, edpp_copy_source_count, edpp_copy_enclosed;
    extern i32 edpp_readout, edpp_num_orphans;
    extern i32 globalframes;
    extern f32 edpp_copy_size;
    extern NUMTL *edpp_mtl, *edpp_boxmtl;
    void edppDrawSpheres(debinftype *, i32);
    void edppDrawTorus(debinftype *, i32);
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
extern i32 edbits_particle_level_page;
}


i32 edpartLookupObjectInScene(char *, NUGSCN *);





void edppDetermineNearest(f32);
i32 edppPtlCreate(NUVEC *, i32);
i32 edppPtlPlace(i32, NUVEC *);




i32 edppPtlPlace(i32 index, NUVEC *position);


void edanimDoInput(nupad_s *pad);


i32 edppPtlCreate(NUVEC *position, i32 effect_index);



void EdDrawLineCube(VuMtx const &transform, float size, i32 colour);


i32 edanimFileSave(char *path);

void edpartInitType(i32 index);


extern "C" {
    void DebFreeInstantly(i32 *);
}


void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour);

void EdDrawLineCross(VuVec const &position, float size, i32 colour);

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset);





void edpartScaleType(i32 index, float scale);




void edanimDrawCursor();

extern "C" i32 part_emits_used;







void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour);

i32 edanimParamCreate(i32 instance_id);

i32 edpartSaveEffects(char *filename, char page);



void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end);

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset);

void edanimParamDestroy(i32 index);






i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end);


NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name);


void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();




void edanimParticleCreate(NUVEC *position);


void edppDetermineNearest(float max_distance_squared);



void edbriDetermineNearest(float distance);










void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);


void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);


void edanimDetermineNearestParticle(float distance);


void EdDrawMtx(VuMtx const *matrix);

















































extern EdEnumControl::Item OpenClosedItems[3];
extern EdEnumControl::Item OnOffItems[3];
extern EdEnumControl::Item YesNoItems[3];











extern i32 AxisColour[8];

































inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}








inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}






extern f32 MouseScale;


template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}


inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}








inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}





























































inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}



























inline EdSubSystem::~EdSubSystem() {
}






inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}















// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"


// Animation editor subsystem stubs (static, internal linkage).


// Remaining animation-editor UI/menu callbacks.















































// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>


// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")








extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----




// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}


// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}



extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}



extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}

// Particle-list editor UI/menu callbacks.





























// Particle editor UI/menu callbacks.










































































































// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----





// Particle list editor subsystem stubs (static, internal linkage).



// ---- end src/gameapi/edtools/edptlall.cpp ----



extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

















// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.



// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.


#include "gameapi/edtools/edtools_types.h"


extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}












extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}













extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern f32 edanim_cam_dist;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}


extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}








extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}






extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);


static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}




// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"


extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern char **TTab;
extern i16 tCANCEL;
extern i16 tLOCKED;
extern i16 tPOWERBRICK;
extern "C" OPTIONSSAVE TempOptions;
extern i32 menu_flash;
extern f32 minikittime;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);

extern i32 memcard_saveneeded;
extern i32 memcard_loadneeded;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);



extern FadeSystem FadeSys;




void APIMenuDrawMemCardSlots(MENU *menu, f32 y);






















void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);



















NUGSCN *IconScene_FindById(i32 character_id);




























void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);


























extern "C" {

extern f32 MENUDY;
extern f32 MENUBOTY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern f32 MENUENTRYEXWIDTH;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;
extern void (*headerdrawfn)();
extern void (*predrawfn)(MENU *);


void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void RemapAddr(void *new_base, void *old_base, void **address);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"


struct ani3_animheader_s;
struct nuanimdatachunk_s;

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);


extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>


// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"


extern i32 players_going_forward;

extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern SPEEDERCHASEVALUES speedervals[4];
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 objopponent_ignoreaiopponent;

void PodKeyReset();


void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);



extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);

i32 ObjIsTargetSpeeder(GameObject_s *object);

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));

void SpeederChaseA_Init(WORLDINFO_s *world);

void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speedup);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);


i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);




extern "C" {

extern i32 edmain_auto_speed;

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

} // extern "C"

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);


extern "C" f32 pod_roll[2];
f32 getPodRoll(i32 index);

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern "C" void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
i32 GetMenuID(void);
extern AREADATA_s *PODSPRINT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);


void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();






void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing);


extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern NUMTX cutscenecammtx, CutCamMtx;
f32 getPodRoll(i32);
extern "C" void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);





NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();




void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);


GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);


void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);




extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"





void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);



i32 EdFileOpen(char *filepath, NUFILEMODE mode);


i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);

extern char FS_LastFileName[64];
extern char FS_Path[256];
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);




void SerialiseInt(EdStream &stream, void *data, i32);


void SerialiseChar(EdStream &stream, void *data, i32);



void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);


void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);





// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();






void SerialiseNuHSpecial(EdStream &stream, void *data, i32);


void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);


i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>


extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"


extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODETITLEY;

void DrawGameObjectsDraw(i32 pass);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern f32 cointotaltime;
extern i32 noscenespecials;
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state

// Graphics loading flags

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).


// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"






extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}



i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];








#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)



extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);



void DrawPauseFade();
















i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);



void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);




void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);



i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);


i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];


extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);







void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);



extern i32 dco_locatorposonly;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}




void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


extern void DrawMenuEntry(MENU_s *, char *);






#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);





void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);







extern void (*BackDrop_AlphaFn)(float *);





void BackDrop_ResetColours();



extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== functions in original address order ====

extern "C" {








i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);


}


extern "C" {
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);


void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);




}



extern "C" {


}






extern "C" {

























void edmainSetReturn(i32 result);



ed_module_s *edmainCurrent(void);

i32 edmainActivate(ed_module_s *module, i32 notify);

void edmainSetCamera(NUMTX *matrix);

NUCAMERA *edmainGetCamera(void);

void edmainExtCamera(NUCAMERA *camera);

void edmainSetCursorEnabled(i32 enabled);

i32 edmainGetCursorEnabled(void);

eduimenu_s *edGetMainMenu(void);

void edmainRegisterLocVec(NUVEC *position);

NUVEC *edmainQueryLocVec(void);

void eduiSetGlobalSliderAccel(f32 acceleration);

void eduiInitMaterials(void);

void eduiSetRenderPlane(i8 plane);

eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);

void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);

i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);

i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuSortItemsByTxt(eduimenu_s *menu);

void eduiMenuSelectFirstEntry(eduimenu_s *menu);

void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);

f32 eduiGetAnalougePadValue(nupad_s *pad);

eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);

void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);

void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);


i32 eduiItemSetText(eduiitem_s *item, char *text);

}



extern "C" {
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);

eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);

eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);

void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);

eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);

eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);

void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);

void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);


i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);

eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);

void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);

void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);

}


extern "C" {
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

i32 eduicbInteractSlider(edui_interact_s *interact);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

void eduiSetCameraEnabled(i32 enabled);

i32 NuKeyToAscii(u32 key, i32 shifted);



eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);

eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);

eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);

f32 edmainSetMainMenuScale(f32 scale);

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

void eduiMenuDestroy(eduimenu_s *menu);

void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);


void edmainClose(void);

i32 edmainRegister(ed_module_s *module);

void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);

void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);

eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);

void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);


eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);

i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);

void eduiSetUsingMenuFocus(i32 enabled);

i32 eduiGetUsingMenuFocus(void);

void eduiSetDefaultActiveMenu(eduimenu_s *menu);

void eduiSetActiveMenu(eduimenu_s *menu);

i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);


void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);




edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);

void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);

void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);



eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);



i32 eduiMenuDetach(eduimenu_s *menu);

}


extern "C" {
void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);









__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);



eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);

eduimenu_s *eduiGetActiveMenuParent(void);

i32 eduiClearActiveMenu(void);

i32 eduiMenuIsActive(eduimenu_s *menu);

i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiMenuEnsureSelection(eduimenu_s *menu);

i32 eduiCursorOverMenu(eduimenu_s *menu);

i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);

void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);

void cbInteractMenuKeySelect(eduimenu_s *menu);

i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);

void eduiFlushInteracts(void);

i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);

i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiRenderInteracts(void);

void eduiShowCursor(i32 show);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiSetCursorColour(u32 colour);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void eduiSetFontScale(f32 x, f32 y);

void CreateTestMenu(void *font);

void TestMenu(nupad_s *pad);

eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);


void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}


extern "C" {
void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}


extern "C" {
void edcamSetDist(f32 distance);

f32 edcamGetDist(void);

edcam_s *edcamGetEdCam(void);

void edcamSetOffset(NUVEC *offset);

void edcamGetOffset(NUVEC *offset);

void edcamMtx(NUMTX *matrix);

void edcamSet(void);

NUVEC *edcamGetPosPointer(void);

}








































extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}












extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}


extern "C" {
void FS_SetFileSelPathFromName(char *name);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}


extern "C" {
void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

}


extern "C" {
extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" void CreateColourPicker(void);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

void FlushMenuHighlights(eduimenu_s *menu);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"


#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"



extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);


extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"




// ==== functions in original address order ====






































































































































extern "C" {
void PartTerrInit(void);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void edpartRegisterPointerToGameCharLocation(NUVEC *position);

}




















extern "C" {
void edpartSetParticlePage(i32 page);

}


extern "C" {
void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

void edpartParticleReset(void);

void NuPartEnableRayCasts(i32 enabled);

void NuPartSetSeed(i32 seed);

u32 NuPartGetSeed(void);

void NuPartResetGlobalTime(void);

void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void NewPartRotation(PART_s *part);

}



extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void KillPart(PART_s *part, i32 reason);

void DrawParts(i32 keep_offscreen);

PART_s *AddPart(ADDPART_s *params);

void KillAllParts(void);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

void PARTStartOffset(i32 index, f32 offset);

i32 PARTLookupType(char *name);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

}


extern "C" {
i32 PARTLookupTypePageOnly(char *name, i32 page);

i32 GetPartCount(void);

i32 GetMaxPartTypes(void);

char *GetPartName(i32 index);

}



extern "C" {
void CheckPartCount(void);

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);

}





extern "C" {
void edpartDestroyAllParticles(void);

}


extern "C" {
f32 PARTGetTotalOnTime(i32 index);

f32 PARTGetTotalOffTime(i32 index);

i32 AddPARTEffect(i32 effect, NUVEC *position);

}




extern "C" {
void RemovePARTEffect(i32 index);

}


extern "C" {
void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

void UpdateParts(f32 time);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

void KillPartsByScene(NUGSCN *scene);

void edpartClearPage(i8 page);

}



// ==== src/legoapi/gizmo/gizmos/gizmos_gizactions.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/fx/parts.h"



i32 Action_CameraCut(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                     i32 first_time, f32);



i32 Action_EndCameraCut(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params, i32 param_count,
                        i32 first_time, float);


i32 Action_PlayCutScene(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params, i32 param_count, i32 first_time,
                        float);



i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float);



i32 Action_BoulderSection(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                          i32 param_count, i32 first_time, f32);


void GameCameraMakeMiniCut3(u32, float, i32, i32, i32, void *, i32, NUVEC *, float, float, float, float, float, float,
                            float, i32, nugspline_s *, char, char);


extern f32 drop_back_in_timer;


struct GIZSPINNER_s;
void ClearSpecialMove(GameObject_s *);
extern u32 GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_TOGGLERIGHT;
extern f32 ai_moveradius;

i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32,
                            f32 elapsed);




// ==== functions in original address order ====

static void VaderA_StartCollapseStage(WORLDINFO_s *world) {
    i32 path_index;
    nuhspecial_s specials[100];
    u32 *path_connection = static_cast<u32 *>(
        AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "Block1_a", "Block1_b", &path_index));

    if (path_connection != NULL) {
        path_connection[path_index] |= 0x80000000;
    }

    vader_a.forces[0] = GizForces_FindForce(world, "force1");
    vader_a.forces[1] = GizForces_FindForce(world, "force2");
    vader_a.forces[2] = GizForces_FindForce(world, "force3");
    vader_a.forces[3] = GizForces_FindForce(world, "force4");
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

void Boulder_Kill(PART_s *part, i32) {
    i32 i;
    if (boulder_part[0] == part)
        i = 0;
    else if (__builtin_expect(boulder_part[1] == part, 0))
        i = 1;
    else
        return;
    if (part == NULL)
        return;
    if (boulder_blowup_type != -1)
        GizmoBlowUpTypeBlowUp(WORLD, boulder_blowup_type, &part->position);
    NuSpecialSetVisibility(&LevHSpecial[i], 0);
    LevInstAnim[i]->ltime = 0.0f;
    LevInstAnim[i]->playing = 0;
    boulder_part[i] = NULL;
}

void Boulder_Move(PART_s *part, float) {
    if (netclient != 0)
        return;
    i32 i;
    if (part == boulder_part[0])
        i = 0;
    else if (part == boulder_part[1])
        i = 1;
    else {
        KillPart(part, 0);
        return;
    }
    boulder_oldpos[i] = part->position;
    part->transform = *NuSpecialGetDrawMtx(&LevHSpecial[i]);
    if (!(1.0f >= part->scale_time)) {
        NuVecSub(&part->velocity, &part->position, &boulder_oldpos[i]);
        NuVecScale(&part->velocity, &part->velocity, 1.0f / FRAMETIME);
    } else {
        part->velocity = v000;
    }
    if (LevInstAnim[i]->ltime > 1.0f && !LevInstAnim[i]->playing) {
        KillPart(part, 0);
        return;
    }
    if (part->active & 1)
        PlaySfx(const_cast<char *>("Kas_BoulderLoop"), &part->position);
}

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
        auto world_area = world->area;
        for (i32 i = 0; i < world_area->level_count; i++) {
            if (static_cast<u16>(world_area->levels[i]) == static_cast<u16>(CRUISERE_LDATA->idx))
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
        if ((*(u8 *)((u8 *)LevelProgressData + CruiserE_ix * 0x2e24 + 0x2800) & 1) != 0) {
            if (CruiserD_direction < 0) {
                CruiserD_LiftAnim->ltime = CruiserD_frame;
                CruiserD_LiftAnim->playing = 1;
                CruiserD_LiftAnim->tfactor = -0.1f;
            } else {
                f32 end_frame = *(f32 *)CruiserD_Lift.scene->instance_animation_data[CruiserD_LiftAnim->anim_ix];
                CruiserD_LiftAnim->playing = 0;
                CruiserD_LiftAnim->tfactor = 0.1f;
                CruiserD_LiftAnim->ltime = end_frame;
            }
        } else {
            CruiserD_frame = 1.0f;
            CruiserD_direction = 1;
            CruiserD_LiftAnim->playing = 1;
            CruiserD_LiftAnim->ltime = 1.0f;
            CruiserD_LiftAnim->tfactor = 0.1f;
        }
    }

    CruiserD_LiftChase = 0;
}

void CruiserDUpdate(WORLDINFO_s *) {
    CruiserD_LiftChase = 0;
    if (!NuSpecialExistsFn(&CruiserD_Lift) || CruiserD_LiftAnim == NULL || CruiserD_LiftChase_msg == NULL)
        return;

    if (netclient == 0) {
        CruiserD_LiftChase_msg->value = 0.0f;
        if (CruiserD_direction >= 0 && LevGizmo[0] != NULL && LevGizmo[0]->object != NULL &&
            (((u8 *)LevGizmo[0]->object)[0x68] & 2) != 0) {
            CruiserD_direction = -1;
            CruiserD_LiftAnim->tfactor = -0.1f;
            CruiserD_LiftAnim->playing = 1;
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
    }

    if (CruiserD_LiftAnim->playing)
        PlaySfx("Cru_HugeWallMoveLp", NuSpecialGetDrawPos(&CruiserD_Lift));

    CruiserD_frame = CruiserD_LiftAnim->ltime;
    if (netclient != 0) {
        CruiserD_LiftAnim->ltime = cruiserd_netpacket->frame;
        CruiserD_LiftAnim->tfactor = cruiserd_netpacket->speed;
        CruiserD_LiftAnim->playing = (cruiserd_netpacket->flags & 1) != 0;
    }
}

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

void KashyyykA_Reset(WORLDINFO_s *) {
    KashyyykA_msg_TotalWookies = CheckGizAIMessage(gizaimessagesys, "TotalWookies", NULL);
    KashyyykA_msg_WookiesToRescue = CheckGizAIMessage(gizaimessagesys, "WookiesToRescue", NULL);
}

void KashyyykA_Update(WORLDINFO_s *) {
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

void KashyyykB_Init(WORLDINFO_s *) {
}

void KashyyykB_Reset(WORLDINFO_s *) {
}

void KashyyykB_Update(WORLDINFO_s *) {
}

void KashyyykC_Init(WORLDINFO_s *world) {
    GIZFORCE_s *f = GizForces_FindForce(world, "force16");
    if (f != NULL) {
        if (f->force_strength == 3.0f)
            f->force_strength = 20.0f;
        f->strength_0x6c = 1.0f;
    }
}

void KashyyykC_Update(WORLDINFO_s *) {
}

void KashyyykD_Init(WORLDINFO_s *) {
    i_boulder = 0;
    BoulderWait = 0.0f;
    boulder_blowup_type = GizmoBlowupGetTypeFromNameTableId(WORLD, GizmoBlowupGetNameTableId("ball_blowup_null"));
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

void KashyyykD_Update(WORLDINFO_s *) {
    if (netclient != 0)
        return;
    if (BoulderWait <= 0.0f) {
        i32 i = i_boulder;
        if (boulder_part[i] != NULL)
            return;
        nuhspecial_s *special;
        if (LevInstAnim[i] != NULL && (special = &LevHSpecial[i])->display_special != NULL) {
            ADDPART_s params = Default_ADDPART;
            params.matrix = &numtx_identity;
            params.velocity = &v000;
            params.owner = NULL;
            params.field_14 = 0.3f;
            params.field_18 = 0.3f;
            params.gravity = 0.0f;
            params.special = special;
            params.flags = 0x80861a;
            params.move_fn = Boulder_Move;
            params.field_40 = PartCollide_3D;
            params.field_44 = Boulder_Kill;
            params.time_step = FRAMETIME;
            PART_s *part = AddPart(&params);
            if (part != NULL)
                boulder_part[i] = part;
            LevInstAnim[i]->ltime = 1.0f;
            LevInstAnim[i]->playing = 1;
            LevInstAnim[i]->mtx = numtx_identity;
            NuSpecialSetVisibility(special, 1);
        }
        ++i_boulder;
        if (i_boulder == 2)
            i_boulder = 0;
        BoulderWait = 8.0f;
    } else {
        BoulderWait -= FRAMETIME;
    }
}

i32 Action_BoulderSection(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                          i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time != 0) {
        processor->action_data_4 = 1.0f;
        processor->action_data_5 = 1.0f;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "boulder_range=");
            if (value != NULL) {
                processor->action_data_4 = AIParamToFloat(&packet->script_process, value + 14);
            } else if ((value = NuStrIStr(params[index], "attack_time=")) != NULL) {
                processor->action_data_5 = AIParamToFloat(&packet->script_process, value + 12);
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, params[index]);
            }
        }
    }
    const bool can_attack = FreePlay == 0 || CharCategory_IsCategory(object, 0) != 0 ||
                            CharCategory_IsCategory(object, 1) != 0 || CharCategory_IsCategory(object, 8) != 0;
    if (!can_attack) {
        processor->action_timer -= FRAMETIME;
        if (processor->action_timer < 0.0f) {
            processor->action_timer = 0.5f;
            object->pad_gamepad->buttons_pressed |= GAMEPAD_TOGGLELEFT;
        }
    }
    object->field_0xef8 |= GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
    PART_s *nearest = NULL;
    f32 nearest_distance = processor->action_data_4 * processor->action_data_4;
    NUVEC difference;
    if (boulder_part[0] != NULL) {
        const f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &boulder_part[0]->position, &difference);
        if (distance < nearest_distance) {
            nearest = boulder_part[0];
            nearest_distance = distance;
        }
    }
    if (boulder_part[1] != NULL) {
        const f32 distance = NuVecDistSqr(&object->apiobj.collision_position, &boulder_part[1]->position, &difference);
        if (distance < nearest_distance)
            nearest = boulder_part[1];
    }
    // The retail third probe reads padding past this two-entry array. Do not
    // reproduce that out-of-bounds access or enlarge the recovered global.
    if (nearest != NULL) {
        packet->movement_look_target = &nearest->position;
        if (can_attack)
            object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
    } else {
        if (system != NULL && system->player_1 != NULL && system->player_1->ai != NULL) {
            AIPACKET_s *player_packet = system->player_1->ai;
            AIMoveInstruction(packet, &player_packet->last_path_position, player_packet->mover_height,
                              &player_packet->path_info, 1, packet->movement_instruction_parameter);
        }
    }
    return 0;
}

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
            if (Player[0] != NULL && Player[0]->apiobj.player_controlled)
                LoseCoins(Player[0], 1);
            if (Player[1] != NULL && Player[1]->apiobj.player_controlled)
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
        packet->timer = vader_a.timer;
        packet->subtitle = vader_a.subtitle;
    } else {
        vader_a.count = packet->count;
        vader_a.timer = packet->timer;
        vader_a.subtitle = packet->subtitle;
    }
}

void VaderA_DrawPanel(WORLDINFO_s *) {
    if (__builtin_expect(vader_a.count <= 2, 0)) {
        if (vader_a.count != 0) {
            DrawTimer((i32)vader_a.timer + 1, vader_a.subtitle, 0);
            vader_a.subtitle = 0;
        }
    }
}

void VaderA_GoneThroughDoor(WORLDINFO_s *world, DOOR_s *door) {
    if (netclient == 0 && vader_a.count == 0)
        VaderA_StartCollapseStage(world);
    if (door != NULL && NuStrICmp(door->name, "door_control") == 0)
        door->active = 1;
}

void VaderB_Init(WORLDINFO_s *) {
}

void VaderB_Reset(WORLDINFO_s *) {
    vader_b_complete_msg = SetGizAIMessage(gizaimessagesys, "VaderBComplete", 0.0f, NULL);
    vader_b_playersDead = 0;
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

void VaderB_DrawPanel(WORLDINFO_s *) {
}

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

void VaderC_Reset(WORLDINFO_s *) {
    vader_c.field_0x94 = 0;
    vader_c.field_0x95 = 0;
}

void VaderC_Update(WORLDINFO_s *world) {
    extern TERRSET *CurTerr;
    extern i32 obstacle_gizmotype_id;

    GIZAIMESSAGE_s *final_fight = vader_c.final_fight_message;
    if (netclient == 0 && final_fight != NULL && ChallengeMode == 0 && final_fight->value == 0.0f &&
        vader_c.big_jump_locator != NULL && player != NULL && player->apiobj.supporting_platform_id != -1) {
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
            final_fight->value = 1.0f;
            DOOR_s *door = Door_FindByName(world, "door_fight");
            if (door != NULL)
                Door_GoThrough(world, door, 1);
        }
    }

    if (__builtin_expect(netclient != 0 && GameTimer.time_elapsed < 5.0f, 0)) {
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
        GameObject_s *second_player = Player[1];
        GameObject_s *first_player = Player[0];
        if (((first_player != NULL && (first_player->apiobj.field_0x1f8 & 0x80) != 0 &&
              first_player->apiobj.field_0x287 != 0 && (first_player->apiobj.field_0x1f4 & 0x40000) == 0) ||
             (second_player != NULL && (second_player->apiobj.field_0x1f8 & 0x80) != 0 &&
              second_player->apiobj.field_0x287 != 0 && (second_player->apiobj.field_0x1f4 & 0x40000) == 0)) &&
            (static_cast<u8 *>(vaderc_netpacket)[0] == 0 || first_player == NULL ||
             (first_player->apiobj.field_0x1f8 & 0x80) == 0 || second_player == NULL ||
             (second_player->apiobj.field_0x1f8 & 0x80) == 0) &&
            (ChallengeMode == 0 || AreaGlobals.values.field_0x1c <= 9)) {
            vader_c.field_0x95 = 1;
            if (__builtin_expect(vader_c.final_fight_message->value == 0.0f, 1))
                ResetLevel(NULL, NULL, 1);
        }
    }

    if (LevGizObst[0] != NULL && LevGizObst[0]->anim_set != NULL && LevGizObst[0]->anim_set->objects != NULL &&
        LevGizObst[0]->anim_set->objects->instance_animation != NULL) {
        GAMEANIMOBJ_s *object = LevGizObst[0]->anim_set->objects;
        nuinstanim_s *anim = object->instance_animation;
        if (object->end_frame > 200.0f) {
            f32 factor = 1.0f;
            if (anim->ltime >= 200.0f) {
                factor = (object->end_frame - anim->ltime) / (object->end_frame - 200.0f);
                factor *= factor;
            }
            if (anim->fparam1 == 0.0f)
                anim->tfactor = factor;
            else
                anim->tfactor = factor * anim->fparam1;
        }
    }
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

