#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/object/technos.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Techno_isReady(TECHNO_s *) {
}

NUVEC *Technos_TgtPos(TECHNO_s *) {
    return NULL;
}

void Techno_MoveCode(WORLDINFO_s *, GameObject_s *) {
}

void Technos_FindTgt(TECHNO_s *) {
}

void Techno_FindNearest(WORLDINFO_s *, nuvec_s *, GameObject_s *, float *) {
}

void Technos_MoveTarget(TECHNO_s *, GameObject_s *) {
}

void GizTechno_CanUseTechno(GameObject_s *, TECHNO_s *) {
}

TECHNO *Technos_FindControllingTechno(GameObject_s *object) {
    if (object != NULL) {
        for (i32 i = 0; i < WORLD->ntechnos; ++i) {
            TECHNO *techno = &WORLD->technos[i];
            if (techno->target_mode == 1 && techno->controlled_object == object)
                return techno;
        }
    }
    return NULL;
}
