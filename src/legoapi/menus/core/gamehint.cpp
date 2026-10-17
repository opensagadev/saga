// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/legoapi_types.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/world/mission.h"

LSW_HINTCONDITIONS_s LSW_HintConditions;
HINTSYS_s hintsys = {};
HINTUIBUTTON_s *hintUIButton = NULL;
extern f32 AlphaBlendTime;
extern f32 ICONX;
extern f32 ICONXPOS_TOUCHYFEELY;
extern f32 ICONYPOS_TOUCHYFEELY;
extern f32 ICONXPOS_VIRTUALS;
extern f32 ICONYPOS_VIRTUALS;
f32 (*Hint_AlphaTargetFn)();
i32 (*Hub_PanelBusyFn)();
extern i32 only_process_this_hint_id;
f32 SeekLinearF(f32, f32, f32);
void Hint_SetHint(HINT_s *, i32, i32);

extern "C" i32 NewMode, Paused, editor_active, CutSceneWaiting, PANELOFF;
extern FadeSystem FadeSys;
i32 GetMenuID();
void MechHintUIButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);

#include "legoapi/menus/core/gamehint_types.h"
DECOMP_ASSERT(sizeof(MechHintUIButton) == 0xa8, "MechHintUIButton size");
DECOMP_ASSERT(offsetof(MechHintUIButton, pending_hint) == 0x7c, "Pending touch hint offset");
DECOMP_ASSERT(offsetof(MechHintUIButton, display_elapsed) == 0x80, "Touch hint display timer offset");

void MechHintUIButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &) {
    MechHintUIButton &button = static_cast<MechHintUIButton &>(element);
    hintsys.active_hint = button.active_hint;
    if (button.active_hint != NULL) {
        if (button.active_hint->on_display != NULL)
            button.active_hint->on_display(button.active_hint);
        if (button.active_hint != NULL) {
            const f32 blend = AlphaBlendTime;
            button.pending_hint = NULL;
            button.slide_from = *button.slide_target;
            button.slide_to = -1.5f;
            button.slide_elapsed = 0.0f;
            button.slide_delay = 0.0f;
            button.active_hint = NULL;
            button.slide_duration = blend;
            button.alpha_from = *button.alpha_target;
            button.pulse_active = 0;
            button.alpha_to = 0.0f;
            button.alpha_elapsed = 0.0f;
            button.alpha_delay = 0.0f;
            button.alpha_duration = blend;
        }
    }
    const f32 x = button.slide;
    const f32 y = button.position.y;
    hintIconPos.from = VuVec(x, y, 1.0f, 1.0f);
    hintIconPos.to = VuVec(-ICONX, -0.7f, 1.0f, 1.0f);
    hintIconPos.duration = 0.5f;
    hintIconPos.delay = 0.0f;
    hintIconPos.elapsed = 0.0f;
    hintIconPos.target->z = 1.0f;
    hintIconPos.target->w = 1.0f;
    hintIconPos.target->x = x;
    hintIconPos.target->y = y;
    const f32 scale = button.scale;
    hintIconScale.from = scale;
    hintIconScale.to = 0.5f;
    hintIconScale.elapsed = 0.0f;
    hintIconScale.duration = 0.5f;
    hintIconScale.delay = 0.0f;
    *hintIconScale.target = scale;
    hintYPop.value = 0.0f;
    hintYPop.from = 0.0f;
    hintYPop.to = 32768.0f;
    hintYPop.elapsed = 0.0f;
    hintYPop.duration = 0.4f;
    hintYPop.delay = 0.0f;
    *hintYPop.target = 0.0f;
    *button.slide_target = button.slide_to;
    button.slide = -1.5f;
    button.slide_to = -1.5f;
    button.slide_elapsed = button.slide_duration;
    GameAudio_PlaySfx(0x30, NULL, 0, 0);
}

void initHintSys() {
    hintsys.current_hint = 0;
    hintsys.state = 2;
    hintsys.active_hint = NULL;
    MechHintUIButton *button = new MechHintUIButton();
    button->UpdateTexture(MechInputTouchVirtualConsoleController::s_textures[3]);
    button->alpha = 0.0f;
    button->alpha_to = 0.0f;
    button->slide = -1.5f;
    button->visible = 0;
    button->slide_to = -1.5f;
    button->pulse_active = 0;
    button->slide_elapsed = button->slide_duration;
    hintUIButton = reinterpret_cast<HINTUIBUTTON_s *>(button);
    button->alpha_elapsed = button->alpha_duration;
    MechSystems::Get()->TouchUI().AddUIElement(*button);
}

void MechHintUIButton::Render() {
    if (NewMode != 0 || NewLData != NULL)
        return;
    LEVEL_OBJECT_RUNTIME *icon = &WORLD->lev_objs[0xd4];
    if (icon->active == 0)
        return;
    const f32 opacity = alpha;
    const u16 angle = static_cast<u16>((NuFmod(GameTimer.time_elapsed, 2.5f) / 2.5f) * 65536.0f);
    DrawPanel3DObject(position.x, position.y, 1.0f, scale, scale, scale, 0, angle, 0, &icon->special, 2, opacity);
}

void MechHintUIButton::Process(float elapsed) {
    if (__builtin_expect(active_hint != NULL, 0)) {
        display_elapsed += elapsed;
        if (MechSystems::Get()->PlayerButton().selector != NULL ||
            (active_hint->display_duration > 0.0f && display_elapsed >= active_hint->display_duration)) {
            if (active_hint != NULL) {
                const f32 blend = AlphaBlendTime;
                pending_hint = NULL;
                slide_from = *slide_target;
                slide_to = -1.5f;
                slide_elapsed = 0.0f;
                slide_delay = 0.0f;
                active_hint = NULL;
                slide_duration = blend;
                alpha_from = *alpha_target;
                pulse_active = 0;
                alpha_to = 0.0f;
                alpha_elapsed = 0.0f;
                alpha_delay = 0.0f;
                alpha_duration = blend;
            }
        }
    }
    HINT_s *next_hint = pending_hint;
    const u8 was_pulsing = pulse_active;
    if (hovered) {
        scale_to = 0.6f;
        scale = scale_to;
    } else {
        scale_to = 0.5f;
        scale = scale_to;
    }
    scale_elapsed = scale_duration;
    if (__builtin_expect(!(slide_duration < 0.0f) && !(slide_elapsed >= slide_duration + slide_delay), 0)) {
        slide_elapsed += elapsed;
        if (__builtin_expect(slide_elapsed > slide_duration + slide_delay, 0))
            slide_elapsed = slide_duration + slide_delay;
        if (slide_elapsed >= slide_delay)
            *slide_target = ((slide_elapsed - slide_delay) / slide_duration) * (slide_to - slide_from) + slide_from;
    }
    position.x = slide;
    if (__builtin_expect(next_hint != NULL && __builtin_expect(slide_duration < 0.0f || slide_elapsed >= slide_duration + slide_delay, 0), 1)) {
        active_hint = next_hint;
        pending_hint = NULL;
        display_elapsed = 0.0f;
        f32 blend;
        if (TouchHacks::TouchControlsActive || MechInputTouchSystem::s_actualTouchMode == 7) {
            position.y = ICONYPOS_TOUCHYFEELY;
            blend = AlphaBlendTime;
            slide_from = *slide_target;
            slide_to = ICONXPOS_TOUCHYFEELY;
        } else {
            position.y = ICONYPOS_VIRTUALS;
            blend = AlphaBlendTime;
            slide_from = *slide_target;
            slide_to = ICONXPOS_VIRTUALS;
        }
        slide_elapsed = 0.0f;
        slide_delay = 0.0f;
        slide_duration = blend;
        alpha_from = *alpha_target;
        alpha_to = 1.0f;
        alpha_elapsed = 0.0f;
        alpha_duration = blend;
        alpha_delay = 0.0f;
    }
    if (was_pulsing) {
        pulse_remaining -= elapsed;
        if (__builtin_expect(0.0f >= pulse_remaining, 0))
            StartPulse();
    }
    MechTouchUITexButton::Process(elapsed);
    if (visible) {
        visible = 0;
        if (__builtin_expect(NewMode == 0, 0) && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
            GameTimer.update_count != 0 && WORLD != NULL && WORLD->current_level != TITLES_LDATA &&
            CutSceneWaiting == 0 && Paused == 0 && CUTSTOPGAME == 0 && MiniCutCam == 0 && PANELOFF == 0 &&
            WORLD->current_level != STATUS_LDATA && (WORLD->current_level->flags & 0x400) == 0)
            visible = 1;
    }
    disabled = (GetMenuID() != -1 && GetMenuID() != 13) || Paused != 0 || NewMode != 0 || NewLData != NULL ||
               CUTSTOPGAME != 0 || MiniCutCam != 0 || FadeSys.fade > 0.0f || player == NULL;
    if (!pulse_active && (slide_duration < 0.0f || slide_elapsed >= slide_duration + slide_delay) && slide > -1.0f)
        StartPulse();
}

void CLEAR_HINT_COMPLETE(i32 hint_id);
i32 HINT_COMPLETE(i32 hint_id);
void SET_HINT_COMPLETE(i32 hint_id);
void Hint_SaveGameState(HINT_s *hint);
void initHintSys();
void Hint_Reset() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return;
    }

    while (hint->control_mode_ids[0] != -1) {
        hint->field_0x20 = 0;
        hint++;
    }

    hintsys.state = 2;
    hintsys.current_hint = 0;

    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL) {
        f32 alpha_blend_time = AlphaBlendTime;
        button->field_0x7c = 0;
        f32 field_0x84 = *button->field_0x84;
        f32 *field_0x40 = button->field_0x40;
        button->field_0x8c = -1.5f;
        button->field_0x90 = 0.0f;
        button->field_0x98 = 0.0f;
        button->field_0x88 = field_0x84;
        button->field_0x78 = NULL;
        button->field_0x94 = alpha_blend_time;
        f32 field_0x40_value = *field_0x40;
        button->field_0xa0 = 0;
        button->field_0x48 = 0.0f;
        button->field_0x4c = 0.0f;
        button->field_0x54 = 0.0f;
        button->field_0x44 = field_0x40_value;
        button->field_0x50 = alpha_blend_time;
    }

    hintsys.field_0x1c = 0;
}

void Hint_Process(float elapsed) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    const i32 only_hint = only_process_this_hint_id;
    only_process_this_hint_id = -1;
    hintIconPos.Update(elapsed);
    hintIconScale.Update(elapsed);
    hintYPop.Update(elapsed);
    if (hintsys.hints == NULL)
        return;

    i32 available =
        HINTS_ON != 0 && CUTSTOPGAME == 0 && MiniCutCam != 1 && MiniCutCam != 2 && MiniCutCam != 3 && SuperStory == 0 &&
        BonusArea == 0 && (world->current_level->flags & 0x200) == 0 &&
        (world->area == NULL || (world->area->flags & 0x124) == 0) && GetMenuID() == -1 && FadeSys.fade == 0.0f &&
        (world->area == NULL || world->area != HUB_ADATA || Hub_PanelBusyFn == NULL || Hub_PanelBusyFn() == 0);
    if (ChallengeMode != 0)
        available = false;
    else if (Mission_Active(NULL) != NULL)
        available = false;
    if (hintUIButton->field_0x78 != NULL || hintUIButton->field_0x7c != NULL)
        available = false;

    for (HINT_s *hint = hintsys.hints; hint->control_mode_ids[0] != -1; ++hint) {
        if (hintsys.active_hint != hint && hint->field_0x20 > 0.0f)
            hint->field_0x20 -= elapsed;
        if (only_hint != -1 && hint->control_mode_ids[0] != only_hint)
            continue;
        const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
        const i32 id = hint->control_mode_ids[control_mode];
        if (id == -1 || TTab[id] == NULL || !available || hintUIButton->field_0x78 != NULL ||
            hintUIButton->field_0x7c != NULL || hintsys.active_hint != NULL ||
            hint->completion_flags[control_mode] != 0 || hint->availability_fn == NULL)
            continue;
        if (hint->availability_fn(hint) == 0 || hint->field_0x20 > 0.0f ||
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] == 1)
            continue;
        Hint_SetHint(hint, 0, 0);
    }

    const f32 target = Hint_AlphaTargetFn != NULL ? Hint_AlphaTargetFn() : 1.0f;
    hintsys.alpha = SeekLinearF(hintsys.alpha, target, 3.0f * FRAMETIME);
    HINT_s *hint = hintsys.active_hint;
    if (hint != NULL) {
        hintsys.display_elapsed += elapsed;
        if ((hint->display_duration > 0.0f && hintsys.display_elapsed >= hint->display_duration) ||
            MechSystems::Get()->PlayerButton().selector != NULL)
            hintsys.active_hint = NULL;
    }
}

void Hint_SetHint(HINT_s *hint, i32 force, i32 allow_completed) {
    if (hintsys.active_hint != NULL && hintsys.active_hint == hint)
        return;
    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL && button->field_0x78 == hint)
        return;
    if (hint == NULL) {
        hintsys.active_hint = NULL;
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x50 = blend_time;
            button->field_0x44 = alpha_value;
        }
        return;
    }
    if (force == 0) {
        if (hint->completion_flags[MechInputTouchSystem::s_baseControlMode] != 0 && allow_completed == 0)
            return;
        if (hint->field_0x20 > 0.0f)
            return;
    }
    if ((hint->flags & 8) == 0) {
        if (button->field_0x78 != hint) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = hint;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
    } else {
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
        hintsys.active_hint = hint;
        if (hint->on_display != NULL)
            hint->on_display(hint);
    }
    hintsys.current_hint = 0;
    hintsys.field_0x1c = 0;
    if (force == 0 && (hint->flags & 1) != 0) {
        hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 1;
        Hint_SaveGameState(hint);
    }
    i32 state;
    do {
        state = qrand() / 16384;
    } while (state == hintsys.state);
    hintsys.state = static_cast<u8>(state);
    hint->field_0x20 = hint->repeat_delay;
    hintsys.field_0x1c = 0;
}

HINT_s *Hint_FindHint(i32 hint_id) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return NULL;
    }

    while (hint->control_mode_ids[0] != -1) {
        if (hint->control_mode_ids[0] == hint_id || hint->control_mode_ids[1] == hint_id) {
            return hint;
        }
        ++hint;
    }
    return NULL;
}

void Hint_SaveBits(i32 hint_id, i32 complete) {
    if (complete != 0) {
        SET_HINT_COMPLETE(hint_id);
        return;
    }
    CLEAR_HINT_COMPLETE(hint_id);
}

i32 Hint_CurrentId() {
    HINT_s *hint = hintsys.active_hint;
    if (hint == NULL) {
        return -1;
    }
    return hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode];
}

void Hint_ResetHint(i32 hint_id, i32 reset_completed) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    while (hint->control_mode_ids[control_mode] != hint_id) {
        hint++;
        if (hint->control_mode_ids[0] == -1) {
            return;
        }
    }

    if (reset_completed == 1) {
        hint->completion_flags[control_mode] = 0;
    }
    hintsys.current_hint = 0;
}

i32 Hint_isComplete(HINT_s *hint) {
    return hint->completion_flags[MechInputTouchSystem::s_baseControlMode];
}

i32 Hint_isComplete(i32 hint_id) {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return 0;
    }

    hint = Hint_FindHint(hint_id);
    if (hint == NULL) {
        return 0;
    }
    return hint->completion_flags[MechInputTouchSystem::s_baseControlMode];
}

void Hint_SetComplete(HINT_s *hint) {
    if (hint == NULL) {
        return;
    }

    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] == 0) {
        hint->completion_flags[control_mode] = 1;
        Hint_SaveGameState(hint);
    }
}

void Hint_SetComplete(i32 hint_id) {
    if (hint_id == -1) {
        return;
    }

    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint == NULL || (hint->flags & 0x40) == 0) {
        return;
    }

    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    if (hint->completion_flags[control_mode] == 0) {
        hint->completion_flags[control_mode] = 1;
        Hint_SaveGameState(hint);
    }
}

i32 Hint_isAvailable(i32 hint_id) {
    HINT_s *active_hint = hintsys.active_hint;
    if (active_hint != NULL && active_hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode] == hint_id) {
        return 0;
    }
    if (hint_id < 0 || TTab[hint_id] == NULL) {
        return 0;
    }

    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint == NULL || hint->completion_flags[MechInputTouchSystem::s_baseControlMode] != 0) {
        return 0;
    }
    return !(hint->field_0x20 > 0.0f);
}

void Hint_CancelCurrent() {
    HINT_s *hint = hintsys.active_hint;
    if (hint != NULL) {
        if (hint->display_duration != 0.0f && hint->display_duration - 0.5f > hintsys.display_elapsed)
            hintsys.display_elapsed = hint->display_duration - 0.5f;
        hint->field_0x20 = 0.0f;
    }
    HINTUIBUTTON_s *button = hintUIButton;
    if (button->field_0x78 != NULL) {
        const f32 blend_time = AlphaBlendTime;
        button->field_0x7c = NULL;
        const f32 position = *button->field_0x84;
        f32 *alpha = button->field_0x40;
        button->field_0x8c = -1.5f;
        button->field_0x90 = 0.0f;
        button->field_0x98 = 0.0f;
        button->field_0x88 = position;
        button->field_0x78 = NULL;
        button->field_0x94 = blend_time;
        const f32 alpha_value = *alpha;
        button->field_0xa0 = 0;
        button->field_0x48 = 0.0f;
        button->field_0x4c = 0.0f;
        button->field_0x54 = 0.0f;
        button->field_0x44 = alpha_value;
        button->field_0x50 = blend_time;
    }
}

void Hint_ExpandButtons(char *input, char *output) {
    char token[256];
    char expanded[64];

    *output = '\0';
    while (*input != '\0') {
        if (*input != '[') {
            *output = *input;
            output[1] = '\0';
            ++input;
            ++output;
            continue;
        }

        token[0] = '[';
        i32 token_length = 1;
        while (input[token_length] != ']' && input[token_length] != '\0') {
            token[token_length] = input[token_length];
            ++token_length;
        }

        char literal = '[';
        if (input[token_length] != '\0') {
            token[token_length] = ']';
            token[token_length + 1] = '\0';
            token_length = NuStrLen(token);
            if (token_length > 0 && Text_ExpandButtonString(token, expanded) != 0) {
                char *character = expanded;
                while (*character != '\0') {
                    *output = *character;
                    output[1] = '\0';
                    ++character;
                    ++output;
                }
                NuStrCat(output, expanded);
                input += token_length;
                continue;
            }
            literal = *input;
        }

        *output = literal;
        output[1] = '\0';
        ++input;
        ++output;
    }
    *output = '\0';
}

void Hint_SaveGameState(HINT_s *hint) {
    HINT_s *candidate = hintsys.hints;
    if (candidate == NULL) {
        return;
    }
    if (hintsys.save_bits == NULL) {
        return;
    }
    if (candidate->control_mode_ids[0] == -1) {
        return;
    }

    const u16 hint_id = static_cast<u16>(hint->control_mode_ids[0]);
    i32 index = 0;
    u16 candidate_id = static_cast<u16>(candidate->control_mode_ids[0]);
    if (candidate_id == hint_id) {
        goto found;
    }
    do {
        ++candidate;
        ++index;
        candidate_id = static_cast<u16>(candidate->control_mode_ids[0]);
        if (candidate_id == 0xffff) {
            return;
        }
    } while (candidate_id != hint_id);

found:
    Hint_SaveBits(index, 1);
}

void Hint_SetHintFromId(i32 hint_id, i32 force, i32 allow_completed) {
    if (hintsys.active_hint != NULL &&
        hintsys.active_hint->control_mode_ids[MechInputTouchSystem::s_baseControlMode] == hint_id)
        return;
    if (hint_id < 0 || TTab[hint_id] == NULL) {
        HINTUIBUTTON_s *button = hintUIButton;
        if (button->field_0x78 != NULL) {
            const f32 blend_time = AlphaBlendTime;
            button->field_0x7c = NULL;
            const f32 position = *button->field_0x84;
            f32 *alpha = button->field_0x40;
            button->field_0x8c = -1.5f;
            button->field_0x90 = 0.0f;
            button->field_0x98 = 0.0f;
            button->field_0x88 = position;
            button->field_0x78 = NULL;
            button->field_0x94 = blend_time;
            const f32 alpha_value = *alpha;
            button->field_0xa0 = 0;
            button->field_0x48 = 0.0f;
            button->field_0x4c = 0.0f;
            button->field_0x54 = 0.0f;
            button->field_0x44 = alpha_value;
            button->field_0x50 = blend_time;
        }
        return;
    }
    HINT_s *hint = Hint_FindHint(hint_id);
    if (hint != NULL)
        Hint_SetHint(hint, force, allow_completed);
}

void Hint_SaveAllGameState() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL) {
        return;
    }
    u32 *save_bits = hintsys.save_bits;
    if (save_bits == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 index = 0;
    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    do {
        if (hint->completion_flags[control_mode] != 0) {
            Hint_SaveBits(index, 1);
        } else {
            Hint_SaveBits(index, 0);
        }
        ++hint;
        ++index;
    } while (hint->control_mode_ids[0] != -1);
}

void Hint_LoadAllGameState() {
    HINTSYS_s *system = &hintsys;
    HINT_s *hint = system->hints;
    if (hint == NULL || system->save_bits == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    for (i32 index = 0; (index >> 5) < system->save_bit_count; ++index) {
        if (HINT_COMPLETE(index) == 0) {
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 0;
        } else {
            hint->completion_flags[MechInputTouchSystem::s_baseControlMode] = 1;
        }
        ++hint;
        if (hint->control_mode_ids[0] == -1) {
            return;
        }
    }
}

void Hint_ClearHintsAndDoneFlags() {
    HINT_s *hint = hintsys.hints;
    if (hint == NULL || hint->control_mode_ids[0] == -1) {
        return;
    }

    i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    do {
        hint->completion_flags[control_mode] = 0;
        hint->flags &= ~0x42;
        hint++;
    } while (hint->control_mode_ids[0] != -1);
}

void Hint_Draw(i32 viewport) {
    if (hintsys.active_hint != NULL && hintsys.update_fn != NULL)
        hintsys.update_fn(hintsys.active_hint, viewport);
}

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

static __used__ nuhspecial_s *GameMsg_GetExtraObj(GAMEMESSAGE_s *message);

static __used__ void GameMsg_EndDelay_Game(GAMEMESSAGE_s *message);

i32 Percent_UpdateHint(HINT_s *hint);

void RegisterWithHintSys(void (*update_fn)(HINT_s *, i32), HINT_s *hints, u32 *save_bits, i32 save_bit_count) {
    hintsys.update_fn = update_fn;
    hintsys.hints = hints;
    hintsys.save_bits = save_bits;
    hintsys.save_bit_count = save_bit_count;
    initHintSys();
}

// ==== src/legoapi/menus/core/hint.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/input/gamepads.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 only_process_this_hint_id;
extern f32 ICONXPOS_VIRTUALS;
extern f32 ICONYPOS_VIRTUALS;
extern f32 ICONYPOS_TOUCHYFEELY;
extern f32 AlphaBlendTime;

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.
extern HintVectorTransition hintIconPos;
extern HintScalarTransition hintIconScale;
extern HintScalarTransition hintYPop;
extern f32 ICONXPOS_TOUCHYFEELY;
static __used__ i32 Attack_UpdateHint(HINT_s *);

u8 show_unlock_shop_hint = 0;
u8 show_unlock_customiser_hint = 0;
u8 show_unlock_minikitviewer_hint = 0;

i32 HINT_COMPLETE(i32 hint_id);

i32 Tilt_UpdateHint(HINT_s *);

f32 CurrentHintAlpha();

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.
static __attribute__((noinline, noclone)) void DrawHint_LSWBody(HINT_s *hint, i32 max_lines)
    __asm__("_Z12DrawHint_LSWP6HINT_si.part.19");

static void DrawHint_LSWBody(HINT_s *hint, i32 max_lines);

void DrawHint_LSW(HINT_s *hint, i32 max_lines);

i32 Dodge_UpdateHint(HINT_s *hint);

void SET_HINT_COMPLETE(i32 hint_id);

void CLEAR_HINT_COMPLETE(i32 hint_id);

i32 DragBomb_UpdateHint(HINT_s *hint);

// Original local callbacks referenced by Hints_LSW.
extern u8 show_lever_hint;
extern i32 show_autojump_hint;
extern AREADATA_s *PODRACE_ADATA, *PODSPRINT_ADATA, *BONUS_GUNSHIP_ADATA;
extern FadeSystem FadeSys;

static __used__ i32 Jump_UpdateHint(HINT_s *hint);

static __used__ i32 Move_UpdateHint(HINT_s *hint);

static __used__ i32 Lever_UpdateHint(HINT_s *);

static __used__ i32 AutoJump_UpdateHint(HINT_s *hint);

static __used__ i32 VehicleStuff_UpdateHint(HINT_s *hint);

static __used__ i32 HoldTag_UpdateHint(HINT_s *);

u8 show_hatmachine_hint;

static __used__ i32 HatMachine_UpdateHint(HINT_s *hint);

extern HINTUIBUTTON_s *hintUIButton;

void *GetHintFromUIButton();

extern HINT_s Hints_LSW[55];
void RegisterWithHintSys(void (*)(HINT_s *, i32), HINT_s *, u32 *, i32);
i32 id_HINT_LSW_AUTOJUMP = -1;
i32 id_HINT_LSW_AUTOJUMP_FAIL = -1;

void initGameHintSys_LSW();

i32 SmartBomb_UpdateHint(HINT_s *);

extern AREADATA *JABBASPALACE_ADATA;

i32 ShinyMetal_UpdateHint(HINT_s *hint);

f32 CurrentHintButtonScale();

void initGameHintSys_Batman();

i32 IndyUnlocked_UpdateHint(HINT_s *hint);

static __used__ i32 Sith_UpdateHint(HINT_s *);

static __used__ void PlayerButton_PlayHint(HINT_s *);

static __used__ i32 UnlockHubStuff_UpdateHint(HINT_s *hint);

static __used__ i32 PlayerButton_UpdateHint(HINT_s *hint);

i32 GizForce_UpdateHint(HINT_s *);
i32 ZipUps_UpdateHint(HINT_s *);
i32 Push_UpdateHints(HINT_s *);
i32 GizPanel_UpdateHint(HINT_s *);
i32 Shop_UpdateHint(HINT_s *);
HINT_s Hints_LSW[55] = {
    {{356, 356}, 12, {255, 0, 0}, 0, 6.0f, 2.0f, {NULL}, NULL, {0, 0, 0, 0}, 0.0f},
    {{601, 375}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizForce_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1548, 1549}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Lever_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1568, 1568}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {AutoJump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1569, 1569}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {AutoJump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{604, 374}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizBuildIts_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{631, 1553}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Attack_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{613, 1500}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Dodge_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1501, 1502}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Dodge_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{610, 1503}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {ZipUps_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{602, -1}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{615, 615}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Push_UpdateHints}, NULL, {0, 0, 0, 0}, 0.0f},
    {{616, 1507}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Teleport_UpdateHints}, NULL, {0, 0, 0, 0}, 0.0f},
    {{617, 617}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{618, 618}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{605, 1508}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{606, 379}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{611, 1509}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{650, 377}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{651, 378}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{643, 643}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {ShinyMetal_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1505, 1506}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {SmartBomb_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{607, 1510}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{608, 1511}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{609, 1512}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1516, 1516}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {Shop_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1515, 1515}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {Shop_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1514, 1514}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {Shop_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1518, 1519}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {DragBomb_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{654, 1520}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {DragBomb_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1521, 1521}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {Move_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1523, 1523}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {Move_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1540, 1541}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Jump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1522, 1522}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {Move_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1542, 1543}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Jump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1524, 1524}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {PlayerButton_UpdateHint}, PlayerButton_PlayHint, {0, 0, 0, 0}, 0.0f},
    {{1525, 1525}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {PlayerButton_UpdateHint}, PlayerButton_PlayHint, {0, 0, 0, 0}, 0.0f},
    {{1544, 1545}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Jump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1546, 1547}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Jump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1526, 1526}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {HoldTag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1561, 1561}, 8, {255, 0, 0}, 0, 6.0f, 2.0f, {UnlockHubStuff_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1562, 1562}, 8, {255, 0, 0}, 0, 6.0f, 2.0f, {UnlockHubStuff_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1563, 1563}, 8, {255, 0, 0}, 0, 6.0f, 2.0f, {UnlockHubStuff_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{649, 1558}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {Tag_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1559, 1560}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {VehicleStuff_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1565, 1565}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {ShinyMetal_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{696, 1566}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {ShinyMetal_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1570, 1570}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {Teleport_UpdateHints}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1571, 1571}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {Sith_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1572, 1572}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1573, 1573}, 32, {255, 0, 0}, 0, 6.0f, 2.0f, {GizPanel_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1575, 1575}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {HatMachine_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1576, 1576}, 0, {255, 0, 0}, 0, 6.0f, 2.0f, {HatMachine_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{1577, 1577}, 16, {255, 0, 0}, 0, 6.0f, 2.0f, {Jump_UpdateHint}, NULL, {0, 0, 0, 0}, 0.0f},
    {{-1, -1}, 0, {255, 0, 0}, 1000, 1.0f, 1.0f, {NULL}, NULL, {0, 0, 0, 0}, 0.0f},
};

// Each control mode retains its own requested and interpolated hint row.
static i32 updatehints_target_y[2];
static f32 updatehints_current_y[2];

void MenuUpdateHints(MENU_s *menu);

void MenuDrawHints(MENU_s *menu);

// ==== functions in original address order ====

static __used__ i32 Move_UpdateHint(HINT_s *hint) {
    if (MechInputTouchSystem::s_baseControlMode == 0 || player == NULL || player->apiobj.field_0x27d == 0 ||
        player->field_0x7a5 != 0xff || VehicleArea != 0 || !TouchHacks::TouchControlsActive)
        return 0;
    switch (hint->control_mode_ids[0]) {
        case 0x5f1:
        case 0x5f2:
            return MechInputTouchSystem::s_actualTouchMode == 2;
        case 0x5f3:
            return 1;
    }
    return 0;
}

static __used__ i32 PlayerButton_UpdateHint(HINT_s *hint) {
    if (WORLD == NULL || WORLD->area == HUB_ADATA || VehicleArea != 0)
        return 0;
    if (hint->control_mode_ids[0] == 0x5f4) {
        if (FreePlay == 0)
            return 1;
    } else if (hint->control_mode_ids[0] == 0x5f5) {
        if (FreePlay != 0)
            return 1;
    }
    return 0;
}

static __used__ i32 HoldTag_UpdateHint(HINT_s *) {
    if (MechInputTouchSystem::s_baseControlMode == 0 || WORLD == NULL || WORLD->area == HUB_ADATA) {
        return 0;
    }
    return VehicleArea == 0;
}

static __used__ i32 UnlockHubStuff_UpdateHint(HINT_s *hint) {
    if (WORLD == NULL || WORLD->current_level != HUB_LDATA)
        return 0;
    switch (hint->control_mode_ids[0]) {
        case 0x619:
            if (show_unlock_shop_hint != 0)
                return 1;
            break;
        case 0x61a:
            if (show_unlock_customiser_hint != 0)
                return 1;
            break;
        case 0x61b:
            if (show_unlock_minikitviewer_hint != 0)
                return 1;
            break;
    }
    return 0;
}

static __used__ i32 Jump_UpdateHint(HINT_s *hint) {
    if (player == NULL || player->field_0x7a5 != 0xff || VehicleArea != 0)
        return 0;
    const CHARACTERDATA *data = player->apiobj.character_data;
    const u32 flags = data->model_flags;
    if ((flags & 0x2010) != 0 || (data->player_config->flags_094 & 4) != 0)
        return 0;
    switch (hint->control_mode_ids[0]) {
        case 0x604:
            return 1;
        case 0x629:
            if (MechInputTouchSystem::s_baseControlMode == 0)
                return 0;
            // This hint shares the double-jump eligibility check below.
        case 0x606:
            if ((flags & 8) != 0 || (data->player_config->flags_090 & 0x400000) != 0)
                return 1;
            break;
        case 0x26b:
        case 0x608:
            if ((flags & 8) != 0)
                return 1;
            break;
    }
    return 0;
}

static __used__ i32 Lever_UpdateHint(HINT_s *) {
    if (show_lever_hint != 0) {
        show_lever_hint = 0;
        return true;
    }
    return false;
}

static __used__ i32 Attack_UpdateHint(HINT_s *) {
    if (player == NULL || player->field_0x7a5 != 0xff || VehicleArea != 0)
        return 0;
    const u32 flags = player->apiobj.character_data->model_flags;
    if ((flags & 0x2010) != 0 || (flags & 0x88) == 0)
        return 0;
    return player->ai.opponent != NULL;
}

static __used__ void PlayerButton_PlayHint(HINT_s *) {
    const VuVec &position = MechSystems::Get()->PlayerButton().position;
    MechSystems::Get()->NewRadarPulse(position, false);
}

static __used__ i32 HatMachine_UpdateHint(HINT_s *hint) {
    if (show_hatmachine_hint == 5 && hint->control_mode_ids[0] == 0x627) {
        show_hatmachine_hint = 0;
        return TTab[0x627] != NULL;
    }
    if (show_hatmachine_hint == 6 && hint->control_mode_ids[0] == 0x628) {
        show_hatmachine_hint = 0;
        return TTab[0x628] != NULL;
    }
    return 0;
}

static __used__ i32 VehicleStuff_UpdateHint(HINT_s *hint) {
    if (VehicleArea == 0)
        return 0;
    AREADATA_s *area = WORLD->area;
    if (area == NULL || (area->flags & 4) != 0 || player == NULL || player->field_0x7a5 != 0xff)
        return 0;
    if (hint->control_mode_ids[0] != 0x617 || area == PODRACE_ADATA || area == PODSPRINT_ADATA ||
        area == BONUS_GUNSHIP_ADATA)
        return 0;
    const GAMECHARACTERDATA *data = player->apiobj.character_data->game_character;
    if (data->field_0x88 > 0.0f)
        return (data->flags_090 & 0x10000) == 0;
    return 0;
}

static __used__ i32 Sith_UpdateHint(HINT_s *) {
    if (FreePlay == 0 || (WORLD->area != NULL && WORLD->area == HUB_ADATA) || player == NULL ||
        player->field_0x7a5 != 0xff)
        return 0;
    if (WORLD->giz_force_sys == NULL || WORLD->giz_force_sys->visible_force_count == 0 ||
        AvailableToPlayer(0xc, -1, 0, 1) != 0)
        return 0;
    for (i32 i = 0; i < WORLD->giz_force_sys->visible_force_count; ++i) {
        GIZFORCE_s *force = WORLD->giz_force_sys->visible_forces[i];
        if ((force->config_flags & 0x10) != 0 && NuVecDistSqr(&player->apiobj.position, &force->position, NULL) < 6.25f)
            return 1;
    }
    return 0;
}

static __used__ i32 AutoJump_UpdateHint(HINT_s *hint) {
    if (hint->control_mode_ids[0] != show_autojump_hint)
        return false;
    if (SuperOptions.touch_controls == 0 || TestForController()) {
        show_autojump_hint = 0;
        return false;
    }
    return true;
}

f32 CurrentHintButtonScale() {
    return __builtin_fabsf(NuSinf(GlobalTimer.time_elapsed * 3.0f)) * 0.45f + 1.0f;
}

f32 CurrentHintAlpha() {
    if (hintsys.active_hint == NULL)
        return 0.0f;
    f32 alpha = 1.0f;
    f32 duration = hintsys.active_hint->display_duration;
    f32 fade_start = duration - 0.5f;
    if (duration > 0.0f && hintsys.display_elapsed >= fade_start) {
        alpha = 1.0f - (hintsys.display_elapsed - fade_start) / (duration - fade_start);
        if (alpha <= 0.0f)
            alpha = 0.0f;
    }
    return alpha * hintsys.alpha;
}

static void DrawHint_LSWBody(HINT_s *hint, i32 max_lines) {
    const f32 alpha = CurrentHintAlpha();
    if (!(alpha > 0.0f))
        return;
    if (alpha == 1.0f)
        hint->flags |= 0x40;

    f32 text_y = 0.0f;
    if ((WORLD->current_level->flags & LEVEL_STATUS) == 0)
        text_y = hint->control_mode_ids[0] == 0x164 ? -0.1f : -0.7f;

    f32 text_x;
    i32 justify;
    if ((hint->flags & 0x04) != 0) {
        text_x = 0.0f;
        justify = 0;
    } else {
        text_x = 0.09f - ICONX;
        justify = 2;
    }
    f32 icon_x = -ICONX;

    const i32 control_mode = MechInputTouchSystem::s_baseControlMode;
    char *hint_text = TTab[hint->control_mode_ids[control_mode]];
    if (hint_text != NULL) {
        char expanded[1024];
        Text_ExpandAllButtonStrings(hint_text, expanded);

        const u16 pulse_angle = static_cast<u16>(NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f);
        const f32 colour_pulse = NU_SIN_LUT(pulse_angle) * 0.5f + 0.5f;
        i32 colour_index = static_cast<i8>(hint->pad_0x05[0]);
        if (static_cast<i8>(hint->pad_0x05[0]) == -1)
            colour_index = hintsys.state;
        i32 colours[3];
        for (i32 channel = 0; channel < 3; ++channel) {
            const f32 base = static_cast<f32>(HintRGB[colour_index][channel]);
            colours[channel] = static_cast<i32>(((base + 255.0f) * 0.5f - base) * colour_pulse + base);
        }

        const i32 text_lines = max_lines == -1 ? 3 : max_lines;
        g_buttonFontScalePulse = CurrentHintButtonScale();
        const u8 text_alpha = static_cast<u8>(static_cast<i32>(alpha * 128.0f));
        const f32 max_width = justify != 0 ? 2.0f - (1.0f + text_x) - 0.15f : 1.4f;
        SmartTextEx(expanded, text_x, text_y, 1.0f, 0.5f, 0.5f, 0.5f, justify, colours[0], colours[1], colours[2],
                    max_width, text_lines, NULL, 0, text_alpha);
        if (justify == 0)
            icon_x = -0.5f * smarttextex_longestwidth - 0.075f;
        g_buttonFontScalePulse = 1.0f;
    }

    LEVEL_OBJECT_RUNTIME *icon = &WORLD->lev_objs[0xd4];
    if (icon->active == 0)
        return;

    const i32 slide_angle = static_cast<i32>(alpha * 16384.0f);
    const f32 wave_x = (icon_x + 1.1f) * NU_SIN_LUT(slide_angle) - 1.1f;
    if (hintIconPos.duration < 0.0f || hintIconPos.elapsed >= hintIconPos.duration + hintIconPos.delay)
        hintIconPos.value.x = wave_x;

    if ((hint->flags & HINT_DIRECT_DISPLAY) != 0) {
        const u16 rotation = static_cast<u16>((NuFmod(GameTimer.time_elapsed, 2.5f) / 2.5f) * 65536.0f);
        DrawPanel3DObjectNoAlpha(wave_x, text_y, 1.0f, 0.5f, 0.5f, 0.5f, 0, rotation, 0, &icon->special, 2);
    } else {
        const f32 bounce_scale = TouchHacks::TouchControlsActive ? 0.2f : 0.8f;
        const u16 rotation = static_cast<u16>((NuFmod(GameTimer.time_elapsed, 2.5f) / 2.5f) * 65536.0f);
        const f32 bounce = NU_SIN_LUT(static_cast<i32>(hintYPop.value));
        const f32 scale = hintIconScale.value;
        DrawPanel3DObjectNoAlpha(hintIconPos.value.x, hintIconPos.value.y + bounce_scale * bounce, hintIconPos.value.z,
                                 scale, scale, scale, 0, rotation, 0, &icon->special, 2);
    }
    if (justify == 0) {
        const u16 mirrored_rotation = static_cast<u16>((NuFmod(GameTimer.time_elapsed, 2.5f) / 2.5f) * 65536.0f);
        DrawPanel3DObjectNoAlpha(-wave_x, text_y, 1.0f, 0.5f, 0.5f, 0.5f, 0, mirrored_rotation, 0, &icon->special, 2);
    }
}

void DrawHint_LSW(HINT_s *hint, i32 max_lines) {
    if (hint == NULL || __builtin_expect(FadeSys.fade != 0.0f, 0))
        return;
    DrawHint_LSWBody(hint, max_lines);
}

void initGameHintSys_LSW() {
    RegisterWithHintSys(DrawHint_LSW, Hints_LSW, Game.hint_completion_bits, 3);
    LSW_HintConditions.force_used = 0;
    LSW_HintConditions.tc14_present = 0;
    LSW_HintConditions.panel_used = 0;
    id_HINT_LSW_AUTOJUMP = 0x620;
    id_HINT_LSW_AUTOJUMP_FAIL = 0x621;
}

void initGameHintSys_Batman() {
}

void SET_HINT_COMPLETE(i32 hint_id) {
    if (hint_id < 0) {
        return;
    }

    i32 word = hint_id >> 5;
    if (word > 2) {
        return;
    }
    if (MechInputTouchSystem::s_baseControlMode != 0) {
        word += 3;
    }
    Game.hint_completion_bits[word] |= 1U << (hint_id & 0x1f);
}

void CLEAR_HINT_COMPLETE(i32 hint_id) {
    if (hint_id < 0) {
        return;
    }

    i32 word = hint_id >> 5;
    if (word > 2) {
        return;
    }
    if (MechInputTouchSystem::s_baseControlMode != 0) {
        word += 3;
    }
    Game.hint_completion_bits[word] &= ~(1U << (hint_id & 0x1f));
}

i32 HINT_COMPLETE(i32 hint_id) {
    if (hint_id < 0) {
        return 0;
    }

    i32 word = hint_id >> 5;
    if (word > 2) {
        return 0;
    }
    if (MechInputTouchSystem::s_baseControlMode != 0) {
        word += 3;
    }
    return Game.hint_completion_bits[word] & (1U << (hint_id & 0x1f));
}

void MenuUpdateHints(MENU_s *menu) {
    if (menu->input_activity != 0 && menu->confirm_pressed != 0) {
        if (menu->selected_item == 0)
            menu->up_pressed = 1;
        else
            menu->down_pressed = 1;
    }
    if (menu->cancel_pressed != 0) {
        BackupMenu();
    } else if (menu->up_pressed != 0) {
        if (updatehints_target_y[MechInputTouchSystem::s_baseControlMode] > 0)
            --updatehints_target_y[MechInputTouchSystem::s_baseControlMode];
    } else if (menu->down_pressed != 0) {
        i32 &target = updatehints_target_y[MechInputTouchSystem::s_baseControlMode];
        target = static_cast<i32>(static_cast<u32>(target) + 1U);
    }

    const i32 mode = MechInputTouchSystem::s_baseControlMode;
    i32 count = 0;
    for (HINT_s *hint = Hints_LSW; hint->control_mode_ids[0] != -1; ++hint) {
        if ((hint->flags & 0x2c) == 0 &&
            (mode == 0 ? (hint->flags & 0x10) == 0 : hint->control_mode_ids[1] != -1) &&
            TTab[hint->control_mode_ids[mode]] != NULL)
            ++count;
    }
    if (count <= updatehints_target_y[mode])
        updatehints_target_y[mode] = count - 1;
    updatehints_current_y[mode] =
        SeekValF(updatehints_current_y[mode], static_cast<f32>(updatehints_target_y[mode]), 5.0f);
}

void MenuDrawHints(MENU_s *menu) {
    const f32 text_x = 0.09f - ICONX;
    f32 icon_x = -ICONX;
    const u16 pulse_angle =
        static_cast<u16>(static_cast<i32>(NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f));
    const f32 pulse = NU_SIN_LUT(pulse_angle) * 0.5f + 0.5f;
    const u8 red =
        static_cast<u8>(static_cast<i32>(HintRGB[2][0] + ((HintRGB[2][0] + 255.0f) * 0.5f - HintRGB[2][0]) * pulse));
    const u8 green =
        static_cast<u8>(static_cast<i32>(HintRGB[2][1] + ((HintRGB[2][1] + 255.0f) * 0.5f - HintRGB[2][1]) * pulse));
    const u8 blue =
        static_cast<u8>(static_cast<i32>(HintRGB[2][2] + ((HintRGB[2][2] + 255.0f) * 0.5f - HintRGB[2][2]) * pulse));
    if (MenuAlpha < 1.0f) {
        const f32 start_x = -1.0f - (1.0f - __builtin_fabsf(icon_x));
        icon_x = start_x + (icon_x - start_x) * NU_SIN_LUT(static_cast<i32>(MenuAlpha * 16384.0f));
    }
    f32 y = -0.15f - -0.35f * updatehints_current_y[MechInputTouchSystem::s_baseControlMode];
    const f32 height = 0.15f / GetAspectRatio();
    Text3D(ASCII_UP, 0.85f, 0.15f, 1.0f, 1.2f, 1.2f, 1.2f, 0, red, green, blue);
    menu->item_column[0] = 0;
    menu->item_row[0] = 0;
    menu->item_x[0] = 0.85f;
    menu->item_y[0] = 0.15f;
    menu->item_width[0] = 0.15f;
    menu->item_height[0] = height;
    Text3D(ASCII_DOWN, 0.85f, -0.55f, 1.0f, 1.2f, 1.2f, 1.2f, 0, red, green, blue);
    menu->item_column[1] = 0;
    menu->item_row[1] = 1;
    menu->item_x[1] = 0.85f;
    menu->item_y[1] = -0.55f;
    menu->item_width[1] = 0.15f;
    menu->item_height[1] = height;

    i32 row = 0;
    for (HINT_s *hint = Hints_LSW; hint->control_mode_ids[0] != -1; ++hint) {
        if ((hint->flags & 0x2c) != 0)
            continue;
        const i32 mode = MechInputTouchSystem::s_baseControlMode;
        if ((hint->flags & 0x10) != 0 && mode == 0)
            continue;
        if (hint->control_mode_ids[1] == -1 && mode != 0)
            continue;
        const i32 text_id = hint->control_mode_ids[mode];
        if (TTab[text_id] == NULL)
            continue;
        if (y <= 0.3f) {
            f32 fade = 1.0f;
            if (y > 0.2f)
                fade = 1.0f - (y - 0.2f) / 0.1f;
            g_buttonFontScalePulse = CurrentHintButtonScale();
            char text[1024];
            Text_ExpandAllButtonStrings(TTab[text_id], text);
            SmartTextEx(text, text_x, y, 1.0f, 0.5f, 0.5f, 0.5f, 2, red, green, blue, 1.9f - (1.0f + text_x) - 0.15f, 3,
                        NULL, 0, static_cast<i32>(static_cast<f32>(MenuA) * fade));
            g_buttonFontScalePulse = 1.0f;
            const f32 scale = 0.5f * fade;
            nuhspecial_s *special = &WORLD->lev_objs[0xd4].special;
            const u16 rotation = static_cast<u16>(
                static_cast<i32>(NuFmod(GlobalTimer.time_elapsed, 2.5f) / 2.5f * 65536.0f) + row * 0x1555);
            DrawPanel3DObjectNoAlpha(icon_x, y, 1.0f, scale, scale, scale, 0, rotation, 0, special, 2);
        }
        y -= 0.35f;
        if (y <= -1.5f)
            break;
        ++row;
    }
}

