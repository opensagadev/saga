#include "nu2api/nusound/nusound.h"

#include "nu2api/nusound/nusound_system.hpp"

extern "C" {

    // The Android original is eight NOPs followed by RET; the callers still
    // supply the full rumble request ABI.
    void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32) {
    }
    i32 NuSound3AddStream(void) {
        return 0;
    }
    i32 NuSound3AddStreamEx(void) {
        return 0;
    }
    f32 NuSound3AmplitudeTodB(f32 amplitude) {
        return NuSoundSystem::AmplitudeTodB(amplitude);
    }
    void NuSound3BeginWaitUpdate(void) {
    }
    void NuSound3CancelCheckStereo(void) {
    }
    i32 NuSound3CheckStream(void) {
        return 0;
    }
    i32 NuSound3CheckWaitUpdate(void) {
        return 0;
    }
    void NuSound3ClearLoopHold(void) {
    }
    void NuSound3Close(void) {
    }
    void NuSound3DisplayVoiceInfo(void) {
    }
    void NuSound3DrawMem(void) {
    }
    i32 NuSound3FClose(void) {
        return 0;
    }
    i32 NuSound3FOpen(void) {
        return 0;
    }
    i32 NuSound3FOpenSize(void) {
        return 0;
    }
    i32 NuSound3FRead(void) {
        return 0;
    }
    i64 NuSound3FSeek(void) {
        return 0;
    }
    i32 NuSound3FindFree(void) {
        return 0;
    }
    i32 NuSound3FlushBG(void) {
        return 0;
    }
    i32 NuSound3FlushFG(void) {
        return 0;
    }
    void NuSound3FlushLoops(void) {
    }
    i32 NuSound3GetSize(void) {
        return 0;
    }
    void NuSound3GetStreamInfo(void) {
    }
    void NuSound3HoldOffMusic(void) {
    }
    i32 NuSound3InitEx(void) {
        NuSound3Init(0);
        return 1;
    }
    void NuSound3InitLoopInfo(void) {
    }
    void NuSound3InitThreadSafeHackyMess(void) {
    }
    void NuSound3KillAllAudio(void) {
    }
    void NuSound3KillAllAudioWait(void) {
    }
    void NuSound3KillAllAudioWaitEx(void) {
    }
    void NuSound3LoadAllSpotFX(void) {
    }
    i32 NuSound3LoadingSfx(void) {
        return 0;
    }
    void NuSound3PlayChan(void) {
    }
    void NuSound3PlayInterleavedStereo(void) {
    }
    i32 NuSound3PlayStream(void) {
        return 0;
    }
    i32 NuSound3ReadStream(void) {
        return 0;
    }
    void NuSound3RemoveStreamID(void) {
    }
    i32 NuSound3ReserveStream(void) {
        return 0;
    }
    i32 NuSound3RestoreStreamPitch(void) {
        return 0;
    }
    void NuSound3SetDPL(i32, i32) {
    }
    void NuSound3SetDat(void) {
    }
    void NuSound3SetDataIopBufferSize(void) {
    }
    void NuSound3SetDuckVol(void) {
    }
    void NuSound3SetLoadCallback(void) {
    }
    void NuSound3SetLoopHoldTime(float t) {
        (void)t;
    }
    void NuSound3SetMonoIopBufferSize(void) {
    }
    i32 NuSound3SetReverb(i32) {
        return 0;
    }
    void NuSound3SetReverbVol(void) {
    }
    void NuSound3SetRumblePads(void *, void *) {
    }
    void NuSound3SetSFXPitch(i32) {
    }
    void NuSound3SetSampleTableFromPakFile(void) {
    }
    void NuSound3SetStereoIopBufferSize(void) {
    }
    i32 NuSound3SetStreamPitch(void) {
        return 0;
    }
    i32 NuSound3SetStreamVolume(i32 stream_index, i32 volume) {
        NuSound3SetStereoStreamVolume(stream_index, volume);
        return 1;
    }
    void NuSound3StopRumble(void) {
    }
    void NuSound3StopSFX(void) {
    }
    i32 NuSound3StopStream(void) {
        return 0;
    }
    i32 NuSound3StreamClose(void) {
        return 0;
    }
    i32 NuSound3StreamOpen(void) {
        return 0;
    }
    void NuSound3UpdateEx(void) {
    }
    i32 NuSound3UpdatePending(void) {
        return 0;
    }
    void NuSound3UpdateRumble(f32) {
    }
    void NuSound3UpdateV(void) {
    }
}
