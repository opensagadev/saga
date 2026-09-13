#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"

#include <float.h>
#include <math.h>
#include <string.h>

struct rtlset;
rtlset *PartRTL = NULL;

extern "C" {
    void SetPartRTLSet(usize rtl_set) {
        PartRTL = reinterpret_cast<rtlset *>(rtl_set);
    }

} // extern "C"
