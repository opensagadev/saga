#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numath/nufloat.h"

void TTNetwork::Broadcast(NetMessage, unsigned char) {
}

void TTNetwork::ClearMyHostAddress() {
    has_my_host_address = false;
}

void TTNetwork::Display(ThingRenderData *) {
}

const NetAddress &TTNetwork::GetMyAddress() const {
    return my_address;
}

const NetAddress *TTNetwork::GetMyHostAddress() const {
    return has_my_host_address ? &my_host_address : NULL;
}

void TTNetwork::Initialise() {
}

void TTNetwork::ProcessEvenWhenPaused(ThingProcessData *) {
}

void TTNetwork::ReliableBroadcast(NetMessage, unsigned char) {
}

void TTNetwork::ReliableSend(NetMessage, unsigned char, NetPeer &, char const *, u32) {
}

void TTNetwork::Resume() {
}

void TTNetwork::Send(NetMessage, unsigned char, NetPeer &) {
}

void TTNetwork::SetMyHostAddress(NetAddress const &address) {
    has_my_host_address = true;
    my_host_address = address;
}

void TTNetwork::Shutdown() {
}

bool TTNetwork::Suspend() {
    return true;
}

TTNetwork::TTNetwork() {
}

void TTNetwork::Update() {
}

TTNetwork::~TTNetwork() {
}

static __used__ void NOSGetGuid() {
}
