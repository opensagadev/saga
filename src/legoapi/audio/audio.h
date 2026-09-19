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

extern i32 MusicOther;
extern i32 PlayersUnderAttack;
extern i32 (*CheckMusicOtherFn)(void);
extern i32 (*GameAudio_ActionMusicFn)(void);

void ProcessMusicChanges(LEVELDATA_s *level, OPTIONSSAVE_s *options);
