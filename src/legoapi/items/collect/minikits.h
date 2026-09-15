#pragma once

#include "nu2api/nucore/common.h"

struct AREASAVE_s;
struct GameObject_s;
struct WORLDINFO_s;
struct nuvec_s;

void MiniKits_Init(variptr_u *buffer, variptr_u *buffer_end);
void CollectMinikit(nuvec_s *position, char *name, i32 type);
void MiniKitDetector(nuvec_s *position);
void CollectAllMiniKits(AREASAVE_s *save);
void CharacterMiniKits_Dump(WORLDINFO_s *world);
void ResetMinikitCounter(void);
void IncrementMinikitCounter(GameObject_s *object);
