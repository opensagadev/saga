#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/render/core/render.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"

f32 CreditsAlpha;
f32 CreditsTime;
f32 CreditsFinishedTime;
i32 CreditsFlag;
static f32 Credits_Duration = 120.0f;

void Credits_Init(WORLDINFO_s *) {
    CreditsAlpha = 1.0f;
    CreditsTime = 0.0f;
    CreditsFinishedTime = 0.0f;
    CreditsFlag = 0;
    if (LastLData != STATUS_LDATA) {
        BackDrop_ResetColours();
    }
    MechSystems::Get()->HookUpClickToPressStart();
}

void Credits_Load(WORLDINFO_s *, variptr_u *, variptr_u *) {
    STUBBED();
}

void Credits_GetInfo(float *duration, i32 *flag, float *alpha) {
    if (duration != NULL) {
        *duration = Credits_Duration;
    }
    if (flag != NULL) {
        *flag = CreditsFlag;
    }
    if (alpha != NULL) {
        *alpha = CreditsAlpha;
    }
}

void Credits_DrawPanel(WORLDINFO_s *) {
    STUBBED();
}

void Credits_UpdateMenu(MENU_s *) {
    STUBBED();
}
