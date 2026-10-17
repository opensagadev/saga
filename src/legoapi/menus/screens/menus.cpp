// The original translation unit's functions now live with their address range;
// its static constructor (header VuVec constants) stays here in link order.
#include "batman.h"
#include <string.h>
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numusic/numusic.h"
