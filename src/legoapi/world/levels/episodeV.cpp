// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuang.h"
#include "MechInputTouch/MechInputTouch_types.h"

#include <string.h>
#include <stdio.h>
#include "legoapi/world/levels/episodeV_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/core/config/cheat.h"
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include <math.h>
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/world/mission.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/world/levels/episode_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/core/input/timer.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "batman.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include <new>
#include "gameapi/edtools/edgra.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nuvec4.h"
#include <limits.h>
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/numusic/sfx.h"
#include <stdlib.h>
#include "legoapi/core/net/netplay.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/props/system/socksys.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/misc/androidbatman.h"
#include <stdarg.h>
#include "legoapi/render/fx/game_deb.h"
#include "gameapi/ai/aisys/gameaiscript.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/gizmo/object/gizbuildit.h"
#include "legoapi/ai/core/aisysall.h"
#include "legoapi/world/levels/episodeV.h"

extern i32 LevFlag[4];
#define LEV_FLAG_U8 (reinterpret_cast<u8 *>(LevFlag))
GIZPANEL_s *LevGizPanel;
AILOCATOR_s *locator;
GameObject_s *gameobj;
static GameObject_s *Vader_obj;
static GIZAIMESSAGE_s *Vader_ai_message;
static u8 turretAliveCount;
nuhspecial_s specialIcon;

extern "C" {
    GIZBOMBGEN *HothBattleC_BombGenerator = NULL;
    HOTHBATTLE_MELEE_s melee;
    u8 dagobahA_nodesNeedUpdating = 1;
}
HOTHBATTLEE_NETPACKET_s *hothbattlee_netpacket;

static f32 alpha[16];

static inline void HothBattle_ClearBackgroundObjects() {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) == 0x1001 && object->id != id_ATAT &&
            object->apiobj.field_0x27c == -1 && (object->apiobj.character_data->model_flags & 4) != 0) {
            KillGameObject(object, 4, 0);
        }
    }
    memset(melee.background_creatures, 0, sizeof(melee.background_creatures));
}

static inline void HothBattle_AssignSpawnLocator(GameObject_s *object, AILOCATOR_s *locator, AILOCATORSET_s *set) {
    auto set_locator_count = set->locator_count;
    for (i32 index = 0; index < set_locator_count; ++index) {
        AILOCATOR_s *entry = &WORLD->ai_sys->locators[set->locator_entries[index]];
        if (entry != locator) {
            continue;
        }
        object->ai.locator = entry;
        set->assigned[index] = object->apiobj.field_0x289;
        break;
    }
}

i32 nasteroids;
ASTEROID_s asteroids[128];
FINALASTEROID_s finalAsteroid;
ASTEROIDCNETPACKET_s *asteroidc_netpacket;
static NUMTX *mtxOrig;
GIZMOBLOWUP_s *classicBlowups[8];
nuhspecial_s escape[8];
GIZTURRET_s *StarDestroyerTurrets[16];
static i32 lastPlaying;
static i8 melee_wavePhase = -1;
static f32 melee_waveDelay;
static u8 drawLights;
static f32 spotLightA_yrot[2];
static f32 spotLightA_zrot[2];
static f32 spotLightB_yrot[2] = {0.5f, 0.5f};
static f32 spotLightB_zrot[2] = {0.5f, 0.5f};

static __used__ void Asteroids_Update();

static inline void AsteroidChaseD_SetTurretTarget(i32 index, NUVEC *position) {
    GIZTURRET_s *turret = StarDestroyerTurrets[index];
    if (turret != NULL && turret->field_0xe4 == NULL) {
        turret->field_0xe4 = position;
        turret->field_0x12c = 2;
    }
}

TROOPERCANNON_s troopercannons[4];

static __used__ void KilledTrooperCannon(GameObject_s *object);

struct AISYS_s;

extern i16 tALLMINIKITSBUILT;

static inline void IncrementMinikitCounterFor(WORLDINFO_s *world, NUVEC *average, i32 gizmo_index, u8 *counter,
                                              f32 camera_distance, u16 camera_pitch, u16 camera_yaw) {
    GIZMO *gizmo = LevGizmo[gizmo_index];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }
    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if ((pickup->state_flags & (GIZMOPICKUP_STATE_COLLECTED | GIZMOPICKUP_STATE_ALTERNATE_TYPE)) != 0 ||
        *counter >= 10) {
        return;
    }
    ++*counter;
    if (*counter == 10) {
        NUVEC camera = {0.0f, 0.0f, camera_distance};
        NuVecRotateX(&camera, &camera, camera_pitch);
        NuVecRotateY(&camera, &camera, camera_yaw);
        NuVecAdd(&camera, &camera, &pickup->position);
        GameCameraMakeMiniCut2(&camera, &pickup->position, 0, 0.0f, 4.0f, 0.0f, 0.0f, 0, 0, 0);
        GizmoActivate(world->gizmo_sys, LevGizmo[gizmo_index], 1, 1);
        // ORIG_BUG: the completion message is placed at a position that is never set.
        NUVEC message_pos;
        AddGameMsgCount(&message_pos, *counter, 10, 200, 100, 30, 0.75f);
        return;
    }
    AddGameMsgCount(average, *counter, 10, 200, 100, 30, 0.75f);
}

struct nuhspecial_s;

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

extern "C" i16 id_SNAKE;

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

extern i16 id_GAMORREANGUARD;

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;

extern MechWeakPtr<MechObjectInterface, 4> nextShootTarget;
extern MechWeakPtr<MechObjectInterface, 4> forceNextAttackOpponent;
extern MechWeakPtr<MechObjectInterface, 4> lungeTarget;

extern EXTRAACTIONDATA_s ExtraActionData[33];

extern i16 id_BUZZDROID;
extern "C" i16 id_MINIATST, id_MINIATTE;
extern i16 id_PKDROID, id_PITDROID;

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

extern i16 id_MINIATST, id_MINIATTE;

extern speedup_s DogFightSpeedList[9];

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

static void Asteroid_AddParts(GIZMOBLOWUP_s *blowup) {
    i32 special_indices[4] = {0, -1, -1, -1};
    const i32 part_count = qrand() / 0x4000 + 1;
    for (i32 index = 1; index < part_count; ++index) {
        special_indices[index] = qrand() / (0xffff / 3 + 1) + 1;
    }

    for (i32 index = 0; index < part_count; ++index) {
        nuhspecial_s *special = &LevHSpecial[special_indices[index]];
        if (NuSpecialExistsFn(special) == 0) {
            continue;
        }

        NUANGVEC rotation = {qrand(), qrand(), qrand()};
        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotateXYZVU0(&matrix, &rotation);
        NuMtxTranslate(&matrix, &blowup->position);

        NUVEC velocity;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 3.0f + 2.0f;
        NuVecRotateY(&velocity, &velocity, qrand());

        ADDPART_ALIGNED16 params = Default_ADDPART;
        params.matrix = &matrix;
        params.velocity = &velocity;
        NUVEC centre;
        NuSpecialGetRadius(special, &centre, &params.field_14);
        params.field_18 = params.field_14;
        params.gravity = 0.0f;
        params.special = special;
        if (index != 0) {
            params.flags = 0x8000193;
        } else {
            params.flags = 0x800019b;
        }
        params.time_step = FRAMETIME;
        params.field_40 = PartCollide_3D;
        params.field_44 = Asteroid_PartKill;
        params.field_a4 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 3.0f + 7.0f;

        PART_s *part = AddPart(&params);
        if (part == NULL) {
            continue;
        }
        part->force_player_mask = index == 0 ? 1 : 2;
    }
}

static __used__ void Asteroids_Update() {
    ASTEROID_s *asteroid = asteroids;
    for (i32 index = 0; index < nasteroids; ++index, ++asteroid) {
        if (asteroid->blowup == NULL) {
            if (NuSpecialGetVisibilityFn(&asteroid->special) != 0) {
                asteroid->activated = 0;
                NUMTX *matrix = NuSpecialGetDrawMtx(&asteroid->special);
                if (matrix != NULL) {
                    NuMtxPreRotateX(matrix, static_cast<i32>(static_cast<f32>(asteroid->rotation_speed_x) * FRAMETIME));
                    NuMtxPreRotateY(matrix, static_cast<i32>(static_cast<f32>(asteroid->rotation_speed_y) * FRAMETIME));
                    NuSpecialUpdate(&asteroid->special);
                    continue;
                }
            }
        } else {
            if ((static_cast<u16>(asteroid->blowup->status_flags) & 0x4001) == 0x4000) {
                asteroid->activated = 0;
                asteroid->blowup->field_0xf0 +=
                    static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_x) * FRAMETIME);
                asteroid->blowup->field_0xf2 +=
                    static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_y) * FRAMETIME);
                asteroid->blowup->field_0xf4 +=
                    static_cast<i16>(static_cast<f32>(asteroid->rotation_speed_z) * FRAMETIME);
                asteroid->blowup->state_flags |= 1;
                GizmoBlowupUpdateMatrix(asteroid->blowup);
                continue;
            }
        }

        if (asteroid->activated == 0) {
            asteroid->activated = 1;
            if (asteroid->blowup != NULL) {
                asteroid->blowup->field_0xa8 = 0;
                Asteroid_AddParts(asteroid->blowup);
            }
        }
    }
}

static void Asteroids_Reset(WORLDINFO_s *world) {
    static const i32 maxrotspd[3] = {0x1555, 0x38e, 0x16c};
    nuhspecial_s specials[128];

    memset(asteroids, 0, sizeof(asteroids));
    nasteroids = 0;

    i32 special_count = NuSpecialFindMulti(world->current_gscn, specials, "asteroid", 128, 0);
    if (special_count == 0)
        return;

    if (__builtin_expect(special_count > 0, 1)) {
        for (i32 special_index = 0; special_index < special_count; ++special_index) {
            for (i32 type_index = 0; type_index < world->gizmo_blowup_type_count; ++type_index) {
                NuSpecialCompare(&world->gizmo_blowup_types[type_index].special, &specials[special_index]);
            }

            if (NuSpecialGetVisibilityFn(&specials[special_index]) != 0) {
                ASTEROID_s *asteroid = &asteroids[nasteroids];
                asteroid->special = specials[special_index];

                char *name = NuSpecialGetName(&asteroid->special);
                i32 asteroid_type;
                if (name == NULL || NuStrIStr(name, "asteroid_a") != NULL || NuStrIStr(name, "asteroid_pop") != NULL) {
                    asteroid_type = 0;
                } else if (NuStrIStr(name, "asteroid_b") != NULL) {
                    asteroid_type = 1;
                } else if (__builtin_expect(NuStrIStr(name, "asteroid_c") != NULL, 1)) {
                    asteroid_type = 2;
                } else {
                    asteroid_type = 0;
                }

                i32 max_speed = maxrotspd[asteroid_type];
                asteroid->rotation_speed_x = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
                asteroid->rotation_speed_y = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
                ++nasteroids;
            }
        }
    }

    for (i32 blowup_index = 0; blowup_index < world->gizmo_blowup_count; ++blowup_index) {
        ASTEROID_s *asteroid = &asteroids[nasteroids];
        char *name = world->gizmo_blowups[blowup_index].name;
        i32 asteroid_type;
        if (name == NULL || NuStrIStr(name, "asteroid_a") != NULL || NuStrIStr(name, "asteroid_pop") != NULL) {
            asteroid_type = 0;
        } else if (NuStrIStr(name, "asteroid_mid") != NULL) {
            asteroid_type = 1;
        } else {
            continue;
        }

        asteroid->blowup = &world->gizmo_blowups[blowup_index];
        i32 max_speed = maxrotspd[asteroid_type];
        asteroid->rotation_speed_x = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        asteroid->rotation_speed_y = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        asteroid->rotation_speed_z = static_cast<i16>(qrand() / ~(0xffff / (max_speed * 2)) + max_speed);
        ++nasteroids;
    }
}

static void DrawFalconSpotLights(GameObject_s *object) {
    if (static_cast<u8>(object->apiobj.field_0x27c) > 1 || object->id != id_MILLENNIUMFALCON ||
        WORLD->lev_objs[0x127].active == 0)
        return;
    NUMTX matrix __attribute__((aligned(16)));
    if (object->apiobj.character_model->points_of_interest[4] != NULL) {
        matrix = object->joint_matrices[4];
        NuSpecialDrawAt(&WORLD->lev_objs[0x127].special, &matrix);
    }
    spotLightA_yrot[object->apiobj.field_0x27c] += FRAMETIME / 5.0f;
    if (spotLightA_yrot[object->apiobj.field_0x27c] > 1.0f)
        spotLightA_yrot[object->apiobj.field_0x27c] -= 1.0f;
    spotLightA_zrot[object->apiobj.field_0x27c] += FRAMETIME / 5.0f;
    if (spotLightA_zrot[object->apiobj.field_0x27c] > 1.0f)
        spotLightA_zrot[object->apiobj.field_0x27c] -= 1.0f;
    if (object->apiobj.character_model->points_of_interest[5] != NULL) {
        matrix = object->joint_matrices[5];
        NuSpecialDrawAt(&WORLD->lev_objs[0x127].special, &matrix);
    }
    spotLightB_yrot[object->apiobj.field_0x27c] += FRAMETIME / 5.0f;
    if (spotLightB_yrot[object->apiobj.field_0x27c] > 1.0f)
        spotLightB_yrot[object->apiobj.field_0x27c] -= 1.0f;
    spotLightB_zrot[object->apiobj.field_0x27c] += FRAMETIME / 5.0f;
    if (spotLightB_zrot[object->apiobj.field_0x27c] > 1.0f)
        spotLightB_zrot[object->apiobj.field_0x27c] -= 1.0f;
}

void Asteroid_PartKill(PART_s *part, i32 reason) {
    AddGameDebris(WORLD->debris_sys, 0x5d, &part->position);
    AddGameDebris(WORLD->debris_sys, 0x5e, &part->position);
    AddGameDebris(WORLD->debris_sys, 0x5f, &part->position);
    AddPartDebris(WORLD->part_debris_sys, 4, &part->position);
    if (part->force_player_mask == 1) {
        i32 type;
        switch (reason) {
            case 4:
                type = 0;
                break;
            case 5:
                type = 1;
                break;
            default:
                return;
        }
        AddMiscPickups(&part->position, type, 1000, 0);
    } else {
        AddMiscPickups(&part->position, -1, 0, 1);
    }
}

static __used__ void KilledTrooperCannon(GameObject_s *object) {
    if (netclient != 0)
        return;

    for (i32 i = 0; i < 4; ++i) {
        if (troopercannons[i].object == object) {
            DeactivateCharacter(troopercannons[i].character_name);
            GizBuildIt_SetToStart(troopercannons[i].buildit, 0, 0);
            GIZMO *gizmo = GizmoFindByData(WORLD->gizmo_sys, gizbuildit_gizmotype_id, troopercannons[i].buildit);
            GizmoActivate(WORLD->gizmo_sys, gizmo, 1, 1);
            GizBuildit_SetVisibility(troopercannons[i].buildit, 1);
            troopercannons[i].rebuilding = 1;
            WORLD->level_progress->destroyed_trooper_cannon_mask |= 1u << i;
            return;
        }
    }
}

void InitTrooperCannons(WORLDINFO_s *) {
    memset(troopercannons, 0, sizeof(troopercannons));
}

void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id) {
    if (troopercannons_beenReset != 0) {
        if (netclient == 0 || troopercannons[0].object != NULL)
            return;
    }

    i32 cannon_index = 0;
    char name[32];
    for (i32 number = 1; number < 5; ++number) {
        troopercannons[cannon_index] = TROOPERCANNON_s();

        sprintf(name, "trooper_cannon%d", number);
        troopercannons[cannon_index].buildit = GizBuildIt_Find(world, name);
        if (troopercannons[cannon_index].buildit != NULL) {
            sprintf(name, "troopercannon_%d", number);
            troopercannons[cannon_index].object = GetNamedGameObject(world->ai_sys, name);
            if (troopercannons[cannon_index].object != NULL)
                NuStrCpy(troopercannons[cannon_index].character_name, name);
        }

        sprintf(name, "BASE%d", number);
        troopercannons[cannon_index].base = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);

        if (netclient != 0) {
            if (troopercannons[cannon_index].object != NULL)
                ++cannon_index;
            continue;
        }

        if (troopercannons[cannon_index].buildit != NULL && troopercannons[cannon_index].object != NULL) {
            if ((world->level_progress->destroyed_trooper_cannon_mask & (1u << (number - 1))) != 0) {
                GizBuildIt_SetToStart(troopercannons[cannon_index].buildit, 0, 0);
                troopercannons[cannon_index].rebuilding = 1;
            } else {
                if (troopercannons[cannon_index].base != NULL) {
                    GizmoSetVisibility(world->gizmo_sys, troopercannons[cannon_index].base, 1, 1);
                    GizObstacle_PlayBackwards(static_cast<GIZOBSTACLE_s *>(troopercannons[cannon_index].base->object));
                }
                GizBuildIt_Finish(troopercannons[cannon_index].buildit);
                GizBuildit_SetVisibility(troopercannons[cannon_index].buildit, 0);
                troopercannons[cannon_index].rebuilding = 0;
                ActivateCharacter(name, NULL, 0);
                AddGameDebris(world->debris_sys, 0x5c, &troopercannons[cannon_index].object->apiobj.collision_position);
                GameObject_s *trooper =
                    AddDynamicCreature(trooper_id, &troopercannons[cannon_index].object->apiobj.collision_position,
                                       troopercannons[cannon_index].object->apiobj.field_0x276, "CannonTrooper",
                                       &troopercannons[cannon_index].object->ai.path_info, NULL, 0, NULL, NULL, 0, 0);
                if (__builtin_expect(trooper != NULL, 1))
                    TakeOverGameObject(trooper, troopercannons[cannon_index].object, 0, 1);
            }
        }
        ++cannon_index;
    }
    troopercannons_beenReset = 1;
}

void UpdateTrooperCannons(WORLDINFO_s *) {
    for (i32 i = 0; i < 4; ++i) {
        TROOPERCANNON_s &cannon = troopercannons[i];

        if (cannon.buildit != NULL && netclient == 0) {
            if (cannon.object != NULL) {
                if (GizBuildIt_AtEnd(cannon.buildit) && cannon.rebuilding != 0) {
                    ActivateCharacter(cannon.character_name, NULL, 0);
                    GizBuildit_SetVisibility(cannon.buildit, 0);
                    cannon.rebuilding = 0;
                    WORLD->level_progress->destroyed_trooper_cannon_mask &= ~(1u << i);
                }
            } else if (GizBuildIt_AtEnd(cannon.buildit)) {
                GizBuildIt_KillParts(cannon.buildit);
                GizBuildIt_SetToStart(cannon.buildit, 0, 0);
                GizBuildit_SetVisibility(cannon.buildit, 0);
            }
        }

        if (cannon.object == NULL || netclient != 0)
            break;

        if (cannon.object->field_0xcc0 != NULL) {
            if (cannon.object->field_0xcc0->field_0xeb4 == NULL)
                cannon.object->field_0xcc0->field_0xeb4 = KilledTrooperCannon;
        } else if (cannon.object->field_0xeb4 == NULL) {
            cannon.object->field_0xeb4 = KilledTrooperCannon;
        }
    }
}

void HothEscapeA_Init(WORLDINFO_s *world) {
    InitTrooperCannons(world);
    troopercannons_beenReset = 0;
}

void HothEscapeA_Reset(WORLDINFO_s *) {
}

void HothEscapeA_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void HothEscapeB_Init(WORLDINFO_s *world) {
    if (netclient == 0) {
        locator = AIPathFindLocator(world->ai_sys, "snow_mob");
        gameobj = GetNamedGameObject(world->ai_sys, "snowmob_1");
    }
    InitTrooperCannons(world);
    troopercannons_beenReset = 0;
}

void HothEscapeB_Reset(WORLDINFO_s *world) {
    locator = AIPathFindLocator(world->ai_sys, "snow_mob");
    gameobj = GetNamedGameObject(world->ai_sys, "snowmob_1");
    TerSurface[9].movement_scale = TerSurface[17].movement_scale;
    TerSurface[9].flags = TerSurface[17].flags & ~2u;
}

void HothEscapeB_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
    if (netclient == 0 && locator != NULL && gameobj != NULL) {
        locator->position = *NUMTX_GET_ROW_VEC(&gameobj->joint_matrices[1], 3);
    }
}

void HothEscapeC_Init(WORLDINFO_s *world) {
    InitTrooperCannons(world);
    troopercannons_beenReset = 0;
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle19");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle20");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle21");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle22");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle23");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle24");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle25");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle26");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "Obstacle27");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "gen_1a", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "gen_2a", 1);
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "gen_1b1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[0];
    }
    blowup = GizmoBlowUp_FindByName(world, "gen_1a1");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "gen_2b1");
    if (blowup != NULL) {
        blowup->field_0x124 = 1;
        blowup->override_special = &LevHSpecial[1];
    }
    blowup = GizmoBlowUp_FindByName(world, "gen_2a1");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
}

void HothEscapeC_Reset(WORLDINFO_s *) {
}

void HothEscapeC_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void HothEscapeC_AlwaysUpdate(WORLDINFO_s *world) {
    LevelStreaming_DoorOverride(world, HOTHESCAPED_LDATA, 7.5f, NULL);
}

void HothEscapeD_Init(WORLDINFO_s *world) {
    InitTrooperCannons(world);
    troopercannons_beenReset = 0;
}

void HothEscapeD_Reset(WORLDINFO_s *) {
}

void HothEscapeD_Update(WORLDINFO_s *world) {
    ResetTrooperCannons(world, id_SNOWTROOPER);
    UpdateTrooperCannons(world);
}

void AsteroidChaseA_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "small_pop_bit1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "small_pop_bit2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "small_pop_bit3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "small_pop_bit4", 1);
}

void AsteroidChaseA_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseA_Update(WORLDINFO_s *) {
    Asteroids_Update();
}

void AsteroidChaseB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "small_pop_bit1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "small_pop_bit2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "small_pop_bit3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "small_pop_bit4", 1);
    memset(classicBlowups, 0, sizeof(classicBlowups));
    i32 count = 0;
    for (i32 index = 0; index < world->gizmo_blowup_count && count < 8; ++index) {
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[index];
        if (NuStrIStr(blowup->name, "classic") != NULL) {
            classicBlowups[count++] = blowup;
        }
    }
    LEV_FLAG_U8[0] = 0;
    LevArea[0] = AISysFindArea(WORLD->ai_sys, "In_Cave");
    drawLights = 0;
    LevTime[0] = 0.0f;
    LevTime[1] = 0.0f;
    spotLightA_yrot[1] = 0.0f;
    spotLightA_yrot[0] = 0.0f;
    spotLightA_zrot[1] = 0.0f;
    spotLightA_zrot[0] = 0.0f;
    spotLightB_yrot[1] = 0.5f;
    spotLightB_yrot[0] = 0.5f;
    spotLightB_zrot[1] = 0.5f;
    spotLightB_zrot[0] = 0.5f;
}

void AsteroidChaseB_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseB_Update(WORLDINFO_s *world) {
    Asteroids_Update();
    for (i32 index = 0; index < 4; ++index) {
        GIZMOBLOWUP_s *blowup = classicBlowups[index];
        if (blowup != NULL) {
            const i32 mask = 1 << index;
            if ((blowup->output_flags & 1) != 0) {
                if ((LEV_FLAG_U8[0] & mask) == 0) {
                    LEV_FLAG_U8[0] |= mask;
                    AddMiscPickups(&blowup->position, -1, -1, 1);
                }
            } else if ((LEV_FLAG_U8[0] & mask) != 0) {
                LEV_FLAG_U8[0] &= ~mask;
            }
        }
    }
    for (i32 index = 0; index < 2; ++index) {
        GameObject_s *object = Player[index];
        if (object == NULL || object->id != id_MILLENNIUMFALCON) {
            LevTime[index] = 0.0f;
        } else if (LevArea[0] != NULL &&
                   (object->apiobj.ai_area_mask & (1 << (LevArea[0] - world->ai_sys->areas))) != 0) {
            if (LevTime[index] >= 2.0f) {
                LevTime[index] = 2.0f;
                object->field_0x1054 |= 8;
                DrawFalconSpotLights(object);
                drawLights = 1;
            } else {
                LevTime[index] += FRAMETIME;
            }
        } else if (LevTime[index] > 0.0f) {
            LevTime[index] -= FRAMETIME;
        } else {
            object->field_0x1054 &= ~8;
            drawLights = 0;
            LevTime[index] = 0.0f;
        }
    }
}

void AsteroidChaseB_Draw(WORLDINFO_s *) {
    for (i32 index = 0; index < 2; ++index) {
        if (drawLights != 0 && Player[index] != NULL) {
            DrawFalconSpotLights(Player[index]);
        }
    }
}

void AsteroidChaseC_Init(WORLDINFO_s *world) {
    asteroidc_netpacket = static_cast<ASTEROIDCNETPACKET_s *>(SetLevelHack(8));
    memset(&finalAsteroid, 0, sizeof(finalAsteroid));
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "small_pop_bit1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "small_pop_bit2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "small_pop_bit3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "small_pop_bit4", 1);
    NuSpecialFind(world->current_gscn, &finalAsteroid.special, "blockrock", 1);

    finalAsteroid.rotation_speed_x = static_cast<i16>(4551.0f - qrand() / (65535.0f / (4551.0f * 2.0f) + 1.0f));
    finalAsteroid.rotation_speed_y = static_cast<i16>(4551.0f - qrand() / (65535.0f / (4551.0f * 2.0f) + 1.0f));
    finalAsteroid.rotation_speed_z = static_cast<i16>(4551.0f - qrand() / (65535.0f / (4551.0f * 2.0f) + 1.0f));
    if (finalAsteroid.rotation_speed_x > -910 && finalAsteroid.rotation_speed_x < 910) {
        finalAsteroid.rotation_speed_x = 0x555;
    }
    if (finalAsteroid.rotation_speed_y > -910 && finalAsteroid.rotation_speed_y < 910) {
        finalAsteroid.rotation_speed_y = 0x555;
    }
    if (finalAsteroid.rotation_speed_z > -910 && finalAsteroid.rotation_speed_z < 910) {
        finalAsteroid.rotation_speed_z = 0x555;
    }

    for (i32 index = 0; index < world->gizmo_blowup_count && finalAsteroid.blowup_count < 8; ++index) {
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[index];
        if (NuStrIStr(blowup->name, "targ") != NULL) {
            finalAsteroid.blowups[finalAsteroid.blowup_count++] = blowup;
        }
    }
}

void AsteroidChaseC_Reset(WORLDINFO_s *world) {
    Asteroids_Reset(world);
}

void AsteroidChaseC_Update(WORLDINFO_s *) {
    Asteroids_Update();
    if (netclient == 0) {
        finalAsteroid.rotation_x += static_cast<i16>(finalAsteroid.rotation_speed_x * FRAMETIME);
        finalAsteroid.rotation_y += static_cast<i16>(finalAsteroid.rotation_speed_y * FRAMETIME);
        finalAsteroid.rotation_z += static_cast<i16>(finalAsteroid.rotation_speed_z * FRAMETIME);
        if (nethost != 0) {
            asteroidc_netpacket->rotation_x = finalAsteroid.rotation_x;
            asteroidc_netpacket->rotation_y = finalAsteroid.rotation_y;
            asteroidc_netpacket->rotation_z = finalAsteroid.rotation_z;
        }
    } else {
        finalAsteroid.rotation_x = SeekRot(finalAsteroid.rotation_x, asteroidc_netpacket->rotation_x, 7.0f);
        finalAsteroid.rotation_y = SeekRot(finalAsteroid.rotation_y, asteroidc_netpacket->rotation_y, 7.0f);
        finalAsteroid.rotation_z = SeekRot(finalAsteroid.rotation_z, asteroidc_netpacket->rotation_z, 7.0f);
    }

    mtxOrig = NuSpecialGetMtx(&finalAsteroid.special);
    if (mtxOrig != NULL) {
        NUMTX_ALIGNED16 matrix = *mtxOrig;
        NUVEC position;
        NuMtxGetTranslation(&matrix, &position);
        NuMtxRotateY(&matrix, finalAsteroid.rotation_y);
        NuMtxPreRotateX(&matrix, finalAsteroid.rotation_x);
        NuMtxPreRotateY(&matrix, finalAsteroid.rotation_z);
        matrix.m30 = position.x;
        matrix.m31 = position.y;
        matrix.m32 = position.z;
        NuSpecialSetDrawMtx(&finalAsteroid.special, &matrix);
        NuSpecialUpdate(&finalAsteroid.special);
    }

    for (i32 index = 0; index < finalAsteroid.blowup_count; ++index) {
        GIZMOBLOWUP_s *blowup = finalAsteroid.blowups[index];
        if (blowup != NULL) {
            blowup->field_0xf0 = finalAsteroid.rotation_x;
            blowup->field_0xf2 = finalAsteroid.rotation_y;
            blowup->field_0xf4 = finalAsteroid.rotation_z;
            blowup->state_flags |= 1;
            GizmoBlowupUpdateMatrix(blowup);
        }
    }
}

void AsteroidChaseD_Init(WORLDINFO_s *world) {
    i16 targets = -1;
    NuSpecialFind(vehicle_scene, &specialIcon, "Gun_Turret_icon", 1);
    memset(escape, 0, sizeof(escape));
    turretAliveCount = 0;
    lastPlaying = 0;
    NuSpecialFind(WORLD->current_gscn, &escape[0], "rebelcruiser2", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[1], "transport3", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[2], "transport2", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[4], "transport1", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[5], "rebelcruiser1", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[6], "transport4", 0);
    char name[16];
    for (i32 index = 0; index < 16; ++index) {
        sprintf(name, "turret%d", index + 1);
        GIZMO_s *gizmo = GizmoFindByName(world->gizmo_sys, turret_gizmotype_id, name);
        if (gizmo != NULL) {
            StarDestroyerTurrets[index] = static_cast<GIZTURRET_s *>(gizmo->object);
        }
        if (StarDestroyerTurrets[index] != NULL) {
            StarDestroyerTurrets[index]->fire_interval = 0.5f;
            ++turretAliveCount;
        }
    }
    DrawMeleeTargetsNumber(&targets, &turretAliveCount, 1, 1, NULL);
    melee_wavePhase = 0;
}

void AsteroidChaseD_Update(WORLDINFO_s *) {
    if (melee_wavePhase == 0 && MiniCutCam != 0) {
        melee_wavePhase = 1;
    }
    NuSpecialFind(WORLD->current_gscn, &escape[0], "rebelcruiser2", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[1], "transport3", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[2], "transport2", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[4], "transport1", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[5], "rebelcruiser1", 0);
    NuSpecialFind(WORLD->current_gscn, &escape[6], "transport4", 0);
    NUVEC *position = NuSpecialGetDrawPos(&escape[6]);
    for (i32 index = 0; index < 2; ++index) {
        AsteroidChaseD_SetTurretTarget(index, position);
    }
    position = NuSpecialGetDrawPos(&escape[0]);
    for (i32 index = 2; index < 6; ++index) {
        AsteroidChaseD_SetTurretTarget(index, position);
    }
    position = NuSpecialGetDrawPos(&escape[1]);
    for (i32 index = 6; index < 8; ++index) {
        AsteroidChaseD_SetTurretTarget(index, position);
    }
    position = NuSpecialGetDrawPos(&escape[5]);
    for (i32 index = 10; index < 14; ++index) {
        AsteroidChaseD_SetTurretTarget(index, position);
    }
    position = NuSpecialGetDrawPos(&escape[4]);
    for (i32 index = 14; index < 16; ++index) {
        AsteroidChaseD_SetTurretTarget(index, position);
    }
    if (melee_wavePhase == 1 && MiniCutCam == 0) {
        for (i32 index = 0; index < 16; ++index) {
            if (StarDestroyerTurrets[index] != NULL) {
                StarDestroyerTurrets[index]->fire_interval = 2.0f;
            }
        }
        melee_wavePhase = 2;
    }
    for (i32 index = 0; index < 16; ++index) {
        if (StarDestroyerTurrets[index] != NULL && (StarDestroyerTurrets[index]->flags & 0x20) != 0) {
            --turretAliveCount;
            StarDestroyerTurrets[index] = NULL;
        }
    }
    if (melee_waveDelay > 0.0f) {
        melee_waveDelay -= FRAMETIME;
    }
    if (turretAliveCount == 0 && netclient == 0) {
        LevTime[0] += FRAMETIME;
        if (melee_waveDelay <= 0.0f && LevTime[0] >= 12.0f) {
            if (FreePlay == 0) {
                GoToNewLevel(ASTEROIDCHASEMITRO_LDATA->idx);
            } else {
                GoToNewLevel(ASTEROIDCHASEA_LDATA->idx);
            }
        }
    }
}

void AsteroidChaseD_Panel(WORLDINFO_s *) {
    i16 targets = -1;
    DrawMeleeTargetsNumber(&targets, &turretAliveCount, 1, 0, &specialIcon);
}

void CloudCityTrapA_Init(WORLDINFO_s *world) {
    if (netclient == 0)
        InitTrooperCannons(world);
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force3");
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_exit1");
    if (blowup != NULL)
        blowup->field_0xa0 |= 2;
}

void CloudCityTrapA_Reset(WORLDINFO_s *) {
    if (netclient == 0)
        troopercannons_beenReset = 0;
}

void CloudCityTrapA_Update(WORLDINFO_s *world) {
    static NUVEC pos = {33.0f, -0.75f, -3.0f};
    if (netclient == 0) {
        ResetTrooperCannons(world, id_SNOWTROOPER);
        UpdateTrooperCannons(world);
    }
    if (LevGameObject[0] == NULL)
        LevGameObject[0] = FindGameObject(id_DARTHVADER, 1, 1, 1, 0);
    if (netclient == 0) {
        if (LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f)
            DrawBossHitPoints(LevGameObject[0]);
        else
            DrawBossHitPoints(NULL);
    }
    GIZMO *gizmo = LevGizmo[0];
    if (gizmo != NULL && gizmo->object != NULL &&
        static_cast<GIZFORCE_s *>(gizmo->object)->anim_set->state == GAMEANIMSET_STATE_AT_END)
        AIAntinodeCreateSingleFrame(&pos, 0.5f);
}

void CloudCityTrapB_Init(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "TrapBEndFight", NULL);
    LevGameObject[0] = FindGameObject(id_DARTHVADER, 1, 1, 1, 0);
    nugspline_s *spline = edSpline_SplineFind(world->current_gscn, "door_window_out");
    if (spline != NULL) {
        spline->pts[0].z -= 0.35f;
        spline->pts[1].z -= 0.35f;
        spline->pts[2].z -= 0.35f;
    }
}

void CloudCityTrapB_Update(WORLDINFO_s *) {
    if (LevGameObject[0] == NULL)
        LevGameObject[0] = FindGameObject(id_DARTHVADER, 1, 1, 1, 0);
    if (netclient != 0)
        return;
    if (LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f)
        DrawBossHitPoints(LevGameObject[0]);
    else
        DrawBossHitPoints(NULL);
    if (LevAIMessage[1] == NULL || LevAIMessage[1]->value != 1.0f || LevGameObject[0] == NULL)
        return;
    GameObject_s *vader = LevGameObject[0];
    if (vader->apiobj.field_0x287 != 0 || vader->current_hp == 0) {
        if (FreePlay != 0)
            CompleteLevel(WORLD);
        else if (CLOUDCITYTRAPOUTRO_LDATA != NULL)
            GoToNewLevel(CLOUDCITYTRAPOUTRO_LDATA->idx);
    }
}

void CloudCityTrapC_Reset(WORLDINFO_s *world) {
    LevGameObject[0] = FindGameObject(id_DARTHVADER, 1, 1, 0, 0);
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
    LevPathCnx[0] =
        AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "gap1_a", "gap1_b", &LevPathCnxDir);
    GIZMO *gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle3");
    LevGizmo[0] = gizmo;
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[0] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
    gizmo = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "???");
    LevGizmo[1] = gizmo;
    if (gizmo != NULL && gizmo->object != NULL)
        LevGizObst[1] = static_cast<GIZOBSTACLE_s *>(gizmo->object);
}

void CloudCityTrapC_Update(WORLDINFO_s *) {
    if (netclient != 0 || LevPathCnx[0] == NULL)
        return;
    AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
    i32 direction = LevPathCnxDir;
    u32 big_jump = LEGO_AIPATHCNX_BIGJUMP;
    u32 glide = LEGO_AIPATHCNX_R2D2GLIDE;
    u32 flags = connection->traversal_flags[direction] & ~(big_jump | glide | 0x80000000);
    u32 active;
    if (LevGizObst[1] != NULL && LevGizObst[1]->anim_set->state != 0 && player->apiobj.collision_position.z < -20.0f)
        active = big_jump;
    else
        active = player->apiobj.collision_position.z < -21.0f ? big_jump : 0;
    if (LevGizObst[0] != NULL && LevGizObst[0]->anim_set->state != 0)
        active |= glide;
    if (active == 0)
        active = 0x80000000;
    connection->traversal_flags[direction] = flags | active;
}

void CloudCityTrapC_Panel(WORLDINFO_s *) {
    if (netclient == 0 && LevGameObject[0] != NULL && LevAIMessage[0] != NULL) {
        if (LevAIMessage[0]->value == 1.0f) {
            DrawBossHitPoints(LevGameObject[0]);
        } else {
            DrawBossHitPoints(NULL);
        }
    }
}

void CloudCityEscapeA_Init(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "BobaFightStarted", NULL);
    LEV_FLAG_U8[0] = 0;
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "gas_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "gas_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "gas_3", 1);
    LevGizPanel = GizPanel_FindByName(world, "panel5");
    GIZOBSTACLE_s *obstacle = GizObstacle_FindByName(world->giz_obstacle_sys, "obstacle23");
    if (obstacle != NULL)
        obstacle->field_a1_0xa1 |= 1;
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "blowup_fruit_1");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "blowup_palm_1");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "blowup_fruit_2");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "blowup_palm_2");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
    blowup = GizmoBlowUp_FindByName(world, "blowup_fruit_3");
    if (blowup != NULL) {
        blowup->field_0x128 = 0.5f;
        blowup->field_0x124 = 1;
    }
    blowup = GizmoBlowUp_FindByName(world, "blowup_palm_3");
    if (blowup != NULL)
        blowup->field_0x124 = 1;
}

void CloudCityEscapeA_Reset(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "BobaFightStarted", NULL);
    LevAIMessage[1] = CheckGizAIMessage(gizaimessagesys, "Built_C3PO", NULL);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizbuildit_gizmotype_id, "buildit2");
}

void CloudCityEscapeA_Update(WORLDINFO_s *) {
    if (LEV_FLAG_U8[0] == 0 && NuSpecialGetVisibilityFn(&LevHSpecial[0]) != 0) {
        PlaySfx("env_ctrl_desk_on", NuSpecialGetDrawPos(&LevHSpecial[0]));
        LEV_FLAG_U8[0] = 1;
    }
    TerSurface[14].flags = 0x2002;
    if (LevGizPanel == NULL || !LevGizPanel->state) {
        for (i32 index = 1; index < 4; ++index) {
            if (NuSpecialExistsFn(&LevHSpecial[index])) {
                nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[index]);
                if (animation != NULL && animation->ltime > 1.0f) {
                    PlaySfx("env_steam_lp", NuSpecialGetDrawPos(&LevHSpecial[index]));
                    TerSurface[14].flags |= 0x4042;
                }
            }
        }
    }
    GIZMO *gizmo = LevGizmo[0];
    if (gizmo != NULL && LevAIMessage[1] != NULL && LevAIMessage[1]->value == 0.0f && gizmo->object != NULL &&
        static_cast<GIZBUILDIT_s *>(gizmo->object)->build_state == 2)
        LevAIMessage[1]->value = 1.0f;
}

void CloudCityEscapeA_Panel(WORLDINFO_s *) {
    if (netclient == 0) {
        GameObject_s *boba = FindGameObject(id_BOBAFETT, 1, 1, 1, 0);
        if (boba != NULL && LevAIMessage[0] != NULL && LevAIMessage[0]->value == 1.0f) {
            DrawBossHitPoints(boba);
        } else if (LevAIMessage[0] != NULL && LevAIMessage[0]->value == 0.0f) {
            DrawBossHitPoints(NULL);
        }
    }
}

void CloudCityEscapeC_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "gas_1_animin", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "gas_2_animin", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "gas_3_animin", 1);
}

void CloudCityEscapeC_Update(WORLDINFO_s *) {
    TerSurface[14].flags = 0x2002;
    for (i32 index = 0; index < 3; ++index) {
        if (NuSpecialExistsFn(&LevHSpecial[index])) {
            nuinstanim_s *animation = NuSpecialGetInstAnim(&LevHSpecial[index]);
            if (animation != NULL && animation->ltime == 1.0f) {
                PlaySfx("env_steam_lp", NuSpecialGetDrawPos(&LevHSpecial[index]));
                TerSurface[14].flags |= 0x4042;
            }
        }
    }
}

i32 TakeOverYodaSeekDistanceHack(GameObject_s *object, GameObject_s *luke, nuvec_s *offset) {
    if ((object->field_0xf00 & 2) == 0 || dagobah_training == 0 || object->field_0xcc0 != NULL || luke == NULL ||
        (luke->apiobj.flags_low & 0x80) == 0)
        return 0;
    if (!(NuVecDistSqr(&object->apiobj.position, &luke->apiobj.position, offset) < 0.7f * 0.7f))
        return 0;
    NuVecRotateY(offset, offset, -static_cast<i32>(luke->apiobj.field_0x276));
    return offset->z > 0.0f;
}

void DagobahA_Init(WORLDINFO_s *world) {
    LevGizForce[0] = GizForce_FindByName(world->giz_force_sys, "force3");
    LevGizForce[1] = GizForce_FindByName(world->giz_force_sys, "force4");
    LevGizForce[2] = GizForce_FindByName(world->giz_force_sys, "force5");
    LevAIPathNode[0] = AIPathFindNode(world->ai_sys, NULL, "force1_a");
    LevAIPathNode[1] = AIPathFindNode(world->ai_sys, NULL, "force1_b");
    LevAIPathNode[2] = AIPathFindNode(world->ai_sys, NULL, "force1_c");
    i32 direction;
    LevPathCnx[0] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_a", "force1_b", &direction);
    LevPathCnx[1] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_b", "force1_c", &direction);
    LevPathCnx[2] = AIPAthFindPathCnx(world->ai_sys, NULL, "force1_c", "force1_d", &direction);
    dagobahA_nodesNeedUpdating = 1;
}

void DagobahA_Update(WORLDINFO_s *world) {
    GIZFORCE_s **forces = LevGizForce;
    GIZFORCE_s *first = forces[0];
    if (first == NULL)
        return;
    GIZFORCE_s *second = forces[1];
    if (second == NULL)
        return;
    GIZFORCE_s *third = forces[2];
    if (third == NULL)
        return;

    GIZFORCEGROUP_s *group = first->group;
    if (group != NULL && (group->field_0x24 & 2) != 0) {
        if (dagobahA_nodesNeedUpdating != 0)
            return;
        dagobahA_nodesNeedUpdating = 1;
        AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }
        connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }
        connection = static_cast<AIPATHCNX_s *>(LevPathCnx[2]);
        if (connection != NULL) {
            connection->traversal_flags[0] &= ~0x80000000;
            connection->traversal_flags[1] &= ~0x80000000;
        }

        AIPATHNODE_s *node0 = static_cast<AIPATHNODE_s *>(LevAIPathNode[0]);
        if (node0 == NULL)
            return;
        AIPATHNODE_s *node1 = static_cast<AIPATHNODE_s *>(LevAIPathNode[1]);
        if (node1 == NULL)
            return;
        AIPATHNODE_s *node2 = static_cast<AIPATHNODE_s *>(LevAIPathNode[2]);
        if (node2 == NULL)
            return;

        GIZFORCE_s *selected = group->forces[0];
        if (selected == first) {
            if (group->forces[1] == second) {
                node0->position.x = -16.25f;
                node0->position.y = 0.30f;
                node0->position.z = 15.17f;
                node1->position.x = -16.21f;
                node1->position.y = 0.98f;
                node1->position.z = 14.83f;
                node2->position.x = -16.19f;
                node2->position.y = 1.31f;
                node2->position.z = 14.64f;
            } else {
                node0->position.x = -16.93f;
                node0->position.y = 0.42f;
                node0->position.z = 14.75f;
                node1->position.x = -16.52f;
                node1->position.y = 0.64f;
                node1->position.z = 14.50f;
                node2->position.x = -16.32f;
                node2->position.y = 1.30f;
                node2->position.z = 14.70f;
            }
        } else if (selected == second) {
            node0->position.x = -16.31f;
            node0->position.y = 0.31f;
            node0->position.z = 15.13f;
            if (group->forces[1] == first) {
                node1->position.x = -16.31f;
                node1->position.y = 0.67f;
                node1->position.z = 14.82f;
                node2->position.x = -16.17f;
                node2->position.y = 1.31f;
                node2->position.z = 14.65f;
            } else {
                node1->position.x = -16.31f;
                node1->position.y = 0.67f;
                node1->position.z = 14.82f;
                node2->position.x = -16.36f;
                node2->position.y = 1.33f;
                node2->position.z = 14.54f;
            }
        } else if (selected == third) {
            if (group->forces[1] != first) {
                node0->position.x = -16.20f;
                node0->position.y = 0.31f;
                node0->position.z = 15.13f;
                node1->position.x = -16.30f;
                node1->position.y = 0.99f;
                node1->position.z = 14.80f;
                node2->position.x = -16.38f;
                node2->position.y = 1.31f;
                node2->position.z = 14.59f;
            } else {
                node0->position.x = -16.73f;
                node0->position.y = 0.51f;
                node0->position.z = 14.51f;
                node1->position.x = -16.49f;
                node1->position.y = 0.95f;
                node1->position.z = 14.45f;
                node2->position.x = -16.31f;
                node2->position.y = 1.29f;
                node2->position.z = 14.66f;
            }
        }
        if (world->ai_sys->path_sys != NULL && world->ai_sys->path_sys->active_path != NULL) {
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node0);
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node1);
            AIPathNodeUpdatePos(world->ai_sys, world->ai_sys->path_sys->active_path, node2);
        }
        return;
    }

    if (dagobahA_nodesNeedUpdating == 0)
        return;
    dagobahA_nodesNeedUpdating = 0;
    AIPATHCNX_s *connection = static_cast<AIPATHCNX_s *>(LevPathCnx[0]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
    connection = static_cast<AIPATHCNX_s *>(LevPathCnx[1]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
    connection = static_cast<AIPATHCNX_s *>(LevPathCnx[2]);
    if (connection != NULL) {
        connection->traversal_flags[0] |= 0x80000000;
        connection->traversal_flags[1] |= 0x80000000;
    }
}

void DagobahB_Init(WORLDINFO_s *) {
    dagobah_training = 0;
}

void DagobahB_Reset(WORLDINFO_s *world) {
    LevSafePlatID[1] = -1;
    LevSafePlatID[0] = -1;

    if (NuSpecialFind(world->current_gscn, &LevHSpecial[0], "pad_2_base_2", 1) != 0) {
        if (world->terrain != NULL) {
            LevSafePlatID[0] = FindPlatInst(NuSpecialGetInstanceix(&LevHSpecial[0]));
        }
    }

    if (NuSpecialFind(world->current_gscn, &LevHSpecial[1], "pad_4_base_2", 1) != 0) {
        if (world->terrain != NULL) {
            LevSafePlatID[1] = FindPlatInst(NuSpecialGetInstanceix(&LevHSpecial[1]));
        }
    }
}

void DagobahE_Init(WORLDINFO_s *world) {
    GIZAIMESSAGE_s *completed = CheckGizAIMessage(gizaimessagesys, "CompletedTraining", NULL);
    if (FreePlay == 0 && completed != NULL && completed->value == 0.0f) {
        dagobah_training = 1;
        DOOR_s *door = Door_FindByName(world, "door_e_to_b");
        if (door != NULL) {
            door->flags |= DOOR_FLAG_DO_NOT_USE;
        }
        door = Door_FindByName(world, "door_b_to_e");
        if (door != NULL) {
            door->flags |= DOOR_FLAG_DO_NOT_USE;
        }
    } else {
        dagobah_training = 0;
        DOOR_s *door = Door_FindByName(world, "door_e_to_b");
        if (door != NULL) {
            door->flags &= ~DOOR_FLAG_DO_NOT_USE;
        }
        door = Door_FindByName(world, "door_b_to_e");
        if (door != NULL) {
            door->flags &= ~DOOR_FLAG_DO_NOT_USE;
        }
    }
    SetGizAIMessage(gizaimessagesys, "DagobahTraining", static_cast<f32>(dagobah_training), NULL);
    SetGizAIMessage(gizaimessagesys, NULL, 1.0f, completed);
}

void DagobahC_Init(WORLDINFO_s *world) {
    Vader_obj = FindGameObject(id_DARTHVADER, 1, 1, 0, 0);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, force_gizmotype_id, "force20");
    GIZMOBLOWUP_s *blowup = GizmoBlowUp_FindByName(world, "Thermo_Box1");
    if (blowup != NULL) {
        blowup->draw_flags |= 2;
    }
}

void DagobahC_Panel(WORLDINFO_s *) {
    if (netclient == 0) {
        Vader_ai_message = CheckGizAIMessage(gizaimessagesys, "ShowHearts", NULL);
        if (Vader_obj != NULL && Vader_ai_message != NULL && Vader_ai_message->value == 1.0f) {
            DrawBossHitPoints(Vader_obj);
        }
    }
}

void ResetMinikitCounter() {
    minikitCounter_C = 0;
    minikitCounter_A = 0;
}

void IncrementMinikitCounter(GameObject_s *) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    NUVEC average;
    Players_AveragePos(&average, NULL);
    if (world->current_level == HOTHBATTLEA_LDATA) {
        IncrementMinikitCounterFor(world, &average, 0, &minikitCounter_A, 10.0f, 0xf1c8, 0xe000);
    } else if (world->current_level == HOTHBATTLEC_LDATA) {
        IncrementMinikitCounterFor(world, &average, 1, &minikitCounter_C, 25.0f, 0xeaab, 0x78e4);
    }
}

void HothBattleA_Init(WORLDINFO_s *world) {
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, gizmopickup_typeid, "m_pup3");
    GizmoSetVisibility(world->gizmo_sys, LevGizmo[0], 0, 1);
    trooper_boltid[1] = BoltType_FindIDByName("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByName("trooper_red", world);
    trooper_side[0] = 0;
    trooper_side[1] = 1;
    InitMiniSnowTroopers(world, 2, 32, 0);
    i32 count = NuSpecialFind(world->current_gscn, &LevHSpecial[0], "minifig_1_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[1], "minifig_1_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[2], "minifig_1_3", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[3], "minifig_2_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[4], "minifig_2_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[5], "minifig_2_3", 1);
    if (count == 6)
        hothtroopers = LevHSpecial;
}

void HothBattleA_Reset(WORLDINFO_s *world) {
    GIZMO *gizmo = LevGizmo[0];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if (minikitCounter_A == 10 && (pickup->state_flags & 8) == 0) {
        GizmoActivate(world->gizmo_sys, gizmo, 1, 1);
        return;
    }
    GizmoSetVisibility(world->gizmo_sys, gizmo, 0, 1);
}

void HothBattleA_Update(WORLDINFO_s *world) {
    UpdateMiniSnowTroopers(world);
}

void HothBattleA_Draw(WORLDINFO_s *world) {
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattleB_Init(WORLDINFO_s *world) {
    NuSpecialFind(world->current_gscn, &LevHSpecial[0], "snow_ball_1", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[1], "snow_ball_2", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[2], "snow_ball_3", 1);
    NuSpecialFind(world->current_gscn, &LevHSpecial[3], "snow_ball_4", 1);
}

void HothBattleC_Init(WORLDINFO_s *world) {
    LevAIMessage[0] = CheckGizAIMessage(gizaimessagesys, "BombGen_ATAT_Killed", NULL);
    i32 direction;
    LevPathCnx[0] =
        AIPAthFindPathCnx(world->ai_sys, world->ai_sys->path_sys->active_path, "ice_a", "ice_b", &direction);
    LevGizmo[0] = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, "obstacle3");
    LevGizmo[1] = GizmoFindByName(world->gizmo_sys, gizmopickup_typeid, "m_pup7");
    NuSpecialSetVisibility(&LevHSpecial[10], 0);
    trooper_boltid[1] = BoltType_FindIDByName("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByName("trooper_red", world);
    trooper_side[0] = 0;
    trooper_side[1] = 1;
    InitMiniSnowTroopers(world, 2, 32, 0);
    i32 count = NuSpecialFind(world->current_gscn, &LevHSpecial[0], "minifig_1_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[1], "minifig_1_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[2], "minifig_1_3", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[3], "minifig_2_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[4], "minifig_2_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[5], "minifig_2_3", 1);
    if (count == 6)
        hothtroopers = LevHSpecial;
}

void HothBattleC_Reset(WORLDINFO_s *world) {
    GIZMO *bomb_generator = GizmoFindByName(world->gizmo_sys, bombgen_gizmotype_id, "bomb_generator1");
    if (bomb_generator != NULL && bomb_generator->object != NULL) {
        HothBattleC_BombGenerator = static_cast<GIZBOMBGEN *>(bomb_generator->object);
    }

    GIZMO *gizmo = LevGizmo[1];
    if (gizmo == NULL || gizmo->object == NULL) {
        return;
    }

    GIZMOPICKUP_s *pickup = static_cast<GIZMOPICKUP_s *>(gizmo->object);
    if (minikitCounter_C == 10 && (pickup->state_flags & 8) == 0) {
        GizmoActivate(world->gizmo_sys, gizmo, 1, 1);
        return;
    }
    GizmoSetVisibility(world->gizmo_sys, gizmo, 0, 1);
}

void HothBattleC_Update(WORLDINFO_s *world) {
    if (netclient == 0 && HothBattleC_BombGenerator != NULL && !HothBattleC_BombGenerator->active &&
        LevAIMessage[0] != NULL && LevAIMessage[0]->value > 0.0f) {
        HothBattleC_BombGenerator->active = 1;
    }
    UpdateMiniSnowTroopers(world);
}

void HothBattleC_Draw(WORLDINFO_s *world) {
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattle_Melee_init(HOTHBATTLE_MELEE_s *melee) {
    if (melee != NULL) {
        melee->wave_delay = 0.0f;
        melee->field_0x0 = 0;
        melee->field_0x1 = 0;
        melee->field_0x2 = 1;
        melee->field_0x4 = -1;
    }
}

void HothBattleE_Draw(WORLDINFO_s *world) {
    if (NuIOS_IsLowEndDevice()) {
        return;
    }
    if (TimingBarSet == 5) {
        TBOPENFN("mini", 5);
    }
    DrawMiniSnowTroopers(world);
    if (TimingBarSet == 5) {
        TBCLOSEFN("mini", 5);
    }
}

void HothBattleE_Panel(WORLDINFO_s *) {
    if (MiniCutCam != 0) {
        return;
    }
    if (netclient != 0) {
        DrawMeleeTargetsRows(hothbattlee_netpacket->targets, hothbattlee_netpacket->defeated,
                             hothbattlee_netpacket->alpha, hothbattlee_netpacket->count);
        return;
    }

    i16 targets[16];
    char defeated[16];
    i32 count = 0;
    switch (melee.current_wave) {
        case 1:
        case 2:
            for (i32 index = 0; index < melee.waves[0].initial_count; ++index) {
                if (__builtin_expect(index == (melee.waves[0].initial_count >> 1) + 1, 1)) {
                    targets[count] = -1;
                    defeated[count] = 0;
                    ++count;
                }
                targets[count] = melee.waves[0].character_id;
                defeated[count] = index >= melee.waves[0].remaining_count;
                ++count;
            }
            break;
        case 3:
            for (i32 index = 0; index < melee.waves[0].initial_count; ++index) {
                targets[count] = melee.waves[0].character_id;
                defeated[count] = index >= melee.waves[0].remaining_count;
                ++count;
            }
            break;
        case 4:
            targets[count] = melee.waves[2].character_id;
            defeated[count] = melee.waves[2].remaining_count == 0;
            ++count;
            for (i32 index = 0; index < melee.waves[0].initial_count; ++index) {
                targets[count] = melee.waves[0].character_id;
                defeated[count] = index >= melee.waves[0].remaining_count;
                ++count;
            }
            targets[count++] = -1;
            for (i32 index = 0; index < melee.waves[1].initial_count; ++index) {
                targets[count] = melee.waves[1].character_id;
                defeated[count] = index >= melee.waves[1].remaining_count;
                ++count;
            }
            break;
    }
    for (i32 index = 0; index < count; ++index) {
        if (!(targets[index] != -1 && defeated[index] != 0)) {
            alpha[index] = 1.0f;
        } else {
            alpha[index] = SeekLinearF(alpha[index], 0.4f, 0.1f);
        }
    }
    DrawMeleeTargetsRows(targets, defeated, alpha, count);
}

i32 isHothBattleWaveCreature(GameObject_s *object) {
    for (i32 wave = 0; wave < 4; ++wave) {
        for (i32 creature = 0; creature < 4; ++creature) {
            if (melee.waves[wave].creatures[creature] == object)
                return 1;
        }
    }
    return 0;
}

AILOCATOR_s *getSpawnLocator(float clip_radius, char *name) {
    AILOCATORSET *locator_set = AIPathFindLocatorSet(WORLD->ai_sys, name);
    if (locator_set == NULL) {
        return NULL;
    }

    AILocatorSet_CheckLocatorsStillAssigned(WORLD->ai_sys, locator_set);
    AILOCATOR *locators[32];
    i32 i = 0;
    i32 locator_count = 0;
    for (; i < locator_set->locator_count && locator_count < 32; ++i) {
        if (locator_set->assigned[i] == 0xff) {
            locators[locator_count++] = &WORLD->ai_sys->locators[locator_set->locator_entries[i]];
        }
    }

    return LocalGetRandomLocator(locators, locator_count, clip_radius, &player->apiobj.collision_position,
                                 1000000000.0f, 0, 1000000000.0f, 1000000000.0f);
}

__attribute__((optimize("O2", "tree-vectorize"))) i32 SpawnMeleeCreatureType(i32 type) {
    u8 active_count = melee.waves[type].reserved_1a;
    f32 radius = apicharsys->char_data[melee.waves[type].character_id].collision_radius;

    for (i32 index = 0; index < active_count; ++index) {
        if (melee.waves[type].character_id != id_ATAT && melee.waves[type].creatures[index] != NULL &&
            melee.waves[type].creatures[index]->field_0xf1c > 5.0f) {
            KillGameObject(melee.waves[type].creatures[index], 4, 0);
            melee.waves[type].creatures[index] = NULL;
            active_count = --melee.waves[type].reserved_1a;
        }
    }

    while (melee.waves[type].field_0x18 > melee.waves[type].reserved_1a) {
        if (melee.waves[type].reserved_1a > 3)
            break;

        i32 slot = -1;
        i32 index = 0;
        do {
            GameObject_s *current = melee.waves[type].creatures[index];
            slot = current == NULL ? index : -1;
            ++index;
        } while (slot == -1 && index <= 3);
        if (slot == -1)
            return 0;

        if (melee.waves[type].character_id == id_ATAT && melee.waves[type].reserved_1a == 0) {
            GameObject_s *named = GetNamedGameObject(WORLD->ai_sys, "ATAT_EDIT");
            melee.waves[type].creatures[slot] = named;
            if (named != NULL && named->apiobj.field_0x287 == 0 && (named->apiobj.field_0x1f8 & 0x1000) != 0) {
                ++melee.waves[type].reserved_1a;
                continue;
            }
            melee.waves[type].creatures[slot] = NULL;
        }

        AILOCATOR *locator = NULL;
        for (i32 attempts = 50; attempts > 0; --attempts) {
            locator = getSpawnLocator(5.0f, "spawn");
            if (locator != NULL) {
                if (__builtin_expect(OnOrInsidePlane(&locator->position, &PlayPlane[1].point, &PlayPlane[1].normal,
                                                     NULL, 0.5f, NULL) == 0,
                                     0) ||
                    OnOrInsidePlane(&locator->position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 0.5f, NULL) ==
                        0) {
                    if (__builtin_expect(OnOrInsidePlane(&locator->position, &PlayPlane[1].point, &PlayPlane[1].normal,
                                                         NULL, 2.5f, NULL) != 0 ||
                                             OnOrInsidePlane(&locator->position, &PlayPlane[2].point,
                                                             &PlayPlane[2].normal, NULL, 2.5f, NULL) != 0,
                                         0))
                        break;
                }
                locator = NULL;
            }
        }
        if (locator == NULL)
            return 0;

        NUVEC offset = {(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) + 1.0f) * radius * 8.0f, 0.0f, 0.0f};
        NuVecRotateY(&offset, &offset, qrand());
        NuVecAdd(&offset, &offset, &locator->position);

        GameObject_s *object;
        if (melee.waves[type].character_id != id_ATAT) {
            object = AddDynamicCreature(melee.waves[type].character_id, &offset, locator->direction,
                                        melee.waves[type].name, &locator->path_info, NULL, 1, NULL, NULL, 0, 0);
        } else {
            object = AddDynamicCreature(melee.waves[type].character_id, &locator->position, locator->direction,
                                        melee.waves[type].name, &locator->path_info, NULL, 1, NULL, NULL, 0, 0);
        }
        if (__builtin_expect(object == NULL, 0))
            return 0;
        melee.waves[type].creatures[slot] = object;
        object->ai.locator = locator;
        ++melee.waves[type].reserved_1a;
    }
    return 1;
}

i32 HothBattle_StartNewWave() {
    if (melee.field_0x2 != 0) {
        switch (melee.field_0x1) {
            case 1:
                melee.waves[0].field_0x18 = 9;
                melee.waves[0].field_0x19 = 9;
                melee.waves[0].character_id = id_PROBEDROID;
                NuStrCpy(melee.waves[0].name, "Probe");
                melee.creature_count = 1;
                if (g_lowEndLevelBehaviour != 0) {
                    melee.waves[0].field_0x18 = 5;
                    melee.waves[0].field_0x19 = 5;
                }
                break;
            case 2:
                melee.waves[0].field_0x18 = 9;
                melee.waves[0].field_0x19 = 9;
                melee.waves[0].character_id = id_ATST_LOWRES;
                NuStrCpy(melee.waves[0].name, "rider");
                melee.creature_count = 1;
                if (g_lowEndLevelBehaviour != 0) {
                    melee.waves[0].field_0x18 = 5;
                    melee.waves[0].field_0x19 = 5;
                }
                break;
            case 3:
                melee.waves[0].field_0x18 = 2;
                melee.waves[0].field_0x19 = 2;
                melee.waves[0].character_id = id_ATAT;
                NuStrCpy(melee.waves[0].name, "ATAT");
                melee.creature_count = 1;
                break;
            case 4:
                melee.waves[0].field_0x18 = 3;
                melee.waves[0].field_0x19 = 3;
                melee.waves[0].character_id = id_PROBEDROID;
                NuStrCpy(melee.waves[0].name, "Probe");
                melee.waves[1].field_0x18 = 5;
                melee.waves[1].field_0x19 = 5;
                melee.waves[1].character_id = id_ATST_LOWRES;
                NuStrCpy(melee.waves[1].name, "rider");
                melee.waves[2].field_0x18 = 1;
                melee.waves[2].field_0x19 = 1;
                melee.waves[2].character_id = id_ATAT;
                NuStrCpy(melee.waves[2].name, "ATAT");
                melee.creature_count = 3;
                if (g_lowEndLevelBehaviour != 0) {
                    melee.waves[0].field_0x18 = 2;
                    melee.waves[0].field_0x19 = 2;
                    melee.waves[1].field_0x18 = 2;
                    melee.waves[1].field_0x19 = 2;
                }
                break;
        }
    }
    melee.field_0x2 = 0;
    if (MiniCutCam != 0)
        return 0;
    for (i32 type = 0; type < melee.creature_count; ++type)
        SpawnMeleeCreatureType(type);
    return 1;
}

void HothBattle_ManageBackgroundCreatures() {
    AILOCATORSET_s *set = AIPathFindLocatorSet(WORLD->ai_sys, "spawn");
    if (NOAICREATURES != 0 || melee.transition_phase != -1) {
        return;
    }
    u8 wave_types = 0;
    for (i32 type = 0; type < melee.creature_count; ++type) {
        i16 id = melee.waves[type].character_id;
        if (id == id_PROBEDROID) {
            wave_types |= 1;
        } else if (id == id_SPEEDERBIKESNOW) {
            wave_types |= 2;
        } else if (id == id_ATST_LOWRES) {
            wave_types |= 4;
        } else if (id == id_ATAT) {
            wave_types |= 8;
        }
    }
    i32 atst_count = g_lowEndLevelBehaviour != 0 ? 2 : 6;
    i32 probe_count = g_lowEndLevelBehaviour != 0 ? 2 : 5;
    f32 radius = apicharsys->char_data[id_PROBEDROID].collision_radius;
    while (probe_count > aicreature_sets_alive[0] && (wave_types & 1) == 0) {
        AILOCATOR_s *spawn = getSpawnLocator(radius, "spawn");
        if (spawn == NULL) {
            return;
        }
        GameObject_s *object = AddDynamicCreature(id_PROBEDROID, &spawn->position, spawn->direction, "Probe",
                                                  &spawn->path_info, NULL, 1, NULL, NULL, 0, 1);
        if (object == NULL) {
            break;
        }
        object->can_shoot_obstructions = 1;
        HothBattle_AssignSpawnLocator(object, spawn, set);
    }

    i32 last = -1;
    for (i32 index = 5; index >= 0; --index) {
        GameObject_s *object = melee.background_creatures[index];
        if (object == NULL) {
            continue;
        }
        if ((object->apiobj.field_0x1f8 & 0x1000) != 0 && object->apiobj.field_0x287 == 0 &&
            object->ai.creature_set == 3) {
            if (last == -1) {
                last = index;
            }
        } else if (last == -1) {
            melee.background_creatures[index] = NULL;
        } else {
            melee.background_creatures[index] = melee.background_creatures[last];
            melee.background_creatures[last] = NULL;
            --last;
        }
    }

    radius = apicharsys->char_data[id_ATST_LOWRES].collision_radius * 2.0f;
    while (atst_count > aicreature_sets_alive[2] && (wave_types & 4) == 0) {
        AILOCATOR_s *spawn = getSpawnLocator(radius, "spawn");
        if (spawn == NULL) {
            return;
        }
        GameObject_s *object = AddDynamicCreature(id_ATST_LOWRES, &spawn->position, spawn->direction, "rider",
                                                  &spawn->path_info, NULL, 1, NULL, NULL, 0, 3);
        melee.background_creatures[++last] = object;
        if (object == NULL) {
            return;
        }
        object->can_shoot_obstructions = 1;
        HothBattle_AssignSpawnLocator(object, spawn, set);
    }
}

void HothBattleE_UpdateWave() {
    i32 pending = 0;
    if (melee.current_wave != melee.next_wave) {
        if (melee.transition_phase == -1) {
            if (melee.wave_delay < 5.0f && melee.next_wave != 1) {
                melee.wave_delay += FRAMETIME;
                return;
            }
            if (melee.next_wave == 5) {
                memset(melee.waves, 0, sizeof(melee) - offsetof(HOTHBATTLE_MELEE_s, waves));
                if (FreePlay != 0) {
                    CompleteLevel(WORLD);
                } else {
                    GoToNewLevel(HOTHBATTLEOUTRO_LDATA->idx);
                }
            }
            melee.wave_delay = 0.0f;
            HothBattle_ClearBackgroundObjects();
            char state[16];
            if (g_lowEndLevelBehaviour != 0) {
                sprintf(state, "CamCutLow_%d", melee.next_wave);
            } else {
                sprintf(state, "CamCut_%d", melee.next_wave);
            }
            if (AIScriptSetBaseScriptStateByName(&WORLD->processors[0].processor, state) != 0) {
                AIScriptProcess(WORLD->ai_sys, NULL, NULL, &WORLD->processors[0].processor, FRAMETIME);
            }
            melee.transition_phase = 0;
            melee.wave_delay = 0.0f;
            melee.initialize_wave = 1;
            pending = 1;
        } else if (melee.transition_phase > 0) {
            if (__builtin_expect(MiniCutCam == 0, 1)) {
                if (melee.transition_phase == 1) {
                    HothBattle_ClearBackgroundObjects();
                    melee.transition_phase = 2;
                }
                if (HothBattle_StartNewWave() == 0) {
                    return;
                }
                melee.transition_phase = -1;
                melee.current_wave = melee.next_wave;
                pending = 1;
            }
        } else if (__builtin_expect(melee.transition_phase == 0 && MiniCutCam != 0, 0)) {
            melee.transition_phase = 1;
        }
    }

    i32 type;
    for (type = 0; type < melee.creature_count; ++type) {
        SpawnMeleeCreatureType(type);
    }
    HothBattle_ManageBackgroundCreatures();
    if (__builtin_expect(
            melee.current_wave == 3 && melee.waves[0].creatures[0] != NULL && melee.waves[0].creatures[1] != NULL, 0)) {
        AIANTINODE_s *node = AIAntinodeCreateSingleFrame(&melee.waves[0].creatures[0]->apiobj.collision_position,
                                                         melee.waves[0].creatures[type]->apiobj.field_0x1dc * 3.0f);
        node->excluded_character_types = ~(1 << melee.waves[0].creatures[1]->apiobj.field_0x289);
        node = AIAntinodeCreateSingleFrame(&melee.waves[0].creatures[1]->apiobj.collision_position,
                                           melee.waves[0].creatures[type]->apiobj.field_0x1dc * 3.0f);
        node->excluded_character_types = ~(1 << melee.waves[0].creatures[0]->apiobj.field_0x289);
    }
    for (i32 wave_index = 0; wave_index < 4; ++wave_index) {
        HOTHBATTLE_MELEE_WAVE_s *wave = &melee.waves[wave_index];
        i32 count = 0;
        for (i32 index = 0; index < 4; ++index) {
            GameObject_s *object = wave->creatures[index];
            if (object != NULL) {
                if ((object->apiobj.field_0x1f8 & 0x1000) == 0 || object->apiobj.field_0x287 != 0) {
                    --wave->active_count;
                    --wave->remaining_count;
                    wave->creatures[index] = NULL;
                    PlaySfxAndSetPitch("TrueJedi_100pc", NULL, 1.5f);
                } else {
                    ++count;
                }
            }
        }
        if (wave->active_count != count) {
            wave->active_count = static_cast<u8>(count);
        }
        if (wave->remaining_count != 0) {
            pending = 1;
        }
    }
    if (__builtin_expect((pending | MiniCutCam) == 0 && NOAICREATURES == 0 && melee.current_wave == melee.next_wave,
                         1)) {
        ++melee.next_wave;
    }
}

void HothBattleE_Update(WORLDINFO_s *world) {
    if (NuIOS_IsLowEndDevice() == 0)
        UpdateMiniSnowTroopers(world);
    if (netclient == 0)
        HothBattleE_UpdateWave();
}

void HothBattleE_Init(WORLDINFO_s *world) {
    trooper_boltid[1] = BoltType_FindIDByName("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByName("trooper_red", world);
    trooper_side[0] = 0;
    trooper_side[1] = 0;
    trooper_side[2] = 0;
    trooper_side[3] = 0;
    trooper_side[4] = 0;
    trooper_side[5] = 1;
    trooper_side[6] = 1;
    trooper_side[7] = 1;
    trooper_side[8] = 1;
    trooper_side[9] = 1;
    if (NuIOS_IsLowEndDevice() == 0)
        InitMiniSnowTroopers(world, 10, 32, 0);
    memset(melee.waves, 0, sizeof(melee) - offsetof(HOTHBATTLE_MELEE_s, waves));
    HothBattle_Melee_init(&melee);
    i32 count = NuSpecialFind(world->current_gscn, &LevHSpecial[0], "minifig_1_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[1], "minifig_1_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[2], "minifig_1_3", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[3], "minifig_2_1", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[4], "minifig_2_2", 1);
    count += NuSpecialFind(world->current_gscn, &LevHSpecial[5], "minifig_2_3", 1);
    if (count == 6)
        hothtroopers = LevHSpecial;
    if (netclient == 0)
        HothBattle_ManageBackgroundCreatures();
    hothbattlee_netpacket = static_cast<HOTHBATTLEE_NETPACKET_s *>(SetLevelHack(sizeof(HOTHBATTLEE_NETPACKET_s)));
}

