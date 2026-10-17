#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct nuvec_s;
extern "C" {
    void PerspectMidPoint(nuvec_s *result, nuvec_s *first, nuvec_s *second, nuvec_s *camera_position);
}
