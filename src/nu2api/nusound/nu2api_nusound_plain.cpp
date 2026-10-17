#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"

#include <string.h>

typedef void (*SoundBitCallback)(i32 sound_id);

extern "C" void MusicPreSeek(i32 track);
extern "C" void RestoreGameMusic(void);

extern "C" {
    i32 g_NuSoundMaxVoicesPerSample = 3;
    f32 AUDIOFADELEVEL = 1.0f;
extern f32 MASTERVOLUME;
    i32 gcutSoundMusVol = 30;
    i32 gcutSoundVol = 100;

extern MusicPlayback Music;

    void NuSound3FlushLoops(void);
    void SoundStopMusic(void);

f32 GetSoundVolume(void);
void SetSoundBitsById(const i32 *sound_ids, SoundBitCallback set_bit);
void SetSoundBitsBySingleId(i32 sound_id, SoundBitCallback set_bit);
void SetSoundVolume(f32 volume);
void SoundKillAll(void);
void SoundStopMusic(void);

void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
void edbitsSoundPlay(NUVEC *, i32);
    void gcutSetSoundVol(i32 sound_volume, i32 music_volume) {
        gcutSoundVol = sound_volume;
        gcutSoundMusVol = music_volume;
    }
}
