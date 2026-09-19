#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"

extern i32 DoubleScore;
extern TIMER BonusTimer;

void NewBuzz(nupad_s *pad, f32 amount, i32 flags);

void GizmoPickup_CollectCoin(WORLDINFO_s *world, nuvec_s *position, i32 type_index, i32 model_variant,
                             GameObject_s *object, i32) {
    i32 player_index;
    if (object != NULL) {
        player_index = object->apiobj.field_0x27c;
        if (player_index > 1) {
            return;
        }
    } else {
        i32 player0_active = Player[0] == NULL ? 0 : static_cast<i8>(Player[0]->apiobj.flags_low) < 0;
        if (Player[1] == NULL || static_cast<i8>(Player[1]->apiobj.flags_low) >= 0) {
            if (player0_active == 0) {
                return;
            }
            player_index = 0;
        } else if (player0_active == 0) {
            player_index = 1;
        } else {
            player_index = qrand() / 0x8000;
        }
    }

    if (static_cast<u32>(type_index - 2) < 2 && object != NULL) {
        if (world->area == NULL || (world->area->flags & AREAFLAG_SUPER_BONUS_AREA) != AREAFLAG_BONUS_AREA) {
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        } else {
            NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
        }
    }

    PlaySfx(BonusTimer.time_elapsed > 0.0f || static_cast<u32>(type_index - 2) < 2 ? const_cast<char *>("PickupCoinB")
                                                                                   : const_cast<char *>("PickupCoin"),
            position);

    const bool main_total = CoinsGoToMainTotal() != 0;
    NUVEC target_position;
    ADDGAMEMSG message = AddGameMsg_Default;
    if (main_total) {
        target_position.x = cointotal_x[player_index];
        target_position.y = STATSPOSY;
        DrawCoinTotalTime = COINMSGTIME + FRAMETIME;
        message.target_scale = COINTOTAL_COINSIZE;
    } else {
        target_position.x = player_index == 1 ? PANEL_COINX : -PANEL_COINX;
        target_position.y = STATSPOSY + PANEL_COINY;
        DrawBuildUpTime = COINMSGTIME + FRAMETIME;
        message.target_scale = PANEL_COINSCALE_END;
    }
    target_position.z = 1.0f;

    GIZMO_PICKUP_TYPE *type = &GizmoPickupSys_Game.types[type_index];
    const i32 model_id = type->first_model_id + model_variant;
    if (model_id == -1 || world->lev_objs[model_id].active == 0) {
        return;
    }

    u32 score = type->score;
    if (object != NULL && object->coinpacket != NULL) {
        if (((DoubleScore >> player_index) & 1) != 0) {
            score *= 2;
        }
        if (object->field_0xdec > 0.0f) {
            score *= 2;
        }
    }

    message.position = position;
    message.target_position = &target_position;
    message.scale = PANEL_COINSCALE_START * AreaPickupScale;
    message.flags = 0x2012d;
    message.duration = COINMSGTIME;
    message.icon = static_cast<i16>(model_id);
    message.extra_position = reinterpret_cast<nuvec_s *>(&world->lev_objs[model_id]);
    message.score = score;
    if (main_total) {
        message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
    }
    message.end_fn = EndScoreMessage;
    message.player_index = static_cast<i8>(player_index);
    AddGameMsg(&message);
}

static __used__ float GizmoPickups_Collide2D(GameObject_s *) {
    STUBBED();
    return 0;
}
