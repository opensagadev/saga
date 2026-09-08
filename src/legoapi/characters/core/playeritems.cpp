#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);
extern "C" f32 AnimDuration(i32 character, i32 animation, f32 start, f32 end, i32 subtract_frame_time);
i32 GetDefaultIdle(GameObject_s *object);
void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation);
i32 (*Fighting_WeaponInActionFn)(GameObject_s *) = NULL;
i32 (*Fighting_WeaponOutActionFn)(GameObject_s *) = NULL;

void LoseHelmet(GameObject_s *, i32, i32) {
}

void SetWeaponIn(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void FastWeaponIn(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    if (force_sound != 0 && object->weapon_scale == 1.0f && object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
        const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
        CHARACTERANIM_s *animation = NULL;
        if (current_animation != -1) {
            animation = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        }
        if (animation == NULL || (animation->flags & CHARACTER_ANIMATION_FLAG_ALLOW_WEAPON_TRANSITION) == 0) {
            const u32 model_flags = object->apiobj.character_data->model_flags;
            if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
                if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
                    GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
                }
            } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
                GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
            }
        }
    }
    object->weapon_scale_rate = 5.0f;
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
}

void KeepWeaponIn(GameObject_s *object) {
    object->weapon_scale = 0.0f;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 &= ~GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void SetWeaponOut(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }
    object->weapon_scale = 1.0f;
    object->weapon_out_timer = 0.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
}

void SlowWeaponIn(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN != -1 && Fighting_WeaponInActionFn != NULL) {
        const i32 action = Fighting_WeaponInActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (start < 1.0f)
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONIN;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 1.0f;
                    return;
                }
            }
        }
    }
    FastWeaponIn(object, 1);
}

void WeaponInCode(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONIN == -1 || object->character_context != LEGOCONTEXT_WEAPONIN)
        return;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (object->pad_gamepad->input_magnitude > 0.0f && (data->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 1.0f && object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
            FastWeaponIn(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 1.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 1.0f)
            return;
    } else {
        if (object->weapon_scale != 1.0f || object->weapon_scale_state == WEAPON_SCALE_RETRACTING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (time < start)
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_RETRACTING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3d, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x42, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void FastWeaponOut(GameObject_s *object, i32 force_sound) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONIN || context == LEGOCONTEXT_WEAPONOUT)) {
        object->character_context = -1;
    }

    if (force_sound != 0 && object->weapon_scale == 0.0f && object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
        const i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
        CHARACTERANIM_s *animation = NULL;
        if (current_animation != -1) {
            animation = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[current_animation]);
        }

        if (animation == NULL || (animation->flags & CHARACTER_ANIMATION_FLAG_ALLOW_WEAPON_TRANSITION) == 0) {
            const u32 model_flags = object->apiobj.character_data->model_flags;
            if ((model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
                if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
                    GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
                }
            } else if ((model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
                GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
            }
        }
    }

    object->weapon_scale_rate = 5.0f;
    object->weapon_out_timer = 0.0f;
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
}

void KeepWeaponOut(GameObject_s *object) {
    object->weapon_scale = 1.0f;
    object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
    object->weapon_scale_state = WEAPON_SCALE_IDLE;
    object->field_0xef8 |= GAMEOBJECT_EF8_FLAG_KEEP_WEAPON_OUT;
}

void ReleaseHearts() {
}

void SlowWeaponOut(GameObject_s *object) {
    if (LEGOCONTEXT_WEAPONOUT != -1 && Fighting_WeaponOutActionFn != NULL) {
        const i32 action = Fighting_WeaponOutActionFn(object);
        if (action != -1 && object->apiobj.character_model->model_data_b[action] != NULL) {
            const f32 end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[action]);
            f32 start = AnimListFrame(object->apiobj.character_model, action, 0);
            if (start < 1.0f)
                start = 1.0f;
            if (start >= 1.0f && start < end) {
                const f32 finish = AnimListFrame(object->apiobj.character_model, action, 1);
                if (finish > end || finish > start) {
                    object->context_animation = action;
                    object->character_context = LEGOCONTEXT_WEAPONOUT;
                    object->context_animation_timer = AnimDuration(object->id, action, 0.0f, 0.0f, 1);
                    object->weapon_scale = 0.0f;
                    object->weapon_out_timer = 0.0f;
                    return;
                }
            }
        }
    }
    FastWeaponOut(object, 1);
}

void WeaponOutCode(GameObject_s *object) {
    object->weapon_out_timer += FRAMETIME;
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (TouchHacks::ShouldKeepWeaponOut(*object)) {
        object->weapon_out_timer = 0.0f;
        if ((object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) == 0 && object->character_context != 7) {
            i32 action = 17;
            if (data->field275_0x116 == 0 && (object->apiobj.character_data->model_flags & 0x80) != 0)
                action = 127;
            if (object->weapon_scale_state == WEAPON_SCALE_IDLE && object->apiobj.field_0x27d != 0 &&
                object->pad_gamepad->input_magnitude == 0.0f &&
                object->apiobj.character_model->model_data_b[action] != NULL &&
                (object->character_context == -1 || (CInfo[object->character_context].flags & 4) != 0)) {
                SlowWeaponOut(object);
            } else {
                FastWeaponOut(object, 1);
            }
        }
    } else if (TouchHacks::ShouldPutWeaponAway(*object) &&
               (object->field_0xe22 & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) != 0 && object->character_context != 6) {
        SlowWeaponIn(object);
    }
    if (LEGOCONTEXT_WEAPONOUT == -1 || object->character_context != LEGOCONTEXT_WEAPONOUT)
        return;
    if (object->pad_gamepad->input_magnitude > 0.0f && (data->field_0x94 & 0x1000) == 0) {
        if (object->weapon_scale == 0.0f && object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            FastWeaponOut(object, 0);
        }
        object->character_context = -1;
        return;
    }
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 animation = packet.blending ? packet.blend_animation_b : packet.animation_index;
    if (animation != object->context_animation) {
        object->weapon_scale = 0.0f;
        return;
    }
    const f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
    object->context_animation_timer -= FRAMETIME;
    if (object->context_animation_timer <= 0.0f) {
        object->character_context = -1;
        if (object->weapon_scale != 0.0f)
            return;
    } else {
        if (object->weapon_scale != 0.0f || object->weapon_scale_state == WEAPON_SCALE_EXTENDING ||
            packet.requested_animation != object->context_animation)
            return;
        const f32 time = packet.blending ? packet.blend_target_time : packet.current_time;
        if (time < start)
            return;
    }
    const f32 end = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
    object->weapon_scale_state = WEAPON_SCALE_EXTENDING;
    object->weapon_scale_rate = 1.0f / AnimDuration(object->id, object->context_animation, start, end, 0);
    if ((object->apiobj.character_data->model_flags & CHARACTER_MODEL_FLAG_JEDI) != 0) {
        if (object->apiobj.field_0x27c != -1 || WeaponInOut_NoAIJediSfx == 0) {
            GameAudio_PlaySfx(0x3e, &object->apiobj.collision_position, GameAudio_GetPlrSfxBits(object), 1);
        }
    } else {
        GameAudio_PlaySfx(0x43, &object->apiobj.collision_position, 0, 1);
    }
    ResetCharacterIdle(object, 1, GetDefaultIdle(object));
}

void AutoWeaponOnOff(GameObject_s *object) {
    const i8 context = object->character_context;
    if (context != -1 && (context == LEGOCONTEXT_WEAPONOUT || context == LEGOCONTEXT_WEAPONIN))
        return;
    const i32 index = CurrentAnim(&object->apiobj.anim_packet);
    if (index == -1)
        return;
    CHARACTERANIM_s *animation = static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[index]);
    const u32 flags = animation->flags;
    const ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if ((flags & 0x400) != 0) {
        if (object->weapon_scale > 0.0f) {
            if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
                FastWeaponIn(object, 0);
            } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                       object->weapon_scale_state != WEAPON_SCALE_RETRACTING) {
                SetWeaponIn(object);
            }
        }
    } else if ((flags & 0x800) != 0 && object->weapon_scale < 1.0f) {
        if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 && packet.blending != 0) {
            FastWeaponOut(object, 0);
        } else if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 ||
                   object->weapon_scale_state != WEAPON_SCALE_EXTENDING) {
            SetWeaponOut(object);
        }
    }
}

void RegenerateHearts(GameObject_s *) {
}

void WeaponScalingCode(GameObject_s *object) {
    switch (object->weapon_scale_state) {
        case WEAPON_SCALE_EXTENDING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 1.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale >= 1.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 |= GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        case WEAPON_SCALE_RETRACTING:
            object->weapon_scale = SeekLinearF(object->weapon_scale, 0.0f, object->weapon_scale_rate * FRAMETIME);
            if (object->weapon_scale <= 0.0f) {
                object->weapon_scale_state = WEAPON_SCALE_IDLE;
                object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION;
            }
            break;
        default:
            break;
    }
}

void FindPlayerAndSetWeapon(i32, i32) {
}
