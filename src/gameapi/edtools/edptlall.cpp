#include "decomp.h"
#include "gameapi_edtools_types.h"
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
    i32 edbits_particle_char_page = 1;
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

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
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
static edui_slider_s *repeatbox_x_item;
static edui_slider_s *repeatbox_z_item;
static void UpdateTotalPtls(debinftype *effect);

// The original particle editor's private callbacks and menu state share this TU.
#include "gameapi/edtools/edptlcallbacks.cpp"

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32) {
    char original_directory[64] = {};
    if (edbits_datapath[0] != '\0') {
        NuFileGetCurrentDirectory(original_directory);
        NuFileSetCurrentDirectory(edbits_datapath);
    }

    char general_directory[256];
    char general_name[256];
    char general_extension[256];
    char level_directory[256];
    char level_name[256];
    char level_extension[256];
    if (!edbits_general_save_directory[0])
        strcpy(general_directory, ".");
    else
        strcpy(general_directory, edbits_general_save_directory);
    if (!edbits_general_save_name[0])
        strcpy(general_name, "particle");
    else
        strcpy(general_name, edbits_general_save_name);
    if (!edbits_general_save_extension[0])
        strcpy(general_extension, "ptl");
    else
        strcpy(general_extension, edbits_general_save_extension);
    if (!edbits_level_save_directory[0])
        strcpy(level_directory, ".");
    else
        strcpy(level_directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        strcpy(level_name, "particle");
    else
        strcpy(level_name, edbits_level_save_name);
    if (!edbits_level_save_extension[0])
        strcpy(level_extension, "ptl");
    else
        strcpy(level_extension, edbits_level_save_extension);

    char filename[256];
    char backup[256];
    bool valid_page = true;
    const char *saved_name = NULL;
    if (edpp_effect_list == 0) {
        saved_name = edpp_save_names[0];
        if (saved_name == NULL) {
            sprintf(filename, "%s\\%s.%s", general_directory, general_name, general_extension);
            sprintf(backup, "%s\\%s.%s.bak", general_directory, general_name, general_extension);
        }
    } else if (edpp_effect_list == 1) {
        saved_name = edpp_save_names[1];
        if (saved_name == NULL) {
            sprintf(filename, "%s\\%s.%s", level_directory, level_name, level_extension);
            sprintf(backup, "%s\\%s.%s.bak", level_directory, level_name, level_extension);
        }
    } else if (edpp_effect_list == 5) {
        saved_name = edpp_save_names[5];
        if (saved_name == NULL) {
            sprintf(filename, "%s\\char.%s", general_directory, general_extension);
            sprintf(backup, "%s\\char.%s.bak", general_directory, general_extension);
        }
    } else {
        valid_page = false;
    }
    if (saved_name != NULL) {
        NuStrCpy(filename, const_cast<char *>(saved_name));
        NuStrCpy(backup, filename);
        NuStrCat(backup, const_cast<char *>(".bak"));
    }

    i32 backup_succeeded = 1;
    i32 save_succeeded = 0;
    if (valid_page) {
        if (edpp_usememcard == 0 && edbits_override_backups == 0)
            backup_succeeded = EdFileBackup(filename, backup);
        save_succeeded = edppSaveEffects(filename, edpp_effect_list);
    }
    const char *message = "File Save Error";
    if (save_succeeded != 0)
        message = backup_succeeded != 0 ? "Saved OK" : "Saved OK - Backup Failed";
    u32 colours[4] = {save_succeeded != 0 ? 0x8000c000u : 0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
    messagemenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbCancelMessageMenu, "Message");
    if (messagemenu != NULL) {
        eduiMenuAddItem(messagemenu, eduiItemSelCreate(1, colours, 0, 0, NULL, const_cast<char *>(message)));
        eduiMenuAttach(parent, messagemenu);
        messagemenu->x = parent->x + 10;
        messagemenu->y = parent->y + 40;
    }
    if (original_directory[0] != '\0')
        NuFileSetCurrentDirectory(original_directory);
}

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

// These callbacks and the repeat-box sliders belong to the same original TU.
static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_repeatbox_menu);
    edptl_repeatbox_menu = NULL;
    repeatbox_x_item = NULL;
    repeatbox_z_item = NULL;
}
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_repeatbox_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, cbPtlCancelRepeatBoxMenu, "Repeat Box");
    if (edptl_repeatbox_menu == NULL)
        return;
    eduiMenuAddItem(edptl_repeatbox_menu,
                    eduiItemToggleCreate(0, edblack, edptl_repeatboxxzlock, 1, edptlChangeRepeatBoxXZLock, "XZ Lock"));
    eduiMenuAddItem(edptl_repeatbox_menu,
                    eduiItemSliderCreate(0, edblack, 0, edptlChangeRepeatBox, 0.1f, 10.0f * edptl_superscale,
                                         effect->repeat_box.x, "Repeat Box X"));
    repeatbox_x_item = static_cast<edui_slider_s *>(edui_last_item);
    eduiMenuAddItem(edptl_repeatbox_menu,
                    eduiItemSliderCreate(1, edblack, 0, edptlChangeRepeatBox, 0.1f, 10.0f * edptl_superscale,
                                         effect->repeat_box.y, "Repeat Box Y"));
    eduiMenuAddItem(edptl_repeatbox_menu,
                    eduiItemSliderCreate(2, edblack, 0, edptlChangeRepeatBox, 0.1f, 10.0f * edptl_superscale,
                                         effect->repeat_box.z, "Repeat Box Z"));
    repeatbox_z_item = static_cast<edui_slider_s *>(edui_last_item);
    eduiMenuAttach(parent, edptl_repeatbox_menu);
    edptl_repeatbox_menu->x = parent->x + 10;
    edptl_repeatbox_menu->y = parent->y + 40;
}

static void UpdateTotalPtls(debinftype *effect) {
    f32 elapsed_time = 0.0f;
    f32 active_time = 0.0f;
    while (effect->particle_lifetime > elapsed_time) {
        f32 remaining_time = effect->particle_lifetime - elapsed_time;
        f32 emission_time = effect->emission_period_random + effect->emission_pause;
        if (remaining_time < emission_time) {
            active_time += remaining_time;
            elapsed_time += remaining_time;
        } else {
            active_time += emission_time;
            elapsed_time += emission_time;
        }

        remaining_time = effect->particle_lifetime - elapsed_time;
        if (remaining_time < effect->emission_pause_random) {
            elapsed_time += remaining_time;
        } else {
            elapsed_time += effect->emission_pause_random;
        }
    }

    i16 particle_count = static_cast<i16>(static_cast<i32>(static_cast<f32>(effect->frequency) *
                                                           (active_time / elapsed_time) * effect->particle_lifetime));
    if (particle_count < 1) {
        particle_count = 1;
    }
    effect->max_particles = static_cast<i16>(particle_count * (effect->trail_count + 1));

    for (i32 i = 0; i < 512; ++i) {
        i32 instance_id = edpp_ptls[i].instance_id;
        if (instance_id == 99999) {
            continue;
        }
        if (instance_id == -1) {
            continue;
        }

        debkeydatatype_s *key = &debkeydata[instance_id];
        if (debtab[key->effect_index] == effect) {
            DebReAlloc(key, effect->max_particles);
        }
    }
}

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edptl_page_menu =
        eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelPageMenu, const_cast<char *>("Test Page Menu"));
    if (edptl_page_menu == NULL)
        return;
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edptlcbStartPage, const_cast<char *>("Start Page 1")));
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(2, colours, 0, 0, edptlcbStartPage, const_cast<char *>("Start Page 2")));
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edptlcbStopPage, const_cast<char *>("Stop Page 1")));
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(2, colours, 0, 0, edptlcbStopPage, const_cast<char *>("Stop Page 2")));
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edptlcbClearPage, const_cast<char *>("Clear Page 1")));
    eduiMenuAddItem(edptl_page_menu,
                    eduiItemSelCreate(2, colours, 0, 0, edptlcbClearPage, const_cast<char *>("Clear Page 2")));
    eduiMenuAttach(parent, edptl_page_menu);
    edptl_page_menu->x = parent->x + 10;
    edptl_page_menu->y = parent->y + 40;
}
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }

    i16 render_group = static_cast<i16>(static_cast<i32>(static_cast<edui_slider_s *>(item)->value));
    edpp_ptls[edpp_nearest].render_group = render_group;
    debkeydata[edpp_ptls[edpp_nearest].instance_id].render_group = render_group;
}
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    u32 colours[4] = {0xc479c000, 0xc479c000, 0xc479c000, 0xc479c000};
    edptl_star_menu = eduiMenuCreate(70, 70, 200, 300, ed_fnt, edptlcbCancelStarMenu, "Star Settings");
    if (edptl_star_menu == NULL)
        return;
    eduiMenuAddItem(edptl_star_menu,
                    eduiItemSliderCreateInt(0, colours, 0, edptlcbApplyStarPoints, 3, 17,
                                            static_cast<i8>(effect->radial_segments), "Number of Points"));
    eduiMenuAddItem(edptl_star_menu, eduiItemSliderCreate(0, colours, 0, edptlcbApplyStarRatio, 0.1f, 0.8f,
                                                          effect->radial_floor, "Radius Ratio"));
    eduiMenuAttach(parent, edptl_star_menu);
    edptl_star_menu->x = parent->x + 10;
    edptl_star_menu->y = parent->y + 40;
}
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32) {
    edppStopPage(static_cast<i8>(item->data));
}
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32) {
    edppClearPage(static_cast<i8>(item->data));
}
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    u32 colours[4] = {0xc479c000, 0xc479c000, 0xc479c000, 0xc479c000};
    edptl_ghost_menu = eduiMenuCreate(70, 70, 200, 300, ed_fnt, edptlcbCancelGhostMenu, "Particle Ghosts");
    if (edptl_ghost_menu == NULL)
        return;
    eduiMenuAddItem(edptl_ghost_menu,
                    eduiItemSliderCreateInt(0, colours, 0, edptlcbApplyNumGhosts, 0, 10,
                                            static_cast<i8>(effect->trail_count), "Number of Ghosts"));
    eduiMenuAddItem(edptl_ghost_menu,
                    eduiItemSliderCreate(0, colours, 0, edptlcbApplyGhostTime, 0.0f, 1.0f,
                                         static_cast<f32>(static_cast<i32>(effect->trail_time)), "Ghost Separation"));
    eduiMenuAttach(parent, edptl_ghost_menu);
    edptl_ghost_menu->x = parent->x + 10;
    edptl_ghost_menu->y = parent->y + 40;
}
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edptl_group_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edptlcbCancelGroupMenu, "Render Settings");
    if (edptl_group_menu == NULL)
        return;
    u32 *colours = edblack;
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
        if (effect->particle_type == 7) {
            eduiMenuAddItem(edptl_group_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Draw Flag..."));
            colours = edblack;
        } else {
            eduiMenuAddItem(edptl_group_menu, eduiItemSelCreate(1, edblack, 0, 0, edptlcbDrawflagMenu, "Draw Flag..."));
        }
        eduiMenuAddItem(edptl_group_menu, eduiItemToggleCreate(2, colours, effect->cutscene_only != 0, 1,
                                                               edptlcbChangeCSDisable, "Disable in Cut-Scenes"));
        eduiMenuAddItem(edptl_group_menu, eduiItemSliderCreateInt(0, colours, 0, edptlcbSetGroup, 0, 32,
                                                                  edpp_ptls[edpp_nearest].render_group, "Group ID"));
    }
    eduiMenuAddItem(edptl_group_menu, eduiItemSliderCreateInt(0, colours, 0, edptlcbSetMasterGroup, 0, 32,
                                                              debris_render_group, "Master Test Switch"));
    eduiMenuAttach(parent, edptl_group_menu);
    edptl_group_menu->x = parent->x + 10;
    edptl_group_menu->y = parent->y + 40;
}
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32) {
    i32 nearest = edpp_nearest;
    if (nearest == 0) {
        return;
    }

    i8 detail_levels;
    if (item->highlighted) {
        detail_levels = static_cast<i8>(item->data | edpp_ptls[nearest].detail_levels);
    } else {
        detail_levels = static_cast<i8>(~item->data & edpp_ptls[nearest].detail_levels);
    }
    edpp_ptls[nearest].detail_levels = detail_levels;

    i32 instance_id = edpp_ptls[nearest].instance_id;
    if (instance_id == -1) {
        return;
    }
    if (instance_id == 99999) {
        return;
    }
    DebrisSetDetailLevels(instance_id, detail_levels);
}
void edppStartPage(i32 page) {
    i32 index = 0;
    edpp_particle_s *particle = edpp_ptls;
    while (index != 512) {
        while (particle->page != static_cast<i8>(page) || particle->instance_id != 99999) {
            ++index;
            ++particle;
            if (index == 512)
                goto page_started;
        }
        particle->instance_id = -1;
        particle->effect_index = LookupDebrisEffectPage(edpp_ptls[index].name, static_cast<i8>(page));
        edppStartSingleEffect(index);
        if (particle->instance_id == -1)
            particle->instance_id = 99999;
        ++index;
        ++particle;
    }
page_started:
    edpp_page_on[static_cast<i8>(page)] = 1;
}
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32) {
    edppStartPage(static_cast<i8>(item->data));
}
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] __attribute__((aligned(16))) = {0xc479c000, 0xc479c000, 0xc479c000, 0xc479c000};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    debkeydatatype_s *key = &debkeydata[edpp_ptls[edpp_nearest].instance_id];
    edptl_bounce_menu = eduiMenuCreate(70, 70, 200, 300, ed_fnt, edptlcbCancelBounceMenu, "Particle Bounce");
    if (edptl_bounce_menu == NULL)
        return;
    eduiMenuAddItem(edptl_bounce_menu,
                    eduiItemSliderCreate(0, colours, 0, edptlcbApplyBounceOffset, -10.0f * edptl_superscale,
                                         10.0f * edptl_superscale, key->collision_plane, "Plane Offset"));
    eduiMenuAddItem(edptl_bounce_menu, eduiItemSliderCreate(0, colours, 0, edptlcbApplyBounceFactor, 0.0f, 2.0f,
                                                            key->reflection_scale, "Bounce Factor"));
    eduiMenuAttach(parent, edptl_bounce_menu);
    edptl_bounce_menu->x = parent->x + 10;
    edptl_bounce_menu->y = parent->y + 40;
}
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edptl_detail_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edptlcbCancelDetailMenu, "Detail Level Settings");
    if (edptl_detail_menu == NULL)
        return;
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
        debinftype *effect = debtab[debkeydata[particle->instance_id].effect_index];
        eduiMenuAddItem(edptl_detail_menu, eduiItemSliderCreate(0, edblack, 0, edptlcbSetMaxThin, 1.0f, 9.0f,
                                                                effect->thinning, "Maximum Thinning"));
        eduiMenuAddItem(edptl_detail_menu, eduiItemToggleCreate(4, edblack, (particle->detail_levels >> 2) & 1, 1,
                                                                edptlcbSetDetail, "High Detail"));
        eduiMenuAddItem(edptl_detail_menu, eduiItemToggleCreate(2, edblack, (particle->detail_levels >> 1) & 1, 2,
                                                                edptlcbSetDetail, "Medium Detail"));
        eduiMenuAddItem(edptl_detail_menu, eduiItemToggleCreate(1, edblack, particle->detail_levels & 1, 3,
                                                                edptlcbSetDetail, "Low Detail"));
        eduiMenuAddItem(edptl_detail_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edptlcbTestDetailMenu, "Detail Level Test..."));
    }
    eduiMenuAttach(parent, edptl_detail_menu);
    edptl_detail_menu->x = parent->x + 10;
    edptl_detail_menu->y = parent->y + 40;
}
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->thinning = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32) {
    edptl_soundid_menu = NULL;

    u32 data = static_cast<u32>(item->data);
    i32 sound_id = static_cast<u16>(data);
    if (sound_id == 9999) {
        sound_id = -1;
    }
    i32 sound_index = data >> 16;

    if (edpp_nearest != -1) {
        i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
        if (instance_id != -1) {
            debinftype *effect = debtab[debkeydata[instance_id].effect_index];
            effect->sound_data[sound_index * 3] = sound_id;
        }
    }

    for (i32 i = 0; i < 512; ++i) {
        i32 instance_id = edpp_ptls[i].instance_id;
        if (instance_id == 99999 || instance_id == -1) {
            continue;
        }

        debkeydatatype_s *key = &debkeydata[instance_id];
        debinftype *effect = debtab[key->effect_index];
        key->process_collision_sound = 0;
        if (effect->sound_data[0] != -1) {
            key->process_collision_sound = 1;
        }
        if (effect->sound_data[3] != -1) {
            key->process_collision_sound = 1;
        }
        if (effect->sound_data[6] != -1) {
            key->process_collision_sound = 1;
        }
        if (effect->sound_data[9] != -1) {
            key->process_collision_sound = 1;
        }
    }

    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;

    const u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    char title[16];
    sprintf(title, "Sound %d Menu", static_cast<i32>(item->data) + 1);
    edptl_soundx_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edptlcbCancelSoundXMenu, title);
    if (edptl_soundx_menu == NULL)
        return;
    eduiMenuAddItem(edptl_soundx_menu, eduiItemSelCreate(item->data, colours, 0, 0, edptlcbSoundIDMenu, "Sound ID..."));
    eduiMenuAddItem(edptl_soundx_menu,
                    eduiItemSelCreate(item->data, colours, 0, 0, edptlcbSoundControlMenu, "Sound Control..."));
    eduiMenuAttach(parent, edptl_soundx_menu);
    edptl_soundx_menu->x = parent->x + 10;
    edptl_soundx_menu->y = parent->y + 40;
}
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    const u32 colours[4] = {0xc479c000, 0xc479c000, 0xc479c000, 0xc479c000};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    edptl_sounds_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edptlcbCancelSoundsMenu, "Attached Sounds");
    if (edptl_sounds_menu == NULL)
        return;
    char title[12];
    sprintf(title, "Sound %d...", 1);
    eduiMenuAddItem(edptl_sounds_menu, eduiItemSelCreate(0, colours, 0, 0, edptlcbSoundXMenu, title));
    sprintf(title, "Sound %d...", 2);
    eduiMenuAddItem(edptl_sounds_menu, eduiItemSelCreate(1, colours, 0, 0, edptlcbSoundXMenu, title));
    sprintf(title, "Sound %d...", 3);
    eduiMenuAddItem(edptl_sounds_menu, eduiItemSelCreate(2, colours, 0, 0, edptlcbSoundXMenu, title));
    sprintf(title, "Sound %d...", 4);
    eduiMenuAddItem(edptl_sounds_menu, eduiItemSelCreate(3, colours, 0, 0, edptlcbSoundXMenu, title));
    eduiMenuAttach(parent, edptl_sounds_menu);
    edptl_sounds_menu->x = parent->x + 10;
    edptl_sounds_menu->y = parent->y + 40;
}
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0xc479c000, 0xc479c000, 0xc479c000, 0xc479c000};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    edptl_switch_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelSwitchMenu, "Switch Menu");
    if (edptl_switch_menu == NULL)
        return;
    eduiMenuAddItem(edptl_switch_menu, eduiItemSelCreate(1, colours, 0, 0, edptlcbSwitchTypeMenu, "Switch Type..."));
    eduiMenuAddItem(edptl_switch_menu, eduiItemSliderCreateInt(0, colours, 0, edptlcbSetSwitchId, -1, 129,
                                                               edpp_ptls[edpp_nearest].switch_id, "Switch ID"));
    eduiMenuAddItem(edptl_switch_menu, eduiItemSliderCreate(0, colours, 0, edptlcbSetSwitchVar, 0.0f, 20.0f,
                                                            edpp_ptls[edpp_nearest].switch_variable, "Switch Var"));
    eduiMenuAttach(parent, edptl_switch_menu);
    edptl_switch_menu->x = parent->x + 10;
    edptl_switch_menu->y = parent->y + 40;
}
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_dpad_mode = item->data;
}
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    if (effect->time_group == item->data) {
        return;
    }

    DebFreeInstantly(&edpp_ptls[edpp_nearest].instance_id);
    effect->time_group = static_cast<i8>(item->data);
    edppStartSingleEffect(edpp_nearest);
}
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }

    i32 switch_id = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
    edpp_ptls[edpp_nearest].switch_id = switch_id;
    debkeydata[edpp_ptls[edpp_nearest].instance_id].trigger_second = switch_id;
}
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32) {
    const u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    const debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_soundid_menu = eduiMenuCreate(70, 70, 180, 200, ed_fnt, edptlcbCancelSoundIDMenu, "Sound ID");
    if (edptl_soundid_menu == NULL)
        return;

    eduiMenuAddItem(edptl_soundid_menu,
                    eduiItemCheckCreate((item->data << 16) + 9999, colours, effect->sound_data[item->data * 3] == -1, 0,
                                        edptlcbSetSoundID, "NONE"));
    for (i32 sound = 0; sound < 1600; ++sound) {
        if (g_soundInfo[sound].sfx_name == NULL)
            continue;
        if (effect->sound_data[item->data * 3] == sound) {
            eduiMenuAddItem(edptl_soundid_menu,
                            eduiItemCheckCreate((item->data << 16) + sound, colours, 1, 1, edptlcbSetSoundID,
                                                const_cast<char *>(g_soundInfo[sound].sfx_name)));
            edptl_soundid_menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(edptl_soundid_menu,
                            eduiItemCheckCreate((item->data << 16) + sound, colours, 0, 1, edptlcbSetSoundID,
                                                const_cast<char *>(g_soundInfo[sound].sfx_name)));
        }
    }
    eduiMenuAttach(parent, edptl_soundid_menu);
    edptl_soundid_menu->x = parent->x + 10;
    edptl_soundid_menu->y = parent->y + 40;
}
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32) {
    debtab[edpp_create_type]->category = 4;
    edptl_clipboard_entry = edpp_create_type;

    eduimenu_s *parent = menu->parent;
    if (parent != NULL) {
        eduiMenuDetach(menu);
    }
    if (menu->callback != NULL) {
        menu->callback(menu, parent);
    }
}
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    dpadmodemenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelDpadModeMenu, "Dpad Mode");
    if (dpadmodemenu == NULL)
        return;
    eduiMenuAddItem(dpadmodemenu,
                    eduiItemCheckCreate(0, colours, edpp_dpad_mode == 0, 1, edptlcbSetDpadMode, "Emitter Rotate"));
    eduiMenuAddItem(dpadmodemenu,
                    eduiItemCheckCreate(1, colours, edpp_dpad_mode == 1, 1, edptlcbSetDpadMode, "Gravity Rotate"));
    eduiMenuAddItem(dpadmodemenu,
                    eduiItemCheckCreate(2, colours, edpp_dpad_mode == 2, 1, edptlcbSetDpadMode, "Offset"));
    eduiMenuAddItem(dpadmodemenu,
                    eduiItemCheckCreate(3, colours, edpp_dpad_mode == 3, 1, edptlcbSetDpadMode, "Reflections"));
    eduiMenuAddItem(dpadmodemenu,
                    eduiItemCheckCreate(4, colours, edpp_dpad_mode == 4, 1, edptlcbSetDpadMode, "Texture Facing"));
    eduiMenuAttach(parent, dpadmodemenu);
    dpadmodemenu->x = parent->x + 10;
    dpadmodemenu->y = parent->y + 40;
}
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_drawflag_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edptlcbCancelDrawflagMenu, "Draw Flag");
    if (edptl_drawflag_menu == NULL)
        return;
    eduiMenuAddItem(edptl_drawflag_menu,
                    eduiItemCheckCreate(1, edblack, effect->time_group == 1, 1, edptlcbSetDrawflag, "Before Fog"));
    eduiMenuAddItem(edptl_drawflag_menu,
                    eduiItemCheckCreate(0, edblack, effect->time_group == 0, 1, edptlcbSetDrawflag, "After Fog"));
    eduiMenuAddItem(edptl_drawflag_menu,
                    eduiItemCheckCreate(3, edblack, effect->time_group == 3, 1, edptlcbSetDrawflag, "Super Early"));
    eduiMenuAddItem(edptl_drawflag_menu,
                    eduiItemCheckCreate(4, edblack, effect->time_group == 4, 1, edptlcbSetDrawflag, "Panel Mode"));
    eduiMenuAttach(parent, edptl_drawflag_menu);
    edptl_drawflag_menu->x = parent->x + 10;
    edptl_drawflag_menu->y = parent->y + 40;
}
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }

    f32 switch_variable = static_cast<edui_slider_s *>(item)->value;
    edpp_ptls[edpp_nearest].switch_variable = switch_variable;
    debkeydata[edpp_ptls[edpp_nearest].instance_id].switch_variable = switch_variable;
}
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
    if (item->data == 0) {
        f32 value = slider->value;
        effect->repeat_box.x = value;
        if (edptl_repeatboxxzlock != 0 && repeatbox_z_item != NULL) {
            effect->repeat_box.z = value;
            repeatbox_z_item->value = value;
            repeatbox_z_item->normalized_value = (value - repeatbox_z_item->minimum) / repeatbox_z_item->range;
        }
    } else if (item->data == 1) {
        effect->repeat_box.y = slider->value;
    } else if (item->data == 2) {
        f32 value = slider->value;
        effect->repeat_box.z = value;
        if (edptl_repeatboxxzlock != 0 && repeatbox_x_item != NULL) {
            effect->repeat_box.x = value;
            repeatbox_x_item->value = value;
            repeatbox_x_item->normalized_value = (value - repeatbox_x_item->minimum) / repeatbox_x_item->range;
        }
    }
}
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (edpp_create_type == -1)
        return;

    const u32 colours[4] __attribute__((aligned(16))) = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    ptlclipmenu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edptlcbCancelClipboardMenu, "Clipboard Menu");
    if (ptlclipmenu == NULL)
        return;

    char list_name[32];
    if (edpp_effect_list == 0)
        strcpy(list_name, "General List");
    else if (edpp_effect_list == 1)
        strcpy(list_name, "Level List");
    else if (edpp_effect_list == 5)
        strcpy(list_name, "Char List");
    char label[60];
    if (edptl_clipboard_entry == -1) {
        sprintf(label, "Cut %s from %s", debtab[edpp_create_type]->name, list_name);
        eduiMenuAddItem(ptlclipmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbCutClipboard, label));
    } else {
        sprintf(label, "Paste %s into %s", debtab[edptl_clipboard_entry]->name, list_name);
        eduiMenuAddItem(ptlclipmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbPasteClipboard, label));
        eduiMenuAddItem(ptlclipmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbEmptyClipboard, "Empty Clipboard"));
    }
    eduiMenuAttach(parent, ptlclipmenu);
    ptlclipmenu->x = parent->x + 10;
    ptlclipmenu->y = parent->y + 40;
}
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32) {
    for (i32 i = 0; i < 512; ++i) {
        if (edpp_ptls[i].effect_index == -1) {
            edppPtlDestroy(i);
        }
    }

    edpp_num_orphans = 0;
    eduimenu_s *parent = menu->parent;
    if (parent != NULL) {
        eduiMenuDetach(menu);
    }
    if (menu->callback != NULL) {
        menu->callback(menu, parent);
    }
}
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        i32 switch_type = item->data;
        edpp_ptls[edpp_nearest].switch_type = switch_type;
        debkeydata[edpp_ptls[edpp_nearest].instance_id].trigger_first = switch_type;
    }

    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    edptl_switchtype_menu = NULL;
}
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->trail_time = static_cast<edui_slider_s *>(item)->value;
}

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->trail_count = static_cast<u8>(static_cast<i32>(static_cast<edui_slider_s *>(item)->value));
    UpdateTotalPtls(effect);
}
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->start_offset_random = static_cast<edui_slider_s *>(item)->value;
    UpdateTotalPtls(effect);
}
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->emission_pause_random = static_cast<edui_slider_s *>(item)->value;
    UpdateTotalPtls(effect);
}
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->emission_pause = static_cast<edui_slider_s *>(item)->value;
    UpdateTotalPtls(effect);
}
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->emission_period_random = static_cast<edui_slider_s *>(item)->value;
    UpdateTotalPtls(effect);
}
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->particle_lifetime = static_cast<edui_slider_s *>(item)->value;
    UpdateTotalPtls(effect);
}
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->frequency = static_cast<i16>(static_cast<i32>(static_cast<edui_slider_s *>(item)->value));
    UpdateTotalPtls(effect);
}
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->radial_floor = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_page_menu);
    edptl_page_menu = NULL;
}
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_star_menu);
    edptl_star_menu = NULL;
}
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->field_140 = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->field_144 = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }

    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1) {
        return;
    }

    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    effect->scale_in_time = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32) {
    i32 create_type = edpp_create_type;
    edpp_create_type = edptl_clipboard_entry;
    edppDeleteEffect(edpp_create_type);
    edpp_create_type = -1;

    eduimenu_s *parent = menu->parent;
    if (parent != NULL) {
        eduiMenuDetach(menu);
    }
    if (menu->callback != NULL) {
        menu->callback(menu, parent);
    }

    edptl_clipboard_entry = -1;
    edpp_create_type = create_type;
}
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edptl_orphanlist_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelOrphanListMenu, "Orphan List");
    if (edptl_orphanlist_menu == NULL)
        return;
    for (i32 index = 0; index < 512; ++index) {
        if (edpp_ptls[index].effect_index != -1)
            continue;
        eduiMenuAddItem(edptl_orphanlist_menu, eduiItemSelCreate(0, edblack, 0, 0, NULL, edpp_ptls[index].name));
        edui_last_item->text_alignment = 0x10;
    }
    eduiMenuAttach(parent, edptl_orphanlist_menu);
    edptl_orphanlist_menu->x = parent->x + 10;
    edptl_orphanlist_menu->y = parent->y + 40;
}
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32) {
    debinftype *effect = debtab[edptl_clipboard_entry];
    edptl_clipboard_entry = -1;
    effect->category = edpp_effect_list;

    eduimenu_s *parent = menu->parent;
    if (parent != NULL) {
        eduiMenuDetach(menu);
    }
    if (menu->callback != NULL) {
        menu->callback(menu, parent);
    }
}
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32) {
    for (i32 i = 0; i < 512; ++i) {
        i32 *instance_id = &edpp_ptls[i].instance_id;
        if (*instance_id != 99999 && *instance_id != -1) {
            DebFreeInstantly(instance_id);
            *instance_id = 99999;
        }
    }
    edppRestartAllEffectsInLevel();
}
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32) {
    debris_render_group = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_scale_factor = static_cast<edui_slider_s *>(item)->value;
}
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    const u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edptl_switchtype_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelSwitchTypeMenu, "Switch Type");
    if (edptl_switchtype_menu == NULL)
        return;
    eduiMenuAddItem(edptl_switchtype_menu, eduiItemCheckCreate(0, colours, edpp_ptls[edpp_nearest].switch_type == 0, 1,
                                                               edptlcbSetSwitchType, "None"));
    eduiMenuAddItem(edptl_switchtype_menu, eduiItemCheckCreate(1, colours, edpp_ptls[edpp_nearest].switch_type == 1, 1,
                                                               edptlcbSetSwitchType, "Global Switch"));
    eduiMenuAttach(parent, edptl_switchtype_menu);
    edptl_switchtype_menu->x = parent->x + 10;
    edptl_switchtype_menu->y = parent->y + 40;
}
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edptl_testdetail_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edptlcbCancelTestDetailMenu, "Detail Level Test");
    if (edptl_testdetail_menu == NULL)
        return;
    eduiMenuAddItem(edptl_testdetail_menu, eduiItemSliderCreate(0, edblack, 0, edptlcbSetDebrisThinning, 1.0f, 9.0f,
                                                                debris_thinning_level, "Thinning Level"));
    eduiMenuAddItem(edptl_testdetail_menu, eduiItemCheckCreate(4, edblack, debris_detail_level == 4, 1,
                                                               edptlcbSetDebrisDetail, "High Detail"));
    eduiMenuAddItem(edptl_testdetail_menu, eduiItemCheckCreate(2, edblack, debris_detail_level == 2, 1,
                                                               edptlcbSetDebrisDetail, "Medium Detail"));
    eduiMenuAddItem(edptl_testdetail_menu,
                    eduiItemCheckCreate(1, edblack, debris_detail_level == 1, 1, edptlcbSetDebrisDetail, "Low Detail"));
    eduiMenuAttach(parent, edptl_testdetail_menu);
    edptl_testdetail_menu->x = parent->x + 10;
    edptl_testdetail_menu->y = parent->y + 40;
}
