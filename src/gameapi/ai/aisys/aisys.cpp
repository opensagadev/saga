#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"

extern i32 Hub_GetRandomCharType();
extern void CurrentStart(GameObject_s *object, i32 mode, i32 start);
extern "C" void ComplexSockAngles(SOCKPOSITION *position);
extern void oneAtOnce_SetInitDistPerRow(f32 distance);
extern bool oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent);
extern f32 oneAtOnce_GetHoldRange(GameObject_s *object);
extern void Hint_CancelCurrent();
extern void ResetAICreature(GameObject_s *object, AISYS_s *system);
extern void DeactivateGameObject(GameObject_s *object);
extern void Player_ClearContext(GameObject_s *object, i32 mode);
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

i32 Action_SetState(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
i32 Action_FollowPlayer(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_GoToOriginalPath(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_BigJumpToLocator(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_UseBigJumpToJump(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_CatchUpForbidden(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_SetAnimSpeedMul(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_ShootAtOpponent(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_SetInvulnerable(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_SetVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_PressJumpButton(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_SetControlSystem(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_FollowDirection(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);
static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **, i32, i32, f32);

enum LEVEL_PROGRESS_LAYOUT : isize {
    LEVEL_PROGRESS_STRIDE = 0x2e24,
    LEVEL_PROGRESS_COMPLETION_FLAGS_OFFSET = 0x2800,
    LEVEL_PROGRESS_STORY_COMPLETE = 1 << 0,
};

enum CREATE_CREATURE_LIMITS {
    CREATE_CREATURE_MAX_LOCATORS = 64,
    CREATE_CREATURE_MAX_MODELS = 10,
};

union AI_CONDITION_LEVEL_ARGUMENT {
    void *pointer;
    isize value;
};

static isize AIConditionArgumentValue(void *argument) {
    AI_CONDITION_LEVEL_ARGUMENT condition_argument = {};
    condition_argument.pointer = argument;
    return condition_argument.value;
}

static char *ActionParamValue(char *param, const char *name) {
    if (param == NULL) {
        return NULL;
    }

    const i32 length = NuStrLen(name);
    if (NuStrNICmp(param, name, length) != 0 || param[length] != '=') {
        return NULL;
    }
    return param + length + 1;
}

static void ActionCopyParam(char *destination, i32 capacity, const char *source) {
    i32 index = 0;
    while (index + 1 < capacity && source[index] != '\0') {
        destination[index] = source[index];
        ++index;
    }
    destination[index] = '\0';
}

static GameObject_s *ActionCharacterAndToggle(AISYS *system, AIPACKET *packet, char **params, i32 param_count,
                                              bool *enabled) {
    GameObject_s *object = packet != NULL ? packet->owner : NULL;
    *enabled = true;
    for (i32 index = 0; index < param_count; ++index) {
        char *name = ActionParamValue(params[index], "character");
        if (name != NULL) {
            if (NuStrICmp(name, "myself") != 0) {
                object = GetNamedGameObject(system, name);
            }
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            *enabled = false;
        }
    }
    return object;
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

static u16 ActionAttackOverride(char *name) {
    if (NuStrICmp(name, "PUNCH_1") == 0)
        return 1;
    if (NuStrICmp(name, "PUNCH_2") == 0)
        return 2;
    if (NuStrICmp(name, "PUNCH_3") == 0)
        return 3;
    if (NuStrICmp(name, "PUNCH_BEHIND") == 0)
        return 4;
    if (NuStrICmp(name, "PUNCH_SPECIAL") == 0)
        return 5;
    if (NuStrICmp(name, "BLOCK") == 0)
        return 6;
    if (NuStrICmp(name, "SHOOT") == 0)
        return 7;
    return 0;
}

static GameObject_s *ActionPacketOpponent(AIPACKET *packet) {
    APIOBJECT *opponent = packet != NULL ? static_cast<APIOBJECT *>(packet->opponent) : NULL;
    return opponent != NULL ? opponent->objptr : NULL;
}

static void ActionApplySide(AISYS *system, GameObject_s *object, i32 side) {
    if (object == NULL) {
        return;
    }

    const u32 old_state = object->apiobj.field_0x1f4;
    u32 new_state = old_state & 0xfffefffau;
    if (side == 1) {
        new_state |= 1;
    } else if (side == 0) {
        new_state |= 4;
    } else if (side == 2) {
        new_state |= 0x10000;
    } else if (side == 3 && (old_state & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0 && object->ai.field_0x134 != 0xff &&
               system != NULL && apicharsys != NULL) {
        const i32 type = system->creatures[object->ai.field_0x134].type;
        if (type >= 0 && type < apicharsys->character_count) {
            const u32 model_flags = apicharsys->char_data[type].model_flags;
            if ((model_flags & 0x200) != 0) {
                new_state |= 4;
            } else if ((model_flags & 4) != 0) {
                new_state |= 1;
            }
        }
    }
    object->apiobj.field_0x1f4 = new_state;

    object->ai.capabilities &=
        ~(static_cast<u32>(LEGO_AIPATHCNX_FORGOODIES) | static_cast<u32>(LEGO_AIPATHCNX_FORBADDIES));
    if ((new_state & 1) != 0) {
        object->ai.capabilities |= LEGO_AIPATHCNX_FORBADDIES;
    } else if ((new_state & 4) == 0) {
        object->ai.capabilities |= LEGO_AIPATHCNX_FORGOODIES;
    }

    if ((old_state & new_state & 0x10005) == 0) {
        object->ai.field_0x1e5 &= static_cast<u8>(~8u);
        object->ai.opponent = NULL;
        *reinterpret_cast<void **>(reinterpret_cast<u8 *>(object) + 0xeac) = NULL;
        *reinterpret_cast<void **>(reinterpret_cast<u8 *>(object) + 0xecc) = NULL;
        *reinterpret_cast<void **>(reinterpret_cast<u8 *>(object) + 0xed0) = NULL;
    }
}

static bool ActionValidOpponent(GameObject_s *opponent) {
    return opponent != NULL &&
           (opponent->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
               (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
           opponent->apiobj.field_0x287 == 0 && opponent->character_context != CHARACTER_CONTEXT_DOOMED &&
           !(static_cast<i8>(opponent->apiobj.flags_low) < 0 && opponent->spawn_protection_timer > 0.0f);
}

// Game-specific AI actions and conditions (registered via
// RegisterAIScriptActions / RegisterAIScriptConditions). These are stubbed to
// satisfy the symbol baseline; the action/condition logic itself is not
// decompiled. Each stub matches the mangled symbol of the original binary.

__used__ static i32 Action_Idle(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                i32 param_5, f32 param_6) {
    (void)sys;
    if (processor == NULL) {
        return 1;
    }

    if (param_5 == 0) {
        if (processor->action_data_1 != 0) {
            --processor->action_data_1;
            return processor->action_data_1 == 0;
        }
        if (processor->action_timer <= 0.0f) {
            return 0;
        }
        processor->action_timer -= param_6;
        if (processor->action_timer > 0.0f) {
            return 0;
        }
        processor->action_timer = 0.0f;
        return 1;
    }

    f32 minimum_time = 0.0f;
    f32 maximum_time = 0.0f;
    i32 frames = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "mintime");
        if (value != NULL) {
            minimum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("mintime") + 1);
            continue;
        }
        value = NuStrIStr(params[index], "maxtime");
        if (value != NULL) {
            maximum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("maxtime") + 1);
            continue;
        }
        value = NuStrIStr(params[index], "frames");
        if (value != NULL) {
            frames = static_cast<i32>(AIParamToFloatEx(packet, processor, value + NuStrLen("frames") + 1));
            continue;
        }
        processor->action_timer = AIParamToFloatEx(packet, processor, params[index]);
    }

    if (frames != 0) {
        processor->action_data_1 = static_cast<u8>(MAX(0, MIN(frames, 255)));
    } else if (processor->action_timer == 0.0f && minimum_time < maximum_time) {
        processor->action_timer = NuRandFloat() * (maximum_time - minimum_time) + minimum_time;
    }
    return 0;
}

__used__ static i32 Action_Kill(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL ? packet->owner : NULL;
    GameObject_s *excluded = NULL;
    AIAREA *area = NULL;
    i32 creature_set = 0;
    bool all_ai = false;
    bool check_if_dead = false;
    bool debris = false;
    bool parts_on = false;
    bool respawn = false;
    bool respawn_at_origin = false;

    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character") + 1);
            continue;
        }
        if (NuStrIStr(params[index], "opponent") != NULL) {
            object = ActionPacketOpponent(packet);
            continue;
        }
        if (NuStrICmp(params[index], "respawn") == 0) {
            respawn = true;
            continue;
        }
        if (NuStrICmp(params[index], "respawn_at_origin") == 0) {
            respawn = true;
            respawn_at_origin = true;
            continue;
        }
        value = NuStrIStr(params[index], "all_ai_except");
        if (value != NULL) {
            excluded = GetNamedGameObject(sys, value + NuStrLen("all_ai_except") + 1);
            all_ai = true;
            continue;
        }
        if (NuStrICmp(params[index], "all_ai") == 0) {
            all_ai = true;
            continue;
        }
        if (NuStrICmp(params[index], "check_if_dead") == 0) {
            check_if_dead = true;
            continue;
        }
        if (NuStrICmp(params[index], "debris") == 0) {
            debris = true;
            continue;
        }
        if (NuStrICmp(params[index], "parts_on") == 0) {
            parts_on = true;
            continue;
        }
        value = ActionParamValue(params[index], "set");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value));
            creature_set = parsed_set >= 0 && parsed_set <= 16 ? parsed_set : 0;
            continue;
        }
        value = ActionParamValue(params[index], "area");
        if (value != NULL && sys != NULL) {
            area = AISysFindArea(sys, value);
        }
    }

    const auto may_kill = [check_if_dead](GameObject_s *candidate) {
        return candidate != NULL &&
               (candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
                   (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
               (!check_if_dead || (candidate->apiobj.field_0x287 == 0 && candidate->field_0x101c <= 0.0f));
    };
    const auto kill = [respawn, respawn_at_origin, debris, parts_on](GameObject_s *candidate) {
        candidate->field_0xefa = static_cast<u8>((candidate->field_0xefa & 0xcfu) | (respawn ? 0x10u : 0u) |
                                                 (respawn_at_origin ? 0x20u : 0u));
        if (parts_on) {
            KillParts(candidate, -1, -1, 1, 0.0f, 0, NULL);
        }
        KillGameObject(candidate, debris ? 2 : 4, 0);
    };

    if (all_ai || creature_set != 0 || area != NULL) {
        for (i32 index = 0; Obj != NULL && index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *candidate = &Obj[index];
            if (!may_kill(candidate) || candidate == excluded) {
                continue;
            }
            if (all_ai && (candidate->apiobj.field_0x1f4 & 0x1000u) == 0) {
                continue;
            }
            if (creature_set != 0 && candidate->ai.creature_set != creature_set) {
                continue;
            }
            if (area != NULL && sys != NULL) {
                const isize area_index = area - sys->areas;
                const u64 mask = static_cast<u64>(candidate->ai_area_mask_low) |
                                 (static_cast<u64>(candidate->ai_area_mask_high) << 32);
                if (area_index < 0 || area_index >= 64 || (mask & (1ull << area_index)) == 0) {
                    continue;
                }
            }
            kill(candidate);
        }
        return 1;
    }

    if (may_kill(object)) {
        kill(object);
    }
    return 1;
}

__used__ static i32 Action_Launch(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                  i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AddPart(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_BigJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CanTurn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_Explode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_PlaySfx(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetBoss(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefb |= 8;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xefb &= static_cast<u8>(~8u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_SetPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0 || sys == NULL || sys->path_sys == NULL) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    AIPATH *path = NULL;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value);
            continue;
        }
        if (NuStrICmp(params[index], "path=LevelPath") == 0) {
            path = sys->path_sys->active_path;
            continue;
        }
        value = ActionParamValue(params[index], "path");
        if (value != NULL) {
            for (i32 path_index = 0; path_index < sys->path_sys->path_count; ++path_index) {
                AIPATH *candidate = sys->path_sys->paths[path_index];
                if (candidate != NULL && NuStrICmp(candidate->name, value) == 0) {
                    path = candidate;
                    break;
                }
            }
        }
    }
    if (object != NULL && path != NULL) {
        AISysCharacterSetPath(&object->ai, path);
        AISysGetCharacterPathPos(WORLD != NULL ? WORLD->ai_sys : sys, &object->apiobj, &object->ai, 0xff, 1);
    }
    return 1;
}

__used__ static i32 Action_SetSide(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params, i32 param_4,
                                   i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL ? packet->owner : NULL;
    i32 side = 0;
    f32 range_squared = 0.0f;
    i32 type_ids[10];
    i32 type_count = 0;

    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "neutral") == 0) {
            side = 0;
        } else if (NuStrICmp(params[index], "goodie") == 0 || NuStrICmp(params[index], "goody") == 0) {
            side = -1;
        } else if (NuStrICmp(params[index], "baddie") == 0 || NuStrICmp(params[index], "baddy") == 0) {
            side = 1;
        } else if (NuStrICmp(params[index], "goodiebaddie") == 0 || NuStrICmp(params[index], "goodybaddy") == 0) {
            side = 2;
        } else if (NuStrICmp(params[index], "default") == 0) {
            side = 3;
        } else if (char *value = ActionParamValue(params[index], "type")) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL && type_count < 10) {
                const i32 local_type = LevelCharacterTypeIDFn(value);
                if (local_type != -1) {
                    const i32 global_type = LevelCharacterGlobalIDFn(static_cast<u8>(local_type));
                    if (global_type != -1) {
                        type_ids[type_count++] = global_type;
                    }
                }
            }
        } else if ((value = ActionParamValue(params[index], "character")) != NULL) {
            object = GetNamedGameObject(sys, value);
        } else if ((value = ActionParamValue(params[index], "range")) != NULL) {
            const f32 range = AIParamToFloat(processor, value);
            range_squared = range * range;
        }
    }

    if (type_count == 0 && range_squared <= 0.0f) {
        ActionApplySide(sys, object, side);
        return 1;
    }

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *candidate = &Obj[index];
        if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
            (candidate->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            continue;
        }

        bool type_matches = type_count == 0;
        for (i32 type_index = 0; type_index < type_count; ++type_index) {
            type_matches |= candidate->id == type_ids[type_index];
        }
        bool range_matches = range_squared <= 0.0f;
        if (!range_matches && object != NULL) {
            range_matches = NuVecDistSqr(&object->apiobj.position, &candidate->apiobj.position, NULL) < range_squared;
        }
        if (type_matches && range_matches) {
            ActionApplySide(sys, candidate, side);
        }
    }
    return 1;
}

__used__ static i32 Action_Activate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value);
            continue;
        }
        value = ActionParamValue(params[index], "set");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
        }
    }

    if (creature_set != 0) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *candidate = &Obj[index];
            if ((candidate->apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0 &&
                candidate->ai.creature_set == creature_set) {
                if (candidate->ai.field_0x134 == 0xff) {
                    candidate->apiobj.flags_high |= 0x10;
                    AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&candidate->ai),
                                                     const_cast<char *>("Active"));
                } else {
                    ResetAICreature(candidate, sys);
                }
                ++aicreature_sets_alive[creature_set - 1];
            }
        }
        return 1;
    }

    if (object != NULL) {
        if (object->ai.field_0x134 == 0xff) {
            object->apiobj.flags_high |= 0x10;
            AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai),
                                             const_cast<char *>("Active"));
        } else {
            ResetAICreature(object, sys);
        }
        if (static_cast<u32>(object->ai.creature_set - 1) < 16) {
            ++aicreature_sets_alive[object->ai.creature_set - 1];
        }
    }
    return 1;
}

__used__ static i32 Action_AddToSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (param_5 != 0 && object != NULL && param_4 > 0) {
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "Reset") == 0) {
                object->ai.creature_set = 0;
                continue;
            }
            const i32 creature_set = static_cast<i32>(AIParamToFloat(processor, params[index]));
            if (static_cast<u32>(creature_set - 1) < 16) {
                object->ai.creature_set = static_cast<u8>(creature_set);
                ++aicreature_sets_alive[creature_set - 1];
            }
        }
    }
    return 1;
}

__used__ static i32 Action_DontPush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    bool dont_push = true;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            dont_push = false;
        }
    }
    if (object != NULL) {
        object->apiobj.flags_low =
            (object->apiobj.flags_low & static_cast<u8>(~2u)) | static_cast<u8>(dont_push ? 2 : 0);
    }
    return 1;
}

__used__ static i32 Action_GoToNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
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
        if (node != NULL && node->connection_count != 0 && node->connections != NULL) {
            processor->path_info.path = packet->path_info.path;
            processor->path_info.connection = node->connections[0];
            processor->path_info.direction = 0;
            processor->path_info.flags |= 1;
            processor->path_info.dist =
                static_cast<u8>(node - packet->path_info.path->nodes) == node->connections[0]->node_indices[0] ? 0.0f
                                                                                                               : 1.0f;
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
    const f32 distance_squared = NuVecXZDistSqr(&packet->terrain_origin, &node->position, NULL);
    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return distance_squared < node->radius_squared;
}

__used__ static i32 Action_SetLayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    u32 set_layers = 0;
    u32 clear_layers = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
            continue;
        }
        value = NuStrIStr(params[index], "set_layer=");
        if (value != NULL) {
            const i32 layer = static_cast<i32>(AIParamToFloat(processor, value + NuStrLen("set_layer=")));
            if (static_cast<u32>(layer - 1) < 32) {
                set_layers |= 1u << (layer - 1);
            }
            continue;
        }
        value = NuStrIStr(params[index], "clear_layer=");
        if (value != NULL) {
            const i32 layer = static_cast<i32>(AIParamToFloat(processor, value + NuStrLen("clear_layer=")));
            if (static_cast<u32>(layer - 1) < 32) {
                clear_layers |= 1u << (layer - 1);
            }
        }
    }
    if (object != NULL) {
        u32 *layers = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(object) + 0x1058);
        *layers = (*layers | set_layers) & ~clear_layers;
    }
    return 1;
}

__used__ static i32 Action_SetParam(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)param_5;
    (void)param_6;
    if (packet == NULL || processor == NULL || processor->script == NULL) {
        return 1;
    }

    AICREATURE *creature = NULL;
    if (sys != NULL && packet->field_0x134 != 0xff) {
        creature = &sys->creatures[packet->field_0x134];
    }

    for (i32 param_index = 0; param_index + 1 < param_4; ++param_index) {
        i32 script_param = -1;
        for (i32 index = 0; index < 4; ++index) {
            char *name = processor->script->params[index].name;
            if (name != NULL && NuStrICmp(params[param_index], name) == 0) {
                script_param = index;
                break;
            }
        }
        if (script_param < 0) {
            continue;
        }

        char *value = params[++param_index];
        if (NuStrICmp(value, "default") == 0) {
            const u32 override_flag = 1u << (script_param + 1);
            processor->params[script_param] = creature != NULL && (creature->flags & override_flag) != 0
                                                  ? creature->script_params[script_param]
                                                  : processor->script->params[script_param].default_val;
            continue;
        }

        char *operand = NuStrIStr(value, "+=");
        if (operand != NULL) {
            processor->params[script_param] += AIParamToFloatEx(packet, processor, operand + NuStrLen("+="));
            continue;
        }
        operand = NuStrIStr(value, "-=");
        if (operand != NULL) {
            processor->params[script_param] -= AIParamToFloatEx(packet, processor, operand + NuStrLen("-="));
            continue;
        }
        processor->params[script_param] = AIParamToFloatEx(packet, processor, value);
    }
    return 1;
}

__used__ static i32 Action_TakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_UseForce(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                    i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AddDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_BlockPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)packet;
    (void)param_6;
    if (sys != NULL && sys->path_sys != NULL && sys->path_sys->active_path != NULL && param_5 != 0 && param_4 > 0) {
        char *from = NULL;
        char *to = NULL;
        bool both_ways = false;
        i32 blocked = 1;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "from=");
            if (value != NULL) {
                from = value + NuStrLen("from=");
                continue;
            }
            value = NuStrIStr(params[index], "to=");
            if (value != NULL) {
                to = value + NuStrLen("to=");
            } else if (NuStrICmp(params[index], "bothways") == 0) {
                both_ways = true;
            } else if (NuStrICmp(params[index], "FALSE") == 0) {
                blocked = 0;
            }
        }
        if (from != NULL && to != NULL) {
            AIPathCnxSetTemporaryBlock(sys->path_sys->active_path, from, to, blocked);
            if (both_ways) {
                AIPathCnxSetTemporaryBlock(sys->path_sys->active_path, to, from, blocked);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CanAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xef9 = static_cast<u8>((object->field_0xef9 & ~2u) | (enabled ? 2u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_CanDefend(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (param_5 != 0 && packet != NULL && packet->owner != NULL) {
        bool enabled = true;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                enabled = false;
            }
        }
        packet->owner->field_0xef8 = static_cast<u8>((packet->owner->field_0xef8 & ~2u) | (enabled ? 2u : 0u));
    }
    return 1;
}

__used__ static i32 Action_CnxHelper(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || param_4 <= 0) {
        return 1;
    }

    AIPATH *path = NULL;
    char *from = NULL;
    char *to = NULL;
    void *grapple = NULL;
    f32 jump_off_dy = 0.0f;
    bool both_ways = false;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "pathname=");
        if (value != NULL) {
            path = AISysFindPath(sys, value + NuStrLen("pathname="));
            continue;
        }
        if (NuStrIStr(params[index], "bothways") != NULL) {
            both_ways = true;
            continue;
        }
        value = NuStrIStr(params[index], "from=");
        if (value != NULL) {
            from = value + NuStrLen("from=");
            continue;
        }
        value = NuStrIStr(params[index], "to=");
        if (value != NULL) {
            to = value + NuStrLen("to=");
            continue;
        }
        value = NuStrIStr(params[index], "jump_off_dy=");
        if (value != NULL) {
            jump_off_dy = AIParamToFloat(processor, value + NuStrLen("jump_off_dy="));
            continue;
        }
        value = NuStrIStr(params[index], "grapple=");
        if (value != NULL && WORLD != NULL && WORLD->gizmo_sys != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, grapple_gizmotype_id, value + NuStrLen("grapple="));
            if (gizmo != NULL) {
                grapple = gizmo->object;
            }
        }
    }

    if (from != NULL && to != NULL) {
        i32 direction = 0;
        AIPATHCNX *connection = static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, path, from, to, &direction));
        if (connection != NULL) {
            if (both_ways) {
                direction = 0xff;
            }
            if (grapple != NULL && WORLD != NULL) {
                AIPATHCNXHELPER_s *helper = AIPathCnxHelperSys_AddHelper(WORLD->ai_path_cnx_helper_sys, connection,
                                                                         static_cast<u8>(direction), grapple, 1);
                if (helper != NULL) {
                    helper->jump_off_dy = jump_off_dy;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_DontAimAt(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xf00 |= 1;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xf00 &= static_cast<u8>(~1u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_EatVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ForcePush(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
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

__used__ static i32 Action_SetSpline(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL || param_5 == 0) {
        return 1;
    }

    NUGSPLINE *spline = NULL;
    i32 looping = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "spline=");
        if (value != NULL) {
            if (WORLD != NULL && WORLD->scene != NULL) {
                spline = NuSplineFind(WORLD->scene, value + 7);
            }
        } else if (NuStrICmp(params[index], "looping") == 0) {
            looping = 1;
        }
    }

    SPLINEPOS_s *position = reinterpret_cast<SPLINEPOS_s *>(&object->movement_spline);
    memset(position, 0, 0x20);
    if (spline != NULL) {
        InitSplinePosition(position, spline, 0.0f, looping);
    }
    return 1;
}

__used__ static i32 Action_UseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                     i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (object != NULL) {
        object->field_0xefb |= 0x20;
    }
    return 1;
}

__used__ static i32 Action_CancelHint(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    Hint_CancelCurrent();
    return 1;
}

__used__ static i32 Action_DeActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value);
            continue;
        }
        value = ActionParamValue(params[index], "set");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
        }
    }

    if (creature_set != 0) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *candidate = &Obj[index];
            if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
                candidate->ai.creature_set == creature_set) {
                DeactivateGameObject(candidate);
            }
        }
    } else if (object != NULL) {
        DeactivateGameObject(object);
    }
    return 1;
}

__used__ static i32 Action_DontAttack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL) {
        bool enabled = true;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                enabled = false;
            }
        }
        packet->owner->jump_input_flags =
            static_cast<u8>((packet->owner->jump_input_flags & ~2u) | (enabled ? 2u : 0u));
    }
    return 1;
}

__used__ static i32 Action_EnableSock(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_FaceCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_FacePlayer(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed) {
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0 && param_count > 0) {
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        for (i32 index = 0; index < param_count; ++index) {
            char *value = ActionParamValue(params[index], "mintime");
            if (value != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value);
            } else if ((value = ActionParamValue(params[index], "maxtime")) != NULL) {
                max_time = AIParamToFloatEx(packet, processor, value);
            } else {
                processor->action_timer = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
        if (processor->action_timer == 0.0f && min_time < max_time) {
            processor->action_timer = NuRandFloat() * (max_time - min_time) + min_time;
        }
    }
    if (sys != NULL && sys->player_1 != NULL) {
        packet->movement_look_target = &sys->player_1->position;
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

    if (processor->action_timer <= completion_time) {
        return 0;
    }

    processor->action_timer -= elapsed;
    return completion_time >= processor->action_timer;
}

__used__ static i32 Action_GoToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32 elapsed) {
    if (sys == NULL || packet == NULL || packet->owner == NULL || packet->path_info.path == NULL ||
        packet->path_info.connection == NULL || (packet->owner->apiobj.field_0x1f4 & 0x400) == 0 ||
        packet->field_0x134 == 0xff || packet->field_0x134 >= sys->creature_count) {
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
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = ActionParamValue(params[index], "waittime");
            if (value != NULL) {
                processor->action_timer = AIParamToFloatEx(packet, processor, value);
            } else if ((value = ActionParamValue(params[index], "mintime")) != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value);
            } else if ((value = ActionParamValue(params[index], "maxtime")) != NULL) {
                max_time = AIParamToFloatEx(packet, processor, value);
            } else if (NuStrICmp(params[index], "xz_rangecheck") == 0) {
                processor->action_data_2 = 1;
            } else if ((value = ActionParamValue(params[index], "goalrange")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, value);
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
        if (processor->action_timer == 0.0f) {
            processor->action_timer = min_time < max_time ? NuRandFloat() * (max_time - min_time) + min_time : 0.01f;
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
    const f32 distance_squared = processor->action_data_2 == 0 ? NuVecDistSqr(&packet->terrain_origin, origin, NULL)
                                                               : NuVecXZDistSqr(&packet->terrain_origin, origin, NULL);
    const f32 range = packet->movement_instruction_parameter + ai_moveradius +
                      elapsed * packet->owner->apiobj.horizontal_velocity_magnitude;
    if (distance_squared < range * range) {
        packet->movement_look_target = &processor->action_pos;
        if (processor->action_timer <= 0.0f) {
            return 1;
        }
        processor->action_timer -= elapsed;
        if (processor->action_timer < 0.0f) {
            processor->action_timer = 0.0f;
        }
    }
    return 0;
}

__used__ static i32 Action_GrabVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
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

__used__ static i32 Action_ProbeDroid(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
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
        char *value = NuStrIStr(params[param_index], "mintime=");
        if (value != NULL) {
            minimum = AIParamToFloatEx(packet, processor, value + 8);
            continue;
        }

        value = NuStrIStr(params[param_index], "maxtime=");
        if (value != NULL) {
            maximum = AIParamToFloatEx(packet, processor, value + 8);
            continue;
        }

        value = NuStrIStr(params[param_index], "time=");
        if (value != NULL) {
            exact = AIParamToFloatEx(packet, processor, value + 5);
        }
    }

    if (minimum != 0.0f || maximum != 0.0f) {
        const f32 maximum_random = NuRandFloat();
        const f32 minimum_random = NuRandFloat();
        processor->script_timer = maximum_random * maximum + (1.0f - minimum_random) * minimum;
    } else {
        processor->script_timer = exact;
    }
    return 1;
}

__used__ static i32 Action_SetLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0 || param_4 == 0) {
        return 1;
    }

    AIPACKET *target_packet = packet;
    char *locator_name = NULL;
    bool personal = false;
    bool indexed = false;
    bool nearest = false;
    i32 random_count = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "name");
        if (value != NULL) {
            locator_name = value;
        } else if (NuStrICmp(params[index], "personal") == 0) {
            personal = true;
        } else if (NuStrICmp(params[index], "indexed") == 0) {
            indexed = true;
        } else if (NuStrICmp(params[index], "nearest") == 0) {
            nearest = true;
        } else if ((value = ActionParamValue(params[index], "random")) != NULL) {
            random_count = static_cast<i32>(AIParamToFloat(processor, value));
        } else if ((value = ActionParamValue(params[index], "character")) != NULL && GetNamedAPIObjectFn != NULL) {
            APIOBJECT *target = GetNamedAPIObjectFn(sys, value);
            target_packet = target != NULL ? target->ai : NULL;
        }
    }

    AILOCATOR *locator = NULL;
    if (locator_name != NULL && nearest && packet != NULL) {
        f32 best_distance = 1.0e9f;
        char numbered_name[72];
        for (i32 index = 0;; ++index) {
            snprintf(numbered_name, sizeof(numbered_name), "%s_%d", locator_name, index);
            AILOCATOR *candidate = AIPathFindLocator(sys, numbered_name);
            if (candidate == NULL) {
                break;
            }
            const f32 distance = NuVecDistSqr(&packet->terrain_origin, &candidate->position, NULL);
            if (distance < best_distance) {
                best_distance = distance;
                locator = candidate;
            }
        }
    } else if (locator_name != NULL && packet != NULL && packet->owner != NULL) {
        char resolved_name[72];
        GameObject_s *object = packet->owner->apiobj.objptr;
        if (indexed && object->apiobj.field_0x27c != 0xff) {
            snprintf(resolved_name, sizeof(resolved_name), "%s_%d", locator_name,
                     static_cast<i8>(object->apiobj.field_0x27c));
        } else if (personal && object->apiobj.character_data != NULL && object->apiobj.character_data->file != NULL) {
            snprintf(resolved_name, sizeof(resolved_name), "%s_%s", locator_name, object->apiobj.character_data->file);
        } else if (random_count != 0) {
            snprintf(resolved_name, sizeof(resolved_name), "%s_%d", locator_name, NuRand(0) % random_count);
        } else {
            snprintf(resolved_name, sizeof(resolved_name), "%s", locator_name);
        }
        locator = AIPathFindLocator(sys, resolved_name);
    }
    if (target_packet != NULL) {
        target_packet->locator = locator;
    }
    return 1;
}

__used__ static i32 Action_SetMessage(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SpinOnSpot(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_TakeDamage(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CameraShake(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CopyMessage(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CreateRider(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
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
            char *value = ActionParamValue(params[index], "name");
            if (value != NULL) {
                processor->action_data_3 = AIPathFindLocator(sys, value);
            }
        }
    }
    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
    if (locator != NULL) {
        packet->movement_look_target = &locator->position;
    }
    return locator != NULL;
}

__used__ static i32 Action_FlatTerrain(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_GoToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_count, i32 is_first_time, f32 elapsed) {
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

        const i32 movement_mode = (processor->action_data_1 & GO_TO_LOCATOR_IGNORE_PATH) != 0
                                      ? AIPACKET_MOVEMENT_DIRECT
                                      : AIPACKET_MOVEMENT_TO_DESTINATION;
        AIMoveInstruction(packet, &locator->position, 0.0f, reinterpret_cast<AIPATHINFO *>(&locator->path),
                          movement_mode, packet->movement_instruction_parameter);

        if ((processor->action_data_1 & GO_TO_LOCATOR_FACE_OPPONENT) != 0 && packet->opponent != NULL) {
            packet->movement_look_target = &static_cast<APIOBJECT *>(packet->opponent)->position;
        }
        if ((processor->action_data_1 & GO_TO_LOCATOR_ON_GROUND) != 0 && packet->owner->apiobj.field_0x27d == 0) {
            return 0;
        }

        NUVEC distance_vector;
        const f32 distance_squared = (processor->action_data_1 & GO_TO_LOCATOR_XZ_RANGE_CHECK) != 0
                                         ? NuVecXZDistSqr(&packet->terrain_origin, &locator->position, &distance_vector)
                                         : NuVecDistSqr(&packet->terrain_origin, &locator->position, &distance_vector);
        const f32 reach_distance = packet->movement_instruction_parameter + 0.1f +
                                   elapsed * packet->owner->apiobj.horizontal_velocity_magnitude;
        if (reach_distance * reach_distance <= distance_squared) {
            if ((packet->field_0x1e6 & 0x40) != 0 &&
                (processor->action_data_1 & GO_TO_LOCATOR_MUST_REACH_DESTINATION) != 0 &&
                AIBigJumpToDestinationFn != NULL) {
                return AIBigJumpToDestinationFn(&packet->owner->apiobj, &locator->position) != 0;
            }
            return 0;
        }

        if ((processor->action_data_1 & GO_TO_LOCATOR_FACE_OPPONENT) == 0 || packet->opponent == NULL) {
            packet->movement_look_target = &processor->action_pos;
        }
        if (processor->action_timer <= 0.0f) {
            return 1;
        }
        processor->action_timer -= elapsed;
        if (processor->action_timer < 0.0f) {
            processor->action_timer = 0.0f;
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

    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(param, &packet->goal_speed_mode) != 0) {
            continue;
        }

        if (NuStrIStr(param, "name") != NULL && ++index < param_count) {
            if (use_indexed_name != 0 && packet->owner->apiobj.field_0x27c != -1) {
                sprintf(locator_name, "%s_%d", params[index], packet->owner->apiobj.field_0x27c);
            } else if (use_personal_name != 0 && packet->owner->apiobj.character_data != NULL &&
                       packet->owner->apiobj.character_data->file != NULL) {
                sprintf(locator_name, "%s_%s", params[index], packet->owner->apiobj.character_data->file);
            } else if (random_locator_count != 0) {
                sprintf(locator_name, "%s_%d", params[index], NuRand(NULL) % random_locator_count);
            } else {
                sprintf(locator_name, params[index]);
            }

            processor->action_data_3 = AIPathFindLocator(sys, locator_name);
            if (NuStrIStr(params[index - 1], "teleport") != NULL) {
                AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
                if (locator == NULL) {
                    return 1;
                }
                packet->owner->apiobj.position = locator->position;
                return 1;
            }
            continue;
        }

        if (NuStrIStr(param, "teleport") != NULL) {
            continue;
        }
        if (NuStrIStr(param, "personal") != NULL) {
            use_personal_name = 1;
            continue;
        }
        if (NuStrIStr(param, "indexed") != NULL) {
            use_indexed_name = 1;
            continue;
        }

        char *value = NuStrIStr(param, "random");
        if (value != NULL) {
            random_locator_count = AIParamToFloatEx(packet, processor, value + NuStrLen("random") + 1);
            continue;
        }
        value = NuStrIStr(param, "waittime");
        if (value != NULL) {
            processor->action_timer = AIParamToFloatEx(packet, processor, value + NuStrLen("waittime") + 1);
            continue;
        }
        value = NuStrIStr(param, "mintime");
        if (value != NULL) {
            minimum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("mintime") + 1);
            continue;
        }
        value = NuStrIStr(param, "maxtime");
        if (value != NULL) {
            maximum_time = AIParamToFloatEx(packet, processor, value + NuStrLen("maxtime") + 1);
            continue;
        }
        value = NuStrIStr(param, "goalrange");
        if (value != NULL) {
            packet->movement_instruction_parameter =
                AIParamToFloatEx(packet, processor, value + NuStrLen("goalrange") + 1);
            continue;
        }

        if (NuStrICmp(param, "on_ground") == 0) {
            processor->action_data_1 |= GO_TO_LOCATOR_ON_GROUND;
        } else if (NuStrICmp(param, "xz_rangecheck") == 0) {
            processor->action_data_1 |= GO_TO_LOCATOR_XZ_RANGE_CHECK;
        } else if (NuStrICmp(param, "face_opponent") == 0) {
            processor->action_data_1 |= GO_TO_LOCATOR_FACE_OPPONENT;
        } else if (NuStrICmp(param, "ignore_path") == 0) {
            processor->action_data_1 |= GO_TO_LOCATOR_IGNORE_PATH;
        } else if (NuStrICmp(param, "must_reach_destination") == 0) {
            processor->action_data_1 |= GO_TO_LOCATOR_MUST_REACH_DESTINATION;
        } else {
            packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, param);
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

    const i32 movement_mode = (processor->action_data_1 & GO_TO_LOCATOR_IGNORE_PATH) != 0
                                  ? AIPACKET_MOVEMENT_DIRECT
                                  : AIPACKET_MOVEMENT_TO_DESTINATION;
    AIMoveInstruction(packet, &locator->position, 0.0f, reinterpret_cast<AIPATHINFO *>(&locator->path), movement_mode,
                      packet->movement_instruction_parameter);

    processor->action_pos.x = 0.0f;
    processor->action_pos.y = 0.0f;
    processor->action_pos.z = 1.0f;
    NuVecRotateY(&processor->action_pos, &processor->action_pos, locator->flags);
    NuVecAdd(&processor->action_pos, &processor->action_pos, &locator->position);
    return 0;
}

__used__ static i32 Action_InitRowDist(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)packet;
    (void)param_6;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "Dist");
            if (value != NULL) {
                oneAtOnce_SetInitDistPerRow(AIParamToFloat(processor, value + NuStrLen("Dist") + 1));
            }
        }
    }
    return 1;
}

__used__ static i32 Action_NoIdleSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xf03 = static_cast<u8>((object->field_0xf03 & ~2u) | (enabled ? 2u : 0u));
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

__used__ static i32 Action_Respawnable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefa = static_cast<u8>((object->field_0xefa & ~0x20u) | 0x10u);
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "origin") == 0) {
                object->field_0xefa |= 0x20;
            } else if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xefa &= static_cast<u8>(~0x10u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetDontMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xefc = static_cast<u8>((object->field_0xefc & ~0x10u) | (enabled ? 0x10u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_SetOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0 || packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    GameObject_s *opponent = NULL;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "opponent=");
        if (value != NULL && NuStrICmp(value + NuStrLen("opponent="), "nearest_enemy") != 0) {
            opponent = GetNamedGameObject(sys, value + NuStrLen("opponent="));
        } else if (NuStrICmp(params[index], "last_attacker") == 0) {
            opponent = static_cast<GameObject_s *>(object->last_attacker);
        }
    }
    object->opponent = opponent;
    packet->opponent = opponent != NULL ? &opponent->apiobj : NULL;
    return 1;
}

__used__ static i32 Action_SetRunSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || processor == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner;
    GAMECHARACTERDATA *character = ActionGameCharacterData(object);
    constexpr f32 disabled_speed = 1.0e9f;
    if (param_5 != 0) {
        f32 target = disabled_speed;
        f32 multiplier = 1.0f;
        f32 minimum = 0.0f;
        f32 maximum = disabled_speed;
        bool multiply = false;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "multiply=");
            if (value != NULL) {
                multiplier = AIParamToFloat(processor, value + NuStrLen("multiply="));
                multiply = true;
            } else if (NuStrICmp(params[index], "max=default") == 0) {
                maximum = character != NULL ? character->run_speed : disabled_speed;
            } else if (NuStrICmp(params[index], "default") == 0) {
                target = character != NULL ? character->run_speed : disabled_speed;
            } else if (NuStrICmp(params[index], "clear") == 0) {
                // The disabled-speed sentinel selected above is the original clear value.
            } else if ((value = NuStrIStr(params[index], "max=")) != NULL) {
                maximum = AIParamToFloat(processor, value + NuStrLen("max="));
            } else if ((value = NuStrIStr(params[index], "min=")) != NULL) {
                minimum = AIParamToFloat(processor, value + NuStrLen("min="));
            } else if ((value = NuStrIStr(params[index], "seek=")) != NULL) {
                processor->action_data_4 = AIParamToFloat(processor, value + NuStrLen("seek="));
            } else if (NuStrIStr(params[index], "player_run_speed") != NULL) {
                GAMECHARACTERDATA *player_character = Player[0] != NULL ? ActionGameCharacterData(Player[0]) : NULL;
                target = player_character != NULL ? player_character->run_speed : disabled_speed;
            } else {
                target = AIParamToFloat(processor, params[index]);
            }
        }

        minimum = NuFabs(minimum);
        maximum = NuFabs(maximum);
        if (processor->action_data_4 == 0.0f) {
            if (multiply) {
                f32 base = object->field_0xee0;
                if (base == disabled_speed) {
                    base = character != NULL ? character->run_speed : disabled_speed;
                }
                target = base * multiplier;
                if (target < 0.0f) {
                    target = MAX(target, -maximum);
                    if (minimum < maximum) {
                        target = MIN(target, -minimum);
                    }
                } else {
                    target = MIN(target, maximum);
                    if (minimum < maximum) {
                        target = MAX(target, minimum);
                    }
                }
            }
            object->field_0xee0 = target;
            return 1;
        }

        if (multiply) {
            f32 base = object->field_0xee0 == disabled_speed ? (character != NULL ? character->run_speed : target)
                                                             : object->field_0xee0;
            target = base * multiplier;
            if (target < 0.0f) {
                target = MAX(target, -maximum);
                if (minimum < maximum) {
                    target = MIN(target, -minimum);
                }
            } else {
                target = MIN(target, maximum);
                if (minimum < maximum) {
                    target = MAX(target, minimum);
                }
            }
        }
        processor->action_data_5 = target;
    }

    if (processor->action_data_4 <= 0.0f || object->field_0xee0 >= disabled_speed) {
        return 1;
    }
    object->field_0xee0 = SeekValF(object->field_0xee0, processor->action_data_5, processor->action_data_4);
    if (NuFabs(object->field_0xee0 - processor->action_data_5) >= 0.01f) {
        return 0;
    }
    object->field_0xee0 = processor->action_data_5;
    return 1;
}

__used__ static i32 Action_SetTaggable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                       i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    bool taggable = false;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
        } else if (NuStrICmp(params[index], "FALSE") != 0 && NuStrIStr(params[index], "tag_to=") == NULL) {
            taggable = true;
        }
    }
    if (object != NULL) {
        u8 *tag_flags = reinterpret_cast<u8 *>(object) + 0x7b5;
        *tag_flags = (*tag_flags & static_cast<u8>(~2u)) | static_cast<u8>(taggable ? 2 : 0);
    }
    return 1;
}

__used__ static i32 Action_ApplyGravity(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefb &= static_cast<u8>(~0x80u);
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xefb |= 0x80;
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CanBeCarried(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            if (object->character_context == 0x3c || object->field_0xcc0 != NULL) {
                Player_ClearContext(object, 1);
            }
            object->field_0xf00 = static_cast<u8>((object->field_0xf00 & ~2u) | (enabled ? 2u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_CanOpenDoors(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL || param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    object->field_0x1050 |= 1;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            object->field_0x1050 &= ~1u;
        } else if (NuStrICmp(params[index], "TRUE") == 0) {
            object->field_0x1050 |= 1;
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

__used__ static i32 Action_CanUseWeapon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xef8 |= 8;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xef8 &= static_cast<u8>(~8u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CannotBeSeen(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefc |= 2;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xefc &= static_cast<u8>(~2u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_CannotDropIn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xefb = static_cast<u8>((object->field_0xefb & ~4u) | (enabled ? 4u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_EngageObject(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_FaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_count, i32 first_time, f32 elapsed) {
    (void)sys;
    if (packet == NULL) {
        return 1;
    }
    if (first_time != 0 && param_count > 0) {
        f32 min_time = 0.0f;
        f32 max_time = 0.0f;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = ActionParamValue(params[index], "mintime");
            if (value != NULL) {
                min_time = AIParamToFloatEx(packet, processor, value);
            } else if ((value = ActionParamValue(params[index], "maxtime")) != NULL) {
                max_time = AIParamToFloatEx(packet, processor, value);
            } else if ((value = ActionParamValue(params[index], "faceoffset")) != NULL) {
                processor->action_data_4 = AIParamToFloatEx(packet, processor, value);
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

    APIOBJECT *opponent =
        static_cast<APIOBJECT *>(processor->action_data_1 == 0 ? packet->opponent : packet->nearest_opponent);
    if (opponent != NULL && opponent->objptr != NULL) {
        if (processor->action_data_4 == 0.0f || packet->owner == NULL) {
            packet->movement_look_target = &opponent->position;
        } else {
            NUVEC direction = {opponent->position.x - packet->owner->apiobj.position.x, 0.0f,
                               packet->owner->apiobj.position.z - opponent->position.z};
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

__used__ static i32 Action_GoToNewLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;

    if (param_5 != 0 && param_4 > 0) {
        LEVELDATA *level = NULL;
        char *cutscene_name = NULL;

        i32 param_index = 0;
        do {
            char *value = NuStrIStr(params[param_index], "level=");
            if (value != NULL) {
                level = Level_FindByName(value + NuStrLen("level="), NULL);
            } else {
                value = NuStrIStr(params[param_index], "cutscene=");
                if (value != NULL) {
                    cutscene_name = value + NuStrLen("cutscene=");
                }
            }
            ++param_index;
        } while (param_4 != param_index);

        if (FreePlay == 0 && cutscene_name != NULL &&
            NewCutScene(NULL, WORLD->cutscene_sys, cutscene_name, 0) != NULL) {
            return 1;
        }

        if (level != NULL) {
            GoToNewLevel(level->idx);
        }
    }

    return 1;
}

__used__ static i32 Action_NotWithParty(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xeff = static_cast<u8>((object->field_0xeff & ~1u) | (enabled ? 1u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_RaceOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ResetContext(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetAnimation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL && param_5 != 0 &&
        param_4 == 1) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        const i32 animation = FindAnimIX(object->apiobj.character_data, params[0]);
        if (animation != -1) {
            ResetAnimPacket(&object->apiobj.anim_packet, animation);
        }
    }
    return 1;
}

__used__ static i32 Action_SetForceBack(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetHitPoints(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    bool set_maximum = true;
    i32 hit_points = -1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
            continue;
        }
        value = NuStrIStr(params[index], "messageval=");
        if (value != NULL) {
            GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, value + NuStrLen("messageval="), NULL);
            if (message != NULL) {
                hit_points = static_cast<i32>(message->value);
            }
            continue;
        }
        if (NuStrICmp(params[index], "dont_set_max") == 0) {
            set_maximum = false;
        } else if (NuStrICmp(params[index], "default") != 0) {
            hit_points = static_cast<i32>(AIParamToFloat(processor, params[index]));
        }
    }
    if (object == NULL) {
        return 1;
    }
    if (hit_points < 0) {
        GAMECHARACTERDATA *character = ActionGameCharacterData(object);
        hit_points = character != NULL ? character->hitpoints : 0;
    }
    if (set_maximum) {
        object->hitpoints = static_cast<u8>(hit_points);
    }
    object->current_hp = static_cast<u8>(hit_points);
    return 1;
}

__used__ static i32 Action_SetInterrupt(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetStateArea(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetWalkSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        packet->owner->walk_speed_override = 1.0e9f;
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            packet->owner->walk_speed_override = AIParamToFloat(processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SnapToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_TagCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_TurnOnPickup(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                        i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AddPartDebris(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CanPullLevers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CircleLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
        return 1;
    }
    if (first_time != 0) {
        processor->action_data_1 = 0;
        processor->action_data_3 = processor->locator;
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            char *value = ActionParamValue(params[index], "name");
            if (value != NULL) {
                processor->action_data_3 = AIPathFindLocator(sys, value);
            } else if (NuStrIStr(params[index], "teleport") != NULL) {
                processor->action_data_3 = AIPathFindLocator(sys, params[index] + NuStrLen("name="));
            } else if ((value = ActionParamValue(params[index], "goalrange")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, value);
            } else if (NuStrICmp(params[index], "reverse") == 0) {
                packet->field_0x1e5 ^= 2;
            } else if (NuStrICmp(params[index], "anticlockwise") == 0) {
                packet->field_0x1e5 &= static_cast<u8>(~2u);
            } else if (NuStrICmp(params[index], "clockwise") == 0) {
                packet->field_0x1e5 |= 2;
            } else {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }
    AILOCATOR *locator = static_cast<AILOCATOR *>(processor->action_data_3);
    if (locator != NULL) {
        AIMoveInstruction(packet, &locator->position, packet->movement_instruction_parameter,
                          reinterpret_cast<AIPATHINFO *>(&locator->path), AIPACKET_MOVEMENT_CIRCLE,
                          packet->movement_instruction_parameter);
    }
    return locator == NULL;
}

static u32 ParseAIPathCnxFlag(char *name) {
    u32 flag = StarWars_ParseAIPathCnxFlag(name);
    if (flag != 0) {
        return flag;
    }
    if (NuStrICmp(name, "BLOCK") == 0) {
        return 0x80000000u;
    }
    if (NuStrICmp(name, "BIGJUMP") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_BIGJUMP);
    }
    if (NuStrICmp(name, "REQUIRESPERMISSION") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_REQUIRESPERMISSION);
    }
    if (NuStrICmp(name, "NO_DESTINATION_CHECK") == 0) {
        return static_cast<u32>(LEGO_AIPATHCNX_NO_DESTINATION_CHECK);
    }
    return 0;
}

__used__ static i32 Action_CnxController(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 param_6) {
    (void)packet;
    (void)param_6;
    if (first_time == 0) {
        return 1;
    }

    AIPATH *path = NULL;
    char *from = NULL;
    char *to = NULL;
    char *target_name = NULL;
    i32 target_type = 0;
    i32 fake_animation_id = -1;
    i32 gizmo_output = 0;
    u32 on_flags = 0;
    u32 off_flags = 0;
    bool both_ways = false;
    bool check_visible = false;
    bool on_obstacle_open = false;
    bool off_obstacle_open = false;

    for (i32 index = 0; index < param_count; ++index) {
        char *param = params[index];
        char *match = NuStrIStr(param, "from=");
        if (match != NULL) {
            from = match + 5;
            continue;
        }
        match = NuStrIStr(param, "to=");
        if (match != NULL) {
            to = match + 3;
            continue;
        }
        match = NuStrIStr(param, "pathname=");
        if (match != NULL) {
            path = AISysFindPath(sys, match + 9);
            continue;
        }
        if (NuStrIStr(param, "CheckVisible") != NULL) {
            check_visible = true;
            continue;
        }
        if (NuStrIStr(param, "bothways") != NULL) {
            both_ways = true;
            continue;
        }

        match = NuStrIStr(param, "on_flag");
        if (match != NULL) {
            char *value = match + 8;
            if (NuStrIStr(param, "OBSTACLE_OPEN") != NULL) {
                on_obstacle_open = true;
            } else if (NuStrIStr(param, "OBSTACLE_CLOSED") == NULL) {
                on_flags |= ParseAIPathCnxFlag(value);
            }
            continue;
        }
        match = NuStrIStr(param, "off_flag");
        if (match != NULL) {
            char *value = match + 9;
            if (NuStrIStr(param, "OBSTACLE_OPEN") != NULL) {
                off_obstacle_open = true;
            } else if (NuStrIStr(param, "OBSTACLE_CLOSED") == NULL) {
                off_flags |= ParseAIPathCnxFlag(value);
            }
            continue;
        }

        struct TARGET_PARAM {
            const char *name;
            i32 type;
        };
        static const TARGET_PARAM target_params[] = {
            {"obj=", 0},    {"cutscene=", 1}, {"buildit=", 2},  {"gizmo=", 3}, {"flowbox=", 6},
            {"blowup=", 4}, {"force=", 7},    {"obstacle=", 8}, {"zipup=", 9},
        };
        bool found_target = false;
        for (const TARGET_PARAM &target_param : target_params) {
            match = NuStrIStr(param, const_cast<char *>(target_param.name));
            if (match != NULL) {
                target_name = match + NuStrLen(target_param.name);
                target_type = target_param.type;
                found_target = true;
                break;
            }
        }
        if (found_target) {
            continue;
        }
        match = NuStrIStr(param, "fakeanimid=");
        if (match != NULL) {
            fake_animation_id = static_cast<i32>(AIParamToFloat(processor, match + 11));
            target_type = 5;
            continue;
        }
        match = NuStrIStr(param, "gizmo_output=");
        if (match != NULL) {
            gizmo_output = static_cast<i32>(AIParamToFloat(processor, match + 13));
        }
    }

    WORLDINFO *world = WORLD;
    AIPATHCNXCONTROLLER_s *controller = AIPathCnxControllerCreate(
        world != NULL ? world->ai_path_cnx_control_sys : NULL, world != NULL ? world->ai_sys : sys, path, from, to,
        target_type, target_name, fake_animation_id, gizmo_output);
    if (controller == NULL) {
        return 1;
    }
    controller->on_flags = on_flags;
    controller->off_flags = off_flags;
    controller->flags = static_cast<u8>((controller->flags & 0xe1) | (both_ways ? 2 : 0) | (check_visible ? 4 : 0) |
                                        (on_obstacle_open ? 8 : 0) | (off_obstacle_open ? 0x10 : 0));

    for (i32 index = 0; index < param_count; ++index) {
        char *match = NuStrIStr(params[index], "on_frames");
        if (match == NULL) {
            continue;
        }
        char range[64];
        NuStrCpy(range, match + 10);
        char *separator = NuStrIStr(range, "..");
        if (separator == NULL) {
            continue;
        }
        *separator = '\0';
        char *end_text = separator + 2;
        i32 start_frame = NuStrICmp(range, "lastframe-1") == 0 ? -2
                          : NuStrICmp(range, "lastframe") == 0 ? -1
                                                               : static_cast<i32>(AIParamToFloat(processor, range));
        i32 end_frame = NuStrICmp(end_text, "lastframe-1") == 0 ? -2
                        : NuStrICmp(end_text, "lastframe") == 0 ? -1
                                                                : static_cast<i32>(AIParamToFloat(processor, end_text));
        if (end_frame != -1 && end_frame != -2 && end_frame < start_frame) {
            continue;
        }
        AIPathCnxControllerSetOnRange(controller, start_frame, end_frame);
    }
    return 1;
}

__used__ static i32 Action_CompleteLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_FaceCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_count, i32 first_time, f32 elapsed) {
    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            char *value = ActionParamValue(params[index], "character");
            if (value != NULL) {
                processor->action_data_3 = GetNamedGameObject(sys, value);
            } else {
                processor->action_timer = AIParamToFloat(processor, params[index]);
            }
        }
    }
    GameObject_s *target = static_cast<GameObject_s *>(processor->action_data_3);
    if (packet != NULL && target != NULL) {
        packet->movement_look_target = &target->apiobj.position;
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

__used__ static i32 Action_FormationMove(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    if (packet != NULL && packet->group != NULL) {
        packet->group->is_in_formation = 1;
    }
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 1;
}

__used__ static i32 Action_GizmoActivate(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_GoToLevelPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)params;
    (void)param_4;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL) {
        return 1;
    }

    if (param_5 != 0) {
        if (sys == NULL || sys->path_sys == NULL || packet->path_info.path == NULL ||
            packet->path_info.connection == NULL) {
            return 0;
        }
        if (packet->path_info.path != sys->path_sys->active_path) {
            AISysCharacterSetPath(packet, sys->path_sys->active_path);
            AISysGetCharacterPathPos(WORLD != NULL ? WORLD->ai_sys : sys, &packet->owner->apiobj, packet, 0xff, 1);
        }
        return 1;
    }

    AIPATHNODE *node = processor != NULL ? static_cast<AIPATHNODE *>(processor->action_data_3) : NULL;
    if (node == NULL) {
        return 0;
    }
    if (NuVecXZDistSqr(&packet->owner->apiobj.position, &node->position, NULL) >= node->radius_squared) {
        AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                          packet->movement_instruction_parameter);
        return 0;
    }

    memset(&packet->path_info, 0, sizeof(packet->path_info));
    if (sys != NULL && sys->path_sys != NULL) {
        AISysCharacterSetPath(packet, sys->path_sys->active_path);
        if ((node->runtime_flags & 1) != 0 && packet->path_info.path != NULL) {
            const u16 connection_index = static_cast<u16>(node->path_flags);
            if (connection_index < packet->path_info.path->connection_count) {
                AISysCharacterSetPathCnx(packet, &packet->owner->apiobj.position,
                                         &packet->path_info.path->connections[connection_index], 0);
            }
        }
    }
    return 1;
}

static i32 Action_SetMaxMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                      i32 param_count, i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }

    f32 range = 0.0f;
    u8 range_type = 1;
    bool all_non_party = false;
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "Default") == 0) {
            range = DEFAULT_MOVE_RANGE;
        } else if (NuStrICmp(params[index], "All_Non_Party") == 0) {
            all_non_party = true;
        } else if (NuStrICmp(params[index], "Locator") == 0) {
            range_type = 2;
        } else {
            range = AIParamToFloat(processor, params[index]);
        }
    }

    if (all_non_party) {
        for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
            GameObject_s *object = &Obj[index];
            if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                    (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
                (object->apiobj.field_0x1f4 & 4) == 0) {
                continue;
            }
            object->ai.movement_target_radius = range;
            object->ai.movement_event_flags = (object->ai.movement_event_flags & 0xf3u) | (range > 0.0f ? 4u : 0u);
        }
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->ai.movement_target_radius = range;
        object->ai.movement_event_flags =
            (object->ai.movement_event_flags & 0xf3u) | (range > 0.0f ? static_cast<u8>(range_type << 2) : 0u);
    }
    return 1;
}

static i32 Action_SetDefaultMovementRange(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char **params,
                                          i32 param_count, i32 first_time, f32) {
    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            DEFAULT_MOVE_RANGE = AIParamToFloat(processor, params[index]);
        }
    }
    return 1;
}

static i32 Action_SetGravityHeight(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                   i32 param_count, i32 first_time, f32) {
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object == NULL || first_time == 0) {
        return 1;
    }
    object->field_0xefb &= 0x7f;
    object->hover_height_override = 1.0e9f;

    f32 minimum = 1.0e9f;
    f32 maximum = 1.0e9f;
    for (i32 index = 0; index < param_count; ++index) {
        if (NuStrICmp(params[index], "reset") == 0) {
            continue;
        }
        char *value = NuStrIStr(params[index], "min=");
        if (value != NULL) {
            minimum = AIParamToFloat(processor, value + 4);
            continue;
        }
        value = NuStrIStr(params[index], "max=");
        if (value != NULL) {
            maximum = AIParamToFloat(processor, value + 4);
        } else {
            object->hover_height_override = AIParamToFloat(processor, params[index]);
        }
    }
    if (minimum != 1.0e9f && maximum != 1.0e9f) {
        const f32 random = NuRandFloat();
        object->hover_height_override = maximum * random + (1.0f - random) * minimum;
    }
    return 1;
}

__used__ static i32 Action_ImmuneToBolts(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0) {
        object->field_0xefa |= 8;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                object->field_0xefa &= static_cast<u8>(~8u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_KeepWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    bool keep_out = true;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            keep_out = false;
            continue;
        }
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
        }
    }
    if (object != NULL) {
        object->field_0xef8 = (object->field_0xef8 & static_cast<u8>(~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT)) |
                              static_cast<u8>(keep_out ? GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT : 0);
    }
    return 1;
}

__used__ static i32 Action_ReleaseVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ResetToOrigin(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet == NULL || sys == NULL || packet->owner == NULL) {
        return 1;
    }
    NUVEC *origin = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
    if (origin != NULL) {
        packet->owner->apiobj.position = *origin;
    } else if (packet->field_0x134 != 0xff && packet->field_0x134 < sys->creature_count) {
        packet->owner->apiobj.position = sys->creatures[packet->field_0x134].pos;
    }
    return 1;
}

__used__ static i32 Action_ReturnToState(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (processor != NULL && processor->return_to_state != NULL) {
        processor->next_state = processor->return_to_state;
        processor->return_to_state = NULL;
    }
    return 1;
}

__used__ static i32 Action_SetHoverPhase(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetLocatorSet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (param_5 != 0 && processor != NULL) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = ActionParamValue(params[index], "name");
            if (value != NULL) {
                processor->unknown_a8 = AIPathFindLocatorSet(sys, value);
            }
        }
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

__used__ static i32 Action_ShadowTerrain(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SnapToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SnapWeaponOut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_TriggerBlowUp(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_UpdateSockPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_WalkBackwards(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                         i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL) {
        return 1;
    }
    if (param_5 != 0) {
        packet->movement_instruction_parameter = 1.0f;
        for (i32 index = 0; index < param_4; ++index) {
            if (AIActionParseSpeedFn == NULL || AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) == 0) {
                packet->movement_instruction_parameter = AIParamToFloatEx(packet, processor, params[index]);
            }
        }
    }

    GameObject_s *owner = packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *opponent = static_cast<GameObject_s *>(packet->opponent);
    if (owner != NULL && opponent != NULL) {
        AIMoveInstruction(packet, &opponent->ai.last_path_position, opponent->ai.mover_height, &opponent->ai.path_info,
                          AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        owner->field_0xefd |= 0x80;
        packet->goal_speed_mode = 1; // GameAIActionParseSpeed's WALK mode.
    }
    return 0;
}

__used__ static i32 Action_AddMiscPickups(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AlertCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AlwaysBackFlip(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        object->field_0xef9 |= 0x01;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xef9 &= static_cast<u8>(~0x01u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_AnimTimeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && packet->owner->apiobj.objptr != NULL) {
        GameObject_s *object = packet->owner->apiobj.objptr;
        SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    }
    return 1;
}

__used__ static i32 Action_AttackOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || processor == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner;
    ai_fighting = 1;
    if (param_5 != 0) {
        for (i32 index = 0; index < param_4; ++index) {
            char *value = ActionSubstringValue(params[index], "goalrange");
            if (value == NULL) {
                value = ActionSubstringValue(params[index], "range");
            }
            if (value != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value);
            } else if (NuStrICmp(params[index], "jediGoodie_attack") == 0) {
                processor->action_data_2 = 1;
            } else if ((value = ActionSubstringValue(params[index], "attack_override")) != NULL) {
                processor->action_data_6 = ActionAttackOverride(value);
            }
        }
        processor->action_data_4 = 0.2f;
    }

    if (processor->action_data_6 != 0) {
        object->attack_override = static_cast<u8>(processor->action_data_6);
    }

    GameObject_s *opponent = ActionPacketOpponent(packet);
    if (!ActionValidOpponent(opponent)) {
        return 0;
    }
    object->field_0xef8 |= 0x20;

    const f32 distance_squared = NuVecDistSqr(&opponent->apiobj.position, &object->apiobj.position, NULL);
    if ((object->field_0xf01 & 0x20) != 0) {
        if (oneAtOnce_CanAttack(object, opponent)) {
            packet->movement_instruction_parameter = 0.0f;
            processor->action_data_1 &= static_cast<u8>(~2u);
        } else {
            packet->movement_instruction_parameter = oneAtOnce_GetHoldRange(object);
            processor->action_data_1 |= 2;
        }
    }

    if ((processor->action_data_1 & 2) == 0) {
        const f32 range = packet->movement_instruction_parameter + opponent->ai.mover_height + packet->mover_height;
        AIMoveInstruction(packet, &opponent->ai.last_path_position, 0.0f, &opponent->ai.path_info,
                          AIPACKET_MOVEMENT_TO_DESTINATION, range);
        packet->goal_speed_mode = 0;
    } else {
        const f32 range = packet->movement_instruction_parameter;
        if (distance_squared < (range - aitol) * (range - aitol)) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, packet->mover_height, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_RETREAT, range);
            packet->goal_speed_mode = 0;
        } else if (distance_squared <= (range + aitol) * (range + aitol)) {
            packet->movement_look_target = &opponent->apiobj.position;
        } else {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, packet->mover_height, &opponent->ai.path_info,
                              AIPACKET_MOVEMENT_TO_DESTINATION, range);
            packet->goal_speed_mode = 0;
        }
    }

    const f32 attack_range = processor->action_data_4 + opponent->ai.mover_height + object->ai.mover_height;
    if (processor->action_data_4 <= 0.0f || distance_squared < attack_range * attack_range) {
        if (object->pad_gamepad != NULL) {
            object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
        }
        if (processor->action_data_2 != 0) {
            object->field_0xef9 |= 4;
        }
    }
    return 0;
}

__used__ static i32 Action_BreakFormation(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ClearInterrupt(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CycleCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_DontRaycastLOS(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 != 0 && WORLD != NULL && WORLD->api_object_sys != NULL) {
        u8 &flags = WORLD->api_object_sys->state[0x208];
        flags |= 1;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "false") == 0) {
                flags &= static_cast<u8>(~1u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_EngageOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    if (packet == NULL || packet->owner == NULL || processor == NULL) {
        return 1;
    }

    GameObject_s *object = packet->owner;
    ai_fighting = 1;
    if (param_5 != 0) {
        processor->action_data_5 = engagefiretime;
        packet->movement_instruction_parameter = idealgoalrange;
        f32 initial_fire_fraction = NuRandFloat();
        bool explicit_fire_range = false;

        for (i32 index = 0; index < param_4; ++index) {
            char *value = ActionSubstringValue(params[index], "firerange");
            if (value != NULL) {
                processor->action_data_4 = AIParamToFloat(processor, value);
                explicit_fire_range = true;
            } else if ((value = ActionSubstringValue(params[index], "goalrange")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value);
            } else if ((value = ActionSubstringValue(params[index], "minrange")) != NULL) {
                packet->movement_instruction_parameter = AIParamToFloat(processor, value);
                processor->action_data_1 |= 2;
            } else if (NuStrIStr(params[index], "static") != NULL) {
                packet->movement_instruction_parameter = 0.0f;
            } else if (NuStrIStr(params[index], "offscreen") != NULL) {
                processor->action_data_1 |= 1;
            } else if (NuStrIStr(params[index], "no_fire_in_minicut") != NULL) {
                processor->action_data_1 |= 4;
            } else if (NuStrIStr(params[index], "circle") != NULL) {
                packet->field_0x1e5 ^= 2;
                processor->action_data_1 |= 8;
            } else if ((value = ActionSubstringValue(params[index], "fireinterval")) != NULL) {
                processor->action_data_5 = AIParamToFloat(processor, value);
            } else if ((value = ActionSubstringValue(params[index], "opponent")) != NULL) {
                processor->action_data_3 = GetNamedGameObject(sys, value);
            } else if (NuStrIStr(params[index], "instant") != NULL) {
                initial_fire_fraction = 0.0f;
            } else if ((value = ActionSubstringValue(params[index], "attack_override")) != NULL) {
                processor->action_data_6 = ActionAttackOverride(value);
            }
        }
        if (LSW1 != 0) {
            processor->action_data_1 |= 0x20;
        }
        processor->action_timer = initial_fire_fraction * processor->action_data_5;
        if (!explicit_fire_range) {
            processor->action_data_4 = packet->movement_instruction_parameter == 0.0f
                                           ? 9999.9f
                                           : packet->movement_instruction_parameter + aitol;
        }
    }

    GameObject_s *opponent = static_cast<GameObject_s *>(processor->action_data_3);
    if (!ActionValidOpponent(opponent)) {
        opponent = ActionPacketOpponent(packet);
    }
    if (!ActionValidOpponent(opponent)) {
        return 0;
    }
    if (processor->action_data_6 != 0) {
        object->attack_override = static_cast<u8>(processor->action_data_6);
    }
    object->field_0xef8 |= 0x20;

    const f32 distance_squared = NuVecDistSqr(&opponent->apiobj.position, &object->apiobj.position, NULL);
    f32 goal_range = packet->movement_instruction_parameter;
    if ((object->field_0xf01 & 0x20) != 0) {
        if (oneAtOnce_CanAttack(object, opponent)) {
            goal_range = 0.0f;
            processor->action_data_1 &= static_cast<u8>(~2u);
        } else {
            goal_range = oneAtOnce_GetHoldRange(object);
            processor->action_data_1 |= 2;
        }
    }

    if (goal_range == 0.0f) {
        packet->movement_look_target = &opponent->apiobj.position;
    } else {
        const f32 near_range = MAX(0.0f, goal_range - aitol);
        const f32 far_range = goal_range + aitol;
        if (distance_squared < near_range * near_range) {
            processor->action_data_1 |= 0x10;
        } else if (distance_squared > far_range * far_range) {
            processor->action_data_1 &= static_cast<u8>(~0x10u);
        }

        if ((processor->action_data_1 & 0x10) != 0) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, MIN(goal_range, opponent->ai.mover_height),
                              &opponent->ai.path_info, AIPACKET_MOVEMENT_RETREAT, goal_range);
            packet->goal_speed_mode = 1;
            object->field_0xefd |= 0x80;
        } else if (distance_squared > far_range * far_range && (processor->action_data_1 & 2) == 0) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, MIN(goal_range, opponent->ai.mover_height),
                              &opponent->ai.path_info, AIPACKET_MOVEMENT_TO_DESTINATION, goal_range);
            packet->goal_speed_mode = 0;
        } else if ((processor->action_data_1 & 8) != 0) {
            AIMoveInstruction(packet, &opponent->ai.last_path_position, MIN(goal_range, opponent->ai.mover_height),
                              &opponent->ai.path_info, AIPACKET_MOVEMENT_CIRCLE, goal_range);
            packet->goal_speed_mode = 0;
        } else {
            packet->movement_look_target = &opponent->apiobj.position;
        }
    }

    const bool may_fire_offscreen = object->apiobj.model_draw_result != 0 || (processor->action_data_1 & 1) != 0;
    const bool minicut_allows_fire = MiniCutCam == 0 || (processor->action_data_1 & 4) == 0;
    if (may_fire_offscreen && minicut_allows_fire &&
        distance_squared < processor->action_data_4 * processor->action_data_4) {
        packet->movement_look_target = &opponent->apiobj.position;
        *reinterpret_cast<GameObject_s **>(reinterpret_cast<u8 *>(object) + 0xeac) = opponent;
        processor->action_timer -= param_6;
        if (processor->action_timer <= 0.0f) {
            processor->action_timer = (2.0f + NuRandFloat()) * processor->action_data_5;
            if (oneAtOnce_CanAttack(object, opponent) && object->pad_gamepad != NULL) {
                object->pad_gamepad->buttons_pressed |= GAMEPAD_ACTION;
            }
        }
    }
    return 0;
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

__used__ static i32 Action_ForceLightning(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_GoToNodeRandom(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 elapsed) {
    (void)elapsed;
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL ||
        packet->path_info.path == NULL || packet->path_info.connection == NULL) {
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
        if (node != NULL && node->connection_count != 0 && node->connections != NULL) {
            processor->path_info.path = packet->path_info.path;
            processor->path_info.connection = node->connections[0];
            processor->path_info.direction = 0;
            processor->path_info.flags |= 1;
            processor->path_info.dist =
                static_cast<u8>(node - packet->path_info.path->nodes) == node->connections[0]->node_indices[0] ? 0.0f
                                                                                                               : 1.0f;
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
    const f32 distance_squared = NuVecXZDistSqr(&packet->terrain_origin, &node->position, NULL);
    AIMoveInstruction(packet, &node->position, 0.0f, &processor->path_info, AIPACKET_MOVEMENT_TO_DESTINATION,
                      packet->movement_instruction_parameter);
    return distance_squared < node->radius_squared;
}

__used__ static i32 Action_LetGoOfBalloon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_PlayGizSpecial(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_PrefersPlayers(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_5;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL) {
        object->field_0xefb |= 0x40;
        for (i32 index = 0; index < param_4; ++index) {
            if (NuStrICmp(params[index], "FALSE") == 0) {
                object->field_0xefb &= static_cast<u8>(~0x40u);
            }
        }
    }
    return 1;
}

__used__ static i32 Action_PressTagButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetCanTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetPathCnxFlag(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_count, i32 first_time, f32 param_6) {
    (void)processor;
    (void)packet;
    (void)param_6;
    if (sys == NULL || sys->path_sys == NULL || sys->path_sys->path_count == 0 || first_time == 0 || param_count < 1) {
        return 1;
    }

    char *from = NULL;
    char *to = NULL;
    u32 add_flags = 0;
    u32 remove_flags = 0;
    bool set = true;
    bool both_ways = false;
    for (i32 index = 0; index < param_count; ++index) {
        char *match = NuStrIStr(params[index], "from=");
        if (match != NULL) {
            from = match + 5;
            continue;
        }
        match = NuStrIStr(params[index], "to=");
        if (match != NULL) {
            to = match + 3;
            continue;
        }
        const u32 flag = ParseAIPathCnxFlag(params[index]);
        if (flag != 0) {
            add_flags |= flag;
            if (flag == static_cast<u32>(LEGO_AIPATHCNX_JUMP_NOW)) {
                remove_flags |= static_cast<u32>(LEGO_AIPATHCNX_DONT_JUMP_NOW);
            } else if (flag == static_cast<u32>(LEGO_AIPATHCNX_DONT_JUMP_NOW)) {
                remove_flags |= static_cast<u32>(LEGO_AIPATHCNX_JUMP_NOW);
            } else if (flag == 0x20000000u) {
                both_ways = true;
            }
            continue;
        }
        if (NuStrICmp(params[index], "bothways") == 0) {
            both_ways = true;
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            set = false;
        }
    }

    if (from != NULL && to != NULL) {
        i32 direction = 0;
        AIPATHCNX *connection =
            static_cast<AIPATHCNX *>(AIPAthFindPathCnx(sys, sys->path_sys->active_path, from, to, &direction));
        if (connection != NULL) {
            if (set) {
                connection->traversal_flags[direction] =
                    (connection->traversal_flags[direction] | add_flags) & ~remove_flags;
                if (both_ways) {
                    connection->traversal_flags[direction ^ 1] =
                        (connection->traversal_flags[direction ^ 1] | add_flags) & ~remove_flags;
                }
            } else {
                connection->traversal_flags[direction] &= ~add_flags;
                if (both_ways) {
                    connection->traversal_flags[direction ^ 1] &= ~add_flags;
                }
            }
        }
    }
    return 1;
}

__used__ static i32 Action_SetScriptParam(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetScriptState(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0 || param_4 < 1) {
        return 1;
    }

    APIOBJECT *target = packet != NULL && packet->owner != NULL ? &packet->owner->apiobj : NULL;
    char *state_name = NULL;
    i32 creature_set = 0;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "character");
        if (value != NULL) {
            target = GetNamedAPIObjectFn != NULL ? GetNamedAPIObjectFn(sys, value) : NULL;
            continue;
        }
        value = ActionParamValue(params[index], "set");
        if (value != NULL) {
            const i32 parsed_set = static_cast<i32>(AIParamToFloat(processor, value));
            creature_set = static_cast<u32>(parsed_set) < 17 ? parsed_set : 0;
            continue;
        }
        value = ActionParamValue(params[index], "state");
        if (value != NULL) {
            state_name = value;
        }
    }
    if (state_name == NULL) {
        return 1;
    }

    const auto set_state = [sys, state_name](APIOBJECT *object) {
        if (object == NULL || object->ai == NULL) {
            return;
        }
        AISCRIPTPROCESS *target_processor = reinterpret_cast<AISCRIPTPROCESS *>(object->ai);
        if (target_processor->base_script == NULL) {
            return;
        }
        AISTATE *state = AIStateFind(state_name, target_processor->base_script);
        if (state == NULL) {
            return;
        }
        target_processor->active_ref_count = 0;
        AIScriptProcessorInit(sys, object->ai, target_processor, NULL, NULL, NULL, 0, target_processor->base_script,
                              state);
    };

    if (creature_set == 0) {
        set_state(target);
        return 1;
    }
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) ==
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) &&
            object->ai.creature_set == creature_set) {
            set_state(&object->apiobj);
        }
    }
    return 1;
}

__used__ static i32 Action_SnapToPosition(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ThrowDetonator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                          i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_AddGameMsgCount(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CreateCreatures(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (param_5 == 0 || sys == NULL) {
        return 1;
    }

    AILOCATOR *locators[CREATE_CREATURE_MAX_LOCATORS];
    i32 locator_count = 0;
    i16 models[CREATE_CREATURE_MAX_MODELS];
    i32 model_count = 0;
    AILOCATORSET *locator_set = NULL;
    char script_name[64] = "default";
    char state_name[64] = "";
    f32 x_offset = 0.0f;
    f32 y_offset = 0.0f;
    f32 z_offset = 0.0f;
    i32 creature_set = 0;
    i32 set_on_surface = 1;

    for (i32 param_index = 0; param_index < param_4; ++param_index) {
        char *param = params[param_index];
        char *value = ActionParamValue(param, "locator_set");
        if (value != NULL) {
            locator_set = AIPathFindLocatorSet(sys, value);
            if (locator_set == NULL) {
                continue;
            }

            AILocatorSet_CheckLocatorsStillAssigned(sys, locator_set);
            for (i32 index = 0; index < locator_set->locator_count && locator_count < CREATE_CREATURE_MAX_LOCATORS;
                 ++index) {
                if (locator_set->assigned[index] != 0xff) {
                    continue;
                }

                const u8 locator_index = locator_set->locator_entries[index];
                if (locator_index < sys->locator_count) {
                    locators[locator_count++] = &sys->locators[locator_index];
                }
            }
            continue;
        }

        value = ActionParamValue(param, "locator");
        if (value != NULL) {
            for (i32 index = 0; index < sys->locator_count && locator_count < CREATE_CREATURE_MAX_LOCATORS; ++index) {
                if (NuStrICmp(sys->locators[index].name, value) == 0) {
                    locators[locator_count++] = &sys->locators[index];
                    break;
                }
            }
            continue;
        }

        value = ActionParamValue(param, "type");
        if (value != NULL && model_count < CREATE_CREATURE_MAX_MODELS) {
            i16 model = -1;
            if (NuStrICmp(value, "RandomMap") == 0) {
                model = Hub_GetRandomCharType();
            } else if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                if (WORLD->current_level == HUB_LDATA) {
                    i16 *hub_character = NULL;
                    if (NuStrICmp(value, "Barman") == 0) {
                        hub_character = &id_BARMAN;
                    } else if (NuStrICmp(value, "JABBA") == 0) {
                        hub_character = &id_JABBA;
                    } else if (NuStrICmp(value, "CANTINABAND") == 0) {
                        hub_character = &id_CANTINABAND;
                    }

                    if (hub_character != NULL && apicharsys->playermodelids[*hub_character] != -1) {
                        model = *hub_character;
                    }
                } else {
                    const i32 level_character = LevelCharacterTypeIDFn(value);
                    if (level_character != -1) {
                        model = LevelCharacterGlobalIDFn(static_cast<u8>(level_character));
                    }
                }
            }
            if (model != -1) {
                models[model_count++] = model;
            }
            continue;
        }

        value = ActionParamValue(param, "script");
        if (value != NULL) {
            ActionCopyParam(script_name, sizeof(script_name), value);
            continue;
        }
        value = ActionParamValue(param, "state");
        if (value != NULL) {
            ActionCopyParam(state_name, sizeof(state_name), value);
            continue;
        }
        value = ActionParamValue(param, "xoffset");
        if (value != NULL) {
            x_offset = AIParamToFloat(processor, value);
            continue;
        }
        value = ActionParamValue(param, "yoffset");
        if (value != NULL) {
            y_offset = AIParamToFloat(processor, value);
            continue;
        }
        value = ActionParamValue(param, "zoffset");
        if (value != NULL) {
            z_offset = AIParamToFloat(processor, value);
            continue;
        }
        value = ActionParamValue(param, "addtoset");
        if (value != NULL) {
            creature_set = NuStrICmp(value, "myset") == 0 && packet != NULL ? packet->creature_set : NuAToI(value);
            continue;
        }
        if (NuStrICmp(param, "dont_set_on_surface") == 0) {
            set_on_surface = 0;
        }
    }

    if (locator_count == 0 || model_count == 0) {
        return 1;
    }

    const i32 locator_choice = qrand() / (0xffff / locator_count + 1);
    const i32 model_choice = qrand() / (0xffff / model_count + 1);
    AILOCATOR *locator = locators[locator_choice];
    NUVEC position = locator->position;
    position.x += x_offset;
    position.y += y_offset;
    position.z += z_offset;

    GameObject_s *object = AddDynamicCreature(models[model_choice], &position, locator->flags, script_name,
                                              reinterpret_cast<AIPATHINFO *>(&locator->path), NULL, set_on_surface,
                                              NULL, NULL, 0, creature_set);
    if (object == NULL) {
        return 1;
    }

    if (state_name[0] != '\0') {
        AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai), state_name);
    }
    object->ai.locator = locator;
    object->ai.locator_set = locator_set;

    if (locator_set != NULL) {
        const i32 selected_index = static_cast<i32>(locator - sys->locators);
        for (i32 index = 0; index < locator_set->locator_count; ++index) {
            if (locator_set->locator_entries[index] == selected_index) {
                locator_set->assigned[index] = object->apiobj.field_0x289;
                break;
            }
        }
    }
    return 1;
}

static i32 Action_MoveAwayFromLastAttacker(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_count, i32 first_time, f32) {
    if (packet == NULL || packet->owner == NULL || packet->owner->apiobj.objptr == NULL) {
        return 1;
    }

    if (first_time != 0) {
        for (i32 index = 0; index < param_count; ++index) {
            if (AIActionParseSpeedFn != NULL && AIActionParseSpeedFn(params[index], &packet->goal_speed_mode) != 0) {
                continue;
            }
            if (NuStrICmp(params[index], "face") == 0) {
                processor->action_data_1 = 1;
            } else {
                packet->movement_instruction_parameter = AIParamToFloat(processor, params[index]);
            }
        }
    }

    GameObject_s *object = packet->owner->apiobj.objptr;
    GameObject_s *attacker = static_cast<GameObject_s *>(object->last_attacker);
    if (attacker != NULL) {
        AIMoveInstruction(packet, &attacker->ai.last_path_position, attacker->ai.movement_stopping_distance,
                          &attacker->ai.path_info, AIPACKET_MOVEMENT_RETREAT, packet->movement_instruction_parameter);
        if (processor->action_data_1 != 0) {
            packet->movement_look_target = &attacker->apiobj.position;
        }
    }
    return 0;
}

extern "C" {
    // Keep this registry in the exact order used by the shipped script parser.
    AIACTIONDEF lego_aiactiondefs[] = {
        {"Activate", Action_Activate, 1, 0, 0},
        {"DeActivate", Action_DeActivate, 1, 0, 0},
        {"GoToLevelPath", Action_GoToLevelPath, 0, 0, 0},
        {"SetPath", Action_SetPath, 0, 0, 0},
        {"GoToOriginalPath", Action_GoToOriginalPath, 0, 0, 0},
        {"SnapToLocator", Action_SnapToLocator, 1, 0, 0},
        {"SetLocator", Action_SetLocator, 1, 0, 0},
        {"SetLocatorSet", Action_SetLocatorSet, 0, 0, 0},
        {"SnapToOrigin", Action_SnapToOrigin, 1, 0, 0},
        {"BigJumpToLocator", Action_BigJumpToLocator, 0, 0, 0},
        {"BigJump", Action_BigJump, 0, 0, 0},
        {"SetDoomedEscapeLocator", NULL, 0, 0, 0},
        {"SnapToPosition", Action_SnapToPosition, 1, 0, 0},
        {"SnapToSockPosition", NULL, 1, 0, 0},
        {"SetAnimation", Action_SetAnimation, 0, 0, 0},
        {"AnimTimeRandom", Action_AnimTimeRandom, 0, 0, 0},
        {"CanOpenDoors", Action_CanOpenDoors, 0, 0, 0},
        {"CanShootOffScreen", NULL, 0, 0, 0},
        {"KeepWeaponOut", Action_KeepWeaponOut, 0, 0, 0},
        {"SnapWeaponOut", Action_SnapWeaponOut, 1, 0, 0},
        {"ResetContext", Action_ResetContext, 0, 0, 0},
        {"PrefersPlayers", Action_PrefersPlayers, 0, 0, 0},
        {"SetBoltsDontGetDeflectedBack", NULL, 0, 0, 0},
        {"CanShootObstructions", NULL, 0, 0, 0},
        {"UseBigJumpToJump", Action_UseBigJumpToJump, 0, 0, 0},
        {"SetTaggable", Action_SetTaggable, 1, 0, 0},
        {"CatchUpForbidden", Action_CatchUpForbidden, 0, 0, 0},
        {"CannotDropIn", Action_CannotDropIn, 1, 0, 0},
        {"CanAttack", Action_CanAttack, 0, 0, 0},
        {"NotWithParty", Action_NotWithParty, 1, 0, 0},
        {"TakeDamage", Action_TakeDamage, 0, 0, 0},
        {"TagCharacter", Action_TagCharacter, 1, 0, 0},
        {"CanHitForceObjects", NULL, 0, 0, 0},
        {"AlwaysBackFlip", Action_AlwaysBackFlip, 0, 0, 0},
        {"PlayerSpeederHack", NULL, 0, 0, 0},
        {"SetAnimSpeedMul", Action_SetAnimSpeedMul, 0, 0, 0},
        {"SetSide", Action_SetSide, 1, 0, 0},
        {"SetStateArea", Action_SetStateArea, 0, 0, 0},
        {"SetOpponent", Action_SetOpponent, 0, 0, 0},
        {"AttackOpponent", Action_AttackOpponent, 0, 0, 0},
        {"EngageOpponent", Action_EngageOpponent, 0, 0, 0},
        {"ShootAtOpponent", Action_ShootAtOpponent, 0, 0, 0},
        {"EngageObject", Action_EngageObject, 0, 0, 0},
        {"GrabVictim", Action_GrabVictim, 0, 0, 0},
        {"EatVictim", Action_EatVictim, 0, 0, 0},
        {"ReleaseVictim", Action_ReleaseVictim, 0, 0, 0},
        {"CanDefend", Action_CanDefend, 0, 0, 0},
        {"CharClipToBlobShadows", NULL, 0, 0, 0},
        {"DontAimAt", Action_DontAimAt, 0, 0, 0},
        {"CanUseWeapon", Action_CanUseWeapon, 0, 0, 0},
        {"SetBoss", Action_SetBoss, 0, 0, 0},
        {"UpdateSockPos", Action_UpdateSockPos, 0, 0, 0},
        {"UseForce", Action_UseForce, 0, 0, 0},
        {"TriggerBlowUp", Action_TriggerBlowUp, 0, 0, 0},
        {"ForcePush", Action_ForcePush, 0, 0, 0},
        {"DeflectPlayersPart", NULL, 0, 0, 0},
        {"Kill", Action_Kill, 1, 0, 0},
        {"Explode", Action_Explode, 0, 0, 0},
        {"SetScriptState", Action_SetScriptState, 0, 0, 0},
        {"SetAIOverrideControl", NULL, 0, 0, 0},
        {"SetLastSafePathPos", NULL, 0, 0, 0},
        {"SetDontMove", Action_SetDontMove, 0, 0, 0},
        {"DontSetStoppedFlag", NULL, 0, 0, 0},
        {"PressSpecialButton", NULL, 0, 0, 0},
        {"PressTagButton", Action_PressTagButton, 0, 0, 0},
        {"PressActionButton", NULL, 0, 0, 0},
        {"UseWeapon", Action_UseWeapon, 0, 0, 0},
        {"SetInvulnerable", Action_SetInvulnerable, 0, 0, 0},
        {"DontPush", Action_DontPush, 0, 0, 0},
        {"DontAvoidCharacter", NULL, 0, 0, 0},
        {"PressJumpButton", Action_PressJumpButton, 0, 0, 0},
        {"AddToSet", Action_AddToSet, 0, 0, 0},
        {"SetSpline", Action_SetSpline, 0, 0, 0},
        {"SetControlSystem", Action_SetControlSystem, 0, 0, 0},
        {"SetZeroAcceleration", NULL, 0, 0, 0},
        {"FollowDirection", Action_FollowDirection, 0, 0, 0},
        {"BreakFormation", Action_BreakFormation, 0, 0, 0},
        {"FormationMove", Action_FormationMove, 0, 0, 0},
        {"CreateCreatures", Action_CreateCreatures, 0, 0, 0},
        {"SelectRandomSpline", NULL, 0, 0, 0},
        {"CreateSplineCreatures", NULL, 0, 0, 0},
        {"Launch", Action_Launch, 0, 0, 0},
        {"SetCurrentSpeed", NULL, 0, 0, 0},
        {"SetRunSpeed", Action_SetRunSpeed, 0, 0, 0},
        {"SetWalkSpeed", Action_SetWalkSpeed, 0, 0, 0},
        {"SetHitPoints", Action_SetHitPoints, 1, 0, 0},
        {"SetShieldHitPoints", NULL, 1, 0, 0},
        {"SetMessage", Action_SetMessage, 1, 0, 0},
        {"CopyMessage", Action_CopyMessage, 0, 0, 0},
        {"SetScriptParam", Action_SetScriptParam, 0, 0, 0},
        {"AddPart", Action_AddPart, 0, 0, 0},
        {"AddPartDebris", Action_AddPartDebris, 0, 0, 0},
        {"LaunchGuidedMissile", NULL, 0, 0, 0},
        {"SetHoverPhase", Action_SetHoverPhase, 0, 0, 0},
        {"UseCurrentSpeed", NULL, 0, 0, 0},
        {"SetMaxMovementRange", NULL, 0, 0, 0},
        {"SetDefaultMovementRange", NULL, 0, 0, 0},
        {"SetGravityHeight", NULL, 0, 0, 0},
        {"ApplyGravity", Action_ApplyGravity, 0, 0, 0},
        {"IgnoreShoveSystem", NULL, 0, 0, 0},
        {"CannotBeSeen", Action_CannotBeSeen, 0, 0, 0},
        {"CannotBeForcedBack", NULL, 0, 0, 0},
        {"CanTurn", Action_CanTurn, 0, 0, 0},
        {"NoIdleSpeed", Action_NoIdleSpeed, 0, 0, 0},
        {"SetVisibility", Action_SetVisibility, 1, 0, 0},
        {"EnableSock", Action_EnableSock, 1, 0, 0},
        {"AddDebris", Action_AddDebris, 0, 0, 0},
        {"JudderGameCamera", NULL, 0, 0, 0},
        {"CameraShake", Action_CameraShake, 0, 0, 0},
        {"ResetGameCamera", NULL, 1, 0, 0},
        {"PlayCutScene", NULL, 1, 0, 0},
        {"SetLevelPath", Action_SetLevelPath, 0, 0, 0},
        {"ImmuneToKillTerrain", NULL, 0, 0, 0},
        {"ImmuneToBolts", Action_ImmuneToBolts, 0, 0, 0},
        {"Respawnable", Action_Respawnable, 0, 0, 0},
        {"SetPathCnxFlag", Action_SetPathCnxFlag, 1, 0, 0},
        {"SetHint", Action_SetHint, 0, 0, 0},
        {"SetHintComplete", NULL, 0, 0, 0},
        {"CancelHint", Action_CancelHint, 0, 0, 0},
        {"CycleCharacter", Action_CycleCharacter, 0, 0, 0},
        {"CnxController", Action_CnxController, 0, 0, 0},
        {"CnxHelper", Action_CnxHelper, 0, 0, 0},
        {"PlaySfx", Action_PlaySfx, 0, 0, 0},
        {"CameraCut", NULL, 1, 0, 0},
        {"DynamicCameraCut", NULL, 1, 0, 0},
        {"EndCameraCut", NULL, 1, 0, 0},
        {"DontRaycastLOS", Action_DontRaycastLOS, 0, 0, 0},
        {"SetForceBack", Action_SetForceBack, 1, 0, 0},
        {"FaceCamera", Action_FaceCamera, 0, 0, 0},
        {"FaceCharacter", Action_FaceCharacter, 0, 0, 0},
        {"SpinOnSpot", Action_SpinOnSpot, 0, 0, 0},
        {"FollowCharacter", NULL, 0, 0, 0},
        {"FollowPlayer", NULL, 0, 0, 0},
        {"MoveForward", NULL, 0, 0, 0},
        {"SetFormationCommander", NULL, 0, 0, 0},
        {"RemoveThrownForceObjects", NULL, 0, 0, 0},
        {"AlwaysTriggerObstacle", NULL, 0, 0, 0},
        {"CanTriggerObstacle", NULL, 0, 0, 0},
        {"PlayGizObstacle", NULL, 1, 0, 0},
        {"PlayObstacle", NULL, 1, 0, 0},
        {"PlayGizSpecial", Action_PlayGizSpecial, 1, 0, 0},
        {"SetObstacleToEnd", NULL, 1, 0, 0},
        {"HelpWithTriggers", NULL, 0, 0, 0},
        {"UseTriggerSet", NULL, 0, 0, 0},
        {"PullLever", NULL, 0, 0, 0},
        {"UsePanel", NULL, 0, 0, 0},
        {"UseTechno", NULL, 0, 0, 0},
        {"ReleaseLocator", NULL, 0, 0, 0},
        {"AssignLocator", NULL, 0, 0, 0},
        {"GetLocatorFromSet", NULL, 0, 0, 0},
        {"MoveAwayFromLastAttacker", NULL, 0, 0, 0},
        {"ProbeDroid", Action_ProbeDroid, 0, 0, 0},
        {"AlertCreatures", Action_AlertCreatures, 0, 0, 0},
        {"SetLastAttacker", NULL, 0, 0, 0},
        {"LinkTurretToController", NULL, 0, 0, 0},
        {"TakeOver", Action_TakeOver, 1, 0, 0},
        {"ReleaseTakeOver", NULL, 1, 0, 0},
        {"RegisterTakeOverObject", NULL, 0, 0, 0},
        {"SetTakeOverTarget", NULL, 0, 0, 0},
        {"ClearTakeOverTarget", NULL, 0, 0, 0},
        {"AddGameMsgCount", Action_AddGameMsgCount, 1, 0, 0},
        {"AddMiscPickups", Action_AddMiscPickups, 0, 0, 0},
        {"SetCanTakeOver", Action_SetCanTakeOver, 0, 0, 0},
        {"CanBeCarried", Action_CanBeCarried, 1, 0, 0},
        {"IgnoreLastSafePathPos", NULL, 0, 0, 0},
        {"AwkwardShapeOverride", NULL, 0, 0, 0},
        {"IgnoreSlideTerrain", NULL, 0, 0, 0},
        {"SplineFollowTerrain", NULL, 0, 0, 0},
        {"SetLayer", Action_SetLayer, 0, 0, 0},
        {"CreateRider", Action_CreateRider, 0, 0, 0},
        {"AddTorpedoPacket", NULL, 1, 0, 0},
        {"SpeederBeingChased", NULL, 0, 0, 0},
        {"ThrowDetonator", Action_ThrowDetonator, 0, 0, 0},
        {"SetScaleOverride", NULL, 0, 0, 0},
        {"DisableNarrowSocks", NULL, 1, 0, 0},
        {"UseTimeBasedUpdate", NULL, 0, 0, 0},
        {"ForceLightning", Action_ForceLightning, 0, 0, 0},
        {"WalkBackwards", Action_WalkBackwards, 0, 0, 0},
        {"AddScriptProcessor", NULL, 0, 0, 0},
        {"SetUseOneAtOnce", NULL, 0, 0, 0},
        {"SetAO_MaxAttackers", NULL, 0, 0, 0},
        {"SetAO_AttackersPerRow", NULL, 0, 0, 0},
        {"SetAO_RowDist", NULL, 0, 0, 0},
        {"SetAO_InitRowDist", Action_InitRowDist, 0, 0, 0},
        {"SetTechnoComplete", NULL, 0, 0, 0},
        {"LetGoOfBalloon", Action_LetGoOfBalloon, 0, 0, 0},
        {"DrawBossHitPoints", NULL, 1, 0, 0},
        {"CompleteLevel", Action_CompleteLevel, 1, 0, 0},
        {"GoToNewLevel", Action_GoToNewLevel, 0, 0, 0},
        {"CircleLocator", Action_CircleLocator, 0, 0, 0},
        {"GizmoActivate", Action_GizmoActivate, 0, 0, 0},
        {"GizmoSetVisibility", NULL, 0, 0, 0},
        {"TurnOnPickup", Action_TurnOnPickup, 0, 0, 0},
        {"CanHelpWithTriggers", NULL, 0, 0, 0},
        {"CanCollideWithObjects", NULL, 0, 0, 0},
        {"SetShootOpponents", NULL, 0, 0, 0},
        {"PartyCanBeUnderCover", NULL, 0, 0, 0},
        {"SetLapTime", NULL, 0, 0, 0},
        {"CreatePod", NULL, 0, 0, 0},
        {"MushroomCollapse", NULL, 0, 0, 0},
        {"BoulderSection", NULL, 0, 0, 0},
        {"RaceOpponent", Action_RaceOpponent, 0, 0, 0},
        {"Sebulba", NULL, 0, 0, 0},
        {"IgnoreTurnAroundSpline", NULL, 0, 0, 0},
        {"CanMoveWhenDeactivated", NULL, 0, 0, 0},
        {"DontAttack", Action_DontAttack, 0, 0, 0},
        {"CanPullLevers", Action_CanPullLevers, 0, 0, 0},
        {"NewSebulba", NULL, 0, 0, 0},
        {NULL, NULL, 0, 0, 0},
    };
}

DECOMP_ASSERT(sizeof(lego_aiactiondefs) / sizeof(lego_aiactiondefs[0]) == LEGO_AI_ACTION_NEW_SEBULBA + 2,
              "complete game action registry");

__used__ static i32 Action_FollowCharacter(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_FollowDirection(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_PlayGizObstacle(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 == 0 || param_4 == 0 || WORLD == NULL || WORLD->gizmo_sys == NULL) {
        return 1;
    }

    GIZOBSTACLE *obstacle = NULL;
    bool backwards = false;
    bool stay_open = false;
    bool stay_shut = false;
    bool snap = false;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, obstacle_gizmotype_id, value + NuStrLen("name="));
            obstacle = gizmo != NULL ? static_cast<GIZOBSTACLE *>(gizmo->object) : NULL;
        } else if (NuStrICmp(params[index], "backwards") == 0) {
            backwards = true;
        } else if (NuStrICmp(params[index], "stayopen") == 0) {
            stay_open = true;
        } else if (NuStrICmp(params[index], "stayshut") == 0) {
            stay_shut = true;
        } else if (NuStrICmp(params[index], "snap") == 0) {
            snap = true;
        }
    }

    if (obstacle != NULL) {
        if (backwards || stay_shut) {
            if (snap) {
                GizObstacle_JumpToStart(obstacle);
            } else {
                GizObstacle_PlayBackwards(obstacle);
            }
        } else if (snap) {
            GizObstacle_JumpToEnd(obstacle);
        } else {
            GizObstacle_PlayForwards(obstacle);
        }
        obstacle->runtime_flags =
            static_cast<u8>((obstacle->runtime_flags & 0xf3u) | (stay_shut ? 8u : 0u) | (stay_open ? 4u : 0u));
    }
    return 1;
}

__used__ static i32 Action_PressJumpButton(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && object->pad_gamepad != NULL) {
        object->pad_gamepad->buttons_pressed |= GAMEPAD_JUMP;
    }
    return 1;
}

__used__ static i32 Action_ReleaseTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_ResetGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    if (param_5 != 0) {
        GameCam_Reset(GameCam);
    }
    return 1;
}

__used__ static i32 Action_SetAnimSpeedMul(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    if (object != NULL && param_5 != 0 && param_4 > 0) {
        f32 multiply_by = 1.0f;
        f32 maximum = 1.0e9f;
        f32 minimum = 0.0f;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = ActionParamValue(params[index], "value");
            if (value != NULL) {
                object->animation_speed_multiplier = AIParamToFloat(processor, value);
            } else if ((value = ActionParamValue(params[index], "multiply_by")) != NULL) {
                multiply_by = AIParamToFloat(processor, value);
            } else if ((value = ActionParamValue(params[index], "max")) != NULL) {
                maximum = AIParamToFloat(processor, value);
            } else if ((value = ActionParamValue(params[index], "min")) != NULL) {
                minimum = AIParamToFloat(processor, value);
            }
        }
        if (multiply_by != 1.0f) {
            object->animation_speed_multiplier =
                MAX(minimum, MIN(maximum, object->animation_speed_multiplier * multiply_by));
        }
    }
    return 1;
}

__used__ static i32 Action_SetCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet->owner;
    f32 speed = 0.0f;
    i32 speed_mode = -1;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "character=");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value + NuStrLen("character="));
        } else if (NuStrICmp(params[index], "speed=TIPTOE") == 0) {
            speed_mode = 2;
        } else if (NuStrICmp(params[index], "speed=WALK") == 0) {
            speed_mode = 1;
        } else if (NuStrICmp(params[index], "speed=RUN") == 0) {
            speed_mode = 0;
        } else {
            speed = AIParamToFloat(processor, params[index]);
        }
    }
    if (object == NULL) {
        return 1;
    }

    GAMECHARACTERDATA *character = ActionGameCharacterData(object);
    if (character != NULL) {
        if (speed_mode == 2) {
            speed = character->tiptoe_speed;
        } else if (speed_mode == 1) {
            speed = character->walk_speed;
        } else if (speed_mode == 0) {
            speed = character->run_speed;
            object->field_0xdc8 = 1.0f;
        }
    }
    object->apiobj.velocity = {0.0f, 0.0f, speed};
    NuVecRotateY(&object->apiobj.velocity, &object->apiobj.velocity, object->apiobj.facing_angle);
    return 1;
}

__used__ static i32 Action_SetHearDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff && sys != NULL) {
            object->heardistance = sys->creatures[packet->field_0x134].hear_distance;
        } else if (GetHearDistanceFn != NULL && object->character_model != NULL) {
            object->heardistance = GetHearDistanceFn(object->character_model->model_id);
        } else {
            object->heardistance = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object->heardistance = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetHintComplete(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetInvulnerable(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet != NULL ? packet->owner : NULL;
    i16 types[10] = {};
    i32 type_count = 0;
    bool enabled = true;
    bool still_take_hit = false;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = ActionParamValue(params[index], "type");
        if (value != NULL && LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL && type_count < 10) {
            const i32 local_type = LevelCharacterTypeIDFn(value);
            if (local_type != -1) {
                const i32 global_type = LevelCharacterGlobalIDFn(static_cast<u8>(local_type));
                if (global_type != -1) {
                    types[type_count++] = static_cast<i16>(global_type);
                }
            }
            continue;
        }
        value = ActionParamValue(params[index], "character");
        if (value != NULL) {
            object = GetNamedGameObject(sys, value);
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            enabled = false;
        } else if (NuStrICmp(params[index], "still_do_take_hit_anim") == 0) {
            still_take_hit = true;
        }
    }

    const auto apply = [enabled, still_take_hit](GameObject_s *candidate) {
        if (candidate == NULL || (candidate->apiobj.field_0x1f4 & 0x1000u) == 0) {
            return;
        }
        if (enabled) {
            candidate->field_0xefe |= 0x40;
            candidate->field_0xefd = static_cast<u8>((candidate->field_0xefd & ~0x10u) | (still_take_hit ? 0x10u : 0u));
        } else {
            candidate->field_0xefe &= static_cast<u8>(~0x40u);
        }
    };

    if (type_count != 0) {
        for (i32 object_index = 0; Obj != NULL && object_index < HIGHGAMEOBJECT; ++object_index) {
            GameObject_s *candidate = &Obj[object_index];
            if ((candidate->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) {
                continue;
            }
            for (i32 type_index = 0; type_index < type_count; ++type_index) {
                if (candidate->id == types[type_index]) {
                    apply(candidate);
                    break;
                }
            }
        }
    } else {
        apply(object);
    }
    return 1;
}

__used__ static i32 Action_SetVisibility(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char **params, i32 param_count,
                                         i32 first_time, f32) {
    if (first_time == 0) {
        return 1;
    }

    nuhspecial_s special = {};
    i32 visible = 1;
    for (i32 index = 0; index < param_count; ++index) {
        char *name = ActionParamValue(params[index], "name");
        if (name != NULL && WORLD != NULL && WORLD->current_gscn != NULL) {
            NuSpecialFind(WORLD->current_gscn, &special, name, 1);
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            visible = 0;
        }
    }
    if (NuSpecialExistsFn(&special) != 0) {
        NuSpecialSetVisibility(&special, visible);
    }
    return 1;
}

__used__ static i32 Action_SetLastAttacker(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 == 0) {
        return 1;
    }

    GameObject_s *victim = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    GameObject_s *attacker = NULL;
    for (i32 index = 0; index < param_4; ++index) {
        char *value = NuStrIStr(params[index], "victim=");
        if (value != NULL) {
            victim = GetNamedGameObject(sys, value + NuStrLen("victim="));
            continue;
        }
        if (NuStrICmp(params[index], "attacker=opponent") == 0) {
            if (packet != NULL && packet->action_target_ref != NULL) {
                attacker = *packet->action_target_ref;
            }
            continue;
        }
        value = NuStrIStr(params[index], "attacker=");
        if (value != NULL) {
            if (NuStrICmp(value + NuStrLen("attacker="), "player") == 0) {
                attacker = sys != NULL && sys->player_1 != NULL ? sys->player_1->objptr : NULL;
            } else {
                attacker = GetNamedGameObject(sys, value + NuStrLen("attacker="));
            }
        }
    }
    if (victim != NULL && attacker != NULL) {
        victim->last_attacker = attacker;
    }
    return 1;
}

__used__ static i32 Action_SetUseOneAtOnce(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)processor;
    (void)param_6;
    if (param_5 != 0) {
        bool enabled;
        GameObject_s *object = ActionCharacterAndToggle(sys, packet, params, param_4, &enabled);
        if (object != NULL) {
            object->field_0xf01 = static_cast<u8>((object->field_0xf01 & ~0x20u) | (enabled ? 0x20u : 0u));
        }
    }
    return 1;
}

__used__ static i32 Action_SetViewDistance(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff && sys != NULL) {
            object->viewdistance = sys->creatures[packet->field_0x134].view_distance;
        } else if (GetViewRangeFn != NULL && object->character_model != NULL) {
            object->viewdistance = GetViewRangeFn(object->character_model->model_id);
        } else {
            object->viewdistance = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object->viewdistance = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_ShootAtOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_UseCurrentSpeed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                           i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)param_6;
    if (packet == NULL || packet->owner == NULL || param_5 == 0) {
        return 1;
    }

    GameObject_s *object = packet->owner;
    object->field_0xf02 |= 0x20;
    object->current_speed_multiplier = 1.0f;
    bool snap_to_speed = false;
    for (i32 index = 0; index < param_4; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            object->field_0xf02 &= static_cast<u8>(~0x20u);
        } else if (char *value = NuStrIStr(params[index], "multiplier")) {
            object->current_speed_multiplier = AIParamToFloat(processor, value + NuStrLen("multiplier") + 1);
        } else if (NuStrICmp(params[index], "snaptospeed") == 0) {
            snap_to_speed = true;
        }
    }

    if ((object->field_0xf02 & 0x20) != 0 && (object->field_0xef9 & 0x40) == 0) {
        object->field_0xef9 |= 0x40;
        if (snap_to_speed && WORLD != NULL) {
            ComplexSockPosition(WORLD->sock_sys, &object->apiobj.position, static_cast<i8>(object->field_0x661),
                                object->sock_segment, &object->sock_position);
            ComplexSockAngles(&object->sock_position);
        }
    }
    if (snap_to_speed) {
        CurrentStart(object, 1, 1);
    }
    return 1;
}

__used__ static i32 Action_AddTorpedoPacket(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_BigJumpToLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CatchUpForbidden(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_CheckWallSplines(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_GoToOriginalPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_JudderGameCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_MoveAwayFromNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetControlSystem(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetMaxViewHeight(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)param_6;
    if (packet != NULL && packet->owner != NULL && param_5 != 0) {
        APIOBJECT *object = &packet->owner->apiobj;
        if (packet->field_0x134 != 0xff && sys != NULL) {
            object->maxviewheight = sys->creatures[packet->field_0x134].max_view_height;
        } else if (GetMaxViewHeightFn != NULL && object->character_model != NULL) {
            object->maxviewheight = GetMaxViewHeightFn(object->character_model->model_id);
        } else {
            object->maxviewheight = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
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
        if (packet->field_0x134 != 0xff && sys != NULL) {
            object->minviewheight = sys->creatures[packet->field_0x134].min_view_height;
        } else if (GetMinViewHeightFn != NULL && object->character_model != NULL) {
            object->minviewheight = GetMinViewHeightFn(object->character_model->model_id);
        } else {
            object->minviewheight = 1.0f;
        }
        if (param_4 != 0 && NuStrICmp(params[0], "default") != 0) {
            object->minviewheight = AIParamToFloatEx(packet, processor, params[0]);
        }
    }
    return 1;
}

__used__ static i32 Action_SetObstacleToEnd(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)param_6;
    if (param_5 != 0 && WORLD != NULL && WORLD->giz_obstacle_sys != NULL) {
        GIZOBSTACLE *obstacle = NULL;
        for (i32 index = 0; index < param_4; ++index) {
            char *value = NuStrIStr(params[index], "name=");
            if (value != NULL) {
                obstacle = GizObstacle_FindByName(WORLD->giz_obstacle_sys, value + NuStrLen("name="));
            }
        }
        if (obstacle != NULL) {
            GizObstacle_JumpToEnd(obstacle);
        }
    }
    return 1;
}

__used__ static i32 Action_SetReturnToState(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_SetScaleOverride(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static i32 Action_UseBigJumpToJump(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char **params,
                                            i32 param_4, i32 param_5, f32 param_6) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)params;
    (void)param_4;
    (void)param_5;
    (void)param_6;
    return 0;
}

__used__ static f32 Condition_IAm(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_PSP(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg, void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IAmA(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Indy(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Side(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_XPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_YPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ZPos(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                   void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Debug(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                    void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_MySet(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                    void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Param(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                    void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

static f32 Condition_Timer(AISYS *, AISCRIPTPROCESS *processor, AIPACKET *, char *, void *) {
    return processor->script_timer;
}

__used__ static f32 Condition_Active(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                     void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GotGun(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                     void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_OnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                     void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

static f32 Condition_Random(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return NuRandFloat();
}

__used__ static f32 Condition_BeenHit(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Context(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_InSwamp(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IsAlive(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

static f32 Condition_Message(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *void_arg) {
    GIZAIMESSAGE_s *message = static_cast<GIZAIMESSAGE_s *>(void_arg);
    return message != NULL ? message->value : 0.0f;
}

__used__ static f32 Condition_MusicOn(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return SuperOptions.music_enabled != 0 ? 1.0f : 0.0f;
}

__used__ static f32 Condition_RaceLap(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                      void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Blocking(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                       void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

static f32 Condition_Freeplay(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *) {
    return FreePlay != 0 ? 1.0f : 0.0f;
}

__used__ static f32 Condition_GlynTest(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                       void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_OnGround(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                       void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_OnObject(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                       void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Colliding(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GotVictim(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_HitPoints(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IAmABaddy(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IAmAGoody(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_InContext(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_InMiniCut(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IsVisible(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_LastLevel(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Player1Is(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_Player2Is(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_StuckTime(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_TakenOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                        void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BeingTowed(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_CategoryIs(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)arg;
    const isize category = reinterpret_cast<isize>(void_arg);
    GameObject_s *object = packet != NULL && packet->owner != NULL ? packet->owner->apiobj.objptr : NULL;
    return category != -1 && object != NULL && CharCategory_IsCategory(object, static_cast<i32>(category)) != 0 ? 1.0f
                                                                                                                : 0.0f;
}

__used__ static f32 Condition_ForceAtEnd(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GotLocator(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return processor != NULL && processor->locator != NULL ? 1.0f : 0.0f;
}

__used__ static f32 Condition_HoverPhase(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IAmPlayer2(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IsOnScreen(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IsSetAlive(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_MissionWon(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_NumBaddies(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ScreenWipe(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ShopActive(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_SpawnCount(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_UsingForce(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                         void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BeenAlerted(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BeenSpawned(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)arg;
    (void)void_arg;
    return packet != NULL && packet->owner != NULL && packet->owner->apiobj.field_0x27c == -1 &&
                   packet->field_0x134 == 0xff
               ? 1.0f
               : 0.0f;
}

static f32 Condition_BeenToLevel(AISYS *, AISCRIPTPROCESS *, AIPACKET *, char *, void *void_arg) {
    const isize area_level = AIConditionArgumentValue(void_arg);
    if (area_level == -1) {
        return 0.0f;
    }
    const u8 *progress = static_cast<const u8 *>(LevelProgressData) + area_level * LEVEL_PROGRESS_STRIDE;
    return (progress[LEVEL_PROGRESS_COMPLETION_FLAGS_OFFSET] & LEVEL_PROGRESS_STORY_COMPLETE) != 0 ? 1.0f : 0.0f;
}

__used__ static f32 Condition_GotOpponent(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)arg;
    (void)void_arg;
    return packet != NULL && packet->opponent != NULL ? 1.0f : 0.0f;
}

__used__ static f32 Condition_HasTakeOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_IAmANeutral(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_InLevelNode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_InterruptID(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_MissionMode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_OpponentIsA(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_OriginRange(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)processor;
    (void)arg;
    (void)void_arg;
    if (packet != NULL) {
        NUVEC *origin = GetAICreatureOriginFn != NULL ? GetAICreatureOriginFn(sys, packet) : NULL;
        if (origin == NULL && sys != NULL && packet->field_0x134 != 0xff && packet->field_0x134 < sys->creature_count) {
            origin = &sys->creatures[packet->field_0x134].pos;
        }
        if (origin != NULL) {
            return NuVecDist(&packet->terrain_origin, origin, NULL);
        }
    }
    return 0.0f;
}

__used__ static f32 Condition_PathBlocked(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_PlayerRange(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)processor;
    (void)arg;
    (void)void_arg;
    if (packet == NULL || packet->owner == NULL || sys == NULL || sys->player_1 == NULL) {
        return 1.0e9f;
    }
    return NuVecDist(&sys->player_1->position, &packet->owner->apiobj.position, NULL);
}

__used__ static f32 Condition_ScriptParam(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_TimeOffPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_TurretAlive(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                          void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_AnimSpeedMul(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_AreaComplete(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BehindCamera(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_CanHearRadio(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ForceAtStart(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ForcePushing(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GizmoOutput0(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GizmoOutput1(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GizmoOutput2(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_GizmoOutput3(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_HintComplete(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_LocatorRange(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)arg;
    AILOCATOR *locator = static_cast<AILOCATOR *>(void_arg);
    if (locator == NULL && processor != NULL) {
        locator = processor->locator;
    }
    if (packet == NULL || packet->owner == NULL || locator == NULL) {
        return 1.0e9f;
    }
    return NuVecDist(&packet->terrain_origin, &locator->position, NULL);
}

__used__ static f32 Condition_PlayerInSock(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_PlayerOnPath(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                           void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BeenTakenOver(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                            void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_BlowupBlownup(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                            void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_ChallengeMode(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                            void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static f32 Condition_CheatProgress(AISYS *sys, AISCRIPTPROCESS *processor, AIPACKET *packet, char *arg,
                                            void *void_arg) {
    (void)sys;
    (void)processor;
    (void)packet;
    (void)arg;
    (void)void_arg;
    return 0.0f;
}

__used__ static void *Condition_IAmInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_IAmAInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_SideInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_ForceInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_BlowupInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_XYZPosInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_BeenHitInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_IsAliveInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

static void *Condition_MessageInit(AISYS *system, char *arg, AISCRIPT *) {
    if (system == NULL || arg == NULL || gizaimessagesys == NULL) {
        return NULL;
    }
    return CheckGizAIMessage(gizaimessagesys, arg, NULL);
}

__used__ static void *Condition_ObstacleInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

__used__ static void *Condition_OnObjectInit(AISYS *sys, char *arg, AISCRIPT *script) {
    (void)sys;
    (void)arg;
    (void)script;
    return NULL;
}

namespace {
    struct AISysRegistryCallbacks {
        AISysRegistryCallbacks() {
            api_aiactiondefs[API_AI_ACTION_IDLE].eval_fn = Action_Idle;
            api_aiactiondefs[API_AI_ACTION_RESET_TIMER].eval_fn = Action_ResetTimer;
            api_aiactiondefs[API_AI_ACTION_FOLLOW_PLAYER].eval_fn = Action_FollowPlayer;
            api_aiactiondefs[API_AI_ACTION_FOLLOW_OPPONENT].eval_fn = Action_FollowOpponent;
            api_aiactiondefs[API_AI_ACTION_SET_VIEW_DISTANCE].eval_fn = Action_SetViewDistance;
            api_aiactiondefs[API_AI_ACTION_SET_MAX_VIEW_HEIGHT].eval_fn = Action_SetMaxViewHeight;
            api_aiactiondefs[API_AI_ACTION_SET_MIN_VIEW_HEIGHT].eval_fn = Action_SetMinViewHeight;
            api_aiactiondefs[API_AI_ACTION_SET_HEAR_DISTANCE].eval_fn = Action_SetHearDistance;
            api_aiactiondefs[API_AI_ACTION_SET_MOVE_RADIUS].eval_fn = Action_SetMoveRadius;
            api_aiactiondefs[API_AI_ACTION_GO_TO_LOCATOR].eval_fn = Action_GoToLocator;
            api_aiactiondefs[API_AI_ACTION_SET_LOCATOR].eval_fn = Action_SetLocator;
            api_aiactiondefs[API_AI_ACTION_FOLLOW_PATH].eval_fn = Action_FollowPath;
            api_aiactiondefs[API_AI_ACTION_RETURN_TO_STATE].eval_fn = Action_ReturnToState;
            api_aiconditiondefs[API_AI_CONDITION_TIMER].eval_fn = Condition_Timer;
            api_aiconditiondefs[API_AI_CONDITION_RANDOM].eval_fn = Condition_Random;
            api_aiconditiondefs[API_AI_CONDITION_GOT_LOCATOR].eval_fn = Condition_GotLocator;
            api_aiconditiondefs[API_AI_CONDITION_PLAYER_RANGE].eval_fn = Condition_PlayerRange;
            api_aiconditiondefs[API_AI_CONDITION_GOT_OPPONENT].eval_fn = Condition_GotOpponent;
            api_aiconditiondefs[API_AI_CONDITION_ORIGIN_RANGE].eval_fn = Condition_OriginRange;
            api_aiconditiondefs[API_AI_CONDITION_LOCATOR_RANGE].eval_fn = Condition_LocatorRange;

            lego_aiconditiondefs[LEGO_AI_CONDITION_CATEGORY_IS].eval_fn = Condition_CategoryIs;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_TO_LEVEL].eval_fn = Condition_BeenToLevel;
            lego_aiconditiondefs[LEGO_AI_CONDITION_MESSAGE].eval_fn = Condition_Message;
            lego_aiconditiondefs[LEGO_AI_CONDITION_MESSAGE].init_fn = Condition_MessageInit;
            lego_aiconditiondefs[LEGO_AI_CONDITION_FREEPLAY].eval_fn = Condition_Freeplay;
            lego_aiconditiondefs[LEGO_AI_CONDITION_BEEN_SPAWNED].eval_fn = Condition_BeenSpawned;
            lego_aiconditiondefs[LEGO_AI_CONDITION_MUSIC_ON].eval_fn = Condition_MusicOn;

            lego_aiactiondefs[LEGO_AI_ACTION_SET_CURRENT_SPEED].eval_fn = Action_SetCurrentSpeed;
            lego_aiactiondefs[LEGO_AI_ACTION_USE_CURRENT_SPEED].eval_fn = Action_UseCurrentSpeed;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_MAX_MOVEMENT_RANGE].eval_fn = Action_SetMaxMovementRange;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_DEFAULT_MOVEMENT_RANGE].eval_fn = Action_SetDefaultMovementRange;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_GRAVITY_HEIGHT].eval_fn = Action_SetGravityHeight;
            lego_aiactiondefs[LEGO_AI_ACTION_PLAY_GIZ_OBSTACLE].eval_fn = Action_PlayGizObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_PLAY_OBSTACLE].eval_fn = Action_PlayGizObstacle;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_OBSTACLE_TO_END].eval_fn = Action_SetObstacleToEnd;
            lego_aiactiondefs[LEGO_AI_ACTION_MOVE_AWAY_FROM_LAST_ATTACKER].eval_fn = Action_MoveAwayFromLastAttacker;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_LAST_ATTACKER].eval_fn = Action_SetLastAttacker;
            lego_aiactiondefs[LEGO_AI_ACTION_SET_USE_ONE_AT_ONCE].eval_fn = Action_SetUseOneAtOnce;
        }
    };

    AISysRegistryCallbacks aisys_registry_callbacks;
} // namespace
