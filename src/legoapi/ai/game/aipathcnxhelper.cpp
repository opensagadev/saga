#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nuanim3.h"

#include <string.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);

void (*AIPathCnxHelperSysInitFn)(WORLDINFO_s *) = NULL;

void AIPathCnxHelperSysReset(WORLDINFO_s *world, AIPATHCNXHELPERSYS_s *system) {
    if (system != NULL) {
        if (system->helper_count != 0) {
            memset(system->helpers, 0, system->helper_count * sizeof(*system->helpers));
            system->helper_count = 0;
        }
        if (AIPathCnxHelperSysInitFn != NULL) {
            AIPathCnxHelperSysInitFn(world);
        }
    }
}

AIPATHCNXHELPER_s *AIPathCnxHelperSys_Find(AIPATHCNXHELPERSYS_s *system, GameObject_s *object, AIPATHCNX_s *connection,
                                           u8 direction, u8 type,
                                           i32 (*filter)(AIPATHCNXHELPER_s *, GameObject_s *, AIPATHCNX_s *, u8, u8)) {
    if (connection == NULL && object != NULL) {
        connection = object->ai.path_info.connection;
        direction = object->ai.path_info.direction;
    }
    if (connection != NULL && system != NULL) {
        AIPATHCNXHELPER_s *helper = system->helpers;
        for (i32 index = 0; index < system->helper_count; ++index, ++helper) {
            if (helper->connection == connection && (helper->direction == 0xff || helper->direction == direction) &&
                helper->type == type && (filter == NULL || filter(helper, object, connection, direction, type) == 0)) {
                return helper;
            }
        }
    }
    return NULL;
}

AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection, u8 direction,
                                                void *target, u8 type) {
    if (system == NULL) {
        return NULL;
    }

    AIPATHCNXHELPER_s *helper = NULL;
    i16 helper_count = system->helper_count;
    if (helper_count < system->field_0x00) {
        if (target == NULL || connection == NULL) {
            return NULL;
        }

        AIPATHCNXHELPER_s *existing = system->helpers;
        for (i32 index = 0; index < helper_count; ++index, ++existing) {
            if (existing->connection == connection && existing->type == type && existing->target == target) {
                if (existing->direction != direction) {
                    existing->direction = 0xff;
                }
                helper = existing;
                break;
            }
        }

        if (helper == NULL) {
            helper = &system->helpers[helper_count];
            ++helper_count;
            system->helper_count = helper_count;
            helper->connection = connection;
            helper->direction = direction;
            helper->target = target;
            helper->type = type;
        }
    }
    return helper;
}

