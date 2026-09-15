#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"

extern "C" {
    extern part_typedesc_s *edpart_nearest_type;
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
static void edppRender() {
    STUBBED();
}
