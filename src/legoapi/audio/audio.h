#pragma once

#include "decomp.h"

struct LEVELDATA_s;
struct OPTIONSSAVE_s;
struct GAMEAUDIO;
struct nuvec_s;

void GameAudio_Init(GAMEAUDIO *audio);
void GameAudio_Reset();
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxAndSetVolume(i32 sfx, nuvec_s *position, f32 volume);

extern i32 MusicOther;
extern i32 PlayersUnderAttack;
extern i32 (*CheckMusicOtherFn)(void);
extern i32 (*GameAudio_ActionMusicFn)(void);

void ProcessMusicChanges(LEVELDATA_s *level, OPTIONSSAVE_s *options);
