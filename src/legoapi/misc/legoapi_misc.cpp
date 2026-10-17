#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include <string.h>

extern "C" {
    extern i32 CutSceneWaiting;
    extern i32 editor_active;
    extern i32 (*GamePads_IgnoreInputFn)(void);
    extern FadeSystem FadeSys;
}

void CatIToX(char *output, i32 value) {
    while (*output != '\0')
        ++output;
    *IToX(output, value) = '\0';
}

void DoInput(WORLDINFO_s *world);

void CatI64ToX(char *output, i64 value) {
    while (*output != '\0')
        ++output;
    *I64ToX(output, value) = '\0';
}

