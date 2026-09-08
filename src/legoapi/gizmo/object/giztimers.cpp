#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

GIZMO_s *createGizTimer(void *, float time, i32 random_time, char *name) {
    WORLDINFO *world = WorldInfo_CurrentlyLoading();
    if (world == NULL || world->giz_timers == NULL ||
        world->giz_timers_count == world->current_level->max_giz_timers) {
        return NULL;
    }
    GIZTIMER *timer = &world->giz_timers[world->giz_timers_count];
    timer->start_time = time;
    timer->flags = (timer->flags & 0xfd) | ((random_time & 1) << 1);
    NuStrNCpy(timer->name, name, 16);
    ++world->giz_timers_count;
    return AddGizmo(world->gizmo_sys, giztimer_gizmotype_id, NULL, timer);
}
