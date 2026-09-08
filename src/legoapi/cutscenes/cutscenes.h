#ifndef LEGOAPI_CUTSCENES_CUTSCENES_H
#define LEGOAPI_CUTSCENES_CUTSCENES_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

// Cutscene API (module legoapi/cutscenes, cutscenes.cpp).
struct CUTSCENEPLAYERCLIP_s {
    i16 level_id;
    u8 pad_02[2];
    char name[0x40];
};
DECOMP_ASSERT(sizeof(CUTSCENEPLAYERCLIP_s) == 0x44, "CUTSCENEPLAYERCLIP ABI");
struct CUTSCENEPLAYER_s {
    CUTSCENEPLAYERCLIP_s *clips;
    CUTSCENEPLAYERCLIP_s *active;
    u16 clip_count;
};
CUTSCENEPLAYERCLIP_s *CutScenePlayer_Active();

CUTINFO *CutScene_Find(CUTSYS *cutsys, char *name);
i32 CutScene_HasPlayed(CUTINFO *cut);
void CutScene_SnapToEnd(CUTINFO *cut);
void CutScene_StoppedFn_LSW(CUTINFO *cut);
i32 CutScene_PlayingOrRequested(CUTINFO *cut);
i32 CutScene_IsSkippable(CUTINFO *cut);

#endif
