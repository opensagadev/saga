#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"

void SetEffectVisibility(char *name, i32 visible);
i32 EffectOffProgress_Update(LEVEL_PROGRESS_s *progress, char *name, i32 visible);
void PlayRadio(char *special_name, char *blowup_name, i32 play);

static void GizActions_EnableSock(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    i32 sock_index = -1;
    i32 enabled = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "ix=");
        if (value != NULL) {
            sock_index = static_cast<i32>(NuAToF(value + 3));
        } else if ((value = NuStrIStr(params[index], "name=")) != NULL) {
            SOCK *sock = FindSock(WORLD->sock_sys, value + 5);
            if (sock != NULL) {
                sock_index = sock - WORLD->sock_sys->sock;
            }
        } else if (NuStrIStr(params[index], "FALSE") != NULL) {
            enabled = 0;
        }
    }
    if (enabled != 0) {
        SockOn(WORLD->sock_sys, sock_index);
    } else {
        SockOff(WORLD->sock_sys, sock_index);
    }
}

static void GizAction_TurnOnFlowBox(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    char *name = NULL;
    i32 enabled = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL)
            name = value + 5;
        else if (NuStrICmp(params[index], "FALSE") == 0)
            enabled = 0;
    }
    if (name != NULL && flow != NULL) {
        for (i32 index = 0; index < flow->flowbox_count; ++index) {
            if (flow->flowboxes[index].name != NULL && NuStrICmp(flow->flowboxes[index].name, name) == 0)
                flow->flowboxes[index].state_flags_low = (flow->flowboxes[index].state_flags_low & ~1) | (enabled & 1);
        }
    }
}

static void GizActions_ActivateBelt(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    i32 active = 1;
    for (i32 index = 0; index < count; ++index) {
        if (NuStrICmp(params[index], "FALSE") == 0) {
            active = 0;
        } else if (NuStrICmp(params[index], "TRUE") == 0) {
            active = 1;
        }
    }
    WorldInfo_CurrentlyActive()->field_0x5174 = static_cast<i8>(active);
}

static void GizActions_ChangeObstTriggerType(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    char *name = NULL;
    i32 mode = -1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            name = value + NuStrLen("name=");
        } else if (NuStrICmp(params[index], "AUTOSTART") == 0) {
            mode = 0;
        } else if (NuStrICmp(params[index], "PROXIMITY") == 0) {
            mode = 1;
        } else if (NuStrICmp(params[index], "PROXIMITYGROUND") == 0) {
            mode = 2;
        } else if (NuStrICmp(params[index], "NOTRIGGER") == 0) {
            mode = 3;
        } else if (NuStrICmp(params[index], "TECHNOONLY") == 0) {
            mode = 4;
        } else if (NuStrICmp(params[index], "INBOXANDGROUND") == 0) {
            mode = 6;
        } else if (NuStrICmp(params[index], "INBOX") == 0) {
            mode = 5;
        } else if (NuStrICmp(params[index], "PUSHONLY") == 0) {
            mode = 7;
        }
    }
    if (mode == -1 || name == NULL) {
        return;
    }
    GIZMO_s *gizmo = GizmoFindByName(flow->gizmo_sys, obstacle_gizmotype_id, name);
    if (gizmo != NULL && gizmo->object != NULL) {
        static_cast<GIZOBSTACLE_s *>(gizmo->object)->mode = static_cast<u8>(mode);
    }
}

static void GizActions_HitBlowup(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    i32 damage = 0;
    i8 gizmo_type = 0;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            name = value + NuStrLen("name=");
        } else if (NuStrICmp(params[index], "BLOWUP") == 0) {
            gizmo_type = 0;
        } else if (NuStrICmp(params[index], "TURRET") == 0) {
            gizmo_type = 1;
        } else if ((value = NuStrIStr(params[index], "damage=")) != NULL) {
            damage = static_cast<i32>(NuAToF(value + NuStrLen("damage=")));
        }
    }
    if (damage == 0 || name == NULL) {
        return;
    }
    if (gizmo_type == 1) {
        GIZMO_s *gizmo = GizmoFindByName(WORLD->gizmo_sys, turret_gizmotype_id, name);
        if (gizmo != NULL && gizmo->object != NULL) {
            GizTurrets_Hit(WORLD, static_cast<GIZTURRET_s *>(gizmo->object), NULL, -1, damage);
        }
    } else {
        GIZMO_s *gizmo = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, name);
        if (gizmo != NULL && gizmo->object != NULL) {
            GizmoBlowupBlowup(static_cast<GIZMOBLOWUP_s *>(gizmo->object), 1, -1, damage, NULL, 1);
        }
    }
}

static void GizActions_PlayCutscene(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    char *name = NULL;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            name = value + NuStrLen("name=");
        }
    }
    NewCutScene(NULL, WorldInfo_CurrentlyActive()->cutscene_sys, name, 0);
}

static void GizActions_PlayRadio(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *special_name = NULL;
    char *blowup_name = NULL;
    i32 play = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "BlowUp=");
        if (value != NULL) {
            blowup_name = value + NuStrLen("BlowUp=");
        } else if ((value = NuStrIStr(params[index], "Special=")) != NULL) {
            special_name = value + NuStrLen("Special=");
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            play = 0;
        }
    }
    if (special_name != NULL || blowup_name != NULL) {
        PlayRadio(special_name, blowup_name, play);
    }
}

static void GizActions_PlayForce(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    i32 forwards = 1;
    i32 snap = 0;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "Name");
        if (value != NULL) {
            name = value + NuStrLen("Name") + 1;
        } else if (NuStrICmp(params[index], "BACKWARD") == 0) {
            forwards = 0;
        } else if (NuStrICmp(params[index], "FORWARD") == 0) {
            forwards = 1;
        } else if (NuStrICmp(params[index], "SNAP") == 0) {
            snap = 1;
        }
    }
    if (name == NULL) {
        return;
    }
    GIZMO_s *gizmo = GizmoFindByName(flow->gizmo_sys, force_gizmotype_id, name);
    GIZFORCE_s *force = gizmo != NULL ? static_cast<GIZFORCE_s *>(gizmo->object) : NULL;
    if (force == NULL) {
        return;
    }
    if (forwards != 0) {
        if (snap != 0) {
            force->runtime_flags |= 0x80;
            GameAnimSet_JumpToEnd(force->anim_set);
        } else {
            GizForce_PlayForwards(force);
        }
    } else if (snap != 0) {
        GameAnimSet_JumpToStart(force->anim_set);
    } else {
        GizForce_PlayBackwards(force);
    }
}

static void GizActions_PlaySpecial(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    i32 forwards = 1;
    i32 snap = 0;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "Name");
        if (value != NULL) {
            name = value + NuStrLen("Name") + 1;
        } else if (NuStrICmp(params[index], "BACKWARD") == 0) {
            forwards = 0;
        } else if (NuStrICmp(params[index], "FORWARD") == 0) {
            forwards = 1;
        } else if (NuStrICmp(params[index], "SNAP") == 0) {
            snap = 1;
        }
    }
    if (name == NULL) {
        return;
    }
    GIZMO_s *gizmo = GizmoFindByName(flow->gizmo_sys, gizspecial_gizmotype_id, name);
    GIZSPECIAL_s *special = gizmo != NULL ? static_cast<GIZSPECIAL_s *>(gizmo->object) : NULL;
    if (special == NULL) {
        return;
    }
    if (forwards != 0) {
        if (snap != 0) {
            GameAnimSet_JumpToEnd(special->anim_set);
        } else {
            GameAnimSet_Play(special->anim_set, 1.0f, 0);
        }
    } else if (snap != 0) {
        GameAnimSet_JumpToStart(special->anim_set);
    } else {
        GameAnimSet_Play(special->anim_set, -1.0f, 0);
    }
}

static void GizActions_PlayObstacle(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    i32 forwards = 1;
    i32 snap = 0;
    i32 stay_open = 0;
    i32 stay_shut = 0;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "Name");
        if (value != NULL) {
            name = value + NuStrLen("Name") + 1;
        } else if (NuStrICmp(params[index], "BACKWARD") == 0) {
            forwards = 0;
        } else if (NuStrICmp(params[index], "FORWARD") == 0) {
            forwards = 1;
        } else if (NuStrICmp(params[index], "SNAP") == 0) {
            snap = 1;
        } else if (NuStrICmp(params[index], "STAYOPEN") == 0) {
            stay_open = 1;
        } else if (NuStrICmp(params[index], "STAYSHUT") == 0) {
            stay_shut = 1;
        }
    }
    if (name == NULL) {
        return;
    }
    GIZMO *gizmo = GizmoFindByName(flow->gizmo_sys, obstacle_gizmotype_id, name);
    GIZOBSTACLE_s *obstacle = gizmo != NULL ? static_cast<GIZOBSTACLE_s *>(gizmo->object) : NULL;
    if (obstacle == NULL) {
        return;
    }
    if (forwards != 0) {
        if (snap != 0) {
            GizObstacle_JumpToEnd(obstacle);
        } else {
            GizObstacle_PlayForwards(obstacle);
        }
    } else if (snap != 0) {
        GizObstacle_JumpToStart(obstacle);
    } else {
        GizObstacle_PlayBackwards(obstacle);
    }
    u8 runtime_flags = obstacle->runtime_flags;
    runtime_flags &= ~0xc;
    runtime_flags |= (stay_shut & 1) << 3;
    runtime_flags |= stay_open << 2;
    obstacle->runtime_flags = runtime_flags;
}

static void GizActions_GoThroughDoor(GIZFLOW_s *, FLOWBOX_s *, char **params, int param_count) {
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

static void GizActions_GoToNewLevel(GIZFLOW_s *, FLOWBOX_s *, char **params, int param_count) {
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

static void GizAction_SetAIState(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    NUVEC origin = v000;
    GameObject_s *named_object = NULL;
    char *state_name = NULL;
    i32 types[10];
    i32 type_count = 0;
    f32 range_squared = 0.0f;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "Character");
        if (value != NULL) {
            named_object = GetNamedGameObject(WORLD->ai_sys, value + 10);
        }
        if ((value = NuStrIStr(params[index], "range")) != NULL) {
            f32 range = NuAToF(value + 6);
            range_squared = range * range;
        } else if ((value = NuStrIStr(params[index], "type")) != NULL) {
            if (LevelCharacterTypeIDFn != NULL && LevelCharacterGlobalIDFn != NULL) {
                u8 local_type = LevelCharacterTypeIDFn(value + 5);
                if (local_type != 0xff) {
                    i32 global_type = LevelCharacterGlobalIDFn(local_type);
                    if (global_type != 0xff && type_count < 10) {
                        types[type_count++] = global_type;
                    }
                }
            }
        } else if ((value = NuStrIStr(params[index], "State")) != NULL) {
            state_name = value + 6;
        }
    }

    if (state_name == NULL || (type_count == 0 && !(range_squared > 0.0f))) {
        if (named_object != NULL) {
            named_object->ai.script_process.next_state =
                AIStateFind(state_name, named_object->ai.script_process.script);
        }
        return;
    }
    if (named_object != NULL) {
        origin = named_object->apiobj.collision_position;
    }
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index) {
        GameObject_s *object = &Obj[object_index];
        if ((object->apiobj.field_0x1f8 & (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER)) !=
                (APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER) ||
            (object->apiobj.field_0x1f4 & 0x400) == 0) {
            continue;
        }
        NUVEC difference;
        NuVecSub(&difference, &origin, &object->apiobj.position);
        f32 distance_squared = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;
        i32 matching_type = type_count == 0;
        for (i32 type_index = 0; type_index < type_count; ++type_index) {
            if (object->id == types[type_index]) {
                matching_type = 1;
            }
        }
        if (matching_type != 0 && range_squared > distance_squared) {
            object->ai.script_process.next_state = AIStateFind(state_name, object->ai.script_process.script);
        }
    }
}

static void GizAction_SetAIMessage(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    f32 value = 0.0f;
    i32 mode = 0;
    char *name = NULL;
    for (i32 index = 0; index < count; ++index) {
        char *argument = NuStrIStr(params[index], "Name");
        if (argument != NULL)
            name = argument + NuStrLen("Name") + 1;
        else if ((argument = NuStrIStr(params[index], "Val")) != NULL)
            value = NuAToF(argument + NuStrLen("Val") + 1);
        else if ((argument = NuStrIStr(params[index], "increment=")) != NULL) {
            value = NuAToF(argument + 10);
            mode = 1;
        } else if ((argument = NuStrIStr(params[index], "decrement=")) != NULL) {
            value = NuAToF(argument + 10);
            mode = -1;
        }
    }
    GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, name, NULL);
    switch (mode) {
        case 0:
            message->value = value;
            break;
        case 1:
            message->value = value + message->value;
            break;
        case -1:
            message->value = message->value - value;
            break;
    }
}

static void GizAction_ChangeTechnoTgt(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *techno_name = NULL;
    char *target_name = NULL;
    i32 target_kind = 0;
    u8 movement = 0xff;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "techno=");
        if (value != NULL) {
            techno_name = value + NuStrLen("techno=");
        } else if ((value = NuStrIStr(params[index], "target=")) != NULL) {
            target_name = value + NuStrLen("target=");
        } else if (NuStrICmp(value, "Move_Waggle") == 0) {
            movement = 1;
        } else if (NuStrICmp(value, "Move_Spin") == 0) {
            movement = 2;
        } else if (NuStrICmp(value, "Move_Horizontal") == 0) {
            movement = 8;
        } else if (NuStrICmp(value, "Move_Vertical") == 0) {
            movement = 4;
        } else if (NuStrICmp(params[index], "GIZMO") == 0) {
            target_kind = 3;
        } else if (NuStrICmp(params[index], "HSPECIAL") == 0) {
            target_kind = 2;
        } else if (NuStrICmp(params[index], "CREATURE") == 0) {
            target_kind = 1;
        } else if (NuStrICmp(params[index], "NO_TARGET") == 0) {
            target_kind = -1;
        }
    }
    if (techno_name == NULL || (target_kind != -1 && target_name == NULL)) {
        return;
    }
    i32 type_id = GizmoGetTypeIDByName(WORLD->gizmo_sys, "Techno");
    GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, type_id, techno_name);
    TECHNO *techno = gizmo != NULL ? static_cast<TECHNO *>(gizmo->object) : NULL;
    if (techno == NULL) {
        return;
    }
    if (movement != 0xff) {
        techno->enabled = movement;
    }
    if (target_kind == -1) {
        GameObject_s *operator_object = NULL;
        Techno_FindOperator(techno->controlled_object, NULL, &operator_object);
        techno->controlled_object = NULL;
        NuStrCpy(techno->target_name, "");
        techno->flags &= ~TECHNO_FLAG_COMPLETE;
        techno->target_mode = 0;
        if (operator_object != NULL) {
            operator_object->character_context = -1;
        }
    }
    char old_name[16];
    NuStrCpy(old_name, techno->target_name);
    void *old_target = techno->controlled_object;
    i32 old_complete = (techno->flags & TECHNO_FLAG_COMPLETE) != 0;
    techno->controlled_object = NULL;
    NuStrNCpy(techno->target_name, target_name, 16);
    techno->target_mode = static_cast<u8>(target_kind);
    techno->flags &= ~TECHNO_FLAG_COMPLETE;
    GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    if (Technos_FindTgt(techno) == NULL) {
        techno->target_mode = 0;
        if (Technos_FindTgt(techno) == NULL) {
            NuStrCpy(techno->target_name, old_name);
            techno->controlled_object = old_target;
            techno->target_mode = static_cast<u8>(target_kind);
            techno->flags = (techno->flags & ~TECHNO_FLAG_COMPLETE) | (old_complete << 3);
        }
    }
}

static void GizAction_ActivatePartEffect(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "part_effect");
        if (value != NULL) {
            name = value + NuStrLen("part_effect") + 1;
        } else {
            NuStrICmp(params[index], "FALSE");
        }
    }
    if (name != NULL) {
        AddFiniteShotPART(PARTLookupType(name), &Player[0]->apiobj.collision_position, 50);
    }
}

static void GizAction_ActivateEffect(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    char *name = NULL;
    i32 visible = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "effect");
        if (value != NULL) {
            name = value + NuStrLen("effect") + 1;
        } else if (NuStrICmp(params[index], "FALSE") == 0) {
            visible = 0;
        }
    }
    if (name != NULL) {
        SetEffectVisibility(name, visible);
        EffectOffProgress_Update(WorldInfo_CurrentlyActive()->level_progress, name, visible);
    }
}

static void GizAction_ActivateChar(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
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

static void GizAction_ActivateGizmo(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    char *name = NULL;
    i32 type = -1;
    i32 active = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL)
            name = value + 5;
        else if ((value = NuStrIStr(params[index], "type=")) != NULL)
            type = GizmoGetTypeIDByName(flow->gizmo_sys, value + 5);
        else if (NuStrIStr(params[index], "FALSE") != NULL)
            active = 0;
    }
    if (name != NULL) {
        GIZMO *gizmo = GizmoFindByName(flow->gizmo_sys, type, name);
        if (gizmo != NULL)
            GizmoActivate(flow->gizmo_sys, gizmo, active, 1);
    }
}

static void GizAction_SetPickupVisibility(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (count <= 0) {
        return;
    }
    GIZMOPICKUP_s *pickup = NULL;
    i32 visible = 1;
    i32 id = -1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            pickup = GizmoPickup_FindByName(WORLD, value + NuStrLen("name="));
        } else if ((value = NuStrIStr(params[index], "id=")) != NULL) {
            id = NuAToI(value + NuStrLen("id") + 1);
        } else if (NuStrIStr(params[index], "FALSE") != NULL) {
            visible = 0;
        }
    }
    if (pickup != NULL) {
        pickup->state_enabled = visible;
        pickup->state_visible = visible;
        pickup->state_activated = visible;
        return;
    }
    if (id < 0 || WORLD->gizmo_pickup_sys->pickups == NULL) {
        return;
    }
    pickup = WORLD->gizmo_pickup_sys->pickups;
    for (i32 index = 0; index < WORLD->gizmo_pickup_sys->pickup_count; ++index, ++pickup) {
        if ((pickup->state_flags & 8) == 0 && pickup->activation_group == id) {
            pickup->state_enabled = visible;
            pickup->state_visible = visible;
            pickup->state_activated = visible;
        }
    }
}

static void GizAction_SetGizmoVisibility(GIZFLOW_s *flow, FLOWBOX_s *, char **params, int count) {
    char *name = NULL;
    i32 type = -1;
    i32 visible = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL) {
            name = value + 5;
        } else if ((value = NuStrIStr(params[index], "type=")) != NULL) {
            type = GizmoGetTypeIDByName(flow->gizmo_sys, value + 5);
        } else if (NuStrIStr(params[index], "FALSE") != NULL) {
            visible = 0;
        }
    }
    if (name != NULL) {
        GIZMO *gizmo = GizmoFindByName(flow->gizmo_sys, type, name);
        if (gizmo != NULL)
            GizmoSetVisibility(flow->gizmo_sys, gizmo, visible, 1);
    }
}

static void GizAction_SetVisibility(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    nuhspecial_s special = {};
    i32 visible = 1;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "name=");
        if (value != NULL)
            NuSpecialFind(WORLD->current_gscn, &special, value + 5, 1);
        else if (NuStrIStr(params[index], "FALSE") != NULL)
            visible = 0;
    }
    if (NuSpecialExistsFn(&special))
        NuSpecialSetVisibility(&special, visible);
}

static void GizActions_CompleteLevel(GIZFLOW_s *, FLOWBOX_s *, char **params, int count) {
    if (netclient != 0) {
        return;
    }
    char *cutscene_name = NULL;
    LEVELDATA *level = NULL;
    for (i32 index = 0; index < count; ++index) {
        char *value = NuStrIStr(params[index], "cutscene=");
        if (value != NULL) {
            cutscene_name = value + NuStrLen("cutscene=");
        } else if ((value = NuStrIStr(params[index], "newlevel=")) != NULL) {
            level = Level_FindByName(value + NuStrLen("newlevel="), NULL);
        }
    }
    if (FreePlay == 0 && cutscene_name != NULL && NewCutScene(NULL, WORLD->cutscene_sys, cutscene_name, 1) != NULL) {
        return;
    }
    if (FreePlay == 0 && level != NULL) {
        GoToNewLevel(level->idx);
    } else {
        CompleteLevel(WORLD);
    }
}

static GIZACTIONDEFN_s game_gizactiondefs[] = {
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

void GameRegisterGizActions(void) {
    RegisterGizActions(game_gizactiondefs);
}
