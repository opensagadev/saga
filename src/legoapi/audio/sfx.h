#ifndef LEGOAPI_AUDIO_SFX_H
#define LEGOAPI_AUDIO_SFX_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/sfx.h"

// Audio / SFX playback API. PlaySfx is a C-linkage symbol in the original
// (unmangled); TickTockSfx is C++. PlaySfxByIdEx comes from its numusic owner header.

#ifdef __cplusplus
extern "C" {
#endif
    void PlaySfx(char *name, nuvec_s *pos);
    i32 IsSfxLooping(i32 sfx_id);
    void SetSfxBit_On(i32 sound);
    void ClearLinkedCutSceneMusic(void *context);
    void SetLinkedCutSceneMusic(void *context, i32 state);
#ifdef __cplusplus
}
#endif

void TickTockSfx(void);
void GameAudio_PlaySfxById(i32 sfx_id, NUVEC *position, i32 flags, i32 volume);
void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);
i32 GameAudio_GetPlrSfxBits(void *object);
void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

#endif
