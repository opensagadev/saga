#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

extern "C" i32 edSfxAllCount;

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32) {
    edanim_active_menu = NULL;
    edbitsStartCubemapDump();
}
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    char directory[256];
    char name[256];
    char extension[256];
    if (!edbits_level_save_directory[0])
        strcpy(directory, ".");
    else
        strcpy(directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        strcpy(name, "anims");
    else
        strcpy(name, edbits_level_save_name);
    if (!edbits_level_save_extension[0])
        strcpy(extension, "anm");
    else
        strcpy(extension, edbits_level_save_extension);
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    edanimParamReset();
    i32 page = -1;
    if (NuFileExists(path)) {
        page = edanimLoadPage(path, edbits_base_scene);
    }
    edanimStartAllPages();
    if (page < 0)
        eduiCreateMessageMenu(menu, const_cast<char *>("File Load Error"), 0);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("Loaded OK"), 1);
}
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    char directory[256];
    char name[256];
    char extension[256];
    if (!edbits_level_save_directory[0])
        strcpy(directory, ".");
    else
        strcpy(directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        strcpy(name, "anims");
    else
        strcpy(name, edbits_level_save_name);
    if (!edbits_level_save_extension[0])
        strcpy(extension, "anm");
    else
        strcpy(extension, edbits_level_save_extension);
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    if (edanimFileSave(path))
        eduiCreateMessageMenu(menu, const_cast<char *>("Saved OK"), 1);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("File Save Error"), 0);
}
void edanimRegisterBaseScene(NUGSCN *scene) {
    (void)scene;
}
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_010 = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadCheckCardType())
        eduiCreateMessageMenu(menu, const_cast<char *>("PS2 Card"), 1);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("Not a PS2 Card"), 0);
}
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_soundtype_menu = NULL;
    edanim_sound_type = item->data == 0x1869f ? -1 : item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_014 = static_cast<edui_slider_s *>(item)->value;
}
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edanim_active_menu = NULL;
}
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_switchtype_menu = NULL;
    AnimParams[edanim_nearest_param_id].field_00c = item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_mctb_menu);
    edanim_mctb_menu = NULL;
}
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadFormatCard()) {
        eduiCreateMessageMenu(menu, const_cast<char *>("Format OK"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("Format Fail"), 0);
    }
}
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32) {
    AnimParams[edanim_nearest_param_id].sound_values[edanim_nearest_sound] = static_cast<edui_slider_s *>(item)->value;
}
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }
    AnimParams[edanim_nearest_param_id].field_018 = static_cast<edui_slider_s *>(item)->value;
}
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_sound_menu);
    edanim_sound_menu = NULL;
}

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadCheckCardPresent()) {
        eduiCreateMessageMenu(menu, const_cast<char *>("Present"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("Not Present"), 0);
    }
}

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32) {
    AnimParams[edanim_nearest_param_id].effect_intervals[edanim_nearest_particle] =
        static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_particletype_menu = NULL;
    edanim_particle_type = item->data == 0 ? -1 : item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_soundtype_menu = NULL;
    if (!(item->highlighted & 1)) {
        AnimParams[edanim_nearest_param_id].sound_values[edanim_nearest_sound] = 1.0f;
        AnimParams[edanim_nearest_param_id].sound_flags[edanim_nearest_sound] = 0;
    } else {
        AnimParams[edanim_nearest_param_id].sound_values[edanim_nearest_sound] = 50.0f;
        AnimParams[edanim_nearest_param_id].sound_flags[edanim_nearest_sound] = 1;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_bouncy_menu);
    edanim_bouncy_menu = NULL;
}

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_switch_menu);
    edanim_switch_menu = NULL;
}

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32) {
    char data[32];
    if (saveloadLoadSlot(item->data, data, sizeof(data))) {
        eduiCreateMessageMenu(menu, data, 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("Load Error"), 0);
    }
}

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32) {
    char data[32];
    sprintf(data, "This is slot %d", item->data);
    if (saveloadSaveSlot(item->data, data, sizeof(data)))
        eduiCreateMessageMenu(menu, const_cast<char *>("Saved OK"), 1);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("Save Error"), 0);
}

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadUnFormatCard()) {
        eduiCreateMessageMenu(menu, const_cast<char *>("Unformat OK"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("Unformat Fail"), 0);
    }
}

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1) {
        return;
    }
    auto &param = AnimParams[edanim_nearest_param_id];
    param.bounce_damping = static_cast<edui_slider_s *>(item)->value;
    if (param.platform_id != -1) {
        PlatInstBounce(param.platform_id, param.bounce_impulse, param.bounce_spring, param.bounce_damping);
    }
}

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1) {
        return;
    }
    auto &param = AnimParams[edanim_nearest_param_id];
    param.bounce_spring = static_cast<edui_slider_s *>(item)->value;
    if (param.platform_id != -1) {
        PlatInstBounce(param.platform_id, param.bounce_impulse, param.bounce_spring, param.bounce_damping);
    }
}

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32) {
    char message[36];
    sprintf(message, "Space = %05d", saveloadCheckCardFreeSpace(0));
    eduiCreateMessageMenu(menu, message, 1);
}

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32) {
    char message[36];
    sprintf(message, "Slots Used = %02d", saveloadCheckSlotsUsed());
    eduiCreateMessageMenu(menu, message, 1);
}

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_localsoundtype_menu = NULL;
    auto &param = AnimParams[edanim_nearest_param_id];
    param.sound_ids[edanim_nearest_sound] = item->data;
    strcpy(param.sound_names[edanim_nearest_sound], edbitsGetSoundName(item->data));
    edanim_sound_type = item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_particle_menu);
    edanim_particle_menu = NULL;
}

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (saveloadDeleteSlot(item->data))
        eduiCreateMessageMenu(menu, const_cast<char *>("Delete OK"), 1);
    else
        eduiCreateMessageMenu(menu, const_cast<char *>("Delete Error"), 0);
}

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_soundtype_menu);
    edanim_soundtype_menu = NULL;
}

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadCheckCardFormatted()) {
        eduiCreateMessageMenu(menu, const_cast<char *>("Formatted"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("Not Formatted"), 0);
    }
}

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest == -1) {
        return;
    }
    auto &param = AnimParams[edanim_nearest_param_id];
    param.bounce_impulse = static_cast<edui_slider_s *>(item)->value;
    if (param.platform_id != -1) {
        PlatInstBounce(param.platform_id, param.bounce_impulse, param.bounce_spring, param.bounce_damping);
    }
}

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_localsound_menu);
    edanim_localsound_menu = NULL;
}

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_switchtype_menu);
    edanim_switchtype_menu = NULL;
}

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadCheckKeyCode(id_test, code_test)) {
        eduiCreateMessageMenu(menu, const_cast<char *>("KeyCard Check OK"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("KeyCard Check Fail"), 0);
    }
}

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32) {
    if (saveloadWriteKeyCode(id_test, code_test)) {
        eduiCreateMessageMenu(menu, const_cast<char *>("KeyCard Write OK"), 1);
    } else {
        eduiCreateMessageMenu(menu, const_cast<char *>("KeyCard Write Fail"), 0);
    }
}

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_localparticletype_menu = NULL;
    AnimParams[edanim_nearest_param_id].effect_ids[edanim_nearest_particle] = item->data;
    strcpy(AnimParams[edanim_nearest_param_id].effect_names[edanim_nearest_particle], debtab[item->data]->name);
    edanim_particle_type = item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32) {
    if (edanim_nearest_param_id == -1 || edanim_nearest_particle == -1)
        return;
    AnimParams[edanim_nearest_param_id].effect_flags[edanim_nearest_particle] = item->highlighted & 1;
}

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_particletype_menu);
    edanim_particletype_menu = NULL;
}

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_localparticle_menu);
    edanim_localparticle_menu = NULL;
}

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_localsoundtype_menu);
    edanim_localsoundtype_menu = NULL;
}

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_localparticletype_menu);
    edanim_localparticletype_menu = NULL;
}

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest == -1) {
        return;
    }

    edanim_bouncy_menu =
        eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelBouncyMenu, const_cast<char *>("Bounciness"));
    if (!edanim_bouncy_menu) {
        return;
    }

    eduiMenuAddItem(edanim_bouncy_menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyPlayerGrav, -0.1f, 0.2f,
                                                             AnimParams[edanim_nearest_param_id].bounce_impulse,
                                                             const_cast<char *>("Player Grav")));
    eduiMenuAddItem(edanim_bouncy_menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyTension, 0.0f, 1.0f,
                                                             AnimParams[edanim_nearest_param_id].bounce_spring,
                                                             const_cast<char *>("Tension")));
    eduiMenuAddItem(edanim_bouncy_menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyDamping, 0.0f, 1.0f,
                                                             AnimParams[edanim_nearest_param_id].bounce_damping,
                                                             const_cast<char *>("Damping")));

    eduiMenuAttach(parent, edanim_bouncy_menu);
    edanim_bouncy_menu->x = parent->x + 10;
    edanim_bouncy_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_param_id == -1) {
        return;
    }

    auto *menu =
        eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelSoundMenu, const_cast<char *>("Attached Sounds"));
    edanim_sound_menu = menu;
    if (!menu) {
        return;
    }

    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbSoundTypeMenu, const_cast<char *>("Sound Type...")));
    if (edanim_nearest_sound != -1) {
        eduiMenuAddItem(menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalSoundMenu,
                                                const_cast<char *>("Highlighted Snd Settings...")));
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_param_id == -1) {
        return;
    }

    auto *menu =
        eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelParticleMenu, const_cast<char *>("Attached Particles"));
    edanim_particle_menu = menu;
    if (!menu) {
        return;
    }

    eduiMenuAddItem(
        menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbParticleTypeMenu, const_cast<char *>("Particle Type...")));
    if (edanim_nearest_particle != -1) {
        eduiMenuAddItem(menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalParticleMenu,
                                                const_cast<char *>("Highlighted Ptl Settings...")));
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }

    edanim_switch_menu =
        eduiMenuCreate(70, 70, 180, 250, ed_fnt, edanimcbCancelSwitchMenu, const_cast<char *>("Switch Menu"));
    if (!edanim_switch_menu) {
        return;
    }

    eduiMenuAddItem(edanim_switch_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbSwitchTypeMenu, const_cast<char *>("Switch Type...")));
    eduiMenuAddItem(edanim_switch_menu, eduiItemSliderCreateInt(0, colours, 0, edanimcbSetSwitchId, -1, 129,
                                                                AnimParams[edanim_nearest_param_id].field_010,
                                                                const_cast<char *>("Switch ID")));
    eduiMenuAddItem(edanim_switch_menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetSwitchDelay, 0.0f, 20.0f,
                                                             AnimParams[edanim_nearest_param_id].field_018,
                                                             const_cast<char *>("Switch Delay")));
    eduiMenuAddItem(edanim_switch_menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetSwitchVar, 0.0f, 20.0f,
                                                             AnimParams[edanim_nearest_param_id].field_014,
                                                             const_cast<char *>("Switch Var")));

    eduiMenuAttach(parent, edanim_switch_menu);
    edanim_switch_menu->x = parent->x + 10;
    edanim_switch_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_particle == -1) {
        return;
    }

    edanim_localparticle_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelLocalParticleMenu,
                                               const_cast<char *>("Highlighted Particle Settings"));
    if (!edanim_localparticle_menu) {
        return;
    }

    eduiMenuAddItem(edanim_localparticle_menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalParticleTypeMenu,
                                                                 const_cast<char *>("Highlighted Particle Type...")));
    eduiMenuAddItem(
        edanim_localparticle_menu,
        eduiItemSliderCreateInt(0, colours, 0, edanimcbSetParticleRate, 0, 300,
                                AnimParams[edanim_nearest_param_id].effect_intervals[edanim_nearest_particle],
                                const_cast<char *>("Particles Per Sec")));
    eduiMenuAddItem(edanim_localparticle_menu,
                    eduiItemToggleCreate(0, colours,
                                         AnimParams[edanim_nearest_param_id].effect_flags[edanim_nearest_particle], 1,
                                         edanimcbToggleParticleSwitch, const_cast<char *>("Only On Moving")));

    eduiMenuAttach(parent, edanim_localparticle_menu);
    edanim_localparticle_menu->x = parent->x + 10;
    edanim_localparticle_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanim_soundtype_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edanimcbCancelSoundTypeMenu, const_cast<char *>("Sound Type"));
    if (!edanim_soundtype_menu) {
        return;
    }

    eduiMenuAddItem(edanim_soundtype_menu, eduiItemCheckCreate(0x1869f, colours, edanim_sound_type == -1, 0,
                                                               edanimcbSetSoundType, const_cast<char *>("NONE")));
    for (i32 index = 0; index < edSfxAllCount; ++index) {
        if (edanim_sound_type == index) {
            eduiMenuAddItem(edanim_soundtype_menu,
                            eduiItemCheckCreate(index, colours, 1, 1, edanimcbSetSoundType, edbitsGetSoundName(index)));
            edanim_soundtype_menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(edanim_soundtype_menu,
                            eduiItemCheckCreate(index, colours, 0, 1, edanimcbSetSoundType, edbitsGetSoundName(index)));
        }
    }

    eduiMenuAttach(parent, edanim_soundtype_menu);
    edanim_soundtype_menu->x = parent->x + 10;
    edanim_soundtype_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanim_localsoundtype_menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelLocalSoundTypeMenu,
                                                const_cast<char *>("Highlighted Sound Type"));
    if (!edanim_localsoundtype_menu) {
        return;
    }

    for (i32 index = 0; index < edSfxAllCount; ++index) {
        if (AnimParams[edanim_nearest_param_id].sound_ids[edanim_nearest_sound] == index) {
            eduiMenuAddItem(
                edanim_localsoundtype_menu,
                eduiItemCheckCreate(index, colours, 1, 1, edanimcbSetLocalSoundType, edbitsGetSoundName(index)));
            edanim_localsoundtype_menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(
                edanim_localsoundtype_menu,
                eduiItemCheckCreate(index, colours, 0, 1, edanimcbSetLocalSoundType, edbitsGetSoundName(index)));
        }
    }

    eduiMenuAttach(parent, edanim_localsoundtype_menu);
    edanim_localsoundtype_menu->x = parent->x + 10;
    edanim_localsoundtype_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanim_localparticletype_menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelLocalParticleTypeMenu,
                                                   const_cast<char *>("Highlighted Particle Type"));
    if (!edanim_localparticletype_menu) {
        return;
    }

    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (!debtab[index]) {
            continue;
        }
        if (AnimParams[edanim_nearest_param_id].effect_ids[edanim_nearest_particle] == index) {
            eduiMenuAddItem(
                edanim_localparticletype_menu,
                eduiItemCheckCreate(index, colours, 1, 1, edanimcbSetLocalParticleType, debtab[index]->name));
            edanim_localparticletype_menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(
                edanim_localparticletype_menu,
                eduiItemCheckCreate(index, colours, 0, 1, edanimcbSetLocalParticleType, debtab[index]->name));
        }
    }

    eduiMenuAttach(parent, edanim_localparticletype_menu);
    edanim_localparticletype_menu->x = parent->x + 10;
    edanim_localparticletype_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanim_particletype_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edanimcbCancelParticleTypeMenu, const_cast<char *>("Particle Type"));
    if (!edanim_particletype_menu) {
        return;
    }

    eduiMenuAddItem(edanim_particletype_menu, eduiItemCheckCreate(0, colours, edanim_particle_type == -1, 0,
                                                                  edanimcbSetParticleType, const_cast<char *>("NONE")));
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (!debtab[index]) {
            continue;
        }
        if (edanim_particle_type == index) {
            eduiMenuAddItem(edanim_particletype_menu,
                            eduiItemCheckCreate(index, colours, 1, 1, edanimcbSetParticleType, debtab[index]->name));
            edanim_particletype_menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(edanim_particletype_menu,
                            eduiItemCheckCreate(index, colours, 0, 1, edanimcbSetParticleType, debtab[index]->name));
        }
    }

    eduiMenuAttach(parent, edanim_particletype_menu);
    edanim_particletype_menu->x = parent->x + 10;
    edanim_particletype_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanim_switchtype_menu =
        eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelSwitchTypeMenu, const_cast<char *>("Switch Type"));
    if (!edanim_switchtype_menu) {
        return;
    }

    const auto add_type = [&](u32 type, const char *name) {
        const bool selected = AnimParams[edanim_nearest_param_id].field_00c == type;
        eduiMenuAddItem(edanim_switchtype_menu, eduiItemCheckCreate(type, colours, selected, 1, edanimcbSetSwitchType,
                                                                    const_cast<char *>(name)));
        if (edui_last_item->highlighted & 1) {
            edanim_switchtype_menu->selected = edui_last_item;
        }
    };
    add_type(0, "None");
    add_type(1, "Switch");
    add_type(2, "Switch One Cycle");
    add_type(3, "Switch Continuous");
    add_type(4, "Proximity");
    add_type(5, "Proximity One Cycle");
    add_type(6, "Proximity Continuous");
    add_type(7, "Terrain");
    add_type(8, "Terrain One Cycle");
    add_type(9, "Terrain Continuous");
    add_type(10, "Override NoAnim");
    add_type(11, "Override Play");
    add_type(12, "Override PlayCont");

    eduiMenuAttach(parent, edanim_switchtype_menu);
    edanim_switchtype_menu->x = parent->x + 10;
    edanim_switchtype_menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edanimcbCancelMCTBMenu, const_cast<char *>("Memory Card Test Menu"));
    edanim_mctb_menu = menu;
    if (!menu) {
        return;
    }

    const auto add_item = [&](u32 slot, EdUiItemCallback callback, const char *name) {
        eduiMenuAddItem(menu, eduiItemSelCreate(slot, colours, 0, 0, callback, const_cast<char *>(name)));
    };
    add_item(1, edanimcbMCTBCardPresent, "Card Present Test");
    add_item(1, edanimcbMCTBCardType, "Card Type Test");
    add_item(1, edanimcbMCTBCardCheckFormat, "Card Format Test");
    add_item(1, edanimcbMCTBCardFreeSpace, "Card Free Space");
    add_item(1, edanimcbMCTBCardSlotsUsed, "Card Slots Used");
    add_item(0, edanimcbMCTBCardSaveSlot, "Card Save Slot 0");
    add_item(0, edanimcbMCTBCardLoadSlot, "Card Load Slot 0");
    add_item(0, edanimcbMCTBCardDeleteSlot, "Card Delete Slot 0");
    add_item(1, edanimcbMCTBCardSaveSlot, "Card Save Slot 1");
    add_item(1, edanimcbMCTBCardLoadSlot, "Card Load Slot 1");
    add_item(1, edanimcbMCTBCardDeleteSlot, "Card Delete Slot 1");
    add_item(1, edanimcbMCTBCardFormat, "Format Card");
    add_item(1, edanimcbMCTBCardUnFormat, "Unformat Card");
    add_item(1, edanimcbMCTBCardWriteKeyCard, "Write KeyCard");
    add_item(1, edanimcbMCTBCardCheckKeyCard, "Check KeyCard");

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_sound == -1) {
        return;
    }
    edanim_localsound_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelLocalSoundMenu,
                                            const_cast<char *>("Highlighted Sound Settings"));
    if (!edanim_localsound_menu) {
        return;
    }

    auto &param = AnimParams[edanim_nearest_param_id];
    const i32 sound = edanim_nearest_sound;
    eduiMenuAddItem(edanim_localsound_menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalSoundTypeMenu,
                                                              const_cast<char *>("Highlighted Sound Type...")));
    const bool repeats = param.sound_flags[sound] == 1;
    eduiMenuAddItem(edanim_localsound_menu, eduiItemToggleCreate(1, colours, repeats, 1, edanimcbToggleSoundType,
                                                                 const_cast<char *>("Repeating Sound")));
    if (repeats) {
        eduiMenuAddItem(edanim_localsound_menu, eduiItemSliderCreateInt(0, colours, 0, edanimcbSetSoundTiming, 1, 99,
                                                                        static_cast<i32>(param.sound_values[sound]),
                                                                        const_cast<char *>("Repeat Every")));
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
        if (NuSpecialTestAnim(&special)) {
            const auto *legacy_specials = reinterpret_cast<const NuSpecialLegacyLayout *>(edbits_base_scene->specials);
            const auto *instance =
                static_cast<const NuLegacyInstanceLayout *>(legacy_specials[edanim_nearest].instance)->animation;
            const f32 end_frame =
                *reinterpret_cast<const f32 *>(edbits_base_scene->instance_animation_data[instance->anim_ix]);
            eduiMenuAddItem(edanim_localsound_menu,
                            eduiItemSliderCreate(0, colours, 0, edanimcbSetSoundTiming, 1.0f, end_frame,
                                                 param.sound_values[sound], const_cast<char *>("Sound Trigger Time")));
            eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), const_cast<char *>("(%1.01f)"));
            eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
        }
    }

    eduiMenuAttach(parent, edanim_localsound_menu);
    edanim_localsound_menu->x = parent->x + 10;
    edanim_localsound_menu->y = parent->y + 40;
}
