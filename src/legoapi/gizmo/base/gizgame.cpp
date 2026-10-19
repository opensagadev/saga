#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/actions/combat/hits.h"
#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/gizmos/fx/guidelines.h"
#include "legoapi/gizmos/transport/ledges.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/gizmos/traps/shards.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/props/objects/tightrope.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include <stdio.h>
#include <string.h>
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"

// Game-side gizmo registration and bolt targeting (retail gizgame.cpp).

// this technically has the wrong linkage, it does not have a C++ mangled name in the original binary... maybe should be
// in a different file?
static REGISTERGIZMOTYPEFN GizmoTypesLSW[] = {GizObstacles_RegisterGizmo,
                                              GizBuildIts_RegisterGizmo,
                                              GizForce_RegisterGizmo,
                                              NewBlowup_RegisterGizmo,
                                              GizmoPickups_RegisterGizmo,
                                              Levers_RegisterGizmo,
                                              Spinner_RegisterGizmo,
                                              MiniCut_RegisterGizmo,
                                              Tubes_RegisterGizmo,
                                              ZipUps_RegisterGizmo,
                                              GizTurrets_RegisterGizmo,
                                              GizBombGen_RegisterGizmo,
                                              AI_RegisterGizmo,
                                              GizSpecial_RegisterGizmo,
                                              GizAIMessage_RegisterGizmo,
                                              GizTimer_RegisterGizmo,
                                              GizRandom_RegisterGizmo,
                                              GizPanel_RegisterGizmo,
                                              HatMachine_RegisterGizmo,
                                              Push_RegisterGizmo,
                                              Door_RegisterGizmo,
                                              Teleport_RegisterGizmo,
                                              GizTorpMachine_RegisterGizmo,
                                              EdGizShadowMachine_RegisterGizmo,
                                              Portal_RegisterGizmo,
                                              Grapples_RegisterGizmo,
                                              Plugs_RegisterGizmo,
                                              Technos_RegisterGizmo,
                                              NULL};

#define GIZMO_TYPES_LSW_COUNT ((sizeof(GizmoTypesLSW) / sizeof(REGISTERGIZMOTYPEFN)) - 1)

static REGISTERGIZMOTYPEFN GizmoTypesBatman[] = {
    GizObstacles_RegisterGizmo,
    GizBuildIts_RegisterGizmo,
    NewBlowup_RegisterGizmo,
    GizmoPickups_RegisterGizmo,
    Shards_RegisterGizmo,
    Signals_RegisterGizmo,
    Grapples_RegisterGizmo,
    TightRopes_RegisterGizmo,
    Ledges_RegisterGizmo,
    Levers_RegisterGizmo,
    Spinner_RegisterGizmo,
    Technos_RegisterGizmo,
    SecurityDoors_RegisterGizmo,
    Attractos_RegisterGizmo,
    MiniCut_RegisterGizmo,
    GuideLines_RegisterGizmo,
    Tubes_RegisterGizmo,
    ZipUps_RegisterGizmo,
    GizTurrets_RegisterGizmo,
    AI_RegisterGizmo,
    GizTimer_RegisterGizmo,
    GizRandom_RegisterGizmo,
    GizSpecial_RegisterGizmo,
    Door_RegisterGizmo,
    GizAIMessage_RegisterGizmo,
    Push_RegisterGizmo,
    EdGizShadowMachine_RegisterGizmo,
    Portal_RegisterGizmo,
    NULL,
};

#define GIZMO_TYPES_BATMAN_COUNT ((sizeof(GizmoTypesBatman) / sizeof(REGISTERGIZMOTYPEFN)) - 1)

i32 GizObstacle_CheckExcludeFlagsFn_LSW(GIZOBSTACLE_s *obstacle, GameObject_s *object) {
    if ((obstacle->field_0x6c & 1) != 0 &&
        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field275_0x116 != 10) {
        return 1;
    }
    if ((obstacle->field_0x6c & 2) != 0 && object->apiobj.character_data->move_fn != Move_BEAST) {
        return 1;
    }
    return 0;
}

void RegisterGizmoTypes_Indy(variptr_u *buffer, variptr_u *buffer_end) {
    REGISTERGIZMOTYPEFN gizmo_types[] = {
        GizObstacles_RegisterGizmo, GizBuildIts_RegisterGizmo,
        NewBlowup_RegisterGizmo,    GizmoPickups_RegisterGizmo,
        Grapples_RegisterGizmo,     TightRopes_RegisterGizmo,
        Levers_RegisterGizmo,       Spinner_RegisterGizmo,
        Technos_RegisterGizmo,      MiniCut_RegisterGizmo,
        Tubes_RegisterGizmo,        ZipUps_RegisterGizmo,
        GizTurrets_RegisterGizmo,   AI_RegisterGizmo,
        GizTimer_RegisterGizmo,     GizRandom_RegisterGizmo,
        GizSpecial_RegisterGizmo,   Door_RegisterGizmo,
        Plugs_RegisterGizmo,        GizAIMessage_RegisterGizmo,
        Push_RegisterGizmo,         EdGizShadowMachine_RegisterGizmo,
        Portal_RegisterGizmo,       NULL,
    };
    RegisterGizmoTypes(buffer, buffer_end, gizmo_types, 12);
}

void RegisterGizmoTypes_Batman(VARIPTR *buffer, VARIPTR *buffer_end) {
    REGISTERGIZMOTYPEFN gizmo_types[GIZMO_TYPES_BATMAN_COUNT + 1];
    memcpy(gizmo_types, GizmoTypesBatman, sizeof(GizmoTypesBatman));
    RegisterGizmoTypes(buffer, buffer_end, gizmo_types, 12);
}

void RegisterGizmoTypes_LSW(VARIPTR *buffer, VARIPTR *buffer_end) {
    REGISTERGIZMOTYPEFN gizmo_types[GIZMO_TYPES_LSW_COUNT + 1];
    memcpy(gizmo_types, GizmoTypesLSW, sizeof(GizmoTypesLSW));
    RegisterGizmoTypes(buffer, buffer_end, gizmo_types, 12);
}

i32 GizmoSys_SetBestBoltTarget(GIZMOSYS_s *system, void *, GameObject_s *object, nuvec_s *position, nuvec_s *direction,
                               f32 radius, f32 range_squared, i32 directional, i32 planar, i32 bolt_id) {
    if (gizmotypes == NULL || object == NULL || system == NULL)
        return 0;
    if ((ObjHitObj_Flags(object) & 0x800) == 0)
        return 0;
    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = system->sets;
    void *best = NULL;
    void *previous = NULL;
    f32 best_distance = 1.0e9f;
    NUVEC best_position, best_velocity, previous_position, previous_velocity;
    for (i32 i = 0; i < gizmotypes->count; ++i, ++type, ++set) {
        if (type->fns.get_best_bolt_target_fn == NULL)
            continue;
        f32 distance;
        NUVEC target_position, target_velocity;
        void *target =
            type->fns.get_best_bolt_target_fn(set, &distance, &target_position, &target_velocity, object, position,
                                              direction, radius, range_squared, directional, planar, bolt_id);
        if (target == NULL)
            continue;
        if (target == object->attack_gizmo_target) {
            previous = target;
            previous_position = target_position;
            previous_velocity = target_velocity;
        } else if (best_distance > distance) {
            best = target;
            best_position = target_position;
            best_velocity = target_velocity;
            best_distance = distance;
        }
    }
    if (best != NULL) {
        object->attack_target_position = best_position;
        object->attack_target_velocity = best_velocity;
        object->attack_gizmo_target = VehicleArea != 0 ? NULL : best;
        object->field_0xe21 |= 8;
        return 1;
    }
    if (previous != NULL) {
        object->attack_target_position = previous_position;
        object->attack_target_velocity = previous_velocity;
        object->attack_gizmo_target = VehicleArea != 0 ? NULL : previous;
        object->field_0xe21 |= 8;
        return 1;
    }
    return 0;
}

i32 GizmoSys_BoltHit(GIZMOSYS_s *gizmo_sys, void *world_info, BOLT_s *bolt, nuvec_s *points, nuvec_s *minimum,
                     nuvec_s *maximum, float radius, unsigned char *hit_flags) {
    nuvec_s *hit_points;
    i32 hit_mode;
    if ((bolt->flags & 0x200) != 0) {
        hit_points = points + 1;
        hit_mode = 1;
    } else {
        hit_points = points;
        hit_mode = 3;
    }
    if (gizmotypes == NULL || gizmo_sys == NULL || (bolt->hit_flags & 0x800) == 0) {
        return 0;
    }

    GIZMOTYPE *type = gizmotypes->types;
    GIZMOSET *set = gizmo_sys->sets;
    for (i32 index = 0; index < gizmotypes->count; ++index, ++set, ++type) {
        if (type->fns.bolt_hit_fn != NULL &&
            __builtin_expect(type->fns.bolt_hit_fn(world_info, set->unknown, bolt->owner, hit_points, hit_mode, radius,
                                                   minimum, maximum, bolt, 1, hit_flags) != 0,
                             0)) {
            BoltSys->debris(bolt, points, -1, NULL, 0);
            if (bolt->owner != NULL) {
                NewRumble(bolt->owner->pad_gamepad->pad, 0.6f, 0);
            }
            Bolt_End(bolt, 1);
            Bolt_PlayHitSfx(bolt);
            return 1;
        }
    }
    return 0;
}

void GizmoSysSetGame() {
    GizObstacle_CheckExcludeFlagsFn = GizObstacle_CheckExcludeFlagsFn_LSW;
}
