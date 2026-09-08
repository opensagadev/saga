#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern numtl_s *ropemtl;

void InitRopeMtl(char *name, variptr_u *buffer, variptr_u *buffer_end) {
    char path[64] = "stuff\\";
    if (name == NULL) {
        ropemtl = NULL;
        return;
    }
    ropemtl = NuMtlCreate3D(1);
    ropemtl->opacity = 1.0f;
    ropemtl->attribs.unknown_2_1_2 = 0;
    ropemtl->attribs.unknown_1_1_2 = 0;
    ropemtl->attribs.unknown_1_4_8 = 0;
    ropemtl->attribs.z_mode = 0;
    ropemtl->attribs.alpha_mode = 0;
    ropemtl->attribs.filter_mode = 1;
    NuStrCat(path, name);
    buffer->addr = ALIGN(buffer->addr, 16);
    ropemtl->tex_id = NuTexRead(path, buffer, reinterpret_cast<VARIPTR *>(buffer_end->addr));
    if (ropemtl->tex_id == 0) {
        NuMtlDestroy(ropemtl);
        ropemtl = NULL;
    } else {
        NuMtlUpdate(ropemtl);
    }
}

void ropesegment(numtl_s *, nuvec_s *, i32, i32) {
}
