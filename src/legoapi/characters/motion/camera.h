#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct GameObject_s;
struct VIEWCAM_s;
struct nuvec_s;
extern VIEWCAM_s ViewCam;
extern f32 testmovef;
extern nuvec_s ObstacleCamCutPts[2];
i32 GoingForwardsAlongNarrowSock(GameObject_s *object);
