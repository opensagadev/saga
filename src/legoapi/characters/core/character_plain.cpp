#include "globals.h"

#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nuhgobj.h"

extern "C" {


    nuhgobj_s *Temphgobj;
    u8 TempNumJoints;

    void NuHGobjRestrictEvaluation(nuhgobj_s *object) {
        Temphgobj = object;
        if (object != NULL) {
            TempNumJoints = object->joint_count;
            object->joint_count = 1;
        }
    }

    void NuHGobjRestoreEvaluation(void) {
        if (Temphgobj != NULL) {
            Temphgobj->joint_count = TempNumJoints;
            Temphgobj = NULL;
        }
    }

} // extern "C"
