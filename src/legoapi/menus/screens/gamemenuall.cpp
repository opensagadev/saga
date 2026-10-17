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
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "batman.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/menus/core/gamehint.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nupostparams.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/menus/screens/movies.h"
#include "legoapi/menus/screens/store.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuvideo.h"
#include "legoapi/misc/androidbatman.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/numemory.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/render/fx/game_deb.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include <stddef.h>
#include "legoapi/items/base/apiobject.h"
#include <float.h>
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include <stdlib.h>
#include <cstdio>
#include "nu2api/nu3d/nuspline.h"
#include <new>
#include "nu2api/numath/nuvec4.h"
#include <stdarg.h>
#include <GLES2/gl2.h>
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nu3d/nurendercontext.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/lib/text.h"
#include "nu2api/nu3d/nu3d_includes.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

MENUPACKET_s MenuPacket = {};
extern f32 HUB_EPISODETITLEY;
extern i16 tSELECT, tSELECTED, tSELECTING;
extern "C" f32 MENUBOTY;

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

static inline i32 MenuSlotColourLerp(u32 first, u32 second, f32 amount) {
    return static_cast<i32>(first * amount + second * (1.0f - amount));
}

void MenuInitialiseEx(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                      void (*draw_save_slots_info_fn)(f32, f32, i32, i32), i32 is_fade_enabled, i32 is_shadow_enabled);

void MenuInitialise(MENUFNINFO *menu_info, i32 menu_id_count, i32 language_count,
                    void (*draw_save_slots_fn)(MENU *, f32), i32 is_fade_enabled, i32 is_shadow_enabled);

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
} // extern "C"

struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

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

extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern f32 MENUBOTY;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct CUTSCENEPLAYERCLIP;

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

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

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

extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern void (*FS_Callback)(char *, char *);

f32 memcard_autosavecanceldelay = 1.5f;
f32 memcard_createmessage_delay;
f32 memcard_createresult_delay;
f32 memcard_message_delay;
f32 memcard_result_delay;
i32 memcard_justformatted;
i32 saveload_autosavedisabled;

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" {
    void (*savesuccessfn)(void);
    void *memcard_headerdata;
    i32 memcard_headerdatasize;
    void *memcard_headerdatabuffer;
} // extern "C"

extern char *apitxt_AUTOSAVE_WARNING, *apitxt_LOADING, *apitxt_SAVING;

f32 text3d_height;
f32 text3d_width;
void (*buttonmapfn)(char *, char *);
f32 (*pulsetimerfn)(f32);
extern "C" {
    i32 smarttextex_drawmessagebox = 0;
}

static f32 QFONTSCALEX = 1.0f;
static f32 QFONTSCALEY = 1.0f;
static f32 STCOORDSCALE = 1.0f;
f32 g_buttonFontScalePulse = 1.0f;
f32 smarttextex_longestwidth;
i32 smarttext_fwn;
static i32 followon_line;
static u32 current_rgba = 0x80808080;
i32 apitext_half_rgb;
u8 text_preset_b[10] = {128, 0, 0, 128, 128, 128, 0, 0, 0, 0};
u8 text_preset_g[10] = {128, 0, 128, 0, 128, 0, 128, 96, 0, 0};
u8 text_preset_r[10] = {128, 128, 0, 0, 0, 128, 128, 128, 0, 0};

NUMTL *MessageMtl;
i32 MessageMtlInit;

extern "C" {
void TextRegisterButtonMapFn(void (*fn)(char *, char *));
void TextRegisterPulseTimerFn(f32 (*fn)(f32));
}

// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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

extern "C" {
    extern "C++" void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
}

extern "C" {
extern "C++" TERRAIN_TRACK_SLOT *AllocTerrId();
} // extern "C"

struct rtlset;

struct spacelevel_s;
struct quickboltinfo;

static __attribute__((used)) void edanimcbCubeMap(eduimenu_s *, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileLoad(eduimenu_s *menu, eduiitem_s *, u32);
static __attribute__((used)) void edanimcbFileSave(eduimenu_s *menu, eduiitem_s *, u32);
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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
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

extern "C" {
static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);
static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);
static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);
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
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern EdInputContext * Input;
extern f32 Scale;
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

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);
static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);
static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);
static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);
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
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
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

extern i32 memcard_deleteneeded;
extern i32 memcard_deletestarted;
extern i32 memcard_deletefailed;
extern i32 memcard_formatting;
extern i32 memcard_formatme;
extern i32 memcard_formatfailed;
extern void (*drawslotinfofn)(f32, f32, i32, i32);
extern void (*drawslotsfn)(MENU *, f32);
extern u8 RAP_WARNING_B;
extern u8 RAP_WARNING_G;
extern u8 RAP_WARNING_R;

extern "C" {
extern f32 MENUBOTY;
extern f32 MENUENTRYEXWIDTH;
} // extern "C"

struct ani3_animheader_s;

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
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
} // extern "C"

struct nupad_s;
struct nuframebuffer_s;
struct nushaderobject_s;
union variptr_u;

// This alignment affects PodRaceAUpdate codegen even though the linked address is 32-byte aligned.

DECOMP_ASSERT(sizeof(speedervals) == 0x70, "Speeder chase tuning table size");

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

extern "C" {
extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
    }

#include "nu2api/nucore/nucore_plain_types.h"
} // extern "C"

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
    __attribute__((optimize("O2", "tree-vectorize"))) void UpdateSaveSlots(void) {
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
