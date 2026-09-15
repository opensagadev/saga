#include "nu2api/numusic/sfx.h"

#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/numusic.h"

#include "gamelib/crc/crc.h"
#include "globals.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"

#include <cstring>

static float g_audioVersion;
static u32 seed = 0x20558057;

struct SoundGroup {
    i16 first_sample;
    i16 sample_count;
    i16 field_0x4;
    i16 field_0x6;
};

i32 g_numGroups;
SoundGroup g_groups[128];
i32 g_lenGroupBuffer;
i16 g_groupBuffer[512];

DECOMP_ASSERT(sizeof(SoundGroup) == 8, "SoundGroup size");

NUSOUNDINFO *g_soundInfo;
NUSOUNDINFO *g_revertSoundInfo;

i16 *g_soundMap;
nusound_filename_info_s *SfxInfo = NULL;

static i32 NumSfx = 0;
static i32 NumSfxInst = 0;
static u32 NumSfxNames = 0;

static char sfx_name[1600][32] = {0};
static char sfx_filename[1600][64];
static i32 sfx_refcount[1600] = {0};

static char cfgfile_name[256] = "Audio/audio.cfg";

void InitSoundInfo(i32 index) {
    NUSOUNDINFO *info = &g_soundInfo[index];
    info->index = -1;
    info->flag_bytes[1] &= 0x7f;
    info->priority = 0;
    info->volume = 0x3fff;
    info->pan = 0.0;
    info->group = -1;
    info->pitch_rnd = 0.0;
    info->rumble_strength = 0;
    info->sfx_name = sfx_name[NumSfxInst];
    u8 flags = info->flag_bytes[2];
    info->category = 0;
    flags &= 0x88;
    info->field29_0x40 = 0;
    info->volume_rnd = 0.0;
    info->flag_bytes[2] = flags;
    info->falloff_near = 0.0;
    info->falloff_far = 0.0;
    info->buzz_timer = 0.0;
    info->rumble_sustain = 0.0;
    info->rumble_release = 0.0;

    g_revertSoundInfo[index] = *info;
}

static void fnAudioAudio(nufpar_s *fpar) {
    g_audioVersion = NuFParGetFloat(fpar);
}

static void fnAudioSample(nufpar_s *fpar) {
    InitSoundInfo(NumSfxInst);

    NUSOUND_FILENAME_INFO *finfo = NULL;
    while (true) {
        if (NuFParGetWord(fpar) == 0) {
            NUSOUNDINFO *sinfo = &g_soundInfo[NumSfxInst];
            sinfo->dirty = 0;
            sinfo->revertable = 1;
            memmove(&g_revertSoundInfo[NumSfxInst], sinfo, sizeof(NUSOUNDINFO));
            NumSfxInst++;
            return;
        }

        if (NuStrICmp(fpar->word_buf, "disable") == 0) {
            g_soundInfo[NumSfxInst].disabled = 1;
        } else if (NuStrICmp(fpar->word_buf, "comment") == 0) {
            g_soundInfo[NumSfxInst].comment = 1;
        } else if (NuStrICmp(fpar->word_buf, "maxvoicesbehaviour") == 0) {
            NuFParGetWord(fpar);
            if (NuStrICmp(fpar->word_buf, "noplay") == 0) {
                g_soundInfo[NumSfxInst].field29_0x40 = 0;
            } else if (NuStrICmp(fpar->word_buf, "replace") == 0) {
                g_soundInfo[NumSfxInst].field29_0x40 = 1;
            }
        } else if (NuStrICmp(fpar->word_buf, "name") == 0) {
            NuFParGetWord(fpar);
            NuStrNCpy(sfx_name[NumSfxInst], fpar->word_buf, 0x20);

            const char *str = sfx_name[NumSfxInst];
            g_soundInfo[NumSfxInst].sfx_name = str;
            u32 hash = CRC_ProcessStringIgnoreCase(str);

            i16 *id = &g_soundMap[hash & 0xff];
            if (*id == -1) {
                *id = NumSfxInst;
                g_soundInfo[NumSfxInst].next = -1;
            } else {
                g_soundInfo[NumSfxInst].next = *id;
                *id = NumSfxInst;
            }
        } else if (NuStrICmp(fpar->word_buf, "fname") == 0) {
            NuFParGetWord(fpar);

            if (g_soundInfo[NumSfxInst].disabled == 0 && g_soundInfo[NumSfxInst].comment == 0) {
                i32 i = 0;
                for (; i < NumSfx; i++) {
                    if (NuStrICmp(SfxInfo[i].filename, fpar->word_buf) == 0) {
                        finfo = &SfxInfo[i];
                        g_soundInfo[NumSfxInst].index = i;
                        g_soundInfo[NumSfxInst].filename = finfo->filename;
                        sfx_refcount[i]++;
                        break;
                    }
                }
                if (i == NumSfx) {
                    const i32 filename_index = NumSfxNames;
                    finfo = &SfxInfo[i];
                    finfo->filename = sfx_filename[filename_index];
                    finfo->field4_0x4 = NULL;
                    finfo->index = i + 0x1000 - SFX_MUSIC_COUNT;
                    NumSfxNames = filename_index + 1;
                    NuStrNCpy(const_cast<char *>(finfo->filename), fpar->word_buf, 0x40);
                    const i32 sample_index = NumSfx;
                    sfx_refcount[sample_index] = 1;
                    g_soundInfo[NumSfxInst].filename = finfo->filename;
                    g_soundInfo[NumSfxInst].index = sample_index;
                    NumSfx = sample_index + 1;
                }
            } else {
                g_soundInfo[NumSfxInst].index = -1;
                i32 i = 0;
                for (; i < NumSfx; i++) {
                    if (NuStrICmp(SfxInfo[i].filename, fpar->word_buf) == 0) {
                        sfx_refcount[i]++;
                        g_soundInfo[NumSfxInst].filename = SfxInfo[i].filename;
                        break;
                    }
                }
                if (i == NumSfx) {
                    NuStrNCpy(sfx_filename[NumSfxNames], fpar->word_buf, 0x40);
                    g_soundInfo[NumSfxInst].filename = sfx_filename[NumSfxNames++];
                }
            }
        } else if (NuStrICmp(fpar->word_buf, "pitch") == 0) {
            g_soundInfo[NumSfxInst].pitch = NuFParGetInt(fpar);
        } else if (NuStrICmp(fpar->word_buf, "pan") == 0) {
            f32 pan = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].pan = CLAMP(pan, -1.0f, 1.0f);
        } else if (NuStrICmp(fpar->word_buf, "pri") == 0) {
            i32 priority = NuFParGetInt(fpar);
            g_soundInfo[NumSfxInst].priority = CLAMP(priority, -128, 127);
        } else if (NuStrICmp(fpar->word_buf, "loop") == 0) {
            g_soundInfo[NumSfxInst].loop = 1;
        } else if (NuStrICmp(fpar->word_buf, "pitch_rnd") == 0) {
            f32 pitch_rnd = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].pitch_rnd = CLAMP(pitch_rnd, 0.0f, 1.0f);
        } else if (NuStrICmp(fpar->word_buf, "volume") == 0) {
            i32 volume = NuFParGetInt(fpar);
            g_soundInfo[NumSfxInst].volume = CLAMP(volume, 0, 0x3fff);
        } else if (NuStrICmp(fpar->word_buf, "nofade") == 0) {
            g_soundInfo[NumSfxInst].nofade = 1;
        } else if (NuStrICmp(fpar->word_buf, "volume_rnd") == 0) {
            f32 volume_rnd = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].volume_rnd = CLAMP(volume_rnd, -1.0f, 0.0f);
        } else if (NuStrICmp(fpar->word_buf, "near") == 0) {
            f32 falloff_near = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].falloff_near = CLAMP(falloff_near, 0.0f, 250.0f);
        } else if (NuStrICmp(fpar->word_buf, "far") == 0) {
            f32 falloff_far = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].falloff_far = CLAMP(falloff_far, 0.0f, 250.0f);
        } else if (NuStrICmp(fpar->word_buf, "global") == 0) {
            g_soundInfo[NumSfxInst].global = 1;
        } else if (NuStrICmp(fpar->word_buf, "rumble") == 0) {
            f32 buzz_timer = NuFParGetFloat(fpar);
            i32 rumble_strength = NuFParGetInt(fpar);
            g_soundInfo[NumSfxInst].buzz_timer = CLAMP(buzz_timer, 0.0f, 5.0f);
            g_soundInfo[NumSfxInst].rumble_strength = CLAMP(rumble_strength, 0, 0xff);
            f32 rumble_sustain = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].rumble_sustain = CLAMP(rumble_sustain, 0.0f, 5.0f);
            f32 rumble_release = NuFParGetFloat(fpar);
            g_soundInfo[NumSfxInst].rumble_release = CLAMP(rumble_release, 0.0f, 5.0f);
        } else if (NuStrICmp(fpar->word_buf, "fcat") == 0) {
            i32 category = NuFParGetInt(fpar);
            g_soundInfo[NumSfxInst].category = CLAMP(category, 0, 0xffff);
        }

        if (finfo != NULL) {
            finfo->field7_0x1c = g_soundInfo[NumSfxInst].field29_0x40;
        }
    }
}

static void fnAudioGroup(nufpar_s *fpar) {
    i32 group_id = -1;
    i32 first = 1;

    while (NuFParGetWord(fpar) != 0) {
        i32 sfx_id = GetSfxId(fpar->word_buf);
        if (first) {
            if (sfx_id == -1) {
                return;
            }
            group_id = GroupBuffer_MakeGroup(sfx_id);
        } else {
            sfx_id = GetSfxId(fpar->word_buf);
            if (sfx_id != -1) {
                GroupBuffer_AddToGroup(group_id, sfx_id);
            }
        }
        first = 0;
    }
}

static NUFPCOMJMP audioCom[] = {
    {"Audio", fnAudioAudio},
    {"Sample", fnAudioSample},
    {"Group", fnAudioGroup},
    {NULL, NULL},
};

void LoadSfx(const char *file, variptr_u *buffer_start, variptr_u buffer_end);

void InitSfx(variptr_u *buffer_start, variptr_u buffer_end, const char *file) {
    // bVar15 = 0;
    // g_soundMap = (short *)((i32)buffer_start->voidptr + 3U & 0xfffffffc);
    usize allocation = ALIGN(buffer_start->addr, 4);
    g_soundMap = reinterpret_cast<i16 *>(allocation);

    // sfx_info = (nusound_filename_info_s *)(g_soundMap + 0x100);
    // SfxInfo = sfx_info;
    // buffer_start->voidptr = g_soundMap + 0x6500;
    // memset(sfx_info, 0, 0xc800);
    SfxInfo = reinterpret_cast<nusound_filename_info_s *>(allocation + 0x100 * sizeof(u16));
    buffer_start->addr = reinterpret_cast<usize>(SfxInfo + 1600);
    memset(SfxInfo, 0, 1600 * sizeof(nusound_filename_info_s));

    // sound_info = (SoundInfo *)((i32)buffer_start->voidptr + 3U & 0xfffffffc);
    // g_soundInfo = sound_info;
    // buffer_start->voidptr = sound_info + 0x640;
    // memset(sound_info, 0, 0x1a900);
    g_soundInfo = static_cast<NUSOUNDINFO *>(BUFFER_ALLOC(buffer_start, 1600 * sizeof(NUSOUNDINFO), 4));
    memset(g_soundInfo, 0, 1600 * sizeof(NUSOUNDINFO));

    //__s = (void *)((i32)buffer_start->voidptr + 3U & 0xfffffffc);
    // g_revertSoundInfo = __s;
    // buffer_start->voidptr = (void *)((i32)__s + 0x1a900);
    // memset(__s, 0, 0x1a900);
    g_revertSoundInfo = static_cast<NUSOUNDINFO *>(BUFFER_ALLOC(buffer_start, 1600 * sizeof(NUSOUNDINFO), 4));
    memset(g_revertSoundInfo, 0, 1600 * sizeof(NUSOUNDINFO));

    CRC_Init(buffer_start);

    // psVar5 = g_soundMap;
    // uVar10 = -(((u32)g_soundMap & 0xf) >> 1) & 7;
    memset(g_soundMap, -1, 0x100 * sizeof(i16));

    NumSfx = 0;
    NumSfxInst = 0;

    for (i32 i = 0; i < SFX_MUSIC_COUNT; i++) {
        SfxInfo[i].filename = g_music[i].filename;
        SfxInfo[i].field4_0x4 = g_music[i].field4_0x4;
        SfxInfo[i].index = g_music[i].index;
        SfxInfo[i].field3_0xc = g_music[i].field3_0xc;
        SfxInfo[i].field1_0x4 = g_music[i].field1_0x4;
        SfxInfo[i].field5_0x14 = g_music[i].field5_0x14;
        SfxInfo[i].sample = g_music[i].sample;
        SfxInfo[i].field7_0x1c = g_music[i].field7_0x1c;
        SfxInfo[i].index = i;

        NuStrCpy(sfx_filename[i], SfxInfo[i].filename);
        SfxInfo[i].filename = sfx_filename[i];

        NumSfx = NumSfx + 1;
        NumSfxNames = NumSfxNames + 1;

        LOG_DEBUG("SfxInfo[%d]: name=%s", i, SfxInfo[i].filename);
    }

    SfxInfo[SFX_MUSIC_COUNT].filename = NULL;
    SfxInfo[SFX_MUSIC_COUNT].field4_0x4 = NULL;
    SfxInfo[SFX_MUSIC_COUNT].index = -1;

    NuStrCpy(cfgfile_name, file);

    LoadSfx(file, buffer_start, buffer_end);

    memset(GlobalSfxBits, 0, sizeof(GlobalSfxBits));

    for (i32 i = 0; i < NumSfxInst; i++) {
        NUSOUNDINFO *sound_info = &g_soundInfo[i];
        if (sound_info->global != 0) {
            i32 index = sound_info->index;
            GlobalSfxBits[index >> 4] |= static_cast<u16>(1 << (index & 0xf));
        }
    }

    ResetSounds();
}

void LoadSfx(const char *file, variptr_u *buffer_start, variptr_u buffer_end) {
    nufpar_s *fp = NuFParCreate(const_cast<char *>(file));
    if (fp != NULL) {
        NuFParPushCom(fp, audioCom);
        while (true) {
            if (NuFParGetLine(fp) == 0) {
                break;
            }
            NuFParGetWord(fp);
            if (*fp->word_buf != '\0') {
                NuFParInterpretWord(fp);
            }
        }
        NuFParDestroy(fp);
    }

    nusound_filename_info_s *last = &SfxInfo[NumSfx];
    last->filename = NULL;
    last->field4_0x4 = NULL;
    last->index = -1;

    NuSound3SetSampleTable(SfxInfo, buffer_start, buffer_end);

    memset(SfxBits, 0, sizeof(SfxBits));

    if (NOSOUND == 0) {
        NuSound3SetRequestTable(SfxBits, 100);
    }
}

void GroupBuffer_MoveToEnd(i32 group_id) {
    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    i32 group_end = first_sample + sample_count;
    if (g_lenGroupBuffer == group_end) {
        return;
    }

    g_groups[group_id].first_sample = static_cast<i16>(g_lenGroupBuffer);
    memmove(&g_groupBuffer[g_lenGroupBuffer], &g_groupBuffer[first_sample],
            static_cast<usize>(sample_count) * sizeof(i16));
    memmove(&g_groupBuffer[first_sample], &g_groupBuffer[group_end],
            static_cast<usize>(g_lenGroupBuffer - first_sample) * sizeof(i16));

    for (i32 i = 0; i < g_numGroups; i++) {
        if (g_groups[i].first_sample > first_sample) {
            g_groups[i].first_sample -= sample_count;
        }
    }
}

void GroupBuffer_AddToGroup(i32 group_id, i32 sample_id) {
    if (g_lenGroupBuffer == 512) {
        return;
    }

    g_groupBuffer[g_lenGroupBuffer++] = static_cast<i16>(sample_id);
    g_groups[group_id].sample_count++;
    g_groups[group_id].field_0x4 = 0;
    g_soundInfo[sample_id].group = static_cast<i16>(group_id);
}

void GroupBuffer_RemoveFromGroup(i32 group_id, i32 sample_id) {
    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    i32 end = first_sample + sample_count;
    i32 index = first_sample;
    while (index < end && g_groupBuffer[index] != sample_id) {
        index++;
    }
    if (index == end) {
        return;
    }

    memmove(&g_groupBuffer[index], &g_groupBuffer[index + 1],
            static_cast<usize>(g_lenGroupBuffer - index - 1) * sizeof(i16));
    g_lenGroupBuffer--;
    g_groups[group_id].sample_count = sample_count - 1;
    g_groups[group_id].field_0x4 = 0;
}

void GroupBuffer_RemoveGroup(i32 group_id) {
    if (group_id == -1) {
        return;
    }

    i32 first_sample = g_groups[group_id].first_sample;
    i16 sample_count = g_groups[group_id].sample_count;
    memmove(&g_groupBuffer[first_sample], &g_groupBuffer[first_sample + sample_count],
            static_cast<usize>(g_lenGroupBuffer - first_sample - sample_count) * sizeof(i16));
    g_lenGroupBuffer -= sample_count;

    for (i32 i = 0; i < g_numGroups; i++) {
        if (g_groups[i].first_sample > first_sample) {
            g_groups[i].first_sample -= sample_count;
        }
    }

    memmove(&g_groups[group_id], &g_groups[group_id + 1], static_cast<usize>(g_numGroups - 1) * sizeof(SoundGroup));
    g_numGroups--;

    for (i32 i = 0; i < SFX_MUSIC_COUNT; i++) {
        if (g_soundInfo[i].group > group_id) {
            g_soundInfo[i].group--;
        } else if (g_soundInfo[i].group == group_id) {
            g_soundInfo[i].group = -1;
        }
    }
}

i32 GroupBuffer_MakeGroup(i32 sample_id) {
    if (g_numGroups == 128 || g_lenGroupBuffer == 512) {
        return -1;
    }

    i32 group_id = g_numGroups;
    g_groups[group_id].first_sample = static_cast<i16>(g_lenGroupBuffer);
    g_groups[group_id].sample_count = 1;
    g_groups[group_id].field_0x4 = 0;
    g_soundInfo[sample_id].group = static_cast<i16>(group_id);
    g_groupBuffer[g_lenGroupBuffer++] = static_cast<i16>(sample_id);
    g_numGroups++;
    return group_id;
}

i32 GroupBuffer_InGroup(i32 group_id, i32 sample_id) {
    i32 index = g_groups[group_id].first_sample;
    i32 end = index + g_groups[group_id].sample_count;
    while (index < end) {
        if (g_groupBuffer[index] == sample_id) {
            return 1;
        }
        index++;
    }
    return 0;
}

i32 GroupBuffer_GetSample(i32 group_id, i32 sequential) {
    i32 loaded_samples[32];
    i16 sample_count = g_groups[group_id].sample_count;
    i16 first_sample = g_groups[group_id].first_sample;
    memset(loaded_samples, 0, sizeof(loaded_samples));
    i32 loaded_count = 0;

    i16 *sample = &g_groupBuffer[first_sample];
    i16 *end = sample + sample_count;
    while (sample != end) {
        i32 sample_id = *sample;
        if (NuSound3IsSampleLoaded(g_soundInfo[sample_id].index)) {
            loaded_samples[loaded_count++] = *sample;
        }
        sample++;
    }

    if (loaded_count == 0) {
        return -1;
    }

    i32 index;
    if (sequential == 0) {
        seed = NuRandIntSeeded(&seed);
        index = static_cast<i32>((seed >> 16) % static_cast<u32>(loaded_count));
    } else {
        index = g_groups[group_id].field_0x4++;
        if (g_groups[group_id].field_0x4 >= loaded_count) {
            g_groups[group_id].field_0x4 = 0;
        }
    }

    return loaded_samples[index];
}

i32 GroupBuffer_GetNumInGroup(i32 group_id) {
    return g_groups[group_id].sample_count;
}

i32 GroupBuffer_GetSampleByIndex(i32 group_id, i32 sample_index) {
    return g_groupBuffer[g_groups[group_id].first_sample + sample_index];
}

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

extern "C" void PlaySfxByIdEx(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch) {
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
        voice_volume =
            static_cast<i32>(static_cast<f32>(voice_volume) * (1.0f + NuRandFloatSeeded(&seed) * sound->volume_rnd));
    }

    if (static_cast<u32>(sample_index) <= 1599) {
        if (position != NULL) {
            if (loop) {
                NuSound3Play3dLoopSfx(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume,
                                      pitch);
            } else if (priority == 0) {
                NuSound3Play3d(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume, pitch,
                               buzz_timer, rumble_strength, rumble_sustain, rumble_release);
            } else {
                NuSound3Play3dPri(position, sample_index, falloff_near, falloff_far, voice_volume, voice_volume, pitch,
                                  buzz_timer, rumble_strength, rumble_sustain, rumble_release, priority);
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
                NuSound3Play3dLoopSfx(&pos, sample_index, falloff_near, falloff_far, volume_left, volume_right, pitch);
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

i32 GetSfxId(const char *name) {
    if (name != NULL) {
        u32 hash = CRC_ProcessStringIgnoreCase(name);
        if (g_soundMap != NULL) {
            for (i32 index = g_soundMap[hash & 0xff]; index != -1; index = g_soundInfo[index].next) {
                if (NuStrNICmp(name, g_soundInfo[index].sfx_name, 32) == 0) {
                    return index;
                }
            }
        }
    }

    return -1;
}

extern "C" i32 GetLogicalSfxCount(void) {
    return NumSfxInst;
}

extern "C" i32 GetSfxCount(void) {
    return NumSfx;
}
