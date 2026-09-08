#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void TractorBeamCode(GameObject_s *) {
}

i32 LEGOCONTEXT_TUBE = -1;

i32 ObjInTube(GameObject_s *object) {
    if (LEGOCONTEXT_TUBE != -1 && LEGOCONTEXT_TUBE == object->character_context)
        return 1;
    if (LEGOCONTEXT_GLIDE != -1 && LEGOCONTEXT_GLIDE == object->character_context && object->field_0x788 != NULL)
        return 1;
    return 0;
}
