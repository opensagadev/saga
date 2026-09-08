#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/characters/core/charconfig.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nuspline.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nuvec.h"

extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);
extern i32 Hub_GetRandomCharType();
extern u8 hub_custodians_finished_loading;
extern void GameObjectOrigin(GameObject_s *object);
extern void PodSprint_GetIAlongVals(nugspline_s *spline, i16 *first_point, i16 *last_point);
extern void TakeOverGameObject(GameObject_s *rider, GameObject_s *mount, i32 seat, i32 immediate);
extern NUVEC plr_lastpos;

enum AI_CREATURE_SET : isize {
    AI_CREATURE_SET_CURRENT = -1,
    AI_CREATURE_SET_NONE = 0,
    AI_CREATURE_SET_FIRST = 1,
    AI_CREATURE_SET_LAST = 16,
};

union AI_CONDITION_SET_ARGUMENT {
    void *pointer;
    isize value;
};

static isize AIConditionArgumentValue(void *argument) {
    AI_CONDITION_SET_ARGUMENT condition_argument = {};
    condition_argument.pointer = argument;
    return condition_argument.value;
}

static GameObject_s *ActionOwner(AIPACKET_s *packet) {
    return packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
}

static bool ActionToggleEnabled(char **params, i32 param_count) {
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            return false;
        }
    }
    return true;
}

static GIZOBSTACLE *ActionFindObstacle(char *name) {
    if (WORLD == NULL || WORLD->gizmo_sys == NULL) {
        return NULL;
    }
    GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, name);
    return gizmo != NULL ? static_cast<GIZOBSTACLE *>(gizmo->object) : NULL;
}

static __used__ f32 Condition_CurrentHintId(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EmptyTakeOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_ForceComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument) {
    if (argument != NULL && GizForce_Complete((GIZFORCE_s *)argument) != 0)
        return 1.0f;
    return 0.0f;
}

static f32 Condition_ForceFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument) {
    GIZFORCE_s *force = static_cast<GIZFORCE_s *>(argument);
    return force != NULL && GizForce_AnimComplete(force) != 0 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_GotLocatorSet(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *, void *) {
    return processor != NULL && processor->locator_set != NULL ? 1.0f : 0.0f;
}

static __used__ f32 Condition_HintAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_HitPointsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_InContextInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_InTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_IsVisibleInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_LastLevelInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_LocatorRangeY(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                            void *void_arg) {
    if (packet == NULL || packet->owner == NULL) {
        return 0.0f;
    }
    AILOCATOR_s *locator = static_cast<AILOCATOR_s *>(void_arg);
    if (locator == NULL && processor != NULL) {
        locator = processor->locator;
    }
    return locator != NULL ? packet->owner->apiobj.position.y - locator->position.y : 0.0f;
}

static f32 Condition_NumInSetAlive(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *void_arg) {
    isize creature_set = AIConditionArgumentValue(void_arg);
    if (creature_set == AI_CREATURE_SET_CURRENT) {
        creature_set = packet->creature_set;
    }
    if (creature_set == AI_CREATURE_SET_NONE) {
        return 0.0f;
    }
    return static_cast<f32>(aicreature_sets_alive[creature_set - AI_CREATURE_SET_FIRST]);
}

static __used__ f32 Condition_ObstacleAtEnd(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OnSpeederBike(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentBelow(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    GameObject_s *owner = ActionOwner(packet);
    GameObject_s *opponent = packet != NULL ? static_cast<GameObject_s *>(packet->opponent) : NULL;
    return owner != NULL && opponent != NULL && opponent->apiobj.position.y < owner->apiobj.position.y - 0.1f ? 1.0f
                                                                                                              : 0.0f;
}

static __used__ f32 Condition_OpponentRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    return packet != NULL && packet->opponent != NULL ? packet->opponent_metric : 1.0e9f;
}

static __used__ f32 Condition_Player2Active(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_TakeOverRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_TakenOverInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_YawToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    GameObject_s *owner = ActionOwner(packet);
    GameObject_s *opponent = packet != NULL ? static_cast<GameObject_s *>(packet->opponent) : NULL;
    if (owner == NULL || opponent == NULL) {
        return 1.0e9f;
    }
    NUVEC relative = {
        opponent->apiobj.position.x - owner->apiobj.position.x,
        opponent->apiobj.position.y - owner->apiobj.position.y,
        opponent->apiobj.position.z - owner->apiobj.position.z,
    };
    NuVecRotateY(&relative, &relative, -owner->apiobj.field_0x276);
    return static_cast<f32>(NuAtan2D(relative.x, relative.z)) * (360.0f / 65536.0f);
}

static __used__ i32 Action_CanShootOffScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->field_0x1050 = (object->field_0x1050 & ~4u) | (ActionToggleEnabled(params, param_count) ? 4u : 0u);
    }
    return 1;
}

static __used__ i32 Action_DrawBossHitPoints(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_IgnoreShoveSystem(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_IgnoreWallSplines(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    if (packet != NULL && first_time != 0) {
        packet->movement_flags |= 0x80;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                packet->movement_flags &= static_cast<u8>(~0x80u);
            }
        }
    }
    return 1;
}

static i32 Action_FollowCharacter(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                  i32 param_count, i32 first_time, f32) {
    if (packet == NULL || processor == NULL) {
        return 1;
    }
    if (first_time != 0) {
        processor->action_data_1 = 0;
        processor->action_data_3 = NULL;
        for (i32 index = 0; index < param_count; ++index) {
            char *param = params[index];
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(param, &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(param, "character=");
            if (value != NULL) {
                processor->action_data_3 = GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(system, value + 10) : NULL;
            } else if (NuStrICmp(param, "ignore_radius") == 0) {
                processor->action_data_1 |= 2;
            } else if (NuStrICmp(param, "can_go_off_path") == 0) {
                processor->action_data_1 |= 1;
            } else if (NuStrICmp(param, "Opponent") == 0) {
                GameObject_s *opponent = static_cast<GameObject_s *>(packet->opponent);
                processor->action_data_3 = opponent != NULL ? &opponent->apiobj : NULL;
            } else if (NuStrICmp(param, "TakeOverTarget") == 0) {
                GameObject_s *owner = ActionOwner(packet);
                processor->action_data_3 =
                    owner != NULL && owner->takeover_target != NULL ? &owner->takeover_target->apiobj : NULL;
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, param);
            }
        }
    }
    APIOBJECT *target = static_cast<APIOBJECT *>(processor->action_data_3);
    GameObject_s *owner = ActionOwner(packet);
    if (target != NULL && owner != NULL) {
        FollowAPIObject(&owner->apiobj, target, processor->action_data_1, packet->movement_instruction_parameter);
    }
    return 0;
}

static i32 Action_MoveForward(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                              i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (Player[0] == NULL || packet == NULL || processor == NULL || object == NULL) {
        return 0;
    }
    if (first_time != 0) {
        processor->action_data_4 = static_cast<f32>(object->apiobj.field_0x276);
        i32 minimum_turn = 0;
        i32 maximum_turn = 0;
        i32 turn = 0;
        bool random_direction = false;
        for (i32 index = 0; index < param_count; ++index) {
            char *param = params[index];
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(param, &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = NuStrIStr(param, "min_turn");
            if (value != NULL) {
                minimum_turn = static_cast<i32>(AIParamToFloat(processor, value + 9)) * (65536 / 360.0f);
                continue;
            }
            value = NuStrIStr(param, "max_turn");
            if (value != NULL) {
                maximum_turn = static_cast<i32>(AIParamToFloat(processor, value + 9)) * (65536 / 360.0f);
                continue;
            }
            if (NuStrIStr(param, "rand_turn_dir") != NULL) {
                random_direction = true;
                continue;
            }
            value = NuStrIStr(param, "turn=");
            if (value != NULL) {
                turn = static_cast<i32>(AIParamToFloat(processor, value + 5)) * (65536 / 360.0f);
            }
        }
        if (maximum_turn != 0) {
            turn = static_cast<i32>(static_cast<f32>(maximum_turn - minimum_turn) * NuRandFloat()) + minimum_turn;
        }
        if (turn != 0) {
            if (random_direction && (NuRand(NULL) & 1) != 0) {
                turn = -turn;
            }
            processor->action_data_4 = static_cast<f32>(NuAngAdd(static_cast<NUANG>(processor->action_data_4), turn));
        }
    }
    if ((packet->path_info.flags & AIPATHINFO_FLAG_ON_PATH) != 0) {
        NUVEC destination = {0.0f, 0.0f, 10.0f};
        NuVecRotateY(&destination, &destination, static_cast<NUANG>(processor->action_data_4));
        NuVecAdd(&destination, &destination, &object->apiobj.position);
        AIMoveInstruction(packet, &destination, 0.0f, &packet->path_info, AIPACKET_MOVEMENT_TO_DESTINATION, 0.01f);
    }
    return 0;
}

static __used__ i32 Action_NotifyStateChange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_OverrideAnimation(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || first_time == 0) {
        return 1;
    }

    i16 from = -1;
    i16 to = -1;
    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        if (NuStrICmp(param, "from=All") == 0) {
            from = 0xe9;
            continue;
        }

        char *value = NuStrIStr(param, "from=");
        if (value != NULL) {
            from = static_cast<i16>(FindAnimIX(packet->owner->apiobj.character_data, value + 5));
            continue;
        }

        value = NuStrIStr(param, "to=");
        if (value != NULL) {
            to = static_cast<i16>(FindAnimIX(packet->owner->apiobj.character_data, value + 3));
            continue;
        }

        if (processor != NULL) {
            processor->action_timer = AIParamToFloatEx(packet, processor, params[0]);
        }
    }

    packet->animation_override_from = to != -1 ? from : -1;
    packet->animation_override_to = to;
    return 1;
}

static __used__ i32 Action_PlayerSpeederHack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                             i32 param_count, i32, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->jump_input_flags |= 8;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0)
                object->jump_input_flags &= static_cast<u8>(~8u);
        }
    }
    return 1;
}

static __used__ i32 Action_PressActionButton(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **, i32, i32,
                                             f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && object->pad_gamepad != NULL) {
        object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
        object->field_0xef9 |= 4;
        object->field_0xef8 |= 0x20;
    }
    return 1;
}

static __used__ i32 Action_SetFullPathSearch(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetRespawnLocator(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetShootOpponents(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetTakeOverTarget(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetTechnoComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ void *Condition_CategoryIsInit(AISYS_s *sys, char *name, AISCRIPT_s *) {
    i32 index = -1;
    if (name != NULL && sys != NULL && CharCategory != NULL)
        index = CharCategory_FindByName(name);
    return (void *)(isize)index;
}

static __used__ f32 Condition_CharacterRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_CutSceneExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerIs(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_FinishedSpline(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    GameObject_s *object = ActionOwner(packet);
    if (object == NULL) {
        return -1.0f;
    }
    const u8 finished = object->movement_spline_position.reached_end;
    return object->movement_spline != NULL && finished == 0 ? 0.0f : 1.0f;
}

static __used__ f32 Condition_ForceBeingUsed(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static void *Condition_GizSpecialInit(AISYS_s *, char *arg, AISCRIPT_s *) {
    if (WORLD == NULL) {
        return NULL;
    }
    return GizmoFindByName(WORLD->gizmo_sys, gizspecial_gizmotype_id, arg);
}

static __used__ f32 Condition_GotOpponentLOS(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_GotTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_IsLowEndDevice(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return g_lowEndLevelBehaviour != 0 ? 1.0f : 0.0f;
}

static __used__ void *Condition_IsOnScreenInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static void *Condition_IsSetAliveInit(AISYS_s *, char *arg, AISCRIPT_s *) {
    isize creature_set = AI_CREATURE_SET_NONE;
    if (arg != NULL) {
        if (NuStrICmp(arg, "myset") == 0) {
            creature_set = AI_CREATURE_SET_CURRENT;
        } else {
            const i32 parsed_set = NuAToI(arg);
            if (parsed_set >= AI_CREATURE_SET_FIRST && parsed_set <= AI_CREATURE_SET_LAST) {
                creature_set = parsed_set;
            }
        }
    }
    return reinterpret_cast<void *>(creature_set);
}

static __used__ f32 Condition_LevelNodeRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_LocatorRangeXZ(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char *,
                                             void *void_arg) {
    AILOCATOR_s *locator = static_cast<AILOCATOR_s *>(void_arg);
    if (locator == NULL && processor != NULL) {
        locator = processor->locator;
    }
    if (packet == NULL || packet->owner == NULL || locator == NULL) {
        return 1.0e9f;
    }
    return NuVecXZDist(&packet->terrain_origin, &locator->position, NULL);
}

static __used__ void *Condition_NumBaddiesInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_OffScreenTimer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *,
                                             void *void_arg) {
    GameObject_s *object = static_cast<GameObject_s *>(void_arg);
    if (object == NULL) {
        object = ActionOwner(packet);
    }
    return object != NULL ? object->field_0xf1c : 0.0f;
}

static __used__ f32 Condition_OpponentOnPath(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerOnGround(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerOnObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerToOrigin(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_RigidAnimFrame(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_UsingForceInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ i32 Action_AddScriptProcessor(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_CanHitForceObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                              i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->field_0xef8 = (object->field_0xef8 & ~0x40u) | (ActionToggleEnabled(params, param_count) ? 0x40u : 0u);
    }
    return 1;
}

static __used__ i32 Action_CanTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                              i32 param_count, i32 first_time, f32) {
    if (first_time == 0 || param_count == 0) {
        return 1;
    }
    GIZOBSTACLE *obstacle = NULL;
    bool blocked = false;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            obstacle = ActionFindObstacle(value + NuStrLen("name="));
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            blocked = true;
        }
    }
    if (obstacle != NULL) {
        obstacle->runtime_flags = static_cast<u8>((obstacle->runtime_flags & ~8u) | (blocked ? 8u : 0u));
    }
    return 1;
}

static __used__ i32 Action_CannotBeForcedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_DeflectPlayersPart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                              i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->field_0xefd = (object->field_0xefd & ~1u) | (ActionToggleEnabled(params, param_count) ? 1u : 0u);
    }
    return 1;
}

static __used__ i32 Action_DisableNarrowSocks(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_DontAvoidCharacter(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                              i32 param_count, i32 first_time, f32) {
    if (first_time == 0 || packet == NULL) {
        return 1;
    }

    GameObject_s *object = ActionOwner(packet);
    GameObject_s *dont_avoid = NULL;
    bool enabled = true;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(system, value + 10);
            continue;
        }
        value = NuStrIStr(params[index], "dont_avoid=");
        if (value != NULL) {
            dont_avoid = GetNamedGameObject(system, value + 11);
            continue;
        }
        if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = false;
        }
    }
    if (object != NULL) {
        object->ai.dont_avoid_character = enabled ? dont_avoid : NULL;
    }
    return 1;
}

static __used__ i32 Action_DontSetStoppedFlag(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                              i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    GameObject_s *object = ActionOwner(packet);
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(system, value + 10);
        }
    }
    if (object != NULL) {
        object->field_0xefc = (object->field_0xefc & ~0x20u) | (ActionToggleEnabled(params, param_count) ? 0x20u : 0u);
    }
    return 1;
}

static __used__ i32 Action_GizmoSetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                              i32 param_count, i32 first_time, f32) {
    if (first_time == 0 || WORLD == NULL || WORLD->gizmo_sys == NULL) {
        return 1;
    }
    GIZMO *gizmo = NULL;
    i32 visible = 1;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            gizmo = GizmoFindByName(WORLD->gizmo_sys, -1, value + NuStrLen("name="));
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            visible = 0;
        }
    }
    if (gizmo != NULL) {
        GizmoSetVisibility(WORLD->gizmo_sys, gizmo, visible, 1);
    }
    return 1;
}

static __used__ i32 Action_IgnoreSlideTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_MoveAwayFromPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_PressSpecialButton(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                              char **params, i32 param_count, i32 first_time, f32) {
    if (processor == NULL) {
        return 1;
    }
    if (first_time != 0) {
        processor->action_data_1 = 0;
        for (i32 index = 0; index < param_count; ++index) {
            if (NuStrICmp(params[index], "hold_button") == 0) {
                processor->action_data_1 = 1;
            }
        }
    }

    GameObject_s *object = ActionOwner(packet);
    if (object != NULL && object->pad_gamepad != NULL) {
        object->pad_gamepad->buttons_pressed |= GAMEPAD_SPECIAL;
        if (processor->action_data_1 != 0) {
            object->pad_gamepad->buttons_held |= GAMEPAD_SPECIAL;
        }
    }
    return processor->action_data_1 == 0 ? 1 : 0;
}

static __used__ i32 Action_SelectRandomSpline(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                              i32 num_params, i32 first_time, f32) {
    if (!first_time)
        return 1;
    script_spline_selected = NULL;
    NUGSPLINE *candidates[32];
    NUGSPLINE *unused_candidates[32];
    i32 count = 0;
    i32 unused_only = 0;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "splines=");
        if (value != NULL) {
            count += NuSplineFindAllBeg(WORLD->current_gscn, value + 8, &candidates[count], 32 - count);
        } else if ((value = NuStrIStr(params[i], "spline=")) != NULL) {
            NUGSPLINE *spline = NuSplineFind(WORLD->current_gscn, value + 7);
            if (spline != NULL && count < 32)
                candidates[count++] = spline;
        } else if (NuStrIStr(params[i], "unused") != NULL) {
            unused_only = 1;
        }
    }
    if (count == 0)
        return 1;
    if (unused_only) {
        for (i32 i = 0; i < HIGHGAMEOBJECT; i++) {
            GameObject *object = &Obj[i];
            if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
                object->movement_spline != NULL)
                object->movement_spline->length |= 0x8000;
        }
        i32 unused_count = 0;
        for (i32 i = 0; i < count; i++) {
            if (candidates[i]->length >= 0)
                unused_candidates[unused_count++] = candidates[i];
        }
        for (i32 i = 0; i < HIGHGAMEOBJECT; i++) {
            GameObject *object = &Obj[i];
            if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->apiobj.field_0x287 == 0 &&
                object->movement_spline != NULL)
                object->movement_spline->length &= 0x7fff;
        }
        if (unused_count != 0)
            script_spline_selected = unused_candidates[qrand() / (65535 / unused_count + 1)];
    } else {
        script_spline_selected = candidates[qrand() / (65535 / count + 1)];
    }
    return 1;
}

static __used__ i32 Action_SetAttackersAtOnce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetAttackersPerRow(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetCircleDirection(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetIgnoreAntinodes(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetLastSafePathPos(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                              i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    APIOBJECT_s *object = packet != NULL && packet->owner != NULL ? &packet->owner->apiobj : NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL && GetNamedAPIObjectFn != NULL) {
            object = GetNamedAPIObjectFn(system, value + 10);
        }
    }
    if (object != NULL) {
        object->respawn_position = object->position;
        object->last_safe_position = object->position;
    }
    return 1;
}

static __used__ i32 Action_SetShieldHitPoints(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                              char **params, i32 num_params, i32 first_time, f32) {
    if (!first_time)
        return 1;
    GameObject *object = NULL;
    if (packet != NULL && packet->owner != NULL)
        object = packet->owner->apiobj.objptr;
    i32 hitpoints = -1;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "character=");
        if (value != NULL)
            object = GetNamedGameObject(sys, value + 10);
        else
            hitpoints = (i32)AIParamToFloat(processor, params[i]);
    }
    if (object != NULL) {
        if (hitpoints == -1)
            hitpoints = ((GAMECHARACTERDATA *)object->apiobj.character_data->field11_0x24)->field_0xf5;
        object->field_0xe37 = (u8)hitpoints;
    }
    return 1;
}

static __used__ i32 Action_SnapToSockPosition(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_UseTimeBasedUpdate(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static void *Condition_BeenToLevelInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    if (system == NULL || arg == NULL || WORLD == NULL || WORLD->area == NULL) {
        return reinterpret_cast<void *>(static_cast<isize>(-1));
    }
    for (i32 area_level = 0; area_level < WORLD->area->level_count; ++area_level) {
        const i32 level = WORLD->area->levels[area_level];
        if (NuStrICmp(arg, LDataList[level].name) == 0) {
            return reinterpret_cast<void *>(static_cast<isize>(area_level));
        }
    }
    return reinterpret_cast<void *>(static_cast<isize>(-1));
}

static __used__ f32 Condition_BigJumpComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_BuildItComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument) {
    GIZMO *gizmo = (GIZMO *)argument;
    if (gizmo != NULL && gizmo->object != NULL && ((GIZBUILDIT_s *)gizmo->object)->build_state == 2)
        return 1.0f;
    return 0.0f;
}

static __used__ f32 Condition_CharacterExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static f32 Condition_CharacterLoaded(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *character_argument) {
    const i32 character = static_cast<i32>(AIConditionArgumentValue(character_argument));
    return APICharacterLoaded(character) != NULL ? 1.0f : 0.0f;
}

static __used__ f32 Condition_CutScenePlaying(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_CutSceneStarted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_DropBackInTimer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_FlowBoxComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_GizmoOutputInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_GizmoVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_GotLocatorInSet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_InLevelNodeInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_LocatorOnScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_NumForceObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static f32 Condition_ObstacleAtStart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg) {
    GIZMO_s *special = static_cast<GIZMO_s *>(void_arg);
    if (special == NULL) {
        return 0.0f;
    }
    return GizmoGetOutput(WORLD->gizmo_sys, special, 1, 1) == 0 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_OnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentContext(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OpponentIsAInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_PartyUnderCover(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerToLocator(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PrefersBrawling(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_ScriptParamInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_TurretAliveInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ i32 Action_CanHelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_ClearTakeOverTarget(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_ImmuneToKillTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_LaunchGuidedMissile(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_MoveAwayFromPlayer2(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_RetreatFromOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetMaxMovementRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetZeroAcceleration(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                               i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }
    GameObject_s *object = ActionOwner(packet);
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "player") == 0 || NuStrICmp(params[index], "player1") == 0) {
            object = player;
        } else if (NuStrICmp(params[index], "player2") == 0) {
            object = player2;
        }
    }
    if (object != NULL) {
        object->ai.runtime_flags =
            (object->ai.runtime_flags & ~4u) | (ActionToggleEnabled(params, param_count) ? 4u : 0u);
    }
    return 1;
}

static __used__ i32 Action_SplineFollowTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ void *Condition_AreaCompleteInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_CurrentLocatorIs(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *, char *,
                                               void *void_arg) {
    return processor != NULL && processor->locator == void_arg ? 1.0f : 0.0f;
}

static f32 Condition_CutSceneFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *void_arg) {
    CUTINFO *cutscene = static_cast<CUTINFO *>(void_arg);
    if (cutscene == NULL || cutscene->instance == NULL) {
        return 0.0f;
    }
    return instNuGCutSceneIsFinished(static_cast<instNUGCUTSCENE_s *>(cutscene->instance)) != 0 ? 1.0f : 0.0f;
}

static __used__ void *Condition_ForcePushingInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_IAmAGoodieBaddie(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_LocatorRangeInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    return arg != NULL ? AIPathFindLocator(system, arg) : NULL;
}

static __used__ f32 Condition_OnDynamicGrapple(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentToOrigin(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerCategoryIs(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument) {
    const i32 category = (i32)(isize)argument;
    if (category != -1 && player != NULL && CharCategory_IsCategory(player, category))
        return 1.0f;
    return 0.0f;
}

static __used__ void *Condition_PlayerInSockInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_PlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_RespawnLocatorIs(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ i32 Action_AwkwardShapeOverride(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_CanShootObstructions(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                                i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->field_0xefb = (object->field_0xefb & ~0x10u) | (ActionToggleEnabled(params, param_count) ? 0x10u : 0u);
    }
    return 1;
}

static __used__ i32 Action_DontUseShadowTerrain(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_MoveAwayFromOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_PartyCanBeUnderCover(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetAIOverrideControl(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                                char **params, i32 param_count, i32 first_time, f32) {
    if (processor == NULL) {
        return 1;
    }
    if (first_time != 0) {
        APIOBJECT_s *object = packet != NULL && packet->owner != NULL ? &packet->owner->apiobj : NULL;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = NuStrIStr(params[index], "character=");
            if (value != NULL && GetNamedAPIObjectFn != NULL) {
                object = GetNamedAPIObjectFn(system, value + 10);
            }
        }
        processor->action_data_3 = object;
    }
    APIOBJECT_s *object = static_cast<APIOBJECT_s *>(processor->action_data_3);
    if (object != NULL) {
        object->flags_high = (object->flags_high & ~1u) | (ActionToggleEnabled(params, param_count) ? 1u : 0u);
    }
    return 1;
}

static __used__ i32 Action_SetAtOnceRowDistance(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ f32 Condition_AIOverrideControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *,
                                                void *void_arg) {
    APIOBJECT_s *object = static_cast<APIOBJECT_s *>(void_arg);
    if (object == NULL && packet != NULL && packet->owner != NULL) {
        object = &packet->owner->apiobj;
    }
    return object != NULL && (object->flags_high & 1) != 0 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_AnimationFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_BeenTakenOverInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_CanFightLikeAJedi(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_ForceCompleteInit(AISYS_s *, char *name, AISCRIPT_s *) {
    GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, force_gizmotype_id, name);
    return gizmo != NULL ? gizmo->object : NULL;
}

static __used__ void *Condition_HintAvailableInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_InTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_LastAttackerRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_MaulShouldRunAway(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_NearestPartyRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OnSpeederBikeInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_OpponentIsAThreat(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    return packet != NULL && (packet->field_0x1e5 & 8) != 0 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_OpponentToLocator(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerInLevelNode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ i32 Action_AlwaysTriggerObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **params,
                                                 i32 param_count, i32 first_time, f32) {
    if (first_time == 0 || param_count == 0) {
        return 1;
    }
    GIZOBSTACLE *obstacle = NULL;
    bool enabled = true;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            obstacle = ActionFindObstacle(value + NuStrLen("name="));
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = false;
        }
    }
    if (obstacle != NULL) {
        obstacle->runtime_flags = static_cast<u8>((obstacle->runtime_flags & ~4u) | (enabled ? 4u : 0u));
    }
    return 1;
}

static __used__ i32 Action_CanCollideWithObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static i32 Action_CharClipToBlobShadows(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                        i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->jump_input_flags =
            (object->jump_input_flags & ~0x80u) | (ActionToggleEnabled(params, param_count) ? 0x80u : 0u);
    }
    return 1;
}

static __used__ i32 Action_CreateSplineCreatures(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                                 char **params, i32 num_params, i32 first_time, f32) {
    char script_name[64] = "default";
    NUVEC spline_offset = {0.0f, 0.0f, 0.0f};
    if (!first_time || num_params <= 0)
        return 1;
    i16 models[10];
    NUGSPLINE *splines[32];
    i32 model_count = 0, spline_count = 0, use_selected = 0;
    i32 min_group_size = -1, max_group_size = -1, group_size = 1;
    f32 min_distance = 1000000000.0f, max_distance = 1000000000.0f, distance = 0.0f;
    i32 relative_to_player = 0, creature_set = 0, looping = 0;
    AILOCATOR *relative_locator = NULL;
    GameObject_s *rider = NULL;
    for (i32 i = 0; i < num_params; i++) {
        char *value = NuStrIStr(params[i], "mingroupsize");
        if (value != NULL)
            min_group_size = (i32)AIParamToFloat(processor, value + 13);
        else if ((value = NuStrIStr(params[i], "maxgroupsize")) != NULL)
            max_group_size = (i32)AIParamToFloat(processor, value + 13);
        else if ((value = NuStrIStr(params[i], "groupsize")) != NULL)
            group_size = (i32)AIParamToFloat(processor, value + 10);
        else if ((value = NuStrIStr(params[i], "type")) != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                u8 type = LevelCharacterTypeIDFn(value + 5);
                if (type != 0xff) {
                    i16 model = LevelCharacterGlobalIDFn(type);
                    if (model != -1 && model_count < 10)
                        models[model_count++] = model;
                }
            }
        } else if ((value = NuStrIStr(params[i], "script")) != NULL)
            NuStrCpy(script_name, value + 7);
        else if ((value = NuStrIStr(params[i], "mindistance")) != NULL)
            min_distance = AIParamToFloat(processor, value + 12);
        else if ((value = NuStrIStr(params[i], "maxdistance")) != NULL)
            max_distance = AIParamToFloat(processor, value + 12);
        else if ((value = NuStrIStr(params[i], "distance")) != NULL)
            distance = AIParamToFloat(processor, value + 9);
        else if (script_spline_selected != NULL && NuStrICmp("use_selected_spline", params[i]) == 0)
            use_selected = 1;
        else if ((value = NuStrIStr(params[i], "splines=")) != NULL)
            spline_count +=
                NuSplineFindAllBeg(WORLD->current_gscn, value + 8, &splines[spline_count], 32 - spline_count);
        else if (NuStrIStr(params[i], "spline=myspline") != NULL) {
            if (processor->unknown_ac != NULL)
                splines[spline_count++] = processor->unknown_ac;
        } else if ((value = NuStrIStr(params[i], "spline=")) != NULL) {
            NUGSPLINE *spline = NuSplineFind(WORLD->current_gscn, value + 7);
            if (spline != NULL && spline_count < 32)
                splines[spline_count++] = spline;
        } else if ((value = NuStrIStr(params[i], "x_offset=")) != NULL)
            spline_offset.x = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[i], "y_offset=")) != NULL)
            spline_offset.y = AIParamToFloat(processor, value + 9);
        else if ((value = NuStrIStr(params[i], "z_offset=")) != NULL)
            spline_offset.z = AIParamToFloat(processor, value + 9);
        else if (NuStrICmp(params[i], "looping") == 0)
            looping = 1;
        else if (NuStrIStr(params[i], "addtoset=myset") != NULL)
            creature_set = processor->unknown_b0;
        else if ((value = NuStrIStr(params[i], "addtoset=")) != NULL)
            creature_set = (i32)AIParamToFloat(processor, value + 9);
        else if (NuStrICmp(params[i], "relative_to_player") == 0)
            relative_to_player = 1;
        else if ((value = NuStrIStr(params[i], "relative_to_locator=")) != NULL)
            relative_locator = AIPathFindLocator(system, value + 20);
        else if (NuStrIStr(params[i], "ridden_by=myself") != NULL) {
            if (packet != NULL && packet->owner != NULL)
                rider = packet->owner->apiobj.objptr;
        } else if ((value = NuStrIStr(params[i], "ridden_by=")) != NULL)
            rider = GetNamedGameObject(system, value + 10);
    }
    if (min_group_size >= 0 && min_group_size < max_group_size)
        group_size = NuRand(NULL) % (max_group_size + 1 - min_group_size) + min_group_size;
    if (min_distance != 1000000000.0f && max_distance != 1000000000.0f) {
        f32 fraction = NuRandFloat();
        distance = max_distance * fraction + min_distance * (1.0f - fraction);
    }
    if ((spline_count | use_selected) == 0 || group_size <= 0 || model_count == 0)
        return 1;
    NUGSPLINE *spline = use_selected ? script_spline_selected : splines[qrand() / (65535 / spline_count + 1)];
    i32 model = models[NuRand(NULL) % model_count];
    for (i32 i = 0; i < group_size; i++) {
        GameObject_s *object = AddDynamicCreature(model, NULL, 0, script_name, NULL, NULL, 0, spline, &spline_offset,
                                                  looping, creature_set);
        if (object == NULL)
            continue;
        if (relative_to_player || relative_locator != NULL) {
            i16 start = -1, end = -1;
            if (WORLD->current_level == PODSPRINTA_LDATA)
                PodSprint_GetIAlongVals(spline, &start, &end);
            SPLINEPOS_s position;
            if (relative_to_player)
                GetNearestSplinePos(&player->apiobj.collision_position, &position, spline, looping, start, end);
            else
                GetNearestSplinePos(&relative_locator->position, &position, spline, looping, start, end);
            object->movement_spline_position = position;
        }
        if (distance != 0.0f) {
            MoveSplinePosition(&object->movement_spline_position, distance);
            NUVEC position, offset;
            u16 angle, pitch;
            PointAlongSpline(object->movement_spline, object->movement_spline_position.along, &position, &angle, &pitch,
                             object->movement_spline_position.looping);
            object->apiobj.facing_angle = angle;
            object->apiobj.movement_facing_angle = angle;
            object->apiobj.field_0x276 = angle;
            object->apiobj.pitch_angle = -pitch;
            if (object->movement_spline_offset.x != 0.0f || object->movement_spline_offset.y != 0.0f ||
                object->movement_spline_offset.z != 0.0f) {
                NuVecRotateX(&offset, &object->movement_spline_offset, object->apiobj.pitch_angle);
                NuVecRotateY(&offset, &offset, object->apiobj.field_0x276);
                NuVecAdd(&position, &position, &offset);
            }
            object->apiobj.position = position;
            object->apiobj.initial_position = position;
            object->apiobj.collision_position = position;
            plr_lastpos = position;
            object->apiobj.start_position = position;
            GameObjectOrigin(object);
            object->apiobj.last_safe_position = object->apiobj.position;
            object->field_0x10c8 = object->apiobj.position.x;
            object->field_0x10cc = object->apiobj.position.y;
            object->field_0x10d0 = object->apiobj.position.z;
        }
        if (rider != NULL)
            TakeOverGameObject(rider, object, 0, 1);
    }
    return 1;
}

static __used__ i32 Action_IgnoreLastSafePathPos(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ i32 Action_SetFormationCommander(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

static __used__ f32 Condition_BaddyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_CharacterRangeInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_CutSceneExistsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_EitherPlayerIsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_ForceStackComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_GoodyInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_HeadTurnRestricted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_IAmAPartyCharacter(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_LevelNodeRangeInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_NearestPlayerRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_NetworkGameOnGoing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_ObstacleLockedOpen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_ObstacleLockedShut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OffScreenTimerInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    return arg != NULL ? GetNamedGameObject(system, arg) : NULL;
}

static __used__ f32 Condition_OpponentOnSamePath(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentToLocatorY(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PickupBeenTurnedOn(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_RigidAnimFrameInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_UnderPlayerControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ i32 Action_CanMoveWhenDeactivated(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ i32 Action_IgnoreTurnAroundSpline(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ i32 Action_LinkTurretToController(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ i32 Action_PathConnectionObstacle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ i32 Action_RegisterTakeOverObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ i32 Action_SetDoomedEscapeLocator(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}

static __used__ f32 Condition_AreaContainsBaddies(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_AreaContainsGoodies(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_BuildItCompleteInit(AISYS_s *, char *name, AISCRIPT_s *) {
    return GizmoFindByName(WORLD->gizmo_sys, gizbuildit_gizmotype_id, name);
}

static __used__ void *Condition_CharacterExistsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static void *Condition_CharacterLoadedInit(AISYS_s *, char *argument, AISCRIPT_s *) {
    if (argument == NULL) {
        return NULL;
    }
    return reinterpret_cast<void *>(static_cast<isize>(CharIDFromName(argument)));
}

static __used__ f32 Condition_CharacterTypeExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_CutScenePlayingInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_CutSceneStartedInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_FlowBoxCompleteInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_GizmoVisibilityInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_GotLocatorInSetInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_LocatorOnScreenInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_NearestPartyXZRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_NumForceObjectsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_OnForcePlatformInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_OpponentToLocatorXZ(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PartyContainsDroids(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_PlayerToLocatorInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ i32 Action_PathConnectionMaxLength(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                   f32) {
    return 0;
}

static __used__ i32 Action_SetDefaultMovementRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                   f32) {
    return 0;
}

static __used__ void *Condition_CurrentLocatorIsInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    return arg != NULL ? AIPathFindLocator(system, arg) : NULL;
}

static void *Condition_CutSceneFinishedInit(AISYS_s *, char *arg, AISCRIPT_s *) {
    if (WORLD == NULL) {
        return NULL;
    }
    return CutScene_Find(WORLD->cutscene_sys, arg);
}

static __used__ f32 Condition_EitherPlayerOnObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_NearestOpponentRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    return packet != NULL && packet->nearest_opponent != NULL ? packet->nearest_opponent_metric : 1.0e9f;
}

static __used__ f32 Condition_NearestPlayerXZRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OnDynamicGrappleInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_OnSameObjectAsPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentPathPosRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_Player2InTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerDeflectingPart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_RespawnLocatorIsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_ShouldAttackOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_SockDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ i32 Action_MoveAwayFromLastAttacker(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                    f32) {
    return 0;
}

static __used__ i32 Action_RemoveThrownForceObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                    f32) {
    return 0;
}

static __used__ void *Condition_AIOverrideControlInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    return arg != NULL && GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(system, arg) : NULL;
}

static __used__ void *Condition_AnimationFinishedInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_CollidingWithOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_OpponentInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OpponentToLocatorInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_OpponentToPlayerRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_PlayerInLevelNodeInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_PlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_SockXDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_BaddyInTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_CannotReachDestination(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *,
                                                     void *) {
    return packet != NULL && (packet->runtime_flags & 0x40) != 0 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_EitherPlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerUsingPanel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_GoodyInTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_NearestPlayerToLocator(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_ObstacleOpenedByPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_PickupBeenTurnedOnInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_SockDistanceToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_UnderPlayerControlInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ i32 Action_RetreatFromNearestOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                      f32) {
    return 0;
}

static __used__ void *Condition_AreaContainsBaddiesInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_AreaContainsGoodiesInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_AreaContainsPartyMember(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_CharacterTypeExistsInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_EitherPlayerInLevelNode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_PlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_PlayerInTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_RandomMapCharsAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    if (hub_custodians_finished_loading == 0) {
        return 0.0f;
    }
    return Hub_GetRandomCharType() != -1 ? 1.0f : 0.0f;
}

static __used__ f32 Condition_EitherPlayerPullingLever(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static i32 Action_SetBoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params,
                                               i32 param_count, i32 first_time, f32) {
    GameObject_s *object = ActionOwner(packet);
    if (first_time != 0 && object != NULL) {
        object->field_0xefc = (object->field_0xefc & ~8u) | (ActionToggleEnabled(params, param_count) ? 8u : 0u);
    }
    return 1;
}

static __used__ f32 Condition_BoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerWearingHelmet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_ForceStackCompleteInOrder(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OpponentInTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_EitherPlayerLocatorRangeXZ(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerPushingSpinner(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ f32 Condition_LastAttackerIsActivePlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_ObstacleOpenedByPlayerInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_AngleAboutMyLocatorToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ void *Condition_AreaContainsPartyMemberInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_EitherPlayerInMyTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ f32 Condition_EitherPlayerUsingHatMachine(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ f32 Condition_NumBaddiesThatCanSeePlayers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ f32 Condition_TakeOverTargetInTriggerArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                          void *) {
    return 0;
}

static __used__ f32 Condition_ObstacleOpenedByEitherPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                           void *) {
    return 0;
}

static __used__ void *Condition_EitherPlayerPushingSpinnerInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ void *Condition_AngleAboutMyLocatorToPlayerInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_FurthestPlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                              void *) {
    return 0;
}

static __used__ void *Condition_TakeOverTargetInTriggerAreaInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}

static __used__ f32 Condition_InSameTriggerAreaAsNearestPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                               void *) {
    return 0;
}

namespace {
    struct GameAIRegistryCallbacks {
        GameAIRegistryCallbacks() {
            api_aiactiondefs[API_AI_ACTION_OVERRIDE_ANIMATION].eval_fn = Action_OverrideAnimation;
            api_aiactiondefs[API_AI_ACTION_IGNORE_WALL_SPLINES].eval_fn = Action_IgnoreWallSplines;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_DOOMED_ESCAPE_LOCATOR].eval_fn = Action_SetDoomedEscapeLocator;
            lego_aiactiondefs[LEGO_AI_ACTION_SNAP_TO_SOCK_POSITION].eval_fn = Action_SnapToSockPosition;
            lego_aiactiondefs[LEGO_AI_ACTION_CAN_SHOOT_OFF_SCREEN].eval_fn = Action_CanShootOffScreen;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_BOLTS_DONT_GET_DEFLECTED_BACK].eval_fn =
                Action_SetBoltsDontGetDeflectedBack;
            lego_aiactiondefs[LEGO_AI_ACTION_CAN_SHOOT_OBSTRUCTIONS].eval_fn = Action_CanShootObstructions;
            lego_aiactiondefs[LEGO_AI_ACTION_CAN_HIT_FORCE_OBJECTS].eval_fn = Action_CanHitForceObjects;
            lego_aiactiondefs[LEGO_AI_ACTION_PLAYER_SPEEDER_HACK].eval_fn = Action_PlayerSpeederHack;
            lego_aiactiondefs[LEGO_AI_ACTION_CHAR_CLIP_TO_BLOB_SHADOWS].eval_fn = Action_CharClipToBlobShadows;
            lego_aiactiondefs[LEGO_AI_ACTION_DEFLECT_PLAYERS_PART].eval_fn = Action_DeflectPlayersPart;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_AI_OVERRIDE_CONTROL].eval_fn = Action_SetAIOverrideControl;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_LAST_SAFE_PATH_POS].eval_fn = Action_SetLastSafePathPos;
            lego_aiactiondefs[LEGO_AI_ACTION_DONT_SET_STOPPED_FLAG].eval_fn = Action_DontSetStoppedFlag;
            lego_aiactiondefs[LEGO_AI_ACTION_PRESS_SPECIAL_BUTTON].eval_fn = Action_PressSpecialButton;
            lego_aiactiondefs[LEGO_AI_ACTION_PRESS_ACTION_BUTTON].eval_fn = Action_PressActionButton;
            lego_aiactiondefs[LEGO_AI_ACTION_DONT_AVOID_CHARACTER].eval_fn = Action_DontAvoidCharacter;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_ZERO_ACCELERATION].eval_fn = Action_SetZeroAcceleration;
            lego_aiactiondefs[LEGO_AI_ACTION_CREATE_SPLINE_CREATURES].eval_fn = Action_CreateSplineCreatures;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_CHARACTER].eval_fn = Action_FollowCharacter;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_FORWARD].eval_fn = Action_MoveForward;
            lego_aiactiondefs[LEGO_AI_ACTION_ALWAYS_TRIGGER_OBSTACLE].eval_fn = Action_AlwaysTriggerObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_CAN_TRIGGER_OBSTACLE].eval_fn = Action_CanTriggerObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_GIZMO_SET_VISIBILITY].eval_fn = Action_GizmoSetVisibility;
            lego_aiactiondefs[LEGO_AI_ACTION_SELECT_RANDOM_SPLINE].eval_fn = Action_SelectRandomSpline;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_SHIELD_HIT_POINTS].eval_fn = Action_SetShieldHitPoints;

            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE].init_fn = Condition_LocatorRangeInit;
            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE_XZ].eval_fn = Condition_LocatorRangeXZ;
            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE_XZ].init_fn = Condition_LocatorRangeInit;
            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE_Y].eval_fn = Condition_LocatorRangeY;
            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE_Y].init_fn = Condition_LocatorRangeInit;
            api_aiconditiondefs[API_AI_CONDITION_GOT_LOCATOR_SET].eval_fn = Condition_GotLocatorSet;
            api_aiconditiondefs[API_AI_CONDITION_CURRENT_LOCATOR_IS].eval_fn = Condition_CurrentLocatorIs;
            api_aiconditiondefs[API_AI_CONDITION_CURRENT_LOCATOR_IS].init_fn = Condition_CurrentLocatorIsInit;
            api_aiconditiondefs[API_AI_CONDITION_OPPONENT_RANGE].eval_fn = Condition_OpponentRange;
            api_aiconditiondefs[API_AI_CONDITION_OPPONENT_IS_A_THREAT].eval_fn = Condition_OpponentIsAThreat;
            api_aiconditiondefs[API_AI_CONDITION_NEAREST_OPPONENT_RANGE].eval_fn = Condition_NearestOpponentRange;
            api_aiconditiondefs[API_AI_CONDITION_YAW_TO_OPPONENT].eval_fn = Condition_YawToOpponent;
            api_aiconditiondefs[API_AI_CONDITION_OPPONENT_BELOW].eval_fn = Condition_OpponentBelow;

            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].eval_fn = Condition_OffScreenTimer;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].init_fn = Condition_OffScreenTimerInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SPECIAL_AT_START].eval_fn = Condition_ObstacleAtStart;
            lego_aiconditiondefs[LEGO_AI_CONDITION_SPECIAL_AT_START].init_fn = Condition_GizSpecialInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_COMPLETE].eval_fn = Condition_ForceComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_COMPLETE].init_fn = Condition_ForceCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_FINISHED].eval_fn = Condition_ForceFinished;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_FINISHED].init_fn = Condition_ForceCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].init_fn = Condition_CategoryIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_NUM_IN_SET_ALIVE].eval_fn = Condition_NumInSetAlive;
            lego_aiconditiondefs[LEGO_AI_CONDITION_NUM_IN_SET_ALIVE].init_fn = Condition_IsSetAliveInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_TO_LEVEL].init_fn = Condition_BeenToLevelInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CUT_SCENE_FINISHED].eval_fn = Condition_CutSceneFinished;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CUT_SCENE_FINISHED].init_fn = Condition_CutSceneFinishedInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_AI_OVERRIDE_CONTROL].eval_fn = Condition_AIOverrideControl;
            lego_aiconditiondefs[LEGO_AI_CONDITION_AI_OVERRIDE_CONTROL].init_fn = Condition_AIOverrideControlInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FINISHED_SPLINE].eval_fn = Condition_FinishedSpline;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CANNOT_REACH_DESTINATION].eval_fn = Condition_CannotReachDestination;
            lego_aiconditiondefs[LEGO_AI_CONDITION_IS_LOW_END_DEVICE].eval_fn = Condition_IsLowEndDevice;
            lego_aiconditiondefs[LEGO_AI_CONDITION_RANDOM_MAP_CHARS_AVAILABLE].eval_fn =
                Condition_RandomMapCharsAvailable;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_LOADED].eval_fn = Condition_CharacterLoaded;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CHARACTER_LOADED].init_fn = Condition_CharacterLoadedInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_COMPLETE].eval_fn = Condition_ForceComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FORCE_COMPLETE].init_fn = Condition_ForceCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BUILDIT_COMPLETE].eval_fn = Condition_BuildItComplete;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BUILDIT_COMPLETE].init_fn = Condition_BuildItCompleteInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].init_fn = Condition_CategoryIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_CATEGORY_IS].init_fn = Condition_CategoryIsInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_PLAYER_CATEGORY_IS].eval_fn = Condition_PlayerCategoryIs;
        }
    };

    GameAIRegistryCallbacks game_ai_registry_callbacks;
} // namespace
