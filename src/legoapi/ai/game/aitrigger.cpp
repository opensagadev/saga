#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/world/world.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void AITriggerSetSysReset(AITRIGGERSETSYS_s *system) {
    if (system == NULL) {
        return;
    }

    memset(system, 0, sizeof(*system));
    for (i32 i = 0; i < 64; ++i) {
        system->field_0x4280[i] = -1;
        system->field_0x42c0[i] = -1;
    }
    for (i32 i = 0; i < 32; ++i) {
        for (i32 j = 0; j < 8; ++j) {
            system->sets[i].trigger_indices[j] = -1;
        }
    }
}

AITRIGGERSET_s *AITriggerSetCreate(AITRIGGERSETSYS_s *system, FLOWBOX_s *box) {
    if (system != NULL) {
        for (i32 index = 0; index < 32; ++index) {
            AITRIGGERSET_s *set = &system->sets[index];
            if (!(set->flags & 1)) {
                set->flags |= 1;
                set->flowbox = box;
                return set;
            }
        }
    }
    return NULL;
}

void AITriggerSetAddTrigger(AISYS_s *, AITRIGGERSET_s *, GIZMO_s *);

void AITriggerSysAutoSetUp(WORLDINFO_s *world, AITRIGGERSETSYS_s *system) {
    AITRIGGERSET_s *groups[32] = {};
    if (world != NULL && world->giz_flow != NULL) {
        GIZFLOW_s *flow = world->giz_flow;
        FLOWBOX_s *box = flow->flowboxes;
        for (i32 index = 0; index < flow->flowbox_count; ++index, ++box) {
            if (box->type == 0 && box->ai_trigger_group != 0 && box->ai_trigger_group <= 32) {
                i32 group_index = box->ai_trigger_group - 1;
                FLOWBOXGIZMODATA_s *data = box->data;
                AITRIGGERSET_s *set = groups[group_index];
                if (set == NULL) {
                    set = AITriggerSetCreate(system, NULL);
                    groups[group_index] = set;
                }
                if (set != NULL) {
                    for (i32 gizmo_index = 0; gizmo_index < data->gizmo_count; ++gizmo_index) {
                        AITriggerSetAddTrigger(world->ai_sys, set, data->gizmos[gizmo_index]->gizmo);
                    }
                }
            }
        }
    }
}

void AITriggerSetAddTrigger(AISYS_s *, AITRIGGERSET_s *, GIZMO_s *) {
}

void AITriggerSetSysProcess(AITRIGGERSETSYS_s *) {
}
