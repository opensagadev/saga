#pragma once

#include "decomp.h"

struct GameObject_s;

struct TAKEOVEROBJECT_s {
    GameObject_s *object;
    u8 reserved_04[0x10];
    char script_name[0x10];
    u16 character_id;
    u8 source_creature;
    u8 registered_level;
    u8 current_level;
    u8 reserved_29[3];
};

DECOMP_ASSERT(sizeof(TAKEOVEROBJECT_s) == 0x2c, "Takeover record size");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, script_name) == 0x14, "Takeover script name offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, character_id) == 0x24, "Takeover character ID offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, source_creature) == 0x26, "Takeover source creature offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, registered_level) == 0x27, "Takeover registered level offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, current_level) == 0x28, "Takeover current level offset");

extern TAKEOVEROBJECT_s takeoverobjects[8];
extern i32 num_takeoverobjects;
void ClearTakeOverObjectSys();
void RegisterTakeOverObject(GameObject_s *object);
