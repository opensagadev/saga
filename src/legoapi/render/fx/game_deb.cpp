// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.


// ==== src/legoapi/misc/supportall.cpp: declarations ====
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

void Hint_SetHintFromId(i32, i32, i32);
void MakeBaddiesForgetAboutParty(i32);
void ResetRadios();
void AITriggerSetSysReset(AITRIGGERSETSYS_s *);
void AITriggerSysAutoSetUp(WORLDINFO_s *, AITRIGGERSETSYS_s *);
void ResetPlayer(GameObject_s *, i32, nuvec_s *, i32);
f32 GetVehicleAreaRememberSpeed();
void CharPlatforms_Reset(CHARPLATFORMSYS_s *);
void SetSoundFadeDist(WORLDINFO_s *, OPTIONSSAVE_s *);
void GameCameraMakeMiniCut(nugspline_s *, f32, f32, f32, f32, i32, i32);
void CutScene_StartAudio();
void oneAtOnce_SetNumAttackers(i32);
void EffectOffProgress_Reset(LEVEL_PROGRESS_s *);
// The reference defines these with dynamic initialization in this unit.
extern MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"
extern BIGBUFFER_s BigBuffer;
extern void * theThingManager;

extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern f32 LevelNameMul, LevelNameTime;
extern rtldata_s lev_rtldata;
extern "C" {
    extern f32 chattersfxwait, tieonsfxwait, tieoffsfxwait;
    extern i32 party_under_cover, nbaddies_can_see_players;
    extern i32 FalconDebKey[2];
    extern f32 TargetDist_Near2, TargetDist_Mid2;
    extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
    extern i32 makebaddiesforgetinresetbits;
    extern i32 reset_reimport;
    extern u32 arcade_placed_stud_total;
}

void CutScenes_Reset(WORLDINFO_s *);
void ClearLevelProgress(i32, WORLDINFO_s *);
void ResetScene(nugscn_s *, SCENEPROGRESS_s *);
void GizmoBlowupVisibilityOverrides(WORLDINFO_s *);
void SetTexAnimSignals(void);
void Customiser_SetUpCharacterData(CUSTOMISER *);
void Surfaces_Reset(void);
void Bolts_Reset(void);
void Batarangs_Reset(void);
void Detonators_Reset(void);
void ResetExplosions(void);
void ShoveObjectSysReset(void);
void ResetGameMessages(void);
void Tag_ResetTransfers(void);
void Tag_SetMode(i32 mode);
u32 TotalLevelCoinTally(WORLDINFO_s *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *);
extern i32 bonusmodearcade;
extern u32 arcade_placed_stud_total;
extern f32 DEFAULT_MOVE_RANGE;
extern f32 drop_back_in_timer;
extern i32 last_chatter_sfx;
extern i32 FalconDebKey[2];
extern f32 LevelNameTime;
extern f32 LevelNameMul;
void Hint_Reset(void);
void Hint_CancelCurrent(void);
void TrafficAnimSys_Reset(TRAFFICANIMSYS_s *);
void Pulses_Reset(PULSESYS_s *);
void ResetRepeatSfx(void);
void ResetRippleSet(ripple_set_s *);
void Grabber_Reset(WORLDINFO_s *);
void Faders_Reset(WORLDINFO_s *);
void InitGameMode(void);
void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 progress_index);
void ResetForceBack(void);
void ClearAICreatures(void);
void ReStoreStatusTakeOverObjectSys(i32 restore_progress);
void InitPlayerAI(GameObject_s *object);
extern GAMECAMERA_s *GameCam;
extern "C" ripple_set_s *ripples;
extern "C" void ResetParts(void);
extern "C" void NuSound3StopRumble(void);
extern "C" f32 chattersfxwait;
extern "C" f32 tieonsfxwait;
extern "C" f32 tieoffsfxwait;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;


DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

extern i32 app_tbgameset;
extern i32 app_tbplayerset;
extern i32 app_tbaiset;
extern i32 app_tbdrawset;



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

void RndrTexQuad(f32 x, f32 y, f32 width, f32 height, i32 colour, numtl_s *material, i32 angle);

i32 SuperWeirdo(GameObject_s *object);

void bgProcClose();

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void BurnoutApply(i32 paused);

void bgprocFreeze();

void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

void AddSlamDebris(GameObject_s *object);

void CloakMovement(GameObject_s *object);


void CheckResetBits();

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material);

void bgProcAbortAll();

void bgprocUnFreeze();

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern "C" i16 id_LANDSPEEDER;
void PodDust(WORLDINFO_s *, GameObject_s *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 bgprocIsFreezing();

void AddSurfaceDebris(GameObject_s *object);

extern "C" void DebFree(i32 *);
void DebFreeWithoutKey(debkeydatatype_s *key);

i32 CannotKill(GameObject_s *object);
extern "C" i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);
extern "C" i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);
extern "C" i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

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

void NuHtmlFlush(i32 force);

extern "C" void NuHtmlBegin(void *file);

extern "C" void NuHtmlWrite(const char *text, ...);

extern "C" void NuHtmlHeading1(const char *fmt, ...);

extern "C" void NuHtmlBanner(void);

extern "C" void NuHtmlHeading2(const char *fmt, ...);

extern "C" void NuHtmlHeading3(const char *fmt, ...);

extern "C" void NuHtmlEnd(void);

void NuHtmlTitle(char *title);

extern "C" void NuHtmlBitmap(char *filename, i32 width, i32 height, char *caption, char *source);

void NuHtmlGraphArray(char **strings);

void AddChunkToRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void AddChunkControlToStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);

extern "C" debkeydatatype_s *debris_keystack;

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

void DebrisProcessAllocation();

VARIPTR *DisplayListRenderBuffer();


void DebrisGetControlStackLock();



i32 SolveQuadratic(f32, f32, f32, f32 *, f32 *);
void DebrisProcessControlChunks(i32 panel_time);

extern "C" {
    extern i32 EDPP_MAX_TYPES;
    void edppDeleteEffect(i32);
}

void DebrisCleanUpDmaDebTypeTables();

void xxxNuDisplayListUpdateSpecial(nuhspecial_s *special);

void DebrisReleaseControlStackLock();

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern f32 globaltime;
    extern NUVEC debris_collide_pt;
}

i32 DebrisSingleCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void TBRESET();

void TBOPENFN(char *name, i32 type);

void RndrArrow(float x, float y, float scale, i32 angle, i32 colour);

void TBCLOSEFN(char *name, i32 type);

// ==== src/legoapi/items/fx/ripples.cpp: declarations ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "nu2api/numath/numtx.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/world.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/render/fx.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include "nu2api/nu3d/nutex.h"



void InitRipples(ripple_set_s **result, variptr_u *buf, variptr_u *, i32 count);

void InitRippleMtl(char *name, numtl_s **result, variptr_u *buffer, variptr_u *buffer_end);

void NewRumble(nupad_s *pad, f32 strength, i32 mode);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 mode);

void AddWaterSplash(GameObject_s *object, nuvec_s *position);

void FindAnglesXY(NUVEC *, u16 *, u16 *);

void BuildRippleMtx(numtx_s *matrix, nuvec_s *normal, nuvec_s *position, u16, u16);

void ResetRippleSet(ripple_set_s *set);

void UpdateRippleSet(ripple_set_s *set);

void VecRotateAxis(NUVEC *, u16, NUVEC *);
void AddRipple(ripple_set_s *, NUMTX *, float, float, float, float, RGBA, RGBA, i32, numtl_s *, NUVEC *);

void AddSurfaceRipples(GameObject_s *object);

extern "C" void NuMtlDestroy(numtl_s *);

void DestroyRippleMtls(WORLDINFO_s *world);

i32 LookupRippleEffectIndex(char *name);

void AddRipple(ripple_set_s *set, numtx_s *matrix, float size, float growth, float lifetime, float delay,
               RGBA start_color, RGBA end_color, i32 flags, numtl_s *material, nuvec_s *velocity);


void RippleEffects_Configure(WORLDINFO_s *world, char *config);

// ==== functions in original address order ====

void DebrisKillPlayers() {
    DebrisPreCheckCollisions(&GameCam->pos, 50.0f);
#define CHECK_DEBRIS_PLAYER(player_index)                                                                              \
    do {                                                                                                               \
        GameObject_s *player = Player[player_index];                                                                   \
        if (player == NULL || (player->apiobj.flags_high & 0x10) == 0 || player->apiobj.field_0x287 != 0 ||            \
            !(player->field_0x1024 <= 0.0f) || !(player->spawn_protection_timer <= 0.0f) ||                            \
            (player->field_0xefe & 0x40) != 0 || CannotKill(player) != 0 || Player_HasInvincibility(player) != 0 ||    \
            (player->apiobj.character_data->game_character->flags_090 & 0x04008000) != 0) {                            \
            break;                                                                                                     \
        }                                                                                                              \
        if (DebrisCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,            \
                                       player->collision_y_scale) != -1) {                                             \
            ObjHitObj(NULL, player, 1, 0, 0, 1);                                                                       \
        }                                                                                                              \
        if (DebrisTorusCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,       \
                                            player->collision_y_scale) != -1) {                                        \
            ObjHitObj(NULL, player, 1, 0, 0, 1);                                                                       \
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
    i32 layer = static_cast<i8>(object->apiobj.field_0x27f);
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
    if (height > 0.0f) {
        height_scale = 1.0f - (object->apiobj.collision_min.y - surface_y) * (1.0f / height);
        if (height_scale <= 0.0f)
            return;
        if (height_scale > 1.0f)
            height_scale = 1.0f;
    }
    f32 emission_rate = (speed / run_speed) * rate;
    if (emission_rate < minimum_rate)
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

