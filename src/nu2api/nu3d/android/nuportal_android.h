#pragma once

#include "nu2api/nu3d/nuportal.h"

extern "C" {
    void Initialise_PS(nugscn_s *scene);
    void FlagRoomInstancesAsVisible(NUROOM *room, nugscn_s *scene);
    void SetAllInstancesVisible(nugscn_s *scene);
    void clipRoomAgainstFrustrum(nugscn_s *scene, NUROOM *room, NUFRUSTRUM *frustum);
}

void SetAllInstancesHidden(nugscn_s *scene);
