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
    strcpy(directory, edbits_level_save_directory[0] ? edbits_level_save_directory : ".");
    strcpy(name, edbits_level_save_name[0] ? edbits_level_save_name : "anims");
    strcpy(extension, edbits_level_save_extension[0] ? edbits_level_save_extension : "anm");
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    edanimParamReset();
    i32 page = -1;
    if (NuFileExists(path)) {
        page = edanimLoadPage(path, edbits_base_scene);
    }
    edanimStartAllPages();
    eduiCreateMessageMenu(menu, const_cast<char *>(page < 0 ? "File Load Error" : "Loaded OK"), page >= 0);
}
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32) {
    char path[256];
    char directory[256];
    char name[256];
    char extension[256];
    strcpy(directory, edbits_level_save_directory[0] ? edbits_level_save_directory : ".");
    strcpy(name, edbits_level_save_name[0] ? edbits_level_save_name : "anims");
    strcpy(extension, edbits_level_save_extension[0] ? edbits_level_save_extension : "anm");
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    const bool saved = edanimFileSave(path) != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(saved ? "Saved OK" : "File Save Error"), saved);
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
    const bool valid = saveloadCheckCardType() != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(valid ? "PS2 Card" : "Not a PS2 Card"), valid);
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
    const bool formatted = saveloadFormatCard() != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(formatted ? "Format OK" : "Format Fail"), formatted);
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
    const bool present = saveloadCheckCardPresent() != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(present ? "Present" : "Not Present"), present);
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
    auto &param = AnimParams[edanim_nearest_param_id];
    const bool repeating = (item->highlighted & 1) != 0;
    param.sound_values[edanim_nearest_sound] = repeating ? 50.0f : 1.0f;
    param.sound_flags[edanim_nearest_sound] = repeating ? 1 : 0;
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
    const bool saved = saveloadSaveSlot(item->data, data, sizeof(data)) != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(saved ? "Saved OK" : "Save Error"), saved);
}

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32) {
    const bool formatted = saveloadUnFormatCard() != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(formatted ? "Unformat OK" : "Unformat Fail"), formatted);
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
    const bool deleted = saveloadDeleteSlot(item->data) != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(deleted ? "Delete OK" : "Delete Error"), deleted);
}

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edanim_soundtype_menu);
    edanim_soundtype_menu = NULL;
}

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32) {
    const bool formatted = saveloadCheckCardFormatted() != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(formatted ? "Formatted" : "Not Formatted"), formatted);
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
    const bool valid = saveloadCheckKeyCode(id_test, code_test) != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(valid ? "KeyCard Check OK" : "KeyCard Check Fail"), valid);
}

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32) {
    const bool written = saveloadWriteKeyCode(id_test, code_test) != 0;
    eduiCreateMessageMenu(menu, const_cast<char *>(written ? "KeyCard Write OK" : "KeyCard Write Fail"), written);
}

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edanim_localparticletype_menu = NULL;
    auto &param = AnimParams[edanim_nearest_param_id];
    param.effect_ids[edanim_nearest_particle] = item->data;
    strcpy(param.effect_names[edanim_nearest_particle], debtab[item->data]->name);
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
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest == -1) {
        return;
    }

    auto *menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelBouncyMenu, const_cast<char *>("Bounciness"));
    edanim_bouncy_menu = menu;
    if (!menu) {
        return;
    }

    auto &param = AnimParams[edanim_nearest_param_id];
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyPlayerGrav, -0.1f, 0.2f,
                                               param.bounce_impulse, const_cast<char *>("Player Grav")));
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyTension, 0.0f, 1.0f, param.bounce_spring,
                                               const_cast<char *>("Tension")));
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetBouncyDamping, 0.0f, 1.0f,
                                               param.bounce_damping, const_cast<char *>("Damping")));

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
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
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
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
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest == -1 || edanim_nearest_param_id == -1) {
        return;
    }

    auto *menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edanimcbCancelSwitchMenu, const_cast<char *>("Switch Menu"));
    edanim_switch_menu = menu;
    if (!menu) {
        return;
    }

    auto &param = AnimParams[edanim_nearest_param_id];
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbSwitchTypeMenu, const_cast<char *>("Switch Type...")));
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(0, colours, 0, edanimcbSetSwitchId, -1, 129, param.field_010,
                                                  const_cast<char *>("Switch ID")));
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetSwitchDelay, 0.0f, 20.0f, param.field_018,
                                               const_cast<char *>("Switch Delay")));
    eduiMenuAddItem(menu, eduiItemSliderCreate(0, colours, 0, edanimcbSetSwitchVar, 0.0f, 20.0f, param.field_014,
                                               const_cast<char *>("Switch Var")));

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_particle == -1) {
        return;
    }

    auto *menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelLocalParticleMenu,
                                const_cast<char *>("Highlighted Particle Settings"));
    edanim_localparticle_menu = menu;
    if (!menu) {
        return;
    }

    auto &param = AnimParams[edanim_nearest_param_id];
    const auto particle = edanim_nearest_particle;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalParticleTypeMenu,
                                            const_cast<char *>("Highlighted Particle Type...")));
    eduiMenuAddItem(menu,
                    eduiItemSliderCreateInt(0, colours, 0, edanimcbSetParticleRate, 0, 300,
                                            param.effect_intervals[particle], const_cast<char *>("Particles Per Sec")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(0, colours, param.effect_flags[particle], 1,
                                               edanimcbToggleParticleSwitch, const_cast<char *>("Only On Moving")));

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edanimcbCancelSoundTypeMenu, const_cast<char *>("Sound Type"));
    edanim_soundtype_menu = menu;
    if (!menu) {
        return;
    }

    eduiMenuAddItem(menu, eduiItemCheckCreate(0x1869f, colours, edanim_sound_type == -1, 0, edanimcbSetSoundType,
                                              const_cast<char *>("NONE")));
    for (i32 index = 0; index < edSfxAllCount; ++index) {
        const bool selected = edanim_sound_type == index;
        eduiMenuAddItem(
            menu, eduiItemCheckCreate(index, colours, selected, 1, edanimcbSetSoundType, edbitsGetSoundName(index)));
        if (selected) {
            menu->selected = edui_last_item;
        }
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelLocalSoundTypeMenu,
                                const_cast<char *>("Highlighted Sound Type"));
    edanim_localsoundtype_menu = menu;
    if (!menu) {
        return;
    }

    const auto current_type = AnimParams[edanim_nearest_param_id].sound_ids[edanim_nearest_sound];
    for (i32 index = 0; index < edSfxAllCount; ++index) {
        const bool selected = current_type == index;
        eduiMenuAddItem(menu, eduiItemCheckCreate(index, colours, selected, 1, edanimcbSetLocalSoundType,
                                                  edbitsGetSoundName(index)));
        if (selected) {
            menu->selected = edui_last_item;
        }
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelLocalParticleTypeMenu,
                                const_cast<char *>("Highlighted Particle Type"));
    edanim_localparticletype_menu = menu;
    if (!menu) {
        return;
    }

    const auto current_type = AnimParams[edanim_nearest_param_id].effect_ids[edanim_nearest_particle];
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (!debtab[index]) {
            continue;
        }
        const bool selected = current_type == index;
        eduiMenuAddItem(
            menu, eduiItemCheckCreate(index, colours, selected, 1, edanimcbSetLocalParticleType, debtab[index]->name));
        if (selected) {
            menu->selected = edui_last_item;
        }
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edanimcbCancelParticleTypeMenu, const_cast<char *>("Particle Type"));
    edanim_particletype_menu = menu;
    if (!menu) {
        return;
    }

    eduiMenuAddItem(menu, eduiItemCheckCreate(0, colours, edanim_particle_type == -1, 0, edanimcbSetParticleType,
                                              const_cast<char *>("NONE")));
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        if (!debtab[index]) {
            continue;
        }
        const bool selected = edanim_particle_type == index;
        eduiMenuAddItem(menu,
                        eduiItemCheckCreate(index, colours, selected, 1, edanimcbSetParticleType, debtab[index]->name));
        if (selected) {
            menu->selected = edui_last_item;
        }
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    auto *menu =
        eduiMenuCreate(70, 70, 250, 200, ed_fnt, edanimcbCancelSwitchTypeMenu, const_cast<char *>("Switch Type"));
    edanim_switchtype_menu = menu;
    if (!menu) {
        return;
    }

    const auto add_type = [&](u32 type, const char *name) {
        const bool selected = AnimParams[edanim_nearest_param_id].field_00c == type;
        eduiMenuAddItem(
            menu, eduiItemCheckCreate(type, colours, selected, 1, edanimcbSetSwitchType, const_cast<char *>(name)));
        if (edui_last_item->highlighted & 1) {
            menu->selected = edui_last_item;
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

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
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
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edanim_nearest_sound == -1) {
        return;
    }
    auto *menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edanimcbCancelLocalSoundMenu,
                                const_cast<char *>("Highlighted Sound Settings"));
    edanim_localsound_menu = menu;
    if (!menu) {
        return;
    }

    auto &param = AnimParams[edanim_nearest_param_id];
    const i32 sound = edanim_nearest_sound;
    eduiMenuAddItem(menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbLocalSoundTypeMenu,
                                            const_cast<char *>("Highlighted Sound Type...")));
    const bool repeats = param.sound_flags[sound] == 1;
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, colours, repeats, 1, edanimcbToggleSoundType,
                                               const_cast<char *>("Repeating Sound")));
    if (repeats) {
        eduiMenuAddItem(menu, eduiItemSliderCreateInt(0, colours, 0, edanimcbSetSoundTiming, 1, 99,
                                                      static_cast<i32>(param.sound_values[sound]),
                                                      const_cast<char *>("Repeat Every")));
    } else {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
        if (NuSpecialTestAnim(&special)) {
            const auto *instance = NuSpecialGetInstAnim(&special);
            const f32 end_frame =
                *reinterpret_cast<const f32 *>(edbits_base_scene->instance_animation_data[instance->anim_ix]);
            eduiMenuAddItem(menu,
                            eduiItemSliderCreate(0, colours, 0, edanimcbSetSoundTiming, 1.0f, end_frame,
                                                 param.sound_values[sound], const_cast<char *>("Sound Trigger Time")));
            eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), const_cast<char *>("(%1.01f)"));
            eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);
        }
    }

    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}
