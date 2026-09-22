#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/numath/nufloat.h"

void refpack_init();

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
    if (field_2140 != 0) {
        return;
    }

    field_2148 = -1;
    ftp_manager.field_1604 = -1;
    network_objects.field_08 = -1;
    field_2134 = 0;
    field_2138 = 0;
    session = reinterpret_cast<NetSession *>(V2SessionManager::mpSessionManager);
    theSession = session;
    ftp_manager.Init();
    network_objects.Init();

    void **vtable = *reinterpret_cast<void ***>(session);
    typedef void (*SessionInitFn)(NetSession *, char *);
    reinterpret_cast<SessionInitFn>(vtable[0])(session, NULL);
    typedef void (*SessionGetLocalAddressFn)(NetSession *, NetAddress *);
    reinterpret_cast<SessionGetLocalAddressFn>(vtable[10])(session, &my_address);

    field_2140 = 1;
    field_2144 = 0;
    theNuNetEmu.field_10 = 1;
    theNuNetEmu.SetConditions(NuNetEmu::CONDITIONS_NORMAL);
    refpack_init();
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

TTNetwork::TTNetwork() : field_20(0), field_24(0), field_2140(0), my_address(), my_host_address() {
    field_2158 = -0.3f;
    field_215c = 0.5f;
    has_my_host_address = 0;
    field_213c = 0;
    field_2150 = 0;
    field_2154 = 1.0f;
    field_2160 = 1.0f;
}

char const *TTNetwork::GetName() {
    return "";
}

void TTNetwork::Update() {
    STUBBED();
}

TTNetwork::~TTNetwork() {
}
