#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edstubs.h"

// Bridge editor subsystem stubs (static, internal linkage).

static void edbricbFileLoad(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbFileSave(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
i32 edbriLoadPage(char *path, void *gscn) {
    STUBBED();
    (void)path;
    (void)gscn;
    return -1;
}
static void edbricbSetDpadMode(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbCancelOptMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
static void edbricbSetBridgeDamp(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbSetPlankCount(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbSetRopeColour(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbDpadModeMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbPlankCountMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbRopeColourMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}
static void edbricbSetPostInterval(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

// Remaining bridge-editor UI/menu callbacks.

static void edbricbPostInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbSetBridgeGravity(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbSetBridgeTension(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbPlankInstanceMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbCancelDpadModeMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edbricbSetBridgePlrweight(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbSetBridgeStability(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbSetBridgeRopeheight(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbSetPostInstanceType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbBridgePropertiesMenu(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbCancelPlankCountMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edbricbCancelRopeColourMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edbricbSetPlankInstanceType(eduimenu_s *, eduiitem_s *, u32) {
    STUBBED();
}

static void edbricbCancelPostInstanceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edbricbCancelPlankInstanceMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}

static void edbricbCancelBridgePropertiesMenu(eduimenu_s *, eduimenu_s *) {
    STUBBED();
}
