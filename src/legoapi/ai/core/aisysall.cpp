// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include <string.h>

#include <float.h>
#include <math.h>

#include "editor/edpath.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/ai/core/ai_pathgeometry.h"
#include "batman.h"
#include <stdio.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nurdp.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/ai/core/aisysall_types.h"
#include "legoapi/ai/core/ai_sys_stubs_types.h"
#include "gameapi/edtools/edtoolsall.h"
#include "legoapi/ai/core/aisysall.h"
#include "legoapi/ai/core/gameai.h"
#include "nu2api/nu3d/nu3d_includes.h"

static i32 iterator_count;
static f32 ***distance_tables;

static __used__ f32 **pathEditorCalculateDistanceTable(AIPATH_s *path, i32 route_mask, variptr_u *cursor,
                                                       variptr_u *end);

#if defined(__i386__)
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AIPATHBUILD_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif
// Retail keeps only the IPA-SRA clone, without the unused path index.
static AIPATHBUILD_CLONE_ATTR2 void pathEditorCreateSpecialRouteData(AIPATH_s *path, EDAIPATH_s *editor_path,
                                                                     VARIPTR *cursor, VARIPTR *end)
    __asm__("_ZL32pathEditorCreateSpecialRouteDataP8AIPATH_siP10EDAIPATH_sP9variptr_uS4_.isra.81");

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

#if defined(__i386__)
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define AISYSALL_CLONE_ATTR1 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2 __attribute__((noinline, noclone))
#define AISYSALL_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// IPA-SRA keeps only the fields this helper reads: values for those read before any call,
// addresses for those read after one.
static AISYSALL_CLONE_ATTR2_SSE u32 CalculateRightIntersection(f32 object_radius, f32 first_distance,
                                                               f32 *second_distance, i32 first_angle,
                                                               i32 second_angle, AIPATHNODE *before, f32 *shared_y,
                                                               f32 shared_radius, AIPATHNODE *after, NUVEC *result)
    __asm__("_ZL26CalculateRightIntersectionP11APIOBJECT_sP11AIPATHCNX_sS2_iiP12AIPATHNODE_sS4_S4_P7nuvec_s.isra.50");

static __attribute__((noinline, noclone)) u32 CalculateIntersection(AISYS *system, AIPACKET *packet, APIOBJECT *object,
                                                                    AIPATHCNX *current_connection,
                                                                    AIPATHCNX *target_connection);

static AISYSALL_CLONE_ATTR2 void AIMoveFindDivertNode(AIPATH *path, AIPACKET *packet, NUVEC *destination)
    __asm__("_ZL20AIMoveFindDivertNodeP7AISYS_sP8AIPATH_sP10AIPACKET_sP7nuvec_s.isra.26");

static AISYSALL_CLONE_ATTR1 i32 AIMoveAdjustDestinationPath(AIPACKET *packet)
    __asm__("_ZL27AIMoveAdjustDestinationPathP7AISYS_sP10AIPACKET_s.isra.48");

// IPA-SRA hands this clone the address of system->path_sys rather than the system.
static AISYSALL_CLONE_ATTR2 i32 AIMoveChooseExitNodePath(AIPATHSYS **path_sys, AIPACKET *packet)
    __asm__("_ZL24AIMoveChooseExitNodePathP7AISYS_sP10AIPACKET_s.isra.82");

void AddGameMsgCount(NUVEC *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);

extern void KillParts(GameObject_s *object, i32 part, i32 joint, i32 visible, f32 velocity, i32 flags, u16 *part_ids);
extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);
extern "C" ADDPART_s Default_ADDPART;
extern f32 ForceThrowSpeed, ForceThrowGravity;
extern "C" PART_s *AddPart(ADDPART_s *);

void PartCollide_3D(PART_s *);

void MakeThrowVector(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32);

static void ActionCopyParam(char *destination, i32 capacity, const char *source) {
    i32 index = 0;
    while (index + 1 < capacity && source[index] != '\0') {
        destination[index] = source[index];
        ++index;
    }
    destination[index] = '\0';
}

static GAMECHARACTERDATA *ActionGameCharacterData(GameObject_s *object) {
    if (object == NULL || object->apiobj.character_data == NULL) {
        return NULL;
    }
    return static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
}

static char *ActionSubstringValue(char *parameter, const char *name) {
    char *value = NuStrIStr(parameter, const_cast<char *>(name));
    if (value == NULL) {
        return NULL;
    }
    value += NuStrLen(name);
    if (*value == '=' || *value == ' ') {
        ++value;
    }
    return value;
}

static GameObject_s *ActionPacketOpponent(AIPACKET *packet) {
    APIOBJECT *opponent = packet != NULL ? static_cast<APIOBJECT *>(packet->opponent) : NULL;
    return opponent != NULL ? opponent->objptr : NULL;
}

static bool ActionValidOpponent(GameObject_s *opponent) {
    return opponent != NULL &&
           (opponent->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
               (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
           opponent->apiobj.field_0x287 == 0 && opponent->character_context != CHARACTER_CONTEXT_DOOMED &&
           !(opponent->apiobj.player_controlled && opponent->spawn_protection_timer > 0.0f);
}

static i32 Action_Idle(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                       i32 is_first_time, f32 elapsed);

__used__ static i32 Action_DontPush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || param_5 == 0) {
        return 1;
    }

    packet->owner->apiobj.flags_low |= 2;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "false") == 0) {
            packet->owner->apiobj.flags_low &= static_cast<u8>(~2u);
        }
    }
    return 1;
}

__used__ static i32 Action_GoToNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32 elapsed);

__used__ static i32 Action_SetParam(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32, f32);

__used__ static i32 Action_BlockPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_NoShadows(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_NoTerrain(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_FacePlayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed);

__used__ static i32 Action_FollowPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 is_first_time, f32 elapsed);

__used__ static i32 Action_GoToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed);

__used__ static i32 Action_NoLosCheck(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6);

static i32 Action_ResetTimer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                             i32 is_first_time, f32);

__used__ static i32 Action_SetLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL || param_5 == 0) {
        return 1;
    }

    processor->locator = NULL;
    if (param_4 == 0) {
        return 1;
    }

    char *locator_name = NULL;
    i32 personal = 0;
    i32 indexed = 0;
    i32 random_count = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value;
        if ((value = NuStrIStr(params[index], "name")) != NULL) {
            locator_name = value + 5;
        } else if (NuStrIStr(params[index], "personal") != NULL) {
            personal = 1;
        } else if (NuStrIStr(params[index], "indexed") != NULL) {
            indexed = 1;
        } else if ((value = NuStrIStr(params[index], "random")) != NULL) {
            random_count = static_cast<i32>(
                AIParamToFloatEx(packet, reinterpret_cast<AISCRIPTPROCESS *>(packet), value + 7));
        }
    }
    if (locator_name == NULL) {
        return 1;
    }

    char resolved_name[64];
    if (indexed && static_cast<i8>(packet->owner->apiobj.field_0x27c) != -1) {
        sprintf(resolved_name, "%s_%d", locator_name, static_cast<i8>(packet->owner->apiobj.field_0x27c));
    } else if (personal && packet->owner->apiobj.character_data != NULL) {
        sprintf(resolved_name, "%s_%s", locator_name, packet->owner->apiobj.character_data->file);
    } else if (random_count != 0) {
        sprintf(resolved_name, "%s_%d", locator_name, NuRand(0) % random_count);
    } else {
        sprintf(resolved_name, locator_name);
    }
    processor->locator = AIPathFindLocator(sys, resolved_name);
    return 1;
}

__used__ static i32 Action_FaceLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32 elapsed);

static i32 Action_FlatTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                              i32 first_time, f32);

__used__ static i32 Action_GoToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 is_first_time, f32 elapsed);

__used__ static i32 Action_RequiresLOS(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_CanSeeBehind(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_FaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_count, i32 first_time, f32 elapsed);

__used__ static i32 Action_SetInterrupt(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_count, i32 is_first_time, f32);

__used__ static i32 Action_ResetToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_ReturnToState(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **, i32, i32, f32);

__used__ static i32 Action_SetMoveRadius(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6);

static i32 Action_ShadowTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                i32 first_time, f32);

__used__ static i32 Action_ClearInterrupt(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 param_count, i32 is_first_time, f32);

__used__ static i32 Action_FollowOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32);

__used__ static i32 Action_GoToNodeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 elapsed);

void PartKill_ForceThrow(PART_s *, i32);

__used__ static i32 Action_SetHearDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_SetViewDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_CheckWallSplines(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);

static i32 Action_MoveAwayFromNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                   i32 param_count, i32 first_time, f32);

__used__ static i32 Action_SetMaxViewHeight(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_SetMinViewHeight(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6);

__used__ static i32 Action_SetReturnToState(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                            i32 param_count, i32 is_first_time, f32);

static f32 Condition_IAm(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *arg, void *void_arg);

__used__ static f32 Condition_Param(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *);

static f32 Condition_Timer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *);
static f32 Condition_OnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_Random(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *);
extern TERRSET *CurTerr;
static f32 Condition_StuckTime(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_GotLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
extern "C" i32 party_under_cover;
static f32 Condition_GotOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_InLevelNode(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *void_arg);

__used__ static f32 Condition_InterruptID(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *);

static f32 Condition_OriginRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_PathBlocked(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_PlayerRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *);
static f32 Condition_TimeOffPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_LocatorRange(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_PlayerOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static void *Condition_IAmInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_AlwaysTrue(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *void_arg);
static void *Condition_AlwaysTrueInit(AISYS *sys, char *arg, AISCRIPT *script);

static i32 Action_RetreatFromNearestOpponent(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32);

static i32 Action_RetreatFromOpponent(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                      i32 param_count, i32 first_time, f32);

static i32 Action_MoveAwayFromPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32);

static i32 Action_MoveAwayFromPlayer2(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                      i32 param_count, i32 first_time, f32);

static i32 Action_SetCircleDirection(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                     i32 first_time, f32);

static i32 Action_MoveAwayFromOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                       i32 param_count, i32 first_time, f32);

static __used__ i32 Action_IgnoreWallSplines(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32);

static i32 Action_DontUseShadowTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                       i32 param_count, i32 first_time, f32);

static i32 Action_SetFullPathSearch(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32 first_time, f32);

static i32 Action_SetRespawnLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                    i32 param_count, i32 first_time, f32);

static __used__ i32 Action_OverrideAnimation(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32);

static i32 Action_PathConnectionObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                         i32 param_count, i32 first_time, f32);

static i32 Action_PathConnectionMaxLength(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                          i32 param_count, i32 first_time, f32);

static i32 Action_SetIgnoreAntinodes(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32);

static __used__ i32 Action_NotifyStateChange(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params,
                                             i32 param_count, i32, f32);

static void *Condition_LocatorRangeInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

static f32 Condition_LocatorRangeXZ(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_LocatorRangeY(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_GotLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);
static f32 Condition_CurrentLocatorIs(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static void *Condition_CurrentLocatorIsInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_InTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                   void *void_arg);

static void *Condition_InTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_NearestPlayerRange(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                        void *void_arg);

static f32 Condition_NearestPlayerXZRange(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                          void *void_arg);

static void *Condition_InLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_PlayerInLevelNode(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg);
static void *Condition_PlayerInLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_EitherPlayerInLevelNode(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg);
static f32 Condition_LevelNodeRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *void_arg);
static void *Condition_LevelNodeRangeInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_GotTriggerArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);

static f32 Condition_PlayerInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                         void *void_arg);

static void *Condition_PlayerInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_Player2InTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                          void *void_arg);

static f32 Condition_EitherPlayerInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *,
                                               void *void_arg);

static f32 Condition_BaddyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *void_arg);
static void *Condition_BaddyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);
static f32 Condition_GoodyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *void_arg);
static void *Condition_GoodyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_OpponentInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                           void *void_arg);

static void *Condition_OpponentInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_OpponentIsAThreat(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    if (packet != NULL && packet->opponent_object != NULL && packet->opponent_is_threat) {
        return 1.0f;
    }
    return 0.0f;
}

static f32 Condition_OpponentOnSamePath(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_OpponentRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);

static f32 Condition_NearestOpponentRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                          void *void_arg);

static f32 Condition_YawToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);
static f32 Condition_OpponentBelow(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static f32 Condition_OpponentToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);
static f32 Condition_PlayerToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_OpponentToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                       void *void_arg);

static void *Condition_OpponentToLocatorInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_OpponentToLocatorXZ(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                         void *void_arg);

static f32 Condition_OpponentToLocatorY(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                        void *void_arg);

static f32 Condition_PlayerToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg);
static void *Condition_PlayerToLocatorInit(AISYS *sys, char *arg, AISCRIPT *);

static f32 Condition_NearestPlayerToLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                            void *void_arg);

static f32 Condition_OpponentOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *);

AIACTIONDEF api_aiactiondefs[] = {
    {"Idle", Action_Idle, 0, 0, 0},
    {"SetState", Action_SetState, 0, 0, 0},
    {"ResetTimer", Action_ResetTimer, 0, 0, 0},
    {"RetreatFromNearestOpponent", Action_RetreatFromNearestOpponent, 0, 0, 0},
    {"RetreatFromOpponent", Action_RetreatFromOpponent, 0, 0, 0},
    {"MoveAwayFromPlayer", Action_MoveAwayFromPlayer, 0, 0, 0},
    {"MoveAwayFromPlayer2", Action_MoveAwayFromPlayer2, 0, 0, 0},
    {"SetCircleDirection", Action_SetCircleDirection, 0, 0, 0},
    {"Circle", Action_Circle, 0, 0, 0},
    {"CircleOpponent", Action_CircleOpponent, 0, 0, 0},
    {"CirclePlayer", Action_CirclePlayer, 0, 0, 0},
    {"FollowPlayer", Action_FollowPlayer, 0, 0, 0},
    {"MoveAwayFromOpponent", Action_MoveAwayFromOpponent, 0, 0, 0},
    {"FollowOpponent", Action_FollowOpponent, 0, 0, 0},
    {"FacePlayer", Action_FacePlayer, 0, 0, 0},
    {"FaceOpponent", Action_FaceOpponent, 0, 0, 0},
    {"FaceLocator", Action_FaceLocator, 0, 0, 0},
    {"IgnoreWallSplines", Action_IgnoreWallSplines, 0, 0, 0},
    {"CheckWallSplines", Action_CheckWallSplines, 0, 0, 0},
    {"NoTerrain", Action_NoTerrain, 0, 0, 0},
    {"FlatTerrain", Action_FlatTerrain, 0, 0, 0},
    {"ShadowTerrain", Action_ShadowTerrain, 0, 0, 0},
    {"DontUseShadowTerrain", Action_DontUseShadowTerrain, 0, 0, 0},
    {"DontPush", Action_DontPush, 0, 0, 0},
    {"CanSeeBehind", Action_CanSeeBehind, 0, 0, 0},
    {"RequiresLOS", Action_RequiresLOS, 0, 0, 0},
    {"SetFullPathSearch", Action_SetFullPathSearch, 0, 0, 0},
    {"SetViewDistance", Action_SetViewDistance, 0, 0, 0},
    {"SetMaxViewHeight", Action_SetMaxViewHeight, 0, 0, 0},
    {"SetMinViewHeight", Action_SetMinViewHeight, 0, 0, 0},
    {"SetHearDistance", Action_SetHearDistance, 0, 0, 0},
    {"SetMoveRadius", Action_SetMoveRadius, 0, 0, 0},
    {"GoToNode", Action_GoToNode, 0, 0, 0},
    {"GoToNodeRandom", Action_GoToNodeRandom, 0, 0, 0},
    {"GoToOrigin", Action_GoToOrigin, 0, 0, 0},
    {"GoToLocator", Action_GoToLocator, 0, 0, 0},
    {"SetLocator", Action_SetLocator, 0, 0, 0},
    {"SetRespawnLocator", Action_SetRespawnLocator, 0, 0, 0},
    {"FollowPath", Action_FollowPath, 0, 0, 0},
    {"MoveAwayFromNode", Action_MoveAwayFromNode, 0, 0, 0},
    {"OverrideAnimation", Action_OverrideAnimation, 0, 0, 0},
    {"BlockPath", Action_BlockPath, 0, 0, 0},
    {"PathConnectionObstacle", Action_PathConnectionObstacle, 0, 0, 0},
    {"PathConnectionMaxLength", Action_PathConnectionMaxLength, 0, 0, 0},
    {"NoLosCheck", Action_NoLosCheck, 0, 0, 0},
    {"ResetToOrigin", Action_ResetToOrigin, 0, 0, 0},
    {"SetInterrupt", Action_SetInterrupt, 0, 0, 0},
    {"ClearInterrupt", Action_ClearInterrupt, 0, 0, 0},
    {"SetIgnoreAntinodes", Action_SetIgnoreAntinodes, 0, 0, 0},
    {"NoShadows", Action_NoShadows, 0, 0, 0},
    {"SetParam", Action_SetParam, 0, 0, 0},
    {"SetReturnToState", Action_SetReturnToState, 0, 0, 0},
    {"ReturnToState", Action_ReturnToState, 0, 0, 0},
    {"NotifyStateChange", Action_NotifyStateChange, 0, 0, 0},
    {NULL, NULL, 0, 0, 0},
};

AICONDITIONDEF api_aiconditiondefs[] = {
    {"PreviousResult", NULL, NULL},
    {"AlwaysTrue", Condition_AlwaysTrue, Condition_AlwaysTrueInit},
    {"LocatorRange", Condition_LocatorRange, Condition_LocatorRangeInit},
    {"LocatorRangeXZ", Condition_LocatorRangeXZ, Condition_LocatorRangeInit},
    {"LocatorRangeY", Condition_LocatorRangeY, Condition_LocatorRangeInit},
    {"Timer", Condition_Timer, NULL},
    {"Random", Condition_Random, NULL},
    {"GotLocator", Condition_GotLocator, NULL},
    {"GotLocatorSet", Condition_GotLocatorSet, NULL},
    {"CurrentLocatorIs", Condition_CurrentLocatorIs, Condition_CurrentLocatorIsInit},
    {"InTriggerArea", Condition_InTriggerArea, Condition_InTriggerAreaInit},
    {"PlayerRange", Condition_PlayerRange, NULL},
    {"NearestPlayerRange", Condition_NearestPlayerRange, NULL},
    {"NearestPlayerXZRange", Condition_NearestPlayerXZRange, NULL},
    {"InLevelNode", Condition_InLevelNode, Condition_InLevelNodeInit},
    {"PlayerInLevelNode", Condition_PlayerInLevelNode, Condition_PlayerInLevelNodeInit},
    {"EitherPlayerInLevelNode", Condition_EitherPlayerInLevelNode, Condition_PlayerInLevelNodeInit},
    {"NodeRange", Condition_LevelNodeRange, Condition_LevelNodeRangeInit},
    {"GotTriggerArea", Condition_GotTriggerArea, NULL},
    {"PlayerInTriggerArea", Condition_PlayerInTriggerArea, Condition_PlayerInTriggerAreaInit},
    {"Player2InTriggerArea", Condition_Player2InTriggerArea, Condition_PlayerInTriggerAreaInit},
    {"EitherPlayerInTriggerArea", Condition_EitherPlayerInTriggerArea, Condition_PlayerInTriggerAreaInit},
    {"BaddyInTriggerArea", Condition_BaddyInTriggerArea, Condition_BaddyInTriggerAreaInit},
    {"GoodyInTriggerArea", Condition_GoodyInTriggerArea, Condition_GoodyInTriggerAreaInit},
    {"OpponentInTriggerArea", Condition_OpponentInTriggerArea, Condition_OpponentInTriggerAreaInit},
    {"GotOpponent", Condition_GotOpponent, NULL},
    {"OpponentIsAThreat", Condition_OpponentIsAThreat, NULL},
    {"OpponentOnSamePath", Condition_OpponentOnSamePath, NULL},
    {"OpponentRange", Condition_OpponentRange, NULL},
    {"NearestOpponentRange", Condition_NearestOpponentRange, NULL},
    {"YawToOpponent", Condition_YawToOpponent, NULL},
    {"OpponentBelow", Condition_OpponentBelow, NULL},
    {"OriginRange", Condition_OriginRange, NULL},
    {"OpponentToOrigin", Condition_OpponentToOrigin, NULL},
    {"PlayerToOrigin", Condition_PlayerToOrigin, NULL},
    {"OpponentToLocator", Condition_OpponentToLocator, Condition_OpponentToLocatorInit},
    {"OpponentToLocatorXZ", Condition_OpponentToLocatorXZ, Condition_OpponentToLocatorInit},
    {"OpponentToLocatorY", Condition_OpponentToLocatorY, Condition_OpponentToLocatorInit},
    {"PlayerToLocator", Condition_PlayerToLocator, Condition_PlayerToLocatorInit},
    {"NearestPlayerToLocator", Condition_NearestPlayerToLocator, Condition_PlayerToLocatorInit},
    {"OnPath", Condition_OnPath, NULL},
    {"PlayerOnPath", Condition_PlayerOnPath, NULL},
    {"OpponentOnPath", Condition_OpponentOnPath, NULL},
    {"TimeOffPath", Condition_TimeOffPath, NULL},
    {"PathBlocked", Condition_PathBlocked, NULL},
    {"InterruptID", Condition_InterruptID, NULL},
    {"IAm", Condition_IAm, Condition_IAmInit},
    {"StuckTime", Condition_StuckTime, NULL},
    {"Param", Condition_Param, NULL},
    {NULL, NULL, NULL},
};

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

AIANTINODE dynamic_antinodes[64] = {};

static void *load_pakfile;
static char *load_path;
static VARIPTR *load_buff;
static VARIPTR *load_endbuff;
static AISCRIPT *load_aiscript;
static AISTATE *load_aistate;
static NULISTHDR *load_actionshdr;
static NULISTHDR *load_conditionshdr;
static NULISTHDR *load_refscripthdr;
static AICONSTPARAMS aiscript_const[8];
static i32 aiscript_const_curr;
static i32 condition_has_no_goto;

AIACTIONDEF *game_aiactiondefs;
static AICONDITIONDEF *game_aiconditiondefs;

NULISTHDR global_aiscripts;

static i32 ExpressionRequiredNameLookup;
static i32 ExpressionNameLookupFailed;

i32 AiParseExpressionFailed;

GAMEPARAMTOFLOAT *GameParamToFloatFn;

static void *AIScriptBufferAlloc(VARIPTR *buf, VARIPTR *buf_end, usize size) {
    void *ret;

    ret = NULL;
    if (buf != NULL && buf_end != NULL) {
        if (buf_end->addr > buf->addr + size) {
            ret = (void *)ALIGN(buf->addr, 0x10);
            buf->addr = (usize)ret + size;
            memset(ret, 0, size);
        } else {
            // Debug logging goes here.
        }
    } else {
        // Debug logging goes here.
    }

    return ret;
}

static char *AIScriptCopyString(char *str, VARIPTR *buf, VARIPTR *buf_end) {
    char *dst;
    usize len;

    dst = NULL;
    if (str != NULL) {
        len = NuStrLen(str);
        if (len != 0) {
            dst = (char *)AIScriptBufferAlloc(buf, buf_end, len + 1);
            NuStrCpy(dst, str);
        }
    }

    return dst;
}

static AIACTIONDEF *AIActionFind(char *name) {
    AIACTIONDEF *def;

    if (game_aiactiondefs != NULL) {
        for (def = game_aiactiondefs; def->name != NULL; def++) {
            if (NuStrICmp(name, def->name) == 0) {
                return def;
            }
        }
    }

    for (def = api_aiactiondefs; def->name != NULL; def++) {
        if (NuStrICmp(name, def->name) == 0) {
            return def;
        }
    }

    return NULL;
}

static void AIScriptCopyActions(NULISTHDR *src, NULISTHDR *dst, VARIPTR *buf, VARIPTR *buf_end);
static void xActions(NUFPAR *parser);

#if defined(__i386__)
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AISCRIPT_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail inlines the "goto" test into each caller; this is the remaining partial clone.
static AISCRIPT_CLONE_ATTR2 void ConditionsParseFinalise(AICONDITION *cond, NUFPAR *parser)
    __asm__("_ZL23ConditionsParseFinaliseP13AICONDITION_sP8nufpar_s.part.51");

// Retail inlines the system test into its callers; this is the remaining partial clone.
static AISCRIPT_CLONE_ATTR2 void AIScriptFindRefScripts(AISYS *sys, AISCRIPT *script)
    __asm__("_ZL22AIScriptFindRefScriptsP7AISYS_sP10AISCRIPT_s.part.61");

static AICONDITIONDEF *AIConditionFind(char *name) {
    AICONDITIONDEF *def;

    if (game_aiconditiondefs != NULL) {
        for (def = game_aiconditiondefs; def->name != NULL; def++) {
            if (NuStrICmp(name, def->name) == 0) {
                return def;
            }
        }
    }

    for (def = api_aiconditiondefs; def->name != NULL; def++) {
        if (NuStrICmp(name, def->name) == 0) {
            return def;
        }
    }

    return NULL;
}

static void AIScriptCopyConditions(NULISTHDR *src, NULISTHDR *dst, VARIPTR *buf, VARIPTR *buf_end);
static void xConditions(NUFPAR *parser);
static void xRefScript(NUFPAR *parser);
static void xEndState(NUFPAR *parser);

static NUFPCOMJMP cfgtab_State[] = {
    {"CONDITIONS", &xConditions},
    {"ACTIONS", &xActions},
    {"REFERENCESCRIPT", &xRefScript},
    {"}", &xEndState},
    {NULL, NULL},
};

static void xState(NUFPAR *parser);
static void xParam(NUFPAR *parser);
static void xConst(NUFPAR *parser);
static void xConditionMacro(NUFPAR *parser);
static void xActionMacro(NUFPAR *parser);
static inline void xDeriveFromScript(NUFPAR *parser);
static void xInclude(NUFPAR *parser);

static NUFPCOMJMP cfgtab_Script[] = {
    {"STATE", &xState},
    {"PARAM", &xParam},
    {"CONST", &xConst},
    {"CONDITIONMACRO", &xConditionMacro},
    {"ACTIONMACRO", &xActionMacro},
    {"REFERENCESCRIPT", &xRefScript},
    {"DERIVEFROMSCRIPT", &xDeriveFromScript},
    {"INCLUDE", &xInclude},
    {NULL, NULL},
};

i32 ai_usepackfile = 1;
i32 ai_onlyusepackfile;

static AISCRIPT *AIScriptLoadPakFile(void *pak, char *script_file, char *path, VARIPTR *buf, VARIPTR *buf_end) {
    AISCRIPT *script;
    AISTATE *state;
    AICONDITION *condition;
    AISTATE *_unused;

    load_path = path;
    load_pakfile = pak;

    script = NULL;
    AIScriptOpenPakFileParse(&script, pak, script_file, path, buf, buf_end);

    load_path = NULL;
    load_pakfile = NULL;

    if (script != NULL) {
        state = (AISTATE *)NuLinkedListGetHead(&script->states);

        while (state != NULL) {
            condition = (AICONDITION *)NuLinkedListGetHead(&state->conditions);
            _unused = state;

            while (condition != NULL) {
                condition->next_state = AIStateFind(condition->next_state_name, script);

                condition = (AICONDITION *)NuLinkedListGetNext(&state->conditions, &condition->list_node);
            }

            while ((_unused = (AISTATE *)NuLinkedListGetNext(&script->states, &_unused->list_node)) != NULL) {
                NuStrICmp(state->name, _unused->name);
            }

            state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
        }
    }

    return script;
}

static AISTATE *AIScriptCopyState(AISTATE *src, VARIPTR *buf, VARIPTR *buf_end) {
    AISTATE *dst;
    AIREFSCRIPT *src_ref;
    AIREFSCRIPT *dst_ref;

    dst = (AISTATE *)AIScriptBufferAlloc(buf, buf_end, sizeof(AISTATE));
    if (dst == NULL) {
        return NULL;
    }

    dst->name = AIScriptCopyString(src->name, buf, buf_end);

    AIScriptCopyConditions(&src->conditions, &dst->conditions, buf, buf_end);
    AIScriptCopyActions(&src->actions, &dst->actions, buf, buf_end);

    src_ref = (AIREFSCRIPT *)NuLinkedListGetHead(&src->ref_scripts);

    while (src_ref != NULL) {
        dst_ref = (AIREFSCRIPT *)AIScriptBufferAlloc(buf, buf_end, sizeof(AIREFSCRIPT));

        if (dst_ref != NULL) {
            dst_ref->name = AIScriptCopyString(src_ref->name, buf, buf_end);
            dst_ref->return_state_name = AIScriptCopyString(src_ref->return_state_name, buf, buf_end);

            dst_ref->check_global_scripts = src_ref->check_global_scripts;
            dst_ref->check_level_scripts = src_ref->check_level_scripts;

            AIScriptCopyConditions(&src_ref->conditions, &dst_ref->conditions, buf, buf_end);

            NuLinkedListAppend(&dst->ref_scripts, &dst_ref->list_node);
        }

        src_ref = (AIREFSCRIPT *)NuLinkedListGetNext(&src->ref_scripts, &src_ref->list_node);
    }

    return dst;
}

static i32 AIScriptBuildDerivedScript(AISCRIPT *script, VARIPTR *buf, VARIPTR *buf_end, AISYS *sys) {
    AISCRIPT *src_script;
    AISTATE *src_state;
    i32 should_copy;
    AISTATE *state;
    AISTATE *new_state;
    i32 i;
    AICONDITION *condition;

    if (script == NULL) {
        return 1;
    }

    if (script->derived_from == NULL) {
        return 1;
    }

    if (!script->is_derived) {
        return 1;
    }

    src_script = AIScriptFind(sys, script->derived_from, 0, script->is_derived_from_level_script,
                              !script->is_derived_from_level_script);

    if (src_script == script) {
        // Do nothing.
    } else if (src_script == NULL) {
        // Debug logging goes here.
    } else {
        if (src_script->is_derived) {
            return 0;
        }

        src_state = (AISTATE *)NuLinkedListGetHead(&src_script->states);

        while (src_state != NULL) {
            should_copy = 1;

            state = (AISTATE *)NuLinkedListGetHead(&script->states);

            while (state != NULL) {
                if (NuStrICmp(src_state->name, state->name) == 0) {
                    should_copy = 0;
                    break;
                }

                state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
            }

            if (should_copy) {
                new_state = AIScriptCopyState(src_state, buf, buf_end);

                if (new_state != NULL) {
                    NuLinkedListAppend(&script->states, &new_state->list_node);
                }
            }

            src_state = (AISTATE *)NuLinkedListGetNext(&src_script->states, &src_state->list_node);
        }

        for (i = 0; i < 4; i++) {
            if (src_script->params[i].name != NULL && script->params[i].name == NULL) {
                script->params[i].name = AIScriptCopyString(src_script->params[i].name, buf, buf_end);
                script->params[i].default_val = src_script->params[i].default_val;
            }
        }

        state = (AISTATE *)NuLinkedListGetHead(&script->states);

        while (state != NULL) {
            condition = (AICONDITION *)NuLinkedListGetHead(&state->conditions);

            while (condition != NULL) {
                condition->next_state = AIStateFind(condition->next_state_name, script);

                condition = (AICONDITION *)NuLinkedListGetNext(&state->conditions, &condition->list_node);
            }

            state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
        }
    }

    script->is_derived = 0;

    return 1;
}

static AISCRIPTPROCESS *AiEvalExpressionProcessor;
static AIPACKET *AiEvalExpressionPacket;

#include "editor/edpath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuplane.h"

#include <stdio.h>
#include <float.h>
#include <string.h>

static const u32 iAISysPathColoursG[5] = {0xffff0000, 0xff00ff00, 0xff00ffff, 0xffff00ff, 0xffffff00};

static __used__ i32 RowMoveWander(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);
static __used__ i32 RowMoveTowards(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);
static void AISysCheckAntinode_Rectangle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);
static void AISysCheckAntinode_Ellipse(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);
static void AISysCheckAntinode_Circle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

extern "C" {
    void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32) = {
        AISysCheckAntinode_Circle, AISysCheckAntinode_Ellipse, AISysCheckAntinode_Rectangle};
    char *AiLevelPathName = "";
    AISCRIPTPROCESS *pSetStateDebugee;
    SCRIPTPROCESSFIRSTTIMEACTION *ScriptProcessFirstTimeActionFn;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

typedef i32 MIDSPECIALMOVE(AISYS *system, AIPACKET *packet, APIOBJECT *object);
extern MIDSPECIALMOVE *MidSpecialMoveFn;
extern "C" void (*GameAISYSRebuildFromEditorDataFn)(AISYS *, VARIPTR *, VARIPTR *);

static void *AISysLoadAlloc(AISYS *system, u32 size) {
    return AISysBufferAlloc(&system->storage_cursor, &system->storage_end, size);
}

static char *AISysLoadString(AISYS *system, i32 length) {
    if (length == 0) {
        return NULL;
    }

    char *text = static_cast<char *>(AISysLoadAlloc(system, length + 1));
    EdFileRead(text, length);
    return text;
}

static void AISysLoadPathRoutes(AISYS *system, AIPATH *path, i32 version, char *name_buffer) {
    path->route_matrix = static_cast<u8 **>(AISysLoadAlloc(system, path->node_count * sizeof(u8 *)));
    for (i32 i = 0; i < path->node_count; ++i) {
        path->route_matrix[i] = static_cast<u8 *>(AISysLoadAlloc(system, path->node_count));
        EdFileRead(path->route_matrix[i], path->node_count);
    }

    if (version <= 4)
        return;
    path->route_count = static_cast<u8>(EdFileReadChar());
    if (path->route_count != 0) {
        path->routes = static_cast<AIPATHROUTE *>(AISysLoadAlloc(system, path->route_count * sizeof(AIPATHROUTE)));
        for (i32 i = 0; i < path->route_count; ++i) {
            AIPATHROUTE *route = &path->routes[i];
            i32 name_length = EdFileReadChar();
            if (name_length != 0) {
                route->name = static_cast<char *>(AISysLoadAlloc(system, name_length));
                EdFileRead(route->name, name_length);
                route->route_count = static_cast<u8>(EdFileReadChar());
                route->exit_node_count = static_cast<u8>(EdFileReadChar());
                EdFileReadChar();
                EdFileReadChar();

                if (path->node_count != 0 && route->route_count != 0) {
                    route->node_routes = static_cast<u8 *>(AISysLoadAlloc(system, path->node_count));
                    EdFileRead(route->node_routes, path->node_count);
                    route->node_directions = static_cast<u8 *>(AISysLoadAlloc(system, route->route_count));
                    EdFileRead(route->node_directions, route->route_count);
                    route->route_nodes = static_cast<u8 **>(AISysLoadAlloc(system, route->route_count * sizeof(u8 *)));
                    for (i32 route_index = 0; route_index < route->route_count; ++route_index) {
                        route->route_nodes[route_index] = static_cast<u8 *>(AISysLoadAlloc(system, route->route_count));
                        EdFileRead(route->route_nodes[route_index], route->route_count);
                    }
                    if (route->exit_node_count != 0) {
                        route->exit_nodes = static_cast<u8 *>(AISysLoadAlloc(system, route->exit_node_count));
                        EdFileRead(route->exit_nodes, route->exit_node_count);
                    }
                }
            }

            i32 character_count = EdFileReadChar();
            for (i32 character_index = 0; character_index < character_count; ++character_index) {
                char *character_name = name_buffer;
                i32 character_name_length = EdFileReadChar();
                EdFileRead(character_name, character_name_length);
                if (SpecialRouteCharacterTypeIDFn != NULL) {
                    u8 type = SpecialRouteCharacterTypeIDFn(character_name);
                    if (type < 64) {
                        route->character_masks[0] |= static_cast<u64>(1) << type;
                        route->character_masks[1] |= static_cast<u64>(1) << type;
                    } else {
                        route->character_mask[0] = 0xffffffff;
                        route->character_mask[1] = 0xffffffff;
                        route->character_mask[3] |= 0x80000000;
                    }
                }
            }
        }
    }

    if (version <= 18)
        return;
    path->special_route_count = static_cast<u8>(EdFileReadChar());
    if (path->special_route_count != 0) {
        path->special_routes =
            static_cast<AIPATHNODELINK *>(AISysLoadAlloc(system, path->special_route_count * sizeof(AIPATHNODELINK)));
        AIPATHNODELINK *route = path->special_routes;
        for (i32 i = 0; i < path->special_route_count; ++i, ++route) {
            route->node_index = EdFileReadUnsignedChar();
            route->special_route_index = EdFileReadShort();
        }
    }
}

static AIPATHSYS *AISysLoadPaths(AISYS *system, i32 version, NUGSCN *scene, char *name_buffer) {
    i32 path_count = EdFileReadInt();
    if (path_count == 0) {
        return NULL;
    }

    AIPATHSYS *path_system = static_cast<AIPATHSYS *>(AISysLoadAlloc(system, sizeof(AIPATHSYS)));
    if (path_system == NULL) {
        return NULL;
    }
    path_system->path_count = static_cast<u8>(path_count);
    path_system->paths = static_cast<AIPATH **>(AISysLoadAlloc(system, path_system->path_count * sizeof(AIPATH *)));

    for (i32 path_index = 0; path_index < path_system->path_count; ++path_index) {
        path_system->paths[path_index] = static_cast<AIPATH *>(AISysLoadAlloc(system, sizeof(AIPATH)));
        AIPATH *path = path_system->paths[path_index];
        EdFileRead(path->name, sizeof(path->name));
        path->node_count = static_cast<u8>(EdFileReadChar());
        path->flags = static_cast<u8>(EdFileReadChar());
        path->index = static_cast<u8>(path_index);
        if (version == 1) {
            path->connection_count = static_cast<i16>(EdFileReadChar());
        } else {
            path->connection_count = EdFileReadShort();
        }

        if (path->connection_count != 0) {
            path->connections =
                static_cast<AIPATHCNX *>(AISysLoadAlloc(system, path->connection_count * sizeof(AIPATHCNX)));
            for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                AIPATHCNX *connection = &path->connections[connection_index];
                connection->direction_a = static_cast<u8>(EdFileReadChar());
                connection->direction_b = static_cast<u8>(EdFileReadChar());
                if (version > 11) {
                    connection->node_a = EdFileReadInt();
                    connection->node_b = EdFileReadInt();
                } else if (version > 8) {
                    connection->node_a = EdFileReadShort();
                    connection->node_b = EdFileReadShort();
                } else {
                    connection->node_a = EdFileReadChar();
                    connection->node_b = EdFileReadChar();
                }
                connection->previous_node_a = connection->node_a;
                connection->previous_node_b = connection->node_b;
                connection->flags = EdFileReadShort();
                connection->game_flags = EdFileReadShort();
                connection->width = EdFileReadFloat();
                connection->cost = EdFileReadFloat();
                if (connection->cost == 0.0f)
                    connection->cost = 0.00001f;
            }
        }

        if (version == 1)
            EdFileReadChar();
        if (path->node_count != 0) {
            path->nodes = static_cast<AIPATHNODE *>(AISysLoadAlloc(system, path->node_count * sizeof(AIPATHNODE)));
            for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                AIPATHNODE *node = &path->nodes[node_index];
                const i32 name_length = EdFileReadInt();
                if (name_length != 0) {
                    node->name = static_cast<char *>(AISysLoadAlloc(system, name_length + 1));
                    EdFileRead(node->name, name_length);
                }
                node->position.x = EdFileReadFloat();
                node->position.y = EdFileReadFloat();
                node->position.z = EdFileReadFloat();
                node->radius = EdFileReadFloat();
                node->radius_squared = node->radius * node->radius;
                if (version < 8) {
                    node->min_height = node->position.y - default_path_heighttol;
                    node->max_height = node->position.y + default_path_heighttol;
                } else {
                    node->min_height = EdFileReadFloat();
                    node->max_height = EdFileReadFloat();
                }
                node->min_height_offset = node->min_height - node->position.y;
                node->max_height_offset = node->max_height - node->position.y;
                node->connection_count = static_cast<u8>(EdFileReadChar());
                node->flags = static_cast<u8>(EdFileReadChar());
                EdFileReadChar();
                node->runtime_flags = static_cast<u8>(EdFileReadChar()) & ~6u;
                node->path_flags = EdFileReadShort();
                if (version < 19) {
                    EdFileReadChar();
                    node->special_route_index = 0xff;
                } else {
                    node->special_route_index = static_cast<u8>(EdFileReadChar());
                }

                char *special_name = name_buffer;
                i32 special_name_length = EdFileReadChar();
                if (special_name_length != 0) {
                    EdFileRead(special_name, special_name_length);
                    node->has_special =
                        static_cast<u8>(NuSpecialFind(scene, &node->special_handle, special_name, 1) != 0);
                    node->special_position.x = EdFileReadFloat();
                    node->special_position.y = EdFileReadFloat();
                    node->special_position.z = EdFileReadFloat();
                }

                if (path->connections != NULL && node->connection_count != 0) {
                    node->connections =
                        static_cast<AIPATHCNX **>(AISysLoadAlloc(system, node->connection_count * sizeof(AIPATHCNX *)));
                    for (i32 i = 0; i < node->connection_count; ++i) {
                        i16 connection_index = EdFileReadShort();
                        node->connections[i] = &path->connections[connection_index];
                    }
                    if ((node->connection_count & 1) != 0) {
                        EdFileReadShort();
                    }
                }

                if (version > 4) {
                    node->route_membership_mask = EdFileReadShort();
                    node->route_boundary_mask = EdFileReadShort();
                }
                node->distance_cache_nodes[0] = 0xff;
                node->distance_cache_nodes[1] = 0xff;
            }
            AIPathCalcExtents(path);
        }

        AISysLoadPathRoutes(system, path, version, name_buffer);
    }

    if (version < 19)
        return path_system;
    path_system->special_route_count = EdFileReadShort();
    if (path_system->special_route_count != 0) {
        path_system->special_routes = static_cast<AIPATHSPECIALROUTE *>(
            AISysLoadAlloc(system, path_system->special_route_count * sizeof(AIPATHSPECIALROUTE)));
        for (i32 i = 0; i < path_system->special_route_count; ++i) {
            AIPATHSPECIALROUTE *route = &path_system->special_routes[i];
            route->path_count = static_cast<u8>(EdFileReadChar());
            route->paths = static_cast<AIPATH **>(AISysLoadAlloc(system, route->path_count * sizeof(AIPATH *)));
            auto route_path_count = route->path_count;
            for (i32 path_index = 0; path_index < route_path_count; ++path_index) {
                const i32 read_path_index = EdFileReadChar();
                route->paths[path_index] = path_system->paths[read_path_index];
            }
        }
    }
    return path_system;
}

static AIAREA *AISysLoadFindArea(AISYS *system, char *name) {
    auto system_area_count = system->area_count;
    for (i32 index = 0; index < system_area_count; ++index) {
        AIAREA *area = &system->areas[index];
        if (NuStrICmp(name, area->name) == 0) {
            return area;
        }
    }
    return NULL;
}

#include "legoapi/ai/core/ai_pathgeometry.h"

static void AISysResetPathSearchConnectionChecks(AIPATH *path) {
    if (path == NULL || path->nodes == NULL || path->node_count == 0) {
        return;
    }

    ++path->search_checksum;
    if (path->search_checksum == 0) {
        path->search_checksum = 1;
    }

    ++path->search_reset_node;
    if (path->search_reset_node >= path->node_count) {
        path->search_reset_node = 0;
    }

    AIPATHNODE &node = path->nodes[path->search_reset_node];
    for (i32 connection_index = 0; connection_index < node.connection_count; ++connection_index) {
        node.connections[connection_index]->last_search_checksum = 0;
    }
}

static AILOCATOR *AISysLoadFindLocator(AISYS *system, char *name) {
    auto system_locator_count = system->locator_count;
    for (i32 index = 0; index < system_locator_count; ++index) {
        AILOCATOR *locator = &system->locators[index];
        if (NuStrICmp(name, locator->name) == 0) {
            return locator;
        }
    }
    return NULL;
}

static AIAREA *AISysLoadAreaReference(AISYS *system, char *name) {
    if (EdFileReadInt() == 0) {
        return NULL;
    }
    EdFileRead(name, 16);
    return AISysLoadFindArea(system, name);
}

static AILOCATOR *AISysLoadLocatorReference(AISYS *system, char *name) {
    if (EdFileReadInt() == 0) {
        return NULL;
    }
    EdFileRead(name, 16);
    return AISysLoadFindLocator(system, name);
}

static void AISysLoadAreas(AISYS *system, i32 version) {
    system->area_count = EdFileReadInt();
    if (system->area_count == 0) {
        return;
    }

    system->areas = static_cast<AIAREA *>(AISysLoadAlloc(system, system->area_count * sizeof(AIAREA)));
    if (version > 19) {
        for (i32 index = 0; index < system->area_count; ++index) {
            AIAREA *area = &system->areas[index];
            EdFileRead(area->name, sizeof(area->name));
            area->min_x = EdFileReadFloat();
            area->min_y = EdFileReadFloat();
            area->min_z = EdFileReadFloat();
            area->max_x = EdFileReadFloat();
            area->max_y = EdFileReadFloat();
            area->max_z = EdFileReadFloat();
            area->flags = EdFileReadShort();
            area->system = system;
            area->game_flags = static_cast<u8>(EdFileReadChar());
            EdFileReadChar();
        }
    } else {
        for (i32 index = 0; index < system->area_count; ++index) {
            AIAREA *area = &system->areas[index];
            EdFileRead(area->name, sizeof(area->name));
            area->min_x = EdFileReadFloat();
            area->min_y = EdFileReadFloat();
            area->min_z = EdFileReadFloat();
            area->max_x = EdFileReadFloat();
            area->max_y = EdFileReadFloat();
            area->max_z = EdFileReadFloat();
            area->flags = EdFileReadShort();
            area->system = system;
            EdFileReadChar();
            EdFileReadChar();
        }
    }
}

static void AISysLoadLocators(AISYS *system, i32 version) {
    system->locator_count = EdFileReadInt();
    if (system->locator_count == 0) {
        return;
    }

    system->locators = static_cast<AILOCATOR *>(AISysLoadAlloc(system, system->locator_count * sizeof(AILOCATOR)));
    for (i32 index = 0; index < system->locator_count; ++index) {
        AILOCATOR *locator = &system->locators[index];
        EdFileRead(locator->name, sizeof(locator->name));
        locator->position.x = EdFileReadFloat();
        locator->position.y = EdFileReadFloat();
        locator->position.z = EdFileReadFloat();
        locator->flags = EdFileReadShort();
        u8 path_index = static_cast<u8>(EdFileReadChar());
        locator->path_info.path = system->path_sys->paths[path_index];
        locator->path_info.direction = static_cast<u8>(EdFileReadChar());
        u16 connection_index = static_cast<u16>(EdFileReadShort());
        locator->path_info.connection = &locator->path_info.path->connections[connection_index];
        locator->path_info.dist = EdFileReadFloat();
        locator->path_info.width = EdFileReadFloat();
        if (version > 14) {
            locator->locator_flags = EdFileReadInt();
        }
    }
}

static void AISysLoadLocatorSets(AISYS *system, i32 version) {
    if (version <= 17) {
        return;
    }

    system->locator_set_count = EdFileReadInt();
    if (system->locator_set_count == 0) {
        return;
    }

    system->locator_sets =
        static_cast<AILOCATORSET *>(AISysLoadAlloc(system, system->locator_set_count * sizeof(AILOCATORSET)));
    for (i32 index = 0; index < system->locator_set_count; ++index) {
        AILOCATORSET *locator_set = &system->locator_sets[index];
        EdFileRead(locator_set->name, sizeof(locator_set->name));
        locator_set->locator_count = static_cast<i8>(EdFileReadInt());
        locator_set->locator_entries =
            static_cast<u8 *>(AISysLoadAlloc(system, locator_set->locator_count * sizeof(AILOCATORSET)));
        for (i32 locator_index = 0; locator_index < locator_set->locator_count; ++locator_index) {
            locator_set->locator_entries[locator_index] = static_cast<u8>(EdFileReadChar());
        }
        locator_set->assigned = static_cast<u8 *>(AISysLoadAlloc(system, locator_set->locator_count));
        memset(locator_set->assigned, 0, locator_set->locator_count);
    }
}

static void AISysLoadCreatures(AISYS *system, i32 version, char *character_name, char *area_name) {
    system->creature_count = EdFileReadInt();
    if (system->creature_count == 0) {
        return;
    }

    system->creatures = static_cast<AICREATURE *>(AISysLoadAlloc(system, system->creature_count * sizeof(AICREATURE)));
    auto system_path_sys = system->path_sys;
    for (i32 index = 0; index < system->creature_count; ++index) {
        AICREATURE *creature = &system->creatures[index];
        EdFileRead(creature->name, sizeof(creature->name));
        EdFileRead(creature->script_name, sizeof(creature->script_name));

        if (creature->script_name[0] == '\0') {
            strcpy(creature->script_name, "default");
        }
        if (version < 14)
            EdFileRead(character_name, 16);
        else
            EdFileRead(character_name, 32);
        if (GlobalCharacterTypeIDFn != NULL)
            creature->type = static_cast<i16>(GlobalCharacterTypeIDFn(character_name));

        creature->pos.x = EdFileReadFloat();
        creature->pos.y = EdFileReadFloat();
        creature->pos.z = EdFileReadFloat();
        creature->y_rot = static_cast<NUANG>(EdFileReadShort());

        if (version > 15) {
            creature->set = static_cast<u8>(EdFileReadChar());
        }
        creature->count = static_cast<u8>(EdFileReadChar());
        creature->count_across = static_cast<u8>(EdFileReadChar());
        creature->active_mask = static_cast<u32>(EdFileReadInt());
        creature->x_spacing = EdFileReadFloat();
        creature->z_spacing = EdFileReadFloat();
        creature->flags = EdFileReadInt();
        u8 path_index = static_cast<u8>(EdFileReadChar());
        creature->path_info.path = system_path_sys->paths[path_index];
        creature->path_info.direction = static_cast<u8>(EdFileReadChar());
        u16 connection_index = static_cast<u16>(EdFileReadShort());
        creature->path_info.connection = &creature->path_info.path->connections[connection_index];

        if (version > 2) {
            creature->script_params[0] = EdFileReadFloat();
            creature->script_params[1] = EdFileReadFloat();
            creature->script_params[2] = EdFileReadFloat();
            creature->script_params[3] = EdFileReadFloat();
        }

        if (version > 3) {
            creature->area = AISysLoadAreaReference(system, area_name);
        }
        if (version > 5) {
            creature->locator = AISysLoadLocatorReference(system, area_name);
        }
        if (version > 16) {
            creature->respawn_locator = AISysLoadLocatorReference(system, area_name);
        }

        if (version > 7) {
            creature->activation_difficulty = static_cast<u8>(EdFileReadChar());
            creature->min_respawn_count = EdFileReadChar();
            creature->max_respawn_count = EdFileReadChar();
            creature->activate_type = static_cast<u8>(EdFileReadChar());
            creature->min_respawn_time = EdFileReadFloat();
            creature->max_respawn_time = EdFileReadFloat();
        } else {
            creature->activation_difficulty = default_activate_difficulty;
            creature->min_respawn_count = static_cast<i8>(default_min_n_respawns);
            creature->max_respawn_count = static_cast<i8>(default_max_n_respawns);
            creature->min_respawn_time = default_min_t_respawn;
            creature->max_respawn_time = default_max_t_respawn;
        }
        if (version > 9) {
            creature->start_stagger = EdFileReadFloat();
        } else {
            creature->start_stagger = default_stagger_start;
        }
        if (creature->activate_type == 1) {
            EdFileRead(area_name, 16);
            creature->activate_area = AISysLoadFindArea(system, area_name);
        }
        if (version > 10) {
            creature->view_distance = EdFileReadFloat();
            creature->hear_distance = EdFileReadFloat();
            creature->max_view_height = EdFileReadFloat();
            creature->min_view_height = EdFileReadFloat();
            EdFileReadInt();
            if (GetViewRangeFn != NULL && creature->view_distance == 0.0f) {
                creature->view_distance = GetViewRangeFn(creature->type);
            }
            if (GetHearDistanceFn != NULL && creature->hear_distance == 0.0f) {
                creature->hear_distance = GetHearDistanceFn(creature->type);
            }
            if (GetMaxViewHeightFn != NULL && creature->max_view_height == 0.0f) {
                creature->max_view_height = GetMaxViewHeightFn(creature->type);
            }
            if (GetMinViewHeightFn != NULL && creature->min_view_height == 0.0f) {
                creature->min_view_height = GetMinViewHeightFn(creature->type);
            }
        } else {
            creature->view_distance = GetViewRangeFn == NULL ? 1.0f : GetViewRangeFn(creature->type);
            creature->hear_distance = GetHearDistanceFn == NULL ? 1.0f : GetHearDistanceFn(creature->type);
            creature->max_view_height = GetMaxViewHeightFn == NULL ? 1.0f : GetMaxViewHeightFn(creature->type);
            creature->min_view_height = GetMinViewHeightFn == NULL ? 1.0f : GetMinViewHeightFn(creature->type);
        }
    }
}

static void AISysLoadAntinodes(AISYS *system, i32 version, NUGSCN *scene, char *name_buffer) {
    system->antinode_count = EdFileReadInt();
    if (system->antinode_count == 0) {
        return;
    }

    system->antinodes = static_cast<AIANTINODE *>(AISysLoadAlloc(system, system->antinode_count * sizeof(AIANTINODE)));
    for (i32 index = 0; index < system->antinode_count; ++index) {
        AIANTINODE *antinode = &system->antinodes[index];
        antinode->enabled = 1;
        antinode->position.x = EdFileReadFloat();
        antinode->position.y = EdFileReadFloat();
        antinode->position.z = EdFileReadFloat();
        antinode->radius = EdFileReadFloat();
        antinode->base_radius = antinode->radius;
        antinode->base_height = antinode->radius;
        antinode->min_y = EdFileReadFloat();
        antinode->max_y = EdFileReadFloat();
        antinode->min_y_offset = antinode->min_y - antinode->position.y;
        antinode->max_y_offset = antinode->max_y - antinode->position.y;

        if (version > 14) {
            antinode->flags = EdFileReadInt();
            antinode->base_radius = EdFileReadFloat();
            antinode->base_height = EdFileReadFloat();
            antinode->type = static_cast<u8>(EdFileReadChar());
            EdFileReadChar();
            EdFileReadChar();
        } else {
            EdFileReadChar();
            EdFileReadChar();
            EdFileReadChar();
        }
        antinode->game_flags = static_cast<u8>(EdFileReadChar());

        char *special_name = name_buffer;
        i32 special_name_length = EdFileReadChar();
        if (special_name_length != 0) {
            EdFileRead(special_name, special_name_length);
            antinode->has_special =
                static_cast<u8>(NuSpecialFind(scene, &antinode->special_handle, special_name, 1) != 0);
            antinode->special_position.x = EdFileReadFloat();
            antinode->special_position.y = EdFileReadFloat();
            antinode->special_position.z = EdFileReadFloat();
            if (version > 14) {
                antinode->rotation_offset = EdFileReadInt();
            }
        }
    }
}

extern "C" {
void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);
} // extern "C"

static AICONDITION *AIScriptFinishStateConditions(NULISTHDR *conditions, AIPACKET *packet, AISCRIPTPROCESS *processor,
                                                  AICONDITION *condition, i32 stack_index) {
    if (processor->param_stack[stack_index].force_complex_eval) {
        if (processor->param_stack[stack_index].is_first_time_state) {
            AICONDITION *remaining = reinterpret_cast<AICONDITION *>(
                NuLinkedListGetNext(conditions, reinterpret_cast<NULISTLNK *>(condition)));
            while (remaining != NULL) {
                if (remaining->is_complex) {
                    processor->param_stack[stack_index].complex_params[remaining->param_idx] =
                        AIParamToFloatEx(packet, processor, remaining->complex_arg);
                }
                remaining = reinterpret_cast<AICONDITION *>(NuLinkedListGetNext(conditions, &remaining->list_node));
            }
        }
        processor->param_stack[stack_index].force_complex_eval = 0;
    }
    processor->param_stack[stack_index].is_first_time_state = 0;
    return condition;
}

#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.
static AI_SYS_STUBS_CLONE_ATTR2 AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions,
                                                                           AIPACKET *packet,
                                                                           AISCRIPTPROCESS *processor,
                                                                           i32 stack_index)
    __asm__("_ZL30AIScriptProcessStateConditionsP7AISYS_sP11nulisthdr_sP11APIOBJECT_s"
            "P10AIPACKET_sP17AISCRIPTPROCESS_sfi.isra.77");

extern "C" {
    f32 AiRndrCullDistance;

    AIEDITOR_RENDER_STATE *aieditor;

    typedef void AIEDITORCALLBACK(void);
    typedef void AIEDITORREBUILDCALLBACK(AISYS *, VARIPTR *, VARIPTR *);

    AIEDITORCALLBACK *AIPathDeletedFn;
    AIEDITORPATHNODECALLBACK *AIPathNodeDeletedFn;
    AIEDITORPATHNODECALLBACK *AIPathNodeMovedFn;
    AIEDITORREBUILDCALLBACK *GameAISYSRebuildFromEditorDataFn;
    AIEDITORCALLBACK *GameAISaveFn;

void InitFn_AIPathDeleted(AIEDITORCALLBACK *function);

void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function);

void InitFn_GameAISave(AIEDITORCALLBACK *function);
} // extern "C"

#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuqfnt.h"
#include <string.h>
#include <stdio.h>

struct nupad_s;

static inline EDAIAREA_s *&area_selected() {
    return *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(aieditor) + 0x37a48);
}
static inline EDAIAREA_s *&area_hovered() {
    return *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(aieditor) + 0x37a4c);
}
static inline NULISTHDR *area_free_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a38);
}
static inline NULISTHDR *area_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
}
static inline i32 &area_rotation_step() {
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
}
static inline EDAIAREA_s *area_head() {
    return reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_list()));
}
static inline EDAIAREA_s *area_next(EDAIAREA_s *area) {
    return reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetNext(area_list(), &area->link));
}

extern "C" void *ed_fnt;
extern "C" void creatureEditor_RenderAllCreatures();

// Item colours shared by the AI editors.
static u32 attr[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};

static __used__ void areaEditor_cbDeleteArea(eduimenu_s *menu, eduiitem_s *, unsigned int);
static __used__ void areaEditor_cbRenameArea(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void areaEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void areaEditor_cbRenameAreaMenu(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void areaEditor_cbAreaCylinderToggle(eduimenu_s *, eduiitem_s *item, unsigned int);

__attribute__((optimize("no-tree-loop-optimize"))) void areaEditor_Enter();

static eduimenu_s *areaEditorOptionsMenu() {
    eduimenu_s *menu =
        eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
    if (menu == NULL) {
        return NULL;
    }
    eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, &attr, 0, 0, aieditor_cvSelectEditorMode,
                                            const_cast<char *>("Select Editor Mode")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbGoToPlayer, const_cast<char *>("Go To Player")));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbMovePlayer, const_cast<char *>("Move Player")));
    if (area_selected() != NULL && AIScriptNameFromIx(aieditor->ai_system, 0) != NULL) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, &attr, 0, 0, areaEditor_cbRenameAreaMenu, const_cast<char *>("Rename Area")));
    }
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &attr, -i32(aieditorsettings.stop_platforms), 3,
                                               aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, &attr, -i32(aieditorsettings.snap_height_display), 2,
                                               aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
    if (area_selected() != NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, NULL, const_cast<char *>("=================")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, &attr, area_selected()->flags & 1, 4,
                                                   areaEditor_cbAreaCylinderToggle, const_cast<char *>("Cylinder")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, NULL, const_cast<char *>("=================")));
    }
    return menu;
}

static eduimenu_s *areaEditorDeleteMenu() {
    eduimenu_s *menu =
        eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Delete area??"));
    if (menu != NULL) {
        eduiMenuAddItem(menu, eduiItemSelCreate(0, &attr, 0, 0, areaEditor_cbDeleteArea, const_cast<char *>("No")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, areaEditor_cbDeleteArea, const_cast<char *>("Yes")));
    }
    return menu;
}

static EDAIAREA_s *areaEditorFindHover() {
    EDAIAREA_s *nearest = NULL;
    f32 best_distance = 3.402823466e38f;
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        NUVEC difference;
        f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &area->position, &difference);
        if (!(distance < best_distance)) {
            continue;
        }
        f32 height = aieditor->camera_position.y - area->position.y;
        if (area->flags & 1) {
            if (distance < area->size.x * area->size.x && height < area->size.y && height > -2.0f) {
                nearest = area;
                best_distance = distance;
            }
        } else {
            difference.y = height;
            NuVecRotateY(&difference, &difference, -area->rotation);
            if (difference.x < area->size.x && difference.y < area->size.y && difference.z < area->size.z &&
                difference.x > -area->size.x && difference.y > -2.0f && difference.z > -area->size.z) {
                nearest = area;
                best_distance = distance;
            }
        }
    }
    return nearest;
}

static EDAIAREA_s *areaEditorCreateArea() {
    AIEDITOR_RENDER_STATE *context = aieditor;
    EDAIAREA_s *previous = area_selected();
    u8 flags = previous != NULL ? previous->flags : 0;
    i32 rotation = aieditorsettings.area_rotation;
    EDAIAREA_s *area = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_free_list()));
    if (area == NULL) {
        *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(context) + 0x37a48) = NULL;
        return NULL;
    }
    NuLinkedListRemove(area_free_list(), &area->link);
    NuLinkedListAppend(area_list(), &area->link);
    area->position = context->camera_position;
    area->rotation = static_cast<i16>(rotation);
    area->flags = flags;
    *reinterpret_cast<EDAIAREA_s **>(reinterpret_cast<u8 *>(context) + 0x37a48) = area;
    area = area_selected();
    if (area == NULL)
        return NULL;
    if (previous != NULL) {
        area->size = previous->size;
    } else {
        area->size.x = 1.0f;
        area->size.y = 1.0f;
        area->size.z = 1.0f;
    }
    char name[16];
    i32 suffix = 0;
    for (;;) {
        sprintf(name, "Area%d", ++suffix);
        bool duplicate = false;
        for (EDAIAREA_s *other = area_head(); other != NULL; other = area_next(other)) {
            if (NuStrICmp(other->name, name) == 0) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            strcpy(area_selected()->name, name);
            return area;
        }
    }
}

#include "decomp.h"
#include "editor/edpath.h"
#include "editor/path_connections.h"
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <stdint.h>

#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "globals.h"
extern "C" {
    extern void *ed_fnt;
    void creatureEditor_PathNodeMoved(EDAIPATHNODE_s *);
    void creatureEditor_PathDeleted(EDAIPATH_s *);
    void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *);
    void creatureEditor_RenderAllCreatures(void);
    NUVEC edpath_addoffset;
    char *(*EdGetCnxFlagNames)(u32 flags);
    f32 default_path_node_radius = .25f;
}
static AIPATHCNXTYPE_s aipathcnxtypes[32];
static i32 naipathcnxtypes;

// Retail splits the null checks into each caller and keeps the body as a
// partial-inlining clone; its connection searches are fully peeled.
static __attribute__((noinline, noclone, optimize("peel-loops"))) void DestroyAIPathNodeBody(EDAIPATHNODE_s *,
                                                                                            EDAIPATH_s *)
    __asm__("_ZL17DestroyAIPathNodeP14EDAIPATHNODE_sP10EDAIPATH_s.part.42");
static inline void DestroyAIPathNode(EDAIPATHNODE_s *node, EDAIPATH_s *path) {
    if (node == NULL || path == NULL)
        return;
    DestroyAIPathNodeBody(node, path);
}
extern "C" AIEDITORPATHNODECALLBACK *AIPathNodeMovedFn;

static inline __attribute__((always_inline)) void pathEditor_DisconnectNodes(EDAIPATHNODE_s *node,
                                                                           EDAIPATHNODE_s *other) {
    for (i32 i = 0; i < 8; ++i) {
        if (node->connections[i].node == other) {
            for (i32 j = 0; j < 8; ++j) {
                if (other->connections[j].node == node) {
                    node->connections[i].node = NULL;
                    node->connections[i].flags = 0;
                    node->connections[i].route_mask = 0;
                    node->connections[i].unknown_0a = 0;
                    other->connections[j].node = NULL;
                    other->connections[j].flags = 0;
                    other->connections[j].route_mask = 0;
                    other->connections[j].unknown_0a = 0;
                    return;
                }
            }
        }
    }
}

struct EDAISHAREDPATHNODE_s;
struct AIPATH_s;
struct eduimenu_s;
struct eduiitem_s;
struct nuvec_s;

void pathEditorDrawNode(NUVEC *position, f32 radius, f32 lower_height, f32 upper_height, u32 colour, numtl_s *material,
                        i32 segments, i32 solid);

static __used__ void pathEditor_cbCreatePath(eduimenu_s *, eduiitem_s *, u32);
static __used__ void pathEditor_cbDeletePath(eduimenu_s *parent, eduiitem_s *item, u32);
static __used__ void pathEditor_cbRenameNode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbRenamePath(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *, u32);
static __used__ void pathEditor_cbSetShareNode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbShareNodeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void pathEditor_cbCnxFlagsToggle(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbDeletePathNode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbRenameNodeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void pathEditor_cbRenamePathMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void pathEditor_cbSelectPathMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbNodeFlagsToggle(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbDisconnectPathNode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbCancelDeleteAreaMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelDeleteNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbDrawWallsplinesToggle(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void pathEditor_cbCancelDisconnectNodeMenu(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *, u32);
static __used__ void routeEditor_cbRouteUsers(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void routeEditor_cbCreateRoute(eduimenu_s *, eduiitem_s *, u32);
static __used__ void routeEditor_cbDeleteRoute(eduimenu_s *parent, eduiitem_s *item, u32);
static __used__ __attribute__((optimize("peel-loops"))) void routeEditor_cbRenameRoute(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *, eduimenu_s *);
static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void routeEditor_cbRenameRouteMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *menu, eduimenu_s *);
static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *menu, eduimenu_s *);

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) __asm__(
    "_ZL22routeEditor_AddToRouteP14EDAIPATHNODE_sS0_.part.7");

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other);
static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared);

static inline i32 pathEditor_FirstFreeConnection(EDAIPATHNODE_s *node) {
    if (node->connections[0].node == NULL)
        return 0;
    if (node->connections[1].node == NULL)
        return 1;
    if (node->connections[2].node == NULL)
        return 2;
    if (node->connections[3].node == NULL)
        return 3;
    if (node->connections[4].node == NULL)
        return 4;
    if (node->connections[5].node == NULL)
        return 5;
    if (node->connections[6].node == NULL)
        return 6;
    if (node->connections[7].node == NULL)
        return 7;
    return -1;
}

static inline bool pathEditor_ConnectNodes(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) {
    const i32 node_slot = pathEditor_FirstFreeConnection(node);
    if (node_slot == -1)
        return false;
    const i32 other_slot = pathEditor_FirstFreeConnection(other);
    if (other_slot == -1)
        return false;
    node->connections[node_slot].node = other;
    node->connections[node_slot].flags = 0;
    other->connections[other_slot].node = node;
    other->connections[other_slot].flags = 0;
    return true;
}

static inline bool pathEditor_HasConnection(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) {
    return node->connections[0].node == other || node->connections[1].node == other ||
           node->connections[2].node == other || node->connections[3].node == other ||
           node->connections[4].node == other || node->connections[5].node == other ||
           node->connections[6].node == other || node->connections[7].node == other;
}

#if defined(__i386__)
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone, regparm(1)))
#else
#define PATHEDITOR_SETCAMPOS_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static PATHEDITOR_SETCAMPOS_BODY_ATTR void pathEditorSetCamPosBody(nuvec_s *position)
    __asm__("_ZL19pathEditorSetCamPosP7nuvec_si.part.43");

#include "decomp.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "editor/path_connections.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "globals.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"

#include <string.h>
#include <stdio.h>
#include <float.h>

extern "C" {
    i32 aidata_version = 20;
}

extern "C" i32 aieditor_Register(const char *, void (*)(), void (*)(), void (*)(), void (*)());
extern "C" void creatureEditorSaveData(AIPATHSYS_s *);
i32 NuSpecialFindByPlatformID(nugscn_s *, nuhspecial_s *, i32);

void areaEditor_Enter();
eduimenu_s *locatorEditor_Process(nupad_s *);
void creatureEditor_Enter();
eduimenu_s *creatureEditor_Process(nupad_s *);
void antinodeEditor_Enter();

extern "C" {
    i32 AIEDITOR_PATHS = -1;
    i32 AIEDITOR_ROUTES = -1;
    i32 AIEDITOR_AREAS = -1;
    i32 AIEDITOR_LOCATORS = -1;
    i32 AIEDITOR_CREATURES = -1;
    i32 AIEDITOR_ANTINODES = -1;
}

aieditor_settings_s aieditorsettings;
static i16 disable_cylinder_check;
extern "C" {
    extern void *ed_fnt;
}

char *antinode_types[3] = {const_cast<char *>("Circle"), const_cast<char *>("Ellipse"),
                           const_cast<char *>("Rectangle")};

static EDANTINODE_s *antinode_pool() {
    return reinterpret_cast<EDANTINODE_s *>(reinterpret_cast<u8 *>(aieditor) + 0x4088c);
}
static NULISTHDR *antinode_free_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e8c);
}
static NULISTHDR *antinode_list() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
}
static EDANTINODE_s *antinode_selected() {
    return static_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c);
}
static EDANTINODE_s *&antinode_nearest() {
    return *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
}
static i32 &antinode_rotation_repeat() {
    return *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
}

#if defined(__i386__)
#define EDANTINODE_REGPARM1 __attribute__((regparm(1)))
#else
#define EDANTINODE_REGPARM1
#endif
static __attribute__((noinline,
                      force_align_arg_pointer)) EDANTINODE_REGPARM1 EDANTINODE_s *CreateAntinode(nuvec_s *position);

__attribute__((optimize("no-tree-loop-optimize"))) void antinodeEditor_Enter();

static void antinodeEditor_cbSelectType(eduimenu_s *parent, eduiitem_s *, unsigned int);
static void antinodeEditor_cbDeleteAntinode(eduimenu_s *, eduiitem_s *item, unsigned int);
static void antinodeEditor_cbCancelDeleteAntinodeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((force_align_arg_pointer)) EDANTINODE_REGPARM1 void
antinodeEditor_AntinodeMoved(EDANTINODE_s *node);

static __used__ void aieditor_cbSetEditorMode(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void aieditor_cbCancelSelectEditorMode(eduimenu_s *menu, eduimenu_s *);
static __used__ void aieditor_cbCancelSaveMenu(eduimenu_s *, eduiitem_s *, unsigned int);

extern "C" {
    typedef void AIEDITORMOVEPLAYERS(NUVEC *position);
    AIEDITORMOVEPLAYERS *AIEditorMovePlayersFn;

    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);

void InitFn_AIEditorMovePlayers(AIEDITORMOVEPLAYERS *function);

i32 aieditor_Register(const char *name, void (*enter)(), void (*callback_24)(), void (*callback_28)(),
                          void (*leave)());

__attribute__((force_align_arg_pointer)) void aieditor_SetCurrentScript(char *name,
                                                                            const AIEditorScriptSelection *selection);
} // extern "C"

#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/edpath.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nurndr.h"
#include <float.h>
#include <stdio.h>
#include <string.h>
struct EDCREATURE_s;
struct AIPATHSYS_s;

static inline __attribute__((always_inline)) NULISTHDR *creatureEditor_AreaList() {
    return reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
}

static inline __attribute__((always_inline)) NULISTHDR *creatureEditor_LocatorList() {
    return &aieditor->locators;
}

static inline __attribute__((always_inline)) NULISTLNK *creatureEditor_ListItem(NULISTHDR *list, i32 index) {
    if (index < 0)
        return nullptr;
    NULISTLNK *entry = NuLinkedListGetHead(list);
    for (i32 i = 0; entry != nullptr && i < index; ++i) {
        entry = NuLinkedListGetNext(list, entry);
    }
    return entry;
}

static inline __attribute__((always_inline)) CreatureEditorRecord *creatureEditor_Current() {
    return reinterpret_cast<CreatureEditorRecord *>(aieditor->mode_selection_36930);
}

extern i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
extern "C" void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
static eduiitem_s *reset_params_option;
static void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSelectRespawnLocator(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbCancelMenu(eduimenu_s *, eduimenu_s *);
static void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cbResetParams(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *, u32);
static void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *, u32);

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM2 __attribute__((regparm(2)))
#else
#define CREATURE_EDITOR_REGPARM2
#endif
static __used__ CREATURE_EDITOR_REGPARM2 void *CreateCreature(i32 type, nuvec_s *position, i32 angle);

static __attribute__((always_inline, optimize("O3"))) inline void *FindCreatureArea(const char *name) {
    if (name == nullptr)
        return nullptr;
    NULISTHDR *areas = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40);
    EditorNamedEntry *area = (EditorNamedEntry *)NuLinkedListGetHead(areas);
    while (area != nullptr) {
        if (NuStrICmp(area->name, name) == 0)
            return area;
        area = (EditorNamedEntry *)NuLinkedListGetNext(areas, &area->link);
    }
    return nullptr;
}

static __attribute__((always_inline, optimize("O3"))) inline EDLOCATOR_s *FindCreatureLocator(const char *name) {
    if (name == nullptr)
        return nullptr;
    EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
    while (locator != nullptr) {
        if (NuStrICmp(locator->name, name) == 0)
            return locator;
        locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
    }
    return nullptr;
}

__attribute__((optimize("O3"))) void creatureEditor_Enter();

#if defined(__i386__)
#define CREATURE_EDITOR_REGPARM1 __attribute__((regparm(1)))
#else
#define CREATURE_EDITOR_REGPARM1
#endif
static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature);

static __used__ void creatureEditor_cbActivationMenu(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void pathEditor_cbCancelDeleteCreatureMenu(eduimenu_s *, eduimenu_s *);
static __used__ void creatureEditor_cbDeleteCreature(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbFlagsToggle(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbGroupMenu(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbRenameCreature(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbRenameCreatureMenu(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbRespawnMenu(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int);

static __used__ void creatureEditor_cbScriptParams(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbSelectScript(eduimenu_s *parent, eduiitem_s *, unsigned int);

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int);

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *menu, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSelectType(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSelectRespawnLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbSelectLocator(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbSelectTriggerArea(eduimenu_s *parent, eduiitem_s *, unsigned int);
static __used__ void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int);
static __used__ void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *item, unsigned int);

extern "C" {
    f32 aieditor_y_tolerance = 0.1f;

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditorSaveData(AIPATHSYS_s *system);

__attribute__((optimize("O2", "omit-frame-pointer"))) EDCREATURE_s *creatureEditor_GetNearest(i32 use_radius);

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathDeleted(EDAIPATH_s *path);

__attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *node);

__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_PathNodeMoved(EDAIPATHNODE_s *node);

__attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_RenderAllCreatures(void);
} // extern "C"

__attribute__((optimize("O2"), force_align_arg_pointer)) eduimenu_s *creatureEditor_Process(nupad_s *pad);

#include "decomp.h"
#include "editor/edpath_types.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/edpath.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include <string.h>
#include <stdio.h>
#include <float.h>

extern "C" {
    extern void *ed_fnt;
    struct EDCREATURE_s;
    EDCREATURE_s *creatureEditor_GetNearest(i32);
    void creatureEditor_RenderAllCreatures();
    void aieditor_SetCurrentScript(char *, const AIEditorScriptSelection *);
}

extern "C" {
    u8 default_ngroup = 1;
    u8 default_nacross = 2;
    f32 default_xspacing = 0.4f;
    f32 default_zspacing = 0.4f;
    f32 default_stagger_start;
}

static void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *, eduimenu_s *);
static void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);
#if defined(__i386__)
static void __attribute__((regparm(1))) DestroyLocator(EDLOCATOR_s *);
static unsigned int __attribute__((regparm(2))) AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#else
static void DestroyLocator(EDLOCATOR_s *);
static unsigned int AddLocatorToSet(EDLOCATORSET_s *, EDLOCATOR_s *, EDLOCATOR_s *);
#endif
static __used__ void locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *, u32);
static __used__ void locatorEditor_cbDeleteLocator(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbSetLocatorSet(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbRenameLocator(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbEmptyLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbCreateLocatorSet(eduimenu_s *, eduiitem_s *, u32);
static __used__ void locatorEditor_cbDeleteLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbRenameLocatorSet(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbSelectLocatorSet(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void locatorEditor_cbAddLocatorsByName(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbRenameLocatorMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void locatorEditor_cbRenameLocatorSetMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ void locatorEditor_cbAddLocatorsByNameMenu(eduimenu_s *parent, eduiitem_s *, u32);
static __used__ __attribute__((optimize("no-tree-vectorize"))) void
locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void locatorEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *menu, eduimenu_s *);
static __used__ void locatorEditor_cbCancelDeleteLocatorMenu(eduimenu_s *, eduimenu_s *);
static __used__ void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *);
static __used__ __attribute__((optimize("no-tree-vectorize"))) void DestroyLocator(EDLOCATOR_s *locator);

#if defined(__i386__)
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2)))
#else
#define LOCATOR_SET_BODY_ATTR __attribute__((noinline, optimize("no-tree-vectorize")))
#endif
static LOCATOR_SET_BODY_ATTR unsigned int
AddLocatorToSetBody(EDLOCATORSET_s *set, EDLOCATOR_s *locator,
                    EDLOCATOR_s *before) __asm__("_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.part.5");

static inline i32 LocatorSetContains(EDLOCATORSET_s *set, EDLOCATOR_s *locator) {
    for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
        if (set->locators[index] == locator) {
            return 1;
        }
    }
    return 0;
}

static inline void LocatorSetRemove(EDLOCATORSET_s *set, EDLOCATOR_s *locator) {
    for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
        if (set->locators[index] == locator) {
            for (; index < 64; ++index) {
                if (index < 63) {
                    set->locators[index] = set->locators[index + 1];
                } else {
                    set->locators[index] = nullptr;
                }
            }
            return;
        }
    }
}

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before);

#if defined(__i386__)
static __attribute__((noinline, optimize("no-tree-vectorize"), regparm(2))) unsigned int
#else
static __attribute__((noinline, optimize("no-tree-vectorize"))) unsigned int
#endif
AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) __asm__(
    "_ZL15AddLocatorToSetP14EDLOCATORSET_sP11EDLOCATOR_sS2_.constprop.131");

__attribute__((optimize("no-tree-vectorize"))) eduimenu_s *locatorEditor_Process(nupad_s *pad);

#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/render.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
    extern i32 FalconDebKey[2];
}

extern GameObject_s *player_tag_from;
extern GameObject_s *player_tag_to;

#include "legoapi/items/objects/gameobjects_types.h"

extern "C" {
extern i32 nbaddies_can_see_players;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

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
    AIGETCREATUREORIGIN *GetAICreatureOriginFn;
}

static SCRIPT_ERROR_LEVEL ScriptErrorLevel;

struct CUTSCENEPLAYERCLIP;

extern f32 LevelNameTime, LevelNameMul;

extern "C" {
void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;
extern i16 id_ANAKINJEDISCARRED;

extern "C" {
extern i32 party_under_cover;
}

i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);

void DestroySnakeBody(GameObject_s *);
void BlowUpSnakeBody(GameObject_s *);

extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;

void DestroySnakeBody(GameObject_s *obj);

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
extern i8 (*adtab)[4];

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

extern "C" {
    extern i32 freechunkcontrolsptr;
}

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

#include "gameapi/ai/aisys/aisys.h"

#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nustring.h"

#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/fx/parts.h"

struct GIZSPINNER_s;

namespace {

} // namespace

#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/render/fx/parts.h"

#include <stdlib.h>

extern void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
extern i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
extern void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);

#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

#include <string.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

__attribute__((force_align_arg_pointer)) void pathEditorDrawNode(nuvec_s *position, float radius, float lower_height,
                                                                 float upper_height, u32 colour, numtl_s *material,
                                                                 i32 segments, i32 solid);

AIPATHCNXHELPER_s *AIPathCnxHelperSys_Find(AIPATHCNXHELPERSYS_s *system, GameObject_s *object, AIPATHCNX_s *connection,
                                           u8 direction, u8 type,
                                           i32 (*filter)(AIPATHCNXHELPER_s *, GameObject_s *, AIPATHCNX_s *, u8, u8));

#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
struct nuhspecial_s;
struct minitrooperteam_s;

#include "legoapi/legoapi_types.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

void GameAntinode_FindGridPosition(WORLDINFO_s *, NUVEC *, f32, f32, u8 *, u8 *, u8 *, u8 *);

void GameAntinode_FindGridPosition(WORLDINFO_s *world, NUVEC *position, f32 radius_x, f32 radius_z, u8 *min_x,
                                   u8 *min_z, u8 *max_x, u8 *max_z);

#include "decomp.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
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
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

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
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern "C" {
    f32 aiEditor_DrawYOffset = 0.25f;
}

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

extern i16 tNONEWBESTTIME, tNEWBESTTIME;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern "C" {
    extern nurenderscene_s currentScene;
}

#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"

struct AIPACKET_s;
struct APIOBJECT_s;
struct AISYS_s;
struct AIGROUP_s;
struct nufpar_s;

typedef i32 (*MIDSPECIALMOVEFN)(AISYS_s *, AIPACKET_s *, APIOBJECT_s *);

PREPARINGSPECIALMOVEFN PreparingForSpecialMoveFn = NULL;
MIDSPECIALMOVEFN MidSpecialMoveFn = NULL;

extern "C" {
void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function);

void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function);
} // extern "C"

#include "decomp.h"
#include "globals.h"
#include "editor/aieditor_state.h"
#include "editor/aieditor_settings.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "legoapi/ai/game/gameantinode.h"
#include "legoapi/render/core/render.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include <string.h>

#include "decomp.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include <stdio.h>
#include <string.h>

i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);

extern "C" {
    extern void *ed_fnt;
    void creatureEditor_RenderAllCreatures();
}

#include "editor/edtoolsall_types.h"

__attribute__((force_align_arg_pointer)) i32 creatureEditor_CalculatePos(EDCREATURE_s *creature, i32 index,
                                                                         nuvec_s *position, i32 check_path);

#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

i32 geom;

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

extern "C" {
    extern i32 EDPP_MAX_TYPES;
    extern f32 panelglobaltime;
    extern f32 globaltime;
    extern usize debris_trash_space;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freechunkcontrolsptr;
    extern u32 *spherecolldata;
    extern u32 *toruscolldata;
    extern NUMTL *DebMat[10];
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
    extern "C++" {
        void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
        void DebrisCleanUpDmaDebTypeTables(void);
        void DebrisProcessAllocation(void);
        void DebrisProcessControlChunks(i32);
        void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
    }
}

extern "C" {
void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatformRemoveCallback(void (*function)(void *));

void PlatformUpdateCallback(void (*function)(void *), void *argument);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

static i32 AiParseExpressionNameLoopup(char *name, f32 *float_out, i32 *int_out) {
    f32 value;

    ExpressionRequiredNameLookup = 1;
    value = 0.0f;
    ExpressionNameLookupFailed = 1;

    if (GameParamToFloatFn != NULL) {
        if ((*GameParamToFloatFn)(NULL, NULL, name, &value) != 0) {
            ExpressionNameLookupFailed = 0;
        }
    }

    if (float_out != NULL) {
        *float_out = value;
    }

    if (int_out != NULL) {
        *int_out = value;
    }

    return 0;
}

static f32 Condition_Timer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    return processor->script_timer;
}

// API script callbacks and their constant registry share internal linkage.
static f32 Condition_AlwaysTrue(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *void_arg) {
    return *(f32 *)&void_arg;
}

static f32 Condition_LocatorRangeY(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    if (packet != NULL && packet->owner != NULL) {
        if (void_arg == NULL) {
            void_arg = processor->unknown_a4;
        }
        if (void_arg != NULL) {
            AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
            return packet->owner->apiobj.position.y - locator->position.y;
        }
    }
    return 0.0f;
}

static f32 Condition_GotLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return processor->unknown_a4 != NULL ? 1.0f : 0.0f;
}

static f32 Condition_GotLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return processor->unknown_a8 != NULL ? 1.0f : 0.0f;
}

static f32 Condition_OnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->path_info.on_path ? 1.0f : 0.0f;
}

static f32 Condition_PlayerOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return sys->player_1 != NULL && sys->player_1->ai->path_info.on_path ? 1.0f : 0.0f;
}

static f32 Condition_OpponentOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->opponent != NULL &&
                   static_cast<APIOBJECT *>(packet->opponent)->ai->path_info.on_path
               ? 1.0f
               : 0.0f;
}

static f32 Condition_TimeOffPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL ? packet->time_off_path : 0.0f;
}

static f32 Condition_CurrentLocatorIs(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return void_arg != NULL && processor->unknown_a4 == void_arg ? 1.0f : 0.0f;
}

static f32 Condition_InTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                   void *void_arg) {
    if (packet != NULL && packet->owner != NULL) {
        GameObject *object = packet->owner;
        AIAREA *area = static_cast<AIAREA *>(void_arg);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        if (area != NULL && area->system != NULL) {
            i32 index = area - area->system->areas;
            i64 mask = 1 << index;
            u64 membership =
                (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
            if ((membership & mask) != 0) {
                return 1.0f;
            }
        }
    }
    return 0;
}

static f32 Condition_InLevelNode(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *void_arg) {
    if (void_arg != NULL && packet != NULL) {
        if (sys != NULL) {
            AIPATHNODE *node = static_cast<AIPATHNODE *>(void_arg);
            if (packet->inside_path_node == node - sys->path_sys->active_path->nodes) {
                return 1.0f;
            }
        }
    }
    return 0.0f;
}

static f32 Condition_PlayerInLevelNode(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg) {
    if (sys != NULL && sys->player_1 != NULL && void_arg != NULL) {
        AIPATHNODE *node = static_cast<AIPATHNODE *>(void_arg);
        if (sys->player_1->ai->inside_path_node == node - sys->path_sys->active_path->nodes) {
            return 1.0f;
        }
    }
    return 0;
}

static f32 Condition_EitherPlayerInLevelNode(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg) {
    if (void_arg != NULL && sys != NULL) {
        AIPATHNODE *node = static_cast<AIPATHNODE *>(void_arg);
        if (sys->player_1 != NULL && sys->player_1->ai->inside_path_node == node - sys->path_sys->active_path->nodes) {
            return 1.0f;
        }
        if (sys->player_2 != NULL && sys->player_2->ai->inside_path_node == node - sys->path_sys->active_path->nodes) {
            return 1.0f;
        }
    }
    return 0;
}

static f32 Condition_GotTriggerArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return processor->unknown_a0 != NULL ? 1.0f : 0.0f;
}

static f32 Condition_PlayerInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                         void *void_arg) {
    if (sys != NULL && sys->player_1 != NULL && sys->areas != NULL) {
        GameObject *object = reinterpret_cast<GameObject *>(sys->player_1);
        AIAREA *area = static_cast<AIAREA *>(void_arg);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        if (area != NULL && area->system != NULL) {
            i32 index = area - area->system->areas;
            i64 mask = 1 << index;
            u64 membership =
                (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
            if ((membership & mask) != 0) {
                return 1.0f;
            }
        }
    }
    return 0;
}

static f32 Condition_Player2InTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                          void *void_arg) {
    if (sys != NULL && sys->player_2 != NULL) {
        GameObject *object = reinterpret_cast<GameObject *>(sys->player_2);
        AIAREA *area = static_cast<AIAREA *>(void_arg);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        if (area != NULL && area->system != NULL) {
            i32 index = area - area->system->areas;
            i64 mask = 1 << index;
            u64 membership =
                (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
            if ((membership & mask) != 0) {
                return 1.0f;
            }
        }
    }
    return 0;
}

static f32 Condition_EitherPlayerInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *,
                                               void *void_arg) {
    if (sys != NULL) {
        AIAREA *area = static_cast<AIAREA *>(void_arg);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        if (area != NULL) {
            if (sys->player_1 != NULL && area->system != NULL) {
                GameObject *object = reinterpret_cast<GameObject *>(sys->player_1);
                i32 index = area - area->system->areas;
                i64 mask = 1 << index;
                u64 membership =
                    (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
                if ((membership & mask) != 0) {
                    return 1.0f;
                }
            }
            if (sys->player_2 != NULL && area->system != NULL) {
                GameObject *object = reinterpret_cast<GameObject *>(sys->player_2);
                i32 index = area - area->system->areas;
                i64 mask = 1 << index;
                u64 membership =
                    (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
                if ((membership & mask) != 0) {
                    return 1.0f;
                }
            }
        }
    }
    return 0;
}

static f32 Condition_BaddyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *void_arg) {
    AIAREA *area = static_cast<AIAREA *>(void_arg);
    if (area == NULL) {
        area = processor->unknown_a0;
    }
    return area != NULL && (area->runtime_flags & AIAREA_RUNTIME_OBJECT_STATE_SET) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_GoodyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *void_arg) {
    AIAREA *area = static_cast<AIAREA *>(void_arg);
    if (area == NULL) {
        area = processor->unknown_a0;
    }
    return area != NULL && (area->runtime_flags & AIAREA_RUNTIME_OBJECT_STATE_CLEAR) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_OpponentInTriggerArea(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                           void *void_arg) {
    if (packet != NULL && packet->opponent != NULL && packet->owner != NULL) {
        GameObject *object = static_cast<GameObject *>(packet->opponent);
        AIAREA *area = static_cast<AIAREA *>(void_arg);
        if (area == NULL) {
            area = processor->unknown_a0;
        }
        if (area != NULL && area->system != NULL) {
            i32 index = area - area->system->areas;
            i64 mask = 1 << index;
            u64 membership =
                (static_cast<u64>(object->apiobj.ai_area_mask_high) << 32) | object->apiobj.ai_area_mask_low;
            if ((membership & mask) != 0) {
                return 1.0f;
            }
        }
    }
    return 0;
}

static f32 Condition_GotOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->opponent != NULL ? 1.0f : 0.0f;
}

static f32 Condition_OpponentOnSamePath(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    if (packet != NULL && packet->opponent_object != NULL && packet->path_info.path != NULL) {
        AIPACKET *opponent = packet->opponent_object->ai;
        if (opponent != NULL && opponent->path_info.path == packet->path_info.path) {
            return 1.0f;
        }
    }
    return 0.0f;
}

static f32 Condition_OpponentRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return packet != NULL && packet->opponent_object != NULL ? packet->opponent_metric : 3.402823466e+38f;
}

static f32 Condition_NearestOpponentRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                          void *void_arg) {
    return packet != NULL && packet->nearest_opponent_object != NULL ? packet->nearest_opponent_metric
                                                                     : 3.402823466e+38f;
}

static f32 Condition_OpponentBelow(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return packet != NULL && packet->opponent_object != NULL &&
                   packet->opponent_object->position.y < packet->owner->apiobj.position.y - 0.1f
               ? 1.0f
               : 0.0f;
}

static f32 Condition_OpponentToLocatorY(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                        void *void_arg) {
    if (packet != NULL && packet->opponent_object != NULL) {
        AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
        if (locator == NULL) {
            locator = processor->locator;
        }
        if (locator != NULL) {
            return packet->opponent_object->position.y - locator->position.y;
        }
    }
    return FLT_MAX;
}

static f32 Condition_PathBlocked(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    // The script condition tests the waypoint bit (0x40), not the route-failure bit (0x20).
    return packet != NULL && (packet->runtime_flags & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0 ? 1.0f : 0.0f;
}

__used__ static f32 Condition_InterruptID(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    if (processor != NULL) {
        return static_cast<u32>(processor->interrupt_id);
    }
    return -1.0f;
}

static void *Condition_IAmInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL && GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(sys, arg) : NULL;
}

static f32 Condition_StuckTime(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL ? packet->owner->apiobj.respawn_timer : 0.0f;
}

__used__ static i32 Action_ReturnToState(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **, i32, i32, f32) {
    if (processor != NULL && processor->return_to_state != NULL) {
        processor->next_state = processor->return_to_state;
        processor->return_to_state = NULL;
    }
    return 1;
}

static void FormationMove(AIGROUP *group, i32 (*move)(AIGROUP *, AIROW *, AIROW *, APIOBJECT *)) {
    i32 turning = 0;
    if (move != NULL && group->is_in_formation) {
        AIROW *previous = NULL;
        if (group->is_reversed) {
            for (i32 i = group->row_count - 1; i != -1; --i) {
                AIROW *row = &group->rows[i];
                if (row->is_alive) {
                    if (move(group, row, previous, group->leader) != 0)
                        break;
                    if (row->is_turning)
                        turning = 1;
                    previous = row;
                }
            }
        } else {
            for (i32 i = 0; i < group->row_count; ++i) {
                AIROW *row = &group->rows[i];
                if (row->is_alive) {
                    if (move(group, row, previous, group->leader) != 0)
                        break;
                    if (row->is_turning)
                        turning = 1;
                    previous = row;
                }
            }
        }
    }
    group->is_row_turning = turning;
}

static __used__ void areaEditor_cbAreaCylinderToggle(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    if (selected != NULL) {
        if (item->flags & EDUI_ITEM_HIGHLIGHTED) {
            selected->flags |= 1;
        } else {
            selected->flags &= ~1;
        }
    }
}

static __used__ void pathEditor_cbDrawWallsplinesToggle(eduimenu_s *, eduiitem_s *item, u32) {
    aieditorsettings.draw_wallsplines = item->highlighted;
}

static __used__ void pathEditor_cbCnxFlagsToggle(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    i32 type_index = item->data;
    if (path == nullptr) {
        return;
    }
    EDAIPATHNODE_s *other = path->other_node;
    EDAIPATHNODE_s *node = path->current_node;
    if (other == nullptr || node == nullptr) {
        return;
    }
    i32 i;
    if (node->connections[0].node == other)
        i = 0;
    else if (node->connections[1].node == other)
        i = 1;
    else if (node->connections[2].node == other)
        i = 2;
    else if (node->connections[3].node == other)
        i = 3;
    else if (node->connections[4].node == other)
        i = 4;
    else if (node->connections[5].node == other)
        i = 5;
    else if (node->connections[6].node == other)
        i = 6;
    else if (node->connections[7].node == other)
        i = 7;
    else
        return;
    AIPATHCNXTYPE_s *type = &aipathcnxtypes[type_index];
    EDAIPATHCNX_s *connection = &node->connections[i];
    u32 flags = connection->flags;
    u32 mask = type->connection_flag;
    u32 updated_flags = flags | mask;
    if (flags & mask)
        updated_flags = flags & ~mask;
    connection->flags = updated_flags;
    if (type->flags != 0) {
        i32 j;
        if (other->connections[0].node == node)
            j = 0;
        else if (other->connections[1].node == node)
            j = 1;
        else if (other->connections[2].node == node)
            j = 2;
        else if (other->connections[3].node == node)
            j = 3;
        else if (other->connections[4].node == node)
            j = 4;
        else if (other->connections[5].node == node)
            j = 5;
        else if (other->connections[6].node == node)
            j = 6;
        else if (other->connections[7].node == node)
            j = 7;
        else
            return;
        if (!(connection->flags & mask)) {
            other->connections[j].flags &= ~mask;
        } else {
            other->connections[j].flags |= mask;
        }
    }
}

static __used__ void routeEditor_cbSetRouteUsers(eduimenu_s *, eduiitem_s *item, u32) {
    if (item == nullptr || aieditor->current_path == nullptr) {
        return;
    }
    EDAIPATHROUTE_s *route = aieditor->current_path->current_route;
    if (route == nullptr || (u32)item->data >= 64) {
        return;
    }
    if ((route->user_mask >> item->data) & 1) {
        route->user_mask &= ~(1ULL << item->data);
        item->highlighted = 0;
    } else {
        route->user_mask |= 1ULL << item->data;
        item->highlighted = 1;
    }
}

extern "C" {
    void aieditor_cbDrawAllToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.draw_all_paths = item->highlighted;
    }

    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.snap_height_display = item->highlighted;
    }

    void aieditor_cbSolidPathDisplayToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.solid_path_display = item->highlighted;
    }

    void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.solid_antinode_display = item->highlighted;
    }

    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.stop_platforms = item->highlighted;
    }

    void aieditor_cbShowCreaturesToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.show_creatures_display = item->highlighted;
    }

    void aieditor_cbShowCreaturesSetToggle(eduimenu_s *, eduiitem_s *item, u32) {
        aieditorsettings.show_creatures_set = item->highlighted;
    }

}

static void antinodeEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDANTINODE_s *node = antinode_selected();
    i32 previous_type = node->type;
    if (item != nullptr)
        node->type = static_cast<u8>(item->data);
    node = antinode_selected();
    if (node->type == 0) {
        if (previous_type != 0)
            node->radius = node->base_radius > node->base_height ? node->base_radius : node->base_height;
    } else if (previous_type == 0) {
        node->base_height = node->radius;
        node->base_radius = node->radius;
    }
}

static __used__ void creatureEditor_cbSetActivation(eduimenu_s *, eduiitem_s *item, unsigned int) {
    creatureEditor_Current()->activation = item->data;
}

static __used__ void creatureEditor_cb_difficulty(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->difficulty = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
}

static __used__ void creatureEditor_cb_stagger_start(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->stagger_start = reinterpret_cast<edui_slider_s *>(item)->value;
}

static __used__ void creatureEditor_cb_assigntoset(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->set = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
}

static __used__ void creatureEditor_cb_maxviewheight(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->max_view_height = reinterpret_cast<edui_slider_s *>(item)->value;
}

static __used__ void creatureEditor_cb_minviewheight(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr)
        creature->negative_min_view_height = -reinterpret_cast<edui_slider_s *>(item)->value;
}

static __used__ void creatureEditor_cbFlagsToggle(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        if (creature->flags & item->data) {
            creature->flags &= ~item->data;
        } else {
            creature->flags |= item->data;
        }
    }
}

static f32 Condition_YawToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    if (packet == NULL || packet->opponent_object == NULL) {
        return -180.0f;
    }
    NUVEC relative;
    // Only the horizontal components participate in this angle calculation.
    relative.x = packet->opponent_object->position.x - packet->owner->apiobj.position.x;
    relative.z = packet->opponent_object->position.z - packet->owner->apiobj.position.z;
    NuVecRotateY(&relative, &relative, -packet->owner->apiobj.field_0x276);
    return static_cast<f32>(NuAtan2D(relative.x, relative.z)) * (360.0f / 65536.0f);
}

static void AISysCheckAntinode_Ellipse(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius) {
    f32 distance_squared = difference->x * difference->x + difference->z * difference->z;
    if (distance_squared < radius * radius) {
        f32 aspect = antinode->base_height / antinode->base_radius;
        NUVEC local_position;
        NUVEC boundary;
        difference->y = 0.0f;
        NuVecRotateY(&local_position, difference, -antinode->flags);
        i32 angle = NuAtan2D(aspect * local_position.x, local_position.z);
        boundary.x = NU_SIN_LUT(angle) * antinode->base_radius;
        boundary.y = 0.0f;
        boundary.z = NU_COS_LUT(angle) * antinode->base_height;
        NuVecNorm(difference, &boundary);
        NuVecScale(difference, difference, object->ai->mover_height);
        NuVecAdd(&boundary, &boundary, difference);
        if (distance_squared < boundary.x * boundary.x + boundary.z * boundary.z) {
            NUVEC local_object;
            NUVEC object_boundary;
            NUVEC local_destination;
            difference->x = object->position.x - antinode->position.x;
            difference->y = 0.0f;
            difference->z = object->position.z - antinode->position.z;
            NuVecRotateY(&local_object, difference, -antinode->flags);
            i32 object_angle = NuAtan2D(aspect * local_object.x, local_object.z);
            object_boundary.x = NU_SIN_LUT(object_angle) * antinode->base_radius;
            object_boundary.y = 0.0f;
            object_boundary.z = NU_COS_LUT(object_angle) * antinode->base_height;
            NuVecNorm(difference, &object_boundary);
            NuVecScale(difference, difference, object->ai->mover_height);
            NuVecAdd(&object_boundary, &object_boundary, difference);
            difference->x = object->ai->movement_destination.x - antinode->position.x;
            difference->y = 0.0f;
            difference->z = object->ai->movement_destination.z - antinode->position.z;
            NuVecRotateY(&local_destination, difference, -antinode->flags);
            i32 turn = NuAngSub(NuAtan2D(aspect * local_destination.x, local_destination.z), object_angle);
            if (object->ai->antinode_timer > 0.0f) {
                if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
                    object->ai->antinode_timer < antinode_time) {
                    object->ai->antinode_clockwise = !object->ai->antinode_clockwise;
                    object->ai->antinode_timer = antinode_time + antinode_reverse_time;
                }
                if (object->ai->antinode_clockwise) {
                    if (turn < 0) {
                        turn += 0x10000;
                    }
                } else if (turn > 0) {
                    turn -= 0x10000;
                }
            } else if (turn > 0) {
                object->ai->antinode_clockwise = 1;
            } else {
                object->ai->antinode_clockwise = 0;
            }
            if ((antinode->game_flags & 2) == 0 && object->ai->antinode_timer < antinode_time) {
                object->ai->antinode_timer = antinode_time;
            }
            f32 tangent_x = NU_COS_LUT(object_angle) * antinode->base_radius;
            f32 tangent_z = -NU_SIN_LUT(object_angle) * antinode->base_height;
            if (turn < 0) {
                tangent_x = -tangent_x;
                tangent_z = -tangent_z;
            }
            i32 absolute_turn = turn < 0 ? -turn : turn;
            if (absolute_turn < 1820) {
                f32 scale = static_cast<f32>(absolute_turn) / 1820.0f;
                tangent_x *= scale;
                tangent_z *= scale;
            }
            object->ai->movement_position.y = 0.0f;
            object->ai->movement_position.x = object_boundary.x + tangent_x;
            object->ai->movement_position.z = object_boundary.z + tangent_z;
            NuVecRotateY(&object->ai->movement_position, &object->ai->movement_position, antinode->flags);
            NuVecAdd(&object->ai->movement_position, &object->ai->movement_position, &antinode->position);
            object->field_0x1fa |= 4;
        }
    }
}

static f32 Condition_IAm(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char *arg, void *void_arg) {
    if (packet != NULL && packet->owner != NULL) {
        if (packet->owner == void_arg) {
            return 1.0f;
        }
        characterdata_s *character = packet->owner->apiobj.character_data;
        if (character != NULL && character->file != NULL && NuStrICmp(character->file, arg) == 0) {
            return 1.0f;
        }
    }
    return 0.0f;
}

static f32 Condition_Random(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return NuRandFloat();
}

static f32 Condition_NearestPlayerXZRange(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                          void *void_arg) {
    NUVEC difference;
    f32 distance = 3.402823466e+38f;
    if (packet != NULL && packet->owner != NULL && sys != NULL) {
        if (sys->player_1 != NULL) {
            distance = NuVecXZDistSqr(&sys->player_1->position, &packet->owner->apiobj.position, &difference);
        }
        if (sys->player_2 != NULL) {
            f32 second = NuVecXZDistSqr(&sys->player_2->position, &packet->owner->apiobj.position, &difference);
            distance = second < distance ? second : distance;
        }
        if (distance != 3.402823466e+38f) {
            distance = NuFsqrt(distance);
        }
    }
    return distance;
}

static f32 Condition_NearestPlayerRange(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                        void *void_arg) {
    NUVEC difference;
    f32 distance = 3.402823466e+38f;
    if (packet != NULL && packet->owner != NULL && sys != NULL) {
        if (sys->player_1 != NULL) {
            distance = NuVecDistSqr(&sys->player_1->position, &packet->owner->apiobj.position, &difference);
        }
        if (sys->player_2 != NULL) {
            f32 second = NuVecDistSqr(&sys->player_2->position, &packet->owner->apiobj.position, &difference);
            distance = second < distance ? second : distance;
        }
        if (distance != 3.402823466e+38f) {
            distance = NuFsqrt(distance);
        }
    }
    return distance;
}

static f32 Condition_OpponentToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                       void *void_arg) {
    NUVEC difference;
    if (packet != NULL && packet->opponent_object != NULL) {
        if (void_arg == NULL) {
            void_arg = processor->unknown_a4;
        }
        if (void_arg != NULL) {
            AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
            return NuVecDist(&packet->opponent_object->position, &locator->position, &difference);
        }
    }
    return 3.402823466e+38f;
}

static f32 Condition_LevelNodeRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *void_arg) {
    NUVEC difference;
    if (packet != NULL && packet->owner != NULL && void_arg != NULL) {
        AIPATHNODE *node = static_cast<AIPATHNODE *>(void_arg);
        return NuVecDist(&node->position, &packet->owner->apiobj.position, &difference);
    }
    return 3.402823466e+38f;
}

static f32 Condition_PlayerRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet == NULL || packet->owner == NULL || sys == NULL || sys->player_1 == NULL) {
        return FLT_MAX;
    }
    NUVEC delta;
    return NuVecDist(&sys->player_1->position, &packet->owner->apiobj.position, &delta);
}

static f32 Condition_LocatorRange(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    NUVEC difference;
    AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
    if (packet != NULL && packet->owner != NULL) {
        if (locator == NULL) {
            locator = processor->unknown_a4;
        }
        if (locator != NULL) {
            return NuVecDist(&packet->terrain_origin, &locator->position, &difference);
        }
    }
    return 3.402823466e+38f;
}

static void pathEditorCalcRouteIterator(AIPATH_s *path, f32 *distances, u8 *visited, i32 previous, i32 current,
                                        f32 distance, i32 route_mask) {
    ++iterator_count;
    if (__builtin_expect((visited[current / 8] & (1 << (current % 8))) != 0, 0)) {
        return;
    }
    visited[current / 8] |= 1 << (current % 8);
    AIPATHNODE_s *node = &path->nodes[current];
    NUVEC difference;
    distance += NuVecDist(&path->nodes[previous].position, &node->position, &difference);
    if (distances[current] >= distance) {
        distances[current] = distance;
        if (__builtin_expect(route_mask != 0, 1)) {
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX_s *connection = node->connections[index];
                bool direction = connection->node_indices[0] != current;
                if ((connection->traversal_flags[direction] & 0x40000000) == 0 &&
                    (connection->route_mask & route_mask) != 0) {
                    pathEditorCalcRouteIterator(path, distances, visited, current, connection->node_indices[!direction],
                                                distance, route_mask);
                }
            }
        } else {
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX_s *connection = node->connections[index];
                bool direction = connection->node_indices[0] != current;
                if (__builtin_expect((connection->traversal_flags[direction] & 0x40000000) == 0, 1)) {
                    pathEditorCalcRouteIterator(path, distances, visited, current, connection->node_indices[!direction],
                                                distance, 0);
                }
            }
        }
    }
    visited[current / 8] &= ~(1 << (current % 8));
}

static f32 Condition_OpponentToLocatorXZ(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *,
                                         void *void_arg) {
    NUVEC difference;
    AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
    if (packet != NULL && packet->opponent_object != NULL) {
        if (locator == NULL) {
            locator = processor->unknown_a4;
        }
        if (locator != NULL) {
            return NuVecXZDist(&packet->opponent_object->position, &locator->position, &difference);
        }
    }
    return 3.402823466e+38f;
}

static f32 Condition_LocatorRangeXZ(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    NUVEC difference;
    AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
    if (packet != NULL && packet->owner != NULL) {
        if (locator == NULL) {
            locator = processor->unknown_a4;
        }
        if (locator != NULL) {
            return NuVecXZDist(&packet->terrain_origin, &locator->position, &difference);
        }
    }
    return 3.402823466e+38f;
}

static void antinodeEditor_cbAntiNodeFlagsToggle(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDANTINODE_s *node = antinode_selected();
    if (node == nullptr)
        return;
    u8 previous_flags = node->game_flags;
    u32 mask = item->data;
    u32 next_flags = previous_flags | mask;
    if (previous_flags & mask)
        node->game_flags = previous_flags & ~mask;
    else
        node->game_flags = next_flags;
    node = antinode_selected();
    if (static_cast<i8>(node->game_flags) < 0)
        memset(&node->special, 0, sizeof(node->special));
}

static __used__ void pathEditor_cbNodeFlagsToggle(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATHNODE_s *node = aieditor->current_path->current_node;
    if (node != nullptr) {
        u8 flags = node->flags;
        u32 mask = item->data;
        if ((u8)flags & mask) {
            flags &= ~mask;
        } else {
            flags |= mask;
        }
        node->flags = flags;
        if ((i8)aieditor->current_path->current_node->flags < 0) {
            node->platform.scene = nullptr;
            node->platform.special = nullptr;
            node->platform.display_special = nullptr;
        }
    }
}

static __used__ void creatureEditor_cbSetRespawnLocator(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    if (item->data == -1) {
        creature->locator = nullptr;
        return;
    }
    NULISTLNK *locator = creatureEditor_ListItem(creatureEditor_LocatorList(), item->data);
    if (locator != nullptr)
        creatureEditor_Current()->respawn_locator = locator;
}

static __used__ void creatureEditor_cbSetAreaActivation(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (__builtin_expect(item == nullptr || creatureEditor_Current() == nullptr, 0))
        return;
    NULISTLNK *area = creatureEditor_ListItem(creatureEditor_AreaList(), item->data);
    if (area != nullptr) {
        CreatureEditorRecord *creature = creatureEditor_Current();
        creature->activation_area = area;
        creature->activation = 1;
    }
}

static __used__ void creatureEditor_cbSetLocator(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    if (item->data == -1) {
        creature->locator = nullptr;
        return;
    }
    NULISTHDR *list = creatureEditor_LocatorList();
    i32 index = 0;
    for (NULISTLNK *locator = NuLinkedListGetHead(list); locator != nullptr;
         locator = NuLinkedListGetNext(list, locator)) {
        if (*reinterpret_cast<void **>(reinterpret_cast<u8 *>(locator) + 0x2c) == creatureEditor_Current()->path) {
            if (index == item->data) {
                creatureEditor_Current()->locator = locator;
                return;
            }
            ++index;
        }
    }
}

static __used__ void creatureEditor_cbSetTriggerArea(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    if (item->data == -1) {
        creature->trigger_area = nullptr;
        return;
    }
    NULISTLNK *area = creatureEditor_ListItem(creatureEditor_AreaList(), item->data);
    if (area != nullptr)
        creatureEditor_Current()->trigger_area = area;
}

static __used__ void pathEditor_cbRenameNode(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_node == nullptr) {
        return;
    }
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (input->name[0] == '\0') {
        memset(path->current_node->name, 0, sizeof(path->current_node->name));
        return;
    }
    EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
    while (node != nullptr) {
        if (NuStrICmp(node->name, input->name) == 0) {
            return;
        }
        node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link);
    }
    strcpy(aieditor->current_path->current_node->name, input->name);
}

static __used__ CREATURE_EDITOR_REGPARM2 void *CreateCreature(i32 type, nuvec_s *position, i32 angle) {
    if (type == -1) {
        return nullptr;
    }
    NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
    CreatureEditorRecord *creature = (CreatureEditorRecord *)NuLinkedListGetHead(free_creatures);
    if (creature == nullptr) {
        return nullptr;
    }
    NuLinkedListRemove(free_creatures, &creature->link);
    NuLinkedListAppend(&aieditor->creatures, &creature->link);
    creature->character_type = type;
    creature->set = 0;
    creature->group_count = default_ngroup;
    creature->across_count = default_nacross;
    creature->x_spacing = default_xspacing;
    creature->z_spacing = default_zspacing;
    creature->stagger_start = default_stagger_start;
    creature->view_distance = GetViewRangeFn != nullptr ? GetViewRangeFn(type) : 1.0f;
    creature->hear_distance = GetHearDistanceFn != nullptr ? GetHearDistanceFn(type) : 1.0f;
    creature->max_view_height = GetMaxViewHeightFn != nullptr ? GetMaxViewHeightFn(type) : 1.0f;
    if (__builtin_expect(GetMinViewHeightFn != nullptr, 0)) {
        creature->negative_min_view_height = GetMinViewHeightFn(type);
    } else {
        creature->negative_min_view_height = 1.0f;
    }
    creature->difficulty = default_activate_difficulty;
    creature->min_respawns = default_min_n_respawns;
    creature->max_respawns = default_max_n_respawns;
    creature->min_respawn_time = default_min_t_respawn;
    creature->max_respawn_time = default_max_t_respawn;
    if (__builtin_expect(position != nullptr, 1)) {
        creature->position = *position;
        creature->angle = angle;
    }
    return creature;
}

static __attribute__((noinline,
                      force_align_arg_pointer)) EDANTINODE_REGPARM1 EDANTINODE_s *CreateAntinode(nuvec_s *position) {
    EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(antinode_free_list()));
    if (node == nullptr)
        return nullptr;
    NuLinkedListRemove(antinode_free_list(), &node->link);
    NuLinkedListAppend(antinode_list(), &node->link);
    node->position = *position;
    EDANTINODE_s *selected = antinode_selected();
    if (selected != nullptr) {
        node->radius = selected->radius;
        node->lower_height = selected->lower_height;
        node->upper_height = selected->upper_height;
        node->type = selected->type;
        selected = antinode_selected();
        node->base_radius = selected->base_radius;
        node->base_height = selected->base_height;
    } else {
        node->radius = 0.25f;
        node->lower_height = -default_path_heighttol;
        node->upper_height = default_path_heighttol;
    }
    return node;
}

__attribute__((optimize("no-tree-loop-optimize"))) void antinodeEditor_Enter() {
    antinode_list()->head = nullptr;
    antinode_list()->tail = nullptr;
    for (i32 i = 0; i < 128; ++i)
        NuLinkedListAppend(antinode_free_list(), &antinode_pool()[i].link);
    AISYS_s *system = aieditor->ai_system;
    for (i32 i = 0; i < aieditor->ai_system->antinode_count; ++i) {
        AIANTINODE *source = &aieditor->ai_system->antinodes[i];
        EDANTINODE_s *node = CreateAntinode(&source->position);
        if (node == nullptr)
            continue;
        node->position = source->position;
        node->radius = source->radius;
        node->lower_height = source->min_y - source->position.y;
        node->upper_height = source->max_y - source->position.y;
        node->game_flags = source->game_flags;
        node->special = source->special_handle;
        node->special_position = source->special_position;
        node->rotation_offset = source->rotation_offset;
        node->flags = source->flags;
        node->base_radius = source->base_radius;
        node->base_height = source->base_height;
        node->type = source->type;
    }
}

static __used__ void pathEditor_DestroySharedNode(EDAISHAREDPATHNODE_s *shared) {
    if (shared != NULL) {
        for (EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); path != NULL;
             path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&aieditor->current_path->nodes);
                 node != NULL;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link)) {
                if (node->shared_node == shared) {
                    node->shared_node = NULL;
                    break;
                }
            }
        }
        NuLinkedListRemove(&aieditor->shared_path_nodes, &shared->link);
        NuLinkedListAppend(&aieditor->free_shared_path_nodes, &shared->link);
    }
}

static __used__ void locatorEditor_cbAddLocatorsByName(eduimenu_s *, eduiitem_s *item, u32) {
    if (aieditor->current_locator_set != nullptr && ((edui_textpicker_s *)item)->value[0] != 0) {
        NuStrNCpy(aieditor->pending_locator_name, ((edui_textpicker_s *)item)->value, 16);
    }
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbVisionMenu(eduimenu_s *parent,
                                                                                          eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Vision");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_viewdistance, 0.5f, 49.5f,
                                               creatureEditor_Current()->view_distance, "View Distance"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_heardistance, 0.5f, 49.5f,
                                               creatureEditor_Current()->hear_distance, "Hearing Distance"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_maxviewheight, 0.1f, 99.9f,
                                               creatureEditor_Current()->max_view_height, "Max View Height"));
    eduiMenuAddItem(menu,
                    eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_minviewheight, 0.1f, 99.9f,
                                         -creatureEditor_Current()->negative_min_view_height, "Min View Height -"));
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbGroupMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    if (creatureEditor_Current() == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Group Values");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_ngroup, 1, 31,
                                                  creatureEditor_Current()->group_count, "Group Size"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_stagger_start, 0.0f, 60.0f,
                                               creatureEditor_Current()->stagger_start, "Stagger Time"));
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_nacross, 1, 31,
                                                  creatureEditor_Current()->across_count, "Formation Width"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_xspacing, 0.2f, 4.8f,
                                               creatureEditor_Current()->x_spacing, "Formation X Spacing"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_zspacing, 0.2f, 4.8f,
                                               creatureEditor_Current()->z_spacing, "Formation Z Spacing"));
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbRespawnMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Respawn Values");
    if (menu == nullptr)
        return;
    if (NuLinkedListGetHead(creatureEditor_LocatorList()) != nullptr) {
        char label[64];
        if (creatureEditor_Current()->respawn_locator != nullptr) {
            EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(creatureEditor_Current()->respawn_locator);
            sprintf(label, "Respawn Locator \"%s\"", locator->name);
        } else {
            strcpy(label, "Respawn Locator NONE");
        }
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbSelectRespawnLocator, label));
    }
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_min_n_respawns, -1, 33,
                                                  static_cast<i8>(creatureEditor_Current()->min_respawns),
                                                  "Min Num Respawns"));
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_max_n_respawns, -1, 33,
                                                  static_cast<i8>(creatureEditor_Current()->max_respawns),
                                                  "Max Num Respawns"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_min_t_respawn, 0.0f, 60.0f,
                                               creatureEditor_Current()->min_respawn_time, "Min Respawn Time"));
    eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cb_max_t_respawn, 0.0f, 60.0f,
                                               creatureEditor_Current()->max_respawn_time, "Max Respawn Time"));
    eduiMenuAttach(parent, menu);
}

static __used__ void pathEditor_cbShareNodeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *current = aieditor->current_path;
    if (current == nullptr || current->current_node == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelSelectMenu, (char *)"Share node with...");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *all_paths = eduiItemSelCreate(0, &attr, 0, 1, pathEditor_cbSetShareNode, (char *)"All paths");
    eduiMenuAddItem(menu, all_paths);

    i32 index = 1;
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    while (path != nullptr) {
        if (path != aieditor->current_path) {
            i32 selected = 0;
            if (aieditor->current_path->current_node->shared_node != nullptr) {
                EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
                while (node != nullptr) {
                    if (node->shared_node == aieditor->current_path->current_node->shared_node) {
                        selected = 1;
                        break;
                    }
                    node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
                }
            }
            eduiitem_s *item =
                eduiItemToggleCreate(index, &attr, selected, index + 1, pathEditor_cbSetShareNode, path->name);
            eduiMenuAddItem(menu, item);
            ++index;
        }
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void pathEditor_cbSelectPathMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelSelectMenu, (char *)"Select Path");
    if (menu == nullptr) {
        return;
    }
    i32 index = 0;
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    while (path != nullptr) {
        eduiitem_s *item = eduiItemSelCreate(index, &attr, 0, 0, pathEditor_cbSetCurrentPath, path->name);
        eduiMenuAddItem(menu, item);
        ++index;
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void locatorEditor_cbSelectLocatorSet(eduimenu_s *parent, eduiitem_s *, u32) {
    eduimenu_s *menu = eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelSelectLocatorSet,
                                      (char *)"Select Locator Set");
    if (menu == nullptr) {
        return;
    }
    eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, locatorEditor_cbSetLocatorSet, (char *)"NONE"));
    i32 index = 1;
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        eduiMenuAddItem(menu, eduiItemSelCreate(index, attr, 0, 0, locatorEditor_cbSetLocatorSet, set->name));
        ++index;
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cb_heardistance(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->view_distance;
        bool exceeds_limit = value > limit;
        creature->hear_distance = value;
        if (exceeds_limit) {
            creature->hear_distance = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_viewdistance(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->hear_distance;
        bool below_limit = value < limit;
        creature->view_distance = value;
        if (below_limit) {
            creature->view_distance = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_max_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->min_respawn_time;
        bool below_limit = value < limit;
        creature->max_respawn_time = value;
        if (below_limit) {
            creature->max_respawn_time = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_min_t_respawn(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        f32 value = slider->value;
        f32 limit = creature->max_respawn_time;
        bool exceeds_limit = value > limit;
        creature->min_respawn_time = value;
        if (exceeds_limit) {
            creature->min_respawn_time = limit;
            eduiItemSliderSetVal(slider, limit);
        }
    }
}

static __used__ void creatureEditor_cb_max_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        creature->max_respawns = static_cast<i32>(slider->value);
        if (static_cast<i8>(creature->max_respawns) < creature->min_respawns) {
            creature->max_respawns = creature->min_respawns;
            eduiItemSliderSetVal(slider, static_cast<i8>(creature->max_respawns));
        }
    }
}

static __used__ void creatureEditor_cb_min_n_respawns(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        edui_slider_s *slider = reinterpret_cast<edui_slider_s *>(item);
        creature->min_respawns = static_cast<i32>(slider->value);
        if (static_cast<i8>(creature->min_respawns) > creature->max_respawns) {
            creature->min_respawns = creature->max_respawns;
            eduiItemSliderSetVal(slider, static_cast<i8>(creature->min_respawns));
        }
    }
}

static __used__ void creatureEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static void antinodeEditor_cbCancelMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void aieditor_cbCancelSelectEditorMode(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void routeEditor_cbCancelRouteUsers(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void routeEditor_cbCancelRenameRouteMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbCancelRenamePathMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbCancelSelectMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void pathEditor_cbCancelRenameNodeMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void locatorEditor_cbCancelSelectLocatorSet(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void locatorEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void areaEditor_cbCancelRenameMenu(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
}

static __used__ void creatureEditor_cbSelectRespawnLocator(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Locator");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    eduiMenuAddItem(menu, eduiItemCheckCreate(-1, attr, creature->respawn_locator == nullptr, 1,
                                              creatureEditor_cbSetRespawnLocator, "NONE"));
    i32 index = 0;
    for (NULISTLNK *link = NuLinkedListGetHead(creatureEditor_LocatorList()); link != nullptr;
         link = NuLinkedListGetNext(creatureEditor_LocatorList(), link)) {
        EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(link);
        if (creatureEditor_Current()->respawn_locator == locator) {
            eduiMenuAddItem(menu,
                            eduiItemCheckCreate(index, attr, 1, 1, creatureEditor_cbSetRespawnLocator, locator->name));
            menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(menu,
                            eduiItemCheckCreate(index, attr, 0, 1, creatureEditor_cbSetRespawnLocator, locator->name));
        }
        ++index;
        eduiMenuAttach(parent, menu);
    }
}

static __used__ void creatureEditor_cbActivationMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    if (creatureEditor_Current() == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Activation Condition");
    if (menu == nullptr)
        return;
    eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_difficulty, 1, 9,
                                                  creatureEditor_Current()->difficulty, "Activation Difficulty"));
    eduiMenuAddItem(menu, eduiItemCheckCreate(0, attr, creatureEditor_Current()->activation == 0, 1,
                                              creatureEditor_cbSetActivation, "AUTOMATIC"));
    eduiMenuAddItem(menu, eduiItemCheckCreate(2, attr, creatureEditor_Current()->activation == 2, 1,
                                              creatureEditor_cbSetActivation, "SCRIPT"));
    i32 index = 0;
    char label[64];
    for (NULISTLNK *link = NuLinkedListGetHead(creatureEditor_AreaList()); link != nullptr;
         link = NuLinkedListGetNext(creatureEditor_AreaList(), link), ++index) {
        if (creatureEditor_Current()->activation_area == link) {
            sprintf(label, "AREA \"%s\"", reinterpret_cast<char *>(link) + 8);
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 1, 1, creatureEditor_cbSetAreaActivation, label));
            menu->selected = edui_last_item;
        } else {
            sprintf(label, "AREA \"%s\"", reinterpret_cast<char *>(link) + 8);
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 0, 1, creatureEditor_cbSetAreaActivation, label));
        }
        eduiMenuAttach(parent, menu);
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void creatureEditor_cbSelectLocator(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Locator");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    eduiMenuAddItem(
        menu, eduiItemCheckCreate(-1, attr, creature->locator == nullptr, 1, creatureEditor_cbSetLocator, "NONE"));
    i32 index = 0;
    for (NULISTLNK *link = NuLinkedListGetHead(creatureEditor_LocatorList()); link != nullptr;
         link = NuLinkedListGetNext(creatureEditor_LocatorList(), link)) {
        EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(link);
        void *locator_path = *reinterpret_cast<void **>(reinterpret_cast<u8 *>(locator) + 0x2c);
        CreatureEditorRecord *current = creatureEditor_Current();
        if (locator_path != current->path)
            continue;
        if (current->locator == locator) {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 1, 1, creatureEditor_cbSetLocator, locator->name));
            menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 0, 1, creatureEditor_cbSetLocator, locator->name));
        }
        eduiMenuAttach(parent, menu);
        ++index;
    }
}

static __used__ void creatureEditor_cbSelectTriggerArea(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Trigger Area");
    if (menu == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature->trigger_area == nullptr) {
        eduiMenuAddItem(menu, eduiItemCheckCreate(-1, attr, 1, 1, creatureEditor_cbSetTriggerArea, "NONE"));
        menu->selected = edui_last_item;
    } else {
        eduiMenuAddItem(menu, eduiItemCheckCreate(-1, attr, 0, 1, creatureEditor_cbSetTriggerArea, "NONE"));
    }
    eduiMenuAttach(parent, menu);
    i32 index = 0;
    NULISTHDR *list = creatureEditor_AreaList();
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;
         link = NuLinkedListGetNext(creatureEditor_AreaList(), link), ++index) {
        if (creatureEditor_Current()->trigger_area == link) {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 1, 1, creatureEditor_cbSetTriggerArea,
                                                      reinterpret_cast<char *>(link) + 8));
            menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 0, 1, creatureEditor_cbSetTriggerArea,
                                                      reinterpret_cast<char *>(link) + 8));
        }
        eduiMenuAttach(parent, menu);
    }
}

static __used__ void creatureEditor_cbSelectType(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    if (LevelCharacterGlobalIDFn == nullptr || GlobalCharacterNameFn == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select AI Type");
    if (menu == nullptr)
        return;
    i32 index = 0;
    i32 type = LevelCharacterGlobalIDFn(0);
    while (type != -1) {
        if (aieditorsettings.current_path_type == type) {
            eduiMenuAddItem(
                menu, eduiItemCheckCreate(type, attr, 1, 1, creatureEditor_cbSetType, GlobalCharacterNameFn(type)));
            menu->selected = edui_last_item;
        } else {
            eduiMenuAddItem(
                menu, eduiItemCheckCreate(type, attr, 0, 1, creatureEditor_cbSetType, GlobalCharacterNameFn(type)));
        }
        eduiMenuAttach(parent, menu);
        type = LevelCharacterGlobalIDFn(static_cast<u8>(++index));
    }
}

extern "C" {
    void aieditor_cvSelectEditorMode(eduimenu_s *parent, eduiitem_s *, u32) {
        eduimenu_s *menu = eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, aieditor_cbCancelSelectEditorMode,
                                          (char *)"Select Editor Mode");
        if (menu == nullptr) {
            return;
        }
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            if ((i16)aieditorsettings.current_mode == index) {
                eduiitem_s *item = eduiItemCheckCreate(index, &attr, 1, 1, aieditor_cbSetEditorMode,
                                                       aieditorsettings.modes[index].name);
                eduiMenuAddItem(menu, item);
                menu->selected = edui_last_item;
            } else {
                eduiitem_s *item = eduiItemCheckCreate(index, &attr, 0, 1, aieditor_cbSetEditorMode,
                                                       aieditorsettings.modes[index].name);
                eduiMenuAddItem(menu, item);
            }
            eduiMenuAttach(parent, menu);
        }
    }

}

static __used__ void creatureEditor_cbRenameCreatureMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Rename Creature");
    if (menu == nullptr)
        return;
    eduiitem_s *item = eduiItemTextPickCreate(0, attr, creatureEditor_cbRenameCreature, "Creature Name");
    eduiMenuAddItem(menu, item);
    edui_textpicker_s *picker = reinterpret_cast<edui_textpicker_s *>(item);
    strcpy(picker->value, creature->name);
    picker->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void routeEditor_cbRenameRouteMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_route == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, routeEditor_cbCancelRenameRouteMenu, (char *)"Rename Route");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, routeEditor_cbRenameRoute, (char *)"Route Name");
    eduiMenuAddItem(menu, item);
    strcpy(static_cast<EdUiNameInputItem *>(edui_last_item)->name, aieditor->current_path->current_route->name);
    static_cast<EdUiNameInputItem *>(edui_last_item)->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void pathEditor_cbRenameNodeMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_node == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelRenameNodeMenu, (char *)"Rename Node");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, pathEditor_cbRenameNode, (char *)"Node Name");
    eduiMenuAddItem(menu, item);
    strcpy(static_cast<EdUiNameInputItem *>(edui_last_item)->name, aieditor->current_path->current_node->name);
    static_cast<EdUiNameInputItem *>(edui_last_item)->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void locatorEditor_cbAddLocatorsByNameMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator_set == nullptr) {
        return;
    }
    eduimenu_s *menu = eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameLocatorSetMenu,
                                      (char *)"Add Locators By Name");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item =
        (edui_textpicker_s *)eduiItemTextPickCreate(0, attr, locatorEditor_cbAddLocatorsByName, (char *)"Name");
    eduiMenuAddItem(menu, item);
    if (aieditor->current_locator != nullptr) {
        strcpy(aieditor->pending_locator_name, aieditor->current_locator->name);
    } else {
        strcpy(aieditor->pending_locator_name, aieditor->current_locator_set->name);
    }
    NuStrCpy(((edui_textpicker_s *)edui_last_item)->value, aieditor->pending_locator_name);
    ((edui_textpicker_s *)edui_last_item)->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void locatorEditor_cbRenameLocatorSetMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator_set == nullptr) {
        return;
    }
    eduimenu_s *menu = eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameLocatorSetMenu,
                                      (char *)"Rename Locator Set");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item =
        (edui_textpicker_s *)eduiItemTextPickCreate(0, attr, locatorEditor_cbRenameLocatorSet, (char *)"Name");
    eduiMenuAddItem(menu, item);
    strcpy(((edui_textpicker_s *)edui_last_item)->value, aieditor->current_locator_set->name);
    ((edui_textpicker_s *)edui_last_item)->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void locatorEditor_cbRenameLocatorMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    if (aieditor->current_locator == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, locatorEditor_cbCancelRenameMenu, (char *)"Rename Locator");
    if (menu == nullptr) {
        return;
    }
    edui_textpicker_s *item =
        (edui_textpicker_s *)eduiItemTextPickCreate(0, attr, locatorEditor_cbRenameLocator, (char *)"Locator Name");
    eduiMenuAddItem(menu, item);
    strcpy(((edui_textpicker_s *)edui_last_item)->value, aieditor->current_locator->name);
    ((edui_textpicker_s *)edui_last_item)->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __used__ void areaEditor_cbRenameAreaMenu(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    if (selected == NULL) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(240, 90, 240, 250, ed_fnt, areaEditor_cbCancelRenameMenu, const_cast<char *>("Rename Area"));
    if (menu == NULL) {
        return;
    }
    eduiMenuAddItem(menu, eduiItemTextPickCreate(0, &attr, areaEditor_cbRenameArea, const_cast<char *>("Area Name")));
    strcpy(static_cast<edui_textpicker_s *>(edui_last_item)->value, area_selected()->name);
    static_cast<edui_textpicker_s *>(edui_last_item)->max_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static __attribute__((force_align_arg_pointer)) EDANTINODE_REGPARM1 void
antinodeEditor_AntinodeMoved(EDANTINODE_s *node) {
    nuvec_s forward = {0.0f, 0.0f, 1.0f};
    if (static_cast<i8>(node->game_flags) >= 0 && NuSpecialExistsFn(&aieditor->cursor_platform)) {
        node->special = aieditor->cursor_platform;
        NUMTX *matrix = NuSpecialGetDrawMtx(&aieditor->cursor_platform);
        NuVecInvMtxTransform(&node->special_position, &node->position, matrix);
        nuvec_s rotated;
        NuVecMtxRotate(&rotated, &forward, matrix);
        node->rotation_offset = NuAtan2D(rotated.x, rotated.z);
        node->rotation_offset = NuAngSub(node->flags, node->rotation_offset);
    } else {
        memset(&node->special, 0, sizeof(node->special));
    }
}

// Retail keeps this out of line; the unit's -O3 would otherwise inline it.
static __attribute__((noinline, noclone)) EDAIPATHNODE_s *pathEditor_GetNearestNode(EDAIPATH_s *path, i32 require_radius) {
    if (path == nullptr) {
        return nullptr;
    }
    EDAIPATHNODE_s *node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
    if (node == nullptr) {
        return nullptr;
    }
    EDAIPATHNODE_s *nearest = nullptr;
    f32 nearest_distance = FLT_MAX;
    if (require_radius) {
        for (; node != nullptr;
             node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &node->link))) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
            if (!(nearest_distance > distance)) {
                continue;
            }
            f32 height = aieditor->cursor_position.y - node->position.y;
            f32 upper = NuFmax(0.2f, node->height_max);
            f32 lower = NuFmin(-0.2f, node->height_min);
            if (height <= upper && height >= lower && distance < node->radius * node->radius) {
                nearest = node;
                nearest_distance = distance;
            }
        }
    } else {
        for (; node != nullptr;
             node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &node->link))) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
            if (nearest_distance > distance) {
                f32 height = aieditor->cursor_position.y - node->position.y;
                f32 upper = NuFmax(0.2f, node->height_max);
                f32 lower = NuFmin(-0.2f, node->height_min);
                if (height <= upper && height >= lower) {
                    nearest = node;
                    nearest_distance = distance;
                }
            }
        }
    }
    return nearest;
}

static __used__ i32 RowMoveWander(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader) {
    AIPATHCNX *connection = row->path_info.connection;
    if (connection == NULL)
        return 0;
    i32 node_index = connection->node_indices[row->path_info.direction == 0];
    if (row->next_connection == NULL) {
        if (previous != NULL) {
            row->next_connection = previous->path_info.connection == connection ? previous->next_connection
                                                                                : previous->path_info.connection;
            if (row->next_connection == NULL)
                return 0;
        } else {
            AIPATHNODE *node = &row->path_info.path->nodes[node_index];
            i32 first = NuRandInt() % node->connection_count;
            connection = row->path_info.connection;
            for (i32 i = 0; i < node->connection_count; ++i) {
                row->next_connection = node->connections[(first + i) % node->connection_count];
                AIPATHCNX *next = row->next_connection;
                if (next != connection) {
                    u8 route = leader->ai->field_0x138;
                    if (route == 0xff || ((static_cast<u64>(next->route_mask) >> route) & 1) != 0 ||
                        ((static_cast<u64>(node->route_boundary_mask) >> route) & 1) != 0) {
                        i32 direction = node_index != next->node_indices[0];
                        AIPATHNODE *other = &row->path_info.path->nodes[next->node_indices[direction ^ 1]];
                        if (other->radius > group->radius && next->traversal_flags[direction] == 0)
                            break;
                    }
                }
                row->next_connection = NULL;
            }
            if (row->next_connection == NULL)
                row->next_connection = connection;
        }
        AIPATHCNX *next = row->next_connection;
        if (next->traversal_flags[node_index != next->node_indices[0]] == 0) {
            i32 current_angle = connection->rotation;
            if (row->path_info.direction != 0)
                current_angle = NuAngAdd(0x8000, current_angle);
            i32 next_angle;
            if (connection->node_indices[row->path_info.direction == 0] == next->node_indices[0]) {
                row->next_direction = 0;
                next_angle = next->rotation;
            } else {
                row->next_direction = 1;
                next_angle = NuAngAdd(0x8000, next->rotation);
            }
            row->is_clockwise = NuAngSub(next_angle, current_angle) < 0;
        }
    }
    NUVEC offset = {row->is_clockwise ? -group->radius : group->radius, 0.0f, 0.0f};
    NUVEC rotated, centre;
    NuVecRotateY(&rotated, &offset, row->y_rot);
    NuVecAdd(&centre, &rotated, &row->pos);
    if (!row->is_turning) {
        if (row->path_info.connection != row->next_connection) {
            AIPATHNODE *nodes = row->path_info.path->nodes;
            f32 distance =
                NuInfiniteLineToPointDistSqr(&nodes[row->next_connection->node_indices[0]].position,
                                             &nodes[row->next_connection->node_indices[1]].position, &centre);
            if (group->radius * group->radius >= distance)
                row->is_turning = 1;
        } else {
            AIPATHNODE *node =
                &row->path_info.path->nodes[row->path_info.connection->node_indices[row->path_info.direction == 0]];
            f32 distance = NuVecXZDistSqr(&row->pos, &node->position, &rotated);
            if (node->radius_squared > distance) {
                for (i32 i = 0; i < group->row_count; ++i) {
                    group->rows[i].next_connection = NULL;
                    group->rows[i].y_rot = NuAngAdd(group->rows[i].y_rot, 0x8000);
                    group->rows[i].path_info.direction = group->rows[i].path_info.direction == 0;
                }
                group->leader = NULL;
                group->is_reversed = !group->is_reversed;
                if (!group->is_reversed) {
                    for (i32 i = 0; i < group->member_count; ++i) {
                        APIOBJECT *object = group->members[i];
                        if (object->objptr != NULL && (object->field_0x1f8 & 0x1001) == 0x1001 &&
                            object->field_0x287 == 0) {
                            group->leader = object;
                            break;
                        }
                    }
                } else {
                    for (i32 i = group->member_count - 1; i != -1; --i) {
                        APIOBJECT *object = group->members[i];
                        if (object != NULL && (object->field_0x1f8 & 0x1001) == 0x1001 && object->field_0x287 == 0) {
                            group->leader = object;
                            break;
                        }
                    }
                }
                return 1;
            }
        }
    }
    if (row->is_turning) {
        i32 angle = row->next_connection->rotation;
        if (row->next_direction != 0)
            angle = NuAngAdd(angle, 0x8000);
        i32 difference = NuAngSub(angle, row->y_rot);
        i32 step = group->rotation_speed;
        if (difference <= step) {
            step = -step;
            if (difference >= step) {
                row->is_turning = 0;
                row->path_info.connection = row->next_connection;
                row->next_connection = NULL;
                row->path_info.direction = row->next_direction;
                step = difference;
            }
        }
        row->y_rot = NuAngAdd(step, row->y_rot);
        NuVecRotateY(&rotated, &offset, row->y_rot);
        NuVecSub(&row->pos, &centre, &rotated);
        return 0;
    }
    if (!group->is_row_turning) {
        i32 angle = row->path_info.connection->rotation;
        if (row->path_info.direction != 0)
            angle = NuAngAdd(angle, 0x8000);
        i32 difference = NuAngSub(angle, row->y_rot);
        i32 step = group->rotation_speed;
        if (difference <= step) {
            step = -step;
            if (difference >= step)
                step = difference;
        }
        row->y_rot = NuAngAdd(step, row->y_rot);
        rotated.x = rotated.y = rotated.z = 0.0f;
        if (GetCharacterGoalSpeedFn != NULL)
            rotated.z = GetCharacterGoalSpeedFn(leader);
        NuVecRotateY(&rotated, &rotated, row->y_rot);
        NuVecAdd(&row->pos, &row->pos, &rotated);
        row->pos.y = leader->position.y;
    }
    return 0;
}

static __used__ i32 RowMoveTowards(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader) {
    AIPATHCNX *connection = row->path_info.connection;
    if (connection == NULL)
        return 0;
    i32 node_index = connection->node_indices[row->path_info.direction == 0];
    i32 direction_node = node_index;
    if (row->next_connection == NULL) {
        if (previous != NULL) {
            row->next_connection = previous->path_info.connection == connection ? previous->next_connection
                                                                                : previous->path_info.connection;
        } else if (group->leader != NULL) {
            AIPATHINFO *destination = &group->leader->ai->script_process.path_info;
            if (destination->path == row->path_info.path) {
                i32 destination_node = destination->connection->node_indices[destination->dist > 0.5f];
                direction_node = destination_node;
                u8 connection_index = destination->path->route_matrix[node_index][destination_node];
                if (connection_index != 0xff) {
                    row->next_connection = destination->path->nodes[node_index].connections[connection_index];
                    if (row->next_connection->traversal_flags[node_index != row->next_connection->node_indices[0]] != 0)
                        row->next_connection = NULL;
                }
            }
        }
        AIPATHCNX *next = row->next_connection;
        if (next != NULL && next->traversal_flags[direction_node != next->node_indices[0]] == 0) {
            i32 current_angle = connection->rotation;
            if (row->path_info.direction != 0)
                current_angle = NuAngAdd(0x8000, current_angle);
            i32 next_angle;
            if (connection->node_indices[row->path_info.direction == 0] == next->node_indices[0]) {
                row->next_direction = 0;
                next_angle = next->rotation;
            } else {
                row->next_direction = 1;
                next_angle = NuAngAdd(0x8000, next->rotation);
            }
            row->is_clockwise = NuAngSub(next_angle, current_angle) < 0;
        }
    }
    if (row->next_connection == NULL) {
        if (row->path_info.direction != 0) {
            if (row->path_info.dist < 0.0f)
                return 1;
        } else if (row->path_info.dist > 1.0f) {
            return 1;
        }
        NUVEC movement;
        i32 angle = row->path_info.connection->rotation;
        if (row->path_info.direction != 0)
            angle = NuAngAdd(angle, 0x8000);
        i32 difference = NuAngSub(angle, row->y_rot);
        i32 step = group->rotation_speed;
        if (difference <= step) {
            step = -step;
            if (difference >= step)
                step = difference;
        }
        row->y_rot = NuAngAdd(step, row->y_rot);
        movement.x = movement.y = movement.z = 0.0f;
        if (GetCharacterGoalSpeedFn != NULL)
            movement.z = GetCharacterGoalSpeedFn(leader);
        NuVecRotateY(&movement, &movement, row->y_rot);
        NuVecAdd(&row->pos, &row->pos, &movement);
        row->pos.y = leader->position.y;
        return 0;
    }
    NUVEC offset = {row->is_clockwise ? -group->radius : group->radius, 0.0f, 0.0f};
    NUVEC rotated, centre;
    NuVecRotateY(&rotated, &offset, row->y_rot);
    NuVecAdd(&centre, &rotated, &row->pos);
    if (!row->is_turning) {
        if (row->path_info.connection != row->next_connection) {
            AIPATHNODE *nodes = row->path_info.path->nodes;
            f32 distance =
                NuInfiniteLineToPointDistSqr(&nodes[row->next_connection->node_indices[0]].position,
                                             &nodes[row->next_connection->node_indices[1]].position, &centre);
            if (group->radius * group->radius >= distance)
                row->is_turning = 1;
        } else {
            AIPATHNODE *node =
                &row->path_info.path->nodes[row->path_info.connection->node_indices[row->path_info.direction == 0]];
            f32 distance = NuVecXZDistSqr(&row->pos, &node->position, &rotated);
            if (node->radius_squared > distance) {
                for (i32 i = 0; i < group->row_count; ++i) {
                    group->rows[i].next_connection = NULL;
                    group->rows[i].y_rot = NuAngAdd(group->rows[i].y_rot, 0x8000);
                    group->rows[i].path_info.direction = group->rows[i].path_info.direction == 0;
                }
                group->leader = NULL;
                group->is_reversed = !group->is_reversed;
                if (group->is_reversed) {
                    for (i32 i = group->member_count - 1; i != -1; --i) {
                        APIOBJECT *object = group->members[i];
                        if (object != NULL && (object->field_0x1f8 & 0x1001) == 0x1001 && object->field_0x287 == 0) {
                            group->leader = object;
                            break;
                        }
                    }
                } else {
                    for (i32 i = 0; i < group->member_count; ++i) {
                        APIOBJECT *object = group->members[i];
                        if (object->objptr != NULL && (object->field_0x1f8 & 0x1001) == 0x1001 &&
                            object->field_0x287 == 0) {
                            group->leader = object;
                            break;
                        }
                    }
                }
                return 1;
            }
        }
    }
    if (row->is_turning) {
        i32 angle = row->next_connection->rotation;
        if (row->next_direction != 0)
            angle = NuAngAdd(angle, 0x8000);
        i32 difference = NuAngSub(angle, row->y_rot);
        i32 step = group->rotation_speed;
        if (difference <= step) {
            step = -step;
            if (difference >= step) {
                row->is_turning = 0;
                row->path_info.connection = row->next_connection;
                row->next_connection = NULL;
                row->path_info.direction = row->next_direction;
                step = difference;
            }
        }
        row->y_rot = NuAngAdd(step, row->y_rot);
        NuVecRotateY(&rotated, &offset, row->y_rot);
        NuVecSub(&row->pos, &centre, &rotated);
        return 0;
    }
    if (!group->is_row_turning) {
        i32 angle = row->path_info.connection->rotation;
        if (row->path_info.direction != 0)
            angle = NuAngAdd(angle, 0x8000);
        i32 difference = NuAngSub(angle, row->y_rot);
        i32 step = group->rotation_speed;
        if (difference <= step) {
            step = -step;
            if (difference >= step)
                step = difference;
        }
        row->y_rot = NuAngAdd(step, row->y_rot);
        rotated.x = rotated.y = rotated.z = 0.0f;
        if (GetCharacterGoalSpeedFn != NULL)
            rotated.z = GetCharacterGoalSpeedFn(leader);
        NuVecRotateY(&rotated, &rotated, row->y_rot);
        NuVecAdd(&row->pos, &row->pos, &rotated);
        row->pos.y = leader->position.y;
    }
    return 0;
}

static void xConst(NUFPAR *parser) {
    i32 cur;
    f32 default_val;

    NuStrLen(parser->word_buf);
    NuFParGetWord(parser);
    NuStrNCpy(aiscript_const[aiscript_const_curr].name, parser->word_buf, 0x20);

    cur = aiscript_const_curr;
    default_val = NuFParGetFloat(parser);
    aiscript_const_curr++;

    aiscript_const[cur].default_val = default_val;
}

static void xEndState(NUFPAR *parser) {
    NuFParPopCom(parser);
}

__used__ static i32 Action_ResetToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet == NULL || sys == NULL) {
        return 1;
    }
    NUVEC *origin = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
    if (origin != NULL) {
        packet->owner->apiobj.position = *origin;
    } else if (packet->owner != NULL && (packet->owner->apiobj.field_0x1f4 & 0x400) != 0 &&
               packet->field_0x134 != 0xff) {
        packet->owner->apiobj.position = sys->creatures[packet->field_0x134].pos;
    }
    return 1;
}

static __used__ i32 routeEditor_AddToRoute(EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path->current_route == nullptr)
        return 0;
    i32 index = path->current_route - path->routes;
    if (index > 15)
        return 0;
    u16 mask = 1u << index;
    i32 slot;
    if (node->connections[0].node == other)
        slot = 0;
    else if (node->connections[1].node == other)
        slot = 1;
    else if (node->connections[2].node == other)
        slot = 2;
    else if (node->connections[3].node == other)
        slot = 3;
    else if (node->connections[4].node == other)
        slot = 4;
    else if (node->connections[5].node == other)
        slot = 5;
    else if (node->connections[6].node == other)
        slot = 6;
    else if (node->connections[7].node == other)
        slot = 7;
    else
        return 0;
    EDAIPATHCNX_s *connection = &node->connections[slot];
    if (!(connection->route_mask & mask)) {
        connection->route_mask |= mask;
        return 1;
    }
    connection->route_mask &= ~mask;
    if (other->connections[slot].node != nullptr && (other->connections[slot].route_mask & mask))
        other->route_mask &= ~mask;
    return -1;
}

static __used__ void creatureEditor_cbSetType(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item != nullptr)
        aieditorsettings.current_path_type = item->data;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (creature != nearest)
        return;
    i32 type = aieditorsettings.current_path_type;
    creature->character_type = type;
    if (GetViewRangeFn != nullptr) {
        const f32 value = GetViewRangeFn(type);
        creature->view_distance = value;
        creature = creatureEditor_Current();
    } else {
        creature->view_distance = 1.0f;
    }
    if (GetHearDistanceFn != nullptr) {
        const f32 value = GetHearDistanceFn(aieditorsettings.current_path_type);
        creature->hear_distance = value;
        creature = creatureEditor_Current();
    } else {
        creature->hear_distance = 1.0f;
    }
    if (GetMaxViewHeightFn != nullptr) {
        const f32 value = GetMaxViewHeightFn(aieditorsettings.current_path_type);
        creature->max_view_height = value;
        creature = creatureEditor_Current();
    } else {
        creature->max_view_height = 1.0f;
    }
    if (GetMinViewHeightFn != nullptr) {
        const f32 value = GetMinViewHeightFn(aieditorsettings.current_path_type);
        creature->negative_min_view_height = value;
    } else {
        creature->negative_min_view_height = 1.0f;
    }
}

static void AISysCheckAntinode_Circle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius) {
    if (difference->x * difference->x + difference->z * difference->z < radius * radius) {
        difference->x = object->position.x - antinode->position.x;
        difference->z = object->position.z - antinode->position.z;
        i32 angle = NuAtan2D(difference->x, difference->z);
        difference->x = object->ai->movement_destination.x - antinode->position.x;
        difference->z = object->ai->movement_destination.z - antinode->position.z;
        i32 turn = NuAngSub(NuAtan2D(difference->x, difference->z), angle);
        if (object->ai->antinode_timer > 0.0f) {
            if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
                object->ai->antinode_timer < antinode_time) {
                object->ai->antinode_clockwise = !object->ai->antinode_clockwise;
                object->ai->antinode_timer = antinode_time + antinode_reverse_time;
            }
            if (object->ai->antinode_clockwise) {
                if (turn < 0) {
                    turn += 0x10000;
                }
            } else if (turn > 0) {
                turn -= 0x10000;
            }
        } else {
            if (turn > 0) {
                object->ai->antinode_clockwise = 1;
            } else {
                object->ai->antinode_clockwise = 0;
            }
        }
        if ((antinode->game_flags & 2) == 0 && object->ai->antinode_timer < antinode_time) {
            object->ai->antinode_timer = antinode_time;
        }
        i32 max_turn = static_cast<i32>((ai_moveradius / radius) * 10430.3779296875f);
        if (turn > max_turn) {
            turn = max_turn;
        } else if (turn < -max_turn) {
            turn = -max_turn;
        }
        angle = NuAngAdd(angle, turn);
        difference->x = 0.0f;
        difference->y = 0.0f;
        difference->z = radius;
        NuVecRotateY(difference, difference, angle);
        object->ai->movement_position.x = difference->x + antinode->position.x;
        object->ai->movement_position.y = object->ai->movement_destination.y;
        object->ai->movement_position.z = difference->z + antinode->position.z;
        object->field_0x1fa |= 4;
    }
}

static void AISysCheckAntinode_Rectangle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius) {
    difference->x = object->ai->movement_position.x - antinode->position.x;
    if (difference->x > radius || difference->x < -radius) {
        return;
    }
    difference->z = object->ai->movement_position.z - antinode->position.z;
    if (difference->z > radius || difference->z < -radius || object->collision_min.y > antinode->max_y ||
        antinode->min_y > object->collision_max.y) {
        return;
    }
    NuVecRotateY(difference, difference, -antinode->flags);
    if (fabsf(difference->x) < antinode->base_radius + object->ai->mover_height &&
        fabsf(difference->z) < antinode->base_height + object->ai->mover_height) {
        NUVEC local_object;
        NUVEC local_destination;
        difference->x = object->position.x - antinode->position.x;
        difference->y = 0.0f;
        difference->z = object->position.z - antinode->position.z;
        NuVecRotateY(&local_object, difference, -antinode->flags);
        i32 object_angle = NuAtan2D(local_object.x, local_object.z);
        difference->x = object->ai->movement_destination.x - antinode->position.x;
        difference->y = 0.0f;
        difference->z = object->ai->movement_destination.z - antinode->position.z;
        NuVecRotateY(&local_destination, difference, -antinode->flags);
        i32 turn = NuAngSub(NuAtan2D(local_destination.x, local_destination.z), object_angle);
        i32 turn_offset = 0;
        if (object->ai->antinode_timer > 0.0f) {
            if (((object->ai->path_info.flags & 3) == 2 || object->respawn_timer > 1.0f) &&
                object->ai->antinode_timer < antinode_time) {
                object->ai->antinode_clockwise = !object->ai->antinode_clockwise;
                object->ai->antinode_timer = antinode_time + antinode_reverse_time;
            }
            if (object->ai->antinode_clockwise) {
                if (turn < 0) {
                    turn_offset = 0x10000;
                }
            } else if (turn > 0) {
                turn_offset = -0x10000;
            }
        } else if (turn > 0) {
            object->ai->antinode_clockwise = 1;
        } else {
            object->ai->antinode_clockwise = 0;
        }
        if ((antinode->game_flags & 2) == 0 && object->ai->antinode_timer < antinode_time) {
            object->ai->antinode_timer = antinode_time;
        }
        i32 corner_angle = NuAtan2D(antinode->base_radius, antinode->base_height);
        i32 angle = NuAngAdd(object_angle, 0);
        f32 tangent_x;
        f32 tangent_z;
        if (angle > 0x8000 - corner_angle) {
            object->ai->movement_position.x = (antinode->base_radius + object->ai->mover_height) * NU_SIN_LUT(angle);
            object->ai->movement_position.z = -(object->ai->mover_height + antinode->base_height);
            tangent_x = -1.0f;
            tangent_z = 0.0f;
        } else if (angle > corner_angle) {
            object->ai->movement_position.x = antinode->base_radius + object->ai->mover_height;
            object->ai->movement_position.z = (object->ai->mover_height + antinode->base_height) * NU_COS_LUT(angle);
            tangent_x = 0.0f;
            tangent_z = -1.0f;
        } else if (angle > -corner_angle) {
            object->ai->movement_position.x = (antinode->base_radius + object->ai->mover_height) * NU_SIN_LUT(angle);
            object->ai->movement_position.z = object->ai->mover_height + antinode->base_height;
            tangent_x = 1.0f;
            tangent_z = 0.0f;
        } else if (angle >= corner_angle - 0x7fff) {
            object->ai->movement_position.x = -(antinode->base_radius + object->ai->mover_height);
            object->ai->movement_position.z = (object->ai->mover_height + antinode->base_height) * NU_COS_LUT(angle);
            tangent_x = 0.0f;
            tangent_z = 1.0f;
        } else {
            object->ai->movement_position.x = (antinode->base_radius + object->ai->mover_height) * NU_SIN_LUT(angle);
            object->ai->movement_position.z = -(object->ai->mover_height + antinode->base_height);
            tangent_x = -1.0f;
            tangent_z = 0.0f;
        }
        if (turn + turn_offset < 0) {
            tangent_x = -tangent_x;
            tangent_z = -tangent_z;
        }
        i32 absolute_turn = turn < 0 ? -turn : turn;
        if (absolute_turn < 1820) {
            f32 scale = static_cast<f32>(absolute_turn) / 1820.0f;
            tangent_x *= scale;
            tangent_z *= scale;
        }
        object->ai->movement_position.y = 0.0f;
        object->ai->movement_position.x += tangent_x;
        object->ai->movement_position.z += tangent_z;
        NuVecRotateY(&object->ai->movement_position, &object->ai->movement_position, antinode->flags);
        NuVecAdd(&object->ai->movement_position, &object->ai->movement_position, &antinode->position);
        object->field_0x1fa |= 4;
    }
}

__used__ static i32 Action_NoShadows(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        packet->owner->apiobj.field_0x1f4 |= 0x2000;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                packet->owner->apiobj.field_0x1f4 &= ~0x2000u;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_NoLosCheck(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        packet->owner->apiobj.flags_high |= 4;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                packet->owner->apiobj.flags_high &= static_cast<u8>(~4u);
            }
        }
    }
    return 1;
}

static i32 Action_SetFullPathSearch(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                    i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL && first_time != 0) {
        packet->owner->apiobj.field_0x1fa &= ~8;
        for (i32 i = 0; i < param_count; i++) {
            if (NuStrICmp(params[i], "false") == 0) {
                packet->owner->apiobj.field_0x1fa |= 8;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_RequiresLOS(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && param_5 != 0) {
        packet->runtime_flags |= 2;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                packet->runtime_flags &= static_cast<u8>(~2u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CanSeeBehind(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || param_5 == 0) {
        return 1;
    }

    packet->owner->apiobj.flags_high |= 8;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "false") == 0) {
            packet->owner->apiobj.flags_high &= static_cast<u8>(~8u);
        }
    }
    return 1;
}

static i32 Action_DontUseShadowTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                       i32 param_count, i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL && first_time != 0) {
        packet->owner->apiobj.flags_low |= 0x40;
        for (i32 i = 0; i < param_count; i++) {
            if (NuStrICmp(params[i], "false") == 0) {
                packet->owner->apiobj.flags_low &= ~0x40;
            }
        }
    }
    return 1;
}

static i32 Action_ShadowTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                                i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL && first_time != 0) {
        packet->owner->apiobj.flags_low |= 8;
        for (i32 i = 0; i < param_count; i++) {
            if (NuStrICmp(params[i], "false") == 0) {
                packet->owner->apiobj.flags_low &= ~8;
            }
        }
    }
    return 1;
}

static i32 Action_FlatTerrain(AISYS *, AISCRIPTPROCESS *, AIPACKET *packet, char **params, i32 param_count,
                              i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL && first_time != 0) {
        packet->owner->apiobj.flags_low |= 0x10;
        auto packet_owner = packet->owner;
        for (i32 i = 0; i < param_count; i++) {
            if (NuStrICmp(params[i], "false") == 0) {
                packet_owner->apiobj.flags_low &= ~0x10;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_NoTerrain(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        packet->owner->apiobj.flags_low |= 0x20;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                packet->owner->apiobj.flags_low &= static_cast<u8>(~0x20u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CheckWallSplines(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    if (packet != NULL) {
        if (param_5 != 0) {
            packet->check_wall_splines = 1;
            for (i32 i = 0; i < param_4; i++) {
                if (NuStrICmp(params[i], "false") == 0) {
                    packet->check_wall_splines = 0;
                }
            }
            if (packet->check_wall_splines) {
                packet->movement_flags |= 0x80;
            }
        }
    }
    return 1;
}

static __used__ i32 Action_IgnoreWallSplines(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    if (packet != NULL) {
        if (first_time != 0) {
            packet->movement_flags |= 0x80;
            for (i32 index = 0; index < param_count; ++index) {
                if (NuStrICmp(params[index], "false") == 0) {
                    packet->movement_flags &= static_cast<u8>(~0x80u);
                }
            }
        }
    }
    return 1;
}

static i32 Action_SetIgnoreAntinodes(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    if (first_time != 0) {
        APIOBJECT *object = packet != NULL ? reinterpret_cast<APIOBJECT *>(packet->owner) : NULL;
        i32 ignore = 1;
        for (i32 i = 0; i < param_count; i++) {
            char *value = NuStrIStr(params[i], "character");
            if (value != NULL) {
                if (GetNamedAPIObjectFn != NULL) {
                    object = GetNamedAPIObjectFn(sys, value + 10);
                }
            } else if (NuStrICmp("FALSE", params[0]) == 0) {
                // The original tests the first parameter, even on later iterations.
                ignore = 0;
            }
        }
        if (object != NULL) {
            object->ignore_antinodes = ignore;
        }
    }
    return 1;
}

static i32 Action_SetCircleDirection(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                                     i32 first_time, f32) {
    if (packet != NULL && packet->owner != NULL && first_time != 0 && param_count != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (NuStrICmp(params[i], "Clockwise") == 0) {
                packet->circle_clockwise = 1;
            } else if (NuStrICmp(params[i], "AntiClockwise") == 0) {
                packet->circle_clockwise = 0;
            } else if (NuStrICmp(params[i], "Reverse") == 0) {
                packet->circle_clockwise = !packet->circle_clockwise;
            } else if (NuStrICmp(params[i], "Random") == 0) {
                if (NuRandFloat() > 0.5f) {
                    packet->circle_clockwise = 1;
                } else {
                    packet->circle_clockwise = 0;
                }
            }
        }
    }
    return 1;
}

static void AIMoveFindDivertNode(AIPATH *path, AIPACKET *packet, NUVEC *destination) {
    if (path->special_route_count != 0 &&
        (packet->navigation_flags & AIPACKET_NAVIGATION_FLAG_USE_SPECIAL_ROUTES) != 0) {
        AIPATHNODELINK *links = path->special_routes;
        if (__builtin_expect(packet->divert_node_index >= path->node_count, 0)) {
            packet->divert_node_index = 0xff;
        }
        if (packet->divert_search_index >= path->special_route_count) {
            packet->divert_search_index = 0;
        }
        f32 nearest = FLT_MAX;
        if (__builtin_expect(packet->divert_node_index < path->node_count, 0)) {
            nearest = NuVecDistSqr(&path->nodes[packet->divert_node_index].position, destination, NULL);
        }
        u8 initial_index = packet->divert_search_index;
        for (i32 index = 0; index < 4; ++index) {
            if (packet->divert_search_index != packet->divert_node_index) {
                f32 distance = NuVecDistSqr(&path->nodes[links[packet->divert_search_index].node_index].position,
                                            destination, NULL);
                if (distance < nearest) {
                    nearest = distance;
                    packet->divert_node_index = links[packet->divert_search_index].node_index;
                }
            }
            ++packet->divert_search_index;
            if (packet->divert_search_index > path->special_route_count) {
                packet->divert_search_index = 0;
            }
            if (packet->divert_search_index == initial_index) {
                break;
            }
        }
    } else {
        if (packet->divert_node_index >= path->node_count) {
            packet->divert_node_index = 0xff;
        }
        if (packet->divert_search_index >= path->node_count) {
            packet->divert_search_index = 0;
        }
        f32 nearest = FLT_MAX;
        if (packet->divert_node_index < path->node_count) {
            nearest = NuVecDistSqr(&path->nodes[packet->divert_node_index].position, destination, NULL);
        }
        for (i32 index = 0; index < 4; ++index) {
            f32 distance = NuVecDistSqr(&path->nodes[packet->divert_search_index].position, destination, NULL);
            if (distance < nearest) {
                nearest = distance;
                packet->divert_node_index = packet->divert_search_index;
            }
            ++packet->divert_search_index;
            if (packet->divert_search_index > path->node_count) {
                packet->divert_search_index = 0;
            }
        }
    }
}

static f32 Condition_NearestPlayerToLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                            void *void_arg) {
    NUVEC difference;
    f32 distance;
    if (packet != NULL && sys != NULL) {
        AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
        if (locator == NULL) {
            locator = processor->unknown_a4;
        }
        if (locator != NULL) {
            distance = 3.402823466e+38f;
            if (sys->player_1 != NULL) {
                distance = NuVecDistSqr(&sys->player_1->position, &locator->position, &difference);
            }
            if (sys->player_2 != NULL) {
                f32 second = NuVecDistSqr(&sys->player_2->position, &locator->position, &difference);
                distance = second < distance ? second : distance;
            }
            if (distance != 3.402823466e+38f) {
                distance = NuFsqrt(distance);
            }
            return distance;
        }
    }
    return 3.402823466e+38f;
}

static EDANTINODE_REGPARM1 EDANTINODE_s *antinodeEditor_GetNearestAntinode(i32 require_inside) {
    EDANTINODE_s *nearest = nullptr;
    f32 nearest_distance = FLT_MAX;
    for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(antinode_list())); node != nullptr;
         node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(antinode_list(), &node->link))) {
        nuvec_s offset;
        f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &node->position, &offset);
        if (!(nearest_distance > distance))
            continue;
        f32 height = aieditor->camera_position.y - node->position.y;
        f32 upper = NuFmax(0.2f, node->upper_height);
        if (!((height >= NuFmin(-0.2f, node->lower_height)) & (upper >= height)))
            continue;
        if (require_inside) {
            if (node->type != 1) {
                if (node->type == 0) {
                    if (!(node->radius * node->radius > distance))
                        continue;
                } else if (node->type == 2) {
                    f32 diagonal =
                        NuFsqrt(node->base_radius * node->base_radius + node->base_height * node->base_height);
                    nuvec_s rectangle_offset;
                    rectangle_offset.x = aieditor->camera_position.x - node->position.x;
                    if (rectangle_offset.x > diagonal || rectangle_offset.x < -diagonal)
                        continue;
                    rectangle_offset.z = aieditor->camera_position.z - node->position.z;
                    if (rectangle_offset.z > diagonal || rectangle_offset.z < -diagonal)
                        continue;
                    rectangle_offset.y = 0.0f;
                    NuVecRotateY(&rectangle_offset, &rectangle_offset, -node->flags);
                    if (!(node->base_radius > NuFabs(rectangle_offset.x)) ||
                        !(node->base_height > NuFabs(rectangle_offset.z)))
                        continue;
                } else {
                    continue;
                }
            } else {
                f32 extent = node->base_radius > node->base_height ? node->base_radius : node->base_height;
                nuvec_s ellipse_offset;
                ellipse_offset.x = aieditor->camera_position.x - node->position.x;
                if (ellipse_offset.x > extent || ellipse_offset.x < -extent)
                    continue;
                ellipse_offset.z = aieditor->camera_position.z - node->position.z;
                if (ellipse_offset.z > extent || ellipse_offset.z < -extent)
                    continue;
                f32 ellipse_distance = ellipse_offset.x * ellipse_offset.x + ellipse_offset.z * ellipse_offset.z;
                if (!(extent * extent > ellipse_distance))
                    continue;
                ellipse_offset.y = 0.0f;
                f32 aspect_ratio = node->base_height / node->base_radius;
                nuvec_s rotated;
                NuVecRotateY(&rotated, &ellipse_offset, -node->flags);
                i32 angle = NuAtan2D(aspect_ratio * rotated.x, rotated.z);
                f32 ellipse_x = NuTrigTable[(angle >> 1) & 0x7fff] * node->base_radius;
                f32 ellipse_z = NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff] * node->base_height;
                if (!(ellipse_x * ellipse_x + ellipse_z * ellipse_z > ellipse_distance))
                    continue;
            }
        }
        nearest = node;
        nearest_distance = distance;
    }
    return nearest;
}

eduimenu_s *antinodeEditor_Process(nupad_s *pad) {
    const u32 held = pad->digital_buttons;
    const u32 pressed = pad->digital_buttons_pressed;
    EDANTINODE_s *selected = antinode_selected();
    EDANTINODE_s *nearest = antinode_nearest();

    if (pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (menu == nullptr)
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ANTINODES, &attr, 0, 0, aieditor_cvSelectEditorMode,
                                                const_cast<char *>("Select Editor Mode")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbGoToPlayer, const_cast<char *>("Go To Player")));
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbMovePlayer, const_cast<char *>("Move Player")));
        selected = antinode_selected();
        if (selected != nullptr)
            eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, antinodeEditor_cbSelectType,
                                                    const_cast<char *>("Select Antinode Type")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, &attr, -i32(aieditorsettings.solid_antinode_display), 1,
                                                   aieditor_cbSolidAntinodeDisplayToggle,
                                                   const_cast<char *>("Solid Antinode Display")));
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, &attr, -i32(aieditorsettings.stop_platforms), 2,
                                             aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, &attr, -i32(aieditorsettings.snap_height_display), 3,
                                                   aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
        selected = antinode_selected();
        if (selected != nullptr) {
            eduiMenuAddItem(menu, eduiItemToggleCreate(0x80, &attr, selected->game_flags >> 7, 4,
                                                       antinodeEditor_cbAntiNodeFlagsToggle,
                                                       const_cast<char *>("Never On A Platform")));
            eduiMenuAddItem(menu, eduiItemToggleCreate(2, &attr, (antinode_selected()->game_flags >> 1) & 1, 5,
                                                       antinodeEditor_cbAntiNodeFlagsToggle,
                                                       const_cast<char *>("Disbale direction memory")));
        }
        return menu;
    }
    if (held & 0x40) {
        if (nearest == nullptr) {
            if (pressed & 0x40) {
                selected = CreateAntinode(&aieditor->camera_position);
                aieditor->mode_selection_42e9c = selected;
                antinodeEditor_AntinodeMoved(selected);
            }
        } else if (pressed & 0x40) {
            aieditor->mode_selection_42e9c = nearest;
            edcamSetPos(&nearest->position);
        } else if (selected != nullptr && selected == nearest) {
            if (!((pressed & 0x100) && NuSpecialExistsFn(&selected->special))) {
                selected->position = aieditor->camera_position;
                antinodeEditor_AntinodeMoved(selected);
                if (selected->type != 0) {
                    if (held & 0x8000)
                        selected->base_radius =
                            selected->base_radius * 0.99f < 0.05f ? 0.05f : selected->base_radius * 0.99f;
                    else if (held & 0x2000)
                        selected->base_radius *= 1.01f;
                    else if (held & 0x1000)
                        selected->base_height *= 1.01f;
                    else if (held & 0x4000)
                        selected->base_height =
                            selected->base_height * 0.99f < 0.05f ? 0.05f : selected->base_height * 0.99f;
                }
            } else {
                NUVEC *platform_position = NuSpecialGetDrawPos(&aieditor->cursor_platform);
                if (platform_position != nullptr) {
                    selected->position.x = platform_position->x;
                    selected->position.z = platform_position->z;
                    antinodeEditor_AntinodeMoved(selected);
                    edcamSetPos(&selected->position);
                }
            }
        }
    } else if (pressed & 0x10) {
        if (selected != nullptr && selected == nearest) {
            eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, antinodeEditor_cbCancelDeleteAntinodeMenu,
                                              const_cast<char *>("Delete Antinode??"));
            if (menu == nullptr)
                return nullptr;
            eduiMenuAddItem(
                menu, eduiItemSelCreate(0, &attr, 0, 0, antinodeEditor_cbDeleteAntinode, const_cast<char *>("No")));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, &attr, 0, 0, antinodeEditor_cbDeleteAntinode, const_cast<char *>("Yes")));
            return menu;
        }
        antinode_nearest() = antinodeEditor_GetNearestAntinode(1);
        return nullptr;
    } else if (pressed & 0x100) {
        selected = antinodeEditor_GetNearestAntinode(0);
        aieditor->mode_selection_42e9c = selected;
        if (selected != nullptr)
            edcamSetPos(&selected->position);
    } else if ((held & 0x100) && (pressed & (1 | 2 | 4 | 8))) {
        bool skip_platforms = false;
        bool forward = false;
        if (pressed & 8) {
            forward = true;
        } else if (pressed & 2) {
            forward = false;
        } else if (pressed & 4) {
            forward = true;
            skip_platforms = true;
        } else {
            skip_platforms = true;
        }
        EDANTINODE_s *first = nullptr;
        EDANTINODE_s *candidate;
        do {
            candidate = antinode_selected();
            NULISTLNK *next = candidate == nullptr ? (forward ? NuLinkedListGetHead(antinode_list())
                                                              : NuLinkedListGetTail(antinode_list()))
                                                   : (forward ? NuLinkedListGetNext(antinode_list(), &candidate->link)
                                                              : NuLinkedListGetPrev(antinode_list(), &candidate->link));
            aieditor->mode_selection_42e9c = next;
            if (next == nullptr)
                next = forward ? NuLinkedListGetHead(antinode_list()) : NuLinkedListGetTail(antinode_list());
            candidate = reinterpret_cast<EDANTINODE_s *>(next);
            aieditor->mode_selection_42e9c = candidate;
            if (candidate == nullptr || (skip_platforms && candidate == first)) {
                aieditor->mode_selection_42e9c = nullptr;
                break;
            }
            if (first == nullptr) {
                first = candidate;
            }
        } while (skip_platforms && NuSpecialExistsFn(&candidate->special));
        candidate = antinode_selected();
        if (candidate != nullptr)
            edcamSetPos(&candidate->position);
    } else if ((held & 0x100) == 0 && selected != nullptr && selected == nearest) {
        if (held & (0x8000 | 0x2000)) {
            if (selected->type == 0) {
                if (held & 0x8000)
                    selected->radius = selected->radius * 0.99f < 0.05f ? 0.05f : selected->radius * 0.99f;
                else
                    selected->radius *= 1.01f;
            } else {
                aieditorsettings.area_rotation = selected->flags;
                if (pressed & ((held & 0x8000) ? 0x8000 : 0x2000))
                    antinode_rotation_repeat() = 20;
                else
                    antinode_rotation_repeat() =
                        antinode_rotation_repeat() + 20 < 601 ? antinode_rotation_repeat() + 20 : 600;
                if ((held & 0x8000)) {
                    selected->flags = NuAngSub(selected->flags, antinode_rotation_repeat());
                } else {
                    selected->flags = NuAngAdd(selected->flags, antinode_rotation_repeat());
                }
                aieditorsettings.area_rotation = selected->flags;
                antinodeEditor_AntinodeMoved(selected);
            }
        } else if (aieditorsettings.solid_antinode_display && (held & (0x1000 | 0x4000))) {
            aieditor->flags |= 4;
            f32 step = (selected->upper_height - selected->lower_height) * 0.05f;
            if (held & 0x1000) {
                if (held & 4)
                    selected->upper_height += step;
                else if (held & 1)
                    selected->upper_height -= step;
                selected->upper_height = selected->upper_height < selected->lower_height + 0.01f
                                             ? selected->lower_height + 0.01f
                                             : selected->upper_height;
            } else {
                if (held & 4)
                    selected->lower_height += step;
                else if (held & 1)
                    selected->lower_height -= step;
                if (selected->lower_height > selected->upper_height - 0.01f) {
                    selected->lower_height = selected->upper_height - 0.01f;
                } else {
                    selected->lower_height = selected->lower_height;
                }
            }
        }
    }

    antinode_nearest() = antinodeEditor_GetNearestAntinode(1);
    return nullptr;
}

static f32 Condition_PlayerToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    NUVEC difference;
    if (packet != NULL) {
        if (sys != NULL && sys->player_1 != NULL) {
            if (void_arg == NULL) {
                void_arg = processor->unknown_a4;
            }
            if (void_arg != NULL) {
                AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
                return NuVecDist(&sys->player_1->position, &locator->position, &difference);
            }
        }
    }
    return 3.402823466e+38f;
}

static f32 Condition_PlayerToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    if (packet != NULL && sys != NULL && sys->player_1 != NULL) {
        NUVEC *origin;
        if (GetAICreatureOriginFn != NULL && (origin = GetAICreatureOriginFn(sys, packet)) != NULL) {
            return NuVecDist(&sys->player_1->position, origin, NULL);
        }
        if (packet->field_0x134 != 0xff) {
            return NuVecDist(&sys->player_1->position, &sys->creatures[packet->field_0x134].pos, NULL);
        }
    }
    return 3.402823466e+38f;
}

static f32 Condition_OpponentToOrigin(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    if (packet != NULL && packet->opponent_object != NULL) {
        NUVEC *origin;
        if (GetAICreatureOriginFn != NULL && (origin = GetAICreatureOriginFn(sys, packet)) != NULL) {
            return NuVecDist(&packet->opponent_object->position, origin, NULL);
        }
        if (sys != NULL && packet->field_0x134 != 0xff) {
            return NuVecDist(&packet->opponent_object->position, &sys->creatures[packet->field_0x134].pos, NULL);
        }
    }
    return 3.402823466e+38f;
}

static f32 Condition_OriginRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet != NULL) {
        NUVEC *origin = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
        if (origin != NULL) {
            return NuVecDist(&packet->terrain_origin, origin, NULL);
        } else if (sys != NULL && packet->field_0x134 != 0xff) {
            return NuVecDist(&packet->terrain_origin, &sys->creatures[packet->field_0x134].pos, NULL);
        }
    }
    return 0.0f;
}

static void *Condition_AlwaysTrueInit(AISYS *sys, char *arg, AISCRIPT *script) {
    f32 value;

    if (arg != NULL && NuStrLen(arg) != 0) {
        value = NuAToF(arg);

        return *(void **)&value;
    }

    value = 1.0f;
    return *(void **)&value;
}

static __used__ __attribute__((optimize("peel-loops"))) void routeEditor_cbRenameRoute(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || path->current_route == nullptr) {
        return;
    }
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (input->name[0] == '\0') {
        return;
    }
    auto input_name = input->name;
    for (i32 index = 0; index < 16; ++index) {
        if (NuStrICmp(aieditor->current_path->routes[index].name, input_name) == 0) {
            return;
        }
    }
    strcpy(aieditor->current_path->current_route->name, input->name);
}

static __used__ void creatureEditor_cbRenameCreature(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    const char *name = reinterpret_cast<edui_textpicker_s *>(item)->value;
    if (__builtin_expect(creature == nullptr || name[0] == '\0', 0))
        return;
    NULISTHDR *list = &aieditor->creatures;
    for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr; link = NuLinkedListGetNext(list, link)) {
        CreatureEditorRecord *other = reinterpret_cast<CreatureEditorRecord *>(link);
        if (__builtin_expect(NuStrICmp(other->name, name) == 0, 0))
            return;
    }
    strcpy(creature->name, name);
}

static __used__ void pathEditor_cbRenamePath(eduimenu_s *, eduiitem_s *item, u32) {
    EDAIPATH_s *current = aieditor->current_path;
    EdUiNameInputItem *input = static_cast<EdUiNameInputItem *>(item);
    if (current == nullptr || input->name[0] == '\0') {
        return;
    }
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    auto input_name = input->name;
    while (path != nullptr) {
        if (NuStrICmp(path->name, input_name) == 0) {
            return;
        }
        path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
    }
    strcpy(aieditor->current_path->name, input->name);
}

static __used__ void locatorEditor_cbRenameLocatorSet(eduimenu_s *, eduiitem_s *item, u32) {
    char *name = ((edui_textpicker_s *)item)->value;
    if (aieditor->current_locator_set == nullptr || name[0] == 0) {
        return;
    }
    if (NuStrICmp("NONE", name) == 0) {
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        if (NuStrICmp(set->name, name) == 0) {
            return;
        }
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    strcpy(aieditor->current_locator_set->name, name);
}

static __used__ void locatorEditor_cbRenameLocator(eduimenu_s *, eduiitem_s *item, u32) {
    char *name = ((edui_textpicker_s *)item)->value;
    if (aieditor->current_locator == nullptr || name[0] == 0) {
        return;
    }
    EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
    while (locator != nullptr) {
        if (NuStrICmp(locator->name, name) == 0) {
            return;
        }
        locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
    }
    strcpy(aieditor->current_locator->name, name);
}

static __used__ void areaEditor_cbRenameArea(eduimenu_s *, eduiitem_s *item, unsigned int) {
    EDAIAREA_s *selected = area_selected();
    const char *name = static_cast<edui_textpicker_s *>(item)->value;
    if (selected == NULL || name[0] == '\0') {
        return;
    }
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        if (NuStrICmp(area->name, name) == 0) {
            return;
        }
    }
    strcpy(area_selected()->name, name);
}

static void pathEditorSetCamPosBody(nuvec_s *position) {
    nuvec_s camera;
    camera.x = position->x;
    camera.y = aieditor->cursor_position.y;
    camera.z = position->z;
    edcamSetPos(&camera);
}

static void antinodeEditor_cbSelectType(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    EDANTINODE_s *node = antinode_selected();
    if (node == nullptr || LevelCharacterGlobalIDFn == nullptr || GlobalCharacterNameFn == nullptr)
        return;
    eduimenu_s *menu =
        eduiMenuCreate(220, 70, 240, 250, ed_fnt, antinodeEditor_cbCancelMenu, const_cast<char *>("Select AI Type"));
    if (menu == nullptr)
        return;
    if (antinode_selected()->type == 0) {
        eduiMenuAddItem(menu, eduiItemCheckCreate(0, &attr, 1, 1, antinodeEditor_cbSetType, antinode_types[0]));
        menu->selected = edui_last_item;
    } else {
        eduiMenuAddItem(menu, eduiItemCheckCreate(0, &attr, 0, 1, antinodeEditor_cbSetType, antinode_types[0]));
    }
    eduiMenuAttach(parent, menu);
    if (antinode_selected()->type == 1) {
        eduiMenuAddItem(menu, eduiItemCheckCreate(1, &attr, 1, 1, antinodeEditor_cbSetType, antinode_types[1]));
        menu->selected = edui_last_item;
    } else {
        eduiMenuAddItem(menu, eduiItemCheckCreate(1, &attr, 0, 1, antinodeEditor_cbSetType, antinode_types[1]));
    }
    eduiMenuAttach(parent, menu);
    if (antinode_selected()->type != 2) {
        eduiMenuAddItem(menu, eduiItemCheckCreate(2, &attr, 0, 1, antinodeEditor_cbSetType, antinode_types[2]));
    } else {
        eduiMenuAddItem(menu, eduiItemCheckCreate(2, &attr, 1, 1, antinodeEditor_cbSetType, antinode_types[2]));
        menu->selected = edui_last_item;
    }
    eduiMenuAttach(parent, menu);
}

static __used__ void routeEditor_cbRouteUsers(eduimenu_s *parent, eduiitem_s *, u32) {
    if (SpecialRouteCharacterNameFn == nullptr || aieditor->current_path == nullptr ||
        aieditor->current_path->current_route == nullptr) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xdc, 0x46, 0xf0, 0xfa, ed_fnt, routeEditor_cbCancelRouteUsers, (char *)"Select route users");
    if (menu == nullptr) {
        return;
    }
    i32 index = 0;
    char *name = SpecialRouteCharacterNameFn(0);
    while (index < 64 && name != nullptr) {
        i32 selected = (aieditor->current_path->current_route->user_mask >> index) & 1;
        eduiitem_s *item = eduiItemCheckCreate(index, &attr, selected, index + 1, routeEditor_cbSetRouteUsers, name);
        eduiMenuAddItem(menu, item);
        eduiMenuAttach(parent, menu);
        ++index;
        name = SpecialRouteCharacterNameFn(index);
    }
    if (index < 64) {
        i32 selected = (aieditor->current_path->current_route->user_mask >> 63) & 1;
        eduiitem_s *item =
            eduiItemCheckCreate(63, &attr, selected, 64, routeEditor_cbSetRouteUsers, (char *)"Everyone");
        eduiMenuAddItem(menu, item);
    }
}

static __used__ void pathEditor_cbRenamePathMenu(eduimenu_s *parent, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr || (path->flags & 1) != 0) {
        return;
    }
    eduimenu_s *menu =
        eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, pathEditor_cbCancelRenamePathMenu, (char *)"Rename Path");
    if (menu == nullptr) {
        return;
    }
    eduiitem_s *item = eduiItemTextPickCreate(0, &attr, pathEditor_cbRenamePath, (char *)"Path Name");
    eduiMenuAddItem(menu, item);
    strcpy(static_cast<EdUiNameInputItem *>(edui_last_item)->name, aieditor->current_path->name);
    static_cast<EdUiNameInputItem *>(edui_last_item)->max_name_length = 15;
    eduiMenuAttach(parent, menu);
    menu->x = parent->x + 10;
    menu->y = parent->y + 40;
}

static void AIScriptCopyActions(NULISTHDR *src, NULISTHDR *dst, VARIPTR *buf, VARIPTR *buf_end) {
    AIACTION *src_action;

    src_action = (AIACTION *)NuLinkedListGetHead(src);
    while (src_action != NULL) {
        AIACTION *dst_action;

        dst_action = (AIACTION *)AIScriptBufferAlloc(buf, buf_end, sizeof(AIACTION));
        if (dst_action != NULL) {
            dst_action->params = (char **)AIScriptBufferAlloc(buf, buf_end, src_action->param_count * sizeof(char *));
            dst_action->param_count = src_action->param_count;

            for (i32 i = 0; i < dst_action->param_count; i++) {
                dst_action->params[i] = AIScriptCopyString(src_action->params[i], buf, buf_end);
            }

            dst_action->def = src_action->def;

            NuLinkedListAppend(dst, &dst_action->list_node);
        }

        src_action = (AIACTION *)NuLinkedListGetNext(src, &src_action->list_node);
    }
}

__attribute__((optimize("no-tree-loop-optimize"))) void areaEditor_Enter() {
    area_list()->head = NULL;
    area_list()->tail = NULL;
    for (i32 index = 0; index < 64; ++index) {
        EDAIAREA_s *pool = reinterpret_cast<EDAIAREA_s *>(reinterpret_cast<u8 *>(aieditor) + 0x36938);
        NuLinkedListAppend(area_free_list(), &pool[index].link);
    }
    if (aieditor->ai_system == NULL) {
        return;
    }
    for (i32 index = 0; index < aieditor->ai_system->area_count; ++index) {
        AIAREA *source = &aieditor->ai_system->areas[index];
        i16 rotation = source->rotation;
        EDAIAREA_s *area = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetHead(area_free_list()));
        if (area != NULL) {
            NuLinkedListRemove(area_free_list(), &area->link);
            NuLinkedListAppend(area_list(), &area->link);
            area->position = source->position;
            memcpy(&area->size, &source->half_width, sizeof(area->size));
            area->flags = source->game_flags;
            area->rotation = rotation;
        }
        NuStrNCpy(area->name, source->name, 16);
    }
}

static void DestroyAIPathNodeBody(EDAIPATHNODE_s *node, EDAIPATH_s *path) {
    if (node != NULL && node->shared_node != NULL) {
        --node->shared_node->reference_count;
        if (node->shared_node->reference_count <= 1)
            pathEditor_DestroySharedNode(node->shared_node);
        node->shared_node = NULL;
    }
    for (i32 connection = 0; connection < 8; ++connection) {
        EDAIPATHNODE_s *other = node->connections[connection].node;
        if (other != NULL) {
            pathEditor_DisconnectNodes(node, other);
        }
    }
    NuLinkedListRemove(&path->nodes, &node->link);
    --path->node_count;
    memset(node, 0, sizeof(*node));
    NuLinkedListAppend(&aieditor->free_path_nodes, &node->link);
}

static __used__ void creatureEditor_cbSetScriptParam(eduimenu_s *menu, eduiitem_s *item, unsigned int) {
    if (item == nullptr)
        return;
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature == nullptr)
        return;
    i32 index = item->data;
    f32 value = reinterpret_cast<edui_slider_s *>(item)->value;
    if (__builtin_expect(value == aieditorsettings.current_script_params[index], 1))
        return;
    aieditorsettings.current_script_params[index] = value;
    u32 bit = static_cast<u32>(u64(2) << (index & 63));
    aieditorsettings.current_script_flags |= bit;
    creature->script_params[index] = value;
    creature->flags = (creature->flags & ~0x1e) | aieditorsettings.current_script_flags;
    if (menu != nullptr && reset_params_option == nullptr) {
        reset_params_option =
            eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, 1, 1, creatureEditor_cbResetParams, "Reset Params"));
    }
}

static void AIScriptCopyConditions(NULISTHDR *src, NULISTHDR *dst, VARIPTR *buf, VARIPTR *buf_end) {
    AICONDITION *src_cond;

    src_cond = (AICONDITION *)NuLinkedListGetHead(src);
    while (src_cond != NULL) {
        AICONDITION *dst_cond;

        dst_cond = (AICONDITION *)AIScriptBufferAlloc(buf, buf_end, sizeof(AICONDITION));
        if (dst_cond != NULL) {
            memset(dst_cond, 0, sizeof(AICONDITION));
            memmove(dst_cond, src_cond, sizeof(AICONDITION));

            dst_cond->list_node.prev = NULL;
            dst_cond->list_node.next = NULL;

            dst_cond->arg = AIScriptCopyString(src_cond->arg, buf, buf_end);
            dst_cond->next_state_name = AIScriptCopyString(src_cond->next_state_name, buf, buf_end);

            NuLinkedListAppend(dst, &dst_cond->list_node);
        }

        src_cond = (AICONDITION *)NuLinkedListGetNext(src, &src_cond->list_node);
    }
}

static void xActions(NUFPAR *parser) {
    while (NuFParGetLine(parser) != 0) {
        AIACTION *action;

        NuFParGetWord(parser);

        if (NuStrICmp(parser->word_buf, "}") == 0) {
            return;
        }

        if (NuStrICmp(parser->word_buf, "DoMacro") == 0) {
            AIACTIONMACRO *macro;

            NuFParGetWord(parser);

            macro = (AIACTIONMACRO *)NuLinkedListGetHead(&load_aiscript->action_macros);
            while (macro != NULL) {
                if (NuStrICmp(macro->name, parser->word_buf) == 0) {
                    AIScriptCopyActions(&macro->actions, load_actionshdr, load_buff, load_endbuff);

                    break;
                }

                macro = (AIACTIONMACRO *)NuLinkedListGetNext(&load_aiscript->action_macros, &macro->list_node);
            }
        } else {
            AIACTIONDEF *def;
            char params[24][128];

            def = AIActionFind(parser->word_buf);
            if (def != NULL) {
                i32 param_count;

                action = (AIACTION *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AIACTION));
                if (action == NULL) {
                    continue;
                }

                action->list_node.next = NULL;

                NuLinkedListAppend(load_actionshdr, &action->list_node);

                action->def = def;

                param_count = 0;

                while (NuFParGetWord(parser) != 0) {
                    if (NuStrCmp(parser->word_buf, "\\") == 0) {
                        NuFParGetLine(parser);
                    } else {
                        NuStrCpy(params[param_count++], parser->word_buf);
                    }
                }

                if (param_count != 0) {
                    action->params =
                        (char **)AIScriptBufferAlloc(load_buff, load_endbuff, param_count * sizeof(char *));
                    if (action->params != NULL) {
                        action->param_count = param_count;

                        for (i32 i = 0; i < param_count; i++) {
                            action->params[i] = AIScriptCopyString(params[i], load_buff, load_endbuff);
                        }
                    }
                }
            } else {
                NuStrChr(parser->word_buf, '{');
            }
        }
    }
}

static LOCATOR_SET_BODY_ATTR unsigned int AddLocatorToSetBody(EDLOCATORSET_s *set, EDLOCATOR_s *locator,
                                                              EDLOCATOR_s *before) {
    if (LocatorSetContains(set, locator)) {
        LocatorSetRemove(set, locator);
    }
    if (before != nullptr) {
        for (i32 index = 0; index < 64; ++index) {
            if (set->locators[index] == before) {
                for (i32 move = 62; move >= index; --move) {
                    set->locators[move + 1] = set->locators[move];
                }
                set->locators[index + 1] = locator;
                return 1;
            }
        }
    }
    for (i32 index = 0; index < 64; ++index) {
        if (set->locators[index] == nullptr) {
            set->locators[index] = locator;
            if (index < 63) {
                set->locators[index + 1] = nullptr;
            }
            return 1;
        }
    }
    return 0;
}

static __used__ unsigned int AddLocatorToSet(EDLOCATORSET_s *set, EDLOCATOR_s *locator, EDLOCATOR_s *before) {
    if (locator == nullptr || set == nullptr || set->locators[63] != nullptr) {
        return 0;
    }
    if (set->locators[0] != nullptr && set->locators[0]->path != locator->path) {
        return 0;
    }
    return AddLocatorToSetBody(set, locator, before);
}

static __used__ __attribute__((optimize("no-tree-vectorize"))) void DestroyLocator(EDLOCATOR_s *locator) {
    if (locator == nullptr) {
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
    while (set != nullptr) {
        i32 index = 0;
        while (index < 64) {
            EDLOCATOR_s *current = set->locators[index];
            if (current == nullptr) {
                break;
            }
            if (current == locator) {
                for (i32 move = index; move < 63; ++move) {
                    set->locators[move] = set->locators[move + 1];
                }
                set->locators[63] = nullptr;
                break;
            }
            ++index;
        }
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
    }
    NuLinkedListRemove(&aieditor->locators, &locator->link);
    memset(locator, 0, sizeof(*locator));
    NuLinkedListAppend(&aieditor->free_locators, &locator->link);
}

static void ConditionsParseFinalise(AICONDITION *cond, NUFPAR *parser) {
    *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) &= static_cast<u8>(~2);

    NuFParGetWord(parser);

    cond->next_state_name = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);

    NuFParGetWord(parser);

    if (NuStrICmp(parser->word_buf, "KeepBlockedMessages") == 0) {
        *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) |= 2;
    }
}

static void xState(NUFPAR *parser) {
    if (load_aiscript != NULL && NuFParGetWord(parser) != 0) {
        load_aistate = (AISTATE *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AISTATE));

        // This can create an invalid pointer when `load_aistate` is NULL, but
        // fortunately doesn't ever seem to be accessed.
        load_refscripthdr = &load_aistate->ref_scripts;

        if (load_aistate == NULL) {
            return;
        }

        memset(load_aistate, 0, sizeof(AISTATE));

        load_aistate->name = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);

        if (NuStrICmp(load_aistate->name, "Base") == 0) {
            NuLinkedListInsert(&load_aiscript->states, &load_aistate->list_node);
        } else {
            NuLinkedListAppend(&load_aiscript->states, &load_aistate->list_node);
        }

        NuFParPushCom(parser, cfgtab_State);

        while (NuFParGetLine(parser) != 0) {
            NuFParGetWord(parser);

            load_conditionshdr = &load_aistate->conditions;
            load_actionshdr = &load_aistate->actions;

            NuFParInterpretWord(parser);
        }

        NuFParPopCom(parser);

        load_aistate = NULL;
        load_actionshdr = NULL;
        load_conditionshdr = NULL;
        load_refscripthdr = NULL;
    }
}

static void xActionMacro(NUFPAR *parser) {
    AIACTIONMACRO *macro;

    if (NuFParGetWord(parser) == 0) {
        return;
    }

    macro = (AIACTIONMACRO *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AIACTIONMACRO));
    if (macro == NULL) {
        return;
    }

    macro->list_node.next = NULL;
    macro->name = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);

    NuLinkedListAppend(&load_aiscript->action_macros, &macro->list_node);

    load_actionshdr = &macro->actions;

    xActions(parser);

    load_actionshdr = NULL;
}

static i32 AIMoveAdjustDestinationPath(AIPACKET *packet) {
    if (packet->path_info.path == packet->fallback_path_info.path && packet->fallback_path_info.connection != NULL) {
        return 0;
    }
    AIMoveFindDivertNode(packet->path_info.path, packet, &packet->fallback_destination);
    if (packet->divert_node_index >= packet->path_info.path->node_count) {
        return -1;
    }
    AIPATHNODE *node = &packet->path_info.path->nodes[packet->divert_node_index];
    if (node->connection_count == 0) {
        return -1;
    }
    packet->fallback_destination = node->position;
    packet->fallback_stopping_distance = 0.0f;
    packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
    memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
    packet->fallback_path_info.path = packet->path_info.path;
    packet->fallback_path_info.connection = node->connections[0];
    packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node_index;
    packet->fallback_path_info.dist = node->connections[0]->node_indices[0] == packet->divert_node_index ? 0.0f : 1.0f;
    return 1;
}

static u32 CalculateRightIntersection(f32 object_radius, f32 first_distance, f32 *second_distance, i32 first_angle,
                                      i32 second_angle, AIPATHNODE *before, f32 *shared_y, f32 shared_radius,
                                      AIPATHNODE *after, NUVEC *result) {
    f32 clearance = object_radius + 0.05f;
    f32 before_radius = before->radius - clearance;
    if (before_radius < 0.0f) {
        before_radius = 0.0f;
    }
    f32 shared_clear = shared_radius - clearance;
    if (shared_clear < 0.0f) {
        shared_clear = 0.0f;
    }
    f32 after_radius = after->radius - clearance;
    if (after_radius < 0.0f) {
        after_radius = 0.0f;
    }
    NUVEC first_start = {before_radius, 0.0f, 0.0f};
    NUVEC first_end = {before_radius, 0.0f, first_distance};
    NUANG angle = NuAngAdd(first_angle, NuASin((shared_clear - before_radius) / first_distance));
    NuVecRotateY(&first_start, &first_start, angle);
    NuVecRotateY(&first_end, &first_end, angle);
    NuVecAdd(&first_start, &first_start, &before->position);
    NuVecAdd(&first_end, &first_end, &before->position);

    NUVEC second_start = {-after_radius, 0.0f, 0.0f};
    NUVEC second_end = {-after_radius, 0.0f, *second_distance};
    angle = NuAngSub(second_angle, NuASin((shared_clear - after_radius) / *second_distance));
    NuVecRotateY(&second_start, &second_start, angle);
    NuVecRotateY(&second_end, &second_end, angle);
    NuVecAdd(&second_start, &second_start, &after->position);
    NuVecAdd(&second_end, &second_end, &after->position);

    NUVEC first_origin = first_start;
    NUVEC first_direction;
    NuVecSub(&first_direction, &first_end, &first_origin);
    NUVEC second_origin = second_start;
    NUVEC second_direction;
    NuVecSub(&second_direction, &second_end, &second_origin);
    f32 first_parameter;
    f32 second_parameter;
    if (NuLineLineIntersect(&first_origin, &first_direction, &second_origin, &second_direction, &first_parameter,
                            &second_parameter) != 0 &&
        first_parameter >= 0.0f && second_parameter >= 0.0f && first_parameter <= 1.0f && second_parameter <= 1.0f) {
        first_origin.y = *shared_y * first_parameter + before->position.y * (1.0f - first_parameter);
        NuVecScale(&first_direction, &first_direction, first_parameter);
        NuVecAdd(result, &first_origin, &first_direction);
        return 1;
    }
    return 0;
}

static inline void xDeriveFromScript(NUFPAR *parser) {
    i32 is_done;

    if (load_aiscript == NULL || load_aiscript->derived_from != NULL) {
        return;
    }

    load_aiscript->is_derived_from_level_script = 0;

    is_done = 0;
    while (!is_done && NuFParGetLine(parser) != 0) {
        while (!is_done && NuFParGetWord(parser) != 0) {
            char *cursor;

            if (NuStrICmp(parser->word_buf, "}") != 0) {
                if ((cursor = NuStrIStr(parser->word_buf, "Script")) != NULL) {
                    load_aiscript->derived_from =
                        AIScriptCopyString(cursor + strlen("Script") + 1, load_buff, load_endbuff);
                } else if (NuStrIStr(parser->word_buf, "Source") != NULL) {
                    if (NuStrIStr(parser->word_buf, "Global") != NULL) {
                        load_aiscript->is_derived_from_level_script = 0;
                    } else if (NuStrIStr(parser->word_buf, "Level") != NULL) {
                        load_aiscript->is_derived_from_level_script = 1;
                    }
                }
            } else {
                is_done = 1;
            }
        }
    }

    if (load_aiscript->derived_from != NULL) {
        load_aiscript->is_derived = 1;
    }
}

static unsigned int AddLocatorToSetAtEnd(EDLOCATORSET_s *set, EDLOCATOR_s *locator) {
    if (locator == nullptr || set == nullptr || set->locators[63] != nullptr) {
        return 0;
    }
    if (set->locators[0] != nullptr && set->locators[0]->path != locator->path) {
        return 0;
    }
    for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
        if (set->locators[index] == locator) {
            for (i32 move = index; move < 63; ++move) {
                set->locators[move] = set->locators[move + 1];
            }
            set->locators[63] = nullptr;
            break;
        }
    }
    for (i32 index = 0; index < 64; ++index) {
        if (set->locators[index] == nullptr) {
            set->locators[index] = locator;
            if (index < 63) {
                set->locators[index + 1] = nullptr;
            }
            return 1;
        }
    }
    return 0;
}

static void xParam(NUFPAR *parser) {
    u32 param_idx;

    if (load_aiscript == NULL) {
        return;
    }

    param_idx = NuFParGetInt(parser);
    if (param_idx >= 4) {
        return;
    }

    if (NuFParGetWord(parser) == 0) {
        return;
    }

    load_aiscript->params[param_idx].name = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);
    load_aiscript->params[param_idx].default_val = NuFParGetFloat(parser);
}

eduimenu_s *routeEditor_Process(nupad_s *pad) {
    u32 pressed = pad->digital_buttons_pressed;
    if (pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (__builtin_expect(menu == nullptr, 1))
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode,
                                                const_cast<char *>("Select Editor Mode")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbCreateRoute, const_cast<char *>("Create New Route")));
        EDAIPATH_s *selected_path = aieditor->current_path;
        if (selected_path != nullptr && selected_path->current_route != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbRenameRouteMenu,
                                                    const_cast<char *>("Rename Current Route")));
            eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, routeEditor_cbDeleteRoute,
                                                    const_cast<char *>("Destroy Current Route")));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbRouteUsers, const_cast<char *>("Route Users")));
        }
        return menu;
    }
    EDAIPATH_s *path = aieditor->current_path;
    if (path == nullptr)
        return nullptr;
    // Each direction visits the fixed route slots in order, stopping at the first active route.
    if (pressed & 0x1000) {
        i32 index = path->current_route == nullptr ? 1 : path->current_route - path->routes + 1;
        if (index >= 16)
            index = 0;
        path->current_route = &path->routes[index];
        if (!(path->current_route->flags & 1)) {
            ++index;
            if (index >= 16)
                index = 0;
            path->current_route = &path->routes[index];
            if (__builtin_expect(!(path->current_route->flags & 1), 1)) {
                ++index;
                if (index >= 16)
                    index = 0;
                path->current_route = &path->routes[index];
                if (!(path->current_route->flags & 1)) {
                    ++index;
                    if (index >= 16)
                        index = 0;
                    path->current_route = &path->routes[index];
                    if (!(path->current_route->flags & 1)) {
                        ++index;
                        if (index >= 16)
                            index = 0;
                        path->current_route = &path->routes[index];
                        if (!(path->current_route->flags & 1)) {
                            ++index;
                            if (index >= 16)
                                index = 0;
                            path->current_route = &path->routes[index];
                            if (!(path->current_route->flags & 1)) {
                                ++index;
                                if (index >= 16)
                                    index = 0;
                                path->current_route = &path->routes[index];
                                if (!(path->current_route->flags & 1)) {
                                    ++index;
                                    if (index >= 16)
                                        index = 0;
                                    path->current_route = &path->routes[index];
                                    if (!(path->current_route->flags & 1)) {
                                        ++index;
                                        if (index >= 16)
                                            index = 0;
                                        path->current_route = &path->routes[index];
                                        if (!(path->current_route->flags & 1)) {
                                            ++index;
                                            if (index >= 16)
                                                index = 0;
                                            path->current_route = &path->routes[index];
                                            if (!(path->current_route->flags & 1)) {
                                                ++index;
                                                if (index >= 16)
                                                    index = 0;
                                                path->current_route = &path->routes[index];
                                                if (!(path->current_route->flags & 1)) {
                                                    ++index;
                                                    if (index >= 16)
                                                        index = 0;
                                                    path->current_route = &path->routes[index];
                                                    if (!(path->current_route->flags & 1)) {
                                                        ++index;
                                                        if (index >= 16)
                                                            index = 0;
                                                        path->current_route = &path->routes[index];
                                                        if (!(path->current_route->flags & 1)) {
                                                            ++index;
                                                            if (index >= 16)
                                                                index = 0;
                                                            path->current_route = &path->routes[index];
                                                            if (!(path->current_route->flags & 1)) {
                                                                ++index;
                                                                if (index >= 16)
                                                                    index = 0;
                                                                path->current_route = &path->routes[index];
                                                                if (!(path->current_route->flags & 1)) {
                                                                    ++index;
                                                                    if (index >= 16)
                                                                        index = 0;
                                                                    path->current_route = &path->routes[index];
                                                                    if (!(path->current_route->flags & 1)) {
                                                                        path->current_route = nullptr;
                                                                        return nullptr;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (pressed & 0x4000) {
        i32 index = path->current_route == nullptr ? 15 : path->current_route - path->routes - 1;
        if (index < 0)
            index = 15;
        path->current_route = &path->routes[index];
        if (!(path->current_route->flags & 1)) {
            --index;
            if (index < 0)
                index = 15;
            path->current_route = &path->routes[index];
            if (!(path->current_route->flags & 1)) {
                --index;
                if (index < 0)
                    index = 15;
                path->current_route = &path->routes[index];
                if (!(path->current_route->flags & 1)) {
                    --index;
                    if (index < 0)
                        index = 15;
                    path->current_route = &path->routes[index];
                    if (!(path->current_route->flags & 1)) {
                        --index;
                        if (index < 0)
                            index = 15;
                        path->current_route = &path->routes[index];
                        if (!(path->current_route->flags & 1)) {
                            --index;
                            if (index < 0)
                                index = 15;
                            path->current_route = &path->routes[index];
                            if (!(path->current_route->flags & 1)) {
                                --index;
                                if (index < 0)
                                    index = 15;
                                path->current_route = &path->routes[index];
                                if (!(path->current_route->flags & 1)) {
                                    --index;
                                    if (index < 0)
                                        index = 15;
                                    path->current_route = &path->routes[index];
                                    if (!(path->current_route->flags & 1)) {
                                        --index;
                                        if (index < 0)
                                            index = 15;
                                        path->current_route = &path->routes[index];
                                        if (!(path->current_route->flags & 1)) {
                                            --index;
                                            if (index < 0)
                                                index = 15;
                                            path->current_route = &path->routes[index];
                                            if (!(path->current_route->flags & 1)) {
                                                --index;
                                                if (index < 0)
                                                    index = 15;
                                                path->current_route = &path->routes[index];
                                                if (!(path->current_route->flags & 1)) {
                                                    --index;
                                                    if (index < 0)
                                                        index = 15;
                                                    path->current_route = &path->routes[index];
                                                    if (!(path->current_route->flags & 1)) {
                                                        --index;
                                                        if (index < 0)
                                                            index = 15;
                                                        path->current_route = &path->routes[index];
                                                        if (!(path->current_route->flags & 1)) {
                                                            --index;
                                                            if (index < 0)
                                                                index = 15;
                                                            path->current_route = &path->routes[index];
                                                            if (!(path->current_route->flags & 1)) {
                                                                --index;
                                                                if (index < 0)
                                                                    index = 15;
                                                                path->current_route = &path->routes[index];
                                                                if (!(path->current_route->flags & 1)) {
                                                                    --index;
                                                                    if (index < 0)
                                                                        index = 15;
                                                                    path->current_route = &path->routes[index];
                                                                    if (!(path->current_route->flags & 1)) {
                                                                        path->current_route = nullptr;
                                                                        return nullptr;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (path->current_route == nullptr)
        return nullptr;
    if ((pad->digital_buttons & 0x40) && (pressed & 0x40) && path->nearest_node != nullptr) {
        path->current_node = path->nearest_node;
        nuvec_s position = path->current_node->position;
        position.y = aieditor->cursor_position.y;
        edcamSetPos(&position);
        pressed = pad->digital_buttons_pressed;
        path = aieditor->current_path;
    }
    if ((pressed & 0x20) && path->current_node != nullptr && path->nearest_node != nullptr &&
        path->current_node != path->nearest_node) {
        EDAIPATHNODE_s *nearest = path->nearest_node;
        i32 change = routeEditor_AddToRoute(path->current_node, nearest);
        routeEditor_AddToRoute(nearest, path->current_node);
        if (__builtin_expect(change == 1 || change == -1, 0)) {
            path->current_node = nearest;
            nuvec_s position;
            position.x = nearest->position.x;
            position.y = nearest->position.y;
            position.z = nearest->position.z;
            position.y = aieditor->cursor_position.y;
            edcamSetPos(&position);
            pressed = pad->digital_buttons_pressed;
            path = aieditor->current_path;
        }
    }
    if (__builtin_expect((pressed & 0x10) && path->nearest_node != nullptr, 1)) {
        // The route toggle acts on the highlighted node, which may differ from the selected node.
        EDAIPATHNODE_s *node = path->nearest_node;
        i32 index = path->current_route - path->routes;
        u64 mask = 1ull << index;
        if ((node->connections[0].node != nullptr && (node->connections[0].route_mask & mask)) ||
            (node->connections[1].node != nullptr && (node->connections[1].route_mask & mask)) ||
            (node->connections[2].node != nullptr && (node->connections[2].route_mask & mask)) ||
            (node->connections[3].node != nullptr && (node->connections[3].route_mask & mask)) ||
            (node->connections[4].node != nullptr && (node->connections[4].route_mask & mask)) ||
            (node->connections[5].node != nullptr && (node->connections[5].route_mask & mask)) ||
            (node->connections[6].node != nullptr && (node->connections[6].route_mask & mask)) ||
            (node->connections[7].node != nullptr && (node->connections[7].route_mask & mask))) {
            if ((static_cast<u64>(node->route_mask) >> index) & 1)
                node->route_mask &= ~mask;
            else
                node->route_mask |= mask;
        }
    }
    if (pressed & 0x100) {
        EDAIPATHNODE_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        for (EDAIPATHNODE_s *node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
             node != nullptr;
             node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &node->link))) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
            if (nearest_distance > distance) {
                f32 height = aieditor->cursor_position.y - node->position.y;
                f32 upper = NuFmax(0.2f, node->height_max);
                f32 lower = NuFmin(-0.2f, node->height_min);
                if (height <= upper && height >= lower) {
                    nearest = node;
                    nearest_distance = distance;
                }
            }
        }
        path->current_node = nearest;
        if (path->current_node != nullptr) {
            edcamSetPos(&path->current_node->position);
            path = aieditor->current_path;
        }
    }
    path->nearest_node = pathEditor_GetNearestNode(path, 1);
    path = aieditor->current_path;
    aieditor->flags &= ~u8(1);
    if (path->current_node != nullptr && path->current_node == path->nearest_node)
        aieditor->flags |= 1;
    return nullptr;
}

extern "C" {
    void SetAiRndrCullDistance(f32 distance) {
        AiRndrCullDistance = distance;
    }

    void AiRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *, NUMTX *matrix) {
        NUVEC first = vertices[0].position;
        NUVEC second = vertices[1].position;
        NUMTX matrix_copy;
        if (matrix != NULL) {
            if ((reinterpret_cast<usize>(matrix) & 15) != 0) {
                matrix_copy = *matrix;
                matrix = &matrix_copy;
            }
            NuVecMtxTransform(&first, &first, matrix);
            NuVecMtxTransform(&second, &second, matrix);
        }
        if (AiRndrCullDistance > 0.0f) {
            NUVEC position;
            if (aieditor && (aieditor->flags & 2))
                position = aieditor->camera_position;
            else
                position = *(NUVEC *)&global_camera.mtx.m30;
            f32 dx = position.x - first.x;
            f32 dy = position.y - first.y;
            f32 dz = position.z - first.z;
            f32 limit = AiRndrCullDistance * AiRndrCullDistance;
            f32 dx2 = position.x - second.x;
            f32 dy2 = position.y - second.y;
            f32 dz2 = position.z - second.z;
            if (dx * dx + dy * dy + dz * dz > limit && dx2 * dx2 + dy2 * dy2 + dz2 * dz2 > limit)
                return;
        }
        NuRndrLine3dDbg(first.x, first.y, first.z, second.x, second.y, second.z, vertices[0].colour);
    }

    void AiRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 colour) {
        if (AiRndrCullDistance > 0.0f) {
            NUVEC position;
            if (aieditor && (aieditor->flags & 2))
                position = aieditor->camera_position;
            else
                position = *(NUVEC *)&global_camera.mtx.m30;
            f32 dx = position.x - x0;
            f32 dy = position.y - y0;
            f32 dz = position.z - z0;
            f32 limit = AiRndrCullDistance * AiRndrCullDistance;
            if (!(dx * dx + dy * dy + dz * dz < limit)) {
                dx = position.x - x1;
                dy = position.y - y1;
                dz = position.z - z1;
                if (!(dx * dx + dy * dy + dz * dz < limit))
                    return;
            }
        }
        NuRndrLine3dDbg(x0, y0, z0, x1, y1, z1, colour);
    }

    void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level) {
        if (level < SCRIPT_ERROR_LEVEL_COUNT) {
            ScriptErrorLevel = level;
        }
    }

}

void RegisterAIScriptActions(AIACTIONDEF *action_defs) {
    AIACTIONDEF *def;
    i16 i;

    game_aiactiondefs = action_defs;

    i = 0;
    def = &action_defs[0];
    while (def->name != NULL) {
        def->idx = i;
        def->is_game_action = 1;

        def++;
        i++;
    }

    i = 0;
    def = &api_aiactiondefs[0];
    while (def->name != NULL) {
        def->idx = i;
        def->is_game_action = 0;

        def++;
        i++;
    }
}

void RegisterAIScriptConditions(AICONDITIONDEF *cond_defs) {
    game_aiconditiondefs = cond_defs;
}

AISTATE *AIStateFind(char *name, AISCRIPT *script) {
    AISTATE *state;

    state = (AISTATE *)NuLinkedListGetHead(&script->states);

    if (name != NULL) {
        while (state != NULL) {
            if (NuStrICmp(name, state->name) == 0) {
                return state;
            }

            state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
        }
    }

    return NULL;
}

__used__ static i32 Action_SetReturnToState(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                            i32 param_count, i32 is_first_time, f32) {
    if (is_first_time) {
        if (processor != NULL) {
            AISTATE *state = processor->state;
            for (i32 i = 0; i < param_count; i++) {
                char *value = NuStrIStr(params[i], "state");
                if (value != NULL) {
                    state = AIStateFind(value + 6, processor->script);
                }
            }
            processor->return_to_state = state;
        }
    }
    return 1;
}

i32 Action_SetState(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params, i32 param_count,
                    i32 is_first_time, float) {
    if (is_first_time == 0 || param_count == 0) {
        return 0;
    }

    processor->next_state = AIStateFind(params[0], processor->script);
    processor->unknown_flag_4 = 0;
    for (i32 param_index = 1; param_index < param_count; ++param_index) {
        if (NuStrICmp(params[param_index], "KeepBlockedMessages") == 0) {
            processor->unknown_flag_4 = 1;
        }
    }
    return 0;
}

AISCRIPT *AIScriptFind(AISYS *sys, char *name, i32 can_use_default, i32 check_level_scripts, i32 check_global_scripts) {
    AISCRIPT *script;

    if (name != NULL) {
        if (check_level_scripts && sys != NULL) {
            script = (AISCRIPT *)NuLinkedListGetHead(&sys->scripts);

            while (script != NULL) {
                if (NuStrICmp(name, script->name) == 0) {
                    return script;
                }

                script = (AISCRIPT *)NuLinkedListGetNext(&sys->scripts, &script->list_node);
            }
        }

        if (check_global_scripts) {
            script = (AISCRIPT *)NuLinkedListGetHead(&global_aiscripts);

            while (script != NULL) {
                if (NuStrICmp(name, script->name) == 0) {
                    return script;
                }

                script = (AISCRIPT *)NuLinkedListGetNext(&global_aiscripts, &script->list_node);
            }
        }
    }

    if (can_use_default) {
        script = (AISCRIPT *)NuLinkedListGetHead(&global_aiscripts);

        while (script != NULL) {
            if (NuStrICmp("default", script->name) == 0) {
                return script;
            }

            script = (AISCRIPT *)NuLinkedListGetNext(&global_aiscripts, &script->list_node);
        }
    }

    return NULL;
}

static __used__ void creatureEditor_cbScriptParams(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    char label[64];
    if (creatureEditor_Current() == nullptr)
        return;
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Script Params");
    if (menu != nullptr) {

        if (NuLinkedListGetHead(creatureEditor_AreaList()) != nullptr) {
            if (creatureEditor_Current()->trigger_area == nullptr) {
                strcpy(label, "Trigger Area NONE");
            } else {
                sprintf(label, "Trigger Area \"%s\"",
                        reinterpret_cast<char *>(creatureEditor_Current()->trigger_area) + 8);
            }
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbSelectTriggerArea, label));
        }
        if (NuLinkedListGetHead(creatureEditor_LocatorList()) != nullptr) {
            if (creatureEditor_Current()->locator != nullptr) {
                sprintf(label, "Locator \"%s\"", reinterpret_cast<char *>(creatureEditor_Current()->locator) + 8);
            } else {
                strcpy(label, "Locator NONE");
            }
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbSelectLocator, label));
        }
        AISCRIPT *script = AIScriptFind(aieditor->ai_system, creatureEditor_Current()->script_name, 1, 1, 1);
        if (script == nullptr) {
            sprintf(label, "Param%d", 0);
            eduiMenuAddItem(menu, eduiItemSliderCreate(0, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[0], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
            sprintf(label, "Param%d", 1);
            eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[1], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
            sprintf(label, "Param%d", 2);
            eduiMenuAddItem(menu, eduiItemSliderCreate(2, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[2], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
            sprintf(label, "Param%d", 3);
            eduiMenuAddItem(menu, eduiItemSliderCreate(3, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[3], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
        } else {
            const char *param_name = script->params[0].name;
            if (param_name != nullptr)
                sprintf(label, param_name);
            else
                sprintf(label, "Param%d", 0);
            eduiMenuAddItem(menu, eduiItemSliderCreate(0, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[0], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

            param_name = script != nullptr ? script->params[1].name : nullptr;
            if (param_name != nullptr)
                sprintf(label, param_name);
            else
                sprintf(label, "Param%d", 1);
            eduiMenuAddItem(menu, eduiItemSliderCreate(1, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[1], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

            param_name = script != nullptr ? script->params[2].name : nullptr;
            if (param_name != nullptr)
                sprintf(label, param_name);
            else
                sprintf(label, "Param%d", 2);
            eduiMenuAddItem(menu, eduiItemSliderCreate(2, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[2], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);

            param_name = script != nullptr ? script->params[3].name : nullptr;
            if (param_name != nullptr)
                sprintf(label, param_name);
            else
                sprintf(label, "Param%d", 3);
            eduiMenuAddItem(menu, eduiItemSliderCreate(3, attr, 0, creatureEditor_cbSetScriptParam, 0.0f, 100.0f,
                                                       aieditorsettings.current_script_params[3], label));
            eduiItemSliderSetGranularity(reinterpret_cast<edui_slider_s *>(edui_last_item), 0.1f);
        }
        reset_params_option = nullptr;
        if ((aieditorsettings.current_script_flags & 0x1e) != 0) {
            reset_params_option = eduiMenuAddItem(
                menu, eduiItemToggleCreate(1, attr, 1, 1, creatureEditor_cbResetParams, "Reset Params"));
        }
        eduiMenuAttach(parent, menu);
    }
}

static void AIScriptFindRefScripts(AISYS *sys, AISCRIPT *script) {
    AIREFSCRIPT *ref;
    AIREFSCRIPT *next;
    AICONDITION *condition;
    AISTATE *state;

    ref = (AIREFSCRIPT *)NuLinkedListGetHead(&script->ref_scripts);

    while (ref != NULL) {
        next = (AIREFSCRIPT *)NuLinkedListGetNext(&script->ref_scripts, &ref->list_node);

        if (!sys->unknown_flag_4 || ref->script == NULL) {
            ref->script = AIScriptFind(sys, ref->name, 1, ref->check_level_scripts, ref->check_global_scripts);

            if (ref->script == NULL || script == ref->script) {
                NuLinkedListRemove(&script->ref_scripts, &ref->list_node);
                ref = next;

                continue;
            }

            ref->return_state = AIStateFind(ref->return_state_name, script);

            condition = (AICONDITION *)NuLinkedListGetHead(&ref->conditions);

            while (condition != NULL) {
                if (condition->def->init_fn != NULL) {
                    condition->void_arg = (*condition->def->init_fn)(sys, condition->arg, script);
                }

                condition = (AICONDITION *)NuLinkedListGetNext(&ref->conditions, &condition->list_node);
            }
        }

        ref = next;
    }

    state = (AISTATE *)NuLinkedListGetHead(&script->states);

    while (state != NULL) {
        ref = (AIREFSCRIPT *)NuLinkedListGetHead(&state->ref_scripts);

        while (ref != NULL) {
            next = (AIREFSCRIPT *)NuLinkedListGetNext(&state->ref_scripts, &ref->list_node);

            if (!sys->unknown_flag_4 || ref->script == NULL) {
                ref->script = AIScriptFind(sys, ref->name, 1, ref->check_level_scripts, ref->check_global_scripts);

                if (ref->script == NULL || script == ref->script) {
                    NuLinkedListRemove(&script->ref_scripts, &ref->list_node);
                    ref = next;

                    continue;
                }

                ref->return_state = AIStateFind(ref->return_state_name, script);

                condition = (AICONDITION *)NuLinkedListGetHead(&ref->conditions);

                while (condition != NULL) {
                    if (condition->def->init_fn != NULL) {
                        condition->void_arg = (*condition->def->init_fn)(sys, condition->arg, script);
                    }

                    condition = (AICONDITION *)NuLinkedListGetNext(&ref->conditions, &condition->list_node);
                }
            }

            ref = next;
        }

        state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
    }
}

extern "C" {
    char *AIScriptNameFromIx(AISYS *system, i32 index) {
        i32 current_index = 0;
        if (system != NULL) {
            AISCRIPT *script = reinterpret_cast<AISCRIPT *>(NuLinkedListGetHead(&system->scripts));
            while (script != NULL) {
                if (current_index == index) {
                    return script->name;
                }
                script = reinterpret_cast<AISCRIPT *>(NuLinkedListGetNext(&system->scripts, &script->list_node));
                ++current_index;
            }
        }

        AISCRIPT *script = reinterpret_cast<AISCRIPT *>(NuLinkedListGetHead(&global_aiscripts));
        while (script != NULL) {
            AISCRIPT *level_script = NULL;
            if (system != NULL) {
                level_script = reinterpret_cast<AISCRIPT *>(NuLinkedListGetHead(&system->scripts));
                while (level_script != NULL) {
                    if (NuStrICmp(script->name, level_script->name) == 0) {
                        break;
                    }
                    level_script =
                        reinterpret_cast<AISCRIPT *>(NuLinkedListGetNext(&system->scripts, &level_script->list_node));
                }
            }
            if (level_script == NULL) {
                if (current_index == index) {
                    return script->name;
                }
                ++current_index;
            }
            script = reinterpret_cast<AISCRIPT *>(NuLinkedListGetNext(&global_aiscripts, &script->list_node));
        }
        return NULL;
    }

}

static __used__ void creatureEditor_cbSelectScript(eduimenu_s *parent, eduiitem_s *, unsigned int) {
    eduimenu_s *menu = eduiMenuCreate(220, 70, 240, 250, ed_fnt, creatureEditor_cbCancelMenu, "Select Script");
    if (menu == nullptr)
        return;
    for (i32 index = 0;; ++index) {
        char *name = AIScriptNameFromIx(aieditor->ai_system, index);
        if (name == nullptr)
            break;
        if (NuStrICmp(name, "Level") == 0)
            continue;
        CreatureEditorRecord *creature = creatureEditor_Current();
        if (creature != nullptr && NuStrICmp(creature->script_name, name) == 0) {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 1, 1, creatureEditor_cbSetScript, name));
            menu->selected = edui_last_item;
            eduiMenuAttach(parent, menu);
        } else {
            eduiMenuAddItem(menu, eduiItemCheckCreate(index, attr, 0, 1, creatureEditor_cbSetScript, name));
            eduiMenuAttach(parent, menu);
        }
    }
}

eduimenu_s *areaEditor_Process(nupad_s *pad) {
    if ((pad->digital_buttons_pressed & 0x80) != 0) {
        return areaEditorOptionsMenu();
    }
    area_hovered() = areaEditorFindHover();
    EDAIAREA_s *selected;
    u32 buttons;
    u32 pressed;
    if ((pad->digital_buttons & 0x40) != 0) {
        if (area_hovered() == NULL) {
            if ((pad->digital_buttons_pressed & 0x40) != 0) {
                areaEditorCreateArea();
            }
        } else {
            if ((pad->digital_buttons_pressed & 0x40) != 0) {
                area_selected() = area_hovered();
                aieditorsettings.area_rotation = area_selected()->rotation;
                edcamSetPos(&area_selected()->position);
            } else {
                EDAIAREA_s *selected = area_selected();
                if (selected == NULL)
                    return NULL;
                selected->position = aieditor->camera_position;
            }
            EDAIAREA_s *selected = area_selected();
            if (selected == NULL)
                return NULL;
            u32 buttons = pad->digital_buttons;
            if ((selected->flags & 1) != 0) {
                if (buttons & 0x2000)
                    selected->size.x *= 1.01f;
                else if (buttons & 0x8000)
                    selected->size.x *= 0.99f;
                selected->size.z = selected->size.x;
            } else {
                if (buttons & 0x2000)
                    selected->size.x *= 1.01f;
                else if (buttons & 0x8000)
                    selected->size.x *= 0.99f;
                if (buttons & 0x1000)
                    selected->size.z *= 1.01f;
                else if (buttons & 0x4000)
                    selected->size.z *= 0.99f;
            }
        }
        goto process_done;
    }
    if ((pad->digital_buttons_pressed & 0x10) != 0) {
        if (area_selected() != NULL && area_selected() == area_hovered()) {
            return areaEditorDeleteMenu();
        }
        goto process_done;
    }
    if ((pad->digital_buttons_pressed & 0x100) != 0) {
        EDAIAREA_s *&selection = area_selected();
        EDAIAREA_s *nearest = NULL;
        f32 distance = 3.402823466e38f;
        for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
            NUVEC difference;
            f32 current = NuVecXZDistSqr(&aieditor->camera_position, &area->position, &difference);
            if (!(current < distance))
                continue;
            nearest = area;
            distance = current;
        }
        selection = nearest;
        nearest = area_selected();
        if (nearest != NULL) {
            edcamSetPos(&nearest->position);
        }
        goto process_done;
    }
    buttons = pad->digital_buttons;
    pressed = pad->digital_buttons_pressed;
    selected = area_selected();
    if (buttons & 0x2000 || buttons & 0x8000) {
        if (selected != NULL && selected == area_hovered()) {
            aieditorsettings.area_rotation = selected->rotation;
        }
        if (!(buttons & 0x2000)) {
            area_rotation_step() = pressed & 0x2000 ? 20 : area_rotation_step() + 20;
            if (area_rotation_step() > 600) {
                area_rotation_step() = 600;
            }
            aieditorsettings.area_rotation = NuAngSub(aieditorsettings.area_rotation, area_rotation_step());
        } else {
            area_rotation_step() = pressed & 0x8000 ? 20 : area_rotation_step() + 20;
            if (area_rotation_step() > 600) {
                area_rotation_step() = 600;
            }
            aieditorsettings.area_rotation = NuAngAdd(aieditorsettings.area_rotation, area_rotation_step());
        }
        selected = area_selected();
        if (selected != NULL && selected == area_hovered()) {
            selected->rotation = static_cast<i16>(aieditorsettings.area_rotation);
        }
        goto process_done;
    }
    if (buttons & 0x4000 || buttons & 0x1000) {
        if (selected != NULL && selected == area_hovered()) {
            selected->size.y *= buttons & 0x4000 ? 0.99f : 1.01f;
        }
        goto process_done;
    }
    if ((buttons & 0x100) != 0 && (pressed & 0x0a) != 0) {
        EDAIAREA_s *next = NULL;
        if (pressed & 0x08) {
            if (area_selected() != NULL) {
                EDAIAREA_s *&selection = area_selected();
                selection = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetNext(area_list(), &selection->link));
            }
            if (area_selected() == NULL) {
                EDAIAREA_s *&selection = area_selected();
                selection = area_head();
            }
        } else if (pressed & 0x02) {
            if (area_selected() != NULL) {
                EDAIAREA_s *&selection = area_selected();
                selection = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetPrev(area_list(), &selection->link));
            }
            if (area_selected() == NULL) {
                EDAIAREA_s *&selection = area_selected();
                selection = reinterpret_cast<EDAIAREA_s *>(NuLinkedListGetTail(area_list()));
            }
        }
        next = area_selected();
        if (next != NULL) {
            edcamSetPos(&next->position);
        }
    }
process_done:
    selected = area_selected();
    if (selected != NULL && (selected->flags & 1) != 0 && selected->size.x != selected->size.z) {
        selected->size.x = NuFmin(selected->size.x, selected->size.z);
    }
    return NULL;
}

f32 AiParseExpression(char *expr) {
    f32 value;

    ExpressionRequiredNameLookup = 0;
    ExpressionNameLookupFailed = 0;
    AiParseExpressionFailed = 0;

    value = NuRDPFVar(expr, AiParseExpressionNameLoopup);

    if (ExpressionNameLookupFailed != 0) {
        AiParseExpressionFailed = 1;
    }

    return value;
}

static void xConditions(NUFPAR *parser) {
    i32 complex_idx = 0;

    while (NuFParGetLine(parser) != 0) {
        AICONDITION *cond;
        AICONDITIONDEF *def;

        NuFParGetWord(parser);

        if (NuStrICmp(parser->word_buf, "}") == 0) {
            return;
        }

        if (NuStrICmp(parser->word_buf, "IF") == 0) {
            NuFParGetWord(parser);

            def = AIConditionFind(parser->word_buf);
            if (def == NULL) {
                continue;
            }

            cond = (AICONDITION *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AICONDITION));
            if (cond == NULL) {
                continue;
            }

            memset(cond, 0, sizeof(AICONDITION));

            NuLinkedListAppend(load_conditionshdr, &cond->list_node);

            cond->def = def;

            NuFParGetWord(parser);

            if (NuStrICmp(parser->word_buf, "=") == 0 || NuStrICmp(parser->word_buf, "==") == 0) {
                cond->type = EQ;
            } else if (NuStrICmp(parser->word_buf, "<") == 0) {
                cond->type = LT;
            } else if (NuStrICmp(parser->word_buf, ">") == 0) {
                cond->type = GT;
            } else if (NuStrICmp(parser->word_buf, "<=") == 0) {
                cond->type = LT_EQ;
            } else if (NuStrICmp(parser->word_buf, ">=") == 0) {
                cond->type = GT_EQ;
            } else if (NuStrICmp(parser->word_buf, "!=") == 0) {
                cond->type = NEQ;
            } else {
                cond->arg = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);

                NuFParGetWord(parser);

                if (NuStrICmp(parser->word_buf, "=") == 0 || NuStrICmp(parser->word_buf, "==") == 0) {
                    cond->type = EQ;
                } else if (NuStrICmp(parser->word_buf, "<") == 0) {
                    cond->type = LT;
                } else if (NuStrICmp(parser->word_buf, ">") == 0) {
                    cond->type = GT;
                } else if (NuStrICmp(parser->word_buf, "<=") == 0) {
                    cond->type = LT_EQ;
                } else if (NuStrICmp(parser->word_buf, ">=") == 0) {
                    cond->type = GT_EQ;
                } else if (NuStrICmp(parser->word_buf, "!=") == 0) {
                    cond->type = NEQ;
                }
            }

            NuFParGetWord(parser);

            if (cond->param_cond == NULL) {
                u8 param_idx;
                char *param;

                param = NuStrIStr(parser->word_buf, "param");
                if (param == NULL) {
                    for (i32 i = 0; i < 4; i++) {
                        char *param_name = load_aiscript->params[i].name;

                        if (param_name != NULL && NuStrICmp(param_name, parser->word_buf) == 0) {
                            cond->param_idx = i;
                            *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) |= 4;

                            break;
                        }
                    }
                } else {
                    param_idx = NuAToI(param);
                    cond->param_idx = param_idx;

                    if (param_idx < 4) {
                        *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) |= 4;
                    }
                }

                if (!(*(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) & 4)) {
                    i32 i;

                    for (i = 0; i < aiscript_const_curr; i++) {
                        if (NuStrICmp(aiscript_const[i].name, parser->word_buf) == 0) {
                            cond->param_val = aiscript_const[i].default_val;

                            break;
                        }
                    }

                    if (i == aiscript_const_curr) {
                        cond->param_val = AiParseExpression(parser->word_buf);

                        if (AiParseExpressionFailed) {
                            *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) |= 8;
                            cond->param_idx = complex_idx++;
                            cond->complex_arg = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);
                        }
                    }
                }
            }

            NuFParGetWord(parser);

            if (NuStrICmp(parser->word_buf, "goto") == 0) {
                ConditionsParseFinalise(cond, parser);
            } else if (NuStrICmp(parser->word_buf, "and") == 0) {
                *(reinterpret_cast<u8 *>(cond) + offsetof(AICONDITION, param_idx) + 1) |= 1;
            }
        } else if (NuStrICmp(parser->word_buf, "OnMacro") == 0) {
            AICONDITIONMACRO *macro;

            NuFParGetWord(parser);

            macro = (AICONDITIONMACRO *)NuLinkedListGetHead(&load_aiscript->condition_macros);
            while (macro != NULL) {
                if (NuStrICmp(macro->name, parser->word_buf) == 0) {
                    AICONDITION *macro_last;

                    AIScriptCopyConditions(&macro->conditions, load_conditionshdr, load_buff, load_endbuff);
                    macro_last = (AICONDITION *)NuLinkedListGetTail(load_conditionshdr);

                    NuFParGetWord(parser);

                    if (NuStrICmp(parser->word_buf, "goto") == 0) {
                        ConditionsParseFinalise(macro_last, parser);
                    } else if (NuStrICmp(parser->word_buf, "and") == 0) {
                        *(reinterpret_cast<u8 *>(macro_last) + offsetof(AICONDITION, param_idx) + 1) |= 1;
                    }

                    break;
                }

                macro = (AICONDITIONMACRO *)NuLinkedListGetNext(&load_aiscript->condition_macros, &macro->list_node);
            }
        } else {
            NuStrChr(parser->word_buf, '{');
        }
    }
}

static void xRefScript(NUFPAR *parser) {
    AIREFSCRIPT *ref_script;
    VARIPTR orig_buf;
    i32 is_done;

    if (load_refscripthdr == NULL) {
        return;
    }

    orig_buf = *load_buff;

    ref_script = (AIREFSCRIPT *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AIREFSCRIPT));
    if (ref_script != NULL) {

        memset(ref_script, 0, sizeof(AIREFSCRIPT));

        is_done = false;
        while (!is_done && NuFParGetLine(parser) != 0) {
            while (!is_done && NuFParGetWord(parser) != 0) {
                char *cursor;

                if (NuStrICmp(parser->word_buf, "}") != 0) {
                    if ((cursor = NuStrIStr(parser->word_buf, "Script")) != NULL) {
                        ref_script->name = AIScriptCopyString(cursor + strlen("Script") + 1, load_buff, load_endbuff);
                    } else if ((cursor = NuStrIStr(parser->word_buf, "ReturnState")) != NULL) {
                        ref_script->return_state_name =
                            AIScriptCopyString(cursor + strlen("ReturnState") + 1, load_buff, load_endbuff);
                    } else if (NuStrICmp(parser->word_buf, "CONDITIONS") == 0) {
                        load_conditionshdr = &ref_script->conditions;
                        condition_has_no_goto = 1;

                        xConditions(parser);

                        load_conditionshdr = NULL;
                        condition_has_no_goto = 0;
                    } else if (NuStrIStr(parser->word_buf, "Source") != NULL) {
                        if (NuStrIStr(parser->word_buf, "Global") != NULL) {
                            ref_script->check_global_scripts = 1;
                        } else if (NuStrIStr(parser->word_buf, "Level") != NULL) {
                            ref_script->check_level_scripts = 1;
                        }
                    }
                } else {
                    is_done = true;
                }
            }
        }

        if (ref_script->name != NULL) {
            if (!ref_script->check_global_scripts && !ref_script->check_level_scripts) {
                ref_script->check_global_scripts = 1;
                ref_script->check_level_scripts = 1;
            }

            NuLinkedListAppend(load_refscripthdr, &ref_script->list_node);
        } else {
            *load_buff = orig_buf;
        }
    }
}

static void xConditionMacro(NUFPAR *parser) {
    AICONDITIONMACRO *macro;

    if (NuFParGetWord(parser) == 0) {
        return;
    }

    macro = (AICONDITIONMACRO *)AIScriptBufferAlloc(load_buff, load_endbuff, sizeof(AICONDITIONMACRO));
    if (macro == NULL) {
        return;
    }

    macro->list_node.next = NULL;
    macro->name = AIScriptCopyString(parser->word_buf, load_buff, load_endbuff);

    NuLinkedListAppend(&load_aiscript->condition_macros, &macro->list_node);

    load_conditionshdr = &macro->conditions;
    condition_has_no_goto = 1;

    xConditions(parser);

    load_conditionshdr = NULL;
    condition_has_no_goto = 0;
}

void AIScriptOpenPakFileParse(AISCRIPT **script_ref, void *pak, char *filename, char *path, VARIPTR *buf,
                              VARIPTR *buf_end) {
    char script_name[64];
    char *ext;
    char str_buf[128];
    NUFPAR *parser;
    AISCRIPT *script;
    i32 item_handle;
    void *item_buf;
    i32 size;
    NUFILE file;

    NuStrCpy(script_name, filename);

    ext = NuStrIStr(script_name, ".scp");
    if (ext == NULL) {
        return;
    }

    *ext = '\0';

    sprintf(str_buf, "%s\\%s", path, filename);

    if (pak != NULL) {
        item_handle = NuFilePakGetItem(pak, filename);
        if (item_handle == 0) {
            goto no_pak;
        } else {
            NuFilePakGetItemInfo(pak, item_handle, &item_buf, &size);
            file = NuMemFileOpen(item_buf, size, NUFILE_READ);

            if (file != 0) {
                parser = NuFParOpen(file);

            } else {
                goto no_pak;
            }
        }
    } else {
    no_pak:
        parser = NuFParCreate(str_buf);
    }

    if (parser == NULL) {
        return;
    }

    if (*script_ref == NULL) {
        *script_ref = (AISCRIPT *)AIScriptBufferAlloc(buf, buf_end, sizeof(AISCRIPT));
        if (*script_ref == NULL) {
            goto done;
        }

        memset(*script_ref, 0, sizeof(AISCRIPT));

        NuStrCpy(str_buf, filename);

        ext = NuStrIStr(str_buf, ".scp");
        if (ext != NULL) {
            *ext = '\0';
        }

        (*script_ref)->name = AIScriptCopyString(str_buf, buf, buf_end);

        if (*script_ref == NULL) {
            goto done;
        }
    }

    script = *script_ref;

    load_buff = buf;
    load_endbuff = buf_end;
    load_aiscript = script;
    aiscript_const_curr = 0;

    NuFParPushCom(parser, cfgtab_Script);

    while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);

        load_refscripthdr = &script->ref_scripts;

        NuFParInterpretWord(parser);
    }

    load_buff = NULL;
    load_endbuff = NULL;
    load_aiscript = NULL;
    load_refscripthdr = NULL;

done:
    NuFParDestroy(parser);
}

static void xInclude(NUFPAR *parser) {
    AISCRIPT *tmp_script;
    VARIPTR *tmp_buf;
    VARIPTR *tmp_buf_end;

    tmp_script = load_aiscript;
    tmp_buf_end = load_endbuff;
    tmp_buf = load_buff;

    NuFParGetWord(parser);
    NuFParSuspend(parser);

    AIScriptOpenPakFileParse(&load_aiscript, load_pakfile, parser->word_buf, load_path, load_buff, load_endbuff);

    NuFParResume(parser);

    load_buff = tmp_buf;
    load_endbuff = tmp_buf_end;
    load_aiscript = tmp_script;
}

void AIScriptLoadAllPakFile(void *pak, char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS *sys) {
    LOG_INFO("script load path=%s system=%p", path != NULL ? path : "<none>", (void *)sys);
    NULISTHDR *scripts;
    i32 item_idx;
    void *memfile;
    i32 memfile_size;
    NUFILE file;
    char filepath[0x80];
    char script_files[0x80][0x18];
    NUFPAR *parser;
    i32 i;
    i32 j;
    AISCRIPT *script;
    AISTATE *state;
    AICONDITION *condition;
    i32 is_done;

    if (sys != NULL) {
        scripts = &sys->scripts;
    } else {
        scripts = &global_aiscripts;
    }

    file = 0;
    if (pak != NULL && (item_idx = NuFilePakGetItem(pak, "script.txt")) != 0) {
        NuFilePakGetItemInfo(pak, item_idx, &memfile, &memfile_size);

        file = NuMemFileOpen(memfile, memfile_size, NUFILE_READ);
    }

    if (file == 0) {
        sprintf(filepath, "%s\\script.txt", path);

        file = NuFileOpen(filepath, NUFILE_READ);
    }

    if (file != 0) {
        parser = NuFParOpen(file);
        i = 0;

        if (parser != NULL) {
            while (NuFParGetLine(parser) != 0) {
                NuFParGetWord(parser);
                NuStrLen(parser->word_buf);
                NuStrCpy(script_files[i], parser->word_buf);
                NuStrCat(script_files[i], ".scp");

                i++;
            }

            NuFParClose(parser);
        }

        NuFileClose(file);

        for (j = 0; j < i; ++j) {
            script = AIScriptLoadPakFile(pak, script_files[j], path, buf, buf_end);

            if (script != NULL) {
                NuLinkedListAppend(scripts, &script->list_node);

                if (sys != NULL) {
                    script->is_level_script = 1;
                }
            }
        }
    }

    if (sys != NULL) {
        do {
            is_done = 1;
            script = (AISCRIPT *)NuLinkedListGetHead(&sys->scripts);

            while (script != NULL) {
                if (AIScriptBuildDerivedScript(script, buf, buf_end, sys) == 0) {
                    is_done = 0;
                }

                script = (AISCRIPT *)NuLinkedListGetNext(&sys->scripts, &script->list_node);
            }
        } while (!is_done);
    }
}

void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS *sys) {
    void *pak;
    VARIPTR pak_start;
    char filepath[0x80];
    i32 pak_size;

    pak_start = *buf_end;

    if (ai_usepackfile) {
        sprintf(filepath, "%s\\ai.pak", path);

        pak_size = NuFileSize(filepath);
        if (pak_size > 0) {
            pak_start.addr = buf_end->addr - ALIGN(pak_size + 1, 0x10);
            pak = NuFilePakLoad(filepath, &pak_start, *buf_end, 0x10);
        } else {
            pak = NULL;
        }
    } else {
        pak = NULL;
    }

    AIScriptLoadAllPakFile(pak, path, buf, &pak_start, sys);
}

void AIScriptInitConditions(AISYS *sys) {
    AISCRIPT *script;
    AISTATE *state;
    AICONDITION *condition;
    AICONDITION *param_cond;

    NuLinkedListGetHead(&global_aiscripts);

    if (sys != NULL) {
        script = (AISCRIPT *)NuLinkedListGetHead(&sys->scripts);

        while (script != NULL) {
            state = (AISTATE *)NuLinkedListGetHead(&script->states);

            while (state != NULL) {
                condition = (AICONDITION *)NuLinkedListGetHead(&state->conditions);

                while (condition != NULL) {
                    if (condition->def->init_fn != NULL) {
                        condition->void_arg = (*condition->def->init_fn)(sys, condition->arg, script);
                    }

                    param_cond = condition->param_cond;
                    if (param_cond != NULL && param_cond->def != NULL && param_cond->def->init_fn != NULL) {
                        param_cond->void_arg = (*param_cond->def->init_fn)(sys, param_cond->arg, script);
                    }

                    condition = (AICONDITION *)NuLinkedListGetNext(&state->conditions, &condition->list_node);
                }

                state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
            }

            script->base_state = AIStateFind("Base", script);

            AIScriptFindRefScripts(sys, script);

            script = (AISCRIPT *)NuLinkedListGetNext(&sys->scripts, &script->list_node);
        }
    }

    script = (AISCRIPT *)NuLinkedListGetHead(&global_aiscripts);

    while (script != NULL) {
        state = (AISTATE *)NuLinkedListGetHead(&script->states);

        while (state != NULL) {
            condition = (AICONDITION *)NuLinkedListGetHead(&state->conditions);

            while (condition != NULL) {
                if (condition->def->init_fn != NULL) {
                    if (sys == NULL) {
                        condition->void_arg = (*condition->def->init_fn)(NULL, condition->arg, script);
                    } else if (!sys->unknown_flag_4 || condition->void_arg == NULL) {
                        condition->void_arg = (*condition->def->init_fn)(sys, condition->arg, script);
                    }
                }

                condition = (AICONDITION *)NuLinkedListGetNext(&state->conditions, &condition->list_node);
            }

            state = (AISTATE *)NuLinkedListGetNext(&script->states, &state->list_node);
        }

        script->base_state = AIStateFind("Base", script);

        if (sys != NULL) {
            AIScriptFindRefScripts(sys, script);
        }

        script = (AISCRIPT *)NuLinkedListGetNext(&global_aiscripts, &script->list_node);
    }
}

void AIScriptSetState(AISCRIPTPROCESS *processor, AISTATE *state) {
    NULISTLNK *action_node;

    if (state != NULL && processor != NULL) {
        LOG_INFO_IF(WORLD != NULL && (uintptr_t)processor >= (uintptr_t)&WORLD->processors[0] &&
                        (uintptr_t)processor < (uintptr_t)&WORLD->processors[32] && processor->state != state,
                    "level-script state processor=%p script=%s from=%s to=%s", (void *)processor,
                    processor->script != NULL ? processor->script->name : "<none>",
                    processor->state != NULL ? processor->state->name : "<none>", state->name);
        if (!processor->unknown_flag_4) {
            processor->unknown_b2 = 0;
        }

        processor->unknown_flag_4 = 0;

        processor->state = state;
        processor->action_node = NULL;
        processor->next_state = NULL;
        processor->param_stack[0].is_first_time_state = 1;

        action_node = NuLinkedListGetHead(&state->actions);

        if (action_node != NULL) {
            processor->action_node = action_node;
            processor->is_first_time_action = 1;
        } else {
            processor->action_node = NULL;
        }
    }
}

extern "C" {
    void AIScriptForceParamReEval(AISCRIPTPROCESS *processor) {
        if (processor != NULL) {
            processor->param_stack[0].is_first_time_state = 1;
            processor->param_stack[1].is_first_time_state = 1;
        }
    }

}

i32 AIScriptSetStateByName(AISCRIPTPROCESS *processor, char *name) {
    AISTATE *state;

    if (name != NULL && processor != NULL && processor->script != NULL) {
        state = AIStateFind(name, processor->script);

        if (state != NULL) {
            AIScriptSetState(processor, state);

            return 1;
        }
    }

    return 0;
}

void AIScriptProcessorInit(AISYS *sys, AIPACKET *packet, AISCRIPTPROCESS *processor, AICREATURE *creature,
                           char *script_name, char *start_state_name, i32 can_use_default, AISCRIPT *script,
                           AISTATE *start_state) {
    i32 i;

    if (processor == NULL) {
        return;
    }

    processor->unknown_flag_4 = 0;
    processor->unknown_b2 = 0;

    if (script == NULL) {
        script = AIScriptFind(sys, script_name, can_use_default, 1, 1);

        processor->local_messages = NULL;
    }

    if (script != NULL) {
        processor->script = script;
        LOG_INFO_IF(packet == NULL, "level-script init processor=%p script=%s start=%s", (void *)processor,
                    script->name, start_state_name != NULL ? start_state_name : "Base");
        processor->next_state = NULL;

        if (processor->base_script == NULL) {
            processor->base_script = script;
        }

        if (creature != NULL) {
            for (i = 0; i < 4; i++) {
                processor->params[i] = creature->script_params[i];
            }

            for (i = 0; i < 4; i++) {
                if ((creature->flags & (1 << (i + 1))) == 0) {
                    processor->params[i] = script->params[i].default_val;
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                processor->params[i] = script->params[i].default_val;
            }
        }

        if (start_state != NULL ||
            (start_state_name != NULL && (start_state = AIStateFind(start_state_name, script)) != NULL)) {
            AIScriptSetState(processor, start_state);
        } else {
            AIScriptSetStateByName(processor, "Base");
        }
    }

    processor->interrupt_timer = 0.0f;
    processor->interrupt_priority = 0;
    processor->interrupt_id = 0;
    processor->interrupt_state = NULL;

    processor->return_to_state = NULL;
}

i32 AIScriptSetBaseScriptStateByName(AISCRIPTPROCESS *processor, char *name) {
    AISTATE *state;

    if (name != NULL && processor != NULL && processor->script != NULL) {
        state = AIStateFind(name, processor->base_script);

        if (state != NULL) {
            if (processor->script == processor->base_script) {
                AIScriptSetState(processor, state);

                return 1;
            }

            AIScriptProcessorInit(NULL, NULL, processor, NULL, NULL, NULL, 0, processor->base_script, state);

            processor->active_ref_count = 0;

            return 1;
        }
    }

    return 0;
}

f32 AIParamToFloat(AISCRIPTPROCESS *processor, char *param) {
    char *cursor;
    u32 i;

    if (param == NULL) {
        return 0.0f;
    }

    if (processor != NULL) {
        cursor = NuStrIStr(param, "param");

        if (cursor != NULL) {
            i = NuAToI(cursor);

            if (i < 4) {
                return processor->params[i];
            }
        } else if (processor->script != NULL) {
            for (i = 0; i < 4; i++) {
                if (processor->script->params[i].name != NULL &&
                    NuStrICmp(processor->script->params[i].name, param) == 0) {
                    return processor->params[i];
                }
            }
        }
    }

    return NuAToF(param);
}

static i32 AiEvalExpressionNameLoopup(char *expr, f32 *float_out, i32 *int_out) {
    f32 value;
    i32 got_value;

    got_value = 0;
    value = 0.0f;

    if (NuStrICmp(expr, "Rand") == 0) {
        got_value = 1;
        value = NuRandFloat();
    } else if (NuStrICmp(expr, "false") == 0) {
        got_value = 1;
        value = 0.0f;
    } else if (NuStrICmp(expr, "true") == 0) {
        got_value = 1;
        value = 1.0f;
    } else if (NuStrICmp(expr, "max") == 0) {
        got_value = 1;
        value = 3.4028235e+38f;
    }

    if (!got_value) {
        if (GameParamToFloatFn != NULL) {
            got_value = (*GameParamToFloatFn)(AiEvalExpressionPacket, AiEvalExpressionProcessor, expr, &value);
        }

        if (!got_value) {
            value = AIParamToFloat(AiEvalExpressionProcessor, expr);
        }
    }

    if (float_out != NULL) {
        *float_out = value;
    }

    if (int_out != NULL) {
        *int_out = value;
    }

    return 0;
}

f32 AIParamToFloatEx(AIPACKET *packet, AISCRIPTPROCESS *processor, char *param) {
    char *cursor;
    f32 result;
    i32 is_number;

    is_number = 1;
    cursor = param;

    do {
        if (*cursor == '\0') {
            break;
        }

        // The original accepts signed character values from -45 through '9',
        // except '/'; this is its numeric fast path, not a digit-only test.
        if (*cursor < -45 || *cursor > '9' || *cursor == '/') {
            is_number = 0;
            break;
        }

        cursor++;
    } while (true);

    if (is_number) {
        result = NuAToF(param);
    } else {
        AiEvalExpressionProcessor = processor;
        AiEvalExpressionPacket = packet;

        result = NuRDPFVar(param, AiEvalExpressionNameLoopup);

        AiEvalExpressionProcessor = NULL;
    }

    return result;
}

__used__ static i32 Action_SetParam(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32, f32) {
    i32 index;
    i32 i;
    char *value;
    i64 flag;
    AICREATURE *creature;

    if (packet != NULL && processor != NULL && processor->script != NULL) {
        creature = NULL;
        if (packet->field_0x134 != 0xff) {
            creature = &sys->creatures[packet->field_0x134];
        }
        for (i = 0; i < param_count - 1; i++) {
            for (index = 0; index < 4; index++) {
                if (NuStrICmp(params[i], processor->script->params[index].name) == 0) {
                    break;
                }
            }
            if (index < 4) {
                flag = 1LL << (index + 1);
                i++;
                if (NuStrICmp(params[i], "default") == 0) {
                    if (creature != NULL && (creature->flags & flag) != 0) {
                        processor->params[index] = creature->script_params[index];
                    } else {
                        processor->params[index] = processor->script->params[index].default_val;
                    }
                } else {
                    // Expressions use the packet's primary processor, even for a secondary action processor.
                    if ((value = NuStrIStr(params[i], "inc=")) == NULL) {
                        if ((value = NuStrIStr(params[i], "dec=")) == NULL) {
                            processor->params[index] = AIParamToFloatEx(packet, &packet->script_process, params[i]);
                        } else {
                            processor->params[index] -=
                                AIParamToFloatEx(packet, &packet->script_process, value + NuStrLen("dec="));
                        }
                    } else {
                        processor->params[index] +=
                            AIParamToFloatEx(packet, &packet->script_process, value + NuStrLen("inc="));
                    }
                }
            }
        }
    }
    return 1;
}

static __used__ i32 Action_OverrideAnimation(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || first_time == 0) {
        return 1;
    }

    i16 from = -1;
    i16 to = -1;
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "from=All") == 0) {
            from = static_cast<i16>(apicharsys->model_id_capacity);
            continue;
        }

        char *value = NuStrIStr(params[index], "from=");
        if (value != NULL) {
            from = static_cast<i16>(FindAnimIX(packet->owner->apiobj.character_data, value + 5));
            continue;
        }

        value = NuStrIStr(params[index], "to=");
        if (value != NULL) {
            to = static_cast<i16>(FindAnimIX(packet->owner->apiobj.character_data, value + 3));
            continue;
        }

        if (processor != NULL) {
            processor->action_timer = AIParamToFloatEx(packet, processor, params[0]);
        }
    }

    if (to == -1) {
        from = -1;
    }
    packet->animation_override_from = from;
    packet->animation_override_to = to;
    return 1;
}

__used__ static i32 Action_SetMoveRadius(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        packet->mover_height = packet->owner->apiobj.collision_radius * 2.0f;
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            packet->mover_height = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetHearDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff) {
            object->heardistance = sys->creatures[packet->field_0x134].hear_distance;
        } else if (GetHearDistanceFn != NULL) {
            object->heardistance = GetHearDistanceFn(object->character_model->model_id);
        } else {
            object->heardistance = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object = &packet->owner->apiobj;
            object->heardistance = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetMinViewHeight(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff) {
            object->minviewheight = sys->creatures[packet->field_0x134].min_view_height;
        } else if (GetMinViewHeightFn != NULL) {
            object->minviewheight = GetMinViewHeightFn(object->character_model->model_id);
        } else {
            object->minviewheight = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object = &packet->owner->apiobj;
            object->minviewheight = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetMaxViewHeight(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff) {
            object->maxviewheight = sys->creatures[packet->field_0x134].max_view_height;
        } else if (GetMaxViewHeightFn != NULL) {
            object->maxviewheight = GetMaxViewHeightFn(object->character_model->model_id);
        } else {
            object->maxviewheight = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object = &packet->owner->apiobj;
            object->maxviewheight = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetViewDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff) {
            object->viewdistance = sys->creatures[packet->field_0x134].view_distance;
        } else if (GetViewRangeFn != NULL) {
            object->viewdistance = GetViewRangeFn(object->character_model->model_id);
        } else {
            object->viewdistance = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object = &packet->owner->apiobj;
            object->viewdistance = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_FaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_count, i32 first_time, f32 elapsed) {
    (void)sys;
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(params[index], "mintime");
            if (value != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value + 8);
            } else if ((value = NuStrIStr(params[index], "maxtime")) != NULL) {
                max_time = AIParamToFloatEx(packet, processor, value + 8);
            } else if ((value = NuStrIStr(params[index], "faceoffset")) != NULL) {
                processor->action_data_4 = AIParamToFloatEx(packet, processor, value + 11);
            } else if (NuStrICmp(params[index], "nearest_opponent") == 0) {
                processor->action_data_1 = 1;
            } else {
                processor->action_timer = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
        if (processor->action_timer == 0.0f && min_time < max_time) {
            processor->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
        }
    }

    APIOBJECT *opponent = processor->action_data_1 != 0 ? packet->nearest_opponent_object : packet->opponent_object;
    if (opponent != NULL && opponent->ai != NULL) {
        if (processor->action_data_4 == 0.0f) {
            packet->movement_look_target = &opponent->position;
        } else {
            NUVEC direction = {opponent->position.z - packet->owner->apiobj.position.z, 0.0f,
                               packet->owner->apiobj.position.x - opponent->position.x};
            NuVecNorm(&direction, &direction);
            processor->action_pos.x = opponent->position.x + direction.x * processor->action_data_4;
            processor->action_pos.y = opponent->position.y;
            processor->action_pos.z = opponent->position.z + direction.z * processor->action_data_4;
            packet->movement_look_target = &processor->action_pos;
        }
    }
    if (processor->action_timer > 0.0f) {
        processor->action_timer -= elapsed;
        if (processor->action_timer <= 0.0f) {
            processor->action_timer = 0.0f;
            return 1;
        }
    }
    return 0;
}

__used__ static i32 Action_FacePlayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "mintime");
            if (value != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value + 8);
            } else if ((value = NuStrIStr(params[index], "maxtime")) != NULL) {
                max_time = AIParamToFloatEx(packet, processor, value + 8);
            } else {
                processor->action_timer = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
        if (processor->action_timer == 0.0f && min_time < max_time) {
            processor->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
        }
    }
    f32 remaining_time = processor->action_timer;
    if (sys->player_1 != NULL) {
        packet->movement_look_target = &sys->player_1->position;
    }
    if (remaining_time > 0.0f) {
        remaining_time -= elapsed;
        processor->action_timer = remaining_time;
        if (remaining_time <= 0.0f) {
            processor->action_timer = 0.0f;
            return 1;
        }
    }
    return 0;
}

static i32 Action_ResetTimer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                             i32 is_first_time, f32) {
    if (is_first_time == 0) {
        return 1;
    }

    f32 minimum = 0.0f;
    f32 maximum = 0.0f;
    f32 exact = 0.0f;
    for (i32 param_index = 0; param_index < param_count; ++param_index) {
        char *value = NuStrIStr(params[param_index], "mintime");
        if (value != NULL) {
            minimum = AIParamToFloatEx(packet, processor, value + 8);
            continue;
        }

        value = NuStrIStr(params[param_index], "maxtime");
        if (value != NULL) {
            maximum = AIParamToFloatEx(packet, processor, value + 8);
            continue;
        }

        value = NuStrIStr(params[param_index], "time");
        if (value != NULL) {
            exact = AIParamToFloatEx(packet, processor, value + 5);
        }
    }

    if (minimum != 0.0f || maximum != 0.0f) {
        processor->script_timer = NuRandFloat() * maximum + (1.0f - NuRandFloat()) * minimum;
    } else {
        processor->script_timer = exact;
    }
    return 1;
}

static i32 Action_Idle(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                       i32 is_first_time, f32 elapsed) {
    f32 minimum = 0.0f;
    f32 maximum = 0.0f;
    i32 frames = 0;
    i32 result = 0;
    char *value;

    if (is_first_time) {
        for (i32 i = 0; i < param_count; i++) {
            value = NuStrIStr(params[i], "mintime");
            if (value != NULL) {
                minimum = AIParamToFloatEx(packet, processor, value + NuStrLen("mintime") + 1);
            } else if ((value = NuStrIStr(params[i], "maxtime")) != NULL) {
                maximum = AIParamToFloatEx(packet, processor, value + NuStrLen("maxtime") + 1);
            } else if ((value = NuStrIStr(params[i], "frames")) != NULL) {
                frames = AIParamToFloatEx(packet, processor, value + NuStrLen("frames") + 1);
            } else {
                processor->action_timer = AIParamToFloatEx(packet, processor, params[i]);
            }
        }

        if (frames != 0) {
            processor->action_data_1 = frames < 0 ? 0 : (frames > 255 ? 255 : frames);
        } else if (processor->action_timer == 0.0f && maximum > minimum) {
            processor->action_timer = NuRandFloat() * (maximum - minimum) + minimum;
        }
    } else if (processor->action_data_1 != 0) {
        processor->action_data_1--;
        result = processor->action_data_1 == 0;
    } else if (processor->action_timer > 0.0f) {
        processor->action_timer -= elapsed;
        if (processor->action_timer <= 0.0f) {
            processor->action_timer = 0.0f;
            result = 1;
        }
    }
    return result;
}

__used__ static f32 Condition_Param(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *) {
    return AIParamToFloatEx(packet, processor, arg);
}

static AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions, AIPACKET *packet,
                                                   AISCRIPTPROCESS *processor, i32 stack_index) {
    if (processor == NULL || conditions == NULL) {
        return NULL;
    }

    AICONDITION *condition = reinterpret_cast<AICONDITION *>(NuLinkedListGetHead(conditions));
    f32 condition_value = 0.0f;

    while (condition != NULL) {
        i32 comparison_passed = 0;
        if (condition->def != NULL) {
            if (condition->def->eval_fn != NULL) {
                condition_value =
                    condition->def->eval_fn(system, processor, packet, condition->arg, condition->void_arg);
            }

            f32 comparison_value;
            if (condition->param_cond != NULL && condition->param_cond->def != NULL) {
                AICONDITIONFN *comparison_fn = condition->param_cond->def->eval_fn;
                comparison_value = comparison_fn != NULL
                                       ? comparison_fn(system, processor, packet, condition->arg, condition->void_arg)
                                       : condition_value;
            } else if (condition->is_complex) {
                if (processor->param_stack[stack_index].is_first_time_state) {
                    processor->param_stack[stack_index].complex_params[condition->param_idx] =
                        AIParamToFloatEx(packet, processor, condition->complex_arg);
                }
                comparison_value = processor->param_stack[stack_index].complex_params[condition->param_idx];
            } else if (condition->is_param_idx_valid) {
                comparison_value = processor->params[condition->param_idx];
            } else {
                comparison_value = condition->param_val;
            }

            switch (condition->type) {
                case AICONDITION_EQUAL:
                    if (condition_value == comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
                case AICONDITION_LESS_THAN:
                    if (condition_value < comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
                case AICONDITION_GREATER_THAN:
                    if (condition_value > comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
                case AICONDITION_LESS_THAN_OR_EQUAL:
                    if (condition_value <= comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
                case AICONDITION_GREATER_THAN_OR_EQUAL:
                    if (condition_value >= comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
                case AICONDITION_NOT_EQUAL:
                    if (condition_value != comparison_value) {
                        comparison_passed = 1;
                    }
                    break;
            }
        }

        if (!comparison_passed) {
            // A failed member invalidates the rest of its AND group.  The
            // first-state pass still snapshots complex operands so later
            // frames compare against the same values as the target.
            while (condition->bool_and) {
                condition = reinterpret_cast<AICONDITION *>(NuLinkedListGetNext(conditions, &condition->list_node));
                if (condition == NULL) {
                    break;
                }
                if (processor->param_stack[stack_index].is_first_time_state && condition->is_complex) {
                    processor->param_stack[stack_index].complex_params[condition->param_idx] =
                        AIParamToFloatEx(packet, processor, condition->complex_arg);
                    condition->param_val = processor->param_stack[stack_index].complex_params[condition->param_idx];
                }
            }
        } else if (!condition->bool_and) {
            return AIScriptFinishStateConditions(conditions, packet, processor, condition, stack_index);
        }

        if (condition != NULL) {
            condition = reinterpret_cast<AICONDITION *>(NuLinkedListGetNext(conditions, &condition->list_node));
        }
    }

    return AIScriptFinishStateConditions(conditions, packet, processor, NULL, stack_index);
}

extern "C" {
    void AIScriptProcess(AISYS *system, APIOBJECT *object, AIPACKET *packet, AISCRIPTPROCESS *processor, f32 elapsed) {
        if (processor == NULL || processor->script == NULL) {
            return;
        }

        processor->script_timer += elapsed;

        for (i32 index = 0; index < processor->active_ref_count; ++index) {
            AIREFSCRIPT *reference = processor->active_refs[index];
            if (NuLinkedListGetHead(&reference->conditions) != NULL &&
                AIScriptProcessStateConditions(system, &reference->conditions, packet, processor, 1) == NULL) {
                processor->active_ref_count = index;
                if (index == 0) {
                    AICREATURE *creature = packet->field_0x134 != 0xff ? &system->creatures[packet->field_0x134] : NULL;
                    AIScriptProcessorInit(system, packet, processor, creature, NULL, NULL, 0, processor->base_script,
                                          reference->return_state);
                } else {
                    AIScriptProcessorInit(system, packet, processor, NULL, NULL, NULL, 0,
                                          processor->active_refs[index - 1]->script, reference->return_state);
                }
                break;
            }
        }

        if (processor->next_state == NULL) {
            if (processor->active_ref_count != 0 && processor->state == processor->script->base_state) {
                const i32 completed_index = processor->active_ref_count - 1;
                AIREFSCRIPT *completed_reference = processor->active_refs[completed_index];
                processor->active_ref_count = completed_index;
                if (completed_index == 0) {
                    AICREATURE *creature = packet->field_0x134 != 0xff ? &system->creatures[packet->field_0x134] : NULL;
                    AIScriptProcessorInit(system, packet, processor, creature, NULL, NULL, 0, processor->base_script,
                                          completed_reference->return_state);
                } else {
                    AIScriptProcessorInit(system, packet, processor, NULL, NULL, NULL, 0,
                                          processor->active_refs[completed_index - 1]->script,
                                          completed_reference->return_state);
                }
            }
        }
        if (processor->next_state != NULL) {
            AIScriptSetState(processor, processor->next_state);
            processor->next_state = NULL;
        }

        if (processor->state == NULL) {
            AIScriptSetState(processor, reinterpret_cast<AISTATE *>(NuLinkedListGetHead(&processor->script->states)));
        }

        if (processor->state != NULL) {
            while (processor->action_node != NULL) {
                AIACTION *action = reinterpret_cast<AIACTION *>(processor->action_node);
                bool action_completed = true;
                if (action->def != NULL && action->def->eval_fn != NULL) {
                    i32 first_time = 0;
                    if (processor->is_first_time_action) {
                        processor->action_data_1 = 0;
                        processor->action_data_2 = 0;
                        processor->action_data_3 = NULL;
                        processor->action_data_6 = 0;
                        processor->action_data_4 = 0.0f;
                        processor->action_data_5 = 0.0f;
                        processor->action_data_7 = 0;
                        processor->action_timer = 0.0f;
                        processor->action_pos.x = processor->action_pos.y = processor->action_pos.z = 0.0f;
                        memset(&processor->path_info, 0, sizeof(processor->path_info));

                        if (packet != NULL) {
                            packet->goal_path_node = NULL;
                            packet->goal_speed_mode = 0;
                            packet->movement_instruction_parameter = 0.0f;
                            packet->frame_flags = 0;
                            if (ScriptProcessFirstTimeActionFn != NULL) {
                                ScriptProcessFirstTimeActionFn(system, packet, processor);
                            }
                        }

                        processor->is_first_time_action = 0;
                        first_time = 1;
                    }

                    LOG_INFO_IF(
                        packet == NULL && first_time,
                        "level-script action processor=%p script=%s state=%s action=%s params=%d [%s, %s, %s, %s]",
                        (void *)processor, processor->script->name, processor->state->name, action->def->name,
                        action->param_count, action->param_count > 0 ? action->params[0] : "",
                        action->param_count > 1 ? action->params[1] : "",
                        action->param_count > 2 ? action->params[2] : "",
                        action->param_count > 3 ? action->params[3] : "");
                    action_completed = action->def->eval_fn(system, processor, packet, action->params,
                                                            action->param_count, first_time, elapsed) != 0;
                    LOG_INFO_IF(packet == NULL && action_completed,
                                "level-script action complete processor=%p action=%s", (void *)processor,
                                action->def->name);
                    if (!action_completed) {
                        break;
                    }
                } else {
                    processor->is_first_time_action = 0;
                }

                NULISTLNK *action_node = processor->action_node;
                if (processor->is_first_time_action) {
                    continue;
                }
                if (action_node != NULL) {
                    action_node = NuLinkedListGetNext(&processor->state->actions, action_node);
                } else {
                    action_node = NuLinkedListGetHead(&processor->state->actions);
                }
                if (action_node != NULL) {
                    processor->is_first_time_action = 1;
                }
                processor->action_node = action_node;
            }
        }

        if (system != NULL && processor->script != NULL && processor->active_ref_count < 4) {
            NULISTHDR *references = &processor->script->ref_scripts;
            for (AIREFSCRIPT *reference = reinterpret_cast<AIREFSCRIPT *>(NuLinkedListGetHead(references));
                 reference != NULL; reference = reinterpret_cast<AIREFSCRIPT *>(
                                        NuLinkedListGetNext(&processor->script->ref_scripts, &reference->list_node))) {
                processor->param_stack[1].is_first_time_state = 1;
                processor->param_stack[1].force_complex_eval = 1;

                if (NuLinkedListGetHead(&reference->conditions) != NULL &&
                    AIScriptProcessStateConditions(system, &reference->conditions, packet, processor, 1) == NULL) {
                    continue;
                }
                if (reference->script == NULL) {
                    continue;
                }

                processor->param_stack[0].is_first_time_state = 1;
                processor->param_stack[0].force_complex_eval = 1;
                AICONDITION *entry_condition = AIScriptProcessStateConditions(
                    system, &reference->script->base_state->conditions, packet, processor, 0);
                if (entry_condition != NULL && entry_condition->next_state != reference->script->base_state) {
                    const i32 reference_index = processor->active_ref_count;
                    processor->active_refs[reference_index] = reference;
                    processor->active_ref_count = reference_index + 1;
                    AIScriptProcessorInit(system, packet, processor, NULL, NULL, NULL, 0, reference->script,
                                          entry_condition->next_state);
                    return;
                }
            }

            if (processor->state != NULL) {
                references = &processor->state->ref_scripts;
                for (AIREFSCRIPT *reference = reinterpret_cast<AIREFSCRIPT *>(NuLinkedListGetHead(references));
                     reference != NULL; reference = reinterpret_cast<AIREFSCRIPT *>(
                                            NuLinkedListGetNext(references, &reference->list_node))) {
                    processor->param_stack[1].is_first_time_state = 1;
                    processor->param_stack[1].force_complex_eval = 1;

                    if (NuLinkedListGetHead(&reference->conditions) != NULL &&
                        AIScriptProcessStateConditions(system, &reference->conditions, packet, processor, 1) == NULL) {
                        continue;
                    }
                    if (reference->script == NULL) {
                        continue;
                    }

                    processor->param_stack[0].is_first_time_state = 1;
                    processor->param_stack[0].force_complex_eval = 1;
                    AICONDITION *entry_condition = AIScriptProcessStateConditions(
                        system, &reference->script->base_state->conditions, packet, processor, 0);
                    if (entry_condition != NULL && entry_condition->next_state != reference->script->base_state) {
                        const i32 reference_index = processor->active_ref_count;
                        processor->active_refs[reference_index] = reference;
                        processor->active_ref_count = reference_index + 1;
                        AIScriptProcessorInit(system, packet, processor, NULL, NULL, NULL, 0, reference->script,
                                              entry_condition->next_state);
                        return;
                    }
                }
            }
        }

        if (processor->interrupt_timer > 0.0f) {
            processor->interrupt_timer -= elapsed;
            if (processor->interrupt_timer <= 0.0f && processor->interrupt_state != NULL) {
                AIScriptSetState(processor, processor->interrupt_state);
                processor->interrupt_priority = 0;
                processor->interrupt_state = NULL;
                return;
            }
        }

        if (processor->state != NULL) {
            AICONDITION *transition =
                AIScriptProcessStateConditions(system, &processor->state->conditions, packet, processor, 0);
            if (transition != NULL) {
                processor->unknown_flag_4 = transition->keep_blocked;
                AIScriptSetState(processor, transition->next_state);
            }
        }
    }

}

i32 IsFacingTarget(nuvec_s *first, nuvec_s *second, i32 facing_angle, i32 arc_degrees) {
    const i32 arc = (arc_degrees << 15) / 360;
    NUVEC direction;
    NuVecSub(&direction, first, second);

    i32 angle_difference = (NuAtan2D(direction.x, direction.z) - facing_angle) & 0xffff;
    if (angle_difference >= 0x8000)
        angle_difference -= 0x10000;
    angle_difference = abs(angle_difference);

    return arc > angle_difference;
}

extern "C" {
    void AiSysUsePackFile(i32 enabled) {
        ai_usepackfile = enabled;
    }

    void AiSysOnlyUsePakFile(i32 enabled) {
        ai_onlyusepackfile = enabled;
    }

    void AiSysSetStateDebugee(AISCRIPTPROCESS *processor) {
        pSetStateDebugee = processor;
    }

}

static __used__ i32 Action_NotifyStateChange(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char **params,
                                             i32 param_count, i32, f32) {
    i32 enabled = 1;
    for (i32 i = 0; i < param_count; i++) {
        if (NuStrICmp(params[i], "false") == 0) {
            enabled = 0;
        }
    }
    if (enabled) {
        AiSysSetStateDebugee(processor);
    } else {
        AiSysSetStateDebugee(NULL);
    }
    return 1;
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

    void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function) {
        FindAlternativeSpecialObjectFn = function;
    }

    void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function) {
        APIOBJECTFromObjIDFn = function;
    }

    void InitFn_GameAILoad(GAMEAILOAD *function) {
        GameAILoadFn = function;
    }

    void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function) {
        GameParamToFloatFn = function;
    }

    void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function) {
        ScriptProcessFirstTimeActionFn = function;
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

extern "C" void AISetPathHeightTol(f32 tolerance) {
    default_path_heighttol = tolerance;
}

}

i32 AIScriptSetInterrupt(AISCRIPTPROCESS *processor, u8 priority, u8 id, char *state_name, f32 time) {
    AISTATE *state;

    if (processor->interrupt_priority > priority) {
        return 0;
    }

    state = AIStateFind(state_name, processor->script);

    if (state == NULL) {
        return 0;
    }

    processor->interrupt_priority = priority;
    processor->interrupt_id = id;
    processor->interrupt_state = state;
    processor->interrupt_timer = time;

    return 1;
}

__used__ static i32 Action_SetInterrupt(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_count, i32 is_first_time, f32) {
    if (is_first_time && processor != NULL && param_count > 0) {
        u8 priority = 0;
        u8 id = 0;
        char *state_name = NULL;
        f32 time = 0.0f;
        char *value;
        for (i32 i = 0; i < param_count; i++) {
            if ((value = NuStrIStr(params[i], "priority")) != NULL) {
                priority = static_cast<i32>(AIParamToFloatEx(packet, processor, value + 9));
            } else if ((value = NuStrIStr(params[i], "id")) != NULL) {
                id = static_cast<i32>(AIParamToFloatEx(packet, processor, value + 3));
            } else if ((value = NuStrIStr(params[i], "state")) != NULL) {
                state_name = value + 6;
            } else if ((value = NuStrIStr(params[i], "time")) != NULL) {
                time = AIParamToFloatEx(packet, processor, value + 5);
            }
        }
        if (state_name != NULL) {
            AIScriptSetInterrupt(processor, priority, id, state_name, time);
        }
    }
    return 1;
}

void AIScriptClearInterrupt(AISCRIPTPROCESS *processor, char *state_name) {
    if (processor->interrupt_state != NULL) {
        if (NuStrCmp(state_name, processor->interrupt_state->name) == 0) {
            processor->interrupt_timer = 0.0f;
            processor->interrupt_priority = 0;
            processor->interrupt_id = 0;
            processor->interrupt_state = NULL;
        }
    }
}

__used__ static i32 Action_ClearInterrupt(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 param_count, i32 is_first_time, f32) {
    if (is_first_time && processor != NULL && param_count > 0) {
        char *state_name = NULL;
        for (i32 i = 0; i < param_count; i++) {
            char *value = NuStrIStr(params[i], "state");
            if (value != NULL) {
                state_name = value + 6;
            }
        }
        if (state_name != NULL) {
            AIScriptClearInterrupt(processor, state_name);
        }
    }
    return 1;
}

extern "C" {
    // libTTapp.so 0x3e5fd0: carve a zeroed, 16-byte-aligned block from the
    // permbuffer cursor. Returns NULL when the cursor or the end pointer is
    // missing or the buffer has less room than requested.
    void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size) {
        void *block = NULL;
        if (cursor != NULL && buf_end != NULL) {
            const u32 offset = (u32)cursor->addr;
            if (buf_end->addr > (usize)(offset + size)) {
                block = (void *)(usize)((offset + 0xf) & ~0xfu);
                cursor->addr = (usize)block + size;
                memset(block, 0, size);
            }
        }
        return block;
    }

}

static __used__ f32 **pathEditorCalculateDistanceTable(AIPATH_s *path, i32 route_mask, variptr_u *cursor,
                                                       variptr_u *end) {
    if (distance_tables == 0 || path == nullptr || path->nodes == nullptr) {
        return nullptr;
    }
    f32 **table = (f32 **)AISysBufferAlloc(cursor, end, path->node_count * sizeof(f32 *));
    if (table == nullptr) {
        return nullptr;
    }
    memset(table, 0, path->node_count * sizeof(f32 *));
    for (i32 row = 0; row < path->node_count; ++row) {
        f32 *distances = (f32 *)AISysBufferAlloc(cursor, end, path->node_count * sizeof(f32));
        table[row] = distances;
        for (i32 column = 0; column < path->node_count; ++column) {
            distances[column] = FLT_MAX;
        }
    }
    for (i32 row = 0; row < path->node_count; ++row) {
        u8 visited[32];
        memset(visited, 0, sizeof(visited));
        pathEditorCalcRouteIterator(path, table[row], visited, row, row, 0.0f, route_mask);
    }
    return table;
}

static void pathEditorCreateSpecialRouteData(AIPATH_s *path, EDAIPATH_s *editor_path, VARIPTR *cursor,
                                             VARIPTR *end) {
    i32 route_indices[16] = {};
    u8 route_members[16][32] = {};
    for (i32 slot = 0; slot < 16; ++slot) {
        if (editor_path->routes[slot].flags & 1) {
            route_indices[slot] = path->route_count;
            path->route_count++;
        }
    }
    if (path->route_count == 0) {
        return;
    }
    path->routes = (AIPATHROUTE_s *)AISysBufferAlloc(cursor, end, path->route_count * sizeof(AIPATHROUTE_s));
    memset(path->routes, 0, path->route_count * sizeof(AIPATHROUTE_s));
    for (i32 slot = 0; slot < 16; ++slot) {
        EDAIPATHROUTE_s *editor_route = &editor_path->routes[slot];
        if (__builtin_expect(!(editor_route->flags & 1), 1)) {
            continue;
        }
        AIPATHROUTE_s *route = &path->routes[route_indices[slot]];
        i32 name_length = strlen(editor_route->name);
        if (__builtin_expect(name_length != 0, 1)) {
            route->name = (char *)AISysBufferAlloc(cursor, end, name_length + 1);
            strcpy(route->name, editor_route->name);
        }
        route->character_masks[0] = editor_route->user_mask;
        if (editor_route->user_mask & (1ULL << 63)) {
            route->character_masks[0] = ~0ULL;
        }
        route->character_masks[1] = editor_route->user_mask;
    }

    for (EDAIPATHNODE_s *editor_node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes);
         editor_node != nullptr;
         editor_node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &editor_node->link)) {
        AIPATHNODE_s *runtime_node = &path->nodes[editor_node->index];
        for (i32 slot = 0; slot < 16; ++slot) {
            if (editor_node->route_mask & (1ULL << slot)) {
                runtime_node->route_boundary_mask |= 1ULL << route_indices[slot];
            }
        }
        for (i32 slot = 0; slot < 8; ++slot) {
            EDAIPATHCNX_s *editor_connection = &editor_node->connections[slot];
            if (editor_connection->node == nullptr || editor_connection->route_mask == 0) {
                continue;
            }
            for (i32 edge = 0; edge < runtime_node->connection_count; ++edge) {
                AIPATHCNX_s *connection = runtime_node->connections[edge];
                if (connection->node_indices[0] != editor_connection->node->index &&
                    connection->node_indices[1] != editor_connection->node->index) {
                    continue;
                }
                for (i32 route_slot = 0; route_slot < 16; ++route_slot) {
                    if (editor_connection->route_mask & (1ULL << route_slot)) {
                        i32 route_index = route_indices[route_slot];
                        u64 bit = 1ULL << route_index;
                        if ((connection->route_mask & bit) == 0) {
                            connection->route_mask |= bit;
                            for (i32 endpoint = 0; endpoint < 2; ++endpoint) {
                                u8 node_index = connection->node_indices[endpoint];
                                route_members[route_index][node_index >> 3] |= 1u << (node_index & 7);
                            }
                        }
                    }
                }
            }
        }
    }

    for (i32 route_index = 0; route_index < path->route_count; ++route_index) {
        AIPATHROUTE_s *route = &path->routes[route_index];
        for (i32 node = 0; node < path->node_count; ++node) {
            if (__builtin_expect(!!(route_members[route_index][node >> 3] & (1u << (node & 7))), 0)) {
                ++route->route_count;
            }
            if (path->nodes[node].route_boundary_mask & (1ULL << route_indices[route_index])) {
                ++route->exit_node_count;
            }
        }
        if (path->node_count != 0) {
            route->node_routes = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
            memset(route->node_routes, 0, path->node_count);
        }
        if (route->route_count != 0) {
            route->node_directions = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
            memset(route->node_directions, 0, route->route_count);
        }
        if (route->exit_node_count != 0) {
            route->exit_nodes = (u8 *)AISysBufferAlloc(cursor, end, route->exit_node_count);
            memset(route->exit_nodes, 0, route->exit_node_count);
        }
        i32 member_index = 0;
        i32 exit_index = 0;
        for (i32 node = 0; node < path->node_count; ++node) {
            route->node_routes[node] = 0xff;
            if (!(route_members[route_index][node >> 3] & (1u << (node & 7)))) {
                continue;
            }
            route->node_routes[node] = member_index;
            path->nodes[node].route_membership_mask |= 1ULL << route_indices[route_index];
            route->node_directions[member_index++] = node;
            if ((path->nodes[node].route_boundary_mask >> route_indices[route_index]) & 1ULL) {
                route->exit_nodes[exit_index++] = node;
            }
        }
        if (route->route_count == 0) {
            continue;
        }
        route->route_nodes = (u8 **)AISysBufferAlloc(cursor, end, route->route_count * sizeof(u8 *));
        memset(route->route_nodes, 0, route->route_count * sizeof(u8 *));
        for (i32 source = 0; source < route->route_count; ++source) {
            route->route_nodes[source] = (u8 *)AISysBufferAlloc(cursor, end, route->route_count);
            memset(route->route_nodes[source], 0, route->route_count);
        }
        // The distance and full-node next-hop tables are temporary. Keep their
        // allocations beyond the persistent route data without advancing it.
        VARIPTR scratch = *cursor;
        f32 **distances = pathEditorCalculateDistanceTable(path, 1ULL << route_index, &scratch, end);
        if (distances == nullptr) {
            continue;
        }
        u8 **next_hops = (u8 **)AISysBufferAlloc(&scratch, end, path->node_count * sizeof(u8 *));
        memset(next_hops, 0, path->node_count * sizeof(u8 *));
        for (i32 source = 0; source < path->node_count; ++source) {
            next_hops[source] = (u8 *)AISysBufferAlloc(&scratch, end, path->node_count);
            memset(next_hops[source], 0, path->node_count);
        }
        for (i32 source = 0; source < path->node_count; ++source) {
            for (i32 destination = 0; destination < path->node_count; ++destination) {
                next_hops[source][destination] = 0xff;
                if (source == destination) {
                    continue;
                }
                f32 best = FLT_MAX;
                AIPATHNODE_s *node = &path->nodes[source];
                for (i32 edge = 0; edge < node->connection_count; ++edge) {
                    AIPATHCNX_s *connection = node->connections[edge];
                    i32 direction = connection->node_indices[0] != source;
                    if (__builtin_expect(!!(connection->traversal_flags[direction] & 0x40000000), 0)) {
                        continue;
                    }
                    if (!(connection->route_mask & (1ULL << route_index))) {
                        continue;
                    }
                    i32 neighbor = connection->node_indices[!direction];
                    f32 distance = distances[source][neighbor] + distances[neighbor][destination];
                    if (distance < best) {
                        best = distance;
                        next_hops[source][destination] = edge;
                    }
                }
            }
        }
        for (i32 source = 0; source < route->route_count; ++source) {
            for (i32 destination = 0; destination < route->route_count; ++destination) {
                route->route_nodes[source][destination] =
                    next_hops[route->node_directions[source]][route->node_directions[destination]];
            }
        }
    }
}

extern "C" {
    i32 AISysSetLevelPath(AISYS *system, char *path_name) {
        if (system == NULL || system->path_sys == NULL) {
            return 0;
        }

        AIPATH *path = NULL;
        if (path_name == NULL) {
            path = system->path_sys->paths[0];
        } else {
            for (i32 i = 0; i < system->path_sys->path_count; ++i) {
                if (NuStrICmp(system->path_sys->paths[i]->name, path_name) == 0) {
                    path = system->path_sys->paths[i];
                    break;
                }
            }
        }

        if (path == NULL || system->path_sys->active_path == path) {
            return 0;
        }
        system->path_sys->active_path = path;
        return 1;
    }

}

void AIPathCalcExtents(AIPATH *path) {
    const f32 max_float = 3.402823466e+38f;
    path->bounds_min.x = max_float;
    path->bounds_min.y = max_float;
    path->bounds_min.z = max_float;
    path->bounds_max.x = -max_float;
    path->bounds_max.y = -max_float;
    path->bounds_max.z = -max_float;

    for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
        AIPATHNODE &node = path->nodes[node_index];
        const f32 x = node.position.x;
        const f32 radius = node.radius;
        const f32 min_x = x - radius;
        const f32 min_z = node.position.z - radius;
        const f32 min_height = node.min_height;

        if (min_x < path->bounds_min.x) {
            path->bounds_min.x = min_x;
        }
        if (min_height < path->bounds_min.y) {
            path->bounds_min.y = min_height;
        }
        if (min_z < path->bounds_min.z) {
            path->bounds_min.z = min_z;
        }
        const f32 max_x = x + radius;
        const f32 max_z = node.position.z + radius;
        if (max_x > path->bounds_max.x) {
            path->bounds_max.x = max_x;
        }
        if (node.max_height > path->bounds_max.y) {
            path->bounds_max.y = node.max_height;
        }
        if (max_z > path->bounds_max.z) {
            path->bounds_max.z = max_z;
        }
    }

    NUVEC margin;
    NuVecSub(&margin, &path->bounds_max, &path->bounds_min);
    NuVecScale(&margin, &margin, 0.1f);
    NuVecSub(&path->bounds_min, &path->bounds_min, &margin);
    NuVecAdd(&path->bounds_max, &path->bounds_max, &margin);
}

i32 AIPathCheckExtents(AIPATH *path, NUVEC *position) {
    if (path->bounds_min.x > position->x || path->bounds_min.y > position->y || path->bounds_min.z > position->z ||
        position->x > path->bounds_max.x || position->y > path->bounds_max.y) {
        return 0;
    }
    return !(position->z > path->bounds_max.z);
}

extern "C" {
    AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed) {
        if (count_across == 0 || system == NULL) {
            return NULL;
        }

        i32 index;
        AIGROUP *group;
        for (index = 0; index < 16; ++index) {
            group = &system->groups[index];
            if (group->is_used == 0)
                break;
        }
        if (index == 16)
            return NULL;
        system->groups[index].is_used = 1;
        group->count_across = count_across;
        group->x_spacing = x_spacing;
        group->z_spacing = z_spacing;
        group->is_in_formation = 0;
        group->max_speed = max_speed;
        return group;
    }

    void AddToAIGroup(AIGROUP *group, APIOBJECT *object) {
        if (object == NULL || group == NULL || group->member_count >= 16) {
            return;
        }

        const u8 member_index = group->member_count;
        const u32 count_across = group->count_across;
        i32 row_count = static_cast<i32>(NuFdiv(static_cast<f32>(member_index + 1), static_cast<f32>(count_across)));
        if ((member_index + 1) % group->count_across != 0)
            ++row_count;
        if (row_count > 4) {
            return;
        }

        if (member_index == 0) {
            group->leader = object;
            if (group->x_spacing > 0.0f) {
                group->radius = (static_cast<f32>(count_across) - 1.0f) * group->x_spacing * 0.5f;
                group->rotation_speed = static_cast<i32>(10430.378f * NuFdiv(group->max_speed, group->radius * 60.0f));
            } else {
                group->radius = 0.0f;
                group->rotation_speed = 100;
            }
        }

        group->row_count = static_cast<u8>(row_count);
        object->ai->group = group;
        object->ai->group_member_index = group->member_count;
        object->ai->group_row = static_cast<i32>(NuFdiv(static_cast<f32>(static_cast<u32>(group->member_count)),
                                                        static_cast<f32>(static_cast<u32>(group->count_across))));
        object->ai->group_column = group->member_count % group->count_across;
        group->members[group->member_count] = object;
        ++group->member_count;
    }

    void DestroyAIGroup(AIGROUP *group) {
        if (group != NULL)
            memset(group, 0, sizeof(*group));
    }

    void AIScriptSetLevelPath(char *path) {
        AiLevelPathName = path;
    }

    void *AISysLoadEx(void *buffer, void *buffer_end, i32 storage_size, void *scene, char *directory, char *name,
                      char *parameter, char *load_directory) {
        VARIPTR *cursor = static_cast<VARIPTR *>(buffer);
        VARIPTR *end = static_cast<VARIPTR *>(buffer_end);
        NUGSCN *gscene = static_cast<NUGSCN *>(scene);
        void *pak = NULL;
        char pak_path[128];
        char ai2_path[128];
        char script_path[128];
        char packed_name[256];
        char character_name[32];
        char area_name[16];

        if (ai_usepackfile != 0) {
            sprintf(pak_path, "%sLevels\\%s\\%s\\ai.pak", AiLevelPathName, directory, parameter);
            i32 pak_size = static_cast<i32>(NuFileSize(pak_path));
            if (pak_size > 0) {
                VARIPTR pak_start;
                pak_start.addr = end->addr - ALIGN(pak_size + 1, 0x10);
                pak = NuFilePakLoad(pak_path, &pak_start, *end, 0x10);
            }
        }

        sprintf(ai2_path, "%sLevels\\%s\\%s\\%s.ai2", AiLevelPathName, load_directory, parameter, name);

        AISYS *system = static_cast<AISYS *>(AISysBufferAlloc(cursor, end, sizeof(AISYS)));
        if (system == NULL) {
            return NULL;
        }
        memset(system, 0, sizeof(AISYS));

        system->storage_size = storage_size;
        system->storage = AISysBufferAlloc(cursor, end, storage_size);
        system->storage_end = *cursor;
        memset(system->storage, 0, storage_size);
        system->storage_cursor.addr = reinterpret_cast<usize>(system->storage);

        EdFileSetMedia(1);
        i32 is_open = 0;
        if (pak != NULL) {
            sprintf(packed_name, "%s.ai2", name);
            EdFileSetPakFile(pak);
            is_open = EdFileOpen(packed_name, NUFILE_READ);
        }
        if (is_open == 0) {
            EdFileSetPakFile(NULL);
            is_open = EdFileOpen(ai2_path, NUFILE_READ);
        }

        if (is_open != 0) {
            i32 version = EdFileReadInt();
            system->scene = gscene;

            system->path_sys = AISysLoadPaths(system, version, gscene, packed_name);
            if (version > 3)
                AISysLoadAreas(system, version);
            if (system->path_sys != NULL) {
                if (version > 5) {
                    AISysLoadLocators(system, version);
                    AISysLoadLocatorSets(system, version);
                }
                AISysLoadCreatures(system, version, character_name, area_name);
                if (version > 12)
                    AISysLoadAntinodes(system, version, gscene, packed_name);
                if (version > 6 && GameAILoadFn != NULL) {
                    GameAILoadFn(system, version, gscene, cursor, end);
                }
            }
            EdFileClose();
        }

        sprintf(script_path, "%sLevels\\%s\\%s", AiLevelPathName, directory, parameter);
        AIScriptLoadAllPakFile(pak, script_path, cursor, end, system);
        AISysSetLevelPath(system, NULL);
        return system;
    }

extern "C" void *AISysLoad(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param) {
    return AISysLoadEx(buf, buf_end, size, gscn, dir, name, param, dir);
}

    void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2) {
        NUVEC forward = {0.0f, 0.0f, 1.0f};
        if (system == NULL) {
            return;
        }

        system->player_1 = player_1;
        system->player_2 = player_2;

        AIANTINODE *dynamic = dynamic_antinodes;
        for (i32 index = 0; index < system->antinode_count + 64; ++index) {
            const i32 antinode_count = system->antinode_count;
            AIANTINODE *antinode =
                index < antinode_count ? &system->antinodes[index] : &dynamic[index - antinode_count];
            if (antinode->has_special == 0) {
                continue;
            }

            nuhspecial_s *special = &antinode->special_handle;
            if (FindAlternativeSpecialObjectFn != NULL && NuSpecialGetVisibilityFn(special) == 0) {
                FindAlternativeSpecialObjectFn(system, special);
            }
            if (NuSpecialGetVisibilityFn(special) == 0) {
                continue;
            }

            NUMTX *draw_matrix = NuSpecialGetDrawMtx(special);
            NuVecMtxTransform(&antinode->position, &antinode->special_position, draw_matrix);
            antinode->min_y = antinode->min_y_offset + antinode->position.y;
            antinode->max_y = antinode->position.y + antinode->max_y_offset;

            if (antinode->type != 0) {
                NUVEC rotated;
                NuVecMtxRotate(&rotated, &forward, draw_matrix);
                antinode->flags = NuAtan2D(rotated.x, rotated.z);
                antinode->flags = NuAngAdd(antinode->flags, antinode->rotation_offset);
            }
        }

        {
            AIGROUP *group = &system->groups[0];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[1];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[2];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[3];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[4];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[5];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[6];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[7];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[8];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[9];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[10];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[11];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[12];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[13];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[14];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }
        {
            AIGROUP *group = &system->groups[15];
            if (group->is_used) {
                if (group->member_is_alive == 0)
                    group->can_respawn = 1;
                else
                    group->can_respawn = 0;
            }
        }

        AILOCATOR *locator = system->locators;
        for (i32 index = 0; index < system->locator_count; ++index, ++locator) {
            AIPATHNODE *start = &locator->path_info.path->nodes[locator->path_info.connection->node_indices[0]];
            AIPATHNODE *end = &locator->path_info.path->nodes[locator->path_info.connection->node_indices[1]];
            if ((start->runtime_flags & AIPATHNODE_RUNTIME_POSITION_CHANGED) == 0 &&
                (end->runtime_flags & AIPATHNODE_RUNTIME_POSITION_CHANGED) == 0) {
                continue;
            }

            NUVEC delta;
            NUVEC direction;
            NuVecSub(&delta, &end->position, &start->position);
            NuVecNorm(&direction, &delta);

            const f32 position_on_connection = locator->path_info.dist;
            f32 radius;
            if (position_on_connection <= 1.0f) {
                if (position_on_connection < 0.0f) {
                    radius = start->radius;
                } else {
                    radius = end->radius * position_on_connection + (1.0f - position_on_connection) * start->radius;
                }
            } else {
                radius = end->radius;
            }

            const f32 direction_x = direction.x;
            direction.x = direction.z * radius;
            direction.z = -direction_x * radius;
            locator->position = start->position;

            NUVEC offset;
            NuVecScale(&offset, &delta, position_on_connection);
            NuVecAdd(&locator->position, &locator->position, &offset);
            NuVecScale(&offset, &direction, locator->path_info.width);
            NuVecAdd(&locator->position, &locator->position, &offset);

            const i32 path_angle = static_cast<i32>(NuAtan2(delta.x, delta.z) * 10430.378f);
            locator->flags = NuAngAdd(path_angle, locator->locator_flags);
        }
    }

extern "C" AIANTINODE *AIAntinodeCreate(NUVEC *position, f32 radius) {
    for (i32 index = 0; index < 64; ++index) {
        AIANTINODE *antinode = &dynamic_antinodes[index];
        if (antinode->enabled == 0) {
            memset(antinode, 0, sizeof(*antinode));
            antinode->enabled = 1;
            antinode->game_flags |= 1;
            antinode->position = *position;
            antinode->radius = radius;
            antinode->min_y = antinode->position.y - default_path_heighttol;
            antinode->max_y = antinode->position.y + default_path_heighttol;
            return antinode;
        }
    }
    return NULL;
}

extern "C" AIANTINODE *AIAntinodeCreateSingleFrame(NUVEC *position, f32 radius) {
    for (i32 index = 0; index < 64; ++index) {
        AIANTINODE *antinode = &dynamic_antinodes[index];
        if (antinode->enabled == 0) {
            memset(antinode, 0, sizeof(*antinode));
            antinode->enabled = 1;
            antinode->game_flags |= 5;
            antinode->position = *position;
            antinode->radius = radius;
            antinode->min_y = antinode->position.y - default_path_heighttol;
            antinode->max_y = antinode->position.y + default_path_heighttol;
            return antinode;
        }
    }
    return NULL;
}

extern "C" void AIAntinodeDestroy(AIANTINODE *antinode) {
    if ((antinode->game_flags & 1) != 0) {
        memset(antinode, 0, sizeof(*antinode));
    }
}

extern "C" void AIAntinodeCullSingleFrame(void) {
    for (i32 index = 0; index < 64; ++index) {
        AIANTINODE *antinode = &dynamic_antinodes[index];
        if (antinode->enabled != 0 && (antinode->game_flags & 4) != 0) {
            memset(antinode, 0, sizeof(*antinode));
        }
    }
}

extern "C" void AIAntinodeMove(AIANTINODE *antinode, NUVEC *position, f32 radius, f32 below, f32 above) {
    if (antinode != NULL) {
        antinode->position = *position;
        antinode->radius = radius;
        antinode->min_y = antinode->position.y - below;
        antinode->max_y = antinode->position.y + above;
    }
}

}

void AISysDrawAntinode_Rectangle(AIANTINODE_s *node, u32 colour) {
    NUMTX matrix;
    NuMtxSetTranslation(&matrix, &node->position);
    NuMtxPreRotateY(&matrix, node->flags);

    NUVEC corners[8] = {
        {-node->base_radius, node->min_y, -node->base_height}, {node->base_radius, node->min_y, -node->base_height},
        {node->base_radius, node->min_y, node->base_height},   {-node->base_radius, node->min_y, node->base_height},
        {-node->base_radius, node->max_y, -node->base_height}, {node->base_radius, node->max_y, -node->base_height},
        {node->base_radius, node->max_y, node->base_height},   {-node->base_radius, node->max_y, node->base_height},
    };
    NuVecMtxTransform(&corners[0], &corners[0], &matrix);
    NuVecMtxTransform(&corners[1], &corners[1], &matrix);
    NuVecMtxTransform(&corners[2], &corners[2], &matrix);
    NuVecMtxTransform(&corners[3], &corners[3], &matrix);
    NuVecMtxTransform(&corners[4], &corners[4], &matrix);
    NuVecMtxTransform(&corners[5], &corners[5], &matrix);
    NuVecMtxTransform(&corners[6], &corners[6], &matrix);
    NuVecMtxTransform(&corners[7], &corners[7], &matrix);

#define ANTINODE_DEBUG_EDGE(a, b)                                                                                      \
    AiRndrLine3dDbg(corners[a].x, corners[a].y, corners[a].z, corners[b].x, corners[b].y, corners[b].z, colour)
    ANTINODE_DEBUG_EDGE(0, 1);
    ANTINODE_DEBUG_EDGE(1, 2);
    ANTINODE_DEBUG_EDGE(2, 3);
    ANTINODE_DEBUG_EDGE(3, 0);
    ANTINODE_DEBUG_EDGE(4, 5);
    ANTINODE_DEBUG_EDGE(5, 6);
    ANTINODE_DEBUG_EDGE(6, 7);
    ANTINODE_DEBUG_EDGE(7, 4);
    ANTINODE_DEBUG_EDGE(0, 4);
    ANTINODE_DEBUG_EDGE(1, 5);
    ANTINODE_DEBUG_EDGE(2, 6);
    ANTINODE_DEBUG_EDGE(3, 7);
#undef ANTINODE_DEBUG_EDGE
}

extern "C" {
    void AISysCreatureAntinodeInteraction(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *) {
        if (system != NULL) {
            AIANTINODE *active[192];
            i32 count = 0;
            AIANTINODE *antinode = system->antinodes;
            for (i32 index = 0; index < system->antinode_count && count < 192; ++index, ++antinode) {
                if (antinode->enabled &&
                    (!antinode->has_special || NuSpecialGetVisibilityFn(&antinode->special_handle))) {
                    active[count++] = antinode;
                }
            }
            antinode = dynamic_antinodes;
            for (i32 index = 0; count < 192 && index < 64; ++index, ++antinode) {
                if (antinode->enabled &&
                    (!antinode->has_special || NuSpecialGetVisibilityFn(&antinode->special_handle))) {
                    active[count++] = antinode;
                }
            }
            if (count != 0) {
                for (i32 index = 0; index < object_count; ++index) {
                    APIOBJECT *object = objects[index];
                    if ((object->object_flags & 0x20080) != 0) {
                        continue;
                    }
                    for (i32 obstacle = 0; obstacle < count; ++obstacle) {
                        antinode = active[obstacle];
                        if (NuSpecialExistsFn(&object->collision_special) &&
                            NuSpecialCompare(&object->collision_special, &antinode->special_handle)) {
                            continue;
                        }
                        if ((antinode->excluded_character_types >> object->field_0x289) & 1) {
                            continue;
                        }
                        NUVEC difference;
                        f32 radius = object->ai->mover_height + antinode->radius;
                        difference.x = object->ai->movement_position.x - antinode->position.x;
                        if (difference.x > radius || difference.x < -radius) {
                            continue;
                        }
                        difference.z = object->ai->movement_position.z - antinode->position.z;
                        if (difference.z > radius || difference.z < -radius ||
                            object->collision_min.y > antinode->max_y) {
                            continue;
                        }
                        if (!(antinode->min_y > object->collision_max.y)) {
                            checkantinodefns[antinode->type](object, antinode, &difference, radius);
                        }
                    }
                }
            }
        }
        AIAntinodeCullSingleFrame();
    }

    void AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time) {
        for (i32 index = 0; index < object_count; ++index) {
            f32 timer = objects[index]->ai->antinode_timer - delta_time;
            objects[index]->ai->antinode_timer = timer < 0.0f ? 0.0f : timer;
        }
        for (i32 first_index = 0; first_index < object_count - 1; ++first_index) {
            APIOBJECT *first = objects[first_index];
            for (i32 second_index = first_index + 1; second_index < object_count; ++second_index) {
                APIOBJECT *second = objects[second_index];
                AIPACKET *first_packet = first->ai;
                AIPACKET *second_packet = second->ai;
                f32 first_radius = first_packet->mover_height;
                f32 second_radius = second_packet->mover_height;
                i32 first_fixed;
                i32 second_fixed;
                if (first->collision_link == second || second->collision_link == first) {
                    first_fixed = second_fixed = 1;
                } else {
                    first_fixed = immovable[first_index];
                    second_fixed = immovable[second_index];
                }
                bool fixed_first = (first->flags_low & 0x80) != 0 || first_fixed != 0;
                bool fixed_second = (second->flags_low & 0x80) != 0 || second_fixed != 0;
                if (fixed_first && fixed_second) {
                    continue;
                }
                f32 radius = first_radius + second_radius;
                NUVEC difference;
                difference.x = first_packet->movement_position.x - second_packet->movement_position.x;
                if (difference.x > radius || difference.x < -radius) {
                    continue;
                }
                difference.z = first_packet->movement_position.z - second_packet->movement_position.z;
                if (difference.z > radius || difference.z < -radius) {
                    continue;
                }
                if (first->collision_min.y > second->collision_max.y ||
                    second->collision_min.y > first->collision_max.y) {
                    continue;
                }
                f32 distance = NuFsqrt(difference.x * difference.x + difference.z * difference.z);
                if (distance < radius) {
                    difference.y = 0.0f;
                    f32 scale = (radius - distance) / distance;
                    if (fixed_first) {
                        NuVecScale(&difference, &difference, scale);
                        NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                    } else if (fixed_second) {
                        NuVecScale(&difference, &difference, scale);
                        NuVecAdd(&first->ai->movement_position, &first->ai->movement_position, &difference);
                    } else {
                        NuVecScale(&difference, &difference, scale);
                        NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                    }
                    AIPACKET *first_ai = first->ai;
                    AIPACKET *second_ai = second->ai;
                    f32 first_timer = first_ai->antinode_timer;
                    f32 second_timer = second_ai->antinode_timer;
                    if (first_timer > antinode_time && second_timer > antinode_time) {
                        if (first_timer > second_timer) {
                            second_ai->antinode_timer = first_timer;
                            second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                        } else {
                            first_ai->antinode_timer = second_timer;
                            first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                        }
                    } else if (second_timer > antinode_time) {
                        first_ai->antinode_timer = second_timer;
                        first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                    } else if (first_timer > 0.0f) {
                        second_ai->antinode_timer = first_timer;
                        second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                    }
                    if (((first->field_0x1f4 ^ second->field_0x1f4) & 1) != 0) {
                        first->ai->field_0x1e5 |= 0x40;
                        second->ai->field_0x1e5 |= 0x40;
                    }
                }
            }
        }
        AISysCreatureAntinodeInteraction(system, object_count, objects, immovable);
    }

    void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time) {
        for (i32 index = 0; index < object_count; ++index) {
            f32 timer = objects[index]->ai->antinode_timer - delta_time;
            if (timer < 0.0f) {
                timer = 0.0f;
            }
            objects[index]->ai->antinode_timer = timer;
        }
        for (i32 first_index = 0; first_index < object_count - 1; ++first_index) {
            APIOBJECT *first = objects[first_index];
            for (i32 second_index = first_index + 1; second_index < object_count; ++second_index) {
                APIOBJECT *second = objects[second_index];
                AIPACKET *first_packet = first->ai;
                AIPACKET *second_packet = second->ai;
                f32 first_radius = first_packet->mover_height;
                f32 second_radius = second_packet->mover_height;
                f32 radius = first_radius + second_radius;
                NUVEC difference;
                difference.x = first_packet->movement_position.x - second_packet->movement_position.x;
                if (difference.x > radius || difference.x < -radius) {
                    continue;
                }
                difference.z = first_packet->movement_position.z - second_packet->movement_position.z;
                if (difference.z > radius || difference.z < -radius) {
                    continue;
                }
                if (first->collision_min.y > second->collision_max.y ||
                    second->collision_min.y > first->collision_max.y) {
                    continue;
                }
                if (first->collision_link == second || second->collision_link == first) {
                    continue;
                }
                i32 first_fixed = immovable != NULL ? immovable[first_index] : 0;
                i32 second_fixed = immovable != NULL ? immovable[second_index] : 0;
                if (first_packet->dont_avoid_character == reinterpret_cast<GameObject_s *>(second)) {
                    first_fixed = 1;
                }
                if (second_packet->dont_avoid_character == reinterpret_cast<GameObject_s *>(first)) {
                    second_fixed = 1;
                }
                if ((first->flags_low & 0x80) != 0) {
                    first_fixed = 1;
                }
                if ((second->flags_low & 0x80) != 0) {
                    second_fixed = 1;
                }
                bool fixed_first;
                bool fixed_second;
                if (first->collision_priority > second->collision_priority) {
                    fixed_first = true;
                    fixed_second = second_fixed != 0;
                } else if (first->collision_priority < second->collision_priority) {
                    fixed_first = first_fixed != 0;
                    fixed_second = true;
                } else {
                    continue;
                }
                if (fixed_first && fixed_second) {
                    continue;
                }
                f32 distance = NuFsqrt(difference.x * difference.x + difference.z * difference.z);
                if (distance < radius) {
                    difference.y = 0.0f;
                    f32 overlap = radius - distance;
                    if (fixed_first) {
                        if (first->collision_priority > second->resolved_collision_priority) {
                            f32 scale = overlap;
                            if (scale == 0.0f || distance == 0.0f) {
                                scale = 0.0f;
                            } else {
                                scale /= distance;
                            }
                            NuVecScale(&difference, &difference, scale);
                            NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                            second->resolved_collision_priority = first->collision_priority;
                        }
                    } else if (fixed_second) {
                        if (second->collision_priority > first->resolved_collision_priority) {
                            f32 scale = overlap;
                            if (scale == 0.0f || distance == 0.0f) {
                                scale = 0.0f;
                            } else {
                                scale /= distance;
                            }
                            NuVecScale(&difference, &difference, scale);
                            NuVecAdd(&first->ai->movement_position, &first->ai->movement_position, &difference);
                            first->resolved_collision_priority = second->collision_priority;
                        }
                    } else {
                        f32 scale = overlap;
                        if (scale == 0.0f || distance == 0.0f) {
                            scale = 0.0f;
                        } else {
                            scale /= distance;
                        }
                        NuVecScale(&difference, &difference, scale);
                        NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                    }
                    AIPACKET *first_ai = first->ai;
                    AIPACKET *second_ai = second->ai;
                    f32 first_timer = first_ai->antinode_timer;
                    f32 second_timer = second_ai->antinode_timer;
                    if (first_timer > antinode_time && second_timer > antinode_time) {
                        if (first_timer > second_timer) {
                            second_ai->antinode_timer = first_timer;
                            second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                        } else {
                            first_ai->antinode_timer = second_timer;
                            first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                        }
                    } else if (second_timer > antinode_time) {
                        first_ai->antinode_timer = second_timer;
                        first_ai->antinode_clockwise = second_ai->antinode_clockwise;
                    } else if (first_timer > 0.0f) {
                        second_ai->antinode_timer = first_timer;
                        second_ai->antinode_clockwise = first_ai->antinode_clockwise;
                    }
                    if (((first->field_0x1f4 ^ second->field_0x1f4) & 1) != 0) {
                        first->ai->field_0x1e5 |= 0x40;
                        second->ai->field_0x1e5 |= 0x40;
                    }
                }
            }
        }
        AISysCreatureAntinodeInteraction(system, object_count, objects, immovable);
    }

    void AISysCreatureInteraction3D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time) {
        for (i32 index = 0; index < object_count; ++index) {
            f32 timer = objects[index]->ai->antinode_timer - delta_time;
            objects[index]->ai->antinode_timer = timer < 0.0f ? 0.0f : timer;
        }
        for (i32 first_index = 0; first_index < object_count - 1; ++first_index) {
            APIOBJECT *first = objects[first_index];
            for (i32 second_index = first_index + 1; second_index < object_count; ++second_index) {
                APIOBJECT *second = objects[second_index];
                AIPACKET *first_packet = first->ai;
                AIPACKET *second_packet = second->ai;
                f32 first_radius = first_packet->mover_height;
                f32 second_radius = second_packet->mover_height;
                i32 first_fixed;
                i32 second_fixed;
                if (first->collision_link == second || second->collision_link == first) {
                    first_fixed = second_fixed = 1;
                } else {
                    first_fixed = immovable[first_index];
                    second_fixed = immovable[second_index];
                }
                bool fixed_first = (first->flags_low & 0x80) != 0 || first_fixed != 0;
                bool fixed_second = (second->flags_low & 0x80) != 0 || second_fixed != 0;
                if (fixed_first && fixed_second) {
                    continue;
                }
                f32 radius = first_radius + second_radius;
                NUVEC difference;
                difference.x = first_packet->movement_position.x - second_packet->movement_position.x;
                if (difference.x > radius || difference.x < -radius) {
                    continue;
                }
                difference.z = first_packet->movement_position.z - second_packet->movement_position.z;
                if (difference.z > radius || difference.z < -radius) {
                    continue;
                }
                difference.y = first_packet->movement_position.y - second_packet->movement_position.y;
                if (difference.y > radius || difference.y < -radius) {
                    continue;
                }
                f32 distance =
                    NuFsqrt(difference.x * difference.x + difference.y * difference.y + difference.z * difference.z);
                if (distance < radius) {
                    f32 scale = (radius - distance) / distance;
                    if (fixed_first) {
                        NuVecScale(&difference, &difference, scale);
                        NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                    } else if (fixed_second) {
                        NuVecScale(&difference, &difference, scale);
                        NuVecAdd(&first->ai->movement_position, &first->ai->movement_position, &difference);
                    } else {
                        NuVecScale(&difference, &difference, scale);
                        NuVecSub(&second->ai->movement_position, &second->ai->movement_position, &difference);
                    }
                    if (((first->field_0x1f4 ^ second->field_0x1f4) & 1) != 0) {
                        first->ai->field_0x1e5 |= 0x40;
                        second->ai->field_0x1e5 |= 0x40;
                    }
                }
            }
        }
        AISysCreatureAntinodeInteraction(system, object_count, objects, immovable);
    }

extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path) {
    if (packet->path_info.path == path) {
        return;
    }

    memset(&packet->path_info, 0, sizeof(packet->path_info));
    packet->path_info.path = path;
    packet->path_info.path_index = 0xff;
    packet->available_routes = 0;
    packet->inside_path_node = -1;
    packet->current_route = 0xff;
    packet->next_route = 0;
    packet->goal_path_node = NULL;

    if (path == NULL || path->route_count == 0) {
        return;
    }

    const u64 character_mask = packet->character_type_mask;
    i32 route_index = 0;
    do {
        AIPATHROUTE &route = path->routes[route_index];
        if ((route.character_masks[0] & character_mask) != 0) {
            packet->available_routes |= static_cast<u16>(static_cast<u64>(1) << route_index);
        }
        ++route_index;
    } while (path->route_count > route_index);
}

}

static i32 AIMoveChooseExitNodePath(AIPATHSYS **path_sys, AIPACKET *packet) {
    AIPATHNODE *first = &packet->path_info.path->nodes[packet->path_info.connection->node_indices[0]];
    AIPATHNODE *second = &packet->path_info.path->nodes[packet->path_info.connection->node_indices[1]];
    f32 distance = NuVecDistSqr(&packet->owner->apiobj.position, &first->position, NULL) - first->radius_squared;
    f32 second_distance =
        NuVecDistSqr(&packet->owner->apiobj.position, &second->position, NULL) - second->radius_squared;
    AIPATHNODE *node = distance < second_distance ? first : second;
    if (node->special_route_index >= packet->path_info.path->special_route_count) {
        return 0;
    }
    AIPATHSPECIALROUTE *route =
        &(*path_sys)
             ->special_routes[packet->path_info.path->special_routes[node->special_route_index].special_route_index];
    for (i32 index = 0; index < route->path_count; ++index) {
        if (route->paths[index] == packet->fallback_path_info.path) {
            AISysCharacterSetPath(packet, packet->fallback_path_info.path);
            return 1;
        }
    }
    f32 nearest = FLT_MAX;
    AIPATH *selected = NULL;
    for (i32 index = 0; index < route->path_count; ++index) {
        AIPATH *path = route->paths[index];
        AIPATHNODELINK *links = path->special_routes;
        i32 nearest_node = -1;
        f32 nearest_node_distance = FLT_MAX;
        for (i32 link = 0; link < path->special_route_count; ++link) {
            f32 candidate =
                NuVecDistSqr(&path->nodes[links[link].node_index].position, &packet->fallback_destination, NULL);
            if (candidate < nearest_node_distance) {
                nearest_node_distance = candidate;
                nearest_node = links[link].node_index;
            }
        }
        if (nearest_node != -1) {
            distance = nearest_node_distance;
        }
        if (distance < nearest) {
            nearest = distance;
            selected = route->paths[index];
        }
    }
    if (selected != NULL) {
        AISysCharacterSetPath(packet, selected);
        return 1;
    }
    return 0;
}

extern "C" {
extern "C" f32 AIPathNodeDistanceToPathNode(AIPATH *path, i32 start_node, i32 destination_node, i32 route_index,
                                            u32 excluded_route_mask) {
    if (path->route_matrix == NULL) {
        return FLT_MAX;
    }
    if (start_node == destination_node) {
        return 0.0f;
    }
    AIPATHNODE *nodes = path->nodes;
    AIPATHNODE *node = &nodes[start_node];
    f32 *cached_distance = NULL;
    AIPATHROUTE *route = NULL;
    if (excluded_route_mask == 0 && route_index == 0xff) {
        if (node->distance_cache_nodes[0] == destination_node) {
            return node->distance_cache[0];
        }
        if (node->distance_cache_nodes[1] == destination_node) {
            return node->distance_cache[1];
        }
        node->distance_cache_nodes[1] = node->distance_cache_nodes[0];
        node->distance_cache_nodes[0] = static_cast<u8>(destination_node);
        nodes = path->nodes;
        node->distance_cache[1] = node->distance_cache[0];
        cached_distance = &node->distance_cache[0];
        *cached_distance = 0.0f;
    }
    i32 node_index = node - nodes;
    if (route_index != 0xff) {
        route = &path->routes[route_index];
        if (((static_cast<u64>(node->route_membership_mask) >> route_index) & 1) == 0) {
            return FLT_MAX;
        }
    }

    AIPATHNODE *destination = &nodes[destination_node];
    AIPATHCNX *previous_connection = NULL;
    f32 distance = 0.0f;
    while (node != destination) {
        i32 connection_index = 0xff;
        if (route != NULL) {
            if ((static_cast<u64>(destination->route_membership_mask) & (static_cast<u64>(1) << route_index)) != 0) {
                u8 from = route->node_routes[node_index];
                if (from < route->route_count && route->node_routes[destination_node] < route->route_count) {
                    connection_index = route->route_nodes[from][route->node_routes[destination_node]];
                }
            } else {
                f32 nearest = FLT_MAX;
                for (i32 index = 0; index < route->exit_node_count; ++index) {
                    f32 first =
                        route->exit_nodes[index] == start_node
                            ? 0.0f
                            : AIPathNodeDistanceToPathNode(path, start_node, route->exit_nodes[index], route_index, 0);
                    f32 second =
                        route->exit_nodes[index] == destination_node
                            ? 0.0f
                            : AIPathNodeDistanceToPathNode(path, route->exit_nodes[index], destination_node, 0xff,
                                                           static_cast<u32>(static_cast<u64>(1) << route_index));
                    f32 candidate = first != FLT_MAX && second != FLT_MAX ? second + first : FLT_MAX;
                    if (candidate < nearest) {
                        nearest = candidate;
                    }
                }
                if (nearest == FLT_MAX) {
                    if (cached_distance != NULL) {
                        *cached_distance = FLT_MAX;
                    }
                    return FLT_MAX;
                }
                distance += nearest;
                break;
            }
        } else {
            connection_index = path->route_matrix[node_index][destination_node];
        }
        if (connection_index == 0xff) {
            if (cached_distance != NULL) {
                *cached_distance = FLT_MAX;
            }
            return FLT_MAX;
        }
        AIPATHCNX *connection = node->connections[connection_index];
        if (connection == NULL) {
            return FLT_MAX;
        }
        if ((connection->route_mask & excluded_route_mask) != 0) {
            if (cached_distance != NULL) {
                *cached_distance = FLT_MAX;
            }
            return FLT_MAX;
        }
        if (connection == previous_connection) {
            break;
        }
        node_index =
            connection->node_indices[0] == node_index ? connection->node_indices[1] : connection->node_indices[0];
        node = &nodes[node_index];
        distance += connection->distance;
        previous_connection = connection;
    }
    if (cached_distance != NULL) {
        *cached_distance = distance;
    }
    return distance;
}

}

static AIPATHCNX *GetNextConnection(const AIPACKET *packet, i32 *direction) {
    if (packet->goal_path_node == NULL || packet->path_info.connection == NULL) {
        return NULL;
    }
    AIPATH *path = packet->path_info.path;
    i32 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
    AIPATHNODE *node = &path->nodes[node_index];
    u32 destination = packet->goal_path_node - path->nodes;
    if (destination > 0xff) {
        return NULL;
    }
    AIPATHCNX *connection = NULL;
    i32 connection_index = path->route_matrix[node_index][destination];
    if (connection_index < node->connection_count) {
        connection = node->connections[connection_index];
        *direction = connection->node_indices[0] != node_index;
    }
    u8 route_index = packet->current_route;
    if (route_index == 0xff) {
        return connection;
    }
    if ((static_cast<u64>(static_cast<u16>(node->route_membership_mask)) >> route_index & 1) == 0) {
        return NULL;
    }
    if ((static_cast<u64>(static_cast<u16>(node->route_boundary_mask)) >> route_index & 1) != 0) {
        if (connection == NULL) {
            return NULL;
        }
        if ((static_cast<u64>(connection->route_mask) >> route_index & 1) == 0) {
            return connection;
        }
    }
    AIPATHROUTE *route = &path->routes[route_index];
    i32 next_node = destination;
    if ((static_cast<u64>(static_cast<u16>(path->nodes[destination].route_membership_mask)) >> route_index & 1) == 0) {
        if (route->exit_node_count == 0) {
            return NULL;
        }
        f32 nearest = FLT_MAX;
        next_node = 0;
        for (i32 index = 0; index < route->exit_node_count; ++index) {
            f32 first = AIPathNodeDistanceToPathNode(packet->path_info.path, node_index, route->exit_nodes[index],
                                                     packet->current_route, 0);
            f32 second =
                AIPathNodeDistanceToPathNode(packet->path_info.path, route->exit_nodes[index], destination, 0xff,
                                             static_cast<u32>(static_cast<u64>(1) << packet->current_route));
            f32 distance = first != FLT_MAX && second != FLT_MAX ? first + second : FLT_MAX;
            if (distance < nearest) {
                nearest = distance;
                next_node = route->exit_nodes[index];
            }
        }
        if (nearest == FLT_MAX) {
            return NULL;
        }
    }
    connection = NULL;
    connection_index = route->route_nodes[route->node_routes[node_index]][route->node_routes[next_node]];
    if (connection_index < node->connection_count) {
        connection = node->connections[connection_index];
        *direction = connection->node_indices[0] != node_index;
    }
    return connection;
}

extern "C" {
    AILOCATOR *AIPathFindLocator(AISYS *sys, char *name) {
        if (sys != NULL) {
            for (i32 i = 0; i < sys->locator_count; ++i) {
                if (NuStrICmp(sys->locators[i].name, name) == 0) {
                    return &sys->locators[i];
                }
            }
        }
        return NULL;
    }

}

static i32 Action_SetRespawnLocator(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                    i32 param_count, i32 first_time, f32) {
    AILOCATOR *locators[32];
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && first_time != 0) {
        packet->respawn_locator = processor->unknown_a4;
        i32 count = 0;
        for (i32 i = 0; i < param_count; i++) {
            char *value = NuStrIStr(params[i], "locator");
            if (value != NULL) {
                if (count < 32) {
                    locators[count] = AIPathFindLocator(sys, value + 8);
                    if (locators[count] != NULL) {
                        count++;
                    }
                }
            }
        }
        if (count != 0) {
            packet->respawn_locator = locators[NuRand(NULL) % count];
        }
    }
    return 1;
}

__used__ static i32 Action_FaceLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
        return 1;
    }
    if (first_time != 0) {
        processor->action_data_3 = processor->locator;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(params[index], "name");
            if (value != NULL) {
                processor->action_data_3 = AIPathFindLocator(sys, value + NuStrLen("name") + 1);
                ++index;
            }
        }
    }
    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
    if (locator != NULL) {
        packet->movement_look_target = &locator->position;
        return 1;
    }
    return 0;
}

static void *Condition_PlayerToLocatorInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

static void *Condition_OpponentToLocatorInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

static void *Condition_CurrentLocatorIsInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
}

extern "C" {
    AILOCATORSET *AIPathFindLocatorSet(AISYS *system, char *name) {
        if (system != NULL) {
            for (i32 index = 0; index < system->locator_set_count; ++index) {
                if (NuStrICmp(system->locator_sets[index].name, name) == 0) {
                    return &system->locator_sets[index];
                }
            }
        }
        return NULL;
    }

    AIAREA *AISysFindArea(AISYS *sys, char *name) {
        if (sys != NULL) {
            for (i32 i = 0; i < sys->area_count; ++i) {
                AIAREA *area = &sys->areas[i];
                if (NuStrICmp(name, area->name) == 0) {
                    return area;
                }
            }
        }
        return NULL;
    }

}

static void *Condition_OpponentInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static void *Condition_GoodyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static void *Condition_BaddyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static void *Condition_PlayerInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static void *Condition_InTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

extern "C" {
    AIPATH *AISysFindPath(AISYS *sys, char *name) {
        if (sys != NULL && sys->path_sys != NULL) {
            for (i32 i = 0; i < sys->path_sys->path_count; ++i) {
                if (NuStrICmp(sys->path_sys->paths[i]->name, name) == 0) {
                    return sys->path_sys->paths[i];
                }
            }
        }
        return NULL;
    }

    AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name) {
        if (path == NULL) {
            if (system == NULL || system->path_sys == NULL || system->path_sys->path_count == 0) {
                return NULL;
            }
            path = system->path_sys->active_path;
            if (path == NULL) {
                return NULL;
            }
        }

        for (i32 index = 0; index < path->node_count; ++index) {
            if (path->nodes[index].name != NULL && NuStrICmp(path->nodes[index].name, name) == 0) {
                return &path->nodes[index];
            }
        }
        return NULL;
    }

}

static void *Condition_LevelNodeRangeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
}

static void *Condition_PlayerInLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
}

static void *Condition_InLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
}

void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked) {
    if (from_name == NULL || path == NULL || to_name == NULL) {
        return;
    }
    AIPATHNODE *from = AIPathFindNode(NULL, path, from_name);
    AIPATHNODE *to = AIPathFindNode(NULL, path, to_name);
    if (to == NULL || from == NULL) {
        return;
    }
    const i32 to_index = to - path->nodes;
    for (i32 index = 0; index < from->connection_count; ++index) {
        AIPATHCNX *connection = from->connections[index];
        i32 direction;
        if (connection->node_indices[0] == to_index) {
            direction = 1;
        } else if (connection->node_indices[1] == to_index) {
            direction = 0;
        } else {
            continue;
        }
        if (blocked != 0) {
            connection->traversal_flags[direction] |= 0x80000000u;
        } else {
            connection->traversal_flags[direction] &= 0x7fffffffu;
        }
        return;
    }
}

__used__ static i32 Action_BlockPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)packet;
    (void)param_6;
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0 && param_5 != 0 && param_4 > 0) {
        char *from = NULL;
        char *to = NULL;
        i32 both_ways = 0;
        i32 blocked = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "from");
            if (value != NULL) {
                from = value + 5;
                continue;
            }
            value = NuStrIStr(params[index], "to");
            if (value != NULL) {
                to = value + 3;
            } else if (NuStrICmp(params[index], "bothways") == 0) {
                both_ways = 1;
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                blocked = 0;
            }
        }
        if (to != NULL && from != NULL) {
            AIPathCnxSetTemporaryBlock(sys->path_sys->active_path, from, to, blocked);
            if (both_ways) {
                AIPathCnxSetTemporaryBlock(sys->path_sys->active_path, to, from, blocked);
            }
        }
    }
    return 1;
}

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction) {
        if (path == NULL) {
            if (system == NULL || system->path_sys == NULL || system->path_sys->path_count == 0) {
                return NULL;
            }
            path = system->path_sys->active_path;
        }
        if (path == NULL) {
            return NULL;
        }

        AIPATHNODE *from = AIPathFindNode(system, path, from_name);
        AIPATHNODE *to = AIPathFindNode(system, path, to_name);
        if (to == NULL || from == NULL || from == to) {
            return NULL;
        }

        const i32 to_index = to - path->nodes;
        for (i32 index = 0; index < from->connection_count; ++index) {
            AIPATHCNX *connection = from->connections[index];
            if (connection->node_indices[0] == to_index) {
                *direction = 1;
                return connection;
            }
            if (connection->node_indices[1] == to_index) {
                *direction = 0;
                return connection;
            }
        }
        return NULL;
    }

}

static i32 Action_PathConnectionMaxLength(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                          i32 param_count, i32 first_time, f32) {
    i32 direction = 0;
    if (sys != NULL) {
        if (first_time != 0) {
            char *from = NULL;
            char *to = NULL;
            f32 length = 0.0f;
            for (i32 i = 0; i < param_count; i++) {
                char *value = NuStrIStr(params[i], "from");
                if (value != NULL) {
                    from = value + 5;
                } else if ((value = NuStrIStr(params[i], "to")) != NULL) {
                    to = value + 3;
                } else if ((value = NuStrIStr(params[i], "length")) != NULL) {
                    length = AIParamToFloatEx(packet, processor, value + 7);
                }
            }
            if (to != NULL && from != NULL) {
                AIPATHCNX *connection =
                    static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, sys->path_sys->active_path, from, to, &direction));
                if (connection != NULL) {
                    connection->max_horizontal_distance = length;
                    if (length != 0.0f &&
                        connection->horizontal_distance > length + sys->path_sys->active_path->nodes[0].radius +
                                                              sys->path_sys->active_path->nodes[0].radius) {
                        connection->traversal_flags[0] |= 0x08000000;
                        connection->traversal_flags[1] |= 0x08000000;
                    } else {
                        connection->traversal_flags[0] &= ~0x08000000;
                        connection->traversal_flags[1] &= ~0x08000000;
                    }
                }
            }
        }
    }
    return 1;
}

static i32 Action_PathConnectionObstacle(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                         i32 param_count, i32 first_time, f32) {
    i32 direction = 0;
    if (sys != NULL) {
        if (first_time != 0) {
            char *from = NULL;
            char *to = NULL;
            i32 open = 0;
            for (i32 i = 0; i < param_count; i++) {
                char *value = NuStrIStr(params[i], "from");
                if (value != NULL) {
                    from = value + 5;
                } else if ((value = NuStrIStr(params[i], "to")) != NULL) {
                    to = value + 3;
                } else if (NuStrICmp(params[i], "open") == 0) {
                    open = 1;
                }
            }
            if (to != NULL && from != NULL) {
                AIPATHCNX *connection = static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, NULL, from, to, &direction));
                if (connection != NULL && (connection->traversal_flags[0] & 0x20000000) != 0) {
                    connection->open = open;
                }
            }
        }
    }
    return 1;
}

extern "C" {
extern "C" AIPATHCNX *AIPathFindPathCnxFromIX(AISYS *system, AIPATH *path, u8 from_index, u8 to_index) {
    if (path == NULL) {
        if (system == NULL || system->path_sys == NULL || system->path_sys->path_count == 0) {
            return NULL;
        }
        path = system->path_sys->active_path;
    }
    if (path != NULL && from_index < path->node_count) {
        AIPATHNODE *node = &path->nodes[from_index];
        if (from_index != to_index) {
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX *connection = node->connections[index];
                if (connection->node_indices[0] == to_index || connection->node_indices[1] == to_index) {
                    return connection;
                }
            }
        }
    }
    return NULL;
}

    void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node) {
        if (node->has_special == 0) {
            return;
        }

        const i32 node_index = static_cast<i32>(node - path->nodes);
        u8 &updated_nodes = path->updated_node_bits[node_index / 8];
        if (((updated_nodes >> (node_index % 8)) & 1) != 0) {
            return;
        }
        updated_nodes |= 1 << (node_index % 8);

        nuhspecial_s *special = &node->special_handle;
        if ((node->runtime_flags & AIPATHNODE_RUNTIME_SPECIAL_UNAVAILABLE) != 0) {
            if (NuSpecialGetVisibilityFn(special) == 0) {
                return;
            }

            node->runtime_flags &= static_cast<u8>(~AIPATHNODE_RUNTIME_SPECIAL_UNAVAILABLE);
            AIPATHCNX **connections = node->connections;
            AIPATHNODE *nodes = path->nodes;
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX *connection = connections[index];
                AIPATHNODE *other_node;
                if (connection->direction_a == node_index)
                    other_node = &nodes[connection->direction_b];
                else
                    other_node = &nodes[connection->direction_a];
                if ((other_node->runtime_flags & AIPATHNODE_RUNTIME_SPECIAL_UNAVAILABLE) == 0) {
                    connection->traversal_flags[0] &= ~AIPATH_CONNECTION_FLAG_SPECIAL_UNAVAILABLE;
                    connection->traversal_flags[1] &= ~AIPATH_CONNECTION_FLAG_SPECIAL_UNAVAILABLE;
                }
            }
        } else {
            if (NuSpecialGetVisibilityFn(special) == 0 &&
                (FindAlternativeSpecialObjectFn == NULL || FindAlternativeSpecialObjectFn(system, special) == 0)) {
                node->runtime_flags |= AIPATHNODE_RUNTIME_SPECIAL_UNAVAILABLE;
                for (i32 index = 0; index < node->connection_count; ++index) {
                    AIPATHCNX *connection = node->connections[index];
                    connection->traversal_flags[0] |= AIPATH_CONNECTION_FLAG_SPECIAL_UNAVAILABLE;
                    connection->traversal_flags[1] |= AIPATH_CONNECTION_FLAG_SPECIAL_UNAVAILABLE;
                }
                return;
            }
        }

        const NUVEC previous_position = node->position;
        NuVecMtxTransform(&node->position, &node->special_position, NuSpecialGetDrawMtx(special));
        node->min_height = node->position.y + node->min_height_offset;
        node->max_height = node->position.y + node->max_height_offset;

        for (i32 index = 0; index < node->connection_count; ++index) {
            AIPATHCNX *connection = node->connections[index];
            AIPATHNODE *other_node;
            NUVEC direction;
            if (connection->direction_a == node_index) {
                other_node = &path->nodes[connection->direction_b];
                connection->horizontal_distance = NuVecXZDist(&other_node->position, &node->position, &direction);
            } else {
                other_node = &path->nodes[connection->direction_a];
                connection->horizontal_distance = NuVecXZDist(&node->position, &other_node->position, &direction);
            }
            connection->rotation = static_cast<i16>(NuAtan2(direction.x, direction.z) * 10430.378f);

            if (connection->max_horizontal_distance != 0.0f) {
                if (connection->max_horizontal_distance + node->radius + other_node->radius <
                    connection->horizontal_distance) {
                    connection->traversal_flags[0] |= AIPATH_CONNECTION_FLAG_DYNAMIC_TOO_LONG;
                    connection->traversal_flags[1] |= AIPATH_CONNECTION_FLAG_DYNAMIC_TOO_LONG;
                } else {
                    connection->traversal_flags[0] &= ~AIPATH_CONNECTION_FLAG_DYNAMIC_TOO_LONG;
                    connection->traversal_flags[1] &= ~AIPATH_CONNECTION_FLAG_DYNAMIC_TOO_LONG;
                }
            }
        }

        nuinstanim_s *animation;
        if (previous_position.x != node->position.x || previous_position.y != node->position.y ||
            previous_position.z != node->position.z ||
            ((animation = NuSpecialGetInstAnim(special)) != NULL && (animation->flags & NUINSTANIM_FLAG_PLAYING) != 0 &&
             animation->tfactor != 0.0f)) {
            node->runtime_flags |= AIPATHNODE_RUNTIME_POSITION_CHANGED;
        } else {
            node->runtime_flags &= static_cast<u8>(~AIPATHNODE_RUNTIME_POSITION_CHANGED);
        }
    }

extern "C" i32 WithinConnection(AISYS *system, NUVEC *position, AIPATH *path, AIPATHCNX *connection, i32 checks,
                                AIPATHCNX *previous_connection, i32 route, i32 ground, AIPATHINFO *path_info,
                                f32 radius, i32 update_once) {
    (void)checks;
    (void)ground;
    if (update_once != 0) {
        if (connection->last_search_checksum == path->search_checksum)
            return 0;
        connection->last_search_checksum = path->search_checksum;
    }
    if (route != 0xff && previous_connection != NULL &&
        ((static_cast<u64>(connection->route_mask) >> route) & 1) == 0) {
        AIPATHNODE *shared;
        if (previous_connection->node_indices[0] == connection->node_indices[0])
            shared = &path->nodes[connection->node_indices[0]];
        else if (previous_connection->node_indices[0] == connection->node_indices[1])
            shared = &path->nodes[connection->node_indices[1]];
        else if (previous_connection->node_indices[1] == connection->node_indices[0])
            shared = &path->nodes[connection->node_indices[0]];
        else if (previous_connection->node_indices[1] == connection->node_indices[1])
            shared = &path->nodes[connection->node_indices[1]];
        else
            return 0;
        if (shared == NULL || ((static_cast<u64>(shared->route_boundary_mask) >> route) & 1) == 0)
            return 0;
    }

    AIPATHNODE *first = &path->nodes[connection->node_indices[0]];
    AIPATHNODE *second = &path->nodes[connection->node_indices[1]];
    f32 first_radius = first->radius;
    u8 narrow = 0;
    if (first_radius > radius + 0.05f)
        first_radius -= radius;
    else
        narrow = 1;
    f32 second_radius = second->radius;
    if (second_radius > radius + 0.05f)
        second_radius -= radius;
    else
        narrow = 1;

    if (first->has_special != 0 &&
        (path->updated_node_bits[connection->node_indices[0] >> 3] & (1 << (connection->node_indices[0] & 7))) == 0)
        AIPathNodeUpdatePos(system, path, first);
    if (second->has_special != 0 &&
        (path->updated_node_bits[connection->node_indices[1] >> 3] & (1 << (connection->node_indices[1] & 7))) == 0)
        AIPathNodeUpdatePos(system, path, second);

    NUVEC delta;
    delta.x = position->x - first->position.x;
    delta.z = position->z - first->position.z;
    NUVEC local;
    NuVecRotateY(&local, &delta, -connection->rotation);
    if (NuFabs(second_radius - first_radius) > connection->horizontal_distance) {
        if (second_radius > first_radius) {
            first = second;
            first_radius = second_radius;
        }
        if (first->min_height > position->y || position->y > first->max_height)
            return 0;
        delta.x = position->x - first->position.x;
        delta.z = position->z - first->position.z;
        if (!(first_radius * first_radius >= delta.x * delta.x + delta.z * delta.z))
            return 0;
    } else {
        f32 width = local.x;
        if (width < 0.0f)
            width = -width;
        if (width > first_radius && width > second_radius)
            return 0;
        if (local.z < -first_radius || local.z > connection->horizontal_distance + second_radius)
            return 0;
        NUVEC wall = local;
        const i32 angle = AISysPathIntersectionAngle((second_radius - first_radius) / connection->horizontal_distance);
        if (wall.x < 0.0f)
            wall.x = -wall.x;
        NuVecRotateY(&wall, &wall, -angle);
        NUVEC radial;
        if (wall.z < 0.0f) {
            if (first->min_height > position->y || position->y > first->max_height)
                return 0;
            if (!(first_radius * first_radius >= NuVecXZDistSqr(position, &first->position, &radial)))
                return 0;
        } else if (wall.z > NU_COS_LUT(angle) * connection->horizontal_distance) {
            if (second->min_height > position->y || position->y > second->max_height)
                return 0;
            if (!(second_radius * second_radius >= NuVecXZDistSqr(position, &second->position, &radial)))
                return 0;
        } else {
            f32 minimum, maximum;
            if (first_radius > local.z) {
                minimum = first->min_height;
                maximum = first->max_height;
            } else if (local.z > connection->horizontal_distance - second_radius) {
                minimum = second->min_height;
                maximum = second->max_height;
            } else {
                const f32 fraction =
                    (local.z - first_radius) / (connection->horizontal_distance - (first_radius + second_radius));
                const f32 inverse = 1.0f - fraction;
                minimum = second->min_height * fraction + first->min_height * inverse;
                maximum = second->max_height * fraction + first->max_height * inverse;
            }
            if (minimum > position->y || position->y > maximum || !(first_radius > wall.x))
                return 0;
        }
    }
    if (path_info != NULL) {
        path_info->direction = 0;
        path_info->flags = ((path_info->flags | 1) & ~8) | (narrow << 3);
        path_info->connection = connection;
        path_info->dist = local.z / connection->horizontal_distance;
        path_info->width = local.x;
    }
    return 1;
}

}

static i32 TestPointPathCheck(nuvec_s *point, EDAIPATHNODE_s *first, EDAIPATHNODE_s *second, f32 *fraction, f32 *width,
                              i32 *angle, f32 tolerance) {
    AIPATH path;
    AIPATHCNX connection;
    AIPATHNODE nodes[2];
    AIPATHINFO info;
    memset(&path, 0, sizeof(path));
    memset(&connection, 0, sizeof(connection));
    memset(nodes, 0, sizeof(nodes));
    memset(&info, 0, sizeof(info));
    nodes[0].position = first->position;
    nodes[0].radius = first->radius + tolerance;
    nodes[0].min_height = first->position.y + first->lower_height - tolerance;
    nodes[0].max_height = first->position.y + first->upper_height + tolerance;
    nodes[1].position = second->position;
    nodes[1].radius = second->radius + tolerance;
    nodes[1].min_height = second->position.y + second->lower_height - tolerance;
    nodes[1].max_height = second->position.y + second->upper_height + tolerance;
    path.nodes = nodes;
    path.connections = &connection;
    path.node_count = 2;
    path.connection_count = 1;
    connection.node_indices[0] = 1;
    NUVEC horizontal;
    f32 distance = NuVecXZDist(&first->position, &second->position, &horizontal);
    connection.horizontal_distance = distance != 0.0f ? distance : 0.0001f;
    connection.rotation = (i16)(NuAtan2(horizontal.x, horizontal.z) * 10430.378f);
    if (!WithinConnection(nullptr, point, &path, &connection, 1, nullptr, 0xff, 0xff, &info, 0.0f, 0)) {
        return 0;
    }

    NUVEC direction;
    NuVecSub(&direction, &second->position, &first->position);
    distance = NuVecMag(&direction);
    NuVecScale(&direction, &direction, 1.0f / distance);
    NUVEC relative;
    NuVecSub(&relative, point, &first->position);
    f32 along = NuVecDot(&direction, &relative);
    *fraction = along / distance;
    NUVEC nearest;
    f32 radius;
    if (*fraction <= 0.0f) {
        nearest = first->position;
        radius = first->radius;
    } else if (*fraction >= 1.0f) {
        nearest = second->position;
        radius = second->radius;
    } else {
        NuVecScale(&nearest, &direction, along);
        NuVecAdd(&nearest, &nearest, &first->position);
        radius = second->radius * *fraction + first->radius * (1.0f - *fraction);
    }
    NUVEC radial;
    f32 distance_squared = NuVecXZDistSqr(&nearest, point, &radial);
    radius += tolerance;
    *angle = (i32)(NuAtan2(direction.x, direction.z) * 10430.378f);
    NuVecRotateY(&relative, &relative, -*angle);
    if (*fraction <= 0.0f) {
        f32 beyond = -*fraction * distance;
        *width = distance_squared - beyond * beyond;
        *width = NuFsqrt(*width) / radius;
    } else if (*fraction >= 1.0f) {
        f32 beyond = (*fraction - 1.0f) * distance;
        *width = distance_squared - beyond * beyond;
        *width = NuFsqrt(*width) / radius;
    } else {
        *width = NuFsqrt(distance_squared) / radius;
    }
    if (relative.x < 0.0f) {
        *width = -*width;
    }
    return 1;
}

// Retail keeps this one whole: neither inlined nor reduced by IPA-SRA.
static __attribute__((noinline, noclone)) u32 CalculateIntersection(AISYS *system, AIPACKET *packet, APIOBJECT *object,
                                                                    AIPATHCNX *current_connection,
                                                                    AIPATHCNX *target_connection) {
    if (packet->intersection_connection != current_connection ||
        packet->intersection_target_connection != target_connection) {
        packet->movement_flags &=
            static_cast<u8>(~(AIPACKET_MOVEMENT_DIVERSION_RIGHT | AIPACKET_MOVEMENT_DIVERSION_LEFT));
        packet->intersection_connection = current_connection;
        packet->intersection_target_connection = target_connection;

        AIPATH *path = packet->path_info.path;
        AIPATHNODE *before;
        AIPATHNODE *shared;
        AIPATHNODE *after;
        i32 first_angle;
        i32 second_angle;
        if (current_connection->node_indices[0] != target_connection->node_indices[0]) {
            if (current_connection->node_indices[0] == target_connection->node_indices[1]) {
                before = &path->nodes[current_connection->node_indices[1]];
                shared = &path->nodes[current_connection->node_indices[0]];
                after = &path->nodes[target_connection->node_indices[0]];
                first_angle = NuAngAdd(current_connection->rotation, NUANG_180DEG);
                second_angle = target_connection->rotation;
            } else if (current_connection->node_indices[1] == target_connection->node_indices[0]) {
                before = &path->nodes[current_connection->node_indices[0]];
                shared = &path->nodes[current_connection->node_indices[1]];
                after = &path->nodes[target_connection->node_indices[1]];
                first_angle = current_connection->rotation;
                second_angle = NuAngAdd(target_connection->rotation, NUANG_180DEG);
            } else if (current_connection->node_indices[1] == target_connection->node_indices[1]) {
                before = &path->nodes[current_connection->node_indices[0]];
                shared = &path->nodes[current_connection->node_indices[1]];
                after = &path->nodes[target_connection->node_indices[0]];
                first_angle = current_connection->rotation;
                second_angle = target_connection->rotation;
            } else {
                return 0;
            }
        } else {
            before = &path->nodes[current_connection->node_indices[1]];
            shared = &path->nodes[current_connection->node_indices[0]];
            after = &path->nodes[target_connection->node_indices[1]];
            first_angle = NuAngAdd(current_connection->rotation, NUANG_180DEG);
            second_angle = NuAngAdd(target_connection->rotation, NUANG_180DEG);
        }
        if (WithinConnection(system, &after->position, path, current_connection, 1, current_connection,
                             packet->current_route, object->field_0x289, NULL, 0.0f, 1) != 0) {
            return 0;
        }
        u32 right = CalculateRightIntersection(object->collision_radius, current_connection->horizontal_distance,
                                               &target_connection->horizontal_distance, first_angle, second_angle,
                                               before, &shared->position.y, shared->radius, after,
                                               &packet->right_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_RIGHT) | ((right & 1) << 5);
        u32 left = CalculateRightIntersection(object->collision_radius, target_connection->horizontal_distance,
                                              &current_connection->horizontal_distance, second_angle, first_angle,
                                              after, &shared->position.y, shared->radius, before,
                                              &packet->left_diversion);
        packet->movement_flags = (packet->movement_flags & ~AIPACKET_MOVEMENT_DIVERSION_LEFT) | ((left & 1) << 6);
    }
    return ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) |
           ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0);
}

extern "C" {
extern "C" void AISysFindRoute(AIPACKET *packet) {
    AIPATH *path = packet->path_info.path;
    if (path->route_count == 0 || packet->path_info.connection == NULL) {
        return;
    }

    const u16 valid_routes = packet->available_routes & packet->path_info.connection->route_mask;
    if (valid_routes == 0) {
        return;
    }

    i32 route = packet->next_route;
    if (route >= path->route_count) {
        route = 0;
    }
    const i32 first_route = route;
    do {
        if (((static_cast<u64>(valid_routes) >> route) & 1) != 0) {
            packet->current_route = static_cast<u8>(route);
            packet->next_route = static_cast<u8>(route + 1);
            return;
        }
        ++route;
        if (route >= path->route_count) {
            route = 0;
        }
    } while (route != first_route);
}

extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction) {
    NUVEC rotated;
    if (connection == NULL || packet->owner == NULL) {
        packet->path_info.connection = connection;
        packet->current_route = 0xff;
        packet->next_route = 0;
        return;
    }

    if (packet->path_info.connection == connection && packet->path_info.direction == direction &&
        (packet->path_info.flags & (AIPATHINFO_FLAG_ON_PATH | AIPATHINFO_FLAG_ROUTE_CHECKED)) !=
            AIPATHINFO_FLAG_ON_PATH) {
        return;
    }

    const u32 connection_flags = connection->traversal_flags[direction];
    if (connection_flags != 0) {
        if ((connection_flags & AIPATH_CONNECTION_FLAG_RESELECT_ROUTE) != 0) {
            AISysFindRoute(packet);
            return;
        }
        if ((packet->capabilities & connection_flags) == 0) {
            return;
        }
    }

    if (packet->path_info.connection != connection || packet->path_info.direction != direction) {
        packet->path_info.direction = static_cast<u8>(direction);
        packet->path_connection_state = 0;
        packet->path_info.connection = connection;

        AIPATHNODE &node = packet->path_info.path->nodes[connection->node_indices[0]];

        NUVEC delta;
        delta.x = position->x - node.position.x;
        delta.z = position->z - node.position.z;
        NuVecRotateY(&rotated, &delta, -connection->rotation);
        packet->path_info.dist = rotated.z / connection->horizontal_distance;
        packet->path_info.width = rotated.x;
        packet->path_info.flags &= static_cast<u8>(~AIPATHINFO_FLAG_ROUTE_CHECKED);
    }

    const u8 route_state = packet->path_info.flags & (AIPATHINFO_FLAG_ON_PATH | AIPATHINFO_FLAG_ROUTE_CHECKED);
    if (route_state == AIPATHINFO_FLAG_ON_PATH) {
        if (packet->current_route != 0xff &&
            ((static_cast<u64>(packet->path_info.connection->route_mask) >> packet->current_route) & 1) == 0) {
            packet->current_route = 0xff;
        }
        if (packet->current_route == 0xff) {
            AISysFindRoute(packet);
        }
        packet->path_info.flags |= AIPATHINFO_FLAG_ROUTE_CHECKED;
    }
}

}

static u32 AISysCharacterTestPathCnx(AISYS *system, APIOBJECT *object, AIPACKET *packet, AIPATHCNX *connection,
                                     i32 direction, f32 *nearest_distance) {
    AIPATH *path = packet->path_info.path;
    if (connection->last_search_checksum == path->search_checksum || path->nodes == NULL) {
        return 0;
    }
    connection->last_search_checksum = path->search_checksum;
    if (object->character_data != NULL) {
        u32 flags = connection->traversal_flags[0] | connection->traversal_flags[1];
        if (flags != 0) {
            if (direction != -1) {
                flags = connection->traversal_flags[direction];
            }
            if (flags != 0 && ((packet->capabilities & flags) == 0 || (flags & 0x98000000) != 0)) {
                return 0;
            }
        }
    }
    if ((object->flags_low & 0x80) == 0 && packet->current_route != 0xff &&
        ((static_cast<u64>(connection->route_mask) >> packet->current_route) & 1) == 0) {
        if (packet->path_info.connection == NULL || packet->path_info.connection == connection) {
            return 0;
        }
        AIPATHNODE *shared = NULL;
        if (packet->path_info.connection->node_indices[0] == connection->node_indices[0]) {
            shared = &path->nodes[connection->node_indices[0]];
        } else if (packet->path_info.connection->node_indices[0] == connection->node_indices[1]) {
            shared = &path->nodes[packet->path_info.connection->node_indices[0]];
        } else if (packet->path_info.connection->node_indices[1] == connection->node_indices[0]) {
            shared = &path->nodes[connection->node_indices[0]];
        } else if (packet->path_info.connection->node_indices[1] == connection->node_indices[1]) {
            shared = &path->nodes[connection->node_indices[1]];
        }
        if (shared == NULL || ((static_cast<u64>(shared->route_boundary_mask) >> packet->current_route) & 1) == 0) {
            return 0;
        }
    }
    if (direction == -1) {
        direction = 0;
    }
    AIPATHNODE *first = &path->nodes[connection->node_indices[0]];
    AIPATHNODE *second = &path->nodes[connection->node_indices[1]];
    f32 first_radius = first->radius;
    bool narrow = false;
    if (first_radius > object->collision_radius + 0.05f) {
        first_radius -= object->collision_radius;
    } else {
        narrow = true;
    }
    f32 second_radius = second->radius;
    if (second_radius > object->collision_radius + 0.05f) {
        second_radius -= object->collision_radius;
    } else {
        narrow = true;
    }
    if (first->has_special != 0 &&
        (path->updated_node_bits[connection->node_indices[0] >> 3] & (1 << (connection->node_indices[0] & 7))) == 0) {
        AIPathNodeUpdatePos(system, path, first);
    }
    if (second->has_special != 0 &&
        (path->updated_node_bits[connection->node_indices[1] >> 3] & (1 << (connection->node_indices[1] & 7))) == 0) {
        AIPathNodeUpdatePos(system, path, second);
    }
    NUVEC delta;
    delta.x = object->position.x - first->position.x;
    delta.z = object->position.z - first->position.z;
    NUVEC local;
    NuVecRotateY(&local, &delta, -connection->rotation);
    NUVEC radial;
    if (NuFabs(second_radius - first_radius) > connection->horizontal_distance) {
        if (second_radius > first_radius) {
            first_radius = second_radius;
            first = second;
        }
        f32 height_distance = NuFabs(first->position.y - packet->terrain_origin.y);
        f32 distance = NuVecXZDist(&object->position, &first->position, &radial);
        f32 clearance;
        if (first->min_height > packet->terrain_origin.y || packet->terrain_origin.y > first->max_height) {
            clearance = distance - first_radius;
            clearance = height_distance > clearance ? height_distance : clearance;
        } else if (first_radius >= distance) {
            packet->path_info.flags = ((packet->path_info.flags | 1) & ~8) | (narrow << 3);
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->last_path_position = object->position;
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            i32 node_index = first - path->nodes;
            path->inside_node_bits[node_index / 8] |= 1 << (node_index % 8);
            packet->inside_path_node = node_index;
            if (NuVecXZDistSqr(&object->position, &second->position, &radial) <= second_radius * second_radius) {
                node_index = second - path->nodes;
                path->inside_node_bits[node_index / 8] |= 1 << (node_index % 8);
                packet->inside_path_node = node_index;
            }
            return 1;
        } else {
            clearance = distance - first_radius;
        }
        if (*nearest_distance > clearance) {
            *nearest_distance = clearance;
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            if (!(first_radius < distance)) {
                packet->last_path_position.x = object->position.x;
                packet->last_path_position.z = object->position.z;
            } else {
                packet->last_path_position.x = first_radius * radial.x / distance + first->position.x;
                packet->last_path_position.z = first_radius * radial.z / distance + first->position.z;
            }
            packet->last_path_position.y = first->position.y;
            packet->inside_path_node = -1;
        }
        return 0;
    }

    const i32 angle = NuASin((second_radius - first_radius) / connection->horizontal_distance);
    NUVEC wall = local;
    if (wall.x < 0.0f) {
        wall.x = -wall.x;
    }
    NuVecRotateY(&wall, &wall, -angle);
    const f32 length = NU_COS_LUT(angle) * connection->horizontal_distance;
    if (wall.z < 0.0f) {
        f32 height_distance = NuFabs(first->position.y - packet->terrain_origin.y);
        f32 distance = NuVecXZDist(&object->position, &first->position, &radial);
        f32 clearance;
        if (first->min_height > packet->terrain_origin.y || packet->terrain_origin.y > first->max_height) {
            clearance = distance - first_radius;
            clearance = height_distance > clearance ? height_distance : clearance;
        } else if (first_radius >= distance) {
            packet->path_info.flags = ((packet->path_info.flags | 1) & ~8) | (narrow << 3);
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->last_path_position = object->position;
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            u8 node_index = connection->node_indices[0];
            path->inside_node_bits[node_index >> 3] |= 1 << (node_index & 7);
            packet->inside_path_node = connection->node_indices[0];
            return 1;
        } else {
            clearance = distance - first_radius;
        }
        if (*nearest_distance > clearance) {
            *nearest_distance = clearance;
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            if (!(first_radius < distance)) {
                packet->last_path_position.x = object->position.x;
                packet->last_path_position.z = object->position.z;
            } else {
                packet->last_path_position.x = first_radius * radial.x / distance + first->position.x;
                packet->last_path_position.z = first_radius * radial.z / distance + first->position.z;
            }
            packet->last_path_position.y = first->position.y;
            packet->inside_path_node = -1;
        }
    }
    if (wall.z > length) {
        f32 height_distance = NuFabs(second->position.y - packet->terrain_origin.y);
        f32 distance = NuVecXZDist(&object->position, &second->position, &radial);
        f32 clearance;
        if (second->min_height > packet->terrain_origin.y || packet->terrain_origin.y > second->max_height) {
            clearance = distance - second_radius;
            clearance = height_distance > clearance ? height_distance : clearance;
        } else if (second_radius >= distance) {
            packet->path_info.flags = ((packet->path_info.flags | 1) & ~8) | (narrow << 3);
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->last_path_position = object->position;
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            u8 node_index = connection->node_indices[1];
            path->inside_node_bits[node_index >> 3] |= 1 << (node_index & 7);
            packet->inside_path_node = connection->node_indices[1];
            return 1;
        } else {
            clearance = distance - second_radius;
        }
        if (*nearest_distance > clearance) {
            *nearest_distance = clearance;
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            if (!(second_radius < distance)) {
                packet->last_path_position.x = object->position.x;
                packet->last_path_position.z = object->position.z;
            } else {
                packet->last_path_position.x = second_radius * radial.x / distance + second->position.x;
                packet->last_path_position.z = second_radius * radial.z / distance + second->position.z;
            }
            packet->last_path_position.y = second->position.y;
            packet->inside_path_node = -1;
        }
    } else if (!(wall.z < 0.0f)) {
        f32 height, minimum, maximum;
        if (first_radius > local.z) {
            height = first->position.y;
            minimum = first->min_height;
            maximum = first->max_height;
            if (local.z > connection->horizontal_distance - second_radius) {
                minimum = second->min_height < minimum ? second->min_height : minimum;
                maximum = second->max_height > maximum ? second->max_height : maximum;
            }
        } else if (local.z > connection->horizontal_distance - second_radius) {
            height = second->position.y;
            minimum = second->min_height;
            maximum = second->max_height;
        } else {
            f32 denominator = connection->horizontal_distance - (first_radius + second_radius);
            f32 numerator = local.z - first_radius;
            f32 fraction = denominator == 0.0f || numerator == 0.0f ? 0.0f : numerator / denominator;
            f32 inverse = 1.0f - fraction;
            height = fraction * second->position.y + first->position.y * inverse;
            minimum = second->min_height * fraction + first->min_height * inverse;
            maximum = fraction * second->max_height + first->max_height * inverse;
        }
        const f32 height_distance = NuFabs(height - packet->terrain_origin.y);
        f32 clearance;
        if (height_distance > 0.0f && (minimum > packet->terrain_origin.y || packet->terrain_origin.y > maximum)) {
            clearance = wall.x - first_radius;
            clearance = height_distance > clearance ? height_distance : clearance;
        } else if (first_radius > wall.x) {
            packet->path_info.flags = ((packet->path_info.flags | 1) & ~8) | (narrow << 3);
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            packet->last_path_position = object->position;
            packet->inside_path_node = -1;
            if (first_radius > local.z) {
                if (NuVecXZDistSqr(&object->position, &first->position, &radial) <= first_radius * first_radius) {
                    u8 node_index = connection->node_indices[0];
                    path->inside_node_bits[node_index >> 3] |= 1 << (node_index & 7);
                    packet->inside_path_node = connection->node_indices[0];
                }
            } else if (local.z > length - second_radius) {
                if (NuVecXZDistSqr(&object->position, &second->position, &radial) <= second_radius * second_radius) {
                    u8 node_index = connection->node_indices[1];
                    path->inside_node_bits[node_index >> 3] |= 1 << (node_index & 7);
                    packet->inside_path_node = connection->node_indices[1];
                }
            }
            return 1;
        } else {
            clearance = wall.x - first_radius;
        }
        if (*nearest_distance > clearance) {
            *nearest_distance = clearance;
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            packet->path_info.dist = local.z / connection->horizontal_distance;
            packet->path_info.width = local.x;
            if (first_radius > wall.x) {
                packet->last_path_position.x = object->position.x;
                packet->last_path_position.z = object->position.z;
            } else {
                wall.x = first_radius;
                NuVecRotateY(&wall, &wall, angle);
                if (local.x < 0.0f) {
                    wall.x = -wall.x;
                }
                NuVecRotateY(&wall, &wall, connection->rotation);
                packet->last_path_position.x = first->position.x + wall.x;
                packet->last_path_position.z = first->position.z + wall.z;
            }
            packet->last_path_position.y = first->position.y;
            packet->inside_path_node = -1;
        }
    }
    return 0;
}

extern "C" {
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground) {
        f32 nearest_distance_squared = 3.402823466e+38f;

        const u8 previous_flags = packet->path_info.flags;
        packet->path_info.flags =
            (previous_flags &
             static_cast<u8>(~(AIPATHINFO_FLAG_ON_PATH | AIPATHINFO_FLAG_WAS_ON_PATH | AIPATHINFO_FLAG_NARROW_PATH))) |
            ((previous_flags & AIPATHINFO_FLAG_ON_PATH) << 1);

        if (system == NULL || system->path_sys == NULL || system->path_sys->path_count == 0) {
            AISysCharacterSetPath(packet, NULL);
            AISysCharacterSetPathCnx(packet, &object->position, NULL, 0);
            return;
        }

        AIPATH *path = packet->path_info.path;
        if (path == NULL) {
            if ((packet->navigation_flags & AIPACKET_NAVIGATION_FLAG_SEARCH_ALL_PATHS) != 0)
                AISysCharacterSetPath(packet, system->path_sys->paths[0]);
            else
                AISysCharacterSetPath(packet, system->path_sys->active_path);
            AISysCharacterSetPathCnx(packet, &object->position, NULL, 0);
            packet->path_info.next_check = 0;
            packet->path_info.path_index = 0;
        }
        path = packet->path_info.path;
        AISysResetPathSearchConnectionChecks(path);

        AIPATHCNX *current_connection = packet->path_info.connection;
        if (current_connection != NULL) {
            const i32 current_direction = packet->path_info.direction;
            if (AISysCharacterTestPathCnx(system, object, packet, current_connection, current_direction,
                                          &nearest_distance_squared) != 0) {
                return;
            }

            const bool check_adjacent_connections =
                (object->flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 ||
                (packet->field_0x180 == NULL && packet->path_connection_state == 0 && ground != 0);
            if (!check_adjacent_connections) {
                return;
            }

            path = packet->path_info.path;
            AIPATHNODE &first_node = path->nodes[current_connection->node_indices[current_direction]];
            for (i32 connection_index = 0; connection_index < first_node.connection_count; ++connection_index) {
                AIPATHCNX *candidate = first_node.connections[connection_index];
                if (candidate != current_connection &&
                    AISysCharacterTestPathCnx(system, object, packet, candidate,
                                              candidate->node_indices[0] !=
                                                  current_connection->node_indices[current_direction],
                                              &nearest_distance_squared) != 0)
                    return;
            }

            path = packet->path_info.path;
            const i32 other_direction = current_direction == 0;
            AIPATHNODE &second_node = path->nodes[current_connection->node_indices[other_direction]];
            for (i32 connection_index = 0; connection_index < second_node.connection_count; ++connection_index) {
                AIPATHCNX *candidate = second_node.connections[connection_index];
                if (candidate != current_connection &&
                    AISysCharacterTestPathCnx(system, object, packet, candidate,
                                              candidate->node_indices[0] !=
                                                  current_connection->node_indices[other_direction],
                                              &nearest_distance_squared) != 0)
                    return;
            }
        }

        path = packet->path_info.path;
        if ((packet->navigation_flags & AIPACKET_NAVIGATION_FLAG_SEARCH_ALL_PATHS) == 0) {
            i32 remaining_checks = path->node_count < checks ? path->node_count : checks;
            while (remaining_checks-- > 0) {
                AIPATHNODE &node = path->nodes[packet->path_info.next_check];
                for (i32 connection_index = 0; connection_index < node.connection_count; ++connection_index) {
                    if (AISysCharacterTestPathCnx(system, object, packet, node.connections[connection_index], -1,
                                                  &nearest_distance_squared) != 0) {
                        return;
                    }
                }
                path = packet->path_info.path;
                packet->path_info.next_check = (packet->path_info.next_check + 1) % path->node_count;
            }
            return;
        }

        // Characters allowed to search across paths retain their current
        // cursor after probing, just as they retain their current connection.
        AIPATH *original_path = packet->path_info.path;
        AIPATHCNX *original_connection = packet->path_info.connection;
        const u8 original_direction = packet->path_info.direction;

        if (packet->path_info.path_index == 0xff) {
            packet->path_info.path_index = original_path->index;
        }
        if (packet->path_info.path_index >= system->path_sys->path_count) {
            packet->path_info.path_index = 0;
        }

        path = system->path_sys->paths[packet->path_info.path_index];
        packet->path_info.path = path;
        if (packet->path_info.next_check >= path->node_count) {
            packet->path_info.next_check = 0;
        }
        AISysResetPathSearchConnectionChecks(path);

        i32 position_inside_path_bounds = 0;
        for (i32 remaining_checks = checks; remaining_checks > 0; --remaining_checks) {
            if (!position_inside_path_bounds) {
                position_inside_path_bounds =
                    AIPathCheckExtents(packet->path_info.path, &packet->owner->apiobj.position);
            }

            if (position_inside_path_bounds) {
                path = packet->path_info.path;
                AIPATHNODE &node = path->nodes[packet->path_info.next_check];
                for (i32 connection_index = 0; connection_index < node.connection_count; ++connection_index) {
                    if (AISysCharacterTestPathCnx(system, object, packet, node.connections[connection_index], -1,
                                                  &nearest_distance_squared) != 0) {
                        return;
                    }
                }

                ++packet->path_info.next_check;
                path = packet->path_info.path;
                if (packet->path_info.next_check < path->node_count) {
                    continue;
                }
            }

            packet->path_info.next_check = 0;
            ++packet->path_info.path_index;
            if (packet->path_info.path_index >= system->path_sys->path_count) {
                packet->path_info.path_index = 0;
            }

            path = system->path_sys->paths[packet->path_info.path_index];
            packet->path_info.path = path;
            AISysCharacterSetPathCnx(packet, &object->position, NULL, 0);
            AISysResetPathSearchConnectionChecks(packet->path_info.path);
            position_inside_path_bounds = 0;
        }

        packet->path_info.path = original_path;
        packet->path_info.connection = original_connection;
        packet->path_info.direction = original_direction;
    }

    void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed) {
        u8 movement_source_flags = packet->field_0x1e7;
        const bool should_find_path =
            (movement_source_flags & AIPACKET_MOVEMENT_FORCE_PATH_REFRESH) == 0 &&
            (((object->flags_low & 4) == 0 || (packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) == 0) ||
             object->supporting_platform_id != -1);

        if (should_find_path) {
            if (packet->path_info.path == NULL) {
                packet->last_path_position = object->position;
            } else {
                AISysGetCharacterPathPos(system, object, packet, (object->field_0x1fa & 8) == 0 ? 1 : 0, checks);
                movement_source_flags = packet->field_0x1e7;
            }

            if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) == 0) {
                packet->time_off_path += elapsed;
            } else {
                packet->time_off_path = 0.0f;
            }
        } else {
            AIPATH *path = packet->path_info.path;
            if (path != NULL && packet->path_info.connection != NULL) {
                u8 node_index = packet->path_info.connection->node_indices[0];
                AIPATHNODE *first = &path->nodes[node_index];
                AIPATHNODE *second = &path->nodes[packet->path_info.connection->node_indices[1]];
                if (first->has_special != 0 &&
                    (path->updated_node_bits[node_index / 8] & (1u << (node_index % 8))) == 0) {
                    AIPathNodeUpdatePos(system, path, first);
                }
                if (second->has_special != 0) {
                    node_index = packet->path_info.connection->node_indices[1];
                    path = packet->path_info.path;
                    if ((path->updated_node_bits[node_index / 8] & (1u << (node_index % 8))) == 0) {
                        AIPathNodeUpdatePos(system, path, second);
                    }
                }
            }

            const i16 node_index = packet->inside_path_node;
            if (node_index != -1) {
                path = packet->path_info.path;
                if (path != NULL) {
                    path->inside_node_bits[node_index / 8] |= static_cast<u8>(1u << (node_index % 8));
                }
            }
            packet->time_off_path = 0.0f;
            movement_source_flags = packet->field_0x1e7;
        }

        packet->field_0x1e7 = movement_source_flags & static_cast<u8>(~AIPACKET_MOVEMENT_FORCE_PATH_REFRESH);
    }

    void AISysGetPathPosEx(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks,
                           i32 search_all_paths, f32 *nearest_distance_squared) {
        info->flags &= static_cast<u8>(~AIPATHINFO_FLAG_ON_PATH);
        if (system == NULL || system->path_sys == NULL || system->path_sys->path_count == 0) {
            return;
        }

        if (search_all_paths != 0) {
            path = info->path;
            if (path == NULL) {
                path = system->path_sys->paths[0];
                info->connection = NULL;
                info->path = path;
                info->next_check = 0;
            }
        } else {
            if (path == NULL) {
                path = system->path_sys->active_path;
            }
            if (info->path != path) {
                info->connection = NULL;
                info->path = path;
                info->next_check = 0;
            }
        }
        AISysResetPathSearchConnectionChecks(path);

        AIPATHCNX *current = info->connection;
        if (current != NULL) {
            if (WithinConnection(system, position, path, current, 1, NULL, 0xff, 0xff, info, 0.0f, 1)) {
                return;
            }
            AIPATHNODE *node = &info->path->nodes[current->node_indices[info->direction]];
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX *candidate = node->connections[index];
                if (candidate != current &&
                    WithinConnection(system, position, path, candidate, 1, NULL, 0xff, 0xff, info, 0.0f, 1)) {
                    return;
                }
            }
            node = &info->path->nodes[current->node_indices[info->direction == 0]];
            for (i32 index = 0; index < node->connection_count; ++index) {
                AIPATHCNX *candidate = node->connections[index];
                if (candidate != current &&
                    WithinConnection(system, position, path, candidate, 1, NULL, 0xff, 0xff, info, 0.0f, 1)) {
                    return;
                }
            }
        }

        if (search_all_paths != 0) {
            AIPATH *saved_path = info->path;
            AIPATHCNX *saved_connection = info->connection;
            u8 saved_direction = info->direction;
            if (info->path_index == 0xff) {
                info->path_index = saved_path->index;
            }
            if (info->path_index >= system->path_sys->path_count) {
                info->path_index = 0;
            }
            info->path = system->path_sys->paths[info->path_index];
            if (info->next_check >= info->path->node_count) {
                info->next_check = 0;
            }
            AISysResetPathSearchConnectionChecks(info->path);
            u8 start_path = info->path_index;
            u16 start_node = info->next_check;
            i32 inside_extents = 0;
            while (checks != 0) {
                if (inside_extents == 0) {
                    inside_extents = AIPathCheckExtents(info->path, position);
                }
                if (inside_extents != 0) {
                    AIPATHNODE *node = &info->path->nodes[info->next_check];
                    for (i32 index = 0; index < node->connection_count; ++index) {
                        AIPATHCNX *candidate = node->connections[index];
                        if (WithinConnection(system, position, info->path, candidate, 1, NULL, 0xff, 0xff, info, 0.0f,
                                             1)) {
                            return;
                        }
                        if (nearest_distance_squared != NULL) {
                            AIPATHCNX *connection = node->connections[index];
                            f32 distance = NuLineToPointDistSqrEx(
                                &info->path->nodes[connection->node_indices[0]].position,
                                &info->path->nodes[connection->node_indices[1]].position, position, NULL);
                            if (distance < *nearest_distance_squared) {
                                *nearest_distance_squared = distance;
                                saved_connection = candidate;
                                saved_path = info->path;
                            }
                        }
                    }
                    ++info->next_check;
                }
                if (inside_extents == 0 || info->next_check >= info->path->node_count) {
                    info->next_check = 0;
                    ++info->path_index;
                    if (info->path_index >= system->path_sys->path_count) {
                        info->path_index = 0;
                    }
                    info->path = system->path_sys->paths[info->path_index];
                    info->connection = NULL;
                    AISysResetPathSearchConnectionChecks(info->path);
                    inside_extents = 0;
                }
                --checks;
                if (info->path_index == start_path && info->next_check == start_node) {
                    break;
                }
            }
            info->path = saved_path;
            info->connection = saved_connection;
            info->direction = saved_direction;
        } else {
            i32 remaining = info->path->node_count < checks ? info->path->node_count : checks;
            while (remaining != 0) {
                AIPATHNODE *node = &info->path->nodes[info->next_check];
                for (i32 index = 0; index < node->connection_count; ++index) {
                    if (WithinConnection(system, position, path, node->connections[index], 1, NULL, 0xff, 0xff, info,
                                         0.0f, 1)) {
                        return;
                    }
                }
                info->next_check = (info->next_check + 1) % info->path->node_count;
                --remaining;
            }
        }
    }

    void AISysGetPathPos(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks) {
        AISysGetPathPosEx(system, position, info, path, checks, 0, NULL);
    }

extern "C" void AIFormationFollow(AIPACKET *packet) {
    AIGROUP *group = packet->group;
    if (packet->group_row < group->row_count) {
        AIROW *row = &group->rows[packet->group_row];
        u8 column = packet->group_column;
        NUVEC offset;
        if ((packet->movement_event_flags & 1) != 0) {
            offset.x = (group->count_across & 1) != 0 ? 0.0f : -(0.5f * group->x_spacing);
        } else {
            f32 column_offset = ((column + 1) / 2) * group->x_spacing;
            offset.x = (column & 1) != 0 ? -column_offset : column_offset;
        }
        if (group->is_reversed) {
            offset.x = -offset.x;
        }
        offset.y = 0.0f;
        offset.z = 0.0f;
        NuVecRotateY(&offset, &offset, row->y_rot);
        NUVEC destination;
        NuVecAdd(&destination, &offset, &row->pos);
        packet->movement_flags |= 8;
        AIMoveInstruction(packet, &destination, 0.0f, &row->path_info, AIPACKET_MOVEMENT_FORMATION, 0.0f);
        packet->script_process.action_pos.x = 0.0f;
        packet->script_process.action_pos.y = 0.0f;
        packet->script_process.action_pos.z = 100.0f;
        NuVecRotateY(&packet->script_process.action_pos, &packet->script_process.action_pos, row->y_rot);
        NuVecAdd(&packet->script_process.action_pos, &packet->script_process.action_pos, &row->pos);
        packet->movement_look_target = &packet->script_process.action_pos;
    }
}

    void AIMoveInstruction(AIPACKET *packet, NUVEC *destination, f32 stopping_distance, AIPATHINFO *path_info, i32 mode,
                           f32 movement_parameter) {
        AIGROUP *group = packet->group;
        if (group != NULL && group->is_in_formation) {
            switch (mode) {
                case AIPACKET_MOVEMENT_WANDER:
                    if (group->leader == reinterpret_cast<APIOBJECT *>(packet->owner))
                        FormationMove(group, RowMoveWander);
                    AIFormationFollow(packet);
                    return;
                case AIPACKET_MOVEMENT_FORMATION:
                    mode = AIPACKET_MOVEMENT_TO_DESTINATION;
                    break;
                case AIPACKET_MOVEMENT_TO_DESTINATION:
                    if (group->leader == reinterpret_cast<APIOBJECT *>(packet->owner))
                        FormationMove(group, RowMoveTowards);
                    AIFormationFollow(packet);
                    return;
            }
        }

        if (destination != NULL) {
            packet->fallback_destination = *destination;
        }
        if (path_info != NULL) {
            packet->fallback_path_info = *path_info;
        }
        packet->fallback_stopping_distance = stopping_distance;
        packet->movement_flags = (packet->movement_flags & static_cast<u8>(~AIPACKET_MOVEMENT_MODE_MASK)) |
                                 (mode & AIPACKET_MOVEMENT_MODE_MASK);
        packet->movement_parameter = movement_parameter;
    }

}

static i32 Action_MoveAwayFromNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                   i32 param_count, i32 first_time, f32) {
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL || packet->path_info.path == NULL ||
        packet->path_info.connection == NULL) {
        return 1;
    }
    if (first_time != 0) {
        if (param_count == 0) {
            return 0;
        }
        for (i32 i = 1; i < param_count; i++) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
        AIPATHNODE *node = AIPathFindNode(sys, packet->path_info.path, params[0]);
        processor->action_data_3 = node;
        if (node == NULL || node->connection_count == 0) {
            return 1;
        }
        i32 index = node - packet->path_info.path->nodes;
        AISysCharacterSetPath(packet, packet->path_info.path);
        processor->path_info.connection = node->connections[0];
        processor->path_info.dist = index == processor->path_info.connection->node_indices[0] ? 0.0f : 1.0f;
        processor->path_info.width = 0.0f;
        processor->path_info.direction = 0;
        processor->path_info.on_path = 1;
        AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_RETREAT,
                          packet->movement_instruction_parameter);
        return 0;
    }
    AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
    if (node != NULL) {
        f32 distance = NuVecXZDistSqr(&packet->terrain_origin, &node->position, &difference);
        if (!(node->radius_squared > distance)) {
            AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_RETREAT,
                              packet->movement_instruction_parameter);
            return 0;
        }
    }
    return 1;
}

__used__ static i32 Action_FollowPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 is_first_time, f32 elapsed) {
    (void)sys;

    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }
    if (packet->path_info.path == NULL || packet->path_info.connection == NULL) {
        return 0;
    }

    f32 completion_time = 0.0f;
    if (is_first_time != 0) {
        packet->movement_target = NULL;

        f32 minimum_time = 0.0f;
        f32 maximum_time = 0.0f;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }

            char *value = NuStrIStr(params[index], "mintime");
            if (value != NULL) {
                minimum_time = AIParamToFloatEx(packet, processor, value + 8);
                continue;
            }

            value = NuStrIStr(params[index], "maxtime");
            if (value != NULL) {
                maximum_time = AIParamToFloatEx(packet, processor, value + 8);
                continue;
            }

            processor->action_timer = AIParamToFloatEx(packet, processor, params[index]);
        }

        if (maximum_time > minimum_time) {
            processor->action_timer = NuRandFloat() * maximum_time + (1.0f - NuRandFloat()) * minimum_time;
        }
    }

    AIMoveInstruction(packet, NULL, 0.0f, NULL, AIPACKET_MOVEMENT_WANDER, packet->movement_instruction_parameter);

    if (!(processor->action_timer > completion_time)) {
        return 0;
    }

    processor->action_timer -= elapsed;
    return completion_time >= processor->action_timer;
}

__used__ static i32 Action_GoToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 is_first_time, f32 elapsed) {
    NUVEC distance_vector;
    enum GO_TO_LOCATOR_FLAGS : u8 {
        GO_TO_LOCATOR_ON_GROUND = 1 << 0,
        GO_TO_LOCATOR_XZ_RANGE_CHECK = 1 << 1,
        GO_TO_LOCATOR_FACE_OPPONENT = 1 << 2,
        GO_TO_LOCATOR_IGNORE_PATH = 1 << 3,
        GO_TO_LOCATOR_MUST_REACH_DESTINATION = 1 << 4,
    };

    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
        return 1;
    }

    if (is_first_time == 0) {
        AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
        if (locator == NULL) {
            return 1;
        }

        if ((processor->action_data_1 & GO_TO_LOCATOR_IGNORE_PATH) != 0) {
            AIMoveInstruction(packet, &locator->position, 0.0f, &locator->path_info, AIPACKET_MOVEMENT_DIRECT,
                              packet->movement_instruction_parameter);
        } else {
            AIMoveInstruction(packet, &locator->position, 0.0f, &locator->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                              packet->movement_instruction_parameter);
        }

        if ((processor->action_data_1 & GO_TO_LOCATOR_FACE_OPPONENT) != 0 && packet->opponent_object != NULL) {
            packet->movement_look_target = &packet->opponent_object->position;
        }
        if ((processor->action_data_1 & GO_TO_LOCATOR_ON_GROUND) != 0 && packet->owner->apiobj.field_0x27d == 0) {
            return 0;
        }

        const f32 distance_squared = (processor->action_data_1 & GO_TO_LOCATOR_XZ_RANGE_CHECK) != 0
                                         ? NuVecXZDistSqr(&packet->terrain_origin, &locator->position, &distance_vector)
                                         : NuVecDistSqr(&packet->terrain_origin, &locator->position, &distance_vector);
        const f32 reach_distance = packet->movement_instruction_parameter + ai_moveradius +
                                   elapsed * packet->owner->apiobj.horizontal_velocity_magnitude;
        if (reach_distance * reach_distance > distance_squared) {
            if ((processor->action_data_1 & GO_TO_LOCATOR_FACE_OPPONENT) == 0 || packet->opponent_object == NULL) {
                packet->movement_look_target = &processor->action_pos;
            }
            if (!(processor->action_timer > 0.0f)) {
                return 1;
            }
            processor->action_timer -= elapsed;
            if (processor->action_timer < 0.0f) {
                processor->action_timer = 0.0f;
            }
            return 0;
        }
        if ((packet->field_0x1e6 & 0x40) != 0 &&
            (processor->action_data_1 & GO_TO_LOCATOR_MUST_REACH_DESTINATION) != 0 &&
            AIBigJumpToDestinationFn != NULL) {
            return AIBigJumpToDestinationFn(&packet->owner->apiobj, &locator->position) != 0;
        }
        return 0;
    }

    processor->action_data_1 = 0;
    processor->action_data_3 = processor->unknown_a4;

    i32 random_locator_count = 0;
    i32 use_personal_name = 0;
    i32 use_indexed_name = 0;
    f32 minimum_time = 0.0f;
    f32 maximum_time = 0.0f;
    char locator_name[64];

    if (param_count != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }

            if (NuStrIStr(params[index], "name") != NULL || NuStrIStr(params[index], "teleport") != NULL) {
                ++index;
                if (index < param_count) {
                    if (use_indexed_name != 0 && packet->owner->apiobj.field_0x27c != -1) {
                        sprintf(locator_name, "%s_%d", params[index], packet->owner->apiobj.field_0x27c);
                    } else if (use_personal_name != 0 && packet->owner->apiobj.character_data != NULL) {
                        sprintf(locator_name, "%s_%s", params[index], packet->owner->apiobj.character_data->file);
                    } else if (random_locator_count != 0) {
                        sprintf(locator_name, "%s_%d", params[index], NuRand(NULL) % random_locator_count);
                    } else {
                        sprintf(locator_name, params[index]);
                    }

                    processor->action_data_3 = AIPathFindLocator(sys, locator_name);
                }
                if (__builtin_expect(NuStrIStr(params[index - 1], "teleport") != NULL, 0)) {
                    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
                    if (locator == NULL) {
                        return 1;
                    }
                    packet->owner->apiobj.position.x = locator->position.x;
                    packet->owner->apiobj.position.y = locator->position.y;
                    packet->owner->apiobj.position.z = locator->position.z;
                    return 1;
                }
                continue;
            }

            if (NuStrIStr(params[index], "personal") != NULL) {
                use_personal_name = 1;
                continue;
            }
            if (NuStrIStr(params[index], "indexed") != NULL) {
                use_indexed_name = 1;
                continue;
            }

            char *value = NuStrIStr(params[index], "random");
            if (value != NULL) {
                random_locator_count = AIParamToFloatEx(packet, processor, value + NuStrLen("random") + 1);
                continue;
            }
            value = NuStrIStr(params[index], "waittime");
            if (value != NULL) {
                processor->action_timer = AIParamToFloatEx(packet, processor, value + NuStrLen("waittime") + 1);
                continue;
            }
            value = NuStrIStr(params[index], "mintime");
            if (value != NULL) {
                minimum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("mintime") + 1);
                continue;
            }
            value = NuStrIStr(params[index], "maxtime");
            if (value != NULL) {
                maximum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("maxtime") + 1);
                continue;
            }
            value = NuStrIStr(params[index], "goalrange");
            if (value != NULL) {
                packet->movement_instruction_parameter =
                    AIParamToFloatEx(packet, processor, value + NuStrLen("goalrange") + 1);
                continue;
            }

            if (NuStrICmp(params[index], "on_ground") == 0) {
                processor->action_data_1 |= GO_TO_LOCATOR_ON_GROUND;
            } else if (NuStrICmp(params[index], "xz_rangecheck") == 0) {
                processor->action_data_1 |= GO_TO_LOCATOR_XZ_RANGE_CHECK;
            } else if (NuStrICmp(params[index], "face_opponent") == 0) {
                processor->action_data_1 |= GO_TO_LOCATOR_FACE_OPPONENT;
            } else if (NuStrICmp(params[index], "ignore_path") == 0) {
                processor->action_data_1 |= GO_TO_LOCATOR_IGNORE_PATH;
            } else if (NuStrICmp(params[index], "must_reach_destination") == 0) {
                processor->action_data_1 |= GO_TO_LOCATOR_MUST_REACH_DESTINATION;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }

    if (processor->action_timer == 0.0f) {
        if (maximum_time > minimum_time) {
            processor->action_timer = NuRandFloat() * (maximum_time - minimum_time) + minimum_time;
        } else {
            processor->action_timer = 0.01f;
        }
    }

    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
    if (locator == NULL) {
        return 1;
    }

    if ((processor->action_data_1 & GO_TO_LOCATOR_IGNORE_PATH) != 0) {
        AIMoveInstruction(packet, &locator->position, 0.0f, &locator->path_info, AIPACKET_MOVEMENT_DIRECT,
                          packet->movement_instruction_parameter);
    } else {
        AIMoveInstruction(packet, &locator->position, 0.0f, &locator->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                          packet->movement_instruction_parameter);
    }

    processor->action_pos.y = 0.0f;
    processor->action_pos.x = 0.0f;
    processor->action_pos.z = 1.0f;
    NuVecRotateY(&processor->action_pos, &processor->action_pos, locator->flags);
    NuVecAdd(&processor->action_pos, &processor->action_pos, &locator->position);
    return 0;
}

__used__ static i32 Action_GoToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed) {
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL || packet->path_info.path == NULL ||
        packet->path_info.connection == NULL || (packet->owner->apiobj.field_0x1f4 & 0x400) == 0 ||
        packet->field_0x134 == 0xff) {
        return 1;
    }
    AICREATURE *creature = &sys->creatures[packet->field_0x134];
    NUVEC *origin = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
    if (origin == NULL) {
        origin = &creature->pos;
    }

    if (first_time != 0) {
        packet->movement_instruction_parameter = 0.2f;
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        if (param_count != 0) {
            for (i32 index = 0; index < param_count; ++index) {
                if (AIActionParseSpeedFn != NULL &&
                    AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                    continue;
                }
                char *value = NuStrIStr(params[index], "waittime");
                if (value != NULL) {
                    processor->action_timer = AIParamToFloatEx(packet, processor, value + NuStrLen("waittime") + 1);
                } else if ((value = NuStrIStr(params[index], "mintime")) != NULL) {
                    min_time = AIParamToFloatEx(packet, processor, value + NuStrLen("mintime") + 1);
                } else if ((value = NuStrIStr(params[index], "maxtime")) != NULL) {
                    max_time = AIParamToFloatEx(packet, processor, value + NuStrLen("maxtime") + 1);
                } else if (NuStrICmp(params[index], "xz_rangecheck") == 0) {
                    processor->action_data_2 = 1;
                } else if ((value = NuStrIStr(params[index], "goalrange")) != NULL) {
                    packet->movement_instruction_parameter =
                        AIParamToFloatEx(packet, processor, value + NuStrLen("goalrange") + 1);
                } else {
                    packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
                }
            }
        }
        if (processor->action_timer == 0.0f) {
            if (min_time < max_time) {
                processor->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
            } else {
                processor->action_timer = 0.01f;
            }
        }
        AIMoveInstruction(packet, origin, 0.0f, &creature->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                          packet->movement_instruction_parameter);
        processor->action_pos = {0.0f, 0.0f, 1.0f};
        NuVecRotateY(&processor->action_pos, &processor->action_pos, creature->y_rot);
        NuVecAdd(&processor->action_pos, &processor->action_pos, origin);
        return 0;
    }

    AIMoveInstruction(packet, origin, 0.0f, &creature->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    const f32 distance_squared = processor->action_data_2 != 0
                                     ? NuVecXZDistSqr(&packet->terrain_origin, origin, &difference)
                                     : NuVecDistSqr(&packet->terrain_origin, origin, &difference);
    const f32 range = packet->movement_instruction_parameter + ai_moveradius +
                      elapsed * packet->owner->apiobj.horizontal_velocity_magnitude;
    if (distance_squared < range * range) {
        f32 remaining_time = processor->action_timer;
        packet->movement_look_target = &processor->action_pos;
        if (!(remaining_time > 0.0f)) {
            return 1;
        }
        remaining_time -= elapsed;
        processor->action_timer = remaining_time;
        if (remaining_time < 0.0f) {
            processor->action_timer = 0.0f;
        }
    }
    return 0;
}

__used__ static i32 Action_GoToNodeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL || packet->path_info.path == NULL ||
        packet->path_info.connection == NULL) {
        return 1;
    }
    if (first_time != 0) {
        if (param_count == 0) {
            return 0;
        }
        i32 selected = static_cast<i32>(NuRandFloat() * param_count);
        if (selected >= param_count) {
            selected = param_count - 1;
        }
        processor->action_data_3 = AIPathFindNode(sys, packet->path_info.path, params[selected]);
        AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
        if (node != NULL && node->connection_count != 0) {
            i32 node_index = node - packet->path_info.path->nodes;
            processor->path_info.path = packet->path_info.path;
            processor->path_info.connection = node->connections[0];
            processor->path_info.dist = node_index == processor->path_info.connection->node_indices[0] ? 0.0f : 1.0f;
            processor->path_info.direction = 0;
            processor->path_info.flags |= 1;
            processor->path_info.width = 0.0f;
            AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                              packet->movement_instruction_parameter);
        } else {
            return 1;
        }
        return 0;
    }
    AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
    if (node == NULL) {
        return 1;
    }
    const f32 distance_squared = NuVecXZDistSqr(&packet->terrain_origin, &node->position, &difference);
    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return distance_squared < node->radius_squared;
}

__used__ static i32 Action_GoToNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    NUVEC difference;
    if (packet == NULL || packet->owner == NULL || packet->path_info.path == NULL ||
        packet->path_info.connection == NULL) {
        return 1;
    }

    if (first_time != 0) {
        if (param_count == 0) {
            return 0;
        }
        for (i32 index = 1; index < param_count; ++index) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
        processor->action_data_3 = AIPathFindNode(sys, packet->path_info.path, params[0]);
        AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
        if (!(node != NULL && node->connection_count != 0)) {
            return 1;
        } else {
            i32 node_index = node - packet->path_info.path->nodes;
            processor->path_info.path = packet->path_info.path;
            processor->path_info.connection = node->connections[0];
            processor->path_info.dist = node_index == processor->path_info.connection->node_indices[0] ? 0.0f : 1.0f;
            processor->path_info.direction = 0;
            processor->path_info.flags |= 1;
            processor->path_info.width = 0.0f;
            AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                              packet->movement_instruction_parameter);
        }
        return 0;
    }

    AIPATHNODE *node = static_cast<AIPATHNODE *>(processor->action_data_3);
    if (node == NULL) {
        return 1;
    }
    const f32 distance_squared = NuVecXZDistSqr(&packet->terrain_origin, &node->position, &difference);
    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return distance_squared < node->radius_squared;
}

static i32 Action_MoveAwayFromOpponent(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                       i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[i], "face") == 0) {
                processor->action_data_1 = 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (packet->opponent_object != NULL && packet->opponent_object->ai != NULL) {
        AIPACKET *target = packet->opponent_object->ai;
        AIMoveInstruction(packet, &target->last_path_position, target->mover_height, &target->path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        if (processor->action_data_1 != 0) {
            packet->movement_look_target = &packet->opponent_object->position;
        }
    }
    return 0;
}

static i32 Action_MoveAwayFromPlayer2(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                      i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[i], "face") == 0) {
                processor->action_data_1 = 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (sys->player_2 != NULL) {
        AIPACKET *target = sys->player_2->ai;
        AIMoveInstruction(packet, &target->last_path_position, target->mover_height, &target->path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        if (processor->action_data_1 != 0) {
            packet->movement_look_target = &sys->player_2->position;
        }
    }
    return 0;
}

static i32 Action_MoveAwayFromPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                     i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[i], "face") == 0) {
                processor->action_data_1 = 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (sys->player_1 != NULL) {
        AIPACKET *target = sys->player_1->ai;
        AIMoveInstruction(packet, &target->last_path_position, target->mover_height, &target->path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        if (processor->action_data_1 != 0) {
            packet->movement_look_target = &sys->player_1->position;
        }
    }
    return 0;
}

static i32 Action_RetreatFromOpponent(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                      i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        packet->movement_instruction_parameter = 1.0f;
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (packet->opponent_object != NULL && packet->opponent_object->ai != NULL) {
        AIPACKET *target = packet->opponent_object->ai;
        AIMoveInstruction(packet, &target->last_path_position, target->mover_height, &target->path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
    }
    return 0;
}

static i32 Action_RetreatFromNearestOpponent(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        packet->movement_instruction_parameter = 1.0f;
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (packet->nearest_opponent_object != NULL) {
        AIPACKET *target = packet->nearest_opponent_object->ai;
        AIMoveInstruction(packet, &target->last_path_position, target->mover_height, &target->path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
    }
    return 0;
}

i32 Action_CircleOpponent(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                          i32 first_time, f32) {
    NUVEC difference;
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0) {
                packet->circle_clockwise = 0;
            } else if (NuStrICmp(params[i], "CLOCKWISE") == 0) {
                packet->circle_clockwise = 1;
            } else if (NuStrICmp(params[i], "REVERSE") == 0) {
                packet->circle_clockwise = !packet->circle_clockwise;
            } else if (NuStrICmp(params[i], "RANDOM") == 0) {
                if (NuRandFloat() < 0.5f) {
                    packet->circle_clockwise = 0;
                } else {
                    packet->circle_clockwise = 1;
                }
            } else if (NuStrICmp(params[i], "facing") == 0) {
                processor->action_data_1 |= 1;
            } else if (NuStrICmp(params[i], "can_go_off_path") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(params[i], "currentdist") == 0) {
                processor->action_data_1 |= 4;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (packet->opponent_object != NULL && packet->opponent_object->ai != NULL) {
        if ((processor->action_data_1 & 4) != 0) {
            packet->movement_instruction_parameter =
                NuVecXZDist(&packet->terrain_origin, &packet->opponent_object->ai->last_path_position, &difference);
        }
        AIPACKET *target = packet->opponent_object->ai;
        NUVEC *position = (processor->action_data_1 & 2) != 0 ? &target->terrain_origin : &target->last_path_position;
        AIMoveInstruction(packet, position, target->mover_height, &target->path_info, AIPACKET_MOVEMENT_CIRCLE,
                          packet->movement_instruction_parameter);
        if ((processor->action_data_1 & 1) != 0) {
            packet->movement_look_target = position;
        }
    }
    return 0;
}

i32 Action_CirclePlayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                        i32 first_time, f32) {
    NUVEC difference;
    if (packet == NULL || sys == NULL || sys->player_1 == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0) {
                packet->circle_clockwise = 0;
            } else if (NuStrICmp(params[i], "CLOCKWISE") == 0) {
                packet->circle_clockwise = 1;
            } else if (NuStrICmp(params[i], "REVERSE") == 0) {
                packet->circle_clockwise = !packet->circle_clockwise;
            } else if (NuStrICmp(params[i], "facing") == 0) {
                processor->action_data_1 |= 1;
            } else if (NuStrICmp(params[i], "can_go_off_path") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(params[i], "currentdist") == 0) {
                processor->action_data_1 |= 4;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
    }
    if (sys->player_1 != NULL) {
        if ((processor->action_data_1 & 4) != 0) {
            packet->movement_instruction_parameter =
                NuVecXZDist(&packet->terrain_origin, &sys->player_1->ai->last_path_position, &difference);
        }
        AIPACKET *target = sys->player_1->ai;
        NUVEC *position = (processor->action_data_1 & 2) != 0 ? &target->terrain_origin : &target->last_path_position;
        AIMoveInstruction(packet, position, target->mover_height, &target->path_info, AIPACKET_MOVEMENT_CIRCLE,
                          packet->movement_instruction_parameter);
        if ((processor->action_data_1 & 1) != 0) {
            packet->movement_look_target = position;
        }
    }
    return 0;
}

i32 Action_Circle(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_count,
                  i32 first_time, f32) {
    NUVEC difference;
    if (packet == NULL || sys == NULL || sys->player_1 == NULL) {
        return 1;
    }
    if (first_time != 0) {
        for (i32 i = 0; i < param_count; i++) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[i], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value;
            if (NuStrICmp(params[i], "ANTICLOCKWISE") == 0) {
                packet->circle_clockwise = 0;
            } else if (NuStrICmp(params[i], "CLOCKWISE") == 0) {
                packet->circle_clockwise = 1;
            } else if (NuStrICmp(params[i], "REVERSE") == 0) {
                packet->circle_clockwise = !packet->circle_clockwise;
            } else if (NuStrICmp(params[i], "facing") == 0) {
                processor->action_data_1 |= 1;
            } else if (NuStrICmp(params[i], "currentdist") == 0) {
                processor->action_data_1 |= 4;
            } else if ((value = NuStrIStr(params[i], "locator=")) != NULL) {
                processor->action_data_3 = AIPathFindLocator(sys, value + 8);
                if (processor->action_data_3 != NULL) {
                    processor->action_data_1 |= 0x10;
                }
            } else if (NuStrIStr(params[i], "current_position") != NULL) {
                processor->action_data_1 |= 8;
                processor->action_pos = packet->terrain_origin;
            } else if (NuStrICmp(params[i], "origin") == 0) {
                if (packet->field_0x134 != 0xff) {
                    processor->action_data_1 |= 0x20;
                }
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[i]);
            }
        }
        if ((processor->action_data_1 & 0x38) == 0) {
            processor->action_data_1 |= 8;
            processor->action_pos = packet->terrain_origin;
        }
    }
    NUVEC *position = NULL;
    AIPATHINFO *path = NULL;
    if ((processor->action_data_1 & 8) != 0) {
        path = &packet->path_info;
        position = &processor->action_pos;
    } else if ((processor->action_data_1 & 0x10) != 0) {
        AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
        path = &locator->path_info;
        position = &locator->position;
    } else if ((processor->action_data_1 & 0x20) != 0) {
        AICREATURE *creature = &sys->creatures[packet->field_0x134];
        path = &creature->path_info;
        position = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
        if (position == NULL) {
            position = &creature->pos;
        }
    }
    if (position != NULL && path != NULL) {
        if ((processor->action_data_1 & 4) != 0) {
            packet->movement_instruction_parameter = NuVecXZDist(&packet->terrain_origin, position, &difference);
        }
        AIMoveInstruction(packet, position, 0.0f, path, AIPACKET_MOVEMENT_CIRCLE,
                          packet->movement_instruction_parameter);
        if ((processor->action_data_1 & 1) != 0) {
            packet->movement_look_target = position;
        }
    }
    return 0;
}

extern "C" {
    void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter) {
        AIPACKET *packet;
        AIPACKET *target_packet;

        NUVEC *destination;
        i32 movement_mode;
        if ((flags & 1) != 0) {
            packet = object->ai;
            target_packet = target->ai;
            bool use_target_path_position = false;
            AIPATH *path = packet->path_info.path;
            if (path != NULL && path == target_packet->path_info.path) {
                if (((target_packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) != 0 ||
                     packet->path_info.connection != target_packet->path_info.connection) &&
                    packet->path_info.connection != NULL && target_packet->path_info.connection != NULL &&
                    (packet->runtime_flags & AIPACKET_RUNTIME_ROUTE_SELECTED) == 0) {
                    const u8 object_direction = packet->path_info.connection->direction_a;
                    const u8 target_direction = target_packet->path_info.connection->direction_a;
                    if (path->route_matrix[object_direction][target_direction] != 0xff ||
                        object_direction == target_direction) {
                        use_target_path_position = true;
                    }
                }
            }

            if (use_target_path_position) {
                destination = &target_packet->last_path_position;
                movement_mode = AIPACKET_MOVEMENT_TO_DESTINATION;
            } else {
                destination = &target_packet->terrain_origin;
                movement_mode = AIPACKET_MOVEMENT_DIRECT;
            }
        } else {
            packet = object->ai;
            target_packet = target->ai;
            destination = &target_packet->last_path_position;
            movement_mode = AIPACKET_MOVEMENT_TO_DESTINATION;
        }

        const f32 stopping_distance = (flags & 2) != 0 ? 0.0f : target_packet->mover_height;
        AIMoveInstruction(packet, destination, stopping_distance, &target_packet->path_info, movement_mode,
                          movement_parameter);
    }

}

__used__ static i32 Action_FollowOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32) {
    if (packet == NULL) {
        return 1;
    }

    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[index], "ignore_radius") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(params[index], "can_go_off_path") == 0) {
                processor->action_data_1 |= 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }

    APIOBJECT *target = static_cast<APIOBJECT *>(packet->opponent);
    if (target != NULL && target->ai != NULL) {
        FollowAPIObject(&packet->owner->apiobj, target, processor->action_data_1,
                        packet->movement_instruction_parameter);
    }
    return 0;
}

i32 Action_FollowPlayer(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params, i32 param_count,
                        i32 first_time, float) {
    if (packet == NULL) {
        return 1;
    }

    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[index], "ignore_radius") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(params[index], "can_go_off_path") == 0) {
                processor->action_data_1 |= 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }

    if (sys->player_1 != NULL && sys->player_1->ai != NULL) {
        FollowAPIObject(&packet->owner->apiobj, sys->player_1, processor->action_data_1,
                        packet->movement_instruction_parameter);
    }
    return 0;
}

i32 AIMoveCanReachPath(AISYS_s *system, AIPATH_s *path, AIPATH_s *destination) {
    AIPATHNODELINK *links = path->special_routes;
    for (i32 link_index = 0; link_index < path->special_route_count; ++link_index) {
        AIPATHSPECIALROUTE *route = &system->path_sys->special_routes[links[link_index].special_route_index];
        // The original increments the outer cursor here and recursively tests
        // the same path pair. Preserve those accesses, including path slot zero.
        for (i32 path_index = 0; path_index < route->path_count; ++link_index) {
            if (route->paths[path_index] == destination) {
                return 1;
            }
            if (AIMoveCanReachPath(system, path, destination) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void AIMoveToDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    i32 direction = 0;
    NUVEC saved_destination = packet->fallback_destination;
    f32 saved_stopping_distance = packet->fallback_stopping_distance;
    AIPATHINFO saved_path_info = packet->fallback_path_info;
    f32 saved_parameter = packet->movement_parameter;
    AIPATH *path = packet->path_info.path;
    i32 diverted = 0;
    if (path != packet->fallback_path_info.path || packet->fallback_path_info.connection == NULL) {
        AIMoveFindDivertNode(path, packet, &packet->fallback_destination);
        if (packet->divert_node_index >= packet->path_info.path->node_count ||
            packet->path_info.path->nodes[packet->divert_node_index].connection_count == 0) {
            packet->movement_destination = packet->owner->apiobj.position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        AIPATHNODE *node = &packet->path_info.path->nodes[packet->divert_node_index];
        packet->fallback_destination = node->position;
        packet->fallback_stopping_distance = 0.0f;
        packet->movement_parameter = NuFmax(node->radius - 1.0f, 1.0f);
        memset(&packet->fallback_path_info, 0, sizeof(packet->fallback_path_info));
        packet->fallback_path_info.path = packet->path_info.path;
        path = packet->path_info.path;
        packet->fallback_path_info.connection = node->connections[0];
        packet->fallback_path_info.direction = node->connections[0]->node_indices[0] == packet->divert_node_index;
        packet->fallback_path_info.dist =
            node->connections[0]->node_indices[0] == packet->divert_node_index ? 0.0f : 1.0f;
        diverted = 1;
    }
    AIPATHCNX *destination_connection = packet->fallback_path_info.connection;
    AIPATHCNX *connection = packet->path_info.connection;
    NUVEC difference;
    f32 distance_squared = NuVecDistSqr(&packet->fallback_destination, &object->position, &difference);
    if ((object->supporting_platform_id == -1 ||
         (path->nodes[connection->node_indices[0]].has_special != 0 &&
          path->nodes[connection->node_indices[1]].has_special != 0)) &&
        packet->movement_parameter != 0.0f && difference.y < 0.5f && difference.y > -0.5f &&
        distance_squared < packet->movement_parameter * packet->movement_parameter) {
        if (diverted == 0) {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
        if ((packet->navigation_flags & 4) != 0) {
            packet->fallback_destination = saved_destination;
            packet->movement_parameter = saved_parameter;
            packet->fallback_path_info = saved_path_info;
            packet->fallback_stopping_distance = saved_stopping_distance;
            AIMoveChooseExitNodePath(&system->path_sys, packet);
            AIMoveAdjustDestinationPath(packet);
            path = packet->path_info.path;
            connection = packet->path_info.connection;
            destination_connection = packet->fallback_path_info.connection;
        }
    }
    if (connection == NULL || path == NULL || destination_connection == NULL) {
        packet->movement_destination = packet->owner->apiobj.position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }
    if (packet->path_info.connection != packet->fallback_path_info.connection) {
        if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path,
                             packet->fallback_path_info.connection, checks, packet->path_info.connection,
                             packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) != 0) {
            AIPATHCNX *target = packet->fallback_path_info.connection;
            AIPATHCNX *current = packet->path_info.connection;
            direction = target->node_indices[0] != current->node_indices[0] &&
                        target->node_indices[0] != current->node_indices[1];
            AISysCharacterSetPathCnx(packet, &object->position, target, direction);
        }
        if (packet->path_info.connection != packet->fallback_path_info.connection) {
            direction = packet->path_info.direction;
            i32 goal_index = destination_connection->node_indices[0];
            f32 current_distance = packet->path_info.dist;
            if (current_distance > 1.0f) {
                current_distance = 1.0f;
            } else if (current_distance < 0.0f) {
                current_distance = 0.0f;
            }
            f32 endpoint_distances[2] = {fabsf(current_distance * connection->distance),
                                         fabsf((1.0f - current_distance) * connection->distance)};
            f32 destination_distances[2] = {
                fabsf(packet->fallback_path_info.dist * destination_connection->distance),
                fabsf((1.0f - packet->fallback_path_info.dist) * destination_connection->distance)};
            if (packet->current_route != 0xff) {
                packet->runtime_flags |= 0x10;
            }
            u32 attempted_routes = 0;
            f32 best_distance = FLT_MAX;
            for (;;) {
                if (packet->current_route != 0xff &&
                    ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) == 0) {
                    for (i32 end = 0; end < 2; ++end) {
                        AIPATHNODE *node = &path->nodes[destination_connection->node_indices[end]];
                        if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >>
                                 packet->current_route &
                             1) == 0) {
                            continue;
                        }
                        destination_distances[end] = FLT_MAX;
                    }
                }
                if ((connection->traversal_flags[packet->path_info.direction] & 0x40000000) == 0) {
                    {
                        f32 candidate = destination_distances[0] != FLT_MAX &&
                                                endpoint_distances[packet->path_info.direction == 0] != FLT_MAX
                                            ? AIPathNodeDistanceToPathNode(
                                                  packet->path_info.path,
                                                  connection->node_indices[packet->path_info.direction == 0],
                                                  destination_connection->node_indices[0], packet->current_route, 0)
                                            : FLT_MAX;
                        if (candidate != FLT_MAX) {
                            candidate = candidate + endpoint_distances[packet->path_info.direction == 0] +
                                        destination_distances[0];
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[0];
                            direction = packet->path_info.direction;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                    {
                        f32 candidate = destination_distances[1] != FLT_MAX &&
                                                endpoint_distances[packet->path_info.direction == 0] != FLT_MAX
                                            ? AIPathNodeDistanceToPathNode(
                                                  packet->path_info.path,
                                                  connection->node_indices[packet->path_info.direction == 0],
                                                  destination_connection->node_indices[1], packet->current_route, 0)
                                            : FLT_MAX;
                        if (candidate != FLT_MAX) {
                            candidate = candidate + endpoint_distances[packet->path_info.direction == 0] +
                                        destination_distances[1];
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[1];
                            direction = packet->path_info.direction;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                }
                if ((connection->traversal_flags[packet->path_info.direction == 0] & 0x40000000) == 0) {
                    {
                        f32 candidate =
                            destination_distances[0] != FLT_MAX &&
                                    endpoint_distances[packet->path_info.direction] != FLT_MAX
                                ? AIPathNodeDistanceToPathNode(
                                      packet->path_info.path, connection->node_indices[packet->path_info.direction],
                                      destination_connection->node_indices[0], packet->current_route, 0)
                                : FLT_MAX;
                        if (candidate != FLT_MAX) {
                            candidate =
                                candidate + destination_distances[0] + endpoint_distances[packet->path_info.direction];
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[0];
                            direction = packet->path_info.direction == 0;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                    {
                        f32 candidate =
                            destination_distances[1] != FLT_MAX &&
                                    endpoint_distances[packet->path_info.direction] != FLT_MAX
                                ? AIPathNodeDistanceToPathNode(
                                      packet->path_info.path, connection->node_indices[packet->path_info.direction],
                                      destination_connection->node_indices[1], packet->current_route, 0)
                                : FLT_MAX;
                        if (candidate != FLT_MAX) {
                            candidate =
                                candidate + destination_distances[1] + endpoint_distances[packet->path_info.direction];
                        }
                        if (candidate < best_distance) {
                            best_distance = candidate;
                            goal_index = destination_connection->node_indices[1];
                            direction = packet->path_info.direction == 0;
                            packet->runtime_flags &= static_cast<u8>(~0x10);
                        }
                    }
                }
                if ((packet->runtime_flags & 0x10) == 0) {
                    break;
                }
                if (packet->current_route != 0xff) {
                    attempted_routes |= 1u << (packet->current_route & 31);
                }
                AISysFindRoute(packet);
                if (((attempted_routes >> (packet->current_route & 31)) & 1) == 0) {
                    destination_distances[0] =
                        fabsf(packet->fallback_path_info.dist * destination_connection->distance);
                    destination_distances[1] =
                        fabsf((1.0f - packet->fallback_path_info.dist) * destination_connection->distance);
                    continue;
                }
                return;
            }
            packet->goal_path_node = &path->nodes[goal_index];
            AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
            AIPATHCNX *next = GetNextConnection(packet, &direction);
            if (next != NULL) {
                while (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, next, checks,
                                        packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                                        object->collision_radius, 0) != 0) {
                    if (connection == destination_connection) {
                        break;
                    }
                    AISysCharacterSetPathCnx(packet, &object->position, next, direction);
                    if (packet->path_info.connection != next || packet->path_info.direction != direction) {
                        connection = next;
                        break;
                    }
                    connection = next;
                    next = GetNextConnection(packet, &direction);
                    if (next != connection && next != NULL) {
                        continue;
                    }
                    break;
                }
            } else {
                u32 flags = destination_connection->traversal_flags[0] | destination_connection->traversal_flags[1];
                if ((flags & 0xd8000000) == 0 &&
                    ((flags & 0xdfffffff) == 0 || (packet->capabilities & (flags & 0xdfffffff)) != 0) &&
                    WithinConnection(system, &packet->terrain_origin, packet->path_info.path, destination_connection,
                                     checks, packet->path_info.connection, packet->current_route, object->field_0x289,
                                     NULL, object->collision_radius, 0) != 0) {
                    AISysCharacterSetPathCnx(packet, &object->position, destination_connection, 0);
                }
            }
        }
    }
    if (PreparingForSpecialMoveFn != NULL) {
        if (connection != destination_connection) {
            packet->movement_event_flags |= 0x20;
        }
        if (PreparingForSpecialMoveFn(packet, object, checks) != 0) {
            packet->runtime_flags |= 8;
            packet->movement_event_flags |= 0x40;
            return;
        }
    }
    AIPATHCNX *current = packet->path_info.connection;
    u32 flags = current->traversal_flags[packet->path_info.direction];
    if ((flags & 0xd8000000) != 0) {
        packet->movement_destination = path->nodes[current->node_indices[packet->path_info.direction]].position;
        packet->runtime_flags |= 0x40;
        packet->movement_stopping_distance = 0.0f;
        packet->frame_state = flags;
        return;
    }
    if (current == destination_connection &&
        (packet->current_route == 0xff ||
         ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0)) {
        if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) == 0 || packet->movement_parameter == 0.0f) {
            packet->movement_stopping_distance = packet->fallback_stopping_distance;
            packet->movement_destination = packet->fallback_destination;
            return;
        }
        if (distance_squared < packet->movement_parameter * packet->movement_parameter) {
            if (diverted == 0) {
                packet->movement_stopping_distance = 0.0f;
                packet->movement_destination = object->position;
                return;
            }
            if ((packet->navigation_flags & 4) != 0) {
                packet->movement_parameter = saved_parameter;
                packet->fallback_destination = saved_destination;
                packet->fallback_path_info = saved_path_info;
                packet->fallback_stopping_distance = saved_stopping_distance;
                AIMoveChooseExitNodePath(&system->path_sys, packet);
                AIMoveAdjustDestinationPath(packet);
            }
        }
        NuVecScale(&difference, &difference, packet->movement_parameter / NuFsqrt(distance_squared));
        packet->movement_destination.x = packet->fallback_destination.x - difference.x;
        packet->movement_destination.y = packet->fallback_destination.y;
        packet->movement_destination.z = packet->fallback_destination.z - difference.z;
        packet->movement_stopping_distance =
            packet->fallback_stopping_distance < packet->movement_parameter ? 0.0f : packet->fallback_stopping_distance;
        return;
    }
    AIPATHNODE *shared = NULL;
    if (current->node_indices[0] == destination_connection->node_indices[0]) {
        shared = &path->nodes[destination_connection->node_indices[0]];
        direction = 0;
    } else if (current->node_indices[0] == destination_connection->node_indices[1]) {
        shared = &path->nodes[destination_connection->node_indices[1]];
        direction = 1;
    } else if (current->node_indices[1] == destination_connection->node_indices[0]) {
        shared = &path->nodes[destination_connection->node_indices[0]];
        direction = 0;
    } else if (current->node_indices[1] == destination_connection->node_indices[1]) {
        shared = &path->nodes[destination_connection->node_indices[1]];
        direction = 1;
    }
    AIPATHCNX *next;
    if (shared != NULL && (packet->current_route == 0xff ||
                           ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) != 0 ||
                           ((static_cast<u64>(shared->route_boundary_mask) >> packet->current_route) & 1) != 0)) {
        next = destination_connection;
    } else {
        next = GetNextConnection(packet, &direction);
    }
    if (next != NULL) {
        flags = next->traversal_flags[direction];
        if ((flags & 0xdfffffff) != 0) {
            if ((flags & 0xd8000000) != 0) {
                packet->movement_destination = object->position;
                packet->runtime_flags |= 0x40;
                packet->movement_stopping_distance = 0.0f;
                packet->frame_state = flags;
                return;
            }
            if ((packet->capabilities & flags) == 0) {
                packet->frame_state = next->traversal_flags[direction];
                packet->runtime_flags |= 0x60;
                next = NULL;
            } else {
                packet->navigation_flags |= 1;
            }
        }
        if (next != NULL) {
            NUVEC target;
            if (next == destination_connection) {
                target = packet->fallback_destination;
            } else {
                target = path->nodes[next->node_indices[direction]].position;
                packet->runtime_flags |= 8;
            }
            if (WithinConnection(system, &target, packet->path_info.path, current, checks, packet->path_info.connection,
                                 packet->current_route, object->field_0x289, NULL, object->collision_radius, 0) == 0) {
                if (CalculateIntersection(system, packet, object, current, next) != 0) {
                    bool use_left = false;
                    NUVEC offset;
                    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
                        NuVecSub(&offset, &packet->left_diversion, &object->position);
                        i32 angle = NuAtan2D(offset.x, offset.z);
                        NuVecSub(&offset, &target, &object->position);
                        if (NuAngSub(NuAtan2D(offset.x, offset.z), angle) < 0) {
                            target = packet->left_diversion;
                            packet->runtime_flags |= 8;
                            use_left = true;
                        }
                    }
                    if (!use_left && (packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
                        NuVecSub(&offset, &packet->right_diversion, &object->position);
                        i32 angle = NuAtan2D(offset.x, offset.z);
                        NuVecSub(&offset, &target, &object->position);
                        if (NuAngSub(NuAtan2D(offset.x, offset.z), angle) > 0) {
                            target = packet->right_diversion;
                            packet->runtime_flags |= 8;
                        }
                    }
                } else {
                    target = path->nodes[next->node_indices[direction]].position;
                }
            }
            packet->movement_stopping_distance = 0.0f;
            packet->movement_destination = target;
            return;
        }
    }
    packet->movement_destination =
        path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]].position;
    packet->movement_stopping_distance = 0.0f;
    NUVEC offset;
    if (NuVecDistSqr(&packet->movement_destination, &object->position, &offset) < 1.0f) {
        packet->movement_destination = object->position;
    }
    packet->runtime_flags |= 0x40;
}

void AIMoveToDestinationAvoidingCamera(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    const NUVEC destination = packet->fallback_destination;
    AIMoveToDestination(system, packet, object, checks);
    packet->fallback_destination = destination;
}

void AIRetreatFromDestination2(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    NUVEC difference;
    f32 distance = NuVecXZDist(&object->position, &packet->fallback_destination, &difference);
    if (distance != 0.0f) {
        NuVecScale(&difference, &difference, packet->movement_parameter / distance);
    } else {
        difference.x = packet->movement_parameter;
        difference.y = 0.0f;
        difference.z = 0.0f;
    }
    NuVecAdd(&packet->movement_destination, &packet->fallback_destination, &difference);
    packet->movement_stopping_distance = 0.0f;
    distance = NuVecXZDist(&packet->movement_destination, &object->position, &difference);
    if (distance > ai_moveradius) {
        NuVecScale(&difference, &difference, ai_moveradius / distance);
        NuVecAdd(&packet->movement_destination, &object->position, &difference);
    }

    NUVEC position = object->position;
    NUVEC path_position = packet->last_path_position;
    AIPATHINFO path_info = packet->path_info;
    object->position = packet->movement_destination;
    AISysGetCharacterPathPos(system, object, packet, 0, 1);
    packet->movement_destination = packet->last_path_position;
    packet->path_info = path_info;
    object->position = position;
    packet->last_path_position = path_position;
}

void AIRetreatFromDestination(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    NUVEC difference;
    if (new_retreat != 0) {
        AIRetreatFromDestination2(system, packet, object, checks);
        return;
    }
    if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) != 0) {
        f32 distance = NuVecXZDist(&object->position, &packet->fallback_destination, &difference);
        if (distance != 0.0f) {
            NuVecScale(&difference, &difference, packet->movement_parameter / distance);
        } else {
            difference.x = packet->movement_parameter;
            difference.y = 0.0f;
            difference.z = 0.0f;
        }
        NuVecAdd(&packet->movement_destination, &object->position, &difference);
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    AIPATHCNX *connection = packet->path_info.connection;
    if (connection == NULL || packet->fallback_path_info.connection == NULL ||
        packet->path_info.path != packet->fallback_path_info.path) {
        return;
    }
    AIPATH *path = packet->path_info.path;
    AIPATHCNX *destination_connection = packet->fallback_path_info.connection;
    if (connection != destination_connection) {
        f32 length = connection->distance;
        f32 parameter = packet->path_info.dist;
        if (parameter > 1.0f) {
            parameter = 1.0f;
        } else if (parameter < 0.0f) {
            parameter = 0.0f;
        }
        f32 remaining_parameter = 1.0f - parameter;
        f32 destination_parameter = packet->fallback_path_info.dist;
        f32 destination_a = fabsf(destination_connection->distance * destination_parameter);
        f32 destination_b = fabsf((1.0f - destination_parameter) * destination_connection->distance);
        if (packet->current_route != 0xff &&
            ((static_cast<u64>(destination_connection->route_mask) >> packet->current_route) & 1) == 0) {
            AIPATHNODE *node = &path->nodes[destination_connection->node_indices[0]];
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >> packet->current_route) &
                1) {
                destination_a = FLT_MAX;
            }
            node = &path->nodes[destination_connection->node_indices[1]];
            if ((static_cast<u64>(node->route_membership_mask & ~node->route_boundary_mask) >> packet->current_route) &
                1) {
                destination_b = FLT_MAX;
            }
        }

        u8 direction = 0;
        f32 best_distance = 0.0f;
        if ((connection->traversal_flags[1] & 0x40000000u) == 0) {
            f32 partial_distance = fabsf(parameter * length);
            f32 candidate =
                AIPathNodeDistanceToPathNode(path, connection->node_indices[0], destination_connection->node_indices[0],
                                             packet->current_route, 0) +
                partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 1;
            }
            candidate = AIPathNodeDistanceToPathNode(
                            packet->path_info.path, packet->path_info.connection->node_indices[0],
                            packet->fallback_path_info.connection->node_indices[1], packet->current_route, 0) +
                        partial_distance + destination_b;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 1;
            }
            connection = packet->path_info.connection;
        }
        if ((connection->traversal_flags[0] & 0x40000000u) == 0) {
            f32 partial_distance = fabsf(length * remaining_parameter);
            f32 candidate = AIPathNodeDistanceToPathNode(packet->path_info.path, connection->node_indices[1],
                                                         packet->fallback_path_info.connection->node_indices[0],
                                                         packet->current_route, 0) +
                            partial_distance + destination_a;
            if (candidate > best_distance) {
                best_distance = candidate;
                direction = 0;
            }
            candidate = AIPathNodeDistanceToPathNode(
                            packet->path_info.path, packet->path_info.connection->node_indices[1],
                            packet->fallback_path_info.connection->node_indices[1], packet->current_route, 0) +
                        partial_distance + destination_b;
            if (candidate > best_distance) {
                direction = 0;
            }
            connection = packet->path_info.connection;
        }
        AISysCharacterSetPathCnx(packet, &object->position, connection, direction);
    } else {
        AISysCharacterSetPathCnx(packet, &object->position, connection,
                                 packet->fallback_path_info.dist > packet->path_info.dist);
    }

    connection = packet->path_info.connection;
    u8 end_node = connection->node_indices[packet->path_info.direction == 0];
    packet->goal_path_node = &path->nodes[end_node];
    if (packet->movement_target != NULL && packet->movement_target->node_indices[0] != end_node &&
        packet->movement_target->node_indices[1] != end_node) {
        packet->movement_target = NULL;
    }
    if (packet->movement_target == NULL) {
        AIPATHNODE *node = &packet->path_info.path->nodes[end_node];
        i32 start = NuRandInt() % node->connection_count;
        connection = packet->path_info.connection;
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[(index + start) % node->connection_count];
            AIPATHCNX *target = packet->movement_target;
            if (target != connection &&
                (packet->current_route == 0xff ||
                 ((static_cast<u64>(target->route_mask) >> packet->current_route) & 1) != 0 ||
                 ((static_cast<u64>(node->route_boundary_mask) >> packet->current_route) & 1) != 0) &&
                target->traversal_flags[target->node_indices[0] != end_node] == 0) {
                break;
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        u8 old_direction = packet->path_info.direction;
        end_node = connection->node_indices[old_direction];
        AIPATHNODE *node = &path->nodes[end_node];
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[index];
            AIPATHCNX *target = packet->movement_target;
            if (target != connection && target->traversal_flags[target->node_indices[0] != end_node] == 0) {
                AISysCharacterSetPathCnx(packet, &object->position, connection, old_direction == 0);
                connection = packet->path_info.connection;
                end_node = connection->node_indices[packet->path_info.direction == 0];
                packet->goal_path_node = &path->nodes[end_node];
                break;
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }
    i32 endpoint = end_node;
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target, checks,
                         connection, packet->current_route, object->field_0x289, NULL, object->collision_radius,
                         0) != 0) {
        if (packet->movement_target->node_indices[0] == endpoint) {
            AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target, 0);
        } else {
            AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target, 1);
        }
        packet->goal_path_node =
            &path->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_target = NULL;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->movement_destination =
        packet->path_info.path->nodes[packet->movement_target->node_indices[packet->movement_target_direction == 0]]
            .position;
    packet->movement_stopping_distance = 0.0f;
    if (CalculateIntersection(system, packet, object, packet->path_info.connection, packet->movement_target) == 0) {
        return;
    }
    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
        NuVecSub(&difference, &packet->left_diversion, &object->position);
        i32 angle = NuAtan2D(difference.x, difference.z);
        NuVecSub(&difference, &packet->movement_destination, &object->position);
        if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) < 0) {
            packet->movement_destination = packet->left_diversion;
            return;
        }
    }
    if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
        NuVecSub(&difference, &packet->right_diversion, &object->position);
        i32 angle = NuAtan2D(difference.x, difference.z);
        NuVecSub(&difference, &packet->movement_destination, &object->position);
        if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) > 0) {
            packet->movement_destination = packet->right_diversion;
        }
    }
}

void AICircle(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    f32 radius = MAX(packet->fallback_stopping_distance, packet->movement_parameter);
    NUVEC difference;
    NuVecSub(&difference, &object->position, &packet->fallback_destination);
    i32 angle = NuAtan2D(difference.x, difference.z);
    difference.x = 0.0f;
    difference.z = radius;
    NuVecRotateY(&difference, &difference, angle);
    NUVEC circle_position;
    NuVecAdd(&circle_position, &packet->fallback_destination, &difference);
    if (packet->circle_clockwise != 0) {
        angle = NuAngSub(angle, NUANG_90DEG);
    } else {
        angle = NuAngAdd(angle, NUANG_90DEG);
    }
    difference.x = 0.0f;
    difference.y = 0.0f;
    difference.z = 0.5f;
    NuVecRotateY(&difference, &difference, angle);
    NuVecAdd(&packet->movement_destination, &circle_position, &difference);
    packet->circle_active = 1;
    packet->movement_stopping_distance = 0.0f;
}

void AIWander(AISYS_s *system, AIPACKET_s *packet, APIOBJECT_s *object, i32 checks) {
    NUVEC difference;
    if (packet->movement_target != NULL) {
        i32 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
        if (packet->movement_target->node_indices[0] != node_index &&
            packet->movement_target->node_indices[1] != node_index) {
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        u8 node_index = packet->path_info.connection->node_indices[packet->path_info.direction == 0];
        AIPATHNODE *node = &packet->path_info.path->nodes[node_index];
        i32 start = NuRandInt() % node->connection_count;
        auto node_route_boundary_mask = node->route_boundary_mask;
        for (i32 index = 0; index < node->connection_count; ++index) {
            packet->movement_target = node->connections[(index + start) % node->connection_count];
            AIPATHCNX *target = packet->movement_target;
            if (target != packet->path_info.connection) {
                if (packet->current_route == 0xff ||
                    ((static_cast<u64>(target->route_mask) >> packet->current_route) & 1) != 0 ||
                    ((static_cast<u64>(node_route_boundary_mask) >> packet->current_route) & 1) != 0) {
                    packet->movement_target_direction = node_index != target->node_indices[0];
                    if (target->traversal_flags[packet->movement_target_direction] == 0) {
                        break;
                    }
                }
            }
            packet->movement_target = NULL;
        }
    }
    if (packet->movement_target == NULL) {
        AIPATHNODE *node = &packet->path_info.path
                                ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        f32 distance = NuVecXZDistSqr(&object->position, &node->position, &difference);
        if (distance < node->radius_squared) {
            AISysCharacterSetPathCnx(packet, &object->position, packet->path_info.connection,
                                     packet->path_info.direction == 0);
            node = &packet->path_info.path
                        ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
            packet->goal_path_node = node;
        }
        packet->movement_destination = node->position;
        packet->movement_stopping_distance = 0.0f;
        return;
    }

    packet->runtime_flags |= AIPACKET_RUNTIME_SPECIAL_MOVE;
    if (WithinConnection(system, &packet->terrain_origin, packet->path_info.path, packet->movement_target, checks,
                         packet->path_info.connection, packet->current_route, object->field_0x289, NULL,
                         object->collision_radius, 0) == 0) {
        packet->movement_destination =
            packet->path_info.path->nodes[packet->movement_target->node_indices[packet->movement_target_direction == 0]]
                .position;
        packet->movement_stopping_distance = 0.0f;
        if (CalculateIntersection(system, packet, object, packet->path_info.connection, packet->movement_target) != 0) {
            if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_LEFT) != 0) {
                NuVecSub(&difference, &packet->left_diversion, &object->position);
                i32 angle = NuAtan2D(difference.x, difference.z);
                NuVecSub(&difference, &packet->movement_destination, &object->position);
                if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) < 0) {
                    packet->movement_destination = packet->left_diversion;
                    return;
                }
            }
            if ((packet->movement_flags & AIPACKET_MOVEMENT_DIVERSION_RIGHT) != 0) {
                NuVecSub(&difference, &packet->right_diversion, &object->position);
                i32 angle = NuAtan2D(difference.x, difference.z);
                NuVecSub(&difference, &packet->movement_destination, &object->position);
                if (NuAngSub(NuAtan2D(difference.x, difference.z), angle) > 0) {
                    packet->movement_destination = packet->right_diversion;
                }
            }
        }
    } else {
        AISysCharacterSetPathCnx(packet, &object->position, packet->movement_target, packet->movement_target_direction);
        packet->goal_path_node =
            &packet->path_info.path
                 ->nodes[packet->path_info.connection->node_indices[packet->path_info.direction == 0]];
        packet->movement_destination = packet->goal_path_node->position;
        packet->movement_stopping_distance = 0.0f;
        packet->movement_target = NULL;
    }
}

void AIMoveDirectlyToDestination(AISYS_s *, AIPACKET_s *packet, APIOBJECT_s *object, i32) {
    if (packet->movement_parameter > 0.0f) {
        NUVEC delta;
        const f32 distance_squared = NuVecDistSqr(&packet->fallback_destination, &object->position, &delta);
        if (packet->movement_parameter * packet->movement_parameter > distance_squared) {
            packet->movement_destination = object->position;
            packet->movement_stopping_distance = 0.0f;
            return;
        }
    }
    packet->movement_destination = packet->fallback_destination;
    packet->movement_stopping_distance = 0.0f;
}

extern "C" {
    void AISysCharacterMovement(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks) {
        packet->movement_event_flags &= static_cast<u8>(~AIPACKET_PATH_CONNECTION_CHANGED);
        if (MidSpecialMoveFn == NULL || MidSpecialMoveFn(system, packet, object) == 0) {
            if (packet->path_info.path != NULL && packet->path_info.connection != NULL) {
                packet->movement_destination = object->position;
                packet->movement_stopping_distance = 0.0f;
                switch (packet->movement_flags & AIPACKET_MOVEMENT_MODE_MASK) {
                    case AIPACKET_MOVEMENT_TO_DESTINATION:
                        AIMoveToDestination(system, packet, object, checks);
                        break;
                    case AIPACKET_MOVEMENT_RETREAT:
                        AIRetreatFromDestination(system, packet, object, checks);
                        break;
                    case AIPACKET_MOVEMENT_CIRCLE:
                        AICircle(system, packet, object, checks);
                        break;
                    case AIPACKET_MOVEMENT_WANDER:
                        AIWander(system, packet, object, checks);
                        break;
                    case AIPACKET_MOVEMENT_AVOIDING_CAMERA:
                        AIMoveToDestinationAvoidingCamera(system, packet, object, checks);
                        break;
                    case AIPACKET_MOVEMENT_DIRECT:
                        AIMoveDirectlyToDestination(system, packet, object, checks);
                        break;
                }
            } else {
                packet->movement_destination = object->position;
                packet->movement_stopping_distance = 0.0f;
                const u8 mode = packet->movement_flags & AIPACKET_MOVEMENT_MODE_MASK;
                if (mode == AIPACKET_MOVEMENT_DIRECT || mode == AIPACKET_MOVEMENT_TO_DESTINATION ||
                    mode == AIPACKET_MOVEMENT_AVOIDING_CAMERA) {
                    packet->movement_stopping_distance = packet->fallback_stopping_distance;
                    packet->movement_destination = packet->fallback_destination;
                }
            }

            packet->movement_flags &= static_cast<u8>(~AIPACKET_MOVEMENT_MODE_MASK);
        } else {
            packet->field_0x1e6 |= AIPACKET_RUNTIME_SPECIAL_MOVE;
            packet->field_0x1e7 |= AIPACKET_MOVEMENT_SPECIAL_HANDLED;
        }
    }

    void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai) {
        AISCRIPTPROCESS *primary_processor = &packet->script_process;
        AISCRIPTPROCESS *processor = packet->alternate_script_process;
        if (processor == NULL) {
            processor = primary_processor;
        }

        if ((packet->field_0x1e6 & AIPACKET_RUNTIME_INITIALISED) == 0) {
            AISTATE *saved_state = primary_processor->state;
            if (AIScriptSetStateByName(primary_processor, "Initialise") != 0) {
                AIScriptProcess(system, object, packet, primary_processor, 0.0f);
                AIScriptSetState(primary_processor, saved_state);
            }
            packet->field_0x1e6 |= AIPACKET_RUNTIME_INITIALISED;
        }

        if (process_ai != 0) {
            AISysUpdateCharacterPathPos(system, object, packet, checks, elapsed);

            const u8 runtime_flags = packet->field_0x1e6;
            packet->movement_flags &= static_cast<u8>(~AIPACKET_MOVEMENT_OPTION_TRANSIENT);
            packet->circle_active = 0;
            packet->frame_flags = 0;
            packet->field_0x1e6 = runtime_flags & static_cast<u8>(~AIPACKET_RUNTIME_SPECIAL_MOVE);

            if ((object->field_0x1f8 & APIOBJECT_FLAG_AI_PLAYER_MASK) == APIOBJECT_FLAG_PLAYER_ACTIVE) {
                packet->path_connection_state = 0;
                packet->movement_destination = object->position;
                packet->field_0x180 = NULL;
                packet->field_0x1e6 = runtime_flags & 7;
                packet->field_0x1e7 &= 0x3f;
                packet->navigation_flags &= static_cast<u8>(~AIPACKET_NAVIGATION_FLAG_TRANSIENT);
            } else {
                if (AIRespawnOnPathFn != NULL && AIRespawnOnPathFn(object) != 0) {
                    return;
                }

                NuSpecialClear(&object->field_0x208);
                object->field_0x1fa &= static_cast<u8>(~4u);
                if (system != NULL && processor->script != NULL) {
                    AIScriptProcess(system, object, packet, processor, elapsed);
                }

                packet->navigation_flags &= static_cast<u8>(~AIPACKET_NAVIGATION_FLAG_TRANSIENT);
                packet->frame_state = 0;
                packet->field_0x1e6 &= 0x0f;

                const u8 source_flags = packet->field_0x1e7;
                packet->field_0x1e7 = source_flags & 0x3f;
                if ((source_flags & AIPACKET_MOVEMENT_SOURCE_ACTIVE) != 0) {
                    const u8 source = (source_flags & AIPACKET_MOVEMENT_SOURCE_MASK) >> 2;
                    if (source == 1) {
                        if (packet->field_0x134 != 0xff) {
                            AICREATURE *creature = &system->creatures[packet->field_0x134];
                            AIMoveInstruction(packet, &creature->pos, 0.0f, &creature->path_info,
                                              AIPACKET_MOVEMENT_TO_DESTINATION, 0.0f);
                        }
                    } else if (source == 2) {
                        if (packet->locator != NULL) {
                            AIPATHINFO *locator_path = &packet->locator->path_info;
                            AIMoveInstruction(packet, &packet->locator->position, 0.0f, locator_path,
                                              AIPACKET_MOVEMENT_TO_DESTINATION, 0.0f);
                        }
                    } else {
                        packet->field_0x1e7 = source_flags & 0x2f;
                    }
                }

                AISysCharacterMovement(system, packet, object, checks);

                if ((packet->field_0x1e7 & AIPACKET_MOVEMENT_SOURCE_MASK) != 0 && packet->field_0x180 == NULL &&
                    packet->movement_target_radius > 0.0f &&
                    (object->field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0) {
                    NUVEC *source_position = NULL;
                    const u8 source = (packet->field_0x1e7 & AIPACKET_MOVEMENT_SOURCE_MASK) >> 2;
                    if (source == 1 && packet->field_0x134 != 0xff) {
                        source_position = &system->creatures[packet->field_0x134].pos;
                    } else if (source == 2 && packet->locator != NULL) {
                        source_position = &packet->locator->position;
                    }

                    if (source_position != NULL) {
                        NUVEC source_delta;
                        const f32 distance_squared = NuVecXZDistSqr(source_position, &object->position, &source_delta);
                        const f32 radius = packet->movement_target_radius;
                        const u8 current_source_flags = packet->field_0x1e7;
                        if ((current_source_flags & AIPACKET_MOVEMENT_SOURCE_ACTIVE) == 0) {
                            if (distance_squared > radius * radius) {
                                const f32 release_radius = radius + 0.5f;
                                if (distance_squared > release_radius * release_radius) {
                                    packet->field_0x1e7 = current_source_flags | AIPACKET_MOVEMENT_SOURCE_ACTIVE;
                                } else {
                                    NUVEC movement_delta = {
                                        packet->movement_destination.x - object->position.x,
                                        0.0f,
                                        packet->movement_destination.z - object->position.z,
                                    };
                                    if (NuVecDot(&movement_delta, &source_delta) < 0.0f) {
                                        packet->movement_destination = object->position;
                                        packet->movement_stopping_distance = 0.0f;
                                    }
                                }
                            }
                        } else if (distance_squared < radius * radius) {
                            packet->field_0x1e7 =
                                current_source_flags & static_cast<u8>(~AIPACKET_MOVEMENT_SOURCE_ACTIVE);
                        }
                    }
                }
            }
        }

        f32 clearance = 0.0f;
        if (packet->movement_stopping_distance != 0.0f) {
            clearance = packet->mover_height;
        }

        NUVEC delta;
        if (use_three_dimensions == 0) {
            const f32 distance = NuVecXZDist(&packet->movement_destination, &object->position, &delta);
            const f32 stopping_clearance = clearance + packet->movement_stopping_distance;
            if (distance > packet->mover_height + clearance + packet->movement_stopping_distance) {
                const f32 scale =
                    distance != 0.0f && packet->mover_height != 0.0f ? packet->mover_height / distance : 0.0f;
                NuVecScale(&delta, &delta, scale);
                NuVecAdd(&packet->movement_position, &object->position, &delta);
            } else {
                const f32 scale = distance != 0.0f && stopping_clearance != 0.0f ? stopping_clearance / distance : 0.0f;
                NuVecScale(&delta, &delta, scale);
                NuVecSub(&packet->movement_position, &packet->movement_destination, &delta);
            }
        } else {
            const f32 distance = NuVecDist(&packet->movement_destination, &object->position, &delta);
            const f32 stopping_clearance = clearance + packet->movement_stopping_distance;
            if (distance > packet->mover_height + clearance + packet->movement_stopping_distance) {
                const f32 scale =
                    distance != 0.0f && packet->mover_height != 0.0f ? packet->mover_height / distance : 0.0f;
                NuVecScale(&delta, &delta, scale);
                NuVecAdd(&packet->movement_position, &object->position, &delta);
            } else {
                const f32 scale = distance != 0.0f && stopping_clearance != 0.0f ? stopping_clearance / distance : 0.0f;
                NuVecScale(&delta, &delta, scale);
                NuVecSub(&packet->movement_position, &packet->movement_destination, &delta);
            }
        }
    }

extern "C" i32 AISysGetCharacterWaypoint(const AIPACKET *packet, NUVEC *position) {
    i32 direction;
    if (packet->path_info.connection != packet->fallback_path_info.connection) {

        AIPATHCNX *connection = GetNextConnection(packet, &direction);
        if (connection != NULL) {
            if (position != NULL) {
                *position = packet->path_info.path->nodes[connection->node_indices[direction == 0]].position;
            }
            return 1;
        }
    }
    return 0;
}

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments) {
    f32 previous_x = centre->x;
    f32 previous_z = centre->z + radius;
    for (i32 i = 1; i <= segments; ++i) {
        i32 angle = (i * 65536) / segments;
        f32 next_x = centre->x + radius * NU_SIN_LUT(angle);
        f32 next_z = centre->z + NU_COS_LUT(angle) * radius;
        AiRndrLine3dDbg(previous_x, lower_y, previous_z, next_x, lower_y, next_z, colour);
        AiRndrLine3dDbg(previous_x, upper_y, previous_z, next_x, upper_y, next_z, colour);
        AiRndrLine3dDbg(next_x, lower_y, next_z, next_x, upper_y, next_z, colour);
        previous_x = next_x;
        previous_z = next_z;
    }
}

}

void AISysDrawAntinode_Circle(AIANTINODE_s *node, u32 colour) {
    LocaledbitsDrawSolidCircleXY(&node->position, node->radius, node->min_y, node->max_y, colour, 0, 16);
}

extern "C" {
extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments) {
    NURND_VERTEX3D vertices[2];
    NURND_VERTEX3D &first = vertices[0];
    NURND_VERTEX3D &second = vertices[1];
    second.position.x = 0.0f;
    second.position.y = 0.0f;
    second.position.z = radius_z;
    NuVecRotateY(&second.position, &second.position, rotation);
    NuVecAdd(&second.position, &second.position, centre);
    first.colour = colour;
    second.colour = colour;
    for (i32 i = 1; i <= segments; ++i) {
        first.position = second.position;
        second.position = *centre;
        i32 angle = (i * 65536) / segments;
        NUVEC offset = {radius_x * NU_SIN_LUT(angle), 0.0f, radius_z * NU_COS_LUT(angle)};
        NuVecRotateY(&offset, &offset, rotation);
        NuVecAdd(&second.position, centre, &offset);
        first.position.y = lower_y;
        second.position.y = lower_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
        first.position.y = upper_y;
        second.position.y = upper_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
        first.position = second.position;
        first.position.y = lower_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
    }
}

}

void AISysDrawAntinode_Ellipse(AIANTINODE_s *node, u32 colour) {
    LocaledbitsDrawSolidEllipseXY(&node->position, node->base_radius, node->base_height, node->flags, node->min_y,
                                  node->max_y, colour, 0, 16);
}

extern "C" {
extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments) {
    NUVEC previous = {centre->x, centre->y, centre->z + radius};
    for (i32 i = 1; i <= segments; ++i) {
        i32 angle = (i * 65536) / segments;
        NUVEC next = {centre->x + NU_SIN_LUT(angle) * radius, centre->y, centre->z + NU_COS_LUT(angle) * radius};
        AiRndrLine3dDbg(previous.x, previous.y, previous.z, next.x, next.y, next.z, colour);
        previous = next;
    }
}

extern "C" void ResetAIMessageSys(AIMESSAGESYS_s *sys) {
    if (sys != NULL) {
        NULISTHDR *free_list = &sys->free_list;
        free_list->head = NULL;
        free_list->tail = NULL;
        sys->active_list.head = NULL;
        sys->active_list.tail = NULL;
        memset(sys->messages, 0, sys->count * sizeof(AIMESSAGE_s));
        auto sys_count = sys->count;
        for (i32 index = 0; index < sys_count; ++index) {
            NuLinkedListAppend(free_list, &sys->messages[index].links);
        }
    }
}

extern "C" AIMESSAGESYS_s *CreateAIMessageSys(VARIPTR *cursor, VARIPTR *end, i32 count) {
    AIMESSAGESYS_s *system = static_cast<AIMESSAGESYS_s *>(AISysBufferAlloc(cursor, end, sizeof(AIMESSAGESYS_s)));
    if (system != NULL) {
        memset(system, 0, sizeof(*system));
        system->messages = static_cast<AIMESSAGE_s *>(AISysBufferAlloc(cursor, end, count * sizeof(AIMESSAGE_s)));
        if (system->messages != NULL) {
            system->count = count;
            ResetAIMessageSys(system);
        }
    }
    return system;
}

extern "C" void ClearAIMessageSys(AIMESSAGESYS_s *system) {
    if (system != NULL) {
        for (NULISTLNK *link = NuLinkedListGetHead(&system->active_list); link != NULL;
             link = NuLinkedListGetNext(&system->active_list, link)) {
            reinterpret_cast<AIMESSAGE_s *>(link)->value = 0.0f;
        }
    }
}

extern "C" AIMESSAGE_s *CheckAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message) {
    if (system == NULL) {
        return NULL;
    }
    if (message != NULL) {
        return message;
    }
    if (name != NULL) {
        for (NULISTLNK *link = NuLinkedListGetHead(&system->active_list); link != NULL;
             link = NuLinkedListGetNext(&system->active_list, link)) {
            AIMESSAGE_s *candidate = reinterpret_cast<AIMESSAGE_s *>(link);
            if (NuStrNICmp(name, candidate->name, 32) == 0) {
                return candidate;
            }
        }
    }
    message = reinterpret_cast<AIMESSAGE_s *>(NuLinkedListGetHead(&system->free_list));
    if (message != NULL) {
        NuLinkedListRemove(&system->free_list, &message->links);
        NuLinkedListAppend(&system->active_list, &message->links);
        NuStrNCpy(message->name, name, 32);
    }
    return message;
}

extern "C" f32 GetAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message) {
    message = CheckAIMessage(system, name, message);
    if (message != NULL) {
        return message->value;
    }
    return 0.0f;
}

extern "C" void SetAIMessage(AIMESSAGESYS_s *system, char *name, f32 value, AIMESSAGE_s *message) {
    message = CheckAIMessage(system, name, message);
    if (message != NULL) {
        LOG_INFO_IF(message->value != value, "script message name=%.32s value=%g -> %g", message->name, message->value,
                    value);
        message->value = value;
    }
}

extern "C" AIMESSAGE_s *QueryAIMessage(AIMESSAGESYS_s *system, AIMESSAGE_s *message) {
    if (message != NULL) {
        return reinterpret_cast<AIMESSAGE_s *>(NuLinkedListGetNext(&system->active_list, &message->links));
    }
    return reinterpret_cast<AIMESSAGE_s *>(NuLinkedListGetHead(&system->active_list));
}

extern "C" void RemoveAIMessage(AIMESSAGESYS_s *system, char *name, AIMESSAGE_s *message) {
    if (system != NULL) {
        if (message == NULL) {
            if (name != NULL) {
                message = reinterpret_cast<AIMESSAGE_s *>(NuLinkedListGetHead(&system->active_list));
                while (message != NULL) {
                    if (NuStrNICmp(name, message->name, 32) == 0) {
                        break;
                    }
                    message =
                        reinterpret_cast<AIMESSAGE_s *>(NuLinkedListGetNext(&system->active_list, &message->links));
                }
            }
        }
        if (message != NULL) {
            NuLinkedListRemove(&system->active_list, &message->links);
            memset(message, 0, sizeof(*message));
            NuLinkedListAppend(&system->free_list, &message->links);
        }
    }
}

extern "C" AILOCALMESSAGE_s *FindLocalAIMessage(AISCRIPTPROCESS *processor, char *name) {
    for (AILOCALMESSAGE_s *message = processor->local_messages; message != NULL; message = message->next) {
        if (NuStrICmp(message->message.name, name) == 0) {
            return message;
        }
    }
    return NULL;
}

extern "C" void AddLocalAIMessage(AISCRIPTPROCESS *processor, AILOCALMESSAGE_s *message, char *name) {
    NuStrNCpy(message->message.name, name, 32);
    if (processor->local_messages != NULL) {
        message->next = processor->local_messages;
        processor->local_messages = message;
    } else {
        processor->local_messages = message;
        message->next = NULL;
    }
}

    void *QueryLocalMessage(void *context, void *message) {
        if (message != NULL)
            return *reinterpret_cast<void **>(static_cast<u8 *>(message) + 0x2c);
        return *reinterpret_cast<void **>(static_cast<u8 *>(context) + 0xc4);
    }

extern "C" void AILocatorSet_CheckLocatorsStillAssigned(AISYS *system, AILOCATORSET *locator_set) {
    if (locator_set == NULL || APIOBJECTFromObjIDFn == NULL) {
        return;
    }

    for (i32 index = 0; index < locator_set->locator_count; ++index) {
        const u8 assignment = locator_set->assigned[index];
        if (assignment == 0x80 || assignment == 0xff) {
            continue;
        }

        u8 locator_index = 0xff;
        APIOBJECT *object = APIOBJECTFromObjIDFn(assignment);
        if (object != NULL && object->ai != NULL && object->ai->locator != NULL) {
            locator_index = object->ai->locator - system->locators;
        }
        if (locator_index != locator_set->locator_entries[index]) {
            locator_set->assigned[index] = 0xff;
        }
    }
}

extern "C" void AILocatorSet_AssignNearestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                  f32 max_range, NUVEC *position, NUVEC *second_position,
                                                  f32 off_screen_radius, i32 ignore_assigned) {
    if (object != NULL && locator_set != NULL && object->ai != NULL && position != NULL) {
        f32 nearest_distance = max_range > 0.0f ? max_range * max_range : FLT_MAX;
        NUVEC difference;
        if (ignore_assigned != 0) {
            AILocatorSet_CheckLocatorsStillAssigned(system, locator_set);
        }

        i32 nearest_index = -1;
        for (i32 index = 0; index < locator_set->locator_count; ++index) {
            if (!(ignore_assigned != 0 && locator_set->assigned[index] != 0xff)) {

                AILOCATOR *locator = &system->locators[locator_set->locator_entries[index]];
                if (off_screen_radius != 0.0f &&
                    NuCameraClipTestSphere(&locator->position, off_screen_radius, &numtx_identity) == 0) {
                    continue;
                }

                const f32 first_distance = NuVecDistSqr(position, &locator->position, &difference);
                if (first_distance < nearest_distance) {
                    nearest_distance = first_distance;
                    nearest_index = index;
                }
                if (second_position != NULL) {
                    const f32 second_distance = NuVecDistSqr(second_position, &locator->position, &difference);
                    if (second_distance < nearest_distance) {
                        nearest_distance = second_distance;
                        nearest_index = index;
                    }
                }
            }
        }

        if (nearest_index != -1) {
            object->ai->locator = &system->locators[locator_set->locator_entries[nearest_index]];
            locator_set->assigned[nearest_index] = object->field_0x289;
        }
    }
}

extern "C" void AILocatorSet_AssignFurthestLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                   f32 max_range, NUVEC *position, NUVEC *second_position,
                                                   f32 off_screen_radius, i32 ignore_assigned) {
    if (object != NULL && locator_set != NULL && object->ai != NULL) {
        if (ignore_assigned != 0) {
            AILocatorSet_CheckLocatorsStillAssigned(system, locator_set);
        }

        NUVEC difference;
        i32 furthest_index = -1;
        f32 furthest_distance = 0.0f;
        for (i32 index = 0; index < locator_set->locator_count; ++index) {
            if (ignore_assigned != 0 && locator_set->assigned[index] != 0xff) {
                continue;
            }

            AILOCATOR *locator = &system->locators[locator_set->locator_entries[index]];
            if (!(off_screen_radius != 0.0f &&
                  NuCameraClipTestSphere(&locator->position, off_screen_radius, &numtx_identity) == 0)) {

                f32 distance = NuVecDistSqr(position, &locator->position, &difference);
                if (distance > furthest_distance) {
                    if (second_position != NULL) {
                        const f32 second_distance = NuVecDistSqr(second_position, &locator->position, &difference);
                        if (second_distance > furthest_distance) {
                            distance = MIN(second_distance, distance);
                            if (max_range == 0.0f || max_range * max_range > distance) {
                                furthest_distance = distance;
                                furthest_index = index;
                            }
                        }
                    } else if (max_range == 0.0f || max_range * max_range > distance) {
                        furthest_distance = distance;
                        furthest_index = index;
                    }
                }
            }
        }

        if (furthest_index != -1) {
            object->ai->locator = &system->locators[locator_set->locator_entries[furthest_index]];
            locator_set->assigned[furthest_index] = object->field_0x289;
        }
    }
}

extern "C" void AILocatorSet_AssignRandomLocator(AISYS *system, AILOCATORSET *locator_set, APIOBJECT *object,
                                                 f32 max_range, NUVEC *position, f32 off_screen_radius,
                                                 i32 ignore_assigned) {
    if (object == NULL || locator_set == NULL || object->ai == NULL) {
        return;
    }

    const f32 max_distance = max_range > 0.0f ? max_range * max_range : FLT_MAX;
    NUVEC difference;
    if (ignore_assigned != 0) {
        AILocatorSet_CheckLocatorsStillAssigned(system, locator_set);
    }
    i32 candidate_count = 0;
    for (i32 index = 0; index < locator_set->locator_count; ++index) {
        if (!(ignore_assigned != 0 && locator_set->assigned[index] != 0xff)) {

            AILOCATOR *locator = &system->locators[locator_set->locator_entries[index]];
            if (!(off_screen_radius != 0.0f &&
                  NuCameraClipTestSphere(&locator->position, off_screen_radius, &numtx_identity) == 0)) {
                if (NuVecDistSqr(position, &locator->position, &difference) <= max_distance) {
                    ++candidate_count;
                }
            }
        }
    }

    if (candidate_count == 0) {
        return;
    }

    const i32 selected_candidate = NuRandInt() % candidate_count;
    i32 candidate_index = 0;
    for (i32 index = 0; index < locator_set->locator_count; ++index) {
        if (!(ignore_assigned != 0 && locator_set->assigned[index] != 0xff)) {

            AILOCATOR *locator = &system->locators[locator_set->locator_entries[index]];
            if (NuVecDistSqr(&object->position, &locator->position, &difference) <= max_distance) {
                if (candidate_index == selected_candidate) {
                    object->ai->locator = locator;
                    locator_set->assigned[index] = object->field_0x289;
                    break;
                }
                ++candidate_index;
            }
        }
    }
}

    u32 AISysGetPathColour(i32 index) {
        return iAISysPathColoursG[index];
    }

    i32 AISysGetPathColourCount(void) {
        return 5;
    }

    void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function) {
        GameAISYSRebuildFromEditorDataFn = function;
    }

    void InitFn_GameAISave(AIEDITORCALLBACK *function) {
        GameAISaveFn = function;
    }

    void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function) {
        AIPathNodeDeletedFn = function;
    }

    void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function) {
        AIPathNodeMovedFn = function;
    }

    void InitFn_AIPathDeleted(AIEDITORCALLBACK *function) {
        AIPathDeletedFn = function;
    }

    void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function) {
        PreparingForSpecialMoveFn = function;
    }

    void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function) {
        MidSpecialMoveFn = function;
    }

    void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function) {
        GetNamedAPIObjectFn = function;
    }

    void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function) {
        GetAICreatureOriginFn = function;
    }

    i32 aieditor_Register(const char *name, void (*enter)(), void (*callback_24)(), void (*callback_28)(),
                          void (*leave)()) {
        if (aieditorsettings.mode_count > 7) {
            return -1;
        }
        i32 index = aieditorsettings.mode_count;
        ++aieditorsettings.mode_count;
        aieditor_settings_s::EditorMode &mode = aieditorsettings.modes[index];
        NuStrCpy(mode.name, name);
        mode.enter = enter;
        mode.callback_24 = callback_24;
        mode.callback_28 = callback_28;
        mode.leave = leave;
        return index;
    }

    void aieditor_SetMode(i32 mode) {
        u16 previous_mode = aieditorsettings.current_mode;
        aieditorsettings.current_mode = mode;
        AIEDITOR_RENDER_STATE *state = aieditor;
        if (state != nullptr && previous_mode != (u16)mode) {
            if (state->current_path != nullptr) {
                state->current_path->current_node = nullptr;
            }
            state->mode_selection_36930 = nullptr;
            state->mode_selection_37a48 = nullptr;
            state->mode_selection_3c260 = nullptr;
            state->mode_selection_42e9c = nullptr;
        }
    }

    void aieditor_Enter(AISYS_s *system, NUVEC *camera_position, void *context, void *owner, void *extra_1,
                        void *extra_2, void *extra_3) {
        AIEDITOR_RENDER_STATE *state = aieditor;
        state->flags |= 2;
        state->ai_system = system;
        state->enter_context = context;
        state->enter_owner = owner;
        state->enter_extra_1 = extra_1;
        state->enter_extra_2 = extra_2;
        state->enter_extra_3 = extra_3;
        if (camera_position != nullptr) {
            edcamSetPos(camera_position);
        }
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            void (*enter)() = aieditorsettings.modes[index].enter;
            if (enter != nullptr) {
                enter();
            }
        }
        aieditorsettings.snap_height_display = 1;
    }

extern "C++" {
namespace {
    struct SpecialPlatform {
        u8 pad_00[0x4a];
        i16 scene_object_index;
        u8 pad_4c[0x20];
    };
    struct SpecialTerrain {
        u8 pad_00[0x68];
        SpecialPlatform *platforms;
    };
    DECOMP_ASSERT(sizeof(SpecialPlatform) == 0x6c, "terrain platform stride for special lookup");
} // namespace

i32 NuSpecialFindByPlatformID(nugscn_s *scene, nuhspecial_s *result, i32 platform_id) {
    result->scene = NULL;
    result->special = NULL;
    result->display_special = NULL;
    if (scene == NULL || CurTerr == NULL || platform_id == -1)
        return 0;

    const i32 special_count = NuGScnNumSpecials(scene);
    const auto *terrain = reinterpret_cast<const SpecialTerrain *>(CurTerr);
    const i32 instance_index = terrain->platforms[platform_id].scene_object_index;
    for (i32 index = 0; index < special_count; ++index) {
        nuhspecial_s candidate;
        NuGScnGetSpecial(&candidate, scene, index);
        if (NuSpecialGetInstanceix(&candidate) == instance_index) {
            *result = candidate;
            return 1;
        }
    }
    return 0;
}
}

    void AISysSetPathCylinderCheck(i32 enabled) {
        disable_cylinder_check = enabled == 0;
    }

    void aieditor_Render(f32 x_scale, f32 y_scale) {
        NUCAMERA *camera = edmainGetCamera();
        if (near_clip_at_cursor == 0) {
            camera->near_clip = 0.15f;
        } else {
            camera->near_clip =
                NuVecDist(&aieditor->cursor_position, reinterpret_cast<NUVEC *>(&camera->mtx.m30), nullptr) - 5.0f;
        }
        edcamSet();

        f32 cursor_radius = 0.5f;
        i32 cursor_colour = -1;
        if (__builtin_expect(aieditorsettings.snap_height_display, 1)) {
            edbitsDrawCross(aieditor->cursor_position.x, aieditor->cursor_position.y, aieditor->cursor_position.z, 0.5f,
                            -1, aieditorsettings.path_material);
            cursor_radius = 0.25f;
            cursor_colour = 0xff00ff00;
        }
        edbitsDrawCross(aieditor->camera_position.x, aieditor->camera_position.y, aieditor->camera_position.z,
                        cursor_radius, cursor_colour, aieditorsettings.path_material);
        NuRndrRect2di(0x1680, 0x9b0, 0x1180, 0x488, 0x80808080, aieditorsettings.overlay_material);
        NuRndrRect2di(0x1670, 0x8f0, 0x11a0, 0xc0, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x1670, 0x9b0, 0x1670, 0xe40, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x2810, 0x9b0, 0x2810, 0xe40, 0x80000000, aieditorsettings.overlay_material);
        NuRndrLine2di(0x1670, 0xe40, 0x2810, 0xe40, 0x80000000, aieditorsettings.overlay_material);

        NuQFntPushPrintMode(2);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80808080);
        NuQFntSetScale(system_qfont, x_scale, y_scale);
        auto render = reinterpret_cast<void (*)(i32, i32, f32, f32)>(
            aieditorsettings.modes[static_cast<i16>(aieditorsettings.current_mode)].callback_28);
        if (render != nullptr) {
            render(0x168, 0x136, x_scale, y_scale);
        }
        NuQFntSet(system_qfont);
        NuQFntSetColour(system_qfont, 0x80000000);
        NuQFntSetScale(system_qfont, x_scale, y_scale);
        NuQFntPrintEx(system_qfont, 0x1720, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.x));
        NuQFntPrintEx(system_qfont, 0x1b80, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.y));
        NuQFntPrintEx(system_qfont, 0x1fe0, 0xde8, 0x10, "%5.2f", static_cast<f64>(aieditor->camera_position.z));
        if (aieditor->main_menu != nullptr) {
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            eduiMenuRender(aieditor->main_menu);
        }
        NuQFntPopPrintMode();
        NuQFntPopCoordinateSystem();
    }

    void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result) {
        AIPATHCNX *connection = packet->path_info.connection;
        if (connection == NULL) {
            *result = packet->owner->apiobj.position;
            return;
        }

        f32 direction_x = direction->x;
        f32 direction_z = direction->z;
        AIPATHNODE *nodes = packet->path_info.path->nodes;
        AIPATHNODE *start = &nodes[connection->direction_a];
        AIPATHNODE *end = &nodes[connection->direction_b];
        f32 dx = end->position.x - start->position.x;
        f32 dz = end->position.z - start->position.z;
        if (direction_x * dx + direction_z * dz < 0.0f) {
            dx = start->position.x - end->position.x;
            dz = start->position.z - end->position.z;
            AIPATHNODE *swap = start;
            start = end;
            end = swap;
        }
        f32 length = connection->horizontal_distance;
        dx /= length;
        dz /= length;
        if ((flags & 2) != 0) {
            distance *= length;
        }
        f32 along = (position->x - start->position.x) * dx + (position->z - start->position.z) * dz + distance;
        if (along > length || along < 0.0f) {
            AIPATHNODE *junction = along < 0.0f ? start : end;
            const i32 connection_end = static_cast<i32>(junction->connection_count) + 1;
            if (connection_end != 1) {
                AIPATHCNX **connections = junction->connections;
                i32 best_index = -1;
                f32 best_dot = -FLT_MAX;
                for (i32 index = 1; index != connection_end; ++index) {
                    AIPATHCNX *candidate = connections[index - 1];
                    if (candidate == connection) {
                        continue;
                    }
                    AIPATHNODE *candidate_start = &nodes[candidate->direction_a];
                    AIPATHNODE *candidate_end = &nodes[candidate->direction_b];
                    dx = candidate_end->position.x - candidate_start->position.x;
                    dz = candidate_end->position.z - candidate_start->position.z;
                    i32 reverse = 0;
                    if (direction_x * dx + direction_z * dz < 0.0f) {
                        dx = candidate_start->position.x - candidate_end->position.x;
                        dz = candidate_start->position.z - candidate_end->position.z;
                        reverse = 1;
                    }
                    dx /= candidate->horizontal_distance;
                    dz /= candidate->horizontal_distance;
                    if ((candidate->traversal_flags[reverse] & 0xc0000000u) == 0) {
                        f32 dot = direction_x * dx + direction_z * dz;
                        if (dot > best_dot) {
                            best_dot = dot;
                            best_index = index - 1;
                        }
                    }
                }
                if (best_index != -1) {
                    connection = connections[best_index];
                    if (along < 0.0f) {
                        length = connection->horizontal_distance;
                        along += length;
                    } else {
                        along -= length;
                        length = connection->horizontal_distance;
                    }
                    start = &nodes[connection->direction_a];
                    end = &nodes[connection->direction_b];
                    dx = end->position.x - start->position.x;
                    dz = end->position.z - start->position.z;
                    if (direction_x * dx + direction_z * dz < 0.0f) {
                        dx = start->position.x - end->position.x;
                        dz = start->position.z - end->position.z;
                        AIPATHNODE *swap = start;
                        start = end;
                        end = swap;
                    }
                    dx /= length;
                    dz /= length;
                }
            }
        }
        f32 fraction = NuFmin(NuFmax(along / length, 0.0f), 1.0f);
        f32 inverse_fraction = 1.0f - fraction;
        f32 radius = fraction * end->radius + inverse_fraction * start->radius;
        along = connection->horizontal_distance * fraction;
        if ((flags & 1) != 0) {
            offset *= radius;
        }
        offset = NuFmin(NuFmax(offset, -radius), radius);
        result->x = start->position.x + along * dx + dz * offset;
        result->y = fraction * end->position.y + inverse_fraction * start->position.y;
        result->z = start->position.z + along * dz - dx * offset;
    }

extern "C" void areaEditorSaveData() {
    i32 count = 0;
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        ++count;
    }
    EdFileWriteInt(count);
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        EdFileWrite(area->name, 16);
        EdFileWriteFloat(area->position.x);
        EdFileWriteFloat(area->position.y);
        EdFileWriteFloat(area->position.z);
        EdFileWriteFloat(area->size.x);
        EdFileWriteFloat(area->size.y);
        EdFileWriteFloat(area->size.z);
        EdFileWriteShort(area->rotation);
        if (aidata_version > 19) {
            EdFileWriteChar(area->flags);
        } else {
            EdFileWriteChar(0);
        }
        EdFileWriteChar(0);
    }
}

}

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour) {
    f32 local_x[4] __attribute__((aligned(16))) = {-size->x, -size->x, size->x, size->x};
    f32 rotated_x[4] __attribute__((aligned(16)));
    f32 local_z[4] __attribute__((aligned(16))) = {-size->z, size->z, size->z, -size->z};
    f32 rotated_z[4] __attribute__((aligned(16)));
    const f32 cosine = NU_COS_LUT(rotation);
    const f32 sine = NU_SIN_LUT(rotation);
    for (i32 i = 0; i < 4; ++i) {
        rotated_x[i] = cosine * local_x[i] + sine * local_z[i];
        rotated_z[i] = cosine * local_z[i] - sine * local_x[i];
    }
#define DRAW_AREA_BOX_EDGE(a, b, y0, y1)                                                                               \
    AiRndrLine3dDbg(position->x + rotated_x[a], y0, position->z + rotated_z[a], position->x + rotated_x[b], y1,        \
                    position->z + rotated_z[b], colour)
    DRAW_AREA_BOX_EDGE(0, 1, position->y, position->y);
    DRAW_AREA_BOX_EDGE(1, 2, position->y, position->y);
    DRAW_AREA_BOX_EDGE(2, 3, position->y, position->y);
    DRAW_AREA_BOX_EDGE(3, 0, position->y, position->y);
    DRAW_AREA_BOX_EDGE(0, 0, position->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(1, 1, position->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(2, 2, position->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(3, 3, position->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(0, 1, position->y + size->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(1, 2, position->y + size->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(2, 3, position->y + size->y, position->y + size->y);
    DRAW_AREA_BOX_EDGE(3, 0, position->y + size->y, position->y + size->y);
#undef DRAW_AREA_BOX_EDGE
}

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour) {
    LocaledbitsDrawSolidCircleXY(centre, NuFmin(size->x, size->z), centre->y, centre->y + size->y, colour, 0, 16);
}

extern "C" {
extern "C" void areaEditorDrawAreas() {
    f32 previous_distance = AiRndrCullDistance;
    SetAiRndrCullDistance(0.0f);
    for (EDAIAREA_s *area = area_head(); area != NULL; area = area_next(area)) {
        i32 colour;
        if (area == area_selected()) {
            colour = 0xff0000ff;
            if (area != area_hovered()) {
                colour = 0x800000ff;
            }
        } else {
            colour = area == area_hovered() ? 0xffffffff : 0;
        }
        if (area->flags & 1) {
            DrawAreaCylinder(&area->position, &area->size, colour);
        } else {
            DrawAreaBox(&area->position, &area->size, area->rotation, colour);
        }
    }
    SetAiRndrCullDistance(previous_distance);
}

    void locatorEditorSaveData(AIPATHSYS_s *path_system) {
        i32 locator_count = 0;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            locator->runtime_index = 0xff;
            if (locator->path != nullptr) {
                locator->runtime_index = locator_count++;
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        EdFileWriteInt(locator_count);
        locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (locator->path != nullptr) {
                EdFileWrite(locator->name, 16);
                EdFileWriteFloat(locator->position.x);
                EdFileWriteFloat(locator->position.y);
                EdFileWriteFloat(locator->position.z);
                EdFileWriteShort(locator->direction);
                EdFileWriteChar(locator->path->draw_index);
                i32 connection_index = 0;
                i32 connection_direction = 0;
                AIPATH_s *path = path_system->paths[locator->path->draw_index];
                for (i32 index = 0; index < path->connection_count; ++index) {
                    AIPATHCNX_s *connection = &path->connections[index];
                    i32 first = locator->first_node->index;
                    i32 second = locator->second_node->index;
                    if (connection->node_indices[0] == first && connection->node_indices[1] == second) {
                        connection_index = index;
                        i32 angle = locator->path_angle;
                        i32 magnitude = angle < 0 ? -angle : angle;
                        if (magnitude > 0x3fff) {
                            connection_direction = 1;
                        } else {
                            connection_direction = 0;
                        }
                        break;
                    }
                    if (connection->node_indices[0] == second && connection->node_indices[1] == first) {
                        connection_index = index;
                        i32 angle = locator->path_angle;
                        i32 magnitude = angle < 0 ? -angle : angle;
                        if (magnitude > 0x3fff) {
                            connection_direction = 0;
                        } else {
                            connection_direction = 1;
                        }
                        break;
                    }
                }
                EdFileWriteChar(connection_direction);
                EdFileWriteShort(connection_index);
                EdFileWriteFloat(locator->path_fraction);
                EdFileWriteFloat(locator->path_width);
                if (aidata_version > 14) {
                    EdFileWriteInt(locator->path_angle);
                }
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        if (aidata_version > 17) {
            i32 set_count = 0;
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            while (set != nullptr) {
                ++set_count;
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
            EdFileWriteInt(set_count);
            set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            while (set != nullptr) {
                i32 member_count = 0;
                i32 index = 0;
                while (true) {
                    EDLOCATOR_s *member = set->locators[index];
                    if (member == nullptr) {
                        break;
                    }
                    if (member->runtime_index != 0xff) {
                        ++member_count;
                    }
                    ++index;
                    if (index == 64) {
                        break;
                    }
                }
                EdFileWrite(set->name, 16);
                EdFileWriteInt(member_count);
                if (member_count != 0) {
                    index = 0;
                    while (true) {
                        EDLOCATOR_s *member = set->locators[index];
                        if (member == nullptr) {
                            break;
                        }
                        if (member->runtime_index != 0xff) {
                            EdFileWriteChar(member->runtime_index);
                        }
                        ++index;
                        if (index == 64) {
                            break;
                        }
                    }
                }
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
        }
    }

    EDLOCATOR_s *locatorEditor_GetNearest(i32 use_width) {
        EDLOCATOR_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &locator->position, &delta);
            if (distance < nearest_distance) {
                if (use_width != 0) {
                    NuVecRotateY(&delta, &delta, -locator->direction);
                    if (delta.x < aiEditor_LocatorWidth && delta.y < aiEditor_LocatorWidth &&
                        delta.z < aiEditor_LocatorWidth && -delta.x < aiEditor_LocatorWidth &&
                        -delta.y < aiEditor_LocatorWidth && -delta.z < aiEditor_LocatorWidth) {
                        nearest = locator;
                        nearest_distance = distance;
                    }
                } else {
                    nearest = locator;
                    nearest_distance = distance;
                }
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        return nearest;
    }

    void locatorEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (locator->first_node == node || locator->second_node == node) {
                NUVEC difference;
                NUVEC direction;
                NUVEC movement;
                NuVecSub(&difference, &locator->second_node->position, &locator->first_node->position);
                NuVecNorm(&direction, &difference);
                f32 radius;
                if (locator->path_fraction > 1.0f) {
                    radius = locator->second_node->radius;
                } else if (locator->path_fraction < 0.0f) {
                    radius = locator->first_node->radius;
                } else {
                    radius = locator->second_node->radius * locator->path_fraction +
                             locator->first_node->radius * (1.0f - locator->path_fraction);
                }
                locator->position = locator->first_node->position;
                f32 normalized_x = direction.x;
                direction.x = direction.z * radius;
                direction.z = -normalized_x * radius;
                NuVecScale(&movement, &difference, locator->path_fraction);
                NuVecAdd(&locator->position, &locator->position, &movement);
                NuVecScale(&movement, &direction, locator->path_width);
                NuVecAdd(&locator->position, &locator->position, &movement);
                locator->direction =
                    NuAngAdd((i32)(NuAtan2(difference.x, difference.z) * 10430.378f), locator->path_angle);
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
    }

    void locatorEditor_PathDeleted(EDAIPATH_s *path) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            EDLOCATOR_s *next = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
            if (locator->path == path) {
                DestroyLocator(locator);
                if (aieditor->current_locator == locator) {
                    aieditor->current_locator = nullptr;
                }
            }
            locator = next;
        }
    }

}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour) {
    NURND_VERTEX3D vertices[2];
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NUVEC centre = *position;
    centre.y += aiEditor_DrawYOffset;
    NUVEC axis = {radius, 0.0f, 0.0f};
    NuVecRotateY(&axis, &axis, rotation);
    NuVecAdd(&vertices[0].position, &centre, &axis);
    NuVecSub(&vertices[1].position, &centre, &axis);
    AiRndrLine3d(vertices, NULL, NULL);
    vertices[0].position.x = centre.x;
    vertices[1].position.x = centre.x;
    vertices[0].position.y = centre.y - radius;
    vertices[0].position.z = centre.z;
    vertices[1].position.z = centre.z;
    vertices[1].position.y = centre.y + radius;
    AiRndrLine3d(vertices, NULL, NULL);
    axis.x = 0.0f;
    axis.z = radius;
    axis.y = 0.0f;
    NuVecRotateY(&axis, &axis, rotation);
    NuVecAdd(&vertices[0].position, &centre, &axis);
    NuVecSub(&vertices[1].position, &centre, &axis);
    AiRndrLine3d(vertices, NULL, NULL);
    NUVEC arrow = {-axis.z * 0.2f, 0.0f, axis.x * 0.2f};
    NuVecAdd(&vertices[1].position, &centre, &arrow);
    AiRndrLine3d(vertices, NULL, NULL);
    NuVecSub(&vertices[1].position, &centre, &arrow);
    AiRndrLine3d(vertices, NULL, NULL);
}

extern "C" {
    void locatorEditorDrawLocators(void) {
        AIEDITOR_RENDER_STATE *const *state = &aieditor;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&(*state)->locators);
        while (locator != nullptr) {
            locator->drawn = 0;
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&(*state)->locators, &locator->link);
        }
        if ((*state)->current_locator_set != nullptr) {
            for (i32 index = 0; index < 64; ++index) {
                locator = (*state)->current_locator_set->locators[index];
                if (locator == nullptr)
                    break;
                i32 colour;
                if (locator == (*state)->current_locator) {
                    colour = locator == (*state)->nearest_locator ? 0xff0000ff : 0x800000ff;
                } else {
                    colour = locator == (*state)->nearest_locator ? -1 : 0x32323232;
                }
                DrawLocator(&locator->position, aiEditor_LocatorWidth, locator->direction, colour);
                locator->drawn = 1;
            }
        }
        locator = (EDLOCATOR_s *)NuLinkedListGetHead(&(*state)->locators);
        while (locator != nullptr) {
            if (!locator->drawn) {
                i32 colour;
                if ((*state)->current_locator_set != nullptr) {
                    if (locator == (*state)->current_locator) {
                        colour = locator == (*state)->nearest_locator ? 0x50 : 0x28;
                    } else {
                        colour = locator == (*state)->nearest_locator ? 0x64646464 : 0x32323232;
                    }
                } else if (locator == (*state)->current_locator) {
                    colour = locator == (*state)->nearest_locator ? 0xff0000ff : 0x800000ff;
                } else {
                    colour = locator == (*state)->nearest_locator ? -1 : 0x32323232;
                }
                DrawLocator(&locator->position, aiEditor_LocatorWidth, locator->direction, colour);
                locator->drawn = 1;
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&(*state)->locators, &locator->link);
        }
    }

extern "C" void aieditor_ClearAllPathCnxTypes(void) {
    naipathcnxtypes = 0;
    memset(aipathcnxtypes, 0, sizeof(aipathcnxtypes));
}

extern "C" void aieditor_RegisterPathCnxType(const char *name, u32 connection_flag, void *context, u32 flags) {
    if (name == nullptr) {
        return;
    }
    usize length = strlen(name);
    if (length > 63 || connection_flag == 0 || naipathcnxtypes >= 32) {
        return;
    }
    AIPATHCNXTYPE_s &type = aipathcnxtypes[naipathcnxtypes++];
    memcpy(type.name, name, length + 1);
    type.connection_flag = connection_flag;
    type.context = context;
    type.flags = flags;
}

extern "C" void aieditor_RegisterDefaultPathCnxTypes(void) {
    aieditor_RegisterPathCnxType("Permanent Block", 0x40000000, nullptr, 0);
    aieditor_RegisterPathCnxType("Temporary Block", 0x80000000, nullptr, 0);
    aieditor_RegisterPathCnxType("Link Obstacle", 0x20000000, nullptr, 1);
}

    void aieditor_Init(AIEDITOR_RENDER_STATE *state, void **external_display_a, void **external_display_b) {
        aieditor = state;
        memset(&aieditorsettings, 0, sizeof(aieditorsettings));

        NUMTL *path_material = NuMtlCreate3D(1);
        aieditorsettings.path_material = path_material;
        path_material->opacity = 1.0f;
        path_material->diffuse_color.r = 0.5f;
        path_material->diffuse_color.g = 0.5f;
        path_material->diffuse_color.b = 0.5f;
        path_material->attribs.alpha_mode = 0;
        path_material->attribs.cull_mode = 2;
        path_material->attribs.z_mode = 0;
        NuMtlUpdate(path_material);

        NUMTL *route_material = NuMtlCreate3D(1);
        aieditorsettings.route_material = route_material;
        route_material->opacity = 1.0f;
        route_material->diffuse_color.r = 0.5f;
        route_material->diffuse_color.g = 0.5f;
        route_material->diffuse_color.b = 0.5f;
        route_material->attribs.cull_mode = 2;
        route_material->attribs.z_mode = 0;
        route_material->attribs.alpha_mode = 2;
        NuMtlUpdate(route_material);

        NUMTL *overlay_material = NuMtlCreate(1);
        aieditorsettings.overlay_material = overlay_material;
        overlay_material->diffuse_color.r = 0.5f;
        overlay_material->diffuse_color.g = 0.5f;
        overlay_material->diffuse_color.b = 0.5f;
        overlay_material->attribs.cull_mode = 2;
        overlay_material->attribs.alpha_mode = 0;
        overlay_material->attribs.z_mode = 3;
        overlay_material->opacity = 1.0f;
        NuMtlUpdate(overlay_material);

        aieditorsettings.current_path_type = -1;
        aieditorsettings.current_script_flags = 0;
        aieditorsettings.draw_all_paths = 0;
        aieditorsettings.unknown_060_bit0 = 0;
        aieditorsettings.show_creatures_display = 1;
        aieditorsettings.snap_height_display = 1;
        aieditorsettings.solid_path_display = 0;
        aieditorsettings.unknown_060_bit5 = 0;
        aieditorsettings.solid_antinode_display = 1;
        aieditorsettings.draw_wallsplines = 1;
        aieditorsettings.show_creatures_set = 0;
        aieditorsettings.current_script_params[0] = 0.0f;
        aieditorsettings.current_script_params[1] = 0.0f;
        aieditorsettings.current_script_params[2] = 0.0f;
        aieditorsettings.current_script_params[3] = 0.0f;
        aieditorsettings.mode_count = 0;
        memset(aieditorsettings.modes, 0, sizeof(aieditorsettings.modes));
        aieditorsettings.path_height_offset = 0.01f;

        AIEDITOR_PATHS =
            aieditor_Register("AIEDITOR_PATHS", pathEditor_Enter, reinterpret_cast<void (*)()>(pathEditor_Process),
                              reinterpret_cast<void (*)()>(pathEditor_Render), nullptr);
        AIEDITOR_ROUTES =
            aieditor_Register("AIEDITOR_ROUTES", nullptr, reinterpret_cast<void (*)()>(routeEditor_Process),
                              reinterpret_cast<void (*)()>(routeEditor_Render), nullptr);
        AIEDITOR_AREAS =
            aieditor_Register("AIEDITOR_AREAS", areaEditor_Enter, reinterpret_cast<void (*)()>(areaEditor_Process),
                              reinterpret_cast<void (*)()>(areaEditor_Render), nullptr);
        AIEDITOR_LOCATORS = aieditor_Register("AIEDITOR_LOCATORS", locatorEditor_Enter,
                                              reinterpret_cast<void (*)()>(locatorEditor_Process),
                                              reinterpret_cast<void (*)()>(locatorEditor_Render), nullptr);
        AIEDITOR_CREATURES = aieditor_Register("AIEDITOR_CREATURES", creatureEditor_Enter,
                                               reinterpret_cast<void (*)()>(creatureEditor_Process),
                                               reinterpret_cast<void (*)()>(creatureEditor_Render), nullptr);
        AIEDITOR_ANTINODES = aieditor_Register("AIEDITOR_ANTINODES", antinodeEditor_Enter,
                                               reinterpret_cast<void (*)()>(antinodeEditor_Process),
                                               reinterpret_cast<void (*)()>(antinodeEditor_Render), nullptr);
        aieditor_RegisterDefaultPathCnxTypes();
        aieditorsettings.external_display_a = *external_display_a;
        aieditorsettings.external_display_b = *external_display_b;
    }

}

__attribute__((force_align_arg_pointer)) void pathEditorDrawNode(nuvec_s *position, float radius, float lower_height,
                                                                 float upper_height, u32 colour, numtl_s *material,
                                                                 i32 segments, i32 solid) {
    NUVEC centre = {position->x, position->y + aiEditor_DrawYOffset, position->z};
    if (solid != 0) {
        LocaledbitsDrawSolidCircleXY(&centre, radius, lower_height, upper_height, colour,
                                     static_cast<i32>(reinterpret_cast<uintptr_t>(material)), segments);
    } else {
        LocaledbitsDrawCircleXY(&centre, radius, colour, static_cast<i32>(reinterpret_cast<uintptr_t>(material)),
                                segments);
    }
}

void pathEditorDrawConnectionInfo(nuvec_s *position, float radius, nuvec_s *other, u32 flags, i32 colour) {
    if (flags != 0) {
        NUVEC start = {position->x, position->y + aiEditor_DrawYOffset, position->z};
        NUVEC end = {other->x, other->y + aiEditor_DrawYOffset, other->z};
        for (i32 i = 0; i < naipathcnxtypes; ++i) {
            AIPATHCNXTYPE_s *type = &aipathcnxtypes[i];
            if (type->context != nullptr && (type->connection_flag & flags) != 0) {
                typedef void DrawConnection(NUVEC *, f32, NUVEC *, u32, i32);
                reinterpret_cast<DrawConnection *>(type->context)(&start, radius, &end, flags, colour);
                return;
            }
        }
        NUVEC direction;
        f32 length = NuVecXZDist(&end, &start, &direction);
        if (length > 0.0f) {
            direction.x /= length;
            direction.z /= length;
        }
        NURND_VERTEX3D vertices[2];
        vertices[0].position = start;
        vertices[1].position = end;
        vertices[0].colour = colour;
        vertices[1].colour = colour;
        AiRndrLine3d(vertices, nullptr, nullptr);
        NuVecScale(&vertices[0].position, &direction, radius * 0.5f);
        vertices[0].position.y = 0.0f;
        NuVecAdd(&vertices[0].position, &vertices[0].position, &start);
        vertices[1].position = start;
        vertices[1].position.x += radius * direction.z * 0.5f;
        vertices[1].position.z -= radius * direction.x * 0.5f;
        AiRndrLine3d(vertices, nullptr, nullptr);
        vertices[1].position = start;
        vertices[1].position.x -= radius * direction.z * 0.5f;
        vertices[1].position.z += radius * direction.x * 0.5f;
        AiRndrLine3d(vertices, nullptr, nullptr);
        if (flags & 0x40000000) {
            NuVecScale(&vertices[0].position, &direction, radius * 0.5f);
            vertices[0].position.y = 0.0f;
            NuVecAdd(&vertices[0].position, &vertices[0].position, &start);
            vertices[1].position.y = vertices[0].position.y;
            vertices[1].position.x = vertices[0].position.x - radius * direction.z * 0.5f;
            vertices[1].position.z = vertices[0].position.z + radius * direction.x * 0.5f;
            vertices[0].position.x += radius * direction.z * 0.5f;
            vertices[0].position.z -= radius * direction.x * 0.5f;
            AiRndrLine3d(vertices, nullptr, nullptr);
        } else if (flags & 0x80000000) {
            NUVEC centre = vertices[0].position;
            vertices[0].colour = colour / 2;
            vertices[1].colour = colour / 2;
            vertices[1].position.x = centre.x + radius * direction.z * 0.25f;
            vertices[1].position.y = centre.y;
            vertices[1].position.z = centre.z - radius * direction.x * 0.25f;
            AiRndrLine3d(vertices, nullptr, nullptr);
            vertices[1].position.x = centre.x - radius * direction.z * 0.25f;
            vertices[1].position.y = centre.y;
            vertices[1].position.z = centre.z + radius * direction.x * 0.25f;
            AiRndrLine3d(vertices, nullptr, nullptr);
        } else if (flags & 0x20000000) {
            NUVEC centre = vertices[0].position;
            NuVecScale(&vertices[0].position, &direction, radius);
            vertices[0].position.y = 0.0f;
            NuVecAdd(&vertices[0].position, &vertices[0].position, position);
            vertices[1].position.x = centre.x + radius * direction.z * 0.15f;
            vertices[1].position.y = centre.y;
            vertices[1].position.z = centre.z - radius * direction.x * 0.15f;
            AiRndrLine3d(vertices, nullptr, nullptr);
            vertices[1].position.x = centre.x - radius * direction.z * 0.25f;
            vertices[1].position.y = centre.y;
            vertices[1].position.z = centre.z + radius * direction.x * 0.25f;
            AiRndrLine3d(vertices, nullptr, nullptr);
            vertices[1].position.x = centre.x - radius * direction.z * 0.35f;
            vertices[1].position.y = centre.y;
            vertices[1].position.z = centre.z + radius * direction.x * 0.35f;
            AiRndrLine3d(vertices, nullptr, nullptr);
        }
    }
}

static void pathEditorDrawPath(EDAIPATH_s *path, i32 path_index) {
    u8 drawn_connections[255][32];
    NURND_VERTEX3D vertices[2];
    memset(drawn_connections, 0, sizeof(drawn_connections));
    i32 solid = aieditorsettings.solid_path_display && ((i16)aieditorsettings.current_mode == AIEDITOR_PATHS ||
                                                        (i16)aieditorsettings.current_mode == AIEDITOR_CREATURES ||
                                                        (i16)aieditorsettings.current_mode == AIEDITOR_LOCATORS);
    u32 active_route = 0;
    if ((i16)aieditorsettings.current_mode == AIEDITOR_ROUTES) {
        EDAIPATH_s *selected = aieditor->current_path;
        active_route = selected->current_route != nullptr ? 1ULL << (selected->current_route - selected->routes) : 1;
    }
    u32 colour =
        path == aieditor->current_path ? 0xffffffff : AISysGetPathColour(path_index % AISysGetPathColourCount());
    if (path != nullptr) {
        for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
             node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link)) {
            i32 first_index = node->index;
            i32 segments;
            u32 node_colour;
            if (__builtin_expect(path == aieditor->current_path, 0)) {
                if (node == path->current_node) {
                    node_colour = 0xff0000ff;
                    segments = 32;
                } else if (node == path->other_node) {
                    node_colour = 0xff00ffff;
                    segments = 32;
                } else {
                    node_colour = colour;
                    segments = 8;
                }
            } else {
                node_colour = colour;
                segments = 8;
            }
            pathEditorDrawNode(&node->position, node->radius, node->position.y + node->lower_height,
                               node->position.y + node->upper_height, node_colour, nullptr, segments, solid);
            if (node->shared_node != nullptr && !(node->shared_node->draw_flags & 1)) {
                f32 radius = node->radius;
                vertices[0].position = node->position;
                vertices[0].colour = node_colour;
                vertices[0].position.x += radius;
                vertices[1].position = node->position;
                vertices[1].colour = node_colour;
                vertices[1].position.x -= radius;
                AiRndrLine3d(vertices, nullptr, nullptr);
                vertices[0].position = node->position;
                vertices[0].position.z += radius;
                vertices[1].position = node->position;
                vertices[1].position.z -= radius;
                AiRndrLine3d(vertices, nullptr, nullptr);
                node->shared_node->draw_flags |= 1;
            }
            if (node->route_mask & active_route) {
                if (path == aieditor->current_path && node == path->current_node) {
                    pathEditorDrawNode(&node->position, node->radius * 0.9f, node->position.y + node->lower_height,
                                       node->position.y + node->upper_height, 0xff0000ff, nullptr, 32, solid);
                } else if (path == aieditor->current_path && node == path->other_node) {
                    pathEditorDrawNode(&node->position, node->radius * 0.9f, node->position.y + node->lower_height,
                                       node->position.y + node->upper_height, 0xff00ffff, nullptr, 32, solid);
                } else {
                    pathEditorDrawNode(&node->position, node->radius * 0.9f, node->position.y + node->lower_height,
                                       node->position.y + node->upper_height, colour, nullptr, 8, solid);
                }
            }
            for (i32 slot = 0; slot < 8; ++slot) {
                EDAIPATHNODE_s *other = node->connections[slot].node;
                if (other == nullptr) {
                    continue;
                }
                i32 second_index = other->index;
                u32 connection_colour = colour;
                if (__builtin_expect(node == path->current_node && other == path->other_node, 0)) {
                    connection_colour = 0xff00ffff;
                }
                pathEditorDrawConnectionInfo(&node->position, node->radius, &other->position,
                                             node->connections[slot].flags, connection_colour);
                if (drawn_connections[first_index][second_index / 8] & (1 << (second_index % 8))) {
                    continue;
                }
                drawn_connections[first_index][second_index / 8] |= 1 << (second_index % 8);
                drawn_connections[second_index][first_index / 8] |= 1 << (first_index % 8);
                u32 edge_colour = colour;
                if (path == aieditor->current_path) {
                    if (__builtin_expect((node == path->current_node && node->connections[slot].node == path->other_node) ||
                        (node == path->other_node && node->connections[slot].node == path->current_node), 1)) {
                        edge_colour = 0xff00ffff;
                    } else if (node == path->current_node || node->connections[slot].node == path->current_node) {
                        edge_colour = 0xff0000ff;
                    }
                }
                if (solid) {
                    NUVEC direction;
                    other = node->connections[slot].node;
                    f32 distance = NuVecXZDist(&node->position, &other->position, &direction);
                    if (distance > 0.0f) {
                        direction.x /= distance;
                        direction.z /= distance;
                    }
                    if (node->radius != other->radius) {
                        NUVEC side;
                        i16 angle = 0x4000 - NuASin((other->radius - node->radius) / distance);
                        NuVecRotateY(&side, &direction, angle);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * side.x;
                        vertices[0].position.z += node->radius * side.z;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * side.x;
                        vertices[1].position.z += other->radius * side.z;
                        vertices[0].position.y = node->position.y + node->lower_height;
                        vertices[1].position.y = other->position.y + other->lower_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position.y = node->position.y + node->upper_height;
                        vertices[1].position.y = other->position.y + other->upper_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        NuVecRotateY(&side, &direction, -angle);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * side.x;
                        vertices[0].position.z += node->radius * side.z;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * side.x;
                        vertices[1].position.z += other->radius * side.z;
                        vertices[0].position.y = node->position.y + node->lower_height;
                        vertices[1].position.y = other->position.y + other->lower_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position.y = node->position.y + node->upper_height;
                        vertices[1].position.y = other->position.y + other->upper_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                    } else {
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * direction.z;
                        vertices[0].position.z -= node->radius * direction.x;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * direction.z;
                        vertices[1].position.z -= other->radius * direction.x;
                        vertices[0].position.y = node->position.y + node->lower_height;
                        vertices[1].position.y = other->position.y + other->lower_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position.y = node->position.y + node->upper_height;
                        vertices[1].position.y = other->position.y + other->upper_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x -= node->radius * direction.z;
                        vertices[0].position.z += node->radius * direction.x;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x -= other->radius * direction.z;
                        vertices[1].position.z += other->radius * direction.x;
                        vertices[0].position.y = node->position.y + node->lower_height;
                        vertices[1].position.y = other->position.y + other->lower_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position.y = node->position.y + node->upper_height;
                        vertices[1].position.y = other->position.y + other->upper_height;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                    }
                } else {
                    NUVEC direction;
                    other = node->connections[slot].node;
                    f32 distance = NuVecXZDist(&node->position, &other->position, &direction);
                    if (__builtin_expect(distance > 0.0f, 0)) {
                        direction.x /= distance;
                        direction.z /= distance;
                    }
                    if (node->radius != other->radius) {
                        NUVEC side;
                        i16 angle = 0x4000 - NuASin((other->radius - node->radius) / distance);
                        NuVecRotateY(&side, &direction, angle);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * side.x;
                        vertices[0].position.z += node->radius * side.z;
                        vertices[0].position.y += aiEditor_DrawYOffset;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * side.x;
                        vertices[1].position.y += aiEditor_DrawYOffset;
                        vertices[1].position.z += other->radius * side.z;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        NuVecRotateY(&side, &direction, -angle);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * side.x;
                        vertices[0].position.z += node->radius * side.z;
                        vertices[0].position.y += aiEditor_DrawYOffset;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * side.x;
                        vertices[1].position.y += aiEditor_DrawYOffset;
                        vertices[1].position.z += other->radius * side.z;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                    } else {
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x += node->radius * direction.z;
                        vertices[0].position.y += aiEditor_DrawYOffset;
                        vertices[0].position.z -= node->radius * direction.x;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x += other->radius * direction.z;
                        vertices[1].position.y += aiEditor_DrawYOffset;
                        vertices[1].position.z -= other->radius * direction.x;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                        vertices[0].position = node->position;
                        vertices[0].colour = edge_colour;
                        vertices[0].position.x -= node->radius * direction.z;
                        vertices[0].position.y += aiEditor_DrawYOffset;
                        vertices[0].position.z += node->radius * direction.x;
                        vertices[1].position = other->position;
                        vertices[1].colour = edge_colour;
                        vertices[1].position.x -= other->radius * direction.z;
                        vertices[1].position.y += aiEditor_DrawYOffset;
                        vertices[1].position.z += other->radius * direction.x;
                        AiRndrLine3d(vertices, nullptr, nullptr);
                    }
                }
                u32 route_colour = 0xffff0000;
                if ((node == path->current_node && node->connections[slot].node == path->other_node) ||
                    (node == path->other_node && node->connections[slot].node == path->current_node)) {
                    route_colour = 0xff00ffff;
                }
                if (node->connections[slot].route_mask & active_route) {
                    NURND_VERTEX3D route_vertices[2];
                    route_vertices[0].position = node->position;
                    route_vertices[0].colour = route_colour;
                    route_vertices[0].position.y += aiEditor_DrawYOffset + 0.02f;
                    route_vertices[1].position = node->connections[slot].node->position;
                    route_vertices[1].colour = route_colour;
                    route_vertices[1].position.y += aiEditor_DrawYOffset + 0.02f;
                    AiRndrLine3d(route_vertices, nullptr, nullptr);
                }
            }
        }
    }
}

extern "C" {
    void pathEditorDrawPaths(void) {
        EDAIPATHWALL_s *wall = (EDAIPATHWALL_s *)NuLinkedListGetHead(&aieditor->path_walls);
        while (wall != nullptr) {
            wall->flags &= ~u8(1);
            wall = (EDAIPATHWALL_s *)NuLinkedListGetNext(&aieditor->path_walls, &wall->link);
        }

        if (aieditorsettings.draw_all_paths) {
            i32 index = 0;
            EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
            while (path != nullptr) {
                path->draw_index = index++;
                path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
            }
            if (aieditor->current_path != nullptr) {
                pathEditorDrawPath(aieditor->current_path, aieditor->current_path->draw_index);
            }
            path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
            while (path != nullptr) {
                if (path != aieditor->current_path) {
                    pathEditorDrawPath(path, path->draw_index);
                }
                path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
            }
        } else {
            pathEditorDrawPath(aieditor->current_path, 0);
        }
    }

    void pathEditor_CalcNodeIXs(void) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr) {
            i32 index = 0;
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                node->index = index++;
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
    }

    void pathEditor_UpdateNodesOnPlatforms(void) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr) {
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                if (NuSpecialExistsFn(&node->special)) {
                    NuVecMtxTransform(&node->position, &node->special_position, NuSpecialGetDrawMtx(&node->special));
                }
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
    }

    void pathEditor_QuickOnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *previous, EDAIPATHCHECK_s *result) {
        // Test the current edge, then the eight connections on each endpoint in slot order.
        result->on_path = 0;
        if (!TestPointPathCheck(point, previous->first, previous->second, &result->fraction, &result->width,
                                &result->angle, 0.0f)) {
            if (previous->first->connections[0].node == nullptr ||
                !TestPointPathCheck(point, previous->first->connections[0].node, previous->first, &result->fraction,
                                    &result->width, &result->angle, 0.0f)) {
                if (previous->second->connections[0].node == nullptr ||
                    !TestPointPathCheck(point, previous->second, previous->second->connections[0].node,
                                        &result->fraction, &result->width, &result->angle, 0.0f)) {
                    if (previous->first->connections[1].node == nullptr ||
                        !TestPointPathCheck(point, previous->first->connections[1].node, previous->first,
                                            &result->fraction, &result->width, &result->angle, 0.0f)) {
                        if (previous->second->connections[1].node == nullptr ||
                            !TestPointPathCheck(point, previous->second, previous->second->connections[1].node,
                                                &result->fraction, &result->width, &result->angle, 0.0f)) {
                            if (previous->first->connections[2].node == nullptr ||
                                !TestPointPathCheck(point, previous->first->connections[2].node, previous->first,
                                                    &result->fraction, &result->width, &result->angle, 0.0f)) {
                                if (previous->second->connections[2].node == nullptr ||
                                    !TestPointPathCheck(point, previous->second, previous->second->connections[2].node,
                                                        &result->fraction, &result->width, &result->angle, 0.0f)) {
                                    if (previous->first->connections[3].node == nullptr ||
                                        !TestPointPathCheck(point, previous->first->connections[3].node,
                                                            previous->first, &result->fraction, &result->width,
                                                            &result->angle, 0.0f)) {
                                        if (previous->second->connections[3].node == nullptr ||
                                            !TestPointPathCheck(
                                                point, previous->second, previous->second->connections[3].node,
                                                &result->fraction, &result->width, &result->angle, 0.0f)) {
                                            if (previous->first->connections[4].node == nullptr ||
                                                !TestPointPathCheck(point, previous->first->connections[4].node,
                                                                    previous->first, &result->fraction, &result->width,
                                                                    &result->angle, 0.0f)) {
                                                if (previous->second->connections[4].node == nullptr ||
                                                    !TestPointPathCheck(
                                                        point, previous->second, previous->second->connections[4].node,
                                                        &result->fraction, &result->width, &result->angle, 0.0f)) {
                                                    if (previous->first->connections[5].node == nullptr ||
                                                        !TestPointPathCheck(point, previous->first->connections[5].node,
                                                                            previous->first, &result->fraction,
                                                                            &result->width, &result->angle, 0.0f)) {
                                                        if (previous->second->connections[5].node == nullptr ||
                                                            !TestPointPathCheck(point, previous->second,
                                                                                previous->second->connections[5].node,
                                                                                &result->fraction, &result->width,
                                                                                &result->angle, 0.0f)) {
                                                            if (previous->first->connections[6].node == nullptr ||
                                                                !TestPointPathCheck(
                                                                    point, previous->first->connections[6].node,
                                                                    previous->first, &result->fraction, &result->width,
                                                                    &result->angle, 0.0f)) {
                                                                if (previous->second->connections[6].node == nullptr ||
                                                                    !TestPointPathCheck(
                                                                        point, previous->second,
                                                                        previous->second->connections[6].node,
                                                                        &result->fraction, &result->width,
                                                                        &result->angle, 0.0f)) {
                                                                    if (previous->first->connections[7].node ==
                                                                            nullptr ||
                                                                        !TestPointPathCheck(
                                                                            point, previous->first->connections[7].node,
                                                                            previous->first, &result->fraction,
                                                                            &result->width, &result->angle, 0.0f)) {
                                                                        if (previous->second->connections[7].node ==
                                                                                nullptr ||
                                                                            !TestPointPathCheck(
                                                                                point, previous->second,
                                                                                previous->second->connections[7].node,
                                                                                &result->fraction, &result->width,
                                                                                &result->angle, 0.0f)) {
                                                                            return;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        result->on_path = 1;
    }

    void pathEditor_OnPathCheck(nuvec_s *point, EDAIPATHCHECK_s *result, EDAIPATH_s *path, f32 tolerance) {
        u8 checked[255][32];
        memset(checked, 0, sizeof(checked));
        memset(result, 0, sizeof(*result));
        if (path == nullptr) {
            return;
        }
        EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
        while (node != nullptr) {
            const i32 node_index = node->index;
#define CHECK_PATH_CONNECTION(slot)                                                                                    \
    {                                                                                                                  \
        EDAIPATHNODE_s *other = node->connections[slot].node;                                                          \
        if (other != nullptr && !((checked[node_index][other->index / 8] >> (other->index % 8)) & 1)) {                \
            checked[node_index][other->index / 8] |= 1 << (other->index % 8);                                          \
            checked[other->index][node_index / 8] |= 1 << (node_index % 8);                                            \
            f32 fraction;                                                                                              \
            f32 width;                                                                                                 \
            i32 angle;                                                                                                 \
            if (TestPointPathCheck(point, node, other, &fraction, &width, &angle, tolerance)) {                        \
                result->fraction = fraction;                                                                           \
                result->angle = angle;                                                                                 \
                result->on_path = 1;                                                                                   \
                result->path = path;                                                                                   \
                result->first = node;                                                                                  \
                result->second = node->connections[slot].node;                                                         \
                result->width = width;                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
    }
            CHECK_PATH_CONNECTION(0);
            CHECK_PATH_CONNECTION(1);
            CHECK_PATH_CONNECTION(2);
            CHECK_PATH_CONNECTION(3);
            CHECK_PATH_CONNECTION(4);
            CHECK_PATH_CONNECTION(5);
            CHECK_PATH_CONNECTION(6);
            CHECK_PATH_CONNECTION(7);
#undef CHECK_PATH_CONNECTION
            node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
        }
    }

    void locatorEditor_PathNodeDeleted(EDAIPATHNODE_s *node) {
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            EDLOCATOR_s *next = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
            if (locator->first_node == node || locator->second_node == node) {
                pathEditor_OnPathCheck(&locator->position, (EDAIPATHCHECK_s *)locator->path_check,
                                       aieditor->current_path, 0.0f);
                if (!locator->on_path) {
                    DestroyLocator(locator);
                    if (aieditor->current_locator == locator) {
                        aieditor->current_locator = nullptr;
                    }
                }
            }
            locator = next;
        }
    }

    EDAIPATH_s *pathEditor_GetPath(const char *name) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (path != nullptr && name != nullptr) {
            if (NuStrICmp(name, path->name) == 0) {
                return path;
            }
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
        }
        return aieditor->current_path;
    }

}

void pathEditor_Enter(void) {
    auto find_connection = [](EDAIPATHNODE_s *node, EDAIPATHNODE_s *other) -> i32 {
        if (node->connections[0].node == other)
            return 0;
        if (node->connections[1].node == other)
            return 1;
        if (node->connections[2].node == other)
            return 2;
        if (node->connections[3].node == other)
            return 3;
        if (node->connections[4].node == other)
            return 4;
        if (node->connections[5].node == other)
            return 5;
        if (node->connections[6].node == other)
            return 6;
        if (__builtin_expect(node->connections[7].node == other, 0))
            return 7;
        return 8;
    };
    aieditor->paths.head = nullptr;
    aieditor->paths.tail = nullptr;
    for (i32 index = 0; index < 32; ++index) {
        NuLinkedListAppend(&aieditor->free_paths, &aieditor->path_storage[index].link);
    }
    aieditor->free_path_nodes.tail = nullptr;
    aieditor->free_path_nodes.head = nullptr;
    for (i32 index = 0; index < 1024; ++index) {
        NuLinkedListAppend(&aieditor->free_path_nodes, &aieditor->node_storage[index].link);
    }
    aieditor->free_shared_nodes.head = nullptr;
    aieditor->free_shared_nodes.tail = nullptr;
    for (i32 index = 0; index < 64; ++index) {
        NuLinkedListAppend(&aieditor->free_shared_nodes, &aieditor->shared_node_storage[index].link);
    }

    EDAISHAREDPATHNODE_s *shared_nodes[64];
    memset(shared_nodes, 0, sizeof(shared_nodes));
    AIPATHSYS_s *runtime_system = aieditor->ai_system != nullptr ? aieditor->ai_system->path_sys : nullptr;
    bool rebuild_paths = aieditor->cached_path_system != runtime_system;
    if (__builtin_expect(!!(rebuild_paths), 1)) {
        aieditor->cached_path_system = runtime_system;
    }
    if (rebuild_paths && runtime_system != nullptr && runtime_system->path_count != 0) {
        i32 node_storage_start = 0;
        for (i32 path_index = 0; path_index < runtime_system->path_count; ++path_index) {
            AIPATH_s *runtime_path = runtime_system->paths[path_index];
            EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
            if (__builtin_expect(path == nullptr, 0)) {
                break;
            }
            NuLinkedListRemove(&aieditor->free_paths, &path->link);
            NuLinkedListAppend(&aieditor->paths, &path->link);
            strcpy(path->name, runtime_path->name);
            if (__builtin_expect(NuStrICmp(path->name, "LevelPath") == 0, 0)) {
                path->flags |= 1;
            }
            for (i32 route_index = 0; route_index < runtime_path->route_count; ++route_index) {
                AIPATHROUTE_s *source = &runtime_path->routes[route_index];
                EDAIPATHROUTE_s *route = &path->routes[route_index];
                route->flags |= 1;
                NuStrCpy(route->name, source->name);
                route->user_mask = source->character_masks[1];
            }

            for (i32 node_index = 0; node_index < runtime_path->node_count; ++node_index) {
                AIPATHNODE_s *source = &runtime_path->nodes[node_index];
                if (path->node_count >= 254) {
                    break;
                }
                EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_path_nodes);
                if (__builtin_expect(node == nullptr, 0)) {
                    break;
                }
                NuLinkedListRemove(&aieditor->free_path_nodes, &node->link);
                NuLinkedListAppend(&path->nodes, &node->link);
                ++path->node_count;
                f32 source_height = source->position.y;
                node->position = source->position;
                node->radius = source->radius;
                node->lower_height = source->min_height - source_height;
                node->upper_height = source->max_height - source_height;
                node->flags = source->runtime_flags;
                node->special = source->special_handle;
                node->special_position = source->special_position;
                node->index = node_index;
                if (source->name != nullptr)
                    strcpy(node->name, source->name);
                if (source->special_route_index < runtime_path->special_route_count) {
                    AIPATHNODELINK_s *link = &runtime_path->special_routes[source->special_route_index];
                    i32 shared_index = link->special_route_index;
                    EDAISHAREDPATHNODE_s *shared = shared_nodes[shared_index];
                    if (shared == nullptr) {
                        shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_shared_nodes);
                        if (shared != nullptr) {
                            NuLinkedListRemove(&aieditor->free_shared_nodes, &shared->link);
                            NuLinkedListAppend(&aieditor->shared_path_nodes, &shared->link);
                            shared_nodes[shared_index] = shared;
                        }
                    }
                    if (__builtin_expect(shared != nullptr, 0)) {
                        if (node->shared_node != shared) {
                            if (__builtin_expect(node->shared_node != nullptr, 0)) {
                                EDAISHAREDPATHNODE_s *previous = node->shared_node;
                                --previous->reference_count;
                                if (previous->reference_count <= 1) {
                                    pathEditor_DestroySharedNode(previous);
                                }
                            }
                            node->shared_node = shared;
                            ++shared->reference_count;
                        }
                    }
                }
            }
            for (i32 node_index = 0; node_index < runtime_path->node_count; ++node_index) {
                EDAIPATHNODE_s *node = &aieditor->node_storage[node_storage_start + node_index];
                AIPATHNODE_s *source = &runtime_path->nodes[node_index];
                for (i32 edge_index = 0; edge_index < source->connection_count; ++edge_index) {
                    AIPATHCNX_s *connection = source->connections[edge_index];
                    i32 direction = connection->node_indices[0] != node_index;
                    i32 other_index = connection->node_indices[direction ^ 1];
                    EDAIPATHNODE_s *other = &aieditor->node_storage[node_storage_start + other_index];
                    if (node != nullptr && other != nullptr) {
                        i32 connection_slot = find_connection(node, other);
                        if (connection_slot == 8) {
                            i32 free_node_slot = -1;
                            i32 free_other_slot = -1;
#define FIND_FREE_OTHER(source_slot)                                                                                   \
    if (__builtin_expect(other->connections[0].node == nullptr, 0)) {                                                                       \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 0;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (__builtin_expect(other->connections[1].node == nullptr, 0)) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 1;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[2].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 2;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[3].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 3;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[4].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 4;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[5].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 5;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[6].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 6;                                                                                           \
        goto free_slots_found;                                                                                         \
    } else if (other->connections[7].node == nullptr) {                                                                \
        free_node_slot = source_slot;                                                                                  \
        free_other_slot = 7;                                                                                           \
        goto free_slots_found;                                                                                         \
    }
                            if (node->connections[0].node == nullptr) {
                                FIND_FREE_OTHER(0)
                            }
                            if (node->connections[1].node == nullptr) {
                                FIND_FREE_OTHER(1)
                            }
                            if (node->connections[2].node == nullptr) {
                                FIND_FREE_OTHER(2)
                            }
                            if (node->connections[3].node == nullptr) {
                                FIND_FREE_OTHER(3)
                            }
                            if (node->connections[4].node == nullptr) {
                                FIND_FREE_OTHER(4)
                            }
                            if (node->connections[5].node == nullptr) {
                                FIND_FREE_OTHER(5)
                            }
                            if (node->connections[6].node == nullptr) {
                                FIND_FREE_OTHER(6)
                            }
                            if (node->connections[7].node == nullptr) {
                                FIND_FREE_OTHER(7)
                            }
#undef FIND_FREE_OTHER
                            continue;
                        free_slots_found:
                            EDAIPATHCNX_s *editor_connection = &node->connections[free_node_slot];
                            EDAIPATHCNX_s *reciprocal = &other->connections[free_other_slot];
                            editor_connection->node = other;
                            editor_connection->flags = 0;
                            reciprocal->node = node;
                            reciprocal->flags = 0;
                        }
                        connection_slot = find_connection(node, other);
                        if (connection_slot == 8)
                            continue;
                        EDAIPATHCNX_s *editor_connection = &node->connections[connection_slot];
                        editor_connection->flags = connection->original_traversal_flags[direction];
                        if (editor_connection->flags & 0x10000000)
                            editor_connection->flags &= ~u32(0x10000000);
                        editor_connection->route_mask |= connection->route_mask;
                    }
                }
                node->route_mask = source->route_boundary_mask;
            }
            node_storage_start += runtime_path->node_count;
        }
    }
    if (runtime_system == nullptr) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
        if (path != nullptr) {
            NuLinkedListRemove(&aieditor->free_paths, &path->link);
            NuLinkedListAppend(&aieditor->paths, &path->link);
            strcpy(path->name, "LevelPath");
            path->flags |= 1;
        }
    }
    if (__builtin_expect(aieditorsettings.current_path_name[0] != '\0', 0)) {
        aieditor->current_path = pathEditor_GetPath(aieditorsettings.current_path_name);
    } else {
        aieditor->current_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
    }
    pathEditor_CalcNodeIXs();
    EDAIPATH_s *path = aieditor->current_path;
    if (path != nullptr) {
        path->current_route = &path->routes[0];
        if (path->routes[0].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[1];
        if (path->routes[1].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[2];
        if (path->routes[2].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[3];
        if (path->routes[3].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[4];
        if (path->routes[4].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[5];
        if (path->routes[5].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[6];
        if (path->routes[6].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[7];
        if (path->routes[7].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[8];
        if (path->routes[8].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[9];
        if (path->routes[9].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[10];
        if (path->routes[10].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[11];
        if (path->routes[11].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[12];
        if (path->routes[12].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[13];
        if (path->routes[13].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[14];
        if (path->routes[14].flags & 1)
            goto route_selected;
        path->current_route = &path->routes[15];
        if ((path->current_route->flags & 1) == 0)
            path->current_route = nullptr;
    route_selected:;
    }
}

void locatorEditor_Enter(void) {
    AIEDITOR_RENDER_STATE *const *state = &aieditor;
    memset(&(*state)->locators, 0, 0x48);
    for (i32 index = 0; index < 256; ++index) {
        NuLinkedListAppend(&(*state)->free_locators, &(*state)->locator_pool[index].link);
    }
    memset(&(*state)->locator_sets, 0, 0x118);
    for (i32 index = 0; index < 64; ++index) {
        NuLinkedListAppend(&(*state)->free_locator_sets, &(*state)->locator_set_pool[index].link);
    }
    if ((*state)->ai_system != nullptr) {
        for (i32 index = 0; index < (*state)->ai_system->locator_count; ++index) {
            AILOCATOR *source = &(*state)->ai_system->locators[index];
            EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&(*state)->free_locators);
            if (locator != nullptr) {
                NuLinkedListRemove(&(*state)->free_locators, &locator->link);
                NuLinkedListAppend(&(*state)->locators, &locator->link);
                locator->position = source->position;
                locator->direction = source->direction;
            }
            strcpy(locator->name, source->name);
            EDAIPATH_s *path = pathEditor_GetPath((const char *)source->path_info.path);
            f32 tolerance = 0.0f;
            do {
                pathEditor_OnPathCheck(&locator->position, (EDAIPATHCHECK_s *)locator->path_check, path, tolerance);
                tolerance += 0.01f;
            } while (!locator->on_path);
            locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
        }
        for (i32 index = 0; index < (*state)->ai_system->locator_set_count; ++index) {
            AILOCATORSET *source = &(*state)->ai_system->locator_sets[index];
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&(*state)->free_locator_sets);
            if (__builtin_expect(set == nullptr, 0)) {
                continue;
            }
            NuLinkedListRemove(&(*state)->free_locator_sets, &set->link);
            memset(set, 0, sizeof(*set));
            NuLinkedListAppend(&(*state)->locator_sets, &set->link);
            strcpy(set->name, source->name);
            for (i32 member = 0; member < source->locator_count; ++member) {
                set->locators[member] = &(*state)->locator_pool[source->locator_entries[member]];
            }
        }
    }
    AIEDITOR_RENDER_STATE *context = *state;
    if (context->current_locator_set != nullptr) {
        strcpy(aieditorsettings.current_route_name, context->current_locator_set->name);
    }
    if (aieditorsettings.current_route_name[0] == 0) {
        context->current_locator_set = nullptr;
        return;
    }
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&context->locator_sets);
    while (set != nullptr && NuStrICmp(aieditorsettings.current_route_name, set->name) != 0) {
        set = (EDLOCATORSET_s *)NuLinkedListGetNext(&(*state)->locator_sets, &set->link);
    }
    context->current_locator_set = set;
}

extern "C" {
    AIPATHSYS_s *pathEditorCreateData(VARIPTR *cursor, VARIPTR *end, void **scratch_cursor, void **scratch_end) {
        iterator_count = 0;
        VARIPTR scratch;
        scratch.void_ptr = *scratch_cursor;
        if (aieditor->current_path == nullptr) {
            distance_tables = nullptr;
            return nullptr;
        }
        EDAIPATH_s *first_editor_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        if (__builtin_expect(first_editor_path == nullptr, 0)) {
            distance_tables = nullptr;
            return nullptr;
        }

        i32 path_count = 0;
        for (EDAIPATH_s *path = first_editor_path; path != nullptr;
             path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
            path->draw_index = path_count++;
        }
        AIPATHSYS_s *system = (AIPATHSYS_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATHSYS_s));
        if (system == nullptr) {
            distance_tables = nullptr;
            return nullptr;
        }
        memset(system, 0, sizeof(*system));
        system->path_count = path_count;
        distance_tables = nullptr;
        VARIPTR *scratch_limit = reinterpret_cast<VARIPTR *>(scratch_end);
        if (system->path_count != 0) {
            distance_tables = (f32 ***)AISysBufferAlloc(&scratch, scratch_limit, system->path_count * sizeof(f32 **));
            memset(distance_tables, 0, system->path_count * sizeof(f32 **));
        }

        system->special_route_count = 0;
        for (EDAISHAREDPATHNODE_s *shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
             shared != nullptr;
             shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
            shared->runtime_index = system->special_route_count++;
        }
        if (system->special_route_count != 0) {
            system->special_routes = (AIPATHSPECIALROUTE_s *)AISysBufferAlloc(
                &scratch, scratch_limit, system->special_route_count * sizeof(AIPATHSPECIALROUTE_s));
            memset(system->special_routes, 0, system->special_route_count * sizeof(AIPATHSPECIALROUTE_s));
            for (EDAISHAREDPATHNODE_s *shared =
                     (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->shared_path_nodes);
                 shared != nullptr;
                 shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetNext(&aieditor->shared_path_nodes, &shared->link)) {
                AIPATHSPECIALROUTE_s *route = &system->special_routes[shared->runtime_index];
                route->paths =
                    (AIPATH_s **)AISysBufferAlloc(&scratch, scratch_limit, shared->reference_count * sizeof(AIPATH_s));
                memset(route->paths, 0, shared->reference_count * sizeof(AIPATH_s));
            }
        }
        system->paths = (AIPATH_s **)AISysBufferAlloc(cursor, end, system->path_count * sizeof(AIPATH_s *));
        if (system->paths == nullptr) {
            distance_tables = nullptr;
            return system;
        }
        memset(system->paths, 0, system->path_count * sizeof(AIPATH_s *));
        i32 path_index = 0;
        for (EDAIPATH_s *editor_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); editor_path != nullptr;
             editor_path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &editor_path->link), ++path_index) {
            system->paths[path_index] = (AIPATH_s *)AISysBufferAlloc(cursor, end, sizeof(AIPATH_s));
            AIPATH_s *path = system->paths[path_index];
            if (path == nullptr) {
                continue;
            }
            memset(path, 0, sizeof(*path));
            strcpy(path->name, editor_path->name);
            path->index = path_index;

            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                node->index = path->node_count++;
                for (i32 slot = 0; slot < 8; ++slot) {
                    if (node->connections[slot].node != nullptr) {
                        ++path->connection_count;
                    }
                }
                if (node->shared_node != nullptr) {
                    ++path->special_route_count;
                }
            }
            if (path->connection_count != 0) {
                path->connection_count /= 2;
                path->connections =
                    (AIPATHCNX_s *)AISysBufferAlloc(cursor, end, path->connection_count * sizeof(AIPATHCNX_s));
                if (path->connections != nullptr) {
                    memset(path->connections, 0, path->connection_count * sizeof(AIPATHCNX_s));
                }
            }
            if (path->special_route_count != 0) {
                path->special_routes = (AIPATHNODELINK_s *)AISysBufferAlloc(
                    cursor, end, path->special_route_count * sizeof(AIPATHNODELINK_s));
                memset(path->special_routes, 0, path->special_route_count * sizeof(AIPATHNODELINK_s));
                path->special_route_count = 0;
            }
            if (path->node_count != 0) {
                path->nodes = (AIPATHNODE_s *)AISysBufferAlloc(cursor, end, path->node_count * sizeof(AIPATHNODE_s));
                if (path->nodes != nullptr) {
                    memset(path->nodes, 0, path->node_count * sizeof(AIPATHNODE_s));

                    i32 connection_index = 0;
                    for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&editor_path->nodes);
                         node != nullptr;
                         node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&editor_path->nodes, &node->link)) {
                        AIPATHNODE_s *runtime = &path->nodes[node->index];
                        i32 length = strlen(node->name);
                        if (length != 0) {
                            runtime->name = (char *)AISysBufferAlloc(cursor, end, length + 1);
                            strcpy(runtime->name, node->name);
                        }
                        runtime->position = node->position;
                        runtime->radius = node->radius;
                        runtime->radius_squared = node->radius * node->radius;
                        runtime->runtime_flags = node->flags;
                        runtime->special_handle = node->special;
                        if (NuSpecialExistsFn(&runtime->special_handle)) {
                            runtime->has_special = 1;
                        }
                        runtime->special_position = node->special_position;
                        runtime->min_height = runtime->position.y + node->lower_height;
                        runtime->max_height = runtime->position.y + node->upper_height;
                        runtime->min_height_offset = runtime->min_height - runtime->position.y;
                        runtime->max_height_offset = runtime->max_height - runtime->position.y;
                        for (i32 slot = 0; slot < 8; ++slot) {
                            if (__builtin_expect(node->connections[slot].node != nullptr, 1)) {
                                ++runtime->connection_count;
                            }
                        }
                        if (__builtin_expect(path->connections != nullptr, 1)) {
                            if (runtime->connection_count != 0) {
                                runtime->connections = (AIPATHCNX_s **)AISysBufferAlloc(
                                    cursor, end, runtime->connection_count * sizeof(AIPATHCNX_s *));
                                if (runtime->connections != nullptr) {
                                    memset(runtime->connections, 0, runtime->connection_count * sizeof(AIPATHCNX_s *));
                                }
                                i32 next = 0;
                                for (i32 slot = 0; slot < 8; ++slot) {
                                    EDAIPATHCNX_s *editor_connection = &node->connections[slot];
                                    EDAIPATHNODE_s *other = editor_connection->node;
                                    if (other == nullptr) {
                                        continue;
                                    }
                                    if (other->index < node->index) {
                                        AIPATHNODE_s *other_runtime = &path->nodes[other->index];
                                        for (i32 reverse = 0; reverse < other_runtime->connection_count; ++reverse) {
                                            AIPATHCNX_s *connection = other_runtime->connections[reverse];
                                            if (connection->node_indices[1] == node->index) {
                                                runtime->connections[next] = connection;
                                                runtime->connections[next]->traversal_flags[1] =
                                                    editor_connection->flags;
                                                runtime->connections[next]->original_traversal_flags[1] =
                                                    editor_connection->flags;
                                                break;
                                            }
                                        }
                                    } else {
                                        runtime->connections[next] = &path->connections[connection_index++];
                                        runtime->connections[next]->node_indices[0] = node->index;
                                        runtime->connections[next]->node_indices[1] = editor_connection->node->index;
                                        AIPATHCNX_s *connection = runtime->connections[next];
                                        connection->traversal_flags[0] = editor_connection->flags;
                                        connection->original_traversal_flags[0] = editor_connection->flags;
                                        NUVEC difference;
                                        connection->distance =
                                            NuVecDist(&editor_connection->node->position, &node->position, &difference);
                                        runtime->connections[next]->horizontal_distance = NuVecXZDist(
                                            &editor_connection->node->position, &node->position, &difference);
                                        connection = runtime->connections[next];
                                        if (connection->horizontal_distance == 0.0f) {
                                            connection->horizontal_distance = 0.0001f;
                                        }
                                        connection->rotation = (i16)(NuAtan2(difference.x, difference.z) * 10430.378f);
                                    }
                                    ++next;
                                }
                            }
                        }
                        runtime->distance_cache_nodes[0] = 0xff;
                        runtime->distance_cache_nodes[1] = 0xff;
                        if (node->shared_node != nullptr) {
                            AIPATHSPECIALROUTE_s *shared_route =
                                &system->special_routes[node->shared_node->runtime_index];
                            AIPATHNODELINK_s *link =
                                &path->special_routes[runtime->special_route_index = path->special_route_count++];
                            shared_route->paths[shared_route->path_count++] = path;
                            link->node_index = node->index;
                            link->special_route_index = node->shared_node->runtime_index;
                        } else {
                            runtime->special_route_index = 0xff;
                        }
                    }
                }
            }
            distance_tables[path_index] = pathEditorCalculateDistanceTable(path, 0, &scratch, scratch_limit);

            if (path->node_count != 0) {
                path->route_matrix = (u8 **)AISysBufferAlloc(cursor, end, path->node_count * sizeof(u8 *));
                memset(path->route_matrix, 0, path->node_count * sizeof(u8 *));
                {
                    for (i32 source = 0; source < path->node_count; ++source) {
                        path->route_matrix[source] = (u8 *)AISysBufferAlloc(cursor, end, path->node_count);
                        memset(path->route_matrix[source], 0, path->node_count);
                    }
                }
                u8 **route_matrix = path->route_matrix;
                f32 **distances = distance_tables[path_index];
                if (distance_tables != nullptr && path->nodes != nullptr && distances != nullptr) {
                    for (i32 source = 0; source < path->node_count; ++source) {
                        AIPATHNODE_s *node = &path->nodes[source];
                        for (i32 destination = 0; destination < path->node_count; ++destination) {
                            route_matrix[source][destination] = 0xff;
                            if (destination == source) {
                                continue;
                            }
                            f32 best = FLT_MAX;
                            for (i32 edge = 0; edge < node->connection_count; ++edge) {
                                AIPATHCNX_s *connection = node->connections[edge];
                                bool direction = connection->node_indices[0] != source;
                                if (__builtin_expect(!!(connection->traversal_flags[direction] & 0x40000000), 0)) {
                                    continue;
                                }
                                i32 neighbor = connection->node_indices[!direction];
                                f32 distance = distances[source][neighbor] + distances[neighbor][destination];
                                if (__builtin_expect(distance < best, 1)) {
                                    best = distance;
                                    route_matrix[source][destination] = edge;
                                }
                            }
                        }
                    }
                }
                pathEditorCreateSpecialRouteData(path, editor_path, &scratch, scratch_limit);
            }
        }
        if (system->path_count > 1) {
            AIPATHINFO info;
            AIPATH_s *first_path = system->paths[0];
            memset(&info, 0, sizeof(info));
            for (i32 path_index = 1; path_index < system->path_count; ++path_index) {
                AIPATH_s *path = system->paths[path_index];
                for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                    AIPATHNODE_s *node = &path->nodes[node_index];
                    AISysGetPathPos(aieditor->ai_system, &node->position, &info, first_path, 0xff);
                    if (info.on_path) {
                        node->path_flags = info.connection - path->connections;
                        if (info.dist > 0.5f || (info.direction != 0 && info.dist < 0.5f)) {
                            node->runtime_flags |= 4;
                        }
                        node->runtime_flags |= 1;
                        node->path_flags = info.connection - info.path->connections;
                        path->flags |= 2;
                    } else {
                        node->path_flags = -1;
                        node->runtime_flags &= ~u8(1);
                    }
                }
                if (__builtin_expect(!(path->flags & 2) || distance_tables == nullptr ||
                                         distance_tables[path_index] == nullptr,
                                     0)) {
                    continue;
                }
                for (i32 node_index = 0; node_index < path->node_count; ++node_index) {
                    AIPATHNODE_s *node = &path->nodes[node_index];
                    if (node->runtime_flags & 1) {
                        continue;
                    }
                    f32 nearest = FLT_MAX;
                    for (i32 candidate = 0; candidate < path->node_count; ++candidate) {
                        if (&path->nodes[candidate] != node && (path->nodes[candidate].runtime_flags & 1) &&
                            distance_tables[path_index][node_index][candidate] < nearest) {
                            nearest = distance_tables[path_index][node_index][candidate];
                            node->path_flags = candidate;
                        }
                    }
                }
            }
        }
        distance_tables = nullptr;
        return system;
    }

    void AISYSRebuildFromEditorData(void) {
        AISYS *system = aieditor->ai_system;
        if (system == NULL) {
            return;
        }

        i32 storage_size = system->storage_size;
        memset(system->storage, 0, storage_size);
        system = aieditor->ai_system;
        *reinterpret_cast<volatile i32 *>(&system->storage_size) = storage_size;
        system->storage_cursor.addr = reinterpret_cast<usize>(system->storage);
        system->path_sys =
            pathEditorCreateData(&system->storage_cursor, &system->storage_end, &aieditorsettings.external_display_a,
                                 &aieditorsettings.external_display_b);
        system = aieditor->ai_system;
        memset(aieditor->ai_system->groups, 0, sizeof(aieditor->ai_system->groups));
        NULISTLNK *area_head =
            NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40));
        aieditor->ai_system->area_count = 0;
        for (NULISTLNK *area = area_head; area != NULL;
             area =
                 NuLinkedListGetNext(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40), area)) {
            ++aieditor->ai_system->area_count;
        }
        if (aieditor->ai_system->area_count != 0) {
            aieditor->ai_system->areas = static_cast<AIAREA *>(
                AISysBufferAlloc(&aieditor->ai_system->storage_cursor, &aieditor->ai_system->storage_end,
                                 aieditor->ai_system->area_count * sizeof(AIAREA)));
            i32 index = 0;
            for (NULISTLNK *area =
                     NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40));
                 area != NULL; area = NuLinkedListGetNext(
                                   reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x37a40), area)) {
                AIAREA *runtime = &aieditor->ai_system->areas[index++];
                memcpy(runtime, reinterpret_cast<u8 *>(area) + sizeof(NULISTLNK), sizeof(AIAREA));
                runtime->system = aieditor->ai_system;
            }
        }
        if (aieditor->ai_system->path_sys != NULL) {
            EDLOCATOR_s *locator_head = reinterpret_cast<EDLOCATOR_s *>(NuLinkedListGetHead(&aieditor->locators));
            aieditor->ai_system->locator_count = 0;
            for (EDLOCATOR_s *locator = locator_head; locator != NULL;
                 locator = reinterpret_cast<EDLOCATOR_s *>(NuLinkedListGetNext(&aieditor->locators, &locator->link))) {
                if (locator->path != NULL) {
                    ++aieditor->ai_system->locator_count;
                }
            }
            if (aieditor->ai_system->locator_count != 0) {
                aieditor->ai_system->locators = static_cast<AILOCATOR *>(
                    AISysBufferAlloc(&aieditor->ai_system->storage_cursor, &aieditor->ai_system->storage_end,
                                     aieditor->ai_system->locator_count * sizeof(AILOCATOR)));
                i32 index = 0;
                for (EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(NuLinkedListGetHead(&aieditor->locators));
                     locator != NULL; locator = reinterpret_cast<EDLOCATOR_s *>(
                                          NuLinkedListGetNext(&aieditor->locators, &locator->link))) {
                    locator->runtime_index = 0xff;
                    if (locator->path == NULL) {
                        continue;
                    }
                    locator->runtime_index = index;
                    AILOCATOR *runtime = &aieditor->ai_system->locators[index++];
                    strcpy(runtime->name, locator->name);
                    runtime->position = locator->position;
                    runtime->direction = locator->direction;
                    runtime->locator_flags = locator->path_angle;
                    runtime->path_info.path = aieditor->ai_system->path_sys->paths[locator->path->draw_index];
                    runtime->path_info.dist = locator->path_fraction;
                    runtime->path_info.width = locator->path_width;
                    AIPATH *path = runtime->path_info.path;
                    if (path != NULL && path->connection_count != 0) {
                        for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                            AIPATHCNX *connection = &path->connections[connection_index];
                            i32 first = locator->first_node->index;
                            i32 second = locator->second_node->index;
                            i32 angle = locator->path_angle;
                            i32 magnitude = angle < 0 ? -angle : angle;
                            if (connection->node_indices[0] == first && connection->node_indices[1] == second) {
                                runtime->path_info.connection = connection;
                                runtime->path_info.direction = magnitude > 0x3fff;
                                break;
                            }
                            if (connection->node_indices[0] == second && connection->node_indices[1] == first) {
                                runtime->path_info.connection = connection;
                                runtime->path_info.dist = 1.0f - locator->path_fraction;
                                runtime->path_info.width = -locator->path_width;
                                runtime->path_info.direction = magnitude <= 0x3fff;
                                break;
                            }
                        }
                    }
                }
            }
            aieditor->ai_system->locator_set_count = 0;
            for (EDLOCATORSET_s *set = reinterpret_cast<EDLOCATORSET_s *>(NuLinkedListGetHead(&aieditor->locator_sets));
                 set != NULL;
                 set = reinterpret_cast<EDLOCATORSET_s *>(NuLinkedListGetNext(&aieditor->locator_sets, &set->link))) {
                ++aieditor->ai_system->locator_set_count;
            }
            if (aieditor->ai_system->locator_set_count != 0) {
                // The original reserves 0x3c bytes per set, but indexes records at the 0x1c-byte stride.
                aieditor->ai_system->locator_sets = static_cast<AILOCATORSET *>(
                    AISysBufferAlloc(&aieditor->ai_system->storage_cursor, &aieditor->ai_system->storage_end,
                                     aieditor->ai_system->locator_set_count * 0x3c));
                // Retail skips locator-set population when the buffer allocation fails.
                if (aieditor->ai_system->locator_sets != NULL) {
                    i32 index = 0;
                    for (EDLOCATORSET_s *set =
                             reinterpret_cast<EDLOCATORSET_s *>(NuLinkedListGetHead(&aieditor->locator_sets));
                         set != NULL; set = reinterpret_cast<EDLOCATORSET_s *>(
                                          NuLinkedListGetNext(&aieditor->locator_sets, &set->link))) {
                        AILOCATORSET *runtime = &aieditor->ai_system->locator_sets[index++];
                        strcpy(runtime->name, set->name);
                        runtime->locator_count = 0;
                        for (i32 member = 0; member < 64 && set->locators[member] != NULL; ++member) {
                            if (set->locators[member]->runtime_index != 0xff) {
                                ++runtime->locator_count;
                            }
                        }
                        if (runtime->locator_count != 0) {
                            runtime->locator_entries = static_cast<u8 *>(
                                AISysBufferAlloc(&aieditor->ai_system->storage_cursor,
                                                 &aieditor->ai_system->storage_end, runtime->locator_count));
                            for (i32 member = 0; member < runtime->locator_count; ++member) {
                                if (set->locators[member]->runtime_index != 0xff) {
                                    runtime->locator_entries[member] = set->locators[member]->runtime_index;
                                }
                            }
                            runtime->assigned = static_cast<u8 *>(AISysBufferAlloc(&aieditor->ai_system->storage_cursor,
                                                                                   &aieditor->ai_system->storage_end,
                                                                                   runtime->locator_count));
                            memset(runtime->assigned, 0, runtime->locator_count);
                        }
                    }
                }
            }
            RebuildEditorCreature *creature_head =
                reinterpret_cast<RebuildEditorCreature *>(NuLinkedListGetHead(&aieditor->creatures));
            aieditor->ai_system->creature_count = 0;
            for (RebuildEditorCreature *creature = creature_head; creature != NULL;
                 creature = reinterpret_cast<RebuildEditorCreature *>(
                     NuLinkedListGetNext(&aieditor->creatures, &creature->link))) {
                ++aieditor->ai_system->creature_count;
            }
            if (aieditor->ai_system->creature_count != 0) {
                aieditor->ai_system->creatures = static_cast<AICREATURE *>(
                    AISysBufferAlloc(&aieditor->ai_system->storage_cursor, &aieditor->ai_system->storage_end,
                                     aieditor->ai_system->creature_count * sizeof(AICREATURE)));
                i32 index = 0;
                for (RebuildEditorCreature *creature =
                         reinterpret_cast<RebuildEditorCreature *>(NuLinkedListGetHead(&aieditor->creatures));
                     creature != NULL; creature = reinterpret_cast<RebuildEditorCreature *>(
                                           NuLinkedListGetNext(&aieditor->creatures, &creature->link))) {
                    AICREATURE *runtime = &aieditor->ai_system->creatures[index++];
                    strcpy(runtime->name, creature->name);
                    strcpy(runtime->script_name, creature->script_name);
                    runtime->type = creature->character_type;
                    runtime->set = creature->set;
                    runtime->count = creature->group_count;
                    runtime->count_across = creature->across_count;
                    runtime->active_mask = creature->valid_positions;
                    runtime->x_spacing = creature->x_spacing;
                    runtime->z_spacing = creature->z_spacing;
                    runtime->flags = creature->flags;
                    runtime->pos = creature->position;
                    runtime->y_rot = creature->angle;
                    runtime->path_info.path =
                        aieditor->ai_system->path_sys->paths[creature->path_check.path->draw_index];
                    runtime->path_info.dist = creature->path_check.fraction;
                    runtime->path_info.width = creature->path_check.width;
                    for (i32 param = 0; param < 4; ++param) {
                        runtime->script_params[param] = creature->script_params[param];
                    }

                    AIPATH *path = runtime->path_info.path;
                    if (path != NULL && path->connection_count != 0) {
                        for (i32 connection_index = 0; connection_index < path->connection_count; ++connection_index) {
                            AIPATHCNX *connection = &path->connections[connection_index];
                            i32 first = creature->path_check.first->index;
                            i32 second = creature->path_check.second->index;
                            i32 angle = creature->path_check.angle;
                            i32 magnitude = angle < 0 ? -angle : angle;
                            if (connection->node_indices[0] == first && connection->node_indices[1] == second) {
                                runtime->path_info.connection = connection;
                                runtime->path_info.direction = magnitude > 0x3fff;
                                break;
                            }
                            if (connection->node_indices[0] == second && connection->node_indices[1] == first) {
                                runtime->path_info.connection = connection;
                                runtime->path_info.dist = 1.0f - creature->path_check.fraction;
                                runtime->path_info.width = -creature->path_check.width;
                                runtime->path_info.direction = magnitude <= 0x3fff;
                                break;
                            }
                        }
                    }

                    if (creature->trigger_area != NULL) {
                        for (i32 area_index = 0; area_index < aieditor->ai_system->area_count; ++area_index) {
                            AIAREA *area = &aieditor->ai_system->areas[area_index];
                            if (NuStrICmp(creature->trigger_area->name, area->name) == 0) {
                                runtime->area = area;
                                break;
                            }
                        }
                    }
                    if (creature->locator != NULL) {
                        for (i32 locator_index = 0; locator_index < aieditor->ai_system->locator_count;
                             ++locator_index) {
                            AILOCATOR *locator = &aieditor->ai_system->locators[locator_index];
                            if (NuStrICmp(creature->locator->name, locator->name) == 0) {
                                runtime->locator = locator;
                                break;
                            }
                        }
                    }
                    if (creature->respawn_locator != NULL) {
                        for (i32 locator_index = 0; locator_index < aieditor->ai_system->locator_count;
                             ++locator_index) {
                            AILOCATOR *locator = &aieditor->ai_system->locators[locator_index];
                            if (NuStrICmp(creature->respawn_locator->name, locator->name) == 0) {
                                runtime->respawn_locator = locator;
                                break;
                            }
                        }
                    }
                    runtime->activation_difficulty = creature->difficulty;
                    runtime->min_respawn_count = creature->min_respawns;
                    runtime->max_respawn_count = creature->max_respawns;
                    runtime->min_respawn_time = creature->min_respawn_time;
                    runtime->max_respawn_time = creature->max_respawn_time;
                    runtime->start_stagger = creature->stagger_start;
                    runtime->view_distance = creature->view_distance;
                    runtime->hear_distance = creature->hear_distance;
                    runtime->max_view_height = creature->max_view_height;
                    runtime->min_view_height = creature->negative_min_view_height;
                    runtime->activate_type = creature->activation;
                    if (creature->activation == 1) {
                        runtime->activate_type = 0;
                        if (creature->activation_area != NULL) {
                            for (i32 area_index = 0; area_index < aieditor->ai_system->area_count; ++area_index) {
                                AIAREA *area = &aieditor->ai_system->areas[area_index];
                                if (NuStrICmp(creature->activation_area->name, area->name) == 0) {
                                    runtime->activate_area = area;
                                    runtime->activate_type = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if (GameAISYSRebuildFromEditorDataFn != NULL) {
                GameAISYSRebuildFromEditorDataFn(aieditor->ai_system, &aieditor->ai_system->storage_cursor,
                                                 &aieditor->ai_system->storage_end);
            }
        }
        system = aieditor->ai_system;
        aieditor->ai_system->antinode_count = 0;
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(
                 NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94)));
             node != NULL;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            ++aieditor->ai_system->antinode_count;
        }
        if (aieditor->ai_system->antinode_count != 0) {
            aieditor->ai_system->antinodes = static_cast<AIANTINODE *>(
                AISysBufferAlloc(&aieditor->ai_system->storage_cursor, &aieditor->ai_system->storage_end,
                                 aieditor->ai_system->antinode_count * sizeof(AIANTINODE)));
            i32 index = 0;
            for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(
                     NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94)));
                 node != NULL;
                 node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                     reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
                AIANTINODE *runtime = &aieditor->ai_system->antinodes[index++];
                runtime->enabled = 1;
                runtime->position = node->position;
                runtime->radius = node->radius;
                runtime->game_flags = node->game_flags;
                runtime->special_handle = node->special;
                if (NuSpecialExistsFn(&runtime->special_handle)) {
                    runtime->has_special = 1;
                }
                runtime->min_y = node->position.y + node->lower_height;
                runtime->min_y_offset = runtime->min_y - node->position.y;
                runtime->max_y = node->position.y + node->upper_height;
                runtime->max_y_offset = runtime->max_y - node->position.y;
                runtime->special_position = node->special_position;
                runtime->flags = node->flags;
                runtime->rotation_offset = node->rotation_offset;
                runtime->base_radius = node->base_radius;
                runtime->base_height = node->base_height;
                runtime->type = node->type;
                if (runtime->type == 1) {
                    runtime->radius =
                        runtime->base_radius > runtime->base_height ? runtime->base_radius : runtime->base_height;
                } else if (runtime->type == 2) {
                    runtime->radius = NuFsqrt(runtime->base_radius * runtime->base_radius +
                                              runtime->base_height * runtime->base_height);
                }
            }
        }
        AISysSetLevelPath(aieditor->ai_system, NULL);
    }

    void aieditor_Leave(void) {
        edmainGetCamera()->near_clip = 0.15f;
        for (i32 index = 0; index < aieditorsettings.mode_count; ++index) {
            void (*leave)() = aieditorsettings.modes[index].leave;
            if (leave != nullptr) {
                leave();
            }
        }
        AIEDITOR_RENDER_STATE *state = aieditor;
        if (state->mode_selection_36930 != nullptr) {
            strcpy(aieditorsettings.current_area_name, state->mode_selection_36930->name);
        }
        if (state->current_path != nullptr) {
            strcpy(aieditorsettings.current_path_name, state->current_path->name);
        }
        if (state->current_route != nullptr) {
            strcpy(aieditorsettings.current_route_name, state->current_route->name);
        }
        AISYSRebuildFromEditorData();
        AIScriptInitConditions(aieditor->ai_system);
        aieditor->flags &= ~u8(2);
    }

    void pathEditorSaveData(AIPATHSYS_s *system) {
        EdFileWriteInt(system->path_count);
        for (i32 path_index = 0; path_index < system->path_count; ++path_index) {
            AIPATH_s *path = system->paths[path_index];
            EdFileWrite(path->name, 16);
            EdFileWriteChar(path->node_count);
            EdFileWriteChar(path->flags);
            EdFileWriteShort(path->connection_count);
            for (i32 index = 0; index < path->connection_count; ++index) {
                AIPATHCNX_s *connection = &path->connections[index];
                EdFileWriteChar(connection->node_indices[0]);
                EdFileWriteChar(connection->node_indices[1]);
                if (aidata_version > 11) {
                    EdFileWriteInt(connection->traversal_flags[0]);
                    EdFileWriteInt(connection->traversal_flags[1]);
                } else if (aidata_version > 8) {
                    EdFileWriteShort(connection->traversal_flags[0]);
                    EdFileWriteShort(connection->traversal_flags[1]);
                } else {
                    EdFileWriteChar(connection->traversal_flags[0]);
                    EdFileWriteChar(connection->traversal_flags[1]);
                }
                EdFileWriteShort(connection->rotation);
                EdFileWriteShort(connection->route_mask);
                EdFileWriteFloat(connection->distance);
                EdFileWriteFloat(connection->horizontal_distance);
            }
            for (i32 index = 0; index < path->node_count; ++index) {
                AIPATHNODE_s *node = &path->nodes[index];
                i32 length = node->name != nullptr ? strlen(node->name) + 1 : 0;
                EdFileWriteInt(length);
                if (length != 0) {
                    EdFileWrite(node->name, length);
                }
                EdFileWriteFloat(node->position.x);
                EdFileWriteFloat(node->position.y);
                EdFileWriteFloat(node->position.z);
                EdFileWriteFloat(node->radius);
                if (aidata_version > 7) {
                    EdFileWriteFloat(node->min_height);
                    EdFileWriteFloat(node->max_height);
                }
                EdFileWriteChar(node->connection_count);
                EdFileWriteChar(node->flags);
                EdFileWriteChar(0);
                EdFileWriteChar(node->runtime_flags);
                EdFileWriteShort(node->path_flags);
                if (aidata_version > 18) {
                    EdFileWriteChar(node->special_route_index);
                } else {
                    EdFileWriteChar(0);
                }
                char *special_name = nullptr;
                if (NuSpecialExistsFn(&node->special_handle)) {
                    special_name = NuSpecialGetName(&node->special_handle);
                }
                if (special_name != nullptr) {
                    i32 special_length = strlen(special_name) + 1;
                    EdFileWriteChar(special_length);
                    EdFileWrite(special_name, special_length);
                    EdFileWriteFloat(node->special_position.x);
                    EdFileWriteFloat(node->special_position.y);
                    EdFileWriteFloat(node->special_position.z);
                } else {
                    EdFileWriteChar(0);
                }
                for (i32 connection = 0; connection < node->connection_count; ++connection) {
                    EdFileWriteShort(node->connections[connection] - path->connections);
                }
                if (node->connection_count & 1) {
                    EdFileWriteShort(0);
                }
                EdFileWriteShort(node->route_membership_mask);
                EdFileWriteShort(node->route_boundary_mask);
            }
            for (i32 index = 0; index < path->node_count; ++index) {
                EdFileWrite(path->route_matrix[index], path->node_count);
            }
            EdFileWriteChar(path->route_count);
            for (i32 index = 0; index < path->route_count; ++index) {
                AIPATHROUTE_s *route = &path->routes[index];
                i32 length = strlen(route->name) + 1;
                EdFileWriteChar(length);
                EdFileWrite(route->name, length);
                EdFileWriteChar(route->route_count);
                EdFileWriteChar(route->exit_node_count);
                EdFileWriteChar(0);
                EdFileWriteChar(0);
                if (route->route_count != 0) {
                    EdFileWrite(route->node_routes, path->node_count);
                    EdFileWrite(route->node_directions, route->route_count);
                    for (i32 node = 0; node < route->route_count; ++node) {
                        EdFileWrite(route->route_nodes[node], route->route_count);
                    }
                    if (route->exit_node_count != 0) {
                        EdFileWrite(route->exit_nodes, route->exit_node_count);
                    }
                }
                if (SpecialRouteCharacterNameFn != nullptr) {
                    i32 user_count = 0;
                    for (i32 user = 0; user < 64; ++user) {
                        user_count += (route->character_masks[1] >> user) & 1;
                    }
                    EdFileWriteChar(user_count);
                    for (i32 user = 0; user < 63; ++user) {
                        if ((route->character_masks[1] >> user) & 1) {
                            char *name = SpecialRouteCharacterNameFn(user);
                            if (name != nullptr) {
                                i32 name_length = strlen(name) + 1;
                                EdFileWriteChar(name_length);
                                EdFileWrite(name, name_length);
                            } else {
                                EdFileWriteChar(0);
                            }
                        }
                    }
                    if ((route->character_masks[1] >> 63) & 1) {
                        EdFileWriteChar(9);
                        EdFileWrite((char *)"Everyone", 9);
                    }
                } else {
                    EdFileWriteChar(0);
                }
            }
            if (aidata_version > 18) {
                EdFileWriteChar(path->special_route_count);
                AIPATHNODELINK_s *link = path->special_routes;
                for (i32 index = 0; index < path->special_route_count; ++index, ++link) {
                    EdFileWriteUnsignedChar(link->node_index);
                    EdFileWriteShort(link->special_route_index);
                }
            }
        }
        if (aidata_version > 18) {
            EdFileWriteShort(system->special_route_count);
            AIPATHSPECIALROUTE_s *route = system->special_routes;
            for (i32 index = 0; index < system->special_route_count; ++index, ++route) {
                EdFileWriteChar(route->path_count);
                for (i32 path = 0; path < route->path_count; ++path) {
                    EdFileWriteChar(route->paths[path]->index);
                }
            }
        }
    }

    void aieditor_ClearMainMenu(void) {
        if (aieditor->main_menu != nullptr) {
            eduiMenuDestroy(aieditor->main_menu);
            aieditor->main_menu = nullptr;
        }
    }

}

static __used__ void creatureEditor_cbDeleteCreature(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
    if (item != nullptr && item->data != 0 && creature != nullptr && creature == nearest) {
        NuLinkedListRemove(&aieditor->creatures, &creature->link);
        memset(creature, 0, sizeof(*creature));
        NULISTHDR *free_list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
        NuLinkedListAppend(free_list, &creature->link);
        aieditor->mode_selection_36930 = nullptr;
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelDeleteCreatureMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static void antinodeEditor_cbDeleteAntinode(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if (item != nullptr && item->data != 0) {
        EDANTINODE_s *node = antinode_selected();
        if (node != nullptr && node == antinode_nearest()) {
            NuLinkedListRemove(antinode_list(), &node->link);
            memset(node, 0, sizeof(*node));
            NuLinkedListAppend(antinode_free_list(), &node->link);
            aieditor->mode_selection_42e9c = nullptr;
        }
    }
    aieditor_ClearMainMenu();
}

static void antinodeEditor_cbCancelDeleteAntinodeMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void aieditor_cbCancelSaveMenu(eduimenu_s *, eduiitem_s *, unsigned int) {
    aieditor_ClearMainMenu();
}

extern "C" {
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32) {
        if (AIEditorMovePlayersFn != nullptr) {
            AIEditorMovePlayersFn(&aieditor->camera_position);
        }
        aieditor_ClearMainMenu();
    }

    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32) {
        AISYS_s *system = aieditor->ai_system;
        if (system != nullptr && system->player_1 != nullptr) {
            edcamSetPos(&system->player_1->position);
        }
        aieditor_ClearMainMenu();
    }

    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *) {
        aieditor_ClearMainMenu();
    }

}

static __used__ void aieditor_cbSetEditorMode(eduimenu_s *, eduiitem_s *item, unsigned int) {
    if ((u32)item->data < (u32)aieditorsettings.mode_count) {
        aieditor_SetMode(item->data);
        aieditor_ClearMainMenu();
    }
}

static __used__ void routeEditor_cbDeleteRoute(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (__builtin_expect(item == nullptr, 1)) {
        return;
    }
    switch (static_cast<u32>(item->data)) {
        case 0: {
            eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, nullptr, "Delete current route??");
            if (menu != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, routeEditor_cbDeleteRoute, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, routeEditor_cbDeleteRoute, "Yes"));
                eduiMenuAttach(parent, menu);
            }
            break;
        }
        case 2:
            eduiMenuDestroy(parent);
            break;
        case 1: {
            EDAIPATH_s *path = aieditor->current_path;
            EDAIPATHROUTE_s *route = path->current_route;
            if (__builtin_expect(route == nullptr, 0)) {
                return;
            }
            i32 index = route - path->routes;
            route->flags &= ~u8(1);
            aieditor->current_path->current_route = nullptr;
            u64 mask = ~(1ULL << index);
            for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
                 node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link)) {
#define CLEAR_CONNECTION_ROUTE(N) node->connections[N].route_mask &= mask
                CLEAR_CONNECTION_ROUTE(0);
                CLEAR_CONNECTION_ROUTE(1);
                CLEAR_CONNECTION_ROUTE(2);
                CLEAR_CONNECTION_ROUTE(3);
                CLEAR_CONNECTION_ROUTE(4);
                CLEAR_CONNECTION_ROUTE(5);
                CLEAR_CONNECTION_ROUTE(6);
                CLEAR_CONNECTION_ROUTE(7);
#undef CLEAR_CONNECTION_ROUTE
                node->route_mask &= mask;
            }
            i32 next_index = index;
#define TRY_NEXT_ROUTE()                                                                                               \
    ++next_index;                                                                                                      \
    next_index = next_index >= 16 ? 0 : next_index;                                                                    \
    if (path->routes[next_index].flags & 1)                                                                            \
    path->current_route = &path->routes[next_index]
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
            TRY_NEXT_ROUTE();
#undef TRY_NEXT_ROUTE
            aieditor_ClearMainMenu();
            break;
        }
    }
}

static __used__ void routeEditor_cbCreateRoute(eduimenu_s *, eduiitem_s *, u32) {
    EDAIPATH_s *path = aieditor->current_path;
    i32 index = path->current_route != nullptr ? path->current_route - path->routes + 1 : 1;
    u8 flags;
    // The original checks each of the fixed 16 slots in sequence.
#define ROUTE_EDITOR_TRY_FREE()                                                                                        \
    if (index >= 16)                                                                                                   \
        index = 0;                                                                                                     \
    flags = path->routes[index].flags;                                                                                 \
    if (!(flags & 1))                                                                                                  \
        goto route_found;                                                                                              \
    ++index
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
    ROUTE_EDITOR_TRY_FREE();
#undef ROUTE_EDITOR_TRY_FREE
    return;
route_found:
    EDAIPATHROUTE_s *route = &path->routes[index];
    route->flags = flags | 1;
    route->user_mask = 0;
    aieditor->current_path->current_route = route;
    char name[16];
    i32 number = 0;
next_name:
    sprintf(name, "route%d", ++number);
#define ROUTE_EDITOR_CHECK_NAME(N)                                                                                     \
    if ((aieditor->current_path->routes[N].flags & 1) && NuStrICmp(name, aieditor->current_path->routes[N].name) == 0) \
    goto next_name
    ROUTE_EDITOR_CHECK_NAME(0);
    ROUTE_EDITOR_CHECK_NAME(1);
    ROUTE_EDITOR_CHECK_NAME(2);
    ROUTE_EDITOR_CHECK_NAME(3);
    ROUTE_EDITOR_CHECK_NAME(4);
    ROUTE_EDITOR_CHECK_NAME(5);
    ROUTE_EDITOR_CHECK_NAME(6);
    ROUTE_EDITOR_CHECK_NAME(7);
    ROUTE_EDITOR_CHECK_NAME(8);
    ROUTE_EDITOR_CHECK_NAME(9);
    ROUTE_EDITOR_CHECK_NAME(10);
    ROUTE_EDITOR_CHECK_NAME(11);
    ROUTE_EDITOR_CHECK_NAME(12);
    ROUTE_EDITOR_CHECK_NAME(13);
    ROUTE_EDITOR_CHECK_NAME(14);
    ROUTE_EDITOR_CHECK_NAME(15);
#undef ROUTE_EDITOR_CHECK_NAME
    strcpy(route->name, name);
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbSetShareNode(eduimenu_s *, eduiitem_s *item, u32) {
    if (aieditor->current_path == nullptr || aieditor->current_path->current_node == nullptr) {
        return;
    }
    EDAIPATHNODE_s *current = aieditor->current_path->current_node;
    EDAISHAREDPATHNODE_s *shared = current->shared_node;
    EDAIPATHNODE_s *matches[16];
    i32 path_number = 1;
    i32 match_count = 0;
    for (EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths); path != nullptr;
         path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link)) {
        if (path == aieditor->current_path) {
            continue;
        }
        if (__builtin_expect(item->data != 0 && path_number != item->data, 0)) {
            ++path_number;
            continue;
        }
        EDAIPATHNODE_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        for (EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes); node != nullptr;
             node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link)) {
            f32 dx = node->position.x - current->position.x;
            f32 dz = node->position.z - current->position.z;
            f32 distance = NuFsqrt(dx * dx + dz * dz);
            if (distance < node->radius + current->radius &&
                current->position.y + current->lower_height <= node->position.y + node->upper_height &&
                node->position.y + node->lower_height <= current->position.y + current->upper_height &&
                distance < nearest_distance) {
                nearest_distance = distance;
                nearest = node;
            }
        }
        if (nearest != nullptr && match_count < 16) {
            matches[match_count++] = nearest;
            if (shared == nullptr) {
                shared = nearest->shared_node;
            }
        }
        ++path_number;
    }
    if (match_count == 0) {
        if (item->data == 0) {
            aieditor_ClearMainMenu();
        }
        return;
    }
    if (item->data != 0 && !(item->flags & EDUI_ITEM_HIGHLIGHTED)) {
        if (current->shared_node == nullptr) {
            return;
        }
        for (i32 i = 0; i < match_count; ++i) {
            EDAIPATHNODE_s *node = matches[i];
            if (node->shared_node == current->shared_node) {
                EDAISHAREDPATHNODE_s *old_shared = node->shared_node;
                --old_shared->reference_count;
                if (old_shared->reference_count <= 1) {
                    pathEditor_DestroySharedNode(old_shared);
                }
                node->shared_node = nullptr;
            }
        }
        return;
    }
    if (shared == nullptr) {
        shared = (EDAISHAREDPATHNODE_s *)NuLinkedListGetHead(&aieditor->free_shared_path_nodes);
        if (shared == nullptr) {
            if (item->data == 0) {
                aieditor_ClearMainMenu();
            }
            return;
        }
        NuLinkedListRemove(&aieditor->free_shared_path_nodes, &shared->link);
        NuLinkedListAppend(&aieditor->shared_path_nodes, &shared->link);
    }
    if (current->shared_node == nullptr) {
        current->shared_node = shared;
        ++shared->reference_count;
    }
    for (i32 i = 0; i < match_count; ++i) {
        EDAIPATHNODE_s *node = matches[i];
        if (__builtin_expect(node->shared_node != shared, 1)) {
            if (node->shared_node != nullptr) {
                EDAISHAREDPATHNODE_s *old_shared = node->shared_node;
                --old_shared->reference_count;
                if (old_shared->reference_count <= 1) {
                    pathEditor_DestroySharedNode(old_shared);
                }
            }
            node->shared_node = shared;
            ++shared->reference_count;
        }
        EDAIPATHNODE_s *source = aieditor->current_path->current_node;
        node->radius = source->radius;
        node->lower_height = source->lower_height;
        node->upper_height = source->upper_height;
        node->position = source->position;
        node->special = source->special;
        node->special_position = source->special_position;
    }
    if (item->data == 0) {
        aieditor_ClearMainMenu();
    }
}

static __used__ void pathEditor_cbCreatePath(eduimenu_s *, eduiitem_s *, u32) {
    EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->free_paths);
    if (path == nullptr) {
        return;
    }
    NuLinkedListRemove(&aieditor->free_paths, &path->link);
    NuLinkedListAppend(&aieditor->paths, &path->link);
    char name[16];
    i32 number = 0;
    EDAIPATH_s *existing;
    do {
        sprintf(name, "NewPath%d", ++number);
        existing = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        while (existing != nullptr) {
            if (NuStrICmp(name, existing->name) == 0) {
                break;
            }
            existing = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &existing->link);
        }
    } while (existing != nullptr);
    strcpy(path->name, name);
    path->flags &= ~1;
    aieditor->current_path = path;
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbSetCurrentPath(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr) {
        EDAIPATH_s *path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
        i32 index = 0;
        auto item_data = item->data;
        while (path != nullptr && index != item_data) {
            path = (EDAIPATH_s *)NuLinkedListGetNext(&aieditor->paths, &path->link);
            ++index;
        }
        if (path != nullptr) {
            aieditor->current_path = path;
            EDAIPATHNODE_s *nearest = nullptr;
            f32 best_distance = 3.402823466e38f;
            EDAIPATHNODE_s *node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes);
            while (node != nullptr) {
                NUVEC difference;
                f32 distance = NuVecXZDistSqr(&aieditor->selection_position, &node->position, &difference);
                if (distance < best_distance) {
                    f32 height = aieditor->selection_position.y - node->position.y;
                    f32 upper = NuFmax(0.2f, node->upper_height);
                    f32 lower = NuFmin(-0.2f, node->lower_height);
                    if (height <= upper && height >= lower) {
                        best_distance = distance;
                        nearest = node;
                    }
                }
                node = (EDAIPATHNODE_s *)NuLinkedListGetNext(&path->nodes, &node->link);
            }
            path->other_node = nearest;
            path->current_node = nearest;
            if (nearest != nullptr) {
                edcamSetPos(&nearest->position);
            }
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelDisconnectNodeMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelDeleteNodeMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbEmptyLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    u32 data = (u32)item->data;
    if (__builtin_expect(data == 1, 0)) {
        goto empty_set;
    }
    if (__builtin_expect(data >= 1, 0)) {
        goto maybe_cancel;
    }
    {
        eduimenu_s *menu =
            eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, nullptr, (char *)"Empty current locator set?");
        if (menu != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, locatorEditor_cbEmptyLocatorSet, (char *)"No"));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbEmptyLocatorSet, (char *)"Yes"));
            eduiMenuAttach(parent, menu);
        }
        return;
    }
maybe_cancel:
    if (data == 2) {
        aieditor_ClearMainMenu();
    }
    return;
empty_set:
    EDLOCATORSET_s *set = aieditor->current_locator_set;
    if (set != nullptr) {
        for (i32 index = 0; index < 64; ++index) {
            set->locators[index] = nullptr;
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbCancelRenameLocatorSetMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbDeleteLocatorSet(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    u32 data = (u32)item->data;
    if (__builtin_expect(data == 1, 0)) {
        goto delete_set;
    }
    if (__builtin_expect(data >= 1, 0)) {
        goto maybe_cancel;
    }
    {
        eduimenu_s *menu =
            eduiMenuCreate(0xf0, 0x5a, 0xf0, 0xfa, ed_fnt, nullptr, (char *)"Delete current locator set?");
        if (menu != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, locatorEditor_cbDeleteLocatorSet, (char *)"No"));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbDeleteLocatorSet, (char *)"Yes"));
            eduiMenuAttach(parent, menu);
        }
        return;
    }
maybe_cancel:
    if (data == 2) {
        aieditor_ClearMainMenu();
    }
    return;
delete_set:
    EDLOCATORSET_s *set = aieditor->current_locator_set;
    if (set != nullptr) {
        NuLinkedListRemove(&aieditor->locator_sets, &set->link);
        memset(set, 0, sizeof(*set));
        NuLinkedListAppend(&aieditor->free_locator_sets, &set->link);
        aieditor->current_locator_set = nullptr;
        aieditor_ClearMainMenu();
    }
}

static __used__ void locatorEditor_cbSetLocatorSet(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr) {
        if (item->data != 0) {
            EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
            for (i32 index = 1; set != nullptr && index < item->data; ++index) {
                set = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &set->link);
            }
            if (set != nullptr && set != aieditor->current_locator_set) {
                aieditor->current_locator_set = set;
                aieditor->current_locator = set->locators[0];
                if (aieditor->current_locator != nullptr) {
                    aieditor->current_path = aieditor->current_locator->path;
                    edcamSetPos(&aieditor->current_locator->position);
                }
            }
        } else {
            aieditor->current_locator_set = nullptr;
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbCreateLocatorSet(eduimenu_s *, eduiitem_s *, u32) {
    EDLOCATORSET_s *set = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->free_locator_sets);
    if (set == nullptr) {
        return;
    }
    NuLinkedListRemove(&aieditor->free_locator_sets, &set->link);
    memset(set, 0, sizeof(*set));
    NuLinkedListAppend(&aieditor->locator_sets, &set->link);
    char name[16];
    i32 index = 0;
    EDLOCATORSET_s *other;
    do {
        sprintf(name, "NewSet%d", ++index);
        other = (EDLOCATORSET_s *)NuLinkedListGetHead(&aieditor->locator_sets);
        while (other != nullptr) {
            if (NuStrICmp(name, other->name) == 0) {
                break;
            }
            other = (EDLOCATORSET_s *)NuLinkedListGetNext(&aieditor->locator_sets, &other->link);
        }
    } while (other != nullptr);
    strcpy(set->name, name);
    aieditor->current_locator_set = set;
    aieditor->current_locator = nullptr;
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbDeleteLocator(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0 && aieditor->current_locator != nullptr &&
        aieditor->current_locator == aieditor->nearest_locator) {
        EDCREATURELOCATOR_s *creature = (EDCREATURELOCATOR_s *)NuLinkedListGetHead(&aieditor->creatures);
        while (creature != nullptr) {
            if (creature->locator == aieditor->current_locator) {
                creature->locator = nullptr;
            }
            creature = (EDCREATURELOCATOR_s *)NuLinkedListGetNext(&aieditor->creatures, &creature->link);
        }
        DestroyLocator(aieditor->current_locator);
        aieditor->current_locator = nullptr;
    }
    aieditor_ClearMainMenu();
}

static __used__ void locatorEditor_cbCancelDeleteLocatorMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

static __used__ __attribute__((optimize("no-tree-vectorize"))) void
locatorEditor_cbAddLocatorsByNameYesNo(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0 && NuStrLen(aieditor->pending_locator_name) != 0 &&
        aieditor->current_locator_set != nullptr) {
        EDLOCATORSET_s *set = aieditor->current_locator_set;
        EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
        while (locator != nullptr) {
            if (__builtin_expect(NuStrNICmp(aieditor->pending_locator_name, locator->name, -1) == 0, 0)) {
                EDLOCATOR_s *before = nullptr;
                for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
                    if (NuStrICmp(set->locators[index]->name, locator->name) > 0) {
                        if (NuStrLen(set->locators[index]->name) < NuStrLen(locator->name)) {
                            continue;
                        }
                        before = set->locators[index];
                        break;
                    }
                }
                AddLocatorToSet(set, locator, before);
            }
            locator = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &locator->link);
        }
        aieditor->current_locator = set->locators[0];
        if (aieditor->current_locator != nullptr) {
            aieditor->current_path = aieditor->current_locator->path;
            edcamSetPos(&aieditor->current_locator->position);
        }
    }
    memset(aieditor->pending_locator_name, 0, sizeof(aieditor->pending_locator_name));
    aieditor_ClearMainMenu();
}

static __used__ void areaEditor_cbDeleteArea(eduimenu_s *menu, eduiitem_s *, unsigned int) {
    if (menu != NULL && menu->field_0c != NULL && area_selected() != NULL && area_selected() == area_hovered()) {
        NULISTHDR *scripts = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x36924);
        for (NULISTLNK *node = NuLinkedListGetHead(scripts); node != NULL;
             node =
                 NuLinkedListGetNext(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x36924), node)) {
            u8 *script = reinterpret_cast<u8 *>(node);
            EDAIAREA_s *selected = area_selected();
            if (*reinterpret_cast<EDAIAREA_s **>(script + 0x80) == selected) {
                *reinterpret_cast<EDAIAREA_s **>(script + 0x80) = NULL;
            }
            if (*reinterpret_cast<EDAIAREA_s **>(script + 0x6c) == selected) {
                *reinterpret_cast<EDAIAREA_s **>(script + 0x6c) = NULL;
            }
        }
        EDAIAREA_s *selected = area_selected();
        if (selected != NULL) {
            NuLinkedListRemove(area_list(), &selected->link);
            memset(selected, 0, sizeof(*selected));
            NuLinkedListAppend(area_free_list(), &selected->link);
        }
        area_selected() = NULL;
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbCancelDeleteAreaMenu(eduimenu_s *, eduimenu_s *) {
    aieditor_ClearMainMenu();
}

extern "C" {
    void InitFn_AIEditorMovePlayers(AIEDITORMOVEPLAYERS *function) {
        AIEditorMovePlayersFn = function;
    }

    eduimenu_s *aieditor_AddMessage(char *title, char *message) {
        eduimenu_s *outer = eduiMenuCreate(0xc8, 0x46, 0xf0, 0x10e, ed_fnt, aieditor_cbCancelMainMenu, title);
        if (outer != nullptr) {
            eduimenu_s *inner = eduiMenuCreate(0x78, 0x5a, 0x1b8, 0xfa, ed_fnt, nullptr, title);
            if (inner != nullptr) {
                eduiitem_s *item = eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbCancelSaveMenu, message);
                eduiMenuAddItem(inner, item);
                eduiMenuAttach(outer, inner);
            }
        }
        return outer;
    }

    void antinodeEditor_UpdateAntiNodesOnPlatforms(void) {
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        NUVEC rotated;
        NUVEC forward = {0.0f, 0.0f, 1.0f};
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            if (!NuSpecialExistsFn(&node->special))
                continue;
            NUMTX *matrix = NuSpecialGetDrawMtx(&node->special);
            NuVecMtxTransform(&node->position, &node->special_position, matrix);
            NuVecMtxRotate(&rotated, &forward, matrix);
            node->flags = NuAtan2D(rotated.x, rotated.z);
            node->flags = NuAngAdd(node->flags, node->rotation_offset);
        }
    }

    __attribute__((optimize("O2"))) void antinodeEditorDrawAntinodes(void) {
        i32 solid = aieditorsettings.solid_antinode_display;
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list)); node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            u32 colour = 0x50505050;
            i32 segments = 8;
            if (aieditorsettings.current_mode == AIEDITOR_ANTINODES) {
                if (node == aieditor->mode_selection_42e9c) {
                    segments = 32;
                    if (node == *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0))
                        colour = NuSpecialExistsFn(&node->special) ? 0xffff0064 : 0xffff0000;
                    else
                        colour = NuSpecialExistsFn(&node->special) ? 0x80800064 : 0x80800000;
                } else if (node == *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0)) {
                    colour = 0xffffffff;
                    segments = 32;
                } else {
                    colour = 0x80640000;
                }
            }
            if (node->type == 0) {
                if (solid)
                    LocaledbitsDrawSolidCircleXY(&node->position, node->radius, node->position.y + node->lower_height,
                                                 node->position.y + node->upper_height, colour, 0, segments);
                else
                    LocaledbitsDrawCircleXY(&node->position, node->radius, colour, 0, segments);
            } else if (node->type == 1) {
                f32 lower = solid ? node->position.y + node->lower_height : node->position.y;
                f32 upper = solid ? node->position.y + node->upper_height : node->position.y;
                LocaledbitsDrawSolidEllipseXY(&node->position, node->base_radius, node->base_height, node->flags, lower,
                                              upper, colour, 0, segments);
            } else if (node->type == 2) {
                NUMTX matrix;
                NuMtxSetTranslation(&matrix, &node->position);
                NuMtxPreRotateY(&matrix, node->flags);
                NUVEC minimum = {-node->base_radius, node->lower_height, -node->base_height};
                NUVEC maximum = {node->base_radius, node->upper_height, node->base_height};
                if (!solid) {
                    minimum.y = 0.0f;
                    maximum.y = 0.0f;
                }
                NuRndrBoundingBox(&minimum, &maximum, &matrix, colour);
            }
        }
    }

    void antinodeEditorSaveData(void) {
        if (aidata_version <= 12)
            return;
        NULISTHDR *list = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94);
        EDANTINODE_s *head = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetHead(list));
        if (head == nullptr) {
            EdFileWriteInt(0);
            return;
        }
        i32 count = 0;
        for (EDANTINODE_s *node = head; node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link)))
            ++count;
        EdFileWriteInt(count);
        for (EDANTINODE_s *node = reinterpret_cast<EDANTINODE_s *>(
                 NuLinkedListGetHead(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94)));
             node != nullptr;
             node = reinterpret_cast<EDANTINODE_s *>(NuLinkedListGetNext(
                 reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x42e94), &node->link))) {
            if (node->type == 2)
                node->radius = NuFsqrt(node->base_radius * node->base_radius + node->base_height * node->base_height);
            else if (node->type == 1)
                node->radius = node->base_radius > node->base_height ? node->base_radius : node->base_height;
            EdFileWriteFloat(node->position.x);
            EdFileWriteFloat(node->position.y);
            EdFileWriteFloat(node->position.z);
            EdFileWriteFloat(node->radius);
            EdFileWriteFloat(node->position.y + node->lower_height);
            EdFileWriteFloat(node->position.y + node->upper_height);
            if (aidata_version > 14) {
                EdFileWriteInt(node->flags);
                EdFileWriteFloat(node->base_radius);
                EdFileWriteFloat(node->base_height);
                EdFileWriteChar(node->type);
            } else {
                EdFileWriteChar(0);
            }
            EdFileWriteChar(0);
            EdFileWriteChar(0);
            EdFileWriteChar(node->game_flags);
            char *name = NuSpecialExistsFn(&node->special) ? NuSpecialGetName(&node->special) : nullptr;
            if (name == nullptr) {
                EdFileWriteChar(0);
            } else {
                i32 length = strlen(name) + 1;
                EdFileWriteChar(length);
                EdFileWrite(name, length);
                EdFileWriteFloat(node->special_position.x);
                EdFileWriteFloat(node->special_position.y);
                EdFileWriteFloat(node->special_position.z);
                if (aidata_version > 14)
                    EdFileWriteInt(node->rotation_offset);
            }
        }
    }

    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditorSaveData(AIPATHSYS_s *system) {
        if (GlobalCharacterNameFn == nullptr) {
            EdFileWriteInt(0);
            return;
        }

        i32 count = 0;
        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            CreatureEditorRecord *creature = reinterpret_cast<CreatureEditorRecord *>(link);
            if (creature->path != nullptr && GlobalCharacterNameFn(creature->character_type) != nullptr)
                ++count;
        }
        EdFileWriteInt(count);

        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            CreatureEditorRecord *creature = reinterpret_cast<CreatureEditorRecord *>(link);
            char *character_name = GlobalCharacterNameFn(creature->character_type);
            if (creature->path == nullptr || character_name == nullptr)
                continue;

            EdFileWrite(creature->name, 0x10);
            EdFileWrite(creature->script_name, 0x10);
            if (aidata_version > 13)
                EdFileWrite(character_name, 0x20);
            else
                EdFileWrite(character_name, 0x10);
            EdFileWriteFloat(creature->position.x);
            EdFileWriteFloat(creature->position.y);
            EdFileWriteFloat(creature->position.z);
            EdFileWriteShort(creature->angle);
            if (aidata_version > 15)
                EdFileWriteChar(creature->set);
            EdFileWriteChar(creature->group_count);
            EdFileWriteChar(creature->across_count);
            EdFileWriteInt(creature->valid_positions);
            EdFileWriteFloat(creature->x_spacing);
            EdFileWriteFloat(creature->z_spacing);
            EdFileWriteInt(creature->flags);

            EdFileWriteChar(reinterpret_cast<EDAIPATH_s *>(creature->path)->draw_index);
            AIPATH *runtime_path = system->paths[reinterpret_cast<EDAIPATH_s *>(creature->path)->draw_index];
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(creature) + 0x38);
            i16 connection_index = 0;
            bool connection_found = false;
            bool reversed = false;
            for (i32 index = 0; index < runtime_path->connection_count; ++index) {
                AIPATHCNX *connection = &runtime_path->connections[index];
                i32 second_index = check->second->index;
                i32 first_index = check->first->index;
                if (connection->node_indices[0] == first_index && connection->node_indices[1] == second_index) {
                    connection_index = index;
                    connection_found = true;
                    break;
                }
                if (connection->node_indices[0] == second_index && connection->node_indices[1] == first_index) {
                    connection_index = index;
                    connection_found = true;
                    reversed = true;
                    break;
                }
            }
            i32 direction = 0;
            if (connection_found) {
                i32 path_angle = check->angle;
                i32 magnitude = path_angle < 0 ? -path_angle : path_angle;
                if (reversed) {
                    if (magnitude <= 0x3fff) {
                        direction = 1;
                    }
                } else if (magnitude > 0x3fff) {
                    direction = 1;
                }
            }
            EdFileWriteChar(direction);
            EdFileWriteShort(connection_index);

            EdFileWriteFloat(creature->script_params[0]);
            EdFileWriteFloat(creature->script_params[1]);
            EdFileWriteFloat(creature->script_params[2]);
            EdFileWriteFloat(creature->script_params[3]);
            if (creature->trigger_area != nullptr) {
                EdFileWriteInt(1);
                EdFileWrite(reinterpret_cast<char *>(creature->trigger_area) + 8, 0x10);
            } else {
                EdFileWriteInt(0);
            }
            if (creature->locator != nullptr) {
                EdFileWriteInt(1);
                EdFileWrite(reinterpret_cast<char *>(creature->locator) + 8, 0x10);
            } else {
                EdFileWriteInt(0);
            }
            if (aidata_version > 16) {
                if (creature->respawn_locator != nullptr) {
                    EdFileWriteInt(1);
                    EdFileWrite(reinterpret_cast<char *>(creature->respawn_locator) + 8, 0x10);
                } else {
                    EdFileWriteInt(0);
                }
            }

            if (aidata_version <= 7)
                continue;
            EdFileWriteChar(creature->difficulty);
            EdFileWriteChar(creature->min_respawns);
            EdFileWriteChar(creature->max_respawns);
            EdFileWriteChar(creature->activation);
            EdFileWriteFloat(creature->min_respawn_time);
            EdFileWriteFloat(creature->max_respawn_time);
            if (aidata_version > 9)
                EdFileWriteFloat(creature->stagger_start);
            if (creature->activation == 1) {
                EdFileWrite(reinterpret_cast<char *>(creature->activation_area) + 8, 0x10);
            }
            if (aidata_version <= 10)
                continue;
            if (GetViewRangeFn != nullptr && creature->view_distance == GetViewRangeFn(creature->character_type)) {
                EdFileWriteFloat(0.0f);
            } else {
                EdFileWriteFloat(creature->view_distance);
            }
            if (GetHearDistanceFn != nullptr &&
                creature->hear_distance == GetHearDistanceFn(creature->character_type)) {
                EdFileWriteFloat(0.0f);
            } else {
                EdFileWriteFloat(creature->hear_distance);
            }
            if (GetMaxViewHeightFn != nullptr &&
                creature->max_view_height == GetMaxViewHeightFn(creature->character_type)) {
                EdFileWriteFloat(0.0f);
            } else {
                EdFileWriteFloat(creature->max_view_height);
            }
            if (GetMinViewHeightFn != nullptr &&
                creature->negative_min_view_height == GetMinViewHeightFn(creature->character_type)) {
                EdFileWriteFloat(0.0f);
            } else {
                EdFileWriteFloat(creature->negative_min_view_height);
            }
            EdFileWriteInt(0);
        }
    }

    i32 aieditor_Save(void) {
        VARIPTR cursor;
        VARIPTR end;
        cursor.u8_ptr = aieditor->save_scratch;
        end.char_ptr = aieditor->warning;
        memset(cursor.u8_ptr, 0, sizeof(aieditor->save_scratch));
        AIPATHSYS_s *paths = pathEditorCreateData(&cursor, &end, &aieditorsettings.external_display_a,
                                                  &aieditorsettings.external_display_b);
        if (paths == nullptr) {
            return 1;
        }
        if (aieditor->enter_extra_2 == nullptr) {
            return 1;
        }

        char filename[128];
        sprintf(filename, "%sLevels\\%s\\%s\\%s.ai2", AiLevelPathName, static_cast<char *>(aieditor->enter_extra_1),
                static_cast<char *>(aieditor->enter_extra_3), static_cast<char *>(aieditor->enter_extra_2));
        if (NuFileExists(filename)) {
            NUFILE_INFO info;
            if (NuFileGetInfo(filename, &info)) {
                char *basename = filename;
                for (char *separator = NuStrIStr(basename, (char *)"\\"); separator != nullptr;
                     separator = NuStrIStr(basename, (char *)"\\")) {
                    basename = separator + 1;
                }
                char backup[128];
                sprintf(backup, "ai_backups\\%s.%d_%d_%d_%d_%d_%d", basename, info.year, info.month, info.day,
                        info.hour, info.minute, info.second);
                NuFileCopy(backup, filename);
            }
            char backup[128];
            sprintf(backup, "%s.bak", filename);
            NuFileCopy(backup, filename);
        }
        EdFileSetMedia(1);
        if (!EdFileOpen(filename, NUFILE_WRITE)) {
            return 0;
        }
        EdFileWriteInt(aidata_version);
        pathEditorSaveData(paths);
        areaEditorSaveData();
        locatorEditorSaveData(paths);
        creatureEditorSaveData(paths);
        antinodeEditorSaveData();
        if (GameAISaveFn != nullptr) {
            reinterpret_cast<void (*)(AIPATHSYS_s *)>(GameAISaveFn)(paths);
        }
        EdFileClose();
        return 1;
    }

    void aieditor_cbSave(eduimenu_s *parent, eduiitem_s *, u32) {
        if (aieditor_Save() == 0) {
            eduimenu_s *menu = eduiMenuCreate(0x78, 0x5a, 0x1b8, 0xfa, ed_fnt, nullptr, (char *)"AI Save FAILED");
            if (menu != nullptr) {
                char message[128];
                sprintf(message, "Make sure there is a folder called \"%s\" in your level directory",
                        static_cast<char *>(aieditor->enter_extra_3));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, &attr, 0, 0, aieditor_cbCancelSaveMenu, message));
                eduiMenuAttach(parent, menu);
            }
        } else {
            aieditor_ClearMainMenu();
        }
    }

}

i32 creatureEditor_IsSelectable(EDCREATURE_s *creature) {
    if (!aieditorsettings.show_creatures_set || aieditor->mode_selection_36930 == nullptr)
        return 1;
    u8 current_set = *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(aieditor->mode_selection_36930) + 0x5a);
    u8 candidate_set = *(reinterpret_cast<u8 *>(creature) + 0x5a);
    return current_set == candidate_set;
}

extern "C" {
    __attribute__((optimize("O2", "omit-frame-pointer"))) EDCREATURE_s *creatureEditor_GetNearest(i32 use_radius) {
        if (GlobalCharacterHGobjFn == nullptr)
            return nullptr;

        EDCREATURE_s *nearest = nullptr;
        f32 nearest_distance = FLT_MAX;
        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(link);
            if (!creatureEditor_IsSelectable(creature))
                continue;
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(creature);
            u8 *model = reinterpret_cast<u8 *>(GlobalCharacterHGobjFn(record->character_type));
            f32 height = 0.4f;
            f32 min_height = 0.0f;
            f32 radius = 0.4f;
            if (model != nullptr) {
                min_height = *reinterpret_cast<f32 *>(model + 0x1c4);
                height = *reinterpret_cast<f32 *>(model + 0x1c8);
                radius = *reinterpret_cast<f32 *>(model + 0x1cc);
            }
            NUVEC delta;
            f32 distance = NuVecXZDistSqr(&aieditor->camera_position, &record->position, &delta);
            if (!(distance < nearest_distance))
                continue;
            if (use_radius == 0) {
                nearest = creature;
                nearest_distance = distance;
                continue;
            }
            f32 lower = record->position.y + min_height;
            if (lower > record->position.y) {
                height += min_height;
                lower = record->position.y;
            }
            if (!(distance < radius * radius))
                continue;
            if (aieditor->camera_position.y < lower - aieditor_y_tolerance)
                continue;
            if (aieditor->camera_position.y <= lower + height + aieditor_y_tolerance) {
                nearest = creature;
                nearest_distance = distance;
            }
        }
        return nearest;
    }

}

__attribute__((optimize("no-tree-vectorize"))) eduimenu_s *locatorEditor_Process(nupad_s *pad) {
    char base[32];
    if ((pad->digital_buttons_pressed & 0x80) != 0) {
        goto options;
    }
    if (__builtin_expect(NuStrLen(aieditor->pending_locator_name) == 0, 1)) {
        goto process_buttons;
    }
    {
        if (aieditor->current_locator_set != nullptr) {
            char title[128];
            sprintf(title, "Add locators \"%s\" to set \"%s\"?", aieditor->pending_locator_name,
                    aieditor->current_locator_set->name);
            eduimenu_s *menu = eduiMenuCreate(100, 70, 440, 270, ed_fnt, aieditor_cbCancelMainMenu, title);
            if (__builtin_expect(menu != nullptr, 1)) {
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(0, attr, 0, 0, locatorEditor_cbAddLocatorsByNameYesNo, (char *)"No"));
                eduiMenuAddItem(
                    menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbAddLocatorsByNameYesNo, (char *)"Yes"));
                return menu;
            }
        }
        memset(aieditor->pending_locator_name, 0, sizeof(aieditor->pending_locator_name));
    }
    goto process_buttons;

options: {
    eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, (char *)"Options");
    if (menu == nullptr) {
        return nullptr;
    }
    eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode,
                                            (char *)"Select Editor Mode"));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, (char *)"Save AI Data"));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbGoToPlayer, (char *)"Go To Player"));
    eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbMovePlayer, (char *)"Move Player"));
    if (aieditor->current_locator != nullptr && AIScriptNameFromIx(aieditor->ai_system, 0) != nullptr) {
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbRenameLocatorMenu, (char *)"Rename Locator"));
    }
    eduiMenuAddItem(menu,
                    eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbCreateLocatorSet, (char *)"Create Locator Set"));
    if (NuLinkedListGetHead(&aieditor->locator_sets) != nullptr) {
        eduiMenuAddItem(
            menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbSelectLocatorSet, (char *)"Select Locator Set"));
        if (aieditor->current_locator_set != nullptr) {
            eduiMenuAddItem(
                menu, eduiItemSelCreate(0, attr, 0, 0, locatorEditor_cbDeleteLocatorSet, (char *)"Delete Locator Set"));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbRenameLocatorSetMenu,
                                                    (char *)"Rename Locator Set"));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbAddLocatorsByNameMenu,
                                                    (char *)"Add Locators To Set By Name"));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(0, attr, 0, 0, locatorEditor_cbEmptyLocatorSet, (char *)"Empty Locator Set"));
        }
    }
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -i32(aieditorsettings.stop_platforms), 3,
                                               aieditor_cbStopPlatformsToggle, (char *)"Stop Platforms"));
    eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -i32(aieditorsettings.snap_height_display), 2,
                                               aieditor_cbSnapHeightToggle, (char *)"Snap Height"));
    return menu;
}

process_buttons:
    if ((pad->digital_buttons & 0x40) != 0) {
        if (aieditor->nearest_locator != nullptr) {
            if ((pad->digital_buttons_pressed & 0x40) != 0) {
                aieditor->current_locator = aieditor->nearest_locator;
                aieditor->current_path = aieditor->current_locator->path;
                aieditorsettings.area_rotation = aieditor->current_locator->direction;
                edcamSetPos(&aieditor->current_locator->position);
            } else if (aieditor->current_locator != nullptr &&
                       *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0) {
                EDLOCATOR_s *locator = aieditor->current_locator;
                locator->position = aieditor->camera_position;
                memcpy(locator->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, 0x1c);
                locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
            } else if (aieditor->current_locator != nullptr) {
                edcamSetPos(&aieditor->current_locator->position);
            }
        } else if ((pad->digital_buttons_pressed & 0x40) != 0 &&
                   *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0) {
            EDLOCATOR_s *previous = aieditor->current_locator;

            if (aieditor->current_locator_set != nullptr && NuStrLen(aieditor->current_locator_set->name) != 0) {
                NuStrCpy(base, aieditor->current_locator_set->name);
            } else if (__builtin_expect(previous != nullptr, 0)) {
                NuStrCpy(base, previous->name);
            } else {
                NuStrCpy(base, "Locator");
            }
            char *suffix = strrchr(base, '_');
            if (__builtin_expect(suffix != nullptr, 0)) {
                *suffix = 0;
            }
            AIEDITOR_RENDER_STATE *context = aieditor;
            i32 direction = aieditorsettings.area_rotation;
            previous = context->current_locator;
            EDLOCATOR_s *locator = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->free_locators);
            if (locator != nullptr) {
                NuLinkedListRemove(&aieditor->free_locators, &locator->link);
                NuLinkedListAppend(&aieditor->locators, &locator->link);
                locator->position = context->camera_position;
                locator->direction = direction;
            }
            context->current_locator = locator;
            locator = aieditor->current_locator;
            if (locator != nullptr) {
                char name[16];
                i32 index = 0;
                EDLOCATOR_s *other;
                do {
                    sprintf(name, "%s_%d", base, ++index);
                    other = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
                    while (other != nullptr && NuStrICmp(name, other->name) != 0) {
                        other = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &other->link);
                    }
                } while (other != nullptr);
                strcpy(aieditor->current_locator->name, name);
                locator = aieditor->current_locator;
                memcpy(locator->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, 0x1c);
                locator->path_angle = NuAngSub(locator->direction, locator->path_angle);
                if (aieditor->current_locator_set != nullptr) {
                    locator = aieditor->current_locator;
                    EDLOCATOR_s *before = previous != locator ? previous : nullptr;
                    AddLocatorToSet(aieditor->current_locator_set, locator, before);
                }
            }
        }
    } else if ((pad->digital_buttons_pressed & 0x10) != 0 && aieditor->current_locator != nullptr &&
               aieditor->current_locator == aieditor->nearest_locator) {
        eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, locatorEditor_cbCancelDeleteLocatorMenu,
                                          (char *)"Delete locator??");
        if (menu != nullptr) {
            eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, locatorEditor_cbDeleteLocator, (char *)"No"));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, locatorEditor_cbDeleteLocator, (char *)"Yes"));
        }
        return menu;
    } else if ((pad->digital_buttons_pressed & 0x10) != 0) {
        // A delete press without a matching selected locator only refreshes hover state.
    } else if ((pad->digital_buttons & 0x100) != 0) {
        EDLOCATOR_s *next = nullptr;
        if ((pad->digital_buttons_pressed & (0x08 | 0x02)) == 0) {
            if ((pad->digital_buttons_pressed & 0x100) == 0) {
                goto refresh_hover;
            }
            next = locatorEditor_GetNearest(0);
        } else {
            EDLOCATORSET_s *set = aieditor->current_locator_set;
            if (set != nullptr) {
                i32 index = -1;
                auto aieditor_current_locator = aieditor->current_locator;
                for (i32 i = 0; i < 64; ++i) {
                    if (set->locators[i] == aieditor_current_locator) {
                        index = i;
                        break;
                    }
                }
                if ((pad->digital_buttons_pressed & 0x08) == 0) {
                    if (aieditor->current_locator == nullptr) {
                        next = set->locators[0];
                    } else if (index > 0) {
                        next = set->locators[index - 1];
                    } else {
                        for (i32 i = 63; i >= 0; --i) {
                            if (set->locators[i] == nullptr) {
                                continue;
                            }
                            next = set->locators[i];
                            break;
                        }
                    }
                } else {
                    next = index < 0 || index >= 63 || set->locators[index + 1] == nullptr ? set->locators[0]
                                                                                           : set->locators[index + 1];
                }
            } else if ((pad->digital_buttons_pressed & 0x08) != 0) {
                if (aieditor->current_locator != nullptr) {
                    EDLOCATOR_s *&selection = aieditor->current_locator;
                    selection = (EDLOCATOR_s *)NuLinkedListGetNext(&aieditor->locators, &selection->link);
                }
                if (aieditor->current_locator == nullptr) {
                    EDLOCATOR_s *&selection = aieditor->current_locator;
                    selection = (EDLOCATOR_s *)NuLinkedListGetHead(&aieditor->locators);
                }
                next = aieditor->current_locator;
            } else {
                if (aieditor->current_locator != nullptr) {
                    EDLOCATOR_s *&selection = aieditor->current_locator;
                    selection = (EDLOCATOR_s *)NuLinkedListGetPrev(&aieditor->locators, &selection->link);
                }
                if (aieditor->current_locator == nullptr) {
                    EDLOCATOR_s *&selection = aieditor->current_locator;
                    selection = (EDLOCATOR_s *)NuLinkedListGetTail(&aieditor->locators);
                }
                next = aieditor->current_locator;
            }
        }
        aieditor->current_locator = next;
        if (next != nullptr) {
            aieditor->current_path = next->path;
            edcamSetPos(&next->position);
        }
    } else if ((pad->digital_buttons & 0x2000) != 0) {
        i32 &step = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditorsettings.area_rotation = aieditor->current_locator->direction;
        }
        step = (pad->digital_buttons_pressed & 0x8000) != 0 ? 20 : step + 20;
        if (step > 600) {
            step = 600;
        }
        aieditorsettings.area_rotation = NuAngAdd(aieditorsettings.area_rotation, step);
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditor->current_locator->direction = aieditorsettings.area_rotation;
            aieditor->current_locator->path_angle =
                NuAngSub(aieditor->current_locator->direction,
                         *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
        }
    } else if ((pad->digital_buttons & 0x8000) != 0) {
        i32 &step = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x36934);
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditorsettings.area_rotation = aieditor->current_locator->direction;
        }
        step = (pad->digital_buttons_pressed & 0x2000) != 0 ? 20 : step + 20;
        if (step > 600) {
            step = 600;
        }
        aieditorsettings.area_rotation = NuAngSub(aieditorsettings.area_rotation, step);
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditor->current_locator->direction = aieditorsettings.area_rotation;
            aieditor->current_locator->path_angle =
                NuAngSub(aieditor->current_locator->direction,
                         *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
        }
    } else if ((pad->digital_buttons & 0x4000) != 0) {
        i32 path_angle = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60);
        i32 difference = NuAngSub(aieditorsettings.area_rotation, path_angle);
        i32 quadrant = difference < 0 ? (difference + 0x3fff) >> 14 : difference >> 14;
        i32 remainder = difference % 0x4000;
        if (remainder > 0x2000) {
            ++quadrant;
        } else {
            if (remainder < -0x2000) {
                --quadrant;
            }
        }
        aieditorsettings.area_rotation =
            NuAngAdd(quadrant << 14, *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
        if (aieditor->current_locator != nullptr && aieditor->current_locator == aieditor->nearest_locator) {
            aieditor->current_locator->direction = aieditorsettings.area_rotation;
            aieditor->current_locator->path_angle =
                NuAngSub(aieditor->current_locator->direction,
                         *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
        }
    } else if ((pad->digital_buttons & 0x20) != 0 && aieditor->current_locator_set != nullptr &&
               (pad->digital_buttons_pressed & 0x20) != 0 && aieditor->nearest_locator != nullptr) {
        EDLOCATORSET_s *set = aieditor->current_locator_set;
        EDLOCATOR_s *nearest = aieditor->nearest_locator;
        for (i32 index = 0; index < 64 && set->locators[index] != nullptr; ++index) {
            if (set->locators[index] == nearest) {
                for (i32 move = index; move < 63; ++move) {
                    set->locators[move] = set->locators[move + 1];
                }
                set->locators[63] = nullptr;
                break;
            }
        }
        EDLOCATOR_s *before = aieditor->current_locator;
        if (before != nullptr && before != nearest) {
            if (AddLocatorToSet(set, nearest, before) != 0) {
                aieditor->current_locator = nearest;
                aieditor->current_path = nearest->path;
                aieditorsettings.area_rotation = nearest->direction;
                edcamSetPos(&nearest->position);
            }
        } else {
            AddLocatorToSetAtEnd(set, nearest);
        }
    }
refresh_hover:
    *reinterpret_cast<EDCREATURE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x3692c) = creatureEditor_GetNearest(1);
    aieditor->nearest_locator = locatorEditor_GetNearest(1);
    return nullptr;
}

extern "C" {
    __attribute__((force_align_arg_pointer)) void aieditor_SetCurrentScript(char *name,
                                                                            const AIEditorScriptSelection *selection) {
        if (NuStrICmp(name, aieditorsettings.current_script_name) != 0) {
            strcpy(aieditorsettings.current_script_name, name);
        }
        if (selection != nullptr) {
            aieditorsettings.current_script_flags = selection->flags & 0x1e;
            for (i32 index = 0; index < 4; ++index) {
                aieditorsettings.current_script_params[index] = selection->script_params[index];
            }
            return;
        }
        AISCRIPT *script = AIScriptFind(aieditor->ai_system, aieditorsettings.current_script_name, 1, 1, 1);
        if (script != nullptr) {
            aieditorsettings.current_script_params[0] = script->params[0].default_val;
            aieditorsettings.current_script_params[1] = script->params[1].default_val;
            aieditorsettings.current_script_params[2] = script->params[2].default_val;
            aieditorsettings.current_script_params[3] = script->params[3].default_val;
        } else {
            memset(aieditorsettings.current_script_params, 0, sizeof(aieditorsettings.current_script_params));
        }
        aieditorsettings.current_script_flags = 0;
    }

}

__attribute__((optimize("O3"))) void creatureEditor_Enter() {
    aieditor->creatures.head = nullptr;
    aieditor->creatures.tail = nullptr;
    for (i32 i = 0; i < 128; ++i) {
        NULISTHDR *free_creatures = reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c);
        CreatureEditorRecord *pool =
            reinterpret_cast<CreatureEditorRecord *>(reinterpret_cast<u8 *>(aieditor) + 0x3131c);
        NuLinkedListAppend(free_creatures, &pool[i].link);
    }

    AISYS *system = aieditor->ai_system;
    if (system != nullptr) {
        for (i32 i = 0; i < system->creature_count; ++i, system = aieditor->ai_system) {
            AICREATURE *source = &system->creatures[i];
            CreatureEditorRecord *creature =
                (CreatureEditorRecord *)CreateCreature(source->type, &source->pos, source->y_rot);
            if (creature == nullptr)
                continue;
            EDAIPATH_s *path = pathEditor_GetPath(reinterpret_cast<const char *>(source->path_info.path));
            f32 tolerance = 0.0f;
            auto creature_path_check = creature->path_check;
            for (;;) {
                pathEditor_OnPathCheck(&creature->position, (EDAIPATHCHECK_s *)creature_path_check, path, tolerance);
                if (*reinterpret_cast<i32 *>(creature_path_check) != 0)
                    break;
                tolerance += 0.01f;
            }
            i32 *path_angle = reinterpret_cast<i32 *>(creature->path_check + 0x18);
            *path_angle = NuAngSub(creature->angle, *path_angle);
            strcpy(creature->name, source->name);
            strcpy(creature->script_name, source->script_name);
            creature->set = source->set;
            creature->group_count = source->count;
            creature->across_count = source->count_across;
            creature->valid_positions = source->active_mask;
            creature->x_spacing = source->x_spacing;
            creature->flags = source->flags;
            creature->z_spacing = source->z_spacing;
            for (i32 p = 0; p < 4; ++p)
                creature->script_params[p] = source->script_params[p];
            AISCRIPT *script = AIScriptFind(aieditor->ai_system, creature->script_name, 1, 1, 1);
            if (script != nullptr) {
                auto source_flags = source->flags;
                for (i32 p = 0; p < 4; ++p) {
                    if ((source_flags & (2 << p)) == 0)
                        creature->script_params[p] = script->params[p].default_val;
                }
            }
            if (source->area != nullptr)
                creature->trigger_area = FindCreatureArea(source->area->name);
            if (source->locator != nullptr)
                creature->locator = FindCreatureLocator(source->locator->name);
            if (source->respawn_locator != nullptr)
                creature->respawn_locator = FindCreatureLocator(source->respawn_locator->name);
            creature->activation = source->activate_type;
            if (source->activate_type == 1) {
                creature->activation = 0;
                if (source->activate_area != nullptr) {
                    creature->activation_area = FindCreatureArea(source->activate_area->name);
                    if (creature->activation_area != nullptr)
                        creature->activation = 1;
                }
            }
            creature->difficulty = source->activation_difficulty;
            creature->min_respawns = source->min_respawn_count;
            creature->max_respawns = source->max_respawn_count;
            creature->min_respawn_time = source->min_respawn_time;
            creature->max_respawn_time = source->max_respawn_time;
            creature->stagger_start = source->start_stagger;
            creature->view_distance = source->view_distance;
            creature->hear_distance = source->hear_distance;
            creature->max_view_height = source->max_view_height;
            creature->negative_min_view_height = source->min_view_height;
        }
    }
    if (aieditorsettings.current_path_type == -1 && LevelCharacterGlobalIDFn != nullptr)
        aieditorsettings.current_path_type = LevelCharacterGlobalIDFn(0);
    if (ClearAICreaturesFn != nullptr)
        ClearAICreaturesFn();
    if (aieditorsettings.current_area_name[0] != 0) {
        CreatureEditorRecord *creature = (CreatureEditorRecord *)NuLinkedListGetHead(&aieditor->creatures);
        while (creature != nullptr) {
            if (NuStrICmp(creature->name, aieditorsettings.current_area_name) == 0) {
                aieditor->mode_selection_36930 = (EditorNamedEntry *)creature;
                aieditor_SetCurrentScript(creature->script_name, (AIEditorScriptSelection *)creature);
                break;
            }
            creature = (CreatureEditorRecord *)NuLinkedListGetNext(&aieditor->creatures, &creature->link);
        }
    }
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbResetParams(eduimenu_s *menu,
                                                                                           eduiitem_s *, unsigned int) {
    aieditor_SetCurrentScript(aieditorsettings.current_script_name, nullptr);
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->flags &= ~0x1e;
        memcpy(creature->script_params, aieditorsettings.current_script_params, sizeof(creature->script_params));
    }
    eduiMenuDetach(menu);
    eduiMenuDestroy(menu);
    reset_params_option = nullptr;
}

static __used__ __attribute__((force_align_arg_pointer)) void creatureEditor_cbSetScript(eduimenu_s *, eduiitem_s *item,
                                                                                         unsigned int) {
    if (__builtin_expect(item == nullptr || creatureEditor_Current() == nullptr, 1))
        return;
    char *name = AIScriptNameFromIx(aieditor->ai_system, item->data);
    if (name == nullptr)
        return;
    aieditor_SetCurrentScript(name, nullptr);
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (NuStrICmp(aieditorsettings.current_script_name, creature->script_name) != 0) {
        strcpy(creature->script_name, aieditorsettings.current_script_name);
        memcpy(creature->script_params, aieditorsettings.current_script_params, sizeof(creature->script_params));
        creature->flags = (creature->flags & ~0x1e) | aieditorsettings.current_script_flags;
    }
}

extern "C" {
    void aieditor_Reset(void) {
        aieditorsettings.current_path_type = -1;
        aieditorsettings.current_area_name[0] = 0;
        aieditorsettings.current_path_name[0] = 0;
        aieditorsettings.current_route_name[0] = 0;
        aieditor_SetCurrentScript((char *)"default", 0);
    }

    __attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
        NULISTHDR *list = &aieditor->creatures;
        for (NULISTLNK *link = NuLinkedListGetHead(list); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(record) + 0x38);
            if (check->first != node && check->second != node)
                continue;

            NUVEC direction;
            NuVecSub(&direction, &check->second->position, &check->first->position);
            NUVEC perpendicular;
            NuVecNorm(&perpendicular, &direction);
            f32 fraction = check->fraction;
            f32 radius;
            if (fraction > 1.0f)
                radius = check->second->radius;
            else if (fraction < 0.0f)
                radius = check->first->radius;
            else
                radius = check->second->radius * fraction + check->first->radius * (1.0f - fraction);

            f32 old_y = record->position.y;
            record->position = check->first->position;
            f32 normalized_x = perpendicular.x;
            perpendicular.x = perpendicular.z * radius;
            perpendicular.z = -normalized_x * radius;
            NUVEC scaled;
            NuVecScale(&scaled, &direction, fraction);
            NuVecAdd(&record->position, &record->position, &scaled);
            NuVecScale(&scaled, &perpendicular, check->width);
            NuVecAdd(&record->position, &record->position, &scaled);
            if (fabsf(old_y - record->position.y) > 1.5f)
                record->position.y = old_y;
            record->angle = NuAngAdd(static_cast<i32>(NuAtan2(direction.x, direction.z) * 10430.378f), check->angle);
        }
    }

}

static void pathEditor_PathNodeMoved(EDAIPATHNODE_s *node) {
    if (!(node->flags & 0x80) && NuSpecialExistsFn(&aieditor->cursor_platform)) {
        node->platform = aieditor->cursor_platform;
        NuVecInvMtxTransform(&node->platform_position, &node->position,
                             NuSpecialGetDrawMtx(&aieditor->cursor_platform));
    } else {
        node->platform.scene = NULL;
        node->platform.special = NULL;
        node->platform.display_special = NULL;
    }
    creatureEditor_PathNodeMoved(node);
    locatorEditor_PathNodeMoved(node);
    if (AIPathNodeDeletedFn != NULL)
        AIPathNodeDeletedFn(aieditor->current_path->current_node);
    if (node->shared_node != NULL) {
        for (EDAIPATH_s *path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetHead(&aieditor->paths)); path != NULL;
             path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetNext(&aieditor->paths, &path->link))) {
            for (EDAIPATHNODE_s *other = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
                 other != NULL;
                 other = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetNext(&path->nodes, &other->link))) {
                if (other != node && other->shared_node == node->shared_node) {
                    other->radius = node->radius;
                    other->height_min = node->height_min;
                    other->height_max = node->height_max;
                    other->position = node->position;
                    other->platform = node->platform;
                    other->platform_position = node->platform_position;
                    creatureEditor_PathNodeMoved(other);
                    locatorEditor_PathNodeMoved(other);
                    break;
                }
            }
        }
    }
}

__attribute__((optimize("O2", "tree-vectorize"))) eduimenu_s *pathEditor_Process(nupad_s *pad) {
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, aieditor_cbCancelMainMenu, "Options");
        if (__builtin_expect(menu == NULL, 0))
            return NULL;
        eduiMenuAddItem(
            menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode, "Select Editor Mode"));
        if (aieditor->current_path && aieditor->current_path->current_node)
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbRenameNodeMenu, "Rename Node"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbSelectPathMenu, "Select Path Network"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, "Save AI Data"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbGoToPlayer, "Go To Player"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbMovePlayer, "Move Player"));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbCreatePath, "Create Path Network"));
        if (aieditor->current_path) {
            if (aieditor->current_path->current_node)
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbShareNodeMenu, "Share node with..."));
            if (aieditor->current_path && !(aieditor->current_path->flags & 1))
                eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDeletePath, "Delete Path Network"));
            if (aieditor->current_path && !(aieditor->current_path->flags & 1))
                eduiMenuAddItem(menu,
                                eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbRenamePathMenu, "Rename Path Network"));
        }
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, static_cast<i8>(aieditorsettings.path_flags) >> 7, 1,
                                                   aieditor_cbSolidPathDisplayToggle, "Solid Path Display"));
        i32 row = 2;
        if (aieditor->current_path) {
            EDAIPATHNODE_s *node = aieditor->current_path->current_node;
            EDAIPATHNODE_s *nearest = aieditor->current_path->nearest_node;
            EDAIPATHCNX_s *connection = NULL;
            if (node && nearest) {
                for (i32 i = 0; i < 8; ++i)
                    if (node->connections[i].node == nearest) {
                        connection = &node->connections[i];
                        break;
                    }
            }
            if (connection && naipathcnxtypes != 0) {
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, NULL, "================="));
                for (i32 i = 0; i < naipathcnxtypes; ++i) {
                    eduiMenuAddItem(menu, eduiItemToggleCreate(
                                              i, attr, (connection->flags & aipathcnxtypes[i].connection_flag) != 0,
                                              i + 2, pathEditor_cbCnxFlagsToggle, aipathcnxtypes[i].name));
                    row = i + 3;
                }
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, NULL, "================="));
            }
            node = aieditor->current_path->current_node;
            if (node) {
                eduiMenuAddItem(menu, eduiItemToggleCreate(0x80, attr, (node->flags >> 7) & 1, row,
                                                           pathEditor_cbNodeFlagsToggle, "Never On A Platform"));
                ++row;
            }
        }
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 6) & 1), row,
                                                   aieditor_cbStopPlatformsToggle, "Stop Platforms"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags2 >> 1) & 1), row + 1,
                                                   pathEditor_cbDrawWallsplinesToggle, "Draw Wallsplines"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 1) & 1), row + 2,
                                                   aieditor_cbDrawAllToggle, "Draw All Paths"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 4) & 1), row + 3,
                                                   aieditor_cbSnapHeightToggle, "Snap Height"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -((aieditorsettings.path_flags >> 3) & 1), row + 4,
                                                   aieditor_cbShowCreaturesToggle, "Show Creatures"));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, near_clip_at_cursor, row + 5, cbNearClipAtCursor,
                                                   "Near Clip At Cursor"));
        return menu;
    }
    EDAIPATH_s *path = aieditor->current_path;
    if (__builtin_expect(!path, 0))
        return NULL;
    EDAIPATHNODE_s *previous = path->current_node;
    if (!(path->flags & 1))
        aieditorsettings.path_flags &= ~1;
    if (aieditorsettings.path_flags & 1) {
        AIPATH_s *runtime = aieditor->runtime_path;
        f32 best = 3.402823466e38f;
        i32 nearest = -1;
        if (runtime && runtime->node_count) {
            for (i32 i = 0; i < runtime->node_count; ++i) {
                NUVEC delta;
                AIPATHNODE_s *node = &runtime->nodes[i];
                f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
                if (!(distance < best && distance < node->radius_squared)) {
                    continue;
                }
                best = distance;
                nearest = i;
            }
        }
        path->runtime_nearest = static_cast<i16>(nearest);
        if (pad->digital_buttons_pressed & 0x40) {
            path = aieditor->current_path;
            if (__builtin_expect(path->runtime_nearest >= 0, 0))
                path->runtime_start = path->runtime_nearest;
        }
        if (pad->digital_buttons_pressed & 0x10) {
            path = aieditor->current_path;
            if (__builtin_expect(path->runtime_nearest >= 0, 1))
                path->runtime_end = path->runtime_nearest;
        }
        if (pad->digital_buttons_pressed & 0x100) {
            path = aieditor->current_path;
            if (__builtin_expect(path->runtime_nearest >= 0, 1)) {
                path->runtime_start = path->runtime_nearest;
                edcamSetPos(&aieditor->runtime_path->nodes[path->runtime_nearest].position);
                path = aieditor->current_path;
            }
            EDAIPATHNODE_s *node = NULL;
            if (path != NULL) {
                f32 nearest_distance = FLT_MAX;
                for (EDAIPATHNODE_s *candidate = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
                     candidate != NULL; candidate = reinterpret_cast<EDAIPATHNODE_s *>(
                                            NuLinkedListGetNext(&path->nodes, &candidate->link))) {
                    NUVEC delta;
                    const f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &candidate->position, &delta);
                    if (distance < nearest_distance) {
                        const f32 height = aieditor->cursor_position.y - candidate->position.y;
                        const f32 upper = NuFmax(0.2f, candidate->height_max);
                        const f32 lower = NuFmin(-0.2f, candidate->height_min);
                        if (height <= upper && height >= lower) {
                            node = candidate;
                            nearest_distance = distance;
                        }
                    }
                }
            }
            path->current_node = node;
            if (aieditor->current_path->current_node)
                edcamSetPos(&aieditor->current_path->current_node->position);
        }
        return NULL;
    }
    if (__builtin_expect(!!(pad->digital_buttons & 0x40), 1)) {
        EDAIPATHNODE_s *nearest = path->nearest_node;
        if (!nearest) {
            if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x40), 1)) {
                EDAIPATHNODE_s *node = NULL;
                if (path->node_count <= 253) {
                    node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&aieditor->free_path_nodes));
                    if (node) {
                        NuLinkedListRemove(&aieditor->free_path_nodes, &node->link);
                        NuLinkedListAppend(&path->nodes, &node->link);
                        ++path->node_count;
                        node->position = aieditor->camera_position;
                        node->radius = default_path_node_radius;
                        node->height_max = default_path_heighttol;
                        node->height_min = -default_path_heighttol;
                    }
                }
                path->current_node = node;
                if (previous) {
                    path = aieditor->current_path;
                    node = path->current_node;
                    if (node) {
                        node->radius = previous->radius;
                        node->height_max = previous->height_max;
                        node->height_min = previous->height_min;
                        bool connected = pathEditor_ConnectNodes(node, previous);
                        if (!connected) {
                            DestroyAIPathNode(node, path);
                            aieditor->current_path->current_node = previous;
                        }
                    } else
                        aieditor->current_path->current_node = previous;
                }
            }
        } else if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x40), 0)) {
            path->current_node = nearest;
            NUVEC position = {nearest->position.x, aieditor->cursor_position.y, nearest->position.z};
            edcamSetPos(&position);
        } else if (path->current_node && path->current_node == nearest) {
            f32 old_x = nearest->position.x, old_z = nearest->position.z;
            nearest->position = aieditor->camera_position;
            pathEditor_PathNodeMoved(nearest);
            locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
            if (AIPathNodeDeletedFn)
                AIPathNodeDeletedFn(aieditor->current_path->current_node);
            if (__builtin_expect(!!(pad->digital_buttons & 0x200), 1)) {
                path = aieditor->current_path;
                f32 dx = path->current_node->position.x - old_x, dz = path->current_node->position.z - old_z;
                for (EDAIPATHNODE_s *node = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes)); node;
                     node = reinterpret_cast<EDAIPATHNODE_s *>(
                         NuLinkedListGetNext(&aieditor->current_path->nodes, &node->link))) {
                    if (__builtin_expect(node != aieditor->current_path->current_node, 0)) {
                        node->position.x += dx;
                        node->position.z += dz;
                        locatorEditor_PathNodeMoved(node);
                        if (AIPathNodeDeletedFn)
                            AIPathNodeDeletedFn(node);
                    }
                }
            }
        }
    }
    if (pad->digital_buttons_pressed & 0x20) {
        path = aieditor->current_path;
        EDAIPATHNODE_s *node = path->current_node, *nearest = path->nearest_node;
        if (node && nearest && node != nearest) {
            bool connected = pathEditor_HasConnection(node, nearest);
            if (connected) {
                eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDisconnectNodeMenu,
                                                  "Disconnect current path node??");
                if (menu) {
                    eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDisconnectPathNode, "No"));
                    eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDisconnectPathNode, "Yes"));
                }
                return menu;
            }
            connected = pathEditor_ConnectNodes(node, nearest);
            NUVEC position = {nearest->position.x, aieditor->cursor_position.y, nearest->position.z};
            edcamSetPos(&position);
        }
    }
    if (pad->digital_buttons_pressed & 0x10) {
        path = aieditor->current_path;
        if (path->current_node && path->current_node == path->nearest_node) {
            eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDeleteNodeMenu,
                                              "Delete current path node??");
            if (menu) {
                eduiMenuAddItem(menu, eduiItemSelCreate(0, attr, 0, 0, pathEditor_cbDeletePathNode, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDeletePathNode, "Yes"));
            }
            return menu;
        }
    }
    if (pad->digital_buttons_pressed & 0x100) {
        if (aieditorsettings.path_flags & 2) {
            EDAIPATHNODE_s *nearest_node = NULL;
            EDAIPATH_s *nearest_path = NULL;
            f32 best = 3.402823466e38f;
            for (path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetHead(&aieditor->paths)); path;
                 path = reinterpret_cast<EDAIPATH_s *>(NuLinkedListGetNext(&aieditor->paths, &path->link))) {
                EDAIPATHNODE_s *node = pathEditor_GetNearestNode(path, 0);
                if (node) {
                    NUVEC delta;
                    f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &node->position, &delta);
                    if (distance < best) {
                        nearest_path = path;
                        nearest_node = node;
                        best = distance;
                    }
                }
            }
            if (nearest_path && nearest_node) {
                aieditor->current_path = nearest_path;
                nearest_path->current_node = nearest_node;
                edcamSetPos(&nearest_node->position);
            }
        } else {
            path = aieditor->current_path;
            EDAIPATHNODE_s *nearest_node = NULL;
            if (path != NULL) {
                f32 nearest_distance = FLT_MAX;
                for (EDAIPATHNODE_s *candidate = reinterpret_cast<EDAIPATHNODE_s *>(NuLinkedListGetHead(&path->nodes));
                     candidate != NULL; candidate = reinterpret_cast<EDAIPATHNODE_s *>(
                                            NuLinkedListGetNext(&path->nodes, &candidate->link))) {
                    NUVEC delta;
                    const f32 distance = NuVecXZDistSqr(&aieditor->cursor_position, &candidate->position, &delta);
                    if (distance < nearest_distance) {
                        const f32 height = aieditor->cursor_position.y - candidate->position.y;
                        const f32 upper = NuFmax(0.2f, candidate->height_max);
                        const f32 lower = NuFmin(-0.2f, candidate->height_min);
                        if (height <= upper && height >= lower) {
                            nearest_node = candidate;
                            nearest_distance = distance;
                        }
                    }
                }
            }
            path->current_node = nearest_node;
            EDAIPATHNODE_s *node = aieditor->current_path->current_node;
            if (node) {
                if (edpath_addoffset.x != 0 || edpath_addoffset.y != 0 || edpath_addoffset.z != 0) {
                    NuVecAdd(&node->position, &node->position, &edpath_addoffset);
                    pathEditor_PathNodeMoved(aieditor->current_path->current_node);
                    locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
                }
                edcamSetPos(&aieditor->current_path->current_node->position);
            }
        }
    }
    aieditor->flags &= ~1;
    path = aieditor->current_path;
    EDAIPATHNODE_s *node = path->current_node;
    if (node && node == path->nearest_node) {
        if (pad->digital_buttons & (0x8000 | 0x2000)) {
            node->radius *= (pad->digital_buttons & 0x8000) ? .99f : 1.01f;
            if (node->radius < .01f)
                node->radius = .01f;
            pathEditor_PathNodeMoved(node);
            locatorEditor_PathNodeMoved(aieditor->current_path->current_node);
            if (AIPathNodeDeletedFn)
                AIPathNodeDeletedFn(aieditor->current_path->current_node);
            path = aieditor->current_path;
        }
        if (aieditorsettings.path_flags & 0x80) {
            if (pad->digital_buttons & 0x1000) {
                aieditor->flags |= 4;
                node = path->current_node;
                f32 step = node->height_max * .05f;
                if (pad->digital_buttons & 4)
                    node->height_max += step;
                else if (pad->digital_buttons & 1)
                    node->height_max -= step;
                if (node->height_max < .01f)
                    node->height_max = .01f;
            } else if (pad->digital_buttons & 0x4000) {
                aieditor->flags |= 4;
                node = path->current_node;
                f32 step = -node->height_min * .05f;
                if (pad->digital_buttons & 4)
                    node->height_min += step;
                else if (pad->digital_buttons & 1)
                    node->height_min -= step;
                if (node->height_min > -.01f)
                    node->height_min = -.01f;
            }
        }
        aieditor->flags |= 1;
    }
    i32 direction = 0;
    bool named_only = false;
    if (pad->digital_buttons & 0x100) {
        if (pad->digital_buttons_pressed & 8)
            direction = 1;
        else if (pad->digital_buttons_pressed & 2)
            direction = -1;
        else if (pad->digital_buttons_pressed & 4) {
            direction = 1;
            named_only = true;
        } else if (pad->digital_buttons_pressed & 1) {
            direction = -1;
            named_only = true;
        }
    }
    if (!direction && !(aieditorsettings.path_flags & 0x80)) {
        if (pad->digital_buttons_pressed & 0x1000)
            direction = 1;
        else if (pad->digital_buttons_pressed & 0x4000)
            direction = -1;
    }
    if (direction) {
        path = aieditor->current_path;
        if (__builtin_expect(!!(path->current_node), 1))
            path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                direction > 0 ? NuLinkedListGetNext(&path->nodes, &path->current_node->link)
                              : NuLinkedListGetPrev(&path->nodes, &path->current_node->link));
        path = aieditor->current_path;
        if (!path->current_node)
            path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(direction > 0 ? NuLinkedListGetHead(&path->nodes)
                                                                                  : NuLinkedListGetTail(&path->nodes));
        path = aieditor->current_path;
        if (named_only && path->current_node && NuStrLen(path->current_node->name) == 0) {
            path = aieditor->current_path;
            EDAIPATHNODE_s *start = path->current_node;
            do {
                path = aieditor->current_path;
                path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                    direction > 0 ? NuLinkedListGetNext(&path->nodes, &path->current_node->link)
                                  : NuLinkedListGetPrev(&path->nodes, &path->current_node->link));
                path = aieditor->current_path;
                if (!path->current_node)
                    path->current_node = reinterpret_cast<EDAIPATHNODE_s *>(
                        direction > 0 ? NuLinkedListGetHead(&path->nodes) : NuLinkedListGetTail(&path->nodes));
                path = aieditor->current_path;
                if (!path->current_node || path->current_node == start)
                    break;
            } while (NuStrLen(path->current_node->name) == 0);
        }
        path = aieditor->current_path;
        if (path->current_node)
            edcamSetPos(&path->current_node->position);
    }
    path = aieditor->current_path;
    path->nearest_node = pathEditor_GetNearestNode(path, 1);
    return NULL;
}

extern "C" {
    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathNodeDeleted(EDAIPATHNODE_s *node) {
        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;) {
            NULISTLNK *next = NuLinkedListGetNext(&aieditor->creatures, link);
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(record) + 0x38);
            if (check->first == node || check->second == node) {
                pathEditor_OnPathCheck(&record->position, check, aieditor->current_path, 0.0f);
                if (check->on_path == 0) {
                    NuLinkedListRemove(&aieditor->creatures, link);
                    memset(record, 0, sizeof(*record));
                    NuLinkedListAppend(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c), link);
                    if (aieditor->mode_selection_36930 == reinterpret_cast<EditorNamedEntry *>(record)) {
                        aieditor->mode_selection_36930 = nullptr;
                    }
                }
            }
            link = next;
        }
    }

}

static __used__ void pathEditor_cbDisconnectPathNode(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0) {
        EDAIPATHNODE_s *node = aieditor->current_path->current_node;
        EDAIPATHNODE_s *other = aieditor->current_path->other_node;
        if (node != nullptr && other != nullptr && node != other) {
            i32 i;
            i32 j;
#define DISCONNECT_OTHER(J)                                                                                            \
    if (__builtin_expect(other->connections[J].node == node, 0)) {                                                                          \
        j = J;                                                                                                         \
        goto disconnect_found;                                                                                         \
    }
#define DISCONNECT_NODE(I)                                                                                             \
    if (node->connections[I].node == other) {                                                                          \
        i = I;                                                                                                         \
        DISCONNECT_OTHER(0)                                                                                            \
        DISCONNECT_OTHER(1)                                                                                            \
        DISCONNECT_OTHER(2)                                                                                            \
        DISCONNECT_OTHER(3)                                                                                            \
        DISCONNECT_OTHER(4)                                                                                            \
        DISCONNECT_OTHER(5)                                                                                            \
        DISCONNECT_OTHER(6)                                                                                            \
        DISCONNECT_OTHER(7)                                                                                            \
    }
            DISCONNECT_NODE(0)
            DISCONNECT_NODE(1)
            DISCONNECT_NODE(2)
            DISCONNECT_NODE(3)
            DISCONNECT_NODE(4)
            DISCONNECT_NODE(5)
            DISCONNECT_NODE(6)
            DISCONNECT_NODE(7)
#undef DISCONNECT_NODE
#undef DISCONNECT_OTHER
            goto disconnected;
        disconnect_found:
            memset(&node->connections[i], 0, sizeof(EDAIPATHCNX_s));
            memset(&other->connections[j], 0, sizeof(EDAIPATHCNX_s));
        disconnected:
            creatureEditor_PathNodeDeleted(aieditor->current_path->current_node);
            locatorEditor_PathNodeDeleted(aieditor->current_path->current_node);
            if (AIPathNodeMovedFn != nullptr) {
                AIPathNodeMovedFn(aieditor->current_path->current_node);
            }
        }
    }
    aieditor_ClearMainMenu();
}

static __used__ void pathEditor_cbDeletePathNode(eduimenu_s *, eduiitem_s *item, u32) {
    if (item != nullptr && item->data != 0) {
        EDAIPATH_s *path = aieditor->current_path;
        EDAIPATHNODE_s *node = path->current_node;
        if (node != nullptr && node == path->other_node) {
            DestroyAIPathNode(node, path);
            creatureEditor_PathNodeDeleted(node);
            locatorEditor_PathNodeDeleted(node);
            if (AIPathNodeMovedFn != nullptr) {
                AIPathNodeMovedFn(node);
            }
            if (node == aieditor->current_path->current_node) {
                aieditor->current_path->current_node = nullptr;
            }
        }
    }
    aieditor_ClearMainMenu();
}

extern "C" {
    __attribute__((optimize("O2", "omit-frame-pointer"))) void creatureEditor_PathDeleted(EDAIPATH_s *path) {
        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;) {
            NULISTLNK *next = NuLinkedListGetNext(&aieditor->creatures, link);
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            if (record->path == path) {
                NuLinkedListRemove(&aieditor->creatures, link);
                memset(record, 0, sizeof(*record));
                NuLinkedListAppend(reinterpret_cast<NULISTHDR *>(reinterpret_cast<u8 *>(aieditor) + 0x3691c), link);
                if (aieditor->mode_selection_36930 == reinterpret_cast<EditorNamedEntry *>(record)) {
                    aieditor->mode_selection_36930 = nullptr;
                }
            }
            link = next;
        }
    }

}

static __used__ void pathEditor_cbDeletePath(eduimenu_s *parent, eduiitem_s *item, u32) {
    if (item == nullptr) {
        return;
    }
    switch (item->data) {
        case 0: {
            eduimenu_s *menu = eduiMenuCreate(240, 90, 240, 250, ed_fnt, nullptr, "Delete current path??");
            if (menu != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(2, attr, 0, 0, pathEditor_cbDeletePath, "No"));
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, pathEditor_cbDeletePath, "Yes"));
                eduiMenuAttach(parent, menu);
            }
            break;
        }
        case 1: {
            if (aieditor->current_path == nullptr || (aieditor->current_path->flags & 1)) {
                return;
            }
            creatureEditor_PathDeleted(aieditor->current_path);
            locatorEditor_PathDeleted(aieditor->current_path);
            if (AIPathDeletedFn != nullptr) {
                reinterpret_cast<void (*)(EDAIPATH_s *)>(AIPathDeletedFn)(aieditor->current_path);
            }
            EDAIPATH_s *path = aieditor->current_path;
            if (path != nullptr) {
                EDAIPATHNODE_s *node;
                while ((node = (EDAIPATHNODE_s *)NuLinkedListGetHead(&path->nodes)) != nullptr) {
                    DestroyAIPathNode(node, path);
                }
                NuLinkedListRemove(&aieditor->paths, &path->link);
                memset(path, 0, sizeof(*path));
                NuLinkedListAppend(&aieditor->free_paths, &path->link);
            }
            aieditor->current_path = (EDAIPATH_s *)NuLinkedListGetHead(&aieditor->paths);
            aieditor_ClearMainMenu();
            break;
        }
        case 2:
            aieditor_ClearMainMenu();
            break;
    }
}

__attribute__((force_align_arg_pointer)) i32 creatureEditor_CalculatePos(EDCREATURE_s *creature, i32 index,
                                                                         nuvec_s *position, i32 check_path) {
    CreaturePositionRecord *record = reinterpret_cast<CreaturePositionRecord *>(creature);
    if (index == 0) {
        *position = record->position;
        return 1;
    }
    i32 across = record->across_count;
    i32 column = index % across;
    nuvec_s offset;
    offset.x = ((column + 1) / 2) * record->x_spacing;
    if (column & 1)
        offset.x = -offset.x;
    offset.y = 0.0f;
    offset.z = -record->z_spacing * (index / record->across_count);
    NuVecRotateY(&offset, &offset, record->angle);
    NuVecAdd(position, &offset, &record->position);
    if (check_path != 0) {
        EDAIPATHCHECK_s result;
        pathEditor_OnPathCheck(position, &result, record->path, 0.0f);
        return result.on_path;
    }
    return 1;
}

void creatureEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    char set_name[32];
    nuvec_s displacement;
    if (GlobalCharacterRenderFn == nullptr)
        return;
    i32 count = NuLinkedListCheck(&aieditor->creatures);
    i32 text_x = (x + 10) * 16;
    NuQFntPrintEx(system_qfont, text_x, y * 8 - 40, 16, "Creature Editor  (%d placed)", count);
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, xscale, yscale);

    u8 *display = reinterpret_cast<u8 *>(aieditor->mode_selection_36930);
    if (display == nullptr)
        display = *reinterpret_cast<u8 **>(aieditor->unknown_3692c);
    if (display != nullptr) {

        f32 distance =
            NuVecXZDist(reinterpret_cast<nuvec_s *>(display + 0x28), &aieditor->camera_position, &displacement);
        if (*reinterpret_cast<u32 *>(display + 0x68) & 0x20) {
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 120, 16, "\"%s\", xzrng=%.2f (NotLowEnd)", display + 8,
                          distance);
        } else {
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 120, 16, "\"%s\", xzrng=%.2f", display + 8, distance);
        }
        u8 set = display[0x5a];

        if (set != 0) {
            sprintf(set_name, "Set=%d", set);
        } else {
            strcpy(set_name, "Set=NONE");
        }
        if (display[0x18] != 0) {
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 240, 16, "Script = \"%s\", %s", display + 0x18, set_name);
        } else {
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 240, 16, "Script = NONE, %s", set_name);
        }
        EDLOCATOR_s *area = *reinterpret_cast<EDLOCATOR_s **>(display + 0x80);
        if (area != nullptr)
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 360, 16, "Area = \"%s\"", area->name);
        EDLOCATOR_s *locator = *reinterpret_cast<EDLOCATOR_s **>(display + 0x84);
        if (locator != nullptr)
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 480, 16, "Locator = \"%s\"", locator->name);
    }
    NuQFntPrintEx(system_qfont, text_x, y * 8 + 600, 16, "SQR - Options");
    u8 *nearest_for_menu = *reinterpret_cast<u8 **>(aieditor->unknown_3692c);
    if (nearest_for_menu == nullptr) {
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 720, 16, "X - Create creature");
        if (aieditor->mode_selection_36930 != nullptr)
            NuQFntPrintEx(system_qfont, text_x, y * 8 + 840, 16, "TRI - Deselect selected");
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 960, 16, "LLEFT/LRight - Rotate");
    } else if (nearest_for_menu != reinterpret_cast<u8 *>(aieditor->mode_selection_36930)) {
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 720, 16, "X - Select creature");
    } else {
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 720, 16, "X - Move selected");
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 840, 16, "TRI - Delete selected");
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 960, 16, "LLEFT/LRight - Rotate");
    }
    if (aieditor->mode_selection_36930 != nullptr && aieditor->nearest_locator != nullptr)
        NuQFntPrintEx(system_qfont, text_x, y * 8 + 720, 16, "O - Set locator.");

    for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
         link = NuLinkedListGetNext(&aieditor->creatures, link)) {
        u8 *record = reinterpret_cast<u8 *>(link);
        EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(record);
        u8 *nearest_for_render = *reinterpret_cast<u8 **>(aieditor->unknown_3692c);
        i32 render_colour = record == reinterpret_cast<u8 *>(aieditor->mode_selection_36930)
                                ? (record == nearest_for_render ? 0xff0000ff : 0x800000ff)
                                : (record == nearest_for_render ? -1 : 0);
        for (i32 group = 0; group < record[0x5b]; ++group, render_colour = 0) {
            u32 valid_positions = *reinterpret_cast<u32 *>(record + 0x54);
            if (!((static_cast<u64>(valid_positions) >> (group & 63)) & 1) || !creatureEditor_IsSelectable(creature)) {
                continue;
            }
            nuvec_s position;
            creatureEditor_CalculatePos(creature, group, &position, 0);
            i16 angle = *reinterpret_cast<i16 *>(record + 0x34);
            i16 type = *reinterpret_cast<i16 *>(record + 0x58);
            GlobalCharacterRenderFn(&position, angle, type, render_colour, creature);
            EDLOCATOR_s *locator = *reinterpret_cast<EDLOCATOR_s **>(record + 0x84);
            if (locator != nullptr) {
                NURND_VERTEX3D line[2];
                line[0].position = *reinterpret_cast<nuvec_s *>(record + 0x28);
                line[1].position = locator->position;
                line[0].colour = render_colour;
                line[1].colour = render_colour;
                AiRndrLine3d(line, nullptr, nullptr);
            }
            EDLOCATOR_s *respawn = *reinterpret_cast<EDLOCATOR_s **>(record + 0x88);
            if (respawn != nullptr) {
                NURND_VERTEX3D line[2];
                line[0].position = *reinterpret_cast<nuvec_s *>(record + 0x28);
                line[1].position = respawn->position;
                line[0].position.y += 0.1f;
                line[1].position.y += 0.1f;
                line[0].colour = render_colour + 0x8000;
                line[1].colour = render_colour + 0x8000;
                AiRndrLine3d(line, nullptr, nullptr);
            }
        }
    }
    if (*reinterpret_cast<u8 **>(aieditor->unknown_3692c) == nullptr &&
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0 &&
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(aieditor) + 0x42ea4) < 1.0f &&
        aieditorsettings.current_path_type >= 0) {
        GlobalCharacterRenderFn(&aieditor->camera_position, static_cast<i16>(aieditorsettings.area_rotation),
                                aieditorsettings.current_path_type, 0, nullptr);
    }
    pathEditorDrawPaths();
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

static __used__
    __attribute__((always_inline)) CREATURE_EDITOR_REGPARM1 void creatureEditor_Updated(EDCREATURE_s *creature) {
    CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(creature);
    record->valid_positions = 1;
    for (i32 i = 1; i < record->group_count; ++i) {
        nuvec_s position;
        if (creatureEditor_CalculatePos(creature, i, &position, 1)) {
            record->valid_positions |= ((i & 0x20) == 0) << i;
        }
    }
}

static __used__ void creatureEditor_cb_zspacing(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->z_spacing = reinterpret_cast<edui_slider_s *>(item)->value;
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_xspacing(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->x_spacing = reinterpret_cast<edui_slider_s *>(item)->value;
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_nacross(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->across_count = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

static __used__ void creatureEditor_cb_ngroup(eduimenu_s *, eduiitem_s *item, unsigned int) {
    CreatureEditorRecord *creature = creatureEditor_Current();
    if (creature != nullptr) {
        creature->group_count = static_cast<i32>(reinterpret_cast<edui_slider_s *>(item)->value);
        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creature));
    }
}

__attribute__((optimize("O2"), force_align_arg_pointer)) eduimenu_s *creatureEditor_Process(nupad_s *pad) {
    char base_name[0x10];
    if (pad->digital_buttons_pressed & 0x80) {
        eduimenu_s *menu =
            eduiMenuCreate(200, 70, 240, 330, ed_fnt, aieditor_cbCancelMainMenu, const_cast<char *>("Options"));
        if (__builtin_expect(menu == nullptr, 0))
            return nullptr;
        eduiMenuAddItem(menu, eduiItemSelCreate(AIEDITOR_ROUTES, attr, 0, 0, aieditor_cvSelectEditorMode,
                                                const_cast<char *>("Select Editor Mode")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, aieditor_cbSave, const_cast<char *>("Save AI Data")));
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, attr, 0, 0, aieditor_cbGoToPlayer, const_cast<char *>("Go To Player")));
        eduiMenuAddItem(menu,
                        eduiItemSelCreate(1, attr, 0, 0, aieditor_cbMovePlayer, const_cast<char *>("Move Player")));
        eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbSelectType,
                                                const_cast<char *>("Select Creature Type")));
        CreatureEditorRecord *creature = creatureEditor_Current();
        if (__builtin_expect(creature != nullptr, 1)) {
            if (AIScriptNameFromIx(aieditor->ai_system, 0) != nullptr) {
                eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbSelectScript,
                                                        const_cast<char *>("Select Script")));
            }
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbScriptParams,
                                                    const_cast<char *>("Script Params")));
            eduiMenuAddItem(menu, eduiItemSliderCreateInt(1, attr, 0, creatureEditor_cb_assigntoset, 0,
                                                          aisys_maxnumcreaturesets, creatureEditor_Current()->set,
                                                          const_cast<char *>("Assigned To Set")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbRenameCreatureMenu,
                                                    const_cast<char *>("Rename Creature")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbActivationMenu,
                                                    const_cast<char *>("Activation Conditions")));
            eduiMenuAddItem(menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbRespawnMenu,
                                                    const_cast<char *>("Respawn Values")));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbGroupMenu, const_cast<char *>("Group Values")));
            eduiMenuAddItem(
                menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbVisionMenu, const_cast<char *>("Vision")));
            eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, creatureEditor_Current()->flags & 1, 1,
                                                       creatureEditor_cbFlagsToggle,
                                                       const_cast<char *>("Ignore Wall Splines")));
            eduiMenuAddItem(menu, eduiItemToggleCreate(0x20, attr, (creatureEditor_Current()->flags >> 5) & 1, 2,
                                                       creatureEditor_cbFlagsToggle,
                                                       const_cast<char *>("Not On LowEnd Device")));
        }
        const i32 toggle_offset = creature != nullptr ? 2 : 0;
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, attr, -i32(aieditorsettings.stop_platforms), 1 + toggle_offset,
                                             aieditor_cbStopPlatformsToggle, const_cast<char *>("Stop Platforms")));
        eduiMenuAddItem(menu,
                        eduiItemToggleCreate(1, attr, -i32(aieditorsettings.snap_height_display), 2 + toggle_offset,
                                             aieditor_cbSnapHeightToggle, const_cast<char *>("Snap Height")));
        eduiMenuAddItem(menu, eduiItemToggleCreate(1, attr, -i32(aieditorsettings.show_creatures_set),
                                                   3 + toggle_offset, aieditor_cbShowCreaturesSetToggle,
                                                   const_cast<char *>("Show Current Set")));
        return menu;
    }

    if ((pad->digital_buttons & 0x40) == 0) {
        if (pad->digital_buttons_pressed & 0x10) {
            CreatureEditorRecord *selected = creatureEditor_Current();
            CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
            if (selected != nullptr && selected == nearest) {
                eduimenu_s *menu = eduiMenuCreate(200, 70, 240, 270, ed_fnt, pathEditor_cbCancelDeleteCreatureMenu,
                                                  const_cast<char *>("Delete creature??"));
                if (menu == nullptr)
                    return nullptr;
                eduiMenuAddItem(
                    menu, eduiItemSelCreate(0, attr, 0, 0, creatureEditor_cbDeleteCreature, const_cast<char *>("No")));
                eduiMenuAddItem(
                    menu, eduiItemSelCreate(1, attr, 0, 0, creatureEditor_cbDeleteCreature, const_cast<char *>("Yes")));
                return menu;
            }
            EDLOCATOR_s *locator = aieditor->nearest_locator;
            if (__builtin_expect(selected != nullptr && locator != nullptr, 0)) {
                if (selected->respawn_locator == locator)
                    selected->respawn_locator = nullptr;
                else if (selected->path == locator->path)
                    selected->respawn_locator = locator;
            } else {
                aieditor->mode_selection_36930 = nullptr;
            }
        } else if (pad->digital_buttons_pressed & 0x20) {
            CreatureEditorRecord *selected = creatureEditor_Current();
            EDLOCATOR_s *locator = aieditor->nearest_locator;
            if (selected != nullptr && locator != nullptr) {
                if (__builtin_expect(selected->locator == locator, 1))
                    selected->locator = nullptr;
                else if (selected->path == locator->path)
                    selected->locator = locator;
            }
        } else {
            CreatureEditorRecord *selected = creatureEditor_Current();
            bool change_selection = false;
            if ((pad->digital_buttons_pressed & 0x1000) == 0 &&
                !((pad->digital_buttons & 0x100) != 0 && (pad->digital_buttons_pressed & 8) != 0)) {
                if ((pad->digital_buttons & 0x100) != 0 && (pad->digital_buttons_pressed & 2) != 0) {
                    if (selected != nullptr)
                        aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(
                            NuLinkedListGetPrev(&aieditor->creatures, &selected->link));
                    if (creatureEditor_Current() == nullptr)
                        aieditor->mode_selection_36930 =
                            reinterpret_cast<EditorNamedEntry *>(NuLinkedListGetTail(&aieditor->creatures));
                    change_selection = true;
                } else if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x100), 1)) {
                    aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(creatureEditor_GetNearest(0));
                    change_selection = true;
                }
            } else {
                if (selected != nullptr)
                    aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(
                        NuLinkedListGetNext(&aieditor->creatures, &selected->link));
                if (creatureEditor_Current() == nullptr)
                    aieditor->mode_selection_36930 =
                        reinterpret_cast<EditorNamedEntry *>(NuLinkedListGetHead(&aieditor->creatures));
                change_selection = true;
            }

            if (change_selection) {
                CreatureEditorRecord *next = creatureEditor_Current();
                if (next != nullptr) {
                    aieditor->current_path = reinterpret_cast<EDAIPATH_s *>(next->path);
                    edcamSetPos(&next->position);
                    next = creatureEditor_Current();
                    aieditorsettings.current_path_type = next->character_type;
                    aieditor_SetCurrentScript(next->script_name,
                                              reinterpret_cast<const AIEditorScriptSelection *>(next));
                }
            } else if ((pad->digital_buttons & 0x100) == 0) {
                EDAIPATHCHECK_s *check = reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48);
                bool rotate = false;
                if (pad->digital_buttons & 0x2000) {
                    rotate = true;
                    CreatureEditorRecord *hover = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
                    if (selected != nullptr && selected == hover)
                        aieditorsettings.area_rotation = selected->angle;
                    i32 &step = *reinterpret_cast<i32 *>(aieditor->unknown_36934);
                    if (__builtin_expect(!!(pad->digital_buttons_pressed & 0x8000), 0))
                        step = 0x14;
                    else
                        step += 0x14;
                    if (step > 600)
                        step = 600;
                    aieditorsettings.area_rotation = NuAngAdd(aieditorsettings.area_rotation, step);
                } else if (pad->digital_buttons & 0x8000) {
                    rotate = true;
                    CreatureEditorRecord *hover = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
                    if (selected != nullptr && selected == hover)
                        aieditorsettings.area_rotation = selected->angle;
                    i32 &step = *reinterpret_cast<i32 *>(aieditor->unknown_36934);
                    if (pad->digital_buttons_pressed & 0x2000)
                        step = 0x14;
                    else
                        step += 0x14;
                    if (step > 600)
                        step = 600;
                    aieditorsettings.area_rotation = NuAngSub(aieditorsettings.area_rotation, step);
                } else if (pad->digital_buttons & 0x4000) {
                    rotate = true;
                    const i32 relative = NuAngSub(aieditorsettings.area_rotation, check->angle);
                    i32 quarter_turns = relative / 0x4000;
                    if (relative % 0x4000 > 0x2000)
                        ++quarter_turns;
                    else if (relative % 0x4000 < -0x2000)
                        --quarter_turns;
                    aieditorsettings.area_rotation =
                        NuAngAdd(quarter_turns << 14,
                                 reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48)->angle);
                }
                if (rotate) {
                    selected = creatureEditor_Current();
                    CreatureEditorRecord *hover = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
                    if (selected != nullptr && selected == hover) {
                        selected->angle = aieditorsettings.area_rotation;
                        check = reinterpret_cast<EDAIPATHCHECK_s *>(selected->path_check);
                        check->angle = NuAngSub(
                            aieditorsettings.area_rotation,
                            reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48)->angle);
                        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creatureEditor_Current()));
                    }
                }
            }
        }
    } else {
        CreatureEditorRecord *nearest = *reinterpret_cast<CreatureEditorRecord **>(aieditor->unknown_3692c);
        if (nearest == nullptr) {
            if ((pad->digital_buttons_pressed & 0x40) != 0 &&
                reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48)->on_path != 0) {
                CreatureEditorRecord *previous = creatureEditor_Current();

                u8 set = 0;
                if (previous == nullptr) {
                    NuStrNCpy(base_name, GlobalCharacterNameFn(aieditorsettings.current_path_type), 0xd);
                } else {
                    NuStrCpy(base_name, previous->name);
                    char *suffix = strrchr(base_name, '_');
                    if (suffix != nullptr)
                        *suffix = 0;
                    base_name[12] = 0;
                    set = creatureEditor_Current()->set;
                }

                AIEDITOR_RENDER_STATE *context = aieditor;
                CreatureEditorRecord *created = static_cast<CreatureEditorRecord *>(CreateCreature(
                    aieditorsettings.current_path_type, &aieditor->camera_position, aieditorsettings.area_rotation));
                context->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(created);
                created = creatureEditor_Current();
                if (created != nullptr) {
                    char name[0x20];
                    for (i32 number = 1;; ++number) {
                        sprintf(name, "%s_%d", base_name, number);
                        bool in_use = false;
                        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
                             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
                            if (NuStrICmp(name, reinterpret_cast<CreatureEditorRecord *>(link)->name) != 0) {
                                continue;
                            }
                            in_use = true;
                            break;
                        }
                        if (!in_use)
                            break;
                    }
                    strcpy(creatureEditor_Current()->name, name);
                    if (aieditorsettings.current_script_name[0] != 0)
                        strcpy(creatureEditor_Current()->script_name, aieditorsettings.current_script_name);
                    created = creatureEditor_Current();
                    memcpy(created->script_params, aieditorsettings.current_script_params,
                           sizeof(created->script_params));
                    created->path = aieditor->current_path;
                    created->flags = (created->flags & ~0x1eu) | aieditorsettings.current_script_flags;
                    memcpy(created->path_check, reinterpret_cast<u8 *>(aieditor) + 0x48, sizeof(created->path_check));
                    reinterpret_cast<EDAIPATHCHECK_s *>(created->path_check)->angle =
                        NuAngSub(created->angle, *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x60));
                    creatureEditor_Current()->set = set;
                    creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(creatureEditor_Current()));
                }
            }
        } else {
            if (!(pad->digital_buttons_pressed & 0x40)) {
                CreatureEditorRecord *selected = creatureEditor_Current();
                if (selected != nullptr) {
                    EDAIPATHCHECK_s *check =
                        reinterpret_cast<EDAIPATHCHECK_s *>(reinterpret_cast<u8 *>(aieditor) + 0x48);
                    if (check->on_path != 0) {
                        selected->position = aieditor->camera_position;
                        memcpy(selected->path_check, check, sizeof(selected->path_check));
                        reinterpret_cast<EDAIPATHCHECK_s *>(selected->path_check)->angle =
                            NuAngSub(selected->angle, check->angle);
                        creatureEditor_Updated(reinterpret_cast<EDCREATURE_s *>(selected));
                    } else {
                        edcamSetPos(&selected->position);
                    }
                }
            } else {
                aieditor->mode_selection_36930 = reinterpret_cast<EditorNamedEntry *>(nearest);
                aieditor->current_path = reinterpret_cast<EDAIPATH_s *>(nearest->path);
                aieditorsettings.area_rotation = nearest->angle;
                edcamSetPos(&nearest->position);
                aieditorsettings.current_path_type = nearest->character_type;
                aieditor_SetCurrentScript(nearest->script_name,
                                          reinterpret_cast<const AIEditorScriptSelection *>(nearest));
            }
        }
    }

    aieditor->nearest_locator = locatorEditor_GetNearest(1);
    *reinterpret_cast<EDCREATURE_s **>(aieditor->unknown_3692c) = creatureEditor_GetNearest(1);
    return nullptr;
}

extern "C" {
    __attribute__((optimize("O2", "omit-frame-pointer"), force_align_arg_pointer)) void
    creatureEditor_RenderAllCreatures(void) {
        if (GlobalCharacterRenderFn == nullptr)
            return;

        for (NULISTLNK *link = NuLinkedListGetHead(&aieditor->creatures); link != nullptr;
             link = NuLinkedListGetNext(&aieditor->creatures, link)) {
            CreatureEditorRecord *record = reinterpret_cast<CreatureEditorRecord *>(link);
            EDCREATURE_s *creature = reinterpret_cast<EDCREATURE_s *>(record);
            for (i32 group = 0; group < record->group_count; ++group) {
                if (((static_cast<u64>(record->valid_positions) >> group) & 1) == 0)
                    continue;
                NURND_VERTEX3D vertices[2];
                vertices[0].colour = 0xffffffff;
                vertices[1].colour = 0xffffffff;
                if (!creatureEditor_IsSelectable(creature))
                    continue;

                NUVEC position;
                creatureEditor_CalculatePos(creature, group, &position, 0);
                GlobalCharacterRenderFn(&position, static_cast<i16>(record->angle), record->character_type, 0,
                                        creature);

                if (record->locator != nullptr) {
                    EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(record->locator);
                    vertices[0].position = record->position;
                    vertices[1].position = locator->position;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                }
                if (record->respawn_locator != nullptr) {
                    EDLOCATOR_s *locator = reinterpret_cast<EDLOCATOR_s *>(record->respawn_locator);
                    vertices[0].colour = 0x8000;
                    vertices[1].colour = 0x8000;
                    vertices[0].position = record->position;
                    vertices[1].position = locator->position;
                    vertices[0].position.y += 0.1f;
                    vertices[1].position.y += 0.1f;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                }
            }
        }
    }

}

void antinodeEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 - 40, 16, "Antinode Editor");
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, xscale, yscale);
    EDANTINODE_s *selected = reinterpret_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c);
    EDANTINODE_s *display = selected;
    if (display == nullptr)
        display = *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
    if (display != nullptr) {
        nuvec_s displacement;
        NuVecXZDist(&display->position, &aieditor->camera_position, &displacement);
    }
    EDANTINODE_s *nearest = *reinterpret_cast<EDANTINODE_s **>(reinterpret_cast<u8 *>(aieditor) + 0x42ea0);
    if (nearest != nullptr && nearest == aieditor->mode_selection_42e9c) {
        char *platform_name = NuSpecialGetName(&selected->special);
        if (platform_name != nullptr) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "Platform=%s", platform_name);
        } else {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "Not attached to platform");
        }
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 240, 16, "X - Move selected/Adjust size");
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 360, 16, "TRI - Delete selected");
        if (reinterpret_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c)->type == 0) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "LRIGHT - Increase radius, %.2f",
                          selected->radius);
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 600, 16, "LLEFT - Decrease radius");
        } else if (aieditor->pad_buttons & 0x40) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "LRIGHT - Increase X, %.2f",
                          selected->base_radius);
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 600, 16, "LLEFT - Decrease X");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 720, 16, "LUP - Increase Z, %.2f",
                          reinterpret_cast<EDANTINODE_s *>(aieditor->mode_selection_42e9c)->base_height);
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 840, 16, "LDOWN - Decrease Z");
        } else if (aieditorsettings.solid_antinode_display && (aieditor->pad_buttons & 0x1000)) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "L1 - Increase upper height");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 600, 16, "R1 - Decrease upper height");
        } else if (aieditorsettings.solid_antinode_display && (aieditor->pad_buttons & 0x4000)) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "L1 - Increase lower height");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 600, 16, "R1 - Decrease lower height");
        } else {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "LLEFT - Rotate left");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 600, 16, "LRIGHT - Rotate right");
            if (aieditorsettings.solid_antinode_display) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 720, 16, "LUP - Adjust upper height");
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 840, 16, "LDOWN - Adjust lower height");
            }
        }
    } else {
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 240, 16,
                      nearest != nullptr ? "X - Select antinode" : "X - Create antinode");
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 360, 16, "SELECT - Select nearest");
    }
    antinodeEditorDrawAntinodes();
    areaEditorDrawAreas();
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display)
        creatureEditor_RenderAllCreatures();
    locatorEditorDrawLocators();
}

void routeEditor_Render(i32 x, i32 y, float xscale, float yscale) {
    if (aieditor->current_path != NULL) {
        NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 - 40, 16, "Edit Routes (Path = \"%s\")",
                      aieditor->current_path->name);
        NuQFntSetColour(system_qfont, 0x80000000);
        NuQFntSetScale(system_qfont, xscale, yscale);
        if (aieditor->current_path->current_route != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "\"%s\"",
                          aieditor->current_path->current_route->name);
        } else {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 120, 16, "NO ROUTES AVAILABLE");
        }
        if (aieditor->current_path->current_node != NULL) {
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 360, 16, "SELECT - Select nearest");
            if (aieditor->current_path->nearest_node != NULL &&
                aieditor->current_path->nearest_node != aieditor->current_path->current_node) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 480, 16, "X - Select");
            }
            if (!(aieditor->flags & 1) && aieditor->current_path->nearest_node != NULL) {
                NuQFntPrintEx(system_qfont, (x + 10) * 16, y * 8 + 720, 16, "O - Add/remove cnx to route.");
            }
        }
    }
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

void pathEditor_Render(i32 x, i32 y, float x_scale, float y_scale) {
    EDAIPATH_s *path = aieditor->current_path;
    if (path != nullptr) {
        i32 screen_x = (x + 10) * 16;
        i32 screen_y = y * 8;
        if (aieditorsettings.unknown_060_bit0) {
            NuQFntPrintEx(system_qfont, screen_x, screen_y - 40, 16, "Show Routes : \"%s\"", path->name);
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            path = aieditor->current_path;
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "\"%s\"", path->name);
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 360, 16, "SELECT - Goto nearest");
            path = aieditor->current_path;
            if (path->runtime_nearest >= 0) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "TRI - Set as start of route");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 600, 16, "X - Set as end of route");
            }
        } else {
            NuQFntPrintEx(system_qfont, screen_x, screen_y - 40, 16, "AI Path Editor: \"%s\"", path->name);
            NuQFntSetColour(system_qfont, 0x80000000);
            NuQFntSetScale(system_qfont, x_scale, y_scale);
            path = aieditor->current_path;
            if (path->current_node != nullptr) {
                if (path->current_node->name[0] == '\0') {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "ix=%d, %d nodes",
                                  path->current_node->index, path->node_count);
                } else {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "\"%s\" %d nodes",
                                  path->current_node->name, path->node_count);
                }
            } else {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 120, 16, "%d nodes", path->node_count);
            }
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 240, 16, "SQR - Sub menu");
            NuQFntPrintEx(system_qfont, screen_x, screen_y + 360, 16, "SELECT - Select nearest");
            path = aieditor->current_path;
            if (aieditor->flags & 1) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Move selected");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 600, 16, "TRI - Delete selected");
                path = aieditor->current_path;
                if (path != nullptr && path->current_node != nullptr) {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 840, 16, "LRIGHT - Increase radius, %.2f",
                                  path->current_node->radius);
                } else {
                    NuQFntPrintEx(system_qfont, screen_x, screen_y + 840, 16, "LRIGHT - Increase radius");
                }
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 960, 16, "LLEFT - Decrease radius");
            } else if (path->nearest_node != nullptr) {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Select");
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 720, 16, "O - Link/unlink to selected");
                if (__builtin_expect(EdGetCnxFlagNames != nullptr, 0)) {
                    path = aieditor->current_path;
                    if (path->nearest_node != nullptr && path->current_node != nullptr) {
                        EDAIPATHNODE_s *nearest = path->nearest_node;
                        EDAIPATHNODE_s *current = path->current_node;
                        i32 index = -1;
                        if (current->connections[0].node == nearest)
                            index = 0;
                        else if (current->connections[1].node == nearest)
                            index = 1;
                        else if (current->connections[2].node == nearest)
                            index = 2;
                        else if (current->connections[3].node == nearest)
                            index = 3;
                        else if (current->connections[4].node == nearest)
                            index = 4;
                        else if (__builtin_expect(current->connections[5].node == nearest, 1))
                            index = 5;
                        else if (__builtin_expect(current->connections[6].node == nearest, 1))
                            index = 6;
                        else if (current->connections[7].node == nearest)
                            index = 7;
                        if (index >= 0) {
                            char *names = EdGetCnxFlagNames(current->connections[index].flags);
                            if (names != nullptr && NuStrLen(names) != 0) {
                                NuQFntSetColour(system_qfont, 0x80808080);
                                NuQFntPrintEx(system_qfont, 5120, 160, 64, names);
                                NuQFntSetColour(system_qfont, 0x80808080);
                            }
                        }
                    }
                }
            } else {
                NuQFntPrintEx(system_qfont, screen_x, screen_y + 480, 16, "X - Create");
            }
        }
    }
    if (aieditorsettings.unknown_060_bit0 && aieditor->cached_path_system != nullptr &&
        aieditor->current_path != nullptr) {
        AIPATH_s *runtime_path = aieditor->cached_path_system->paths[0];
        if (runtime_path != nullptr) {
            i32 current = aieditor->current_path->runtime_start;
            i32 end = aieditor->current_path->runtime_end;
            if (__builtin_expect(current >= 0 && current < runtime_path->node_count && end >= 0 && end < runtime_path->node_count &&
                current != end, 1)) {
                AIPATHNODE_s *node = &runtime_path->nodes[current];
                u8 edge = runtime_path->route_matrix[current][end];
                while (edge != 0xff) {
                    AIPATHCNX_s *connection = node->connections[edge];
                    i32 next = connection->node_indices[0] != current ? connection->node_indices[0]
                                                                      : connection->node_indices[1];
                    AIPATHNODE_s *next_node = &runtime_path->nodes[next];
                    NURND_VERTEX3D vertices[2];
                    vertices[0].position = node->position;
                    vertices[1].position = next_node->position;
                    vertices[0].position.y += 0.1f;
                    vertices[1].position.y += 0.1f;
                    vertices[0].colour = 0xff0000ff;
                    vertices[1].colour = 0xff0000ff;
                    AiRndrLine3d(vertices, nullptr, nullptr);
                    current = next;
                    end = aieditor->current_path->runtime_end;
                    if (current == end)
                        break;
                    edge = runtime_path->route_matrix[current][end];
                    node = &runtime_path->nodes[current];
                }
            }
        }
    }
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    antinodeEditorDrawAntinodes();
}

void locatorEditor_Render(i32 x, i32 y, float x_scale, float y_scale) {
    i32 text_x = (x + 10) * 16;
    if (aieditor->current_locator_set != nullptr) {
        NuQFntPrintEx(system_qfont, text_x, y * 8 - 40, 16, "Locator Editor: Set=\"%s\"",
                      aieditor->current_locator_set->name);
    } else {
        NuQFntPrintEx(system_qfont, text_x, y * 8 - 40, 16, "Locator Editor: Set=\"NONE\"");
    }
    NuQFntSetColour(system_qfont, 0x80000000);
    NuQFntSetScale(system_qfont, x_scale, y_scale);
    EDLOCATOR_s *display_locator =
        aieditor->current_locator != nullptr ? aieditor->current_locator : aieditor->nearest_locator;
    if (display_locator != nullptr) {
        NUVEC delta;
        f32 distance = NuVecXZDist(&display_locator->position, &aieditor->camera_position, &delta);
        NuQFntPrintEx(system_qfont, text_x, (y + 15) * 8, 16, "\"%s\", xzrng=%.2f", display_locator->name,
                      static_cast<f64>(distance));
    }
    NuQFntPrintEx(system_qfont, text_x, (y + 30) * 8, 16, "SQR - Options");
    if (aieditor->nearest_locator != nullptr) {
        if (aieditor->nearest_locator != aieditor->current_locator) {
            NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "X - Select locator");
            NuQFntPrintEx(system_qfont, text_x, (y + 60) * 8, 16, "SELECT - Select nearest");
        } else {
            NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "X - Move selected");
            NuQFntPrintEx(system_qfont, text_x, (y + 60) * 8, 16, "TRI - Delete selected");
            NuQFntPrintEx(system_qfont, text_x, (y + 75) * 8, 16, "LLEFT - Rotate left");
            NuQFntPrintEx(system_qfont, text_x, (y + 90) * 8, 16, "LRIGHT - Rotate right");
        }
    } else {
        NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "X - Create locator");
    }
    if (aieditor->nearest_locator == nullptr &&
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(aieditor) + 0x48) != 0 &&
        aieditorsettings.elapsed_time < 0.5f) {
        DrawLocator(&aieditor->camera_position, aiEditor_LocatorWidth, aieditorsettings.area_rotation, 0);
    }
    areaEditorDrawAreas();
    locatorEditorDrawLocators();
    pathEditorDrawPaths();
    antinodeEditorDrawAntinodes();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    EDLOCATORSET_s *set = aieditor->current_locator_set;
    if (set != nullptr && set->locators[0] != nullptr) {
        NUVEC previous = set->locators[0]->position;
        for (i32 index = 1; index <= 64 && set->locators[index - 1] != nullptr; ++index) {
            NUVEC current = set->locators[index - 1]->position;
            if (index != 1) {
                NURND_VERTEX3D vertices[2] __attribute__((aligned(16))) = {};
                vertices[0].colour = 0x32323232;
                vertices[1].colour = 0x32323232;
                vertices[0].position = current;
                vertices[1].position = previous;
                AiRndrLine3d(vertices, nullptr, nullptr);
                NUVEC direction;
                NuVecSub(&direction, &previous, &current);
                NuVecNorm(&direction, &direction);
                NuVecScale(&direction, &direction, 0.05f);
                NUVEC midpoint;
                NuVecAdd(&midpoint, &current, &previous);
                NuVecScale(&midpoint, &midpoint, 0.5f);
                NUVEC tip = {midpoint.x - direction.x * 0.5f, midpoint.y - direction.y * 0.5f,
                             midpoint.z - direction.z * 0.5f};
                vertices[0].position = tip;
                NuVecRotateY(&vertices[1].position, &direction, 0xe39);
                NuVecAdd(&vertices[1].position, &vertices[1].position, &tip);
                AiRndrLine3d(vertices, nullptr, nullptr);
                NuVecRotateY(&vertices[1].position, &direction, -0xe39);
                NuVecAdd(&vertices[1].position, &vertices[1].position, &tip);
                AiRndrLine3d(vertices, nullptr, nullptr);
            }
            previous = current;
        }
    }
}

void areaEditor_Render(i32 x, i32 y, f32, f32) {
    i32 text_x = (x + 10) * 16;
    NuQFntPrintEx(system_qfont, text_x, y * 8 - 40, 16, "Area Editor");
    NuQFntSetColour(system_qfont, 0x80000000);
    EDAIAREA_s *focus = area_selected() != NULL ? area_selected() : area_hovered();
    if (focus != NULL) {
        NUVEC delta;
        f32 range = NuVecXZDist(&focus->position, &aieditor->camera_position, &delta);
        y += 15;
        NuQFntPrintEx(system_qfont, text_x, y * 8, 16, "\"%s\", xzrng=%.2f", focus->name, static_cast<f64>(range));
    }
    NuQFntPrintEx(system_qfont, text_x, (y + 15) * 8, 16, "SQR - Options");
    if (area_hovered() != NULL) {
        if (area_hovered() != area_selected()) {
            NuQFntPrintEx(system_qfont, text_x, (y + 30) * 8, 16, "X - Select area");
            NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "SELECT - Select nearest");
        } else {
            NuQFntPrintEx(system_qfont, text_x, (y + 30) * 8, 16, "X - Move selected/Adjust size");
            if ((aieditor->pad_buttons & 0x40) != 0) {
                if (area_hovered()->flags & 1) {
                    NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "LRIGHT/LLEFT - Adjust Radius");
                } else {
                    NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "LRIGHT/LLEFT - Adjust X size");
                    NuQFntPrintEx(system_qfont, text_x, (y + 60) * 8, 16, "LUP/LDOWN - Adjust Z size");
                }
            } else {
                i32 next_line = y + 45;
                NuQFntPrintEx(system_qfont, text_x, next_line * 8, 16, "TRI - Delete selected");
                next_line += 15;
                if ((area_hovered()->flags & 1) == 0) {
                    NuQFntPrintEx(system_qfont, text_x, next_line * 8, 16, "LLEFT/LRIGHT - Rotate");
                    next_line += 15;
                }
                NuQFntPrintEx(system_qfont, text_x, next_line * 8, 16, "LUP - Increase height");
                NuQFntPrintEx(system_qfont, text_x, (next_line + 15) * 8, 16, "LDOWN - Decrease height");
            }
        }
    } else {
        NuQFntPrintEx(system_qfont, text_x, (y + 30) * 8, 16, "X - Create area");
        NuQFntPrintEx(system_qfont, text_x, (y + 45) * 8, 16, "SELECT - Select nearest");
    }
    areaEditorDrawAreas();
    pathEditorDrawPaths();
    if (aieditorsettings.show_creatures_display) {
        creatureEditor_RenderAllCreatures();
    }
    antinodeEditorDrawAntinodes();
    locatorEditorDrawLocators();
}

extern "C" {
    void AITerrInit(void) {
    }

    f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask) {
        f32 height = 0.0f;
        if (geom == 0)
            height = NewShadow(position, height_above, height_below, terrain_mask);
        return height;
    }

    i32 AITerrShadowOnPlatform(void) {
        i32 platform = -1;
        if (geom == 0)
            platform = NewShadowOnPlatform();
        return platform;
    }

    i32 aieditor_Proc(f32 menu_delta_time, nupad_s *pad, f32 frame_delta_time, u32 exit_buttons) {
        aieditor->pad_buttons = pad->digital_buttons;
        aieditor->pad_pressed = pad->digital_buttons_pressed;
        if (aieditor->main_menu != nullptr) {
            eduiMenuProcess(aieditor->main_menu, menu_delta_time, pad);
            return false;
        }
        if (NuStrLen(aieditor->warning) != 0) {
            aieditor->main_menu = aieditor_AddMessage((char *)"Warning", aieditor->warning);
            NuStrCpy(aieditor->warning, "");
            if (aieditor->main_menu != nullptr) {
                return false;
            }
        }

        aieditorsettings.elapsed_time = NuFmod(frame_delta_time + aieditorsettings.elapsed_time, 1.0f);
        if ((pad->digital_buttons & 0x100) == 0 && (aieditor->flags & 4) == 0) {
            edcamMoveEx(pad, frame_delta_time);
        }

        NUVEC movement;
        NUVEC previous_position = aieditor->cursor_position;
        edcamGetPosAng(&aieditor->cursor_position, &aieditor->camera_pitch, &aieditor->camera_yaw);
        NuVecSub(&aieditor->cursor_movement, &aieditor->cursor_position, &previous_position);
        aieditor->flags &= ~u8(4);
        aieditor->cursor_platform.scene = nullptr;
        aieditor->cursor_platform.special = nullptr;
        aieditor->cursor_platform.display_special = nullptr;
        aieditor->camera_position = aieditor->cursor_position;
        f32 terrain_height = AITerrShadow(&aieditor->cursor_position, 0.0f, 5.0f, -1);
        i32 platform = AITerrShadowOnPlatform();
        NuSpecialFindByPlatformID(static_cast<nugscn_s *>(aieditor->enter_context), &aieditor->cursor_platform,
                                  platform);

        if (aieditorsettings.snap_height_display) {
            const f32 absent_height = 2000000.0f;
            f32 ground_shadow = EShadY;
            f32 roof_shadow = ShadRoofY;
            f32 closest = absent_height;
            if (terrain_height != absent_height) {
                aieditor->camera_position.y = terrain_height;
                closest = __builtin_fabsf(aieditor->cursor_position.y - terrain_height);
            }
            if (ground_shadow != absent_height) {
                f32 distance = __builtin_fabsf(aieditor->cursor_position.y - ground_shadow);
                if (distance < closest) {
                    aieditor->camera_position.y = ground_shadow;
                    closest = distance;
                }
            }
            if (roof_shadow != absent_height && __builtin_fabsf(aieditor->cursor_position.y - roof_shadow) < closest) {
                aieditor->camera_position.y = roof_shadow;
            }
            if (disable_cylinder_check == 0 && aieditor->current_path != nullptr &&
                aieditor->current_path->current_node != nullptr &&
                aieditor->current_path->current_node == aieditor->current_path->nearest_node &&
                aieditor->camera_position.y < aieditor->cursor_position.y &&
                terrain_height == aieditor->camera_position.y) {
                NuVecSub(&movement, &aieditor->camera_position, &aieditor->cursor_position);
                union {
                    u32 bits;
                    f32 value;
                } ray_scale = {0x233877aa};
                i32 hit = NewRayCastScaleY(&aieditor->cursor_position, &movement,
                                           aieditor->current_path->current_node->radius, ray_scale.value, 0);
                if (hit != 0 && (hit & 0x10) == 0) {
                    NuVecAdd(&aieditor->camera_position, &aieditor->cursor_position, &movement);
                }
            }
            aieditor->camera_position.y += aieditorsettings.path_height_offset;
            if (aieditorsettings.unknown_060_bit5) {
                aieditor->cursor_position = aieditor->camera_position;
            }
        }
        auto process = reinterpret_cast<eduimenu_s *(*)(nupad_s *)>(
            aieditorsettings.modes[static_cast<i16>(aieditorsettings.current_mode)].callback_24);
        if (process != nullptr) {
            aieditor->main_menu = process(pad);
        }
        if (aieditor->main_menu != nullptr)
            return false;
        return (pad->digital_buttons_pressed & exit_buttons) != 0;
    }

}
