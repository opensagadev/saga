#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include <math.h>
#include <string.h>

struct AIROW_s;
struct nunativegscene_s;

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

