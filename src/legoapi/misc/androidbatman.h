#pragma once

#include "nu2api/nucore/common.h"

struct nupad_s;

void InitOnce(i32 argc, char **argv);

extern "C" nupad_s **Game_NuPad;

// Declared here for the other units that use them.
extern i32 app_tbaiset;
extern i32 app_tbdrawset;
extern i32 app_tbgameset;
extern i32 app_tbplayerset;
