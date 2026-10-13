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
    } else {
        i32 player0_active = Player[0] == NULL ? 0 : Player[0]->apiobj.player_controlled;
        if (Player[1] == NULL || !Player[1]->apiobj.player_controlled) {
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
    if (static_cast<u32>(player_index) > 1) {
        return;
    }

    if ((type_index == 2 || type_index == 3) && object != NULL) {
        if (world->area != NULL && (world->area->flags & AREAFLAG_SUPER_BONUS_AREA) == AREAFLAG_BONUS_AREA) {
            NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
        } else {
            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
        }
    }

    if (DoubleScoreTime > 0.0f || type_index == 2 || type_index == 3) {
        PlaySfx(const_cast<char *>("PickupCoinB"), position);
    } else {
        PlaySfx(const_cast<char *>("PickupCoin"), position);
    }

    i32 main_total;
    NUVEC target_position;
    f32 target_scale;
    if (CoinsGoToMainTotal() != 0) {
        main_total = 1;
        target_position.x = cointotal_x[player_index];
        target_position.y = STATSPOSY;
        DrawCoinTotalTime = 1.0f + COINMSGTIME;
        target_scale = COINTOTAL_COINSIZE;
    } else {
        main_total = 0;
        target_position.x = player_index == 1 ? PANEL_COINX : -PANEL_COINX;
        target_position.y = STATSPOSY + PANEL_COINY;
        DrawBuildUpTime = 1.0f + COINMSGTIME;
        target_scale = PANEL_COINSCALE_END;
    }
    target_position.z = 1.0f;

    GIZMO_PICKUP_TYPE *type = &GizmoPickupType[type_index];
    const i32 model_id = static_cast<i16>(type->first_model_id) + model_variant;
    if (model_id != -1 && world->lev_objs[model_id].active != 0) {

        ADDGAMEMSG message = AddGameMsg_Default;
        message.target_scale = target_scale;
        u32 score = type->score;
        if (object != NULL && object->coinpacket != NULL) {
            if (((DoubleScore >> player_index) & 1) != 0) {
                score *= 2;
            }
            if (object->field_0xdec > 0.0f) {
                score *= 2;
            }
        }

        message.target_position = &target_position;
        message.position = position;
        message.scale = PANEL_COINSCALE_START * AreaPickupScale;
        message.flags = 0x2012d;
        message.duration = COINMSGTIME;
        message.icon = static_cast<i16>(model_id);
        message.extra_position = reinterpret_cast<nuvec_s *>(&world->lev_objs[model_id]);
        message.score = score;
        if (main_total != 0) {
            message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
        }
        message.end_fn = EndScoreMessage;
        message.player_index = static_cast<i8>(player_index);
        AddGameMsg(&message);
    }
}

static __used__ float GizmoPickups_Collide2D(GameObject_s *object) {
    if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->id == id_SPEEDERBIKE)
        return 2.0f;
    return 0.0f;
}
