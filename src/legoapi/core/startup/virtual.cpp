#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numath/nutrig.h"

extern "C" i32 NuIOS_IsSmallScreen();
i32 GetMenuID();
void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);












