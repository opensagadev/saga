#include "legoapi/world/world_shared.h"
#include "legoapi/audio/audio.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "decomp_assert.h"

#include <string.h>

struct SoundTable;

enum RepeatSfxState : u8 {
    REPEAT_SFX_INACTIVE = 0,
    REPEAT_SFX_INITIAL_DELAY = 1,
    REPEAT_SFX_PLAY = 2,
    REPEAT_SFX_INTERVAL = 3,
};

struct RepeatSfx {
    i16 sfx_id;
    RepeatSfxState state;
    i8 plays_remaining;
    f32 timer;
    f32 interval;
    nuvec_s *position;
};

DECOMP_ASSERT(sizeof(RepeatSfx) == 0x10, "RepeatSfx size");

static i32 repsfxcount;
static RepeatSfx repsfxtab[32];

extern "C" {
    u16 GlobalSfxBits[100];
}

i32 GroupBuffer_GetSample(i32 group_id, i32 sequential);
i32 GroupBuffer_GetNumInGroup(i32 group_id);
i32 GroupBuffer_GetSampleByIndex(i32 group_id, i32 sample_index);

bool HandleGroupLimit(i32 group_id) {
    i32 voice_count = 0;
    NuSoundVoice *oldest_voice = NULL;
    f32 oldest_position = -1.0f;

    i32 sample_count = GroupBuffer_GetNumInGroup(group_id);
    for (i32 i = 0; i < sample_count; i++) {
        i32 sfx_id = GroupBuffer_GetSampleByIndex(group_id, i);
        i32 sample_index = g_soundInfo[sfx_id].index;
        voice_count += NuSound3CountVoices(sample_index);

        f32 playback_position = 0.0f;
        NuSoundVoice *voice = NuSound3FindOldestVoice(sample_index, &playback_position);
        if (voice != NULL && oldest_position < playback_position) {
            oldest_position = playback_position;
            oldest_voice = voice;
        }
    }

    if (voice_count < g_NuSoundMaxVoicesPerSample) {
        return true;
    }

    i32 first_sfx = GroupBuffer_GetSampleByIndex(group_id, 0);
    if (g_soundInfo[first_sfx].field29_0x40 == 1) {
        NuSound3StopVoice(oldest_voice);
        return true;
    }
    return false;
}

extern "C" void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
void SetSfxBit_OnEx(i32);

i32 ActionFromQuiet(i32 idx) {
    static i16 ActionPairTab[14] = {-1};
    if (idx != -1) {
        i16 *pair = ActionPairTab;
        while (*pair != -1) {
            if (*pair == idx) {
                return pair[1];
            }
            pair += 14;
        }
    }
    return -1;
}
i32 AmbientFromQuiet(i32 idx) {
    static i16 AmbientPairTab[2] = {-1};
    if (idx != -1) {
        i16 *pair = AmbientPairTab;
        while (*pair != -1) {
            if (*pair == idx) {
                return pair[1];
            }
            pair += 2;
        }
    }
    return -1;
}

extern "C" void ResetSounds(void) {
    memcpy(SfxBits, GlobalSfxBits, sizeof(SfxBits));
}

void SetLevelSfxBits(WORLDINFO *world) {
    (void)world;
}
void ResetLevSfx(WORLDINFO *world) {
    for (i32 i = 0; i < 0x40; i++) {
        world->level_sfx[i].id = -1;
    }
    world->level_sfx_count = 0;
}

void InitSpecialSfx(WORLDINFO *world) {
    (void)world;
}
void LoadSpecialSfxFile(WORLDINFO *world) {
    (void)world;
}

i32 ActionMusicFn() {
    return {};
}

i32 CheckMusicOther() {
    return {};
}

extern "C" {

    f32 sfx_wait;

    void GetLogicalSfxCount(void) {
    }

    void GetSfxCount(void) {
    }

    i32 GetSfxIdN(char *, i32) {
        return -1;
    }

    void GetSfxName(void) {
    }

    i32 IsSfxLooping(i32) {
        return 0;
    }

    void PauseGameAudio(void) {
    }

    void PauseGameMusic(void) {
    }

    void PauseGameSfx(void) {
    }

    void PlayAltGameMusic(void) {
    }

    void PlayCutMusic(void) {
    }

    void PlayMusic(void) {
    }

    void PlaySfx(char *, struct nuvec_s *) {
    }

    void PlaySfxAndSetPitch(void) {
    }

    void PlaySfxAndSetVolume(char *name, nuvec_s *position, f32 volume) {
        i32 id = GetSfxId(name);
        if (id != -1) {
            PlaySfxByIdEx(id, position, volume, 1.0f);
        }
    }

    void PlaySfxAndSetVolumeAndPitch(void) {
    }

    void PlaySfxById(i32 sfx_id, nuvec_s *position) {
        PlaySfxByIdEx(sfx_id, position, 1.0f, 1.0f);
    }

    void PlaySfxByIdAndSetPitch(void) {
    }

    void PlaySfxByIdAndSetVolume(i32 sfx_id, nuvec_s *position, f32 volume) {
        PlaySfxByIdEx(sfx_id, position, volume, 1.0f);
    }

    void PlaySfxByIdAndSetVolumeAndPitch(void) {
    }

    void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch) {
        static u32 seed;
        static nuvec_s pos;

        if (sfx_id == -1) {
            return;
        }

        NUSOUNDINFO *sound = &g_soundInfo[sfx_id];
        if (sound->disabled != 0 || sound->comment != 0) {
            return;
        }

        i8 priority = sound->priority;
        if (sound->group != -1) {
            sfx_id = GroupBuffer_GetSample(sound->group, sound->seq);
            if (sfx_id == -1) {
                return;
            }
            sound = &g_soundInfo[sfx_id];
        }

        f32 pan = sound->pan;
        i32 sample_index = sound->index;
        bool loop = sound->loop != 0;
        f32 buzz_timer = sound->buzz_timer;
        i32 rumble_strength = sound->rumble_strength;
        f32 rumble_sustain = sound->rumble_sustain;
        f32 rumble_release = sound->rumble_release;

        f32 falloff_near = sound->falloff_near;
        f32 falloff_far = sound->falloff_far;
        f32 saved_fade_start = 0.0f;
        f32 saved_fade_end = 0.0f;
        bool custom_falloff = falloff_near != 0.0f || falloff_far != 0.0f;
        if (custom_falloff) {
            saved_fade_start = nusound_fade_start;
            saved_fade_end = nusound_fade_end;
            nusound_fade_start = falloff_near * saved_fade_start * 0.5f;
            nusound_fade_end = falloff_far * saved_fade_end / 15.0f;
        } else {
            falloff_near = 2.0f;
            falloff_far = 15.0f;
        }

        const NUMTX *listener = reinterpret_cast<const NUMTX *>(NuSound3GetListener());
        if (listener == NULL) {
            if (custom_falloff) {
                nusound_fade_start = saved_fade_start;
                nusound_fade_end = saved_fade_end;
            }
            return;
        }

        if (position != NULL) {
            f32 dx = position->x - listener->m30;
            f32 dy = position->y - listener->m31;
            f32 dz = position->z - listener->m32;
            f32 max_distance_squared = nusound_fade_end * nusound_fade_end;
            if (dx * dx + dy * dy + dz * dz > max_distance_squared) {
                if (custom_falloff) {
                    nusound_fade_start = saved_fade_start;
                    nusound_fade_end = saved_fade_end;
                }
                return;
            }
        }

        if (sound->group != -1 && !HandleGroupLimit(sound->group)) {
            if (custom_falloff) {
                nusound_fade_start = saved_fade_start;
                nusound_fade_end = saved_fade_end;
            }
            return;
        }

        f32 volume_scale;
        if (volume == 1.0f) {
            volume_scale = static_cast<f32>(sound->volume);
        } else {
            if (volume > 1.0f) {
                volume = 1.0f;
            }
            volume_scale = static_cast<f32>(sound->volume) * volume;
        }

        if (sound->nofade == 0) {
            volume_scale *= AUDIOFADELEVEL;
            volume_scale *= numusicGetDuckVolume();
        }
        i32 voice_volume = static_cast<i32>(volume_scale * MASTERVOLUME);

        if (sound->pitch_rnd != 0.0f) {
            f32 pitch_variation = NuRandFloatSeeded(&seed) * sound->pitch_rnd;
            if ((NuRandIntSeeded(&seed) & 1) == 0) {
                pitch_variation *= 0.5f;
                pitch *= 1.0f - pitch_variation;
            } else {
                pitch *= 1.0f + pitch_variation;
            }
        }

        if (sound->volume_rnd != 0.0f) {
            voice_volume = static_cast<i32>(static_cast<f32>(voice_volume) *
                                            (1.0f + NuRandFloatSeeded(&seed) * sound->volume_rnd));
        }

        if (static_cast<u32>(sample_index) <= 1599) {
            if (position != NULL) {
                if (loop) {
                    NuSound3Play3dLoopSfx(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume,
                                          pitch, buzz_timer, rumble_strength, rumble_sustain, rumble_release);
                } else if (priority == 0) {
                    NuSound3Play3d(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume, pitch,
                                   buzz_timer, rumble_strength, rumble_sustain, rumble_release);
                } else {
                    NuSound3Play3dPri(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume,
                                      pitch, buzz_timer, rumble_strength, rumble_sustain, rumble_release, priority);
                }
            } else {
                i32 volume_left = voice_volume;
                i32 volume_right = voice_volume;
                if (pan < 0.0f) {
                    volume_right = static_cast<i32>(static_cast<f32>(voice_volume) * (1.0f + pan));
                } else if (pan > 0.0f) {
                    volume_left = static_cast<i32>(static_cast<f32>(voice_volume) * (1.0f - pan));
                }

                if (loop) {
                    pos.x = listener->m30;
                    pos.y = listener->m31;
                    pos.z = listener->m32;
                    nuvec_s *forward = reinterpret_cast<nuvec_s *>(const_cast<f32 *>(&listener->m20));
                    NuVecAdd(&pos, &pos, forward);
                    NuSound3Play3dLoopSfx(&pos, sample_index, falloff_near, falloff_far, volume_left, volume_right,
                                          pitch, buzz_timer, rumble_strength, rumble_sustain, rumble_release);
                } else if (priority == 0) {
                    NuSound3Play(sample_index, volume_left, volume_right, pitch, buzz_timer, rumble_strength,
                                 rumble_sustain, rumble_release);
                } else {
                    NuSound3PlayPri(sample_index, volume_left, volume_right, pitch, buzz_timer, rumble_strength,
                                    rumble_sustain, rumble_release, priority);
                }
            }
        }

        if (custom_falloff) {
            nusound_fade_start = saved_fade_start;
            nusound_fade_end = saved_fade_end;
        }
    }

    void PlayingCutMusic(void) {
    }

    void PrepareAllSounds(void) {
    }

    void RegisterSounds(void) {
    }

    void ResetPreSeek(void) {
        if (Music.state == MUSIC_PLAYBACK_DUAL_STREAM) {
            Music.current_track = -1;
        } else {
            Music.queued_track = -1;
            Music.requested_track = -1;
            Music.pause_requested = false;
        }
        Music.track_data = NULL;
    }

    void RestoreGameMusic(void) {
    }

    void ResumeGameAudio(void) {
    }

    void SOUND_SFXRequest_Table(void) {
    }

    void SetAudioFadeLevel(void) {
    }

    void SetCutVolume(void) {
    }

    void SetLinkedCutSceneMusic(void) {
    }

    void SetMusicVolume(void) {
    }

    void SetPreSeekStartPoint(void) {
    }

    void SetSfxBitTab_Off(void) {
    }

    void SetSfxBitTab_On(void) {
    }

    void SetSfxBit_Off(void) {
    }

    void SetSfxBit_On(i32 sound) {
        if (sound >= 0)
            SetSfxBit_OnEx(g_soundInfo[sound].index);
    }

    void SfxBit(void) {
    }

    void SfxBitMaskTable(void) {
    }

    void SfxBitTab(void) {
    }

    void SfxBitsRestore(void) {
    }

    void SfxBitsSetAll(void) {
    }

    void SfxBitsStore(void) {
    }

    void StopAltGameMusic(void) {
    }

    void SwapMusic(void) {
    }

} // extern "C"

void PlayDieSfx(GameObject_s *) {
}

void PlayHurtSfx(GameObject_s *) {
}

void PlayJumpSfx(GameObject_s *, i32) {
}

void PlayLandSfx(GameObject_s *, i32, i32) {
}

void SfxBitTabEx(SoundTable const *, i32) {
}

void TickTockSfx() {
}

void AddFootSteps(GameObject_s *) {
}

void PlayGruntSfx(GameObject_s *) {
}

void PlaySabreSfx(char *, GameObject_s *, nuvec_s *, i32) {
}

void LevChatterSfx(char *, nuvec_s *) {
}

void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position) {
    if (play_count == 1 && initial_delay == 0.0f) {
        if (sfx_id != -1) {
            GameAudio_PlaySfxById(sfx_id, position, 0, 0);
        } else {
            PlaySfx(name, position);
        }
        return;
    }

    if (initial_delay > 0.0f) {
        repsfxtab[repsfxcount].state = REPEAT_SFX_INITIAL_DELAY;
    } else {
        repsfxtab[repsfxcount].state = REPEAT_SFX_PLAY;
    }

    if (sfx_id == -1)
        sfx_id = GetSfxId(name);

    repsfxtab[repsfxcount].sfx_id = static_cast<i16>(sfx_id);
    RepeatSfx &repeat = repsfxtab[repsfxcount];
    repsfxcount = (repsfxcount + 1) & 31;
    repeat.timer = initial_delay;
    repeat.plays_remaining = play_count;
    repeat.interval = interval;
    repeat.position = position;
}

void ResetRepeatSfx() {
    repsfxcount = 0;
    memset(repsfxtab, 0, sizeof(repsfxtab));
    repsfxtab[0].sfx_id = -1;
}

void SetSfxBit_OnEx(i32 sound) {
    if (static_cast<u32>(sound) < 1600) {
        SfxBits[sound >> 4] |= 1 << (sound & 15);
    }
}

void UpdateLevelSfx(WORLDINFO_s *, i32) {
}

void PlayFootStepSfx(GameObject_s *) {
}

void SetSfxBit_OffEx(i32) {
}

void UpdateRepeatSfx() {
    for (RepeatSfx &repeat : repsfxtab) {
        switch (repeat.state) {
            case REPEAT_SFX_INITIAL_DELAY:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            case REPEAT_SFX_PLAY:
                GameAudio_PlaySfxById(repeat.sfx_id, repeat.position, 0, 0);
                --repeat.plays_remaining;
                if (repeat.plays_remaining > 0) {
                    repeat.timer = repeat.interval;
                    repeat.state = REPEAT_SFX_INTERVAL;
                } else {
                    memset(&repeat, 0, sizeof(repeat));
                }
                break;

            case REPEAT_SFX_INTERVAL:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            default:
                break;
        }
    }
}

void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx || sfx_id == -1) {
        return;
    }
    for (i32 index = 0; index < *sfx_count; ++index) {
        if (sfx_ids[index] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void SetSfxBitTab_OnEx(SoundTable *, i32) {
}

void SetSfxBitTab_OffEx(SoundTable *, i32) {
}

void SfxCheckMusicOnOff(OPTIONSSAVE_s *) {
}

void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx_count) {
        return;
    }

    i32 sfx_id = GetSfxId(sfx_name);
    if (sfx_id == -1) {
        return;
    }

    for (i32 i = 0; i < *sfx_count; i++) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }

    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void AddLevelSfxGizmoSys(GIZMOSYS_s *gizmo_sys, void *world_info, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOSET *set = gizmo_sys->sets;
    GIZMOTYPE *type = gizmotypes->types;
    i32 i = 0;
    while (i < gizmotypes->count) {
        if (type->fns.add_level_sfx_fn != NULL) {
            type->fns.add_level_sfx_fn(world_info, set->unknown, sfx_ids, sfx_count, max_sfx_count);
        }
        ++i;
        ++set;
        ++type;
    }
}

void BlockSfx(GameObject_s *) {
}

void SfxBitEx(i32) {
}

void AddLevSfx(WORLDINFO_s *, nuvec_s *, char *, i32) {
}
