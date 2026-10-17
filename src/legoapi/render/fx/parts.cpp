#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edpart_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmo/object/giztorpedo.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/motion/gameanim.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/render/fx/edsplines.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern ADDPART_s Default_ADDPART;

struct rtlset;
extern rtlset * PartRTL;
extern f32 PARTSCALEUPTIME;
void NewPartOrderedRotation(PART_s *);
void PartTimeSlip();
void PartCleanupTypes();
void UpdatePartEmits(f32);

extern "C" {
    void DebFreeInstantly(i32 *handle);
    // Shared suspend flag consulted by all timed debris emitters.
extern i32 debris_suspended;

    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    extern DEBRISGENERATOR gensorttab[13];
    extern DEBRISMOMENTUMADJUSTER gencodetab[7];
    extern f32 globaltime;
    extern f32 partglobaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    f32 CameraEmitterDistance(NUVEC *);
    void SetSfxBit_On(i32);
    void PlaySfxById(i32, NUVEC *);
    void DebrisEmitterMomentum(i32, f32, f32, f32);
    void DebrisParticleMomentum(i32, f32, f32, f32);
    void AddFiniteShotDebrisEffect2(i32 *, i32, NUVEC *, NUVEC *, NUVEC *, i32);
    void AddVariableShotDebrisEffectMtx3(i32, NUVEC *, NUVEC *, i32, NUMTX *, NUMTX *);
    void DebrisSetUserData(i32, void *);
    i32 CreateScaledPARTEffect(i32, f32);
    extern i32 debris_suspended;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern u32 partseed;
    extern NUGSCN *part_scene[32];

void NuPartEnableRayCasts(i32 enabled);

u32 NuPartGetSeed(void);

void NuPartSetSeed(i32 seed);

    extern part_type_s part_types[128];
    extern NUGSCN *part_scene[32];
    extern part_emit_s part_emits[40];
    extern i32 part_emits_used;
    void CheckPartCount();
    i32 AddPARTEffect(i32, NUVEC *);
    void PARTStartOffset(i32, f32);

    void ResetParts(void);

    i32 DebAlloc(void);
    void DebrisStartOffset(i32, f32);
    void DebrisEmitterPos(i32, f32, f32, f32);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisOrientation(i32, i16, i16);
    void DebrisOrientationMtx(i32, NUMTX *);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisEmitterOrientationMtx(i32, NUMTX *);
    i32 CreateScaledEffect(i32, f32);
    i32 NuCameraClipTestExtentsAxisAligned(NUVEC *, NUVEC *, f32);
    void NuVecAddScale(NUVEC *, NUVEC *, NUVEC *, f32);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
}

void AddDebrisEffectToStack(debkeydatatype_s *);
void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisProcessSpheres(uv1deb *, f32, debinftype *, debkeydatatype_s *, i32);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern f32 AreaPickupGravity;
extern i16 temp_xrot;
extern i16 temp_zrot;

// Forward declarations for local (static) part/gizmo helper stubs.
struct spacelevel_s;
struct quickboltinfo;



// Local (static) part (PART_s), pickup (GIZMOPICKUP_s), gizmo flow
// (GIZFLOW_s/FLOWBOX_s), power-up and space/starfighter helpers. Stubbed as
// local `t` symbols matching res/libTTapp.so.

extern GameObject_s *Obj;
extern i32 HIGHGAMEOBJECT, VehicleArea;
extern WORLDINFO_s *WORLD;
extern GAMECAMERA_s *GameCam;
extern f32 ForceThrowSpeed, ForceThrowGravity;
extern "C" void KillPart(PART_s *, i32);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);
void SetCoinType(i32 model, GIZMOPICKUP_s *pickup);
u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void ReleasePush(GameObject_s *);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
i32 CannotKill(GameObject_s *);

static void PartCollide(PART_s *part, i32 three_dimensional) {
    const f32 radius = part->field_0e4;
    NUVEC minimum = part->position;
    NUVEC maximum = part->position;
    minimum.x -= radius;
    maximum.x += radius;
    minimum.y -= radius;
    maximum.y += radius;
    minimum.z -= radius;
    maximum.z += radius;
    GameObject_s *object = Obj;
    i8 force_player_mask = part->force_player_mask;
    auto part_pickup_type = part->pickup_type;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        APIOBJECT_s *api = &object->apiobj;
        if ((api->field_0x1f8 & 0x1001) != 0x1001 || api->field_0x287 != 0)
            continue;
        i8 context = static_cast<i8>(object->character_context);
        if ((CInfo[context].flags & 0x8000) != 0 || (object->field_0xe20 & 0x20) != 0)
            continue;
        if (force_player_mask == 0) {
            if (part->owner == object) {
                if (part->scale_time < 0.5f)
                    continue;
            } else if (api->field_0x27c != -1 && part->scale_time < 0.25f)
                continue;
        } else if ((part->flags & 0x8000) == 0 && part->owner == object)
            continue;
        if (context == 0x39 || context == 0x3b || context == 0x3c)
            continue;
        if ((part->flags & 0x4000) != 0 && part->field_207 == api->field_0x289)
            continue;
        if ((part->flags & 4) != 0 && (api->flags_low & 0x80) == 0)
            continue;
        if (minimum.x > api->collision_max.x || api->collision_min.x > maximum.x || minimum.z > api->collision_max.z ||
            api->collision_min.z > maximum.z)
            continue;
        if (three_dimensional != 0) {
            if ((api->character_data->model_flags & 0x2000) != 0 && (part->flags & 0x40) != 0)
                goto collect_pickup;
            if (minimum.y > api->collision_max.y || api->collision_min.y > maximum.y)
                continue;
        }
        if ((part->flags & 0x40) == 0) {
            GAMECHARACTERDATA_s *character = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24);
            if ((character->flags_090 & 0x8000) != 0)
                continue;
            if (!SphereSphereOverlapScaleY(&part->position, part->field_0e4, part->field_0e4, &api->collision_position,
                                           api->collision_radius, api->field_0x1e0))
                continue;
            if (!((part->active & 2) != 0 && (part->flags & 0x1000008) == 0x1000008))
                KillPart(part, 1);
            if ((part->flags & 8) != 0 && !((part->flags & 0x1000000) != 0 && (part->active & 2) != 0) &&
                ((api->flags_low & 0x80) != 0 || (part->flags & 0x800000) == 0)) {
                u32 flags = CInfo[static_cast<i8>(object->character_context)].flags;
                if ((flags & 0x4000000) != 0 || ((flags & 0x8000000) != 0 && (object->jump_flags & 2) != 0)) {
                    i32 effect = -1;
                    if (object->id == id_BOB)
                        effect = (object->field_0xefd & 2) != 0 ? 2 : 3;
                    else if (object->blade_index != -1)
                        effect = BladeTab[object->blade_index].hit_effect;
                    DeflectPart(part, object, ForceThrowSpeed, ForceThrowGravity, 0, effect);
                } else {
                    GameCam_Judder(GameCam, 0.2f, 0, NULL);
                    ReleaseBuildIt(object, 0);
                    ReleasePush(object);
                    if (object->field_0xd24 >= 1.0f)
                        ObjHitShield(part->owner, object, object->field_0xe37, NULL);
                    else if (!CannotKill(object)) {
                        ObjHitObj((part->flags & 0x10000) != 0 ? NULL : part->owner, object, part->field_204,
                                  static_cast<u16>(part->force_flags), 0, 1);
                    } else if (part->owner != NULL) {
                        NewRumble(part->owner->pad_gamepad->pad, 0.5f, 0);
                        NewRumble(object->pad_gamepad->pad, 0.5f, 0);
                    }
                }
            }
            if (api->field_0x287 != 0 || (api->flags_low & 2) != 0)
                continue;
            if ((part->active & 1) == 0) {
                f32 speed = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24)->run_speed;
                if (api->scaled_radius > 1.0f)
                    speed /= api->scaled_radius;
                NUVEC direction;
                NuVecNorm(&direction, &part->velocity);
                api->velocity.x += direction.x * speed;
                api->velocity.z += direction.z * speed;
            }
            if ((part->flags & 0x2000000) != 0 && ((part->flags & 0x4000000) == 0 || (part->active & 2) != 0)) {
                u16 angle = NuAtan2D(api->position.x - part->position.x, api->position.z - part->position.z);
                f32 speed = static_cast<GAMECHARACTERDATA_s *>(api->character_data->field11_0x24)->run_speed;
                api->field_0x1fc = NU_SIN_LUT(angle) * speed;
                api->field_0x204 = NU_COS_LUT(angle) * speed;
            }
        } else {
        collect_pickup:
            if ((part->active & 2) == 0) {
                if (part_pickup_type == 0xcb) {
                    if (!(part->scale_time >= 0.5f))
                        continue;
                    if (VehicleArea == 0 && (static_cast<i8>(object->current_hp) <= 0 ||
                                             static_cast<i8>(object->current_hp) >= object->hitpoints))
                        continue;
                } else if (!(part->scale_time >= 0.1f))
                    continue;
            }
            if ((api->field_0x1f4 & 0x40000) != 0) {
                KillPart(part, 2);
                continue;
            }
            if (part_pickup_type == 0xcb) {
                CollectHitPoint(object, &part->position, 1);
                KillPart(part, 2);
            } else if (part_pickup_type == 0xd0) {
                CollectPowerUp(object, &part->position, part->rotation_y, 1);
                KillPart(part, 2);
            } else if (force_player_mask == 3) {
                if ((api->character_data->model_flags & 0x2000) == 0 || object->torpedo == NULL)
                    continue;
                if (object->torpedo->count < getMaxTorpedos(object)) {
                    TORPEDOPACKET_s *packet = object->torpedo;
                    packet->pickup_positions[packet->count] = part->position;
                    packet->pickup_data[packet->count] = part->field_10c[0];
                    packet->pickup_flags[packet->count] = part->field_10c[1];
                    ++packet->count;
                    object->torpedo->field_08 = 0.0f;
                    object->torpedo->field_03 = 0;
                    KillPart(part, 2);
                }
            } else {
                GIZMOPICKUP_s pickup;
                SetCoinType(part_pickup_type, &pickup);
                GizmoPickup_CollectCoin(WORLD, &part->position, pickup.type_index, pickup.model_variant, object, 1);
                KillPart(part, 2);
            }
        }
    }
}


extern "C" f32 coinimpactwait;
extern i32 GAMEDEMO;
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);

static __used__ void PartImpact_Coin(PART_s *part);

static __used__ void PartStolen_Coin(PART_s *part);

extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
static __used__ void PartExtra_BlueCoin(PART_s *part);

static __used__ void PartExtra_PurpleCoin(PART_s *part);

void PowerUp_Particles(WORLDINFO_s *, NUVEC *);

static __used__ i32 PowerUp_DrawPart(PART_s *part);

static __used__ void PowerUp_ImpactPart(PART_s *part);

static __used__ void PowerUp_UpdatePart(PART_s *part);

static __used__ void PowerUp_EndMsg(GAMEMESSAGE_s *message);

static __used__ void PowerUp_UpdateMsg(GAMEMESSAGE_s *message);

void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern WORLDINFO_s *WORLD;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;

static __used__ i32 PartDraw_VehicleHeart(PART_s *part);

static __used__ void PartMove_VehicleHeart(PART_s *part, f32);

static __used__ void PartMove_VehiclePickup(PART_s *part, f32);

extern "C" {

void AddDebrisEffect(i32 *handle, i32 effect_index, f32 x, f32 y, f32 z);

void AddFiniteShotDebrisEffect(i32 *handle, i32 effect, NUVEC *position, i32 count);

void AddFiniteShotDebrisEffect2(i32 *handle, i32 effect, NUVEC *position, NUVEC *emitter_momentum,
                                    NUVEC *particle_momentum, i32 count);

void AddFiniteShotDebrisEffectUserData(i32 *handle, i32 effect, NUVEC *position, i32 count, void *user_data);

i32 AddFiniteShotPART(i32 effect, NUVEC *position, i32 count);

    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 AddPARTEffect(i32 effect, NUVEC *position);

    void KillPart(PART_s *, i32);
    void NewPartRotation(PART_s *);
    void DebrisPopulateInstance(i32, f32);
    i32 rtlDynamicAllocTemplate(rtlset *, i32);

PART_s *AddPart(ADDPART_s *params);

void AddRotatedDebrisEffect(i32 *handle, i32 effect, f32 x, f32 y, f32 z, i16 rotation_x, i16 rotation_y);

void AddScaledFiniteShotDebrisEffect(i32 *key, i32 effect, NUVEC *position, NUVEC *orientation, NUVEC *momentum,
                                         i32 count, f32 scale);

i32 AddScaledFiniteShotPART(i32 effect, NUVEC *position, i32 count, f32 scale);

i32 AddScaledVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                          i16 y_rotation, f32 scale);

    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);

i32 AddScaledVariableShotDebrisEffect1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect2(i32 effect_index, NUVEC *position, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, f32 scale);

i32 AddScaledVariableShotDebrisEffect4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count, i32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                           i8 flags, f32 scale);

i32 AddScaledVariableShotDebrisEffect5(i32 effect, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 time, NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                           u16 priority, i8 flags, f32 scale);

void AddScaledVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation,
                                         f32 scale);

    void AddVariableShotDebrisEffectMtx(i32, NUVEC *, i32, i16, i16, NUMTX *);

void AddVariableShotDebrisEffect(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation);

void AddVariableShotDebrisEffectMtx(i32 effect, NUVEC *position, i32 count, i16 z_rotation, i16 y_rotation,
                                        NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx3(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);

    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);

void AddVariableShotDebrisEffectTimed1(i32 effect, NUVEC *position, i32 count, f32 time, i16 z_rotation,
                                           i16 y_rotation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed3(i32 effect_index, NUVEC *position, NUVEC *momentum, i32 count, f32 time,
                                           NUMTX *emitter_orientation, NUMTX *particle_orientation);

void AddVariableShotDebrisEffectTimed5(i32 effect_index, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 duration, NUMTX *emitter_orientation,
                                           NUMTX *particle_orientation, u16 render_priority, i8 timed_flags);

void AddVariableShotPARTEffect(i32 effect_index, NUVEC *position, f32 rate, f32 duration, NUMTX *orientation);

i32 PartRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 flags);

i32 PartPlatId();

void CastPart(PART_s *part, f32 time);

void CheckPartCount(void);

void DrawParts(i32 keep_offscreen);

PART_s *FindPart(NUVEC *position, i32 player, GameObject_s *owner);

void SetPartRTLSet(usize rtl_set);

i32 GetMaxPartTypes(void);

i32 GetPartCount(void);

char *GetPartName(i32 index);

extern part_type_s part_types[128];
    extern part_emit_s part_emits[40];
    extern i32 part_page_on[8];
    extern i32 part_page_used[8];
extern i32 part_types_used;
extern i32 part_emits_used;
extern NUGSCN * part_scene[32];
    // The editor loader clears these page IDs with aligned vector stores.
extern i32 part_scene_pageid[32];
extern i32 part_platimpactcnt;
extern i16 part_platimpactlist[4];
    void ResetParts(void);

void InitParts(i32 count, VARIPTR *buffer, VARIPTR *);

void KillAllParts(void);

    void DebFreeInstantly(i32 *);
    void rtlDynamicFree(i32);

void KillPart(PART_s *part, i32 reason);

void KillPartsByScene(NUGSCN *scene);

void NewPartRotation(PART_s *part);

void PARTEmitterOrientation(i32 index, i16 x_rotation, i16 y_rotation, i16 z_rotation);

void PARTEmitterPos(i32 index, f32 x, f32 y, f32 z);

f32 PARTGetTotalOffTime(i32 index);

f32 PARTGetTotalOnTime(i32 index);

i32 PARTLookupType(char *name);

extern i32 edpart_load_particle_page;

void edpartSetParticlePage(i32 page);

void edpartStartPage(i8 page);

void edpartStopPage(i8 page);

i32 PARTLookupTypePageOnly(char *name, i32 page);

void PARTStartOffset(i32 index, f32 offset);

void ParticleReset(void);

void RemovePARTEffect(i32 index);

void ResetParts(void);

    void DebrisStatusAlwaysOff(i32 *);
    i32 rtlDynamicSetPos(i32, NUVEC *);

void UpdateParts(f32 time);

} // extern "C"

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *TargetPart(GameObject_s *object, nuvec_s *position, nuvec_s *direction, f32 radius, f32 range_squared,
                   i32 directional, i32 bolt_id) {
    NUVEC delta;
    NUVEC aim = *direction;
    BOLTTYPE_s *bolt = BoltType_FindByID(bolt_id, WORLD);
    PART_s *part = Part;
    f32 max_x = position->x + radius, min_x = position->x - radius;
    f32 min_z = position->z - radius, max_z = position->z + radius;
    if (directional != 0) {
        aim = *direction;
        if ((bolt->field_60 & 0x20000) != 0) {
            aim.y = 0.0f;
            NuVecNorm(&aim, &aim);
        }
    }
    PART_s *best = NULL;
    PART_s *previous = NULL;
    f32 nearest_distance = range_squared;
    for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
        if ((part->active & 1) == 0 || ((part->flags & 0x8000000) == 0 && (part->flags & 0xa) != 0xa))
            continue;
        if (part->position.x < min_x || part->position.x > max_x || part->position.z < min_z ||
            part->position.z > max_z)
            continue;

        f32 distance = NuVecDistSqr(&part->position, position, &delta);
        if (!(range_squared > distance))
            continue;
        if (directional == 0)
            NuVecRotateY(&aim, &v001, NuAtan2D(part->position.x - position->x, part->position.z - position->z));
        if ((bolt->field_60 & 0x20000) != 0)
            delta.y = 0.0f;
        NuVecNorm(&delta, &delta);
        f32 dot = NuVecDot(&delta, &aim);
        u16 angle;
        if (TargetDist_Near2 > distance && directional != 0)
            angle = TargetDeg_Near;
        else if (TargetDist_Mid2 > distance)
            angle = TargetDeg_Mid;
        else
            angle = TargetDeg_Far;
        if (!(dot > NuTrigTable[((angle + 0x4000) >> 1) & 0x7fff]) || !(nearest_distance > distance))
            continue;
        if (object->attack_part_target == part)
            previous = part;
        else {
            nearest_distance = distance;
            best = part;
        }
    }
    return best != NULL ? best : previous;
}

extern "C" i32 NuGScnNumSpecials(nugscn_s *);
i32 edpartLookupObjectInScene(char *name, nugscn_s *scene);
i32 edpartLookupDebrisEffect(char *name);
extern "C" i32 GetSfxIdN(char *, i32);

void edpartLoadSingleType(part_typedesc_s *type, i32 version, i32 page);

extern "C" {
extern i32 edpart_nearest;
extern NUVEC edpart_cam_pos;
extern part_emit_s * edpart_nearest_emit;
extern part_typedesc_s * edpart_nearest_type;
}

void edpartDetermineNearest(f32 distance);
extern "C" {
extern i32 edbits_part_level_page;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}

void PartTimeSlip();

extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);

static i32 PDEBCOUNT;
static char **PDebNameList;

void InitPartTable(char **names) {
    PDEBCOUNT = 0;
    PDebNameList = names;
    if (names != NULL) {
        while (names[PDEBCOUNT] != NULL) {
            ++PDEBCOUNT;
        }
    }
}

void LoadPartFile(WORLDINFO *world) {
    char path[256];
    world->page_part = -1;
    edpartSetParticlePage(world->page_pp);

    if ((world->current_level->flags & (LEVEL_OUTRO | LEVEL_MIDTRO | LEVEL_INTRO)) == 0) {
        sprintf(path, "%s.par", world->config_file);
        if (NuFileExists(path) > 0)
            world->page_part = edpartLoadPage(path, 1, world->current_gscn);
        world->part_debris_sys = static_cast<PARTDEBSYS_s *>(
            InitPartDebris(&world->giz_buffer, &world->unknown_0108, 0x40, PDEBCOUNT, PDebNameList, world->page_part));
    }
}

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position) {
    if (index >= 0 && system != NULL && index < PDEBCOUNT) {
        const i32 type = system->entries[index].type_id;
        if (type != -1) {
            AddFiniteShotPART(type, position, 1);
        }
    }
}

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_3D(PART_s *);
void PartCollide_2D(PART_s *);
void PartUpdate_Heart(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);

void AddTorpedoAsPart(nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void AddHeartAsPart(GameObject_s *recipient, nuvec_s *position, nuvec_s *velocity, float scale, float lifetime);

void PowerUp_AddPart(NUVEC *position, NUVEC *velocity, f32 scale, f32 lifetime);

void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name) {
    if (system != NULL) {
        for (i32 i = system->named_count; i < system->capacity; ++i) {
            if (NuStrICmp(name, system->entries[i].name) == 0)
                return system->entries[i].type_id;
        }
    }
    return -1;
}

void MakePartVector(NUVEC *velocity, NUVEC *direction, f32 scale) {
    if (direction == NULL) {
        f32 random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->x = random + random - 1.0f;
        velocity->y = AreaPickupGravity == 0.0f ? 0.0f : static_cast<f32>(qrand()) * (1.0f / 65535.0f) + 1.0f;
        random = static_cast<f32>(qrand()) * (1.0f / 65535.0f);
        velocity->z = random + random - 1.0f;
        NuVecScale(velocity, velocity, AreaPickupScale);
    } else {
        FindAnglesZX(direction, NULL, NULL);
        *velocity = v010;
        const i32 z_random = qrand();
        const i32 x_random = qrand();
        NuVecRotateZ(velocity, velocity, static_cast<u16>(temp_zrot + z_random / 6 - 0x1555));
        NuVecRotateX(velocity, velocity, static_cast<u16>(temp_xrot + x_random / 6 - 0x1555));
        NuVecScale(velocity, velocity, scale);
    }
}

void PartCollide_2D(PART_s *part) {
    PartCollide(part, 0);
}

void PartCollide_3D(PART_s *part) {
    PartCollide(part, 1);
}

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartStop_Coin(PART_s *part);

void AddCoinsAsParts(i32 type_id, nuvec_s *position, nuvec_s *velocity, float lifetime, float scale);

// The original repeats the per-sound emitter checks for each of the four sound slots.
#define PART_EMIT_SOUND_ON(slot)                                                                                       \
    if (type->sounds[slot] != -1) {                                                                                    \
        SetSfxBit_On(type->sounds[slot]);                                                                              \
        if (type->sound_modes[slot] == 4)                                                                              \
            PlaySfxById(type->sounds[slot], &emitter->position);                                                       \
    }
#define PART_EMIT_SOUND(slot, mode)                                                                                    \
    if (type->sounds[slot] != -1 && type->sound_modes[slot] == (mode))                                                 \
        PlaySfxById(type->sounds[slot], &emitter->position);

void UpdatePartEmits(f32 time);
#undef PART_EMIT_SOUND_ON
#undef PART_EMIT_SOUND

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);

PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range) {
    PART_s *part = Part;
    radius *= radius;
    f32 nearest_distance = range > 0.0f ? range : 100.0f;
    PART_s *nearest = NULL;
    for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
        if ((part->active & 1) == 0 || part->owner == owner || (part->flags & flags) != flags)
            continue;
        if (range > 0.0f) {
            f32 distance = NuVecDistSqr(&part->position, position, NULL);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = part;
            }
        } else {
            f32 speed = NuVecMag(&part->velocity);
            if (speed != 0.0f) {
                NUVEC direction;
                NuVecScale(&direction, &part->velocity, 1.0f / speed);
                f32 reach = speed * 0.5f;
                reach *= reach;
                f32 distance = NuVecDistSqr(&part->position, position, NULL);
                if (distance < reach && LineIntersectSphere(&part->position, &direction, position, radius, NULL)) {
                    nearest_distance = distance;
                    nearest = part;
                }
            }
        }
    }
    return nearest;
}

void edpartDestroy(i32 index);

void PartCleanupTypes();

extern "C" f32 brickimpactwait;
extern "C" i32 GetSfxId(const char *);
extern "C" void PlaySfxByIdAndSetVolume(i32, NUVEC *, f32);

void PartImpact_Brick(PART_s *part) {
    if (brickimpactwait <= 0.0f) {
        f32 speed = NuVecMag(&part->velocity);
        if (speed > 0.0f) {
            if (speed > 1.0f)
                speed = 1.0f;
            f32 volume = NU_SIN_LUT(static_cast<i32>((1.0f - speed) * 16384.0f + 16384.0f));
            PlaySfxByIdAndSetVolume(GetSfxId("LegoSingle"), &part->position, volume);
            brickimpactwait = 0.1f;
        }
    }
}

void PartUpdate_Heart(PART_s *part) {
    NUVEC position = part->position;
    part->rotation_y += 16384.0f * FRAMETIME;
    NuMtxSetRotationY(&part->transform, part->rotation_y);
    NuMtxTranslate(&part->transform, &position);
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

void PartStop_Flickerer(PART_s *part) {
    part->field_100 = 5.0f;
    if ((part->field_20a & 1) != 0)
        part->field_100 = 2.5f;
}

i32 PartDraw_Flickerer(PART_s *part) {
    if (part->field_100 > 0.0f) {
        if (PickupFlickerFrame % PickUpFlickerFrames < PickUpFlickerTest)
            return 1;
        f32 threshold = (part->field_20a & 1) == 0 ? 2.0f : 1.0f;
        return !(part->field_100 < threshold);
    }
    return 1;
}

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32) {
    LEVELDATA *level = WorldInfo_CurrentlyActive()->current_level;
    i32 debris = -1;
    i32 part_debris = -1;
    if (level == CLOUDCITYTRAPB_LDATA) {
        debris = 0x75;
        part_debris = 8;
    } else if (level == MAULA_LDATA) {
        debris = 0x2d;
        part_debris = 9;
    } else if (level == MAULF_LDATA) {
        i32 type = 11;
        if (NuSpecialExistsFn(&LevHSpecial[0]) && NuSpecialCompare(&LevHSpecial[0], &part->special))
            type = 11;
        else if (NuSpecialExistsFn(&LevHSpecial[1]) && NuSpecialCompare(&LevHSpecial[1], &part->special))
            type = 12;
        else if (NuSpecialExistsFn(&LevHSpecial[2]) && NuSpecialCompare(&LevHSpecial[2], &part->special))
            type = 13;
        part_debris = type;
    } else if (level == DOOKUC_LDATA) {
        debris = 0x34;
        part_debris = 10;
    }
    if (debris != -1)
        AddGameDebris(WORLD->debris_sys, debris, &part->position);
    if (part_debris != -1)
        AddPartDebris(WORLD->part_debris_sys, part_debris, &part->position);
    NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
    GameCam_Judder(GameCam, qrand() > 0x7fff ? -0.4f : 0.4f, 2, NULL);
    GameCam_NewShake(GameCam, 0.6f, 0.6f, 1.0f);
    PlaySfx("Explode1", &part->position);
}

void PartImpact_Basketball(PART_s *part);

void PartUpdate_Basketball(PART_s *part);

PART_s *Part_FindFromHSpecial(nuhspecial_s *special) {
    if (special != NULL) {
        PART_s *part = Part;
        for (i32 i = 0; i < MAXPARTS; ++i, ++part) {
            if ((part->active & 1) != 0 && NuSpecialCompare(&part->special, special))
                return part;
        }
    }
    return NULL;
}

void NewPartOrderedRotation(PART_s *part);
