#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nutrig.h"

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

i32 AITriggerSetAddTrigger(AISYS_s *, AITRIGGERSET_s *, GIZMO_s *);
void AISysGetPathPos2(AISYS_s *, nuvec_s *, AIPATHINFO_s *, nuvec_s *, AIPATH_s *, i32);

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

i32 AITriggerSetAddTrigger(AISYS_s *system, AITRIGGERSET_s *set, GIZMO_s *gizmo) {
    if (set == NULL || system == NULL || !(set->flags & 1) || set->trigger_count >= 8 || gizmo == NULL)
        return 0;

    char *name = GizmoGetName(gizmo);
    AILOCATOR *locator = name != NULL ? AIPathFindLocator(system, name) : NULL;
    if (gizmo->type_id == lever_gizmotype_id) {
        LEVER_s *lever = static_cast<LEVER_s *>(gizmo->object);
        set->gizmos[set->trigger_count] = gizmo;
        if (locator != NULL) {
            set->locators[set->trigger_count] = *locator;
        } else {
            set->locators[set->trigger_count].position = lever->floor_position;
            AISysGetPathPos2(system, &set->locators[set->trigger_count].position,
                &set->locators[set->trigger_count].path_info, &set->locators[set->trigger_count].position, NULL, 0xff);
            set->locators[set->trigger_count].direction = NuAtan2D(lever->position.x - lever->floor_position.x,
                lever->position.z - lever->floor_position.z);
        }
    } else if (gizmo->type_id == obstacle_gizmotype_id) {
        GIZOBSTACLE_s *obstacle = static_cast<GIZOBSTACLE_s *>(gizmo->object);
        if (obstacle->mode != 1 && obstacle->mode != 2 && obstacle->mode != 5 && obstacle->mode != 6 && obstacle->mode != 7)
            return 0;
        set->gizmos[set->trigger_count] = gizmo;
        if (locator != NULL) {
            set->locators[set->trigger_count] = *locator;
        } else {
            set->locators[set->trigger_count].position = obstacle->secondary_position;
            AISysGetPathPos2(system, &set->locators[set->trigger_count].position,
                &set->locators[set->trigger_count].path_info, &set->locators[set->trigger_count].position, NULL, 0xff);
        }
        if (set->locators[set->trigger_count].path_info.connection == NULL)
            return 0;
        ++set->trigger_count;
        return 1;
    } else if (gizmo->type_id == spinner_gizmotype_id) {
        GIZSPINNER_s *spinner = static_cast<GIZSPINNER_s *>(gizmo->object);
        set->gizmos[set->trigger_count] = gizmo;
        if (locator != NULL) {
            set->locators[set->trigger_count] = *locator;
        } else {
            set->locators[set->trigger_count].position = spinner->position;
            AISysGetPathPos2(system, &set->locators[set->trigger_count].position,
                &set->locators[set->trigger_count].path_info, &set->locators[set->trigger_count].position, NULL, 0xff);
        }
    } else if (gizmo->type_id == force_gizmotype_id) {
        GIZFORCE_s *force = static_cast<GIZFORCE_s *>(gizmo->object);
        set->gizmos[set->trigger_count] = gizmo;
        if (locator != NULL) {
            set->locators[set->trigger_count] = *locator;
        } else {
            set->locators[set->trigger_count].position = force->file_position;
            f32 height = GameShadow(NULL, &set->locators[set->trigger_count].position, 5.0f, -1);
            if (height != 2000000.0f && height > set->locators[set->trigger_count].position.y)
                set->locators[set->trigger_count].position.y = height;
            AISysGetPathPos2(system, &set->locators[set->trigger_count].position,
                &set->locators[set->trigger_count].path_info, &set->locators[set->trigger_count].position, NULL, 0xff);
        }
    } else if (gizmo->type_id == grapple_gizmotype_id) {
        GRAPPLE_s *grapple = static_cast<GRAPPLE_s *>(gizmo->object);
        set->gizmos[set->trigger_count] = gizmo;
        if (locator != NULL) {
            set->locators[set->trigger_count] = *locator;
        } else {
            set->locators[set->trigger_count].position = grapple->ground_position;
            f32 height = GameShadow(NULL, &set->locators[set->trigger_count].position, 5.0f, -1);
            if (height != 2000000.0f && height > set->locators[set->trigger_count].position.y)
                set->locators[set->trigger_count].position.y = height;
            AISysGetPathPos2(system, &set->locators[set->trigger_count].position,
                &set->locators[set->trigger_count].path_info, &set->locators[set->trigger_count].position, NULL, 0xff);
            set->gizmos[set->trigger_count] = gizmo;
        }
    } else {
        return 0;
    }
    if (set->locators[set->trigger_count].path_info.connection == NULL)
        return 0;
    ++set->trigger_count;
    return 1;
}

void AITriggerSetSysProcess(AITRIGGERSETSYS_s *) {
}
