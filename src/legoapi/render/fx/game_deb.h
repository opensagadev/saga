#pragma once

#include "decomp.h"

struct PartHeader;
struct debinftype;
struct debkeydatatype_s;
struct uv1deb;
extern "C" void DebrisSetTimeIncrement(f32 increment);
extern "C" void DebrisStartOffsetEx(debkeydatatype_s *key, f32 offset);
extern "C" void DebrisSetSeed(i32 seed);
extern "C" u32 DebrisGetSeed(void);
extern "C" void DebrisSetThinningLevel(f32 level);
extern "C" void DebrisSetDetailLevel(i32 level);
extern "C" void DebrisSetForcedThinning(i32 forced);
void DebrisProcessGeneration(void);
void DebrisProcessTriggers(void);
void DebFreeWithoutKey(debkeydatatype_s *key);
void DebrisFreeOldestDmaDebTypeTable(void);
void DebrisProcessSpheres(uv1deb *data, f32 time, debinftype *effect, debkeydatatype_s *key, i32 finite);

extern "C" {
    extern PartHeader **DmaDebTypes;
    extern i32 EDPP_MAX_DMADEBTYPES;
    extern i32 freeDmaDebType;
    extern debinftype **debtab;
    extern i32 debris_render_group;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
    extern i32 debris_detail_level;
    extern i32 DebrisSuspendDrawObjectSwitch;
    extern i32 g_renderingDebris;
    void Debris(i32 update_panel_time);
    void DebrisDraw(i32 paused, i32 pass);
    i32 DebrisGlassParticlesActive(void);
    void DebrisGlassClose(void);
    void DebrisGlassInit(void);
    void DebrisDrawGlassEx(i32 flicker);
    void DebrisDrawGlass(void);
    void DebrisSetCutSceneMode(i32 enabled);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
}

// Declared here for the other units that use them.
struct GameObject_s;
struct numtl_s;
struct numtx_s;
struct nuvec_s;
union variptr_u;
void AddSlamDebris(GameObject_s *);
void AddSurfaceDebris(GameObject_s *);
void AddSurfaceRipples(GameObject_s *);
void AddWaterSplash(GameObject_s *, nuvec_s *);
void BuildRippleMtx(numtx_s *matrix, nuvec_s *normal, nuvec_s *position, u16, u16);
void InitRippleMtl(char *name, numtl_s **result, variptr_u *buffer, variptr_u *buffer_end);
