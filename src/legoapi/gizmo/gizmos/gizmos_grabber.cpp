#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/motion.h"
#include "nu2api/numath/numtx.h"

void Grabber_Draw(WORLDINFO_s *) {
}

void Grabber_Reset(WORLDINFO_s *) {
}

void Grabber_Update(WORLDINFO_s *) {
}

// Original 0x22aae0, 264 bytes.
NUVEC *Grabber_GetGrabPos(GRABBER_s *grabber, numtx_s *matrix) {
    if (grabber->character_model != NULL && (grabber->flags_559 & 1) != 0 &&
        grabber->character_model->points_of_interest[0] != NULL) {
        if (matrix != NULL)
            *matrix = grabber->grab_matrix;
        return reinterpret_cast<NUVEC *>(&grabber->grab_matrix.m30);
    }
    if (matrix != NULL)
        *matrix = numtx_identity;
    return &grabber->grab_position;
}

void Grabber_StoreProgress(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress) {
    if (progress != NULL && world != NULL) {
        GRABBER_s *grabber = world->grabber;
        if (grabber != NULL) {
            progress->grabber_field_0x48c = grabber->field_0x48c;
            progress->grabber_field_0x484 = grabber->field_0x484;
            progress->grabber_field_0x494 = grabber->field_0x494;
        } else {
            progress->grabber_field_0x484 = 2000000.0f;
        }
    }
}

// Static grabber victim-pos helper. Moved from gizmisc_stubs.cpp.

static __used__ void Grabber_SetVictimPos(GRABBER_s *) {
}
