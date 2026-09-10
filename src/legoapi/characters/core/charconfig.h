#pragma once

#include "legoapi/characters/core/character.h"
#include "nu2api/nufile/nufpar.h"

struct CHARCATEGORY;
GameObject_s *ActivateCharacter(char *name, nuvec_s *position, i32 angle);
void DeactivateCharacter(char *name);

struct CHARACTER_EFFECT_s {
    i16 character_id;
    i16 debris_id;
    i16 action_id;
    u8 bits_on;
    u8 bits_off;
    f32 frame_1;
    f32 frame_2;
    f32 particle_rate;
    u32 flags;
    u16 locators;
    u8 random;
    u8 particle_count;
    f32 rumble;
    f32 shake;
    f32 judder;
    i16 sound_id;
    u16 surfaces;
    f32 minimum;
    f32 maximum;
};

DECOMP_ASSERT(sizeof(CHARACTER_EFFECT_s) == 0x34, "CHARACTER_EFFECT size");
DECOMP_ASSERT(offsetof(CHARACTER_EFFECT_s, flags) == 0x14, "CHARACTER_EFFECT flags offset");
DECOMP_ASSERT(offsetof(CHARACTER_EFFECT_s, sound_id) == 0x28, "CHARACTER_EFFECT sound offset");

struct CHARCONFIG_s {
    characterdata_s *character;
    GAMECHARACTERDATA_s *runtime;
    GAMECHARACTERLAYER_s *layer_scratch;
    u8 flags;
    u8 named_layers;
    u8 field_0x0e[2];
    u32 field_0x10;
    i32 effect_count;
    CHARACTER_EFFECT_s *effect_scratch;
    i16 character_id;
    u16 animation_count;
    VARIPTR *arena;
    VARIPTR *arena_end;
    char animation_names[100][40];
};

DECOMP_ASSERT(sizeof(CHARCONFIG_s) == 0xfc8, "CHARCONFIG size");
DECOMP_ASSERT(offsetof(CHARCONFIG_s, flags) == 0x0c, "CHARCONFIG flags offset");
DECOMP_ASSERT(offsetof(CHARCONFIG_s, effect_count) == 0x14, "CHARCONFIG effect count offset");
DECOMP_ASSERT(offsetof(CHARCONFIG_s, character_id) == 0x1c, "CHARCONFIG character offset");
DECOMP_ASSERT(offsetof(CHARCONFIG_s, arena) == 0x20, "CHARCONFIG arena offset");
DECOMP_ASSERT(offsetof(CHARCONFIG_s, animation_names) == 0x28, "CHARCONFIG names offset");

extern CHARCONFIG_s charconfig;
extern NUFPCOMJMP ConfigChar_GameKeywords[];
NUFPCOMJMP *CharConfig_GetKeywords();

i32 CharCategory_IsCategory(GameObject_s *object, i32 index);

struct CHARCATEGORY;
// Original table shared across the reconstructed source split; retain its
// original ELF name without adding a synthetic accessor function.
void CharCategories_Init(CHARCATEGORY *categories);
i32 CharCategory_FindByName(char *name);
