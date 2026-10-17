#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct AIPACKET_s;
struct APIOBJECT_s;
struct GameObject_s;
extern f32 jump_stuck_time;
extern u32 StarWars_ParseAIPathCnxFlag(char *name);
i32 StarWars_PrepareJump(AIPACKET_s *, APIOBJECT_s *, i32);
i32 StarWars_PrepareR2D2Glide(AIPACKET_s *, APIOBJECT_s *, i32);
void StarWars_AutoSetAICapabilities(GameObject_s *object);
void StarWars_GameAISysInit();
