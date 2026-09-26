#include "decomp.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/text.h"
char *ASCII_UP = "\xc2\xac";
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
char *txt_NULL = const_cast<char *>("?");
char *apitxt_ENGLISH = const_cast<char *>("English");
char *apitxt_FRENCH = const_cast<char *>("Fran\xc3\xa7"
                                         "ais");
char *apitxt_GERMAN = const_cast<char *>("Deutsch");
char *apitxt_ITALIAN = const_cast<char *>("Italiano");
char *apitxt_SPANISH = const_cast<char *>("Espa\xc3\xb1"
                                          "ol");
char *apitxt_DANISH = const_cast<char *>("Dansk");
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
void (*Text_GameSetLanguageFn)(i32);
char *Text_GetLanguagePath(i32 language);
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
void IntroText_SetTextID(i32 id);
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
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
static VUFNT *app_fnt;
void Text_LoadFont(char *path, variptr_u *buf, variptr_u *buf_end) {
    create_qfont3dz = 1;
    app_fnt = LoadGameFont(path, path, buf, buf_end, 1);
    LoadButtonFont("stuff\\text\\Buttons", 0, buf, buf_end, 0);
}
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
void Text_MakeTime(float time, i32 show_hours, i32 show_minutes, i32 show_centiseconds, char *text) {
    time = MAX(0.0f, time);

    i32 hours = 0;
    i32 minutes;
    if (show_hours != 0) {
        hours = static_cast<i32>(time / 3600.0f);
        minutes = static_cast<i32>(NuFmod(time / 60.0f, 60.0f));
    } else {
        minutes = static_cast<i32>(time / 60.0f);
    }

    i32 seconds;
    if (show_minutes != 0 || show_centiseconds != 0) {
        seconds = static_cast<i32>(NuFmod(time, 60.0f));
    } else {
        seconds = static_cast<i32>(time);
    }
    const i32 centiseconds = static_cast<i32>(NuFmod(time, 1.0f) * 100.0f);

    if (show_hours != 0) {
        if (show_centiseconds != 0) {
            sprintf(text, "%i:%.2i:%.2i.%.2i", hours, minutes, seconds, centiseconds);
        } else {
            sprintf(text, "%i:%.2i:%.2i", hours, minutes, seconds);
        }
    } else if (show_minutes != 0) {
        if (show_centiseconds != 0) {
            sprintf(text, "%i:%.2i.%.2i", minutes, seconds, centiseconds);
        } else {
            sprintf(text, "%i:%.2i", minutes, seconds);
        }
    } else if (show_centiseconds != 0) {
        sprintf(text, "%i.%.2i", seconds, centiseconds);
    } else {
        sprintf(text, "%i", seconds);
    }
}

static TEXTCRAWL_s *s_crawlPtr = nullptr;
static NUMTX s_textcrawl_mtx;
static NUVEC s_textcrawl_offset = {0.0f, -1.0f, 10.0f};
f32 APITEXTSCALEX = 1.0f;
f32 APITEXTSCALEY = 1.0f;
i32 textcrawlactive = 0;
i32 Arcade_TextCrawlID = 0x1f1;
i32 Arcade_TextCrawlParagraphs = 2;
extern i32 g_bgloadAsFastAsPossibleHack;

void TextCrawl_Init(TEXTCRAWL_s *crawl, i32 id, i32 unk) {
    if (crawl == nullptr || QFont3DZ == nullptr) {
        return;
    }
    s_crawlPtr = crawl;
    reinterpret_cast<i8 *>(crawl)[0x0d] = id >= 0 && id < AREACOUNT ? static_cast<i8>(id) : -1;
    NuMtxSetRotationX(&s_textcrawl_mtx, 0x3333);
    NuMtxRotateY(&s_textcrawl_mtx, 0);
    NuMtxRotateZ(&s_textcrawl_mtx, 0);
    NuMtxTranslate(&s_textcrawl_mtx, &s_textcrawl_offset);
    if (unk != 0) {
        QFont3DTime = 0.0f;
    }
}

void TextCrawl_Draw(float dt, i32 paragraphs, float alpha, char *text) {
    if (s_crawlPtr == nullptr || QFont3DZ == nullptr) {
        return;
    }
    NuQFntSetJustifiedTolerances(1.2f, 1.2f);
    if (paragraphs == 0 && QFont3DTime >= 60.0f) {
        return;
    }

    AREADATA *area = NULL;
    i32 body_text_id = -1;
    i32 episode_index = -1;
    i32 area_index = -1;
    const i8 crawl_area = reinterpret_cast<i8 *>(s_crawlPtr)[0x0d];
    if (crawl_area != -1) {
        area = &ADataList[crawl_area];
        body_text_id = area->text_id;
        episode_index = static_cast<i8>(area->episode_index);
        area_index = static_cast<i8>(area->area_index);
    }

    char *episode_name = NULL;
    char *episode_text = NULL;
    char *chapter_text = NULL;
    char *area_name = NULL;
    i32 paragraph_count = 0;
    char chapter_buffer[0x40];

    if (text != NULL) {
        episode_name = text;
        body_text_id = -1;
    } else {
        MISSIONDATA *mission = Mission_Active(NULL);
        if (mission != NULL) {
            if (mission->name_id != -1) {
                area_name = TTab[mission->name_id];
            }
            body_text_id = mission->text_id;
            paragraph_count = 3;
        } else if (Arcade != 0) {
            if (area != NULL && area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
            body_text_id = Arcade_TextCrawlID;
            paragraph_count = Arcade_TextCrawlParagraphs;
        } else if (episode_index == -1) {
            if (area != NULL && area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
        } else {
            if (EDataList[episode_index].name_id != -1) {
                episode_name = TTab[EDataList[episode_index].name_id];
            }
            if (EDataList[episode_index].text_id != -1) {
                episode_text = TTab[EDataList[episode_index].text_id];
            }
            if (area_index != -1) {
                if ((area->flags & AREAFLAG_BONUS_AREA) == 0) {
                    if (s_crawlPtr->heading_text != NULL) {
                        sprintf(chapter_buffer, "%s %i", TTab[*s_crawlPtr->heading_text], area_index + 1);
                        chapter_text = chapter_buffer;
                    }
                } else {
                    i16 *bonus_text = (area->flags & AREAFLAG_VEHICLE_AREA) != 0 ? s_crawlPtr->vehicle_bonus_text
                                                                                 : s_crawlPtr->character_bonus_text;
                    if (bonus_text != NULL) {
                        chapter_text = TTab[*bonus_text];
                    }
                }
            }
            if (area->name_id != -1) {
                area_name = TTab[area->name_id];
            }
            paragraph_count = 0;
        }
    }

    g_bgloadAsFastAsPossibleHack = alpha > 0.75f;
    QFont3DTime += dt;
    if (QFont3DTime >= 60.0f) {
        if (paragraphs == 0) {
            return;
        }
        QFont3DTime = 0.0f;
    }

    NUMTX matrix;
    NuMtxMul(&matrix, &s_textcrawl_mtx, NuCameraGetMtx());
    NuQFntSet(QFont3DZ);
    NuQFntSetMtx(QFont3DZ, &matrix);
    NuQFntPushPrintMode(4);
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);

    f32 colour_scale = 1.0f;
    if (paragraphs == 0 && QFont3DTime >= 55.0f) {
        colour_scale = 1.0f - (QFont3DTime - 55.0f) / 5.0f;
    }
    const u32 colour =
        (static_cast<u32>(128.0f * colour_scale * alpha) << 24) | (static_cast<u32>(111.0f * colour_scale) << 8) | 0xff;
    NuQFntSetColour(QFont3DZ, colour);

    f32 y = QFont3DTime * 0.4f - 4.5f;
    const f32 x_scale = APITEXTSCALEX * 0.01f;
    const f32 y_scale = APITEXTSCALEY * 0.01f;
    u16 encoded[1024];

    if (episode_name != NULL) {
        if (text != NULL && NuStrCmp(text, "?") == 0) {
            NuQFntSetScale(QFont3DZ, x_scale * 2.0f, y_scale * 3.0f);
        } else {
            NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        }
        Text3DStringEncode(episode_name, encoded);
        NuQFntMove(QFont3DZ, NuQFntPrintLenW(QFont3DZ, encoded) * -0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (episode_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(episode_text, encoded);
        NuQFntMove(QFont3DZ, NuQFntPrintLenW(QFont3DZ, encoded) * -0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (chapter_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        Text3DStringEncode(chapter_text, encoded);
        NuQFntMove(QFont3DZ, NuQFntPrintLenW(QFont3DZ, encoded) * -0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (area_name != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(area_name, encoded);
        NuQFntMove(QFont3DZ, NuQFntPrintLenW(QFont3DZ, encoded) * -0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (body_text_id != -1) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        if (paragraph_count == 0) {
            paragraph_count = area == NULL ? s_crawlPtr->paragraph_count : area->text_id_value;
        }
        for (i32 i = 0; i < paragraph_count; ++i) {
            char *paragraph = TTab[body_text_id + i];
            if (paragraph == NULL || *paragraph == '\0') {
                break;
            }
            Text3DStringEncode(paragraph, encoded);
            y += NuQFntPrintJustifiedW(QFont3DZ, encoded, -3.5f, y, 0.0f, x_scale, y_scale, 7.0f, 1.3f, colour,
                                       &matrix) +
                 NuQFntHeight(QFont3DZ);
        }
    }
    NuQFntPopPrintMode();
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
static char **TTab_Original;
static i32 Text_MaxStrings_Overall;
static u32 *Text_StringBits;

void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last) {
    if (entry == nullptr)
        return;

    i32 index = 0;
    while (entry->text_id != nullptr) {
        if (entry->value != -1) {
            index = entry->value;
            if (index >= first && index <= last)
                *entry->text_id = entry->value;
        } else {
            if (index >= first && index <= last) {
                entry->value = index;
                *entry->text_id = index;
                Text_StringBits[index >> 5] |= 1U << (index & 0x1f);
            } else {
                entry->value = 0;
                *entry->text_id = 0;
            }
        }
        index++;
        entry++;
    }
}
void Text_MakeScore(u32 score, char *text) {
    char digits[80];
    char *end = &digits[79];
    *end = '\0';

    char *first = end;
    do {
        *--first = static_cast<char>('0' + score % 10);
        score /= 10;
    } while (score != 0);

    Text_InsertCommasIntoNumber(first, text, static_cast<i32>(end - first));
}
extern i16 tALONGTIMEAGO;
void Text_LoadStrings(variptr_u *buf, variptr_u *) {
    unsigned char *string_buffer = buf->u8_ptr;
    char language[32];
    char path[256];

    NuStrCpy(language, Text_GetLanguagePath(Text_Language));
    NuStrCpy(path, "stuff\\text\\");
    NuStrCat(path, language);
    NuStrCat(path, ".txt");
    Text_LoadAndFixUpStrings(reinterpret_cast<unsigned char *>(path), &string_buffer, TTab, 0x70d);
    IntroText_SetTextID(tALONGTIMEAGO);
    buf->addr = ALIGN(reinterpret_cast<usize>(string_buffer), 4);
}
void Text_SetLanguage(i32 language) {
    if (language == -1) {
        language = Text_Language;
    } else {
        if (language == 0) {
            language = 1;
        }
        Text_Language = language;
    }

    NuLanguageSet(language);
    if (Text_GameSetLanguageFn != NULL) {
        Text_GameSetLanguageFn(Text_Language);
    }
}
void *Text_IsFontLoaded() {
    return app_fnt;
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
static f32 QFONTSCALEX = 1.0f;
static f32 QFONTSCALEY = 1.0f;
static f32 STCOORDSCALE = 1.0f;
static f32 g_buttonFontScalePulse = 1.0f;
i32 MenuStopDraw;
i32 smarttext_fwn;
static i32 followon_line;

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font) {
    VUFNT *base_font = SmartTextFont != nullptr ? SmartTextFont : QFont2D;
    VUFNT *font;
    f32 pulse;
    f32 relative_scale;
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
void Text_DecodeButtons(char *source, char *destination) {
    if (NuStrICmp(source, "wibble") == 0) {
        NuStrCpy(destination, "cross");
    }
}
char *Text_GetLanguagePath(i32 language) {
    static char japanese[] = "japanese";
    static char danish[] = "danish";
    static char spanish[] = "spanish";
    static char italian[] = "italian";
    static char german[] = "german";
    static char french[] = "french";
    static char english[] = "english";

    switch (language) {
        case 0:
            return japanese;
        case 2:
            return spanish;
        case 3:
            return italian;
        case 4:
            return german;
        case 5:
            return french;
        case 8:
            return danish;
        default:
            return english;
    }
}
void Text_InitStringTable(i32 count, variptr_u *buf, variptr_u *) {
    TTab_Original = reinterpret_cast<char **>(buf->void_ptr);
    usize table_size = (count + 1) * sizeof(char *);
    buf->addr = ALIGN(buf->addr + table_size, 4);
    memset(TTab_Original, 0, table_size);
    Text_MaxStrings_Overall = count;
    TTab_Original[0] = "Err...";
    TTab = TTab_Original + 1;

    i32 flags_size = ((count + 31) >> 5) * sizeof(u32);
    Text_StringBits = buf->u32_ptr;
    memset(Text_StringBits, 0, flags_size);
    buf->addr += flags_size;
}
void Text_InitLanguageList(LANGUAGEDATA *language_list) {
    if (language_list != NULL) {
        Text_LanguageList = language_list;
    }

    LANGUAGECOUNT = 0;
    while (Text_LanguageList[LANGUAGECOUNT].language != -1) {
        ++LANGUAGECOUNT;
    }
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
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count) {
    unsigned char *out = *buffer;
    NUFPAR *parser = NuFParCreate(reinterpret_cast<char *>(filename));
    if (parser != nullptr) {
        while (NuFParGetLine(parser) != 0) {
            i32 index = NuFParGetInt(parser);
            if (index <= 0 || index >= count)
                continue;
            if (NuFParGetWord(parser) <= 0)
                continue;

            char *word = parser->word_buf;
            if (NuStrICmp(word, "360") == 0 || NuStrICmp(word, "gc") == 0 || NuStrICmp(word, "ps2") == 0 ||
                NuStrICmp(word, "ps3") == 0 || NuStrICmp(word, "psp") == 0 || NuStrICmp(word, "pc") == 0 ||
                NuStrICmp(word, "wii") == 0 || NuStrICmp(word, "playstation") == 0)
                continue;

            table[index] = reinterpret_cast<char *>(out);
            if (parser->is_utf16 != 0) {
                u16 *wide = reinterpret_cast<u16 *>(word);
                i32 length = NuStrLenW(wide);
                for (i32 i = 0; i < length; i++)
                    out = NuUTF8CharFromUnicode(out, wide[i]);
                *out++ = 0;
            } else {
                i32 length = NuStrLen(word);
                NuStrCpy(reinterpret_cast<char *>(out), word);
                out += length + 1;
            }
        }
        NuFParDestroy(parser);
    }
    *buffer = out;
}
i32 Text_GetMaxOverallStrings() {
    return Text_MaxStrings_Overall;
}
void Text_LocaliseDecimalPoint(char *text) {
    if ((Text_Language >= 2 && Text_Language <= 5) || Text_Language == 6 || Text_Language == 7 || Text_Language == 8 ||
        Text_Language == 12 || Text_Language == 16) {
        while (*text != '\0') {
            if (*text == '.') {
                *text = ',';
                return;
            }
            ++text;
        }
    }
}
void Text_ExpandAllButtonStrings(char *input, char *output) {
    char token[256];
    char expanded[64];

    *output = '\0';
    while (*input != '\0') {
        if (*input != '[') {
            *output = *input;
            output[1] = '\0';
            ++input;
            ++output;
            continue;
        }

        token[0] = '[';
        i32 token_length = 1;
        while (input[token_length] != ']' && input[token_length] != '\0') {
            token[token_length] = input[token_length];
            ++token_length;
        }

        char literal = '[';
        if (input[token_length] != '\0') {
            token[token_length] = ']';
            token[token_length + 1] = '\0';
            token_length = NuStrLen(token);
            if (token_length > 0 && Text_ExpandButtonString(token, expanded) != 0) {
                char *character = expanded;
                while (*character != '\0') {
                    *output = *character;
                    output[1] = '\0';
                    ++character;
                    ++output;
                }
                NuStrCat(output, expanded);
                input += token_length;
                continue;
            }
            literal = *input;
        }

        *output = literal;
        output[1] = '\0';
        ++input;
        ++output;
    }
    *output = '\0';
}
void Text_FillInExtendedSaveInfo() {
}
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length) {
    char separator;
    if (Text_Language == 2) {
        separator = ' ';
    } else if (Text_Language == 0 || Text_Language == 1 || Text_Language == 18) {
        separator = ',';
    } else {
        separator = '.';
    }

    if (length < 0) {
        length = NuStrLen(number);
    }

    i32 output = 0;
    for (i32 digit = 0; digit < length; ++digit) {
        text[output++] = number[digit];
        const i32 remaining = length - digit - 1;
        if (remaining > 0 && remaining % 3 == 0) {
            text[output++] = separator;
        }
    }
    text[output] = '\0';
}
NUMTL *MessageMtl;
i32 MessageMtlInit;

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

void DrawMessageBoxRGBA(f32 x, f32 y, f32 width, f32 height, u32 blue, u32 green, u32 red, u32 alpha, numtl_s *material,
                        i32 alignment, f32 scale) {
    const f32 scaled_width = width * scale;
    const f32 scaled_height = height * scale;

    f32 pulse = 1.0f;
    if (pulsetimerfn != NULL) {
        pulse = (pulsetimerfn((1.0f - y) * 0.5f) - 1.0f) * 4.0f + 1.0f;
    }

    i32 opacity = static_cast<i32>((static_cast<f32>(static_cast<i32>(alpha >> 16)) * 65536.0f +
                                    static_cast<f32>(static_cast<i32>(alpha & 0xffff))) *
                                   pulse);

    const i32 box_width = static_cast<i32>(scaled_width * 10240.0f * QFONTSCALEX);
    const i32 box_height = static_cast<i32>(scaled_height * 3584.0f * QFONTSCALEY);
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

    i32 screen_x = static_cast<i32>((x + 1.0f) * 0.5f * 10240.0f * QFONTSCALEX);
    i32 screen_y = static_cast<i32>((2.0f - (y + 1.0f)) * 0.5f * 3584.0f * QFONTSCALEY);
    const u32 colour = (static_cast<u32>(opacity) << 24) | ((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff);
    const u32 edge_colour = (static_cast<u32>(opacity) << 25) | 0xffffff;

    NuRndrRect2di(screen_x, screen_y, box_width, box_height, colour, material);
    NuRndrRect2di(screen_x, screen_y, edge_width, corner_height, edge_colour, material);
    NuRndrRect2di(screen_x, screen_y, corner_width, edge_height, edge_colour, material);
    screen_x += box_width;
    screen_y += box_height;
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
    void FixUpButtonsInFont(VUFNT *game_font, VUFNT *button_font) {
        if (game_font == nullptr)
            game_font = QFont2D;
        if (button_font == nullptr)
            button_font = QFont2DButtons;
        if (game_font != nullptr && button_font != nullptr) {
            f32 scale = game_font->height / button_font->height;
            for (i32 i = 0; i < button_font->glyph_count; i++) {
                for (i32 j = 0; j < game_font->glyph_count; j++) {
                    if (button_font->unicode_map[i].unicode == game_font->unicode_map[j].unicode) {
                        if (button_font->unicode_map[i].unicode >= 0x531 &&
                            button_font->unicode_map[i].unicode <= 0x53f) {
                            game_font->glyphs[game_font->unicode_map[j].index].width =
                                button_font->glyphs[button_font->unicode_map[i].index].width * scale;
                        }
                        break;
                    }
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
    void MenuText3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u8 alignment, u8 red,
                      u8 green, u8 blue, i32 alpha) {
        if (MenuDrawDropShadows != 0) {
            Text3DEx2(text, x + x_scale * 0.015f, y - y_scale * 0.015f, z, x_scale, y_scale, z_scale, alignment, 0, 0,
                      0, alpha >> 2);
        }
        Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, alpha);
    }
    void Set3DGameFont(VUFNT *font) {
        QFont3DZ = font;
    }
    void SetGameFont(VUFNT *font) {
        QFont2D = font;
    }
    void SmartText(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                   u8 green, u8 blue, f32 max_width, i32 max_lines) {
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, max_width, max_lines, NULL,
                    0, 0x80);
    }
    void SmartTextEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red,
                     u8 green, u8 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw, u32 alpha) {
        VUFNT *font = SmartTextFont != nullptr ? SmartTextFont : QFont2D;
        if (font == nullptr || text == nullptr || text[0] == '\0' || MenuStopDraw != 0)
            return;

        f32 saved_x_scale = APITEXTSCALEX;
        f32 saved_y_scale = APITEXTSCALEY;
        f32 draw_x_scale = APITEXTSCALEX * x_scale;
        f32 draw_y_scale = APITEXTSCALEY * y_scale;
        APITEXTSCALEX = 1.0f;
        APITEXTSCALEY = 1.0f;

        unsigned char decoded[513];
        TextDecode(text, decoded);
        u16 encoded[512];
        Text3DStringEncodeFont(decoded, encoded, font);
        NuQFntSet(font);
        NuQFntSetScale(font, draw_x_scale * QFONTSCALEX, draw_y_scale * QFONTSCALEY);
        f32 width = NuQFntPrintLenW(font, encoded);
        f32 available_width = max_width * STCOORDSCALE;
        if (message_box == nullptr && suppress_draw == 0 && max_lines != 0) {
            bool has_explicit_break = false;
            for (unsigned char *cursor = decoded; cursor[0] != '\0'; ++cursor) {
                if (cursor[0] == '\\' && cursor[1] == 'n') {
                    has_explicit_break = true;
                    break;
                }
            }

            if (!has_explicit_break && (available_width <= 0.0f || width <= available_width || max_lines == 1)) {
                if (available_width > 0.0f && width > available_width) {
                    draw_x_scale *= available_width / width;
                }
                Text3DEx(reinterpret_cast<char *>(decoded), x * STCOORDSCALE, y, z, draw_x_scale, draw_y_scale, z_scale,
                         alignment, red, green, blue, alpha & 0xff);
            } else {
                constexpr i32 max_wrapped_lines = 16;
                unsigned char lines[max_wrapped_lines][513] = {};
                const i32 line_limit = max_lines < max_wrapped_lines ? max_lines : max_wrapped_lines;
                unsigned char *remaining = decoded;
                i32 line_count = 0;

                while (*remaining != '\0' && line_count < line_limit) {
                    const i32 remaining_length = NuStrLen(reinterpret_cast<char *>(remaining));
                    i32 break_position = remaining_length;
                    bool forced_break = false;

                    for (i32 pos = 0; pos + 1 < remaining_length; ++pos) {
                        if (remaining[pos] == '\\' && remaining[pos + 1] == 'n') {
                            break_position = pos;
                            forced_break = true;
                            break;
                        }
                    }

                    if (!forced_break && line_count + 1 < line_limit && available_width > 0.0f) {
                        i32 last_fitting_space = -1;
                        for (i32 pos = 0; pos < remaining_length; ++pos) {
                            if (remaining[pos] != ' ') {
                                continue;
                            }

                            const unsigned char saved = remaining[pos];
                            remaining[pos] = '\0';
                            Text3DStringEncodeFont(remaining, encoded, font);
                            const f32 candidate_width = NuQFntPrintLenW(font, encoded);
                            remaining[pos] = saved;
                            if (candidate_width <= available_width) {
                                last_fitting_space = pos;
                            } else {
                                break;
                            }
                        }
                        if (last_fitting_space >= 0) {
                            break_position = last_fitting_space;
                        }
                    }

                    memcpy(lines[line_count], remaining, static_cast<usize>(break_position));
                    lines[line_count][break_position] = '\0';
                    ++line_count;
                    remaining += break_position;
                    if (forced_break) {
                        remaining += 2;
                    } else {
                        while (*remaining == ' ') {
                            ++remaining;
                        }
                    }
                }

                const f32 line_height = NuQFntHeight(font);
                f32 line_y = y;
                if ((alignment & 1) == 0) {
                    const f32 line_offset = static_cast<f32>(line_count - 1) * line_height;
                    line_y -= (alignment & 4) != 0 ? line_offset : line_offset * 0.5f;
                }
                for (i32 line = 0; line < line_count; ++line) {
                    Text3DStringEncodeFont(lines[line], encoded, font);
                    const f32 line_width = NuQFntPrintLenW(font, encoded);
                    f32 line_x_scale = draw_x_scale;
                    if (line_width > available_width) {
                        line_x_scale *= available_width / line_width;
                    }
                    Text3DEx(reinterpret_cast<char *>(lines[line]), x * STCOORDSCALE, line_y, z, line_x_scale,
                             draw_y_scale, z_scale, alignment, red, green, blue, alpha & 0xff);
                    line_y += line_height;
                }
            }
        }

        APITEXTSCALEX = saved_x_scale;
        APITEXTSCALEY = saved_y_scale;
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
    void SmartTextExDrop(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u32 red,
                         u32 green, u32 blue, f32 max_width, i32 max_lines, void *message_box, i32 suppress_draw,
                         u32 alpha) {
        SmartTextEx2(text, x - x_scale * 0.007f, y - y_scale * 0.01f, z, x_scale, y_scale, z_scale, alignment, 0, 0, 0,
                     max_width, max_lines, message_box, suppress_draw, alpha);
        SmartTextEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red & 0xff, green & 0xff, blue & 0xff,
                    max_width, max_lines, message_box, suppress_draw, alpha);
    }
    void SmartTextGetWidescreen(f32 *font_scale_x, f32 *coordinate_scale) {
        if (font_scale_x != nullptr)
            *font_scale_x = QFONTSCALEX;
        if (coordinate_scale != nullptr)
            *coordinate_scale = STCOORDSCALE;
    }
    void SmartTextSetFWNMode(i32 mode) {
        smarttext_fwn = mode;
    }
    void SmartTextSetFont(VUFNT *font) {
        SmartTextFont = font;
    }
    void SmartTextSetWidescreen(f32 font_scale_x, f32 coordinate_scale) {
        QFONTSCALEX = 1.0f - (1.0f - font_scale_x) * 0.5f;
        QFONTSCALEY = 1.0f / QFONTSCALEX;
        STCOORDSCALE = coordinate_scale;
    }
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
    void Text3D(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32 z_scale, u32 alignment, u8 red, u8 green,
                u8 blue) {
        Text3DEx(text, x, y, z, x_scale, y_scale, z_scale, alignment, red, green, blue, 128);
    }
    void Text3DEx(char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, f32, u32 alignment, u8 red, u8 green,
                  u8 blue, i32 alpha) {
        VUFNT *font = SmartTextFont != nullptr ? SmartTextFont : QFont2D;
        if (font == nullptr || text == nullptr || text[0] == '\0' || MenuStopDraw != 0 || x < -2.0f || x > 2.0f ||
            y < -2.0f || y > 2.0f)
            return;

        NuQFntPushPrintMode(smarttext_fwn == 0 ? 2 : 3);
        unsigned char decoded[512];
        TextDecode(text, decoded);
        if (followon_line == 0)
            NuQFntSet(font);

        f32 draw_x_scale = APITEXTSCALEX * x_scale;
        f32 draw_y_scale = APITEXTSCALEY * y_scale;
        u16 encoded[512];
        Text3DStringEncodeFont(decoded, encoded, font);
        NuQFntSetScale(font, draw_x_scale * QFONTSCALEX, draw_y_scale * QFONTSCALEY);
        f32 width = NuQFntPrintLenW(font, encoded);
        f32 height = NuQFntHeight(font);
        text3d_width = width;
        text3d_height = height;
        f32 draw_y = y + NuQFntBaseline(font) - height * 0.5f;

        if ((alignment & 4) != 0)
            draw_y -= height * 0.5f;
        else if ((alignment & 1) != 0)
            draw_y += height * 0.5f;
        if ((alignment & 2) == 0)
            x -= (alignment & 8) != 0 ? width : width * 0.5f;

        u32 colour =
            (static_cast<u32>(alpha) << 24) | (static_cast<u32>(red) << 16) | (static_cast<u32>(green) << 8) | blue;
        TextPrintSubstring(decoded, x, draw_y, z, draw_x_scale, draw_y_scale, colour, 0);
        NuQFntPopPrintMode();
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
    void TextDecode(char *source, unsigned char *dest) {
        i32 source_pos = 0;
        i32 dest_pos = 0;
        while (source[source_pos] != '\0' && dest_pos < 0x1e0) {
            if (source[source_pos] == '[' && source[source_pos + 1] == '[') {
                source_pos += 2;
                char codeword[512];
                i32 codeword_len = 0;
                while (source[source_pos] != ']' && source[source_pos] != '\0' && codeword_len < 511)
                    codeword[codeword_len++] = source[source_pos++];
                while (source[source_pos] == ']')
                    source_pos++;
                codeword[codeword_len] = '\0';
                char decoded[520];
                decoded[0] = '\0';
                TextDecodeCodeword(codeword, decoded);
                for (i32 i = 0; decoded[i] != '\0' && dest_pos < 0x1e0; i++)
                    dest[dest_pos++] = static_cast<unsigned char>(decoded[i]);
                continue;
            }

            dest[dest_pos++] = static_cast<unsigned char>(source[source_pos++]);
            while ((static_cast<u8>(source[source_pos]) + 0x80U) < 0x40U && dest_pos < 0x1e0)
                dest[dest_pos++] = static_cast<unsigned char>(source[source_pos++]);
        }
        dest[dest_pos] = 0;
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
}
void MenuUpdateViewTextStrings(MENU_s *menu) {
    if (menu->cancel_pressed != 0) {
        BackupMenu();
        return;
    }

    if ((Player[0] == nullptr || (Player[0]->pad_gamepad->buttons_held & GAMEPAD_MENUSELECT) == 0) &&
        (Player[1] == nullptr || (Player[1]->pad_gamepad->buttons_held & GAMEPAD_MENUSELECT) == 0)) {
        return;
    }

    if (menu->up_held != 0) {
        menu->selected_row -= 5;
        if (menu->selected_row < menu->first_row) {
            menu->selected_row = menu->last_row;
        }
        menu->selected_item = menu->selected_row - menu->first_row;
    } else if (menu->down_held != 0) {
        menu->selected_row += 5;
        if (menu->selected_row > menu->last_row) {
            menu->selected_row = menu->first_row;
        }
        menu->selected_item = menu->selected_row - menu->first_row;
    }
}

static inline u8 Text_MenuColourLerp(u8 first, u8 second, f32 amount) {
    return static_cast<u8>(
        static_cast<i32>(static_cast<f32>(first) * amount + static_cast<f32>(second) * (1.0f - amount)));
}

void MenuDrawViewTextStrings(MENU_s *menu) {
    menu->item_scale = 2.0f;
    menu->centre_offset = MENUDY * 2.0f;
    menu->draw_x = -0.75f;
    menu->draw_y = static_cast<f32>(-menu->selected_row) * menu->centre_offset;

    char text[2048];
    for (i32 i = 0; i < Text_MaxStrings_Overall; ++i) {
        dme_sx = 0.6f;
        dme_sy = menu->item_scale;
        dme_align = 0;
        GameDrawMenuEntry(menu, const_cast<char *>(" "));

        const f32 y = menu->draw_y - menu->centre_offset;
        if (MenuStopDraw != 0 || y < -1.25f || y >= 1.25f) {
            continue;
        }

        const bool registered = (Text_StringBits[i >> 5] & (1U << (i & 0x1f))) != 0;
        i32 red;
        i32 green;
        i32 blue;
        if (registered) {
            if (menu->selected_item == i) {
                if (TestForController()) {
                    if (menu_pulsate > 0.0f) {
                        red = Text_MenuColourLerp(MENUFLASH0R, MENUFLASH1R, menu_pulsate);
                        green = Text_MenuColourLerp(MENUFLASH0G, MENUFLASH1G, menu_pulsate);
                        blue = Text_MenuColourLerp(MENUFLASH0B, MENUFLASH1B, menu_pulsate);
                    } else if (menu_flash != 0) {
                        red = MENUFLASH0R;
                        green = MENUFLASH0G;
                        blue = MENUFLASH0B;
                    } else {
                        red = MENUFLASH1R;
                        green = MENUFLASH1G;
                        blue = MENUFLASH1B;
                    }
                } else if (menu_pulse > 0.0f) {
                    red = Text_MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                    green = Text_MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                    blue = Text_MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                } else {
                    red = MENUENTRYR;
                    green = MENUENTRYG;
                    blue = MENUENTRYB;
                }
            } else {
                if (menu_pulse > 0.0f) {
                    red = Text_MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                    green = Text_MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                    blue = Text_MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                } else {
                    red = MENUENTRYR;
                    green = MENUENTRYG;
                    blue = MENUENTRYB;
                }
            }
        } else {
            if (menu->selected_item == i) {
                red = 255;
                if (menu_flash < 1) {
                    green = 191;
                    blue = 0;
                } else {
                    green = 255;
                    blue = 255;
                }
            } else {
                red = 255;
                green = 0;
                blue = 0;
            }
        }

        dme_r = static_cast<u8>(red);
        dme_g = static_cast<u8>(green);
        dme_b = static_cast<u8>(blue);
        dme_rgb = 1;
        sprintf(text, "%i", i);
        SmartTextEx(text, -0.76f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 8, static_cast<u8>(red),
                    static_cast<u8>(green), static_cast<u8>(blue), 0.09000003f, 1, nullptr, 0, MenuA);

        if (TTab[i] != nullptr) {
            dme_rgb = 1;
            Text_ExpandAllButtonStrings(TTab[i], text);
            SmartTextEx(text, -0.74f, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 2, static_cast<u8>(red),
                        static_cast<u8>(green), static_cast<u8>(blue), 1.59f, 2, nullptr, 0, MenuA);
        }
    }
}

abi_ulong GetMatchLength(unsigned char *first, unsigned char *second, abi_ulong maximum) {
    abi_ulong length = 0;
    if (maximum != 0 && *first == *second) {
        do {
            ++length;
        } while (length != maximum && first[length] == second[length]);
    }
    return length;
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
extern "C" void Text3DStringEncode(char *src, u16 *dst) {
    VUFNT *font = SmartTextFont;
    if (font == nullptr)
        font = QFont2D;
    Text3DStringEncodeFont(reinterpret_cast<unsigned char *>(src), dst, font);
}
extern "C" void SetQFont2D(void) {
    NuQFntSetCoordinateSystem(static_cast<NUQFNT_CSMODE>(3));
    NuQFntSet(QFont2D);
    NuQFntSetSpaceWidth(QFont2D, 12.0f);
}
void IntroText_SetTextID(i32 id) {
    if (id > 0 && TTab[id] != nullptr)
        IntroText_TextID = id;
}
void IntroText_Draw(float alpha) {
    if (IntroText_TextID == -1)
        return;
    SetQFont2D();
    NuQFntSetSpaceWidth(QFont2D, 8.0f);
    u16 encoded[128];
    Text3DStringEncode(TTab[IntroText_TextID], encoded);
    NuQFntSetJustifiedTolerances(1.0f, 1.0f);
    u32 colour = (static_cast<i32>(alpha * 128.0f) << 24) | 0x7f5f00;
    NuQFntPrintJustifiedW(QFont2D, encoded, -0.85f, INTROTEXT_Y, 1.0f, INTROTEXT_SCALE, INTROTEXT_SCALE, 1.7f, 1.0f,
                          colour, 0);
}
