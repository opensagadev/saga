#include "decomp.h"
#include "gameapi_edtools_types.h"

// Particle-list editor UI/menu callback stubs (static, internal linkage).

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

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *) {
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

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeX(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeY(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeZ(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlColMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlJibMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlRotMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCollMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDataMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelGCode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelGSort(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSizeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlGSortMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDamageMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSScaleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbSelEffectList(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbEffectListMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTextureMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlInstanceSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCancel(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
