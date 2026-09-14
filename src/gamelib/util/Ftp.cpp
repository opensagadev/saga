#include "decomp.h"
#include <stddef.h>

#include "gamelib_util_types.h"

extern NetTransporter theNetwork;

i32 NetFtpManager::Abort(char const *, NetAddress const &, i32, i32) {
    STUBBED();
    return 0;
}

void NetFtpManager::FindTransfer(char const *, NetAddress const &, i32) {
    STUBBED();
}

void NetFtpManager::FindTransfer(char const *, NetAddress const &, i32) const {
    STUBBED();
}

i32 NetFtpManager::Get(char const *, void *, i32, NetAddress const &) {
    STUBBED();
    return 0;
}

FtpFile const *NetFtpManager::GetTransfers() const {
    return files;
}

void NetFtpManager::Init() {
    STUBBED();
}

NetFtpManager::NetFtpManager() {
    STUBBED();
}

void NetFtpManager::PeerLeft(NetAddress const &, ePeerLeftReason) {
    STUBBED();
}

void NetFtpManager::Receive(NetMessage, unsigned char, NetAddress const &) {
    STUBBED();
}

void NetFtpManager::Reset() {
    FtpFile *file = files;
    FtpFile *end = file + 32;
    NetTransporter *network = &theNetwork;
    do {
        if (file->transfer != NULL) {
            network->FtpComplete(file, static_cast<i32>(0xa0200001));
            file->Term();
            if (file->transfer != NULL && file->network_object != NULL) {
                if (file->network_object->reference_count > 1) {
                    --file->network_object->reference_count;
                } else {
                    file->network_object->reference_count = 0;
                }
            }
            file->transfer = NULL;
        }
        ++file;
    } while (file != end);
}

i32 NetFtpManager::Send(char const *, void const *, i32, NetAddress const &) {
    STUBBED();
    return 0;
}

void NetFtpManager::Term() {
    STUBBED();
}

void NetFtpManager::Update() {
    STUBBED();
}

NetFtpManager::~NetFtpManager() {
}

void FtpFile::Accept(i32, void *) {
    STUBBED();
}

void FtpFile::Accept(i32) {
    STUBBED();
}

i32 FtpFile::Accept() {
    accepted = true;
    return 0;
}

void FtpFile::Init(i32, char const *, i32, NetAddress const &, void *, i32) {
    STUBBED();
}

void FtpFile::RecvData(NetMessage &) {
    STUBBED();
}

void FtpFile::SendData() {
    STUBBED();
}

void FtpFile::Term() {
    STUBBED();
}

void FtpFile::Update() {
    STUBBED();
}
