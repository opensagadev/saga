#include "decomp.h"
#include "gamelib_util_types.h"

#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/render/core/terrain.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"

f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
i32 SuperWeirdo(GameObject_s *);
i32 GizForce_StoodOnForce(GIZFORCE_s *, GameObject_s *);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);

NUCOLOUR3 flashCol = {2.0f, 2.0f, 2.0f};
bool TouchHacks::TouchControlsActive;
extern i32 BonusArea;
extern "C" i16 id_GRABCONTROL, id_WICKET, id_EWOK;
extern "C" i16 id_ATST, id_ATST_LOWRES;
extern "C" i16 id_WATTO, id_GONKDROID;

bool TouchHacks::AiPlayerTakeDamageOnKillRescue(GameObject_s &) {
    return TouchControlsActive;
}

VuVec TouchHacks::CalculateJumpVelToHitPoint(GameObject_s &object, VuVec const &target) {
    const GAMECHARACTERDATA *character = object.apiobj.character_data->game_character;
    const VuVec position(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
    return CalculateXZVelForArcToHitPoint(position, target, character->jump_speed, character->gravity);
}

VuVec TouchHacks::CalculateJumpVelToHitPointDblJump(GameObject_s &object, VuVec const &target) {
    const GAMECHARACTERDATA *character = object.apiobj.character_data->game_character;
    const GAMECHARACTERDATA *player_character = player->apiobj.character_data->game_character;
    const f32 player_jump_height = -(player_character->jump_speed * player_character->jump_speed) /
                                   (player_character->gravity + player_character->gravity);
    const f32 height = -(player_jump_height * 0.7f);
    const f32 half_gravity = character->gravity * 0.5f;
    f32 first_time;
    f32 unused_time;
    if (!SolveRoot(half_gravity, character->jump_speed, height, first_time, unused_time)) {
        return VuVec_Zero;
    }

    f32 second_time;
    if (!SolveRoot(half_gravity, character->jump_speed, height, second_time, unused_time)) {
        return VuVec_Zero;
    }

    const f32 total_time = first_time + second_time;
    return VuVec((target.x - object.apiobj.position.x) / total_time, 0.0f,
                 (target.z - object.apiobj.position.z) / total_time, 1.0f);
}

VuVec TouchHacks::CalculateXZVelForArcToHitPoint(VuVec const &position, VuVec const &target, float jump_speed,
                                                 float gravity) {
    f32 vertical_distance;
    if (position.y > target.y) {
        vertical_distance = target.y - position.y;
    } else {
        vertical_distance = position.y - target.y;
    }
    vertical_distance = -vertical_distance;

    f32 first_time;
    f32 second_time;
    VuVec velocity = VuVec_Zero;
    if (SolveRoot(gravity * 0.5f, jump_speed, vertical_distance, first_time, second_time)) {
        velocity.x = (target.x - position.x) / first_time;
        velocity.z = (target.z - position.z) / first_time;
    }
    return velocity;
}

i32 TouchHacks::CanBlowupBeBlownUp(GIZMOBLOWUP_s &blowup, i32 hit_type) {
    if (hit_type != 1) {
        return 1;
    }
    return (blowup.draw_flags >> 7) & 1;
}

bool TouchHacks::CanForceTargetObj(GameObject_s &source, GameObject_s &target) {
    return !TouchControlsActive || !CanTagTo(source, target);
}

bool TouchHacks::CanJump(GameObject_s &object) {
    return (object.apiobj.field_0x27d != 0 || object.ground_contact_grace_timer > 0.0f) &&
           object.apiobj.character_model != NULL && ObjLandReady(&object) &&
           (object.apiobj.character_model->model_data_b[6] != NULL ||
            (object.apiobj.character_data->model_flags & 0x40) != 0 || object.id == id_WATTO ||
            (object.id == id_GONKDROID && Cheat_IsOn(8)));
}

bool TouchHacks::CanJumpToPoint(GameObject_s &object, AIPATHNODE_s const &node) {
    VuVec direction(node.position.x - object.apiobj.position.x, node.position.y - object.apiobj.position.y,
                    node.position.z - object.apiobj.position.z, 1.0f);
    const f32 distance = NuVecMag(&direction.xyz) - node.radius;
    NuVecNorm(&direction.xyz, &direction.xyz);
    const VuVec target(object.apiobj.position.x + direction.x * distance,
                       object.apiobj.position.y + direction.y * distance,
                       object.apiobj.position.z + direction.z * distance, 1.0f);
    return CanJumpToPoint(object, target);
}

bool TouchHacks::CanJumpToPoint(GameObject_s &object, VuVec const &target) {
    const GAMECHARACTERDATA *character = object.apiobj.character_data->game_character;
    f32 first_time;
    f32 second_time;
    if (!SolveRoot(character->gravity * 0.5f, character->jump_speed, object.apiobj.position.y - target.y, first_time,
                   second_time)) {
        return false;
    }

    const f32 dx = target.x - object.apiobj.position.x;
    const f32 dz = target.z - object.apiobj.position.z;
    const f32 range = first_time * 1.4f;
    return range * range > dx * dx + dz * dz;
}

bool TouchHacks::CanLunge(GameObject_s &object) {
    CHARACTERDATA *character = object.apiobj.character_data;
    return (character->game_character->field275_0x116 != 0 || (character->model_flags & 8) != 0) &&
           LEGOACT_LUNGE != -1 && object.apiobj.character_model->model_data_b[LEGOACT_LUNGE] != NULL;
}

bool TouchHacks::CanPoo(GameObject_s &object) {
    return (object.apiobj.character_data->game_character->flags_094[3] & 0x80) != 0 && object.character_context == -1 &&
           object.apiobj.field_0x27d != 0 && (object.apiobj.flags_low & 0x80) != 0 &&
           (Cheat[1].enabled != 0 || Cheat[9].enabled != 0);
}

bool TouchHacks::CanShoot(GameObject_s &object) {
    CHARACTERDATA *character = object.apiobj.character_data;
    return (character->model_flags & 0x10000000) != 0 && (character->game_character->flags_094[0] & 8) == 0;
}

bool TouchHacks::CanSlam(GameObject_s &object) {
    return LEGOACT_SLAM != -1 && object.apiobj.character_model->model_data_b[LEGOACT_SLAM] != NULL;
}

bool TouchHacks::CanTagTo(GameObject_s &source, GameObject_s &target) {
    if (&source == &target || (target.apiobj.field_0x1f8 & 0x1001) != 0x1001 || target.apiobj.field_0x287 != 0 ||
        (target.tag_flags & 2) != 0) {
        return false;
    }

    const i8 context = target.character_context;
    if (context == 0x3d || context == 0x17 || context < 0 || (CInfo[context].flags & 0x8000) != 0) {
        return false;
    }
    if ((target.field_0xf00 & 2) != 0 && (player == NULL || player->id != id_LUKESKYWALKERDAGOBAH)) {
        return false;
    }
    if (target.apiobj.model_draw_result == 0) {
        return false;
    }
    if ((target.apiobj.field_0x1f4 & 5) != 0 && (HUB_ADATA == NULL || WORLD == NULL || WORLD->area != HUB_ADATA)) {
        return false;
    }
    if (static_cast<i32>(target.apiobj.field_0x1f4) < 0) {
        return false;
    }

    const f32 vertical_distance = NuFabs(target.apiobj.position.y - source.apiobj.position.y);
    const f32 maximum_height = target.apiobj.scaled_height > source.apiobj.scaled_height ? target.apiobj.scaled_height
                                                                                         : source.apiobj.scaled_height;
    if (vertical_distance > maximum_height) {
        return false;
    }
    const f32 dx = source.apiobj.position.x - target.apiobj.position.x;
    const f32 dy = source.apiobj.position.y - target.apiobj.position.y;
    const f32 dz = source.apiobj.position.z - target.apiobj.position.z;
    return dx * dx + dy * dy + dz * dz <= 4.0f;
}

void Move_CHARACTER(GameObject_s *);
void Move_WEIRDO(GameObject_s *);
void Move_JEDI(GameObject_s *);
void Move_DROIDGENERIC(GameObject_s *);
void Move_JAWA(GameObject_s *);
void Move_GEONOSIAN(GameObject_s *);
extern i16 id_SKELETON, id_GRIEVOUS, id_BODYGUARD, id_ATAT, id_STAP, id_STAP2;

bool TouchHacks::CanTagVehicle(GameObject_s &object, GameObject_s &vehicle) {
    CHARACTERDATA *character = object.apiobj.character_data;
    if (character->move_fn != Move_CHARACTER && character->move_fn != Move_WEIRDO && character->move_fn != Move_JEDI &&
        character->move_fn != Move_DROIDGENERIC && character->move_fn != Move_JAWA &&
        character->move_fn != Move_GEONOSIAN) {
        return false;
    }
    if ((vehicle.apiobj.character_data->model_flags & 0x40000000) != 0 &&
        (vehicle.character_context == 0x17 || vehicle.character_context == 0x3e)) {
        return false;
    }
    if (vehicle.field_0xcc0 != NULL || (character->model_flags & 0x10) != 0 || object.id == id_SKELETON ||
        object.id == id_GRIEVOUS || object.id == id_BODYGUARD) {
        return false;
    }
    if (vehicle.id != id_ATST && vehicle.id != id_ATST_LOWRES && vehicle.id != id_ATAT && vehicle.id != id_STAP &&
        vehicle.id != id_STAP2 &&
        fabsf(vehicle.apiobj.position.y - object.apiobj.position.y) > object.apiobj.scaled_height) {
        return false;
    }
    const f32 x = object.apiobj.position.x - vehicle.apiobj.position.x;
    const f32 y = object.apiobj.position.y - vehicle.apiobj.position.y;
    const f32 z = object.apiobj.position.z - vehicle.apiobj.position.z;
    return !(x * x + y * y + z * z > 4.0f);
}

bool TouchHacks::CanThrowBountyBomb(GameObject_s &object) {
    if (WORLD->lev_objs[0xe9].active == 0 || static_cast<i8>(object.apiobj.flags_low) >= 0) {
        return false;
    }
    if ((object.apiobj.character_data->model_flags & 0x01000000) == 0 && object.field_0x108e != 6 &&
        SuperWeirdo(&object) == 0) {
        return false;
    }
    if (object.apiobj.field_0x27d == 0 && object.field_0xe31 != 1) {
        return false;
    }

    const i8 context = object.character_context;
    return context == 6 || context == -1 || context == 7 || (CInfo[context].flags & 4) != 0;
}

void Move_DEFAULT(GameObject_s *);

bool TouchHacks::CanToggleTo(GameObject_s &object, i32 id) {
    if (object.id == id)
        return false;
    if ((CInfo[object.character_context].flags & 0x100) != 0)
        return false;
    if (object.apiobj.field_0x27f <= 16 && (TerLayer[static_cast<i8>(object.apiobj.field_0x27f)].flags & 1) != 0 &&
        GCDataList[id].field_0x28 <= 0.0f)
        return false;
    if (object.apiobj.field_0x218 != 2000000.0f && object.apiobj.field_0x220 != 2000000.0f &&
        object.apiobj.character_data->move_fn != Move_DEFAULT &&
        CDataList[id].bounds_max_y - CDataList[id].bounds_min_y >=
            object.apiobj.field_0x220 - object.apiobj.field_0x218)
        return false;
    return true;
}

bool TouchHacks::CanUseBuildIt(GameObject_s &object) {
    return LEGOACT_BUILD != -1 && object.apiobj.character_model != NULL &&
           object.apiobj.character_model->model_data_b[LEGOACT_BUILD] != NULL &&
           !AnimPlaying(&object.apiobj.anim_packet, LEGOACT_BUILD, 1, 1) && object.apiobj.field_0x27d != 0 &&
           ObjLandReady(&object) != 0;
}

bool TouchHacks::CanUseGizForce(GameObject_s &object) {
    return object.apiobj.character_data != NULL && (object.apiobj.character_data->model_flags & 8) != 0;
}

bool TouchHacks::CanUseGizForce(GameObject_s &object, GIZFORCE_s &force) {
    if (force.using_object != NULL || force.field_0x3c_bits != 0 ||
        (force.state_flags & GIZFORCE_STATE_DESTROYED_OR_THROWN) != 0) {
        return false;
    }

    i32 can_use_restricted_force;
    if (SuperWeirdo(&object) == 0) {
        if (static_cast<i8>(object.apiobj.flags_low) < 0 && Cheat_IsOn(25) != 0) {
            can_use_restricted_force = 1;
        } else {
            can_use_restricted_force = 0;
        }
    } else {
        can_use_restricted_force = 1;
    }
    if ((force.config_flags & GIZFORCE_CONFIG_JEDI_BADDIE_ONLY) != 0 &&
        (object.apiobj.character_data->model_flags & 4) == 0 && can_use_restricted_force == 0) {
        return false;
    }

    if (force.group == NULL) {
        if (GizForce_Complete(&force) != 0) {
            return false;
        }
    } else if (force.group->count != 0) {
        GIZFORCE_s *last = force.group->forces[force.group->count - 1];
        if (last != &force) {
            if ((force.group->field_0x24 & GIZFORCE_GROUP_ACTIVE) != 0 ||
                force.anim_set->state == GAMEANIMSET_STATE_AT_END) {
                return false;
            }
            if (last != NULL && ((last->anim_set->flags & 7) != 0 || last->using_object != NULL)) {
                return false;
            }
        }
    }

    return GizForce_StoodOnForce(&force, &object) == 0;
}

bool TouchHacks::CanUseHatMachine(GameObject_s &object) {
    return object.apiobj.character_model->model_data_b[93] != NULL && object.apiobj.field_0x27d != 0 &&
           ObjLandReady(&object) != 0;
}

bool TouchHacks::CanUseLever(GameObject_s &object) {
    return object.apiobj.character_model->model_data_b[93] != NULL && object.apiobj.field_0x27d != 0;
}

bool TouchHacks::CanUseTeleport(GameObject_s &object) {
    return object.apiobj.character_data != NULL &&
           ((object.apiobj.character_data->model_flags & 0x40000) != 0 || SuperWeirdo(&object));
}

bool TouchHacks::CanUseVehicleSmartBomb(GameObject_s &object) {
    return Cheat_IsOn(20) && (object.apiobj.flags_low & 0x80) != 0 &&
           (object.apiobj.character_data->model_flags & 0x2000) != 0 && InCollectList_Index(object.id, NULL, 0) != -1;
}

bool TouchHacks::CanUseZipup(GameObject_s &object) {
    extern i32 ObjLandReady(GameObject_s *);
    extern i32 SuperWeirdo(GameObject_s *);
    extern i32 Cheat_IsOn(i32);
    if (object.apiobj.character_data == NULL || !ObjLandReady(&object))
        return false;
    if ((object.apiobj.character_data->model_flags & 0x100000) != 0 || SuperWeirdo(&object))
        return true;
    return (object.apiobj.character_data->model_flags & 8) != 0 &&
           (object.apiobj.character_data->game_character->flags_094[1] & 0x80) == 0 && Cheat_IsOn(13) != 0;
}

bool TouchHacks::CheckForAboutToRunIntoKillTerrain(GameObject_s &object, float time) {
    if (WORLD->current_level != SPEEDERCHASEA_LDATA) {
        const f32 dx = object.apiobj.velocity.x * time;
        const f32 dz = time * object.apiobj.velocity.z;
        VuVec position(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
        position.x = dx + position.x;
        position.z = dz + position.z;
        position.y += 0.3f;
        if (GameShadow(&object, reinterpret_cast<NUVEC *>(&position), 5.0f, -1) == 2000000.0f)
            return false;
        u32 layer = EShadowInfo();
        if (layer > 16 || (TerLayer[layer].flags & 1) == 0)
            return false;
        VuVec direction(object.apiobj.velocity.x, 0.0f, object.apiobj.velocity.z, 1.0f);
        NuVecNorm(reinterpret_cast<NUVEC *>(&direction), reinterpret_cast<NUVEC *>(&direction));
        const f32 radius = object.apiobj.collision_radius * 0.8f;
        position.x += direction.x * radius;
        position.z += radius * direction.z;
        if (GameShadow(&object, reinterpret_cast<NUVEC *>(&position), 5.0f, -1) == 0.0f)
            return true;
        layer = EShadowInfo();
        return layer > 16 || (TerLayer[layer].flags & 1) != 0;
    }
    return false;
}

bool TouchHacks::CheckForAboutToRunOffAnEdge(GameObject_s &object, float time) {
    VuVec position(object.apiobj.position.x + object.apiobj.velocity.x * time, object.apiobj.position.y + 0.3f,
                   object.apiobj.position.z + object.apiobj.velocity.z * time, 1.0f);
    const f32 minimum_height = object.apiobj.position.y - 0.3f;
    if (minimum_height <= GameShadow(&object, &position.xyz, 5.0f, -1)) {
        return false;
    }

    VuVec direction(object.apiobj.velocity.x, 0.0f, object.apiobj.velocity.z, 1.0f);
    NuVecNorm(&direction.xyz, &direction.xyz);
    const f32 radius = object.apiobj.collision_radius * 0.8f;
    position.x += direction.x * radius;
    position.z += direction.z * radius;
    return GameShadow(&object, &position.xyz, 5.0f, -1) < minimum_height;
}

bool TouchHacks::CheckJumpForLandingSpot(GameObject_s &object, float maximum_drop) {
    VuVec position(object.apiobj.position.x, object.apiobj.position.y, object.apiobj.position.z, 1.0f);
    const f32 minimum_height = object.apiobj.position.y - maximum_drop;
    const GAMECHARACTERDATA *character = object.apiobj.character_data->game_character;
    const f32 step_x = object.apiobj.velocity.x * 0.2f;
    const f32 step_z = object.apiobj.velocity.z * 0.2f;
    f32 vertical_velocity = character->jump_speed + object.apiobj.velocity.y;

    while (position.y >= minimum_height) {
        const VuVec next(position.x + step_x, position.y + vertical_velocity * 0.2f, position.z + step_z, 1.0f);
        VuVec displacement(next.x - position.x, next.y - position.y, next.z - position.z, 1.0f);
        if (GameRayCast(&position.xyz, &displacement.xyz, 0.0f, 0) != 0) {
            VuVec normal = VuVec_Zero;
            NewRayCastGetImpactNormal(&normal.xyz);
            if (normal.y > 0.8f && GameShadow(&object, &position.xyz, 5.0f, -1) != 2000000.0f) {
                const u32 layer = EShadowInfo();
                if (layer <= 16 && (TerLayer[layer].flags & 1) == 0) {
                    return true;
                }
            }
        }
        position = next;
        vertical_velocity += character->gravity * 0.2f;
    }
    return false;
}

void TouchHacks::CleanupAllMechObjectInterfaces(WORLDINFO_s *world) {
    if (world == NULL) {
        return;
    }

    for (i32 i = 0; i < world->gizmo_blowup_count; ++i) {
        world->gizmo_blowups[i].ClearMechObjectInterface();
    }
    if (world->giz_buildit_sys != NULL) {
        for (i32 i = 0; i < world->giz_buildit_sys->count; ++i) {
            world->giz_buildit_sys->buildits[i].ClearMechObjectInterface();
        }
    }
    for (i32 i = 0; i < world->nlevers; ++i) {
        world->levers[i].ClearMechObjectInterface();
    }
    if (world->hat_machine_sys != NULL) {
        for (i32 i = 0; i < world->hat_machine_sys->count; ++i) {
            world->hat_machine_sys->machines[i].ClearMechObjectInterface();
        }
    }
    for (i32 i = 0; i < world->teleport_count; ++i) {
        world->teleports[i].ClearMechObjectInterface();
    }
    if (world->giz_panel_sys != NULL) {
        for (i32 i = 0; i < world->giz_panel_sys->count; ++i) {
            world->giz_panel_sys->panels[i].ClearMechObjectInterface();
        }
    }
    if (world->giz_turret_sys != NULL) {
        for (i32 i = 0; i < world->giz_turret_sys->count; ++i) {
            world->giz_turret_sys->turrets[i].ClearMechObjectInterface();
        }
    }
    if (WORLD != NULL && WORLD->giz_obstacle_sys != NULL) {
        for (i32 i = 0; i < WORLD->giz_obstacle_sys->count; ++i) {
            WORLD->giz_obstacle_sys->obstacles[i].ClearMechObjectInterface();
        }
    }
    if (world->giz_force_sys != NULL) {
        for (i32 i = 0; i < world->giz_force_sys->count; ++i) {
            world->giz_force_sys->forces[i].ClearMechObjectInterface();
        }
    }
    if (Part != NULL) {
        for (i32 i = 0; i < MAXPARTS; ++i) {
            Part[i].ClearMechObjectInterface();
        }
    }
}

MechObjectInterface *TouchHacks::FindBombTarget(GameObject_s &object) {
    const VuVec forward(NU_SIN_LUT(object.apiobj.facing_angle), 0.0f, NU_COS_LUT(object.apiobj.facing_angle), 1.0f);
    MechObjectInterface *result = NULL;
    f32 best_alignment = 0.5f;
    GIZMOBLOWUP_s *blowup = WORLD->gizmo_blowups;
    if (blowup != NULL) {
        for (i32 i = 0; i < WORLD->gizmo_blowup_count; ++i, ++blowup) {
            if ((blowup->state_flags & 0x80) == 0 || blowup->type == NULL || (blowup->output_flags & 1) != 0 ||
                (blowup->type->type_flags & 0x02000000) == 0) {
                continue;
            }
            VuVec direction(blowup->mid_position.x - object.apiobj.position.x,
                            blowup->mid_position.y - object.apiobj.position.y,
                            blowup->mid_position.z - object.apiobj.position.z, 1.0f);
            const f32 squared_distance =
                direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
            if (squared_distance < 90000.0f) {
                NuVecNorm(&direction.xyz, &direction.xyz);
                const f32 alignment = direction.x * forward.x + direction.y * forward.y + direction.z * forward.z;
                if (alignment > best_alignment) {
                    result = blowup->GetMechObjectInterface();
                    best_alignment = alignment;
                }
            }
        }
    }
    return result;
}

nucolour3_s *TouchHacks::GetFlashColour() {
    return &flashCol;
}

f32 TouchHacks::GetIncomingPartRange() {
    return 16.0f;
}

i32 TouchHacks::GetLoseStudsDieValue() {
    return BonusArea != 0 ? 10000 : 1000;
}

i32 TouchHacks::GetLoseStudsFallValue() {
    return 0;
}

bool TouchHacks::InParty(GameObject_s &object) {
    return (Player[0] != NULL && Player[0] == &object) || (Player[1] != NULL && Player[1] == &object) ||
           (Player[2] != NULL && Player[2] == &object) || (Player[3] != NULL && Player[3] == &object) ||
           (Player[4] != NULL && Player[4] == &object) || (Player[5] != NULL && Player[5] == &object) ||
           (Player[6] != NULL && Player[6] == &object) || (Player[7] != NULL && Player[7] == &object);
}

void TouchHacks::PlaySmartBombBuildupEffects(GameObject_s &, float elapsed, float duration) {
    PlaySfxAndSetVolume("JForceUse", NULL, 3.0f * elapsed / duration);
}

bool TouchHacks::ShouldAutoGrabDragBomb(GameObject_s &object) {
    if (object.id == id_ATST || object.id == id_ATST_LOWRES)
        return false;
    return TouchControlsActive;
}

bool TouchHacks::ShouldBlock(GameObject_s &object) {
    if (TouchControlsActive && object.incoming_melee != NULL && object.apiobj.field_0x27c == -1) {
        return qrand() > 14999;
    }
    return true;
}

CABLE_s *GameObjIsCableTied(GameObject_s *);
i32 TouchHacks::ShouldDeflectBolt(GameObject_s &object, BOLT_s &bolt) {
    if (!TouchControlsActive || VehicleArea == 0)
        return 0;
    if (object.id != id_ATST && object.id != id_ATST_LOWRES)
        return 0;
    if (bolt.owner == NULL || (bolt.owner->apiobj.flags_low & 0x80) == 0)
        return 0;
    CABLE_s *cable = GameObjIsCableTied(&object);
    if (cable == NULL)
        return 0;
    return cable->source == bolt.owner;
}

bool TouchHacks::ShouldFlash(float timer) {
    return timer > 0.0f && NuFmod(timer, 0.3f) < 0.15f;
}

bool TouchHacks::ShouldKeepWeaponOut(GameObject_s &object) {
    return TouchControlsActive && object.id != id_GRABCONTROL && (object.apiobj.flags_low & 0x80) != 0 &&
           object.ai.opponent != NULL && object.character_context == -1;
}

bool TouchHacks::ShouldPutWeaponAway(GameObject_s &object) {
    return TouchControlsActive && object.id != id_GRABCONTROL && object.id != id_WICKET && object.id != id_EWOK &&
           (object.apiobj.flags_low & 0x80) != 0 && object.ai.opponent == NULL && object.weapon_out_timer > 5.0f &&
           object.character_context == -1 && object.field_0xe31 != 1;
}

bool TouchHacks::SolveRoot(float a, float b, float c, float &root1, float &root2) {
    if (a == 0.0f) {
        return false;
    }

    const f32 discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return false;
    }

    const f32 square_root = NuFsqrt(discriminant);
    const f32 denominator = a + a;
    root1 = (-b - square_root) / denominator;
    root2 = (square_root - b) / denominator;
    return true;
}

void TouchHacks::TriggerVehicleSmartBomb(GameObject_s &object) {
    AddExplosion(&object.apiobj.collision_position, 7.5f * AreaPickupScale, 1.0f, NULL, -1, 0x4021);
    NewRumbleAllPlayers(1.0f, 0.1f, 0, 0);
    GameCam_Judder(GameCam, qrand() > 0x7fff ? 1.5f : -1.5f, 2, NULL);
    GameCam_NewShake(GameCam, 2.0f, 1.0f, 1.0f);
    PlaySfx("Explode1", NULL);
    Hint_SetComplete(0x5e1);
}
