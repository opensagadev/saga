#include "decomp.h"

struct edui_interact_s;
struct eduimenu_s;
struct eduiitem_s;

// Editor-UI callback/helper stubs (C, unmangled). These are static callback
// functions for the edui menu system (item interact/process/render handlers,
// directory-list callbacks, and the graph-line renderer). They are stubbed
// because the editor menu code is not being decompiled.
//
// All are declared static so they reproduce the original binary's local `t`
// symbols exactly; __used__ keeps them emitted and lint-clean.

static __used__ void eduiRenderGraphLine(void) {
    STUBBED();
}

static __used__ void eduicbDestroyDirectoryList(void) {
    STUBBED();
}
static __used__ void eduicbInteractColourPick(void) {
    STUBBED();
}
static __used__ void eduicbInteractExpander(void) {
    STUBBED();
}
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact) {
    STUBBED();
    return 0;
}
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact) {
    return eduicbInteractProp(interact);
}
static __used__ void eduicbInteractSel(void) {
    STUBBED();
}
static __used__ void eduicbItemDestroyExpander(void) {
    STUBBED();
}
static __used__ void eduicbItemDestroyFilter(void) {
    STUBBED();
}
static __used__ void eduicbItemFilePickDestroy(void) {
    STUBBED();
}
static __used__ void eduicbItemGradPickDestroy(void) {
    STUBBED();
}
static __used__ void eduicbItemSliderDestroy(void) {
    STUBBED();
}
static __used__ void eduicbItemTextPickDestroy(void) {
    STUBBED();
}
static __used__ void eduicbPickCFGCancel(void) {
    STUBBED();
}
static __used__ void eduicbProcessColourPick(void) {
    STUBBED();
}
static __used__ void eduicbProcessColourSlider(void) {
    STUBBED();
}
static __used__ void eduicbProcessExpander(void) {
    STUBBED();
}
static __used__ void eduicbProcessFilePick(void) {
    STUBBED();
}
static __used__ void eduicbProcessFilter(void) {
    STUBBED();
}
static __used__ void eduicbProcessGradPick(void) {
    STUBBED();
}
static __used__ void eduicbProcessGraph(void) {
    STUBBED();
}
static __used__ void eduicbProcessGreyPick(void) {
    STUBBED();
}
static __used__ void eduicbProcessProp(void) {
    STUBBED();
}
static __used__ void eduicbProcessSel(void) {
    STUBBED();
}
static __used__ void eduicbProcessSeparator(void) {
    STUBBED();
}
static __used__ void eduicbProcessSlider(void) {
    STUBBED();
}
static __used__ void eduicbProcessTextPick(void) {
    STUBBED();
}
static __used__ void eduicbProcessTexturePick(void) {
    STUBBED();
}
static __used__ void eduicbRenderCheck(void) {
    STUBBED();
}
static __used__ void eduicbRenderColourPick(void) {
    STUBBED();
}
static __used__ void eduicbRenderColourSlider(void) {
    STUBBED();
}
static __used__ void eduicbRenderExpander(void) {
    STUBBED();
}
static __used__ void eduicbRenderFilePick(void) {
    STUBBED();
}
static __used__ i32 eduicbRenderGradPick(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale) {
    STUBBED();
    return 0;
}
static __used__ void eduicbRenderGraph(void) {
    STUBBED();
}
static __used__ void eduicbRenderGreyPick(void) {
    STUBBED();
}
static __used__ void eduicbRenderNumber(void) {
    STUBBED();
}
static __used__ i32 eduicbRenderProp(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale) {
    STUBBED();
    return 0;
}
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale) {
    return eduicbRenderProp(menu, item, x, y, scale);
}
static __used__ void eduicbRenderSel(void) {
    STUBBED();
}
static __used__ void eduicbRenderSelWithClipColour(void) {
    STUBBED();
}
static __used__ void eduicbRenderSeparator(void) {
    STUBBED();
}
static __used__ void eduicbRenderSlider(void) {
    STUBBED();
}
static __used__ void eduicbRenderSliderInt(void) {
    STUBBED();
}
static __used__ void eduicbRenderTextPick(void) {
    STUBBED();
}
static __used__ void eduicbRenderTextSelector(void) {
    STUBBED();
}
static __used__ void eduicbRenderTexturePick(void) {
    STUBBED();
}
static __used__ void eduicbSelectDirectoryEntry(void) {
    STUBBED();
}
static __used__ void eduicbSelectDirectoryExit(void) {
    STUBBED();
}
