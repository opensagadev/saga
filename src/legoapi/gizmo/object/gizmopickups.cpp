#include "decomp.h"
#include "batman.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

WORLDINFO_s *WorldInfo_CurrentlyActive();
void MakePartVector(NUVEC *, NUVEC *, f32);
void AddCoinsAsParts(i32, NUVEC *, NUVEC *, f32, f32);
void AddHeartAsPart(GameObject_s *, NUVEC *, NUVEC *, f32, f32);
void AddTorpedoAsPart(NUVEC *, NUVEC *, f32, f32);
void PowerUp_AddPart(NUVEC *, NUVEC *, f32, f32);
i32 GetRandomCoinType() {
    i32 random_value = qrand();
    if (random_value < 0) {
        random_value += 0x3fff;
    }
    return CoinTab[random_value >> 14];
}

void AddCoinsToPanel(i32 coins, nuvec_s *position, i32 player, float, GameObject_s *, i32 random_types) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (coins == 0)
        return;
    if (ChallengeMode || Mission_Active(NULL) != NULL)
        coins = 0;
    u8 counts[10] = {};
    i32 remainder = coins % 10;
    if (remainder > 0)
        coins -= remainder;
    if (coins > 512000)
        coins = 512000;
    if (coins > 0)
        PlaySfx("CoinsLand", position);
    if (random_types) {
        while (coins > 0) {
            i32 type = GetRandomCoinType();
            i32 remaining = coins - GizmoPickupType[type].score;
            if (remaining >= 0) {
                ++counts[type];
                coins = remaining;
            }
        }
    } else if (coins > 0) {
        for (i32 i = 3; coins > 0; --i) {
            i32 type = CoinTab[i];
            while (coins - GizmoPickupType[type].score >= 0) {
                coins -= GizmoPickupType[type].score;
                ++counts[type];
            }
        }
    }
    if (static_cast<u32>(player) > 1)
        player = -1;
    NUVEC target;
    target.x = player == 1 ? PANEL_COINX : -PANEL_COINX;
    bool main_total = CoinsGoToMainTotal() != 0;
    f32 target_scale;
    if (main_total) {
        target.y = STATSPOSY;
        target_scale = COINTOTAL_COINSIZE;
    } else {
        target.y = STATSPOSY + PANEL_COINY;
        target_scale = PANEL_COINSCALE_END;
    }
    target.z = 1.0f;
    DrawBuildUpTime = COINMSGTIME + 1.0f;
    for (i32 i = 0; i < 4; ++i) {
        GIZMO_PICKUP_TYPE *type = &GizmoPickupType[CoinTab[i]];
        i32 base_model = static_cast<i16>(type->first_model_id);
        for (i32 j = 0; j < counts[CoinTab[i]]; ++j) {
            i32 model = base_model;
            if (type->random_model_count != 0)
                model += qrand() / (65535 / type->random_model_count + 1);
            if (world->lev_objs[model].active == 0 || player == -1)
                continue;
            ADDGAMEMSG_ALIGNED16 message = AddGameMsg_Default;
            message.position = position;
            message.target_position = &target;
            message.target_scale = target_scale;
            message.icon = model;
            message.extra_position = reinterpret_cast<NUVEC *>(&world->lev_objs[message.icon]);
            message.score = type->score;
            message.end_fn = EndScoreMessage;
            message.player_index = player;
            message.field_0x4d = 1;
            message.field_0x20 = AddCoinDelay[player];
            message.flags = 0x12d;
            message.scale = 1.0f;
            message.duration = COINMSGTIME;
            if (main_total) {
                message.update_fn = GameMsg_DrawAdjustNewPos_CoinToTotal;
                message.field_0x4e = 1;
            }
            AddGameMsg(&message);
            AddCoinDelay[player] += 0.1f;
        }
    }
}

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

void AddMiscPickups(nuvec_s *, i32, i32, i32) {
    STUBBED();
}

i32 IsACoinType(i32 type) {
    for (i32 i = 0; i < 4; ++i) {
        if (CoinTab[i] == type) {
            return 1;
        }
    }
    return 0;
}

i32 LoseCoins(GameObject_s *object, i32 cause) {
    if (Player_HasInvincibility(object) != 0 ||
        (object->apiobj.character_data->game_character->flags_090 & 0x8000) != 0 || object->coinpacket == NULL ||
        (Arcade != 0 && (Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].field8_0x8 & 8) != 0)) {
        return 0;
    }
    u32 lost = 0;
    if (cause == 1) {
        lost = static_cast<u32>(TouchHacks::GetLoseStudsDieValue());
    } else if (cause == 2) {
        lost = static_cast<u32>(TouchHacks::GetLoseStudsFallValue());
    }
    if (lost != 0) {
        const i32 adjustment = adtab[adaptivedifficulty[0]][0];
        if (adjustment == 1) {
            lost *= 2;
        } else if (adjustment == -1) {
            lost >>= 1;
        }
        if (lost > object->coinpacket->coins) {
            lost = object->coinpacket->coins;
            object->coinpacket->coins = 0;
        } else if (lost != 0) {
            object->coinpacket->coins -= lost;
        }
    }
    if (Cheats_CheckFlags(0x7c) != 0 || DoubleScoreTime > 0.0f) {
        return 0;
    }
    return static_cast<i32>(lost);
}

void GizmoPickups_SetOnOff() {
    u32 arcade_flags;
    Arcade_GetMode(&arcade_flags);
    WORLDINFO_s *world = WORLD;
    for (i32 index = 0; index < 10; ++index) {
        if (ChallengeMode != 0) {
            if (index == 7) {
                GizmoPickupType[index].field_0x0f = 0;
            } else {
                GizmoPickupType[index].field_0x0f = 1;
            }
        } else if (index == 7) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 6 && world->level_sub_id != -1 && Game.area_save[world->level_sub_id].field_0x5[1] != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 4 && SuperStory != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else if (index == 9 && world->area != NULL && (world->area->flags & 0x100) != 0) {
            GizmoPickupType[index].field_0x0f = 1;
        } else {
            GizmoPickupType[index].field_0x0f = (arcade_flags & 0x20) != 0 && index != 9 && index != 5;
        }
    }
}
