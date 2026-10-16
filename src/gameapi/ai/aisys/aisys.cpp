#include "decomp.h"

#include "legoapi/actions/combat/hits.h"

#include "legoapi/actions/character/speederchase.h"

#include "batman.h"

#include "gameapi/ai/aisys/aisys.h"

#include "globals.h"

#include "legoapi/ai/core/ai_sys_stubs.h"

#include "legoapi/ai/core/legoai.h"

#include <stdio.h>

#include <float.h>

#include <string.h>

#include <math.h>

#include "globals.h"

#include "legoapi/characters/core/character.h"

#include "legoapi/characters/core/charconfig.h"

#include "legoapi/characters/core/players.h"

#include "legoapi/characters/motion.h"

#include "legoapi/characters/motion/gameanim.h"

#include "legoapi/audio/sfx.h"

#include "legoapi/items/objects/gameobjects.h"

#include "legoapi/menus/core/panel.h"

#include "legoapi/core/input/gamepads.h"

#include "legoapi/core/input/qrand.h"

#include "legoapi/gizmo/base/gizmo.h"

#include "legoapi/gizmo/base/gizactions.h"

#include "legoapi/gizmos/traps/gizturrets.h"

#include "legoapi/gizmos/object/gizobstacles.h"

#include "legoapi/gizmos/object/gizbuildits.h"

#include "legoapi/gizmo/object/takeoverobjects.h"

#include "legoapi/gizmo/object/gizmoblowups.h"

#include "legoapi/gizmos/fx/gizmopickups.h"

#include "legoapi/gizmo/object/gizmopickup.h"

#include "legoapi/gizmos/object/newblowup.h"

#include "legoapi/gizmos/traps/gizforce.h"

#include "legoapi/gizmos/trigger/gizspecial.h"

#include "legoapi/gizmos/transport/grapples.h"

#include "legoapi/gizmos/door/zipups.h"

#include "legoapi/items/objects/gameobjects.h"

#include "legoapi/props/system/socksys.h"

#include "legoapi/ai/core/ai_sys_stubs.h"

#include "legoapi/gizmos/traps/gizforce.h"

#include "legoapi/gizmos/object/newblowup.h"

#include "legoapi/legoapi_types.h"

#include "legoapi/render/fx.h"

#include "legoapi/world/level.h"

#include "legoapi/world/area.h"

#include "legoapi/world/world.h"

#include "legoapi/world/world_shared.h"

#include "nu2api/nucore/nustring.h"

#include "nu2api/nu3d/nuspline.h"

#include "nu2api/nu3d/nuspecial.h"

#include "nu2api/nu3d/nulgtlaser.h"

#include "nu2api/numath/nuang.h"

#include "nu2api/numath/nufloat.h"

#include "nu2api/numath/nutrig.h"

#include "nu2api/numath/nurand.h"

#include "nu2api/nu3d/nuspecial.h"

#include "nu2api/numath/numtx.h"

extern i32 Hub_GetRandomCharType();

extern void *perm_debrissys;

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);

void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx);

void ResetForceBack();

void AlertSurroundingCreatures(GameObject_s *object, NUVEC *position);

void LetGoOfBalloon(GameObject_s *object);

void GizmoPickup_TurnOnPickup(GIZMOPICKUP_s *pickup);

void SetForceBack(GameObject_s *object, NUVEC *position, f32 radius, i32 type);

void AddGameMsgCount(NUVEC *position, i32 count, i32 total, u8 red, u8 green, u8 blue, f32 duration);

extern void CurrentStart(GameObject_s *object, i32 mode, i32 start);

extern "C" void ComplexSockAngles(SOCKROT *angles);

extern void oneAtOnce_SetInitDistPerRow(f32 distance);

extern i32 oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);

extern f32 oneAtOnce_GetHoldRange(GameObject_s *object);

extern void Hint_CancelCurrent();

extern i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);

extern void SetPlayer();

extern void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position);

extern void ResetAICreature(GameObject_s *object, AISYS_s *system);

extern void DeactivateGameObject(GameObject_s *object);

extern void Player_ClearContext(GameObject_s *object, i32 mode);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);

extern EXPLOSION *Detonate(NUVEC *position, u16 flags);

extern void EatVictim(GameObject_s *object);

extern void GrabVictim(GameObject_s *object, GameObject_s *victim);

extern void ReleaseEat(GameObject_s *object);

extern void ReleaseTakeOver(GameObject_s *object, i32 immediate);

extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *vehicle, i32 blend_camera, i32 immediate);

extern void FindForcePushTarget(GameObject_s *object, i32 held, i32 mode);

extern void InitSplinePosition(SPLINEPOS_s *position, nugspline_s *spline, f32 distance, i32 looping);

extern void KillParts(GameObject_s *object, i32 part, i32 joint, i32 visible, f32 velocity, i32 flags, u16 *part_ids);

extern u32 StarWars_ParseAIPathCnxFlag(char *name);

extern AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                        AIPATH_s *path, char *from, char *to, i32 target_type,
                                                        char *target_name, i32 fake_animation_id, i32 gizmo_output);

extern void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);

extern void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked);

extern AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection,
                                                       u8 direction, void *target, u8 type);

extern "C" void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from, char *to, i32 *direction);

extern ADDPART_s Default_ADDPART;

extern f32 ForceThrowSpeed, ForceThrowGravity;

extern "C" PART_s *AddPart(ADDPART_s *);

void PartCollide_3D(PART_s *);

void MakeThrowVector(NUVEC *, NUVEC *, NUVEC *, NUVEC *, f32, f32);

static i32 Action_SetCurrentSpeed(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UsePanel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_PullLever(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTechno(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_HelpWithTriggers(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_UseTriggerSet(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_GoToOriginalPath(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_BigJumpToLocator(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_UseBigJumpToJump(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_CatchUpForbidden(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_JudderGameCamera(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_ReleaseTakeOver(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetHintComplete(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetAnimSpeedMul(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_ShootAtOpponent(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetInvulnerable(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

i32 Action_SetVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_PressJumpButton(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_SetControlSystem(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_FollowDirection(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

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

// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.

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

void LevelScriptReStoreProgress(WORLDINFO_s *, LEVELSCRIPTPROCESS_s *);

#include "legoapi/props/objects/techno.h"

extern void oneAtOnce_SetDistPerRow(f32);

extern void oneAtOnce_SetNumAttackers(i32);

extern void oneAtOnce_SetAttackersPerRow(i32);

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

static i32 Action_EngageOpponent(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

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

__used__ static i32 Action_ReturnToState(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **, i32, i32, f32) {
    if (processor != NULL && processor->return_to_state != NULL) {
        processor->next_state = processor->return_to_state;
        processor->return_to_state = NULL;
    }
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

static i32 PartyMemberInWay(GameObject_s *, GameObject_s *);

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

AILOCATOR *LocalGetRandomLocator(AILOCATOR **, i32, f32, NUVEC *, f32, i32, f32, f32);

AILOCATOR *LocalGetNearestLocator(AILOCATOR **, i32, f32, NUVEC *, f32, i32, f32, f32);

i16 GetGenericGoon(i32);

extern "C" AIGROUP *CreateAIGroup(AISYS *, i32, f32, f32, f32);

void PlayJumpSfx(GameObject_s *, i32);

void SetWeaponOut(GameObject_s *);

void StartBallooning(GameObject_s *, i32);

void SetBallooningHeight(GameObject_s *, f32);

void SpawnCreatureFromCrate(GameObject_s *, f32, f32);

void PartKill_ForceThrow(PART_s *, i32);

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

extern i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

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

__used__ static f32 Condition_Param(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *) {
    return AIParamToFloatEx(packet, processor, arg);
}

static f32 Condition_Timer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    return processor->script_timer;
}

static f32 Condition_OnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->path_info.on_path ? 1.0f : 0.0f;
}

static f32 Condition_Random(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return NuRandFloat();
}

extern TERRSET *CurTerr;

static f32 Condition_StuckTime(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->owner != NULL ? packet->owner->apiobj.respawn_timer : 0.0f;
}

static f32 Condition_GotLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return processor->unknown_a4 != NULL ? 1.0f : 0.0f;
}

extern "C" i32 party_under_cover;

static f32 Condition_GotOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->opponent != NULL ? 1.0f : 0.0f;
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

__used__ static f32 Condition_InterruptID(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    if (processor != NULL) {
        return static_cast<u32>(processor->interrupt_id);
    }
    return -1.0f;
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

static f32 Condition_PathBlocked(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    // The script condition tests the waypoint bit (0x40), not the route-failure bit (0x20).
    return packet != NULL && (packet->runtime_flags & AIPACKET_RUNTIME_USING_PATH_WAYPOINT) != 0 ? 1.0f : 0.0f;
}

static f32 Condition_PlayerRange(AISYS *sys, AISCRIPTPROCESS *, AIPACKET *packet, char *, void *) {
    if (packet == NULL || packet->owner == NULL || sys == NULL || sys->player_1 == NULL) {
        return FLT_MAX;
    }
    NUVEC delta;
    return NuVecDist(&sys->player_1->position, &packet->owner->apiobj.position, &delta);
}

static f32 Condition_TimeOffPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL ? packet->time_off_path : 0.0f;
}

extern FLOWBOX_s *FlowBoxFindByName(GIZFLOW_s *, char *);

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

static f32 Condition_PlayerOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return sys->player_1 != NULL && sys->player_1->ai->path_info.on_path ? 1.0f : 0.0f;
}

static void *Condition_IAmInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL && GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(sys, arg) : NULL;
}

// API script callbacks and their constant registry share internal linkage.
static f32 Condition_AlwaysTrue(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *void_arg) {
    return *(f32 *)&void_arg;
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

static void *Condition_LocatorRangeInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
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

static f32 Condition_GotLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return processor->unknown_a8 != NULL ? 1.0f : 0.0f;
}

static f32 Condition_CurrentLocatorIs(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return void_arg != NULL && processor->unknown_a4 == void_arg ? 1.0f : 0.0f;
}

static void *Condition_CurrentLocatorIsInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
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

static void *Condition_InTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
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

static void *Condition_InLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
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

static void *Condition_PlayerInLevelNodeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
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

static f32 Condition_LevelNodeRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *void_arg) {
    NUVEC difference;
    if (packet != NULL && packet->owner != NULL && void_arg != NULL) {
        AIPATHNODE *node = static_cast<AIPATHNODE *>(void_arg);
        return NuVecDist(&node->position, &packet->owner->apiobj.position, &difference);
    }
    return 3.402823466e+38f;
}

static void *Condition_LevelNodeRangeInit(AISYS *sys, char *arg, AISCRIPT *) {
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->path_count != 0) {
        return AIPathFindNode(sys, sys->path_sys->active_path, arg);
    }
    return NULL;
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

static void *Condition_PlayerInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
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

static void *Condition_BaddyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static f32 Condition_GoodyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *void_arg) {
    AIAREA *area = static_cast<AIAREA *>(void_arg);
    if (area == NULL) {
        area = processor->unknown_a0;
    }
    return area != NULL && (area->runtime_flags & AIAREA_RUNTIME_OBJECT_STATE_CLEAR) != 0 ? 1.0f : 0.0f;
}

static void *Condition_GoodyInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
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

static void *Condition_OpponentInTriggerAreaInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AISysFindArea(sys, arg) : NULL;
}

static f32 Condition_OpponentIsAThreat(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    if (packet != NULL && packet->opponent_object != NULL && packet->opponent_is_threat) {
        return 1.0f;
    }
    return 0.0f;
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

static f32 Condition_OpponentBelow(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *void_arg) {
    return packet != NULL && packet->opponent_object != NULL &&
                   packet->opponent_object->position.y < packet->owner->apiobj.position.y - 0.1f
               ? 1.0f
               : 0.0f;
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

static void *Condition_OpponentToLocatorInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
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

static void *Condition_PlayerToLocatorInit(AISYS *sys, char *arg, AISCRIPT *) {
    return arg != NULL ? AIPathFindLocator(sys, arg) : NULL;
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

static f32 Condition_OpponentOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *, void *) {
    return packet != NULL && packet->opponent != NULL &&
                   static_cast<APIOBJECT *>(packet->opponent)->ai->path_info.on_path
               ? 1.0f
               : 0.0f;
}

i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

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

i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

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

extern "C" void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *end, u32 size);

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

AIANTINODE dynamic_antinodes[64] = {};

extern "C" f32 default_path_heighttol;

extern "C" void AISetPathHeightTol(f32 tolerance) {
    default_path_heighttol = tolerance;
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
