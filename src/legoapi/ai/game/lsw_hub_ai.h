#ifndef LEGOAPI_AI_GAME_LSW_HUB_AI_H
#define LEGOAPI_AI_GAME_LSW_HUB_AI_H

#include "decomp.h"

// Pointer/isize pun shared by Condition_InHubArea (lsw_hub_ai.cpp) and
// Condition_InHubAreaInit (lsw_hub_ai_stubs.cpp). Defined once here so the
// duplicate-definition lint stays green.
union AI_HUB_AREA_ARGUMENT {
    void *pointer;
    isize value;
};

#endif

// Declared here for the other units that use them.
struct AIPACKET_s;
struct AISCRIPTPROCESS_s;
struct AISCRIPT_s;
struct AISYS_s;
struct WORLDINFO_s;
extern i32 Hub_GetRandomCharType();
extern void LSW_Hub_InitAI(WORLDINFO_s *);
extern void LSW_Hub_ResetAI(WORLDINFO_s *);
extern void LSW_Hub_UpdateAI(WORLDINFO_s *);
f32 Condition_InHubArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);
i32 Hub_CurrentArea();
i32 Hub_Outside(void);
void *Condition_InHubAreaInit(AISYS_s *, char *, AISCRIPT_s *);
