#include "decomp.h"
#include "batman.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/audio/audio.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/credits.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/core/render.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nufile/nufpar.h"
#include <stdio.h>

f32 CreditsAlpha;
f32 CreditsTime;
f32 CreditsFinishedTime;
i32 CreditsFlag;
static f32 Credits_Duration = 120.0f;

struct CREDIT_TYPE_s {
    const char *name;
    f32 scale;
    f32 width;
    u8 default_colour[4];
    u8 colour[4];
    u8 reserved[3];
    u8 type;
};
DECOMP_ASSERT(sizeof(CREDIT_TYPE_s) == 0x18, "credit style ABI");
static CREDIT_TYPE_s CreditType[5] = {
    {"heading", 0.4f, 1.9f, {255, 255, 255, 128}, {}, {}, 0},  {"company", 0.6f, 1.9f, {255, 255, 255, 128}, {}, {}, 0},
    {"title", 0.35f, 0.935f, {255, 255, 255, 128}, {}, {}, 0}, {"name", 0.4f, 0.935f, {255, 255, 255, 128}, {}, {}, 0},
    {"credit", 0.4f, 1.9f, {255, 255, 255, 128}, {}, {}, 0},
};
CREDIT_s *CreditList;
static f32 CreditsSize;
static i32 CreditCount;

static inline void InitCredit(CREDIT_s *credit, char *text, f32 x, f32 y, f32 scale, u8 red, u8 green, u8 blue,
                              u8 alpha, u8 alignment, VARIPTR *buffer, VARIPTR *buffer_end) {
    char *copy = NULL;
    if (text != NULL) {
        i32 length = NuStrLen(text);
        if (length != 0) {
            copy = static_cast<char *>(GameBufferAlloc(buffer, buffer_end, length + 1));
            NuStrCpy(copy, text);
        }
    }
    credit->text = copy;
    credit->x = x;
    credit->y = y;
    credit->scale = scale;
    credit->red = red;
    credit->green = green;
    credit->blue = blue;
    credit->alpha = alpha;
    credit->alignment = alignment;
}

void Credits_Init(WORLDINFO_s *) {
    CreditsAlpha = 1.0f;
    CreditsTime = 0.0f;
    CreditsFinishedTime = 0.0f;
    CreditsFlag = 0;
    if (LastLData != STATUS_LDATA) {
        BackDrop_ResetColours();
    }
    MechSystems::Get()->HookUpClickToPressStart();
}

void Credits_Load(WORLDINFO_s *, VARIPTR *buffer, VARIPTR *buffer_end) {
    for (i32 i = 0; i < 5; ++i) {
        for (i32 component = 0; component < 4; ++component) {
            CreditType[i].colour[component] = CreditType[i].default_colour[component];
        }
        CreditType[i].type = i;
    }
    CreditsSize = 0.0f;
    Credits_Duration = 120.0f;
    char filename[256];
    NuStrCpy(filename, "stuff\\text\\");
    NuStrCat(filename, "english");
    NuStrCat(filename, "_credits.txt");
    CreditList = static_cast<CREDIT_s *>(GameBufferAlloc(buffer, buffer_end, 1000 * sizeof(CREDIT_s)));
    CreditCount = 0;
    if (CreditList == NULL) {
        return;
    }
    NUFPAR *parser = NuFParCreate(filename);
    if (parser == NULL) {
        return;
    }
    CREDIT_s *credit = CreditList;
    f32 y = 0.2f;
    while (NuFParGetLine(parser)) {
        if (NuFParGetWord(parser)) {
            if (NuStrICmp(parser->word_buf, "duration") == 0) {
                if (NuFParGetWord(parser) && Credits_Duration == 120.0f) {
                    f32 duration = NuAToF(parser->word_buf);
                    if (duration > 0.0f) {
                        Credits_Duration = duration;
                    }
                }
            } else {
                const char *style_word = parser->word_buf;
                i32 style_index;
                if (NuStrICmp(CreditType[0].name, style_word) == 0)
                    style_index = 0;
                else if (NuStrICmp(CreditType[1].name, style_word) == 0)
                    style_index = 1;
                else if (NuStrICmp(CreditType[2].name, style_word) == 0)
                    style_index = 2;
                else if (NuStrICmp(CreditType[3].name, style_word) == 0)
                    style_index = 3;
                else if (NuStrICmp(CreditType[4].name, style_word) == 0)
                    style_index = 4;
                else
                    style_index = -1;
                CREDIT_TYPE_s *type = style_index == -1 ? NULL : &CreditType[style_index];
                if (type != NULL) {
                    char *text = NuFParGetWord(parser) ? parser->word_buf : NULL;
                    if (text != NULL && NuStrICmp(text, "colour") == 0) {
                        for (i32 component = 0; component < 4; ++component) {
                            if (NuFParGetWord(parser)) {
                                f32 value = NuAToF(parser->word_buf);
                                i32 maximum = component == 3 ? 128 : 255;
                                type->colour[component] = value > 1.0f   ? maximum
                                                          : value < 0.0f ? 0
                                                                         : static_cast<i32>(value * maximum);
                            }
                        }
                    } else if (CreditCount < 1000) {
                        credit->type = type->type;
                        switch (type->type) {
                            case 0:
                                InitCredit(credit, text, 0.0f, y, type->scale, type->colour[0], type->colour[1],
                                           type->colour[2], type->colour[3], 0, buffer, buffer_end);
                                y += CreditType[0].scale / CreditType[1].scale * 0.2f;
                                break;
                            case 1:
                                InitCredit(credit, text, 0.0f, y, type->scale, type->colour[0], type->colour[1],
                                           type->colour[2], type->colour[3], 0, buffer, buffer_end);
                                y += CreditType[1].scale / CreditType[1].scale * 0.2f;
                                break;
                            case 4:
                                InitCredit(credit, text, 0.0f, y, type->scale, type->colour[0], type->colour[1],
                                           type->colour[2], type->colour[3], 0, buffer, buffer_end);
                                y += CreditType[4].scale / CreditType[1].scale * 0.2f;
                                break;
                            case 2:
                                InitCredit(credit, text, -0.015f, y, type->scale, type->colour[0], type->colour[1],
                                           type->colour[2], type->colour[3], 8, buffer, buffer_end);
                                break;
                            case 3:
                                InitCredit(credit, text, 0.015f, y, type->scale, type->colour[0], type->colour[1],
                                           type->colour[2], type->colour[3], 2, buffer, buffer_end);
                                y += CreditType[3].scale / CreditType[1].scale * 0.2f;
                                break;
                        }
                        ++CreditCount;
                        ++credit;
                    }
                }
            }
        }
        CreditsSize = y + 2.2f;
    }
    NuFParDestroy(parser);
}

void Credits_GetInfo(float *duration, i32 *flag, float *alpha) {
    if (duration != NULL) {
        *duration = Credits_Duration;
    }
    if (flag != NULL) {
        *flag = CreditsFlag;
    }
    if (alpha != NULL) {
        *alpha = CreditsAlpha;
    }
}

void Credits_DrawPanel(WORLDINFO_s *) {
    f32 time = CreditsTime;
    CREDIT_s *credit = CreditList;
    i32 count = CreditCount;
    if (CreditsFlag == 0) {
        f32 offset = (time / Credits_Duration) * CreditsSize;
        for (i32 i = 0; i < count; ++i, ++credit) {
            if (offset >= credit->y - 0.2f && offset <= credit->y + 2.2f) {
                i32 alpha = static_cast<i32>(credit->alpha * CreditsAlpha);
                SmartTextEx(credit->text, credit->x, -1.0f - (credit->y - offset), 1.0f, credit->scale, credit->scale,
                            credit->scale, credit->alignment, credit->red, credit->green, credit->blue,
                            CreditType[credit->type].width, 1, NULL, 0, alpha);
            }
        }
    } else {
        char percentage[32];
        f32 points = Game_CompletionSave != NULL ? Game_CompletionSave[0] * 100 : 0;
        sprintf(percentage, "%.1f%%", static_cast<double>(points / COMPLETIONPOINTS));
        f32 x = 0.0f;
        if (CreditsFlag == 1) {
            x = CreditsAlpha * -0.1f + 0.1f;
        } else if (CreditsFlag == 3) {
            x = 0.0f - (1.0f - CreditsAlpha) * 0.1f;
        }
        i32 alpha = static_cast<i32>(static_cast<i32>(CreditsAlpha * 255.0f) * 0.25f);
        SmartTextEx(percentage, x, 0.0f, 1.0f, 1.125f, 1.125f, 1.125f, 0, 255, 191, 0, 1.7f, 1, NULL, 0, alpha);
    }
}

void Credits_UpdateMenu(MENU_s *menu) {
    f32 volume = Game_OptionsSave != NULL ? GameSetMusicVolume(Game_OptionsSave) : 1.0f;
    f32 previous_time = CreditsTime;
    if (CreditsFlag == 0) {
        if (FadeSys.fade == 0.0f) {
            CreditsTime += FRAMETIME;
        }
        if (CreditsFinishedTime > 0.0f) {
            CreditsFinishedTime += FRAMETIME;
            if (CreditsFinishedTime >= 1.1f) {
                CreditsFlag = 1;
                CreditsAlpha = 0.0f;
                CreditsTime = 0.0f;
            }
            if (CreditsAlpha > 0.0f) {
                CreditsAlpha -= FRAMETIME;
                if (CreditsAlpha < 0.0f) {
                    CreditsAlpha = 0.0f;
                }
            }
        } else if ((menu->buttons_pressed & GAMEPAD_SKIP) != 0 || CreditsTime >= Credits_Duration ||
                   MechSystems::SkipTextScroll != 0) {
            CreditsFinishedTime = 0.001f;
            MechSystems::Get()->UnhookClickToPressStart();
        }
        legoSetMusicVolume(CreditsAlpha * volume);
    } else {
        legoSetMusicVolume(0.0f);
        CreditsTime += FRAMETIME;
        f32 progress;
        if (CreditsFlag == 1) {
            if (previous_time < 0.1f && CreditsTime >= 0.1f) {
                music_man.StopAll(0);
                MusicClearAll();
                SoundKillAll();
            }
            if (CreditsTime >= 1.0f) {
                CreditsFlag = 2;
                GameAudio_PlaySfx(0x2b, NULL, 0, 0);
                progress = 1.0f;
            } else {
                progress = CreditsTime;
            }
        } else if (CreditsFlag == 2) {
            if (CreditsTime >= 3.0f) {
                CreditsFlag = 3;
                CreditsTime = 0.0f;
            }
            progress = 1.0f;
        } else {
            if (CreditsTime >= 1.1f) {
                NewLData = HUB_LDATA;
                FADETYPE fade_type = {FADE_TYPE_STILL};
                FadeSys.SetFade(fade_type, 0);
            }
            progress = 1.0f - CreditsTime;
            if (progress < 0.0f) {
                progress = 0.0f;
            }
        }
        CreditsAlpha = NuTrigTable[(static_cast<i32>(progress * 16384.0f) >> 1) & 0x7fff];
    }
}
