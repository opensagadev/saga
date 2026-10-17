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
extern i32 GAMEDEMO;
extern NUGSCN * big_icon_scene;
extern NUGSCN * area_scene;
extern NUGSCN * things_scene;
void *things_scene_terrain = NULL;
extern LEVELDATA * PLATFORM_LDATA;
extern LEVELDATA * RETAKED_LDATA;
extern LEVELDATA * CREDITS_LDATA;
extern i32 Text_Language;

// --- Players globals ---
// NOTE: `WORLD` (WORLDINFO) field offsets in world.h are an approximation; the
// original binary reaches the level/area pointers through WORLDINFO[0x128]
// while world.h places them at 0x130/0x134.  Players_Init still links and runs
// with the world.h layout but its level/area guard bytes will differ until that
// WORLDINFO drift is corrected.

extern GameObject_s * Player[8];
i32 PLAYERCOUNT = 0;
i32 netclient = 0;
i32 UsePlayerList = 0;
i32 makeplayerlist_freeplay = 0;
extern i16 PlayerList[8];
i32 PlayerID[2] = {-1, -1};
i16 Area_PlayerIDList[9];
APICHARACTERMODELLIST_s Area_StoryModelList[52];
PLAYERPROGRESS PlayerProgress[8];
i32 Hub_UsePlayerList = 0;
i32 LevelChangesInArea = 0;
extern i32 bonusmodearcade;
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

void SetAreaPickupGravity(i32 area, i32 level);

void StoreLevelProgressFn(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress, i32 area_progress);

void SaveSceneObjectAnimTFactors(NUGSCN *gscn);
void CalculateWorldSize(WORLDINFO *world);

WORLDINFO WorldInfo[2];
extern WORLDINFO * WORLD;

f32 g_BgLoadDelayHackTimer;

volatile i32 waiting_for_level;
i32 level_already_loaded = -1;
i32 next_level;

void WorldInfo_Load(WORLDINFO *world);

extern "C" {
    extern i16 id_DARTHVADER;
    extern i16 id_THEEMPEROR;
}

