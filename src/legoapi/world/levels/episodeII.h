#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct BOLT_s;
struct GameObject_s;
struct JEDIBNETPACKET_s;
struct nuhspecial_s;
extern i32 last;
extern nuhspecial_s green_light;
extern nuhspecial_s paintlights[3];
extern nuhspecial_s painttargetcolour[3];
i32 KaminoDiscoOn(void);
i32 KaminoE_CheckPlatHit(BOLT_s *);
i32 KaminoInside(void);
void GunShip_DragBombSeekBlowUp(GameObject_s *object);
extern "C" {
    extern JEDIBNETPACKET_s * jedib_netpacket;
}
