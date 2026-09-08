#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
struct MENU_s;

static f32 Arcade_NeedTwoPlayers_Scale = 1.0f;

void Arcade_Kill(i32, i32) {
}

i32 Arcade_GetMode(u32 *flags) {
    if (Arcade == 0) {
        if (flags != NULL) {
            *flags = 0;
        }
        return -1;
    }
    if (flags != NULL) {
        *flags = Arcade_Mode[ArcadeItem.field_c_0xc].field8_0x8;
    }
    return ArcadeItem.field_c_0xc;
}

void Arcade_AIKilled(i32) {
}

void Arcade_DrawPanel(i32) {
}

void Arcade_AwardPoint(i32, i32, i32) {
}

void Arcade_ResetPanel() {
    Arcade_NeedTwoPlayers_Scale = 1.0f;
}

void Arcade_DrawEndMenu(MENU_s *) {
}

void Arcade_UpdatePanel(i32) {
}

void Arcade_PlayerKilled(i32, i32) {
}

void Arcade_CoinCollected(i32, u32 *, u32) {
}

void Arcade_UpdateEndMenu(MENU_s *) {
}

void Arcade_BothPlayersActive() {
}
