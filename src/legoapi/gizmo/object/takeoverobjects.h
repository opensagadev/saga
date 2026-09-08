#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct GameObject_s;

struct TAKEOVEROBJECT_s {
    GameObject_s *object;
    NUVEC last_safe_position;
    i32 heading;
    char script_name[0x10];
    i16 character_id;
    u8 source_creature;
    u8 registered_level;
    u8 current_level;
    u8 hitpoints;
    u8 contact_index;
    u8 reserved_2b;
};

DECOMP_ASSERT(sizeof(TAKEOVEROBJECT_s) == 0x2c, "Takeover record size");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, last_safe_position) == 0x04, "Takeover last safe position offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, heading) == 0x10, "Takeover heading offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, hitpoints) == 0x29, "Takeover hitpoints offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, contact_index) == 0x2a, "Takeover contact index offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, script_name) == 0x14, "Takeover script name offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, character_id) == 0x24, "Takeover character ID offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, source_creature) == 0x26, "Takeover source creature offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, registered_level) == 0x27, "Takeover registered level offset");
DECOMP_ASSERT(offsetof(TAKEOVEROBJECT_s, current_level) == 0x28, "Takeover current level offset");

extern TAKEOVEROBJECT_s takeoverobjects[8];
extern i32 num_takeoverobjects;
void ClearTakeOverObjectSys();
void RegisterTakeOverObject(GameObject_s *object);
