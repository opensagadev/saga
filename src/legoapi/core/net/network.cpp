#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/legoapi_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "gamelib/util/Utilities.h"
#include "nu2api/nucore/NuNetEmu.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nufloat.h"
#include <string.h>

void refpack_init();
extern EdRegistry theRegistry;

static inline void CloneMessageData(NetMessage &message) {
    NetMessage::MessageData *new_data = NULL;
    for (i32 i = 0; i < 512; ++i) {
        if (NetMessage::sm_poolMessageData[i].references == 0) {
            new_data = &NetMessage::sm_poolMessageData[i];
            new_data->references = 1;
            break;
        }
    }

    if (message.data != NULL) {
        memmove(new_data->bytes + message.read_offset, message.data->bytes + message.read_offset,
                message.write_offset - message.read_offset);
    } else {
        NetMessage::RaiseError();
    }

    if (message.data != NULL) {
        if (message.data->references > 1) {
            --message.data->references;
        } else {
            message.data->references = 0;
        }
    }
    message.data = new_data;
}

void TTNetwork::Broadcast(NetMessage message, unsigned char channel) {
    if (message.data == NULL || static_cast<i32>(message.write_offset - message.read_offset) <= 0) {
        return;
    }

    message.data->bytes[--message.read_offset] = channel;
    StatsSendMessage(message, channel);

    NetPeer *peer = V2SessionManager::mpSessionManager->first_peer;
    while (peer != NULL) {
        CloneMessageData(message);
        peer->vtable->send(peer, message);
        peer = peer->next;
    }
}

void TTNetwork::ClearMyHostAddress() {
    has_my_host_address = false;
}

void TTNetwork::Display(ThingRenderData *) {
    if (field_2150 == 0) {
        return;
    }

    NetSmallStats *stats = NULL;
    if (field_2150 == 1) {
        stats = &theNuNetEmu.packet_stats;
    } else if (field_2150 == 2) {
        stats = &theNuNetEmu.raw_stats;
    } else if (field_2150 == 3) {
        stats = &reinterpret_cast<V2SessionManager *>(session)->stats;
    } else {
        i32 display_index = 4;
        NetPeer *peer = reinterpret_cast<V2SessionManager *>(session)->first_peer;
        while (peer != NULL) {
            if (field_2150 == display_index) {
                stats = &peer->stats;
                break;
            }
            ++display_index;
            peer = peer->next;
        }

        if (stats == NULL) {
            for (i32 i = 0; i < theRegistry.class_count; ++i) {
                EdClass *object_class = theRegistry.GetClass(i);
                if (object_class->interface != NULL) {
                    if (field_2150 == display_index) {
                        stats = network_objects.class_stats[i];
                        ++display_index;
                        break;
                    }
                    ++display_index;
                }
            }
        }

        if (stats == NULL) {
            NetListenerBinding *binding = first_listener;
            while (binding != NULL) {
                if (field_2150 == display_index) {
                    stats = &binding->stats;
                    break;
                }
                ++display_index;
                binding = binding->next;
            }
        }

        if (stats == NULL) {
            field_2150 = 1;
            return;
        }
    }

    stats->Draw(field_2154, field_2158, field_215c, field_2160, static_cast<NetSmallStats::eInfo>(0));
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

void TTNetwork::ProcessEvenWhenPaused(ThingProcessData *data) {
    if (field_2140 == 0) {
        return;
    }

    Update();
    if (field_2150 == 0) {
        return;
    }

    nupad_s *pad0 = data->pads[0];
    if (pad0 != NULL && (pad0->digital_buttons_pressed & 4) != 0) {
        ++field_2150;
    }
    nupad_s *pad1 = data->pads[1];
    if (pad1 != NULL && (pad1->digital_buttons_pressed & 4) != 0) {
        ++field_2150;
    }
    if (pad0 != NULL && (pad0->digital_buttons_pressed & 8) != 0) {
        --field_2150;
    }
    if (pad1 != NULL && (pad1->digital_buttons_pressed & 8) != 0) {
        --field_2150;
    }
    if (field_2150 <= 0) {
        field_2150 = 1;
    }
}

void TTNetwork::ReliableBroadcast(NetMessage message, unsigned char channel) {
    if (message.data == NULL || static_cast<i32>(message.write_offset - message.read_offset) <= 0) {
        return;
    }

    message.data->bytes[--message.read_offset] = channel;
    StatsSendMessage(message, channel);

    NetPeer *peer = V2SessionManager::mpSessionManager->first_peer;
    while (peer != NULL) {
        CloneMessageData(message);
        peer->vtable->reliable_send(peer, message, NULL, 0);
        peer = peer->next;
    }
}

void TTNetwork::ReliableSend(NetMessage message, unsigned char channel, NetPeer &peer, char const *name, u32 value) {
    if (message.data == NULL || static_cast<i32>(message.write_offset - message.read_offset) <= 0) {
        return;
    }

    message.data->bytes[--message.read_offset] = channel;
    StatsSendMessage(message, channel);
    peer.vtable->reliable_send(&peer, message, name, value);
}

void TTNetwork::Resume() {
}

void TTNetwork::Send(NetMessage message, unsigned char channel, NetPeer &peer) {
    if (message.data == NULL || static_cast<i32>(message.write_offset - message.read_offset) <= 0) {
        return;
    }

    message.data->bytes[--message.read_offset] = channel;
    StatsSendMessage(message, channel);
    peer.vtable->send(&peer, message);
}

void TTNetwork::SetMyHostAddress(NetAddress const &address) {
    has_my_host_address = true;
    my_host_address = address;
}

void TTNetwork::Shutdown() {
    if (field_2140 != 0) {
        Suspend();
        network_objects.Term();
        ftp_manager.Term();
        void **vtable = *reinterpret_cast<void ***>(session);
        typedef void (*SessionCloseFn)(NetSession *);
        reinterpret_cast<SessionCloseFn>(vtable[4])(session);
        field_2140 = 0;
    }
}

bool TTNetwork::Suspend() {
    return true;
}

TTNetwork::TTNetwork() : field_20(0), field_24(0), field_2140(0), my_address(), my_host_address() {
    field_2158 = -0.3f;
    field_215c = 0.5f;
    has_my_host_address = 0;
    field_213c = 0;
    field_2150 = 0;
    field_2154 = 0.3f;
    field_2160 = 0.3f;
}

char const *TTNetwork::GetName() {
    return "Network";
}

void TTNetwork::Update() {
    if (field_2140 != 0) {
        ++field_2134;
        UtilFrameStart();
        void **vtable = *reinterpret_cast<void ***>(session);
        typedef void (*SessionUpdateFn)(NetSession *);
        reinterpret_cast<SessionUpdateFn>(vtable[2])(session);
        ftp_manager.Update();
        network_objects.Update();
        theNuNetEmu.Update();
        StatsUpdate();
    }
}

TTNetwork::~TTNetwork() {
}
