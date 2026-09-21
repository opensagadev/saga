#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
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

static void GizActions_ActivateBelt(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizActions_PlayCutscene(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_PlayRadio(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizActions_PlayObstacle(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizAction_SetAIState(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizAction_ChangeTechnoTgt(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizAction_ActivatePartEffect(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizAction_SetPickupVisibility(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizActions_CompleteLevel(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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
