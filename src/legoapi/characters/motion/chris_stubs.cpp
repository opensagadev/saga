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

void DrawSpaceLevel(spacelevel_s *) __asm__("_ZL14DrawSpaceLevelP12spacelevel_s")
    __attribute__((visibility("hidden"), regparm(1)));
void ProcessSpaceLevel(spacelevel_s *) __asm__("_ZL17ProcessSpaceLevelP12spacelevel_s")
    __attribute__((visibility("hidden"), regparm(1)));

__attribute__((optimize("O3,omit-frame-pointer"))) void ChrisAnakinADraw() {
    DrawSpaceLevel(WORLD->space_level);
}

__attribute__((optimize("O3,omit-frame-pointer"))) void ChrisAnakinDDraw() {
    DrawSpaceLevel(WORLD->space_level);
}

__attribute__((optimize("O3,omit-frame-pointer"))) void ChrisAnakinAUpdate(WORLDINFO_s *) {
    ProcessSpaceLevel(WORLD->space_level);
}

__attribute__((optimize("O3,omit-frame-pointer"), aligned(16))) void ChrisUnallocLevelStuff(WORLDINFO_s *world) {
    world->level_specific_data = NULL;
    if (AnakinC != NULL) {
        return;
    }
    asm volatile(".p2align 3");
    AnakinC = NULL;
}
