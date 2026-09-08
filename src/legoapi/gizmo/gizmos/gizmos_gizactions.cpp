#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizbuildits.h"

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

i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char **, i32, i32, float);

i32 Action_UseTriggerSet(AISYS_s *system, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **params,
                         i32 param_count, i32 first_time, f32 elapsed) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || system == NULL ||
        system->player_1 == NULL)
        return 1;
    GameObject_s *object = packet->owner->apiobj.objptr;
    if (first_time != 0 && WORLD->ai_trigger_set_sys != NULL) {
        for (i32 i = 0; i < param_count; ++i) {
            char *value = NuStrIStr(params[i], "set=");
            if (value != NULL) {
                i32 index = static_cast<i32>(AIParamToFloat(processor, value + 4)) - 1;
                if (static_cast<u32>(index) < 32) {
                    processor->action_data_3 = &WORLD->ai_trigger_set_sys->sets[index];
                }
            }
        }
    }
    AITRIGGERSET_s *set = static_cast<AITRIGGERSET_s *>(processor->action_data_3);
    if (set != NULL) {
        object->active_trigger_set = set;
        set->flags |= 2;
        Action_HelpWithTriggers(system, processor, packet, params, param_count, first_time, elapsed);
    }
    return 0;
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

struct GIZSPINNER_s;
f32 GizSpinner_GetNearestTargetPoint(GIZSPINNER_s *, NUVEC *, NUVEC *, NUVEC *, i32);
void GameObjectSetCanUse(GameObject_s *, void *, u8, u8, f32);
void ClearSpecialMove(GameObject_s *);
extern i32 spinner_gizmotype_id;
extern i32 LEGOCONTEXT_GRAPPLE;
extern u32 GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_TOGGLERIGHT;
extern f32 ai_moveradius;

i32 Action_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32,
                            f32 elapsed) {
    AITRIGGERSETSYS_s *system = WORLD->ai_trigger_set_sys;
    if (system == NULL || packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL)
        return 0;
    GameObject_s *object = packet->owner->apiobj.objptr;
    u8 object_index = packet->owner->apiobj.field_0x289;
    i32 trigger_index = system->field_0x42c0[object_index];
    if (trigger_index == -1)
        return 0;
    AITRIGGERSET_s *set = &system->sets[system->field_0x4280[object_index]];
    AITRIGGERSET_TARGET *target = &set->targets[trigger_index];
    GIZMO_s *gizmo = set->triggers[trigger_index];
    NUVEC spinner_position, spinner_direction;
    NUVEC *spinner_origin = NULL;
    if (gizmo->type_id == spinner_gizmotype_id) {
        spinner_origin = &packet->owner->apiobj.collision_position;
        GIZSPINNER_s *spinner = static_cast<GIZSPINNER_s *>(gizmo->object);
        f32 result =
            GizSpinner_GetNearestTargetPoint(spinner, spinner_origin, &spinner_position, &spinner_direction, 1);
        if (result == -1.0f)
            result =
                GizSpinner_GetNearestTargetPoint(spinner, spinner_origin, &spinner_position, &spinner_direction, 0);
        if (result != -1.0f) {
            NuVecAddScale(&spinner_position, &spinner_position, &spinner_direction, 0.5f);
            AIMoveInstruction(packet, &spinner_position, 0.0f, &target->path, 1, 0.0f);
        }
    } else if (gizmo->type_id == grapple_gizmotype_id) {
        GRAPPLE *grapple = static_cast<GRAPPLE *>(gizmo->object);
        if (object->character_context == LEGOCONTEXT_GRAPPLE && object->field_0x788 == grapple) {
            object->pad_1094[0] = 5;
            GameObjectSetCanUse(object, grapple, 1, 50, 0.0f);
            ClearSpecialMove(object);
            for (i32 i = 0; i < 2; ++i) {
                GameObject_s *other = Player[i];
                if (other == NULL || other->character_context != 0x46)
                    continue;
                for (i32 j = 0; j < set->trigger_count; ++j) {
                    if (set->triggers[j] != NULL && set->triggers[j]->object == other->field_0x788) {
                        if (player->apiobj.position.y > object->apiobj.position.y + 0.05f)
                            object->pad_107e[0] = 1;
                        else if (object->apiobj.position.y - 0.05f > player->apiobj.position.y)
                            object->pad_107e[0] = 2;
                        return 0;
                    }
                }
            }
            return 0;
        }
        GameObjectSetCanUse(object, grapple, 1, 50, 0.0f);
        AIMoveInstruction(packet, &target->position, 0.0f, &target->path, 1, 0.0f);
    } else {
        AIMoveInstruction(packet, &target->position, 0.0f, &target->path, 1, 0.0f);
    }
    gizmo = set->triggers[trigger_index];
    if (gizmo == NULL)
        return 0;
    NUVEC offset;
    if (gizmo->type_id == lever_gizmotype_id) {
        if (object->apiobj.character_model->model_data_b[0x5d] == NULL)
            goto change_character;
        LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
        if (NuVecXZDistSqr(&packet->terrain_origin, &target->position, &offset) < ai_moveradius * ai_moveradius) {
            packet->movement_look_target = &lever->position;
            object->pad_gamepad->buttons_down_08 |= GAMEPAD_SPECIAL;
        }
    } else if (gizmo->type_id == spinner_gizmotype_id) {
        if (object->apiobj.field_0x27d != 0 &&
            NuVecXZDistSqr(spinner_origin, &spinner_position, NULL) < packet->mover_height) {
            NuVecAddScale(&spinner_position, &spinner_position, &spinner_direction, -0.5f);
            AIMoveInstruction(packet, &spinner_position, 0.0f, &target->path, 1, 0.0f);
            object->field_0xf02 |= 2;
            object->apiobj.respawn_timer = 0.0f;
        }
    } else if (gizmo->type_id == force_gizmotype_id) {
        GIZFORCE_s *force = static_cast<GIZFORCE_s *>(gizmo->object);
        if (CharCategory_IsCategory(object, (force->config_flags & 0x10) != 0 ? 1 : 0) == 0)
            goto change_character;
        if (NuVecXZDistSqr(&packet->terrain_origin, &target->position, &offset) < ai_moveradius * ai_moveradius) {
            packet->movement_look_target = &force->position;
            object->pad_gamepad->allocated_5a |= 4;
            object->gizforce_target = force;
        }
    } else if (gizmo->type_id == grapple_gizmotype_id) {
        GRAPPLE *grapple = static_cast<GRAPPLE *>(gizmo->object);
        if (processor->action_data_1 != 0) {
            if (object->character_context != 0) {
                processor->action_data_1 = 1;
                return 0;
            }
            GameObjectSetCanUse(object, grapple, 1, 50, 0.0f);
            AIMoveInstruction(packet, &grapple->ground_position, 0.0f, &target->path, 7,
                              packet->movement_instruction_parameter);
            if (processor->action_data_1 != 0)
                return 0;
        }
        if (NuVecXZDistSqr(&packet->terrain_origin, &target->position, &offset) < ai_moveradius * ai_moveradius) {
            processor->action_data_1 = 1;
            GameObjectSetCanUse(object, grapple, 1, 50, 0.0f);
            object->pad_gamepad->buttons_down_08 |= GAMEPAD_JUMP;
        }
    }
    return 0;
change_character:
    if (FreePlay != 0) {
        processor->action_timer -= elapsed;
        if (processor->action_timer < 0.0f) {
            processor->action_timer = 0.5f;
            object->pad_gamepad->buttons_down_08 |= GAMEPAD_TOGGLERIGHT;
        }
    }
    return 0;
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

void ReleaseTakeOver(GameObject_s *, i32);

static __used__ void GizAction_ActivateChar(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    char *name = NULL;
    i32 activate = 1;
    GIZBUILDIT_s *buildit = NULL;
    for (i32 i = 0; i < count; ++i) {
        char *value = NuStrIStr(params[i], "name=");
        if (value != NULL) {
            name = value + 5;
        } else if (NuStrICmp(params[i], "TRUE") == 0) {
            activate = 1;
        } else if (NuStrICmp(params[i], "FALSE") == 0) {
            activate = 0;
        } else if ((value = NuStrIStr(params[i], "buildit=")) != NULL) {
            GIZMO *gizmo = GizmoFindByName(flow->gizmo_sys, gizbuildit_gizmotype_id, value + 8);
            if (gizmo != NULL && gizmo->object != NULL) {
                buildit = static_cast<GIZBUILDIT_s *>(gizmo->object);
            }
        }
    }
    if (name != NULL && activate != 0) {
        if (buildit != NULL) {
            if (ActivateCharacter(name, &buildit->position, buildit->field_0x7c) == NULL) {
                GizBuildIt_KillParts(buildit);
            }
        } else {
            ActivateCharacter(name, NULL, 0);
        }
    } else if (activate == 0 && name != NULL) {
        if (Player[0] != NULL && (Player[0]->apiobj.field_0x1f8 & 0x1000) != 0 && Player[0]->apiobj.field_0x287 == 0 &&
            Player[0]->ai.field_0x134 != 0xff) {
            AICREATURE *creature = &world->ai_sys->creatures[Player[0]->ai.field_0x134];
            if (creature != NULL && NuStrICmp(creature->name, name) == 0)
                ReleaseTakeOver(Player[0], 0);
        }
        if (Player[1] != NULL && (Player[1]->apiobj.field_0x1f8 & 0x1000) != 0 && Player[1]->apiobj.field_0x287 == 0 &&
            Player[1]->ai.field_0x134 != 0xff) {
            // The original second-player branch also reads player zero's creature index.
            AICREATURE *creature = &world->ai_sys->creatures[Player[0]->ai.field_0x134];
            if (creature != NULL && NuStrICmp(creature->name, name) == 0)
                ReleaseTakeOver(Player[1], 0);
        }
        DeactivateCharacter(name);
    }
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
