// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

MENUPACKET_s MenuPacket = {};
extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern i32 MenuSFX;
extern i32 MENUSFX_MENUSELECT;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUMOVE;
extern char *apitxt_LOADGAME;
extern char *apitxt_SAVEGAME;
extern char *apitxt_CONFIRMSAVE;
extern char *apitxt_CONFIRMOVERWRITE;
extern char *apitxt_CONFIRMLOAD;
extern char *apitxt_DOYOUWANTTOABORT;
extern char *apitxt_DOYOUWANTTOABORTLOAD;
extern char *apitxt_DOYOUWANTTOABORTFORMAT;
extern char *apitxt_CONFIRMDELETE;
extern char *apitxt_RETRY;
extern char *apitxt_SLOT;
extern char *apitxt_NEWSAVE;
extern char *apitxt_CANCEL;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_NOTENOUGHSPACE;
extern char *apitxt_DELETEGAME;
extern char *apitxt_DELETING;
extern char *apitxt_DELETECOMPLETE;
extern char *apitxt_FORMATTING;
extern char *apitxt_FORMATTINGCOMPLETE;
extern char *apiGameName;
extern char *apitxt_YES;
extern char *apitxt_NO;
extern char **TTab;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tCONTROLLERCONNECTED;
extern i16 tCONTROLS;
extern i16 tCONSOLE;
extern i16 tTOUCH;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tMUSIC;
extern i16 tWIDESCREEN;
extern i16 tON;
extern i16 tOFF;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tCANCEL;
extern i16 tSTORY;
extern i16 tFREEPLAY;
extern i16 tREPLAYSTORY;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern i16 tHOWTOPLAY;
extern "C" OPTIONSSAVE TempOptions;
extern i32 GAMEDEMO;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern f32 minikittime;
extern "C" TIMER BonusTimer;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
void GameDrawMenuEntry(MENU *menu, char *text);
i32 GetParentMenuID(void);
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void SfxCheckMusicOnOff(OPTIONSSAVE *options);
void NewGame(void);
void NuIOS_RestoreInAppPurchases(void);
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 sfx_wait;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
void Draw_NODATAAVAILABLE();
void Draw_NOMEMORYCARD();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);
void Draw_OK(MENU_s *menu);

i32 memcard_cardchanged;
i32 MenuASCancelFinished;
i32 MenuCardWarningState;
i32 ButtonScaleMode;
i32 Menu_InLoadFlow;
i32 Menu_InWarningFlow;
i32 Menu_LastFlow;
i32 MenuLoadStarted;
i32 memcard_slot;
i32 Menu_InSaveFlow;
i32 Menu_DisableCancel;
i32 Menu_InHeaderSave;
i32 Menu_Overwriteflag;
i32 memcard_slotsused;
i32 memcard_saveneeded;
i32 memcard_savestarted;
i32 memcard_savefailed;
i32 memcard_loadneeded;
i32 memcard_loadstarted;
i32 memcard_loadfailed;
i32 memcard_loadcorrupt;
i32 memcard_deleteneeded;
i32 memcard_deletestarted;
i32 memcard_deletefailed;
f32 memcard_deletemessage_delay;
f32 memcard_deleteresult_delay;
i32 memcard_formatting;
i32 memcard_formatme;
i32 memcard_formatfailed;
f32 memcard_formatmessage_delay;
f32 memcard_formatresult_delay;
f32 memcard_savemessage_delay;
f32 memcard_saveresult_delay;
f32 memcard_loadmessage_delay;
f32 memcard_loadresult_delay;
i32 SAVESIZE = 50;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
static i32 header_r;
static i32 header_g;
static i32 header_b;
// Retail clears these "fast repeat" latches but never sets them.
static i32 fastup_lock;
static i32 fastdown_lock;
static i32 fastleft_lock;
static i32 fastright_lock;
void (*drawslotinfofn)(f32, f32, i32, i32) = APIMenuDrawGameState;
void (*drawslotsfn)(MENU *, f32) = APIMenuDrawMemCardSlots;
static i32 lastslot;
static i32 slideleft;
static i32 slideright;

char *extrasavetext;
f32 extrasavealpha = 1.0f;
u8 RAP_WARNING_B = 0xff;
u8 RAP_WARNING_G;
u8 RAP_WARNING_R = 0xff;

static i32 g_enableButtonPrompts = 1;

extern i32 hub_new_level;
extern i32 hub_selectmode;
extern f32 selectmodetime;
extern i32 selectmodemode;
extern "C" i32 FinishLoop_On;
extern FadeSystem FadeSys;
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);
void Hub_UpdateFreePlaySelect();
void WipeBackToHub();
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);



static inline i32 MenuSlotColourLerp(u32 first, u32 second, f32 amount) {
    return static_cast<i32>(first * amount + second * (1.0f - amount));
}

void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void MenuDrawLoad(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);


void MenuExitLoad(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuDrawClips(MENU_s *) {
}

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

void MenuInitClips(MENU_s *) {
}

void MenuStartLoad();

void MenuStartSave();




void MenuDrawNoData(MENU_s *menu);

void MenuDrawSaving(MENU_s *);

void MenuUpdateLoad(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);



static NUGSCN **IconScene;
static char IconPath[0x40] = "stuff\\icons\\";

void IconScenes_Dump() {
    if (IconScene == NULL) {
        return;
    }

    for (i32 i = 0; i < CHARCOUNT; ++i) {
        if (IconScene[i] != NULL) {
            NuGScnRemove(IconScene[i]);
            IconScene[i] = NULL;
        }
    }
}

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *) {
    const usize table_size = static_cast<usize>(CHARCOUNT) * sizeof(*IconScene);
    IconScene = reinterpret_cast<NUGSCN **>(ALIGN(buf->addr, 4));
    buf->addr = reinterpret_cast<usize>(IconScene) + table_size;
    memset(IconScene, 0, table_size);

    if (path != NULL && NuStrLen(path) <= 0x3f) {
        NuStrCpy(IconPath, path);
    }
}

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end) {
    if (list == NULL || IconScene == NULL) {
        return;
    }

    for (; list->model_id != -1; ++list) {
        const i32 character_id = list->model_id;
        if (IconScene[character_id] != NULL || list->count == 0) {
            continue;
        }

        const i32 object_index = CDataList[character_id].field20_0x42;
        if (object_index == -1) {
            continue;
        }

        char path[0x100];
        NuStrCpy(path, IconPath);
        NuStrCat(path, LevelObject_FindNameFromIndex(object_index));
        NuStrCat(path, ".gsc");
        IconScene[character_id] = NuGScnRead(buf, *buf_end, path);
        if (IconScene[character_id] != NULL && IconScene[character_id]->display_list != NULL) {
            IconScene[character_id]->display_list->flags |= NU_DISPLAYSCENE_FLAG_NEEDS_BUILD;
        }
    }
}

void MenuDrawLoading(MENU_s *);




i32 MenuIsAvailable();

void MenuUpdateClips(MENU_s *) {
}

void MenuDrawDeleting(MENU_s *);

void MenuDrawFreePlay(MENU_s *) {
    if (MenuStopDraw == 0) {
        Hub_DrawFreePlaySelect();
    }
}



void MenuInitFreePlay(MENU_s *menu) {
    menu->first_row = 0;
    menu->last_row = 2;
    menu->selected_row = 0;
    menu->selected_item = 0;
}



void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateSaving(MENU_s *);



NUGSCN *IconScene_FindById(i32 character_id) {
    if (character_id < 0 || character_id >= CHARCOUNT) {
        return NULL;
    }
    return IconScene[character_id];
}


void MenuDrawFormatting(MENU_s *);

void MenuDrawInsertCard(MENU_s *);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFreePlay(MENU_s *) {
    Hub_UpdateFreePlaySelect();
}


void MenuDrawCardWarning(MENU_s *);

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

void MenuEnterInsertCard(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawEndChallenge(MENU_s *) {
    if (MenuStopDraw != 0)
        return;
    if (ChallengeMode != 2) {
        SmartText(TTab[tOUTOFTIME], 0.0f, STATSPOSY, 1.0f, 1.0f, 1.0f, 1.0f, 0,
                  191 + (static_cast<u32>(menu_flash) < 1 ? 64 : 0), 31 + (static_cast<u32>(menu_flash) < 1 ? 32 : 0),
                  0, 1.7f, 1);
    } else if (NuFmod(GameTimer.time_elapsed, 0.3f) < 0.2f) {
        char text[64];
        f32 remaining = static_cast<f32>(static_cast<i32>(ADataList[WORLD->level_sub_id].challenge_trial_time)) -
                        ChallengeTimer.time_elapsed;
        if (remaining < 0.0f)
            remaining = 0.0f;
        Text_MakeTime(remaining, 0, 1, 1, text);
        Text3D(text, 0.0f, STATSPOSY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0);
    }
    DrawMiniKitCount(minikittime, MiniKitScale, AreaGlobals.values.field_0x20, 10);
}

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuEnterSaveConfirm(MENU_s *);


void MenuUpdateFormatting(MENU_s *);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

void MenuDrawFormatConfirm(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterStartNewGame(MENU_s *);

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawNotEnoughSpace(MENU_s *);



void MenuUpdateEndChallenge(MENU_s *menu) {
    if (menu->menu_time >= 5.0f) {
        CompleteLevel(WORLD);
    } else if (menu->menu_time >= 1.5f && BonusWinFlag == 0) {
        PlaySfx(const_cast<char *>("Victory"), NULL);
        BonusWinFlag = 1;
    }
}

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawDoNotRemoveCard(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuEnterAutoSaveWarning(MENU_s *);

extern f32 memcard_autosavecanceldelay;
void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);



void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

extern "C" {

    f32 MENUDY = -0.1066400036f;
    f32 MENUBOTY = -0.75f;
    f32 MENUTOPY = 0.8f;
    f32 dme_sx = 1.0f;
    f32 dme_sy = 1.0f;
    f32 MENUENTRYEXWIDTH = 1.6f;
    u8 MENUFLASH1R = 0x7f;
    u8 MENUFLASH1G;
    u8 MENUFLASH1B = 0xff;
    u8 MENUFLASH0R;
    u8 MENUFLASH0G = 0xff;
    u8 MENUFLASH0B = 0xff;
    u8 MENUWARNR = 0xff;
    u8 MENUWARNG;
    u8 MENUWARNB;
    u8 MENUENTRYR;
    u8 MENUENTRYG = 0x7f;
    u8 MENUENTRYB = 0xff;
    u8 MENUNORMALR = 0xff;
    u8 MENUNORMALG = 0x7f;
    u8 MENUNORMALB;
    u8 dme_r;
    u8 dme_g;
    u8 dme_b;
    i32 dme_align;
    i32 dme_rgb;
    f32 menu_pulse;
    f32 menu_pulse_speed;
    i32 menu_pulse_angle;
    f32 menu_pulsate;
    f32 menu_pulsate_speed;
    i32 menu_pulsate_angle;
    void (*headerdrawfn)(void) = DrawMenuHeader;
    void (*predrawfn)(MENU *) = NULL;

    static u8 MenuColourLerp(u8 first, u8 second, f32 amount) {
        return static_cast<u8>(
            static_cast<i32>(static_cast<f32>(first) * amount + static_cast<f32>(second) * (1.0f - amount)));
    }

void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

    void RemapAddr(void *new_base, void *old_base, void **address) {
        *address = static_cast<u8 *>(new_base) + (static_cast<u8 *>(*address) - static_cast<u8 *>(old_base));
    }

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

} // extern "C"

// ==== src/legoapi/menus/screens/menus.cpp: declarations ====
#include "decomp.h"
#include "batman.h"
#include "gamelib/crc/crc.h"
#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/render/fx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);
extern "C" void loadsaveCallEachFrame(void);
extern "C" bool TestForController(void);
i32 GetMenuID(void);
void MenuDrawBackground(void);
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_TOGGLERIGHT;
extern "C" GAMEPAD_s GamePad[64];
extern i32 readpads_always;
extern "C" i32 reset_area;
extern i32 MenuSFX;

eduimenu_s *GetMenuActiveChild(eduimenu_s *menu);
void ResizePauseScreenTexture(i32, i32);

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3);

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODESUBTITLEY;
extern f32 HUB_EPISODETITLEY;
extern i32 draw_para;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *apiGameName;
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_OK;
extern char *apitxt_GAMELOADED;
extern char *apitxt_GAMELOADING;
extern char *apitxt_FAILEDTOLOAD;
extern char *apitxt_CORRUPTLOAD;
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern i32 menu_flash;
extern f32 text3d_height;
extern f32 text3d_width;
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
extern i32 Paused;
extern "C" i32 screendump;
extern f32 pause_fade;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
extern i32 RemoveDirectionalMaps;
extern i32 RemoveNormalMaps;

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

extern i32 solid_cable;
extern i32 nsegments_drawn;
extern f32 slack_factor;
extern f32 cable_slack;
extern f32 nsegments_per_unit;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

void DrawCables();

void DrawRipple(ripple_node_s *node);

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);

extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);

void Draw_LOADED();

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);

void DrawParallax(nuhspecial_s *special);

void DrawQuestion(nuvec_s *position, float scale_value, float y_push);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);


void Draw_LOADING();

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters);

void DrawArrow_Now(_vum_s *, float, i32, i32);

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha);

void DrawCross_Now(_vuv_s *, float, i32, i32);

void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);



void ClearScreen();



void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters);

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour);

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

void DrawItemMenu2D();

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment);

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

extern nuhspecial_s green_light;
extern nuhspecial_s painttargetcolour[3];
extern nuhspecial_s paintlights[3];

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void Draw_LOADFAILED();

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);

void DrawCameraTarget(nuvec_s *);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

void DrawStatusBG_LSW(STATUSPACKET_s *status);

void Draw_LOADCORRUPT();

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

void DrawCameraTarget2(nuvec_s *);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];
extern i32 currentminikit;
extern f32 slideseek;

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D();

void DrawSubItemMenu3D();

void Draw_NOMEMORYCARD();

void DrawFadeScreenWipe();


void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *);

void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

void Draw_NODATAAVAILABLE();

extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
extern u8 CharClipToBlobShadows;


i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

extern void DrawMenuEntry(MENU_s *, char *);
void Draw_OK(MENU_s *menu);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour);

void DrawArrow(nuhspecial_s *special, float scale_value);

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);





void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);


void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== src/gameapi/gui/apimenu.cpp: all remaining content ====
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
extern "C" OPTIONSSAVE TempOptions;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
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
extern "C" i32 SuperStory;
extern "C" GAMEPAD_s GamePad[64];
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

void APIMenuDrawGameState(f32 x, f32 y, i32 highlight, i32 slot);

void MenuLoadTechnicalStrings(char *filepath, char *language, VARIPTR *buf, VARIPTR buf_end);





// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/numusic/numusic.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/nupad.h"

#include <string.h>

extern "C" void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32);
extern "C" void BackupMenu(void);
extern GAMECAMERA_s *GameCam;
extern WORLDINFO_s *WORLD;
extern i32 (*GamePads_IgnoreInputFn)(void);
extern NUPAD *pActivePad;
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
extern GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.
extern i32 readpads_always;

extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_TOGGLERIGHT;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_R3;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.
extern f32 PadOldSpeed2[2];
extern f32 PadOldSpeed[2];
extern u16 PadOldAngle2[2];
extern u16 PadOldAngle[2];


void GamePads_Init();

f32 GamePad_Rotate(GameObject_s *object);

i32 GamePad_Waggle(GAMEPAD_s *pad);

GAMEPAD_s *GamePad_Allocate();

void GamePads_NetHost();

void GamePads_NetReset(i32);

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);

void GamePads_NetClient();

i32 GamePads_SkipMovie();

extern "C" {

bool TestForController(void);

} // extern "C"

void PadOutPause(i32 port, WORLDINFO_s *world);

void ResetRumble(RUMBLEPACKET *packet);

void UpdateRumble(RUMBLEPACKET *packet);

void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

void TakeHitRumble(GameObject_s *object, float strength);

extern f32 SpaceRumbleTimer;
extern "C" f32 NuFsqrt(f32);
extern "C" f32 NuRandFloat();
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern f32 MulDist;
extern f32 SpeedAdd;
extern f32 SpeedDist;
extern f32 MulAdd;

extern "C" void SetPreSeekStartPoint(f32 start_point);

void SpaceResetAudioPoint();

void SpaceRumbleProcess();

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32);

i32 ObjLookingWithLeftStick(GameObject_s *object);

void PerformPauseButtonStuff();

void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);

void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

i32 NoPad(i32 port, i32 require_game_input);

void NewBuzz(nupad_s *pad, float duration, i32);

i32 ReadPad(i32 port);

void ReadPads();

void NewRumble(nupad_s *pad, float strength, i32);

// ==== src/gameframework/savesystem.cpp: all remaining content ====
#include <string.h>

#include "decomp.h"

#include "gameframework/saveload.h"

#include "nu2api/nucore/numemory.h"

typedef i16 (*hashfn_t)(void);

i32 SAVESLOTS = 3;

void *memcard_savedata = NULL;
i32 memcard_savedatasize = 0;
void *memcard_savedatabuffer = NULL;

void *memcard_extra_savedata = NULL;
i32 memcard_extra_savedatasize = 0;
void *memcard_extra_savedatabuffer = NULL;

hashfn_t memcard_hashfn = NULL;
void (*memcard_drawasiconfn)(void) = NULL;
i32 memcard_autosave = 0;
i32 memcard_autosaveneeded = 0;
i32 memcard_autosavestarted = 0;
i32 memcard_autosaveinprogress = 0;
f32 memcard_autosavepredelay = 0.0f;
f32 memcard_autosavepostdelay = 0.0f;

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize);

i32 ChecksumSaveData(void *buffer, i32 size);

bool TriggerExtraDataSave(void);

i32 TriggerExtraDataLoad(void);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 memcard_cardchanged;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
extern i32 numfilteroutblocks;
extern char filteroutblocks[8][12];
extern i32 numfilterblocks;
extern char filterblocks[8][96];

extern char FS_FileList[65536];
extern char FS_LastFileName[64];
extern char FS_Path[256];
extern i32 FS_NumFiles;
extern i32 FS_CurrentPosFileNum;
extern i32 FS_CursorLine;
extern char FS_ParentEntry[8];
extern i32 FS_SortMode;
extern char * FS_CurrentCursorPos;
extern char * FS_CurrentPos;
extern char * FS_FileListEnd;
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

f32 memcard_autosavecanceldelay = 1.5f;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
f32 memcard_createmessage_delay;
f32 memcard_createresult_delay;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
f32 memcard_message_delay;
f32 memcard_result_delay;
i32 memcard_justformatted;
i32 saveload_autosavedisabled;

void InitMemCard();

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32);

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32);

i32 FS_PrevNameLen(char *name);


void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

extern char * SortStrings[4];

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status();

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32);

void SerialiseStringAddr(EdStream &stream, void *data, i32 count);

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
    void (*savesuccessfn)(void);
    void *memcard_headerdata;
    i32 memcard_headerdatasize;
    void *memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/menus/core/text.cpp: declarations ====
#include "decomp.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/text.h"
extern char * ASCII_UP;
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nushader_plain.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include <stdio.h>
#include <string.h>
i32 FindNextBreak(unsigned char *, i32);
i32 FindNearestBreak(unsigned char *, i32);
extern char **TTab;
extern i16 tNULL;
extern i16 tUNKNOWN;
extern i16 tENGLISH;
extern i16 tFRENCH;
extern i16 tGERMAN;
extern i16 tITALIAN;
extern i16 tSPANISH;
extern i16 tDANISH;
extern i16 tPLAYER1, tPLAYER2, tIOSAUTOSAVEWARNING, tLOADING, tSAVING;
extern char *apitxt_AUTOSAVE_WARNING, *apitxt_LOADING, *apitxt_SAVING;
extern char * txt_NULL;
extern char * apitxt_ENGLISH;
extern char * apitxt_FRENCH;
extern char * apitxt_GERMAN;
extern char * apitxt_ITALIAN;
extern char * apitxt_SPANISH;
extern char * apitxt_DANISH;

// Original .data @0x667ce0; the names are filled in by the static initializer.
extern LANGUAGEDATA * Text_LanguageList;
f32 text3d_height;
f32 text3d_width;
void (*buttonmapfn)(char *, char *);
f32 (*pulsetimerfn)(f32);
extern "C" {
    i32 smarttextex_drawmessagebox = 0;
    void NuQFntSetJustifiedTolerances(float squash, float stretch);
    unsigned char *NuUnicodeCharFromUTF8(u16 *character, unsigned char *text);
    unsigned char *NuUTF8CharFromUnicode(unsigned char *text, u16 character);
    VUFNT *LoadGameFont(char *, char *, variptr_u *, variptr_u *, i32);
    VUFNT *LoadButtonFont(char *, char *, variptr_u *, variptr_u *, i32);
    void NuLanguageSet(i32 language);
    void NuQFntDestroy(VUFNT *font);
    i32 NuRndrBeginScene(i32 flags);
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
}
extern void (*Text_GameSetLanguageFn)(i32);
char *Text_GetLanguagePath(i32 language);
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
void IntroText_SetTextID(i32 id);
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
void Text_DecodeButtons(char *source, char *destination);
void GameDrawMenuEntry(MENU *menu, char *text);
extern "C" void BackupMenu(void);
extern "C" {
    extern u8 MENUFLASH1R;
    extern u8 MENUFLASH1G;
    extern u8 MENUFLASH1B;
    extern u8 MENUFLASH0R;
    extern u8 MENUFLASH0G;
    extern u8 MENUFLASH0B;
    extern u8 MENUENTRYR;
    extern u8 MENUENTRYG;
    extern u8 MENUENTRYB;
    extern u8 MENUNORMALR;
    extern u8 MENUNORMALG;
    extern u8 MENUNORMALB;
}
void Text_LoadFont(char *path, variptr_u *buf, variptr_u *buf_end);
#include "nu2api/numath/numtx.h"
extern "C" {
    void NuMtxSetRotationX(NUMTX *mtx, i32 angle);
    void NuMtxRotateY(NUMTX *mtx, i32 angle);
    void NuMtxRotateZ(NUMTX *mtx, i32 angle);
    void NuMtxTranslate(NUMTX *mtx, NUVEC *vec);
}
void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);
void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);
i32 SplitTextFindNextWS(unsigned char *text, i32 position);
extern "C" void TextDecode(char *source, unsigned char *dest);
extern "C" void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, i32 alpha);
extern "C" void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                          u8 green, u8 blue, i32 alpha);
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text);

extern f32 APITEXTSCALEX;
extern f32 APITEXTSCALEY;
extern i32 textcrawlactive;
extern i32 Arcade_TextCrawlID;
extern i32 Arcade_TextCrawlParagraphs;
extern i32 g_bgloadAsFastAsPossibleHack;

void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk);

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text);
f32 TextPulseTimer(f32 delay);

void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last);
void Text_MakeScore(u32 score, char *text);
extern i16 tALONGTIMEAGO;
void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end);
void Text_SetLanguage(i32 language);
void *Text_IsFontLoaded();
void TextDecodeCodeword(char *source, char *destination);
static f32 QFONTSCALEX = 1.0f;
static f32 QFONTSCALEY = 1.0f;
static f32 STCOORDSCALE = 1.0f;
f32 g_buttonFontScalePulse = 1.0f;
f32 smarttextex_longestwidth;
extern i32 MenuStopDraw;
i32 smarttext_fwn;
static i32 followon_line;
static u32 current_rgba = 0x80808080;
i32 apitext_half_rgb;
u8 text_preset_b[10] = {128, 0, 0, 128, 128, 128, 0, 0, 0, 0};
u8 text_preset_g[10] = {128, 0, 128, 0, 128, 0, 128, 96, 0, 0};
u8 text_preset_r[10] = {128, 128, 0, 0, 0, 128, 128, 128, 0, 0};
extern i32 ButtonScaleMode;

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font);
void Text_DecodeButtons(char *source, char *destination);
extern char * txtpath_JAPANESE;
extern char * txtpath_DANISH;
extern char * txtpath_SPANISH;
extern char * txtpath_ITALIAN;
extern char * txtpath_GERMAN;
extern char * txtpath_FRENCH;
extern char * txtpath_ENGLISH;

char *Text_GetLanguagePath(i32 language);
void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *);
void Text_InitLanguageList(LANGUAGEDATA *language_list);
void Text_SetLanguage_Game(i32 language);
void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);
void Text_InitDefaultStrings();
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
i32 Text_GetMaxOverallStrings();
void Text_LocaliseDecimalPoint(char *text);
void Text_ExpandAllButtonStrings(char *input, char *output);
void Text_FillInExtendedSaveInfo();
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
NUMTL *MessageMtl;
i32 MessageMtlInit;

extern "C" void MessageBoxInitMtl(void);

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale);

void DrawMessageBox(i32, f32 x, f32 y, f32 width, f32 height);

extern "C" {
void FixUpButtonsInFont(VUFNT *game_font, VUFNT *button_font);

VUFNT *LoadButtonFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);

VUFNT *LoadGameFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane);
void MatrixText(VUFNT *font, unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, u32 alignment,
                    u32 colour, NUMTX *matrix, i32 camera_relative);
void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha);
void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, u8 alpha);
void Set3DGameFont(VUFNT *font);
void SetGameFont(VUFNT *font);
void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines);
void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha);
void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha);
void SmartTextExDrop(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u32 red,
                         u32 green, u32 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         u32 alpha);
void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale);
void SmartTextSetFWNMode(i32 mode);
void SmartTextSetFont(VUFNT *font);
void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale);
i32 SplitText(unsigned char *text, f32 max_width);
void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue);
void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha);
void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                   u8 green, u8 blue, i32 alpha);
void TextDecode(char *source, unsigned char *dest);
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
void UnloadGameFont(void);
}
void MenuUpdateViewTextStrings(MENU_s *menu);


void MenuDrawViewTextStrings(MENU_s *menu);

abi_ulong GetMatchLength(unsigned char *first, unsigned char *second, abi_ulong maximum);
i32 SplitTextFindNextWS(unsigned char *text, i32 position);
void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);
extern "C" void Text3DStringEncode(char *src, u16 *dst);
extern "C" void SetQFont2D(void);
void IntroText_SetTextID(i32 id);
void IntroText_Draw(float alpha);

i32 GetMenuID(void);

i32 GetParentMenuID();

// ==== src/legoapi/items/objects/objectsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/nuspecial.h"
#include "decomp.h"
#include "legoapi/actions/movement/jumping.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern f32 BUCK_RIDERJUMPCLEARANCE;
extern void (*BuckStartExtraFn)(GameObject_s *);
void NewRumble(nupad_s *, f32, i32);

void Buck_Start(GameObject_s *object, GameObject_s *rider);

extern i16 id_SNOWMOB;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

i32 DoBuckStart(GameObject_s *object);

pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support);

extern PART_s *boulder_part[2];
extern i32 boulder_blowup_type;
extern NUVEC boulder_oldpos[2];
extern "C" void KillPart(PART_s *, i32);

void Boulder_Kill(PART_s *part, i32);

void Boulder_Move(PART_s *part, float);

void Buck_MoveCode(GameObject_s *object, i32 start);

i32 FindNextBreak(unsigned char *text, i32 index);

static i32 IsTextBreak(unsigned char *text, i32 index) {
    const u8 ch = text[index];
    if (ch == ' ') {
        const u8 next = text[index + 1];
        if (next == '?' || next == '!' || next == ';' || next == ':') {
            return 0;
        }
        return 1;
    }
    if (ch == '.') {
        if (text[index + 1] == '.') {
            return 0;
        }
        return 1;
    }
    if (ch == ',' || ch == '-') {
        return 1;
    }
    return 0;
}

i32 FindNearestBreak(unsigned char *text, i32 index);

void BuckStartExtra_LSW(GameObject_s *object);

void Buck_StartRiderJump(GameObject_s *rider, GameObject_s *mount);

void SetEffectVisibility(char *name, i32 visible);

i32 Conveyor_AdjustSpeed(NUVEC *velocity);

void LevelObjects_InitForLevel(WORLDINFO_s *world);

void EquivalentObjects_Configure(WORLDINFO_s *world, char *config);

// ==== src/gameapi/edtools/edpartall.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"

#include <stdio.h>
#include <string.h>

void edpartDoInput(nupad_s *pad);
void edpartDetermineNearest(f32 distance);
void edpartDrawCursor();
void edpartHighlightNearest();
void edpartScaleType(i32 type, f32 scale);
void edpartInitType(i32 type);
void edpartDestroy(i32 index);
void edpartPlace(i32 index, nuvec_s *position);
void edpartCreate(nuvec_s *position, i32 type);
void edpartMultipleCopyCopy();
void edpartMultipleCopyClear();
i32 edpartSaveEffects(char *path, char page);

extern "C" {
extern i32 edpart_set_part;
extern i32 edpart_filter;
extern char edpart_filter_string[16];
extern i32 edpart_first_time_this_level;
extern i32 edpart_curr;
extern i32 edpart_create_type;
extern i32 edpart_nearest_orphans;
extern i32 edpart_nearest_duplicates;
extern i8 edpart_effect_list;
extern i32 edpart_emitrotx;
extern i32 edpart_emitroty;
extern i32 edpart_emitrotz;
extern i32 edpart_roty;
extern i32 edpart_rotz;
extern f32 edpart_offset;
extern i32 edpart_dpad_mode;
extern i32 edpart_copy_mode;
extern i32 edpart_copy_enclosed;
extern i32 edpart_copy_source[8];
extern i32 edpart_copy_source_count;
extern NUVEC edpart_copy_source_vec;
extern i32 edpart_copyrotz;
extern f32 edpart_copy_size;
extern i32 edpart_copyroty;
extern i32 edpart_snap_enabled;
extern i32 edpart_refroty;
extern i32 edpart_refrotz;
extern i32 edpart_cam_ax;
extern i32 edpart_cam_ay;
    extern NUVEC edpart_cam_pos;
extern i32 edpart_num_orphans;
extern i32 edpart_readout;
extern f32 edpart_scale_factor;
extern NUMTL * edpart_mtl;
extern NUMTL * edpart_boxmtl;
    extern i32 edpart_nearest;
    extern part_emit_s *edpart_nearest_emit;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern i32 part_platimpactcnt;
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern char edbits_general_save_directory[256], edbits_general_save_name[256], edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256], edbits_level_save_name[256], edbits_level_save_extension[256];
    extern i32 edbits_part_general_page, edbits_part_level_page;
    extern i32 edbits_override_backups;
    void ResetParts(void);
    extern eduimenu_s *edpart_active_menu;
    extern eduimenu_s *edpart_opt_menu;
    extern eduimenu_s *edpart_type_menu;
    void eduiMenuRender(eduimenu_s *menu);
extern eduimenu_s * edpart_active_menu;
extern eduimenu_s * edpart_opt_menu;
extern eduimenu_s * edpart_sscale_menu;
extern eduimenu_s * edpart_message_menu;
extern eduimenu_s * edpart_name_menu;
extern eduimenu_s * edpart_data_menu;
extern eduimenu_s * edpart_scaletype_menu;
extern eduimenu_s * edpart_switchtype_menu;
extern eduimenu_s * edpart_switch_menu;
extern eduimenu_s * edpart_soundcontrol_menu;
extern eduimenu_s * edpart_soundid_menu;
extern eduimenu_s * edpart_soundx_menu;
extern eduimenu_s * edpart_sounds_menu;
extern eduimenu_s * edpart_debrisscale_menu;
extern eduimenu_s * edpart_partindex_menu;
extern eduimenu_s * edpart_impactpart_menu;
extern eduimenu_s * edpart_debrisindex_menu;
extern eduimenu_s * edpart_diedebris_menu;
extern eduimenu_s * edpart_impactdebris_menu;
extern eduimenu_s * edpart_emitterdebris_menu;
extern eduimenu_s * edpart_trail2debris_menu;
extern eduimenu_s * edpart_trail1debris_menu;
extern eduimenu_s * edpart_debrissettings_menu;
extern eduimenu_s * edpart_cutoff_menu;
extern eduimenu_s * edpart_emittime_menu;
extern eduimenu_s * edpart_changegenrate_menu;
extern eduimenu_s * edpart_varemit_menu;
extern eduimenu_s * edpart_varstart_menu;
extern eduimenu_s * edpart_grav_menu;
extern eduimenu_s * edpart_emitvel_menu;
extern eduimenu_s * edpart_emit_menu;
extern eduimenu_s * edpart_instancescale_menu;
extern eduimenu_s * edpart_tint_menu;
extern eduimenu_s * edpart_instanceflags_menu;
extern eduimenu_s * edpart_instorient_menu;
extern eduimenu_s * edpart_maxlife_menu;
extern eduimenu_s * edpart_instancesettings_menu;
extern eduimenu_s * edpart_instanceorphans_menu;
extern eduimenu_s * edpart_thingsinstance_menu;
extern eduimenu_s * edpart_worldinstance_menu;
extern eduimenu_s * edpart_instance_menu;
extern eduimenu_s * edpart_leveltype_menu;
extern eduimenu_s * edpart_generaltype_menu;
extern eduimenu_s * edpart_type_menu;
}




extern "C" {
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern eduimenu_s *edpart_sscale_menu;
    extern eduimenu_s *edpart_cutoff_menu;
    extern eduimenu_s *edpart_emittime_menu;
    extern eduimenu_s *edpart_scaletype_menu;
    extern eduimenu_s *edpart_debrisscale_menu;
    extern eduimenu_s *edpart_instancescale_menu;
    extern eduimenu_s *edpart_changegenrate_menu;
    extern eduimenu_s *edpart_name_menu;
    extern i32 edpart_nearest;
    extern i32 edpart_create_type;
    extern f32 edpart_scale_factor;
    extern part_typedesc_s part_types[128];
}

static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);
static void edpartSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSoundControlMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);
static void edpartSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsGeneral(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffectsLevel(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddGeneralType(eduimenu_s *, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *);
static void edpartGeneralPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelPartIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartGeneralDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartLevelDebrisIndexMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *, u32);
static void edpartWorldInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartThingsInstanceMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartInstanceOrphansMenu(eduimenu_s *, eduiitem_s *, u32);
static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void edpartDeleteInstanceOrphan(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edpartDeleteAllInstanceDuplicates(eduimenu_s *, eduiitem_s *, u32);

extern "C" {
extern i32 edpart_which_scene;
extern i32 edpart_set_debris;
extern i32 edpart_part_list;
extern i32 edpart_particle_list;
}


static void edpartInit();
static i32 edpartProc(float delta_time, nupad_s *pad);
static void edpartApply();
static void edpartClose();
static void edpartEnter();
static void edpartRender();
static void edpartSelType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartCopyType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDataMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartGravMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartMoveList(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTintMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeGrav(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeName(eduimenu_s *, eduiitem_s *item, u32);
static void edpartCutOffMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartDeleteType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundXMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartSoundsMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSwitchMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeTintB(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintG(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeTintR(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSoundIDMenu(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartVarEmitMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartAddLevelType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeBounce(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeSScale(eduimenu_s *, eduiitem_s *item, u32);
static void edpartEmitTimeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartInstanceMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartToggleFilter(eduimenu_s *, eduiitem_s *item, u32);
static void edpartVarStartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartCancelOptMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeMaxLife(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangeVarEmit(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDieDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartLevelTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartScaleTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartAddGeneralType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartApplyScaleType(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTintMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void edpartChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeVarStart(eduimenu_s *, eduiitem_s *item, u32);
static void edpartImpactPartMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSwitchTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void edpartChangePartIndex(eduimenu_s *, eduiitem_s *item, u32);
static void edpartDebrisScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);


static void edpartFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);
static void edpartGeneralTypeMenu(eduimenu_s *menu, eduiitem_s *, u32);
static void edpartSetDistribution(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetInstanceType(eduimenu_s *, eduiitem_s *item, u32);
static void edpartSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);
static void edpartCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void edpartCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeFilterName(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeRanMaxLife(eduimenu_s *, eduiitem_s *item, u32);

static void edpartImpactDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartTrail1DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartTrail2DebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeDebrisIndex(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeDebrisScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeGenRateMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartChangeInstanceRot(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeMaxLifeMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartEmitterDebrisMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceFlagsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartWorldInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeDebrisPerSec(eduimenu_s *, eduiitem_s *item, u32);

static void edpartChangeInstanceFlag(eduimenu_s *, eduiitem_s *item, u32);

static void edpartDebrisSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartInstanceOrientMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartPartIndexMenu(eduimenu_s *menu) __asm__("_ZL19edpartPartIndexMenuP10eduimenu_sP10eduiitem_sj.isra.12");
static void edpartPartIndexMenu(eduimenu_s *menu);

static void edpartLevelPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartThingsInstanceMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelDieDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelLevelTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelPartIndexMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelScaleTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeInstanceScale(eduimenu_s *, eduiitem_s *item, u32);

static void edpartInstanceOrphansMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelImpactPartMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartChangeInstanceVarRot(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((optimize("O3"))) void edpartDeleteInstanceOrphan(eduimenu_s *menu, eduiitem_s *item, u32);

static void edpartFileSaveEffectsLevel(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralPartIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartInstanceSettingsMenu(eduimenu_s *menu, eduiitem_s *, u32);

// Retail only keeps the IPA-SRA clone, which takes just the parent menu.
static __attribute__((noinline, noclone)) void edpartDebrisIndexMenu(eduimenu_s *menu) __asm__("_ZL21edpartDebrisIndexMenuP10eduimenu_sP10eduiitem_sj.isra.13");
static void edpartDebrisIndexMenu(eduimenu_s *menu);

static void edpartLevelDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartCancelDebrisIndexMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelDebrisScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelGeneralTypeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelImpactDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelTrail1DebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelTrail2DebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartFileSaveEffectsGeneral(eduimenu_s *parent, eduiitem_s *, u32);

static void edpartGeneralDebrisIndexMenu(eduimenu_s *menu, eduiitem_s *item, u32 value);

static void edpartCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelChangeMaxLifeMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelEmitterDebrisMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceFlagsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceScaleMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelWorldInstanceMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelDebrisSettingsMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceOrientMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelThingsInstanceMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceOrphans(eduimenu_s *menu, eduiitem_s *, u32);

static void edpartCancelInstanceOrphansMenu(eduimenu_s *, eduimenu_s *);

static void edpartCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((optimize("O3"))) void edpartDeleteAllInstanceDuplicates(eduimenu_s *menu, eduiitem_s *, u32);

void edpartDoInput(nupad_s *pad);

extern "C" {
extern ed_module_s edpartdesc;
}

// ==== src/legoapi/render/core/terrain_stubs.cpp: declarations ====
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/edtools/edpp_internal.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/android/nurain_android.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nustring.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

extern i16 TerrImpact;
extern i32 terrhitflags;
extern i32 TERRAINMASK_NONWEAPON;
extern i32 TERRAINMASK_NONDROID;
extern i32 geom;
extern NUVEC TerrImpactPos;
extern NUVEC TerrImpactNormal;
extern NUVEC ShadNorm;
extern NUVEC ShadRoofNorm;
extern NUVEC EShadNorm;
extern NUVEC EShadRoofNorm;
extern f32 ShadRoofY;
extern f32 EShadRoofY;
extern TerrainLastImpact_s TerrLastImpact;
extern TERRSET * CurTerr;
// Runtime-selected groups appended after the fixed terrain allocation.
extern i32 WallSplinesOnly;
extern "C" {
extern i32 IgnoreWallSplines;
}
extern f32 TerrPlatScanDist;
extern i32 cntrots;
#include "legoapi/render/core/terrain_stubs_types.h"
void ScanTerrIDRemovePlat(i32 platform_index);

void TerrainSkinAllocate(terrsitu_s *terrain_group);

void SkinPlatformSize(i32 group_index, unsigned char *buffer, PLATSKININFO *info);

extern "C" {
    void *NuScratchAlloc32(i32 size);
    void NuScratchRelease(void);
}

TERRAIN_TRACK_SLOT *ScanTerrId(void *hit_flags);
void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
i32 PlatformChecks(i32 count, NUVEC *movement);
i32 HitTerrain(void);
void TerrainImpactNorm(void);
void RayImpact(NUVEC *);
void StorePlatImpact(void);
i32 TerrainPlatformEmbedded(NUVEC *movement);
i32 TerrShapeSideStep(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrainImpact(NUVEC *position, NUVEC *movement, u8 *hit_flags);
void TerrFlush(void);
void NewScanRot(NUVEC *position, i32 terrain_mask);
f32 NewCast(NUVEC *position, f32 height_above, f32 height_below);
void TerrDrawPlat(tertype *terrain, i16 index);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
i32 DebrisSingleCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);
i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32, NUVEC *, f32, f32, u8);

extern "C" {
    i32 DebrisCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisCollisionCheckScaleY(NUVEC *, f32, f32);
    i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *, f32, f32, u8);
    i32 DebrisTorusCollisionCheckScaleY(NUVEC *, f32, f32);
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern i32 EDPP_SCALE_TYPES;
    extern debinftype *effecttypes;
    extern debscale_s *debscale;
    extern f32 panelglobaltime;
    extern f32 globaltime;
    extern f32 renderpanelglobaltime;
    extern f32 renderglobaltime;
    extern f32 timeincrement;
    extern i32 globalframes;
    extern i32 update_debris_enabled;
    extern i32 debris_setup_called;
    extern usize debris_trash_space;
    extern usize debris_trash_size;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern debris_chunk_control_s *debris_chunk_controls;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debkeydatatype_s *debris_keystack;
    extern u32 *spherecolldata;
    extern u32 *toruscolldata;
    extern i32 numspherecolldata;
    extern i32 numtoruscolldata;
    extern NUMTL *DebMat[10];
    extern i32 debris_suspended;
    void DebrisReScale(i32, f32);
    void DebReAlloc(debkeydatatype_s *, i32);
    void DebFreeInstantly(i32 *);
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
    extern "C++" {
        void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
        void DebrisCleanUpDmaDebTypeTables(void);
        void DebrisProcessAllocation(void);
        void DebrisProcessControlChunks(i32);
        void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
    }
}

void DebrisFreeOldestDmaDebTypeTable();

extern "C" i32 edppLoadPage(char *path, i32 flag, usize scene);
// NuFileExists @nufile (nucore_plain.cpp).
extern "C" i32 NuFileExists(char *name);

extern "C" void TerrainSetCur(void *terrain);
extern "C" void TerrSetPlatScanDist(f32 dist);

extern "C" void TerrainPlatformNewUpdate(void);
extern "C" i32 PARTLookupTypePageOnly(char *, i32);
extern "C" part_type_s part_types[128];
extern "C" i32 part_types_used;
extern "C" f32 partglobaltime;
void edpartScaleType(i32 index, f32 scale);
void *InitPartDebris(VARIPTR *buf, VARIPTR *, i32 capacity, i32 named_count, char **names, i32 page);

void Particles_Load(WORLDINFO *world, char **debris_name, i32 count, i32 flags);

extern "C" {

void AITerrInit(void);

f32 AITerrShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    i32 NewShadowOnPlatform(void);

i32 AITerrShadowOnPlatform(void);

i32 CheckForPlatInst(i32 instance);


i32 CreateScaledEffect(i32 effect_index, f32 requested_scale);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

void DebFreeAllCreatedEffects(void);

void DebFreeAllDMADebTablesInstantly(void);

void DebFreeAllPanelEffects(void);

void Debris(i32 update_panel_time);

i32 DebrisAllCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

} // extern "C"

void DebrisTimeSlip(i32 group);


extern "C" {

void DebrisEmitterMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);

void DebrisEmitterOrientationMtx(i32 handle, NUMTX *source);

void DebrisEmitterPos(i32 handle, f32 x, f32 y, f32 z);

i32 DebrisFindAllOfType(i32 effect_index, NUVEC *positions, i32 *handles, i32 capacity, i32 skip, i32 active_only);

i32 DebrisGetConeProperties(i32 handle, NUVEC *origin, NUVEC *end, f32 *start_radius, f32 *end_radius,
                                NUVEC *direction, f32 *length, f32 *speed);

f32 DebrisGetDuration(i32 effect_index);

char *DebrisGetName(i32 effect_index);

i32 DebrisGetParticleCount(i32 handle);

i32 DebrisGetRingProperties(i32 handle, NUVEC *origin, NUVEC *direction, f32 *radius);

void DebrisOrientation(i32 handle, i16 z, i16 y);

void DebrisOrientationMtx(i32 handle, NUMTX *source);

void DebrisParticleMomentum(i32 handle, f32 x, f32 y, f32 z);

void DebrisPopulateInstance(i32 handle, f32 duration);

void DebrisPosOrientationMtx(i32 handle, NUMTX *source);

i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);

void DebrisProcessTimeSlip(void);

i32 DebrisQueryPriority(i32 effect_index);

void DebrisReScale(i32 effect_index, f32 scale);

void DebrisReflectionOrientation(i32 handle, i16 x, i16 y, f32 plane, f32 scale);

extern NUVEC * CutoffCameraVec;

void DebrisRegisterCutoffCameraVec(void *position);

void DebrisReserveTrashableSpace(VARIPTR *buffer, VARIPTR, i32 chunk_count, i32 glass_chunk_count);

void DebrisResetTimers(void);

void DebrisSetDetailLevels(i32 handle, i32 detail_levels);

void DebrisSetDrawFlag(i32 handle, i8 draw_flag);

void DebrisSetFacing(i32 handle, u8 enabled, i16 x_angle, i16 y_angle);

void DebrisSetGroupID(i32 handle, i16 group);

void DebrisSetPriority(i32 handle, u16 priority, u8 automatic);

extern i32 debris_render_group;
void DebrisSetRenderGroup(i32 group);

void DebrisSetRoomID(i32 handle, nugscn_s *scene);

void DebrisSetTrigger(i32 handle, i32 first, i32 second, f32 third);

void DebrisSetUserData(i32 handle, void *user_data);

void DebrisShift(NUMTX *transform);

void DebrisStartOffset(i32 handle, f32 offset);

void DebrisStatusAlwaysOff(i32 *handle);

void DebrisStatusAlwaysOn(i32 *handle);

void DebrisStatusNormal(i32 *handle);

i32 DebrisTorusCollisionCheck(NUVEC *position, f32 radius);

i32 DebrisTorusCollisionCheckFlag(NUVEC *position, f32 radius, u8 flags);

i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

i32 DebrisTorusCollisionCheckScaleYFlag(NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void DebrisTrashableSetup(VARIPTR *buffer, VARIPTR *);

void DebrisTypeStatusAlwaysOff(i32 type);

void DebrisTypeStatusAlwaysOn(i32 type);

void DebrisTypeStatusNormal(i32 type);

i32 DeletePlatinst(i32 index);

void DrawPlatform(i32 platform_index);

i32 FindPlatInst(i32 instance);

i32 NewMSituTerrEx(NUVEC *position, void *data, NUVEC *minimum, NUVEC *maximum, f32 radius);

i32 NewPlatInst(void *object, i32 instance);

i32 NewPlatInstMSitu(void *object, i32 source);

i32 NewRayCastEx(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

i32 NewRayCastScaleY(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags);

    f32 NewShadowEx(NUVEC *position, i32 handle, f32 height_above, f32 height_below, i32 terrain_mask);

f32 NewShadow(NUVEC *position, f32 height_above, f32 height_below, i32 terrain_mask);

    void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void NewTerrain(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius, f32 collision_radius,
                    i32 scan_flags);

void NewTerrainScaleY(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                          f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags);

void PartTerrInit(void);

void PlatInstBounce(i32 index, f32 impulse, f32 spring, f32 damping);

void PlatInstCenter(i32 index, NUVEC *position);

i32 PlatInstGetHit(i32 index);

void PlatInstRotate(i32 index, i32 rotate);

void PlatSkinMemRigister(void *start, void *end, i32 maximum);

void PlatSkinMemReset(void);

i32 PlatInstSkinRegisterEx(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags,
                               TERRSET *source);

i32 PlatInstSkinRegister(NUMTX *matrix, void *skin_data, void *matrix_data, f32 scale, i32 instance, i32 flags);

void PlatOnOff(i32 index, i32 enabled);

void *PlatSkinEndReigster(i32 cache_slots);

void PlatformRemoveCallback(void (*function)(void *));

void PlatformUpdateCallback(void (*function)(void *), void *argument);

void SortDebrisRenderStack(void);

const char *TerrErrorString(i32 error);

extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();

i32 TerrainFreeId(void *id);

TERRSET *TerrainGetCur(void);

TERRAIN_GROUP *TerrainGetModelByInst(i32 instance);

void TerrainPlatGetMtx(i32 index, NUMTX **previous, NUMTX **current);

void UpdateDebrisRenderStackPriority(void);

void *terraininit(i32 level_num, void *buffer, void *buffer_end, i32 options, char *path, void *scene, i32 group);

void UpdatePlatinst(i32 index, void *object);

} // extern "C"

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern ADDPART_s Default_ADDPART;

struct rtlset;
extern rtlset * PartRTL;
extern f32 PARTSCALEUPTIME;
void NewPartOrderedRotation(PART_s *);
void PartTimeSlip();
void PartCleanupTypes();
void UpdatePartEmits(f32);

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.
extern i32 debris_suspended;

    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    extern f32 globaltime;
    extern f32 partglobaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void DebrisEmitterMomentum(i32, f32, f32, f32);
    void DebrisParticleMomentum(i32, f32, f32, f32);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    void DebrisSetUserData(i32, void *);
    i32 CreateScaledPARTEffect(i32, f32);
    extern i32 debris_suspended;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern u32 partseed;
    extern NUGSCN *part_scene[32];

void NuPartEnableRayCasts(i32 enabled);

u32 NuPartGetSeed(void);

void NuPartSetSeed(i32 seed);

    extern part_type_s part_types[128];
    extern NUGSCN *part_scene[32];
    extern part_emit_s part_emits[40];
    extern i32 part_emits_used;
    void CheckPartCount();
    i32 AddPARTEffect(i32, NUVEC *);
    void PARTStartOffset(i32, f32);

    void ResetParts(void);

    i32 DebAlloc(void);
    void DebrisStartOffset(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisOrientation(i32, i16, i16);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

void AddDebrisEffectToStack(debkeydatatype_s *);
void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;



// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
extern f32 ForceThrowSpeed, ForceThrowGravity;
extern "C" void KillPart(PART_s *, i32);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);
void SetCoinType(i32 model, GIZMOPICKUP_s *pickup);
u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void ReleasePush(GameObject_s *);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
i32 CannotKill(GameObject_s *);



extern "C" f32 coinimpactwait;
extern i32 GAMEDEMO;
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);



extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);


void PowerUp_Particles(WORLDINFO_s *, NUVEC *);






void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern WORLDINFO_s *WORLD;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;




extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

void AddFiniteShotDebrisEffectUserData(i32 *handle, i32 effect, NUVEC *position, i32 count, void *user_data);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 AddPARTEffect(i32 effect, NUVEC *position);

    void KillPart(PART_s *, i32);
    void NewPartRotation(PART_s *);
    void DebrisPopulateInstance(i32, f32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

void AddRotatedDebrisEffect(i32 *handle, i32 effect, f32 x, f32 y, f32 z, i16 rotation_x, i16 rotation_y);

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position, NUVEC *orientation, NUVEC *momentum,
                                         i32 count, f32 scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

i32 AddScaledVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                          i16 y_rotation, f32 scale);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

i32 AddScaledVariableShotDebrisEffect1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect2(i32 effect_index, NUVEC *position, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, i32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                           i8 flags, f32 scale);

i32 AddScaledVariableShotDebrisEffect5(i32 effect, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 time, NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                           u16 priority, i8 flags, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

    void AddVariableShotDebrisEffectMtx(i32, NUVEC *, i32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation,
                                        NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed3(i32 effect_index, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void CheckPartCount(void);

void DrawParts(i32 keep_offscreen);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 GetMaxPartTypes(void);

i32 GetPartCount(void);

char *GetPartName(i32 index);

extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
extern i32 part_types_used;
extern i32 part_emits_used;
extern NUGSCN * part_scene[32];
    // The editor loader clears these page IDs with aligned vector stores.
extern i32 part_scene_pageid[32];
extern i32 part_platimpactcnt;
extern i16 part_platimpactlist[4];
    void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void KillAllParts(void);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

void KillPartsByScene(NUGSCN *scene);

void NewPartRotation(PART_s *part);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

f32 PARTGetTotalOffTime(i32 index);

f32 PARTGetTotalOnTime(i32 index);

i32 PARTLookupType(char *name);

extern i32 edpart_load_particle_page;

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void PARTStartOffset(i32 index, f32 offset);

void ParticleReset(void);

void RemovePARTEffect(i32 index);

void ResetParts(void);

    void DebrisStatusAlwaysOff(i32 *);
    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);
i32 edpartLookupDebrisEffect(char *name);
extern "C" i32 GetSfxIdN(char *, i32);

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
extern i32 edpart_nearest;
extern NUVEC edpart_cam_pos;
extern part_emit_s * edpart_nearest_emit;
extern part_typedesc_s * edpart_nearest_type;
}

void edpartDetermineNearest(f32 distance);
extern "C" {
extern i32 edbits_part_level_page;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

void PartTimeSlip();

extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);


void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_3D(PART_s *);
void PartCollide_2D(PART_s *);
void PartUpdate_Heart(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

void AddTorpedoAsPart(nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void AddHeartAsPart(GameObject_s *recipient, nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void PowerUp_AddPart(NUVEC *position, NUVEC *velocity, f32 scale, f32 lifetime);

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);

void MakePartVector(NUVEC *velocity, NUVEC *direction, f32 scale);

void PartCollide_2D(PART_s *part);

void PartCollide_3D(PART_s *part);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartStop_Coin(PART_s *part);

void AddCoinsAsParts(i32 type_id, nuvec_s *position, nuvec_s *velocity, float lifetime, float scale);

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

void UpdatePartEmits(f32 time);

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range);

void edpartDestroy(i32 index);

void PartCleanupTypes();

extern "C" f32 brickimpactwait;
extern "C" i32 GetSfxId(const char *);
extern "C" void PlaySfxByIdAndSetVolume(i32, NUVEC *, f32);

void PartImpact_Brick(PART_s *part);

void PartUpdate_Heart(PART_s *part);

void Asteroid_PartKill(PART_s *part, i32 reason);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

void PartImpact_Basketball(PART_s *part);

void PartUpdate_Basketball(PART_s *part);

PART_s *Part_FindFromHSpecial(nuhspecial_s *special);

void NewPartOrderedRotation(PART_s *part);

// ==== src/gameapi/edtools/edtoolsall_plain.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

extern "C" i32 edSfxAllCount;

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")





static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

float edanimPlayerAnimDistance(i32 parameter_index);
void edpartDestroy(i32 index);
void edgraDoInput(nupad_s *pad);
void edgraDetermineNearestInstance(f32 distance);
void edgraDrawCursor();
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
extern "C" i32 NuRndrDoingScreenGrab;



// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}

void edppPtlShelve(i32 index);


extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void UpdateTotalPtls(debinftype *effect);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDoInput(nupad_s *pad);
void edppDetermineNearest(f32 distance);
void edppDrawCursor();
void edppHighlightNearest();
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

















// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif


// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtools_types.h"

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

static void edgraInit();

static void edgraClose();

static void edgraEnter();

static i32 edgraProc(f32 delta_time, nupad_s *pad);

static void edgraRender();

static void edppInit();

static void edppClose();

static void edppEnter();

static void edppApply();

static i32 edppProc(f32 delta_time, nupad_s *pad);

static void edppRender();

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit();

static void edbriClose();

static i32 edbriProc(f32 delta_time, nupad_s *pad);

static void edbriRender();

static void edanimInit();

static void edanimClose();

static i32 edanimProc(f32 delta_time, nupad_s *pad);

static void edanimRender();

void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern f32 edanim_cam_dist;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static void edbriEnter();

static void edanimEnter();

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);


void edcamSetContoller(i32 invert_pitch);



extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);


static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

// ==== src/gameapi/edtools/edtoolsall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>
#include <string.h>
#include <new>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/numath/nurand.h"

extern EdRegistry theRegistry;
extern "C" VuVec SpherePos1;
extern "C" VuVec SpherePos2;
extern VuVec SpherePos1;
extern VuVec SpherePos2;
extern EdInputContext * Input;
extern MemoryManager theMemoryManager;
extern LevelEditor theLevelEditor;
extern PropertyTool thePropertyTool;
extern eduiiattr_s EdLevelAttr;
extern i32 EdLevelFnt;
extern "C" void eduiSetCameraEnabled(i32);
extern "C" void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
extern "C" i32 edbri_nearest;
extern "C" NUVEC edbri_cam_pos;
extern "C" NUMTL *edbri_mtl, *edbri_mtl_zoff;
extern "C" f32 edbri_length, edbri_width;
extern "C" i32 edbri_rotz, edbri_roty, edbri_planks, edbri_post_interval;
extern "C" i32 edbri_plank_instance_type, edbri_post_instance_type, edbri_bridges_used;
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void NuRndrLine2di(i32, i32, i32, i32, i32, NUMTL *);
extern "C" void edbitsDrawCircleTilted(NUVEC *, f32, i32, NUMTL *, i32, i32);
NUVEC NuFadeObjGetAngleTerrainValues(NUVEC *);
extern "C" void eduiAddPropTextPickEnt(eduimenu_s *, eduiitem_s *);
extern "C" char *GetSfxName(i32);
extern "C" void PlaySfxById(i32, nuvec_s *);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
static i32 get_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
static void set_manipulator_attribute(ClassObjectListEntry *, i32, i32, void *);
void cbEdLevelDestroy(eduimenu_s *, eduimenu_s *);
void cbEdLevelDestroyOnSelect(eduimenu_s *, eduiitem_s *, u32);
extern i32 EdType_Char;
extern i32 EdType_Short;
extern i32 EdType_Int;
extern i32 EdType_Float;
extern i32 EdType_VuVec;
extern i32 EdType_VuMtx;
extern i32 EdType_Enumeration;
extern i32 EdType_String;
extern i32 EdType_Colour3;
extern i32 EdType_NuHSpecial;
extern i32 EdType_NuVec;
extern i32 EdType_NuMtx;

void SerialiseChar(EdStream &, void *, i32);
void SerialiseShort(EdStream &, void *, i32);
void SerialiseInt(EdStream &, void *, i32);
void SerialiseFloat(EdStream &, void *, i32);
void SerialiseVuVec(EdStream &, void *, i32);
void SerialiseVuMtx(EdStream &, void *, i32);
void SerialiseString(EdStream &, void *, i32);
void SerialiseColour3(EdStream &, void *, i32);
void SerialiseNuHSpecial(EdStream &, void *, i32);
void SerialiseNuVec(EdStream &, void *, i32);
void SerialiseNuMtx(EdStream &, void *, i32);

extern f32 Scale;
extern EdManipulator theDefaultManipulator;
extern EdManMove theMoveManipulator;
extern EdManRotate theRotateManipulator;
extern EdManScale theScaleManipulator;
extern SplineTool theSplineTool;
DECOMP_ASSERT(sizeof(EdManipulator) == 0x6c, "EdManipulator ABI");
extern SplineHelper theSplineHelper;
extern KnotHelper theKnotHelper;
extern ClassEditor theClassEditor;
extern i32 pad_disabled;
extern EdSystem theEdSystem;
extern eduimenu_s * edLevelPinnedMenu;

void eduiSetPinnedMenu(eduimenu_s *menu);

void edSetPadDisabled(i32 disabled);

i32 edGetPadDisabled();

extern NUMTL * EdDrawMtl[2];
void EdDrawPolyArrow(VuVec const &, VuVec const &, i32, i32, float, float, float, float);
void EdDrawPolyCylinder(VuMtx const &, float, float, float, i32, i32, i32, i32);
void EdDrawMtx(VuMtx const *);
extern char * EDSPLINE_FILECHECK;

extern "C" {
extern char edgra_filter_string[16];
extern i32 edgra_mode;
    extern NUGSCN *edbits_base_scene;
    extern NUGSCN *edbits_things_scene;
    extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 edpart_emitrotx, edpart_emitroty, edpart_emitrotz;
    extern NUVEC edanim_cam_pos;
extern i32 edgra_last_clump_in_buffer;
extern i32 edgra_copy_source;
extern f32 edgra_global_fadein;
extern f32 edgra_global_fadeout;
    void edgraInitAllClumps(void);
    void DebFreeInstantly(i32 *);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetPriority(i32, u16, u8);
    void DebrisSetRoomID(i32, NUGSCN *);
    void DebrisSetDetailLevels(i32, i32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    i32 AddPARTEffect(i32, NUVEC *);
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 EDPP_MAX_TYPES;
    extern i32 edpp_usememcard;
    extern i32 edpp_copy_mode, edpp_copyrotz, edpp_copyroty, edpp_copy_source_count, edpp_copy_enclosed;
    extern i32 edpp_readout, edpp_num_orphans;
    extern i32 globalframes;
    extern f32 edpp_copy_size;
    extern NUMTL *edpp_mtl, *edpp_boxmtl;
    void edppDrawSpheres(debinftype *, i32);
    void edppDrawTorus(debinftype *, i32);
    extern i32 part_page_used[8];
    extern i32 edanim_params_used;
    extern i32 edanim_particle_type;
    extern i32 edanim_emitrotz;
    extern i32 edanim_emitroty;
    extern f32 edpp_offset;
extern i32 edbits_particle_level_page;
}

void edpartDestroy(i32 index);

i32 edpartLookupObjectInScene(char *, NUGSCN *);

void EdTerrInit(void *, void *);

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);

void EdDrawBegin(i32 material);

void edpartPlace(i32 index, nuvec_s *position);

void edppDetermineNearest(f32);
void edppMultipleCopyPaste();
void edppMultipleCopyCopy();
void edppMultipleCopyClear();
i32 edppPtlCreate(NUVEC *, i32);
i32 edppPtlPlace(i32, NUVEC *);
void edppPtlDestroy(i32);

void edppDoInput(nupad_s *pad);

void EdTerrShadow(nuvec_s *position, float height_above, float height_below, i32 terrain_mask);

void edpartCreate(nuvec_s *position, i32 type);

i32 edppPtlPlace(i32 index, NUVEC *position);

void EdDrawPolyTri(VuVec const &a, VuVec const &b, VuVec const &c, i32 colour);

void edanimDoInput(nupad_s *pad);

i32 edbriFileSave(char *path);

i32 edppPtlCreate(NUVEC *position, i32 effect_index);

i32 edgraFileSave(char *path);

void edppPtlShelve(i32 index);

void EdDrawLineCube(VuMtx const &transform, float size, i32 colour);

void EdDrawPolyAxis(VuMtx const &transform, float size, i32 opacity);

i32 edanimFileSave(char *path);

void edpartInitType(i32 index);

void edppDrawCursor();

extern "C" {
    void DebFreeInstantly(i32 *);
}

void edppPtlDestroy(i32 index);

void EdDrawLineArrow(VuMtx const &transform, float size, i32 colour);

void EdDrawLineCross(VuVec const &position, float size, i32 colour);

void EdDrawPolyArrow(VuVec const &start, VuVec const &end, i32 sides, i32 colour, float radius, float limit,
                     float radius_factor, float radius_offset);

void edbriDrawCursor();

void edgraClumpPlace(i32 index, NUVEC *position);

void edgraDrawCursor();

void edpartPtlShelve(i32 index);

void edpartScaleType(i32 index, float scale);

i32 edppSaveEffects(char *filename, char page);

void EdDrawLineSphere(VuVec const &center, float radius, float scale, i32 colour);

void EdDrawPolySector(VuVec const &center, float radius, i32 axis, i32 first_angle, i32 last_angle, i32 colour,
                      i32 segments);

void edanimDrawCursor();

extern "C" i32 part_types_used;
extern "C" i32 part_emits_used;

void edpartDrawCursor();

i32 edgraClumpCreate(NUVEC *position);


void EdDrawLineCircleX(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineCircleZ(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineCircleY(VuVec const &centre, float radius, i32 colour, i32 segments);

void EdDrawLineSegment(VuVec const &a, VuVec const &b, i32 colour);

i32 edanimParamCreate(i32 instance_id);

i32 edpartSaveEffects(char *filename, char page);

void edppPtlChangeType(i32 index, i32 effect_index);

i32 edppPtlCreateCopy(NUVEC *position, i32 source_index);

void EdDrawPolyCylinder(VuMtx const &transform, float half_length, float radius, float end_radius, i32 sides,
                        i32 colour, i32 cap_start, i32 cap_end);

__attribute__((force_align_arg_pointer)) void EdDrawPolyCylinder(VuVec const &start, VuVec const &end, i32 sides,
                                                                 i32 colour, i32 cap_colour, float radius, float limit,
                                                                 float offset);

void edanimParamDestroy(i32 index);

void edbitsDoSingleDump(i32 face);

void edgraCalculatePage(char page, i32 calculate_vectors);

void edgraInstancePlace(i32 index, NUVEC *position);

i32 edpartLookupObject(char *name);

i32 edSpline_FindAllBeg(NUGSCN *scene, char *prefix, NUGSPLINE **results, i32 capacity);

i32 LoadEditorSplines(char *path, VARIPTR *buf, VARIPTR *buf_end);

i32 edSpline_FindAllSub(NUGSCN *scene, char *name, NUGSPLINE **results, i32 capacity);

NUGSPLINE *edSpline_SplineFind(NUGSCN *scene, char *name);

void edSpline_SplineList(nugscn_s *);

void edanimParticlePlace(i32 index, NUVEC *position);

void edanimStartAllPages();

void edgraInstanceCreate(NUVEC *position);

i32 edpartPtlChangeType(i32 index, i32 type);

void edppDestroyAllPages();

void edanimParticleCreate(NUVEC *position);

void edgraInstanceDestroy(i32 index);

void edppDetermineNearest(float max_distance_squared);

void edppHighlightNearest();

void edppMultipleCopyCopy();

void edbriDetermineNearest(float distance);

void edgraSortVectorBuffer(i32 index);

void edpartDestroyAllPages();

void edppMultipleCopyClear();

void edppMultipleCopyPaste();

void edppStartSingleEffect(i32 index);

void edpartHighlightNearest();

void edpartMultipleCopyCopy();

void edpartMultipleCopyClear();

float edanimPlayerAnimDistance(i32 parameter_index);

void edanimRenderSoundEmitters(i32 parameter_index);

void edanimDetermineNearestAnim(float distance);

void edgraDetermineNearestClump(f32 distance);

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

void edanimRenderParticleEmitters(i32 parameter_index);

void edanimDetermineNearestSound(float distance);

void edgraDetermineNearestInstance(f32 distance);

void edanimDetermineNearestParticle(float distance);

void EdDrawEnd();

void EdDrawMtx(VuMtx const *matrix);

i32 EdTerrRay(VuVec &, VuVec &);
















































extern EdEnumControl::Item OpenClosedItems[3];
extern EdEnumControl::Item OnOffItems[3];
extern EdEnumControl::Item YesNoItems[3];











extern i32 AxisColour[8];

































inline EdMatrixControl::~EdMatrixControl() {
    Destroy();
}

inline void EdMatrixControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdMatrixControl));
}








inline void EdStringControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdStringControl));
}






extern f32 MouseScale;


template <> inline void EdValueControl<f32>::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdValueControl<f32>));
}


inline void EdFloatControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdFloatControl));
}








inline EdVectorControl::~EdVectorControl() {
    Destroy();
}

inline void EdVectorControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdVectorControl));
}































extern EdSfxNameControl * sfxNameControl;



























extern eduimenu_s *edLevelDestroyThisMenu;
extern eduimenu_s *edLevelDestroyThisMenu2;



inline EdClassObjectNameControl::~EdClassObjectNameControl() {
}

inline void EdClassObjectNameControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdClassObjectNameControl));
}



























inline EdSubSystem::~EdSubSystem() {
}






inline void EdControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdControl));
}

// Match each control's allocation type, rather than inheriting the base size.
inline void EdEnumControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdEnumControl));
}

inline void EdBitControl::operator delete(void *memory) {
    theMemoryManager.FreePool(memory, sizeof(EdBitControl));
}















// ==== src/gameapi/edtools/edtoolsall_plain.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
// ---- begin src/gameapi/edtools/edanimall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameframework/saveload.h"

extern "C" i32 edSfxAllCount;

// Animation editor subsystem stubs (static, internal linkage).

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
void edanimRegisterBaseScene(NUGSCN *scene);
static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbMCTBCardType(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbCancelOptMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbCancelMCTBMenu(eduimenu_s *, eduimenu_s *);
static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbMCTBCardFormat(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbSetSoundTiming(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSetSwitchDelay(eduimenu_s *, eduiitem_s *item, u32);
static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);

// Remaining animation-editor UI/menu callbacks.

static __attribute__((used)) void edanimcbCancelSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardPresent(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetParticleRate(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelBouncyMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardLoadSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardSaveSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbMCTBCardUnFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyDamping(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbSetBouncyTension(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardFreeSpace(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardSlotsUsed(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalSoundType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardDeleteSlot(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckFormat(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetBouncyPlayerGrav(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbCancelLocalSoundMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbMCTBCardCheckKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBCardWriteKeyCard(eduimenu_s *menu, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSetLocalParticleType(eduimenu_s *menu, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbToggleParticleSwitch(eduimenu_s *, eduiitem_s *item, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbCancelParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalSoundTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbCancelLocalParticleTypeMenu(eduimenu_s *, eduimenu_s *);

static __attribute__((used)) void edanimcbBouncyMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbParticleTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbMCTBMenu(eduimenu_s *parent, eduiitem_s *, u32);

static __attribute__((used)) void edanimcbLocalSoundMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edanimall.cpp ----
// ---- begin src/gameapi/edtools/edmenucallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")





static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options

// ---- end src/gameapi/edtools/edmenucallbacks.cpp ----

float edanimPlayerAnimDistance(i32 parameter_index);
void edpartDestroy(i32 index);
void edgraDoInput(nupad_s *pad);
void edgraDetermineNearestInstance(f32 distance);
void edgraDrawCursor();
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
extern "C" i32 NuRndrDoingScreenGrab;



// ---- begin src/gameapi/edtools/edptlall.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

// The original particle editor's private callbacks and menu state share this TU.
// ---- begin src/gameapi/edtools/edptlcallbacks.cpp ----
#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/numtl.h"
#include <stdio.h>
#include <string.h>
#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern NUMTL *DebMat[];
    extern i32 edpp_readout;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *namemenu;
}

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDamageFlagMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTorusMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeNameMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeTextureSelect(eduimenu_s *, eduiitem_s *, u32);


extern "C" {
    extern eduimenu_s *edpp_active_menu;
    extern eduimenu_s *edptl_testdetail_menu;
    extern eduimenu_s *edptl_detail_menu;
    extern eduimenu_s *edptl_drawflag_menu;
    extern eduimenu_s *edptl_group_menu;
    extern eduimenu_s *edptl_switchtype_menu;
    extern eduimenu_s *edptl_switch_menu;
    extern eduimenu_s *dpadmodemenu;
    extern eduimenu_s *sscalemenu;
    extern eduimenu_s *edptl_scaleeffect_menu;
    extern eduimenu_s *edptl_instancesettings_menu;
    extern eduimenu_s *edptl_torus_menu;
    extern eduimenu_s *collmenu;
    extern eduimenu_s *edptl_damageflag_menu;
    extern eduimenu_s *edptl_damage_menu;
    extern eduimenu_s *edptl_soundcontrol_menu;
    extern eduimenu_s *edptl_soundid_menu;
    extern eduimenu_s *edptl_soundx_menu;
    extern eduimenu_s *edptl_sounds_menu;
    extern eduimenu_s *edptl_bounce_menu;
    extern eduimenu_s *ptljibmenu;
    extern eduimenu_s *ptlrotmenu;
    extern eduimenu_s *ptlsizemenu;
    extern eduimenu_s *edptl_ghost_menu;
    extern eduimenu_s *ptlcutoffmenu;
    extern eduimenu_s *emittimemenu;
    extern eduimenu_s *etimemenu;
    extern eduimenu_s *changegenratemenu;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlstartvelmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlgravmenu;
    extern eduimenu_s *ptlemitvelmenu;
    extern eduimenu_s *ptlemitmenu;
    extern eduimenu_s *textureselectmenu;
    extern eduimenu_s *texturemenu;
    extern eduimenu_s *ptlcolmenu;
    extern eduimenu_s *ptlgsortmenu;
    extern eduimenu_s *ptlreadoutmenu;
    extern eduimenu_s *edptl_quickdel_menu;
    extern eduimenu_s *edptl_orphanlist_menu;
    extern eduimenu_s *ptlclipmenu;
    extern eduimenu_s *messagemenu;
    extern eduimenu_s *ptldatamenu;
    extern eduimenu_s *ptltypemenu;
    extern eduimenu_s *effectlistmenu;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern u32 edgrey[4];
    extern u32 eddarkred[4];
    extern i32 EDPP_MAX_TYPES;
    extern i32 edbits_particle_level_page;
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    void edppDeleteEffect(i32 index);
    void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);
    void DebrisOrientation(i32 handle, i16 z, i16 y);
    void DebrisEmitterOrientation(i32 handle, i16 z, i16 y, i16 x);
    void DebrisStartOffset(i32 handle, f32 offset);
    void DebrisSetTrigger(i32 handle, i32 type, i32 id, f32 value);
    void DebrisReflectionOrientation(i32 handle, i16 z, i16 y, f32 offset, f32 bounce);
    void DebrisSetFacing(i32 handle, u8 enabled, i16 x, i16 y);
    void DebrisSetGroupID(i32 handle, i16 group);
    void DebrisSetRoomID(i32 handle, nugscn_s *scene);
}

void edppPtlShelve(i32 index);


extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 edpp_readout;
    extern i32 edpp_showAllPlaced;
    extern i32 edpp_snap_enabled;
    extern eduimenu_s *ptlvaremitmenu;
    extern eduimenu_s *ptlvarstartmenu;
    extern eduimenu_s *ptlstartvelmenu;
}
static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlQuickDeleteType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSelTextureType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlTextureSelectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlAddEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopyEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDeleteEffect(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeNameMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlQuickDeleteMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlReadoutMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbFileLoadEffects(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlSelType(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbScaleEffectMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlEmitVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlGravMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarStartMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlStartVelMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlVarEmitMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeGenRateMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeETimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlEmitTimeMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCutOffMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCopySize(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbApplyScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);
static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlDefaultCollEnv(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);
static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *, u32);
static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

// Particle-list editor UI/menu callbacks.

static void edptlcbApplyStarPoints(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelGhostMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelGroupMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbChangeCSDisable(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbScaleEffectMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlcbSetDebrisDetail(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbSetSoundControl(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyScaleFactor(eduimenu_s *menu, eduiitem_s *, u32);

static void edptlcbCancelBounceMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundXMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundsMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSoundControlMenu(eduimenu_s *menu, eduiitem_s *item, u32);

static void edptlcbApplyBounceFactor(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyBounceOffset(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelSoundIDMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbSetDebrisThinning(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelDrawflagMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbJumpToGameLocation(eduimenu_s *, eduiitem_s *item, u32);

static void edptlChangeRepeatBoxXZLock(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbCancelClipboardMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelOrphanListMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSwitchTypeMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelTestDetailMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelScaleEffectMenu(eduimenu_s *, eduimenu_s *);

static void edptlcbCancelSoundControlMenu(eduimenu_s *, eduimenu_s *);

// Particle editor UI/menu callbacks.

static void cbChangeName(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeX(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeY(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeZ(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlColMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlJibMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlRotMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlSelType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlShowAll(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyJib(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyRot(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCollMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlCopySize(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDataMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlGravMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelGCode(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSelGSort(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlSizeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlAddEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlApplyGrad(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplySize(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlGSortMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTorusMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeGrav(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCopyEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlCutOffMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlDamageMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSScaleMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelReadout(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlSnapToggle(eduimenu_s *, eduiitem_s *item, u32);

static void cbSelEffectList(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbChangeNameMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbEffectListMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeCutOn(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlEmitVelMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlReadoutMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSetXZFacing(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlVarEmitMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeETimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbChangeTorusLife(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad1(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeTorusRad2(eduimenu_s *, eduiitem_s *item, u32);

static void cbFileLoadEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyCollEnv(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangeCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSScale(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDeleteEffect(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlEmitTimeMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlStartVelMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlVarStartMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlChangeEmitVel(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeGenRateMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv1(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv2(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlApplyTorusEnv3(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlChangePriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlDamageFlagMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlDefaultCollEnv(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlSelTextureType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlQuickDeleteMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlQuickDeleteType(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlChangeDrawCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeRepeatFlag(eduimenu_s *, eduiitem_s *item, u32);

static void cbChangeNumCollSpheres(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeDamageFlags(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeSoundCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlTextureSelectMenu(eduimenu_s *menu, eduiitem_s *, u32);

static void cbPtlChangeCameraCutOff(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlChangeTextureSelect(eduimenu_s *menu, eduiitem_s *item, u32);

static void cbPtlInstanceSettingsMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void cbPtlToggleDynamicPriority(eduimenu_s *, eduiitem_s *item, u32);

static void cbPtlCancel(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelColMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelJibMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelRotMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelMessageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCollMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDataMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGravMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSizeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTypeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelGSortMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTorusMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelCutOffMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelSScaleMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeNameMenu(eduimenu_s *menu, eduimenu_s *);

static void cbCancelEffectListMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelReadoutMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelTextureMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarEmitMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeETimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelEmitTimeMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelStartVelMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelVarStartMenu(eduimenu_s *, eduimenu_s *);

static void cbCancelChangeGenRateMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelDamageFlagMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelQuickDeleteMenu(eduimenu_s *, eduimenu_s *);

static void cbPtlCancelInstanceSettingsMenu(eduimenu_s *, eduimenu_s *);

// ---- end src/gameapi/edtools/edptlcallbacks.cpp ----

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void UpdateTotalPtls(debinftype *effect);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);

// ---- end src/gameapi/edtools/edptlall.cpp ----


void edppDoInput(nupad_s *pad);
void edppDetermineNearest(f32 distance);
void edppDrawCursor();
void edppHighlightNearest();
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

















// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");



// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtools_types.h"

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
extern f32 edpp_copy_size;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
extern eduimenu_s * edgra_instance_menu;
extern eduimenu_s * edgra_changeinstance_menu;
extern eduimenu_s * edgra_clumpproperties_menu;
extern eduimenu_s * edgra_clumpsizes_menu;
extern eduimenu_s * edgra_clumparea_menu;
extern eduimenu_s * edgra_clumpdist_menu;
extern eduimenu_s * edgra_clumpfade_menu;
extern eduimenu_s * edgra_clumpterrain_menu;
extern eduimenu_s * edgra_clumpmode_menu;
extern eduimenu_s * edgra_dpadmode_menu;
extern eduimenu_s * edgra_sscale_menu;
extern eduimenu_s * edgra_globals_menu;
extern i32 edgra_filter;
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
extern NUVEC edgra_cam_pos;
extern f32 edgra_cam_dist;
extern i32 edgra_cam_ax;
extern i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
extern i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
extern i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
extern i32 edgra_pageid;
extern i32 edgra_rotz;
extern i32 edgra_roty;
extern i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
extern i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
extern NUVEC edpp_cam_pos;
extern f32 edpp_cam_dist;
extern i32 edpp_cam_ax;
extern i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
extern i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
extern i32 edpp_copy_mode;
extern i32 edpp_copy_enclosed;
extern i32 edpp_copy_source_count;
extern NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
extern eduimenu_s * memcardmenu;
extern eduimenu_s * messagemenu;
extern eduimenu_s * loadeffectsmenu;
extern eduimenu_s * mergeeffectsmenu;
extern eduimenu_s * deleteeffectsmenu;
extern eduimenu_s * filenamemenu;
extern eduimenu_s * namemenu;
extern eduimenu_s * confirmmenu;
extern eduimenu_s * changegenratemenu;
extern eduimenu_s * totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
extern i32 edpp_rotz;
extern i32 edpp_roty;
extern f32 edpp_offset;
extern i32 edpp_refrotz;
extern i32 edpp_refroty;
extern i32 edpp_copyrotz;
extern i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

static void edgraInit();

static void edgraClose();

static void edgraEnter();

static i32 edgraProc(f32 delta_time, nupad_s *pad);

static void edgraRender();

static void edppInit();

static void edppClose();

static void edppEnter();

static void edppApply();

static i32 edppProc(f32 delta_time, nupad_s *pad);

static void edppRender();

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit();

static void edbriClose();

static i32 edbriProc(f32 delta_time, nupad_s *pad);

static void edbriRender();

static void edanimInit();

static void edanimClose();

static i32 edanimProc(f32 delta_time, nupad_s *pad);

static void edanimRender();

void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
extern part_emit_s part_emits[40];
extern i32 part_page_on[8];
extern i32 part_page_used[8];
extern i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
extern NUVEC edanim_cam_pos;
extern f32 edanim_cam_dist;
extern i32 edanim_cam_ax;
extern i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
extern i32 edanim_emitrotz;
extern i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
extern i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
extern i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
extern eduimenu_s * edbri_dpadmode_menu;
extern eduimenu_s * edbri_plankinstance_menu;
extern eduimenu_s * edbri_postinstance_menu;
extern eduimenu_s * edbri_plankcount_menu;
extern eduimenu_s * edbri_bridgeproperties_menu;
extern eduimenu_s * edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
extern NUVEC edbri_cam_pos;
extern f32 edbri_cam_dist;
extern i32 edbri_cam_ax;
extern i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
extern i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static void edbriEnter();

static void edanimEnter();

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);


void edcamSetContoller(i32 invert_pitch);



extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
void edpartClearPage(i8 page);
void edpartDestroyAllParticles(void);
void edpartParticleReset(void);
void edpartRegisterPointerToGameCharLocation(NUVEC *position);
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);


static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}

// ==== src/legoapi/menus/screens/gamemenuall.cpp: declarations ====
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

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern MENUPACKET_s MenuPacket;
extern i32 NextArea_FreePlay;
void InitMission(MISSIONSYS *, i32);
extern f32 ICONSIZE, ICONX, DROPINALPHA, HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING, tEXIT, tCANCEL;
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
void Hub_DrawImportantBrick(i32, f32, f32, f32, i32, i32);
f32 GetAspectRatio();
extern "C" void BackupMenu(void);
extern "C" void BackupMenuNoFn(void);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                          u8 green, u8 blue, f32 max_width, i32 max_lines);
extern "C" void NuIOS_RecordFlurryEvent(char *event_name);
extern "C" void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                                      u8 alpha);
extern "C" void DrawMenuButtonPromptsEx(i32 confirm_prompt, i32 cancel_prompt, i32 flags, i32 enabled, u8 red, u8 green,
                                        u8 blue, u8 alpha);
extern i32 SAVESLOTS;
extern i32 MenuSFX;
extern i32 MENUSFX_MENUSELECT;
extern i32 MENUSFX_MENUBACK;
extern i32 MENUSFX_MENUNOENTRY;
extern i32 MENUSFX_MENUMOVE;
extern char *apitxt_LOADGAME;
extern char *apitxt_SAVEGAME;
extern char *apitxt_CONFIRMSAVE;
extern char *apitxt_CONFIRMOVERWRITE;
extern char *apitxt_CONFIRMLOAD;
extern char *apitxt_DOYOUWANTTOABORT;
extern char *apitxt_DOYOUWANTTOABORTLOAD;
extern char *apitxt_DOYOUWANTTOABORTFORMAT;
extern char *apitxt_CONFIRMDELETE;
extern char *apitxt_RETRY;
extern char *apitxt_SLOT;
extern char *apitxt_NEWSAVE;
extern char *apitxt_CANCEL;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_NOTENOUGHSPACE;
extern char *apitxt_DELETEGAME;
extern char *apitxt_DELETING;
extern char *apitxt_DELETECOMPLETE;
extern char *apitxt_FORMATTING;
extern char *apitxt_FORMATTINGCOMPLETE;
extern char *apiGameName;
extern char *apitxt_YES;
extern char *apitxt_NO;
extern char **TTab;
extern i16 tSELECTCONTROLS;
extern i16 tCONTROLSCANBECHANGED;
extern i16 tCLASSIC;
extern i16 tTOUCHSCREEN;
extern i16 tCONTROLLERCONNECTED;
extern i16 tCONTROLS;
extern i16 tCONSOLE;
extern i16 tTOUCH;
extern i16 tSURROUNDSOUND;
extern i16 tAUDIOVOLUME;
extern i16 tMUSIC;
extern i16 tWIDESCREEN;
extern i16 tON;
extern i16 tOFF;
extern i16 tACCEPT;
extern i16 tBACK;
extern i16 tCONTINUE;
extern i16 tCANCEL;
extern i16 tSTORY;
extern i16 tFREEPLAY;
extern i16 tREPLAYSTORY;
extern i16 tLOCKED;
extern i16 tAVAILABLETOBUY;
extern i16 tPOWERBRICK;
extern i16 tHOWTOPLAY;
extern "C" OPTIONSSAVE TempOptions;
extern i32 GAMEDEMO;
extern i32 menu_flash;
extern i16 tOUTOFTIME;
extern f32 minikittime;
extern "C" TIMER BonusTimer;
extern f32 text3d_height;
extern f32 text3d_width;
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
void GameDrawMenuEntry(MENU *menu, char *text);
i32 GetParentMenuID(void);
f32 GameSetSoundVolume(OPTIONSSAVE *options);
f32 GameSetMusicVolume(OPTIONSSAVE *options);
void SfxCheckMusicOnOff(OPTIONSSAVE *options);
void NewGame(void);
void NuIOS_RestoreInAppPurchases(void);
extern i32 startnewgame;
extern i32 startnewgame_initiated;
extern f32 newgamecamtime;
extern i32 newgamecam;
extern "C" u8 MENUNORMALR;
extern "C" u8 MENUNORMALG;
extern "C" u8 MENUNORMALB;
extern "C" f32 sfx_wait;
extern "C" f32 MENUBOTY;
void Draw_AUTOSAVEWARNING();
void Draw_NODATAAVAILABLE();
void Draw_NOMEMORYCARD();
extern "C" void Draw_NOTENOUGHSPACE(void);
extern "C" void Draw_SPACENEEDED(void);
extern "C" void Draw_CHECKINGMEMORYCARD(void);
extern "C" void Draw_DONOTREMOVEMEMORYCARD(void);
void Draw_OK(MENU_s *menu);

extern i32 memcard_cardchanged;
extern i32 MenuASCancelFinished;
extern i32 MenuCardWarningState;
extern i32 ButtonScaleMode;
extern i32 Menu_InLoadFlow;
extern i32 Menu_InWarningFlow;
extern i32 Menu_LastFlow;
extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 Menu_InSaveFlow;
extern i32 Menu_DisableCancel;
extern i32 Menu_InHeaderSave;
extern i32 Menu_Overwriteflag;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 SAVESIZE;

extern i32 memcard_autosave;
extern "C" i32 memcard_autosaveenabled;
extern "C" i32 memcard_autosavedisabled;
// Retail clears these "fast repeat" latches but never sets them.
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);

extern char * extrasavetext;
extern f32 extrasavealpha;
extern u8 RAP_WARNING_B;
extern u8 RAP_WARNING_G;
extern u8 RAP_WARNING_R;


extern i32 hub_new_level;
extern i32 hub_selectmode;
extern f32 selectmodetime;
extern i32 selectmodemode;
extern "C" i32 FinishLoop_On;
extern FadeSystem FadeSys;
void Hub_DrawFreePlaySelect();
void Hub_InitFreePlaySelect(i32 area, i32 first_model, i32 second_model);
void Hub_UpdateFreePlaySelect();
void WipeBackToHub();
void NewLevelFromMenu(LEVELDATA_s *level, i32 menu_id, i32 menu_y, i32 remember_hub);




void APIMenuDrawMemCardSlots(MENU *menu, f32 y);

void MenuDrawLoad(MENU_s *menu);

void MenuDrawSave(MENU_s *menu);


void MenuExitLoad(MENU_s *);

void MenuExitSave(MENU_s *);

void MenuDrawClips(MENU_s *);

void MenuEnterLoad(MENU_s *menu);

void MenuEnterSave(MENU_s *menu);

void MenuInitClips(MENU_s *);

void MenuStartLoad();

void MenuStartSave();




void MenuDrawNoData(MENU_s *menu);

void MenuDrawSaving(MENU_s *);

void MenuUpdateLoad(MENU_s *menu);

void MenuUpdateSave(MENU_s *menu);




void IconScenes_Dump();

void IconScenes_Init(char *path, variptr_u *buf, variptr_u *);

void IconScenes_Load(APICHARACTERMODELLIST_s *list, i32, variptr_u *buf, variptr_u *buf_end);

void MenuDrawLoading(MENU_s *);




i32 MenuIsAvailable();

void MenuUpdateClips(MENU_s *);

void MenuDrawDeleting(MENU_s *);

void MenuDrawFreePlay(MENU_s *);



void MenuInitFreePlay(MENU_s *menu);



void MenuUpdateNoData(MENU_s *menu);

void MenuUpdateLoading(MENU_s *);

void MenuUpdateSaving(MENU_s *);



NUGSCN *IconScene_FindById(i32 character_id);


void MenuDrawFormatting(MENU_s *);

void MenuDrawInsertCard(MENU_s *);

void MenuDrawLoadCancel(MENU_s *menu);

void MenuDrawSaveCancel(MENU_s *menu);

void MenuUpdateDeleting(MENU_s *);

void MenuUpdateFreePlay(MENU_s *);


void MenuDrawCardWarning(MENU_s *);

void MenuDrawFileCorrupt(MENU_s *);

void MenuDrawLoadConfirm(MENU_s *menu);

void MenuDrawSaveConfirm(MENU_s *menu);

void MenuEnterHeaderSave(MENU_s *);

void MenuEnterInsertCard(MENU_s *);

void MenuExitCardWarning(MENU_s *);

void MenuDrawEndChallenge(MENU_s *);

void MenuDrawFormatCancel(MENU_s *menu);

void MenuDrawNoMemoryCard(MENU_s *menu);

void MenuEnterCardWarning(MENU_s *);

void MenuEnterSaveConfirm(MENU_s *);


void MenuUpdateFormatting(MENU_s *);

void MenuUpdateLoadCancel(MENU_s *menu);

void MenuUpdateSaveCancel(MENU_s *menu);

void MenuUpdateInsertCard(MENU_s *menu);

void MenuDrawDeleteConfirm(MENU_s *menu);

void MenuDrawFormatConfirm(MENU_s *);

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuEnterNoMemoryCard(MENU_s *);

void MenuEnterStartNewGame(MENU_s *);

void MenuUpdateCardWarning(MENU_s *menu);

void MenuUpdateFileCorrupt(MENU_s *menu);

void MenuUpdateLoadConfirm(MENU_s *menu);

void MenuUpdateSaveConfirm(MENU_s *menu);

void MenuDrawAutoSaveCancel(MENU_s *menu);

void MenuDrawNotEnoughSpace(MENU_s *);



void MenuUpdateEndChallenge(MENU_s *menu);

void MenuUpdateFormatCancel(MENU_s *menu);

void MenuUpdateNoMemoryCard(MENU_s *menu);

void MenuDrawAutoSaveWarning(MENU_s *menu);

void MenuDrawDoNotRemoveCard(MENU_s *);

void MenuEnterAutoSaveCancel(MENU_s *menu);

void MenuUpdateDeleteConfirm(MENU_s *menu);

void MenuUpdateFormatConfirm(MENU_s *menu);

void MenuEnterAutoSaveWarning(MENU_s *);

extern f32 memcard_autosavecanceldelay;
void MenuUpdateAutoSaveCancel(MENU_s *menu);

void MenuUpdateNotEnoughSpace(MENU_s *menu);



void MenuUpdateAutoSaveWarning(MENU_s *menu);

void MenuUpdateDoNotRemoveCard(MENU_s *menu);

extern "C" {

extern f32 MENUDY;
extern f32 MENUBOTY;
extern f32 MENUTOPY;
extern f32 dme_sx;
extern f32 dme_sy;
extern f32 MENUENTRYEXWIDTH;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUWARNR;
extern u8 MENUWARNG;
extern u8 MENUWARNB;
extern u8 MENUENTRYR;
extern u8 MENUENTRYG;
extern u8 MENUENTRYB;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 dme_r;
extern u8 dme_g;
extern u8 dme_b;
extern i32 dme_align;
extern i32 dme_rgb;
extern f32 menu_pulse;
extern f32 menu_pulse_speed;
extern i32 menu_pulse_angle;
extern f32 menu_pulsate;
extern f32 menu_pulsate_speed;
extern i32 menu_pulsate_angle;
extern void (*headerdrawfn)();
extern void (*predrawfn)(MENU *);


void MenuRememberCursor(MENU *menu);

void BackupMenu(void);

void BackupMenuNoFn(void);

void DrawMenu(i32 paused);

void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha);

void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8);

void DrawMenuEntry(MENU *menu, char *text);

void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha);

void DrawMenuHeader(void);

void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue);

void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue);

void Draw_CANCEL(MENU *menu);

void Draw_CHECKINGMEMORYCARD(void);

void Draw_DONOTREMOVEMEMORYCARD(void);

void Draw_NOTENOUGHSPACE(void);

void Draw_SPACENEEDED(void);

void FlushMenuHighlights(eduimenu_s *menu);

i32 MenuCurrentID(void);

void MenuDrawBackground(void);

i32 MenuGetSlotNum(void);

i32 MenuInCriticalMemoryCard(void);

i32 MenuInMemoryCard(void);

i32 MenuInMemoryCardLoad(void);

i32 MenuInMemoryCardWarning(void);

void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);

void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed);

void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b);

void MenuSetPulsateSpeed(f32 speed);

void MenuSetTopBottom(f32 top, f32 bottom);

void MenuSetPreDrawFn(void (*draw_fn)(MENU *));

void MenuSetHeaderDrawFn(void (*draw_fn)(void));

void MenuReset(void);

void RemapAddr(void *new_base, void *old_base, void **address);

void SetButtonScaleMode(i32 mode);

i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask);

void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

} // extern "C"

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif
void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== src/gameapi/edtools/edui.cpp: all remaining content ====
#include "gameapi/edtools/edui.h"

extern "C" {

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);
}

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

// ==== src/legoapi/actions/character/speederchase.cpp: declarations ====
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legogame/game.h"
#include "decomp.h"
#include "legoapi/actions/character/speederchase.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

#include "legoapi/actions/character/speederchase_types.h"

extern SPEEDERCHASEANETPACKET_s * speederchasea_netpacket;

extern u8 troopercannons_beenReset;
extern i32 players_going_forward;
extern i32 players_cannot_exit_speeder;

extern f32 zoom_ahead_extra;
extern f32 zoom_ahead_extra_time;
extern f32 speeder_level_height_lerpf;
extern f32 speeder_level_height;
extern SPEEDERCHASEVALUES speedervals[4];
extern f32 speederfirerange;
extern f32 speeder_shootrate;
extern f32 speeder_kill_dist;
extern f32 speeder_midrange_time;
extern f32 speeder_ahead_time;
extern f32 speeder_mode_ahead_timer;
DECOMP_ASSERT(sizeof(SPEEDERCHASEVALUES) == 0x1c, "Speeder chase tuning stride");
DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern i32 objopponent_ignoreaiopponent;
extern i32 test_ai_combo;
extern i32 CanPunchGirls;
extern i16 id_GAMORREANGUARD;
extern i16 id_STORMTROOPER;
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);

void PodKeyReset();

void TakeHitRumble(GameObject_s *, f32);

void PodLoseSpeed(GameObject_s *object, i32 hit, i32 rumble);

i32 SpeederBlowupHack(GIZMOBLOWUP_s *blowup, i32);

f32 FindPodHoverHeight(GameObject_s *object);

extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
extern i32 IDLESPEEDINNARROWSOCKSONLY;

i32 ObjIsTargetSpeeder(GameObject_s *object);
f32 GetVehicleSpeedMul(GameObject_s *object, f32 speed);

void SetLevelExBlowupFunc(i32 (*callback)(GIZMOBLOWUP_s *, i32));
void InitTrooperCannons(WORLDINFO_s *world);
void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);
void UpdateTrooperCannons(WORLDINFO_s *world);
i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void SpeederChaseA_Init(WORLDINFO_s *world);

void ProcessCurrentSpeed(WORLDINFO_s *world, speedup_s *speedup);

void SpeederChaseA_Panel(WORLDINFO_s *);

void SpeederChaseA_Reset(WORLDINFO_s *);

i32 SpeedersDroppedBack();

i32 Action_SpeederBeingChased(AISYS_s *, AISCRIPTPROCESS_s *processor, AIPACKET_s *packet, char **, i32, i32 first_time,
                              f32 elapsed);

void SpeederChaseA_Update(WORLDINFO_s *world);

i32 ObjOpponentStillThere(GameObject_s *object, GameObject_s *opponent, f32 gap);

f32 SpeederChaseATATInOutMul(nuvec_s *start, nuvec_s *end);

f32 GetVehicleAreaRememberSpeed();

i32 SpeederChase_ObjIsAGroundTroop(GameObject_s *object);

extern "C" {

extern i32 edmain_auto_speed;

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

} // extern "C"

extern "C" void AddDebrisEffect(i32 *, i32, f32, f32, f32);
extern "C" void DebrisPosOrientationMtx(i32, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void PodDust(WORLDINFO_s *world, GameObject_s *object);

extern "C" f32 pod_roll[2];
f32 getPodRoll(i32 index);

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern "C" void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
extern i32 newgamecam;
i32 GetMenuID(void);
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance_out);
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
void SpecialMove_Cancel(GameObject_s *object);
void Hint_CancelCurrent(void);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
extern VIEWCAM_s ViewCam;
// Original data symbol testmovef is float0.005 at 0x617cb0.
extern f32 testmovef;

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);

void GameCam_ResetLookRot(GAMECAMERA_s *camera);

void GameCam_UpdateLookRot(GAMECAMERA_s *camera);

void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move);

extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern NUVEC ObstacleCamCutPts[2];

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing);

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);

extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" f32 MainRenderTime;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern i32 LevFlag[4];
extern NUMTX cutscenecammtx, CutCamMtx;
extern u8 set_cutscenecammtx;
extern NUGSPLINE *hub_minikitviewer_camspl;
extern AREADATA_s *PODRACE_ADATA;
f32 getPodRoll(i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 PodRace_InStartCountdown(WORLDINFO_s *);
extern "C" i32 editor_active;
extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp);
extern "C" void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
void MakePlayPlanes(GAMECAMERA_s *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
extern f32 HUB_MINIKITVIEWER_CAMDY;
extern f32 PodCamDist;
extern NUVEC ObstacleCamCutPts[2];
extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);


void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void KeepOnScreen(GameObject_s *object);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);

void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

void InitCameraTargetMaterial();

i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void GetTopBot(GameObject_s *object);

extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/gameapi/edtools/edfile.cpp: all remaining content ====
#include "decomp.h"
#include "gameapi/edtools/edfile.h"

#include <string.h>

#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"

extern i32 edfile_handle;
extern i32 edfile_write_flag;


extern char edfile_buffer[4096];
extern i32 edfile_buffer_pointer;
extern i32 edfile_lock[2];

extern i32 edfile_readwrongendianess;
extern i32 edfile_mcresult;

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

void EdFileResetBuffers();

void EdFileFillBuffer();

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

void EdFileFlushBuffer();

i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 MenuLoadStarted;
extern i32 memcard_slot;
extern i32 memcard_cardchanged;
extern i32 memcard_slotsused;
extern i32 memcard_saveneeded;
extern i32 memcard_savestarted;
extern i32 memcard_savefailed;
extern i32 memcard_loadneeded;
extern i32 memcard_loadstarted;
extern i32 memcard_loadfailed;
extern i32 memcard_loadcorrupt;
extern f32 memcard_savemessage_delay;
extern f32 memcard_saveresult_delay;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);
extern i32 numfilteroutblocks;
extern char filteroutblocks[8][12];
extern i32 numfilterblocks;
extern char filterblocks[8][96];

extern char FS_FileList[65536];
extern char FS_LastFileName[64];
extern char FS_Path[256];
extern i32 FS_NumFiles;
extern i32 FS_CurrentPosFileNum;
extern i32 FS_CursorLine;
extern char FS_ParentEntry[8];
extern i32 FS_SortMode;
extern char * FS_CurrentCursorPos;
extern char * FS_CurrentPos;
extern char * FS_FileListEnd;
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);

extern f32 memcard_autosavecanceldelay;
extern f32 memcard_formatmessage_delay;
extern f32 memcard_formatresult_delay;
extern f32 memcard_createmessage_delay;
extern f32 memcard_createresult_delay;
extern f32 memcard_deletemessage_delay;
extern f32 memcard_deleteresult_delay;
extern f32 memcard_message_delay;
extern f32 memcard_result_delay;
extern i32 memcard_justformatted;
extern i32 saveload_autosavedisabled;

void InitMemCard();

i32 SaveGizmoSys(GIZMOSYS_s *, char *, char *);

void SerialiseInt(EdStream &stream, void *data, i32);

void FS_GetDirList(char *path, char *filter, char *filter_out);

void SerialiseChar(EdStream &stream, void *data, i32);

i32 FS_PrevNameLen(char *name);


void FS_SortStrings(char *start, char *end, i32 mode);

void SerialiseFloat(EdStream &stream, void *data, i32);

void SerialiseNuMtx(EdStream &stream, void *data, i32);

void SerialiseNuVec(EdStream &stream, void *data, i32);

void SerialiseShort(EdStream &stream, void *data, i32);

void SerialiseVuMtx(EdStream &stream, void *data, i32);

void SerialiseVuVec(EdStream &stream, void *data, i32);

void FS_MoveCursorUp(i32 steps);

void SerialiseString(EdStream &stream, void *data, i32 count);

void SerialiseColour3(EdStream &stream, void *data, i32);

i32 FS_FileNameFilter(char *name);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

extern "C" void NuRndrRect2d(f32 x, f32 y, f32 z, f32 width, f32 height, i32 colour, NUMTL *material);

extern char * SortStrings[4];

// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();

void FS_MoveCursorDown(i32 steps);

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 getsaveload_status();

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

void SerialiseNuHSpecial(EdStream &stream, void *data, i32);

void SerialiseStringAddr(EdStream &stream, void *data, i32 count);

void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);

void FS_SetCursorToLastFileName();

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

} // extern "C"

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODESUBTITLEY;
extern f32 HUB_EPISODETITLEY;
extern i32 draw_para;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *apiGameName;
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_OK;
extern char *apitxt_GAMELOADED;
extern char *apitxt_GAMELOADING;
extern char *apitxt_FAILEDTOLOAD;
extern char *apitxt_CORRUPTLOAD;
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern i32 menu_flash;
extern f32 text3d_height;
extern f32 text3d_width;
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
extern i32 Paused;
extern "C" i32 screendump;
extern f32 pause_fade;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
extern i32 RemoveDirectionalMaps;
extern i32 RemoveNormalMaps;

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

extern i32 solid_cable;
extern i32 nsegments_drawn;
extern f32 slack_factor;
extern f32 cable_slack;
extern f32 nsegments_per_unit;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

void DrawCables();

void DrawRipple(ripple_node_s *node);

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);

extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);

void Draw_LOADED();

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);

void DrawParallax(nuhspecial_s *special);

void DrawQuestion(nuvec_s *position, float scale_value, float y_push);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);


void Draw_LOADING();

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters);

void DrawArrow_Now(_vum_s *, float, i32, i32);

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha);

void DrawCross_Now(_vuv_s *, float, i32, i32);

void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);



void ClearScreen();



void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters);

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour);

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

void DrawItemMenu2D();

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment);

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

extern nuhspecial_s green_light;
extern nuhspecial_s painttargetcolour[3];
extern nuhspecial_s paintlights[3];

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void Draw_LOADFAILED();

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);

void DrawCameraTarget(nuvec_s *);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

void DrawStatusBG_LSW(STATUSPACKET_s *status);

void Draw_LOADCORRUPT();

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

void DrawCameraTarget2(nuvec_s *);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];
extern i32 currentminikit;
extern f32 slideseek;

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D();

void DrawSubItemMenu3D();

void Draw_NOMEMORYCARD();

void DrawFadeScreenWipe();


void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *);

void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

void Draw_NODATAAVAILABLE();

extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
extern u8 CharClipToBlobShadows;


i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

extern void DrawMenuEntry(MENU_s *, char *);
void Draw_OK(MENU_s *menu);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour);

void DrawArrow(nuhspecial_s *special, float scale_value);

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);





void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);


void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== functions in original address order ====

extern "C" {








i32 cbInteractMenuTitle(edui_interact_s *interact);

i32 cbInteractMenuScrollUp(edui_interact_s *interact);

i32 cbInteractMenuScrollDown(edui_interact_s *interact);


}


extern "C" {
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);


void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);




}



extern "C" {


}






extern "C" {

























void edmainSetReturn(i32 result);



ed_module_s *edmainCurrent(void);

i32 edmainActivate(ed_module_s *module, i32 notify);

void edmainSetCamera(NUMTX *matrix);

NUCAMERA *edmainGetCamera(void);

void edmainExtCamera(NUCAMERA *camera);

void edmainSetCursorEnabled(i32 enabled);

i32 edmainGetCursorEnabled(void);

eduimenu_s *edGetMainMenu(void);

void edmainRegisterLocVec(NUVEC *position);

NUVEC *edmainQueryLocVec(void);

void eduiSetGlobalSliderAccel(f32 acceleration);

void eduiInitMaterials(void);

void eduiSetRenderPlane(i8 plane);

eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);

void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);

i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);

i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);

void eduiMenuSortItemsByTxt(eduimenu_s *menu);

void eduiMenuSelectFirstEntry(eduimenu_s *menu);

void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);

f32 eduiGetAnalougePadValue(nupad_s *pad);

eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);

void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);

void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);

void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);


i32 eduiItemSetText(eduiitem_s *item, char *text);

}



extern "C" {
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);

eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);

eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);

void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);

eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

i32 cbCompateDirentByDateAsc(const void *first, const void *second);

i32 cbCompateDirentByDateDec(const void *first, const void *second);

i32 cbCompateDirentBySizeAsc(const void *first, const void *second);

i32 cbCompateDirentBySizeDec(const void *first, const void *second);

i32 cbCompateDirentByNameAsc(const void *first, const void *second);

i32 cbCompateDirentByNameDec(const void *first, const void *second);

void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);

eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);

void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);

edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);

void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);


i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);

eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);

void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);

void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);

}


extern "C" {
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);

void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);

void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);

void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);

void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);

void eduiItemSliderSetVal(edui_slider_s *item, f32 value);

i32 eduicbInteractSlider(edui_interact_s *interact);

void eduiItemSliderSetFmt(edui_slider_s *item, char *format);

void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);

eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);

eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);

eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);

i32 eduiItemPropSetText(edui_prop_s *item, char *text);

i32 eduiGetCameraEnabled(void);

void eduiSetCameraEnabled(i32 enabled);

i32 NuKeyToAscii(u32 key, i32 shifted);



eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);

eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);

eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);

void eduicbMenuOpenAllexpanders(eduimenu_s *menu);

void eduicbMenuCloseAllexpanders(eduimenu_s *menu);

void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);

f32 edmainSetMainMenuScale(f32 scale);

extern "C" void eduiMenuDestroyItems(eduimenu_s *menu);

void eduiMenuDestroy(eduimenu_s *menu);

void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);


void edmainClose(void);

i32 edmainRegister(ed_module_s *module);

void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);

void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);

eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);

void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);

void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);


eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);

i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);

void eduiSetUsingMenuFocus(i32 enabled);

i32 eduiGetUsingMenuFocus(void);

void eduiSetDefaultActiveMenu(eduimenu_s *menu);

void eduiSetActiveMenu(eduimenu_s *menu);

i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);


void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);

void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);




edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);

void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);

void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);



eduimenu_s *eduiGetActiveMenu(void);

void eduiMenuRender(eduimenu_s *menu);

void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);



i32 eduiMenuDetach(eduimenu_s *menu);

}


extern "C" {
void cbCancelSubMenu(eduimenu_s *menu, eduimenu_s *);









__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);



eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);

eduimenu_s *eduiGetActiveMenuParent(void);

i32 eduiClearActiveMenu(void);

i32 eduiMenuIsActive(eduimenu_s *menu);

i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiMenuEnsureSelection(eduimenu_s *menu);

i32 eduiCursorOverMenu(eduimenu_s *menu);

i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);

void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);

void cbInteractMenuKeySelect(eduimenu_s *menu);

i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);

void eduiFlushInteracts(void);

i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);

i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);

void eduiRenderInteracts(void);

void eduiShowCursor(i32 show);

void eduiGetCursorCoords(f32 *x, f32 *y);

void eduiSetCursorCoords(f32 x, f32 y);

void eduiGetCursorDelta(f32 *x, f32 *y);

i32 eduiUsedAlgPad(nupad_s *pad);

void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);

void eduiProcessCursor(f32 delta_time, nupad_s *pad);

i32 edmainProcess(f32 delta_time, nupad_s *pad);

void eduiSetCursorColour(u32 colour);

void eduiRenderCursor(void);

void edmainRender(void);

void eduiInit(void);

void edmainInit(void *font, char *configuration_file);

void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);

void eduiSetFont(NUQFNT *font);

void eduiSetFontScale(f32 x, f32 y);

void CreateTestMenu(void *font);

void TestMenu(nupad_s *pad);

eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);

void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);

void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);


void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);

void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *item, u32);

void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);

void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);

}

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

extern "C" {
void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

void edcamMoveEx(nupad_s *pad, f32 delta_time);

void edcamMove(nupad_s *pad);

void edcamSetPos(NUVEC *position);

void edcamSetAng(i32 pitch, i32 yaw);

void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);

void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);

void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);

}

void edcamSetContoller(i32 invert_pitch);

extern "C" {
void edcamSetDist(f32 distance);

f32 edcamGetDist(void);

edcam_s *edcamGetEdCam(void);

void edcamSetOffset(NUVEC *offset);

void edcamGetOffset(NUVEC *offset);

void edcamMtx(NUMTX *matrix);

void edcamSet(void);

NUVEC *edcamGetPosPointer(void);

}

void EdFileSwapEndianess16(void *data);

void EdFileSwapEndianess32(void *data);

void EdFileSetReadWrongEndianess(i32 value);

void EdFileSetMedia(i32 media);

void EdFileSetPakFile(void *pak);

void EdFileResetBuffers();

void EdFileFillBuffer();

i32 EdFileOpen(char *filepath, NUFILEMODE mode);

void EdFileFlushBuffer();

i32 EdFileClose();

void EdFileWrite(void *data, i32 len);

void EdFileWriteFloat(f32 value);

void EdFileWriteInt(i32 value);

void EdFileWriteUnsignedInt(u32 value);

void EdFileWriteShort(i16 value);

void EdFileWriteUnsignedShort(u16 value);

void EdFileWriteChar(char value);

void EdFileWriteUnsignedChar(u8 value);

void EdFileWriteNuVec(NUVEC *value);

void EdFileRead(void *buf, i32 len);

f32 EdFileReadFloat();

i32 EdFileReadInt();

u32 EdFileReadUnsignedInt();

i16 EdFileReadShort();

u16 EdFileReadUnsignedShort();

char EdFileReadChar();

unsigned char EdFileReadUnsignedChar();

void EdFileReadNuVec(NUVEC *out);

i32 EdFileWriteMemCard(char *filename, i32 size, void *data);

i32 EdFileReadMemCard(char *filename, i32 size, void *data);

i32 EdFileBackup(char *source, char *destination);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);


void FS_MoveCursorDown(i32 steps);

i32 FS_PrevNameLen(char *name);

void FS_MoveCursorUp(i32 steps);

void RenderFileSel3(i32 draw_title);

extern "C" {
extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

}

void RenderFileSel();

f32 FS_GetDirTextWidth();

char *FS_GetFilterString(char *input, char *output);

i32 FS_FileNameFilter(char *name);

void FS_BuildFilterBlocks(char *input);

void FS_BuildFilterOutBlocks(char *input);

void FS_SortStrings(char *start, char *end, i32 mode);

void FS_SetCursorToLastFileName();

void FS_GetDirList(char *path, char *filter, char *filter_out);

i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);

i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" {
extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

}

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" {
void FS_SetFileSelPathFromName(char *name);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

eduiitem_s *eduiItemFileSelectorCreate(u32 data, eduiiattr_s *colours, EdUiItemCallback callback, char *text);

extern "C" {
void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);

void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

}

void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);

extern "C" {
extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" void CreateColourPicker(void);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

void FlushMenuHighlights(eduimenu_s *menu);

}

// ==== src/nu2api/nucore/nucore_plain.cpp: declarations ====
// Temporary C-linkage compatibility surface for unresolved engine owners.
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec4.h"
extern "C" {
extern NULGTARCMATERIAL NuLgtArcMtl[4];
extern i32 NuLgtLaserOldCnt;
}
#include "nu2api/nucore/nuonline.h"
//
// This file collects functions whose original TU has not yet been
// reconstructed. Domain families move to their evidenced owner as complete
// groups; this file is not itself evidence of one original nucore TU.

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nutexanm.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nuqfnt.h"

void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
extern NUQFNT *system_qfont;
void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *control, f32 width, f32 segment_length, f32 wobble,
                     f32 duration, i32 colour, i32 flags);

#include "decomp.h"
#include "java/android.h"
#include "java/java.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/NuInputManager.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nucore/NuDynamicLight.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/numath/numtx.h"
#include "globals.h"

extern NuInputManager *inputManager;
extern bool used_touch_IDs[10];

struct ani3_animheader_s;
struct nuanimdatachunk_s;
nuanimdatachunk_s *NuAnimDataChunkCreate(i32 curve_set_count);

extern "C" {

extern u8 CutSceneBoundingBoxTrackRoot;
extern i32 NuGCutAudioStream;
extern NUANIMBUFFEVALUATECB AnimBuffEvalCB;
extern void ** AnimBuffEvalData;
extern i32 * AnimBuffEvalJoint;
extern GLuint g_colorRenderbuffer;
extern GLuint g_depthRenderbuffer;
}


using NUHGOBJVIDEOMEMFN = void (*)(nuhgobj_s *);

extern NUHGOBJVIDEOMEMFN hgobj_to_video_mem;
extern NUHGOBJVIDEOMEMFN video_mem_to_hgobj;

extern "C" {
    extern NUGLOBALRNDRSTATE render_state;
    void RndrStateSetConstAlphaTint(i32 alpha_enabled, i32 tint_enabled, f32 alpha, const NUCOLOUR3 *tint, NUMTL *mtl);
}

// C++-linkage helpers defined in sibling TUs.
void NuPadRecordEndFrame(void);                      // nupad_interface.cpp
void bgSuspendMain(i32);                             // main.cpp
void NuAnimBuffInit(i32, VARIPTR *, VARIPTR);        // nu2api_nucore_misc.cpp
i32 NuCameraClipHGobj(nugscn_s *, NUMTX *, NUMTX *); // nu2api_nucore_misc.cpp
i32 findrange(nugscn_s *, i32);                      // utilities.cpp

extern "C" {

    // ---------------------------------------------------------------------------
    // Display-list bootstrap (original nucore TU file-statics)
    // ---------------------------------------------------------------------------

    i32 NuHasError(void);
    void NuMtlAnimate(f32 frame_time);
    void NuTexAnimProcess(f32 frame_time);
    void NuShaderManagerSetfv(i32 semantic, const f32 *values);

    // ---------------------------------------------------------------------------
    // Redirected symbols — real bodies live elsewhere (kept as comments)
    // ---------------------------------------------------------------------------

    // Nu360_dxClear(u32,u32) is implemented in ios_graphics.cpp (original 0x317070).
    // NuDisplayListSwapBuffersBeginFrame / EndFrame are transcribed in full
    // in nudlist.cpp (originals 0x2eb5d0 / 0x2eaef0).
    // NuHtmlBegin(void*) is transcribed in legoapi/misc/supportall.cpp (original 0x2d5ca0).
    // NuIOS_SetVertexFormat is transcribed in nuiosdl_gl.cpp (original 0x29c070).
    // NuIOS_WaitForRenderThreadCompletion is transcribed in ios_graphics.cpp (original 0xe34b0).
    // NuIOS_WakeRenderThread is transcribed in ios_graphics.cpp (original 0xe3590).
    // NuRenderContextSetZFunc is transcribed below (original 0x2a3860).

    // ---------------------------------------------------------------------------
    // Camera (thin wrappers; full matrix work is in NuCameraSetEx)
    // ---------------------------------------------------------------------------

f32 NuCameraCalcAperture(f32 focal_length, f32 root_fstop);
void NuCameraCalcClipMtx(NUCAMERACLIP *clip, NUCAMERA *camera, i32 use_cached_projection);
f32 NuCameraCalcRootFStop(f32 focal_length, f32 aperture);
void NuCameraClearStateBuffer(void);
i32 NuCameraClipTestExtentsGeneric(NUVEC *min, NUVEC *max, NUMTX *world_mtx, f32 far_clip, i32 flags,
                                       f32 *min_depth);
i32 NuCameraClipTestPointScissor(NUVEC *point);
i32 NuCameraClipTestPointVport(NUVEC *point);
i32 NuCameraClipTestPoints(NUVEC *points, i32 count, NUMTX *world_mtx);
f32 NuCameraFOVToFocalLen(f32 fov);
f32 NuCameraFocalLenToFOV(f32 focal_length);
void NuCameraForceFarclip(i32 enabled);
void NuCameraGetAxes(NUVEC *axes);
NUMTX *NuCameraGetClippingMtx(void);
void NuCameraGetClippingRatios(f32 *horizontal, f32 *vertical);
NUMTX *NuCameraGetPCMtx(void);
NUMTX *NuCameraGetPCSMtx(void);
NUMTX *NuCameraGetVPCSMtx(void);
i32 NuCameraIntersectsAABB(NUVEC *center, NUVEC *extent, f32 far_clip, i32 use_scissor);
extern i32 prev_lock;
extern NUCAMERA locked_camera;
extern NUCAMERA cam_copy;
extern i32 camfx;
extern NuCameraReflect global_reflect;
    DECOMP_ASSERT(sizeof(NuCameraReflect) == 0x84, "camera reflection state size");
    DECOMP_ASSERT(__builtin_offsetof(NuCameraReflect, mtx) == 4, "camera reflection matrix offset");

void NuCameraLock(i32 lock);
    extern nurenderscene_s currentScene;
extern i32 NuRndrDoingScreenGrab;
extern i32 motionBlurAccumActiveThisFrame;

void NuCameraMotionBlurEffect(const NUMTX *previous, const NUMTX *current, f32 scale, f32 maximum, f32 falloff);
void NuCameraMotionBlurParams(const NUMTX *current);
void NuCameraRelock(void);
i32 NuCameraSaveState(void);
void NuCameraSet(NUCAMERA *cam);
void NuCameraSetAxes(NUVEC *axes);
    extern i32 PS2_REZ_W;
    extern i32 PS2_REZ_H;
extern i32 PS2_SREZ_W;
extern i32 PS2_SREZ_H;
void NuCameraSetEx(NUCAMERA *cam, i32 fast);
void NuCameraSetReflect(NUCAMERA *camera, NuCameraReflect *reflect);
void NuCameraSetScissorClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraSetVPortClipMtx(NUMTX *out, NUMTX *view, NUCAMERA *camera, i32 fast);
void NuCameraTransformScreen(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenClip(NUVEC *screen, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraTransformScreenVU0(NUVEC4 *screen, NUVEC4 *world, i32 count, NUMTX *matrix);
void NuCameraTransformView(NUVEC *view, NUVEC *world, i32 count, NUMTX *matrix);
void NuCameraUnlock(void);

    // ---------------------------------------------------------------------------
    // Display list — faithful helpers + pending stubs
    // ---------------------------------------------------------------------------

    void NuDisplayListBeginCriticalSection(void);
    void NuDisplayListEndCriticalSection(void);

void NuDisplayListAnimateMtls(f32 frame_time);
void NuDisplayListBeginCriticalSection(void);
void NuDisplayListClipSpecials(i32 enabled);
    void DisplayListCreateFxItemPS(void *item, i32 type);
void DisplayListCreateFxList(VARIPTR *buffer, VARIPTR end, i32 count);
i32 NuDisplayListCreateFx(i32 type, i32 priority, i32 layer);
void NuDisplayListCreateMtl(NUMTL *mtl);
    void DisplayListDestroyFxItemPS(void *item);
void NuDisplayListDestroyFx(i32 handle);
void NuDisplayListDestroyMtl(NUMTL *mtl);
void NuDisplayListDrawAll(void);
void NuDisplayListEndCriticalSection(void);
} // extern "C"

// Local helpers matching original static display-list setters (t local symbols)

extern "C" {
void NuDisplayListLinkMtl(nudisplaylist_s *list, NUMTL *mtl);
void NuDisplayListLinkList(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *first, NUDISPLAYLISTITEM *last);
    void *DisplayListCreateFaceonTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *faceon);
    void *DisplayListCreateGeomTransformPS(VARIPTR *buffer, NUMTX *transform, NUMTL *mtl, void *next, void *tx);

    extern "C++" {
        // Original local helper cloned as DisplayListProcessSkin.isra.36 by GCC.
        // It appends the world transform, skin palette, and geometry calls as one
        // contiguous run in the material display list.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        // Original local helper cloned as DisplayListProcessLightmapped.isra.34.
        // Lightmapped geometry carries an AE/AF/B0 setup command two entries before
        // its geometry command. Keep that setup command adjacent to the dynamic
        // transform and geometry entries when the special is submitted.
        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
    NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
    void WindShear(NUMTX *out, NUMTX *in, i32 wind_scale, i32 wind_speed);

i32 NuDisplayListRndrSpecial(nuhspecial_s *special_handle, NUMTX *mtx, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void DisplayListSetFxItemParamPS(void *item, i32 parameter, f32 value, i32 mode);
void NuDisplayListSetFxParam(i32 handle, i32 parameter, f32 value, i32 mode);

    // ---------------------------------------------------------------------------
    // Scene / render-scene
    // ---------------------------------------------------------------------------

void NuDisplaySceneDebug(NUDLDLISTSCENE *scene, i32 flags, i32 depth, i32 *state);

    // ---------------------------------------------------------------------------
    // Frame lifecycle (NuFrameEnd is faithful; neighbours are stubs)
    // ---------------------------------------------------------------------------

    // Per-frame animation/pad update hooks (originals take the frame time).
    void NuPad_Interface_Render(void);
    void NuPadUpdatePads(void);

    // Frame-end callbacks (original bss @0x6bdaec/0x6bdaf0/0x6bdad0).
    extern void (*preRenderFlashingHack)(void);
    extern void (*postRenderFlashingHack)(void);
    extern void (*nuapi_endframe_callbackfn)(void);

    // Faithful transcription of the original frame-end pump. Waits for the
    // target frame interval, ticks material/tex/wind anims, swaps screens
    // via NuRndrSwapScreenEx, then advances nuapi clocks and pad state.

    // ---------------------------------------------------------------------------
    // iOS / platform
    // ---------------------------------------------------------------------------

void NuIOS_AwardAchievement(void);
void NuIOS_CheckCurrentFramebuffer(void);
    void NuIOS_DeallocateSystemRenderbuffer(GLuint renderbuffer);
void NuIOS_DeallocateSystemFramebuffers(void);
void NuIOS_DeallocateSystemRenderbuffer(GLuint);
    // Locale string is owned by the Android/JNI TU; cache only its index here.
extern i32 g_languageIndex;

i32 NuIOS_GetDeviceLanguage(void);
i32 NuIOS_HardwareSupportsRetina(void);
i32 NuIOS_IsLowestEndDevice(void);
i32 NuIOS_IsMidRangeDevice(void);
i32 NuIOS_IsSmallScreen(void);
void NuIOS_RecordFlurryEvent(char *event_name);
void NuIOS_ShowAchievements(void);

    // ---------------------------------------------------------------------------
    // Animation / curves / data
    // ---------------------------------------------------------------------------

void NuAnimBuffAccumulate_3(nuanimbuff_s *buffer, ani3_animheader_s *animation, f32 time, i32 overwrite, f32 blend,
                                i32 first_joint, nuhgobj_s *object, NUVEC *root_translation);
void *NuAnimBuffCreate(i32 max_joints, VARIPTR *buf, VARIPTR);
void NuAnimBuffCreateScratch(nuanimbuff_s *buffer);
void NuAnimBuffDestroyScratch(nuanimbuff_s *buffer);
void NuAnimBuffEvaluateCallback(NUANIMBUFFEVALUATECB callback, void **data, i32 *joints);
void NuAnimBuffEvaluate_3(nuanimbuff_s *buffer, nuhgobj_s *object, NUMTX *matrices, ani3_animheader_s *animation,
                              NUHGOBJROOTFN root_fn, NUVEC *root_translation, void *root_data);
void NuAnimBuffProceduralAnimation(nuanimbuff_s *buffer, nuhgobj_s *object, i32 override_count,
                                       NUJOINTANIM_s *overrides);
void NuAnimCurve2SetApplyToJointTransLoc(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                             nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                             NUJOINTANIM_s *override_anim, NUVEC *root_translation,
                                             NUVEC *locator_translation);
void NuAnimCurve2SetApplyToJoint(nuanimcurve2_s *curves, i8 *types, i8 flags, nuanimtime_s *time,
                                     nuhgobjjoint_s *joint, NUVEC *scale, NUVEC *parent_scale, NUMTX *matrix,
                                     NUJOINTANIM_s *override_anim);
void NuAnimCurve2SetApplyToMatrix_3(ani3_animheader_s *animation, i32 node, f32 frame, NUMTX *matrix);
void *NuAnimCurveCreate(void);
void NuAnimCurveDestroy(void *curve);
f32 *NuAnimCurveExtractAllNodeCurves_3(ani3_animheader_s *anim, i32 node, f32 frame, char *curve_mask);
void NuAnimCurveSetApplyBlendToJoint2(nuanimcurveset_s *first, nuanimtime_s *first_time, nuanimcurveset_s *second,
                                          nuanimtime_s *second_time, f32 blend, nuhgobjjoint_s *joint, NUVEC *scale,
                                          NUVEC *parent_scale, NUMTX *matrix, NUJOINTANIM_s *override_anim);
void NuAnimCurveSetApplyToMatrix(nuanimcurveset_s *set, nuanimtime_s *time, NUMTX *matrix);
void *NuAnimCurveSetCreate(i32 curve_count);
void NuAnimCurveSetDestroy(void *curve_set, i32 destroy_curves);
void NuAnimData2CalcMatrix(nuanimdata_s *animation, i32 node, f32 frame, numtx_s *matrix);
void NuAnimData2CalcTime(nuanimdata2_s *anim, f32 frame, nuanimtime_s *time);
void NuAnimDataCalcTime(void *animation, f32 frame, nuanimtime_s *time);
void *NuAnimDataCreate(i32 chunk_count);
    extern "C++" void NuAnimDataChunkDestroy(nuanimdatachunk_s *chunk);

void NuAnimDataDestroy(void *animation);
i32 NuAnimDataFindVersion(char *path);
void *NuAnimDataLoadBuff(char *path, VARIPTR *buf, VARIPTR *buf_end);
void *NuAnimGetAnimLOD(void *animation, i32 lod);
i32 NuAnimGetUseQuatsFlag(void);
i32 NuAnimNumNodes(void *animation);
i32 NuAnimPopUseQuatsFlag(void);
i32 NuAnimPushSetUseQuatsFlag(i32 enabled);
i32 NuAnimSetUseQuatsFlag(i32 enabled);

    // ---------------------------------------------------------------------------
    // Bridge / file / heap / memory
    // ---------------------------------------------------------------------------

void *NuMemAllocFn(u32 size);
void NuMemFreeFn(void *ptr);
void *NuMemReAllocFn(void *ptr, u32 size);


i32 NuSysDirClose(i32 index);
i32 NuSysDirOpen(void);
i32 NuSysDirRead(void);

    // ---------------------------------------------------------------------------
    // Math / geometry
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Quick-font platform rendering (the generic font run lives in nuqfnt.cpp)
    // ---------------------------------------------------------------------------

VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *);
void NuQFntDestroy(VUFNT *font);
void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
u32 NuQFntGetPrintMode(void);
f32 NuQFntHeightScale(void);
f32 NuQFntLenScale(void);
void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
void NuQFntPopCoordinateSystem(void);
void NuQFntPrint2dU(NUQFNT *font, char *text);
void NuQFntPrint2dW(NUQFNT *font, u16 *text);
void NuQFntPrint3DU(NUQFNT *font, char *text);
void NuQFntPrint3DW(NUQFNT *font, u16 *text);
void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 alignment, const char *format, ...);
f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPrintV(NUQFNT *font, const char *format, va_list arguments);
void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
void NuQFntSet2d(NUQFNT *font);
void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
void NuQFntSetPrintMode(u32 mode);
void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);

    // ---------------------------------------------------------------------------
    // Rendering / materials / effects (host has GL paths elsewhere)
    // ---------------------------------------------------------------------------

void NuAccumulationMotionBlurEffect(i32 frames, f32 blend, i32 mode);
void NuAccumulationMotionBlurParams(i32 frames, f32 blend, i32 mode);
    extern nurenderscene_s currentScene;
void NuBackbufferCopy(i32 texture_id);
void NuDeferredShadingRender(void);
void NuDeferredShadingSetParameterf(i32 parameter, f32 value);
void NuDepthOfFieldEffect(f32 strength, f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect1(f32 near_distance, f32 far_distance);
void NuDepthOfFieldEffect2(f32 near_distance, f32 far_distance, f32 strength);
void NuDepthOfFieldEffectEx(const NuDepthOfFieldParameters *parameters);
extern i32 speedblur_enabled;
extern f32 NuLightsx;
extern f32 NuLightsy;
void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void NuLightSpeedBlurScale(f32 x, f32 y);
extern NULGTARCLASER NuLgtArcLaserData[16];
extern i32 NuLgtArcLaserOldCnt;
void NuLgtArcLaser(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                       f32 bend_amount, i32 colour);
extern i32 NuLgtLaserCnt;
extern i32 NuLgtArcLaserCnt;
extern i32 NuLgtArcLaserFrame;
extern NULGTLASER NuLgtLaserData[64];

void NuLgtLaser(i32 type, f32 width, f32 segment_length, f32 width_wobble, NUVEC *start, NUVEC *delta, u32 colour,
                    f32 end_width, f32 length);
void NuLgtLaserDraw(i32 paused);

void NuLgtSetArcMat(NUMTL *material, f32 u0, f32 v0, f32 u1, f32 v1);
void NuPostBloom(i32, const NuBloomParameters *parameters);
void NuSpecialBurstDrawAt(nuhspecial_s *special, u32 count, NUMTX *matrices, i32 clip);
i32 NuSpecialDrawAt(void *special, NUMTX *mtx);
i32 NuSpecialDrawSmoothSkin(void *special, NUMTX *skin_matrices, NUMTX *world_matrix);
i32 NuSpecialDrawSmoothSkinDwa(void *special, NUMTX *skin_matrices, NUMTX *world_matrix,
                                   DEFORMERWEIGHTSARRAY *blend_values);
i32 NuSpecialDrawWith(void *special, NUMTX *mtx);
void NuSpecialSetDrawMtx(void *special, NUMTX *mtx);

    // ---------------------------------------------------------------------------
    // Light / wind / particles / debris
    // ---------------------------------------------------------------------------

void NuDynamicLightAddRenderScene(NuDynamicLight *light, i32 index, i32 scene_id);
void NuDynamicLightAddShadowCasterScene(NuDynamicLight *light, nugscn_s *scene);
void NuDynamicLightBeginCapture(NuDynamicLight *light, i32 render_set);
NuDynamicLight *NuDynamicLightClone(NuDynamicLight *light, VARIPTR *arena, VARIPTR end);
NuDynamicLight *NuDynamicLightCreate();
void NuDynamicLightDestroy(NuDynamicLight *light);
void NuDynamicLightEndCapture(void);
i32 NuDynamicLightGetActiveRenderSetCount(NuDynamicLight *light);
NUDISPLAYLIST *NuDynamicLightGetDList(NuDynamicLight *light, i32 render_set, NUMTL *material);
f32 NuDynamicLightGetParameterf(NuDynamicLight *light, i32 parameter);
i32 NuDynamicLightGetParameteri(NuDynamicLight *light, i32 parameter);
NUMTX *NuDynamicLightGetProjection(NuDynamicLight *light);
NUMTX *NuDynamicLightGetView(NuDynamicLight *light);
i32 NuDynamicLightIsUsedOnSpecials(NuDynamicLight *light);
void NuDynamicLightLookAt(NuDynamicLight *light, NUVEC *eye, NUVEC *target, NUVEC *up);
void NuDynamicLightResetGeometry(NuDynamicLight *light);
void NuDynamicLightSetDirectional(NuDynamicLight *light, f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane);
void NuDynamicLightSetEnabled(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetParameterf(NuDynamicLight *light, i32 parameter, f32 value);
void NuDynamicLightSetParameteri(NuDynamicLight *light, i32 parameter, i32 value);
void NuDynamicLightSetUsedOnSpecials(NuDynamicLight *light, i32 enabled);
void NuDynamicLightSetupCustomCameraFrustum(NuDynamicLight *light, NUCAMERA *camera, const f32 *splits, i32 count);
i32 NuDynamicLightTestShadowExtrusionExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent,
                                                i32 render_set);
void NuDynamicLightTestShadowExtrusions(NuDynamicLight *light, const VuVec *first, const VuVec *second);
void NuDynamicLightTestShadowExtrusionsExtent(NuDynamicLight *light, const NUVEC *center, const NUVEC *extent);

i32 NuWindLoad(NUWIND *wind, i32 index, char *name, VARIPTR *buffer, VARIPTR buffer_end);

void NuWindSetCurrent(NUWIND *wind, i32 index);

void NuWindUnload(NUWIND *wind, i32 index);
    void NuWindUpdateArray(NUVEC **);

    extern "C" f32 partglobaltime;
void NuPartResetGlobalTime(void);
    // ---------------------------------------------------------------------------
    // Gobj / hierarchy / scene graph
    // ---------------------------------------------------------------------------

void NuHGobjDestroy(nuhgobj_s *object);
void NuHGobjEval(nuhgobj_s *object, i32 override_count, nuhgobjjointoverride_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnim(void);
void NuHGobjEvalAnim2(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                          NUJOINTANIM_s *overrides, NUMTX *matrices);
extern i32 ddmaxjoints;
void NuHGobjEvalAnim2Root(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                              NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);

void NuHGobjEvalAnim2Root_3(nuhgobj_s *object, ani3_animheader_s *animation, f32 time, i32 override_count,
                                NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
#include "nu2api/nucore/nucore_plain_types.h"
void NuHGobjEvalAnimBlend(nuhgobj_s *object, void *animation_a, f32 frame_a, void *animation_b, f32 frame_b,
                              f32 blend, i32 override_count, NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                               ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                               NUJOINTANIM_s *overrides, NUMTX *matrices);
void NuHGobjEvalAnimBlend2Root(nuhgobj_s *object, ani3_animheader_s *animation_a, f32 time_a,
                                   ani3_animheader_s *animation_b, f32 time_b, f32 blend, i32 override_count,
                                   NUJOINTANIM_s *overrides, NUMTX *matrices, NUHGOBJROOTFN root_fn, void *root_data);
void **NuHGobjEvalDwa(i32 render_count, i16 *render_indices, void *animation, f32 frame);
void **NuHGobjEvalDwa2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation, f32 frame);
void **NuHGobjEvalDwaBlend(i32 render_count, i16 *render_indices, void *animation_a, f32 frame_a, void *animation_b,
                               f32 frame_b, f32 blend);
void **NuHGobjEvalDwaBlend2(i32 render_count, i16 *render_indices, nuanimdata2_s *animation_a, f32 frame_a,
                                nuanimdata2_s *animation_b, f32 frame_b, f32 blend);
i32 NuHGobjForceShadowsOnCharacters(i32 enabled);
void NuHGobjFromVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuHGobjJointMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
void NuHGobjPOIMtx(nuhgobj_s *object, u8 index, NUMTX *world_matrix, NUMTX *joint_matrices, NUMTX *result);
i32 NuHGobjReversibleCharacters(i32 enabled);
i32 NuHGobjRndrMtxDwa(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                          NUMTX *joint_matrices, void **blend_values, i32);
    u32 NuWindRand(void);
i32 NuHGobjRndrRandShadowSurfacePoints(nuhgobj_s *object, NUMTX *world_matrix, NUMTX *joint_matrices, i32 count,
                                           NUVEC *positions, i32 exclusion_mask);
void NuHGobjSetClippingRootTrackerOverride(i32 enabled);
void NuHGobjToVideoMem(NUHGOBJVIDEOMEMFN callback);
void NuGCutCharAnimProcess(NUGCUTCHAR_s *character, f32 frame, NUMTX *matrix, i32 *visible, u32 *animation_index,
                               f32 *animation_rate, f32 *blend_time, f32 *animation_start_frame, i32 *layer_mask);
void NuGCutSetCutAudioStream(i32 stream);
    void NuTexAnimRemoveList(void *texture_anims);
    void NuGHGPreRelocateFixupPS(NUGSCN *scene);
    void NuGHGPostRelocateFixupPS(NUGSCN *scene, i32 delta);
NUGSCN *NuGHGRelocate(NUGSCN *scene, VARIPTR *destination);

    // ---------------------------------------------------------------------------
    // Input / pad / key / mouse
    // ---------------------------------------------------------------------------

extern NUKEYCHAR NuKeyChar[65];
i32 NuKeyToAscii(u32 key, i32 shifted);
void NuPad_Interface_TouchScreenInput(i32 x, i32 y, i32 prior_x, i32 prior_y, i32 is_down, i32 is_up, i32 is_move,
                                          i32 is_cancelled);
    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    i32 clipTestSphere(NUPORTALSPHERE *sphere, NUFRUSTRUM *frustum);

i32 NuPortalClipTest(NUGSCN *scene, NUVEC *position, f32 radius, i16 room_id);
i32 NuPortalEnabled(i32 enabled);
i32 NuPortalNumRooms(NUGSCN *scene);
void NuPortalResetActive(NUGSCN *scene);
i32 NuPortalRoomClipTest(NUGSCN *scene, i16 room_id);
void NuPortalRoomClipTestAll(NUGSCN *scene, u8 *room_visibility);
i32 NuPortalWhichRoom(NUGSCN *scene, NUVEC *position);
extern i32 VisiSysCameraLock;
extern i32 LoadedOcclusionData;
extern i32 UsingOcclusionData;
extern i32 CurrentViewBox;
extern u8 OcclusionBitArray[800];
extern i32 do_InstTree;
extern i32 do_occlusion;
extern i32 do_visibility;
extern i32 do_octree;

    void NuVisiInstTree(void *, NUGSCN *);
    void NuVisiOcclusion(NuVisibilityResult *);

void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
    DECOMP_ASSERT(sizeof(NuInstanceTreeNode) == 0x0c, "instance tree node stride");
    DECOMP_ASSERT(sizeof(NuInstanceTreeBounds) == 0x20, "instance tree bounds stride");
    DECOMP_ASSERT(sizeof(NuInstanceTree_s) == 0x28, "instance tree size");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, nodes) == 0x14, "instance tree nodes offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, roots) == 0x20, "instance tree roots offset");
    DECOMP_ASSERT(offsetof(NuInstanceTree_s, far_clips) == 0x24, "instance tree far clips offset");
extern NuVisibilityResult * myvisi;
    extern "C++" {
        // Retail only has the IPA-SRA clone (the unused scene and bits arguments dropped).
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }
void NuVisiInstTree(void *visibility_result, NUGSCN *scene);

void NuVisiOcclusion(NuVisibilityResult *result);
    extern "C++" void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled);
void NuVisiOctree(NuVisibilityResult *result);
void NuOcclusionManagerAddOccluderOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
void NuOcclusionManagerAddOccluderQuad(const NUVEC *a, const NUVEC *b, const NUVEC *c, const NUVEC *d);
void NuOcclusionManagerAddOccluderSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerEndFrame(void);
void NuOcclusionManagerInit(u32 capacity, VARIPTR *buffer, VARIPTR buffer_end);
bool NuOcclusionManagerIsEnabled(void);
bool NuOcclusionManagerIsInitialised(void);
i32 NuOcclusionManagerIsOccludedOBB(const NUVEC *minimum, const NUVEC *maximum, const NUMTX *matrix);
i32 NuOcclusionManagerIsOccludedSphere(const NUVEC *center, f32 radius);
void NuOcclusionManagerOnCameraSet(void);
void NuOcclusionManagerRenderStats(void);
void NuOcclusionManagerRenderZPass(void);
void NuOcclusionManagerSetEnabled(i32 enabled);
void NuOcclusionManagerSetOccluderDotProductThreshold(f32 threshold);
void NuOcclusionManagerSetOccluderScreenSpaceThreshold(f32 threshold);
    // ---------------------------------------------------------------------------
    // Viewport
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Strings / conversion / Unicode
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Containers / lists / params
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Debug / error / profiling
    // ---------------------------------------------------------------------------

    // ---------------------------------------------------------------------------
    // Thread / misc OS
    // ---------------------------------------------------------------------------

void NuPause(i32 paused);
    // ---------------------------------------------------------------------------
    // Spline / online / net / other gameplay support
    // ---------------------------------------------------------------------------

void NuSplineList(void);
    extern void (*nuapi_endframe_callbackfn)(void);
void NuRegisterEndFrameCallBackFn(void (*callback)(void));

    // ---------------------------------------------------------------------------
    // Rendering extras
    // ---------------------------------------------------------------------------

i32 NuStreamInit(void);

    // ---------------------------------------------------------------------------
    // PS2 / Xbox legacy shims
    // ---------------------------------------------------------------------------

} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

void NuLgtSetArcMatEx(i32 type, numtl_s *material, f32 u0, f32 v0, f32 u1, f32 v1);

void NuLgtArcLaserEx(i32 type, NUVEC *start, NUVEC *end, NUVEC *bend, f32 width, f32 segment_length, f32 wobble,
                     f32 bend_amount, i32 colour, i32 flags);

// ==== functions in original address order ====






































































































































extern "C" {
void PartTerrInit(void);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void edpartRegisterPointerToGameCharLocation(NUVEC *position);

}

void edpartPtlShelve(i32 index);

void edpartPlace(i32 index, nuvec_s *position);

void edpartMultipleCopyCopy();

void edpartMultipleCopyClear();

void edpartDrawCursor();

void edpartDetermineNearest(f32 distance);

void edpartHighlightNearest();


void edpartInitType(i32 index);



void edpartScaleType(i32 index, float scale);


i32 edpartSaveEffects(char *filename, char page);




i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);

i32 edpartLookupDebrisEffect(char *name);

extern "C" {
void edpartSetParticlePage(i32 page);

}

i32 edpartLookupObject(char *name);

extern "C" {
void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

void edpartParticleReset(void);

void NuPartEnableRayCasts(i32 enabled);

void NuPartSetSeed(i32 seed);

u32 NuPartGetSeed(void);

void NuPartResetGlobalTime(void);

void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void NewPartRotation(PART_s *part);

}

void NewPartOrderedRotation(PART_s *part);

void PartTimeSlip();

extern "C" {
extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

void KillPart(PART_s *part, i32 reason);

void DrawParts(i32 keep_offscreen);

PART_s *AddPart(ADDPART_s *params);

void KillAllParts(void);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

void PARTStartOffset(i32 index, f32 offset);

i32 PARTLookupType(char *name);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

}

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
i32 PARTLookupTypePageOnly(char *name, i32 page);

i32 GetPartCount(void);

i32 GetMaxPartTypes(void);

char *GetPartName(i32 index);

}



extern "C" {
void CheckPartCount(void);

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);

}


void edpartDestroy(i32 index);

void PartCleanupTypes();


extern "C" {
void edpartDestroyAllParticles(void);

}

i32 edpartPtlChangeType(i32 index, i32 type);

extern "C" {
f32 PARTGetTotalOnTime(i32 index);

f32 PARTGetTotalOffTime(i32 index);

i32 AddPARTEffect(i32 effect, NUVEC *position);

}

void edpartCreate(nuvec_s *position, i32 type);

void edpartDoInput(nupad_s *pad);


extern "C" {
void RemovePARTEffect(i32 index);

}

void UpdatePartEmits(f32 time);

extern "C" {
void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

void UpdateParts(f32 time);

i32 CreateScaledPARTEffect(i32 effect_index, f32 requested_scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

void KillPartsByScene(NUGSCN *scene);

void edpartClearPage(i8 page);

}

void edpartDestroyAllPages();


// ==== src/legoapi/misc/legoapi_status.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {

i32 DEVCDDVDROM_Interrogate(NUFILE_DEVICE *device);

i32 DEVMEMORYCARD_Interrogate(NUFILE_DEVICE *device);

void DebugLog(void);

void Debug_Print(void);

} // extern "C"

// ==== functions in original address order ====

void MenuEnterInsertCard(MENU_s *) {
}

void MenuEnterNoMemoryCard(MENU_s *) {
}

void MenuEnterAutoSaveWarning(MENU_s *) {
    memcard_autosaveenabled = 1;
    memcard_autosavedisabled = 0;
}

void MenuEnterAutoSaveCancel(MENU_s *menu) {
    if (MenuASCancelFinished == 0) {
        memcard_autosaveenabled = 0;
        memcard_autosavedisabled = 0;
        menu->last_row = 0;
    }
}

void MenuEnterCardWarning(MENU_s *) {
    MenuCardWarningState = 0;
    memcard_cardchanged = 0;
}

void MenuExitSave(MENU_s *) {
    Menu_InSaveFlow = 0;
}

void MenuExitLoad(MENU_s *) {
    Menu_InLoadFlow = 0;
}

void MenuExitCardWarning(MENU_s *) {
    Menu_InWarningFlow = 0;
}

void MenuDrawFormatConfirm(MENU_s *) {
}

void MenuDrawCardWarning(MENU_s *) {
}

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled) {
    char menus_used_str[64];

    i32 menu_ids_used = TOTAL_MENUS_COUNT - RESERVED_MENUS_COUNT;
    if (menu_id_count <= TOTAL_MENUS_COUNT - RESERVED_MENUS_COUNT) {
        menu_ids_used = menu_id_count;
    }

    // menu_id_count describes the valid ID range, not the number of records.
    // The game table is sparse (ID 0x1c is absent), sorted, and ends with the
    // record for menu_id_count - 1.
    for (i32 i = 0; i < menu_ids_used; i++) {
        MenuInfo[RESERVED_MENUS_COUNT + i] = menu_info[i];
        if (!(menu_info[i].id >= menu_ids_used - 1)) {
            continue;
        }
        break;
    }

    MenusUsed = menu_ids_used + RESERVED_MENUS_COUNT;
    sprintf(menus_used_str, "Menus used: %d", MenusUsed);

    MenuLanguages = language_count;
    MenuHeader[0] = '\0';

    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;

    if (draw_save_slots_info_fn != NULL) {
        drawslotinfofn = draw_save_slots_info_fn;
    }

    MenuFadeEnabled = is_fade_enabled;
    MenuDrawDropShadows = is_shadow_enabled;

    NUMTL *menu_fade_mtl = NuMtlCreate(1);
    MenuFadeMtl = menu_fade_mtl;

    menu_fade_mtl->attribs.z_mode = 3;
    menu_fade_mtl->attribs.alpha_mode = 1;

    menu_fade_mtl->attribs.unknown_2_1_2 = 2;

    menu_fade_mtl->attribs.unknown_1_1_2 = 1;
    menu_fade_mtl->attribs.unknown_1_4_8 = 1;

    menu_fade_mtl->attribs.unknown_2_4 = 1;

    menu_fade_mtl->attribs.filter_mode = 1;

    NuMtlUpdate(menu_fade_mtl);
}

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled) {
    MenuInitialiseEx(menu_info, menu_id_count, language_count, NULL, is_fade_enabled, is_shadow_enabled);

    if (draw_save_slots_fn != NULL) {
        drawslotsfn = draw_save_slots_fn;
    }
}

extern "C" {
    void MenuSetColours(u8 normal_r, u8 normal_g, u8 normal_b, u8 entry_r, u8 entry_g, u8 entry_b, u8 header_r,
                        u8 header_g, u8 header_b, u8 warn_r, u8 warn_g, u8 warn_b, u8 flash0_r, u8 flash0_g,
                        u8 flash0_b, u8 flash1_r, u8 flash1_g, u8 flash1_b) {
        MENUNORMALR = normal_r;
        MENUNORMALG = normal_g;
        MENUNORMALB = normal_b;
        MENUENTRYR = entry_r;
        MENUENTRYG = entry_g;
        MENUENTRYB = entry_b;
        MENUHEADERR = header_r;
        MENUHEADERG = header_g;
        MENUHEADERB = header_b;
        MENUWARNR = warn_r;
        MENUWARNG = warn_g;
        MENUWARNB = warn_b;
        MENUFLASH0R = flash0_r;
        MENUFLASH0G = flash0_g;
        MENUFLASH0B = flash0_b;
        MENUFLASH1R = flash1_r;
        MENUFLASH1G = flash1_g;
        MENUFLASH1B = flash1_b;
    }

    void MenuSetPulsateSpeed(f32 speed) {
        menu_pulsate_speed = speed;
    }

    void MenuSetTopBottom(f32 top, f32 bottom) {
        MENUTOPY = top;
        MENUBOTY = bottom;
    }

    void MenuSetPreDrawFn(void (*draw_fn)(MENU *)) {
        predrawfn = draw_fn;
    }

    void MenuSetHeaderDrawFn(void (*draw_fn)(void)) {
        headerdrawfn = draw_fn;
    }

    void MenuReset(void) {
        memset(GameMenu, 0, sizeof(GameMenu));
        GameMenu[0].menu = -1;
        MenuSFX = -1;
        GameMenuLevel = 0;
    }

    void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry) {
        MENUSFX_MENUMOVE = move;
        MENUSFX_MENUSELECT = select;
        MENUSFX_MENUBACK = back;
        MENUSFX_MENUNOENTRY = no_entry;
    }

    void MenuRememberCursor(MENU *menu) {
        if (menu->menu == -1)
            return;

        if (MenuInfo[menu->menu].memory_x != -1)
            MenuInfo[menu->menu].memory_x = menu->selected_column;
        if (MenuInfo[menu->menu].memory_y != -1)
            MenuInfo[menu->menu].memory_y = menu->selected_row;
    }

extern "C" void NewMenu(i32 menu_id, i32 menu_y, i32 param3) {
    (void)param3;

    CurrentMenuId = menu_id;

    i32 menu_index = -1;
    for (i32 i = 0; i < MenusUsed; ++i) {
        if (MenuInfo[i].id == menu_id) {
            menu_index = i;
            break;
        }
    }

    MENU *previous = &GameMenu[GameMenuLevel];
    const i16 previous_menu = previous->menu;
    previous->field_bc = 0;
    MenuRememberCursor(previous);
    if (menu_index == -1) {
        GameMenu[GameMenuLevel].menu = -1;
        MenuAlpha = 0.0f;
        MenuA = 0;
        MenuValidated = 0;
        return;
    }

    if (GameMenuLevel + 1 < 10) {
        ++GameMenuLevel;
    }

    MENU *menu = &GameMenu[GameMenuLevel];
    menu->menu_time = 0.0f;
    menu->unk = 0.0f;
    menu->menu = static_cast<i16>(menu_index);
    menu->previous_menu = static_cast<i8>(previous_menu);
    menu->first_column = 0;
    menu->first_row = 0;
    menu->last_column = 0;
    menu->last_row = 0;
    menu->state = 0;
    menu->field_b8 = 1;
    menu->draw_item = 0;
    menu->field_b4 = 1;
    menu->field_b0 = 0;
    menu->field_c0 = 0;
    menu->field_50 = 0;

    MenuStopDraw = 1;
    if (MenuInfo[menu->menu].id != -1 && MenuInfo[menu->menu].draw_fn != NULL) {
        MenuInfo[menu->menu].draw_fn(menu);
    }
    MenuStopDraw = 0;
    menu->last_row = static_cast<i16>(menu->draw_item - 1);
    menu->selected_row = (menu_y >= menu->first_row && menu_y <= menu->last_row) ? static_cast<i16>(menu_y) : 0;
    menu->selected_column = 0;

    if (MenuInfo[menu->menu].enter_fn != NULL) {
        MenuInfo[menu->menu].enter_fn(menu);
    }

    if (MenuInfo[menu->menu].memory_x != -1) {
        menu->selected_column = MenuInfo[menu->menu].memory_x;
    }
    if (menu->selected_column < menu->first_column) {
        menu->selected_column = menu->first_column;
    } else if (menu->selected_column > menu->last_column) {
        menu->selected_column = menu->last_column;
    }
    if (menu_y < menu->first_row || menu_y > menu->last_row) {
        if (MenuInfo[menu->menu].memory_y != -1) {
            menu->selected_row = MenuInfo[menu->menu].memory_y;
        }
        if (menu->selected_row < menu->first_row) {
            menu->selected_row = menu->first_row;
        } else if (menu->selected_row > menu->last_row) {
            menu->selected_row = menu->last_row;
        }
    } else {
        menu->selected_row = static_cast<i16>(menu_y);
    }

    menu->menu_time = 0.0f;
    menu->unk = 0.0f;
    menu->flags_17 = -1;
    menu->transition_time = 0.0f;
    menu->transition_duration = 0.0f;
    memset(menu->item_width, 0, sizeof(menu->item_width));
    memset(menu->item_height, 0, sizeof(menu->item_height));
    menu->queued_item = -1;
    MenuAlpha = 0.0f;
    MenuA = 0;
    MenuValidated = 0;
}

}

void MenuEnterLoad(MENU_s *menu) {
    memcard_cardchanged = 0;
    i32 last_column = SAVESLOTS - 1;
    if (SAVESLOTS > 6) {
        last_column = memcard_slotsused - 1;
    }
    menu->state = 2;
    menu->last_column = static_cast<i16>(last_column);
    menu->last_row = 1;
    lastslot = 0;
    slideleft = 0;
    slideright = 0;
    Menu_InLoadFlow = 1;
    Menu_LastFlow = 3;
    MenuLoadOccurred = 0;
    MenuLoadStarted = 0;

    if (saveload_filecorrupt != 0) {
        NewMenu(1020, -1, -1);
    } else if (saveload_cardtype != 2) {
        NewMenu(1004, -1, -1);
    } else if (saveload_savepresent == 0 || saveload_cardformatted == 0) {
        NewMenu(1005, -1, -1);
    }
}

void MenuEnterSave(MENU_s *menu) {
    MenuSaveOccurred = 0;
    memcard_cardchanged = 0;
    Menu_DisableCancel = 0;
    Menu_InSaveFlow = 1;
    Menu_LastFlow = 2;

    if (SAVESLOTS <= 6) {
        menu->last_column = static_cast<i16>(SAVESLOTS - 1);
    } else {
        menu->last_column = static_cast<i16>(memcard_slotsused);
        if (SAVESLOTS == memcard_slotsused || saveload_freespace < SAVESIZE_ADDITIONAL) {
            --menu->last_column;
        }
    }
    menu->state = 2;
    lastslot = 0;
    slideleft = 0;
    slideright = 0;

    NuStrCpy(MenuHeader, apitxt_SAVEGAME);
    menu->last_row = 1;
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;

    if (saveload_cardtype != 2) {
        NewMenu(1002, -1, -1);
    } else if (saveload_filecorrupt != 0) {
        NewMenu(1020, -1, -1);
    } else if (saveload_cardformatted == 0) {
        NewMenu(1006, 1, -1);
    } else if (__builtin_expect(saveload_savepresent == 0, 1) && saveload_freespace < SAVESIZE) {
        NewMenu(1003, -1, -1);
    } else if (__builtin_expect(Menu_InHeaderSave != 0, 1)) {
        memcard_slot = -1;
        NewMenu(1008, 1, -1);
    }
}

extern "C" {
    // Both backup variants leave the current menu (calling its exit hook), reset
    // the parent's cursor state, queue the "back" sound unless another sound
    // is already pending, and restart the menu fade-in.
    void BackupMenu(void) {
        if (GameMenuLevel == 0) {
            return;
        }

        MENU *menu = &GameMenu[GameMenuLevel];
        if (MenuInfo[menu->menu].exit_fn != NULL) {
            MenuInfo[menu->menu].exit_fn(menu);
        }
        MenuRememberCursor(menu);
        menu->menu = -1;
        menu->field_b4 = 0;
        menu->field_b8 = 1;
        menu->field_bc = 0;

        --GameMenuLevel;
        MENU *parent = &GameMenu[GameMenuLevel];
        parent->field_b8 = 1;
        parent->field_b4 = 1;
        parent->field_b0 = 0;
        parent->field_c0 = 0;
        parent->menu_time = 0.0f;
        if (parent->menu != -1 && MenuInfo[parent->menu].enter_fn != NULL) {
            MenuInfo[parent->menu].enter_fn(parent);
        }
        if (MenuSFX == -1) {
            MenuSFX = MENUSFX_MENUBACK;
        }
        MenuAlpha = 0.0f;
        MenuA = 0;
        MenuValidated = 0;
    }

}

void MenuUpdateCardWarning(MENU_s *menu) {
    menu->previous_item = MenuCardWarningState;
    if (MenuCardWarningState == 0) {
        if (saveload_status == 1) {
            MenuCardWarningState = 3;
            Menu_InWarningFlow = 1;
            Menu_LastFlow = 1;
            BackupMenu();
            return;
        }
    } else if (MenuCardWarningState == 7) {
        if (menu->confirm_pressed != 0) {
            MenuCardWarningState = 3;
            MenuSFX = MENUSFX_MENUSELECT;
            BackupMenu();
            return;
        }
    } else if (MenuCardWarningState == 3) {
        BackupMenu();
        return;
    } else {
        return;
    }
    if (menu->previous_item != MenuCardWarningState) {
        if (MenuCardWarningState != 0) {
            MenuAlpha = 0.0f;
            MenuA = 0;
        }
        menu->unk = 0.0f;
    }
}

void MenuUpdateAutoSaveCancel(MENU_s *menu) {
    static bool firstTimeIn = true;
    if (MenuASCancelFinished != 0) {
        MenuASCancelFinished = 0;
        BackupMenu();
    } else {
        switch (memcard_savefailed) {
            default: {
                if (firstTimeIn) {
                    firstTimeIn = false;
                    memcard_autosavecanceldelay = 5.0f;
                    g_enableButtonPrompts = 0;
                } else if (!(memcard_autosavecanceldelay > 0.0f)) {
                    g_enableButtonPrompts = 1;
                    NewMenu(1000, -1, -1);
                    MenuASCancelFinished = 1;
                    firstTimeIn = true;
                    memcard_autosavecanceldelay = 5.0f;
                    goto check_input;
                }
                break;
            }
            case 0:
            check_input:
                if (menu->confirm_pressed != 0 || menu->cancel_pressed != 0) {
                    MenuASCancelFinished = 1;
                    MenuSFX = MENUSFX_MENUSELECT;
                }
                break;
        }
    }
}

void MenuUpdateDeleting(MENU_s *) {
    if (memcard_deleteneeded == 0) {
        if (memcard_deletemessage_delay > 0.0f) {
            memcard_deleteresult_delay = 1.5f;
            return;
        }
        if (memcard_deleteresult_delay <= 0.0f)
            BackupMenu();
        return;
    }
    memcard_deleteresult_delay = 1.5f;
    if (memcard_cardchanged != 0) {
        memcard_deleteneeded = 0;
        memcard_deletefailed = 1;
    }
}

void MenuUpdateFormatting(MENU_s *) {
    if (memcard_formatme != 0) {
        memcard_formatmessage_delay = 1.5f;
        memcard_formatresult_delay = 1.5f;
        if (memcard_cardchanged != 0) {
            memcard_formatme = 0;
            memcard_formatfailed = 1;
        }
        return;
    }
    if (__builtin_expect(memcard_formatting != 0, 1) || memcard_formatmessage_delay > 0.0f) {
        memcard_formatresult_delay = 1.5f;
        return;
    }
    if (memcard_formatresult_delay <= 0.0f)
        BackupMenu();
}

void MenuUpdateDoNotRemoveCard(MENU_s *menu) {
    if (menu->menu_time > 2.0f && saveload_status == 1) {
        BackupMenu();
    }
}

void MenuEnterStartNewGame(MENU_s *) {
    memcard_autosaveenabled = 0;
    saveload_autosave = -1;
    BackupMenu();
}

extern "C" {
    void BackupMenuNoFn(void) {
        if (GameMenuLevel == 0) {
            return;
        }

        MENU *menu = &GameMenu[GameMenuLevel];
        if (MenuInfo[menu->menu].exit_fn != NULL) {
            MenuInfo[menu->menu].exit_fn(menu);
        }
        MenuRememberCursor(menu);
        menu->field_b4 = 0;
        menu->field_b8 = 1;
        menu->menu = -1;
        menu->field_bc = 0;

        --GameMenuLevel;
        MENU *parent = &GameMenu[GameMenuLevel];
        parent->menu_time = 0.0f;
        parent->field_b8 = 1;
        parent->field_b4 = 1;
        parent->field_b0 = 0;
        parent->field_c0 = 0;
        if (MenuSFX == -1) {
            MenuSFX = MENUSFX_MENUBACK;
        }
        MenuAlpha = 0.0f;
        MenuA = 0;
        MenuValidated = 0;
    }

}

void MenuUpdateFormatCancel(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 0) {
            const i32 previous_id = MenuInfo[GameMenu[GameMenuLevel - 1].menu].id;
            if (previous_id == 1000) {
                BackupMenuNoFn();
                NewMenu(1016, 1, -1);
            } else if (previous_id == 1012) {
                BackupMenuNoFn();
                NewMenu(1017, 1, -1);
            } else {
                BackupMenu();
            }
        } else {
            BackupMenuNoFn();
            NewMenu(1006, 1, -1);
        }
    } else if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1006, 1, -1);
    }
}

void MenuUpdateLoadCancel(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        if (menu->selected_row == 0) {
            BackupMenuNoFn();
        }
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    }
}

void MenuUpdateSaveCancel(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        if (menu->selected_row == 0) {
            BackupMenuNoFn();
        }
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    }
}

void MenuUpdateLoading(MENU_s *) {
    if (memcard_loadneeded != 0) {
        memcard_loadmessage_delay = 1.0f;
        memcard_loadresult_delay = 1.0f;
        if (memcard_cardchanged != 0 || saveload_cardtype == -1) {
            memcard_loadneeded = 0;
            memcard_loadfailed = 1;
        }
    } else if (memcard_loadstarted != 0 || memcard_loadmessage_delay > 0.0f) {
        memcard_loadresult_delay = 1.0f;
    } else if (memcard_loadresult_delay <= 0.0f) {
        if (memcard_loadfailed == 0 && memcard_loadcorrupt == 0) {
            if (memcard_autosave == 0 || saveload_autosave == -1) {
                BackupMenuNoFn();
            } else {
                BackupMenuNoFn();
                BackupMenuNoFn();
                memcard_autosaveenabled = 1;
                memcard_autosavedisabled = 0;
                return;
            }
        }
        BackupMenu();
    }
}

void MenuUpdateAutoSaveWarning(MENU_s *menu) {
    if (memcard_cardchanged != 0) {
        BackupMenu();
    }
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
    }
}

void MenuUpdateSaving(MENU_s *) {
    if (memcard_saveneeded != 0) {
        memcard_savemessage_delay = 1.0f;
        memcard_saveresult_delay = 1.0f;
        if (memcard_cardchanged != 0 || saveload_cardtype == -1) {
            memcard_saveneeded = 0;
            memcard_savefailed = 1;
        }
        return;
    }

    if (memcard_savestarted != 0) {
        memcard_saveresult_delay = 1.0f;
        return;
    }
    if (memcard_savemessage_delay > 0.0f || memcard_saveresult_delay > 0.0f) {
        return;
    }

    if (memcard_savefailed == 0) {
        if (memcard_autosave != 0 && saveload_autosave != -1) {
            BackupMenuNoFn();
            BackupMenuNoFn();
            memcard_autosaveenabled = 1;
            memcard_autosavedisabled = 0;
            return;
        }
        BackupMenuNoFn();
    }
    BackupMenu();
}

void MenuUpdateDeleteConfirm(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 1) {
            BackupMenu();
        } else {
            memcard_slot = -1;
            memcard_deleteneeded = 1;
            memcard_deletestarted = 0;
            BackupMenuNoFn();
            BackupMenuNoFn();
            NewMenu(1015, 0, -1);
        }
    } else if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenu();
    }
}

void MenuUpdateFileCorrupt(MENU_s *menu) {
    if (MenuInfo[GameMenu[GameMenuLevel - 1].menu].id != 1019)
        MenuInfo[menu->menu].cancel_prompt = 1;
    else
        MenuInfo[menu->menu].cancel_prompt = 0;
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (MenuInfo[GameMenu[GameMenuLevel - 1].menu].id == 1000) {
            BackupMenuNoFn();
            NewMenu(1016, 1, -1);
        } else if (MenuInfo[GameMenu[GameMenuLevel - 1].menu].id == 1019) {
            MenuCardWarningState = 3;
            memcard_cardchanged = 0;
            BackupMenuNoFn();
        } else {
            BackupMenuNoFn();
            NewMenu(1017, 1, -1);
        }
    } else if (menu->cancel_pressed != 0 && MenuInfo[GameMenu[GameMenuLevel - 1].menu].id != 1019) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (MenuInfo[GameMenu[GameMenuLevel - 1].menu].id == 1000) {
            BackupMenuNoFn();
            NewMenu(1016, 1, -1);
        } else {
            BackupMenuNoFn();
            NewMenu(1017, 1, -1);
        }
    }
}

void MenuUpdateFormatConfirm(MENU_s *menu) {
    if (__builtin_expect(menu->confirm_pressed != 0, 1)) {
        MenuSFX = MENUSFX_MENUSELECT;
    } else if (menu->cancel_pressed != 0 && MenuInfo[GameMenu[GameMenuLevel - 1].menu].id != 1019) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1018, 1, -1);
    }
}

void MenuUpdateNoData(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        BackupMenu();
    }
    if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        BackupMenu();
    }
}

void MenuUpdateNotEnoughSpace(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1016, 1, -1);
    }
}

void MenuUpdateNoMemoryCard(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 0) {
            BackupMenu();
        } else {
            BackupMenuNoFn();
            NewMenu(1017, 1, -1);
        }
    }
    if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1017, 1, -1);
    }
}

void MenuUpdateInsertCard(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1016, 1, -1);
    }
    if (menu->confirm_pressed != 0 && menu->selected_row == menu->last_row) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenuNoFn();
        NewMenu(1016, 1, -1);
    }
}

void MenuEnterHeaderSave(MENU_s *) {
    Menu_InHeaderSave = 1;
    BackupMenuNoFn();
    NewMenu(1000, -1, -1);
}

extern "C" {
    void MenuRepeat(i32 *held, i32 *repeat, f32 *repeat_time, u8 *repeat_count, f32 interval, f32 elapsed) {
        if (*held != 0) {
            if (*repeat_time > 0.0f) {
                *repeat_time -= elapsed;
                if (*repeat_time <= 0.0f) {
                    *repeat = 1;
                    *repeat_time = interval;
                    ++*repeat_count;
                }
            }
        } else {
            *repeat_time = 0.5f;
            *repeat_count = 0;
        }
    }

}

i32 MenuIsAvailable() {
    return GameMenu[GameMenuLevel].menu != -1;
}

extern "C" {
    void DrawMenuButtonPromptsEx(i32, i32, i32, i32, u8, u8, u8, u8) {
    }

    void DrawMenuButtonPrompts(i32 confirm_prompt, i32 cancel_prompt, i32 enabled, u8 red, u8 green, u8 blue,
                               u8 alpha) {
        DrawMenuButtonPromptsEx(confirm_prompt, cancel_prompt, 0, enabled, red, green, blue, alpha);
    }

    void DrawMenu(i32 paused) {
        if (memcard_autosavestarted != 0 || memcard_autosavepostdelay > 0.0f || memcard_autosavepredelay > 0.0f) {
            if (memcard_drawasiconfn != NULL) {
                memcard_drawasiconfn();
            }
            return;
        }

        MENU *menu = &GameMenu[GameMenuLevel];
        i16 menu_index = menu->menu;
        if (menu_index == -1)
            return;

        MenuValidated = 1;
        f32 saved_baseline;
        f32 saved_button_baseline;
        vufnt_s *font = SmartTextFont;
        if (font == NULL) {
            font = QFont2D;
        }
        if (font != NULL) {
            saved_baseline = font->baseline;
            font->baseline = 0.0f;
        } else {
            saved_baseline = 0.0f;
        }

        if (QFont2DButtons != NULL) {
            saved_button_baseline = QFont2DButtons->baseline;
            QFont2DButtons->baseline = 0.0f;
        } else {
            saved_button_baseline = 0.0f;
        }

        menu->draw_item = menu->first_row;
        f32 centre_offset = static_cast<f32>(menu->last_row - menu->first_row) * MENUDY * 0.5f;
        menu->paused = paused;
        menu->item_scale = 1.0f;
        MenuHeader[0] = '\0';
        menu->field_a0 = 0.0f;
        menu->centre_offset = centre_offset;
        menu->draw_x = 0.0f;
        menu->draw_y = -centre_offset;
        menu->draw_z = 1.0f;
        if (predrawfn != NULL) {
            predrawfn(menu);
            menu_index = menu->menu;
        }
        if (MenuInfo[menu_index].draw_fn != NULL)
            MenuInfo[menu_index].draw_fn(menu);
        if (headerdrawfn != NULL)
            headerdrawfn();
        if (MenuAlpha >= 0.5f && g_enableButtonPrompts != 0) {
            DrawMenuButtonPrompts(MenuInfo[menu->menu].confirm_prompt, MenuInfo[menu->menu].cancel_prompt, 1,
                                  MENUNORMALR, MENUNORMALG, MENUNORMALB, static_cast<u8>(MenuA));
        }

        font = SmartTextFont;
        if (font == NULL) {
            font = QFont2D;
        }
        if (font != NULL) {
            font->baseline = saved_baseline;
        }
        if (QFont2DButtons != NULL) {
            QFont2DButtons->baseline = saved_button_baseline;
        }
    }

}

void MenuStartLoad() {
    MenuLoadStarted = 1;
    memcard_loadneeded = 1;
    memcard_loadstarted = 0;
    memcard_loadfailed = 0;
    memcard_loadcorrupt = 0;
}

void MenuUpdateLoadConfirm(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 1) {
            BackupMenu();
        } else {
            MenuStartLoad();
            BackupMenuNoFn();
            NewMenu(1014, 0, -1);
        }
    } else if (menu->cancel_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenu();
    }
}

void MenuUpdateLoad(MENU_s *menu) {
    if (memcard_cardchanged != 0 || saveload_cardtype != 2) {
        MenuEnterLoad(menu);
        return;
    }

    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 0) {
            memcard_slot = menu->selected_column - menu->first_column;
            if (saveload_slotused[memcard_slot] == 0) {
                MenuSFX = MENUSFX_MENUNOENTRY;
                return;
            }

            MenuStartLoad();
            NewMenu(1014, 0, -1);
            return;
        }

        if (menu->selected_row == menu->last_row) {
            BackupMenu();
        }
        return;
    }

    if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    }
}

void MenuStartSave() {
    memcard_saveneeded = 1;
    memcard_savestarted = 0;
    memcard_savefailed = 0;
}

void MenuUpdateSaveConfirm(MENU_s *menu) {
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 1) {
            BackupMenu();
        } else {
            MenuStartSave();
            BackupMenuNoFn();
            NewMenu(1009, 0, -1);
        }
    } else if (menu->cancel_pressed != 0) {
        BackupMenu();
        MenuSFX = MENUSFX_MENUSELECT;
    }
}

void MenuUpdateSave(MENU_s *menu) {
    if (Menu_DisableCancel != 0) {
        MenuInfo[menu->menu].cancel_prompt = 0;
    } else {
        MenuInfo[menu->menu].cancel_prompt = 1;
    }

    if (memcard_cardchanged != 0 || saveload_cardtype != 2) {
        MenuEnterSave(menu);
    }

    const i32 slot = menu->selected_column - menu->first_column;
    if (menu->confirm_pressed != 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        if (menu->selected_row == 0) {
            if (saveload_slotused[slot] != 0) {
                memcard_slot = slot;
                NewMenu(1008, 1, -1);
            } else if (saveload_freespace >= SAVESIZE_ADDITIONAL) {
                memcard_slot = slot;
                MenuStartSave();
                NewMenu(1009, 0, -1);
            } else {
                MenuSFX = MENUSFX_MENUNOENTRY;
            }
        } else if (menu->selected_row == menu->last_row && Menu_DisableCancel == 0) {
            BackupMenu();
        }
    } else if (menu->cancel_pressed != 0 && Menu_DisableCancel == 0) {
        MenuSFX = MENUSFX_MENUSELECT;
        BackupMenu();
    }
}

void MenuEnterSaveConfirm(MENU_s *) {
    const i32 slot = Menu_InHeaderSave != 0 ? SAVESLOTS : memcard_slot;
    Menu_Overwriteflag = saveload_slotused[slot] != 0;
    if (Menu_Overwriteflag == 0) {
        MenuStartSave();
        BackupMenuNoFn();
        NewMenu(1009, 0, -1);
    }
}

extern "C" {
    void Draw_SPACENEEDED(void) {
    }

}

void Draw_NOMEMORYCARD() {
}

void MenuDrawInsertCard(MENU_s *) {
    Draw_NOMEMORYCARD();
    Draw_SPACENEEDED();
}

void Draw_AUTOSAVECANCEL() {
}

extern "C" {
    void Draw_CHECKINGMEMORYCARD(void) {
    }

    void Draw_DONOTREMOVEMEMORYCARD(void) {
    }

}

void MenuDrawDoNotRemoveCard(MENU_s *) {
    Draw_CHECKINGMEMORYCARD();
    Draw_DONOTREMOVEMEMORYCARD();
}

extern "C" {
    i32 MenuGetSlotNum(void) {
        return memcard_slot;
    }

    i32 MenuInMemoryCard(void) {
        if (GameMenuLevel == -1 || MenuValidated == 0)
            return 0;

        const i32 menu_id = MenuInfo[GameMenu[GameMenuLevel].menu].id;
        return (u32)(menu_id - LEGO_MENU_MEMORY_CARD_FIRST) <=
               (LEGO_MENU_MEMORY_CARD_LAST - LEGO_MENU_MEMORY_CARD_FIRST);
    }

    i32 MenuInCriticalMemoryCard(void) {
        if (MenuValidated == 0)
            return 0;

        const i32 menu_id = MenuInfo[GameMenu[GameMenuLevel].menu].id;
        switch (menu_id) {
            case 1001:
            case 1007:
            case 1009:
            case 1014:
            case 1015:
                return 1;
            default:
                return 0;
        }
    }

    i32 MenuInMemoryCardLoad(void) {
        if (MenuValidated == 0) {
            return 0;
        }
        return Menu_InLoadFlow;
    }

    i32 MenuInMemoryCardWarning(void) {
        if (MenuValidated == 0) {
            return 0;
        }
        return Menu_InWarningFlow;
    }

    i32 MenuCurrentID(void) {
        return MenuInfo[GameMenu[GameMenuLevel].menu].id;
    }

    void MenuDrawBackground(void) {
        static i32 menufadelevel;
        if (MenuInCriticalMemoryCard() != 0) {
            if (__builtin_expect(menufadelevel <= 127, 1)) {
                menufadelevel += 8;
                if (menufadelevel <= 0)
                    return;
            }
        } else {
            if (menufadelevel <= 0)
                return;
            menufadelevel -= 5;
            if (menufadelevel <= 0)
                return;
        }

        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_ABSOLUTE);
        NuPrim2DBegin(4, 5, MenuFadeMtl);

        struct MenuFadeVertex {
            f32 x;
            f32 y;
            f32 z;
            u32 colour;
        };
        VARIPTR **stream = &g_NuPrim_StreamBufferPtr;
        char *overbright = &g_NuPrim_NeedsOverbrightening;
        u32 colour = static_cast<u32>(menufadelevel) << 24;
        MenuFadeVertex *vertex = reinterpret_cast<MenuFadeVertex *>((*stream)->void_ptr);
        if (__builtin_expect(*overbright == 0, 1))
            colour &= 0xff000000u;
        vertex->colour = colour;
        NuPrim2DAddXYZ(0.0f, 0.0f, 0.0f);

        colour = static_cast<u32>(menufadelevel) << 24;
        vertex = reinterpret_cast<MenuFadeVertex *>((*stream)->void_ptr);
        if (__builtin_expect(*overbright == 0, 1))
            colour &= 0xff000000u;
        vertex->colour = colour;
        NuPrim2DAddXYZ(1.0f, 1.0f, 0.0f);

        NuPrim2DEnd();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
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

extern "C" {
    bool TestForController(void) {
        return Controller_IsConnected() != 0 || enable_touch_controls == 0;
    }

    i32 UpdateMenu(u32 primary_held, u32 primary_pressed, u32 alternate_held, u32 alternate_pressed, f32 elapsed,
                   u32 confirm_mask, u32 cancel_mask, u32 start_mask, u32 select_mask) {
        MenuResult = 0;
        if (MenuValidated == 0)
            return 0;

        MENU *menu = &GameMenu[GameMenuLevel];
        if (MenuFadeEnabled != 0) {
            if (MenuAlpha < 1.0f) {
                MenuAlpha += elapsed + elapsed;
                if (MenuAlpha > 1.0f)
                    MenuAlpha = 1.0f;
            }
        } else {
            MenuAlpha = 1.0f;
        }

        const i32 alpha_angle = static_cast<i32>(MenuAlpha * 16384.0f);
        MenuA = static_cast<i32>(NuTrigTable[(alpha_angle >> 1) & 0x7fff] * 128.0f);

        if (menu_pulsate_speed > 0.0f) {
            menu_pulsate_angle += static_cast<i32>(elapsed * 65536.0f * menu_pulsate_speed);
            menu_pulsate = (NuTrigTable[(menu_pulsate_angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        } else {
            menu_pulsate = 0.0f;
        }
        if (menu_pulse_speed > 0.0f && TestForController() == 0) {
            menu_pulse_angle += static_cast<i32>(elapsed * 65536.0f * menu_pulse_speed);
            menu_pulse = (NuTrigTable[(menu_pulse_angle >> 1) & 0x7fff] - 0.75f) * 4.0f;
        } else {
            menu_pulse = 0.0f;
        }

        MenuSFX = -1;
        if (sfx_wait > 0.0f)
            sfx_wait -= elapsed;
        if (menu->menu == -1)
            return 0;

        const u32 directions = 0xf000;
        u32 held = primary_held | alternate_held;
        if (__builtin_expect((primary_held & directions) != 0, 1))
            held = primary_held;
        u32 pressed = primary_pressed | alternate_pressed;
        if ((primary_pressed & directions) != 0)
            pressed = primary_pressed;

        if (__builtin_expect(MenuAlpha < 0.5f, 1)) {
            held = 0;
            pressed = 0;
        }

        i32 down_pressed = pressed & 0x4000;
        i32 up_pressed = pressed & 0x1000;
        i32 right_pressed = pressed & 0x2000;
        i32 left_pressed = pressed & 0x8000;
        i32 up_held = held & 0x1000;
        if (fastup_lock != 0) {
            if (up_held != 0)
                up_held = 0;
            else
                fastup_lock = 0;
        }
        i32 down_held = held & 0x4000;
        if (fastdown_lock != 0) {
            if (down_held != 0)
                down_held = 0;
            else
                fastdown_lock = 0;
        }
        i32 left_held = held & 0x8000;
        if (fastleft_lock != 0) {
            if (left_held != 0)
                left_held = 0;
            else
                fastleft_lock = 0;
        }
        i32 right_held = held & 0x2000;
        if (fastright_lock != 0) {
            if (right_held != 0)
                right_held = 0;
            else
                fastright_lock = 0;
        }

        u32 action_04_pressed = pressed & 4;
        u32 action_08_pressed = pressed & 8;
        const u32 select_pressed = pressed & select_mask;
        i32 confirm_pressed;
        i32 cancel_pressed;
        if (menu->unk == 0.0f) {
            confirm_pressed = cancel_pressed = 0;
        } else {
            confirm_pressed = pressed & confirm_mask;
            cancel_pressed = pressed & cancel_mask;
        }
        if (__builtin_expect(down_pressed != 0, 1)) {
            if (up_pressed != 0)
                up_pressed = down_pressed = 0;
        }
        if (right_pressed != 0) {
            if (left_pressed != 0)
                left_pressed = right_pressed = 0;
        }
        if (up_held != 0 && down_held != 0)
            up_held = down_held = 0;
        if (left_held != 0 && right_held != 0)
            left_held = right_held = 0;
        if (action_04_pressed != 0 && action_08_pressed != 0)
            action_04_pressed = action_08_pressed = 0;

        menu->input_activity = 0;
        if (menu->move_left != 0) {
            left_pressed = 1;
            menu->move_left = 0;
            menu->input_activity = 1;
        }
        if (menu->move_right != 0) {
            right_pressed = 1;
            menu->move_right = 0;
            menu->input_activity = 1;
        }
        if (menu->move_up != 0) {
            up_pressed = 1;
            menu->move_up = 0;
            menu->input_activity = 1;
        }
        if (menu->move_down != 0) {
            down_pressed = 1;
            menu->move_down = 0;
            menu->input_activity = 1;
        }

        u32 start_pressed = pressed & start_mask;
        const i32 output_up_pressed = up_pressed;
        const i32 output_down_pressed = down_pressed;

        if (menu->input_disabled != 0) {
            up_pressed = down_pressed = 0;
            confirm_pressed = cancel_pressed = 0;
            start_pressed = 0;
        }

        MenuRepeat(&up_held, &up_pressed, &menu->repeat_up_time, &menu->repeat_up_count, 0.1f, elapsed);
        MenuRepeat(&down_held, &down_pressed, &menu->repeat_down_time, &menu->repeat_down_count, 0.1f, elapsed);
        MenuRepeat(&left_held, &left_pressed, &menu->repeat_left_time, &menu->repeat_left_count, 0.1f, elapsed);
        MenuRepeat(&right_held, &right_pressed, &menu->repeat_right_time, &menu->repeat_right_count, 0.1f, elapsed);

        const i16 last_column = menu->last_column;
        const i16 first_column = menu->first_column;
        const i16 first_row = menu->first_row;
        const i16 last_row = menu->last_row;
        const i32 column_count = last_column - first_column + 1;
        const i32 row_count = last_row - first_row + 1;
        i32 previous_item = 0;
        f32 item_time = 0.0f;
        if (menu->transition_duration > menu->transition_time) {
            menu->transition_time += elapsed;
            if (menu->transition_time >= menu->transition_duration) {
                menu->transition_time = menu->transition_duration;
                if (menu->flags_17 != -1)
                    NewMenu(menu->flags_17, -1, -1);
            }
            confirm_pressed = cancel_pressed = 0;
            item_time = menu->unk;
        } else {
            const i16 old_row = menu->selected_row;
            const MENUFNINFO &info = MenuInfo[menu->menu];
            const bool wrap = info.wrap != 0;
            bool row_clamped = false;
            if (!(menu->selected_row < first_row)) {
                if (menu->selected_row > last_row) {
                    menu->selected_row = last_row;
                    row_clamped = true;
                } else if (down_pressed != 0 && (wrap || menu->selected_row < last_row)) {
                    if (__builtin_expect(!wrap, 0)) {
                        ++menu->selected_row;
                    } else {
                        ++menu->selected_row;
                        if (menu->selected_row > last_row)
                            menu->selected_row = first_row;
                    }
                } else if (up_pressed != 0 && (wrap || menu->selected_row > first_row)) {
                    if (!wrap) {
                        --menu->selected_row;
                    } else {
                        --menu->selected_row;
                        if (menu->selected_row < first_row)
                            menu->selected_row = last_row;
                    }
                }
            } else {
                menu->selected_row = first_row;
                row_clamped = true;
            }
            if (menu->selected_row != old_row) {
                menu->unk = 0.0f;
                if (!row_clamped)
                    MenuSFX = MENUSFX_MENUMOVE;
            }
            previous_item = old_row;
            const bool column_navigation = menu->state == 1 || (menu->state == 2 && menu->selected_row == 0) ||
                                           (menu->state == 3 && menu->selected_row != last_row);
            if (column_navigation) {
                const i16 old_column = menu->selected_column;
                previous_item = old_column;
                if (menu->selected_column < first_column)
                    menu->selected_column = first_column;
                else if (menu->selected_column > last_column)
                    menu->selected_column = last_column;
                if (right_pressed != 0 && menu->selected_column < last_column)
                    ++menu->selected_column;
                else if (left_pressed != 0 && menu->selected_column > first_column)
                    --menu->selected_column;
                if (menu->selected_column != old_column)
                    MenuSFX = MENUSFX_MENUMOVE;
                else
                    item_time = menu->unk;
            } else {
                item_time = menu->unk;
            }
        }
        menu->selected_item = menu->selected_row;
        menu->selected_item_column = menu->selected_column;
        menu->previous_item = previous_item;
        menu->buttons_held = held;
        menu->buttons_pressed = pressed;
        menu->column_count = column_count;
        menu->row_count = row_count;
        menu->up_pressed = output_up_pressed;
        menu->down_pressed = output_down_pressed;
        menu->left_pressed = left_pressed;
        menu->confirm_pressed = confirm_pressed;
        menu->right_pressed = right_pressed;
        menu->cancel_pressed = cancel_pressed;
        menu->up_held = up_held;
        menu->down_held = down_held;
        menu->left_held = left_held;
        menu->right_held = right_held;
        menu->start_pressed = start_pressed;
        menu->select_pressed = select_pressed;
        menu->action_04_pressed = action_04_pressed;
        menu->action_08_pressed = action_08_pressed;
        menu->menu_time += elapsed;
        menu->unk = item_time + elapsed;

        if (menu->queued_item != -1) {
            menu->selected_item = menu->queued_item;
            menu->selected_column = static_cast<i16>(menu->queued_column);
            menu->confirm_pressed = 1;
            menu->selected_row = static_cast<i16>(menu->queued_row);
            menu->queued_item = -1;
            menu->input_activity = 1;
        }
        if (menu->close_requested != 0) {
            menu->cancel_pressed = 1;
            menu->close_requested = 0;
            menu->input_activity = 1;
        }

        if (MenuInfo[menu->menu].update_fn != NULL)
            MenuInfo[menu->menu].update_fn(menu);
        if (MenuSFX != -1 && menu->input_disabled == 0) {
            PlaySfxById(MenuSFX, 0);
            MenuSFX = -1;
        }
        return MenuResult;
    }

}

void SaveSystemInitialise(i32 slots, void *makeSaveHash, void *save, i32 saveSize, i32 saveCount,
                          void (*drawSaveIcon)(void), void *extradata, i32 extradataSize) {
    i32 saveslots;
    if (extradata == NULL) {
        saveslots = 6;
        if (slots < 7) {
            saveslots = slots;
        }
    } else {
        saveslots = 5;
        if (slots < 6) {
            saveslots = slots;
        }
    }
    SAVESLOTS = saveslots;

    memcard_hashfn = (hashfn_t)makeSaveHash;
    memcard_savedata = save;
    memcard_savedatasize = saveSize;
    memcard_extra_savedata = extradata;
    memcard_extra_savedatasize = extradataSize;

    memcard_savedatabuffer = NU_ALLOC(saveSize + 4, 4, 1, "", NUMEMORY_CATEGORY_NONE);
    memcard_extra_savedatabuffer = NU_ALLOC(extradataSize + 4, 4, 1, "", NUMEMORY_CATEGORY_NONE);

    memcard_autosave = saveCount;
    memcard_drawasiconfn = drawSaveIcon;
}

extern "C" {
    void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void)) {
        memcard_hashfn = reinterpret_cast<i16 (*)(void)>(makeSaveHash);
        memcard_savedata = save;
        memcard_savedatasize = saveSize;
        memcard_savedatabuffer = NU_ALLOC(saveSize + 4, 4, 1, "Main", 0);

        memcard_autosave = autosave;
        memcard_drawasiconfn = drawSaveIcon;
        memcard_headerdata = header;
        memcard_headerdatasize = headerSize;
        memcard_headerdatabuffer = NU_ALLOC(headerSize + 4, 4, 1, "Main", 0);

        i32 saveSlots;
        if (header == nullptr) {
            saveSlots = 6;
            if (slots < 7) {
                saveSlots = slots;
            }
        } else {
            saveSlots = 5;
            if (slots < 6) {
                saveSlots = slots;
            }
        }
        SAVESLOTS = saveSlots;
    }

    void SetSaveSuccessFn(void (*callback)(void)) {
        savesuccessfn = callback;
    }

}

i32 ChecksumSaveData(void *buffer, i32 size) {
    i32 n = size / 4;

    u32 sum = 0x5c0999;

    for (i32 i = 0; i < n; i++) {
        sum += reinterpret_cast<u32 *>(buffer)[i];
    }

    return static_cast<i32>(sum);
}

extern "C" {
    void UpdateSaveSlots(void) {
        const f32 elapsed = NuTimeGetFrameTime();
        if (memcard_autosavecanceldelay > 0.0f)
            memcard_autosavecanceldelay -= elapsed;
#define ADVANCE_SAVE_DELAY(message, result)                                                                            \
    if (message > 0.0f) {                                                                                              \
        message -= elapsed;                                                                                            \
    } else if (result > 0.0f) {                                                                                        \
        result -= elapsed;                                                                                             \
    }
        ADVANCE_SAVE_DELAY(memcard_loadmessage_delay, memcard_loadresult_delay);
        ADVANCE_SAVE_DELAY(memcard_savemessage_delay, memcard_saveresult_delay);
        if (memcard_autosavepredelay > 0.0f) {
            memcard_autosavepredelay -= elapsed;
        } else if (memcard_autosavepostdelay > 0.0f) {
            memcard_autosavepostdelay -= elapsed;
            if (memcard_autosavepostdelay <= 0.0f)
                memcard_autosaveinprogress = 0;
        }
        ADVANCE_SAVE_DELAY(memcard_formatmessage_delay, memcard_formatresult_delay);
        ADVANCE_SAVE_DELAY(memcard_createmessage_delay, memcard_createresult_delay);
        ADVANCE_SAVE_DELAY(memcard_deletemessage_delay, memcard_deleteresult_delay);
        ADVANCE_SAVE_DELAY(memcard_message_delay, memcard_result_delay);
#undef ADVANCE_SAVE_DELAY

        if (saveload_cardtype != 2)
            memcard_justformatted = 0;
        if (saveload_autosavedisabled != 0) {
            if (memcard_autosavestarted != 0) {
                memcard_savefailed = 1;
                memcard_saveresult_delay = 1.5f;
            }
            memcard_autosavestarted = 0;
            memcard_autosavepredelay = 0.0f;
            memcard_autosaveinprogress = 0;
            memcard_autosaveneeded = 0;
            memcard_autosavepostdelay = 0.0f;
            memcard_autosavedisabled = 1;
            saveload_autosavedisabled = 0;
        }

        if (memcard_loadneeded != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                memcard_loadmessage_delay = 1.5f;
                if (memcard_savedatabuffer == NULL || memcard_savedatasize <= 0) {
                    memcard_loadneeded = 0;
                    memcard_loadstarted = 0;
                    memcard_loadfailed = 1;
                    return;
                }
                if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL)
                    saveloadASLoad(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4);
                saveloadASLoad(memcard_slot, memcard_savedatabuffer, memcard_savedatasize + 4);
                memcard_loadneeded = 0;
                memcard_loadcorrupt = 0;
                if (saveload_status == 1) {
                    memcard_loadstarted = 0;
                    memcard_loadfailed = 1;
                } else {
                    memcard_loadstarted = 1;
                    memcard_loadfailed = 0;
                }
            }
        } else if (memcard_loadstarted != 0 && saveload_status == 1) {
            memcard_loadstarted = 0;
            if (saveload_error == 0 && memcard_savedatabuffer != NULL && memcard_savedatasize >= 0) {
                i32 stored_checksum;
                memcpy(&stored_checksum, static_cast<u8 *>(memcard_savedatabuffer) + memcard_savedatasize, 4);
                if (ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize) != stored_checksum) {
                    saveload_autosave = -1;
                    memcard_loadcorrupt = 1;
                }
                if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                    memcard_extra_savedatasize >= 0) {
                    i32 stored_extra_checksum;
                    memcpy(&stored_extra_checksum,
                           static_cast<u8 *>(memcard_extra_savedatabuffer) + memcard_extra_savedatasize, 4);
                    if (ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize) !=
                        stored_extra_checksum) {
                        memcard_loadcorrupt = 1;
                        saveload_autosave = -1;
                    }
                }
                if (memcard_loadcorrupt == 0 && memcard_savedata != NULL) {
                    memmove(memcard_savedata, memcard_savedatabuffer, memcard_savedatasize);
                    if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                        memcard_extra_savedatasize >= 0)
                        memmove(memcard_extra_savedata, memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                    MenuLoadOccurred = 1;
                }
            } else if (saveload_error == 0x80) {
                memcard_loadcorrupt = 1;
                saveload_autosave = -1;
            } else {
                memcard_loadfailed = 1;
                saveload_autosave = -1;
            }
        }

        if (memcard_autosaveneeded != 0) {
            memcard_autosaveneeded = 0;
            if (saveload_autosave != -1) {
                memcard_autosavepredelay = 1.0f;
                memcard_autosavestarted = 1;
                memcard_autosaveinprogress = 1;
            }
        } else if (memcard_autosavestarted != 0 && memcard_saveneeded == 0 && memcard_savestarted == 0 &&
                   !(memcard_autosavepredelay > 0.0f)) {
            memcard_slot = saveload_autosave;
            memcard_saveneeded = 1;
        }

        if (memcard_saveneeded == 0) {
            if (memcard_savestarted != 0 && saveload_status == 1) {
                memcard_savestarted = 0;
                if (saveload_error == 0) {
                    MenuSaveOccurred = 1;
                    if (savesuccessfn != NULL)
                        savesuccessfn();
                    if (memcard_autosavestarted != 0) {
                        memcard_autosavepostdelay = 1.0f;
                        memcard_autosavestarted = 0;
                    }
                } else {
                    memcard_savefailed = 1;
                    if (saveload_autosave != -1) {
                        memcard_saveresult_delay = 1.5f;
                        saveload_autosave = -1;
                        memcard_autosavedisabled = 1;
                        saveload_autosavedisabled = 1;
                    }
                    memcard_autosavestarted = 0;
                    memcard_autosaveinprogress = 0;
                }
            }
        } else {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                memcard_savemessage_delay = 1.5f;
                if (memcard_savedatabuffer == NULL || memcard_savedata == NULL || memcard_savedatasize <= 0) {
                    memcard_saveneeded = 0;
                    memcard_savestarted = 0;
                    memcard_savefailed = 1;
                    return;
                }
                i32 save_hash = memcard_hashfn != NULL ? memcard_hashfn() : -1;
                if (memcard_slot != -1) {
                    if (memcard_extra_savedata != NULL && memcard_extra_savedatabuffer != NULL &&
                        memcard_extra_savedatasize >= 0) {
                        memmove(memcard_extra_savedatabuffer, memcard_extra_savedata, memcard_extra_savedatasize);
                        i32 checksum = ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
                        memcpy(static_cast<u8 *>(memcard_extra_savedatabuffer) + memcard_extra_savedatasize, &checksum,
                               4);
                        saveloadASSave(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4, -1);
                    }
                    memmove(memcard_savedatabuffer, memcard_savedata, memcard_savedatasize);
                    i32 checksum = ChecksumSaveData(memcard_savedatabuffer, memcard_savedatasize);
                    memcpy(static_cast<u8 *>(memcard_savedatabuffer) + memcard_savedatasize, &checksum, 4);
                    saveloadASSave(memcard_slot, memcard_savedatabuffer, memcard_savedatasize + 4, save_hash);
                }
                memcard_saveneeded = 0;
                memcard_savestarted = 1;
                memcard_savefailed = 0;
                memcard_justformatted = 0;
            }
        }

        if (memcard_deleteneeded != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2 && saveload_cardformatted != 0) {
                saveloadASDelete(memcard_slot);
                memcard_deleteneeded = 0;
                memcard_deletemessage_delay = 1.5f;
                memcard_deletestarted = 1;
                memcard_deletefailed = 0;
            }
        } else if (memcard_deletestarted != 0 && saveload_status == 1) {
            memcard_deletefailed = saveload_error != 0;
            memcard_deletestarted = 0;
        }
        if (memcard_formatme != 0) {
            if (saveload_status == 1 && saveload_cardtype == 2) {
                saveloadASFormat();
                memcard_formatmessage_delay = 1.5f;
                memcard_formatme = 0;
                memcard_formatting = 1;
                memcard_formatfailed = 0;
                memcard_justformatted = 0;
            }
        } else if (memcard_formatting != 0 && saveload_status == 3) {
            memcard_formatting = 0;
            if (saveload_error == 0)
                memcard_justformatted = 1;
            else
                memcard_formatfailed = 1;
        }
    }

    void loadsaveCallEachFrame(void) {
        saveloadASCallEachFrame();
        if (saveload_cardchanged != 0) {
            memcard_cardchanged = 1;
        }
        UpdateSaveSlots();

        memcard_slotsused = 0;
        if (saveload_savepresent != 0) {
            i32 slots = SAVESLOTS;
            const i32 slot_capacity = sizeof(saveload_slotused) / sizeof(saveload_slotused[0]);
            if (slots > slot_capacity) {
                slots = slot_capacity;
            }
            i32 used = 0;
            for (i32 slot = 0; slot < slots; ++slot) {
                if (saveload_slotused[slot] != 0) {
                    ++used;
                }
            }
            memcard_slotsused = used;
        }
    }

    i32 TriggerAutoSave(void) {
        if (memcard_autosave != 0 && memcard_autosaveenabled != 0 && saveload_autosave != -1) {
            memcard_autosaveneeded = 1;
            MenuSaveOccurred = 0;
            return 1;
        }
        return 0;
    }

}

bool TriggerExtraDataSave(void) {
    memmove(memcard_extra_savedatabuffer, memcard_extra_savedata, memcard_extra_savedatasize);

    i32 checksum = ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
    *(i32 *)((char *)memcard_extra_savedatabuffer + memcard_extra_savedatasize) = checksum;

    i32 save_error = saveloadSaveSlot(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4);
    return save_error != 0;
}

i32 TriggerExtraDataLoad(void) {
    if (saveloadLoadSlot(SAVESLOTS, memcard_extra_savedatabuffer, memcard_extra_savedatasize + 4) != 0) {
        i32 correct = ChecksumSaveData(memcard_extra_savedatabuffer, memcard_extra_savedatasize);
        i32 checksum = *(i32 *)((char *)memcard_extra_savedatabuffer + memcard_extra_savedatasize);
        LOG_DEBUG("checksum=%08X, correct=%08X", checksum, correct);
        if (correct == checksum) {
            memmove(memcard_extra_savedata, memcard_extra_savedatabuffer, memcard_extra_savedatasize);
            return 1;
        }
    }

    return 0;
}

i32 getsaveload_status() {
    return saveload_status;
}

void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font) {
    u16 character;

    while (*src != 0) {
        src = NuUnicodeCharFromUTF8(&character, src);
        if (character == '~') {
            if (*src == 0)
                break;
            src = NuUnicodeCharFromUTF8(&character, src);
            continue;
        }

        *dst = NuQFntEncodeUnicodeChar(font, character);
        if (*dst == 0xffff) {
            NuUnicodeCharFromUTF8(&character, (unsigned char *)"\xe2\x96\xa1");
            *dst = NuQFntEncodeUnicodeChar(font, character);
            if (*dst == 0xffff)
                *dst = NuQFntEncodeUnicodeChar(font, '?');
        }
        dst++;
    }
    *dst = 0;
}

extern "C" {
extern "C" void Text3DStringEncode(char *src, u16 *dst) {
    VUFNT *font = SmartTextFont;
    if (font == nullptr)
        font = QFont2D;
    Text3DStringEncodeFont(reinterpret_cast<unsigned char *>(src), dst, font);
}

    void TextRegisterButtonMapFn(void (*fn)(char *, char *)) {
        if (fn != nullptr) {
            buttonmapfn = fn;
        }
    }

    void TextRegisterPulseTimerFn(f32 (*fn)(f32)) {
        if (fn != nullptr) {
            pulsetimerfn = fn;
        }
    }

}

void TextDecodeCodeword(char *source, char *destination) {
    if (buttonmapfn != nullptr)
        buttonmapfn(source, source);

    if (NuStrICmp(source, "accept") == 0)
        NuStrCpy(source, "cross");
    if (NuStrICmp(source, "back") == 0)
        NuStrCpy(source, "triangle");
    if (NuStrICmp(source, "wii") == 0)
        NuStrCpy(source, "logo");
    if (NuStrICmp(source, "playstation") == 0)
        NuStrCpy(source, "logo");

    NuStrCpy(destination, source);
    if (NuStrICmp(source, "cross") == 0)
        NuStrCpy(destination, "~0\xd4\xb1~~");
    if (NuStrICmp(source, "circle") == 0)
        NuStrCpy(destination, "~0\xd4\xb2~~");
    if (NuStrICmp(source, "square") == 0)
        NuStrCpy(destination, "~0\xd4\xb3~~");
    if (NuStrICmp(source, "triangle") == 0)
        NuStrCpy(destination, "~0\xd4\xb4~~");
    if (NuStrICmp(source, "l1") == 0)
        NuStrCpy(destination, "~0\xd4\xb5~~");
    if (NuStrICmp(source, "r1") == 0)
        NuStrCpy(destination, "~0\xd4\xb6~~");
    if (NuStrICmp(source, "l2") == 0)
        NuStrCpy(destination, "~0\xd4\xb7~~");
    if (NuStrICmp(source, "r2") == 0)
        NuStrCpy(destination, "~0\xd4\xb8~~");
    if (NuStrICmp(source, "select") == 0)
        NuStrCpy(destination, "~0\xd4\xbd~~");
    if (NuStrICmp(source, "start") == 0)
        NuStrCpy(destination, "~0\xd4\xbe~~");
    if (NuStrICmp(source, "home") == 0)
        NuStrCpy(destination, "~0\xd4\xbf~~");
    if (NuStrICmp(source, "a") == 0)
        NuStrCpy(destination, "~0\xd4\xb1~~");
    if (NuStrICmp(source, "b") == 0)
        NuStrCpy(destination, "~0\xd4\xb2~~");
    if (NuStrICmp(source, "x") == 0)
        NuStrCpy(destination, "~0\xd4\xb3~~");
    if (NuStrICmp(source, "y") == 0)
        NuStrCpy(destination, "~0\xd4\xb4~~");
    if (NuStrICmp(source, "lb") == 0)
        NuStrCpy(destination, "~0\xd4\xb5~~");
    if (NuStrICmp(source, "rb") == 0)
        NuStrCpy(destination, "~0\xd4\xb6~~");
    if (NuStrICmp(source, "lt") == 0)
        NuStrCpy(destination, "~0\xd4\xb7~~");
    if (NuStrICmp(source, "rt") == 0)
        NuStrCpy(destination, "~0\xd4\xb8~~");
    if (NuStrICmp(source, "back") == 0)
        NuStrCpy(destination, "~0\xd4\xbd~~");
    if (NuStrICmp(source, "guide") == 0)
        NuStrCpy(destination, "~0\xd4\xbf~~");
    if (NuStrICmp(source, "xboxguide") == 0)
        NuStrCpy(destination, "~0\xd4\xbf~~");
    if (NuStrICmp(source, "xbox guide") == 0)
        NuStrCpy(destination, "~0\xd4\xbf~~");
    if (NuStrICmp(source, "xbox") == 0)
        NuStrCpy(destination, "~0\xd4\xbf~~");
    if (NuStrICmp(source, "1") == 0)
        NuStrCpy(destination, "~0\xd4\xb3~~");
    if (NuStrICmp(source, "2") == 0)
        NuStrCpy(destination, "~0\xd4\xb4~~");
    if (NuStrICmp(source, "c") == 0)
        NuStrCpy(destination, "~0\xd4\xb5~~");
    if (NuStrICmp(source, "z") == 0)
        NuStrCpy(destination, "~0\xd4\xb6~~");
    if (NuStrICmp(source, "-") == 0)
        NuStrCpy(destination, "~0\xd4\xbd~~");
    if (NuStrICmp(source, "+") == 0)
        NuStrCpy(destination, "~0\xd4\xbe~~");
    if (NuStrICmp(source, "paddown") == 0)
        NuStrCpy(destination, "~0\xd4\xb9~~");
    if (NuStrICmp(source, "padright") == 0)
        NuStrCpy(destination, "~0\xd4\xba~~");
    if (NuStrICmp(source, "padleft") == 0)
        NuStrCpy(destination, "~0\xd4\xbb~~");
    if (NuStrICmp(source, "padup") == 0)
        NuStrCpy(destination, "~0\xd4\xbc~~");
    if (NuStrICmp(source, "logo") == 0)
        NuStrCpy(destination, "\xd5\x80");
}

extern "C" {
    void TextDecode(char *source, unsigned char *dest) {
        i32 source_pos = 0;
        i32 dest_pos = 0;
        const unsigned char *text = reinterpret_cast<unsigned char *>(source);
        do {
            if (text[source_pos] == '\0')
                break;
            if (text[source_pos] == '[' && text[source_pos + 1] == '[') {
                source_pos += 2;
                char codeword[512];
                i32 codeword_len = 0;
                while (text[source_pos] != '\0' && text[source_pos] != ']')
                    codeword[codeword_len++] = static_cast<char>(text[source_pos++]);
                while (text[source_pos] == ']')
                    source_pos++;
                codeword[codeword_len] = '\0';
                char decoded[512];
                TextDecodeCodeword(codeword, decoded);
                for (i32 i = 0; decoded[i] != '\0'; i++)
                    dest[dest_pos++] = static_cast<unsigned char>(decoded[i]);
                continue;
            }

            // Wide CJK punctuation is followed by a space so that lines can break after it.
            if (text[source_pos] == 0xe3 && text[source_pos + 1] == 0x80) {
                switch (text[source_pos + 2]) {
                    case 0x80:
                        dest[dest_pos++] = ' ';
                        source_pos += 3;
                        continue;
                    case 0x81:
                    case 0x82:
                    case 0x85:
                        dest[dest_pos] = 0xe3;
                        dest[dest_pos + 1] = text[source_pos + 1];
                        dest[dest_pos + 2] = text[source_pos + 2];
                        dest[dest_pos + 3] = ' ';
                        dest_pos += 4;
                        source_pos += 3;
                        continue;
                }
            } else if (text[source_pos] == 0xef && text[source_pos + 1] == 0xbc) {
                switch (text[source_pos + 2]) {
                    case 0x81:
                    case 0x9f:
                        dest[dest_pos] = 0xef;
                        dest[dest_pos + 1] = text[source_pos + 1];
                        dest[dest_pos + 2] = text[source_pos + 2];
                        dest[dest_pos + 3] = ' ';
                        dest_pos += 4;
                        source_pos += 3;
                        continue;
                }
            }

            dest[dest_pos++] = text[source_pos++];
            while (static_cast<u8>(text[source_pos] + 0x80) < 0x40)
                dest[dest_pos++] = text[source_pos++];
        } while (dest_pos < 0x1e0);
        dest[dest_pos] = 0;
    }

}

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font) {
    VUFNT *base_font = SmartTextFont != nullptr ? SmartTextFont : QFont2D;
    VUFNT *font;
    f32 relative_scale;
    f32 pulse;
    if (button_font == 0 || QFont2DButtons == nullptr) {
        font = base_font;
        pulse = 1.0f;
        relative_scale = 1.0f;
    } else {
        font = QFont2DButtons;
        pulse = g_buttonFontScalePulse;
        relative_scale = base_font->height / QFont2DButtons->height;
    }

    NuQFntSet(font);
    NuQFntSetScale(font, x_scale * QFONTSCALEX * relative_scale * pulse,
                   y_scale * QFONTSCALEY * relative_scale * pulse);
    u16 encoded[510];
    Text3DStringEncodeFont(text, encoded, font);
    f32 width = NuQFntPrintLenW(font, encoded);
    f32 height = NuQFntHeight(font);
    NuQFntSetColour(font, colour);
    NuQFntMove(font, width / (pulse + pulse) * (1.0f - pulse) + x, height / (pulse + pulse) * (1.0f - pulse) + y, z);
    NuQFntPrintW(font, encoded);
    return width / pulse;
}

extern "C" {
    void SetButtonScaleMode(i32 mode) {
        ButtonScaleMode = mode;
    }

    void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha) {
        text3d_width = text3d_height = 0.0f;
        if ((SmartTextFont == NULL && QFont2D == NULL) || text == NULL || text[0] == '\0' || MenuStopDraw != 0)
            return;
        // The reference captures API scales before callbacks and reloads the active font for each operation.
        f32 draw_x_scale = APITEXTSCALEX;
        f32 draw_y_scale = APITEXTSCALEY;
        if (x < -2.0f || x > 2.0f || y < -2.0f || y > 2.0f)
            return;
        if (smarttext_fwn != 0)
            NuQFntPushPrintMode(3);
        else
            NuQFntPushPrintMode(2);
        unsigned char decoded[512];
        TextDecode(text, decoded);
        if (__builtin_expect(followon_line == 0, 1))
            NuQFntSet(SmartTextFont != NULL ? SmartTextFont : QFont2D);
        draw_x_scale *= x_scale;
        draw_y_scale *= y_scale;
        u16 encoded[512];
        Text3DStringEncode(reinterpret_cast<char *>(decoded), encoded);
        NuQFntSetScale(SmartTextFont != NULL ? SmartTextFont : QFont2D, draw_x_scale * QFONTSCALEX,
                       draw_y_scale * QFONTSCALEY);
        f32 width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
        text3d_width = width;
        f32 height = NuQFntHeight(SmartTextFont != NULL ? SmartTextFont : QFont2D);
        text3d_height = height;
        y += NuQFntBaseline(SmartTextFont != NULL ? SmartTextFont : QFont2D);
        f32 draw_y = y - height * 0.5f;
        f32 button_scale = draw_x_scale;
        if (draw_x_scale != draw_y_scale) {
            if (__builtin_expect(ButtonScaleMode == 1, 0))
                button_scale = draw_x_scale;
            else if (ButtonScaleMode == 2)
                button_scale = draw_y_scale;
            else
                button_scale = 0.5f * (draw_x_scale + draw_y_scale);
            unsigned char buttons[512], normal[512];
            i32 button_count = 0, normal_count = 0;
            for (i32 i = 0; decoded[i] != 0;) {
                if (decoded[i] == 0xd4 && decoded[i + 1] >= 0xb1 && decoded[i + 1] <= 0xbf) {
                    buttons[button_count++] = decoded[i++];
                    while (static_cast<u8>(decoded[i] - 0x80) <= 0x3f)
                        buttons[button_count++] = decoded[i++];
                } else {
                    normal[normal_count++] = decoded[i++];
                    while (static_cast<u8>(decoded[i] - 0x80) <= 0x3f)
                        normal[normal_count++] = decoded[i++];
                }
            }
            buttons[button_count] = normal[normal_count] = 0;
            f32 button_width = 0.0f, normal_width = 0.0f;
            u16 button_encoded[512], normal_encoded[512];
            if (button_count != 0) {
                Text3DStringEncode(reinterpret_cast<char *>(buttons), button_encoded);
                NuQFntSetScale(SmartTextFont != NULL ? SmartTextFont : QFont2D, button_scale * QFONTSCALEX,
                               button_scale * QFONTSCALEY);
                button_width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, button_encoded);
            }
            if (normal_count != 0) {
                Text3DStringEncode(reinterpret_cast<char *>(normal), normal_encoded);
                NuQFntSetScale(SmartTextFont != NULL ? SmartTextFont : QFont2D, draw_x_scale * QFONTSCALEX,
                               draw_y_scale * QFONTSCALEY);
                normal_width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, normal_encoded);
            }
            if (__builtin_expect(normal_width == 0.0f, 1)) {
                button_scale = draw_x_scale;
                if (ButtonScaleMode == 2) {
                    width = draw_y_scale * width / draw_x_scale;
                    button_scale = draw_y_scale;
                }
            } else {
                draw_x_scale = (width - button_width) / normal_width * draw_x_scale;
            }
        }
        if ((alignment & 4) != 0)
            draw_y -= height * 0.5f;
        else if ((alignment & 1) != 0)
            draw_y += height * 0.5f;
        if ((alignment & 2) == 0)
            x -= (alignment & 8) != 0 ? width : width * 0.5f;
        const u32 opacity = static_cast<u32>(alpha) << 24;
        if (followon_line == 0) {
            if (apitext_half_rgb == 0)
                current_rgba = opacity | red | (static_cast<u32>(green) << 8) | (static_cast<u32>(blue) << 16);
            else
                current_rgba =
                    opacity | (red >> 1) | (static_cast<u32>(green >> 1) << 8) | (static_cast<u32>(blue >> 1) << 16);
        }
        unsigned char fragment[1024];
        i32 count = 0;
        bool button_font = false;
        i32 i = 0;
        do {
            if (decoded[i] == 0)
                break;
            if (decoded[i] == '~' && (decoded[i + 1] == '~' || (decoded[i + 1] >= '0' && decoded[i + 1] <= '9'))) {
                u32 colour;
                if (decoded[i + 1] != '~') {
                    colour = opacity;
                    if (red != 0 || green != 0 || blue != 0) {
                        const i32 preset = decoded[i + 1] - '0';
                        if (apitext_half_rgb == 0)
                            colour |= static_cast<u8>(text_preset_r[preset] * 2) |
                                      (static_cast<u32>((text_preset_g[preset] * 2) & 255) << 8) |
                                      (static_cast<u32>((text_preset_b[preset] * 2) & 255) << 16);
                        else
                            colour |= text_preset_r[preset] | (static_cast<u32>(text_preset_g[preset]) << 8) |
                                      (static_cast<u32>(text_preset_b[preset]) << 16);
                    }
                } else {
                    colour = opacity | red | (static_cast<u32>(green) << 8) | (static_cast<u32>(blue) << 16);
                    if (apitext_half_rgb != 0)
                        colour = opacity | (red >> 1) | (static_cast<u32>(green >> 1) << 8) |
                                 (static_cast<u32>(blue >> 1) << 16);
                }
                i += 2;
                if (__builtin_expect(count != 0 && colour != current_rgba, 0)) {
                    fragment[count] = 0;
                    x += TextPrintSubstring(fragment, x, draw_y, z, button_font ? button_scale : draw_x_scale,
                                            button_font ? button_scale : draw_y_scale, current_rgba, button_font);
                    count = 0;
                }
                current_rgba = colour;
                continue;
            }
            const bool next_button = decoded[i] == 0xd4 && decoded[i + 1] >= 0xb1 && decoded[i + 1] <= 0xbf;
            if (next_button) {
                if (!button_font && count != 0) {
                    fragment[count] = 0;
                    x += TextPrintSubstring(fragment, x, draw_y, z, draw_x_scale, draw_y_scale, current_rgba, 0);
                    count = 0;
                }
                fragment[count++] = decoded[i++];
                while (static_cast<u8>(decoded[i] - 0x80) <= 0x3f)
                    fragment[count++] = decoded[i++];
                button_font = true;
            } else {
                if (button_font && count != 0) {
                    fragment[count] = 0;
                    x += TextPrintSubstring(fragment, x, draw_y, z, button_scale, button_scale, current_rgba, 1);
                    count = 0;
                }
                fragment[count++] = decoded[i++];
                while (static_cast<u8>(decoded[i] - 0x80) <= 0x3f)
                    fragment[count++] = decoded[i++];
                button_font = false;
            }
        } while (count < 509);
        if (count != 0) {
            fragment[count] = 0;
            TextPrintSubstring(fragment, x, draw_y, z, button_font ? button_scale : draw_x_scale,
                               button_font ? button_scale : draw_y_scale, current_rgba, button_font);
        }
        NuQFntPopPrintMode();
    }

    void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue) {
        Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, 128);
    }

    void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                   u8 green, u8 blue, i32 alpha) {
        if (QFont2DLower == nullptr) {
            return;
        }

        VUFNT *saved_font = SmartTextFont;
        SmartTextFont = QFont2DLower;
        Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, alpha);
        SmartTextFont = saved_font;
    }

    void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, u8 alpha) {
        if (MenuDrawDropShadows != 0) {
            Text3DEx2(text, x + x_scale * 0.015f, y - y_scale * 0.015f, z, x_scale, y_scale, z_scale, alignment, 0, 0,
                      0, alpha >> 2);
        }
        Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, alpha);
    }

}

void APIMenuDrawMemCardSlots(MENU *menu, f32 y) {
    if (slideright > 0) {
        --slideright;
    }
    if (slideleft > 0) {
        --slideleft;
    }

    i32 count = SAVESLOTS;
    i32 first, last;
    f32 x;
    if (count > 3) {
        if (menu->selected_column != lastslot) {
            if (menu->selected_column < lastslot) {
                slideright = 10;
            } else {
                slideleft = 10;
            }
            lastslot = menu->selected_column;
        }
        const f32 right_offset = slideright * 0.5f / 10.0f;
        const f32 left_offset = slideleft * 0.5f / 10.0f;
        last = slideright != 0 ? menu->selected_column + 2 : menu->selected_column + 1;
        first = slideleft != 0 ? menu->selected_column - 2 : menu->selected_column - 1;
        if (count > 6) {
            count = memcard_slotsused;
        }
        x = 0.0f - (menu->selected_column - first) * 0.5f - right_offset + left_offset;
    } else {
        first = 0;
        last = count - 1;
        x = (count - 1) * -0.25f;
    }

    auto menu_first_row = menu->first_row;
    for (i32 slot = first; slot <= last; ++slot, x += 0.5f) {
        const i32 highlight =
            menu->selected_row == menu_first_row && menu->selected_column == slot && slideleft == 0 && slideright == 0;
        if (slot < 0 || slot > menu->last_column) {
            continue;
        }
        if (!(slot < count)) {
            i32 red, green, blue;
            if (menu->selected_column == count && menu->selected_row == 0 && TestForController()) {
                if (menu_pulsate > 0.0f) {
                    red = MenuSlotColourLerp(MENUFLASH0R, MENUFLASH1R, menu_pulsate);
                    green = MenuSlotColourLerp(MENUFLASH0G, MENUFLASH1G, menu_pulsate);
                    blue = MenuSlotColourLerp(MENUFLASH0B, MENUFLASH1B, menu_pulsate);
                } else if (menu_flash != 0) {
                    red = MENUFLASH0R;
                    blue = MENUFLASH0B;
                    green = MENUFLASH0G;
                } else {
                    red = MENUFLASH1R;
                    blue = MENUFLASH1B;
                    green = MENUFLASH1G;
                }
            } else if (menu_pulse > 0.0f) {
                red = MenuSlotColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                green = MenuSlotColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                blue = MenuSlotColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }
            MenuText3DEx(apitxt_NEWSAVE, x, y, 1.0f, MENUTEXTSCALE * 0.85f, MENUTEXTSCALE * 0.85f, MENUTEXTSCALE, 0,
                         red, green, blue, static_cast<u8>(MenuA));
        } else {
            drawslotinfofn(x, y, highlight, slot);
        }
    }

    i32 red, green, blue;
    if (menu->selected_row == 0 && TestForController()) {
        if (menu_pulsate > 0.0f) {
            red = MenuSlotColourLerp(MENUFLASH0R, MENUFLASH1R, menu_pulsate);
            green = MenuSlotColourLerp(MENUFLASH0G, MENUFLASH1G, menu_pulsate);
            blue = MenuSlotColourLerp(MENUFLASH0B, MENUFLASH1B, menu_pulsate);
        } else if (menu_flash != 0) {
            red = MENUFLASH0R;
            blue = MENUFLASH0B;
            green = MENUFLASH0G;
        } else {
            red = MENUFLASH1R;
            blue = MENUFLASH1B;
            green = MENUFLASH1G;
        }
    } else if (menu_pulse > 0.0f) {
        red = MenuSlotColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
        green = MenuSlotColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
        blue = MenuSlotColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
    } else {
        red = MENUENTRYR;
        green = MENUENTRYG;
        blue = MENUENTRYB;
    }
    f32 scale = MENUTEXTSCALE;
    if (slideleft == 0 && slideright == 0) {
        scale *= 2.0f;
        if (first > 0) {
            MenuText3DEx("<", -0.8f, y, 1.0f, scale, scale, scale, 0, red, green, blue, static_cast<u8>(MenuA));
        }
        if (menu->last_column > last) {
            MenuText3DEx(">", 0.8f, y, 1.0f, scale, scale, scale, 0, red, green, blue, static_cast<u8>(MenuA));
        }
    }
}

// The break index is a byte offset. Never return a UTF-8 continuation byte.
i32 FindNextBreak(unsigned char *text, i32 index) {
    i32 length = NuStrLen(reinterpret_cast<char *>(text));
    while (static_cast<u8>(text[index] - 0x80) <= 0x3f)
        --index;

    u8 ch = text[index];
    if (ch == ' ') {
        u8 next = text[index + 1];
        if (next == '?')
            goto begin_scan;
        if (next == '!')
            goto begin_scan;
        if (next == ';')
            goto begin_scan;
        if (next == ':')
            goto begin_scan;
        return index;
    } else if (ch == '.') {
        if (text[index + 1] != '.')
            return index;
    } else if (static_cast<u8>(ch - ',') <= 1) {
        return index;
    }

begin_scan:
    i32 count = 20;
    do {
        ++index;
        if (index >= length)
            return length;
        while (static_cast<u8>(text[index] - 0x80) <= 0x3f)
            ++index;
        ch = text[index];
        if (ch == ' ') {
            u8 next = text[index + 1];
            if (next == '?')
                goto continue_scan;
            if (next == '!')
                goto continue_scan;
            if (next == ';')
                goto continue_scan;
            if (next == ':')
                goto continue_scan;
            return index;
        } else if (ch == '.') {
            if (text[index + 1] != '.')
                return index;
        } else if (static_cast<u8>(ch - ',') <= 1) {
            return index;
        }
    continue_scan:;
    } while (--count != 0);

    if (text[index] == '~') {
        if (index > 0 && text[index - 1] == '~')
            index -= 1;
    } else if (index > 0 && text[index - 1] == '~' && index != 1) {
        index -= text[index - 2] != '~';
    }
    return index;
}

i32 FindNearestBreak(unsigned char *text, i32 index) {
    while (static_cast<u8>(text[index] - 0x80) <= 0x3f)
        --index;

    if (IsTextBreak(text, index))
        return index;

    i32 length = NuStrLen(reinterpret_cast<char *>(text)) - 1;
    i32 backward = index;
    i32 forward = index;
    i32 count = 20;
    do {
        forward += forward < length;
        while (static_cast<u8>(text[forward] - 0x80) <= 0x3f)
            ++forward;
        backward -= backward > 0;
        while (static_cast<u8>(text[backward] - 0x80) <= 0x3f)
            --backward;

        if (IsTextBreak(text, forward))
            return forward;
        if (IsTextBreak(text, backward))
            return backward;
    } while (--count != 0);

    if (text[index] == '~') {
        if (index > 0 && text[index - 1] == '~')
            index -= 1;
    } else if (index > 0 && text[index - 1] == '~' && index != 1) {
        index -= text[index - 2] != '~';
    }
    return index;
}

extern "C" {
extern "C" void MessageBoxInitMtl(void) {
    MessageMtl = NuMtlCreate(1);
    MessageMtl->attribs.z_mode = 3;
    MessageMtl->attribs.alpha_mode = 1;
    MessageMtl->attribs.unknown_2_1_2 = 2;
    MessageMtl->attribs.unknown_1_1_2 = 1;
    MessageMtl->attribs.unknown_1_4_8 = 1;
    MessageMtl->attribs.unknown_2_4 = 1;
    MessageMtl->attribs.filter_mode = 1;
    NuMtlUpdate(MessageMtl);
    MessageMtlInit = 1;
}

}

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale) {
    const f32 scaled_width = width * scale;
    const f32 scaled_height = height * scale;

    f32 pulse = 1.0f;
    if (pulsetimerfn != NULL) {
        pulse = (pulsetimerfn((1.0f - y) * 0.5f) - 1.0f) * 4.0f + 1.0f;
    }

    i32 opacity = static_cast<i32>(static_cast<f32>(alpha) * pulse);

    const i32 box_width = static_cast<i32>(scaled_width * 10240.0f * 0.5f);
    const i32 box_height = static_cast<i32>(scaled_height * 3584.0f * 0.5f);
    const i32 edge_width = static_cast<i32>(204.79999f * QFONTSCALEX * 0.75f);
    const i32 edge_height = static_cast<i32>(71.68f * QFONTSCALEY);
    const i32 corner_width = static_cast<i32>(40.960003f * QFONTSCALEX * 0.75f);
    const i32 corner_height = static_cast<i32>(14.336f * QFONTSCALEY);

    if ((alignment & 5) == 4) {
        y += scaled_height;
    } else if ((alignment & 5) == 0) {
        y += scaled_height * 0.5f;
    }
    if ((alignment & 10) == 8) {
        x -= scaled_width;
    } else if ((alignment & 10) == 0) {
        x -= scaled_width * 0.5f;
    }

    i32 screen_x = static_cast<i32>((x + 1.0f) * 0.5f * 10240.0f);
    i32 screen_y = static_cast<i32>((2.0f - (y + 1.0f)) * 0.5f * 3584.0f);
    const u32 colour = (static_cast<u32>(opacity) << 24) | ((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff);
    const u32 edge_colour = (static_cast<u32>(opacity) << 25) | 0xffffff;

    NuRndrRect2di(screen_x, screen_y, box_width, box_height, colour, material);
    NuRndrRect2di(screen_x, screen_y, edge_width, corner_height, edge_colour, material);
    NuRndrRect2di(screen_x, screen_y, corner_width, edge_height, edge_colour, material);
    screen_y += box_height;
    screen_x += box_width;
    NuRndrRect2di(screen_x, screen_y, -edge_width, -corner_height, edge_colour, material);
    NuRndrRect2di(screen_x, screen_y, -corner_width, -edge_height, edge_colour, material);
}

void DrawMessageBox(i32, f32 x, f32 y, f32 width, f32 height) {
    if (MessageMtlInit == 0) {
        MessageBoxInitMtl();
    }

    if (NuRndrBeginScene(-1) != 0) {
        const i32 box_width = static_cast<i32>(width * 10240.0f * 0.5f);
        const i32 box_height = static_cast<i32>(height * 3584.0f * 0.5f);
        const i32 screen_x = static_cast<i32>((x + 1.0f) * 0.5f * 10240.0f) - box_width / 2;
        const i32 screen_y = static_cast<i32>((y + 1.0f) * 0.5f * 3584.0f) - box_height / 2;

        NuRndrRect2di(screen_x, screen_y, box_width, box_height, 0x80808080, MessageMtl);

        u32 colours[4] = {0x00808080, 0x80808080, 0x00808080, 0x80808080};
        NuRndrGradRectUV2di(screen_x - 30, screen_y, 60, box_height, 0.0f, 0.0f, 0.0f, 1.0f, colours, MessageMtl);

        colours[0] = 0x80808080;
        colours[1] = 0x00808080;
        colours[2] = 0x80808080;
        colours[3] = 0x00808080;
        NuRndrGradRectUV2di(screen_x + box_width, screen_y, 60, box_height, 1.0f, 0.0f, 1.0f, 1.0f, colours,
                            MessageMtl);

        colours[0] = 0x00808080;
        colours[1] = 0x00808080;
        colours[2] = 0x80808080;
        colours[3] = 0x80808080;
        NuRndrGradRectUV2di(screen_x, screen_y - 7, box_width, 30, 0.0f, 0.0f, 1.0f, 0.0f, colours, MessageMtl);

        colours[0] = 0x80808080;
        colours[1] = 0x80808080;
        colours[2] = 0x00808080;
        colours[3] = 0x00808080;
        NuRndrGradRectUV2di(screen_x, screen_y + box_height - 15, box_width, 30, 0.0f, 1.0f, 1.0f, 1.0f, colours,
                            MessageMtl);
        NuRndrEndScene();
    }
}

extern "C" {
    void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha) {
        struct BoxGeometry {
            f32 reserved, x, y, z, width, height;
        };
        BoxGeometry local_box;
        unsigned char decoded[512];
        unsigned char candidate[512];
        u16 encoded[512];
        unsigned char lines[8][512];
        lines[0][0] = '\0';
        if (!(SmartTextFont == NULL && QFont2D == NULL) && text != NULL && text[0] != '\0' && MenuStopDraw == 0) {

            const f32 saved_x_scale = APITEXTSCALEX;
            f32 draw_x_scale = saved_x_scale * x_scale;
            const f32 saved_y_scale = APITEXTSCALEY;
            y_scale = saved_y_scale * y_scale;
            APITEXTSCALEX = 1.0f;
            APITEXTSCALEY = 1.0f;
            BoxGeometry *box = static_cast<BoxGeometry *>(message_box);
            if (smarttextex_drawmessagebox != 0 && box == NULL)
                box = &local_box;
            x *= STCOORDSCALE;
            f32 available_width = STCOORDSCALE * max_width;
            TextDecode(text, decoded);
            Text3DStringEncode(reinterpret_cast<char *>(decoded), encoded);
            NuQFntSet(SmartTextFont != NULL ? SmartTextFont : QFont2D);
            NuQFntSetScale(SmartTextFont != NULL ? SmartTextFont : QFont2D, draw_x_scale * QFONTSCALEX,
                           y_scale * QFONTSCALEY);
            f32 width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
            f32 line_height = NuQFntHeight(SmartTextFont != NULL ? SmartTextFont : QFont2D);
            NuQFntBaseline(SmartTextFont != NULL ? SmartTextFont : QFont2D);
            f32 preferred_width;
            if (available_width <= 0.0f) {
                available_width = 1.5f;
                preferred_width = 1.3499999f;
            } else {
                preferred_width = available_width * 0.9f;
            }

            i32 explicit_breaks = 0;
            for (i32 pos = 0; decoded[pos] != '\0';) {
                if (decoded[pos] == '\\') {
                    ++pos;
                    explicit_breaks += decoded[pos] == 'n';
                }
                ++pos;
                while (static_cast<u8>(decoded[pos] - 0x80) <= 0x3f)
                    ++pos;
            }
            i32 last_line = static_cast<i32>(width / available_width);
            if (last_line < 0)
                last_line = 0;
            if (preferred_width < width / static_cast<f32>(last_line + 1))
                ++last_line;
            bool constrained = false;
            if (max_lines > 8)
                max_lines = 8;
            if (max_lines > 0 && max_lines <= last_line) {
                last_line = max_lines - 1;
                constrained = true;
            }
            const i32 characters = NuStrLenU(decoded);
            const i32 bytes = NuStrLen(reinterpret_cast<char *>(decoded));
            i32 widest_line = -1;
            if (explicit_breaks != 0) {
                width = 0.0f;
                i32 line = 0;
                i32 begin = 0;
                do {
                    while (decoded[begin] == ' ')
                        ++begin;
                    i32 end = begin;
                    while (end < bytes) {
                        if (decoded[end] == '\\' && decoded[end + 1] == 'n')
                            break;
                        ++end;
                    }
                    i32 length = end - begin;
                    if (length > 511)
                        length = 511;
                    memcpy(lines[line], decoded + begin, length);
                    lines[line][length] = '\0';
                    Text3DStringEncode(reinterpret_cast<char *>(lines[line]), encoded);
                    const f32 line_width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
                    if (width < line_width) {
                        widest_line = line;
                        width = line_width;
                    }
                    ++line;
                    begin = end + 2;
                } while (line <= explicit_breaks);
                last_line = explicit_breaks;
                last_line -= lines[last_line][0] == '\0';
            } else if (last_line != 0) {
                width = 0.0f;
                if ((alignment & 10) != 0 && !constrained) {
                    i32 end = 0;
                    i32 begin = 0;
                    i32 line = 0;
                    do {
                        while (decoded[begin] == ' ')
                            ++begin;
                        if (line == last_line) {
                            end = bytes - 1;
                        } else {
                            i32 next = begin;
                            if (available_width > 0.0f) {
                                do {
                                    end = next;
                                    next = end + 1;
                                    while (decoded[next] == ' ')
                                        ++next;
                                    if (next == bytes) {
                                        end = bytes - 1;
                                        break;
                                    }
                                    next = FindNextBreak(decoded, next);
                                    if (static_cast<u8>(decoded[next] - ',') > 2) {
                                        --next;
                                        while (decoded[next] == ' ')
                                            --next;
                                    }
                                    i32 length = next - begin + 1;
                                    if (length > 511)
                                        length = 511;
                                    memcpy(candidate, decoded + begin, length);
                                    candidate[length] = '\0';
                                    Text3DStringEncode(reinterpret_cast<char *>(candidate), encoded);
                                } while (NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded) <
                                         available_width);
                            }
                        }
                        i32 length = end - begin + 1;
                        if (length > 511)
                            length = 511;
                        memcpy(lines[line], decoded + begin, length);
                        lines[line][length] = '\0';
                        Text3DStringEncode(reinterpret_cast<char *>(lines[line]), encoded);
                        const f32 line_width =
                            NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
                        if (width < line_width) {
                            widest_line = line;
                            width = line_width;
                        }
                        begin = end + 1;
                    } while (++line <= last_line);
                } else {
                    const i32 count = last_line + 1;
                    i32 line = 0;
                    do {
                        i32 begin = 0;
                        if (line != 0)
                            begin = FindNearestBreak(decoded, NuStrFindPosU(decoded, (characters / count) * line));
                        i32 end = bytes;
                        if (line != last_line)
                            end = FindNearestBreak(decoded, NuStrFindPosU(decoded, (characters / count) * (line + 1)));
                        if (static_cast<u8>(decoded[end] - ',') > 2) {
                            --end;
                            while (decoded[end] == ' ')
                                --end;
                        }
                        while (decoded[begin] == '-' || decoded[begin] == ' ' || decoded[begin] == '.' ||
                               decoded[begin] == ',')
                            ++begin;
                        i32 length = end - begin + 1;
                        if (length > 511)
                            length = 511;
                        memcpy(lines[line], decoded + begin, length);
                        lines[line][length] = '\0';
                        Text3DStringEncode(reinterpret_cast<char *>(lines[line]), encoded);
                        const f32 line_width =
                            NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
                        if (width < line_width) {
                            widest_line = line;
                            width = line_width;
                        }
                    } while (++line <= last_line);
                }
                last_line -= lines[last_line][0] == '\0';
            }
            if (available_width < width) {
                smarttextex_longestwidth = available_width;
                draw_x_scale = (available_width / width) * draw_x_scale;
            } else {
                smarttextex_longestwidth = width;
            }
            if (box != NULL) {
                NuQFntSetScale(SmartTextFont != NULL ? SmartTextFont : QFont2D, draw_x_scale, y_scale);
                f32 box_width;
                if (widest_line != -1) {
                    Text3DStringEncode(reinterpret_cast<char *>(lines[widest_line]), encoded);
                    box_width = NuQFntPrintLenW(SmartTextFont != NULL ? SmartTextFont : QFont2D, encoded);
                } else {
                    box_width = smarttextex_longestwidth;
                }
                line_height -= 0.01f;
                box->x = x;
                box->y = y;
                box_width = 0.03f * QFONTSCALEX * 0.75f + box_width;
                box->width = box_width;
                const f32 box_height = -static_cast<f32>(last_line + 1) * line_height;
                box->height = box_height;
                const i32 mode = smarttextex_drawmessagebox;
                f32 box_y = y;
                f32 box_x = x;
                if (mode == 1) {
                    if ((alignment & 2) != 0)
                        box_x = box->x = x + 0.5f * box_width;
                    else if ((alignment & 8) != 0)
                        box_x = box->x = x - 0.5f * box_width;
                    if ((alignment & 1) != 0)
                        box_y = box->y = y + 0.5f * line_height;
                    else if ((alignment & 4) != 0)
                        box_y = box->y = y - 0.5f * line_height;
                }
                if (mode != 0) {
                    DrawMessageBoxRGBA(box_x, box_y, box_width, static_cast<f32>(mode) * box_height, 0, 127, 255,
                                       static_cast<i32>(static_cast<f32>(static_cast<i32>(alpha)) * 0.25f), MenuFadeMtl,
                                       0, 1.0f);
                    smarttextex_drawmessagebox = 0;
                }
            }
            followon_line = 0;
            if (suppress_draw == 0) {
                if (last_line != 0) {
                    f32 first_y;
                    if ((alignment & 1) != 0)
                        first_y = y;
                    else if ((alignment & 4) != 0)
                        first_y = y - static_cast<f32>(last_line) * line_height;
                    else
                        first_y = y - static_cast<f32>(last_line) * 0.5f * line_height;
                    for (i32 line = 0; line <= last_line; ++line) {
                        if (line != 0)
                            followon_line = 1;
                        Text3DEx(reinterpret_cast<char *>(lines[line]), x,
                                 static_cast<f32>(line) * line_height + first_y, z, draw_x_scale, y_scale, z_scale,
                                 alignment, red, green, blue, alpha & 0xff);
                    }
                } else {
                    Text3DEx(reinterpret_cast<char *>(decoded), x, y, z, draw_x_scale, y_scale, z_scale, alignment, red,
                             green, blue, alpha & 0xff);
                }
            }
            followon_line = 0;
            APITEXTSCALEX = saved_x_scale;
            APITEXTSCALEY = saved_y_scale;
        }
    }

    void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines) {
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, max_width, max_lines, NULL,
                    0, 0x80);
    }

    void SmartTextEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                      u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                      u32 alpha) {
        VUFNT *saved_font = SmartTextFont;
        if (QFont2DLower != nullptr)
            SmartTextFont = QFont2DLower;
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, max_width, max_lines,
                    message_box, suppress_draw, alpha);
        SmartTextFont = saved_font;
    }

    void MenuSmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         i32 alpha) {
        if (MenuDrawDropShadows != 0) {
            SmartTextEx2(text, x + x_scale * 0.015f, y - y_scale * 0.015f, z, x_scale, y_scale, z_scale, alignment, 0,
                         0, 0, max_width, max_lines, message_box, suppress_draw, alpha >> 2);
        }
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, max_width, max_lines,
                    message_box, suppress_draw, alpha);
    }

}

void MenuDrawFileCorrupt(MENU_s *) {
    char message[1024];
    sprintf(message, apitxt_NODATAAVAILABLE, apiGameName);
    MenuSmartTextEx(message, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR, MENUNORMALG,
                    MENUNORMALB, 1.3f, 3, NULL, 0, MenuA);
}

void MenuDrawDeleting(MENU_s *) {
    static i32 messageswitched;
    NuStrCpy(MenuHeader, apitxt_DELETEGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;

    if (memcard_deleteneeded != 0 || memcard_deletestarted != 0 || memcard_deletemessage_delay > 0.0f) {
        messageswitched = 0;
        MenuSmartTextEx(apitxt_DELETING, 0.0f, 0.2f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                        MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
        Draw_DONOTREMOVEMEMORYCARD();
        return;
    }

    if (memcard_deletefailed != 0) {
        if (messageswitched == 0) {
            messageswitched = 1;
            MenuAlpha = 0.0f;
            MenuA = 0;
        }
        return;
    }

    if (messageswitched == 0) {
        messageswitched = 1;
        MenuAlpha = 0.0f;
        MenuA = 0;
    }
    MenuSmartTextEx(apitxt_DELETECOMPLETE, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
}

void MenuDrawFormatting(MENU_s *) {
    static i32 messageswitched;
    if (memcard_formatting != 0 || memcard_formatme != 0 || memcard_formatmessage_delay > 0.0f) {
        messageswitched = 0;
        MenuSmartTextEx(apitxt_FORMATTING, 0.0f, 0.2f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                        MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
        Draw_DONOTREMOVEMEMORYCARD();
        return;
    }

    if (memcard_formatfailed != 0) {
        if (messageswitched == 0) {
            messageswitched = 1;
            MenuAlpha = 0.0f;
            MenuA = 0;
        }
        return;
    }

    if (messageswitched == 0) {
        messageswitched = 1;
        MenuAlpha = 0.0f;
        MenuA = 0;
    }
    MenuSmartTextEx(apitxt_FORMATTINGCOMPLETE, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
}

void Draw_AUTOSAVEWARNING() {
    f32 scale = MENUTEXTSCALE;

    if (memcard_drawasiconfn != NULL) {
        memcard_drawasiconfn();
    }
    scale *= 0.8f;

    if (memcard_loadmessage_delay > 0.0f || memcard_loadresult_delay > 0.0f) {
        MenuSmartTextEx(apitxt_LOADING, AUTOSAVEICONX - AUTOSAVEICONSIZE * 1.8f, AUTOSAVEICONY, 1.0f, scale, scale,
                        scale, 8, MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.2f, 1, NULL, 0, MenuA);
    } else {
        MenuSmartTextEx(apitxt_SAVING, AUTOSAVEICONX - AUTOSAVEICONSIZE * 1.8f, AUTOSAVEICONY, 1.0f, scale, scale,
                        scale, 8, MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.2f, 1, NULL, 0, MenuA);
    }
}

void MenuDrawLoading(MENU_s *) {
    Draw_AUTOSAVEWARNING();
}

void MenuDrawSaving(MENU_s *) {
    Draw_AUTOSAVEWARNING();
}

void Draw_LOADCORRUPT() {
    MenuSmartTextEx(apitxt_CORRUPTLOAD, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
}

void Draw_LOADFAILED() {
    MenuSmartTextEx(apitxt_FAILEDTOLOAD, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
}

void Draw_LOADED() {
    MenuSmartTextEx(apitxt_GAMELOADED, 0.0f, -0.4f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
}

void Draw_LOADING() {
    MenuSmartTextEx(apitxt_GAMELOADING, 0.0f, -0.4f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
}

void Draw_NODATAAVAILABLE() {
    char message[1024];
    const f32 scale = MENUTEXTSCALE;
    sprintf(message, apitxt_NODATAAVAILABLE, apiGameName);
    MenuSmartTextEx(message, 0.0f, -0.4f, 1.0f, scale, scale, scale, 0, MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 3,
                    NULL, 0, MenuA);
}

extern "C" {
    void Draw_NOTENOUGHSPACE(void) {
        MenuSmartTextEx(apitxt_NOTENOUGHSPACE, 0.0f, 0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                        MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
    }

}

void MenuDrawNotEnoughSpace(MENU_s *) {
    Draw_NOTENOUGHSPACE();
    Draw_SPACENEEDED();
}

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

extern "C" {
    void DrawMenuEntryEx(MENU *menu, char *text, i32 alpha) {
        i32 draw_alpha = -128;
        f32 scale = menu->item_scale_x;
        if (static_cast<u8>(alpha) <= 128) {
            draw_alpha = alpha;
        }
        if (scale == 0.0f) {
            scale = MENUTEXTSCALE;
        }
        f32 spacing = menu->item_spacing;
        const f32 y_scale = dme_sy;
        scale *= y_scale;
        if (spacing == 0.0f) {
            spacing = MENUDY;
        }
        spacing *= y_scale;
        u8 red;
        u8 green;
        u8 blue;

        if (menu->item_offsets != NULL) {
            Text3DEx2(text, menu->draw_x + menu->item_offsets[0] * menu->item_offset_scale,
                      menu->draw_y + menu->item_offsets[1] * menu->item_offset_scale, menu->draw_z, scale * dme_sx,
                      scale, scale, dme_align, 0, 0, 0, static_cast<u8>(draw_alpha) >> 2);
        }

        if (menu->draw_item >= menu->first_row && menu->draw_item <= menu->last_row) {
            if (menu->draw_item == menu->selected_row && TestForController() != 0) {
                if (TestForController() == 0) {
                    if (menu_pulse > 0.0f) {
                        red = MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                        green = MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                        blue = MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                    } else {
                        red = MENUENTRYR;
                        green = MENUENTRYG;
                        blue = MENUENTRYB;
                    }
                } else if (menu_pulsate > 0.0f) {
                    red = MenuColourLerp(MENUFLASH0R, MENUFLASH1R, menu_pulsate);
                    green = MenuColourLerp(MENUFLASH0G, MENUFLASH1G, menu_pulsate);
                    blue = MenuColourLerp(MENUFLASH0B, MENUFLASH1B, menu_pulsate);
                } else if (menu_flash != 0) {
                    red = MENUFLASH0R;
                    green = MENUFLASH0G;
                    blue = MENUFLASH0B;
                } else {
                    red = MENUFLASH1R;
                    green = MENUFLASH1G;
                    blue = MENUFLASH1B;
                }
            } else if (dme_rgb != 0) {
                red = dme_r;
                green = dme_g;
                blue = dme_b;
            } else if (menu_pulse > 0.0f) {
                red = MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                green = MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                blue = MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
            } else {
                red = MENUENTRYR;
                green = MENUENTRYG;
                blue = MENUENTRYB;
            }

            if (menu->item_offsets != NULL) {
                Text3DEx(text, menu->draw_x, menu->draw_y, menu->draw_z, scale * dme_sx, scale, scale, dme_align, red,
                         green, blue, draw_alpha);
            } else {
                smarttextex_drawmessagebox = 1;
                MenuSmartTextEx(text, menu->draw_x, menu->draw_y, menu->draw_z, scale * dme_sx, scale, scale,
                                static_cast<u32>(dme_align), red, green, blue, MENUENTRYEXWIDTH, 1, NULL, 0,
                                draw_alpha);
            }

            const i32 item = menu->draw_item;
            menu->item_x[item] = menu->draw_x;
            menu->item_y[item] = menu->draw_y;
            menu->item_width[item] = text3d_width;
            menu->item_height[item] = text3d_height;
            menu->item_column[item] = 0;
            menu->item_row[item] = item;
        }
        ++menu->draw_item;
        dme_sx = 1.0f;
        dme_sy = 1.0f;
        dme_rgb = 0;
        dme_align = 0;
        menu->draw_y += spacing;
    }

    void DrawMenuEntry(MENU *menu, char *text) {
        DrawMenuEntryEx(menu, text, static_cast<u8>(MenuA));
    }

}

void MenuDrawLoadConfirm(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_LOADGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    MenuText3DEx(apitxt_CONFIRMLOAD, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                 MENUNORMALG, MENUNORMALB, MenuA);
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

void MenuDrawNoMemoryCard(MENU_s *menu) {
    Draw_NOMEMORYCARD();
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_RETRY);
    DrawMenuEntry(menu, apitxt_CANCEL);
}

extern "C" {
    void Draw_CANCEL(MENU *menu) {
        menu->draw_y = MENUBOTY;
        DrawMenuEntry(menu, apitxt_CANCEL);
    }

}

void MenuDrawLoad(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_LOADGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    if (drawslotsfn != NULL && MenuAlpha > 0.2f && MenuStopDraw == 0) {
        drawslotsfn(menu, 0.0f);
    }
    ++menu->draw_item;
    Draw_CANCEL(menu);
}

void MenuDrawNoData(MENU_s *menu) {
    Draw_NODATAAVAILABLE();
    Draw_CANCEL(menu);
}

void MenuDrawSave(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_SAVEGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    if (drawslotsfn != NULL && MenuAlpha > 0.2f && MenuStopDraw == 0) {
        drawslotsfn(menu, extrasavetext != NULL ? 0.25f : 0.0f);
    }
    ++menu->draw_item;

    if (extrasavetext != NULL) {
        SmartTextEx(extrasavetext, 0.0f, -0.25f, 1.0f, 0.5f, 0.5f, 0.5f, 0, RAP_WARNING_R, RAP_WARNING_G, RAP_WARNING_B,
                    1.9f, 4, NULL, 0, static_cast<i32>(static_cast<f32>(MenuA) * extrasavealpha));
    }
    if (Menu_DisableCancel == 0) {
        Draw_CANCEL(menu);
    }
}

void Draw_OK(MENU_s *menu) {
    menu->draw_y = MENUBOTY;
    DrawMenuEntry(menu, apitxt_OK);
}

void MenuDrawAutoSaveCancel(MENU_s *menu) {
    if (MenuASCancelFinished == 0) {
        Draw_AUTOSAVECANCEL();
        if (memcard_savefailed == 0) {
            Draw_OK(menu);
        }
    }
}

void MenuDrawAutoSaveWarning(MENU_s *menu) {
    Draw_AUTOSAVEWARNING();
    Draw_OK(menu);
}

void MenuDrawFormatCancel(MENU_s *menu) {
    MenuSmartTextEx(apitxt_DOYOUWANTTOABORTFORMAT, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

void MenuDrawLoadCancel(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_LOADGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    MenuSmartTextEx(apitxt_DOYOUWANTTOABORTLOAD, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

void MenuDrawSaveCancel(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_SAVEGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    MenuSmartTextEx(apitxt_DOYOUWANTTOABORT, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                    MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.5f, 2, NULL, 0, MenuA);
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

void MenuDrawSaveConfirm(MENU_s *menu) {
    NuStrCpy(MenuHeader, apitxt_SAVEGAME);
    header_r = MENUHEADERR;
    header_g = MENUHEADERG;
    header_b = MENUHEADERB;
    if (Menu_Overwriteflag != 0) {
        MenuSmartTextEx(apitxt_CONFIRMOVERWRITE, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                        MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.6f, 3, NULL, 0, MenuA);
    } else {
        MenuSmartTextEx(apitxt_CONFIRMSAVE, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                        MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.6f, 3, NULL, 0, MenuA);
    }

    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

void MenuDrawDeleteConfirm(MENU_s *menu) {
    MenuSmartTextEx(apitxt_CONFIRMDELETE, 0.0f, 0.0f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.2f, 2, NULL, 0, MenuA);
    menu->draw_y = MENUBOTY - MENUDY;
    DrawMenuEntry(menu, apitxt_YES);
    DrawMenuEntry(menu, apitxt_NO);
}

extern "C" {
    void DrawMenuBottomMessage(char *text, u8 red, u8 green, u8 blue) {
        MenuSmartTextEx(text, 0.0f, -0.3f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, red, green, blue, 1.5f,
                        4, NULL, 0, MenuA);
    }

    void DrawMenuTopMessage(char *text, u8 red, u8 green, u8 blue) {
        MenuSmartTextEx(text, 0.0f, 0.15f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, red, green, blue, 1.5f,
                        4, NULL, 0, MenuA);
    }

    void DrawMenuHeaderMessage(char *text, u8 red, u8 green, u8 blue) {
        MenuSmartTextEx(text, 0.0f, MENUTOPY, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 1, red, green, blue,
                        1.5f, 3, NULL, 0, MenuA);
    }

    void DrawMenuHeader(void) {
        if (MenuDisableHeaders == 0) {
            MenuSmartTextEx(MenuHeader, 0.0f, MENUTOPY, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0,
                            static_cast<u8>(header_r), static_cast<u8>(header_g), static_cast<u8>(header_b), 1.2f, 1,
                            NULL, 0, MenuA);
        }
    }

    void SmartTextExDrop(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u32 red,
                         u32 green, u32 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         u32 alpha) {
        SmartTextEx2(text, x - x_scale * 0.007f, y - y_scale * 0.01f, z, x_scale, y_scale, z_scale, alignment, 0, 0, 0,
                     max_width, max_lines, message_box, suppress_draw, alpha);
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red & 0xff, green & 0xff, blue & 0xff,
                    max_width, max_lines, message_box, suppress_draw, alpha);
    }

    void SmartTextSetFWNMode(i32 mode) {
        smarttext_fwn = mode;
    }

    void SmartTextSetFont(VUFNT *font) {
        SmartTextFont = font;
    }

    void SetGameFont(VUFNT *font) {
        QFont2D = font;
    }

    void Set3DGameFont(VUFNT *font) {
        QFont3DZ = font;
    }

    VUFNT *LoadGameFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane) {
        QFont2D = static_cast<VUFNT *>(NuQFntLoadPtr(path, name, 4, render_plane, buf, buf_end));
        if (QFont2D != nullptr) {
            if (create_qfont2dlower != 0)
                QFont2DLower = NuQFntDuplicate(QFont2D, 4, render_plane - 1, buf, buf_end);
            if (create_qfont2dz != 0)
                QFont2DZ = NuQFntDuplicate(QFont2D, 4, 0x40000, buf, buf_end);
            if (create_qfont3d != 0)
                QFont3D = NuQFntDuplicate(QFont2D, 0x18, render_plane, buf, buf_end);
            if (create_qfont3dz != 0)
                QFont3DZ = NuQFntDuplicate(QFont2D, 0x4c, render_plane, buf, buf_end);
        }
        return QFont2D;
    }

    void UnloadGameFont(void) {
        if (QFont2DLower != NULL) {
            NuQFntDestroy(QFont2DLower);
            QFont2DLower = NULL;
        }
        if (QFont2DZ != NULL) {
            NuQFntDestroy(QFont2DZ);
            QFont2DZ = NULL;
        }
        if (QFont3D != NULL) {
            NuQFntDestroy(QFont3D);
            QFont3D = NULL;
        }
        if (QFont3DZ != NULL) {
            NuQFntDestroy(QFont3DZ);
            QFont3DZ = NULL;
        }
        NuQFntDestroy(QFont2D);
    }

extern "C" void SetQFont2D(void) {
    NuQFntSetCoordinateSystem(static_cast<NUQFNT_CSMODE>(3));
    NuQFntSet(QFont2D);
    NuQFntSetSpaceWidth(QFont2D, 12.0f);
}

    void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale) {
        QFONTSCALEX = 1.0f - (1.0f - font_scale_x) * 0.5f;
        QFONTSCALEY = 1.0f / QFONTSCALEX;
        STCOORDSCALE = coordinate_scale;
    }

    void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale) {
        if (font_scale_x != nullptr)
            *font_scale_x = QFONTSCALEX;
        if (coordinate_scale != nullptr)
            *coordinate_scale = STCOORDSCALE;
    }

    void Debug_Print(void) {
    }

}

i32 SplitTextFindNextWS(unsigned char *text, i32 position) {
    unsigned char character = text[position];
check_character:
    if (character == ' ') {
        const unsigned char next = text[position + 1];
        if (next == '?')
            goto next_character;
        if (next == '!')
            goto next_character;
        if (next == ';')
            goto next_character;
        if (next == ':')
            goto next_character;
        return position;
    }
    if (character == '\0') {
        return position;
    }
    if (character == '\\' && text[position + 1] == 'n') {
        return position;
    }

next_character:
    do {
        ++position;
        character = text[position];
    } while (static_cast<u8>(character - 0x80) <= 0x3f);
    goto check_character;
}

extern "C" {
    i32 SplitText(unsigned char *text, f32 max_width) {
        if (max_width <= 0.0f)
            return -1;

        i32 line_count = 1;
        i32 line_start = 0;
        i32 position = 0;
        unsigned char substring[512];
        unsigned char decoded[512];
        u16 encoded[512];

        while (max_width > 0.0f) {
            i32 previous_break = position;
            unsigned char *line = text + line_start;

            for (;;) {
                const i32 next_break = SplitTextFindNextWS(text, position);
                i32 length = next_break - line_start;
                if (length > 511)
                    length = 511;
                memcpy(substring, line, length);
                substring[length] = '\0';
                TextDecode(reinterpret_cast<char *>(substring), decoded);
                Text3DStringEncode(reinterpret_cast<char *>(decoded), encoded);
                const f32 width = NuQFntPrintLenW(QFont2D, encoded);

                position = next_break;
                while (text[position] == ' ')
                    ++position;

                if (text[position] == '\0')
                    return line_count;

                if (text[position] == '\\' && text[position + 1] == 'n') {
                    previous_break = position + 1;
                    text[previous_break] = ' ';
                } else if (max_width > width) {
                    previous_break = position;
                    continue;
                } else if (previous_break == line_start) {
                    previous_break = position;
                }

                text[previous_break - 1] = '\n';
                ++line_count;
                line_start = previous_break;
                break;
            }
        }

        text[line_start - 1] = '\n';
        return line_count + 1;
    }

}

void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination) {
    u16 character;
    while (*source != '\0') {
        source = NuUnicodeCharFromUTF8(&character, source);
        if (character == '~') {
            if (*source == '\0') {
                break;
            }
            source = NuUnicodeCharFromUTF8(&character, source);
            continue;
        }

        *destination = NuQFntEncodeUnicodeChar(font, character);
        if (*destination == 0xffff) {
            NuUnicodeCharFromUTF8(&character, reinterpret_cast<unsigned char *>(const_cast<char *>("\xe2\x96\xa1")));
            *destination = NuQFntEncodeUnicodeChar(font, character);
            if (*destination == 0xffff) {
                *destination = NuQFntEncodeUnicodeChar(font, '?');
            }
        }
        ++destination;
    }
    *destination = 0;
}

extern "C" {
    void MatrixText(VUFNT *font, unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, u32 alignment,
                    u32 colour, NUMTX *matrix, i32 camera_relative) {
        if (text == nullptr || text[0] == '\0' || MenuStopDraw != 0)
            return;
        if (font == nullptr)
            font = QFont3DZ;
        if (font == nullptr)
            return;

        NuQFntPushPrintMode(4);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
        NuQFntSet(font);

        u16 encoded[512];
        MatrixTextStringEncode(font, text, encoded);
        NuQFntSetScale(font, x_scale * 0.004f, y_scale * 0.004f);
        const f32 width = NuQFntPrintLenW(font, encoded);
        const f32 height = NuQFntHeight(font);

        y -= height * 0.5f;
        if ((alignment & 4) != 0) {
            y -= height * 0.5f;
        } else if ((alignment & 1) != 0) {
            y += height * 0.5f;
        }

        if ((alignment & 2) == 0) {
            if ((alignment & 8) != 0) {
                x -= width;
            } else {
                x -= width * 0.5f;
            }
        }

        NUMTX draw_matrix;
        if (camera_relative == 0) {
            draw_matrix = *matrix;
        } else {
            NuMtxMul(&draw_matrix, matrix, NuCameraGetMtx());
        }
        NuQFntSetMtx(font, &draw_matrix);
        NuQFntSetColour(font, colour);
        NuQFntSetICGap(font, 2.0f);
        NuQFntMove(font, x, y, z);
        NuQFntPrintW(font, encoded);
        NuQFntPopCoordinateSystem();
        NuQFntPopPrintMode();
    }

    void FixUpButtonsInFont(VUFNT *game_font, VUFNT *button_font) {
        if (game_font == nullptr)
            game_font = QFont2D;
        if (button_font == nullptr)
            button_font = QFont2DButtons;
        if (game_font != nullptr && button_font != nullptr) {
            f32 scale = game_font->height / button_font->height;
            for (i32 i = 0; i < button_font->glyph_count; i++) {
                for (i32 j = 0; j < game_font->glyph_count; j++) {
                    if (button_font->unicode_map[i].unicode != game_font->unicode_map[j].unicode) {
                        continue;
                    }
                    if (button_font->unicode_map[i].unicode >= 0x531 && button_font->unicode_map[i].unicode <= 0x53f) {
                        game_font->glyphs[game_font->unicode_map[j].index].width =
                            button_font->glyphs[button_font->unicode_map[i].index].width * scale;
                    }
                    break;
                }
            }
        }
    }

    VUFNT *LoadButtonFont(char *path, char *name, variptr_u *buf, variptr_u *buf_end, i32 render_plane) {
        if (QFont2DButtons == nullptr) {
            QFont2DButtons = static_cast<VUFNT *>(NuQFntLoadPtr(path, name, 4, render_plane, buf, buf_end));
            if (QFont2DButtons != nullptr) {
                FixUpButtonsInFont(QFont2D, QFont2DButtons);
                QFont2DButtons->baseline = QFont2DButtons->height / QFont2D->height * QFont2D->baseline;
            }
        }
        return QFont2DButtons;
    }

}
