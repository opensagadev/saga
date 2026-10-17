#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edui.h"
#include <cstdio>

extern "C" void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
extern "C" eduiitem_s *eduiItemColourPickCreate(usize, const void *, EdUiItemCallback, char *);
extern "C" i32 PS2_REZ_H;
extern "C" void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);

// Editor UI callbacks retained in the original editor's menu code.
#pragma GCC push_options
#pragma GCC optimize("O2", "omit-frame-pointer")





static __used__ void cbColourPickSel(eduimenu_s *menu, eduiitem_s *, u32);
static __used__ void cbToggleIndicatorMode(eduimenu_s *, eduiitem_s *item, u32);
static __used__ void cbCopy(eduimenu_s *, eduiitem_s *, u32);
static __used__ void cbPaste(eduimenu_s *, eduiitem_s *, u32);
static __used__ i32 cbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, float delta_time, nupad_s *pad);

extern "C" void CreateColourPicker(void);

extern "C" void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);

extern "C" {

static __used__ void cbGradChange(void);

static __used__ void cbSel(eduimenu_s *, eduiitem_s *item);

static __used__ void cbSubMenu(eduimenu_s *menu, eduiitem_s *item);

static __used__ void cbgpcfgAdd(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCPPress(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgCopy(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgDel(eduimenu_s *menu, eduiitem_s *item, u32 value);

static __used__ void cbgpcfgPaste(eduimenu_s *menu, eduiitem_s *item, u32 value);

} // extern "C"

#pragma GCC pop_options
