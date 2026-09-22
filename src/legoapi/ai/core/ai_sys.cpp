#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "legoapi/items/base/apiobject.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aiscript_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>
struct nuhspecial_s;
struct minitrooperteam_s;
struct nuvec_s;

void *GameBufferAlloc(VARIPTR *, VARIPTR *, i32);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx);
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
void *CreateClimbObjectSys(VARIPTR *buf, VARIPTR *buf_end, i32 count) {
    if (count == 0)
        return NULL;
    CLIMBOBJECTSYS_s *system = static_cast<CLIMBOBJECTSYS_s *>(GameBufferAlloc(buf, buf_end, sizeof(CLIMBOBJECTSYS_s)));
    if (system == NULL)
        return NULL;
    system->capacity = static_cast<u16>(count);
    system->objects =
        static_cast<CLIMBOBJECT_s *>(GameBufferAlloc(buf, buf_end, system->capacity * sizeof(CLIMBOBJECT_s)));
    return system;
}
