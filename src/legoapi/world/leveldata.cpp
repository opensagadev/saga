
#include <string.h>
#include "legoapi/world/level.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/gizmos/fx/gizmopickups.h"

extern GameObject_s * CutDeadVehiclePlayer;
extern i32 reset_reimport;
extern void GizmoTypeStoreProgress(GIZMOSYS_s *, void *, i32, i32, char *);

void ResetLevel(WORLDINFO_s *world, char *cutscene, i32);

void *SetLevelHack(i32 size);
