#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"

#include <stdio.h>
#include <string.h>

void edpartDoInput(nupad_s *pad);
void edpartDetermineNearest(f32 distance);
void edpartDrawCursor();
void edpartHighlightNearest();
void edpartScaleType(i32 type, f32 scale);
void edpartInitType(i32 type);
void edpartDestroy(i32 index);
void edpartPlace(i32 index, nuvec_s *position);
void edpartCreate(nuvec_s *position, i32 type);
void edpartMultipleCopyCopy();
void edpartMultipleCopyClear();
i32 edpartSaveEffects(char *path, char page);

extern "C" {
    i32 edpart_set_part = 5;
    i32 edpart_filter;
    char edpart_filter_string[16] = "PART";
    i32 edpart_first_time_this_level;
    i32 edpart_curr;
    i32 edpart_create_type = -1;
    i32 edpart_nearest_orphans;
    i32 edpart_nearest_duplicates;
    i8 edpart_effect_list;
    i32 edpart_emitrotx;
    i32 edpart_emitroty;
    i32 edpart_emitrotz;
    i32 edpart_roty;
    i32 edpart_rotz;
    f32 edpart_offset;
    i32 edpart_dpad_mode;
    i32 edpart_copy_mode;
    i32 edpart_copy_enclosed;
    i32 edpart_copy_source[8];
    i32 edpart_copy_source_count;
    NUVEC edpart_copy_source_vec;
    i32 edpart_copyrotz;
    f32 edpart_copy_size = 0.2f;
    i32 edpart_copyroty;
    i32 edpart_snap_enabled;
    i32 edpart_refroty;
    i32 edpart_refrotz;
    i32 edpart_cam_ax;
    i32 edpart_cam_ay;
    extern NUVEC edpart_cam_pos;
    i32 edpart_num_orphans;
    i32 edpart_readout;
    f32 edpart_scale_factor = 1.0f;
    NUMTL *edpart_mtl;
    NUMTL *edpart_boxmtl;
    extern i32 edpart_nearest;
    extern part_emit_s *edpart_nearest_emit;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern i32 part_platimpactcnt;
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern char edbits_general_save_directory[256], edbits_general_save_name[256], edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    extern i32 edbits_override_backups;
    void ResetParts(void);
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_type_menu;
    void eduiMenuRender(eduimenu_s *menu);
    eduimenu_s *edpart_active_menu;
    eduimenu_s *edpart_opt_menu;
    eduimenu_s *edpart_sscale_menu;
    eduimenu_s *edpart_message_menu;
    eduimenu_s *edpart_name_menu;
    eduimenu_s *edpart_data_menu;
    eduimenu_s *edpart_scaletype_menu;
    eduimenu_s *edpart_switchtype_menu;
    eduimenu_s *edpart_switch_menu;
    eduimenu_s *edpart_soundcontrol_menu;
    eduimenu_s *edpart_soundid_menu;
    eduimenu_s *edpart_soundx_menu;
    eduimenu_s *edpart_sounds_menu;
    eduimenu_s *edpart_debrisscale_menu;
    eduimenu_s *edpart_partindex_menu;
    eduimenu_s *edpart_impactpart_menu;
    eduimenu_s *edpart_debrisindex_menu;
    eduimenu_s *edpart_diedebris_menu;
    eduimenu_s *edpart_impactdebris_menu;
    eduimenu_s *edpart_emitterdebris_menu;
    eduimenu_s *edpart_trail2debris_menu;
    eduimenu_s *edpart_trail1debris_menu;
    eduimenu_s *edpart_debrissettings_menu;
    eduimenu_s *edpart_cutoff_menu;
    eduimenu_s *edpart_emittime_menu;
    eduimenu_s *edpart_changegenrate_menu;
    eduimenu_s *edpart_varemit_menu;
    eduimenu_s *edpart_varstart_menu;
    eduimenu_s *edpart_grav_menu;
    eduimenu_s *edpart_emitvel_menu;
    eduimenu_s *edpart_emit_menu;
    eduimenu_s *edpart_instancescale_menu;
    eduimenu_s *edpart_tint_menu;
    eduimenu_s *edpart_instanceflags_menu;
    eduimenu_s *edpart_instorient_menu;
    eduimenu_s *edpart_maxlife_menu;
    eduimenu_s *edpart_instancesettings_menu;
    eduimenu_s *edpart_instanceorphans_menu;
    eduimenu_s *edpart_thingsinstance_menu;
    eduimenu_s *edpart_worldinstance_menu;
    eduimenu_s *edpart_instance_menu;
    eduimenu_s *edpart_leveltype_menu;
    eduimenu_s *edpart_generaltype_menu;
    eduimenu_s *edpart_type_menu;
}

static __attribute__((always_inline, optimize("O3"))) inline void edpartRemoveInstance(part_typedesc_s *type,
                                                                                       i32 index) {
    for (i32 next = index; next < 7; ++next) {
        type->effect_ids[next] = type->effect_ids[next + 1];
        type->effect_pages[next] = type->effect_pages[next + 1];
    }
    --type->variant_count;
    type->effect_ids[7] = -1;
    type->effect_pages[7] = -1;
}

static inline void edpartFinishMenu(eduimenu_s *menu) {
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

static inline void edpartAddType(eduimenu_s *menu, i8 page) {
    if (part_types_used < 128) {
        part_typedesc_s *scan = part_types;
        for (i32 index = 0; index < 128; ++index, ++scan) {
            if (scan->name[0] == '\0') {
                part_typedesc_s *type = &part_types[index];
                sprintf(type->name, "New%d", index);
                edpartInitType(index);
                type->field_b3 = page;
                type->page = page;
                type->effect_ids[0] = -1;
                type->effect_pages[0] = -1;
                type->effect_ids[1] = -1;
                type->effect_pages[1] = -1;
                type->effect_ids[2] = -1;
                type->effect_pages[2] = -1;
                type->effect_ids[3] = -1;
                type->effect_pages[3] = -1;
                type->effect_ids[4] = -1;
                type->effect_pages[4] = -1;
                type->effect_ids[5] = -1;
                type->effect_pages[5] = -1;
                type->effect_ids[6] = -1;
                type->effect_pages[6] = -1;
                type->effect_ids[7] = -1;
                type->effect_pages[7] = -1;
                part_page_on[page] = 1;
                ++part_types_used;
                part_page_used[page] = 1;
                part_scene[page] = page == 0 ? edbits_things_scene : edbits_base_scene;
                part_scene_pageid[page] = page;
                edpart_create_type = index;
                type->field_160 = 1.0f;
                type->field_164 = 1.0f;
                type->field_168 = 1.0f;
                break;
            }
        }
    }
    edpartFinishMenu(menu);
}

static eduiitem_s *edpart_nullobject_highlight;
static char nullobjectname[16] = "NULL instance";
static i32 edpart_count;
static i32 edpart_superscale = 1;
static NUVEC edpart_entry_position;
extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_name_menu;
    extern i32 edpart_nearest;
    extern i32 edpart_create_type;
    extern f32 edpart_scale_factor;
    extern part_typedesc_s part_types[128];
}

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *, u32);
static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32);

extern "C" {
    i32 edpart_which_scene = 1;
    i32 edpart_set_debris;
    i32 edpart_part_list;
    i32 edpart_particle_list;
}

static inline void edpartRefreshEmitterSounds() {
    i32 effect = edpart_nearest_emit->effect_id;
    part_typedesc_s *type = &part_types[effect];
    for (i32 emitter = 0; emitter < 40; ++emitter) {
        if (part_emits[emitter].effect_id == effect) {
            part_emits[emitter].sounds_active = 0;
            // Both original setters test the emitter index here, not the sound index.
            for (i32 sound = 0; emitter < 4; ++sound) {
                if (type->sounds[sound] != -1 && type->sound_modes[sound] != 0) {
                    part_emits[emitter].sounds_active = 1;
                    break;
                }
            }
        }
    }
}

static void edpartInit() {
    edpart_first_time_this_level = 1;
    edpart_mtl = NuMtlCreate3D(1);
    edpart_mtl->opacity = 1.0f;
    edpart_mtl->diffuse_color.r = 0.5f;
    edpart_mtl->diffuse_color.g = 0.5f;
    edpart_mtl->diffuse_color.b = 0.5f;
    edpart_mtl->attribs.alpha_mode = 0;
    edpart_mtl->attribs.cull_mode = 2;
    edpart_mtl->attribs.z_mode = 0;
    NuMtlUpdate(edpart_mtl);
    edpart_boxmtl = NuMtlCreate(1);
    edpart_boxmtl->opacity = 1.0f;
    edpart_boxmtl->diffuse_color.r = 0.5f;
    edpart_boxmtl->diffuse_color.g = 0.5f;
    edpart_boxmtl->diffuse_color.b = 0.5f;
    edpart_boxmtl->attribs.alpha_mode = 0;
    edpart_boxmtl->attribs.cull_mode = 2;
    edpart_boxmtl->attribs.z_mode = 3;
    NuMtlUpdate(edpart_boxmtl);
    edpart_nearest = -1;
    edpart_curr = -1;
    edpart_create_type = -1;
    edpart_effect_list = 0;
    edpart_emitrotz = 0;
    edpart_emitroty = 0;
    edpart_emitrotx = 0;
}
static i32 edpartProc(float delta_time, nupad_s *pad) {
    edpart_count += 5;
    if (edpart_active_menu) {
        eduiMenuProcess(edpart_active_menu, delta_time, pad);
        return 0;
    }
    edpartDoInput(pad);
    edpartDetermineNearest(1.0f);
    return (pad->digital_buttons_pressed >> 11) & 1;
}
static void edpartApply() {
}
static void edpartClose() {
    eduiMenuDestroy(edpart_opt_menu);
}
static void edpartEnter() {
    NUVEC origin = {0.0f, 0.0f, 0.0f};
    if (edmainQueryLocVec())
        edpart_entry_position = *edmainQueryLocVec();
    else {
        edpart_entry_position.x = global_camera.mtx.m30;
        edpart_entry_position.y = global_camera.mtx.m31;
        edpart_entry_position.z = global_camera.mtx.m32;
    }
    if (edpart_first_time_this_level) {
        if (edmainQueryLocVec())
            edcamSetPosAng(edmainQueryLocVec(), 0, 0);
        else
            edcamSetPosAng(&origin, 0, 0);
        edpart_first_time_this_level = 0;
    }
    edpart_nearest = -1;
}
static void edpartRender() {
    edcamSet();
    edpartDrawCursor();
    edpartHighlightNearest();
    if (edpart_active_menu)
        eduiMenuRender(edpart_active_menu);
}
static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_type_menu = NULL;
    edpart_active_menu = NULL;
    edpart_create_type = item->data;
    edpart_effect_list = part_types[edpart_create_type].field_b3;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartCopyType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (part_types_used < 128 && edpart_create_type != -1) {
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].name[0] == '\0') {
                part_type_s *source = &part_types[edpart_create_type];
                part_type_s *copy = &part_types[index];
                *copy = *source;
                size_t name_length = strlen(source->name);
                if (name_length <= 12) {
                    sprintf(copy->name, "%s%03d", source->name, index);
                } else {
                    char name[16];
                    memcpy(name, source->name, name_length + 1);
                    name[12] = '\0';
                    sprintf(copy->name, "%s%03d", name, index);
                }
                ++part_types_used;
                edpart_create_type = index;
                break;
            }
        }
    }
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartDataMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_data_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelDataMenu, "Data Menu");
    if (edpart_data_menu != NULL) {
        if (edpart_create_type != -1)
            eduiMenuAddItem(edpart_data_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartChangeNameMenu, "Type Name..."));
        else
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Type Name..."));
        if (edpart_create_type != -1) {
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartCopyType, "Copy Type"));
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartDeleteType, "Delete Type"));
            char *move_name;
            if (part_types[edpart_create_type].field_b3)
                move_name = "Move Type to General List";
            else
                move_name = "Move Type to Level List";
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartMoveList, move_name));
        } else {
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Copy Type"));
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Delete Type"));
            eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Move Type"));
        }
        eduiMenuAddItem(edpart_data_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartFileSaveEffectsGeneral, "Save General List"));
        eduiMenuAddItem(edpart_data_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartFileSaveEffectsLevel, "Save Level List"));
        eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartFileSaveEffects, "Save All"));
        eduiMenuAddItem(edpart_data_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartFileLoadEffects, "Load All"));
        eduiMenuAttach(menu, edpart_data_menu);
        edpart_data_menu->x = menu->x + 10;
        edpart_data_menu->y = menu->y + 40;
    }
}
static void edpartEmitMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_emit_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelEmitMenu, "Emitter Settings");
        if (edpart_emit_menu != NULL) {
            eduiMenuAddItem(edpart_emit_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartEmitVelMenu, "Emitter Vel..."));
            eduiMenuAddItem(edpart_emit_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartGravMenu, "Gravity..."));
            eduiMenuAddItem(edpart_emit_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartVarStartMenu, "Random Start..."));
            eduiMenuAddItem(edpart_emit_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartVarEmitMenu, "Random Emit..."));
            eduiMenuAddItem(edpart_emit_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartChangeGenRateMenu, "Emits per Sec..."));
            eduiMenuAddItem(edpart_emit_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartEmitTimeMenu, "Emitter Timing..."));
            eduiMenuAddItem(edpart_emit_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartCutOffMenu, "Radii..."));
        }
        eduiMenuAttach(menu, edpart_emit_menu);
        edpart_emit_menu->x = menu->x + 10;
        edpart_emit_menu->y = menu->y + 40;
    }
}
static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_grav_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelGravMenu, "Gravity");
        if (edpart_grav_menu != NULL) {
            eduiMenuAddItem(edpart_grav_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeGrav, edpart_superscale * -10.0f,
                                                 edpart_superscale * 20.0f, edpart_nearest_type->gravity * 0.5f,
                                                 "Gravity"));
            eduiMenuAddItem(edpart_grav_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeBounce, 0.0f, edpart_superscale,
                                                 edpart_nearest_type->bounce, "Bounce"));
            eduiMenuAttach(menu, edpart_grav_menu);
            edpart_grav_menu->x = menu->x + 10;
            edpart_grav_menu->y = menu->y + 40;
        }
    }
}
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32) {
    part_type_s *type = &part_types[edpart_create_type];
    type->page = !type->field_b3;
    type->field_b3 = type->page;
    edpart_effect_list = type->page;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartTintMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_tint_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelTintMenu, "Instance Tint");
        if (edpart_tint_menu != NULL) {
            eduiMenuAddItem(edpart_tint_menu, eduiItemSliderCreate(0, edblack, 0, edpartChangeTintR, 0.0f, 2.0f,
                                                                   edpart_nearest_type->field_160, "Red Tint"));
            eduiMenuAddItem(edpart_tint_menu, eduiItemSliderCreate(0, edblack, 0, edpartChangeTintG, 0.0f, 2.0f,
                                                                   edpart_nearest_type->field_164, "Green Tint"));
            eduiMenuAddItem(edpart_tint_menu, eduiItemSliderCreate(0, edblack, 0, edpartChangeTintB, 0.0f, 2.0f,
                                                                   edpart_nearest_type->field_168, "Blue Tint"));
            eduiMenuAttach(menu, edpart_tint_menu);
            edpart_tint_menu->x = menu->x + 10;
            edpart_tint_menu->y = menu->y + 40;
        }
    }
}
static void edpartTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_type_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelTypeMenu, "Emitter Type");
    if (edpart_type_menu != NULL) {
        eduiMenuAddItem(edpart_type_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralTypeMenu, "General List..."));
        eduiMenuAddItem(edpart_type_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartLevelTypeMenu, "Level List..."));
        eduiMenuAttach(menu, edpart_type_menu);
        edpart_type_menu->x = menu->x + 10;
        edpart_type_menu->y = menu->y + 40;
    }
}
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->gravity = static_cast<edui_slider_s *>(item)->value * 2.0f;
}
static void edpartChangeName(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_create_type != -1) {
        char *name = static_cast<edui_textpicker_s *>(item)->value;
        NuStrNCpy(part_types[edpart_create_type].name, name, 16);
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].effect_id == edpart_create_type)
                NuStrNCpy(part_emits[index].name, name, 16);
        }
    }
}
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_cutoff_menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edpartCancelCutOffMenu, "Radii");
        if (edpart_cutoff_menu != NULL) {
            eduiMenuAddItem(edpart_cutoff_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeCutOff, 0.0f, edpart_superscale * 25.0f,
                                                 edpart_nearest_type->maximum_distance, "CutOff Rad"));
            eduiMenuAttach(menu, edpart_cutoff_menu);
            edpart_cutoff_menu->x = menu->x + 10;
            edpart_cutoff_menu->y = menu->y + 40;
        }
    }
}
static void edpartDeleteType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == &part_types[edpart_create_type]) {
        edpart_nearest_type = NULL;
        edpart_nearest = -1;
        edpart_nearest_emit = NULL;
    }
    for (i32 emitter = 0; emitter < 40; ++emitter) {
        if (part_emits[emitter].effect_id == edpart_create_type)
            edpartDestroy(emitter);
    }
    edpartDetermineNearest(1.0f);
    part_types[edpart_create_type].name[0] = '\0';
    part_types[edpart_create_type].effect_ids[0] = -1;
    --part_types_used;
    edpartFinishMenu(menu);
    edpart_create_type = -1;
}
static void edpartSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_sscale_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelSScaleMenu, "Super Scale");
    if (edpart_sscale_menu != NULL) {
        eduiMenuAddItem(edpart_sscale_menu, eduiItemSliderCreateInt(0, edblack, 0, edpartChangeSScale, 1, 99,
                                                                    edpart_superscale, "Super Scale"));
        eduiMenuAttach(menu, edpart_sscale_menu);
        edpart_sscale_menu->x = menu->x + 10;
        edpart_sscale_menu->y = menu->y + 40;
    }
}
static void edpartSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_soundid_menu = NULL;
    u32 slot = static_cast<u32>(item->data) >> 16;
    i32 value = item->data & 0xffff;
    if (value == 9999)
        value = -1;
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->sounds[slot] = value;
    edpartRefreshEmitterSounds();
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartSoundXMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        char title[16];
        sprintf(title, "Sound %d Menu", item->data + 1);
        edpart_soundx_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelSoundXMenu, title);
        if (edpart_soundx_menu != NULL) {
            eduiMenuAddItem(edpart_soundx_menu,
                            eduiItemSelCreate(item->data, edblack, 0, 0, edpartSoundIDMenu, "Sound ID..."));
            eduiMenuAddItem(edpart_soundx_menu,
                            eduiItemSelCreate(item->data, edblack, 0, 0, edpartSoundControlMenu, "Sound Control..."));
            eduiMenuAttach(menu, edpart_soundx_menu);
            edpart_soundx_menu->x = menu->x + 10;
            edpart_soundx_menu->y = menu->y + 40;
        }
    }
}
static void edpartSoundsMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_sounds_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelSoundsMenu, "Attached Sounds");
        if (edpart_sounds_menu != NULL) {
            char text[16];
            sprintf(text, "Sound %d...", 1);
            eduiMenuAddItem(edpart_sounds_menu, eduiItemSelCreate(0, edblack, 0, 0, edpartSoundXMenu, text));
            sprintf(text, "Sound %d...", 2);
            eduiMenuAddItem(edpart_sounds_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartSoundXMenu, text));
            sprintf(text, "Sound %d...", 3);
            eduiMenuAddItem(edpart_sounds_menu, eduiItemSelCreate(2, edblack, 0, 0, edpartSoundXMenu, text));
            sprintf(text, "Sound %d...", 4);
            eduiMenuAddItem(edpart_sounds_menu, eduiItemSelCreate(3, edblack, 0, 0, edpartSoundXMenu, text));
            eduiMenuAttach(menu, edpart_sounds_menu);
            edpart_sounds_menu->x = menu->x + 10;
            edpart_sounds_menu->y = menu->y + 40;
        }
    }
}
static void edpartSwitchMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest != -1 && part_emits[edpart_nearest].instance_id != -1) {
        edpart_switch_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelSwitchMenu, "Switch Menu");
        if (edpart_switch_menu != NULL) {
            eduiMenuAddItem(edpart_switch_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartSwitchTypeMenu, "Switch Type..."));
            eduiMenuAddItem(edpart_switch_menu,
                            eduiItemSliderCreateInt(0, edblack, 0, edpartSetSwitchId, 0, 128,
                                                    part_emits[edpart_nearest].switch_id, "Switch ID"));
            eduiMenuAttach(menu, edpart_switch_menu);
            edpart_switch_menu->x = menu->x + 10;
            edpart_switch_menu->y = menu->y + 40;
        }
    }
}
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_168 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_164 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_160 = static_cast<edui_slider_s *>(item)->value;
}
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_emitvel_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelEmitVelMenu, "Emitter Vel");
        if (edpart_emitvel_menu != NULL) {
            eduiMenuAddItem(edpart_emitvel_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeEmitVel, -(edpart_superscale * 10.0f),
                                                 edpart_superscale * 20.0f, edpart_nearest_type->speed, "Emitter Vel"));
            eduiMenuAttach(menu, edpart_emitvel_menu);
            edpart_emitvel_menu->x = menu->x + 10;
            edpart_emitvel_menu->y = menu->y + 40;
        }
    }
}
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_emit)
        edpart_nearest_emit->field_46 = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartSoundIDMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_soundid_menu = eduiMenuCreate(70, 70, 250, 200, ed_fnt, edpartCancelSoundIDMenu, "Sound ID");
        if (edpart_soundid_menu != NULL) {
            eduiMenuAddItem(edpart_soundid_menu, eduiItemCheckCreate((item->data << 16) + 9999, edblack,
                                                                     edpart_nearest_type->sounds[item->data] == -1, 0,
                                                                     edpartSetSoundID, "NONE"));
            for (i32 sound = 0; sound < 1600; ++sound) {
                if (g_soundInfo[sound].sfx_name != NULL) {
                    if (edpart_nearest_type->sounds[item->data] == sound) {
                        eduiMenuAddItem(edpart_soundid_menu,
                                        eduiItemCheckCreate((item->data << 16) + sound, edblack, 1, 1, edpartSetSoundID,
                                                            const_cast<char *>(g_soundInfo[sound].sfx_name)));
                        edpart_soundid_menu->selected = edui_last_item;
                    } else {
                        eduiMenuAddItem(edpart_soundid_menu,
                                        eduiItemCheckCreate((item->data << 16) + sound, edblack, 0, 1, edpartSetSoundID,
                                                            const_cast<char *>(g_soundInfo[sound].sfx_name)));
                    }
                }
            }
            eduiMenuAttach(menu, edpart_soundid_menu);
            edpart_soundid_menu->x = menu->x + 10;
            edpart_soundid_menu->y = menu->y + 40;
        }
    }
}
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_varemit_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelVarEmitMenu, "Emitter Variation");
        if (edpart_varemit_menu != NULL) {
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.x, "Rand Emit X"));
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(1, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.y, "Rand Emit Y"));
            eduiMenuAddItem(edpart_varemit_menu,
                            eduiItemSliderCreate(2, edblack, 0, edpartChangeVarEmit, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->velocity_random.z, "Rand Emit Z"));
            eduiMenuAttach(menu, edpart_varemit_menu);
            edpart_varemit_menu->x = menu->x + 10;
            edpart_varemit_menu->y = menu->y + 40;
        }
    }
}
static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32) {
    edpartAddType(menu, 1);
}
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->bounce = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->maximum_distance = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_superscale = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_emittime_menu = eduiMenuCreate(70, 70, 260, 300, ed_fnt, edpartCancelEmitTimeMenu, "Emitter Timing");
        if (edpart_emittime_menu != NULL) {
            eduiMenuAddItem(edpart_emittime_menu,
                            eduiItemToggleCreate(0x400000, edblack, (edpart_nearest_type->flags >> 22) & 1, 1,
                                                 edpartChangeInstanceFlag, "Trigger effect inside cutoff"));
            eduiMenuAddItem(edpart_emittime_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOn, 0.01f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->emission_period, "On Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOnRan, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->emission_period_random, "Random On Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOff, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->emission_pause, "Off Time"));
            eduiMenuAddItem(edpart_emittime_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeIvalOffRan, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->emission_pause_random, "Random Off Time"));
            eduiMenuAttach(menu, edpart_emittime_menu);
            edpart_emittime_menu->x = menu->x + 10;
            edpart_emittime_menu->y = menu->y + 40;
        }
    }
}
static void edpartInstanceMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_instance_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelInstanceMenu, "Instance Select");
    if (edpart_instance_menu != NULL && edpart_nearest_type != NULL) {
        edpart_nullobject_highlight = eduiItemToggleCreate(9999, edblack, edpart_nearest_type->effect_ids[0] == 9999, 2,
                                                           edpartSetInstanceType, nullobjectname);
        eduiMenuAddItem(edpart_instance_menu, edpart_nullobject_highlight);
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_instance_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartWorldInstanceMenu, "World Scene..."));
        else
            eduiMenuAddItem(edpart_instance_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "World Scene..."));
        eduiMenuAddItem(edpart_instance_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartThingsInstanceMenu, "Things Scene..."));
        if (edpart_nearest_orphans == 0 && edpart_nearest_duplicates == 0)
            eduiMenuAddItem(edpart_instance_menu,
                            eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Instance Orphans/Dupes..."));
        else
            eduiMenuAddItem(edpart_instance_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceOrphansMenu,
                                                                    "Instance Orphans/Dupes..."));
        eduiMenuAddItem(edpart_instance_menu, eduiItemCheckCreate(0, edblack, edpart_nearest_type->variant_mode == 0, 1,
                                                                  edpartSetDistribution, "Random Distribution"));
        eduiMenuAddItem(edpart_instance_menu, eduiItemCheckCreate(1, edblack, edpart_nearest_type->variant_mode == 1, 1,
                                                                  edpartSetDistribution, "Sequential Distribution"));
        eduiMenuAttach(menu, edpart_instance_menu);
        edpart_instance_menu->x = menu->x + 10;
        edpart_instance_menu->y = menu->y + 40;
    }
}
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_filter = item->highlighted;
}
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_varstart_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelVarStartMenu, "Random Start");
        if (edpart_varstart_menu != NULL) {
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.x, "Rand Start X"));
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(1, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.y, "Rand Start Y"));
            eduiMenuAddItem(edpart_varstart_menu,
                            eduiItemSliderCreate(2, edblack, 0, edpartChangeVarStart, 0.0f, edpart_superscale * 5.0f,
                                                 edpart_nearest_type->position_random.z, "Rand Start Z"));
            eduiMenuAttach(menu, edpart_varstart_menu);
            edpart_varstart_menu->x = menu->x + 10;
            edpart_varstart_menu->y = menu->y + 40;
        }
    }
}
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edpart_active_menu = NULL;
    edpart_opt_menu = NULL;
}
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->speed = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_rate = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->data == 0)
            edpart_nearest_type->velocity_random.x = static_cast<edui_slider_s *>(item)->value;
        else if (item->data == 1)
            edpart_nearest_type->velocity_random.y = static_cast<edui_slider_s *>(item)->value;
        else if (item->data == 2)
            edpart_nearest_type->velocity_random.z = static_cast<edui_slider_s *>(item)->value;
    }
}
static void edpartDieDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 4;
    edpart_diedebris_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelDieDebrisMenu, "Die Debris");
    if (edpart_diedebris_menu != NULL) {
        eduiMenuAddItem(edpart_diedebris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_diedebris_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelDebrisIndexMenu : NULL,
                                          "Level List..."));
    }
    eduiMenuAttach(menu, edpart_diedebris_menu);
    edpart_diedebris_menu->x = menu->x + 10;
    edpart_diedebris_menu->y = menu->y + 40;
}
static void edpartLevelTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_leveltype_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelLevelTypeMenu, "Emitter Type (Level)");
    if (edpart_leveltype_menu != NULL) {
        if (part_types_used < 128)
            eduiMenuAddItem(edpart_leveltype_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartAddLevelType, "Add Type"));
        else
            eduiMenuAddItem(edpart_leveltype_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Add Type"));
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].name[0] != '\0' && part_types[index].field_b3 == 1) {
                if (index == edpart_create_type) {
                    eduiMenuAddItem(edpart_leveltype_menu,
                                    eduiItemCheckCreate(index, edblack, 1, 1, edpartSelType, part_types[index].name));
                    edpart_leveltype_menu->selected = edui_last_item;
                } else {
                    eduiMenuAddItem(edpart_leveltype_menu,
                                    eduiItemCheckCreate(index, edblack, 0, 1, edpartSelType, part_types[index].name));
                }
            }
        }
        eduiMenuAttach(menu, edpart_leveltype_menu);
        edpart_leveltype_menu->x = menu->x + 10;
        edpart_leveltype_menu->y = menu->y + 40;
    }
}
static void edpartScaleTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest != -1) {
        edpart_scaletype_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelScaleTypeMenu, "Scale Type");
        if (edpart_scaletype_menu != NULL) {
            eduiMenuAddItem(edpart_scaletype_menu, eduiItemSliderCreate(0, edblack, 0, edpartSetScaleFactor, 0.1f, 9.9f,
                                                                        edpart_scale_factor, "Scale Factor"));
            eduiMenuAddItem(edpart_scaletype_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartApplyScaleType, "Apply Scale to Type"));
            eduiMenuAttach(menu, edpart_scaletype_menu);
            edpart_scaletype_menu->x = menu->x + 10;
            edpart_scaletype_menu->y = menu->y + 40;
        }
    }
}
static void edpartSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_switchtype_menu = NULL;
    if (edpart_nearest_emit != NULL) {
        edpart_nearest_emit->switch_type = item->data;
        if (edpart_nearest_emit->switch_type == 0)
            edpart_nearest_emit->switch_state = 1;
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32) {
    edpartAddType(menu, 0);
}
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type)
        edpartScaleType(edpart_nearest_emit->effect_id, edpart_scale_factor);
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_data_menu);
    edpart_data_menu = NULL;
}
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emit_menu);
    edpart_emit_menu = NULL;
}
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_grav_menu);
    edpart_grav_menu = NULL;
}
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_tint_menu);
    edpart_tint_menu = NULL;
}
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_type_menu);
    edpart_type_menu = NULL;
}
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_create_type != -1) {
        part_typedesc_s *type = &part_types[edpart_create_type];
        edpart_name_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelChangeNameMenu, "Type Name");
        if (edpart_name_menu != NULL) {
            eduiMenuAddItem(edpart_name_menu, eduiItemTextPickCreate(0, edblack, edpartChangeName, "Name: "));
            strcpy(static_cast<edui_textpicker_s *>(edui_last_item)->value, type->name);
            static_cast<edui_textpicker_s *>(edui_last_item)->max_length = 15;
            eduiMenuAttach(menu, edpart_name_menu);
            edpart_name_menu->x = menu->x + 10;
            edpart_name_menu->y = menu->y + 40;
        }
    }
}
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->data == 0)
            edpart_nearest_type->position_random.x = static_cast<edui_slider_s *>(item)->value;
        else if (item->data == 1)
            edpart_nearest_type->position_random.y = static_cast<edui_slider_s *>(item)->value;
        else if (item->data == 2)
            edpart_nearest_type->position_random.z = static_cast<edui_slider_s *>(item)->value;
    }
}
static void edpartImpactPartMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_part = 5;
    edpart_impactpart_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelImpactPartMenu, "Impact Part");
    if (edpart_impactpart_menu != NULL) {
        eduiMenuAddItem(edpart_impactpart_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralPartIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_impactpart_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelPartIndexMenu : NULL,
                                          "Level List..."));
    }
    eduiMenuAttach(menu, edpart_impactpart_menu);
    edpart_impactpart_menu->x = menu->x + 10;
    edpart_impactpart_menu->y = menu->y + 40;
}
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_scale_factor = static_cast<edui_slider_s *>(item)->value;
}
static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_switchtype_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelSwitchTypeMenu, "Switch Type");
    if (edpart_switchtype_menu != NULL) {
        eduiMenuAddItem(edpart_switchtype_menu,
                        eduiItemCheckCreate(0, edblack, part_emits[edpart_nearest].switch_type == 0, 1,
                                            edpartSetSwitchType, "None"));
        eduiMenuAddItem(edpart_switchtype_menu,
                        eduiItemCheckCreate(1, edblack, part_emits[edpart_nearest].switch_type == 1, 1,
                                            edpartSetSwitchType, "Global Switch"));
        eduiMenuAttach(menu, edpart_switchtype_menu);
        edpart_switchtype_menu->x = menu->x + 10;
        edpart_switchtype_menu->y = menu->y + 40;
    }
}
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period_random = static_cast<edui_slider_s *>(item)->value;
}
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL && edpart_set_part == 5) {
        edpart_nearest_type->impact_part = item->data;
    }
}
static void edpartDebrisScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_debrisscale_menu = eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelDebrisScaleMenu, "Debris Scale");
    if (edpart_debrisscale_menu != NULL && edpart_nearest_type != NULL) {
        eduiMenuAddItem(edpart_debrisscale_menu,
                        eduiItemSliderCreate(0, edblack, 0, edpartChangeDebrisScale, 0.0f, edpart_superscale * 10.0f,
                                             edpart_nearest_type->effect_scale, "Debris Scale"));
        eduiMenuAttach(menu, edpart_debrisscale_menu);
        edpart_debrisscale_menu->x = menu->x + 10;
        edpart_debrisscale_menu->y = menu->y + 40;
    }
}
static void edpartFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32) {
    ResetParts();
    memset(part_types, 0, sizeof(part_types));
    for (i32 type = 0; type < 128; ++type) {
#define EDPART_INIT_EFFECT(variant)                                                                                    \
    part_types[type].effect_ids[variant] = -1;                                                                         \
    part_types[type].effect_pages[variant] = -1;
        EDPART_INIT_EFFECT(0)
        EDPART_INIT_EFFECT(1)
        EDPART_INIT_EFFECT(2)
        EDPART_INIT_EFFECT(3)
        EDPART_INIT_EFFECT(4)
        EDPART_INIT_EFFECT(5)
        EDPART_INIT_EFFECT(6)
        EDPART_INIT_EFFECT(7)
#undef EDPART_INIT_EFFECT
    }
    part_types_used = 0;
    memset(part_emits, 0, 40 * sizeof(part_emit_s));
    for (i32 emitter = 0; emitter < 40; ++emitter)
        part_emits[emitter].effect_id = -1;
    part_emits_used = 0;
    memset(part_page_used, 0, sizeof(i32) * 8);
    memset(part_page_on, 0, sizeof(i32) * 8);
    memset(part_scene, 0, sizeof(NUGSCN *) * 32);
    typedef i32 ScenePageVector __attribute__((vector_size(16)));
    ScenePageVector blank_pages = {-1, -1, -1, -1};
    ScenePageVector *scene_pages = reinterpret_cast<ScenePageVector *>(__builtin_assume_aligned(part_scene_pageid, 16));
    scene_pages[0] = blank_pages;
    scene_pages[1] = blank_pages;
    scene_pages[2] = blank_pages;
    scene_pages[3] = blank_pages;
    scene_pages[4] = blank_pages;
    scene_pages[5] = blank_pages;
    scene_pages[6] = blank_pages;
    scene_pages[7] = blank_pages;
    part_platimpactcnt = 0;
    char path[256];
    char general_directory[256], general_name[256], general_extension[256];
    char level_directory[256], level_name[256], level_extension[256];
    if (!edbits_general_save_directory[0])
        __builtin_memcpy(general_directory, ".", 2);
    else
        strcpy(general_directory, edbits_general_save_directory);
    if (!edbits_general_save_name[0])
        __builtin_memcpy(general_name, "part", 5);
    else
        strcpy(general_name, edbits_general_save_name);
    if (!edbits_general_save_extension[0])
        __builtin_memcpy(general_extension, "par", 4);
    else
        strcpy(general_extension, edbits_general_save_extension);
    if (!edbits_level_save_directory[0])
        __builtin_memcpy(level_directory, ".", 2);
    else
        strcpy(level_directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        __builtin_memcpy(level_name, "part", 5);
    else
        strcpy(level_name, edbits_level_save_name);
    if (!edbits_level_save_extension[0])
        __builtin_memcpy(level_extension, "par", 4);
    else
        strcpy(level_extension, edbits_level_save_extension);
    sprintf(path, "%s\\%s.%s", general_directory, general_name, general_extension);
    if (NuFileExists(path))
        edpartLoadPage(path, 0, edbits_things_scene);
    sprintf(path, "%s\\%s.%s", level_directory, level_name, level_extension);
    if (NuFileExists(path)) {
        i32 page = edpartLoadPage(path, 1, edbits_base_scene);
        edpartStartPage(static_cast<i8>(page));
    }
    u32 colours[4] = {0x8000c000, 0x80ff0000, 0x80808080, 0x80404040};
    edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
    if (edpart_message_menu != NULL) {
        eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Loaded OK"));
        eduiMenuAttach(parent, edpart_message_menu);
        edpart_message_menu->x = parent->x + 10;
        edpart_message_menu->y = parent->y + 40;
    }
}
static inline void edpartSavePath(char *path, char *backup, bool level) {
    char directory[256], name[256], extension[256];
    char *save_directory = level ? edbits_level_save_directory : edbits_general_save_directory;
    char *save_name = level ? edbits_level_save_name : edbits_general_save_name;
    char *save_extension = level ? edbits_level_save_extension : edbits_general_save_extension;
    if (!save_directory[0])
        __builtin_memcpy(directory, ".", 2);
    else
        strcpy(directory, save_directory);
    if (!save_name[0])
        __builtin_memcpy(name, "part", 5);
    else
        strcpy(name, save_name);
    if (!save_extension[0])
        __builtin_memcpy(extension, "par", 4);
    else
        strcpy(extension, save_extension);
    sprintf(path, "%s\\%s.%s", directory, name, extension);
    sprintf(backup, "%s\\%s.%s.bak", directory, name, extension);
}

static inline void edpartSaveMessage(eduimenu_s *parent, const char *message, bool success) {
    u32 colours[4] = {success ? 0x8000c000u : 0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
    edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
    if (edpart_message_menu != NULL) {
        eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, const_cast<char *>(message)));
        eduiMenuAttach(parent, edpart_message_menu);
        edpart_message_menu->x = parent->x + 10;
        edpart_message_menu->y = parent->y + 40;
    }
}

static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    char general_directory[256], general_name[256], general_extension[256];
    char level_directory[256], level_name[256], level_extension[256];
    if (!edbits_general_save_directory[0])
        __builtin_memcpy(general_directory, ".", 2);
    else
        strcpy(general_directory, edbits_general_save_directory);
    if (!edbits_general_save_name[0])
        __builtin_memcpy(general_name, "part", 5);
    else
        strcpy(general_name, edbits_general_save_name);
    if (!edbits_general_save_extension[0])
        __builtin_memcpy(general_extension, "par", 4);
    else
        strcpy(general_extension, edbits_general_save_extension);
    if (!edbits_level_save_directory[0])
        __builtin_memcpy(level_directory, ".", 2);
    else
        strcpy(level_directory, edbits_level_save_directory);
    if (!edbits_level_save_name[0])
        __builtin_memcpy(level_name, "part", 5);
    else
        strcpy(level_name, edbits_level_save_name);
    if (!edbits_level_save_extension[0])
        __builtin_memcpy(level_extension, "par", 4);
    else
        strcpy(level_extension, edbits_level_save_extension);

    sprintf(path, "%s\\%s.%s", general_directory, general_name, general_extension);
    sprintf(backup, "%s\\%s.%s.bak", general_directory, general_name, general_extension);
    bool general_backup = edbits_override_backups || EdFileBackup(path, backup);
    i32 general_saved = edpartSaveEffects(path, 0);
    sprintf(path, "%s\\%s.%s", level_directory, level_name, level_extension);
    sprintf(backup, "%s\\%s.%s.bak", level_directory, level_name, level_extension);
    bool level_backup = edbits_override_backups || EdFileBackup(path, backup);
    bool level_saved = edpartSaveEffects(path, 1) != 0;
    if (!general_saved && !level_saved) {
        edpartSaveMessage(parent, "Both Saves Failed", false);
        return;
    }
    if (!general_saved) {
        edpartSaveMessage(parent, "General Save Failed", false);
        return;
    }
    if (!level_saved) {
        edpartSaveMessage(parent, "Level Save Failed", false);
        return;
    }
    if (!general_backup && !level_backup) {
        edpartSaveMessage(parent, "Saved OK - Both Backups Failed", false);
        return;
    }
    if (!general_backup) {
        edpartSaveMessage(parent, "Saved OK - General Backup Failed", false);
        return;
    }
    if (!level_backup) {
        edpartSaveMessage(parent, "Saved OK - Level Backup Failed", false);
        return;
    }
    edpartSaveMessage(parent, "Saved OK", true);
}
static void edpartGeneralTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_generaltype_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelGeneralTypeMenu, "Emitter Type (General)");
    if (edpart_generaltype_menu != NULL) {
        if (part_types_used < 128)
            eduiMenuAddItem(edpart_generaltype_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartAddGeneralType, "Add Type"));
        else
            eduiMenuAddItem(edpart_generaltype_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Add Type"));
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].name[0] != '\0' && part_types[index].field_b3 == 0) {
                if (index == edpart_create_type) {
                    eduiMenuAddItem(edpart_generaltype_menu,
                                    eduiItemCheckCreate(index, edblack, 1, 1, edpartSelType, part_types[index].name));
                    edpart_generaltype_menu->selected = edui_last_item;
                } else {
                    eduiMenuAddItem(edpart_generaltype_menu,
                                    eduiItemCheckCreate(index, edblack, 0, 1, edpartSelType, part_types[index].name));
                }
            }
        }
        eduiMenuAttach(menu, edpart_generaltype_menu);
        edpart_generaltype_menu->x = menu->x + 10;
        edpart_generaltype_menu->y = menu->y + 40;
    }
}
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->variant_mode = item->data;
}
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type == NULL)
        return;
    if (item->data == 9999) {
#define EDPART_CLEAR_INSTANCE(index)                                                                                   \
    edpart_nearest_type->effect_ids[index] = -1;                                                                       \
    edpart_nearest_type->effect_pages[index] = 1;
        EDPART_CLEAR_INSTANCE(0)
        EDPART_CLEAR_INSTANCE(1)
        EDPART_CLEAR_INSTANCE(2)
        EDPART_CLEAR_INSTANCE(3)
        EDPART_CLEAR_INSTANCE(4)
        EDPART_CLEAR_INSTANCE(5)
        EDPART_CLEAR_INSTANCE(6)
        EDPART_CLEAR_INSTANCE(7)
#undef EDPART_CLEAR_INSTANCE
        if (item->highlighted)
            edpart_nearest_type->effect_ids[0] = 9999;
        edpart_nearest_type->flags |= 0x10;
        edpart_nearest_type->variant_count = 0;
        return;
    }
    edpart_nullobject_highlight->highlighted = 0;
    if (item->highlighted) {
        if (edpart_nearest_type->variant_count < 8) {
            i32 index = edpart_nearest_type->variant_count;
            edpart_nearest_type->effect_ids[index] = item->data;
            edpart_nearest_type->effect_pages[index] = edpart_which_scene;
            ++edpart_nearest_type->variant_count;
        } else {
            item->highlighted = 0;
        }
    } else {
        i32 index;
#define EDPART_FIND_INSTANCE(slot)                                                                                     \
    if (edpart_nearest_type->effect_ids[slot] == item->data) {                                                         \
        index = slot;                                                                                                  \
        goto remove_instance;                                                                                          \
    }
        EDPART_FIND_INSTANCE(0)
        EDPART_FIND_INSTANCE(1)
        EDPART_FIND_INSTANCE(2)
        EDPART_FIND_INSTANCE(3)
        EDPART_FIND_INSTANCE(4)
        EDPART_FIND_INSTANCE(5)
        EDPART_FIND_INSTANCE(6)
        EDPART_FIND_INSTANCE(7)
#undef EDPART_FIND_INSTANCE
        return;
    remove_instance:
        for (i32 next = index + 1; next < 8; ++next) {
            edpart_nearest_type->effect_ids[next - 1] = edpart_nearest_type->effect_ids[next];
            edpart_nearest_type->effect_pages[next - 1] = edpart_nearest_type->effect_pages[next];
        }
        edpart_nearest_type->effect_ids[7] = -1;
        edpart_nearest_type->effect_pages[7] = -1;
        --edpart_nearest_type->variant_count;
    }
}
static void edpartSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_soundcontrol_menu = NULL;
    u32 data = item->data;
    u32 value = static_cast<u16>(data);
    u32 slot = data >> 16;
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->sound_modes[slot] = value == 9999 ? -1 : value;
    edpartRefreshEmitterSounds();
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
}
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_cutoff_menu);
    edpart_cutoff_menu = NULL;
}
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sscale_menu);
    edpart_sscale_menu = NULL;
}

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundx_menu);
    edpart_soundx_menu = NULL;
}

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sounds_menu);
    edpart_sounds_menu = NULL;
}

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switch_menu);
    edpart_switch_menu = NULL;
}

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32) {
    NuStrNCpy(edpart_filter_string, static_cast<edui_textpicker_s *>(item)->value, 16);
}

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartImpactDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 3;
    edpart_impactdebris_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelImpactDebrisMenu, "Impact Debris");
    if (edpart_impactdebris_menu != NULL) {
        eduiMenuAddItem(edpart_impactdebris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_impactdebris_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelDebrisIndexMenu : NULL,
                                          "Level List..."));
    }
    eduiMenuAttach(menu, edpart_impactdebris_menu);
    edpart_impactdebris_menu->x = menu->x + 10;
    edpart_impactdebris_menu->y = menu->y + 40;
}

static void edpartSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_soundcontrol_menu =
            eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelSoundControlMenu, "Sound Control");
        if (edpart_soundcontrol_menu != NULL) {
            eduiMenuAddItem(edpart_soundcontrol_menu,
                            eduiItemCheckCreate(static_cast<u32>(item->data) << 16, edblack,
                                                edpart_nearest_type->sound_modes[item->data] == 0, 1,
                                                edpartSetSoundControl, "Off"));
            if (edui_last_item->highlighted)
                edpart_soundcontrol_menu->selected = edui_last_item;
            eduiMenuAddItem(edpart_soundcontrol_menu,
                            eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 1, edblack,
                                                edpart_nearest_type->sound_modes[item->data] == 1, 1,
                                                edpartSetSoundControl, "On Edge"));
            if (edui_last_item->highlighted)
                edpart_soundcontrol_menu->selected = edui_last_item;
            eduiMenuAddItem(edpart_soundcontrol_menu,
                            eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 2, edblack,
                                                edpart_nearest_type->sound_modes[item->data] == 2, 1,
                                                edpartSetSoundControl, "Off Edge"));
            if (edui_last_item->highlighted)
                edpart_soundcontrol_menu->selected = edui_last_item;
            eduiMenuAddItem(edpart_soundcontrol_menu,
                            eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 3, edblack,
                                                edpart_nearest_type->sound_modes[item->data] == 3, 1,
                                                edpartSetSoundControl, "Per PART"));
            if (edui_last_item->highlighted)
                edpart_soundcontrol_menu->selected = edui_last_item;
            eduiMenuAddItem(edpart_soundcontrol_menu,
                            eduiItemCheckCreate((static_cast<u32>(item->data) << 16) + 4, edblack,
                                                edpart_nearest_type->sound_modes[item->data] == 4, 1,
                                                edpartSetSoundControl, "Continuous"));
            if (edui_last_item->highlighted)
                edpart_soundcontrol_menu->selected = edui_last_item;
            eduiMenuAttach(menu, edpart_soundcontrol_menu);
            edpart_soundcontrol_menu->x = menu->x + 10;
            edpart_soundcontrol_menu->y = menu->y + 40;
        }
    }
}

static void edpartTrail1DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 0;
    edpart_trail1debris_menu = eduiMenuCreate(70, 70, 300, 300, ed_fnt, edpartCancelTrail1DebrisMenu, "Trail 1 Debris");
    if (edpart_trail1debris_menu != NULL) {
        eduiMenuAddItem(edpart_trail1debris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_trail1debris_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelDebrisIndexMenu : NULL,
                                          "Level List..."));
        eduiMenuAddItem(edpart_trail1debris_menu,
                        eduiItemSliderCreateInt(
                            0, edblack, 0, edpartChangeDebrisPerSec, 0, static_cast<i32>(edpart_superscale * 1200.0f),
                            static_cast<i32>(edpart_nearest_type->trail_rates[0]), "Trail 1 particles per second"));
    }
    eduiMenuAttach(menu, edpart_trail1debris_menu);
    edpart_trail1debris_menu->x = menu->x + 10;
    edpart_trail1debris_menu->y = menu->y + 40;
}

static void edpartTrail2DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 1;
    edpart_trail2debris_menu = eduiMenuCreate(70, 70, 300, 300, ed_fnt, edpartCancelTrail2DebrisMenu, "Trail 2 Debris");
    if (edpart_trail2debris_menu != NULL) {
        eduiMenuAddItem(edpart_trail2debris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_trail2debris_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelDebrisIndexMenu : NULL,
                                          "Level List..."));
        eduiMenuAddItem(edpart_trail2debris_menu,
                        eduiItemSliderCreateInt(
                            0, edblack, 0, edpartChangeDebrisPerSec, 0, static_cast<i32>(edpart_superscale * 1200.0f),
                            static_cast<i32>(edpart_nearest_type->trail_rates[1]), "Trail 2 particles per second"));
    }
    eduiMenuAttach(menu, edpart_trail2debris_menu);
    edpart_trail2debris_menu->x = menu->x + 10;
    edpart_trail2debris_menu->y = menu->y + 40;
}

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitvel_menu);
    edpart_emitvel_menu = NULL;
}

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_message_menu);
    edpart_message_menu = NULL;
}

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundid_menu);
    edpart_soundid_menu = NULL;
}

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varemit_menu);
    edpart_varemit_menu = NULL;
}

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (edpart_set_debris == 0)
            edpart_nearest_type->trail_effects[0] = item->data;
        else if (edpart_set_debris == 1)
            edpart_nearest_type->trail_effects[1] = item->data;
        else if (edpart_set_debris == 2)
            edpart_nearest_type->attached_effect = item->data;
        else if (edpart_set_debris == 3)
            edpart_nearest_type->impact_effect = item->data;
        else if (edpart_set_debris == 4)
            edpart_nearest_type->kill_effect = item->data;
    }
}

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->effect_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_changegenrate_menu =
            eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelChangeGenRateMenu, "Emits per Second");
        if (edpart_changegenrate_menu != NULL) {
            eduiMenuAddItem(edpart_changegenrate_menu,
                            eduiItemSliderCreateInt(
                                0, edblack, 0, edpartChangeGenRate, 0, static_cast<i32>(edpart_superscale * 1200.0f),
                                static_cast<i32>(edpart_nearest_type->emission_rate), "Emits per Second"));
            eduiMenuAttach(menu, edpart_changegenrate_menu);
            edpart_changegenrate_menu->x = menu->x + 10;
            edpart_changegenrate_menu->y = menu->y + 40;
        }
    }
}

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->data == 0)
            edpart_nearest_type->rotation[0] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
        else if (item->data == 1)
            edpart_nearest_type->rotation[1] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
        else if (item->data == 2)
            edpart_nearest_type->rotation[2] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
    }
}

static void edpartChangeMaxLifeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_maxlife_menu =
            eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelChangeMaxLifeMenu, "Max Instance Life");
        if (edpart_maxlife_menu != NULL) {
            eduiMenuAddItem(edpart_maxlife_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeMaxLife, 0.0f, edpart_superscale * 10.0f,
                                                 edpart_nearest_type->lifetime, "Base Max Life"));
            eduiMenuAddItem(edpart_maxlife_menu,
                            eduiItemSliderCreate(0, edblack, 0, edpartChangeRanMaxLife, 0.0f, edpart_superscale * 10.0f,
                                                 edpart_nearest_type->lifetime_random, "Random Max Life"));
            eduiMenuAttach(menu, edpart_maxlife_menu);
            edpart_maxlife_menu->x = menu->x + 10;
            edpart_maxlife_menu->y = menu->y + 40;
        }
    }
}

static void edpartEmitterDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 2;
    edpart_emitterdebris_menu =
        eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelEmitterDebrisMenu, "Emitter Debris");
    if (edpart_emitterdebris_menu != NULL) {
        eduiMenuAddItem(edpart_emitterdebris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        bool level = edpart_nearest_type->field_b3 == 1;
        eduiMenuAddItem(edpart_emitterdebris_menu,
                        eduiItemSelCreate(1, level ? edblack : edgrey, 0, 0, level ? edpartLevelDebrisIndexMenu : NULL,
                                          "Level List..."));
    }
    eduiMenuAttach(menu, edpart_emitterdebris_menu);
    edpart_emitterdebris_menu->x = menu->x + 10;
    edpart_emitterdebris_menu->y = menu->y + 40;
}

static void edpartInstanceFlagsMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_instanceflags_menu =
        eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelInstanceFlagsMenu, "Instance Flags");
    if (edpart_instanceflags_menu != NULL) {
        i32 group = 1;
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(1, edblack, (edpart_nearest_type->flags & 1) != 0, group++,
                                             edpartChangeInstanceFlag, "Die when stopped"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(2, edblack, (edpart_nearest_type->flags & 2) != 0, group++,
                                             edpartChangeInstanceFlag, "Collide with Characters"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(4, edblack, (edpart_nearest_type->flags & 4) != 0, group++,
                                             edpartChangeInstanceFlag, "Interact only with Active Chars"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(8, edblack, (edpart_nearest_type->flags & 8) != 0, group++,
                                             edpartChangeInstanceFlag, "Damage Characters"));
        if (edpart_nearest_type->effect_ids[0] != 9999)
            eduiMenuAddItem(edpart_instanceflags_menu,
                            eduiItemToggleCreate(0x10, edblack, (edpart_nearest_type->flags & 0x10) != 0, group++,
                                                 edpartChangeInstanceFlag, "Don't die when off-screen"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x40, edblack, (edpart_nearest_type->flags & 0x40) != 0, group++,
                                             edpartChangeInstanceFlag, "Is a Collectible"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x80, edblack, (edpart_nearest_type->flags & 0x80) != 0, group++,
                                             edpartChangeInstanceFlag, "Rotate Randomly"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x100000, edblack, (edpart_nearest_type->flags & 0x100000) != 0, group++,
                                             edpartChangeInstanceFlag, "Ordered Rotate Randomly"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x100, edblack, (edpart_nearest_type->flags & 0x100) != 0, group++,
                                             edpartChangeInstanceFlag, "No Bounce"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x200, edblack, (edpart_nearest_type->flags & 0x200) != 0, group++,
                                             edpartChangeInstanceFlag, "Slot Cannot be Stolen"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x400, edblack, (edpart_nearest_type->flags & 0x400) != 0, group++,
                                             edpartChangeInstanceFlag, "Ignore Terrain"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x800, edblack, (edpart_nearest_type->flags & 0x800) != 0, group++,
                                             edpartChangeInstanceFlag, "Face Direction of Movement"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x1000, edblack, (edpart_nearest_type->flags & 0x1000) != 0, group++,
                                             edpartChangeInstanceFlag, "Disable Draw"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x2000, edblack, (edpart_nearest_type->flags & 0x2000) != 0, group++,
                                             edpartChangeInstanceFlag, "Real Time Lighting"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x4000, edblack, (edpart_nearest_type->flags & 0x4000) != 0, group++,
                                             edpartChangeInstanceFlag, "Ignore Creature"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x8000, edblack, (edpart_nearest_type->flags & 0x8000) != 0, group++,
                                             edpartChangeInstanceFlag, "Thrown"));
        eduiMenuAddItem(edpart_instanceflags_menu,
                        eduiItemToggleCreate(0x10000, edblack, (edpart_nearest_type->flags & 0x10000) != 0, group++,
                                             edpartChangeInstanceFlag, "Can Damage Owner"));
        eduiMenuAttach(menu, edpart_instanceflags_menu);
        edpart_instanceflags_menu->x = menu->x + 10;
        edpart_instanceflags_menu->y = menu->y + 40;
    }
}

static void edpartInstanceScaleMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_instancescale_menu =
        eduiMenuCreate(70, 70, 180, 300, ed_fnt, edpartCancelInstanceScaleMenu, "Instance Scale");
    if (edpart_instancescale_menu != NULL && edpart_nearest_type != NULL) {
        eduiMenuAddItem(edpart_instancescale_menu,
                        eduiItemSliderCreate(0, edblack, 0, edpartChangeInstanceScale, 0.0f, edpart_superscale * 10.0f,
                                             edpart_nearest_type->particle_scale, "Instance Scale"));
        eduiMenuAttach(menu, edpart_instancescale_menu);
        edpart_instancescale_menu->x = menu->x + 10;
        edpart_instancescale_menu->y = menu->y + 40;
    }
}

static void edpartWorldInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edpart_which_scene = 0;
    edpart_worldinstance_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelWorldInstanceMenu,
                       const_cast<char *>(edpart_filter ? "World Scene (Filtered)" : "World Scene"));
    if (edpart_worldinstance_menu == NULL || edbits_base_scene == NULL || edpart_nearest_type == NULL)
        return;
    i32 group = 1;
    i32 selected_first = 0;
    i32 count = NuGScnNumSpecials(edbits_base_scene);
    for (i32 index = 0; index < count; ++index) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, index);
        char *name = NuSpecialExistsFn(&special) ? NuSpecialGetName(&special) : NULL;
        i32 selected = 0;
        i32 included = 0;
        for (i32 variant = 0; variant < edpart_nearest_type->variant_count; ++variant) {
            if (edpart_nearest_type->effect_ids[variant] == index) {
                if (edpart_nearest_type->effect_pages[variant] == 0) {
                    selected = 1;
                    included = 1;
                }
            }
        }
        if (edpart_filter && NuStrNCmp(edpart_filter_string, name, NuStrLen(edpart_filter_string)) != 0 && !included)
            continue;
        eduiitem_s *item = eduiItemToggleCreate(index, edblack, selected, group++, edpartSetInstanceType, name);
        eduiMenuAddItem(edpart_worldinstance_menu, item);
        if (selected & (selected_first ^ 1)) {
            selected_first = 1;
            edpart_worldinstance_menu->selected = edui_last_item;
        }
    }
    if (group == 1)
        eduiMenuAddItem(edpart_worldinstance_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "All Instances Filtered"));
    eduiMenuAttach(parent, edpart_worldinstance_menu);
    edpart_worldinstance_menu->x = parent->x + 10;
    edpart_worldinstance_menu->y = parent->y + 40;
}

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emittime_menu);
    edpart_emittime_menu = NULL;
}

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instance_menu);
    edpart_instance_menu = NULL;
    edpart_nullobject_highlight = 0;
}

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varstart_menu);
    edpart_varstart_menu = NULL;
}

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (edpart_set_debris != 0) {
            if (edpart_set_debris == 1)
                edpart_nearest_type->trail_rates[1] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
            return;
        }
        edpart_nearest_type->trail_rates[0] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
    }
}

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->highlighted)
            edpart_nearest_type->flags |= item->data;
        else
            edpart_nearest_type->flags &= ~item->data;
    }
}

static void edpartDebrisSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_debrissettings_menu =
            eduiMenuCreate(70, 70, 300, 300, ed_fnt, edpartCancelDebrisSettingsMenu, "Debris Settings");
        if (edpart_debrissettings_menu != NULL) {
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartTrail1DebrisMenu, "Trail 1 Debris..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartTrail2DebrisMenu, "Trail 2 Debris..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartEmitterDebrisMenu, "Emitter Debris..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartImpactDebrisMenu, "Impact Debris..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartDieDebrisMenu, "Die Debris..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartImpactPartMenu, "Impact Part..."));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemToggleCreate(0x200000, edblack, (edpart_nearest_type->flags >> 21) & 1, 1,
                                                 edpartChangeInstanceFlag, "Debris stops when Part stops"));
            eduiMenuAddItem(edpart_debrissettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartDebrisScaleMenu, "Debris Scale..."));
        }
        eduiMenuAttach(menu, edpart_debrissettings_menu);
        edpart_debrissettings_menu->x = menu->x + 10;
        edpart_debrissettings_menu->y = menu->y + 40;
    }
}

static void edpartInstanceOrientMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_instorient_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelInstanceOrientMenu, "Instance Orientation");
    if (edpart_instorient_menu == NULL)
        return;
#define EDPART_ORIENT_SLIDER(axis, field, callback, minimum, maximum, label)                                           \
    do {                                                                                                               \
        eduiMenuAddItem(edpart_instorient_menu,                                                                        \
                        eduiItemSliderCreate(axis, edblack, 0, callback, minimum, maximum,                             \
                                             edpart_nearest_type->field[axis] * (360.0f / 65536.0f), label));          \
        eduiItemSliderSetFmt(static_cast<edui_slider_s *>(edui_last_item), "(%1.01f)");                                \
        eduiItemSliderSetGranularity(static_cast<edui_slider_s *>(edui_last_item), 0.1f);                              \
    } while (0)
    EDPART_ORIENT_SLIDER(0, rotation, edpartChangeInstanceRot, edpart_superscale * -180.0f, edpart_superscale * 360.0f,
                         "Base Rotation X");
    EDPART_ORIENT_SLIDER(1, rotation, edpartChangeInstanceRot, edpart_superscale * -180.0f, edpart_superscale * 360.0f,
                         "Base Rotation Y");
    EDPART_ORIENT_SLIDER(2, rotation, edpartChangeInstanceRot, edpart_superscale * -180.0f, edpart_superscale * 360.0f,
                         "Base Rotation Z");
    EDPART_ORIENT_SLIDER(0, rotation_random, edpartChangeInstanceVarRot, 0.0f, edpart_superscale * 180.0f,
                         "Rotation Variation X");
    EDPART_ORIENT_SLIDER(1, rotation_random, edpartChangeInstanceVarRot, 0.0f, edpart_superscale * 180.0f,
                         "Rotation Variation Y");
    EDPART_ORIENT_SLIDER(2, rotation_random, edpartChangeInstanceVarRot, 0.0f, edpart_superscale * 180.0f,
                         "Rotation Variation Z");
#undef EDPART_ORIENT_SLIDER
    eduiMenuAttach(menu, edpart_instorient_menu);
    edpart_instorient_menu->x = menu->x + 10;
    edpart_instorient_menu->y = menu->y + 40;
}

static void edpartPartIndexMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_partindex_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelPartIndexMenu, "Part Type");
    if (edpart_partindex_menu == NULL || edpart_nearest_type == NULL)
        return;
    i32 selected = edpart_set_part == 3 ? edpart_nearest_type->impact_part : -1;
    eduiMenuAddItem(edpart_partindex_menu, eduiItemCheckCreate(static_cast<usize>(-1), edblack, selected == -1, 1,
                                                               edpartChangePartIndex, "None"));
    for (i32 index = 0; index < 128; ++index) {
        part_type_s *type = &part_types[index];
        if (type->name[0] == '\0' || type->field_b3 != edpart_part_list)
            continue;
        bool current_type = index == edpart_nearest_emit->effect_id;
        eduiitem_s *item =
            eduiItemCheckCreate(index, current_type ? edgrey : edblack, !current_type && index == selected,
                                current_type ? 0 : 1, current_type ? NULL : edpartChangePartIndex, type->name);
        eduiMenuAddItem(edpart_partindex_menu, item);
        if (index == selected)
            edpart_partindex_menu->selected = item;
    }
    eduiMenuAttach(menu, edpart_partindex_menu);
    edpart_partindex_menu->x = menu->x + 10;
    edpart_partindex_menu->y = menu->y + 40;
}

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_part_list = 1;
    edpartPartIndexMenu(menu, item, value);
}

static void edpartThingsInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edpart_which_scene = 1;
    edpart_thingsinstance_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelThingsInstanceMenu,
                       const_cast<char *>(edpart_filter ? "Things Scene (Filtered)" : "Things Scene"));
    if (edpart_thingsinstance_menu == NULL || edbits_things_scene == NULL || edpart_nearest_type == NULL)
        return;
    i32 group = 1;
    i32 selected_first = 0;
    i32 count = NuGScnNumSpecials(edbits_things_scene);
    for (i32 index = 0; index < count; ++index) {
        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_things_scene, index);
        char *name = NuSpecialExistsFn(&special) ? NuSpecialGetName(&special) : NULL;
        i32 selected = 0;
        i32 included = 0;
        for (i32 variant = 0; variant < edpart_nearest_type->variant_count; ++variant) {
            if (edpart_nearest_type->effect_ids[variant] == index) {
                if (edpart_nearest_type->effect_pages[variant] == 1) {
                    selected = 1;
                    included = 1;
                }
            }
        }
        if (edpart_filter && NuStrNCmp(edpart_filter_string, name, NuStrLen(edpart_filter_string)) != 0 && !included)
            continue;
        eduiitem_s *item = eduiItemToggleCreate(index, edblack, selected, group++, edpartSetInstanceType, name);
        eduiMenuAddItem(edpart_thingsinstance_menu, item);
        if (selected & (selected_first ^ 1)) {
            selected_first = 1;
            edpart_thingsinstance_menu->selected = edui_last_item;
        }
    }
    if (group == 1)
        eduiMenuAddItem(edpart_thingsinstance_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "All Instances Filtered"));
    eduiMenuAttach(parent, edpart_thingsinstance_menu);
    edpart_thingsinstance_menu->x = parent->x + 10;
    edpart_thingsinstance_menu->y = parent->y + 40;
}

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_diedebris_menu);
    edpart_diedebris_menu = NULL;
}

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_leveltype_menu);
    edpart_leveltype_menu = NULL;
}

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_partindex_menu);
    edpart_partindex_menu = NULL;
}

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_scaletype_menu);
    edpart_scaletype_menu = NULL;
}

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->particle_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edpart_which_scene = 1;
    edpart_instanceorphans_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelInstanceOrphansMenu, "Instance Orphans/Dupes");
    if (edpart_instanceorphans_menu == NULL || edpart_nearest_type == NULL)
        return;
    i32 group = 1;
    char effect_name[20];
    char label[38];
#define EDPART_ORPHAN_ITEM(index)                                                                                      \
    if (edpart_nearest_type->effect_ids[index] == 9998) {                                                              \
        NuStrNCpy(effect_name, edpart_nearest_type->object_names[index], 17);                                          \
        sprintf(label, "Remove - %s", effect_name);                                                                    \
        eduiMenuAddItem(edpart_instanceorphans_menu,                                                                   \
                        eduiItemSelCreate(index, edblack, 0, group++, edpartDeleteInstanceOrphan, label));             \
    }
    EDPART_ORPHAN_ITEM(0)
    EDPART_ORPHAN_ITEM(1)
    EDPART_ORPHAN_ITEM(2)
    EDPART_ORPHAN_ITEM(3)
    EDPART_ORPHAN_ITEM(4)
    EDPART_ORPHAN_ITEM(5)
    EDPART_ORPHAN_ITEM(6)
    EDPART_ORPHAN_ITEM(7)
#undef EDPART_ORPHAN_ITEM
    if (edpart_nearest_orphans) {
        eduiMenuAddItem(edpart_instanceorphans_menu,
                        eduiItemSelCreate(8, edblack, 0, 0, edpartDeleteAllInstanceOrphans, "Remove All Orphans"));
    } else {
        eduiMenuAddItem(edpart_instanceorphans_menu, eduiItemSelCreate(8, edgrey, 0, 0, NULL, "Remove All Orphans"));
    }
    if (edpart_nearest_duplicates) {
        eduiMenuAddItem(edpart_instanceorphans_menu,
                        eduiItemSelCreate(8, edblack, 0, 0, edpartDeleteAllInstanceDuplicates, "Remove Duplicates"));
    } else {
        eduiMenuAddItem(edpart_instanceorphans_menu, eduiItemSelCreate(8, edgrey, 0, 0, NULL, "Remove Duplicates"));
    }
    eduiMenuAttach(parent, edpart_instanceorphans_menu);
    edpart_instanceorphans_menu->x = parent->x + 10;
    edpart_instanceorphans_menu->y = parent->y + 40;
}

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_name_menu);
    edpart_name_menu = NULL;
}

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactpart_menu);
    edpart_impactpart_menu = NULL;
}

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switchtype_menu);
    edpart_switchtype_menu = NULL;
}

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->data == 0)
            edpart_nearest_type->rotation_random[0] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
        else if (item->data == 1)
            edpart_nearest_type->rotation_random[1] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
        else if (item->data == 2)
            edpart_nearest_type->rotation_random[2] =
                static_cast<i32>(static_cast<edui_slider_s *>(item)->value * (65536.0f / 360.0f));
    }
}

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpartRemoveInstance(edpart_nearest_type, item->data);
    --edpart_nearest_orphans;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    edpartSavePath(path, backup, true);
    bool backed_up = edbits_override_backups || EdFileBackup(path, backup);
    bool saved = edpartSaveEffects(path, 1) != 0;
    if (!saved) {
        edpartSaveMessage(parent, "Save Failed", false);
    } else if (!backed_up) {
        edpartSaveMessage(parent, "Saved OK - Backup Failed", false);
    } else {
        edpartSaveMessage(parent, "Saved OK", true);
    }
}

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_part_list = 0;
    edpartPartIndexMenu(menu, item, value);
}

static void edpartInstanceSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type != NULL) {
        edpart_instancesettings_menu =
            eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelInstanceSettingsMenu, "Instance Settings");
        if (edpart_instancesettings_menu != NULL) {
            eduiMenuAddItem(edpart_instancesettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartChangeMaxLifeMenu, "Max Instance Life..."));
            eduiMenuAddItem(edpart_instancesettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceOrientMenu, "Instance Orientation..."));
            eduiMenuAddItem(edpart_instancesettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceFlagsMenu, "Instance Flags..."));
            eduiMenuAddItem(edpart_instancesettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartTintMenu, "Instance Tint..."));
            eduiMenuAddItem(edpart_instancesettings_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceScaleMenu, "Instance Scale..."));
        }
        eduiMenuAttach(menu, edpart_instancesettings_menu);
        edpart_instancesettings_menu->x = menu->x + 10;
        edpart_instancesettings_menu->y = menu->y + 40;
    }
}

static void edpartDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_debrisindex_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelDebrisIndexMenu, "Debris Type");
    if (edpart_debrisindex_menu == NULL || edpart_nearest_type == NULL)
        return;
    i32 selected = -1;
    switch (edpart_set_debris) {
        case 0:
            selected = edpart_nearest_type->trail_effects[0];
            break;
        case 1:
            selected = edpart_nearest_type->trail_effects[1];
            break;
        case 2:
            selected = edpart_nearest_type->attached_effect;
            break;
        case 3:
            selected = edpart_nearest_type->impact_effect;
            break;
        case 4:
            selected = edpart_nearest_type->kill_effect;
            break;
    }
    bool found_selected = false;
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        debinftype *type = debtab[index];
        if (type == NULL || type->category != edpart_particle_list)
            continue;
        eduiitem_s *item =
            eduiItemCheckCreate(index, edblack, index == selected, 1, edpartChangeDebrisIndex, type->name);
        eduiMenuAddItem(edpart_debrisindex_menu, item);
        if (index == selected) {
            edpart_debrisindex_menu->selected = item;
            found_selected = true;
        }
    }
    eduiMenuSortItemsByTxt(edpart_debrisindex_menu);
    eduiMenuAddItemFirst(edpart_debrisindex_menu, eduiItemCheckCreate(static_cast<usize>(-1), edblack, selected == -1,
                                                                      1, edpartChangeDebrisIndex, "None"));
    eduiMenuAttach(menu, edpart_debrisindex_menu);
    edpart_debrisindex_menu->x = menu->x + 10;
    edpart_debrisindex_menu->y = menu->y + 40;
    if (found_selected)
        edpart_debrisindex_menu->field_0c = edpart_debrisindex_menu->selected;
}

static void edpartLevelDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_particle_list = 1;
    edpartDebrisIndexMenu(menu, item, value);
}

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisindex_menu);
    edpart_debrisindex_menu = NULL;
}

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisscale_menu);
    edpart_debrisscale_menu = NULL;
}

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_generaltype_menu);
    edpart_generaltype_menu = NULL;
}

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactdebris_menu);
    edpart_impactdebris_menu = NULL;
}

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundcontrol_menu);
    edpart_soundcontrol_menu = NULL;
}

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail1debris_menu);
    edpart_trail1debris_menu = NULL;
}

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail2debris_menu);
    edpart_trail2debris_menu = NULL;
}

static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    edpartSavePath(path, backup, false);
    bool backed_up = edbits_override_backups || EdFileBackup(path, backup);
    bool saved = edpartSaveEffects(path, 0) != 0;
    if (!saved) {
        edpartSaveMessage(parent, "Save Failed", false);
    } else if (!backed_up) {
        edpartSaveMessage(parent, "Saved OK - Backup Failed", false);
    } else {
        edpartSaveMessage(parent, "Saved OK", true);
    }
}

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_particle_list = 0;
    edpartDebrisIndexMenu(menu, item, value);
}

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_changegenrate_menu);
    edpart_changegenrate_menu = NULL;
}

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_maxlife_menu);
    edpart_maxlife_menu = NULL;
}

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitterdebris_menu);
    edpart_emitterdebris_menu = NULL;
}

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instanceflags_menu);
    edpart_instanceflags_menu = NULL;
}

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancescale_menu);
    edpart_instancescale_menu = NULL;
}

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_worldinstance_menu);
    edpart_worldinstance_menu = NULL;
}

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrissettings_menu);
    edpart_debrissettings_menu = NULL;
}

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instorient_menu);
    edpart_instorient_menu = NULL;
}

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_thingsinstance_menu);
    edpart_thingsinstance_menu = NULL;
}

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceOrphans(eduimenu_s *menu, eduiitem_s *, u32) {
    for (i32 index = 0; index < 8; ++index) {
        if (edpart_nearest_type->effect_ids[index] == 9998) {
            edpartRemoveInstance(edpart_nearest_type, index);
            --index;
        }
    }
    edpart_nearest_orphans = 0;
    edpartFinishMenu(menu);
}

static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *) {
    if (edpart_nearest_type->variant_count == 0) {
        edpart_nearest_type->effect_pages[0] = 1;
        edpart_nearest_type->effect_ids[1] = -1;
        edpart_nearest_type->effect_pages[1] = 1;
        edpart_nearest_type->effect_ids[2] = -1;
        edpart_nearest_type->effect_pages[2] = 1;
        edpart_nearest_type->effect_ids[3] = -1;
        edpart_nearest_type->effect_pages[3] = 1;
        edpart_nearest_type->effect_ids[4] = -1;
        edpart_nearest_type->effect_pages[4] = 1;
        edpart_nearest_type->effect_ids[5] = -1;
        edpart_nearest_type->effect_pages[5] = 1;
        edpart_nearest_type->effect_ids[6] = -1;
        edpart_nearest_type->effect_pages[6] = 1;
        edpart_nearest_type->effect_ids[7] = -1;
        edpart_nearest_type->effect_pages[7] = 1;
        edpart_nearest_type->effect_ids[0] = 9999;
        edpart_nearest_type->flags |= 0x10;
    }
    eduiMenuDestroy(edpart_instanceorphans_menu);
    edpart_instanceorphans_menu = NULL;
}

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancesettings_menu);
    edpart_instancesettings_menu = NULL;
}

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceDuplicates(eduimenu_s *menu, eduiitem_s *, u32) {
    for (i32 index = 0; index < 8; ++index) {
        i16 effect = edpart_nearest_type->effect_ids[index];
        if (effect == 9999)
            continue;
        if (effect == -1)
            continue;
        if (effect == 9998)
            continue;
        for (i32 previous = 0; previous < index; ++previous) {
            if (effect == edpart_nearest_type->effect_ids[previous]) {
                edpartRemoveInstance(edpart_nearest_type, index);
                --index;
                break;
            }
        }
    }
    edpart_nearest_duplicates = 0;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

void edpartDoInput(nupad_s *pad) {
    if ((pad->digital_buttons & 0x100) == 0)
        edcamMove(pad);
    if (pad->digital_buttons & 0x100) {
        if (edpart_nearest == -1)
            edpartDetermineNearest(-1.0f);
        else {
            if (pad->digital_buttons_pressed & 8) {
                do {
                    ++edpart_nearest;
                    if (edpart_nearest == 40)
                        edpart_nearest = 0;
                } while (part_emits[edpart_nearest].effect_id == -1);
            }
            if (pad->digital_buttons_pressed & 2) {
                do {
                    --edpart_nearest;
                    if (edpart_nearest == -1)
                        edpart_nearest = 39;
                } while (part_emits[edpart_nearest].effect_id == -1);
            }
        }
        if (edpart_nearest != -1) {
            part_emit_s *emit = &part_emits[edpart_nearest];
            edcamSetPos(&emit->position);
            const i16 *reference_rotation = reinterpret_cast<const i16 *>(&emit->trailing_state_words[0]);
            edpart_rotz = reference_rotation[0];
            edpart_roty = reference_rotation[1];
            edpart_emitrotz = emit->rotation_2c;
            edpart_emitroty = emit->rotation_2e;
            edpart_emitrotx = emit->rotation_30;
            edpart_offset = *reinterpret_cast<f32 *>(&emit->trailing_state_words[1]);
            edpart_create_type = emit->effect_id;
            edpart_effect_list = part_types[emit->effect_id].field_b3;
        }
    }

    if (edpart_snap_enabled)
        edcamGetPosAngSnap(&edpart_cam_pos, &edpart_cam_ax, &edpart_cam_ay);
    else
        edcamGetPosAng(&edpart_cam_pos, &edpart_cam_ax, &edpart_cam_ay);

    if ((pad->digital_buttons & 0x100) == 0) {
        if (pad->digital_buttons_pressed & 0x80) {
            edpart_opt_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelOptMenu, "PART Editor");
            if (edpart_opt_menu != NULL) {
                eduiMenuAddItem(edpart_opt_menu,
                                eduiItemSelCreate(1, edblack, 0, 0, edpartTypeMenu, "Emitter Type..."));
                if (edpart_nearest != -1) {
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceMenu, "Instance Select..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartInstanceSettingsMenu,
                                                                       "Instance Settings..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartEmitMenu, "Emitter Settings..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edblack, 0, 0, edpartDebrisSettingsMenu,
                                                                       "Debris Settings..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartSoundsMenu, "Attached Sounds..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartSwitchMenu, "Switch Menu..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartScaleTypeMenu, "Scale Type..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartDataMenu, "Data Menu..."));
                    eduiMenuAddItem(edpart_opt_menu,
                                    eduiItemSelCreate(1, edblack, 0, 0, edpartSScaleMenu, "Super Scale..."));
                } else {
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Instance Select..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Instance Settings..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Emitter Settings..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Debris Settings..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Attached Sounds..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Switch Menu..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Scale Type..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Data Menu..."));
                    eduiMenuAddItem(edpart_opt_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Super Scale..."));
                }
                eduiMenuAddItem(edpart_opt_menu, eduiItemToggleCreate(1, edblack, edpart_filter, 1, edpartToggleFilter,
                                                                      "Instance Filter"));
                eduiMenuAddItem(edpart_opt_menu,
                                eduiItemTextPickCreate(0, edblack, edpartChangeFilterName, "Filter String: "));
                edui_textpicker_s *filter = static_cast<edui_textpicker_s *>(edui_last_item);
                strcpy(filter->value, edpart_filter_string);
                filter->max_length = 15;
            }
            edpart_active_menu = edpart_opt_menu;
        }
        if ((pad->digital_buttons_pressed & 0x40) && edpart_copy_mode == 0) {
            if (edpart_create_type != -1)
                edpartCreate(&edpart_cam_pos, edpart_create_type);
        }
        if (pad->digital_buttons & 0x20) {
            if (edpart_copy_mode)
                edpartMultipleCopyCopy();
            else if (edpart_nearest != -1)
                edpartPlace(edpart_nearest, &edpart_cam_pos);
        }
        if (pad->digital_buttons & 0x400) {
            if (!edpart_copy_mode && edpart_nearest != -1)
                edpartPlace(edpart_nearest, &edpart_cam_pos);
        }
        if (pad->digital_buttons_pressed & 0x10) {
            if (edpart_copy_mode)
                edpartMultipleCopyClear();
            else if (edpart_nearest != -1) {
                edpartDestroy(edpart_nearest);
                edpart_nearest = -1;
            }
        }
    }

    if (edpart_copy_mode == 0) {
        i32 right = pad->analog_left_pad_right;
        i32 left = pad->analog_left_pad_left;
        i32 up = pad->analog_left_pad_up;
        i32 down = pad->analog_left_pad_down;
        if (edpart_dpad_mode == 1) {
            if (pad->digital_buttons & 0x200)
                edpart_roty = edpart_rotz = 0;
            edpart_roty += right - left;
            i32 rotation = edpart_rotz + up;
            if (rotation > 0)
                rotation = 0;
            rotation -= down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpart_rotz = rotation;
        } else if (edpart_dpad_mode == 0) {
            if (pad->digital_buttons & 0x200)
                edpart_emitrotx = edpart_emitroty = edpart_emitrotz = 0;
            if (pad->digital_buttons & 0x400)
                edpart_emitrotx = edpart_emitrotx + right - left;
            else {
                edpart_emitroty = edpart_emitroty + right - left;
                i32 rotation = edpart_emitrotz + up;
                if (rotation > 0x8000)
                    rotation = 0x8000;
                rotation -= down;
                if (rotation < -0x8000)
                    rotation = -0x8000;
                edpart_emitrotz = rotation;
            }
        } else if (edpart_dpad_mode == 3) {
            edpart_refroty = edpart_refroty + right - left;
            i32 rotation = edpart_refrotz + up;
            if (rotation > 0)
                rotation = 0;
            rotation -= down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpart_refrotz = rotation;
        } else if (edpart_dpad_mode == 2) {
            if (up == 255 || (pad->digital_buttons_pressed & 0x1000))
                edpart_offset += 1.25f;
            if (down == 255 || (pad->digital_buttons_pressed & 0x4000))
                edpart_offset -= 1.25f;
            if (edpart_offset < 0.0f)
                edpart_offset = 0.0f;
        }
    } else {
        f32 size = edpart_copy_size + static_cast<f32>(pad->analog_left_pad_up) / 5000.0f -
                   static_cast<f32>(pad->analog_left_pad_down) / 5000.0f;
        if (size < 0.05f)
            edpart_copy_size = 0.05f;
        else
            edpart_copy_size = 2.0f < size ? 2.0f : size;
        edpart_copyroty = edpart_copyroty + pad->analog_left_pad_right - pad->analog_left_pad_left;
    }

    if (edpart_nearest == -1)
        return;

    edpart_nearest_emit = &part_emits[edpart_nearest];
    if (edpart_nearest_emit->effect_id != -1)
        edpart_nearest_type = &part_types[edpart_nearest_emit->effect_id];
    edpart_nearest_duplicates = 0;
    i32 orphans = 0;
    i32 duplicates = 0;
    for (i32 variant = 0; variant < 8; ++variant) {
        i16 effect = edpart_nearest_type->effect_ids[variant];
        if (effect == 9999 || effect == -1)
            continue;
        if (effect == 9998)
            ++orphans;
        else {
            for (i32 previous = 0; previous < variant; ++previous) {
                if (effect == edpart_nearest_type->effect_ids[previous]) {
                    ++duplicates;
                    break;
                }
            }
        }
    }
    edpart_nearest_orphans = orphans;
    edpart_nearest_duplicates = duplicates;
}

extern "C" {
    ed_module_s edpartdesc = {NULL,        NULL, "PART Editor", edpartInit, edpartClose, edpartEnter,  NULL,
                              edpartApply, NULL, NULL,          0x74726170, edpartProc,  edpartRender, NULL};
}
