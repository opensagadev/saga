#pragma once
#include "legoapi/legoapi_types.h"

extern "C" {
    extern ADDPART_s Default_ADDPART;
    PART_s *AddPart(ADDPART_s *part);
    PART_s *HitParts(GameObject_s *owner, NUVEC *positions, i32 count, f32 radius, NUVEC *minimum, NUVEC *maximum,
                     u32 flags);
    void SetPartRTLSet(usize rtl_set);
    void AddVariableShotDebrisEffectMtx4(i32 effect, NUVEC *position, NUVEC *momentum, i32 count,
                                         NUMTX *emitter_orientation, NUMTX *particle_orientation, u16 priority,
                                         i8 flags);
    void AddVariableShotDebrisEffectTimed5(i32 effect, NUVEC *position, NUVEC *momentum, NUVEC *position_delta,
                                           i32 count, f32 time, NUMTX *emitter_orientation, NUMTX *particle_orientation,
                                           u16 priority, i8 flags);
    void AddVariableShotPARTEffect(i32 effect, NUVEC *position, f32 rate, f32 time, NUMTX *orientation);
}
i32 FindPartDebris(PARTDEBSYS_s *system, char *name);
void InitPartTable(char **names);
void LoadPartFile(WORLDINFO_s *world);
void AddPartDebris(PARTDEBSYS_s *system, i32 index, NUVEC *position);
void SetKillPartMom(NUVEC *momentum);
void KillParts(GameObject_s *object, i32 animation, i32 variant, i32 mode, f32 scale, i32 flags, u16 *rotations);
void PartImpact_Brick(PART_s *part);
void PartStop_Flickerer(PART_s *part);
i32 PartDraw_Flickerer(PART_s *part);
void PartCollide_3D(PART_s *part);
void PartKill_ForceThrow(PART_s *part, i32 reason);
void PowerUp_Particles(WORLDINFO_s *world, NUVEC *position);
void CollectPowerUp(GameObject_s *object, NUVEC *position, u16 rotation, i32 flags);
void CollectHitPoint(GameObject_s *object, NUVEC *position, i32 flags);
