#include "MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/common.h"
#include "nu2api/numath/nuvec.h"

f32 CalcCapsuleIntersectDistance(VuVec const &, VuVec const &, f32, VuVec const &, f32);

i32 MechInputTouchSystem::s_baseControlMode = 1;
i32 MechInputTouchSystem::s_actualTouchMode = 2;

char const *MechInputTouchSystem::GetName() {
    return "MechInputTouchSystem";
}

void MechAutoJumpGetBest(JumpTriggerPacket const &, i32) {
}

void MechAutoJumpSetIsUsing(GameObject_s &, MechAutoJumpConnection &) {
}

void MechTouchUITagButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &) {
}

void MechTouchUIPauseButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &) {
}

void MechTouchUIPartySelector_OnRelease_Callback(MechTouchUIElement &, TouchHolder &) {
}

void MechInputTouchSystem::AddChangeLayoutButtons(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::ChooseTouchLayout(bool) {
}

void MechInputTouchSystem::ConvertToScreenCoords(float, float, float &, float &) {
}

void MechInputTouchSystem::CouldTouchBeLockedBy(u32, MechInputTouchButton *) {
}

void MechInputTouchSystem::CreateGamePanels() {
}

void MechInputTouchSystem::CreateGamePlayLayoutBlank(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutConsoleMode(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutGestureBased(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutGestureBased_Cavalry(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutGestureBased_DeathStarTurret(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutGestureBased_Podrace(NuVirtualTouchDevice &, i32) {
}

void MechInputTouchSystem::CreateGamePlayLayoutGestureBased_SpeederChase(NuVirtualTouchDevice &, i32) {
}

f32 MechInputTouchSystem::DetermineMoveDir2D(GameObject_s &object, VuVec const &target, bool flatten,
                                             VuVec &direction) {
    VuVec object_position(object.apiobj.collision_position.x, object.apiobj.collision_position.y,
                          object.apiobj.collision_position.z, 1.0f);
    VuVec object_screen;
    NuCameraTransformScreenClip(&object_screen.xyz, &object_position.xyz, 1, NULL);

    VuVec target_position = target;
    if (flatten) {
        target_position.y = object_position.y;
    }

    VuVec target_screen;
    NuCameraTransformScreenClip(&target_screen.xyz, &target_position.xyz, 1, NULL);

    direction = VuVec(target_screen.x - object_screen.x, target_screen.y - object_screen.y, 0.0f, 0.0f);

    f32 magnitude = NuVecMag(&direction.xyz);
    if (magnitude > 0.001f) {
        f32 scale = 1.0f / magnitude;
        direction.x *= scale;
        direction.y *= scale;
        direction.z *= scale;
    } else {
        direction = VuVec(0.0f, 0.0f, 0.0f, 1.0f);
    }
    return magnitude;
}

void MechInputTouchSystem::FindTargetForce(WORLDINFO_s *world, GameObject_s &object, VuVec const &start,
                                           VuVec const &direction, f32 &best_distance,
                                           MechObjectInterface *&best_target, bool &found, bool keep_current_target) {
    GIZFORCESYS_s *force_system = world->giz_force_sys;
    if (force_system == NULL || force_system->visible_force_count == 0) {
        return;
    }

    for (i32 index = 0; index < force_system->visible_force_count; ++index) {
        GIZFORCE_s *force = force_system->visible_forces[index];
        if (force == NULL || !TouchHacks::CanUseGizForce(object, *force)) {
            continue;
        }

        if ((force->config_flags & GIZFORCE_CONFIG_TARGET_ANIMATION_OBJECTS) != 0) {
            for (GAMEANIMOBJ_s *animation = force->anim_set->objects; animation != NULL; animation = animation->next) {
                if ((animation->flags & 1) != 0 || (object.force_glow_object != animation && keep_current_target)) {
                    continue;
                }

                NUVEC *draw_position = NuSpecialGetDrawPos(&animation->special);
                if (draw_position == NULL) {
                    continue;
                }

                f32 radius = animation->current_frame;
                if (radius <= 0.0f) {
                    radius = NuSpecialGetOriginRadius(&animation->special);
                }
                VuVec target_position(draw_position->x, draw_position->y, draw_position->z, 1.0f);
                f32 distance = CalcCapsuleIntersectDistance(start, direction, best_distance, target_position, radius);
                if (best_distance > distance) {
                    best_distance = distance;
                    static_cast<GizForceObjectInterface *>(force->GetMechObjectInterface())->selected_object =
                        animation;
                    best_target = force->GetMechObjectInterface();
                    found = true;
                }
            }
            continue;
        }

        if (object.force_glow_object != force && keep_current_target) {
            continue;
        }

        f32 radius = force->strength_0x6c > 0.0f ? force->strength_0x6c : force->radius;
        radius = MAX(0.2f, radius);
        VuVec target_position(force->position.x, force->position.y, force->position.z, 1.0f);
        f32 distance = CalcCapsuleIntersectDistance(start, direction, best_distance, target_position, radius);
        if (best_distance > distance) {
            best_distance = distance;
            static_cast<GizForceObjectInterface *>(force->GetMechObjectInterface())->selected_object = NULL;
            best_target = force->GetMechObjectInterface();
            found = true;
        }
    }
}

void MechInputTouchSystem::FindTargetObject(GameObject_s &, VuVec const &, i32, MechObjectInterface *,
                                            MechTempPosInterface *) {
}

void MechInputTouchSystem::Init() {
}

MechInputTouchSystem::MechInputTouchSystem() {
}

void MechInputTouchSystem::ProcessEvenWhenPaused(ThingProcessData *) {
}

void MechInputTouchSystem::ResetAllOwners() {
}

void MechInputTouchSystem::SetTouchLockedBy(u32, MechInputTouchButton *, bool) {
}

void MechInputTouchSystem::TouchLockedBy(u32) {
}
