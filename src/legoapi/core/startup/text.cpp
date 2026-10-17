// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "batman.h"
#include "nu2api/nucore/nuvuvec.hpp"

extern "C" {
    extern char *apitxt_ENGLISH, *apitxt_FRENCH, *apitxt_GERMAN, *apitxt_ITALIAN, *apitxt_SPANISH, *apitxt_DANISH;
}

// Retail keeps the game's language list in its own text.cpp; the names are filled in by its static initializer.
LANGUAGEDATA Game_LanguageList[7] = {{1, apitxt_ENGLISH}, {2, apitxt_FRENCH}, {4, apitxt_GERMAN}, {5, apitxt_ITALIAN},
                                     {3, apitxt_SPANISH}, {8, apitxt_DANISH}, {-1, NULL}};

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
char *txt_NULL = const_cast<char *>("?");
char *apitxt_ENGLISH = const_cast<char *>("English");
char *apitxt_FRENCH = const_cast<char *>("Fran\xc3\xa7"
                                         "ais");
char *apitxt_GERMAN = const_cast<char *>("Deutsch");
char *apitxt_ITALIAN = const_cast<char *>("Italiano");
char *apitxt_SPANISH = const_cast<char *>("Espa\xc3\xb1"
                                          "ol");
char *apitxt_DANISH = const_cast<char *>("Dansk");

// Original .data @0x667ce0; the names are filled in by the static initializer.
extern LANGUAGEDATA * Text_LanguageList;
extern f32 text3d_height;
extern f32 text3d_width;
extern void (*buttonmapfn)(char *, char *);
extern f32 (*pulsetimerfn)(f32);
extern "C" {
extern i32 smarttextex_drawmessagebox;
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
void Text_DecodeButtons(char *source, char *destination);
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
#include "nu2api/numath/numtx.h"
extern "C" {
    void NuMtxSetRotationX(NUMTX *mtx, i32 angle);
    void NuMtxRotateY(NUMTX *mtx, i32 angle);
    void NuMtxRotateZ(NUMTX *mtx, i32 angle);
    void NuMtxTranslate(NUMTX *mtx, NUVEC *vec);
}
extern "C" void TextDecode(char *source, unsigned char *dest);
extern "C" void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                         u8 green, u8 blue, i32 alpha);
extern "C" void Text3DEx2(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                          u8 green, u8 blue, i32 alpha);
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text);


void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk);

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text);
f32 TextPulseTimer(f32 delay);

void Text_MakeScore(u32 score, char *text);
extern i16 tALONGTIMEAGO;
void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end);
void Text_SetLanguage(i32 language);
extern f32 g_buttonFontScalePulse;
extern f32 smarttextex_longestwidth;
extern i32 MenuStopDraw;

extern char * txtpath_JAPANESE;
extern char * txtpath_DANISH;
extern char * txtpath_SPANISH;
extern char * txtpath_ITALIAN;
extern char * txtpath_GERMAN;
extern char * txtpath_FRENCH;
extern char * txtpath_ENGLISH;

void Text_InitLanguageList(LANGUAGEDATA *language_list);
void Text_SetLanguage_Game(i32 language);
void Text_InitDefaultStrings();
i32 Text_GetMaxOverallStrings();
void Text_LocaliseDecimalPoint(char *text);
void Text_ExpandAllButtonStrings(char *input, char *output);
void Text_FillInExtendedSaveInfo();

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

extern "C" void Text3DStringEncode(char *src, u16 *dst);
extern "C" void SetQFont2D(void);

i32 GetMenuID(void);


// ==== functions in original address order ====

void Text_DecodeButtons(char *source, char *destination) {
    if (NuStrICmp(source, "wibble") == 0) {
        NuStrCpy(destination, "cross");
    }
}

f32 TextPulseTimer(f32 delay) {
    if (TestForController()) {
        return 1.0f;
    }

    f32 elapsed = GlobalTimer.time_elapsed - (LastTouchTime + delay * 4.0f);
    if (elapsed > 4.0f) {
        elapsed = NuFmod(elapsed, 4.0f);
        const i32 angle = static_cast<i32>(elapsed * 0.25f * 65536.0f);
        f32 pulse = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
        if (0.0f > pulse) {
            return 1.0f;
        }
        return pulse + 1.0f;
    }
    return 1.0f;
}

void Text_SetLanguage_Game(i32 language) {
    switch (language) {
        case 0:
            INTROTEXT_Y = 0.0f;
            INTROTEXT_SCALE = 1.8f;
            break;
        case 2:
            INTROTEXT_Y = 0.115f;
            INTROTEXT_SCALE = 0.5f;
            break;
        case 3:
            INTROTEXT_Y = 0.14f;
            INTROTEXT_SCALE = 0.61f;
            break;
        case 4:
            INTROTEXT_Y = 0.13f;
            INTROTEXT_SCALE = 0.575f;
            break;
        case 5:
            INTROTEXT_Y = 0.17f;
            INTROTEXT_SCALE = 0.76f;
            break;
        case 8:
            INTROTEXT_Y = 0.15f;
            INTROTEXT_SCALE = 0.67f;
            break;
        default:
            INTROTEXT_Y = 0.175f;
            INTROTEXT_SCALE = 0.79f;
            break;
    }
}

void Text_InitDefaultStrings() {
    TTab[tNULL] = txt_NULL;
    TTab[tUNKNOWN] = txt_UNKNOWN;
    TTab[tENGLISH] = apitxt_ENGLISH;
    TTab[tFRENCH] = apitxt_FRENCH;
    TTab[tGERMAN] = apitxt_GERMAN;
    TTab[tITALIAN] = apitxt_ITALIAN;
    TTab[tSPANISH] = apitxt_SPANISH;
    TTab[tDANISH] = apitxt_DANISH;
}

void Text_FillInExtendedSaveInfo() {
}

void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end) {
    unsigned char *string_buffer = buf->u8_ptr;
    char language[32];
    char path[256];

    TextRegisterButtonMapFn(Text_DecodeButtons);
    TextRegisterPulseTimerFn(TextPulseTimer);
    NuStrCpy(language, Text_GetLanguagePath(Text_Language));
    NuStrCpy(path, "stuff\\text\\");
    NuStrCat(path, language);
    NuStrCat(path, ".txt");
    Text_LoadAndFixUpStrings(reinterpret_cast<unsigned char *>(path), &string_buffer, TTab, 0x70d);
    IntroText_SetTextID(tALONGTIMEAGO);
    buf->addr = ALIGN(reinterpret_cast<usize>(string_buffer), 4);
    if (TTab[tPLAYER1] != NULL)
        NuStrCpy(Game.customizer[0].name, TTab[tPLAYER1]);
    if (TTab[tPLAYER2] != NULL)
        NuStrCpy(Game.customizer[1].name, TTab[tPLAYER2]);
    FinishWeirdoNames(-1);
    Customiser_InitNames(CharacterCustomiser);
    MenuLoadTechnicalStrings(const_cast<char *>("stuff\\text\\trc.csv"), language, buf, *buf_end);
    apitxt_AUTOSAVE_WARNING = TTab[tIOSAUTOSAVEWARNING];
    apitxt_LOADING = TTab[tLOADING];
    apitxt_SAVING = TTab[tSAVING];
}

