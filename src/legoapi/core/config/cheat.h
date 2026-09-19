#pragma once

#include "decomp.h"

struct GameObject_s;
struct nuvec_s;

struct CHEAT {
    char *name;
    i16 *text_id;
    byte enabled;
    undefined field_0x09;
    undefined field_0x0a;
    u8 area;
    i32 field_0x0c;
    char *code;
    i32 extra_price;
    char *extra_name;
    u32 flag;
};
DECOMP_ASSERT(sizeof(CHEAT) == 0x20, "CHEAT size");
DECOMP_ASSERT(offsetof(CHEAT, enabled) == 0x08, "CHEAT enabled offset");
DECOMP_ASSERT(offsetof(CHEAT, extra_price) == 0x14, "CHEAT extra price offset");
DECOMP_ASSERT(offsetof(CHEAT, extra_name) == 0x18, "CHEAT extra name offset");
DECOMP_ASSERT(offsetof(CHEAT, flag) == 0x1c, "CHEAT flags offset");

#ifdef __cplusplus

void Cheat_SetArea(i32 cheat, i32 areaId);
void Cheats_Init(CHEAT *cheats);
i32 Cheat_FindByName(char *name);
void Cheats_SetFlags(void);
u32 Cheats_CheckFlags(u32 flag);
u32 Cheat_CheckFlags(i32 cheat_index, u32 flag_mask);
void Cheat_SetOn(i32 cheat, i32 enabled, i32 update_save);
i32 Cheat_IsOn(i32 cheat);
void Cheat_GetOnOffBitfield(i32 *onoffs, i32 count);
void Cheat_SetOnOffBitfield(i32 *onoffs, i32 count);
u32 Cheat_MultiplyScore(u32 score);
void Cheats_TurnOff(i32 cheat);
void Cheat_StartPowerUp(nuvec_s *position, GameObject_s *object);
i32 Cheat_PowerUpActive(i32 index);
void Cheats_Reset(void);
void Cheats_Update(void);
extern i32 ONEPLAYERPOWERUPS;
extern f32 CHEAT_POWERUPTIME;
extern i32 POWERUP_TEXTID;

#endif
