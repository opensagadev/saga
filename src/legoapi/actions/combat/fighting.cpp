// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/actions/combat/hits.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/actions/character/speederchase.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <stdlib.h>
#include "legoapi/items/collect/torpedo.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numusic/sfx.h"
#include <float.h>
#include <string.h>
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include <stdio.h>
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include <math.h>
#include "legoapi/audio/audio.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/gizmo/base/GizBuildItObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizLeverObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizPanelObjectInterface.h"
#include "legoapi/gizmo/base/GizTurretObjectInterface.h"
#include "legoapi/gizmo/base/HatMachineObjectInterface.h"
#include "legoapi/gizmo/base/TeleportObjectInterface.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "gameframework/saveload.h"
#include "nu2api/nu3d/nuhspecial.h"
#include "legoapi/world/levels/podrace.h"
#include <stdint.h>
#include "nu2api/nucore/nulist.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "editor/edpath.h"
#include "nu2api/nucore/common.h"
#include "editor/aieditor_state.h"
#include "nu2api/numath/nuplane.h"
#include "editor/aieditor_settings.h"
#include "nu2api/nu3d/numtl.h"
#include "editor/edpath_types.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/render/fx/game_deb.h"
#include <stddef.h>
#include <ctype.h>
#include "legoapi/world/areas.h"
#include "nu2api/numusic/numusic.h"
#include <stdarg.h>
#include <limits.h>
#include "legoapi/actions/movement/climbing.h"

void (*Punch_HitHoldFn)(GameObject_s *, GameObject_s *);
i32 (*Punch_GetDamageFn)(GameObject_s *, GameObject_s *);
void (*Punch_HitExtraCodeFn)(GameObject_s *, nuvec_s *);
BLADE_s BladeTab[4] = {
    {101, 223, 1, 59, 64, {255, 31, 0}, {0, 0, 0}},
    {103, 221, 2, 60, 65, {30, 191, 15}, {0, 0, 0}},
    {105, 225, 3, 61, 66, {0, 191, 255}, {0, 0, 0}},
    {107, 227, 4, 62, 67, {200, 0, 255}, {0, 0, 0}},
};

// Original action/context lookup data, with its arrays and pointers in one owner.

i32 (*CanStartHoldFn)(GameObject_s *) = NULL;

extern i32 LevFlag[4];

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

struct nuvec_s;

extern AIACTIONDEF api_aiactiondefs[55];
extern AICONDITIONDEF api_aiconditiondefs[50];

DECOMP_ASSERT(sizeof(api_aiactiondefs) == 0x294, "API action registry size");

DECOMP_ASSERT(sizeof(api_aiconditiondefs) == 0x258, "API condition registry size");

struct EDCREATURE_s;

extern "C" {
    struct EDCREATURE_s;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

// Original writable flagremaptab at 0x668520, 32 pairs of 32-bit masks.
extern "C" {
extern u32 flagremaptab[32][2];
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

void ComboHitFrame(GameObject_s *object, i32 damage) {
    object->context_flags |= 0x40;
    object->sabre_flags |= 5;
    object->sabre_damage = damage;
    if ((object->apiobj.flags_low & 0x80) && Player_HasDoubleWeaponDamage(object)) {
        object->sabre_damage <<= 1;
    }
    BlockSfx(object);
}

void StartHold(GameObject_s *object) {
    if (CanStartHoldFn == NULL)
        return;
    if (CanStartHoldFn(object) == 2) {
        object->hold_timer = 0.0f;
        object->character_context = -1;
        return;
    }
    if (LEGOCONTEXT_HOLD != -1 && NewBlockAction(object) != 0) {
        object->context_animation_timer = 0.3f;
        object->character_context = LEGOCONTEXT_HOLD;
        PlaySabreSfx(NULL, object, NULL, 1);
        return;
    }
    object->character_context = -1;
}

void HoldCode(GameObject_s *object) {
    if (LEGOCONTEXT_HOLD != -1 && object->character_context == LEGOCONTEXT_HOLD) {
        if (object->context_animation_timer > 0.0f)
            object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0)
            object->character_context = -1;
    }
}

void HoldCode_Copy(GameObject_s *object) {
    if (LEGOCONTEXT_HOLD != -1 && object->character_context == LEGOCONTEXT_HOLD) {
        if (object->context_animation_timer > 0.0f)
            object->context_animation_timer -= FRAMETIME;
        else if ((object->pad_gamepad->buttons_held & GAMEPAD_ACTION) == 0)
            object->character_context = -1;
    }
}

void Punch_Hit(GameObject_s *attacker, GameObject_s *target, float, float gap) {
    NUVEC *hit_position;
    attacker->context_flags |= 0x40;

    if (LEGOCONTEXT_PUNCH != -1 && attacker->character_context == LEGOCONTEXT_PUNCH) {
        CHARACTERANIM_s *animation =
            static_cast<CHARACTERANIM_s *>(attacker->apiobj.character_model->model_data_a[attacker->context_animation]);
        if (animation != NULL) {
            attacker->attack_locator = animation->locator;
        }
    }

    if (target != NULL) {
        if ((target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
            target->apiobj.collision_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > target->apiobj.collision_max.y) {
            goto finish;
        }

        if (LEGOCONTEXT_HOLD != -1 && target->character_context == LEGOCONTEXT_HOLD) {
            NewBlockAction(target);
            hit_position = &target->apiobj.collision_position;
            if (Punch_HitHoldFn != NULL) {
                Punch_HitHoldFn(attacker, target);
            }
            goto hit;
        }

        if (target->apiobj.player_controlled) {
            if (LEGOCONTEXT_PUNCH != -1 && target->character_context == LEGOCONTEXT_PUNCH) {
                goto finish;
            }
            if (LEGOCONTEXT_JUMP != -1 && target->character_context == LEGOCONTEXT_JUMP) {
                if (attacker->action_movement_state == 3) {
                    goto finish;
                }
            }
        }

        if (ObjOpponentStillThere(attacker, target, gap) == 0) {
            goto finish;
        }

        i32 damage = Punch_GetDamageFn != NULL ? Punch_GetDamageFn(attacker, target) : 1;
        if (damage != -1 && Cheats_CheckFlags(0x800) != 0) {
            damage *= 2;
        }

        hit_position = NULL;
        u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x80) == 0) {
            if ((context_flags & 0x04000000) != 0 ||
                ((context_flags & 0x08000000) != 0 && (target->jump_flags & 2) != 0) || CannotKill(target) != 0) {
                NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
                NewRumble(target->pad_gamepad->pad, 0.5f, 0);
                if (static_cast<i32>(CInfo[target->character_context].flags) >= 0) {
                    target->apiobj.movement_facing_angle =
                        NuAtan2D(attacker->apiobj.collision_position.x - target->apiobj.collision_position.x,
                                 attacker->apiobj.collision_position.z - target->apiobj.collision_position.z);
                }
            } else {
                ObjHitObj(attacker, target, damage, static_cast<u16>(ObjHitObj_Flags(attacker) | 0x40), 0, 1);
            }
            hit_position = &target->apiobj.collision_position;
        }

        if ((target->apiobj.field_0x1f8 & 2) == 0) {
            f32 impulse = target->apiobj.character_data->game_character->movement_speed;
            if (attacker->apiobj.scaled_radius > 1.0f) {
                impulse /= attacker->apiobj.scaled_radius;
            }
            u16 angle = attacker->apiobj.movement_facing_angle;
            if (static_cast<i8>(attacker->context_flags) < 0) {
                angle -= 0x8000;
            }
            target->apiobj.velocity.z += NuTrigTable[((static_cast<u32>(angle) + 0x4000) >> 1) & 0x7fff] * impulse;
            target->apiobj.velocity.x += NuTrigTable[angle >> 1] * impulse;
        }
    } else {
        GIZMOBLOWUP_s *blowup = attacker->blowup_target;
        if (blowup == NULL || (blowup->status_flags & 0x804001) != 0x804000 ||
            blowup->bounds_min.y > attacker->apiobj.collision_max.y ||
            attacker->apiobj.collision_min.y > blowup->bounds_max.y) {
            goto finish;
        }
        hit_position = &blowup->mid_position;
        if (GizmoBlowupBlowup(blowup, 1, 1, 1, NULL, 1) != 0) {
            NewBuzz(attacker->pad_gamepad->pad, 0.1f, 0);
        }
        NewRumble(attacker->pad_gamepad->pad, 0.5f, 0);
    }

    if (hit_position == NULL) {
        goto finish;
    }

hit:
    attacker->movement_runtime_flags |= 1;
    if (__builtin_expect(attacker->apiobj.player_controlled != 0, 1)) {
        GameCam_HitJudder();
    }
    goto extra_code;

finish:
    hit_position = NULL;
    attacker->movement_runtime_flags &= ~1;

extra_code:
    if (Punch_HitExtraCodeFn != NULL) {
        Punch_HitExtraCodeFn(attacker, hit_position);
    }
}

