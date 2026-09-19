#include "decomp.h"
#include "gameapi_edtools_types.h"

// Particle editor UI/menu callback stubs (static, internal linkage).
// edpart* / edptl* / edptlcb* / edpp* symbols from edtools_other_B.txt.

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

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbSetSoundControl(eduimenu_s *, eduiitem_s *, u32) {
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

static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbSoundControlMenu(eduimenu_s *, eduiitem_s *, u32) {
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

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *, u32) {
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

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *, u32) {
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

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *) {
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

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *) {
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

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *) {
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

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
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
