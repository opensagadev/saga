#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"

struct AIPACKET_s;
struct APIOBJECT_s;
struct AISYS_s;
struct AIGROUP_s;
struct AIROW_s;
struct nufpar_s;

typedef i32 (*MIDSPECIALMOVEFN)(AISYS_s *, AIPACKET_s *, APIOBJECT_s *);

PREPARINGSPECIALMOVEFN PreparingForSpecialMoveFn = NULL;
MIDSPECIALMOVEFN MidSpecialMoveFn = NULL;

static __used__ void SpecialMove_ConfigParticipant(nufpar_s *, i16 *, i8 *, i16 *) {
}

extern "C" {

    void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function) {
        MidSpecialMoveFn = function;
    }

    void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function) {
        PreparingForSpecialMoveFn = function;
    }

} // extern "C"
