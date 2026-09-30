#pragma once

#include "decomp.h"
#include "nu2api/nucore/common.h"

struct MENUPACKET_s {
    i16 player_model[2];
    u8 active_player[2];
    union {
        u8 reserved_0[2];
        u8 customise_other_player[2];
    };
    union {
        u8 reserved_1[2];
        u8 customise_demo_exit[2];
    };
};
DECOMP_ASSERT(sizeof(MENUPACKET_s) == 0x0a, "MENUPACKET_s ABI");

extern MENUPACKET_s MenuPacket;
extern i32 memcard_saveneeded;
extern i32 memcard_loadneeded;
extern f32 BlipL[2], BlipR[2], BlipU[2], BlipD[2];
extern f32 uprepeattime[2], downrepeattime[2], leftrepeattime[2], rightrepeattime[2];
extern u8 uprepeatcount[2], downrepeatcount[2], leftrepeatcount[2], rightrepeatcount[2];
extern "C" void MenuRepeat(i32 *, i32 *, f32 *, u8 *, f32, f32);

struct MENU_s;
void MenuEnterNewGame(MENU_s *menu);
void MenuExitNewGame(MENU_s *menu);
void MenuInitEpisodes(MENU_s *menu);
void MenuUpdateEpisodes(MENU_s *menu);
void MenuDrawEpisodes(MENU_s *menu);

void MakeMenuPacket();

#ifdef __cplusplus
extern "C" {
#endif
    i32 MenuGetSlotNum(void);
    i32 MenuCurrentID(void);
    i32 MenuInMemoryCardLoad(void);
    i32 MenuInMemoryCardWarning(void);
    i32 MenuInMemoryCard(void);
    i32 MenuInCriticalMemoryCard(void);
    void DrawMenu(i32 paused);
    void SetButtonScaleMode(i32 mode);
#ifdef __cplusplus
}
#endif
