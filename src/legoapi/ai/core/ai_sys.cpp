#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
struct nuhspecial_s;
struct minitrooperteam_s;
struct nuvec_s;

extern "C" void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
extern "C" void *AISysLoadEx(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param,
                             char *load_dir);

extern "C" void *AISysLoad(void *buf, void *buf_end, i32 size, void *gscn, char *dir, char *name, char *param) {
    return AISysLoadEx(buf, buf_end, size, gscn, dir, name, param, dir);
}
void *AIPathCnxControlSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    AIPATHCNXCONTROLSYS_s *system =
        static_cast<AIPATHCNXCONTROLSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXCONTROLSYS_s)));
    if (system != NULL) {
        memset(system, 0, sizeof(*system));
        system->controllers = static_cast<AIPATHCNXCONTROLLER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXCONTROLLER_s)));
        if (system->controllers != NULL) {
            system->controller_count = count;
            AIPathCnxControlSysReset(system);
        }
    }
    return system;
}
void *AIPathCnxHelperSysCreate(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    if (count == 0) {
        return NULL;
    }
    AIPATHCNXHELPERSYS_s *system =
        static_cast<AIPATHCNXHELPERSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(AIPATHCNXHELPERSYS_s)));
    if (system != NULL) {
        system->helpers = static_cast<AIPATHCNXHELPER_s *>(
            AISysBufferAlloc(buf, buf_end, static_cast<u32>(count) * sizeof(AIPATHCNXHELPER_s)));
        if (system->helpers != NULL) {
            system->field_0x00 = static_cast<i16>(count);
        }
    }
    return system;
}
void GameAIScriptAddLevelSfx(WORLDINFO *world, NULISTHDR *scripts) {
    (void)world;
    (void)scripts;
}
void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    (void)buf;
    (void)buf_end;
    (void)count;
    return NULL;
}
extern "C" APIOBJECTSYS_s *APIObjectSysInit(i32 size, VARIPTR *buf, VARIPTR *buf_end) {
    APIOBJECTSYS_s *system = static_cast<APIOBJECTSYS_s *>(AISysBufferAlloc(buf, buf_end, sizeof(APIOBJECTSYS_s)));
    if (system == NULL) {
        return NULL;
    }

    memset(system, 0, sizeof(*system));
    if (size != 0) {
        system->objects = static_cast<APIOBJECT *>(AISysBufferAlloc(buf, buf_end, static_cast<u32>(size) * 64));
        if (system->objects != NULL) {
            system->object_size = static_cast<u32>(size);
            memset(system->objects, 0, static_cast<u32>(size) * 64);
        }
    }
    return system;
}

static __used__ void FormationMove(AIGROUP_s *, int (*)(AIGROUP_s *, AIROW_s *, AIROW_s *, APIOBJECT_s *)) {
}

static __used__ void GenerateTrooperTeamShape(minitrooperteam_s *, int) {
}
