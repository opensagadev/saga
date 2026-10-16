#include "decomp.h"
#include "gameapi/edtools/edui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/gui/apimenu_internal.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/panel.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/world_shared.h"

extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern char **TTab;
extern i16 tCANCEL;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern TIMER BonusTimer;
extern "C" void PlaySfx(char *, NUVEC *);
extern i32 GetMenuID(void);
extern void SetBonusWinner(i32);
extern void ResetGameMessages(void);
extern GameObject_s *FindGameObject(i32, u32, i32, i32, i32);

static f32 MissionsAlpha;
static i32 hub_mission;
static f32 MissionIconScale[20];
static f32 MissionIconTargetX[20];
static f32 MissionIconX[20];

void EndMissionsMenu() {
    LevLock[3] = 1;
    LevTime[3] = 0.6f;
    MenuRememberCursor(&GameMenu[GameMenuLevel]);
    MenuReset();
    ResetTimer(&JoinInTimer, 0.0f);
}

void MenuInitMissions(MENU_s *) {
    MissionsAlpha = 0.0f;
    if (MissionSys != NULL) {
        for (i32 i = 0; i < MissionSys->count; ++i)
            MissionIconScale[i] = i == hub_mission ? 1.0f : 0.0f;
        for (i32 i = 0; i < MissionSys->count; ++i) {
            f32 x = 0.0f;
            if (i < hub_mission)
                x = -0.035f - (hub_mission - i) * 0.12f;
            else if (i != hub_mission)
                x = 0.035f + (i - hub_mission) * 0.12f;
            MissionIconTargetX[i] = MissionIconX[i] = x;
        }
    }
}

void MenuDrawMissions(MENU_s *menu) {
    if (MenuStopDraw)
        return;
    const f32 alpha = MenuAlpha;
    SmartTextEx(TTab[MissionSys->missions[hub_mission].name_id], 0.0f, -0.525f, 1.0f, 0.625f, 0.625f, 0.625f, 0, 255,
                191, 0, 1.7f, 1, 0, 0, static_cast<i32>(255.0f * MissionsAlpha * alpha));
    if (Game.mission_save.completed[hub_mission] != 0 && alpha * MissionsAlpha > 0.0f) {
        const f32 opacity = alpha * MissionsAlpha;
        Hub_DrawImportantBrick(211, -0.15f, HUB_EPISODETITLEY + 0.1f, opacity, -1, -1);
        char time[64], text[64];
        Text_MakeTime(Game.mission_save.best_times[hub_mission], 0, 1, 1, time);
        sprintf(text, "~0(~2%s~0)", time);
        Text3DEx(text, 0.15f, -0.365f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 0, 255, 0, static_cast<u8>(opacity * 255.0f));
    }
    for (i32 i = 0; i < MissionSys->count; ++i) {
        f32 size = MissionIconScale[i] * 0.075f + 0.125f;
        f32 x = MissionIconX[i];
        if (fabsf(x) > 1.0f)
            size = 0.0f;
        else {
            f32 opacity = (MissionIconScale[i] * 0.3f + 0.7f) * alpha;
            if (fabsf(x) > 0.5f)
                opacity *= 1.0f - (fabsf(x) - 0.5f) * 2.0f;
            if (opacity > 0.0f)
                DrawCharIcon(MissionSys->missions[i].find_char, x, -0.725f, 0.0f, size,
                             Game.mission_save.completed[i] >= 1 ? 168 : 167, opacity,
                             (i == hub_mission && menu_flash ? 0.7f : 1.0f) * opacity, 1, NULL);
            if (opacity == 0.0f)
                size = 0.0f;
        }
        menu->item_x[i] = x;
        menu->item_y[i] = -0.725f;
        menu->item_width[i] = size;
        menu->item_height[i] = size / GetAspectRatio();
        menu->item_column[i] = i;
        menu->item_row[i] = 0;
    }
    f32 opacity = MenuPacket.active_player[0] ? 1.0f : DROPINALPHA;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 166, opacity, opacity, 1, NULL);
    i32 select[2], cancel[2], status[2];
    if (!(MenuPacket.active_player[0] && MenuPacket.active_player[1])) {
        select[0] = select[1] = tSELECT;
        cancel[0] = cancel[1] = tEXIT;
        status[0] = status[1] = tSELECTING;
    } else {
        // Keep the second player's defaults: the reference leaves those locals unset.
        select[1] = tSELECT;
        cancel[1] = tEXIT;
        status[1] = tSELECTING;
        if (MenuPacket.reserved_1[0]) {
            select[0] = -1;
            cancel[0] = tCANCEL;
            status[0] = tSELECTED;
        } else {
            select[0] = tSELECT;
            cancel[0] = tEXIT;
            status[0] = tSELECTING;
        }
    }
    DrawPlayerIconPrompts(MenuPacket.active_player[0], select[0], 1.0f, -1, cancel[0], -1, status[0],
                          MenuPacket.active_player[1], select[1], 1.0f, -1, cancel[1], -1, status[1]);
}

void MenuUpdateEndMission(MENU_s *menu) {
    if (menu->menu_time >= 5.0f) {
        CompleteLevel(WORLD);
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

void MenuDrawEndMission(MENU_s *) {
    if (MenuStopDraw != 0)
        return;
    if (MissionSys->field8_0x1d != 2) {
        SmartText(TTab[tOUTOFTIME], 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0,
                  191 + (static_cast<u32>(menu_flash) < 1 ? 64 : 0), 31 + (static_cast<u32>(menu_flash) < 1 ? 32 : 0),
                  0, 1.7f, 1);
        return;
    }
    if (NuFmod(GameTimer.time_elapsed, 0.3f) < 0.2f) {
        char text[32];
        i32 mission_index = static_cast<i8>(MissionSys->mission->count);
        f32 remaining =
            static_cast<f32>(static_cast<i32>(MissionSys->missions[mission_index].time)) - BonusTimer.time_elapsed;
        if (remaining < 0.0f)
            remaining = 0.0f;
        Text_MakeTime(remaining, 0, 1, 1, text);
        Text3D(text, 0.0f, STATSPOSY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
    }
}

void InitMission(MISSIONSYS *ms, i32 idx) {
    ms->field8_0x1d = 1;
    ms->mission = &ms->missions[idx];
    ResetTimer(&ms->timer, 0.0f);
    Cheats_TurnOff(0);
}

void MenuUpdateMissions(MENU_s *menu) {
    const i32 previous = hub_mission;
    i32 start = 0, cancel = 0, left = 0, right = 0;
    if (MissionSys != NULL) {
        if (MenuPacket.active_player[0]) {
            const u32 pressed = GamePad[0].buttons_pressed;
            if (pressed & GAMEPAD_MENUSELECT) {
                if (!MenuPacket.active_player[1]) {
                    start = 1;
                    goto collected_input;
                }
                if (!MenuPacket.reserved_1[0]) {
                    MenuPacket.reserved_1[0] = 1;
                    if (MenuPacket.reserved_1[1]) {
                        start = 1;
                        goto collected_input;
                    }
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                }
            } else if (pressed & GAMEPAD_MENUCANCEL) {
                if (!MenuPacket.active_player[1] || !MenuPacket.reserved_1[0]) {
                    cancel = 1;
                    goto collected_input;
                }
                MenuPacket.reserved_1[0] = 0;
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
            } else if (MenuPacket.reserved_1[0] == 0 && MenuPacket.reserved_1[1] == 0) {
                const u32 directions = GamePad[0].unknown_10;
                if ((directions & GAMEPAD_DLEFT) || (pressed & (GAMEPAD_DLEFT | GAMEPAD_TOGGLELEFT)))
                    left = 1;
                else if ((directions & GAMEPAD_DRIGHT) || (pressed & (GAMEPAD_DRIGHT | GAMEPAD_TOGGLERIGHT)))
                    right = 1;
            }
        }
        if (MenuPacket.active_player[1]) {
            const u32 pressed = GamePad[1].buttons_pressed;
            if (pressed & GAMEPAD_MENUSELECT) {
                if (!MenuPacket.active_player[0]) {
                    start = 1;
                    goto collected_input;
                }
                if (!MenuPacket.reserved_1[1]) {
                    MenuPacket.reserved_1[1] = 1;
                    if (MenuPacket.reserved_1[0]) {
                        start = 1;
                        goto collected_input;
                    }
                    GameAudio_PlaySfx(0x30, NULL, 0, 0);
                }
            } else if (pressed & GAMEPAD_MENUCANCEL) {
                if (!MenuPacket.active_player[0] || !MenuPacket.reserved_1[1]) {
                    cancel = 1;
                    goto collected_input;
                }
                MenuPacket.reserved_1[1] = 0;
                GameAudio_PlaySfx(0x31, NULL, 0, 0);
            } else if (MenuPacket.reserved_1[0] == 0 && MenuPacket.reserved_1[1] == 0) {
                const u32 directions = GamePad[1].unknown_10;
                if ((directions & GAMEPAD_DLEFT) || (pressed & (GAMEPAD_DLEFT | GAMEPAD_TOGGLELEFT)))
                    left = 1;
                else if ((directions & GAMEPAD_DRIGHT) || (pressed & (GAMEPAD_DRIGHT | GAMEPAD_TOGGLERIGHT)))
                    right = 1;
            }
        }
    } else
        cancel = 1;
collected_input:
    if (menu->input_activity) {
        if (menu->confirm_pressed) {
            if (hub_mission == menu->selected_column)
                start = 1;
            else
                hub_mission = menu->selected_column;
        }
        if (menu->cancel_pressed)
            cancel = 1;
        if (menu->left_pressed) {
            hub_mission -= static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
            if (hub_mission < 0)
                hub_mission = 0;
        }
        if (menu->right_pressed) {
            hub_mission += static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
            if (hub_mission >= MissionSys->count)
                hub_mission = MissionSys->count - 1;
        }
    }
    if (start) {
        InitMission(MissionSys, hub_mission);
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
        NextArea_FreePlay = FreePlay = 0;
        NewLData = &LDataList[MissionSys->mission->level];
    } else if (cancel) {
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
        EndMissionsMenu();
    } else if (left) {
        if (hub_mission > 0)
            --hub_mission;
    } else if (right && hub_mission < MissionSys->count - 1)
        ++hub_mission;
    if (hub_mission != previous) {
        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        MissionsAlpha = 0.0f;
    } else {
        MissionsAlpha += FRAMETIME * 2.0f;
        if (MissionsAlpha > 1.0f)
            MissionsAlpha = 1.0f;
    }
    if (MissionSys != NULL) {
        for (i32 i = 0; i < MissionSys->count; ++i)
            MissionIconScale[i] = SeekValF(MissionIconScale[i], i == hub_mission ? 1.0f : 0.0f, 5.0f);
        const u8 *mission_count = &MissionSys->count;
        for (i32 i = 0; i < *mission_count; ++i) {
            f32 x = 0.0f;
            if (i < hub_mission)
                x = -0.035f - (hub_mission - i) * 0.12f;
            else if (i != hub_mission)
                x = 0.035f + (i - hub_mission) * 0.12f;
            MissionIconTargetX[i] = x;
            MissionIconX[i] = SeekValF(MissionIconX[i], x, 5.0f);
        }
    }
}

void EndMission(MISSIONSYS *ms, i32 param1, i32 param2) {
    if (netclient == 0 || param2 == 0) {
        ms->field8_0x1d = (u8)param1;
        SetBonusWinner(qrand() / 0x8000);
        if (Player[BonusWinner] == NULL || !Player[BonusWinner]->apiobj.player_controlled) {
            BonusWinner = (BonusWinner == 0);
        }
        BonusWinFlag = 0;
        NewMenu(9, -1, -1);
        ResetGameMessages();
        if (param1 == 3) {
            PlaySfx("MK-Panel", 0);
        }
    }
}

void CheckMissionEnd(MISSIONSYS *ms) {
    GameObject_s *player;
    GameObject_s *obj;
    u64 mask;
    i32 i;

    obj = FindGameObject((u32)ms->mission->find_char, 0, 0, 1, 0);
    if (ms->timer.time_elapsed < 3.0f) {
        return;
    }
    if (GetMenuID() == 9) {
        return;
    }

    if (obj != NULL) {
        mask = *(u64 *)&obj->apiobj.field_0x1e4;
    } else {
        mask = 0;
        for (i = 0; i < HIGHGAMEOBJECT; i++) {
            GameObject_s *candidate = &Obj[i];
            if ((candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && candidate->apiobj.field_0x287 == 0 &&
                (u8)candidate->apiobj.field_0x27c == 0xff) {
                mask |= *(u64 *)&candidate->apiobj.field_0x1e4;
            }
        }
    }

    for (i = 0; i < 8; i++) {
        player = Player[i];
        if (player != NULL && *(i8 *)&player->apiobj.field_0x1f8 < 0) {
            if ((mask & *(u64 *)&player->apiobj.field_0x1ec) != 0) {
                EndMission(ms, 2, 1);
                return;
            }
        }
    }
}
