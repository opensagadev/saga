#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct JumpTriggerPacket;
struct MechAutoJumpConnection;
MechAutoJumpConnection *MechAutoJumpGetBest(JumpTriggerPacket const &, i32);
void MechAutoJumpSetIsUsing(GameObject_s &, MechAutoJumpConnection &);
