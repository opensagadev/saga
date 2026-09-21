#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <string.h>

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

static i32 edptl_superscale = 1;

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

static eduiitem_s *torus_env1_item;
static eduiitem_s *torus_env2_item;
static eduiitem_s *torus_env3_item;
static eduiitem_s *coll_env_item;
static edui_slider_s *grad_jib_x_freq_item;
static edui_slider_s *grad_jib_x_amp_item;
static edui_slider_s *grad_jib_y_freq_item;
static edui_slider_s *grad_jib_y_amp_item;
static eduiitem_s *grad_rot_item;
static edui_slider_s *grad_rot_min_item;
static edui_slider_s *grad_rot_max_item;
static eduiitem_s *grad_size_w_item;
static eduiitem_s *grad_size_h_item;
static edui_slider_s *grad_size_min_item;
static edui_slider_s *grad_size_max_item;
static eduiitem_s *grad_item;
static eduiitem_s *grad_alpha_item;

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

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->radial_segments = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_ghost_menu);
    edptl_ghost_menu = NULL;
}

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_group_menu);
    edptl_group_menu = NULL;
}

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->cutscene_only = item->highlighted;
}

static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32) {
    debris_detail_level = item->data;
}

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32) {
    edptl_soundcontrol_menu = NULL;
    u32 data = static_cast<u32>(item->data);
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
        u32 sound_index = data >> 16;
        effect->sound_data[sound_index * 3 + 1] = data - (sound_index << 16);
    }
    for (i32 i = 0; i < 512; ++i) {
        i32 instance_id = edpp_ptls[i].instance_id;
        if (instance_id == 99999 || instance_id == -1) {
            continue;
        }
        debkeydatatype_s *key = &debkeydata[instance_id];
        debinftype *effect = debtab[key->effect_index];
        key->collision_timers[0] = 9999;
        if (effect->sound_data[1] == 3 || effect->sound_data[1] == 4) {
            key->collision_timers[0] = 1;
        }
        key->collision_timers[1] = 9999;
        if (effect->sound_data[4] == 3 || effect->sound_data[4] == 4) {
            key->collision_timers[1] = 1;
        }
        key->collision_timers[2] = 9999;
        if (effect->sound_data[7] == 3 || effect->sound_data[7] == 4) {
            key->collision_timers[2] = 1;
        }
        key->collision_timers[3] = 9999;
        if (effect->sound_data[10] == 3 || effect->sound_data[10] == 4) {
            key->collision_timers[3] = 1;
        }
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_bounce_menu);
    edptl_bounce_menu = NULL;
}

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_detail_menu);
    edptl_detail_menu = NULL;
}

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundx_menu);
    edptl_soundx_menu = NULL;
}

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_sounds_menu);
    edptl_sounds_menu = NULL;
}

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_switch_menu);
    edptl_switch_menu = NULL;
}

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_soundcontrol_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edptlcbCancelSoundControlMenu, "Sound Control");
    if (edptl_soundcontrol_menu != NULL) {
        eduiMenuAddItem(edptl_soundcontrol_menu, eduiItemCheckCreate(static_cast<u32>(item->data) << 16, colours,
                                                                     effect->sound_data[item->data * 3 + 1] == 0, 1,
                                                                     edptlcbSetSoundControl, "Off"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu, eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 1, colours,
                                                                     effect->sound_data[item->data * 3 + 1] == 1, 1,
                                                                     edptlcbSetSoundControl, "On Edge"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu, eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 2, colours,
                                                                     effect->sound_data[item->data * 3 + 1] == 2, 1,
                                                                     edptlcbSetSoundControl, "Off Edge"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu, eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 3, colours,
                                                                     effect->sound_data[item->data * 3 + 1] == 3, 1,
                                                                     edptlcbSetSoundControl, "Per Particle"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAddItem(edptl_soundcontrol_menu, eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 4, colours,
                                                                     effect->sound_data[item->data * 3 + 1] == 4, 1,
                                                                     edptlcbSetSoundControl, "Continuous"));
        if (edui_last_item->highlighted) {
            edptl_soundcontrol_menu->selected = edui_last_item;
        }
        eduiMenuAttach(menu, edptl_soundcontrol_menu);
        edptl_soundcontrol_menu->x = menu->x + 10;
        edptl_soundcontrol_menu->y = menu->y + 40;
    }
}

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
    if (particle->instance_id == -1) {
        return;
    }
    particle->reflection_bounce = static_cast<edui_slider_s *>(item)->value;
    debkeydata[particle->instance_id].reflection_scale = particle->reflection_bounce;
}

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
    if (particle->instance_id == -1) {
        return;
    }
    particle->reflection_offset = static_cast<edui_slider_s *>(item)->value;
    debkeydata[particle->instance_id].collision_plane = particle->reflection_offset;
}

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundid_menu);
    edptl_soundid_menu = NULL;
}

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32) {
    debris_thinning_level = static_cast<edui_slider_s *>(item)->value;
}

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(dpadmodemenu);
    dpadmodemenu = NULL;
}

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_drawflag_menu);
    edptl_drawflag_menu = NULL;
}

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlclipmenu);
    ptlclipmenu = NULL;
}

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_orphanlist_menu);
    edptl_orphanlist_menu = NULL;
}

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_switchtype_menu);
    edptl_switchtype_menu = NULL;
}

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_testdetail_menu);
    edptl_testdetail_menu = NULL;
}

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_scaleeffect_menu);
    edptl_scaleeffect_menu = NULL;
}

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_soundcontrol_menu);
    edptl_soundcontrol_menu = NULL;
}

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_create_type != -1 && debtab[edpp_create_type] != NULL) {
        strcpy(debtab[edpp_create_type]->name, static_cast<edui_textpicker_s *>(item)->value);
    }
}

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    if (menu != NULL) {
        if (menu == ptlvaremitmenu)
            effect->field_04c = static_cast<edui_slider_s *>(item)->value;
        else if (menu == ptlvarstartmenu)
            effect->field_058 = static_cast<edui_slider_s *>(item)->value;
        else if (menu == ptlstartvelmenu)
            effect->emitter_velocity.x = static_cast<edui_slider_s *>(item)->value;
    }
}

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    if (menu != NULL) {
        if (menu == ptlvaremitmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->field_050 = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 11 ||
                     effect->generator_type == 12)
                effect->field_050 = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
        } else if (menu == ptlvarstartmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->field_05c = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 11 ||
                     effect->generator_type == 12)
                effect->field_05c = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
        } else if (menu == ptlstartvelmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->emitter_velocity.y = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 11 ||
                     effect->generator_type == 12)
                effect->emitter_velocity.y =
                    static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
        }
    }
}

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance_id = edpp_ptls[edpp_nearest].instance_id;
    if (instance_id == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance_id].effect_index];
    if (menu != NULL) {
        if (menu == ptlvaremitmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->field_054 = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 12)
                effect->field_054 = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
            else if (effect->generator_type == 11) {
                effect->field_054 = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
                f32 limit = 16384.0f - effect->field_054;
                effect->field_060 = MIN(effect->field_060, limit);
                effect->field_060 = MAX(effect->field_060, -limit);
            }
        } else if (menu == ptlvarstartmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->field_060 = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 12)
                effect->field_060 = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
            else if (effect->generator_type == 11) {
                effect->field_060 = static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
                f32 limit = 16384.0f - effect->field_060;
                effect->field_054 = MIN(effect->field_054, limit);
                effect->field_054 = MAX(effect->field_054, -limit);
            }
        } else if (menu == ptlstartvelmenu) {
            if (effect->generator_type == 0 || effect->generator_type == 8 || effect->generator_type == 9 ||
                effect->generator_type == 10)
                effect->emitter_velocity.z = static_cast<edui_slider_s *>(item)->value;
            else if (effect->generator_type == 6 || effect->generator_type == 7 || effect->generator_type == 11 ||
                     effect->generator_type == 12)
                effect->emitter_velocity.z =
                    static_cast<i32>((65536.0f / 360.0f) * static_cast<edui_slider_s *>(item)->value);
        }
    }
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

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32) {
    ptltypemenu = NULL;
    edpp_active_menu = NULL;
    edpp_create_type = item->data;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32) {
    edpp_showAllPlaced = !edpp_showAllPlaced;
}

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    if (grad_jib_x_freq_item)
        effect->jib_x_frequency = grad_jib_x_freq_item->value;
    if (grad_jib_x_amp_item)
        effect->jib_x_amplitude = grad_jib_x_amp_item->value;
    if (grad_jib_y_freq_item)
        effect->jib_y_frequency = grad_jib_y_freq_item->value;
    if (grad_jib_y_amp_item)
        effect->jib_y_amplitude = grad_jib_y_amp_item->value;
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!grad_rot_min_item || !grad_rot_max_item || !grad_rot_item)
        return;
    i32 count = eduiGradPickRead(grad_rot_item, stages, 8);
    f32 minimum = grad_rot_min_item->value;
    f32 maximum = grad_rot_max_item->value;
    if (minimum == maximum)
        maximum += 0.1f;
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->rotation_keys[i].time = stages[i].time;
        effect->rotation_keys[i].value =
            static_cast<i32>((stages[i].red * (maximum - minimum) + minimum) * (65536.0f / 360.0f));
    }
    effect->min_rotation = minimum;
    effect->max_rotation = maximum;
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlCollMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    edui_gradient_pick_s *gradient;
    if (item->data == 1) {
        memcpy(effect->height_keys, effect->width_keys, sizeof(effect->height_keys));
        gradient = static_cast<edui_gradient_pick_s *>(grad_size_h_item);
    } else {
        memcpy(effect->width_keys, effect->height_keys, sizeof(effect->width_keys));
        gradient = static_cast<edui_gradient_pick_s *>(grad_size_w_item);
    }
    while (gradient->first_stage)
        eduiGradStageDelete(gradient, gradient->first_stage);
    for (i32 i = 0; i < 8; ++i) {
        if (item->data == 1) {
            f32 grey = (effect->height_keys[i].value - effect->min_size) / (effect->max_size - effect->min_size);
            eduiGradStageAddRGB(static_cast<edui_gradient_pick_s *>(grad_size_h_item), effect->height_keys[i].time,
                                grey, grey, grey);
            if (effect->height_keys[i].time == 1.0f)
                break;
        } else {
            f32 grey = (effect->width_keys[i].value - effect->min_size) / (effect->max_size - effect->min_size);
            eduiGradStageAddRGB(static_cast<edui_gradient_pick_s *>(grad_size_w_item), effect->width_keys[i].time, grey,
                                grey, grey);
            if (effect->width_keys[i].time == 1.0f)
                break;
        }
    }
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlDataMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    ptlgravmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelGravMenu, "Gravity");
    if (ptlgravmenu != NULL) {
        eduiMenuAddItem(ptlgravmenu, eduiItemSliderCreate(0, colours, 0, cbPtlChangeGrav, -10.0f * edptl_superscale,
                                                          20.0f * edptl_superscale, effect->field_0a0, "Gravity"));
        eduiMenuAttach(menu, ptlgravmenu);
        ptlgravmenu->x = menu->x + 10;
        ptlgravmenu->y = menu->y + 40;
    }
}

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32) {
    eduiMenuDetach(menu);
    edpp_active_menu = NULL;
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debkeydatatype_s *key = &debkeydata[edpp_ptls[edpp_nearest].instance_id];
    debinftype *effect = debtab[key->effect_index];
    effect->momentum_adjustment_type = static_cast<u8>(item->data - 1);
    key->momentum_adjuster = gencodetab[item->data - 1];
}

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32) {
    eduiMenuDetach(menu);
    edpp_active_menu = NULL;
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debkeydatatype_s *key = &debkeydata[edpp_ptls[edpp_nearest].instance_id];
    debinftype *effect = debtab[key->effect_index];
    if (static_cast<i8>(effect->generator_type) == item->data) {
        return;
    }
    if (effect->generator_type == 6 && item->data == 11) {
        effect->field_054 *= 0.5f;
        effect->field_060 *= 0.5f;
        effect->emitter_velocity.z *= 0.5f;
    } else if (effect->generator_type == 11 && item->data == 6) {
        effect->field_054 *= 2.0f;
        effect->field_060 *= 2.0f;
        effect->emitter_velocity.z *= 2.0f;
    } else if (!((effect->generator_type == 6 && item->data == 12) ||
                 (effect->generator_type == 12 && item->data == 6))) {
        effect->field_04c = 0.0f;
        effect->scale_in_time = 0.0f;
        effect->field_050 = 0.0f;
        effect->field_054 = 0.0f;
        effect->field_058 = 0.0f;
        effect->field_05c = 0.0f;
        effect->field_060 = 0.0f;
        effect->emitter_velocity.x = 0.0f;
        effect->emitter_velocity.y = 0.0f;
        effect->emitter_velocity.z = 0.0f;
        effect->trail_count = 0;
        key = &debkeydata[edpp_ptls[edpp_nearest].instance_id];
    }
    effect->generator_type = static_cast<u8>(item->data);
    key->generator = gensorttab[item->data];
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
    edui_gradient_stage_s stages[8];
    if (grad_item) {
        i32 count = eduiGradPickRead(grad_item, stages, 8);
        if (count >= 2 && count <= 8 && edpp_nearest != -1) {
            i32 instance = edpp_ptls[edpp_nearest].instance_id;
            if (instance != -1) {
                debinftype *effect = debtab[debkeydata[instance].effect_index];
                for (i32 i = 0; i < count; ++i) {
                    effect->colour_keys[i].time = stages[i].time;
                    effect->colour_keys[i].red = static_cast<i32>(stages[i].red * 255.0f);
                    effect->colour_keys[i].green = static_cast<i32>(stages[i].green * 255.0f);
                    effect->colour_keys[i].blue = static_cast<i32>(stages[i].blue * 255.0f);
                }
                GenericDebinfoDmaTypeUpdate(effect);
            }
        }
    }
    if (grad_alpha_item) {
        i32 count = eduiGradPickRead(grad_alpha_item, stages, 8);
        if (count >= 2 && count <= 8 && edpp_nearest != -1) {
            i32 instance = edpp_ptls[edpp_nearest].instance_id;
            if (instance != -1) {
                debinftype *effect = debtab[debkeydata[instance].effect_index];
                for (i32 i = 0; i < count; ++i) {
                    effect->alpha_keys[i].time = stages[i].time;
                    effect->alpha_keys[i].value = stages[i].red * 255.0f;
                }
                GenericDebinfoDmaTypeUpdate(effect);
            }
        }
    }
}

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!grad_size_min_item || !grad_size_max_item || !grad_size_h_item || !grad_size_w_item)
        return;
    i32 count = eduiGradPickRead(grad_size_w_item, stages, 8);
    f32 minimum = grad_size_min_item->value;
    f32 maximum = grad_size_max_item->value;
    if (count >= 2 && count <= 8 && edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < count; ++i) {
                effect->width_keys[i].time = stages[i].time;
                effect->width_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
            }
            effect->min_size = minimum;
            effect->max_size = maximum;
            GenericDebinfoDmaTypeUpdate(effect);
        }
    }
    count = eduiGradPickRead(grad_size_h_item, stages, 8);
    if (count >= 2 && count <= 8 && edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < count; ++i) {
                effect->height_keys[i].time = stages[i].time;
                effect->height_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
            }
            effect->min_size = minimum;
            effect->max_size = maximum;
            GenericDebinfoDmaTypeUpdate(effect);
        }
    }
}

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    ptlgsortmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelGSortMenu, "GenSort Type");
    if (ptlgsortmenu) {
        eduiMenuAddItem(ptlgsortmenu,
                        eduiItemCheckCreate(0, colours, effect->generator_type == 0, 1, cbPtlSelGSort, "Normal"));
        if (effect->particle_type == 7) {
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(6, edgrey, 0, 0, NULL, "Radial"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(7, edgrey, 0, 0, NULL, "Radial Rotor"));
        } else {
            eduiMenuAddItem(ptlgsortmenu,
                            eduiItemCheckCreate(6, colours, effect->generator_type == 6, 1, cbPtlSelGSort, "Radial"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemCheckCreate(7, colours, effect->generator_type == 7, 1, cbPtlSelGSort,
                                                              "Radial Rotor"));
        }
        eduiMenuAddItem(ptlgsortmenu,
                        eduiItemCheckCreate(8, colours, effect->generator_type == 8, 1, cbPtlSelGSort, "Spheroid"));
        if (effect->particle_type == 7) {
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(9, edgrey, 0, 0, NULL, "BounceY"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(10, edgrey, 0, 0, NULL, "BounceXZ"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(11, edgrey, 0, 0, NULL, "Improved Radial"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemSelCreate(12, edgrey, 0, 0, NULL, "Star Radial"));
        } else {
            eduiMenuAddItem(ptlgsortmenu,
                            eduiItemCheckCreate(9, colours, effect->generator_type == 9, 1, cbPtlSelGSort, "BounceY"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemCheckCreate(10, colours, effect->generator_type == 10, 1,
                                                              cbPtlSelGSort, "BounceXZ"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemCheckCreate(11, colours, effect->generator_type == 11, 1,
                                                              cbPtlSelGSort, "Improved Radial"));
            eduiMenuAddItem(ptlgsortmenu, eduiItemCheckCreate(12, colours, effect->generator_type == 12, 1,
                                                              cbPtlSelGSort, "Star Radial"));
        }
    }
    eduiMenuAttach(menu, ptlgsortmenu);
    ptlgsortmenu->x = menu->x + 10;
    ptlgsortmenu->y = menu->y + 40;
}

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
    particle->facing_mode = item->highlighted;
    if (item->highlighted) {
        particle->facing_rotation_x = edpp_facrotx;
        particle->facing_rotation_y = edpp_facroty;
        DebrisSetFacing(particle->instance_id, 1, particle->facing_rotation_x, particle->facing_rotation_y);
    } else {
        particle->facing_rotation_x = 0;
        particle->facing_rotation_y = 0;
        DebrisSetFacing(particle->instance_id, 0, 0, 0);
    }
}

static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->field_0a0 = static_cast<edui_slider_s *>(item)->value;
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    edptl_damage_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, cbPtlCancelDamageMenu, "Particle Damage");
    if (edptl_damage_menu != NULL) {
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlDamageFlagMenu, "Damage Flags..."));
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlCollMenu, "Collision Spheres..."));
        eduiMenuAddItem(edptl_damage_menu, eduiItemSelCreate(1, edblack, 0, 0, cbPtlTorusMenu, "Collision Torus..."));
    }
    eduiMenuAttach(menu, edptl_damage_menu);
    edptl_damage_menu->x = menu->x + 10;
    edptl_damage_menu->y = menu->y + 40;
}

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    sscalemenu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, cbPtlCancelSScaleMenu, "Super Scale");
    if (sscalemenu != NULL) {
        eduiMenuAddItem(sscalemenu, eduiItemSliderCreateInt(0, colours, 0, cbPtlChangeSScale, 1, 99, edptl_superscale,
                                                            "Super Scale"));
        eduiMenuAttach(menu, sscalemenu);
        sscalemenu->x = menu->x + 10;
        sscalemenu->y = menu->y + 40;
    }
}

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_readout = item->data;
}

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_snap_enabled = item->highlighted;
}

static void cbSelEffectList(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_create_type == -1) {
        return;
    }
    debinftype *effect = debtab[edpp_create_type];
    namemenu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, cbCancelChangeNameMenu, "Type Name");
    if (namemenu != NULL) {
        eduiMenuAddItem(namemenu, eduiItemTextPickCreate(0, colours, cbChangeName, "Type Name"));
        strcpy(static_cast<edui_textpicker_s *>(edui_last_item)->value, effect->name);
        static_cast<edui_textpicker_s *>(edui_last_item)->max_length = 15;
        eduiMenuAttach(menu, namemenu);
        namemenu->x = menu->x + 10;
        namemenu->y = menu->y + 40;
    }
}

static void cbEffectListMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->cut_on = static_cast<edui_slider_s *>(item)->value;
    if (effect->cut_on > effect->clip_extent) {
        effect->clip_extent = effect->cut_on;
    }
}

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    ptlemitvelmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelEmitVelMenu, "Emitter Vel");
    if (ptlemitvelmenu != NULL) {
        eduiMenuAddItem(ptlemitvelmenu,
                        eduiItemSliderCreate(0, colours, 0, cbPtlChangeEmitVel, -(10.0f * edptl_superscale),
                                             20.0f * edptl_superscale, effect->field_048, "Emitter Vel"));
        eduiMenuAttach(menu, ptlemitvelmenu);
        ptlemitvelmenu->x = menu->x + 10;
        ptlemitvelmenu->y = menu->y + 40;
    }
}

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    ptlreadoutmenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelReadoutMenu, "Info Box Style");
    if (ptlreadoutmenu != NULL) {
        eduiMenuAddItem(ptlreadoutmenu,
                        eduiItemCheckCreate(0, colours, edpp_readout == 0, 1, cbPtlSelReadout, "Normal"));
        eduiMenuAddItem(ptlreadoutmenu,
                        eduiItemCheckCreate(1, colours, edpp_readout == 1, 1, cbPtlSelReadout, "Co-ordinates"));
    }
    eduiMenuAttach(menu, ptlreadoutmenu);
    ptlreadoutmenu->x = menu->x + 10;
    ptlreadoutmenu->y = menu->y + 40;
}

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32) {
    debtab[edpp_ptls[edpp_nearest].effect_index]->camera_facing = item->highlighted;
}

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    texturemenu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, cbPtlCancelTextureMenu, "Texture");
    if (texturemenu) {
        eduiMenuAddItem(texturemenu, eduiItemCheckCreate(0, colours, effect->particle_type == 0, 1, cbPtlSelTextureType,
                                                         "Addative"));
        eduiMenuAddItem(texturemenu, eduiItemCheckCreate(2, colours, effect->particle_type == 2, 1, cbPtlSelTextureType,
                                                         "Modulative"));
        eduiMenuAddItem(texturemenu, eduiItemCheckCreate(3, colours, effect->particle_type == 3, 1, cbPtlSelTextureType,
                                                         "Subtractive"));
        if (effect->generator_type == 0 || effect->generator_type == 8) {
            eduiMenuAddItem(texturemenu, eduiItemCheckCreate(7, colours, effect->particle_type == 7, 1,
                                                             cbPtlSelTextureType, "Glass"));
        } else {
            eduiMenuAddItem(texturemenu, eduiItemSelCreate(7, edgrey, 0, 0, NULL, "Glass"));
        }
        if (effect->particle_type == 7) {
            eduiMenuAddItem(texturemenu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Texture Selector..."));
        } else {
            eduiMenuAddItem(texturemenu,
                            eduiItemSelCreate(1, colours, 0, 0, cbPtlTextureSelectMenu, "Texture Selector..."));
        }
        eduiMenuAddItem(texturemenu, eduiItemToggleCreate(0, edblack, static_cast<i8>(effect->camera_facing), 2,
                                                          cbPtlSetXZFacing, "Default to XZ Plane"));
        eduiMenuAttach(menu, texturemenu);
        texturemenu->x = menu->x + 10;
        texturemenu->y = menu->y + 40;
    }
}

static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->torus_lifetime = static_cast<edui_slider_s *>(item)->value;
}

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->torus_radius1 = static_cast<edui_slider_s *>(item)->value;
}

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->torus_radius2 = static_cast<edui_slider_s *>(item)->value;
}

static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!coll_env_item)
        return;
    i32 count = eduiGradPickRead(coll_env_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    f32 minimum = effect->min_size / 10000.0f;
    f32 maximum = effect->max_size / 10000.0f;
    for (i32 i = 0; i < count; ++i) {
        effect->collision_keys[i].time = stages[i].time;
        effect->collision_keys[i].value = stages[i].red * (maximum - minimum) + minimum;
    }
}

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->clip_extent = static_cast<edui_slider_s *>(item)->value;
    if (effect->clip_extent < effect->cut_on) {
        effect->cut_on = effect->clip_extent;
    }
}

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32) {
    edptl_superscale = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32) {
    edppDeleteEffect(edpp_create_type);
    edpp_create_type = -1;
    eduimenu_s *parent = menu->parent;
    if (parent) {
        eduiMenuDetach(menu);
    }
    if (menu->callback) {
        menu->callback(menu, parent);
    }
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

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->field_048 = static_cast<edui_slider_s *>(item)->value;
}

static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env1_item)
        return;
    i32 count = eduiGradPickRead(torus_env1_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys1[i].time = stages[i].time;
        effect->torus_keys1[i].value = stages[i].red;
    }
}

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env2_item)
        return;
    i32 count = eduiGradPickRead(torus_env2_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys2[i].time = stages[i].time;
        effect->torus_keys2[i].value = stages[i].red;
    }
}

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32) {
    edui_gradient_stage_s stages[8];
    if (!torus_env3_item)
        return;
    i32 count = eduiGradPickRead(torus_env3_item, stages, 8);
    if (count < 2 || count > 8 || edpp_nearest == -1)
        return;
    i32 instance = edpp_ptls[edpp_nearest].instance_id;
    if (instance == -1)
        return;
    debinftype *effect = debtab[debkeydata[instance].effect_index];
    for (i32 i = 0; i < count; ++i) {
        effect->torus_keys3[i].time = stages[i].time;
        effect->torus_keys3[i].value = stages[i].red;
    }
}

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32) {
    i16 priority = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
    edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
    particle->render_priority = priority;
    debkeydata[particle->instance_id].render_priority = priority;
}

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edptl_damageflag_menu = eduiMenuCreate(70, 70, 200, 250, ed_fnt, cbPtlCancelDamageFlagMenu, "Damage Flags");
    if (edptl_damageflag_menu != NULL) {
        eduiMenuAddItem(edptl_damageflag_menu,
                        eduiItemToggleCreate(1, edblack, effect->field_2f2 & 1, 1, cbPtlChangeDamageFlags, "Good"));
        eduiMenuAddItem(edptl_damageflag_menu, eduiItemToggleCreate(2, edblack, (effect->field_2f2 >> 1) & 1, 2,
                                                                    cbPtlChangeDamageFlags, "Evil"));
    }
    eduiMenuAttach(menu, edptl_damageflag_menu);
    edptl_damageflag_menu->x = menu->x + 10;
    edptl_damageflag_menu->y = menu->y + 40;
}

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpp_nearest != -1) {
        i32 instance = edpp_ptls[edpp_nearest].instance_id;
        if (instance != -1) {
            debinftype *effect = debtab[debkeydata[instance].effect_index];
            for (i32 i = 0; i < 8; ++i) {
                effect->collision_keys[i].time = effect->width_keys[i].time;
                effect->collision_keys[i].value = effect->width_keys[i].value / 10000.0f;
            }
        }
    }
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest != -1 && edpp_ptls[edpp_nearest].instance_id != -1) {
        i32 type = debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index;
        debinftype *effect = debtab[type];
        i8 old_type = static_cast<i8>(effect->particle_type);
        if (old_type != item->data) {
            switch (item->data) {
                case 0:
                    edpp_ptls[edpp_nearest].render_priority = 20000;
                    break;
                case 2:
                    edpp_ptls[edpp_nearest].render_priority = static_cast<i16>(40000);
                    break;
                case 3:
                    edpp_ptls[edpp_nearest].render_priority = 30000;
                    break;
                case 7:
                    edpp_ptls[edpp_nearest].render_priority = 10000;
                    break;
            }
        }
        if (item->data == 7) {
            effect->time_group = 2;
        } else if (old_type == 7) {
            effect->time_group = 0;
        }
        if (item->data == 7 ||
            debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index]->particle_type == 7) {
            for (i32 index = 0; index < 512; ++index) {
                if (edpp_ptls[index].effect_index == type) {
                    edppPtlShelve(index);
                }
            }
            effect->particle_type = static_cast<u8>(item->data);
            GenericDebinfoDmaTypeUpdate(effect);
            for (i32 index = 0; index < 512; ++index) {
                edpp_particle_s *particle = &edpp_ptls[index];
                if (particle->effect_index != type || particle->instance_id != 99999) {
                    continue;
                }
                particle->instance_id = -1;
                AddDebrisEffect(&particle->instance_id, type, particle->position.x, particle->position.y,
                                particle->position.z);
                if (particle->instance_id == -1) {
                    particle->instance_id = 99999;
                    continue;
                }
                debkeydata[particle->instance_id].field_2f9 = 0;
                DebrisOrientation(particle->instance_id, particle->rotation_z, particle->rotation_y);
                DebrisEmitterOrientation(particle->instance_id, particle->emitter_rotation_z,
                                         particle->emitter_rotation_y, particle->emitter_rotation_x);
                DebrisStartOffset(particle->instance_id, particle->start_offset);
                DebrisSetTrigger(particle->instance_id, particle->switch_type, particle->switch_id,
                                 particle->switch_variable);
                DebrisReflectionOrientation(particle->instance_id, particle->reflection_rotation_z,
                                            particle->reflection_rotation_y, particle->reflection_offset,
                                            particle->reflection_bounce);
                DebrisSetFacing(particle->instance_id, particle->facing_mode, particle->facing_rotation_x,
                                particle->facing_rotation_y);
                DebrisSetGroupID(particle->instance_id, particle->render_group);
                DebrisSetRoomID(particle->instance_id, reinterpret_cast<nugscn_s *>(edpp_page_scene[particle->page]));
            }
        } else {
            effect->particle_type = static_cast<u8>(item->data);
        }
    }
    eduimenu_s *parent = menu->parent;
    if (parent) {
        eduiMenuDetach(menu);
    }
    if (menu->callback) {
        menu->callback(menu, parent);
    }
}

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edptl_quickdel_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbPtlCancelQuickDeleteMenu, "Quick Delete");
    if (edptl_quickdel_menu) {
        for (i32 type = 1; type < EDPP_MAX_TYPES; ++type) {
            debinftype *effect = debtab[type];
            if (!effect || effect->category != edpp_effect_list) {
                continue;
            }
            i32 index;
            for (index = 0; index < 512; ++index) {
                if (edpp_ptls[index].instance_id != 99999 && edpp_ptls[index].instance_id != -1 &&
                    edpp_ptls[index].effect_index == type) {
                    break;
                }
            }
            if (index < 512) {
                eduiMenuAddItem(edptl_quickdel_menu,
                                eduiItemSelCreate(type, edblack, 0, 1, cbPtlQuickDeleteType, effect->name));
            } else {
                eduiMenuAddItem(edptl_quickdel_menu,
                                eduiItemSelCreate(type, eddarkred, 0, 1, cbPtlQuickDeleteType, effect->name));
            }
        }
        eduiMenuAttach(menu, edptl_quickdel_menu);
        edptl_quickdel_menu->x = menu->x + 10;
        edptl_quickdel_menu->y = menu->y + 40;
    }
}

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edppDeleteEffect(item->data);
    eduimenu_s *parent = menu->parent;
    if (parent) {
        eduiMenuDetach(menu);
    }
    if (menu->callback) {
        menu->callback(menu, parent);
    }
}

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->sound_range = static_cast<edui_slider_s *>(item)->value;
}

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    if (item->highlighted) {
        debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index]->use_explicit_clip_box = 1;
    } else {
        debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index]->use_explicit_clip_box = 0;
    }
}

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->process_spheres = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    if (item->highlighted) {
        effect->field_2f2 |= item->data;
    } else {
        effect->field_2f2 &= ~item->data;
    }
}

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->sound_range_override = static_cast<edui_slider_s *>(item)->value;
}

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    if (textureselectmenu == NULL) {
        textureselectmenu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, NULL, "Texture Select");
        if (textureselectmenu == NULL) {
            return;
        }
        eduiMenuAddItem(textureselectmenu,
                        eduiItemTexturePickCreate(0, colours, cbPtlChangeTextureSelect, "Texture Select"));
        edui_texture_pick_s *texture = static_cast<edui_texture_pick_s *>(edui_last_item);
        NUMTL *material = NuMtlCreate(1);
        texture->material = material;
        NUMTL *source = DebMat[1];
        material->tex_id = source->tex_id;
        material->attribs.unknown_1_1_2 = source->attribs.unknown_1_1_2;
        material->attribs.unknown_1_4_8 = source->attribs.unknown_1_4_8;
        material->diffuse_color.r = source->diffuse_color.r;
        material->diffuse_color.g = source->diffuse_color.g;
        material->diffuse_color.b = source->diffuse_color.b;
        material->opacity = source->opacity;
        material->attribs.unknown_2_1_2 = source->attribs.unknown_2_1_2;
        material->attribs.alpha_mode = source->attribs.alpha_mode;
        material->attribs.z_mode = source->attribs.z_mode;
        material->attribs.unknown_0_64_128 = source->attribs.unknown_0_64_128;
        NuMtlUpdate(material);
        texture->uv_x[0] = (effect->texture_u0 - 524288.0f) * (1.0f / 256.0f);
        texture->uv_y[0] = (effect->texture_v0 - 524288.0f) * (1.0f / 256.0f);
        texture->uv_x[1] = (effect->texture_u1 - 524288.0f) * (1.0f / 256.0f);
        texture->uv_y[1] = (effect->texture_v1 - 524288.0f) * (1.0f / 256.0f);
    }
    if (textureselectmenu != NULL) {
        eduiMenuAttach(menu, textureselectmenu);
        textureselectmenu->x = menu->x + 10;
        textureselectmenu->y = menu->y + 40;
    }
}

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpp_nearest == -1) {
        return;
    }
    if (edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    effect->field_044 = static_cast<edui_slider_s *>(item)->value;
}

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpp_nearest == -1 || edpp_ptls[edpp_nearest].instance_id == -1) {
        return;
    }
    debinftype *effect = debtab[debkeydata[edpp_ptls[edpp_nearest].instance_id].effect_index];
    edui_texture_pick_s *texture = static_cast<edui_texture_pick_s *>(item);
    eduimenu_s *parent = menu->parent;
    effect->texture_u0 = texture->uv_x[0] * 256.0f + 524288.0f;
    effect->texture_v0 = texture->uv_y[0] * 256.0f + 524288.0f;
    effect->texture_u1 = texture->uv_x[1] * 256.0f + 524288.0f;
    effect->texture_v1 = texture->uv_y[1] * 256.0f + 524288.0f;
    if (parent != NULL) {
        eduiMenuDetach(menu);
    }
    if (menu->callback != NULL) {
        menu->callback(menu, parent);
    }
    GenericDebinfoDmaTypeUpdate(effect);
}

static void cbPtlInstanceSettingsMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32) {
    edpp_particle_s *particle = &edpp_ptls[edpp_nearest];
    particle->dynamic_priority = item->highlighted;
    debkeydata[particle->instance_id].timed_flags = item->highlighted;
}

static void cbPtlCancel(eduimenu_s *, eduimenu_s *) {
    edpp_active_menu = NULL;
}

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlcolmenu);
    ptlcolmenu = NULL;
    grad_item = NULL;
    grad_alpha_item = NULL;
}

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptljibmenu);
    ptljibmenu = NULL;
    grad_jib_x_freq_item = NULL;
    grad_jib_x_amp_item = NULL;
    grad_jib_y_freq_item = NULL;
    grad_jib_y_amp_item = NULL;
}

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlrotmenu);
    ptlrotmenu = NULL;
    grad_rot_item = NULL;
    grad_rot_min_item = NULL;
    grad_rot_max_item = NULL;
}

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(messagemenu);
    messagemenu = NULL;
}

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(collmenu);
    collmenu = NULL;
    coll_env_item = NULL;
}

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptldatamenu);
    ptldatamenu = NULL;
}

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlemitmenu);
    ptlemitmenu = NULL;
}

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlgravmenu);
    ptlgravmenu = NULL;
}

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlsizemenu);
    ptlsizemenu = NULL;
    grad_size_w_item = NULL;
    grad_size_h_item = NULL;
    grad_size_min_item = NULL;
    grad_size_max_item = NULL;
}

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptltypemenu);
    ptltypemenu = NULL;
}

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlgsortmenu);
    ptlgsortmenu = NULL;
}

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_torus_menu);
    edptl_torus_menu = NULL;
    torus_env1_item = NULL;
    torus_env2_item = NULL;
    torus_env3_item = NULL;
}

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlcutoffmenu);
    ptlcutoffmenu = NULL;
}

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_damage_menu);
    edptl_damage_menu = NULL;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(sscalemenu);
    sscalemenu = NULL;
}

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *) {
    if (edpp_create_type == -1 || debtab[edpp_create_type] == NULL) {
        return;
    }
    if (debtab[edpp_create_type]->name[0] != '\0') {
        eduiMenuDestroy(namemenu);
        namemenu = NULL;
        return;
    }
    u32 colours[4] = {0x800000c0, 0x80ff0000, 0x80808080, 0x80404040};
    messagemenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, cbCancelMessageMenu, "Message");
    if (messagemenu != NULL) {
        eduiMenuAddItem(messagemenu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Name Duplicate"));
        eduiMenuAttach(menu, messagemenu);
        messagemenu->x = menu->x + 10;
        messagemenu->y = menu->y + 40;
    }
}

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(effectlistmenu);
    effectlistmenu = NULL;
}

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlemitvelmenu);
    ptlemitvelmenu = NULL;
}

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlreadoutmenu);
    ptlreadoutmenu = NULL;
}

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *) {
    if (textureselectmenu) {
        eduiMenuDestroy(textureselectmenu);
        textureselectmenu = NULL;
    }
    eduiMenuDestroy(texturemenu);
    texturemenu = NULL;
}

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlvaremitmenu);
    ptlvaremitmenu = NULL;
}

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(etimemenu);
    etimemenu = NULL;
}

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(emittimemenu);
    emittimemenu = NULL;
}

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlstartvelmenu);
    ptlstartvelmenu = NULL;
}

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(ptlvarstartmenu);
    ptlvarstartmenu = NULL;
}

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(changegenratemenu);
    changegenratemenu = NULL;
}

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_damageflag_menu);
    edptl_damageflag_menu = NULL;
}

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_quickdel_menu);
    edptl_quickdel_menu = NULL;
}

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edptl_instancesettings_menu);
    edptl_instancesettings_menu = NULL;
}
