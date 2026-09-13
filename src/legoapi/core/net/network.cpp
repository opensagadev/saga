#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numath/nufloat.h"

void TTNetwork::Broadcast(NetMessage, unsigned char) {
    STUBBED();
}

void TTNetwork::ClearMyHostAddress() {
    has_my_host_address = false;
}

void TTNetwork::Display(ThingRenderData *) {
    STUBBED();
}

const NetAddress &TTNetwork::GetMyAddress() const {
    return my_address;
}

const NetAddress *TTNetwork::GetMyHostAddress() const {
    return has_my_host_address ? &my_host_address : NULL;
}

void TTNetwork::Initialise() {
    STUBBED();
}

void TTNetwork::ProcessEvenWhenPaused(ThingProcessData *) {
    STUBBED();
}

void TTNetwork::ReliableBroadcast(NetMessage, unsigned char) {
    STUBBED();
}

void TTNetwork::ReliableSend(NetMessage, unsigned char, NetPeer &, char const *, u32) {
    STUBBED();
}

void TTNetwork::Resume() {
    STUBBED();
}

void TTNetwork::Send(NetMessage, unsigned char, NetPeer &) {
    STUBBED();
}

void TTNetwork::SetMyHostAddress(NetAddress const &address) {
    has_my_host_address = true;
    my_host_address = address;
}

void TTNetwork::Shutdown() {
    STUBBED();
}

bool TTNetwork::Suspend() {
    STUBBED();
    return true;
}

TTNetwork::TTNetwork() {
    STUBBED();
}

void TTNetwork::Update() {
    STUBBED();
}

TTNetwork::~TTNetwork() {
}

static __used__ void NOSGetGuid() {
    STUBBED();
}
