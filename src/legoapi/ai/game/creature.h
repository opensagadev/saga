#pragma once

#include "decomp.h"

struct AIPATHINFO_s;
struct AILOCATOR_s;
struct GameObject_s;
struct LEVEL_PROGRESS_s;
struct nuvec_s;

void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress);
AILOCATOR_s *getSpawnLocator(f32 clip_radius, char *name);
i32 SpawnMeleeCreatureType(i32 type);

// Declared here for the other units that use them.
struct AISYS_s;
struct CHARACTERDATA_s;
struct CHARFIXUP;
struct GameObject_s;
struct PLAYERPACKET_s;
struct nuvec_s;
extern GameObject_s *GetOtherActivePlayer(GameObject_s *);
extern u8 CharClipToBlobShadows;
extern void DeactivateGameObject(GameObject_s *object);
extern void ResetAICreature(GameObject_s *object, AISYS_s *system);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void SetGameObjectCharacterData(GameObject_s *obj);
i32 AnakinGreenSabre(GameObject_s *object);
void AICreatureResumeScript(GameObject_s *);
void ClearAICreatures();
void ConfigureComplexShadow(GameObject_s *);
void DrawGameObjectsDraw(i32 pass);
void ExtraCharacterFixUpAfterConfig();
void FixUpCharacters(CHARFIXUP *fixup);
void InitAICreatures(AISYS_s *system);
void KeepOnScreen(GameObject_s *);
void MovePlayer(GameObject_s *object);
void MovePlayerSpline(GameObject_s *);
void ResetAICreatures(AISYS_s *system);
void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface);
void ResetPlayerAI(GameObject_s *obj);
void ResetPlayerMoves(GameObject_s *object);
void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);
void TerrainPlayer(GameObject_s *);
void UpdateCharacterIdle(GameObject_s *object);
