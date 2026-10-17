#include "decomp.h"
#include "gameapi_edtools_types.h"
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

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
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
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

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

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
