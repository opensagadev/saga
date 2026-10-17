#pragma once

struct pushblock_s;
struct nuvec_s;

void KnockPushBlock(pushblock_s *block, nuvec_s *direction);

// Declared here for the other units that use them.
struct WORLDINFO_s;
void GizmoPushBlockInitAndReset(WORLDINFO_s *world, void *progress);
