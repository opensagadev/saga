#include "decomp.h"
#include "gamelib_util_types.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"

#include <stdarg.h>
#include <stdio.h>

extern "C" void NuDisableVBlankE(void);
extern "C" void NuEnableVBlankE(void);
extern TTNetwork theNetwork;

i32 V2SessionManager::mLogHandle;
char V2SessionManager::mLogFilename[32];
V2SessionManager *V2SessionManager::mpSessionManager;

void V2SessionManager::Log(char *format, ...) {
    char text[128];
    va_list arguments;

    NuDisableVBlankE();
    va_start(arguments, format);
    vsnprintf(text, 126, format, arguments);
    va_end(arguments);
    NuEnableVBlankE();
    NuStrCat(text, const_cast<char *>("\r\n"));
    printf(text);
}

void V2SessionManager::RemoveAllPeers(ePeerLeftReason reason) {
    Log(const_cast<char *>("V2SessionManager::RemoveAllPeers() called"));
    NetPeer *peer = first_peer;
    while (peer != NULL) {
        NetPeer *next = peer->next;
        RemovePeer(peer, reason);
        peer = next;
    }
    if (local_peer != NULL) {
        RemovePeer(local_peer, reason);
    }
}

void V2SessionManager::RemovePeer(NetPeer *peer, ePeerLeftReason reason) {
    theNetwork.PeerLeft(*peer, reason);
    peer->vtable->disconnect(peer);
    if (peer == local_peer) {
        return;
    }

    if (peer->next != NULL) {
        peer->next->previous = peer->previous;
    } else {
        last_peer = peer->previous;
    }
    if (peer->previous != NULL) {
        peer->previous->next = peer->next;
    } else {
        first_peer = peer->next;
    }
    peer->next = NULL;
    peer->previous = NULL;
    --peer_count;
    peer->vtable->destroy(peer);
}

void V2SessionManager::Reset() {
    if (field_90 != 0) {
        puts("mHostList");
    }
    if (peer_count != 0) {
        puts("mPeerList");
    }

    field_6c = 0;
    field_68 = 0;
    field_74 = 0;
    field_04 = 0;
    field_34 = 0;
    fields_44[0] = -1;
    fields_44[1] = -1;
    fields_44[2] = -1;
    fields_44[3] = -1;
    fields_44[4] = -1;
    fields_44[5] = -1;
    fields_44[6] = -1;
    fields_44[7] = -1;
    fields_44[8] = -1;
}

void V2SessionManager::SetHostGameData(i32 *data, i32 count) {
    const i32 copy_count = count <= 8 ? count : 8;
    for (i32 i = 0; i < copy_count; ++i) {
        fields_44[i + 1] = data[i];
    }
}

void V2SessionManager::Update() {
    stats.total.Reset();
    for (NetPeer *peer = first_peer; peer != NULL; peer = peer->next) {
        stats.total += peer->stats.total;
    }
    stats.Update();
}

V2SessionManager::V2SessionManager(char *new_name) : stats("SessionAll") {
    field_88 = 0;
    field_8c = 0;
    field_90 = 0;
    first_peer = NULL;
    last_peer = NULL;
    peer_count = 0;
    field_7c = 0;
    field_40 = 2;
    num_local_players = 0;
    mpSessionManager = this;
    NuStrCpy(name, new_name);
    field_78 = 0;
}

i32 V2SessionManager::VerifyStrings(char **destination, char **source, i32 count, char *replacement) {
    i32 changed = 0;
    for (i32 i = 0; i < count; ++i) {
        changed |=
            NuStringFilterBadWords(reinterpret_cast<NUWCHAR8 *>(destination[i]),
                                   reinterpret_cast<NUWCHAR8 *>(source[i]), reinterpret_cast<NUWCHAR8 *>(replacement));
    }
    return changed;
}
