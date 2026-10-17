// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"

#include <string.h>
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
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
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
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
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
#include "nu2api/numath/nuvec.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"

extern EXPLOSION Explosion[8];
extern i32 i_explosion;

void UpdateExplosion_Generic(EXPLOSION *);

extern WORLDINFO_s *WORLD;
extern "C" i32 ParticlesPerSecond(f32, f32);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
PART_s *Bolt_HitParts(BOLT_s *, NUVEC *, NUVEC *, NUVEC *, f32, i32);
i32 CannotKill(GameObject_s *);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
void GameCam_HitJudder();
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);

void UpdateExplosion_Generic(EXPLOSION *explosion);

// ==== src/legoapi/render/core/render.cpp: declarations ====




// Camera zoom state

// Graphics loading flags

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---


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

// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.

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


// ==== functions in original address order ====

void ResetExplosions() {
    memset(Explosion, 0, sizeof(Explosion));
    i_explosion = 0;
}

EXPLOSION *AddExplosion(nuvec_s *position, float radius, float strength, GameObject_s *object, i32 effect, i32 flags) {
    EXPLOSION *explosion = &Explosion[i_explosion];
    explosion->position = *position;
    explosion->field_0x1c = 0;
    explosion->field_0x18 = radius;
    explosion->object = object;
    explosion->field_0x20 = strength;
    explosion->field_0x00 = 0;
    explosion->field_0x04 = 0;
    explosion->field_0x2a = qrand();
    explosion->field_0x2c = effect;
    explosion->field_0x24 = flags;
    explosion->field_0x2e = ObjHitObj_Flags(object);
    explosion->field_0x32 = 1;
    explosion->field_0x33 = 0xff;
    explosion->field_0x30 = 0;
    if (++i_explosion == 8) {
        i_explosion = 0;
    }
    return explosion;
}

void DrawExplosions() {
}

void UpdateExplosion_Generic(EXPLOSION *explosion) {
    const f32 progress = explosion->field_0x1c / explosion->field_0x20;
    const f32 radius = explosion->field_0x18 * progress;
    u16 angle = explosion->field_0x2a;
    if ((explosion->field_0x24 & 0x10) != 0) {
        for (i32 i = 0; i < 5; ++i) {
            const i32 count = ParticlesPerSecond(40.0f, FRAMETIME);
            NUVEC position;
            position.x = radius * NU_SIN_LUT(angle) + explosion->position.x;
            position.y = progress * 0.1f + explosion->position.y;
            position.z = radius * NU_COS_LUT(angle) + explosion->position.z;
            AddGameDebrisRot(WORLD->debris_sys, static_cast<i16>(explosion->field_0x2c), &position, count, 0, 0);
            angle += 0x3333;
        }
    }
    NUVEC maximum = {explosion->position.x + radius, explosion->position.y + radius, explosion->position.z + radius};
    NUVEC minimum = {explosion->position.x - radius, explosion->position.y - radius, explosion->position.z - radius};
    i32 hit_character = 0;
    if ((explosion->field_0x24 & 0x100) == 0) {
        GameObject_s *target = Obj;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++target) {
            APIOBJECT_s *object = &target->apiobj;
            if (__builtin_expect((object->field_0x1f8 & 0x1001) != 0x1001 || object->field_0x287 != 0, 1))
                continue;
            if ((CInfo[static_cast<i8>(target->character_context)].flags & 0x20008000) != 0)
                continue;
            if ((object->character_data->game_character->flags_090 & 0x8000) != 0)
                continue;
            if ((explosion->field_0x24 & 0x80) != 0 && (object->character_data->model_flags & 0x10) != 0)
                continue;
            if ((explosion->field_0x24 & 0x4000) != 0 && object->field_0x27c != -1)
                continue;
            if ((explosion->field_0x24 & 8) == 0 &&
                (((object->field_0x1e4 & explosion->field_0x00) | (object->field_0x1e8 & explosion->field_0x04)) != 0 ||
                 explosion->object == target))
                continue;
            if (__builtin_expect(object->collision_min.x > maximum.x || minimum.x > object->collision_max.x ||
                object->collision_min.y > maximum.y || minimum.y > object->collision_max.y ||
                object->collision_min.z > maximum.z || minimum.z > object->collision_max.z, 0))
                continue;
            if (!SphereSphereOverlapScaleY(&object->collision_position, object->field_0x1dc, object->field_0x1e0,
                                           &explosion->position, radius, radius))
                continue;
            if ((explosion->field_0x24 & 8) != 0) {
                f32 x = object->collision_position.x - explosion->position.x;
                f32 z = object->collision_position.z - explosion->position.z;
                if (z == 0.0f && x == 0.0f) {
                    x = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                    z = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                }
                const f32 inverse = 1.0f / NuFsqrt(x * x + z * z);
                object->field_0x1fc = (x * inverse) + (x * inverse);
                object->field_0x204 = (z * inverse) + (z * inverse);
            }
            if (hit_character || (explosion->field_0x24 & 1) == 0)
                continue;
            if (((object->field_0x1e4 & explosion->field_0x00) | (object->field_0x1e8 & explosion->field_0x04)) != 0 ||
                explosion->object == target || target->spawn_protection_timer > 0.0f ||
                (target->field_0xefe & 0x40) != 0)
                continue;
            if (object->field_0x27c != -1 && !(target->field_0x1024 <= 0.0f) && (explosion->field_0x24 & 0x2000) == 0)
                continue;
            GameObject_s *source;
            if (explosion->object != NULL && !(Arcade_GetMode(NULL) == 99 && (explosion->field_0x24 & 0x10010) != 0)) {
                source = explosion->object;
                if (object->field_0x27c == -1) {
                    if (source->apiobj.field_0x27c == -1 &&
                        ((object->field_0x1f4 ^ source->apiobj.field_0x1f4) & 1) == 0)
                        continue;
                } else if (source->apiobj.field_0x27c != -1 && target->field_0xd24 != 1.0f &&
                           ((object->field_0x1f4 ^ source->apiobj.field_0x1f4) & 1) == 0) {
                    continue;
                }
            }
            explosion->field_0x00 |= object->field_0x1e4;
            explosion->field_0x04 |= object->field_0x1e8;
            if (target->field_0xd24 >= 0.99f) {
                ObjHitShield(explosion->object, target, target->field_0xe37, NULL);
                if ((explosion->field_0x24 & 0x20) == 0) {
                    hit_character = 1;
                    continue;
                }
            }
            if (__builtin_expect(!!(CannotKill(target)), 0)) {
                hit_character = 1;
                continue;
            }
            i32 damage;
            if ((object->flags_low & 0x80) != 0 && Player_HasInvincibility(target))
                damage = 0;
            else if ((explosion->field_0x24 & 0x800) != 0 && (target->field_0xefb & 8) == 0)
                damage = -1;
            else if ((explosion->field_0x24 & 0x20) != 0 && (target->field_0xefb & 8) == 0 &&
                     static_cast<i8>(target->current_hp) > 0)
                damage = -1;
            else
                damage = explosion->field_0x32;
            source = explosion->object;
            if (source == NULL) {
                if (explosion->field_0x33 == 0)
                    source = Player[0];
                else if (explosion->field_0x33 == 1)
                    source = Player[1];
            }
            const u16 hit_flags = explosion->field_0x2e | ((explosion->field_0x24 & 0x200) != 0 ? 0x4200 : 0x200);
            if (ObjHitObj(source, target, damage, hit_flags, 0, 1) == 2) {
                if (source != NULL)
                    NewRumble(source->pad_gamepad->pad, 0.5f, 0);
                ++explosion->field_0x30;
                if ((explosion->field_0x24 & 0x200) != 0 && explosion->field_0x33 <= 1 && Arcade != 0) {
                    if (static_cast<u8>(object->field_0x27c) <= 1)
                        Arcade_PlayerKilled((object->field_0x27c ^ 1) & 1, 0);
                    else if (object->field_0x27c == -1)
                        Arcade_AIKilled(static_cast<i8>(explosion->field_0x33));
                }
            }
            if ((explosion->field_0x24 & 0x40) != 0) {
                NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
                GameCam_HitJudder();
            } else if (explosion->object != NULL)
                NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
            hit_character = 1;
        }
    }
    if (__builtin_expect(!hit_character && (explosion->field_0x24 & 2) != 0, 1)) {
        u32 hit_type;
        if (__builtin_expect((explosion->field_0x24 & 0x10) != 0, 0))
            hit_type = 3;
        else if ((explosion->field_0x24 & 0x100) != 0)
            hit_type = 7;
        else if ((explosion->field_0x24 & 0x200) != 0)
            hit_type = 8;
        else
            hit_type = (explosion->field_0x24 & 0x8000) != 0 ? 9 : 2;
        if (GizmoBlowUp_Hit(explosion->object, &explosion->position, 1, radius, &minimum, &maximum, NULL, hit_type,
                            NULL)) {
            if ((explosion->field_0x24 & 0x40) != 0) {
                NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
                GameCam_HitJudder();
            } else if (explosion->object != NULL)
                NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
            return;
        }
    }
    if (!((explosion->field_0x24 & 0x100) == 0 && (explosion->field_0x24 & 4) != 0)) {
        return;
    }
    BOLT_s bolt;
    NUVEC points[3];
    bolt.owner = explosion->object;
    points[1] = explosion->position;
    bolt.position = explosion->position;
    bolt.active = 0;
    bolt.flags = 0x200;
    bolt.type_id = 1;
    bolt.type = BoltType_FindByID(1, WORLD);
    if (Bolt_HitParts(&bolt, points, &minimum, &maximum, radius, 0) != NULL) {
        if ((explosion->field_0x24 & 0x40) != 0) {
            NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
            GameCam_HitJudder();
        } else if (explosion->object != NULL)
            NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
    }
}

void UpdateExplosions() {
#define UPDATE_EXPLOSION(index)                                                                                        \
    do {                                                                                                               \
        EXPLOSION *explosion = &Explosion[index];                                                                      \
        if (explosion->field_0x1c < explosion->field_0x20) {                                                           \
            explosion->field_0x1c += FRAMETIME;                                                                        \
            if (explosion->field_0x1c >= explosion->field_0x20) {                                                      \
                explosion->field_0x1c = explosion->field_0x20;                                                         \
            } else {                                                                                                   \
                if (explosion->object != NULL && (explosion->object->apiobj.flags_low & 1) == 0) {                     \
                    explosion->object = NULL;                                                                          \
                }                                                                                                      \
                UpdateExplosion_Generic(explosion);                                                                    \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    UPDATE_EXPLOSION(0);
    UPDATE_EXPLOSION(1);
    UPDATE_EXPLOSION(2);
    UPDATE_EXPLOSION(3);
    UPDATE_EXPLOSION(4);
    UPDATE_EXPLOSION(5);
    UPDATE_EXPLOSION(6);

#undef UPDATE_EXPLOSION

    EXPLOSION *explosion = &Explosion[7];
    if (explosion->field_0x1c < explosion->field_0x20) {
        explosion->field_0x1c += FRAMETIME;
        if (explosion->field_0x1c >= explosion->field_0x20) {
            explosion->field_0x1c = explosion->field_0x20;
            return;
        }
        if (explosion->object != NULL && (explosion->object->apiobj.flags_low & 1) == 0) {
            explosion->object = NULL;
        }
        UpdateExplosion_Generic(explosion);
    }
}

