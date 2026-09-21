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
    void DebrisDraw(i32 paused, i32 pass);
    i32 DebrisGlassParticlesActive(void);
    void DebrisGlassClose(void);
    void DebrisGlassInit(void);
    void DebrisDrawGlassEx(i32 flicker);
    void DebrisDrawGlass(void);
    void DebrisSetCutSceneMode(i32 enabled);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
}
