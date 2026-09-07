#pragma once

#include "globals.h"
#include "nu2api/nucore/common.h"

struct NuSoundStreamingSample;
class NuSoundVoice;
struct nuvec_s;
struct VuMtx;

typedef struct nusound_filename_info_s {
    const char *filename;
    void *field4_0x4;
    i32 index;
    i32 field3_0xc;
    u32 field1_0x4;
    u32 field5_0x14;
    struct NuSoundStreamingSample *sample;
    u32 field7_0x1c;
} NUSOUND_FILENAME_INFO;

typedef enum {
    NUSOUNDPLAYTOK_END = 1,
    NUSOUNDPLAYTOK_STEREOSTREAM = 2,
    NUSOUNDPLAYTOK_SAMPLE = 3,
    NUSOUNDPLAYTOK_VOL = 6,
    NUSOUNDPLAYTOK_PITCH = 9,
    NUSOUNDPLAYTOK_STARTOFFSET = 10,
    NUSOUNDPLAYTOK_LOOPTYPE = 11,
    NUSOUNDPLAYTOK_ONESHOT = 12,
} NUSOUNDPLAYTOK;

typedef enum {
    NUSOUND_STEREO_STREAM_INACTIVE = 0,
    NUSOUND_STEREO_STREAM_PLAYING = 1,
    NUSOUND_STEREO_STREAM_FINISHED = 2,
} NUSOUND_STEREO_STREAM_STATUS;

#ifdef __cplusplus
enum MusicPlaybackState : i16 {
    MUSIC_PLAYBACK_STOPPED = 0,
    MUSIC_PLAYBACK_ACTIVE = 4,
    // Both transition states keep the non-primary stereo stream alive while
    // a game cutscene temporarily owns the mixer.
    MUSIC_PLAYBACK_DUAL_STREAM = 11,
    MUSIC_PLAYBACK_DUAL_STREAM_PENDING = 13,
};

struct MusicPlayback {
    MusicPlaybackState state;
    i16 requested_track;
    i16 primary_stream;
    i16 current_track;
    i16 transition_frames;
    i16 queued_track;
    i16 secondary_stream;
    i16 resume_frames;
    bool pause_requested;
    bool restore_requested;
    bool field_0x12;
    bool field_0x13;
    f32 seek_offset;
    f32 transition;
    u16 update_delay;
    i16 resume_track;
    void *track_data;
};

DECOMP_ASSERT(sizeof(MusicPlayback) == 0x24, "MusicPlayback size");
#endif

#ifdef __cplusplus

NUSOUND_FILENAME_INFO *ConfigureMusic(char *file, VARIPTR *bufferStart, VARIPTR *bufferEnd);

extern "C" {
#endif
#ifdef __cplusplus
    extern MusicPlayback Music;
#endif
    extern i32 NUSOUND_STREAM_3;

    i32 NuSound3InitV(VARIPTR *bufferStart, VARIPTR bufferEnd, i32 zero1, i32 zero2);
    i32 NuSound3PlayStereoV(NUSOUNDPLAYTOK, ...);

    void NuSound3Init(i32 zero);
    i32 NuSound3InitEx(void);
    f32 NuSound3AmplitudeTodB(f32 amplitude);
    NuSoundVoice *NuSound3FindQuietestVoice(i32 sample_index, f32 *playback_position);
    void NuSound3Listener(VuMtx *matrix);
    void NuSound3SetSampleTable(NUSOUND_FILENAME_INFO *info, VARIPTR *buffer_start, VARIPTR buffer_end);
    void NuSound3SetRequestTable(u16 *request_bits, i32 short_count);
    void NuSound3SetLoopHoldTime(float t);
    bool NuSound3IsSampleLoaded(i32 sample_index);
    i32 NuSound3LoadingSfx(void);
    const VuMtx *NuSound3GetListener(void);
    void NuSound3StopVoice(NuSoundVoice *voice);
    void NuSound3StopRumble(void);
    i32 NuSound3CountVoices(i32 sample_index);
    NuSoundVoice *NuSound3FindOldestVoice(i32 sample_index, f32 *playback_position);
    void NuSound3Play(i32 sample_index, i32 volume_left, i32 volume_right, f32 pitch, f32 buzz_timer,
                      i32 rumble_strength, f32 rumble_sustain, f32 rumble_release);
    void NuSound3PlayPri(i32 sample_index, i32 volume_left, i32 volume_right, f32 pitch, f32 buzz_timer,
                         i32 rumble_strength, f32 rumble_sustain, f32 rumble_release, i32 priority);
    void NuSound3Play3d(nuvec_s *position, i32 sample_index, f32 falloff_near, f32 falloff_far, i32 volume_left,
                        i32 volume_right, f32 pitch, f32 buzz_timer, i32 rumble_strength, f32 rumble_sustain,
                        f32 rumble_release);
    void NuSound3Play3dPri(nuvec_s *position, i32 sample_index, f32 falloff_near, f32 falloff_far, i32 volume_left,
                           i32 volume_right, f32 pitch, f32 buzz_timer, i32 rumble_strength, f32 rumble_sustain,
                           f32 rumble_release, i32 priority);
    void NuSound3Play3dLoopSfx(nuvec_s *position, i32 sample_index, f32 falloff_near, f32 falloff_far, i32 volume_left,
                               i32 volume_right, f32 pitch);

    // Stereo-stream control used by the NuMusic player. Streams live in slots
    // 0/1 (one per music voice); both status queries use the enum above.
    void NuSound3Update(void);
    i32 NuSound3GetStereoStreamStatus(i32 stream_index);
    f32 NuSound3GetStreamPlaybackTime(i32 stream_index);

    void NuSound3StopStereoStream(i32 stream_index);
    void NuSound3PauseStereoStream(i32 stream_index);
    void NuSound3ResumeStereoStream(i32 stream_index);
    void NuSound3CancelCheckStereo(void);
    i32 NuSound3StreamKeyStatus(i32 stream_index);
    void NuSound3SetStereoStreamVolume(i32 stream_index, i32 volume);
    i32 NuSound3SetStreamVolume(i32 stream_index, i32 volume);
    void NuSound3StopSFX(void);
    void NuSound3SetSFXPitch(i32 pitch);
    f32 NuSound3dBToAmplitude(f32 db);
#ifdef __cplusplus
}
#endif
