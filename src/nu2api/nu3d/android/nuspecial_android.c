#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial_internal.h"
#include "nu2api/numath/vuvec_internal.h"

// The reference places this platform wrapper immediately before
// _GLOBAL__sub_I_nuspecial_android.c (0x2a08d5).
extern "C" i32 NuSpecialDrawAtAlpha(void *special, NUMTX *mtx, f32 alpha) {
    if (static_cast<NuSpecialHandleLayout *>(special)->scene == NULL)
        return 0;
    if (alpha <= 0.0f)
        return 0;
    i32 result;
    if (alpha < 1.0f) {
        NuSpecialConstAlpha(1, alpha);
        result = NuDisplayListRndrSpecial(static_cast<nuhspecial_s *>(special), mtx, 0, NULL, NULL);
        NuSpecialConstAlpha(0, 0.0f);
    } else {
        result = NuDisplayListRndrSpecial(static_cast<nuhspecial_s *>(special), mtx, 0, NULL, NULL);
    }
    return result;
}
