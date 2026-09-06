#pragma once

#include "nu2api/nucore/common.h"

#include "decomp.h"

class NuSoundEffect;

class NuSoundBus {
    NuSoundBus *intrusive_prev;
    NuSoundBus *intrusive_next;
    f32 output_mix[8];
    NuSoundBus *parent_bus;
    void *effect_begin_prev;
    void *effect_begin_next;
    void *effect_end_prev;
    void *effect_end_next;
    void *effect_begin;
    void *effect_end;
    i32 effect_count;
    char name[128];

  public:
    NuSoundBus(const char *name, bool is_master);
    NuSoundBus(const char *name, NuSoundBus *parent);
    ~NuSoundBus();

    const char *GetName();

    bool AddEffect(NuSoundEffect *);
    void RemoveEffect(NuSoundEffect *);
    void ApplyFinalMix(float *);
    void GetOutputMix(float *);
    void SetOutputMix(float);
    void SetOutputMix(float *);
    void SetOutputBus(NuSoundBus *);
};

DECOMP_ASSERT(sizeof(NuSoundBus) == 0xc8, "NuSoundBus size");
