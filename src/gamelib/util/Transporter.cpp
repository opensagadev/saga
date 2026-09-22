#include "decomp.h"
#include "gamelib_util_types.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include <new>

extern MemoryManager theMemoryManager;

NetListenerBinding::NetListenerBinding(NetListenerInterface *new_listener, unsigned char new_channel, char *new_name)
    : next(NULL), previous(NULL), listener(new_listener), channel(new_channel), stats(name) {
    if (new_name != NULL) {
        NuStrNCpy(name, new_name, 0x20);
    } else {
        name[0] = '\0';
    }
}

void NetListenerBinding::operator=(NetListenerBinding const &other) {
    listener = other.listener;
    channel = other.channel;
}

bool NetListenerBinding::operator==(NetListenerBinding const &other) {
    return listener == other.listener && channel == other.channel;
}

NetListenerBinding *NetListenerList::Find(NetListenerBinding *binding) {
    NetListenerBinding *current = first;
    while (current != NULL) {
        if (*current == *binding) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void NetTransporter::AddListener(NetListenerInterface *listener, unsigned char channel, char *name) {
    NetListenerBinding *binding =
        new (theMemoryManager.AllocPool(sizeof(NetListenerBinding), 1)) NetListenerBinding(listener, channel, name);

    if (reinterpret_cast<NetListenerList *>(&first_listener)->Find(binding) != NULL) {
        theMemoryManager.FreePool(binding, sizeof(NetListenerBinding));
        return;
    }

    binding->next = NULL;
    binding->previous = last_listener;
    if (last_listener != NULL) {
        last_listener->next = binding;
    }
    last_listener = binding;
    if (first_listener == NULL) {
        first_listener = binding;
    }
    ++listener_count;
}

void NetTransporter::Distribute(NetMessage const &message, unsigned char channel, NetPeer const &peer) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        if (binding->channel == channel) {
            binding->stats.total.values[1] += message.data != NULL ? message.write_offset - message.read_offset : 0;
            ++binding->stats.total.values[3];
            binding->listener->Receive(message, channel, peer);
        }
    }
}

void NetTransporter::FtpComplete(FtpFile *file, i32 result) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->FtpComplete(file, result);
    }
}

void NetTransporter::FtpDownload(FtpFile *file) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->FtpDownload(file);
    }
}

void NetTransporter::FtpUpload(FtpFile *file) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->FtpUpload(file);
    }
}

i32 NetTransporter::NosAcquire(NetworkObject *object, NetPeer const &peer) const {
    i32 result = 1;
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        if (binding->listener->NosAcquire(object, peer) == 0) {
            result = 0;
        }
    }
    return result;
}

void NetTransporter::NosAdopted(NetworkObject *object, NetPeer const &peer) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->NosAdopted(object, peer);
    }
}

void NetTransporter::PeerDead(NetPeer const &peer) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->PeerDead(peer);
    }
}

void NetTransporter::PeerJoined(NetPeer const &peer) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->PeerJoined(peer);
    }
}

void NetTransporter::PeerLeft(NetPeer const &peer, ePeerLeftReason reason) const {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->listener->PeerLeft(peer, reason);
    }
}

i32 NetTransporter::PeerRequest(NetPeer const &peer) const {
    i32 result = 1;
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        if (binding->listener->PeerRequest(peer) == 0) {
            result = 0;
        }
    }
    return result;
}

void NetTransporter::RemoveListener(NetListenerInterface *listener, unsigned char channel) {
    NetListenerBinding key(listener, channel, NULL);
    NetListenerBinding *binding = reinterpret_cast<NetListenerList *>(&first_listener)->Find(&key);
    if (binding == NULL) {
        return;
    }

    if (binding->next != NULL) {
        binding->next->previous = binding->previous;
    } else {
        last_listener = binding->previous;
    }
    if (binding->previous != NULL) {
        binding->previous->next = binding->next;
    } else {
        first_listener = binding->next;
    }
    binding->next = NULL;
    binding->previous = NULL;
    --listener_count;
    theMemoryManager.FreePool(binding, sizeof(NetListenerBinding));
}

void NetTransporter::StatsReceiveMessage(NetMessage message, unsigned char channel) {
    NetListenerBinding *binding = first_listener;
    while (binding != NULL && binding->channel != channel) {
        binding = binding->next;
    }
    if (binding != NULL) {
        binding->stats.total.values[1] += message.data != NULL ? message.write_offset - message.read_offset : 0;
        ++binding->stats.total.values[3];
    }
}

void NetTransporter::StatsSendMessage(NetMessage message, unsigned char channel) {
    NetListenerBinding *binding = first_listener;
    while (binding != NULL && binding->channel != channel) {
        binding = binding->next;
    }
    if (binding != NULL) {
        binding->stats.total.values[0] += message.data != NULL ? message.write_offset - message.read_offset : 0;
        ++binding->stats.total.values[2];
    }
}

void NetTransporter::StatsUpdate() {
    for (NetListenerBinding *binding = first_listener; binding != NULL; binding = binding->next) {
        binding->stats.Update();
    }
}
