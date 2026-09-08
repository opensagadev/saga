#include "decomp.h"
#include "globals.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void randyfloat() {
}

GIZMO *createGizRandom(void *, i32 output_count, i32 *output_weights, char *name) {
    WORLDINFO *world = WorldInfo_CurrentlyLoading();
    if (world == NULL || world->giz_randoms->count == world->current_level->max_giz_randoms) {
        return NULL;
    }

    GIZRANDOM *random = &world->giz_randoms->randoms[world->giz_randoms->count];
    random->output_count = output_count;
    for (i32 index = 0; index < output_count; ++index) {
        random->output_weights[index] = output_weights[index];
    }
    NuStrNCpy(random->name, name, sizeof(random->name));
    ++world->giz_randoms->count;
    return AddGizmo(world->gizmo_sys, gizrandom_gizmotype_id, NULL, random);
}

i32 RandomIDFromFlags(u32, u32, i32, APICHARACTERMODELLIST_s *, i32) {
    return -1;
}
