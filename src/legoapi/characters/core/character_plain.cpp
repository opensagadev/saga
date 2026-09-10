#include "globals.h"

#include "legoapi/characters/core/character.h"
#include "nu2api/nucore/nuhgobj.h"

extern "C" {

    void APIResetCharacterRemap(void) {
        for (i32 i = 0; i < apicharsys->character_count; ++i) {
            if ((apicharsys->char_data[i].model_flags & 2) == 0) {
                apicharsys->playermodelids[i] = -1;
            }
        }
    }

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
