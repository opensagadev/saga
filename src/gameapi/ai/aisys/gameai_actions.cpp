#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
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
#include "nu2api/numath/nurand.h"
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


static GameObject_s *ActionOwner(AIPACKET_s *packet) {
    return packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
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
























static __used__ f32 Condition_BuildItComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}




















static __used__ i32 Action_LaunchGuidedMissile(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
}

































static void PositionSplineCreature(GameObject_s *object) {
    SPLINEPOSITION_RUNTIME_s *runtime = reinterpret_cast<SPLINEPOSITION_RUNTIME_s *>(&object->movement_spline);
    NUVEC position;
    u16 yaw = 0;
    u16 pitch = 0;
    PointAlongSpline(runtime->spline, runtime->normalized_position, &position, &yaw, &pitch, runtime->looping);
    object->apiobj.facing_angle = yaw;
    object->apiobj.movement_facing_angle = yaw;
    object->apiobj.field_0x276 = yaw;
    object->apiobj.pitch_angle = static_cast<u16>(-pitch);

    NUVEC *offset = reinterpret_cast<NUVEC *>(reinterpret_cast<u8 *>(runtime) + 0x20);
    if (offset->x != 0.0f || offset->y != 0.0f || offset->z != 0.0f) {
        NUVEC rotated;
        NuVecRotateX(&rotated, offset, static_cast<u16>(-pitch));
        NuVecRotateY(&rotated, &rotated, yaw);
        NuVecAdd(&position, &position, &rotated);
    }

    object->apiobj.position = position;
    object->apiobj.collision_position = position;
    object->apiobj.start_position = position;
    object->apiobj.initial_position = position;
    plr_lastpos = position;
    GameObjectOrigin(object);
    object->apiobj.last_safe_position = position;
    object->field_0x10c8 = position.x;
    object->field_0x10cc = position.y;
    object->field_0x10d0 = position.z;
}

static __used__ i32 Action_CreateSplineCreatures(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet,
                                                 char **params, i32 param_count, i32 first_time, f32) {
    if (first_time == 0 || param_count < 1) {
        return 1;
    }

    i32 min_group_size = -1;
    i32 max_group_size = -1;
    i32 group_size = 1;
    i16 character_types[10];
    i32 character_type_count = 0;
    char script_name[64] = "default";
    f32 min_distance = 1.0e9f;
    f32 max_distance = 1.0e9f;
    f32 distance = 0.0f;
    NUGSPLINE *splines[32];
    i32 spline_count = 0;
    bool use_selected_spline = false;
    NUVEC offset = {};
    i32 looping = 0;
    i32 creature_set = 0;
    bool relative_to_player = false;
    AILOCATOR *relative_locator = NULL;
    GameObject_s *rider = NULL;

    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        char *value = NuStrIStr(param, "mingroupsize");
        if (value != NULL) {
            min_group_size = static_cast<i32>(AIParamToFloat(processor, value + 13));
            continue;
        }
        value = NuStrIStr(param, "maxgroupsize");
        if (value != NULL) {
            max_group_size = static_cast<i32>(AIParamToFloat(processor, value + 13));
            continue;
        }
        value = NuStrIStr(param, "groupsize");
        if (value != NULL) {
            group_size = static_cast<i32>(AIParamToFloat(processor, value + 10));
            continue;
        }
        value = NuStrIStr(param, "type=");
        if (value != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL && character_type_count < 10) {
                const i32 level_type = LevelCharacterTypeIDFn(value + 5);
                const i32 global_type = level_type != -1 ? LevelCharacterGlobalIDFn(static_cast<u8>(level_type)) : -1;
                if (global_type != -1) {
                    character_types[character_type_count++] = static_cast<i16>(global_type);
                }
            }
            continue;
        }
        value = NuStrIStr(param, "script");
        if (value != NULL) {
            NuStrCpy(script_name, value + 7);
            continue;
        }
        value = NuStrIStr(param, "mindistance");
        if (value != NULL) {
            min_distance = AIParamToFloat(processor, value + 12);
            continue;
        }
        value = NuStrIStr(param, "maxdistance");
        if (value != NULL) {
            max_distance = AIParamToFloat(processor, value + 12);
            continue;
        }
        value = NuStrIStr(param, "distance");
        if (value != NULL) {
            distance = AIParamToFloat(processor, value + 9);
            continue;
        }
        if (script_spline_selected != NULL && NuStrICmp(param, "use_selected_spline") == 0) {
            use_selected_spline = true;
            continue;
        }
        value = NuStrIStr(param, "splines=");
        if (value != NULL) {
            spline_count +=
                NuSplineFindAllBeg(WORLD->current_gscn, value + 8, splines + spline_count, 32 - spline_count);
            continue;
        }
        value = NuStrIStr(param, "spline=myspline");
        if (value != NULL) {
            if (processor != NULL && processor->unknown_ac != NULL && spline_count < 32) {
                splines[spline_count++] = processor->unknown_ac;
            }
            continue;
        }
        value = NuStrIStr(param, "spline=");
        if (value != NULL) {
            NUGSPLINE *spline = NuSplineFind(WORLD->current_gscn, value + 7);
            if (spline != NULL && spline_count < 32) {
                splines[spline_count++] = spline;
            }
            continue;
        }
        value = NuStrIStr(param, "x_offset=");
        if (value != NULL) {
            offset.x = AIParamToFloat(processor, value + 9);
            continue;
        }
        value = NuStrIStr(param, "y_offset=");
        if (value != NULL) {
            offset.y = AIParamToFloat(processor, value + 9);
            continue;
        }
        value = NuStrIStr(param, "z_offset=");
        if (value != NULL) {
            offset.z = AIParamToFloat(processor, value + 9);
            continue;
        }
        if (NuStrICmp(param, "looping") == 0) {
            looping = 1;
            continue;
        }
        value = NuStrIStr(param, "addtoset=myset");
        if (value != NULL) {
            creature_set = processor != NULL ? processor->unknown_b0 : 0;
            continue;
        }
        value = NuStrIStr(param, "addtoset=");
        if (value != NULL) {
            creature_set = static_cast<i32>(AIParamToFloat(processor, value + 9));
            continue;
        }
        if (NuStrICmp(param, "relative_to_player") == 0) {
            relative_to_player = true;
            continue;
        }
        value = NuStrIStr(param, "relative_to_locator=");
        if (value != NULL) {
            relative_locator = AIPathFindLocator(system, value + 20);
            continue;
        }
        value = NuStrIStr(param, "ridden_by=myself");
        if (value != NULL) {
            rider = ActionOwner(packet);
            continue;
        }
        value = NuStrIStr(param, "ridden_by=");
        if (value != NULL) {
            rider = GetNamedGameObject(system, value + 10);
        }
    }

    if (min_group_size >= 0 && min_group_size < max_group_size) {
        group_size = NuRand(NULL) % (max_group_size + 1 - min_group_size) + min_group_size;
    }
    if (min_distance != 1.0e9f || max_distance != 1.0e9f) {
        const f32 random = NuRandFloat();
        distance = (1.0f - random) * min_distance + random * max_distance;
    }
    if (group_size <= 0 || character_type_count == 0 || (!use_selected_spline && spline_count == 0)) {
        return 1;
    }

    NUGSPLINE *spline = use_selected_spline ? script_spline_selected : splines[qrand() / (0xffff / spline_count + 1)];
    const i16 character_type = character_types[NuRand(NULL) % character_type_count];
    for (i32 index = 0; index < group_size; ++index) {
        GameObject_s *object = AddDynamicCreature(character_type, NULL, 0, script_name, NULL, NULL, 0, spline, &offset,
                                                  looping, creature_set);
        if (object == NULL) {
            continue;
        }
        if (relative_to_player || relative_locator != NULL) {
            i16 first_point = -1;
            i16 last_point = -1;
            PodSprint_GetIAlongVals(spline, &first_point, &last_point);
            NUVEC *relative_position = relative_to_player && Player[0] != NULL ? &Player[0]->apiobj.collision_position
                                                                               : &relative_locator->position;
            GetNearestSplinePos(relative_position, reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline), spline,
                                looping, first_point, last_point);
        }
        if (distance != 0.0f) {
            MoveSplinePosition(reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline), distance);
            PositionSplineCreature(object);
        }
        if (rider != NULL) {
            TakeOverGameObject(rider, object, 0, 1);
        }
    }
    return 1;
}





static __used__ f32 Condition_HeadTurnRestricted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}




static __used__ f32 Condition_NetworkGameOnGoing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

static __used__ void *Condition_OffScreenTimerInit(AISYS_s *system, char *arg, AISCRIPT_s *) {
    return arg != NULL ? GetNamedGameObject(system, arg) : NULL;
}







static __used__ i32 Action_LinkTurretToController(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}


static __used__ i32 Action_RegisterTakeOverObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32,
                                                  f32) {
    return 0;
}




static __used__ void *Condition_BuildItCompleteInit(AISYS_s *, char *, AISCRIPT_s *) {
    return nullptr;
}



































































namespace {
    struct GameAIRegistryCallbacks {
        GameAIRegistryCallbacks() {
            lego_aiactiondefs[LEGO_AI_ACTION_CREATE_SPLINE_CREATURES].eval_fn = Action_CreateSplineCreatures;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_CHARACTER].eval_fn = Action_FollowCharacter;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_FORWARD].eval_fn = Action_MoveForward;


            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].eval_fn = Condition_OffScreenTimer;
            lego_aiconditiondefs[LEGO_AI_CONDITION_OFF_SCREEN_TIMER].init_fn = Condition_OffScreenTimerInit;
        }
    };

    GameAIRegistryCallbacks game_ai_registry_callbacks;
} // namespace
