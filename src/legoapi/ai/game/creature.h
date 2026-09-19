#pragma once

#include "decomp.h"

struct AIPATHINFO_s;
struct GameObject_s;
struct LEVEL_PROGRESS_s;
struct nuvec_s;

void SnapCreaturePos(GameObject_s *object, nuvec_s *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);
void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress);
