#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"

extern "C" {
    i32 edpart_set_part = 5;
}

// Particle editor subsystem stubs (static, internal linkage).

static void edpartInit() {
    STUBBED();
}
static void edpartProc(float, nupad_s *) {
    STUBBED();
}
static void edpartApply() {
    STUBBED();
}
static void edpartClose() {
    STUBBED();
}
static void edpartEnter() {
    STUBBED();
}
static void edpartRender() {
    STUBBED();
}
static void edpartSelType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartCopyType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartDataMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartGravMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartMoveList(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartTintMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeName(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartCutOffMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSScaleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetSoundID(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSoundXMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSoundsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSwitchMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartEmitVelMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartVarEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartVarStartMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartDieDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartScaleTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetSwitchType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartChangeNameMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartImpactPartMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL && edpart_set_part == 5) {
        edpart_nearest_type->impact_part = item->data;
    }
}
static void edpartDebrisScaleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartSetSoundControl(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartImpactDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartTrail1DebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartTrail2DebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeMaxLifeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartEmitterDebrisMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceFlagsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceScaleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartDebrisSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceOrientMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartInstanceSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
