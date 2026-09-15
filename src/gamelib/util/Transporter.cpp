#include "decomp.h"
#include "gamelib_util_types.h"

void NetTransporter::AddListener(NetListenerInterface *, unsigned char, char *) {
    STUBBED();
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

void NetTransporter::NosAcquire(NetworkObject *, NetPeer const &) const {
    STUBBED();
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

void NetTransporter::RemoveListener(NetListenerInterface *, unsigned char) {
    STUBBED();
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
