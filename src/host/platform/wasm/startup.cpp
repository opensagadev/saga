#include "host/harness/startup.hpp"

#include <cstdio>

#include "globals.h"
#include "legoapi/world/level.h"

extern void NewGame();
extern "C" void __real__Z8LoadPermv();
extern "C" void __real__Z7EndPermv();

extern "C" void __wrap__Z8LoadPermv() {
    HostLoadPermImmediately(__real__Z8LoadPermv);
}

extern "C" void __wrap__Z7EndPermv() {
    __real__Z7EndPermv();
    static bool started = false;
    if (started)
        return;
    started = true;

    LEVELDATA_s *cantina = Level_FindByName(const_cast<char *>("Map"), nullptr);
    if (cantina == nullptr || (cantina->flags & LEVEL_GAMEPLAY) == 0) {
        fprintf(stderr, "browser: cantina unavailable; using normal startup\n");
        return;
    }

    // Character and area configuration is now available to NewGame.
    NewGame();
    BackupGame = Game;
    HostEnterLevel(*cantina);
    fprintf(stderr, "browser: entering cantina\n");
}
