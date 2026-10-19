#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GAMECAMERA_s;
struct GameObject_s;
struct NARROWSOCKEXCEPTION;
struct SOCKSYS;
struct nuvec_s;
extern i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);
void CentreTwoPlayerCamera(nuvec_s *, nuvec_s *, nuvec_s *, nuvec_s *);
void GameCam_HitRoll(void);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
void GameCam_UpdateLookRot(GAMECAMERA_s *camera);
void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);
void MakePlayPlanes(GAMECAMERA_s *);
void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *);
void SetCameraMatrices(void);
void GameCam_HitJudder(void);
