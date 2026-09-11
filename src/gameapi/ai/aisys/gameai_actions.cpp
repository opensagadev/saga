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

static __used__ i32 Action_LaunchGuidedMissile(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32) {
    return 0;
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

static __used__ f32 Condition_HeadTurnRestricted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *) {
    GameObject_s *object = ActionOwner(packet);
    if (object == NULL) {
        return 0.0f;
    }
    return static_cast<f32>(static_cast<u32>(object->field_0xefe & 1));
}

static __used__ f32 Condition_NetworkGameOnGoing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *) {
    return 0;
}

i32 Action_GameFollowPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, f32);

namespace {
    struct GameAIRegistryCallbacks {
        GameAIRegistryCallbacks() {
            lego_aiactiondefs[LEGO_AI_ACTION_CREATE_SPLINE_CREATURES].eval_fn = Action_CreateSplineCreatures;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_CHARACTER].eval_fn = Action_FollowCharacter;
            lego_aiactiondefs[LEGO_AI_ACTION_FOLLOW_PLAYER].eval_fn = Action_GameFollowPlayer;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_FORWARD].eval_fn = Action_MoveForward;
        }
    };

    GameAIRegistryCallbacks game_ai_registry_callbacks;
} // namespace
