#include "legoapi/items/objects/gameobjects.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>

// NuCore profiling timebars (nucore_plain.cpp): NuTimeBarCreateSet returns a
// deferred-subsystem stub handle; the slot functions are no-op stubs.
extern "C" {
    void *NuTimeBarCreateSet(i32);
    void _NuTimeBarSlotBegin(void *, i32, char const *);
    u32 _NuTimeBarSlotEnd(void *, i32);
    void AddToAIGroup(AIGROUP_s *group, APIOBJECT_s *object);
    extern NUVEC plr_lastpos;
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;
extern void ReleaseTakeOver(GameObject_s *object, i32 immediate);
extern void oneAtOnce_MaintainArray();

void legoSetMusicVolume(float);
void MovePlayer(GameObject_s *object);
void AnimatePlayer(GameObject_s *object);
void TerrainPlayer(GameObject_s *object);
void KeepOnScreen(GameObject_s *object);
void SetPlayer();
void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);
void MovePlayerSpline(GameObject_s *object);
void GetTopBot(GameObject_s *object);
void ResetRumble(RUMBLEPACKET *packet);
void ResetLights(NUVEC *position, rtldata_s *data, void *set);
void LightGameObject(GameObject_s *object, void *set);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void PortalGameObject(GameObject_s *object, i32 enable, i32 immediate, i16 portal, nugscn_s *scene);
i32 Arcade_GetMode(u32 *mode);
void StarWars_GameAISysInit();
void GameAISysSetGame();
void ClearAICreatures();
void CollideGameObjects(WORLDINFO_s *world);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
i32 TagCode(GameObject_s *source, GameObject_s *target, i32 takeover, i32 blend, i32 mode);
extern i32 do_player_tag;
extern f32 player_tag_timer;
extern GameObject_s *player_tag_from;
extern GameObject_s *player_tag_to;
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
void AIPathCnxHelperSysReset(WORLDINFO_s *world, AIPATHCNXHELPERSYS_s *system);
void InitAICreatures(AISYS_s *system);
void ResetAICreatures(AISYS_s *system);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);
void ResetCoinPacket(COINPACKET_s *packet);
GIZMOPICKUP_s *GizmoPickups_Collide(WORLDINFO_s *world, GameObject_s *object, i32 arg);

f32 Condition_InHubArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);
void *Condition_InHubAreaInit(AISYS_s *, char *, AISCRIPT_s *);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern "C" void NuLightFogX(f32 start, f32 end, u32 colour, f32 unused_start, f32 unused_end, i32 high_quality,
                            f32 density);

GAMEFOG_STATE GameFog = {};

static i32 GameFogSnap;
static i32 GameFogSet;
static f32 GameFogDuration;
static f32 GameFogTime;

enum AI_ACTION_SPEED_MODE : u8 {
    AI_ACTION_SPEED_RUN = 0,
    AI_ACTION_SPEED_WALK = 1,
    AI_ACTION_SPEED_TIPTOE = 2,
};

enum SCRIPT_ERROR_LEVEL : u32 {
    SCRIPT_ERROR_LEVEL_NONE = 0,
    SCRIPT_ERROR_LEVEL_WARNING = 1,
    SCRIPT_ERROR_LEVEL_STRICT = 2,
    SCRIPT_ERROR_LEVEL_COUNT = 3,
};

enum AI_DEFAULTS : u8 {
    AI_DEFAULT_ACTIVATE_DIFFICULTY = 1,
};

enum AI_OBJECT_ROUTE_STATE : u8 {
    AI_OBJECT_ROUTE_STATE_SCRIPT_VISIBLE = 3,
};

enum CHARACTER_AI_MODEL_FLAGS : u32 {
    CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP = 0x00200000,
    CHARACTER_AI_MODEL_FLAG_DISABLE_RESPAWN = 0x00400000,
};

enum AI_GAME_OBJECT_TYPE : u8 {
    AI_GAME_OBJECT_TYPE_VEHICLE = 0x2b,
};

enum GAME_OBJECT_AI_UPDATE_FLAGS : u8 {
    GAME_OBJECT_AI_UPDATE_PROCESS = 0x08,
    GAME_OBJECT_AI_UPDATE_FORCED = 0x10,
    GAME_OBJECT_AI_UPDATE_SPECIAL_STATE = 0x20,
};

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
i32 timebase_updates = 1;

static i32 GameObjectAIUpdateInterval(WORLDINFO_s *world, GameObject_s *object) {
    if (object->apiobj.field_0x27d == 0 || object->apiobj.character_data == NULL ||
        object->apiobj.character_data->field11_0x24 == NULL) {
        return 1;
    }

    // The target updates characters outside the visible portal set every ten
    // frames.  This avoids doing a full script, controller and terrain pass
    // for every off-screen Cantina inhabitant on the same frame.
    const i16 room = object->room_id;
    if (world == NULL || world->rooms_visible_ptr == NULL || room < 0 || world->rooms_visible_ptr[room] == 0 ||
        object->apiobj.model_draw_result == 0) {
        return 10;
    }

    const GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    const f32 distance = object->ai_update_distance;
    i32 interval = character->ai_update_interval_0;
    if (interval == 0 || distance <= character->ai_update_distance_0) {
        interval = character->ai_update_interval_1;
        if (interval == 0 || distance <= character->ai_update_distance_1) {
            interval = character->ai_update_interval_2;
            if (interval == 0 || distance <= character->ai_update_distance_2) {
                interval = character->ai_update_interval_3;
                if (interval == 0 || distance <= character->ai_update_distance_3) {
                    return 1;
                }
            }
        }
    }

    // Cadence entries are distance-tier sentinels: zero advances to the next
    // tier rather than selecting a zero-frame interval.  Values at or above
    // seven use the target's capped seven-frame cadence.
    return interval < 7 ? interval : 7;
}

static const f32 AI_RESPAWN_DELAY = 2.0f;

extern "C" {
    AICONDITIONDEF lego_aiconditiondefs[] = {
        {"GlynTest", NULL, NULL},
        {"Debug", NULL, NULL},
        {"Active", NULL, NULL},
        {"GotGun", NULL, NULL},
        {"PrefersBrawling", NULL, NULL},
        {"IsAlive", NULL, NULL},
        {"IsOnScreen", NULL, NULL},
        {"OffScreenTimer", NULL, NULL},
        {"OnObject", NULL, NULL},
        {"OnSameObjectAsPlayer", NULL, NULL},
        {"PlayerOnObject", NULL, NULL},
        {"EitherPlayerOnObject", NULL, NULL},
        {"EitherPlayerLocatorRangeXZ", NULL, NULL},
        {"OnGround", NULL, NULL},
        {"BeenAlerted", NULL, NULL},
        {"PlayerOnGround", NULL, NULL},
        {"SpawnCount", NULL, NULL},
        {"BehindCamera", NULL, NULL},
        {"LocatorOnScreen", NULL, NULL},
        {"Blocking", NULL, NULL},
        {"BeenHit", NULL, NULL},
        {"HoverPhase", NULL, NULL},
        {"HitPoints", NULL, NULL},
        {"OnDynamicGrapple", NULL, NULL},
        {"XPos", NULL, NULL},
        {"YPos", NULL, NULL},
        {"ZPos", NULL, NULL},
        {"CollidingWithOpponent", NULL, NULL},
        {"Colliding", NULL, NULL},
        {"ObstacleAtStart", NULL, NULL},
        {"ObstacleAtEnd", NULL, NULL},
        {"SpecialAtStart", NULL, NULL},
        {"SpecialAtEnd", NULL, NULL},
        {"ObstacleLockedOpen", NULL, NULL},
        {"ObstacleLockedShut", NULL, NULL},
        {"ForceAtStart", NULL, NULL},
        {"ForceAtEnd", NULL, NULL},
        {"ObstacleOpenedByPlayer", NULL, NULL},
        {"ObstacleOpenedByEitherPlayer", NULL, NULL},
        {"AnimationFinished", NULL, NULL},
        {"EitherPlayerPullingLever", NULL, NULL},
        {"EitherPlayerUsingHatMachine", NULL, NULL},
        {"EitherPlayerUsingPanel", NULL, NULL},
        {"EitherPlayerWearingHelmet", NULL, NULL},
        {"PartyUnderCover", NULL, NULL},
        {"NumBaddiesThatCanSeePlayers", NULL, NULL},
        {"PlayerUsingForce", NULL, NULL},
        {"EitherPlayerUsingForce", NULL, NULL},
        {"UsingForce", NULL, NULL},
        {"OnForcePlatform", NULL, NULL},
        {"PlayerOnForcePlatform", NULL, NULL},
        {"EitherPlayerOnForcePlatform", NULL, NULL},
        {"ForceBeingUsed", NULL, NULL},
        {"ForcePushing", NULL, NULL},
        {"TurretAlive", NULL, NULL},
        {"PlayerDeflectingPart", NULL, NULL},
        {"ForceComplete", NULL, NULL},
        {"ForceFinished", NULL, NULL},
        {"ForceStackComplete", NULL, NULL},
        {"ForceStackCompleteInOrder", NULL, NULL},
        {"BuildItComplete", NULL, NULL},
        {"BlowupBlownup", NULL, NULL},
        {"IAmA", NULL, NULL},
        {"OpponentIsA", NULL, NULL},
        {"OpponentIsAThreat", NULL, NULL},
        {"CanFightLikeAJedi", NULL, NULL},
        {"IAmAGoody", NULL, NULL},
        {"IAmABaddy", NULL, NULL},
        {"IAmANeutral", NULL, NULL},
        {"IAmAGoodyBaddy", NULL, NULL},
        {"IAmAPartyCharacter", NULL, NULL},
        {"CategoryIs", NULL, NULL},
        {"PlayerCategoryIs", NULL, NULL},
        {"EitherPlayerIs", NULL, NULL},
        {"Player1Is", NULL, NULL},
        {"Player2Is", NULL, NULL},
        {"IsSetAlive", NULL, NULL},
        {"NumInSetAlive", NULL, NULL},
        {"Context", NULL, NULL},
        {"InContext", NULL, NULL},
        {"OpponentContext", NULL, NULL},
        {"Player2Active", NULL, NULL},
        {"NumBaddies", NULL, NULL},
        {"NumForceObjects", NULL, NULL},
        {"BeenToLevel", NULL, NULL},
        {"LastLevel", NULL, NULL},
        {"Message", NULL, NULL},
        {"ScriptParam", NULL, NULL},
        {"CutSceneStarted", NULL, NULL},
        {"CutSceneFinished", NULL, NULL},
        {"CutSceneExists", NULL, NULL},
        {"PlayerInSock", NULL, NULL},
        {"CutScenePlaying", NULL, NULL},
        {"RigidAnimFrame", NULL, NULL},
        {"SockDistanceToPlayer", NULL, NULL},
        {"SockDistanceToOpponent", NULL, NULL},
        {"SockXDistanceToPlayer", NULL, NULL},
        {"PlayerDistanceAlongSock", NULL, NULL},
        {"FurthestPlayerDistanceAlongSock", NULL, NULL},
        {"FinishedSpline", NULL, NULL},
        {"CurrentHintId", NULL, NULL},
        {"HintAvailable", NULL, NULL},
        {"HintComplete", NULL, NULL},
        {"Freeplay", NULL, NULL},
        {"Indy", NULL, NULL},
        {"MissionMode", NULL, NULL},
        {"MissionWon", NULL, NULL},
        {"ChallengeMode", NULL, NULL},
        {"PSP", NULL, NULL},
        {"AIOverrideControl", NULL, NULL},
        {"BoltsDontGetDeflectedBack", NULL, NULL},
        {"CheatProgress", NULL, NULL},
        {"BigJumpComplete", NULL, NULL},
        {"RespawnLocatorIs", NULL, NULL},
        {"InMiniCut", NULL, NULL},
        {"MaulShouldRunAway", NULL, NULL},
        {"DropBackInTimer", NULL, NULL},
        {"HelpWithTriggers", NULL, NULL},
        {"EitherPlayerPushingSpinner", NULL, NULL},
        {"CharacterRange", NULL, NULL},
        {"BeenSpawned", NULL, NULL},
        {"LastAttackerRange", NULL, NULL},
        {"LastAttackerIsActivePlayer", NULL, NULL},
        {"PartyContainsDroids", NULL, NULL},
        {"CannotReachDestination", NULL, NULL},
        {"TakenOver", NULL, NULL},
        {"PlayerTakenOver", NULL, NULL},
        {"EitherPlayerTakenOver", NULL, NULL},
        {"BeenTakenOver", NULL, NULL},
        {"OnSpeederBike", NULL, NULL},
        {"UnderPlayerControl", NULL, NULL},
        {"CharacterExists", NULL, NULL},
        {"CharacterTypeExists", NULL, NULL},
        {"GotLocatorInSet", NULL, NULL},
        {"GotOpponentLOS", NULL, NULL},
        {"EmptyTakeOver", NULL, NULL},
        {"HasTakeOverTarget", NULL, NULL},
        {"TakeOverRange", NULL, NULL},
        {"TakeOverTargetInTriggerArea", NULL, NULL},
        {"EitherPlayerInMyTriggerArea", NULL, NULL},
        {"AreaContainsBaddies", NULL, NULL},
        {"AreaContainsGoodies", NULL, NULL},
        {"AreaContainsPartyMember", NULL, NULL},
        {"GotVictim", NULL, NULL},
        {"IsVisible", NULL, NULL},
        {"MySet", NULL, NULL},
        {"ScreenWipe", NULL, NULL},
        {"IAmPlayer2", NULL, NULL},
        {"HeadTurnRestricted", NULL, NULL},
        {"ShopActive", NULL, NULL},
        {"Side", NULL, NULL},
        {"NearestPartyRange", NULL, NULL},
        {"NearestPartyXZRange", NULL, NULL},
        {"OpponentToPlayerRange", NULL, NULL},
        {"OpponentPathPosRange", NULL, NULL},
        {"GizmoOutput0", NULL, NULL},
        {"GizmoOutput1", NULL, NULL},
        {"GizmoOutput2", NULL, NULL},
        {"GizmoOutput3", NULL, NULL},
        {"GizmoVisibility", NULL, NULL},
        {"AngleAboutMyLocatorToPlayer", NULL, NULL},
        {"AnimSpeedMul", NULL, NULL},
        {"PickupBeenTurnedOn", NULL, NULL},
        {"FlowBoxComplete", NULL, NULL},
        {"CanHearRadio", NULL, NULL},
        {"BeingTowed", NULL, NULL},
        {"RaceLap", NULL, NULL},
        {"MusicOn", NULL, NULL},
        {"CharacterLoaded", NULL, NULL},
        {"AreaComplete", NULL, NULL},
        {"ShouldAttackOpponent", NULL, NULL},
        {"InSwamp", NULL, NULL},
        {"InSameTriggerAreaAsNearestPlayer", NULL, NULL},
        {"NetworkGameOnGoing", NULL, NULL},
        {"InHubArea", &Condition_InHubArea, &Condition_InHubAreaInit},
        {"IsLowEndDevice", NULL, NULL},
        {"RandomMapCharsAvailable", NULL, NULL},
        {NULL, NULL, NULL},
    };

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

    f32 default_path_heighttol = 0.2f;
    u8 default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    u8 default_min_n_respawns;
    u8 default_max_n_respawns;
    f32 default_min_t_respawn;
    f32 default_max_t_respawn;

    CHARACTERNAMEFN *LevelCharacterNameFn;
    CHARACTERNAMEFN *SpecialRouteCharacterNameFn;
    CHARACTERGLOBALIDFN *LevelCharacterGlobalIDFn;
    GLOBALCHARACTERNAMEFN *GlobalCharacterNameFn;
    CHARACTERHGOBJFN *GlobalCharacterHGobjFn;
    CHARACTERRENDERFN *GlobalCharacterRenderFn;
    CHARACTERGOALSPEEDFN *GetCharacterGoalSpeedFn;
    CHARACTERTYPEIDFN *LevelCharacterTypeIDFn;

    AICHARACTERTYPEID *GlobalCharacterTypeIDFn;
    AISPECIALROUTECHARACTERTYPEID *SpecialRouteCharacterTypeIDFn;
    AICHARACTERDISTANCE *GetViewRangeFn;
    AICHARACTERDISTANCE *GetHearDistanceFn;
    AICHARACTERDISTANCE *GetMaxViewHeightFn;
    AICHARACTERDISTANCE *GetMinViewHeightFn;
    GAMEAILOAD *GameAILoadFn;
    AIACTIONPARSESPEED *AIActionParseSpeedFn;
    AIBIGJUMPTODESTINATION *AIBigJumpToDestinationFn;
    AIRESPAWNONPATH *AIRespawnOnPathFn;
    AICLEARCREATURES *ClearAICreaturesFn;
    APIOBJECTFROMOBJID *APIOBJECTFromObjIDFn;
    AIFINDALTERNATIVESPECIALOBJECT *FindAlternativeSpecialObjectFn;
    AIGETNAMEDAPIOBJECT *GetNamedAPIObjectFn;
    AIGETCREATUREORIGIN *GetAICreatureOriginFn;
}

static SCRIPT_ERROR_LEVEL ScriptErrorLevel;

// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.
static const char *skip_chars[] = {"Batman", "Robin", "Glide_Pack", NULL};

static char *LevelCharacterName(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return NULL;
    }

    const i16 character_type = CurrentStoryCList[character_index].model_id;
    if (character_type == -1) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static i32 LevelCharacterGlobalID(u8 character_index) {
    if (character_index == 0xff || CurrentStoryCList == NULL) {
        return -1;
    }
    return CurrentStoryCList[character_index].model_id;
}

static char *GlobalCharacterName(i32 character_type) {
    if (character_type == -1 || character_type >= apicharsys->character_count) {
        return NULL;
    }
    return apicharsys->char_data[character_type].file;
}

static void *GlobalCharacterHGobj(i32 character_type) {
    if (character_type == -1) {
        return NULL;
    }

    const i16 model_index = apicharsys->playermodelids[character_type];
    if (model_index == -1) {
        return NULL;
    }
    return apicharsys->models[model_index].hierarchy;
}

static f32 GetViewRange(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->viewdistance;
}

static f32 GetHearDistance(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->heardistance;
}

static f32 GetMaxViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->maxviewheight;
}

static f32 GetMinViewHeight(i32 character_type) {
    if (character_type == -1) {
        return 0.0f;
    }
    return static_cast<GAMECHARACTERDATA *>(apicharsys->char_data[character_type].field11_0x24)->minviewheight;
}

static NUVEC *GetAICreatureOrigin(AISYS *, AIPACKET *) {
    return NULL;
}

static APIOBJECT *GetNamedAPIObject(AISYS *system, char *name) {
    if (system != NULL && Obj != NULL) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *object = &Obj[index];
            if ((object->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
                continue;
            }

            char *object_name = NULL;
            if ((object->apiobj.field_0x1f4 & 0x400) != 0 && object->ai.field_0x134 != 0xff &&
                system->creatures != NULL) {
                object_name = system->creatures[object->ai.field_0x134].name;
            } else if (object->apiobj.character_data != NULL) {
                object_name = object->apiobj.character_data->file;
            }
            if (object_name != NULL && NuStrICmp(object_name, name) == 0) {
                return &object->apiobj;
            }
        }
    }

    if (NuStrICmp(name, "player") == 0) {
        return player != NULL ? &player->apiobj : NULL;
    }
    if (NuStrICmp(name, "player_2") == 0) {
        GameObject_s *second = Player[0] == player ? Player[1] : Player[0];
        return second != NULL ? &second->apiobj : NULL;
    }
    for (i32 index = 0; index < 8; ++index) {
        if (Player[index] == NULL) {
            continue;
        }
        char player_name[72];
        sprintf(player_name, "Player%d", index);
        if (NuStrICmp(player_name, name) == 0) {
            return &Player[index]->apiobj;
        }
    }
    return NULL;
}

static i32 GlobalCharacterTypeID(char *name) {
    for (i32 character_type = 0; character_type < apicharsys->character_count; ++character_type) {
        if (NuStrICmp(name, apicharsys->char_data[character_type].file) == 0) {
            return character_type;
        }
    }
    return -1;
}

static i32 GameFindAlternativeSpecialObject(AISYS *, nuhspecial_s *special) {
    return EquivalentObject_Find(WorldInfo_CurrentlyActive(), special);
}

static void GameAILoad(AISYS *, i32, NUGSCN *, VARIPTR *, VARIPTR *) {
}

static void GlobalCharacterRender(NUVEC *, i16, i32, i32, EDCREATURE_s *) {
}

static f32 GetCharacterGoalSpeed(APIOBJECT *object) {
    if (object == NULL || object->ai == NULL) {
        return 0.0f;
    }

    switch (object->ai->goal_speed_mode) {
        case AI_ACTION_SPEED_RUN:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->movement_speed * FRAMETIME;
        case AI_ACTION_SPEED_WALK:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x18 * FRAMETIME;
        case AI_ACTION_SPEED_TIPTOE:
            return static_cast<GAMECHARACTERDATA *>(object->character_data->field11_0x24)->field_0x14 * FRAMETIME;
        default:
            return 0.0f;
    }
}

static i32 GameAIActionParseSpeed(char *name, u8 *speed) {
    if (NuStrICmp(name, "RUN") == 0) {
        *speed = AI_ACTION_SPEED_RUN;
        return 1;
    }
    if (NuStrICmp(name, "WALK") == 0) {
        *speed = AI_ACTION_SPEED_WALK;
        return 1;
    }
    if (NuStrICmp(name, "TIPTOE") == 0) {
        *speed = AI_ACTION_SPEED_TIPTOE;
        return 1;
    }
    return 0;
}

static char *SpecialRouteCharacterName(u8 route_id) {
    if (route_id == 0xff) {
        return NULL;
    }
    if (route_id < 10) {
        return Suit[route_id].suit_character_name;
    }

    i32 skipped_count = 0;
    for (i32 route_index = 0; route_index < 64;) {
        const i32 list_index = route_index + skipped_count;
        const i16 character_type = CurrentStoryCList[list_index].model_id;
        if (character_type == -1 || list_index > 63) {
            return NULL;
        }

        char *name = apicharsys->char_data[character_type].file;
        bool skip = false;
        for (const char **skip_name = &skip_chars[1]; *skip_name != NULL; ++skip_name) {
            if (NuStrICmp(name, *skip_name) == 0) {
                skip = true;
                break;
            }
        }
        if (skip) {
            ++skipped_count;
            continue;
        }
        if (route_index + 10 == route_id) {
            return name;
        }
        ++route_index;
    }
    return NULL;
}

static i32 LevelCharacterTypeID(char *name) {
    if (NuStrICmp(name, "Everyone") == 0) {
        return 0x40;
    }
    if (CurrentStoryCList == NULL) {
        return -1;
    }

    i32 character_index = 0;
    i16 character_type = CurrentStoryCList[character_index].model_id;
    if (character_type == -1) {
        return -1;
    }

    while (true) {
        if (NuStrICmp(name, apicharsys->char_data[character_type].file) == 0) {
            return character_index;
        }

        ++character_index;
        character_type = CurrentStoryCList[character_index].model_id;
        if (character_type == -1) {
            return -1;
        }
        if (character_index == 64) {
            return -1;
        }
    }
}

static u32 AIBigJumpToDestination(APIOBJECT *object, NUVEC *destination) {
    if (destination == NULL || object == NULL || object->objptr == NULL || object->field_0x287 != 0 ||
        object->objptr->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE) {
        return 1;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0xcc0 != NULL) {
        SnapCreaturePos(game_object, destination, 0, NULL, 0);
    } else if ((object->character_data->model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) != 0) {
        object->start_position = *destination;
        object->position = *destination;
        object->velocity = v000;
        ResetPlayerMoves(game_object);
        object->respawn_timer = 0.0f;
    } else {
        StartBigJump(game_object, destination, 0, 0.5f, 1.0f, 0, 0);
    }

    return 1;
}

static u32 AIRespawnOnPath(APIOBJECT *object) {
    if (object->field_0x287 != 0 || (object->ai->path_info.flags & AI_RESPAWN_FLAG_DISABLED) != 0 ||
        (object->flags_high & APIOBJECT_HIGH_FLAG_RESPAWN_ENABLED) == 0) {
        return 0;
    }

    const u32 model_flags = object->character_data->model_flags;
    if ((model_flags & CHARACTER_AI_MODEL_FLAG_DISABLE_RESPAWN) != 0) {
        return 0;
    }

    GameObject_s *game_object = object->objptr;
    if (game_object->field_0x7a5 == AI_GAME_OBJECT_TYPE_VEHICLE || game_object->field_0xcc0 != NULL) {
        return 0;
    }

    if (object->respawn_timer > AI_RESPAWN_DELAY) {
        if ((model_flags & CHARACTER_AI_MODEL_FLAG_SNAP_ON_BIG_JUMP) != 0) {
            object->start_position = object->respawn_position;
            object->position = object->respawn_position;
            object->velocity = v000;
            ResetPlayerMoves(game_object);
            object->respawn_timer = 0.0f;
        } else {
            StartBigJump(game_object, &object->respawn_position, 0, 0.5f, 1.0f, 0, 0);
        }
    }

    return 0;
}

static i32 SpecialRouteCharacterTypeID(char *name) {
    if (NuStrICmp(name, "Everyone") == 0) {
        return 0x40;
    }

    for (i32 suit_index = 0; suit_index < 10; ++suit_index) {
        if (NuStrICmp(name, Suit[suit_index].suit_character_name) == 0) {
            return suit_index;
        }
    }

    if (CurrentStoryCList == NULL) {
        return -1;
    }
    i32 skipped_count = 0;
    for (i32 route_index = 0; route_index < 64;) {
        const i32 list_index = route_index + skipped_count;
        const i16 character_type = CurrentStoryCList[list_index].model_id;
        if (character_type == -1 || list_index > 63) {
            return -1;
        }

        char *character_name = apicharsys->char_data[character_type].file;
        bool skip = false;
        for (const char **skip_name = &skip_chars[1]; *skip_name != NULL; ++skip_name) {
            if (NuStrICmp(character_name, *skip_name) == 0) {
                skip = true;
                break;
            }
        }
        if (skip) {
            ++skipped_count;
            continue;
        }
        if (NuStrICmp(name, character_name) == 0) {
            return route_index + 10;
        }
        ++route_index;
    }
    return -1;
}

extern "C" {
    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);
    void PlatOnOff(i32 platform_id, i32 enabled);
}
extern i32 TimingBarSet;
extern i32 SHADOWCALLS;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask) {
    i32 object_platform_id = -1;
    if (object != NULL && object->field_0x107c != -1) {
        object_platform_id = object->field_0x107c;
        PlatOnOff(object_platform_id, 0);
    }

    if (TimingBarSet == 2) {
        TBOPENFN("Ter", 2);
    }
    const f32 shadow_height = NewShadowEx(position, 0, probe_height, probe_height, terrain_mask);
    ++SHADOWCALLS;
    if (TimingBarSet == 2) {
        TBCLOSEFN("Ter", 2);
    }

    if (object_platform_id != -1) {
        PlatOnOff(object_platform_id, 1);
    }
    return shadow_height;
}

extern f32 MainRenderTime;
extern f32 MainRenderTargetTime;
extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
extern void *CutScenePlayer_Active();

void UpdateGameMessages();
extern i32 DoubleScore;

void GameTiming(WORLDINFO_s *, float *game_time) {
    if (Paused == 0) {
        if (game_time != NULL) {
            *game_time += FRAMETIME;
        }
        UpdateTimer(&GameTimer);
        UpdateTimer(&LevelTimer);
        UpdateTimer(&AreaTimer);
        if (CUTSTOPGAME == 0) {
            UpdateGameMessages();
            f32 target = 0.0f;
            if (DoubleScore != 0 && GetMenuID() == -1)
                target = 1.0f;
            DoubleScoreTime = SeekLinearF(DoubleScoreTime, target, FRAMETIME);
        } else {
            DoubleScoreTime = 0.0f;
        }
    } else {
        DoubleScoreTime = 0.0f;
    }

    UpdateTimer(&GlobalTimer);
    menu_flash = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.2f) < 0.1f;

    f32 pulse_time = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
    game_pulse = NuTrigTable[(i32)(pulse_time * 2.0f * 65536.0f) >> 1 & 0x7fff];
    pulse_time = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
    global_pulse = NuTrigTable[(i32)(pulse_time * 2.0f * 65536.0f) >> 1 & 0x7fff];

    MainRenderTime = SeekLinearF(MainRenderTime, MainRenderTargetTime, FRAMETIME);
    qrand();
}

void GameFog_Set() {
    if (NuIOS_IsLowEndDevice()) {
        NuLightFogX(GameFog.low_quality_start, GameFog.low_quality_end, GameFog.colour, 0.0f, 0.0f, 0, 0.0f);
        return;
    }

    NuLightFogX(GameFog.high_quality_start, GameFog.high_quality_end, GameFog.colour, 0.0f, 0.0f, 1,
                GameFog.high_quality_density);
}

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask) {
    i32 hit = NewRayCastScaleYMask(position, displacement, radius, 1.0f, 0, mask);
    ++RAYCASTCALLS;
    return hit;
}

void GameAIProcess() {
    if (WORLD == NULL || WORLD->ai_sys == NULL || Obj == NULL) {
        return;
    }

    APIOBJECT *first_player = player != NULL ? &player->apiobj : NULL;
    APIOBJECT *second_player = player2 != NULL ? &player2->apiobj : NULL;
    AISysProcess(WORLD->ai_sys, first_player, second_player);

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags || object->apiobj.field_0x287 != 0) {
            continue;
        }

        i32 ground_checks = object->apiobj.field_0x27d != 0;
        if (ground_checks == 0 && object->apiobj.character_data != NULL) {
            ground_checks = (object->apiobj.character_data->model_flags >> 13) & 1;
        }

        const i32 process_ai = (object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0;
        AISysProcessCharacter(WORLD->ai_sys, &object->apiobj, &object->ai, ground_checks, object->ai_elapsed_time, 0,
                              process_ai);
    }
    oneAtOnce_MaintainArray();
}

extern "C" {
    void InitFn_LevelCharacterTypeID(CHARACTERTYPEIDFN *function) {
        LevelCharacterTypeIDFn = function;
        SpecialRouteCharacterTypeIDFn = function;
    }

    void InitFn_SpecialRouteCharacterTypeID(CHARACTERTYPEIDFN *function) {
        SpecialRouteCharacterTypeIDFn = function;
    }

    void InitFn_LevelCharacterName(CHARACTERNAMEFN *function) {
        LevelCharacterNameFn = function;
        SpecialRouteCharacterNameFn = function;
    }

    void InitFn_SpecialRouteCharacterName(CHARACTERNAMEFN *function) {
        SpecialRouteCharacterNameFn = function;
    }

    void InitFn_LevelCharacterGlobalID(CHARACTERGLOBALIDFN *function) {
        LevelCharacterGlobalIDFn = function;
    }

    void InitFn_GlobalCharacterTypeID(CHARACTERTYPEIDFN *function) {
        GlobalCharacterTypeIDFn = function;
    }

    void InitFn_GlobalCharacterName(GLOBALCHARACTERNAMEFN *function) {
        GlobalCharacterNameFn = function;
    }

    void InitFn_GlobalCharacterRender(CHARACTERRENDERFN *function) {
        GlobalCharacterRenderFn = function;
    }

    void InitFn_GlobalCharacterHGobj(CHARACTERHGOBJFN *function) {
        GlobalCharacterHGobjFn = function;
    }

    void InitFn_ClearAICreatures(AICLEARCREATURES *function) {
        ClearAICreaturesFn = function;
    }

    void InitFn_GetCharacterGoalSpeedFn(CHARACTERGOALSPEEDFN *function) {
        GetCharacterGoalSpeedFn = function;
    }

    void InitFn_GetViewRange(CHARACTERDISTANCEFN *function) {
        GetViewRangeFn = function;
    }

    void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function) {
        GetHearDistanceFn = function;
    }

    void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function) {
        GetMaxViewHeightFn = function;
    }

    void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function) {
        GetMinViewHeightFn = function;
    }

    void InitFn_GameAILoad(GAMEAILOAD *function) {
        GameAILoadFn = function;
    }

    void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function) {
        AIActionParseSpeedFn = function;
    }

    void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function) {
        AIRespawnOnPathFn = function;
    }

    void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function) {
        AIBigJumpToDestinationFn = function;
    }

    void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function) {
        FindAlternativeSpecialObjectFn = function;
    }

    void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function) {
        APIOBJECTFromObjIDFn = function;
    }

    void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function) {
        GetNamedAPIObjectFn = function;
    }

    void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function) {
        GetAICreatureOriginFn = function;
    }

    void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level) {
        if (level < SCRIPT_ERROR_LEVEL_COUNT) {
            ScriptErrorLevel = level;
        }
    }
}

void GameAISysInit() {
    RegisterAIScriptActions(lego_aiactiondefs);
    RegisterAIScriptConditions(lego_aiconditiondefs);
    InitFn_LevelCharacterTypeID(LevelCharacterTypeID);
    InitFn_SpecialRouteCharacterTypeID(SpecialRouteCharacterTypeID);
    InitFn_LevelCharacterName(LevelCharacterName);
    InitFn_SpecialRouteCharacterName(SpecialRouteCharacterName);
    InitFn_LevelCharacterGlobalID(LevelCharacterGlobalID);
    InitFn_GlobalCharacterTypeID(GlobalCharacterTypeID);
    InitFn_GlobalCharacterName(GlobalCharacterName);
    InitFn_GlobalCharacterRender(GlobalCharacterRender);
    InitFn_GlobalCharacterHGobj(GlobalCharacterHGobj);
    InitFn_ClearAICreatures(ClearAICreatures);
    InitFn_GetCharacterGoalSpeedFn(GetCharacterGoalSpeed);
    InitFn_GetViewRange(GetViewRange);
    InitFn_GetHearDistance(GetHearDistance);
    InitFn_GlobalGetMaxViewHeight(GetMaxViewHeight);
    InitFn_GlobalGetMinViewHeight(GetMinViewHeight);
    InitFn_GameAILoad(GameAILoad);
    InitFn_AIActionParseSpeed(GameAIActionParseSpeed);
    InitFn_AIRespawnOnPath(AIRespawnOnPath);
    InitFn_AIBigJumpToDestination(AIBigJumpToDestination);
    InitFn_FindAlternativeSpecialObjectFn(GameFindAlternativeSpecialObject);
    InitFn_APIOBJECTFromObjIDFn(GameAPIOBJECTFromObjID);
    InitFn_GetNamedAPIObject(GetNamedAPIObject);
    InitFn_GetAICreatureOrigin(GetAICreatureOrigin);

    default_path_heighttol = 0.2f;
    default_activate_difficulty = AI_DEFAULT_ACTIVATE_DIFFICULTY;
    default_min_n_respawns = 0;
    default_max_n_respawns = 0;
    default_min_t_respawn = 0.0f;
    default_max_t_respawn = 0.0f;

    SetScriptErrorLevel(SCRIPT_ERROR_LEVEL_WARNING);
    GameAISysSetGame();
}

void GameFog_Reset() {
    GameFogSnap = 1;
    GameFogDuration = 0.0f;
    GameFogSet = 0;
    GameFogTime = 0.0f;
}

void Game_KillPart(PART_s *, i32) {
}

void GameAISysReset(AISYS_s *system) {
    if (system == NULL) {
        return;
    }

    AISysSetLevelPath(system, NULL);

    WORLD->processor_count = 0;
    for (i32 script_index = 0; script_index < 32; ++script_index) {
        char script_name[16];
        if (script_index != 0) {
            sprintf(script_name, "Level%d", script_index);
        } else {
            sprintf(script_name, "Level");
        }

        if (AIScriptFind(WORLD->ai_sys, script_name, 0, 1, 0) == NULL) {
            continue;
        }

        AIScriptProcessorInit(system, NULL, &WORLD->processors[WORLD->processor_count].processor, NULL, script_name,
                              NULL, 0, NULL, NULL);
        NuStrCpy(WORLD->processors[WORLD->processor_count].name, script_name);
        LevelScriptReStoreProgress(WORLD, &WORLD->processors[WORLD->processor_count]);
        ++WORLD->processor_count;
    }

    AIPATHSYS *path_system = system->path_sys;
    if (path_system != NULL) {
        for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
            AIPATH *path = path_system->paths[path_index];
            AIPATHCNX *connection = path->connections;
            for (i32 connection_index = 0; connection_index < path->connection_count;
                 ++connection_index, ++connection) {
                connection->node_a = connection->previous_node_a;
                connection->node_b = connection->previous_node_b;

                if (LEGO_AIPATHCNX_BLOCKAGE != 0) {
                    connection->node_a &= ~LEGO_AIPATHCNX_BLOCKAGE;
                    connection->node_b &= ~LEGO_AIPATHCNX_BLOCKAGE;
                }
            }
        }
    }

    AIPathCnxControlSysReset(WORLD->ai_path_cnx_control_sys);
    AIPathCnxHelperSysReset(WORLD, WORLD->ai_path_cnx_helper_sys);
    InitAICreatures(system);
    ResetAICreatures(system);

    if (WORLD->processor_count != 0) {
        GizmoSysAddGizmos(WORLD->gizmo_sys, WORLD->giz_flow, WORLD);
    }
}

void GameAttackInit() {
}

extern "C" void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

static GAMEAUDIO GameAudio_Default;
__attribute__((visibility("hidden"))) GAMEAUDIO *GameAudio asm("_ZL9GameAudio");

void GameAudio_Init(GAMEAUDIO *audio) {
    GameAudio = audio;
    for (i32 i = 0; i < 0x55; ++i) {
        audio->sfx_ids[i] = static_cast<i16>(GetSfxId(audio->sfx_names[i]));
    }

    MenuRegisterSoundFX(GameAudio_GetSfxId(0x2f), GameAudio_GetSfxId(0x30), GameAudio_GetSfxId(0x31),
                        GameAudio_GetSfxId(0x32));
}

void GameFog_Update(WORLDINFO_s *) {
}

void GameAudio_Reset() {
    memset(&GameAudio_Default, 0, sizeof(GameAudio_Default));
    GameAudio = &GameAudio_Default;
    memset(GameAudio_Default.sfx_names, 0, sizeof(GameAudio_Default.sfx_names));
    for (i32 i = 0; i < 0x55; ++i) {
        GameAudio_Default.sfx_ids[i] = -1;
    }
}

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size) {
    // Carves `size` bytes out of the permanent buffer (original at
    // 0x4890a0); returns the previous cursor.
    void *ptr = (void *)(usize)buf->addr;
    buf->addr += size;
    return ptr;
}

void GameObj_GetName(i32, GameObject_s *, char *) {
}

void Game_AutoSaving() {
}

void GameAISysSetGame() {
    AIPathCnxHelperSysInitFn = NULL;
    StarWars_GameAISysInit();
}

void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx) {
        return;
    }

    i32 sfx_id = GameAudio_GetSfxId(sfx);
    if (sfx_id == -1) {
        return;
    }
    for (i32 i = 0; i < *sfx_count; ++i) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void GameObjectOrigin(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    api.field_0x1f4 |= 0x100u;

    f32 predicted_vertical_displacement;
    f32 origin_x;
    f32 origin_z;
    if (object->use_model_origin != 0) {
        PLAYERCHARACTERCONFIG_s *config = api.character_data->player_config;
        const i32 model_origin_joint = config->model_origin_joint;
        CHARACTERMODEL_s *model = api.character_model;
        if (model_origin_joint != -1 && model != NULL && model->points_of_interest[model_origin_joint] != NULL) {
            const f32 frame_time = FRAMETIME;
            const f32 predicted_x = api.previous_velocity.x * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            const f32 predicted_z = api.previous_velocity.z * frame_time;
            if ((object->field_0xe24 & GAMEOBJECT_E24_FLAG_JOINT_MATRICES_UPDATED) == 0) {
                NUVEC local_origin = {0.0f, -object->character_bottom, 0.0f};
                NuVecMtxRotate(&api.collision_position, &local_origin, &api.field_0xb8);
                NuVecAdd(&api.collision_position, &api.collision_position, &api.position);
                api.collision_position.x += predicted_x;
                api.collision_position.y += predicted_vertical_displacement;
                api.collision_position.z += predicted_z;
            } else {
                const NUVEC &joint_position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[model_origin_joint], 3);
                api.collision_position.x = joint_position.x + predicted_x * 2.0f;
                api.collision_position.y = joint_position.y + predicted_vertical_displacement * 2.0f;
                api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
                api.collision_position.z = joint_position.z + predicted_z * 2.0f;
            }
            origin_x = api.position.x + predicted_x;
            origin_z = api.position.z + predicted_z;
            goto update_bounds;
        }

        const i32 collision_origin_joint = config->collision_origin_joint;
        if (object->field_0xd24 == 1.0f && collision_origin_joint != -1 &&
            model->points_of_interest[collision_origin_joint] != NULL && api.field_0x288 != 0) {
            api.collision_position = *NUMTX_GET_ROW_VEC(&object->joint_matrices[collision_origin_joint], 3);
            const f32 frame_time = FRAMETIME;
            origin_x = api.position.x + api.previous_velocity.x * frame_time;
            origin_z = api.position.z + api.previous_velocity.z * frame_time;
            predicted_vertical_displacement = api.previous_velocity.y * frame_time;
            goto update_bounds;
        }
    }

    api.field_0x1f4 &= ~0x100u;
    predicted_vertical_displacement = api.previous_velocity.y * FRAMETIME;
    api.collision_position.x = api.position.x + api.previous_velocity.x * FRAMETIME;
    api.collision_position.y = api.position.y + predicted_vertical_displacement;
    api.collision_position.y += (object->character_bottom + object->character_top) * api.field_0xa8 * 0.5f;
    api.collision_position.z = api.position.z + api.previous_velocity.z * FRAMETIME;
    origin_x = api.collision_position.x;
    origin_z = api.collision_position.z;

update_bounds:
    const f32 radius = api.field_0x1dc;
    const f32 half_height = api.field_0x1e0;
    api.collision_min.x = api.collision_position.x - radius;
    api.collision_min.y = api.collision_position.y - half_height;
    api.collision_min.z = api.collision_position.z - radius;
    api.collision_max.x = api.collision_position.x + radius;
    api.collision_max.y = api.collision_position.y + half_height;
    api.collision_max.z = api.collision_position.z + radius;

    api.upper_position.x = origin_x;
    api.upper_position.y = api.collision_max.y;
    api.upper_position.z = origin_z;
    api.lower_position.x = origin_x;
    api.lower_position.y = api.collision_min.y;
    api.lower_position.z = origin_z;
    api.collision_origin.x = origin_x;
    api.collision_origin.y = api.collision_min.y + predicted_vertical_displacement;
    api.collision_origin.z = origin_z;

    object->ai.terrain_origin = api.collision_origin;
    const u32 context_flags = CInfo[object->character_context].flags;
    if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_TOP) != 0 &&
        (object->context_variant_flags & 0x08) != 0) {
        object->ai.terrain_origin.y += api.collision_max.y - api.collision_min.y;
    } else if ((context_flags & CHARACTER_CONTEXT_INFO_FLAG_TERRAIN_ORIGIN_AT_POSITION) != 0) {
        object->ai.terrain_origin = api.position;
    } else {
        object->ai.terrain_origin.y = api.collision_origin.y - object->terrain_origin_floor_offset;
        if (object->ai.terrain_origin.y < api.field_0x218 &&
            api.field_0x218 < object->ai.terrain_origin.y + api.scaled_height) {
            object->ai.terrain_origin.y = api.field_0x218;
        }
    }
}

i32 Game_IgnoreInput() {
    extern i32 newgamecam;
    return newgamecam != 0;
}

void GameAI_TotalScore() {
}

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume) {
    if ((u32)sfx < 0x55) {
        GameAudio_PlaySfxById(GameAudio->sfx_ids[sfx], position, flags, volume);
    }
}

void GameDrawMenuEntry(MENU_s *menu, char *text) {
    if (Paused != 0) {
        dme_align = PauseMenus_Align;
        menu->draw_x = PauseMenus_X;
    }
    DrawMenuEntryEx(menu, text, static_cast<u8>(MenuA));
}

void GameAnimSys_Update(GAMEANIMSYS_s *system) {
    if (system == NULL) {
        return;
    }

    GAMEANIMSET_s *set = reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetHead(&system->active_sets));
    while (set != NULL) {
        GAMEANIMSET_s *next_set =
            reinterpret_cast<GAMEANIMSET_s *>(NuLinkedListGetNext(&system->active_sets, &set->links));

        const u8 was_no_visibility_test = set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
        set->flags &= ~GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;

        if ((set->flags & GAMEANIMSET_FLAG_STOP_REQUESTED) != 0) {
            set->flags &= ~(GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED);
            GameAnimSet_RemoveFromSystemList(set);
            set = next_set;
            continue;
        }

        set->state = GAMEANIMSET_STATE_AT_START;
        i32 all_playing_forward = 1;
        i32 all_at_start = 1;
        i32 all_at_end = 1;
        i32 any_special_no_visibility_test = 0;

        GAMEANIMOBJ_s *object = set->objects;
        while (object != NULL) {
            if (NuSpecialGetNoVisiTestFn(&object->special) != 0) {
                any_special_no_visibility_test = 1;
            }

            if (object->instance_animation != NULL) {
                const f32 direction = object->start_frame > object->end_frame ? -1.0f : 1.0f;
                const f32 previous_frame = object->instance_animation->ltime;

                if (previous_frame * direction >= object->end_frame * direction) {
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        object->instance_animation->ltime = object->end_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = previous_frame - object->end_frame + object->start_frame;
                    } else {
                        object->instance_animation->ltime = object->end_frame;
                        object->instance_animation->playing = 0;
                    }
                } else if (previous_frame * direction <= object->start_frame * direction) {
                    if (object->instance_animation->tfactor * direction > 0.0f) {
                        object->instance_animation->ltime = object->start_frame;
                    } else if (object->instance_animation->repeating != 0) {
                        object->instance_animation->ltime = object->end_frame - (object->start_frame - previous_frame);
                    } else {
                        object->instance_animation->ltime = object->start_frame;
                        object->instance_animation->playing = 0;
                    }
                }

                if (previous_frame != object->instance_animation->ltime) {
                    EvalAnim2(&object->special, object->instance_animation->ltime);
                }

                if (object->instance_animation->playing != 0) {
                    set->flags |= GAMEANIMSET_FLAG_NO_VISIBILITY_TEST;
                    if (object->instance_animation->tfactor * direction < 0.0f) {
                        all_playing_forward = 0;
                    }
                }

                const f32 directed_frame = object->instance_animation->ltime * direction;
                if (directed_frame > object->start_frame * direction) {
                    all_at_start = 0;
                }
                if (directed_frame < object->end_frame * direction) {
                    all_at_end = 0;
                }
            }
            object = object->next;
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != any_special_no_visibility_test) {
            object = set->objects;
            while (object != NULL) {
                NuSpecialSetNoVisiTest(&object->special, set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST);
                object = object->next;
            }
        }

        if ((set->flags & GAMEANIMSET_FLAG_NO_VISIBILITY_TEST) != 0) {
            set->state =
                all_playing_forward != 0 ? GAMEANIMSET_STATE_ACTIVE_FORWARD : GAMEANIMSET_STATE_ACTIVE_BACKWARD;
        } else {
            if (all_at_end != 0) {
                set->state = GAMEANIMSET_STATE_AT_END;
            } else if (all_at_start == 0) {
                set->state = GAMEANIMSET_STATE_BETWEEN_ENDPOINTS;
            }
            if (was_no_visibility_test != 0) {
                set->flags |= GAMEANIMSET_FLAG_STOP_REQUESTED;
            }
        }

        if ((set->flags & (GAMEANIMSET_FLAG_NO_VISIBILITY_TEST | GAMEANIMSET_FLAG_STOP_REQUESTED)) == 0) {
            GameAnimSet_RemoveFromSystemList(set);
        }
        set = next_set;
    }
}

i32 GameAudio_GetSfxId(i32 sfx) {
    if (static_cast<u32>(sfx) <= 0x54) {
        return GameAudio->sfx_ids[sfx];
    }
    return -1;
}

void GameObjIsCableTied(GameObject_s *) {
}

void GameObjectRotation(GameObject_s *, i32) {
}

// Original: reads the user's music volume from the options save as the product
// of two 0..10 sliders scaled to 0..1 (option bytes at 0x4 and 0x5).
f32 GameGetMusicVolume(OPTIONSSAVE_s *options) {
    return ((f32)(u8)options->field5_0x5 / 10.0f) * ((f32)(u8)options->field4_0x4 / 10.0f);
}

// Original: applies GameGetMusicVolume, zeroing it while the title logos are
// up (SuperOptions.music_enabled == 0 on the titles level); the title menu restores
// the user's volume via GameGetMusicVolume once the menu phase starts.
f32 GameSetMusicVolume(OPTIONSSAVE_s *options) {
    f32 volume = GameGetMusicVolume(options);
    if (SuperOptions.music_enabled == 0 && WORLD->current_level == TITLES_LDATA) {
        volume = 0.0f;
    }
    legoSetMusicVolume(volume);
    return volume;
}

void GameAISysStartFrame(AISYS_s *system) {
    if (system == NULL || netclient != 0) {
        return;
    }

    if (system->path_sys != NULL && system->path_sys->path_count != 0) {
        for (i32 index = 0; index < system->path_sys->path_count; ++index) {
            memset(&system->path_sys->paths[index]->updated_node_bits[0], 0, 0x20);
            memmove(&system->path_sys->paths[index]->updated_node_bits[0x20],
                    system->path_sys->paths[index]->inside_node_bits, 0x20);
            memset(system->path_sys->paths[index]->inside_node_bits, 0,
                   sizeof(system->path_sys->paths[index]->inside_node_bits));
        }

        // Moving specials can carry path endpoints. Update both ends of each
        // connection; AIPathNodeUpdatePos uses updated_node_bits to ensure a
        // shared endpoint is transformed only once this frame.
        for (i32 path_index = 0; path_index < system->path_sys->path_count; ++path_index) {
            AIPATH *path = system->path_sys->paths[path_index];
            for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                AIPATHCNX *connection = &path->connections[connection_index];
                AIPATHNODE *node = &path->nodes[connection->direction_a];
                if (node->has_special != 0) {
                    AIPathNodeUpdatePos(system, path, node);
                }
                node = &path->nodes[connection->direction_b];
                if (node->has_special != 0) {
                    AIPathNodeUpdatePos(system, path, node);
                }
            }
        }
    }

    // Spread the object/area overlap work across frames. Area runtime flags
    // summarize the kinds of live characters inside it, while every object
    // retains a 64-area occupancy mask for script queries.
    if (system->next_area_check < system->area_count) {
        AIAREA *area = &system->areas[system->next_area_check];
        const i32 area_index = static_cast<i32>(area - WORLD->ai_sys->areas);
        const u64 area_bit = 1ULL << area_index;
        area->runtime_flags &= static_cast<u8>(
            ~(AIAREA_RUNTIME_PLAYER_PRESENT | AIAREA_RUNTIME_OBJECT_STATE_CLEAR | AIAREA_RUNTIME_OBJECT_STATE_SET));

        GameObject_s *object = Obj;
        for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
            if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) !=
                (APIOBJECT_FLAG_CHARACTER | APIOBJECT_FLAG_IN_USE)) {
                continue;
            }
            if (object->apiobj.field_0x287 != 0 && !(object->field_0x101c > 0.0f) &&
                static_cast<i8>(object->apiobj.flags_low) >= 0) {
                continue;
            }

            NUVEC local_position;
            NuVecSub(&local_position, &object->apiobj.position, &area->position);
            NuVecRotateY(&local_position, &local_position, -area->rotation);
            const bool is_inside = local_position.x >= -area->half_width && local_position.y >= -0.1f &&
                                   local_position.z >= -area->half_depth && local_position.x <= area->half_width &&
                                   local_position.y <= area->height && local_position.z <= area->half_depth;
            if (!is_inside) {
                object->ai_area_mask_low &= ~static_cast<u32>(area_bit);
                object->ai_area_mask_high &= ~static_cast<u32>(area_bit >> 32);
                continue;
            }

            object->ai_area_mask_low |= static_cast<u32>(area_bit);
            object->ai_area_mask_high |= static_cast<u32>(area_bit >> 32);
            if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_PLAYER_PRESENT;
            }
            if (object->apiobj.field_0x27c != -1) {
                area->runtime_flags |= AIAREA_RUNTIME_CHARACTER_SLOT_SEEN;
            }
            if ((object->apiobj.field_0x1f4 & 1) != 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_SET;
            } else if ((object->apiobj.field_0x1f4 & 4) == 0) {
                area->runtime_flags |= AIAREA_RUNTIME_OBJECT_STATE_CLEAR;
            }
        }

        ++system->next_area_check;
        if (system->next_area_check >= system->area_count) {
            system->next_area_check = 0;
        }
    }

    // Level scripts are ordinary AI processors without an object or packet.
    // Disabled processors retain their state but do not advance this frame.
    for (i32 index = 0; index < WORLD->processor_count; ++index) {
        if (!WORLD->processors[index].processor.is_disabled) {
            AIScriptProcess(system, NULL, NULL, &WORLD->processors[index].processor, FRAMETIME);
        }
    }
}

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours) {
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && cut->camera_near_clip != 0.0f) {
        pNuCam->near_clip = cut->camera_near_clip;
    } else {
        pNuCam->near_clip = display->unknown_04;
    }

    u16 far_clip;
    if (cut != NULL) {
        far_clip = cut->camera_far_clip;
        if (far_clip == 0) {
            far_clip = static_cast<u16>(display->unknown_14);
        }
    } else {
        far_clip = static_cast<u16>(display->unknown_14);
    }
    pNuCam->far_clip = static_cast<f32>(static_cast<u32>(far_clip));

    LEVELDATA *level = WORLD->current_level;
    const bool use_backdrop = level == TITLES_LDATA || (level->flags & LEVEL_STATUS) != 0 || level == STATUS_LDATA ||
                              level == CREDITS_LDATA || MainRenderTime < 0.0f;
    if (!use_backdrop) {
        background_colours[0] =
            0x80000000u | static_cast<u8>(display->bg_red_top) |
            (static_cast<u8>(display->bg_green_top) << 8 | static_cast<u8>(display->bg_blue_top) << 16);
        background_colours[1] =
            0x80000000u | static_cast<u8>(display->bg_red_bottom) |
            (static_cast<u8>(display->bg_green_bottom) << 8 | static_cast<u8>(display->bg_blue_bottom) << 16);
        return;
    }

    BackDrop_UpdateColours(0);

    i32 top_g = static_cast<i32>(backdrop_top_g);
    i32 top_r = static_cast<i32>(backdrop_top_r);
    i32 top_b = static_cast<i32>(backdrop_top_b);
    i32 bottom_r = static_cast<i32>(backdrop_bot_r);
    i32 bottom_g = static_cast<i32>(backdrop_bot_g);
    i32 bottom_b = static_cast<i32>(backdrop_bot_b);

    f32 alpha = 1.0f;
    if (BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
        if (alpha != 0.0f) {
            top_r = static_cast<i32>(static_cast<f32>(top_r) * alpha);
            top_g = static_cast<i32>(static_cast<f32>(top_g) * alpha);
            top_b = static_cast<i32>(static_cast<f32>(top_b) * alpha);
            bottom_r = static_cast<i32>(static_cast<f32>(bottom_r) * alpha);
            bottom_g = static_cast<i32>(static_cast<f32>(bottom_g) * alpha);
            bottom_b = static_cast<i32>(static_cast<f32>(bottom_b) * alpha);
        }
    }

    background_colours[0] = 0x80000000u | (top_r & 0xff) | (top_g & 0xff) << 8 | (top_b & 0xff) << 16;
    background_colours[1] = 0x80000000u | (bottom_r & 0xff) | (bottom_g & 0xff) << 8 | (bottom_b & 0xff) << 16;
}

void GameObjectSetCanUse(GameObject_s *, void *, unsigned char, unsigned char, float) {
}

void GameObjOwnsAnyCables(GameObject_s *) {
}

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

void GameObjectDimensions(GameObject_s *object) {
    APIOBJECT &api = object->apiobj;
    PLAYERCHARACTERCONFIG_s *config = api.character_data->player_config;
    const i32 collision_origin_joint = config->collision_origin_joint;
    if (object->use_model_origin != 0 && object->field_0xd24 == 1.0f && collision_origin_joint != -1 &&
        api.character_model->points_of_interest[collision_origin_joint] != NULL && api.field_0x288 != 0) {
        const f32 radius = object->field_0x1004 * config->collision_origin_radius;
        api.field_0x1dc = radius;
        api.field_0x1e0 = radius;
    } else {
        api.field_0x1dc = api.collision_radius;
        api.field_0x1e0 = api.collision_height;
    }
    GameObjectDimensionsExtra_LSW(object);
}

void GameObjectUsingLever(GameObject_s *, LEVER_s *) {
}

void GameAntiNodeData_Init(GAMEANTINODEDATA_s *data, nuhspecial_s *) {
    if (data != NULL) {
        memset(data, 0, sizeof(*data));
    }
}

void GameAntiNodeData_Read(GAMEANTINODEDATA_s *data) {
    if (EdFileReadChar() == 0) {
        return;
    }
    EdFileReadNuVec(&data->position);
    data->radius = EdFileReadFloat();
    data->min_y = EdFileReadFloat();
    data->max_y = EdFileReadFloat();
    data->extent_x = EdFileReadFloat();
    data->extent_z = EdFileReadFloat();
    data->flags = EdFileReadShort();
    data->use_largest_extent = static_cast<u8>(EdFileReadChar());
    data->mode = static_cast<u8>(EdFileReadChar());
}

extern "C" {
    NUVEC nusound_special_positions[4];
    void PlaySfxById(i32 sfx_id, nuvec_s *position);
}

void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32) {
    if (flags == 0) {
        PlaySfxById(sfx_id, position);
        return;
    }
    if ((flags & ~2) == 1) {
        nusound_special_positions[1] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[1]);
    }
    flags -= 2;
    if (static_cast<u32>(flags) <= 1) {
        nusound_special_positions[2] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[2]);
    }
}

void Game_GotAllGoldBricks() {
}

APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id) {
    if (object_id >= HIGHGAMEOBJECT) {
        return NULL;
    }

    GameObject_s *object = &Obj[object_id];
    if ((object->apiobj.flags_low & APIOBJECT_FLAG_IN_USE) == 0) {
        return NULL;
    }

    if ((object->apiobj.flags_high & APIOBJECT_HIGH_FLAG_PLAYER_CHARACTER) == 0 &&
        object->ai.reset_mode != AI_OBJECT_ROUTE_STATE_SCRIPT_VISIBLE) {
        return NULL;
    }
    if (object->apiobj.field_0x287 != 0 && object->field_0x101c <= 0.0f) {
        return NULL;
    }
    return &object->apiobj;
}

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags) {
    if (model == NULL) {
        return 0;
    }

    drawcharactermodel_keepmergeaction = game_keepmergeaction;
    MakeLayerList = GCDataList[model->model_id].make_layer_list;

    CHARACTERDATA *character_data =
        object != NULL ? object->apiobj.character_data : &apicharsys->char_data[model->model_id];

    // The original reserves a fixed 256-matrix evaluation array in this
    // wrapper before calling APIDrawCharacterModel.
    NUMTX output_matrices[256];
    return APIDrawCharacterModel(model, character_data, animation, matrix, secondary_matrix, reflection_matrix, 0,
                                 auxiliary_matrix, object, flags, NULL, 0, WORLD, FRAMETIME, output_matrices, 0, NULL);
}

void GameObjectToCameraCode(GameObject_s *) {
}

void GameRegisterGizActions() {
    RegisterGizActions(game_gizactiondefs);
}

i32 GameAudio_GetPlrSfxBits(void *object_ptr) {
    APIOBJECT *object = static_cast<APIOBJECT *>(object_ptr);
    i32 sfx_bits = 0;
    if (object != NULL && static_cast<i8>(object->flags_low) < 0) {
        sfx_bits = 1 << object->field_0x27c;
    }
    return sfx_bits;
}

void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *) {
}

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area) {
    if (area_models != 0 && CutScenePlayer_Active() != 0 && area != -1 && &ADataList[area] != HUB_ADATA) {
        area_models = 0;
    }

    APILoadCharacterModels(list, append, buf, *buf_end, area_models);
}

i32 Game_100PercentComplete() {
    if (Game_CompletionSave == NULL) {
        return 0;
    }
    return reinterpret_cast<STATUSCOLLECT_s *>(Game_CompletionSave)->flags & 1;
}

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *) {
    if (world->current_level == TITLES_LDATA) {
        *menu_id = 0;
    } else if (world->current_level == CREDITS_LDATA) {
        *menu_id = 30;
    }
}

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *, i32) {
}

void GameAnimSys_GetProgressData(i32) {
}

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *, i32) {
}

void GameObjectToCameraDistances() {
    const NUVEC camera_position = GameCam->pos;
    GameObject_s *object = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++object) {
        const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & required_flags) != required_flags) {
            continue;
        }

        const f32 dx = camera_position.x - object->apiobj.position.x;
        const f32 dy = camera_position.y - object->apiobj.position.y;
        const f32 dz = camera_position.z - object->apiobj.position.z;
        object->ai_update_distance = NuFsqrt(dx * dx + dy * dy + dz * dz);
    }
}

void GameCreatureOpponentSelection(AISYS_s *, i32, APIOBJECT_s **, i32, APIOBJECT_s **, i32, APIOBJECT_s **, u64,
                                   float) {
}

void GameObjectDimensionsExtra_LSW(GameObject_s *) {
}

i32 AnakinGreenSabre(GameObject_s *object);
extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
void NewRumble(nupad_s *, f32, i32);
i32 CannotKill(GameObject_s *object);
u16 ObjHitObj_Flags(GameObject_s *object);
i32 ObjHitObj(GameObject_s *, GameObject_s *, i32, u16, i32, i32);
void AddStreakPoints(NUVEC *, f32, u32, void **, i32, void *);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);
i32 GizmoBlowupBlowup(GIZMOBLOWUP_s *, i32, i32, i32, GameObject_s *, i32);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, i16, i16);

static void LightSabreStreakCode(GameObject_s *object, i32 blade, i32 effect) {
    if (object->weapon_scale < 1.0f)
        return;
    const i8 context = object->character_context;
    if ((context == 0 && (object->action_movement_state == 4 || object->action_movement_state == 2)) || context == 4 ||
        context == 16 || (context == 13 && object->id != id_THEEMPEROR) || context == 14 ||
        (context == 31 && object->field_0x7a3 == 1) ||
        ((object->apiobj.flags_low & 0x80) != 0 && Cheat_PowerUpActive(object->apiobj.field_0x27c))) {
        object->sabre_flags |= 2;
    }
    if (object->id == id_IMPERIALGUARD)
        object->sabre_flags &= ~3;
    if (object->sabre_flags == 0 || object->apiobj.field_0x288 == 0 || (object->field_0xe23 & 8) == 0)
        return;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    const i32 joint_a = data->streak_joints[blade][0];
    const i32 joint_b = data->streak_joints[blade][1];
    if (joint_a == -1 || object->apiobj.character_model->points_of_interest[joint_a] == NULL || joint_b == -1 ||
        object->apiobj.character_model->points_of_interest[joint_b] == NULL)
        return;
    object->blade_states[blade] = -1;
    NUVEC points[3];
    points[0] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_a], 3);
    points[1] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[joint_b], 3);
    if ((object->sabre_flags & 2) != 0) {
        i32 colour;
        if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25))
            colour = 0;
        else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
            colour = 3;
        else if (object->id == id_GRIEVOUS && (blade == 3 || blade == 0))
            colour = 2;
        else if (AnakinGreenSabre(object) || (object->id == id_BOB && (object->field_0xefd & 2) != 0))
            colour = 1;
        else
            colour = static_cast<i8>(GCDataList[object->id].field_0x117);
        object->blade_states[blade] = colour;
        if (object->apiobj.model_draw_result != 0) {
            const u8 *rgb = BladeTab[colour].colour;
            const u32 packed = 0xff000000u | rgb[0] | (rgb[1] << 8) | (rgb[2] << 16);
            AddStreakPoints(points, 0.25f, packed, &object->sabre_streaks[blade][0], 0, object);
            if (object->field_0x1087 != 0 && object->field_0x1020 != 2000000.0f) {
                f32 plane = object->field_0x1020;
                if (WORLD->current_level->unknown_0cc != 2000000.0f)
                    plane = WORLD->current_level->unknown_0cc;
                NUVEC reflected[2] = {points[0], points[1]};
                reflected[0].y = plane - (reflected[0].y - plane);
                reflected[1].y = plane - (reflected[1].y - plane);
                AddStreakPoints(reflected, 0.25f, packed, &object->sabre_streaks[blade][1], 1, object);
            }
        }
    }
    if ((object->sabre_flags & 7) == 0)
        return;
    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25))
        effect = 1;
    else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object))
        effect = 4;
    else if (object->id == id_GRIEVOUS)
        effect = (blade == 3 || blade == 0) ? 3 : 2;
    if (data->field275_0x116 == 12 && context == 5 && object->combo_stage == 2 && object->context_animation == 52 &&
        object->apiobj.character_model->points_of_interest[4] != NULL) {
        points[0] = *NUMTX_GET_ROW_VEC(&object->joint_matrices[4], 3);
        NuVecAdd(&points[1], &points[0], &object->apiobj.collision_position);
        NuVecScale(&points[1], &points[1], 0.5f);
    }
    NUVEC difference;
    f32 length = NuVecDist(&points[0], &points[1], &difference);
    points[2].x = difference.x * 0.5f + points[1].x;
    points[2].y = difference.y * 0.5f + points[1].y;
    points[2].z = difference.z * 0.5f + points[1].z;
    object->sabre_collision_radius = length * 0.25f;
    f32 extent = length * 1.5f;
    NUVEC minimum = {points[2].x - extent, points[2].y - extent, points[2].z - extent};
    NUVEC maximum = {points[2].x + extent, points[2].y + extent, points[2].z + extent};
    if ((object->sabre_flags & 4) != 0) {
        NUVEC direction;
        NuVecRotateY(&direction, &v001, static_cast<u16>(object->apiobj.facing_angle + (context == 16 ? 0x8000 : 0)));
        NuVecScale(&difference, &direction, 0.2f);
        NuVecAdd(&points[0], &object->apiobj.collision_position, &difference);
        NuVecScale(&difference, &direction, 0.5f);
        NuVecAdd(&points[1], &object->apiobj.collision_position, &difference);
        NuVecScale(&difference, &direction, 0.35f);
        NuVecAdd(&points[2], &object->apiobj.collision_position, &difference);
        if (context == 13) {
            points[0].y = object->apiobj.collision_min.y +
                          (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.333f;
            points[1].y = points[2].y = points[0].y;
        }
        length = NuVecDist(&points[0], &points[1], &difference);
        points[2].x = difference.x * 0.5f + points[1].x;
        points[2].y = difference.y * 0.5f + points[1].y;
        points[2].z = difference.z * 0.5f + points[1].z;
        object->sabre_collision_radius = length * 0.25f;
        extent = length * 1.5f;
        minimum = NUVEC{points[2].x - extent, points[2].y - extent, points[2].z - extent};
        maximum = NUVEC{points[2].x + extent, points[2].y + extent, points[2].z + extent};
    }
    if ((object->sabre_flags & 5) == 0)
        return;
    GameObject_s *nearest = NULL;
    i32 nearest_point = 0;
    f32 nearest_distance = 1000000.0f;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *target = &Obj[i];
        if (target == object || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->use_model_origin <= 1 ||
            target->apiobj.field_0x287 != 0 || (target->character_context & 0xfd) == 57 ||
            target->character_context == 60 || (CInfo[target->character_context].flags & 0x8000) != 0)
            continue;
        GAMECHARACTERDATA *target_data = static_cast<GAMECHARACTERDATA *>(target->apiobj.character_data->field11_0x24);
        if ((target_data->flags_090 & 0x8000) != 0 || target->apiobj.collision_min.x > maximum.x ||
            target->apiobj.collision_max.x < minimum.x || target->apiobj.collision_min.z > maximum.z ||
            target->apiobj.collision_max.z < minimum.z || target->apiobj.collision_min.y > maximum.y ||
            target->apiobj.collision_max.y < minimum.y)
            continue;
        for (i32 point = 2; point >= 0; --point) {
            if (!SphereSphereOverlapScaleY(&target->apiobj.collision_position, target->apiobj.field_0x1dc,
                                           target->apiobj.field_0x1e0, &points[point], object->sabre_collision_radius,
                                           object->sabre_collision_radius))
                continue;
            if ((object->sabre_flags & 1) != 0) {
                AddGameDebrisRot(WORLD->debris_sys, effect, &points[point], ParticlesPerSecond(10.0f, FRAMETIME), 0, 0);
            }
            const f32 distance =
                NuVecDistSqr(&object->apiobj.collision_position, &target->apiobj.collision_position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest_point = point;
                nearest = target;
            }
        }
    }
    if (nearest != NULL && (object->sabre_flags & 4) != 0) {
        AddGameDebris(WORLD->debris_sys, effect, &points[nearest_point]);
        if (!CannotKill(nearest)) {
            i32 damage = object->sabre_damage;
            if (damage != 0 && (nearest->apiobj.flags_low & 0x80) != 0) {
                damage = (object->apiobj.flags_low & 0x80) != 0 && Player_HasDoubleWeaponDamage(object) ? 2 : 1;
            }
            ObjHitObj(object, nearest, damage, ObjHitObj_Flags(object) | 0x100, 0, 1);
        } else {
            NewRumble(object->pad_gamepad->pad, 0.75f, 0);
            NewRumble(nearest->pad_gamepad->pad, 0.75f, 0);
        }
        return;
    }
    if ((object->sabre_flags & 4) != 0 && (object->apiobj.flags_low & 0x80) != 0) {
        if (GizmoBlowUp_Hit(object, points, 3, object->sabre_collision_radius, &minimum, &maximum, NULL, 0, NULL)) {
            AddGameDebris(WORLD->debris_sys, effect, &points[0]);
            AddGameDebris(WORLD->debris_sys, effect, &points[1]);
            AddGameDebris(WORLD->debris_sys, effect, &points[2]);
            NewRumble(object->pad_gamepad->pad, 0.75f, 0);
        }
    }
}

void GameObjectStuffAfterAnimation() {
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            object->apiobj.field_0x288 == 0 || object->apiobj.model_draw_result == 0)
            continue;
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if ((object->apiobj.character_data->model_flags & 8) == 0 && object->id != id_BODYGUARD &&
            object->id != id_IMPERIALGUARD)
            continue;
        const i32 effect = object->blade_index == -1 ? -1 : BladeTab[object->blade_index].hit_effect;
        if ((data->field275_0x116 == 3 || object->id == id_GRIEVOUS || object->id == id_COUNTDOOKU) &&
            object->character_context == 0 && object->action_movement_state == 3)
            object->sabre_flags |= 2;
        const u8 streak = object->sabre_flags & 2;
        LightSabreStreakCode(object, 0, effect);
        if (object->id == id_DARTHMAUL) {
            object->sabre_flags = streak | 1;
            LightSabreStreakCode(object, 1, effect);
        }
    }
}

void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message) {
    message->target_position.x = cointotal_x[message->player_index];
}

void GameAnimSys_AllocateLevelProgressData(variptr_u *, variptr_u *, i32, i32) {
}

i32 Game_Exit(i32) {
    return 0;
}

void GameObject_s::ClearAddons() {
}

void GameObject_s::ClearMechObjectInterface() {
}

void GameObject_s::GetAddons(bool) {
}

void GameObject_s::GetMechObjectInterface() {
}

void GameObject_s::IsRunningTaskType(HashedKey const &) {
}

void GameObject_s::KillTasks() {
}

// ThingManager::AddThing @0x424c10. Appends at count; the pending
// AddThingAfterThis reservation (field_0x14) is folded into the index and
// cleared here.
void ThingManager::AddThing(BaseThing *thing) {
    i32 index = this->count;
    if (thing != NULL) {
        if (index < this->max_things) {
            this->things[index] = thing;
            index = index + 1;
        }
    }
    index = index + this->field_0x14;
    this->field_0x14 = 0;
    this->count = index;
}

// ThingManager::AddThingAfterThis @0x424c40. Reserves the slot after the
// current tail: bumps field_0x14 and stores the thing at count+field_0x14;
// the next AddThing folds the reservation into count.
void ThingManager::AddThingAfterThis(BaseThing *thing) {
    if (thing != NULL) {
        i32 index = this->field_0x14 + 1;
        this->field_0x14 = index;
        index = index + this->count;
        if (index < this->max_things) {
            this->things[index] = thing;
        }
    }
}

// ThingManager::DisplayThings @0x4252c0. Single pass over Display,
// bracketed with timebar slot 3 ("Dis"). PanelRender uses this pass for the
// display-layer things that render on top of the gameplay panel.
void ThingManager::DisplayThings(ThingRenderData *data) {
    static const char *name = "Dis"; // timebar slot name @0x5734db

    if (this->count <= 0) {
        return;
    }
    for (i32 i = 0; i < this->count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL || (thing->flags & THING_FLAG_SKIP_DISPLAY)) {
            continue;
        }
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotBegin(this->timebar, 3, name);
        }
        thing->Display(data);
        thing = this->things[i];
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotEnd(this->timebar, 3);
        }
    }
}

void ThingManager::EffectsThings(ThingRenderData *) {
}

// ThingManager::EnableActions @0x425930. Finds the first thing whose 0x4 id
// matches and sets (invert==0) or clears (invert!=0) the given flags bits.
void ThingManager::EnableActions(i32 id, i32 flags, i32 invert) {
    i32 count = this->count;
    if (count <= 0) {
        return;
    }
    for (i32 i = 0; i < count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL) {
            continue;
        }
        if (thing->field_0x4 == (u32)id) {
            if (invert == 0) {
                thing->flags |= (u32)flags;
            } else {
                thing->flags &= ~(u32)flags;
            }
            return;
        }
    }
}

void ThingManager::EnterLevelThings(ThingLevelData *) {
}

void ThingManager::ExitLevelThings(ThingLevelData *) {
}

// ThingManager::ProcessThings @0x425460. Pass 1 always runs
// ProcessEvenWhenPaused first; then, per ThingProcessData.paused, either
// Process or ProcessOnlyWhenPaused. Each pass has its own opt-out flag. The count
// is re-read every iteration because thing Process calls may add things.
// Profiling: things with a non-NULL profiling handle are bracketed with
// NuTimeBarSlotBegin/End (stubbed no-ops on this build).
void ThingManager::ProcessThings(ThingProcessData *data) {
    static const char *name = "PROC"; // timebar slot name @0x5734e3

    if (this->count <= 0) {
        return;
    }
    for (i32 i = 0; i < this->count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL || (thing->flags & THING_FLAG_SKIP_PROCESS_EVEN_WHEN_PAUSED)) {
            continue;
        }
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotBegin(this->timebar, 0, name);
        }
        thing->ProcessEvenWhenPaused(data);
        thing = this->things[i];
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotEnd(this->timebar, 0);
        }
    }
    if (data->paused != 0) {
        if (this->count <= 0) {
            return;
        }
        for (i32 i = 0; i < this->count; i++) {
            BaseThing *thing = this->things[i];
            if (thing == NULL || (thing->flags & THING_FLAG_SKIP_PROCESS_ONLY_WHEN_PAUSED)) {
                continue;
            }
            if (thing->profiling_0xc != NULL) {
                _NuTimeBarSlotBegin(this->timebar, 0, name);
            }
            thing->ProcessOnlyWhenPaused(data);
            thing = this->things[i];
            if (thing->profiling_0xc != NULL) {
                _NuTimeBarSlotEnd(this->timebar, 0);
            }
        }
    } else {
        if (this->count <= 0) {
            return;
        }
        for (i32 i = 0; i < this->count; i++) {
            BaseThing *thing = this->things[i];
            if (thing == NULL || (thing->flags & THING_FLAG_SKIP_PROCESS)) {
                continue;
            }
            if (thing->profiling_0xc != NULL) {
                _NuTimeBarSlotBegin(this->timebar, 0, name);
            }
            thing->Process(data);
            thing = this->things[i];
            if (thing->profiling_0xc != NULL) {
                _NuTimeBarSlotEnd(this->timebar, 0);
            }
        }
    }
}

void ThingManager::RemoveDependanciesThings(ThingRemoveData *) {
}

void ThingManager::RemoveTemporaryThings() {
}

// ThingManager::RenderThings @0x425390. Single pass over Render,
// bracketed with timebar slot 1 ("Rnd").
void ThingManager::RenderThings(ThingRenderData *data) {
    static const char *name = "Rnd"; // timebar slot name @0x5734df

    if (this->count <= 0) {
        return;
    }
    for (i32 i = 0; i < this->count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL || (thing->flags & THING_FLAG_SKIP_RENDER)) {
            continue;
        }
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotBegin(this->timebar, 1, name);
        }
        thing->Render(data);
        thing = this->things[i];
        if (thing->profiling_0xc != NULL) {
            _NuTimeBarSlotEnd(this->timebar, 1);
        }
    }
}

void ThingManager::ResetThings(ThingResetData *data) {
    const char *name = "Res";

    i32 i = 0;
    if (this->count > 0) {
        do {
            BaseThing *thing = this->things[i];
            if (thing != NULL && (thing->flags & 8) == 0) {
                if (thing->profiling_0xc != NULL) {
                    _NuTimeBarSlotBegin(this->timebar, 4, name);
                }
                thing = this->things[i];
                thing->Reset(data);
                thing = this->things[i];
                if (thing->profiling_0xc != NULL) {
                    _NuTimeBarSlotEnd(this->timebar, 4);
                }
            }
            ++i;
        } while (i < this->count);
    }
}

// ThingManager::ThingManager @0x425870. Stores the manager in theThingManager
// and carves the thing-pointer array from the
// theMemoryManager linear pool. On allocation failure the array is NULL — the
// manager then simply never accepts things (AddThing's count < max check).
ThingManager::ThingManager(i32 max_things) {
    const usize need = static_cast<usize>(max_things) * sizeof(*things);

    BaseThing **array = NULL;
    if (*theMemoryManager.end_cell - *theMemoryManager.cursor_cell > need) {
        const usize aligned = ALIGN(*theMemoryManager.cursor_cell, 0x10);
        *theMemoryManager.cursor_cell = aligned + need;
        array = reinterpret_cast<BaseThing **>(aligned);
        memset(array, 0, need);
        theMemoryManager.allocated += need;
        theMemoryManager.remaining -= need;
        theMemoryManager.high_water = *theMemoryManager.cursor_cell;
    }
    this->things = array;
    this->max_things = max_things;
    // Profiling sets are a deferred subsystem; the handle is only ever passed
    // to the NuTimeBarSlotBegin/End stubs, so NULL behaves like the original
    // with profiling disabled.
    this->timebar = NuTimeBarCreateSet(0);
    theThingManager = this;
}

ThingManager::~ThingManager() {
}

void ThingManager::cbEdTimingSelect(eduimenu_s *, eduiitem_s *, u32) {
}

void ThingManager::cbEdTrackCancel(eduimenu_s *, eduimenu_s *) {
}

void ThingManager::edTimingEnter() {
}

void ThingManager::edTimingInit() {
}

void ThingManager::edTimingProc(float, nupad_s *) {
}

void ThingManager::edTimingRender() {
}

void SpecialObject::Exists() const {
}

void SpecialObject::GetCollision() const {
}

void SpecialObject::GetCurrentPosition() const {
}

void SpecialObject::GetCurrentTransform() const {
}

void SpecialObject::GetInitialPosition() const {
}

void SpecialObject::GetInitialTransform() const {
}

void SpecialObject::GetMtl(i32) const {
}

void SpecialObject::GetName() const {
}

void SpecialObject::GetNumMtls() const {
}

void SpecialObject::GetRadius() const {
}

void SpecialObject::GetVisibility() const {
}

void SpecialObject::Render(VuMtx const *) const {
}

void SpecialObject::SetCollision(i32) {
}

void SpecialObject::SetCurrentPosition(VuVec const *) {
}

void SpecialObject::SetCurrentTransform(VuMtx const *) {
}

void SpecialObject::SetInitialPosition(VuVec const *) {
}

void SpecialObject::SetInitialTransform(VuMtx const *) {
}

void SpecialObject::SetVisibility(i32) {
}

SpecialObject::SpecialObject() {
}

void GameThingManager::AddLevelOnlyThings() {
}

// GameThingManager::AddOnceOnlyThings @0x4e8bb0: registers the MechSystems
// singleton as the manager's once-only thing (via the virtual AddThing slot).
void GameThingManager::AddOnceOnlyThings() {
    this->AddThing(MechSystems::Get());
}

// GameThingManager::GameThingManager @0x4e8b00: stores the object in
// theGameThings (the vptr switch to the derived vtable is compiler-generated).
GameThingManager::GameThingManager(i32 max_things) : ThingManager(max_things) {
    theGameThings = this;
}

// GameThingManager D1 dtor @0x4e8a80 clears the global before destruction.
GameThingManager::~GameThingManager() {
    theGameThings = NULL;
}

CantPickupBombTimerAddon::CantPickupBombTimerAddon(MechObjectInterface &, float) {
}

void CantPickupBombTimerAddon::OnProcess(MechAddon::ProcessStage, float) {
}

CantPickupBombTimerAddon::~CantPickupBombTimerAddon() {
}

// BaseThing::BaseThing @0x425840 zeroes the data fields after the vptr.
BaseThing::BaseThing() {
    this->field_0x4 = 0;
    this->flags = 0;
    this->profiling_0xc = NULL;
}

// BaseThing defaults @0x424bf0 (dtor) and 0x425990..0x425a20 (interface
// defaults); RemoveDependancies returns 1, the rest are no-ops. GetName holds
// a 0 slot in the original base vtable (pure) — see basething.h.
BaseThing::~BaseThing() {
}

i32 BaseThing::RemoveDependancies(ThingRemoveData *) {
    return 1;
}

void BaseThing::EnterLevel(ThingLevelData *) {
}

void BaseThing::ExitLevel(ThingLevelData *) {
}

void BaseThing::Reset(ThingResetData *) {
}

void BaseThing::Process(ThingProcessData *) {
}

void BaseThing::ProcessEvenWhenPaused(ThingProcessData *) {
}

void BaseThing::ProcessOnlyWhenPaused(ThingProcessData *) {
}

void BaseThing::Render(ThingRenderData *) {
}

void BaseThing::Display(ThingRenderData *) {
}

void BaseThing::Effects(ThingRenderData *) {
}

static __used__ void LEGO_100PercentFn() {
}
static __used__ void LEGO_AllGoldBricksFn() {
}

i32 NoLayerKill(GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((data->flags_094[2] & 0x80) != 0)
        return object->apiobj.field_0x27f == 6;
    return 0;
}

void GetUsageMask(NuShaderUsageMask_s *) {
}

void TakeOverCode(GameObject_s *, i32) {
}

void InitExtraList() {
    for (i32 i = 0; i < 44; ++i) {
        NuStrCpy(ExtraItems[i].special_name, Cheat[i].extra_name);
        NuSpecialFind(WORLD->current_gscn, &ExtraItems[i].special, ExtraItems[i].special_name, 1);
        ExtraItems[i].type = 2;
        ExtraItems[i].unlocked = 0;
        ExtraItems[i].item_id = i;

        u32 unlocked = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7bf0)[i >> 5];
        if (((static_cast<u64>(unlocked) >> (i & 0x1f)) & 1) != 0) {
            ExtraItems[i].unlocked = 1;
            reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7c00)[i >> 5] |= 1u << (i & 0x1f);
        }
        ExtraItems[i].price = Cheat[i].extra_price;
    }

    SHOPEXTRACOUNT = 44;
    for (i32 i = 0; i < SHOPEXTRACOUNT; ++i) {
        char name[32];
        NuStrCpy(name, ExtraItems[i].special_name);
        NuStrCat(name, "b");
        NuSpecialFind(WORLD->current_gscn, &extrasils[i], name, 1);
    }
}

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only) {
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        if ((object->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
            continue;
        }
        if (vehicle_only != 0 && (object->apiobj.field_0x1f8 & 0x1000) == 0) {
            continue;
        }
        if (required_flags != 0 && (object->apiobj.field_0x1f4 & required_flags) != required_flags) {
            continue;
        }
        if (character_id != -1 && object->id != character_id) {
            continue;
        }
        if (alive_only != 0 && object->apiobj.field_0x287 != 0) {
            continue;
        }
        if (non_level_only != 0 && object->field_0x107c != -1) {
            continue;
        }
        return object;
    }
    return NULL;
}

void KillGameObject(GameObject_s *object, i32 reason, i32) {
    if (object == NULL || (object->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0) {
        return;
    }

    const i32 requested_reason = reason;
    if (reason == 5) {
        reason = 4;
    }

    object->KillTasks();
    object->current_hp = 0;
    object->apiobj.velocity.x = 0.0f;
    object->apiobj.velocity.z = 0.0f;

    // The shipped function converts the ordinary scripted kill (reason 4)
    // into the terminal death state 2 after its effects have been emitted.
    const bool terminal_kill = reason == 4;
    if (terminal_kill) {
        reason = 2;
    }
    object->apiobj.field_0x287 = static_cast<u8>(reason == 3 ? 2 : reason);
    if (object->apiobj.field_0x287 == 2) {
        object->movement_lean_angle = 0;
        object->field_0x1018 = 0.0f;
    } else {
        object->apiobj.field_0x287 = 1;
        object->apiobj.start_position = object->apiobj.position;
        object->field_0x1018 = 0.5f;
    }

    object->ai.opponent = NULL;
    object->ai.nearest_opponent = NULL;
    object->ai.dont_avoid_character = NULL;
    object->last_attacker = NULL;
    object->force_target = NULL;
    object->airborne_collision_target = NULL;
    object->field_0xecc = 0;
    object->field_0xed0 = 0;

    if (!terminal_kill) {
        AISCRIPTPROCESS *processor = reinterpret_cast<AISCRIPTPROCESS *>(&object->ai);
        if (AIScriptSetBaseScriptStateByName(processor, const_cast<char *>("BeenKilled")) != 0 && WORLD != NULL &&
            WORLD->ai_sys != NULL) {
            AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, processor, FRAMETIME);
        }
    }

    if (terminal_kill && object->apiobj.field_0x27c == -1) {
        const u8 respawn_flags = object->field_0xefa >> 4;
        if (requested_reason == 5 || (respawn_flags & 1) == 0) {
            object->ai.reset_mode = 4;
        } else if ((respawn_flags & 2) != 0) {
            object->field_0x101c = 1.0f;
        } else {
            object->ai.reset_mode = 1;
            object->ai_spawn_delay = 1.0f;
        }
    }
}

void PowerUp_Update(GameObject_s *) {
}

void TakeOver2GetIn(GameObject_s *, GameObject_s *) {
}

void PowerUp_AddPart(nuvec_s *, nuvec_s *, float, float) {
}

void ScaleGameObject(GameObject_s *object) {
    const f32 scale = object->apiobj.field_0xa8;
    CHARACTERDATA *character = object->apiobj.character_data;
    object->apiobj.scaled_radius = character->field13_0x2c * scale;
    object->apiobj.collision_radius = object->field_0x1008 * scale;
    object->apiobj.collision_height = object->apiobj.collision_radius * object->collision_y_scale;
    object->apiobj.scaled_height = (character->field16_0x38 - character->field15_0x34) * object->field_0x1004;
}

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32) {
    if (obj == NULL) {
        return;
    }

    obj->KillTasks();
    obj->ClearAddons();
    obj->ClearMechObjectInterface();

    const u32 low_mask = ~obj->apiobj.field_0x1e4;
    const u32 high_mask = ~obj->apiobj.field_0x1e8;
    const u8 index = obj->apiobj.field_0x289;
    const u32 index_low_mask = index < 32 ? ~(1u << index) : ~0u;
    const u32 index_high_mask = index < 32 ? ~0u : ~(1u << (index - 32));
    for (i32 i = 0; i < HIGHGAMEOBJECT; i++) {
        Obj[i].apiobj.field_0x1ec &= low_mask;
        Obj[i].apiobj.field_0x1f0 &= high_mask;
        Obj[i].apiobj.field387_0x2a0 &= index_low_mask;
        Obj[i].apiobj.field388_0x2a4 &= index_high_mask;
        Obj[i].field_0xebc &= index_low_mask;
        Obj[i].field_0xec0 &= index_high_mask;
        Obj[i].field_0xec4 &= index_low_mask;
        Obj[i].field_0xec8 &= index_high_mask;
    }

    if (obj->pad_gamepad != NULL) {
        obj->pad_gamepad->allocated_5a &= ~1u;
    }
    DestroySnakeBody(obj);
    APIObjectDestroy(WORLD->api_object_sys, &obj->apiobj);

    HIGHGAMEOBJECT = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.field_0x1f8 & 1) != 0) {
            HIGHGAMEOBJECT = i + 1;
        }
    }
    for (i32 i = 0; i < 8; i++) {
        if (Player[i] == obj) {
            Player[i] = NULL;
        }
    }
}

void TargetGameObject(GameObject_s *, nuvec_s *, nuvec_s *, float, float, u32, i32, i32, i32) {
}

void ManageGameObjects() {
}

void PowerUp_GetPanelY(i32) {
}

void PowerUp_Particles(WORLDINFO_s *, nuvec_s *) {
}

void UpdateGameObjects(WORLDINFO_s *world) {
    if (world == NULL || Obj == NULL) {
        return;
    }

    SetPlayer();

    // AI updates are scheduled before the object/player movement passes. The
    // elapsed value is accumulated until an object becomes eligible for its
    // next script and path update.
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags) {
            continue;
        }

        if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            object->ai_elapsed_time = 0.0f;
        }
        object->ai_elapsed_time += FRAMETIME;

        const bool force_update =
            timebase_updates == 0 || (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0 ||
            (object->field_0xf00 & GAME_OBJECT_AI_UPDATE_SPECIAL_STATE) != 0 ||
            object->apiobj.supporting_platform_id != -1 || object->apiobj.field_0x27d == 0 ||
            (object->field_0xcc0 != NULL && object->character_context == CHARACTER_CONTEXT_LINKED_OBJECT) ||
            (object->ai.group != NULL && object->ai.group->is_in_formation);
        const i32 interval = force_update ? 1 : GameObjectAIUpdateInterval(world, object);

        if (interval <= 1) {
            object->field_0xf00 |= GAME_OBJECT_AI_UPDATE_FORCED | GAME_OBJECT_AI_UPDATE_PROCESS;
        } else {
            object->field_0xf00 &= ~GAME_OBJECT_AI_UPDATE_FORCED;
            const u32 update_phase =
                static_cast<u32>(object->apiobj.field_0x289) + static_cast<u32>(GameTimer.update_count);
            if (update_phase % static_cast<u32>(interval) == 0) {
                object->field_0xf00 |= GAME_OBJECT_AI_UPDATE_PROCESS;
            } else {
                object->field_0xf00 &= ~GAME_OBJECT_AI_UPDATE_PROCESS;
            }
        }

        // AI positions between scheduled updates are render extrapolations.
        // The target restores the last terrain-resolved position here before
        // GameAIProcess and the movement/terrain passes (0x140c2..0x140f0),
        // so an extrapolated floor offset never becomes the next collision
        // query's starting point.
        const u32 authoritative_position_flags =
            APIOBJECT_MOTION_FLAG_AI_CONTROLLED | APIOBJECT_STATE_FLAG_IGNORE_DOORS;
        if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0 &&
            (object->apiobj.field_0x1f4 & authoritative_position_flags) == APIOBJECT_MOTION_FLAG_AI_CONTROLLED) {
            object->apiobj.position.x = object->field_0x10c8;
            object->apiobj.position.y = object->field_0x10cc;
            object->apiobj.position.z = object->field_0x10d0;
        }
    }

    GameAIProcess();

    // The original has separate object and player passes. Ordinary hub
    // players reach this player pass with no movement override or spline.
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        const u16 player_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
        if (object == NULL || (object->apiobj.field_0x1f8 & player_flags) != player_flags) {
            continue;
        }
        if (object->move_override != NULL) {
            object->move_override(object);
        } else {
            Player_ToggleCharacter(object, 0, 1);
            MovePlayer(object);
        }
    }

    // AI movement controllers run only on their scheduled AI frame and use
    // the full interval accumulated since the previous scheduled update.
    // Between scheduled updates the terrain/animation pass below advances the
    // last resolved velocity; calling MovePlayer here every frame would both
    // reselect path targets and integrate that velocity twice.
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags ||
            (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0 ||
            (object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) == 0) {
            continue;
        }

        bool is_local_player = false;
        for (i32 player_index = 0; player_index < 8; ++player_index) {
            if (Player[player_index] == object) {
                is_local_player = true;
                break;
            }
        }
        if (is_local_player) {
            continue;
        }

        const f32 frame_time = FRAMETIME;
        FRAMETIME = object->ai_elapsed_time;
        if (object->move_override != NULL) {
            object->move_override(object);
        } else if (object->movement_spline == NULL) {
            MovePlayer(object);
        } else {
            MovePlayerSpline(object);
        }
        FRAMETIME = frame_time;
    }

    // AI-controlled characters use their accumulated AI interval for a full
    // terrain update when their script was processed. Between those updates,
    // the original advances the last resolved velocity and still animates the
    // object every frame.
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags ||
            (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            continue;
        }

        bool is_local_player = false;
        for (i32 player_index = 0; player_index < 8; ++player_index) {
            if (Player[player_index] == object) {
                is_local_player = true;
                break;
            }
        }
        if (is_local_player) {
            // Player-owned objects have a dedicated terrain/animation pass
            // below. Processing them here as AI as well applies terrain state
            // twice and can discard the movement produced by MovePlayer.
            continue;
        }

        if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            const f32 frame_time = FRAMETIME;
            FRAMETIME = object->ai_elapsed_time;
            TerrainPlayer(object);

            object->field_0x10c8 = object->apiobj.position.x;
            object->field_0x10cc = object->apiobj.position.y;
            object->field_0x10d0 = object->apiobj.position.z;

            const f32 vertical_displacement = object->apiobj.position.y - object->apiobj.start_position.y;
            if (vertical_displacement == 0.0f || object->ai_elapsed_time == 0.0f) {
                object->vertical_velocity = 0.0f;
            } else {
                object->vertical_velocity = vertical_displacement / object->ai_elapsed_time;
            }
            FRAMETIME = frame_time;
        } else {
            PreResetCode(object);
            PostResetCode(object);
            if ((object->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) != 0) {
                GameObjectOrigin(object);
            } else {
                object->apiobj.position.x += object->apiobj.velocity.x * FRAMETIME;
                object->apiobj.position.y += object->vertical_velocity * FRAMETIME;
                object->apiobj.position.z += object->apiobj.velocity.z * FRAMETIME;
            }
        }

        AnimatePlayer(object);
        object->context_target_position = NULL;
        ScaleGameObject(object);
        GameObjectDimensions(object);
        if (object->use_model_origin != 0xff) {
            ++object->use_model_origin;
        }
    }

    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        const u16 player_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
        if (object == NULL || (object->apiobj.field_0x1f8 & player_flags) != player_flags) {
            continue;
        }

        TerrainPlayer(object);
        KeepOnScreen(object);
        Doors_Check(world, object);
        object->field_0x10c8 = object->apiobj.position.x;
        object->field_0x10cc = object->apiobj.position.y;
        object->field_0x10d0 = object->apiobj.position.z;
        AnimatePlayer(object);
        object->context_target_position = NULL;
        ScaleGameObject(object);
        GameObjectDimensions(object);
        UpdateCoinPacket(object->coinpacket, static_cast<u32>(object->apiobj.flags_low) >> 7,
                         object->apiobj.field_0x27c);
        if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 && object->apiobj.field_0x287 == 0 &&
            object->field_0x7a5 != 0x2b && !(object->field_0x7a5 == 0x0f && object->field_0x7a3 == 1)) {
            GizmoPickups_Collide(world, object, 1);
        } else {
            ResetCoinPacket(object->coinpacket);
        }
        if (object->use_model_origin != 0xff) {
            ++object->use_model_origin;
        }
    }

    // Character lighting is refreshed only for models which were visible in
    // the preceding render pass. The original amortises this pass across the
    // object array; the complete active set is small enough to update here.
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) == character_flags &&
            object->apiobj.model_draw_result != 0) {
            LightGameObject(object, world->rtl_set);
        }
    }

    CollideGameObjects(world);

    if (do_player_tag == 0) {
        bool pending_tag_valid = false;
        if (player_tag_timer > 0.0f) {
            player_tag_timer -= FRAMETIME;
            const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
            pending_tag_valid = player_tag_timer >= 0.0f && player_tag_to != NULL && player_tag_from != NULL &&
                                (player_tag_to->apiobj.field_0x1f8 & required_flags) == required_flags &&
                                player_tag_to->apiobj.field_0x287 == 0 &&
                                (player_tag_to->tag_context_flags & 2) == 0 &&
                                (player_tag_from->apiobj.field_0x1f8 & required_flags) == required_flags &&
                                player_tag_from->apiobj.field_0x287 == 0 &&
                                (player_tag_from->tag_context_flags & 2) == 0;
        }
        if (!pending_tag_valid) {
            player_tag_to = NULL;
            player_tag_timer = 0.0f;
            player_tag_from = NULL;
            do_player_tag = 0;
        }
    } else {
        const u16 required_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
        bool clear_pending_tag = true;
        if (player_tag_to != NULL && player_tag_from != NULL && player_tag_to != player_tag_from &&
            (player_tag_to->apiobj.field_0x1f8 & required_flags) == required_flags &&
            player_tag_to->apiobj.field_0x287 == 0 && (player_tag_to->tag_context_flags & 2) == 0 &&
            (player_tag_from->apiobj.field_0x1f8 & required_flags) == required_flags &&
            player_tag_from->apiobj.field_0x287 == 0 && (player_tag_from->tag_context_flags & 2) == 0) {
            const i32 tag_result = TagCode(player_tag_to, player_tag_from, 0, 0, 1);
            if (tag_result == 1) {
                GameAudio_PlaySfx(0x22, &player_tag_to->apiobj.collision_position, 0, 0);
                GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                const i32 player_1_id = Player[1] == NULL ? -1 : Player[1]->id;
                const i32 player_0_id = Player[0] == NULL ? -1 : Player[0]->id;
                RememberPlayerIDs(0, player_0_id, player_1_id);
                player_tag_to->tag_state = 2.0f;
                player_tag_from->tag_state = 2.0f;
            } else if (tag_result == 2) {
                clear_pending_tag = false;
            }
        }
        if (clear_pending_tag) {
            player_tag_to = NULL;
            player_tag_timer = 0.0f;
            player_tag_from = NULL;
            do_player_tag = 0;
        }
    }
}

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set) {
    const bool has_no_spline = spline == NULL;
    if (position == NULL && spline == NULL) {
        return NULL;
    }

    if (NOAICREATURES != 0 && model != id_DRAGBOMB && (GCDataList[model].flags_090 & 0x40) == 0) {
        return NULL;
    }

    u32 arcade_mode = 0;
    Arcade_GetMode(&arcade_mode);
    if ((arcade_mode & 0x10) != 0) {
        return NULL;
    }

    if (static_cast<u32>(model) >= 0x154 || apicharsys->playermodelids[model] == -1) {
        return NULL;
    }

    GameObject_s *object = AddCreature(model, 0);
    if (object == NULL) {
        return NULL;
    }

    object->apiobj.field_0x1f4 |= APIOBJECT_MOTION_FLAG_AI_CONTROLLED;
    const u32 model_flags = apicharsys->char_data[model].model_flags;
    if ((model_flags & 0x200) != 0) {
        object->apiobj.field_0x1f4 |= 0x404;
    } else if ((model_flags & 0x400) != 0) {
        object->apiobj.field_0x1f4 |= 0x401;
    }
    object->field_0x1050 |= (model_flags & 0x1000) != 0 ? 5 : 1;

    GAMECHARACTERDATA &game_character = GCDataList[model];
    object->apiobj.viewdistance = game_character.viewdistance;
    object->apiobj.heardistance = game_character.heardistance;
    object->apiobj.maxviewheight = game_character.maxviewheight;
    object->apiobj.minviewheight = game_character.minviewheight;
    object->field_0xef9 &= static_cast<u8>(~8u);
    object->ai.field_0xe0 = 0x4e6e6b28;
    object->ai.field_0xf0 = 0x4e6e6b28;
    object->field_0xef8 &= static_cast<u8>(~1u);
    object->ai.field_0x1e5 &= static_cast<u8>(~0x50u);
    object->apiobj.field387_0x2a0 = 0;
    object->apiobj.field388_0x2a4 = 0;
    object->field_0xebc = 0;
    object->field_0xec0 = 0;
    object->field_0xecc = 0;
    object->field_0xed0 = 0;
    object->field_0xed8 = 0;
    object->ai.nearest_opponent = NULL;
    object->ai.nearest_opponent_metric = 0.0f;
    object->ai.field_0xdc = 0;
    object->ai.field_0xec = 0;
    object->ai.opponent = NULL;
    object->ai.antinode_timer = 0.0f;
    InitPlayerAI(object);

    if (spline == NULL) {
        object->apiobj.position = *position;
        object->apiobj.facing_angle = static_cast<u16>(angle);
        object->apiobj.movement_facing_angle = static_cast<u16>(angle);
        object->apiobj.field_0x276 = static_cast<u16>(angle);
        if (group != NULL) {
            AddToAIGroup(group, &object->apiobj);
        }
    } else {
        SPLINEPOS_s *spline_position = reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline);
        InitSplinePosition(spline_position, spline, 0.0f, spline_mode);
        SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(spline_position);
        NUVEC spline_point;
        u16 yaw = 0;
        u16 pitch = 0;
        PointAlongSpline(runtime->spline, runtime->normalized_position, &spline_point, &yaw, &pitch, runtime->looping);
        object->apiobj.facing_angle = yaw;
        object->apiobj.movement_facing_angle = yaw;
        object->apiobj.field_0x276 = yaw;
        object->apiobj.pitch_angle = static_cast<u16>(-pitch);

        if (spline_offset != NULL) {
            NUVEC *stored_offset = reinterpret_cast<NUVEC *>(reinterpret_cast<u8 *>(spline_position) + 0x20);
            *stored_offset = *spline_offset;
            if (spline_offset->x != 0.0f || spline_offset->y != 0.0f || spline_offset->z != 0.0f) {
                NUVEC rotated;
                NuVecRotateX(&rotated, spline_offset, static_cast<u16>(-pitch));
                NuVecRotateY(&rotated, &rotated, yaw);
                NuVecAdd(&spline_point, &spline_point, &rotated);
            }
        }
        object->apiobj.position = spline_point;
    }

    ResetPlayerMoves(object);
    object->apiobj.pos_x = object->apiobj.position.x;
    object->apiobj.pos_y = object->apiobj.position.y;
    object->apiobj.pos_z = object->apiobj.position.z;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.initial_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.velocity = v000;

    GetTopBot(object);
    GameObjectDimensions(object);
    ResetRumble(&object->pad_gamepad->rumble_packet);
    ResetLights(&object->apiobj.position, &object->light_data, WORLD->rtl_set);

    object->sock_location_flags = 0;
    object->field_0x661 = 0xff;
    object->sock_segment = -1;
    object->facing_direction.x = NU_SIN_LUT(angle);
    object->facing_direction.y = 0.0f;
    object->facing_direction.z = NU_COS_LUT(angle);
    object->apiobj.model_draw_result = 1;
    object->use_model_origin = 0;
    object->apiobj.field_0x288 = 0;
    object->field_0xefe &= ~4u;

    if (has_no_spline) {
        InitSurfaceInfo(object);
        if (set_on_surface != 0) {
            SetObjOnSurface(object, 0);
        }
    } else {
        PortalGameObject(object, 1, 1, -1, WORLD->current_gscn);
    }

    object->apiobj.field_0x1f4 &= ~0x100u;
    object->apiobj.field_0x1f8 &= static_cast<u16>(~4u);
    object->field_0x1004 = 1.0f;
    object->apiobj.field_0x287 = 0;
    object->field_0x7a5 = 0xff;
    object->apiobj.field_0x285 = 0;
    memset(&object->ai.path_info, 0, sizeof(object->ai.path_info));
    object->ai.field_0x124 = -1;
    object->ai.field_0x138 = 0xff;
    object->ai.field_0x139 = 0;
    if (path_info != NULL) {
        AISysCharacterSetPath(&object->ai, path_info->path);
        AISysCharacterSetPathCnx(&object->ai, &object->apiobj.position, path_info->connection, path_info->direction);
    }
    if (object->ai.movement_target == NULL) {
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff,
                                 static_cast<i8>(object->apiobj.field_0x27d));
    }
    AIScriptProcessorInit(WORLD->ai_sys, &object->ai, reinterpret_cast<AISCRIPTPROCESS *>(&object->ai), NULL,
                          script_name, NULL, 1, NULL, NULL);
    object->apiobj.field_0x214 = 2000000.0f;
    PreResetCode(object);
    PostResetCode(object);
    GameObjectOrigin(object);
    object->apiobj.previous_position[0] = object->apiobj.position.x;
    object->apiobj.previous_position[1] = object->apiobj.position.y;
    object->apiobj.previous_position[2] = object->apiobj.position.z;
    object->field_0x10c8 = object->apiobj.position.x;
    object->field_0x10cc = object->apiobj.position.y;
    object->field_0x10d0 = object->apiobj.position.z;
    if (static_cast<u32>(creature_set - 1) < 16) {
        object->ai.creature_set = static_cast<u8>(creature_set);
        ++aicreature_sets_alive[creature_set - 1];
    }
    return object;
}

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name) {
    if (GetNamedAPIObjectFn != NULL) {
        APIOBJECT *object = GetNamedAPIObjectFn(system, name);
        if (object != NULL) {
            return object->objptr;
        }
    }
    return NULL;
}

void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32) {
}

void TakeOverGameObject2(GameObject_s *, GameObject_s *, i32) {
}

void DeactivateGameObject(GameObject_s *object) {
    if (object == NULL) {
        return;
    }

    if (object->field_0xcc0 != NULL) {
        if ((object->apiobj.flags_high & 0x40) == 0) {
            KillGameObject(object->field_0xcc0, 4, 0);
            object->apiobj.flags_high &= static_cast<u8>(~0x10u);
        } else {
            ReleaseTakeOver(object, 1);
        }
    }
    object->apiobj.flags_high &= static_cast<u8>(~0x10u);

    if (object->ai.field_0x134 != 0xff &&
        AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai),
                                         const_cast<char *>("InActive")) != 0) {
        object->ai.reset_mode = 0;
        if (WORLD != NULL && WORLD->ai_sys != NULL && object->ai.field_0x134 < WORLD->ai_sys->creature_count) {
            WORLD->ai_sys->creatures[object->ai.field_0x134].activate_type = 2;
        }
    } else {
        object->ai.reset_mode = 4;
    }
}

i32 EquivalentObject_Find(WORLDINFO_s *, nuhspecial_s *) {
    return 0;
}

void FindNearestGameObject(nuvec_s *, GameObject_s *, u32, float, float, i32, i32, i32, float *, i32,
                           i32 (*)(GameObject_s *), bool) {
}

void SetAllInstancesHidden(nugscn_s *) {
    memset(PortalVisiFlags, 0, sizeof(PortalVisiFlags));
}

void RemoveAnyChunkControls(i32 *) {
}

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack) {
    if (chunk->previous != NULL) {
        chunk->previous->next = chunk->next;
    } else if (*stack == chunk) {
        *stack = chunk->next;
    }
    if (chunk->next != NULL) {
        chunk->next->previous = chunk->previous;
    }
    chunk->previous = NULL;
    chunk->next = NULL;
}

void RemoveChunkControlFromStack(debris_chunk_control_s *control, debris_chunk_control_s **stack) {
    debris_chunk_control_s *current = *stack;
    while (current != NULL && current != control) {
        stack = &current->next;
        current = current->next;
    }
    if (current == control) {
        *stack = control->next;
    }
    control->next = NULL;
}

extern "C" debkeydatatype_s *debris_keystack;

void RemoveDebrisEffectFromStack(debkeydatatype_s *key) {
    if (key->next == NULL) {
        debris_keystack = key->previous;
        if (debris_keystack != NULL) {
            debris_keystack->next = NULL;
        }
    } else {
        key->next->previous = key->previous;
        if (key->previous != NULL) {
            key->previous->next = key->next;
        }
    }
    key->next = NULL;
    key->previous = NULL;
}

void ReStoreStatusTakeOverObjectSys(i32) {
}

extern "C" {

    i32 InModelList(APICHARACTERMODELLIST_s *list, i32 id, i32 *out_index) {
        if (list != NULL) {
            i32 i = 0;
            for (; list->model_id != -1; list++, i++) {
                if (list->model_id == id) {
                    if (out_index != NULL)
                        *out_index = i;
                    return 1;
                }
            }
        }
        if (out_index != NULL)
            *out_index = -1;
        return 0;
    }

} // extern "C"
