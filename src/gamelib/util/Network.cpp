#include "decomp.h"
#include "gamelib_util_types.h"
#include "gamelib/util/Utilities.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/legoapi_types.h"
#include <string.h>
#include <new>
NetSession *theSession;
NetworkObjectManager *theNos;
i16 NetReplicator::smNextId;
i16 NetChangedReplicator::mTableInited;
i16 NetChangedReplicator::mCrc32Table[256];
static i32 ForceDummySerialise;

extern EdRegistry theRegistry;
TTNetwork theNetwork;
extern MemoryManager theMemoryManager;

static __used__ i32 NOSGetGuid() {
    return theNos->GetNextGuid();
}

void NetRotator2::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **,
                               float *, i32) {
    STUBBED();
}

bool NetPredictor::AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) {
    STUBBED();
    return false;
}

void NetPredictor::CheckPredictionError(EdClass const *, void *, float *, float *, i32) {
    STUBBED();
}

void NetPredictor::DoPrediction(EdClass const *, void *, ReplicatorData &, NetPredictor::PredictorTime *, i32) {
    STUBBED();
}

i32 NetPredictor::DoPrediction(EdClass const *, void *, ReplicatorData &, i32) {
    STUBBED();
    return 0;
}

void NetPredictor::SerialiseObject(EdStream &, NetPeer *, EdClass const *, void *, ReplicatorData &,
                                   NetPredictor::PredictorTime *, i16 *) {
    STUBBED();
}

i32 NetPredictor::SerialiseObject(EdStream &, NetPeer *, EdClass const *, void *, ReplicatorData &, i16 *) {
    STUBBED();
    return 0;
}

void NetPredictor::StoreSampleData(EdClass const *, void *, NetPredictor::PredictorTime *,
                                   NetPredictor::PredictorData **, float *, i32) {
    STUBBED();
}

void NetPredictor2::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **,
                                 float *, i32) {
    STUBBED();
}

void NetPredictor3::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **,
                                 float *, i32) {
    STUBBED();
}

NetReplicator::NetReplicator(i32 group, float minimum_seconds, float maximum_seconds) {
    next = 0;
    previous = 0;
    minimum_interval = minimum_seconds < 0.1f ? 100 : static_cast<u32>(minimum_seconds * 1000.0f);
    if (maximum_seconds > 0.0f && maximum_seconds < 0.1f) {
        maximum_interval = 100;
    } else {
        maximum_interval = static_cast<u32>(maximum_seconds * 1000.0f);
    }
    data_size = 0;
    message_size = 0;
    id = smNextId++;
    replication_group = static_cast<u16>(group);
}

i32 NetReplicator::SerialiseObject(EdStream &stream, NetPeer *peer, EdClass const *object_class, void *object,
                                   ReplicatorData &data, i16 *class_mapping) {
    u8 member_data[256] = {};
    for (EdRef *member = object_class->members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(object);
            if (ForceDummySerialise && member_object == NULL) {
                member_object = object;
            }
            if (member_class->SerialiseObjectHeader(stream, member_object)) {
                SerialiseObject(stream, peer, member_class, member_object, data, NULL);
            }
        } else if (member->replication_group == id) {
            EdType *type = theRegistry.GetType(member->type_id);
            i32 size = member->size;
            if (size <= 0) {
                size = type->size;
            }
            if (object != NULL) {
                if (stream.mode == 2) {
                    member->GetMemberData(object, member->type_id, member_data, sizeof(member_data));
                }
                type->serialise(stream, member_data, size);
                if (stream.mode == 1) {
                    member->SetMemberData(object, member->type_id, member_data, sizeof(member_data), class_mapping);
                }
            } else {
                stream.Eat(size, 1);
            }
        }
    }
    return 1;
}

i32 NetReplicator::DoPrediction(EdClass const *, void *, ReplicatorData &, i32) {
    return 0;
}

void NetworkObject::Destroy() {
    i32 class_id = theRegistry.GetClassId(object_class);
    i32 data_size = theNetwork.network_objects.replicator_data_sizes[class_id];
    if (data_size > 0) {
        theMemoryManager.FreePool(replicator_data, static_cast<u32>(data_size));
        replicator_data = NULL;
    }
    id = 0;
    object = NULL;
    object_class = NULL;
    owner = NULL;
    flags = 0;
}

void NetworkObject::Initialise(i32 guid, void *new_object, EdClass *new_class, NetPeer const &new_owner,
                               i32 new_flags) {
    id = static_cast<i16>(guid);
    object = new_object;
    object_class = new_class;
    owner = &new_owner;
    flags |= static_cast<u16>(new_flags);

    i32 data_size = theNetwork.network_objects.replicator_data_sizes[theRegistry.GetClassId(new_class)];
    if (data_size > 0) {
        replicator_data = theMemoryManager.AllocPool(static_cast<u32>(data_size), 1);
    }
}

void NetListenerList::Find(NetListenerBinding *) {
    STUBBED();
}

bool NetConstReplicator::AllowPush(EdClass const *, void const *, ReplicatorData &data, i32 force, i32) {
    u32 *last_push = reinterpret_cast<u32 *>((reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u);
    data.cursor = reinterpret_cast<u8 *>(last_push + 1);

    u32 now = UtilGetFrameStartTime();
    if (force == 0 && *last_push != 0) {
        return false;
    }
    *last_push = now;
    return true;
}

NetListenerBinding::NetListenerBinding(NetListenerInterface *, unsigned char, char *) {
    STUBBED();
}

void NetListenerBinding::operator=(NetListenerBinding const &) {
    STUBBED();
}

void NetListenerBinding::operator==(NetListenerBinding const &) {
    STUBBED();
}

bool NetSimpleReplicator::AllowPush(EdClass const *, void const *, ReplicatorData &data, i32 force, i32) {
    u32 *last_push = reinterpret_cast<u32 *>((reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u);
    data.cursor = reinterpret_cast<u8 *>(last_push + 1);

    u32 now = UtilGetFrameStartTime();
    if (force != 0 || now - *last_push > minimum_interval) {
        *last_push = now;
        return true;
    }
    return false;
}

bool NetChangedReplicator::AllowPush(EdClass const *object_class, void const *object, ReplicatorData &data, i32 force,
                                     i32 skip_checksum) {
    u32 checksum = 0xffffffffu;
    u32 *last_push = reinterpret_cast<u32 *>((reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u);
    u32 *last_checksum = last_push + 1;
    data.cursor = reinterpret_cast<u8 *>(last_checksum + 1);

    u32 now = UtilGetFrameStartTime();
    u32 elapsed = now - *last_push;
    if (force != 0) {
        *last_push = now;
        return true;
    }
    if (maximum_interval != 0 && elapsed > maximum_interval) {
        *last_push = now;
        return true;
    }
    if (elapsed <= minimum_interval || skip_checksum != 0) {
        return false;
    }

    CheckSumObject(object_class, object, checksum);
    if (*last_checksum == checksum) {
        return false;
    }
    *last_checksum = checksum;
    *last_push = now;
    return true;
}

void NetChangedReplicator::CheckSum(unsigned char const *bytes, u32 size, u32 &checksum) const {
    u32 value = checksum;
    for (u32 i = 0; i < size; i++) {
        value = static_cast<i32>(mCrc32Table[static_cast<u8>(value ^ bytes[i])]) ^ (value >> 8);
        checksum = value;
    }
    checksum = ~value;
}

void NetChangedReplicator::CheckSumObject(EdClass const *object_class, void const *object, u32 &checksum) const {
    EdRef *member = object_class->members;
    while (member != NULL) {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(const_cast<void *>(object));
            if (member_object != NULL) {
                CheckSumObject(member_class, member_object, checksum);
            }
        } else if (static_cast<u16>(member->replication_group) == replication_group) {
            EdType *type = theRegistry.GetType(member->type_id);
            i32 size = member->size > 0 ? member->size : type->size;
            u8 data[256];
            member->GetMemberData(const_cast<void *>(object), member->type_id, data, sizeof(data));
            CheckSum(data, static_cast<u32>(size), checksum);
        }
        member = member->next;
    }
}

void NetChangedReplicator::InitTable() {
    if (mTableInited != 0) {
        return;
    }
    mTableInited = 1;

    for (u32 i = 0; i < 256; i++) {
        u32 value = i;
        for (i32 bit = 0; bit < 8; bit++) {
            if ((value & 1) != 0) {
                value = (value >> 1) ^ 0xedb88320u;
            } else {
                value >>= 1;
            }
        }
        mCrc32Table[i] = static_cast<i16>(value);
    }
}

void NetworkObjectManager::Acquire(i32) {
    STUBBED();
}

void NetworkObjectManager::AddToLocalObjectList(NetworkObject *object) {
    NetworkObject **slot = local_objects;
    if (local_object_count <= 0) {
        local_object_count = 1;
    } else if (*slot != NULL) {
        i32 index = 0;
        do {
            index++;
            slot++;
            if (index == local_object_count) {
                local_object_count = static_cast<i32>(slot - local_objects) + 1;
                break;
            }
        } while (*slot != NULL);
    }
    *slot = object;
}

void NetworkObjectManager::BindFilter(NOSFilter *filter, EdClass const *object_class) {
    filters[theRegistry.GetClassId(const_cast<EdClass *>(object_class))] = filter;
}

void NetworkObjectManager::BindReplicator(NetReplicator *replicator, EdClass const *object_class) {
    i32 class_id = theRegistry.GetClassId(const_cast<EdClass *>(object_class));
    ReplicatorList &list = replicators[class_id];
    replicator->next = NULL;
    replicator->previous = list.tail;
    if (list.tail != NULL) {
        list.tail->next = replicator;
    }
    list.tail = replicator;
    if (list.head == NULL) {
        list.head = replicator;
    }
    list.count++;

    i32 data_size;
    i32 message_size;
    CalcReplicatorDataSize(replicator, object_class, data_size, message_size);
    replicator_data_sizes[class_id] += data_size;
    replicator->data_size = static_cast<u16>(data_size);
    replicator->message_size = static_cast<u16>(message_size);
}

void NetworkObjectManager::CalcReplicatorDataSize(NetReplicator *, EdClass const *, i32 &, i32 &) {
    STUBBED();
}

void NetworkObjectManager::ChangeContext(NOSContext &new_context) {
    memmove(&context, &new_context, sizeof(context));
}

void NetworkObjectManager::ConstructObject(NetworkObject *object, NetworkObjectManager::NetPeerPush *peer_push) {
    u8 constructor_data[256];
    EdClassInterface *interface = object->object_class->interface;
    i16 size = static_cast<i16>(
        interface->vtable->get_constructor_data(interface, object->object, constructor_data, sizeof(constructor_data)));
    i16 class_id = static_cast<i16>(theRegistry.GetClassId(object->object_class));
    NetMessage *message = peer_push->GetReliableMessage(size + 10);
    message->Write8(1);
    message->Write16(object->id);
    message->Write16(class_id);
    message->Write16(size);
    if (size > 0) {
        message->Write(constructor_data, size);
    }
}

void NetworkObjectManager::ContinuityBreak(i32, float) {
    STUBBED();
}

NetworkObject *NetworkObjectManager::FindNetworkObject(void *object) {
    if (object == NULL) {
        return NULL;
    }
    for (i32 i = 0; i < 2048; i++) {
        if (objects[i].id != 0 && objects[i].object == object) {
            return &objects[i];
        }
    }
    return NULL;
}

void NetworkObjectManager::FlushObjects(i32) {
    STUBBED();
}

i32 NetworkObjectManager::GetNextGuid() {
    if (guid_group < 0) {
        return 0;
    }

    i32 group_start = guid_group << 10;
    i32 group_end = (guid_group + 1) << 10;
    if (next_guid < 0) {
        next_guid = group_start == 0 ? 0 : group_start - 1;
    }

    i32 attempts = 0;
    do {
        next_guid++;
        attempts++;
        if (next_guid >= group_end) {
            next_guid = group_start == 0 ? 1 : group_start;
        }
        if (objects[next_guid].object == NULL) {
            return next_guid;
        }
    } while (attempts != 1024);
    return 0;
}

void *NetworkObjectManager::GetObject(i32 id) {
    if (id < 1 || id > 2048) {
        return NULL;
    }
    return objects[id].object;
}

i32 NetworkObjectManager::GetPeerStatus() {
    i32 status = 0;
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer != NULL && peer_push[i].stage != 3) {
            if (status == 0 && (peer_push[i].stage == 1 || peer_push[i].stage == 2)) {
                status = 1;
            } else {
                status = 2;
            }
        }
    }
    return status;
}

void NetworkObjectManager::ImportObjects() {
    STUBBED();
}

void NetworkObjectManager::Init() {
    STUBBED();
}

void NetworkObjectManager::InitClassStats() {
    for (i32 i = 0; i < theRegistry.class_count; i++) {
        EdClass *object_class = theRegistry.GetClass(i);
        if (object_class->interface != NULL) {
            class_stats[i] = new NetStats(object_class->name);
        }
    }
}

i32 NetworkObjectManager::IsLocal(i32 id) {
    if (id == 0) {
        return 1;
    }
    NetworkObject *network_object = FindNetworkObject(id);
    if (network_object == NULL) {
        return 1;
    }
    return network_object->owner->local;
}

i32 NetworkObjectManager::IsPeerReady(NetPeer const &peer) const {
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer == &peer) {
            return peer_push[i].stage == 3;
        }
    }
    return 0;
}

i32 NetworkObjectManager::IsPeerStarted(NetPeer const &peer) const {
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer == &peer) {
            return peer_push[i].stage > 0;
        }
    }
    return 0;
}

NetworkObjectManager::NetworkObjectManager() {
    context.words[0] = 0;
    context.words[1] = 0;
    context.words[2] = 0;
    context.words[3] = 0;
    ReplicatorList *replicator = replicators;
    ReplicatorList *end = replicators + 64;
    do {
        replicator->head = NULL;
        replicator->tail = NULL;
        replicator->count = 0;
        ++replicator;
    } while (replicator != end);
    active = 0;
    guid_group = -1;
    next_guid = -1;
    theNos = this;
    theRegistry.create_object_guid = NOSGetGuid;
}

void NetworkObjectManager::NotifyCreateObject(void *object, EdClass *object_class, void *, i32, i32 guid, i32 flags) {
    if ((flags & 1) == 0) {
        RegisterObject(object, object_class, guid);
    }
}

void NetworkObjectManager::NotifyDestroyObject(void *object, EdClass *object_class, i32 guid, i32 flags) {
    if ((flags & 1) == 0) {
        ReleaseObject(object, object_class, guid);
    }
}

void NetworkObjectManager::ObjectCall(void *, i32, NetMessage, NetPeer const *) {
    STUBBED();
}

void NetworkObjectManager::ObjectOtherCall(void *, i32, NetMessage) {
    STUBBED();
}

void NetworkObjectManager::ObjectOwnerCall(void *, i32, NetMessage) {
    STUBBED();
}

NetPeer const *NetworkObjectManager::Owner(i32 id) {
    if (id == 0) {
        return NULL;
    }
    NetworkObject *network_object = FindNetworkObject(id);
    if (network_object == NULL) {
        return NULL;
    }
    return network_object->owner;
}

void NetworkObjectManager::PeerJoined(NetPeer const &peer) {
    if (theSession->status == 3 || theSession->status == 4) {
        if (!peer.local) {
            for (i32 i = 0; i < 2; i++) {
                if (guid_peers[i] == NULL) {
                    guid_peers[i] = &peer;
                    NetMessage message;
                    message.Write8(0);
                    message.Write32(i);
                    theNetwork.ReliableSend(message, 3, const_cast<NetPeer &>(peer), NULL, 0);
                    break;
                }
            }
        }
    }
    if (peer.local) {
        return;
    }
    if (active != 0) {
        NetMessage message;
        message.Write8(9);
        message.Write(&context, sizeof(context));
        theNetwork.ReliableSend(message, 3, const_cast<NetPeer &>(peer), NULL, 0);
    }
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer == NULL) {
            peer_push[i].peer = &peer;
            peer_push[i].Stop();
            break;
        }
    }
}

void NetworkObjectManager::PeerLeft(NetPeer const &peer, ePeerLeftReason) {
    if (!peer.local && (theSession->status == 3 || theSession->status == 4)) {
        for (i32 i = 0; i < 2; i++) {
            if (guid_peers[i] == &peer) {
                guid_peers[i] = NULL;
            }
        }
        for (i32 i = 0; i < 2048; i++) {
            if (objects[i].id != 0 && objects[i].owner == &peer) {
                Recover(&objects[i]);
            }
        }
    }
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer != NULL && peer_push[i].peer == &peer) {
            peer_push[i].peer = NULL;
            peer_push[i].Stop();
        }
    }
}

void NetworkObjectManager::Push(NetworkObject const *, NetReplicator *, ReplicatorData &,
                                NetworkObjectManager::NetPeerPush *) {
    STUBBED();
}

void NetworkObjectManager::PushObject(NetworkObject *, NetworkObjectManager::NetPeerPush *, i32) {
    STUBBED();
}

void NetworkObjectManager::Receive(NetMessage, unsigned char, NetPeer const &) {
    STUBBED();
}

void NetworkObjectManager::ReceiveAcquireMessage(NetMessage &message, NetPeer const &peer) {
    i16 id;
    message.Read16(id);
    NetworkObject *object = &objects[id];
    if (object->id == id && object->owner->local != 0 && theNetwork.NosAcquire(object, peer) == 1) {
        RemoveFromLocalObjectList(object);
        objects[id].owner = &peer;
        objects[id].flags |= 0x20;
        theNetwork.NosAdopted(&objects[id], peer);
        SendAcquiredMessage(id, peer);
    }
}

void NetworkObjectManager::ReceiveAcquiredMessage(NetMessage &message, NetPeer const &) {
    i16 id;
    message.Read16(id);
    objects[id].owner = theSession->local_peer;
    AddToLocalObjectList(&objects[id]);
    SendAdoptedMessage(id);
    theNetwork.NosAdopted(&objects[id], *objects[id].owner);
    RemovePendingObject(&objects[id]);
}

void NetworkObjectManager::ReceiveAdoptedMessage(NetMessage &message, NetPeer const &peer) {
    i16 id;
    message.Read16(id);
    objects[id].flags &= ~0x20;
    if (objects[id].owner != &peer) {
        objects[id].owner = &peer;
        theNetwork.NosAdopted(&objects[id], peer);
    }
}

void NetworkObjectManager::ReceiveConstructorMessage(NetMessage &message, NetPeer const &peer) {
    i16 guid;
    i16 class_id;
    i16 size;
    u8 constructor_data[256];
    message.Read16(guid);
    message.Read16(class_id);
    message.Read16(size);
    if (size > 0) {
        message.Read(constructor_data, size);
    }
    EdClass *object_class = theRegistry.GetClass(class_id);
    NetworkObject *network_object = &objects[guid];
    if (network_object->object == NULL) {
        void *object = theRegistry.CreateObject(object_class->interface, constructor_data, size, guid, 1);
        network_object->Initialise(guid, object, object_class, peer, 0);
    }
}

void NetworkObjectManager::ReceiveContinuityBreak(NetMessage &message, NetPeer const &) {
    i16 id;
    i16 class_id;
    message.Read16(id);
    message.Read16(class_id);
    EdClass *object_class = theRegistry.GetClass(class_id);
    NetworkObject *object = &objects[id];
    if (object->object_class == object_class) {
        object->flags &= 2;
    }
}

void NetworkObjectManager::ReceiveObjectCallMessage(NetMessage &message, NetPeer const &peer) {
    u8 call_id;
    message.Read8(call_id);
    if (static_cast<i8>(call_id) <= 0) {
        return;
    }
    i32 call_index = static_cast<i8>(call_id - 1);
    if (registered_calls[call_index].type != 1) {
        return;
    }

    i16 id;
    i16 class_id;
    message.Read16(id);
    message.Read16(class_id);
    EdClass *object_class = theRegistry.GetClass(class_id);
    NetworkObject *object = &objects[id];
    if (object->object == NULL && (registered_calls[call_index].flags & 1) != 0) {
        void *instance = theRegistry.CreateObject(object_class->interface, NULL, 0, id, 1);
        object->Initialise(id, instance, object_class, peer, 0);
    }
    if (object->object != NULL) {
        reinterpret_cast<void (*)(void *, NetMessage &)>(registered_calls[call_index].callback)(object->object,
                                                                                                message);
    }
}

void NetworkObjectManager::ReceiveReleaseMessage(NetMessage &message, NetPeer const &) {
    i16 id;
    i16 class_id;
    message.Read16(id);
    message.Read16(class_id);
    NetworkObject *object = &objects[id];
    if (object->id == id && class_id == theRegistry.GetClassId(object->object_class) && object->object != NULL) {
        theRegistry.DestroyObject(object->object_class->interface, object->object, id, 1);
        RemoveFromLocalObjectList(object);
        object->Destroy();
    }
}

void NetworkObjectManager::ReceiveRemoteCallMessage(NetMessage &message, NetPeer const &) {
    u8 call_id;
    message.Read8(call_id);
    if (static_cast<i8>(call_id) > 0) {
        i32 call_index = static_cast<i8>(call_id - 1);
        if (registered_calls[call_index].type == 0) {
            reinterpret_cast<void (*)(NetMessage &)>(registered_calls[call_index].callback)(message);
        }
    }
}

void NetworkObjectManager::ReceiveReplicaMessage(NetMessage &, NetPeer const &) {
    STUBBED();
}

void NetworkObjectManager::ReceiveStartMessage(NetMessage &message, NetPeer const &peer) {
    NOSContext received_context = {};
    message.Read(&received_context, sizeof(received_context));
    if (!active) {
        return;
    }
    for (i32 i = 0; i < 8; ++i) {
        if (peer_push[i].peer != NULL && peer_push[i].peer == &peer) {
            i32 accepted = memcmp(&received_context, &context, sizeof(context)) == 0;
            if (accepted) {
                peer_push[i].Sync();
            } else {
                peer_push[i].Stop();
            }
            NetMessage reply;
            reply.Write8(11);
            reply.Write32(accepted);
            reply.Write(&context, sizeof(context));
            theNetwork.ReliableSend(reply, 3, const_cast<NetPeer &>(peer), NULL, 0);
            return;
        }
    }
}

void NetworkObjectManager::ReceiveStatusMessage(NetMessage &message, NetPeer const &peer) {
    NOSContext received_context = {};
    i32 accepted;
    message.Read32(accepted);
    message.Read(&received_context, sizeof(received_context));
    bool matching_context = accepted && memcmp(&received_context, &context, sizeof(context)) == 0;
    for (i32 i = 0; i < 8; ++i) {
        if (peer_push[i].peer != NULL && peer_push[i].peer == &peer) {
            if (!matching_context) {
                peer_push[i].Stop();
            } else if (peer_push[i].stage != 3 && peer_push[i].stage != 1 && peer_push[i].stage != 2) {
                peer_push[i].Sync();
            }
            return;
        }
    }
}

void NetworkObjectManager::ReceiveStopMessage(NetMessage &, NetPeer const &peer) {
    for (i32 i = 0; i < 8; ++i) {
        if (peer_push[i].peer != NULL && peer_push[i].peer == &peer) {
            peer_push[i].Stop();
            return;
        }
    }
}

void NetworkObjectManager::Recover(NetworkObject *) {
    STUBBED();
}

void NetworkObjectManager::RegisterObject(void *, EdClass *, i32) {
    STUBBED();
}

i32 NetworkObjectManager::RegisterObjectCall(void (*callback)(void *, NetMessage &), i32 flags) {
    if (registered_call_count <= 31) {
        registered_calls[registered_call_count].flags = flags;
        registered_calls[registered_call_count].type = 1;
        registered_calls[registered_call_count].callback = reinterpret_cast<void *>(callback);
        return ++registered_call_count;
    }
    return 0;
}

i32 NetworkObjectManager::RegisterRemoteCall(void (*callback)(NetMessage &), i32 flags) {
    if (registered_call_count <= 31) {
        registered_calls[registered_call_count].flags = flags;
        registered_calls[registered_call_count].type = 0;
        registered_calls[registered_call_count].callback = reinterpret_cast<void *>(callback);
        return ++registered_call_count;
    }
    return 0;
}

void NetworkObjectManager::ReleaseObject(void *, EdClass *, i32) {
    STUBBED();
}

void NetworkObjectManager::RemoteCall(i32, NetMessage, NetPeer const *) {
    STUBBED();
}

void NetworkObjectManager::RemoveFromLocalObjectList(NetworkObject *object) {
    NetworkObject **slot = local_objects;
    for (i32 i = 0; i < local_object_count; i++, slot++) {
        if (*slot == object) {
            *slot = NULL;
            return;
        }
    }
}

void NetworkObjectManager::RemovePendingObject(NetworkObject *object) {
    for (i32 i = 0; i < 32; i++) {
        if (pending_objects[i].object == object) {
            pending_objects[i].object = NULL;
            return;
        }
    }
}

void NetworkObjectManager::Reset() {
}

void NetworkObjectManager::SendAcquireMessage(NetworkObject *) {
    STUBBED();
}

void NetworkObjectManager::SendAcquiredMessage(i16, NetPeer const &) {
    STUBBED();
}

void NetworkObjectManager::SendAdoptedMessage(i16) {
    STUBBED();
}

i32 NetworkObjectManager::SendPushMessage(NetMessage *message, NetPeerPush const *push, i32 flags) {
    NetPeer *peer = const_cast<NetPeer *>(push->peer);
    if (message == NULL || message->data == NULL ||
        static_cast<i32>(message->write_offset - message->read_offset) <= 0) {
        return 1;
    }
    if (peer != NULL) {
        if ((flags & 1) && push->stage != 1 && push->stage != 2) {
            theNetwork.Send(*message, 3, *peer);
        } else {
            theNetwork.ReliableSend(*message, 3, *peer, NULL, 0);
        }
        return peer->vtable->get_available_messages(peer) > 15;
    }
    i32 available = 1;
    for (i32 i = 0; i < 8; ++i) {
        peer = const_cast<NetPeer *>(peer_push[i].peer);
        if (peer != NULL && peer_push[i].stage == 3) {
            if (flags & 1) {
                theNetwork.Send(*message, 3, *peer);
            } else {
                theNetwork.ReliableSend(*message, 3, *peer, NULL, 0);
            }
            if (peer->vtable->get_available_messages(peer) <= 15) {
                available = 0;
            }
        }
    }
    return available;
}

void NetworkObjectManager::Start(NOSContext const &new_context) {
    if (active != 0) {
        return;
    }
    active = 1;
    memmove(&context, &new_context, sizeof(context));
    NetMessage message;
    message.Write8(9);
    message.Write(&context, sizeof(context));
    theNetwork.ReliableBroadcast(message, 3);
}

NetworkObjectManager::PendingObject *NetworkObjectManager::StealPendingObject() {
    for (i32 i = 0; i < 32; i++) {
        if (UtilGetFrameStartTime() > pending_objects[i].field_04 + 1000) {
            pending_objects[i].field_00 = 0;
            pending_objects[i].field_04 = 0;
            return &pending_objects[i];
        }
    }
    return NULL;
}

void NetworkObjectManager::Stop() {
    if (active == 0) {
        return;
    }
    active = 0;
    NetMessage message;
    message.Write8(10);
    theNetwork.ReliableBroadcast(message, 3);
    for (i32 i = 0; i < 8; i++) {
        if (peer_push[i].peer != NULL) {
            peer_push[i].Stop();
        }
    }
}

void NetworkObjectManager::Term() {
    STUBBED();
}

void NetworkObjectManager::Update() {
    STUBBED();
}

void NetworkObjectManager::UpdateLocalObjectList() {
    STUBBED();
}

NetworkObjectManager::~NetworkObjectManager() {
    theNos = NULL;
}

static void ReleasePushMessage(NetMessage *&message) {
    if (message != NULL) {
        message->~NetMessage();
        theMemoryManager.FreePool(message, sizeof(NetMessage));
    }
    message = NULL;
}

static NetMessage *GetPushMessage(NetworkObjectManager::NetPeerPush *push, NetMessage *&message, i32 size,
                                  i32 unreliable) {
    if (message != NULL) {
        i32 available = message->data != NULL ? 0x4af - message->write_offset : 0;
        if (size > available) {
            theNos->SendPushMessage(message, push, unreliable);
            ReleasePushMessage(message);
        }
    }
    if (message == NULL) {
        message = new (theMemoryManager.AllocPool(sizeof(NetMessage), 1)) NetMessage;
    }
    return message;
}

void NetworkObjectManager::NetPeerPush::FlushMessages() {
    if (message != NULL) {
        theNos->SendPushMessage(message, this, 1);
        ReleasePushMessage(message);
    }
    if (reliable_message != NULL) {
        theNos->SendPushMessage(reliable_message, this, 0);
        ReleasePushMessage(reliable_message);
    }
}

NetMessage *NetworkObjectManager::NetPeerPush::GetMessage(i32 size) {
    return GetPushMessage(this, message, size, 1);
}

NetMessage *NetworkObjectManager::NetPeerPush::GetReliableMessage(i32 size) {
    return GetPushMessage(this, reliable_message, size, 0);
}

void NetworkObjectManager::NetPeerPush::NextStage() {
    if (stage < 3) {
        field_10 = 0;
        stage++;
    }
}

void NetworkObjectManager::NetPeerPush::Stop() {
    field_10 = 0;
    stage = 0;
}

void NetworkObjectManager::NetPeerPush::Sync() {
    field_10 = 0;
    stage = 1;
}
