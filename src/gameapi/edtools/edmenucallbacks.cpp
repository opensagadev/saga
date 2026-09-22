#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"

// Remaining editor UI/menu callbacks for colour picking, shared menu
// configuration, and level-editor settings.

static void cbColourPickSel(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void cbCopy(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void cbPaste(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void cbProcessColourPick(eduimenu_s *, eduiitem_s *, float, nupad_s *) {
    STUBBED();
}
static u32 cbSortSeg(void const *, void const *) {
    STUBBED();
    return 0;
}

extern "C" {

    static void cbGradChange(void) {
        STUBBED();
    }

    static void cbSel(void *, void *) {
        STUBBED();
    }

    static void cbSubMenu(void *, void *) {
        STUBBED();
    }

    static void cbgpcfgAdd(void *, void *, void *) {
        STUBBED();
    }

    static void cbgpcfgCPPress(void *, void *, void *) {
        STUBBED();
    }

    static void cbgpcfgCopy(void *, void *, void *) {
        STUBBED();
    }

    static void cbgpcfgDel(void *, void *, void *) {
        STUBBED();
    }

    static void cbgpcfgPaste(void *, void *, void *) {
        STUBBED();
    }

} // extern "C"
