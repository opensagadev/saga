#pragma once

#include "decomp.h"

struct LEVELDATA_s;
struct WORLDINFO_s;

void PauseGame(i32 pad_index);
void NetworkSyncPause(void);
void ResumeGame(i32 play_sound, i32 resume_music);
void ClearPause(void);
void NewGameMode(void);
void RestoreOptions(void);
void InitSuperStory(i32 mode);
i32 InStory(void);
i32 Game_Exit(i32 last_area);
LEVELDATA_s *CanSaveAndExit(WORLDINFO_s *world);

void AddToCompletionPoints(u32 points);
i32 Game_100PercentComplete(void);
void AddToGoldBricks(void);
i32 Game_GotAllGoldBricks(void);
i32 Game_AutoSaving(void);
bool FreePlayUnlocked(void);

extern void (*Game_AllGoldBricksFn)(void);
extern void (*Game_100PercentFn)(void);
