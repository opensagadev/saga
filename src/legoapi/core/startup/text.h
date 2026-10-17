#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
extern char * LEGOASCII_UP;
extern i32 (*Tag_NoHiddenIconFn)(GameObject_s *);
f32 TextPulseTimer(f32 delay);
void Tag_DrawIcon_Batman(GameObject_s *object);
void Tag_DrawIcon_LSW(GameObject_s *object);
void Text_DecodeButtons(char *source, char *destination);
void Text_FillInExtendedSaveInfo();
void Text_InitDefaultStrings(void);
void Text_LoadStrings(VARIPTR *, VARIPTR *);
