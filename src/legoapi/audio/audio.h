#pragma once

#include "decomp.h"

struct LEVELDATA_s;
struct OPTIONSSAVE_s;
struct GAMEAUDIO;
struct nuvec_s;
struct nusound_filename_info_s;

void GameAudio_Init(GAMEAUDIO *audio);
void GameAudio_Reset();
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxAndSetVolume(i32 sfx, nuvec_s *position, f32 volume);
i16 GetMusicIndex(char *name, nusound_filename_info_s *table, i32 default_index);
f32 GameSetSoundVolume(OPTIONSSAVE_s *options);
f32 GameSetMusicVolume(OPTIONSSAVE_s *options);
f32 GameGetMusicVolume(OPTIONSSAVE_s *options);
void legoSetMusicVolume(f32 volume);

extern i32 MusicOther;
extern i32 PlayersUnderAttack;
extern i32 (*CheckMusicOtherFn)(void);
extern i32 (*GameAudio_ActionMusicFn)(void);

void ProcessMusicChanges(LEVELDATA_s *level, OPTIONSSAVE_s *options);

// Declared here for the other units that use them.
struct GameObject_s;
struct OPTIONSSAVE_s;
struct WORLDINFO_s;
struct nuvec_s;
extern void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position);
extern void SetBackgroundMusic(i32);
void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx);
void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count);
void EngineNoiseCode(GameObject_s *, i32);
void PlayDieSfx(GameObject_s *object);
void PlayFootStepSfx(GameObject_s *object);
void PlayGruntSfx(GameObject_s *);
void PlayHurtSfx(GameObject_s *object);
void PlayJumpSfx(GameObject_s *, i32);
void PlayLandSfx(GameObject_s *, i32, i32);
void PlaySabreSfx(char *, GameObject_s *, nuvec_s *, i32);
void ResetRepeatSfx();
void SetSoundFadeDist(WORLDINFO_s *, OPTIONSSAVE_s *);
void SfxCheckMusicOnOff(OPTIONSSAVE_s *);
void legoSetCutVolume(float v);
extern "C" {
    extern void (*ExtraDieSfxFn)(GameObject_s *);
    extern void (*ExtraHurtSfxFn)(GameObject_s *);
}
