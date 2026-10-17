#include "decomp.h"
#include "legoapi/core/net/netplay.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

APICHARACTERMODELLIST_s NetFreePlayModelList[49];
i8 net_FreePlayModelCount;
u8 net_recievedFreePlayList;

void GetClientMineInfo(nuvec_s **positions, u64 **present, u64 **exploded);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);
extern void CharShadows_Reset(PLAYERPACKET_s *packet);
extern void ResetForceGlow(PLAYERPACKET_s *packet);

void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *);

void FinishLoop_Network() {
    STUBBED();
}

i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32);

void setObjInNetWaitContext(GameObject_s *object, i32 animation) {
    if (object != NULL) {
        object->field_0x7a5 = static_cast<u8>(LEGOCONTEXT_NETWAIT);
        object->context_animation = static_cast<i16>(animation);
    }
}
