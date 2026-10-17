#include "decomp.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/game_deb.h"

#include <stdio.h>
#include <string.h>

#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"
#include "MechInputTouch/MechInputTouch_types.h"

// Globals shared across world loading — defined here until moved to globals.cpp
TIMER LevelTimer;

void (*WorldInfo_InitMenuFn)(WORLDINFO *, i32 *, i32 *) = NULL;
void (*WorldInfo_InitLastFn)(WORLDINFO *) = NULL;
void *things_scene_terrain = NULL;

// --- Players globals ---
// NOTE: `WORLD` (WORLDINFO) field offsets in world.h are an approximation; the
// original binary reaches the level/area pointers through WORLDINFO[0x128]
// while world.h places them at 0x130/0x134.  Players_Init still links and runs
// with the world.h layout but its level/area guard bytes will differ until that
// WORLDINFO drift is corrected.

i32 PLAYERCOUNT = 0;
i32 netclient = 0;
i32 UsePlayerList = 0;
i32 makeplayerlist_freeplay = 0;
i32 PlayerID[2] = {-1, -1};
i16 Area_PlayerIDList[9];
APICHARACTERMODELLIST_s Area_StoryModelList[52];
PLAYERPROGRESS PlayerProgress[8];
i32 Hub_UsePlayerList = 0;
i32 LevelChangesInArea = 0;
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

WORLDINFO WorldInfo[2];

f32 g_BgLoadDelayHackTimer;

volatile i32 waiting_for_level;
i32 level_already_loaded = -1;
i32 next_level;

