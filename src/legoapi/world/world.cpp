#include "decomp.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/surfaces.h"

#include <stdio.h>
#include <string.h>

#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/items/objects/grabber.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/NuDeviceSpecs.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edgra.h"

// Globals shared across world loading — defined here until moved to globals.cpp
TIMER LevelTimer;

void (*WorldInfo_InitMenuFn)(WORLDINFO *, i32 *, i32 *) = NULL;
void (*WorldInfo_InitLastFn)(WORLDINFO *) = NULL;
volatile i32 abort_load = 0;
char ConfigBuffer[0x10000];
i32 numtl_force_mipmode = 0;
i32 GAMEDEMO = 0;
NUGSCN *big_icon_scene = NULL;
NUGSCN *area_scene = NULL;
NUGSCN *things_scene = NULL;
void *things_scene_terrain = NULL;
LEVELDATA *PLATFORM_LDATA = NULL;
LEVELDATA *RETAKED_LDATA = NULL;
LEVELDATA *CREDITS_LDATA = NULL;
i32 Text_Language = 1;
nufpcomjmp_s LevelConfigKeywords_AfterLoad;

// --- Players globals ---
// NOTE: `WORLD` (WORLDINFO) field offsets in world.h are an approximation; the
// original binary reaches the level/area pointers through WORLDINFO[0x128]
// while world.h places them at 0x130/0x134.  Players_Init still links and runs
// with the world.h layout but its level/area guard bytes will differ until that
// WORLDINFO drift is corrected.

GameObject_s *Player[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
i32 PLAYERCOUNT = 0;
i32 netclient = 0;
i32 UsePlayerList = 0;
i32 makeplayerlist_freeplay = 0;
i16 PlayerList[8];
i32 PlayerID[2] = {-1, -1};
i16 Area_PlayerIDList[9];
APICHARACTERMODELLIST_s Area_StoryModelList[52];
PLAYERPROGRESS PlayerProgress[8];
i32 Hub_UsePlayerList = 0;
i32 LevelChangesInArea = 0;
i32 bonusmodearcade = 0;
i32 VehicleArea = 0;
SOCKPOSITION OldPlrSPos[8];
BATARANG_s Batarang[8];
void *PlayerSuit[8];
u8 PlayerTorpedoCount[8];
COINPACKET CoinPacket[2];
u32 BackUpPlayers[0x872];
GizForceLOSState_s GizForceLOSInfo[2];
i32 DEFAULT_PLAYERHITPOINTS = 8;
u32 LEGOOBJ_DEFAULTLASTCOIN = -1;

// --- World-module helpers (kept with the WorldInfo API) ---

void SetAreaPickupGravity(i32 area, i32 level) {
    AreaPickupGravity = -6.0f;
    if (area < 0 || area >= AREACOUNT) {
        return;
    }
    AREADATA *ad = &ADataList[area];
    if (ad->flags & AREAFLAG_NOPICKUPGRAVITY) {
        if ((*(u8 *)((char *)LDataList + level * 0x144 + 0x66) & 0x40) == 0) {
            AreaPickupGravity = 0.0f;
            return;
        }
    }
    if (ad->flags & AREAFLAG_VEHICLE_AREA) {
        AreaPickupGravity = -20.0f;
    }
}
void StoreSceneProgress(NUGSCN *gscn, SCENEPROGRESS_s *progress, i32 param) {
    if (progress == NULL || gscn == NULL)
        return;
    memset(progress, 0, sizeof(*progress));
    i32 count = NuSpecialGetNumSpecials(gscn);
    nuhspecial_s special;
    for (i32 i = 0; i < count; ++i) {
        if (i != 0)
            NuSpecialGetNext(&special);
        else
            NuSpecialGetFirst(gscn, &special, 1);
        progress->specials[i].visible = NuSpecialGetVisibilityFn(&special);
        if (gscn->instance_animation_data == NULL)
            continue;
        nuinstanim_s *animation = NuSpecialGetInstAnim(&special);
        if (animation == NULL || gscn->instance_animation_data[animation->anim_ix] == NULL)
            continue;
        progress->specials[i].frame =
            animation->ltime <= 2048.0f && animation->ltime >= 0.0f ? static_cast<u32>(animation->ltime) : 1;
        progress->specials[i].playing = animation->playing;
        progress->specials[i].waiting = animation->waiting;
        progress->specials[i].repeating = animation->repeating;
        progress->specials[i].tfactor = animation->tfactor;
        if (param != 0)
            animation->fparam1 = animation->tfactor;
    }
}

void StoreLevelProgressFn(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress, i32 area_progress) {
    i32 index;
    if (area_progress != 0) {
        if (world == NULL || world->area == NULL)
            return;
        index = world->area->level_count;
    } else {
        if (VADER_ADATA != NULL && VADER_ADATA == WORLD->area)
            return;
        if (BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area && bonus_gunship_store_progress_flag == 0)
            return;
        if (world == NULL)
            return;
        index = (i8)world->current_level->area_level_index;
        if (world->area != NULL && (world->area->flags & 4) != 0)
            goto store_flags;
    }
    StoreProgressAICharacter(progress);
    Grabber_StoreProgress(world, progress);
    GizmoSysStoreProgress(world->gizmo_sys, world, index);
    if (progress != NULL) {
        GizFlowStoreProgress(world->giz_flow, &progress->giz_flow_progress);
        StoreSceneProgress(world->current_gscn, reinterpret_cast<SCENEPROGRESS_s *>(progress), 0);
    }
    GameAnimSys_StoreProgress(world->game_anim_sys, index);
    for (i32 i = 0; i < world->processor_count; ++i) {
        if (progress == NULL)
            continue;
        LEVELSCRIPTPROCESS &processor = world->processors[i];
        if (NuStrLen(processor.name) == 0)
            continue;
        for (i32 j = 0; j < 32; ++j) {
            if (NuStrLen(progress->scripts[j].name) == 0) {
                NuStrCpy(world->level_progress->scripts[j].name, processor.name);
                for (i32 k = 0; k < 4; ++k)
                    progress->scripts[j].params[k] = processor.processor.params[k];
                break;
            }
            if (NuStrICmp(progress->scripts[j].name, processor.name) != 0) {
                continue;
            }
            memcpy(progress->scripts[j].params, processor.processor.params, 16);
            break;
        }
    }
store_flags:
    if (progress != NULL) {
        if (world->level_progress != progress)
            memmove(progress->disabled_effect_names, world->level_progress->disabled_effect_names, 0xc0);
        reinterpret_cast<u8 *>(&progress->flags)[0] =
            (reinterpret_cast<u8 *>(&progress->flags)[0] & ~4) | ((world->field_0x5174 & 1) << 2) | 2;
    }
}

void SaveSceneObjectAnimTFactors(NUGSCN *gscn) {
    i32 count;
    char *p;
    void *obj;

    if (gscn == NULL) {
        return;
    }
    if (*(void **)((char *)gscn + 0x110) != NULL) {
        return;
    }
    count = *(i32 *)((char *)gscn + 0x1c);
    p = *(char **)((char *)gscn + 0x20) + count * 0x50;
    if (count == 0) {
        return;
    }
    do {
        p -= 0x50;
        obj = *(void **)(p + 0x48);
        if (obj != NULL) {
            *(f32 *)((char *)obj + 0x54) = *(f32 *)((char *)obj + 0x40);
        }
        count--;
    } while (count != 0);
}
void CalculateWorldSize(WORLDINFO *world) {
    NUVEC minimum = v000;
    NUVEC maximum = v000;
    if (world == NULL || world->current_gscn == NULL) {
        return;
    }

    NUGSCN *scene = world->current_gscn;
    for (i32 i = 0; i < scene->portal_instance_count * 2; i += 2) {
        // Retail walks the boxes as consecutive first/second vectors.
        NUVEC4 *first = &((NUVEC4 *)scene->portal_boxes)[i];
        NUVEC4 *second = &((NUVEC4 *)scene->portal_boxes)[i + 1];
        NUVEC box_maximum = *(NUVEC *)second;
        NUVEC box_minimum = *(NUVEC *)first;
        if (scene->display_list != NULL &&
            (scene->display_list->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
            box_minimum.z = first->z - second->z;
            box_minimum.x = first->x - second->x;
            box_minimum.y = first->y - second->y;
            box_maximum.x = first->x + second->x;
            box_maximum.y = first->y + second->y;
            box_maximum.z = first->z + second->z;
        }
        minimum.x = MIN(box_minimum.x, minimum.x);
        minimum.y = MIN(box_minimum.y, minimum.y);
        minimum.z = MIN(box_minimum.z, minimum.z);
        maximum.x = MAX(box_maximum.x, maximum.x);
        maximum.y = MAX(box_maximum.y, maximum.y);
        maximum.z = MAX(box_maximum.z, maximum.z);
    }

    world->level_max[0] = maximum.x;
    world->level_max[1] = maximum.y;
    world->level_max[2] = maximum.z;
    world->level_min[0] = minimum.x;
    world->level_min[1] = minimum.y;
    world->level_min[2] = minimum.z;
}

WORLDINFO WorldInfo[2];
WORLDINFO *WORLD = &WorldInfo[0];

f32 g_BgLoadDelayHackTimer;

volatile i32 waiting_for_level;
i32 level_already_loaded = -1;
i32 next_level;

void WorldInfo_Load(WORLDINFO *world) {
    LOG_INFO("world load world=%p", (void *)world);
    char buf[268];
    char titles[64];
    NUGSCN *cutscene_scene;
    i32 *page_handles;
    i32 aligned_buf;
    char ai_name[32];
    char title_light_path[] = "levels\\titles\\titles";
    i32 ai_buf_size;
    i32 rtl_id;
    char *light_path;

    Level_LoadConfigFile(world);

    if (abort_load != 0 || (world->config_count > 0 && (LevelConfig_BeforeLoad(world->current_level, ConfigBuffer,
                                                                               Level_ConfigBeforeLoad_GameKeywords),
                                                        abort_load != 0))) {
        goto abort;
    }

    if ((world->current_level->flags & LEVEL_UNKNOWN_FLAG_4) != 0) {
        // Align giz_buffer
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);

        if (world->current_level == TITLES_LDATA) {
            NuStrCpy(buf, "levels\\titles\\");
            switch (Text_Language) {
                case 2:
                    NuStrCpy(titles, "titles_french");
                    break;
                case 3:
                    NuStrCpy(titles, "titles_spanish");
                    break;
                case 4:
                    NuStrCpy(titles, "titles_german");
                    break;
                case 5:
                    NuStrCpy(titles, "titles_italian");
                    break;
                case 8:
                    NuStrCpy(titles, "titles_danish");
                    break;
                default:
                    if (Text_Language != 0x12) {
                        NuStrCpy(titles, "titles_uk");
                    } else {
                        NuStrCpy(titles, "titles_us");
                    }
                    break;
            }
            NuStrCat(buf, titles);
        } else if (world->current_level == (LEVELDATA *)PLATFORM_LDATA) {
            NuStrCpy(buf, "levels\\episode_v\\cloudcityescape\\cloudcityescape_b\\cloudcityescape_b");
        } else {
            NuStrCpy(buf, world->config_file);
        }
        NuStrCat(buf, ".gsc");

        numtl_force_mipmode = (i32)(u8)world->current_level->mipmap_mode + 1;
        world->current_gscn = NuGScnRead(&world->giz_buffer, world->unknown_0108, buf);
        numtl_force_mipmode = 0;

        StoreSceneProgress(world->current_gscn, (SCENEPROGRESS_s *)&world->progress_data, 1);
        SaveSceneObjectAnimTFactors(world->current_gscn);

        if (world->current_gscn != NULL) {
            CalculateWorldSize(world);
        }
        if (abort_load != 0)
            goto abort;
    }

    // Load pictures for titles/credits
    if (world->current_level == TITLES_LDATA || world->current_level == CREDITS_LDATA) {
        numtl_force_mipmode = (i32)(u8)world->current_level->mipmap_mode + 1;
        world->scene = NuGScnRead(&world->giz_buffer, world->unknown_0108, "levels\\titles\\pictures.gsc");
        if (abort_load != 0)
            goto abort;
    }

    // Create game animation system
    world->game_anim_sys = (GAMEANIMSYS_s *)GameAnimSys_Create(&world->giz_buffer, &world->unknown_0108);

    // Load particles
    Particles_Load(world, debris_name, 400, 0x93);
    if (abort_load != 0)
        goto abort;
    LoadPartFile(world);
    if (abort_load != 0)
        goto abort;

    // Configure bolt types
    if (world->config_count > 0) {
        BoltTypes_Configure(world, ConfigBuffer);
    }

    // Create antinode system
    world->game_antinode_sys = (GAMEANTINODESYS_s *)GameAntnode_CreateSys(
        world, &world->giz_buffer, &world->unknown_0108, (i32)(u16)world->current_level->max_antinodes);

    // Create gizmo system
    world->gizmo_sys = (GIZMOSYS_s *)CreateGizmoSys(world, &world->giz_buffer, &world->unknown_0108);

    // Load editor splines
    sprintf(buf, "%s%s", world->config_file, ".spl");
    LoadEditorSplines(buf, &world->giz_buffer, &world->unknown_0108);

    GizmoBlowupResetNameTable();
    LoadGizmoSys((GIZMOSYS_s *)world->gizmo_sys, world, world->config_file);
    if (abort_load != 0)
        goto abort;

    // Area-specific loading
    if (world->area == NULL) {
        goto after_area;
    }
    if (world->area == HUB_ADATA && GAMEDEMO == 0) {
        Hub_LoadAndFixUpMiniKits(world, &world->giz_buffer, &world->unknown_0108);
        if (abort_load != 0)
            goto abort;
        goto after_area;
    }
    if ((world->current_level->flags & LEVEL_STATUS) != 0 && world->area->minikit_id != -1) {
        MiniKit_Load(&world->minikit, (i32)(i16)world->area->minikit_id, &world->giz_buffer, &world->unknown_0108,
                     NULL);
        if (world->minikit.gscn != NULL) {
            MiniKit_InitPieces(&world->minikit, 10, &world->giz_buffer, &world->unknown_0108);
        }
        if (abort_load != 0)
            goto abort;
    }

after_area:
    // Load big icon scene for hub/status levels
    if ((world->current_level == HUB_LDATA || (world->current_level->flags & LEVEL_STATUS) != 0) &&
        big_icon_scene == NULL) {
        world->icons_gscn = NuGScnRead(&world->giz_buffer, world->unknown_0108, "stuff\\icons\\starwars_icons_all.gsc");
    }

    // Determine which scene to use for cutscenes
    cutscene_scene = area_scene;
    if (cutscene_scene == NULL || world->area == NULL || (world->area->flags & AREAFLAG_OVERRIDE_THINGS_SCENE) == 0) {
        cutscene_scene = things_scene;
    }

    // Load cutscenes
    page_handles = (i32 *)&world->page_pp;
    world->cutscene_sys =
        (CUTSYS *)CutScenes_Load(ConfigBuffer, world->current_gscn, cutscene_scene, world->page_pp, &world->giz_buffer,
                                 // Sublevel and level indices follow the two buffer arguments.
                                 &world->unknown_0108, world->level_sub_id, world->level_idx, world);
    if (abort_load != 0)
        goto abort;

    // Character scenes
    aligned_buf = ALIGN((i32)world->giz_buffer.addr, 4);
    world->minikit.character_scenes = reinterpret_cast<CHARSCENE_s *>(aligned_buf);
    world->giz_buffer.addr = (usize)(aligned_buf + CHARCOUNT * 0x10);
    CharScenes_LevelLoad(world);

    // SockSys for certain level types
    if ((world->current_level->flags & (LEVEL_GAMEPLAY | LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == LEVEL_GAMEPLAY) {
        world->sock_sys = SockSysInit(&world->giz_buffer, world->unknown_0108, world->current_gscn);
    }

    LevelSplines_InitForLevel(world);
    LevelObjects_InitForLevel(world);
    BoltTypes_Init(world);

    // Config-based subsystem initialization
    if (world->config_count > 0) {
        LevelConfig_AfterLoad(world->current_level, ConfigBuffer, &LevelConfigKeywords_AfterLoad);
        EquivalentObjects_Configure(world, ConfigBuffer);
        Teleports_Configure(world, ConfigBuffer);
        Doors_Configure(world, ConfigBuffer);
        Faders_Configure(world, ConfigBuffer);
        CharPlatforms_Configure(world, ConfigBuffer);
        Grabber_Configure(world, ConfigBuffer);
        Pulses_Configure(world, ConfigBuffer);
        TrafficAnimSys_Configure(world, ConfigBuffer);
        SpecialMiniKits_Configure(world, ConfigBuffer);
        GizForceSFX_Configure(world, ConfigBuffer);
    }

    // SockSys configuration
    if (world->sock_sys != NULL) {
        if (world->config_count > 0) {
            SockSys_Configure(world->sock_sys, ConfigBuffer, 0, &world->giz_buffer, &world->unknown_0108,
                              world->current_gscn);
        }
        SockSys_GenerateData(world->sock_sys, &world->giz_buffer, world->unknown_0108);
        if (abort_load != 0)
            goto abort;
    }

    // Terrain/grass/bridge loading
    if (world->area != NULL &&
        (world->area->flags & (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA)) ==
            (AREAFLAG_VEHICLE_AREA | AREAFLAG_BONUS_AREA) &&
        (world->current_level->flags & (LEVEL_STATUS | LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0) {
        CharacterMiniKits_Load(&MiniKitCollection, world, &world->giz_buffer, &world->unknown_0108);
        if (abort_load != 0)
            goto abort;
    } else {
        LoadTerrainFile(world);
        if (abort_load != 0)
            goto abort;
        LoadGrassFile(world);
        if (abort_load != 0)
            goto abort;
        LoadBridgeFile(world);
        if (abort_load != 0)
            goto abort;
    }

    // AI system loading
    if ((world->current_level->flags & (LEVEL_GAMEPLAY | LEVEL_INTRO | LEVEL_MIDTRO | LEVEL_OUTRO)) == LEVEL_GAMEPLAY &&
        world->current_level != (LEVELDATA *)PLATFORM_LDATA) {
        world->ai_loaded = 0;
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        ai_name[0] = 'a';
        ai_name[1] = 'i';
        ai_name[2] = 0;
        ai_buf_size = 0x1cc00;
        if (RETAKED_LDATA != NULL && world->current_level == (LEVELDATA *)RETAKED_LDATA) {
            ai_buf_size = 0x1e800;
        }
        world->ai_sys = (AISYS *)AISysLoad(&world->giz_buffer, &world->unknown_0108, ai_buf_size, world->current_gscn,
                                           world->current_level->dir, world->current_level->name, ai_name);
        world->ai_path_cnx_control_sys =
            (AIPATHCNXCONTROLSYS_s *)AIPathCnxControlSysCreate(&world->giz_buffer, &world->unknown_0108, 0x40);
        world->ai_path_cnx_helper_sys =
            (AIPATHCNXHELPERSYS_s *)AIPathCnxHelperSysCreate(&world->giz_buffer, &world->unknown_0108, 0x20);
        world->ai_trigger_set_sys =
            (AITRIGGERSETSYS_s *)AITriggerSetSysCreate(&world->giz_buffer, &world->unknown_0108);

        if (world->ai_sys != NULL) {
            world->mech_auto_jump_manager = new MechAutoJumpManager(world->ai_sys);
        }

        GameAIScriptAddLevelSfx(world, &global_aiscripts);
        GameAIScriptAddLevelSfx(world, &world->ai_sys->scripts);

        world->climb_object_sys = (CLIMBOBJECTSYS_s *)CreateClimbObjectSys(
            &world->giz_buffer, &world->unknown_0108, (i32)(u8)world->current_level->max_climb_objs);
    } else {
        world->ai_loaded = 1;
    }

    if (abort_load != 0)
        goto abort;

    // API object system
    world->api_object_sys =
        (APIOBJECTSYS_s *)APIObjectSysInit(sizeof(GameObject_s), &world->giz_buffer, &world->unknown_0108);
    if (abort_load != 0)
        goto abort;

    // Lights
    if ((world->current_level->flags & LEVEL_UNKNOWN_FLAG_4) != 0) {
        light_path = world->current_level == TITLES_LDATA ? title_light_path : world->config_file;
        LoadLights(world, light_path);
        rtl_id = rtlFindByUserId(reinterpret_cast<usize>(world->rtl_set), 1);
        world->rtl_id = rtl_id;
        if (rtl_id == -1) {
            world->light_dir = 0;
        } else {
            rtlGetDirection(reinterpret_cast<usize>(world->rtl_set), rtl_id, (void **)&world->light_dir);
        }
    } else {
        world->rtl_id = -1;
        world->light_dir = 0;
    }

    if (abort_load != 0)
        goto abort;

    // More config-based subsystems
    if (world->config_count > 0) {
        RippleEffects_Configure(world, ConfigBuffer);
        PortalDoors_Configure(world, ConfigBuffer);
        if (abort_load != 0)
            goto abort;
    }

    GizmoSysAddGizmos((GIZMOSYS_s *)world->gizmo_sys, (GIZFLOW_s *)world->giz_flow, world);
    if (abort_load != 0)
        goto abort;

    // Load gizmo flow
    NuStrCpy(buf, world->config_file);
    NuStrCat(buf, ".git");
    world->giz_flow =
        (GIZFLOW_s *)LoadGizFlow(world, (GIZMOSYS_s *)world->gizmo_sys, buf, &world->giz_buffer, &world->unknown_0108);
    if (abort_load != 0)
        goto abort;

    InitSpecialSfx(world);
    LoadSpecialSfxFile(world);
    if (abort_load != 0)
        goto abort;

    // Level load function
    if (world->current_level->load_fn != NULL) {
        world->current_level->load_fn(world, &world->giz_buffer, &world->unknown_0108);
    }

    SetAreaPickupGravity(world->level_sub_id, world->level_idx);
    world->loaded = 1;
    return;

abort:
    WorldInfo_Dump(world);
    WorldInfo_Reset(world, -1);
}

extern "C" {
    extern i16 id_DARTHVADER;
    extern i16 id_THEEMPEROR;
    extern i16 id_GRANDMOFFTARKIN;
    extern i16 id_IMPERIALOFFICER;
    extern i16 id_IMPERIALSHUTTLEPILOT;
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *, u32, u32, i32, i32);
static inline i32 World_AddFreePlayModel(i32 model) {
    if (model == -1 || FreePlayModelCount >= 48)
        return 0;
    if (FreePlayModelCount < 2)
        PlayerList[FreePlayModelCount] = static_cast<i16>(model);
    for (i32 i = 0; i < FreePlayModelCount; ++i) {
        if (FreePlayModelList[i].model_id == model)
            return 0;
    }
    FreePlayModelList[FreePlayModelCount].model_id = static_cast<i16>(model);
    FreePlayModelList[FreePlayModelCount].count = 1;
    ++FreePlayModelCount;
    FreePlayModelList[FreePlayModelCount].model_id = -1;
    return 1;
}

static inline bool World_FreePlayImperial(i32 model) {
    const i8 action = static_cast<i8>(GCDataList[model].uses_weapon_action);
    return action == 8 || action == 1 || model == id_DARTHVADER || model == id_THEEMPEROR ||
           model == id_GRANDMOFFTARKIN || model == id_IMPERIALOFFICER || model == id_IMPERIALSHUTTLEPILOT;
}

void MakeFreePlayModelList(i32 model1, i32 model2, i32 area, i32 level, i32 param5) {
    i16 ids[341];
    i16 imperial_ids[500];
    u32 flags = 0;
    if (WORLD != NULL && WORLD->area != NULL && WORLD->area == HUB_ADATA && bonusmodearcade != 0)
        flags = Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].field8_0x8;

    FreePlayModelCount = 0;
    FreePlayResidentCount = 0;
    FreePlayBonusCount = 0;
    // Empty input must not leave a previous roster visible to category scans.
    FreePlayModelList[0].model_id = -1;
    if (model1 == -1)
        model2 = -1;
    const i32 models[2] = {model1, model2};
    // Area -1 has no resident/vehicle data; never form ADataList[-1].
    AREADATA *data = area == -1 ? NULL : &ADataList[area];
    const u16 area_flags = data == NULL ? 0 : data->flags;
    if ((area_flags & 5) == 5) {
        for (i32 i = 0; i < 2; ++i)
            World_AddFreePlayModel(models[i]);
        Collection_GetIDList(&MiniKitCollection, 0x4000000, 0x4000000, ids, NULL, NULL, makefreeplaymodellist != 0);
        for (i16 *id = ids; *id != -1; ++id)
            World_AddFreePlayModel(*id);
        if (FreePlayModelCount == 1)
            PlayerList[1] = PlayerList[0];
        PlayerList[2] = -1;
    } else {
        for (i32 i = 0; i < 2; ++i) {
            i32 model = models[i];
            if (model == -1 && Player[i] != NULL)
                model = Player[i]->id;
            World_AddFreePlayModel(model);
        }
        if (FreePlayModelCount == 1) {
            PlayerList[1] = PlayerList[0];
            PlayerList[2] = -1;
        } else {
            PlayerList[FreePlayModelCount] = -1;
        }
    }

    if (data != NULL && (flags & 0x10) == 0 && data->hub_player_ids != NULL) {
        for (i16 *id = data->hub_player_ids; *id != -1; ++id) {
            World_AddFreePlayModel(*id);
            // Retail counts resident entries, including a duplicate or one
            // beyond the roster cap, rather than only successful insertions.
            ++FreePlayResidentCount;
        }
    }
    if (data != NULL && (data->flags & 1) != 0) {
        if ((data->flags & 4) == 0) {
            const i32 second = PlayerList[1];
            if (second == -1 || (CDataList[second].model_flags & 0x2000) == 0 || Collection_Got(second) == 0)
                PlayerList[1] = PlayerList[0];
            const i32 count = VehicleCollection.count_y < 340 ? VehicleCollection.count_y : 340;
            for (i32 i = 0; i < count; ++i)
                ids[i] = VehicleCollection.list[i].id;
            ids[count] = -1;
            for (i16 *id = ids; *id != -1; ++id) {
                if (World_AddFreePlayModel(*id) != 0)
                    ++FreePlayBonusCount;
            }
        } else {
            const i32 second = PlayerList[1];
            if (second == -1 || (CDataList[second].model_flags & 0x4000000) == 0 || Collection_Got(second) == 0)
                PlayerList[1] = PlayerList[0];
        }
    } else {
        i32 hats = InModelListDataFlags(FreePlayModelList, 0, 0, 1, 0);
        if (CharCategory != NULL) {
            for (i32 i = 0; i < CHARCATEGORYCOUNT; ++i) {
                if (InModelListDataFlags(FreePlayModelList, CharCategory[i].model_flags, CharCategory[i].game_flags, 0,
                                         1) != 0)
                    continue;
                i32 model = -1;
                if (hats == 0 || (CharCategory[i].model_flags & 0x80) != 0) {
                    model = RandomIDFromFlags(CharCategory[i].model_flags, CharCategory[i].game_flags, 1, NULL, 1);
                    if (model != -1) {
                        hats = 1;
                    }
                }
                if (model == -1) {
                    model = RandomIDFromFlags(CharCategory[i].model_flags, CharCategory[i].game_flags, 0, NULL, 1);
                }
                if (World_AddFreePlayModel(model) != 0)
                    ++FreePlayBonusCount;
            }
            bool imperial = false;
            for (APICHARACTERMODELLIST_s *model = FreePlayModelList; model->model_id != -1; ++model) {
                if (!(Collection_Got(model->model_id) != 0 && World_FreePlayImperial(model->model_id))) {
                    continue;
                }
                imperial = true;
                break;
            }
            if (!imperial) {
                i32 count = 0;
                for (i32 i = 0; i < CHARCOUNT; ++i) {
                    if (InModelList(FreePlayModelList, i, NULL) == 0 &&
                        (Game_Customiser == NULL ||
                         (i != Game_Customiser->character_ids[0] && i != Game_Customiser->character_ids[1])) &&
                        CDataList[i].move_fn != Move_DEFAULT && Collection_Got(i) != 0 && World_FreePlayImperial(i) &&
                        count < 500)
                        imperial_ids[count++] = static_cast<i16>(i);
                }
                if (count != 0) {
                    if (count != 1)
                        imperial_ids[0] = imperial_ids[qrand() / (0xffff / count + 1)];
                    if (World_AddFreePlayModel(imperial_ids[0]) != 0)
                        ++FreePlayBonusCount;
                }
            }
        }
    }
    for (EXTRAMODEL *extra = ExtraModelList; extra->model_list != NULL; ++extra) {
        const i32 source = *extra->model_list;
        bool found = false;
        if (source != -1) {
            for (i32 i = 0; i < FreePlayModelCount; ++i) {
                if (FreePlayModelList[i].model_id == source) {
                    found = true;
                    break;
                }
            }
        }
        if (found && extra->field_04 != NULL)
            World_AddFreePlayModel(*static_cast<i16 *>(extra->field_04));
    }
}
