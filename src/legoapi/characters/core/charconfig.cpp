#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/nufile/nufilepak.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nutex.h"

#include <stdio.h>
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Move_JEDI(GameObject_s *object);
void Animate_JEDI(GameObject_s *object);
void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function);
void CharConfig_CalculateJumpStats(f32 jump_speed, f32 gravity, f32 *duration, f32 *height);
i32 Text_StripComments(char *text, char *destination, i32 separators);

void CharVariant_Find(char *) {
}

void CharVariants_Init(CHARVARIANT *, i32) {
}

void CharCategories_Init(CHARCATEGORY *) {
}

void CanWearHatsInFreePlay(i32) {
}

void CharCategory_FindByName(char *) {
}

void CharCategory_IsCategory(GameObject_s *, i32) {
}

CHARCONFIG_s charconfig;
i32 SecondCharConfigure;
i32 configureallcharacters_perm;
f32 vehicle_timebase_dist[4] = {50.0f, 25.0f, 0.0f, 0.0f};
i32 vehicle_timebase_nframes[4] = {4, 2, 0, 0};
char MiniBuffer[0x4000];
extern i32 CHARPAK;
extern "C" char ConfigBuffer[0x10000];

static i32 RedirectTextFile(char *text, char *filename, i32 strip_comments) {
    i32 result = 0;
    NUFPAR *parser = NuFParCreateMem("redirect", text, 0xffff);
    if (parser != NULL) {
        while (NuFParGetLine(parser) != 0) {
            if (strip_comments != 0 && Text_StripComments(parser->line_buf, parser->line_buf, 1) == 0) continue;
            if (NuFParGetWord(parser) == 0) continue;
            if (NuStrICmp(parser->word_buf, "txt_file") == 0) {
                if (result == 0 && NuFParGetWord(parser) != 0 && NuStrLen(parser->word_buf) < 64) {
                    result = 1;
                    NuStrCpy(filename, parser->word_buf);
                }
            } else if (result != 0) {
                result = 2;
                break;
            }
        }
        NuFParDestroy(parser);
    }
    return result;
}

static i32 CharConfig(i32 character_id, char *directory, char *filename, VARIPTR *arena, VARIPTR *arena_end,
                      i32 permanent, char *text, i32 text_length, i32 redirect, NUFPCOMJMP *game_keywords) {
    CHARACTERDATA *character = &apicharsys->char_data[character_id];
    char redirected_name[64];
    char path[128];
    i32 redirected = 0;
    if (text == NULL) {
        if (directory == NULL) {
            SecondCharConfigure = 0;
            return 0;
        }
        NuStrCpy(path, directory);
        NuStrCat(path, filename);
        i32 size = NuFileLoadBuffer(path, MiniBuffer, sizeof(MiniBuffer));
        if (size < 1) {
            SecondCharConfigure = 0;
            return 0;
        }
        MiniBuffer[size] = 0;
        text_length = Text_StripComments(MiniBuffer, MiniBuffer, 1);
        if (text_length < 1) {
            SecondCharConfigure = 0;
            return 0;
        }
        if (redirect != 0 && (redirected = RedirectTextFile(MiniBuffer, redirected_name, 1)) != 0) {
            NuStrCpy(path, directory);
            NuStrCat(path, redirected_name);
            NuStrCat(path, ".txt");
            size = NuFileLoadBuffer(path, MiniBuffer, sizeof(MiniBuffer));
            if (size < 1) {
                SecondCharConfigure = 0;
                return 0;
            }
            MiniBuffer[size] = 0;
            text_length = Text_StripComments(MiniBuffer, MiniBuffer, 1);
        }
        text = MiniBuffer;
    }
    if (text_length < 1) {
        SecondCharConfigure = 0;
        return redirected;
    }

    GAMECHARACTERLAYER_s layers[32];
    CHARACTER_EFFECT_s effects[33];
    memset(&charconfig, 0, sizeof(charconfig));
    charconfig.character_id = static_cast<i16>(character_id);
    charconfig.character = character;
    charconfig.runtime = static_cast<GAMECHARACTERDATA_s *>(character->field11_0x24);
    charconfig.flags = (permanent & 1) << 6;
    charconfig.effect_scratch = effects;
    charconfig.arena = arena;
    charconfig.arena_end = arena_end;
    charconfig.layer_scratch = layers;
    memset(effects, 0, sizeof(effects));
    GAMECHARACTERDATA_s *data = charconfig.runtime;
    if (data->layers == NULL && (character->model_flags & 1) == 0 && arena != NULL && arena_end != NULL) {
        data->layer_count = 0;
        data->layers = layers;
        charconfig.flags |= 2;
    } else if (data->layer_count != 0) {
        charconfig.named_layers = 1;
    }
    if (character->animations == NULL && arena != NULL && arena_end != NULL) {
        arena->addr = ALIGN(arena->addr, 4);
        character->animations = static_cast<CHARACTERANIM_s *>(arena->void_ptr);
        charconfig.flags |= 0x20;
    }
    NUFPAR *parser = NuFParCreateMem("character", text, 0xffff);
    if (parser != NULL) {
        NuFParPushCom2(parser, CharConfig_GetKeywords(), game_keywords);
        while (NuFParGetLine(parser) != 0) {
            if (NuFParGetWord(parser) != 0) NuFParInterpretWord(parser);
        }
        NuFParDestroy(parser);
    }
    character->model_flags |= 1;
    if ((charconfig.flags & 0xc) == 4) data->field_0x78 = data->turn_rate;
    if ((charconfig.flags & 0x10) == 0 && (character->model_flags & 0x2000) != 0) {
        data->ai_update_distance_0 = vehicle_timebase_dist[0];
        data->ai_update_distance_1 = vehicle_timebase_dist[1];
        data->ai_update_distance_2 = vehicle_timebase_dist[2];
        data->ai_update_distance_3 = vehicle_timebase_dist[3];
        data->ai_update_interval_0 = static_cast<u8>(vehicle_timebase_nframes[0]);
        data->ai_update_interval_1 = static_cast<u8>(vehicle_timebase_nframes[1]);
        data->ai_update_interval_2 = static_cast<u8>(vehicle_timebase_nframes[2]);
        data->ai_update_interval_3 = static_cast<u8>(vehicle_timebase_nframes[3]);
    }
    CharConfig_CalculateJumpStats(data->jump_speed, data->gravity, &data->jump_duration, &data->jump_height);
    CharConfig_CalculateJumpStats(data->second_jump_speed, data->gravity, &data->second_jump_duration, &data->second_jump_height);
    if ((charconfig.flags & 0x20) != 0) {
        CHARACTERANIM_s *sentinel = &character->animations[charconfig.animation_count];
        character->field5_0x14 = charconfig.animation_count++;
        sentinel->name = NULL;
        sentinel->action_id = -1;
        arena->void_ptr = character->animations + charconfig.animation_count;
        for (i32 i = 0; i < charconfig.animation_count - 1; ++i) {
            char *name = static_cast<char *>(arena->void_ptr);
            NuStrCpy(name, charconfig.animation_names[i]);
            character->animations[i].name = name;
            arena->addr += NuStrLen(name) + 1;
        }
        if (charconfig.effect_count > 0) {
            effects[charconfig.effect_count++].character_id = -1;
            arena->addr = ALIGN(arena->addr, 4);
            character->effects = static_cast<CHARACTER_EFFECT_s *>(arena->void_ptr);
            memmove(character->effects, effects, charconfig.effect_count * sizeof(*effects));
            arena->addr += charconfig.effect_count * sizeof(*effects);
        }
    }
    if ((charconfig.flags & 2) != 0) {
        if (data->layer_count == 0) {
            data->layers = NULL;
            data->layer_count = 0;
        } else {
            arena->addr = ALIGN(arena->addr, 4);
            data->layers = static_cast<GAMECHARACTERLAYER_s *>(arena->void_ptr);
            arena->addr += data->layer_count * sizeof(*layers);
            memmove(data->layers, layers, data->layer_count * sizeof(*layers));
            data->layer_lookup = static_cast<i8 *>(arena->void_ptr);
            arena->addr += 32;
            for (i32 bit = 0; bit < 32; ++bit) {
                for (i32 layer = 0; layer < data->layer_count; ++layer) {
                    if (data->layers[layer].mask_bit == bit) data->layer_lookup[bit] = static_cast<i8>(layer);
                }
            }
        }
    }
    SecondCharConfigure = 0;
    return redirected;
}

void CharConfig_ConfigureAll(i32 permanent, NUFPCOMJMP *game_keywords) {
    void *pak = NULL;
    configureallcharacters_perm = permanent;
    if (CHARPAK != 0 && permanent != 0) {
        VARIPTR cursor;
        cursor.addr = superbuffer_end.addr - 0x100000;
        pak = NuFilePakLoad("chars\\charstxt.fpk", &cursor, superbuffer_end, 4);
    }
    for (i32 id = 0; id < CHARCOUNT; ++id) {
        if (permanent == 0 && apicharsys->playermodelids[id] == -1) continue;
        CHARACTERDATA *character = &CDataList[id];
        char directory[256];
        char filename[256];
        char path[256];
        char original_path[256];
        NuStrCpy(directory, "chars\\");
        NuStrCat(directory, character->dir);
        NuStrCat(directory, "\\");
        NuStrCpy(filename, character->file);
        NuStrCat(filename, ".txt");
        NuStrCpy(path, directory);
        NuStrCat(path, filename);
        NuStrCpy(original_path, path);
        if (pak == NULL) {
            VARIPTR *arena = permanent != 0 ? &permbuffer_ptr : NULL;
            VARIPTR *end = permanent != 0 ? &permbuffer_end : NULL;
            if (CharConfig(id, directory, filename, arena, end, permanent != 0, NULL, 0, 1, game_keywords) == 2) {
                SecondCharConfigure = 1;
                CharConfig(id, directory, filename, arena, end, permanent != 0, NULL, 0, 0, game_keywords);
            }
            continue;
        }
        i32 item = NuFilePakGetItem(pak, path);
        void *address;
        i32 size;
        if (item == 0 || NuFilePakGetItemInfo(pak, item, &address, &size) == 0 || size < 1) continue;
        char *text = static_cast<char *>(address);
        char saved = text[size];
        text[size] = 0;
        i32 length = Text_StripComments(text, ConfigBuffer, 1);
        text[size] = saved;
        if (length < 1) continue;
        i32 redirected = RedirectTextFile(text, filename, 1);
        if (redirected != 0) {
            NuStrCat(filename, ".txt");
            NuStrCpy(path, directory);
            NuStrCat(path, filename);
            item = NuFilePakGetItem(pak, path);
            if (item == 0 || NuFilePakGetItemInfo(pak, item, &address, &size) == 0) {
                redirected = 0;
            } else {
                if (size < 1) continue;
                text = static_cast<char *>(address);
                saved = text[size];
                text[size] = 0;
                length = Text_StripComments(text, ConfigBuffer, 1);
                text[size] = saved;
                if (length < 1) continue;
            }
        }
        CharConfig(id, NULL, NULL, &permbuffer_ptr, &permbuffer_end, 1, ConfigBuffer, length, 0, game_keywords);
        if (redirected == 2) {
            item = NuFilePakGetItem(pak, original_path);
            if (item == 0 || NuFilePakGetItemInfo(pak, item, &address, &size) == 0 || size < 1) continue;
            text = static_cast<char *>(address);
            saved = text[size];
            text[size] = 0;
            length = Text_StripComments(text, ConfigBuffer, 1);
            text[size] = saved;
            if (length > 0) {
                SecondCharConfigure = 1;
                CharConfig(id, NULL, NULL, &permbuffer_ptr, &permbuffer_end, 0, ConfigBuffer, length, 0, game_keywords);
            }
        }
    }
}

void CharConfig_CalculateJumpStats(float jump_speed, float gravity, float *duration, float *height) {
    f32 ascent_time = 0.0f;
    const f32 downward_speed = 0.0f - jump_speed;
    if (gravity != 0.0f && downward_speed != 0.0f) ascent_time = downward_speed / gravity;
    if (duration != NULL) {
        *duration = ascent_time + ascent_time;
    }
    if (height != NULL) {
        *height = jump_speed * ascent_time + gravity * 0.5f * ascent_time * ascent_time;
    }
}

void ExtraCharacterFixUpAfterConfig() {
    // This is the first assignment made by the target fix-up routine and
    // covers the ordinary playable Jedi characters in the Cantina.
    SetMoveAndAnimateFunctions(CHARACTER_MODEL_FLAG_JEDI, CHARACTER_MODEL_FLAG_JEDI, 0, 0, -1,
                               reinterpret_cast<void *>(Move_JEDI), reinterpret_cast<void *>(Animate_JEDI), NULL);
}
