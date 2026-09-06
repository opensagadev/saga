#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GetGenericGoon(i32) {
}

i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32) {
    return 0;
}

i32 objInNetWaitContext(GameObject_s *object, i32 context) {
    if (LEGOCONTEXT_NETWAIT != -1 && object != NULL && LEGOCONTEXT_NETWAIT == object->character_context) {
        return object->context_animation == context;
    }
    return false;
}

void oneAtOnce_CanAttack(GameObject_s *, GameObject_s *) {
}

void oneAtOnce_GetHoldRange(GameObject_s *) {
}

void oneAtOnce_MaintainArray() {
}

void oneAtOnce_SetDistPerRow(float) {
}

void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *) {
}

void oneAtOnce_SetNumAttackers(i32) {
}

void MakeBaddiesForgetAboutParty(i32) {
}

void oneAtOnce_SetInitDistPerRow(float) {
}

void oneAtOnce_SetAttackersPerRow(i32) {
}
