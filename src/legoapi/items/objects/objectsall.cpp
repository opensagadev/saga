// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>

struct AIROW_s;
struct nunativegscene_s;
struct SHOPINPUT;

f32 BUCK_RIDERJUMPCLEARANCE = 1.0f;
void (*BuckStartExtraFn)(GameObject_s *) = NULL;
void NewRumble(nupad_s *, f32, i32);

void Buck_Start(GameObject_s *object, GameObject_s *rider) {
    if (LEGOCONTEXT_BUCK == -1 || LEGOACT_BUCK == -1 ||
        object->apiobj.character_model->model_data_b[LEGOACT_BUCK] == NULL)
        return;
    if (rider != NULL)
        NewRumble(rider->pad_gamepad->pad, 0.6f, 0);
    object->character_context = LEGOCONTEXT_BUCK;
    object->context_animation = LEGOACT_BUCK;
    object->context_animation_timer = AnimDuration(object->id, LEGOACT_BUCK, 0.0f, 0.0f, 1);
    if (BuckStartExtraFn != NULL)
        BuckStartExtraFn(object);
}

extern i16 id_SNOWMOB;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support) {
    NUVEC *position = block->position;
    if (support)
        *support = NULL;
    for (i32 i = 0; i < world->push_block_count; ++i) {
        if (i == excluded)
            continue;
        pushblock_s *other = &world->push_blocks[i];
        if (other->flags_0cb & 2)
            continue;
        if (!NuSpecialGetVisibilityFn(&other->special))
            continue;
        f32 x = position->x - other->position->x, y = position->y - other->position->y,
            z = position->z - other->position->z;
        bool overlapx = x > (other->bounds_min.x - block->bounds_max.x) + 0.01f &&
                        x < (other->bounds_max.x - block->bounds_min.x) - 0.01f;
        f32 ymax = other->bounds_max.y - block->bounds_min.y;
        bool overlapz = z > (other->bounds_min.z - block->bounds_max.z) + 0.01f &&
                        z < (other->bounds_max.z - block->bounds_min.z) - 0.01f;
        bool overlapy = y > other->bounds_min.y - block->bounds_max.y && y < ymax;
        if (support && overlapz && overlapx && y - ymax > 0.0f) {
            if (!*support)
                *support = other;
            else if ((position->y - (*support)->position->y) - ((*support)->bounds_max.y - block->bounds_min.y) >
                     y - ymax) {
                other->supporting_block = *support;
                *support = other;
            }
        }
        if (overlapx && overlapy && overlapz)
            return other;
    }
    return NULL;
}

extern "C" void KillPart(PART_s *, i32);

void Buck_MoveCode(GameObject_s *object, i32 start) {
    if (LEGOCONTEXT_BUCK != -1 && object->character_context == LEGOCONTEXT_BUCK) {
        object->context_animation_timer -= FRAMETIME;
        if (object->context_animation_timer <= 0.0f)
            object->character_context = -1;
    } else if (object->character_context == -1 && start != 0) {
        Buck_Start(object, object->field_0xcc0);
    }
}

void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount) {
    u16 angle = qrand();
    f32 speed = static_cast<f32>(qrand()) * 1.5259021893143654e-05f * 0.09f + 0.01f;
    rider->apiobj.velocity.x = NuTrigTable[static_cast<u16>(angle / 2)] * speed;
    rider->apiobj.velocity.z = speed * NuTrigTable[((i32)angle + 0x4000) >> 1 & 0x7fff];
    StartJump(rider, 0);
    rider->airborne_collision_target = mount;
    rider->tag_flags |= 1;
    rider->apiobj.velocity.z = 0.0f;
    rider->apiobj.velocity.x = 0.0f;
    rider->pad_gamepad->buttons_held = 0;
    rider->pad_gamepad->buttons_pressed = 0;
    f32 clearance = mount->apiobj.upper_position.y;
    if ((mount->apiobj.character_data->game_character->flags_094[2] & 2) != 0) {
        clearance += BUCK_RIDERJUMPCLEARANCE;
    }
    clearance = clearance - rider->apiobj.lower_position.y + 0.1f;
    GAMECHARACTERDATA *character = rider->apiobj.character_data->game_character;
    if (clearance > character->jump_height) {
        rider->apiobj.velocity.y = NuFsqrt(-2.0f * character->gravity * clearance);
    }
}

void SetEffectVisibility(char *name, i32 visible) {
    i32 type = LookupDebrisEffectPageOnly(name, static_cast<char>(WorldInfo_CurrentlyActive()->page_pp));
    if (type != -1) {
        if (visible != 0)
            DebrisTypeStatusAlwaysOn(type);
        else
            DebrisTypeStatusAlwaysOff(type);
    }
}

i32 Conveyor_AdjustSpeed(NUVEC *velocity) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world != NULL && world->field_0x5174 != 0) {
        velocity->x += world->current_level->conveyor_x_speed;
        velocity->z += world->current_level->conveyor_z_speed;
    }
    return 0;
}

// LevelObjects_InitForLevel @0x475630. Creates the runtime model table and
// resolves each registered model from the scene selected by its table kind.
// Found models are hidden in their source scene: their runtime handle is the
// template used by gizmos and other placed objects to draw them elsewhere.
void LevelObjects_InitForLevel(WORLDINFO_s *world) {
    usize aligned = ALIGN(world->giz_buffer.addr, 4);
    world->lev_objs = reinterpret_cast<LEVEL_OBJECT_RUNTIME *>(aligned);
    world->giz_buffer.addr = aligned + static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME);
    memset(world->lev_objs, 0, static_cast<usize>(LEVELOBJECTCOUNT) * sizeof(LEVEL_OBJECT_RUNTIME));

    if (ObjTabList == NULL || LEVELOBJECTCOUNT <= 0) {
        return;
    }

    for (i32 object_index = 0; object_index < LEVELOBJECTCOUNT; ++object_index) {
        LEVELOBJECT &object_type = ObjTabList[object_index];
        LEVEL_OBJECT_RUNTIME &object = world->lev_objs[object_index];

        if (NuStrICmp(object_type.name, "power_up") == 0) {
            KNOBS = object_index;
        }

        NUGSCN *scene = NULL;
        switch (object_type.kind) {
            case LEVEL_OBJECT_SCENE_LEVEL:
                scene = world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_AREA:
                scene = area_scene != NULL ? area_scene : world->current_gscn;
                break;

            case LEVEL_OBJECT_SCENE_CHARACTER_ICON:
                for (i32 character_id = 0; character_id < CHARCOUNT; ++character_id) {
                    scene = IconScene_FindById(character_id);
                    if (scene != NULL && NuSpecialFind(scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = big_icon_scene != NULL ? big_icon_scene : world->icons_gscn;
                break;

            case LEVEL_OBJECT_SCENE_SAVE_ICON:
                scene = saveicon_scene;
                break;

            case LEVEL_OBJECT_SCENE_VEHICLE:
                scene = vehicle_scene;
                break;

            case LEVEL_OBJECT_SCENE_BUTTON:
                scene = button_scene;
                break;

            case LEVEL_OBJECT_SCENE_THINGS:
            default:
                if (area_scene != NULL && world->area != NULL &&
                    (world->area->flags & AREAFLAG_OVERRIDE_THINGS_SCENE) != 0) {
                    if (NuSpecialFind(area_scene, &object.special, object_type.name, 1) != 0) {
                        goto object_resolved;
                    }
                }
                scene = things_scene;
                break;
        }

        if (scene != NULL) {
            NuSpecialFind(scene, &object.special, object_type.name, 1);
        }

    object_resolved:
        object.active = static_cast<u8>(NuSpecialExistsFn(&object.special));
        if (object.active != 0) {
            NuSpecialSetVisibility(&object.special, 0);
        }
    }
}

void EquivalentObjects_Configure(WORLDINFO_s *world, char *config) {
    world->equivalent_groups = NULL;
    world->equivalent_group_count = 0;
    if (world->current_gscn == NULL)
        return;

    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("equivalentobjects"), config, 0xffff);
    if (parser == NULL)
        return;

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    u8 *cursor = reinterpret_cast<u8 *>(world->giz_buffer.addr);
    world->equivalent_groups = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(cursor);
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, const_cast<char *>("equivalentobjects")) == 0) {

            EQUIVALENTOBJECTGROUP_s *group = reinterpret_cast<EQUIVALENTOBJECTGROUP_s *>(cursor);
            group->object_count = 0;
            group->byte_size = 4;
            while (NuFParGetWord(parser) != 0) {
                nuhspecial_s *handle = &group->objects[group->object_count];
                if (NuSpecialFind(world->current_gscn, handle, parser->word_buf, 1) == 0)
                    break;
                ++group->object_count;
                group->byte_size += sizeof(nuhspecial_s);
            }
            if (group->object_count > 0) {
                ++world->equivalent_group_count;
                cursor += group->byte_size;
            }
        }
    }
    NuFParDestroy(parser);
    if (world->equivalent_group_count == 0)
        world->equivalent_groups = NULL;
    else
        world->giz_buffer.addr = ALIGN(reinterpret_cast<usize>(cursor), 16);
}

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
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

static void edpartInit();
static i32 edpartProc(float delta_time, nupad_s *pad);
static void edpartApply();
static void edpartClose();
static void edpartEnter();
static void edpartRender();
static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartCopyType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDataMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTintMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeName(eduimenu_s *, eduiitem_s *item, u32);
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundXMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundsMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSwitchMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSoundIDMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartInstanceMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32);
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDieDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartScaleTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32);
static void edpartImpactPartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDebrisScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);
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

static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32);

static void edpartImpactDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartTrail1DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartTrail2DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeMaxLifeMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartEmitterDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceFlagsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartWorldInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32);

static void edpartDebrisSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceOrientMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartPartIndexMenu(eduimenu_s *menu) __asm__("_ZL19edpartPartIndexMenuP10eduimenu_sP10eduiitem_sj.isra.12");
static void edpartPartIndexMenu(eduimenu_s *menu);

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartThingsInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartInstanceSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartDebrisIndexMenu(eduimenu_s *menu) __asm__("_ZL21edpartDebrisIndexMenuP10eduimenu_sP10eduiitem_sj.isra.13");
static void edpartDebrisIndexMenu(eduimenu_s *menu);

static void edpartLevelDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceOrphans(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceDuplicates(eduimenu_s *menu, eduiitem_s *, u32);

extern "C" {
    ed_module_s edpartdesc = {NULL,        NULL, "PART Editor", edpartInit, edpartClose, edpartEnter,  NULL,
                              edpartApply, NULL, NULL,          0x74726170, edpartProc,  edpartRender, NULL};
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

extern i16 TerrImpact;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
extern NUVEC TerrImpactPos;
extern NUVEC TerrImpactNormal;
extern NUVEC ShadNorm;
extern NUVEC ShadRoofNorm;
extern NUVEC EShadNorm;
extern NUVEC EShadRoofNorm;
extern f32 ShadRoofY;
extern f32 EShadRoofY;
extern TerrainLastImpact_s TerrLastImpact;
extern TERRSET * CurTerr;
// Runtime-selected groups appended after the fixed terrain allocation.
extern i32 WallSplinesOnly;
extern "C" {
extern i32 IgnoreWallSplines;
}
#include "legoapi/render/core/terrain_stubs_types.h"

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

i32 HitTerrain(void);
void RayImpact(NUVEC *);

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

void DebrisFreeOldestDmaDebTypeTable();

// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
extern "C" i32 part_types_used;
extern "C" f32 partglobaltime;
void edpartScaleType(i32 index, f32 scale);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page);

void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags);

extern "C" {

i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

void Debris(i32 update_panel_time);

} // extern "C"

extern "C" {

void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z);

void DebrisOrientationMtx(i32 handle, NUMTX *source);

void DebrisPopulateInstance(i32 handle, f32 duration);

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

extern i32 debris_render_group;

void DebrisStatusAlwaysOff(i32 *handle);

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

i32 FindPlatInst(i32 instance);

i32 NewPlatInst(void *object, i32 instance);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstRotate(i32 index, i32 rotate);

void PlatOnOff(i32 index, i32 enabled);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

TERRSET *TerrainGetCur(void);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern ADDPART_s Default_ADDPART;

struct rtlset;
rtlset *PartRTL = NULL;
f32 PARTSCALEUPTIME = 0.5f;
void NewPartOrderedRotation(PART_s *);
void PartTimeSlip();
void PartCleanupTypes();
void UpdatePartEmits(f32);
static i32 part_raycasts_enabled = 1;
static NUVEC PartNorm;

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.

    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern f32 partglobaltime;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    i32 CreateScaledPARTEffect(i32, f32);
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern u32 partseed;
    extern NUGSCN *part_scene[32];

    extern part_type_s part_types[128];
    extern NUGSCN *part_scene[32];
    extern part_emit_s part_emits[40];
    extern i32 part_emits_used;
    void CheckPartCount();
    i32 AddPARTEffect(i32, NUVEC *);
    void PARTStartOffset(i32, f32);

    void ResetParts(void);

    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;

// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);

extern i32 GAMEDEMO;

extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PowerUp_Particles(WORLDINFO_s *, NUVEC *);

void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 AddPARTEffect(i32 effect, NUVEC *position);

    void KillPart(PART_s *, i32);
    void NewPartRotation(PART_s *);
    void DebrisPopulateInstance(i32, f32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position, NUVEC *orientation, NUVEC *momentum,
                                         i32 count, f32 scale);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

i32 AddScaledVariableShotDebrisEffect1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *orientation, f32 scale);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void CheckPartCount(void);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

    part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    i32 part_types_used;
    i32 part_emits_used;
    NUGSCN *part_scene[32];
    // The editor loader clears these page IDs with aligned vector stores.
    i32 part_scene_pageid[32] __attribute__((aligned(16)));
    i32 part_platimpactcnt;
    i16 part_platimpactlist[4];
    void ResetParts(void);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

void KillPartsByScene(NUGSCN *scene);

void NewPartRotation(PART_s *part);

i32 PARTLookupType(char *name);

    i32 edpart_load_particle_page = -1;

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void PARTStartOffset(i32 index, f32 offset);

void RemovePARTEffect(i32 index);

void ResetParts(void);

    void DebrisStatusAlwaysOff(i32 *);
    i32 rtlDynamicSetPos(i32, NUVEC *);

} // extern "C"

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);
i32 edpartLookupDebrisEffect(char *name);
extern "C" i32 GetSfxIdN(char *, i32);

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
    i32 edpart_nearest;
    NUVEC edpart_cam_pos;
    part_emit_s *edpart_nearest_emit;
    part_typedesc_s *edpart_nearest_type;
}

extern "C" {
    i32 edbits_part_level_page = 1;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_3D(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);

void PartCollide_3D(PART_s *part);

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

void UpdatePartEmits(f32 time);

extern "C" i32 GetSfxId(const char *);

void PartImpact_Brick(PART_s *part);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

void NewPartOrderedRotation(PART_s *part);

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" i32 PS2_REZ_H;

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

extern "C" void edgraClumpsReset(void);
extern "C" i32 NuRndrDoingScreenGrab;

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern i32 edbits_override_backups;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

void edppStartPage(i32 page);

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif

// Only caller prints the property edit buffer left-aligned.

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
    f32 edpp_copy_size = 0.2f;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern u32 edblack[4];
extern u32 edgrey[4];

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
    eduimenu_s *edgra_instance_menu;
    eduimenu_s *edgra_changeinstance_menu;
    eduimenu_s *edgra_clumpproperties_menu;
    eduimenu_s *edgra_clumpsizes_menu;
    eduimenu_s *edgra_clumparea_menu;
    eduimenu_s *edgra_clumpdist_menu;
    eduimenu_s *edgra_clumpfade_menu;
    eduimenu_s *edgra_clumpterrain_menu;
    eduimenu_s *edgra_clumpmode_menu;
    eduimenu_s *edgra_dpadmode_menu;
    eduimenu_s *edgra_sscale_menu;
    eduimenu_s *edgra_globals_menu;
    i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
    NUVEC edgra_cam_pos;
    f32 edgra_cam_dist;
    i32 edgra_cam_ax;
    i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
    i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
    i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
    i32 edgra_pageid;
    i32 edgra_rotz;
    i32 edgra_roty;
    i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
    i32 edpp_copy_source[8];
extern edpp_particle_s edpp_ptls[512];
    NUVEC edpp_cam_pos;
    f32 edpp_cam_dist;
    i32 edpp_cam_ax;
    i32 edpp_cam_ay;
    i32 edpp_numparticles;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
    i32 edpp_copy_mode;
    i32 edpp_copy_enclosed;
    i32 edpp_copy_source_count;
    NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
    eduimenu_s *memcardmenu;
    eduimenu_s *loadeffectsmenu;
    eduimenu_s *mergeeffectsmenu;
    eduimenu_s *deleteeffectsmenu;
    eduimenu_s *filenamemenu;
    eduimenu_s *confirmmenu;
    eduimenu_s *totalptlsmenu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
    i32 edpp_rotz;
    i32 edpp_roty;
    f32 edpp_offset;
    i32 edpp_refrotz;
    i32 edpp_refroty;
    i32 edpp_copyrotz;
    i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    part_emit_s part_emits[40];
    i32 part_page_on[8];
    i32 part_page_used[8];
    i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
    NUVEC edanim_cam_pos;
    f32 edanim_cam_dist;
    i32 edanim_cam_ax;
    i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
    i32 edanim_emitrotz;
    i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
    i32 edbits_part_general_page;
    i32 edbits_duplicates_error;
extern i32 edbits_numsounds;
extern i32 edbits_override_backups;
    eduimenu_s *edbri_dpadmode_menu;
    eduimenu_s *edbri_plankinstance_menu;
    eduimenu_s *edbri_postinstance_menu;
    eduimenu_s *edbri_plankcount_menu;
    eduimenu_s *edbri_bridgeproperties_menu;
    eduimenu_s *edbri_ropecolour_menu;
    NUVEC edbri_cam_pos;
    f32 edbri_cam_dist;
    i32 edbri_cam_ax;
    i32 edbri_cam_ay;
extern i32 edbri_page_used[8];
    i32 edbri_mode;
extern edbridge_s edBridges[64];
}

void edanimDetermineNearestAnim(f32);
extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" i32 NuKeyboard(i32 key);

void edcamSetContoller(i32 invert_pitch);

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern EdRegistry theRegistry;
extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void PlaySfxById(i32, nuvec_s *);
extern i32 EdType_Char;
extern i32 EdType_Short;
extern i32 EdType_Int;
extern i32 EdType_Float;
extern i32 EdType_VuVec;
extern i32 EdType_VuMtx;
extern i32 EdType_Enumeration;
extern i32 EdType_String;
extern i32 EdType_Colour3;
extern i32 EdType_NuHSpecial;
extern i32 EdType_NuVec;
extern i32 EdType_NuMtx;

void SerialiseString(EdStream &, void *, i32);

extern f32 Scale;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern EdSystem theEdSystem;

extern "C" {
extern char edgra_filter_string[16];
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    i32 AddPARTEffect(i32, NUVEC *);
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_readout, edpp_num_orphans;
    extern f32 edpp_copy_size;
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
}

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void edanimDoInput(nupad_s *pad);

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour);

i32 edanimFileSave(char *path);

void edpartInitType(i32 index);

extern "C" {
    void DebFreeInstantly(i32 *);
}

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour);

void EdDrawLineCross(VuVec const &position, float size, i32 colour);

void edpartScaleType(i32 index, float scale);

void edanimDrawCursor();

extern "C" i32 part_emits_used;

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour);

i32 edanimParamCreate(i32 instance_id);

i32 edpartSaveEffects(char *filename, char page);

void edanimParamDestroy(i32 index);

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end);

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name);

void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();

void edanimParticleCreate(NUVEC *position);

void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);

void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);

void edanimDetermineNearestParticle(float distance);

extern EdEnumControl::Item OpenClosedItems[3];
extern EdEnumControl::Item OnOffItems[3];
extern EdEnumControl::Item YesNoItems[3];

extern i32 AxisColour[8];

inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}

inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}

extern f32 MouseScale;

template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}

inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}

inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}

inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}

inline EdSubSystem::~EdSubSystem() {
}

inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

// Animation editor subsystem stubs (static, internal linkage).

// Remaining animation-editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern i32 edbits_override_backups;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
}

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
}

extern "C" {
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern i32 EDPP_MAX_TYPES;
    extern debinftype **debtab;
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
}

// Particle-list editor UI/menu callbacks.

// Particle editor UI/menu callbacks.

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

// Particle list editor subsystem stubs (static, internal linkage).

// ---- end src/gameapi/edtools/edptlall.cpp ----

extern "C" {
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.

// Left-aligned (16) specialisation of the integer print helper.

// Centred (64) specialisation of the integer print helper.

// Only caller prints the property edit buffer left-aligned.

// Only eduiMenuRender calls the integer clip helper, always left-aligned.

#include "gameapi/edtools/edtools_types.h"

extern "C" {
    // Retail only has IPA-SRA clones of these two helpers.

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 eduiPropTextPos;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern u32 edblack[4];
extern u32 edgrey[4];

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern i32 edpp_nearest;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_num_orphans;
extern i32 edpp_dpad_mode;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

}

extern "C" {
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern "C" {
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern i32 edbits_numsounds;
extern i32 edbits_override_backups;
extern i32 edbri_page_used[8];
extern edbridge_s edBridges[64];
}

extern "C" {
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
i32 edbitsStartCubemapDump(void);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edpartClearPage(i8 page);
void edppClearPage(i8 page);

void edppStopPage(i32 page_id);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetUsingMenuFocus(i32 enabled);
i32 eduiUsedAlgPad(nupad_s *pad);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

// Text-picker commit callbacks. Retail keeps them unmangled and local.

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern i32 SAVESLOTS;
extern char **TTab;
extern i16 tCANCEL;
extern "C" OPTIONSSAVE TempOptions;
extern i32 menu_flash;
extern f32 minikittime;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
void Draw_AUTOSAVEWARNING();

extern i32 memcard_saveneeded;
extern i32 memcard_loadneeded;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern FadeSystem FadeSys;

void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

NUGSCN *IconScene_FindById(i32 character_id);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

extern "C" {

extern f32 MENUDY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;

void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void DrawMenu(i32 paused);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void Draw_CANCEL(MENU *menu);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuReset(void);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

struct ani3_animheader_s;

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    extern "C" f32 partglobaltime;
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern i32 players_going_forward;

extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern SPEEDERCHASEVALUES speedervals[4];
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 objopponent_ignoreaiopponent;

void PodKeyReset();

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);

i32 ObjIsTargetSpeeder(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);

extern "C" f32 pod_roll[2];

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
i32 GetMenuID(void);
extern AREADATA_s *PODSPRINT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

extern "C" {

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

GAMEPAD_s *ViewCamGetGamePad();

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern char FS_LastFileName[64];
extern char FS_Path[256];
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void SerialiseString(EdStream &stream, void *data, i32 count);

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern f32 MENUTEXTSCALE;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern f32 cointotaltime;
extern i32 noscenespecials;
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state

// Graphics loading flags

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawPauseFade();

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

extern i32 dco_locatorposonly;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);

extern void DrawMenuEntry(MENU_s *, char *);

#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)

extern i32 GetMenuID(void);

void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);

void BackDrop_ResetColours();

// ==== functions in original address order ====

extern "C" {

i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);

}

extern "C" {

ed_module_s *edmainCurrent(void);

i32 edmainActivate(ed_module_s *module, i32 notify);

void edmainSetCamera(NUMTX *matrix);

NUCAMERA *edmainGetCamera(void);

void edmainExtCamera(NUCAMERA *camera);

void edmainSetCursorEnabled(i32 enabled);

i32 edmainGetCursorEnabled(void);

eduimenu_s *edGetMainMenu(void);

void edmainRegisterLocVec(NUVEC *position);

NUVEC *edmainQueryLocVec(void);

void eduiSetGlobalSliderAccel(f32 acceleration);

void eduiInitMaterials(void);

eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);

void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);

i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);

i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuSortItemsByTxt(eduimenu_s *menu);

void eduiMenuSelectFirstEntry(eduimenu_s *menu);

void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);

f32 eduiGetAnalougePadValue(nupad_s *pad);

eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);

void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);

void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);

i32 eduiItemSetText(eduiitem_s *item, char *text);

}

extern "C" {
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);

eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);

eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);

void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);

eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);

eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);

void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);

void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);

i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);

eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);

}

extern "C" {

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

i32 NuKeyToAscii(u32 key, i32 shifted);

eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);

eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);

eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);

f32 edmainSetMainMenuScale(f32 scale);

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

void eduiMenuDestroy(eduimenu_s *menu);

void edmainClose(void);

i32 edmainRegister(ed_module_s *module);

void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);

void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);

eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);

void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);

eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);

i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);

void eduiSetUsingMenuFocus(i32 enabled);

i32 eduiGetUsingMenuFocus(void);

void eduiSetDefaultActiveMenu(eduimenu_s *menu);

void eduiSetActiveMenu(eduimenu_s *menu);

i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);

eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);

i32 eduiMenuDetach(eduimenu_s *menu);

}

extern "C" {

eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);

eduimenu_s *eduiGetActiveMenuParent(void);

i32 eduiClearActiveMenu(void);

i32 eduiMenuIsActive(eduimenu_s *menu);

i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiMenuEnsureSelection(eduimenu_s *menu);

i32 eduiCursorOverMenu(eduimenu_s *menu);

i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);

void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);

void cbInteractMenuKeySelect(eduimenu_s *menu);

i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);

void eduiFlushInteracts(void);

i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);

i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiRenderInteracts(void);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);

void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}

extern "C" {

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}

extern "C" {
void edcamSetDist(f32 distance);

f32 edcamGetDist(void);

edcam_s *edcamGetEdCam(void);

void edcamSetOffset(NUVEC *offset);

void edcamGetOffset(NUVEC *offset);

void edcamMtx(NUMTX *matrix);

void edcamSet(void);

NUVEC *edcamGetPosPointer(void);

}

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}

extern "C" {

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

extern "C" {

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

void FlushMenuHighlights(eduimenu_s *menu);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern "C" {

extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
}

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 camfx;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

extern i32 NuRndrDoingScreenGrab;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListBeginCriticalSection(void);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_DeallocateSystemFramebuffers(void);
    // Locale string is owned by the Android/JNI TU; cache only its index here.

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void *NuAnimDataCreate(i32 chunk_count);

void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);

    extern "C" f32 partglobaltime;
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
    }

void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

// ==== functions in original address order ====

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->maximum_distance = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_rate = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->lifetime_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_160 = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_164 = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->field_168 = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->particle_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_emit)
        edpart_nearest_emit->field_46 = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_scale_factor = static_cast<edui_slider_s *>(item)->value;
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

static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL)
        edpart_nearest_type->variant_mode = item->data;
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

static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_superscale = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
}

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (item->highlighted)
            edpart_nearest_type->flags |= item->data;
        else
            edpart_nearest_type->flags &= ~item->data;
    }
}

static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_period_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->emission_pause_random = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->speed = static_cast<edui_slider_s *>(item)->value;
}

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->gravity = static_cast<edui_slider_s *>(item)->value * 2.0f;
}

static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->bounce = static_cast<edui_slider_s *>(item)->value;
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

static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL && edpart_set_part == 5) {
        edpart_nearest_type->impact_part = item->data;
    }
}

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type != NULL) {
        if (edpart_set_debris == 0) {
            edpart_nearest_type->trail_rates[0] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
        } else if (edpart_set_debris == 1) {
            edpart_nearest_type->trail_rates[1] = static_cast<i32>(static_cast<edui_slider_s *>(item)->value);
        }
    }
}

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32) {
    if (edpart_nearest_type)
        edpart_nearest_type->effect_scale = static_cast<edui_slider_s *>(item)->value;
}

static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32) {
    edpart_filter = item->highlighted;
}

static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    edpart_active_menu = NULL;
    edpart_opt_menu = NULL;
}

static void edpartApply() {
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

static void edpartClose() {
    eduiMenuDestroy(edpart_opt_menu);
}

static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sscale_menu);
    edpart_sscale_menu = NULL;
}

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_message_menu);
    edpart_message_menu = NULL;
}

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_name_menu);
    edpart_name_menu = NULL;
}

static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_data_menu);
    edpart_data_menu = NULL;
}

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_scaletype_menu);
    edpart_scaletype_menu = NULL;
}

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switchtype_menu);
    edpart_switchtype_menu = NULL;
}

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_switch_menu);
    edpart_switch_menu = NULL;
}

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundcontrol_menu);
    edpart_soundcontrol_menu = NULL;
}

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundid_menu);
    edpart_soundid_menu = NULL;
}

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_soundx_menu);
    edpart_soundx_menu = NULL;
}

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_sounds_menu);
    edpart_sounds_menu = NULL;
}

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisscale_menu);
    edpart_debrisscale_menu = NULL;
}

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_partindex_menu);
    edpart_partindex_menu = NULL;
}

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactpart_menu);
    edpart_impactpart_menu = NULL;
}

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrisindex_menu);
    edpart_debrisindex_menu = NULL;
}

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_diedebris_menu);
    edpart_diedebris_menu = NULL;
}

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_impactdebris_menu);
    edpart_impactdebris_menu = NULL;
}

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitterdebris_menu);
    edpart_emitterdebris_menu = NULL;
}

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail2debris_menu);
    edpart_trail2debris_menu = NULL;
}

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_trail1debris_menu);
    edpart_trail1debris_menu = NULL;
}

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_debrissettings_menu);
    edpart_debrissettings_menu = NULL;
}

static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_cutoff_menu);
    edpart_cutoff_menu = NULL;
}

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emittime_menu);
    edpart_emittime_menu = NULL;
}

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_changegenrate_menu);
    edpart_changegenrate_menu = NULL;
}

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varemit_menu);
    edpart_varemit_menu = NULL;
}

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_varstart_menu);
    edpart_varstart_menu = NULL;
}

static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_grav_menu);
    edpart_grav_menu = NULL;
}

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emitvel_menu);
    edpart_emitvel_menu = NULL;
}

static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_emit_menu);
    edpart_emit_menu = NULL;
}

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancescale_menu);
    edpart_instancescale_menu = NULL;
}

static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_tint_menu);
    edpart_tint_menu = NULL;
}

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instanceflags_menu);
    edpart_instanceflags_menu = NULL;
}

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instorient_menu);
    edpart_instorient_menu = NULL;
}

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_maxlife_menu);
    edpart_maxlife_menu = NULL;
}

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instancesettings_menu);
    edpart_instancesettings_menu = NULL;
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

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_thingsinstance_menu);
    edpart_thingsinstance_menu = NULL;
}

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_worldinstance_menu);
    edpart_worldinstance_menu = NULL;
}

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_instance_menu);
    edpart_instance_menu = NULL;
    edpart_nullobject_highlight = 0;
}

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_leveltype_menu);
    edpart_leveltype_menu = NULL;
}

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_generaltype_menu);
    edpart_generaltype_menu = NULL;
}

static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *) {
    eduiMenuDestroy(edpart_type_menu);
    edpart_type_menu = NULL;
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

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32) {
    NuStrNCpy(edpart_filter_string, static_cast<edui_textpicker_s *>(item)->value, 16);
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

static void edpartImpactPartMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_part = 5;
    edpart_impactpart_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelImpactPartMenu, "Impact Part");
    if (edpart_impactpart_menu != NULL) {
        eduiMenuAddItem(edpart_impactpart_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralPartIndexMenu, "General List..."));
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_impactpart_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelPartIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_impactpart_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
    }
    eduiMenuAttach(menu, edpart_impactpart_menu);
    edpart_impactpart_menu->x = menu->x + 10;
    edpart_impactpart_menu->y = menu->y + 40;
}

static void edpartDieDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 4;
    edpart_diedebris_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelDieDebrisMenu, "Die Debris");
    if (edpart_diedebris_menu != NULL) {
        eduiMenuAddItem(edpart_diedebris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_diedebris_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelDebrisIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_diedebris_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
    }
    eduiMenuAttach(menu, edpart_diedebris_menu);
    edpart_diedebris_menu->x = menu->x + 10;
    edpart_diedebris_menu->y = menu->y + 40;
}

static void edpartImpactDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 3;
    edpart_impactdebris_menu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, edpartCancelImpactDebrisMenu, "Impact Debris");
    if (edpart_impactdebris_menu != NULL) {
        eduiMenuAddItem(edpart_impactdebris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_impactdebris_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelDebrisIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_impactdebris_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
    }
    eduiMenuAttach(menu, edpart_impactdebris_menu);
    edpart_impactdebris_menu->x = menu->x + 10;
    edpart_impactdebris_menu->y = menu->y + 40;
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
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_emitterdebris_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelDebrisIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_emitterdebris_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
    }
    eduiMenuAttach(menu, edpart_emitterdebris_menu);
    edpart_emitterdebris_menu->x = menu->x + 10;
    edpart_emitterdebris_menu->y = menu->y + 40;
}

static void edpartTrail2DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 1;
    edpart_trail2debris_menu = eduiMenuCreate(70, 70, 300, 300, ed_fnt, edpartCancelTrail2DebrisMenu, "Trail 2 Debris");
    if (edpart_trail2debris_menu != NULL) {
        eduiMenuAddItem(edpart_trail2debris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_trail2debris_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelDebrisIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_trail2debris_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
        eduiMenuAddItem(edpart_trail2debris_menu,
                        eduiItemSliderCreateInt(
                            0, edblack, 0, edpartChangeDebrisPerSec, 0, static_cast<i32>(edpart_superscale * 1200.0f),
                            static_cast<i32>(edpart_nearest_type->trail_rates[1]), "Trail 2 particles per second"));
    }
    eduiMenuAttach(menu, edpart_trail2debris_menu);
    edpart_trail2debris_menu->x = menu->x + 10;
    edpart_trail2debris_menu->y = menu->y + 40;
}

static void edpartTrail1DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    if (edpart_nearest_type == NULL)
        return;
    edpart_set_debris = 0;
    edpart_trail1debris_menu = eduiMenuCreate(70, 70, 300, 300, ed_fnt, edpartCancelTrail1DebrisMenu, "Trail 1 Debris");
    if (edpart_trail1debris_menu != NULL) {
        eduiMenuAddItem(edpart_trail1debris_menu,
                        eduiItemSelCreate(1, edblack, 0, 0, edpartGeneralDebrisIndexMenu, "General List..."));
        if (edpart_nearest_type->field_b3 == 1)
            eduiMenuAddItem(edpart_trail1debris_menu,
                            eduiItemSelCreate(1, edblack, 0, 0, edpartLevelDebrisIndexMenu, "Level List..."));
        else
            eduiMenuAddItem(edpart_trail1debris_menu, eduiItemSelCreate(1, edgrey, 0, 0, NULL, "Level List..."));
        eduiMenuAddItem(edpart_trail1debris_menu,
                        eduiItemSliderCreateInt(
                            0, edblack, 0, edpartChangeDebrisPerSec, 0, static_cast<i32>(edpart_superscale * 1200.0f),
                            static_cast<i32>(edpart_nearest_type->trail_rates[0]), "Trail 1 particles per second"));
    }
    eduiMenuAttach(menu, edpart_trail1debris_menu);
    edpart_trail1debris_menu->x = menu->x + 10;
    edpart_trail1debris_menu->y = menu->y + 40;
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

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    edpart_which_scene = 1;
    edpart_instanceorphans_menu =
        eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelInstanceOrphansMenu, "Instance Orphans/Dupes");
    if (edpart_instanceorphans_menu != NULL && edpart_nearest_type != NULL) {
        i32 group = 1;
        char effect_name[20];
        char label[38];
#define EDPART_ORPHAN_ITEM(index)                                                                                      \
    if (edpart_nearest_type->effect_ids[index] == 9998) {                                                              \
        NuStrNCpy(effect_name, edpart_nearest_type->object_names[index], 17);                                          \
        sprintf(label, "Remove - %s", effect_name);                                                                    \
        eduiMenuAddItem(edpart_instanceorphans_menu,                                                                   \
                        eduiItemSelCreate(index, edblack, 0, group, edpartDeleteInstanceOrphan, label));               \
        ++group;                                                                                                       \
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
            eduiMenuAddItem(edpart_instanceorphans_menu,
                            eduiItemSelCreate(8, edgrey, 0, 0, NULL, "Remove All Orphans"));
        }
        if (edpart_nearest_duplicates) {
            eduiMenuAddItem(
                edpart_instanceorphans_menu,
                eduiItemSelCreate(8, edblack, 0, 0, edpartDeleteAllInstanceDuplicates, "Remove Duplicates"));
        } else {
            eduiMenuAddItem(edpart_instanceorphans_menu, eduiItemSelCreate(8, edgrey, 0, 0, NULL, "Remove Duplicates"));
        }
        eduiMenuAttach(parent, edpart_instanceorphans_menu);
        edpart_instanceorphans_menu->x = parent->x + 10;
        edpart_instanceorphans_menu->y = parent->y + 40;
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

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpartRemoveInstance(edpart_nearest_type, item->data);
    --edpart_nearest_orphans;
    eduimenu_s *parent = menu->parent;
    if (parent)
        eduiMenuDetach(menu);
    if (menu->callback)
        menu->callback(menu, parent);
}

static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32) {
    edpart_type_menu = NULL;
    edpart_active_menu = NULL;
    edpart_create_type = item->data;
    edpart_effect_list = part_types[edpart_create_type].field_b3;
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
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

static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32) {
    edpart_switchtype_menu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, edpartCancelSwitchTypeMenu, "Switch Type");
    if (__builtin_expect(edpart_switchtype_menu != NULL, 1)) {
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

static void edpartPartIndexMenu(eduimenu_s *menu) {
    edpart_partindex_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelPartIndexMenu, "Part Type");
    if (__builtin_expect(edpart_partindex_menu == NULL || edpart_nearest_type == NULL, 0))
        return;
    i32 selected = edpart_set_part == 3 ? edpart_nearest_type->impact_part : -1;
    eduiMenuAddItem(edpart_partindex_menu, eduiItemCheckCreate(static_cast<usize>(-1), edblack, selected == -1, 1,
                                                               edpartChangePartIndex, "None"));
    for (i32 index = 0; index < 128; ++index) {
        part_type_s *type = &part_types[index];
        if (__builtin_expect(type->name[0] == '\0' || type->field_b3 != edpart_part_list, 0))
            continue;
        bool current_type = index == edpart_nearest_emit->effect_id;
        eduiitem_s *item =
            eduiItemCheckCreate(index, current_type ? edgrey : edblack, !current_type && index == selected,
                                current_type ? 0 : 1, current_type ? NULL : edpartChangePartIndex, type->name);
        eduiMenuAddItem(edpart_partindex_menu, item);
        if (index == selected)
            edpart_partindex_menu->selected = edui_last_item;
    }
    eduiMenuAttach(menu, edpart_partindex_menu);
    edpart_partindex_menu->x = menu->x + 10;
    edpart_partindex_menu->y = menu->y + 40;
}

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_part_list = 1;
    edpartPartIndexMenu(menu);
}

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_part_list = 0;
    edpartPartIndexMenu(menu);
}

static void edpartDebrisIndexMenu(eduimenu_s *menu) {
    edpart_debrisindex_menu = eduiMenuCreate(70, 70, 250, 250, ed_fnt, edpartCancelDebrisIndexMenu, "Debris Type");
    if (edpart_debrisindex_menu == NULL || edpart_nearest_type == NULL)
        return;
    i32 selected = -1;
    if (edpart_set_debris == 0)
        selected = edpart_nearest_type->trail_effects[0];
    else if (edpart_set_debris == 1)
        selected = edpart_nearest_type->trail_effects[1];
    else if (edpart_set_debris == 2)
        selected = edpart_nearest_type->attached_effect;
    else if (edpart_set_debris == 3)
        selected = edpart_nearest_type->impact_effect;
    else if (edpart_set_debris == 4)
        selected = edpart_nearest_type->kill_effect;
    i32 found_selected = 0;
    for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
        debinftype *type = debtab[index];
        if (__builtin_expect(type == NULL || static_cast<i8>(type->category) != edpart_particle_list, 0))
            continue;
        eduiitem_s *item =
            eduiItemCheckCreate(index, edblack, index == selected, 1, edpartChangeDebrisIndex, type->name);
        eduiMenuAddItem(edpart_debrisindex_menu, item);
        if (__builtin_expect(index == selected, 1)) {
            edpart_debrisindex_menu->selected = edui_last_item;
            found_selected = 1;
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
    edpartDebrisIndexMenu(menu);
}

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value) {
    edpart_particle_list = 0;
    edpartDebrisIndexMenu(menu);
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

extern "C" {
    void PartTerrInit(void) {
    }

    i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags) {
        i32 hit = NewRayCast(position, movement, radius, flags);
        NewRayCastGetImpactNormal(&PartNorm);
        return hit;
    }

    i32 PartPlatId() {
        return TerrainPlatId();
    }

    void CastPart(PART_s *part, f32 time) {
        NUVEC movement;
        NUVEC cast_movement;
        part->active &= ~4;
        part->elapsed = time;
        movement.x = time * part->velocity.x;
        movement.y = time * part->velocity.y + time * time * (0.5f * part->gravity);
        movement.z = time * part->velocity.z;
        if (part->velocity.y > 0.0f && movement.y <= 0.0f) {
            time = -part->velocity.y / part->gravity;
            part->elapsed = time;
            movement.x = time * part->velocity.x;
            movement.y = time * part->velocity.y + time * time * (0.5f * part->gravity);
            movement.z = time * part->velocity.z;
        }
        f32 distance = NuVecMagSqr(&movement);
        if (distance < 0.001f || distance > 10000.0f) {
            part->field_209 = -1;
            return;
        }
        i32 hit;
        if ((part->active & 0x60) == 0x20) {
            f32 radius = (part->time_step + time) / PARTSCALEUPTIME * part->target_radius;
            if (radius > part->target_radius)
                radius = part->target_radius;
            cast_movement = movement;
            hit = PartRayCast(&part->position, &cast_movement, radius, 0);
            if (hit == 0) {
                part->field_209 = -1;
                return;
            }
            part->field_209 = NewRayCastGetImpactTerrainType();
            if ((NewRayCastGetTOFI() > 0.0f || PartNorm.y > 0.5f) &&
                ((part->flags & 0x10000000) == 0 || NewRayCastHitWallSpline() != 4))
                goto impact;
            if (hit > 0x10) {
                part->position.x -= PartNorm.x * NewRayCastGetEmbedDist() * 1.05f;
                part->position.y -= PartNorm.y * NewRayCastGetEmbedDist() * 1.05f;
                part->position.z -= PartNorm.z * NewRayCastGetEmbedDist() * 1.05f;
            }
        }
        part->active &= ~0x60;
        cast_movement = movement;
        hit = PartRayCast(&part->position, &cast_movement, part->radius, 0);
        if (hit == 0) {
            part->field_209 = -1;
            return;
        }
        part->field_209 = NewRayCastGetImpactTerrainType();
        if ((NewRayCastGetTOFI() > 0.0f || PartNorm.y > 0.5f) &&
            ((part->flags & 0x10000000) == 0 || NewRayCastHitWallSpline() != 4))
            goto impact;
        if (hit > 0x10) {
            part->position.x -= PartNorm.x * NewRayCastGetEmbedDist() * 1.05f;
            part->position.y -= PartNorm.y * NewRayCastGetEmbedDist() * 1.05f;
            part->position.z -= PartNorm.z * NewRayCastGetEmbedDist() * 1.05f;
        }
        {
            f32 radius = (part->time_step - time) / PARTSCALEUPTIME * part->target_radius;
            if (!(radius > 0.0f))
                return;
            part->active = (part->active & ~0x60) | 0x40;
            cast_movement = movement;
            if (PartRayCast(&part->position, &cast_movement, radius, 0) == 0) {
                part->field_209 = -1;
                return;
            }
            part->field_209 = NewRayCastGetImpactTerrainType();
            if ((part->flags & 0x10000000) != 0 && NewRayCastHitWallSpline() == 4)
                return;
        }
    impact:
        part->active |= 4;
        part->field_200 = PartPlatId();
        part->active = (part->active & ~0x10) | (part->field_200 != static_cast<u32>(-1) ? 0x10 : 0);
        part->elapsed *= NewRayCastGetTOFI();
        part->impact_normal = PartNorm;
        part->impact_position.x = part->position.x + cast_movement.x;
        part->impact_position.y = part->position.y + cast_movement.y;
        part->impact_position.z = part->position.z + cast_movement.z;
    }

    void edpartRegisterPointerToGameCharLocation(NUVEC *position) {
        edmainRegisterLocVec(position);
    }

}

void edpartPtlShelve(i32 index) {
    if (part_emits[index].instance_id != -1) {
        if (part_emits[index].instance_id != 99999) {
            DebFreeInstantly(&part_emits[index].instance_id);
            part_emits[index].instance_id = 99999;
        }
    }
}

void edpartPlace(i32 index, nuvec_s *position) {
    part_emit_s *emitter = &part_emits[index];
    emitter->position = *position;
    emitter->rotation_2c = static_cast<i16>(edpart_emitrotz);
    emitter->rotation_30 = static_cast<i16>(edpart_emitrotx);
    emitter->rotation_2e = static_cast<i16>(edpart_emitroty);
}

void edpartMultipleCopyCopy() {
    extern i32 edpart_copy_source_count, edpart_copy_source[8], edpart_copyroty;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos, edpart_copy_source_vec;

    edpart_copy_source_count = 0;
    for (i32 index = 0; index < 512; ++index) {
        if (part_emits[index].instance_id == -1 || part_emits[index].instance_id == 99999)
            continue;

        NUVEC delta;
        NuVecSub(&delta, &edpart_cam_pos, &part_emits[index].position);
        if (__builtin_fabsf(delta.x) <= edpart_copy_size && __builtin_fabsf(delta.y) <= edpart_copy_size &&
            __builtin_fabsf(delta.z) <= edpart_copy_size) {
            edpart_copy_source[edpart_copy_source_count++] = index;
            if (edpart_copy_source_count == 8)
                break;
        }
    }
    edpart_copy_source_vec = edpart_cam_pos;
    edpart_copyroty = 0;
}

void edpartMultipleCopyClear() {
    extern i32 edpart_copy_source_count, edpart_copyroty;
    edpart_copy_source_count = 0;
    edpart_copyroty = 0;
}

void edpartDrawCursor() {
    extern i32 edpart_copy_mode, edpart_copyrotz, edpart_copyroty, edpart_rotz, edpart_roty;
    extern i32 edpart_emitrotz, edpart_emitroty, edpart_emitrotx, edpart_refrotz, edpart_refroty;
    extern i32 edpart_dpad_mode, edpart_nearest, edpart_readout, edpart_num_orphans;
    extern i32 edpart_create_type, edpart_nearest_orphans, edpart_nearest_duplicates, edpart_copy_enclosed;
    extern i8 edpart_effect_list;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos;
    extern NUMTL *edpart_mtl, *edpart_boxmtl;
    extern part_emit_s *edpart_nearest_emit;
    extern part_typedesc_s *edpart_nearest_type;

    i32 rotation_z, rotation_y;
    if (edpart_copy_mode != 0) {
        rotation_z = edpart_copyrotz;
        rotation_y = edpart_copyroty;
    } else {
        rotation_z = edpart_rotz;
        rotation_y = edpart_roty;
    }
    NUVEC vector;
    NURND_VERTEX3D line[2];
    auto rotate = [&]() {
        NuVecRotateZ(&vector, &vector, rotation_z);
        NuVecRotateY(&vector, &vector, rotation_y);
    };
    auto draw_axis = [&]() {
        rotate();
        line[0].position.x = edpart_cam_pos.x - vector.x;
        line[0].position.y = edpart_cam_pos.y - vector.y;
        line[0].position.z = edpart_cam_pos.z - vector.z;
        line[0].colour = 0xffffffff;
        line[1].position.x = edpart_cam_pos.x;
        line[1].position.y = edpart_cam_pos.y;
        line[1].position.z = edpart_cam_pos.z;
        line[1].colour = 0xffffffff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        line[0].position.x = edpart_cam_pos.x;
        line[0].position.y = edpart_cam_pos.y;
        line[0].position.z = edpart_cam_pos.z;
        line[0].colour = 0xff00ff00;
        line[1].position.x = edpart_cam_pos.x + vector.x;
        line[1].position.y = edpart_cam_pos.y + vector.y;
        line[1].position.z = edpart_cam_pos.z + vector.z;
        line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpart_mtl, NULL);
    };
    vector = {0.5f, 0.0f, 0.0f};
    draw_axis();
    vector = {0.0f, 0.5f, 0.0f};
    draw_axis();
    vector = {0.0f, 0.0f, 0.5f};
    draw_axis();
    auto draw_mark = [&](f32 sx, f32 sy, f32 sz, f32 ex, f32 ey, f32 ez) {
        vector.x = sx;
        vector.y = sy;
        vector.z = sz;
        rotate();
        line[0].position.x = edpart_cam_pos.x + vector.x;
        line[0].position.y = edpart_cam_pos.y + vector.y;
        line[0].position.z = edpart_cam_pos.z + vector.z;
        line[0].colour = 0xff00ff00;
        vector.x = ex;
        vector.y = ey;
        vector.z = ez;
        rotate();
        line[1].position.x = edpart_cam_pos.x + vector.x;
        line[1].position.y = edpart_cam_pos.y + vector.y;
        line[1].position.z = edpart_cam_pos.z + vector.z;
        line[1].colour = 0xff00ff00;
        NuRndrLine3d(line, edpart_mtl, NULL);
    };
    draw_mark(0.55f, 0.05f, 0.0f, 0.6f, -0.05f, 0.0f);
    draw_mark(0.6f, 0.05f, 0.0f, 0.55f, -0.05f, 0.0f);
    draw_mark(0.0f, 0.65f, -0.025f, 0.0f, 0.6f, 0.0f);
    draw_mark(0.0f, 0.65f, 0.025f, 0.0f, 0.6f, 0.0f);
    draw_mark(0.0f, 0.6f, 0.0f, 0.0f, 0.55f, 0.0f);
    draw_mark(0.0f, 0.05f, 0.55f, 0.0f, 0.05f, 0.6f);
    draw_mark(0.0f, 0.05f, 0.6f, 0.0f, -0.05f, 0.55f);
    draw_mark(0.0f, -0.05f, 0.55f, 0.0f, -0.05f, 0.6f);

    if (edpart_copy_mode != 0) {
        edbitsDrawCube(edpart_cam_pos.x, edpart_cam_pos.y, edpart_cam_pos.z, edpart_copy_size, edpart_copy_size,
                       edpart_copy_size, 0, 0, 0, 0, 0, 0xffffffff, edpart_mtl);
    } else {
        vector.x = 0.0f;
        vector.y = 0.375f;
        vector.z = 0.0f;
        NuVecRotateZ(&vector, &vector, edpart_emitrotz);
        NuVecRotateY(&vector, &vector, edpart_emitroty);
        NuVecRotateX(&vector, &vector, edpart_emitrotx);
        NuVecRotateZ(&vector, &vector, rotation_z);
        NuVecRotateY(&vector, &vector, rotation_y);
        line[0].position.x = edpart_cam_pos.x;
        line[0].position.y = edpart_cam_pos.y;
        line[0].position.z = edpart_cam_pos.z;
        line[1].position.x = edpart_cam_pos.x + vector.x;
        line[1].position.y = edpart_cam_pos.y + vector.y;
        line[1].position.z = edpart_cam_pos.z + vector.z;
        line[0].colour = 0xff0000ff;
        line[1].colour = 0xff0000ff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        vector.x = 0.0f;
        vector.y = 0.375f;
        vector.z = 0.125f;
        NuVecRotateZ(&vector, &vector, edpart_emitrotz);
        NuVecRotateY(&vector, &vector, edpart_emitroty);
        NuVecRotateX(&vector, &vector, edpart_emitrotx);
        NuVecRotateZ(&vector, &vector, rotation_z);
        NuVecRotateY(&vector, &vector, rotation_y);
        line[0].position.x = edpart_cam_pos.x + vector.x;
        line[0].position.y = edpart_cam_pos.y + vector.y;
        line[0].position.z = edpart_cam_pos.z + vector.z;
        line[0].colour = 0xff0000ff;
        NuRndrLine3d(line, edpart_mtl, NULL);
        if (edpart_dpad_mode == 3) {
            vector.x = 0.0f;
            vector.y = 0.25f;
            vector.z = 0.0f;
            NuVecRotateZ(&vector, &vector, edpart_refrotz);
            NuVecRotateY(&vector, &vector, edpart_refroty);
            NuVecRotateZ(&vector, &vector, rotation_z);
            NuVecRotateY(&vector, &vector, rotation_y);
            line[0].position.x = edpart_cam_pos.x;
            line[0].position.y = edpart_cam_pos.y;
            line[0].position.z = edpart_cam_pos.z;
            line[1].position.x = edpart_cam_pos.x + vector.x;
            line[1].position.y = edpart_cam_pos.y + vector.y;
            line[1].position.z = edpart_cam_pos.z + vector.z;
            line[0].colour = 0xffff0000;
            line[1].colour = 0xffff0000;
            NuRndrLine3d(line, edpart_mtl, NULL);
            if (edpart_nearest != -1) {
                vector.x = 0.0f;
                vector.y = *reinterpret_cast<f32 *>(&part_emits[edpart_nearest].trailing_state_words[4]);
                vector.z = 0.0f;
                NuVecRotateZ(&vector, &vector, edpart_refrotz);
                NuVecRotateY(&vector, &vector, edpart_refroty);
                NuVecRotateZ(&vector, &vector, rotation_z);
                NuVecRotateY(&vector, &vector, rotation_y);
                edbitsDrawCube(edpart_cam_pos.x + vector.x, edpart_cam_pos.y + vector.y, edpart_cam_pos.z + vector.z,
                               0.25f, 0.0f, 0.25f, edpart_refrotz, edpart_refroty, 0, rotation_z, rotation_y,
                               0xffff0000, edpart_mtl);
            }
        }
    }

    NuRndrRect2di(0x1720, 0x9b0, 0xf00, 0x410, 0x80808080, edpart_boxmtl);
    NuRndrRect2di(0x1710, 0x8f0, 0xf20, 0xc0, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x1710, 0x9b0, 0x1710, 0xdc8, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x2630, 0x9b0, 0x2630, 0xdc8, 0x80000000, edpart_boxmtl);
    NuRndrLine2di(0x1710, 0xdc8, 0x2630, 0xdc8, 0x80000000, edpart_boxmtl);
    NuQFntPushPrintMode(2);
    NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
    NuQFntSet(system_qfont);
    NuQFntSetColour(system_qfont, 0x80808080);
    NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
    if (edpart_readout == 0) {
        if (edpart_num_orphans < 1) {
            NuQFntSetColour(system_qfont, 0x80808080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Info Box");
        } else {
            NuQFntSetColour(system_qfont, 0x80202080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "WARNING: %d Orphans", edpart_num_orphans);
        }
    } else if (edpart_readout == 1) {
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntPrintEx(system_qfont, 0x17c0, 0x988, 0x10, "Co-ordinates");
    }
    NuQFntSetColour(system_qfont, 0x80000000);
    if (edpart_readout == 0) {
        if (edpart_copy_mode == 0) {
            if (edpart_create_type == -1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Current Type: <none>");
            else
                NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Current Type: %s",
                              part_types[edpart_create_type].name);
            if (edpart_create_type == -1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: <none>");
            else if (edpart_effect_list == 1)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: Level");
            else if (edpart_effect_list == 0)
                NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "Current List: General");
        }
        if (edpart_copy_mode == 0) {
            if (edpart_nearest == -1) {
                NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Highlight: <none>");
            } else {
                edbitsDrawCube(edpart_cam_pos.x, edpart_cam_pos.y, edpart_cam_pos.z,
                               edpart_nearest_type->position_random.x, edpart_nearest_type->position_random.y,
                               edpart_nearest_type->position_random.z, edpart_emitrotz, edpart_emitroty,
                               edpart_emitrotx, rotation_z, rotation_y, 0xff0000ff, edpart_mtl);
                NuQFntSet(system_qfont);
                NuQFntSetColour(system_qfont, 0x80000000);
                NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Highlight: %s", edpart_nearest_type->name);
                if (edpart_nearest_orphans == 0 && edpart_nearest_duplicates == 0) {
                    NuQFntPrintEx(system_qfont, 0x1860, 0xcd0, 0x10, "Instances: %d of %d",
                                  edpart_nearest_type->variant_count, 8);
                } else {
                    NuQFntSetColour(system_qfont, 0x80000080);
                    NuQFntPrintEx(system_qfont, 0x1860, 0xcd0, 0x10, "Instances: %d of %d (Or = %d, Du = %d)",
                                  edpart_nearest_type->variant_count, 8, edpart_nearest_orphans,
                                  edpart_nearest_duplicates);
                    NuQFntSetColour(system_qfont, 0x80000000);
                }
            }
        } else {
            if (edpart_copy_enclosed > 8)
                NuQFntSetColour(system_qfont, 0x80000080);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "Enclosed: %d", edpart_copy_enclosed);
            NuQFntSetColour(system_qfont, 0x80000000);
        }
        NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "Types: %d/%d; Emits: %d/%d", part_types_used, 128,
                      part_emits_used, 40);
    } else if (edpart_readout == 1) {
        if (edpart_nearest == -1) {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Highlight: <none>");
        } else {
            NuQFntPrintEx(system_qfont, 0x17c0, 0xa50, 0x10, "Highlight: %s", edpart_nearest_type->name);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xaf0, 0x10, "XYZ: %0.2f %0.2f %0.2f", edpart_nearest_emit->position.x,
                          edpart_nearest_emit->position.y, edpart_nearest_emit->position.z);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xb90, 0x10, "RotZ: %d",
                          reinterpret_cast<const i16 *>(&edpart_nearest_emit->trailing_state_words[0])[0]);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xc30, 0x10, "RotY: %d",
                          reinterpret_cast<const i16 *>(&edpart_nearest_emit->trailing_state_words[0])[1]);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xcd0, 0x10, "EmitRotZ: %d", edpart_nearest_emit->rotation_2c);
            NuQFntPrintEx(system_qfont, 0x17c0, 0xd70, 0x10, "EmitRotY: %d", edpart_nearest_emit->rotation_2e);
        }
    }
    NuQFntPopPrintMode();
    NuQFntPopCoordinateSystem();
}

void edpartDetermineNearest(f32 distance) {
    NUVEC delta;
    if (edpart_nearest != -1 && part_emits[edpart_nearest].effect_id != -1) {
        NuVecSub(&delta, &edpart_cam_pos, &part_emits[edpart_nearest].position);
        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z == 0.0f)
            return;
    }
    edpart_nearest = -1;
    for (i32 i = 0; i < 40; ++i) {
        if (part_emits[i].effect_id != -1) {
            NuVecSub(&delta, &edpart_cam_pos, &part_emits[i].position);
            const f32 squared_distance = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (distance < 0.0f || squared_distance < distance) {
                distance = squared_distance;
                edpart_nearest = i;
            }
        }
    }
    edpart_nearest_emit = NULL;
    edpart_nearest_type = NULL;
    if (edpart_nearest != -1) {
        edpart_nearest_emit = &part_emits[edpart_nearest];
        if (edpart_nearest_emit->effect_id != -1)
            edpart_nearest_type = &part_types[edpart_nearest_emit->effect_id];
    }
}

void edpartHighlightNearest() {
    NUVEC delta;
    extern i32 edpart_copy_mode, edpart_nearest, edpart_copy_enclosed;
    extern i32 edpart_copy_source_count, edpart_copy_source[8], edpart_copyroty;
    extern f32 edpart_copy_size;
    extern NUVEC edpart_cam_pos, edpart_copy_source_vec;
    extern NUMTL *edpart_mtl;
    if (edpart_copy_mode != 0) {
        edpart_copy_enclosed = 0;
        for (i32 index = 0; index < 512; ++index) {
            if (part_emits[index].instance_id == 99999)
                continue;
            if (part_emits[index].instance_id == -1)
                continue;

            NuVecSub(&delta, &edpart_cam_pos, &part_emits[index].position);
            if (edpart_copy_size < __builtin_fabsf(delta.x) || edpart_copy_size < __builtin_fabsf(delta.y) ||
                edpart_copy_size < __builtin_fabsf(delta.z))
                continue;
            ++edpart_copy_enclosed;
            edbitsDrawDiagonalCross(part_emits[index].position.x, part_emits[index].position.y,
                                    part_emits[index].position.z, 0.05f, 0xffffffff, edpart_mtl);
        }
        for (i32 index = 0; index < edpart_copy_source_count; ++index) {
            NUVEC offset, destination;
            NuVecSub(&offset, &part_emits[edpart_copy_source[index]].position, &edpart_copy_source_vec);
            NuVecRotateY(&offset, &offset, edpart_copyroty);
            NuVecAdd(&destination, &edpart_cam_pos, &offset);
            edbitsDrawDiagonalCross(destination.x, destination.y, destination.z, 0.05f, 0xff808080, edpart_mtl);
        }
    } else if (edpart_nearest != -1) {
        const NUVEC &position = part_emits[edpart_nearest].position;
        edbitsDrawCube(position.x, position.y, position.z, 0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 0, 0xffffffff, edpart_mtl);
    }
}

static void edpartRender() {
    edcamSet();
    edpartDrawCursor();
    edpartHighlightNearest();
    if (edpart_active_menu)
        eduiMenuRender(edpart_active_menu);
}

void edpartInitType(i32 index) {
    part_types[index].lifetime = 0.0f;
    for (i32 variant = 0; variant < 8; ++variant) {
        part_types[index].effect_ids[variant] = -1;
        part_types[index].effect_pages[variant] = 1;
    }
    part_type_s *type = &part_types[index];
    type->variant_count = 0;
    type->speed = 1.0f;
    type->gravity = 0.0f;
    type->emission_rate = 1.0f;
    type->emission_period = 1.0f;
    type->emission_period_random = 0.0f;
    type->emission_pause = 0.0f;
    type->emission_pause_random = 0.0f;
    type->rotation[0] = 0;
    type->rotation[1] = 0;
    type->rotation[2] = 0;
    type->flags = 0;
    type->trail_effects[0] = -1;
    type->trail_effects[1] = -1;
    type->attached_effect = -1;
    type->kill_effect = -1;
    type->impact_effect = -1;
    type->impact_part = -1;
    type->last_used_time = 0.0f;
    type->field_174 = -1;
    type->scale = 1.0f;
    for (i32 sound = 0; sound < 4; ++sound) {
        type->sounds[sound] = -1;
        type->sound_modes[sound] = 0;
    }
}

static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32) {
    edpartAddType(menu, 1);
}

static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32) {
    edpartAddType(menu, 0);
}

void edpartScaleType(i32 index, float scale) {
    part_type_s *type = &part_types[index];
    type->particle_scale *= scale;
    type->effect_scale *= scale;
    type->speed *= scale;
    type->gravity *= scale;
    type->position_random.x *= scale;
    type->position_random.y *= scale;
    type->position_random.z *= scale;
    type->velocity_random.x *= scale;
    type->velocity_random.y *= scale;
    type->velocity_random.z *= scale;
    type->maximum_distance *= scale;
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

i32 edpartSaveEffects(char *filename, char page) {
    char empty_name[16] = {};
    i32 type_count = 0;
    for (i32 index = 0; index < 128; ++index) {
        if (part_types[index].name[0] != '\0' && part_types[index].field_b3 == static_cast<u8>(page))
            ++type_count;
    }

    EdFileSetMedia(1);
    if (EdFileOpen(filename, NUFILE_WRITE) == 0)
        return 0;
    EdFileSetReadWrongEndianess(1);
    EdFileWriteInt(16);
    EdFileWriteInt(type_count);

    static char null_instance_name[16] = "NULL instance";
    for (i32 index = 0; index < 128; ++index) {
        if (part_types[index].name[0] == '\0' || part_types[index].field_b3 != static_cast<u8>(page))
            continue;

        EdFileWrite(part_types[index].name, 16);
        EdFileWriteChar(part_types[index].effect_pages[0]);
        EdFileWriteChar(part_types[index].effect_pages[1]);
        EdFileWriteChar(part_types[index].effect_pages[2]);
        EdFileWriteChar(part_types[index].effect_pages[3]);
        EdFileWriteChar(part_types[index].effect_pages[4]);
        EdFileWriteChar(part_types[index].effect_pages[5]);
        EdFileWriteChar(part_types[index].effect_pages[6]);
        EdFileWriteChar(part_types[index].effect_pages[7]);
#define WRITE_PART_OBJECT_NAME(variant)                                                                                \
    do {                                                                                                               \
        i16 effect_id = part_types[index].effect_ids[variant];                                                         \
        if (effect_id == -1)                                                                                           \
            EdFileWrite(empty_name, 16);                                                                               \
        else if (effect_id == 9999)                                                                                    \
            EdFileWrite(null_instance_name, 16);                                                                       \
        else if (effect_id == 9998)                                                                                    \
            EdFileWrite(part_types[index].object_names[variant], 16);                                                  \
        else if (part_types[index].effect_pages[variant] == 0) {                                                       \
            nuhspecial_s special;                                                                                      \
            NuGScnGetSpecial(&special, edbits_base_scene, effect_id);                                                  \
            EdFileWrite(NuSpecialGetName(&special), 16);                                                               \
        } else if (part_types[index].effect_pages[variant] == 1) {                                                     \
            nuhspecial_s special;                                                                                      \
            NuGScnGetSpecial(&special, edbits_things_scene, effect_id);                                                \
            EdFileWrite(NuSpecialGetName(&special), 16);                                                               \
        } else                                                                                                         \
            EdFileWrite(empty_name, 16);                                                                               \
    } while (0)
        WRITE_PART_OBJECT_NAME(0);
        WRITE_PART_OBJECT_NAME(1);
        WRITE_PART_OBJECT_NAME(2);
        WRITE_PART_OBJECT_NAME(3);
        WRITE_PART_OBJECT_NAME(4);
        WRITE_PART_OBJECT_NAME(5);
        WRITE_PART_OBJECT_NAME(6);
        WRITE_PART_OBJECT_NAME(7);
        EdFileWriteChar(part_types[index].variant_mode);
        EdFileWriteFloat(part_types[index].particle_scale);
        EdFileWriteFloat(part_types[index].effect_scale);
        EdFileWriteFloat(part_types[index].lifetime);
        EdFileWriteFloat(part_types[index].lifetime_random);
        EdFileWriteFloat(part_types[index].speed);
        EdFileWriteFloat(part_types[index].gravity);
        EdFileWriteFloat(part_types[index].emission_rate);
        EdFileWriteFloat(part_types[index].bounce);
        EdFileWriteNuVec(&part_types[index].position_random);
        EdFileWriteNuVec(&part_types[index].velocity_random);
        EdFileWriteFloat(part_types[index].emission_period);
        EdFileWriteFloat(part_types[index].emission_period_random);
        EdFileWriteFloat(part_types[index].emission_pause);
        EdFileWriteFloat(part_types[index].emission_pause_random);
        EdFileWriteInt(part_types[index].rotation[0]);
        EdFileWriteInt(part_types[index].rotation[1]);
        EdFileWriteInt(part_types[index].rotation[2]);
        EdFileWriteInt(part_types[index].rotation_random[0]);
        EdFileWriteInt(part_types[index].rotation_random[1]);
        EdFileWriteInt(part_types[index].rotation_random[2]);
        EdFileWriteUnsignedInt(part_types[index].flags);

        if (part_types[index].trail_effects[0] != -1)
            EdFileWrite(debtab[part_types[index].trail_effects[0]]->name, 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].trail_effects[1] != -1)
            EdFileWrite(debtab[part_types[index].trail_effects[1]]->name, 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].attached_effect != -1)
            EdFileWrite(debtab[part_types[index].attached_effect]->name, 16);
        else
            EdFileWrite(empty_name, 16);
        EdFileWriteFloat(part_types[index].trail_rates[0]);
        EdFileWriteFloat(part_types[index].trail_rates[1]);
        if (part_types[index].kill_effect != -1)
            EdFileWrite(debtab[part_types[index].kill_effect]->name, 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].impact_effect != -1)
            EdFileWrite(debtab[part_types[index].impact_effect]->name, 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].impact_part != -1)
            EdFileWrite(part_types[part_types[index].impact_part].name, 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].sounds[0] != -1)
            EdFileWrite(const_cast<char *>(g_soundInfo[part_types[index].sounds[0]].sfx_name), 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].sounds[1] != -1)
            EdFileWrite(const_cast<char *>(g_soundInfo[part_types[index].sounds[1]].sfx_name), 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].sounds[2] != -1)
            EdFileWrite(const_cast<char *>(g_soundInfo[part_types[index].sounds[2]].sfx_name), 16);
        else
            EdFileWrite(empty_name, 16);
        if (part_types[index].sounds[3] != -1)
            EdFileWrite(const_cast<char *>(g_soundInfo[part_types[index].sounds[3]].sfx_name), 16);
        else
            EdFileWrite(empty_name, 16);
        EdFileWriteChar(part_types[index].sound_modes[0]);
        EdFileWriteChar(part_types[index].sound_modes[1]);
        EdFileWriteChar(part_types[index].sound_modes[2]);
        EdFileWriteChar(part_types[index].sound_modes[3]);
        EdFileWriteFloat(part_types[index].maximum_distance);
        EdFileWriteFloat(part_types[index].field_160);
        EdFileWriteFloat(part_types[index].field_164);
        EdFileWriteFloat(part_types[index].field_168);
    }

    if (page == 1) {
        i32 emitter_count = 0;
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].effect_id != -1 && part_emits[index].shots_remaining == 0)
                ++emitter_count;
        }
        EdFileWriteInt(emitter_count);
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].effect_id == -1)
                continue;
            EdFileWriteNuVec(&part_emits[index].position);
            EdFileWrite(part_emits[index].name, 16);
            EdFileWriteShort(part_emits[index].rotation_30);
            EdFileWriteShort(part_emits[index].rotation_2e);
            EdFileWriteShort(part_emits[index].rotation_2c);
            EdFileWriteShort(part_emits[index].field_44);
            EdFileWriteShort(part_emits[index].field_46);
        }
    } else {
        EdFileWriteInt(0);
    }
    EdFileSetReadWrongEndianess(0);
    EdFileClose();
    return 1;
}

static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    char general_directory[256], general_name[256], general_extension[256];
    char level_directory[256], level_name[256], level_extension[256];
    if (__builtin_expect(!edbits_general_save_directory[0], 1))
        __builtin_memcpy(general_directory, ".", 2);
    else
        strcpy(general_directory, edbits_general_save_directory);
    if (__builtin_expect(!edbits_general_save_name[0], 0))
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

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    edpartSavePath(path, backup, true);
    bool backed_up = edbits_override_backups || EdFileBackup(path, backup);
    bool saved = edpartSaveEffects(path, 1) != 0;
    if (!saved) {
        u32 colours[4] = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Save Failed"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->y = parent->y + 40;
            edpart_message_menu->x = parent->x + 10;
        }
    } else if (!backed_up) {
        u32 colours[4] = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Saved OK - Backup Failed"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->x = parent->x + 10;
            edpart_message_menu->y = parent->y + 40;
        }
    } else {
        u32 colours[4] = {0x8000c000u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Saved OK"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->x = parent->x + 10;
            edpart_message_menu->y = parent->y + 40;
        }
    }
}

static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32) {
    char path[256], backup[256];
    edpartSavePath(path, backup, false);
    bool backed_up = edbits_override_backups || EdFileBackup(path, backup);
    bool saved = edpartSaveEffects(path, 0) != 0;
    if (!saved) {
        u32 colours[4] = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Save Failed"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->y = parent->y + 40;
            edpart_message_menu->x = parent->x + 10;
        }
    } else if (!backed_up) {
        u32 colours[4] = {0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Saved OK - Backup Failed"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->x = parent->x + 10;
            edpart_message_menu->y = parent->y + 40;
        }
    } else {
        u32 colours[4] = {0x8000c000u, 0x80ff0000, 0x80808080, 0x80404040};
        edpart_message_menu = eduiMenuCreate(70, 70, 300, 250, ed_fnt, edpartCancelMessageMenu, "Message");
        if (edpart_message_menu != NULL) {
            eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Saved OK"));
            eduiMenuAttach(parent, edpart_message_menu);
            edpart_message_menu->x = parent->x + 10;
            edpart_message_menu->y = parent->y + 40;
        }
    }
}

i32 edpartLookupObjectInScene(char *name, nugscn_s *scene) {
    if (name[0] == 0)
        return -1;
    if (NuStrNCmp(name, nullobjectname, 16) == 0)
        return 9999;
    if (scene != NULL) {
        nuhspecial_s special;
        const i32 count = NuGScnNumSpecials(scene);
        i32 i;
        for (i = 0; i < count; ++i) {
            NuGScnGetSpecial(&special, scene, i);
            if (__builtin_expect(NuSpecialExistsFn(&special) && NuStrNCmp(name, NuSpecialGetName(&special), 16) == 0, 0))
                break;
        }
        if (i < count)
            return i;
    }
    return -1;
}

i32 edpartLookupDebrisEffect(char *name) {
    if (edpart_load_particle_page != -1)
        return LookupDebrisEffectPageOnly(name, static_cast<i8>(edpart_load_particle_page));
    return LookupDebrisEffect(name);
}

extern "C" {
    void edpartSetParticlePage(i32 page) {
        edpart_load_particle_page = page;
    }

}

i32 edpartLookupObject(char *name) {
    if (edbits_base_scene != NULL)
        return edpartLookupObjectInScene(name, edbits_base_scene);
    return -1;
}

extern "C" {
    void edpartStartPage(i8 page) {
        for (i32 i = 0; i < 40; ++i) {
            if (part_emits[i].effect_id != -1 && part_emits[i].page == page) {
                part_emits[i].camera_distance = 9999.9f;
            }
        }
        part_page_on[page] = 1;
    }

    void edpartStopPage(i8 page) {
        part_page_on[page] = 0;
    }

    void edpartParticleReset(void) {
        part_emit_s *emit = part_emits;
        part_emit_s *const emit_end = part_emits + 512;
        do {
            emit->instance_id = -1;
        } while (++emit != emit_end);
        memset(part_page_used, 0, sizeof(part_page_used));
        memset(part_page_on, 0, sizeof(part_page_on));
        edpart_instances_used = 0;
    }

    void NuPartEnableRayCasts(i32 enabled) {
        part_raycasts_enabled = enabled;
    }

    void NuPartSetSeed(i32 seed) {
        partseed = static_cast<u32>(seed);
    }

    u32 NuPartGetSeed(void) {
        return partseed;
    }

    void NuPartResetGlobalTime(void) {
        partglobaltime = 0;
    }

    void ResetParts(void) {
        if (Part != NULL) {
            memset(Part, 0, static_cast<usize>(MAXPARTS) * sizeof(PART_s));
        }
        i_part = 0;
    }

    void InitParts(i32 count, VARIPTR *buffer, VARIPTR *) {
        MAXPARTS = count;
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        Part = reinterpret_cast<PART_s *>(buffer->void_ptr);
        buffer->char_ptr += MAXPARTS * sizeof(PART_s);
        ResetParts();
        memset(part_types, 0, sizeof(part_types));
        for (i32 i = 0; i < 128; ++i) {
            for (i32 j = 0; j < 8; ++j) {
                part_types[i].effect_ids[j] = -1;
                part_types[i].effect_pages[j] = -1;
            }
        }
        part_types_used = 0;
        memset(part_emits, 0, 40 * sizeof(part_emit_s));
        for (i32 i = 0; i < 40; ++i) {
            part_emits[i].effect_id = -1;
        }
        part_emits_used = 0;
        memset(part_page_used, 0, sizeof(part_page_used));
        memset(part_page_on, 0, sizeof(part_page_on));
        memset(part_scene, 0, sizeof(part_scene));
        for (i32 i = 0; i < 32; ++i) {
            part_scene_pageid[i] = -1;
        }
        part_platimpactcnt = 0;
    }

    void NewPartRotation(PART_s *part) {
        part->rotation_axis_1 = (NuRandIntSeeded(&partseed) % 65535u) / 21845u + 1;
        part->field_124[3] = static_cast<i32>((NuRandFloatSeeded(&partseed) * 2.0f - 1.0f) * 65536.0f);
        do {
            part->rotation_axis_2 = (NuRandIntSeeded(&partseed) % 65535u) / 21845u + 1;
        } while (part->rotation_axis_2 == part->rotation_axis_1);
        part->field_124[4] = static_cast<i32>((NuRandFloatSeeded(&partseed) * 2.0f - 1.0f) * 65536.0f);
    }

}

void NewPartOrderedRotation(PART_s *part) {
    f32 range = static_cast<i32>(part->field_10c[3]);
    i32 base = part->field_10c[0];
    part->field_124[0] = base + static_cast<i32>((NuRandFloatSeeded(&partseed) * 2.0f - 1.0f) * range);
    range = static_cast<i32>(part->field_10c[4]);
    base = part->field_10c[1];
    part->field_124[1] = base + static_cast<i32>((NuRandFloatSeeded(&partseed) * 2.0f - 1.0f) * range);
    range = static_cast<i32>(part->field_10c[5]);
    base = part->field_10c[2];
    part->field_124[2] = base + static_cast<i32>((NuRandFloatSeeded(&partseed) * 2.0f - 1.0f) * range);
}

void PartTimeSlip() {
    for (i32 i = 0; i < 40; ++i) {
        if (part_emits[i].effect_id != -1) {
            part_emits[i].time_20 -= 800.0f;
            part_emits[i].time_24 -= 800.0f;
            part_emits[i].time_28 -= 800.0f;
        }
    }
    for (i32 i = 0; i < 128; ++i) {
        if (part_types[i].effect_ids[0] != -1) {
            part_types[i].last_used_time -= 800.0f;
        }
    }
    partglobaltime -= 800.0f;
}

extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags) {
    PART_s *part = Part;
    for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
        if ((part->active & 1) == 0 || part->owner == owner || (part->flags & flags) == 0)
            continue;
        if (part->bounds_min.x > maximum->x || minimum->x > part->bounds_max.x || part->bounds_min.z > maximum->z ||
            minimum->z > part->bounds_max.z || part->bounds_min.y > maximum->y || minimum->y > part->bounds_max.y)
            continue;
        for (i32 j = count - 1; j >= 0; --j) {
            NUVEC difference;
            NuVecSub(&difference, &positions[j], &part->position);
            f32 combined_radius = radius + part->target_radius;
            f32 distance = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
            if (distance <= combined_radius * combined_radius)
                return part;
        }
    }
    return NULL;
}

    void KillPart(PART_s *part, i32 reason) {
        if ((part->active & 1) == 0)
            return;
        part->active &= ~1;
        if ((part->flags & 0x20000) != 0 && part->debris_key != NULL) {
            DebFreeInstantly(part->debris_key);
        }
        if (part->lighting_template != NULL) {
            rtlDynamicFree(*part->lighting_template);
            *part->lighting_template = -1;
        }
        if (part->field_1c0 != NULL)
            part->field_1c0(part, reason);
        if (reason != 6 && part->field_1f4 != -1) {
            i32 key = -1;
            AddScaledFiniteShotDebrisEffect(&key, part->field_1f4, &part->position, 0, 0, 1, part->field_1fc);
        }
    }

    void DrawParts(i32 keep_offscreen) {
        PART_s *part = Part;
        for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
            if ((part->active & 1) == 0 || (part->flags & 0x1000) != 0)
                continue;
            if (part->draw_callback != NULL && part->draw_callback(part) == 0)
                continue;
            if ((part->active & 8) != 0) {
                NuRndrLightingStateCurrent.direction[0] = part->lighting[3];
                NuRndrLightingStateCurrent.direction[1] = part->lighting[4];
                NuRndrLightingStateCurrent.direction[2] = part->lighting[5];
                NuRndrLightingStateCurrent.intensity[0] = *reinterpret_cast<NUCOLOUR3 *>(&part->lighting[0]);
                NuRndrLightingStateCurrent.intensity[1] = *reinterpret_cast<NUCOLOUR3 *>(&part->lighting[1]);
                NuRndrLightingStateCurrent.intensity[2] = *reinterpret_cast<NUCOLOUR3 *>(&part->lighting[2]);
                NuRndrSetDirectionalLightsPS(&part->lighting[3], reinterpret_cast<NUCOLOUR3 *>(&part->lighting[0]),
                                             &part->lighting[4], reinterpret_cast<NUCOLOUR3 *>(&part->lighting[1]),
                                             &part->lighting[5], reinterpret_cast<NUCOLOUR3 *>(&part->lighting[2]));
                NuRndrLightingStateCurrent.ambient = *reinterpret_cast<NUCOLOUR3 *>(&part->lighting[6]);
                NuRndrSetAmbientLightPS(reinterpret_cast<NUCOLOUR3 *>(&part->lighting[6]));
            } else if ((part->render_flags & 0x20) != 0) {
                NuRndrLightingStateCurrent.ambient = *reinterpret_cast<NUCOLOUR3 *>(&part->lighting[6]);
                NuRndrSetAmbientLightPS(reinterpret_cast<NUCOLOUR3 *>(&part->lighting[6]));
            }
            part->render_flags &= ~2;
            NUMTX_ALIGNED16 scaled;
            NUMTX *matrix = &part->transform;
            if (part->field_214 != 1.0f) {
                scaled = *matrix;
                NUVEC scale = {part->field_214, part->field_214, part->field_214};
                NuMtxPreScale(&scaled, &scale);
                matrix = &scaled;
            }
            i32 drawn = 0;
            if (part->source_special != NULL && NuSpecialDrawAt(&part->special, matrix) != 0) {
                part->render_flags |= 6;
                drawn = 1;
            }
            if ((drawn | keep_offscreen) == 0 && ((part->flags & 0x10) == 0 || (part->render_flags & 1) != 0)) {
                KillPart(part, 0);
            }
        }
    }

    PART_s *AddPart(ADDPART_s *params) {
        NUVEC forward = {0.0f, 0.0f, -1.0f};
        if (params->special != NULL) {
            if (!NuSpecialExistsFn(params->special))
                return NULL;
        } else if (params->field_28 != -1 && params->field_28 != 9999) {
            return NULL;
        }
        PART_s *part = &Part[i_part];
        i32 scanned = 0;
        if ((part->active & 1) != 0) {
            while ((part->active & 1) != 0 && (part->flags & 0x200) != 0 && scanned < MAXPARTS) {
                if (++i_part == MAXPARTS)
                    i_part = 0;
                part = &Part[i_part];
                ++scanned;
            }
        }
        if (scanned == MAXPARTS)
            return NULL;
        if ((part->active & 1) != 0) {
            if (__builtin_expect(part->replace_callback != NULL, 0))
                part->replace_callback(part);
            KillPart(part, 0);
        }
        if (params->matrix != NULL) {
            NUMTX *source = params->matrix;
            part->transform = *source;
            NUMTX_ALIGNED16 matrix = *source;
            NuVecMtxRotate(&forward, &forward, &matrix);
            part->rotation_y = NuAtan2D(forward.x, forward.z);
            NuVecRotateY(&forward, &forward, -part->rotation_y);
            part->rotation_x = -NuAtan2D(forward.y, forward.z);
        } else {
            if (params->position == NULL)
                return NULL;
            NuMtxSetTranslation(&part->transform, params->position);
        }
        part->initial_position = part->position;
        part->recipient = params->recipient;
        part->owner = params->owner;
        part->velocity = *params->velocity;
        part->field_20b = -1;
        part->force_player_mask = -1;
        part->field_100 = part->field_104 = params->field_a4;
        part->time_step = params->time_step;
        part->active = (part->active & ~0x60) | 0x21;
        part->scale_time = 0.0f;
        part->target_radius = params->field_14;
        part->radius = (params->time_step / PARTSCALEUPTIME) * params->field_14;
        part->field_0e4 = params->field_18;
        part->field_214 = params->field_c0;
        part->gravity = params->gravity;
        part->field_0ec = params->field_20;
        part->pickup_type = params->field_28;
        part->field_0f0 = params->field_88;
        part->flags = params->flags;
        part->active &= ~2;
        part->field_20a = params->field_c4;
        part->render_flags &= ~0x40;
        if (params->lighting != NULL) {
            part->lighting[0] = params->lighting->values[0];
            part->lighting[3] = params->lighting->values[3];
            part->lighting[1] = params->lighting->values[1];
            part->lighting[4] = params->lighting->values[4];
            part->lighting[2] = params->lighting->values[2];
            part->lighting[5] = params->lighting->values[5];
            part->lighting[6] = params->lighting->values[6];
            part->active |= 8;
        } else {
            part->active &= ~8;
            if (params->field_98 != 1.0f || params->field_9c != 1.0f || params->field_a0 != 1.0f) {
                part->lighting[6].x = params->field_98;
                part->lighting[6].y = params->field_9c;
                part->lighting[6].z = params->field_a0;
                part->render_flags |= 0x20;
            } else {
                part->render_flags &= ~0x20;
            }
        }
        part->source_special = params->special;
        if (__builtin_expect(params->special != NULL, 1))
            part->special = *params->special;
        else
            memset(&part->special, 0, sizeof(part->special));
        part->active &= ~0x14;
        part->field_1ac = params->field_30;
        part->move_callback = params->move_fn;
        part->update_callback = params->update_fn;
        part->field_1b8 = params->field_3c;
        part->field_1bc = params->field_40;
        part->field_1c0 = params->field_44;
        part->field_1c4 = params->field_48;
        part->stop_callback = params->stop_fn;
        part->replace_callback = params->replace_fn;
        part->draw_callback = params->draw_fn;
        part->bounds_min.x = part->position.x - part->target_radius;
        part->bounds_min.y = part->position.y - part->target_radius;
        part->bounds_min.z = part->position.z - part->target_radius;
        part->bounds_max.x = part->position.x + part->target_radius;
        part->bounds_max.y = part->position.y + part->target_radius;
        part->bounds_max.z = part->position.z + part->target_radius;
        part->elapsed = 0.0f;
        for (i32 i = 0; i < 6; ++i)
            part->field_10c[i] = params->field_a8[i];
        for (i32 i = 0; i < 5; ++i)
            part->field_124[i] = 0.0f;
        if ((part->flags & 0x80) != 0)
            NewPartRotation(part);
        if ((part->flags & 0x100000) != 0)
            NewPartOrderedRotation(part);
        if (params->field_58 != -1) {
            part->debris_handle = -1;
            part->debris_key = &part->debris_handle;
            i32 effect = params->field_58;
            if (params->field_7c != 1.0f)
                effect = CreateScaledEffect(effect, params->field_7c);
            AddDebrisEffect(part->debris_key, effect, part->position.x, part->position.y, part->position.z);
            DebrisPopulateInstance(*part->debris_key, 0.0f);
        } else {
            part->debris_key = params->debris_key;
        }
        part->field_1e0 = params->field_60;
        part->field_1e4 = params->field_64;
        part->field_1e8 = params->field_68;
        part->field_1ec = params->field_6c;
        part->field_1f8 = params->field_74;
        part->field_1f0 = params->field_70;
        part->field_1f4 = params->field_78;
        part->field_1fc = params->field_7c;
        part->previous_transform = part->transform;
        part->field_208 = 1;
        part->force_flags = 0;
        part->field_21c = 0;
        part->render_flags |= 0x18;
        part->field_210 = 0.0f;
        part->field_207 = params->field_90;
        part->field_204 = 1;
        if ((part->flags & 0x2000) != 0 && params->lighting_template != NULL && PartRTL != NULL) {
            part->lighting_template = params->lighting_template;
            *params->lighting_template = rtlDynamicAllocTemplate(PartRTL, params->field_84);
        }
        if (__builtin_expect(++i_part == MAXPARTS, 1))
            i_part = 0;
        return part;
    }

    void KillAllParts(void) {
        if (Part != NULL) {
            PART_s *part = Part;
            for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
                if ((part->active & 1) != 0) {
                    KillPart(part, 0);
                }
            }
        }
    }

    PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner) {
        if (Part == NULL)
            return NULL;
        PART_s *result = NULL;
        PART_s *part = Part;
        f32 nearest = 1000000.0f;
        for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
            if ((part->active & 1) != 0 && (player == -1 || part->force_player_mask == player) &&
                (owner == NULL || part->owner == owner)) {
                if (position != NULL) {
                    f32 distance = NuVecDistSqr(position, &part->position, NULL);
                    if (distance < nearest) {
                        nearest = distance;
                        result = part;
                    }
                } else {
                    return part;
                }
            }
        }
        return result;
    }

    void SetPartRTLSet(usize rtl_set) {
        PartRTL = reinterpret_cast<rtlset *>(rtl_set);
    }

    void PARTStartOffset(i32 index, f32 offset) {
        if (index == -1)
            return;
        part_emit_s *emitter = &part_emits[index];
        i32 effect = emitter->effect_id;
        if (effect == -1)
            return;
        part_type_s *type = &part_types[effect];
        if (type->emission_period_random == 0.0f && type->emission_pause_random == 0.0f) {
            f32 period = type->emission_period + type->emission_pause;
            emitter->time_24 = static_cast<i32>(partglobaltime / period) * period + offset;
            emitter->time_28 = emitter->time_24 + type->emission_period;
            while (partglobaltime > emitter->time_24 && partglobaltime > emitter->time_28) {
                if (emitter->time_24 >= emitter->time_28)
                    emitter->time_28 = emitter->time_24 + type->emission_period;
                else
                    emitter->time_24 = emitter->time_28 + type->emission_pause;
            }
        } else {
            emitter->time_24 = partglobaltime;
            f32 end = partglobaltime + type->emission_period;
            emitter->time_28 = end + NuRandFloatSeeded(&partseed) * type->emission_period_random;
        }
        f32 interval = 1.0f / type->emission_rate;
        f32 last_emission;
        if (partglobaltime > emitter->time_28)
            last_emission = emitter->time_24 - interval;
        else
            last_emission = static_cast<i32>(partglobaltime / interval) * interval;
        emitter->time_20 = last_emission;
        emitter->time_38 = partglobaltime;
    }

    i32 PARTLookupType(char *name) {
        if (name != NULL && name[0] != 0) {
            for (i32 i = 0; i < 128; ++i) {
                if (NuStrNICmp(name, part_types[i].name, 16) == 0)
                    return i;
            }
        }
        return -1;
    }

    void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation) {
        NUVEC part_position;
        part_type_s *type = &part_types[effect_index];
        if (type->name[0] == 0 || type->emission_rate <= 0.0f) {
            return;
        }

        const f32 effective_rate = rate <= 0.0f ? type->emission_rate : rate;
        if (orientation == NULL) {
            orientation = &numtx_identity;
        }

        const f32 interval = NuFdiv(1.0f, effective_rate);
        const f32 end_time = partglobaltime + duration;
        f32 emission_time = static_cast<i32>(NuFdiv(partglobaltime, interval)) * interval + interval;
        if (emission_time <= end_time) {

            i32 remaining = 99;
            do {
                ADDPART_ALIGNED16 params = Default_ADDPART;

                f32 random = NuRandFloatSeeded(&partseed);
                part_position.x = (random + random) * type->position_random.x - type->position_random.x;
                random = NuRandFloatSeeded(&partseed);
                part_position.y = (random + random) * type->position_random.y - type->position_random.y;
                random = NuRandFloatSeeded(&partseed);
                part_position.z = (random + random) * type->position_random.z - type->position_random.z;
                NuVecMtxTransformVU0(&part_position, &part_position, orientation);
                NuVecAdd(&part_position, &part_position, position);

                NUVEC velocity = {0.0f, type->speed, 0.0f};
                random = NuRandFloatSeeded(&partseed);
                velocity.x += (random + random) * type->velocity_random.x - type->velocity_random.x;
                random = NuRandFloatSeeded(&partseed);
                velocity.y += (random + random) * type->velocity_random.y - type->velocity_random.y;
                random = NuRandFloatSeeded(&partseed);
                velocity.z += (random + random) * type->velocity_random.z - type->velocity_random.z;
                NuVecMtxTransformVU0(&velocity, &velocity, orientation);

                params.gravity = type->gravity;
                params.position = &part_position;
                params.velocity = &velocity;
                params.field_20 = type->bounce;

                i32 variant = 0;
                if (type->variant_count != 0 && type->variant_mode <= 1) {
                    variant = static_cast<i32>(NuRandFloatSeeded(&partseed) * 65535.0f) % type->variant_count;
                }
                params.field_28 = type->effect_ids[variant];
                nuhspecial_s special;
                if (params.field_28 == 9999) {
                    params.special = NULL;
                } else if (params.field_28 != -1) {
                    params.special = &special;
                    NuGScnGetSpecial(&special, part_scene[type->scene_indices[variant]], params.field_28);
                }

                params.field_a4 = type->lifetime + NuRandFloatSeeded(&partseed) * type->lifetime_random;
                NUMTX_ALIGNED16 matrix;
                NuMtxSetIdentity(&matrix);
                const f32 rotation_x = static_cast<f32>(type->rotation[0]);
                const f32 random_x = NuRandFloatSeeded(&partseed);
                const f32 range_x = static_cast<f32>(type->rotation_random[0]);
                const f32 rotation_y = static_cast<f32>(type->rotation[1]);
                const f32 random_y = NuRandFloatSeeded(&partseed);
                const f32 range_y = static_cast<f32>(type->rotation_random[1]);
                const f32 rotation_z = static_cast<f32>(type->rotation[2]);
                const f32 random_z = NuRandFloatSeeded(&partseed);
                const f32 range_z = static_cast<f32>(type->rotation_random[2]);
                for (i32 axis = 0; axis < 3; ++axis) {
                    params.field_a8[axis] = type->rotation[axis];
                    params.field_a8[axis + 3] = type->rotation_random[axis];
                }
                NuMtxRotateX(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_x + random_x) * range_x + rotation_x - range_x)));
                NuMtxRotateY(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_y + random_y) * range_y + rotation_y - range_y)));
                NuMtxRotateZ(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_z + random_z) * range_z + rotation_z - range_z)));
                matrix.m30 = part_position.x;
                matrix.m31 = part_position.y;
                matrix.m32 = part_position.z;
                params.matrix = &matrix;

                params.flags = type->flags & ~0x60000;
                if (type->trail_effects[0] != -1) {
                    params.field_60 = type->trail_effects[0];
                    params.flags |= 0x40000;
                }
                if (type->trail_effects[1] != -1) {
                    params.field_64 = type->trail_effects[1];
                    params.flags |= 0x40000;
                }
                if (type->attached_effect != -1) {
                    params.field_58 = type->attached_effect;
                    params.flags |= 0x20000;
                }
                params.field_68 = type->trail_rates[0];
                params.field_6c = type->trail_rates[1];
                params.field_70 = type->impact_effect;
                if (type->impact_part == -1 && type->impact_part_name[0] != 0) {
                    type->impact_part = PARTLookupType(type->impact_part_name);
                }
                params.field_74 = type->impact_part;
                params.field_78 = type->kill_effect;
                params.field_98 = type->field_160;
                params.field_9c = type->field_164;
                params.field_a0 = type->field_168;
                AddPart(&params);

                for (i32 sound = 0; sound < 4; ++sound) {
                    if (type->sounds[sound] != -1 && type->sound_modes[sound] == 3) {
                        PlaySfxById(type->sounds[sound], &part_position);
                    }
                }
                type->last_used_time = emission_time;
                emission_time += interval;
            } while (emission_time <= end_time && --remaining != 0);
        }
    }

}

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page) {
    char name[16];
    EdFileRead(type->name, 16);
    if (version > 5) {
        for (i32 i = 0; i < 8; ++i)
            type->effect_pages[i] = EdFileReadChar();
    } else {
        if (!(version > 1)) {
            type->effect_pages[0] = 0;
        } else {
            type->effect_pages[0] = EdFileReadChar();
        }
        for (i32 i = 1; i < 8; ++i)
            type->effect_pages[i] = -1;
    }
    const i32 variants = version > 5 ? 8 : 1;
    for (i32 i = 0; i < variants; ++i) {
        EdFileRead(type->object_names[i], 16);
        type->effect_ids[i] = -1;
        if (__builtin_expect(type->effect_pages[i] == 0 || type->effect_pages[i] == 1, 1)) {
            const i32 scene_page = type->effect_pages[i] == 0 ? page : 0;
            for (i32 scene = 0; scene < 32; ++scene) {
                if (part_scene_pageid[scene] == scene_page) {
                    type->effect_ids[i] = edpartLookupObjectInScene(type->object_names[i], part_scene[scene]);
                }
                if (type->effect_ids[i] != -1) {
                    type->scene_indices[i] = scene;
                    break;
                }
            }
        }
        if (type->effect_ids[i] == -1 && type->object_names[i][0] != 0)
            type->effect_ids[i] = 9998;
    }
    type->variant_count = 0;
    for (i32 i = 0; i < 8; ++i) {
        if (type->effect_ids[i] != 9999 && type->effect_ids[i] != -1)
            ++type->variant_count;
    }
    type->variant_mode = version > 5 ? EdFileReadChar() : 0;
    type->particle_scale = version > 15 ? EdFileReadFloat() : 1.0f;
    type->effect_scale = version > 15 ? EdFileReadFloat() : 1.0f;
    type->lifetime = EdFileReadFloat();
    type->lifetime_random = version > 13 ? EdFileReadFloat() : 0.0f;
    type->speed = EdFileReadFloat();
    type->gravity = EdFileReadFloat();
    type->emission_rate = EdFileReadFloat();
    if (version > 7) {
        type->bounce = EdFileReadFloat();
    } else {
        type->bounce = 0.75f;
    }
    type->position_random.x = EdFileReadFloat();
    type->position_random.y = EdFileReadFloat();
    type->position_random.z = EdFileReadFloat();
    type->velocity_random.x = EdFileReadFloat();
    type->velocity_random.y = EdFileReadFloat();
    type->velocity_random.z = EdFileReadFloat();
    type->emission_period = EdFileReadFloat();
    type->emission_period_random = EdFileReadFloat();
    type->emission_pause = EdFileReadFloat();
    type->emission_pause_random = EdFileReadFloat();
    if (!(version > 12)) {
        for (i32 i = 0; i < 3; ++i)
            type->rotation[i] = EdFileReadShort();
        for (i32 i = 0; i < 3; ++i)
            type->rotation_random[i] = EdFileReadShort();
    } else {
        for (i32 i = 0; i < 3; ++i)
            type->rotation[i] = EdFileReadInt();
        for (i32 i = 0; i < 3; ++i)
            type->rotation_random[i] = EdFileReadInt();
    }
    type->flags = EdFileReadUnsignedInt();
    if (type->effect_ids[0] == 9999)
        type->flags |= 0x10;
    if (version > 4) {
        EdFileRead(name, 16);
        type->trail_effects[0] = edpartLookupDebrisEffect(name);
        if (version > 8) {
            EdFileRead(name, 16);
            type->trail_effects[1] = edpartLookupDebrisEffect(name);
        } else
            type->trail_effects[1] = -1;
        EdFileRead(name, 16);
        type->attached_effect = edpartLookupDebrisEffect(name);
        type->trail_rates[0] = EdFileReadFloat();
        type->trail_rates[1] = version > 8 ? EdFileReadFloat() : 60.0f;
    } else {
        type->trail_effects[0] = -1;
        type->trail_effects[1] = -1;
        type->attached_effect = -1;
        if (__builtin_expect(version > 2, 0)) {
            const char mode = EdFileReadChar();
            EdFileRead(name, 16);
            if (__builtin_expect(mode == 0, 0))
                type->trail_effects[0] = edpartLookupDebrisEffect(name);
            else if (mode == 1)
                type->attached_effect = edpartLookupDebrisEffect(name);
            type->trail_rates[0] = EdFileReadFloat();
        } else
            type->trail_rates[0] = 60.0f;
        type->trail_rates[1] = 60.0f;
    }
    if (version > 3) {
        EdFileRead(name, 16);
        type->kill_effect = edpartLookupDebrisEffect(name);
        EdFileRead(name, 16);
        type->impact_effect = edpartLookupDebrisEffect(name);
    } else {
        type->impact_effect = -1;
        type->kill_effect = -1;
    }
    if (version > 11) {
        EdFileRead(type->impact_part_name, 16);
        type->impact_part = PARTLookupType(type->impact_part_name);
    } else {
        type->impact_part_name[0] = 0;
        type->impact_part = -1;
    }
    if (version > 6) {
        for (i32 i = 0; i < 4; ++i) {
            EdFileRead(name, 16);
            type->sounds[i] = GetSfxIdN(name, 16);
        }
        for (i32 i = 0; i < 4; ++i)
            type->sound_modes[i] = EdFileReadChar();
    } else {
        for (i32 i = 0; i < 4; ++i)
            type->sounds[i] = -1;
        for (i32 i = 0; i < 4; ++i)
            type->sound_modes[i] = 0;
    }
    type->maximum_distance = version > 9 ? EdFileReadFloat() : 25.0f;
    if (version > 14) {
        type->field_160 = EdFileReadFloat();
    } else {
        type->field_160 = 1.0f;
    }
    type->field_164 = version > 14 ? EdFileReadFloat() : 1.0f;
    if (version > 14) {
        type->field_168 = EdFileReadFloat();
    } else {
        type->field_168 = 1.0f;
    }
    type->scale = 1.0f;
    type->field_174 = 0;
}

extern "C" {
    i32 PARTLookupTypePageOnly(char *name, i32 page) {
        if (name == NULL || name[0] == 0)
            return -1;
        if (static_cast<u32>(page - 1) <= 6) {
            for (i32 i = 0; i < 128; ++i) {
                if (part_types[i].page == page && NuStrNICmp(name, part_types[i].name, 16) == 0)
                    return i;
            }
        }
        for (i32 i = 0; i < 128; ++i) {
            if (part_types[i].page == 0 && NuStrNCmp(name, part_types[i].name, 16) == 0)
                return i;
        }
        return -1;
    }

    i32 GetPartCount(void) {
        i32 count = 0;
        for (i32 i = 0; i < 40; ++i) {
            if (part_emits[i].effect_id != -1)
                ++count;
        }
        return count;
    }

    i32 GetMaxPartTypes(void) {
        return 0x80;
    }

    char *GetPartName(i32 index) {
        if (part_types[index].name[0] == 0)
            return NULL;
        return part_types[index].name;
    }

}

MechObjectInterface *PART_s::GetMechObjectInterface() {
    if (mech_object_interface == NULL) {
        new PartObjectInterface(*this);
    }
    return mech_object_interface;
}

void PART_s::ClearMechObjectInterface() {
    delete mech_object_interface;
}

extern "C" {
    void CheckPartCount(void) {
    }

    i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count) {
        i32 page = 0;
        if (__builtin_expect(mode != 0, 1)) {
            for (page = 1; page < 8; ++page) {
                if (part_page_used[page] == 0)
                    break;
            }
            if (page == 8)
                return -1;
        }
        i32 scene_slot = 0;
        for (i32 i = 0; i < scene_count; ++i) {
            if (scenes[i] != NULL) {
                while (part_scene[scene_slot] != NULL && scene_slot < 32)
                    ++scene_slot;
                if (scene_slot < 32) {
                    part_scene[scene_slot] = scenes[i];
                    part_scene_pageid[scene_slot] = page;
                }
            }
        }
        EdFileSetMedia(1);
        if (EdFileOpen(path, NUFILE_READ) == 0)
            return -1;
        EdFileSetReadWrongEndianess(1);
        const i32 version = EdFileReadInt();
        if (static_cast<u32>(version - 1) > 15) {
            EdFileClose();
            return -1;
        }
        i32 type_count = EdFileReadInt();
        i32 skipped_types = 0;
        if (type_count + part_types_used > 128) {
            skipped_types = type_count + part_types_used - 128;
            type_count -= skipped_types;
        }
        i32 type_slot = 0;
        for (i32 i = 0; i < type_count; ++i) {
            while (part_types[type_slot].name[0] != 0)
                ++type_slot;
            part_typedesc_s *type = &part_types[type_slot];
            edpartLoadSingleType(type, version, page);
            if (type->name[0] != 0) {
                type->page = page;
                type->field_b3 = mode;
                ++part_types_used;
            }
        }
        part_typedesc_s discarded_type;
        for (i32 i = 0; i < skipped_types; ++i)
            edpartLoadSingleType(&discarded_type, version, page);
        i32 emitter_count = EdFileReadInt();
        if (emitter_count + part_emits_used > 40)
            emitter_count = 40 - part_emits_used;
        CheckPartCount();
        i32 emitter_slot = 0;
        for (i32 i = 0; i < emitter_count; ++i) {
            while (part_emits[emitter_slot].effect_id != -1)
                ++emitter_slot;
            part_emit_s *emitter = &part_emits[emitter_slot];
            emitter->position.x = EdFileReadFloat();
            emitter->position.y = EdFileReadFloat();
            emitter->position.z = EdFileReadFloat();
            EdFileRead(emitter->name, 16);
            emitter->effect_id = PARTLookupTypePageOnly(emitter->name, page);
            emitter->rotation_30 = EdFileReadShort();
            emitter->rotation_2e = EdFileReadShort();
            emitter->rotation_2c = EdFileReadShort();
            if (version > 10) {
                emitter->field_44 = EdFileReadShort();
                emitter->field_46 = EdFileReadShort();
            } else {
                emitter->field_44 = 0;
                emitter->field_46 = 0;
            }
            emitter->page = page;
            emitter->field_3c = 0;
            part_typedesc_s *type = &part_types[emitter->effect_id];
            for (i32 sound = 0; sound < 4; ++sound) {
                if (type->sounds[sound] != -1 && type->sound_modes[sound] != 0) {
                    emitter->field_3c = 1;
                    break;
                }
            }
            if (emitter->effect_id != -1) {
                ++part_emits_used;
                PARTStartOffset(emitter_slot, 0.0f);
                emitter->field_3d = 1;
            }
        }
        CheckPartCount();
        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edpartDetermineNearest(1.0f);
        part_page_used[page] = 1;
        if (page == 0)
            part_page_on[0] = 1;
        if (mode == 0)
            edbits_part_general_page = page;
        else if (__builtin_expect(mode == 1, 1))
            edbits_part_level_page = page;
        return page;
    }

    i32 edpartLoadPage(char *path, i32 mode, void *scene) {
        return edpartLoadPageEx(path, static_cast<i8>(mode), reinterpret_cast<nugscn_s **>(&scene), 1);
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
    if (edpart_message_menu == NULL) {
        return;
    }
    eduiMenuAddItem(edpart_message_menu, eduiItemSelCreate(1, colours, 0, 0, NULL, "Loaded OK"));
    eduiMenuAttach(parent, edpart_message_menu);
    edpart_message_menu->x = parent->x + 10;
    edpart_message_menu->y = parent->y + 40;
}

void edpartDestroy(i32 index) {
    CheckPartCount();
    if (part_emits[index].effect_id != -1) {
        part_emits[index].effect_id = -1;
        --part_emits_used;
    }
    CheckPartCount();
}

void PartCleanupTypes() {
    static i32 pcount1 = 0;
    static i32 pcount2 = 0;
    if (++pcount1 > 5) {
        pcount1 = 0;
        if (part_types[pcount2].effect_ids[0] != -1 && partglobaltime > part_types[pcount2].last_used_time + 5.0f &&
            part_types[pcount2].scale != 1.0f) {
            for (i32 i = 0; i < 40; ++i) {
                if (part_emits[i].effect_id == pcount2)
                    edpartDestroy(i);
            }
            part_types[pcount2].name[0] = 0;
            part_types[pcount2].effect_ids[0] = -1;
            --part_types_used;
        }
        if (++pcount2 >= 128)
            pcount2 = 0;
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

extern "C" {
    void edpartDestroyAllParticles(void) {
        for (i32 index = 0; index < 512; ++index) {
            edpartDestroy(index);
        }
    }

}

i32 edpartPtlChangeType(i32 index, i32 type) {
    if (part_emits[index].effect_id != type) {
        edpartDestroy(index);
        AddDebrisEffect(&part_emits[index].instance_id, type, part_emits[index].position.x,
                        part_emits[index].position.y, part_emits[index].position.z);
        part_emits[index].effect_id = type;
    }
    return index;
}

extern "C" {
    f32 PARTGetTotalOnTime(i32 index) {
        if (index < 0)
            return 0.0f;
        return part_types[index].emission_period + part_types[index].emission_period_random;
    }

    f32 PARTGetTotalOffTime(i32 index) {
        if (index < 0)
            return 0.0f;
        return part_types[index].emission_pause + part_types[index].emission_pause_random;
    }

    i32 AddPARTEffect(i32 effect, NUVEC *position) {
        CheckPartCount();
        if (part_emits_used == 40 || part_types[effect].effect_ids[0] == -1)
            return -1;
        for (i32 i = 0; i < 40; ++i) {
            if (part_emits[i].effect_id != -1)
                continue;
            part_emit_s *emitter = &part_emits[i];
            emitter->position = *position;
            emitter->effect_id = effect;
            part_type_s *type = &part_types[effect];
            NuStrNCpy(emitter->name, type->name, 16);
            PARTStartOffset(i, 0.0f);
            emitter->rotation_30 = 0;
            emitter->camera_distance = 9999.9f;
            emitter->page = type->page;
            emitter->rotation_2e = 0;
            emitter->rotation_2c = 0;
            emitter->shots_remaining = 0;
            emitter->field_33 = 0;
            emitter->field_3d = 1;
            emitter->field_44 = 0;
            emitter->field_46 = 0;
            ++part_emits_used;
            return i;
        }
        return -1;
    }

}

void edpartCreate(nuvec_s *position, i32 type) {
    AddPARTEffect(type, position);
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
            emit = &part_emits[edpart_nearest];
            const i16 *reference_rotation = reinterpret_cast<const i16 *>(&emit->trailing_state_words[0]);
            edpart_roty = reference_rotation[1];
            edpart_rotz = reference_rotation[0];
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
        if (edpart_dpad_mode == 1) {
            if (pad->digital_buttons & 0x200)
                edpart_roty = edpart_rotz = 0;
            edpart_roty += pad->analog_left_pad_right - pad->analog_left_pad_left;
            i32 rotation = edpart_rotz + pad->analog_left_pad_up;
            if (rotation > 0)
                rotation = 0;
            rotation -= pad->analog_left_pad_down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpart_rotz = rotation;
        } else if (edpart_dpad_mode == 0) {
            if (pad->digital_buttons & 0x200)
                edpart_emitrotx = edpart_emitroty = edpart_emitrotz = 0;
            if (pad->digital_buttons & 0x400)
                edpart_emitrotx = edpart_emitrotx + pad->analog_left_pad_right - pad->analog_left_pad_left;
            else {
                edpart_emitroty = edpart_emitroty + pad->analog_left_pad_right - pad->analog_left_pad_left;
                i32 rotation = edpart_emitrotz + pad->analog_left_pad_up;
                if (rotation > 0x8000)
                    rotation = 0x8000;
                rotation -= pad->analog_left_pad_down;
                if (rotation < -0x8000)
                    rotation = -0x8000;
                edpart_emitrotz = rotation;
            }
        } else if (edpart_dpad_mode == 3) {
            edpart_refroty = edpart_refroty + pad->analog_left_pad_right - pad->analog_left_pad_left;
            i32 rotation = edpart_refrotz + pad->analog_left_pad_up;
            if (rotation > 0)
                rotation = 0;
            rotation -= pad->analog_left_pad_down;
            if (rotation < -0x8000)
                rotation = -0x8000;
            edpart_refrotz = rotation;
        } else if (edpart_dpad_mode == 2) {
            if (pad->analog_left_pad_up == 255 || (pad->digital_buttons_pressed & 0x1000))
                edpart_offset += 0.01f;
            if (pad->analog_left_pad_down == 255 || (pad->digital_buttons_pressed & 0x4000))
                edpart_offset -= 0.01f;
            if (edpart_offset < 0.0f)
                edpart_offset = 0.0f;
        }
    } else {
        f32 size = edpart_copy_size + static_cast<f32>(pad->analog_left_pad_up) / 5000.0f -
                   static_cast<f32>(pad->analog_left_pad_down) / 5000.0f;
        if (size < 0.05f)
            edpart_copy_size = 0.05f;
        else if (2.0f < size)
            edpart_copy_size = 2.0f;
        else
            edpart_copy_size = size;
        edpart_copyroty = edpart_copyroty + pad->analog_left_pad_right - pad->analog_left_pad_left;
    }

    if (edpart_nearest == -1)
        return;

    edpart_nearest_emit = &part_emits[edpart_nearest];
    if (edpart_nearest_emit->effect_id != -1)
        edpart_nearest_type = &part_types[edpart_nearest_emit->effect_id];
    edpart_nearest_duplicates = 0;
    i32 duplicates = 0;
    i32 orphans = 0;
    for (i32 variant = 0; variant < 8; ++variant) {
        i16 effect = edpart_nearest_type->effect_ids[variant];
        if (effect == 9999 || effect == -1)
            continue;
        if (effect == 9998)
            ++orphans;
        else {
            for (i32 previous = 0; previous < variant; ++previous) {
                if (effect != edpart_nearest_type->effect_ids[previous]) {
                    continue;
                }
                ++duplicates;
                break;
            }
        }
    }
    edpart_nearest_orphans = orphans;
    edpart_nearest_duplicates = duplicates;
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

extern "C" {
    void RemovePARTEffect(i32 index) {
        if (index != -1) {
            CheckPartCount();
            if (part_emits[index].effect_id != -1) {
                part_emits[index].effect_id = -1;
                --part_emits_used;
            }
            CheckPartCount();
        }
    }

}

void UpdatePartEmits(f32 time) {
    i32 switch_changes[32][2];
    i32 last_switch_change = -1;
    for (i32 i = 0; i < 40; ++i) {
        part_emit_s *emitter = &part_emits[i];
        if (part_page_on[emitter->page] == 0 || emitter->effect_id == -1)
            continue;
        part_type_s *type = &part_types[emitter->effect_id];
        if (type->effect_ids[0] == -1)
            continue;
        f32 interval = 1.0f / type->emission_rate;
        if (emitter->field_44 == 1 && emitter->field_3d != 2 && emitter->field_46 != -1) {
            i32 state = object_switches[emitter->field_46];
            switch (state) {
                case 0:
                    emitter->field_3d = 0;
                    break;
                case 1:
                    emitter->field_3d = 1;
                    break;
                case 2:
                case 3: {
                    last_switch_change = (last_switch_change + 1) & 31;
                    emitter->field_3d = 2;
                    switch_changes[last_switch_change][0] = emitter->field_46;
                    switch_changes[last_switch_change][1] = state == 2 ? 4 : 0;
                    PARTStartOffset(i, 0.0f);
                    f32 shift = emitter->time_24 - partglobaltime;
                    emitter->time_20 = partglobaltime - interval;
                    emitter->time_24 -= shift;
                    emitter->time_28 -= shift;
                    break;
                }
                case 4:
                    last_switch_change = (last_switch_change + 1) & 31;
                    switch_changes[last_switch_change][0] = emitter->field_46;
                    switch_changes[last_switch_change][1] = 0;
                    break;
            }
        }
        f32 previous_distance = emitter->camera_distance;
        emitter->camera_distance = CameraEmitterDistance(&emitter->position);
        if (emitter->field_3c != 0) {
            PART_EMIT_SOUND_ON(0);
            PART_EMIT_SOUND_ON(1);
            PART_EMIT_SOUND_ON(2);
            PART_EMIT_SOUND_ON(3);
        }
        if ((type->flags & 0x400000) != 0) {
            if (type->maximum_distance > emitter->camera_distance && previous_distance >= type->maximum_distance) {
                emitter->time_24 = partglobaltime;
                emitter->time_20 = partglobaltime - interval + 0.001f;
                emitter->time_28 = partglobaltime - 1.0f;
            } else if (emitter->time_24 > emitter->time_28) {
                emitter->time_28 += time;
                emitter->time_24 += time;
                emitter->time_20 += time;
            }
        }
        f32 emission_time = emitter->time_20 + interval;
        i32 remaining = 101;
        while (partglobaltime + time > emission_time && --remaining != 0) {
            while (emission_time >= emitter->time_28 && emission_time > emitter->time_24 &&
                   partglobaltime + time > emission_time) {
                if (emitter->time_24 >= emitter->time_28) {
                    f32 end = emitter->time_24 + type->emission_period;
                    emitter->time_28 = end + NuRandFloatSeeded(&partseed) * type->emission_period_random;
                    if (emitter->field_3c != 0 && emitter->field_3d != 0) {
                        PART_EMIT_SOUND(0, 1);
                        PART_EMIT_SOUND(1, 1);
                        PART_EMIT_SOUND(2, 1);
                        PART_EMIT_SOUND(3, 1);
                    }
                    if (emitter->shots_remaining != 0 && --emitter->shots_remaining == 0) {
                        RemovePARTEffect(i);
                        emission_time = partglobaltime + 99999.9f;
                    }
                } else {
                    f32 pause = type->emission_pause;
                    emitter->time_24 =
                        pause + NuRandFloatSeeded(&partseed) * type->emission_pause_random + emitter->time_28;
                    emission_time = emitter->time_24 + 0.001f;
                    emitter->time_20 = emission_time - interval + 0.001f;
                    if (emitter->field_3d == 2)
                        emitter->field_3d = 0;
                    if (emitter->field_3c != 0 && emitter->field_3d != 0) {
                        PART_EMIT_SOUND(0, 2);
                        PART_EMIT_SOUND(1, 2);
                        PART_EMIT_SOUND(2, 2);
                        PART_EMIT_SOUND(3, 2);
                    }
                }
            }
            if (!(partglobaltime + time > emission_time))
                continue;
            if (emitter->field_3d != 0 &&
                (type->maximum_distance == 0.0f || type->maximum_distance > emitter->camera_distance)) {
                ADDPART_s params = Default_ADDPART;
                params.field_c0 = type->particle_scale;
                NUVEC position;
                f32 random = NuRandFloatSeeded(&partseed);
                position.x = (random + random) * type->position_random.x - type->position_random.x;
                random = NuRandFloatSeeded(&partseed);
                position.y = (random + random) * type->position_random.y - type->position_random.y;
                random = NuRandFloatSeeded(&partseed);
                position.z = (random + random) * type->position_random.z - type->position_random.z;
                NuVecRotateZ(&position, &position, emitter->rotation_2c);
                NuVecRotateY(&position, &position, emitter->rotation_2e);
                NuVecRotateX(&position, &position, emitter->rotation_30);
                NuVecAdd(&position, &position, &emitter->position);
                params.position = &position;
                NUVEC velocity = {0.0f, type->speed, 0.0f};
                random = NuRandFloatSeeded(&partseed);
                velocity.x += (random + random) * type->velocity_random.x - type->velocity_random.x;
                random = NuRandFloatSeeded(&partseed);
                velocity.y += (random + random) * type->velocity_random.y - type->velocity_random.y;
                random = NuRandFloatSeeded(&partseed);
                velocity.z += (random + random) * type->velocity_random.z - type->velocity_random.z;
                NuVecRotateZ(&velocity, &velocity, emitter->rotation_2c);
                NuVecRotateY(&velocity, &velocity, emitter->rotation_2e);
                NuVecRotateX(&velocity, &velocity, emitter->rotation_30);
                params.velocity = &velocity;
                params.gravity = type->gravity;
                params.field_20 = type->bounce;
                i32 variant = 0;
                if (type->variant_count != 0) {
                    if (type->variant_mode == 0)
                        variant = static_cast<i32>(NuRandFloatSeeded(&partseed) * 65535.0f) % type->variant_count;
                    else if (type->variant_mode == 1) {
                        i8 next = static_cast<i8>(emitter->field_33 + 1);
                        if (type->variant_count > next)
                            variant = next;
                        emitter->field_33 = variant;
                    }
                }
                params.field_28 = type->effect_ids[variant];
                nuhspecial_s special;
                if (static_cast<u16>(params.field_28 - 9998) <= 1)
                    params.special = NULL;
                else if (params.field_28 != -1) {
                    params.special = &special;
                    NuGScnGetSpecial(&special, part_scene[type->scene_indices[variant]], params.field_28);
                }
                f32 lifetime = type->lifetime;
                params.field_a4 = lifetime + NuRandFloatSeeded(&partseed) * type->lifetime_random;
                NUMTX_ALIGNED16 matrix;
                NuMtxSetIdentity(&matrix);
                f32 rotation_x = static_cast<f32>(type->rotation[0]);
                f32 random_x = NuRandFloatSeeded(&partseed);
                f32 range_x = static_cast<f32>(type->rotation_random[0]);
                f32 rotation_y = static_cast<f32>(type->rotation[1]);
                f32 random_y = NuRandFloatSeeded(&partseed);
                f32 range_y = static_cast<f32>(type->rotation_random[1]);
                f32 rotation_z = static_cast<f32>(type->rotation[2]);
                f32 random_z = NuRandFloatSeeded(&partseed);
                f32 range_z = static_cast<f32>(type->rotation_random[2]);
                for (i32 axis = 0; axis < 3; ++axis) {
                    params.field_a8[axis] = type->rotation[axis];
                    params.field_a8[axis + 3] = type->rotation_random[axis];
                }
                NuMtxRotateX(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_x + random_x) * range_x + rotation_x - range_x)));
                NuMtxRotateY(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_y + random_y) * range_y + rotation_y - range_y)));
                NuMtxRotateZ(&matrix, static_cast<i16>(
                                          static_cast<i32>((random_z + random_z) * range_z + rotation_z - range_z)));
                matrix.m30 = position.x;
                matrix.m31 = position.y;
                matrix.m32 = position.z;
                params.matrix = &matrix;
                params.flags = type->flags & ~0x60000;
                if (type->trail_effects[0] != -1) {
                    params.field_60 = type->trail_effects[0];
                    params.flags |= 0x40000;
                }
                if (type->trail_effects[1] != -1) {
                    params.field_64 = type->trail_effects[1];
                    params.flags |= 0x40000;
                }
                if (type->attached_effect != -1) {
                    params.field_58 = type->attached_effect;
                    params.flags |= 0x20000;
                }
                params.field_68 = type->trail_rates[0];
                params.field_6c = type->trail_rates[1];
                params.field_70 = type->impact_effect;
                if (type->impact_part == -1 && type->impact_part_name[0] != 0)
                    type->impact_part = PARTLookupType(type->impact_part_name);
                params.field_7c = type->effect_scale;
                params.field_74 = type->impact_part;
                params.field_98 = type->field_160;
                params.field_9c = type->field_164;
                params.field_a0 = type->field_168;
                params.field_78 = type->kill_effect;
                AddPart(&params);
                if (emitter->field_3c != 0) {
                    PART_EMIT_SOUND(0, 3);
                    PART_EMIT_SOUND(1, 3);
                    PART_EMIT_SOUND(2, 3);
                    PART_EMIT_SOUND(3, 3);
                }
                type->last_used_time = emission_time;
            }
            emitter->time_20 = emission_time;
            emission_time += interval;
        }
    }
    for (i32 i = 0; i <= last_switch_change; ++i)
        object_switches[switch_changes[i][0]] = switch_changes[i][1];
}

extern "C" {
    void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z) {
        if (index != -1 && part_emits[index].effect_id != -1) {
            part_emits[index].position.x = x;
            part_emits[index].position.y = y;
            part_emits[index].position.z = z;
        }
    }

    void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation) {
        if (index != -1 && part_emits[index].effect_id != -1) {
            part_emits[index].rotation_30 = z_rotation;
            part_emits[index].rotation_2e = y_rotation;
            part_emits[index].rotation_2c = x_rotation;
        }
    }

    i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count) {
        i32 index = -1;
        if (effect != -1) {
            part_type_s *type = &part_types[effect];
            if (type->effect_ids[0] != -1) {
                index = AddPARTEffect(effect, position);
                if (index != -1) {
                    part_emit_s *emitter = &part_emits[index];
                    emitter->shots_remaining = count;
                    emitter->time_24 = partglobaltime;
                    f32 end = partglobaltime + type->emission_period;
                    emitter->time_28 = end + NuRandFloatSeeded(&partseed) * type->emission_period_random;
                    emitter->time_20 = partglobaltime - 1.0f / type->emission_rate;
                }
            }
        }
        return index;
    }

    void UpdateParts(f32 time) {
        i32 key;
        NUVEC direction;
        NUMTX_ALIGNED16 movement_orientation;
        NUMTX_ALIGNED16 impact_orientation;
        NUMTX_ALIGNED16 emitter_orientation;
        partglobaltime += time;
        if (partglobaltime > 900.0f)
            PartTimeSlip();
        PartCleanupTypes();
        part_platimpactcnt = 0;
        UpdatePartEmits(time);
        PART_s *parts = Part;
        for (i32 i = 0; i < MAXPARTS; ++i) {
            PART_s *part = &parts[i];
            if ((part->active & 1) == 0)
                continue;
            memcpy(&part->previous_transform, &part->transform, sizeof(NUMTX));
            part->scale_time += time;
            if ((part->active & 2) == 0) {
                f32 movement_time = time;
                if ((part->flags & 0x400) == 0) {
                    if (time >= part->elapsed) {
                        part->active = (part->active & ~0x60) | (part->target_radius > part->radius ? 0x20 : 0);
                        if ((part->active & 4) != 0) {
                            if ((part->flags & 0x80000) != 0 && (part->active & 0x10) != 0 && part_platimpactcnt < 4) {
                                part_platimpactlist[part_platimpactcnt++] = part->field_200;
                            }
                            if ((part->flags & 0x100) != 0) {
                                part->active |= 2;
                                if (part->stop_callback != NULL)
                                    part->stop_callback(part);
                                if ((part->flags & 1) != 0 || (part->active & 0x10) != 0)
                                    KillPart(part, 0);
                            } else {
                                movement_time -= part->elapsed;
                                part->position.x = part->impact_position.x;
                                part->position.y = part->impact_position.y;
                                part->position.z = part->impact_position.z;
                                FullReflect(&part->impact_normal, &part->velocity, &part->velocity);
                                if ((part->flags & 0x80) != 0)
                                    NewPartRotation(part);
                                if ((part->flags & 0x100000) != 0)
                                    NewPartOrderedRotation(part);
                                part->velocity.x *= part->field_0ec;
                                part->velocity.z *= part->field_0ec;
                                if (part->impact_normal.y > 0.5f) {
                                    part->velocity.y *= part->field_0ec;
                                    if (part->velocity.x * part->velocity.x + part->velocity.y * part->velocity.y +
                                            part->velocity.z * part->velocity.z <
                                        0.1f) {
                                        part->active |= 2;
                                        if (part->stop_callback != NULL)
                                            part->stop_callback(part);
                                        if ((part->flags & 1) != 0 || (part->active & 0x10) != 0 ||
                                            (part->render_flags & 1) != 0)
                                            KillPart(part, 0);
                                    } else if ((part->render_flags & 1) != 0) {
                                        KillPart(part, 0);
                                    }
                                }
                            }
                            if (part->field_1f0 != -1) {
                                key = -1;
                                NuMtxSetIdentity(&impact_orientation);
                                NuMtxAlignY(&impact_orientation, &part->impact_normal);
                                AddScaledFiniteShotDebrisEffect(&key, part->field_1f0, &part->impact_position, NULL,
                                                                NULL, 1, part->field_1fc);
                                DebrisOrientationMtx(key, &impact_orientation);
                            }
                            if (part->field_1f8 != -1)
                                AddFiniteShotPART(part->field_1f8, &part->impact_position, 1);
                            if (part->field_1b8 != NULL)
                                part->field_1b8(part);
                        }
                        if (part_raycasts_enabled != 0)
                            CastPart(part, 0.1f);
                        if ((part->active & 4) != 0 && movement_time >= part->elapsed)
                            movement_time = 0.0f;
                    }
                    part->elapsed -= movement_time;
                }
                if (movement_time > 0.0f) {
                    if (part->move_callback != NULL) {
                        part->move_callback(part, movement_time);
                    } else {
                        part->position.x += part->velocity.x * movement_time;
                        part->position.y += part->velocity.y * movement_time;
                        part->velocity.y += part->gravity * movement_time;
                        part->position.z += part->velocity.z * movement_time;
                    }
                }
                if ((part->render_flags & 1) == 0 && part->field_0f0 != 2000000.0f &&
                    part->position.y < part->field_0f0)
                    part->render_flags |= 1;
                part->bounds_min.x = part->position.x - part->target_radius;
                part->bounds_max.x = part->position.x + part->target_radius;
                part->bounds_min.y = part->position.y - part->target_radius;
                part->bounds_max.y = part->position.y + part->target_radius;
                part->bounds_min.z = part->position.z - part->target_radius;
                part->bounds_max.z = part->position.z + part->target_radius;
                if ((part->active & 0x60) == 0x20) {
                    part->time_step += time;
                    part->radius = part->time_step / PARTSCALEUPTIME * part->target_radius;
                    if (part->radius > part->target_radius) {
                        part->radius = part->target_radius;
                        part->active &= ~0x60;
                    }
                } else if ((part->active & 0x60) == 0x40) {
                    part->time_step -= time;
                    if (part->time_step <= 0.0f)
                        KillPart(part, 0);
                    else
                        part->radius = part->time_step / PARTSCALEUPTIME * part->target_radius;
                }
                if ((part->flags & 0x80) != 0) {
                    i32 angle = static_cast<i32>(part->field_124[3] * time);
                    switch (part->rotation_axis_1) {
                        case 1:
                            NuMtxPreRotateX(&part->transform, angle);
                            break;
                        case 2:
                            NuMtxPreRotateY(&part->transform, angle);
                            break;
                        case 3:
                            NuMtxPreRotateZ(&part->transform, angle);
                            break;
                    }
                    angle = static_cast<i32>(part->field_124[4] * time);
                    switch (part->rotation_axis_2) {
                        case 1:
                            NuMtxPreRotateX(&part->transform, angle);
                            break;
                        case 2:
                            NuMtxPreRotateY(&part->transform, angle);
                            break;
                        case 3:
                            NuMtxPreRotateZ(&part->transform, angle);
                            break;
                    }
                }
                if ((part->flags & 0x100000) != 0) {
                    if (part->field_124[0] != 0)
                        NuMtxPreRotateX(&part->transform, static_cast<i32>(part->field_124[0] * time));
                    if (part->field_124[1] != 0)
                        NuMtxPreRotateY(&part->transform, static_cast<i32>(part->field_124[1] * time));
                    if (part->field_124[2] != 0)
                        NuMtxPreRotateZ(&part->transform, static_cast<i32>(part->field_124[2] * time));
                }
                if ((part->flags & 0x800) != 0) {
                    direction.x = -part->velocity.x;
                    direction.y = -part->velocity.y;
                    direction.z = -part->velocity.z;
                    NuMtxSetIdentity(&movement_orientation);
                    NuMtxLookAtZ(&movement_orientation, &direction);
                    memmove(&part->transform, &movement_orientation, 0x30);
                }
            }
            if ((part->active & 1) != 0) {
                if ((part->flags & 2) != 0 && part->field_1bc != NULL)
                    part->field_1bc(part);
                if ((part->active & 1) != 0) {
                    if (part->field_1c4 != NULL)
                        part->field_1c4(part);
                    if (part->field_100 > 0.0f) {
                        part->field_100 -= time;
                        if (part->field_100 <= 0.0f)
                            KillPart(part, 0);
                    }
                }
            }
            if ((part->flags & 0x20000) != 0 && part->debris_key != NULL) {
                NuMtxSetIdentity(&emitter_orientation);
                direction = part->velocity;
                direction.x = -direction.x;
                direction.y = -direction.y;
                direction.z = -direction.z;
                NuMtxLookAtY(&emitter_orientation, &direction);
                DebrisOrientationMtx(*part->debris_key, &emitter_orientation);
                DebrisEmitterPos(*part->debris_key, part->position.x, part->position.y, part->position.z);
                if (((part->flags & 0x200000) != 0 && (part->active & 2) != 0) || (part->flags & 0x1000) != 0)
                    DebrisStatusAlwaysOff(part->debris_key);
            }
            if (time != 0.0f) {
                if ((part->flags & 0x40000) != 0 && part->field_1e0 != -1 &&
                    !((part->flags & 0x200000) != 0 && (part->active & 2) != 0) && (part->flags & 0x1000) == 0)
                    AddScaledVariableShotDebrisEffect1(part->field_1e0, &part->position,
                                                       static_cast<i32>(part->field_1e8), time, 0, 0, NULL,
                                                       part->field_1fc);
                if ((part->flags & 0x40000) != 0 && part->field_1e4 != -1 &&
                    !((part->flags & 0x200000) != 0 && (part->active & 2) != 0) && (part->flags & 0x1000) == 0)
                    AddScaledVariableShotDebrisEffect1(part->field_1e4, &part->position,
                                                       static_cast<i32>(part->field_1ec), time, 0, 0, NULL,
                                                       part->field_1fc);
            }
            if (part->lighting_template != NULL) {
                if ((part->flags & 0x1000) == 0) {
                    rtlDynamicSetPos(*part->lighting_template, &part->position);
                } else {
                    direction.x = direction.y = direction.z = 10000.0f;
                    rtlDynamicSetPos(*part->lighting_template, &direction);
                }
            }
            if (part->field_1ac != NULL)
                part->field_1ac(part);
            part->field_208 = 0;
            if ((part->active & 1) == 0) {
                KillPart(part, 0);
                memset(part, 0, sizeof(PART_s));
            }
        }
    }

    i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale) {
        // Retail uses this inclusive upper check; loaded type IDs are 0..127.
        if (effect_index <= 0) {
            return -1;
        }
        if (effect_index > 128 || part_types[effect_index].effect_ids[0] == -1) {
            return -1;
        }
        if (part_types[effect_index].scale != 1.0f) {
            // The final word identifies the original, unscaled type.
            effect_index = part_types[effect_index].field_174;
            if (part_types[effect_index].effect_ids[0] == -1) {
                return -1;
            }
        }
        if (requested_scale == 1.0f && effect_index != 0) {
            return effect_index;
        }
        if (requested_scale < 0.01f) {
            requested_scale = 0.01f;
        }

        i32 closest_index = effect_index;
        f32 ratio = requested_scale / part_types[effect_index].scale;
        f32 closest_distance = ratio <= 1.0f ? 1.0f - ratio : ratio - 1.0f;
        for (i32 i = 0; i < 128; ++i) {
            if (__builtin_expect(part_types[i].effect_ids[0] != -1 && part_types[i].field_174 == static_cast<u32>(effect_index), 1)) {
                ratio = requested_scale / part_types[i].scale;
                f32 distance = ratio <= 1.0f ? 1.0f - ratio : ratio - 1.0f;
                if (__builtin_expect(distance < closest_distance, 1)) {
                    closest_index = i;
                    closest_distance = distance;
                }
            }
        }
        if (closest_index != 0 && closest_distance < 1.1f) {
            return closest_index;
        }

        for (i32 i = 0; i < 128; ++i) {
            if (part_types[i].effect_ids[0] != -1) {
                continue;
            }
            part_type_s *scaled = &part_types[i];
            part_type_s *source = &part_types[effect_index];
            *scaled = *source;
            edpartScaleType(i, requested_scale);
            scaled->last_used_time = partglobaltime;
            scaled->scale = requested_scale;
            scaled->field_174 = effect_index;
            char shortened[16];
            const char *base_name = source->name;
            const size_t name_length = strlen(base_name);
            if (__builtin_expect(name_length > 12, 1)) {
                memcpy(shortened, base_name, name_length + 1);
                shortened[12] = '\0';
                base_name = shortened;
            }
            sprintf(scaled->name, "%s%03d", base_name, i);
            ++part_types_used;
            return i != 0 ? i : closest_index;
        }
        return closest_index;
    }

    i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale) {
        i32 scaled = CreateScaledPARTEffect(effect, scale);
        if (scaled != -1)
            AddFiniteShotPART(scaled, position, count);
        return scaled;
    }

    void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale) {
        i32 scaled = CreateScaledPARTEffect(effect, scale);
        if (scaled != -1)
            AddVariableShotPARTEffect(scaled, position, rate, time, orientation);
    }

    void KillPartsByScene(NUGSCN *scene) {
        PART_s *part = Part;
        for (i32 index = 0; index < MAXPARTS; ++index, ++part) {
            if ((part->active & 1) != 0 && part->source_special != NULL && part->special.scene == scene) {
                memset(&part->special, 0, sizeof(part->special));
                part->active &= ~1;
                part->source_special = NULL;
            }
        }
    }

    void edpartClearPage(i8 page) {
        NuThreadDisableThreadSwap();
        CheckPartCount();
        if (part_page_on[page] != 0)
            edpartStopPage(page);
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].page == page && part_types[index].name[0] != 0) {
                part_types[index].name[0] = 0;
                part_types[index].effect_ids[0] = -1;
                --part_types_used;
            }
        }
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].page == page && part_emits[index].effect_id != -1) {
                part_emits[index].effect_id = -1;
                --part_emits_used;
            }
        }
        CheckPartCount();
        for (i32 index = 0; index < 32; ++index) {
            if (part_scene_pageid[index] == page) {
                KillPartsByScene(part_scene[index]);
                part_scene_pageid[index] = -1;
                part_scene[index] = NULL;
            }
        }
        NuThreadEnableThreadSwap();
        part_page_used[page] = 0;
    }

}

void edpartDestroyAllPages() {
    for (i32 i = 0; i < 8; i++) {
        if (part_page_used[i])
            edpartClearPage(i);
    }
}

