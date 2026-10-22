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
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/items/collect/thermaldetonators.h"
#include "legoapi/menus/core/hint.h"
#include "legoapi/props/objects/zipup.h"
#include "legoapi/actions/movement/pushblocks.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/menus/screens/shop.h"

LSW_HINTCONDITIONS_s LSW_HintConditions;
HINTSYS_s hintsys = {};

extern "C" i32 editor_active, CutSceneWaiting, PANELOFF;

#include "legoapi/menus/core/gamehint_types.h"

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
    if (__builtin_expect(active_hint != NULL, 1)) {
        display_elapsed += elapsed;
        if (__builtin_expect(
                MechSystems::Get()->PlayerButton().selector != NULL ||
                    (active_hint->display_duration > 0.0f && display_elapsed >= active_hint->display_duration),
                1)) {
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
    if (!(hovered)) {
        scale_to = 0.5f;
        scale = scale_to;
    } else {
        scale_to = 0.6f;
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
        if (NewMode == 0 && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
            GameTimer.update_count != 0 && WORLD != NULL && WORLD->current_level != TITLES_LDATA &&
            CutSceneWaiting == 0 && Paused == 0 && CUTSTOPGAME == 0 && MiniCutCam == 0 && PANELOFF == 0 &&
            WORLD->current_level != STATUS_LDATA && (WORLD->current_level->flags & 0x400) == 0)
            visible = 1;
    }
    disabled = (GetMenuID() != -1 && GetMenuID() != 13) || Paused != 0 || NewMode != 0 || NewLData != NULL ||
               CUTSTOPGAME != 0 || MiniCutCam != 0 || FadeSys.fade > 0.0f || player == NULL;
    if (!pulse_active && (slide_duration < 0.0f || !(slide_elapsed < slide_duration + slide_delay)) && slide > -1.0f)
        StartPulse();
}

// Static game message and hint helpers. Stubbed to satisfy the symbol baseline.

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.
static __used__ i32 Attack_UpdateHint(HINT_s *);

u8 show_unlock_shop_hint = 0;
u8 show_unlock_customiser_hint = 0;
u8 show_unlock_minikitviewer_hint = 0;

static __attribute__((noinline, noclone)) void DrawHint_LSWBody(HINT_s *hint, i32 max_lines)
    __asm__("_Z12DrawHint_LSWP6HINT_si.part.19");

// Original local callbacks referenced by Hints_LSW.
extern u8 show_lever_hint;
static __used__ i32 Jump_UpdateHint(HINT_s *hint);
static __used__ i32 Move_UpdateHint(HINT_s *hint);
static __used__ i32 Lever_UpdateHint(HINT_s *);
static __used__ i32 AutoJump_UpdateHint(HINT_s *hint);
static __used__ i32 VehicleStuff_UpdateHint(HINT_s *hint);
static __used__ i32 HoldTag_UpdateHint(HINT_s *);

u8 show_hatmachine_hint;

static __used__ i32 HatMachine_UpdateHint(HINT_s *hint);

i32 id_HINT_LSW_AUTOJUMP = -1;
i32 id_HINT_LSW_AUTOJUMP_FAIL = -1;

static __used__ i32 Sith_UpdateHint(HINT_s *);
static __used__ void PlayerButton_PlayHint(HINT_s *);
static __used__ i32 UnlockHubStuff_UpdateHint(HINT_s *hint);
static __used__ i32 PlayerButton_UpdateHint(HINT_s *hint);

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

static i32 updatehints_target_y[2];
static f32 updatehints_current_y[2];

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
    if (!(hintIconPos.duration >= 0.0f) || hintIconPos.elapsed >= hintIconPos.duration + hintIconPos.delay)
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
    const f32 scroll = -0.35f * updatehints_current_y[MechInputTouchSystem::s_baseControlMode];
    f32 y = -0.15f - scroll;
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

