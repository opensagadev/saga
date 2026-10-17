#include "legoapi/world/world_shared.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nufmv_android.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuvport.h"
#include "globals.h"

#include <string.h>

struct numtl_s;
typedef struct numtl_s NUMTL;

extern "C" {

    extern NUGLOBALRNDRSTATE render_state;
    extern nurenderscene_s currentScene;

    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);

    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *matrix, i32 skinned, NUMTX *skin_matrices,
                                 DEFORMERWEIGHTSARRAY *deformer_weights);

f32 FmvTimePS(void);

void PerspectMidPoint(NUVEC *result, NUVEC *first, NUVEC *second, NUVEC *camera_position);

} // extern "C"
