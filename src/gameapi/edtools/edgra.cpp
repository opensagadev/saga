#include "gameapi/edtools/edgra.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nudlist.h"
#include "legoapi/legoapi_types.h"
#include "gamelib/nuwind/nuwind.h"

void edgraCalculatePage(char page, i32 calculate_vectors);
struct NuFadeObjGType;

extern "C" {
    i32 edgra_units_used;
    i32 edgra_page_used[8];
    i32 edgra_page_on[8];
    i32 edgra_page_calculate_done[8];
    i32 edgra_page_vectors_valid[8];
    void NuWindInit(void);
    void NuFadeObjInit(void);
    extern edgra_clump_s *GrassClumps;
    extern i32 EDGRA_MAX_CLUMPS;
    NUGSCN *edgra_page_scene[8];
    void *edgra_page_terrain[8];
    void *TerrainGetCur(void);
    void TerrainSetCur(void *);
    NuWindGType *NuWindCreateMtx(u32 *, NUMTX *, i16, f32, f32, i32, f32, f32);
    NuFadeObjGType *NuFadeObjCreateMtx(nuhspecial_s *, NUMTX *, i16, f32, f32, i32);

    void edgraStartPage(i8 page) {
        if (!edgra_page_used[page] || !edgra_page_scene[page] || edgra_page_on[page])
            return;
        void *terrain = TerrainGetCur();
        TerrainSetCur(edgra_page_terrain[page]);
        if (!edgra_page_calculate_done[page]) {
            if (!edgra_page_vectors_valid[page]) {
                edgraCalculatePage(page, 1);
                edgra_page_vectors_valid[page] = 1;
            } else {
                edgraCalculatePage(page, 0);
            }
            edgra_page_calculate_done[page] = 1;
        }
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
            edgra_clump_s *clump = &GrassClumps[i];
            if (clump->element_count && clump->page == static_cast<u8>(page)) {
                i32 count = clump->element_count;
                if (edgra_units_used + count > 0x3000)
                    count = 0x3000 - edgra_units_used;
                if (count > 0) {
                    NUGSCN *scene = edgra_page_scene[page];
                    nuhspecial_s special;
                    special.scene = scene;
                    if (scene->display_list) {
                        special.special = NULL;
                        special.display_special =
                            &static_cast<NUDISPLAYSPECIAL *>(scene->display_list->specials)[clump->special_index];
                    } else {
                        special.special =
                            &reinterpret_cast<NuSpecialLegacyLayout *>(scene->specials)[clump->special_index];
                        special.display_special = NULL;
                    }
                    if (clump->kind == 2 || clump->kind == 3) {
                        NuFadeObjCreateMtx(&special, clump->matrices, count, clump->near_distance, clump->far_distance,
                                           clump->flags);
                    } else {
                        NuWindCreateMtx(reinterpret_cast<u32 *>(&special), clump->matrices, count, clump->field_18,
                                        clump->field_20, clump->flags, clump->near_distance, clump->far_distance);
                    }
                    edgra_units_used += count;
                }
            }
        }
        edgra_page_on[page] = 1;
        TerrainSetCur(terrain);
    }

    void edgraStopPage(i8 page) {
        if (edgra_page_on[page]) {
            edgra_page_on[page] = 0;
            edgra_units_used = 0;
            NuWindInit();
            NuFadeObjInit();
            for (i32 i = 0; i < 8; ++i) {
                if (edgra_page_on[i]) {
                    edgra_page_on[i] = 0;
                    edgraStartPage(i);
                }
            }
        }
    }

    void edgraInitAllClumps(void) {
        edgra_units_used = 0;
        NuWindInit();
        NuFadeObjInit();
        for (i32 page = 0; page < 8; ++page)
            edgra_page_on[page] = 0;
        for (i32 page = 0; page < 8; ++page) {
            if (edgra_page_used[page]) {
                edgra_page_calculate_done[page] = 0;
                edgra_page_vectors_valid[page] = 0;
                edgraStartPage(page);
            }
        }
    }
}
