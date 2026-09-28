#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/bgproc.h"

#if defined(__GNUC__) && !defined(__clang__)
__attribute__((optimize("omit-frame-pointer")))
#endif
i32 bgprocIsFrozen() {
    return bgproc_frozen;
}
