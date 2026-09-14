#ifndef LEGOAPI_ITEMS_COLLECT_TORPEDO_H
#define LEGOAPI_ITEMS_COLLECT_TORPEDO_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

void InitTorpedoPackets();
TORPEDOPACKET_s *GetTorpedoPacket();
void FreeTorpedoPacket(TORPEDOPACKET_s **packet);
void DropTorpedoPickups(TORPEDOPACKET_s *packet, i32 count);
i32 getMaxTorpedos(GameObject_s *object);
void Torpedo_UpdateJobbies(GameObject_s *object);
void *FindNearestTorpTarget(WORLDINFO_s *world, NUVEC *position, f32 distance_squared, u8 *target_type);
void TorpedoCode(GameObject_s *object, i32 fire, f32 fire_cooldown);
void HomeNearestTorpTarget(BOLT_s *bolt, TORPEDOPACKET_s *packet);
void Torpedo_InitRicochet(BOLT_s *bolt, NUVEC *position);
void Torpedo_Ricochet(BOLT_s *bolt, TORPEDOPACKET_s *packet);
void TorpedoHitTarget(BOLT_s *bolt);
void DrawTorpedoTargetSprite(void *target, u8 type, f32 alpha);
void DrawTorpedos(GameObject_s *object);
void Torpedo_InitBolt(BOLT_s *bolt);
void Torpedo_UpdateBolt(BOLT_s *bolt);
f32 Torpedo_Scale(BOLT_s *bolt);
void Torpedo_EndBolt(BOLT_s *bolt);
void Torpedo_Shoot(GameObject_s *object);

#endif
