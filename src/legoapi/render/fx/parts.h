#pragma once
#include "legoapi/legoapi_types.h"

extern "C" {
    extern ADDPART_s Default_ADDPART;
    PART_s *AddPart(ADDPART_s *part);
    void HitParts(void);
    void SetPartRTLSet(usize rtl_set);
}
i32 FindPartDebris(PARTDEBSYS_s *system, char *name);
void InitPartTable(char **names);
void LoadPartFile(WORLDINFO_s *world);
void AddPartDebris(PARTDEBSYS_s *system, i32 index, NUVEC *position);
void SetKillPartMom(NUVEC *momentum);
void PartImpact_Brick(PART_s *part);
void PartStop_Flickerer(PART_s *part);
i32 PartDraw_Flickerer(PART_s *part);
void PartCollide_3D(PART_s *part);
void PartKill_ForceThrow(PART_s *part, i32 reason);
