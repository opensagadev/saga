#include "legoapi/world/world_shared.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "decomp.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/android/nurndr_android.h"
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

    void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context) {
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, &visibility_context));
        const bool portal_filter = visibility != NULL && portal_special_objects != 0 &&
                                   visibility->portal_marker != NULL && portals_enabled != 0;
        const bool shadow_pass = currentScene.unknown_3c != NULL;

        if ((scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) != 0 ||
            noscenespecials != 0) {
            return;
        }

        NUGSCN temporary_scene;
        NUGSCN *gscene = scene->gscene;
        if (gscene == NULL) {
            temporary_scene.display_list = scene;
            gscene = &temporary_scene;
        }

        nuhspecial_s special_handle;
        special_handle.scene = gscene;
        for (i32 index = 0; index < scene->nspecials; ++index) {
            NUDISPLAYSPECIAL *special = &static_cast<NUDISPLAYSPECIAL *>(scene->specials)[index];
            if ((special->flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                continue;
            }

            const i32 instance_index = special->instance_ix;
            if (shadow_pass &&
                (static_cast<u8>(scene->visibility_flags[instance_index]) & NUDL_INSTANCE_FLAG_CASTS_SHADOW) == 0) {
                continue;
            }
            if (portal_filter &&
                (static_cast<u8>(scene->visibility_flags[instance_index]) & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) ==
                    0 &&
                (visibility->portal_bits[instance_index >> 3] & (1U << (instance_index & 7))) == 0) {
                continue;
            }

            special_handle.display_special = special;
            NUMTX *draw_matrix = special->draw_mtx_ptr;
            if (draw_matrix == NULL || draw_matrix == reinterpret_cast<NUMTX *>(-1)) {
                draw_matrix = &special->draw_mtx;
            }
            NuDisplayListRndrSpecial(&special_handle, draw_matrix, 0, NULL, NULL);
        }
    }

    void FmvTimePS(void) {
        STUBBED();
    }

    void PerspectMidPoint(NUVEC *result, NUVEC *first, NUVEC *second, NUVEC *camera_position) {
        f32 first_distance = NuVecDist(camera_position, first, NULL);
        f32 second_distance = NuVecDist(camera_position, second, NULL);
        f32 ratio = first_distance / (first_distance + second_distance);
        result->x = first->x + (second->x - first->x) * ratio;
        result->y = first->y + (second->y - first->y) * ratio;
        result->z = first->z + (second->z - first->z) * ratio;
    }

    void RndrMaskScreen(void) {
        STUBBED();
    }

} // extern "C"
