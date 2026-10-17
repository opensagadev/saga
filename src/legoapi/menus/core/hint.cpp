#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/panel.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/characters/core/players.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/core/input/gamepads.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 only_process_this_hint_id = -1;
f32 ICONXPOS_VIRTUALS = -0.61500001f;
f32 ICONYPOS_VIRTUALS = 0.47f;
f32 ICONYPOS_TOUCHYFEELY = -0.699999988f;
f32 AlphaBlendTime = 1.0f;

// The hint icon transitions and the touch icon x position are dynamically
// initialized in this unit's static constructor in the reference.
HintVectorTransition hintIconPos;
HintScalarTransition hintIconScale;
HintScalarTransition hintYPop;
f32 ICONXPOS_TOUCHYFEELY = -ICONX;
static __used__ i32 Attack_UpdateHint(HINT_s *);

extern u8 show_unlock_shop_hint;
extern u8 show_unlock_customiser_hint;
extern u8 show_unlock_minikitviewer_hint;

i32 HINT_COMPLETE(i32 hint_id);

i32 Tilt_UpdateHint(HINT_s *) {
    return 0;
}

f32 CurrentHintAlpha();

// Retail keeps this body as GCC's split-out clone; the inline head stays in
// the caller.
static __attribute__((noinline, noclone)) void DrawHint_LSWBody(HINT_s *hint, i32 max_lines)
    __asm__("_Z12DrawHint_LSWP6HINT_si.part.19");

static void DrawHint_LSWBody(HINT_s *hint, i32 max_lines);

void DrawHint_LSW(HINT_s *hint, i32 max_lines);

i32 Dodge_UpdateHint(HINT_s *hint);

void SET_HINT_COMPLETE(i32 hint_id);

void CLEAR_HINT_COMPLETE(i32 hint_id);

i32 DragBomb_UpdateHint(HINT_s *hint);

// Original local callbacks referenced by Hints_LSW.
extern u8 show_lever_hint;
extern i32 show_autojump_hint;
extern AREADATA_s *PODRACE_ADATA, *PODSPRINT_ADATA, *BONUS_GUNSHIP_ADATA;
extern FadeSystem FadeSys;

static __used__ i32 Jump_UpdateHint(HINT_s *hint);

static __used__ i32 Move_UpdateHint(HINT_s *hint);

static __used__ i32 Lever_UpdateHint(HINT_s *);

static __used__ i32 AutoJump_UpdateHint(HINT_s *hint);

static __used__ i32 VehicleStuff_UpdateHint(HINT_s *hint);

static __used__ i32 HoldTag_UpdateHint(HINT_s *);

extern u8 show_hatmachine_hint;

static __used__ i32 HatMachine_UpdateHint(HINT_s *hint);

extern HINTUIBUTTON_s *hintUIButton;

void *GetHintFromUIButton() {
    return hintUIButton != NULL ? hintUIButton->field_0x78 : NULL;
}

extern HINT_s Hints_LSW[55];
void RegisterWithHintSys(void (*)(HINT_s *, i32), HINT_s *, u32 *, i32);
extern i32 id_HINT_LSW_AUTOJUMP;
extern i32 id_HINT_LSW_AUTOJUMP_FAIL;

void initGameHintSys_LSW();

i32 SmartBomb_UpdateHint(HINT_s *);

extern AREADATA *JABBASPALACE_ADATA;

i32 ShinyMetal_UpdateHint(HINT_s *hint);

f32 CurrentHintButtonScale();

void initGameHintSys_Batman();

i32 IndyUnlocked_UpdateHint(HINT_s *hint);

static __used__ i32 Sith_UpdateHint(HINT_s *);

static __used__ void PlayerButton_PlayHint(HINT_s *);

static __used__ i32 UnlockHubStuff_UpdateHint(HINT_s *hint);

static __used__ i32 PlayerButton_UpdateHint(HINT_s *hint);

i32 GizForce_UpdateHint(HINT_s *);
i32 ZipUps_UpdateHint(HINT_s *);
i32 Push_UpdateHints(HINT_s *);
i32 GizPanel_UpdateHint(HINT_s *);
i32 Shop_UpdateHint(HINT_s *);
extern HINT_s Hints_LSW[55];

// Each control mode retains its own requested and interpolated hint row.

void MenuUpdateHints(MENU_s *menu);

void MenuDrawHints(MENU_s *menu);
