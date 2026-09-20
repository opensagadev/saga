#pragma once

#include "nu2api/nucore/common.h"

struct nupad_s;

void InitOnce(i32 argc, char **argv);

extern "C" nupad_s **Game_NuPad;
