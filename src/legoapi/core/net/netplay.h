#pragma once

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

// Shared pod-race client state. Keep the 64-bit API views and the existing
// two-word level logic in the same storage, with natural host alignment.
struct CLIENTMINES_s {
    NUVEC positions[64];
    union {
        u64 present_mask;
        u32 present_words[2];
    };
    union {
        u64 exploded_mask;
        u32 exploded_words[2];
    };
    u32 field_310;
};

DECOMP_ASSERT(sizeof(CLIENTMINES_s) == 0x314, "client mine state size");
DECOMP_ASSERT(offsetof(CLIENTMINES_s, present_mask) == 0x300, "client mine presence offset");
DECOMP_ASSERT(offsetof(CLIENTMINES_s, exploded_mask) == 0x308, "client mine explosion offset");
DECOMP_ASSERT(offsetof(CLIENTMINES_s, field_310) == 0x310, "client mine tail offset");

extern CLIENTMINES_s client_mines;
extern APICHARACTERMODELLIST_s NetFreePlayModelList[49];
extern i8 net_FreePlayModelCount;
extern u8 net_recievedFreePlayList;
void GetClientMineInfo(nuvec_s **positions, u64 **present, u64 **exploded);
