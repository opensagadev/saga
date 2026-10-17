#include "decomp.h"
#include "globals.h"

#include <string.h>

#include "gameapi/gui/apimenu.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"

#include <stdio.h>
#include "legoapi/items/objects/things.h"

struct AIROW_s;
struct nunativegscene_s;

namespace {

} // namespace

void IncreaseScore(u32 *total, u64 amount, i32 apply_multiplier);

