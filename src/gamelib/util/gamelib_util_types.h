#ifndef GAMELIB_UTIL_TYPES_H
#define GAMELIB_UTIL_TYPES_H
#pragma once

#include "nu2api/nucore/fixed_width.h"
#include "decomp.h"
#include "gamelib/util/CRC16.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/EdObjectNotifier.h"
#include <stddef.h>
#include <string.h>

struct AIPATHNODE_s;
struct AndroidOBBUtils;
struct BOLT_s;
struct EdClass;
struct EdStream;
struct FtpFile;
struct GIZFORCE_s;
struct GIZMOBLOWUP_s;
struct GameObject_s;
struct NetHost;
struct MechObjectInterface;
struct NOSContext;
struct NOSFilter;
struct NetAddress;
struct NetChangedReplicator;
struct NetConstReplicator;
struct NetFtpManager;
struct NetListenerBinding;
struct NetListenerInterface;
struct NetListenerList;
struct NetMessage;
struct NetPeer;
struct NetPredictor;
struct NetPredictor2;
struct NetPredictor3;
struct NetReplicator;
struct NetRotator2;
struct NetSample;
struct NetSimpleReplicator;
struct NetSmallStats;
struct NetStats;
struct NetTransporter;
struct NetworkObject;
struct NetworkObjectManager;
struct nucolour3_s;
struct NuFileDeviceAndroidOBBType;
struct ReplicatorData;
struct TouchHacks;
struct V2SessionManager;
struct VirtualStackAllocator;
struct VuVec;
struct WORLDINFO_s;
struct ePeerLeftReason;

struct AIPATHNODE_s;
struct BOLT_s;
struct EdClass;
struct EdStream;
struct GIZFORCE_s;
struct GIZMOBLOWUP_s;
struct GameObject_s;
struct NOSContext {
    union {
        u8 bytes[0x10];
        u32 words[4];
    };
};
struct NOSFilter {
    virtual i32 AllowPush(EdClass const *, void *) = 0;
};
struct NetAddress {
    u32 value;
};
struct NetSample {
    u32 values[4];

    NetSample() {
        values[0] = 0;
        values[1] = 0;
        values[2] = 0;
        values[3] = 0;
    }

    void Max(NetSample const &);
    void Reset();
    void operator+=(NetSample const &);
    void operator-=(NetSample const &);
};
struct NetSmallStats {
    enum eInfo {};

    explicit NetSmallStats(char const *stat_name) : name(stat_name) {
    }
    virtual void Update() {
    }
    virtual void Draw(float, float, float, float, NetSmallStats::eInfo) const;

    char const *name;
    NetSample total;
};
struct NetStats : NetSmallStats {
    explicit NetStats(char const *stat_name) : NetSmallStats(stat_name), sample_index(0), sample_time(0) {
    }
    void Update() override;
    void Draw(float, float, float, float, NetSmallStats::eInfo) const override;

    i32 sample_index;
    u32 sample_time;
    NetSample maximum;
    NetSample previous;
    NetSample samples[30];
};
DECOMP_ASSERT(sizeof(NetSmallStats) == 0x18, "NetSmallStats size");
DECOMP_ASSERT(offsetof(NetSmallStats, total) == 8, "NetSmallStats total offset");
DECOMP_ASSERT(sizeof(NetStats) == 0x220, "NetStats size");
DECOMP_ASSERT(offsetof(NetStats, sample_index) == 0x18, "NetStats sample index offset");
DECOMP_ASSERT(offsetof(NetStats, samples) == 0x40, "NetStats sample history offset");
struct NetPeer {
    struct Vtable {
        void *reserved_00;
        void (*destroy)(NetPeer *);
        void (*disconnect)(NetPeer *);
        void (*send)(NetPeer *, NetMessage);
        void (*reliable_send)(NetPeer *, NetMessage, char const *, u32);
        void *reserved_14;
        i32 (*get_available_messages)(NetPeer *);
    } *vtable;
    NetPeer *next;
    NetPeer *previous;
    u8 local;
    u8 reserved_0d[3];
    NetStats stats;
    u8 reserved_230[0x29c - 0x230];
    i32 time_offset;
};
struct ReplicatorData {
    u8 *start;
    u8 *end;
    u8 *cursor;
};
DECOMP_ASSERT(sizeof(ReplicatorData) == 0xc, "ReplicatorData ABI");
DECOMP_ASSERT(offsetof(ReplicatorData, cursor) == 8, "ReplicatorData cursor offset");
struct WORLDINFO_s;
struct ePeerLeftReason {};
struct eHostVisibility {
    i32 value;
};

struct NuFileDeviceAndroidOBBType {
    enum T { NONE = 0, MAIN = 1, PATCH = 2 };
};
struct AndroidOBBUtils {
    static char ms_packageName[3][512];
    static bool ms_initializedPackage[3];
    static bool ms_initializedPackageIsAsset[3];
    static void InitPackagePaths();
    static i32 LookupPackagePath(char *, NuFileDeviceAndroidOBBType::T);
    static i32 OpenFile(char const *);
};
struct FtpFile {
    NetAddress address;
    i32 direction;
    i32 accepted;
    char name[0x80];
    i32 size;
    void *buffer;
    i32 capacity;
    i32 transferred;
    i32 message_swap_endianness;
    struct TransferReference {
        u8 reserved_00[0x4b0];
        u32 reference_count;
    };
    TransferReference *message_data;
    u32 message_read_offset;
    u32 message_write_offset;
    void *transfer;
    i32 Accept();
    i32 Accept(i32);
    i32 Accept(i32, void *);
    void Init(i32, char const *, i32, NetAddress const &, void *, i32);
    void RecvData(NetMessage &);
    void SendData();
    void Term();
    void Update();
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(FtpFile) == 0xb0, "FtpFile 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(FtpFile, name) == 0xc, "FtpFile name 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(FtpFile, message_data) == 0xa0, "FtpFile message data 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(FtpFile, transfer) == 0xac, "FtpFile transfer 32-bit offset");
struct NetReplicator {
    static i16 smNextId;

    NetReplicator *next;
    NetReplicator *previous;
    u16 id;
    u16 replication_group;
    u16 minimum_interval;
    u16 maximum_interval;
    u16 data_size;
    u16 message_size;

    NetReplicator(i32, float, float);
    virtual bool AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) = 0;
    virtual i32 SerialiseObject(EdStream &, NetPeer *, EdClass const *, void *, ReplicatorData &, i16 *);
    virtual i32 DoPrediction(EdClass const *, void *, ReplicatorData &, i32);
};
struct NetChangedReplicator : NetReplicator {
    static i16 mTableInited;
    static i16 mCrc32Table[256];

    bool AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) override;
    void CheckSum(unsigned char const *, u32, u32 &) const;
    void CheckSumObject(EdClass const *, void const *, u32 &) const;
    void InitTable();
};
struct NetConstReplicator : NetReplicator {
    bool AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) override;
};
struct NetMessage {
    struct MessageData {
        u8 bytes[0x4b0];
        u32 references;
    };
    static MessageData sm_poolMessageData[512];
    i32 swap_endianness;
    MessageData *data;
    u32 read_offset;
    u32 write_offset;

    NetMessage() : swap_endianness(1), data(NULL), read_offset(0x20), write_offset(0x20) {
        for (i32 i = 0; i < 512; ++i) {
            if (sm_poolMessageData[i].references == 0) {
                data = &sm_poolMessageData[i];
                data->references = 1;
                break;
            }
        }
    }
    NetMessage(NetMessage const &other)
        : swap_endianness(other.swap_endianness), data(other.data), read_offset(other.read_offset),
          write_offset(other.write_offset) {
        if (data != NULL) {
            ++data->references;
        } else {
            RaiseError();
        }
    }
    ~NetMessage() {
        if (data != NULL) {
            if (data->references > 1) {
                --data->references;
            } else {
                data->references = 0;
            }
        }
    }
    void Read8(u8 &value) {
        if (data != NULL) {
            value = data->bytes[read_offset++];
        }
    }
    void Read16(i16 &value) {
        if (data != NULL) {
            memmove(&value, data->bytes + read_offset, 2);
            if (swap_endianness) {
                EdFileSwapEndianess16(&value);
            }
            read_offset += 2;
        }
    }
    void Read32(i32 &value) {
        if (data != NULL) {
            memmove(&value, data->bytes + read_offset, 4);
            if (swap_endianness) {
                EdFileSwapEndianess32(&value);
            }
            read_offset += 4;
        }
    }
    void ReadFloat(float &value) {
        if (data != NULL) {
            memmove(&value, data->bytes + read_offset, 4);
            if (swap_endianness) {
                EdFileSwapEndianess32(&value);
            }
            read_offset += 4;
        }
    }
    void Read(void *value, u32 size) {
        if (data != NULL) {
            memmove(value, data->bytes + read_offset, size);
            read_offset += size;
        }
    }
    void Write8(u8 value) {
        if (data != NULL) {
            data->bytes[write_offset++] = value;
        }
    }
    void Write16(i16 value) {
        if (data != NULL) {
            memcpy(data->bytes + write_offset, &value, 2);
            if (swap_endianness) {
                EdFileSwapEndianess16(data->bytes + write_offset);
            }
            write_offset += 2;
        }
    }
    void Write32(i32 value) {
        if (data != NULL) {
            memcpy(data->bytes + write_offset, &value, 4);
            if (swap_endianness) {
                EdFileSwapEndianess32(data->bytes + write_offset);
            }
            write_offset += 4;
        }
    }
    void Write(void const *value, u32 size) {
        if (data != NULL) {
            memmove(data->bytes + write_offset, value, size);
            write_offset += size;
        }
    }
    void DebugPrint() const;
    static void RaiseError();
};
struct NetListenerInterface {
    virtual ~NetListenerInterface() {
    }
    virtual void Receive(NetMessage, unsigned char, NetPeer const &) {
    }
    virtual i32 PeerRequest(NetPeer const &) {
        return 1;
    }
    virtual void PeerJoined(NetPeer const &) {
    }
    virtual void PeerLeft(NetPeer const &, ePeerLeftReason) {
    }
    virtual void PeerDead(NetPeer const &) {
    }
    virtual void FtpUpload(FtpFile *) {
    }
    virtual void FtpDownload(FtpFile *) {
    }
    virtual void FtpComplete(FtpFile *, i32) {
    }
    virtual i32 NosAcquire(NetworkObject *, NetPeer const &) {
        return 1;
    }
    virtual void NosAdopted(NetworkObject *, NetPeer const &) {
    }
};
struct NetFtpManager : NetListenerInterface {
    FtpFile files[32];
    i32 field_1604;
    i32 Abort(char const *, NetAddress const &, i32, i32);
    FtpFile *FindTransfer(char const *, NetAddress const &, i32);
    FtpFile const *FindTransfer(char const *, NetAddress const &, i32) const;
    i32 Get(char const *, void *, i32, NetAddress const &);
    FtpFile const *GetTransfers() const;
    void Init();
    NetFtpManager();
    virtual void PeerLeft(NetAddress const &, ePeerLeftReason);
    virtual void Receive(NetMessage, unsigned char, NetAddress const &);
    void Reset();
    i32 Send(char const *, void const *, i32, NetAddress const &);
    void Term();
    void Update();
    ~NetFtpManager() override;
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NetFtpManager) == 0x1608, "NetFtpManager 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetFtpManager, files) == 4, "NetFtpManager files 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetFtpManager, field_1604) == 0x1604,
              "NetFtpManager trailing field 32-bit offset");
struct NetSession {
    u8 reserved_00[4];
    i32 status;
    u8 reserved_08[0x34 - 8];
    u32 error;
    u8 reserved_38[0x84 - 0x38];
    NetPeer *local_peer;
};
extern NetSession *theSession;
DECOMP_ASSERT(sizeof(NetMessage) == 0x10, "NetMessage ABI");
DECOMP_ASSERT(offsetof(NetMessage, data) == 4, "NetMessage data offset");
DECOMP_ASSERT(offsetof(NetMessage, read_offset) == 8, "NetMessage read cursor offset");
DECOMP_ASSERT(offsetof(NetMessage, write_offset) == 12, "NetMessage write cursor offset");
static_assert(sizeof(NetMessage::MessageData) == 0x4b4, "NetMessage pool entry size");
static_assert(offsetof(NetMessage::MessageData, references) == 0x4b0, "NetMessage pool reference offset");
struct NetPredictor : NetReplicator {
    struct PredictorData {
        f32 values[3];
    };
    struct PredictorTime {
        i32 sample_count;
        f32 values[3];
        f32 factors[3];
    };

    f32 maximum_prediction_error;
    f32 maximum_sample_delta;
    f32 minimum_value;
    f32 maximum_value;

    bool AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) override;
    virtual i32 CheckPredictionError(EdClass const *, void *, float *, float *, i32);
    i32 DoPrediction(EdClass const *, void *, ReplicatorData &, NetPredictor::PredictorTime *, i32);
    i32 DoPrediction(EdClass const *, void *, ReplicatorData &, i32) override;
    i32 SerialiseObject(EdStream &, NetPeer *, EdClass const *, void *, ReplicatorData &, NetPredictor::PredictorTime *,
                        i16 *);
    i32 SerialiseObject(EdStream &, NetPeer *, EdClass const *, void *, ReplicatorData &, i16 *) override;
    virtual void StoreSampleData(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **,
                                 float *, i32);
    virtual void PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **,
                              float *, i32) = 0;
};
struct NetPredictor2 : NetPredictor {
    void PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **, float *,
                      i32) override;
};
struct NetPredictor3 : NetPredictor {
    void PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **, float *,
                      i32) override;
};
struct NetRotator2 : NetPredictor {
    void PredictValue(EdClass const *, void *, NetPredictor::PredictorTime *, NetPredictor::PredictorData **, float *,
                      i32) override;
};
static_assert(sizeof(void *) != 4 || sizeof(NetPredictor::PredictorData) == 0xc,
              "NetPredictor::PredictorData 32-bit size");
static_assert(sizeof(void *) != 4 || sizeof(NetPredictor::PredictorTime) == 0x1c,
              "NetPredictor::PredictorTime 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NetPredictor) == 0x28, "NetPredictor 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetPeer, time_offset) == 0x29c, "NetPeer::time_offset 32-bit offset");
DECOMP_ASSERT(offsetof(NetPeer, stats) == 0x10, "NetPeer stats offset");
struct NetSimpleReplicator : NetReplicator {
    bool AllowPush(EdClass const *, void const *, ReplicatorData &, i32, i32) override;
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NetReplicator) == 0x18, "NetReplicator 32-bit size");
DECOMP_ASSERT(offsetof(NetReplicator, next) == 4, "NetReplicator next offset");
DECOMP_ASSERT(offsetof(NetReplicator, data_size) == 0x14, "NetReplicator data size offset");
struct NetListenerBinding {
    NetListenerBinding *next;
    NetListenerBinding *previous;
    NetListenerInterface *listener;
    u8 channel;
    char name[0x23];
    NetStats stats;

    NetListenerBinding(NetListenerInterface *, unsigned char, char *);
    void operator=(NetListenerBinding const &);
    bool operator==(NetListenerBinding const &);
};
struct NetListenerList {
    NetListenerBinding *first;

    NetListenerBinding *Find(NetListenerBinding *);
};
DECOMP_ASSERT(sizeof(NetListenerBinding) == 0x250, "NetListenerBinding size");
DECOMP_ASSERT(offsetof(NetListenerBinding, listener) == 8, "NetListenerBinding listener offset");
DECOMP_ASSERT(offsetof(NetListenerBinding, channel) == 0xc, "NetListenerBinding channel offset");
DECOMP_ASSERT(offsetof(NetListenerBinding, stats) == 0x30, "NetListenerBinding stats offset");
struct NetTransporter {
    NetListenerBinding *first_listener;
    NetListenerBinding *last_listener;
    i32 listener_count;
    NetTransporter() : first_listener(NULL), last_listener(NULL), listener_count(0) {
    }
    virtual void Send(NetMessage, unsigned char, NetPeer &) = 0;
    virtual void ReliableSend(NetMessage, unsigned char, NetPeer &, char const *, u32) = 0;
    virtual void Broadcast(NetMessage, unsigned char) = 0;
    virtual void ReliableBroadcast(NetMessage, unsigned char) = 0;
    virtual void AddListener(NetListenerInterface *, unsigned char, char *);
    void Distribute(NetMessage const &, unsigned char, NetPeer const &) const;
    void FtpComplete(FtpFile *, i32) const;
    void FtpDownload(FtpFile *) const;
    void FtpUpload(FtpFile *) const;
    i32 NosAcquire(NetworkObject *, NetPeer const &) const;
    void NosAdopted(NetworkObject *, NetPeer const &) const;
    void PeerDead(NetPeer const &) const;
    void PeerJoined(NetPeer const &) const;
    void PeerLeft(NetPeer const &, ePeerLeftReason) const;
    i32 PeerRequest(NetPeer const &) const;
    virtual void RemoveListener(NetListenerInterface *, unsigned char);
    virtual ~NetTransporter() {
    }
    void StatsReceiveMessage(NetMessage, unsigned char);
    void StatsSendMessage(NetMessage, unsigned char);
    void StatsUpdate();
};
struct NetworkObject {
    u16 flags;
    i16 id;
    u32 reserved_04;
    NetPeer const *owner;
    void *object;
    EdClass *object_class;
    void *replicator_data;

    void Destroy();
    void Initialise(i32, void *, EdClass *, NetPeer const &, i32);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NetworkObject) == 0x18, "NetworkObject 32-bit size");
struct NetworkObjectManager : NetListenerInterface, EdObjectNotifier {
    // The manager reset routine is an intentional no-op in the original.
    struct NetPeerPush {
        NetPeer const *peer;
        NetMessage *message;
        NetMessage *reliable_message;
        i32 stage;
        i32 field_10;

        void FlushMessages();
        NetMessage *GetMessage(i32);
        NetMessage *GetReliableMessage(i32);
        void NextStage();
        void Stop();
        void Sync();
    };
    i32 Acquire(i32);
    void AddToLocalObjectList(NetworkObject *);
    void BindFilter(NOSFilter *, EdClass const *);
    void BindReplicator(NetReplicator *, EdClass const *);
    void CalcReplicatorDataSize(NetReplicator *, EdClass const *, i32 &, i32 &);
    void ChangeContext(NOSContext &);
    void ConstructObject(NetworkObject *, NetworkObjectManager::NetPeerPush *);
    void ContinuityBreak(i32, float);
    NetworkObject *FindNetworkObject(i32);
    NetworkObject *FindNetworkObject(void *);
    struct PendingObject {
        u32 field_00;
        u32 field_04;
        NetworkObject *object;
    };

    PendingObject *FindPendingObject(NetworkObject *);
    void FlushObjects(i32);
    i32 GetGuid(void *);
    i32 GetNextGuid();
    void *GetObject(i32);
    i32 GetPeerStatus();
    void ImportObjects();
    void Init();
    void InitClassStats();
    i32 IsLocal(i32);
    i32 IsPeerReady(NetPeer const &) const;
    i32 IsPeerStarted(NetPeer const &) const;
    NetworkObjectManager();
    void NotifyCreateObject(void *, EdClass *, void *, i32, i32, i32) override;
    void NotifyDestroyObject(void *, EdClass *, i32, i32) override;
    i32 ObjectCall(void *, i32, NetMessage, NetPeer const *);
    i32 ObjectOtherCall(void *, i32, NetMessage);
    i32 ObjectOwnerCall(void *, i32, NetMessage);
    NetPeer const *Owner(i32);
    void PeerJoined(NetPeer const &) override;
    void PeerLeft(NetPeer const &, ePeerLeftReason) override;
    i32 Push(NetworkObject const *, NetReplicator *, ReplicatorData &, NetworkObjectManager::NetPeerPush *);
    i32 PushObject(NetworkObject *, NetworkObjectManager::NetPeerPush *, i32);
    void Receive(NetMessage, unsigned char, NetPeer const &) override;
    void ReceiveAcquireMessage(NetMessage &, NetPeer const &);
    void ReceiveAcquiredMessage(NetMessage &, NetPeer const &);
    void ReceiveAdoptedMessage(NetMessage &, NetPeer const &);
    void ReceiveConstructorMessage(NetMessage &, NetPeer const &);
    void ReceiveContinuityBreak(NetMessage &, NetPeer const &);
    void ReceiveObjectCallMessage(NetMessage &, NetPeer const &);
    void ReceiveReleaseMessage(NetMessage &, NetPeer const &);
    void ReceiveRemoteCallMessage(NetMessage &, NetPeer const &);
    void ReceiveReplicaMessage(NetMessage &, NetPeer const &);
    void ReceiveStartMessage(NetMessage &, NetPeer const &);
    void ReceiveStatusMessage(NetMessage &, NetPeer const &);
    void ReceiveStopMessage(NetMessage &, NetPeer const &);
    void Recover(NetworkObject *);
    i32 RegisterObject(void *, EdClass *, i32);
    i32 RegisterObjectCall(void (*)(void *, NetMessage &), i32);
    i32 RegisterRemoteCall(void (*)(NetMessage &), i32);
    i32 ReleaseObject(void *, EdClass *, i32);
    i32 RemoteCall(i32, NetMessage, NetPeer const *);
    void RemoveFromLocalObjectList(NetworkObject *);
    void RemovePendingObject(NetworkObject *);
    void Reset();
    void SendAcquireMessage(NetworkObject *);
    void SendAcquiredMessage(i16, NetPeer const &);
    void SendAdoptedMessage(i16);
    i32 SendPushMessage(NetMessage *, NetworkObjectManager::NetPeerPush const *, i32);
    void Start(NOSContext const &);
    PendingObject *StealPendingObject();
    void Stop();
    void Term();
    void Update();
    void UpdateLocalObjectList();
    ~NetworkObjectManager() override;

    i32 field_08;
    i32 active;
    NOSContext context;
    NetPeer const *guid_peers[2];
    i32 guid_group;
    i32 next_guid;
    NetworkObject objects[2048];
    i32 local_object_count;
    NetworkObject *local_objects[1024];
    PendingObject pending_objects[32];
    struct ReplicatorList {
        NetReplicator *head;
        NetReplicator *tail;
        i32 count;
    } replicators[64];
    i32 replicator_data_sizes[64];
    NOSFilter *filters[64];
    struct RegisteredCall {
        i32 type;
        i32 flags;
        void *callback;
    } registered_calls[32];
    i32 registered_call_count;
    NetPeerPush peer_push[8];
    NetPeerPush default_push;
    NetStats *class_stats[32];
    u8 field_d96c;
};
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetSession, local_peer) == 0x84, "NetSession local peer offset");
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(NetworkObjectManager::NetPeerPush) == 0x14,
              "NetworkObjectManager::NetPeerPush 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, active) == 0xc,
              "NetworkObjectManager::active 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, guid_peers) == 0x20,
              "NetworkObjectManager::guid_peers 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, peer_push) == 0xd838,
              "NetworkObjectManager::peer_push 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, default_push) == 0xd8d8,
              "NetworkObjectManager::default_push 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, class_stats) == 0xd8ec,
              "NetworkObjectManager::class_stats 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, field_d96c) == 0xd96c,
              "NetworkObjectManager::field_d96c 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, objects) == 0x30,
              "NetworkObjectManager::objects 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, local_object_count) == 0xc030,
              "NetworkObjectManager::local_object_count 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, local_objects) == 0xc034,
              "NetworkObjectManager::local_objects 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, pending_objects) == 0xd034,
              "NetworkObjectManager::pending_objects 32-bit offset");
DECOMP_ASSERT(offsetof(NetworkObjectManager, replicators) == 0xd1b4, "NetworkObjectManager replicator lists");
DECOMP_ASSERT(offsetof(NetworkObjectManager, replicator_data_sizes) == 0xd4b4,
              "NetworkObjectManager replicator data sizes");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, filters) == 0xd5b4,
              "NetworkObjectManager::filters 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, registered_calls) == 0xd6b4,
              "NetworkObjectManager::registered_calls 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(NetworkObjectManager, registered_call_count) == 0xd834,
              "NetworkObjectManager::registered_call_count 32-bit offset");
DECOMP_ASSERT(offsetof(NetworkObjectManager::RegisteredCall, flags) == 4, "RegisteredCall flags offset");
DECOMP_ASSERT(offsetof(NetworkObjectManager::RegisteredCall, callback) == 8, "RegisteredCall callback offset");
extern NetworkObjectManager *theNos;
struct TouchHacks {
    static bool TouchControlsActive;

    struct TintStack {
        float ambient[3];

        TintStack();
        ~TintStack();
    };
    static bool AiPlayerTakeDamageOnKillRescue(GameObject_s &);
    static VuVec CalculateJumpVelToHitPoint(GameObject_s &, VuVec const &);
    static VuVec CalculateJumpVelToHitPointDblJump(GameObject_s &, VuVec const &);
    static VuVec CalculateXZVelForArcToHitPoint(VuVec const &, VuVec const &, float, float);
    static i32 CanBlowupBeBlownUp(GIZMOBLOWUP_s &, i32);
    static bool CanForceTargetObj(GameObject_s &, GameObject_s &);
    static bool CanJump(GameObject_s &);
    static bool CanJumpToPoint(GameObject_s &, AIPATHNODE_s const &);
    static bool CanJumpToPoint(GameObject_s &, VuVec const &);
    static bool CanLunge(GameObject_s &);
    static bool CanPoo(GameObject_s &);
    static bool CanShoot(GameObject_s &);
    static bool CanSlam(GameObject_s &);
    static bool CanTagTo(GameObject_s &, GameObject_s &);
    static bool CanTagVehicle(GameObject_s &, GameObject_s &);
    static bool CanThrowBountyBomb(GameObject_s &);
    static bool CanToggleTo(GameObject_s &, i32);
    static bool CanUseBuildIt(GameObject_s &);
    static bool CanUseGizForce(GameObject_s &);
    static bool CanUseGizForce(GameObject_s &, GIZFORCE_s &);
    static bool CanUseHatMachine(GameObject_s &);
    static bool CanUseLever(GameObject_s &);
    static bool CanUseTeleport(GameObject_s &);
    static bool CanUseVehicleSmartBomb(GameObject_s &);
    static bool CanUseZipup(GameObject_s &);
    static bool CheckForAboutToRunIntoKillTerrain(GameObject_s &, float);
    static bool CheckForAboutToRunOffAnEdge(GameObject_s &, float);
    static bool CheckJumpForLandingSpot(GameObject_s &, float);
    static void CleanupAllMechObjectInterfaces(WORLDINFO_s *);
    static MechObjectInterface *FindBombTarget(GameObject_s &);
    static nucolour3_s *GetFlashColour();
    static float GetIncomingPartRange();
    static i32 GetLoseStudsDieValue();
    static i32 GetLoseStudsFallValue();
    static bool InParty(GameObject_s &);
    static void PlaySmartBombBuildupEffects(GameObject_s &, float, float);
    static bool ShouldAutoGrabDragBomb(GameObject_s &);
    static bool ShouldBlock(GameObject_s &);
    static i32 ShouldDeflectBolt(GameObject_s &, BOLT_s &);
    static bool ShouldFlash(float);
    static bool ShouldKeepWeaponOut(GameObject_s &);
    static bool ShouldPutWeaponAway(GameObject_s &);
    static bool SolveRoot(float, float, float, float &, float &);
    static void TriggerVehicleSmartBomb(GameObject_s &);
};
struct V2SessionManager {
    static V2SessionManager *mpSessionManager;
    static char mLogFilename[32];
    static i32 mLogHandle;

    i32 field_04;
    u8 reserved_08[0x14 - 0x8];
    char name[0x20];
    i32 field_34;
    i32 host_visibility;
    i32 num_local_players;
    i32 field_40;
    i32 fields_44[9];
    i32 field_68;
    i32 field_6c;
    u8 reserved_70[0x74 - 0x70];
    i32 field_74;
    u8 field_78;
    u8 reserved_79[3];
    i32 field_7c;
    i32 field_80;
    NetPeer *local_peer;
    i32 field_88;
    i32 field_8c;
    i32 field_90;
    NetPeer *first_peer;
    NetPeer *last_peer;
    i32 peer_count;
    NetStats stats;

    virtual void Init(char *) {
    }
    virtual void Start() {
    }
    virtual void Update();
    virtual void Leave() {
    }
    virtual void Close() {
    }
    virtual i32 SignIn() {
        return 0;
    }
    virtual void RemovePeer(NetPeer *, ePeerLeftReason);
    virtual void Render() {
    }
    virtual i32 IsChatRestricted() {
        return 0;
    }
    virtual i32 IsAgeRestricted() {
        return 0;
    }
    virtual void GetLocalAddress(NetAddress *) {
    }
    virtual i32 IsSearchingForHosts() {
        return 0;
    }
    virtual void Join(NetHost *) {
    }
    virtual void Host(eHostVisibility) {
    }
    virtual void SearchForHosts(i32, i32 *) {
    }
    virtual void SetHostVisibility(eHostVisibility visibility) {
        host_visibility = visibility.value;
    }
    virtual void SetNumLocalPlayers(i32 count) {
        num_local_players = count;
    }
    virtual void RemoveRemotePlayer(NetPeer &) {
    }
    virtual void AcceptInvitation() {
    }
    virtual void SendInvitation(char *) {
    }
    virtual void ViewInvitations() {
    }
    virtual i32 VerifyStrings(char **, char **, i32, char *);

    void Log(char *, ...);
    void RemoveAllPeers(ePeerLeftReason);
    void Reset();
    void SetHostGameData(i32 *, i32);
    V2SessionManager(char *);
};
DECOMP_ASSERT(sizeof(void *) != 4 || sizeof(V2SessionManager) == 0x2c0, "V2SessionManager 32-bit size");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(V2SessionManager, local_peer) == 0x84,
              "V2SessionManager local peer 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(V2SessionManager, first_peer) == 0x94,
              "V2SessionManager first peer 32-bit offset");
DECOMP_ASSERT(sizeof(void *) != 4 || offsetof(V2SessionManager, stats) == 0xa0, "V2SessionManager stats 32-bit offset");
struct VirtualStackAllocator {
    u8 owns_memory;
    u8 *cursor;
    u8 *end;
    u8 *base;

    VirtualStackAllocator();
    VirtualStackAllocator(VirtualStackAllocator &, u32);
    VirtualStackAllocator(i32);
    VirtualStackAllocator(void *, u32);
    void setExternalMemoryPool(void *, u32);
    ~VirtualStackAllocator();
};

#endif // GAMELIB_UTIL_TYPES_H
