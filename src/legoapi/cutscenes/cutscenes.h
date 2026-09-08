#ifndef LEGOAPI_CUTSCENES_CUTSCENES_H
#define LEGOAPI_CUTSCENES_CUTSCENES_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

// Cutscene API (module legoapi/cutscenes, cutscenes.cpp).
enum CUTSCENEPLAYER_CLIP_TYPE { CLIP_INTRO, CLIP_MIDTRO, CLIP_OUTRO, CLIP_ENDING };
struct CUTSCENEPLAYERCLIP {
    i16 level_id;
    u8 type;
    i8 guest_episode;
    char name[64];
};
CUTSCENEPLAYERCLIP *CutScenePlayer_Active();
struct CUTSCENEPLAYER_s {
    CUTSCENEPLAYERCLIP *clips;
    CUTSCENEPLAYERCLIP *active;
    u16 clip_count;
    i16 return_door;
    i16 *clip_text;
    i16 *intro_text;
    i16 *midtro_text;
    i16 *outro_text;
    i16 *ending_text;
};
DECOMP_ASSERT(sizeof(CUTSCENEPLAYERCLIP) == 0x44, "Cutscene clip ABI");
DECOMP_ASSERT(sizeof(CUTSCENEPLAYER_s) == 0x20, "Cutscene player ABI");
extern CUTSCENEPLAYER_s *CutScenePlayer;
extern "C" i32 (*CutScenePlayer_AcceptFn)(CUTSCENEPLAYERCLIP *);
void CutScenePlayer_Configure(char *, VARIPTR *, VARIPTR *, i16 *, i16 *, i16 *, i16 *, i16 *);
i32 CutScenePlayer_CountEpisodeClips(i32, i32, i16 *);
void CutScenePlayer_Start(i32, i32);
void *CutScenePlayer_Available();
i32 CutScenePlayer_CanStart(i32);
void CutScenePlayer_GetText(i32, char *, char *, i32);

CUTINFO *CutScene_Find(CUTSYS *cutsys, char *name);
i32 CutScene_HasPlayed(CUTINFO *cut);
void CutScene_SnapToEnd(CUTINFO *cut);
void CutScene_StoppedFn_LSW(CUTINFO *cut);
i32 CutScene_PlayingOrRequested(CUTINFO *cut);
i32 CutScene_IsSkippable(CUTINFO *cut);

#endif
