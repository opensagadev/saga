#include "decomp.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "globals.h"

static void Pup_CollectCharKit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32) {
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
    AddGameDebris(WORLD->debris_sys, 0x13, &pickup->position);
    if (WORLD->lev_objs[0xcf].active == 0)
        return;
    NUVEC target = {KITPOSX, KITPOSY + PANEL_MINIKITY, 1.0f};
    ADDGAMEMSG message = AddGameMsg_Default;
    message.position = &pickup->position;
    message.target_position = &target;
    message.scale = AreaPickupScale;
    message.target_scale = PANEL_MINIKITSCALE;
    message.flags = 0x2112d;
    message.duration = 1.0f;
    message.icon = 0xcf;
    message.special = &WORLD->lev_objs[0xcf].special;
    message.tick_fn = MiniKit_GameMsg_Update;
    message.end_fn = MiniKit_GameMsg_End;
    message.field_0x4d = 1;
    GAMEMESSAGE_s *queued = AddGameMsg(&message);
    if (queued != NULL)
        queued->target_type = 6;
    DrawMiniKitTime = 2.0f;
    if (AreaGlobals.values.field_0x1c < 10)
        ++AreaGlobals.values.field_0x1c;
}

static void EndRedBrickMessage(GAMEMESSAGE_s *) {
    if (AreaGlobals.values.field_0x08 != 0)
        AreaGlobals.values.field_0x08 = 2;
    RedBrickScale = 2.0f;
    GameAudio_PlaySfx(0x26, NULL, 0, 0);
    NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, -0.2f, 0, NULL);
}

static void Pup_CollectRedBrick(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32) {
    AddGameDebris(WORLD->debris_sys, 0x62, &pickup->position);
    GameAudio_PlaySfxById(GetSfxId(const_cast<char *>("MK-Pickup")), &pickup->position, 3, 0);
    if (WORLD->level_sub_id == -1 || Game.area_save[WORLD->level_sub_id].red_brick_collected == 0) {
        NUVEC target = {REDBRICKPOSX, REDBRICKPOSY, 1.0f};
        ADDGAMEMSG message = AddGameMsg_Default;
        message.position = &pickup->position;
        message.target_position = &target;
        message.scale = AreaPickupScale;
        message.target_scale = PANEL_REDBRICKSCALE;
        message.flags = 0x2112d;
        message.duration = 1.0f;
        message.icon = 0xd2;
        message.special = &WORLD->lev_objs[0xd2].special;
        message.tick_fn = MiniKit_GameMsg_Update;
        message.end_fn = EndRedBrickMessage;
        message.field_0x4d = 1;
        AddGameMsg(&message);
        DrawRedBrickTime = 2.0f;
        if (WORLD->level_sub_id != -1)
            AreaGlobals.values.field_0x08 = 1;
    }
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
}

static void Pup_UpdatePurpleCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup) {
    if ((pickup->state_flags & 0x30) != 0x30) {
        return;
    }
    i32 effect = WORLD->debris_sys->entries[57].effect;
    if (effect == -1) {
        return;
    }
    f32 rate = 5.0f;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x104) == 4) {
        rate = 2.5f;
    }
    i32 count = ParticlesPerSecond(rate, FRAMETIME);
    if (count > 0) {
        AddVariableShotDebrisEffect(effect, &pickup->position, count, 0, 0);
    }
}

static void Pup_UpdateBlueCoin(WORLDINFO_s *, GIZMOPICKUP_s *pickup) {
    if ((pickup->state_flags & 0x30) != 0x30) {
        return;
    }
    i32 effect = WORLD->debris_sys->entries[56].effect;
    if (effect == -1) {
        return;
    }
    f32 rate = 5.0f;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x104) == 4) {
        rate = 2.5f;
    }
    i32 count = ParticlesPerSecond(rate, FRAMETIME);
    if (count > 0) {
        AddVariableShotDebrisEffect(effect, &pickup->position, count, 0, 0);
    }
}

static void Pup_UpdatePowerUp(WORLDINFO_s *world, GIZMOPICKUP_s *pickup) {
    PowerUp_Particles(world, &pickup->position);
    PlaySfx("Grv_GuardWeaponLp", &pickup->position);
}

static void Pup_CollectPowerUp(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags) {
    CollectPowerUp(object, &pickup->position, pickup->draw_rotation, flags);
}

void Pup_CollectCoin(WORLDINFO_s *world, GIZMOPICKUP_s *pickup, i32 type, GameObject_s *object, i32 arg) {
    GizmoPickup_CollectCoin(world, &pickup->position, type, pickup->model_variant, object, arg);
}

static void Pup_CollectMinikit(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 type) {
    CollectMinikit(&pickup->position, pickup->name, type);
    NewBuzz(object->pad_gamepad->pad, 0.2f, 0);
}

static void Pup_CollectHeart(WORLDINFO_s *, GIZMOPICKUP_s *pickup, i32, GameObject_s *object, i32 flags) {
    CollectHitPoint(object, &pickup->position, flags);
}

u8 CoinTab[4] = {0, 1, 2, 3};

GIZMO_PICKUP_TYPE GizmoPickupType[10] = {
    {"Silver Coin", NULL, NULL, 's', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00b7, 10, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Gold Coin", NULL, NULL, 'g', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00bf, 100, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Blue Coin", "PickupCoinB", NULL, 'b', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00c7, 1000, 0.045f, 0.045f, 6.0f, 0.0f,
     Pup_UpdateBlueCoin, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Purple Coin", "PickupCoinB", NULL, 'p', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d5, 10000, 0.045f, 0.045f, 6.0f, 0.0f,
     Pup_UpdatePurpleCoin, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Minikit", "MK-Appear", NULL, 'm', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_MINIKIT_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00ce, 0,
     0.125f, 0.15f, 2.0f, 0.05f, NULL, Pup_CollectMinikit, 20, -1, 0.0f, 0.0f},
    {"Heart", NULL, NULL, 'h', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x00cb, 0, 0.1f, 0.08f, 4.0f, 0.0f, NULL,
     Pup_CollectHeart, -1, -1, 0.0f, 0.0f},
    {"Red Brick", "MK-Appear", NULL, 'r', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_RED_BRICK_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d2, 0,
     0.12f, 0.06f, 2.0f, 0.1f, NULL, Pup_CollectRedBrick, 99, -1, 0.0f, 0.0f},
    {"Charkit", "MK-Appear", "MK-Pickup", 'c', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_CHALLENGE_MODE_FILTER, 0, 0x00cf, 0, 0.125f, 0.15f, 2.0f, 0.05f,
     NULL, Pup_CollectCharKit, 20, -1, 0.0f, 0.0f},
    {"Torpedo", NULL, NULL, 't', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x0079, 0, 0.1f, 0.1f, 5.0f, 0.0f, NULL, NULL,
     -1, -1, 0.0f, 0.0f},
    {"Power Up", "MK-Appear", NULL, 'u', 0, GIZMOPICKUP_TYPE_DRAW_TUMBLING, 0, 0x00d0, 0, 0.125f, 0.125f, 2.0f, 0.05f,
     Pup_UpdatePowerUp, Pup_CollectPowerUp, -1, 0x00d1, 0.0f, 0.0f},
};

GIZMOPICKUPSYS_s GizmoPickupSys_Game = {
    GizmoPickupType, CoinTab, 10, 4, 4, 2, 0, 0,
};
