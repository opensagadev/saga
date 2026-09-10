#include "gameapi_edtools_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"

struct nuvtx_tc1_s;
struct numtl_s;
struct numtx_s;

typedef rtlfog_s EDRTLFOG_s;

static i32 numsegs = 16;
static i32 curFogLoc = -1;

static EDRTLFOG_s *SelectPrevFog() {
    i32 index;
    i32 count = 0;
    index = curFogLoc - 1;
    if (index < 0) {
        index = 32;
    }
    if (curr_set != NULL) {
        while (index != curFogLoc) {
            if (curr_set->fog[index].type != 0) {
                curFogLoc = index;
                return &curr_set->fog[index];
            }
            if (index == 0) {
                index = 32;
            }
            ++count;
            if (count > 31) {
                break;
            }
            --index;
        }
    }
    return NULL;
}

static EDRTLFOG_s *SelectNextFog() {
    i32 index;
    i32 count = 0;
    if (curFogLoc == -1 || curFogLoc == 31) {
        index = 0;
    } else {
        index = curFogLoc + 1;
    }
    if (curr_set != NULL) {
        while (index != curFogLoc) {
            if (curr_set->fog[index].type != 0) {
                curFogLoc = index;
                return &curr_set->fog[index];
            }
            if (index > 31) {
                index = -1;
            }
            ++count;
            if (count > 31) {
                break;
            }
            ++index;
        }
    }
    return NULL;
}

// RTL editor subsystem stubs (static, internal linkage).

static void edrtlClose() {
}
static void edrtlEnter() {
}
static void edrtlLeave() {
}
static void edrtlRender() {
}
static void edrtlProcFog(float, nupad_s *) {
}
static void edrtlProcRTL(float, nupad_s *) {
}
static void edrtlDrawFogs() {
}

extern "C" void edrtlDrawFog(EDRTLFOG_s *fog) {
    if (fog != NULL) {
        i32 colour = (fog->colour & 0xffffff) | 0x80000000;
        switch (fog->type) {
            default:
                break;
            case 1:
                RndrOSphere(&fog->position, fog->radius, colour, numsegs, 0);
                break;
        }
    }
}
static void edrtlDrawHelp() {
}
static void edrtlProcBurn(float, nupad_s *) {
}
static void edrtlSaveUndo() {
}
static void edrtlDrawCursor() {
}
static void edrtlDrawLights() {
}
static void edrtlRndrLine3d(nuvtx_tc1_s *, numtl_s *, numtx_s *) {
}
static void edrtlBurnSetMenu(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlDrawFogInfo() {
}
static void edrtlDrawRTLInfo() {
}
static void edrtlBurnMainMenu() {
}
static void edrtlDrawBurnInfo() {
}
static void edrtlDrawBurnouts() {
}
static void edrtlSetBurnRadius(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlBurnRadiusMenu(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlInvalidateUndo() {
}
static void edrtlSetBurnFalloff(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlBurnoutFileLoad(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlBurnoutFileSave(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlSetBurnsetFlare(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlBurnDefaultsMenu(eduimenu_s *, eduiitem_s *, u32) {
}
static void edrtlSetBurnsetRadius(eduimenu_s *, eduiitem_s *, u32) {
}
