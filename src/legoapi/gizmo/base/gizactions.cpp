#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"

static void GizActions_EnableSock(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizActions_ChangeObstTriggerType(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_HitBlowup(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_PlayCutscene(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_PlayRadio(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_PlayForce(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
}

static void GizActions_PlaySpecial(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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

static void GizAction_ActivateEffect(GIZFLOW_s *, FLOWBOX_s *, char **, int) {
    STUBBED();
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
