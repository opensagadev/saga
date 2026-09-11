#pragma once
#include "legoapi/legoapi_types.h"

extern "C" {
    extern ADDPART_s Default_ADDPART;
    PART_s *AddPart(ADDPART_s *part);
}
i32 FindPartDebris(PARTDEBSYS_s *system, char *name);
void SetKillPartMom(NUVEC *momentum);
void PartImpact_Brick(PART_s *part);
void PartStop_Flickerer(PART_s *part);
i32 PartDraw_Flickerer(PART_s *part);
