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

// World info lifetime (retail worldinfo.cpp): registering, initialising,
// activating, streaming and resetting the two WORLDINFO slots.

void CutScenes_Destroy(CUTSYS *system);
void CharScenes_LevelDump(WORLDINFO *world);
void Customiser_DumpAll(CUSTOMISER *, WORLDINFO *);
void DestroyRippleMtls(WORLDINFO *);
extern "C" {
    extern i32 part_page_on[8];
}

/// @brief Pointer to the currently loading world info
static WORLDINFO *LWORLD = &WorldInfo[0];

void WorldInfo_InitOnce(void) {
    memset(WorldInfo, 0, sizeof(WorldInfo));
}

WORLDINFO *WorldInfo_CurrentlyActive(void) {
    return WORLD;
}

WORLDINFO *WorldInfo_CurrentlyLoading(void) {
    return LWORLD;
}

void WorldInfo_ReArrangeBuffers(i32 area1, i32 area2) {
    VARIPTR *bufferEnd = (VARIPTR *)&WorldInfo[1].unknown_0108;

    if (area1 == area2 || area1 == -1) {
        if (area1 != -1 && (ADataList[area1].flags & AREAFLAG_SINGLE_BUFFER) != 0) {
            return;
        }
    } else if ((ADataList[area1].flags & AREAFLAG_SINGLE_BUFFER) != 0) {
        LWORLD = &WorldInfo[0];
        WORLD = &WorldInfo[0];
        if (WorldInfo[0].unknown_0108.addr >= bufferEnd->addr) {
            return;
        }
        usize end = bufferEnd->addr;
        bufferEnd->addr = WorldInfo[0].unknown_0108.addr;
        WorldInfo[0].unknown_0108.addr = end - EDITBUFFERENDSIZE;
        return;
    }

    if (WorldInfo[0].unknown_0108.addr <= bufferEnd->addr) {
        return;
    }
    usize end = WorldInfo[0].unknown_0108.addr;
    WorldInfo[0].unknown_0108.addr = bufferEnd->addr;
    bufferEnd->addr = end + EDITBUFFERENDSIZE;
}

i32 WorldInfo_OtherLevel(WORLDINFO *world) {
    WORLDINFO *other = &WorldInfo[0];

    if (world == &WorldInfo[0]) {
        other = &WorldInfo[1];
    }

    if (other->loaded != 0) {
        return other->level_idx;
    }

    return -1;
}

void WorldInfo_Register(WORLDINFO *world) {
    edbitsRegisterBaseScene(world->current_gscn);
    edanimRegisterBaseScene(world->current_gscn);
    edbitsRegisterBaseTerrain(world->terrain);
}

void WorldInfo_UpdateRoomVisibility(WORLDINFO *world, i32 param) {
    world->room_visibility_flag = 1;
    world->rooms_visible_ptr = world->rooms_visible;
    memset(world->rooms_visible, 0, 0x100);

    if (param == 0 && world->current_gscn != NULL) {
        const i32 room_count = world->current_gscn->num_rooms;
        if (room_count > 0) {
            u8 *visible = world->rooms_visible_ptr;
            NUROOM *rooms = world->current_gscn->rooms;
            for (i32 i = 0; i < room_count; ++i) {
                visible[i] = (rooms[i].flags & 4) != 0;
            }
        }
    }
}

void WorldInfo_LoadObjectAnimFile(WORLDINFO *world) {
    if (world->page_anim == -1) {
        char path[256];
        sprintf(path, "%s.anm", world->config_file);
        if (NuFileExists(path)) {
            world->page_anim = edanimLoadPage(path, world->current_gscn);
        }
    }
}

void WorldInfo_Init(WORLDINFO *world) {
    i32 local_menu_id = -1;
    i32 local_menu_y = -1;

    disable_narrow_socks = 0;
    script_spline_selected = 0;

    world->reset_flags = 0;
    Reflections_On = NuDeviceSpecs::ms_instance->specs > 1;
    g_lowEndLevelBehaviour = g_isLowEndDevice;
    if (!g_isLowEndDevice) {
        if (world->current_level != NULL && NuStrIStr(world->current_level->name, "JabbasPalace_Intro1") != NULL) {
            DebrisSetThinningLevel(6.0f);
            DebrisSetForcedThinning(1);
        } else if (NuDeviceSpecs::ms_instance->specs <= 2 &&
                   ((world->current_level != NULL && NuStrIStr(world->current_level->name, "Dogfight_A") != NULL) ||
                    (world->current_level != NULL &&
                     NuStrIStr(world->current_level->name, "DeathStarRescue") != NULL) ||
                    (world->current_level != NULL && NuStrIStr(world->current_level->name, "GunGan_A") != NULL) ||
                    (world->current_level != NULL && NuStrIStr(world->current_level->name, "SpeederChase") != NULL))) {
            DebrisSetDetailLevel(1);
            g_lowEndLevelBehaviour = 1;
            DebrisSetThinningLevel(6.0f);
            DebrisSetForcedThinning(1);
            character_farclip = world->current_level->unknown_11c;
            SetCameraZoom(world->current_level->unknown_120);
        } else {
            DebrisSetDetailLevel(4);
            DebrisSetThinningLevel(1.0f);
            DebrisSetForcedThinning(0);
        }
        if (NuDeviceSpecs::ms_instance->specs <= 2 && world->current_level != NULL &&
            NuStrIStr(world->current_level->name, "Negotiations_") != NULL) {
            Reflections_On = 0;
        }
    } else {
        if (world->current_level != NULL) {
            DebrisSetThinningLevel(world->current_level->data_display.particle_thin);
            DebrisSetForcedThinning(world->current_level->data_display.particle_thin > 1.0f);
            character_farclip = world->current_level->unknown_11c;
            SetCameraZoom(world->current_level->unknown_120);
        }
    }

    // Sound and SFX setup
    ResetSounds();
    SetLevelSfxBits(world);

    // Music setup
    LevMusicAction = ActionFromQuiet((i32)world->current_level->music_index);
    LevMusicAmbient = AmbientFromQuiet((i32)world->current_level->music_index);
    LevMusicOtherAction = ActionFromQuiet((i32)(i16)world->current_level->unknown_0a8);
    LevMusicOtherAmbient = AmbientFromQuiet((i32)(i16)world->current_level->unknown_0a8);

    ResetBits |= 1;

    if (NOSOUND == 0) {
        WorldInfo_Register(world);
    }

    // Start page loading for various subsystems
    if (world->page_pp != -1) {
        edppStartPage(static_cast<i8>(world->page_pp));
    }
    if (world->page_part != -1) {
        edpartStartPage(static_cast<i8>(world->page_part));
    }
    if (world->page_bridge != -1) {
        edbriStartPage(static_cast<i8>(world->page_bridge));
    }

    // Terrain initialization
    noterraininit();
    void *terrain_cur = world->terrain;
    TerrainSetCur(terrain_cur);
    TerrSetPlatScanDist((f32)(u8)world->current_level->unknown_0db);

    if (world->page_grass != -1) {
        edgraStartPage(static_cast<i8>(world->page_grass));
    }

    TerrainPlatformOldUpdate();
    TerrainPlatformNewUpdate();
    TerrainSetWallDeflectYScale(1.0f);
    LevObj_FixUpPlatIDs(world);

    if (NOSOUND == 0) {
        NuRndrInitWorld();
    }

    TerrainPlatformOldUpdate();
    if (world->current_gscn != NULL) {
        NuGScnUpdate(world->current_gscn, 0.0f);
    }
    TerrainPlatformNewUpdate();

    // Level progress copy
    LEVEL_PROGRESS_s *progress = (LEVEL_PROGRESS_s *)world->level_progress;
    if (NOSOUND == 0 && progress != NULL && (progress->flags & 1) == 0) {
        progress->flags |= 1;
        progress->data = world->progress_data;
    }

    Doors_Init(world);
    Players_InitPositions(world);
    Obj = reinterpret_cast<GameObject_s *>(world->api_object_sys->objects);
    ClearGameObjects(world->api_object_sys);
    PlayerItemTypes_Reset(world);
    Players_Init();
    rtlResetDynamic();
    SetPartRTLSet(reinterpret_cast<usize>(world->rtl_set));

    WorldInfo_UpdateRoomVisibility(world, 1);

    // Level init function
    if (world->current_level->init_fn != NULL) {
        world->current_level->init_fn(world);
    }

    if (NOSOUND == 0 && WorldInfo_InitMenuFn != NULL) {
        WorldInfo_InitMenuFn(world, &local_menu_id, &local_menu_y);
    }

    // A level transition may override the menu selected by the game hook.
    if (newlevelfrommenu_newmenuid != -1) {
        local_menu_id = newlevelfrommenu_newmenuid;
        local_menu_y = newlevelfrommenu_newmenuy;
        newlevelfrommenu_newmenuid = -1;
        newlevelfrommenu_newmenuy = -1;
    }

    // FreePlay check
    if (world->area != NULL) {
        if ((world->area->flags & AREAFLAG_BONUS_AREA) != 0) {
            NextArea_FreePlay = 1;
            FreePlay = 1;
        } else {
            if (world->area == HUB_ADATA) {
                NextArea_FreePlay = 0;
                FreePlay = 0;
            }
        }
    }

    MenuReset();
    NewMenu(local_menu_id, local_menu_y, -1);

    if (NOSOUND == 0) {
        ResetTimer(&LevelTimer, 0.0f);
    }

    if ((world->current_level->flags & (LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0) {
        WorldInfo_LoadObjectAnimFile(world);
    }

    reset_restart = 1;
    world->field_0x114 = 1;

    InitGameObjectLights();

    world->field_5174 = 1;
    TerSurface[14].flags = 0x2000;
    TerSurface[14].movement_scale = 1.0f;
    TerSurface[15].flags = 0x2000;
    TerSurface[24].flags = 0x2000;
    TerSurface[15].movement_scale = 1.0f;
    TerSurface[24].movement_scale = 1.0f;
    TerSurface[20].flags = 0x2000;
    TerSurface[9].flags = 0x2000;
    TerSurface[20].movement_scale = 1.0f;
    TerSurface[9].movement_scale = 1.0f;

    // Init last function
    if (WorldInfo_InitLastFn != NULL) {
        WorldInfo_InitLastFn(world);
    }

    if (NOSOUND == 0) {
        MechSystems::Get()->EnterLevel(world);
    }

    if (world->mech_auto_jump_manager != NULL) {
        world->mech_auto_jump_manager->Init();
    }
}

void WorldInfo_Activate(void) {
    char used_fog_color;

    WORLD = LWORLD;
    WorldInfo_Init(LWORLD);

    used_fog_color = 0;
    if (NuIOS_IsLowEndDevice() && WORLD != NULL) {
        LEVELDATA *current_level = WORLD->current_level;

        if (current_level && current_level->data_display.far_clip < 20000.0f) {
            used_fog_color = current_level->data_display.fog_start < 20000.0f;
        }
    }

    g_BackgroundUsedFogColour = used_fog_color;
}

void WorldInfo_DrawScene(WORLDINFO *world) {
    if (world->current_gscn == NULL) {
        return;
    }
    if (CUTCAM == 0 && world->current_gscn->display_list == NULL) {
        NuPortalInit();
        NuPortalVisibility(world->current_gscn);
        WorldInfo_UpdateRoomVisibility(world, 0);
    }
    NuGScnRndr3(world->current_gscn);
    if (world->current_gscn->display_list != NULL) {
        WorldInfo_UpdateRoomVisibility(world, 0);
    }
}

void WorldInfo_Dump(WORLDINFO *world) {
    if (world->mech_auto_jump_manager != NULL) {
        delete world->mech_auto_jump_manager;
        world->mech_auto_jump_manager = NULL;
    }
    MechSystems::Get()->ExitLevel(world);
    Customiser_DumpAll(Game_Customiser, world);
    if (world->cutscene_sys != NULL) {
        for (i32 index = 0; index < world->cutscene_sys->count; ++index)
            ClearLinkedCutSceneMusic(world->cutscene_sys->cuts[index]->instance);
    }
    CharacterMiniKits_Dump(world);
    CharScenes_LevelDump(world);
    CutScenes_Destroy(world->cutscene_sys);
    if (world->icons_gscn != nullptr) {
        NuGScnRemove(world->icons_gscn);
        world->icons_gscn = nullptr;
    }
    if (world->page_pp != -1) {
        edppStopPage(static_cast<i8>(world->page_pp));
        edppClearPage(static_cast<i8>(world->page_pp));
    }
    if (world->page_part != -1) {
        edpartStopPage(static_cast<i8>(world->page_part));
        edpartClearPage(static_cast<i8>(world->page_part));
    }
    if (world->page_grass != -1) {
        edgraStopPage(static_cast<i8>(world->page_grass));
        edgraClearPage(static_cast<i8>(world->page_grass));
    }
    if (world->page_bridge != -1)
        edbriClearPage(static_cast<i8>(world->page_bridge));
    if (world->page_anim != -1) {
        edanimStopPage(world->page_anim);
        edanimClearPage(world->page_anim);
    }
    if (BGLOAD == 0) {
        for (i32 page = 2; page < 8; ++page) {
            if (edpp_page_on[page] != 0) {
                edppStopPage(page);
                edppClearPage(page);
            }
        }
        for (i32 page = 1; page < 8; ++page) {
            if (part_page_on[page] != 0) {
                edpartStopPage(page);
                edpartClearPage(page);
            }
        }
    }
    DestroyRippleMtls(world);
    if (world->current_gscn != nullptr) {
        NuGScnRemove(world->current_gscn);
        world->current_gscn = nullptr;
    }
    if (world->scene != nullptr) {
        NuGScnRemove(world->scene);
        world->scene = nullptr;
    }
}

i32 WorldInfo_Reset(WORLDINFO *world, i32 level_idx) {
    if (world->loaded != 0) {
        SetLevelExBlowupFlags(0);
        if (world->level_idx == level_idx && (u8)LDataList[level_idx].area_index != 0xff && new_level_from_menu == 0) {
            return 0;
        }
        WorldInfo_Dump(world);
    } else {
        SetLevelExBlowupFlags(0);
    }

    if (new_level_from_menu != 0) {
        WORLDINFO *other = WorldInfo;
        if (world == WorldInfo) {
            other = world + 1;
        }
        if (other->loaded != 0 && other->level_idx == level_idx) {
            WorldInfo_Dump(other);
        }
    }

    // Save buffer cursors before clearing the world.
    void *bufStart = world->buffer_start;
    void *bufEnd = world->unknown_0108.void_ptr;

    TouchHacks::CleanupAllMechObjectInterfaces(world);

    memset(world, 0, sizeof(*world));

    // Restore the buffer cursors: 0x108, then 0x100, then 0x104 (giz_buffer).
    world->unknown_0108.void_ptr = bufEnd;
    world->buffer_start = bufStart;
    world->giz_buffer.void_ptr = bufStart;
    isize buffer_size = static_cast<char *>(bufEnd) - static_cast<char *>(bufStart);
    if (buffer_size > 0) {
        memset(bufStart, 0, buffer_size);
    }

    // Set level info
    world->level_idx = level_idx;
    world->level_sub_id = -1;
    world->area_sub_id = -1;

    if (level_idx != -1) {
        LEVELDATA *levelData = &LDataList[level_idx];
        world->current_level = levelData;
        i32 areaIdx = (i8)levelData->area_index;
        world->level_sub_id = areaIdx;
        if (areaIdx != -1) {
            world->area = &ADataList[areaIdx];
            world->area_sub_id = (i8)ADataList[areaIdx].episode_index;
        }

        // Invalidate every page handle (0x2a98 .. 0x2aac).
        world->page_pp = -1;
        world->page_anim = -1;
        world->page_grass = -1;
        world->page_bridge = -1;
        world->page_part = -1;

        i32 progress_index = (i8)levelData->area_level_index;
        if (progress_index <= 0xb && progress_index != -1) {
            world->level_progress = static_cast<LEVEL_PROGRESS_s *>(LevelProgressData) + progress_index;
        } else {
            world->level_progress = NULL;
        }

        // Build config file path
        NuStrCpy(world->name, "levels\\");
        NuStrCat(world->name, world->current_level->dir);
        NuStrCpy(world->config_file, world->name);
        NuStrCat(world->config_file, "\\");
        NuStrCat(world->config_file, world->current_level->name);
        ResetLevSfx(world);
    }

    return 1;
}

void WorldInfo_StreamLevel(BGPROCINFO *bg_info) {
    WORLDINFO *world;
    NUTIME time;

    world = WorldInfo_CurrentlyActive();

    if (world == LWORLD && (Area == -1 || (ADataList[Area].flags & AREAFLAG_SINGLE_BUFFER) == 0)) {
        LWORLD = world == &WorldInfo[0] ? &WorldInfo[1] : &WorldInfo[0];
    }

    if (WorldInfo_Reset(LWORLD, next_level) != 0) {
        waiting_for_level = next_level;

        WorldInfo_Load(LWORLD);
    }

    waiting_for_level = -1;

    if (LWORLD->loaded != 0) {
        level_already_loaded = next_level;
    }

    NuTimeGet(&time);
    g_BgLoadDelayHackTimer = NuTimeSeconds(&time) + 5.0f;
}

void WorldInfo_ClearAllIfScreenFaded(void) {
    if (LastLData == TITLES_LDATA || new_level_from_menu != 0 || Area != last_area || BGLOAD == 0) {
        WorldInfo_Reset(&WorldInfo[0], -1);
        if (last_area == -1 || (ADataList[last_area].flags & AREAFLAG_SINGLE_BUFFER) == 0) {
            WorldInfo_Reset(&WorldInfo[1], -1);
        }
        level_already_loaded = -1;
    }
}
