#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GizTorpMachine_FindNearest(WORLDINFO_s *, nuvec_s *, float *) {
}

bool ZapTarget(GameObject_s *object) {
    if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
        (object->apiobj.character_data->model_flags & 0x10) == 0) return false;
    void **animations = object->apiobj.character_model->model_data_b;
    i32 context = object->character_context;
    if (animations[0x41] == NULL || (CInfo[context].flags & 0x8000) != 0) return false;
    if ((object->apiobj.character_data->model_flags & 0x20) != 0 &&
        (animations[0x42] == NULL || animations[0x43] == NULL || animations[0x44] == NULL)) return false;
    return context != 0x15 && context != 0x17 && context != 0x41 && context != 0x33 && context != 0x3b && context != 0x39;
}
