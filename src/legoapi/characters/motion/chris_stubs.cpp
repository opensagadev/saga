#include "decomp.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/nutex.h"
#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

GameObject_s *volatile AnakinC = NULL;

void DrawSpaceLevel(spacelevel_s *) __asm__("_ZL14DrawSpaceLevelP12spacelevel_s") __attribute__((visibility("hidden")));
void ProcessSpaceLevel(spacelevel_s *) __asm__("_ZL17ProcessSpaceLevelP12spacelevel_s")
    __attribute__((visibility("hidden")));

void ChrisAnakinADraw() {
    DrawSpaceLevel(WORLD->space_level);
}

void ChrisAnakinDDraw() {
    DrawSpaceLevel(WORLD->space_level);
}

void ChrisAnakinAUpdate(WORLDINFO_s *) {
    ProcessSpaceLevel(WORLD->space_level);
}

void ChrisUnallocLevelStuff(WORLDINFO_s *world) {
    world->level_specific_data = NULL;
    if (AnakinC != NULL) {
        return;
    }
    AnakinC = NULL;
}
