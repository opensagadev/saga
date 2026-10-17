#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

#include <string.h>
#include <stdint.h>
#include "nu2api/nucore/nulist.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
extern void *CutScene_FindInst(CUTSYS *, char *);
extern FLOWBOX_s *FlowBoxFindByName(GIZFLOW_s *, char *);
extern "C" {
    extern f32 aiEditor_DrawYOffset;
    void LocaledbitsDrawCircleXY(NUVEC *, f32, u32, i32, i32);
    void LocaledbitsDrawSolidCircleXY(NUVEC *, f32, f32, f32, u32, i32, i32);
}

void AIPathCalcExtents(AIPATH *path);

i32 AIPathCheckExtents(AIPATH *path, NUVEC *position);

__attribute__((force_align_arg_pointer)) void pathEditorDrawNode(nuvec_s *position, float radius, float lower_height,
                                                                 float upper_height, u32 colour, numtl_s *material,
                                                                 i32 segments, i32 solid);

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
    if (connection == NULL || system == NULL) {
        return NULL;
    }

    AIPATHCNXHELPER_s *helper = system->helpers;
    for (i32 index = 0; index < system->helper_count; ++index, ++helper) {
        if (helper->connection == connection && (helper->direction == 0xff || helper->direction == direction) &&
            helper->type == type && (filter == NULL || filter(helper, object, connection, direction, type) == 0)) {
            return helper;
        }
    }
    return NULL;
}

void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);

void AIPathCnxControlSysUpdate(AIPATHCNXCONTROLSYS_s *system);

AIPATHCNXCONTROLLER_s *AIPathCnxControllerCreate(AIPATHCNXCONTROLSYS_s *control_system, AISYS_s *ai_system,
                                                 AIPATH_s *path, char *from, char *to, i32 target_type,
                                                 char *target_name, i32 fake_animation_id, i32 gizmo_output);

void AIPathCnxControllerDestroy(AIPATHCNXCONTROLSYS_s *system, AIPATHCNXCONTROLLER_s *controller);

void AIPathCnxSetTemporaryBlock(AIPATH_s *path, char *from_name, char *to_name, i32 blocked);

AIPATHCNXHELPER_s *AIPathCnxHelperSys_AddHelper(AIPATHCNXHELPERSYS_s *system, AIPATHCNX_s *connection, u8 direction,
                                                void *target, u8 type) {
    if (system == NULL) {
        return NULL;
    }

    i16 helper_count = system->helper_count;
    if (helper_count >= system->field_0x00 || target == NULL || connection == NULL) {
        return NULL;
    }

    AIPATHCNXHELPER_s *helper = system->helpers;
    for (i32 index = 0; index < helper_count; ++index, ++helper) {
        if (helper->connection == connection && helper->type == type && helper->target == target) {
            if (helper->direction != direction) {
                helper->direction = 0xff;
            }
            return helper;
        }
    }

    helper = &system->helpers[helper_count];
    ++helper_count;
    system->helper_count = helper_count;
    helper->connection = connection;
    helper->direction = direction;
    helper->target = target;
    helper->type = type;
    return helper;
}

void AIPathCnxControllerSetOnRange(AIPATHCNXCONTROLLER_s *controller, i32 start_frame, i32 end_frame);
