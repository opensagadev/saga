#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nurand.h"

void Action_Circle(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_Sebulba(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
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

void Action_UsePanel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_CameraCut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_CreatePod(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_PullLever(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_UseTechno(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_NewSebulba(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_SetLapTime(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_MoveForward(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_CirclePlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_EndCameraCut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
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

    if (sys != NULL && sys->player_1 != NULL && sys->player_1->ai != NULL) {
        FollowAPIObject(&packet->owner->apiobj, sys->player_1, processor->action_data_1,
                        packet->movement_instruction_parameter);
    }
    return 0;
}

void Action_PlayCutScene(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_SetVisibility(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_UseTriggerSet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_BoulderSection(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_CircleOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

i32 Action_ReleaseLocator(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                          i32 first_time, float) {
    if (first_time == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    for (i32 index = 0; index < param_count; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
        }
    }
    if (object != NULL) {
        object->ai.locator = NULL;
    }
    return 1;
}

void Action_DynamicCameraCut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_GameFollowPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

void Action_MushroomCollapse(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

i32 Action_GetLocatorFromSet(AISYS_s *sys, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                             i32 param_count, i32 first_time, float) {
    if (first_time == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    AILOCATORSET *locator_set = processor->unknown_a8;
    f32 max_range = 0.0f;
    f32 off_screen_radius = 0.0f;
    bool random = false;
    bool next = false;
    bool first = false;
    bool looping = false;
    bool finish_at_end = false;
    bool use_player = false;
    bool use_opponent = false;
    bool use_second_player = false;
    bool furthest = false;
    i32 ignore_assigned = -1;

    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        char *value = NuStrIStr(param, "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
            continue;
        }
        value = NuStrIStr(param, "max_range=");
        if (value != NULL) {
            max_range = AIParamToFloat(processor, value + NuStrLen("max_range="));
            continue;
        }
        value = NuStrIStr(param, "max_player_range=");
        if (value != NULL) {
            max_range = AIParamToFloat(processor, value + NuStrLen("max_player_range="));
            use_player = true;
            continue;
        }
        value = NuStrIStr(param, "max_opponent_range=");
        if (value != NULL) {
            max_range = AIParamToFloat(processor, value + NuStrLen("max_opponent_range="));
            use_opponent = true;
            continue;
        }
        value = NuStrIStr(param, "off_screen_radius=");
        if (value != NULL) {
            off_screen_radius = AIParamToFloat(processor, value + NuStrLen("off_screen_radius="));
            continue;
        }
        value = NuStrIStr(param, "name=");
        if (value != NULL) {
            locator_set = AIPathFindLocatorSet(sys, value + NuStrLen("name="));
            continue;
        }

        if (NuStrICmp(param, "random") == 0) {
            random = true;
        } else if (NuStrICmp(param, "next") == 0) {
            next = true;
        } else if (NuStrICmp(param, "first") == 0) {
            first = true;
        } else if (NuStrICmp(param, "looping") == 0) {
            looping = true;
        } else if (NuStrICmp(param, "finish_at_end") == 0) {
            finish_at_end = true;
        } else if (NuStrICmp(param, "ignore_assigned=TRUE") == 0) {
            ignore_assigned = 1;
        } else if (NuStrICmp(param, "ignore_assigned=FALSE") == 0) {
            ignore_assigned = 0;
        } else if (NuStrICmp(param, "furthest_from_opponent") == 0) {
            furthest = true;
            use_opponent = true;
        } else if (NuStrICmp(param, "furthest_from_either_player") == 0) {
            furthest = true;
            use_player = true;
            use_second_player = true;
        } else if (NuStrICmp(param, "nearest_either_player") == 0) {
            use_player = true;
            use_second_player = true;
        }
    }

    if (ignore_assigned == -1) {
        ignore_assigned = next ? 0 : 1;
    }
    if (object == NULL || locator_set == NULL) {
        return 1;
    }

    APIOBJECT *target = &object->apiobj;
    NUVEC *reference_position = &target->position;
    if (use_player && player != NULL) {
        reference_position = &player->apiobj.position;
    } else if (use_opponent && object->ai.opponent != NULL) {
        reference_position = &static_cast<APIOBJECT *>(object->ai.opponent)->position;
    }
    NUVEC *second_position =
        use_second_player && player2 != NULL ? &player2->apiobj.position : static_cast<NUVEC *>(NULL);

    if (first) {
        if (locator_set->locator_count > 0) {
            object->ai.locator = &sys->locators[locator_set->locator_entries[0]];
            locator_set->assigned[0] = target->field_0x289;
        }
        return 1;
    }

    if (!next) {
        if (random) {
            AILocatorSet_AssignRandomLocator(sys, locator_set, target, max_range, reference_position, off_screen_radius,
                                             ignore_assigned);
        } else if (furthest) {
            AILocatorSet_AssignFurthestLocator(sys, locator_set, target, max_range, reference_position, second_position,
                                               off_screen_radius, ignore_assigned);
        } else {
            AILocatorSet_AssignNearestLocator(sys, locator_set, target, max_range, reference_position, second_position,
                                              off_screen_radius, ignore_assigned);
        }
        return 1;
    }

    const i32 locator_count = locator_set->locator_count;
    if (locator_count < 2) {
        return 1;
    }
    if (ignore_assigned != 0) {
        AILocatorSet_CheckLocatorsStillAssigned(sys, locator_set);
    }

    i32 current_index = -1;
    for (i32 index = 0; index < locator_count; ++index) {
        if (object->ai.locator == &sys->locators[locator_set->locator_entries[index]]) {
            current_index = index;
            locator_set->assigned[index] = 0xff;
            break;
        }
    }
    if (current_index == -1) {
        current_index = NuRandInt() % locator_count;
    }

    i32 direction = (target->field_0x1fa & 0x10) == 0 ? 1 : -1;
    for (i32 checked = 0; checked < locator_count; ++checked) {
        i32 candidate = current_index + direction;
        if (candidate < 0 || candidate >= locator_count) {
            if (looping) {
                if (finish_at_end) {
                    object->ai.locator = NULL;
                    return 1;
                }
                candidate = candidate < 0 ? locator_count - 1 : 0;
            } else {
                direction = -direction;
                if (direction < 0) {
                    target->field_0x1fa |= 0x10;
                    candidate = locator_count - 2;
                } else {
                    target->field_0x1fa &= static_cast<u8>(~0x10);
                    candidate = 1;
                }
                if (finish_at_end && checked != 0) {
                    object->ai.locator = NULL;
                    return 1;
                }
            }
        }
        current_index = candidate;
        if (ignore_assigned == 0 || locator_set->assigned[current_index] == 0xff) {
            object->ai.locator = &sys->locators[locator_set->locator_entries[current_index]];
            locator_set->assigned[current_index] = target->field_0x289;
            return 1;
        }
    }

    object->ai.locator = NULL;
    return 1;
}

i32 Action_AssignLocatorInSet(AISYS_s *sys, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char **params, i32 param_count,
                              i32 first_time, float) {
    if (first_time == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    AILOCATOR *locator = NULL;
    AILOCATORSET *locator_set = NULL;
    u8 assignment = object != NULL ? object->apiobj.field_0x289 : 0xff;

    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        if (NuStrICmp(param, "locator=mylocator") == 0) {
            locator = object != NULL ? object->ai.locator : NULL;
            continue;
        }
        char *value = NuStrIStr(param, "locator");
        if (value != NULL) {
            locator = AIPathFindLocator(sys, value + NuStrLen("locator") + 1);
            continue;
        }
        value = NuStrIStr(param, "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character") + 1);
            assignment = object != NULL ? object->apiobj.field_0x289 : 0xff;
            continue;
        }
        value = NuStrIStr(param, "set");
        if (value != NULL) {
            locator_set = AIPathFindLocatorSet(sys, value + NuStrLen("set") + 1);
            continue;
        }
        if (NuStrICmp(param, "reserve") == 0) {
            assignment = 0x80;
        }
    }

    if (locator == NULL || locator_set == NULL) {
        return 1;
    }
    const i32 locator_index = locator - sys->locators;
    for (i32 index = 0; index < locator_set->locator_count; ++index) {
        if (locator_set->locator_entries[index] == locator_index) {
            locator_set->assigned[index] = assignment;
            break;
        }
    }
    return 1;
}

void Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float) {
}

namespace {
    struct GizmosAIRegistryCallbacks {
        GizmosAIRegistryCallbacks() {
            lego_aiactiondefs[LEGO_AI_ACTION_RELEASE_LOCATOR].eval_fn = Action_ReleaseLocator;
            lego_aiactiondefs[LEGO_AI_ACTION_ASSIGN_LOCATOR].eval_fn = Action_AssignLocatorInSet;
            lego_aiactiondefs[LEGO_AI_ACTION_GET_LOCATOR_FROM_SET].eval_fn = Action_GetLocatorFromSet;
        }
    };

    GizmosAIRegistryCallbacks gizmos_ai_registry_callbacks;
} // namespace

// Static GIZFLOW/FLOWBOX action callbacks (GizAction*/GizActions*). Moved from
// gizactions_stubs.cpp to satisfy the symbol baseline.

static __used__ void GizAction_SetAIState(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_HitBlowup(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_PlayForce(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_PlayRadio(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_EnableSock(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_ActivateChar(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_SetAIMessage(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_PlaySpecial(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_ActivateGizmo(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_SetVisibility(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_TurnOnFlowBox(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_ActivateBelt(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_GoToNewLevel(GIZFLOW_s *, FLOWBOX_s *, char **params, int param_count) {
    if (param_count <= 0) {
        return;
    }

    char *cutscene_name = NULL;
    LEVELDATA *level = NULL;

    for (i32 param_index = 0; param_index < param_count; ++param_index) {
        char *value = NuStrIStr(params[param_index], "level=");
        if (value != NULL) {
            level = Level_FindByName(value + NuStrLen("level="), NULL);
        } else {
            value = NuStrIStr(params[param_index], "cutscene=");
            if (value != NULL) {
                cutscene_name = value + NuStrLen("cutscene=");
            }
        }
    }

    if (FreePlay == 0 && cutscene_name != NULL && NewCutScene(NULL, WORLD->cutscene_sys, cutscene_name, 0) != NULL) {
        return;
    }

    if (level != NULL && netclient == 0) {
        GoToNewLevel(level->idx);
    }
}

static __used__ void GizActions_PlayCutscene(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_PlayObstacle(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_ActivateEffect(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_CompleteLevel(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_GoThroughDoor(GIZFLOW_s *, FLOWBOX_s *, char **params, int param_count) {
    char *door_name = NULL;

    for (i32 param_index = 0; param_index < param_count; ++param_index) {
        char *value = NuStrIStr(params[param_index], "Name");
        if (value != NULL) {
            door_name = value + NuStrLen("Name=");
        }
    }

    if (door_name != NULL) {
        DOOR_s *door = Door_FindByName(WORLD, door_name);
        if (door != NULL) {
            Door_GoThrough(WORLD, door, 1);
        }
    }
}

static __used__ void GizAction_ChangeTechnoTgt(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_ActivatePartEffect(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_SetGizmoVisibility(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizAction_SetPickupVisibility(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

static __used__ void GizActions_ChangeObstTriggerType(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
}

GIZACTIONDEFN_s game_gizactiondefs[] = {
    {"SetVisibility", GizAction_SetVisibility},
    {"SetGizmoVisibility", GizAction_SetGizmoVisibility},
    {"SetPickupVisibility", GizAction_SetPickupVisibility},
    {"ActivateGizmo", GizAction_ActivateGizmo},
    {"ActivateChar", GizAction_ActivateChar},
    {"TurnOnFlowBox", GizAction_TurnOnFlowBox},
    {"ActivateEffect", GizAction_ActivateEffect},
    {"ActivatePartEffect", GizAction_ActivatePartEffect},
    {"ChangeTechnoTarget", GizAction_ChangeTechnoTgt},
    {"SetAIMessage", GizAction_SetAIMessage},
    {"SetAIState", GizAction_SetAIState},
    {"CompleteLevel", GizActions_CompleteLevel},
    {"GoToNewLevel", GizActions_GoToNewLevel},
    {"GoThroughDoor", GizActions_GoThroughDoor},
    {"PlayObstacle", GizActions_PlayObstacle},
    {"PlaySpecial", GizActions_PlaySpecial},
    {"PlayForce", GizActions_PlayForce},
    {"ChangeObstTriggerType", GizActions_ChangeObstTriggerType},
    {"PlayRadio", GizActions_PlayRadio},
    {"PlayCutscene", GizActions_PlayCutscene},
    {"HitBlowup", GizActions_HitBlowup},
    {"ActivateBelt", GizActions_ActivateBelt},
    {"EnableSock", GizActions_EnableSock},
    {NULL, NULL},
};
