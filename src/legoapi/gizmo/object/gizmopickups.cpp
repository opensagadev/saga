#include "decomp.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

u8 CoinTab[4] = {0, 1, 2, 3};

void Pup_CollectCoin(WORLDINFO_s *, GIZMOPICKUP_s *, i32, GameObject_s *, i32);

GIZMO_PICKUP_TYPE GizmoPickupType[10] = {
    {"Silver Coin", NULL, NULL, 's', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00b7, 10, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Gold Coin", NULL, NULL, 'g', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00bf, 100, 0.045f, 0.045f, 10.0f, 0.0f, NULL,
     Pup_CollectCoin, -1, -1, 0.0f, 0.0f},
    {"Blue Coin", "PickupCoinB", NULL, 'b', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00c7, 1000, 0.045f, 0.045f, 6.0f, 0.0f,
     NULL, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Purple Coin", "PickupCoinB", NULL, 'p', 4, GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d5, 10000, 0.045f, 0.045f, 6.0f, 0.0f,
     NULL, Pup_CollectCoin, 8, -1, 0.0f, 0.0f},
    {"Minikit", "MK-Appear", NULL, 'm', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_MINIKIT_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00ce, 0,
     0.125f, 0.15f, 2.0f, 0.05f, NULL, NULL, 20, -1, 0.0f, 0.0f},
    {"Heart", NULL, NULL, 'h', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x00cb, 0, 0.1f, 0.08f, 4.0f, 0.0f, NULL, NULL,
     -1, -1, 0.0f, 0.0f},
    {"Red Brick", "MK-Appear", NULL, 'r', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_RED_BRICK_DETECTOR | GIZMOPICKUP_TYPE_FLAG_40, 0, 0x00d2, 0,
     0.12f, 0.06f, 2.0f, 0.1f, NULL, NULL, 99, -1, 0.0f, 0.0f},
    {"Charkit", "MK-Appear", "MK-Pickup", 'c', 0,
     GIZMOPICKUP_TYPE_DRAW_TUMBLING | GIZMOPICKUP_TYPE_CHALLENGE_MODE_FILTER, 0, 0x00cf, 0, 0.125f, 0.15f, 2.0f, 0.05f,
     NULL, NULL, 20, -1, 0.0f, 0.0f},
    {"Torpedo", NULL, NULL, 't', 0, GIZMOPICKUP_TYPE_DRAW_Y_ROTATION, 0, 0x0079, 0, 0.1f, 0.1f, 5.0f, 0.0f, NULL, NULL,
     -1, -1, 0.0f, 0.0f},
    {"Power Up", "MK-Appear", NULL, 'u', 0, GIZMOPICKUP_TYPE_DRAW_TUMBLING, 0, 0x00d0, 0, 0.125f, 0.125f, 2.0f, 0.05f,
     NULL, NULL, -1, 0x00d1, 0.0f, 0.0f},
};

GIZMOPICKUPSYS_s GizmoPickupSys_Game = {
    GizmoPickupType, CoinTab, 10, 4, 4, 2, 0, 0,
};

WORLDINFO_s *WorldInfo_CurrentlyActive();
i32 Arcade_GetMode(u32 *);
extern f32 COINMSGTIME;
extern "C" void PlaySfx(char *, NUVEC *);
void MakePartVector(NUVEC *, NUVEC *, f32);
void AddCoinsAsParts(i32, NUVEC *, NUVEC *, f32, f32);
void AddHeartAsPart(GameObject_s *, NUVEC *, NUVEC *, f32, f32);
void AddTorpedoAsPart(NUVEC *, NUVEC *, f32, f32);
void PowerUp_AddPart(NUVEC *, NUVEC *, f32, f32);
void AddCoinsToPanel(i32, NUVEC *, i32, f32, GameObject_s *, i32);

void AddPickups(i32 coins, i32 hearts, i32 torpedoes, i32 powerups, nuvec_s *position, nuvec_s *direction, float,
                i32 player_id, float speed, float lifetime, GameObject_s *owner, i32, i32 panel, bool consolidate) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    const f32 scale = AreaPickupScale;
    u32 arcade_flags;
    Arcade_GetMode(&arcade_flags);
    if ((arcade_flags & 0x40) != 0)
        coins = torpedoes = 0;
    i32 to_panel;
    if ((world->current_level->flags & LEVEL_PICKUPS_TO_PANEL) != 0) {
        to_panel = 1;
        if (player_id == -1) {
            if (player2 != NULL)
                player_id = qrand() / 32768;
            else
                player_id = Player[0] != NULL && (Player[0]->apiobj.flags_low & 0x80) != 0 ? 0 : 1;
        }
    } else if (world->area != NULL && (world->area->flags & 0x100) != 0) {
        player_id = qrand() / 32768;
        to_panel = 1;
    } else {
        to_panel = player_id == -1 ? 0 : panel;
    }
    if (ChallengeMode != 0 || Mission_Active(NULL) != NULL)
        coins = 0;
    if (speed == 1.0f)
        speed = scale;
    u8 counts[10] = {};
    if (netclient != 0)
        return;
    i32 remainder = coins % 10;
    if (remainder > 0)
        coins -= remainder;
    if (coins > 512000)
        coins = 512000;
    bool has_coins = false;
    if (coins > 0) {
        PlaySfx("CoinsLand", position);
        i32 remaining = coins;
        for (i32 i = 3; remaining > 0; --i) {
            i32 type = CoinTab[i];
            while (remaining - GizmoPickupType[type].score >= 0) {
                remaining -= GizmoPickupType[type].score;
                ++counts[type];
            }
        }
        has_coins = true;
    }
    if (world->area != NULL && (world->area->flags & 0x104) == 0 && consolidate) {
        i32 silver = counts[0], gold = counts[1], blue = counts[2], purple = counts[3];
        if (silver > 1) {
            silver = 0;
            ++gold;
            if (gold > 9) {
                gold %= 10;
                ++blue;
                if (blue > 9) {
                    blue %= 10;
                    ++purple;
                }
            }
        }
        if (gold > 4) {
            gold = qrand() / 32768;
            ++blue;
            if (blue > 9) {
                blue %= 10;
                ++purple;
            }
        }
        if (blue > 5) {
            blue = qrand() / 65536;
            ++purple;
        }
        counts[0] = silver;
        counts[1] = gold;
        counts[2] = blue;
        counts[3] = purple;
    }
    if (static_cast<u32>(player_id) >= 2)
        player_id = -1;
    NUVEC velocity;
    if (to_panel != 0) {
        if (has_coins)
            DrawBuildUpTime = COINMSGTIME + 1.0f;
        AddCoinsToPanel(coins, position, player_id, speed, owner, 0);
    } else {
        for (i32 i = 0; i < 4; ++i) {
            i32 type = CoinTab[i];
            for (i32 j = 0; j < counts[type]; ++j) {
                MakePartVector(&velocity, direction, speed);
                AddCoinsAsParts(type, position, &velocity, lifetime, scale);
            }
        }
    }
    if (hearts > 0) {
        bool both_players = false;
        if (VehicleArea != 0 && Player[0] != NULL && (Player[0]->apiobj.flags_low & 0x80) != 0 && Player[1] != NULL &&
            (Player[1]->apiobj.flags_low & 0x80) != 0) {
            both_players = true;
            hearts = 2;
        }
        for (i32 i = 0; i < hearts; ++i) {
            if (to_panel != 0)
                velocity = v000;
            else if (TouchHacks::TouchControlsActive && player != NULL) {
                NUVEC toward_player = {player->apiobj.position.x - position->x, 0.0f,
                                       player->apiobj.position.z - position->z};
                if (toward_player.x * toward_player.x + toward_player.y * toward_player.y +
                        toward_player.z * toward_player.z >
                    1.0f)
                    NuVecNorm(&toward_player, &toward_player);
                toward_player.y =
                    AreaPickupGravity == 0.0f ? 0.0f : static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;
                velocity = toward_player;
            } else
                MakePartVector(&velocity, direction, speed);
            GameObject_s *recipient = owner;
            if (both_players)
                recipient = i == 0 ? Player[0] : Player[1];
            else if (recipient == NULL) {
                GameObject_s *eligible[2];
                i32 count = 0;
                if (Player[0] != NULL && (Player[0]->apiobj.flags_low & 0x80) != 0)
                    eligible[count++] = Player[0];
                if (Player[1] != NULL && (Player[1]->apiobj.flags_low & 0x80) != 0)
                    eligible[count++] = Player[1];
                if (count != 0)
                    recipient = eligible[count == 1 ? 0 : qrand() / 32768];
            }
            AddHeartAsPart(recipient, position, &velocity, scale, lifetime);
        }
    }
    for (i32 i = 0; i < torpedoes; ++i) {
        MakePartVector(&velocity, direction, speed);
        AddTorpedoAsPart(position, &velocity, scale, lifetime);
    }
    if ((world->current_level->flags & LEVEL_PICKUPS_TO_PANEL) == 0) {
        for (i32 i = 0; i < powerups; ++i) {
            MakePartVector(&velocity, direction, speed);
            PowerUp_AddPart(position, &velocity, scale, lifetime);
        }
    }
}

i32 IsACoinType(i32 type) {
    for (i32 i = 0; i < 4; ++i) {
        if (CoinTab[i] == type) {
            return 1;
        }
    }
    return 0;
}

void AddMiscPickups(nuvec_s *, i32, i32, i32) {
}

i32 GetRandomCoinType() {
    i32 random_value = qrand();
    if (random_value < 0) {
        random_value += 0x3fff;
    }
    return CoinTab[random_value >> 14];
}

i32 OutSideSplineArea(NUVEC *, nugspline_s *, NUVEC *, NUVEC *, i32);

i32 InDoubleScoreZone(GameObject_s *object) {
    if (Mission_Active(NULL) != NULL)
        return 0;
    if ((WORLD->current_level->flags & LEVEL_DOUBLE_SCORE) != 0)
        return 1;
    for (i32 i = 19; i < 24; ++i) {
        nugspline_s *spline = reinterpret_cast<nugspline_s *>(WORLD->portal_places[i]);
        if (spline != NULL && OutSideSplineArea(&object->apiobj.collision_position, spline, NULL, NULL, 0) == 0)
            return 1;
    }
    return 0;
}

void DropTorpedoPickups(TORPEDOPACKET_s *, i32) {
}
