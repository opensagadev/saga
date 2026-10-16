#include "decomp.h"
#include "gamelib_util_types.h"
#include "nu2api/nucore/nuvuvec.hpp"

TPool<NetMessage::MessageData, 512> NetMessage::sm_poolMessageData;

void NetMessage::RaiseError() {
    theSession->error = 0xa0001000;
}

void NetMessage::DebugPrint() const {
}
