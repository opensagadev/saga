#include "decomp.h"
#include "globals.h"
#include "batman.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/world/mission.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/misc.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include <stdio.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void TimingBars(void);
void Arcade_ResetPanel(void);

static f32 redbrickslidetime;
f32 TimerAlpha = 0.0f;
f32 TimerScale = 1.0f;

namespace {
    enum PANEL_BLOCKING_MENU {
        PANEL_MENU_EPISODE_I = 20,
        PANEL_MENU_EPISODE_II = 21,
        PANEL_MENU_EPISODE_III = 22,
        PANEL_MENU_EPISODE_IV = 23,
        PANEL_MENU_SAVE = 25,
        PANEL_MENU_LOAD = 26,
    };

    bool CoinTotalCanOpen() {
        if (FadeSys.fade != 0.0f || CUTSTOPGAME != 0) {
            return false;
        }
        if (Paused == 0 && NetPaused == 0 && DrawCoinTotalTime <= 0.0f) {
            return false;
        }
        if (screendump != 0 || MenuInMemoryCard() != 0) {
            return false;
        }

        const i32 menu = GetMenuID();
        return menu != PANEL_MENU_EPISODE_I && menu != PANEL_MENU_EPISODE_II && menu != PANEL_MENU_EPISODE_III &&
               menu != PANEL_MENU_EPISODE_IV && menu != PANEL_MENU_SAVE && menu != PANEL_MENU_LOAD;
    }
} // namespace

enum COIN_TOTAL_SOURCE { COIN_TOTAL_SAVED_GAME, COIN_TOTAL_SUPER_STORY, COIN_TOTAL_BONUS };
static f32 DrawCoinTotalY = 2000000.0f;
static void DrawCoinTotal(i32 source, i32 hide_super_story_target) {
    if (FadeSys.fade != 0.0f || (WORLD->current_level->flags & LEVEL_GAMEPLAY) == 0) {
        return;
    }

    const f32 timer = source == COIN_TOTAL_BONUS ? statstime : cointotaltime;
    const i32 angle = static_cast<i32>(timer * static_cast<f32>(NUANG_90DEG));
    const f32 y = NuTrigTable[(angle >> 1) & 0x7fff] * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y + COINTOTAL_SCOREDY;

    DrawCoinTotalY = y;

    i32 total;
    i32 red = 255;
    i32 green = 191;
    i32 blue = 0;

    if (source == COIN_TOTAL_SUPER_STORY) {
        DrawSuperStoryTime(-y, SuperStoryTimer[0], Game.episode_save[SuperStoryEpisode].superstory_time_limit, 0, 1);
        total = static_cast<i32>(SuperStoryScore);

        if (Game.episode_save[SuperStoryEpisode].superstory_score_target != 0) {
            if (hide_super_story_target == 0) {
                char target[64];
                char text[64];
                Text_MakeScore(static_cast<u32>(Game.episode_save[SuperStoryEpisode].superstory_score_target), target);
                NuStrCpy(text, const_cast<char *>("("));
                NuStrCat(text, target);
                NuStrCat(text, const_cast<char *>(")"));
                Text3DEx(text, 0.0f, y - 0.1f, 1.0f, 0.35f, 0.35f, 0.35f, 0, 255, 255, 255, 48);
            }
            if (SuperStoryScore > static_cast<u32>(Game.episode_save[SuperStoryEpisode].superstory_score_target)) {
                red = 63;
                green = 255;
                blue = 31;
            }
        }
    } else if (source == COIN_TOTAL_BONUS) {
        total = BonusCoinTotal;
    } else {
        total = static_cast<i32>(Game.coins);
    }

    CoinTotal_Draw(total, y, CoinTotalScale, 1, 1.0f, red, green, blue);
}

i32 Tag_UpdateHint(HINT_s *hint) {
    if (WORLD->area != NULL && WORLD->area == HUB_ADATA)
        return 0;
    u8 conditions = static_cast<u8>(LSW_HintConditions);
    i32 tc14 = (conditions >> 1) & 1;
    auto check_tc14 = [&](GameObject_s *object) {
        if (object != NULL && (object->apiobj.field_0x1f8 & 0x1080) == 0x1080 && object->id == id_TC14)
            tc14 = 1;
    };
    check_tc14(Player[0]);
    check_tc14(Player[1]);
    check_tc14(Player[2]);
    check_tc14(Player[3]);
    check_tc14(Player[4]);
    check_tc14(Player[5]);
    check_tc14(Player[6]);
    check_tc14(Player[7]);
    reinterpret_cast<u8 *>(&LSW_HintConditions)[0] = (conditions & ~2) | (tc14 << 1);
    switch (hint->control_mode_ids[0]) {
        case 600:
            if (FreePlay == 0)
                return 0;
            if (player != NULL && static_cast<i8>(player->apiobj.flags_low) < 0 && player->field_0xcc0 != NULL)
                return 0;
            return player2 == NULL || static_cast<i8>(player2->apiobj.flags_low) >= 0 || player2->field_0xcc0 == NULL;
        case 602: {
            if (Tag_DoneFirst != 0) {
                Hint_SetComplete(hint);
                return 0;
            }
            if (VehicleArea != 0 || WORLD->current_level == HUB_LDATA)
                return 0;
            GameObject_s *first = player;
            GameObject_s *second = player2;
            if (first != NULL && static_cast<i8>(first->apiobj.flags_low) < 0 && first->field_0xcc0 != NULL)
                return 0;
            if (second != NULL && static_cast<i8>(second->apiobj.flags_low) < 0 && second->field_0xcc0 != NULL)
                return 0;
            i32 count = 0;
            i32 active = 0;
            auto count_player = [&](GameObject_s *object) {
                if (object != NULL) {
                    ++count;
                    active += (object->apiobj.field_0x1f8 & 0x1080) == 0x1080;
                }
            };
            count_player(Player[0]);
            count_player(Player[1]);
            count_player(Player[2]);
            count_player(Player[3]);
            count_player(Player[4]);
            count_player(Player[5]);
            count_player(Player[6]);
            count_player(Player[7]);
            if (active == 2 && count == 2)
                return 0;
            return (first != NULL && first->field_0xcc0 == NULL) || (second != NULL && second->field_0xcc0 == NULL);
        }
        case 603: {
            if ((LSW_HintConditions & 4) == 0) {
                if (Tag_DoneFirst > 1)
                    Tag_DoneFirst = 1;
                return 0;
            }
            HINT_s *previous = Hint_FindHint(602);
            if (Tag_DoneFirst == 2) {
                Hint_SetComplete(hint);
                return 0;
            }
            if (Tag_DoneFirst != 1 || (previous != NULL && Hint_isComplete(previous) == 0))
                return 0;
            if (VehicleArea != 0 || WORLD->current_level == HUB_LDATA)
                return 0;
            return (player != NULL && player->field_0xcc0 == NULL) || (player2 != NULL && player2->field_0xcc0 == NULL);
        }
        case 605:
        case 611:
        case 650:
            for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
                GameObject_s *object = &Obj[i];
                if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
                    static_cast<i8>(object->field_0xe23) >= 0)
                    continue;
                const bool grab = object->id == id_GRABCONTROL || object->id == id_GRABR2CONTROL;
                if (hint->control_mode_ids[0] == 650) {
                    if (grab)
                        return 1;
                } else if (!grab) {
                    const bool beast = object->apiobj.character_data->move_fn == Move_BEAST;
                    if (beast == (hint->control_mode_ids[0] == 611))
                        return 1;
                }
            }
            return 0;
        case 606:
            for (i32 i = 0; i < 2; ++i) {
                GameObject_s *object = Player[i];
                if (object != NULL && static_cast<i8>(object->apiobj.flags_low) < 0 && object->field_0xcc0 != NULL &&
                    object->id != id_LUKESKYWALKERDAGOBAH)
                    return WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks != 0;
            }
            return 0;
        case 647:
            return WORLD->current_level == SPEEDERCHASEA_LDATA &&
                   ((Player[0] != NULL && Player[0]->id == id_SPEEDERBIKE) ||
                    (Player[1] != NULL && Player[1]->id == id_SPEEDERBIKE));
        case 649:
            if (VehicleArea == 0)
                return 0;
            for (i32 i = 0; i < 2; ++i) {
                if (Player[i] != NULL && Player[i]->torpedo != NULL && Player[i]->torpedo->count != 0 &&
                    Player[i]->torpedo->target != 0)
                    return 1;
            }
            return 0;
        case 651:
            for (i32 i = 0; i < 2; ++i) {
                if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 &&
                    (Player[i]->id == id_GRABCONTROL || Player[i]->id == id_GRABR2CONTROL))
                    return 1;
            }
            return 0;
        case 654:
            if (VehicleArea == 0)
                return 0;
            for (i32 i = 0; i < 2; ++i) {
                if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 &&
                    Player[i]->apiobj.field_0x287 == 0 && (Player[i]->field_0xe24 & 0x10) != 0)
                    return 1;
            }
            return 0;
        default:
            return 0;
    }
}

char *GameObj_GetName(i32 model, GameObject_s *object, char *buffer) {
    if (object != NULL) {
        if (object->field_0xcc0 != NULL && object->field_0xcc0->apiobj.character_data->name_id != -1)
            model = object->field_0xcc0->id;
        else
            model = object->id;
    } else if (model == -1) {
        return TTab[tUNKNOWN];
    }
    if (buffer != NULL) {
        i32 index = -1;
        if (model == id_WEIRDO1) {
            if (Game.customizer.primary_use_saved_name)
                index = 0;
        } else if (model == id_WEIRDO2) {
            if (Game.customizer.secondary_use_saved_name)
                index = 1;
        }
        if (index == -1)
            return TTab[CDataList[model].name_id];
        NuStrCpy(buffer,
                 reinterpret_cast<char *>(&Game.customizer) + offsetof(CUSTOMISESAVE_s, primary_name) + index * 0x38);
        for (i32 i = 14; i >= 0; --i) {
            if (buffer[i] != ' ')
                return buffer;
            buffer[i] = '\0';
        }
        NuStrCpy(buffer, "?");
        return buffer;
    }
    return TTab[CDataList[model].name_id];
}

static void DrawHitPoints(GameObject_s *object, float x, float y, float scale, float alpha, i32 alignment, float, i32) {
    if (object == NULL || WORLD == NULL) {
        return;
    }

    i32 two_rows = 0;
    if (SuperStory != 0 && WORLD->current_level == VADERC_LDATA && object->hitpoints == 10) {
        two_rows = 1;
    }
    if ((object->field_0xefb & 8) != 0) {
        two_rows = drawbosshitpoints_2rows != 0;
        drawbosshitpoints_2rows = 0;
    }

    const i32 heart_object = object->field_0xcc0 == NULL ? 0xcc : 0xcd;
    LEVEL_OBJECT_RUNTIME_s &heart = WORLD->lev_objs[heart_object];
    if (heart.active == 0) {
        return;
    }

    i32 hitpoints;
    i32 current_hp;
    if (object->hitpoints == 0) {
        hitpoints = 1;
        current_hp = 1;
    } else {
        hitpoints = object->hitpoints;
        current_hp = static_cast<i8>(object->current_hp);
    }

    if (PLAYERHITPOINTS_2HEARTSIN1 != 0 && static_cast<i8>(object->apiobj.flags_low) < 0) {
        hitpoints = (hitpoints + 1) / 2;
        current_hp = (current_hp + 1) / 2;
    }

    i32 transitioning = 0;
    if (static_cast<u8>(object->apiobj.field_0x27c) <= 1 && hitpoints > 1 && object->apiobj.field_0x287 != 0 &&
        object->field_0x101c > 0.0f && object->field_0x101c < 1.0f) {
        current_hp = static_cast<i32>(static_cast<float>(hitpoints) * (1.0f - object->field_0x101c));
        transitioning = 1;
    }

    float spacing = scale * 0.35f;
    const bool widescreen = GetMenuID() == 4 ? TempOptions.field11_0xb != 0
                                             : Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0;
    if (widescreen) {
        spacing *= 0.85f;
    }
    if (alignment == 8) {
        spacing = -spacing;
    } else if (alignment == 0) {
        const i32 row_width = two_rows != 0 ? hitpoints / 2 : hitpoints;
        x -= static_cast<float>(row_width - 1) * spacing * 0.5f;
    }

    float draw_x = x;
    float draw_y = y * PANEL3DMULY;
    const i32 split = hitpoints / 2;
    for (i32 i = 0; i < hitpoints; ++i) {
        if (two_rows == 1 && i >= split) {
            two_rows = 2;
            draw_y -= scale * 0.14f;
            draw_x = x;
        }

        float draw_alpha = 0.5f;
        float scale_xy = scale;
        float z = 1.001f;
        if (i < current_hp) {
            draw_alpha = 1.0f;
            if (PLAYERHITPOINTS_2HEARTSIN1 != 0 && static_cast<i8>(object->apiobj.flags_low) < 0 &&
                i == current_hp - 1 && static_cast<i8>(object->current_hp) < (i + 1) * 2) {
                draw_alpha = 0.75f;
            }

            if (transitioning == 0 && current_hp > 0 && i == current_hp - 1) {
                const float pulse = i == 0 && current_hp == 1 ? 0.5f : 0.2f;
                const float pulse_scale = 1.0f + pulse - NuFmod(GlobalTimer.time_elapsed, 0.5f) * (pulse * 2.0f);
                scale_xy *= pulse_scale;
                z = 0.999f;
            }
        }

        NUVEC object_scale = {scale_xy, scale_xy, scale};
        NUMTX matrix;
        NuMtxSetScale(&matrix, &object_scale);
        NUVEC translation = {draw_x * PANEL3DMULX, draw_y, z};
        NuMtxTranslate(&matrix, &translation);
        DrawPanel3DObjectMtx(&heart.special, &matrix, draw_alpha * alpha);
        draw_x += spacing;
    }
}

f32 PowerUp_GetPanelY(i32) {
    STUBBED();
    return 0.0f;
}

void DrawAutoSaveIcon(void) {
    drawautosaveicon = 1;
    return;
}

void UpdateStats() {
    LEVELDATA *level = WORLD->current_level;
    if ((level->flags & LEVEL_GAMEPLAY) == 0) {
        return;
    }

    f32 stats_target = 0.0f;
    if (FadeSys.fade == 0.0f && CUTSTOPGAME == 0 && newgamecam == 0) {
        const bool hub_camera_hidden = HUB_ADATA != NULL && WORLD->area == HUB_ADATA && GameCam->mode == 4;
        const i32 menu = GetMenuID();
        if (!hub_camera_hidden && (menu < PANEL_MENU_EPISODE_I || menu > PANEL_MENU_EPISODE_IV)) {
            stats_target = 1.0f;
        }
    }
    statstime = SeekLinearF(statstime, stats_target, FRAMETIME);

    if ((level->flags & LEVEL_SHOW_COIN_TOTAL) != 0) {
        DrawCoinTotalTime = 1.0f;
    }
    if (DrawCoinTotalTime > 0.0f) {
        DrawCoinTotalTime -= FRAMETIME;
    }

    const f32 coin_total_target = CoinTotalCanOpen() ? 1.0f : 0.0f;
    cointotaltime = SeekLinearF(cointotaltime, coin_total_target, FRAMETIME);
    CoinTotalScale = SeekLinearF(CoinTotalScale, 1.0f, 3.0f * FRAMETIME);
}

void DrawPlayerIconPrompts(i32, i32, float, i32, i32, i32, i32, i32, i32, float, i32, i32, i32, i32) {
    STUBBED();
}

void CoinTotal_Draw(i32 total, f32 y, f32 scale, i32 remember_positions, f32 icon_phase, i32 red, i32 green, i32 blue) {
    char text[32];
    Text_MakeScore(static_cast<u32>(total), text);

    const f32 score_scale = scale * COINTOTAL_SCORESIZE;
    const i32 alpha = static_cast<u8>(icon_phase * 128.0f);
    Text3DEx(text, 0.0f, y + PANEL_COINADJUSTDY, 1.0f, score_scale, score_scale, score_scale, 0, static_cast<u8>(red),
             static_cast<u8>(green), static_cast<u8>(blue), alpha);

    const i32 phase = static_cast<i32>(icon_phase * 16384.0f);
    const f32 icon_scale = scale * COINTOTAL_COINSIZE * NuTrigTable[(phase >> 1) & 0x7fff];

    LEVEL_OBJECT_RUNTIME *left = &WORLD->lev_objs[cointotal_i_obj[0]];
    if (left->active != 0) {
        const f32 x = text3d_width * -0.5f - scale * COINTOTAL_COINDX;
        if (remember_positions != 0) {
            cointotal_x[0] = x;
        }
        DrawPanel3DObject(x, y, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0, &left->special, 0, 1.0f);
    }

    LEVEL_OBJECT_RUNTIME *right = &WORLD->lev_objs[cointotal_i_obj[1]];
    if (right->active != 0) {
        const f32 x = text3d_width * 0.5f + scale * COINTOTAL_COINDX;
        if (remember_positions != 0) {
            cointotal_x[1] = x;
        }
        DrawPanel3DObject(x, y, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0, &right->special, 0, 1.0f);
    }
}

void DrawSuperStoryTime(float, float, float, i32, i32) {
    STUBBED();
}

void DrawBuildUpBar(float x, float y, i32 amount, i32 maximum, float scale, float width, float alpha, u16 angle) {
    const f32 progress = static_cast<f32>(amount * 10) / maximum;
    const i32 full = progress;
    const f32 fraction = NuFmod(progress, 1.0f);
    const f32 phase = GlobalTimer.time_elapsed_mod_seconds * 10.0f;
    const f32 size = scale * 0.085f * width;
    const f32 step = width * 0.02975f * NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff];
    f32 px = x - step * 9.0f * 0.5f;
    i32 shimmer = 0xb3 - static_cast<i32>(phase);
    for (i32 i = 0; i < 10; ++i) {
        i32 object;
        if (amount == maximum) {
            if (shimmer >= 0xb3)
                shimmer = 0xa9;
            object = shimmer++;
        } else if (i < full)
            object = 0xb2;
        else if (i == full)
            object = fraction * 9.0f + 169.0f;
        else
            object = 0xa9;
        const f32 depth[10] = {1.009f, 1.008f, 1.007f, 1.006f, 1.005f, 1.004f, 1.003f, 1.002f, 1.001f, 1.0f};
        DrawPanel3DObject(px, y, depth[i], size, size, size, 0, 0, 0,
                          reinterpret_cast<nuhspecial_s *>(&WORLD->lev_objs[object]), 0, alpha);
        px += step;
    }
}

void DrawBonusScore(float, i32, i32, float, i32 *) {
    STUBBED();
}

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

void DoubleScoreAlpha() {
    STUBBED();
}

void DrawInDoubleScoreZone(float) {
    STUBBED();
}

void Panel_Clear() {
    statstime = 0.0f;
    DrawMiniKitTime = 0.0f;
    MiniKitScale = 1.0f;
    DrawBuildUpTime = 0.0f;
    builduptime = 0.0f;
    BuildUpScale = 1.0f;
    DrawRedBrickTime = 0.0f;
    RedBrickScale = 1.0f;
    DrawCoinTotalTime = 0.0f;
    cointotaltime = 0.0f;
    CoinTotalScale = 1.0f;
    redbrickslidetime = 0.0f;
    goldbricktime = 0.0f;
    Arcade_ResetPanel();
}

extern "C" {
    GameObject_s *drawbosshitpoints = NULL;
}

void DrawBossHitPoints(GameObject_s *object) {
    drawbosshitpoints = object;
}

void SpeederChase_DrawMeleeTargets(i16 *character_ids, char *dimmed, i32 count) {
    if (FadeSys.fade != 0.0f || count <= 0)
        return;

    f32 base_alpha = statstime;
    i32 angle = 0x6000;
    if (SuperStory == 0)
        angle = (static_cast<i32>(minikittime * 32768.0f + 16384.0f) >> 1) & 0x7fff;
    i32 left_count = (count + 1) / 2;
    f32 left_x;
    f32 right_x;
    if ((count & 1) != 0) {
        f32 spread = 1.0f - (1.0f + NuTrigTable[angle]) * 0.5f;
        left_x = -((0.475f - left_count * 0.05f) * spread);
        right_x = -0.075f + (0.325f - (left_count - 1.0f) * 0.05f) * spread;
    } else {
        f32 spread = 1.0f - (1.0f + NuTrigTable[angle]) * 0.5f;
        left_x = -0.075f - (0.4f - left_count * 0.05f) * spread;
        right_x = 0.075f + (0.4f - left_count * 0.05f) * spread;
    }

    f32 alpha = (1.0f - CurrentHintAlpha()) * base_alpha;
    for (i32 i = 0; i < count; ++i) {
        f32 icon_alpha = (dimmed[i] != 0 ? 0.25f : 1.0f) * alpha;
        if (i < left_count) {
            DrawCharIcon(character_ids[i], left_x, KITPOSY, 0.0f, 0.16f, 0xa7, icon_alpha, icon_alpha, 1, NULL);
            left_x -= 0.15f;
        } else {
            DrawCharIcon(character_ids[i], right_x, KITPOSY, 0.0f, 0.16f, 0xa7, icon_alpha, icon_alpha, 1, NULL);
            right_x += 0.15f;
        }
    }
}

void DrawMeleeTargetsRows(i16 *, char *, float *, i32) {
    STUBBED();
}

void DrawMeleeTargetsNumber(i16 *, unsigned char *, i32, unsigned char, nuhspecial_s *) {
    STUBBED();
}

void DrawMeleeTargets(i16 *, char *, float *, i32) {
    STUBBED();
}

void DrawTimer(i32 time, i32 expanded, i32 reset) {
    if (reset != 0) {
        TimerScale = 1.0f;
        TimerAlpha = 0.0f;
        return;
    }
    if (expanded != 0)
        TimerScale = 2.0f;
    TimerScale = SeekLinearF(TimerScale, 1.0f, FRAMETIME * 2.0f);
    if (FadeSys.fade != 0.0f)
        return;
    if (TimerAlpha < 1.0f)
        TimerAlpha = TimerAlpha + FRAMETIME * 2.0f < 1.0f ? TimerAlpha + FRAMETIME * 2.0f : 1.0f;
    char text[16];
    sprintf(text, "%d", time);
    f32 scale = TimerScale * 0.75f;
    Text3DEx(text, 0.0f, BOSSICONY, 1.0f, scale, scale, scale, 0, 255, 0, 255,
             static_cast<u8>(static_cast<i32>(TimerAlpha * 128.0f)));
}

void InitPanel(i32) {
    const f32 panel_fov = pNuCam->fov / 0.75f;
    const f32 aspect_ratio = NuIOS_GetAspectRatio();
    const f32 divisor = (1.0f - panel_fov) * 0.22f + 2.545f;
    PANEL3DMULX = aspect_ratio * panel_fov / divisor;
    PANEL3DMULY = panel_fov / divisor;
}

void DrawPanel() {
    const i32 menu = GetMenuID();
    SetQFont2D();
    if (CUTSTOPGAME == 0)
        TransformGameMessages(&GameCam->pos, &GameCam->shaken_right, &GameCam->dir);
    if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA)
        Customiser_TransformToPanel(CharacterCustomiser);
    const i32 paused = screendump ? save_paused : Paused;
    // The original loading shortcut reads this before initialization. Give that path a stable result.
    i32 removed_controller = -1;
    char text[128], auxiliary[128], loading_text[128];
    // Original debug coordinates were never initialized by this port.
    NUVEC coordinate_positions[8] = {};
    f32 status_y = 0.0f;
    if (PANELOFF && !paused && (WORLD->current_level->flags & LEVEL_GAMEPLAY))
        return;
    if (waiting_for_level != -1) {
        if (DRAWBGLOAD && bgGetProcActive()) {
            i32 red, green;
            if (abort_load) {
                sprintf(loading_text, "Aborting ''%s''", LDataList[waiting_for_level].name);
                red = 255;
                green = 0;
            } else {
                sprintf(loading_text, "Loading ''%s''", LDataList[waiting_for_level].name);
                red = 0;
                green = 255;
            }
            f32 y = 0.035f * NU_SIN_LUT(static_cast<u16>(NuFmod(WaitingForLevelTime, 0.430f) / 0.430f * 65536.0f)) -
                    STATSPOSY;
            f32 x = 0.035f * NU_SIN_LUT(static_cast<u16>(NuFmod(WaitingForLevelTime, 0.479f) / 0.479f * 65536.0f));
            Text3D(loading_text, x, y, 1.0f, 0.3f, 0.3f, 0.3f, 0, red, green, 0);
        }
        if (gone_through_door_to_new_level)
            goto draw_panel_menu;
    }
    {
        f32 pulse = 0.25f * NU_SIN_LUT(static_cast<i32>(GlobalTimer.time_elapsed_mod_seconds * 65536.0f));
        {
            const i32 i = 0;
            if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 && NoPad(i, 1) &&
                (WORLD->current_level == NULL || !(WORLD->current_level->flags & 0xe0)) &&
                !MenuInCriticalMemoryCard()) {
                removed_controller = GamePad[i].pad->port;
                sprintf(text, apitxt_CONTROLLERREMOVED, removed_controller + 1, removed_controller + 1);
                i32 alpha = static_cast<u8>(static_cast<i32>((i == 0 ? 0.75f + pulse : 0.75f - pulse) * 128.0f));
                SmartTextEx(text, 0.0f, i == 0 ? 0.5f : -0.5f, 1.0f, 0.4f, 0.4f, 0.4f, 0, 63, 127, 255, 1.5f, 4, 0, 0,
                            alpha);
            }
        }
        {
            const i32 i = 1;
            if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 && NoPad(i, 1) &&
                (WORLD->current_level == NULL || !(WORLD->current_level->flags & 0xe0)) &&
                !MenuInCriticalMemoryCard()) {
                removed_controller = GamePad[i].pad->port;
                sprintf(text, apitxt_CONTROLLERREMOVED, removed_controller + 1, removed_controller + 1);
                i32 alpha = static_cast<u8>(static_cast<i32>((i == 0 ? 0.75f + pulse : 0.75f - pulse) * 128.0f));
                SmartTextEx(text, 0.0f, i == 0 ? 0.5f : -0.5f, 1.0f, 0.4f, 0.4f, 0.4f, 0, 63, 127, 255, 1.5f, 4, 0, 0,
                            alpha);
            }
        }
    }
    if (removed_controller == -1) {
        LEVELDATA *level = WORLD->current_level;
        if (level == STATUS_LDATA || (level->flags & LEVEL_STATUS)) {
            if (level->draw_status_fn != NULL)
                level->draw_status_fn(WORLD);
            DrawGameMessages();
            goto draw_panel_menu;
        }
        if (BonusWinner != -1)
            goto draw_panel_menu;
        if (!(menu >= 15 && menu <= 19) && !CUTSTOPGAME) {
            bool player_hud =
                menu != 8 && menu != 14 && menu != 24 && (menu != 12 || customiser_quit) && (menu != 13 || shop_quit);
            if (player_hud && FadeSys.fade == 0.0f && (WORLD->current_level->flags & LEVEL_GAMEPLAY)) {
                u32 arcade_flags;
                i32 arcade_mode = Arcade_GetMode(&arcade_flags);
                status_y = NU_SIN_LUT(static_cast<i32>(statstime * 16384.0f)) * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y;
                bool raised_hearts =
                    (WORLD->area != NULL && (WORLD->area == HUB_ADATA || (WORLD->area->flags & 0x100))) || SuperStory ||
                    ChallengeMode || Mission_Active(NULL) != NULL || arcade_mode == 99;
                GetMenuID();
                f32 pulse =
                    NU_SIN_LUT(static_cast<u16>(NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f));
                GameObject_s *object = Player[0];
                if (object != NULL) {
                    f32 base_alpha = 1.0f;
                    if (paused && pause_i_pad != 0 && static_cast<i8>(object->apiobj.flags_low) < 0)
                        base_alpha = 0.5f;
                    f32 alpha = base_alpha * (static_cast<i8>(object->apiobj.flags_low) < 0 ? 1.0f : DROPINALPHA);
                    f32 icon_x = -ICONX;
                    drawcharicon_i_panel = 0;
                    i32 alpha_byte = static_cast<i32>(alpha * 128.0f);
                    f32 icon_size = ICONSIZE;
                    if (MechSystems::Get()->PlayerButton().hovered)
                        icon_size *= 1.2f;
                    bool own_icon = WORLD->current_level == DAGOBAHE_LDATA && object->field_0xcc0 != NULL &&
                                    object->field_0xcc0->id == id_YODA;
                    f32 icon_time = object->hud_icon_timer;
                    i32 visible = icon_time <= 0.0f || (icon_time < 2.0f && NuFmod(icon_time, 0.4f) < 0.2f);
                    i32 id = own_icon || object->field_0xcc0 == NULL ? object->id : object->field_0xcc0->id;
                    DrawCharIcon(id, icon_x, status_y, 0.0f, icon_size, 0xa6, alpha, alpha, visible, NULL);
                    f32 name_x = -(ICONX + 0.075f);
                    if (static_cast<i8>(object->apiobj.flags_low) < 0 && object->apiobj.character_data->name_id != -1) {
                        bool draw_name = paused != 0;
                        if (!draw_name && object->hud_icon_timer > 0.0f && object->hud_icon_timer < 2.0f)
                            draw_name = NuFmod(object->hud_icon_timer, 0.4f) < 0.2f;
                        if (draw_name) {
                            f32 width = Game.options_save.widescreen ? 0.7f : 0.5f;
                            f32 name_y = status_y - 0.125f;
                            char *name = GameObj_GetName(-1, object, auxiliary);
                            SmartTextEx(name, name_x, name_y, 1.0f, 0.35f, 0.35f, 0.35f, 3, 255, 255, 255, width, 2, 0,
                                        0, static_cast<i32>(base_alpha * 128.0f));
                        }
                    }
                    if (!paused && FadeSys.fade == 0.0f && static_cast<i8>(object->apiobj.flags_low) < 0 &&
                        MechSystems::Get()->PlayerButton().panel_state == NULL) {
                        if (ONEPLAYERPOWERUPS && object->field_0xdec > 0.0f) {
                            if (!FindGameMsgsWithID(7, 0, object->apiobj.field_0x27c, NULL) &&
                                (object->field_0xdec >= 3.0f ||
                                 PickupFlickerFrame % PickUpFlickerFrames < PickUpFlickerTest)) {
                                nuhspecial_s *special = &WORLD->lev_objs[0xd0].special;
                                u16 angle = PowerUp_PanelYRot;
                                f32 scale = POWERUPOBJSIZE;
                                f32 y = PowerUp_GetPanelY(0);
                                DrawPanel3DObject(-ICONX, y + status_y, 1.0f, scale, scale, scale, 0, angle, 0, special,
                                                  0, 1.0f);
                                special = &WORLD->lev_objs[0xd1].special;
                                angle = PowerUp_PanelYRot;
                                scale = POWERUPOBJSIZE;
                                y = PowerUp_GetPanelY(0);
                                DrawPanel3DObject(-ICONX, y + status_y, 1.0f, scale, scale, scale, 0, angle, 0, special,
                                                  0, 1.0f);
                            }
                        } else {
                            u32 multiplier = Cheat_MultiplyScore(1);
                            if (DoubleScore & 1)
                                multiplier *= 2;
                            if (multiplier > 1) {
                                sprintf(text, "x%i", multiplier);
                                i32 flash_alpha = static_cast<u8>(static_cast<i32>(pulse * 16.0f + 96.0f));
                                Text3DEx(text, -ICONX, status_y - 0.285f, 1.0f, 0.3f, 0.35f, 0.35f, 1, 255, 0, 255,
                                         flash_alpha);
                            }
                        }
                    }
                    if (static_cast<i8>(object->apiobj.flags_low) < 0) {
                        DrawHitPoints(object, -PANEL_HITPOINTSX, status_y + (raised_hearts ? 0.0f : PANEL_HEARTY),
                                      0.195f, alpha, 2, 0.0f, 0);
                    } else if (!paused && !CUTSTOPGAME) {
                        i32 dropin_alpha = static_cast<i32>(DROPINALPHA * 128.0f);
                        if (dropin_alpha > 0) {
                            f32 y = status_y + (raised_hearts ? 0.0f : PANEL_HEARTY);
                            char *prompt = NoPad(0, 0) ? TTab[tDROPIN_INSERTCONTROLLER] : apitxt_PRESSSTART;
                            SmartTextEx(prompt, -0.685f, y, 1.0f, 0.35f, 0.35f, 0.35f, 2, 255, 255, 255, 0.3f, 2, 0, 0,
                                        dropin_alpha);
                        }
                    }
                    if (!raised_hearts) {
                        if (arcade_flags & 2) {
                            sprintf(text, "%i/%i", AreaGlobals.values.field_0x2c,
                                    Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].target);
                            f32 coin_x = -PANEL_COINX;
                            f32 scale = object->coinpacket->scale * PANEL_SCORESCALE;
                            f32 y = status_y + PANEL_COINY;
                            Text3DEx(text, -PANEL_SCOREX, PANEL_COINADJUSTDY + y, 1.0f, scale, scale, scale, 2, 255,
                                     191, 0, static_cast<u8>(alpha_byte));
                            if (WORLD->lev_objs[0x35].active) {
                                scale = object->coinpacket->scale * PANEL_COINSCALE_END;
                                DrawPanel3DObject(coin_x, y, 1.0f, scale, scale, scale, 0, 0, 0,
                                                  &WORLD->lev_objs[0x35].special, 0, alpha);
                            }
                        } else if (object->coinpacket != NULL) {
                            Text_MakeScore(object->coinpacket->coins, text);
                            f32 coin_x = -PANEL_COINX;
                            f32 scale = object->coinpacket->scale * PANEL_SCORESCALE;
                            f32 y = status_y + PANEL_COINY;
                            Text3DEx(text, -PANEL_SCOREX, y + PANEL_COINADJUSTDY, 1.0f, scale, scale, scale, 2, 255,
                                     191, 0, static_cast<u8>(alpha_byte));
                            COINPACKET_s *packet = object->coinpacket;
                            i32 model = static_cast<i16>(packet->lastcoin);
                            if ((model >= 0xb7 && model <= 0xba) || (model >= 0xbf && model <= 0xc2) ||
                                (model >= 0xc7 && model <= 0xca))
                                model -= 4;
                            else if (model >= 0xd5 && model <= 0xd8)
                                model += 4;
                            if (WORLD->lev_objs[model].active) {
                                scale = packet->scale * PANEL_COINSCALE_END;
                                DrawPanel3DObject(coin_x, y, 1.0f, scale, scale, scale, 0, 0, 0,
                                                  &WORLD->lev_objs[model].special, 0, alpha);
                            }
                            i32 target = 0;
                            bool draw_target = false;
                            if (Arcade) {
                                if (arcade_flags & 8) {
                                    target = arcade_placed_stud_total;
                                    draw_target = target != 0;
                                } else if (arcade_flags & 4) {
                                    target = Arcade_Mode[static_cast<i8>(ArcadeItem.field_c_0xc)].target;
                                    draw_target = target != 0;
                                }
                            } else if (BonusArea && VehicleArea) {
                                target = BonusCoinTarget;
                                draw_target = true;
                            }
                            if (draw_target) {
                                Text_MakeScore(target, auxiliary);
                                sprintf(text, "(%s)", auxiliary);
                                Text3DEx(text, 0.0f, y + PANEL_COINADJUSTDY, 1.0f, 0.35f, 0.35f, 0.35f, 0, 255, 255,
                                         255, 48);
                            }
                        }
                    }
                }
                if (!MenuInMemoryCard()) {
                    if (WORLD->area != NULL) {
                        i32 freeplay = GAMEDEMO ? 0 : FreePlay;
                        if (!SuperStory && !ChallengeMode && Mission_Active(NULL) == NULL && !Arcade &&
                            (WORLD->area->flags & 0x4010)) {
                            i32 maximum = freeplay ? WORLD->area->field38_0x90 : WORLD->area->field37_0x8c;
                            if (maximum != 0) {
                                status_y =
                                    NU_SIN_LUT(static_cast<i32>(builduptime * 16384.0f)) * (STATSPOSY - STATSPOS2Y) +
                                    STATSPOS2Y;
                                f32 y = status_y + PANEL_COINY;
                                AREASAVE_s *save = &Game.area_save[WORLD->level_sub_id];
                                i32 amount;
                                if (save->story_buildup_complete || save->freeplay_buildup_complete)
                                    maximum = amount = BuildUpTotal;
                                else
                                    amount = BuildUpDone ? maximum : BuildUpTotal;
                                DrawBuildUpBar(0.0f, y, amount, maximum, 1.0f, BuildUpScale, 1.0f, 0);
                            }
                        }
                        if (!SuperStory && Mission_Active(NULL) == NULL && !Arcade) {
                            bool draw_minikits = (WORLD->area->flags & 0x10) != 0;
                            if (!draw_minikits && (WORLD->current_level->flags & 0x200))
                                draw_minikits = GizmoPickup_NumberOfType(WORLD, 4, 0) > 0;
                            if (draw_minikits)
                                DrawMiniKitCount(
                                    minikittime, MiniKitScale,
                                    ChallengeMode ? AreaGlobals.values.field_0x20 : AreaGlobals.values.field_0x14, 10);
                        }
                        if (!SuperStory && !ChallengeMode && Mission_Active(NULL) == NULL && !Arcade &&
                            (WORLD->area->flags & 0x10) &&
                            (AreaGlobals.values.field_0x08 == 2 ||
                             Game.area_save[WORLD->level_sub_id].red_brick_collected) &&
                            redbrickslidetime > 0.0f) {
                            f32 factor = NU_SIN_LUT(static_cast<i32>(redbrickslidetime * 16384.0f));
                            f32 x = (REDBRICKPOSX - REDBRICKPOS2X) * factor + REDBRICKPOS2X;
                            status_y = (REDBRICKPOSY - REDBRICKPOS2Y) * factor + REDBRICKPOS2Y;
                            u16 angle = static_cast<u16>(
                                static_cast<i32>(NuFmod(GlobalTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f) + 0x1555);
                            f32 scale = PANEL_REDBRICKSCALE * RedBrickScale;
                            u16 pitch = static_cast<u16>(1820.0f * NuTrigTable[angle & 0x7fff]);
                            DrawPanel3DObjectNoAlpha(x, status_y, 1.0f, scale, scale, scale, pitch, angle, 0,
                                                     &WORLD->lev_objs[0xd2].special, 2);
                        }
                    }
                    if (DoubleScoreTime > 0.0f)
                        DrawInDoubleScoreZone(DoubleScoreTime);
                }
                if (BonusArea && WORLD->area != NULL && (WORLD->area->flags & 0x104) == 4) {
                    i32 *scores = Arcade ? Arcade_Points : BonusScore;
                    i32 active2 = Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.flags_low) < 0;
                    i32 active1 = Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.flags_low) < 0;
                    DrawBonusScore(status_y, active1, active2, 1.0f, scores);
                }
                if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA && goldbricktime > 0.0f) {
                    f32 y =
                        (STATSPOS2Y - STATSPOSY) * NU_SIN_LUT(static_cast<i32>(goldbricktime * 16384.0f)) - STATSPOS2Y;
                    Hub_DrawImportantBrick(0xd3, 0.0f, y, 1.0f, Game.gold_bricks, GOLDBRICKPOINTS);
                }
            }
            i32 hide_target = 0;
            GameObject_s *boss = drawbosshitpoints;
            if (boss != NULL && boss->apiobj.field_0x287 == 0 && static_cast<i8>(boss->apiobj.flags_low) >= 0) {
                if (FadeSys.fade == 0.0f) {
                    DrawCharIcon(boss->id, 0.0f, BOSSICONY, 0.0f, 0.16f, 0xa7, statstime, statstime, 1, NULL);
                    DrawHitPoints(boss, 0.0f, 0.47f, 0.2f, statstime, 0, 0.0f, 0);
                    hide_target = 1;
                } else
                    drawbosshitpoints_2rows = 0;
            }
            if (WORLD->area == HUB_ADATA)
                DrawCoinTotal(0, hide_target);
            else if (SuperStory) {
                if (WORLD->current_level->flags & 0x2000)
                    DrawCoinTotal(1, hide_target);
            } else if (BonusArea) {
                if (Arcade)
                    Arcade_DrawPanel(Paused || NetPaused);
                else {
                    if (WORLD->area->flags & 0x100) {
                        DrawCoinTotal(2, hide_target);
                        if (DrawCoinTotalY != 2000000.0f) {
                            Text_MakeScore(BonusCoinTarget, auxiliary);
                            NuStrCpy(text, const_cast<char *>("("));
                            NuStrCat(text, auxiliary);
                            NuStrCat(text, const_cast<char *>(")"));
                            Text3DEx(text, 0.0f, DrawCoinTotalY - 0.1f, 1.0f, 0.35f, 0.35f, 0.35f, 0, 255, 255, 255,
                                     48);
                        }
                    }
                    if (FadeSys.fade == 0.0f && (WORLD->current_level->flags & LEVEL_GAMEPLAY)) {
                        f32 y =
                            NU_SIN_LUT(static_cast<i32>(statstime * 16384.0f)) * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y;
                        DrawSuperStoryTime(-y, BonusTimer.time_elapsed,
                                           Game.area_save[WORLD->level_sub_id].challenge_trial_time, 0, 1);
                    }
                }
            } else if (ChallengeMode) {
                f32 remaining =
                    static_cast<f32>(ADataList[WORLD->level_sub_id].challenge_trial_time) - ChallengeTimer.time_elapsed;
                if (remaining < 0.0f)
                    remaining = 0.0f;
                Text_MakeTime(remaining, 0, 1, 1, text);
                f32 y = NU_SIN_LUT(static_cast<i32>(statstime * 16384.0f)) * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y;
                Text3D(text, 0.0f, y, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
            } else if (Mission_Active(NULL) != NULL) {
                status_y = NU_SIN_LUT(static_cast<i32>(statstime * 16384.0f)) * (STATSPOSY - STATSPOS2Y) + STATSPOS2Y;
                i32 mission_index = static_cast<i8>(MissionSys->mission->count);
                f32 remaining = static_cast<f32>(static_cast<u16>(MissionSys->missions[mission_index].time)) -
                                MissionSys->timer.time_elapsed;
                if (remaining < 0.0f)
                    remaining = 0.0f;
                Text_MakeTime(remaining, 0, 1, 1, text);
                Text3D(text, 0.0f, status_y, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
                if (!paused) {
                    GameObject_s *target = Mission_FindTarget(MissionSys, NULL);
                    if (target != NULL && player != NULL) {
                        f32 alpha =
                            0.8f + 0.2f * NU_SIN_LUT(static_cast<u16>(NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f) *
                                                                      2.0f * 65536.0f));
                        NUVEC target_point = v000;
                        NUVEC *position = &target->apiobj.collision_position;
                        bool hide_target = false;
                        if (target->id == id_QUIGONJINN &&
                            (player->field_0x661 == 10 || player->field_0x661 == 4 || player->field_0x661 == 11))
                            hide_target = true;
                        if (!hide_target) {
                            if (target->id == id_MACEWINDU && player->field_0x661 != 1) {
                                target_point.x = 64.7f;
                                target_point.y = 0.9f;
                                target_point.z = -3.7f;
                                position = &target_point;
                            } else if (target->id == id_C3PO && WORLD->current_level == CLOUDCITYESCAPEA_LDATA &&
                                       player->field_0x661 != 12) {
                                target_point.x = 7.9f;
                                target_point.y = 0.8f;
                                target_point.z = -33.9f;
                                position = &target_point;
                            }
                            f32 distance = NuVecDistSqr(&player->apiobj.collision_position, position, NULL);
                            if (player2 != NULL) {
                                f32 distance2 = NuVecDistSqr(&player2->apiobj.collision_position, position, NULL);
                                if (distance2 < distance)
                                    distance = distance2;
                            }
                            distance = NuFsqrt(distance);
                            if (distance > 10.0f)
                                distance = 10.0f;
                            alpha *= 1.0f - distance / 10.0f;
                        } else
                            alpha = 0.0f;
                        DrawCharIcon(MissionSys->missions[static_cast<i8>(MissionSys->mission->count)].find_char, 0.0f,
                                     0.055f - status_y, 0.0f, 0.25f, 0xa7, alpha, alpha, 1, NULL);
                    }
                }
            }
        }
    }
    if (FPSDISPLAY) {
        sprintf(text, "fps: %d", static_cast<i32>(1.0f / FRAMETIME));
        Text3D(text, 0.85f, -0.85f, 1.0f, 0.4f, 0.4f, 0.4f, 12, 255, 255, 255);
    }
    if (CUTSTOPGAME) {
        CutScene_DrawSubtitles();
        goto draw_panel_menu;
    }
    if (WORLD->current_level->draw_status_fn == NULL && !(WORLD->current_level->flags & LEVEL_GAMEPLAY))
        goto draw_panel_menu;
    for (i32 i = 0; i < 8; ++i) {
        if (Player[i] != NULL && static_cast<i8>(Player[i]->apiobj.flags_low) < 0 && ShowPlayerCoordinate) {
            sprintf(text, "X:%.2f Y:%.2f Z:%.2f", Player[i]->apiobj.position.x, Player[i]->apiobj.position.y,
                    Player[i]->apiobj.position.z);
            Text3DEx(text, coordinate_positions[i].x, coordinate_positions[i].y, 1.0f, 0.4f, 0.5f, 0.5f, 0, 255, 191, 0,
                     48);
        }
    }
    if (TimingBarSet == 2) {
        sprintf(text, "Terrain %i", TERRAINCALLS);
        Text3D(text, 0.9f, 0.075f, 1.0f, 0.3f, 0.3f, 0.3f, 8, 255, 255, 255);
        sprintf(text, "Shadow %i", SHADOWCALLS);
        Text3D(text, 0.9f, 0.0f, 1.0f, 0.3f, 0.3f, 0.3f, 8, 255, 255, 255);
        sprintf(text, "RayCast %i", RAYCASTCALLS);
        Text3D(text, 0.9f, -0.075f, 1.0f, 0.3f, 0.3f, 0.3f, 8, 255, 255, 255);
    }
    DebrisDraw(paused, 4);
    if (removed_controller == -1 && WORLD->current_level->draw_status_fn != NULL)
        WORLD->current_level->draw_status_fn(WORLD);
    GizmoSysPanelDraw(WORLD->gizmo_sys, WORLD, FRAMETIME);
    if (!paused) {
        Hint_Draw(-1);
        DrawGameMessages();
    }
draw_panel_menu:
    if (removed_controller == -1 && !editor_active)
        DrawMenu(paused);
    if (drawautosaveicon && WORLD->lev_objs[0].active) {
        f32 scale = AUTOSAVEICONSIZE *
                    (0.9f + 0.1f * NU_SIN_LUT(static_cast<u16>(NuFmod(GlobalTimer.time_elapsed, 1.0f) * 65536.0f)));
        DrawPanel3DObject(AUTOSAVEICONX, AUTOSAVEICONY, 1.0f, scale, scale, scale, 0, 0, 0, &WORLD->lev_objs[0].special,
                          0, 1.0f);
        if (memcard_autosavepredelay == 1.0f || memcard_saveneeded || memcard_loadneeded) {
            VuVec position(AUTOSAVEICONX, AUTOSAVEICONY, 0.0f, 0.0f);
            MechSystems::Get()->NewRadarPulse(position, true);
        }
    }
    drawautosaveicon = 0;
    if (GameCam != NULL)
        pNuCam->mtx = GameCam->render_mtx;
    NuCameraSet(pNuCam);
}

void PanelRender(WORLDINFO_s *) {
    NuRndrBeginScene(-1);
    SetQFont2D();
    SetPanelLights(1.0f);
    NuRndrClear(0xe00, 0xff000000, 1.0f);
    DrawPauseFade();
    if (TimingBarSet == 5) {
        TBOPENFN("Panel", 5);
    }
    DrawPanel();
    if (TimingBarSet == 5) {
        TBCLOSEFN("Panel", 5);
    }
    TimingBars();
    FadeSys.Update();
    static_cast<ThingManager *>(theGameThings)->DisplayThings(NULL);
    NuRndrEndScene();
    FadeSys.Draw();

    if (WORLD != NULL && WORLD->current_level == TITLES_LDATA) {
        i32 colour = static_cast<i32>((1.0f - newgamealpha) * 255.0f);

        NuRndrBeginScene(-1);
        colour <<= 24;
        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_ABSOLUTE);
        NuPrim2DBegin(4, 5, FadeMtl2);

        struct PanelFadeVertex {
            f32 x;
            f32 y;
            f32 z;
            u32 colour;
        };

        PanelFadeVertex *vertex = reinterpret_cast<PanelFadeVertex *>(g_NuPrim_StreamBufferPtr->void_ptr);
        if (g_NuPrim_NeedsOverbrightening == 0) {
            vertex->colour = colour & 0xff000000u;
        } else {
            vertex->colour = colour;
        }
        NuPrim2DAddXYZ(0.0f, 0.0f, 0.0f);

        vertex = reinterpret_cast<PanelFadeVertex *>(g_NuPrim_StreamBufferPtr->void_ptr);
        if (g_NuPrim_NeedsOverbrightening == 0) {
            vertex->colour = colour & 0xff000000u;
        } else {
            vertex->colour = colour;
        }
        NuPrim2DAddXYZ(1.0f, 1.0f, 0.0f);

        NuPrim2DEnd();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
        NuRndrEndScene();
    }
}

i32 CoinsGoToMainTotal() {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if ((HUB_ADATA != NULL && HUB_ADATA == world->area) || SuperStory != 0 ||
        (world->area != NULL && (world->area->flags & (AREAFLAG_SUPER_BONUS_AREA & ~AREAFLAG_BONUS_AREA)) != 0) ||
        (world->current_level != NULL && (world->current_level->flags & LEVEL_STATUS) != 0)) {
        return 1;
    }
    return 0;
}
