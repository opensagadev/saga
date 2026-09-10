#include "decomp.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

typedef void (*SoundBitCallback)(i32 sound_id);

i32 GroupBuffer_GetNumInGroup(i32 group_id);
i32 GroupBuffer_GetSampleByIndex(i32 group_id, i32 sample_index);
extern "C" void MusicPreSeek(i32 track);
extern "C" void RestoreGameMusic(void);
extern "C" edanim_param_s AnimParams[64];

extern "C" {
    i32 g_NuSoundMaxVoicesPerSample = 3;
    f32 AUDIOFADELEVEL = 1.0f;
    f32 MASTERVOLUME = 1.0f;
    i32 gcutSoundMusVol = 30;
    i32 gcutSoundVol = 100;

    MusicPlayback Music = {
        MUSIC_PLAYBACK_STOPPED, -1, 0, -1, 0, -1, 0, 0, false, false, false, false, 0.0f, 0.0f, 0, -1, NULL,
    };

    void NuSound3FlushLoops(void);
    void NuSound3KillAllAudio(void);
    void SoundStopMusic(void);

    f32 GetSoundVolume(void) {
        return MASTERVOLUME;
    }
    void SetSoundBitsById(const i32 *sound_ids, SoundBitCallback set_bit) {
        while (true) {
            i32 sound_id = *sound_ids++;
            if (sound_id == -1) {
                break;
            }
            if (sound_id >= 0) {
                set_bit(sound_id);

                i32 group_id = g_soundInfo[sound_id].group;
                if (group_id != -1) {
                    i32 sample_count = GroupBuffer_GetNumInGroup(group_id);
                    for (i32 i = 0; i < sample_count; ++i) {
                        set_bit(GroupBuffer_GetSampleByIndex(group_id, i));
                    }
                }
            }
        }
    }
    void SetSoundBitsBySingleId(i32 sound_id, SoundBitCallback set_bit) {
        if (sound_id >= 0) {
            set_bit(sound_id);

            i32 group_id = g_soundInfo[sound_id].group;
            if (group_id != -1) {
                i32 sample_count = GroupBuffer_GetNumInGroup(group_id);
                for (i32 i = 0; i < sample_count; ++i) {
                    set_bit(GroupBuffer_GetSampleByIndex(group_id, i));
                }
            }
        }
    }
    void SetSoundVolume(f32 volume) {
        MASTERVOLUME = volume;
    }
    void SoundKillAll(void) {
        SoundStopMusic();
        NuSound3FlushLoops();
        NuSound3KillAllAudio();
    }
    void SoundStopMusic(void) {
        if (NOSOUND == 0 && NOMUSIC == 0) {
            NuSound3StopStereoStream(0);
            NuSound3StopStereoStream(1);

            Music.transition = 0.0f;
            Music.state = MUSIC_PLAYBACK_STOPPED;
            Music.requested_track = -1;
            Music.current_track = -1;
            Music.queued_track = -1;
            Music.primary_stream = 0;
            Music.transition_frames = 0;
            Music.pause_requested = false;
            Music.resume_frames = 0;
            Music.field_0x12 = false;
            Music.field_0x13 = false;
            Music.update_delay = 0;
            Music.restore_requested = false;
            Music.resume_track = -1;
        }
    }

    void edanimSoundDestroy(i32 parameter_index, i32 sound_index) {
        edanim_param_s &params = AnimParams[parameter_index];
        i32 final_count = params.sound_count - 1;
        while (sound_index < final_count) {
            params.sound_values[sound_index] = params.sound_values[sound_index + 1];
            params.sound_ids[sound_index] = params.sound_ids[sound_index + 1];
            params.sound_positions[sound_index][0] = params.sound_positions[sound_index + 1][0];
            params.sound_positions[sound_index][1] = params.sound_positions[sound_index + 1][1];
            params.sound_positions[sound_index][2] = params.sound_positions[sound_index + 1][2];
            strcpy(params.sound_names[sound_index], params.sound_names[sound_index + 1]);
            sound_index++;
            final_count = params.sound_count - 1;
        }
        params.sound_count = final_count;
    }
    void edbitsSoundPlay(NUVEC *, i32) {
    }
    void gcutSetSoundVol(i32 sound_volume, i32 music_volume) {
        gcutSoundVol = sound_volume;
        gcutSoundMusVol = music_volume;
    }
}
