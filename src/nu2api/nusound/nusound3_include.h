#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct nupad_s;
struct nuvec_s;
f32 PS2VolumeToScalar(i32 volume);
void NuSound3CreateVoice(nuvec_s *pos, i32 index, f32 falloff_a, f32 falloff_b, i32 volume_left, i32 volume_right, f32 pitch, bool loop);
void NuSound3ExitThreads();
void NuSound3SampleLoadThread(void *arg);
extern "C" {
    i32 NuStreamInit(void);
    void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32);
    void NuSound3ClearLoopHold(nuvec_s *, i32);
    void SOUND_SFXRequest_Table(void);
}
