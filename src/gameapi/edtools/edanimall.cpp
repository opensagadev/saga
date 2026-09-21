#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"

// Animation editor subsystem stubs (static, internal linkage).

static void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbFileLoad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbFileSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
void edanimRegisterBaseScene(NUGSCN *scene) {
    (void)scene;
}
static void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_010 = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edanimcbMCTBCardType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSetSoundType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_014 = static_cast<edui_slider_s *>(item)->value;
}
static void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edanimcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbMCTBCardFormat(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32) {
    AnimParams[edanim_nearest_param_id].sound_values[edanim_nearest_sound] = static_cast<edui_slider_s *>(item)->value;
}
static void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_018 = static_cast<edui_slider_s *>(item)->value;
}
static void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

// Remaining animation-editor UI/menu callbacks.

static void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbMCTBCardPresent(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetParticleType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbToggleSoundType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbMCTBCardLoadSlot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardSaveSlot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardUnFormat(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardFreeSpace(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardSlotsUsed(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetLocalSoundType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardDeleteSlot(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbMCTBCardCheckFormat(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbMCTBCardCheckKeyCard(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbMCTBCardWriteKeyCard(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbSetLocalParticleType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
