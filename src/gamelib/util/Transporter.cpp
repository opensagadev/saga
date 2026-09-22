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

void NetTransporter::Distribute(NetMessage const &, unsigned char, NetPeer const &) const {
    STUBBED();
}

void NetTransporter::FtpComplete(FtpFile *, i32) const {
    STUBBED();
}

void NetTransporter::FtpDownload(FtpFile *) const {
    STUBBED();
}

void NetTransporter::FtpUpload(FtpFile *) const {
    STUBBED();
}

i32 NetTransporter::NosAcquire(NetworkObject *, NetPeer const &) const {
    STUBBED();
    return 0;
}

void NetTransporter::NosAdopted(NetworkObject *, NetPeer const &) const {
    STUBBED();
}

void NetTransporter::PeerDead(NetPeer const &) const {
    STUBBED();
}

void NetTransporter::PeerJoined(NetPeer const &) const {
    STUBBED();
}

void NetTransporter::PeerLeft(NetPeer const &, ePeerLeftReason) const {
    STUBBED();
}

void NetTransporter::PeerRequest(NetPeer const &) const {
    STUBBED();
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

void NetTransporter::StatsReceiveMessage(NetMessage, unsigned char) {
    STUBBED();
}

void NetTransporter::StatsSendMessage(NetMessage, unsigned char) {
    STUBBED();
}

void NetTransporter::StatsUpdate() {
    STUBBED();
}
