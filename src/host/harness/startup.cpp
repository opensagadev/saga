#include "host/harness/startup.hpp"

#include "globals.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/players.h"

void HostLoadPermImmediately(void (*load_perm)()) {
    // Keep permanent asset setup while skipping the startup screen loop.
    const i32 previous = LOADEROFF;
    LOADEROFF = 1;
    load_perm();
    LOADEROFF = previous;
}

void HostEnterLevel(const LEVELDATA_s &level) {
    GAMEDEMO = 0;
    Level = level.idx;
    NewMode = 0;
    PlayerProgress[0].active = 1;
    PlayerProgress[1].active = 0;
    MenuReset();
}
