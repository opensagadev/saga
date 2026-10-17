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
#include "legoapi/gizmos/object/newblowup.h"
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
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "MechInputTouch/MechInputTouch_types.h"

#include <string.h>
#include <stdio.h>

extern "C" i32 dagobah_training;
extern i32 obstacle_gizmotype_id;
extern i32 LevFlag[4];
#define LEV_FLAG_U8 (reinterpret_cast<u8 *>(LevFlag))
extern MechWeakPtr<MechObjectInterface, 4> BobaRocketTarget;
extern "C" i16 id_PROBEDROID, id_ATST_LOWRES, id_ATAT;
GIZPANEL_s *LevGizPanel;
AILOCATOR_s *locator;
GameObject_s *gameobj;
extern u8 troopercannons_beenReset;
void Asteroid_PartKill(PART_s *, i32);
void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *);
void PartCollide_3D(PART_s *);
void ResetTrooperCannons(WORLDINFO_s *, i32);
void InitTrooperCannons(WORLDINFO_s *);
void HothBattleE_UpdateWave();
void HothBattle_Melee_init(HOTHBATTLE_MELEE_s *);
i32 HothBattle_StartNewWave();
void HothBattle_ManageBackgroundCreatures();
void UpdateTrooperCannons(WORLDINFO_s *);
extern "C" void NewPartRotation(PART_s *);
extern "C" void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
nugspline_s *edSpline_SplineFind(nugscn_s *, char *);
extern i16 BoltType_FindIDByNameWide(char *, WORLDINFO_s *) asm("_Z21BoltType_FindIDByNamePcP11WORLDINFO_s");

static GameObject_s *Vader_obj;
static GIZAIMESSAGE_s *Vader_ai_message;
static u8 turretAliveCount;
nuhspecial_s specialIcon;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern "C" {
    GIZBOMBGEN *HothBattleC_BombGenerator = NULL;
    HOTHBATTLE_MELEE_s melee;
    u8 dagobahA_nodesNeedUpdating = 1;
}
#include "legoapi/world/levels/episodeV_types.h"
DECOMP_ASSERT(sizeof(HOTHBATTLEE_NETPACKET_s) == 0x58, "Hoth panel packet size");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, alpha) == 0x24, "Hoth panel packet alpha offset");
DECOMP_ASSERT(offsetof(HOTHBATTLEE_NETPACKET_s, count) == 0x54, "Hoth panel packet count offset");
HOTHBATTLEE_NETPACKET_s *hothbattlee_netpacket;

void DagobahA_Init(WORLDINFO_s *world);

void DagobahB_Init(WORLDINFO_s *);

void DagobahC_Init(WORLDINFO_s *world);

void DagobahB_Reset(WORLDINFO_s *world);

void DagobahE_Init(WORLDINFO_s *world);

void DagobahC_Panel(WORLDINFO_s *);


void BobaRocket_Kill(PART_s *part, i32);

void BobaRocket_Move(PART_s *part, f32 elapsed);

void DagobahA_Update(WORLDINFO_s *world);

void HothBattleA_Draw(WORLDINFO_s *world);

void HothBattleA_Init(WORLDINFO_s *world);

void HothBattleB_Init(WORLDINFO_s *world);

void HothBattleC_Draw(WORLDINFO_s *world);

void HothBattleC_Init(WORLDINFO_s *world);

void HothBattleE_Draw(WORLDINFO_s *world);

void HothBattleE_Init(WORLDINFO_s *world);

void HothEscapeA_Init(WORLDINFO_s *world);

void HothEscapeB_Init(WORLDINFO_s *world);

void HothEscapeC_Init(WORLDINFO_s *world);

void HothEscapeD_Init(WORLDINFO_s *world);

void HothBattleA_Reset(WORLDINFO_s *world);

void HothBattleC_Reset(WORLDINFO_s *world);

static f32 alpha[16];

void HothBattleE_Panel(WORLDINFO_s *);

void HothEscapeA_Reset(WORLDINFO_s *);

void HothEscapeB_Reset(WORLDINFO_s *world);

void HothEscapeC_Reset(WORLDINFO_s *);

void HothEscapeD_Reset(WORLDINFO_s *);

void BobaRocket_Deflect(PART_s *part);

void HothBattleA_Update(WORLDINFO_s *world);

void HothBattleC_Update(WORLDINFO_s *world);

void HothBattleE_Update(WORLDINFO_s *world);

void HothEscapeA_Update(WORLDINFO_s *world);

void HothEscapeB_Update(WORLDINFO_s *world);

void HothEscapeC_Update(WORLDINFO_s *world);

void HothEscapeD_Update(WORLDINFO_s *world);

void CloudCityTrapA_Init(WORLDINFO_s *world);

void CloudCityTrapB_Init(WORLDINFO_s *world);

void CloudCityTrapA_Reset(WORLDINFO_s *);

void CloudCityTrapC_Panel(WORLDINFO_s *);

void CloudCityTrapC_Reset(WORLDINFO_s *world);

void CloudCityEscapeA_Init(WORLDINFO_s *world);

void CloudCityEscapeC_Init(WORLDINFO_s *world);

void CloudCityTrapB_Update(WORLDINFO_s *);

void CloudCityTrapA_Update(WORLDINFO_s *world);

void CloudCityTrapC_Update(WORLDINFO_s *);

void HothBattle_Melee_init(HOTHBATTLE_MELEE_s *melee);

void CloudCityEscapeA_Panel(WORLDINFO_s *);

void CloudCityEscapeA_Reset(WORLDINFO_s *world);

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


void CloudCityEscapeA_Update(WORLDINFO_s *);

void CloudCityEscapeC_Update(WORLDINFO_s *);


void HothEscapeC_AlwaysUpdate(WORLDINFO_s *world);

i32 isHothBattleWaveCreature(GameObject_s *object);

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


// ===========================================================================
// Asteroid chase (AsteroidChase_A / B / C / D)
// ===========================================================================

DECOMP_ASSERT(sizeof(ASTEROID_s) == 0x18, "ASTEROID_s size");

DECOMP_ASSERT(sizeof(FINALASTEROID_s) == 0x3c, "final asteroid size");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, blowup_count) == 0x2c, "final asteroid blowup count offset");
DECOMP_ASSERT(offsetof(FINALASTEROID_s, rotation_x) == 0x34, "final asteroid rotation offset");

DECOMP_ASSERT(sizeof(ASTEROIDCNETPACKET_s) == 8, "asteroid C network packet size");

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

static void DrawFalconSpotLights(GameObject_s *object);

static void Asteroid_AddParts(GIZMOBLOWUP_s *blowup);

static __used__ void Asteroids_Update();

static void Asteroids_Reset(WORLDINFO_s *world);

void AsteroidChaseA_Init(WORLDINFO_s *world);

void AsteroidChaseB_Init(WORLDINFO_s *world);

void AsteroidChaseB_Draw(WORLDINFO_s *);

void AsteroidChaseC_Init(WORLDINFO_s *world);

void AsteroidChaseD_Init(WORLDINFO_s *world);

void AsteroidChaseA_Reset(WORLDINFO_s *world);

void AsteroidChaseB_Reset(WORLDINFO_s *world);

void AsteroidChaseC_Reset(WORLDINFO_s *world);

void AsteroidChaseD_Panel(WORLDINFO_s *);

void AsteroidChaseA_Update(WORLDINFO_s *);

void AsteroidChaseB_Update(WORLDINFO_s *world);

void AsteroidChaseC_Update(WORLDINFO_s *);

static inline void AsteroidChaseD_SetTurretTarget(i32 index, NUVEC *position) {
    GIZTURRET_s *turret = StarDestroyerTurrets[index];
    if (turret != NULL && turret->field_0xe4 == NULL) {
        turret->field_0xe4 = position;
        turret->field_0x12c = 2;
    }
}

void AsteroidChaseD_Update(WORLDINFO_s *);

// ==== src/legoapi/render/fx/parts.cpp: declarations ====
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
void NewPartOrderedRotation(PART_s *);
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
void CollectPowerUp(GameObject_s *, NUVEC *, u16, i32);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void NewBuzz(nupad_s *, f32, i32);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
i32 CannotKill(GameObject_s *);



extern "C" f32 coinimpactwait;
extern i32 GAMEDEMO;
extern "C" void PlaySfxAndSetVolume(char *, NUVEC *, f32);



extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);


void PowerUp_Particles(WORLDINFO_s *, NUVEC *);






void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32);

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;




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
                   i32 directional, i32 bolt_id);

extern "C" i32 NuGScnNumSpecials(nugscn_s *);


extern "C" {
extern i32 edpart_nearest;
extern NUVEC edpart_cam_pos;
extern part_emit_s * edpart_nearest_emit;
extern part_typedesc_s * edpart_nearest_type;
}

extern "C" {
extern i32 edbits_part_level_page;

i32 edpartLoadPageEx(char *path, i8 mode, nugscn_s **scenes, i32 scene_count);

i32 edpartLoadPage(char *path, i32 mode, void *scene);
}


extern "C" PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum,
                            NUVEC *maximum, u32 flags);


void InitPartTable(char **names);

void LoadPartFile(WORLDINFO *world);

void AddPartDebris(PARTDEBSYS_s *system, i32 index, nuvec_s *position);

i32 SetPartTarget(GameObject_s *object, PART_s *target);

WORLDINFO_s *WorldInfo_CurrentlyActive();
extern f32 FRAMETIME;
void PartCollide_2D(PART_s *);
void PartUpdate_Heart(PART_s *);
void PartStop_Flickerer(PART_s *);
i32 PartDraw_Flickerer(PART_s *);




void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);

i32 FindPartDebris(PARTDEBSYS_s *system, char *name);


void PartCollide_2D(PART_s *part);

void PartCollide_3D(PART_s *part);



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


PART_s *FindIncomingPart(void *owner, NUVEC *position, f32 radius, u32 flags, f32 range);



extern "C" f32 brickimpactwait;
extern "C" i32 GetSfxId(const char *);
extern "C" void PlaySfxByIdAndSetVolume(i32, NUVEC *, f32);

void PartImpact_Brick(PART_s *part);

void PartUpdate_Heart(PART_s *part);

void Asteroid_PartKill(PART_s *part, i32 reason);

void PartStop_Flickerer(PART_s *part);

i32 PartDraw_Flickerer(PART_s *part);

void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);

void PartKill_ForceThrow(PART_s *part, i32);

void PartImpact_Basketball(PART_s *part);

void PartUpdate_Basketball(PART_s *part);

PART_s *Part_FindFromHSpecial(nuhspecial_s *special);

void NewPartOrderedRotation(PART_s *part);

// ==== src/legoapi/world/levels/episode.cpp: declarations ====
#include "legoapi/world/levels/episode.h"

#include "MechInputTouch/MechInputTouch_types.h"
#include "gameapi/edtools/edstubs.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/playeritems.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nuspline.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/numtx_inline.h"

#include <stdio.h>
#include <string.h>



#include "legoapi/world/levels/episode_types.h"
DECOMP_ASSERT(sizeof(TROOPERCANNON_s) == 0x30, "Trooper cannon state size");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, character_name) == 0x0c, "Trooper cannon name offset");
DECOMP_ASSERT(offsetof(TROOPERCANNON_s, rebuilding) == 0x2c, "Trooper cannon rebuilding offset");

TROOPERCANNON_s troopercannons[4];


extern i32 GizBuildIt_AtEnd(GIZBUILDIT_s *buildit);
extern GIZMO *GizmoFindByData(GIZMOSYS *system, i32 type_id, void *data);

extern f32 text3d_width;

void Text_MakeScore(u32 score, char *text);

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"


// Episode-global logic and helpers shared across episodes: completion/
// episode bookkeeping, super-story, boss/cutscene-adjacent helpers. The
// per-episode level *handlers* live in the matching episodeI..VI.cpp files.

// ===========================================================================
// Episode bookkeeping
// ===========================================================================

GameObject_s *FindGameObject(i32, u32, i32, i32, i32);

GameObject_s *BossKilled(i32 character_id);

void ReCalculateCompletionPoints();


void GizmoBlowupCreateStuff(GIZMOBLOWUP_s *blowup);

// ===========================================================================
// HUD / score helpers
// ===========================================================================

// ===========================================================================
// Shared gameplay helpers
// ===========================================================================




void InitMiniSnowTroopers(WORLDINFO_s *world, i32 team_count, i32 trooper_count, i32 use_droids);



bool FireBountyHunterRocket(GameObject_s *object);

static __used__ void KilledTrooperCannon(GameObject_s *object);


void ResetTrooperCannons(WORLDINFO_s *world, i32 trooper_id);







void UpdateMiniSnowTroopers(WORLDINFO_s *world);


void DrawMiniSnowTroopers(WORLDINFO_s *world);

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"


// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern f32 animduration_blendouttime;
    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}

struct AISYS_s;
void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS_s *sys);
void BackDrop_Init(char *path, VARIPTR *buf, VARIPTR *buf_end);


// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.

extern "C" ripple_set_s *ripples;
extern "C" NUMTL *ShadowMat;


void CharConfig_ConfigureAll(i32 permanent, nufpcomjmp_s *game_keywords);

extern i32 GetMenuID(void);
extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);
extern CUSTOMISER *CharacterCustomiser;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
extern "C" i32 waiting_for_character;
extern f32 WaitingForCharacterTime;

extern "C" {
extern i32 hub_character_ready;
}


#include "legoapi/characters/core/characters_types.h"

void LSW_SetIndy(i32);




void CharScene_Draw(WORLDINFO_s *world, i32 character_id, numtx_s *matrix, numtx_s *reflection_matrix);

i32 CharIDFromName(char *name);

CHARACTERDATA *CDataFromName(char *name);

i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path);


void CharScenes_LevelLoad(WORLDINFO *world);

extern i16 tUNKNOWN;
extern "C" {
    extern i16 id_R2Q5;
    void PlaySfx(char *, nuvec_s *);
    void PlaySfxAndSetVolume(char *, nuvec_s *, f32);
}
void Move_CHARACTER(GameObject_s *);
void Animate_CHARACTER(GameObject_s *);


void PostAnimate_FETT(GameObject_s *object);

void ResetAICreature(GameObject_s *, AISYS_s *);
void LightGameObject(GameObject_s *, void *);
void InitSurfaceInfo(GameObject_s *);
i32 SetObjOnSurface(GameObject_s *, i32);
extern "C" NUVEC plr_lastpos;

void FinishWeirdoNames(i32 which);

GameObject_s *ActivateCharacter(char *name, nuvec_s *position, i32 angle);


extern void SetGameObjectCharacterData(GameObject_s *obj);
extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);



// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif
static MOSEISLEY_LAYERS_ATTR void SetLayers_MOSEISLEYCITIZEN(u32 *layers)
    __asm__("_ZL26SetLayers_MOSEISLEYCITIZENP12GameObject_s.isra.2");
static void SetLayers_MOSEISLEYCITIZEN(u32 *layers);

i32 InitCreature(GameObject_s *obj, i32 id, i32 param);

void DestroySnakeBody(GameObject_s *);
void InitPlayerAI(GameObject_s *);
void Player_ClearContext(GameObject_s *, i32);
void ResetPlayerMoves(GameObject_s *);
void Player_ResetContexts(PLAYERPACKET_s *);
void GizForce_ResetLOS(GameObject_s *);
i32 StartSlide(GameObject_s *, i32);

i32 NewPlayerCharacter(GameObject_s *object, i32 id, i32 old_id, i32);


i32 GetDefaultIdle(GameObject_s *obj);

void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation);



void UpdateCharacterIDs();


void CharScenes_AreaDump();

void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function);

void DeactivateGameObject(GameObject_s *);

void DeactivateCharacter(char *name);

void LoadSingleCharacter(bgprocinfo_s *);


void UpdateCharacterLoad();

extern "C" VARIPTR characterbuffer_base;
extern i32 CHARACTERBUFFERSIZE;
extern i32 Area;
extern i32 last_area;

void ResetCharacterBuffer(i32 force_reset);

void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern f32 ICONSIZE;
extern i32 STATUS_R, STATUS_G, STATUS_B;




void PostAnimate_ASTROMECH(GameObject_s *object);

AILOCATOR_s *LocalGetRandomLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                   f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);







void ScaleGameObject(GameObject_s *obj);

void SetGameObjectCharacterData(GameObject_s *obj);



i32 TakeOverYodaSeekDistanceHack(GameObject_s *object, GameObject_s *luke, nuvec_s *offset);


void SetProtocolDroidDeactivatedAction(GameObject_s *object);




// ==== src/legoapi/items/collect/minikits.cpp: declarations ====
#include "decomp.h"
#include "legoapi/items/collect/minikits.h"
#include "globals.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/lighting.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


extern HUBMINIKITPIECES_s ** Char_MiniKit;

void MiniKits_Init(variptr_u *buffer, variptr_u *);

void CharacterMiniKits_Load(COLLECTION_s *collection, WORLDINFO *world, VARIPTR *buf, VARIPTR *buf_end);

extern f32 KITPOSX, PANEL_MINIKITY, PANEL_MINIKITSCALE;
void MiniKit_GameMsg_Update(GAMEMESSAGE_s *message);
void MiniKit_GameMsg_End(GAMEMESSAGE_s *message);

void CollectMinikit(nuvec_s *position, char *name, i32);

i32 AllMiniKitsDone(AREASAVE_s *save);

i32 MatrixReflection(NUMTX *, i32, f32, f32, NUMTX *);
void CharMiniKit_Draw(i32 id, numtx_s *matrix, i32 reflection_axis, float reflection_plane, float reflection_height);

extern i32 currentminikit, newminikitcount;
void DrawStatusMiniKit(f32, f32, f32, f32, f32, i32, STATUSPACKET_s *, f32);
void DrawMiniKitCount(f32, f32, i32, i32);



void CollectAllMiniKits(AREASAVE_s *save);



extern i32 LEGOOBJ_MINIKIT, LEGOOBJ_CHARKIT;
void EndChallenge(i32, i32);

void ResetMinikitCounter();

extern i16 id_SLAVE1, tALLMINIKITSBUILT;



extern NUCOLOUR3 panelcol[3], panelamb;



void CharacterMiniKits_Dump(WORLDINFO_s *world);



void EffectOffProgress_Reset(LEVEL_PROGRESS_s *progress);

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
    }
    // The reference's completion branch passes an uninitialized temporary.
    // Keep the message at the same valid averaged position as earlier counts.
    AddGameMsgCount(average, *counter, 10, 200, 100, 30, 0.75f);
}

void IncrementMinikitCounter(GameObject_s *);


// ==== src/legoapi/world/levels.cpp: declarations ====
#include "decomp.h"
#include "batman.h"
#include "globals.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"

// Level-system stubs that have not yet been split into a subsystem file.



AILOCATOR_s *getSpawnLocator(float clip_radius, char *name);




// ==== src/legoapi/ai/game/creature.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

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


void RemoveGameObject(GameObject_s *object, i32 immediate);

void ClearAICreatures();

void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress);


extern "C" {
    extern NUVEC plr_lastpos;
    AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);
    void AddToAIGroup(AIGROUP *group, APIOBJECT *object);
    void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
    void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet);
}

void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 context);
void Player_ResetContexts(PLAYERPACKET_s *packet);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
#include "legoapi/ai/game/creature_types.h"



void ResetAICreature(GameObject_s *object, AISYS_s *system);


i32 ObjHitObj_Flags(GameObject_s *);

extern "C" HOTHBATTLE_MELEE_s melee;
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance);

i32 SpawnMeleeCreatureType(i32 type);

extern GameObject_s * alert_obj;

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position);

// ==== src/legoapi/render/core/render.cpp: declarations ====
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
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);


void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();

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
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
extern i32 Paused;
extern "C" i32 screendump;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state

// Graphics loading flags

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).


// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"


void DrawCables();




extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}



i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];


void DrawParallax(nuhspecial_s *special);






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



extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);



void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);









void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();



i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();


void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);




void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
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
extern i32 currentminikit;

void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);







void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();


extern i32 dco_locatorposonly;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}


i32 DrawGameObjectsProcess();


void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


extern void DrawMenuEntry(MENU_s *, char *);






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









void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;


#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);







extern void (*BackDrop_AlphaFn)(float *);


void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);


void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"


// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern f32 animduration_blendouttime;
    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}



// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.





extern "C" {
extern i32 hub_character_ready;
}


#include "legoapi/characters/core/characters_types.h"











extern "C" {
    extern i16 id_R2Q5;
    void PlaySfx(char *, nuvec_s *);
    void PlaySfxAndSetVolume(char *, nuvec_s *, f32);
}










// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif










































// ==== src/legoapi/render/core/terrain.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/render/core/gameliball.h"

#include <stdio.h>
#include <new>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"

#include <limits.h>
#include <math.h>
#include <string.h>

struct tertype;
struct terrsitu_s;
struct PLATSKININFO;
struct GAMECAMERA_s;
struct pushblock_s;
extern i16 id_ATST;
extern i16 id_SPEEDERBIKE;
extern i32 TimingBarSet;
void TBOPENFN(char *, i32);
void TBCLOSEFN(char *, i32);
extern GameObject_s *CarWashHack;
void AddWaterSplash(GameObject_s *object, NUVEC *position);
i32 CannotKill(GameObject_s *object);
extern AREADATA_s *PODSPRINT_ADATA;
void NewRumble(nupad_s *pad, f32 strength, i32 mode);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 delay, i32 mode);
i32 NoLayerKill(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void PlayDieSfx(GameObject_s *object);
i32 StartBigJump(GameObject_s *, NUVEC *, i32, f32, f32, i32, i8);
extern "C" i16 id_SNAKE, id_TRACTOR;
extern f32 MiscTime;
void ApplyGravity(GameObject_s *object, f32 *gravity, f32 hover_height, f32 seek_rate, f32 *ground_height);
void InstantKillParts(GameObject_s *object, i32 mode, f32 delay);
extern "C" void NewTerrPlatformsOff();


namespace {
#include "legoapi/render/core/terrain_types.h"
    DECOMP_ASSERT(sizeof(TERRAIN_SCENE_OBJECT) == 0x50, "TERRAIN_SCENE_OBJECT ABI");

    DECOMP_ASSERT(sizeof(TERRAIN_DISPLAY_OBJECT) == 0xd0, "TERRAIN_DISPLAY_OBJECT ABI");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, transform) == 0xb8, "TERRAIN_DISPLAY_OBJECT transform offset");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, scene_object_index) == 0xc0,
                  "TERRAIN_DISPLAY_OBJECT scene_object_index offset");

    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_SCENE, object_count) == 0x6c, "TERRAIN_DISPLAY_SCENE object_count offset");

    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_count) == 0x1c, "TERRAIN_GSCENE object_count offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_map_present) == 0x58, "TERRAIN_GSCENE object_map_present offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, display_scene) == 0x110, "TERRAIN_GSCENE display_scene offset");

} // namespace

extern "C" void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);




void ScanWallSplineTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
void Tag_Check(GameObject_s *object);
void BigJumpCode(GameObject_s *object);
f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
extern "C" i32 NewShadowOnPlatform();
extern i32 LEGO_AIPATHCNX_BLOCKAGE;
extern i32 LEGO_AIPATHCNX_FULLTERRAIN;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;



namespace {







    // Retail scan callers contain the scale-and-normal reconstruction directly.
#define TERRAIN_SCALE_SCAN_SHAPE(candidate_arg, group_arg, writer_arg)                                                 \
    do {                                                                                                               \
        TERRAIN_SHAPE *&scale_candidate = (candidate_arg);                                                             \
        const TERRAIN_GROUP &scale_group = (group_arg);                                                                \
        TerrainScanWriter *scale_writer = (writer_arg);                                                                \
        if (TerI->object_scale != 1.0f) {                                                                              \
            TERRAIN_SHAPE *source = scale_candidate;                                                                   \
            scale_candidate = &ScaleTerrain[scale_writer->scaled_shape_count];                                         \
            scale_candidate->material[0] = source->material[0];                                                        \
            scale_candidate->material[1] = source->material[1];                                                        \
            scale_candidate->flags = source->flags;                                                                    \
            scale_candidate->normal_flags = source->normal_flags;                                                      \
                                                                                                                       \
            for (i32 vector_index = 0; vector_index < 3; ++vector_index) {                                             \
                scale_candidate->vectors[vector_index].x = source->vectors[vector_index].x;                            \
                scale_candidate->vectors[vector_index].z = source->vectors[vector_index].z;                            \
                scale_candidate->vectors[vector_index].y =                                                             \
                    (source->vectors[vector_index].y + scale_group.origin.y) * TerI->inverse_object_scale -            \
                    scale_group.origin.y;                                                                              \
            }                                                                                                          \
                                                                                                                       \
            if (source->normals[1].y < 65535.0f) {                                                                     \
                scale_candidate->vectors[3].x = source->vectors[3].x;                                                  \
                scale_candidate->vectors[3].z = source->vectors[3].z;                                                  \
                scale_candidate->vectors[3].y =                                                                        \
                    (source->vectors[3].y + scale_group.origin.y) * TerI->inverse_object_scale - scale_group.origin.y; \
                                                                                                                       \
                const f32 length = NuFsqrt(source->normals[1].x * source->normals[1].x +                               \
                                           source->normals[1].y * source->normals[1].y * TerI->object_scale_sq +       \
                                           source->normals[1].z * source->normals[1].z);                               \
                const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                      \
                scale_candidate->normals[1].x = source->normals[1].x * inverse_length;                                 \
                scale_candidate->normals[1].y = source->normals[1].y * TerI->object_scale * inverse_length;            \
                scale_candidate->normals[1].z = source->normals[1].z * inverse_length;                                 \
            } else {                                                                                                   \
                scale_candidate->normals[1].y = 65536.0f;                                                              \
            }                                                                                                          \
                                                                                                                       \
            const f32 length = NuFsqrt(source->normals[0].x * source->normals[0].x +                                   \
                                       source->normals[0].y * source->normals[0].y * TerI->object_scale_sq +           \
                                       source->normals[0].z * source->normals[0].z);                                   \
            const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                          \
            scale_candidate->normals[0].x = source->normals[0].x * inverse_length;                                     \
            scale_candidate->normals[0].y = source->normals[0].y * TerI->object_scale * inverse_length;                \
            scale_candidate->normals[0].z = source->normals[0].z * inverse_length;                                     \
            ++scale_writer->scaled_shape_count;                                                                        \
        }                                                                                                              \
    } while (0)


    // Preserve the retail arithmetic ordering and the sweep-diagonal service
    // call, even in the wall-only caller where its result is unused.
#define TERRAIN_GET_SCAN_BOUNDS(bounds_arg, radius_arg, wall_arg)                                                      \
    do {                                                                                                               \
        TerrainScanBounds &scan_bounds = (bounds_arg);                                                                 \
        f32 &bounds_radius = (radius_arg);                                                                             \
        const TerrainQuery_s &scan_query = *TerI;                                                                      \
        if (scan_query.scan_result != 1) {                                                                             \
            const f32 move_length = NuFsqrt(scan_query.movement.x * scan_query.movement.x +                            \
                                            scan_query.movement.y * scan_query.movement.y +                            \
                                            scan_query.movement.z * scan_query.movement.z);                            \
            const TerrainQuery_s &sphere_query = *TerI;                                                                \
            const f32 reach = (wall_arg) ? 0.1f + sphere_query.collision_radius + move_length                          \
                                         : move_length + 0.1f + sphere_query.collision_radius;                         \
            const f32 vertical = reach * sphere_query.object_scale;                                                    \
            scan_bounds.max_x = sphere_query.position.x + 0.05f + reach;                                               \
            scan_bounds.max_z = sphere_query.position.z + 0.05f + reach;                                               \
            scan_bounds.min_x = sphere_query.position.x - 0.05f - reach;                                               \
            scan_bounds.min_z = sphere_query.position.z - 0.05f - reach;                                               \
            scan_bounds.min_y = sphere_query.position.y - 0.05f - vertical;                                            \
            scan_bounds.max_y = sphere_query.position.y + 0.05f + vertical;                                            \
            bounds_radius = reach;                                                                                     \
        } else {                                                                                                       \
            const f32 radius = scan_query.collision_radius;                                                            \
            if (!(scan_query.movement.x > 0.0f)) {                                                                     \
                scan_bounds.min_x = scan_query.position.x + scan_query.movement.x - 0.02f - radius;                    \
                scan_bounds.max_x = scan_query.position.x + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_x = scan_query.position.x - 0.02f - radius;                                            \
                scan_bounds.max_x = scan_query.position.x + scan_query.movement.x + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.y > 0.0f)) {                                                                     \
                scan_bounds.min_y = scan_query.position.y + scan_query.movement.y - 0.02f - radius;                    \
                scan_bounds.max_y = scan_query.position.y + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_y = scan_query.position.y - 0.02f - radius;                                            \
                scan_bounds.max_y = scan_query.position.y + scan_query.movement.y + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.z > 0.0f)) {                                                                     \
                scan_bounds.min_z = scan_query.position.z + scan_query.movement.z - 0.02f - radius;                    \
                scan_bounds.max_z = scan_query.position.z + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_z = scan_query.position.z - 0.02f - radius;                                            \
                scan_bounds.max_z = scan_query.position.z + scan_query.movement.z + 0.02f + radius;                    \
            }                                                                                                          \
            const f32 dx = scan_bounds.min_x - scan_bounds.max_x;                                                      \
            const f32 dy = scan_bounds.min_y - scan_bounds.max_y;                                                      \
            const f32 dz = scan_bounds.min_z - scan_bounds.max_z;                                                      \
            bounds_radius = NuFsqrt(dx * dx + dy * dy + dz * dz);                                                      \
        }                                                                                                              \
        if (!(wall_arg) && scan_query.object_scale > 1.0f)                                                             \
            bounds_radius += (scan_query.object_scale - 1.0f) * scan_query.collision_radius;                           \
    } while (0)

    // These wall-spline traversals are part of all three retail scan bodies.
#define TERRAIN_COLLECT_WALL_SPLINES(bounds_arg, mask_arg, clear_arg, reset_empty_arg)                                 \
    do {                                                                                                               \
        TerrainScanBounds wall_bounds = (bounds_arg);                                                                  \
        const i32 wall_mask = (mask_arg);                                                                              \
        const bool wall_clear_override = (clear_arg);                                                                  \
        wall_bounds.min_x -= 0.02f;                                                                                    \
        wall_bounds.min_z -= 0.02f;                                                                                    \
        wall_bounds.max_x += 0.02f;                                                                                    \
        wall_bounds.max_z += 0.02f;                                                                                    \
        if ((reset_empty_arg) || (CurTerr->spatial_nodes != NULL && WallSplCount < 64))                                \
            WallSplCount = 0;                                                                                          \
        for (TERRAIN_SPATIAL_NODE *node = CurTerr->spatial_nodes; node != NULL;                                        \
             node = *reinterpret_cast<TERRAIN_SPATIAL_NODE **>(reinterpret_cast<u8 *>(node) - sizeof(void *))) {       \
            const u8 mask = node->field_0x02 >> 8;                                                                     \
            if (mask != 0 && (mask & wall_mask) == 0)                                                                  \
                continue;                                                                                              \
            for (i32 first = 0; first < node->point_count; first += 16) {                                              \
                NUVEC *points = node->points + first;                                                                  \
                i32 end = node->point_count;                                                                           \
                if (points[0].y != 2147483648.0f) {                                                                    \
                    if (!(points[1].y >= wall_bounds.min_x && wall_bounds.max_x >= points[0].y &&                      \
                          points[3].y >= wall_bounds.min_z && wall_bounds.max_z >= points[2].y))                       \
                        continue;                                                                                      \
                    end = MIN(end, first + 16);                                                                        \
                }                                                                                                      \
                for (i32 i = first; i < end; ++i) {                                                                    \
                    NUVEC &a = node->points[i];                                                                        \
                    NUVEC &b = node->points[i + 1];                                                                    \
                    if (!((a.x >= wall_bounds.min_x && wall_bounds.max_x >= b.x) ||                                    \
                          (b.x >= wall_bounds.min_x && wall_bounds.max_x >= a.x)))                                     \
                        continue;                                                                                      \
                    if (!((a.z >= wall_bounds.min_z && wall_bounds.max_z >= b.z) ||                                    \
                          (b.z >= wall_bounds.min_z && wall_bounds.max_z >= a.z)))                                     \
                        continue;                                                                                      \
                    if (WallSplCount < 64) {                                                                           \
                        WallSplList[WallSplCount].position = a;                                                        \
                        WallSplList[WallSplCount + 1].position = b;                                                    \
                        for (i32 p = 0; p < 2; ++p) {                                                                  \
                            u8 *material = WallSplList[WallSplCount + p].material;                                     \
                            material[0] = static_cast<u8>(node->field_0x02);                                           \
                            material[1] = mask;                                                                        \
                            material[2] = 0;                                                                           \
                            material[3] = 0;                                                                           \
                        }                                                                                              \
                        WallSplCount += 2;                                                                             \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        if (wall_clear_override)                                                                                       \
            TerrOverRideScan = NULL;                                                                                   \
    } while (0)

} // namespace


// Original 400-pointer table at 0x61e860: 147 named effects, then empty slots.
extern char * debris_name[400];
extern "C" {
extern i32 Grass_Available;
}

extern "C" void TerrainSetImpactData(void *impact_data, i32 *impact_count, i32 maximum_impacts);



// Constructs the fixed terrain header and its variable-sized group, index and
// platform arrays in the caller's arena. The four bounds arrays and the cell
// assignment array are temporary storage carved backwards from buf_end.
#define IS_SNAN(x) (*(u32 *)&(x) == 0xffc00000)



extern "C" void *TerrainInitEx(i32 level_num, void *buf, void *buf_end, i32 options, char *path, void *gscn,
                               i32 scene_id, u32 max_group_indices, u32 max_groups, u32 max_platforms);
void LoadTerrainFile(WORLDINFO *world);
void LoadGrassFile(WORLDINFO *world);
void LoadBridgeFile(WORLDINFO *world);
i32 CheckCylinder(i32 first_vertex, i32 second_vertex, i32 *vertex_mask, i32 remaining_vertex_mask);

i32 CheckSphere(i32 vertex_index);

i32 CheckSphereTer(NUVEC *position, f32 radius);

i32 HitPoly(f32 primary_start, f32 primary_end, f32 secondary_start, f32 secondary_end, tertype *surface);


NUVEC TerrainSkin(PLATSKININFO *info, nuvec_s *position, float weight, i32 mode);
void DerotateMovementVector();

void RotateVec(NUVEC *source, NUVEC *destination);


void DeRotateTerrain(tertype *surface);

void DeRotatePoint(nuvec_s *point);

namespace {

    DECOMP_ASSERT(sizeof(TERRAIN_IMPACT_RECORD) == 0x1c, "TERRAIN_IMPACT_RECORD ABI");


} // namespace


// Retail only keeps the IPA-SRA clone, which takes &normal->y.
#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif
static TERRAIN_KILL_PLAYER_ATTR i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y)
    __asm__("_ZL17TerrainKillPlayerP12GameObject_siP7nuvec_s.isra.4");
static i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y);



void MakePlayPlanes(GAMECAMERA_s *camera);

DECOMP_ASSERT(sizeof(TERRAIN_DEBUG_RECORD) == 100, "Terrain debug record ABI");





void ScanTerrainHandel(i32 scan_type, i16 *handle);

extern "C" i32 NewRayCastPlatForm(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                  i32 platform_index, i32 terrain_mask);

extern "C" void *NuScratchAlloc32(i32 size);
extern "C" void NuScratchRelease();





void ScanWallSplineTerrain(i32, i32 terrain_mask, i32);



void NewTerrStoreAnyInfo();



i32 TerrainPlatformMoveCheck(nuvec_s *, nuvec_s *, i32, i32, i32);




extern "C" void FullDeflect(NUVEC *normal, NUVEC *movement, NUVEC *result);

void FullDeflectSmallY(NUVEC *normal, NUVEC *movement, NUVEC *result);



extern "C" void FullReflect(NUVEC *normal, NUVEC *movement, NUVEC *result);



extern "C" void TerrainSetPlatConnectTol(f32 tolerance);

extern "C" void TerrainSetWallDeflectYScale(f32 scale);




i32 TerrainPlatformMoveCheck(nuvec_s *position, nuvec_s *normal, i32 platform_index, i32 resolve_impact,
                             i32 keep_embedded_disabled);
NUVEC TerCrossProduct(NUVEC *a, NUVEC *b);





extern "C" void terrainpickupinit(char *path, void **buffer);

extern "C" void noterraininit(void);


void DrawWallSpline(float height);

namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.





} // namespace

void NewScan(nuvec_s *position, i32 terrain_mask, i32 scan_platforms);



namespace {




} // namespace




extern "C" i32 EShadowInfo(void);


extern "C" i32 EShadowRoofInfo(void);

extern "C" void NewTerrPlatformsOff(void);

extern "C" i32 NewShadowOnPlatform(void);









extern "C" void CubeImpact(NUMTX *current, NUMTX *previous, NUVEC *normal, f32 scale, NUVEC *impact);



extern "C" void NewTerrainScaleYMask(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                                     f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags,
                                     i32 terrain_mask);

extern "C" f32 NewRayCastGetTOFI(void);

extern "C" i32 NewRayCastGetImpactTerrainType(void);

extern "C" void NewRayCastGetImpactNormal(NUVEC *normal);

extern "C" f32 NewRayCastGetEmbedDist(void);

extern "C" i32 NewRayCastHitWallSpline(void);

extern "C" i32 NewRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

extern "C" i32 NewRayCastScaleYMask(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags,
                                    u32 terrain_mask);


extern "C" i32 NewRayCastMask(NUVEC *position, NUVEC *movement, f32 radius, i32 terrain_mask, i32 scan_flags);

extern "C" i32 NewRayCastSet(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon, f32 compare_epsilon,
                             i32 scan_type, i32 scan_flags);

extern "C" i32 NewRayCastSetMask(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                 f32 compare_epsilon, i32 scan_type, i32 terrain_mask, i32 scan_flags);


extern "C" i16 NewPlatPickupInst(void *object, i32 type);




extern "C" i32 NewRayCastSetHandel(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                   f32 compare_epsilon, i16 *handle, i32 scan_type);

void ScanTerrainHandel(i32 terrain_mask, i16 *handle);


extern "C" i32 TerrainPlatId(void);


extern "C" i32 TerrainIntensityInfo();

extern "C" i32 TerrainInfo();

extern "C" i32 TerrainInfoExtra();

extern "C" i32 ShadowInfo(void);

extern "C" i32 ShadowIntensityInfo(void);

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.
extern TERRSET *CurTerr;
extern "C" void *NuScratchAlloc32(i32);

extern "C" void TerrainPlatformOldUpdate(void);



namespace {


} // namespace

i16 *NewScanHandelFull(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i32 terrain_mask);


i16 *NewScanHandelSubset(i16 *subset, nuvec_s *position, nuvec_s *movement, f32 radius, i32 terrain_mask);

extern "C" i16 *NewScanHandel(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i16 *subset);


namespace {



} // namespace


extern "C" void TerrainTrackFlush(void);

i32 HitTerrain();

extern "C" f32 NewShadowEx(NUVEC *position, i32, f32 height_above, f32 height_below, i32 terrain_mask);

i32 HitTerrPoly(tertype *surface, i32 group_index);

void PlatformConnect(char *track_id, NUVEC *position_delta, NUVEC *movement_delta, i32 platform_index);

void RayImpact(NUVEC *movement);

// ==== src/legoapi/characters/core/charconfig.cpp: declarations ====
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/level.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "globals.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C" {
    extern void *perm_debrissys;
    extern u16 SURFACEBITS_DUST;
}










































































































// Retail keeps only the IPA-SRA clones (taking the flags field directly),
// mangled with the original ANIMLIST_s parameter type.
#if defined(__i386__)
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone, regparm(2)))
// The u32 clone keeps retail's push-based prologue only when size-optimized.
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone, optimize("Os"), regparm(2)))
#else
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone))
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone))
#endif








































































































void CharVariants_Init(CHARVARIANT *variants, i32 count);

i32 LayerFromName(GAMECHARACTERDATA_s *character, char *name);

i32 CharVariant_Find(char *name);

i32 MakeLayerList_Name(CHARACTERMODEL_s *model, i16 *output, u32 mask);

void CharCategories_Init(CHARCATEGORY *categories);


i32 CharCategory_FindByName(char *name);

i32 CharCategory_IsCategory(GameObject_s *object, i32 index);

void Move_DEFAULT(GameObject_s *);

i32 RandomIDFromFlags(u32 model_flags, u32 game_flags, i32 require_hat, APICHARACTERMODELLIST_s *excluded,
                      i32 exclude_hidden);

extern CHARCONFIG_s charconfig;

// Retail only keeps the clone with strip_comments fixed to 1 (regparm text, filename).
#if defined(__i386__)
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone))
#endif


void CharConfig_ConfigureAll(i32 permanent, NUFPCOMJMP *game_keywords);

void CharConfig_CalculateJumpStats(float jump_speed, float gravity, float *duration, float *height);


// ==== src/legoapi/core/net/netplay.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/net/netplay.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"


extern APICHARACTERMODELLIST_s NetFreePlayModelList[49];
extern i8 net_FreePlayModelCount;
extern u8 net_recievedFreePlayList;

void GetClientMineInfo(nuvec_s **positions, u64 **present, u64 **exploded);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);
extern void ResetForceGlow(PLAYERPACKET_s *packet);

void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *);




// ==== src/legoapi/characters/core/players.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/world.h"

#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"

struct MechTouchUIElement;
struct PLAYERITEM_s;
struct PLAYERITEMTYPE_s;
struct PLAYERPACKET_s;
struct SOCKPOSITION_s;



extern f32 FindGunshipHoverHeight(GameObject_s *obj);
extern f32 GetVehicleSpeedMul(GameObject_s *obj, f32 input);
extern i32 ObjInNarrowSock(GameObject_s *obj, SOCKSYS *socks, i32 level);
extern i32 complexsockposition_forcesock;
extern i32 movegamecamera_forcesock;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *BATTLEOVERCORUSCANT_ADATA;
extern void ResetRumble(RUMBLEPACKET *packet);
extern void InitSurfaceInfo(GameObject_s *obj);
extern i32 SetObjOnSurface(GameObject_s *obj, i32 mode);
extern void GizForce_ResetLOS(GameObject_s *obj);
void Arcade_PlayerKilled(i32 player, i32 active);
void ResetPlayerAI(GameObject_s *obj);
void ResetPlayerMoves(GameObject_s *obj);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
f32 VehicleTurnOrLoopOffset(GameObject_s *object);
extern "C" f32 chattersfxwait;

void Players_Init(void);

// --- Helpers moved from world.cpp ---



void Players_InitPositions(WORLDINFO *world);


extern i8 BoltType_FindIDByName(char *name, WORLDINFO *world);

void PlayerItemTypes_Reset(WORLDINFO *world);

GameObject_s *AddCreature(i32 id, i32 param);


void PreResetCode(GameObject_s *obj);

void PostResetCode(GameObject_s *obj);

void SetHitPoints(GameObject_s *obj, i32 hp);

void RememberPlayerIDs(i32 a, i32 b, i32 c);

// ---- Player start spawn entries ----
extern PLAYERSTARTENTRY PlayerStart[8];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void *CutScenePlayer_Available(void);

void ChatterSfx(GameObject_s *g, i32 a, float b);

void DrawOffsetCode(GameObject_s *obj, i32 param);

float GetHoverPosY(GameObject_s *obj);

void PlayerItem_Set(PLAYERITEM_s *item, PLAYERITEMTYPE_s *type);

GameObject_s *Player_FindByID(i32 id);

NUVEC *Player_StartPos(GameObject_s *obj);

i32 PlayersDropInOut();

i32 PlayerItem_GotAmmo(PLAYERITEM_s *item);

i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position);

i32 Players_BothActive();

PLAYERITEMTYPE_s *PlayerItemType_Find(i32 id);

extern void (*Player_ClearContextFn)(GameObject_s *, i32);


i32 Player_HasFastBuild(GameObject_s *player);

void PlayerItemTypes_Init(PLAYERITEMTYPE_s *types);


void Player_CopyEssentials(GameObject_s *source, GameObject_s *destination);


extern i32 (*Player_ToggleSubCharacterFn)(GameObject_s *, i32, i32);
extern void (*Player_ToggledCharacterFn)(GameObject_s *, i32);
extern i32 LEGOHINT_FREEPLAYTOGGLE;
void Hint_SetComplete(i32 hint_id);
void Move_DEFAULT(GameObject_s *object);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

void Player_ToggleCharacter(GameObject_s *object, i32 direction, i32 sound);

i32 Player_HasInvincibility(GameObject_s *object);



i32 Player_HasDoubleWeaponDamage(GameObject_s *object);




extern u32 (*CanPushObstaclesFn)(GameObject_s *);

namespace {



} // namespace

void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface);

i32 CanPullLevers(i32 id);
extern f32 DEFAULT_MOVE_RANGE;

void SetFlicker(GameObject_s *object, float duration);

#include "legoapi/characters/core/players_types.h"



void ResetPlayerAI(GameObject_s *object);


i32 MakePlayerList(i32 count);

void ResetPlayerPacket(PLAYERPACKET_s *, CHARACTERDATA_s *);
void ResetCharacterIdle(GameObject_s *, i32, i32);
void SetGameObjectCharacterData(GameObject_s *);
void ResetCoinPacket(COINPACKET_s *);


void SetToLastSafePos(GameObject_s *object);

i32 AvailableToPlayer(u32 character_flags, i32 weapon_action, i32 context, i32 require_all);

i32 GetNumLocalPlayers();


i32 ActivePlayerInRange(nuvec_s *position, float range_squared, float *distance_squared);

GameObject_s *GetOtherActivePlayer(GameObject_s *object);


DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");


i32 Player_HasPurpleForce(GameObject_s *obj);

void FixUpLayers();

u32 AdjustLayerBits(u32 mask, GameObject_s *object);

void SetPlayerGroupPosition(f32 x, f32 y, f32 z);

bool FindNearestPlayerToVec(nuvec_s *position, GameObject_s **nearest_player, float &distance_squared,
                            bool require_character_flags, u32 character_flags);

extern i32 (*LastSafePosExtraFn)(GameObject_s *);
extern void (*GameObjectDimensionsExtraFn)(GameObject_s *);


bool FindFurthestPlayerFromVec(nuvec_s *position, GameObject_s **furthest_player, float &distance_squared,
                               bool require_character_flags, u32 character_flags);



// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/screens/arcade.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>

void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void GameObjectSetCanUse(GameObject_s *, void *, u8, u8, f32);
i32 Suit_GetIndex(SUIT_s *);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartUpdate_Basketball(PART_s *);
void PartImpact_Basketball(PART_s *);
void AddSlamDebris(GameObject_s *);
void BobaRocket_Kill(PART_s *, i32);
void BobaRocket_Move(PART_s *, f32);
void BobaRocket_Deflect(PART_s *);
void ConfigureComplexShadow(GameObject_s *);
f32 DropInOutScale(GameObject_s *);
extern "C" f32 sabrerubwait;
extern i32 lightning_type;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i16 id_MOUSEDROID;
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;


extern "C" {
    void AddToAIGroup(AIGROUP_s *group, APIOBJECT_s *object);
    extern NUVEC plr_lastpos;
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_BASKETCANNON;
    extern i16 id_R2Q5;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    extern i32 FalconDebKey[2];
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;

void legoSetMusicVolume(float);
void AnimatePlayer(GameObject_s *object);
void KeepOnScreen(GameObject_s *object);
void GetTopBot(GameObject_s *object);
void ResetRumble(RUMBLEPACKET *packet);
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);
void GameAISysSetGame();
extern GameObject_s *player_tag_from;
extern GameObject_s *player_tag_to;
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);
void ResetCoinPacket(COINPACKET_s *packet);



extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;


#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
































































extern "C" f32 NuAnimEndFrameOld(void *animation);

extern "C" {
extern i32 nbaddies_can_see_players;
}


























































































extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

extern f32 default_path_heighttol;
extern u8 default_activate_difficulty;
extern u8 default_min_n_respawns;
extern u8 default_max_n_respawns;
extern f32 default_min_t_respawn;
extern f32 default_max_t_respawn;

extern CHARACTERNAMEFN * LevelCharacterNameFn;
extern CHARACTERNAMEFN * SpecialRouteCharacterNameFn;
extern CHARACTERGLOBALIDFN * LevelCharacterGlobalIDFn;
extern GLOBALCHARACTERNAMEFN * GlobalCharacterNameFn;
extern CHARACTERHGOBJFN * GlobalCharacterHGobjFn;
extern CHARACTERRENDERFN * GlobalCharacterRenderFn;
extern CHARACTERGOALSPEEDFN * GetCharacterGoalSpeedFn;
extern CHARACTERTYPEIDFN * LevelCharacterTypeIDFn;

extern AICHARACTERTYPEID * GlobalCharacterTypeIDFn;
extern AISPECIALROUTECHARACTERTYPEID * SpecialRouteCharacterTypeIDFn;
extern AICHARACTERDISTANCE * GetViewRangeFn;
extern AICHARACTERDISTANCE * GetHearDistanceFn;
extern AICHARACTERDISTANCE * GetMaxViewHeightFn;
extern AICHARACTERDISTANCE * GetMinViewHeightFn;
extern GAMEAILOAD * GameAILoadFn;
extern AIACTIONPARSESPEED * AIActionParseSpeedFn;
extern AIBIGJUMPTODESTINATION * AIBigJumpToDestinationFn;
extern AIRESPAWNONPATH * AIRespawnOnPathFn;
extern AICLEARCREATURES * ClearAICreaturesFn;
extern APIOBJECTFROMOBJID * APIOBJECTFromObjIDFn;
extern AIFINDALTERNATIVESPECIALOBJECT * FindAlternativeSpecialObjectFn;
extern AIGETNAMEDAPIOBJECT * GetNamedAPIObjectFn;
extern AIGETCREATUREORIGIN * GetAICreatureOriginFn;
}


// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.






















extern "C" i32 SHADOWCALLS;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

extern "C" f32 MainRenderTargetTime;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

extern "C" i32 DoubleScore;
extern f32 LevelNameTime, LevelNameMul;
extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;

void GameTiming(WORLDINFO_s *world, float *game_time);

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

extern i32 active_neutral_count;
extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern AREADATA *VADER_ADATA;
extern GameObject_s *alert_obj;
extern i32 active_baddy_count;
void GameCreatureOpponentSelection(AISYS_s *, i32, APIOBJECT **, i32, APIOBJECT **, i32, APIOBJECT **, u64, f32);

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.



extern "C" {
void InitFn_LevelCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_SpecialRouteCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_LevelCharacterName(CHARACTERNAMEFN *function);

void InitFn_SpecialRouteCharacterName(CHARACTERNAMEFN *function);

void InitFn_LevelCharacterGlobalID(CHARACTERGLOBALIDFN *function);

void InitFn_GlobalCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_GlobalCharacterName(GLOBALCHARACTERNAMEFN *function);

void InitFn_GlobalCharacterRender(CHARACTERRENDERFN *function);

void InitFn_GlobalCharacterHGobj(CHARACTERHGOBJFN *function);

void InitFn_ClearAICreatures(AICLEARCREATURES *function);

void InitFn_GetCharacterGoalSpeedFn(CHARACTERGOALSPEEDFN *function);

void InitFn_GetViewRange(CHARACTERDISTANCEFN *function);

void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GameAILoad(GAMEAILOAD *function);

void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function);

void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function);

void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function);

void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function);

void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

extern f32 OFFSCREEN_CATCHUP_TIME;
extern f32 drop_back_in_timer;

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope);

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move);

void ClearSpecialMove(GameObject_s *object);

void GameAISysInit();



void GameAISysReset(AISYS_s *system);


void MakeBaddiesForgetAboutParty(i32 check_hostility);


i32 GameAudio_GetSfxId(i32 sfx);

void GameFog_Update(WORLDINFO_s *world);

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);


void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

void GameObjectOrigin(GameObject_s *object);

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene);

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();


void GameAnimSys_Update(GAMEANIMSYS_s *system);


f32 GetVehicleHoverHeight(GameObject_s *, f32 *);
u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours);

void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);


void GameObjectDimensions(GameObject_s *object);




i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;



void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

extern "C" {
extern i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);


extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);


void GameObjectStuffAfterAnimation();

void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message);




















// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}








extern "C" {
extern ed_module_s edTimingDesc;
}




































extern CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *);






GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

void BlowUpSnakeBody(GameObject_s *);
void PlayDieSfx(GameObject_s *);
void DieRumble(GameObject_s *);
void SetToLastSafePos(GameObject_s *);

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);



extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);



void ScaleGameObject(GameObject_s *object);

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);


extern i32 nethost;
extern i16 id_PKDROID;
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
extern GameObject_s * CarWashHack;
extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
extern "C" f32 tieonsfxwait, tieoffsfxwait;
void MovePlayer_NETWORK(GameObject_s *);
void UpdateRumble(RUMBLEPACKET *);
void Player_ToggleCharacter(GameObject_s *, i32, i32);
void AddSurfaceDebris(GameObject_s *);


GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);





void DeactivateGameObject(GameObject_s *object);


GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

void GameObjectToCameraDistances();

extern "C" {
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
}
void RemoveAnyChunkControls(i32 *chunk);

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void RemoveChunkControlFromStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);


void RemoveDebrisEffectFromStack(debkeydatatype_s *key);

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern i32 newgamecam;
i32 GetMenuID(void);
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance_out);
void Hint_CancelCurrent(void);
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
// Original data symbol testmovef is float0.005 at 0x617cb0.

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");


void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);



void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move);


void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);



extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern NUMTX cutscenecammtx, CutCamMtx;
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 PodRace_InStartCountdown(WORLDINFO_s *);
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
void MakePlayPlanes(GAMECAMERA_s *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);


void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();


NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);




i32 GoingForwardsAlongNarrowSock(GameObject_s *object);


extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/legoapi/characters/motion/move.cpp: declarations ====
#include "decomp.h"
#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"


#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

// The retail move.cpp initializer owns these four managed references.
extern MechWeakPtr<MechObjectInterface, 4> nextShootTarget;
extern MechWeakPtr<MechObjectInterface, 4> forceNextAttackOpponent;
extern MechWeakPtr<MechObjectInterface, 4> lungeTarget;

extern "C" i16 id_R2Q5;

// Original action/context lookup data, with its arrays and pointers in one owner.


extern CHARACTER_CONTEXT_INFO_s * CInfo;
extern ACTIONINFO_s * ActionInfo;
extern EXTRAACTIONDATA_s ExtraActionData[33];

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL;


i32 ForcePushed_SetTargetMom(GameObject_s *object, f32 *seek_rate);
void MoveInactiveVehicle(GameObject_s *object, i32 mode, GameObject_s **vehicle);
static void ShootThisFrame(GameObject_s *object, i32 bolt_id, i32 flags);
GIZMOBLOWUP_s *GizmoBlowUp_Target(GameObject_s *, NUVEC *, NUVEC *, f32, f32, i32, i32, i32);
i32 GizmoSys_SetBestBoltTarget(GIZMOSYS *, void *, GameObject_s *, NUVEC *, NUVEC *, f32, f32, i32, i32, i32);
void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height);
float GetVehicleHoverHeight(GameObject_s *object, float *separation_offset);
float FindGunshipHoverHeight(GameObject_s *object);
void DisorientateCode(GameObject_s *object, NUVEC *limit, float range);
float VehicleTurnOrLoopOffset(GameObject_s *object);
i32 Grapple_LookAtPos(GameObject_s *object, NUVEC *position);
NUVEC *Technos_TgtPos(TECHNO_s *techno);
void GameCam_UpdateShake(GAMECAMERA_s *camera, f32 ambient_amount);
u16 SeekRot(u16 current, u16 target, f32 rate);
void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate);
void FloatCode(GameObject_s *object);
void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 special_animation);
void Lever_MoveCode(WORLDINFO_s *world, GameObject_s *object);
extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
void SetWeaponIn(GameObject_s *object);
void Hint_SetComplete(i32);
extern i32 (*CanStartHoldFn)(GameObject_s *);
extern "C" f32 *AnimListFrameArray(CHARACTERMODEL_s *, i32);
i32 SuperWeirdo(GameObject_s *object);
extern i16 id_IMPERIALGUARD;
extern i16 id_GAMORREANGUARD;
extern i16 id_BAT;
PART_s *FindIncomingPart(void *, NUVEC *, f32, u32, f32);
void TakeHitRumble(GameObject_s *, f32);
extern i32 (*FindSlamOrigin_UseCPosFn)(GameObject_s *);
extern void (*Jump_EndOfLandContextFn)(GameObject_s *);
void ConstantRumble(GameObject_s *object, f32 strength, f32 duration);
extern "C" void PlaySfxAndSetVolume(char *name, NUVEC *position, f32 volume);
i32 GizForce_FindBestForceTarget(GIZFORCESYS_s *, GameObject_s *);
i32 GizForce_StoodOnForce(GIZFORCE_s *, GameObject_s *);
u16 GizForces_AngleToForce(NUVEC *, GIZFORCE_s *);
void SetHeadTarget(GameObject_s *, NUVEC *, i8, f32, f32, f32);
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
void GameCam_HitRoll(void);
void ResetForceGlow(PLAYERPACKET_s *);
extern f32 ForceThrowGravity;
extern f32 ForceThrowSpeed;
PART_s *Part_FindFromHSpecial(nuhspecial_s *);
PART_s *GizForce_Throw(GameObject_s *, GIZFORCE_s *, f32, f32, i32);
extern i32 ForcePush_SuperMindTrick;
extern i32 ForcePush_SuperPush;
extern i32 ForcePush_Waft;
i32 ZapTarget(GameObject_s *);
void Arcade_AIKilled(i32);
// Retail keeps several of this file's static helpers only as GCC clones; these
// declarations pin the clone names and register conventions.
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

static void ShootThisFrame(GameObject_s *, i32, i32);
f32 FindGunshipHoverHeight(GameObject_s *);
void Move_VEHICLE(GameObject_s *);
void CableCode(GameObject_s *, i32, f32);
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
extern "C" i16 id_BASKETCANNON;
// Retail keeps only the IPA-SRA clone of this function.
void JetPackCode(GameObject_s *, i32, i32, i32);
void ChatterSfx(GameObject_s *, i32, f32);
void BigJumpCode(GameObject_s *);
void InstantKillParts(GameObject_s *, i32, f32);
void AddFancyMessage(char *, f32, f32, f32, f32, i32, i32);
extern i16 id_BATTLEDROID, id_BUZZDROID, id_SUPERBATTLEDROID, id_PROBEDROID;
extern i16 id_NAFFDROID1, id_NAFFDROID2, id_NAFFDROID4, id_MOUSEDROID;
void Tag_Check(GameObject_s *);
i32 PodLevel(AREADATA_s *);
void KeepOnScreen(GameObject_s *);
void CableCode(GameObject_s *, i32, float);
void CatchUpCode(GameObject_s *, f32, f32, i32);
extern i16 id_SNAKE;
extern "C" i16 id_ATAT, id_DODGEM;
extern i16 id_YODA, id_YODAGHOST;
extern i16 id_C3PO, id_TC14;
extern i16 id_DROIDEKA, id_RANCOR;
extern "C" i16 id_ATST, id_MINIATST, id_ATST_LOWRES, id_MINIATAT, id_MINIATTE;
void SecurityDoor_MoveCode(WORLDINFO_s *, GameObject_s *);
bool FireBountyHunterRocket(GameObject_s *);
extern f32 DIEAIRSPEED;
extern f32 DIEAIRJUMPSPEED;
void KillRumble(GameObject_s *);
extern i16 id_BATTLEDROIDSECURITY, id_PKDROID, id_PITDROID;
extern i16 id_LUKESKYWALKERDAGOBAH;
extern f32 newgamecamtime;

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.
extern f32 CamStopBlend;
extern i32 netcamera;

extern "C" {
    extern i16 id_BODYGUARD;
    extern i16 id_GRIEVOUS;
}

#include "legoapi/characters/motion/move_types.h"





void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *);


void Move_BEAST(GameObject_s *object);

void Move_BARMAN(GameObject_s *object);

void Move_CANNON(GameObject_s *object);

void Move_WALKER(GameObject_s *object);


void Move_CRITTER(GameObject_s *object);


void Move_DRAGBOMB(GameObject_s *object);

void Move_DROIDEKA(GameObject_s *object);

i32 PodLevel(AREADATA_s *area);
extern LEVELDATA_s *PODRACEB_LDATA;
extern "C" {
    extern f32 pod_roll[2], pod_roll_target[2];
    extern i32 podrace_section;
}


void Move_CHARACTER(GameObject_s *object);

void Move_GEONOSIAN(GameObject_s *object);

void Move_HOVERDROID(GameObject_s *object);


extern f32 avg_currentspeed_mul;



extern f32 GetVehicleSpeedMul(GameObject_s *, f32);

void Move_SPEEDERBIKE(GameObject_s *object);




// Retail splits the guard off as an inline head; the rest is this partial clone.



void PeriscodeCode(GameObject_s *object);

void Move_DROIDGENERIC(GameObject_s *object);



void MoveSplinePosition(SPLINEPOS_s *position, f32 distance);

void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);

void MoveInactiveVehicle(GameObject_s *object, i32, GameObject_s **followed_object);

// Original exported level control, initialized to 2 at 0x623d00.
extern i32 GUNSHIPAHACK;


void Move_REPUBLICGUNSHIP(GameObject_s *object);

void Move_SUPERBATTLEDROID(GameObject_s *object);


#include <stdlib.h>
extern "C" i16 id_LANDSPEEDER, id_WOOKIEFLYER, id_STAP2;
extern GameObject_s *GetOtherActivePlayer(GameObject_s *);
extern i32 GoingForwardsAlongNarrowSock(GameObject_s *);
extern f32 PodSprint_InStartCountdown(WORLDINFO_s *);
extern void VehicleCollisionCode(GameObject_s *);

i32 NeedsPretendAnim(GameObject_s *object);


i32 GetShootDirection_LSW(GameObject_s *object, nuvec_s *direction);

void GetShootOrigin_LSW(GameObject_s *object, nuvec_s *position);



// These pod tuning values are zero-initialized in the original image.

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);

void Move_POD(GameObject_s *object);

extern void (*LEGO_SET_SLOWDOWNFN)(GameObject_s *);


void Move_VEHICLE(GameObject_s *object);

void Move_ATAT(GameObject_s *object);

void Move_JAWA(GameObject_s *object);



GameObject_s *ObjOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 allow_untargeted, i32 mode,
                          i32 player_filter);



// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)






// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.
static MOVE_CLONE_ATTR2 void ForceGlowCode(GameObject_s *object, i32 model)
    __asm__("_ZL13ForceGlowCodeP12GameObject_si.part.34");


void CableCode(GameObject_s *object, i32 special_pressed, f32);

i32 LightSabre_ColourFromObj(i32 model, i32 *glow_model);



i32 ForcePushed_SetTargetMom(GameObject_s *object, float *seek_rate);








// Retail splits the context test off as an inline head; the rest is this partial clone.





// Retail keeps only the IPA-SRA clone of this function.


// Retail keeps only the IPA-SRA clone of this function.


void Move_WEIRDO(GameObject_s *object);

void Move_JEDI(GameObject_s *object);

void MovePlayer_NETWORK(GameObject_s *object);










extern "C" {
    extern i16 id_ATST, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE;
}

i32 CanObjSlide(GameObject_s *object, i32);

i32 StartSlide(GameObject_s *object, i32 check_contact);




void StartJetPackFall(GameObject_s *object, i32 fall);
void PlayLandSfx(GameObject_s *object, i32 type, i32 surface);


void JetPackCode(GameObject_s *object, i32 jump_pressed, i32 fall, i32);

float SeekLinearF(float current, float target, float step);

void StartLaunch(GameObject_s *object);




i32 SetObjTarget(GameObject_s *object, GameObject_s *target);





void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);

void PlayLandSfx(GameObject_s *, i32, i32);
void AddWaterSplash(GameObject_s *, NUVEC *);



void UpdateLastSafePosition(GameObject_s *object);

void FindSlamOrigin(GameObject_s *object, NUVEC *start, NUVEC *end);

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect);






void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);

i32 Hang_SetTargetMom(GameObject_s *object);

i32 Glide_SetTargetMom(GameObject_s *object);

void SetObjAsHeadTarget(GameObject_s *object, GameObject_s *target, signed char, float, float, float);

i32 Slide_SetTargetMom(GameObject_s *object, u16 input_angle, float input_speed);




extern "C" i16 id_TIEFIGHTER, id_XWING;
void VehicleCollisionCode(GameObject_s *object);


i32 WallShuffle_SetTargetMom(GameObject_s *object, u16 input_angle);



u16 TurnRot(u16 current, u16 target, i32 speed, i32 *difference_out);


f32 SeekValF(f32 current, f32 target, f32 rate);


void FloatCode(GameObject_s *);




// Retail splits the context test off as an inline head; the rest is this partial clone.



extern i16 id_RANCOR;
extern i16 id_DEWBACK;
extern i16 id_BIGGUN;
extern i16 id_BANTHA;
extern i16 id_ATAT;
extern i16 id_CATAPULT;
extern i16 id_FIRETRUCK;
extern i32 TERRAINMASK_NONWEAPON;
void PushAway(NUVEC *, f32, NUVEC *, NUVEC *, GameObject_s *, GameObject_s *, f32, u32);

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");







i32 PlayerItem_GotAmmo(PLAYERITEM_s *);
extern "C" f32 animduration_blendouttime;
extern i16 id_GEONOSIAN, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE, id_SENTRYDROID;
void Move_CANNON(GameObject_s *);
void SetWeaponIn(GameObject_s *);



extern i16 id_EWOK;
extern i16 id_WICKET;
void StartJetPackFall(GameObject_s *, i32);


extern "C" PART_s *FindPart(NUVEC *, i32, GameObject_s *);




void ResetForceBack();


void DrawForceBackEffect(nuhspecial_s *special);

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 GetDefaultIdle(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.
f32 PodSprint_InStartCountdown(WORLDINFO_s *world);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, NUVEC *position, f32 volume);

extern i16 id_BODYGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;
extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;

#include "legoapi/characters/motion/gameanim_types.h"






void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object);


void Animate_JEDI(GameObject_s *object);



void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

extern i32 NeedsPretendAnim(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);


i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state);

i32 GameAnimSet_Stop(GAMEANIMSET_s *set);

void Animate_ASTROMECH(GameObject_s *object);

void Animate_CHARACTER(GameObject_s *object);

void Animate_GEONOSIAN(GameObject_s *object);

i32 GameAnimSet_Reset(GAMEANIMSET_s *set);

void Animate_HOVERDROID(GameObject_s *object);

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system);

void Animate_BATTLEDROID(GameObject_s *object);

void Animate_SPEEDERBIKE(GameObject_s *object);

i32 GameAnimSet_Playing(GAMEANIMSET_s *set);

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set);

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append);

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set);

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset);

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object);

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position);

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor);

void Animate_REPUBLICGUNSHIP(GameObject_s *object);

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set);

void Animate_SUPERBATTLEDROID(GameObject_s *object);


void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale);

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating);

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set);

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static);

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set);

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position);


void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility);

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix);

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix);

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set);

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix);

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set);

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity);

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set);

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set);

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static);

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set);

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set);


extern "C" {

    i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                              i32 first_joint);
    i32 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                    i32 joint_count, i32 first_joint, NUVEC *root_translation);

void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32);

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation);

extern NUJOINTPROCANIMFN JointProcAnimFn;

void SetProceduralAnimationFn(void *function);

i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame);

i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);

void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);

StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta);

} // extern "C"






i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation);

void EvalAnim2(nuhspecial_s *special, float frame);



void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index);

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index);

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end);

// ==== src/legoapi/world/levels/episodeIII.cpp: declarations ====
#include <string.h>
#include <stdio.h>
#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/chris.h"
#include "legoapi/render/fx.h"


extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}
#include "legoapi/render/core/render.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/nutex.h"

extern i16 id_WOOKIEE;


#include "legoapi/world/levels/episodeIII_types.h"
DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");




DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");

extern "C" {
extern CRUISERCNETPACKET_s * crusiserc_netpacket;
}

DECOMP_ASSERT(sizeof(CRUISERDNETPACKET_s) == 12, "Cruiser D packet size");

extern "C" {
extern CRUISERDNETPACKET_s * cruiserd_netpacket;
    extern i32 CruiserD_LiftChase;
}

// Episode 3 level handlers, in the game's Episode_III progression:
// dogfight / cruiser / grievous / kashyyyk / temple / vader / a-new-hope.

// ===========================================================================
// Dogfight (Dogfight_A)
// ===========================================================================

extern AREADATA *DOGFIGHT_ADATA;

extern speedup_s DogFightSpeedList[9];

// ===========================================================================
// Cruiser (Cruiser_A / Cruiser_C / Cruiser_D)
// ===========================================================================

void CruiserAInit(WORLDINFO_s *world);

void CruiserAUpdate(WORLDINFO_s *);

void CruiserCReset(WORLDINFO_s *);

void CruiserCPanel(WORLDINFO_s *);

void CruiserCUpdate(WORLDINFO_s *);

void CruiserDInit(WORLDINFO_s *world);

void CruiserDUpdate(WORLDINFO_s *);

void CruiserDReset(WORLDINFO_s *);

// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================

void GrievousA_Init(WORLDINFO_s *world);

void GrievousA_Reset(WORLDINFO_s *);

void GrievousA_Update(WORLDINFO_s *world);

// ===========================================================================
// Kashyyyk (Kashyyyk_A / Kashyyyk_B / Kashyyyk_C / Kashyyyk_D)
// ===========================================================================

void KashyyykA_Init(WORLDINFO_s *world);

void KashyyykB_Init(WORLDINFO_s *);

void KashyyykC_Init(WORLDINFO_s *world);

void KashyyykD_Init(WORLDINFO_s *);

void KashyyykA_Panel(WORLDINFO_s *);

void KashyyykA_Reset(WORLDINFO_s *);

void KashyyykB_Reset(WORLDINFO_s *);

void KashyyykD_Reset(WORLDINFO_s *world);

i32 AnakinGreenSabre(GameObject_s *obj);

void KashyyykA_Update(WORLDINFO_s *);

void KashyyykB_Update(WORLDINFO_s *);

void KashyyykC_Update(WORLDINFO_s *);

void KashyyykD_Update(WORLDINFO_s *);

// ===========================================================================
// Temple (Temple_A / Temple_C)
// ===========================================================================

void TempleA_Init(WORLDINFO_s *world);

void TempleC_Init(WORLDINFO_s *world);

void TempleC_AlwaysUpdate(WORLDINFO_s *);

// ===========================================================================
// Vader (Vader_A / Vader_B / Vader_C)
// ===========================================================================




void GameCameraMakeMiniCut2(NUVEC *, NUVEC *, i32, f32, f32, f32, f32, i32, i32, i32);
void TickTockSfx();
int LoseCoins(GameObject_s *, i32);

void VaderA_Init(WORLDINFO_s *world);

void VaderB_Init(WORLDINFO_s *);


void VaderC_Init(WORLDINFO_s *world);

void VaderA_Reset(WORLDINFO_s *world);

void VaderB_Reset(WORLDINFO_s *);

void VaderC_Reset(WORLDINFO_s *);

void VaderA_Update(WORLDINFO_s *);

void VaderB_Update(WORLDINFO_s *);

void VaderC_Update(WORLDINFO_s *world);

void VaderA_DrawPanel(WORLDINFO_s *);

void VaderB_DrawPanel(WORLDINFO_s *);

void VaderC_DrawPanel(WORLDINFO_s *);



// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================

void ANewHopeA_Init(WORLDINFO_s *world);

// ==== src/legoapi/render/light/shadow.cpp: declarations ====
#include "legoapi/render/light/shadow.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"




f32 BlobShadowFade(NUVEC *position, f32 fade_start, f32 fade_end, f32 alpha);


void Shadow_SetMode();

void InitShadowLights();

void RenderShadowLights(i32);

void ConfigureComplexShadow(GameObject_s *object);


void EnableShadowMapRendering(i32);



// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/numusic/numusic.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/nupad.h"

#include <string.h>

extern "C" void BackupMenu(void);
extern i32 (*GamePads_IgnoreInputFn)(void);
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
extern GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.

extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_TOGGLERIGHT;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_R3;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.
extern f32 PadOldSpeed2[2];
extern f32 PadOldSpeed[2];
extern u16 PadOldAngle2[2];
extern u16 PadOldAngle[2];


void GamePads_Init();

f32 GamePad_Rotate(GameObject_s *object);

i32 GamePad_Waggle(GAMEPAD_s *pad);

GAMEPAD_s *GamePad_Allocate();



u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);


i32 GamePads_SkipMovie();

extern "C" {

bool TestForController(void);

} // extern "C"

void PadOutPause(i32 port, WORLDINFO_s *world);


void UpdateRumble(RUMBLEPACKET *packet);

void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

void TakeHitRumble(GameObject_s *object, float strength);

extern "C" f32 NuFsqrt(f32);
extern "C" f32 NuRandFloat();



void SpaceResetAudioPoint();

void SpaceRumbleProcess();

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32);

i32 ObjLookingWithLeftStick(GameObject_s *object);






i32 NoPad(i32 port, i32 require_game_input);

void NewBuzz(nupad_s *pad, float duration, i32);

i32 ReadPad(i32 port);

void ReadPads();

void NewRumble(nupad_s *pad, float strength, i32);

// ==== src/legoapi/misc/supportall.cpp: declarations ====
// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nupostparams.h"

void MakeBaddiesForgetAboutParty(i32);
void ResetPlayer(GameObject_s *, i32, nuvec_s *, i32);
void GameCameraMakeMiniCut(nugspline_s *, f32, f32, f32, f32, i32, i32);
void EffectOffProgress_Reset(LEVEL_PROGRESS_s *);
// The reference defines these with dynamic initialization in this unit.
extern MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"
extern void * theThingManager;

extern f32 LevelNameMul, LevelNameTime;
extern "C" {
    extern f32 chattersfxwait, tieonsfxwait, tieoffsfxwait;
    extern i32 party_under_cover, nbaddies_can_see_players;
    extern i32 FalconDebKey[2];
    extern "C++" f32 TargetDist_Near2, TargetDist_Mid2;
    extern "C++" u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
    extern i32 makebaddiesforgetinresetbits;
    extern i32 reset_reimport;
    extern u32 arcade_placed_stud_total;
}

void ClearLevelProgress(i32, WORLDINFO_s *);
void SetTexAnimSignals(void);
void ShoveObjectSysReset(void);
u32 TotalLevelCoinTally(WORLDINFO_s *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *);
extern i32 bonusmodearcade;
extern u32 arcade_placed_stud_total;
extern i32 last_chatter_sfx;
extern i32 FalconDebKey[2];
extern f32 LevelNameTime;
extern f32 LevelNameMul;
void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 progress_index);
void ResetForceBack(void);
void ClearAICreatures(void);
extern "C" void ResetParts(void);
extern "C" void NuSound3StopRumble(void);
extern "C" f32 tieonsfxwait;
extern "C" f32 tieoffsfxwait;



DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");




extern f32 TargetDist_Near2;
extern f32 TargetDist_Mid2;
extern u16 TargetDeg_Near;
extern u16 TargetDeg_Mid;
extern u16 TargetDeg_Far;
extern i32 party_under_cover;
extern i32 makebaddiesforgetinresetbits;
extern i32 nbaddies_can_see_players;
extern i32 reset_reimport;

void CatchUpCode(GameObject_s *object, f32 divisor, f32 maximum, i32 mode);




extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void BurnoutApply(i32 paused);


void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);

void AddSlamDebris(GameObject_s *object);



void CheckResetBits();




extern "C" i16 id_LANDSPEEDER;


void AddSurfaceDebris(GameObject_s *object);

extern "C" void DebFree(i32 *);
void DebFreeWithoutKey(debkeydatatype_s *key);


void DebrisKillPlayers();

i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material);

void DebrisProcessSpheres(uv1deb *data, float time, debinftype *effect, debkeydatatype_s *key, i32 finite);

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40
extern "C" {
extern NUFILE hfh;
extern char unknown[8];
}



extern "C" void NuHtmlWrite(const char *text, ...);

extern "C" void NuHtmlHeading1(const char *fmt, ...);








void AddChunkToRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void AddChunkControlToStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);


void AddDebrisEffectToStack(debkeydatatype_s *key);

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern f32 globaltime;
    extern f32 panelglobaltime;
}
void DebrisGetControlStackLock(void);
void DebrisReleaseControlStackLock(void);
void RemoveAnyChunkControls(i32 *);
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebFreeChunksInstantly(i32 *handle);

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i16 *freedebkeys;
    extern i32 freedebkeyptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern f32 globaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
}

extern "C" void DebReAlloc2(debkeydatatype_s *);
extern "C" void DebReAlloc(debkeydatatype_s *, i32);








extern "C" {
    extern i32 EDPP_MAX_TYPES;
    void edppDeleteEffect(i32);
}




extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern f32 globaltime;
    extern NUVEC debris_collide_pt;
}




void TBOPENFN(char *name, i32 type);

void RndrArrow(float x, float y, float scale, i32 angle, i32 colour);

void TBCLOSEFN(char *name, i32 type);

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/screens/arcade.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>



extern "C" {
    void AddToAIGroup(AIGROUP_s *group, APIOBJECT_s *object);
    extern NUVEC plr_lastpos;
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_BASKETCANNON;
    extern i16 id_R2Q5;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    extern i32 FalconDebKey[2];
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).







#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

































































extern "C" {
extern i32 nbaddies_can_see_players;
}


























































































extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

extern f32 default_path_heighttol;
extern u8 default_activate_difficulty;
extern u8 default_min_n_respawns;
extern u8 default_max_n_respawns;
extern f32 default_min_t_respawn;
extern f32 default_max_t_respawn;

extern CHARACTERNAMEFN * LevelCharacterNameFn;
extern CHARACTERNAMEFN * SpecialRouteCharacterNameFn;
extern CHARACTERGLOBALIDFN * LevelCharacterGlobalIDFn;
extern GLOBALCHARACTERNAMEFN * GlobalCharacterNameFn;
extern CHARACTERHGOBJFN * GlobalCharacterHGobjFn;
extern CHARACTERRENDERFN * GlobalCharacterRenderFn;
extern CHARACTERGOALSPEEDFN * GetCharacterGoalSpeedFn;
extern CHARACTERTYPEIDFN * LevelCharacterTypeIDFn;

extern AICHARACTERTYPEID * GlobalCharacterTypeIDFn;
extern AISPECIALROUTECHARACTERTYPEID * SpecialRouteCharacterTypeIDFn;
extern AICHARACTERDISTANCE * GetViewRangeFn;
extern AICHARACTERDISTANCE * GetHearDistanceFn;
extern AICHARACTERDISTANCE * GetMaxViewHeightFn;
extern AICHARACTERDISTANCE * GetMinViewHeightFn;
extern GAMEAILOAD * GameAILoadFn;
extern AIACTIONPARSESPEED * AIActionParseSpeedFn;
extern AIBIGJUMPTODESTINATION * AIBigJumpToDestinationFn;
extern AIRESPAWNONPATH * AIRespawnOnPathFn;
extern AICLEARCREATURES * ClearAICreaturesFn;
extern APIOBJECTFROMOBJID * APIOBJECTFromObjIDFn;
extern AIFINDALTERNATIVESPECIALOBJECT * FindAlternativeSpecialObjectFn;
extern AIGETNAMEDAPIOBJECT * GetNamedAPIObjectFn;
extern AIGETCREATUREORIGIN * GetAICreatureOriginFn;
}


// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.






























#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.



extern "C" {
void InitFn_LevelCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_SpecialRouteCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_LevelCharacterName(CHARACTERNAMEFN *function);

void InitFn_SpecialRouteCharacterName(CHARACTERNAMEFN *function);

void InitFn_LevelCharacterGlobalID(CHARACTERGLOBALIDFN *function);

void InitFn_GlobalCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_GlobalCharacterName(GLOBALCHARACTERNAMEFN *function);

void InitFn_GlobalCharacterRender(CHARACTERRENDERFN *function);

void InitFn_GlobalCharacterHGobj(CHARACTERHGOBJFN *function);

void InitFn_ClearAICreatures(AICLEARCREATURES *function);

void InitFn_GetCharacterGoalSpeedFn(CHARACTERGOALSPEEDFN *function);

void InitFn_GetViewRange(CHARACTERDISTANCEFN *function);

void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GameAILoad(GAMEAILOAD *function);

void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function);

void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function);

void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function);

void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function);

void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}










































extern "C" {
extern i32 party_under_cover;
}


































extern "C" {
extern ed_module_s edTimingDesc;
}




































































extern "C" {
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
}





// ==== src/legoapi/misc/memory.cpp: all remaining content ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nuptl_android.h"

#include <string.h>



void DebFreeChunksInstantly(i32 *);
void RemoveDebrisEffectFromStack(debkeydatatype_s *);

extern "C" {

    extern debkeydatatype_s *debkeydata;
    extern i16 *freedebkeys;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];

i32 DebAlloc(void);

    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern f32 globaltime, panelglobaltime;

void DebFree(i32 *handle);

void DebFreeAllChunksInstantly(void);

void DebFreeInstantly(i32 *handle);

void DebFreeOrphansInstantly(debinftype *effect);

    void DebReAlloc2(debkeydatatype_s *key);

void DebReAlloc(debkeydatatype_s *key, i32 particle_count);

    extern "C++" void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebReAlloc2(debkeydatatype_s *key);

} // extern "C"

// ==== functions in original address order ====

























// ==== functions in original address order ====




























































// ==== functions in original address order ====

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

        NUVEC velocity = {0.0f, 0.0f, static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 3.0f + 2.0f};
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

    if (special_count > 0) {
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
                } else if (NuStrIStr(name, "asteroid_c") != NULL) {
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
        GIZMOBLOWUP_s *blowup = &world->gizmo_blowups[blowup_index];
        char *name = blowup->name;
        i32 asteroid_type;
        if (name == NULL || NuStrIStr(name, "asteroid_a") != NULL || NuStrIStr(name, "asteroid_pop") != NULL) {
            asteroid_type = 0;
        } else if (NuStrIStr(name, "asteroid_mid") != NULL) {
            asteroid_type = 1;
        } else {
            continue;
        }

        asteroid->blowup = blowup;
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
            TROOPERCANNON_s &cannon = troopercannons[i];
            DeactivateCharacter(cannon.character_name);
            GizBuildIt_SetToStart(cannon.buildit, 0, 0);
            GIZMO *gizmo = GizmoFindByData(WORLD->gizmo_sys, gizbuildit_gizmotype_id, cannon.buildit);
            GizmoActivate(WORLD->gizmo_sys, gizmo, 1, 1);
            GizBuildit_SetVisibility(cannon.buildit, 1);
            cannon.rebuilding = 1;
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
        TROOPERCANNON_s &cannon = troopercannons[cannon_index];
        cannon = TROOPERCANNON_s();

        sprintf(name, "trooper_cannon%d", number);
        cannon.buildit = GizBuildIt_Find(world, name);
        if (cannon.buildit != NULL) {
            sprintf(name, "troopercannon_%d", number);
            cannon.object = GetNamedGameObject(world->ai_sys, name);
            if (cannon.object != NULL)
                NuStrCpy(cannon.character_name, name);
        }

        sprintf(name, "BASE%d", number);
        cannon.base = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, name);

        if (netclient != 0) {
            if (cannon.object != NULL)
                ++cannon_index;
            continue;
        }

        if (cannon.buildit != NULL && cannon.object != NULL) {
            if ((world->level_progress->destroyed_trooper_cannon_mask & (1u << cannon_index)) != 0) {
                GizBuildIt_SetToStart(cannon.buildit, 0, 0);
                cannon.rebuilding = 1;
            } else {
                if (cannon.base != NULL) {
                    GizmoSetVisibility(world->gizmo_sys, cannon.base, 1, 1);
                    GizObstacle_PlayBackwards(static_cast<GIZOBSTACLE_s *>(cannon.base->object));
                }
                GizBuildIt_Finish(cannon.buildit);
                GizBuildit_SetVisibility(cannon.buildit, 0);
                cannon.rebuilding = 0;
                ActivateCharacter(name, NULL, 0);
                AddGameDebris(world->debris_sys, 0x5c, &cannon.object->apiobj.collision_position);
                GameObject_s *trooper = AddDynamicCreature(trooper_id, &cannon.object->apiobj.collision_position,
                                                           cannon.object->apiobj.field_0x276, "CannonTrooper",
                                                           &cannon.object->ai.path_info, NULL, 0, NULL, NULL, 0, 0);
                if (trooper != NULL)
                    TakeOverGameObject(trooper, cannon.object, 0, 1);
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

        GameObject_s *callback_object = cannon.object->field_0xcc0;
        if (callback_object == NULL)
            callback_object = cannon.object;
        if (callback_object->field_0xeb4 == NULL)
            callback_object->field_0xeb4 = KilledTrooperCannon;
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
    if (__builtin_expect(group != NULL && (group->field_0x24 & 2) != 0, 0)) {
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
    trooper_boltid[1] = BoltType_FindIDByNameWide("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByNameWide("trooper_red", world);
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
    trooper_boltid[1] = BoltType_FindIDByNameWide("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByNameWide("trooper_red", world);
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
                if (index == (melee.waves[0].initial_count >> 1) + 1) {
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

i32 SpawnMeleeCreatureType(i32 type) {
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
                if (OnOrInsidePlane(&locator->position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 0.5f, NULL) ==
                        0 ||
                    OnOrInsidePlane(&locator->position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 0.5f, NULL) ==
                        0) {
                    if (OnOrInsidePlane(&locator->position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL, 2.5f,
                                        NULL) != 0 ||
                        OnOrInsidePlane(&locator->position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL, 2.5f,
                                        NULL) != 0)
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
        if (object == NULL)
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
    i32 probe_count = g_lowEndLevelBehaviour != 0 ? 2 : 5;
    i32 atst_count = g_lowEndLevelBehaviour != 0 ? 2 : 6;
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
        object->field_0xefb |= 0x10;
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
        object->field_0xefb |= 0x10;
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
            if (g_lowEndLevelBehaviour == 0) {
                sprintf(state, "CamCut_%d", melee.next_wave);
            } else {
                sprintf(state, "CamCutLow_%d", melee.next_wave);
            }
            if (AIScriptSetBaseScriptStateByName(&WORLD->processors[0].processor, state) != 0) {
                AIScriptProcess(WORLD->ai_sys, NULL, NULL, &WORLD->processors[0].processor, FRAMETIME);
            }
            melee.transition_phase = 0;
            melee.wave_delay = 0.0f;
            melee.initialize_wave = 1;
            pending = 1;
        } else if (melee.transition_phase > 0) {
            if (MiniCutCam == 0) {
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
        } else if (melee.transition_phase == 0 && MiniCutCam != 0) {
            melee.transition_phase = 1;
        }
    }

    i32 type;
    for (type = 0; type < melee.creature_count; ++type) {
        SpawnMeleeCreatureType(type);
    }
    HothBattle_ManageBackgroundCreatures();
    if (melee.current_wave == 3 && melee.waves[0].creatures[0] != NULL && melee.waves[0].creatures[1] != NULL) {
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
    if ((pending | MiniCutCam) == 0 && NOAICREATURES == 0 && melee.current_wave == melee.next_wave) {
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
    trooper_boltid[1] = BoltType_FindIDByNameWide("trooper_green", world);
    trooper_boltid[0] = BoltType_FindIDByNameWide("trooper_red", world);
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

