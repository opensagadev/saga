#pragma once

#include "decomp.h"

void Batarang_GetSightInfo(i32 character, i32 *red, i32 *green, i32 *blue, char *text);

// Declared here for the other units that use them.
struct BATARANG_s;
struct GameObject_s;
i32 Batarang_GetObjectFromCharID(i32);
i32 Batarang_SeekToTarget(BATARANG_s *);
i32 Batarang_StartTargetting(GameObject_s *);
void Batarang_MoveCode(GameObject_s *);
void Batarang_Release(GameObject_s *, i32);
void Batarangs_CheckLostData(void *);
void Batarangs_Reset(void);
