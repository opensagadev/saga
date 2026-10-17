#include "decomp.h"
#include "globals.h"

#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"

#include <stdio.h>

struct AIROW_s;
struct nunativegscene_s;

extern void FinishWeirdoNames(i32);
extern "C" void NuSound3StopRumble(void);
extern "C" i32 NewMode;
extern "C" i32 Paused;
extern "C" i32 memcard_autosavedisabled;
extern "C" i32 memcard_autosaveenabled;

namespace {

} // namespace

void IncreaseScore(u32 *total, u64 amount, i32 apply_multiplier) {
    if (apply_multiplier != 0 && BonusArea == 0) {
        if (Cheats_CheckFlags(4) != 0) {
            amount *= 2;
        }
        if (Cheats_CheckFlags(8) != 0) {
            amount *= 4;
        }
        if (Cheats_CheckFlags(0x10) != 0) {
            amount *= 6;
        }
        if (Cheats_CheckFlags(0x20) != 0) {
            amount *= 8;
        }
        if (Cheats_CheckFlags(0x40) != 0) {
            amount *= 10;
        }
    }
    amount += *total;
    if (amount > 4000000000ULL) {
        amount = 4000000000ULL;
    }
    *total = static_cast<u32>(amount);
}

void RegisterHelpers() {
}

