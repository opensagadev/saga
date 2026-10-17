#ifndef LEGOAPI_CUTSCENES_CUTSCENES_H
#define LEGOAPI_CUTSCENES_CUTSCENES_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

// Cutscene API shared by the cutscene owners.

// The instance look-at record is still opaque in legoapi_types.h. Its fields
// are used at the offsets written by the original cutscene code.
struct GCutLookAtState {
    struct nugscn_s *scene;
    f32 lid_frame; // Eyelid texture frame, eased towards open or closed.
    u16 character_index;
    u16 material_index;
    u16 texture_animation_index;
    u16 secondary_material_index;
    u16 tertiary_material_index;
    u16 joint_override_index;
    struct NUGCUTLOCATOR_s *locator;
};
struct CutSceneListEntry {
    char *directory;
    char *filename;
    i16 level;
    i16 padding;
};
DECOMP_ASSERT(sizeof(CutSceneListEntry) == 0xc, "Cutscene list entry ABI");
DECOMP_ASSERT(offsetof(CutSceneListEntry, level) == 8, "Cutscene list level offset");
extern i32 CUTCOUNT;
extern CutSceneListEntry *CutList;

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
extern CUTSCENESYS *CutSceneSys;
extern void (*CutScene_OverrideConfigFileNameFn)(char *, i32, i32);
extern "C" i32 (*CutScenePlayer_AcceptFn)(CUTSCENEPLAYERCLIP *);
void CutScenePlayer_Configure(char *, VARIPTR *, VARIPTR *, i16 *, i16 *, i16 *, i16 *, i16 *);
i32 CutScenePlayer_CountEpisodeClips(i32, i32, i16 *);
void CutScenePlayer_DrawGrid(COLLECTION_s *, i16 *, f32, f32, i32, f32);
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
void CutScene_DrawSubtitles();
extern "C" void instNuGCutSceneEnd(instNUGCUTSCENE_s *instance);

#endif

// Declared here for the other units that use them.
struct CUTINFO;
struct CUTSCENESYS;
struct CUTSYS;
struct WORLDINFO_s;
extern CUTSYS *CS_cutsys;
extern i32 ACTIVECUTCOUNT;
extern i32 CutInstEndCount;
extern u8 cutskip_dontplaylevelintro;
extern void *CutScene_FindInst(CUTSYS *, char *);
extern void CutScenePlayer_Reset();
void CutScenePlayer_SetObjects(CUTINFO *);
void CutScene_StartAudio();
void CutScenes_Destroy(CUTSYS *system);
void CutScenes_InitSystem(CUTSCENESYS *);
void CutScenes_Reset(WORLDINFO_s *);
