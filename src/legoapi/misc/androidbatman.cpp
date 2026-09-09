#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nufloat.h"

#include <stddef.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 ObjZappedBlue(GameObject_s *object) {
    if (object->field_0x7a5 == 0x42)
        return 1;
    if (object->field_0x7a5 == 0x1c) {
        GameObject_s *holder = static_cast<GameObject_s *>(object->field_0x780);
        if (holder != NULL && (holder->field_0xe21 & 1) != 0)
            return 1;
    }
    return 0;
}

void PeriscodeCode(GameObject_s *) {
}

void NewScanHandelFull(nuvec_s *, nuvec_s *, float, i32, i32) {
}

void NewScanHandelSubset(i16 *, nuvec_s *, nuvec_s *, float, i32) {
}

void RegisterGizmoTypes_Batman(variptr_u *, variptr_u *) {
}

void NewScan(nuvec_s *, i32, i32) {
}
