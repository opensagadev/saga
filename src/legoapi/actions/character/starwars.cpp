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

extern "C" {

#if defined(__GNUC__) && !defined(__clang__)
#define STARWARS_OMIT_FRAME_POINTER __attribute__((optimize("omit-frame-pointer")))
#else
#define STARWARS_OMIT_FRAME_POINTER
#endif

    STARWARS_OMIT_FRAME_POINTER void InitFn_MidSpecialMove(MIDSPECIALMOVEFN function) {
        MidSpecialMoveFn = function;
    }

    STARWARS_OMIT_FRAME_POINTER void InitFn_PreparingForSpecialMove(PREPARINGSPECIALMOVEFN function) {
        PreparingForSpecialMoveFn = function;
    }

#undef STARWARS_OMIT_FRAME_POINTER

} // extern "C"
