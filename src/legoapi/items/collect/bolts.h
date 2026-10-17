#pragma once

#include "nu2api/nucore/fixed_width.h"

extern i32 addbolt_nosfx;

struct BOLT_s;
struct GameObject_s;
struct PART_s;
struct nuvec_s;

i32 Bolt_HitGameObjects(BOLT_s *bolt, nuvec_s *points, nuvec_s *minimum, nuvec_s *maximum, f32 radius, u8 *hit_flags);
BOLT_s *Bolt_Find(i32 type_id, nuvec_s *position, GameObject_s *owner);
void Bolt_End(BOLT_s *bolt, i32 run_callback);
void Bolt_AddDeflectedBolt(BOLT_s *bolt, nuvec_s *direction, nuvec_s *normal, unsigned char *hit_flags);
void GuidedMissile_Kill(PART_s *part, i32 reason);
void GuidedMissile_Move(PART_s *part, f32 time);
void GuidedMissile_Deflect(PART_s *part);

// Declared here for the other units that use them.
struct BOLTSYS;
struct BOLT_s;
struct GameObject_s;
struct NetMessage;
struct PART_s;
struct WORLDINFO_s;
struct nuvec_s;
BOLT_s *Bolt_Alloc();
BOLT_s *FindIncomingBolt(GameObject_s *, i32, i32);
PART_s *Bolt_HitParts(BOLT_s *bolt, nuvec_s *points, nuvec_s *minimum, nuvec_s *maximum, f32 radius, i32 reason);
extern BOLT_s *objhitobj_bolt;
extern i32 (*BoltInitSfxFn)(GameObject_s *);
extern i32 addbolt_newsfx;
extern nuvec_s addbolt_newpos;
i32 Bolt_HitGameObject(BOLT_s *bolt, GameObject_s *object, nuvec_s *points, nuvec_s *, nuvec_s *, f32 radius, u8 *);
i32 Bolt_HitPartMode(BOLT_s *bolt);
void BoltSys_Init(BOLTSYS *);
void BoltTypes_Reset(WORLDINFO_s *);
void Bolt_HitGameObjectRC(NetMessage &);
void Bolt_Init(void *, NetMessage &);
void Bolt_PlayHitSfx(BOLT_s *);
void Bolt_Reflect(nuvec_s *normal, nuvec_s *incoming, nuvec_s *out);
void Bolt_Shoot(GameObject_s *, i32, i32);
void Bolts_Reset(void);
