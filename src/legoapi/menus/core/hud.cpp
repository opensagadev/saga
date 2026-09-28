#include "MechInputTouch/MechInputTouch_types.h"
void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"

struct spacelevel_s;

extern FadeSystem FadeSys;
extern f32 statstime;
extern f32 cointotaltime;

void Text_MakeScore(u32 score, char *text);

HudRadarPulse::HudRadarPulse(VuVec const &initial_position) : position(initial_position) {
    pulses[1].delay = 0.2f;
    pulses[2].delay = 0.4f;

    pulses[0].delay_finished = 0;
    pulses[0].finished = 0;
    pulses[0].angle = 0;
    pulses[0].delay = 0.0f;
    pulses[1].delay_finished = 0;
    pulses[0].radius = 0.0f;
    pulses[1].finished = 0;
    pulses[0].speed = 0.75f;
    pulses[1].angle = 0;
    pulses[1].radius = 0.0f;
    pulses[2].delay_finished = 0;
    pulses[1].speed = 0.75f;
    pulses[2].finished = 0;
    pulses[2].radius = 0.0f;
    pulses[2].angle = 0;
    pulses[2].speed = 0.75f;

    active = 1;
    paused = 0;
}

u8 HudRadarPulse::IsFinished() {
    if (pulses[0].delay_finished && pulses[0].finished && pulses[1].delay_finished && pulses[1].finished &&
        pulses[2].delay_finished)
        return pulses[2].finished;
    return 0;
}

extern i32 NewMode;
extern i32 editor_active;
extern i32 CutSceneWaiting;
extern "C" i32 Paused;
void HudRadarPulse::Process(float delta) {
    if (active) {
        active = 0;
        if (NewMode == 0 && NewLData == NULL && editor_active == 0 && GameTimer.time_elapsed > 0.0f &&
            GameTimer.update_count != 0 && WORLD != NULL && CutSceneWaiting == 0 &&
            (Paused == 0 || GetMenuID() == 0x19 || GetMenuID() == 0x15) &&
            (CUTSTOPGAME == 0 || CutScene_IsSkippable(static_cast<CUTINFO *>(CutStopInfo))) && MiniCutCam == 0 &&
            (paused || (WORLD->current_level != TITLES_LDATA && WORLD->current_level != STATUS_LDATA &&
                        !(WORLD->current_level->flags & 0x400) && memcard_autosavestarted == 0 &&
                        !(memcard_autosavepostdelay > 0.0f) && !(memcard_autosavepredelay > 0.0f))))
            active = 1;
    }
    for (i32 i = 0; i < 3; ++i) {
        HudRadarPulseStage &pulse = pulses[i];
        if (!pulse.delay_finished) {
            pulse.delay -= delta;
            if (pulse.delay <= 0.0f)
                pulse.delay_finished = 1;
        }
        if (pulse.delay_finished && !pulse.finished) {
            pulse.radius += pulse.speed * delta;
            float speed = pulse.speed - 0.4f * delta;
            pulse.speed = speed > 0.0f ? speed : 0.0f;
            pulse.angle += static_cast<i32>(32768.0f * delta);
            if (pulse.angle > 0x7fff)
                pulse.finished = 1;
        }
    }
}

void HudRadarPulse::Render() {
    if (active) {
        if (pulses[0].delay_finished && !pulses[0].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[0].angle));
            f32 radius = pulses[0].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
        if (pulses[1].delay_finished && !pulses[1].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[1].angle));
            f32 radius = pulses[1].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
        if (pulses[2].delay_finished && !pulses[2].finished) {
            f32 x = position.x + 1.0f;
            f32 y = 1.0f - position.y;
            numtl_s *material = MechSystems::Get()->radar_pulse_material;
            i32 alpha = static_cast<i32>(90.0f * NU_SIN_LUT(pulses[2].angle));
            f32 radius = pulses[2].radius;
            f32 width = GetAspectRatio() * radius;
            RndrTexQuad(x * 0.5f, y * 0.5f, width, radius, static_cast<i32>((static_cast<u32>(alpha) << 24) | 0x808080),
                        material, 0);
        }
    }
}

extern i8 episodesmode, i_episodes, i_clip[6];
extern i32 hub_new_level;
extern f32 episodestime, episodesduration, HUB_EPISODESUBTITLESIZE, PANEL3DMULX;
extern i16 tPLAY;
extern "C" {
    extern u8 MENUNORMALR, MENUNORMALG, MENUNORMALB, MENUENTRYR, MENUENTRYG, MENUENTRYB;
    extern u8 MENUFLASH0R, MENUFLASH0G, MENUFLASH0B, MENUFLASH1R, MENUFLASH1G, MENUFLASH1B;
    extern f32 menu_pulse, menu_pulsate;
}
void CutScenePlayer_DrawGrid(COLLECTION_s *, i16 *, float, float, i32, float);

static char *EpisodeNumerals[6] = {"I", "II", "III", "IV", "V", "VI"};

static __used__ void DrawEpisodesMenu(int selected, float alpha) {
    MENU *menu = &GameMenu[GameMenuLevel];
    if (episodesmode == 1 || episodesmode == 2) {
        if (episodesmode == 2 || (hub_new_level != -1 && LDataList[hub_new_level].episode_index != -1)) {
            COLLECTION_s collection = CharacterCollection;
            i16 clips[126];
            i32 include_guests = hub_new_level == -1 || LDataList[hub_new_level].episode_index == -1;
            collection.count_y = CutScenePlayer_CountEpisodeClips(i_episodes, include_guests, clips);
            collection.count_x = 7;
            collection.field_10 = 1.0f;
            if (episodesmode != 1 && episodestime < episodesduration)
                alpha *= episodestime / episodesduration;
            CutScenePlayer_DrawGrid(&collection, clips, 0.0f, 0.05f, i_clip[i_episodes], alpha);
            return;
        }
    } else if (episodesmode == 3) {
        if (episodestime < episodesduration)
            alpha *= episodestime / episodesduration;
        float pulse =
            (NU_SIN_LUT(static_cast<i32>(NuFmod(GameTimer.time_elapsed, 0.5f) * 65536.0f)) * 0.25f + 0.75f) * alpha;
        Text3DEx(TTab[tPLAY], 0.0f, -0.15f, 1.0f, HUB_EPISODESUBTITLESIZE, HUB_EPISODESUBTITLESIZE,
                 HUB_EPISODESUBTITLESIZE, 0, 255, 255, 255, static_cast<u8>(pulse * 255.0f));
        DrawPanel3DObject(0.0f, 0.15f, 1.0f, 0.35f, 0.35f, 0.35f, 0, 0, 0, &WORLD->lev_objs[167].special, 0, pulse);
        menu->item_x[0] = 0.0f;
        menu->item_y[0] = 0.15f;
        menu->item_width[0] = 0.175f;
        menu->item_height[0] = 0.0f;
        Text3DEx(" ", 0.0105f, 0.15f, 1.0f, 1.575f, 1.05f, 1.05f, 0, 255, 255, 255, static_cast<u8>(pulse * 255.0f));
        for (i32 i = 1; i < 400; ++i) {
            menu->item_width[i] = 0.0f;
            menu->item_height[i] = 0.0f;
        }
        return;
    } else if (episodesmode != 0) {
        return;
    }

    NUVEC minimum, maximum;
    NuSpecialGetBounds(&WORLD->lev_objs[167].special, &minimum, &maximum);
    const float dx = (maximum.x - minimum.x) * 1.2f / PANEL3DMULX;
    const float dy = -(maximum.y - minimum.y) * 1.2f / PANEL3DMULY;
    float y = -dy * 2.0f;
    for (i32 row = 0; row < 2; ++row, y += dy) {
        for (i32 column = 0; column < 3; ++column) {
            i32 episode = row * 3 + column;
            float x = (column - 1) * dx;
            i32 red, green, blue;
            if (selected && episode == i_episodes && TestForController()) {
                if (menu_pulsate > 0.0f) {
                    red = MENUFLASH0R * menu_pulsate + MENUFLASH1R * (1.0f - menu_pulsate);
                    green = MENUFLASH0G * menu_pulsate + MENUFLASH1G * (1.0f - menu_pulsate);
                    blue = MENUFLASH0B * menu_pulsate + MENUFLASH1B * (1.0f - menu_pulsate);
                } else {
                    red = menu_flash ? MENUFLASH0R : MENUFLASH1R;
                    green = menu_flash ? MENUFLASH0G : MENUFLASH1G;
                    blue = menu_flash ? MENUFLASH0B : MENUFLASH1B;
                }
            } else if (menu_pulse > 0.0f) {
                red = MENUFLASH0R * menu_pulse + MENUNORMALR * (1.0f - menu_pulse);
                green = MENUFLASH0G * menu_pulse + MENUNORMALG * (1.0f - menu_pulse);
                blue = MENUFLASH0B * menu_pulse + MENUNORMALB * (1.0f - menu_pulse);
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }
            float opacity = alpha;
            if (Game_AreaSave != NULL && !Game_AreaSave[EDataList[episode].area_ids[0]].complete)
                opacity *= 0.25f;
            if (episodesmode != 1 && episodestime < episodesduration)
                opacity *= episodestime / episodesduration;
            Text3DEx(EpisodeNumerals[episode], x, y, 1.0f, 0.95f, 0.95f, 0.95f, 0, red, green, blue,
                     static_cast<u8>(opacity * 255.0f));
            DrawPanel3DObject(x, y, 1.0f, 0.35f, 0.35f, 0.35f, 0, 0, 0, &WORLD->lev_objs[167].special, 0, opacity);
            menu->item_x[episode] = x;
            menu->item_y[episode] = y;
            menu->item_width[episode] = 0.175f;
            menu->item_height[episode] = 0.0f;
        }
    }
    for (i32 i = 6; i < 400; ++i) {
        menu->item_width[i] = 0.0f;
        menu->item_height[i] = 0.0f;
    }
}

extern i8 episodesmode;
extern f32 episodestime;
extern i32 last_hub_area;
extern f32 MainRenderTime;
extern f32 ICONX, ICONSIZE, STATSPOSY, DROPINALPHA;
extern i16 tSELECT, tBACK, tSELECTING;
void Hub_DrawAreaStats(f32, i32, i32);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);

void MenuDrawEpisodes(MENU_s *) {
    if (FadeSys.fade > 0.0f)
        return;

    f32 alpha = 1.0f;
    if (episodesmode == 1) {
        if (episodestime >= 0.5f)
            return;
        alpha = 1.0f - NU_SIN_LUT(static_cast<i32>(episodestime * 2.0f * 16384.0f));
        if (!(alpha > 0.0f))
            return;
    }

    Hub_DrawAreaStats(alpha, last_hub_area, 18);
    const f32 icon_alpha = (MenuPacket.active_player[0] ? 1.0f : DROPINALPHA) * alpha;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 166, icon_alpha, icon_alpha, 1, NULL);

    if (MainRenderTime <= 0.0f) {
        DrawEpisodesMenu(1, alpha);
    } else if (alpha > MainRenderTime) {
        const f32 menu_alpha = 1.0f - NU_SIN_LUT(static_cast<i32>((alpha - MainRenderTime) * 16384.0f + 16384.0f));
        DrawEpisodesMenu(0, menu_alpha * alpha);
    }

    if (MainRenderTime <= 0.0f && alpha == 1.0f)
        DrawPlayerIconPrompts(MenuPacket.active_player[0], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING,
                              MenuPacket.active_player[1], tSELECT, 1.0f, -1, tBACK, -1, tSELECTING);
}
