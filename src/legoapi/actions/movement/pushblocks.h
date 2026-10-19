#pragma once

#include "nu2api/nucore/fixed_width.h"

struct pushblock_s;
struct nuvec_s;

void KnockPushBlock(pushblock_s *block, nuvec_s *direction);

// Declared here for the other units that use them.
struct GameObject_s;
struct HINT_s;
struct WORLDINFO_s;
extern u32 (*CanPushBlocksFn)(GameObject_s *);
i32 Push_UpdateHints(HINT_s *);
i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index);
pushblock_s *BlockInBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, pushblock_s **support);
void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity);
void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);
i32 PushBlock(GameObject_s *object);
