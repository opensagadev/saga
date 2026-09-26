#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void) {
    for (i32 page = 0; page < 8; ++page) {
        if (edbri_page_used[page])
            edbriStartPage(page);
    }
}

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue) {
    i32 colour = 0xff000000 | red | (green << 8) | (blue << 16);
    NuRndrLine3dDbg(centre->x - 0.5f, centre->y, centre->z, centre->x + 0.5f, centre->y, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y - 0.5f, centre->z, centre->x, centre->y + 0.5f, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y, centre->z - 0.5f, centre->x, centre->y, centre->z + 0.5f, colour);
}

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments) {
    NURND_VERTEX3D vertices[2];
    NURND_VERTEX3D &first = vertices[0];
    NURND_VERTEX3D &second = vertices[1];
    second.position.x = 0.0f;
    second.position.y = 0.0f;
    second.position.z = radius_z;
    NuVecRotateY(&second.position, &second.position, rotation);
    NuVecAdd(&second.position, &second.position, centre);
    first.colour = colour;
    second.colour = colour;
    for (i32 i = 1; i <= segments; ++i) {
        first.position = second.position;
        second.position = *centre;
        i32 angle = (i * 65536) / segments;
        NUVEC offset = {radius_x * NU_SIN_LUT(angle), 0.0f, radius_z * NU_COS_LUT(angle)};
        NuVecRotateY(&offset, &offset, rotation);
        NuVecAdd(&second.position, centre, &offset);
        first.position.y = lower_y;
        second.position.y = lower_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
        first.position.y = upper_y;
        second.position.y = upper_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
        first.position = second.position;
        first.position.y = lower_y;
        AiRndrLine3dDbg(first.position.x, first.position.y, first.position.z, second.position.x, second.position.y,
                        second.position.z, colour);
    }
}

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments) {
    NUVEC previous = {centre->x, centre->y, centre->z + radius};
    for (i32 i = 1; i <= segments; ++i) {
        i32 angle = (i * 65536) / segments;
        NUVEC next = {centre->x + NU_SIN_LUT(angle) * radius, centre->y, centre->z + NU_COS_LUT(angle) * radius};
        AiRndrLine3dDbg(previous.x, previous.y, previous.z, next.x, next.y, next.z, colour);
        previous = next;
    }
}

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments) {
    f32 previous_x = centre->x;
    f32 previous_z = centre->z + radius;
    for (i32 i = 1; i <= segments; ++i) {
        i32 angle = (i * 65536) / segments;
        f32 next_x = centre->x + radius * NU_SIN_LUT(angle);
        f32 next_z = centre->z + NU_COS_LUT(angle) * radius;
        AiRndrLine3dDbg(previous_x, lower_y, previous_z, next_x, lower_y, next_z, colour);
        AiRndrLine3dDbg(previous_x, upper_y, previous_z, next_x, upper_y, next_z, colour);
        AiRndrLine3dDbg(next_x, lower_y, next_z, next_x, upper_y, next_z, colour);
        previous_x = next_x;
        previous_z = next_z;
    }
}

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour) {
    NUMTX matrix = global_camera.mtx;
    f32 corners[4][2] = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    for (i32 i = 0; i < 4; ++i) {
        NUVEC start = {corners[i][0] * radius, corners[i][1] * radius, 0.0f};
        NUVEC end = {corners[(i + 1) & 3][0] * radius, corners[(i + 1) & 3][1] * radius, 0.0f};
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, colour);
    }
}

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32) {
    NUMTX matrix = global_camera.mtx;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D first, second;
    first.position.x = radius;
    first.position.y = 0.0f;
    first.position.z = 0.0f;
    first.colour = colour;
    second.colour = colour;
    NuVecRotateZ(&second.position, &first.position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC start = first.position;
        NUVEC end = second.position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, first.colour);
        first.position = second.position;
        NuVecRotateZ(&second.position, &second.position, step);
    }
}

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments) {
    NUMTX matrix = numtx_identity;
    matrix.m30 = centre->x;
    matrix.m31 = centre->y;
    matrix.m32 = centre->z;
    i32 step = 65536 / segments;
    NURND_VERTEX3D first, second;
    first.position.x = radius;
    first.position.y = 0.0f;
    first.position.z = 0.0f;
    first.colour = colour;
    second.colour = colour;
    NuVecRotateY(&second.position, &first.position, step);
    for (i32 i = 0; i < segments; ++i) {
        NUVEC start = first.position;
        NUVEC end = second.position;
        NuVecMtxTransform(&start, &start, &matrix);
        NuVecMtxTransform(&end, &end, &matrix);
        NuRndrLine3dDbg(start.x, start.y, start.z, end.x, end.y, end.z, first.colour);
        first.position = second.position;
        NuVecRotateY(&second.position, &second.position, step);
    }
}

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

f32 HUB_EPISODESUBTITLEY = -0.745f;
f32 HUB_EPISODETITLEY = -0.595f;
i32 draw_para = 1;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" bool StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern char *apiGameName;
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_NODATAAVAILABLE;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern i32 menu_flash;
extern f32 text3d_height;
extern f32 text3d_width;
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern f32 MainRenderTime;
extern i32 editor_active;
extern i32 Paused;
extern i32 screendump;
extern f32 pause_fade;
extern i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

namespace {
    struct NuDisplaySpecialLayout {
        NUMTX mtx;
        NUMTX draw_mtx;
        NUVEC min;
        f32 min_w;
        NUVEC max;
        f32 max_w;
        u8 pad_a0[0x10];
        NUCLIPOBJECT *clip_objects;
        char *name;
        u32 flags;
        f32 *clip_range;
        i32 instance_ix;
        NUMTX *draw_mtx_ptr;
        i16 wind_speed;
        i16 wind_scale;
        u32 pad_cc;
    };

} // namespace

DECOMP_ASSERT(sizeof(NuDisplaySpecialLayout) == 0xd0, "display special size");

// Camera zoom state
f32 CameraZoom = 1.0f;

// Graphics loading flags
i32 RemoveDirectionalMaps = 0;
i32 RemoveNormalMaps = 0;

NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);

    static void DisplaySceneSetClipResult(NUDLDLISTSCENE *scene, i32 clip_index, i32 clip_result) {
        const u32 buffer = scene->render_buffer >> 7;
        u8 *clip_bits = scene->clip_used[buffer];
        const u32 shift = static_cast<u32>(clip_index & 3) * 2;
        clip_bits[clip_index >> 2] |= static_cast<u8>(clip_result << shift);

        if (clip_result == 0) {
            return;
        }

        NUCLIPOBJECT &clip = scene->clip_objects[clip_index];
        u8 *material_bits = scene->mtl_used[buffer];
        for (i32 i = 0; i < clip.nmaterials; ++i) {
            const i32 material_id = clip.material_ids[i];
            material_bits[material_id >> 3] |= static_cast<u8>(1u << (material_id & 7));
        }
    }

    static f32 DisplaySceneDistanceSqrToCamera(const NUCLIPBOUNDS &bounds) {
        const f32 x = global_camera.mtx.m30 - bounds.center.x;
        const f32 y = global_camera.mtx.m31 - bounds.center.y;
        const f32 z = global_camera.mtx.m32 - bounds.center.z;
        return x * x + y * y + z * z;
    }

    // A scene has one bound/visibility record per instance, but one or more
    // clip-object records per instance.  lod_ranges terminates every instance
    // group with zero; preceding values select progressively nearer LODs.
    static i32 DisplaySceneSelectLod(const NUDLDLISTSCENE *scene, i32 first_clip, f32 distance_sqr) {
        i32 selected_clip = first_clip;
        if (scene->lod_ranges[selected_clip] != 0.0f && distance_sqr < scene->lod_ranges[selected_clip]) {
            do {
                ++selected_clip;
            } while (distance_sqr < scene->lod_ranges[selected_clip]);
        }
        return selected_clip;
    }

    static i32 DisplaySceneNextInstance(const NUDLDLISTSCENE *scene, i32 clip_index) {
        while (scene->lod_ranges[clip_index] != 0.0f) {
            ++clip_index;
        }
        return clip_index + 1;
    }

    static f32 DisplaySceneFarClip(const NUDLDLISTSCENE *scene, i32 instance_index, i32 clip_index) {
        f32 far_clip = scene->far_clip_ranges[clip_index];
        if (scene->fade_ranges == NULL ||
            (scene->visibility_flags[instance_index] & NUDL_INSTANCE_FLAG_DISTANCE_FADE) == 0) {
            return far_clip;
        }

        const f32 fade_start = scene->fade_ranges[instance_index * 2];
        const f32 fade_end = scene->fade_ranges[instance_index * 2 + 1];
        if (fade_end > fade_start) {
            return fade_end;
        }
        return far_clip;
    }

    static f32 DisplaySceneDistanceFade(const NUDLDLISTSCENE *scene, i32 instance_index, f32 distance_sqr) {
        if (scene->fade_ranges == NULL ||
            (scene->visibility_flags[instance_index] & NUDL_INSTANCE_FLAG_DISTANCE_FADE) == 0) {
            return 1.0f;
        }

        const f32 fade_start = scene->fade_ranges[instance_index * 2];
        const f32 fade_end = scene->fade_ranges[instance_index * 2 + 1];
        if (fade_end <= fade_start) {
            if (fade_end * fade_end < distance_sqr) {
                const f32 alpha = (NuFsqrt(distance_sqr) - fade_end) / (fade_start - fade_end);
                return alpha < 1.0f ? alpha : 1.0f;
            }
            return 0.0f;
        }

        if (fade_start * fade_start < distance_sqr) {
            const f32 alpha = (fade_end - NuFsqrt(distance_sqr)) / (fade_end - fade_start);
            return alpha > 0.0f ? alpha : 0.0f;
        }
        return 1.0f;
    }

    static void DisplaySceneEvaluateClipFallback(NUDLDLISTSCENE *scene) {
        u8 visibility_context[0x2800];
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, visibility_context));
        const bool portal_filter = visibility != NULL && visibility->portal_marker != NULL && portals_enabled != 0;
        scene->alpha_values = NULL;
        if (scene->nclip_objects <= 0) {
            return;
        }

        if (scene->fade_ranges != NULL) {
            VARIPTR *buffer = NuDisplayListGetBuffer();
            scene->alpha_values = static_cast<f32 *>(buffer->void_ptr);
            buffer->addr += static_cast<usize>(scene->nclip_objects) * sizeof(f32);
        }

        i32 clip_index = 0;
        i32 instance_index = 0;
        while (clip_index < scene->nclip_objects) {
            const u8 flags = static_cast<u8>(scene->visibility_flags[instance_index]);
            bool visibility_bit =
                visibility == NULL || visibility->instance_tree_bits == NULL ||
                (visibility->instance_tree_bits[instance_index >> 3] & (1U << (instance_index & 7))) != 0;
            if (portal_filter && (flags & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) == 0) {
                visibility_bit = (visibility->portal_bits[instance_index >> 3] & (1U << (instance_index & 7))) != 0;
            }
            if (visibility_bit && (flags & NUDL_INSTANCE_FLAG_VISIBLE) != 0) {
                NUCLIPBOUNDS &bounds = scene->clip_bounds[instance_index];
                const f32 distance_sqr = DisplaySceneDistanceSqrToCamera(bounds);
                const i32 selected_clip = DisplaySceneSelectLod(scene, clip_index, distance_sqr);
                const f32 far_clip = DisplaySceneFarClip(scene, instance_index, selected_clip);
                const i32 clip_result = NuCameraClipTestExtentsAxisAligned(&bounds.center, &bounds.extent, far_clip);

                if (clip_result != 0 && scene->alpha_values != NULL) {
                    scene->alpha_values[selected_clip] = DisplaySceneDistanceFade(scene, instance_index, distance_sqr);
                }
                DisplaySceneSetClipResult(scene, selected_clip, clip_result);
            }

            clip_index = DisplaySceneNextInstance(scene, clip_index);
            ++instance_index;
        }
    }

    void NuDisplaySceneRndr(void *display_scene) {
        NUDLDLISTSCENE *scene = static_cast<NUDLDLISTSCENE *>(display_scene);
        if ((scene->flags & NUDL_SCENE_FLAG_CLIPPING) != 0) {
            return;
        }

        RndrStateCopyGlobalState(scene->local_state);
        if (scene->nclip_objects != 0) {
            scene->flags |= NUDL_SCENE_FLAG_CLIPPING;
            DisplaySceneEvaluateClipFallback(scene);
        }
        DisplayListGenerateTransforms(scene);

        if ((scene->instance_visibility_enabled & NUDL_SCENE_INSTANCE_VISIBILITY_ENABLED) == 0 &&
            noscenespecials == 0 && scene->nspecials > 0) {
            NUGSCN temporary_scene = {};
            NUGSCN *gscene = scene->gscene;
            if (gscene == NULL) {
                temporary_scene.display_list = scene;
                gscene = &temporary_scene;
            }

            nuhspecial_s handle = {gscene, NULL, NULL};
            NuDisplaySpecialLayout *special = static_cast<NuDisplaySpecialLayout *>(scene->specials);
            for (i32 i = 0; i < scene->nspecials; ++i, ++special) {
                if ((special->flags & 2) != 0) {
                    handle.display_special = reinterpret_cast<NUDISPLAYSPECIAL_s *>(special);
                    NUMTX *draw_mtx = special->draw_mtx_ptr;
                    if (draw_mtx == NULL || draw_mtx == reinterpret_cast<NUMTX *>(-1)) {
                        draw_mtx = &special->draw_mtx;
                    }
                    NuDisplayListRndrSpecial(reinterpret_cast<nuhspecial_s *>(&handle), draw_mtx, 0, NULL, NULL);
                }
            }
        }
    }
}

void SetCameraZoom(f32 zoom) {
    CameraZoom = zoom;
}

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta) {
    if (gscn->instance_animation_data == NULL) {
        return;
    }

    // The lookup array is reference-counted by the allocation word immediately
    // before its first entry.  A sole owner means the optional table was not
    // populated and the original runtime discards it here.
    if (gscn->animation_end_frames != NULL && reinterpret_cast<i32 *>(gscn->animation_end_frames)[-1] <= 1) {
        gscn->animation_end_frames = NULL;
    }

    if (gscn->num_instance_animations <= 0) {
        return;
    }

    NUDISPLAYSPECIAL *display_specials = static_cast<NUDISPLAYSPECIAL *>(gscn->display_list->specials);
    nuinstanim_s *instance_animation = gscn->instance_animations;
    NUMTX *animation_matrix = gscn->instance_animation_matrices;

    for (i32 i = 0; i < gscn->num_instance_animations; ++i, ++instance_animation, ++animation_matrix) {
        if (instance_animation->anim_ix >= gscn->num_instance_animation_data) {
            continue;
        }

        NUDISPLAYSPECIAL *display_special = &display_specials[instance_animation->instance_ix];
        nuanimendlookup_s *state_animation = NULL;
        nuanimdata_s *animation = NULL;

        if ((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
            gscn->animation_end_frames != NULL) {
            const u16 lookup_index = instance_animation->end_frame_lookup_index;
            state_animation = &gscn->animation_end_frames[lookup_index - 1];
            animation = gscn->instance_animation_data[instance_animation->anim_ix];
        } else {
            if ((display_special->flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                continue;
            }
            animation = gscn->instance_animation_data[instance_animation->anim_ix];
            if (animation == NULL) {
                continue;
            }
        }

        f32 end_frame;
        if (animation != NULL) {
            end_frame = NuAnimEndFrameOld(animation);
        } else if (state_animation != NULL) {
            end_frame = static_cast<f32>(state_animation->end_frame);
        } else {
            continue;
        }

        if (end_frame == 0.0f) {
            continue;
        }

        f32 frame;
        u32 flags = static_cast<u32>(instance_animation->flags);
        if ((flags & NUINSTANIM_FLAG_PLAYING) != 0) {
            instance_animation->ltime += frame_delta * instance_animation->tfactor;
            frame = instance_animation->ltime;
            bool waiting_for_start = false;

            if ((flags & NUINSTANIM_FLAG_WAITING) != 0) {
                if (frame < instance_animation->tfirst) {
                    waiting_for_start = true;
                } else {
                    flags &= ~NUINSTANIM_FLAG_WAITING;
                    instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(flags);
                    instance_animation->ltime -= instance_animation->tfirst - 1.0f;
                    frame = instance_animation->ltime;
                }
            }

            if (!waiting_for_start) {
                const f32 interval_end = instance_animation->tinterval + end_frame;
                if (frame >= interval_end) {
                    if ((flags & NUINSTANIM_FLAG_REPEATING) != 0) {
                        const f32 repeat_length = interval_end - 1.0f;
                        instance_animation->ltime -=
                            NuFloor((instance_animation->ltime - 1.0f) / repeat_length) * repeat_length;
                        frame = instance_animation->ltime;
                    } else {
                        flags &= ~NUINSTANIM_FLAG_PLAYING;
                        instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(flags);
                        instance_animation->ltime = end_frame;
                        frame = end_frame;
                    }
                } else if (frame > end_frame) {
                    frame = end_frame;
                } else if (frame < 1.0f) {
                    flags &= ~NUINSTANIM_FLAG_PLAYING;
                    instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(flags);
                    instance_animation->ltime = 1.0f;
                    frame = 1.0f;
                }

                if ((flags & NUINSTANIM_FLAG_BACKWARDS) != 0) {
                    frame = end_frame + 1.0f - frame;
                }
            }
        } else {
            frame = instance_animation->ltime;
        }

        if (frame != instance_animation->prev_eval_time) {
            if (state_animation != NULL) {
                u8 state_index =
                    static_cast<u8>((static_cast<u32>(instance_animation->flags) & NUINSTANIM_STATE_INDEX_MASK) >>
                                    NUINSTANIM_STATE_INDEX_SHIFT);
                char state_value;
                const bool state_changed = StateAnimEvaluate2(reinterpret_cast<StateAnim *>(state_animation),
                                                              &state_index, &state_value, frame);
                const u32 state_flags = (static_cast<u32>(instance_animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                                        (static_cast<u32>(state_index) << NUINSTANIM_STATE_INDEX_SHIFT);
                instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(state_flags);

                if (state_changed) {
                    if (instance_animation->instance_ix != 0xffff && state_value != 0) {
                        display_special->flags |= NUDISPLAYSPECIAL_FLAG_VISIBLE;
                    } else if (state_value == 0) {
                        if (instance_animation->instance_ix != 0xffff) {
                            display_special->flags &= ~NUDISPLAYSPECIAL_FLAG_VISIBLE;
                        }
                        animation = NULL;
                    }
                }
            }

            if (animation != NULL) {
                NuAnimData2CalcMatrix(animation, 0, frame, animation_matrix);
            }
            instance_animation->prev_eval_time = frame;
        }

        if (animation != NULL) {
            instance_animation->mtx = *animation_matrix;
            instance_animation->mtx.m30 += display_special->draw_mtx.m30;
            instance_animation->mtx.m31 += display_special->draw_mtx.m31;
            instance_animation->mtx.m32 += display_special->draw_mtx.m32;
        }
    }
}

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

static NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path, char *, char *scene_data) {
    NUGSCN *scene = reinterpret_cast<NUGSCN *>(scene_data);
    if (scene == NULL) {
        char converted_path[1033];
        NuFileExtConvert(converted_path, path);
        NUFILE file = NuFileOpen(converted_path, NUFILE_READ);
        if (file == 0) {
            return NULL;
        }

        i32 file_size = (i32)NuFileOpenSize(file);
        buf->addr = ALIGN(buf->addr, 0x20);
        i32 uploaded = NuGScnUploadGfxDataFromFilePS(buf, *buf_end, (i32)file);
        scene = (NUGSCN *)ALIGN(buf->addr, 0x20);
        buf->addr = (usize)((char *)scene + file_size - uploaded);
        NuFileRead(file, scene, file_size - uploaded);
        NuFileClose(file);
    }

    void **fixed = (void **)NuPtrBlockFix((char *)scene + 0x18);
    NUGSCN *fixed_scene = (NUGSCN *)*fixed;
    if (fixed_scene != NULL) {
        NuGScnCreatePS(fixed_scene, buf, buf_end);

        i32 *texture_ids = fixed_scene->texture_ids;
        NUNATIVETEX **textures = fixed_scene->textures;
        for (i32 i = 0; i < fixed_scene->ntextures; ++i) {
            if (textures[i]->ref_count < 0) {
                texture_ids[i] = 0xabcdabcd;
            } else {
                texture_ids[i] = NuTexCreateNative(textures[i], true);
            }
        }
        NuGScnFixupTIDs(fixed_scene);
        nutexanim_s *animations = static_cast<nutexanim_s *>(fixed_scene->texture_anims);
        u32 animation_texture_count = 0;
        for (i32 i = 0; i < fixed_scene->num_texture_anims; ++i) {
            u32 end = reinterpret_cast<usize>(animations[i].texture_ids) + animations[i].texture_count;
            if (animation_texture_count < end)
                animation_texture_count = end;
        }
        for (u32 i = 0; i < animation_texture_count; ++i) {
            fixed_scene->texture_anim_ids[i] = fixed_scene->texture_ids[fixed_scene->texture_anim_ids[i]];
        }
        for (i32 i = 0; i < fixed_scene->num_texture_anims; ++i) {
            nutexanim_s *anim = &animations[i];
            anim->flags = 0;
            anim->next = NULL;
            anim->previous = NULL;
            anim->texture_ids = fixed_scene->texture_anim_ids + reinterpret_cast<usize>(anim->texture_ids);
            anim->material = fixed_scene->mtls[reinterpret_cast<usize>(anim->material)];
            nutexanimprog_s *program = NuTexAnimProgFind(anim->program_name);
            anim->env = NuTexAnimEnvCreate(buf, anim->material, anim->texture_ids, program);
        }
        for (i32 i = 0; i < fixed_scene->num_texture_anims - 1; ++i) {
            animations[i].next = &animations[i + 1];
            animations[i + 1].previous = &animations[i];
        }
        NuTexAnimAddList(animations);
        if (fixed_scene->display_list != NULL) {
            NuDisplaySceneAdd(reinterpret_cast<NUDLDLISTSCENE *>(fixed_scene->display_list));
        }
        NuGScnFixupPS(fixed_scene);
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x40) = 0;
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x44) = 0;
    }
    return fixed_scene;
}

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
    NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path) {
        RemoveDirectionalMaps = 1;
        RemoveNormalMaps = 1;
        NUGSCN *scene = NuReadGraphicsData(buf, &buf_end, path, NULL, NULL);
        RemoveNormalMaps = 0;
        RemoveDirectionalMaps = 0;
        return scene;
    }
    void NuGScnReadFromMemory(NUGSCN *scene) {
        NuReadGraphicsData(NULL, NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }
    NUGSCN *NuGHGFixup(NUGSCN *scene, void *) {
        return NuReadGraphicsData(NULL, NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }
    nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end) {
        nuapi.loading_hgobj = 1;
        nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(NuReadGraphicsData(buf, &buf_end, path, NULL, NULL));
        if (object != NULL && nuapi.force_shadows_on_characters != 0 && object->display_list != NULL) {
            for (i32 i = 0; i < object->display_list->nspecials; ++i) {
                object->display_list->visibility_flags[i] |= 0x20;
            }
        }
        nuapi.loading_hgobj = 0;
        return object;
    }
} // extern "C"

i32 solid_cable;
i32 nsegments_drawn;
f32 slack_factor = 1.0f;
f32 cable_slack = 1.0f;
f32 nsegments_per_unit = 6.0f;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

static void DrawCableSegment(const NUVEC &start, const NUVEC &end, f32 sag) {
    if (solid_cable == 0) {
        NURND_VERTEX3D vertices[2] = {};
        vertices[0].position = start;
        vertices[1].position = end;
        vertices[0].colour = 0xff000000;
        vertices[1].colour = 0xff000000;
        NuRndrLine3d(vertices, SolidMtl3D, NULL);
    } else {
        NUVEC first = start;
        NUVEC second = end;
        DrawRopeSingle(&first, &second, 1.0f, ropemtl, sag, sag, 10.0f, 5.0f);
    }
    ++nsegments_drawn;
}

void DrawCables() {
    nsegments_drawn = 0;
    for (i32 cable_index = 0; cable_index < 8; ++cable_index) {
        CABLE_s &cable = cables[cable_index];
        if ((cable.flags_1e9 & 1) == 0 || cable.point_count < 2) {
            continue;
        }

        if ((cable.flags_1e9 & 4) != 0) {
            cable.slack += FRAMETIME * cable_slack * slack_factor;
            if (cable.slack > cable_slack) {
                cable.slack = cable_slack;
            }
        } else if (tow_length > 0.0f && cable.total_length < tow_length) {
            cable.slack = (1.0f - cable.total_length / tow_length) * cable_slack;
        } else {
            cable.slack = 0.0f;
        }

        const f32 first_y = cable.points[0].y;
        const f32 last_y = cable.points[cable.point_count - 1].y;
        f32 distance_along = 0.0f;
        for (i32 segment = 0; segment < cable.point_count - 1; ++segment) {
            const f32 segment_length = cable.segment_lengths[segment];
            i32 subdivisions = cable.slack == 0.0f ? 1 : static_cast<i32>(ceilf(segment_length * nsegments_per_unit));
            if (subdivisions < 1) {
                subdivisions = 1;
            }
            NUVEC previous = cable.points[segment];
            for (i32 subdivision = 0; subdivision < subdivisions; ++subdivision) {
                const f32 local_start = static_cast<f32>(subdivision) / static_cast<f32>(subdivisions);
                const f32 local_end = static_cast<f32>(subdivision + 1) / static_cast<f32>(subdivisions);
                const f32 global_start = cable.total_length > 0.0f
                                             ? (distance_along + segment_length * local_start) / cable.total_length
                                             : 0.0f;
                const f32 global_end = cable.total_length > 0.0f
                                           ? (distance_along + segment_length * local_end) / cable.total_length
                                           : 0.0f;
                NUVEC start = {
                    cable.points[segment].x + (cable.points[segment + 1].x - cable.points[segment].x) * local_start,
                    first_y + (last_y - first_y) * global_start,
                    cable.points[segment].z + (cable.points[segment + 1].z - cable.points[segment].z) * local_start};
                NUVEC end = {
                    cable.points[segment].x + (cable.points[segment + 1].x - cable.points[segment].x) * local_end,
                    first_y + (last_y - first_y) * global_end,
                    cable.points[segment].z + (cable.points[segment + 1].z - cable.points[segment].z) * local_end};
                start.y -= cable.slack * NU_SIN_LUT(static_cast<i32>(global_start * 32768.0f));
                end.y -= cable.slack * NU_SIN_LUT(static_cast<i32>(global_end * 32768.0f));
                f32 ground = GameShadow(NULL, &start, 5.0f, -1);
                if (ground != 2000000.0f && start.y < ground + 0.1f) {
                    start.y = ground + 0.1f;
                }
                ground = GameShadow(NULL, &end, 5.0f, -1);
                if (ground != 2000000.0f && end.y < ground + 0.1f) {
                    end.y = ground + 0.1f;
                }
                DrawCableSegment(start, end, cable.slack);
                previous = end;
            }
            distance_along += segment_length;
        }
    }
}

void DrawRipple(ripple_node_s *node) {
    NURND_VERTEX3D vertices[4];
    vertices[0].position = {0.0f, node->size, 0.0f};
    vertices[1].position = {-node->size, 0.0f, 0.0f};
    vertices[2].position = {node->size, -0.0f, 0.0f};
    vertices[3].position = {-0.0f, -node->size, 0.0f};
    vertices[0].u = 1.0f;
    vertices[0].v = 0.0f;
    vertices[1].u = 0.0f;
    vertices[1].v = 0.0f;
    vertices[2].u = 1.0f;
    vertices[2].v = 1.0f;
    vertices[3].u = 1.0f;
    vertices[3].v = 0.0f;
    vertices[0].colour = node->color.value;
    vertices[1].colour = node->color.value;
    vertices[2].colour = node->color.value;
    vertices[3].colour = node->color.value;
    NUMTX matrix = node->matrix;
    NuRndrTriStrip3dClip(vertices, 4, &matrix, node->material);
}

void DrawAreaBox(nuvec_s *, nuvec_s *, i32, i32) {
    STUBBED();
}

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32) {
    STUBBED();
}

extern "C" {
    f32 aiEditor_DrawYOffset = 0.25f;
    f32 aiEditor_LocatorWidth = 0.25f;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour) {
    NURND_VERTEX3D vertices[2];
    vertices[0].colour = colour;
    vertices[1].colour = colour;
    NUVEC centre = *position;
    centre.y += aiEditor_DrawYOffset;
    NUVEC axis = {radius, 0.0f, 0.0f};
    NuVecRotateY(&axis, &axis, rotation);
    NuVecAdd(&vertices[0].position, &centre, &axis);
    NuVecSub(&vertices[1].position, &centre, &axis);
    AiRndrLine3d(vertices, NULL, NULL);
    vertices[0].position.x = centre.x;
    vertices[1].position.x = centre.x;
    vertices[0].position.y = centre.y - radius;
    vertices[0].position.z = centre.z;
    vertices[1].position.z = centre.z;
    vertices[1].position.y = centre.y + radius;
    AiRndrLine3d(vertices, NULL, NULL);
    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = radius;
    NuVecRotateY(&axis, &axis, rotation);
    NuVecAdd(&vertices[0].position, &centre, &axis);
    NuVecSub(&vertices[1].position, &centre, &axis);
    AiRndrLine3d(vertices, NULL, NULL);
    NUVEC arrow = {-axis.z * 0.2f, 0.0f, axis.x * 0.2f};
    NuVecAdd(&vertices[1].position, &centre, &arrow);
    AiRndrLine3d(vertices, NULL, NULL);
    NuVecSub(&vertices[1].position, &centre, &arrow);
    AiRndrLine3d(vertices, NULL, NULL);
}

void Draw_LOADED() {
    STUBBED();
}

void Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                  u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order) {
    if (object_index == -1) {
        return;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }
    if (world != NULL) {
        if (world->lev_objs[object_index].active != 0 && (scale_x != 0.0f || scale_y != 0.0f || scale_z != 0.0f)) {
            NUVEC scale = {scale_x, scale_y, scale_z};
            NUMTX matrix;
            NuMtxSetScale(&matrix, &scale);
            RotateGameMatrix(&matrix, rotate_order, x_rotation, y_rotation, z_rotation);
            NuMtxTranslate(&matrix, position);
            NuSpecialDrawAt(&world->lev_objs[object_index].special, &matrix);
        }
    }
}

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special) {
    WORLDINFO *world = WorldInfo_CurrentlyActive();
    const u16 character_spin = static_cast<u16>(drawcharicon_hspecial_spin);
    drawcharicon_hspecial_spin = 0;

    if (ObjTabList == NULL || world == NULL) {
        return;
    }

    nuhspecial_s special;
    if (drawcharicon_find != 0) {
        NuSpecialFind(things_scene, &special, ObjTabList[frame_object_id].name, 1);
    } else {
        special = world->lev_objs[frame_object_id].special;
    }

    if (NuSpecialExistsFn(&special) != 0) {
        DrawPanel3DObject(x, y, z + 1.0f, scale, scale, scale, 0, 0, 0, &special, 0, frame_alpha);
    }

    if (draw_character != 0) {
        if (override_special != NULL && NuSpecialExistsFn(override_special) != 0) {
            const f32 special_scale = scale * drawcharicon_hspecial_scale;
            DrawPanel3DObject(x, y, z + 1.0f + drawcharicon_hspecial_dz, special_scale, special_scale, special_scale, 0,
                              character_spin, 0, override_special, 0, character_alpha);
        } else {
            i32 icon_object_id;
            if (character_id == -1) {
                icon_object_id = LEGOOBJ_ICON_QUESTION;
            } else {
                icon_object_id = CDataList[character_id].field20_0x42;
                if (icon_object_id != -1 && GCDataList[character_id].field275_0x116 == 0 &&
                    icon_object_id != LEGOOBJ_ICON_WEIRDO) {
                    ++icon_object_id;
                }
            }

            if (icon_object_id != -1) {
                if (drawcharicon_find != 0) {
                    NUGSCN *icon_scene = character_id == -1 ? NULL : IconScene_FindById(character_id);
                    if (icon_scene != NULL) {
                        NuSpecialFind(icon_scene, &special, ObjTabList[icon_object_id].name, 1);
                    } else {
                        NuSpecialFind(things_scene, &special, ObjTabList[icon_object_id].name, 1);
                    }
                } else {
                    special = world->lev_objs[icon_object_id].special;
                }

                if (NuSpecialExistsFn(&special) != 0) {
                    DrawPanel3DObject(x, y, z + 1.0f, scale, scale, scale, 0, 0, 0, &special, 0, character_alpha);
                }
            }
        }
    }

    drawcharicon_find = 0;
    drawcharicon_i_panel = -1;
}

void DrawHint_LSW(HINT_s *, i32) {
    STUBBED();
}

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32) {
    STUBBED();
}

void DrawParallax(nuhspecial_s *special) {
    if (NuIOS_IsLowEndDevice() != 0 && WORLD != NULL && WORLD->current_level != NULL &&
        WORLD->current_level->data_display.far_clip < 20000.0f &&
        WORLD->current_level->data_display.fog_start < 20000.0f) {
        return;
    }

    const f32 uniform_scale = pNuCam->far_clip * 0.05f;
    NUVEC scale = {uniform_scale, uniform_scale, uniform_scale};
    NUMTX_ALIGNED16 matrix;
    NuMtxSetScale(&matrix, &scale);
    matrix.m30 = pNuCam->mtx.m30;
    matrix.m31 = pNuCam->mtx.m31;
    matrix.m32 = pNuCam->mtx.m32;
    NuRndrStateSetFogEnabled(0);
    NuSpecialDrawAt(special, &matrix);
    NuRndrStateSetFogEnabled(1);
}

void DrawQuestion(nuvec_s *position, float scale_value, float y_push) {
    if (position && NuSpecialExistsFn(&question)) {
        NUMTX_ALIGNED16 matrix;
        NuMtxSetIdentity(&matrix);
        NUVEC scale;
        scale.x = scale.y = scale.z = scale_value;
        NuMtxSetScale(&matrix, &scale);
        *reinterpret_cast<NUVEC *>(&matrix.m30) = *position;
        matrix.m31 += y_push;
        NuSpecialDrawAt(&question, &matrix);
    }
}

void DrawRectRGBA(float, float, float, float, u32, numtl_s *, i32, float) {
    STUBBED();
}

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);
// These rotations are also used by the shop translation unit. Keep them local
// so the compiler can inline the matrix arithmetic at each call site.
static inline void ShopRotateX(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m01 = m->m01;
    f32 m11 = m->m11;
    f32 m21 = m->m21;
    f32 m31 = m->m31;

    m->m01 = m01 * cosx - m->m02 * sinx;
    m->m02 = m01 * sinx + m->m02 * cosx;
    m->m11 = m11 * cosx - m->m12 * sinx;
    m->m12 = m11 * sinx + m->m12 * cosx;
    m->m21 = m21 * cosx - m->m22 * sinx;
    m->m22 = m21 * sinx + m->m22 * cosx;
    m->m31 = m31 * cosx - m->m32 * sinx;
    m->m32 = m31 * sinx + m->m32 * cosx;
}

static inline void ShopRotateY(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx + m->m02 * sinx;
    m->m02 = m->m02 * cosx - m00 * sinx;
    m->m10 = m10 * cosx + m->m12 * sinx;
    m->m12 = m->m12 * cosx - m10 * sinx;
    m->m20 = m20 * cosx + m->m22 * sinx;
    m->m22 = m->m22 * cosx - m20 * sinx;
    m->m30 = m30 * cosx + m->m32 * sinx;
    m->m32 = m->m32 * cosx - m30 * sinx;
}

static inline void ShopRotateZ(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx - m->m01 * sinx;
    m->m01 = m00 * sinx + m->m01 * cosx;
    m->m10 = m10 * cosx - m->m11 * sinx;
    m->m11 = m10 * sinx + m->m11 * cosx;
    m->m20 = m20 * cosx - m->m21 * sinx;
    m->m21 = m20 * sinx + m->m21 * cosx;
    m->m30 = m30 * cosx - m->m31 * sinx;
    m->m31 = m30 * sinx + m->m31 * cosx;
}
void Draw_LOADING() {
    STUBBED();
}

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale;                                                                                   \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale;                                                                                    \
        else                                                                                                           \
            brightness = (brightness - near_angle) * scale_delta / angle_delta + near_scale;                           \
        brightness *= parameters->intensity;                                                                           \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        i32 colour =                                                                                                   \
            RGBA_TO_NUCOLOUR32((MIN(255.0f, (MAX(0.0f, brightness)))), (MIN(255.0f, (MAX(0.0f, brightness)))),         \
                               (MIN(255.0f, (MAX(0.0f, brightness)))), 128);                                           \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);                 \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters) {
    f32 inv_col = 1.0f / (cols - 1);
    f32 inv_row = 1.0f / (rows - 1);
    f32 x0 = 0.0f * inv_row * 2.0f - 1.0f;
    f32 y_start = 0.0f * inv_col * 2.0f - 1.0f;
    f32 x1 = inv_row * 2.0f - 1.0f;
    f32 y_next = inv_col * 2.0f - 1.0f;
    f32 step_x = inv_row * 2.0f;
    f32 step_y = inv_col * 2.0f;
    f32 near_scale = parameters->near_scale * 128.0f;
    f32 far_scale = parameters->far_scale * 128.0f;
    f32 near_angle = parameters->near_angle / 180.0f;
    f32 far_angle = parameters->far_angle / 180.0f;
    f32 angle_delta = far_angle - near_angle;
    f32 scale_delta = far_scale - near_scale;
    NUCAMERA camera;
    NUVEC direction;
    f32 brightness;
    NuCameraGet(&camera);
    static i32 first = 1;
    static f32 camFov;
    static f32 adjacent;
    static i32 row, col;
    if (first) {
        camFov = camera.fov;
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        first = 0;
    }
    if (camera.fov != camFov) {
        adjacent = 1.0f / NU_TAN_LUT((i32)(camera.fov * 0.5f * 10430.3779296875f));
        // The original writes the saved FOV back into this camera copy.
        camera.fov = camFov;
    }
    camera.mtx.m30 = 0.0f;
    camera.mtx.m31 = 0.0f;
    camera.mtx.m32 = 0.0f;
    camera.mtx.m33 = 1.0f;
    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    for (row = 0; row < rows - 1; ++row) {
        NuPrim2DBegin(2, 5, NULL);
        f32 y0 = y_start;
        f32 y1 = y_next;
        for (col = 0; col < cols; ++col) {
            ALPHA_GRID_VERTEX(x0, y0);
            ALPHA_GRID_VERTEX(x1, y0);
            if (col != cols - 1) {
                ALPHA_GRID_VERTEX(x0, y0);
                ALPHA_GRID_VERTEX(x0, y1);
                ALPHA_GRID_VERTEX(x1, y0);
                ALPHA_GRID_VERTEX(x1, y1);
                ALPHA_GRID_VERTEX(x1, y0);
                ALPHA_GRID_VERTEX(x0, y1);
            }
            y0 += step_y;
            y1 += step_y;
        }
        NuPrim2DEnd();
        x0 += step_x;
        x1 += step_x;
    }
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
}
#undef ALPHA_GRID_VERTEX

void DrawArrow_Now(_vum_s *, float, i32, i32) {
    STUBBED();
}

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha) {
    const f32 y = -0.5f * position;
    char *title;
    i32 red, green;
    if (packet->new_best_time != 0.0f) {
        title = TTab[tNEWBESTTIME];
        red = 0;
        green = 255;
    } else {
        title = TTab[tNONEWBESTTIME];
        red = 255;
        green = 0;
    }
    SmartTextEx(title, 0.0f, 0.15f + y, 1.0f, 0.7f, 0.7f, 0.7f, 0, red, green, 0, 1.7f, 1, NULL, 0,
                static_cast<u8>(alpha));
    char time[256];
    char previous[256];
    Text_MakeTime(packet->new_best_time != 0.0f ? packet->new_best_time : packet->elapsed_time, 1, 1, 1, time);
    Text3DEx(time, 0.0f, 0.0f + y, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255, static_cast<u8>(alpha));
    Text_MakeTime(packet->previous_best_time, 1, 1, 1, time);
    NuStrCpy(previous, "(");
    NuStrCat(previous, time);
    NuStrCat(previous, ")");
    Text3DEx(previous, 0.0f, y - 0.15f, 1.0f, 0.7f, 0.7f, 0.7f, 0, 255, 255, 255,
             static_cast<u8>(static_cast<u8>(alpha) >> 1));
}

void DrawCross_Now(_vuv_s *, float, i32, i32) {
    STUBBED();
}

void DrawGameState(float x, float y, i32 highlight, i32 slot) {
    char game_name[64];
    if (slot == -1) {
        NuStrCpy(game_name, TTab[tCURRENTGAME]);
    } else {
        sprintf(game_name, "%s %i", TTab[tGAME], slot + 1);
    }

    u8 red = MENUNORMALR;
    u8 green = MENUNORMALG;
    u8 blue = MENUNORMALB;
    if (highlight != 0 && TestForController() != 0) {
        if (menu_pulsate > 0.0f) {
            red = static_cast<u8>(MENUFLASH0R * menu_pulsate + MENUFLASH1R * (1.0f - menu_pulsate));
            green = static_cast<u8>(MENUFLASH0G * menu_pulsate + MENUFLASH1G * (1.0f - menu_pulsate));
            blue = static_cast<u8>(MENUFLASH0B * menu_pulsate + MENUFLASH1B * (1.0f - menu_pulsate));
        } else if (menu_flash != 0) {
            red = MENUFLASH0R;
            green = MENUFLASH0G;
            blue = MENUFLASH0B;
        } else {
            red = MENUFLASH1R;
            green = MENUFLASH1G;
            blue = MENUFLASH1B;
        }
    } else if (menu_pulse > 0.0f) {
        red = static_cast<u8>(MENUFLASH0R * menu_pulse + MENUNORMALR * (1.0f - menu_pulse));
        green = static_cast<u8>(MENUFLASH0G * menu_pulse + MENUNORMALG * (1.0f - menu_pulse));
        blue = static_cast<u8>(MENUFLASH0B * menu_pulse + MENUNORMALB * (1.0f - menu_pulse));
    }
    SmartTextEx(game_name, x, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 4, red, green, blue, 0.45f, 1, NULL,
                0, MenuA);

    if (slot >= 0) {
        if (saveload_slotused[slot] != 0) {
            char progress[32];
            sprintf(progress, "%.1f%%", static_cast<f32>(saveload_slotcode[slot] * 100) / COMPLETIONPOINTS);
            Text_LocaliseDecimalPoint(progress);
            Text3DEx(progress, x, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 1, 255, 191, 0, MenuA);
        } else {
            char *state = TTab[saveload_freespace < SAVESIZE_ADDITIONAL ? tNOSPACE : tEMPTY];
            const u8 state_red = saveload_freespace < SAVESIZE_ADDITIONAL ? 255 : 0;
            SmartTextEx(state, x, y, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 1, state_red, 255 - state_red,
                        0, 0.45f, 1, NULL, 0, MenuA);
        }
    }
}

void DrawPauseFade() {
    if (editor_active != 0 || screendump != 0)
        return;

    f32 step = FRAMETIME * 2.0f;
    i32 fade;
    if (Paused == 0 && NetPaused == 0) {
        fade = static_cast<i32>(pause_fade - step);
        if (fade < 0)
            fade = 0;
    } else {
        fade = static_cast<i32>(pause_fade + step);
        if (fade > 0) {
            pause_fade = 1.0f;
            return;
        }
    }
    pause_fade = static_cast<f32>(fade);
}

void DrawRippleSet(ripple_set_s *set) {
    if (set == NULL)
        return;
    ripple_node_s *node = set->newest;
    for (i32 i = 0; i < set->active_count; ++i) {
        if (node != NULL) {
            DrawRipple(node);
            node = node->next;
        }
    }
}

void DrawSaveSlots(MENU_s *menu, float y) {
    DrawGameState(-0.5f, y, menu->selected_row == menu->first_row && menu->selected_column == 0, 0);
    menu->item_width[2] = text3d_width;
    menu->item_column[2] = 0;
    menu->item_y[2] = y;
    menu->item_x[2] = -0.5f;
    menu->item_height[2] = text3d_height * 2.0f;
    menu->item_row[2] = 0;

    DrawGameState(0.0f, y, menu->selected_row == menu->first_row && menu->selected_column == 1, 1);
    menu->item_width[3] = text3d_width;
    menu->item_column[3] = 1;
    menu->item_y[3] = y;
    menu->item_x[3] = 0.0f;
    menu->item_height[3] = text3d_height * 2.0f;
    menu->item_row[3] = 0;

    DrawGameState(0.5f, y, menu->selected_row == menu->first_row && menu->selected_column == 2, 2);
    menu->item_column[4] = 2;
    menu->item_y[4] = y;
    menu->item_row[4] = 0;
    menu->item_width[4] = text3d_width;
    menu->item_x[4] = 0.5f;
    menu->item_height[4] = text3d_height * 2.0f;
}

void DrawAlphaImage(i32, i32, numtl_s *, i32, NuBloomParameters *) {
    STUBBED();
}

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour) {
    static i32 bezier_draw_depth;

    const f32 camera_x = global_camera.mtx.m30;
    const f32 camera_y = global_camera.mtx.m31;
    const f32 camera_z = global_camera.mtx.m32;
    const f32 start_x = start.x - camera_x;
    const f32 start_y = start.y - camera_y;
    const f32 start_z = start.z - camera_z;
    const f32 end_x = end.x - camera_x;
    const f32 end_y = end.y - camera_y;
    const f32 end_z = end.z - camera_z;
    const f32 start_distance = start_x * start_x + start_y * start_y + start_z * start_z;
    const f32 end_distance = end_x * end_x + end_y * end_y + end_z * end_z;
    const f32 distance = end_distance < start_distance ? end_distance : start_distance;
    const f32 threshold = distance < 5.5f ? 0.0055f : distance * 0.001f;

    const f32 chord_x = end.x - start.x;
    const f32 chord_y = end.y - start.y;
    const f32 chord_z = end.z - start.z;
    if (chord_x * chord_x + chord_y * chord_y + chord_z * chord_z <= threshold || bezier_draw_depth > 15) {
        EdDrawLineSegment(start, end, colour);
        return;
    }

    ++bezier_draw_depth;
    VuVec first = {(start.x + start_control.x) * 0.5f, (start.y + start_control.y) * 0.5f,
                   (start.z + start_control.z) * 0.5f, 0.0f};
    VuVec second = {(start_control.x + end_control.x) * 0.5f, (start_control.y + end_control.y) * 0.5f,
                    (start_control.z + end_control.z) * 0.5f, 0.0f};
    VuVec third = {(end_control.x + end.x) * 0.5f, (end_control.y + end.y) * 0.5f, (end_control.z + end.z) * 0.5f,
                   0.0f};
    VuVec fourth = {(first.x + second.x) * 0.5f, (first.y + second.y) * 0.5f, (first.z + second.z) * 0.5f, 0.0f};
    VuVec fifth = {(second.x + third.x) * 0.5f, (second.y + third.y) * 0.5f, (second.z + third.z) * 0.5f, 0.0f};
    VuVec midpoint = {(fourth.x + fifth.x) * 0.5f, (fourth.y + fifth.y) * 0.5f, (fourth.z + fifth.z) * 0.5f, 0.0f};

    DrawBezierLine(start, first, midpoint, fourth, material, colour);
    DrawBezierLine(midpoint, fifth, end, third, material, colour);
    --bezier_draw_depth;
}

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32) {
    STUBBED();
}

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen) {
    if (CutBorderScale <= 0.0f) {
        return;
    }

    NuRndrBeginScene(-1);
    f32 border_scale = 0.1f;
    const CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    const bool active_cut_wide = cut != NULL && (cut->flags & 0x4000) != 0;
    bool single_cut_wide = false;
    if (WORLD->cutscene_sys != NULL && WORLD->cutscene_sys->count == 1 && WORLD->cutscene_sys->cuts[0] != NULL) {
        single_cut_wide = (WORLD->cutscene_sys->cuts[0]->flags & 0x4000) != 0;
    }
    if (active_cut_wide || single_cut_wide) {
        border_scale = 0.21276596f;
    }
    if (widescreen != 0) {
        border_scale = (border_scale * 4.0f - 0.5f) / 3.0f;
    }
    if (border_scale < 0.02f) {
        border_scale = 0.02f;
    }
    const i32 height =
        static_cast<i32>(border_scale * 3584.0f * NU_SIN_LUT(static_cast<i32>(CutBorderScale * 16384.0f)));
    NuRndrRect2di(0, 0, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrRect2di(0, 0xe00 - height, 0x2800, height, 0x80ffffff, FadeMtl);
    NuRndrEndScene();
}

void DrawExplosions() {
}

void DrawItemMenu2D() {
    if (0.0f < ShopNameAlpha) {
        i16 text_id;
        if (picked == 0) {
            text_id = tHINTS;
        } else if (picked == 1) {
            text_id = tCHARACTERS;
        } else if (picked == 2) {
            text_id = tEXTRAS;
        } else if (picked == 4) {
            text_id = tGOLDBRICKS;
        } else if (picked == 5) {
            text_id = tSTORYCLIPS;
        } else {
            text_id = tENTERCODE;
        }

        const i32 alpha = static_cast<i32>(ShopNameAlpha * 128.0f);
        const f32 y = (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f;
        SmartTextEx(TTab[text_id], 0.0f, y, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                    static_cast<u32>(alpha));
    }
}

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment) {
    static NUMTX status_mtx;

    NUVEC origin = {0.0f, 0.0f, 800.0f};
    NUVEC position = {x * 350.0f, y * 300.0f, 0.0f};
    u16 encoded[512];

    NuQFntSetJustifiedTolerances(1.2f, 1.2f);
    NuMtxSetIdentity(&status_mtx);
    NuMtxSetRotationX(&status_mtx, 0);
    NuMtxRotateY(&status_mtx, 0);
    NuMtxRotateZ(&status_mtx, angle);
    NuMtxTranslate(&status_mtx, &origin);
    NuMtxTranslate(&status_mtx, &position);
    NuMtxMul(&status_mtx, &status_mtx, NuCameraGetMtx());

    NuQFntSet(QFont3DZ);
    NuQFntSetMtx(QFont3DZ, &status_mtx);
    NuQFntPushPrintMode(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_ABSOLUTE);
    NuQFntSetColour(QFont3DZ, colour);
    NuQFntSetScale(QFont3DZ, scale, scale);
    Text3DStringEncode(text, encoded);

    if (alignment == 2) {
        const f32 height = NuQFntHeight(QFont3DZ);
        NuQFntMove(QFont3DZ, 0.0f, -height * 0.5f, 0.0f);
    } else if (alignment == 8) {
        const f32 height = NuQFntHeight(QFont3DZ);
        const f32 width = NuQFntPrintLenW(QFont3DZ, encoded);
        NuQFntMove(QFont3DZ, -width, -height * 0.5f, 0.0f);
    } else {
        const f32 height = NuQFntHeight(QFont3DZ);
        const f32 width = NuQFntPrintLenW(QFont3DZ, encoded);
        NuQFntMove(QFont3DZ, -width * 0.5f, -height * 0.5f, 0.0f);
    }
    NuQFntPrintW(QFont3DZ, encoded);
    NuQFntPopPrintMode();
    NuQFntSetCoordinateSystem(NUQFNT_CSMODE_NORMALISED);
}

void Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx) {
    if (object_index == -1) {
        return;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
        if (world == NULL) {
            return;
        }
    }
    LEVEL_OBJECT_RUNTIME &object = world->lev_objs[object_index];
    if (object.active != 0) {
        NuSpecialDrawAt(&object.special, mtx);
    }
}

void DrawGameObjects() {
    const f32 saved_far_clip = global_camera.unknown_64;
    const f32 object_far_clip = VehicleArea != 0 ? 50.0f : 20.0f;
    if (object_far_clip <= global_camera.unknown_64) {
        global_camera.unknown_64 = object_far_clip;
    }

    NuCameraSet(&global_camera);
    DrawGameObjectsDraw(0);

    global_camera.unknown_64 = saved_far_clip;
    NuCameraSet(&global_camera);
}

void DrawPaintLights() {
    STUBBED();
}

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha) {
    f32 icon_alpha;
    if (status->player0_active == 0) {
        icon_alpha = 0.25f;
    } else {
        icon_alpha = 1.0f;
    }
    icon_alpha *= alpha;
    const f32 size = ICONSIZE;
    DrawCharIcon(static_cast<i16>(status->player0_model), -ICONX, y, 0.0f, size, 0xa5, icon_alpha, icon_alpha, 1, NULL);
}

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large) {
    (void)unused_label;
    float text_x_offset;
    float text_y_offset;

    // Retained from the original routine even though the result is unused.
    NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.5f);

    if (NuStrICmp(prompt, ">") == 0) {
        text_x_offset = 0.005f;
        text_y_offset = 0.0f;
    } else if (NuStrICmp(prompt, "<<") == 0) {
        text_x_offset = -0.01f;
        text_y_offset = 0.0f;
    } else if (NuStrICmp(prompt, "II") == 0) {
        text_x_offset = 0.0f;
        text_y_offset = -0.01f;
    } else {
        text_x_offset = 0.0f;
        text_y_offset = 0.0f;
    }

    const float zero = 0.0f;
    float pulse = 1.0f;
    if (TestForController() == 0) {
        const float elapsed_since_touch = GlobalTimer.time_elapsed - (LastTouchTime + zero);
        if (elapsed_since_touch > 4.0f) {
            const float phase = NuFmod(elapsed_since_touch, 4.0f);
            const i32 angle = static_cast<i32>(phase * 0.25f * 65536.0f);
            const float wave = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
            if (zero <= wave) {
                pulse = wave + 1.0f;
            }
        }
    }

    float icon_scale = ICONSIZE;
    if (hovered) {
        icon_scale *= 1.25f;
        DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                          &WORLD->lev_objs[0xa5].special, 0, 0.75f);
        pulse = 0.75f;
    } else if (large) {
        icon_scale *= 1.25f;
        DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                          &WORLD->lev_objs[0xa5].special, 0, 0.75f);
        pulse *= 0.6f;
    } else {
        DrawPanel3DObject(ICONX, STATSPOSY, 1.0f, icon_scale, icon_scale, icon_scale, 0, 0, 0,
                          &WORLD->lev_objs[0xa5].special, 0, 0.75f);
        pulse = 0.6f;
    }

    if (prompt != NULL) {
        Text3DEx(prompt, ICONX + text_x_offset * pulse, STATSPOSY + text_y_offset * pulse, 1.0f, pulse * 1.3f, pulse,
                 pulse, 0, 255, 255, 255, 0x60);
    }
}

void Draw_LOADFAILED() {
    STUBBED();
}

void DrawAreaCylinder(nuvec_s *, nuvec_s *, i32) {
    STUBBED();
}

void DrawCameraTarget(nuvec_s *) {
    STUBBED();
}

void DrawGameMessages() {
    struct RENDER_MESSAGE {
        char *text;
        char text_buffer[0x78];
        NUVEC position_a;
        NUVEC target_position;
        NUVEC position;
        NUVEC start_position;
        f32 field_0xac;
        f32 target_scale;
        f32 field_0xb4;
        f32 field_0xb8;
        f32 elapsed;
        f32 duration;
        f32 field_0xc4;
        f32 field_0xc8;
        u32 field_0xcc;
        f32 field_0xd0;
        f32 field_0xd4;
        u32 flags;
        u32 score;
        u16 field_0xe0;
        u16 field_0xe2;
        u16 field_0xe4;
        u16 icon;
        nuhspecial_s extra_special;
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
        u8 active;
        u8 field_0xf9;
        u8 field_0xfa;
        u8 field_0xfb;
        u8 field_0xfc;
        u8 field_0xfd;
        u8 field_0xfe;
        u8 field_0xff;
        u32 field_0x100;
        u32 field_0x104;
        void (*update_fn)(GAMEMESSAGE_s *);
        void (*draw_fn)(GAMEMESSAGE_s *, NUVEC *, f32);
        void (*end_fn)(GAMEMESSAGE_s *);
    };
    extern GAMEMESSAGE_s GameMessage[128];
    extern i32 DrawPanel3DObjectNoAlpha(float, float, float, float, float, float, u16, u16, u16, nuhspecial_s *, i32);

    auto draw_special = [](nuhspecial_s *special, RENDER_MESSAGE *message, f32 x, f32 y, f32 z, f32 scale, f32 alpha) {
        if (NuSpecialExistsFn(special) == 0) {
            return;
        }
        const u32 flags = message->flags;
        if ((flags & 0x20000) != 0) {
            DrawPanel3DObjectNoAlpha(x, y, z, scale, scale, scale, message->field_0xe0, message->field_0xe2,
                                     message->field_0xe4, special, 2);
        } else {
            DrawPanel3DObject(x, y, z, scale, scale, scale, message->field_0xe0, message->field_0xe2,
                              message->field_0xe4, special, 2, alpha);
        }
    };

    RENDER_MESSAGE *message = reinterpret_cast<RENDER_MESSAGE *>(GameMessage);
    RENDER_MESSAGE *end = message + 128;
    for (; message != end; ++message) {
        if (message->active == 0) {
            continue;
        }
        if (message->field_0xfb != 0 && (message->flags & 0x40000) == 0) {
            continue;
        }
        if (message->field_0xd0 > 0.0f) {
            continue;
        }
        if (message->elapsed >= message->duration && message->field_0xfa == 0) {
            continue;
        }

        f32 progress = message->elapsed / message->duration;
        if ((message->flags & 0x100) != 0) {
            progress = 1.0f - NU_SIN_LUT(static_cast<i32>(progress * 16384.0f + 16384.0f));
        } else if ((message->flags & 0x200) != 0) {
            progress = NU_SIN_LUT(static_cast<i32>(progress * 16384.0f));
        } else if ((message->flags & 0x400) != 0) {
            progress = NU_SIN_LUT(static_cast<i32>(progress * 32768.0f));
        } else if ((message->flags & 0x800) != 0) {
            progress = 1.0f - NU_SIN_LUT(static_cast<i32>(progress * 32768.0f));
        }

        NUVEC position = message->start_position;
        if (message->field_0xd4 != 0.0f && message->field_0xfb == 0) {
            const f32 limit = message->field_0xd4;
            if (position.x < -limit) {
                position.x = -limit;
            } else if (position.x > limit) {
                position.x = limit;
            }
            if (position.y < -limit) {
                position.y = -limit;
            } else if (position.y > limit) {
                position.y = limit;
            }
        }

        f32 scale = (message->flags & 5) == 5 ? message->field_0xb8 : message->field_0xb4;
        if ((message->flags & 8) != 0) {
            if (message->update_fn != NULL) {
                message->update_fn(reinterpret_cast<GAMEMESSAGE_s *>(message));
            }
            position.x = position.x + (message->target_position.x - position.x) * progress;
            position.y = position.y + (message->target_position.y - position.y) * progress;
            position.z = position.z + (message->target_position.z - position.z) * progress;
        }
        if ((message->flags & 0x20) != 0) {
            scale += (message->target_scale - scale) * progress;
        }
        position.x += message->field_0xc4;
        position.z += message->field_0xc8;

        if (message->draw_fn != NULL) {
            message->draw_fn(reinterpret_cast<GAMEMESSAGE_s *>(message), &position, scale);
            continue;
        }

        if (NuSpecialExistsFn(&message->extra_special) == 0) {
            char *text = message->text != NULL ? message->text : message->text_buffer;
            Text3DEx(text, position.x, position.y, position.z, scale, scale, scale, message->field_0xfc, message->red,
                     message->green, message->blue, message->alpha);
            continue;
        }
        draw_special(&message->extra_special, message, position.x, position.y, position.z, scale,
                     (message->flags & 0x10000) != 0 ? static_cast<f32>(message->alpha) / 128.0f : 1.0f);
        if (GameMsg_GetExtraObjFn != NULL) {
            nuhspecial_s *extra = GameMsg_GetExtraObjFn(reinterpret_cast<GAMEMESSAGE_s *>(message));
            if (extra != NULL) {
                draw_special(extra, message, position.x, position.y, position.z, scale,
                             (message->flags & 0x10000) != 0 ? static_cast<f32>(message->alpha) / 128.0f : 1.0f);
            }
        }
    }
}

f32 KITPOSX = -0.725f;
f32 KITPOSY = -0.7f;
f32 KITPOS2X = -1.275f;
f32 KITPOS2Y = -1.3f;
f32 PANEL_MINIKITSCALE = 0.25f;
f32 PANEL_REDBRICKSCALE = 0.4f;
f32 PANEL_MINIKITY = 0.03f;
f32 PANEL_MINIKITCOUNTSCALE = 0.5f;
f32 PANEL_MINIKITCOUNTY = -0.1f;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
i32 DRAWBGLOAD = 0;
u16 PowerUp_PanelYRot[2] = {0, 0};
f32 PowerUp_PanelPosMul[2] = {0.0f, 0.0f};
f32 POWERUPOBJSIZE = 0.0f;
f32 REDBRICKPOSX = 0.0f;
f32 REDBRICKPOSY = -0.5f;
f32 REDBRICKPOS2X = 1.25f;
f32 REDBRICKPOS2Y = 0.0f;
i32 Arcade_Points[2] = {0};
f32 BOSSICONY = 0.0f;
i32 FPSDISPLAY = 0;
i32 ShowPlayerCoordinate = 0;
i32 LEGOOBJ_CHARKIT = -1;
i32 LEGOOBJ_MINIKIT = -1;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum) {
    const i32 model = ChallengeMode != 0 ? LEGOOBJ_CHARKIT : LEGOOBJ_MINIKIT;
    if (model == -1 || position <= 0.0f)
        return;
    const f32 blend = NuTrigTable[(static_cast<i32>(position * 16384.0f) >> 1) & 0x7fff];
    const f32 x = (KITPOSX - KITPOS2X) * blend + KITPOS2X;
    const f32 y = (KITPOSY - KITPOS2Y) * blend + KITPOS2Y;
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (world->lev_objs[model].active != 0) {
        const u16 rotation = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
        const u16 tilt = static_cast<u16>(1820.0f * NuTrigTable[rotation & 0x7fff]);
        const f32 size = scale * PANEL_MINIKITSCALE;
        DrawPanel3DObjectNoAlpha(x, PANEL_MINIKITY + y, 1.0f, size, size, size, tilt, rotation, 0,
                                 &world->lev_objs[model].special, 2);
    }
    char text[32];
    sprintf(text, "%i/%i", count, maximum);
    const f32 size = scale * PANEL_MINIKITCOUNTSCALE;
    Text3D(text, x, y + PANEL_MINIKITCOUNTY, 1.0f, size, size, size, 0, 255, 0, 127);
}

void DrawStatusBG_LSW(STATUSPACKET_s *status) {
    STATUS_STAGE_s *stage = status->stage;
    if (stage->field_0x14 == -1) {
        stage->field_0x18 = 0;
        stage->field_0x14 = 0;
    }
}

void Draw_LOADCORRUPT() {
    STUBBED();
}

void Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                       u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha) {
    if (object_index == -1) {
        return;
    }
    if (world == NULL) {
        world = WorldInfo_CurrentlyActive();
    }
    if (world != NULL) {
        if (world->lev_objs[object_index].active != 0 && (scale_x != 0.0f || scale_y != 0.0f || scale_z != 0.0f)) {
            NUVEC scale = {scale_x, scale_y, scale_z};
            NUMTX matrix;
            NuMtxSetScale(&matrix, &scale);
            RotateGameMatrix(&matrix, rotate_order, x_rotation, y_rotation, z_rotation);
            NuMtxTranslate(&matrix, position);
            NuSpecialDrawAtAlpha(&world->lev_objs[object_index].special, &matrix, alpha);
        }
    }
}

void DrawCameraTarget2(nuvec_s *) {
    STUBBED();
}

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha) {
    if (alpha > 0.0f && special != NULL && NuSpecialExistsFn(special) != 0 &&
        (scale_y != 0.0f || scale_x != 0.0f || scale_z != 0.0f)) {
        NUVEC scale = {scale_x / CameraZoom, scale_y / CameraZoom, scale_z / CameraZoom};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        RotateGameMatrix(&matrix, rotate_order, rotate_x, rotate_y, rotate_z);
        matrix.m30 = x * PANEL3DMULX;
        matrix.m31 = y * PANEL3DMULY;
        matrix.m32 = z;
        NuMtxMulVU0(&matrix, &matrix, NuCameraGetMtx());
        NuSpecialDrawAtAlpha(special, &matrix, alpha);
    }
    return 0;
}

void DrawStatusMiniKit(float, float, float, float, float, i32, STATUSPACKET_s *, float) {
    STUBBED();
}

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D() {
    shopitem_s *items;
    i32 *ids;
    if (picked == 0) {
        items = HintItems;
        ids = HintShelfIds;
    } else if (picked == 1) {
        items = CharItems;
        ids = CharShelfIds;
    } else if (picked == 2) {
        items = ExtraItems;
        ids = ExtraShelfIds;
    } else if (picked == 4) {
        items = BrickItems;
        ids = BrickShelfIds;
    } else if (picked == 5) {
        items = CutItems;
        ids = CutShelfIds;
    } else {
        return;
    }
    if (!items)
        return;
    char title[128], subtitle[128];
    char *name = items[ids[3]].name;
    i32 buyable = 0;
    i32 show_name = 0;
    const u16 id = items[ids[3]].item_id;
    switch (items[ids[3]].type) {
        case 1: {
            if (items[ids[3]].unlocked == 1)
                show_name = 1;
            else if (CollectIDUnlocked(id))
                buyable = show_name = 1;
            else {
                f32 scale = 0.7f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f, 1.0f, scale, scale,
                            scale, 0, 255, 0, 0, 1.7f, 1, NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
            break;
        }
        case 0: {
            buyable = items[ids[3]].unlocked != 1 && items[ids[3]].price != 0;
            break;
        }
        case 2: {
            NuStrCpy(subtitle, TTab[Cheat[id].text_id ? *Cheat[id].text_id : tUNKNOWN]);
            name = subtitle;
            i8 area = static_cast<i8>(Cheat[id].area);
            if (items[ids[3]].unlocked == 1)
                show_name = 1;
            else if (id <= 7 || area == -1 || Game.area_save[area].red_brick_collected)
                buyable = show_name = 1;
            else {
                sprintf(title, "%s %i", TTab[tPOWERBRICK], id - 7);
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                            static_cast<i32>(128.0f * ShopNameAlpha));
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
            break;
        }
        case 4: {
            sprintf(title, "%s %i", TTab[tGOLDBRICK], id + 1);
            name = title;
            if (items[ids[3]].unlocked == 1)
                show_name = 1;
            else if (!(static_cast<f32>(id * 3600) > Game.field30_0x7c2c))
                buyable = show_name = 1;
            else {
                SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                            static_cast<i32>(128.0f * ShopNameAlpha));
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
            break;
        }
        case 5: {
            i32 available = CutScenePlayer_CanStart(id);
            CutScenePlayer_GetText(id, title, subtitle, available);
            SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                        static_cast<i32>(128.0f * ShopNameAlpha));
            if (available && shopcutsceneplayer)
                SmartTextEx(subtitle, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0, 1.7f, 1, NULL,
                            0, static_cast<i32>(128.0f * ShopNameAlpha));
            else {
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
            break;
        }
    }
    u32 price;
    switch (items[ids[3]].type) {
        case 0:
            price = items[HintShelfIds[3]].price;
            break;
        case 2:
            price = items[ExtraShelfIds[3]].price;
            break;
        case 4:
            price = items[BrickShelfIds[3]].price;
            break;
        case 5:
            return;
        default:
            price = items[CharShelfIds[3]].price;
            break;
    }
    if (subitemselected > 0) {
        if (!buyable) {
            ShopLockedScale = 1.5f;
            GameAudio_PlaySfx(0x32, NULL, 0, 0);
            subitemselected = 0;
        } else if (Game.coins < price) {
            CoinTotalScale = 1.5f;
            PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
            subitemselected = 0;
        }
    }
    if (!(ShopNameAlpha > 0.0f))
        return;
    if (show_name) {
        f32 y = buyable ? HUB_EPISODETITLEY : (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f;
        f32 scale = buyable ? 0.6f : 0.8f;
        SmartTextEx(name, 0.0f, y, 1.0f, scale, scale, scale, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                    static_cast<i32>(128.0f * ShopNameAlpha));
    }
    if (buyable)
        CoinTotal_Draw(price, HUB_EPISODESUBTITLEY, 1.2f, 0, ShopNameAlpha, 255, 191, 0);
    if (subitemselected == 2) {
        f32 pulse = 1.0f;
        if (!TestForController()) {
            f32 since_touch = GlobalTimer.time_elapsed - (LastTouchTime + 2.0f);
            if (since_touch > 4.0f) {
                i32 angle = static_cast<i32>(NuFmod(since_touch, 4.0f) * 0.25f * 65536.0f);
                f32 value = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
                if (!(value < 0.0f))
                    pulse = value + 1.0f;
            }
        }
        f32 size = ICONSIZE * pulse;
        DrawPanel3DObject(-0.3f, -0.3f, 1.0f, size, size, size, 0, 0, 0, &WORLD->lev_objs[165].special, 0,
                          ShopNameAlpha);
        DrawPanel3DObject(0.3f, -0.3f, 1.0f, size, size, size, 0, 0, 0, &WORLD->lev_objs[165].special, 0,
                          ShopNameAlpha);
        f32 text_size = pulse * 0.6f;
        f32 alpha = static_cast<i32>(128.0f * ShopNameAlpha);
        u8 text_alpha = static_cast<i32>(alpha);
        Text3DEx("$", -0.3f, -0.3f, 1.0f, 1.3f * text_size, text_size, text_size, 0, 0, 255, 0, text_alpha);
        Text3DEx("X", 0.3f, -0.31f, 1.0f, 1.3f * text_size, text_size, text_size, 0, 255, 0, 0, text_alpha);
        MENU_s *menu = &GameMenu[GameMenuLevel];
        menu->item_x[11] = -0.3f;
        menu->item_y[11] = -0.3f;
        menu->item_x[12] = 0.3f;
        menu->item_y[12] = -0.3f;
        menu->item_width[11] = size;
        menu->item_column[11] = 0;
        menu->item_row[11] = 2;
        menu->item_column[12] = 1;
        menu->item_width[12] = size;
        menu->item_row[12] = 2;
    }
}

void DrawSubItemMenu3D() {
    DrawSubItems();
}

void Draw_NOMEMORYCARD() {
    STUBBED();
}

void DrawFadeScreenWipe() {
    NuRndrBeginScene(-1);
    extern numtl_s *SolidMtl;

    // The original routine unconditionally dereferences the shared fade
    // pointer after beginning a scene.  `pFadeInfo` is installed by
    // LoadPermData and points at FadeSys; retaining that indirection keeps
    // this call ABI-identical to the original.
    FadeSystem &fade_info = *pFadeInfo;
    const f32 fade_amount = fade_info.fade;
    const u32 direction = fade_info.direction;
    i32 gradient[4];
    i32 solid_x = 0;
    i32 solid_y = 0;
    i32 solid_width = 10240;
    i32 solid_height = 3584;

    if ((direction & 3) != 0) {
        const bool positive = (direction & 1) != 0;
        if ((positive && fade_info.rate > 0.0f) || (!positive && fade_info.rate <= 0.0f)) {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = 0;
            solid_width = static_cast<i32>(fade_amount * 10240.0f);
            NuRndrGradRect2di(solid_width, 0, 1024, 3584, gradient, FadeMtl2);
        } else {
            gradient[0] = 0;
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = static_cast<i32>(0x80000000u);
            const i32 edge = static_cast<i32>((1.0f - fade_amount) * 10240.0f);
            NuRndrGradRect2di(edge - 1024, 0, 1024, 3584, gradient, FadeMtl2);
            solid_x = edge;
            solid_width = 10240 - edge;
        }
    } else if ((direction & 0xc) != 0) {
        // The vertical sign test is the same two-way rate/direction test as
        // the horizontal one, with bit 2 selecting the opposite side.
        const bool edge_first = (direction & 4) != 0 ? fade_info.rate <= 0.0f : fade_info.rate > 0.0f;
        if (edge_first) {
            gradient[0] = static_cast<i32>(0x80000000u);
            gradient[1] = static_cast<i32>(0x80000000u);
            gradient[2] = 0;
            gradient[3] = 0;
            const i32 edge = static_cast<i32>((1.0f - fade_amount) * 3584.0f);
            NuRndrGradRect2di(0, edge - 358, 10240, 358, gradient, FadeMtl2);
            solid_y = edge;
            solid_height = 3584 - edge;
        } else {
            gradient[0] = 0;
            gradient[1] = 0;
            gradient[2] = static_cast<i32>(0x80000000u);
            gradient[3] = static_cast<i32>(0x80000000u);
            solid_height = static_cast<i32>(fade_amount * 3584.0f);
            NuRndrGradRect2di(0, solid_height, 10240, 358, gradient, FadeMtl2);
        }
    }

    if (fade_amount >= 0.0f) {
        NuRndrRect2di(solid_x, solid_y, solid_width, solid_height, 0, SolidMtl);
    }
    NuRndrEndScene();
}

static inline void RotateForceGlowMatrix(NUMTX *matrix, i32 angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    const f32 x0 = matrix->m00;
    const f32 x1 = matrix->m10;
    const f32 x2 = matrix->m20;
    const f32 x3 = matrix->m30;
    matrix->m00 = x0 * cosine - matrix->m01 * sine;
    matrix->m01 = x0 * sine + matrix->m01 * cosine;
    matrix->m10 = x1 * cosine - matrix->m11 * sine;
    matrix->m11 = x1 * sine + matrix->m11 * cosine;
    matrix->m20 = x2 * cosine - matrix->m21 * sine;
    matrix->m21 = x2 * sine + matrix->m21 * cosine;
    matrix->m30 = x3 * cosine - matrix->m31 * sine;
    matrix->m31 = x3 * sine + matrix->m31 * cosine;
}

void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (model == -1 || world->lev_objs[model].active == 0) {
        return;
    }
    ResetShadowMapRendering();
    NUVEC_ALIGNED16 scale;
    NUVEC direction;
    NUVEC draw_position;
    NuVecSub(&direction, position, reinterpret_cast<NUVEC *>(&pNuCam->mtx.m30));
    const f32 distance = NuFsqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
    NuVecNorm(&direction, &direction);
    f32 offset = radius;
    if (radius < distance - 0.2f || (offset = distance - 0.2f, !(distance < 0.2f))) {
        NuVecScale(&direction, &direction, offset);
        NuVecSub(&draw_position, position, &direction);
        if (offset != 0.0f) {
            scale.x = scale.y = scale.z = radius * (distance - offset) / distance;
            goto draw_glow;
        }
    } else {
        NuVecScale(&direction, &direction, 0.0f);
        NuVecSub(&draw_position, position, &direction);
    }
    scale.x = scale.y = scale.z = radius;
draw_glow:
    NUMTX_ALIGNED16 matrix;
    NuMtxSetScale(&matrix, &scale);
    u16 angle = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 20.0f) / 20.0f * 65536.0f);
    RotateForceGlowMatrix(&matrix, -angle);
    NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
    NuMtxTranslate(&matrix, &draw_position);
    NuSpecialDrawAtAlpha(&WORLD->lev_objs[model].special, &matrix, alpha);
    if (WORLD->lev_objs[model + 1].active != 0) {
        NuMtxSetScale(&matrix, &scale);
        angle = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 16.777f) / 16.777f * 65536.0f);
        RotateForceGlowMatrix(&matrix, angle);
        NuMtxMulR(&matrix, &matrix, &GameCam->render_mtx);
        NuMtxTranslate(&matrix, &draw_position);
        NuSpecialDrawAtAlpha(&WORLD->lev_objs[model + 1].special, &matrix, alpha);
    }
    EnableShadowMapRendering(0);
}

void DrawGameObjectsDraw(i32) {
    extern f32 FORCEGLOWTIME;
    EnableShadowMapRendering(0);

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];

        if ((object->apiobj.field_0x1f4 & 0x800) != 0) {
            if (object->field_0xcc0 != NULL && object->field_0x7a5 == 0x3b) {
                object->apiobj.model_draw_result = object->field_0xcc0->apiobj.model_draw_result;
            }
            continue;
        }

        NUMTX *secondary_matrix = (object->field_0xefe & 2) != 0 ? &object->apiobj.field_0xf8 : NULL;
        NUMTX *tertiary_matrix = object->field_0x1088 != 0 ? &object->apiobj.field_0x138 : NULL;
        const i32 drawn = GameDrawCharacterModel(object->apiobj.character_model, &object->apiobj.anim_packet,
                                                 &object->apiobj.field_0xb8, secondary_matrix, tertiary_matrix,
                                                 &object->field_0x7f4, object, object->field_0x1054);

        object->apiobj.model_draw_result = static_cast<u8>(drawn);
        object->field_0xe24 =
            static_cast<u8>((object->field_0xe24 & ~8) | ((drawcharactermodel_locatorsupdated & 1) << 3));

        if (drawn != 0) {
            DrawParaphernalia(object);
        } else if (((object->field_0xe21 & 4) != 0 || object->character_context == 8) && object->field_0xd80 > 0.0f &&
                   object->field_0xd8c > 0.0f && WORLD->lev_objs[object->field_0xe1e].active != 0 &&
                   (object->apiobj.character_data->game_character->flags_090 & 0x400) == 0) {
            DrawForceGlowSprite(&object->force_glow_position, object->field_0xd8c, object->field_0xe1e,
                                object->field_0xd80 / FORCEGLOWTIME, object);
        }

        if (object->field_0x10b8 != NULL) {
            DrawSnakeBody(object);
        }

        object->apiobj.field_0x288 = 1;
        if (object->apiobj.model_draw_result != 0) {
            object->field_0xefe |= 4;
        }

        if (Paused == 0 && object->apiobj.character_data != NULL && object->apiobj.character_data->draw_fn != NULL) {
            object->apiobj.character_data->draw_fn(object);
        }
    }

    ResetShadowMapRendering();
}

void Draw_AUTOSAVECANCEL() {
    STUBBED();
}

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha) {
    if (alpha > 0.0f) {
        NUVEC scale = {1.0f / CameraZoom, 1.0f / CameraZoom, 1.0f / CameraZoom};
        NuMtxPreScale(matrix, &scale);
        if (special != NULL && NuSpecialExistsFn(special) != 0) {
            NuMtxMulVU0(matrix, matrix, NuCameraGetMtx());
            NuSpecialDrawAtAlpha(special, matrix, alpha);
        }
    }
}

void Draw_AUTOSAVEWARNING() {
    const f32 scale = MENUTEXTSCALE * 0.8f;

    if (memcard_drawasiconfn != NULL) {
        memcard_drawasiconfn();
    }

    const char *message = apitxt_SAVING;
    if (memcard_loadmessage_delay > 0.0f || memcard_loadresult_delay > 0.0f) {
        message = apitxt_LOADING;
    }

    MenuSmartTextEx(const_cast<char *>(message), AUTOSAVEICONX - AUTOSAVEICONSIZE * 1.8f, AUTOSAVEICONY, 1.0f, scale,
                    scale, scale, 8, MENUNORMALR, MENUNORMALG, MENUNORMALB, 1.2f, 1, NULL, 0, MenuA);
}

void Draw_NODATAAVAILABLE() {
    char message[1024];
    sprintf(message, apitxt_NODATAAVAILABLE, apiGameName);
    MenuSmartTextEx(message, 0.0f, -0.4f, 1.0f, MENUTEXTSCALE, MENUTEXTSCALE, MENUTEXTSCALE, 0, MENUNORMALR,
                    MENUNORMALG, MENUNORMALB, 1.5f, 3, NULL, 0, MenuA);
}

i32 dco_locatorposonly;
i32 dco_id = -1;
i32 dco_reflectaxis;
i32 dco_wearinghat;
u16 dco_prerotatez;
f32 dco_reflectcoord = 2000000.0f;
GAMECHARACTERDATA_s *dco_gcdata;
CHARACTERMODEL_s *dco_cmodel;
void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale) {
    i32 position_only = dco_locatorposonly;
    dco_locatorposonly = 0;
    u16 rotate_z = dco_prerotatez;
    dco_prerotatez = 0;
    i32 id, axis, hat;
    f32 plane;
    CHARACTERMODEL_s *model;
    if (object != NULL) {
        id = object->id;
        model = object->apiobj.character_model;
        axis = object->field_0x1087;
        plane = object->field_0x1020;
        hat = object->field_0x108e;
    } else {
        id = dco_id;
        if (id == -1 || dco_gcdata == NULL || dco_cmodel == NULL)
            return;
        model = dco_cmodel;
        axis = dco_reflectaxis;
        plane = dco_reflectcoord;
        hat = dco_wearinghat;
    }
    dco_gcdata = NULL;
    dco_cmodel = NULL;
    dco_id = -1;
    dco_reflectaxis = 0;
    dco_reflectcoord = 2000000.0f;
    dco_wearinghat = 0;
    if (world == NULL)
        world = WorldInfo_CurrentlyActive();
    if (object_id != -1) {
        if (!world->lev_objs[object_id].active)
            return;
    } else if (special == NULL || !NuSpecialExistsFn(special)) {
        return;
    }
    if (locator == -1 || model->points_of_interest[locator] == NULL)
        return;
    NUMTX matrix;
    if (position_only)
        NuMtxSetTranslation(&matrix, reinterpret_cast<NUVEC *>(&joints[locator].m30));
    else
        matrix = joints[locator];
    if (second_locator != -1 && model->points_of_interest[second_locator] != NULL) {
        NUVEC position = {matrix.m30, matrix.m31, matrix.m32};
        if (!position_only) {
            NUMTX first = matrix;
            NUMTX second = joints[second_locator];
            first.m30 = first.m31 = first.m32 = 0.0f;
            second.m30 = second.m31 = second.m32 = 0.0f;
            QuatInterpolateRotationMatrix(&matrix, &first, &second, 0.5f);
        }
        matrix.m30 = (position.x + joints[second_locator].m30) * 0.5f;
        matrix.m31 = (position.y + joints[second_locator].m31) * 0.5f;
        matrix.m32 = (position.z + joints[second_locator].m32) * 0.5f;
    }
    if (rotate_z != 0)
        NuMtxPreRotateZ(&matrix, rotate_z);
    if (object_id != -1 && CutSceneSys->field_04 == object_id && DisguiseAdjustFn != NULL) {
        NUVEC adjustment, offset;
        DisguiseAdjustFn(id, hat, &adjustment, &offset);
        if (adjustment.x != 1.0f || adjustment.y != 1.0f || adjustment.z != 1.0f)
            NuMtxPreScale(&matrix, &adjustment);
        if (offset.x != 0.0f || offset.y != 0.0f || offset.z != 0.0f)
            NuMtxPreTranslate(&matrix, &offset);
    }
    if (rotation != NULL) {
        NUVEC position = {matrix.m30, matrix.m31, matrix.m32};
        NuMtxMulR(&matrix, rotation, &matrix);
        matrix.m30 = position.x;
        matrix.m31 = position.y;
        matrix.m32 = position.z;
    }
    if (translation != NULL)
        NuMtxTranslate(&matrix, translation);
    i32 use_alpha = 0;
    if (object_id != -1) {
        special = &world->lev_objs[object_id].special;
        if (ObjTabList != NULL)
            use_alpha = ObjTabList[object_id].pad_01;
    }
    if (scale != 1.0f)
        NuMtxPreScaleU(&matrix, scale);
    if (use_alpha)
        NuSpecialDrawAtAlpha(special, &matrix, alpha);
    else
        NuSpecialDrawAt(special, &matrix);
    NUMTX reflected;
    if (reflect && MatrixReflection(&matrix, axis, plane, world->current_level->unknown_0cc, &reflected)) {
        NuRndrStartReflectionRender(0);
        if (use_alpha)
            NuSpecialDrawAtAlpha(special, &reflected, alpha);
        else
            NuSpecialDrawAt(special, &reflected);
        NuRndrEndReflectionRender();
    }
}

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
u8 CharClipToBlobShadows;

static inline void DrawSetRotationY(NUMTX *matrix, NUANG angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    NUMTX value = {cosine, 0.0f, -sine, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sine, 0.0f, cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    *matrix = value;
}

i32 DrawGameObjectsProcess() {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        object->apiobj.field_0x1f4 |= 0x800;
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
            continue;
        const u8 previous_draw_pending = object->apiobj.field_0x288 & 1;
        object->apiobj.field_0x288 = 0;
        object->field_0xe23 = (object->field_0xe23 & ~8) | (previous_draw_pending << 3);
        object->field_0xe24 &= ~8;
        if (object->apiobj.field_0x287 != 0 && (object->field_0x101c > 0.0f || object->field_0x1018 == 0.0f))
            continue;
        if ((object->field_0xe20 & 0x20) != 0 && object->character_context != 0x23 && object->character_context != 0x24)
            continue;
        if (object->character_context == 0x0f) {
            TELEPORT_s *teleport = static_cast<TELEPORT_s *>(object->field_0x788);
            if (object->field_0x7a3 == 1 && teleport != NULL && (teleport->flags & 4) == 0)
                continue;
        } else if (object->character_context == 0x39 || object->character_context == 0x3b) {
            continue;
        }
        if (CharClipToBlobShadows != 0 && WORLD->area != KAMINO_ADATA && WORLD->area != HOTHBATTLE_ADATA &&
            WORLD->area != DEATHSTARBATTLE2_ADATA && static_cast<i8>(object->field_0xf04) < 0 &&
            object->apiobj.field_0x27c == -1 && (object->field_0xefb & 8) == 0 &&
            object->ai_update_distance > static_cast<u8>(WORLD->current_level->blob_shadow_fade_far))
            continue;

        NUVEC scale;
        NUVEC position;
        position.x = object->apiobj.position.x;
        position.y = object->apiobj.position.y;
        if ((object->apiobj.field_0x1f4 & 0x100) != 0)
            position.y += object->character_bottom * object->apiobj.field_0xa8;
        position.z = object->apiobj.position.z;
        NuVecAdd(&position, &position, &object->render_offset);
        object->field_0xf01 = (object->field_0xf01 & ~1) | (object->apiobj.model_draw_result & 1);
        object->apiobj.model_draw_result = 1;
        scale.x = object->apiobj.field_0xa8;
        const f32 drop_scale = DropInOutScale(object);
        scale.x *= drop_scale;
        scale.y = scale.z = scale.x;
        NUMTX matrix __attribute__((aligned(16)));
        NUMTX shadow __attribute__((aligned(16)));
        NUMTX reflected __attribute__((aligned(16)));
        NUMTX orientation __attribute__((aligned(16)));
        NuMtxSetScale(&matrix, &scale);
        switch (object->field_0x1086) {
            case 0:
                DrawSetRotationY(&orientation, NUANG_180DEG);
                if (object->apiobj.pitch_angle != 0)
                    ShopRotateX(&orientation, object->apiobj.pitch_angle);
                if (object->apiobj.field_0x276 != 0)
                    ShopRotateY(&orientation, object->apiobj.field_0x276);
                if (object->apiobj.roll_angle != 0)
                    ShopRotateZ(&orientation, object->apiobj.roll_angle);
                NuMtxMulVU0(&matrix, &matrix, &orientation);
                NuMtxTranslate(&matrix, &position);
                break;
            case 1:
                DrawSetRotationY(&orientation, object->apiobj.field_0x276 + NUANG_180DEG);
                if (object->apiobj.pitch_angle != 0)
                    ShopRotateX(&orientation, object->apiobj.pitch_angle);
                if (object->apiobj.roll_angle != 0)
                    ShopRotateZ(&orientation, object->apiobj.roll_angle);
                NuMtxMulVU0(&matrix, &matrix, &orientation);
                NuMtxTranslate(&matrix, &position);
                break;
            default:
            case 2:
                DrawSetRotationY(&orientation, object->apiobj.field_0x276 + NUANG_180DEG);
                if (object->apiobj.roll_angle != 0)
                    ShopRotateZ(&orientation, object->apiobj.roll_angle);
                if (object->apiobj.pitch_angle != 0)
                    ShopRotateX(&orientation, object->apiobj.pitch_angle);
                NuMtxMulVU0(&matrix, &matrix, &orientation);
                NuMtxTranslate(&matrix, &position);
                break;
            case 3:
                DrawSetRotationY(&orientation, NUANG_180DEG);
                if (object->apiobj.roll_angle != 0)
                    ShopRotateZ(&orientation, object->apiobj.roll_angle);
                if (object->apiobj.pitch_angle != 0)
                    ShopRotateX(&orientation, object->apiobj.pitch_angle);
                if (object->apiobj.field_0x276 != 0)
                    ShopRotateY(&orientation, object->apiobj.field_0x276);
                NuMtxMulVU0(&matrix, &matrix, &orientation);
                NuMtxTranslate(&matrix, &position);
                break;
            case 4:
                matrix = object->vehicle_orientation;
                NuMtxPreScale(&matrix, &scale);
                NuMtxPreRotateY(&matrix, NUANG_180DEG);
                NuMtxTranslate(&matrix, &position);
                break;
            case 5:
                matrix = object->vehicle_orientation;
                NuMtxPreScale(&matrix, &scale);
                NuMtxPreRotateY(&matrix, NUANG_180DEG);
                break;
        }
        if (object->movement_lean_angle != 0) {
            i32 angle = object->movement_lean_angle;
            if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                angle = static_cast<i32>(angle * PodSprint_RollMul(object));
            NuMtxPreRotateZ(&matrix, angle);
        }
        if (object->secondary_lean_angle != 0)
            NuMtxPreRotateX(&matrix, object->secondary_lean_angle);
        if (object->tertiary_lean_angle != 0)
            NuMtxPreRotateY(&matrix, object->tertiary_lean_angle);
        ApplyExtraRotation(object, &matrix);
        if (object->field_0x1086 != 4 && (object->character_context == 0x23 || object->character_context == 0x24)) {
            const f32 distance =
                object->apiobj.character_data->collision_radius * object->apiobj.character_data->model_scale * 7.5f;
            f32 offset = 1.0f - drop_scale;
            if (object->character_context != 0x23)
                offset = -offset;
            offset *= distance;
            NUVEC displacement;
            NuVecMtxTransformVU0(&displacement, &v001, &orientation);
            NuVecScale(&displacement, &displacement, offset);
            NuVecAdd(reinterpret_cast<NUVEC *>(&matrix.m30), reinterpret_cast<NUVEC *>(&matrix.m30), &displacement);
        }
        if (object->use_model_origin <= 1 &&
            (object->id == id_ATST || object->id == id_ATST_LOWRES || object->id == id_ATAT))
            matrix.m31 += object->character_bottom * object->apiobj.field_0xa8;
        object->apiobj.field_0xb8 = matrix;
        if (((object->field_0xefa & 0x40) != 0 && WORLD->rooms_visible_ptr[object->room_id] == 0) ||
            (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f)) {
            object->apiobj.model_draw_result = 0;
            continue;
        }
        object->apiobj.field_0x1f4 ^= 0x800;
        object->field_0xefe &= ~2;
        object->field_0x1088 = 0;
        if (object->field_0x1087 != 0 && object->field_0x1020 != 2000000.0f &&
            static_cast<u32>(static_cast<u8>(WORLD->current_level->reflection_range)) > object->ai_update_distance) {
            object->field_0x1088 = MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                    WORLD->current_level->unknown_0cc, &reflected);
            object->apiobj.field_0x138 = reflected;
        }
        if (object->apiobj.field_0x287 == 0 && object->apiobj.field_0x218 != 2000000.0f &&
            (object->apiobj.field_0x1f4 & 0x40) != 0 &&
            object->apiobj.character_data->game_character->shadow_locators == 0) {
            NuMtxSetIdentity(&shadow);
            ShopRotateY(&shadow, object->apiobj.field_0x276 + NUANG_180DEG);
            if (object->field_0x1060 != 0)
                ShopRotateZ(&shadow, object->field_0x1060);
            if (object->field_0x105e != 0)
                ShopRotateX(&shadow, object->field_0x105e);
            shadow.m30 = object->apiobj.position.x;
            shadow.m31 = object->apiobj.field_0x218 + 0.005f;
            shadow.m32 = object->apiobj.position.z;
            object->apiobj.field_0xf8 = shadow;
            object->field_0xefe |= 2;
        }
    }
    return 0;
}

void DrawStatusTextFraction(i32, i32, float, float, u16, float, u32, float, float) {
    STUBBED();
}

void DrawGameMessage_Targets(GAMEMESSAGE_s *, nuvec_s *, float) {
    STUBBED();
}

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order) {
    if (special == NULL || NuSpecialExistsFn(special) == 0)
        return 0;
    if (scale_y == 0.0f && scale_x == 0.0f && scale_z == 0.0f)
        return 0;
    NUVEC scale = {scale_x / CameraZoom, scale_y / CameraZoom, scale_z / CameraZoom};
    NUMTX matrix;
    NuMtxSetScale(&matrix, &scale);
    RotateGameMatrix(&matrix, rotate_order, rotate_x, rotate_y, rotate_z);
    matrix.m30 = x * PANEL3DMULX;
    matrix.m31 = y * PANEL3DMULY;
    matrix.m32 = z;
    NuMtxMulVU0(&matrix, &matrix, NuCameraGetMtx());
    NuSpecialDrawAt(special, &matrix);
    return 0;
}

void DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *, numtx_s *) {
    STUBBED();
}

void Draw_OK(MENU_s *) {
    STUBBED();
}

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float, float y_push, u16 x_rot, u16 y_rot,
              u16 z_rot) {
    NUVEC scale;
    NUANGVEC rotation;
    NUMTX_ALIGNED16 matrix;
    if (position != NULL) {
        if (NuSpecialExistsFn(special) != 0) {
            rotation.x = x_rot;
            rotation.y = y_rot;
            rotation.z = z_rot;
            NuMtxSetRotateXYZVU0(&matrix, &rotation);

            scale.x = scale.y = scale.z = scale_value;
            NuMtxScaleVU0(&matrix, &scale);
            *reinterpret_cast<NUVEC *>(&matrix.m30) = *position;
            matrix.m31 += y_push;
            NuSpecialDrawAt(special, &matrix);
        }
    }
}

void DrawAABox(_vuv_s *, _vuv_s *, i32) {
    STUBBED();
}

void DrawArrow(nuhspecial_s *special, float scale_value) {
    if (NuSpecialExistsFn(special)) {
        NUVEC scale = {scale_value, scale_value, scale_value};
        NUMTX matrix = *NuSpecialGetDrawMtx(special);
        NuMtxPreScale(&matrix, &scale);
        NuSpecialDrawAt(special, &matrix);
    }
}

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour) {
    if (material != NULL) {
        colour = (((i32)(material->diffuse_color.b * 255.0f) & 255) << 16) |
                 (((i32)(material->diffuse_color.g * 255.0f) & 255) << 8) |
                 ((i32)(material->diffuse_color.r * 255.0f) & 255);
    }
    NuRndrLine3dDbg(centre->x - radius, centre->y, centre->z, centre->x + radius, centre->y, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y - radius, centre->z, centre->x, centre->y + radius, centre->z, colour);
    NuRndrLine3dDbg(centre->x, centre->y, centre->z - radius, centre->x, centre->y, centre->z + radius, colour);
}

void SwipeDecalRenderer::Process(float) {
    STUBBED();
}

void SwipeDecalRenderer::Render() {
    STUBBED();
}

SwipeDecalRenderer::SwipeDecalRenderer(TouchHolder &, i32, SwipeDecalRenderer::Style) {
    STUBBED();
}

static __used__ void PauseRenderOff() {
    STUBBED();
}

static __used__ i32 MatrixReflection_CanOverride() {
    i32 result = 1;
    if (WORLD->current_level == DEATHSTARESCAPEA_LDATA) {
        const i8 sock = GameCam->sock_position.location.sock;
        if (sock != 0) {
            result = sock == 3;
        }
    }
    return result;
}

static __used__ void DrawStarFighter(starfighter_s *) {
    STUBBED();
}

static void DrawWeapon_SetSabreObjects(GameObject_s *object, i32 red, i32 green, i32 blue, i32 purple, i32 *models,
                                       i32 *hilt) {
    if (!(red || green || blue || purple))
        goto blue_or_purple;

    if (object->id == id_DARTHMAUL) {
        *hilt = models[0] = 0x12;
    } else if (object->id == id_COUNTDOOKU && WORLD->lev_objs[0x13].active) {
        *hilt = models[0] = 0x13;
    } else {
        *hilt = models[0] = 0x11;
    }

    if (red) {
        if (object->apiobj.field_0x287 != 0)
            return;
        if (object->id == id_DARTHMAUL) {
            models[1] = 0x6d;
            models[2] = 0x6e;
            if (object->field_0xe22 & 8)
                models[3] = 0x6e;
        } else {
            models[1] = 0x65;
            models[2] = 0x66;
            if (object->field_0xe22 & 8)
                models[3] = 0x66;
        }
        return;
    }

    if (green) {
        if (object->apiobj.field_0x287 != 0)
            return;
        models[1] = 0x67;
        models[2] = 0x68;
        if (object->field_0xe22 & 8)
            models[3] = 0x68;
        return;
    }

blue_or_purple:
    if (blue) {
        if (object->apiobj.field_0x287 == 0) {
            models[1] = 0x69;
            models[2] = 0x6a;
            if (object->field_0xe22 & 8) {
                models[3] = 0x6a;
            }
        }
    } else if (purple && object->apiobj.field_0x287 == 0) {
        models[1] = 0x6b;
        models[2] = 0x6c;
        if (object->field_0xe22 & 8) {
            models[3] = 0x6c;
        }
    }
}

static void DrawWeapons(GameObject_s *object, i32 reflection, f32 weapon_scale) {
    i32 models[4] = {-1, -1, -1, -1};
    i32 hilt = -1;
    i32 reflected_models[4] = {-1, -1, -1, -1};
    if (weapon_scale <= 0.0f) {
        return;
    }
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (data->field_0x94 & 0x200000) {
        return;
    }
    bool sabre = false;
    u16 rotation = 0;
    for (i32 hand = 0; hand < 4; ++hand) {
        const i32 joint = data->weapon_joints[hand];
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            if (Cheat_IsOn(15) && (data->field275_0x116 == 8 || data->field275_0x116 == 1)) {
                models[0] = 0x59;
            } else {
                models[0] = data->weapon_model;
                if (models[0] == -1) {
                    if ((object->apiobj.character_data->model_flags & 0x90) == 0x80) {
                        models[0] = 0xd;
                    } else {
                        i32 color = data->field_0x117;
                        if (object->id == id_BOB) {
                            color = (object->field_0xefd & 2) ? 1 : 2;
                        } else if (AnakinGreenSabre(object)) {
                            color = 1;
                        }
                        if (color < 4) {
                            if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25)) {
                                color = 0;
                            } else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object)) {
                                color = 3;
                            } else if (color == 1 && object->id == id_GRIEVOUS && (hand == 0 || hand == 3)) {
                                color = 2;
                            }
                            DrawWeapon_SetSabreObjects(object, color == 0, color == 1, color == 2, color == 3, models,
                                                       &hilt);
                            sabre = true;
                        }
                    }
                }
            }
            if (data->weapon_model != -1 || models[0] == 0x59) {
                if (object->id == id_JANGOFETT) {
                    rotation = 0xd1c8;
                } else if (models[0] == 0x65 || models[0] == 0x67 || models[0] == 0x69 || models[0] == 0x6b) {
                    i32 color = (models[0] - 0x65) / 2;
                    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25)) {
                        color = 0;
                    } else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object)) {
                        color = 3;
                    }
                    DrawWeapon_SetSabreObjects(object, color == 0, color == 1, color == 2, color == 3, models, &hilt);
                    sabre = true;
                }
            }
            i32 count = 0;
            for (i32 part = 0; part < 4; ++part) {
                if (models[part] != -1) {
                    reflected_models[part] = LevelObject_GetReflection(models[part]);
                    if (part != 3) {
                        ++count;
                    }
                }
            }
            if (count != 0) {
                NUMTX blade_matrix = object->joint_matrices[joint];
                NUMTX hilt_matrix;
                NUVEC scale = {weapon_scale, weapon_scale, weapon_scale};
                if (hilt != -1) {
                    f32 hilt_scale = weapon_scale + weapon_scale;
                    if (hilt_scale > 1.0f) {
                        hilt_scale = 1.0f;
                    }
                    NUVEC hilt_scale_vec = {hilt_scale, hilt_scale, hilt_scale};
                    hilt_matrix = blade_matrix;
                    NuMtxPreScale(&hilt_matrix, &hilt_scale_vec);
                }
                if (rotation != 0) {
                    NuMtxPreRotateX(&blade_matrix, rotation);
                }
                NuMtxPreScale(&blade_matrix, &scale);
                const NUMTX saved_matrix = blade_matrix;
                for (i32 side = 0;; ++side) {
                    for (i32 part = 0; part < 4; ++part) {
                        if (models[part] == -1 || (side && models[part] == hilt)) {
                            continue;
                        }
                        if (part == 3) {
                            blade_matrix.m30 += object->weapon_trail_offset.x;
                            blade_matrix.m31 += object->weapon_trail_offset.y;
                            blade_matrix.m32 += object->weapon_trail_offset.z;
                        }
                        NUMTX *matrix = models[part] == hilt ? &hilt_matrix : &blade_matrix;
                        Draw3DObjectMtx(NULL, models[part], matrix);
                        if (reflection) {
                            i32 model = reflected_models[part];
                            if (model == -1 || !WORLD->lev_objs[model].active) {
                                model = models[part];
                            }
                            NUMTX reflected;
                            if (MatrixReflection(matrix, object->field_0x1087, object->field_0x1020,
                                                 WORLD->current_level->unknown_0cc, &reflected)) {
                                NuRndrStartReflectionRender(0);
                                Draw3DObjectMtx(NULL, model, &reflected);
                                NuRndrEndReflectionRender();
                            }
                        }
                    }
                    if (side || object->id != id_DARTHMAUL || object->apiobj.field_0x27c == -1 ||
                        (!Cheat_IsOn(25) && !Player_HasPurpleForce(object))) {
                        break;
                    }
                    blade_matrix = saved_matrix;
                    NUVEC reverse = {1.0f, -1.0f, 1.0f};
                    NuMtxPreScale(&blade_matrix, &reverse);
                }
            }
        }
        if (sabre && object->id != id_GRIEVOUS) {
            return;
        }
    }
}

static void DrawCharacterAttachments(GameObject_s *object, NUMTX *joint_matrices) {
    extern i16 id_BATMAN, id_GLIDEPACK, id_CATWOMAN, id_PENGUIN, id_WHIP, id_UMBRELLA;
    i32 character_id;
    ANIMPACKET_s animation;
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    if (suit != NULL && (suit->flags & 2) != 0) {
        if (object->id != id_BATMAN)
            return;
        character_id = id_GLIDEPACK;
        AnimPacket_MiniToFull(&object->mini_animation, &animation);
    } else if (object->id == id_CATWOMAN && (AnimPlaying(&object->apiobj.anim_packet, 0x51, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x55, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x56, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x94, 1, 1))) {
        character_id = id_WHIP;
        animation = object->apiobj.anim_packet;
        if (animation.blending != 0 && animation.blend_animation_b != 0x51 && animation.blend_animation_b != 0x55 &&
            animation.blend_animation_b != 0x56 && animation.blend_animation_b != 0x94)
            return;
    } else if (object->id == id_PENGUIN) {
        character_id = id_UMBRELLA;
        AnimPacket_MiniToFull(&object->mini_animation, &animation);
    } else {
        return;
    }
    if (character_id == -1)
        return;
    i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    CHARACTERANIM_s *configuration = static_cast<CHARACTERANIM_s *>(model->model_data_a[current_animation]);
    if (configuration != NULL && (configuration->misc_flags & 8) != 0)
        return;
    i32 locator = object->apiobj.character_data->game_character->extra_character_locator;
    if (locator == -1 || model->points_of_interest[locator] == NULL)
        return;
    i16 model_index = apicharsys->playermodelids[character_id];
    if (model_index == -1)
        return;
    NUMTX matrix = joint_matrices[locator];
    NUMTX reflected;
    NUMTX *reflection = NULL;
    if (object->field_0x1088 != 0 && object->field_0x1020 != 2000000.0f &&
        MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020, WORLD->current_level->unknown_0cc,
                         &reflected)) {
        reflection = &reflected;
    }
    GameDrawCharacterModel(&apicharsys->models[model_index], &animation, &matrix, NULL, reflection, NULL, NULL,
                           0xffffffff);
}

void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;

static void DrawParaphernalia(GameObject_s *object) {
    if (draw_para == 0)
        return;
    GAMECHARACTERDATA_s *config = object->apiobj.character_data->game_character;
    GameObject_s *carried = object->field_0xcc0;
    if (carried != NULL && object->field_0x7a5 != 0x3b && (config->flags_098[0] & 0x40) == 0) {
        carried->field_0x1088 = object->field_0x1088;
        carried->field_0x1020 = object->field_0x1020;
        carried->field_0x1087 = object->field_0x1087;
        i32 locator = config->ride_locator;
        NUMTX matrix, reflected, attachment_matrices[16];
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
            matrix = object->joint_matrices[locator];
        } else {
            // The original fallback does not initialize its temporary Z component.
            // Zero gives this otherwise undefined path a deterministic position.
            NUVEC position = {0.0f, object->apiobj.field_0x1e0, 0.0f};
            NuVecMtxRotate(&position, &position, &object->apiobj.field_0xb8);
            NuVecAdd(&position, &position, &object->apiobj.collision_position);
            matrix = object->apiobj.field_0xb8;
            matrix.m30 = position.x;
            matrix.m31 = position.y;
            matrix.m32 = position.z;
        }
        NUMTX *reflection = NULL;
        if (carried->field_0x1087 && carried->field_0x1020 != 2000000.0f &&
            static_cast<u8>(WORLD->current_level->reflection_range) > object->ai_update_distance &&
            MatrixReflection(&matrix, carried->field_0x1087, carried->field_0x1020, WORLD->current_level->unknown_0cc,
                             &reflected))
            reflection = &reflected;
        GAMECHARACTERDATA_s *carried_config = carried->apiobj.character_data->game_character;
        u32 layers = carried->id == id_ANAKINJEDISCARRED ? carried_config->layer_mask_special
                                                         : carried_config->layer_mask_medium;
        layers = AdjustLayerBits(layers, carried);
        if (GameDrawCharacterModel(carried->apiobj.character_model, &carried->apiobj.anim_packet, &matrix, NULL,
                                   reflection, attachment_matrices, NULL, layers)) {
            if (carried->field_0x108e)
                DrawObjectOnCharacter(WORLD, carried, carried->field_0x108e + 0xf9, NULL,
                                      carried_config->helmet_locator, -1, attachment_matrices, carried->field_0x1088,
                                      layers, NULL, NULL, 1.0f, 1.0f);
            if (Cheat_IsOn(2))
                DrawObjectOnCharacter(WORLD, carried, 0xe7, NULL, carried_config->head_locator, -1, attachment_matrices,
                                      carried->field_0x1088, layers, NULL, NULL, 1.0f, 1.0f);
            DrawCharacterAttachments(carried, attachment_matrices);
            if (carried->id == id_WEIRDO1 || carried->id == id_WEIRDO2)
                Customiser_DrawAccessories(CharacterCustomiser, carried, attachment_matrices);
        }
    } else if ((object->field_0xe24 & 1) && object->field_0x780 != NULL) {
        GameObject_s *passenger = static_cast<GameObject_s *>(object->field_0x780);
        i32 locator = config->weapon_joints[0];
        NUMTX matrix;
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
            matrix = object->joint_matrices[locator];
        } else {
            NUANGVEC rotation = {-0x4000, object->apiobj.field_0x276, 0};
            NuMtxSetRotationXYVU0(&matrix, &rotation);
            matrix.m30 = object->apiobj.position.x +
                         NuTrigTable[object->apiobj.field_0x276 >> 1] * object->apiobj.field_0x1dc * 1.75f;
            matrix.m31 = object->apiobj.collision_min.y +
                         (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.6f;
            matrix.m32 =
                object->apiobj.position.z + NuTrigTable[static_cast<u16>(object->apiobj.field_0x276 + 0x4000) >> 1] *
                                                object->apiobj.field_0x1dc * 1.75f;
        }
        GameDrawCharacterModel(passenger->apiobj.character_model, &passenger->apiobj.anim_packet, &matrix, NULL, NULL,
                               NULL, NULL, passenger->apiobj.character_data->game_character->layer_mask_medium);
    }
    config = object->apiobj.character_data->game_character;
    NUMTX *joints = object->joint_matrices;
    if ((config->flags_094[0] & 1) == 0) {
        if (object->apiobj.character_data->flags & 1) {
            i32 locator = config->thingy_locator;
            NUMTX matrix = object->apiobj.field_0xb8;
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL &&
                object->apiobj.model_draw_result)
                matrix = joints[locator];
            NUMTX reflected = object->apiobj.field_0x138;
            CharScene_Draw(WORLD, object->id, &matrix, object->field_0x1088 ? &reflected : NULL);
        } else if (object->id == id_CATAPULT && (object->field_0x7a5 != 0x0a || (object->context_flags & 0x40) == 0 ||
                                                 object->quick_shoot_bolt_id != -1 ||
                                                 (object->context_animation_timer < 0.5f &&
                                                  PickupFlickerFrame % PickUpFlickerFrames < PickUpFlickerTest))) {
            dco_prerotatez = 0xc000;
            DrawObjectOnCharacter(WORLD, object, 0xf0, NULL, config->weapon_shoot_joints[0], -1, joints, 0,
                                  object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
        }
    } else if (BonusArea && VehicleArea) {
        CharMiniKit_Draw(object->id, &object->apiobj.field_0xb8, object->field_0x1087, object->field_0x1020,
                         WORLD->current_level->unknown_0cc);
    }
    if (config->uses_weapon_action == 1 && AnimPlaying(&object->apiobj.anim_packet, 0x60, 1, 1)) {
        DrawObjectOnCharacter(WORLD, object, 9, NULL, config->weapon_joints[0], -1, joints, object->field_0x1088,
                              object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    } else if (object->weapon_scale > 0.0f) {
        DrawWeapons(object, object->field_0x1088, object->weapon_scale);
        if (object->timer_d50 > 0.0f) {
            dco_locatorposonly = 1;
            DrawObjectOnCharacter(WORLD, object, 0x35, NULL, config->weapon_shoot_joints[0], -1, joints,
                                  object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
        }
    } else {
        f32 scale = 0.0f;
        f32 *weapon_time = NULL;
        if (config->uses_weapon_action == 9 &&
            (weapon_time = AnimPlaying(&object->apiobj.anim_packet, 0x5e, 1, 0)) != NULL) {
            f32 frame = *weapon_time;
            if (frame >= 18.0f && frame <= 65.0f) {
                if (frame <= 27.0f)
                    scale = (frame - 18.0f) / 9.0f;
                else if (frame < 55.0f)
                    scale = 1.0f;
                else
                    scale = 1.0f - (frame - 55.0f) / 10.0f;
            }
        }
        DrawWeapons(object, object->field_0x1088, scale);
    }
    if (object->field_0xd24 > 0.0f || object->timer_d28 > 0.0f) {
        ResetShadowMapRendering();
        f32 scale = object->field_0xd24;
        bool show = scale > 0.0f;
        if (object->field_0xe37 == 0 && config->field_0xf5 != 0) {
            scale = 1.0f;
            show = !(object->timer_d28 > 0.0f && (GameTimer.update_count & 3) > 1);
        }
        if (show) {
            NUVEC scaling = {scale, scale, scale};
            NUMTX matrix, reflected;
            NuMtxSetScale(&matrix, &scaling);
            i32 locator = config->shield_locator;
            NUVEC *position = &object->apiobj.collision_position;
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL)
                position = reinterpret_cast<NUVEC *>(&joints[locator].m30);
            NuMtxTranslate(&matrix, position);
            Draw3DObjectMtx(NULL, 0x6f + (object->timer_d28 > 0.0f), &matrix);
            if (object->field_0x1088 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                         WORLD->current_level->unknown_0cc, &reflected)) {
                NuRndrStartReflectionRender(0);
                Draw3DObjectMtx(NULL, 0x92 + (object->timer_d28 > 0.0f), &reflected);
                NuRndrEndReflectionRender();
            }
        }
        EnableShadowMapRendering(0);
    }
    if (object->communicate_blend > 0.0f && config->thingy_locator != -1 &&
        object->apiobj.character_model->points_of_interest[config->thingy_locator] != NULL) {
        i32 model_id;
        NUMTX matrix, reflected;
        if (object->apiobj.character_data->model_flags & 0x40) {
            f32 scale = object->communicate_blend * object->apiobj.field_0xa8;
            NUVEC scaling = {scale, scale, scale};
            NuMtxSetScale(&matrix, &scaling);
            NuMtxRotateY(&matrix, static_cast<u16>(object->apiobj.field_0x276 + 0x8000));
            matrix.m30 = joints[config->thingy_locator].m30;
            matrix.m31 = joints[config->thingy_locator].m31;
            matrix.m32 = joints[config->thingy_locator].m32;
            if (object->apiobj.field_0x27f == 9 && object->apiobj.water_height > matrix.m31)
                matrix.m31 = object->apiobj.water_height;
            model_id = 0x19;
        } else {
            model_id = (config->flags_094[0] & 0x80) ? 0x0e : -1;
            matrix = joints[config->thingy_locator];
            if (object->communicate_blend != 1.0f) {
                NUVEC scaling = {object->communicate_blend, object->communicate_blend, object->communicate_blend};
                NuMtxPreScale(&matrix, &scaling);
            }
        }
        if (model_id != -1 && WORLD->lev_objs[model_id].active) {
            NuSpecialDrawAt(&WORLD->lev_objs[model_id].special, &matrix);
            i32 reflection_id = LevelObject_GetReflection(model_id);
            if (reflection_id != -1 && WORLD->lev_objs[reflection_id].active && object->field_0x1088 &&
                MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020, WORLD->current_level->unknown_0cc,
                                 &reflected)) {
                NuRndrStartReflectionRender(0);
                NuSpecialDrawAt(&WORLD->lev_objs[reflection_id].special, &reflected);
                NuRndrEndReflectionRender();
            }
        }
    }
    if (object->field_0x7a5 == 0x22) {
        if ((WORLD->area != NULL && WORLD->area == HUB_ADATA) || WORLD->current_level == BLOCKADERUNNERB_LDATA ||
            WORLD->current_level == DOOKUC_LDATA) {
            NUVEC position;
            position.x = object->apiobj.collision_position.x;
            position.y = object->character_bottom * object->apiobj.field_0xa8 + object->apiobj.position.y +
                         (object->character_top - object->character_bottom) * 0.5f * object->apiobj.field_0xa8;
            position.z = object->apiobj.collision_position.z;
            f32 radius = NuFmax(object->apiobj.field_0x1dc, object->apiobj.field_0x1e0) * 1.75f;
            f32 alpha;
            if (object->field_0x768 < 0.2f) {
                alpha = object->field_0x768 / 0.2f;
            } else if (object->context_animation_timer < 0.2f) {
                alpha = object->context_animation_timer / 0.2f;
            } else {
                alpha = 1.0f;
            }
            DrawForceGlowSprite(&position, radius, 0xdf, alpha, object);
        }
    } else if (object->field_0xd80 > 0.0f && object->field_0xd8c > 0.0f &&
               WORLD->lev_objs[object->field_0xe1e].active != 0 &&
               (object->apiobj.character_data->player_config->flags_090 & 0x400) == 0) {
        DrawForceGlowSprite(&object->force_glow_position, object->field_0xd8c, object->field_0xe1e,
                            object->field_0xd80 / FORCEGLOWTIME, object);
    }
    if (Cheat_IsOn(2))
        DrawObjectOnCharacter(WORLD, object, 0xe7, NULL, config->head_locator, -1, joints, object->field_0x1088,
                              object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    if (object->field_0x108e) {
        i32 locator = config->helmet_locator;
        if ((object->id == id_CHEWBACCA || object->id == id_WOOKIEE) && object->field_0x108e != 5 &&
            object->field_0x108e != 6)
            locator = 9;
        DrawObjectOnCharacter(WORLD, object, object->field_0x108e + 0xf9, NULL, locator, -1, joints,
                              object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    }
    if (object->apiobj.model_draw_result && config->thrust_locators && object->thrust_effect_scale > 0.0f &&
        WORLD->lev_objs[0x77].active && WORLD->lev_objs[0x78].active) {
        i32 effect_index = 0;
        for (i32 locator = 0; locator < 16; ++locator) {
            if ((config->thrust_locators & (1 << locator)) == 0 ||
                object->apiobj.character_model->points_of_interest[locator] == NULL)
                continue;
            f32 scale = object->thrust_effect_scale +
                        ((object->reserved_e27[effect_index++] / 255.0f - 0.5f) * 0.5f) * object->thrust_effect_scale;
            if (scale > 0.0f) {
                NUMTX matrix = joints[locator], reflected;
                NUVEC scaling = {scale, scale, scale};
                NuMtxPreScale(&matrix, &scaling);
                Draw3DObjectMtx(NULL, 0x77, &matrix);
                Draw3DObjectMtx(NULL, 0x78, &matrix);
                if (object->field_0x1088 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                             WORLD->current_level->unknown_0cc, &reflected)) {
                    i32 first = LevelObject_GetReflection(0x77);
                    if (!WORLD->lev_objs[first].active)
                        first = 0x77;
                    i32 second = LevelObject_GetReflection(0x78);
                    if (!WORLD->lev_objs[second].active)
                        second = 0x77;
                    NuRndrStartReflectionRender(0);
                    Draw3DObjectMtx(NULL, first, &matrix);
                    Draw3DObjectMtx(NULL, second, &matrix);
                    NuRndrEndReflectionRender();
                }
            }
        }
    }
    if (object->field_0x7a5 == 0x2d)
        GizDrawBuildItPiece(object, object->field_0x1088);
    f32 *placement_time = NULL;
    if ((object->field_0x7a5 == 0x48 || object->field_0x7a5 == 0x49) &&
        object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
        ((object->field_0x7a5 == 0x48 && object->field_0x7a3 == 0) ||
         (object->field_0x7a5 == 0x49 && object->field_0x7a3 != 0)) &&
        WORLD->lev_objs[0xec].active && config->place_locator != -1 &&
        object->apiobj.character_model->points_of_interest[config->place_locator] != NULL &&
        (placement_time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)) != NULL) {
        f32 animation_end = NuAnimEndFrame(object->apiobj.character_model->model_data_b[object->context_animation]);
        f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
        if (start < animation_end) {
            if (start < 1.0f)
                start = 1.0f;
            f32 frame = *placement_time;
            if (frame >= start) {
                f32 finish = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
                if (start < finish) {
                    f32 scale = 1.0f;
                    bool show = true;
                    if (object->field_0x7a5 == 0x48) {
                        if (finish < animation_end)
                            animation_end = finish;
                        if (frame < animation_end)
                            scale = (frame - start) / (animation_end - start);
                    } else {
                        f32 last = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
                        f32 decay_start = finish < animation_end ? finish : animation_end;
                        if (last < finish)
                            last = finish;
                        f32 decay_end = last < animation_end ? last : animation_end;
                        if (frame > decay_start) {
                            if (decay_end > frame && decay_end > decay_start)
                                scale = 1.0f - (frame - decay_start) / (decay_end - decay_start);
                            else
                                show = false;
                        }
                    }
                    if (show) {
                        NUMTX matrix = joints[config->place_locator], reflected;
                        NuMtxPreRotateY(&matrix, object->takeover_start_angle);
                        NUVEC scaling = {scale, scale, scale};
                        NuMtxPreScale(&matrix, &scaling);
                        NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &matrix);
                        if (WORLD->lev_objs[0xed].active)
                            NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &matrix);
                        if (object->field_0x1088 &&
                            MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                             WORLD->current_level->unknown_0cc, &reflected)) {
                            NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &reflected);
                            if (WORLD->lev_objs[0xed].active)
                                NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &matrix);
                        }
                    }
                }
            }
        }
    }
    if (object->field_0xe22 & 0x40) {
        dco_prerotatez = 0x4000;
        DrawObjectOnCharacter(WORLD, object, Batarang_GetObjectFromCharID(object->id), NULL, config->throw_locator, -1,
                              joints, object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    }
    if (object->field_0x7a5 == 0x46)
        Grapple_DrawLine(object);
    if ((object->field_0xe22 & 0x80) || object->field_0x7a5 == 0x58)
        SuperCarry_DrawObject(object);
    DrawCharacterAttachments(object, joints);
    if (object->field_0x7a5 == 0x5d && WORLD->lev_objs[0xf7].active && config->hand_locators[0] != -1 &&
        object->apiobj.character_model->points_of_interest[config->hand_locators[0]] != NULL) {
        NUMTX matrix, reflected;
        NuMtxSetTranslation(&matrix, reinterpret_cast<NUVEC *>(&joints[config->hand_locators[0]].m30));
        Draw3DObjectMtx(NULL, 0xf7, &matrix);
        if (object->field_0x1088 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                     WORLD->current_level->unknown_0cc, &reflected)) {
            NuRndrStartReflectionRender(0);
            Draw3DObjectMtx(NULL, 0xf7, &reflected);
            NuRndrEndReflectionRender();
        }
    }
    if (object->torpedo != NULL && object->torpedo->count != 0)
        DrawTorpedos(object);
    if (config->uses_weapon_action == 0)
        Customiser_DrawAccessories(CharacterCustomiser, object, NULL);
    if ((config->flags_090 & 0x01000000) && object->field_0xd80 > 0.0f && object->field_0xd8c > 0.0f)
        Transform_DrawTarget(&object->force_glow_position,
                             (1.4f + 0.20000004768371582f * object->field_0xd80) * object->field_0xd8c,
                             0.4f + 0.6f * object->field_0xd80);
    f32 *special_time = NULL;
    if (config->uses_weapon_action == 1) {
        i32 locator = config->thingy_locator;
        if (locator == -1 || object->apiobj.character_model->points_of_interest[locator] == NULL ||
            (special_time = AnimPlaying(&object->apiobj.anim_packet, 0x6b, 1, 1)) == NULL)
            return;
        f32 frame = *special_time;
        if ((frame >= 70.0f && frame <= 210.0f) || (frame >= 310.0f && frame <= 433.0f)) {
            NUMTX matrix = joints[locator], reflected;
            NuMtxPreRotateZ(&matrix, 0xc000);
            NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, &matrix);
            if (object->field_0x1088 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                         WORLD->current_level->unknown_0cc, &reflected))
                NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, &reflected);
        } else {
            f32 distance = 1.0f;
            i32 chosen = -1;
            for (i32 i = 10; i < 18; ++i) {
                if (NuSpecialExistsFn(&LevHSpecial[i])) {
                    f32 next =
                        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&LevHSpecial[i]), NULL);
                    if (next < distance) {
                        distance = next;
                        chosen = i;
                    }
                }
            }
            if (chosen != -1)
                NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, NuSpecialGetDrawMtx(&LevHSpecial[chosen]));
        }
    } else if (object->id == id_TWOFACE && (special_time = AnimPlaying(&object->apiobj.anim_packet, 1, 0, 0)) != NULL) {
        f32 start = AnimListFrame(object->apiobj.character_model, 1, 0);
        if (start >= 1.0f) {
            f32 finish = AnimListFrame(object->apiobj.character_model, 1, 1);
            f32 frame = *special_time;
            if (finish >= start && frame > start && frame < finish) {
                f32 fraction = (frame - start) / (finish - start);
                f32 scale;
                if (fraction < 0.1f)
                    scale = fraction / 0.1f;
                else if (fraction < 0.9f)
                    scale = 1.0f;
                else
                    scale = 1.0f - (fraction - 0.9f) / 0.1f;
                NUVEC scaling = {scale, scale, scale};
                NUMTX matrix;
                NuMtxSetScale(&matrix, &scaling);
                NuMtxRotateX(&matrix, static_cast<u16>(-65536.0f * fraction));
                NuMtxRotateY(&matrix, object->apiobj.field_0x276);
                NuMtxTranslate(&matrix, reinterpret_cast<NUVEC *>(&joints[0].m30));
                matrix.m31 += 0.2f * NuTrigTable[(static_cast<i32>(fraction * 32768.0f) >> 1) & 0x7fff];
                DrawObjectOnCharacter(WORLD, object, 0xf9, NULL, 0, -1, &matrix, object->field_0x1088,
                                      object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
            }
        }
    }
}

static __used__ void DrawFalconSpotLights(GameObject_s *) {
    STUBBED();
}

static __used__ void DisplayListMaterialClipUpdate(nudisplayscene_s *) {
    STUBBED();
}

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);

static NUGSCN *backdrop_scene = nullptr;

static nuhspecial_s s_backdrop_hspecial[4];

static f32 backdrop_back_wait = 0.0f;

f32 backdrop_top_r = 0.0f;
f32 backdrop_top_g = 0.0f;
f32 backdrop_top_b = 0.0f;
f32 backdrop_bot_r = 0.0f;
f32 backdrop_bot_g = 0.0f;
f32 backdrop_bot_b = 0.0f;

static f32 backdrop_top_tr = 0.0f;
static f32 backdrop_top_tg = 0.0f;
static f32 backdrop_top_tb = 0.0f;
static f32 backdrop_bot_tr = 0.0f;
static f32 backdrop_bot_tg = 0.0f;
static f32 backdrop_bot_tb = 0.0f;

i32 backdrop_black = 0;

void (*BackDrop_AlphaFn)(float *) = nullptr;

static __used__ void BackDrop_Alpha(float *alpha) {
    if (alpha == nullptr)
        return;
    if (backdrop_black) {
        *alpha *= 0.0f;
    } else if (backdrop_back_wait > 0.0f) {
        *alpha *= 0.5f;
    }
}

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end) {
    NUGSCN *scene = backdrop_scene;
    if (scene == NULL) {
        scene = NuGScnRead(buf, *buf_end, path);
        backdrop_scene = scene;
    }
    memset(s_backdrop_hspecial, 0, sizeof(s_backdrop_hspecial));
    if (scene == NULL) {
        return;
    }
    NuSpecialFind(scene, &s_backdrop_hspecial[0], (char *)"ball1", 1);
    NuSpecialFind(scene, &s_backdrop_hspecial[1], (char *)"ball2", 1);
    NuSpecialFind(scene, &s_backdrop_hspecial[2], (char *)"ball3", 1);
    NuSpecialFind(scene, &s_backdrop_hspecial[3], (char *)"ball4", 1);
}

void BackDrop_Dump() {
    backdrop_scene = nullptr;
    memset(s_backdrop_hspecial, 0, sizeof(s_backdrop_hspecial));
}

void BackDrop_Update(float dt) {
    if (backdrop_scene != NULL) {
        NuGScnUpdate(backdrop_scene, dt * 60.0f);
    }
}

void BackDrop_ResetColours() {
    backdrop_top_r = 0.0f;
    backdrop_top_g = 0.0f;
    backdrop_top_b = 0.0f;
    backdrop_bot_r = 0.0f;
    backdrop_bot_g = 0.0f;
    backdrop_bot_b = 0.0f;
    backdrop_back_wait = 0.0001f;
    backdrop_top_tr = 0.0f;
    backdrop_top_tg = 0.0f;
    backdrop_top_tb = 0.0f;
    backdrop_bot_tr = 0.0f;
    backdrop_bot_tg = 0.0f;
    backdrop_bot_tb = 0.0f;
}

void BackDrop_UpdateColours(i32 instant) {
    const f32 lerp = (instant != 0) ? 1.0f : 0.05f;
    auto seek = [&](f32 &cur, f32 tgt) { cur += (tgt - cur) * lerp; };
    if (backdrop_black) {
        seek(backdrop_top_r, 0.0f);
        seek(backdrop_top_g, 0.0f);
        seek(backdrop_top_b, 0.0f);
        seek(backdrop_bot_r, 0.0f);
        seek(backdrop_bot_g, 0.0f);
        seek(backdrop_bot_b, 0.0f);
        return;
    }
    seek(backdrop_top_r, backdrop_top_tr);
    seek(backdrop_top_g, backdrop_top_tg);
    seek(backdrop_top_b, backdrop_top_tb);
    seek(backdrop_bot_r, backdrop_bot_tr);
    seek(backdrop_bot_g, backdrop_bot_tg);
    seek(backdrop_bot_b, backdrop_bot_tb);
}

void BackDrop_Draw(float alpha, i32 flags) {
    if (backdrop_scene == NULL) {
        return;
    }
    if (flags == 0 && BackDrop_AlphaFn != NULL) {
        BackDrop_AlphaFn(&alpha);
    }
    if (alpha <= 0.0f) {
        return;
    }

    for (i32 special_index = 0; special_index < 2; ++special_index) {
        nuhspecial_s *special = &s_backdrop_hspecial[special_index];
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }
        NUMTX mtx = *NuSpecialGetDrawMtx(special);
        f32 x = mtx.m30;
        f32 y = mtx.m31;
        f32 z = mtx.m32;
        u16 angle = (u16)qrand();
        for (i32 i = 0; i < 3; ++i) {
            mtx.m30 = NuTrigTable[angle >> 1] * 0.01f + x;
            mtx.m31 = NuTrigTable[((i32)angle + 0x4000) >> 1 & 0x7fff] * 0.01f + y;
            mtx.m32 = z;
            NuSpecialDrawAtAlpha(special, &mtx, alpha);
            angle = (u16)(angle + 0x5555);
        }
    }
}
