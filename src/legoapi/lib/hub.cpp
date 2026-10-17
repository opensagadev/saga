// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// ==== src/legoapi/core/input/timer.cpp: all remaining content ====
#include "legoapi/core/input/timer.h"

#include "globals.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "legoapi/core/input/qrand.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/areas.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nurand.h"
#include "legoapi/world/world.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/misc/supportall_types.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/items/objects/gameobjects_types.h"
#include "legoapi/misc/androidbatman.h"
#include "nu2api/nu3d/nu3d_includes.h"

// Original bss @0x127a164.
TIMER GlobalTimer;
f32 LastTouchTime;

// ==== src/legoapi/misc/supportall.cpp: declarations ====

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

// The reference defines these with dynamic initialization in this unit.

static i32 TBGAMECOUNT;
static i32 TBDRAWCOUNT;
static i32 TBPLAYERCOUNT;
static i32 TBAICOUNT;

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

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

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40





// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).


// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

// ==== src/legoapi/misc/memory.cpp: all remaining content ====

// ==== functions in original address order ====

// ==== functions in original address order ====

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
    if (!(length > 0)) {
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

