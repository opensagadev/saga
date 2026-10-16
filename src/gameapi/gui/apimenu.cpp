#include "gameapi/gui/apimenu_internal.h"

#include <stdio.h>
#include <string.h>

#include "decomp.h"
#include "globals.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"


extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" void BackupMenu(void);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" i32 NuIOS_IsWidescreen(void);
f32 GameGetMusicVolume(OPTIONSSAVE_s *options);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 volume, i32 flags);
void legoSetMusicVolume(f32 volume);
extern u8 cutskip_dontplaylevelintro;
void Hint_LoadAllGameState(void);
void NuIOS_RestoreInAppPurchases(void);
void NewGame(void);
void Hub_UpdateKit(void);
void MenuDrawNewGame(MENU *menu);
void MenuUpdateNewGame(MENU *menu);
void MenuDrawLoad(MENU *menu);
void MenuDrawLoading(MENU *menu);
void MenuDrawAutoSaveCancel(MENU *menu);
void MenuDrawAutoSaveWarning(MENU *menu);
void MenuDrawCardWarning(MENU *menu);
void MenuDrawDeleteConfirm(MENU *menu);
void MenuDrawDeleting(MENU *menu);
void MenuDrawDoNotRemoveCard(MENU *menu);
void MenuDrawFileCorrupt(MENU *menu);
void MenuDrawFormatCancel(MENU *menu);
void MenuDrawFormatConfirm(MENU *menu);
void MenuDrawFormatting(MENU *menu);
void MenuDrawInsertCard(MENU *menu);
void MenuDrawLoadCancel(MENU *menu);
void MenuDrawLoadConfirm(MENU *menu);
void MenuDrawNoData(MENU *menu);
void MenuDrawNoMemoryCard(MENU *menu);
void MenuDrawNotEnoughSpace(MENU *menu);
void MenuDrawSave(MENU *menu);
void MenuDrawSaveCancel(MENU *menu);
void MenuDrawSaveConfirm(MENU *menu);
void MenuDrawSaving(MENU *menu);
void MenuEnterAutoSaveCancel(MENU *menu);
void MenuEnterAutoSaveWarning(MENU *menu);
void MenuEnterCardWarning(MENU *menu);
void MenuEnterHeaderSave(MENU *menu);
void MenuEnterInsertCard(MENU *menu);
void MenuEnterLoad(MENU *menu);
void MenuEnterNoMemoryCard(MENU *menu);
void MenuEnterSave(MENU *menu);
void MenuEnterSaveConfirm(MENU *menu);
void MenuEnterStartNewGame(MENU *menu);
void MenuExitCardWarning(MENU *menu);
void MenuExitLoad(MENU *menu);
void MenuExitSave(MENU *menu);
void MenuUpdateAutoSaveCancel(MENU *menu);
void MenuUpdateAutoSaveWarning(MENU *menu);
void MenuUpdateCardWarning(MENU *menu);
void MenuUpdateDeleteConfirm(MENU *menu);
void MenuUpdateDeleting(MENU *menu);
void MenuUpdateDoNotRemoveCard(MENU *menu);
void MenuUpdateFileCorrupt(MENU *menu);
void MenuUpdateFormatCancel(MENU *menu);
void MenuUpdateFormatConfirm(MENU *menu);
void MenuUpdateFormatting(MENU *menu);
void MenuUpdateInsertCard(MENU *menu);
void MenuUpdateLoad(MENU *menu);
void MenuUpdateLoadCancel(MENU *menu);
void MenuUpdateLoadConfirm(MENU *menu);
void MenuUpdateLoading(MENU *menu);
void MenuUpdateNoData(MENU *menu);
void MenuUpdateNoMemoryCard(MENU *menu);
void MenuUpdateNotEnoughSpace(MENU *menu);
void MenuUpdateSave(MENU *menu);
void MenuUpdateSaveCancel(MENU *menu);
void MenuUpdateSaveConfirm(MENU *menu);
void MenuUpdateSaving(MENU *menu);
extern char *Trailer[3];
extern OPTIONSSAVE TempOptions;
extern VARIPTR characterbuffer_ptr;
extern VARIPTR characterbuffer_end;
void MusicClearAll(void);
extern "C" void SoundKillAll(void);
void GameDrawMenuEntry(MENU *menu, char *text);
void DropOut(i32 player, i32 show_effect, i32 silent, i32 force);
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);
struct CUTSCENEPLAYERCLIP;
extern i32 saveload_autosave;
extern FadeSystem FadeSys;
extern i32 level_already_loaded;
extern "C" i32 NewMode;
extern i32 from_save_and_exit;
extern i16 tCANCEL;
extern u8 MENULOSTR1, MENULOSTG1, MENULOSTB1, MENULOSTA1;
extern u8 MENULOSTR2, MENULOSTG2, MENULOSTB2, MENULOSTA2;
extern i16 tALLLEVELPROGRESSWILLBELOST, tALLSUPERSTORYPROGRESSWILLBELOST;
extern i16 tGAMEPROGRESSWILLBELOST, tEXITTOMAP, tQUITGAME, tRESTARTDEMO2;
extern i16 tARCADE_NOWIN, tEXITWITHOUTSAVING, tSAVEANDEXIT;
extern i32 SuperStory;
extern GAMEPAD_s GamePad[64];
extern i32 pause_i_pad;

extern i16 tHELPANDOPTIONS;
extern i16 tEXTRAS;
extern i16 tDROPOUT;
extern i16 tMAP;
extern i16 tRESTARTDEMO;
extern i16 tQUIT;
extern i16 tRESTARTLEVEL;
extern i16 tYES;
extern i16 tNO;
extern i16 tCANCEL;
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
extern i16 tEXIT;
extern i16 tSELECTING;
extern i16 tCONTROLLER;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
extern u8 MENUEXITR;
extern u8 MENUEXITG;
extern u8 MENUEXITB;
extern i32 reset_restart;
extern u32 ResetBits;
void Door_Reset(void);

void Arcade_DrawEndMenu(MENU *menu);
void Arcade_UpdateEndMenu(MENU *menu);
void Credits_UpdateMenu(MENU *menu);
void CustomiserMenu_Draw(MENU *menu);
void CustomiserMenu_Update(MENU *menu);
void MenuDrawClips(MENU *menu);
void MenuDrawEndChallenge(MENU *menu);
void MenuDrawEndMission(MENU *menu);
void MenuDrawEpisodes(MENU *menu);
void MenuDrawExtras(MENU *menu);
void MenuDrawFreePlay(MENU *menu);
void MenuDrawMissions(MENU *menu);
void MenuDrawOptions(MENU *menu);
void MenuDrawRestoreNewGame(MENU *menu);
void MenuDrawSelectControls(MENU *menu);
void MenuEnterOptions(MENU *menu);
void MenuExitOptions(MENU *menu);
void MenuInitClips(MENU *menu);
void MenuInitFreePlay(MENU *menu);
void MenuInitMissions(MENU *menu);
void MenuUpdateClips(MENU *menu);
void MenuUpdateEndChallenge(MENU *menu);
void MenuUpdateEndMission(MENU *menu);
void MenuUpdateEpisodes(MENU *menu);
void MenuUpdateExtras(MENU *menu);
void MenuUpdateFreePlay(MENU *menu);
void MenuUpdateMissions(MENU *menu);
void MenuUpdateOptions(MENU *menu);
void MenuUpdateRestoreNewGame(MENU *menu);
void MenuUpdateSelectControls(MENU *menu);

MENU GameMenu[10];

i32 MenuSFX = -1;
i32 MENUSFX_MENUMOVE = -1;
i32 MENUSFX_MENUBACK = -1;
i32 MENUSFX_MENUNOENTRY = -1;
i32 MENUSFX_MENUSELECT = -1;
i32 GameMenuLevel = -1;
i32 MenuValidated;
i32 MenuResult;
i32 CurrentMenuId;
i32 from_save_and_exit;
i32 pausecut_skip_to_level = -1;
u8 pausecut_skip_to_gameplay;
i32 startnewgame;
i32 startnewgame_initiated;
f32 newgamecamtime;
i32 newgamecam;
f32 MenuAlpha;
i32 MenuA;
f32 MENUTEXTSCALE = 0.5f;

extern i16 tTOUCHTOSTART;
extern i16 tNEWGAME;
extern i16 tLOADGAME;
extern i16 tCONTROLS;
extern i16 tCONTROLLERCONNECTED;
extern i16 tTOUCH;
extern i16 tCONSOLE;
extern "C" bool TestForController();
extern "C" void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment,
                            u8 red, u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box,
                            i32 suppress_draw, u32 alpha);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);

MENUFNINFO MenuInfo[TOTAL_MENUS_COUNT] = {
    {1000, MenuEnterSave, MenuDrawSave, MenuUpdateSave, MenuExitSave, -1, -1, 0},
    {1001, NULL, MenuDrawDoNotRemoveCard, MenuUpdateDoNotRemoveCard, NULL, -1, -1, 0},
    {1002, MenuEnterInsertCard, MenuDrawInsertCard, MenuUpdateInsertCard, NULL, -1, -1, 0},
    {1004, MenuEnterNoMemoryCard, MenuDrawNoMemoryCard, MenuUpdateNoMemoryCard, NULL, -1, -1, 0},
    {1003, NULL, MenuDrawNotEnoughSpace, MenuUpdateNotEnoughSpace, NULL, -1, -1, 0},
    {1005, NULL, MenuDrawNoData, MenuUpdateNoData, NULL, -1, -1, 0},
    {1006, NULL, MenuDrawFormatConfirm, MenuUpdateFormatConfirm, NULL, -1, -1, 0},
    {1007, NULL, MenuDrawFormatting, MenuUpdateFormatting, NULL, -1, -1, 0},
    {1008, MenuEnterSaveConfirm, MenuDrawSaveConfirm, MenuUpdateSaveConfirm, NULL, -1, -1, 0},
    {1009, NULL, MenuDrawSaving, MenuUpdateSaving, NULL, -1, -1, 0},
    {1010, MenuEnterAutoSaveWarning, MenuDrawAutoSaveWarning, MenuUpdateAutoSaveWarning, NULL, -1, -1, 0},
    {1011, MenuEnterAutoSaveCancel, MenuDrawAutoSaveCancel, MenuUpdateAutoSaveCancel, NULL, -1, -1, 0},
    {1012, MenuEnterLoad, MenuDrawLoad, MenuUpdateLoad, MenuExitLoad, -1, -1, 0},
    {1013, NULL, MenuDrawLoadConfirm, MenuUpdateLoadConfirm, NULL, -1, -1, 0},
    {1014, NULL, MenuDrawLoading, MenuUpdateLoading, NULL, -1, -1, 0},
    {1015, NULL, MenuDrawDeleting, MenuUpdateDeleting, NULL, -1, -1, 0},
    {1016, NULL, MenuDrawSaveCancel, MenuUpdateSaveCancel, NULL, -1, -1, 0},
    {1017, NULL, MenuDrawLoadCancel, MenuUpdateLoadCancel, NULL, -1, -1, 0},
    {1018, NULL, MenuDrawFormatCancel, MenuUpdateFormatCancel, NULL, -1, -1, 0},
    {1019, MenuEnterCardWarning, MenuDrawCardWarning, MenuUpdateCardWarning, MenuExitCardWarning, -1, -1, 0},
    {1020, NULL, MenuDrawFileCorrupt, MenuUpdateFileCorrupt, NULL, -1, -1, 0},
    {1021, MenuEnterHeaderSave, NULL, NULL, NULL, -1, -1, 0},
    {1022, MenuEnterStartNewGame, NULL, NULL, NULL, -1, -1, 0},
    {1023, NULL, MenuDrawDeleteConfirm, MenuUpdateDeleteConfirm, NULL, -1, -1, 0},
    {1024, NULL, NULL, NULL, NULL, -1, -1, 0},
};

i32 MenusUsed = RESERVED_MENUS_COUNT;
i32 MenuLanguages = 1;
char MenuHeader[64];

i32 MenuDisableHeaders;

u8 MENUHEADERR = 0;
u8 MENUHEADERG = 0x7f;
u8 MENUHEADERB = 0xff;

i32 MenuFadeEnabled;
NUMTL *MenuFadeMtl;

i32 MenuDrawDropShadows;

char *apiGameName = "Game";
char *apiGameTitle = "Game";
char *apiGameSubTitle = "";

char *apitxt_YES = "Yes";
char *apitxt_NO = "No";
char *apitxt_OK = "OK";
char *apitxt_CANCEL = "Cancel";
char *apitxt_RETRY = "Retry";
char *apitxt_SLOT = "Profile";
char *apitxt_PRESENT = "Present";
char *apitxt_EMPTY = "Empty";
char *apitxt_NOSPACE = "No Space";
char *apitxt_CONTINUE = "Continue";
char *apitxt_SAVEGAME = "Save Game";
char *apitxt_LOADGAME = "Load Game";
char *apitxt_DELETEGAME = "Delete Game";
char *apitxt_GAMESAVED = "Save Successful";
char *apitxt_GAMEOVERWRITTEN = "Overwrite Successful";
char *apitxt_GAMELOADED = "Game Loaded";
char *apitxt_DELETING = "Deleting data.";
char *apitxt_FORMATTING = "Formatting.";
char *apitxt_GAMEOVERWRITING = "Overwriting data.";
char *apitxt_CORRUPTLOAD = "Load Corrupted";
char *apitxt_CONFIRMLOAD = "Confirm Load?";
char *apitxt_CONFIRMSAVE = "Confirm Save?";
char *apitxt_CONFIRMOVERWRITE = "Are you sure you wish to overwrite this save file?";
char *apitxt_DOYOUWANTTOSTART = "Do you want to start the game?";
char *apitxt_DOYOUWANTTOABORT = "Do you want to cancel the Save?";
char *apitxt_DOYOUWANTTOABORTLOAD = "Do you want to cancel the Load?";
char *apitxt_DOYOUWANTTOABORTFORMAT = "Do you want to cancel the Format?";
char *apitxt_CONTINUEWITHOUTSAVING = "Continue without Saving";
char *apitxt_NEWSAVE = "New Save";
char *apitxt_CONTINUEWITHOUTLOADING = "Continue without loading";
char *apitxt_COMPLETED = "Completed";
char *apitxt_SELECT = "Select";
char *apitxt_BACK = "Back";
char *apitxt_VIBRATION = "Vibration";
char *apitxt_CONFIRMDELETE = "Are you sure you wish to delete this save file?";
char *apitxt_PROGRESS = "Progress: %.1f%%";
char *apitxt_TIMEPLAYED = "Time Played: %s";
char *apitxt_CONTROLLERREMOVED = "The controller in controller port %i has been removed. Please insert a controller "
                                 "into controller port %i to continue with the game.";
char *apitxt_PRESSSTART = "Press START";
char *apitxt_GAMESAVING = "Saving game...";
char *apitxt_GAMELOADING = "Loading game...";
char *apitxt_FAILEDTOLOAD = "Failed to Load";
char *apitxt_FAILEDTOSAVE = "Failed to Save";
char *apitxt_FAILEDTOOVERWRITE = "Failed to Overwrite";
char *apitxt_NODATAAVAILABLE = "No %s data present";
char *apitxt_AUTOSAVE_ENABLED = "Autosave will now use Game %i";
char *apitxt_FORMATTINGCOMPLETE = "Formatting complete";
char *apitxt_DELETECOMPLETE = "Delete complete";
char *apitxt_NOTENOUGHSPACE = "Insufficient drive space to save games.";
char *apitxt_AUTOSAVE_CANCEL1 = " ";
char *apitxt_AUTOSAVE_CANCEL2 = " ";
char *apitxt_AUTOSAVE_CANCEL3 = " ";
char *apitxt_AUTOSAVE_WARNING = " ";
char *apitxt_LOADING = " ";
char *apitxt_SAVING = " ";

f32 AUTOSAVEICONY = 0.745f;
f32 AUTOSAVEICONX = 0.775f;
f32 AUTOSAVEICONSIZE = 0.075f;
f32 ICONX = 0.775f;
f32 ICONSIZE = 0.16f;
f32 DROPINALPHA = 0.75f;
i32 shop_quit;
i32 shop_save_done;
GAMESAVE_s TempGame = {};

extern i32 hub_forceshopsave;
extern "C" i32 TriggerAutoSave(void);
void Hub_ClearStats();
void Text_FillInExtendedSaveInfo();
i32 UpdateAchievements(STATUSPACKET_s *packet);
void ReCalculateCompletionPoints();

void DrawShopPrompts();

void APIMenuDrawGameState(f32 x, f32 y, i32 highlight, i32 slot) {
    i32 red, green, blue;
    if (highlight != 0) {
        if (!TestForController()) {
            if (menu_pulse > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulse +
                                       static_cast<u32>(MENUNORMALR) * (1.0f - menu_pulse));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulse +
                                         static_cast<u32>(MENUNORMALG) * (1.0f - menu_pulse));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulse +
                                        static_cast<u32>(MENUNORMALB) * (1.0f - menu_pulse));
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }
        } else {
            if (menu_pulsate > 0.0f) {
                red = static_cast<i32>(static_cast<u32>(MENUFLASH0R) * menu_pulsate +
                                       static_cast<u32>(MENUFLASH1R) * (1.0f - menu_pulsate));
                green = static_cast<i32>(static_cast<u32>(MENUFLASH0G) * menu_pulsate +
                                         static_cast<u32>(MENUFLASH1G) * (1.0f - menu_pulsate));
                blue = static_cast<i32>(static_cast<u32>(MENUFLASH0B) * menu_pulsate +
                                        static_cast<u32>(MENUFLASH1B) * (1.0f - menu_pulsate));
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
        red = MENUFLASH1R;
        green = MENUFLASH1G;
        blue = MENUFLASH1B;
    }

    char label[32];
    sprintf(label, "%s %i", apitxt_SLOT, slot + 1);
    MenuText3DEx(label, x, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, red, green, blue,
                 static_cast<u8>(MenuA));

    if (saveload_slotused[slot] != 0) {
        MenuSmartTextEx(apitxt_PRESENT, x, y + MENUTEXTSCALE * 0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE,
                        0, MENUNORMALR, MENUNORMALG, MENUNORMALB, 0.4f, 2, NULL, 0, MenuA);
    } else if (saveload_freespace >= SAVESIZE_ADDITIONAL) {
        MenuSmartTextEx(apitxt_EMPTY, x, y + MENUTEXTSCALE * 0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                        MENUNORMALR, MENUNORMALG, MENUNORMALB, 0.4f, 2, NULL, 0, MenuA);
    } else {
        MenuSmartTextEx(apitxt_NOSPACE, x, y + MENUTEXTSCALE * 0.5f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE,
                        0, MENUWARNR, MENUWARNG, MENUWARNB, 0.4f, 2, NULL, 0, MenuA);
    }
}

void MenuLoadTechnicalStrings(char *filepath, char *language, VARIPTR *buf, VARIPTR buf_end) {
    NuStringTableSetBank(2);
    NuStringTableSetFormat(0);
    NuStringTableLoadCSV(filepath, buf, buf_end, "LABEL", language);

    apiGameName = (char *)NuStringTableGetByName("GAMENAME");
    apiGameTitle = (char *)NuStringTableGetByName("GAMETITLE");
    apiGameSubTitle = (char *)NuStringTableGetByName("GAMESUBTITLE");

    apitxt_YES = (char *)NuStringTableGetByName("GEN_YES");
    apitxt_NO = (char *)NuStringTableGetByName("GEN_NO");
    apitxt_OK = (char *)NuStringTableGetByName("GEN_OK");
    apitxt_CANCEL = (char *)NuStringTableGetByName("GEN_CANCEL");
    apitxt_RETRY = (char *)NuStringTableGetByName("GEN_RETRY");
    apitxt_SLOT = (char *)NuStringTableGetByName("GEN_SLOT");
    apitxt_PRESENT = (char *)NuStringTableGetByName("GEN_PRESENT");
    apitxt_EMPTY = (char *)NuStringTableGetByName("GEN_EMPTY");
    apitxt_NOSPACE = (char *)NuStringTableGetByName("GEN_NOSPACE");
    apitxt_CONTINUE = (char *)NuStringTableGetByName("GEN_CONTINUE");
    apitxt_SAVEGAME = (char *)NuStringTableGetByName("GEN_SAVEGAME");
    apitxt_LOADGAME = (char *)NuStringTableGetByName("GEN_LOADGAME");
    apitxt_DELETEGAME = (char *)NuStringTableGetByName("GEN_DELETEGAME");
    apitxt_GAMESAVED = (char *)NuStringTableGetByName("GEN_GAMESAVED");
    apitxt_GAMEOVERWRITTEN = (char *)NuStringTableGetByName("GEN_GAMEOVERWRITTEN");
    apitxt_GAMELOADED = (char *)NuStringTableGetByName("GEN_GAMELOADED");
    apitxt_DELETING = (char *)NuStringTableGetByName("GEN_DELETING");
    apitxt_FORMATTING = (char *)NuStringTableGetByName("GEN_FORMATTING");
    apitxt_CORRUPTLOAD = (char *)NuStringTableGetByName("GEN_CORRUPTLOAD");
    apitxt_CONFIRMLOAD = (char *)NuStringTableGetByName("GEN_CONFIRMLOAD");
    apitxt_CONFIRMSAVE = (char *)NuStringTableGetByName("GEN_CONFIRMSAVE");
    apitxt_CONFIRMOVERWRITE = (char *)NuStringTableGetByName("GEN_CONFIRMOVERWRITE");
    apitxt_DOYOUWANTTOSTART = (char *)NuStringTableGetByName("GEN_DOYOUWANTTOSTART");
    apitxt_DOYOUWANTTOABORT = (char *)NuStringTableGetByName("GEN_DOYOUWANTTOABORT");
    apitxt_DOYOUWANTTOABORTLOAD = (char *)NuStringTableGetByName("GEN_DOYOUWANTTOABORTLOAD");
    apitxt_DOYOUWANTTOABORTFORMAT = (char *)NuStringTableGetByName("GEN_DOYOUWANTTOABORTFORMAT");
    apitxt_CONTINUEWITHOUTSAVING = (char *)NuStringTableGetByName("GEN_CONTINUEWITHOUTSAVING");
    apitxt_GAMEOVERWRITING = (char *)NuStringTableGetByName("GEN_GAMEOVERWRITING");
    apitxt_NEWSAVE = (char *)NuStringTableGetByName("GEN_NEWSAVE");
    apitxt_CONTINUEWITHOUTLOADING = (char *)NuStringTableGetByName("GEN_CONTINUEWITHOUTLOADING");
    apitxt_COMPLETED = (char *)NuStringTableGetByName("GEN_COMPLETED");
    apitxt_SELECT = (char *)NuStringTableGetByName("GEN_SELECT");
    apitxt_BACK = (char *)NuStringTableGetByName("GEN_BACK");
    apitxt_VIBRATION = (char *)NuStringTableGetByName("GEN_VIBRATION");
    apitxt_CONFIRMDELETE = (char *)NuStringTableGetByName("GEN_CONFIRMDELETE");
    apitxt_PROGRESS = (char *)NuStringTableGetByName("GEN_PROGRESS");
    apitxt_TIMEPLAYED = (char *)NuStringTableGetByName("GEN_TIMEPLAYED");
    apitxt_PRESSSTART = (char *)NuStringTableGetByName("GEN_PRESSSTART");

    apitxt_GAMESAVING = (char *)NuStringTableGetByName("PC_GAMESAVING");
    apitxt_GAMELOADING = (char *)NuStringTableGetByName("PC_GAMELOADING");
    apitxt_FAILEDTOLOAD = (char *)NuStringTableGetByName("PC_FAILEDTOLOAD");
    apitxt_FAILEDTOSAVE = (char *)NuStringTableGetByName("PC_FAILEDTOSAVE");
    apitxt_FAILEDTOOVERWRITE = (char *)NuStringTableGetByName("PC_FAILEDTOOVERWRITE");
    apitxt_NODATAAVAILABLE = (char *)NuStringTableGetByName("PC_NODATAAVAILABLE");
    apitxt_AUTOSAVE_ENABLED = (char *)NuStringTableGetByName("PC_AUTOSAVE_ENABLED");
    apitxt_FORMATTINGCOMPLETE = (char *)NuStringTableGetByName("PC_FORMATTINGCOMPLETE");
    apitxt_DELETECOMPLETE = (char *)NuStringTableGetByName("PC_DELETECOMPLETE");
    apitxt_NOTENOUGHSPACE = (char *)NuStringTableGetByName("PC_NOTENOUGHSPACE");
    apitxt_CONTROLLERREMOVED = (char *)NuStringTableGetByName("PC_CONTROLLERREMOVED");
}




