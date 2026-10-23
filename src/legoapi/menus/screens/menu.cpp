#include <stdio.h>
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "gameapi/edtools/edui.h"
#include <math.h>
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/audio/audio.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/items/collect/torpedo.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nurndr.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"
#include "batman.h"
#include "globals.h"
#include "legoapi/actions/movement/dropinout.h"
#include "legoapi/core/startup/text.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/lib/text.h"
#include "legoapi/menus/core/hint.h"
#include "legoapi/menus/screens/customise.h"
#include "legoapi/menus/screens/menu.h"
#include "legoapi/menus/screens/missions.h"
#include "legoapi/world/levels.h"
#include "legoapi/world/levels/hub.h"
#include "legogame/game.h"
#include "nu2api/nucore/nu2api_nucore_misc.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/credits.h"

extern u8 MENULOSTR1, MENULOSTG1, MENULOSTB1, MENULOSTA1;
extern u8 MENULOSTR2, MENULOSTG2, MENULOSTB2, MENULOSTA2;
extern i16 tALLLEVELPROGRESSWILLBELOST, tALLSUPERSTORYPROGRESSWILLBELOST;
extern i16 tGAMEPROGRESSWILLBELOST, tEXITTOMAP, tQUITGAME, tRESTARTDEMO2;
extern i16 tARCADE_NOWIN, tEXITWITHOUTSAVING, tSAVEANDEXIT;
extern i16 tHELPANDOPTIONS;
extern i16 tEXTRAS;
extern i16 tDROPOUT;
extern i16 tMAP;
extern i16 tRESTARTDEMO;
extern i16 tQUIT;
extern i16 tRESTARTLEVEL;
extern i16 tYES;
extern i16 tNO;
extern i16 tRESUME;
extern i16 tSKIP;
extern i16 tEXITTOMAP;
extern i16 tALLLEVELPROGRESSWILLBELOST;
extern i16 tALLSUPERSTORYPROGRESSWILLBELOST;
extern i16 tEXITWITHOUTSAVING;
extern i16 tSAVEANDEXIT;
extern i16 tRESTARTDEMO2;
extern i16 tGAMEPROGRESSWILLBELOST;
extern i16 tQUITGAME;
extern i16 tMUSIC;
extern i16 tHOWTOPLAY;
extern i16 tON;
extern i16 tOFF;
extern i16 tALLSTUDSCOLLECTED;
extern i16 tSELECTING;
extern i16 tCONTROLLER;
extern i16 tTOUCHTOSTART;
extern i16 tNEWGAME;
extern i16 tLOADGAME;
extern i16 tCONTROLS;
extern i16 tCONTROLLERCONNECTED;
extern i16 tTOUCH;
extern i16 tCONSOLE;
extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tWIDESCREEN;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern f32 newgamecamtime;
struct MENU_s;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
static f32 newgamewait;
static u32 buttons_store[15];
// Initialised by the unit's static constructor; nothing in the retail binary reads it.
static __used__ u32 gameoptions_sequence[15] = {
    GAMEPAD_TAG,    GAMEPAD_SPECIAL, GAMEPAD_SPECIAL, GAMEPAD_JUMP, GAMEPAD_JUMP,
    GAMEPAD_JUMP,   GAMEPAD_ACTION,  GAMEPAD_ACTION,  GAMEPAD_ACTION, GAMEPAD_ACTION,
    GAMEPAD_TAG,    GAMEPAD_TAG,     GAMEPAD_TAG,     GAMEPAD_TAG,  GAMEPAD_TAG,
};
static f32 updateextras_current_y = 0.0f;
static f32 opts_sfx_wait = 1.5f;
static i32 opts_sfx_i;
static inline bool MenuCheatUnlocked(i32 cheat) {
    const u32 *unlocked = reinterpret_cast<const u32 *>(Game.field_0x7c00);
    return ((unlocked[cheat / 32] >> (cheat & 31)) & 1) != 0;
}

static bool MenuCheatAvailable(i32 cheat) {
    if (cheat < 0 || cheat >= 44) {
        return false;
    }
    const u32 *unlocked = reinterpret_cast<const u32 *>(Game.field_0x7c00);
    return (unlocked[cheat >> 5] & (1u << (cheat & 31))) != 0;
}

static void MenuDrawTitles(MENU *);
static void MenuUpdateTitles(MENU *);
static void MenuEnterTitles(MENU *);
static void MenuDrawBonusComplete(MENU *);
static void MenuDrawBonusWin(MENU *);
static void MenuDrawHowToPlay(MENU *);
static void MenuDrawMiniKit(MENU *);
static void MenuDrawPauseCut(MENU *);
static void MenuDrawPauseExit(MENU *);
static void MenuDrawPauseMain(MENU *);
static void MenuDrawPauseRestart(MENU *);
static void MenuDrawSelectLanguage(MENU *);
static void MenuDrawShop(MENU *);
static void MenuExitHowToPlay(MENU *);
static void MenuInitHowToPlay(MENU *);
static void MenuUpdateBonusComplete(MENU *);
static void MenuUpdateBonusWin(MENU *);
static void MenuUpdateHowToPlay(MENU *);
static void MenuUpdateMiniKit(MENU *);
static void MenuUpdatePauseCut(MENU *);
static void MenuUpdatePauseExit(MENU *);
static void MenuUpdatePauseMain(MENU *);
static void MenuUpdatePauseRestart(MENU *);
static void MenuUpdateSelectLanguage(MENU *);
static void MenuUpdateShop(MENU *);

static void MenuRefreshPauseCutTarget() {
    pausecut_skip_to_level = -1;
    pausecut_skip_to_gameplay = 0;
    if (CutScenePlayer_Active() != 0) {
        return;
    }

    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if ((cut->flags & 0x40000) != 0) {
        return;
    }

    AREADATA *area = WORLD->area;
    if (area != NULL && (area->flags & 2) != 0) {
        if (CREDITS_LDATA != NULL) {
            pausecut_skip_to_level = CREDITS_LDATA->idx;
        }
        return;
    }

    i32 level_flags = WORLD->current_level->flags;
    if ((level_flags & LEVEL_OUTRO) != 0 || (cut->flags & 0x20000) != 0 ||
        (cut->skip_level != -1 && (LDataList[cut->skip_level].flags & LEVEL_STATUS) != 0)) {
        Area_FindStatusLevel(area, &pausecut_skip_to_level);
    } else if ((level_flags & LEVEL_INTRO) != 0) {
        LEVELDATA_s *level = Area_FindNextPlayLevel(WORLD->level_idx);
        if (level != WORLD->current_level) {
            pausecut_skip_to_level = level->idx;
        }
    } else if ((level_flags & LEVEL_MIDTRO) != 0 || (cut->flags & 0x80000) != 0) {
        if (cut->skip_level != -1 && cut->skip_level != WORLD->level_idx) {
            pausecut_skip_to_level = cut->skip_level;
        }
    } else if ((cut->skip_level == -1 || cut->skip_level == WORLD->level_idx) && (level_flags & LEVEL_GAMEPLAY) != 0) {
        pausecut_skip_to_gameplay = 1;
    }
}

MENUFNINFO GameMenuInfo[LEGO_MENU_INFO_COUNT] = {
    {0, MenuEnterTitles, MenuDrawTitles, MenuUpdateTitles, NULL, -1, -1, 0},
    {1, MenuEnterNewGame, MenuDrawNewGame, MenuUpdateNewGame, MenuExitNewGame, -1, -1, 0},
    {2, NULL, MenuDrawPauseMain, MenuUpdatePauseMain, NULL, -1, -1, 0},
    {3, NULL, MenuDrawPauseRestart, MenuUpdatePauseRestart, NULL, -1, -1, 0},
    {4, MenuEnterOptions, MenuDrawOptions, MenuUpdateOptions, MenuExitOptions, 0, 0, 0},
    {5, NULL, MenuDrawExtras, MenuUpdateExtras, NULL, 0, 0, 0},
    {6, NULL, MenuDrawPauseExit, MenuUpdatePauseExit, NULL, -1, -1, 0},
    {7, NULL, MenuDrawSelectLanguage, MenuUpdateSelectLanguage, NULL, 0, 0, 0},
    {8, MenuInitMissions, MenuDrawMissions, MenuUpdateMissions, NULL, -1, -1, 0},
    {9, NULL, MenuDrawEndMission, MenuUpdateEndMission, NULL, -1, -1, 0},
    {10, NULL, MenuDrawEndChallenge, MenuUpdateEndChallenge, NULL, -1, -1, 0},
    {11, NULL, Arcade_DrawEndMenu, Arcade_UpdateEndMenu, NULL, -1, -1, 0},
    {12, NULL, CustomiserMenu_Draw, CustomiserMenu_Update, NULL, -1, -1, 0},
    {13, NULL, MenuDrawShop, MenuUpdateShop, NULL, -1, -1, 0},
    {14, NULL, MenuDrawMiniKit, MenuUpdateMiniKit, NULL, -1, -1, 0},
    {15, MenuInitSelectMode, MenuDrawSelectMode, MenuUpdateSelectMode, NULL, -1, -1, 0},
    {16, MenuInitBonusMode, MenuDrawBonusMode, MenuUpdateBonusMode, NULL, -1, -1, 0},
    {17, MenuInitFreePlay, MenuDrawFreePlay, MenuUpdateFreePlay, NULL, -1, -1, 0},
    {18, MenuInitEpisodes, MenuDrawEpisodes, MenuUpdateEpisodes, NULL, -1, -1, 0},
    {19, MenuInitClips, MenuDrawClips, MenuUpdateClips, NULL, -1, -1, 0},
    {20, MenuInitStoreHolding, MenuDrawStoreHolding, MenuUpdateStoreHolding, MenuExitStoreHolding, -1, -1, 0},
    {21, MenuInitStore, MenuDrawStore, MenuUpdateStore, MenuExitStore, -1, -1, 0},
    {22, MenuInitStoreRestoring, MenuDrawStoreRestoring, MenuUpdateStoreRestoring, MenuExitStoreRestoring, -1, -1, 0},
    {23, MenuInitStorePurchase, MenuDrawStorePurchase, MenuUpdateStorePurchase, MenuExitStorePurchase, -1, -1, 0},
    {24, NULL, MenuDrawDebugStore, MenuUpdateDebugStore, NULL, 0, 0, 0},
    {25, MenuInitHowToPlay, MenuDrawHowToPlay, MenuUpdateHowToPlay, MenuExitHowToPlay, 0, 0, 0},
    {26, NULL, MenuDrawHints, MenuUpdateHints, NULL, 0, 0, 0},
    {27, NULL, MenuDrawPauseCut, MenuUpdatePauseCut, NULL, -1, -1, 0},
    {29, NULL, MenuDrawRestoreNewGame, MenuUpdateRestoreNewGame, NULL, -1, -1, 0},
    {30, NULL, NULL, Credits_UpdateMenu, NULL, -1, -1, 0},
    {31, NULL, MenuDrawBonusWin, MenuUpdateBonusWin, NULL, -1, -1, 0},
    {32, NULL, MenuDrawBonusComplete, MenuUpdateBonusComplete, NULL, -1, -1, 0},
    {33, NULL, MenuDrawSelectControls, MenuUpdateSelectControls, NULL, -1, -1, 0},
};

static __used__ void MenuInitHowToPlay(MENU *) {
    // The original callback is empty.
}

void MenuEnterNewGame(MENU_s *) {
    if (MenuLoadOccurred == 0 && startnewgame_initiated == 0 && startnewgame == 0) {
        saveload_autosave = -1;
    }
    MenuLoadOccurred = MenuSaveOccurred = 0;
}

static __used__ void MenuEnterTitles(MENU *) {
    // The original callback is empty.
}

static __used__ void MenuUpdatePauseCut(MENU *menu) {
    MenuRefreshPauseCutTarget();
    if (__builtin_expect(menu->confirm_pressed != 0 && menu->selected_item == 0, 0)) {
        ResumeGame(1, 1);
        return;
    }
    if (menu->confirm_pressed != 0 && menu->selected_item == 1) {
        if (CutScenePlayer_Active() != 0) {
            NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
            hub_from_cutsceneplayer = 1;
        } else {
            if (pausecut_skip_to_level != -1) {
                NewLData = &LDataList[pausecut_skip_to_level];
                cutskip_dontplaylevelintro = 1;
                music_man.StopTrack(0x10, 0);
            } else if (pausecut_skip_to_gameplay != 0) {
                instNuGCutSceneEnd(static_cast<instNUGCUTSCENE_s *>(static_cast<CUTINFO *>(CutStopInfo)->instance));
                if (CutStopInfo != NULL && (static_cast<CUTINFO *>(CutStopInfo)->flags & 0x400) != 0) {
                    NewMode = 1;
                }
                CutStopInfo = NULL;
                CUTSTOPGAME = 0;
                GameCam_Reset(GameCam);
                const FADETYPE fade = {FADE_TYPE_STILL_WIPE};
                FadeSys.SetFade(fade, 0);
                FadeSys.SetStage(1);
                GameAudio_PlaySfx(0x2d, NULL, 0, 0);
                ResumeGame(0, 0);
                music_man.StopTrack(0x10, 0);
            } else {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
            }
        }
    }
}

static __used__ void MenuDrawSelectLanguage(MENU *menu) {
    for (i32 i = 0; i < LANGUAGECOUNT; ++i) {
        GameDrawMenuEntry(menu, Text_LanguageList[i].name);
    }
}

void MenuDrawRestoreNewGame(MENU_s *menu) {
    menu->draw_y = -0.25f;
    GameDrawMenuEntry(menu, TTab[tCONTINUE]);
    GameDrawMenuEntry(menu, TTab[tCANCEL]);
}

static __used__ void MenuDrawPauseCut(MENU *menu) {
    GameDrawMenuEntry(menu, TTab[tRESUME]);
    i32 alpha = MenuA;
    if (CutScenePlayer_Active() == 0 && pausecut_skip_to_level == -1 && pausecut_skip_to_gameplay == 0) {
        alpha = alpha / 2;
    }
    DrawMenuEntryEx(menu, TTab[tSKIP], static_cast<u8>(alpha));
    dme_r = 0;
    dme_g = 0xff;
    dme_b = 0x3f;
}

static __used__ void MenuExitHowToPlay(MENU *) {
    GameAudio_PlaySfx(0x31, NULL, 0, 0);
    TriggerExtraDataSave();
}

static __used__ void MenuDrawHowToPlay(MENU *menu) {
    char text[64];
    menu->draw_y = 0.5f;

    NuStrCpy(text, TTab[tMUSIC]);
    NuStrCat(text, ": ");
    NuStrCat(text, SuperOptions.music_enabled != 0 ? TTab[tON] : TTab[tOFF]);
    GameDrawMenuEntry(menu, text);

    NuStrCpy(text, TTab[tHOWTOPLAY]);
    NuStrCat(text, " (");
    if (TestForController() != 0 && TTab[tCONTROLLER] != NULL) {
        NuStrCat(text, TTab[tCONTROLLER]);
    } else {
        NuStrCat(text, MechInputTouchSystem::s_baseControlMode != 0 ? TTab[tTOUCH] : TTab[tCONSOLE]);
    }
    NuStrCat(text, ")");
    GameDrawMenuEntry(menu, text);
}

void MenuDrawNewGame(MENU *menu) {
    const i32 menu_a = MenuA;
    const f32 menu_fade = newgamealpha;
    if (MenuLoadOccurred != 0 || newgame_menudrawoff != 0) {
        return;
    }

    f32 title_alpha;
    if (GameTimer.time_elapsed < 3.5f) {
        title_alpha = 0.0f;
    } else if (GameTimer.time_elapsed < 4.0f) {
        const i32 angle = static_cast<i32>((GameTimer.time_elapsed - 3.5f) * 16384.0f * 2.0f) >> 1;
        title_alpha = NuTrigTable[angle & 0x7fff];
    } else {
        title_alpha = 1.0f;
    }

    menu->draw_y = -0.5f - menu->centre_offset;
    const i32 menu_alpha = static_cast<i32>(static_cast<f32>(menu_a) * menu_fade);
    const i32 alpha = static_cast<i32>(static_cast<f32>(menu_alpha) * title_alpha);
    DrawMenuEntryEx(menu, TTab[tNEWGAME], static_cast<u8>(alpha));
    DrawMenuEntryEx(menu, TTab[tLOADGAME], static_cast<u8>(alpha));
    if (TestForController()) {
        DrawMenuEntryEx(menu, TTab[tCONTROLLERCONNECTED], static_cast<u8>(alpha / 2));
    } else {
        char controls[64];
        NuStrCpy(controls, TTab[tCONTROLS]);
        NuStrCat(controls, ": ");
        char *control_text;
        if (MechInputTouchSystem::s_baseControlMode != 0) {
            control_text = TTab[tTOUCH];
        } else {
            control_text = TTab[tCONSOLE];
        }
        NuStrCat(controls, control_text != NULL ? control_text : "?");
        DrawMenuEntryEx(menu, controls, static_cast<u8>(alpha));
    }
}

static __used__ void MenuUpdateMiniKit(MENU *) {
    Hub_UpdateKit();
}

static __used__ void MenuDrawPauseExit(MENU *menu) {
    LEVELDATA_s *save_level = CanSaveAndExit(WORLD);
    char *warning;
    char *title;
    f32 spacing;
    if (__builtin_expect(save_level != NULL, 0)) {
        title = TTab[tEXITTOMAP];
        warning = TTab[tALLLEVELPROGRESSWILLBELOST];
        spacing = 2.0f;
    } else {
        spacing = 1.5f;
        if (GAMEDEMO == 0) {
            if (WORLD->current_level == HUB_LDATA) {
                title = TTab[tQUITGAME];
                warning = TTab[tGAMEPROGRESSWILLBELOST];
            } else {
                title = TTab[tEXITTOMAP];
                if (Arcade != 0) {
                    warning = pause_i_pad != -1 ? TTab[tARCADE_NOWIN] : NULL;
                } else {
                    if (SuperStory != 0) {
                        warning = TTab[tALLSUPERSTORYPROGRESSWILLBELOST];
                    } else {
                        warning = WORLD->area != NULL && WORLD->area == VEHICLES_ADATA
                                      ? NULL
                                      : TTab[tALLLEVELPROGRESSWILLBELOST];
                    }
                }
            }
        } else {
            title = TTab[tRESTARTDEMO2];
            warning = TTab[tALLLEVELPROGRESSWILLBELOST];
        }
    }
    if (MenuStopDraw == 0) {
        SmartTextEx(title, PauseMenus_X, menu->draw_y - spacing * MENUDY, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE,
                    MENUTEXTSCALE, PauseMenus_Align, MENUEXITR, MENUEXITG, MENUEXITB, 1.7f, 1, NULL, 0, MenuA);
        if (warning != NULL) {
            const u16 angle = static_cast<u16>(NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f) * 2.0f * 65536.0f);
            const f32 blend = NuTrigTable[static_cast<u16>(angle / 2)] * 0.5f + 0.5f;
            const u8 r = static_cast<u8>(MENULOSTR1 + (MENULOSTR2 - MENULOSTR1) * blend);
            const u8 b = static_cast<u8>(MENULOSTB1 + (MENULOSTB2 - MENULOSTB1) * blend);
            const u8 g = static_cast<u8>(MENULOSTG1 + (MENULOSTG2 - MENULOSTG1) * blend);
            const i32 a = static_cast<i32>(static_cast<i32>(MENULOSTA1 + (MENULOSTA2 - MENULOSTA1) * blend) *
                                           (static_cast<f32>(MenuA) * 0.0078125f));
            SmartTextEx(warning, PauseMenus_X,
                        menu->draw_y + (save_level != NULL ? 2.0f : 1.0f) * MENUDY + spacing * MENUDY, 1.0f,
                        MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, PauseMenus_Align, r, g, b, 1.7f, 1, NULL, 0, a);
        }
    }
    if (__builtin_expect(save_level != NULL, 0)) {
        GameDrawMenuEntry(menu, TTab[tEXITWITHOUTSAVING]);
        GameDrawMenuEntry(menu, TTab[tSAVEANDEXIT]);
        GameDrawMenuEntry(menu, TTab[tCANCEL]);
    } else {
        GameDrawMenuEntry(menu, TTab[tYES]);
        GameDrawMenuEntry(menu, TTab[tNO]);
    }
}

static void MenuDrawPauseRestart(MENU *menu) {
    if (MenuStopDraw == 0) {
        SmartTextEx(TTab[tRESTARTLEVEL], PauseMenus_X, menu->draw_y - MENUDY * 0.25f, 1.0f, MENUTEXTSCALE,
                    MENUTEXTSCALE, MENUTEXTSCALE, PauseMenus_Align, MENUEXITR, MENUEXITG, MENUEXITB, 1.7f, 1, NULL, 0,
                    MenuA);
    }
    GameDrawMenuEntry(menu, TTab[tYES]);
    GameDrawMenuEntry(menu, TTab[tNO]);
}

static inline __used__ void MenuDrawPauseMain(MENU *menu) {
    const i32 both_players_active = Players_BothActive();

    dme_r = 0;
    dme_g = 0xff;
    dme_b = 0x3f;

    if (TestForController() != 0) {
        DrawMenuEntryEx(menu, TTab[tCONTROLLERCONNECTED], static_cast<u8>(MenuA / 2));
    } else {
        char controls[64];
        NuStrCpy(controls, TTab[tCONTROLS]);
        NuStrCat(controls, ": ");
        char *control_name = MechInputTouchSystem::s_baseControlMode != 0 ? TTab[tTOUCH] : TTab[tCONSOLE];
        NuStrCat(controls, control_name != NULL ? control_name : "?");
        GameDrawMenuEntry(menu, controls);
    }

    GameDrawMenuEntry(menu, TTab[tHELPANDOPTIONS]);

    if (SuperStory != 0 || ChallengeMode != 0 || Mission_Active(NULL) != NULL) {
        DrawMenuEntryEx(menu, TTab[tEXTRAS], static_cast<i32>(MenuA * 0.5f) & 0xff);
    } else {
        GameDrawMenuEntry(menu, TTab[tEXTRAS]);
    }
    if (both_players_active != 0) {
        GameDrawMenuEntry(menu, TTab[tDROPOUT]);
    }

    char *last_entry;
    if (WORLD->current_level != HUB_LDATA) {
        if (GAMEDEMO != 0) {
            last_entry = TTab[tRESTARTDEMO];
        } else {
            last_entry = TTab[tMAP];
        }
    } else {
        last_entry = TTab[tQUIT];
    }
    GameDrawMenuEntry(menu, last_entry);
}

void MenuDrawOptions(MENU_s *menu) {
    char text[256];

    menu->item_scale = 1.0f;
    dme_sy = 1.0f;

    if (TestForController() != 0) {
        DrawMenuEntryEx(menu, TTab[tCONTROLLERCONNECTED], MenuA / 2);
    } else {
        NuStrCpy(text, TTab[tCONTROLS]);
        NuStrCat(text, ": ");
        NuStrCat(text, TTab[MechInputTouchSystem::s_baseControlMode == 0 ? tCONSOLE : tTOUCH]);
        GameDrawMenuEntry(menu, text);
    }

    NuStrCpy(text, TTab[tSURROUNDSOUND]);
    NuStrCat(text, ": ");
    NuStrCat(text, TTab[TempOptions.field2_0x2 != 0 ? tON : tOFF]);
    dme_sy = menu->item_scale;
    GameDrawMenuEntry(menu, text);

    sprintf(text, "%s: %i/%i", TTab[tAUDIOVOLUME], TempOptions.field5_0x5, 10);
    dme_sy = menu->item_scale;
    GameDrawMenuEntry(menu, text);

    NuStrCpy(text, TTab[tMUSIC]);
    NuStrCat(text, ": ");
    NuStrCat(text, TTab[TempOptions.field6_0x6 != 0 ? tON : tOFF]);
    dme_sy = menu->item_scale;
    GameDrawMenuEntry(menu, text);

    if (__builtin_expect(GAMEDEMO == 0, 1)) {
        sprintf(text, "%s: %s", TTab[tWIDESCREEN], TTab[TempOptions.field11_0xb != 0 ? tON : tOFF]);
        dme_sy = menu->item_scale;
        GameDrawMenuEntry(menu, text);
    }

    if (__builtin_expect(memcmp(&TempOptions, &Game.options_save, sizeof(TempOptions)) == 0, 0)) {
        NuStrCpy(text, TTab[tBACK]);
    } else {
        if (menu->selected_item != menu->draw_item && menu_flash != 0) {
            text[0] = '\0';
        } else {
            NuStrCpy(text, TTab[tACCEPT]);
        }
    }
    dme_sy = menu->item_scale;
    GameDrawMenuEntry(menu, text);
}

void MenuDrawSelectControls(MENU_s *menu) {
    MenuSmartTextEx(TTab[tSELECTCONTROLS], 0.0f, 0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.4f, 2, NULL, 0, MenuA);
    MenuSmartTextEx(TTab[tCONTROLSCANBECHANGED], 0.0f, -0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.4f, 2, NULL, 0, MenuA);
    DrawMenuEntry(menu, TTab[tCLASSIC]);
    DrawMenuEntry(menu, TTab[tTOUCHSCREEN]);
}

void MenuUpdateRestoreNewGame(MENU_s *menu) {
    startnewgame = 0;

    if (startnewgame_initiated != 0 || saveload_autosave != -1) {
        startnewgame_initiated = 0;
        startnewgame = 1;
    } else if (menu->confirm_pressed == 0) {
        return;
    } else if (menu->selected_item == 0) {
        MenuSFX = GameAudio_GetSfxId(0x30);
        NuIOS_RecordFlurryEvent("menu_restore");
        NuIOS_RestoreInAppPurchases();
        NewMenu(22, -1, -1);
    } else if (menu->selected_item == 1) {
        MenuSFX = GameAudio_GetSfxId(0x30);
        if (PlayerProgress[0].active == 0 && PlayerProgress[1].active == 0) {
            PlayerProgress[0].active = 1;
        }
        NewMenu(1000, -1, -1);
    } else if (menu->selected_item == 2) {
        BackupMenu();
    }

    if (startnewgame != 0) {
        startnewgame = 0;
        const i32 save_occurred = MenuSaveOccurred;
        NewGame();
        MenuSaveOccurred = save_occurred;
        NewLData = NEWGAME_LDATA;
        UsePlayerList = 2;
        newgamecam = 1;
        newgamecamtime = 0.0f;
    }
}

void MenuDrawExtras(MENU_s *menu) {
    char text[256];
    char value[64];
    menu->draw_y = -updateextras_current_y * MENUDY * menu->item_scale;
    for (i32 cheat = 0; cheat < 44; ++cheat) {
        f32 alpha = 0.5f;
        bool locked = false;

        if (MenuCheatAvailable(cheat)) {
            NuStrCpy(value, TTab[Cheat_IsOn(cheat) != 0 ? tON : tOFF]);
            if (BonusArea == 0 || Cheat_CheckFlags(cheat, 0x200000) == 0) {
                alpha = 1.0f;
            }
        } else if (cheat < 8 || static_cast<i8>(Cheat[cheat].area) == -1 ||
                   Game.area_save[static_cast<i8>(Cheat[cheat].area)].field_0x5[1] != 0) {
            NuStrCpy(value, TTab[tAVAILABLETOBUY]);
        } else {
            NuStrCpy(text, TTab[tPOWERBRICK]);
            sprintf(value, " %i: ", cheat - 7);
            NuStrCat(text, value);
            NuStrCat(text, TTab[tLOCKED]);
            locked = true;
        }
        if (!locked) {
            sprintf(text, "%s: %s", TTab[*Cheat[cheat].text_id], value);
        }

        dme_sy = menu->item_scale;
        if (cheat > 7) {
            dme_rgb = 1;
        }
        dme_r = 0xdf;
        dme_g = 0x3f;
        dme_b = 0;

        const f32 distance = NuFabs(menu->draw_y);
        if (distance > 0.15f) {
            alpha = distance > 0.6f ? 0.0f : alpha * (1.0f - (distance - 0.15f) / 0.45000002f);
        }
        if (Paused != 0) {
            dme_align = PauseMenus_Align;
            menu->draw_x = PauseMenus_X;
        }
        DrawMenuEntryEx(menu, text, static_cast<u8>(static_cast<f32>(MenuA) * alpha));
    }
}

void MenuUpdateExtras(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        const i32 cheat = menu->selected_item;
        if (MenuCheatUnlocked(cheat) && (BonusArea == 0 || Cheat_CheckFlags(cheat, 0x200000) == 0)) {
            Cheat_SetOn(cheat, Cheat_IsOn(cheat) == 0, 1);
            GameAudio_PlaySfx(0x30, NULL, 0, 0);
        } else {
            GameAudio_PlaySfx(0x32, NULL, 0, 0);
        }
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = GameAudio_GetSfxId(0x31);
    }
    updateextras_current_y = SeekValF(updateextras_current_y, static_cast<f32>(menu->selected_item), 5.0f);
}

void MenuExitOptions(MENU_s *) {
    RestoreOptions();
}

void MenuExitNewGame(MENU_s *) {
    if (PlayerProgress[0].active == 0 && PlayerProgress[1].active == 0) {
        PlayerProgress[0].active = 1;
    }
}

static __used__ void MenuUpdateBonusComplete(MENU *menu) {
    if (menu->menu_time >= 12.0f) {
        CompleteLevel(WORLD);
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

static __used__ void MenuUpdateBonusWin(MENU *menu) {
    if (menu->menu_time >= 12.0f) {
        CompleteLevel(WORLD);
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

static __used__ void MenuDrawBonusComplete(MENU *) {
    if (MenuStopDraw == 0) {
        const i32 green = 143 + (static_cast<u32>(menu_flash) < 1u ? 48 : 0);
        const i32 red = 191 + (static_cast<u32>(menu_flash) < 1u ? 64 : 0);
        SmartText(TTab[tALLSTUDSCOLLECTED], 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0, red, green, 0, 1.7f, 1);
    }
}

static void MenuDrawBonusWin(MENU *) {
    if (MenuStopDraw == 0) {
        char text[256];
        Text_MakeScore(1000000, text);
        const i32 green = 143 + (static_cast<u32>(menu_flash) < 1u ? 48 : 0);
        const i32 red = 191 + (static_cast<u32>(menu_flash) < 1u ? 64 : 0);
        Text3D(text, 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0, red, green, 0);
    }
}

static __used__ void MenuUpdateHowToPlay(MENU *menu) {
    if (menu->cancel_pressed != 0) {
        BackupMenu();
        return;
    }
    if (menu->confirm_pressed == 0) {
        return;
    }

    switch (menu->selected_item) {
        case 0:
            SuperOptions.music_enabled = SuperOptions.music_enabled == 0;
            MenuSFX = GameAudio_GetSfxId(0x30);
            break;
        case 1:
            NewMenu(26, -1, -1);
            MenuSFX = GameAudio_GetSfxId(0x30);
            NuIOS_RecordFlurryEvent("menu_howtoplay");
            break;
    }
}

static __used__ void MenuDrawMiniKit(MENU *) {
    if (MenuStopDraw == 0) {
        const f32 alpha = MenuPacket.active_player[0] != 0 ? 1.0f : DROPINALPHA;
        DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 0xa6, alpha, alpha, 1, NULL);
        DrawPlayerIconPrompts(MenuPacket.active_player[0], -1, 1.0f, -1, tEXIT, -1, tSELECTING,
                              MenuPacket.active_player[1], -1, 1.0f, -1, tEXIT, -1, tSELECTING);
        NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
        NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);
    }
}

static __used__ void MenuDrawShop(MENU *) {
    if (MenuStopDraw == 0 && shop_quit == 0) {
        const f32 alpha = MenuPacket.active_player[0] != 0 ? 1.0f : DROPINALPHA;
        DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 0xa6, alpha, alpha, 1, NULL);
        DrawShopPrompts();
    }
}

static __used__ void MenuUpdateSelectLanguage(MENU *menu) {
    if (menu->confirm_pressed != 0) {
        Text_SetLanguage(Text_LanguageList[menu->selected_item].language);
        BackupMenu();
    }
}

static __used__ void MenuUpdatePauseExit(MENU *menu) {
    if (menu->confirm_pressed != 0) {
        LEVELDATA_s *save_level = CanSaveAndExit(WORLD);
        if (save_level != NULL) {
            if (menu->selected_item == 0) {
                level_already_loaded = -1;
                NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
                MenuSFX = GameAudio_GetSfxId(0x30);
                return;
            } else if (menu->selected_item == 1) {
                NewLData = save_level;
                from_save_and_exit = 1;
                level_already_loaded = -1;
                FADETYPE wipe = {FADE_TYPE_WIPE};
                FadeSys.SetFade(wipe, 0);
                MenuSFX = GameAudio_GetSfxId(0x30);
                return;
            } else {
                BackupMenu();
                MenuSFX = GameAudio_GetSfxId(0x31);
                return;
            }
        } else if (menu->selected_item == 0) {
            if (GAMEDEMO != 0) {
                NewLevelFromMenu(TITLES_LDATA, -1, -1, 1);
            } else {
                save_level = CanSaveAndExit(WORLD);
                if (save_level != NULL) {
                    NewLData = save_level;
                } else if (WORLD->current_level == HUB_LDATA) {
                    NuPadSetStatus(0, 0);
                    NuPadSetStatus(1, 0);
                    NewLevelFromMenu(TITLES_LDATA, -1, -1, 0);
                } else {
                    g_BackgroundUsedFogColour = 0;
                    NewLevelFromMenu(HUB_LDATA, -1, -1, 1);
                }
            }
        } else {
            BackupMenu();
            MenuSFX = GameAudio_GetSfxId(0x31);
            return;
        }
        MenuSFX = GameAudio_GetSfxId(0x30);
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = GameAudio_GetSfxId(0x31);
    }
}

static __used__ void MenuDrawTitles(MENU *) {
    if (GameTimer.time_elapsed >= 3.0f && GameTimer.time_elapsed_mod_seconds < 0.666f) {
        SmartTextEx(TTab[tTOUCHTOSTART], 0.0f, (pNuCam->fov / pNuCam->aspect) * -0.5f, 1.0f, MENUTEXTSCALE,
                    MENUTEXTSCALE, MENUTEXTSCALE, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                    static_cast<i32>(static_cast<f32>(MenuA) * newgamealpha));
    }
}

static __used__ void MenuUpdatePauseRestart(MENU *menu) {
    if (menu->confirm_pressed != 0) {
        if (menu->selected_item == 0) {
            Door_Reset();
            Players_InitPositions(WORLD);
            NewGameMode();
            reset_restart = 1;
            ResetBits |= 0x21;
            MenuSFX = GameAudio_GetSfxId(0x30);
        } else {
            BackupMenu();
            MenuSFX = GameAudio_GetSfxId(0x31);
        }
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = GameAudio_GetSfxId(0x31);
    }
}

void MenuEnterOptions(MENU_s *) {
    const i32 parent_menu = GetParentMenuID();
    if (parent_menu == 1 || parent_menu == 2) {
        TempOptions = Game.options_save;
    }
}

static __used__ void MenuUpdatePauseMain(MENU *menu) {
    Game.options_save.field11_0xb = static_cast<u8>(NuIOS_IsWidescreen());
    if (menu->confirm_pressed == 0) {
        return;
    }

    const i32 both_players_active = Players_BothActive();
    MenuSFX = GameAudio_GetSfxId(0x30);

    if (menu->selected_item == 0) {
        if (TestForController() != 0) {
            MenuSFX = GameAudio_GetSfxId(0x32);
            return;
        }

        ++SuperOptions.touch_controls;
        if (SuperOptions.touch_controls > 1) {
            SuperOptions.touch_controls = 0;
        }
        MenuSFX = GameAudio_GetSfxId(0x30);
        MechSystems::Get()->input_touch_system.control_mode = SuperOptions.touch_controls != 0 ? 2 : 1;
        TriggerExtraDataSave();
        return;
    }

    if (menu->selected_item == 1) {
        NewMenu(25, -1, -1);
        return;
    }

    if (GAMEDEMO == 0 && menu->selected_item == 2) {
        if (SuperStory != 0 || ChallengeMode != 0 || Mission_Active(NULL) != NULL) {
            GameAudio_PlaySfx(0x32, NULL, 0, 0);
        } else {
            NewMenu(5, -1, -1);
            NuIOS_RecordFlurryEvent("menu_extras");
        }
        return;
    }

    i32 next_item = GAMEDEMO == 0 ? 3 : 2;
    if (both_players_active != 0) {
        if (menu->selected_item == next_item) {
            i32 dropout_player = -1;
            for (i32 player = 0; player < 8; ++player) {
                if (Player[player] != NULL) {
                    if (Player[player]->pad_gamepad == &GamePad[pause_i_pad]) {
                        dropout_player = player;
                    }
                    Player[player]->field_0xefe &= ~0x20;
                }
            }
            if (dropout_player != -1) {
                DropOut(dropout_player, 1, 0, 0);
            }
            return;
        }
        ++next_item;
    }

    if (menu->selected_item == next_item) {
        const i32 selected_row = CanSaveAndExit(WORLD) != NULL ? 2 : 1;
        NewMenu(6, selected_row, -1);
    }
}

void MenuUpdateSelectControls(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = GameAudio_GetSfxId(0x30);
        const bool touch_controls = menu->selected_row != 0;
        SuperOptions.touch_controls = touch_controls;
        const i32 control_mode = touch_controls ? 2 : 1;
        MechSystems::Get()->input_touch_system.control_mode = control_mode;
        TriggerExtraDataSave();
        BackupMenuNoFn();
        NewMenu(1000, -1, -1);
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
    }
}

static i32 UpdateTitleSequence(MENU *menu) {
    f32 music_volume = 0.0f;
    if (SuperOptions.music_enabled != 0) {
        music_volume = GameGetMusicVolume(&TempOptions);
    }

    if (newgamefade == 0) {
        legoSetMusicVolume(music_volume);
        return 0;
    }

    if (newgamefade == 1) {
        if (newgamewait > 0.0f) {
            newgamewait -= FRAMETIME;
        } else {
            newgamealpha += FRAMETIME * 2.0f;
            if (newgamealpha > 1.0f) {
                newgamealpha = 1.0f;
                newgamefade = 0;
            }
        }
    } else if (newgamewait > 0.0f) {
        newgamewait -= FRAMETIME;
        music_volume = 0.0f;
        if (newgamewait <= 0.0f) {
            if (NOSOUND == 0) {
                music_man.StopAll(0);
                MusicClearAll();
                SoundKillAll();
            }

            const f32 movie_volume = static_cast<f32>(static_cast<u32>(Game.options_save.field4_0x4)) / 10.0f;
            Movie_Play(Trailer[0], &characterbuffer_ptr, &characterbuffer_end, DEFAULTFRAMETIME, NULL, movie_volume);
            FinishLoop(2);
            ResetTimer(&GameTimer, music_volume);
            menu->menu_time = music_volume;
            menu->unk = music_volume;
            newgamefade = 1;
            newgamealpha = music_volume;
            newgamewait = 0.5f;

            if (NOSOUND == 0 && NOMUSIC == 0) {
                music_man.StopAll(0);
                GamePlayMusic(WORLD->current_level, 0, &Game.options_save);
            }
        }
    } else if (newgamefade == 2) {
        newgamealpha -= FRAMETIME * 2.0f;
        if (newgamealpha <= 0.0f) {
            newgamealpha = 0.0f;
            newgamewait = 0.1f;
        }
        music_volume *= newgamealpha;
    }

    legoSetMusicVolume(music_volume);
    return 1;
}

static __used__ void MenuUpdateTitles(MENU *menu) {
    if (UpdateTitleSequence(menu) != 0) {
        return;
    }
    if (GameTimer.time_elapsed < 4.0f) {
        return;
    }
    if (GameTimer.time_elapsed >= 3.0f) {
        if (menu->start_pressed == 0 && MechInputTouchMenuController::AnyTouchesThisFrame <= 0) {
            return;
        }

        MechInputTouchMenuController::AnyTouchesThisFrame = 0;
        NuPadSetStatus(0, NUPAD_STATUS_ACTIVE);
        NewMenu(1, saveload_savepresent != 0, -1);
        MenuSFX = GameAudio_GetSfxId(0x30);
        NuIOS_RecordFlurryEvent("loadscreen_startgame");
    }
}

void MenuUpdateOptions(MENU_s *menu) {
    GameSetSoundVolume(&TempOptions);
    if (__builtin_expect(WORLD->current_level != TITLES_LDATA, 1))
        GameSetMusicVolume(&TempOptions);
    else
        legoSetMusicVolume(SuperOptions.music_enabled != 0 ? GameGetMusicVolume(&TempOptions) : 0.0f);

    i32 preview = 0;
    opts_sfx_wait -= FRAMETIME;
    if (!(opts_sfx_wait > 0.0f)) {
        opts_sfx_i = static_cast<i32>(static_cast<u32>(opts_sfx_i) + 1);
        opts_sfx_wait += 1.5f;
        if (__builtin_expect(opts_sfx_i == 1, 1))
            opts_sfx_i = 0;
        preview = 1;
    }

    if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = GameAudio_GetSfxId(0x31);
    } else {
        i32 row = 0;
        if (menu->selected_item == row++) {
            if (menu->confirm_pressed != 0) {
                MenuSFX = GameAudio_GetSfxId(0x30);
                SuperOptions.touch_controls = SuperOptions.touch_controls == 0;
                i32 control_mode = SuperOptions.touch_controls == 0 ? 1 : 2;
                MechSystems::Get()->input_touch_system.control_mode = control_mode;
            }
        } else if (menu->selected_item == row++) {
            if (menu->confirm_pressed != 0) {
                TempOptions.surround_sound = TempOptions.surround_sound == 0;
                MenuSFX = GameAudio_GetSfxId(0x30);
            } else if (preview != 0 && TempOptions.surround_sound != 0) {
                i32 angle = static_cast<i32>(NuFmod(GlobalTimer.time_elapsed, 8.0f) * 0.125f * 65536.0f) & 0xffff;
                NUVEC position;
                position.x = GameCam->pos.x + NU_SIN_LUT(angle) * nusound_fade_start;
                position.y = GameCam->pos.y;
                position.z = GameCam->pos.z + NU_COS_LUT(angle) * nusound_fade_start;
                PlaySfx("PickupCoinB", &position);
            }
        } else if (menu->selected_item == row++) {
            menu->selected_item_column = TempOptions.master_volume;
            if (menu->left_pressed != 0 && TempOptions.master_volume != 0)
                --TempOptions.master_volume;
            else if (menu->right_pressed != 0 && TempOptions.master_volume < 10)
                ++TempOptions.master_volume;
        } else if (menu->selected_item == row++) {
            if (menu->confirm_pressed != 0) {
                TempOptions.music_enabled = TempOptions.music_enabled == 0;
                MenuSFX = GameAudio_GetSfxId(0x30);
            }
        } else if (GAMEDEMO == 0 && menu->selected_item == row++) {
            if (menu->confirm_pressed != 0) {
                TempOptions.widescreen = TempOptions.widescreen == 0;
                MenuSFX = GameAudio_GetSfxId(0x30);
            }
        }
        if (menu->selected_item == row && menu->confirm_pressed != 0) {
            i32 sound =
                GameAudio_GetSfxId(memcmp(&TempOptions, &Game.options_save, sizeof(TempOptions)) == 0 ? 0x31 : 0x30);
            Game.options_save = TempOptions;
            BackupMenu();
            SfxCheckMusicOnOff(&Game.options_save);
            MenuSFX = sound;
        }
    }
}

void MenuUpdateNewGame(MENU *menu) {
    if (menu->close_requested != 0 || menu->cancel_pressed != 0) {
        BackupMenu();
        return;
    }

    Game.options_save.field11_0xb = static_cast<u8>(NuIOS_IsWidescreen());

    if (MenuLoadOccurred != 0) {
        MenuLoadOccurred = 0;
        NewLData = LOADGAME_LDATA;
        UsePlayerList = 2;
        GameSetSoundVolume(&Game.options_save);
        legoSetMusicVolume(SuperOptions.music_enabled != 0 ? GameGetMusicVolume(&TempOptions) : 0.0f);
        Hint_LoadAllGameState();
        LSW_HintConditions.force_used = 1;
        LSW_HintConditions.tc14_present = 1;
        LSW_HintConditions.panel_used = 1;
        newgamealpha = 0.0f;

        u8 brightness = Game.options_save.field12_0xc;
        if (brightness > 20) {
            Game.options_save.field12_0xc = 10;
            brightness = 10;
        }
        NuVideoSetBrightness(static_cast<f32>(brightness) / 10.0f);
        return;
    }

    if (UpdateTitleSequence(menu) != 0 || GameTimer.time_elapsed < 4.0f) {
        return;
    }

    if (__builtin_expect((GamePad[0].unknown_04 & GAMEPAD_START) != 0, 1)) {
        NuPadSetStatus(GamePad[0].pad->port, NUPAD_STATUS_ACTIVE);
    }
    if (__builtin_expect(static_cast<u8>(GamePad[0].unknown_24) == 0, 0) || GamePad[0].pad->is_valid == 0) {
        PlayerProgress[0].active = 0;
    } else if (__builtin_expect(g_nupadMapping[0].is_active != 0, 1)) {
        PlayerProgress[0].active = 1;
    }

    if ((GamePad[1].unknown_04 & GAMEPAD_START) != 0) {
        NuPadSetStatus(GamePad[1].pad->port, NUPAD_STATUS_ACTIVE);
    }
    if (static_cast<u8>(GamePad[1].unknown_24) == 0 || GamePad[1].pad->is_valid == 0) {
        PlayerProgress[1].active = 0;
    } else if (g_nupadMapping[1].is_active != 0) {
        PlayerProgress[1].active = 1;
    }

    startnewgame = 0;
    if (__builtin_expect(startnewgame_initiated != 0 || saveload_autosave != -1, 0)) {
        startnewgame_initiated = 0;
        startnewgame = 0;
        const i32 save_occurred = MenuSaveOccurred;
        NewGame();
        MenuSaveOccurred = save_occurred;
        NewLData = NEWGAME_LDATA;
        UsePlayerList = 2;
        newgamecam = 1;
        newgamecamtime = 0.0f;
        return;
    }

    if (menu->confirm_pressed == 0) {
        return;
    }

    switch (menu->selected_item) {
        case 0:
            MenuSFX = GameAudio_GetSfxId(0x30);
            if (PlayerProgress[0].active == 0 && PlayerProgress[1].active == 0) {
                PlayerProgress[0].active = 1;
            }
            if (TestForController()) {
                NewMenu(1000, -1, -1);
            } else {
                NewMenu(33, -1, -1);
            }
            NuIOS_RecordFlurryEvent("mainmenu_newgame");
            break;

        case 1:
            MenuSFX = GameAudio_GetSfxId(0x30);
            NewMenu(1012, -1, -1);
            NuIOS_RecordFlurryEvent("mainmenu_loadgame");
            break;

        case 2:
            if (TestForController()) {
                MenuSFX = GameAudio_GetSfxId(0x32);
            } else {
                MenuSFX = GameAudio_GetSfxId(0x30);
                ++SuperOptions.touch_controls;
                if (SuperOptions.touch_controls > 1) {
                    SuperOptions.touch_controls = 0;
                }
                MenuSFX = GameAudio_GetSfxId(0x30);
                MechSystems::Get()->input_touch_system.control_mode = SuperOptions.touch_controls != 0 ? 2 : 1;
                TriggerExtraDataSave();
            }
            break;

        case 3:
            NuIOS_RecordFlurryEvent("mainmenu_restore");
            NuIOS_RestoreInAppPurchases();
            NewMenu(22, -1, -1);
            break;
    }

    if (startnewgame != 0) {
        const i32 save_occurred = MenuSaveOccurred;
        NewGame();
        MenuSaveOccurred = save_occurred;
        NewLData = NEWGAME_LDATA;
        UsePlayerList = 2;
        newgamecam = 1;
        newgamecamtime = 0.0f;
    }
}

void DrawGameState(float x, float y, i32 highlight, i32 slot) {
    const f32 text_scale = MENUTEXTSCALE;
    char game_name[256];
    if (slot == -1) {
        NuStrCpy(game_name, TTab[tCURRENTGAME]);
    } else {
        sprintf(game_name, "%s %i", TTab[tGAME], slot + 1);
    }

    u8 red;
    u8 green;
    u8 blue;
    if (highlight != 0) {
        if (TestForController() == 0) {
            if (menu_pulse > 0.0f) {
                red = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                                       static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse)));
                green = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                                         static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse)));
                blue = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                                        static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse)));
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }
        } else {
            if (menu_pulsate > 0.0f) {
                red = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                                       static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate)));
                green = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                                         static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate)));
                blue = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                                        static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate)));
            } else if (menu_flash != 0) {
                red = MENUFLASH0R;
                green = MENUFLASH0G;
                blue = MENUFLASH0B;
            } else {
                red = MENUFLASH1R;
                green = MENUFLASH1G;
                blue = MENUFLASH1B;
            }
        }
    } else {
        if (menu_pulse > 0.0f) {
            red = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                                   static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse)));
            green = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                                     static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse)));
            blue = static_cast<u8>(static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                                    static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse)));
        } else {
            red = MENUENTRYR;
            green = MENUENTRYG;
            blue = MENUENTRYB;
        }
    }
    smarttextex_drawmessagebox = 2;
    SmartTextEx(game_name, x, y, 1.0f, text_scale, text_scale, text_scale, 4, red, green, blue, 0.45f, 1, NULL, 0,
                MenuA);

    if (slot == -1 || saveload_slotused[slot] != 0) {
        char progress[32];
        const u32 completion = slot == -1 ? Game.completion : saveload_slotcode[slot];
        sprintf(progress, "%.1f%%", static_cast<f32>(static_cast<i32>(completion * 100)) / COMPLETIONPOINTS);
        Text_LocaliseDecimalPoint(progress);
        Text3DEx(progress, x, y, 1.0f, text_scale, text_scale, text_scale, 1, 255, 191, 0, static_cast<u8>(MenuA));
    } else {
        if (saveload_freespace >= SAVESIZE_ADDITIONAL) {
            SmartTextEx(TTab[tEMPTY], x, y, 1.0f, text_scale, text_scale, text_scale, 1, 0, 255, 0, 0.45f, 2, NULL, 0,
                        MenuA);
        } else {
            SmartTextEx(TTab[tNOSPACE], x, y, 1.0f, text_scale, text_scale, text_scale, 1, 255, 0, 0, 0.45f, 2, NULL, 0,
                        MenuA);
        }
    }
}

void DrawSaveSlots(MENU_s *menu, float y) {
    DrawGameState(-0.5f, y, menu->selected_row == menu->first_row && menu->selected_column == 0, 0);
    menu->item_width[2] = text3d_width;
    menu->item_column[2] = 0;
    menu->item_y[2] = y;
    menu->item_x[2] = -0.5f;
    menu->item_height[2] = text3d_height * 2.0f;
    menu->item_row[2] = 0;
    y += 0.0f;

    DrawGameState(0.0f, y, menu->selected_row == menu->first_row && menu->selected_column == 1, 1);
    menu->item_width[3] = text3d_width;
    menu->item_column[3] = 1;
    menu->item_y[3] = y;
    menu->item_x[3] = 0.0f;
    menu->item_height[3] = text3d_height * 2.0f;
    menu->item_row[3] = 0;
    y = 0.0f + y;

    DrawGameState(0.5f, y, menu->selected_row == menu->first_row && menu->selected_column == 2, 2);
    menu->item_column[4] = 2;
    menu->item_y[4] = y;
    menu->item_row[4] = 0;
    menu->item_width[4] = text3d_width;
    menu->item_x[4] = 0.5f;
    menu->item_height[4] = text3d_height * 2.0f;
}

void UpdateGameMenu(GAMEPAD_s *pad, i32 a2) {
    const i32 menu_id = GetMenuID();
    if (a2 != 0 && WORLD != NULL) {
        const i32 active_menu = GetMenuID();
        if (Paused != 0 || active_menu != -1) {
            memset(buttons_store, 0, sizeof(buttons_store));
        } else {
            const u32 buttons = GamePad[0].buttons_down_08;
            u8 special_button;
            if ((GamePad[0].unknown_04 & GAMEPAD_R3) != 0) {
                if ((buttons & GAMEPAD_SPECIAL) != 0)
                    special_button = 1;
                else if ((buttons & GAMEPAD_ACTION) != 0)
                    special_button = 2;
                else if ((buttons & GAMEPAD_TAG) != 0)
                    special_button = 3;
                else
                    special_button = 0;
            } else {
                special_button = 0;
            }
            if (__builtin_expect((special_button != 0 || buttons != 0) && buttons != 0, 1)) {
                for (i32 index = 14; index > 0; --index)
                    buttons_store[index] = buttons_store[index - 1];
                buttons_store[0] = buttons;
            }
        }
    }
    MENU *menu = &GameMenu[GameMenuLevel];
    const i16 previous_menu = menu->menu;
    i32 both_pads = false;
    if ((Paused != 0 || NetPaused != 0) && pause_i_pad != -1) {
        pad = &GamePad[pause_i_pad];
    } else if (WORLD == NULL) {
        both_pads = true;
    } else if (__builtin_expect(menu_id != -1 && WORLD->current_level == TITLES_LDATA, 1)) {
        both_pads = false;
    } else if (__builtin_expect(menu_id == 0x1e && PlayerProgress[0].active != 0 && PlayerProgress[1].active != 0, 0)) {
        both_pads = true;
    } else if (Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
               Player[1]->apiobj.player_controlled) {
        both_pads = true;
    } else if (pad == &GamePad[0]) {
        if (WORLD->current_level == CREDITS_LDATA) {
            if (PlayerProgress[1].active != 0) {
                if (PlayerProgress[0].active == 0)
                    pad = &GamePad[1];
            } else if (PlayerProgress[0].active == 0) {
                goto no_player;
            }
        } else if (Player[0] != NULL && Player[1] != NULL) {
            const bool first_active =
                (Player[0]->apiobj.field_0x1f4 & 0x40000) == 0 && Player[0]->apiobj.player_controlled;
            if ((Player[1]->apiobj.field_0x1f4 & 0x40000) == 0 && Player[1]->apiobj.player_controlled) {
                if (!first_active)
                    pad = Player[1]->pad_gamepad;
            } else if (__builtin_expect(!first_active, 1)) {
                goto no_player;
            }
        } else {
        no_player:
            readpads_always = 1;
            both_pads = true;
        }
    }
    if (editor_active == 0) {
        const u8 widescreen = Game.options_save.widescreen;
        loadsaveCallEachFrame();
        Game.options_save.widescreen = widescreen;
    }
    // Pending transitions must not re-enter menu callbacks (e.g. NewGame)
    // after the save loader has selected the destination level.
    if (!(FadeSys.fade > 0.0f) && NewMode == 0 && NewLData == NULL && editor_active == 0) {
        u32 held = 0, pressed = 0, alternate_held = 0, alternate_pressed = 0;
        if (menu_id != 1 || GameTimer.time_elapsed >= 4.0f) {
            if (both_pads) {
                held = GamePad[0].unknown_04 | GamePad[1].unknown_04;
                pressed = GamePad[0].buttons_down_08 | GamePad[1].buttons_down_08;
                alternate_held = GamePad[0].unknown_0c | GamePad[1].unknown_0c;
                alternate_pressed = GamePad[0].unknown_10 | GamePad[1].unknown_10;
            } else {
                held = pad->unknown_04;
                pressed = pad->buttons_down_08;
                alternate_held = pad->unknown_0c;
                alternate_pressed = pad->unknown_10;
            }
        }
        const i32 previous_item = menu->selected_item;
        const i32 previous_column = menu->selected_item_column;
        const i32 result = UpdateMenu(held, pressed, alternate_held, alternate_pressed, FRAMETIME, GAMEPAD_MENUSELECT,
                                      GAMEPAD_MENUCANCEL, GAMEPAD_START, GAMEPAD_SELECT);
        if (MenuSFX == -1 && menu->selected_item != -1 && menu->selected_item_column != -1 && previous_column != -1 &&
            previous_item != -1 &&
            (previous_item != menu->selected_item || previous_column != menu->selected_item_column) &&
            GetMenuID() == menu_id)
            GameAudio_PlaySfx(0x2f, NULL, 0, 0);
        if (Paused != 0 && result == 1)
            ResumeGame(1, 1);
    }
    if (previous_menu != -1 && NewLData != NULL && menu_id != 0x1b && gone_through_door_to_new_level == 0)
        reset_area = 1;
}

void EndShopMenu(i32) {
    LevLock[0] = 1;
    LevTime[0] = 2.0f;
    DrawCoinTotalTime = 2.0f;
    MenuRememberCursor(&GameMenu[GameMenuLevel]);
    MenuReset();
    ReCalculateCompletionPoints();
}

static __used__ void MenuUpdateShop(MENU *menu) {
    if (shop_save_done != 0) {
        EndShopMenu(0);
        shop_save_done = 0;
        return;
    }

    if (UpdateShop(menu) == 0) {
        return;
    }

    GameAudio_PlaySfx(0x31, NULL, 0, 0);
    SHOPACTIVE = 0;
    ShopNameAlpha = 0.0f;
    shop_quit = 1;

    if (__builtin_expect(hub_forceshopsave == 0, 0)) {
        TempGame.field30_0x7c2c = Game.field30_0x7c2c;
        if (memcmp(&TempGame, &Game, sizeof(Game)) == 0) {
            NuIOS_RecordFlurryEvent(const_cast<char *>("hubshop_leavewithoutbuying"));
            if (hub_forceshopsave == 0) {
                EndShopMenu(0);
                return;
            }
        }
    }

    hub_forceshopsave = 0;
    UpdateAchievements(NULL);
    Hub_ClearStats();
    Text_FillInExtendedSaveInfo();
    if (TriggerAutoSave() != 0) {
        EndShopMenu(0);
        shop_save_done = 0;
        return;
    }

    shop_save_done = 1;
    NewMenu(1000, -1, -1);
}

void MakeMenuPacket() {
    for (i32 player_index = 0; player_index < 2; ++player_index) {
        GameObject_s *object = Player[player_index];
        if (object == NULL) {
            MenuPacket.player_model[player_index] = -1;
            MenuPacket.active_player[player_index] = 0;
        } else {
            MenuPacket.player_model[player_index] = object->id;
            MenuPacket.active_player[player_index] = (object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0;
        }
        MenuPacket.reserved_0[player_index] = 0;
        MenuPacket.reserved_1[player_index] = 0;
    }
}
