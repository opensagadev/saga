#include "decomp.h"
#include "legoapi/render/fx/particles.h"
#include "legoapi/render/fx.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nufile/nufile.h"
#include "gameapi/edtools/edstubs.h"
#include "globals.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nucamera.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern NUGLOBALRNDRSTATE render_state;
extern i32 back_rgba[2];
extern f32 MainRenderTime;
i32 draw_octree = 1;
void BackDrop_Draw(f32 alpha, i32 flags);
extern "C" {
    extern i32 DEBPAGE_AREA;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_GENERAL;
    void DebFreeAllCreatedEffects(void);
    void DebrisSetRenderGroup(i32 group);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    i32 NuRndrBeginScene(i32 flags);
    void NuRndrEndScene(void);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
}

void OctreeRndr(u8 *visibility, nuoctreenode_s *root, i32 enabled) {
    struct OCTREE_STACK_ENTRY_s {
        nuoctreenode_s *node;
        i32 child_index;
    };

    if (enabled == 0 || root == NULL)
        return;

    OCTREE_STACK_ENTRY_s stack[128];
    i32 depth = 0;
    stack[0].child_index = 0;
    stack[0].node = root;

    while (depth >= 0) {
        nuoctreenode_s *node = stack[depth].node;
        f32 minimum_depth;
        i32 clip = NuCameraClipTestExtentsGeneric(&node->minimum, &node->maximum, &numtx_identity, node->far_clip, 1,
                                                  &minimum_depth);
        if (clip == 0) {
        } else if (clip == 1) {
            if (draw_octree != 0)
                NuRndrBoundingBox(&node->minimum, &node->maximum, &numtx_identity, 0xff0000ff);
            i32 *index = node->fully_visible_indices;
            i32 *end = index + node->fully_visible_count;
            if (node->fully_visible_count > 0) {
                do {
                    i32 value = *index++;
                    visibility[value >> 2] |= 1 << ((value & 3) << 1);
                } while (index != end);
            }
        } else {
            i32 *index = node->partially_visible_indices;
            i32 *end = index + node->partially_visible_count;
            if (node->partially_visible_count > 0) {
                do {
                    i32 value = *index++;
                    visibility[value >> 2] |= 2 << ((value & 3) << 1);
                } while (index != end);
            }
            if (node->child_count == 0 && draw_octree != 0)
                NuRndrBoundingBox(&node->minimum, &node->maximum, &numtx_identity, 0xff00ff00);

            i32 child_index = stack[depth].child_index++;
            if (child_index != node->child_count) {
                ++depth;
                stack[depth].child_index = 0;
                stack[depth].node = node->children[child_index];
                continue;
            }
        }
        --depth;
    }
}

void AddCameraRain(WORLDINFO_s *world, i32 mode) {
    if ((world->current_level->flags & 0x4000) != 0) {
        NUVEC position = GameCam->pos;
        position.x += GameCam->dir.x + GameCam->dir.x;
        position.y += GameCam->dir.y + GameCam->dir.y;
        position.z += GameCam->dir.z + GameCam->dir.z;
        AddVariableShotDebrisEffectTimed1(world->debris_sys->entries[mode].effect, &position, 60, FRAMETIME, 0, 0,
                                          NULL);
    }
}

void Particles_Stop(WORLDINFO_s *world) {
    if (world->page_anim != -1) {
        edanimStopPage(world->page_anim);
    }
    if (world->page_pp != -1) {
        edppStopPage((i8)world->page_pp);
    }
    if (DEBPAGE_CHARACTER != -1) {
        edppStopPage((i8)DEBPAGE_CHARACTER);
    }
    if (DEBPAGE_GENERAL != -1) {
        edppStopPage((i8)DEBPAGE_GENERAL);
    }
    if (DEBPAGE_AREA != -1) {
        edppStopPage((i8)DEBPAGE_AREA);
    }
    DebFreeAllCreatedEffects();
}

void Particles_Start(WORLDINFO_s *world) {
    if (DEBPAGE_AREA != -1) {
        edppStartPage((i8)DEBPAGE_AREA);
    }
    if (world->page_pp != -1) {
        edppStartPage((i8)world->page_pp);
    }
    DebrisSetRenderGroup(1);
    if (world->page_anim >= 0) {
        edanimStartPage(world->page_anim);
    }
}

void Particles_DumpAreaPage() {
    if (DEBPAGE_AREA != -1) {
        edppStopPage(static_cast<i8>(DEBPAGE_AREA));
        edppClearPage(static_cast<i8>(DEBPAGE_AREA));
        DEBPAGE_AREA = -1;
    }
}

void Particles_LoadAreaPage(char *path) {
    DEBPAGE_AREA = -1;
    if (NuFileExists(path) != 0) {
        DEBPAGE_AREA = edppLoadPage(path, 1, 0);
    }
}

void NoRender() {
    pNuCam->mtx = numtx_identity;
    NuCameraSet(pNuCam);
    NuRndrBeginScene(-1);

    if (back_rgba[0] == back_rgba[1]) {
        NuRndrClear(0xf00, back_rgba[0], 1.0f);
    } else {
        NuRndrGradClear(0xf00, back_rgba[0], back_rgba[1], 1.0f);
    }

    if (MainRenderTime != 1.0f) {
        BackDrop_Draw(1.0f - MainRenderTime, 0);
    }
    NuRndrEndScene();
}
