// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/core/input/timer.h"

#include "globals.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/core/config/cheat.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/world/world.h"
#include "legoapi/misc/supportall_types.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/misc/androidbatman.h"

// Original bss @0x127a164.
TIMER GlobalTimer;
f32 LastTouchTime;

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

static i32 TBGAMECOUNT;
static i32 TBDRAWCOUNT;
static i32 TBPLAYERCOUNT;
static i32 TBAICOUNT;

static TIMINGBAR_LABEL GameTB[12];
static TIMINGBAR_LABEL PlayerTB[12];
static TIMINGBAR_LABEL AITB[12];
static TIMINGBAR_LABEL DrawTB[12];

static inline void TBSTART(i32 slot, i32 type) {
    i32 set;
    switch (type) {
        case 2:
            set = app_tbgameset;
            break;
        case 3:
            set = app_tbplayerset;
            break;
        case 4:
            set = app_tbaiset;
            break;
        case 5:
            set = app_tbdrawset;
            break;
        default:
            return;
    }
    _NuTimeBarSlotBegin(set, slot, NULL);
}

static inline void TBEND(i32 slot, i32 type) {
    i32 set;
    switch (type) {
        case 2:
            set = app_tbgameset;
            break;
        case 3:
            set = app_tbplayerset;
            break;
        case 4:
            set = app_tbaiset;
            break;
        case 5:
            set = app_tbdrawset;
            break;
        default:
            return;
    }
    _NuTimeBarSlotEnd(set, slot);
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

void ResetTimer(TIMER *timer, f32 reset_time) {
    timer->last_time_elapsed = reset_time;
    timer->time_elapsed = reset_time;
    timer->time_elapsed_mod_seconds = NuFmod(reset_time, 1.0f);
    timer->update_count = 0;
}

void UpdateTimer(TIMER *timer) {
    timer->last_time_elapsed = timer->time_elapsed;
    timer->time_elapsed += FRAMETIME;
    timer->time_elapsed_mod_seconds += FRAMETIME;

    while (timer->time_elapsed_mod_seconds >= 1.0f) {
        timer->time_elapsed_mod_seconds -= 1.0f;
    }

    if (timer->update_count == -1)
        return;

    timer->update_count++;
}

void TBRESET() {
    TBGAMECOUNT = 0;
    TBDRAWCOUNT = 0;
    TBPLAYERCOUNT = 0;
    TBAICOUNT = 0;
}

void TBOPENFN(char *name, i32 type) {
    TIMINGBAR_LABEL *labels;
    i32 *count;
    switch (type) {
        case 2:
            labels = GameTB;
            count = &TBGAMECOUNT;
            break;
        case 3:
            labels = PlayerTB;
            count = &TBPLAYERCOUNT;
            break;
        case 4:
            labels = AITB;
            count = &TBAICOUNT;
            break;
        case 5:
            labels = DrawTB;
            count = &TBDRAWCOUNT;
            break;
        default:
            return;
    }
    const i32 existing_count = *count;
    for (i32 index = 0; index < existing_count; ++index) {
        if (NuStrCmp(name, labels[index].name) == 0) {
            TBSTART(index, type);
            return;
        }
    }
    if (!(*count < 12)) {
        return;
    }
    char label[256];
    NuStrCpy(label, name);
    i32 length = NuStrLen(label);
    if (__builtin_expect(!(length > 0), 0)) {
        return;
    }
    if (length > 10)
        label[10] = '\0';
    NuStrCpy(labels[*count].name, label);
    labels[*count].type = static_cast<u8>(type);
    TBSTART(*count, type);
    ++*count;
}

void TBCLOSEFN(char *name, i32 type) {
    TIMINGBAR_LABEL *label;
    switch (type) {
        case 2:
            label = GameTB;
            for (i32 index = 0, count = TBGAMECOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 3:
            label = PlayerTB;
            for (i32 index = 0, count = TBPLAYERCOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 4:
            label = AITB;
            for (i32 index = 0, count = TBAICOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 5:
            label = DrawTB;
            for (i32 index = 0, count = TBDRAWCOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
    }
}

