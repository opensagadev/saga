// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nucore/bgproc.h"

#include <stdarg.h>
#include <string.h>

#include "nu2api/nucore/android/NuThread_android.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/nulst.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/core/config/cheat.h"
#include <stdio.h>
#include <limits.h>
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/world/world.h"
#include "legoapi/items/collect/torpedo.h"
#include "gameapi/edtools/edui.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nucore/nuhgobj.h"

NuThreadBase *g_bgProcThread;
i32 bgproc_frozen;
i32 bgproc_freeze;

static NULSTHDR *procinfo_pool;
static i32 g_bgCritSec;

i32 multithreaded = 1;

i32 g_performingBgProcWorkCritSec = -1;

char *g_CrashDumpId = "CRASHDUMP_ID=LEGOSAGAANDROID_ANDROID_Feb_17_2014_01_01_01$";

static char dummyBuf[0x80];
static BGPROCINFO *cur_pi;

static NuThreadSemaphore events[2] = {
    NuThreadSemaphore(1),
    NuThreadSemaphore(1),
};

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

static void bgThreadMain(void *data) {
    NuStrNCpy(dummyBuf, g_CrashDumpId, 0x20);

    events[0].Wait();

    NuThreadCriticalSectionBegin(g_bgCritSec);
    cur_pi = (BGPROCINFO *)NuLstGetNext(procinfo_pool, NULL);
    NuThreadCriticalSectionEnd(g_bgCritSec);

    while (true) {
        cur_pi->work_started = true;

        NuThreadCriticalSectionBegin(g_performingBgProcWorkCritSec);

        if (cur_pi->do_fn != NULL) {
            (*cur_pi->do_fn)(cur_pi);
        }

        if (cur_pi->ack_fn != NULL) {
            (*cur_pi->ack_fn)(cur_pi);
        }

        NuThreadCriticalSectionEnd(g_performingBgProcWorkCritSec);

        NuThreadCriticalSectionBegin(g_bgCritSec);
        NuLstFree((NULNKHDR *)cur_pi);

        while (true) {
            cur_pi = (BGPROCINFO *)NuLstGetNext(procinfo_pool, NULL);

            NuThreadCriticalSectionEnd(g_bgCritSec);

            if (cur_pi != NULL) {
                break;
            }

            events[0].Wait();

            NuThreadCriticalSectionBegin(g_bgCritSec);
        }
    }
}

BGPROCINFO *bgGetProcActive(void) {
    if (cur_pi != NULL) {
        return cur_pi;
    }

    return (BGPROCINFO *)NuLstGetNext(procinfo_pool, NULL);
}

void bgSuspendMain(i32) {
}

BGPROCINFO *bgPostRequestV(bgprocdofn *do_fn, bgprocackfn *ack_fn, i32 first_type, ...) {
    NuThreadCriticalSectionBegin(g_bgCritSec);

    BGPROCINFO *info = reinterpret_cast<BGPROCINFO *>(NuLstAllocTail(procinfo_pool));
    if (info != NULL) {
        info->do_fn = do_fn;
        info->ack_fn = ack_fn;
        info->work_started = false;
        info->unknown_flag_2 = false;
        info->var_count = 0;
        info->vars = reinterpret_cast<BGVAR *>(info->data);

        BGVAR *var = info->vars;
        char *string_end = reinterpret_cast<char *>(&info->vars);
        BGPROCARGTYPE type = static_cast<BGPROCARGTYPE>(first_type);
        va_list args;
        va_start(args, first_type);

        while (type != BGPROC_ARG_END) {
            var->type = type;
            switch (type) {
                case BGPROC_ARG_I32:
                    var->value.i32_value = va_arg(args, i32);
                    break;
                case BGPROC_ARG_FLOAT:
                    // C varargs promote float to double.
                    var->value.float_value = static_cast<f32>(va_arg(args, f64));
                    break;
                case BGPROC_ARG_STRING: {
                    char *string = va_arg(args, char *);
                    string_end -= NuStrLen(string) + 1;
                    var->value.string_value = string_end;
                    NuStrCpy(string_end, string);
                    break;
                }
                case BGPROC_ARG_U32:
                    var->value.u32_value = va_arg(args, u32);
                    break;
                case BGPROC_ARG_POINTER:
                    var->value.pointer_value = va_arg(args, void *);
                    break;
                case BGPROC_ARG_BOOL:
                    var->value.i32_value = va_arg(args, i32);
                    break;
                case BGPROC_ARG_NONE:
                case BGPROC_ARG_END:
                    break;
            }

            ++info->var_count;
            ++var;
            type = static_cast<BGPROCARGTYPE>(va_arg(args, i32));
        }
        va_end(args);
    }

    NuThreadCriticalSectionEnd(g_bgCritSec);
    events[0].Signal();
    return info;
}

BGPROCINFO *bgPostRequest(bgprocdofn *do_fn, bgprocackfn *ack_fn, void *data, i32 data_size) {
    BGPROCINFO local __attribute__((aligned(16)));
    BGPROCINFO *info;

    if (!multithreaded) {
        local.work_started = false;
        local.unknown_flag_2 = false;

        local.do_fn = do_fn;
        local.ack_fn = ack_fn;

        local.vars = NULL;
        local.var_count = 0;

        if (data_size != 0 && data != NULL) {
            memcpy(local.data, data, data_size);
        }

        if (do_fn != NULL) {
            (*do_fn)(&local);
        }

        if (ack_fn != NULL) {
            (*ack_fn)(&local);
        }

        // ORIG_BUG: This is returned uninitialized.
        return info;
    }

    NuThreadCriticalSectionBegin(g_bgCritSec);

    info = (BGPROCINFO *)NuLstAllocTail(procinfo_pool);
    if (info != NULL) {
        info->work_started = false;
        info->unknown_flag_2 = false;

        info->ack_fn = ack_fn;
        info->do_fn = do_fn;

        info->vars = NULL;
        info->var_count = 0;

        if (data_size != 0 && data != NULL) {
            memcpy(info->data, data, data_size);
        }
    }

    NuThreadCriticalSectionEnd(g_bgCritSec);

    events[0].Signal();

    return info;
}

void bgProcInit() {
    static i32 init;

    if (init == 0) {
        init = 1;

        procinfo_pool = NuLstCreate(0x10, sizeof(BGPROCINFO));

        g_bgCritSec = NuThreadCreateCriticalSection();
        g_performingBgProcWorkCritSec = NuThreadCreateCriticalSection();

        g_bgProcThread = NuCore::m_threadManager->CreateThread(
            bgThreadMain, NULL, 0, "bgProc", 0x100000, NUTHREADCAFECORE_UNKNOWN_1, NUTHREADXBOX360CORE_UNKNOWN_1);
    }
}

void bgProcAbortAll() {
}

void bgprocFreeze() {
}

void bgprocUnFreeze() {
}

i32 bgprocIsFrozen() {
    return bgproc_frozen;
}

i32 bgprocIsFreezing() {
    i32 freezing = 1;
    if (bgproc_frozen == 0)
        freezing = bgproc_freeze != 0;
    return freezing;
}

i32 bgProcIsBgThread(void) {
    return NuCore::m_threadManager->GetCurrentThread() == g_bgProcThread;
}

