#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MENU_s;
struct TEXTENTRY;
extern f32 APITEXTSCALEX;
extern f32 APITEXTSCALEY;
extern i32 textcrawlactive;
i32 GetParentMenuID(void);
void GameDrawMenuEntry(MENU_s *menu, char *text);
void IntroText_Draw(f32 alpha);
void IntroText_SetTextID(i32 id);
void Text_InitStringTable(i32, VARIPTR *, VARIPTR *);
void Text_InitTable(TEXTENTRY *, i32, i32);
void Text_InsertCommasIntoNumber(char *number, char *text, i32 length);
void Text_LoadAndFixUpStrings(unsigned char *filename, unsigned char **buffer, char **table, i32 count);
