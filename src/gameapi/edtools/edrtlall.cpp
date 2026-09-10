#include "gameapi_edtools_types.h"
#include "legoapi/render/core/render.h"

struct nuvtx_tc1_s;
struct numtl_s;
struct numtx_s;

struct EDRTLFOG_s {
    u8 reserved_00[0x8];
    u32 colour;
    u8 reserved_0c[0x8];
    i32 type;
    u8 reserved_18[0x4];
    f32 radius;
    NUVEC position;
    u8 reserved_2c[0x20];
};
DECOMP_ASSERT(sizeof(EDRTLFOG_s) == 0x4c, "EDRTL fog size");

static i32 numsegs = 16;

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
