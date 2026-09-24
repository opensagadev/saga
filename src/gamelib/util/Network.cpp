#include "decomp.h"
#include "gamelib/util/NetInputStream.h"
#include "gamelib/util/NetOutputStream.h"
#include "gamelib_util_types.h"
#include "gamelib/util/Utilities.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/NuNetEmu.h"
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

__attribute__((weak)) void EdRefNosGuid::SetMemberData(void *object, i32, void *data, i32, i16 *) {
    void *member = static_cast<u8 *>(object) + member_offset;
    if (attributes & 0x40000000)
        member = *static_cast<void **>(member);
    *static_cast<void **>(member) = theNos->GetObject(*static_cast<i16 *>(data));
}

__attribute__((weak)) void EdRefNosGuid::GetMemberData(void *object, i32 type, void *data, i32) {
    CheckType(type);
    void *member = static_cast<u8 *>(object) + member_offset;
    if (attributes & 0x40000000)
        member = *static_cast<void **>(member);
    *static_cast<i16 *>(data) = static_cast<i16>(theNos->GetGuid(*static_cast<void **>(member)));
}

static f32 NetworkFrameTime() {
    u32 time = UtilGetFrameStartTime();
    return static_cast<f32>(time & 0xffff) + static_cast<f32>(time >> 16) * 65536.0f;
}

static f32 ClampPrediction(NetPredictor const *predictor, f32 value) {
    if ((predictor->replication_group & 0x10) != 0 && value < predictor->minimum_value) {
        value = predictor->minimum_value;
    }
    if ((predictor->replication_group & 0x20) != 0 && value > predictor->maximum_value) {
        value = predictor->maximum_value;
    }
    return value;
}

void NetRotator2::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *time,
                               NetPredictor::PredictorData **samples, float *values, i32 count) {
    f32 interval_scale = 1.0f / (time->values[2] - time->values[1]);
    f32 elapsed = NetworkFrameTime() - time->values[2];
    for (i32 i = 0; i < count; i++) {
        i32 delta = (static_cast<i32>(samples[i]->values[2]) - static_cast<i32>(samples[i]->values[1])) & 0xffff;
        if (delta >= 0x8000) {
            delta -= 0x10000;
        }
        f32 predicted = samples[i]->values[2] + static_cast<f32>(delta) * interval_scale * elapsed;
        values[i] = ClampPrediction(this, predicted);
    }
}

bool NetPredictor::AllowPush(EdClass const *object_class, void const *object, ReplicatorData &data, i32 force, i32) {
    u32 *last_push = reinterpret_cast<u32 *>((reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u);
    u32 *failed_predictions = last_push + 1;
    data.cursor = reinterpret_cast<u8 *>(failed_predictions + 1);

    u32 now = UtilGetFrameStartTime();
    if (force != 0) {
        *last_push = now;
        return true;
    }
    if (now - *last_push <= minimum_interval) {
        return false;
    }

    ReplicatorData prediction_data = data;
    if (DoPrediction(object_class, const_cast<void *>(object), prediction_data, 1) != 0) {
        *failed_predictions = 0;
        replication_group |= 1;
    } else {
        if (*failed_predictions > 2) {
            return false;
        }
        ++*failed_predictions;
        replication_group &= ~1;
    }
    *last_push = now;
    return true;
}

i32 NetPredictor::CheckPredictionError(EdClass const *, void *, float *actual, float *predicted, i32 count) {
    for (i32 i = 0; i < count; i++) {
        f32 error = actual[i] - predicted[i];
        if (error > maximum_prediction_error || error < -maximum_prediction_error) {
            return 1;
        }
    }
    return 0;
}

static i32 PredictorValues(EdRef *member, u8 *member_data, f32 *converted, f32 *&values) {
    if (member->type_id == EdType_Float) {
        values = reinterpret_cast<f32 *>(member_data);
        return 1;
    }
    if (member->type_id == EdType_VuVec || member->type_id == EdType_NuVec) {
        values = reinterpret_cast<f32 *>(member_data);
        return 3;
    }
    if (member->type_id == EdType_Char) {
        converted[0] = static_cast<f32>(*reinterpret_cast<i8 *>(member_data));
    } else if (member->type_id == EdType_Short) {
        converted[0] = static_cast<f32>(*reinterpret_cast<i16 *>(member_data));
    } else if (member->type_id == EdType_Int) {
        converted[0] = static_cast<f32>(*reinterpret_cast<i32 *>(member_data));
    }
    values = converted;
    return 1;
}

static void StorePredictedValues(EdRef *member, u8 *member_data, f32 const *values) {
    if (member->type_id == EdType_Char) {
        *reinterpret_cast<i8 *>(member_data) = static_cast<i8>(values[0]);
    } else if (member->type_id == EdType_Short) {
        *reinterpret_cast<i16 *>(member_data) = static_cast<i16>(values[0]);
    } else if (member->type_id == EdType_Int) {
        *reinterpret_cast<i32 *>(member_data) = static_cast<i32>(values[0]);
    }
}

static NetPredictor::PredictorData *AllocatePredictorData(ReplicatorData &data) {
    uintptr_t cursor = (reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u;
    NetPredictor::PredictorData *sample = reinterpret_cast<NetPredictor::PredictorData *>(cursor);
    data.cursor = reinterpret_cast<u8 *>(sample + 1);
    return sample;
}

void NetPredictor::StoreSampleData(EdClass const *, void *, NetPredictor::PredictorTime *time,
                                   NetPredictor::PredictorData **samples, float *values, i32 count) {
    for (i32 i = 0; i < count; i++) {
        samples[i]->values[0] = samples[i]->values[1];
        samples[i]->values[1] = samples[i]->values[2];
        samples[i]->values[2] = values[i];
        if (maximum_sample_delta > 0.0f && samples[i]->values[2] - samples[i]->values[1] > maximum_sample_delta) {
            time->sample_count = 0;
        }
    }
}

void NetPredictor2::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *time,
                                 NetPredictor::PredictorData **samples, float *values, i32 count) {
    f32 interval_scale = 1.0f / (time->values[2] - time->values[1]);
    f32 elapsed = NetworkFrameTime() - time->values[2];
    for (i32 i = 0; i < count; i++) {
        f32 predicted =
            samples[i]->values[2] + (samples[i]->values[2] - samples[i]->values[1]) * interval_scale * elapsed;
        predicted = ClampPrediction(this, predicted);
        values[i] = values[i] * 0.8f + predicted * 0.2f;
    }
}

void NetPredictor3::PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *time,
                                 NetPredictor::PredictorData **samples, float *values, i32 count) {
    f32 now = NetworkFrameTime();
    for (i32 i = 0; i < count; i++) {
        f32 predicted = samples[i]->values[0] * (now - time->values[1]) * (now - time->values[2]) * time->factors[0] +
                        samples[i]->values[1] * (now - time->values[0]) * (now - time->values[2]) * time->factors[1] +
                        samples[i]->values[2] * (now - time->values[0]) * (now - time->values[1]) * time->factors[2];
        predicted = ClampPrediction(this, predicted);
        values[i] = values[i] * 0.8f + predicted * 0.2f;
    }
}

i32 NetPredictor::SerialiseObject(EdStream &stream, NetPeer *peer, EdClass const *object_class, void *object,
                                  ReplicatorData &data, NetPredictor::PredictorTime *time, i16 *class_mapping) {
    u8 member_data[256];
    for (EdRef *member = object_class->members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(object);
            if (ForceDummySerialise != 0 && member_object == NULL) {
                member_object = object;
            }
            if (member_class->SerialiseObjectHeader(stream, member_object) != 0) {
                SerialiseObject(stream, peer, member_class, member_object, data, time, class_mapping);
            }
        } else if (member->replication_group == id) {
            EdType *type = theRegistry.GetType(member->type_id);
            i32 size = member->size;
            if (size <= 0) {
                size = type->size;
            }
            if (object == NULL) {
                stream.Eat(size, 1);
                continue;
            }

            if (stream.mode == 2) {
                member->GetMemberData(object, member->type_id, member_data, sizeof(member_data));
            }
            type->serialise(stream, member_data, size);
            if (stream.mode == 1 && time->sample_count <= 2 && (replication_group & 8) == 0) {
                member->SetMemberData(object, member->type_id, member_data, sizeof(member_data), class_mapping);
            }

            f32 converted[1];
            f32 *values;
            i32 count = PredictorValues(member, member_data, converted, values);
            NetPredictor::PredictorData *samples[3];
            for (i32 i = 0; i < count; i++) {
                samples[i] = AllocatePredictorData(data);
            }
            StoreSampleData(object_class, object, time, samples, values, count);
        }
    }
    return 1;
}

i32 NetPredictor::SerialiseObject(EdStream &stream, NetPeer *peer, EdClass const *object_class, void *object,
                                  ReplicatorData &data, i16 *class_mapping) {
    uintptr_t cursor = (reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u;
    NetPredictor::PredictorTime *time = reinterpret_cast<NetPredictor::PredictorTime *>(cursor);
    data.cursor = reinterpret_cast<u8 *>(time + 1);

    u8 continuity_break = (replication_group & 8) != 0;
    UtilGetFrameStartTime();
    stream.SerialiseBuffer(&continuity_break, 1, 1);
    if (continuity_break != 0) {
        time->sample_count = 0;
    }

    time->values[0] = time->values[1];
    time->values[1] = time->values[2];
    if (stream.mode == 2) {
        time->values[2] = NetworkFrameTime();
    } else {
        stream.SerialiseBuffer(&time->values[2], 4, 1);
        if (stream.mode == 1) {
            UtilGetFrameStartTime();
            time->values[2] = static_cast<f32>(static_cast<i32>(time->values[2]) + peer->time_offset);
        }
    }

    if (time->values[1] >= time->values[2]) {
        time->sample_count = 1;
    } else if (time->sample_count <= 2) {
        ++time->sample_count;
        if (time->sample_count == 3) {
            f32 t0 = time->values[0];
            f32 t1 = time->values[1];
            f32 t2 = time->values[2];
            time->factors[0] = 1.0f / ((t0 - t2) * (t0 - t1));
            time->factors[1] = 1.0f / ((t1 - t0) * (t1 - t2));
            time->factors[2] = 1.0f / ((t2 - t0) * (t2 - t1));
        }
    }

    SerialiseObject(stream, peer, object_class, object, data, time, class_mapping);
    if (continuity_break != 0) {
        replication_group &= ~8;
    }
    return 1;
}

i32 NetPredictor::DoPrediction(EdClass const *object_class, void *object, ReplicatorData &data,
                               NetPredictor::PredictorTime *time, i32 check_only) {
    i32 result = 0;
    u8 member_data[256];
    for (EdRef *member = object_class->members; member != NULL; member = member->next) {
        if (member->attributes < 0) {
            EdClass *member_class = theRegistry.GetClass(member->type_id);
            void *member_object = member->GetMemberObject(object);
            result |= DoPrediction(member_class, member_object, data, time, check_only);
        } else if (member->replication_group == id) {
            member->GetMemberData(object, member->type_id, member_data, sizeof(member_data));

            f32 converted[1];
            f32 *values;
            i32 count = PredictorValues(member, member_data, converted, values);
            f32 actual[3];
            memcpy(actual, values, static_cast<u32>(count) * sizeof(f32));

            NetPredictor::PredictorData *samples[3];
            for (i32 i = 0; i < count; i++) {
                samples[i] = AllocatePredictorData(data);
            }
            PredictValue(object_class, object, time, samples, values, count);

            if (check_only != 0) {
                result |= CheckPredictionError(object_class, object, actual, values, count);
            } else {
                StorePredictedValues(member, member_data, values);
                member->SetMemberData(object, member->type_id, member_data, sizeof(member_data), NULL);
            }
        }
    }
    return result;
}

i32 NetPredictor::DoPrediction(EdClass const *object_class, void *object, ReplicatorData &data, i32 check_only) {
    uintptr_t cursor = (reinterpret_cast<uintptr_t>(data.cursor) + 3) & ~3u;
    NetPredictor::PredictorTime *time = reinterpret_cast<NetPredictor::PredictorTime *>(cursor);
    data.cursor = reinterpret_cast<u8 *>(time + 1);

    if (time->sample_count <= 2) {
        return 1;
    }
    if (check_only == 0 && (time->values[0] >= time->values[1] || time->values[1] >= time->values[2])) {
        return 1;
    }
    return DoPrediction(object_class, object, data, time, check_only);
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

i32 NetworkObjectManager::Acquire(i32 id) {
    if (id == 0) {
        return -1;
    }

    NetworkObject *object = FindNetworkObject(id);
    if (object == NULL) {
        return -1;
    }
    if (object->owner->local != 0) {
        return 1;
    }
    if ((object->flags & 8) == 0) {
        return 0;
    }
    if (IsPeerReady(*object->owner) == 0) {
        return -1;
    }
    if ((object->flags & 0x20) != 0) {
        return 0;
    }

    PendingObject *pending = FindPendingObject(object);
    if (pending == NULL) {
        pending = FindPendingObject(NULL);
        if (pending == NULL) {
            pending = StealPendingObject();
        }
        if (pending == NULL) {
            return 0;
        }
    }

    u32 now = UtilGetFrameStartTime();
    if (now <= pending->field_04) {
        return 0;
    }
    pending->field_00 = 1;
    pending->field_04 = now + 250;
    pending->object = object;
    SendAcquireMessage(object);
    return 0;
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

void NetworkObjectManager::CalcReplicatorDataSize(NetReplicator *replicator, EdClass const *object_class,
                                                  i32 &data_size, i32 &message_size) {
    u8 data_memory[0x1400] = {};
    u8 object_memory[0x1400] = {};
    ReplicatorData data;
    data.start = data_memory;
    data.end = data_memory + 0x100;
    data.cursor = data_memory;
    replicator->AllowPush(object_class, object_memory, data, 1, 1);

    NetOutputStream stream;
    NetMessage message;
    i16 flags = 0x10;
    ForceDummySerialise = 1;
    stream.message = &message;
    replicator->SerialiseObject(stream, NULL, object_class, object_memory, data, &flags);
    data_size = data.cursor - data.start;
    ForceDummySerialise = 0;
    message_size = message.data != NULL ? message.write_offset - message.read_offset : 0;
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

void NetworkObjectManager::ContinuityBreak(i32 id, float) {
    if (id == 0) {
        return;
    }
    NetworkObject *object = FindNetworkObject(id);
    if (object == NULL) {
        return;
    }
    object->flags |= 2;
    if (object->owner->local == 0) {
        return;
    }

    NetMessage message;
    i16 object_id = object->id;
    i32 class_id = theRegistry.GetClassId(object->object_class);
    message.Write8(12);
    message.Write16(object_id);
    message.Write16(class_id);
    theNetwork.ReliableBroadcast(message, 3);
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

void NetworkObjectManager::FlushObjects(i32 flush_level_editor) {
    NetworkObject *object = objects;
    NetworkObject *end = objects + 2048;
    while (end != object) {
        if (object->id != 0) {
            EdClassInterface *interface = object->object_class->interface;
            interface->vtable->set_object_guid(interface, object->object, 0);
            object->Destroy();
        }
        ++object;
    }
    memset(local_objects, 0, sizeof(local_objects));
    local_object_count = 0;
    memset(objects, 0, sizeof(objects));
    if (flush_level_editor != 0) {
        theLevelEditor.Flush();
    }
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
    i32 class_count = theRegistry.class_count;
    for (i32 class_index = 0; class_index < class_count; ++class_index) {
        EdClass *object_class = theRegistry.GetClass(class_index);
        if (object_class == NULL || object_class->interface == NULL) {
            continue;
        }

        void *object = object_class->interface->vtable->get_next_object(object_class->interface, NULL);
        while (object != NULL) {
            i32 guid = object_class->interface->vtable->get_object_guid(object_class->interface, object);
            if (guid == 0) {
                guid = GetNextGuid();
                object_class->interface->vtable->set_object_guid(object_class->interface, object, guid);
            }
            RegisterObject(object, object_class, guid);
            object = object_class->interface->vtable->get_next_object(object_class->interface, object);
        }
    }
}

void NetworkObjectManager::Init() {
    guid_peers[0] = NULL;
    guid_peers[1] = NULL;
    memset(objects, 0, sizeof(objects));
    memset(local_objects, 0, sizeof(local_objects));
    theNetwork.AddListener(this, 3, const_cast<char *>("NOS"));
    theRegistry.AddObjectNotifier(this);
    guid_group = 0;
    guid_peers[0] = reinterpret_cast<NetPeer const *>(-1);
    field_d96c = 1;
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

static NetMessage MakeObjectCallMessage(NetMessage const &message, i32 call_id, NetworkObject const *object) {
    NetMessage outgoing(message);
    if (outgoing.data != NULL) {
        i16 class_id = static_cast<i16>(theRegistry.GetClassId(object->object_class));
        i16 object_id = object->id;
        outgoing.read_offset -= 2;
        memcpy(outgoing.data->bytes + outgoing.read_offset, &class_id, sizeof(class_id));
        if (outgoing.swap_endianness != 0) {
            EdFileSwapEndianess16(outgoing.data->bytes + outgoing.read_offset);
        }
        outgoing.read_offset -= 2;
        memcpy(outgoing.data->bytes + outgoing.read_offset, &object_id, sizeof(object_id));
        if (outgoing.swap_endianness != 0) {
            EdFileSwapEndianess16(outgoing.data->bytes + outgoing.read_offset);
        }
        outgoing.data->bytes[--outgoing.read_offset] = static_cast<u8>(call_id);
        outgoing.data->bytes[--outgoing.read_offset] = 8;
    }
    return outgoing;
}

i32 NetworkObjectManager::ObjectCall(void *instance, i32 call_id, NetMessage message, NetPeer const *peer) {
    NetworkObject *object = FindNetworkObject(instance);
    if (object == NULL) {
        return 0;
    }

    NetMessage outgoing = MakeObjectCallMessage(message, call_id, object);
    if (peer == NULL) {
        theNetwork.ReliableBroadcast(outgoing, 3);
        reinterpret_cast<void (*)(void *, NetMessage &)>(registered_calls[call_id - 1].callback)(instance, message);
    } else if (peer->local != 0) {
        reinterpret_cast<void (*)(void *, NetMessage &)>(registered_calls[call_id - 1].callback)(instance, message);
    } else {
        theNetwork.ReliableSend(outgoing, 3, *const_cast<NetPeer *>(peer), NULL, 0);
    }
    return 1;
}

i32 NetworkObjectManager::ObjectOtherCall(void *instance, i32 call_id, NetMessage message) {
    NetworkObject *object = FindNetworkObject(instance);
    if (object == NULL) {
        return 0;
    }
    if (object->owner->local == 0) {
        return 1;
    }

    NetMessage outgoing = MakeObjectCallMessage(message, call_id, object);
    theNetwork.ReliableBroadcast(outgoing, 3);
    return 1;
}

i32 NetworkObjectManager::ObjectOwnerCall(void *instance, i32 call_id, NetMessage message) {
    NetworkObject *object = FindNetworkObject(instance);
    if (object == NULL) {
        return 0;
    }
    if (object->owner->local != 0) {
        reinterpret_cast<void (*)(void *, NetMessage &)>(registered_calls[call_id - 1].callback)(instance, message);
        return 1;
    }

    NetMessage outgoing = MakeObjectCallMessage(message, call_id, object);
    theNetwork.ReliableSend(outgoing, 3, *const_cast<NetPeer *>(object->owner), NULL, 0);
    return 1;
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

i32 NetworkObjectManager::Push(NetworkObject const *object, NetReplicator *replicator, ReplicatorData &data,
                               NetworkObjectManager::NetPeerPush *peer_push) {
    if (peer_push == NULL) {
        peer_push = &default_push;
    }

    i32 required_size = replicator->message_size + 10;
    NetMessage *message;
    if ((replicator->replication_group & 1) != 0) {
        message = peer_push->GetMessage(required_size);
    } else {
        message = peer_push->GetReliableMessage(required_size);
    }

    NetOutputStream stream;
    i16 object_id = object->id;
    i32 class_id = theRegistry.GetClassId(object->object_class);
    i16 flags = object->flags;
    message->Write8(2);
    message->Write16(object_id);
    message->Write16(static_cast<i16>(class_id));
    message->Write16(replicator->id);
    message->Write16(flags);

    i32 initial_size = message->data != NULL ? message->write_offset - message->read_offset : 0;
    stream.message = message;
    if (replicator->SerialiseObject(stream, NULL, object->object_class, object->object, data, &flags) == 0 &&
        message->data != NULL) {
        message->write_offset -= 9;
    }

    if (class_stats[class_id] != NULL) {
        i32 final_size = message->data != NULL ? message->write_offset - message->read_offset : 0;
        class_stats[class_id]->total.values[0] += final_size - initial_size;
    }
    return 1;
}

i32 NetworkObjectManager::PushObject(NetworkObject *object, NetworkObjectManager::NetPeerPush *peer_push, i32 force) {
    if (peer_push == NULL) {
        peer_push = &default_push;
        object->flags &= ~4;
    } else if (peer_push->stage == 1 || peer_push->stage == 2) {
        object->flags |= 4;
    } else {
        object->flags &= ~4;
    }

    i32 class_id = theRegistry.GetClassId(object->object_class);
    i32 allowed = 1;
    NOSFilter *filter = filters[class_id];
    if (filter != NULL && force == 0) {
        allowed = filter->AllowPush(object->object_class, object->object);
    }

    i32 message_limit = theNuNetEmu.field_1c;
    if (theNuNetEmu.packet_stats.pack_ratio > 0.0f) {
        message_limit = static_cast<i32>(message_limit * theNuNetEmu.packet_stats.pack_ratio);
    }

    i32 result = 1;
    if ((allowed | force) != 0) {
        NetReplicator *replicator = replicators[class_id].head;
        if (replicator != NULL) {
            i32 data_offset = 0;
            i32 permit_push = 1;
            do {
                if ((object->flags & 2) != 0) {
                    replicator->replication_group |= 8;
                }

                ReplicatorData data;
                data.start = static_cast<u8 *>(object->replicator_data) + data_offset;
                data.end = data.start + replicator->data_size;
                data.cursor = data.start;
                data_offset += replicator->data_size;

                if (theNuNetEmu.field_00 < message_limit && (replicator->replication_group & 0x40) == 0) {
                    permit_push = 0;
                }
                if ((permit_push | force) != 0 &&
                    replicator->AllowPush(object->object_class, object->object, data, force, 0) != 0 &&
                    Push(object, replicator, data, peer_push) == 0) {
                    result = 0;
                }
                replicator = replicator->next;
            } while (replicator != NULL);
        }
    }

    if ((object->flags & 2) != 0) {
        object->flags &= ~2;
    }
    return result;
}

void NetworkObjectManager::Receive(NetMessage message, unsigned char, NetPeer const &peer) {
    u8 type;
    i32 stop;
    do {
        message.Read8(type);
        stop = 0;
        switch (type) {
            case 0:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    message.Read32(guid_group);
                } else {
                    stop = 1;
                }
                break;
            case 1:
                if (active != 0 && IsPeerStarted(peer) != 0) {
                    ReceiveConstructorMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 2:
                if (active != 0 && IsPeerStarted(peer) != 0) {
                    ReceiveReplicaMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 3:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveAcquireMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 4:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveAcquiredMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 5:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveAdoptedMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 6:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveReleaseMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 7:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveRemoteCallMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 8:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveObjectCallMessage(message, peer);
                } else {
                    stop = 1;
                }
                break;
            case 9:
                ReceiveStartMessage(message, peer);
                break;
            case 10:
                ReceiveStopMessage(message, peer);
                break;
            case 11:
                ReceiveStatusMessage(message, peer);
                break;
            case 12:
                if (active != 0 && IsPeerReady(peer) != 0) {
                    ReceiveContinuityBreak(message, peer);
                } else {
                    stop = 1;
                }
                break;
            default:
                stop = 1;
                break;
        }
    } while (message.data != NULL && static_cast<i32>(message.write_offset - message.read_offset) > 0 && stop == 0);
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

void NetworkObjectManager::ReceiveReplicaMessage(NetMessage &message, NetPeer const &peer) {
    NetInputStream stream;
    i16 id;
    i16 class_id;
    i16 replicator_id;
    i16 flags;
    message.Read16(id);
    message.Read16(class_id);
    message.Read16(replicator_id);
    message.Read16(flags);

    EdClass *object_class = theRegistry.GetClass(class_id);
    NetworkObject *object = &objects[id];
    object->flags = static_cast<u16>(flags | 8);

    NetReplicator *replicator = replicators[class_id].head;
    i32 data_offset = 0;
    while (replicator != NULL && replicator->id != replicator_id) {
        data_offset += replicator->data_size;
        replicator = replicator->next;
    }

    if (object->object == NULL || object->object_class != object_class) {
        if (message.data != NULL) {
            message.read_offset += replicator->message_size;
        }
        return;
    }

    if ((flags & 2) != 0) {
        replicator->replication_group |= 8;
    }

    ReplicatorData data;
    data.start = static_cast<u8 *>(object->replicator_data) + data_offset;
    data.end = data.start + replicator->data_size;
    data.cursor = data.start;
    replicator->AllowPush(object_class, object->object, data, 1, 0);
    stream.message = &message;
    replicator->SerialiseObject(stream, const_cast<NetPeer *>(&peer), object_class, object->object, data,
                                reinterpret_cast<i16 *>(object));
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

void NetworkObjectManager::Recover(NetworkObject *object) {
    if (object != NULL) {
        object->owner = theSession->local_peer;
        AddToLocalObjectList(object);
        SendAdoptedMessage(object->id);
        theNetwork.NosAdopted(object, *object->owner);
    }
}

i32 NetworkObjectManager::RegisterObject(void *object, EdClass *object_class, i32 guid) {
    if (guid == 0) {
        guid = GetNextGuid();
    }
    if (guid <= 0) {
        return guid;
    }

    for (i32 i = 0; i < 2048; ++i) {
        if (objects[i].object == object && field_d96c == 0) {
            return 0;
        }
    }

    NetworkObject *network_object = &objects[guid];
    network_object->Initialise(guid, object, object_class, *theSession->local_peer, 1);
    AddToLocalObjectList(network_object);
    if (active != 0) {
        ConstructObject(network_object, &default_push);
        default_push.FlushMessages();
    }
    return guid;
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

i32 NetworkObjectManager::ReleaseObject(void *object, EdClass *, i32 guid) {
    if (object == NULL) {
        return 1;
    }

    NetworkObject *network_object;
    if (guid != 0) {
        network_object = FindNetworkObject(guid);
    } else {
        network_object = FindNetworkObject(object);
    }
    if (network_object == NULL) {
        return 1;
    }

    RemoveFromLocalObjectList(network_object);
    if (active != 0) {
        i16 id = network_object->id;
        i32 class_id = theRegistry.GetClassId(network_object->object_class);
        NetMessage message;
        message.Write8(6);
        message.Write16(id);
        message.Write16(class_id);
        theNetwork.ReliableBroadcast(message, 3);
    }
    network_object->Destroy();
    return 1;
}

i32 NetworkObjectManager::RemoteCall(i32 call_id, NetMessage message, NetPeer const *peer) {
    NetMessage outgoing(message);
    if (outgoing.data != NULL) {
        outgoing.data->bytes[--outgoing.read_offset] = static_cast<u8>(call_id);
        outgoing.data->bytes[--outgoing.read_offset] = 7;
    }

    if (peer == NULL) {
        theNetwork.ReliableBroadcast(outgoing, 3);
        reinterpret_cast<void (*)(NetMessage &)>(registered_calls[call_id - 1].callback)(message);
    } else if (peer->local != 0) {
        reinterpret_cast<void (*)(NetMessage &)>(registered_calls[call_id - 1].callback)(message);
    } else {
        theNetwork.ReliableSend(outgoing, 3, *const_cast<NetPeer *>(peer), NULL, 0);
    }
    return 1;
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

void NetworkObjectManager::SendAcquireMessage(NetworkObject *object) {
    NetMessage message;
    message.Write8(3);
    message.Write16(object->id);
    theNetwork.Send(message, 3, *const_cast<NetPeer *>(object->owner));
}

void NetworkObjectManager::SendAcquiredMessage(i16 id, NetPeer const &peer) {
    NetMessage message;
    message.Write8(4);
    message.Write16(id);
    theNetwork.ReliableSend(message, 3, const_cast<NetPeer &>(peer), NULL, 0);
}

void NetworkObjectManager::SendAdoptedMessage(i16 id) {
    NetMessage message;
    message.Write8(5);
    message.Write16(id);
    theNetwork.ReliableBroadcast(message, 3);
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
    Reset();
    theNetwork.RemoveListener(this, 3);
}

void NetworkObjectManager::Update() {
    for (i32 i = 0; i < 2048; i++) {
        NetworkObject *object = &objects[i];
        if (object->id == 0) {
            continue;
        }

        if (object->owner->local != 0) {
            PushObject(object, NULL, 0);
            continue;
        }
        if ((object->flags & 2) != 0) {
            continue;
        }

        i32 class_id = theRegistry.GetClassId(object->object_class);
        NetReplicator *replicator = replicators[class_id].head;
        i32 data_offset = 0;
        while (replicator != NULL) {
            i32 data_size = replicator->data_size;
            if ((replicator->replication_group & 4) != 0) {
                ReplicatorData data;
                data.start = static_cast<u8 *>(object->replicator_data) + data_offset;
                data.end = data.start + data_size;
                data.cursor = data.start;
                replicator->AllowPush(object->object_class, object->object, data, 0, 0);
                replicator->DoPrediction(object->object_class, object->object, data, 0);
            }
            data_offset += data_size;
            replicator = replicator->next;
        }
    }

    for (i32 i = 0; i < 8; i++) {
        NetPeerPush *push = &peer_push[i];
        if (push->peer == NULL) {
            continue;
        }
        if (push->stage == 1 || push->stage == 2) {
            i32 object_index = push->field_10;
            i32 object_count = local_object_count;
            i32 message_limit = theNuNetEmu.field_1c;
            if (theNuNetEmu.packet_stats.pack_ratio > 0.0f) {
                message_limit = static_cast<i32>(message_limit * theNuNetEmu.packet_stats.pack_ratio);
            }

            while (object_index < object_count && message_limit < theNuNetEmu.field_00) {
                NetworkObject *object = local_objects[object_index++];
                if (object == NULL || object->object == NULL) {
                    continue;
                }

                if (push->stage == 1) {
                    ConstructObject(object, push);
                }
                if (push->stage == 2) {
                    PushObject(object, push, 1);
                }

                NetPeer *peer = const_cast<NetPeer *>(push->peer);
                if (peer->vtable->get_available_messages(peer) <= 15) {
                    break;
                }

                message_limit = theNuNetEmu.field_1c;
                if (theNuNetEmu.packet_stats.pack_ratio > 0.0f) {
                    message_limit = static_cast<i32>(message_limit * theNuNetEmu.packet_stats.pack_ratio);
                }
                object_count = local_object_count;
            }

            if (object_index >= local_object_count) {
                push->NextStage();
            } else {
                push->field_10 = object_index;
            }
        }
        push->FlushMessages();
    }

    default_push.FlushMessages();
    for (i32 i = 0; i < 32; i++) {
        if (class_stats[i] != NULL) {
            class_stats[i]->Update();
        }
    }
}

void NetworkObjectManager::UpdateLocalObjectList() {
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
