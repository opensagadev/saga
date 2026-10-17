#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MechTouchUIElement;
struct TouchHolder;
void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &, TouchHolder &);
