// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "globals.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/fx/explosions.h"
#include "legoapi/items/fx/ripples.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/gameliball.h"
#include "legoapi/world/levels/episodeI.h"

extern "C" ripple_set_s *ripples;

extern "C" i16 id_LANDSPEEDER;


void DebrisKillPlayers() {
    DebrisPreCheckCollisions(&GameCam->pos, 50.0f);
#define CHECK_DEBRIS_PLAYER(player_index)                                                                              \
    do {                                                                                                               \
        GameObject_s *player = Player[player_index];                                                                   \
        if (player != NULL && (player->apiobj.flags_high & 0x10) != 0 && player->apiobj.field_0x287 == 0 &&            \
            player->field_0x1024 <= 0.0f && !(player->spawn_protection_timer > 0.0f) &&                               \
            (player->field_0xefe & 0x40) == 0 && CannotKill(player) == 0 && Player_HasInvincibility(player) == 0 &&    \
            (player->apiobj.character_data->game_character->flags_090 & 0x04008000) == 0) {                            \
            if (DebrisCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,        \
                                           player->collision_y_scale) != -1) {                                         \
                ObjHitObj(NULL, player, 1, 0, 0, 1);                                                                   \
            }                                                                                                          \
            if (DebrisTorusCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,   \
                                                player->collision_y_scale) != -1) {                                    \
                ObjHitObj(NULL, player, 1, 0, 0, 1);                                                                   \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

    CHECK_DEBRIS_PLAYER(0);
    CHECK_DEBRIS_PLAYER(1);
    CHECK_DEBRIS_PLAYER(2);
    CHECK_DEBRIS_PLAYER(3);
    CHECK_DEBRIS_PLAYER(4);
    CHECK_DEBRIS_PLAYER(5);
    CHECK_DEBRIS_PLAYER(6);
    CHECK_DEBRIS_PLAYER(7);

#undef CHECK_DEBRIS_PLAYER
}

void AddSurfaceDebris(GameObject_s *object) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (object->field_0x7a5 == 0x2b)
        return;
    i8 layer = object->apiobj.field_0x27f;
    if (layer != -1 && (TerLayer[layer].flags & 1) != 0)
        return;
    f32 surface_y = object->apiobj.field_0x218;
    if (surface_y == 2000000.0f)
        return;
    if ((PODRACE_ADATA != NULL && world->area == PODRACE_ADATA) || world->current_level == PODSPRINTA_LDATA) {
        PodDust(world, object);
        return;
    }
    i32 effect;
    f32 height;
    f32 rate;
    f32 minimum_rate;
    if (object->id == id_LANDSPEEDER) {
        effect = 102;
        rate = 20.0f;
        minimum_rate = 0.0f;
        height = 0.0f;
    } else if (WORLD->area != NULL && WORLD->area == HOTHBATTLE_ADATA) {
        effect = 86;
        rate = 100.0f;
        minimum_rate = 10.0f;
        height = 3.0f;
    } else if ((BONUS_GUNSHIP_ADATA != NULL && world->area == BONUS_GUNSHIP_ADATA) ||
               (GUNSHIP_ADATA != NULL && world->area == GUNSHIP_ADATA)) {
        effect = 141;
        rate = 75.0f;
        minimum_rate = 0.0f;
        height = 0.0f;
    } else {
        return;
    }
    f32 speed = object->apiobj.horizontal_velocity_magnitude;
    f32 run_speed = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->run_speed;
    if (world->debris_sys->entries[effect].effect == -1 || run_speed <= 0.0f)
        return;
    f32 height_scale = 1.0f;
    if (!(height <= 0.0f)) {
        height_scale = 1.0f - (object->apiobj.collision_min.y - surface_y) * (1.0f / height);
        if (height_scale <= 0.0f)
            return;
        if (height_scale > 1.0f)
            height_scale = 1.0f;
    }
    f32 emission_rate = (speed / run_speed) * rate;
    if (!(emission_rate >= minimum_rate))
        emission_rate = minimum_rate;
    emission_rate *= height_scale;
    NUVEC current = {object->apiobj.position.x, surface_y, object->apiobj.position.z};
    NUVEC previous;
    if (object->apiobj.field_0x214 == 2000000.0f) {
        previous = current;
    } else {
        previous.x = object->apiobj.start_position.x;
        previous.y = object->apiobj.field_0x214;
        previous.z = object->apiobj.start_position.z;
    }
    i32 count = ParticlesPerSecond(emission_rate, FRAMETIME);
    if (count <= 0)
        return;
    NUVEC delta = {current.x - previous.x, current.y - previous.y, current.z - previous.z};
    do {
        f32 fraction = qrand() * (1.0f / 65535.0f);
        NUVEC position = {previous.x + delta.x * fraction, previous.y + delta.y * fraction,
                          previous.z + delta.z * fraction};
        AddVariableShotDebrisEffect(world->debris_sys->entries[effect].effect, &position, 1, 0, 0);
    } while (--count != 0);
}

void AddWaterSplash(GameObject_s *object, nuvec_s *position) {
    AddGameDebrisXYZ(WORLD->debris_sys, 13, position->x, object->apiobj.water_height, position->z);
    PlaySfx("FS_WaterJump", &object->apiobj.collision_position);
    NewRumble(object->pad_gamepad->pad, 0.5f, 0);
    NewBuzzFrames(object->pad_gamepad->pad, 1, 0);
}

void AddSlamDebris(GameObject_s *object) {
    NUVEC position;
    FindSlamOrigin(object, &position, NULL);
    f32 radius = 0.6f;
    f32 strength = 0.3f;
    i32 damage = 1;
    i32 flags = 0x17;
    if ((object->apiobj.flags_low & 0x80) != 0 && Cheat_IsOn(0x16)) {
        radius = 1.0f;
        strength = 0.5f;
        flags = 0x37;
        damage = 2;
    }
    EXPLOSION *explosion = AddExplosion(&position, radius, strength, object, object->slam_debris_effect, flags);
    if (explosion != NULL)
        explosion->field_0x32 = damage;
}

void InitRippleMtl(char *name, numtl_s **result, variptr_u *buffer, variptr_u *buffer_end) {
    char filename[64] = "stuff\\";
    if (name != NULL) {
        NuStrCat(filename, name);
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        NuStrCat(filename, ".tex");
        i32 texture = NuTexRead(filename, buffer, *buffer_end);
        if (texture != 0) {
            numtl_s *material = NuMtlCreateEx3D(1, 1);
            *result = material;
            material->tex_id = static_cast<i16>(texture);
            material->opacity = 1.0f;
            u8 *attributes = reinterpret_cast<u8 *>(&material->attribs);
            attributes[1] = (attributes[1] & 0x30) | 0x40;
            attributes[0] = (attributes[0] & 0xc0) | 0x12;
            attributes[2] = (attributes[2] & 0xfc) | 2;
            NuMtlUpdate(material);
            return;
        }
    }
    *result = NULL;
}

void BuildRippleMtx(numtx_s *matrix, nuvec_s *normal, nuvec_s *position, u16, u16) {
    FindAnglesXY(normal, NULL, NULL);
    NuMtxSetRotationX(matrix, static_cast<u16>(temp_xrot));
    NuMtxRotateY(matrix, static_cast<u16>(temp_yrot));
    NuMtxTranslate(matrix, position);
}

void AddSurfaceRipples(GameObject_s *object) {

    if (!(WORLD->ripple_effects == NULL || object->apiobj.field_0x287 != 0)) {
        NUVEC position;
        if (object->field_0x1084 != 1) {
            if (!object->apiobj.intersects_water || !object->apiobj.model_draw_result)
                return;
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            f32 rate = (object->pad_gamepad->input_magnitude / data->run_speed) * 20.0f;
            if (rate < 1.0f)
                rate = 1.0f;
            i32 count = ParticlesPerSecond(rate, FRAMETIME);
            if (count <= 0)
                return;
            NUVEC position = {0.0f, 0.0f, 0.0f};
            NUMTX matrix;
            BuildRippleMtx(&matrix, &v010, &position, 0, 0);
            f32 size = 0.5f * object->apiobj.field_0x1dc;
            static f32 randrad = 0.15f;
            for (i32 i = 0; i < count; ++i) {
                matrix.m30 = object->apiobj.collision_position.x + (qrand() * (1.0f / 65535.0f) - 0.5f) * 0.15f;
                matrix.m31 = 0.001f + object->apiobj.water_height;
                matrix.m32 = object->apiobj.collision_position.z + (qrand() * (1.0f / 65535.0f) - 0.5f) * 0.15f;
                RIPPLEEFFECT_s *effect = &WORLD->ripple_effects[WORLD->water_ripple_effect];
                randrad = object->apiobj.field_0x1dc + object->apiobj.field_0x1dc + object->apiobj.field_0x1dc;
                AddRipple(ripples, &matrix, size, randrad, effect->lifetime, 0.0f, effect->start_color,
                          effect->end_color, 7, effect->material, NULL);
            }
            return;
        }
        if (static_cast<u8>(object->field_0x6b0 - 12) <= 1) {
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            f32 rate = (object->pad_gamepad->input_magnitude / data->run_speed) * 100.0f;
            if (rate < 0.5f)
                rate = 0.5f;
            i32 count = ParticlesPerSecond(rate, FRAMETIME);

            if (count > 0) {
                position = object->contact_position;
                NUVEC normal = object->contact_normal;
                NUMTX matrix;
                BuildRippleMtx(&matrix, &normal, &position, 0, 0);
                const NUVEC seed = {normal.y * FRAMETIME, -(normal.z * FRAMETIME), -(normal.x * FRAMETIME)};
                for (i32 i = 0; i < count; ++i) {
                    u16 angle = static_cast<u16>(qrand());
                    NUVEC velocity = seed;
                    VecRotateAxis(&velocity, angle, &normal);
                    RIPPLEEFFECT_s *effect = &WORLD->ripple_effects[WORLD->sabre_ripple_effect];
                    AddRipple(ripples, &matrix, effect->initial_size, effect->end_size, effect->lifetime, 0.0f,
                              effect->start_color, effect->end_color, 15, effect->material, &velocity);
                }
            }
            // The original only initializes the sound position when particles are emitted.
            if (object->sabre_contact_sfx_timer <= 0.0f) {
                PlaySfx("SabFField", &position);
                object->sabre_contact_sfx_timer = 0.5f;
            }
        }
        if (object->sabre_contact_sfx_timer > 0.0f)
            object->sabre_contact_sfx_timer -= FRAMETIME;
    }
}

void BurnoutApply(i32 paused) {
    NuBloomParameters parameters = {};
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && (cut->flags & 0x80) != 0)
        return;
    if (WORLD->burnset == NULL)
        return;
    edrtlCalculateBurnoutEx(WORLD->burnset, &parameters, reinterpret_cast<NUVEC *>(&global_camera.mtx.m30), FRAMETIME);
    if (parameters.intensity > 0.0f)
        NuPostBloom(paused, &parameters);
}

