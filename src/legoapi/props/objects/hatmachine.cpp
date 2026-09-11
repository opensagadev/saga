#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Hat_GetAbsTargetPos(HATMACHINE_s *machine, nuvec_s *position) {
    if (position == NULL || machine == NULL) {
        return;
    }

    NUVEC offset = machine->target_offset;
    NuVecRotateY(&offset, &offset, machine->y_rotation);
    offset.x += machine->position.x;
    offset.z += machine->position.z;
    *position = offset;
}
