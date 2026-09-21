#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Whip_Release(GameObject_s *object) {
    if (LEGOCONTEXT_WHIP != -1 && object->character_context == LEGOCONTEXT_WHIP && object->field_0x788 != NULL) {
        object->carried_object_basis[0] = v100;
        object->carried_object_basis[1] = v010;
        object->carried_object_basis[2] = v001;
        SuperCarry_Throw(object, 1);
    }
}

void Whip_MoveCode(GameObject_s *) {
    STUBBED();
}
