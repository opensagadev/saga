#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/gizmos/transport/tightropes.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/world/world.h"
#include <math.h>
#include "globals.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/input/gamepads.h"

DECOMP_ASSERT(offsetof(WORLDINFO_s, tightropes) == 0x505c, "World tightrope pointer offset");
DECOMP_ASSERT(offsetof(WORLDINFO_s, tightrope_count) == 0x5060, "World tightrope count offset");

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" f32 AnimDuration(i32, i32, f32, f32, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
void GameObjectOrigin(GameObject_s *);

void StartJump(GameObject_s *, i32);
void StartEndOfJump(GameObject_s *);
i32 StartFallLand(GameObject_s *, i32);

// Original 0x1ccf00, 441 bytes.
