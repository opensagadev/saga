#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "decomp.h"
#include "legoapi/ai/game/creature.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/core/input/gamepads.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nuang.h"
#include "legoapi/actions/character/speederchase.h"
#include "legoapi/audio/audio.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/menus/screens/arcade.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legogame/game.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/actions/combat/fighting.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/characters/motion/contexts.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "gameapi/ai/aisys/aipath.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/traps/gizbombgen.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "nu2api/nucore/numechptr.hpp"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nu2api/nu3d/nuspline.h"
#include "gameframework/saveload.h"

// Game-object update, hit and part-kill code (retail game_obj.cpp).

extern f32 AIFireIntervalMul, BaseCurrentSpeed, CurrentSpeed, CurrentSpeedOverride, GhostLightMul, GhostLightTargetMul;
extern f32 DEFENDTIME, SHIELDFLICKERTIME, Hub_PadSpeed[2];
extern f32 tieonsfxwait, tieoffsfxwait;
extern char *ASCII_DOWN;
extern GameObject_s *CarWashHack;
extern i32 DoubleScore;
extern EXPLOSION Explosion[8];
extern i32 FalconDebKey[2];
extern i32 timebase_updates;
extern AREADATA_s *SPEEDERCHASE_ADATA;
extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
extern i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
extern i32 do_player_tag;
extern f32 player_tag_timer;
extern GameObject_s *player_tag_from, *player_tag_to;
extern i32 disable_narrow_socks, players_cannot_exit_speeder, BuildUpDone;
extern i16 id_ANAKINJEDI, id_ANAKINJEDISCARRED, id_GAMORREANGUARD, id_MOUSEDROID, id_OBIWANKENOBIEP3, id_PKDROID, id_SNAKE,
    id_WOOKIEE;
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;
extern BOLT_s *objhitobj_bolt;
extern i16 *objhitobj_killparts_yrot;
extern i32 objhitobj_noattackerrumble, objhitobj_nohurtsfx, objhitobj_noimpactsfx, objhitobj_throwkillpartsup;

EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);
void AddSurfaceDebris(GameObject_s *);
void AddSurfaceRipples(GameObject_s *);
extern "C" void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
void AveragePlayerCurrentSpeedMul();
void Batarangs_CheckLostData(void *);
i32 ForcePushed_SuperPush_Occurring(GameObject_s *, GameObject_s *);
void GameAIProcess();
void GameAntinode_Update(GAMEANTINODESYS_s *);
void GameObjectDimensions(GameObject_s *);
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void GetTakeOverPos(GameObject_s *, NUVEC *);
f32 GetVehicleHoverHeight(GameObject_s *, f32 *);
GIZMOPICKUP_s *GizmoPickups_Collide(WORLDINFO_s *, GameObject_s *, i32);
void Grapple_RemoveDynamic(void *);
void HitRumble(GameObject_s *);
i32 Hub_InMenu();
void KeepOnScreen(GameObject_s *);
void KillRumble(GameObject_s *);
void MovePlayer(GameObject_s *);
void MovePlayerSpline(GameObject_s *);
void MovePlayer_NETWORK(GameObject_s *);
i32 NewBlockAction(GameObject_s *);
extern "C" void NuSound3ClearLoopHold(NUVEC *, i32);
void PlayDieSfx(GameObject_s *);
void PlayHurtSfx(GameObject_s *);
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);
void PopBalloon(GameObject_s *);
void PowerUp_Update(GameObject_s *);
void RegenerateHearts(GameObject_s *);
void ReleaseEat(GameObject_s *);
void ReleaseForce(GameObject_s *, i32);
void ReleaseLever(GameObject_s *);
void ReleasePush(GameObject_s *);
void ResetAICreature(GameObject_s *, AISYS_s *);
void ResetCoinPacket(COINPACKET_s *);
void ResetPlayerMoves(GameObject_s *);
void ScaleGameObject(GameObject_s *);
void SetPlayer();
void SetProtocolDroidDeactivatedAction(GameObject_s *);
void SetToLastSafePos(GameObject_s *);
void Shards_HandleLostObj(WORLDINFO_s *, GameObject_s *);
void SnakeBeenHit(GameObject_s *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);
void StartFlatten(GameObject_s *, GameObject_s *);
i32 TagCode(GameObject_s *, GameObject_s *, i32, i32, i32);
void TakeHitRumble(GameObject_s *, f32);
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);
void TerrainPlayer(GameObject_s *);
void UpdateCoinPacket(COINPACKET_s *, i32, i32);
void UpdateRumble(RUMBLEPACKET *);

void TiePart_Kill(PART_s *, i32) asm("_ZL12TiePart_KillP6PART_si") __attribute__((visibility("hidden")));
void TiePart_Move(PART_s *, f32) asm("_ZL12TiePart_MoveP6PART_sf") __attribute__((visibility("hidden")));
void TiePart_Impact(PART_s *) asm("_ZL14TiePart_ImpactP6PART_s") __attribute__((visibility("hidden")));
void TiePart_KillExplode(PART_s *, i32) asm("_ZL19TiePart_KillExplodeP6PART_si") __attribute__((visibility("hidden")));
void TieSpinZPart_Move(PART_s *, f32) asm("_ZL17TieSpinZPart_MoveP6PART_sf") __attribute__((visibility("hidden")));
void AtatPart_Stop(PART_s *part) __asm__("_ZL13AtatPart_StopP6PART_s") __attribute__((visibility("hidden")));
void AtatPart_Update(PART_s *part) __asm__("_ZL15AtatPart_UpdateP6PART_s") __attribute__((visibility("hidden")));

static const u8 objhit_damage_joints[3] = {6, 8, 7};

struct BIKEPART_s {
    SPLINEPOS_s spline_position;
    u8 active;
    u8 pad_21[3];
    GameObject_s *rider;
    NUVEC rider_position;
};
DECOMP_ASSERT(sizeof(BIKEPART_s) == 0x34, "BIKEPART size");
DECOMP_ASSERT(offsetof(BIKEPART_s, active) == 0x20, "BIKEPART active offset");
DECOMP_ASSERT(offsetof(BIKEPART_s, rider) == 0x24, "BIKEPART rider offset");
DECOMP_ASSERT(offsetof(BIKEPART_s, rider_position) == 0x28, "BIKEPART rider position offset");
static BIKEPART_s bikeParts[8];

void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
void KillParts_SpeederBike(ADDPART_s *, i32, i32, GameObject_s *);
void KillParts_TIEFIGHTER(ADDPART_s *, i32, i32, GameObject_s *, i32, u16, u16, NUVEC *);
void KillParts_ATAT(ADDPART_s *, i32, i32, GameObject_s *);
static i32 SpeederPart_Draw(PART_s *);
static void SpeederPart_Kill(PART_s *, i32);
static void SpeederPart_Update(PART_s *);

static i32 lastrandkillmode;

static i32 GameObjectAIUpdateInterval(WORLDINFO_s *world, GameObject_s *object) {
    // The target updates characters outside the visible portal set every ten
    // frames.  This avoids doing a full script, controller and terrain pass
    // for every off-screen Cantina inhabitant on the same frame.
    const i16 room = object->room_id;
    if (world->rooms_visible_ptr[room] == 0 || object->apiobj.model_draw_result == 0) {
        return 10;
    }
    if (object->apiobj.field_0x27d == 0)
        return 1;

    const GAMECHARACTERDATA *character = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    const f32 distance = object->ai_update_distance;
    i32 interval = character->ai_update_interval_0;
    if (interval == 0)
        return 1;
    if (!(distance > character->ai_update_distance_0)) {
        interval = character->ai_update_interval_1;
        if (interval == 0)
            return 1;
        if (!(distance > character->ai_update_distance_1)) {
            interval = character->ai_update_interval_2;
            if (interval == 0)
                return 1;
            if (!(distance > character->ai_update_distance_2)) {
                interval = character->ai_update_interval_3;
                if (interval == 0 || !(distance > character->ai_update_distance_3)) {
                    return 1;
                }
            }
        }
    }

    // Zero requests an immediate update at every tier. Nonzero cadence
    // values are selected only beyond that tier's distance threshold.
    return interval < 7 ? interval : 7;
}


void CreatureCrate_MoveFn(PART_s *part, f32 elapsed) {
    part->crate_spawn_delay -= elapsed;
    if (part->crate_spawn_delay <= 0.0f) {
        part->move_callback = NULL;
        part->draw_callback = NULL;
    }
}

i32 CreatureCrate_DrawFn(PART_s *part) {
    return !(part->crate_spawn_delay > 0.0f);
}

static __used__ i32 SpeederPart_Draw(PART_s *) {
    return 1;
}

__used__ void TiePart_Impact(PART_s *part) {
    AddGameDebris(WORLD->debris_sys, 0x6a, &part->position);
}

__used__ void TiePart_Kill(PART_s *part, i32) {
    AddGameDebris(WORLD->debris_sys, 0x6a, &part->position);
}

void AtatPart_Stop(PART_s *part) {
    PlaySfx("EXPLODE1", &part->position);
    PartStop_Flickerer(part);
}

void AtatPart_Update(PART_s *part) {
    f32 choice = NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 100.0f + 1.0f;
    if (part->scale_time < 1.0f) {
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[118].effect, &part->position,
                                          static_cast<i32>(part->scale_time * 20.0f), FRAMETIME, 0, 0, NULL);
    } else if (part->scale_time < 2.0f) {
        if (choice < 10.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[120].effect, &part->position,
                                              static_cast<i32>(part->scale_time * 3.0f), FRAMETIME, qrand(), qrand(),
                                              NULL);
        } else if (choice > 90.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[120].effect, &part->position,
                                              static_cast<i32>(part->scale_time * 15.0f), FRAMETIME, qrand(), qrand(),
                                              NULL);
        }
    } else if (part->scale_time < 5.0f) {
        if (choice < 10.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[120].effect, &part->position, 15, FRAMETIME,
                                              qrand(), qrand(), NULL);
        } else if (choice > 70.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[120].effect, &part->position, 30, FRAMETIME,
                                              qrand(), qrand(), NULL);
        }
        if (choice < 30.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 1, FRAMETIME,
                                              qrand(), qrand(), NULL);
        } else if (choice < 70.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 3, FRAMETIME,
                                              qrand(), qrand(), NULL);
        } else {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 20, FRAMETIME,
                                              qrand(), qrand(), NULL);
        }
    } else if (part->scale_time < 10.0f) {
        if (choice < 30.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 1, FRAMETIME,
                                              qrand(), qrand(), NULL);
        } else if (choice < 70.0f) {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 3, FRAMETIME,
                                              qrand(), qrand(), NULL);
        } else {
            AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[119].effect, &part->position, 20, FRAMETIME,
                                              qrand(), qrand(), NULL);
        }
    }
}

static __used__ void SpeederPart_Update(PART_s *part) {
    f32 time = part->field_100 / part->field_104;
    f32 distance = Player[0]->apiobj.horizontal_velocity_magnitude * FRAMETIME * 1.1f;
    BIKEPART_s *bike = &bikeParts[static_cast<i32>(part->speeder_index)];
    GAMECAMERA_s *camera = GameCam;
    NUVEC previous_position;
    NUVEC position;
    NUVEC velocity;
    NUMTX_ALIGNED16 matrix;
    if (bike->spline_position.spline != NULL) {
        previous_position = bike->spline_position.position;
        MoveSplinePosition(&bike->spline_position, distance);
        u16 yaw, pitch;
        PointAlongSpline(bike->spline_position.spline, bike->spline_position.along, &position, &yaw, &pitch, 1);
        yaw -= 0x8000;
        if (distance < 0.0f) {
            yaw += 0x8000;
            pitch = -pitch;
        }
        f32 tilt = NU_SIN_LUT(static_cast<i32>(time * 32768.0f + 16384.0f));
        i32 spin =
            static_cast<i32>((NU_SIN_LUT(static_cast<i32>(time * 16384.0f + 32768.0f + 16384.0f)) + 1.0f) * 196608.0f);
        part->rotation_x =
            SeekRot(part->rotation_x, static_cast<i32>(-((1.0f - (tilt + 1.0f) * 0.5f) * 5461.0f)) - pitch, 10.0f);
        part->rotation_y =
            SeekRot(part->rotation_y, yaw + static_cast<i32>(2730.0f - (1.0f - fabsf(tilt)) * 5461.0f), 10.0f);
        part->field_13c = SeekRot(part->field_13c, spin + yaw, 10.0f);
        GameShadow(NULL, &bike->spline_position.position, 3.0f, 0);
        NuMtxSetIdentity(&matrix);
        NuMtxPreRotateX(&matrix, part->rotation_x);
        NuMtxPreRotateY(&matrix, part->rotation_y);
        NuMtxPreRotateZ(&matrix, part->field_13c);
        NuMtxTranslate(&matrix, &bike->spline_position.position);
        part->transform = matrix;
        NuVecSub(&velocity, &bike->spline_position.position, &previous_position);
        NuVecScale(&velocity, &velocity, 1.0f / FRAMETIME);
        part->velocity = velocity;
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[96].effect,
                                          &bikeParts[static_cast<i32>(part->speeder_index)].spline_position.position, 5,
                                          FRAMETIME, pitch, yaw, NULL);
    } else {
        previous_position = part->position;
        f32 tilt = fabsf(NU_SIN_LUT(static_cast<i32>(time * 32768.0f + 16384.0f)));
        i32 turn = static_cast<i32>(2730.0f - (1.0f - tilt) * 5461.0f);
        i32 spin =
            static_cast<i32>((NU_SIN_LUT(static_cast<i32>(time * 16384.0f + 32768.0f + 16384.0f)) + 1.0f) * 196608.0f);
        part->rotation_x = SeekRot(part->rotation_x, 0xd556, 10.0f);
        part->rotation_y = SeekRot(part->rotation_y, Player[0]->apiobj.field_0x276 + 0x4000, 10.0f);
        part->field_13c = SeekRot(part->field_13c, spin, 10.0f);
        GameShadow(NULL, &previous_position, 3.0f, 0);
        velocity = v001;
        NuVecRotateY(&velocity, &velocity, turn);
        NuVecScale(&velocity, &velocity, Player[0]->apiobj.horizontal_velocity_magnitude / FRAMETIME);
        velocity.y = 1.0f / NU_COS_LUT(part->rotation_x);
        position.x = SeekLinearF(previous_position.x, camera->mtx.m30, FRAMETIME * 1.8f);
        position.y = SeekLinearF(previous_position.y, camera->mtx.m31, FRAMETIME * 1.8f);
        position.z = SeekLinearF(previous_position.z, camera->mtx.m32, FRAMETIME * 1.8f);
        NuVecAdd(&position, &position, &velocity);
        NuMtxSetIdentity(&matrix);
        NuMtxPreRotateX(&matrix, part->rotation_x);
        NuMtxPreRotateY(&matrix, part->rotation_y);
        NuMtxPreRotateZ(&matrix, part->field_13c);
        NuMtxTranslate(&matrix, &position);
        part->transform = matrix;
        AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[96].effect, &position, 5, FRAMETIME,
                                          part->field_13c, part->rotation_x, NULL);
    }
    if (bike->rider != NULL) {
        if ((bike->rider->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
            bike->rider->field_0xefe |= 0x40;
            bike->rider->apiobj.flags_low |= 0x20;
            bike->rider->field_0xf00 |= 0x20;
            bike->rider_position.x = SeekLinearF(bike->rider_position.x, camera->mtx.m30, FRAMETIME);
            bike->rider_position.y = SeekLinearF(bike->rider_position.y, camera->mtx.m31 - 2.0f, FRAMETIME * 1.5f);
            bike->rider_position.z = SeekLinearF(bike->rider_position.z, camera->mtx.m32, FRAMETIME);
            bike->rider->apiobj.position = bike->rider_position;
            bike->rider->saved_position = bike->rider_position;
            bike->rider->apiobj.velocity = velocity;
            bike->rider->apiobj.pitch_angle +=
                static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 2730.0f);
            bike->rider->apiobj.field_0x276 +=
                static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 2730.0f);
            bike->rider->apiobj.roll_angle +=
                static_cast<i32>(NuFloatRand(reinterpret_cast<NURAND *>(&GAMERAND)) * 2730.0f);
            bike->rider->lighting_state.intensity[0].r = 1.0f;
            bike->rider->lighting_state.intensity[0].g = 1.0f;
            bike->rider->lighting_state.intensity[0].b = 1.0f;
            bike->rider->lighting_state.intensity[1].r = 1.0f;
            bike->rider->lighting_state.intensity[1].g = 1.0f;
            bike->rider->lighting_state.intensity[1].b = 1.0f;
        } else {
            bike->rider = NULL;
        }
    }
}

__used__ void TiePart_KillExplode(PART_s *part, i32) {
    AddGameDebris(WORLD->debris_sys, 0x6b, &part->position);
    AddPartDebris(WORLD->part_debris_sys, 3, &part->position);
}

static i32 Collide2Objects(APIOBJECT *first, APIOBJECT *second) {
    const f32 first_vertical_velocity = first->velocity.y;
    const f32 second_vertical_velocity = second->velocity.y;
    GameObject_s *first_object = first->objptr;
    GameObject_s *second_object = second->objptr;
    i32 collision_result = ForcePushed_SuperPush_Occurring(first_object, second_object);
    if (collision_result != 0) {
        collision_result =
            (second_object->apiobj.collision_identity_mask & first_object->apiobj.collision_contact_mask) != 0 ||
                    (first_object->apiobj.collision_identity_mask & second_object->apiobj.collision_contact_mask) != 0
                ? 1
                : 2;
        first->collision_contact_mask |= second->collision_identity_mask;
        second->collision_contact_mask |= first->collision_identity_mask;
        return collision_result;
    }
    if (WORLD->current_level == DOGFIGHTA_LDATA) {
        collision_result = APIObjectCollision(first, second);
    } else {
        collision_result = APIObjectCollision2D(first, second);
    }

    if (collision_result == 2 && VehicleArea == 0) {
        GAMECHARACTERDATA *second_data =
            static_cast<GAMECHARACTERDATA *>(second_object->apiobj.character_data->field11_0x24);
        GAMECHARACTERDATA *first_data =
            static_cast<GAMECHARACTERDATA *>(first_object->apiobj.character_data->field11_0x24);
        if (!((first_data->flags_090 & 0x1000) != 0 &&
              first->horizontal_velocity_magnitude > first_data->movement_speed * 0.5f)) {
            if ((second_data->flags_090 & 0x1000) != 0 &&
                second->horizontal_velocity_magnitude > second_data->movement_speed * 0.5f) {
                StartFlatten(second_object, first_object);
            } else if (second_vertical_velocity < 0.0f && (first_data->flags_090 & 0x40) != 0 &&
                       first_object->field_0x107c == -1) {
                first->velocity.y += second_vertical_velocity * 0.5f;
            } else if (first_vertical_velocity < 0.0f && (second_data->flags_090 & 0x40) != 0 &&
                       second_object->field_0x107c == -1) {
                second->velocity.y += first_vertical_velocity * 0.5f;
            }
        } else {
            StartFlatten(first_object, second_object);
        }
    }

    f32 maximum_velocity = first->character_data->game_character->movement_speed * 1.25f;
    if (maximum_velocity < 1.0f) {
        maximum_velocity = 1.0f;
    }
    f32 velocity_squared = first->velocity.x * first->velocity.x + first->velocity.z * first->velocity.z;
    if (velocity_squared > maximum_velocity * maximum_velocity) {
        const f32 scale = maximum_velocity / NuFsqrt(velocity_squared);
        first->velocity.x *= scale;
        first->velocity.z *= scale;
    }

    maximum_velocity = second->character_data->game_character->movement_speed * 1.25f;
    if (maximum_velocity < 1.0f) {
        maximum_velocity = 1.0f;
    }
    velocity_squared = second->velocity.x * second->velocity.x + second->velocity.z * second->velocity.z;
    if (velocity_squared > maximum_velocity * maximum_velocity) {
        const f32 scale = maximum_velocity / NuFsqrt(velocity_squared);
        second->velocity.x *= scale;
        second->velocity.z *= scale;
    }
    return collision_result;
}

void CreatureCrate_Stop(PART_s *part) {
    GameObject_s *object = part->owner;
    if (object == NULL)
        return;
    i16 debris = FindGameDebris(WORLD->debris_sys, "CRATE_POP");
    if (debris != -1)
        AddGameDebris(WORLD->debris_sys, debris, &part->position);
    i32 type = PARTLookupType("CRATE_PART");
    if (type != -1)
        AddFiniteShotPART(type, &part->position, 1);
    object->ai.reset_mode = 2;
    object->apiobj.flags_high |= APIOBJECT_HIGH_FLAG_CHARACTER;
    PlaySfx("Explode1", &part->position);
    GameCam_Judder(GameCam, 0.1f, 0, NULL);
    NewRumbleAllPlayers(0.0f, 0.0f, 2, 0);
}

__used__ void TieSpinZPart_Move(PART_s *part, f32 time) {
    static NUVEC vec = {0.0f, 0.0f, -0.05f};
    part->field_124[3] = 200000;
    part->field_13c = static_cast<i32>(200000.0f * FRAMETIME);
    NuMtxPreTranslate(&part->transform, &vec);
    NUVEC position;
    position.x = part->position.x + part->velocity.x * time;
    position.y = part->position.y + part->velocity.y * time;
    position.z = part->position.z + part->velocity.z * time;
    NuMtxRotateZ(&part->transform, part->field_13c);
    part->position.x = position.x;
    part->position.y = position.y;
    part->position.z = position.z;
    AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[100].effect, &position, 10, FRAMETIME, 0, 0, NULL);
    position.x = part->velocity.x * 0.1f;
    position.y = part->velocity.y * 0.1f;
    position.z = part->velocity.z * 0.1f;
    AddVariableShotDebrisEffectTimed3(WORLD->debris_sys->entries[96].effect, &part->position, &position, 10, FRAMETIME,
                                      NULL, NULL);
}

__used__ void TiePart_Move(PART_s *part, f32 time) {
    part->field_124[3] = -32768;
    part->field_13c = static_cast<u16>(-32768.0f * FRAMETIME);
    NUVEC position;
    position.x = part->position.x + part->velocity.x * time;
    position.y = part->position.y + part->velocity.y * time;
    position.z = part->position.z + part->velocity.z * time;
    NuMtxRotateZ(&part->transform, part->field_13c);
    part->position.x = position.x;
    part->position.y = position.y;
    part->position.z = position.z;
    AddVariableShotDebrisEffectTimed1(WORLD->debris_sys->entries[100].effect, &position, 10, FRAMETIME, 0, 0, NULL);
}

void SpawnCreatureFromCrate(GameObject_s *object, f32 height, f32 delay) {
    NUVEC velocity = {0.0f, 0.0f, 0.0f};
    if (!NuSpecialExistsFn(&WORLD->lev_objs[0xe6].special))
        return;
    NUMTX matrix;
    NuMtxSetTranslation(&matrix, &object->apiobj.collision_position);
    ADDPART_s parameters = Default_ADDPART;
    parameters.flags = 0x313;
    parameters.matrix = &matrix;
    parameters.velocity = &velocity;
    matrix.m31 += height;
    parameters.special = &WORLD->lev_objs[0xe6].special;
    parameters.owner = object;
    parameters.field_14 = 0.1f;
    parameters.field_18 = 0.1f;
    parameters.gravity = -2.0f;
    parameters.draw_fn = CreatureCrate_DrawFn;
    parameters.move_fn = CreatureCrate_MoveFn;
    parameters.stop_fn = CreatureCrate_Stop;
    parameters.time_step = FRAMETIME;
    PART_s *part = AddPart(&parameters);
    if (part == NULL)
        return;
    part->force_flags = static_cast<u16>(ObjHitObj_Flags(object));
    if (!(delay > 0.0f)) {
        part->move_callback = NULL;
        part->draw_callback = NULL;
        delay = 0.0f;
    }
    part->crate_spawn_delay = delay;
    object->ai.reset_mode = 3;
    object->apiobj.field_0x1f8 &= ~0x1000;
}

void ManageGameObjects() {
    GameObject_s *object = Obj;
    memset(aicreature_sets_alive, 0, sizeof(aicreature_sets_alive));
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *candidate = &object[i];
        if ((candidate->apiobj.field_0x1f8 & 0x1000) != 0 && candidate->apiobj.field_0x287 == 0 &&
            candidate->ai.creature_set != 0)
            ++aicreature_sets_alive[candidate->ai.creature_set - 1];
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 1) == 0)
            continue;
        if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
            if (nethost != 0 && (object->apiobj.field_0x1f8 & 0x1000) == 0 && object->ai.field_0x134 == 0xff &&
                (object->ai.reset_mode & ~2) != 1 && (object->apiobj.field_0x1f4 & 0x4000) != 0)
                AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, &object->ai.script_process, FRAMETIME);
            continue;
        }
        if ((object->apiobj.field_0x1f8 & 0x1000) != 0) {
            if (object->apiobj.field_0x287 == 0)
                continue;
            if (object->field_0xeb4 != NULL) {
                object->field_0xeb4(object);
                object->field_0xeb4 = NULL;
            }
            if (object->field_0x101c > 0.0f) {
                object->field_0x101c -= FRAMETIME;
                if (object->field_0x101c <= 0.0f) {
                    object->field_0x101c = 0.0f;
                    GameObject_s *other = NULL;
                    if (WORLD->current_level == PODSPRINTA_LDATA)
                        other = GetOtherActivePlayer(object);
                    if (other != NULL) {
                        object->ai_update_position = object->apiobj.position = object->apiobj.start_position =
                            other->apiobj.position;
                        GameObjectOrigin(object);
                        object->apiobj.velocity = other->apiobj.velocity;
                        object->apiobj.field_0x276 = object->apiobj.facing_angle =
                            object->apiobj.movement_facing_angle = other->apiobj.field_0x276;
                    } else {
                        SetToLastSafePos(object);
                        GameObjectOrigin(object);
                        object->apiobj.velocity.x = object->apiobj.velocity.z = 0.0f;
                        object->apiobj.velocity.y = -0.1f;
                        object->apiobj.facing_angle = object->apiobj.movement_facing_angle;
                        object->apiobj.field_0x276 = object->apiobj.movement_facing_angle;
                    }
                    u16 saved_flags = object->apiobj.field_0x1f8 & 0x2000;
                    ResetPlayerMoves(object);
                    object->apiobj.field_0x287 = 0;
                    object->apiobj.field_0x1f8 = (object->apiobj.field_0x1f8 & ~0x2000) | saved_flags;
                    object->current_hp = object->hitpoints;
                    object->field_0xe38 = 4;
                    object->field_0xe37 = object->apiobj.character_data->game_character->field_0xf5;
                    object->field_0xefc |= 0x80;
                    object->field_0x7a5 = 0xff;
                    if (object->apiobj.field_0x27c != -1)
                        object->spawn_protection_timer = 2.5f;
                    AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
                    GameObject_s *vehicle = object->takeover_source;
                    if (vehicle != NULL && (vehicle->field_0xcc0 == NULL || vehicle->field_0xcc0 == object) &&
                        (vehicle->apiobj.field_0x1f8 & 0x1000) != 0 && vehicle->field_0x7a5 != 0x2b &&
                        vehicle->apiobj.field_0x287 == 0) {
                        GetTakeOverPos(vehicle, &object->apiobj.position);
                        TakeOverGameObject(object, object->takeover_source, 0, 1);
                    }
                    if (object->apiobj.player_controlled && WORLD->current_level == SPEEDERCHASEA_LDATA &&
                        disable_narrow_socks == 0)
                        GameCam_Blend(GameCam, 0.5f, 0.0f, 0);
                } else if (WORLD->current_level == PODSPRINTA_LDATA) {
                    GameObject_s *other = GetOtherActivePlayer(object);
                    if (other != NULL)
                        object->apiobj.position = other->apiobj.position;
                }
                continue;
            }
            object->timer_1014 += FRAMETIME;
            if (!(object->timer_1014 >= object->field_0x1018))
                continue;
            object->timer_1014 = object->field_0x1018;
            GameObject_s *other = Obj;
            for (i32 j = 0; j < HIGHGAMEOBJECT; ++j, ++other) {
                other->apiobj.collision_contact_mask &= ~object->apiobj.collision_identity_mask;
                other->apiobj.ai_awareness_mask &= ~(1ULL << object->apiobj.field_0x289);
                other->ai_seen_mask &= ~(1ULL << object->apiobj.field_0x289);
                other->ai_opponent_exclusion_mask &= ~(1ULL << object->apiobj.field_0x289);
            }
            if ((object->apiobj.field_0x1f4 & 0x4000) != 0) {
                if ((object->apiobj.field_0x1f4 & 0x400) == 0) {
                    object->field_0x101c = 2.0f;
                } else {
                    AICREATURE *creature = &WORLD->ai_sys->creatures[object->ai.field_0x134];
                    if ((object->field_0xefa & 0x10) != 0) {
                        if ((object->field_0xefa & 0x20) == 0) {
                            if (object->id == id_STAP2 && WORLD->current_level == NEGOTIATIONSC_LDATA) {
                                object->field_0x101c = 5.0f;
                            } else {
                                object->field_0x101c = 1.0f;
                            }
                            continue;
                        }
                    } else {
                        u32 limit = object->ai_respawn_count + 1;
                        if (creature->max_respawn_count != -1 && object->ai.respawn_locator == NULL)
                            limit = creature->min_respawn_count +
                                    (Game.difficulty - 1) *
                                        (creature->max_respawn_count - creature->min_respawn_count) / 9 +
                                    1;
                        if (limit <= object->ai_respawn_count) {
                            object->apiobj.field_0x1f8 &= ~0x1000;
                            object->ai.reset_mode = 4;
                            continue;
                        }
                    }
                    object->apiobj.field_0x1f8 &= ~0x1000;
                    object->ai.reset_mode = 1;
                    f32 blend = 1.0f - (static_cast<f32>(static_cast<u32>(Game.difficulty)) - 1.0f) / 9.0f;
                    object->ai_spawn_delay =
                        creature->max_respawn_time * blend + creature->min_respawn_time * (1.0f - blend);
                }
                continue;
            }
            if ((object->field_0xefa & 0x10) != 0) {
                object->field_0x101c = 1.0f;
                continue;
            }
            goto remove_object;
        }
        if (object->ai.field_0x134 != 0xff) {
            AISYS *system = WORLD->ai_sys;
            AICREATURE *creature = &system->creatures[object->ai.field_0x134];
            if (object->ai.reset_mode == 0) {
                switch (creature->activate_type) {
                    case 0:
                        if (Game.difficulty >= creature->activation_difficulty)
                            ResetAICreature(object, system);
                        else
                            object->ai.reset_mode = 4;
                        break;
                    case 1:
                        if (creature->activate_area == NULL) {
                            creature->activate_type = 0;
                        } else {
                            i32 area = creature->activate_area - system->areas;
                            i64 mask = static_cast<i32>(1u << (area & 31));
                            if (player != NULL && (player->apiobj.ai_area_mask & mask) != 0) {
                                if (Game.difficulty < creature->activation_difficulty) {
                                    object->ai.reset_mode = 4;
                                } else if (creature->count > 1 && creature->start_stagger > 0.0f &&
                                           object->ai.group_member_index != 0) {
                                    object->ai.reset_mode = 1;
                                    object->ai_spawn_delay =
                                        static_cast<f32>(static_cast<u32>(object->ai.group_member_index)) *
                                        creature->start_stagger;
                                } else {
                                    ResetAICreature(object, system);
                                }
                            }
                        }
                        break;
                    case 2:
                        if (Game.difficulty >= creature->activation_difficulty)
                            AIScriptProcess(system, &object->apiobj, &object->ai, &object->ai.script_process,
                                            FRAMETIME);
                        break;
                }
            } else if (object->ai.reset_mode == 1) {
                AIGROUP *group = object->ai.group;
                if (group != NULL && !group->can_respawn)
                    continue;
                if (creature->activate_type == 1) {
                    i32 area = creature->activate_area - system->areas;
                    i64 mask = static_cast<i32>(1u << (area & 31));
                    if (player == NULL || (player->apiobj.ai_area_mask & mask) == 0 ||
                        Game.difficulty < creature->activation_difficulty) {
                        if (creature->count > 1 && creature->start_stagger > 0.0f)
                            object->ai_spawn_delay = static_cast<f32>(static_cast<u32>(object->ai.group_member_index)) *
                                                     creature->start_stagger;
                        continue;
                    }
                }
                object->ai_spawn_delay -= FRAMETIME;
                if (!(object->ai_spawn_delay <= 0.0f))
                    continue;
                if (group != NULL) {
                    AILOCATOR *locator = object->ai.locator;
                    AILOCATOR *respawn_locator = object->ai.respawn_locator;
                    for (i32 j = 0; j < group->member_count; ++j) {
                        APIOBJECT *member = group->members[j];
                        if (member != NULL) {
                            GameObject_s *target = member->objptr;
                            target->ai_spawn_delay = 0.0f;
                            target->ai.respawn_locator = respawn_locator;
                            target->ai.locator = locator;
                            ResetAICreature(target, WORLD->ai_sys);
                            group = object->ai.group;
                            if (group->leader == NULL)
                                group->leader = group->members[j];
                        }
                        group = object->ai.group;
                    }
                    group->is_in_formation = 0;
                    object->ai.group->is_reversed = 0;
                } else {
                    object->ai_spawn_delay = 0.0f;
                    if (object->id != id_PKDROID) {
                        ResetAICreature(object, WORLD->ai_sys);
                    } else {
                        ResetAICreature(object, system);
                        SpawnCreatureFromCrate(object, 3.0f, 0.0f);
                    }
                }
            }
            continue;
        }
        if (object->ai.reset_mode == 1) {
            object->ai_spawn_delay -= FRAMETIME;
            if (object->ai_spawn_delay <= 0.0f) {
                object->ai.reset_mode = 2;
                object->ai_spawn_delay = 0.0f;
                object->apiobj.field_0x1f8 |= 0x1000;
            }
            continue;
        }
        if (object->ai.reset_mode == 3)
            continue;
        if ((object->apiobj.field_0x1f4 & 0x4000) != 0) {
            AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, &object->ai.script_process, FRAMETIME);
            continue;
        }
    remove_object:
        AIGROUP *group = object->ai.group;
        if (group != NULL) {
            if (group->rows[0].is_alive == 0 && group->rows[1].is_alive == 0 && group->rows[2].is_alive == 0 &&
                group->rows[3].is_alive == 0) {
                DestroyAIGroup(group);
            } else {
                auto group_member_count = group->member_count;
                for (i32 j = 0; j < group_member_count; ++j) {
                    if (group->members[j] == &object->apiobj)
                        group->members[j] = NULL;
                }
            }
        }
        FreeTorpedoPacket(&object->torpedo);
        RemoveGameObject(object, 1);
    }
}

void CollideGameObjects(WORLDINFO_s *world) {
    if (world->level_sub_id != -1 && (world->area->flags & 0x80) != 0) {
        return;
    }

    u32 narrow_speeder_mask = 0;
    u32 flattened_mask = 0;
    u32 player_collision_mask = 0;
    u32 no_vertical_movement_mask = 0;
    u32 vertical_movement_mask = 0;
    u32 player_slot_mask = 0;

    i32 object_count = HIGHGAMEOBJECT;
    GameObject_s *objects = Obj;
    GameObject_s *object = objects;
    for (i32 index = 0; index < object_count; ++index, ++object) {
        const i32 flags = object->apiobj.flags_low;
        if ((flags & APIOBJECT_FLAG_IN_USE) == 0) {
            continue;
        }

        const u32 bit = static_cast<u32>(static_cast<u64>(1) << object->apiobj.field_0x289);
        if (object->apiobj.field_0x27c != -1) {
            player_slot_mask |= bit;
        }
        GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
        if (!(data->field_0x28 > 0.0f)) {
            no_vertical_movement_mask |= bit;
        } else {
            vertical_movement_mask |= bit;
        }
        if (((flags & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 || (object->field_0xf02 & 8) != 0) &&
            (data->flags_090 & 0x40) == 0) {
            player_collision_mask |= bit;
        }
        if (object->character_context == 0x3d && (flags & APIOBJECT_FLAG_PLAYER_ACTIVE) == 0) {
            flattened_mask |= bit;
        }
        if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 && object->id == id_SPEEDERBIKE &&
            object->apiobj.field_0x27c != -1) {
            narrow_speeder_mask |= bit;
        }
    }

    APIOBJECT *collision_objects[64];
    NUVEC collision_maximums[64];
    NUVEC collision_minimums[64];
    APIOBJECT **object_cursor = collision_objects;
    NUVEC *minimum_cursor = collision_minimums;
    NUVEC *maximum_cursor = collision_maximums;
    i32 collision_count = 0;
    object = objects;
    for (i32 index = 0; index < object_count; ++index, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 || object->apiobj.field_0x287 != 0 ||
            object->apiobj.model_draw_result == 0 || object->use_model_origin <= 1) {
            continue;
        }

        const i8 context = object->character_context;
        if ((CInfo[context].flags & 0x8000) != 0 || context == 0x3b || context == 0x3c || context == 0x0f ||
            context == 0x39) {
            continue;
        }
        if (!(context == 0x26 && object->field_0x7a7 != -1 && (object->context_variant_flags & 2) != 0)) {
            if (context == 0x1f && object->field_0x7a3 == 1) {
                continue;
            }

            GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
            if ((data->flags_090 & 0x8000) != 0 || (object->action_flags & 0x20) != 0 ||
                (context == 0x47 && object->field_0x788 != NULL &&
                 (*reinterpret_cast<u8 *>(static_cast<u8 *>(object->field_0x788) + 0x68) & 1) != 0)) {
                continue;
            }

            ++collision_count;
            *minimum_cursor++ = object->apiobj.collision_min;
            *maximum_cursor++ = object->apiobj.collision_max;
            *object_cursor++ = &object->apiobj;

            object->apiobj.collision_mask_low = 0;
            object->apiobj.collision_mask_high = 0;
            if ((data->flags_090 & 0x1000) != 0) {
                object->apiobj.collision_mask_low = flattened_mask;
            }

            if (object->field_0x107c != -1) {
                object->apiobj.collision_mask_low |= player_collision_mask;
                for (i32 player_index = 0; player_index < 8; ++player_index) {
                    GameObject_s *player = Player[player_index];
                    if (player == NULL || (player->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player == object ||
                        !(player->field_0xdc4 > 0.0f) ||
                        player->apiobj.supporting_platform_id != object->field_0x107c ||
                        (object->field_0x107c == player->field_0x1078 &&
                         (player->apiobj.field_0x27d != 0 || GameObjectNearFloor(player, 3.0f, NULL) != 0))) {
                        continue;
                    }
                    NUVEC direction = {object->apiobj.collision_position.x - player->apiobj.collision_position.x, 0.0f,
                                       object->apiobj.collision_position.z - player->apiobj.collision_position.z};
                    NuVecNorm(&direction, &direction);
                    object->apiobj.movement_direction.x = direction.x;
                    object->apiobj.movement_direction.z = direction.z;
                }
            }

            if (object->character_context == CHARACTER_CONTEXT_JUMP && object->airborne_collision_target != NULL) {
                const u64 bit = static_cast<u64>(1) << object->airborne_collision_target->apiobj.field_0x289;
                object->apiobj.collision_exclusion_mask |= bit;
            }
            if (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0 &&
                object->id == id_SPEEDERBIKE && object->apiobj.field_0x27c != -1) {
                object->apiobj.collision_mask_low |= narrow_speeder_mask;
            }

            if (VehicleArea != 0 && WORLD->current_level != BOUNTYHUNTERPURSUITA_LDATA &&
                WORLD->current_level != DOGFIGHTA_LDATA) {
                data = object->apiobj.character_data->game_character;
                if (object->apiobj.field_0x27c == -1 && data->field_0x28 != 0.0f) {
                    object->apiobj.collision_mask_low |=
                        (vertical_movement_mask & player_slot_mask) | no_vertical_movement_mask;
                } else if (data->field_0x28 == 0.0f) {
                    object->apiobj.collision_mask_low |= (~player_slot_mask) & vertical_movement_mask;
                } else {
                    object->apiobj.collision_mask_low |= vertical_movement_mask;
                }
            }
        }
    }

    APIObjectCollisions(collision_count, collision_objects, collision_minimums, collision_maximums, Collide2Objects);
}

// Original 0x133040, 5,608 bytes. Suspension state shares the action packet.
void GameObjectRotation(GameObject_s *object, i32 mode) {
    APIOBJECT &api = object->apiobj;
    GAMECHARACTERDATA_s *game = api.character_data->game_character;
    u32 flags = game->flags_090;
    if ((flags & 0x200000) != 0) {
        static NUVEC buggy[4] = {
            {-0.1175f, 0, 0.1575f}, {0.1175f, 0, 0.1575f}, {0.1175f, 0, -0.1575f}, {-0.1175f, 0, -0.1575f}};
        static NUVEC radio[4] = {{-0.12f, 0, 0.08f}, {0.12f, 0, 0.08f}, {0.12f, 0, -0.08f}, {-0.12f, 0, -0.08f}};
        static NUVEC police[4] = {{-0.19f, 0, 0.31f}, {0.19f, 0, 0.31f}, {0.19f, 0, -0.31f}, {-0.19f, 0, -0.31f}};
        static NUVEC mower[4] = {
            {-0.1175f, 0, 0.155f}, {0.1175f, 0, 0.155f}, {0.1175f, 0, -0.155f}, {-0.1175f, 0, -0.155f}};
        static NUVEC standard[4] = {{-0.2f, 0, 0.2f}, {0.2f, 0, 0.2f}, {0.2f, 0, -0.2f}, {-0.2f, 0, -0.2f}};
        const f32 dy = api.position.y - api.start_position.y;
        const f32 vertical_speed = dy / FRAMETIME;
        NUVEC *offsets = object->id == id_BUGGY       ? buggy
                         : object->id == id_RADIOCAR  ? radio
                         : object->id == id_POLICECAR ? police
                         : object->id == id_MOWER     ? mower
                                                      : standard;
        NUVEC points[4];
        for (i32 i = 0; i < 4; ++i) {
            NuVecRotateY(&points[i], &offsets[i], api.field_0x276);
            NuVecAdd(&points[i], &points[i], &api.lower_position);
            const f32 floor = GameShadow(NULL, &points[i], 5.0f, -1);
            if (floor == 2000000.0f) {
                object->suspension[i].height = api.lower_position.y;
                object->suspension[i].velocity = dy / FRAMETIME;
            } else {
                object->suspension[i].velocity += -5.0f * FRAMETIME;
                const f32 height = object->suspension[i].height + dy + object->suspension[i].velocity * FRAMETIME;
                if (floor > height) {
                    object->suspension[i].height = floor;
                    object->suspension[i].velocity = vertical_speed;
                } else {
                    object->suspension[i].height = height;
                }
                const f32 lower = api.lower_position.y - (api.field_0x1dc + api.field_0x1dc);
                if (lower > object->suspension[i].height) {
                    object->suspension[i].height = lower;
                    object->suspension[i].velocity = vertical_speed;
                } else {
                    const f32 upper = api.lower_position.y + 3.0f * api.field_0x1dc;
                    if (object->suspension[i].height > upper) {
                        object->suspension[i].height = upper;
                        object->suspension[i].velocity = vertical_speed;
                    }
                }
            }
        }
        NUVEC normal = v000;
        for (i32 i = 0; i < 4; ++i) {
            NUVEC a = points[i], b = points[(i + 1) & 3], c = points[(i + 2) & 3];
            a.y = object->suspension[i].height;
            b.y = object->suspension[(i + 1) & 3].height;
            c.y = object->suspension[(i + 2) & 3].height;
            NUVEC ab, ac;
            NuVecSub(&ab, &b, &a);
            NuVecSub(&ac, &c, &a);
            NuVecCross(&ab, &ab, &ac);
            NuVecAdd(&normal, &normal, &ab);
        }
        NuVecScale(&normal, &normal, 0.25f);
        if (normal.y > 0.0f)
            NuVecNorm(&object->suspension_normal, &normal);
        game = api.character_data->game_character;
        flags = game->flags_090;
    }
    if ((api.field_0x1f8 & 4) == 0) {
        if ((flags & 0x200000) == 0) {
            if (object->surface_normal.x != 0.0f || object->surface_normal.y != 1.0f ||
                object->surface_normal.z != 0.0f) {
                FindAnglesZX(&object->surface_normal, &object->field_0x1062, &object->field_0x1064);
            } else {
                object->field_0x1062 = object->field_0x1064 = 0;
            }
        } else {
            FindAnglesZX(&object->suspension_normal, &object->field_0x1062, &object->field_0x1064);
        }
        game = api.character_data->game_character;
        flags = game->flags_090;
    }
    u16 x = object->field_0x1062, z = object->field_0x1064;
    temp_xrot = x;
    temp_zrot = z;
    const bool sliding = (flags & 0x200) != 0 && object->character_context == 0x33;
    const bool magnet = !sliding && (flags & 0x10000000) != 0 && object->character_context == 0x44;
    const bool grapple = !sliding && object->character_context == 0x46 && object->field_0x7a3 == 0;
    i32 alignment = 0;
    if (object->field_0x1086 == 2 &&
        ((api.character_data->model_flags & 0x100) != 0 || (flags & 0x200000) != 0 || sliding)) {
        const f32 hover =
            (api.character_data->model_flags & 0x2000) != 0 ? GetVehicleHoverHeight(object, NULL) : game->field_0x28;
        if (hover > 0.0f) {
            alignment = 2;
            if (api.field_0x218 == 2000000.0f) {
                x = z = 0;
            } else {
                const f32 height = api.collision_min.y - api.field_0x218;
                f32 fraction;
                if (hover > height)
                    fraction = 1.0f;
                else if (2.5f * hover > height)
                    fraction = 1.0f - (height - hover) / (2.5f * hover - hover);
                else
                    fraction = 0.0f;
                x = static_cast<i32>(RotDiff(0, static_cast<u16>(temp_xrot)) * fraction);
                z = static_cast<i32>(RotDiff(0, static_cast<u16>(temp_zrot)) * fraction);
            }
        } else {
            alignment = 1;
            if (api.field_0x27d == 0 && (api.character_data->game_character->flags_090 & 0x200000) == 0 && !sliding) {
                alignment = 0;
                if ((api.character_data->model_flags & 0x100) != 0 &&
                    api.character_data->game_character->field_0x28 == 0.0f &&
                    (api.character_data->game_character->flags_090 & 0x100) != 0 && api.field_0x218 != 2000000.0f) {
                    alignment = api.field_0x1e0 > api.collision_min.y - api.field_0x218;
                }
            }
        }
        if (object->id == id_ATST && VehicleArea == 0) {
            x = static_cast<i32>(RotDiff(0, static_cast<u16>(temp_xrot)) * 0.5f);
            z = static_cast<i32>(RotDiff(0, static_cast<u16>(temp_zrot)) * 0.5f);
        }
    }
    const bool reflection = (api.character_data->model_flags & 0x2000) != 0 &&
                            api.character_data->game_character->field_0x28 > 0.0f &&
                            (api.field_0x27f == 0x10 || api.field_0x27f == 7) && api.field_0x27c == -1;
    if (mode == 1) {
        object->field_0x105e = x;
        object->field_0x1060 = z;
        if (alignment == 0) {
            api.pitch_angle = api.roll_angle = 0;
        } else if (magnet) {
            if (static_cast<u16>(object->context_animation - 5) <= 1)
                api.pitch_angle = api.roll_angle = 0;
            else {
                api.pitch_angle = object->magnet_surface_angle;
                api.roll_angle = object->grapple_swing_phase;
            }
        } else if (grapple) {
            NUVEC direction;
            NuVecSub(&direction, &static_cast<GRAPPLE_s *>(object->field_0x788)->hook_position,
                     &api.collision_position);
            FindAnglesZX(&direction, &api.pitch_angle, &api.roll_angle);
        } else if (reflection) {
            api.pitch_angle = object->field_0x1068;
            api.roll_angle = object->field_0x106a;
        } else if (alignment == 2 || object->surface_normal.y > NuTrigTable[0x3000]) {
            api.pitch_angle = x;
            api.roll_angle = z;
        } else {
            api.pitch_angle = api.roll_angle = 0;
        }
    } else if (mode == 2) {
        if (object->field_0x105e != object->field_0x1062)
            object->field_0x105e = SeekRot(object->field_0x105e, x, 8.0f);
        if (object->field_0x1060 != object->field_0x1064)
            object->field_0x1060 = SeekRot(object->field_0x1060, z, 8.0f);
        if (object->field_0x1086 != 2)
            return;
        if (!magnet) {
            if (grapple) {
                NUVEC direction;
                NuVecSub(&direction, &static_cast<GRAPPLE_s *>(object->field_0x788)->hook_position,
                         &api.collision_position);
                FindAnglesZX(&direction, &x, &z);
            } else if (reflection) {
                z = object->field_0x106a;
                x = object->field_0x1068;
            } else if (alignment != 2 && (alignment == 0 || !(object->surface_normal.y > NuTrigTable[0x3000]))) {
                x = z = 0;
            }
        } else {
            if (static_cast<u16>(object->context_animation - 5) <= 1) {
                api.pitch_angle = api.roll_angle = x = z = 0;
            } else {
                z = object->grapple_swing_phase;
                x = object->magnet_surface_angle;
            }
        }
        const f32 speed = object->id == id_ATST && VehicleArea == 0 ? 3.0f : 8.0f;
        if (api.pitch_angle != x)
            api.pitch_angle = SeekRot(api.pitch_angle, x, speed);
        if (api.roll_angle != z)
            api.roll_angle = SeekRot(api.roll_angle, z, speed);
    }
}

void PlayerTakeHit(GameObject_s *object, GameObject_s *attacker) {
    if (object->apiobj.field_0x27c != -1)
        return;

    const u8 state = object->field_0xe31;
    i32 animation;
    void *animation_data;
    if (state != 1) {
        animation = 0x3d;
        animation_data = object->apiobj.character_model->model_data_b[0x3d];
    } else {
        animation = 0x3e;
        animation_data = object->apiobj.character_model->model_data_b[0x3e];
    }
    if (animation_data == NULL) {
        if ((object->field_0xefb & 8) != 0) {
            ResetAnimPacket(&object->apiobj.anim_packet, -1);
            Player_ClearContext(object, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
        }
        return;
    }

    if (object->field_0x7a5 == 0x1b && object->field_0x780 != NULL) {
        GameObject_s *partner = static_cast<GameObject_s *>(object->field_0x780);
        if (partner->field_0x7a5 == 0x1c)
            partner->field_0x7a5 = 0xff;
    }

    object->context_animation = animation;
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    Player_ClearContext(object, 1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->field_0x7a5 = 0x15;

    f32 duration = AnimDuration(object->id, object->context_animation, 0.0f, 0.0f, 1);
    object->field_0xe31 = state;
    object->context_animation_timer = duration;
    if (duration <= 0.0f)
        object->context_animation_timer = 1.0f;
    SetFlicker(object, 0.4f);

    if (attacker != NULL) {
        const u16 angle = NuAtan2D(attacker->apiobj.collision_position.x - object->apiobj.collision_position.x,
                                   attacker->apiobj.collision_position.z - object->apiobj.collision_position.z);
        object->apiobj.field_0x276 = angle;
        object->apiobj.movement_facing_angle = angle;
        object->apiobj.facing_angle = angle;
    }
}

i32 StunGameObject(GameObject_s *object, GameObject_s *attacker, float duration, i32 flags) {
    if (object->apiobj.field_0x27c != -1)
        return 0;

    void **animations = object->apiobj.character_model->model_data_b;
    i32 candidates[5];
    i32 candidate_count = 0;
    if (animations[0xa7] != NULL && animations[0xaa] != NULL)
        candidates[candidate_count++] = 0xa7;
    if (animations[0xa8] != NULL && animations[0xab] != NULL)
        candidates[candidate_count++] = 0xa8;
    if (animations[0xa9] != NULL && animations[0xac] != NULL)
        candidates[candidate_count++] = 0xa9;

    if ((flags & 0x200) == 0) {
        if (animations[0x3d] != NULL)
            candidates[candidate_count++] = 0x3d;
        if (attacker != NULL && attacker->apiobj.player_controlled && animations[0xb9] != NULL)
            candidates[candidate_count++] = 0xb9;
    }

    i32 animation = 1;
    if (candidate_count != 0)
        animation = candidates[qrand() / (0xffff / candidate_count + 1)];
    if (animation == -1)
        return 0;

    object->context_animation = animation;
    object->external_force.z = AnimSpeed(object->apiobj.character_model, animation);
    if (animation != 0x3d && animation != 0xb9) {
        if (object->external_force.z <= 0.0f)
            object->external_force.z = 1.0f;
        else if (object->external_force.z <= 0.8f)
            object->external_force.z = 0.8f;
    }

    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    Player_ClearContext(object, 1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->character_context = 0x5a;
    object->field_0x768 = duration;
    object->field_0x7a3 = 0;

    if (attacker != NULL) {
        const u16 angle = NuAtan2D(attacker->apiobj.collision_position.x - object->apiobj.collision_position.x,
                                   attacker->apiobj.collision_position.z - object->apiobj.collision_position.z);
        object->apiobj.field_0x276 = angle;
        object->apiobj.movement_facing_angle = angle;
        object->apiobj.facing_angle = angle;
    }

    animation = object->context_animation;
    animations = object->apiobj.character_model->model_data_b;
    CHARACTERANIM_s *animation_data =
        static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation]);
    if (animations[animation] != NULL && (animation_data->flags & 2) != 0) {
        object->context_animation_timer = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 0.5f + 1.0f;
    } else {
        object->context_animation_timer = AnimDuration(object->id, animation, 0.0f, 0.0f, animation != 0xb9);
        if (object->context_animation_timer <= 0.0f)
            object->context_animation_timer = 0.6f;
    }

    if (attacker != NULL) {
        object->apiobj.movement_facing_angle =
            NuAtan2D(-(object->apiobj.collision_position.x - attacker->apiobj.collision_position.x),
                     -(object->apiobj.collision_position.z - attacker->apiobj.collision_position.z));
    }

    object->field_0xe31 = object->field_0xe31;
    return (object->context_animation == 0xb9) + 1;
}

i32 DeactivatePlayer(GameObject_s *object, f32 duration, GameObject_s *source) {
    if (object->character_context == 0x17 && !(duration > object->context_animation_timer))
        return 0;
    f32 model_state = object->field_0xd24;
    Player_ClearContext(object, 1);
    if (object->character_context == 0x3e)
        return 0;
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    object->character_context = 0x17;
    object->field_0xd24 = model_state;
    AISCRIPTPROCESS *process = reinterpret_cast<AISCRIPTPROCESS *>(&object->ai);
    if (AIScriptSetBaseScriptStateByName(process, "BeenDeactivated")) {
        AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, process, FRAMETIME);
    }
    if (source != NULL && (source->apiobj.character_data->model_flags & 0x40) != 0 &&
        static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field275_0x116 == 1 &&
        object->apiobj.character_model->model_data_b[0x62] != NULL) {
        duration = 4.0f;
        object->context_animation = 0x62;
    } else if (object->apiobj.character_model->model_data_b[0x81] != NULL &&
               (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[0x81])->flags & 2) == 0) {
        object->context_animation = 0x81;
    } else {
        object->context_animation = object->apiobj.character_model->model_data_b[0x41] != NULL ? 0x41 : 1;
    }
    if ((object->apiobj.character_data->model_flags & 0x20) != 0)
        SetProtocolDroidDeactivatedAction(object);
    object->action_movement_state = 0;
    object->context_animation_timer = duration;
    object->airborne_action_duration = 0.0f;
    object->field_0x768 = 0.0f;
    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
    return 1;
}

void ActivatePlayer(GameObject_s *object) {
    object->character_context = -1;
    if (object->apiobj.character_model->model_data_b[0x80] != NULL) {
        object->character_context = 0x41;
        object->context_animation = 0x80;
        f32 duration = AnimDuration(object->id, 0x80, 0.0f, 0.0f, 1);
        object->airborne_action_duration = 0.0f;
        object->context_animation_timer = duration;
    }
}

void FindPlayerAndSetWeapon(i32 id, i32 weapon_out) {
    GameObject_s *object = Player_FindByID(id);
    if (object == NULL)
        return;

    object->field_0xe22 = (object->field_0xe22 & ~GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION) |
                          (weapon_out & GAMEOBJECT_E22_FLAG_WEAPON_ANIMATION);
    if (weapon_out != 0) {
        object->weapon_scale = 1.0f;
        if (!object->apiobj.player_controlled)
            object->field_0xed8 = DEFENDTIME;
    } else {
        object->weapon_scale = 0.0f;
    }
}

void GameObjectToCameraCode(GameObject_s *object) {
    if (GameTimer.update_count == 0) {
        object->shadow_opacity = 0.0f;
        return;
    }
    f32 distance = object->ai_update_distance;
    LEVELDATA *level = WORLD->current_level;
    i32 far_distance = static_cast<u8>(level->blob_shadow_fade_far);
    if (distance < static_cast<f32>(far_distance)) {
        u32 near_distance = static_cast<u8>(level->blob_shadow_fade_near);
        if (distance > static_cast<f32>(near_distance))
            object->shadow_opacity = 1.0f - (distance - static_cast<f32>(near_distance)) /
                                                static_cast<f32>(static_cast<i32>(far_distance - near_distance));
        else
            object->shadow_opacity = 1.0f;
    } else {
        object->shadow_opacity = 0.0f;
    }
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if ((data->flags_094[0] & 0x10) != 0)
        return;
    u32 mask;
    if (object->apiobj.field_0x287 != 0) {
        GAMECHARACTERDATA *model_data = static_cast<GAMECHARACTERDATA *>(
            apicharsys->char_data[object->apiobj.character_model->model_id].field11_0x24);
        mask = model_data->layer_mask_dead;
        if (mask != 0)
            goto apply_layers;
    }
    if (object->id == id_ANAKINJEDISCARRED)
        mask = data->layer_mask_special;
    else if (g_lowEndLevelBehaviour == 0 || object->apiobj.player_controlled) {
        if (distance > data->field_0xb8)
            mask = data->layer_mask_low;
        else if (!(distance > data->field_0xb4))
            mask = data->layer_mask;
        else
            mask = data->layer_mask_medium;
    } else {
        if (data->field_0xb4 > distance)
            mask = data->layer_mask_medium;
        else
            mask = data->layer_mask_low;
    }
apply_layers:
    object->field_0x1054 = mask | object->extra_layer_mask;
    object->field_0x1054 = AdjustLayerBits(object->field_0x1054, object);
}

void UpdateGameObjects(WORLDINFO_s *world) {
    NUVEC position;
    NUVEC offset;
    // The three message paths reuse the same record in the original. Its
    // text scratch starts at esp+0xd0 in the 0x150-byte reference frame.
    char text[128];
    ADDGAMEMSG message;
    Hub_PadSpeed[1] = 0.0f;
    Hub_PadSpeed[0] = 0.0f;
    SetPlayer();
    AIFireIntervalMul = 1.0f;
    CarWashHack = 0;
    const i32 difficulty = static_cast<i8>(adtab[adaptivedifficulty[0]][3]);
    if (difficulty == 1)
        AIFireIntervalMul = 0.5f;
    else if (difficulty == -1)
        AIFireIntervalMul = 2.0f;
    GhostLightMul = SeekLinearF(GhostLightMul, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f + 0.75f,
                                FRAMETIME + FRAMETIME);
    if (GhostLightMul == GhostLightTargetMul) {
        GhostLightTargetMul = (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f + 0.75f;
    }
    if (TimingBarSet == 2)
        TBOPENFN("AI", 2);

    // AI updates are scheduled before the object/player movement passes. The
    // elapsed value is accumulated until an object becomes eligible for its
    // next script and path update.
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags) {
            continue;
        }

        MechAddonCollection *addons = object->GetAddons(false);
        if (addons != NULL)
            addons->Process(MechAddon::PROCESS_STAGE_0, FRAMETIME);
        if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            object->ai_elapsed_time = 0.0f;
        }
        object->ai_elapsed_time += FRAMETIME;

        const bool force_update =
            timebase_updates == 0 || (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0 ||
            (object->field_0xefb & 8) != 0 || (object->field_0xf00 & GAME_OBJECT_AI_UPDATE_SPECIAL_STATE) != 0 ||
            (CInfo[object->character_context].flags & 0x200) != 0 || object->apiobj.supporting_platform_id != -1 ||
            (object->field_0xcc0 != NULL && object->character_context == CHARACTER_CONTEXT_LINKED_OBJECT) ||
            Technos_FindControllingTechno(object) != NULL ||
            (object->active_trigger_set != NULL && (object->active_trigger_set->flags & 2) != 0) ||
            (object->ai.group != NULL && object->ai.group->is_in_formation);
        const i32 interval = force_update ? 1 : GameObjectAIUpdateInterval(world, object);

        if (interval <= 1) {
            object->field_0xf00 |= GAME_OBJECT_AI_UPDATE_FORCED;
        } else {
            object->field_0xf00 &= ~GAME_OBJECT_AI_UPDATE_FORCED;
            const u32 update_phase =
                static_cast<u32>(object->apiobj.field_0x289) + static_cast<u32>(GameTimer.update_count);
            if (update_phase % static_cast<u32>(interval) != 0) {
                object->field_0xf00 &= ~GAME_OBJECT_AI_UPDATE_PROCESS;
                continue;
            }
        }
        object->field_0xf00 |= GAME_OBJECT_AI_UPDATE_PROCESS;

        // AI positions between scheduled updates are render extrapolations.
        // The target restores the last terrain-resolved position here before
        // GameAIProcess and the movement/terrain passes (0x140c2..0x140f0),
        // so an extrapolated floor offset never becomes the next collision
        // query's starting point.
        const u32 authoritative_position_flags =
            APIOBJECT_MOTION_FLAG_AI_CONTROLLED | APIOBJECT_STATE_FLAG_IGNORE_DOORS;
        if ((object->apiobj.field_0x1f4 & authoritative_position_flags) == APIOBJECT_MOTION_FLAG_AI_CONTROLLED) {
            object->apiobj.position = object->ai_update_position;
        }
    }

    GameAntinode_Update(world->game_antinode_sys);
    if (TimingBarSet == 4)
        TBOPENFN("AIProc", 4);
    GameAIProcess();
    if (TimingBarSet == 4)
        TBCLOSEFN("AIProc", 4);

    // AI movement precedes the separate player movement pass.
    object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
            (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0)
            continue;
        object->apiobj.flags_high = (object->apiobj.flags_high & ~0x40) | ((object->field_0x7a5 == 0) << 6);
        object->field_0x1024 -= FRAMETIME;
        if (object->field_0x1024 < -1.0f)
            SetFlicker(object, 0.0f);
        if (object->timer_d28 > 0.0f)
            object->timer_d28 -= FRAMETIME;
        if (object->timer_d50 > 0.0f)
            object->timer_d50 -= FRAMETIME;
        if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
            MovePlayer_NETWORK(object);
        } else if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            f32 frame_time = FRAMETIME;
            FRAMETIME = object->ai_elapsed_time;
            object->field_0xe20 &= ~0x20;
            if (object->spawn_protection_timer > 0.0f)
                object->spawn_protection_timer -= FRAMETIME;
            object->field_0xdec = 0.0f;
            if (TimingBarSet == 4)
                TBOPENFN("Move", 4);
            if (static_cast<i32>(object->apiobj.field_0x1f4) < 0) {
                object->apiobj.field_0x276 = object->field_0x106e;
                object->apiobj.movement_facing_angle = object->field_0x106e;
                object->apiobj.position.x = object->field_0xee8;
                object->apiobj.facing_angle = object->field_0x106e;
                object->apiobj.position.z = object->field_0xeec;
            }
            if (object->move_override != NULL)
                object->move_override(object);
            else if (object->movement_spline != NULL)
                MovePlayerSpline(object);
            else
                MovePlayer(object);
            if (TimingBarSet == 4)
                TBCLOSEFN("Move", 4);
            if ((object->field_0xefa & 0x40) != 0 && (object->apiobj.flags_low & 4) == 0) {
                object->room_id = -1;
                if ((object->field_0xefa & 0x80) != 0 && world->current_gscn != NULL)
                    object->room_id = NuPortalWhichRoom(world->current_gscn, &object->apiobj.collision_position);
            }
            FRAMETIME = frame_time;
        }
        GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        if (data->field275_0x116 == 10 &&
            ((object->apiobj.model_draw_result != 0) != ((object->field_0xf01 & 1) != 0))) {
            f32 *wait = object->apiobj.model_draw_result != 0 ? &tieonsfxwait : &tieoffsfxwait;
            if (*wait <= 0.0f) {
                PlaySfx("veh_tie_by", &object->apiobj.collision_position);
                *wait = (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f + 0.5f;
            }
        }
    }

    if (TimingBarSet == 2)
        TBCLOSEFN("AI", 2);

    // Player movement has its own timers, power-up and controller processing.
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        const u16 player_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
        if (object == NULL || (object->apiobj.field_0x1f8 & player_flags) != player_flags) {
            continue;
        }
        if (VehicleArea != 0 && static_cast<u8>(object->apiobj.field_0x27c) <= 1 && !object->apiobj.player_controlled)
            object->field_0xe20 |= 0x20;
        else
            object->field_0xe20 &= ~0x20;
        if (object->timer_d5c > 0.0f)
            object->timer_d5c -= FRAMETIME;
        if (object->tag_cooldown > 0.0f)
            object->tag_cooldown -= FRAMETIME;
        if (object->spawn_protection_timer > 0.0f)
            object->spawn_protection_timer -= FRAMETIME;
        PowerUp_Update(object);
        if (object->apiobj.player_controlled && (Cheats_CheckFlags(0x1000) != 0 || object->field_0xdec > 0.0f))
            RegenerateHearts(object);
        object->field_0xf03 &= ~0x10;
        f32 flicker_decay = 1.0f;
        if (object->field_0x1024 > 0.0f && object->apiobj.player_controlled) {
            if (adtab[adaptivedifficulty[0]][2] == 1)
                flicker_decay = 2.0f;
            else if (adtab[adaptivedifficulty[0]][2] == -1)
                flicker_decay = 0.5f;
        }
        object->field_0x1024 -= flicker_decay * FRAMETIME;
        if (object->field_0x1024 < -1.0f)
            SetFlicker(object, 0.0f);
        if (object->timer_d28 > 0.0f)
            object->timer_d28 -= FRAMETIME;
        if (object->timer_d50 > 0.0f)
            object->timer_d50 -= FRAMETIME;
        object->apiobj.flags_high = (object->apiobj.flags_high & ~0x40) | ((object->field_0x7a5 == 0) << 6);
        if ((object->apiobj.field_0x1f4 & 0x40000) != 0) {
            MovePlayer_NETWORK(object);
        } else {
            if (!object->apiobj.player_controlled) {
                if (FreePlay != 0) {
                    Player_ToggleCharacter(object, 0, 0);
                }
            } else {
                if (object->pad_gamepad->pad != NULL) {
                    UpdateRumble(&object->pad_gamepad->rumble_packet);
                    Player_ToggleCharacter(object, 0, 1);
                }
            }
            if (TimingBarSet == 3)
                TBOPENFN("Move", 3);
            if (object->move_override != NULL)
                object->move_override(object);
            else if (object->movement_spline != NULL)
                MovePlayerSpline(object);
            else
                MovePlayer(object);
            if (TimingBarSet == 3)
                TBCLOSEFN("Move", 3);
        }
        if ((object->field_0xefa & 0x40) != 0 && (object->apiobj.flags_low & 4) == 0) {
            object->room_id = -1;
            if ((object->field_0xefa & 0x80) != 0 && world->current_gscn != NULL)
                object->room_id = NuPortalWhichRoom(world->current_gscn, &object->apiobj.collision_position);
        }
    }
    CurrentSpeedOverride = -1000.0f;
    AveragePlayerCurrentSpeedMul();

    // AI-controlled characters use their accumulated AI interval for a full
    // terrain update when their script was processed. Between those updates,
    // the original advances the last resolved velocity and still animates the
    // object every frame.
    object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags ||
            (object->apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            object->field_0xef8 &= ~1;
            if (object->timer_ed8 > 0.0f)
                object->timer_ed8 -= FRAMETIME;
            continue;
        }
        if (TimingBarSet == 2)
            TBOPENFN("TA", 2);
        if ((object->field_0xf00 & GAME_OBJECT_AI_UPDATE_PROCESS) != 0) {
            const f32 frame_time = FRAMETIME;
            FRAMETIME = object->ai_elapsed_time;
            TerrainPlayer(object);

            object->ai_update_position = object->apiobj.position;

            const f32 vertical_displacement = object->apiobj.position.y - object->apiobj.start_position.y;
            if (!(vertical_displacement != 0.0f && object->ai_elapsed_time != 0.0f)) {
                object->vertical_velocity = 0.0f;
            } else {
                object->vertical_velocity = vertical_displacement / object->ai_elapsed_time;
            }
            FRAMETIME = frame_time;
        } else {
            PreResetCode(object);
            PostResetCode(object);
            if ((object->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) != 0) {
                GameObjectOrigin(object);
            } else {
                object->apiobj.position.x += object->apiobj.velocity.x * FRAMETIME;
                object->apiobj.position.y += object->vertical_velocity * FRAMETIME;
                object->apiobj.position.z += object->apiobj.velocity.z * FRAMETIME;
            }
        }

        AnimatePlayer(object);
        object->context_target_position = NULL;
        if (TimingBarSet == 2)
            TBCLOSEFN("TA", 2);
        if (world->sock_sys != NULL && (object->field_0xef9 & 0x40) != 0) {
            ComplexSockPosition(world->sock_sys, &object->apiobj.position, static_cast<i8>(object->field_0x661),
                                object->sock_segment, &object->sock_position);
            ComplexSockAngles(&object->sock_angles);
        } else {
            object->sock_segment = -1;
            object->field_0x661 = 0xff;
        }
        if (TimingBarSet == 4)
            TBOPENFN("Misc", 4);
        if (WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks != 0 || object->id != id_SPEEDERBIKE ||
            object->apiobj.field_0x287 == 0)
            UpdateLastSafePosition(object);
        f32 scale = 1.0f;
        if (object->apiobj.field_0x287 == 1)
            scale -= object->timer_1014 / object->field_0x1018;
        object->field_0x1004 = scale;
        if (object->field_0x1038 != 1000000000.0f)
            object->field_0x1004 = object->field_0x1038;
        object->apiobj.field_0xa8 = object->field_0x1004 * object->apiobj.character_data->model_scale;
        ScaleGameObject(object);
        GameObjectDimensions(object);
        AddSurfaceRipples(object);
        if (object->use_model_origin != 0xff) {
            ++object->use_model_origin;
        }
        NuCameraTransformScreenClip(&object->camera_screen_position, &object->apiobj.collision_position, 1, NULL);
        GameObjectToCameraCode(object);
        if (TimingBarSet == 4)
            TBCLOSEFN("Misc", 4);
        object->field_0xef8 &= ~1;
        if (object->timer_ed8 > 0.0f)
            object->timer_ed8 -= FRAMETIME;
    }

    CurrentSpeed = 0.0f;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *object = Player[i];
        const u16 player_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_PLAYER_CHARACTER;
        if (object == NULL || (object->apiobj.field_0x1f8 & player_flags) != player_flags) {
            continue;
        }

        if (VehicleArea != 0 || (object->apiobj.character_data->model_flags & 0x2000) != 0) {
            object->reset_velocity = object->apiobj.velocity;
            object->pre_terrain_speed = NuFsqrt(object->reset_velocity.x * object->reset_velocity.x +
                                                object->reset_velocity.y * object->reset_velocity.y +
                                                object->reset_velocity.z * object->reset_velocity.z);
        }
        if (TimingBarSet == 2)
            TBOPENFN("TA", 2);
        TerrainPlayer(object);
        object->ai_update_position = object->apiobj.position;
        const f32 vertical_displacement = object->apiobj.position.y - object->apiobj.start_position.y;
        if (vertical_displacement == 0.0f || object->ai_elapsed_time == 0.0f) {
            object->vertical_velocity = 0.0f;
        } else {
            object->vertical_velocity = vertical_displacement / object->ai_elapsed_time;
        }
        AnimatePlayer(object);
        object->context_target_position = NULL;
        if (TimingBarSet == 2)
            TBCLOSEFN("TA", 2);
        if (VehicleArea != 0 || (object->apiobj.character_data->model_flags & 0x2000) != 0) {
            object->post_terrain_speed = NuFsqrt(object->apiobj.velocity.x * object->apiobj.velocity.x +
                                                 object->apiobj.velocity.y * object->apiobj.velocity.y +
                                                 object->apiobj.velocity.z * object->apiobj.velocity.z);
        }
        if (object->oldpos != NULL)
            *object->oldpos = object->sock_position;
        if (!(world->sock_sys != NULL &&
              ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 || VehicleArea != 0))) {
            object->sock_segment = -1;
            object->field_0x661 = 0xff;
            if (object->oldpos != NULL) {
                object->oldpos->location.segment = -1;
                object->oldpos->location.sock = -1;
            }
        } else {
            ComplexSockPosition(world->sock_sys, &object->apiobj.position, static_cast<i8>(object->field_0x661),
                                object->sock_segment, &object->sock_position);
            ComplexSockAngles(&object->sock_angles);
        }
        if (object == Player[0]) {
            if (object->field_0x661 != 0xff)
                CurrentSpeed = world->sock_sys->sock[static_cast<i8>(object->field_0x661)].current_speed;
            BaseCurrentSpeed = CurrentSpeed;
            if (CurrentSpeedOverride != -1000.0f)
                CurrentSpeed = CurrentSpeedOverride;
        }
        UpdateLastSafePosition(object);
        plr_lastpos = object->field_0x661 == 0xff ? object->apiobj.position : object->apiobj.start_position;
        if (object->field_0x661 != 0xff && object->oldpos != NULL && object->oldpos->location.sock == -1) {
            *object->oldpos = object->sock_position;
        }
        KeepOnScreen(object);
        Doors_Check(world, object);
        f32 scale = 1.0f;
        if (object->apiobj.field_0x287 == 1)
            scale -= object->timer_1014 / object->field_0x1018;
        object->field_0x1004 = scale;
        if (FreePlay != 0 && WORLD->area != NULL && WORLD->area == PODSPRINT_ADATA && object->id != id_ANAKINSNEWPOD &&
            object->id != id_ANAKINSNEWPODGREEN && object->id != id_SEBULBASPOD) {
            object->field_0x1004 = scale * 0.55f;
        }
        if (object->field_0x1038 != 1000000000.0f)
            object->field_0x1004 = object->field_0x1038;
        object->apiobj.field_0xa8 = object->field_0x1004 * object->apiobj.character_data->model_scale;
        ScaleGameObject(object);
        GameObjectDimensions(object);
        if ((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 && FadeSys.fade == 0.0f &&
            InDoubleScoreZone(object) != 0) {
            DoubleScore |= static_cast<i32>(1u << (static_cast<u8>(object->apiobj.field_0x27c) & 31));
        }
        UpdateCoinPacket(object->coinpacket, static_cast<u8>(object->apiobj.flags_low) >> 7,
                         static_cast<i8>(object->apiobj.field_0x27c));
        if (!((object->apiobj.flags_low & APIOBJECT_FLAG_PLAYER_ACTIVE) != 0 && object->apiobj.field_0x287 == 0 &&
              object->field_0x7a5 != 0x2b && !(object->field_0x7a5 == 0x0f && object->field_0x7a3 == 1))) {
            ResetCoinPacket(object->coinpacket);
        } else {
            GizmoPickups_Collide(world, object, 1);
        }
        AddSurfaceRipples(object);
        if ((object->apiobj.character_data->model_flags & 0x2000) != 0 && object->apiobj.field_0x287 == 0 &&
            object->apiobj.model_draw_result != 0 && (object->field_0xe20 & 0x20) == 0) {
            AddSurfaceDebris(object);
        }
        if (object->use_model_origin != 0xff) {
            ++object->use_model_origin;
        }
        NuCameraTransformScreenClip(&object->camera_screen_position, &object->apiobj.collision_position, 1, NULL);
        GameObjectToCameraCode(object);
    }

    i32 lighting_start = 0;
    const i32 lighting_phase = static_cast<i8>(MainFrameCounters.third_frame);
    if (lighting_phase != -1)
        lighting_start = (lighting_phase * HIGHGAMEOBJECT) / 3;
    if (TimingBarSet == 2)
        TBOPENFN("rtl", 2);
    object = Obj;
    GameObject_s *dagobah_luke = FindGameObject(id_LUKESKYWALKERDAGOBAH, 0, 1, 1, 0);
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if (world->rooms_visible_ptr[object->room_id] != 0 &&
            (object->apiobj.field_0x1f8 & character_flags) == character_flags) {

            i32 yoda_proximity = TakeOverYodaSeekDistanceHack(object, dagobah_luke, &offset);
            f32 target = (object->field_0xe23 & 0x80) != 0 || yoda_proximity != 0 ? 1.0f : 0.0f;
            object->interaction_arrow_blend =
                SeekLinearF(object->interaction_arrow_blend, target, FRAMETIME + FRAMETIME);
            if (object->apiobj.model_draw_result == 0)
                continue;
            if (i >= lighting_start && i < HIGHGAMEOBJECT)
                LightGameObject(object, world->rtl_set);
            if (object->interaction_arrow_blend > 0.0f) {
                GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                i32 locator = data->ride_locator;

                if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
                    position.x = object->joint_matrices[locator].m30;
                    position.y = object->joint_matrices[locator].m31;
                    position.z = object->joint_matrices[locator].m32;
                } else {
                    position.x = object->apiobj.field_0x190;
                    position.y = object->apiobj.field_0x194;
                    position.z = object->apiobj.field_0x198;
                }
                f32 height = object->apiobj.field_0x1e0 * 0.5f;
                position.y += 0.21f > height ? 0.21f : height;
                i32 phase = static_cast<u16>(object->apiobj.field_0x289 << 13);
                f32 amplitude = 0.05f * object->apiobj.field_0x1dc;
                i32 angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.5f) / 1.5f * 65536.0f);
                position.x += amplitude * NuTrigTable[((angle + phase) >> 1) & 0x7fff];
                angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.174f) / 1.174f * 65536.0f);
                position.y += (0.75f * amplitude) * NuTrigTable[((angle + phase) >> 1) & 0x7fff];
                angle = static_cast<u16>(NuFmod(GameTimer.time_elapsed, 1.259f) / 1.259f * 65536.0f);
                position.z += amplitude * NuTrigTable[((angle + phase) >> 1) & 0x7fff];
                NuStrCpy(text, ASCII_DOWN);
                message = AddGameMsg_Default;
                message.position = &position;
                message.text = text;
                message.scale = 1.0f;
                f32 pulse = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);
                angle = static_cast<u16>((pulse + pulse) * 65536.0f);
                message.alpha = static_cast<u8>(
                    static_cast<i32>((24.0f * NuTrigTable[angle >> 1] + 104.0f) * object->interaction_arrow_blend));
                message.flags = 0x1083;
                message.field_0x4f = 4;
                AddGameMsg(&message);
            }
        }
    }

    if (TimingBarSet == 2)
        TBOPENFN("rtl", 2);
    if (TimingBarSet == 2)
        TBOPENFN("Coll", 2);
    CollideGameObjects(world);
    if (TimingBarSet == 2)
        TBCLOSEFN("Coll", 2);
    if (do_player_tag == 0) {
        if (player_tag_timer > 0.0f) {
            player_tag_timer -= FRAMETIME;
            if (player_tag_timer < 0.0f || player_tag_to == NULL || player_tag_from == NULL ||
                (player_tag_to->apiobj.field_0x1f8 & 0x1001) != 0x1001 || player_tag_to->apiobj.field_0x287 != 0 ||
                (player_tag_to->tag_flags & 2) != 0 || (player_tag_from->apiobj.field_0x1f8 & 0x1001) != 0x1001 ||
                player_tag_from->apiobj.field_0x287 != 0 || (player_tag_from->tag_flags & 2) != 0) {
                player_tag_to = NULL;
                player_tag_timer = 0.0f;
                player_tag_from = NULL;
                do_player_tag = 0;
            }
        }
    } else {
        i32 result = 0;
        if (player_tag_to != NULL && player_tag_from != NULL && player_tag_to != player_tag_from &&
            (player_tag_to->apiobj.field_0x1f8 & 0x1001) == 0x1001 && player_tag_to->apiobj.field_0x287 == 0 &&
            (player_tag_to->tag_flags & 2) == 0 && (player_tag_from->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            player_tag_from->apiobj.field_0x287 == 0 && (player_tag_from->tag_flags & 2) == 0) {
            result = TagCode(player_tag_to, player_tag_from, 0, 0, 1);
            if (result == 1) {
                GameAudio_PlaySfx(0x22, &player_tag_to->apiobj.collision_position, 0, 0);
                GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
                RememberPlayerIDs(0, Player[0] != NULL ? Player[0]->id : -1, Player[1] != NULL ? Player[1]->id : -1);
                player_tag_to->hud_icon_timer = 2.0f;
                player_tag_from->hud_icon_timer = 2.0f;
            }
        }
        if (result != 2) {
            player_tag_to = NULL;
            player_tag_timer = 0.0f;
            player_tag_from = NULL;
            do_player_tag = 0;
        }
    }
    i32 player_indicator[2] = {0, 0};
    if ((VehicleArea != 0 || (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0)) &&
        Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
        Player[1]->apiobj.player_controlled) {
        player_indicator[0] = 1;
        player_indicator[1] = 1;
    } else {
        // The reference has two fixed player gates, not a dynamic player loop.
#define CHECK_PLAYER_VEHICLE_INDICATOR(i)                                                                              \
    {                                                                                                                  \
        GameObject_s *object = Player[i];                                                                              \
        if (object != NULL && object->apiobj.player_controlled && (object->field_0xe24 & 8) != 0 &&                    \
            object->field_0xcc0 != NULL &&                                                                             \
            (object->id == id_ATST || object->id == id_ATST_LOWRES || object->id == id_ATAT)) {                        \
            player_indicator[i] = 2;                                                                                   \
        }                                                                                                              \
    }
        CHECK_PLAYER_VEHICLE_INDICATOR(0)
        CHECK_PLAYER_VEHICLE_INDICATOR(1)
#undef CHECK_PLAYER_VEHICLE_INDICATOR
    }
    // Keep the two reference message blocks while sharing their source body.
#define ADD_PLAYER_VEHICLE_INDICATOR(i)                                                                                \
    do {                                                                                                               \
        GameObject_s *object = Player[i];                                                                              \
        if (object == NULL || player_indicator[i] == 0 || object->apiobj.field_0x287 != 0 ||                           \
            (object->hud_icon_timer > 0.0f && VehicleArea == 0))                                                       \
            break;                                                                                                     \
        NUVEC position;                                                                                                \
        if (player_indicator[i] == 2) {                                                                                \
            GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);   \
            i32 locator = data->ride_locator;                                                                          \
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {                \
                position.x = object->joint_matrices[locator].m30;                                                      \
                position.y = object->joint_matrices[locator].m31;                                                      \
                position.z = object->joint_matrices[locator].m32;                                                      \
            } else {                                                                                                   \
                position = object->apiobj.upper_position;                                                              \
            }                                                                                                          \
            position.y += 0.5f * object->apiobj.field_0x1e0;                                                           \
        } else if (world->current_level == DOGFIGHTA_LDATA) {                                                          \
            position.x = 0.0f;                                                                                         \
            position.y = object->field_0x1000 * object->apiobj.field_0xa8;                                             \
            position.z = 0.0f;                                                                                         \
            NuVecMtxRotate(&position, &position, &object->vehicle_orientation);                                        \
            NuVecAdd(&position, &position, &object->apiobj.position);                                                  \
        } else {                                                                                                       \
            position = object->apiobj.collision_position;                                                              \
            position.y += 1.5f * object->apiobj.field_0x1e0;                                                           \
        }                                                                                                              \
        message = AddGameMsg_Default;                                                                                  \
        message.text = ASCII_DOWN;                                                                                     \
        message.position = &position;                                                                                  \
        message.scale = 0.666f;                                                                                        \
        message.red = PlayerRGB[i][0];                                                                                 \
        message.green = PlayerRGB[i][1];                                                                               \
        message.blue = PlayerRGB[i][2];                                                                                \
        f32 pulse = NuFmod(GameTimer.time_elapsed_mod_seconds, 0.5f);                                                  \
        u16 angle = static_cast<u16>((pulse + pulse) * 65536.0f);                                                      \
        message.alpha = static_cast<u8>(48.0f * NuTrigTable[angle >> 1] + 80.0f);                                      \
        message.flags = 0x83;                                                                                          \
        message.field_0x4f = 4;                                                                                        \
        AddGameMsg(&message);                                                                                          \
    } while (0)
    ADD_PLAYER_VEHICLE_INDICATOR(0);
    ADD_PLAYER_VEHICLE_INDICATOR(1);
#undef ADD_PLAYER_VEHICLE_INDICATOR
    if (MissionSys != NULL && Mission_Active(MissionSys) != NULL && MissionSys->mission != NULL) {
        CheckMissionEnd(MissionSys);
    }
}

void SetKillPartMom(nuvec_s *momentum) {
    momentum->x = 0.0f;
    momentum->y = 1.0f;
    momentum->z = 0.0f;
    NuVecRotateZ(momentum, momentum, qrand());
    NuVecRotateX(momentum, momentum, qrand());
}

void KillParts_TIEFIGHTER(ADDPART_s *params, i32 part_index, i32 variant, GameObject_s *object, i32 mode, u16 xrot,
                          u16 yrot, nuvec_s *velocity) {
    params->flags = static_cast<u32>(variant) < 1 ? 0x400 : 0x10;
    f32 speed_ratio =
        object->apiobj.horizontal_velocity_magnitude / object->apiobj.character_data->game_character->run_speed;
    if (speed_ratio <= 0.25f) {
        params->flags = 0x90;
        params->stop_fn = PartStop_Flickerer;
        params->draw_fn = PartDraw_Flickerer;
        params->gravity = -8.0f;
        params->field_3c = PartImpact_Brick;
        params->velocity = velocity;
        AddPart(params);
        return;
    }

    if (mode == 0) {
        params->field_44 = TiePart_KillExplode;
        params->flags = 0x111;
        params->field_a4 = FRAMETIME;
        params->velocity = &v000;
        AddPart(params);
        return;
    }

    if (__builtin_expect(mode == 1, 0)) {
        NUVEC spin_velocity = {object->apiobj.velocity.x * 0.75f, object->apiobj.velocity.x * 0.75f,
                               object->apiobj.velocity.z * 0.75f};
        if (part_index == 5) {
            NuVecRotateX(&spin_velocity, &spin_velocity, -static_cast<i32>(xrot));
            NuVecRotateY(&spin_velocity, &spin_velocity, -static_cast<i32>(yrot));
            params->move_fn = TiePart_Move;
        } else {
            NuVecRotateX(&spin_velocity, &spin_velocity, xrot);
            NuVecRotateY(&spin_velocity, &spin_velocity, yrot);
            params->move_fn = TieSpinZPart_Move;
        }
        params->flags = 0x111;
        params->field_a4 = 2.0f;
        params->field_44 = TiePart_Kill;
        params->field_3c = TiePart_Impact;
        params->velocity = &spin_velocity;
        PlaySfx("Tie_Spins", &object->apiobj.collision_position);
        AddPart(params);
        return;
    }

    params->field_20 = 0.5f;
    params->field_44 = TiePart_Kill;
    params->flags = 0x91;
    params->draw_fn = PartDraw_Flickerer;
    params->field_3c = TiePart_Impact;
    params->velocity = velocity;
    AddPart(params);
}

void InitBikeParts() {
    memset(bikeParts, 0, sizeof(BIKEPART_s) * 4);
}

void KillParts_SpeederBike(ADDPART_s *params, i32, i32, GameObject_s *object) {
    NUVEC velocity = {object->apiobj.velocity.x * 1.5f, 0.0f, object->apiobj.velocity.z * 1.5f};
    params->flags = 0x400;
    params->field_44 = SpeederPart_Kill;
    params->field_48 = SpeederPart_Update;
    params->velocity = &velocity;
    params->draw_fn = SpeederPart_Draw;

    PART_s *part = AddPart(params);
    if (part == NULL) {
        return;
    }
    part->field_100 = 1.5f;
    part->field_104 = 1.5f;

    i32 index;
    for (index = 0; index < 4; ++index) {
        if (bikeParts[index].active == 0)
            break;
    }
    if (index == 4) {
        part->speeder_index = -1.0f;
    } else {
        part->speeder_index = static_cast<f32>(index);
        if (object->movement_spline != NULL) {
            f32 distance = NuVecMag(&part->velocity) * FRAMETIME;
            const i32 bike_index = static_cast<i32>(part->speeder_index);
            BIKEPART_s *bike = &bikeParts[bike_index];
            bike->spline_position = object->movement_spline_position;
            bike->active = 1;
            GameObject_s *rider = object->field_0xcc0;
            if (rider != NULL) {
                bike->rider = rider;
                rider->apiobj.velocity = velocity;
                rider->apiobj.velocity.y += 1.5f;
            } else {
                bike->rider = NULL;
            }
            bike->rider_position = object->apiobj.position;

            NUVEC position;
            u16 yaw;
            u16 pitch;
            PointAlongSpline(bikeParts[bike_index].spline_position.spline, bikeParts[bike_index].spline_position.along,
                             &position, &yaw, &pitch, 1);
            part->rotation_y = distance < 0.0f ? yaw : yaw - 0x8000;
            part->rotation_x = -pitch;
            part->field_13c = 0;
            return;
        }
    }
    KillPart(part, 0);
}

void KillParts_ATAT(ADDPART_s *params, i32, i32 variant, GameObject_s *) {
    params->flags = static_cast<u32>(variant) < 1 ? 0x500 : 0x110;
    params->velocity->z = 0.0f;
    params->velocity->y = 0.0f;
    params->velocity->x = 0.0f;
    params->field_48 = AtatPart_Update;
    params->stop_fn = AtatPart_Stop;
    params->draw_fn = PartDraw_Flickerer;
    PART_s *part = AddPart(params);
    if (part != NULL)
        part->field_100 = 10.0f;
}

void KillParts(GameObject_s *object, i32 animation, i32 excluded_layer, i32 mode, float vertical_scale,
               i32 use_vehicle_velocity, u16 *angle_override) {
    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *game_character = character->game_character;
    if ((character->model_flags & 0x02000000) != 0 ||
        (game_character->flags_090 & GAMECHARACTER_FLAG_GRAB_DISABLED) != 0)
        return;
    if (object->id == id_BOBAFETT && WORLD->current_level == SARLACCPITA_LDATA && FreePlay == 0 &&
        static_cast<u8>(object->apiobj.field_0x27c) == 0xff)
        return;

    GameAudio_PlaySfx(0x4f, &object->apiobj.collision_position, 0, 0);

    extern HUBMINIKITPIECES_s **Char_MiniKit;
    if ((object->apiobj.character_data->model_flags & 0x04000000) != 0 && object->id != id_MINIDROIDEKA) {
        HUBMINIKITPIECES_s *pieces = Char_MiniKit[object->id];
        if (pieces == NULL || pieces->piece_count == 0)
            return;
        for (i32 i = 0; i < pieces->piece_count; ++i) {
            HUBMINIKITPIECE_s *piece = &pieces->pieces[i];
            if (!NuSpecialExistsFn(&piece->special))
                continue;
            NUMTX matrix = pieces->pieces[i].matrix;
            NuMtxMul(&matrix, &matrix, &object->apiobj.field_0xb8);
            NUVEC momentum;
            SetKillPartMom(&momentum);
            NuVecScale(&momentum, &momentum, 3.0f);
            momentum.x += object->apiobj.velocity.x * 0.75f;
            momentum.y = 5.0f + momentum.y + object->apiobj.velocity.y * 0.75f;
            momentum.z += object->apiobj.velocity.z * 0.75f;
            ADDPART_s params = Default_ADDPART;
            params.matrix = &matrix;
            params.field_14 = 0.2f;
            params.velocity = &momentum;
            params.special = &pieces->pieces[i].special;
            params.field_18 = 0.2f;
            params.gravity = -10.0f;
            params.lighting = Cheats_CheckFlags(1) ? reinterpret_cast<PARTLIGHTSOURCE_s *>(ZeroRTL)
                                                   : reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
            params.flags = mode == 0 ? 0x480 : 0x90;
            params.time_step = FRAMETIME;
            params.stop_fn = PartStop_Flickerer;
            params.draw_fn = PartDraw_Flickerer;
            params.field_3c = PartImpact_Brick;
            params.field_c4 = 1;
            PART_s *part = AddPart(&params);
            if (part != NULL)
                part->field_100 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.0f + 7.0f;
        }
        return;
    }

    const u16 random_rotation_a = static_cast<u16>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 25486.0f - 12743.0f);
    const u16 random_rotation_b = static_cast<u16>(static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 16384.0f - 8192.0f);
    i32 random_variant = qrand() / 21846;
    for (i32 retry = 0; retry < 8 && random_variant == lastrandkillmode; ++retry)
        random_variant = qrand() / 21846;
    lastrandkillmode = random_variant;

    u32 layers = (object->apiobj.character_data->game_character->flags_094[2] & 4) != 0
                     ? object->field_0x1054
                     : AdjustLayerBits(object->apiobj.character_data->game_character->layer_mask_dead, object);
    if (animation != -1)
        layers = 1u << animation;
    else if (excluded_layer != -1)
        layers &= ~(1u << excluded_layer);

    // Retail realigns this frame with `and esp, -32` for the joint matrices.
    NUMTX_ALIGNED32 joint_matrices[256];
    i16 render_indices[32];
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    EvalModelAnim(model, &object->apiobj.anim_packet, &object->apiobj.field_0xb8, joint_matrices, NULL, NULL, NULL,
                  layers);
    const i32 render_count = object->apiobj.character_data->game_character->make_layer_list(
        object->apiobj.character_model, render_indices, layers);
    i32 part_index = 0;
    for (i32 render_index = 0; render_index < render_count; ++render_index) {
        const i16 layer = render_indices[render_index];
        if (layer != -1 && (animation == -1 || layer == animation) &&
            (excluded_layer == -1 || layer != excluded_layer)) {
            nuhgobjrender_s *render = &object->apiobj.character_model->hierarchy->render_parts[layer];
            if (render->rigid_specials == NULL)
                continue;
            for (i32 joint = 0; joint < object->apiobj.character_model->hierarchy->joint_count; ++joint, ++part_index) {
                nuhspecial_s *special = static_cast<nuhspecial_s *>(render->rigid_specials[joint]);
                if (special == NULL)
                    continue;
                NUMTX matrix;
                NUVEC momentum;
                ADDPART_s params;
                if (VehicleArea != 0 && (object->apiobj.character_data->model_flags & 0x2000) != 0 &&
                    object->apiobj.character_data->game_character->field_0x28 > 0.0f) {
                    NuMtxMul(&matrix, &joint_matrices[joint], &object->apiobj.field_0xb8);
                    SetKillPartMom(&momentum);
                    NuVecScale(&momentum, &momentum, 4.0f);
                    momentum.x += object->apiobj.velocity.x * 0.75f;
                    momentum.y = 6.0f + momentum.y + object->apiobj.velocity.y * 0.75f;
                    momentum.z += object->apiobj.velocity.z * 0.75f;
                    params = Default_ADDPART;
                    params.field_14 = 0.4f;
                    params.field_18 = 0.4f;
                    params.gravity = -10.0f;
                    params.matrix = &matrix;
                    params.velocity = &momentum;
                } else {
                    SetKillPartMom(&momentum);
                    momentum.y += vertical_scale;
                    if (animation != -1)
                        momentum.y += 1.0f;
                    if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
                        momentum.x += object->apiobj.velocity.x * 0.75f;
                        momentum.y += object->apiobj.velocity.y * 0.75f;
                        momentum.z += object->apiobj.velocity.z * 0.75f;
                    } else if (use_vehicle_velocity != 0) {
                        momentum.x += object->reset_velocity.x * 0.75f;
                        momentum.y += object->reset_velocity.y * 0.75f;
                        momentum.z += object->reset_velocity.z * 0.75f;
                    } else if (angle_override != NULL) {
                        momentum.z += NuTrigTable[((static_cast<u16>(*angle_override + 0x4000)) >> 1) & 0x7fff] * 2.0f;
                        momentum.x += NuTrigTable[(*angle_override >> 1) & 0x7fff] * 2.0f;
                    }
                    NuMtxMul(&matrix, &joint_matrices[joint], &object->apiobj.field_0xb8);
                    params = Default_ADDPART;
                    params.matrix = &matrix;
                    params.velocity = &momentum;
                    params.field_14 = 0.1f;
                    params.field_18 = 0.1f;
                    params.gravity = -5.0f;
                }
                params.special = static_cast<nuhspecial_s *>(render->rigid_specials[joint]);
                params.lighting = Cheats_CheckFlags(1) ? reinterpret_cast<PARTLIGHTSOURCE_s *>(ZeroRTL)
                                                       : reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
                if (object->id == id_TIEFIGHTER) {
                    KillParts_TIEFIGHTER(&params, part_index, mode, object, random_variant, random_rotation_a,
                                         random_rotation_b, &momentum);
                } else if (object->id == id_ATAT) {
                    KillParts_ATAT(&params, part_index, mode, object);
                } else if (object->id == id_SPEEDERBIKE) {
                    KillParts_SpeederBike(&params, part_index, mode, object);
                } else {
                    params.field_c4 = 1;
                    params.flags = mode == 0 ? 0x480 : 0x90;
                    params.time_step = FRAMETIME;
                    params.stop_fn = PartStop_Flickerer;
                    params.draw_fn = PartDraw_Flickerer;
                    params.field_3c = PartImpact_Brick;
                    PART_s *part = AddPart(&params);
                    if (part != NULL)
                        part->field_100 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.0f + 7.0f;
                }
            }
        }
    }

    if ((object->apiobj.character_data->flags & 1) != 0 && VehicleArea != 0 &&
        (object->apiobj.character_data->model_flags & 0x2000) != 0 &&
        object->apiobj.character_data->game_character->field_0x28 > 0.0f) {
        extern nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *, i32);
        nuhspecial_s *special = CharScene_FindHSpecial(WORLD, object->id);
        if (special != NULL) {
            NUMTX matrix = object->apiobj.field_0xb8;
            NUVEC momentum;
            SetKillPartMom(&momentum);
            NuVecScale(&momentum, &momentum, 4.0f);
            momentum.x += object->apiobj.velocity.x * 0.75f;
            momentum.y = 6.0f + momentum.y + object->apiobj.velocity.y * 0.75f;
            momentum.z += object->apiobj.velocity.z * 0.75f;
            ADDPART_s params = Default_ADDPART;
            params.velocity = &momentum;
            params.matrix = &matrix;
            params.special = special;
            params.field_14 = 0.8f;
            params.field_18 = 0.4f;
            params.gravity = -10.0f;
            params.lighting = Cheats_CheckFlags(1) ? reinterpret_cast<PARTLIGHTSOURCE_s *>(ZeroRTL)
                                                   : reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
            params.flags = mode == 0 ? 0x480 : 0x90;
            params.field_c4 = 1;
            params.time_step = FRAMETIME;
            params.stop_fn = PartStop_Flickerer;
            params.draw_fn = PartDraw_Flickerer;
            params.field_3c = PartImpact_Brick;
            PART_s *part = AddPart(&params);
            if (part != NULL)
                part->field_100 = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 2.0f + 3.0f;
        }
    }
    if (object->apiobj.character_data->game_character->uses_weapon_action == 0)
        Customiser_AddPartAccessories(CharacterCustomiser, object, animation, mode, vertical_scale);
}

void InstantKillParts(GameObject_s *object, i32 mode, float scale) {
    i32 variant = (object->id == id_BODYGUARD && object->current_hp <= 1) ? 4 : -1;
    KillParts(object, -1, variant, mode, scale, 0, NULL);
}

void LoseHelmet(GameObject_s *object, i32, i32) {
    if (object->field_0x108e == 0) {
        return;
    }

    const i32 locator = object->apiobj.character_data->player_config->helmet_locator;
    if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
        NUVEC velocity;
        velocity.z = 0.0f;
        velocity.x = 0.0f;
        velocity.y = 1.0f;
        NuVecRotateZ(&velocity, &velocity, qrand());
        NuVecRotateX(&velocity, &velocity, qrand());

        ADDPART_s params = Default_ADDPART;
        params.matrix = &object->joint_matrices[locator];
        params.velocity = &velocity;
        params.field_14 = 0.1f;
        params.field_18 = 0.1f;
        params.gravity = -5.0f;
        params.special = &WORLD->lev_objs[object->field_0x108e + 0xf9].special;
        params.flags = 0x90;
        params.time_step = FRAMETIME;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_Flickerer;
        params.field_3c = PartImpact_Brick;
        params.field_c4 = 1;
        AddPart(&params);
    }
    object->field_0x108e = 0;
}

i32 KillGameObject(GameObject_s *object, i32 requested_reason, i32) {
    NUVEC momentum;
    NUVEC position;
    i32 reason = requested_reason == 5 ? 4 : requested_reason;
    if (object->apiobj.player_controlled) {
        ++AreaGlobals.values.field_0x00;
        ++LevDeaths;
    }
    object->KillTasks();
    NuSound3ClearLoopHold(&object->apiobj.upper_position, 0);
    NuSound3ClearLoopHold(&object->apiobj.collision_position, 0);
    NuSound3ClearLoopHold(&object->apiobj.lower_position, 0);
    Grapple_RemoveDynamic(object);
    Shards_HandleLostObj(WORLD, object);

    i32 debris[4] = {-1, -1, -1, -1};
    if (reason != 4) {
        i32 part_debris = -1;
        if ((object->apiobj.character_data->model_flags & 0x04000000) != 0) {
            debris[0] = 73;
            debris[1] = 74;
            debris[2] = 75;
        } else if (object->apiobj.character_data->move_fn == Move_CANNON) {
            debris[0] = 75;
            debris[1] = 117;
        } else if (object->id == id_TRAININGREMOTE) {
            debris[0] = 38;
            part_debris = 0;
        } else if (object->id == id_CLONEWALKER || object->id == id_ATST_LOWRES) {
            debris[1] = 130;
            debris[2] = 133;
            debris[3] = 132;
        } else if (object->id == id_ATST) {
            debris[0] = 44;
        } else if (object->id == id_ATAT) {
            GameCam_NewShake(GameCam, 1.0f, 1.0f, 1.0f);
            NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
        } else if (object->id == id_XWING || object->id == id_SNOWSPEEDER || object->id == id_MILLENNIUMFALCON ||
                   object->id == id_TIEFIGHTER || object->id == id_TIEINTERCEPTOR || object->id == id_TIEFIGHTERDARTH ||
                   object->id == id_TIEBOMBER || object->id == id_IMPERIALSHUTTLE || object->id == id_SLAVE1) {
            debris[0] = 34;
            if (WORLD->current_level == DEATHSTARRESCUEE_LDATA && object->apiobj.field_0x27c == -1) {
                GameCam_Judder(GameCam, qrand() < 32768 ? -0.75f : 0.75f, 2, NULL);
                NewRumbleAllPlayers(1.0f, 0.1f, 0, 0);
            }
        } else if (object->id == id_SPEEDERBIKESNOW) {
            debris[0] = 34;
        } else if (object->id == id_MOUSEDROID) {
            debris[0] = 69;
            part_debris = 1;
        } else if (object->id == id_DRAGBOMB) {
            GameCam_NewShake(GameCam, 1.0f, 1.0f, 1.0f);
            NewRumbleAllPlayers(1.0f, 0.1f, 0, 0);
            AddExplosion(&object->apiobj.collision_position, 0.6f * AreaPickupScale, 0.5f, NULL, -1, 0x807);
            if (Cheat_IsOn(5)) {
                debris[0] = 125;
                debris[1] = 126;
            } else {
                debris[0] = 82;
                debris[1] = 83;
                debris[2] = 84;
                part_debris = 3;
            }
            PlaySfx("exp_thermalDet", &object->apiobj.collision_position);
        } else if (object->id == id_WOMPRAT) {
            part_debris = 5;
        } else if (object->id == id_PROBEDROID) {
            debris[0] = 130;
            debris[1] = 133;
        }
        if (object->character_context != 43)
            PlayDieSfx(object);
        if (object->apiobj.player_controlled)
            DieRumble(object);
        if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
            momentum.x = object->apiobj.velocity.x * 0.5f;
            momentum.y = object->apiobj.velocity.y * 0.5f;
            momentum.z = object->apiobj.velocity.z * 0.5f;
        } else {
            momentum = v000;
        }
        for (i32 i = 0; i < 4; ++i) {
            if (debris[i] != -1) {
                if ((object->apiobj.character_data->model_flags & 0x2000) != 0)
                    AddGameDebrisMomentum(WORLD->debris_sys, debris[i], &object->apiobj.collision_position, &momentum,
                                          &momentum);
                else
                    AddGameDebris(WORLD->debris_sys, debris[i], &object->apiobj.collision_position);
            }
        }
        if (part_debris != -1)
            AddPartDebris(WORLD->part_debris_sys, part_debris, &object->apiobj.collision_position);
        BlowUpSnakeBody(object);
    } else {
        DestroySnakeBody(object);
    }

    if (object->character_context == 43) {
        object->character_context = -1;
        if (object->apiobj.player_controlled &&
            (WORLD->current_level == HOTHBATTLEA_LDATA || WORLD->current_level == GUNSHIPA_LDATA))
            GameCam_Blend(GameCam, 0.5f, 0.0f, 1);
    }
    bool terminal_kill = false;
    bool kill_rider = false;
    if (reason == 4) {
        terminal_kill = true;
        reason = 2;
        kill_rider = true;
    } else if (WORLD->current_level == SPEEDERCHASEA_LDATA && object->ai.creature_set == 2) {
        GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, "SpeedersKilled", NULL);
        if (message != NULL)
            message->value += 1.0f;
        ++speeder_hitpoints_lost;
    }
    if ((object->apiobj.character_data->model_flags & 0x2000) != 0) {
        object->apiobj.velocity.x *= 0.5f;
        object->apiobj.velocity.y *= 0.5f;
        object->apiobj.velocity.z *= 0.5f;
    } else {
        object->apiobj.velocity.x = 0.0f;
        object->apiobj.velocity.z = 0.0f;
    }
    object->current_hp = 0;
    object->apiobj.field_0x287 = reason == 3 ? 2 : reason;
    if (object->character_context == 7 || object->field_0xe32 == 1)
        object->field_0xe22 |= 1;
    else if (object->character_context == 6 || object->field_0xe32 == 2)
        object->field_0xe22 &= ~1;
    if (object->field_0x108e != 0)
        LoseHelmet(object, 1, 0);
    GameObject_s *rider = object->field_0xcc0;
    Player_ClearContext(object, 1);
    if (kill_rider && rider != NULL)
        KillGameObject(rider, 4, 0);
    ReleaseLever(object);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    if (object->apiobj.player_controlled) {
        while (object->torpedo != NULL && object->torpedo->count != 0) {
            AddVariableShotDebrisEffect(WORLD->debris_sys->entries[71].effect,
                                        &object->torpedo->pickup_positions[object->torpedo->count], 30, 0, 0);
            --object->torpedo->count;
        }
        if (object->apiobj.field_0x287 == 0 || !object->apiobj.player_controlled ||
            (object->apiobj.character_data->model_flags & 0x2000) == 0 ||
            (WORLD->current_level == SPEEDERCHASEA_LDATA && disable_narrow_socks == 0))
            GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
        if ((object->apiobj.field_0x1f4 & 0x40000) == 0) {
            if (object->takeover_source != NULL &&
                (object->takeover_source->field_0xcc0 == NULL || object->takeover_source->field_0xcc0 == object)) {
                GetTakeOverPos(object->takeover_source, &position);
                object->apiobj.start_position = position;
                object->apiobj.position = position;
                object->saved_position = position;
            } else {
                SetToLastSafePos(object);
            }
        }
        GameObjectOrigin(object);
        AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 255, 1);
    }
    object->field_0x1014 = 0;
    if (object->apiobj.field_0x287 == 2) {
        object->movement_lean_angle = 0;
        object->field_0x1018 = 0.0f;
    } else {
        object->apiobj.field_0x287 = 1;
        object->apiobj.start_position = object->apiobj.position;
        object->field_0x1018 = 0.5f;
    }
    if (terminal_kill == 0 && AIScriptSetBaseScriptStateByName(&object->ai.script_process, "BeenKilled"))
        AIScriptProcess(WORLD->ai_sys, &object->apiobj, &object->ai, &object->ai.script_process, FRAMETIME);

    AIGROUP *group = object->ai.group;
    if (group != NULL) {
        group->member_is_alive &= ~static_cast<u32>(1ull << object->ai.group_member_index);
        group->rows[object->ai.group_row].is_alive &=
            ~static_cast<u8>(1ull << (object->ai.group_member_index - object->ai.group_row * group->count_across));
        group = object->ai.group;
        if (group->leader == &object->apiobj) {
            group->leader = NULL;
            if (!group->is_reversed) {
                const u32 member_count = group->member_count;
                for (u32 i = 0; i < member_count; ++i) {
                    if (group->members[i] != NULL) {
                        GameObject_s *candidate = group->members[i]->objptr;
                        if (candidate != object && candidate != NULL &&
                            (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 && candidate->apiobj.field_0x287 == 0) {
                            group->leader = &candidate->apiobj;
                            break;
                        }
                        if (candidate == NULL)
                            group->members[i] = NULL;
                    }
                }
            } else {
                for (i32 i = group->member_count - 1; i != -1; --i) {
                    if (group->members[i] != NULL) {
                        GameObject_s *candidate = group->members[i]->objptr;
                        if (candidate != object && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
                            candidate->apiobj.field_0x287 == 0) {
                            group->leader = &candidate->apiobj;
                            break;
                        }
                    }
                }
            }
        }
    }
    object->ai.field_0x1e5 &= ~0x50;
    object->ai.pending_nearest_opponent = NULL;
    object->ai.nearest_opponent = NULL;
    object->ai.pending_opponent = NULL;
    object->ai.pending_nearest_metric = 1000000000.0f;
    object->ai.pending_opponent_metric = 1000000000.0f;
    object->apiobj.ai_awareness_mask = 0;
    object->ai_seen_mask = 0;
    object->opponent = NULL;
    object->ai.opponent = NULL;
    object->field_0xecc = 0;
    object->field_0xed0 = 0.0f;
    if (object->takeover_target != NULL) {
        object->takeover_target->takeover_target = NULL;
        object->takeover_target = NULL;
    }
    if (object->character_context == 71 && object->field_0x788 != NULL) {
        static_cast<ZIPUP_s *>(object->field_0x788)->runtime_flags &= ~ZIPUP_RUNTIME_FLAG_OCCUPIED;
        object->field_0x788 = NULL;
    }
    Batarangs_CheckLostData(object);
    SpecialMove_ReleaseVictim(object);
    ReleaseEat(object);
    ReleaseBuildIt(object, 0);
    ReleasePush(object);
    GameObject_s *candidate = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++candidate) {
        if ((candidate->apiobj.field_0x1f8 & 0x1001) != 0x1001 || candidate == object)
            continue;
        if (candidate->character_context == 27 && candidate->force_target == object)
            ReleaseForce(candidate, 0);
        if (candidate->force_target == object)
            candidate->force_target = NULL;
        if (candidate->airborne_collision_target == object)
            candidate->airborne_collision_target = NULL;
    }
    ReleaseForce(object, 0);
    if (object->apiobj.field_0x27c == -1) {
        for (i32 i = 0; i < 32; ++i) {
            if (Bolt[i].active != 0 && Bolt[i].owner == object)
                Bolt[i].owner = NULL;
        }
        PART_s *part = Part;
        for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
            if ((part->active & 1) != 0 && part->owner == object)
                part->owner = NULL;
        }
        for (i32 i = 0; i < 8; ++i) {
            if (Explosion[i].field_0x20 > Explosion[i].field_0x1c && Explosion[i].object == object)
                Explosion[i].object = NULL;
        }
    }
    if (DOGFIGHTA_LDATA != NULL && WorldInfo_CurrentlyActive()->current_level == DOGFIGHTA_LDATA && BonusWinner == -1 &&
        (object->apiobj.field_0x1f4 & 0x40000) == 0)
        ResetLevel(WorldInfo_CurrentlyActive(), "EP3_DOGFIGHT_DIE", 1);
    if (terminal_kill != 0 && object->apiobj.field_0x27c == -1) {
        bool remove = requested_reason == 5;
        AICREATURE *creature = NULL;
        bool respawn = false;
        if (!remove) {
            bool persistent = (object->field_0xefa & 0x10) != 0;
            if ((object->apiobj.field_0x1f4 & 0x4400) == 0x4400) {
                creature = &WORLD->ai_sys->creatures[object->ai.field_0x134];
                if (persistent) {
                    respawn = (object->field_0xefa & 0x20) != 0;
                } else {
                    u32 limit = object->ai_respawn_count + 1;
                    if (creature->max_respawn_count != -1 && object->ai.respawn_locator == NULL)
                        limit =
                            creature->min_respawn_count +
                            ((creature->max_respawn_count - creature->min_respawn_count) * (Game.difficulty - 1)) / 9 +
                            1;
                    remove = limit <= object->ai_respawn_count;
                    respawn = !remove;
                }
            } else {
                remove = !persistent;
            }
        }
        if (remove) {
            object->apiobj.character = 0;
            object->ai.reset_mode = 4;
        } else if (respawn && creature != NULL) {
            object->apiobj.character = 0;
            object->ai.reset_mode = 1;
            object->ai_spawn_delay = 1.0f - (static_cast<f32>(static_cast<u32>(Game.difficulty)) - 1.0f) / 9.0f;
            if (creature->min_respawn_time > 0.0f && creature->max_respawn_time > 0.0f)
                object->ai_spawn_delay = creature->max_respawn_time * object->ai_spawn_delay +
                                         creature->min_respawn_time * (1.0f - object->ai_spawn_delay);
            else {
                const f32 random_fraction = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
                object->ai_spawn_delay = (random_fraction + random_fraction) * object->ai_spawn_delay;
            }
        } else {
            object->field_0x101c = 1.0f;
        }
    }
    if (object->torpedo != NULL && object->apiobj.field_0x27c == -1) {
        FreeTorpedoPacket(&object->torpedo);
        object->torpedo = NULL;
    }
    if (VehicleArea != 0 && static_cast<u8>(object->apiobj.field_0x27c) < 2)
        DebFreeInstantly(&FalconDebKey[object->apiobj.field_0x27c]);
    return 1;
}

static __used__ void SpeederPart_Kill(PART_s *part, i32) {
    AddGameDebris(WORLD->debris_sys, 0x6a, &part->position);
    if (part->speeder_index != -1.0f) {
        GameObject_s *rider = bikeParts[static_cast<i32>(part->speeder_index)].rider;
        if (rider != NULL && (rider->apiobj.field_0x1f8 & 0x1001) == 0x1001) {
            rider->field_0xefe &= ~0x40;
            KillParts(bikeParts[static_cast<i32>(part->speeder_index)].rider, -1, -1, 1, 0.0f, 0, NULL);
            KillGameObject(bikeParts[static_cast<i32>(part->speeder_index)].rider, 2, 0);
        }
        memset(&bikeParts[static_cast<i32>(part->speeder_index)], 0, sizeof(BIKEPART_s));
    }
}

i32 KillPlayer(GameObject_s *object, i32 reason, i32 ignore_spawn_protection, nuvec_s *) {
    if (object->apiobj.field_0x287 != 0)
        return 0;

    if (ignore_spawn_protection != 0)
        object->spawn_protection_timer = 0.0f;
    else if (object->spawn_protection_timer > 0.0f)
        return 0;

    if (Arcade != 0) {
        i32 other_player = 1;
        if (object->apiobj.field_0x27c != 0) {
            if (object->apiobj.field_0x27c != 1)
                return KillGameObject(object, reason, 0);
            other_player = 0;
        }

        i32 active = 0;
        if (Player[other_player] != NULL)
            active = (Player[other_player]->apiobj.field_0x1f4 >> 18) & 1;
        Arcade_PlayerKilled(other_player, active);
    }

    return KillGameObject(object, reason, 0);
}

void ObjHitShield(GameObject_s *attacker, GameObject_s *target, i32 damage, BOLT_s *) {
    if (target == NULL) {
        return;
    }
    if (attacker != NULL && (attacker->apiobj.flags_low & 1) == 0) {
        attacker = NULL;
    }
    if (target->field_0xd24 == 1.0f && target->timer_d28 > 0.0f) {
        if (attacker != NULL) {
            if (attacker->apiobj.player_controlled) {
                AlertSurroundingCreatures(attacker, &attacker->apiobj.collision_position);
            } else if (attacker->character_context != 0x39) {
                damage = 0;
            }
        }
    } else if (attacker != NULL && attacker->apiobj.player_controlled) {
        AlertSurroundingCreatures(attacker, &attacker->apiobj.collision_position);
    }

    GameAudio_PlaySfx(0x41, &target->apiobj.collision_position, GameAudio_GetPlrSfxBits(attacker), 0);
    if (target->apiobj.player_controlled) {
        TakeHitRumble(target, 0.6f);
    }

    if (MiniCutCam == 0 || !target->apiobj.player_controlled) {
        if (target->field_0xe37 != 0 && damage > 0) {
            if (target->apiobj.player_controlled && Player_HasInvincibility(target)) {
                damage = 0;
            }
            i32 shield = target->field_0xe37 - damage;
            f32 first_rumble;
            f32 second_rumble;
            if (shield > 0) {
                PlaySfx("DDekaHit", &target->apiobj.collision_position);
                target->field_0xe37 = shield;
                target->timer_d28 = SHIELDFLICKERTIME;
                first_rumble = 0.5f;
                second_rumble = 0.0f;
            } else {
                PlaySfx("DDekaFlicker", &target->apiobj.collision_position);
                target->timer_d28 = 0.5f;
                target->field_0xe37 = 0;
                target->field_0xd24 = 0.0f;
                first_rumble = 0.8f;
                second_rumble = 0.3f;
            }
            if (attacker == NULL) {
                return;
            }
            NewRumble(attacker->pad_gamepad->pad, first_rumble, 0);
            if (second_rumble > 0.0f) {
                NewRumble(attacker->pad_gamepad->pad, second_rumble, 0);
            }
            return;
        }
    }
    if (attacker != NULL) {
        NewBuzz(attacker->pad_gamepad->pad, 0.1f, 0);
    }
}

i32 ObjHitObj(GameObject_s *attacker, GameObject_s *target, i32 damage, u16 flags, i32 probe, i32) {
    if (target == NULL || (target->apiobj.field_0x1f8 & 0x1001) != 0x1001 || target->apiobj.field_0x287 != 0 ||
        target->field_0x101c > 0.0f)
        return 0;
    i32 no_hurt = objhitobj_nohurtsfx;
    i32 no_impact = objhitobj_noimpactsfx;
    i32 no_rumble = objhitobj_noattackerrumble;
    i32 throw_up = objhitobj_throwkillpartsup;
    i16 *parts_angle = objhitobj_killparts_yrot;
    i32 random = qrand();
    objhitobj_nohurtsfx = 0;
    objhitobj_noimpactsfx = 0;
    objhitobj_noattackerrumble = 0;
    objhitobj_throwkillpartsup = 0;
    objhitobj_killparts_yrot = NULL;
    BOLT_s *bolt = objhitobj_bolt;
    objhitobj_bolt = NULL;
    if (Hub_InMenu() && (target->apiobj.player_controlled))
        return 0;
    if (target->apiobj.character_data->game_character->flags_090 & 0x8000)
        return 0;
    if (target->character_context == 95 || target->character_context == 96)
        return 0;
    if (target->character_context == 90 && (!target->field_0x7a3 || (attacker && !attacker->apiobj.player_controlled)))
        return 0;
    if (!(damage == -1 && (flags & 0x200))) {
        if (target->character_context == 0 &&
            (target->action_movement_state == 3 || target->action_movement_state == 4))
            return 0;
        if (target->character_context == 13 || target->character_context == 14)
            return 0;
    }
    if (CInfo[target->character_context].parameter & 4)
        return 0;
    if (attacker && !(attacker->apiobj.flags_low & 1))
        attacker = NULL;
    if (target->apiobj.field_0x27c != -1)
        ObstacleCamHoldUntilPlayersMove = 0;
    i32 hit = damage;
    bool instant = damage == -1;
    if (target->character_context == 76 || target->character_context == 81) {
        hit = 0;
        instant = false;
    }
    if (attacker && (attacker->apiobj.player_controlled))
        AlertSurroundingCreatures(attacker, &target->apiobj.collision_position);
    if (hit > 0 || instant)
        target->field_0xef8 |= 1;
    if (!(flags & 0x100)) {
        damage = 0;
    } else {
        if ((CInfo[target->character_context].flags & 0x4000000) ||
            (target->character_context != -1 && (target->character_context == LEGOCONTEXT_LAND_SLAM ||
                                                 target->character_context == LEGOCONTEXT_LAND_LUNGE))) {
            i32 angle = RotDiff(attacker->apiobj.movement_facing_angle, target->apiobj.movement_facing_angle);
            if (angle < 0)
                angle = -angle;
            if (angle > 0x4000 && !(LEGOCONTEXT_COMBO != -1 && attacker->character_context == LEGOCONTEXT_COMBO &&
                                    attacker->combo_branch == 6)) {
                if (target->apiobj.player_controlled)
                    NewRumble(target->pad_gamepad->pad, 0.75f, 0);
                if (LEGOCONTEXT_HOLD != -1 && target->character_context == LEGOCONTEXT_HOLD) {
                    NewBlockAction(target);
                    if ((target->id == id_IMPERIALGUARD || target->id == id_GAMORREANGUARD) &&
                        (target->character_context == 24 || target->character_context == 12))
                        GameAudio_PlaySfx(74, &target->apiobj.collision_position, 0, 0);
                }
                return 0;
            }
        }
    }
    if (target->field_0xd24 >= 1.0f) {
        if ((flags & 0x100) &&
            (instant || (LEGOCONTEXT_COMBO != -1 && attacker->character_context == LEGOCONTEXT_COMBO &&
                         attacker->combo_branch == 6)))
            damage = target->field_0xe37;
        ObjHitShield(attacker, target, damage, bolt);
        return 0;
    }
    if (flags == 0)
        flags = ObjHitObj_Flags(attacker);
    if ((target->field_0xefb & 8) || WORLD->current_level == VADERC_LDATA) {
        if (static_cast<u32>(hit) >= 2)
            hit = 1;
    } else if (attacker && (flags & 0x100) && attacker->character_context == 5 && attacker->combo_branch == 6 &&
               target->apiobj.field_0x27c == -1) {
        hit = -1;
    }
    if ((target->field_0xefa & 8) && (flags & 0x80)) {
        hit = 0;
    } else {
        if (WORLD->current_level == SPEEDERCHASEA_LDATA) {
            if (!disable_narrow_socks && (flags & 4) && (target->apiobj.player_controlled)) {
                hit = 0;
                goto attributed_hit;
            }
            if (target->id == id_SPEEDERBIKE && !target->apiobj.player_controlled) {
                if (attacker && !(attacker->apiobj.character_data->model_flags & 0x2000))
                    hit = 0;
                else if (target->ai.creature_set != 2)
                    hit = 0;
                goto attributed_hit;
            }
        }
        if (WORLD->area == HOTHBATTLE_ADATA &&
            ((target->id == id_ATAT && !target->apiobj.player_controlled &&
              (!attacker || !attacker->apiobj.player_controlled)) ||
             (attacker && attacker->id == id_ATAT && !target->apiobj.player_controlled &&
              !attacker->apiobj.player_controlled))) {
            if (hit == -1 && target->id == id_ATAT) {
                if (target->character_context != 23)
                    hit = 0;
            } else
                hit = attacker && attacker->id == id_ATAT && target->id == id_DRAGBOMB ? -1 : 0;
            goto attributed_hit;
        }
        if (flags & 1)
            goto attributed_hit;
        if ((flags & 2) && !target->apiobj.player_controlled && WORLD->current_level != HUB_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((flags & 4) && (target->apiobj.field_0x1f4 & 0x10405) == 0x400 && WORLD->current_level != HUB_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((flags & 12) == 8 && (target->apiobj.player_controlled) && WORLD->current_level != HUB_LDATA &&
            WORLD->current_level != VADERC_LDATA) {
            hit = 0;
            goto attributed_hit;
        }
        if ((MiniCutCam && (target->apiobj.player_controlled)) || target->pad_gamepad == ViewCamGetGamePad() ||
            ((target->apiobj.character_data->model_flags & 0x20000000) && target->field_0xcc0 == NULL))
            hit = 0;
    }
attributed_hit:
    if (attacker) {
        target->last_attacker = attacker;
        if (attacker->apiobj.player_controlled) {
            if ((target->apiobj.character_data->game_character->flags_090 & 0x40) && !VehicleArea &&
                (WORLD->current_level != SPEEDERCHASEA_LDATA || disable_narrow_socks))
                hit = -1;
            if (WORLD->current_level == HUB_LDATA &&
                AIScriptSetBaseScriptStateByName(&target->ai.script_process, "TakenHitFromPlayer")) {
                AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
                goto impact;
            }
        }
    }
    if (AIScriptSetBaseScriptStateByName(&target->ai.script_process, "TakenHit"))
        AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
impact:
    if (!no_impact) {
        if (attacker && attacker->id == id_BODYGUARD)
            PlaySfx("Grv_GuardImpact", &target->apiobj.collision_position);
        else if (attacker && attacker->id == id_IMPERIALGUARD)
            PlaySfx("wpn_bib_stab", &target->apiobj.collision_position);
        else if (bolt) {
            if (!(bolt->type_id >= 27 && bolt->type_id <= 29) && WORLD->area &&
                (WORLD->area == PODRACE_ADATA || WORLD->area == PODSPRINT_ADATA)) {
                PlaySfx("Pod_TuskHit", &bolt->position);
                no_hurt = 1;
            }
        } else if (VehicleArea || (WORLD->current_level == SPEEDERCHASEA_LDATA && !disable_narrow_socks)) {
            i32 bits =
                attacker && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1 ? 1 << attacker->apiobj.field_0x27c : 0;
            GameAudio_PlaySfx(40, &target->apiobj.collision_position, bits, 0);
        } else if (flags & 0x40) {
            if (attacker && attacker->character_context == 38 &&
                (AnimMiscFlags(attacker->apiobj.character_model, attacker->context_animation) & 4))
                PlaySfx("WhipHit", &target->apiobj.collision_position);
            else {
                i32 bits = attacker && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1
                               ? 1 << attacker->apiobj.field_0x27c
                               : 0;
                GameAudio_PlaySfx(74, &target->apiobj.collision_position, bits, 0);
            }
        } else if (attacker) {
            if (attacker->apiobj.character_data->model_flags & 8)
                GameAudio_PlaySfx(65, &target->apiobj.collision_position, GameAudio_GetPlrSfxBits(attacker), 0);
            else
                GameAudio_PlaySfx(74, &target->apiobj.collision_position, 0, 0);
        }
    }
    i32 result;
    i32 health;
    i32 coins;
    i32 hearts;
    u16 computed_angle;
    if (target->spawn_protection_timer > 0.0f || (target->field_0xefe & 0x40)) {
        result = 0;
        if (target->field_0xefd & 0x10)
            PlayerTakeHit(target, attacker);
        goto finish;
    }
    if (target->character_context == 21 && (target->field_0xefb & 8)) {
        target->spawn_protection_timer = 2.5f;
        result = 0;
        goto finish;
    }
    if ((target->apiobj.character_data->model_flags & 0x10) && !target->current_hp && (flags & 0x40) &&
        static_cast<f32>(random) * 1.5259021893143654e-05f > 0.75f) {
        DeactivatePlayer(target, 5.0f, NULL);
        result = 0;
        goto finish;
    }
    if (hit != -1 && !(target->flicker_time <= 0.0f) && (!attacker || !attacker->apiobj.player_controlled))
        return 0;
    if (!(hit > 0 && (target->apiobj.player_controlled))) {
        if (target->character_context == 45 && !Player_HasFastBuild(target))
            GizBuildIt_SetToStart(static_cast<GIZBUILDIT_s *>(target->field_0x788), 1, 1);
        Player_ClearContext(target, 0);
        ReleaseForce(target, 1);
    }
    if (target->field_0x108e && !TouchHacks::TouchControlsActive)
        LoseHelmet(target, 0, 0);
    if (hit != -1) {
        if (!target->current_hp || ((target->apiobj.player_controlled) && Player_HasInvincibility(target))) {
            if (target->id == id_ROYALGUARD || target->id == id_WOOKIEE)
                SetFlicker(target, 0.4f);
            PlayerTakeHit(target, attacker);
            result = 0;
            if (target->apiobj.player_controlled)
                TakeHitRumble(target, 0.666f);
            goto hurt;
        }
        health = target->current_hp - hit;
        if (health > 0) {
            if ((target->apiobj.character_data->model_flags & 0x40000000) && target->character_context == 23)
                goto refill;
            goto surviving_hit;
        }
    } else
        health = 0;
    no_impact = players_cannot_exit_speeder && target->id == id_SPEEDERBIKE && target->field_0xcc0 &&
                target->apiobj.field_0x27c != -1;
    if ((target->apiobj.character_data->model_flags & 0x20000000) && !ObjIsTargetSpeeder(target) && !no_impact) {
        coins = 0;
        if ((target->apiobj.player_controlled) && target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
            coins = LoseCoins(target, 1);
        AddPickups(coins, 0, 0, 0, &target->apiobj.collision_position, NULL, 2.0f,
                   attacker ? attacker->apiobj.field_0x27c : -1, 1.0f, 2000000.0f, attacker, 1, 0, false);
        DeactivatePlayer(target, 1000000000.0f, NULL);
        target->current_hp = target->hitpoints;
        SetFlicker(target, 0.4f);
        result = 0;
        goto hurt;
    }
    if (target->apiobj.character_data->model_flags & 0x40000000)
        goto refill;
    if ((target->field_0xefb & 8) && (!FreePlay || WORLD->current_level != CRUISERC_LDATA))
        goto surviving_hit;
    if (probe) {
        result = 2;
        goto finish;
    }
    if (target->character_context == 93) {
        PopBalloon(target);
        target->current_hp = 1;
        result = 0;
        goto hurt;
    }
    target->current_hp = 0;
    coins = 0;
    hearts = 0;
    if (target->apiobj.field_0x27c == -1) {
        if (!target->apiobj.player_controlled) {
            coins = BonusArea ? static_cast<u16>(target->apiobj.character_data->game_character->field_0xee)
                              : (Cheat_IsOn(16) ? 350 : 0);
            hearts = ReleaseHearts();
        }
    } else if (target->apiobj.player_controlled) {
        if (target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
            coins = LoseCoins(target, 1);
        if (!BuildUpDone)
            BuildUpScale = 1.5f;
        DrawBuildUpTime = 1.0f;
        builduptime = 1.0f;
    }
    if (hearts || coins > 0)
        AddPickups(coins, hearts, 0, 0, &target->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, attacker,
                   !BonusArea && coins < 2500, BonusArea ? 1 : 0, false);
    if (target->torpedo && WORLD->area != SPEEDERCHASE_ADATA && (!WORLD->area || !(WORLD->area->flags & 4)))
        DropTorpedoPickups(target->torpedo, target->torpedo->count);
    if (!parts_angle) {
        if (!attacker)
            parts_angle = NULL;
        else if (flags & 0x240) {
            computed_angle = NuAtan2D(target->apiobj.position.x - attacker->apiobj.position.x,
                                      target->apiobj.position.z - attacker->apiobj.position.z);
            parts_angle = reinterpret_cast<i16 *>(&computed_angle);
        }
    }
    if (WORLD->current_level == VADERC_LDATA && !netclient) {
        GIZAIMESSAGE_s *message = CheckGizAIMessage(gizaimessagesys, "FinalFight", NULL);
        if (message && message->value == 1.0f && (!attacker || (attacker->apiobj.player_controlled))) {
            grab_screen_image = 1;
            if (FreePlay)
                CompleteLevel(WORLD);
            else {
                char name[16];
                nuhspecial_s special;
                for (i32 i = 1; i != 12; ++i) {
                    sprintf(name, "rock%d", i);
                    if (NuSpecialFind(WORLD->current_gscn, &special, name, 1))
                        NuSpecialSetVisibility(&special, 0);
                }
                if (attacker && attacker->id == id_OBIWANKENOBIEP3 && target->id == id_ANAKINJEDI) {
                    NewCutScene(NULL, WORLD->cutscene_sys, const_cast<char *>("ep3_darthvader_outro2"), 1);
                } else {
                    NewCutScene(NULL, WORLD->cutscene_sys, const_cast<char *>("ep3_darthvader_outro1"), 1);
                }
            }
            return 0;
        }
    }
    if (no_impact && target->field_0xcc0) {
        KillParts(target->field_0xcc0, -1, target->field_0xcc0->id == id_BODYGUARD ? 4 : -1, 1, 0.0f, 0,
                  reinterpret_cast<u16 *>(parts_angle));
        KillGameObject(target->field_0xcc0, 2, 0);
    }
    KillParts(target, -1, target->id == id_BODYGUARD ? 4 : -1, 1, throw_up ? 1.0f : 0.0f, 0,
              reinterpret_cast<u16 *>(parts_angle));
    KillGameObject(target, 2, 0);
    if (target->apiobj.player_controlled)
        GameCam_Judder(GameCam, 0.2f, 0, NULL);
    result = 2;
    goto finish;
surviving_hit:
    if (target->apiobj.player_controlled)
        TakeHitRumble(target, 0.666f);
    target->current_hp = health;
    if (hit != 0) {
        if (AIScriptSetBaseScriptStateByName(&target->ai.script_process, "LostHitPoints"))
            AIScriptProcess(WORLD->ai_sys, &target->apiobj, &target->ai, &target->ai.script_process, FRAMETIME);
        SetFlicker(target, 0.4f);
        if (target->id == id_SNAKE)
            SnakeBeenHit(target);
        PlayerTakeHit(target, attacker);
        if (target->apiobj.field_0x27c != -1 && hit > 0) {
            u8 value = target->field_0xe38 - hit;
            target->field_0xe38 = value ? value : 1;
            if ((target->apiobj.character_data->model_flags & 0x20) && target->apiobj.field_0x288) {
                i32 index = target->field_0xe38 - 1;
                if (index > 2)
                    index = 2;
                index = objhit_damage_joints[2 - index];
                if (target->apiobj.character_model->points_of_interest[index])
                    AddGameDebris(WORLD->debris_sys, 113,
                                  reinterpret_cast<NUVEC *>(&target->joint_matrices[index].m30));
            }
        }
    } else
        PlayerTakeHit(target, attacker);
    result = 1;
    if (target->id == id_BODYGUARD && target->current_hp == 1)
        KillParts(target, 4, -1, 1, 0.0f, 0, NULL);
    goto hurt;
hurt:
    if (!no_hurt && target->character_context != 23)
        PlayHurtSfx(target);
    goto finish;
finish:
    if (attacker && (attacker->apiobj.player_controlled)) {
        if (!no_rumble) {
            if (result == 2)
                KillRumble(attacker);
            else
                HitRumble(attacker);
        }
        if (!(flags & 0x4000) && result == 2 && static_cast<u8>(attacker->apiobj.field_0x27c) <= 1 && Arcade)
            Arcade_Kill(attacker->apiobj.field_0x27c, target->apiobj.field_0x27c);
    }
    return result;
refill:
    coins = 0;
    if ((target->apiobj.player_controlled) && target->coinpacket && target->coinpacket->coins && BonusWinner == -1)
        coins = LoseCoins(target, 1);
    AddPickups(coins, 0, 0, 0, &target->apiobj.collision_position, NULL, 2.0f,
               attacker ? attacker->apiobj.field_0x27c : -1, 1.0f, 2000000.0f, attacker, 1, 0, false);
    if (target->character_context != 23)
        SetFlicker(target, 0.4f);
    Player_ClearContext(target, 1);
    target->current_hp = target->hitpoints;
    result = 0;
    goto hurt;
}

GameObject_s *TargetGameObject(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius,
                               f32 range_squared, u32 model_mask, i32 directional, i32 require_drawn, i32 bolt_id) {
    NUVEC aim = *direction;
    BOLTTYPE_s *bolt = BoltType_FindByID(bolt_id, WORLD);
    u16 near_angle = TargetDeg_Near, mid_angle = TargetDeg_Mid, far_angle = TargetDeg_Far;
    f32 near_distance = TargetDist_Near2, mid_distance = TargetDist_Mid2;
    if (object->apiobj.player_controlled &&
        (WORLD->current_level == DEATHSTAR2BATTLEA_LDATA || WORLD->current_level == ASTEROIDCHASED_LDATA)) {
        near_angle = 0x2aaa;
        mid_angle = 0x1c71;
        far_angle = 0xe38;
        near_distance = 6.25f;
        mid_distance = 100.0f;
    }
    f32 min_x = position->x - radius, max_x = position->x + radius;
    f32 min_z = position->z - radius, max_z = position->z + radius;
    if (directional != 0) {
        aim = *direction;
        if (bolt != NULL && (bolt->field_60 & 0x20000) != 0) {
            aim.y = 0.0f;
            NuVecNorm(&aim, &aim);
        }
    }
    i32 arcade = Arcade_GetMode(NULL) == 99;
    GameObject_s *target = Obj;
    GameObject_s *best = NULL, *previous = NULL;
    f32 nearest_distance = range_squared;
    i32 count = HIGHGAMEOBJECT;
    for (i32 i = 0; i < count; ++i, ++target) {
        if (target == object || (target->apiobj.flags_high & 0x10) == 0 || target->apiobj.field_0x287 != 0 ||
            (require_drawn != 0 && target->apiobj.model_draw_result == 0) || (target->field_0xf00 & 1) != 0)
            continue;
        i8 context = target->character_context;
        if ((static_cast<u8>(context) & 0xfd) == 0x39 || context == 0x3c || (CInfo[context].flags & 0x8000) != 0)
            continue;
        if (model_mask != 0) {
            if ((target->apiobj.character_data->model_flags & model_mask) == 0)
                continue;
        } else if (arcade == 0 && ((target->apiobj.field_0x1f4 ^ object->apiobj.field_0x1f4) & 0x10001) == 0)
            continue;
        if (static_cast<u8>(context - 0x5f) <= 1)
            continue;
        if (arcade == 0 && object->apiobj.player_controlled && (target->field_0xf02 & 0x80) == 0) {
            u32 *mask = WORLD->api_object_sys->hostility_masks[object->apiobj.field_0x289];
            u64 hostility = static_cast<u64>(mask[0]) | (static_cast<u64>(mask[1]) << 32);
            if (((hostility >> target->apiobj.field_0x289) & 1) == 0)
                continue;
        }
        if (target->apiobj.collision_position.x < min_x || target->apiobj.collision_position.x > max_x ||
            target->apiobj.collision_position.z < min_z || target->apiobj.collision_position.z > max_z)
            continue;
        if (WORLD->current_level == DEATHSTARRESCUEA_LDATA && target->apiobj.field_0x27c == -1 &&
            target->apiobj.position.y < -1.5f && GameCam->sock_position.location.sock == 2)
            continue;
        NUVEC delta;
        f32 distance = NuVecDistSqr(&target->apiobj.collision_position, position, &delta);
        if (nearest_distance > distance) {
            if (directional == 0)
                NuVecRotateY(&aim, &v001,
                             NuAtan2D(target->apiobj.collision_position.x - position->x,
                                      target->apiobj.collision_position.z - position->z));
            if (bolt != NULL && (bolt->field_60 & 0x20000) != 0)
                delta.y = 0.0f;
            NuVecNorm(&delta, &delta);
            f32 dot = NuVecDot(&delta, &aim);
            u16 angle = near_distance > distance && directional != 0
                            ? near_angle
                            : (mid_distance > distance ? mid_angle : far_angle);
            if (dot > NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff]) {
                if (object->collision_target == target)
                    previous = target;
                else {
                    nearest_distance = distance;
                    best = target;
                }
            }
        }
        count = HIGHGAMEOBJECT;
    }
    return best != NULL ? best : previous;
}

void ClearLastSafeTakeOver(GameObject_s *object) {
    if (object == NULL || (object->field_0xefa & 0x10) != 0) {
        return;
    }
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *candidate = &Obj[i];
        if (candidate != NULL && (candidate->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            candidate->takeover_source == object) {
            candidate->takeover_source = NULL;
        }
    }
}
