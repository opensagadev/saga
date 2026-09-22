#include "decomp.h"
#include <stddef.h>
#include <string.h>

#include "gamelib_util_types.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"

extern TTNetwork theNetwork;

i32 NetFtpManager::Abort(char const *, NetAddress const &, i32, i32) {
    return 0;
}

FtpFile *NetFtpManager::FindTransfer(char const *name, NetAddress const &address, i32 direction) {
    FtpFile *file = files;
    FtpFile *end = files + 32;
    if (direction != 0) {
        do {
            if (file->transfer != NULL && memcmp(&address, &file->address, sizeof(NetAddress)) == 0 &&
                NuStrICmp(file->name, name) == 0 && file->direction == direction) {
                return file;
            }
            ++file;
        } while (file != end);
    } else {
        do {
            if (file->transfer != NULL && memcmp(&address, &file->address, sizeof(NetAddress)) == 0 &&
                NuStrICmp(file->name, name) == 0) {
                return file;
            }
            ++file;
        } while (file != end);
    }
    return NULL;
}

FtpFile const *NetFtpManager::FindTransfer(char const *name, NetAddress const &address, i32 direction) const {
    FtpFile const *file = files;
    FtpFile const *end = files + 32;
    if (direction != 0) {
        do {
            if (file->transfer != NULL && memcmp(&address, &file->address, sizeof(NetAddress)) == 0 &&
                NuStrICmp(file->name, name) == 0 && file->direction == direction) {
                return file;
            }
            ++file;
        } while (file != end);
    } else {
        do {
            if (file->transfer != NULL && memcmp(&address, &file->address, sizeof(NetAddress)) == 0 &&
                NuStrICmp(file->name, name) == 0) {
                return file;
            }
            ++file;
        } while (file != end);
    }
    return NULL;
}

i32 NetFtpManager::Get(char const *, void *, i32, NetAddress const &) {
    return 0;
}

FtpFile const *NetFtpManager::GetTransfers() const {
    return files;
}

void NetFtpManager::Init() {
    theNetwork.AddListener(this, 2, const_cast<char *>("FTP"));
}

NetFtpManager::NetFtpManager() {
    void **transfer = &files[0].transfer;
    void **end = reinterpret_cast<void **>(reinterpret_cast<u8 *>(transfer) + sizeof(files));
    do {
        *transfer = NULL;
        transfer = reinterpret_cast<void **>(reinterpret_cast<u8 *>(transfer) + sizeof(FtpFile));
    } while (transfer != end);
    field_1604 = 0;
}

void NetFtpManager::PeerLeft(NetAddress const &address, ePeerLeftReason) {
    for (i32 i = 0; i < 32; ++i) {
        FtpFile *file = &files[i];
        if (file->transfer != NULL && memcmp(&address, &file->address, sizeof(NetAddress)) == 0) {
            theNetwork.FtpComplete(file, static_cast<i32>(0xa0200001));
            file->Term();
            if (file->transfer != NULL && file->message_data != NULL) {
                if (file->message_data->reference_count > 1) {
                    --file->message_data->reference_count;
                } else {
                    file->message_data->reference_count = 0;
                }
            }
            file->transfer = NULL;
        }
    }
}

void NetFtpManager::Receive(NetMessage, unsigned char, NetAddress const &) {
}

void NetFtpManager::Reset() {
    FtpFile *file = files;
    FtpFile *end = file + 32;
    NetTransporter *network = &theNetwork;
    do {
        if (file->transfer != NULL) {
            network->FtpComplete(file, static_cast<i32>(0xa0200001));
            file->Term();
            if (file->transfer != NULL && file->message_data != NULL) {
                if (file->message_data->reference_count > 1) {
                    --file->message_data->reference_count;
                } else {
                    file->message_data->reference_count = 0;
                }
            }
            file->transfer = NULL;
        }
        ++file;
    } while (file != end);
}

i32 NetFtpManager::Send(char const *, void const *, i32, NetAddress const &) {
    return 0;
}

void NetFtpManager::Term() {
    Reset();
    theNetwork.RemoveListener(this, 2);
}

void NetFtpManager::Update() {
}

NetFtpManager::~NetFtpManager() {
    for (i32 i = 0; i < 32; ++i) {
        FtpFile *file = &files[i];
        if (file->transfer != NULL) {
            if (file->message_data != NULL) {
                if (file->message_data->reference_count > 1) {
                    --file->message_data->reference_count;
                } else {
                    file->message_data->reference_count = 0;
                }
            }
            file->transfer = NULL;
        }
    }
}

i32 FtpFile::Accept(i32 new_capacity, void *new_buffer) {
    switch (direction) {
        case 1:
            accepted = 1;
            buffer = new_buffer;
            capacity = new_capacity;
            size = new_capacity;
            return 0;
        case 2:
            if (size > capacity) {
                return static_cast<i32>(0xa0210001);
            }
            accepted = 1;
            buffer = new_buffer;
            capacity = new_capacity;
            return 0;
        default:
            return static_cast<i32>(0xa0210000);
    }
}

i32 FtpFile::Accept(i32 new_size) {
    if (capacity < new_size) {
        return static_cast<i32>(0xa0210001);
    }
    size = new_size;
    accepted = 1;
    return 0;
}

i32 FtpFile::Accept() {
    accepted = true;
    return 0;
}

void FtpFile::Init(i32 new_direction, char const *new_name, i32 new_size, NetAddress const &new_address,
                   void *new_buffer, i32 new_capacity) {
    address = new_address;
    direction = new_direction;
    accepted = 0;
    NuStrCpy(name, new_name);
    size = new_size;
    buffer = new_buffer;
    capacity = new_capacity;
    transferred = 0;
}

void FtpFile::RecvData(NetMessage &) {
}

void FtpFile::SendData() {
}

void FtpFile::Term() {
}

void FtpFile::Update() {
    if (direction != 1 || message_data == NULL || message_data->reference_count != 1 || accepted != 1) {
        return;
    }

    NetMessage::MessageData *new_data = NULL;
    for (i32 i = 0; i < 512; ++i) {
        if (NetMessage::sm_poolMessageData[i].references == 0) {
            new_data = &NetMessage::sm_poolMessageData[i];
            new_data->references = 1;
            break;
        }
    }

    if (message_data->reference_count > 1) {
        --message_data->reference_count;
    } else {
        message_data->reference_count = 0;
    }
    message_data = reinterpret_cast<TransferReference *>(new_data);
    message_read_offset = 0xc;
    message_write_offset = 0xc;

    if (size - transferred > 0) {
        SendData();
    }
}
