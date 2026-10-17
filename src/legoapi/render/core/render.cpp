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
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
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
#include <string.h>

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

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
#include "nu2api/nu3d/nuprim_internal.h"
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
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

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

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern f32 MENUTEXTSCALE;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
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
extern "C" i32 editor_active;
extern i32 Paused;
extern "C" i32 screendump;
extern f32 pause_fade;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
i32 RemoveDirectionalMaps = 0;
i32 RemoveNormalMaps = 0;

NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta) {
    NUMTX *animation_matrix = gscn->instance_animation_matrices;
    NUDLDLISTSCENE *display_list = gscn->display_list;
    if (gscn->instance_animation_data == NULL) {
        return;
    }

    nuinstanim_s *instance_animation = gscn->instance_animations;

    // The lookup array is reference-counted by the allocation word immediately
    // before its first entry.  A sole owner means the optional table was not
    // populated and the original runtime discards it here.
    if (gscn->animation_end_frames != NULL && reinterpret_cast<i32 *>(gscn->animation_end_frames)[-1] <= 1) {
        gscn->animation_end_frames = NULL;
    }

    i16 animation_count = gscn->num_instance_animations;
    if (animation_count <= 0) {
        return;
    }

    // A waiting animation retains the previous evaluation frame.
    f32 frame = 0.0f;
    i32 i = 0;
    do {
        if (__builtin_expect(instance_animation->anim_ix < gscn->num_instance_animation_data, 1)) {
            NUDISPLAYSPECIAL *display_special =
                &static_cast<NUDISPLAYSPECIAL *>(display_list->specials)[instance_animation->instance_ix];
            const u32 display_flags = display_special->flags;
            nuanimendlookup_s *state_animation = NULL;
            nuanimdata_s *animation = NULL;

            if (__builtin_expect((instance_animation->end_frame_lookup_bits & NUINSTANIM_END_FRAME_LOOKUP_MASK) != 0 &&
                gscn->animation_end_frames != NULL, 1)) {
                const u16 lookup_index = instance_animation->end_frame_lookup_index;
                state_animation = &gscn->animation_end_frames[lookup_index - 1];
                animation = gscn->instance_animation_data[instance_animation->anim_ix];
            } else {
                if ((display_flags & NUDISPLAYSPECIAL_FLAG_VISIBLE) == 0) {
                    goto next_animation;
                }
                animation = gscn->instance_animation_data[instance_animation->anim_ix];
                if (animation == NULL) {
                    goto next_animation;
                }
            }

            f32 end_frame;
            if (animation != NULL) {
                end_frame = NuAnimEndFrameOld(animation);
            } else if (__builtin_expect(state_animation != NULL, 0)) {
                end_frame = static_cast<f32>(static_cast<u32>(state_animation->end_frame));
            } else {
                goto next_animation;
            }

            if (end_frame == 0.0f) {
                animation_count = gscn->num_instance_animations;
                goto next_animation;
            }

            if (instance_animation->playing != 0) {
                instance_animation->ltime += frame_delta * instance_animation->tfactor;
                if (instance_animation->waiting != 0) {
                    if (__builtin_expect(!(instance_animation->ltime >= instance_animation->tfirst), 0)) {
                        goto evaluate_animation;
                    }
                    instance_animation->waiting = 0;
                    instance_animation->ltime -= instance_animation->tfirst - 1.0f;
                }

                frame = instance_animation->ltime;
                const f32 interval_end = instance_animation->tinterval + end_frame;
                if (!(frame >= interval_end)) {
                    if (frame > end_frame) {
                        frame = end_frame;
                    } else if (frame < 1.0f) {
                        instance_animation->playing = 0;
                        instance_animation->ltime = 1.0f;
                        frame = 1.0f;
                    }
                } else {
                    if (__builtin_expect(instance_animation->repeating != 0, 0)) {
                        const f32 repeat_length = interval_end - 1.0f;
                        const f32 repeats = NuFloor((instance_animation->ltime - 1.0f) / repeat_length);
                        const f32 live_repeat_length = instance_animation->tinterval + end_frame - 1.0f;
                        instance_animation->ltime -= live_repeat_length * repeats;
                        frame = instance_animation->ltime;
                    } else {
                        instance_animation->playing = 0;
                        instance_animation->ltime = end_frame;
                        frame = end_frame;
                    }
                }

                if (__builtin_expect(instance_animation->backwards != 0, 1)) {
                    frame = end_frame + 1.0f - frame;
                }
            } else {
                frame = instance_animation->ltime;
            }

        evaluate_animation:
            if (frame != instance_animation->prev_eval_time) {
                if (state_animation != NULL) {
                    u8 state_index =
                        static_cast<u8>((static_cast<u32>(instance_animation->flags) & NUINSTANIM_STATE_INDEX_MASK) >>
                                        NUINSTANIM_STATE_INDEX_SHIFT);
                    char state_value;
                    const i32 state_changed = StateAnimEvaluate2(reinterpret_cast<StateAnim *>(state_animation),
                                                                 &state_index, &state_value, frame);
                    const u32 state_flags =
                        (static_cast<u32>(instance_animation->flags) & ~NUINSTANIM_STATE_INDEX_MASK) |
                        (static_cast<u32>(state_index) << NUINSTANIM_STATE_INDEX_SHIFT);
                    instance_animation->flags = static_cast<NUINSTANIM_FLAGS>(state_flags);

                    if (state_changed) {
                        if (instance_animation->instance_ix != 0xffff) {
                            display_special = &static_cast<NUDISPLAYSPECIAL *>(
                                display_list->specials)[instance_animation->instance_ix];
                            if (state_value == 0) {
                                display_special->flags &= ~NUDISPLAYSPECIAL_FLAG_VISIBLE;
                                animation = NULL;
                            } else {
                                display_special->flags |= NUDISPLAYSPECIAL_FLAG_VISIBLE;
                            }
                        } else if (state_value == 0) {
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
            animation_count = gscn->num_instance_animations;
        }
    next_animation:
        ++i;
        ++instance_animation;
        ++animation_matrix;
    } while (i < animation_count);
}

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");
static NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path, char *scene_data) {
    NUGSCN *scene = reinterpret_cast<NUGSCN *>(scene_data);
    if (__builtin_expect(scene == NULL, 0)) {
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

    const i32 version = *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(scene) + 8);
    void **fixed = (void **)NuPtrBlockFix((char *)scene + 0x18);
    NUGSCN *fixed_scene = (NUGSCN *)*fixed;
    if (fixed_scene != NULL) {
        if (__builtin_expect(*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x168) > 0, 0)) {
            if (version < 0 || version > 2) {
                NuSevereWarning("Your Character version is %d, whereas the Engine only accepts Character versions "
                                "from %d to %d",
                                version, 0, 2);
            }
        } else if (version < 0 || version > 2) {
            NuSevereWarning("Your Scene version is %d, whereas the Engine only accepts Scene versions from %d to %d",
                            version, 0, 2);
        }
        if (version < 2) {
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(fixed_scene) + 0x1f4) = 1;
        }
        NuGScnCreatePS(fixed_scene, buf, buf_end);

        for (i32 i = 0; i < fixed_scene->ntextures; ++i) {
            if (fixed_scene->textures[i]->ref_count < 0) {
                fixed_scene->texture_ids[i] = 0xabcdabcd;
            } else {
                fixed_scene->texture_ids[i] = NuTexCreateNative(fixed_scene->textures[i], true);
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
            nutexanim_s *anim = &static_cast<nutexanim_s *>(fixed_scene->texture_anims)[i];
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
    NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count) {
        NUGSCN *scene = NuReadGraphicsData(buf, &buf_end, path, NULL);
        if (scene != NULL && render_count > 1) {
            NUGSCN **additional_scenes = reinterpret_cast<NUGSCN **>(ALIGN(buf->addr, 32));
            buf->addr = reinterpret_cast<usize>(additional_scenes + render_count);
            i32 index = 0;
            for (; index < render_count - 1; ++index) {
                additional_scenes[index] = reinterpret_cast<NUGSCN *>(ALIGN(buf->addr, 32));
                buf->addr = ALIGN(buf->addr, 32) + sizeof(NUGSCN);
                memcpy(additional_scenes[index], scene, sizeof(NUGSCN));
                additional_scenes[index]->display_list = NuDisplaySceneClone(scene->display_list, buf, buf_end);
                additional_scenes[index]->display_list->gscene = additional_scenes[index];
            }
            additional_scenes[index] = NULL;
            scene->additional_scenes = additional_scenes;
        }
        return scene;
    }

    NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path) {
        RemoveDirectionalMaps = 1;
        RemoveNormalMaps = 1;
        NUGSCN *scene = NuReadGraphicsData(buf, &buf_end, path, NULL);
        RemoveNormalMaps = 0;
        RemoveDirectionalMaps = 0;
        return scene;
    }
    void NuGScnReadFromMemory(NUGSCN *scene) {
        NuReadGraphicsData(NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }
    NUGSCN *NuGHGFixup(NUGSCN *scene, void *) {
        return NuReadGraphicsData(NULL, NULL, NULL, reinterpret_cast<char *>(scene));
    }
    nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end) {
        nuapi.loading_hgobj = 1;
        nuhgobj_s *object = reinterpret_cast<nuhgobj_s *>(NuReadGraphicsData(buf, &buf_end, path, NULL));
        if (object != NULL && nuapi.force_shadows_on_characters != 0) {
            for (i32 i = 0; i != object->display_list->nspecials; ++i) {
                object->display_list->visibility_flags[i] |= 0x20;
            }
        }
        nuapi.loading_hgobj = 0;
        return object;
    }
} // extern "C"

void DrawRipple(ripple_node_s *node);

extern "C" {
    f32 aiEditor_LocatorWidth = 0.25f;
}

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3] = {
    {255, 0, 0}, {0, 255, 0}, {0, 127, 255}, {127, 0, 255}, {255, 255, 0},
};

void DrawParallax(nuhspecial_s *special);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
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
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
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
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
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
            NUVEC direction;
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

void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawPauseFade();

void DrawRippleSet(ripple_set_s *set);

static u8 cacheValues[256];

void ClearScreen();

static inline u16 AlphaImageFloatToHalf(f32 value) {
    u32 bits;
    memcpy(&bits, &value, sizeof(bits));
    i32 mantissa = bits & 0x7fffff;
    i32 exponent = static_cast<i32>((bits >> 23) & 0xff) - 0x70;
    i32 sign = bits >> 31;
    i32 half_exponent = 0;
    if (exponent >= 0) {
        half_exponent = 0x7c00;
        if (exponent < 0x20) {
            half_exponent = exponent * 0x400;
        }
    }
    return static_cast<u16>((mantissa >> 13) | (sign << 15) | half_exponent);
}

static inline void AlphaImagePrimUV(f32 u, f32 v) {
    PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);
    if (!g_NuPrim_NeedsHalfUVs) {
        vertex->float_uv[0] = u;
        vertex->float_uv[1] = v;
    } else {
        vertex->half_uv[0] = AlphaImageFloatToHalf(u);
        vertex->half_uv[1] = AlphaImageFloatToHalf(v);
    }
}

void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters) {
    f32 inv_col = 1.0f / (cols - 1);
    f32 inv_row = 1.0f / (rows - 1);
    f32 x0 = 0.0f * inv_row * 2.0f - 1.0f;
    f32 y_start = 0.0f * inv_col * 2.0f - 1.0f;
    f32 x1 = inv_row * 2.0f - 1.0f;
    f32 step_x = inv_row * 2.0f;
    f32 step_y = inv_col * 2.0f;
    f32 near_scale = parameters->near_scale * 128.0f;
    f32 far_scale = parameters->far_scale * 128.0f;
    f32 near_angle = parameters->near_angle / 180.0f;
    f32 far_angle = parameters->far_angle / 180.0f;
    f32 scale_delta = far_scale - near_scale;
    f32 angle_delta = far_angle - near_angle;
    NUVPREGION saved_region = g_NuVpRegion;
    NuVpResetRegions();

    NUCAMERA camera;
    NuCameraGet(&camera);
    u32 clear_screen;
    memcpy(&clear_screen, &parameters->unknown_18, sizeof(clear_screen));
    if (clear_screen != 0)
        ClearScreen();
    if (parameters->directional)
        NuVecNorm(&parameters->direction, &parameters->direction);

    static f32 pixelOffsetX;
    static f32 pixelOffsetY;
    if (use_pixel_offsets) {
        pixelOffsetX = 0.5f / static_cast<f32>(NuTexWidth(material->tex_id));
        pixelOffsetY = 0.5f / static_cast<f32>(NuTexHeight(material->tex_id));
    }

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
        camera.fov = camFov;
    }
    camera.mtx.m30 = 0.0f;
    camera.mtx.m31 = 0.0f;
    camera.mtx.m32 = 0.0f;
    camera.mtx.m33 = 1.0f;

    ++NuPrimCSPos;
    NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_NORMALISED);
    for (row = 0; row < rows - 1; ++row) {
        const i32 cache_row = row;
        NuPrim2DBegin(1, 7, material);
        f32 y0 = y_start;
        for (col = 0; col < cols; ++col) {
            NUVEC direction;
            const f32 direction_y = ((y0 + 1.0f) - 1.0f) * -camera.aspect;
            direction.x = (x0 + 1.0f) - 1.0f;
            direction.y = direction_y;
            direction.z = adjacent;
            NuVecNorm(&direction, &direction);
            NuVecMtxTransform(&direction, &direction, &camera.mtx);
            NuVecNorm(&direction, &direction);

            f32 brightness = direction.y;
            u32 colour;
            if (!(cache_row > 0)) {
                brightness = brightness * 0.5f + 0.5f;
                if (brightness <= near_angle)
                    brightness = near_scale * parameters->intensity;
                else if (brightness >= far_angle)
                    brightness = far_scale * parameters->intensity;
                else
                    brightness =
                        ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity;
                if (parameters->directional) {
                    f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;
                    if (angle < parameters->direction_near_angle) {
                        brightness += 128.0f * parameters->direction_far_scale;
                    } else if (!(angle > parameters->direction_far_angle)) {
                        f32 blend;
                        if (parameters->direction_bias == 1.0f)
                            blend = 1.0f - (angle - parameters->direction_near_angle) /
                                               (parameters->direction_far_angle - parameters->direction_near_angle);
                        else
                            blend = 1.0f -
                                    NuPowFast((angle - parameters->direction_near_angle) /
                                                  (parameters->direction_far_angle - parameters->direction_near_angle),
                                              parameters->direction_bias);
                        brightness +=
                            128.0f * (blend * (parameters->direction_far_scale - parameters->direction_near_scale) +
                                      parameters->direction_near_scale);
                    }
                }
                colour = (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 24) | 0x808080;
                NuRndrPrimSetColour(colour);
            } else {
                colour = (static_cast<u32>(cacheValues[col]) << 24) | 0x808080;
                NuRndrPrimSetColour(colour);
            }
            AlphaImagePrimUV(static_cast<f32>(row) * inv_row + pixelOffsetX,
                             (static_cast<f32>(col) * inv_col + pixelOffsetY) * 0.9f);
            NuPrim2DAddXYZ(x0, y0, 0.0f);

            direction.x = (x1 + 1.0f) - 1.0f;
            direction.y = direction_y;
            direction.z = adjacent;
            NuVecNorm(&direction, &direction);
            NuVecMtxTransform(&direction, &direction, &camera.mtx);
            NuVecNorm(&direction, &direction);
            brightness = direction.y * 0.5f + 0.5f;
            if (brightness <= near_angle)
                brightness = near_scale * parameters->intensity;
            else if (brightness >= far_angle)
                brightness = far_scale * parameters->intensity;
            else
                brightness =
                    ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity;
            if (parameters->directional) {
                f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;
                if (angle < parameters->direction_near_angle) {
                    brightness += 128.0f * parameters->direction_far_scale;
                } else if (!(angle > parameters->direction_far_angle)) {
                    f32 blend;
                    if (parameters->direction_bias == 1.0f)
                        blend = 1.0f - (angle - parameters->direction_near_angle) /
                                           (parameters->direction_far_angle - parameters->direction_near_angle);
                    else
                        blend =
                            1.0f - NuPowFast((angle - parameters->direction_near_angle) /
                                                 (parameters->direction_far_angle - parameters->direction_near_angle),
                                             parameters->direction_bias);
                    brightness +=
                        128.0f * (blend * (parameters->direction_far_scale - parameters->direction_near_scale) +
                                  parameters->direction_near_scale);
                }
            }
            const u8 alpha = static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)));
            cacheValues[col] = alpha;
            colour = (static_cast<u32>(alpha) << 24) | 0x808080;
            NuRndrPrimSetColour(colour);
            AlphaImagePrimUV(static_cast<f32>(row + 1) * inv_row + pixelOffsetX,
                             (static_cast<f32>(col) * inv_col + pixelOffsetY) * 0.9f);
            NuPrim2DAddXYZ(x1, y0, 0.0f);
            y0 += step_y;
        }
        NuPrim2DEnd();
        x0 += step_x;
        x1 += step_x;
    }
    --NuPrimCSPos;
    NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    g_NuVpRegion = saved_region;
}

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour) {
    static i32 bezier_draw_depth;

    const f32 camera_y = global_camera.mtx.m31;
    const f32 camera_x = global_camera.mtx.m30;
    const f32 camera_z = global_camera.mtx.m32;
    const f32 start_y = start.y - camera_y;
    const f32 start_z = start.z - camera_z;
    const f32 start_x = start.x - camera_x;
    const f32 start_distance = start_x * start_x + start_y * start_y + start_z * start_z;
    const f32 end_x = end.x - camera_x;
    const f32 end_y = end.y - camera_y;
    const f32 end_z = end.z - camera_z;
    const f32 end_distance = end_x * end_x + end_y * end_y + end_z * end_z;
    const f32 distance = end_distance < start_distance ? end_distance : start_distance;
    const f32 threshold = distance < 5.5f ? 5.5f * 0.001f : distance * 0.001f;

    const f32 chord_y = end.y - start.y;
    const f32 chord_x = end.x - start.x;
    const f32 chord_z = end.z - start.z;
    if (!(chord_x * chord_x + chord_y * chord_y + chord_z * chord_z > threshold) || bezier_draw_depth > 15) {
        EdDrawLineSegment(start, end, colour);
        return;
    }

    ++bezier_draw_depth;
    const f32 end_control_x = end_control.x;
    const f32 end_control_y = end_control.y;
    const f32 control_x = start_control.x;
    const f32 end_control_z = end_control.z;
    VuVec second = {(control_x + end_control_x) * 0.5f, (end_control_y + start_control.y) * 0.5f,
                    (end_control_z + start_control.z) * 0.5f, 0.0f};
    VuVec first = {(start.x + control_x) * 0.5f, (start.y + start_control.y) * 0.5f, (start_control.z + start.z) * 0.5f,
                   0.0f};
    VuVec third = {(end.x + end_control_x) * 0.5f, (end.y + end_control_y) * 0.5f, (end.z + end_control_z) * 0.5f,
                   0.0f};
    VuVec fourth = {(first.x + second.x) * 0.5f, (first.y + second.y) * 0.5f, (first.z + second.z) * 0.5f, 0.0f};
    VuVec fifth = {(third.x + second.x) * 0.5f, (third.y + second.y) * 0.5f, (third.z + second.z) * 0.5f, 0.0f};
    VuVec midpoint = {(fifth.x + fourth.x) * 0.5f, (fifth.y + fourth.y) * 0.5f, (fifth.z + fourth.z) * 0.5f, 0.0f};

    DrawBezierLine(start, first, midpoint, fourth, material, colour);
    DrawBezierLine(midpoint, fifth, end, third, material, colour);
    --bezier_draw_depth;
}

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
f32 PANEL_REDBRICKSCALE = 0.4f;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
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
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;

void DrawFadeScreenWipe();

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

extern void DrawMenuEntry(MENU_s *, char *);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour) {
    const NUVEC *centre = reinterpret_cast<const NUVEC *>(position);
    const NUVEC *extent = reinterpret_cast<const NUVEC *>(size);
    NUVEC minimum __attribute__((aligned(16)));
    NUVEC maximum __attribute__((aligned(16)));
    maximum.x = centre->x + extent->x;
    maximum.y = centre->y + extent->y;
    maximum.z = centre->z + extent->z;
    minimum.x = centre->x - extent->x;
    minimum.y = centre->y - extent->y;
    minimum.z = centre->z - extent->z;
    NuCameraUnlock();
    NuRndrBoundingBox(&minimum, &maximum, &numtx_identity, colour);
    NuCameraRelock();
}

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)


#undef SWIPE_PRIM_UV


extern i32 GetMenuID(void);
static __used__ i32 PauseRenderOff();

static __used__ i32 MatrixReflection_CanOverride();

void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);

static __used__ void BackDrop_Alpha(float *alpha);

void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
    void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context) {
        NuVisibilityResult *visibility =
            static_cast<NuVisibilityResult *>(NuVisiEvaluate(scene->gscene, &visibility_context));
        i32 portal_filter = 0;
        if (visibility != NULL && portal_special_objects != 0 && visibility->portal_marker != NULL)
            portal_filter = portals_enabled != 0;
        void *shadow_pass = currentScene.unknown_3c;

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
            special_handle.display_special = special;
            if (shadow_pass && (scene->visibility_flags[special->instance_ix] & NUDL_INSTANCE_FLAG_CASTS_SHADOW) == 0) {
                continue;
            }
            if (portal_filter &&
                __builtin_expect(
                    (scene->visibility_flags[special->instance_ix] & NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) == 0, 1) &&
                (visibility->portal_bits[special->instance_ix >> 3] & (1U << (special->instance_ix & 7))) == 0) {
                continue;
            }
            NUMTX *draw_mtx = special->draw_mtx_ptr;
            if (draw_mtx == reinterpret_cast<NUMTX *>(-1)) {
                draw_mtx = NULL;
            }
            if (draw_mtx == NULL) {
                NuDisplayListRndrSpecial(&special_handle, &special->draw_mtx, 0, NULL, NULL);
            } else {
                NuDisplayListRndrSpecial(&special_handle, draw_mtx, 0, NULL, NULL);
            }
        }
    }
}
