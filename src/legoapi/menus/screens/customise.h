#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct CUSTOMISER;
struct GAMESAVE_s;
struct MENU_s;
struct WORLDINFO_s;
extern GAMESAVE_s OldCustomiseGame;
extern i32 Customiser_MenuAvailable(CUSTOMISER *);
extern i32 customiser_changed;
extern i32 customiser_save_done;
extern void Customiser_Draw3D(CUSTOMISER *);
extern void Customiser_Init(CUSTOMISER *);
extern void Customiser_Reset(CUSTOMISER *);
extern void Customiser_Update(CUSTOMISER *, WORLDINFO_s *);
void CustomiserMenu_Draw(MENU_s *menu);
void CustomiserMenu_Update(MENU_s *menu);
void Customiser_GetActiveWeirdoIndex(i32 *index, i32 *count);
void Customiser_SetNameAndIcon(CUSTOMISER *, i32);
void Customiser_SetUpCharacterData(CUSTOMISER *);
