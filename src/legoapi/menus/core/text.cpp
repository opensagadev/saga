#include "decomp.h"
#include "gameapi/gui/apimenu_internal.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
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
static LANGUAGEDATA Text_LanguageList_Default[7] = {{1, apitxt_ENGLISH}, {2, apitxt_FRENCH},  {4, apitxt_GERMAN},
                                                    {5, apitxt_ITALIAN}, {3, apitxt_SPANISH}, {8, apitxt_DANISH},
                                                    {-1, NULL}};
LANGUAGEDATA *Text_LanguageList = Text_LanguageList_Default;
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
void (*Text_GameSetLanguageFn)(i32);
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
    if (show_hours != 0 || show_minutes != 0) {
        seconds = static_cast<i32>(NuFmod(time, 60.0f));
    } else {
        seconds = static_cast<i32>(time);
    }
    const i32 centiseconds = static_cast<i32>(NuFmod(time, 1.0f) * 100.0f);

    if (text == nullptr) {
        return;
    }

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
    i32 episode_index = -1;
    i32 body_text_id = -1;
    i32 area_index = -1;
    const i8 crawl_area = reinterpret_cast<i8 *>(s_crawlPtr)[0x0d];
    if (crawl_area != -1) {
        area = &ADataList[crawl_area];
        area_index = static_cast<i8>(area->area_index);
        episode_index = static_cast<i8>(area->episode_index);
        body_text_id = area->text_id;
    }

    char *episode_text = NULL;
    char *episode_name = NULL;
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
                if ((area->flags & AREAFLAG_BONUS_AREA) != 0) {
                    i16 *bonus_text = (area->flags & AREAFLAG_VEHICLE_AREA) != 0 ? s_crawlPtr->vehicle_bonus_text
                                                                                 : s_crawlPtr->character_bonus_text;
                    if (bonus_text != NULL) {
                        chapter_text = TTab[*bonus_text];
                    }
                } else {
                    if (s_crawlPtr->heading_text != NULL) {
                        sprintf(chapter_buffer, "%s %i", TTab[*s_crawlPtr->heading_text], area_index + 1);
                        chapter_text = chapter_buffer;
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
    const u32 colour = (static_cast<u32>(static_cast<i32>(128.0f * colour_scale * alpha)) << 24) |
                       ((static_cast<u32>(static_cast<i32>(111.0f * colour_scale)) & 0xff) << 8) | 0xff;
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
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (episode_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(episode_text, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (chapter_text != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale);
        Text3DStringEncode(chapter_text, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
        NuQFntPrintW(QFont3DZ, encoded);
        y += NuQFntHeight(QFont3DZ) * 1.5f;
    }

    if (area_name != NULL) {
        NuQFntSetScale(QFont3DZ, x_scale, y_scale * 2.0f);
        Text3DStringEncode(area_name, encoded);
        NuQFntMove(QFont3DZ, -NuQFntPrintLenW(QFont3DZ, encoded) * 0.5f, y, 0.0f);
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
            y +=
                NuQFntPrintJustifiedW(QFont3DZ, encoded, -3.5f, y, 0.0f, x_scale, y_scale, 7.0f, 1.3f, colour, &matrix);
            y += NuQFntHeight(QFont3DZ);
        }
    }
    NuQFntPopPrintMode();
}
f32 TextPulseTimer(f32 delay);
static char **TTab_Original;
static i32 Text_MaxStrings_Overall;
static u32 *Text_StringBits;

void Text_InitTable(TEXTENTRY *entry, i32 first, i32 last) {
    if (entry == nullptr)
        return;

    i32 index = 0;
    while (entry != nullptr && entry->text_id != nullptr) {
        if (entry->value != -1) {
            index = entry->value;
            if (index >= first && index <= last)
                *entry->text_id = entry->value;
        } else {
            if (index >= first && index <= last) {
                entry->value = index;
                *entry->text_id = index;
                Text_StringBits[index / 32] |= 1U << (index & 0x1f);
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
void Text_LoadStrings(variptr_u *buf, variptr_u *buf_end);
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
void TextDecodeCodeword(char *source, char *destination);
extern f32 g_buttonFontScalePulse;
extern f32 smarttextex_longestwidth;
i32 MenuStopDraw;
extern i32 smarttext_fwn;
extern i32 apitext_half_rgb;
extern u8 text_preset_b[10];
extern u8 text_preset_g[10];
extern u8 text_preset_r[10];
extern i32 ButtonScaleMode;

f32 TextPrintSubstring(unsigned char *text, f32 x, f32 y, f32 z, f32 x_scale, f32 y_scale, i32 colour,
                       i32 button_font);
void Text_DecodeButtons(char *source, char *destination);
char *txtpath_JAPANESE = const_cast<char *>("japanese");
char *txtpath_DANISH = const_cast<char *>("danish");
char *txtpath_SPANISH = const_cast<char *>("spanish");
char *txtpath_ITALIAN = const_cast<char *>("italian");
char *txtpath_GERMAN = const_cast<char *>("german");
char *txtpath_FRENCH = const_cast<char *>("french");
char *txtpath_ENGLISH = const_cast<char *>("english");

char *Text_GetLanguagePath(i32 language) {
    switch (language) {
        case 0:
            return txtpath_JAPANESE;
        case 2:
            return txtpath_FRENCH;
        case 3:
            return txtpath_SPANISH;
        case 4:
            return txtpath_GERMAN;
        case 5:
            return txtpath_ITALIAN;
        case 8:
            return txtpath_DANISH;
        default:
            return txtpath_ENGLISH;
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
void Text_SetLanguage_Game(i32 language);
void Text3DStringEncodeFont(unsigned char *src, u16 *dst, void *font);
void Text_InitDefaultStrings();
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count) {
    unsigned char *out = *buffer;
    NUFPAR *parser = NuFParCreate(reinterpret_cast<char *>(filename));
    if (parser != nullptr) {
        if (parser->is_utf16 != 0) {
            while (NuFParGetLineW(parser) != 0) {
                i32 index = NuFParGetInt(parser);
                if (!(index <= 0) && !(index >= count)) {
                    NuFParGetWordW(parser);
                    u16 *wide = reinterpret_cast<u16 *>(parser->word_buf);
                    i32 length = NuStrLenW(wide);
                    if (length <= 0)
                        continue;
                    table[index] = reinterpret_cast<char *>(out);
                    for (i32 i = 0; i < length; i++)
                        out = NuUTF8CharFromUnicode(out, wide[i]);
                    *out++ = 0;
                }
            }
        } else if (parser->is_utf8 != 0) {
            while (NuFParGetLine(parser) != 0) {
                NuFParGetWord(parser);
                if (parser->word_buf[0] == 0)
                    continue;
                i32 index = NuAToI(parser->word_buf);
                if (index <= 0 || index >= count)
                    continue;
                NuFParGetWord(parser);
                if (NuStrICmp(parser->word_buf, "360") == 0 || NuStrICmp(parser->word_buf, "gc") == 0 ||
                    NuStrICmp(parser->word_buf, "ps2") == 0 || NuStrICmp(parser->word_buf, "ps3") == 0 ||
                    NuStrICmp(parser->word_buf, "psp") == 0 || NuStrICmp(parser->word_buf, "pc") == 0 ||
                    NuStrICmp(parser->word_buf, "wii") == 0 || NuStrICmp(parser->word_buf, "xbox") == 0)
                    continue;
                i32 length = NuStrLen(parser->word_buf);
                table[index] = reinterpret_cast<char *>(out);
                NuStrCpy(reinterpret_cast<char *>(out), parser->word_buf);
                out += length + 1;
            }
        } else {
            while (NuFParGetLine(parser) != 0) {
                i32 index = NuFParGetInt(parser);
                if (index <= 0 || index >= count)
                    continue;
                NuFParGetWord(parser);
                if (NuStrICmp(parser->word_buf, "360") == 0 || NuStrICmp(parser->word_buf, "gc") == 0 ||
                    NuStrICmp(parser->word_buf, "ps2") == 0 || NuStrICmp(parser->word_buf, "ps3") == 0 ||
                    NuStrICmp(parser->word_buf, "psp") == 0 || NuStrICmp(parser->word_buf, "pc") == 0 ||
                    NuStrICmp(parser->word_buf, "wii") == 0 || NuStrICmp(parser->word_buf, "xbox") == 0)
                    continue;
                char *word = parser->word_buf;
                i32 length = NuStrLen(word);
                if (length <= 0)
                    continue;
                table[index] = reinterpret_cast<char *>(out);
                for (i32 i = 0; i < length; ++i) {
                    const u8 character = static_cast<u8>(word[i]);
                    if (character >= 0x80)
                        out = NuUTF8CharFromUnicode(out, character);
                    else
                        *out++ = character;
                }
                *out++ = 0;
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
    if ((Text_Language >= 2 && Text_Language <= 5) || Text_Language == 8 || Text_Language == 6 || Text_Language == 16 ||
        Text_Language == 7 || Text_Language == 12) {
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
void Text_FillInExtendedSaveInfo();
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length) {
    char separator;
    if (Text_Language == 2) {
        separator = ' ';
    } else if (Text_Language == 1 || Text_Language == 18 || Text_Language == 0) {
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
        if (remaining != 0 && remaining % 3 == 0) {
            text[output++] = separator;
        }
    }
    text[output] = '\0';
}
extern NUMTL * MessageMtl;
extern i32 MessageMtlInit;

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
    i32 blue;
    i32 green;
    menu->item_scale = 2.0f;
    menu->centre_offset = MENUDY * 2.0f;
    menu->draw_x = -0.75f;
    menu->draw_y = static_cast<f32>(-menu->selected_row) * menu->centre_offset;

    char text[2048];
    auto menu_item_scale = menu->item_scale;
    for (i32 i = 0; i < Text_MaxStrings_Overall; ++i) {
        dme_sy = menu_item_scale;
        dme_sx = 0.6f;
        dme_align = 0;
        GameDrawMenuEntry(menu, const_cast<char *>(" "));

        const f32 draw_y = menu->draw_y;
        const f32 centre_offset = menu->centre_offset;
        if (MenuStopDraw != 0) {
            continue;
        }
        const f32 y = draw_y - centre_offset;
        if (!(y >= -1.25f && y < 1.25f)) {
            continue;
        }

        const bool registered = (Text_StringBits[i >> 5] & (1U << (i & 0x1f))) != 0;
        i32 red;

        if (registered) {
            if (menu->selected_item == i) {
                if (!TestForController()) {
                    if (menu_pulse > 0.0f) {
                        red = Text_MenuColourLerp(MENUFLASH0R, MENUNORMALR, menu_pulse);
                        green = Text_MenuColourLerp(MENUFLASH0G, MENUNORMALG, menu_pulse);
                        blue = Text_MenuColourLerp(MENUFLASH0B, MENUNORMALB, menu_pulse);
                    } else {
                        red = MENUENTRYR;
                        green = MENUENTRYG;
                        blue = MENUENTRYB;
                    }
                } else {
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
            if (menu->selected_item != i) {
                red = 255;
                green = 0;
                blue = 0;
            } else {
                red = 255;
                if (menu_flash < 1) {
                    green = 191;
                    blue = 0;
                } else {
                    green = 255;
                    blue = 255;
                }
            }
        }

        dme_r = static_cast<u8>(red);
        dme_g = static_cast<u8>(green);
        dme_rgb = 1;
        dme_b = static_cast<u8>(blue);
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
    if (maximum != 0 && *second == *first) {
        do {
            ++length;
        } while (length != maximum && first[length] == second[length]);
    }
    return length;
}
i32 SplitTextFindNextWS(unsigned char *text, i32 position);
void MatrixTextStringEncode(void *font, unsigned char *source, u16 *destination);
extern "C" void Text3DStringEncode(char *src, u16 *dst);
extern "C" void SetQFont2D(void);
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

// These queries follow GameDrawMenuEntry in the reference text/menu helpers,
// separate from UpdateGameMenu's input-routing translation unit.
i32 GetMenuID(void) {
    if (GameMenu[GameMenuLevel].menu != -1) {
        return MenuInfo[GameMenu[GameMenuLevel].menu].id;
    }
    return -1;
}

i32 GetParentMenuID() {
    if (GameMenuLevel <= 1) {
        return -1;
    }
    const i16 parent_menu = GameMenu[GameMenuLevel - 1].menu;
    if (parent_menu == -1) {
        return -1;
    }
    return MenuInfo[parent_menu].id;
}
