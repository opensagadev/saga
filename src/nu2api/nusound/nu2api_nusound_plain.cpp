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
extern "C" f32 MusicVolume __asm__("_ZL11MusicVolume") __attribute__((visibility("hidden")));
extern "C" edanim_param_s AnimParams[64];

struct SoundTrackData {
    u8 reserved_00[0x88];
    u8 flags_88;
};

extern "C" {

    u16 SfxBits[100];
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
    void MaskSounds(const u16 *mask) {
        for (i32 i = 0; i < 100; ++i) {
            SfxBits[i] &= mask[i];
        }
    }
    void PrepareSounds(const u16 *sounds) {
        for (i32 i = 0; i < 100; ++i) {
            SfxBits[i] |= sounds[i];
        }
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
    void SoundUpdate(float frame_time) {
        if (Music.update_delay != 0 || frame_time == 0.0f || static_cast<u16>(Music.state - 4) >= 10) {
            return;
        }

        switch (Music.state) {
            case MUSIC_PLAYBACK_ACTIVE:
                goto active_music;
            case 5:
            case 6:
            case 7:
            case 9:
                goto transition_music;
            case 8:
            case 10:
                if (NuSound3GetStereoStreamStatus(Music.primary_stream) != NUSOUND_STEREO_STREAM_FINISHED) {
                    Music.state = static_cast<MusicPlaybackState>(7);
                }
                return;
            case MUSIC_PLAYBACK_DUAL_STREAM:
            case 12:
            case MUSIC_PLAYBACK_DUAL_STREAM_PENDING:
                goto linked_music;
            default:
                return;
        }

    transition_music:
        if (Music.state == 6 || Music.state == 9) {
            frame_time += frame_time;
        } else {
            frame_time *= 0.2f;
        }
        Music.transition += frame_time;
        if (Music.transition >= 1.0f) {
            Music.transition = 1.0f;
            const i32 other_stream = 1 - Music.primary_stream;
            if (static_cast<u16>(Music.state - 5) <= 1) {
                NuSound3PauseStereoStream(other_stream);
            } else {
                NuSound3StopStereoStream(other_stream);
            }
            Music.state = MUSIC_PLAYBACK_ACTIVE;
            Music.transition_frames = 0;
        }

        if (Music.current_track != -1) {
            NuSound3SetStereoStreamVolume(Music.primary_stream,
                                          static_cast<i32>(static_cast<f32>(g_music[Music.current_track].index) *
                                                           Music.transition * MusicVolume));
        }
        if (Music.queued_track != -1) {
            NuSound3SetStereoStreamVolume(1 - Music.primary_stream,
                                          static_cast<i32>(static_cast<f32>(g_music[Music.queued_track].index) *
                                                           (1.0f - Music.transition) * MusicVolume));
        }
        return;

    active_music:
        if (NuSound3GetStereoStreamStatus(Music.secondary_stream) == NUSOUND_STEREO_STREAM_FINISHED) {
            return;
        }
        if (Music.transition_frames <= 0x7f) {
            ++Music.transition_frames;
            if (Music.transition_frames <= 0x40) {
                return;
            }
        }
        if (Music.pause_requested) {
            MusicPreSeek(Music.requested_track);
        }
        if (Music.transition_frames <= 0x18) {
            return;
        }
        if (Music.restore_requested) {
            RestoreGameMusic();
            Music.restore_requested = false;
        }
        return;

    linked_music:
        SoundTrackData *track_data = static_cast<SoundTrackData *>(Music.track_data);
        if (track_data != NULL && (track_data->flags_88 & 2) != 0) {
            Music.resume_frames = 0;
        } else {
            const i32 other_stream = 1 - Music.primary_stream;
            if (NuSound3GetStereoStreamStatus(other_stream) == NUSOUND_STEREO_STREAM_FINISHED) {
                Music.resume_frames = 0;
            } else {
                if (Music.resume_frames <= 0x3f) {
                    ++Music.resume_frames;
                    if (Music.resume_frames <= 8) {
                        goto update_secondary_stream;
                    }
                }
                if (Music.current_track != -1 && Music.state != MUSIC_PLAYBACK_DUAL_STREAM_PENDING &&
                    NuSound3GetStereoStreamStatus(Music.primary_stream) != NUSOUND_STEREO_STREAM_FINISHED &&
                    !Music.pause_requested) {
                    if (Music.state == 12) {
                        Music.state = MUSIC_PLAYBACK_STOPPED;
                        NuSound3StopStereoStream(other_stream);
                        NuSound3StopStereoStream(Music.primary_stream);
                    } else {
                        Music.state = MUSIC_PLAYBACK_ACTIVE;
                        NuSound3StopStereoStream(other_stream);
                        NuSound3ResumeStereoStream(Music.primary_stream);
                    }
                    Music.transition = 1.0f;
                    NuSound3SetStereoStreamVolume(
                        Music.primary_stream,
                        static_cast<i32>(static_cast<f32>(g_music[Music.current_track].index) * MusicVolume));
                    Music.transition_frames = 0;
                    Music.requested_track = -1;
                }
            }
        }

    update_secondary_stream:
        if (NuSound3GetStereoStreamStatus(Music.secondary_stream) == NUSOUND_STEREO_STREAM_FINISHED) {
            return;
        }
        if (Music.transition_frames <= 0x7f) {
            ++Music.transition_frames;
            if (Music.transition_frames <= 0x40) {
                return;
            }
        }
        if (Music.pause_requested) {
            MusicPreSeek(Music.requested_track);
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
    void edbitsSoundPlay(void) {
    }
    void gcutSetSoundVol(i32 sound_volume, i32 music_volume) {
        gcutSoundVol = sound_volume;
        gcutSoundMusVol = music_volume;
    }
}
