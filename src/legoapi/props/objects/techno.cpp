#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "legoapi/gizmos/object/technos.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
void *Technos_FindTgt(TECHNO_s *techno);

i32 Techno_isReady(TECHNO_s *techno) {
    if (techno == NULL) {
        return 0;
    }

    if (techno->controlled_object == NULL) {
        Technos_FindTgt(techno);
    }

    switch (techno->target_mode) {
        case 1: {
            GameObject_s *object = static_cast<GameObject_s *>(techno->controlled_object);
            return object != NULL && (object->apiobj.field_0x1f8 & 0x1001) == 0x1001;
        }
        case 2:
            return NuSpecialGetVisibilityFn(techno->controlled_object) != 0;
        case 3:
            return techno->controlled_object != NULL;
        default:
            return 0;
    }
}

NUVEC *Technos_TgtPos(TECHNO_s *techno) {
    if (techno == NULL) {
        return NULL;
    }

    if (techno->controlled_object == NULL) {
        Technos_FindTgt(techno);
    }

    switch (techno->target_mode) {
        case 1:
            return &static_cast<GameObject_s *>(techno->controlled_object)->apiobj.collision_position;
        case 2:
            return NuSpecialGetPos(techno->controlled_object);
        case 3:
            return GizmoGetPos(WORLD->gizmo_sys, static_cast<GIZMO *>(techno->controlled_object));
        default:
            return NULL;
    }
}

void Techno_MoveCode(WORLDINFO_s *, GameObject_s *) {
}

void *Technos_FindTgt(TECHNO_s *techno) {
    if (techno == NULL || techno->controlled_object != NULL) {
        return techno != NULL ? techno->controlled_object : NULL;
    }

    switch (techno->target_mode) {
        case 0: {
            void *target = GetNamedGameObject(WORLD->ai_sys, techno->target_name);
            if (target != NULL) {
                techno->controlled_object = target;
                techno->target_mode = 1;
                break;
            }
            target = GizmoFindByName(WORLD->gizmo_sys, -1, techno->target_name);
            if (target != NULL) {
                techno->controlled_object = target;
                techno->target_mode = 3;
                break;
            }
            if (NuSpecialFind(WORLD->current_gscn,
                              reinterpret_cast<nuhspecial_s *>(techno->target_special_storage),
                              techno->target_name, 0) != 0) {
                techno->target_mode = 2;
                techno->controlled_object = techno->target_special_storage;
                break;
            }
            techno->target_mode = 0;
            techno->controlled_object = NULL;
            break;
        }
        case 1:
            techno->controlled_object = GetNamedGameObject(WORLD->ai_sys, techno->target_name);
            if (techno->controlled_object == NULL) {
                techno->target_mode = 0;
            }
            break;
        case 2:
            techno->controlled_object = techno->target_special_storage;
            if (NuSpecialFind(WORLD->current_gscn,
                              reinterpret_cast<nuhspecial_s *>(techno->target_special_storage),
                              techno->target_name, 0) == 0) {
                techno->target_mode = 0;
                techno->controlled_object = NULL;
            }
            break;
        case 3:
            techno->controlled_object = GizmoFindByName(WORLD->gizmo_sys, -1, techno->target_name);
            if (techno->controlled_object == NULL) {
                techno->target_mode = 0;
            }
            break;
        default:
            techno->target_mode = 0;
            break;
    }
    return techno->controlled_object;
}

void Techno_FindNearest(WORLDINFO_s *, nuvec_s *, GameObject_s *, float *) {
}

void Technos_MoveTarget(TECHNO_s *, GameObject_s *) {
}

void GizTechno_CanUseTechno(GameObject_s *, TECHNO_s *) {
}
