#include "decomp.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct GizMiniCutData {
    u8 reserved_00[0x1a];
    u16 guid;
};

i32 GizMiniCut_GetGuid(GIZMO_s *gizmo) {
    if (gizmo == NULL || gizmo->object == NULL) {
        return -1;
    }
    return static_cast<GizMiniCutData *>(gizmo->object)->guid;
}
