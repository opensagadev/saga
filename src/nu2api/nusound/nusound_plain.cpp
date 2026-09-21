#include "decomp.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nusound/nusound.h"

#include "nu2api/nusound/nusound_system.hpp"

extern "C" {

    // The Android original is eight NOPs followed by RET; the callers still
    // supply the full rumble request ABI.
    void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32) {
    }
    i32 NuSound3AddStream(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3AddStreamEx(void) {
        STUBBED();
        return 0;
    }
    f32 NuSound3AmplitudeTodB(f32 amplitude) {
        return NuSoundSystem::AmplitudeTodB(amplitude);
    }
    void NuSound3BeginWaitUpdate(void) {
        STUBBED();
    }
    void NuSound3CancelCheckStereo(void) {
        STUBBED();
    }
    i32 NuSound3CheckStream(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3CheckWaitUpdate(void) {
        STUBBED();
        return 0;
    }
    void NuSound3ClearLoopHold(NUVEC *, i32) {
    }
    void NuSound3Close(void) {
        STUBBED();
    }
    void NuSound3DisplayVoiceInfo(void) {
        STUBBED();
    }
    void NuSound3DrawMem(void) {
        STUBBED();
    }
    i32 NuSound3FClose(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FOpen(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FOpenSize(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FRead(void) {
        STUBBED();
        return 0;
    }
    i64 NuSound3FSeek(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FindFree(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FlushBG(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3FlushFG(void) {
        STUBBED();
        return 0;
    }
    void NuSound3FlushLoops(void) {
    }
    i32 NuSound3GetSize(void) {
        STUBBED();
        return 0;
    }
    void NuSound3GetStreamInfo(i32, NuSoundStreamInfo *) {
        STUBBED();
    }
    void NuSound3HoldOffMusic(void) {
        STUBBED();
    }
    i32 NuSound3InitEx(void) {
        NuSound3Init(0);
        return 1;
    }
    void NuSound3InitLoopInfo(void) {
        STUBBED();
    }
    void NuSound3InitThreadSafeHackyMess(void) {
        STUBBED();
    }
    void NuSound3KillAllAudio(void) {
    }
    void NuSound3KillAllAudioWait(void) {
        STUBBED();
    }
    void NuSound3KillAllAudioWaitEx(void) {
        STUBBED();
    }
    void NuSound3LoadAllSpotFX(void) {
        STUBBED();
    }
    i32 NuSound3LoadingSfx(void) {
        return 0;
    }
    void NuSound3PlayChan(void) {
        STUBBED();
    }
    void NuSound3PlayInterleavedStereo(void) {
        STUBBED();
    }
    i32 NuSound3PlayStream(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3ReadStream(void) {
        STUBBED();
        return 0;
    }
    void NuSound3RemoveStreamID(void) {
        STUBBED();
    }
    i32 NuSound3ReserveStream(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3RestoreStreamPitch(void) {
        STUBBED();
        return 0;
    }
    void NuSound3SetDPL(i32, i32) {
    }
    void NuSound3SetDat(void) {
        STUBBED();
    }
    void NuSound3SetDataIopBufferSize(void) {
        STUBBED();
    }
    void NuSound3SetDuckVol(void) {
        STUBBED();
    }
    void NuSound3SetLoadCallback(void) {
        STUBBED();
    }
    void NuSound3SetLoopHoldTime(float t) {
        (void)t;
    }
    void NuSound3SetMonoIopBufferSize(void) {
        STUBBED();
    }
    i32 NuSound3SetReverb(i32) {
        return 0;
    }
    void NuSound3SetReverbVol(void) {
        STUBBED();
    }
    void NuSound3SetRumblePads(nupad_s *, nupad_s *) {
    }
    void NuSound3SetSFXPitch(i32) {
    }
    void NuSound3SetSampleTableFromPakFile(void) {
        STUBBED();
    }
    void NuSound3SetStereoIopBufferSize(void) {
        STUBBED();
    }
    i32 NuSound3SetStreamPitch(void) {
        STUBBED();
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
        STUBBED();
        return 0;
    }
    i32 NuSound3StreamClose(void) {
        STUBBED();
        return 0;
    }
    i32 NuSound3StreamOpen(void) {
        STUBBED();
        return 0;
    }
    void NuSound3UpdateEx(void) {
        STUBBED();
    }
    i32 NuSound3UpdatePending(void) {
        STUBBED();
        return 0;
    }
    void NuSound3UpdateRumble(f32) {
    }
    void NuSound3UpdateV(void) {
        STUBBED();
    }
}
