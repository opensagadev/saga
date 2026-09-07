#pragma once

#include "nu2api/nusound/nulist.hpp"

class NuSoundEffect;

class NuSoundBus {
  public:
    NuSoundBus *previous;
    NuSoundBus *next;
    float output_mix[8];

  private:
    NuSoundBus *parent_bus;
    NuList<NuSoundEffect *> effects;
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
