#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct HINTUIBUTTON_s;
struct HINT_s;
extern HINTUIBUTTON_s * hintUIButton;
extern f32 (*Hint_AlphaTargetFn)();
extern f32 AlphaBlendTime;
extern f32 ICONXPOS_TOUCHYFEELY;
extern f32 ICONXPOS_VIRTUALS;
extern f32 ICONYPOS_TOUCHYFEELY;
extern f32 ICONYPOS_VIRTUALS;
extern i32 (*Hub_PanelBusyFn)();
extern i32 only_process_this_hint_id;
extern void *GetHintFromUIButton();
void Hint_ClearHintsAndDoneFlags();
void Hint_ExpandButtons(char *input, char *output);
void Hint_LoadAllGameState();
void Hint_Reset();
void Hint_ResetHint(i32 hint_id, i32 reset_completed);
void Hint_SaveAllGameState();
void Hint_SaveGameState(HINT_s *hint);
void Hint_SetHint(HINT_s *, i32, i32);
void Hint_SetHintFromId(i32, i32, i32);
void initHintSys();
